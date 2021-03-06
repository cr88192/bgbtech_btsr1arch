#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TGVLZ_NOMAIN
#include "tgvlz1.c"

#if 0
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

typedef unsigned short word;

typedef struct
{
	byte	identification[4];		// should be IWAD
	u32		numlumps;				//04: Number of lumps in the directory.
	u32		infotableofs;			//08: Offset of Directory.
 	u32		hashoffs;				//0C: Offset of Directory Hash.
 	u32		bmpoffs;				//10: Offset of Allocation Bitmap.
 	u32		bmpsize;				//14: Bitmap Size
 	u32		resv4;					//18: Reserved
 	u32		resv5;					//1C: Reserved
} wadinfo_t;


typedef struct
{
	u32		filepos;
	u32		size;
	byte	name[8];
} filelump_t;

typedef struct
{
	u32		filepos;
	u32		csize;
	u32		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	u16		chn;		//Chain (ExWAD)
	byte	name[16];
} wad2lump_t;

typedef struct
{
	u32		filepos;	//00: offset
	u32		csize;		//04: compressed size
	u32		dsize;		//08: uncompressed size
	byte	ety;		//0C: Entry Type
	byte	cmp;		//0D: Compression
	u16		chn;		//0E: Chain (ExWAD)
 	u16		dirid;		//10: Directory ID / Parent Directory (DirEnt Index)
 	u16		dirnext;	//12: Next in Directory (DirEnt Index)
 	u16		uid;		//14: UID
 	u16		gid;		//16: GID
 	u16		mode;		//18: Mode Flags (---Srwxrwxrwx)
 	u16		hibits;		//1A: Some extra bits
 	u32		mtime;		//1C: File timestamp
 	byte	name[32];	//20: name
} wad4lump_t;


FILE		*wad_fd;
// filelump_t	wad_dir[2048];
wad4lump_t	wad_dir[16384];
wadinfo_t	wad_head;
int 		wad_n_lumps;
int			wad_types[256];
int			wad_ntypes;
int			wad_ldirid;
int			wad_dirid;
int			wad_fragrov;

u16			wad_hash[8192];

byte		*wad_data;
int			wad_rover;

byte		*iwad_data;
filelump_t	*iwad_dir;
wadinfo_t	*iwad_head;
int			iwad_size;

int		wad_mincsz;
int		wad_maxcsz;
int 	wad_cmp[16];

void w_strupr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = toupper(*s++); }
	for(; i<n; i++)
		*t++=0;
}

void w_strlwr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = tolower(*s++); }
	for(; i<n; i++)
		*t++=0;
}

int WadTypeForTag(int tag)
{
	int i, j, k;

	if(!tag)
		return(0);

	for(i=0; i<(0x80-0x20); i++)
	{
		j=i+0x20;
		if(wad_types[j]==tag)
			return(j);
		if(!wad_types[j])
		{
			wad_types[j]=tag;
			wad_ntypes=i+1;
			return(j);
		}
	}
	
	return(0);
}

int AddWadLump2(char *name, void *buf, int csz, int dsz,
	int cmp, int tag, int pfx)
{
	byte *obuf;
	char tn[33];
	int osz, ety;
	int n;

//	ety=WadTypeForTag(tag);
	ety=1;

	if(csz!=dsz)
	{
		if(csz<wad_mincsz)
			wad_mincsz=csz;
		if(csz>wad_maxcsz)
			wad_maxcsz=csz;
	}

//	w_strupr_n(tn, name, 16);
//	w_strlwr_n(tn, name, 32);
	memset(tn, 0, 33);
	strncpy(tn, name, 32);
	
	n=wad_n_lumps++;
	memset(wad_dir+n, 0, sizeof(wad4lump_t));

	wad_rover=(wad_rover+63)&(~63);

	wad_dir[n].filepos=wad_rover/64;
	wad_dir[n].csize=csz;
	wad_dir[n].dsize=dsz;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=cmp;
	wad_dir[n].chn=0;
	wad_dir[n].dirid=pfx;
	memcpy(wad_dir[n].name, tn, 32);
	
	memcpy(wad_data+wad_rover, buf, csz);
	wad_rover+=csz;
	wad_rover=(wad_rover+63)&(~63);
	
	return(n);
}

int AddWadLumpFrag(char *name, byte *buf, int isz, int tag, int pfx)
{
	char tn[64];
	int fra[64];
	byte *cs, *cse;
	int nfra;
	int i, j, k, n;
	
	cs=buf; cse=buf+isz;
	nfra=(isz+131071)/131072;
	for(i=0; i<nfra; i++)
	{
		j=wad_fragrov++;
		sprintf(tn, "$FRAG:%08X", j);
		j=cse-cs;
		if(j>131072)
			j=131072;
		k=AddWadLump(tn, cs, j, 0, 1);
		fra[i]=k;
		cs+=j;
	}

	wad_cmp[1]++;
	n=AddWadLump2(name, (void *)fra, nfra*4, isz, 1, tag, pfx);
	return(n);
}

byte		wad_packbuf[131072];
wad4lump_t	wad_packlump[256];
int 		wad_packpos=0;
int 		wad_n_packlump=0;

int FlushPack()
{
	char tn[64];
	int i, j, k, n, n1;

	if(wad_n_packlump<=0)
		return(0);

	j=wad_fragrov++;
	sprintf(tn, "$PACK:%08X", j);
	n=AddWadLump(tn, wad_packbuf, wad_packpos, 0, 1);
	
	for(i=0; i<wad_n_packlump; i++)
	{
		n1=wad_n_lumps++;
		memcpy(wad_dir+n1, wad_packlump+i, sizeof(wad4lump_t));
		wad_dir[n1].filepos|=(n<<16);
		wad_cmp[2]++;
	}
	
	wad_packpos=0;
	wad_n_packlump=0;
	return(n);
}

int AddWadLumpPack(char *name, byte *buf, int isz, int tag, int pfx)
{
	byte *obuf;
	char tn[33];
	int osz, ety;
	int n;

	if(((wad_packpos+isz)>=131072) || ((wad_n_packlump+1)>=256))
	{
		FlushPack();
	}

	memset(tn, 0, 33);
	strncpy(tn, name, 32);
	
	n=wad_n_packlump++;
	memset(wad_packlump+n, 0, sizeof(wad4lump_t));

	wad_packpos=(wad_packpos+63)&(~63);
	wad_packlump[n].filepos=wad_packpos/64;
	wad_packlump[n].csize=isz;
	wad_packlump[n].dsize=isz;
	wad_packlump[n].ety=1;
	wad_packlump[n].cmp=2;
	wad_packlump[n].chn=0;
	wad_packlump[n].dirid=pfx;
	memcpy(wad_packlump[n].name, tn, 32);

	memcpy(wad_packbuf+wad_packpos, buf, isz);
	wad_packpos+=isz;
//	wad_packpos=(wad_packpos+63)&(~63);
	
	return(n);
}

int AddWadLump(char *name, byte *buf, int isz, int tag, int pfx)
{
	TgvLz_Context *ctx1, *ctx2;
	byte *ibuf;
	byte *obuf;
	byte *obuf1;
	byte *obuf2;
	int osz, osz1, osz2, n, cmp;

	if((isz>131072) && (tag>0))
	{
		n=AddWadLumpFrag(name, buf, isz, tag, pfx);
		return(n);
	}

	if((isz<32768) && (tag>0))
	{
		n=AddWadLumpPack(name, buf, isz, tag, pfx);
		return(n);
	}

	ctx1=TgvLz_CreateContext();
	ctx2=TgvLz_CreateContextLZ4();

	ibuf=malloc(isz+24);
	memset(ibuf, 0, isz+24);
	memcpy(ibuf, buf, isz);
	
	obuf1=malloc(isz*2+1024);
	obuf2=malloc(isz*2+1024);
//	osz=TgvLz_DoEncode(ctx, ibuf, obuf, isz);
	osz1=TgvLz_DoEncodeSafe(ctx1, ibuf, obuf1, isz);
	osz2=TgvLz_DoEncodeSafe(ctx2, ibuf, obuf2, isz);
	TgvLz_DestroyContext(ctx1);
	TgvLz_DestroyContext(ctx2);
	
	if((osz1<=osz2) && (osz1>0))
	{
		obuf=obuf1;
		osz=osz1;
		cmp=3;
	}else if(osz2>0)
	{
		obuf=obuf2;
		osz=osz2;		
		cmp=4;
	}else
	{
		obuf=NULL;
		osz=-1;
		cmp=-1;
	}
	
	if(((1.2*osz)<isz) && (osz>64))
//	if(((1.2*osz)<isz) && (osz>32))
//	if(((1.2*osz)<isz) && (osz>16))
	{
		wad_cmp[cmp]++;
		n=AddWadLump2(name, obuf, osz, isz, cmp, tag, pfx);
	}else
	{
		wad_cmp[0]++;
		n=AddWadLump2(name, ibuf, isz, isz, 0, tag, pfx);
	}
	
	free(ibuf);
	free(obuf1);
	free(obuf2);
	
	return(n);
}

int DigitBase32(int v)
{
	if((v>=0) && (v<=9))
		return('0'+v);
	return('a'+(v-10));
}

int GetLumpNumForName(char *name)
{
	int i;
	
	for(i=0; i<wad_n_lumps; i++)
	{
//		if(!strncmp(wad_dir[i].name, name, 16))
		if(!strncmp(wad_dir[i].name, name, 32))
			return(i);
	}
	return(-1);
}

int GetDirLumpNumForName(char *name, int pfx)
{
	int i;
	
	for(i=0; i<wad_n_lumps; i++)
	{
//		if(wad_dir[i].cmp!=1)
		if(wad_dir[i].ety!=2)
			continue;
		if(wad_dir[i].dirid!=pfx)
			continue;
//		if(!strncmp(wad_dir[i].name, name, 16))
		if(!strncmp(wad_dir[i].name, name, 32))
			return(i);
	}
	return(-1);
}

int WadBuildPfxName(char *tn, char *name, int pfx)
{
	char *s, *t;

//	memset(tn, 0, 16);
	memset(tn, 0, 32);
	s=name; t=tn;
	while(*s)
		*t++=*s++;
	*t++=0;

	return(0);

#if 0
	if(!pfx)
	{
//		w_strlwr_n(tn, name, 16);
		s=name; t=tn;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;

		return(0);
	}

	if(pfx<32)
	{
		tn[0]=DigitBase32(pfx&31);
		tn[1]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+2;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	if(pfx<1024)
	{
		tn[0]=DigitBase32((pfx>> 5)&31);
		tn[1]=DigitBase32((pfx>> 0)&31);
		tn[2]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+3;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	tn[0]=DigitBase32((pfx>>10)&31);
	tn[1]=DigitBase32((pfx>> 5)&31);
	tn[2]=DigitBase32((pfx>> 0)&31);
	tn[3]='|';
//	w_strlwr_n(tn+4, name, 12);
	s=name; t=tn+4;
	while(*s && (*s!='.'))
		*t++=tolower(*s++);
	*t++=0;
	return(0);
#endif
}

int GetWadPathId(char *name, int pfx, int tag)
{
	char tn[43];
	int i, n, id, ety;

//	WadBuildPfxName(tn, name, pfx);
	WadBuildPfxName(tn, name, 0);
	
	i=GetDirLumpNumForName(tn, pfx);
	if(i>=0)
	{
		id=(wad_dir[i].filepos)&32767;
//		if((id<0) || (id>=wad_dirid))
//			{ __debugbreak(); }
		return(id);
	}
	
//	if(wad_dirid!=(wad_ldirid+1))
//	{
//		__debugbreak();
//	}
	
	ety=WadTypeForTag(tag);

//	id=wad_dirid++;
//	if(id!=(wad_ldirid+1))
//		{ __debugbreak(); }
//	wad_ldirid=id;

	n=wad_n_lumps++;
	memset(wad_dir+n, 0, sizeof(wad4lump_t));

	id=n;

	wad_dir[n].filepos=id;
	wad_dir[n].csize=0;
	wad_dir[n].dsize=0;
	wad_dir[n].ety=2;
	wad_dir[n].cmp=0;
	wad_dir[n].chn=0;
	wad_dir[n].dirid=pfx;
	memcpy(wad_dir[n].name, tn, 32);

//	printf("new dir %04X\n", id);

	return(id);
}

int AddWadLumpPathI(char *name, byte *buf, int isz, int pfx)
{
	char tb[256];
	int tag, pfx1;
	char *s, *t;
	int i;

	tag=FccTagForName(name);
	
	s=name; t=tb;
	while(*s && (*s!='/'))
		*t++=*s++;
	*t++=0;

	if(!(*s))
	{
		if(	!strcmp(name, ".") ||
			!strcmp(name, ".."))
				return(0);
//		WadBuildPfxName(tb, name, pfx);
		WadBuildPfxName(tb, name, 0);
		return(AddWadLump(tb, buf, isz, tag, pfx));
	}

	tag=FccTagForName(tb);
	pfx1=GetWadPathId(tb, pfx, tag);
	i=AddWadLumpPathI(s+1, buf, isz, pfx1);
	return(i);
}

int AddWadLumpPath(char *name, byte *buf, int isz)
{
	char tb[256];
	int tag, pfx;
	char *s, *t;
	int i;

	tag=FccTagForName(name);
	
	s=name; t=tb;
	while(*s && (*s!='/'))
		*t++=*s++;
	*t++=0;

	if(!(*s))
	{
		s=name; t=tb;
//		while(*s && (*s!='.'))
		while(*s)
			*t++=*s++;
		*t++=0;
		return(AddWadLump(tb, buf, isz, tag, 0));
	}

	tag=FccTagForName(tb);
	pfx=GetWadPathId(tb, 0, tag);
	i=AddWadLumpPathI(s+1, buf, isz, pfx);
	return(i);
}

void *LoadFile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	*rsz=sz;
	return(buf);
}

void StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

byte *bgbcc_rlcbuf=NULL;
int bgbcc_rlcpos;

void *bgbcc_ralloc(int sz)
{
	void *p;

	if(!bgbcc_rlcbuf)
	{
		bgbcc_rlcbuf=(byte *)malloc(1<<18);
		bgbcc_rlcpos=0;
	}

	if((bgbcc_rlcpos+sz)>=((1<<18)-4096))
		bgbcc_rlcpos=0;
	p=(void *)(bgbcc_rlcbuf+bgbcc_rlcpos);
	bgbcc_rlcpos=(bgbcc_rlcpos+sz+15)&(~15);

	return(p);
}

char *bgbcc_rstrdup(char *s)
{
	char *t;
	t=(char *)bgbcc_ralloc(strlen(s)+1);
	strcpy(t, s);
	return(t);
}

char **bgbcc_split(char *s)
{
	char **a, *t;
	int i;

	a=bgbcc_ralloc(64*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		if(*s=='#')break;
		if(*s==';')break;
		if((s[0]=='/') && (s[1]=='/'))
			break;

		t=bgbcc_ralloc(256);
		a[i++]=t;

		if(*s=='"')
		{
			s++;
			while(*s && (*s!='"'))*t++=*s++;
			if(*s=='"')s++;

			*t++=0;
			continue;
		}

		while(*s && (*s>' '))*t++=*s++;
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}

int FccTagForName(char *src)
{
	char tfc[5];
	int v;
	char *s;
	
	s=src+strlen(src);
	while((s>src) && (*s!='.'))
		s--;

	if(*s=='.')
	{
		w_strlwr_n(tfc, s+1, 4);

		if(!strcmp(tfc, "wav"))
			return(BGBCC_FOURCC('W', 'A', 'V', ' '));
		if(!strcmp(tfc, "bmp"))
			return(BGBCC_FOURCC('B', 'M', 'P', ' '));
		if(!strcmp(tfc, "avi"))
			return(BGBCC_FOURCC('A', 'V', 'I', ' '));
		if(!strcmp(tfc, "ico"))
			return(BGBCC_FOURCC('I', 'C', 'O', ' '));

		memcpy(&v, tfc, 4);
		return(v);
	}

	return(0);
}

int log2u(int sz)
{
	int v, k;
	v=sz; k=0;
	while(v>1)
		{ k++; v=(v+1)>>1; }
	return(k);
}

int HashIndexForName16(char *s)
{
	int j, h;
	j = (((int *)(s))[0])+(((int *)(s))[1])+
		(((int *)(s))[2])+(((int *)(s))[3]);
	h = ((j*0xF14A83)>>24)&63;
	return(h);
}

int HashIndexForName32(char *s, int pfx)
{
	u64 v0, v1, v2, v3, v;
	v0=((u64 *)s)[0];
	v1=((u64 *)s)[1];
	v2=((u64 *)s)[2];
	v3=((u64 *)s)[3];
	v=v0+v1+v2+v3+pfx;
	v=((u32)v)+(v>>32);
//	v=((u32)v)+(v>>32);
	v=(u32)v;
	v=v*0xE20B7AC6ULL;
//	v=(v>>48)&65535;
	v=(v>>32)&65535;
	return(v);
}

int main(int argc, char *argv[])
{
	char tbuf[512];
	char tn[257];
	FILE *ifd;
	char *ibuf;
	char *ifn, *ofn, *dir;
	char **a;
	char *s;
	int isz, tag, tag1, tyofs, dirofs;
	int hashofs, hashsz;
	int i, j, k, h;
	
//	ifn=argv[1];
//	ofn=argv[2];

	ifn=NULL;
	ofn=NULL;
	dir=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(argv[i][1]=='C')
			{
				dir=argv[i+1];
				i++;
				continue;
			}
			continue;
		}
		
		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}
		if(!ofn)
		{
			ofn=argv[i];
			continue;
		}
	}

	if(!ifn)
	{
		printf("no input file\n");
		return(-1);
	}

	if(!ofn)
	{
		printf("no output file\n");
		return(-1);
	}

	wad_data=malloc(1<<26);
	wad_rover=64;
	wad_n_lumps=0;
	wad_ldirid=0;
	wad_dirid=0;

//	wad_n_lumps=2;

	AddWadLump("$ROOT", tn, 0, 0, 0);
	AddWadLump("$BITMAP", tn, 0, 0, 0);

	wad_mincsz=99999999;
	wad_maxcsz=0;

	memset(wad_data, 0, 1<<26);

//	ibuf=LoadFile(ifn, &isz);

	ifd=fopen(ifn, "rt");
	if(!ifd)
	{
		printf("fail open %s\n", ifn);
	}
	
	while(!feof(ifd))
	{
		memset(tbuf, 0, 512);
		fgets(tbuf, 511, ifd);
		
		a=bgbcc_split(tbuf);
		if(!a[0])
			continue;
		if((a[0][0]=='#') || (a[0][0]==';') || (a[0][0]=='/'))
			continue;

		if(a[1])
			{ s=a[1]; }
		else
			{ s=a[0]; }
		if((s[0]=='.') && (s[1]=='/'))
			s+=2;
		
		if(dir)
			{ sprintf(tn, "%s/%s", dir, s); }
		else
			{ strcpy(tn, s); }

//		ibuf=LoadFile(a[1], &isz);
		ibuf=LoadFile(tn, &isz);
		
		if(!ibuf)
		{
			printf("fail load %s\n", a[1]);
			continue;
		}

//		strncpy(tn, a[0], 256);
		strncpy(tn, s, 256);
		AddWadLumpPath(tn, ibuf, isz);

//		tag=FccTagForName(a[1]);
//		tag1=FccTagForName(a[0]);
//		if(tag1)
//		{
//			w_strupr_n(tn, a[0], 256);
//			AddWadLumpPath(tn, ibuf, isz);
//		}
//		else
//		{
//			w_strupr_n(tn, a[0], 16);
//			tn[16]=0;
//			AddWadLump(tn, ibuf, isz, tag, 0);
//		}
	}

	FlushPack();

	fclose(ifd);


#if 0
	iwad_data=LoadFile(ifn, &iwad_size);
	iwad_head=(wadinfo_t *)iwad_data;
	iwad_dir=(filelump_t *)(iwad_data+(iwad_head->infotableofs));
	
	for(i=0; i<iwad_head->numlumps; i++)
	{
		printf("%d/%d\r", i, iwad_head->numlumps);
		w_strupr_n(tn, iwad_dir[i].name, 8);
		tn[8]=0;
		AddWadLump(tn,
			iwad_data+iwad_dir[i].filepos,
			iwad_dir[i].size);
	}
	printf("\n");
#endif

	k=log2u(wad_n_lumps)-4;
	if(k<6)k=6;
	if(k>16)k=16;
	hashsz=1<<k;

	for(i=0; i<hashsz; i++)
	{
		wad_hash[i]=0;
	}
	
	for(i=(wad_n_lumps-1); i>=0; i--)
	{
//		h=HashIndexForName16(wad_dir[i].name);
		h=HashIndexForName32(wad_dir[i].name, wad_dir[i].dirid);
		h&=(hashsz-1);
		wad_dir[i].chn=wad_hash[h];
		wad_hash[h]=i;

		j=wad_dir[i].dirid;
		wad_dir[i].dirnext=wad_dir[j].filepos;
		wad_dir[j].filepos=i;
	}

	wad_rover=(wad_rover+63)&(~63);

//	tyofs=wad_rover;
//	memcpy(wad_data+wad_rover, wad_types+0x40, wad_ntypes*4);
//	wad_rover+=wad_ntypes*4;
//	wad_rover=(wad_rover+63)&(~63);

	dirofs=wad_rover;
	wad_rover+=wad_n_lumps*64;
	wad_rover=(wad_rover+63)&(~63);

	hashofs=wad_rover;
	wad_rover+=hashsz*2;
	wad_rover=(wad_rover+63)&(~63);
	
	memcpy(wad_head.identification, "WAD4", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.infotableofs=dirofs/64;
//	wad_head.typeofs=tyofs;
	wad_head.hashoffs=hashofs/64;

	wad_head.bmpoffs=0;
	wad_head.bmpsize=0;

//	memcpy(wad_data+wad_rover, wad_dir, wad_n_lumps*64);
	memcpy(wad_data+dirofs, wad_dir, wad_n_lumps*64);
//	wad_rover+=wad_n_lumps*64;

	memcpy(wad_data+hashofs, wad_hash, hashsz*2);
	
//	memcpy(wad_data+wad_rover, wad_hash, 64*2);
//	wad_rover+=64*2;
	
	memcpy(wad_data, &wad_head, sizeof(wad_head));

//	printf("%d -> %d bytes %d%%\n", iwad_size, wad_rover,
//		(100*wad_rover)/iwad_size);
	
	printf("csz: min=%d max=%d\n", wad_mincsz, wad_maxcsz);
	
	for(i=0; i<16; i++)
	{
		if(wad_cmp[i])
			printf("%d: %d\n", i, wad_cmp[i]);
	}
	
	StoreFile(ofn, wad_data, wad_rover);

	free(wad_data);
}

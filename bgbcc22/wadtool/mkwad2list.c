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
	byte	ident[4];		//Header Magic
	u32		numlumps;
	u32		diroffs;
	u32		typeofs;
} wad2head_t;


typedef struct
{
	u32		foffs;
	u32		size;
	byte	name[8];
} wadlump_t;

typedef struct
{
	u32		foffs;
	u32		csize;
	u32		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	u16		chn;		//Chain
	byte	name[16];
} wad2lump_t;


FILE		*wad_fd;
// wadlump_t	wad_dir[2048];
wad2lump_t	wad_dir[16384];
wad2head_t	wad_head;
int 		wad_n_lumps;
int			wad_types[256];
int			wad_ntypes;
int			wad_ldirid;
int			wad_dirid;

u16			wad_hash[64];

byte		*wad_data;
int			wad_rover;

byte		*iwad_data;
wadlump_t	*iwad_dir;
wad2head_t	*iwad_head;
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

int AddWadLump2(char *name, void *buf, int csz, int dsz, int cmp, int tag)
{
	byte *obuf;
	char tn[17];
	int osz, ety;
	int n;

	ety=WadTypeForTag(tag);

	if(csz!=dsz)
	{
		if(csz<wad_mincsz)
			wad_mincsz=csz;
		if(csz>wad_maxcsz)
			wad_maxcsz=csz;
	}

//	w_strupr_n(tn, name, 16);
	w_strlwr_n(tn, name, 16);
	
	n=wad_n_lumps++;
	wad_dir[n].foffs=wad_rover;
	wad_dir[n].csize=csz;
	wad_dir[n].dsize=dsz;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=cmp;
	wad_dir[n].chn=0;
	memcpy(wad_dir[n].name, tn, 16);
	
	memcpy(wad_data+wad_rover, buf, csz);
	wad_rover+=csz;
//	wad_rover=(wad_rover+15)&(~15);
	wad_rover=(wad_rover+7)&(~7);
	
	return(n);
}

int AddWadLump(char *name, byte *buf, int isz, int tag)
{
	TgvLz_Context *ctx1, *ctx2;
	byte *ibuf;
	byte *obuf;
	byte *obuf1;
	byte *obuf2;
	int osz, osz1, osz2, n, cmp;

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
	
//	if((1.5*osz)<isz)
//	if((1.3*osz)<isz)
//	if((1.2*osz)<isz)
//	if(((1.2*osz)<isz) && (osz>0))
//	if(((1.2*osz)<isz) && (osz>32))
	if(((1.2*osz)<isz) && (osz>16))
	{
		wad_cmp[cmp]++;
		n=AddWadLump2(name, obuf, osz, isz, cmp, tag);
	}else
	{
		wad_cmp[0]++;
		n=AddWadLump2(name, ibuf, isz, isz, 0, tag);
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
		if(!strncmp(wad_dir[i].name, name, 16))
			return(i);
	}
	return(-1);
}

int GetDirLumpNumForName(char *name)
{
	int i;
	
	for(i=0; i<wad_n_lumps; i++)
	{
		if(wad_dir[i].cmp!=1)
			continue;
		if(!strncmp(wad_dir[i].name, name, 16))
			return(i);
	}
	return(-1);
}

int WadBuildPfxName(char *tn, char *name, int pfx)
{
	char *s, *t;

	memset(tn, 0, 16);

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
}

int GetWadPathId(char *name, int pfx, int tag)
{
	char tn[23];
	int i, n, id, ety;

	WadBuildPfxName(tn, name, pfx);

#if 0
	if(pfx)
	{
		tn[0]=DigitBase32((pfx>>10)&31);
		tn[1]=DigitBase32((pfx>> 5)&31);
		tn[2]=DigitBase32((pfx>> 0)&31);
		tn[3]='|';
//		strcpy(tn+4, name);
		w_strlwr_n(tn+4, name, 12);
	}else
	{
//		strcpy(tn, name);
		w_strlwr_n(tn, name, 16);
	}
#endif
	
	i=GetDirLumpNumForName(tn);
	if(i>=0)
	{
		id=(wad_dir[i].foffs)&32767;
		if((id<0) || (id>=wad_dirid))
			{ debug_break }
		return(id);
	}
	
	if(wad_dirid!=(wad_ldirid+1))
	{
		debug_break
	}
	
	ety=WadTypeForTag(tag);

	id=wad_dirid++;
	if(id!=(wad_ldirid+1))
		{ debug_break }
	wad_ldirid=id;

	n=wad_n_lumps++;
	wad_dir[n].foffs=id;
	wad_dir[n].csize=0;
	wad_dir[n].dsize=0;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=1;
	wad_dir[n].chn=0;
	memcpy(wad_dir[n].name, tn, 16);

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
		WadBuildPfxName(tb, name, pfx);

#if 0
		tb[0]=DigitBase32((pfx>>10)&31);
		tb[1]=DigitBase32((pfx>> 5)&31);
		tb[2]=DigitBase32((pfx>> 0)&31);
		tb[3]='|';

		s=name; t=tb+4;
		while(*s && (*s!='.'))
			*t++=*s++;
		*t++=0;
#endif

		return(AddWadLump(tb, buf, isz, tag));
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
		while(*s && (*s!='.'))
			*t++=*s++;
		*t++=0;
		return(AddWadLump(tb, buf, isz, tag));
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

int HashIndexForName16(char *s)
{
	int j, h;
	j = (((int *)(s))[0])+(((int *)(s))[1])+
		(((int *)(s))[2])+(((int *)(s))[3]);
	h = ((j*0xF14A83)>>24)&63;
	return(h);
}

int print_usage(char *argv0)
{
	printf("Usage: %s [opts] <inlist> <outwad>\n", argv0);
	printf("    -C <path>    Read input files from path\n");
	return(0);
}

int main(int argc, char *argv[])
{
	char tbuf[512];
	char tn[257];
	FILE *ifd;
	char *ibuf;
	char *ifn, *ofn, *dir;
	char **a;
	char *s, *s1;
	int isz, tag, tag1, tyofs;
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
		print_usage(argv[0]);
		return(-1);
	}

	if(!ofn)
	{
		printf("no output file\n");
		print_usage(argv[0]);
		return(-1);
	}

	wad_data=malloc(1<<26);
	wad_rover=32;
	wad_n_lumps=0;
	wad_ldirid=0;
	wad_dirid=1;

	wad_mincsz=99999999;
	wad_maxcsz=0;

	memset(wad_data, 0, 1<<26);

//	ibuf=LoadFile(ifn, &isz);

	ifd=fopen(ifn, "rt");
	if(!ifd && dir)
	{
		sprintf(tn, "%s/%s", dir, ifn);
		ifd=fopen(tn, "rt");
	}
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
		
		s1=a[0];
		if((s1[0]=='.') && (s1[1]=='/'))
			s1+=2;

		if(dir)
			{ sprintf(tn, "%s/%s", dir, s); }
		else
			{ strcpy(tn, s); }

		ibuf=LoadFile(tn, &isz);

//		ibuf=LoadFile(a[1], &isz);
		
		if(!ibuf)
		{
			printf("fail load %s\n", a[1]);
			continue;
		}
		
//		tag=FccTagForName(a[1]);
//		tag1=FccTagForName(a[0]);
		tag=FccTagForName(s);
		tag1=FccTagForName(s1);

		if(tag1)
		{
//			w_strupr_n(tn, a[0], 256);
			w_strupr_n(tn, s1, 256);
			AddWadLumpPath(tn, ibuf, isz);
		}
		else
		{
//			w_strupr_n(tn, a[0], 16);
			w_strupr_n(tn, s1, 16);
			tn[16]=0;
			AddWadLump(tn, ibuf, isz, tag);
		}
	}
	
	fclose(ifd);


#if 0
	iwad_data=LoadFile(ifn, &iwad_size);
	iwad_head=(wad2head_t *)iwad_data;
	iwad_dir=(wadlump_t *)(iwad_data+(iwad_head->diroffs));
	
	for(i=0; i<iwad_head->numlumps; i++)
	{
		printf("%d/%d\r", i, iwad_head->numlumps);
		w_strupr_n(tn, iwad_dir[i].name, 8);
		tn[8]=0;
		AddWadLump(tn,
			iwad_data+iwad_dir[i].foffs,
			iwad_dir[i].size);
	}
	printf("\n");
#endif

	for(i=0; i<64; i++)
	{
		wad_hash[i]=0xFFFF;
	}
	
	for(i=(wad_n_lumps-1); i>=0; i--)
	{
		h=HashIndexForName16(wad_dir[i].name);
		wad_dir[i].chn=wad_hash[h];
		wad_hash[h]=i;
	}

	wad_rover=(wad_rover+3)&(~3);

	tyofs=wad_rover;
	memcpy(wad_data+wad_rover, wad_types+0x20, wad_ntypes*4);
	wad_rover+=wad_ntypes*4;

	wad_rover=(wad_rover+15)&(~15);
	
	memcpy(wad_head.ident, "WAD2", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.diroffs=wad_rover;
	wad_head.typeofs=tyofs;

	memcpy(wad_data+wad_rover, wad_dir, wad_n_lumps*32);
	wad_rover+=wad_n_lumps*32;
	
	memcpy(wad_data+wad_rover, wad_hash, 64*2);
	wad_rover+=64*2;
	
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

/*
Make a WAD4 image from a list of files.

Starting a line with '#' or '/' will result in a comment.

Format is generally:
  <lumppath> <filepath> [<ugm>] [<conv>]
Which will import a given file and added as a given lump path.

Meanwhile:
  <lumppath> $<dstpath>
Will create a symbolic link.
If dstpath begins with '/', it will be relative to the global VFS.
If dstpath begins with ':', it will be relative to the mount point for the local filesystem.

Otherwise, it will be relative to its base path within the filesystem.

ugm:
  uid/gid,<mode>
    u+rwx / u=rwx: Set Mode for User
    g+rwx / g=rwx: Set Mode for Group
    o+rwx / o=rwx: Set Mode for Other
    a+rwx / a=rwx: Set Mode for Everyone

conv:
  fmt[,<args>]
Specify a format for conversion on load.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef linux
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define TGVLZ_NOMAIN
#include "tgvlz1.c"

#define BGBCC_BCCX2
#include "../bgbcc_mtool.c"

#if 0
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define	FCC_DIRS					BGBCC_FOURCC('D', 'I', 'R', '$')
#define	FCC_SYMS					BGBCC_FOURCC('S', 'Y', 'M', '$')

#define	FCC_EXE						BGBCC_FOURCC('E', 'X', 'E', ' ')
#define	FCC_COM						BGBCC_FOURCC('C', 'O', 'M', ' ')
#define	FCC_SH						BGBCC_FOURCC('S', 'H', ' ', ' ')
#define	FCC_PF						BGBCC_FOURCC('P', 'F', ' ', ' ')


// #define	BGBCC_FMT_CRAM				BGBCC_FOURCC('C', 'R', 'A', 'M')
// #define	BGBCC_FMT_RGL3				BGBCC_FOURCC('R', 'G', 'L', '3')
// #define	BGBCC_FMT_CQL0				BGBCC_FOURCC('C', 'Q', 'L', '0')

// #include "../mm/cc_fmt_targa.c"
// #include "../mm/cc_fmt_bmp.c"
// #include "../bgbcc_mtool.c"


#define TKFAT_EMODE_SETUID		0x00000800
#define TKFAT_EMODE_SETGID		0x00000400
#define TKFAT_EMODE_STICKY		0x00000200

#define TKFAT_EMODE_ACC_USR		0x000001C0
#define TKFAT_EMODE_ACC_GRP		0x00000038
#define TKFAT_EMODE_ACC_OTH		0x00000007
#define TKFAT_EMODE_ACC_RU		0x00000100
#define TKFAT_EMODE_ACC_WU		0x00000080
#define TKFAT_EMODE_ACC_XU		0x00000040
#define TKFAT_EMODE_ACC_RG		0x00000020
#define TKFAT_EMODE_ACC_WG		0x00000010
#define TKFAT_EMODE_ACC_XG		0x00000008
#define TKFAT_EMODE_ACC_RO		0x00000004
#define TKFAT_EMODE_ACC_WO		0x00000002
#define TKFAT_EMODE_ACC_XO		0x00000001

typedef unsigned short word;

typedef struct
{
	byte	ident[4];		//00: Header Magic
	u32		numlumps;		//04: Number of lumps in the directory.
	u32		diroffs;		//08: Offset of Directory.
 	u32		hashoffs;		//0C: Offset of Directory Hash.
 	u32		bmpoffs;		//10: Offset of Allocation Bitmap.
 	u32		bmpsize;		//14: Bitmap Size
 	u32		resv4;			//18: Reserved
 	u32		resv5;			//1C: Reserved
} wad4head_t;


typedef struct
{
	u32		foffs;
	u32		size;
	byte	name[8];
} wadlump_t;

#if 0
typedef struct
{
	u32		foffs;
	u32		csize;
	u32		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	u16		chn;		//Chain (ExWAD)
	byte	name[16];
} wad2lump_t;
#endif

typedef struct
{
	u32		foffs;		//00: offset
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
// wadlump_t	wad_dir[2048];
// wad4lump_t	wad_dir[16384];
wad4lump_t	*wad_dir;
wad4head_t	wad_head;
int 		wad_n_lumps;
int 		wad_m_lumps;
int			wad_types[256];
int			wad_ntypes;
int			wad_ldirid;
int			wad_dirid;
int			wad_fragrov;

// u16			wad_hash[8192];
u16			wad_hash[65536];

byte		*wad_data;
int			wad_rover;
int			wad_data_sz;

byte		*iwad_data;
wadlump_t	*iwad_dir;
wad4head_t	*iwad_head;
int			iwad_size;

int		wad_mincsz;
int		wad_maxcsz;
int 	wad_cmp[16];

u64		wad_curugm;

int AddWadLump(char *name, byte *buf, int isz, int tag, int pfx);
int FccTagForName(char *src);

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

int W_DefaultModeForTag(int tag)
{
	if((tag==FCC_EXE) || (tag==FCC_COM) || (tag==FCC_SH) || (tag==FCC_PF))
	{
		return(
			TKFAT_EMODE_ACC_RU|TKFAT_EMODE_ACC_XU|
			TKFAT_EMODE_ACC_RG|TKFAT_EMODE_ACC_XG|
			TKFAT_EMODE_ACC_RO|TKFAT_EMODE_ACC_XO);
	}

	return(
		TKFAT_EMODE_ACC_RU|TKFAT_EMODE_ACC_WU|
		TKFAT_EMODE_ACC_RG| TKFAT_EMODE_ACC_RO);
}

u64 W_ParseUGM(char *str)
{
	char *s;
	u64 v;
	int gid, uid, mode;
	
	s=str;
	uid=0;
	gid=0;
	mode=0;

	if((*s>='0') && (*s<='9'))
	{
		uid=atoi(s);
		while((*s>='0') && (*s<='9'))s++;
	}
	if(*s=='/')s++;
	if((*s>='0') && (*s<='9'))
	{
		gid=atoi(s);
		while((*s>='0') && (*s<='9'))s++;
	}

	while(*s)
	{
		if((s[0]=='u') && ((s[1]=='+') || (s[1]=='=')))
		{
			s+=2;
			if(*s=='r')
				{ mode|=TKFAT_EMODE_ACC_RU; s++; }
			if(*s=='w')
				{ mode|=TKFAT_EMODE_ACC_WU; s++; }
			if(*s=='x')
				{ mode|=TKFAT_EMODE_ACC_XU; s++; }

			if(*s=='X')
				{ mode|=TKFAT_EMODE_ACC_XU; s++; }
			if(*s=='s')
				{ mode|=TKFAT_EMODE_SETUID; s++; }
			if(*s=='t')
				{ mode|=TKFAT_EMODE_STICKY; s++; }
			continue;
		}
		if((s[0]=='g') && ((s[1]=='+') || (s[1]=='=')))
		{
			s+=2;
			if(*s=='r')
				{ mode|=TKFAT_EMODE_ACC_RG; s++; }
			if(*s=='w')
				{ mode|=TKFAT_EMODE_ACC_WG; s++; }
			if(*s=='x')
				{ mode|=TKFAT_EMODE_ACC_XG; s++; }
			if(*s=='X')
				{ mode|=TKFAT_EMODE_ACC_XG; s++; }

			if(*s=='s')
				{ mode|=TKFAT_EMODE_SETUID; s++; }
			if(*s=='t')
				{ mode|=TKFAT_EMODE_STICKY; s++; }
			continue;
		}
		if((s[0]=='o') && ((s[1]=='+') || (s[1]=='=')))
		{
			s+=2;
			if(*s=='r')
				{ mode|=TKFAT_EMODE_ACC_RO; s++; }
			if(*s=='w')
				{ mode|=TKFAT_EMODE_ACC_WO; s++; }
			if(*s=='x')
				{ mode|=TKFAT_EMODE_ACC_XO; s++; }
			if(*s=='X')
				{ mode|=TKFAT_EMODE_ACC_XO; s++; }

			if(*s=='s')
				{ mode|=TKFAT_EMODE_SETUID; s++; }
			if(*s=='t')
				{ mode|=TKFAT_EMODE_STICKY; s++; }
			continue;
		}

		if((s[0]=='a') && ((s[1]=='+') || (s[1]=='=')))
		{
			s+=2;
			if(*s=='r')
				{ mode|=
					TKFAT_EMODE_ACC_RU|
					TKFAT_EMODE_ACC_RG|
					TKFAT_EMODE_ACC_RO; s++; }
			if(*s=='w')
				{ mode|=
					TKFAT_EMODE_ACC_WU|
					TKFAT_EMODE_ACC_WG|
					TKFAT_EMODE_ACC_WO; s++; }
			if(*s=='x')
				{ mode|=
					TKFAT_EMODE_ACC_XU|
					TKFAT_EMODE_ACC_XG|
					TKFAT_EMODE_ACC_XO; s++; }

			if(*s=='X')
				{ mode|=
					TKFAT_EMODE_ACC_XU|
					TKFAT_EMODE_ACC_XG|
					TKFAT_EMODE_ACC_XO; s++; }
			if(*s=='s')
				{ mode|=TKFAT_EMODE_SETUID; s++; }
			if(*s=='t')
				{ mode|=TKFAT_EMODE_STICKY; s++; }
			continue;
		}

		if((s[0]=='u') && (s[1]=='g') && ((s[2]=='+') || (s[2]=='=')))
		{
			s+=3;
			if(*s=='r')
				{ mode|=TKFAT_EMODE_ACC_RU|TKFAT_EMODE_ACC_RG; s++; }
			if(*s=='w')
				{ mode|=TKFAT_EMODE_ACC_WU|TKFAT_EMODE_ACC_WG; s++; }
			if(*s=='x')
				{ mode|=TKFAT_EMODE_ACC_XU|TKFAT_EMODE_ACC_XG; s++; }

			if(*s=='X')
				{ mode|=TKFAT_EMODE_ACC_XU|TKFAT_EMODE_ACC_XG; s++; }
			if(*s=='s')
				{ mode|=TKFAT_EMODE_SETUID; s++; }
			if(*s=='t')
				{ mode|=TKFAT_EMODE_STICKY; s++; }
			continue;
		}

		if((*s==',') || (*s==';'))
		{
			s++;
			continue;
		}

		break;
	}
	
	v=mode;
	v=(v<<16)|gid;
	v=(v<<16)|uid;
	return(v);
}

int	WadCheckExpand(int csz)
{
	int osz;

	if((wad_rover+csz+131072)>=wad_data_sz)
	{
		osz=wad_data_sz+(wad_data_sz>>1);
		while((wad_rover+csz+64)>=osz)
			osz=osz+(osz>>1);
		printf("Expand %d->%d\n", wad_data_sz, osz);
		wad_data=realloc(wad_data, osz);
		wad_data_sz=osz;
	}

	if((wad_n_lumps+256)>wad_m_lumps)
	{
		osz=wad_m_lumps+(wad_m_lumps>>1);
		printf("Expand Dir %d->%d\n", wad_m_lumps, osz);
		wad_dir=realloc(wad_dir, osz*sizeof(wad4lump_t));
		wad_m_lumps=osz;
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
	
	if(tag==FCC_DIRS)
		ety=2;
	if(tag==FCC_SYMS)
		ety=3;

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

	if(ety!=2)
		{ wad_dir[n].foffs=wad_rover/64; }
	wad_dir[n].csize=csz;
	wad_dir[n].dsize=dsz;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=cmp;
	wad_dir[n].chn=0;
	wad_dir[n].dirid=pfx;
	memcpy(wad_dir[n].name, tn, 32);

	if(wad_curugm)
	{
		wad_dir[n].uid =wad_curugm>> 0;
		wad_dir[n].gid =wad_curugm>>16;
		wad_dir[n].mode=wad_curugm>>32;
	}else
	{
		wad_dir[n].mode=W_DefaultModeForTag(tag);
		wad_dir[n].uid=0;
		wad_dir[n].gid=0;
	}

	WadCheckExpand(csz);
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
	{
		wad_packpos=0;
		wad_n_packlump=0;
		return(0);
	}

	j=wad_fragrov++;
	sprintf(tn, "$PACK:%08X", j);
	n=AddWadLump(tn, wad_packbuf, wad_packpos, 0, 1);
	
	for(i=0; i<wad_n_packlump; i++)
	{
		n1=wad_n_lumps++;
		memcpy(wad_dir+n1, wad_packlump+i, sizeof(wad4lump_t));
		wad_dir[n1].foffs|=(n<<16);
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

	if(((wad_packpos+isz+128)>=131072) || ((wad_n_packlump+1)>=256))
	{
		FlushPack();
	}

	ety=1;
	if(tag==FCC_SYMS)
		ety=3;

	memset(tn, 0, 33);
	strncpy(tn, name, 32);
	
	n=wad_n_packlump++;
	memset(wad_packlump+n, 0, sizeof(wad4lump_t));

	wad_packpos=(wad_packpos+63)&(~63);
	wad_packlump[n].foffs=wad_packpos/64;
	wad_packlump[n].csize=isz;
	wad_packlump[n].dsize=isz;
//	wad_packlump[n].ety=1;
	wad_packlump[n].ety=ety;
	wad_packlump[n].cmp=2;
	wad_packlump[n].chn=0;
	wad_packlump[n].dirid=pfx;
	memcpy(wad_packlump[n].name, tn, 32);

	if(wad_curugm)
	{
		wad_packlump[n].uid =wad_curugm>> 0;
		wad_packlump[n].gid =wad_curugm>>16;
		wad_packlump[n].mode=wad_curugm>>32;
	}else
	{
		wad_packlump[n].mode=W_DefaultModeForTag(tag);
		wad_packlump[n].uid=0;
		wad_packlump[n].gid=0;
	}


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
	u32 csum, xcsum;
	int osz, osz1, osz2, n, cmp;

	if(!isz)
	{
		n=AddWadLump2(name, buf, isz, isz, 0, tag, pfx);
		return(n);
	}

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

//	csum=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);
	csum=TgvLz_CalculateImagePel4BChecksumAc(ibuf, isz);
	xcsum=~csum;
	obuf1[osz1+0]=(csum>> 0)&255;
	obuf1[osz1+1]=(csum>> 8)&255;
	obuf1[osz1+2]=(csum>>16)&255;
	obuf1[osz1+3]=(csum>>24)&255;
	obuf1[osz1+4]=(xcsum>> 0)&255;
	obuf1[osz1+5]=(xcsum>> 8)&255;
	obuf1[osz1+6]=(xcsum>>16)&255;
	obuf1[osz1+7]=(xcsum>>24)&255;
	osz1+=8;
	
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
	
//	if(((1.2*osz)<isz) && (osz>64) && (cmp>0))
	if(((1.2*osz)<isz) && (isz>128) && (cmp>0))
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
		if(!strncmp((char *)(wad_dir[i].name), name, 32))
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
		if(!strncmp((char *)(wad_dir[i].name), name, 32))
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
		return(i);
//		id=(wad_dir[i].foffs)&32767;
//		if((id<0) || (id>=wad_dirid))
//			{ debug_break }
//		return(id);
	}
	
//	if(wad_dirid!=(wad_ldirid+1))
//	{
//		debug_break
//	}
	
	ety=WadTypeForTag(tag);

//	id=wad_dirid++;
//	if(id!=(wad_ldirid+1))
//		{ debug_break }
//	wad_ldirid=id;

	n=wad_n_lumps++;
	memset(wad_dir+n, 0, sizeof(wad4lump_t));

	id=n;

//	wad_dir[n].foffs=id;
	wad_dir[n].foffs=0;
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

int AddWadLumpPathI(char *name, byte *buf, int isz, int tag, int pfx)
{
	char tb[256];
	int pfx1, tag1;
	char *s, *t;
	int i;

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

	tag1=FccTagForName(tb);
	pfx1=GetWadPathId(tb, pfx, tag1);
	i=AddWadLumpPathI(s+1, buf, isz, tag, pfx1);
	return(i);
}

int AddWadLumpPathTag(char *name, byte *buf, int isz, int tag)
{
	char tb[256];
	int pfx, tag1;
	char *s, *t;
	int i;

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

	tag1=FccTagForName(tb);
	pfx=GetWadPathId(tb, 0, tag1);
	i=AddWadLumpPathI(s+1, buf, isz, tag, pfx);
	return(i);
}

int AddWadLumpPath(char *name, byte *buf, int isz)
{
	int tag;
	tag=FccTagForName(name);
	return(AddWadLumpPathTag(name, buf, isz, tag));
}

#ifdef linux
int W_IsRegFile(const char *path)
{
    struct stat statbuf;
    int r;
    r=stat(path, &statbuf);
    if(r<0)
		return(0);
    if (S_ISREG(statbuf.st_mode))
        return(1);
    return(0);
}
#endif

void *LoadFile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;

// #ifdef linux
#if 0
	if(!W_IsRegFile(name))
	{
		printf("LoadFile: Not File %s\n", name);
		return(NULL);
	}
#endif
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("LoadFile: Fail Open %s\n", name);
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	*rsz=sz;
	return(buf);
}

void *bgbcc_loadfile(char *name, int *rsz)
{
	return(LoadFile(name, rsz));
}

void StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

int BGBCC_StoreFile(char *name, void *buf, int sz)
{
	StoreFile(name, buf, sz);
	return(0);
}

int bgbcc_storefile(char *name, void *buf, int sz)
{
	StoreFile(name, buf, sz);
	return(0);
}

#if 0
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
	memset(a, 0, 64*sizeof(char *));
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
#endif

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

		if(!strcmp(tfc, "exe"))
			return(BGBCC_FOURCC('E', 'X', 'E', ' '));
		if(!strcmp(tfc, "com"))
			return(BGBCC_FOURCC('C', 'O', 'M', ' '));
		if(!strcmp(tfc, "sh"))
			return(BGBCC_FOURCC('S', 'H', ' ', ' '));
		if(!strcmp(tfc, "pf"))
			return(BGBCC_FOURCC('P', 'F', ' ', ' '));

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

int print_usage(char *argv0)
{
	printf("Usage: %s [opts] <inlist> <outwad>\n", argv0);
	printf("    -C <path>    Read input files from path\n");
	printf("\n");
	printf("Makes a WAD4 image from a listing file\n");
	printf("  <path> <infile>    Import file given by input path\n");
	printf("  <path> $<dstpath>  Create a symbolic link\n");
	return(0);
}

// int main(int argc, char *argv[])
int main(int argc, char *argv[], char **env)
{
	char tbuf[512];
	char tn[257];
	FILE *ifd;
	char *ibuf, *ibuf1;
	char *ifn, *ofn, *dir, *tcnv;
	char **a;
	char *s, *s1, *sugm;
	u64 ugm;
	u32 tag, tag1;
	int isz, isz1, tyofs, dirofs;
	int	rov1, rov2, drov;
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
		print_usage(argv[0]);
		return(-1);
	}

	if(!ofn)
	{
		printf("no output file\n");
		print_usage(argv[0]);
		return(-1);
	}

//	wad_data_sz=1<<24;
	wad_data_sz=1<<28;
	wad_data=malloc(wad_data_sz);

	wad_m_lumps=16384;
	wad_dir=malloc(wad_m_lumps*sizeof(wad4lump_t));

	wad_rover=64;
	wad_n_lumps=0;
	wad_ldirid=0;
	wad_dirid=0;

//	wad_n_lumps=2;

//	AddWadLump("$ROOT", tn, 0, 0, 0);
	AddWadLump("$ROOT", (byte *)tn, 0, FCC_DIRS, 0);
//	AddWadLump("$BITMAP", tn, 0, 0, 0);
	AddWadLump("$BITMAP", (byte *)tn, 0, 0, 1);

	wad_mincsz=99999999;
	wad_maxcsz=0;

	memset(wad_data, 0, wad_data_sz);

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
		return(-1);
	}
	
	wad_curugm=0;
	
	while(!feof(ifd))
	{
		memset(tbuf, 0, 512);
		fgets(tbuf, 511, ifd);
		
		a=bgbcc_split(tbuf);
		if(!a[0])
			continue;
		if((a[0][0]=='#') || (a[0][0]==';') || (a[0][0]=='/'))
			continue;

		sugm=NULL;
		tcnv=NULL;

		if(a[1])
		{
			s=a[1];
			if(a[2] && (a[2][0]>='0') && (a[2][0]<='9'))
			{
				sugm=a[2];
				if(a[3])
					tcnv=a[3];
			}else if(a[2])
			{
				tcnv=a[2];
			}
		}
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

		tag=FccTagForName(s1);
		if(s[0]=='$')
		{
			tag=FCC_SYMS;
			ibuf=strdup(s+1);
			isz=strlen(ibuf);
		}else
		{
//			ibuf=LoadFile(a[1], &isz);
			ibuf=LoadFile(tn, &isz);
		}
		
		if(!ibuf)
		{
			printf("fail load %s\n", tn);
			continue;
		}

		wad_curugm=0;
		if(sugm)
			{ wad_curugm=W_ParseUGM(sugm); }

		printf("Add: %-24s (%08X)  ", tn, tag);

		rov1=wad_rover;

		if(tcnv && tcnv[1])
		{
			ibuf1=(char *)BGBCC_LoadConvResource(
				(byte *)ibuf, isz, tag,
				tcnv, &isz1, &tag1);
			
			if(ibuf1)
			{
				ibuf=ibuf1;
				isz=isz1;
				tag=tag1;
			}
		}

//		strncpy(tn, a[0], 256);
//		strncpy(tn, s, 256);
		strncpy(tn, s1, 256);
//		AddWadLumpPath(tn, ibuf, isz);
		AddWadLumpPathTag(tn, (byte *)ibuf, isz, tag);

		rov2=wad_rover;
		drov=rov2-rov1;
		printf("%d->%d, %.2f%% (tot=%d)\n",
			isz, drov, (100.0*drov)/(isz+1), rov2);

		free(ibuf);

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
	
	printf("End Of List\n");

	FlushPack();

	fclose(ifd);


#if 0
	iwad_data=LoadFile(ifn, &iwad_size);
	iwad_head=(wad4head_t *)iwad_data;
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
		if(wad_dir[i].name[0]=='$')
			wad_dir[i].dirid=1;
	}
	
	for(i=(wad_n_lumps-1); i>=0; i--)
	{
//		h=HashIndexForName16(wad_dir[i].name);
		h=HashIndexForName32((char *)(wad_dir[i].name), wad_dir[i].dirid);
		h&=(hashsz-1);

		printf("TK_Wad4_LookupLumpNameW4: %s pfx=%d h=%X\n",
			wad_dir[i].name, wad_dir[i].dirid, h);

		wad_dir[i].chn=wad_hash[h];
		wad_hash[h]=i;

		j=wad_dir[i].dirid;
		if((i!=1) && (j!=1) && (i!=j) &&
			(wad_dir[i].name[0]!='$'))
		{
			printf("TK_Wad4_LookupLumpNameW4: link %d -> %d\n",
				i, wad_dir[j].foffs);

			wad_dir[i].dirnext=wad_dir[j].foffs;
			wad_dir[j].foffs=i;
		}
		
		printf("TK_Wad4_LookupLumpNameW4: %s nxt=%d\n",
			wad_dir[i].name, wad_dir[i].dirnext);
	}

	wad_rover=(wad_rover+63)&(~63);

//	tyofs=wad_rover;
//	memcpy(wad_data+wad_rover, wad_types+0x40, wad_ntypes*4);
//	wad_rover+=wad_ntypes*4;
//	wad_rover=(wad_rover+63)&(~63);

	WadCheckExpand(wad_n_lumps*64);

	dirofs=wad_rover;
	wad_rover+=wad_n_lumps*64;
	wad_rover=(wad_rover+63)&(~63);

	WadCheckExpand(hashsz*2);

	hashofs=wad_rover;
	wad_rover+=hashsz*2;
	wad_rover=(wad_rover+63)&(~63);
	
	memcpy(wad_head.ident, "WAD4", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.diroffs=dirofs/64;
//	wad_head.typeofs=tyofs;
	wad_head.hashoffs=hashofs/64;

	wad_head.bmpoffs=0;
	wad_head.bmpsize=0;

	WadCheckExpand(1);

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

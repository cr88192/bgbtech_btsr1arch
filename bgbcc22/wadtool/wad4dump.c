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
	u32		csize;
	u32		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	u16		chn;		//Chain (ExWAD)
	byte	name[16];
} wad2lump_t;

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
// filelump_t	wad_dir[2048];
wad4lump_t	wad_dir[16384];
wad4head_t	wad_head;
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
wad4lump_t	*iwad_dir;
wad4head_t	*iwad_head;
int			iwad_size;

int		wad_mincsz;
int		wad_maxcsz;
int 	wad_cmp[16];



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

int PathnameForLump(int lump, char *path)
{
	int pfx;

	if(lump<0)
	{
		strcpy(path, "");
		return(0);
	}

	pfx=iwad_dir[lump].dirid;
	if(pfx>0)
	{
		PathnameForLump(iwad_dir[lump].dirid, path);
		strcat(path, "/");
		strcat(path, iwad_dir[lump].name);
	}else
	{
		strcpy(path, iwad_dir[lump].name);
	}
	return(0);
}

char *NameForEty(int ty)
{
	char *s0;
	switch(ty)
	{
	case 0: s0="Fre"; break;
	case 1: s0="Reg"; break;
	case 2: s0="Dir"; break;
	case 3: s0="Lnk"; break;
	}
	return(s0);
}

char *NameForCmp(int ty)
{
	char *s0;
	switch(ty)
	{
	case 0: s0="Sto";		break;
	case 1: s0="Frg";		break;
	case 2: s0="Pck";		break;
	case 3: s0="RP2";		break;
	case 4: s0="LZ4";		break;
	}
	return(s0);
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
//		printf("no output file\n");
//		return(-1);
	}

#if 1
	iwad_data=LoadFile(ifn, &iwad_size);
	iwad_head=(wad4head_t *)iwad_data;
	iwad_dir=(wad4lump_t *)(iwad_data+(iwad_head->diroffs)*64);
	
	for(i=0; i<iwad_head->numlumps; i++)
	{
//		printf("%s\n", iwad_dir[i].name);

		PathnameForLump(i, tbuf);
		printf("%03X  %-40s %6d/%6d %s/%s %04X_%04X\n",
			i,
			tbuf,
			iwad_dir[i].dsize,
			iwad_dir[i].csize,
			NameForEty(iwad_dir[i].ety),
			NameForCmp(iwad_dir[i].cmp),
			iwad_dir[i].foffs>>16,
			iwad_dir[i].foffs&65535);
	
//		printf("%d/%d\r", i, iwad_head->numlumps);
//		w_strupr_n(tn, iwad_dir[i].name, 8);
//		tn[8]=0;
//		AddWadLump(tn,
//			iwad_data+iwad_dir[i].foffs,
//			iwad_dir[i].size);
	}
//	printf("\n");
#endif

}

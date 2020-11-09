#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
} wad1head_t;

typedef struct
{
	u32		foffs;
	u32		size;
	byte	name[8];
} wad1lump_t;


FILE		*wad_fd;
wad1lump_t	wad_dir[256];
wad1head_t	wad_head;
int 		wad_n_lumps;

byte		*wad_data;
int			wad_rover;

void w_strupr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = toupper(*s++); }
	for(; i<n; i++)
		*t++=0;
}

int AddWad1Lump(char *name, void *buf, int sz)
{
	byte *obuf;
	char tn[17];
	int algn;
//	int osz, ety;
	int n;

	algn=63;

	w_strupr_n(tn, name, 16);
//	w_strlwr_n(tn, name, 16);

	if(!memcmp(tn, "TKPE", 4))
		algn=1023;

	wad_rover=(wad_rover+algn)&(~algn);
	
	n=wad_n_lumps++;
	wad_dir[n].foffs=wad_rover;
	wad_dir[n].size=sz;
	memcpy(wad_dir[n].name, tn, 8);
	
	memcpy(wad_data+wad_rover, buf, sz);
	wad_rover+=sz;
//	wad_rover=(wad_rover+15)&(~15);
//	wad_rover=(wad_rover+7)&(~7);
	wad_rover=(wad_rover+algn)&(~algn);
	
	return(n);
}

int AddPlfwLump(char *name, void *buf, int sz)
{
	byte *cs;
	char tn1[64];
	int n, w1, w2;

	strcpy(tn1, name);

	cs=buf;
	if(	!memcmp(cs , "PEL" , 3) &&
		 memcmp(tn1, "TKPE", 4))
	{
		w1=*(u16 *)(cs+0x04);
		w2=*(u16 *)(cs+0x18);
		
		if(w1==0xB264)
		{
			strcpy(tn1, "TKPEB2DA");
			if(w2==0x010B)
			{
				/* 32 bit pointers */
			}else if(w2==0x020B)
			{
				/* 64 bit pointers */
			}
		}
	}

	n=AddWad1Lump(tn1, buf, sz);
	return(n);
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
	wad_rover=1024;
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

		w_strupr_n(tn, s1, 16);
		tn[16]=0;
//		AddWadLump(tn, ibuf, isz, tag);
		AddPlfwLump(tn, ibuf, isz);
	}
	
	fclose(ifd);

//	wad_rover=(wad_rover+3)&(~3);

	wad_rover=(wad_rover+15)&(~15);
	
	memcpy(wad_head.ident, "PLFW", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.diroffs=wad_rover;

	memcpy(wad_data+16, wad_dir, wad_n_lumps*16);
	memcpy(wad_data, &wad_head, sizeof(wad_head));
	
	StoreFile(ofn, wad_data, wad_rover);

	free(wad_data);
}

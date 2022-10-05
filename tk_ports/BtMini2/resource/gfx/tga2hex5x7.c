#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#ifdef _WIN32
// #include <windows.h>
#endif

#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

#include "bt1h_targa.c"

byte *TgvLz_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		if(i>0)
		{
			sz=i;
		}else
		{
			free(buf);
			return(NULL);
		}
	}

	memset(buf+sz, 0, 16);
	
	*rsz=sz;
	return(buf);
}

int TgvLz_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int main()
{
	u64 tab[256];
	u16 *tbuf;
	u64 tv;
	int xs, ys;
	int x, y, x1, y1;
	int i, j, k;
	
//	tbuf=BTIC1H_Img_LoadTGA555("fixed0_8px_1.tga", &xs, &ys);
//	tbuf=BTIC1H_Img_LoadTGA555("cyril0.tga", &xs, &ys);
	tbuf=BTIC1H_Img_LoadTGA555("fixed_5x6_0.tga", &xs, &ys);
	
//	for(y=0; y<16; y++)
//	for(y=0; y<8; y++)
	for(y=0; y<6; y++)
		for(x=0; x<16; x++)
	{
		tv=0;
		for(y1=0; y1<6; y1++)
			for(x1=0; x1<5; x1++)
		{
//			i=tbuf[((127-(y*8+y1))*128)+(x*8+x1)];
//			i=tbuf[((63-(y*8+y1))*128)+(x*8+x1)];
			i=tbuf[((35-(y*6+y1))*80)+(x*5+x1)];
			j=((5-y1)*5)+(4-x1);
			if(i)
				tv|=1LL<<j;
		}
		
		i=(y*16+x)+32;
		j=i;
		if((j<' ') || (j>'~'))
			j=' ';
		if(	(j=='.') || (j==',') ||
			(j==':') || (j==';') ||
			(j=='p') || (j=='q') ||
			(j=='g') || (j=='y') )
				tv|=1<<30;
			
		printf("0x%08X, /* %02X '%c' */\n", (u32)tv, i, j);
	}
}

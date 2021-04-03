#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>

typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#else

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#endif

#include "btrlz0_dec.c"
#include "btrlz0_enc.c"

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
	
//	*(u64 *)(buf+sz+0)=0;
//	*(u64 *)(buf+sz+8)=0;
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

int main(int argc, char *argv[])
{
	BTRLZ0_EncContext *ectx;
	BTRLZ0_DecContext *dctx;
	byte *ibuf, *obuf, *o2buf;
	char *ifn, *ofn;
	u64 totb;
	double ts;
	int ibsz, obsz, osz, o2sz;
	int t0, t1, t1e;
	int i;
	
	ifn=NULL;
	ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}
	
	if(!ifn)
	{
		printf("No input file\n");
		return(0);
	}
	
	ibuf=TgvLz_LoadFile(ifn, &ibsz);
	
	if(!ibuf)
	{
		printf("Failed to load input\n");
		return(0);
	}
	
	obsz=ibsz*4;
	obuf=malloc(obsz);
	o2buf=malloc(obsz);
	
	ectx=malloc(sizeof(BTRLZ0_EncContext));
	memset(ectx, 0, sizeof(BTRLZ0_EncContext));
	osz=BTRLZ0_EncodeBuffer(ectx, ibuf, obuf, ibsz, obsz);
	printf("Enc %d -> %d\n", ibsz, osz);
	
	dctx=malloc(sizeof(BTRLZ0_DecContext));
	memset(dctx, 0, sizeof(BTRLZ0_DecContext));
	o2sz=BTRLZ0_DecodeBuffer(dctx, obuf, o2buf, osz, obsz);
	printf("Dec %d -> %d\n", osz, o2sz);

	if(ibsz!=o2sz)
	{
		printf("Size Mismatch\n");
		return(0);
	}
	
	if(memcmp(ibuf, o2buf, ibsz))
	{
		printf("Data Mismatch\n");
		return(0);
	}

	totb=0;
	t0=clock();
	t1=t0;
	t1e=t0+(10*CLOCKS_PER_SEC);
	while(t1<t1e)
	{
		ts=(t1-t0)*(1.0/CLOCKS_PER_SEC);
		printf("%fMB/s\r", totb/(ts*1048576+1));
		o2sz=BTRLZ0_DecodeBuffer(dctx, obuf, o2buf, osz, obsz);
		totb+=o2sz;
		t1=clock();
	}
	printf("\n");
}

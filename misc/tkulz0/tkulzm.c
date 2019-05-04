#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tkulzd.c"
#include "tkulze.c"

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int storefile(char *path, byte *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(path, "wb");
	if(!fd)
		return(-1);

	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int main(int argc, char *argv[])
{
	TKuLZ_EncState tectx;
	TKuLZ_DecState tdctx;
	byte *ibuf, *tbuf, *obuf;
	char *ifn, *ofn;
	int isz, tsz, osz;
	int i, j, k;
	
	ifn=NULL;
	ofn=NULL;
	
	for(i=1; argv[i]; i++)
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
		return(-1);
	}
	
	ibuf=loadfile(ifn, &isz);
	
	tsz=isz*2;
	osz=isz*2;
	tbuf=malloc(tsz);
	obuf=malloc(osz);

	tsz=TKuLZ_EncodeBuffer(&tectx, tbuf, ibuf, tsz, isz);
	
	printf("TKuLZe %d->%d %.2f%%\n", isz, tsz, (100.0*tsz)/isz);

	osz=TKuLZ_DecodeBuffer(&tdctx, obuf, tbuf, osz, tsz);
	printf("TKuLZd %d->%d, %s\n", isz, osz, (isz==osz)?"Pass":"Fail");
	
	if(isz==osz)
	{
		if(memcmp(ibuf, obuf, isz))
			printf("Mismatch\n");
	}
	
	return(0);
}

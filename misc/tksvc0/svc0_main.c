#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svc0_dec.c"
#include "svc0_enc.c"
#include "rasw_targa.c"


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

int usage(char *arg0)
{
	printf("%s [opts] infile outfile\n", arg0);
	printf("-d      decode\n");
	printf("-e      encode\n");
	return(0);
}

int main(int argc, char *argv[])
{
	TKSVC0E_EncState *ctxe;
	TKSVC0D_DecState *ctxd;
	char *ifn, *ofn;
	byte *ibuf, *obuf, *tbuf;
	int ixs, iys, md, qlvl, osz;
	int i, j, k;

	ifn=NULL;
	ofn=NULL;
	md=0;
	qlvl=75;
	
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-e"))
				{ md=1; continue; }
			if(!strcmp(argv[i], "-d"))
				{ md=2; continue; }
			if(!strcmp(argv[i], "-q"))
				{ i++; qlvl=atoi(argv[i]); continue; }
			continue;
		}
		
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}
	
	if(!ifn || !ofn || !md)
	{
		usage(argv[0]);
		return(-1);
	}

	if(md==1)
	{
		ibuf=BGBRASW_Img_LoadTGA(ifn, &ixs, &iys);
		if(!ibuf)
		{
			printf("Failed to load TGA %s\n", ifn);
			return(-1);
		}
		
		ctxe=TKSVC0E_CreateContext();
		TKSVC0E_SetQuality(ctxe, qlvl);
		TKSVC0E_SetFrameSize(ctxe, ixs, iys);
		TKSVC0E_UpdateFrameImage(ctxe, ibuf);
		
		obuf=malloc(1<<24);
		memset(obuf, 0, 256);
		
		osz=TKSVC0E_EncodeBuffer(ctxe, obuf+64, 1<<24);
		
		obuf[0]='S';	obuf[1]='V';
		obuf[2]='C';	obuf[3]='0';
		obuf[4]=ixs;	obuf[5]=ixs>>8;
		obuf[6]=iys;	obuf[7]=iys>>8;
		
		printf("Encoded as %d bytes\n", osz);
		
		if(osz>0)
		{
			storefile(ofn, obuf, osz+64);
		}
		
		memset(ibuf, 0, ixs*iys*4);
		
		ctxd=TKSVC0D_CreateContext();
		TKSVC0D_SetFrameSize(ctxd, ixs, iys);
		TKSVC0D_DecodeBuffer(ctxd, obuf+64, osz);
		TKSVC0D_GetImageRGBA(ctxd, ibuf);
		BGBRASW_Img_SaveTGA("dump0.tga", ibuf, ixs, iys);
		
		return(0);
	}

	usage(argv[0]);
	return(-1);
}

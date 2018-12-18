#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef unsigned int u32;

int main(int argc, char *argv[])
{
	FILE *fd, *fdoa, *fdob, *fdoc, *fdod;
	byte *ibuf;
	char *infn;
	u32 va, vb, vc, vd;
	int sz, n;
	int i, j, k;
	
	infn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		
		if(!infn)
			infn=argv[i];
	}
	
	if(!infn)
	{
		printf("No Input File\n");
		return(-1);
	}
	
	
	fd=fopen(infn, "rb");
	if(!fd)
	{
		printf("Can't Open File %s\n", infn);
		return(-1);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	ibuf=malloc(sz+32);
	fread(ibuf, 1, sz, fd);
	fclose(fd);
	
	n=(sz+15)/16;
	
	fdoa=fopen("bootrom_1a.txt", "wt");
	fdob=fopen("bootrom_1b.txt", "wt");
	fdoc=fopen("bootrom_1c.txt", "wt");
	fdod=fopen("bootrom_1d.txt", "wt");
	
	for(i=0; i<n; i++)
	{
		va=*(u32 *)(ibuf+i*16+ 0);
		vb=*(u32 *)(ibuf+i*16+ 4);
		vc=*(u32 *)(ibuf+i*16+ 8);
		vd=*(u32 *)(ibuf+i*16+12);
		
		fprintf(fdoa, "%08X\n", va);
		fprintf(fdob, "%08X\n", vb);
		fprintf(fdoc, "%08X\n", vc);
		fprintf(fdod, "%08X\n", vd);
	}
	
	fclose(fdoa);
	fclose(fdob);
	fclose(fdoc);
	fclose(fdod);
	
	return(0);
}

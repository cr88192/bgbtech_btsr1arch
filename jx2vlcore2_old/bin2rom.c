#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef unsigned int u32;

int main(int argc, char *argv[])
{
	FILE *fd, *fdoa, *fdob, *fdoc, *fdod;
	FILE *fdoe, *fdof, *fdog, *fdoh;
	byte *ibuf;
	char *infn;
	u32 va, vb, vc, vd;
	u32 ve, vf, vg, vh;
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

#if 1
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
#endif
	
#if 1
	n=(sz+31)/32;
	
	fdoa=fopen("bootrom_2a.txt", "wt");
	fdob=fopen("bootrom_2b.txt", "wt");
	fdoc=fopen("bootrom_2c.txt", "wt");
	fdod=fopen("bootrom_2d.txt", "wt");
	fdoe=fopen("bootrom_2e.txt", "wt");
	fdof=fopen("bootrom_2f.txt", "wt");
	fdog=fopen("bootrom_2g.txt", "wt");
	fdoh=fopen("bootrom_2h.txt", "wt");
	
	for(i=0; i<n; i++)
	{
		va=*(u32 *)(ibuf+i*32+ 0);
		vb=*(u32 *)(ibuf+i*32+ 4);
		vc=*(u32 *)(ibuf+i*32+ 8);
		vd=*(u32 *)(ibuf+i*32+12);
		ve=*(u32 *)(ibuf+i*32+16);
		vf=*(u32 *)(ibuf+i*32+20);
		vg=*(u32 *)(ibuf+i*32+24);
		vh=*(u32 *)(ibuf+i*32+28);
		
		fprintf(fdoa, "%08X\n", va);
		fprintf(fdob, "%08X\n", vb);
		fprintf(fdoc, "%08X\n", vc);
		fprintf(fdod, "%08X\n", vd);
		fprintf(fdoe, "%08X\n", ve);
		fprintf(fdof, "%08X\n", vf);
		fprintf(fdog, "%08X\n", vg);
		fprintf(fdoh, "%08X\n", vh);
	}
	
	fclose(fdoa);
	fclose(fdob);
	fclose(fdoc);
	fclose(fdod);
	fclose(fdoe);
	fclose(fdof);
	fclose(fdog);
	fclose(fdoh);
#endif
	
	return(0);
}

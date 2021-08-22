#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef unsigned int u32;
typedef unsigned long long u64;

int main(int argc, char *argv[])
{
	byte	shufa[4][64];
	char tb[256];
	FILE *fd, *fdoa, *fdob, *fdoc, *fdod;
	FILE *fdoe, *fdof, *fdog, *fdoh;
	byte *ibuf;
	char *infn;
	char *ofn_pfx;
	u64 lva, lvb, lvc, lvd;
	u32 va, vb, vc, vd;
	u32 ve, vf, vg, vh;
	int sz, n;
	int i, j, k, l;
	
	infn=NULL;
	ofn_pfx=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		
		if(!infn)
			{ infn=argv[i]; continue; }
		if(!ofn_pfx)
			{ ofn_pfx=argv[i]; continue; }
	}
	
	if(!infn)
	{
		printf("No Input File\n");
		return(-1);
	}
	
	if(!ofn_pfx)
		ofn_pfx="bootrom";
	
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
	
	sprintf(tb, "%s_1.txt", ofn_pfx);
//	fdoa=fopen("bootrom_1.txt", "wt");
	fdoa=fopen(tb, "wt");
	
	for(i=0; i<n; i++)
	{
		va=*(u32 *)(ibuf+i*16+ 0);
		vb=*(u32 *)(ibuf+i*16+ 4);
		vc=*(u32 *)(ibuf+i*16+ 8);
		vd=*(u32 *)(ibuf+i*16+12);

		fprintf(fdoa, "%08X", vd);
		fprintf(fdoa, "%08X", vc);
		fprintf(fdoa, "%08X", vb);
		fprintf(fdoa, "%08X\n", va);
	}
	
	fclose(fdoa);
//	fclose(fdob);
//	fclose(fdoc);
//	fclose(fdod);
#endif

#if 0
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
	
	sprintf(tb, "%s_2.txt", ofn_pfx);
//	fdoa=fopen("bootrom_2.txt", "wt");
	fdoa=fopen(tb, "wt");
	
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

		fprintf(fdoa, "%08X", vh);
		fprintf(fdoa, "%08X", vg);
		fprintf(fdoa, "%08X", vf);
		fprintf(fdoa, "%08X", ve);
		fprintf(fdoa, "%08X", vd);
		fprintf(fdoa, "%08X", vc);
		fprintf(fdoa, "%08X", vb);
		fprintf(fdoa, "%08X\n", va);
	}
	
	fclose(fdoa);
#endif

#if 1
	n=(sz+31)/32;
	
//	fdoa=fopen("bootrom_2a.txt", "wt");
//	fdob=fopen("bootrom_2b.txt", "wt");

	sprintf(tb, "%s_2a.txt", ofn_pfx);
	fdoa=fopen(tb, "wt");
	sprintf(tb, "%s_2b.txt", ofn_pfx);
	fdob=fopen(tb, "wt");

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

		fprintf(fdoa, "%08X", vd);
		fprintf(fdoa, "%08X", vc);
		fprintf(fdoa, "%08X", vb);
		fprintf(fdoa, "%08X\n", va);

		fprintf(fdob, "%08X", vh);
		fprintf(fdob, "%08X", vg);
		fprintf(fdob, "%08X", vf);
		fprintf(fdob, "%08X\n", ve);
	}
	
	fclose(fdoa);
	fclose(fdob);
#endif

#if 0
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


	n=(sz+15)/16;
	lva=0;
	lvb=0;
	lvc=0;
	lvd=0;
		
	for(i=0; i<n; i++)
	{
		va=*(u32 *)(ibuf+i*16+ 0);
		vb=*(u32 *)(ibuf+i*16+ 4);
		vc=*(u32 *)(ibuf+i*16+ 8);
		vd=*(u32 *)(ibuf+i*16+12);
		ve=lva>>32;
		vf=lvb>>32;
		vg=lvc>>32;
		vh=lvd>>32;
		lva=lva*0x0000F7A83CF9E588ULL+va+vf;
		lvb=lvb*0x0000FC4BA2F7ACABULL+vb+vg;
		lvc=lvc*0x0000F7A83CF9E588ULL+vc+vh;
		lvd=lvd*0x0000FC4BA2F7ACABULL+vd+va;
	}

#ifdef linux
	fd=fopen("/dev/random", "rb");
	if(fd)
	{
		fread(tb, 1, 256, fd);

		n=256/16;
		for(i=0; i<n; i++)
		{
			va=*(u32 *)(tb+i*16+ 0);
			vb=*(u32 *)(tb+i*16+ 4);
			vc=*(u32 *)(tb+i*16+ 8);
			vd=*(u32 *)(tb+i*16+12);
			ve=lva>>32;
			vf=lvb>>32;
			vg=lvc>>32;
			vh=lvd>>32;
			lva=lva*0x0000F7A83CF9E588ULL+va+vf;
			lvb=lvb*0x0000FC4BA2F7ACABULL+vb+vg;
			lvc=lvc*0x0000F7A83CF9E588ULL+vc+vh;
			lvd=lvd*0x0000FC4BA2F7ACABULL+vd+va;
		}
	}
#endif

	va=lva>>32;
	vb=lvb>>32;
	vc=lvc>>32;
	vd=lvd>>32;

//	byte	shufa1[64];
//	byte	shufb1[64];
//	byte	shufa1i[64];
//	byte	shufb1i[64];

	for(j=0; j<4; j++)
		for(i=0; i<64; i++)
			{ shufa[j][i]=i; }

	ve=va^vc;
	vf=vb^vd;
	ve=ve*0x00F7A83CU+1;
	vf=vf*0x00FC4BA2U+1;
	for(i=0; i<64; i++)
	{
		j=(ve>>24)&63;
		k=(vf>>24)&63;
		l=shufa[0][i]; shufa[0][i]=shufa[0][j]; shufa[0][j]=l;
		l=shufa[1][i]; shufa[1][i]=shufa[1][k]; shufa[1][k]=l;
		ve=ve*0x00F7A83CU+1;
		vf=vf*0x00FC4BA2U+1;
	}

	ve=ve*0x00FC4BA2U+1;
	vf=vf*0x00F7A83CU+1;
	for(i=0; i<64; i++)
	{
		j=(ve>>24)&63;
		k=(vf>>24)&63;
		l=shufa[0][i]; shufa[0][i]=shufa[0][j]; shufa[0][j]=l;
		l=shufa[1][i]; shufa[1][i]=shufa[1][k]; shufa[1][k]=l;
		ve=ve*0x00FC4BA2U+1;
		vf=vf*0x00F7A83CU+1;
	}

	for(i=0; i<64; i++)
	{
		j=shufa[0][i];
		k=shufa[1][i];
		shufa[2][j]=i;
		shufa[3][k]=i;
	}

	ve=lva^lvb;
	vf=lvb^lvc;
	vg=lvc^lvd;
	vh=lvd^lva;

	fd=fopen("ExModKrrKeys.v", "wt");
	fprintf(fd, "/* Autogenerated.  */\n");
	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY1=64'h%08X_%08X;\n", va, vb);
	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY2=64'h%08X_%08X;\n", vc, vd);
	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY3=64'h%08X_%08X;\n", ve, vf);
	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY4=64'h%08X_%08X;\n", vg, vh);
	fclose(fd);

	fd=fopen("ExModKrrFixSh.v", "wt");
	fprintf(fd, "/* Autogenerated.  */\n");
//	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY1=64'h%08X_%08X;\n", va, vb);
//	fprintf(fd, "parameter[63:0] JX2_KRR_FIXKEY2=64'h%08X_%08X;\n", vc, vd);

	fprintf(fd, "module ExModKrrFixSh(\n"
		"\tval0I, val1I, val2I, val3I,\n"
		"\tval0O, val1O, val2O, val3O);\n");
	fprintf(fd, "input[63:0] val0I;\n");
	fprintf(fd, "input[63:0] val1I;\n");
	fprintf(fd, "input[63:0] val2I;\n");
	fprintf(fd, "input[63:0] val3I;\n");
	fprintf(fd, "output[63:0] val0O;\n");
	fprintf(fd, "output[63:0] val1O;\n");
	fprintf(fd, "output[63:0] val2O;\n");
	fprintf(fd, "output[63:0] val3O;\n");
	
	for(i=0; i<4; i++)
	{
		fprintf(fd, "assign val%dO = {\n", i);
		for(j=0; j<16; j++)
		{
			fprintf(fd, "\t");
			for(k=0; k<4; k++)
			{
				l=63-(j*4+k);
				fprintf(fd, "val%dI[%2d]", i, shufa[i][l]);
//				if(l<63)
				if(l>0)
					fprintf(fd, ", ");
			}
			fprintf(fd, "\n");
		}
		fprintf(fd, "};\n");
	}
	
	fprintf(fd, "endmodule\n");

	fclose(fd);
	
	return(0);
}

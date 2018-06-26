#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#include "bt1h_targa.c"

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=(byte *)malloc(sz);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int storefile(char *name, byte *ibuf, int isz)
{
	byte *tbuf;
	FILE *fd;
	int sz;

	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("Fail Open Write %s\n", name);
		return(-1);
	}

	fwrite(ibuf, 1, isz, fd);
	fclose(fd);
	return(0);
}

// int vqenc_rgb2p64(int cr, int cg, int cb, int ctsp);
int vqenc_dyuvd(int cy, int dy, int cu, int cv, u64 *rst);
int vqdec(byte *iblk, int ystr, u64 blk, u64 *rst);

int clamp255(int clr)
{
	if(clr<0)return(0);
	if(clr>255)return(255);
	return(clr);
}

int clamp(int clr, int min, int max)
{
	if(clr<min)return(min);
	if(clr>max)return(max);
	return(clr);
}


int vqenc(byte *iblk, int ystr, u32 *oblk)
{
//	byte pxa[16*4];
//	byte pxb[16*4];
	byte pxc[16*4];
	short pxy[16];
	short pxa[16];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;

	int acr, acg, acb, acy, aca;

	int cr, cg, cb, cy, ca;
	int mr, mg, mb, my, mdy, ma;
	int nr, ng, nb, ny, ndy, na;
	int cu, cv;
	int mu, mv, nu, nv;

	int mr2, mg2, mb2, my2, mu2, mv2;
	int nr2, ng2, nb2, ny2, nu2, nv2;

//	int cy_c, cy_m, cy_y;
//	int my_c, my_m, my_y;
//	int ny_c, ny_m, ny_y;
	int ax, dy;
	int e0, e1;

	int cya, cyb, ctsp;
	int acu, acv, ady;
	int ia, ib, ix, px;
	int ixa, ixb, ixc;
	int i, j, k, l;
	
	my=256; ny=-1; ma=256; na=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0; aca=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		ca=iblk[i*ystr+j*4+3];
		cy=(cr+2*cg+cb)/4;

		k=i*4+j;
//		k=(3-i)*4+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }

		if(ca<ma) { ma=ca; }
		if(ca>na) { na=ca; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
		aca+=ca;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;
	acu=acu/16;	acv=acv/16;
	aca=aca/16;

	acy=(2*acy+my+ny)/4;

	ax=0; dy=ny-my;
	
	dy=2*(ny-acy);
	
	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	mdy=0; ndy=0;

	my2=256; ny2=-1;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		ca=iblk[i*ystr+j*4+3];

		cy=(cr+2*cg+cb)/4;
		pxy[i*4+j]=cy;
		pxa[i*4+j]=ca;
		
		if(cy<acy)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; mdy+=acy-cy; }
		else
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; ndy+=cy-acy; }

		if(cy<my2)
			{ my2=cy; mr2=cr; mg2=cg; mb2=cb; }
		if(cy>ny2)
			{ ny2=cy; nr2=cr; ng2=cg; nb2=cb; }
	}
	
	if(!cya)cya++;
	if(!cyb)cyb++;

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

	mdy=mdy/cya;
	ndy=ndy/cya;
	ady=(mdy+ndy)/2;

	mr=(mr+mr2)/2;	mg=(mg+mg2)/2;
	mb=(mb+mb2)/2;	my=(my+my2)/2;

	nr=(nr+nr2)/2;	ng=(ng+ng2)/2;
	nb=(nb+nb2)/2;	ny=(ny+ny2)/2;
	
	mu=((mb-mg)/2)+128;
	mv=((mr-mg)/2)+128;
	nu=((nb-ng)/2)+128;
	nv=((nr-ng)/2)+128;

//	acu=128;
//	acv=128;

//	dy=dy*0.75;
//	ady=(ady+3*dy)/4;
	ady=(ady+dy)/2;

	acy=clamp255(acy);	acu=clamp255(acu);
	acv=clamp255(acv);	ady=clamp255(ady);
	my=clamp255(my);	mu=clamp255(mu);	mv=clamp255(mv);
	ny=clamp255(ny);	nu=clamp255(nu);	nv=clamp255(nv);

//	my2=(my+15)>>5;	ny2=(ny+15)>>5;
//	mu2=(mu+31)>>6;	nu2=(nu+31)>>6;
//	mv2=(mv+31)>>6;	nv2=(nv+31)>>6;
//	my2=clamp(my2, 0, 7);	ny2=clamp(ny2, 0, 7);
//	mu2=clamp(mu2, 1, 3);	nu2=clamp(nu2, 1, 3);
//	mv2=clamp(mv2, 1, 3);	nv2=clamp(nv2, 1, 3);

//	ixa=0x8000|(my2<<11)|(ny2<<8)|
//		(mu2<<6)|(mv2<<4)|(nu2<<2)|(nv2<<0);

	if(aca<160)
	{
		mr2=clamp((acr+32)/64, 0, 3);
		mg2=clamp((acg+32)/64, 0, 3);
		mb2=clamp((acb+32)/64, 0, 3);

		cya=(mr2<<4)|(mg2<<2)|mb2;
		cyb=0x0F;
		ixa=0x4000|(cyb<<6)|cya;

//		cya=(5*ma+3*na)/8;
//		cyb=(3*ma+5*na)/8;
		cya=(5*0+3*255)/8;
		cyb=(3*0+5*255)/8;

		px=0;
		if(pxa[ 0]>cya)px|=0x0008;
		if(pxa[ 1]>cyb)px|=0x0004;
		if(pxa[ 2]>cya)px|=0x0002;
		if(pxa[ 3]>cyb)px|=0x0001;

		if(pxa[ 4]>cyb)px|=0x0080;
		if(pxa[ 5]>cya)px|=0x0040;
		if(pxa[ 6]>cyb)px|=0x0020;
		if(pxa[ 7]>cya)px|=0x0010;

		if(pxa[ 8]>cya)px|=0x0800;
		if(pxa[ 9]>cyb)px|=0x0400;
		if(pxa[10]>cya)px|=0x0200;
		if(pxa[11]>cyb)px|=0x0100;

		if(pxa[12]>cyb)px|=0x8000;
		if(pxa[13]>cya)px|=0x4000;
		if(pxa[14]>cyb)px|=0x2000;
		if(pxa[15]>cya)px|=0x1000;

		blk=px|(ixa<<16);

		*oblk=blk;
//		*rsta=stb;
		return(0);
	}
	
	
	mr2=clamp((mr+32)/64, 0, 3);
	mg2=clamp((mg+32)/64, 0, 3);
	mb2=clamp((mb+32)/64, 0, 3);

	nr2=clamp((nr+32)/64, 0, 3);
	ng2=clamp((ng+32)/64, 0, 3);
	nb2=clamp((nb+32)/64, 0, 3);
	
	cyb=(mr2<<4)|(mg2<<2)|mb2;
	cya=(nr2<<4)|(ng2<<2)|nb2;
	ixa=0x4000|(cyb<<6)|cya;
	
	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	px=0;
	if(pxy[ 0]>cya)px|=0x0008;
	if(pxy[ 1]>cyb)px|=0x0004;
	if(pxy[ 2]>cya)px|=0x0002;
	if(pxy[ 3]>cyb)px|=0x0001;

	if(pxy[ 4]>cyb)px|=0x0080;
	if(pxy[ 5]>cya)px|=0x0040;
	if(pxy[ 6]>cyb)px|=0x0020;
	if(pxy[ 7]>cya)px|=0x0010;

	if(pxy[ 8]>cya)px|=0x0800;
	if(pxy[ 9]>cyb)px|=0x0400;
	if(pxy[10]>cya)px|=0x0200;
	if(pxy[11]>cyb)px|=0x0100;

	if(pxy[12]>cyb)px|=0x8000;
	if(pxy[13]>cya)px|=0x4000;
	if(pxy[14]>cyb)px|=0x2000;
	if(pxy[15]>cya)px|=0x1000;

	blk=px|(ixa<<16);

	
	*oblk=blk;
//	*rsta=stb;
	return(0);
}


int main(int argc, char *argv[])
{
	char tb[256];
	FILE *ofd;
	char *ifn, *ofn, *sfx;
	char *s, *t;
	byte *ibuf;
	int xs, ys, xs2, ys2;
	u32 *bbuf;
	int x, y, n;
	int i, j, k;
	
//	ifn=argv[1];
//	ofn=argv[2];

	ifn=NULL;
	ofn=NULL;
	sfx=NULL;
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

	if(!sfx)
	{
		s=ofn+strlen(ofn);
		while((s>ofn) && *s!='/' && *s!='\\')
			s--;
		if((*s=='/') || (*s=='\\'))		s++;
		
		t=tb;
		while(*s && *s!='.')
			*t++=*s++;
		*t++=0;
		sfx=strdup(tb);
	}

	
	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);
	
	xs2=xs/4;
	ys2=ys/4;
	
	bbuf=malloc(xs2*ys2*4);
	
	for(y=0; y<ys2; y++)
		for(x=0; x<xs2; x++)
	{
		vqenc(ibuf+((y*4)*xs+(x*4))*4, xs*4, bbuf+y*xs2+x);
	}
	
	ofd=fopen(ofn, "wt");
	n=xs2*ys2;

	fprintf(ofd, "const unsigned int %s[]={\n", sfx);
	
	for(i=0; i<n; i++)
	{
		if(i && !(i&3))
			fprintf(ofd, "\n");
		fprintf(ofd, "0x%08X", bbuf[i]);
		if((i+1)<n)
			fprintf(ofd, ", ");
	}
	fprintf(ofd, "\n};\n");

	fclose(ofd);
}

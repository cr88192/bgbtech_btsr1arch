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

int vqberr(byte *blka, byte *blkb)
{
	double e;
	int dr, dg, db;
	int i;
	
	e=0;
	for(i=0; i<16; i++)
	{
		dr=blka[i*4+0]-blkb[i*4+0];
		dg=blka[i*4+1]-blkb[i*4+1];
		db=blka[i*4+2]-blkb[i*4+2];
//		e+=dr*dr+2*dg*dg+(db*db/2);
		e+=dr*dr+2*dg*dg+db*db;
	}
	
	e=sqrt(e/(3*16));
	return(e);
}

double vqierr(byte *blka, byte *blkb, int xs, int ys)
{
	double e;
	int dr, dg, db;
	int n;
	int i;
	
	e=0; n=xs*ys;
	for(i=0; i<n; i++)
	{
		dr=blka[i*4+0]-blkb[i*4+0];
		dg=blka[i*4+1]-blkb[i*4+1];
		db=blka[i*4+2]-blkb[i*4+2];
//		e+=dr*dr+2*dg*dg+(db*db/2);
		e+=dr*dr+2*dg*dg+db*db;
	}
	
	e=sqrt(e/(3*n));
	return(e);
}

int vqenc(byte *iblk, int ystr, u32 *oblk, u64 *rsta)
{
	byte pxa[16*4];
	byte pxb[16*4];
	byte pxc[16*4];
	short pxy[16];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my, mdy;
	int nr, ng, nb, ny, ndy;
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
	
	my=256; ny=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
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
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;
	acu=acu/16;	acv=acv/16;

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

		cy=(cr+2*cg+cb)/4;
		pxy[i*4+j]=cy;
		
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

	my2=(my+15)>>5;	ny2=(ny+15)>>5;
	mu2=(mu+31)>>6;	nu2=(nu+31)>>6;
	mv2=(mv+31)>>6;	nv2=(nv+31)>>6;
	my2=clamp(my2, 0, 7);	ny2=clamp(ny2, 0, 7);
	mu2=clamp(mu2, 1, 3);	nu2=clamp(nu2, 1, 3);
	mv2=clamp(mv2, 1, 3);	nv2=clamp(nv2, 1, 3);

	ixa=0x8000|(my2<<11)|(ny2<<8)|
		(mu2<<6)|(mv2<<4)|(nu2<<2)|(nv2<<0);

#if 1
	sta=*rsta;
	stb=sta;

	stc=sta;
	ixb=vqenc_dyuvd(acy, ady, acu, acv, &stc);
#endif

#if 0
//	i=acy>>4; j=dy>>4;
	i=(acy+7)>>4; j=(dy+7)>>4;
	if(i<0)i=0;
	if(j<0)j=0;
	if(i>15)i=15;
	if(j>15)j=15;

	k=acu>>5;
	l=acv>>5;
//	k=(acu+15)>>5;
//	l=(acv+15)>>5;

	if(k<1)k=1;
	if(l<1)l=1;
	if(k>7)k=7;
	if(l>7)l=7;

	ixb=0xC000|(i<<10)|(j<<6)|(k<<3)|l;
#endif

	
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

	blka=px|(ixa<<16);
	blkb=px|(ixb<<16);
	blkc=px|(ixc<<16);

	std=sta;
	vqdec(pxa, 16, blka, &std);
	std=sta;
	vqdec(pxb, 16, blkc, &std);

#if 0
	e0=vqberr(pxa, pxc);
	e1=vqberr(pxb, pxc);
	if(e1<e0)
		blka=blkc;
//	blka=blkc;

#endif

	std=sta;
	vqdec(pxa, 16, blka, &std);
	std=sta;
	vqdec(pxb, 16, blkb, &std);
	
	e0=vqberr(pxa, pxc);
	e1=vqberr(pxb, pxc);

	blk=blka;
	if(e1<=e0)
//	if(1)
	{
		blk=blkb;
		stb=stc;
	}
	
	*oblk=blk;
	*rsta=stb;
	return(0);
}



int vqenc64(byte *iblk, int ystr, u64 *oblk, u64 *rsta)
{
	static byte idxtab[16]=
		{0, 0, 0, 0,  0, 0, 2, 2,  3, 3, 1, 1,  1, 1, 1, 1};
	byte pxa[16*4];
	byte pxb[16*4];
	byte pxc[16*4];
	short pxy[16];
	
	u64 sta, stb, stc, std;
	int min, max;
	u64 blk, blka, blkb, blkc;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my, mdy;
	int nr, ng, nb, ny, ndy;
	int cu, cv;
	int mu, mv, nu, nv;
	int mr2, mg2, mb2, my2, mu2, mv2;
	int nr2, ng2, nb2, ny2, nu2, nv2;
	
	int p0, p1, p2, p3;
	int q0, q1, q2, q3;
	
	int ax, dy;
	int e0, e1;
	int cya, cyb, ctsp;
	int acu, acv, ady;
	u32 ia, ib, ix, px;
	u32 ixa, ixb, ixc;
	int i, j, k, l;
	
	my=256; ny=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		k=i*4+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;
	acu=acu/16;	acv=acv/16;

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

		cy=(cr+2*cg+cb)/4;
		pxy[i*4+j]=cy;
		
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

	my2=my>>1;	ny2=ny>>1;
	mu2=(mu+7)>>4;	nu2=(nu+7)>>4;
	mv2=(mv+7)>>4;	nv2=(nv+7)>>4;
	my2=clamp(my2, 0, 127);	ny2=clamp(ny2, 0, 127);
	mu2=clamp(mu2, 1, 15);	nu2=clamp(nu2, 1, 15);
	mv2=clamp(mv2, 1, 15);	nv2=clamp(nv2, 1, 15);

	ixa=0x80000000|(my2<<23)|(ny2<<16)|
		(mu2<<12)|(mv2<<8)|(nu2<<4)|(nv2<<0);

#if 1
	cu=acu>>1;
	cv=acv>>1;
	ixb=0xC0000000|(cu<<23)|(cv<<16)|(acy<<8)|ady;
#endif

	
//	cya=(5*my+3*ny)/8;
//	cyb=(3*my+5*ny)/8;

	cya=(9*my+7*ny)/16;
	cyb=(7*my+9*ny)/16;

	px=0;
	
	l=1024/(ny-acy+1);

	p0=idxtab[((pxy[ 0]-cya)*l+2048)>>8];
	p1=idxtab[((pxy[ 1]-cyb)*l+2048)>>8];
	p2=idxtab[((pxy[ 2]-cya)*l+2048)>>8];
	p3=idxtab[((pxy[ 3]-cyb)*l+2048)>>8];
	q0=(p0<<6)|(p1<<4)|(p2<<2)|p3;

	p0=idxtab[((pxy[ 4]-cyb)*l+2048)>>8];
	p1=idxtab[((pxy[ 5]-cya)*l+2048)>>8];
	p2=idxtab[((pxy[ 6]-cyb)*l+2048)>>8];
	p3=idxtab[((pxy[ 7]-cya)*l+2048)>>8];
	q1=(p0<<6)|(p1<<4)|(p2<<2)|p3;

	p0=idxtab[((pxy[ 8]-cya)*l+2048)>>8];
	p1=idxtab[((pxy[ 9]-cyb)*l+2048)>>8];
	p2=idxtab[((pxy[10]-cya)*l+2048)>>8];
	p3=idxtab[((pxy[11]-cyb)*l+2048)>>8];
	q2=(p0<<6)|(p1<<4)|(p2<<2)|p3;

	p0=idxtab[((pxy[12]-cyb)*l+2048)>>8];
	p1=idxtab[((pxy[13]-cya)*l+2048)>>8];
	p2=idxtab[((pxy[14]-cyb)*l+2048)>>8];
	p3=idxtab[((pxy[15]-cya)*l+2048)>>8];
	q3=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	
	px=q0|(q1<<8)|(q2<<16)|(q3<<24);

	blka=px|(((u64)ixa)<<32);
	blkb=px|(((u64)ixb)<<32);
	blkc=px|(((u64)ixc)<<32);

	std=sta;
	vqdec(pxa, 16, blka, &std);
	std=sta;
	vqdec(pxb, 16, blkc, &std);

#if 0
	e0=vqberr(pxa, pxc);
	e1=vqberr(pxb, pxc);
	if(e1<e0)
		blka=blkc;
//	blka=blkc;

#endif

	std=sta;
	vqdec(pxa, 16, blka, &std);
	std=sta;
	vqdec(pxb, 16, blkb, &std);
	
	e0=vqberr(pxa, pxc);
	e1=vqberr(pxb, pxc);

	blk=blka;
	if(e1<=e0)
//	if(1)
	{
		blk=blkb;
		stb=stc;
	}
	
	*oblk=blk;
	*rsta=stb;
	return(0);
}


#if 1
u32 vqdec_fromyuv(int cy, int cu, int cv)
{
	int cr, cg, cb;
	int cc;

//	cg=cy;
//	cg=(4*cy-(cu+cv-256))/2;
	cg=cy-(cu+cv-256)/2;
	cr=cg+(cv-128)*2;
	cb=cg+(cu-128)*2;
	cr=clamp255(cr);
	cg=clamp255(cg);
	cb=clamp255(cb);
	cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
	return(cc);
}

int vqenc_dyuvd(int cy, int dy, int cu, int cv, u64 *rst)
{
	u64 sta, stb;

	int dcy, ddy, dcu, dcv;			//deltas
	int ecy, edy, ecu, ecv;			//exps
	int pcy, pdy, pcu, pcv;			//preds
	int dcy2, ddy2, dcu2, dcv2;		//deltas
	int pcy2, pdy2, pcu2, pcv2;		//preds
	int bdcy, bddy, bdcu, bdcv;		//deltas
	int edcy, eddy, edcu, edcv;		//delta error
	int ixa, ixb, ixc;
	int i, j, k;
	
//	cu=128;
//	cv=128;

	dcy2=(cy+7)>>4;	pcy2=(dcy2<<4)|dcy2;
	ddy2=(dy+7)>>4;	pdy2=(ddy2<<4)|ddy2;
	if(dcy2>15)dcy2=15;
	if(ddy2>15)ddy2=15;

	dcu2=(cu+15)>>5;	pcu2=dcu2<<5;
	dcv2=(cv+15)>>5;	pcv2=dcv2<<5;
	if(dcu2>7)dcu2=7;
	if(dcv2>7)dcv2=7;

	k=0;
	j=pcy2-cy; j=j^(j>>31);
	if(j<edcy) k++;
	j=pdy2-dy; j=j^(j>>31);
	if(j<eddy) k++;
	j=pcu2-cu; j=j^(j>>31);
	if(j<edcu) k++;
	j=pcv2-cv; j=j^(j>>31);
	if(j<edcv) k++;
	
	if(1)
	{
		ixa=0xC000|(dcy2<<10)|(ddy2<<6)|(dcu2<<3)|(dcv2<<0);

		k=5;
		i=pcy2|(pdy2<<8)|(pcu2<<16)|(pcv2<<24);
		j=(k<<0)|(k<<4)|(k<<8)|(k<<12);
		stb=((u32)i)|(((u64)j)<<32);
	}

	if(rst)
		*rst=stb;
	
	return(ixa);
}

int vqdec(byte *iblk, int ystr, u64 blk, u64 *rst)
{
	u32 *ct0, *ct1, *ct2, *ct3;
	u32 tclra[4];
	u32 clra, clrb, clrc, clrd, px32, blkh;
	u64 sta, stb;
	int dcy, ddy, dcu, dcv;		//deltas
	int ecy, edy, ecu, ecv;		//exps
	int pcy, pdy, pcu, pcv;		//preds
	int mcy, ncy, mcu, ncu, mcv, ncv;
	int cta, ctb, ctsp;
	int i, j, k;

	if(rst)
		sta=*rst;
	else
		sta=0;
	stb=sta;

	px32=0;

	blkh=blk>>32;
	if(blkh)
	{
		px32=blk;
		switch((blkh>>30)&3)
		{
		case 2:
			dcy=(blkh>>23)&127;	ddy=(blkh>>16)&127;
			mcy=(dcy<<1)|(dcy>>6);
			ncy=(ddy<<1)|(ddy>>6);
			dcu=(blkh>>12)&15;	dcv=(blkh>>8)&15;
			mcu=dcu<<4;			mcv=dcv<<4;
			dcu=(blkh>>4)&15;	dcv=(blkh>>0)&15;
			ncu=dcu<<4;			ncv=dcv<<4;
			break;
		case 3:
			dcy=(blkh>>8)&255;	ddy=(blkh>> 0)&255;
			dcu=(blkh>>23)&127;	dcv=(blkh>>16)&127;
			pcy=dcy;			pdy=ddy;
			pcu=dcu<<1;			pcv=dcv<<1;
			mcy=pcy-(pdy>>1);	ncy=mcy+pdy;			
			mcu=pcu; mcv=pcv;	ncu=pcu; ncv=pcv;
			break;
		}
	}else
	{
		switch((blk>>30)&3)
		{
		case 0:
		case 1:
			mcy=(sta    )&255;	pcy=(sta>> 8)&255;
			mcu=(sta>>16)&255;	mcv=(sta>>24)&255;
			ncy=(sta    )&255;	pdy=(sta>> 8)&255;
			ncu=(sta>>16)&255;	ncv=(sta>>24)&255;
			px32=blk;
			break;

		case 2:
			dcy=(blk>>27)&7;		ddy=(blk>>24)&7;
			mcy=(dcy<<5)|(dcy<<2)|(dcy>>1);
			ncy=(ddy<<5)|(ddy<<2)|(ddy>>1);
			dcu=(blk>>22)&3;	dcv=(blk>>20)&3;
			mcu=dcu<<6;			mcv=dcv<<6;
			dcu=(blk>>18)&3;	dcv=(blk>>16)&3;
			ncu=dcu<<6;			ncv=dcv<<6;
			break;

		case 3:
			dcy=(blk>>26)&15;	ddy=(blk>>22)&15;
			dcu=(blk>>19)&7;	dcv=(blk>>16)&7;
			pcy=(dcy<<4)|dcy;	pdy=(ddy<<4)|ddy;
			pcu=dcu<<5;			pcv=dcv<<5;
			mcy=pcy-(pdy>>1);	ncy=mcy+pdy;			
			mcu=pcu; mcv=pcv;	ncu=pcu; ncv=pcv;
			break;
		}
	}

	pcy=(mcy+ncy)>>1;
	pdy=ncy-mcy;
	i=mcy|(pcy<<8)|(mcu<<16)|(mcv<<24);
	j=ncy|(pdy<<8)|(ncu<<16)|(ncv<<24);
	stb=((u32)i)|(((u64)j)<<32);

	if(rst)
		*rst=stb;

	ct0=(u32 *)(iblk+0*ystr);
	ct1=(u32 *)(iblk+1*ystr);
	ct2=(u32 *)(iblk+2*ystr);
	ct3=(u32 *)(iblk+3*ystr);

	clra=vqdec_fromyuv(mcy, mcu, mcv);
	clrb=vqdec_fromyuv(ncy, ncu, ncv);
	ecy=(3*mcy+ncy)/4;	ecu=(3*mcu+ncu)/4;	ecv=(3*mcv+ncv)/4;
	clrc=vqdec_fromyuv(ecy, ecu, ecv);
	ecy=(3*ncy+mcy)/4;	ecu=(3*ncu+mcu)/4;	ecv=(3*ncv+mcv)/4;
	clrd=vqdec_fromyuv(ecy, ecu, ecv);

	tclra[0]=clra;	tclra[1]=clrb;
	tclra[2]=clrc;	tclra[3]=clrd;

	if(px32)
	{
		ct3[0]=tclra[(px32>>30)&3];		ct3[1]=tclra[(px32>>28)&3];
		ct3[2]=tclra[(px32>>26)&3];		ct3[3]=tclra[(px32>>24)&3];
		ct2[0]=tclra[(px32>>22)&3];		ct2[1]=tclra[(px32>>20)&3];
		ct2[2]=tclra[(px32>>18)&3];		ct2[3]=tclra[(px32>>16)&3];
		ct1[0]=tclra[(px32>>14)&3];		ct1[1]=tclra[(px32>>12)&3];
		ct1[2]=tclra[(px32>>10)&3];		ct1[3]=tclra[(px32>> 8)&3];
		ct0[0]=tclra[(px32>> 6)&3];		ct0[1]=tclra[(px32>> 4)&3];
		ct0[2]=tclra[(px32>> 2)&3];		ct0[3]=tclra[(px32>> 0)&3];
	}else
	{
		ct3[0]=tclra[(px32>>15)&1];		ct3[1]=tclra[(px32>>14)&1];
		ct3[2]=tclra[(px32>>13)&1];		ct3[3]=tclra[(px32>>12)&1];
		ct2[0]=tclra[(px32>>11)&1];		ct2[1]=tclra[(px32>>10)&1];
		ct2[2]=tclra[(px32>> 9)&1];		ct2[3]=tclra[(px32>> 8)&1];
		ct1[0]=tclra[(px32>> 7)&1];		ct1[1]=tclra[(px32>> 6)&1];
		ct1[2]=tclra[(px32>> 5)&1];		ct1[3]=tclra[(px32>> 4)&1];
		ct0[0]=tclra[(px32>> 3)&1];		ct0[1]=tclra[(px32>> 2)&1];
		ct0[2]=tclra[(px32>> 1)&1];		ct0[3]=tclra[(px32>> 0)&1];
	}
}
#endif

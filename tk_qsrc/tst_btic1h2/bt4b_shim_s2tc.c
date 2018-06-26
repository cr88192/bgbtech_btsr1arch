/*
 * TBD: Fast Shim decoder to S2TC.
 * Goal will be to quickly shim MetaBlock2 blocks to analogous S2TC blocks.
 * Conversion Speed will be the priority.
 */

#define BTIC4B_ENABLE_S3TC

u32 btic4b_s2tc_2x2to4x4[256];
byte btic4b_s2tc_4x1to4x1a[256];
byte btic4b_s2tc_4x1to4x1b[256];
byte btic4b_s2tc_4x1to4x1c[256];
byte btic4b_s2tc_4x1x1to4x1[16];
byte btic4b_s2tc_4x1x3to4x1a[4096];
byte btic4b_s2tc_4x1x3to4x1b[4096];
u16 btic4b_s2tc_4x1x3to4x1x3[4096];
u16 btic4b_s2tc_4x1x2to4x1x3[256];
byte btic4b_s2tc_2x1x4to2x1[256];
byte btic4b_s2tc_2x1x4to2x1x3[256];

#ifdef BTIC4B_ENABLE_S3TC
static byte btic4b_s2tc_idxmap2[4]={0, 2, 3, 1};
// static byte btic4b_s2tc_idxmap2[4]={1, 3, 2, 0};
static byte btic4b_s2tc_idxmap2b[4]={0, 0, 1, 1};
static byte btic4b_s2tc_idxmap2to3[4]={0, 3, 6, 1};
static byte btic4b_s2tc_idxmap3[8]= {0, 2, 3, 4, 5, 6, 7, 1};
static byte btic4b_s2tc_idxmap3to2[8]= {0, 0, 2, 2, 3, 3, 1, 1};
static byte btic4b_s2tc_idxmap3to2a[8]={0, 2, 2, 3, 3, 1, 1, 1};
static byte btic4b_s2tc_idxmap3to2b[8]={0, 0, 0, 2, 2, 3, 3, 1};

//static byte btic4b_s2tc_idxmap4to3[16]={
//	0, 0, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 1, 1};
#endif

void BTIC4B_ConvBlockS2TC_Init()
{
	static int init=0;
	int p0, p1, p2, p3;
	int q0, q1, q2, q3;
	int r0, r1, r2, r3;
	int i, j, k;
	
	if(init)return;
	init=1;
	
	for(i=0; i<256; i++)
	{
		p3=(i>>6)&3;	p2=(i>>4)&3;
		p1=(i>>2)&3;	p0=(i>>0)&3;

#ifdef BTIC4B_ENABLE_S3TC
		q0=btic4b_s2tc_idxmap2[p0];		q1=btic4b_s2tc_idxmap2[p0];
		q2=btic4b_s2tc_idxmap2[p1];		q3=btic4b_s2tc_idxmap2[p1];
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);
		r1=q1|(q0<<2)|(q3<<4)|(q2<<6);

		q0=btic4b_s2tc_idxmap2[p2];		q1=btic4b_s2tc_idxmap2[p2];
		q2=btic4b_s2tc_idxmap2[p3];		q3=btic4b_s2tc_idxmap2[p3];
		r2=q0|(q1<<2)|(q2<<4)|(q3<<6);
		r3=q1|(q0<<2)|(q3<<4)|(q2<<6);
#else
		q0=(p0>2);		q1=(p0>1);
		q2=(p1>2);		q3=(p1>1);
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);
		r1=q1|(q0<<2)|(q3<<4)|(q2<<6);

		q0=(p2>2);		q1=(p2>1);
		q2=(p3>2);		q3=(p3>1);
		r2=q0|(q1<<2)|(q2<<4)|(q3<<6);
		r3=q1|(q0<<2)|(q3<<4)|(q2<<6);
#endif

		j=r0|(r1<<8)|(r2<<16)|(r3<<24);
		btic4b_s2tc_2x2to4x4[i]=j;
	}

	for(i=0; i<256; i++)
	{
		p3=(i>>6)&3;	p2=(i>>4)&3;
		p1=(i>>2)&3;	p0=(i>>0)&3;

#ifdef BTIC4B_ENABLE_S3TC
		q0=btic4b_s2tc_idxmap2[p0];		q1=btic4b_s2tc_idxmap2[p1];
		q2=btic4b_s2tc_idxmap2[p2];		q3=btic4b_s2tc_idxmap2[p3];
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);
		btic4b_s2tc_4x1to4x1a[i]=r0;
		btic4b_s2tc_4x1to4x1b[i]=r0;

		q0=btic4b_s2tc_idxmap2b[p0];	q1=btic4b_s2tc_idxmap2b[p1];
		q2=btic4b_s2tc_idxmap2b[p2];	q3=btic4b_s2tc_idxmap2b[p3];
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);
		btic4b_s2tc_4x1to4x1c[i]=r0;
#else
		q0=(p0>2);		q1=(p1>1);
		q2=(p2>2);		q3=(p3>1);
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);

		q0=(p0>1);		q1=(p1>2);
		q2=(p2>1);		q3=(p3>2);
		r1=q0|(q1<<2)|(q2<<4)|(q3<<6);
		
		r2=r0;

		btic4b_s2tc_4x1to4x1a[i]=r0;
		btic4b_s2tc_4x1to4x1b[i]=r1;
		btic4b_s2tc_4x1to4x1c[i]=r2;
#endif
	}

	for(i=0; i<16; i++)
	{
		p3=(i>>3)&1;	p2=(i>>2)&1;
		p1=(i>>1)&1;	p0=(i   )&1;

		q0=p0|(p1<<2)|(p2<<4)|(p3<<6);
		btic4b_s2tc_4x1x1to4x1[i]=q0;
	}

	for(i=0; i<4096; i++)
	{
		p3=(i>>9)&7;	p2=(i>>6)&7;
		p1=(i>>3)&7;	p0=(i>>0)&7;

#ifdef BTIC4B_ENABLE_S3TC
		q0=btic4b_s2tc_idxmap3to2b[p0];	q1=btic4b_s2tc_idxmap3to2a[p1];
		q2=btic4b_s2tc_idxmap3to2b[p2];	q3=btic4b_s2tc_idxmap3to2a[p3];
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);

		q0=btic4b_s2tc_idxmap3to2a[p0];	q1=btic4b_s2tc_idxmap3to2b[p1];
		q2=btic4b_s2tc_idxmap3to2a[p2];	q3=btic4b_s2tc_idxmap3to2b[p3];
		r1=q0|(q1<<2)|(q2<<4)|(q3<<6);

		btic4b_s2tc_4x1x3to4x1a[i]=r0;
		btic4b_s2tc_4x1x3to4x1b[i]=r1;
#else
		q0=(p0>6);		q1=(p1>2);
		q2=(p2>6);		q3=(p3>2);
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);

		q0=(p0>2);		q1=(p1>6);
		q2=(p2>2);		q3=(p3>6);
		r1=q0|(q1<<2)|(q2<<4)|(q3<<6);

		btic4b_s2tc_4x1x3to4x1a[i]=r0;
		btic4b_s2tc_4x1x3to4x1b[i]=r1;
#endif
	}

	for(i=0; i<4096; i++)
	{
		p3=(i>>9)&7;	p2=(i>>6)&7;
		p1=(i>>3)&7;	p0=(i>>0)&7;

#ifdef BTIC4B_ENABLE_S3TC
		p0=btic4b_s2tc_idxmap3[p0];	p1=btic4b_s2tc_idxmap3[p1];
		p2=btic4b_s2tc_idxmap3[p2];	p3=btic4b_s2tc_idxmap3[p3];
		q0=p0|(p1<<3)|(p2<<6)|(p3<<9);
		btic4b_s2tc_4x1x3to4x1x3[i]=q0;
#else
		p0=(p0>=4);		p1=(p1>=4);
		p2=(p2>=4);		p3=(p3>=4);
		q0=p0|(p1<<3)|(p2<<6)|(p3<<9);
		btic4b_s2tc_4x1x3to4x1x3[i]=q0;
#endif
	}

	for(i=0; i<256; i++)
	{
		p3=(i>>6)&3;	p2=(i>>4)&3;
		p1=(i>>2)&3;	p0=(i>>0)&3;

#ifdef BTIC4B_ENABLE_S3TC
		p0=btic4b_s2tc_idxmap2to3[p0];	p1=btic4b_s2tc_idxmap2to3[p1];
		p2=btic4b_s2tc_idxmap2to3[p2];	p3=btic4b_s2tc_idxmap2to3[p3];
		q0=p0|(p1<<3)|(p2<<6)|(p3<<9);
		btic4b_s2tc_4x1x2to4x1x3[i]=q0;
#else
		p0=(p0>=2);		p1=(p1>=2);
		p2=(p2>=2);		p3=(p3>=2);
		q0=p0|(p1<<3)|(p2<<6)|(p3<<9);
		btic4b_s2tc_4x1x2to4x1x3[i]=q0;
#endif
	}
	
	for(i=0; i<256; i++)
	{
		p1=(i>>4)&15;	p0=(i>>0)&15;
//		p0=(p0>=8);		p1=(p1>=8);
//		q0=p0|(p1<<2);

		p2=btic4b_s2tc_idxmap3to2[p0>>1];
		p3=btic4b_s2tc_idxmap3to2[p1>>1];
		q0=p2|(p3<<2);
		btic4b_s2tc_2x1x4to2x1[i]=q0;

		p2=btic4b_s2tc_idxmap3[p0>>1];
		p3=btic4b_s2tc_idxmap3[p1>>1];
		q0=p2|(p3<<3);
		btic4b_s2tc_2x1x4to2x1x3[i]=q0;
	}

}

void BTIC4B_S2TC_DecodeBlockRGBA(byte *block,
	byte *rgba, int xstride, int ystride, int pfb)
{
	int clri[4];
	byte *clr, *ct;
	int *cti;
	int i0, i1, i2, i3;
	int cxa, cxb, px;
	int cra, cga, cba, crb, cgb, cbb;
	
	clr=(byte *)clri;
	cxa=block[0]|(block[1]<<8);
	cxb=block[2]|(block[3]<<8);
	
	cra=(cxa>>8)&0xF8; cra=cra|(cra>>5);
	cga=(cxa>>3)&0xFC; cga=cga|(cga>>6);
	cba=(cxa<<3)&0xF8; cba=cba|(cba>>5);
	crb=(cxb>>8)&0xF8; crb=crb|(crb>>5);
	cgb=(cxb>>3)&0xFC; cgb=cgb|(cgb>>6);
	cbb=(cxb<<3)&0xF8; cbb=cbb|(cbb>>5);
	
	clr[0]=cra;	clr[1]=cga;
	clr[2]=cba;	clr[3]=255;
	clr[4]=crb;	clr[5]=cgb;
	clr[6]=cbb;	clr[7]=255;

#ifdef BTIC4B_ENABLE_S3TC
	if(cxa>cxb)
	{
		clr[ 8]=(cra*11+crb*5)>>4;
		clr[ 9]=(cga*11+cgb*5)>>4;
		clr[10]=(cba*11+cbb*5)>>4;
		clr[11]=255;
		clr[12]=(cra*5+crb*11)>>4;
		clr[13]=(cga*5+cgb*11)>>4;
		clr[14]=(cba*5+cbb*11)>>4;
		clr[15]=255;
	}else
	{
		clr[ 8]=(cra+crb)>>1;
		clr[ 9]=(cga+cgb)>>1;
		clr[10]=(cba+cbb)>>1;
		clr[11]=255;
		clr[12]=0;
		clr[13]=0;
		clr[14]=0;
		clr[15]=0;
	}
#else
	clri[2]=(clri[0]+clri[1])>>1;
	clri[3]=0;
#endif

//	clri[2]=(clri[0]+clri[1])>>1;
//	clri[3]=0;
	
	if(xstride==4)
	{
		cti=(int *)(rgba+0); px=block[4];
		cti[0]=clri[(px   )&3];	cti[1]=clri[(px>>2)&3];
		cti[2]=clri[(px>>4)&3];	cti[3]=clri[(px>>6)&3];
		cti=(int *)(rgba+ystride); px=block[5];
		cti[0]=clri[(px   )&3];	cti[1]=clri[(px>>2)&3];
		cti[2]=clri[(px>>4)&3];	cti[3]=clri[(px>>6)&3];
		cti=(int *)(rgba+2*ystride); px=block[6];
		cti[0]=clri[(px   )&3];	cti[1]=clri[(px>>2)&3];
		cti[2]=clri[(px>>4)&3];	cti[3]=clri[(px>>6)&3];
		cti=(int *)(rgba+3*ystride); px=block[7];
		cti[0]=clri[(px   )&3];	cti[1]=clri[(px>>2)&3];
		cti[2]=clri[(px>>4)&3];	cti[3]=clri[(px>>6)&3];
		return;
	}else if(xstride==3)
	{
		ct=rgba+0; px=block[4];
		i0=clri[(px   )&3];		i1=clri[(px>>2)&3];
		i2=clri[(px>>4)&3];		i3=clri[(px>>6)&3];
		ct[0]=i0; ct[ 1]=i0>>8; ct[ 2]=i0>>16;
		ct[3]=i1; ct[ 4]=i1>>8; ct[ 5]=i1>>16;
		ct[6]=i2; ct[ 7]=i2>>8; ct[ 8]=i2>>16;
		ct[9]=i3; ct[10]=i3>>8; ct[11]=i3>>16;
		ct=rgba+ystride; px=block[5];
		i0=clri[(px   )&3];		i1=clri[(px>>2)&3];
		i2=clri[(px>>4)&3];		i3=clri[(px>>6)&3];
		ct[0]=i0; ct[ 1]=i0>>8; ct[ 2]=i0>>16;
		ct[3]=i1; ct[ 4]=i1>>8; ct[ 5]=i1>>16;
		ct[6]=i2; ct[ 7]=i2>>8; ct[ 8]=i2>>16;
		ct[9]=i3; ct[10]=i3>>8; ct[11]=i3>>16;
		ct=rgba+2*ystride; px=block[6];
		i0=clri[(px   )&3];		i1=clri[(px>>2)&3];
		i2=clri[(px>>4)&3];		i3=clri[(px>>6)&3];
		ct[0]=i0; ct[ 1]=i0>>8; ct[ 2]=i0>>16;
		ct[3]=i1; ct[ 4]=i1>>8; ct[ 5]=i1>>16;
		ct[6]=i2; ct[ 7]=i2>>8; ct[ 8]=i2>>16;
		ct[9]=i3; ct[10]=i3>>8; ct[11]=i3>>16;
		ct=rgba+3*ystride; px=block[7];
		i0=clri[(px   )&3];		i1=clri[(px>>2)&3];
		i2=clri[(px>>4)&3];		i3=clri[(px>>6)&3];
		ct[0]=i0; ct[ 1]=i0>>8; ct[ 2]=i0>>16;
		ct[3]=i1; ct[ 4]=i1>>8; ct[ 5]=i1>>16;
		ct[6]=i2; ct[ 7]=i2>>8; ct[ 8]=i2>>16;
		ct[9]=i3; ct[10]=i3>>8; ct[11]=i3>>16;
		return;
	}
}

void BTIC4B_S2TC_DecodeImage(byte *block, int blkstride,
	byte *rgba, int xs, int ys, int stride, int pfb)
{
	byte *rgba1;
	int xstr, ystr;
	int xs1, ys1, xs2, ys2;
	int i, j;
	
	if(pfb==3)
	{
		BTIC4B_S2TC_DecodeImage(block+8, blkstride,
			rgba, xs, ys, stride, 0);
		return;
	}
	
	xstr=stride; ystr=xs*stride; rgba1=rgba;
	if(ys<0)
	{
		ys=-ys;
		xstr=stride; ystr=-xs*stride;
		rgba1=rgba+(ys-1)*xs*stride;
	}
	
	xs1=xs>>2; ys1=ys>>2;
	xs2=(xs+3)>>2; ys2=(ys+3)>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC4B_S2TC_DecodeBlockRGBA(
			block+(i*xs2+j)*blkstride,
			rgba1+i*4*ystr+j*4*xstr,
			xstr, ystr, pfb);
	}
}

force_inline u16 btic4b_s2tc_packrgb(int cr, int cg, int cb)
{
	u16 i;

	cr+=3;
	cg+=1;
	cb+=3;

	if((cr|cg|cb)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
	}
	
	i=  ((cr<<8)&0xF800) |
		((cg<<3)&0x07E0) |
		((cb>>3)&0x001F);
	return(i);
}

void BTIC4B_S2TC_EncodeBlockI(byte *block,
	s16 *pxy,
	int *min, int *max,
	int mcy, int ncy)
{
	int min2[4], max2[4];
	int avg[4];
	int p0, p1, p2, p3;
	int acy, acya, acyb;
	int pxa, pxb;
	
	acy=(mcy+ncy)>>1;
//	acya=(acy+mcy)>>1;
//	acyb=(acy+ncy)>>1;
	acya=(3*acy+mcy)>>2;
	acyb=(3*acy+ncy)>>2;

	pxa=btic4b_s2tc_packrgb(min[0], min[1], min[2]);
	pxb=btic4b_s2tc_packrgb(max[0], max[1], max[2]);
	block[0]=pxa;	block[1]=pxa>>8;
	block[2]=pxb;	block[3]=pxb>>8;

	p0=((pxy[ 0]-acya)>>31)&1;	p1=((pxy[ 1]-acyb)>>31)&1;
	p2=((pxy[ 2]-acya)>>31)&1;	p3=((pxy[ 3]-acyb)>>31)&1;
	block[4]=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[ 4]-acyb)>>31)&1;	p1=((pxy[ 5]-acya)>>31)&1;
	p2=((pxy[ 6]-acyb)>>31)&1;	p3=((pxy[ 7]-acya)>>31)&1;
	block[5]=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[ 8]-acya)>>31)&1;	p1=((pxy[ 9]-acyb)>>31)&1;
	p2=((pxy[10]-acya)>>31)&1;	p3=((pxy[11]-acyb)>>31)&1;
	block[6]=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[12]-acyb)>>31)&1;	p1=((pxy[13]-acya)>>31)&1;
	p2=((pxy[14]-acyb)>>31)&1;	p3=((pxy[15]-acya)>>31)&1;
	block[7]=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;

//	if(pxa>pxb)
	if(pxa<pxb)
	{
		block[0]=pxb;		block[1]=pxb>>8;
		block[2]=pxa;		block[3]=pxa>>8;
		block[4]^=0x55;		block[5]^=0x55;
		block[6]^=0x55;		block[7]^=0x55;
	}
}

void BTIC4B_S3TC_EncodeBlockI(byte *block,
	s16 *pxy,
	int *min, int *max,
	int mcy, int ncy)
{
	static byte tab[8]={0,0,0,2,3,1,1,1};
	int min2[4], max2[4];
	int avg[4];
	int l0, l0a, l0b;
	int p0, p1, p2, p3;
	int acy, acya, acyb;
	int cxa, cxb;
	
	avg[0]=(min[0]+max[0])>>1;
	avg[1]=(min[1]+max[1])>>1;
	avg[2]=(min[2]+max[2])>>1;

	min2[0]=(3*min[0]+avg[0])>>2;
	min2[1]=(3*min[1]+avg[1])>>2;
	min2[2]=(3*min[2]+avg[2])>>2;
	max2[0]=(3*max[0]+avg[0])>>2;
	max2[1]=(3*max[1]+avg[1])>>2;
	max2[2]=(3*max[2]+avg[2])>>2;

#if 0
	if((min2[0]|min2[1]|min2[2]|max2[0]|max2[1]|max2[2])>>8)
	{
		min2[0]=lqtvq_clamp255(min2[0]);
		min2[1]=lqtvq_clamp255(min2[1]);
		min2[2]=lqtvq_clamp255(min2[2]);
		max2[0]=lqtvq_clamp255(max2[0]);
		max2[1]=lqtvq_clamp255(max2[1]);
		max2[2]=lqtvq_clamp255(max2[2]);
	}
#endif

	acy=(mcy+ncy)>>1;
	acya=(3*acy+mcy)>>2;
	acyb=(3*acy+ncy)>>2;

//	cxa=((min2[0]<<8)&0xF800)|
//		((min2[1]<<3)&0x07E0)|
//		((min2[2]>>3)&0x001F);
//	cxb=((max2[0]<<8)&0xF800)|
//		((max2[1]<<3)&0x07E0)|
//		((max2[2]>>3)&0x001F);

	cxa=btic4b_s2tc_packrgb(min2[0], min2[1], min2[2]);
	cxb=btic4b_s2tc_packrgb(max2[0], max2[1], max2[2]);

	block[0]=cxa;	block[1]=cxa>>8;
	block[2]=cxb;	block[3]=cxb>>8;

	l0=8388608/(ncy-acy+1);
	l0a=16777216+4096;
	l0b=16777216-4096;

	p0=tab[((pxy[ 0]-acy)*l0+l0a)>>22];
	p1=tab[((pxy[ 1]-acy)*l0+l0b)>>22];
	p2=tab[((pxy[ 2]-acy)*l0+l0a)>>22];
	p3=tab[((pxy[ 3]-acy)*l0+l0b)>>22];
	block[4]=p0|(p1<<2)|(p2<<4)|(p3<<6);
	p0=tab[((pxy[ 4]-acy)*l0+l0b)>>22];
	p1=tab[((pxy[ 5]-acy)*l0+l0a)>>22];
	p2=tab[((pxy[ 6]-acy)*l0+l0b)>>22];
	p3=tab[((pxy[ 7]-acy)*l0+l0a)>>22];
	block[5]=p0|(p1<<2)|(p2<<4)|(p3<<6);
	p0=tab[((pxy[ 8]-acy)*l0+l0a)>>22];
	p1=tab[((pxy[ 9]-acy)*l0+l0b)>>22];
	p2=tab[((pxy[10]-acy)*l0+l0a)>>22];
	p3=tab[((pxy[11]-acy)*l0+l0b)>>22];
	block[6]=p0|(p1<<2)|(p2<<4)|(p3<<6);
	p0=tab[((pxy[12]-acy)*l0+l0b)>>22];
	p1=tab[((pxy[13]-acy)*l0+l0a)>>22];
	p2=tab[((pxy[14]-acy)*l0+l0b)>>22];
	p3=tab[((pxy[15]-acy)*l0+l0a)>>22];
	block[7]=p0|(p1<<2)|(p2<<4)|(p3<<6);

	if(cxb>=cxa)
	{
		if(cxb==cxa)
		{
			cxb=cxb--;
			if(cxb<0)cxb=0;
			block[0]=cxa;		block[1]=cxa>>8;
			block[2]=cxb;		block[3]=cxb>>8;
			return;
		}

		block[0]=cxb;		block[1]=cxb>>8;
		block[2]=cxa;		block[3]=cxa>>8;
		block[4]^=0x55;
		block[5]^=0x55;
		block[6]^=0x55;
		block[7]^=0x55;
	}
}

void BTIC4B_S2TC_EncodeBlockAlphaI(byte *block,
	s16 *pxy, int mcy, int ncy)
{
	int min2[4], max2[4];
	int avg[4];
	int p0, p1, p2, p3;
	int q0, q1, q2, q3;
	int acy, acya, acyb;
	int pxa, pxb;
	
	acy=(mcy+ncy)>>1;
	acya=(3*acy+mcy)>>2;
	acyb=(3*acy+ncy)>>2;

//	block[0]=mcy;
//	block[1]=ncy;
	block[0]=lqtvq_clamp255(mcy);
	block[1]=lqtvq_clamp255(ncy);

	p0=((pxy[ 0]-acya)>>31)&1;	p1=((pxy[ 1]-acyb)>>31)&1;
	p2=((pxy[ 2]-acya)>>31)&1;	p3=((pxy[ 3]-acyb)>>31)&1;
	q0=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[ 4]-acyb)>>31)&1;	p1=((pxy[ 5]-acya)>>31)&1;
	p2=((pxy[ 6]-acyb)>>31)&1;	p3=((pxy[ 7]-acya)>>31)&1;
	q1=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[ 8]-acya)>>31)&1;	p1=((pxy[ 9]-acyb)>>31)&1;
	p2=((pxy[10]-acya)>>31)&1;	p3=((pxy[11]-acyb)>>31)&1;
	q2=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;
	p0=((pxy[12]-acyb)>>31)&1;	p1=((pxy[13]-acya)>>31)&1;
	p2=((pxy[14]-acyb)>>31)&1;	p3=((pxy[15]-acya)>>31)&1;
	q3=(p0|(p1<<2)|(p2<<4)|(p3<<6))^0x55;

	q0=btic4b_s2tc_4x1x2to4x1x3[q0];
	q1=btic4b_s2tc_4x1x2to4x1x3[q1];
	q2=btic4b_s2tc_4x1x2to4x1x3[q2];
	q3=btic4b_s2tc_4x1x2to4x1x3[q3];

	btic4b_setu16le(block+ 2, q0|(q1<<12));
	btic4b_setu32le(block+ 4, (q1>>4)|(q2<<8)|(q3<<20));
}

void BTIC4B_S3TC_EncodeBlockAlphaI(byte *block,
	s16 *pxa, int mca, int nca)
{
	static byte tab[32]={
		1,1,1,1, 1,1,1,1, 1,1,1,1,
		1,7,6,5, 4,3,2,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0};
	int min2[4], max2[4];
	int avg[4];
	int p0, p1, p2, p3;
	int aca, acaa, acab;
	int cxa, cxb;
	int mca2, nca2;
	int l0, l0a, l0b;
	
	if(mca>nca)
		{ p0=nca; nca=mca; mca=p0; }
	
	aca=(mca+nca)>>1;
	acaa=(aca+mca)>>1;
	acab=(aca+nca)>>1;

	mca2=(7*mca+aca)/8;
	nca2=(7*nca+aca)/8;

	if((mca2|nca2)>>8)
	{
		mca2=lqtvq_clamp255(mca2);
		nca2=lqtvq_clamp255(nca2);
	}

	block[0]=nca2;	block[1]=mca2;

	if(mca2==nca2)
	{
		block[2]=0;	block[3]=0;
		block[4]=0;	block[5]=0;
		block[6]=0;	block[7]=0;
		return;
	}

	l0=(8388608-16384)/(nca-aca+1);
	l0a=33554432+4096;
	l0b=33554432-4096;

	p0=tab[(((pxa[ 0]-aca)*l0+l0a)>>21)&31];
	p1=tab[(((pxa[ 1]-aca)*l0+l0b)>>21)&31];
	p2=tab[(((pxa[ 2]-aca)*l0+l0a)>>21)&31];
	p3=tab[(((pxa[ 3]-aca)*l0+l0b)>>21)&31];
	cxa=p0|(p1<<3)|(p2<<6)|(p3<<9);
	p0=tab[(((pxa[ 4]-aca)*l0+l0b)>>21)&31];
	p1=tab[(((pxa[ 5]-aca)*l0+l0a)>>21)&31];
	p2=tab[(((pxa[ 6]-aca)*l0+l0b)>>21)&31];
	p3=tab[(((pxa[ 7]-aca)*l0+l0a)>>21)&31];
	cxb=p0|(p1<<3)|(p2<<6)|(p3<<9);

	cxa=cxa|(cxb<<12);
	block[2]=cxa;
	block[3]=cxa>>8;
	block[4]=cxa>>16;

	p0=tab[(((pxa[ 8]-aca)*l0+l0a)>>21)&31];
	p1=tab[(((pxa[ 9]-aca)*l0+l0b)>>21)&31];
	p2=tab[(((pxa[10]-aca)*l0+l0a)>>21)&31];
	p3=tab[(((pxa[11]-aca)*l0+l0b)>>21)&31];
	cxa=p0|(p1<<3)|(p2<<6)|(p3<<9);
	p0=tab[(((pxa[12]-aca)*l0+l0b)>>21)&31];
	p1=tab[(((pxa[13]-aca)*l0+l0a)>>21)&31];
	p2=tab[(((pxa[14]-aca)*l0+l0b)>>21)&31];
	p3=tab[(((pxa[15]-aca)*l0+l0a)>>21)&31];
	cxb=p0|(p1<<3)|(p2<<6)|(p3<<9);

	cxa=cxa|(cxb<<12);
	block[5]=cxa;
	block[6]=cxa>>8;
	block[7]=cxa>>16;
}


void BTIC4B_BC1_EncodeBlock(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
//	BTIC4B_S2TC_EncodeBlockI(block, pxy, min, max, mcy, ncy);
	BTIC4B_S3TC_EncodeBlockI(block, pxy, min, max, mcy, ncy);
}

void BTIC4B_BC3_EncodeBlock(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
//	BTIC4B_S2TC_EncodeBlockAlphaI(block, pxa, mca, nca);
//	BTIC4B_S2TC_EncodeBlockI(block+8, pxy, min, max, mcy, ncy);

	BTIC4B_S3TC_EncodeBlockAlphaI(block, pxa, mca, nca);
	BTIC4B_S3TC_EncodeBlockI(block+8, pxy, min, max, mcy, ncy);
}

void BTIC4B_BC4_EncodeBlock(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
	BTIC4B_S3TC_EncodeBlockAlphaI(block, pxy, mcy, ncy);
}

void BTIC4B_BC5_EncodeBlock(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
	s16 pxr[16], pxg[16];
	int mcr, ncr, mcg, ncg;
	int acy;
	int cr, cg;
	int l0, l0b;
	int i, j, k;
	
	acy=(mcy+ncy)/2;
	l0=8388608/(ncy-acy+1);
	l0b=8388608;

	mcr=32767; mcg=32767;
	ncr=-32767; ncg=-32767;
	for(i=0; i<16; i++)
	{
		j=((pxy[i]-acy)*l0+l0b)>>16;
		j=lqtvq_clamp255(j);
		cr=(((255-j)*min[0])+(j*max[0]))>>8;
		cg=(((255-j)*min[1])+(j*max[1]))>>8;
		pxr[i]=cr;
		pxg[i]=cg;
		if(cr<mcr)mcr=cr;
		if(cg<mcg)mcg=cg;
		if(cr>ncr)ncr=cr;
		if(cg>ncg)ncg=cg;
	}

//	BTIC4B_S3TC_EncodeBlockAlphaI(block+0, pxy, min[0], max[0]);
//	BTIC4B_S3TC_EncodeBlockAlphaI(block+8, pxy, min[1], max[1]);

	BTIC4B_S3TC_EncodeBlockAlphaI(block+0, pxr, mcr, ncr);
	BTIC4B_S3TC_EncodeBlockAlphaI(block+8, pxg, mcg, ncg);
}

void BTIC4B_BC5_EncodeBlockBGRA(byte *block,
	byte *rgba, int ystr)
{
	s16 pxr[16], pxg[16];
	int mcr, ncr, mcg, ncg;
	int acy;
	int cr, cg;
	int l0, l0b;
	int i, j, k;

	mcr=32767; mcg=32767;
	ncr=-32767; ncg=-32767;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=rgba[i*ystr+j*4+2];
		cg=rgba[i*ystr+j*4+1];
		pxr[i*4+j]=cr;
		pxg[i*4+j]=cg;
		if(cr<mcr)mcr=cr;
		if(cg<mcg)mcg=cg;
		if(cr>ncr)ncr=cr;
		if(cg>ncg)ncg=cg;
	}

	BTIC4B_S3TC_EncodeBlockAlphaI(block+0, pxr, mcr, ncr);
	BTIC4B_S3TC_EncodeBlockAlphaI(block+8, pxg, mcg, ncg);
}

void BTIC4B_S2TC_EncodeBlockBits32(byte *block,
	u32 pxy, int *min, int *max)
{
	int p0, p1, p2, p3;
	int pa, pb, px;

	pa=btic4b_s2tc_packrgb(min[0], min[1], min[2]);
	pb=btic4b_s2tc_packrgb(max[0], max[1], max[2]);

	p0=btic4b_s2tc_4x1to4x1a[(pxy    )&0xFF];
	p1=btic4b_s2tc_4x1to4x1b[(pxy>> 8)&0xFF];
	p2=btic4b_s2tc_4x1to4x1a[(pxy>>16)&0xFF];
	p3=btic4b_s2tc_4x1to4x1b[(pxy>>24)&0xFF];
	px=p0|(p1<<8)|(p2<<16)|(p3<<24);

//	if(pa>pb)
	if(pa<pb)
	{
		btic4b_setu16le(block+0, pb);
		btic4b_setu16le(block+2, pa);
		btic4b_setu32le(block+4, px^0x55555555);
//		btic4b_setu32le(block+4, px);
	}else
	{
		btic4b_setu16le(block+0, pa);
		btic4b_setu16le(block+2, pb);
		btic4b_setu32le(block+4, px);
//		btic4b_setu32le(block+4, px^0x55555555);
	}
}

void BTIC4B_S2TC_EncodeBlockBits32T(byte *block,
	u32 pxy, u32 pxa, int *min, int *max)
{
	int p0, p1, p2, p3;
	int pa, pb, px, pxm;

	pa=btic4b_s2tc_packrgb(min[0], min[1], min[2]);
	pb=btic4b_s2tc_packrgb(max[0], max[1], max[2]);

	if(min[3]>=128)
	{
		BTIC4B_S2TC_EncodeBlockBits32(block, pxy, min, max);
		return;
	}

	px=(pxa|(pxa>>1))&0x55555555;
	px=px|(px<<1);
	pxm=~px;

	p0=btic4b_s2tc_4x1to4x1c[(pxy    )&0xFF];
	p1=btic4b_s2tc_4x1to4x1c[(pxy>> 8)&0xFF];
	p2=btic4b_s2tc_4x1to4x1c[(pxy>>16)&0xFF];
	p3=btic4b_s2tc_4x1to4x1c[(pxy>>24)&0xFF];
	px=p0|(p1<<8)|(p2<<16)|(p3<<24);
	
	px|=pxm;

	if(pa>pb)
//	if(pa<pb)
	{
		btic4b_setu16le(block+0, pb);
		btic4b_setu16le(block+2, pa);
		btic4b_setu32le(block+4, px^0x55555555);
//		btic4b_setu32le(block+4, px);
	}else
	{
		btic4b_setu16le(block+0, pa);
		btic4b_setu16le(block+2, pb);
		btic4b_setu32le(block+4, px);
//		btic4b_setu32le(block+4, px^0x55555555);
	}
}

void BTIC4B_BC1_EncodeBlockBits32(byte *block,
	u32 pxy, u32 pxa, int *min, int *max)
{
	BTIC4B_S2TC_EncodeBlockBits32T(block, pxy, pxa, min, max);
}

void BTIC4B_BC3_EncodeBlockBits32(byte *block,
	u32 pxy, u32 pxa, int *min, int *max)
{
	int p0, p1, p2, p3;
	int pa;
	
	if((max[3]-min[3])>=8)
	{
		p0=btic4b_s2tc_4x1x2to4x1x3[(pxa    )&0xFF];
		p1=btic4b_s2tc_4x1x2to4x1x3[(pxa>> 8)&0xFF];
		p2=btic4b_s2tc_4x1x2to4x1x3[(pxa>>16)&0xFF];
		p3=btic4b_s2tc_4x1x2to4x1x3[(pxa>>24)&0xFF];
//		pa=min[3]|(max[3]<<8);
		pa=max[3]|(min[3]<<8);
		btic4b_setu16le(block+ 0, pa);
		btic4b_setu16le(block+ 2, p0|(p1<<12));
		btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
	}else
	{
		pxa=max[3]|(max[3]<<8);
		btic4b_setu32le(block+ 0, pxa);
		btic4b_setu32le(block+ 4, 0);
	}

	BTIC4B_S2TC_EncodeBlockBits32(block+8, pxy, min, max);
}


void BTIC4B_BC4_EncodeBlockBits32(byte *block,
	u32 pxy, u32 pxa, int *min, int *max)
{
	int p0, p1, p2, p3;
	int pa;
	
	p0=btic4b_s2tc_4x1x2to4x1x3[(pxy    )&0xFF];
	p1=btic4b_s2tc_4x1x2to4x1x3[(pxy>> 8)&0xFF];
	p2=btic4b_s2tc_4x1x2to4x1x3[(pxy>>16)&0xFF];
	p3=btic4b_s2tc_4x1x2to4x1x3[(pxy>>24)&0xFF];
//	pa=min[0]|(max[0]<<8);
	pa=max[0]|(min[0]<<8);
	btic4b_setu16le(block+ 0, pa);
	btic4b_setu16le(block+ 2, p0|(p1<<12));
	btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
}

void BTIC4B_BC5_EncodeBlockBits32(byte *block,
	u32 pxy, u32 pxa, int *min, int *max)
{
	int p0, p1, p2, p3;
	int pa, pb;
	
	p0=btic4b_s2tc_4x1x2to4x1x3[(pxy    )&0xFF];
	p1=btic4b_s2tc_4x1x2to4x1x3[(pxy>> 8)&0xFF];
	p2=btic4b_s2tc_4x1x2to4x1x3[(pxy>>16)&0xFF];
	p3=btic4b_s2tc_4x1x2to4x1x3[(pxy>>24)&0xFF];
//	pa=min[0]|(max[0]<<8);
//	pb=min[1]|(max[1]<<8);

	pa=max[0]|(min[0]<<8);
	pb=max[1]|(min[1]<<8);
	btic4b_setu16le(block+ 0, pa);
	btic4b_setu16le(block+ 2, p0|(p1<<12));
	btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
	btic4b_setu16le(block+ 8, pb);
	btic4b_setu16le(block+10, p0|(p1<<12));
	btic4b_setu32le(block+12, (p1>>4)|(p2<<8)|(p3<<20));
}

void BTIC4B_BC1_EncodeBlockBits48(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	u32 px, pxa2;
	int p0, p1, p2, p3;

	if(min[3]<128)
	{
		p0=btic4b_bc7_4x1x3to4x1x2a[(pxy    )&0xFFF];
		p1=btic4b_bc7_4x1x3to4x1x2b[(pxy>>12)&0xFFF];
		p2=btic4b_bc7_4x1x3to4x1x2a[(pxy>>24)&0xFFF];
		p3=btic4b_bc7_4x1x3to4x1x2b[(pxy>>36)&0xFFF];
		px=p0|(p1<<8)|(p2<<16)|(p3<<24);

		p0=btic4b_bc7_4x1x3to4x1x2a[(pxa    )&0xFFF];
		p1=btic4b_bc7_4x1x3to4x1x2b[(pxa>>12)&0xFFF];
		p2=btic4b_bc7_4x1x3to4x1x2a[(pxa>>24)&0xFFF];
		p3=btic4b_bc7_4x1x3to4x1x2b[(pxa>>36)&0xFFF];
		pxa2=p0|(p1<<8)|(p2<<16)|(p3<<24);

		BTIC4B_S2TC_EncodeBlockBits32T(block, px, pxa2, min, max);
	}else
	{
		p0=btic4b_s2tc_4x1x3to4x1a[(pxy    )&0xFFF];
		p1=btic4b_s2tc_4x1x3to4x1b[(pxy>>12)&0xFFF];
		p2=btic4b_s2tc_4x1x3to4x1a[(pxy>>24)&0xFFF];
		p3=btic4b_s2tc_4x1x3to4x1b[(pxy>>36)&0xFFF];
		px=p0|(p1<<8)|(p2<<16)|(p3<<24);
		BTIC4B_S2TC_EncodeBlockBits32(block, px, min, max);
	}
}

void BTIC4B_BC3_EncodeBlockBits48(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	u32 px;
	int p0, p1, p2, p3;
	int p4, p5, p6, p7;
	int pa;
	
	if((max[3]-min[3])>=8)
	{
		p0=btic4b_s2tc_4x1x3to4x1x3[(pxa    )&0xFFF];
		p1=btic4b_s2tc_4x1x3to4x1x3[(pxa>>12)&0xFFF];
		p2=btic4b_s2tc_4x1x3to4x1x3[(pxa>>24)&0xFFF];
		p3=btic4b_s2tc_4x1x3to4x1x3[(pxa>>36)&0xFFF];
		pa=min[3]|(max[3]<<8);
		btic4b_setu16le(block+ 0, pa);
		btic4b_setu16le(block+ 0, p0|(p1<<12));
		btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
	}else
	{
		pxa=max[3]|(max[3]<<8);
		btic4b_setu32le(block+ 0, pxa);
		btic4b_setu32le(block+ 4, 0);
	}

//	return;

//	BTIC4B_BC1_EncodeBlockBits48(block+8, pxy, 0, min, max);
//	p0=btic4b_s2tc_4x1x3to4x1a[(pxy    )&0xFFF];
//	p1=btic4b_s2tc_4x1x3to4x1b[(pxy>>12)&0xFFF];
//	p2=btic4b_s2tc_4x1x3to4x1a[(pxy>>24)&0xFFF];
//	p3=btic4b_s2tc_4x1x3to4x1b[(pxy>>36)&0xFFF];

	p0=(pxy    ); p0=((p0&0x30)>>2)|((p0&0x06)>>1);
	p1=(pxy>> 6); p1=((p1&0x30)>>2)|((p1&0x06)>>1);
	p2=(pxy>>12); p2=((p2&0x30)>>2)|((p2&0x06)>>1);
	p3=(pxy>>18); p3=((p3&0x30)>>2)|((p3&0x06)>>1);
	p4=(pxy>>24); p4=((p4&0x30)>>2)|((p4&0x06)>>1);
	p5=(pxy>>30); p5=((p5&0x30)>>2)|((p5&0x06)>>1);
	p6=(pxy>>36); p6=((p6&0x30)>>2)|((p6&0x06)>>1);
	p7=(pxy>>42); p7=((p7&0x30)>>2)|((p7&0x06)>>1);
	px=	(p0    )|(p1<< 4)|(p2<< 8)|(p3<<12)|
		(p4<<16)|(p5<<20)|(p6<<24)|(p7<<28);
//	px=p0|(p1<<8)|(p2<<16)|(p3<<24);
	BTIC4B_S2TC_EncodeBlockBits32(block+8, px, min, max);
}

void BTIC4B_BC4_EncodeBlockBits48(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	u32 px;
	int p0, p1, p2, p3;
	int pa;
	
	p0=btic4b_s2tc_4x1x3to4x1x3[(pxy    )&0xFFF];
	p1=btic4b_s2tc_4x1x3to4x1x3[(pxy>>12)&0xFFF];
	p2=btic4b_s2tc_4x1x3to4x1x3[(pxy>>24)&0xFFF];
	p3=btic4b_s2tc_4x1x3to4x1x3[(pxy>>36)&0xFFF];
	pa=min[0]|(max[0]<<8);
	btic4b_setu16le(block+ 0, pa);
	btic4b_setu16le(block+ 2, p0|(p1<<12));
	btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
}

void BTIC4B_BC5_EncodeBlockBits48(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	u32 px;
	int p0, p1, p2, p3;
	int pa, pb;
	
	p0=btic4b_s2tc_4x1x3to4x1x3[(pxy    )&0xFFF];
	p1=btic4b_s2tc_4x1x3to4x1x3[(pxy>>12)&0xFFF];
	p2=btic4b_s2tc_4x1x3to4x1x3[(pxy>>24)&0xFFF];
	p3=btic4b_s2tc_4x1x3to4x1x3[(pxy>>36)&0xFFF];
	pa=min[0]|(max[0]<<8);
	pb=min[1]|(max[1]<<8);
	btic4b_setu16le(block+ 0, pa);
	btic4b_setu16le(block+ 2, p0|(p1<<12));
	btic4b_setu32le(block+ 4, (p1>>4)|(p2<<8)|(p3<<20));
	btic4b_setu16le(block+ 8, pb);
	btic4b_setu16le(block+10, p0|(p1<<12));
	btic4b_setu32le(block+12, (p1>>4)|(p2<<8)|(p3<<20));
}


void BTIC4B_BC1_EncodeBlockBits64(byte *block,
	u64 pxy, int *min, int *max)
{
	u32 px;
//	u32 pxa, pxb;
//	u64 px;
	int p0, p1, p2, p3;
	int p4, p5, p6, p7;

#if 0
	p0=(pxy    )&0xFF; p0=((p0&0xE0)>>2)|((p0&0x0E)>>1);
	p1=(pxy>> 8)&0xFF; p1=((p1&0xE0)>>2)|((p1&0x0E)>>1);
	p2=(pxy>>16)&0xFF; p2=((p2&0xE0)>>2)|((p2&0x0E)>>1);
	p3=(pxy>>24)&0xFF; p3=((p3&0xE0)>>2)|((p3&0x0E)>>1);
	p4=(pxy>>32)&0xFF; p4=((p4&0xE0)>>2)|((p4&0x0E)>>1);
	p5=(pxy>>40)&0xFF; p5=((p5&0xE0)>>2)|((p5&0x0E)>>1);
	p6=(pxy>>48)&0xFF; p6=((p6&0xE0)>>2)|((p6&0x0E)>>1);
	p7=(pxy>>56)&0xFF; p7=((p7&0xE0)>>2)|((p7&0x0E)>>1);
	pxa=(p0    )|(p1<< 6)|(p2<<12)|(p3<<18);
	pxb=(p4    )|(p5<< 6)|(p6<<12)|(p7<<18);
	px=pxa|(((u64)pxb)<<24);
	
	BTIC4B_BC1_EncodeBlockBits48(block, px, 0, min, max);
#endif

#if 1
	p0=(pxy    )&0xFF; p0=((p0&0xC0)>>4)|((p0&0x0C)>>2);
	p1=(pxy>> 8)&0xFF; p1=((p1&0xC0)>>4)|((p1&0x0C)>>2);
	p2=(pxy>>16)&0xFF; p2=((p2&0xC0)>>4)|((p2&0x0C)>>2);
	p3=(pxy>>24)&0xFF; p3=((p3&0xC0)>>4)|((p3&0x0C)>>2);
	p4=(pxy>>32)&0xFF; p4=((p4&0xC0)>>4)|((p4&0x0C)>>2);
	p5=(pxy>>40)&0xFF; p5=((p5&0xC0)>>4)|((p5&0x0C)>>2);
	p6=(pxy>>48)&0xFF; p6=((p6&0xC0)>>4)|((p6&0x0C)>>2);
	p7=(pxy>>56)&0xFF; p7=((p7&0xC0)>>4)|((p7&0x0C)>>2);
	px=	(p0    )|(p1<< 4)|(p2<< 8)|(p3<<12)|
		(p4<<16)|(p5<<20)|(p6<<24)|(p7<<28);
	BTIC4B_S2TC_EncodeBlockBits32(block, px, min, max);
#endif

//	p0=btic4b_s2tc_2x1x4to2x1[(pxy    )&0xFF];
//	p1=btic4b_s2tc_2x1x4to2x1[(pxy>> 8)&0xFF];
//	p2=btic4b_s2tc_2x1x4to2x1[(pxy>>16)&0xFF];
//	p3=btic4b_s2tc_2x1x4to2x1[(pxy>>24)&0xFF];
//	p4=btic4b_s2tc_2x1x4to2x1[(pxy>>32)&0xFF];
//	p5=btic4b_s2tc_2x1x4to2x1[(pxy>>40)&0xFF];
//	p6=btic4b_s2tc_2x1x4to2x1[(pxy>>48)&0xFF];
//	p7=btic4b_s2tc_2x1x4to2x1[(pxy>>56)&0xFF];
//	px=	(p0    )|(p1<< 4)|(p2<< 8)|(p3<<12)|
//		(p4<<16)|(p5<<20)|(p6<<24)|(p7<<28);
//	BTIC4B_S2TC_EncodeBlockBits32(block, px, min, max);
}

void BTIC4B_BC3_EncodeBlockBits64(byte *block,
	u64 pxy, int *min, int *max)
{
	int pxa;
	pxa=max[3]|(max[3]<<8);
	btic4b_setu32le(block+ 0, pxa);
	btic4b_setu32le(block+ 4, 0);
	BTIC4B_BC1_EncodeBlockBits64(block+8, pxy, min, max);
}

void BTIC4B_BC4_EncodeBlockBits64(byte *block,
	u64 pxy, int *min, int *max)
{
	u32 pxa, pxb;
	u64 px;
	int p0, p1, p2, p3;
	int p4, p5, p6, p7;

	p0=(pxy    )&0xFF; p0=((p0&0xE0)>>2)|((p0&0x0E)>>1);
	p1=(pxy>> 8)&0xFF; p1=((p1&0xE0)>>2)|((p1&0x0E)>>1);
	p2=(pxy>>16)&0xFF; p2=((p2&0xE0)>>2)|((p2&0x0E)>>1);
	p3=(pxy>>24)&0xFF; p3=((p3&0xE0)>>2)|((p3&0x0E)>>1);
	p4=(pxy>>32)&0xFF; p4=((p4&0xE0)>>2)|((p4&0x0E)>>1);
	p5=(pxy>>40)&0xFF; p5=((p5&0xE0)>>2)|((p5&0x0E)>>1);
	p6=(pxy>>48)&0xFF; p6=((p6&0xE0)>>2)|((p6&0x0E)>>1);
	p7=(pxy>>56)&0xFF; p7=((p7&0xE0)>>2)|((p7&0x0E)>>1);
	pxa=(p0    )|(p1<< 6)|(p2<<12)|(p3<<18);
	pxb=(p4    )|(p5<< 6)|(p6<<12)|(p7<<18);
	px=pxa|(((u64)pxb)<<24);
	
	BTIC4B_BC4_EncodeBlockBits48(block, px, 0, min, max);
}

void BTIC4B_BC5_EncodeBlockBits64(byte *block,
	u64 pxy, int *min, int *max)
{
	int min2[4], max2[4];
	u32 pxa, pxb;
	u64 px;
	int p0, p1, p2, p3;
	int p4, p5, p6, p7;

	p0=(pxy    )&0xFF; p0=((p0&0xE0)>>2)|((p0&0x0E)>>1);
	p1=(pxy>> 8)&0xFF; p1=((p1&0xE0)>>2)|((p1&0x0E)>>1);
	p2=(pxy>>16)&0xFF; p2=((p2&0xE0)>>2)|((p2&0x0E)>>1);
	p3=(pxy>>24)&0xFF; p3=((p3&0xE0)>>2)|((p3&0x0E)>>1);
	p4=(pxy>>32)&0xFF; p4=((p4&0xE0)>>2)|((p4&0x0E)>>1);
	p5=(pxy>>40)&0xFF; p5=((p5&0xE0)>>2)|((p5&0x0E)>>1);
	p6=(pxy>>48)&0xFF; p6=((p6&0xE0)>>2)|((p6&0x0E)>>1);
	p7=(pxy>>56)&0xFF; p7=((p7&0xE0)>>2)|((p7&0x0E)>>1);
	pxa=(p0    )|(p1<< 6)|(p2<<12)|(p3<<18);
	pxb=(p4    )|(p5<< 6)|(p6<<12)|(p7<<18);
	px=pxa|(((u64)pxb)<<24);
	
	min2[0]=min[1];
	max2[0]=max[1];
	BTIC4B_BC4_EncodeBlockBits48(block+0, px, 0, min, max);
	BTIC4B_BC4_EncodeBlockBits48(block+8, px, 0, min2, max2);
}

void BTIC4B_BC1_EncodeBlockFlat(byte *block, int *avg)
{
	u32 px;
	int pxa;
	
	px=0x00000000;
	if(avg[3]<128)
		{ px=0xFFFFFFFF; }

	pxa=btic4b_s2tc_packrgb(avg[0], avg[1], avg[2]);
	btic4b_setu16le(block+0, pxa);
	btic4b_setu16le(block+2, pxa);
	btic4b_setu32le(block+4, px);
}

void BTIC4B_BC3_EncodeBlockFlat(byte *block, int *avg)
{
	int pxa;
	pxa=avg[3]|(avg[3]<<8);
	btic4b_setu32le(block+ 0, pxa);
	btic4b_setu32le(block+ 4, 0);
	pxa=btic4b_s2tc_packrgb(avg[0], avg[1], avg[2]);
	btic4b_setu16le(block+ 8, pxa);
	btic4b_setu16le(block+10, pxa);
	btic4b_setu32le(block+12, 0);
}

void BTIC4B_BC4_EncodeBlockFlat(byte *block, int *avg)
{
	int pxa;
	pxa=avg[1]|(avg[1]<<8);
	btic4b_setu32le(block+ 0, pxa);
	btic4b_setu32le(block+ 4, 0);
}

void BTIC4B_BC5_EncodeBlockFlat(byte *block, int *avg)
{
	int pxa;

	pxa=avg[0]|(avg[0]<<8);
	btic4b_setu32le(block+ 0, pxa);
	btic4b_setu32le(block+ 4, 0);
	pxa=avg[1]|(avg[1]<<8);
	btic4b_setu32le(block+ 8, pxa);
	btic4b_setu32le(block+12, 0);
}

void BTIC4B_ConvImageBC1n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC1_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC1_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC1_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC1_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC1_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, oblock, 8, xs, ys);
}

void BTIC4B_ConvImageBC1nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	byte *pblk;
	byte *ct;
	int xs1, ys1, xs2, ys2;
	
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC1_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC1_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC1_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC1_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC1_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	ct=oblock; xs1=xs; ys1=ys;
	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, ct, 8, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	if(!ctx->pblk)
		ctx->pblk=malloc(xs1*ys1*16);

	BTIC4B_ConvImageBC7_BI(ctx,
		iblock, iblkstr, ct, 8, ctx->pblk, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	
	while((xs1>=4) && (ys1>=4))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 8, ctx->pblk, xs1, ys1);
		ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
		xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	}

	xs2=xs1; ys2=ys1;
	while((xs2>1) || (ys2>1))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 8, ctx->pblk, xs1, ys1);
		ct+=((xs2+3)>>2)*((ys2+3)>>2)*8;
		xs2=(xs2+1)>>1;		ys2=(ys2+1)>>1;
	}
}

void BTIC4B_ConvImageBC3n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC3_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC3_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC3_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC3_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC3_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, oblock, 16, xs, ys);
}

void BTIC4B_ConvImageBC3nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	byte *pblk;
	byte *ct;
	int xs1, ys1, xs2, ys2;
	
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC3_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC3_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC3_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC3_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC3_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	ct=oblock; xs1=xs; ys1=ys;
	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, ct, 16, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	if(!ctx->pblk)
		ctx->pblk=malloc(xs1*ys1*16);

	BTIC4B_ConvImageBC7_BI(ctx,
		iblock, iblkstr, ct, 16, ctx->pblk, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	
	while((xs1>=4) && (ys1>=4))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
		xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	}

	xs2=xs1; ys2=ys1;
	while((xs2>1) || (ys2>1))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs2+3)>>2)*((ys2+3)>>2)*16;
		xs2=(xs2+1)>>1;		ys2=(ys2+1)>>1;
	}
}


void BTIC4B_ConvImageBC4n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC4_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC4_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC4_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC4_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC4_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, oblock, 8, xs, ys);
}

void BTIC4B_ConvImageBC4nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	byte *pblk;
	byte *ct;
	int xs1, ys1, xs2, ys2;
	
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC4_EncodeBlock;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC4_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC4_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC4_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC4_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=NULL;

	ct=oblock; xs1=xs; ys1=ys;
	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, ct, 8, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	if(!ctx->pblk)
		ctx->pblk=malloc(xs1*ys1*16);

	BTIC4B_ConvImageBC7_BI(ctx,
		iblock, iblkstr, ct, 8, ctx->pblk, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	
	while((xs1>=4) && (ys1>=4))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 8, ctx->pblk, xs1, ys1);
		ct+=((xs1+3)>>2)*((ys1+3)>>2)*8;
		xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	}

	xs2=xs1; ys2=ys1;
	while((xs2>1) || (ys2>1))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 8, ctx->pblk, xs1, ys1);
		ct+=((xs2+3)>>2)*((ys2+3)>>2)*8;
		xs2=(xs2+1)>>1;		ys2=(ys2+1)>>1;
	}
}

void BTIC4B_ConvImageBC5n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC5_EncodeBlock;
//	ctx->BCnEncodeBlockBits32=BTIC4B_BC5_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits32=NULL;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC5_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC5_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC5_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=BTIC4B_BC5_EncodeBlockBGRA;

	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, oblock, 16, xs, ys);
}

void BTIC4B_ConvImageBC5nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	byte *pblk;
	byte *ct;
	int xs1, ys1, xs2, ys2;
	
	BTIC4B_ConvBlockS2TC_Init();

	ctx->BCnEncodeBlockGen=BTIC4B_BC5_EncodeBlock;
//	ctx->BCnEncodeBlockBits32=BTIC4B_BC5_EncodeBlockBits32;
	ctx->BCnEncodeBlockBits32=NULL;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC5_EncodeBlockBits48;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC5_EncodeBlockBits64;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC5_EncodeBlockFlat;
	ctx->BCnEncodeBlockBGRA=BTIC4B_BC5_EncodeBlockBGRA;

	ct=oblock; xs1=xs; ys1=ys;
	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, ct, 16, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	if(!ctx->pblk)
		ctx->pblk=malloc(xs1*ys1*16);

	BTIC4B_ConvImageBC7_BI(ctx,
		iblock, iblkstr, ct, 16, ctx->pblk, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	
	while((xs1>=4) && (ys1>=4))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
		xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	}

	xs2=xs1; ys2=ys1;
	while((xs2>1) || (ys2>1))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs2+3)>>2)*((ys2+3)>>2)*16;
		xs2=(xs2+1)>>1;		ys2=(ys2+1)>>1;
	}
}

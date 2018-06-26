/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*

Reserve 128 or 256 bits.

Y,U,V,d, PaPb,Pc,Pd, x, x, x, x,  x, x, x, x,  y, y, y, y,  y, y, y, y
Y,U,V,0, m,Dy,Pc,Pd, Du,Dv,Qe,Qf, Qa,Qb,Qc,Qd, y, y, y, y,  y, y, y, y
Y,U,V,0, m,d, x , x, Y1,U1,V1,d1, Qa,Qb,Qc,Qd, Y2,U2,V2,D2, Y3,U3,V3,D3
x,x,x,0, m,x, x , x, U0,U1,U2,U3, V0,V1,V2,V2, Y0,Y1,Y2,Y3, Y4,Y5,Y6,Y7,...

d==0: Flat or Special
  Pa==0: Flat
  Pa==1: 2x2x2bpp
    Pb=Diff
    Pc=Pixel Bits
  Pa==2: 4x4x1bpp
    Pb=Diff
    Pc/Pd=Pixel Bits
  Pa==3: Skip (No Translate)
  Pa==4: Skip+Translate
	Pc=Y Offset
	Pd=X Offset

  Pa==5: 2x2x1bpp
    Pb=Diff
    Pc=Pixel Bits

  Pa==6: 4x4x3bpp (YUVD)
    Pb=Diff
    Qe/Qf/Qa/Qb/Qc/Qd=Pixel Bits
    
  Pa==7: YUV 4:2:0
    Y0..Y15 (16..31), U0..U3 (8..11), V0..V3 (12..15)
  
  Pa==8: 2x1x2bpp
  Pa==9: 1x2x2bpp
    Pb=Diff
    Pc=Pixel Bits

  Pa==10: 4x2x2bpp
  Pa==11: 2x4x2bpp
    Pb=Diff
    Pc/Pd=Pixel Bits

  Pa==12: 4x2x2bpp (YUVDyuv)
  Pa==13: 2x4x2bpp
    Pb=Diff
    Pc/Pd=Pixel Bits
  Pa==14: 4x4x2bpp (YUVDyuv)
    Pb=Diff
    Qa/Qb/Qc/Qd=Pixel Bits
  Pa==15: 4x4x3bpp (YUVDyuv)
    Pb=Diff
    Qa/Qb/Qc/Qd/Qe/Qf=Pixel Bits

  Pa==16: 4x4x2bpp (YUVDx2)
    Pb=Diff
    Pn=1(Horz), 2(Vert)
    Qa/Qb/Qc/Qd=Pixel Bits
  Pa==17: 4x4x2bpp (YUVDx4)
    Pb=Diff
    Pn=1(Horz), 2(Vert)
    Qa/Qb/Qc/Qd=Pixel Bits
  Pa==18: 2x2x2bpp (Gradient)
  Pa==19: 2x2x2bpp (YUVDyuv)
  
 Alpha Block
 A,D,Pe,Pf, Pa,Pb,Pc,Pd
 A,0,D, M,  Pa,Pb,Pc,Pd
 
 A=(A0+A1)>>1
 D=A1-A0
 
 Default mode encodes alpha at 3 bits/pixel.
  M==0: No Alpha
  M==1: 2x2x2bpp
    Pb=Diff
    Pc=Pixel Bits
  M==2: 4x4x1bpp
    Pb=Diff
    Pc/Pd=Pixel Bits
  M==3: Skip (No Translate)
  M==4: Skip+Translate
    Pc=Y Offset
    Pd=X Offset
  M==5: 2x2x1bpp
    Pb=Diff
    Pc=Pixel Bits
  M==6: 4x4x2bpp
    Pb=Diff
    Pc=Pixel Bits
  M==7: Flat
*/

typedef struct bt1h_encparm_s bt1h_encparm;

struct bt1h_encparm_s {
int dflat;
int d2x2;
int d4x4x1;
int d4x4x2;
int d4x4x3;

int d2x2h;
int d4x4x2h;
int dchflat;

void (*EncodeBlock)(byte *block, int blksz,
	byte *yuv, int xstride, int ystride, int tflip,
	bt1h_encparm *ep);
};

int bt1h_fxdtab[256];

//int clamp255(int i)
//	{ return((i<0)?0:((i>255)?255:i)); }

int clamp255(int i)
{
//	int j;
//	j=i|((255-i)>>8);
//	j&=(~(i>>31))&255;
//	return(j);
	return((i<0)?0:((i>255)?255:i));
}

int clamp255sg(int i)
{
//	int j;
//	j=i|((255-i)>>8);
//	j&=(~(i>>31))&255;
//	return(j);
	return((i<-255)?-255:((i>255)?255:i));
}

int clamp15(int i)
	{ return((i<0)?0:((i>15)?15:i)); }
int clamp31(int i)
	{ return((i<0)?0:((i>31)?31:i)); }

static int clamp252(int y)
{
//	return((y<4)?4:((y>251)?251:y));
//	return((y<16)?16:((y>239)?239:y));
	return((y<8)?8:((y>243)?243:y));
}

int btic1h_subfold(int a, int b)
{
	int c;
	c=a-b;
	c=(c<<1)^(c>>31);
	return(c);
}

int btic1h_sgnfold(int a)
{
	int c;
	c=(a<<1)^(a>>31);
	return(c);
}

void BT1H_EncodeBlockYUY2_0(byte *block,
	byte *yuv, int xstride, int ystride,
	int dflat, int d2x2)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	byte pxy[16];
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int acy, acu, acv;
	int cy, cu, cv, dy;
	int cy0, cy1, cy2, cy3;
	int cu0, cv0, cu1, cv1;
	int cu2, cv2, cu3, cv3;
	int i, j, k, l;

#if 0
	mcy=999; ncy=-999;
	for(i=0; i<4; i++)
	{
		k=i*ystride;
		cy0=yuv[k+0];	cu0=yuv[k+1];
		cy1=yuv[k+2];	cv0=yuv[k+3];
		cy2=yuv[k+4];	cu1=yuv[k+5];
		cy3=yuv[k+6];	cv1=yuv[k+7];

		l=i<<2;
		pxy[l+0]=cy0;	pxy[l+1]=cy1;
		pxy[l+2]=cy2;	pxy[l+3]=cy3;

		if(cy0<mcy) { mcy=cy0; mcu=cu0; mcv=cv0; }
		if(cy1<mcy) { mcy=cy1; mcu=cu0; mcv=cv0; }
		if(cy2<mcy) { mcy=cy2; mcu=cu1; mcv=cv1; }
		if(cy3<mcy) { mcy=cy3; mcu=cu1; mcv=cv1; }

		if(cy0>ncy) { ncy=cy0; ncu=cu0; ncv=cv0; }
		if(cy1>ncy) { ncy=cy1; ncu=cu0; ncv=cv0; }
		if(cy2>ncy) { ncy=cy2; ncu=cu1; ncv=cv1; }
		if(cy3>ncy) { ncy=cy3; ncu=cu1; ncv=cv1; }
	}
#endif

#if 1
	mcy=999; ncy=-999; acu=0; acv=0;
	for(i=0; i<4; i++)
	{
		k=i*ystride;
		cy0=yuv[k+0];	cy1=yuv[k+2];
		cy2=yuv[k+4];	cy3=yuv[k+6];

		l=i<<2;
		pxy[l+0]=cy0;	pxy[l+1]=cy1;
		pxy[l+2]=cy2;	pxy[l+3]=cy3;

		if(cy0<mcy) { mcy=cy0; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }
		if(cy3<mcy) { mcy=cy3; }

		if(cy0>ncy) { ncy=cy0; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy2>ncy) { ncy=cy2; }
		if(cy3>ncy) { ncy=cy3; }

		cu0=yuv[k+1];		cv0=yuv[k+3];
		cu1=yuv[k+5];		cv1=yuv[k+7];
		acu=acu+cu0+cu1;
		acv=acv+cv0+cv1;
	}
	acu=acu>>3;
	acv=acv>>3;
#endif
	
	acy=(mcy+ncy)>>1;
//	acu=(mcu+ncu)>>1;
//	acv=(mcv+ncv)>>1;
//	dy=ncy-acy;
	dy=ncy-mcy;

	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l2=65536-2048;

	l1=(32768-6144)/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l3a=65536-1024;
	l3b=65536+1024;

	block[0]=acy;	block[1]=acu;
	block[2]=acv;	block[3]=dy;

	if(dy<dflat)
	{
		block[3]=0;
		block[4]=0;
		block[5]=0;
		block[6]=0;
		block[7]=0;
		return;
	}

	if(dy<d2x2)
	{
		block[3]=0;
		block[4]=1;
		block[5]=i;
		block[6]=0;
		block[7]=0;

		p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
		p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
		p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
		p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
		p0=idxtab[((p4-acy)*l1+l3a)>>13];
		p1=idxtab[((p5-acy)*l1+l3b)>>13];
		p2=idxtab[((p6-acy)*l1+l3a)>>13];
		p3=idxtab[((p7-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		return;
	}

	p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
	p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
	p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
	p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
	block[4]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
	p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
	p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
	p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
	block[5]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
	p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
	p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
	p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
	block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
	p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
	p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
	p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
	block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
}

void BT1H_EncodeBlockAlpha(byte *block,
	byte *rgba, int xstride, int ystride,
	bt1h_encparm *ep)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	static const char idxtab2[16]=
		{ 0,0,0,0, 0,1,2,3, 4,5,6,7, 7,7,7,7 };
	byte pxy[16];
	byte *cs, *ct;
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int l0, l1, l2, l3a, l3b;
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int acy, acu, acv;
	int cy, cu, cv, dy;
	int cy0, cy1, cy2, cy3;
	int cu0, cv0, cu1, cv1;
	int cu2, cv2, cu3, cv3;
	int i, j, k, l;

	i0=0; i1=xstride; i2=2*xstride; i3=3*xstride;

	cs=rgba; ct=pxy;
	mcy=999; ncy=-999;
	for(i=0; i<4; i++)
	{
		cy0=cs[i0];		cy1=cs[i1];
		cy2=cs[i2];		cy3=cs[i3];
		ct[0]=cy0;		ct[1]=cy1;
		ct[2]=cy2;		ct[3]=cy3;
		cs=cs+ystride;	ct=ct+4;

		if(cy0<mcy) { mcy=cy0; }
		if(cy3<mcy) { mcy=cy3; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }

		if(cy0>ncy) { ncy=cy0; }
		if(cy3>ncy) { ncy=cy3; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy2>ncy) { ncy=cy2; }
	}
	
	acy=(mcy+ncy)>>1;
	dy=ncy-mcy;

//	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
//	l2=65536-2048;

//	l1=(32768-6144)/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l1=bt1h_fxdtab[ncy-acy];
	l3a=65536-1024;
	l3b=65536+1024;

	if(dy<ep->dflat)
	{
		block[0]=acy;	block[1]=0;
		block[2]=0;		block[3]=7;
		block[4]=0;		block[5]=0;
		block[6]=0;		block[7]=0;
		return;
	}

	if(dy<ep->d2x2)
	{
		block[0]=acy;	block[1]=0;
		block[2]=dy;	block[3]=1;
		block[4]=0;		block[5]=0;
		block[6]=0;		block[7]=0;

		p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
		p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
		p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
		p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
		p0=idxtab[((p4-acy)*l1+l3a)>>13];
		p1=idxtab[((p5-acy)*l1+l3b)>>13];
		p2=idxtab[((p6-acy)*l1+l3a)>>13];
		p3=idxtab[((p7-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		return;
	}

	if(dy<ep->d4x4x2)
	{
		block[0]=acy;	block[1]=0;
		block[2]=dy;	block[3]=6;

		p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
		block[4]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
		block[5]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
		block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		return;
	}

	p0=idxtab2[((pxy[ 0]-acy)*l1+l3a)>>13];
	p1=idxtab2[((pxy[ 1]-acy)*l1+l3b)>>13];
	p2=idxtab2[((pxy[ 2]-acy)*l1+l3a)>>13];
	p3=idxtab2[((pxy[ 3]-acy)*l1+l3b)>>13];
	i0=(p0<<9)|(p1<<6)|(p2<<3)|p3;
	p0=idxtab2[((pxy[ 4]-acy)*l1+l3b)>>13];
	p1=idxtab2[((pxy[ 5]-acy)*l1+l3a)>>13];
	p2=idxtab2[((pxy[ 6]-acy)*l1+l3b)>>13];
	p3=idxtab2[((pxy[ 7]-acy)*l1+l3a)>>13];
	i1=(p0<<9)|(p1<<6)|(p2<<3)|p3;
	p0=idxtab2[((pxy[ 8]-acy)*l1+l3a)>>13];
	p1=idxtab2[((pxy[ 9]-acy)*l1+l3b)>>13];
	p2=idxtab2[((pxy[10]-acy)*l1+l3a)>>13];
	p3=idxtab2[((pxy[11]-acy)*l1+l3b)>>13];
	i2=(p0<<9)|(p1<<6)|(p2<<3)|p3;
	p0=idxtab2[((pxy[12]-acy)*l1+l3b)>>13];
	p1=idxtab2[((pxy[13]-acy)*l1+l3a)>>13];
	p2=idxtab2[((pxy[14]-acy)*l1+l3b)>>13];
	p3=idxtab2[((pxy[15]-acy)*l1+l3a)>>13];
	i3=(p0<<9)|(p1<<6)|(p2<<3)|p3;
	i4=(i0<<12)|i1;		i5=(i2<<12)|i3;
	block[0]=acy;		block[1]=dy;
	block[2]=i4>>16;	block[3]=i4>> 8;
	block[4]=i4;		block[5]=i5>>16;
	block[6]=i5>> 8;	block[7]=i5;
	return;
}

void BT1H_EncodeBlockInit()
{
	static int init=0;
	int i, j, k, l;
	
	if(init)return;
	init=1;

	for(i=0; i<256; i++)
	{
//		bt1h_fxdtab[i]=(32768-1536)/(i+1);
		bt1h_fxdtab[i]=(32768-3072)/(i+1);
//		bt1h_fxdtab[i]=(32768-4096)/(i+1);
//		bt1h_fxdtab[i]=(32768-6144)/(i+1);
	}
}


static void BT1H_EncodeBlockFilt_YUYV(
	byte *pxy,	byte *pxu,	byte *pxv,
	byte *pmcy,	byte *pncy,
	byte *yuv, int xstride, int ystride)
{
//	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3;
	int mcy, ncy;
//	int mcy, mcu, mcv;
//	int ncy, ncu, ncv;
//	int acy, acu, acv;
//	int cy, cu, cv, dy, dy1;
	int cy0, cy1, cy2, cy3;
	int cu0, cv0, cu1, cv1;
//	int cu2, cv2, cu3, cv3;
//	int i, j, k, l;

#if 0
	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		p0=((i<<1)+0);	p1=((i<<1)+1);
		p2=j<<2;		p3=j<<1;
		l0=p0*ystride+p2;
		l1=p1*ystride+p2;
		l2=(p0<<2)+p3;
		l3=(p1<<2)+p3;

		cy0=yuv[l0+0];	cy1=yuv[l0+2];
		cy2=yuv[l1+0];	cy3=yuv[l1+2];
		cu0=yuv[l0+1];	cv0=yuv[l0+3];
		cu1=yuv[l1+1];	cv1=yuv[l1+3];

		pxy[l2+0]=cy0;	pxy[l2+1]=cy1;
		pxy[l3+0]=cy2;	pxy[l3+1]=cy3;
		
		mcy=cy0;	ncy=cy0;
		if(cy3<mcy) { mcy=cy3; }
		if(cy3>ncy) { ncy=cy3; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }
		if(cy2>ncy) { ncy=cy2; }

		k=i*2+j;
		pmcy[k]=mcy;	pncy[k]=ncy;
		pxu[k]=(cu0+cu1)>>1;
		pxv[k]=(cv0+cv1)>>1;

//		acu=acu+cu0+cu1;
//		acv=acv+cv0+cv1;
	}
//	acu=(acu+4)>>3;
//	acv=(acv+4)>>3;
#endif

#if 1
	l1=ystride;
	cy0=yuv[0];	cy1=yuv[2];
	cy2=yuv[l1+0];	cy3=yuv[l1+2];
	cu0=yuv[1];	cv0=yuv[3];
	cu1=yuv[l1+1];	cv1=yuv[l1+3];
	pxy[0]=cy0;	pxy[1]=cy1;
	pxy[4]=cy2;	pxy[5]=cy3;		
	mcy=cy0;	ncy=cy0;
	if(cy3<mcy) { mcy=cy3; }
	if(cy3>ncy) { ncy=cy3; }
	if(cy1>ncy) { ncy=cy1; }
	if(cy1<mcy) { mcy=cy1; }
	if(cy2<mcy) { mcy=cy2; }
	if(cy2>ncy) { ncy=cy2; }
	pmcy[0]=mcy;	pncy[0]=ncy;
	pxu[0]=(cu0+cu1)>>1;
	pxv[0]=(cv0+cv1)>>1;

	cy0=yuv[4];	cy1=yuv[6];
	cy2=yuv[l1+4];	cy3=yuv[l1+6];
	cu0=yuv[5];	cv0=yuv[7];
	cu1=yuv[l1+5];	cv1=yuv[l1+7];
	pxy[2]=cy0;	pxy[3]=cy1;
	pxy[6]=cy2;	pxy[7]=cy3;
	mcy=cy0;	ncy=cy0;
	if(cy3<mcy) { mcy=cy3; }
	if(cy3>ncy) { ncy=cy3; }
	if(cy1>ncy) { ncy=cy1; }
	if(cy1<mcy) { mcy=cy1; }
	if(cy2<mcy) { mcy=cy2; }
	if(cy2>ncy) { ncy=cy2; }
	pmcy[1]=mcy;	pncy[1]=ncy;
	pxu[1]=(cu0+cu1)>>1;
	pxv[1]=(cv0+cv1)>>1;

	l0=2*ystride;
	l1=3*ystride;
	cy0=yuv[l0+0];	cy1=yuv[l0+2];
	cy2=yuv[l1+0];	cy3=yuv[l1+2];
	cu0=yuv[l0+1];	cv0=yuv[l0+3];
	cu1=yuv[l1+1];	cv1=yuv[l1+3];
	pxy[ 8]=cy0;	pxy[ 9]=cy1;
	pxy[12]=cy2;	pxy[13]=cy3;
	mcy=cy0;	ncy=cy0;
	if(cy3<mcy) { mcy=cy3; }
	if(cy3>ncy) { ncy=cy3; }
	if(cy1>ncy) { ncy=cy1; }
	if(cy1<mcy) { mcy=cy1; }
	if(cy2<mcy) { mcy=cy2; }
	if(cy2>ncy) { ncy=cy2; }
	pmcy[2]=mcy;	pncy[2]=ncy;
	pxu[2]=(cu0+cu1)>>1;
	pxv[2]=(cv0+cv1)>>1;

	cy0=yuv[l0+4];	cy1=yuv[l0+6];
	cy2=yuv[l1+4];	cy3=yuv[l1+6];
	cu0=yuv[l0+5];	cv0=yuv[l0+7];
	cu1=yuv[l1+5];	cv1=yuv[l1+7];
	pxy[10]=cy0;	pxy[11]=cy1;
	pxy[14]=cy2;	pxy[15]=cy3;
	mcy=cy0;	ncy=cy0;
	if(cy3<mcy) { mcy=cy3; }
	if(cy3>ncy) { ncy=cy3; }
	if(cy1>ncy) { ncy=cy1; }
	if(cy1<mcy) { mcy=cy1; }
	if(cy2<mcy) { mcy=cy2; }
	if(cy2>ncy) { ncy=cy2; }
	pmcy[3]=mcy;	pncy[3]=ncy;
	pxu[3]=(cu0+cu1)>>1;
	pxv[3]=(cv0+cv1)>>1;
#endif
}

#define BT1H_MIN(x, y) ((x<y)?x:y)

static void BT1H_EncodeBlockFilt_RGBI(
	byte *pxy,	byte *pxu,	byte *pxv,
	byte *pmcy,	byte *pncy,
	byte *rgb, int xstride, int ystride, int tflip)
{
	byte *rgb0, *rgb1, *rgb2, *rgb3;
	byte *pxy2;
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3;
	int mcy, ncy;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;	
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy, cu, cv;
	int cr, cg, cb;
	int i, j, k, l;

#if 1
	if((xstride==4) && !(tflip&1))
	{
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			p0=i<<1; p1=p0+1; p2=(j<<3);
			pxy2=pxy+((p0<<2)+(j<<1));
			rgb0=rgb+p0*ystride+p2;		rgb1=rgb+p1*ystride+p2;

			cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
			cr1=rgb0[4]; cg1=rgb0[5]; cb1=rgb0[6];
			cr2=rgb1[0]; cg2=rgb1[1]; cb2=rgb1[2];
			cr3=rgb1[4]; cg3=rgb1[5]; cb3=rgb1[6];

#ifdef BT1H_CHEAPYUV
//			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb0+cb3)>>1;
//			cy0=((cg0<<2)+3*cr0+cb0+4)>>3;	cy1=((cg1<<2)+3*cr1+cb1+4)>>3;
//			cy2=((cg2<<2)+3*cr2+cb2+4)>>3;	cy3=((cg3<<2)+3*cr3+cb3+4)>>3;
//			cu=((2*cb-cr-cg)>>2)+128;		cv=((2*cr-cb-cg)>>2)+128;

//			cy0=((cg0<<2)+3*cr0+cb0)>>3;	cy1=((cg1<<2)+3*cr1+cb1)>>3;
//			cy2=((cg2<<2)+3*cr2+cb2)>>3;	cy3=((cg3<<2)+3*cr3+cb3)>>3;
			cy0=((cg0<<1)+cr0+cb0)>>2;	cy1=((cg1<<1)+cr1+cb1)>>2;
			cy2=((cg2<<1)+cr2+cb2)>>2;	cy3=((cg3<<1)+cr3+cb3)>>2;

//			cy0=cg0;	cy1=cg1;
//			cy2=cg2;	cy3=cg3;

			cr=cr0; cg=cg0; cb=cb0;
			cy=cy0;

//			cy=((cg<<2)+3*cr+cb)>>3;
//			cy=((cg<<1)+cr+cb)>>2;
			cu=((cb-cy)>>1)+128;
			cv=((cr-cy)>>1)+128;
#else
//			cr=(cr0+cr1+cr2+cr3)>>2;
//			cg=(cg0+cg1+cg2+cg3)>>2;
//			cb=(cb0+cb1+cb2+cb3)>>2;
//			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb0+cb3)>>1;
//			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb1+cb2)>>1;

			cy0=(77*cr0 + 150*cg0 + 29*cb0 + 127)>>8;
			cy1=(77*cr1 + 150*cg1 + 29*cb1 + 127)>>8;
			cy2=(77*cr2 + 150*cg2 + 29*cb2 + 127)>>8;
			cy3=(77*cr3 + 150*cg3 + 29*cb3 + 127)>>8;

			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb0+cb3)>>1;
			cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
			cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

			pxy2[0]=cy0;	pxy2[1]=cy1;	pxy2[4]=cy2;	pxy2[5]=cy3;
//			l0=(cy0<cy3)?cy0:cy3;
//			l1=(cy1<cy2)?cy1:cy2;
//			l2=(cy0>cy3)?cy0:cy3;
//			l3=(cy1>cy2)?cy1:cy2;
//			mcy=(l0<l1)?l0:l1;
//			ncy=(l2>l3)?l2:l3;
			mcy=cy0; ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			k=i*2+j; pmcy[k]=mcy; pncy[k]=ncy; pxu[k]=cu; pxv[k]=cv;
		}
		return;
	}
#endif

#if 1
	if((xstride==4) && (tflip&1))
	{
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			p0=i<<1; p1=p0+1; p2=(j<<3);
			pxy2=pxy+((p0<<2)+(j<<1));
			rgb0=rgb+p0*ystride+p2;		rgb1=rgb+p1*ystride+p2;
			cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
			cr1=rgb0[6]; cg1=rgb0[5]; cb1=rgb0[4];
			cr2=rgb1[2]; cg2=rgb1[1]; cb2=rgb1[0];
			cr3=rgb1[6]; cg3=rgb1[5]; cb3=rgb1[4];

#ifdef BT1H_CHEAPYUV
			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb0+cb3)>>1;
			cy0=((cg0<<2)+3*cr0+cb0+4)>>3;	cy1=((cg1<<2)+3*cr1+cb1+4)>>3;
			cy2=((cg2<<2)+3*cr2+cb2+4)>>3;	cy3=((cg3<<2)+3*cr3+cb3+4)>>3;
			cu=((2*cb-cr-cg)>>2)+128;		cv=((2*cr-cb-cg)>>2)+128;
#else
//			cr=(cr0+cr1+cr2+cr3)>>2;
//			cg=(cg0+cg1+cg2+cg3)>>2;
//			cb=(cb0+cb1+cb2+cb3)>>2;
			cr=(cr0+cr3)>>1; cg=(cg0+cg3)>>1; cb=(cb0+cb3)>>1;

			cy0=(77*cr0 + 150*cg0 + 29*cb0 + 127)>>8;
			cy1=(77*cr1 + 150*cg1 + 29*cb1 + 127)>>8;
			cy2=(77*cr2 + 150*cg2 + 29*cb2 + 127)>>8;
			cy3=(77*cr3 + 150*cg3 + 29*cb3 + 127)>>8;
			cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
			cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

			pxy2[0]=cy0;	pxy2[1]=cy1;	pxy2[4]=cy2;	pxy2[5]=cy3;
//			l0=(cy0<cy3)?cy0:cy3;	l1=(cy1<cy2)?cy1:cy2;
//			l2=(cy0>cy3)?cy0:cy3;	l3=(cy1>cy2)?cy1:cy2;
//			mcy=(l0<l1)?l0:l1;		ncy=(l2>l3)?l2:l3;
			mcy=cy0; ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			k=i*2+j; pmcy[k]=mcy; pncy[k]=ncy; pxu[k]=cu; pxv[k]=cv;
		}
		return;
	}
#endif

	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		if(xstride==4)
		{
			p0=(i<<1)+0; p1=(i<<1)+1; p2=j*8;
			rgb0=rgb+p0*ystride+p2;
			rgb1=rgb+p1*ystride+p2;

			if(tflip&1)
			{
				cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
				cr1=rgb0[6]; cg1=rgb0[5]; cb1=rgb0[4];
				cr2=rgb1[2]; cg2=rgb1[1]; cb2=rgb1[0];
				cr3=rgb1[6]; cg3=rgb1[5]; cb3=rgb1[4];
			}else
			{
				cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
				cr1=rgb0[4]; cg1=rgb0[5]; cb1=rgb0[6];
				cr2=rgb1[0]; cg2=rgb1[1]; cb2=rgb1[2];
				cr3=rgb1[4]; cg3=rgb1[5]; cb3=rgb1[6];
			}
		}else
		{
			p0=((i<<1)+0);	p1=((i<<1)+1);
			p2=(j*2+0)*xstride;
			p3=(j*2+1)*xstride;

			l0=p0*ystride+p2;	l1=p0*ystride+p3;
			l2=p1*ystride+p2;	l3=p1*ystride+p3;
			rgb0=rgb+l0;		rgb1=rgb+l1;
			rgb2=rgb+l2;		rgb3=rgb+l3;

			if(tflip&1)
			{
				cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
				cr1=rgb1[2]; cg1=rgb1[1]; cb1=rgb1[0];
				cr2=rgb2[2]; cg2=rgb2[1]; cb2=rgb2[0];
				cr3=rgb3[2]; cg3=rgb3[1]; cb3=rgb3[0];
			}else
			{
				cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
				cr1=rgb1[0]; cg1=rgb1[1]; cb1=rgb1[2];
				cr2=rgb2[0]; cg2=rgb2[1]; cb2=rgb2[2];
				cr3=rgb3[0]; cg3=rgb3[1]; cb3=rgb3[2];
			}
		}

#ifdef BT1H_CHEAPYUV
		cy0=((cg0<<2)+3*cr0+cb0+4)>>3;
		cy1=((cg1<<2)+3*cr1+cb1+4)>>3;
		cy2=((cg2<<2)+3*cr2+cb2+4)>>3;
		cy3=((cg3<<2)+3*cr3+cb3+4)>>3;

		cr=(cr0+cr1+cr2+cr3)>>2;
		cg=(cg0+cg1+cg2+cg3)>>2;
		cb=(cb0+cb1+cb2+cb3)>>2;

//		cu=((2*cb-cr-cg+2)>>2)+128;
//		cv=((2*cr-cb-cg+2)>>2)+128;

		cu=((2*cb-cr-cg)>>2)+128;
		cv=((2*cr-cb-cg)>>2)+128;
#else
		cr=(cr0+cr1+cr2+cr3)>>2;
		cg=(cg0+cg1+cg2+cg3)>>2;
		cb=(cb0+cb1+cb2+cb3)>>2;
		cy0=(77*cr0 + 150*cg0 + 29*cb0 + 127)>>8;
		cy1=(77*cr1 + 150*cg1 + 29*cb1 + 127)>>8;
		cy2=(77*cr2 + 150*cg2 + 29*cb2 + 127)>>8;
		cy3=(77*cr3 + 150*cg3 + 29*cb3 + 127)>>8;
		cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
		cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

#if 0
		if((cy0|cy1|cy2|cy3|cu|cv)>>8)
//		if((cu|cv)>>8)
		{
			cy0=clamp255(cy0);	cy1=clamp255(cy1);
			cy2=clamp255(cy2);	cy3=clamp255(cy3);
			cu=clamp255(cu);	cv=clamp255(cv);
		}
#endif

		l2=(p0<<2)+(j<<1);
		l3=(p1<<2)+(j<<1);
		pxy[l2+0]=cy0;	pxy[l2+1]=cy1;
		pxy[l3+0]=cy2;	pxy[l3+1]=cy3;
		
		mcy=cy0;	ncy=cy0;
		if(cy3<mcy) { mcy=cy3; }
		if(cy3>ncy) { ncy=cy3; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }
		if(cy2>ncy) { ncy=cy2; }

		k=i*2+j;
		pmcy[k]=mcy;	pncy[k]=ncy;
		pxu[k]=cu;		pxv[k]=cv;
	}
}


#if 1
static void BT1H_EncodeBlockFilt_RGBI_GDbDr(
	byte *pxy,	byte *pxu,	byte *pxv,
	byte *pmcy,	byte *pncy,
	byte *rgb, int xstride, int ystride, int tflip)
{
	byte *rgb0, *rgb1, *rgb2, *rgb3;
	byte *pxy2;
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3;
	int mcy, ncy;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;	
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy, cu, cv;
	int cr, cg, cb;
	int i, j, k, l;

#if 1
	if((xstride==4) && !(tflip&1))
	{
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			p0=i<<1; p1=p0+1; p2=(j<<3);
			pxy2=pxy+((p0<<2)+(j<<1));
			rgb0=rgb+p0*ystride+p2;		rgb1=rgb+p1*ystride+p2;

#if defined(X86)||defined(X86_64)
			l0=((u32 *)rgb0)[0];	l1=((u32 *)rgb0)[1];
			l2=((u32 *)rgb1)[0];	l3=((u32 *)rgb1)[1];			
			cr0=(byte)l0;	cg0=(byte)(l0>>8);	cb0=(byte)(l0>>16);
			cr1=(byte)l1;	cg1=(byte)(l1>>8);	cb1=(byte)(l1>>16);
			cr2=(byte)l2;	cg2=(byte)(l2>>8);	cb2=(byte)(l2>>16);
			cr3=(byte)l3;	cg3=(byte)(l3>>8);	cb3=(byte)(l3>>16);
#else
			cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
			cr1=rgb0[4]; cg1=rgb0[5]; cb1=rgb0[6];
			cr2=rgb1[0]; cg2=rgb1[1]; cb2=rgb1[2];
			cr3=rgb1[4]; cg3=rgb1[5]; cb3=rgb1[6];
#endif

			cy0=cg0;	cy1=cg1;
			cy2=cg2;	cy3=cg3;

			cr=cr0; cg=cg0; cb=cb0;
			cy=cy0;
			cu=((cb-cy)>>1)+128;
			cv=((cr-cy)>>1)+128;

			pxy2[0]=cy0;	pxy2[1]=cy1;	pxy2[4]=cy2;	pxy2[5]=cy3;
			mcy=cy0; ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			k=i*2+j; pmcy[k]=mcy; pncy[k]=ncy; pxu[k]=cu; pxv[k]=cv;
		}
		return;
	}
#endif

#if 1
	if((xstride==4) && (tflip&1))
	{
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			p0=i<<1; p1=p0+1; p2=(j<<3);
			pxy2=pxy+((p0<<2)+(j<<1));
			rgb0=rgb+p0*ystride+p2;		rgb1=rgb+p1*ystride+p2;

#if defined(X86)||defined(X86_64)
			l0=((u32 *)rgb0)[0];	l1=((u32 *)rgb0)[1];
			l2=((u32 *)rgb1)[0];	l3=((u32 *)rgb1)[1];			
			cb0=(byte)l0;	cg0=(byte)(l0>>8);	cr0=(byte)(l0>>16);
			cb1=(byte)l1;	cg1=(byte)(l1>>8);	cr1=(byte)(l1>>16);
			cb2=(byte)l2;	cg2=(byte)(l2>>8);	cr2=(byte)(l2>>16);
			cb3=(byte)l3;	cg3=(byte)(l3>>8);	cr3=(byte)(l3>>16);
#else
			cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
			cr1=rgb0[6]; cg1=rgb0[5]; cb1=rgb0[4];
			cr2=rgb1[2]; cg2=rgb1[1]; cb2=rgb1[0];
			cr3=rgb1[6]; cg3=rgb1[5]; cb3=rgb1[4];
#endif

			cy0=cg0;	cy1=cg1;
			cy2=cg2;	cy3=cg3;

			cr=cr0; cg=cg0; cb=cb0;
			cy=cy0;
			cu=((cb-cy)>>1)+128;
			cv=((cr-cy)>>1)+128;

			pxy2[0]=cy0;	pxy2[1]=cy1;	pxy2[4]=cy2;	pxy2[5]=cy3;
//			l0=(cy0<cy3)?cy0:cy3;	l1=(cy1<cy2)?cy1:cy2;
//			l2=(cy0>cy3)?cy0:cy3;	l3=(cy1>cy2)?cy1:cy2;
//			mcy=(l0<l1)?l0:l1;		ncy=(l2>l3)?l2:l3;
			mcy=cy0; ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			k=i*2+j; pmcy[k]=mcy; pncy[k]=ncy; pxu[k]=cu; pxv[k]=cv;
		}
		return;
	}
#endif

	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		if(xstride==4)
		{
			p0=(i<<1)+0; p1=(i<<1)+1; p2=j*8;
			rgb0=rgb+p0*ystride+p2;
			rgb1=rgb+p1*ystride+p2;

			if(tflip&1)
			{
				cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
				cr1=rgb0[6]; cg1=rgb0[5]; cb1=rgb0[4];
				cr2=rgb1[2]; cg2=rgb1[1]; cb2=rgb1[0];
				cr3=rgb1[6]; cg3=rgb1[5]; cb3=rgb1[4];
			}else
			{
				cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
				cr1=rgb0[4]; cg1=rgb0[5]; cb1=rgb0[6];
				cr2=rgb1[0]; cg2=rgb1[1]; cb2=rgb1[2];
				cr3=rgb1[4]; cg3=rgb1[5]; cb3=rgb1[6];
			}
		}else
		{
			p0=((i<<1)+0);	p1=((i<<1)+1);
			p2=(j*2+0)*xstride;
			p3=(j*2+1)*xstride;

			l0=p0*ystride+p2;	l1=p0*ystride+p3;
			l2=p1*ystride+p2;	l3=p1*ystride+p3;
			rgb0=rgb+l0;		rgb1=rgb+l1;
			rgb2=rgb+l2;		rgb3=rgb+l3;

			if(tflip&1)
			{
				cr0=rgb0[2]; cg0=rgb0[1]; cb0=rgb0[0];
				cr1=rgb1[2]; cg1=rgb1[1]; cb1=rgb1[0];
				cr2=rgb2[2]; cg2=rgb2[1]; cb2=rgb2[0];
				cr3=rgb3[2]; cg3=rgb3[1]; cb3=rgb3[0];
			}else
			{
				cr0=rgb0[0]; cg0=rgb0[1]; cb0=rgb0[2];
				cr1=rgb1[0]; cg1=rgb1[1]; cb1=rgb1[2];
				cr2=rgb2[0]; cg2=rgb2[1]; cb2=rgb2[2];
				cr3=rgb3[0]; cg3=rgb3[1]; cb3=rgb3[2];
			}
		}

		cy0=cg0;	cy1=cg1;
		cy2=cg2;	cy3=cg3;

		cr=cr0; cg=cg0; cb=cb0;
		cu=((cb-cg)>>1)+128;
		cv=((cr-cg)>>1)+128;

		l2=(p0<<2)+(j<<1);
		l3=(p1<<2)+(j<<1);
		pxy[l2+0]=cy0;	pxy[l2+1]=cy1;
		pxy[l3+0]=cy2;	pxy[l3+1]=cy3;
		
		mcy=cy0;	ncy=cy0;
		if(cy3<mcy) { mcy=cy3; }
		if(cy3>ncy) { ncy=cy3; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }
		if(cy2>ncy) { ncy=cy2; }

		k=i*2+j;
		pmcy[k]=mcy;	pncy[k]=ncy;
		pxu[k]=cu;		pxv[k]=cv;
	}
}
#endif

void BT1H_EncodeBlock_Inner(byte *block, int blksz,
	byte *pxy,	byte *pxu,	byte *pxv,
	byte *pmcy,	byte *pncy,
	byte *yuv, int xstride, int ystride,
	bt1h_encparm *ep)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	static const char idxtab2[16]=
		{ 0,0,0,0, 0,1,2,3, 4,5,6,7, 7,7,7,7 };
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int q0, q1, q2, q3, q4, q5, q6, q7;
	int l0, l1, l2, l3, l3a, l3b;
	byte mcyl, mcyr, mcyu, mcyd;
	byte ncyl, ncyr, ncyu, ncyd;
	int mcy, ncy, acy, acu, acv;
	int mcu, ncu, mcv, ncv;
//	int mcu, mcv;
//	int ncu, ncv;
//	int acy, acu, acv;
	int dyh, dyv, dyx1, dyhvp;
	int dcu, dcv, dcuv, dyce;
//	int cy, cu, cv;
	int dy, dy1, ddu, ddv;
//	int cy0, cy1, cy2, cy3;
//	int cu0, cv0, cu1, cv1;
//	int cu2, cv2, cu3, cv3;
	int i, j, k, l;

	acu=(pxu[0]+pxu[1]+pxu[2]+pxu[3]+2)>>2;
	acv=(pxv[0]+pxv[1]+pxv[2]+pxv[3]+2)>>2;
//	acu=clamp252(acu);
//	acv=clamp252(acv);

	dcu=btic1h_subfold(pxu[0], pxu[3])+
		btic1h_subfold(pxu[1], pxu[2]);
	dcv=btic1h_subfold(pxv[0], pxv[3])+
		btic1h_subfold(pxv[1], pxv[2]);
//	dcuv=(dcu+dcv)>>3;
	dcuv=(dcu+2*dcv)>>3;

	mcy=pmcy[0];	ncy=pncy[0];
	if(pmcy[3]<mcy) { mcy=pmcy[3]; }
	if(pncy[3]>ncy) { ncy=pncy[3]; }
	if(pmcy[1]<mcy) { mcy=pmcy[1]; }
	if(pncy[1]>ncy) { ncy=pncy[1]; }
	if(pmcy[2]<mcy) { mcy=pmcy[2]; }
	if(pncy[2]>ncy) { ncy=pncy[2]; }

//	mcy=clamp252(mcy);
//	ncy=clamp252(ncy);

	dy=ncy-mcy;
	dyce=dy+dcuv;
	
//	if(dcuv>ep->d2x2)
//	if(dcuv>ep->dflat)
	if(dcuv>ep->dchflat)
		goto hackygoto0;

	if(dyce<=ep->dflat)
	{
		acy=(mcy+ncy)>>1;

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=0;		block[5]=0;
		block[6]=0;		block[7]=0;
		return;
	}

	dyhvp=0;

#ifdef BT1H_USEHV
	mcyl=pmcy[0];	ncyl=pncy[0];
	mcyr=pmcy[1];	ncyr=pncy[1];
	if(pmcy[2]<mcyl) { mcyl=pmcy[2]; }
	if(pncy[2]>ncyl) { ncyl=pncy[2]; }
	if(pmcy[3]<mcyr) { mcyr=pmcy[3]; }
	if(pncy[3]>ncyr) { ncyr=pncy[3]; }

	mcyu=pmcy[0];	ncyu=pncy[0];
	mcyd=pmcy[2];	ncyd=pncy[2];
	if(pmcy[1]<mcyu) { mcyu=pmcy[1]; }
	if(pncy[1]>ncyu) { ncyu=pncy[1]; }
	if(pmcy[3]<mcyd) { mcyd=pmcy[3]; }
	if(pncy[3]>ncyd) { ncyd=pncy[3]; }

	dyh=ncyu-mcyu;	dyv=ncyl-mcyl;
	j=ncyd-mcyd;	k=ncyr-mcyr;
	if(j>dyh)dyh=j;
	if(k>dyv)dyv=k;

	dy1=(dy*3)>>2;
	dyhvp=(dyh<dy1)|(dyv<dy1);
#endif

	acy=(mcy+ncy)>>1;
//	dy=ncy-mcy;

	l1=bt1h_fxdtab[ncy-acy];
	l3a=65536-1024;
	l3b=65536+1024;

#if 1
	if(dyce<=ep->d2x2)
	{
//		dyx1=(ep->d2x2+ep->dflat)>>1;
		dyx1=ep->d2x2h;

#if 0
		if(dy<=dyx1)
		{
			block[0]=acy;	block[1]=acu;
			block[2]=acv;	block[3]=0;
			block[4]=5;		block[5]=dy;
			p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
			p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
			p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
			p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
			p0=(((p4-acy)*l1+l3a)>>16)&1;
			p1=(((p5-acy)*l1+l3b)>>16)&1;
			p2=(((p6-acy)*l1+l3a)>>16)&1;
			p3=(((p7-acy)*l1+l3b)>>16)&1;
			block[6]=(p0<<3)|(p1<<2)|(p2<<1)|p3;
			block[7]=0;
			return;
		}
#endif

#ifdef BT1H_USEHV
		if(dyhvp && (dy1<dyx1))
		{
			if(dyv<=dyh)
			{
				block[0]=acy;	block[1]=acu;
				block[2]=acv;	block[3]=0;
				block[4]=8;		block[5]=dy;

				p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5]+
					pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>3;
				p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7]+
					pxy[10]+pxy[11]+pxy[14]+pxy[15])>>3;
				p0=idxtab[((p4-acy)*l1+l3a)>>13];
				p1=idxtab[((p5-acy)*l1+l3b)>>13];
				block[6]=(p0<<2)|p1;
				block[7]=0;
				return;
			}
			else
			{
				block[0]=acy;	block[1]=acu;
				block[2]=acv;	block[3]=0;
				block[4]=9;		block[5]=dy;

				p4=(pxy[ 0]+pxy[ 1]+pxy[ 2]+pxy[ 3]+
					pxy[ 4]+pxy[ 5]+pxy[ 6]+pxy[ 7])>>3;
				p5=(pxy[ 8]+pxy[ 9]+pxy[10]+pxy[11]+
					pxy[12]+pxy[13]+pxy[14]+pxy[15])>>3;
				p0=idxtab[((p4-acy)*l1+l3a)>>13];
				p1=idxtab[((p5-acy)*l1+l3b)>>13];
				block[6]=(p0<<2)|p1;
				block[7]=0;
				return;
			}
		}
#endif

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=1;		block[5]=dy;

		p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
		p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
		p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
		p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
		
#ifdef BT1H_USEGRAD
// #if 0
// #if 1

#if 0
		q4=p5+p6-p7;
		q5=p4+p7-p6;
		q6=p4+p7-p5;
		q7=p5+p6-p4;
		
		q0=p4-q4;
		q1=p5-q5;
		q2=p6-q6;
		q3=p7-q7;
		q0=q0^(q0>>31);
		q1=q1^(q1>>31);
		q2=q2^(q2>>31);
		q3=q3^(q3>>31);
//		q2=(q0+q1)>>1;
//		q2=(q0+q1)<<1;
		q2=(q0+q1+q2+q3)<<2;
#endif

//		q0=(pxy[5]+pxy[6]+pxy[9]+pxy[10])>>2;
		q0=(pxy[6]+pxy[ 9])>>1;
		q1=(pxy[5]+pxy[10])>>1;
		q2=(pxy[0]+pxy[15])>>1;
		q3=(pxy[3]+pxy[12])>>1;

//		q1=(pxy[4]+pxy[7]+pxy[ 8]+pxy[11])>>2;
//		q2=(pxy[1]+pxy[2]+pxy[13]+pxy[14])>>2;

//		q1=(p4+p7)>>1;
//		q2=(p5+p6)>>1;

		q4=q2-q0;
		q5=q3-q1;
		q4=q4^(q4>>31);
		q5=q5^(q5>>31);
		
		q2=(q4+q5)<<1;
//		q2=(q4+q5);
		
		if(q2<ep->dflat)
//		if(1)
		{
			block[4]=18;
			
//			p4=pxy[ 0]; p5=pxy[ 3];
//			p6=pxy[12]; p7=pxy[15];
			p0=idxtab[((p4-acy)*l1+l3a)>>13];
			p1=idxtab[((p5-acy)*l1+l3b)>>13];
			p2=idxtab[((p6-acy)*l1+l3a)>>13];
			p3=idxtab[((p7-acy)*l1+l3b)>>13];
			block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
			block[7]=0;
			return;
		}
#endif
		
		p0=idxtab[((p4-acy)*l1+l3a)>>13];
		p1=idxtab[((p5-acy)*l1+l3b)>>13];
		p2=idxtab[((p6-acy)*l1+l3a)>>13];
		p3=idxtab[((p7-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		block[7]=0;

		return;
	}
#endif

#ifdef BT1H_USEHV
// #if 0
	if(dyhvp && (dy1<=ep->d2x2))
	{
		if(dyv<=dyh)
		{
			block[0]=acy;	block[1]=acu;
			block[2]=acv;	block[3]=0;
			block[4]=10;	block[5]=dy;

			p4=(pxy[ 0]+pxy[ 4])>>1;
			p5=(pxy[ 1]+pxy[ 5])>>1;
			p6=(pxy[ 2]+pxy[ 6])>>1;
			p7=(pxy[ 3]+pxy[ 7])>>1;
			p0=idxtab[((p4-acy)*l1+l3a)>>13];
			p1=idxtab[((p5-acy)*l1+l3b)>>13];
			p2=idxtab[((p6-acy)*l1+l3a)>>13];
			p3=idxtab[((p7-acy)*l1+l3b)>>13];
			block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

			p4=(pxy[ 8]+pxy[12])>>1;
			p5=(pxy[ 9]+pxy[13])>>1;
			p6=(pxy[10]+pxy[14])>>1;
			p7=(pxy[11]+pxy[15])>>1;
			p0=idxtab[((p4-acy)*l1+l3a)>>13];
			p1=idxtab[((p5-acy)*l1+l3b)>>13];
			p2=idxtab[((p6-acy)*l1+l3a)>>13];
			p3=idxtab[((p7-acy)*l1+l3b)>>13];
			block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

			return;
		}
		else
		{
			block[0]=acy;	block[1]=acu;
			block[2]=acv;	block[3]=0;
			block[4]=11;	block[5]=dy;

			p4=(pxy[ 0]+pxy[ 1])>>1;
			p5=(pxy[ 2]+pxy[ 3])>>1;
			p6=(pxy[ 4]+pxy[ 5])>>1;
			p7=(pxy[ 6]+pxy[ 7])>>1;
			p0=idxtab[((p4-acy)*l1+l3a)>>13];
			p1=idxtab[((p5-acy)*l1+l3b)>>13];
			p2=idxtab[((p6-acy)*l1+l3a)>>13];
			p3=idxtab[((p7-acy)*l1+l3b)>>13];
			block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

			p4=(pxy[ 8]+pxy[ 9])>>1;
			p5=(pxy[10]+pxy[11])>>1;
			p6=(pxy[12]+pxy[13])>>1;
			p7=(pxy[14]+pxy[15])>>1;
			p0=idxtab[((p4-acy)*l1+l3a)>>13];
			p1=idxtab[((p5-acy)*l1+l3b)>>13];
			p2=idxtab[((p6-acy)*l1+l3a)>>13];
			p3=idxtab[((p7-acy)*l1+l3b)>>13];
			block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

			return;
		}
	}
#endif

#if 1
	if(dyce<=ep->d4x4x1)
	{
		dy=(dy*181)>>8;

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=2;		block[5]=dy;

		p0=(((pxy[ 0]-acy)*l1+l3a)>>16)&1;
		p1=(((pxy[ 1]-acy)*l1+l3b)>>16)&1;
		p2=(((pxy[ 2]-acy)*l1+l3a)>>16)&1;
		p3=(((pxy[ 3]-acy)*l1+l3b)>>16)&1;
		p4=(((pxy[ 4]-acy)*l1+l3b)>>16)&1;
		p5=(((pxy[ 5]-acy)*l1+l3a)>>16)&1;
		p6=(((pxy[ 6]-acy)*l1+l3b)>>16)&1;
		p7=(((pxy[ 7]-acy)*l1+l3a)>>16)&1;
		block[6]=
			(p0<<7)|(p1<<6)|(p2<<5)|(p3<<4) |
			(p4<<3)|(p5<<2)|(p6<<1)|(p7   );
		p0=(((pxy[ 8]-acy)*l1+l3a)>>16)&1;
		p1=(((pxy[ 9]-acy)*l1+l3b)>>16)&1;
		p2=(((pxy[10]-acy)*l1+l3a)>>16)&1;
		p3=(((pxy[11]-acy)*l1+l3b)>>16)&1;
		p4=(((pxy[12]-acy)*l1+l3b)>>16)&1;
		p5=(((pxy[13]-acy)*l1+l3a)>>16)&1;
		p6=(((pxy[14]-acy)*l1+l3b)>>16)&1;
		p7=(((pxy[15]-acy)*l1+l3a)>>16)&1;
		block[7]=
			(p0<<7)|(p1<<6)|(p2<<5)|(p3<<4) |
			(p4<<3)|(p5<<2)|(p6<<1)|(p7   );

		return;
	}
#endif

	if(dyce<=ep->d4x4x2)
//	if(0)
//	if(1)
	{
		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=dy;

		p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
		block[4]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
		block[5]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
		block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		return;
	}

//	if(dyce<=ep->d4x4x3)
	if((dyce<=ep->d4x4x3) ||
		(dcuv<ep->dflat))
//	if(1)
//	if(0)
	{
		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=6;		block[5]=dy;

		p0=idxtab2[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab2[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab2[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab2[((pxy[ 3]-acy)*l1+l3b)>>13];
		p4=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p0=idxtab2[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab2[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab2[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab2[((pxy[ 7]-acy)*l1+l3a)>>13];
		p5=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p6=(p4<<12)|p5;		

		p0=idxtab2[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab2[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab2[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab2[((pxy[11]-acy)*l1+l3b)>>13];
		p4=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p0=idxtab2[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab2[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab2[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab2[((pxy[15]-acy)*l1+l3a)>>13];
		p5=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p7=(p4<<12)|p5;

		block[10]=p6>>16;
		block[11]=p6>>8;
		block[12]=p6;
		block[13]=p7>>16;
		block[14]=p7>>8;
		block[15]=p7;
		return;
	}
	
	hackygoto0:

//	dcu=btic1h_subfold(pxu[0], pxu[3])+
//		btic1h_subfold(pxu[1], pxu[2]);
//	dcv=btic1h_subfold(pxv[0], pxv[3])+
//		btic1h_subfold(pxv[1], pxv[2]);
//	dcuv=(dcu+dcv)>>3;

#if 1
	mcu=pxu[0];	ncu=pxu[0];
	if(pxu[3]<mcu) { mcu=pxu[3]; }
	if(pxu[3]>ncu) { ncu=pxu[3]; }
	if(pxu[1]<mcu) { mcu=pxu[1]; }
	if(pxu[1]>ncu) { ncu=pxu[1]; }
	if(pxu[2]<mcu) { mcu=pxu[2]; }
	if(pxu[2]>ncu) { ncu=pxu[2]; }

	mcv=pxv[0];	ncv=pxv[0];
	if(pxv[3]<mcv) { mcv=pxv[3]; }
	if(pxv[3]>ncv) { ncv=pxv[3]; }
	if(pxv[1]<mcv) { mcv=pxv[1]; }
	if(pxv[1]>ncv) { ncv=pxv[1]; }
	if(pxv[2]<mcv) { mcv=pxv[2]; }
	if(pxv[2]>ncv) { ncv=pxv[2]; }

	ddu=ncu-mcu;
	ddv=ncv-mcv;
#endif

#if 1
	if(dy<=ep->d2x2)
//	if(0)
	{
		acy=(mcy+ncy)>>1;
		acu=(mcu+ncu)>>1;
		acv=(mcv+ncv)>>1;

		l3a=65536-1024;
		l3b=65536+1024;

		l1=bt1h_fxdtab[ncy-acy];
		l2=bt1h_fxdtab[ncu-acu];
		l3=bt1h_fxdtab[ncv-acv];

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=23;	block[5]=(dy>>1)+128;
		block[6]=0;		block[7]=0;
		block[8]=(ddu>>1)+128;
		block[9]=(ddv>>1)+128;

		p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
		p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
		p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
		p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
				
		p0=idxtab[((p4-acy)*l1+l3a)>>13];
		p1=idxtab[((p5-acy)*l1+l3b)>>13];
		p2=idxtab[((p6-acy)*l1+l3a)>>13];
		p3=idxtab[((p7-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		p0=idxtab[((pxu[0]-acu)*l2+l3a)>>13];
		p1=idxtab[((pxu[1]-acu)*l2+l3b)>>13];
		p2=idxtab[((pxu[2]-acu)*l2+l3a)>>13];
		p3=idxtab[((pxu[3]-acu)*l2+l3b)>>13];
		block[10]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxv[0]-acv)*l3+l3a)>>13];
		p1=idxtab[((pxv[1]-acv)*l3+l3b)>>13];
		p2=idxtab[((pxv[2]-acv)*l3+l3a)>>13];
		p3=idxtab[((pxv[3]-acv)*l3+l3b)>>13];
		block[11]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		return;
	}
#endif

	if(dy<=ep->d4x4x2)
//	if(0)
	{
		acy=(mcy+ncy)>>1;
		acu=(mcu+ncu)>>1;
		acv=(mcv+ncv)>>1;

		l3a=65536-1024;
		l3b=65536+1024;

		l1=bt1h_fxdtab[ncy-acy];
		l2=bt1h_fxdtab[ncu-acu];
		l3=bt1h_fxdtab[ncv-acv];

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=20;	block[5]=(dy>>1)+128;
		block[6]=0;		block[7]=0;
		block[8]=(ddu>>1)+128;
		block[9]=(ddv>>1)+128;

		p0=idxtab[((pxu[0]-acu)*l2+l3a)>>13];
		p1=idxtab[((pxu[1]-acu)*l2+l3b)>>13];
		p2=idxtab[((pxu[2]-acu)*l2+l3a)>>13];
		p3=idxtab[((pxu[3]-acu)*l2+l3b)>>13];
		block[10]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxv[0]-acv)*l3+l3a)>>13];
		p1=idxtab[((pxv[1]-acv)*l3+l3b)>>13];
		p2=idxtab[((pxv[2]-acv)*l3+l3a)>>13];
		p3=idxtab[((pxv[3]-acv)*l3+l3b)>>13];
		block[11]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
		block[12]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
		block[13]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
		block[14]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
		block[15]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		return;
	}

//	if(dy<=ep->d4x4x3)
//	if(0)
	if(1)
	{
		acy=(mcy+ncy)>>1;
		acu=(mcu+ncu)>>1;
		acv=(mcv+ncv)>>1;

		l3a=65536-1024;
		l3b=65536+1024;

		l1=bt1h_fxdtab[ncy-acy];
		l2=bt1h_fxdtab[ncu-acu];
		l3=bt1h_fxdtab[ncv-acv];

		block[0]=acy;	block[1]=acu;
		block[2]=acv;	block[3]=0;
		block[4]=22;	block[5]=(dy>>1)+128;
		block[8]=(ddu>>1)+128;
		block[9]=(ddv>>1)+128;

		p0=idxtab[((pxu[0]-acu)*l2+l3a)>>13];
		p1=idxtab[((pxu[1]-acu)*l2+l3b)>>13];
		p2=idxtab[((pxu[2]-acu)*l2+l3a)>>13];
		p3=idxtab[((pxu[3]-acu)*l2+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxv[0]-acv)*l3+l3a)>>13];
		p1=idxtab[((pxv[1]-acv)*l3+l3b)>>13];
		p2=idxtab[((pxv[2]-acv)*l3+l3a)>>13];
		p3=idxtab[((pxv[3]-acv)*l3+l3b)>>13];
		block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		p0=idxtab2[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab2[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab2[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab2[((pxy[ 3]-acy)*l1+l3b)>>13];
		p4=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p0=idxtab2[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab2[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab2[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab2[((pxy[ 7]-acy)*l1+l3a)>>13];
		p5=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p6=(p4<<12)|p5;		

		p0=idxtab2[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab2[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab2[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab2[((pxy[11]-acy)*l1+l3b)>>13];
		p4=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p0=idxtab2[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab2[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab2[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab2[((pxy[15]-acy)*l1+l3a)>>13];
		p5=(p0<<9)|(p1<<6)|(p2<<3)|p3;
		p7=(p4<<12)|p5;

		block[10]=p6>>16;
		block[11]=p6>>8;
		block[12]=p6;
		block[13]=p7>>16;
		block[14]=p7>>8;
		block[15]=p7;
		return;
	}

	if(1)
	{
		q0=(pmcy[0]+pncy[0])>>1;
		q1=(pmcy[1]+pncy[1])>>1;
		q2=(pmcy[2]+pncy[2])>>1;
		q3=(pmcy[3]+pncy[3])>>1;
		q4=pncy[0]-pmcy[0];
		q5=pncy[1]-pmcy[1];
		q6=pncy[2]-pmcy[2];
		q7=pncy[3]-pmcy[3];

		l0=bt1h_fxdtab[pncy[0]-q0];
		l1=bt1h_fxdtab[pncy[1]-q1];
		l2=bt1h_fxdtab[pncy[2]-q2];
		l3=bt1h_fxdtab[pncy[3]-q3];

		l3a=65536-1024;
		l3b=65536+1024;
	
		block[ 0]=q0;		block[ 1]=pxu[0];
		block[ 2]=pxv[0];	block[ 3]=0;
		block[ 4]=17;		block[ 5]=q4;
		block[ 6]=0;		block[ 7]=0;
		block[ 8]=q1;		block[ 9]=pxu[1];
		block[10]=pxv[1];	block[11]=q5;

		block[12]=0;		block[13]=0;
		block[14]=0;		block[15]=0;

		block[16]=q2;		block[17]=pxu[2];
		block[18]=pxv[2];	block[19]=q6;
		block[20]=q3;		block[21]=pxu[3];
		block[22]=pxv[3];	block[23]=q7;

		p0=idxtab[((pxy[ 0]-q0)*l0+l3a)>>13];
		p1=idxtab[((pxy[ 1]-q0)*l0+l3b)>>13];
		p2=idxtab[((pxy[ 2]-q1)*l1+l3a)>>13];
		p3=idxtab[((pxy[ 3]-q1)*l1+l3b)>>13];
		block[12]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 4]-q0)*l0+l3b)>>13];
		p1=idxtab[((pxy[ 5]-q0)*l0+l3a)>>13];
		p2=idxtab[((pxy[ 6]-q1)*l1+l3b)>>13];
		p3=idxtab[((pxy[ 7]-q1)*l1+l3a)>>13];
		block[13]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[ 8]-q2)*l2+l3a)>>13];
		p1=idxtab[((pxy[ 9]-q2)*l2+l3b)>>13];
		p2=idxtab[((pxy[10]-q3)*l3+l3a)>>13];
		p3=idxtab[((pxy[11]-q3)*l3+l3b)>>13];
		block[14]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		p0=idxtab[((pxy[12]-q2)*l2+l3b)>>13];
		p1=idxtab[((pxy[13]-q2)*l2+l3a)>>13];
		p2=idxtab[((pxy[14]-q3)*l3+l3b)>>13];
		p3=idxtab[((pxy[15]-q3)*l3+l3a)>>13];
		block[15]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
		return;
	}
	
	if(0)
	{
		block[ 0]=acy;		block[1]=acu;
		block[ 2]=acv;		block[3]=0;
		block[ 4]=7;		block[5]=dy;
		block[ 6]=0;		block[7]=0;
		block[ 8]=pxu[ 0];	block[ 9]=pxu[ 1];
		block[10]=pxu[ 2];	block[11]=pxu[ 3];
		block[12]=pxv[ 0];	block[13]=pxv[ 1];
		block[14]=pxv[ 2];	block[15]=pxv[ 3];
		block[16]=pxy[ 0];	block[17]=pxy[ 1];
		block[18]=pxy[ 2];	block[19]=pxy[ 3];
		block[20]=pxy[ 4];	block[21]=pxy[ 5];
		block[22]=pxy[ 6];	block[23]=pxy[ 7];
		block[24]=pxy[ 8];	block[25]=pxy[ 9];
		block[26]=pxy[10];	block[27]=pxy[11];
		block[28]=pxy[12];	block[29]=pxy[13];
		block[30]=pxy[14];	block[31]=pxy[15];
		return;
	}

	printf("BT1H_EncodeBlock_Inner: Block Encode Error\n");
	block[ 0]=acy;		block[1]=acu;
	block[ 2]=acv;		block[3]=0;
	block[ 4]=0;		block[5]=dy;
	block[ 6]=0;		block[7]=0;
	return;
}

void BT1H_EncodeBlockYUY2(byte *block, int blksz,
	byte *yuv, int xstride, int ystride,
	bt1h_encparm *ep)
//	int dflat, int d2x2, int d4x4x1)
{
	byte pxy[16], pxu[4], pxv[4];
	byte pmcy[4], pncy[4];

	BT1H_EncodeBlockFilt_YUYV(
		pxy, pxu, pxv, pmcy, pncy, yuv, xstride, ystride);
	BT1H_EncodeBlock_Inner(block, blksz,
		pxy, pxu, pxv, pmcy, pncy, yuv, xstride, ystride,
		ep);
//		dflat, d2x2, d4x4x1);
}

void BT1H_EncodeBlockRGBI(byte *block, int blksz,
	byte *yuv, int xstride, int ystride, int tflip,
	bt1h_encparm *ep)
{
	byte pxy[16], pxu[4], pxv[4];
	byte pmcy[4], pncy[4];

	BT1H_EncodeBlockFilt_RGBI(
		pxy, pxu, pxv, pmcy, pncy, yuv,
		xstride, ystride, tflip);
	BT1H_EncodeBlock_Inner(block, blksz,
		pxy, pxu, pxv, pmcy, pncy, yuv, xstride, ystride,
		ep);
	
	if(tflip&2)
	{
		BT1H_EncodeBlockAlpha(block+24,
			yuv+3, 4, ystride, ep);
	}
}

void BT1H_EncodeBlockRGBI_GDbDr(byte *block, int blksz,
	byte *yuv, int xstride, int ystride, int tflip,
	bt1h_encparm *ep)
{
	byte pxy[16], pxu[4], pxv[4];
	byte pmcy[4], pncy[4];

	BT1H_EncodeBlockFilt_RGBI_GDbDr(
		pxy, pxu, pxv, pmcy, pncy, yuv,
		xstride, ystride, tflip);
	BT1H_EncodeBlock_Inner(block, blksz,
		pxy, pxu, pxv, pmcy, pncy, yuv, xstride, ystride,
		ep);
	
	if(tflip&2)
	{
		BT1H_EncodeBlockAlpha(block+24,
			yuv+3, 4, ystride, ep);
	}
}

void BT1H_EncodeBlockEdgeRGBI(byte *block, int blksz,
	byte *rgba, int xstride, int ystride,
	int xfrac, int yfrac, int tflip,
	bt1h_encparm *ep)
{
	byte tblk[16*4];
	byte cr, cg, cb, ca;
	byte *cs, *ct;
	int i, j, xn;

	xn=xfrac*xstride;
	for(i=0; i<yfrac; i++)
	{
		cs=rgba+i*ystride;
		ct=tblk+i*4*4;
		if(xstride==4)
		{
			for(j=0; j<xfrac; j++)
			{
				cr=*cs++; cg=*cs++;
				cb=*cs++; ca=*cs++;
				*ct++=cr; *ct++=cg;
				*ct++=cb; *ct++=ca;
			}
			for(; j<4; j++)
			{
				*ct++=cr; *ct++=cg;
				*ct++=cb; *ct++=ca;
			}
			continue;
		}
		if(xstride==3)
		{
			ca=255;
			for(j=0; j<xfrac; j++)
			{
				cr=*cs++; cg=*cs++; cb=*cs++; 
				*ct++=cr; *ct++=cg;
				*ct++=cb; *ct++=ca;
			}
			for(; j<4; j++)
			{
				*ct++=cr; *ct++=cg;
				*ct++=cb; *ct++=ca;
			}
			continue;
		}
		for(j=0; j<xn; j++)
			*ct++=*cs++;
	}

	for(; i<4; i++)
	{
		ct=tblk+i*4*4;
		for(j=0; j<4; j++)
		{
			*ct++=cr; *ct++=cg;
			*ct++=cb; *ct++=ca;
		}
	}
	
	ep->EncodeBlock(
//	BT1H_EncodeBlockRGBI(
		block, blksz, tblk, 4, 4*4, tflip, ep);
}

BTIC1H_PTune *btic1h_dbg_ptune=NULL;

void BTIC1H_EncodeImageYUY2(byte *block, int blkstride,
	byte *yuv, int xs, int ys, int qf)
{
//	int df, d2x2, d4x4x1, d4x4x2, qr;
	byte *yuv2, *yuv3;
	float qrf;
	int qr, ystr;
	bt1h_encparm tep;
	bt1h_encparm *ep;
	int xs1, ys1, xs2, ys2;
	int i, j;
	
	BT1H_EncodeBlockInit();
	
	qr=100-(qf&127);
	if(qr<0)qr=0;
	
	ep=&tep;

//	ep->EncodeBlock=BT1H_EncodeBlockRGBI;

	qrf=qr/50.0;
	ep->dflat  =qr*(0.33+0.5*qrf);
//	ep->d2x2   =qr*(0.85+0.7*qrf);
//	ep->d4x4x1 =qr*(1.50+1.0*qrf);
//	ep->d4x4x2 =qr*(3.00+2.0*qrf);
//	ep->d4x4x3 =qr*(9.00+2.0*qrf);
	ep->d2x2   =qr*(0.70+0.7*qrf);
	ep->d4x4x1 =qr*(0.75+1.0*qrf);
	ep->d4x4x2 =qr*(2.00+2.0*qrf);
	ep->d4x4x3 =qr*(8.00+2.0*qrf);
//	ep->dchflat=qr*(0.45+2.5*qrf);
//	ep->dchflat=qr*(0.15+1.5*qrf);
//	ep->dchflat=qr*(0.15+1.0*qrf);
	ep->dchflat=qr*(0.45+0.7*qrf);

#if 0
//	ep->dflat  =qr*0.56;
	ep->dflat  =qr*0.33;
	ep->d2x2   =qr*0.85;
	ep->d4x4x1 =qr*1.5;
	ep->d4x4x2 =qr*3.0;
	ep->d4x4x3 =qr*9.0;
//	ep->dchflat=ep->dflat;
	ep->dchflat=qr*0.45;
	
	if((qf&127)<50)
	{
		ep->dflat  =qr*0.75;
		ep->d2x2   =qr*1.5;
		ep->d4x4x1 =qr*2.5;
		ep->d4x4x2 =qr*5.0;
		ep->d4x4x3 =qr*9.0;
		ep->dchflat=qr*2.75;
	}
#endif

	if(blkstride<32)
	{
		ep->d4x4x3=256;
	}
	
	if(ys<0)
	{
		ys=-ys;
		ystr=-2*xs;
		yuv2=yuv+xs*(ys-1)*2;
	}else
	{
		ystr=2*xs;
		yuv2=yuv;
	}
	
	ep->d2x2h=(ep->dflat+ep->d2x2)>>1;

	xs1=xs>>2; ys1=ys>>2;
	xs2=(xs+3)>>2; ys2=(ys+3)>>2;
	for(i=0; i<ys1; i++)
	{
		yuv3=yuv2+(i*4*ystr);
		for(j=0; j<xs1; j++)
		{
			BT1H_EncodeBlockYUY2(
				block+(i*xs2+j)*blkstride, blkstride,
				yuv3+(j*4*2),
				2, ystr, ep);
		}
	}
}

// BTIC1H_PTune *btic1h_dbg_ptune=NULL;

void BTIC1H_EncodeImageRGBI(byte *block, int blkstride,
	byte *yuv, int xs, int ys, int qf, int xstr, int tflip)
{
//	int df, d2x2, d4x4x1, d4x4x2, qr;
	BTIC1H_PTune *pt;
	byte *yuv2, *yuv3;
	float qrf;
	int qr, ystr;
	bt1h_encparm tep;
	bt1h_encparm *ep;
	int xs1, ys1, xs2, ys2, xf, yf;
	int i, j;
	
	BT1H_EncodeBlockInit();
	
	qr=100-(qf&127);
	if(qr<0)qr=0;
	
	ep=&tep;

	ep->EncodeBlock=BT1H_EncodeBlockRGBI;
	if(qf&BTIC1H_QFL_USEGDBDR)
		ep->EncodeBlock=BT1H_EncodeBlockRGBI_GDbDr;

	if((qf&BTIC1H_QFL_DBGPTUNE) &&
		(btic1h_dbg_ptune->parmfl&BTIC1H_PTFL_BLKENC))
	{
		pt=btic1h_dbg_ptune;
		qrf=qr/50.0;
		ep->dflat  =qr*(pt->dflat  +pt->dflate  *qrf);
		ep->d2x2   =qr*(pt->d2x2   +pt->d2x2e   *qrf);
		ep->d4x4x1 =qr*(pt->d4x4x1 +pt->d4x4x1e *qrf);
		ep->d4x4x2 =qr*(pt->d4x4x2 +pt->d4x4x2e *qrf);
		ep->d4x4x3 =qr*(pt->d4x4x3 +pt->d4x4x3e *qrf);
		ep->dchflat=qr*(pt->dchflat+pt->dchflate*qrf);
	}else
	{
		qrf=qr/50.0;

#if 0
		ep->dflat  =qr*(0.33+0.5*qrf);
//		ep->d2x2   =qr*(0.85+0.7*qrf);
//		ep->d4x4x1 =qr*(1.50+1.0*qrf);
//		ep->d4x4x2 =qr*(3.00+2.0*qrf);
//		ep->d4x4x3 =qr*(9.00+2.0*qrf);
		ep->d2x2   =qr*(0.70+0.7*qrf);
		ep->d4x4x1 =qr*(0.75+1.0*qrf);
		ep->d4x4x2 =qr*(2.00+2.0*qrf);
		ep->d4x4x3 =qr*(8.00+2.0*qrf);
//		ep->dchflat=qr*(0.45+2.5*qrf);
//		ep->dchflat=qr*(0.15+1.5*qrf);
//		ep->dchflat=qr*(0.15+1.0*qrf);
		ep->dchflat=qr*(0.35+0.7*qrf);
#endif

#if 0
		ep->dflat  =qr*( 0.90+1.00*qrf);
		ep->d2x2   =qr*( 1.60+1.30*qrf);
		ep->d4x4x1 =qr*( 1.30+1.50*qrf);
		ep->d4x4x2 =qr*( 3.25+2.80*qrf);
		ep->d4x4x3 =qr*( 8.50+2.45*qrf);
		ep->dchflat=qr*( 0.90+1.20*qrf);
#endif

#if 1
		ep->dflat  =qr*( 0.68+0.94*qrf);
		ep->d2x2   =qr*( 1.15+1.02*qrf);
		ep->d4x4x1 =qr*( 1.07+1.27*qrf);
		ep->d4x4x2 =qr*( 3.08+2.53*qrf);
		ep->d4x4x3 =qr*( 9.12+2.38*qrf);
		ep->dchflat=qr*( 0.58+0.96*qrf);
#endif

#if 0
//		1.49+1.61 1.89+2.22 2.34+2.24 3.42+4.22 11.37+2.68 1.82+2.20

		ep->dflat  =qr*( 1.49+1.61*qrf);
		ep->d2x2   =qr*( 1.89+2.22*qrf);
		ep->d4x4x1 =qr*( 2.34+2.24*qrf);
		ep->d4x4x2 =qr*( 3.42+4.22*qrf);
		ep->d4x4x3 =qr*(11.37+2.68*qrf);
		ep->dchflat=qr*( 1.82+2.20*qrf);
#endif
	}

#if 0
//	ep->dflat  =qr*0.56;
	ep->dflat  =qr*0.33;
	ep->d2x2   =qr*0.85;
	ep->d4x4x1 =qr*1.5;
	ep->d4x4x2 =qr*3.0;
	ep->d4x4x3 =qr*9.0;
	ep->dchflat=qr*0.45;
	
	if((qf&127)<50)
	{
		ep->dflat  =qr*0.75;
		ep->d2x2   =qr*1.5;
		ep->d4x4x1 =qr*2.5;
		ep->d4x4x2 =qr*5.0;
		ep->d4x4x3 =qr*9.0;
		ep->dchflat=qr*2.75;
	}
#endif
	
	if(blkstride<32)
	{
		ep->d4x4x3=256;
	}
	
	if(ys<0)
	{
		ys=-ys;
		ystr=-xstr*xs;
		yuv2=yuv+xs*(ys-1)*xstr;
	}else
	{
		ystr=xs*xstr;
		yuv2=yuv;
	}

	ep->d2x2h=(ep->dflat+ep->d2x2)>>1;

	xs1=xs>>2; ys1=ys>>2;
	xs2=(xs+3)>>2; ys2=(ys+3)>>2;
	xf=xs&3; yf=ys&3;
	for(i=0; i<ys1; i++)
	{
		yuv3=yuv2+(i*4*ystr);
		for(j=0; j<xs1; j++)
		{
			ep->EncodeBlock(
//			BT1H_EncodeBlockRGBI(
				block+(i*xs2+j)*blkstride, blkstride,
				yuv3+(j*4*xstr),
				xstr, ystr, tflip, ep);
		}
		if(xf)
		{
			BT1H_EncodeBlockEdgeRGBI(
				block+(i*xs2+j)*blkstride, blkstride,
				yuv3+(j*4*xstr),
				xstr, ystr, xf, 4, tflip, ep);
		}
	}

	if(yf)
	{
		yuv3=yuv2+(i*4*ystr);
		for(j=0; j<xs1; j++)
		{
			BT1H_EncodeBlockEdgeRGBI(
				block+(i*xs2+j)*blkstride, blkstride,
				yuv3+(j*4*xstr),
				xstr, ystr, 4, yf, tflip, ep);
		}
		if(xf)
		{
			BT1H_EncodeBlockEdgeRGBI(
				block+(i*xs2+j)*blkstride, blkstride,
				yuv3+(j*4*xstr),
				xstr, ystr, xf, yf, tflip, ep);
		}
	}
}

void BTIC1H_EncodeImageClrs(byte *block, int blkstride,
	byte *rgba, int xs, int ys, int qf, int clrs)
{
	int xstr, ystr, tflip;
	int xs1, ys1;
	int i, j;
	
	if(clrs==BTIC1H_PXF_YUYV)
	{
		BTIC1H_EncodeImageYUY2(block, blkstride, rgba, xs, ys, qf);
		return;
	}

	if(clrs==BTIC1H_PXF_RGBA)
		{ xstr=4; ystr=xstr*xs; tflip=2; }
	if(clrs==BTIC1H_PXF_BGRA)
		{ xstr=4; ystr=xstr*xs; tflip=3; }
	if(clrs==BTIC1H_PXF_RGBX)
		{ xstr=4; ystr=xstr*xs; tflip=0; }
	if(clrs==BTIC1H_PXF_BGRX)
		{ xstr=4; ystr=xstr*xs; tflip=1; }
	if(clrs==BTIC1H_PXF_RGB)
		{ xstr=3; ystr=xstr*xs; tflip=0; }
	if(clrs==BTIC1H_PXF_BGR)
		{ xstr=3; ystr=xstr*xs; tflip=1; }
	
	BTIC1H_EncodeImageRGBI(block, blkstride, rgba,
		xs, ys, qf, xstr, tflip);
}


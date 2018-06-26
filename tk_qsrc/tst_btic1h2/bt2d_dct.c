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

void BTIC2D_TransDCT_Horiz(int *iblk, int *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int ib0p7, ib1p6, ib2p5, ib3p4;
	int ib0n7, ib1n6, ib2n5, ib3n4;
	int ib0p7n3n4, ib1p6n2n5;

	ib0=iblk[0];	ib1=iblk[1];
	ib2=iblk[2];	ib3=iblk[3];
	ib4=iblk[4];	ib5=iblk[5];
	ib6=iblk[6];	ib7=iblk[7];

	ib0p7=ib0+ib7;	ib1p6=ib1+ib6;
	ib2p5=ib2+ib5;	ib3p4=ib3+ib4;
	ib0n7=ib0-ib7;	ib1n6=ib1-ib6;
	ib2n5=ib2-ib5;	ib3n4=ib3-ib4;

	oblk[0]=(ib0p7+ib1p6+ib2p5+ib3p4)*91;
	oblk[4]=(ib0p7-ib1p6-ib2p5+ib3p4)*91;

	ib0p7n3n4=ib0p7-ib3p4;
	ib1p6n2n5=ib1p6-ib2p5;
	oblk[2]=ib0p7n3n4*118 +ib1p6n2n5* 49;
	oblk[6]=ib0p7n3n4* 49 -ib1p6n2n5*118;

	oblk[1]=ib0n7*126 +ib1n6*106 +ib2n5* 71 +ib3n4* 25;
	oblk[3]=ib0n7*106 -ib1n6* 25 -ib2n5*126 -ib3n4* 71;
	oblk[5]=ib0n7* 71 -ib1n6*126 +ib2n5* 25 +ib3n4*106;
	oblk[7]=ib0n7* 25 -ib1n6* 71 +ib2n5*106 -ib3n4*126;
}

void BTIC2D_TransDCT_Vert(int *iblk, int *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int ib0p7, ib1p6, ib2p5, ib3p4;
	int ib0n7, ib1n6, ib2n5, ib3n4;
	int ib0p7n3n4, ib1p6n2n5;

	ib0=iblk[ 0];	ib1=iblk[ 8];
	ib2=iblk[16];	ib3=iblk[24];
	ib4=iblk[32];	ib5=iblk[40];
	ib6=iblk[48];	ib7=iblk[56];

	ib0p7=ib0+ib7;	ib1p6=ib1+ib6;
	ib2p5=ib2+ib5;	ib3p4=ib3+ib4;
	ib0n7=ib0-ib7;	ib1n6=ib1-ib6;
	ib2n5=ib2-ib5;	ib3n4=ib3-ib4;

	ob0=(ib0p7+ib1p6+ib2p5+ib3p4)*91;
	ob4=(ib0p7-ib1p6-ib2p5+ib3p4)*91;

	ib0p7n3n4=ib0p7-ib3p4;
	ib1p6n2n5=ib1p6-ib2p5;
	ob2=ib0p7n3n4*118 +ib1p6n2n5* 49;
	ob6=ib0p7n3n4* 49 -ib1p6n2n5*118;

	ob1=ib0n7*126 +ib1n6*106 +ib2n5* 71 +ib3n4* 25;
	ob3=ib0n7*106 -ib1n6* 25 -ib2n5*126 -ib3n4* 71;
	ob5=ib0n7* 71 -ib1n6*126 +ib2n5* 25 +ib3n4*106;
	ob7=ib0n7* 25 -ib1n6* 71 +ib2n5*106 -ib3n4*126;

	oblk[ 0]=(ob0+32768)>>16;	oblk[ 8]=(ob1+32768)>>16;
	oblk[16]=(ob2+32768)>>16;	oblk[24]=(ob3+32768)>>16;
	oblk[32]=(ob4+32768)>>16;	oblk[40]=(ob5+32768)>>16;
	oblk[48]=(ob6+32768)>>16;	oblk[56]=(ob7+32768)>>16;
}

void BTIC2D_TransDCT(int *iblk, int *oblk)
{
	int s[64];
	int i, j;

	BTIC2D_TransDCT_Horiz(iblk+0, s+0);
	BTIC2D_TransDCT_Horiz(iblk+8, s+8);
	BTIC2D_TransDCT_Horiz(iblk+16, s+16);
	BTIC2D_TransDCT_Horiz(iblk+24, s+24);
	BTIC2D_TransDCT_Horiz(iblk+32, s+32);
	BTIC2D_TransDCT_Horiz(iblk+40, s+40);
	BTIC2D_TransDCT_Horiz(iblk+48, s+48);
	BTIC2D_TransDCT_Horiz(iblk+56, s+56);

	BTIC2D_TransDCT_Vert(s+0, oblk+0);
	BTIC2D_TransDCT_Vert(s+1, oblk+1);
	BTIC2D_TransDCT_Vert(s+2, oblk+2);
	BTIC2D_TransDCT_Vert(s+3, oblk+3);
	BTIC2D_TransDCT_Vert(s+4, oblk+4);
	BTIC2D_TransDCT_Vert(s+5, oblk+5);
	BTIC2D_TransDCT_Vert(s+6, oblk+6);
	BTIC2D_TransDCT_Vert(s+7, oblk+7);
}


void BTIC2D_TransDCT_Stride4(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3;
	int ib0p, ib1p, ib0n, ib1n;

	ib0=iblk[istride*0];
	ib1=iblk[istride*1];
	ib2=iblk[istride*2];
	ib3=iblk[istride*3];

	ib0p=ib0+ib3;	ib0n=ib0-ib3;
	ib1p=ib1+ib2;	ib1n=ib1-ib2;

	oblk[ostride*0]=(ib0p+ib1p)*128;
	oblk[ostride*2]=(ib0p-ib1p)*128;
	oblk[ostride*1]=ib0n*167+ib1n* 69;
	oblk[ostride*3]=ib0n* 69-ib1n*167;
}

void BTIC2D_TransDCT4Stride(int *iblk, int *oblk,
	int xstride, int ystride)
{
	int s[16], t[16];
	int i, j;

	BTIC2D_TransDCT_Stride4(iblk+ystride*0, s+4*0, xstride, 1);
	BTIC2D_TransDCT_Stride4(iblk+ystride*1, s+4*1, xstride, 1);
	BTIC2D_TransDCT_Stride4(iblk+ystride*2, s+4*2, xstride, 1);
	BTIC2D_TransDCT_Stride4(iblk+ystride*3, s+4*3, xstride, 1);

	BTIC2D_TransDCT_Stride4(s+0, t+0, 4, 4);
	BTIC2D_TransDCT_Stride4(s+1, t+1, 4, 4);
	BTIC2D_TransDCT_Stride4(s+2, t+2, 4, 4);
	BTIC2D_TransDCT_Stride4(s+3, t+3, 4, 4);

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		oblk[i*ystride+j*xstride]=(t[i*4+j]+32768)>>16;
	}
}

void BTIC2D_TransDCT4(int *iblk, int *oblk)
{
	int s[16], t[16];
	int i, j;

	BTIC2D_TransDCT_Stride4(iblk+4*0, s+4*0, 1, 1);
	BTIC2D_TransDCT_Stride4(iblk+4*1, s+4*1, 1, 1);
	BTIC2D_TransDCT_Stride4(iblk+4*2, s+4*2, 1, 1);
	BTIC2D_TransDCT_Stride4(iblk+4*3, s+4*3, 1, 1);

	BTIC2D_TransDCT_Stride4(s+0, t+0, 4, 4);
	BTIC2D_TransDCT_Stride4(s+1, t+1, 4, 4);
	BTIC2D_TransDCT_Stride4(s+2, t+2, 4, 4);
	BTIC2D_TransDCT_Stride4(s+3, t+3, 4, 4);

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		oblk[i*4+j]=(t[i*4+j]+32768)>>16;
	}
}


void BTIC2D_TransWHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[istride*0];	ib1=iblk[istride*1];
	ib2=iblk[istride*2];	ib3=iblk[istride*3];
	ib4=iblk[istride*4];	ib5=iblk[istride*5];
	ib6=iblk[istride*6];	ib7=iblk[istride*7];

	a0=ib0+ib4;			a1=ib1+ib5;
	a2=ib2+ib6;			a3=ib3+ib7;
	a4=(a0>>1)-ib4;		a5=(a1>>1)-ib5;
	a6=(a2>>1)-ib6;		a7=(a3>>1)-ib7;

	b0=a0+a2;			b1=a1+a3;
	b2=(b0>>1)-a2;		b3=(b1>>1)-a3;
	b4=a4+a6;			b5=a5+a7;
	b6=(b4>>1)-a6;		b7=(b5>>1)-a7;

	c0=b0+b1;			c1=(c0>>1)-b1;
	c2=b2+b3;			c3=(c2>>1)-b3;
	c4=b4+b5;			c5=(c4>>1)-b5;
	c6=b6+b7;			c7=(c6>>1)-b7;

	oblk[ostride*0]=c0;		oblk[ostride*1]=c4;
	oblk[ostride*2]=c6;		oblk[ostride*3]=c2;
	oblk[ostride*4]=c3;		oblk[ostride*5]=c7;
	oblk[ostride*6]=c5;		oblk[ostride*7]=c1;
}

force_inline void BTIC2D_TransWHT_Stride4(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3;
	int a0, a1, a2, a3;
	int b0, b1, b2, b3;

	ib0=iblk[istride*0];	ib1=iblk[istride*1];
	ib2=iblk[istride*2];	ib3=iblk[istride*3];

	a0=ib0+ib2;			a1=ib1+ib3;
	a2=(a0>>1)-ib2;		a3=(a1>>1)-ib3;

	b0=a0+a1;			b1=(b0>>1)-a1;
	b2=a2+a3;			b3=(b2>>1)-a3;

	oblk[ostride*0]=b0;		oblk[ostride*1]=b2;
	oblk[ostride*2]=b3;		oblk[ostride*3]=b1;
}

force_inline void BTIC2D_TransWHT_Horz8(int *iblk, int *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[0];	ib1=iblk[1];	ib2=iblk[2];	ib3=iblk[3];
	ib4=iblk[4];	ib5=iblk[5];	ib6=iblk[6];	ib7=iblk[7];

	a0=ib0+ib4;			a1=ib1+ib5;
	a2=ib2+ib6;			a3=ib3+ib7;
	a4=(a0>>1)-ib4;		a5=(a1>>1)-ib5;
	a6=(a2>>1)-ib6;		a7=(a3>>1)-ib7;

	b0=a0+a2;			b1=a1+a3;
	b2=(b0>>1)-a2;		b3=(b1>>1)-a3;
	b4=a4+a6;			b5=a5+a7;
	b6=(b4>>1)-a6;		b7=(b5>>1)-a7;

	c0=b0+b1;			c1=(c0>>1)-b1;
	c2=b2+b3;			c3=(c2>>1)-b3;
	c4=b4+b5;			c5=(c4>>1)-b5;
	c6=b6+b7;			c7=(c6>>1)-b7;

	oblk[0]=c0;		oblk[1]=c4;		oblk[2]=c6;		oblk[3]=c2;
	oblk[4]=c3;		oblk[5]=c7;		oblk[6]=c5;		oblk[7]=c1;
}

force_inline void BTIC2D_TransWHT_Vert8(int *iblk, int *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[8*0];	ib1=iblk[8*1];	ib2=iblk[8*2];	ib3=iblk[8*3];
	ib4=iblk[8*4];	ib5=iblk[8*5];	ib6=iblk[8*6];	ib7=iblk[8*7];

	a0=ib0+ib4;			a1=ib1+ib5;
	a2=ib2+ib6;			a3=ib3+ib7;
	a4=(a0>>1)-ib4;		a5=(a1>>1)-ib5;
	a6=(a2>>1)-ib6;		a7=(a3>>1)-ib7;

	b0=a0+a2;			b1=a1+a3;
	b2=(b0>>1)-a2;		b3=(b1>>1)-a3;
	b4=a4+a6;			b5=a5+a7;
	b6=(b4>>1)-a6;		b7=(b5>>1)-a7;

	c0=b0+b1;			c1=(c0>>1)-b1;
	c2=b2+b3;			c3=(c2>>1)-b3;
	c4=b4+b5;			c5=(c4>>1)-b5;
	c6=b6+b7;			c7=(c6>>1)-b7;

	oblk[8*0]=c0;	oblk[8*1]=c4;	oblk[8*2]=c6;	oblk[8*3]=c2;
	oblk[8*4]=c3;	oblk[8*5]=c7;	oblk[8*6]=c5;	oblk[8*7]=c1;
}

void BTIC2D_TransWHT8(int *iblk, int *oblk)
{
	int s[64];
	int i, j;

#if 0
	BTIC2D_TransWHT_Stride8(iblk+8*0, s+8*0, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*1, s+8*1, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*2, s+8*2, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*3, s+8*3, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*4, s+8*4, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*5, s+8*5, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*6, s+8*6, 1, 1);
	BTIC2D_TransWHT_Stride8(iblk+8*7, s+8*7, 1, 1);

	BTIC2D_TransWHT_Stride8(s+0, oblk+0, 8, 8);
	BTIC2D_TransWHT_Stride8(s+1, oblk+1, 8, 8);
	BTIC2D_TransWHT_Stride8(s+2, oblk+2, 8, 8);
	BTIC2D_TransWHT_Stride8(s+3, oblk+3, 8, 8);
	BTIC2D_TransWHT_Stride8(s+4, oblk+4, 8, 8);
	BTIC2D_TransWHT_Stride8(s+5, oblk+5, 8, 8);
	BTIC2D_TransWHT_Stride8(s+6, oblk+6, 8, 8);
	BTIC2D_TransWHT_Stride8(s+7, oblk+7, 8, 8);
#endif

#if 1
	BTIC2D_TransWHT_Horz8(iblk+8*0, s+8*0);
	BTIC2D_TransWHT_Horz8(iblk+8*1, s+8*1);
	BTIC2D_TransWHT_Horz8(iblk+8*2, s+8*2);
	BTIC2D_TransWHT_Horz8(iblk+8*3, s+8*3);
	BTIC2D_TransWHT_Horz8(iblk+8*4, s+8*4);
	BTIC2D_TransWHT_Horz8(iblk+8*5, s+8*5);
	BTIC2D_TransWHT_Horz8(iblk+8*6, s+8*6);
	BTIC2D_TransWHT_Horz8(iblk+8*7, s+8*7);

	BTIC2D_TransWHT_Vert8(s+0, oblk+0);
	BTIC2D_TransWHT_Vert8(s+1, oblk+1);
	BTIC2D_TransWHT_Vert8(s+2, oblk+2);
	BTIC2D_TransWHT_Vert8(s+3, oblk+3);
	BTIC2D_TransWHT_Vert8(s+4, oblk+4);
	BTIC2D_TransWHT_Vert8(s+5, oblk+5);
	BTIC2D_TransWHT_Vert8(s+6, oblk+6);
	BTIC2D_TransWHT_Vert8(s+7, oblk+7);
#endif
}

void BTIC2D_TransWHT4(int *iblk, int *oblk)
{
	int s[16];
	int i, j;

	BTIC2D_TransWHT_Stride4(iblk+4*0, s+4*0, 1, 1);
	BTIC2D_TransWHT_Stride4(iblk+4*1, s+4*1, 1, 1);
	BTIC2D_TransWHT_Stride4(iblk+4*2, s+4*2, 1, 1);
	BTIC2D_TransWHT_Stride4(iblk+4*3, s+4*3, 1, 1);

	BTIC2D_TransWHT_Stride4(s+0, oblk+0, 4, 4);
	BTIC2D_TransWHT_Stride4(s+1, oblk+1, 4, 4);
	BTIC2D_TransWHT_Stride4(s+2, oblk+2, 4, 4);
	BTIC2D_TransWHT_Stride4(s+3, oblk+3, 4, 4);
}

#if 1
force_inline void BTIC2D_TransBHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[istride*0];	ib1=iblk[istride*1];
	ib2=iblk[istride*2];	ib3=iblk[istride*3];
	ib4=iblk[istride*4];	ib5=iblk[istride*5];
	ib6=iblk[istride*6];	ib7=iblk[istride*7];

	a0=ib0+ib4;			a1=ib1+ib5;
	a2=ib2+ib6;			a3=ib3+ib7;
	a4=(a0>>1)-ib4;		a5=(a1>>1)-ib5;
	a6=(a2>>1)-ib6;		a7=(a3>>1)-ib7;
	b0=a0+a2;			b1=a1+a3;
	b2=(b0>>1)-a2;		b3=(b1>>1)-a3;

	c0=b0+b1;			c1=(c0>>1)-b1;

	oblk[ostride*0]=c0;		oblk[ostride*1]=c1;
	oblk[ostride*2]=b2;		oblk[ostride*3]=b3;
	oblk[ostride*4]=a4;		oblk[ostride*5]=a5;
	oblk[ostride*6]=a6;		oblk[ostride*7]=a7;
}
#endif

#if 0
force_inline void BTIC2D_TransBHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[istride*0];	ib1=iblk[istride*1];
	ib2=iblk[istride*2];	ib3=iblk[istride*3];
	ib4=iblk[istride*4];	ib5=iblk[istride*5];
	ib6=iblk[istride*6];	ib7=iblk[istride*7];

	a0=ib0+ib1;			a1=ib2+ib3;
	a2=ib4+ib5;			a3=ib6+ib7;
	a4=(a0>>1)-ib1;		a5=(a1>>1)-ib3;
	a6=(a2>>1)-ib5;		a7=(a3>>1)-ib7;
	b0=a0+a1;			b1=a2+a3;
	b2=(b0>>1)-a1;		b3=(b1>>1)-a3;

	c0=b0+b1;			c1=(c0>>1)-b1;

	oblk[ostride*0]=c0;		oblk[ostride*1]=c1;
	oblk[ostride*2]=b2;		oblk[ostride*3]=b3;
	oblk[ostride*4]=a4;		oblk[ostride*5]=a5;
	oblk[ostride*6]=a6;		oblk[ostride*7]=a7;
}
#endif

force_inline void BTIC2D_TransBHT_Stride4(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3;
	int a0, a1, a2, a3;
	int b0, b1, b2, b3;

	ib0=iblk[istride*0];	ib1=iblk[istride*1];
	ib2=iblk[istride*2];	ib3=iblk[istride*3];

	a0=ib0+ib2;			a1=ib1+ib3;
	a2=(a0>>1)-ib2;		a3=(a1>>1)-ib3;
	b0=a0+a1;			b1=(b0>>1)-a1;

	oblk[ostride*0]=b0;		oblk[ostride*1]=b1;
	oblk[ostride*2]=a2;		oblk[ostride*3]=a3;
}

void BTIC2D_TransBHT8(int *iblk, int *oblk)
{
	int s[64];
	int i, j;

#if 1
	BTIC2D_TransBHT_Stride8(iblk+8*0, s+8*0, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*1, s+8*1, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*2, s+8*2, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*3, s+8*3, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*4, s+8*4, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*5, s+8*5, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*6, s+8*6, 1, 1);
	BTIC2D_TransBHT_Stride8(iblk+8*7, s+8*7, 1, 1);

	BTIC2D_TransBHT_Stride8(s+0, oblk+0, 8, 8);
	BTIC2D_TransBHT_Stride8(s+1, oblk+1, 8, 8);
	BTIC2D_TransBHT_Stride8(s+2, oblk+2, 8, 8);
	BTIC2D_TransBHT_Stride8(s+3, oblk+3, 8, 8);
	BTIC2D_TransBHT_Stride8(s+4, oblk+4, 8, 8);
	BTIC2D_TransBHT_Stride8(s+5, oblk+5, 8, 8);
	BTIC2D_TransBHT_Stride8(s+6, oblk+6, 8, 8);
	BTIC2D_TransBHT_Stride8(s+7, oblk+7, 8, 8);
#endif

#if 0
	BTIC2D_TransWHT_Horz8(iblk+8*0, s+8*0);
	BTIC2D_TransWHT_Horz8(iblk+8*1, s+8*1);
	BTIC2D_TransWHT_Horz8(iblk+8*2, s+8*2);
	BTIC2D_TransWHT_Horz8(iblk+8*3, s+8*3);
	BTIC2D_TransWHT_Horz8(iblk+8*4, s+8*4);
	BTIC2D_TransWHT_Horz8(iblk+8*5, s+8*5);
	BTIC2D_TransWHT_Horz8(iblk+8*6, s+8*6);
	BTIC2D_TransWHT_Horz8(iblk+8*7, s+8*7);

	BTIC2D_TransWHT_Vert8(s+0, oblk+0);
	BTIC2D_TransWHT_Vert8(s+1, oblk+1);
	BTIC2D_TransWHT_Vert8(s+2, oblk+2);
	BTIC2D_TransWHT_Vert8(s+3, oblk+3);
	BTIC2D_TransWHT_Vert8(s+4, oblk+4);
	BTIC2D_TransWHT_Vert8(s+5, oblk+5);
	BTIC2D_TransWHT_Vert8(s+6, oblk+6);
	BTIC2D_TransWHT_Vert8(s+7, oblk+7);
#endif
}

void BTIC2D_TransBHT4(int *iblk, int *oblk)
{
	int s[16];
	int i, j;

	BTIC2D_TransBHT_Stride4(iblk+4*0, s+4*0, 1, 1);
	BTIC2D_TransBHT_Stride4(iblk+4*1, s+4*1, 1, 1);
	BTIC2D_TransBHT_Stride4(iblk+4*2, s+4*2, 1, 1);
	BTIC2D_TransBHT_Stride4(iblk+4*3, s+4*3, 1, 1);

	BTIC2D_TransBHT_Stride4(s+0, oblk+0, 4, 4);
	BTIC2D_TransBHT_Stride4(s+1, oblk+1, 4, 4);
	BTIC2D_TransBHT_Stride4(s+2, oblk+2, 4, 4);
	BTIC2D_TransBHT_Stride4(s+3, oblk+3, 4, 4);
}

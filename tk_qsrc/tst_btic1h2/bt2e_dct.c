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



#if 0
force_inline void BTIC2D_TransIBHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[istride*0];
	c1=iblk[istride*1];
	c2=iblk[istride*2];
	c3=iblk[istride*3];
	c4=iblk[istride*4];
	c5=iblk[istride*5];
	c6=iblk[istride*6];
	c7=iblk[istride*7];

	b1=(c0>>1)-c1;
	b0=c0-b1;

	a2=(b0>>1)-c2;
	a3=(b1>>1)-c3;
	a0=b0-a2;
	a1=b1-a3;

//	a6=(b4>>1)-b6;
//	a7=(b5>>1)-b7;
//	a4=b4-a6;
//	a5=b5-a7;

	ob4=(a0>>1)-c4;
	ob5=(a1>>1)-c5;
	ob6=(a2>>1)-c6;
	ob7=(a3>>1)-c7;
	ob0=a0-ob4;
	ob1=a1-ob5;
	ob2=a2-ob6;
	ob3=a3-ob7;

	oblk[ostride*0]=ob0;
	oblk[ostride*1]=ob1;
	oblk[ostride*2]=ob2;
	oblk[ostride*3]=ob3;
	oblk[ostride*4]=ob4;
	oblk[ostride*5]=ob5;
	oblk[ostride*6]=ob6;
	oblk[ostride*7]=ob7;
}
#endif

#if 1
force_inline void BTIC2D_TransIBHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[istride*0];
	c1=iblk[istride*1];
	c2=iblk[istride*2];
	c3=iblk[istride*3];
	c4=iblk[istride*4];
	c5=iblk[istride*5];
	c6=iblk[istride*6];
	c7=iblk[istride*7];

	b1=(c0>>1)-c1;
	b0=c0-b1;

	a1=(b0>>1)-c2;
	a0=b0-a1;

	a3=(b1>>1)-c3;
	a2=b1-a3;

//	a6=(b4>>1)-b6;
//	a7=(b5>>1)-b7;
//	a4=b4-a6;
//	a5=b5-a7;

	ob1=(a0>>1)-c4;
	ob0=a0-ob1;
	ob3=(a1>>1)-c5;
	ob2=a1-ob3;
	ob5=(a2>>1)-c6;
	ob4=a2-ob5;
	ob7=(a3>>1)-c7;
	ob6=a3-ob7;

	oblk[ostride*0]=ob0;
	oblk[ostride*1]=ob1;
	oblk[ostride*2]=ob2;
	oblk[ostride*3]=ob3;
	oblk[ostride*4]=ob4;
	oblk[ostride*5]=ob5;
	oblk[ostride*6]=ob6;
	oblk[ostride*7]=ob7;
}
#endif

void BTIC2D_TransIBHT8Stride(int *iblk, int *oblk,
	int ixstride, int iystride,
	int oxstride, int oystride)
{
	int s[64], t[64];
	int i, j;

	BTIC2D_TransIBHT_Stride8(iblk+ixstride*0, s+0, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*1, s+1, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*2, s+2, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*3, s+3, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*4, s+4, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*5, s+5, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*6, s+6, iystride, 8);
	BTIC2D_TransIBHT_Stride8(iblk+ixstride*7, s+7, iystride, 8);

	BTIC2D_TransIBHT_Stride8(s+0*8, t+0*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+1*8, t+1*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+2*8, t+2*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+3*8, t+3*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+4*8, t+4*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+5*8, t+5*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+6*8, t+6*8, 1, 1);
	BTIC2D_TransIBHT_Stride8(s+7*8, t+7*8, 1, 1);

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		oblk[i*oystride+j*oxstride]=t[i*8+j]+128;
	}
}

void BTIC2D_TransIWHT_Stride8(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[istride*0];
	c4=iblk[istride*1];
	c6=iblk[istride*2];
	c2=iblk[istride*3];
	c3=iblk[istride*4];
	c7=iblk[istride*5];
	c5=iblk[istride*6];
	c1=iblk[istride*7];

	b1=(c0>>1)-c1;
	b0=c0-b1;
	b3=(c2>>1)-c3;
	b2=c2-b3;
	b5=(c4>>1)-c5;
	b4=c4-b5;
	b7=(c6>>1)-c7;
	b6=c6-b7;

	a2=(b0>>1)-b2;
	a3=(b1>>1)-b3;
	a0=b0-a2;
	a1=b1-a3;
	a6=(b4>>1)-b6;
	a7=(b5>>1)-b7;
	a4=b4-a6;
	a5=b5-a7;

	ob4=(a0>>1)-a4;
	ob5=(a1>>1)-a5;
	ob6=(a2>>1)-a6;
	ob7=(a3>>1)-a7;
	ob0=a0-ob4;
	ob1=a1-ob5;
	ob2=a2-ob6;
	ob3=a3-ob7;

	oblk[ostride*0]=ob0;
	oblk[ostride*1]=ob1;
	oblk[ostride*2]=ob2;
	oblk[ostride*3]=ob3;
	oblk[ostride*4]=ob4;
	oblk[ostride*5]=ob5;
	oblk[ostride*6]=ob6;
	oblk[ostride*7]=ob7;
}

void BTIC2D_TransIWHT8Stride(int *iblk, int *oblk,
	int ixstride, int iystride,
	int oxstride, int oystride)
{
	int s[64], t[64];
	int i, j;

	BTIC2D_TransIWHT_Stride8(iblk+ixstride*0, s+0, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*1, s+1, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*2, s+2, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*3, s+3, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*4, s+4, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*5, s+5, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*6, s+6, iystride, 8);
	BTIC2D_TransIWHT_Stride8(iblk+ixstride*7, s+7, iystride, 8);

	BTIC2D_TransIWHT_Stride8(s+0*8, t+0*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+1*8, t+1*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+2*8, t+2*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+3*8, t+3*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+4*8, t+4*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+5*8, t+5*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+6*8, t+6*8, 1, 1);
	BTIC2D_TransIWHT_Stride8(s+7*8, t+7*8, 1, 1);

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		oblk[i*oystride+j*oxstride]=t[i*8+j]+128;
	}
}

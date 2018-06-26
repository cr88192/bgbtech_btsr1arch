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
 *  0  1  2  3  4  5  6  7
 *  8  9 10 11 12 13 14 15
 * 16 17 18 19 20 21 22 23
 * 
 *
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 *
 * 0 1 4 8 5 2 3 6 9 12 13 10 7 11 14 15
 */

static const int pdjpg_zigzag8_1[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

static const int pdjpg_zigzag8_2[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};

void BTIC2D_QuantReorder(int *iblk, int *oblk, int *ifqt)
{
#if 1
	int i, j;
	
	for(i=0; i<8; i++)
	{
		j=i*8;
		oblk[j+0]=(iblk[pdjpg_zigzag8_2[j+0]]*ifqt[j+0]+2048)>>12;
		oblk[j+1]=(iblk[pdjpg_zigzag8_2[j+1]]*ifqt[j+1]+2048)>>12;
		oblk[j+2]=(iblk[pdjpg_zigzag8_2[j+2]]*ifqt[j+2]+2048)>>12;
		oblk[j+3]=(iblk[pdjpg_zigzag8_2[j+3]]*ifqt[j+3]+2048)>>12;
		oblk[j+4]=(iblk[pdjpg_zigzag8_2[j+4]]*ifqt[j+4]+2048)>>12;
		oblk[j+5]=(iblk[pdjpg_zigzag8_2[j+5]]*ifqt[j+5]+2048)>>12;
		oblk[j+6]=(iblk[pdjpg_zigzag8_2[j+6]]*ifqt[j+6]+2048)>>12;
		oblk[j+7]=(iblk[pdjpg_zigzag8_2[j+7]]*ifqt[j+7]+2048)>>12;
	}
#endif

#if 0
#define YBVQ(x) \
	oblk[x+0]=(iblk[pdjpg_zigzag8_2[x+0]]*ifqt[x+0]+2048)>>12;	\
	oblk[x+1]=(iblk[pdjpg_zigzag8_2[x+1]]*ifqt[x+1]+2048)>>12;	\
	oblk[x+2]=(iblk[pdjpg_zigzag8_2[x+2]]*ifqt[x+2]+2048)>>12;	\
	oblk[x+3]=(iblk[pdjpg_zigzag8_2[x+3]]*ifqt[x+3]+2048)>>12;	\
	oblk[x+4]=(iblk[pdjpg_zigzag8_2[x+4]]*ifqt[x+4]+2048)>>12;	\
	oblk[x+5]=(iblk[pdjpg_zigzag8_2[x+5]]*ifqt[x+5]+2048)>>12;	\
	oblk[x+6]=(iblk[pdjpg_zigzag8_2[x+6]]*ifqt[x+6]+2048)>>12;	\
	oblk[x+7]=(iblk[pdjpg_zigzag8_2[x+7]]*ifqt[x+7]+2048)>>12;

	YBVQ(0)		YBVQ(8)
	YBVQ(16)	YBVQ(24)
	YBVQ(32)	YBVQ(40)
	YBVQ(48)	YBVQ(56)
#endif
}

void BTIC2D_TransBlock2YUY2(
	byte *iblk, byte *ibufs, byte *ibufe,
	int ystr, int *oblk, int *ifqt)
{
	int yblka[4*64], ublka[64], vblka[64];
	byte *ics, *ics0, *ics1;
	int *yct, *uct, *vct;
	int *oct;
	int y0, y1, y2, y3, u0, u1, v0, v1;
	int i, j, k, l;

	ics0=iblk;
	ics1=ics0+(16*ystr)+(16*2);
	
	if((ics0<ibufs) || (ics0>=ibufe) || (ics1<ibufs) || (ics1>=ibufe))
	{
		l=0;
		for(i=0; i<8; i++)
			for(j=0; j<8; j++)
		{
			ics=iblk+(i+0)*ystr+(j+0)*2;
			yct=yblka+0*64+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			yct[0]=l;

			ics=iblk+(i+0)*ystr+(j+8)*2;
			yct=yblka+1*64+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			yct[0]=l;

			ics=iblk+(i+8)*ystr+(j+0)*2;
			yct=yblka+2*64+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			yct[0]=l;

			ics=iblk+(i+8)*ystr+(j+8)*2;
			yct=yblka+3*64+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			yct[0]=l;
		}

		l=0;
		for(i=0; i<8; i++)
			for(j=0; j<8; j++)
		{
			ics=iblk+(i*2)*ystr+j*4+1;
			uct=ublka+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			uct[0]=l;
		}

		l=0;
		for(i=0; i<8; i++)
			for(j=0; j<8; j++)
		{
			ics=iblk+(i*2)*ystr+j*4+3;
			vct=vblka+i*8+j;
			if((ics>=ibufs) && (ics<ibufe))
				{ l=ics[0]-128; }
			vct[0]=l;
		}
	}else
	{
		for(i=0; i<8; i++)
		{
			ics=iblk+i*ystr;

			yct=yblka+0*64+i*8;
			yct[0]=ics[ 0]-128;
			yct[1]=ics[ 2]-128;
			yct[2]=ics[ 4]-128;
			yct[3]=ics[ 6]-128;
			yct[4]=ics[ 8]-128;
			yct[5]=ics[10]-128;
			yct[6]=ics[12]-128;
			yct[7]=ics[14]-128;

			yct=yblka+1*64+i*8;
			yct[0]=ics[16]-128;
			yct[1]=ics[18]-128;
			yct[2]=ics[20]-128;
			yct[3]=ics[22]-128;
			yct[4]=ics[24]-128;
			yct[5]=ics[26]-128;
			yct[6]=ics[28]-128;
			yct[7]=ics[30]-128;

			ics=iblk+(i+8)*ystr;

			yct=yblka+2*64+i*8;
			yct[0]=ics[ 0]-128;
			yct[1]=ics[ 2]-128;
			yct[2]=ics[ 4]-128;
			yct[3]=ics[ 6]-128;
			yct[4]=ics[ 8]-128;
			yct[5]=ics[10]-128;
			yct[6]=ics[12]-128;
			yct[7]=ics[14]-128;

			yct=yblka+3*64+i*8;
			yct[0]=ics[16]-128;
			yct[1]=ics[18]-128;
			yct[2]=ics[20]-128;
			yct[3]=ics[22]-128;
			yct[4]=ics[24]-128;
			yct[5]=ics[26]-128;
			yct[6]=ics[28]-128;
			yct[7]=ics[30]-128;
		}
	}

	for(i=0; i<8; i++)
	{
		ics0=iblk+(i*2+0)*ystr;
		ics1=iblk+(i*2+1)*ystr;
		uct=ublka+i*8;
		vct=vblka+i*8;
		uct[0]=((ics0[ 1]+ics1[ 1])>>1)-128;
		vct[0]=((ics0[ 3]+ics1[ 3])>>1)-128;
		uct[1]=((ics0[ 5]+ics1[ 5])>>1)-128;
		vct[1]=((ics0[ 7]+ics1[ 7])>>1)-128;
		uct[2]=((ics0[ 9]+ics1[ 9])>>1)-128;
		vct[2]=((ics0[11]+ics1[11])>>1)-128;
		uct[3]=((ics0[13]+ics1[13])>>1)-128;
		vct[3]=((ics0[15]+ics1[15])>>1)-128;

		uct[4]=((ics0[17]+ics1[17])>>1)-128;
		vct[4]=((ics0[19]+ics1[19])>>1)-128;
		uct[5]=((ics0[21]+ics1[21])>>1)-128;
		vct[5]=((ics0[23]+ics1[23])>>1)-128;
		uct[6]=((ics0[25]+ics1[25])>>1)-128;
		vct[6]=((ics0[27]+ics1[27])>>1)-128;
		uct[7]=((ics0[29]+ics1[29])>>1)-128;
		vct[7]=((ics0[31]+ics1[31])>>1)-128;
	}
	
#if 0
	BTIC2D_TransWHT8(yblka+  0, yblka+  0);
	BTIC2D_TransWHT8(yblka+ 64, yblka+ 64);
	BTIC2D_TransWHT8(yblka+128, yblka+128);
	BTIC2D_TransWHT8(yblka+192, yblka+192);

	BTIC2D_TransWHT8(ublka, ublka);
	BTIC2D_TransWHT8(vblka, vblka);
#endif

#if 1
	BTIC2D_TransBHT8(yblka+  0, yblka+  0);
	BTIC2D_TransBHT8(yblka+ 64, yblka+ 64);
	BTIC2D_TransBHT8(yblka+128, yblka+128);
	BTIC2D_TransBHT8(yblka+192, yblka+192);

	BTIC2D_TransBHT8(ublka, ublka);
	BTIC2D_TransBHT8(vblka, vblka);
#endif

	BTIC2D_QuantReorder(yblka+  0, oblk+  0, ifqt);
	BTIC2D_QuantReorder(yblka+ 64, oblk+ 64, ifqt);
	BTIC2D_QuantReorder(yblka+128, oblk+128, ifqt);
	BTIC2D_QuantReorder(yblka+192, oblk+192, ifqt);

	BTIC2D_QuantReorder(ublka, oblk+256, ifqt+64);
	BTIC2D_QuantReorder(vblka, oblk+320, ifqt+64);
}

void BTIC2D_EncodeByteVal(BTIC1H_Context *ctx, int z, int val)
{
	int uval;
	int i, j, k;
	
	uval=(val<<1)^(val>>31);

#if 0
	if(!uval)
	{
		k=-1;
	}
	
	if((uval<0) || (z<0))
	{
		*(int *)-1=-1;
	}
#endif

	if((uval<16) && (z<8))
	{
		*ctx->bs_ct++=(z<<4)+uval;
//		*ctx->bs_ct++=(z<<5)+uval;
		return;
	}

	if(uval<16)
	{
		i=z;

#if 0
		if((i<16) && (uval<3) && uval)
		{
			*ctx->bs_ct++=0x80+((i-0)<<1)+(uval-1);
			return;
		}
#endif

		if(i>=8)
//		if(i>=4)
		{
#if 0
			if((i<24) && (uval<3) && uval)
			{
				*ctx->bs_ct++=0x80+((i-8)<<1)+((uval-1)&1);
				return;
			}
#endif

#if 1
			if((i<16) && (uval<5) && uval)
			{
				*ctx->bs_ct++=0x80+((i-8)<<2)+((uval-1)&3);
				return;
			}
#endif

#if 0
			while(i>=32)
			{
				*ctx->bs_ct++=0xBF;
				i-=32;
			}
			if(i>=8)
			{
				*ctx->bs_ct++=0xA0+(i-1);
				i=0;
			}
#endif

#if 1
			while(i>=8)
			{
				j=i;
				if(j>32)j=32;
				*ctx->bs_ct++=0xA0+(j-1);
				i-=j;
			}
#endif
		}
		*ctx->bs_ct++=(i<<4)+uval;
//		*ctx->bs_ct++=(i<<5)+uval;
		return;
	}

#if 0
	if((uval<80) && ((z==0) || (z>4)))
	{
		i=z;
		while(i>0)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}

		*ctx->bs_ct++=0x80+(uval-16);
		return;
	}
#endif

#if 1
	if((uval<48) && ((z==0) || (z>4)))
	{
		i=z;
		while(i>0)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}

		*ctx->bs_ct++=0xC0+(uval-16);
		return;
	}
#endif

#if 1
	if(uval<1024)
	{
		i=z;
		while(i>=4)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
		*ctx->bs_ct++=0xE0+(i<<2)+((uval>>8)&3);
		*ctx->bs_ct++=uval;
		return;
	}

//	if(uval<262144)
	if(uval<65536)
	{
		i=z;
		while(i>=4)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
//		*ctx->bs_ct++=0xF0+((uval>>16)&3);
		*ctx->bs_ct++=0xF0+i;
//		*ctx->bs_ct++=uval>>8;
//		*ctx->bs_ct++=uval;
		*(u16 *)ctx->bs_ct=uval;
		ctx->bs_ct+=2;
		return;
	}

	if(uval<(1<<24))
	{
		i=z;
		while(i>=4)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
//		*ctx->bs_ct++=0xF4+((uval>>24)&3);
		*ctx->bs_ct++=0xF4+(i&3);
//		*ctx->bs_ct++=uval>>16;
//		*ctx->bs_ct++=uval>>8;
//		*ctx->bs_ct++=uval;
		*(u32 *)ctx->bs_ct=uval;
		ctx->bs_ct+=3;
		return;
	}
#endif

#if 0
	if(uval<2048)
	{
		i=z;
		while(i>=4)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
		*ctx->bs_ct++=0xC0+(i<<3)+((uval>>8)&7);
		*ctx->bs_ct++=uval;
		return;
	}
	if(uval<1048576)
	{
		i=z;
		while(i>=1)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
		*ctx->bs_ct++=0xE0+((uval>>16)&15);
		*ctx->bs_ct++=(uval>>8);
		*ctx->bs_ct++=uval;
		return;
	}

	if(uval<(1<<27))
	{
		i=z;
		while(i>=1)
		{
			j=i;
			if(j>32)j=32;
			*ctx->bs_ct++=0xA0+(j-1);
			i-=j;
		}
		*ctx->bs_ct++=0xF0+((uval>>24)&7);
		*ctx->bs_ct++=(uval>>16);
		*ctx->bs_ct++=(uval>>8);
		*ctx->bs_ct++=uval;
		return;
	}
#endif

	*(int *)-1=-1;
}

int BTIC2D_DecodeBlockByte8Y(BTIC1H_Context *ctx, int *blk);

void BTIC2D_EncodeBlockByte8Y(BTIC1H_Context *ctx, int *blk)
{
//	int ttab[64];
//	byte *ct0;

	int i, j, k, l;

//	*ctx->bs_ct++=0xAF;

	if(!blk[0])
	{
//		for(j=1; j<64; j++)
//			if(blk[j])
//				break;

		for(j=0; (j+3)<64; j+=4)
//		for(j=0; (j+7)<64; j+=8)
		{
			k=	blk[j+0]|blk[j+1]|
				blk[j+2]|blk[j+3];
			if(k)break;
//			k=	blk[j+4]|blk[j+5]|
//				blk[j+6]|blk[j+7];
//			if(k)break;
		}

		if(j>=64)
		{
			*ctx->bs_ct++=0x10;
//			*ctx->bs_ct++=0xA0;
//			*ctx->bs_ct++=0;
//			*ctx->bs_ct++=0;
			return;
		}
	}

//	ct0=ctx->bs_ct;
	BTIC2D_EncodeByteVal(ctx, 0, blk[0]);
	for(i=1; i<64; i++)
	{
		if(!blk[i])
		{
#if 1
//			for(j=i+1; j<61; j+=4)
			for(j=i+1; (j+3)<64; j+=4)
			{
				k=	blk[j+0]|blk[j+1]|
					blk[j+2]|blk[j+3];
				if(k)break;
			}
			for(; j<64; j++)
				if(blk[j])
					break;
#endif

#if 0
			for(j=i+1; j<64; j++)
				if(blk[j])
					break;
#endif
			if(j>=64)
			{
				*ctx->bs_ct++=0x00;
				break;
			}
			j=j-i;
			BTIC2D_EncodeByteVal(ctx, j, blk[i+j]);
			i+=j;
			continue;
		}
		BTIC2D_EncodeByteVal(ctx, 0, blk[i]);
	}

#if 0
	ctx->bs_cs=ct0;
	BTIC2D_DecodeBlockByte8Y(ctx, ttab);
	for(i=0; i<64; i++)
		if(blk[i]!=ttab[i])
			*(int *)-1=-1;
#endif
}

void BTIC2D_EncodeBlockByte8(BTIC1H_Context *ctx, int *blk)
{
	int dy, du, dv;
	int i, j, k, l;

	dy=blk[  0]-ctx->cy;	ctx->cy+=dy;	blk[  0]=dy;
	dy=blk[ 64]-ctx->cy;	ctx->cy+=dy;	blk[ 64]=dy;
	dy=blk[128]-ctx->cy;	ctx->cy+=dy;	blk[128]=dy;
	dy=blk[192]-ctx->cy;	ctx->cy+=dy;	blk[192]=dy;

	du=blk[256]-ctx->cu;	ctx->cu+=du;	blk[256]=du;
	dv=blk[320]-ctx->cv;	ctx->cv+=dv;	blk[320]=dv;

	BTIC2D_EncodeBlockByte8Y(ctx, blk+  0);
	BTIC2D_EncodeBlockByte8Y(ctx, blk+ 64);
	BTIC2D_EncodeBlockByte8Y(ctx, blk+128);
	BTIC2D_EncodeBlockByte8Y(ctx, blk+192);

	BTIC2D_EncodeBlockByte8Y(ctx, blk+256);
	BTIC2D_EncodeBlockByte8Y(ctx, blk+320);

//	BTIC2D_EncodeBlockByte4Uv(ctx, blk+64);
//	BTIC2D_EncodeBlockByte4Uv(ctx, blk+80);
}

static const int pdjpg_ijg_qtab_y[64] = {
16, 11, 10, 16,  24,  40,  51,  61,
12, 12, 14, 19,  26,  58,  60,  55,
14, 13, 16, 24,  40,  57,  69,  56,
14, 17, 22, 29,  51,  87,  80,  62,
18, 22, 37, 56,  68, 109, 103,  77,
24, 35, 55, 64,  81, 104, 113,  92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103,  99};

static const int pdjpg_ijg_qtab_uv[64] = {
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
47, 66, 99, 99, 99, 99, 99, 99,
24, 26, 56, 99, 99, 99, 99, 99,
18, 21, 26, 66, 99, 99, 99, 99,
17, 18, 24, 47, 99, 99, 99, 99};

void BTIC2D_MakeQuantTabFastIJG_Y(int *tab, float qf)
{
	double s, q;
	int i, j;

	q=(qf*100);
	s=(q<50)?5000/q:(200-2*q);
	for(i=0; i<64; i++)
	{
		j=(int)((s * pdjpg_ijg_qtab_y[i]+50)/100);
		j=(j<1)?1:((j<256)?j:255);
		tab[i]=j;
	}
}

void BTIC2D_MakeQuantTabFastIJG_UV(int *tab, float qf)
{
	double s, q;
	int i, j;

	q=(qf*100);
	s=(q<50)?5000/q:(200-2*q);
	for(i=0; i<64; i++)
	{
		j=(int)((s*pdjpg_ijg_qtab_uv[i]+50)/100);
		j=(j<1)?1:((j<256)?j:255);
		tab[i]=j;
	}
}

void BTIC2D_MakeQuantY(BTIC1H_Context *ctx, int *tab, int qf)
{
	int ctab[64];
	int i, j, k;

	BTIC2D_MakeQuantTabFastIJG_Y(ctab, (qf&127)/100.0);
	
	for(i=0; i<64; i++)
		{ tab[i]=ctab[pdjpg_zigzag8_2[i]]; }
}

void BTIC2D_MakeQuantUV(BTIC1H_Context *ctx, int *tab, int qf)
{
	int ctab[64];
	int i, j, k;

	BTIC2D_MakeQuantTabFastIJG_UV(ctab, (qf&127)/100.0);
	for(i=0; i<64; i++)
		{ tab[i]=ctab[pdjpg_zigzag8_2[i]]; }
}

void BTIC2D_EncodeEmitUVLI(BTIC1H_Context *ctx, int val)
{
	if(val<0x80)
	{
		*ctx->bs_ct++=val;
	}else if(val<0x4000)
	{
		*ctx->bs_ct++=0x80|(val>>8);
		*ctx->bs_ct++=val;
	}else if(val<0x200000)
	{
		*ctx->bs_ct++=0xC0|(val>>16);
		*ctx->bs_ct++=val>>8;
		*ctx->bs_ct++=val;
	}else if(val<0x10000000)
	{
		*ctx->bs_ct++=0xE0|(val>>24);
		*ctx->bs_ct++=val>>16;
		*ctx->bs_ct++=val>>8;
		*ctx->bs_ct++=val;
	}else
	{
		*ctx->bs_ct++=0xF0;
		*ctx->bs_ct++=val>>24;
		*ctx->bs_ct++=val>>16;
		*ctx->bs_ct++=val>> 8;
		*ctx->bs_ct++=val    ;
	}
}

void BTIC2D_EncodeEmitSVLI(BTIC1H_Context *ctx, int val)
{
	BTIC2D_EncodeEmitUVLI(ctx, (val<<1)^(val>>31));
}

void BTIC2D_EncodeQuantTab(BTIC1H_Context *ctx, int *tab)
{
	int i, j, k, l;
	
	l=-1;
	for(i=0; i<64; i++)
	{
		if(tab[i]==l)
		{
			for(j=i; (j<64) && (tab[j]==l); j++);
			BTIC2D_EncodeEmitSVLI(ctx, -(j-i));
			i=j-1;
			continue;
		}
		BTIC2D_EncodeEmitSVLI(ctx, tab[i]);
		l=tab[i];
	}
}

void BTIC2D_EncodeImageCtxYUY2(BTIC1H_Context *ctx,
	byte *ibuf, int xs, int ys, int qf)
{
	int qt[128];
	int ifqt[128];
	int tblk[512];
	byte *ct0, *cs0, *cs1, *cse;
	int xs1, ys1, ystr;
	int i, j, k;
	
	for(i=0; i<128; i++)
	{
		ifqt[i]=4096/8;
	}
	
	BTIC2D_MakeQuantY(ctx, qt, qf);
	BTIC2D_MakeQuantUV(ctx, qt+64, qf);
//	BTIC2D_MakeQuantY(ctx, qt+64, qf);

//	for(i=0; i<128; i++)
//		qt[i]=1;

	for(i=0; i<128; i++)
		{ ifqt[i]=4096/qt[i]; }

	BTIC2D_EncodeEmitSVLI(ctx, -1);
	BTIC2D_EncodeEmitUVLI(ctx, xs);
	BTIC2D_EncodeEmitUVLI(ctx, ys);

	BTIC2D_EncodeEmitSVLI(ctx, -4);
	BTIC2D_EncodeQuantTab(ctx, qt);

	BTIC2D_EncodeEmitSVLI(ctx, -5);
	BTIC2D_EncodeQuantTab(ctx, qt+64);

	BTIC2D_EncodeEmitSVLI(ctx, 0);

	xs1=(xs+15)>>4; ys1=(ys+15)>>4;
	ystr=xs*2;
	cse=ibuf+ys*ystr;
	
	for(i=0; i<ys1; i++)
	{
		ct0=ctx->bs_ct;
		ctx->bs_ct+=4;
		ctx->cy=0;
		ctx->cu=0;
		ctx->cv=0;
		
		for(j=0; j<xs1; j++)
		{
			cs0=ibuf+(i*16*ystr)+(j*16*2);
//			cs1=cs0+(16*ystr)+(16*2);
//			if(cs1>cse)
//			{
//				BTIC2D_EncodeBlockByte8(ctx, tblk);
//				continue;
//			}
			
			BTIC2D_TransBlock2YUY2(cs0, ibuf, cse,
				ystr, tblk, ifqt);
			BTIC2D_EncodeBlockByte8(ctx, tblk);
		}
		k=(ctx->bs_ct-ct0)-4;
		ct0[0]=0xE0|(k>>24);
		ct0[1]=k>>16;
		ct0[2]=k>> 8;
		ct0[3]=k    ;
	}
}

byte *BTIC2D_EncodeBlocksBufferYUY2(byte *obuf, int obmsz,
	byte *ibuf, int xs, int ys, int qf)
{
	BTIC1H_Context tctx;
	BTIC1H_Context *ctx;
	int i;
	
	ctx=&tctx;
	memset(ctx, 0, sizeof(BTIC1H_Context));
	
	BTIC1H_SetupContextInitial(ctx);
		
	BTIC1H_Rice_SetupWrite(ctx, obuf, obmsz);
	
	BTIC2D_EncodeImageCtxYUY2(ctx, ibuf, xs, ys, qf);
	
	BTIC1H_Rice_FlushBits(ctx);
	return(ctx->bs_ct);
}

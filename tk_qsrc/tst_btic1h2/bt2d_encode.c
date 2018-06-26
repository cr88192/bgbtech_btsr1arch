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

static const int pdjpg_zigzag4_1[16]={
 0,  1,  5,  6,
 2,  4,  7, 12,
 3,  8, 11, 13,
 9, 10, 14, 15
};

static const int pdjpg_zigzag4_2[16]={
 0,  1,  4,  8,
 5,  2,  3,  6,
 9, 12, 13, 10,
 7, 11, 14, 15
};

void BTIC2D_TransBlockYUY2(byte *iblk, int ystr, s16 *oblk, int *ifqt)
{
	int yblka[16];
	int ublka[4];
	int vblka[4];

	byte *ics, *jcs;
	int *yct, *uct, *vct;
	int y0a, y1a, y2a, y3a;
	int y0b, y1b, y2b, y3b;
	int y0c, y1c, y2c, y3c;
	int i;
	
	for(i=0; i<4; i++)
	{
		ics=iblk+i*ystr;
		yct=yblka+i*4;
		y0a=ics[0];		y1a=ics[2];
		y2a=ics[4];		y3a=ics[6];
		y0b=y0a+y2a;		y1b=y1a+y3a;
		y2b=(y0b>>1)-y2a;	y3b=(y1b>>1)-y3a;
		y0c=y0b+y1b;		y1c=(y0c>>1)-y1b;
		y2c=y2b+y3b;		y3c=(y2c>>1)-y3b;
		yct[0]=y0c;			yct[1]=y1c;
		yct[2]=y2c;			yct[3]=y3c;
	}

	for(i=0; i<4; i++)
	{
		yct=yblka+i;
		y0a=yct[0];			y1a=yct[ 4];
		y2a=yct[8];			y3a=yct[12];
		y0b=y0a+y2a;		y1b=y1a+y3a;
		y2b=(y0b>>1)-y2a;	y3b=(y1b>>1)-y3a;
		y0c=y0b+y1b;		y1c=(y0c>>1)-y1b;
		y2c=y2b+y3b;		y3c=(y2c>>1)-y3b;
		yct[0]=y0c;			yct[ 4]=y1c;
		yct[8]=y2c;			yct[12]=y3c;
	}

	for(i=0; i<2; i++)
	{
		ics=iblk+(i*2+0)*ystr;		jcs=iblk+(i*2+1)*ystr;
		uct=ublka+i*2;				vct=vblka+i*2;

		y0a=(ics[1]+jcs[1])>>1;		y1a=(ics[5]+jcs[5])>>1;
		y2a=(ics[3]+jcs[3])>>1;		y3a=(ics[7]+jcs[7])>>1;
		y0b=y0a+y1a;				y1b=(y0b>>1)-y1a;
		y2b=y2a+y3a;				y3b=(y2b>>1)-y3a;
		uct[0]=y0b;					uct[1]=y1b;
		vct[0]=y2b;					vct[1]=y3b;
	}

	uct=ublka;		vct=vblka;

	y0a=uct[0];		y1a=uct[2];
	y2a=uct[1];		y3a=uct[3];
	y0b=y0a+y1a;	y1b=(y0b>>1)-y1a;
	y2b=y2a+y3a;	y3b=(y2b>>1)-y3a;
	uct[0]=y0b;		uct[2]=y1b;
	uct[1]=y2b;		uct[3]=y3b;

	y0a=vct[0];		y1a=vct[2];
	y2a=vct[1];		y3a=vct[3];
	y0b=y0a+y1a;	y1b=(y0b>>1)-y1a;
	y2b=y2a+y3a;	y3b=(y2b>>1)-y3a;
	vct[0]=y0b;		vct[2]=y1b;
	vct[1]=y2b;		vct[3]=y3b;

	oblk[ 0]=(yblka[ 0]*ifqt[ 0]+2048)>>12;
	oblk[ 1]=(yblka[ 1]*ifqt[ 1]+2048)>>12;
	oblk[ 2]=(yblka[ 4]*ifqt[ 2]+2048)>>12;
	oblk[ 3]=(yblka[ 8]*ifqt[ 3]+2048)>>12;
	oblk[ 4]=(yblka[ 5]*ifqt[ 4]+2048)>>12;
	oblk[ 5]=(yblka[ 2]*ifqt[ 5]+2048)>>12;
	oblk[ 6]=(yblka[ 3]*ifqt[ 6]+2048)>>12;
	oblk[ 7]=(yblka[ 6]*ifqt[ 7]+2048)>>12;
	oblk[ 8]=(yblka[ 9]*ifqt[ 8]+2048)>>12;
	oblk[ 9]=(yblka[12]*ifqt[ 9]+2048)>>12;
	oblk[10]=(yblka[13]*ifqt[10]+2048)>>12;
	oblk[11]=(yblka[10]*ifqt[11]+2048)>>12;
	oblk[12]=(yblka[ 7]*ifqt[12]+2048)>>12;
	oblk[13]=(yblka[11]*ifqt[13]+2048)>>12;
	oblk[14]=(yblka[14]*ifqt[14]+2048)>>12;
	oblk[15]=(yblka[15]*ifqt[15]+2048)>>12;
	oblk[16]=(ublka[ 0]*ifqt[16]+2048)>>12;
	oblk[17]=(ublka[ 1]*ifqt[17]+2048)>>12;
	oblk[18]=(ublka[ 2]*ifqt[18]+2048)>>12;
	oblk[19]=(ublka[ 3]*ifqt[19]+2048)>>12;
	oblk[20]=(vblka[ 0]*ifqt[20]+2048)>>12;
	oblk[21]=(vblka[ 1]*ifqt[21]+2048)>>12;
	oblk[22]=(vblka[ 2]*ifqt[22]+2048)>>12;
	oblk[23]=(vblka[ 3]*ifqt[23]+2048)>>12;
}

void BTIC2D_TransBlock2YUY2(byte *iblk, int ystr, s16 *oblk, int *ifqt)
{
	int yblka[64], ublka[16], vblka[16];
	byte *ics, *ics0, *ics1;
	int *yct, *uct, *vct;
	s16 *oct;
	int y0, y1, y2, y3, u0, u1, v0, v1;
	int i, j, k, l;

	for(i=0; i<8; i++)
	{
		ics=iblk+i*ystr;
		yct=yblka+i*8;
		yct[0]=ics[ 0]-128;
		yct[1]=ics[ 2]-128;
		yct[2]=ics[ 4]-128;
		yct[3]=ics[ 6]-128;
		yct[4]=ics[ 8]-128;
		yct[5]=ics[10]-128;
		yct[6]=ics[12]-128;
		yct[7]=ics[14]-128;
	}

	for(i=0; i<4; i++)
	{
		ics0=iblk+(i*2+0)*ystr;
		ics1=iblk+(i*2+1)*ystr;
		uct=ublka+i*4;
		vct=vblka+i*4;
		uct[0]=((ics0[ 1]+ics1[ 1])>>1)-128;
		vct[0]=((ics0[ 3]+ics1[ 3])>>1)-128;
		uct[1]=((ics0[ 5]+ics1[ 5])>>1)-128;
		vct[1]=((ics0[ 7]+ics1[ 7])>>1)-128;
		uct[2]=((ics0[ 9]+ics1[ 9])>>1)-128;
		vct[2]=((ics0[11]+ics1[11])>>1)-128;
		uct[3]=((ics0[13]+ics1[13])>>1)-128;
		vct[3]=((ics0[15]+ics1[15])>>1)-128;
	}
	
//	BTIC2D_TransDCT(yblka, yblka);
//	BTIC2D_TransDCT4(ublka, ublka);
//	BTIC2D_TransDCT4(vblka, vblka);

//	BTIC2D_TransWHT8(yblka, yblka);
//	BTIC2D_TransWHT4(ublka, ublka);
//	BTIC2D_TransWHT4(vblka, vblka);

	BTIC2D_TransBHT8(yblka, yblka);
	BTIC2D_TransBHT4(ublka, ublka);
	BTIC2D_TransBHT4(vblka, vblka);
	
//	for(i=0; i<64; i++)
//	{
//		j=pdjpg_zigzag8_2[i];
//		oblk[i]=(yblka[j]*ifqt[i]+2048)>>12;
//	}

#if 0
	yct=yblka;
	for(i=0; i<8; i++)
	{
		k=i*8;
		oct=oblk+k;
		uct=ifqt+k;
		vct=pdjpg_zigzag8_2+k;
		oct[0]=(yct[vct[0]]*uct[0]+2048)>>12;
		oct[1]=(yct[vct[1]]*uct[1]+2048)>>12;
		oct[2]=(yct[vct[2]]*uct[2]+2048)>>12;
		oct[3]=(yct[vct[3]]*uct[3]+2048)>>12;
		oct[4]=(yct[vct[4]]*uct[4]+2048)>>12;
		oct[5]=(yct[vct[5]]*uct[5]+2048)>>12;
		oct[6]=(yct[vct[6]]*uct[6]+2048)>>12;
		oct[7]=(yct[vct[7]]*uct[7]+2048)>>12;
	}
#endif

#define YBVQ(x) \
		oblk[x+0]=(yblka[pdjpg_zigzag8_2[x+0]]*ifqt[x+0]+2048)>>12;	\
		oblk[x+1]=(yblka[pdjpg_zigzag8_2[x+1]]*ifqt[x+1]+2048)>>12;	\
		oblk[x+2]=(yblka[pdjpg_zigzag8_2[x+2]]*ifqt[x+2]+2048)>>12;	\
		oblk[x+3]=(yblka[pdjpg_zigzag8_2[x+3]]*ifqt[x+3]+2048)>>12;	\
		oblk[x+4]=(yblka[pdjpg_zigzag8_2[x+4]]*ifqt[x+4]+2048)>>12;	\
		oblk[x+5]=(yblka[pdjpg_zigzag8_2[x+5]]*ifqt[x+5]+2048)>>12;	\
		oblk[x+6]=(yblka[pdjpg_zigzag8_2[x+6]]*ifqt[x+6]+2048)>>12;	\
		oblk[x+7]=(yblka[pdjpg_zigzag8_2[x+7]]*ifqt[x+7]+2048)>>12;

	YBVQ(0)		YBVQ(8)
	YBVQ(16)	YBVQ(24)
	YBVQ(32)	YBVQ(40)
	YBVQ(48)	YBVQ(56)

#if 0
	for(i=0; i<16; i++)
	{
		j=pdjpg_zigzag4_2[i];
		oblk[64+i]=(ublka[j]*ifqt[64+i]+2048)>>12;
		oblk[80+i]=(vblka[j]*ifqt[80+i]+2048)>>12;
	}
#endif

#define UBVQ(i) \
		oblk[64+i+0]=(ublka[pdjpg_zigzag4_2[i+0]]*ifqt[64+i+0]+2048)>>12; \
		oblk[80+i+0]=(vblka[pdjpg_zigzag4_2[i+0]]*ifqt[80+i+0]+2048)>>12; \
		oblk[64+i+1]=(ublka[pdjpg_zigzag4_2[i+1]]*ifqt[64+i+1]+2048)>>12; \
		oblk[80+i+1]=(vblka[pdjpg_zigzag4_2[i+1]]*ifqt[80+i+1]+2048)>>12; \
		oblk[64+i+2]=(ublka[pdjpg_zigzag4_2[i+2]]*ifqt[64+i+2]+2048)>>12; \
		oblk[80+i+2]=(vblka[pdjpg_zigzag4_2[i+2]]*ifqt[80+i+2]+2048)>>12; \
		oblk[64+i+3]=(ublka[pdjpg_zigzag4_2[i+3]]*ifqt[64+i+3]+2048)>>12; \
		oblk[80+i+3]=(vblka[pdjpg_zigzag4_2[i+3]]*ifqt[80+i+3]+2048)>>12;
	UBVQ(0)		UBVQ(4)
	UBVQ(8)		UBVQ(12)
}

const u32 btic2c_dbase[64]={
		   0,    1,    2,     3,     4,     6,     8,    12,
		  16,   24,   32,    48,    64,    96,   128,   192,
		 256,  384,  512,   768,  1024,  1536,  2048,  3072,
		4096, 6144, 8192, 12288, 16384, 24576, 32768, 49152,
		//(32-bit range)
		0x00010000, 0x00018000, 0x00020000, 0x00030000,
		0x00040000, 0x00060000, 0x00080000, 0x000C0000,
		0x00100000, 0x00180000, 0x00200000, 0x00300000,
		0x00400000, 0x00600000, 0x00800000, 0x00C00000,
		0x01000000, 0x01800000, 0x02000000, 0x03000000,
		0x04000000, 0x06000000, 0x08000000, 0x0C000000,
		0x10000000, 0x18000000, 0x20000000, 0x30000000,
		0x40000000, 0x60000000, 0x80000000, 0xC0000000,
		};

const int btic2c_dextra[128]={
		//(16-bit range)
		 0,  0,  0,  0,  1,  1,  2,  2,
		 3,  3,  4,  4,  5,  5,  6,  6,
		 7,  7,  8,  8,  9,  9, 10, 10,
		11, 11, 12, 12, 13, 13, 14, 14,
		//(32-bit range)
		15, 15, 16, 16, 17, 17, 18, 18,
		19, 19, 20, 20, 21, 21, 22, 22,
		23, 23, 24, 24, 25, 25, 26, 26,
		27, 27, 28, 28, 29, 29, 30, 30,
		//(64-bit range)
		31, 31, 32, 32, 33, 33, 34, 34,
		35, 35, 36, 36, 37, 37, 38, 38,
		39, 39, 40, 40, 41, 41, 42, 42,
		43, 43, 44, 44, 45, 45, 46, 46,
		47, 47, 48, 48, 49, 49, 50, 50,
		51, 51, 52, 52, 53, 53, 54, 54,
		55, 55, 56, 56, 57, 57, 58, 58,
		59, 59, 60, 60, 61, 61, 62, 62
		};

void BTIC2D_EncodeCoeffUValue(BTIC1H_Context *ctx, u32 val, int *rk)
{
	int i, j, k;

//	BTIC1H_Rice_WriteAdSRice(ctx, val+1, rk);
	BTIC1H_Rice_WriteAdSRice(ctx, val, rk);

#if 0
	j=val;
	for(i=0; i<64; i++)
	{
		k=btic2c_dbase[i]+(1<<btic2c_dextra[i]);
//		if((j>=btic2c_dbase[i]) && (j<k))
		if(j<k)
		{
//			BTIC2CH_EncodeSymbol(ctx, tab, i|(z<<5));
			BTIC1H_Rice_WriteAdSRice(ctx, i+1, rk);
			BTIC1H_Rice_WriteNBits(ctx, j-btic2c_dbase[i], btic2c_dextra[i]);
			break;
		}
	}
#endif
}

void BTIC2D_EncodeVal(BTIC1H_Context *ctx, int v, int *rk)
{
	int i, j, k;

	j=(v<0)?(((-v)<<1)-1):(v<<1);
	BTIC2D_EncodeCoeffUValue(ctx, j, rk);
}

void BTIC2D_EncodeBlock8Vlc(BTIC1H_Context *ctx, s16 *blk,
	int *rkd, int *rka)
{
	int i, j, k, l;

	if(!blk[0])
	{
		for(j=1; j<64; j++)
			if(blk[j])
				break;
		if(j>=64)
		{
//			BTIC2D_EncodeVal(ctx, 0, &(ctx->rk_dy));
			BTIC1H_Rice_WriteAdSRice(ctx, -1, rkd);
			return;
		}
	}

	BTIC2D_EncodeVal(ctx, blk[0], rkd);
	for(i=1; i<16; i++)
	{
		if(!blk[i])
		{
			for(j=i+1; j<64; j++)
				if(blk[j])
					break;
			if(j==64)
			{
				BTIC1H_Rice_WriteAdSRice(ctx, 0, rka);
				break;
			}
			j=j-i;
			BTIC1H_Rice_WriteAdSRice(ctx, -j, rka);
			i+=j-1;
			continue;
		}
		BTIC2D_EncodeVal(ctx, blk[i], rka);
	}
}

void BTIC2D_EncodeBlockVlc8Y(BTIC1H_Context *ctx, s16 *blk)
{
	BTIC2D_EncodeBlock8Vlc(ctx, blk, &(ctx->rk_dy), &(ctx->rk_ay));
}

void BTIC2D_EncodeBlock4Vlc(BTIC1H_Context *ctx, s16 *blk,
	int *rkd, int *rka)
{
	int i, j, k, l;

	if(!blk[0])
	{
		for(j=1; j<16; j++)
			if(blk[j])
				break;
		if(j>=16)
		{
//			BTIC2D_EncodeVal(ctx, 0, &(ctx->rk_dy));
			BTIC1H_Rice_WriteAdSRice(ctx, -1, rkd);
			return;
		}
	}

	BTIC2D_EncodeVal(ctx, blk[0], rkd);
	for(i=1; i<16; i++)
	{
		if(!blk[i])
//		if(!blk[i] && !blk[i+1])
		{
			for(j=i+1; j<16; j++)
				if(blk[j])
					break;
			if(j==16)
			{
				BTIC1H_Rice_WriteAdSRice(ctx, 0, rka);
				break;
			}
			j=j-i;
			BTIC1H_Rice_WriteAdSRice(ctx, -j, rka);
			i+=j-1;
			continue;
		}
		BTIC2D_EncodeVal(ctx, blk[i], rka);
	}
}

void BTIC2D_EncodeBlockVlc4Y(BTIC1H_Context *ctx, s16 *blk)
{
	BTIC2D_EncodeBlock4Vlc(ctx, blk, &(ctx->rk_dy), &(ctx->rk_ay));
}

void BTIC2D_EncodeBlockVlc4Uv(BTIC1H_Context *ctx, s16 *blk)
{
	BTIC2D_EncodeBlock4Vlc(ctx, blk, &(ctx->rk_duv), &(ctx->rk_auv));
}

void BTIC2D_EncodeBlockVlc2Uv(BTIC1H_Context *ctx, s16 *blk)
{
	int i, j, k, l;

	if(!(blk[0]|blk[1]|blk[2]|blk[3]))
	{
//		BTIC2D_EncodeVal(ctx, 0, &(ctx->rk_duv));
		BTIC1H_Rice_WriteAdSRice(ctx, -1, &(ctx->rk_duv));
		return;
	}

	BTIC2D_EncodeVal(ctx, blk[0], &(ctx->rk_duv));
	for(i=1; i<4; i++)
	{
		if(!blk[i])
		{
			for(j=i+1; j<4; j++)
				if(blk[j])
					break;
			if(j==4)
			{
				BTIC1H_Rice_WriteAdSRice(ctx, 0, &(ctx->rk_auv));
				break;
			}
			j=j-i;
//			if(j>0)
//			{
				BTIC1H_Rice_WriteAdSRice(ctx, -j, &(ctx->rk_auv));
				i+=j-1;
				continue;
//			}
		}
		BTIC2D_EncodeVal(ctx, blk[i], &(ctx->rk_auv));
	}
}

void BTIC2D_EncodeBlockVlc4(BTIC1H_Context *ctx, s16 *blk)
{
	int dy, du, dv;
	int i, j, k, l;

	dy=blk[0]-ctx->cy;
	du=blk[16]-ctx->cu;
	dv=blk[20]-ctx->cv;

	ctx->cy+=dy;
	ctx->cu+=du;
	ctx->cv+=dv;

	blk[0]=dy;
	blk[16]=du;
	blk[20]=dv;

	BTIC2D_EncodeBlockVlc4Y(ctx, blk);
	BTIC2D_EncodeBlockVlc2Uv(ctx, blk+16);
	BTIC2D_EncodeBlockVlc2Uv(ctx, blk+20);
}

void BTIC2D_EncodeBlockVlc8(BTIC1H_Context *ctx, s16 *blk)
{
	int dy, du, dv;
	int i, j, k, l;

	dy=blk[ 0]-ctx->cy;
	du=blk[64]-ctx->cu;
	dv=blk[80]-ctx->cv;

	ctx->cy+=dy;
	ctx->cu+=du;
	ctx->cv+=dv;

	blk[ 0]=dy;
	blk[64]=du;
	blk[80]=dv;

	BTIC2D_EncodeBlockVlc8Y(ctx, blk);
	BTIC2D_EncodeBlockVlc4Uv(ctx, blk+64);
	BTIC2D_EncodeBlockVlc4Uv(ctx, blk+80);
}

void BTIC2D_EncodeByteVal(BTIC1H_Context *ctx, int z, int val)
{
	int uval;
	int i, j, k;
	
	uval=(val<<1)^(val>>31);
	if(uval<16)
	{
		i=z;
		if(i>=8)
		{
			if((i<24) && (uval<3) && uval)
			{
				*ctx->bs_ct++=0x80+((i-8)<<1)+(uval-1);
				return;
			}

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
		return;
	}

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
		*ctx->bs_ct++=0xC0+(i<<3)+(uval>>8);
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
		*ctx->bs_ct++=0xE0+(uval>>16);
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
		*ctx->bs_ct++=0xF0+(uval>>24);
		*ctx->bs_ct++=(uval>>16);
		*ctx->bs_ct++=(uval>>8);
		*ctx->bs_ct++=uval;
		return;
	}
}

void BTIC2D_EncodeBlockByte4Uv(BTIC1H_Context *ctx, s16 *blk)
{
	int i, j, k, l;

	if(!blk[0])
	{
		for(j=1; j<16; j++)
			if(blk[j])
				break;
		if(j>=16)
		{
			*ctx->bs_ct++=0xA0;
//			*ctx->bs_ct++=0;
			return;
		}
	}

	BTIC2D_EncodeByteVal(ctx, 0, blk[0]);
	for(i=1; i<16; i++)
	{
		if(!blk[i])
		{
			for(j=i+1; j<16; j++)
				if(blk[j])
					break;
			if(j==16)
			{
				*ctx->bs_ct++=0;
				break;
			}
			j=j-i;
			BTIC2D_EncodeByteVal(ctx, j, blk[i]);
			i+=j;
			continue;
		}
		BTIC2D_EncodeByteVal(ctx, 0, blk[i]);
	}
}

void BTIC2D_EncodeBlockByte8Y(BTIC1H_Context *ctx, s16 *blk)
{
	int i, j, k, l;

	if(!blk[0])
	{
		for(j=1; j<64; j++)
			if(blk[j])
				break;
		if(j>=64)
		{
			*ctx->bs_ct++=0;
			*ctx->bs_ct++=0;
			return;
		}
	}

	BTIC2D_EncodeByteVal(ctx, 0, blk[0]);
	for(i=1; i<16; i++)
	{
		if(!blk[i])
		{
			for(j=i+1; j<64; j++)
				if(blk[j])
					break;
			if(j==64)
			{
				*ctx->bs_ct++=0;
				break;
			}
			j=j-i;
			BTIC2D_EncodeByteVal(ctx, j, blk[i]);
			i+=j;
			continue;
		}
		BTIC2D_EncodeByteVal(ctx, 0, blk[i]);
	}
}

void BTIC2D_EncodeBlockByte8(BTIC1H_Context *ctx, s16 *blk)
{
	int dy, du, dv;
	int i, j, k, l;

	dy=blk[ 0]-ctx->cy;
	du=blk[64]-ctx->cu;
	dv=blk[80]-ctx->cv;

	ctx->cy+=dy;
	ctx->cu+=du;
	ctx->cv+=dv;

	blk[ 0]=dy;
	blk[64]=du;
	blk[80]=dv;

	BTIC2D_EncodeBlockByte8Y(ctx, blk);
	BTIC2D_EncodeBlockByte4Uv(ctx, blk+64);
	BTIC2D_EncodeBlockByte4Uv(ctx, blk+80);
}


void BTIC2D_EncodeImageCtxYUY2(BTIC1H_Context *ctx,
	byte *ibuf, int xs, int ys)
{
	int ifqt[128];
	s16 tblk[128];
	int xs1, ys1, ystr;
	int i, j, k;
	
	for(i=0; i<96; i++)
	{
		ifqt[i]=4096/8;
	}

#if 0
	xs1=xs>>2; ys1=ys>>2;
	ystr=xs*2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC2D_TransBlockYUY2(ibuf+(i*4*ystr)+(j*4*2), ystr, tblk, ifqt);
		BTIC2D_EncodeBlockVlc4(ctx, tblk);
	}
#endif

#if 1
	xs1=xs>>3; ys1=ys>>3;
	ystr=xs*2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC2D_TransBlock2YUY2(ibuf+(i*8*ystr)+(j*8*2), ystr, tblk, ifqt);
//		BTIC2D_EncodeBlockVlc8(ctx, tblk);
		BTIC2D_EncodeBlockByte8(ctx, tblk);
	}
#endif
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
	
	BTIC2D_EncodeImageCtxYUY2(ctx, ibuf, xs, ys);
	
	BTIC1H_Rice_FlushBits(ctx);
	return(ctx->bs_ct);
}

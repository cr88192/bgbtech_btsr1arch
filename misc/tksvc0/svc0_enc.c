#define TKULZ_STATUS_NONE	0
#define TKULZ_STATUS_END	1
#define TKULZ_STATUS_BADTAG	2
#define TKULZ_STATUS_BADESC	3
#define TKULZ_STATUS_BADHT	4

#ifndef TKULZ_BYTE
#define TKULZ_BYTE
typedef unsigned char		byte;
typedef unsigned short	u16;
typedef unsigned int		u32;

typedef signed char		sbyte;
typedef signed short		s16;
typedef signed int		s32;
#endif

// #define TKULZ_LEN13

// #define TKULZ_HTABSZ 8192
#define TKULZ_HTABNB 13

#ifndef TKSVC_DBGBREAK
#ifdef _MSC_VER
#define TKSVC_DBGBREAK	__debugbreak();
#else
#define TKSVC_DBGBREAK
#endif
#endif

typedef struct TKSVC0E_EncState_s TKSVC0E_EncState;

struct TKSVC0E_EncState_s {
	byte *ct;
	u32 win;
	sbyte pos;
	byte status;

	byte is_pf;					/* Is P-Frame */
	byte is_444;
	byte quality;
	int xs, ys;

	/* Huffman Tables */
	u32 hetab[8][256];			/* Huffman Encode Table */
	byte hltab[8][256];			/* Huffman Length Table */
	int hstab[8][256];			/* Huffman Stats Table */

	int qtab[4][64];			/* Quantization Tables */
	byte use_dct[4];
	
	/* DC Planes */
	int dcxs[4];
	int dcys[4];
	int *dcp[4];				/* DC Planes */
	int *mvxp[4];				/* Motion Vector Planes */
	int *mvyp[4];				/* Motion Vecroe Planes */
	
	int *blkp[4];				/* DCT/WHT+AC blocks per plane */
	
	byte *yuvp[4];				/* YUV Planes */
	byte *lyuvp[4];				/* Last Frame YUV Planes */

};

static const char tksvc0e_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

/* ZigZag->Linear */
static const char tksvc0e_zigzag2[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};


void TKSVC0E_WriteBits(TKSVC0E_EncState *ctx, int val, int bits)
{
	if(!ctx->ct)
		return;

	if((bits>=24) || (bits<0))
	{
		TKSVC_DBGBREAK
	}

	ctx->win|=(val&((1<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void TKSVC0E_FlushWriteBits(TKSVC0E_EncState *ctx)
{
	if(!ctx->ct)
		return;

	while(ctx->pos>0)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
	ctx->pos=0;
}

byte TKSVC0E_TransposeByte(byte v)
{
	static const byte trans4[16]={
		0x0,0x8,0x4,0xC, 0x2,0xA,0x6,0xE,
		0x1,0x9,0x5,0xD, 0x3,0xB,0x7,0xF};
	return((trans4[v&15]<<4)|trans4[(v>>4)&15]);
}

u16 TKSVC0E_TransposeWord(u16 v)
	{ return((TKSVC0E_TransposeByte(v&255)<<8)|TKSVC0E_TransposeByte(v>>8)); }


void TKSVC0E_SetupEncTableLengths(TKSVC0E_EncState *ctx, u32 *htab, byte *cls)
{
	int c, l, tc;
	int i, j, k;

	for(i=0; i<256; i++)
		htab[i]=0;
	
	c=0;
	for(l=1; l<14; l++)
	{
		for(i=0; i<256; i++)
			if(cls[i]==l)
		{
			tc=c<<(13-l);
			k=TKSVC0E_TransposeWord(tc)>>3;
			htab[i]=(l<<16)|k;
			c++;
		}
		c=c<<1;
	}
	
//	if(c>8192)
//		TKSVC_DBGBREAK
}

void TKSVC0E_WritePackedLengths(TKSVC0E_EncState *ctx, byte *cls)
{
	byte *s, *s1, *se;
	int l, ll, n;
	
	s=cls; se=cls+256; ll=-1;
	while(s<se)
	{
		l=*s++;
		if(!l)
		{
			s1=s;
			while((s1<se) && !(*s1))s1++;
			n=(s1-s)+1;
			
			if((s1>=se) && (n>18))
			{
				TKSVC0E_WriteBits(ctx, 0xF, 4);
//				TKSVC0E_WriteBits(ctx, 3, 2);
				TKSVC0E_WriteBits(ctx, 1, 2);
				TKSVC0E_WriteBits(ctx, 0, 6);
				break;
			}
			
			if((n>=3) && (n<=18))
			{
				TKSVC0E_WriteBits(ctx, 0xE, 4);
				TKSVC0E_WriteBits(ctx, n-3, 4);
				s=s1;
				continue;
			}
			if((n>=19) && (n<=82))
			{
				TKSVC0E_WriteBits(ctx, 0xF, 4);
				TKSVC0E_WriteBits(ctx, 0, 2);
				TKSVC0E_WriteBits(ctx, n-19, 6);
				s=s1;
				continue;
			}

			if(n>82)
			{
				TKSVC0E_WriteBits(ctx, 0xF, 4);
				TKSVC0E_WriteBits(ctx, 0, 2);
				TKSVC0E_WriteBits(ctx, 63, 6);
				s+=81;
				continue;
			}

			ll=0;
			TKSVC0E_WriteBits(ctx, 0x0, 4);
			continue;
		}

#if 1
		if(l==ll)
		{
			s1=s;
			while((s1<se) && (*s1==ll))s1++;
			n=(s1-s)+1;

			if((n>=4) && (n<=66))
			{
				TKSVC0E_WriteBits(ctx, 0xF, 4);
				TKSVC0E_WriteBits(ctx, 1, 2);
				TKSVC0E_WriteBits(ctx, n-3, 6);
				s=s1;
				continue;
			}

#if 1
			if(n>66)
			{
				TKSVC0E_WriteBits(ctx, 0xF, 4);
				TKSVC0E_WriteBits(ctx, 1, 2);
				TKSVC0E_WriteBits(ctx, 63, 6);
				s+=65;
				continue;
			}
#endif
		}
#endif

		if((l>=1) && (l<=13))
		{
			TKSVC0E_WriteBits(ctx, l, 4);
			ll=l;
			continue;
		}
		
		TKSVC_DBGBREAK
	}
}

void TKSVC0E_EncodeHuffSym(TKSVC0E_EncState *ctx, u32 *hetab, int sym)
{
	int b;
	b=hetab[sym];
	
	if(!((b>>16)&15))
	{
		TKSVC_DBGBREAK
	}
	
	TKSVC0E_WriteBits(ctx, b, b>>16);
}

void TKSVC0E_EncodeSymbol(TKSVC0E_EncState *ctx, int htab, int sym)
{
	if(ctx->ct)
	{
		TKSVC0E_EncodeHuffSym(ctx, ctx->hetab[htab], sym);
	}
	ctx->hstab[htab][sym]++;
}

void TKSVC0E_EncodeExtraBits(TKSVC0E_EncState *ctx, int v, int n)
{
	TKSVC0E_WriteBits(ctx, v, n);
}

#if 1
void TKSVC0E_EncodeCoeff(TKSVC0E_EncState *ctx, int htab, int z, int val)
{
	int uval, zn1;
	int eb0, v0, ebv0;
	int eb1, v1, ebv1;
	int ebz0, vz0, ebvz0;
	int ebz1, vz1, ebvz1;

	uval=(val<<1)^(val>>31);
	zn1=z-1;

	eb0=0; v0=uval;
	while(v0>=8)
		{ eb0++; v0=v0>>1; }
	ebv0=((eb0+1)<<2)|(v0&3);

#if 0
	eb1=eb0;
	v1=v0;
	ebv1=((eb1+1)<<2)|(v1&3);

	ebz0=0; vz0=z;
	while(vz0>=8)
		{ ebz0++; vz0=vz0>>1; }
	ebvz0=((ebz0+1)<<2)|(vz0&3);

	ebz1=0; vz1=zn1;
	while(vz1>=8)
		{ ebz1++; vz1=vz1>>1; }
	ebvz1=((ebz1+1)<<2)|(vz1&3);
#endif

#if 1
	eb1=0; v1=uval;
	while(v1>=4)
		{ eb1++; v1=v1>>1; }
	ebv1=((eb1+1)<<1)|(v1&1);

	ebz0=0; vz0=z;
	while(vz0>=4)
		{ ebz0++; vz0=vz0>>1; }
	ebvz0=((ebz0+1)<<1)|(vz0&1);

	ebz1=0; vz1=zn1;
	while(vz1>=4)
		{ ebz1++; vz1=vz1>>1; }
	ebvz1=((ebz1+1)<<1)|(vz1&1);
#endif

	if(z)
	{
		if(z<0)
		{
			TKSVC_DBGBREAK
		}
	
		if((z<8) && uval)
		{
			if(uval<4)
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0x80|(z<<4)|uval);
				return;
			}
			if(ebv1<16)
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0x80|(z<<4)|ebv1);
				TKSVC0E_EncodeExtraBits(ctx, uval, eb1);
				return;
			}
		}

//		if(z<32)
		if(ebvz0<16)
		{
			if(val)
			{
				if(zn1<4)
				{
					TKSVC0E_EncodeSymbol(ctx, htab, 0x80|zn1);
				}else
				{
					TKSVC0E_EncodeSymbol(ctx, htab, 0x80|ebvz1);
					TKSVC0E_EncodeExtraBits(ctx, zn1, ebz1);
				}
			
				TKSVC0E_EncodeCoeff(ctx, htab, 0, val);
			}else
			{
				if(z<4)
				{
					TKSVC0E_EncodeSymbol(ctx, htab, 0x80|z);
				}else
				{
					TKSVC0E_EncodeSymbol(ctx, htab, 0x80|ebvz0);
					TKSVC0E_EncodeExtraBits(ctx, z, ebz0);
				}
			}
			return;
		}
		
		if(z>256)
		{
			TKSVC0E_EncodeCoeff(ctx, htab, 255, 0);
			TKSVC0E_EncodeCoeff(ctx, htab, z-256, val);
			return;
		}

		TKSVC_DBGBREAK
//		TKSVC0E_EncodeSymbol(ctx, htab, 0x8F);
//		TKSVC0E_EncodeExtraBits(ctx, 3, 2);
//		if((z>32) || val)
//			TKSVC0E_EncodeCoeff(ctx, htab, z-32, val);
		return;
	}else
	{
		if(uval<4)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, uval);
			return;
		}
		if(ebv0<0x7F)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, ebv0);
			TKSVC0E_EncodeExtraBits(ctx, uval, eb0);
			return;
		}
	}

	TKSVC_DBGBREAK
}
#endif


#if 0
void TKSVC0E_EncodeCoeff(TKSVC0E_EncState *ctx, int htab, int z, int val)
{
	int uval, zn1;
	int eb0, v0, ebv0;
	int eb1, v1, ebv1;
	int ebz0, vz0, ebvz0;
	int ebz1, vz1, ebvz1;

	uval=(val<<1)^(val>>31);
	zn1=z-1;

	eb0=0; v0=uval;
	while(v0>=4)
		{ eb0++; v0=v0>>1; }
	ebv0=((eb0+1)<<1)|(v0&1);

#if 1
	ebz0=0; vz0=z;
	while(vz0>=4)
		{ ebz0++; vz0=vz0>>1; }
	ebvz0=((ebz0+1)<<1)|(vz0&1);

	ebz1=0; vz1=zn1;
	while(vz1>=4)
		{ ebz1++; vz1=vz1>>1; }
	ebvz1=((ebz1+1)<<1)|(vz1&1);
#endif

	if(!z && !uval)
	{
		TKSVC0E_EncodeSymbol(ctx, htab, 0x00);
		return;
	}

	if((z<7) && uval)
	{
		if(uval<4)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, (z<<5)|uval);
			return;
		}
		if(ebv0<32)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, (z<<5)|ebv0);
			TKSVC0E_EncodeExtraBits(ctx, uval, eb0);
			return;
		}
	}
	
	if(!z)
	{
		TKSVC_DBGBREAK
	}

//	if(z<32)
	if(ebvz0<32)
	{
		if(val)
		{
			if(zn1<4)
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0xE0|zn1);
			}else
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0xE0|ebvz1);
				TKSVC0E_EncodeExtraBits(ctx, zn1, ebz1);
			}
			TKSVC0E_EncodeCoeff(ctx, htab, 0, val);
		}else
		{
			if(z<4)
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0xE0|z);
			}else
			{
				TKSVC0E_EncodeSymbol(ctx, htab, 0xE0|ebvz0);
				TKSVC0E_EncodeExtraBits(ctx, z, ebz0);
			}
		}
		return;
	}

#if 0
	if(z)
	{
		if(z<0)
		{
			TKSVC_DBGBREAK
		}

//		TKSVC_DBGBREAK
		TKSVC0E_EncodeSymbol(ctx, htab, 0x8F);
		TKSVC0E_EncodeExtraBits(ctx, 3, 2);
		if((z>32) || val)
			TKSVC0E_EncodeCoeff(ctx, htab, z-32, val);
		return;
	}else
	{
		if(uval<4)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, uval);
			return;
		}
		if(ebv0<0x7F)
		{
			TKSVC0E_EncodeSymbol(ctx, htab, ebv0);
			TKSVC0E_EncodeExtraBits(ctx, uval, eb0);
			return;
		}
	}

	TKSVC_DBGBREAK
#endif
}
#endif

void TKSVC0E_EncodeBlockAC(TKSVC0E_EncState *ctx, int htn,
	int *blk)
{
	int z, v;
	int i, j, k;
	
	i=1; z=0;
	while(i<64)
	{
		v=blk[tksvc0e_zigzag2[i++]];
		if(!v)
			{ z++; continue; }
		TKSVC0E_EncodeCoeff(ctx, htn, z, v);
		z=0;
	}
	
	if(z)
	{
		TKSVC0E_EncodeCoeff(ctx, htn, 0, 0);
	}
}

int TKSVC0_DumpBlockAC(int *blk)
{
	int i;

	for(i=0; i<64; i++)
	{
		printf("% 4d ", blk[i]);
	}
	printf("\n");
	return(0);
}

int TKSVC0E_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;

	p=a+b-c;
	pa=(p>a)?(p-a):(a-p);
	pb=(p>b)?(p-b):(b-p);
	pc=(p>c)?(p-c):(c-p);

	p=(pa<=pb)?((pa<=pc)?a:c):((pb<=pc)?b:c);
	return(p);
}

void TKSVC0E_EncodeDcPlaneData(TKSVC0E_EncState *ctx, int htn, int pn)
{
	static const char qn2pn4[4]={0,1,2,0};
	int xs, ys;
	int *dcp;
	int a0, a1, a2, ap, qv;
	int x, y, px, z, v;
	
	xs=ctx->dcxs[pn&3];
	ys=ctx->dcys[pn&3];
	dcp=ctx->dcp[pn&3];
	
	if(pn&0xC)
	{
		xs=ctx->dcxs[qn2pn4[pn&3]];
		ys=ctx->dcys[qn2pn4[pn&3]];
		if(pn&0x4)
			{ dcp=ctx->mvxp[pn&3]; }
		if(pn&0x8)
			{ dcp=ctx->mvyp[pn&3]; }
	}
	
	z=0;
	for(y=0; y<ys; y++)
		for(x=0; x<xs; x++)
	{
		px=y*xs+x;

		if(y)
		{
			if(x)
			{
				a0=dcp[px-xs-1];
				a1=dcp[px-xs-0];
				a2=dcp[px   -1];
				ap=TKSVC0E_Paeth(a2, a1, a0);
			}else
				{ ap=dcp[px-xs]; }
		}else
		{
			if(x)
				{ ap=dcp[px-1]; }
			else
				{ ap=0; }
		}
	
//		if(z)
//		{
//			dcp[px]=ap;
//			z--;
//			continue;
//		}
		
		v=dcp[px]-ap;

#if 1
//		if(!v && (z<30))
//		if(!v && (z<255))
		if(!v)
		{
			z++;
			continue;
		}
#endif
		
		TKSVC0E_EncodeCoeff(ctx, htn, z, v);
		z=0;
	}
	
	TKSVC0E_EncodeCoeff(ctx, htn, z, 0);
	z=0;
}

void TKSVC0E_EncodeDcPlane(TKSVC0E_EncState *ctx, int pn)
{
	static const byte htpn[16]={
		0, 1, 1, 2, 0, 1, 1, 2,
		3, 3, 3, 3, 3, 3, 3, 3};
	byte *ct0;
	int i;
	
	ct0=ctx->ct;
	
	TKSVC0E_WriteBits(ctx, 0x4, 4);
	TKSVC0E_WriteBits(ctx, pn, 4);
	TKSVC0E_EncodeDcPlaneData(ctx, htpn[pn], pn);
	
	i=ctx->ct-ct0;
	printf("DC %d %d\n", pn, i);
}

int TKSVC0E_CheckAcBlockAllZeroes(s32 *blk)
{
	int i;
	
	for(i=1; i<64; i++)
	{
		if(blk[i]!=0)
			break;
	}
	return(i>=64);
}

void TKSVC0E_EncodeAcPlaneData(TKSVC0E_EncState *ctx, int htn, int pn, int qn)
{
	int blk[64];
	int *qt;
	
	int xs, ys, xsp, ysp;
	int *dcp, *acp, *mvxp, *mvyp;
	byte *yvp, *lyvp;
	int a0, a1, a2, ap, qv, mvy, mvx;
	int x, y, x1, y1, px, z, v, bpx, lpx;
	int i;

	if(pn>=4)
	{
		ctx->status=TKULZ_STATUS_BADTAG;
		return;
	}
	
	xs=ctx->dcxs[pn];
	ys=ctx->dcys[pn];
	dcp=ctx->dcp[pn];

	qt=ctx->qtab[qn];
//	qv=qt[0];
	
	acp=ctx->blkp[pn];
	
	yvp=ctx->yuvp[pn];
	lyvp=ctx->lyuvp[pn];
	xsp=xs*8;
	ysp=ys*8;
	
	mvxp=ctx->mvxp[qn];
	mvyp=ctx->mvyp[qn];
	
	z=0;
	for(y=0; y<ys; y++)
		for(x=0; x<xs; x++)
	{
		px=y*xs+x;
		
//		if(TKSVC0E_CheckAcBlockAllZeroes(acp+(px*64)) && (z<128))
		if(TKSVC0E_CheckAcBlockAllZeroes(acp+(px*64)))
			{ z++; continue; }
		
		if(z>16)
//		if(z>15)
//		if(z>20)
//		if(z>14)
//		if(0)
		{
			while(z>190)
			{
				TKSVC0E_EncodeCoeff(ctx, htn, 190+63, 0);
				z-=190;
			}
			TKSVC0E_EncodeCoeff(ctx, htn, z+63, 0);
			z=0;
		}else if(z>0)
		{
			while(z--)
				{ TKSVC0E_EncodeCoeff(ctx, htn, 0, 0); }
			z=0;
		}
		
		TKSVC0E_EncodeBlockAC(ctx, htn, acp+(px*64));
		
		if(!x && !y)
		{
//			TKSVC0_DumpBlockAC(acp+(px*64));
//			TKSVC0_DumpBlockAC(qt);
		}
	}

	if(z>16)
//	if(0)
	{
		while(z>190)
		{
			TKSVC0E_EncodeCoeff(ctx, htn, 190+63, 0);
			z-=190;
		}
		TKSVC0E_EncodeCoeff(ctx, htn, z+63, 0);
		z=0;
	}else if(z>0)
	{
		while(z--)
			{ TKSVC0E_EncodeCoeff(ctx, htn, 0, 0); }
		z=0;
	}
}

void TKSVC0E_EncodeAcPlane(TKSVC0E_EncState *ctx, int pn)
{
	static const byte htpn[16]={
		4, 5, 5, 6, 4, 5, 5, 6,
		3, 3, 3, 3, 3, 3, 3, 3};
	static const byte htqn[16]={
		0, 1, 1, 2, 0, 0, 0, 0,
		3, 3, 3, 3, 3, 3, 3, 3};

	byte *ct0;
	int i;
	
	ct0=ctx->ct;
	
	TKSVC0E_WriteBits(ctx, 0x5, 4);
	TKSVC0E_WriteBits(ctx, pn, 4);
	TKSVC0E_EncodeAcPlaneData(ctx, htpn[pn], pn, htqn[pn]);
	
	i=ctx->ct-ct0;
	printf("AC %d %d\n", pn, i);
}


#if 1
void TKSVC0E_TransDCT_Horiz(s32 *iblk, s32 *oblk)
{
	int ib[8];

	ib[0]=iblk[0];	ib[1]=iblk[1];
	ib[2]=iblk[2];	ib[3]=iblk[3];
	ib[4]=iblk[4];	ib[5]=iblk[5];
	ib[6]=iblk[6];	ib[7]=iblk[7];

	oblk[0]=ib[0]*91  +ib[1]*91  +ib[2]*91  +ib[3]*91  
			+ib[4]*91  +ib[5]*91  +ib[6]*91  +ib[7]*91;
	oblk[1]=ib[0]*126 +ib[1]*106 +ib[2]*71  +ib[3]*25  
			-ib[4]*25  -ib[5]*71  -ib[6]*106 -ib[7]*126;
	oblk[2]=ib[0]*118 +ib[1]*49  -ib[2]*49  -ib[3]*118 
			-ib[4]*118 -ib[5]*49  +ib[6]*49  +ib[7]*118;
	oblk[3]=ib[0]*106 -ib[1]*25  -ib[2]*126 -ib[3]*71  
			+ib[4]*71  +ib[5]*126 +ib[6]*25  -ib[7]*106;
	oblk[4]=ib[0]*91  -ib[1]*91  -ib[2]*91  +ib[3]*91  
			+ib[4]*91  -ib[5]*91  -ib[6]*91  +ib[7]*91;
	oblk[5]=ib[0]*71  -ib[1]*126 +ib[2]*25  +ib[3]*106 
			-ib[4]*106 -ib[5]*25  +ib[6]*126 -ib[7]*71;
	oblk[6]=ib[0]*49  -ib[1]*118 +ib[2]*118 -ib[3]*49  
			-ib[4]*49  +ib[5]*118 -ib[6]*118 +ib[7]*49;
	oblk[7]=ib[0]*25  -ib[1]*71  +ib[2]*106 -ib[3]*126 
			+ib[4]*126 -ib[5]*106 +ib[6]*71  -ib[7]*25;
}

void TKSVC0E_TransDCT_Vert(s32 *iblk, s32 *oblk)
{
	oblk[ 0]=iblk[0]*91  +iblk[8]*91  
			+iblk[16]*91  +iblk[24]*91  
			+iblk[32]*91  +iblk[40]*91  
			+iblk[48]*91  +iblk[56]*91;
	oblk[ 8]=iblk[0]*126 +iblk[8]*106 
			+iblk[16]*71  +iblk[24]*25  
			-iblk[32]*25  -iblk[40]*71  
			-iblk[48]*106 -iblk[56]*126;
	oblk[16]=iblk[0]*118 +iblk[8]*49  
			-iblk[16]*49  -iblk[24]*118 
			-iblk[32]*118 -iblk[40]*49  
			+iblk[48]*49  +iblk[56]*118;
	oblk[24]=iblk[0]*106 -iblk[8]*25  
			-iblk[16]*126 -iblk[24]*71  
			+iblk[32]*71  +iblk[40]*126 
			+iblk[48]*25  -iblk[56]*106;
	oblk[32]=iblk[0]*91  -iblk[8]*91  
			-iblk[16]*91  +iblk[24]*91  
			+iblk[32]*91  -iblk[40]*91  
			-iblk[48]*91  +iblk[56]*91;
	oblk[40]=iblk[0]*71  -iblk[8]*126 
			+iblk[16]*25  +iblk[24]*106
			-iblk[32]*106 -iblk[40]*25  
			+iblk[48]*126 -iblk[56]*71;
	oblk[48]=iblk[0]*49  -iblk[8]*118
			+iblk[16]*118 -iblk[24]*49  
			-iblk[32]*49  +iblk[40]*118 
			-iblk[48]*118 +iblk[56]*49;
	oblk[56]=iblk[0]*25  -iblk[8]*71  
			+iblk[16]*106 -iblk[24]*126 
			+iblk[32]*126 -iblk[40]*106 
			+iblk[48]*71  -iblk[56]*25;
}

void TKSVC0E_TransDCT(s32 *iblk, s32 *oblk)
{
	int s[64];
	int t[64];
	int i, j;

	TKSVC0E_TransDCT_Horiz(iblk+0, s+0);
	TKSVC0E_TransDCT_Horiz(iblk+8, s+8);
	TKSVC0E_TransDCT_Horiz(iblk+16, s+16);
	TKSVC0E_TransDCT_Horiz(iblk+24, s+24);
	TKSVC0E_TransDCT_Horiz(iblk+32, s+32);
	TKSVC0E_TransDCT_Horiz(iblk+40, s+40);
	TKSVC0E_TransDCT_Horiz(iblk+48, s+48);
	TKSVC0E_TransDCT_Horiz(iblk+56, s+56);

	TKSVC0E_TransDCT_Vert(s+0, t+0);
	TKSVC0E_TransDCT_Vert(s+1, t+1);
	TKSVC0E_TransDCT_Vert(s+2, t+2);
	TKSVC0E_TransDCT_Vert(s+3, t+3);
	TKSVC0E_TransDCT_Vert(s+4, t+4);
	TKSVC0E_TransDCT_Vert(s+5, t+5);
	TKSVC0E_TransDCT_Vert(s+6, t+6);
	TKSVC0E_TransDCT_Vert(s+7, t+7);

	for(i=0; i<64; i++)
		oblk[i]=t[i]>>16;
}
#endif

void TKSVC0E_TransWHT_Horiz(s32 *iblk, s32 *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[0];		ib1=iblk[1];
	ib2=iblk[2];		ib3=iblk[3];
	ib4=iblk[4];		ib5=iblk[5];
	ib6=iblk[6];		ib7=iblk[7];

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

	oblk[0]=c0;			oblk[1]=c4;
	oblk[2]=c6;			oblk[3]=c2;
	oblk[4]=c3;			oblk[5]=c7;
	oblk[6]=c5;			oblk[7]=c1;

//	oblk[0]=c0;			oblk[1]=c1;
//	oblk[2]=c2;			oblk[3]=c3;
//	oblk[4]=c4;			oblk[5]=c5;
//	oblk[6]=c6;			oblk[7]=c7;
}

void TKSVC0E_TransWHT_Vert(s32 *iblk, s32 *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[8*0];		ib1=iblk[8*1];
	ib2=iblk[8*2];		ib3=iblk[8*3];
	ib4=iblk[8*4];		ib5=iblk[8*5];
	ib6=iblk[8*6];		ib7=iblk[8*7];

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

	oblk[8*0]=c0;		oblk[8*1]=c4;
	oblk[8*2]=c6;		oblk[8*3]=c2;
	oblk[8*4]=c3;		oblk[8*5]=c7;
	oblk[8*6]=c5;		oblk[8*7]=c1;

//	oblk[8*0]=c0;		oblk[8*1]=c1;
//	oblk[8*2]=c2;		oblk[8*3]=c3;
//	oblk[8*4]=c4;		oblk[8*5]=c5;
//	oblk[8*6]=c6;		oblk[8*7]=c7;
}

void TKSVC0E_TransWHT(s32 *iblk, s32 *oblk)
{
	int s[64];
//	int t[64];
//	int i, j;

	TKSVC0E_TransWHT_Horiz(iblk+0, s+0);
	TKSVC0E_TransWHT_Horiz(iblk+8, s+8);
	TKSVC0E_TransWHT_Horiz(iblk+16, s+16);
	TKSVC0E_TransWHT_Horiz(iblk+24, s+24);
	TKSVC0E_TransWHT_Horiz(iblk+32, s+32);
	TKSVC0E_TransWHT_Horiz(iblk+40, s+40);
	TKSVC0E_TransWHT_Horiz(iblk+48, s+48);
	TKSVC0E_TransWHT_Horiz(iblk+56, s+56);

	TKSVC0E_TransWHT_Vert(s+0, oblk+0);
	TKSVC0E_TransWHT_Vert(s+1, oblk+1);
	TKSVC0E_TransWHT_Vert(s+2, oblk+2);
	TKSVC0E_TransWHT_Vert(s+3, oblk+3);
	TKSVC0E_TransWHT_Vert(s+4, oblk+4);
	TKSVC0E_TransWHT_Vert(s+5, oblk+5);
	TKSVC0E_TransWHT_Vert(s+6, oblk+6);
	TKSVC0E_TransWHT_Vert(s+7, oblk+7);
}

void TKSVC0E_GetPlanePixelBlock(TKSVC0E_EncState *ctx, int pn,
	byte *blk, int bx, int by)
{
	byte *yuvp;
	int xs, ys, xsp, px, dpx;
	int x, y;
	
	xs=ctx->dcxs[pn];
	ys=ctx->dcys[pn];
	xsp=xs*8;
	yuvp=ctx->yuvp[pn];
	
	for(y=0; y<8; y++)
	{
		px=(by*8+y)*xsp+(bx*8);
		dpx=y*8;
		blk[dpx+0]=yuvp[px+0];
		blk[dpx+1]=yuvp[px+1];
		blk[dpx+2]=yuvp[px+2];
		blk[dpx+3]=yuvp[px+3];
		blk[dpx+4]=yuvp[px+4];
		blk[dpx+5]=yuvp[px+5];
		blk[dpx+6]=yuvp[px+6];
		blk[dpx+7]=yuvp[px+7];
	}
}

void TKSVC0E_QuantizeTransformBlock(s32 *iblk, s32 *oblk, int *qtab)
{
	int qa, qv, q0, q1, q2, lq2;
	int tq0, tq1, tq2;
	int i, j, k;

#if 0
	for(i=0; i<64; i++)
	{
		oblk[i]=iblk[i]/qtab[i];
	}
#endif

#if 1
	lq2=-999;
	for(i=0; i<64; i++)
	{
//		j=qtab[tksvc0e_zigzag2[i]];
		qa=iblk[i];
		qv=qtab[i];
		
		if(qv==1)
		{
			oblk[i]=qa;
			lq2=qa;
			continue;
		}
		
		/* Round towards zero and towards nearest. */
		q0=qa/qv;
		q1=(qa>0)?((qa+(qv/2))/qv):((qa-(qv/2))/qv);
		
		q2=q1;	/* Initially assume nearest. */
//		q2=q0;	/* Initially assume nearest. */

#if 1
		/* If significant bits will differ, truncate towards 0 */
		tq0=(q0<<1)^(q0>>31);
		tq1=(q1<<1)^(q1>>31);
		while(tq1>=4)
			{ tq0>>=1; tq1>>=1; }
		if(tq0!=tq1)
			q2=q0;
		
		if(!lq2)
		{
			tq2=(q2<<1)^(q2>>31);
			if(tq2<3)q2=0;
		}
#endif
		
		oblk[i]=q2;
		lq2=q2;
	}
#endif
}

void TKSVC0E_TransformPlaneWHT(TKSVC0E_EncState *ctx, int pn)
{
	static const char pn2qn4[4]={0,1,1,2};
	byte pblk[64];
	s32 tblk[64];
//	byte *yuvp;
	int *blkp;
	int *dcp;
	int *qt;
	
	int xs, ys, px, qn;
	int x, y;
	int i, j, k;
	
	qn=pn2qn4[pn];
	xs=ctx->dcxs[pn];
	ys=ctx->dcys[pn];
	blkp=ctx->blkp[pn];
	dcp=ctx->dcp[pn];

	qt=ctx->qtab[qn];
	
	for(y=0; y<ys; y++)
		for(x=0; x<xs; x++)
	{
		TKSVC0E_GetPlanePixelBlock(ctx, pn, pblk, x, y);
		for(i=0; i<64; i++)
//			tblk[i]=pblk[i];
			tblk[i]=pblk[i]-128;

		if(!ctx->use_dct[qn])
		{
			TKSVC0E_TransWHT(tblk, tblk);
		}else
		{
			TKSVC0E_TransDCT(tblk, tblk);
		}

		px=y*xs+x;
		TKSVC0E_QuantizeTransformBlock(tblk, blkp+(px*64), qt);
		dcp[px]=blkp[px*64+0];

		if(!x && !y)
		{
			printf("A: ");
			TKSVC0_DumpBlockAC(tblk);
			printf("B: ");
			TKSVC0_DumpBlockAC(blkp+(px*64));

//			TKSVC0_DumpBlockAC(blk);
//			TKSVC0_DumpBlockAC(qt);
		}
	}
}

int PDZ2_BalanceTree_r(short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)
	{
//		printf("L");
		return(0);
	}

//	printf("{");

	h1=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

	if((h+h0)<=ml)	//depth limit not exceeded
	{
//		printf("}");
		return(h0);
	}

	//ok, so part of the tree is too deep
//	if((h1+1)<h2)
	if(h1<h2)
	{
		l0=nodes[root*2+1];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[root*2+0];
		nodes[root*2+0]=l0;
		nodes[root*2+1]=l1;
	}else
//		if((h2+1)<h1)
		if(h2<h1)
	{
		l0=nodes[root*2+0];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[root*2+1];
		nodes[root*2+0]=l1;
		nodes[root*2+1]=l0;
	}else
	{
//		printf("bal}");
		//rotating would be ineffective or would make things worse...
		return(h0);
	}

	//recalc depth of modified sub-tree
	l1=nodes[l0*2+0];
	l2=nodes[l0*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h3=((h1>h2)?h1:h2)+1;
	nlen[l0]=h3;

	//recalc height of root node
	l1=nodes[root*2+0];
	l2=nodes[root*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

//	printf("rebal}");

	return(h0);
}

void PDZ2_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		cl[(-root)-1]=h;
		return;
	}

	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int PDZ2_BuildLengths(int *stat, int nc, byte *cl, int ml)
{
	static short nodes[1024], nlen[512];
	static short roots[512], clen[512];
	static int cnts[512];
	int nr, nn;
	int i, j, k, l;

	nr=0; nn=0;
	for(i=0; i<nc; i++)
	{
		if(!stat[i])continue;
		roots[nr]=-(i+1);
		cnts[nr]=stat[i];
		clen[nr]=0;
		nr++;
	}

	for(i=0; i<nc; i++)cl[i]=0;
	if(!nr)
	{
		printf("empty tree\n");
		return(-1);
	}


	while(nr>1)
	{
		if(cnts[0]>=cnts[1]) { j=0; k=1; }
			else { j=1; k=0; }
		for(i=2; i<nr; i++)
		{
			if(cnts[i]<=cnts[k])
			{
				j=k; k=i;
				continue;
			}
			if(cnts[i]<=cnts[j])
			{
				j=i;
				continue;
			}
		}

		nlen[nn]=((clen[j]>clen[k])?clen[j]:clen[k])+1;
		nodes[nn*2+0]=roots[j];
		nodes[nn*2+1]=roots[k];

		roots[nr]=nn;
		cnts[nr]=cnts[j]+cnts[k];
		clen[nr]=nlen[nn];

//		printf("%d %d %d\n", cnts[j], cnts[k], cnts[nr]);

		nn++; nr++;

		l=0;
		for(i=0; i<nr; i++)
		{
			if((i==j) || (i==k))continue;
			roots[l]=roots[i];
			cnts[l]=cnts[i];
			clen[l]=clen[i];
			l++;
		}
		nr=l;
	}

	l=roots[0];
	j=clen[0];
	k=j;

	i=16;
	while((i--) && (k>ml))
		k=PDZ2_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)
	{
		printf("tree balance failure\n");
		printf("tree depth %d, org %d, %d nodes\n", k, j, nn);
		return(-2);
	}

	PDZ2_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

int PDZ2_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=PDZ2_BuildLengths(stat, nc, cl, ml);
		if(j<0)
			printf("PDZ2_BuildLengthsAdjust: Huff Fail %d\n", j);

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("PDZ2_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
	return(0);
}

int TKSVC0E_BuildLengths(TKSVC0E_EncState *ctx, int *stat, byte *cls)
{
	int i, j, k;

//	PDZ2_BuildLengthsAdjust(stat, 256, cls, 12);
	PDZ2_BuildLengthsAdjust(stat, 256, cls, TKULZ_HTABNB);
	
//	for(i=0; i<256; i++)
//		cls[i]=8;

	return(0);
}

void TKSVC0E_SetupEncodeBuffer(TKSVC0E_EncState *ctx,
	byte *dst, int dsz)
{
	ctx->ct=dst;
	ctx->win=0;
	ctx->pos=0;
	ctx->status=0;

//	ctx->hash_pos=malloc(TKULZ_HASH_SZ*TKULZ_HASH_LVL*sizeof(byte *));
//	ctx->hash_rov=malloc(TKULZ_HASH_SZ*sizeof(byte));
	
//	memset(ctx->hash_pos, 0, TKULZ_HASH_SZ*TKULZ_HASH_LVL*sizeof(byte *));
//	memset(ctx->hash_rov, 0, TKULZ_HASH_SZ*sizeof(byte));
}

void TKSVC0E_TeardownBuffers(TKSVC0E_EncState *ctx)
{
	int i, j, k;

	if(ctx->dcp[0])
	{
		for(i=0; i<4; i++)
		{
			if(ctx->dcp[i])
				free(ctx->dcp[i]);
			if(ctx->yuvp[i])
				free(ctx->yuvp[i]);
			if(ctx->lyuvp[i])
				free(ctx->lyuvp[i]);
			if(ctx->mvxp[i])
				free(ctx->mvxp[i]);
			if(ctx->mvyp[i])
				free(ctx->mvyp[i]);

			ctx->dcp[i]=NULL;
			ctx->yuvp[i]=NULL;
			ctx->lyuvp[i]=NULL;
			ctx->mvxp[i]=NULL;
			ctx->mvyp[i]=NULL;
		}
	}
}

void TKSVC0E_SetupForResolution(TKSVC0E_EncState *ctx)
{
	int xs, ys, xs_y, ys_y, xs_uv, ys_uv;
	int i, j, k;
	
	xs=ctx->xs;
	ys=ctx->ys;

	if(ctx->is_444)
	{
		xs_y=((xs+7)/8);
		ys_y=((ys+7)/8);
		xs_uv=xs_y;
		ys_uv=ys_y;
	}else
	{
		xs_y=((xs+15)/16)*2;
		ys_y=((ys+15)/16)*2;
		xs_uv=xs_y/2;
		ys_uv=ys_y/2;
	}

	ctx->dcxs[0]=xs_y;
	ctx->dcxs[1]=xs_uv;
	ctx->dcxs[2]=xs_uv;
	ctx->dcxs[3]=xs_y;

	ctx->dcys[0]=ys_y;
	ctx->dcys[1]=ys_uv;
	ctx->dcys[2]=ys_uv;
	ctx->dcys[3]=ys_y;
	
	if(!ctx->dcp[0])
	{
		for(i=0; i<4; i++)
		{
			ctx->dcp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*sizeof(int));
			ctx->yuvp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*64);
			ctx->lyuvp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*64);

			ctx->blkp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*64*sizeof(int));
		}

		ctx->mvxp[0]=malloc(ctx->dcxs[0]*ctx->dcys[0]*sizeof(int));
		ctx->mvyp[0]=malloc(ctx->dcxs[0]*ctx->dcys[0]*sizeof(int));
		ctx->mvxp[1]=malloc(ctx->dcxs[1]*ctx->dcys[1]*sizeof(int));
		ctx->mvyp[1]=malloc(ctx->dcxs[1]*ctx->dcys[1]*sizeof(int));
		ctx->mvxp[2]=malloc(ctx->dcxs[3]*ctx->dcys[3]*sizeof(int));
		ctx->mvyp[2]=malloc(ctx->dcxs[3]*ctx->dcys[3]*sizeof(int));
	}
}

void TKSVC0E_SetFrameSize(TKSVC0E_EncState *ctx, int xs, int ys, int is444)
{
	int i, j, k;

	if((xs!=ctx->xs) || (ys!=ctx->ys) || (ctx->is_444!=is444))
	{
		TKSVC0E_TeardownBuffers(ctx);
		ctx->xs=xs;
		ctx->ys=ys;
		ctx->is_444=is444;
		TKSVC0E_SetupForResolution(ctx);
		
		for(i=0; i<8; i++)
			for(j=0; j<256; j++)
		{
			ctx->hstab[i][j]=1;
		}
	}
}

void TKSVC0E_UpdateFrameImage(TKSVC0E_EncState *ctx, byte *ibuf)
{
	byte *yvp, *uvp, *vvp, *avp;

	int xs, ys, xsh, ysh, px0, px1, px;
	int cr0, cg0, cb0, ca0;
	int cr1, cg1, cb1, ca1;
	int cr2, cg2, cb2, ca2;
	int cr3, cg3, cb3, ca3;
	int cy0, cy1, cy2, cy3, cy;
	int cu0, cu1, cu2, cu3, cu;
	int cv0, cv1, cv2, cv3, cv;
	int x, y;
	
	yvp=ctx->yuvp[0];
	uvp=ctx->yuvp[1];
	vvp=ctx->yuvp[2];
	avp=ctx->yuvp[3];
	
	xs=ctx->xs;
	ys=ctx->ys;
	xsh=xs/2;
	ysh=ys/2;

	if(ctx->is_444)
	{
		for(y=0; y<ys; y++)
			for(x=0; x<xs; x++)
		{
			px0=(y*xs+x)*4;
			px1=(y*xs+x);
			cr0=ibuf[px0+2];	cg0=ibuf[px0+1];
			cb0=ibuf[px0+0];	ca0=ibuf[px0+3];
			yvp[px1]=cg0;		uvp[px1]=cb0;
			vvp[px1]=cr0;		avp[px1]=ca0;
		}
		return;
	}
	
	for(y=0; y<ysh; y++)
		for(x=0; x<xsh; x++)
	{
		px0=((y*2+0)*xs+(x*2))*4;
		px1=((y*2+1)*xs+(x*2))*4;
		cr0=ibuf[px0+2];	cg0=ibuf[px0+1];
		cb0=ibuf[px0+0];	ca0=ibuf[px0+3];
		cr1=ibuf[px0+6];	cg1=ibuf[px0+5];
		cb1=ibuf[px0+4];	ca1=ibuf[px0+7];
		cr2=ibuf[px1+2];	cg2=ibuf[px1+1];
		cb2=ibuf[px1+0];	ca2=ibuf[px1+3];
		cr3=ibuf[px1+6];	cg3=ibuf[px1+5];
		cb3=ibuf[px1+4];	ca3=ibuf[px1+7];
		
//		cy0=(cr0*3+cb0+cg0*4)/8;
//		cy1=(cr1*3+cb1+cg1*4)/8;
//		cy2=(cr2*3+cb2+cg2*4)/8;
//		cy3=(cr3*3+cb3+cg3*4)/8;

		cy0=(cr0+cb0+cg0*2)/4;
		cy1=(cr1+cb1+cg1*2)/4;
		cy2=(cr2+cb2+cg2*2)/4;
		cy3=(cr3+cb3+cg3*2)/4;
		
#if 1
		cu0=(cb0-cy0)/2+128;
		cu1=(cb1-cy1)/2+128;
		cu2=(cb2-cy2)/2+128;
		cu3=(cb3-cy3)/2+128;
		
		cv0=(cr0-cy0)/2+128;
		cv1=(cr1-cy1)/2+128;
		cv2=(cr2-cy2)/2+128;
		cv3=(cr3-cy3)/2+128;
#endif

#if 0
		cu0=(cb0-cg0)/2+128;
		cu1=(cb1-cg1)/2+128;
		cu2=(cb2-cg2)/2+128;
		cu3=(cb3-cg3)/2+128;
		
		cv0=(cr0-cg0)/2+128;
		cv1=(cr1-cg1)/2+128;
		cv2=(cr2-cg2)/2+128;
		cv3=(cr3-cg3)/2+128;
#endif
		
		cy=(cy0+cy1+cy2+cy3)/4;
		cu=(cu0+cu1+cu2+cu3)/4;
		cv=(cv0+cv1+cv2+cv3)/4;
		
		px0=((y*2+0)*xs+(x*2));
		px1=((y*2+1)*xs+(x*2));
		px=(y*xsh+x);
		yvp[px0+0]=cy0;		yvp[px0+1]=cy1;
		yvp[px1+0]=cy2;		yvp[px1+1]=cy3;
		avp[px0+0]=ca0;		avp[px0+1]=ca1;
		avp[px1+0]=ca2;		avp[px1+1]=ca3;
		uvp[px]=cu;
		vvp[px]=cv;
	}
}

void TKSVC0E_SetupSwapFrames(TKSVC0E_EncState *ctx)
{
	byte *p1, *p2;
	int i;

	ctx->is_pf=1;	/* Assume P-Frame for now */

	for(i=0; i<4; i++)
	{
		p1=ctx->yuvp[i];		p2=ctx->lyuvp[i];
		ctx->yuvp[i]=p2;		ctx->lyuvp[i]=p1;
	}
}

static int pdjpg_ijg_qtab_y[64] = {
16, 11, 10, 16,  24,  40,  51,  61,
12, 12, 14, 19,  26,  58,  60,  55,
14, 13, 16, 24,  40,  57,  69,  56,
14, 17, 22, 29,  51,  87,  80,  62,
18, 22, 37, 56,  68, 109, 103,  77,
24, 35, 55, 64,  81, 104, 113,  92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103,  99};

static int pdjpg_ijg_qtab_uv[64] = {
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
47, 66, 99, 99, 99, 99, 99, 99,
24, 26, 56, 99, 99, 99, 99, 99,
18, 21, 26, 66, 99, 99, 99, 99,
17, 18, 24, 47, 99, 99, 99, 99};

static int pdjpg_ijg_qtab_y2[64];

void TKSVC0E_MakeQuantTabFastIJG_Y(
	int *tab, float qf)
{
	double s, q;
	int i, j;

	q = (qf * 100);
	s = (q < 50) ? 5000 / q : (200 - 2 * q);
	for (i = 0; i < 64; i++)
	{
//		j = (int)((s * pdjpg_ijg_qtab_y[i] + 50) / 100);
		j = (int)((s * pdjpg_ijg_qtab_y2[i] + 50) / 100);
		j = (j < 1) ? 1 : ((j < 256) ? j : 255);
		tab[i] = j;
//		tab[tksvc0_zigzag2[i]] = j;
	}
}

void TKSVC0E_MakeQuantTabFastIJG_UV(
	int *tab, float qf)
{
	double s, q;
	int i, j;

	q = (qf * 100);
	s = (q < 50) ? 5000 / q : (200 - 2 * q);
	for (i = 0; i < 64; i++)
	{
		j = (int)((s * pdjpg_ijg_qtab_uv[i] + 50) / 100);
		j = (j < 1) ? 1 : ((j < 256) ? j : 255);
		tab[i] = j;
//		tab[tksvc0_zigzag2[i]] = j;
	}
}

void TKSVC0E_SetQuality(TKSVC0E_EncState *ctx, int qual)
{
	int *qt;
	int i, j, k;
	
#if 0
	for(i=0; i<64; i++)
	{
		pdjpg_ijg_qtab_y2[i]=
			(13*pdjpg_ijg_qtab_y[i]+
			  3*pdjpg_ijg_qtab_uv[i])/16;
	}
#endif

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		k=((16+3*i)*(16+3*j))/16;
		if(k>255)k=255;
		pdjpg_ijg_qtab_y2[i*8+j]=k;
//			((16+2*i)*(16+2*j))/16;
//			((8+2*i)*(8+2*j))/16;
	}
	
	qt=pdjpg_ijg_qtab_y2;
	i=tksvc0e_zigzag[63];
	qt[i]=qt[i]/2;
	i=tksvc0e_zigzag[62];
	qt[i]=(qt[i]*3)/4;
	i=tksvc0e_zigzag[61];
	qt[i]=(qt[i]*3)/4;

	ctx->quality=qual;

	TKSVC0E_MakeQuantTabFastIJG_Y(ctx->qtab[0], (qual&127)/100.0);
//	TKSVC0E_MakeQuantTabFastIJG_UV(ctx->qtab[0], (qual&127)/100.0);
//	TKSVC0E_MakeQuantTabFastIJG_UV(ctx->qtab[1], ((qual&127)/100.0)*1.4);
	TKSVC0E_MakeQuantTabFastIJG_Y(ctx->qtab[1], (qual&127)/100.0);
	TKSVC0E_MakeQuantTabFastIJG_Y(ctx->qtab[2], (qual&127)/100.0);

	ctx->use_dct[0]=0;
	ctx->use_dct[1]=0;
	ctx->use_dct[2]=0;
	
	if((qual&127)<96)
	{
		ctx->use_dct[0]=1;
		ctx->use_dct[1]=1;
	}
}

void TKSVC0E_EncodeHuffmanTable(TKSVC0E_EncState *ctx, int tn)
{
	byte *ct0;
	int i, j, k;

	TKSVC0E_BuildLengths(ctx, ctx->hstab[tn], ctx->hltab[tn]);
	
//	for(i=0; i<256; i++)
//		ctx->hltab[tn][i]=8;
	
	TKSVC0E_SetupEncTableLengths(ctx, ctx->hetab[tn], ctx->hltab[tn]);

	ct0=ctx->ct;

	TKSVC0E_WriteBits(ctx, 0x1, 4);
	TKSVC0E_WriteBits(ctx, tn, 4);
	TKSVC0E_WritePackedLengths(ctx, ctx->hltab[tn]);

	j=ctx->ct-ct0;
	printf("Ht %d, %d bytes\n", tn, j);
}

void TKSVC0E_EncodeQuantizationTable(TKSVC0E_EncState *ctx, int qn)
{
	byte *ct0;
	int *qtab;
	int i, j, k, l;
	int i1, qn1;
	
	qn1=qn;
	if(ctx->use_dct[qn])
		qn1|=4;

	TKSVC0E_WriteBits(ctx, 0x2, 4);
//	TKSVC0E_WriteBits(ctx, qn, 4);
	TKSVC0E_WriteBits(ctx, qn1, 4);
	
	qtab=ctx->qtab[qn&3];

#if 0
	printf("%d: ", qn);
	for(i=0; i<64; i++)
	{
		printf("%d ", qtab[i]);
	}
	printf("\n");
#endif

	ct0=ctx->ct;
	l=0;
	for(i=0; i<64; i++)
	{
		j=qtab[tksvc0e_zigzag2[i]];
		
//		if(!i && ctx->use_dct[qn])
//			{ j=-j; }
		
		k=j-l;
		k=(k<<1)^(k>>31);
		l=j;

#if 1
		for(i1=0; i1<4; i1++)
		{
//			if(k<(1<<(4+i1*3)))
//			if(k<(1<<(3+i1*3)))
//			if(k<(1<<(3+i1*2)))
			if(k<(1<<(2+i1*3)))
//			if(k<(1<<(2+i1*2)))
//			if(k<(1<<(2+(i1+i1/2)*2)))
			{
				TKSVC0E_WriteBits(ctx, i1, 2);
//				TKSVC0E_WriteBits(ctx, k, 4+i1*3);
//				TKSVC0E_WriteBits(ctx, k, 3+i1*3);
//				TKSVC0E_WriteBits(ctx, k, 3+i1*2);
				TKSVC0E_WriteBits(ctx, k, 2+i1*3);
//				TKSVC0E_WriteBits(ctx, k, 2+i1*2);
//				TKSVC0E_WriteBits(ctx, k, 2+(i1+i1/2)*2);
				break;
			}
		}
#endif

#if 0
		for(i1=0; i1<7; i1++)
		{
			if(k<(1<<(2+i1)))
			{
				TKSVC0E_WriteBits(ctx, i1, 3);
				TKSVC0E_WriteBits(ctx, k, 2+i1);
				break;
			}
		}
#endif

#if 0
		if(k<8)
		{
			TKSVC0E_WriteBits(ctx, k, 4);
			continue;
		}else
		{
			TKSVC0E_WriteBits(ctx, k>>5, 4);
			TKSVC0E_WriteBits(ctx, k&31, 5);
			continue;
		}
#endif

#if 0
		if(k<12)
		{
			TKSVC0E_WriteBits(ctx, k, 4);
			continue;
		}else
		{
			for(i1=0; i1<4; i1++)
			{
//				if(k<(1<<(4+i1*2)))
				if(k<(1<<(3+i1*2)))
//				if(k<(1<<(2+i1*2)))
				{
					TKSVC0E_WriteBits(ctx, 12+i1, 4);
//					TKSVC0E_WriteBits(ctx, k, 4+i1*2);
					TKSVC0E_WriteBits(ctx, k, 3+i1*2);
//					TKSVC0E_WriteBits(ctx, k, 2+i1*2);
					break;
				}
			}
		}
#endif

		
#if 0
		if(k<16)
		{
			TKSVC0E_WriteBits(ctx, 0, 2);
			TKSVC0E_WriteBits(ctx, k, 4);
			continue;
		}
		if(k<128)
		{
			TKSVC0E_WriteBits(ctx, 1, 2);
			TKSVC0E_WriteBits(ctx, k, 7);
			continue;
		}
		if(k<1024)
		{
			TKSVC0E_WriteBits(ctx, 2, 2);
			TKSVC0E_WriteBits(ctx, k, 10);
			continue;
		}
		if(k<8192)
		{
			TKSVC0E_WriteBits(ctx, 3, 2);
			TKSVC0E_WriteBits(ctx, k, 13);
			continue;
		}
#endif
	}
	
	j=ctx->ct-ct0;
	printf("Qt %d, %d bytes\n", qn, j);
}

int TKSVC0E_EncodeBuffer(TKSVC0E_EncState *ctx,
	byte *dst, int dsz)
{
	if(!ctx->dcp[0])
	{
		ctx->status=TKULZ_STATUS_BADTAG;
		return(-1);
	}

	if(!ctx->quality)
	{
		TKSVC0E_SetQuality(ctx, 75);
	}

	TKSVC0E_TransformPlaneWHT(ctx, 0);
	TKSVC0E_TransformPlaneWHT(ctx, 1);
	TKSVC0E_TransformPlaneWHT(ctx, 2);
	TKSVC0E_TransformPlaneWHT(ctx, 3);

	if(!ctx->is_pf)
	{
		/* Gather Huffman Stats */
		TKSVC0E_SetupEncodeBuffer(ctx, NULL, 0);

//		TKSVC0E_EncodeQuantizationTable(ctx, 0);
//		TKSVC0E_EncodeQuantizationTable(ctx, 1);
//		TKSVC0E_EncodeQuantizationTable(ctx, 2);

		TKSVC0E_EncodeDcPlane(ctx, 0);
		TKSVC0E_EncodeDcPlane(ctx, 1);
		TKSVC0E_EncodeDcPlane(ctx, 2);
		TKSVC0E_EncodeDcPlane(ctx, 3);
		
		TKSVC0E_EncodeAcPlane(ctx, 0);
		TKSVC0E_EncodeAcPlane(ctx, 1);
		TKSVC0E_EncodeAcPlane(ctx, 2);
		TKSVC0E_EncodeAcPlane(ctx, 3);
	}


	TKSVC0E_SetupEncodeBuffer(ctx, dst, dsz);
	
	if(!ctx->is_pf)
	{
		TKSVC0E_EncodeHuffmanTable(ctx, 0);
		TKSVC0E_EncodeHuffmanTable(ctx, 1);
		TKSVC0E_EncodeHuffmanTable(ctx, 2);
		TKSVC0E_EncodeHuffmanTable(ctx, 3);
		TKSVC0E_EncodeHuffmanTable(ctx, 4);
		TKSVC0E_EncodeHuffmanTable(ctx, 5);
		TKSVC0E_EncodeHuffmanTable(ctx, 6);
		TKSVC0E_EncodeHuffmanTable(ctx, 7);
		
		TKSVC0E_EncodeQuantizationTable(ctx, 0);
		TKSVC0E_EncodeQuantizationTable(ctx, 1);
		TKSVC0E_EncodeQuantizationTable(ctx, 2);
	}

	TKSVC0E_EncodeDcPlane(ctx, 0);
	TKSVC0E_EncodeDcPlane(ctx, 1);
	TKSVC0E_EncodeDcPlane(ctx, 2);
	TKSVC0E_EncodeDcPlane(ctx, 3);
	
	if(ctx->is_pf)
	{
		TKSVC0E_EncodeDcPlane(ctx, 0x8);
		TKSVC0E_EncodeDcPlane(ctx, 0x9);
		TKSVC0E_EncodeDcPlane(ctx, 0xA);

		TKSVC0E_EncodeDcPlane(ctx, 0xC);
		TKSVC0E_EncodeDcPlane(ctx, 0xD);
		TKSVC0E_EncodeDcPlane(ctx, 0xE);
	}

	TKSVC0E_EncodeAcPlane(ctx, 0);
	TKSVC0E_EncodeAcPlane(ctx, 1);
	TKSVC0E_EncodeAcPlane(ctx, 2);
	TKSVC0E_EncodeAcPlane(ctx, 3);

	TKSVC0E_WriteBits(ctx, 0x0, 4);
	TKSVC0E_FlushWriteBits(ctx);

	return(ctx->ct-dst);
}


TKSVC0E_EncState *TKSVC0E_CreateContext(void)
{
	TKSVC0E_EncState *ctx;
	
	ctx=malloc(sizeof(TKSVC0E_EncState));
	memset(ctx, 0, sizeof(TKSVC0E_EncState));
	return(ctx);
}

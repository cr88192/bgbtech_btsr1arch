
#define		BTIC2F_TWOCC(a, b)			((a)|((b)<<8))
#define		BTIC2F_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define		BTIC2F_TCC_IX	BTIC2F_TWOCC('I', 'X')
#define		BTIC2F_TCC_PX	BTIC2F_TWOCC('P', 'X')
// #define		BTIC2F_TCC_IZ	BTIC2F_TWOCC('I', 'Z')
// #define		BTIC2F_TCC_PZ	BTIC2F_TWOCC('P', 'Z')
#define		BTIC2F_TCC_HX	BTIC2F_TWOCC('H', 'X')
// #define		BTIC2F_TCC_PT	BTIC2F_TWOCC('P', 'T')
#define		BTIC2F_TCC_HT	BTIC2F_TWOCC('H', 'T')
#define		BTIC2F_TCC_QT	BTIC2F_TWOCC('Q', 'T')

// #define		BTIC2F_TCC_Z3	BTIC2F_TWOCC('Z', '3')
// #define		BTIC2F_TCC_Z4	BTIC2F_TWOCC('Z', '4')


#define		unaligned		__unaligned
#define		force_inline	__forceinline

#define		btic2f_getu16(p)		(*(unaligned u16 *)(p))
#define		btic2f_getu32(p)		(*(unaligned u32 *)(p))
#define		btic2f_getu64(p)		(*(unaligned u64 *)(p))

#define		btic2f_setu16(p, v)		((*(unaligned u16 *)(p))=(v))
#define		btic2f_setu32(p, v)		((*(unaligned u32 *)(p))=(v))
#define		btic2f_setu64(p, v)		((*(unaligned u64 *)(p))=(v))


#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

// #define BTIC2F_LEN13

#ifdef BTIC2F_LEN13
#define BTIC2F_HTABSZ 8192
#define BTIC2F_HTABNB 13
#else
#define BTIC2F_HTABSZ 4096
#define BTIC2F_HTABNB 12
#endif

#define BTIC2F_STATUS_BADHT	2

typedef struct BTIC2F_DecState_s BTIC2F_DecState;

struct BTIC2F_DecState_s {
	byte *cs;
	u32 win;
	byte pos;
	byte status;

	int xs;			//image width (pixels)
	int ys;			//image height (pixels)
	int bxs;		//image width (macroblocks)
	int bys;		//image height (macroblocks)
	int fl;
	byte clrs;
	byte mbty;
	byte mbshr;
	byte has_alpha;

	u16 htab_dc[BTIC2F_HTABSZ];
	u16 htab_ac[BTIC2F_HTABSZ];
	byte qtab_y[64];
	byte qtab_uv[64];
	
	int dcp_y;			//DC predictor, Y
	int dcp_u;			//DC predictor, U
	int dcp_v;			//DC predictor, V
	int dcp_a;			//DC predictor, A
	
//	int *dctab_y;		//DC predictor table, Y (macroblock)
//	int *dctab_u;		//DC predictor table, U (macroblock)
//	int *dctab_v;		//DC predictor table, V (macroblock)

void (*DecodeTransBlock)(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride);
void (*DecodeTransBlockClip)(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip);

void (*DecodeTransBlockYuv420)(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride);
void (*DecodeTransBlockYuv444)(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride);
void (*DecodeTransBlockYuv444A)(
	s16 *pxy, s16 *pxu, s16 *pxv, s16 *pxa,
	byte *iptr, int xstride, int ystride);

};

BTIC2F_DecState *BTIC2F_AllocDecodeContext()
{
	BTIC2F_DecState *ctx;
	ctx=malloc(sizeof(BTIC2F_DecState));
	memset(ctx, 0, sizeof(BTIC2F_DecState));
	return(ctx);
}

int BTIC2F_SetupReadBits(BTIC2F_DecState *ctx, byte *cs, int csz)
{
	ctx->cs=cs;
	ctx->pos=0;
	ctx->win=btic2f_getu32(cs);
	ctx->status=0;
	return(0);
}

#if 0

force_inline int BTIC2F_PeekBits(BTIC2F_DecState *ctx, int bits)
{
	return((ctx->win>>ctx->pos)&((1<<bits)-1));
}

force_inline void BTIC2F_SkipBits(BTIC2F_DecState *ctx, int bits)
{
#if 1
	byte *cs;
	int p;

	p=ctx->pos;
	p=p+bits;
	cs=ctx->cs;
	cs=cs+(p>>3);
	ctx->pos=p&7;
	ctx->cs=cs;
	ctx->win=btic2f_getu32(cs);
#endif

#if 0
	ctx->pos+=bits;
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
#endif
}

force_inline int BTIC2F_DecodeHuffSym(BTIC2F_DecState *ctx, u16 *htab)
{
	int b, c;
	b=(ctx->win>>ctx->pos)&(BTIC2F_HTABSZ-1);
	c=htab[b];
//	if(!((c>>8)&15))
//		{ __debugbreak(); }
	BTIC2F_SkipBits(ctx, (c>>8)&15);
	return(c&255);
}

force_inline int BTIC2F_ReadBits(BTIC2F_DecState *ctx, int bits)
{
#if 1
	byte *cs;
	int p, b;

	p=ctx->pos;
	b=ctx->win>>p;
	p=p+bits;
	cs=ctx->cs;
	b=b&((1<<bits)-1);
	cs=cs+(p>>3);
	ctx->pos=p&7;
	ctx->cs=cs;
	ctx->win=btic2f_getu32(cs);
	return(b);
#endif

#if 0
	int b;
	b=BTIC2F_PeekBits(ctx, bits);
	BTIC2F_SkipBits(ctx, bits);
	return(b);
#endif
}

#endif


// force_inline
int BTIC2F_ReadBits(BTIC2F_DecState *ctx, int bits)
{
	byte *cs;
	int p, b;

	cs=ctx->cs;
	p=ctx->pos;
	b=btic2f_getu32(cs);
	b=b>>p;
	p=p+bits;
	b=b&((1<<bits)-1);
	cs=cs+(p>>3);
	ctx->pos=p&7;
	ctx->cs=cs;
	return(b);
}

// force_inline
int BTIC2F_DecodeHuffSym(BTIC2F_DecState *ctx, u16 *htab)
{
	byte *cs;
	int p, b, c, n;

	cs=ctx->cs;
	p=ctx->pos;
	b=btic2f_getu32(cs);
	b=(b>>p)&(BTIC2F_HTABSZ-1);
	c=htab[b];
	n=(c>>8)&15;
	p=p+n;
	cs=cs+(p>>3);
	ctx->pos=p&7;
	ctx->cs=cs;

	return(c&255);
}

int BTIC2F_ReadPackedLengths(BTIC2F_DecState *ctx, byte *cls)
{
	byte *t, *te;
	int i, j, c, nz, zc, lc;
	
	t=cls; te=cls+256; lc=0;
	while(t<te)
	{
		c=BTIC2F_ReadBits(ctx, 4); zc=0; nz=0;
		if(c<14)
			{ *t++=c; lc=c; }
		else if(c==14)
			{ nz=BTIC2F_ReadBits(ctx, 4)+3; }
		else if(c==15)
		{
			j=BTIC2F_ReadBits(ctx, 2);
			if(j==0)
				{ nz=BTIC2F_ReadBits(ctx, 6)+19; }
			else if(j==1)
			{
				nz=BTIC2F_ReadBits(ctx, 6)+3; zc=lc;
				if(nz==3)
					{ nz=te-t; zc=0; }
			}
			else
				{ ctx->status=BTIC2F_STATUS_BADHT; }
		}
		while(nz>0)
			{ *t++=zc; nz--; }
	}
	
	if(t>te)
		{ ctx->status=BTIC2F_STATUS_BADHT; }
	return(0);
}

byte BTIC2F_TransposeByte(byte v)
{
	static const byte trans4[16]={
		0x0,0x8,0x4,0xC, 0x2,0xA,0x6,0xE,
		0x1,0x9,0x5,0xD, 0x3,0xB,0x7,0xF};
	return((trans4[v&15]<<4)|trans4[(v>>4)&15]);
}

u16 BTIC2F_TransposeWord(u16 v)
	{ return((BTIC2F_TransposeByte(v&255)<<8)|BTIC2F_TransposeByte(v>>8)); }

void BTIC2F_SetupTableLengths(BTIC2F_DecState *ctx, u16 *htab, byte *cls)
{
	int c, l, tc, ntc;
	int i, j, k;

	for(i=0; i<BTIC2F_HTABSZ; i++)
		htab[i]=0;
	
	c=0;
	for(l=1; l<(BTIC2F_HTABNB+1); l++)
	{
		for(i=0; i<256; i++)
			if(cls[i]==l)
		{
			ntc=1<<(BTIC2F_HTABNB-l);
			tc=c<<(BTIC2F_HTABNB-l);
			for(j=0; j<ntc; j++)
			{
				k=BTIC2F_TransposeWord(tc+j)>>(16-BTIC2F_HTABNB);
				htab[k]=(l<<8)|i;
			}
			c++;
		}
		c=c<<1;
	}
}

void BTIC2F_DecodeHuffTable(BTIC2F_DecState *ctx, int tn)
{
	byte cl[256];
	BTIC2F_ReadPackedLengths(ctx, cl);
	if(tn==1)		{ BTIC2F_SetupTableLengths(ctx, ctx->htab_dc, cl); }
	else if(tn==2)	{ BTIC2F_SetupTableLengths(ctx, ctx->htab_ac, cl); }
//	else if(tn==3)	{ BTIC2F_SetupTableLengths(ctx, ctx->htab_d, cl); }
}

int BTIC2F_ReadHuffDcSym(BTIC2F_DecState *ctx)
	{ return(BTIC2F_DecodeHuffSym(ctx, ctx->htab_dc)); }
int BTIC2F_ReadHuffAcSym(BTIC2F_DecState *ctx)
	{ return(BTIC2F_DecodeHuffSym(ctx, ctx->htab_ac)); }

//static byte btic2f_vln_basetab[32]=
//{	0x00, 0x01, 0x02, 0x03,  0x0A, 0x0B, 0x12, 0x13,
//	0x1A, 0x1B, 0x22, 0x23,  0x2A, 0x2B, 0x32, 0x33,
//	0x3A, 0x3B, 0x42, 0x43,  0x4A, 0x4B, 0x52, 0x53,
//	0x5A, 0x5B, 0x62, 0x63,  0x6A, 0x6B, 0x72, 0x73 };

static byte btic2f_vln_basetab[32]=
{	0x00, 0x01, 0x02, 0x03,  0x12, 0x13, 0x22, 0x23,	//00..07
	0x32, 0x33, 0x42, 0x43,  0x52, 0x53, 0x62, 0x63,	//08..0F
	0x72, 0x73, 0x82, 0x83,  0x92, 0x93, 0xA2, 0xA3,	//10..17
	0xB2, 0xB3, 0xC2, 0xC3,  0xD2, 0xD3, 0xE2, 0xE3 };	//18..1F

static byte btic2f_inv_zigzag[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};

int BTIC2F_ReadBlockDc(BTIC2F_DecState *ctx, int qdc)
{
	int v, bt, eb;

	v=BTIC2F_ReadHuffDcSym(ctx);
//	if(v>>6)
//	{
//		__debugbreak();
//	}

	bt=btic2f_vln_basetab[v&31];
	eb=bt>>4;
	v=((bt&7)<<eb)|BTIC2F_ReadBits(ctx, eb);
	v=(v>>1)^(((int)(v<<31))>>31);
	return(v*qdc);
}

void BTIC2F_ReadBlockAc(BTIC2F_DecState *ctx, s32 *oblk, int dc, byte *qtab)
{
	int cn, v, qv, bt, eb, ci;

	memset(oblk, 0, 64*sizeof(int));

	oblk[0]=dc; cn=1;
	while(cn<64)
	{
//		v=BTIC2F_ReadHuffAcSym(ctx);
		v=BTIC2F_DecodeHuffSym(ctx, ctx->htab_ac);
		if(!v)break;
		cn+=v>>5;
		bt=btic2f_vln_basetab[v&31];
		ci=btic2f_inv_zigzag[cn];
		cn++;
		eb=bt>>4;
		qv=qtab[ci];
		v=((bt&7)<<eb)|BTIC2F_ReadBits(ctx, eb);
		v=(v>>1)^(((int)(v<<31))>>31);
		oblk[ci]=v*qv;
	} 
}

force_inline void BTIC2F_TransIBHT_8V(s32 *iblk, s32 *oblk)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[8*0];	c1=iblk[8*1];
	c2=iblk[8*2];	c3=iblk[8*3];
	c4=iblk[8*4];	c5=iblk[8*5];
	c6=iblk[8*6];	c7=iblk[8*7];

	b1=(c0>>1)-c1;	b0=c0-b1;
	a1=(b0>>1)-c2;	a0=b0-a1;
	a3=(b1>>1)-c3;	a2=b1-a3;

	ob1=(a0>>1)-c4;	ob0=a0-ob1;
	ob3=(a1>>1)-c5;	ob2=a1-ob3;
	ob5=(a2>>1)-c6;	ob4=a2-ob5;
	ob7=(a3>>1)-c7;	ob6=a3-ob7;

	oblk[8*0]=ob0;	oblk[8*1]=ob1;
	oblk[8*2]=ob2;	oblk[8*3]=ob3;
	oblk[8*4]=ob4;	oblk[8*5]=ob5;
	oblk[8*6]=ob6;	oblk[8*7]=ob7;
}

force_inline void BTIC2F_TransIBHT_8H(s32 *iblk, s16 *oblk)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[0];	c1=iblk[1];	c2=iblk[2];	c3=iblk[3];
	c4=iblk[4];	c5=iblk[5];	c6=iblk[6];	c7=iblk[7];

	b1=(c0>>1)-c1;	b0=c0-b1;
	a1=(b0>>1)-c2;	a0=b0-a1;
	a3=(b1>>1)-c3;	a2=b1-a3;

	ob1=(a0>>1)-c4;	ob0=a0-ob1;
	ob3=(a1>>1)-c5;	ob2=a1-ob3;
	ob5=(a2>>1)-c6;	ob4=a2-ob5;
	ob7=(a3>>1)-c7;	ob6=a3-ob7;

	oblk[0]=ob0;	oblk[1]=ob1;
	oblk[2]=ob2;	oblk[3]=ob3;
	oblk[4]=ob4;	oblk[5]=ob5;
	oblk[6]=ob6;	oblk[7]=ob7;
}

void BTIC2F_TransIBHT8Stride(s32 *iblk, s16 *oblk, int ystride)
{
	int t[64];

	BTIC2F_TransIBHT_8V(iblk+0, t+0);
	BTIC2F_TransIBHT_8V(iblk+1, t+1);
	BTIC2F_TransIBHT_8V(iblk+2, t+2);
	BTIC2F_TransIBHT_8V(iblk+3, t+3);
	BTIC2F_TransIBHT_8V(iblk+4, t+4);
	BTIC2F_TransIBHT_8V(iblk+5, t+5);
	BTIC2F_TransIBHT_8V(iblk+6, t+6);
	BTIC2F_TransIBHT_8V(iblk+7, t+7);

	BTIC2F_TransIBHT_8H(t+0*8, oblk+0*ystride);
	BTIC2F_TransIBHT_8H(t+1*8, oblk+1*ystride);
	BTIC2F_TransIBHT_8H(t+2*8, oblk+2*ystride);
	BTIC2F_TransIBHT_8H(t+3*8, oblk+3*ystride);
	BTIC2F_TransIBHT_8H(t+4*8, oblk+4*ystride);
	BTIC2F_TransIBHT_8H(t+5*8, oblk+5*ystride);
	BTIC2F_TransIBHT_8H(t+6*8, oblk+6*ystride);
	BTIC2F_TransIBHT_8H(t+7*8, oblk+7*ystride);
}

#if 0
force_inline int btic2f_clamp255(int v)
{
	int v1, vp1, vn1;

	vp1=(~(((v>>8)-1)>>31))&255;
	vn1=~(v>>31);
	v1=(v|vp1)&vn1;
	return(v1);
}
#endif

#if 0
force_inline int btic2f_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}
#endif

#if 1
force_inline int btic2f_clamp255(int v)
{
	if(v<0)v=0;
	if(v>255)v=255;
	return(v);
}
#endif

void BTIC2F_DecodeTransBlockYuv420_GDbDr(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	byte *ct0, *ct1, *ct0e;
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(16*4);
			
			memset(ct0, 255, 16*4);
			memset(ct1, 255, 16*4);

			while(ct0<ct0e)
			{
//				cu=*csu++;		cv=*csv++;
				cu=csu[0];		cv=csv[0];
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				csy+=2;	csu++;	csv++;

#if 0
				cr0=cy0+cv;	cg0=cy0;	cb0=cy0+cu;
				cr1=cy2+cv;	cg1=cy2;	cb1=cy2+cu;
				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct1[0]=cr1;	ct1[1]=cg1;	ct1[2]=cb1;
				cr0=cy1+cv;	cg0=cy1;	cb0=cy1+cu;
				cr1=cy3+cv;	cg1=cy3;	cb1=cy3+cu;
				ct0[4]=cr0;	ct0[5]=cg0;	ct0[6]=cb0;
				ct1[4]=cr1;	ct1[5]=cg1;	ct1[6]=cb1;
				ct0+=8;		ct1+=8;
#endif

#if 1
				cr0=cy0+cv;	cg0=cy0;	cb0=cy0+cu;
				cr1=cy1+cv;	cg1=cy1;	cb1=cy1+cu;
				cr2=cy2+cv;	cg2=cy2;	cb2=cy2+cu;
				cr3=cy3+cv;	cg3=cy3;	cb3=cy3+cu;
				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct0[4]=cr1;	ct0[5]=cg1;	ct0[6]=cb1;
				ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2;
				ct1[4]=cr3;	ct1[5]=cg3;	ct1[6]=cb3;
				ct0+=8;		ct1+=8;
#endif
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(xstride<<4);
			
			while(ct0<ct0e)
			{
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				cu=*csu++;		cv=*csv++;		csy+=2;

				cr0=cy0+cv;	cg0=cy0;	cb0=cy0+cu;
				cr1=cy2+cv;	cg1=cy2;	cb1=cy2+cu;
				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct1[0]=cr1;	ct1[1]=cg1;	ct1[2]=cb1;
				ct0+=xstride;
				ct1+=xstride;

				cr0=cy1+cv;	cg0=cy1;	cb0=cy1+cu;
				cr1=cy3+cv;	cg1=cy3;	cb1=cy3+cu;
				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct1[0]=cr1;	ct1[1]=cg1;	ct1[2]=cb1;
				ct0+=xstride;
				ct1+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv420_RCT(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	byte *ct0, *ct1, *ct0e;
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y;
	
	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(16*4);
			
			memset(ct0, 255, 16*4);
			memset(ct1, 255, 16*4);

			while(ct0<ct0e)
			{
				cu=csu[0];		cv=csv[0];
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				csy+=2;	csu++;	csv++;

				cg=((cu+cv)>>2);
				cg0=cy0-cg;		cg1=cy1-cg;
				cg2=cy2-cg;		cg3=cy3-cg;
				cr0=cg0+cv;		cb0=cg0+cu;
				cr1=cg1+cv;		cb1=cg1+cu;
				cr2=cg2+cv;		cb2=cg2+cu;
				cr3=cg3+cv;		cb3=cg3+cu;

				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct0[4]=cr1;	ct0[5]=cg1;	ct0[6]=cb1;
				ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2;
				ct1[4]=cr3;	ct1[5]=cg3;	ct1[6]=cb3;
				ct0+=8;		ct1+=8;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(xstride<<4);
			
			while(ct0<ct0e)
			{
				cu=csu[0];		cv=csv[0];
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				csy+=2;	csu++;	csv++;

				cg=((cu+cv)>>2);
				cg0=cy0-cg;		cg1=cy1-cg;
				cg2=cy2-cg;		cg3=cy3-cg;
				cr0=cg0+cv;		cb0=cg0+cu;
				cr1=cg1+cv;		cb1=cg1+cu;
				cr2=cg2+cv;		cb2=cg2+cu;
				cr3=cg3+cv;		cb3=cg3+cu;

				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2;
				ct0+=xstride;	ct1+=xstride;
				ct0[0]=cr1;	ct0[1]=cg1;	ct0[2]=cb1;
				ct1[0]=cr3;	ct1[1]=cg3;	ct1[2]=cb3;
				ct0+=xstride;	ct1+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv420_Apx1(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	byte *ct0, *ct1, *ct0e;
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y;
	
	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(16*4);
			
			memset(ct0, 255, 16*4);
			memset(ct1, 255, 16*4);

			while(ct0<ct0e)
			{
				cu=csu[0];		cv=csv[0];
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				csy+=2;	csu++;	csv++;

				cg=((5*cv+3*cu)>>3);
				cg0=cy0-cg;		cg1=cy1-cg;
				cg2=cy2-cg;		cg3=cy3-cg;
				cr0=cy0+cv;		cb0=cy0+cu;
				cr1=cy1+cv;		cb1=cy1+cu;
				cr2=cy2+cv;		cb2=cy2+cu;
				cr3=cy3+cv;		cb3=cy3+cu;

				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct0[4]=cr1;	ct0[5]=cg1;	ct0[6]=cb1;
				ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2;
				ct1[4]=cr3;	ct1[5]=cg3;	ct1[6]=cb3;
				ct0+=8;		ct1+=8;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*2*16;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+(y*2)*ystride;
			ct1=ct0+ystride;
			ct0e=ct0+(xstride<<4);
			
			while(ct0<ct0e)
			{
				cu=csu[0];		cv=csv[0];
				cy0=csy[0];		cy1=csy[1];
				cy2=csy[16];	cy3=csy[17];
				csy+=2;	csu++;	csv++;

				cg=((5*cv+3*cu)>>3);
				cg0=cy0-cg;		cg1=cy1-cg;
				cg2=cy2-cg;		cg3=cy3-cg;
				cr0=cy0+cv;		cb0=cy0+cu;
				cr1=cy1+cv;		cb1=cy1+cu;
				cr2=cy2+cv;		cb2=cy2+cu;
				cr3=cy3+cv;		cb3=cy3+cu;

				ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
				ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2;
				ct0+=xstride;	ct1+=xstride;
				ct0[0]=cr1;	ct0[1]=cg1;	ct0[2]=cb1;
				ct1[0]=cr3;	ct1[1]=cg3;	ct1[2]=cb3;
				ct0+=xstride;	ct1+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv444_GDbDr(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	byte *ct0, *ct0e;
	int cy, cu, cv;
	int cr, cg, cb;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(8*4);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cr=cy+cv;	cg=cy;	cb=cy+cu;
				ct0[0]=cr;	ct0[1]=cg;
				ct0[2]=cb;	ct0[3]=255;
				ct0+=4;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(xstride<<3);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cr=cy+cv;	cg=cy;	cb=cy+cu;
				ct0[0]=cr;	ct0[1]=cg;	ct0[2]=cb;
				ct0+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv444_RCT(
	s16 *pxy, s16 *pxu, s16 *pxv,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	byte *ct0, *ct0e;
	int cy, cu, cv;
	int cr, cg, cb;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(8*4);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cg=cy-((cu+cv)>>2);
				cr=cg+cv;	cb=cg+cu;
				ct0[0]=cr;	ct0[1]=cg;
				ct0[2]=cb;	ct0[3]=255;
				ct0+=4;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(xstride<<3);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cg=cy-((cu+cv)>>2);
				cr=cg+cv;	cb=cg+cu;
				ct0[0]=cr;	ct0[1]=cg;	ct0[2]=cb;
				ct0+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv444A_GDbDr(
	s16 *pxy, s16 *pxu, s16 *pxv, s16 *pxa,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv, *csa;
	byte *ct0, *ct0e;
	int cy, cu, cv, ca;
	int cr, cg, cb;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;
			csa=pxa+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(8*4);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		ca=csa[0];
				cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;	csa++;
				cr=cy+cv;	cg=cy;	cb=cy+cu;
				ct0[0]=cr;	ct0[1]=cg;
				ct0[2]=cb;	ct0[3]=ca;
				ct0+=4;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(xstride<<3);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cr=cy+cv;	cg=cy;	cb=cy+cu;
				ct0[0]=cr;	ct0[1]=cg;	ct0[2]=cb;
				ct0+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlockYuv444A_RCT(
	s16 *pxy, s16 *pxu, s16 *pxv, s16 *pxa,
	byte *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv, *csa;
	byte *ct0, *ct0e;
	int cy, cu, cv, ca;
	int cr, cg, cb;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	if(xstride==4)
	{
		cr=0; cb=0;

		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;
			csa=pxa+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(8*4);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		ca=csa[0];
				cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;	csa++;
				cg=cy-((cu+cv)>>2);
				cr=cg+cv;	cb=cg+cu;
				ct0[0]=cr;	ct0[1]=cg;
				ct0[2]=cb;	ct0[3]=ca;
				ct0+=4;
			}
		}
	}else
	{
		for(y=0; y<8; y++)
		{
			csy=pxy+y*8;
			csu=pxu+y*8;
			csv=pxv+y*8;

			ct0=iptr+y*ystride;
			ct0e=ct0+(xstride<<3);
			
			while(ct0<ct0e)
			{
				cy=csy[0];		cu=csu[0];		cv=csv[0];
				csy++;	csu++;	csv++;
				cg=cy-((cu+cv)>>2);
				cr=cg+cv;	cb=cg+cu;
				ct0[0]=cr;	ct0[1]=cg;	ct0[2]=cb;
				ct0+=xstride;
			}
		}
	}
}

void BTIC2F_DecodeTransBlock420(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride)
{
	int b_y0[64], b_y1[64], b_y2[64], b_y3[64];
	int b_u0[64], b_v0[64];
	s16 pix_y[16*16];
	s16 pix_u[8*8];
	s16 pix_v[8*8];
	byte *qt_y, *qt_uv;
//	int *dcty, *dctu, *dctv;
	int bxs, bys;
//	int dcp_y, pa_y, pb_y, pc_y;
//	int dcp_u, pa_u, pb_u, pc_u;
//	int dcp_v, pa_v, pb_v, pc_v;
	int dcy, dcu, dcv;
	int i, j, k;
	
	bxs=ctx->bxs;
	bys=ctx->bys;

#if 0
	pa=0;	pb=0;	pc=0;
	dcty=ctx->dctab_y;
	dctu=ctx->dctab_u;
	dctv=ctx->dctab_v;

	if(x>0)
	{
		if(y>0)
		{
			i=(y-1)*bxs;
			j=i+(x-1);		k=i+(x  );
			pa_y=dcty[j];	pb_y=dcty[k];
			pa_u=dctu[j];	pb_u=dctu[k];
			pa_v=dctv[j];	pb_v=dctv[k];
		}
		i=y*bxs+(x-1);
		pc_y=dcty[i];
		pc_u=dctu[i];
		pc_v=dctv[i];
	}else if(y>0)
	{
		pc_y=dcty[(y-1)*bxs];
	}
	
	dcp_y=pb_y+pc_y-pa_y;
	dcp_u=pb_u+pc_u-pa_u;
	dcp_v=pb_v+pc_v-pa_v;
#endif

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;

	qt_y=ctx->qtab_y;
	qt_uv=ctx->qtab_uv;
	
	dcy=dcy+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
	BTIC2F_ReadBlockAc(ctx, b_y0, dcy, qt_y);
	dcy=dcy+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
	BTIC2F_ReadBlockAc(ctx, b_y2, dcy, qt_y);
	dcy=dcy+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
	BTIC2F_ReadBlockAc(ctx, b_y3, dcy, qt_y);
	dcy=dcy+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
	BTIC2F_ReadBlockAc(ctx, b_y1, dcy, qt_y);

	dcu=dcu+BTIC2F_ReadBlockDc(ctx, qt_uv[0]);
	BTIC2F_ReadBlockAc(ctx, b_u0, dcu, qt_uv);
	dcv=dcv+BTIC2F_ReadBlockDc(ctx, qt_uv[0]);
	BTIC2F_ReadBlockAc(ctx, b_v0, dcv, qt_uv);


//	memset(b_y0+1, 0, 63*sizeof(int));
//	memset(b_y3+1, 0, 63*sizeof(int));

//	memset(b_u0, 0, 64*sizeof(int));
//	memset(b_v0, 0, 64*sizeof(int));

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;

//	i=y*bxs+x;
//	dcty[i]=dcy;
//	dctu[i]=dcu;
//	dctv[i]=dcv;
	
	BTIC2F_TransIBHT8Stride(b_y0, pix_y+(0*16+0), 16);
	BTIC2F_TransIBHT8Stride(b_y1, pix_y+(0*16+8), 16);
	BTIC2F_TransIBHT8Stride(b_y2, pix_y+(8*16+0), 16);
	BTIC2F_TransIBHT8Stride(b_y3, pix_y+(8*16+8), 16);
	BTIC2F_TransIBHT8Stride(b_u0, pix_u, 8);
	BTIC2F_TransIBHT8Stride(b_v0, pix_v, 8);
	
//	BTIC2F_DecodeTransBlockYuv420(pix_y, pix_u, pix_v,
	ctx->DecodeTransBlockYuv420(pix_y, pix_u, pix_v,
		iptr, xstride, ystride);
}

void BTIC2F_DecodeTransBlock444(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride)
{
	int b_y0[64], b_u0[64], b_v0[64], b_a0[64];
	s16 pix_y[8*8];
	s16 pix_u[8*8];
	s16 pix_v[8*8];
	s16 pix_a[8*8];
	byte *qt_y, *qt_uv;
	int bxs, bys;
	int dcy, dcu, dcv, dca;
	int i, j, k;
	
	bxs=ctx->bxs;
	bys=ctx->bys;

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;
	dca=ctx->dcp_a;

	qt_y=ctx->qtab_y;
	qt_uv=ctx->qtab_uv;
	
	dcy=dcy+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
	BTIC2F_ReadBlockAc(ctx, b_y0, dcy, qt_y);
	dcu=dcu+BTIC2F_ReadBlockDc(ctx, qt_uv[0]);
	BTIC2F_ReadBlockAc(ctx, b_u0, dcu, qt_uv);
	dcv=dcv+BTIC2F_ReadBlockDc(ctx, qt_uv[0]);
	BTIC2F_ReadBlockAc(ctx, b_v0, dcv, qt_uv);

	if(ctx->has_alpha)
	{
		dca=dca+BTIC2F_ReadBlockDc(ctx, qt_y[0]);
		BTIC2F_ReadBlockAc(ctx, b_a0, dca, qt_y);
	}

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;
	ctx->dcp_a=dca;
	
	BTIC2F_TransIBHT8Stride(b_y0, pix_y, 8);
	BTIC2F_TransIBHT8Stride(b_u0, pix_u, 8);
	BTIC2F_TransIBHT8Stride(b_v0, pix_v, 8);

	if(ctx->has_alpha)
	{
		BTIC2F_TransIBHT8Stride(b_a0, pix_a, 8);
//		BTIC2F_DecodeTransBlockYuv444A(pix_y, pix_u, pix_v, pix_a,
		ctx->DecodeTransBlockYuv444A(pix_y, pix_u, pix_v, pix_a,
			iptr, xstride, ystride);
	}else
	{
//		BTIC2F_DecodeTransBlockYuv444(pix_y, pix_u, pix_v,
		ctx->DecodeTransBlockYuv444(pix_y, pix_u, pix_v,
			iptr, xstride, ystride);
	}
}

void BTIC2F_DecodeTransBlockClip420(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip)
{
	byte pix[16*16*4];
	int px, py;
	BTIC2F_DecodeTransBlock420(ctx, x, y, pix, xstride, 16*4);
	for(py=0; py<yclip; py++)
		{ memcpy(iptr+py*ystride, pix+py*(16*4), xclip*xstride); }
}

void BTIC2F_DecodeTransBlockClip444(
	BTIC2F_DecState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip)
{
	byte pix[8*8*4];
	int px, py;
	BTIC2F_DecodeTransBlock444(ctx, x, y, pix, xstride, 8*4);
	for(py=0; py<yclip; py++)
		{ memcpy(iptr+py*ystride, pix+py*(8*4), xclip*xstride); }
}

void BTIC2F_DecodeTransImage(
	BTIC2F_DecState *ctx,
	byte *img, int xstride, int ystride)
{
	int xs, ys, bxs0, bys0, bsz;
	int x, y;

	ctx->dcp_y=0;
	ctx->dcp_u=0;
	ctx->dcp_v=0;
	ctx->dcp_a=0;
	
	if(ctx->clrs==0)
	{
		ctx->DecodeTransBlockYuv420=BTIC2F_DecodeTransBlockYuv420_GDbDr;
		ctx->DecodeTransBlockYuv444=BTIC2F_DecodeTransBlockYuv444_GDbDr;
		ctx->DecodeTransBlockYuv444A=BTIC2F_DecodeTransBlockYuv444A_GDbDr;
	}else
		if(ctx->clrs==1)
	{
		ctx->DecodeTransBlockYuv420=BTIC2F_DecodeTransBlockYuv420_RCT;
		ctx->DecodeTransBlockYuv444=BTIC2F_DecodeTransBlockYuv444_RCT;
		ctx->DecodeTransBlockYuv444A=BTIC2F_DecodeTransBlockYuv444A_RCT;
	}else
		if(ctx->clrs==2)
	{
		ctx->DecodeTransBlockYuv420=BTIC2F_DecodeTransBlockYuv420_Apx1;
		ctx->DecodeTransBlockYuv444=BTIC2F_DecodeTransBlockYuv444_RCT;
		ctx->DecodeTransBlockYuv444A=BTIC2F_DecodeTransBlockYuv444A_RCT;
	}

	xs=ctx->xs;
	ys=ctx->ys;
//	bxs0=(xs+0)>>4;
	bxs0=(xs+0)>>(ctx->mbshr);
//	bys0=(ys+0)>>4;
	bys0=(ys+0)>>(ctx->mbshr);
	bsz=1<<ctx->mbshr;
	
	for(y=0; y<bys0; y++)
	{
		for(x=0; x<bxs0; x++)
		{
//			BTIC2F_DecodeTransBlock420(ctx, x, y,
			ctx->DecodeTransBlock(ctx, x, y,
//				img+(y*16*ystride)+(x*16*xstride),
				img+(y*bsz*ystride)+(x*bsz*xstride),
				xstride, ystride);
		}
//		if(xs&15)
		if(xs&(bsz-1))
		{
//			BTIC2F_DecodeTransBlockClip420(ctx, x, y,
			ctx->DecodeTransBlockClip(ctx, x, y,
//				img+(y*16*ystride)+(x*16*xstride),
				img+(y*bsz*ystride)+(x*bsz*xstride),
//				xstride, ystride, xs&15, 16);
				xstride, ystride, xs&(bsz-1), bsz);
		}
	}

	if(ys&15)
	{
		for(x=0; x<bxs0; x++)
		{
//			BTIC2F_DecodeTransBlockClip420(ctx, x, y,
			ctx->DecodeTransBlockClip(ctx, x, y,
//				img+(y*16*ystride)+(x*16*xstride),
				img+(y*bsz*ystride)+(x*bsz*xstride),
//				xstride, ystride, 16, ys&15);
				xstride, ystride, bsz, ys&(bsz-1));
		}
		if(xs&15)
		{
//			BTIC2F_DecodeTransBlockClip420(ctx, x, y,
			ctx->DecodeTransBlockClip(ctx, x, y,
//				img+(y*16*ystride)+(x*16*xstride),
				img+(y*bsz*ystride)+(x*bsz*xstride),
//				xstride, ystride, xs&15, ys&15);
				xstride, ystride, xs&(bsz-1), ys&(bsz-1));
		}
	}
}

int BTIC2F_DecodeImageBuffer(
	BTIC2F_DecState *ctx,
	byte *cdat, int csz,
	byte *img, int xstride, int ystride)
{
	byte *cs, *cs0, *cs1, *cs2, *cse;
	byte *fdat;
	u32 fcc;
	int sz, dsz, fsz, fl;
	int i, j, k, n;
	
	if(!csz)
		return(0);
	
	fdat=NULL;
	cs=cdat; cse=cdat+csz;
	while(cs<cse)
	{
		i=cs[0];
		if((i>>5)==0)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btic2f_getu16(cs+2);
			cs0=cs+4;	cs1=cs+sz;
		}else if((i>>5)==1)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btic2f_getu32(cs+2);
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==2)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btic2f_getu16(cs+4);
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==3)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btic2f_getu32(cs+4);
			cs0=cs+8;	cs1=cs+sz;
		}else if((i>>5)==4)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btic2f_getu16(cs+3);
			cs0=cs+5;	cs1=cs+sz;
		}else if((i>>5)==5)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btic2f_getu32(cs+3);
			cs0=cs+7;	cs1=cs+sz;
		}
		
		if(fcc==BTIC2F_TCC_HX)
		{
			ctx->xs=btic2f_getu16(cs0+0);
			ctx->ys=btic2f_getu16(cs0+2);
			ctx->fl=btic2f_getu16(cs0+4);
			ctx->bxs=(ctx->xs+15)>>4;
			ctx->bys=(ctx->ys+15)>>4;

			ctx->clrs=cs0[6];
			ctx->mbty=cs0[7];

			switch(ctx->mbty)
			{
			case 0:
				ctx->bxs=(ctx->xs+15)>>4;
				ctx->bys=(ctx->ys+15)>>4;
				ctx->mbshr=4;
				ctx->DecodeTransBlock=BTIC2F_DecodeTransBlock420;
				ctx->DecodeTransBlockClip=BTIC2F_DecodeTransBlockClip420;
				break;
			case 1:
			case 2:
				ctx->bxs=(ctx->xs+7)>>3;
				ctx->bys=(ctx->ys+7)>>3;
				ctx->mbshr=3;
				ctx->DecodeTransBlock=BTIC2F_DecodeTransBlock444;
				ctx->DecodeTransBlockClip=BTIC2F_DecodeTransBlockClip444;
				if(ctx->mbty==2)
					ctx->has_alpha=1;
				break;
			}

			cs=cs1;
			continue;
		}

		if(fcc==BTIC2F_TCC_HT)
		{
			BTIC2F_SetupReadBits(ctx, cs0, cs1-cs0);
			while(1)
			{
				i=BTIC2F_ReadBits(ctx, 4);
				if(!i)break;
				BTIC2F_DecodeHuffTable(ctx, i);
			}
			cs=cs1;
			continue;
		}

		if(fcc==BTIC2F_TCC_QT)
		{
			cs2=cs0;
			while(cs2<cs1)
			{
				i=*cs2++;
				if(!i)break;
				if(i==1)
					{ memcpy(ctx->qtab_y, cs2, 64); cs2+=64; }
				else if(i==2)
					{ memcpy(ctx->qtab_uv, cs2, 64); cs2+=64; }
				else
					break;
			}
			cs=cs1;
			continue;
		}

		if(	(fcc==BTIC2F_TCC_IX) ||
			(fcc==BTIC2F_TCC_PX)	)
		{
			fdat=cs0; fsz=cs1-cs0;
			cs=cs1;
			continue;
		}

		cs=cs1;
		continue;
	}
	
	if(fdat)
	{
		BTIC2F_SetupReadBits(ctx, fdat, fsz);
		BTIC2F_DecodeTransImage(ctx,
			img, xstride, ystride);
	}
	
	return(0);
}

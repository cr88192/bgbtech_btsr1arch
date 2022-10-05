#define TKULZ_STATUS_NONE	0
#define TKULZ_STATUS_END	1
#define TKULZ_STATUS_BADTAG	2
#define TKULZ_STATUS_BADESC	3
#define TKULZ_STATUS_BADHT	4

#ifndef TKULZ_BYTE
#define TKULZ_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

// #define TKULZ_LEN13

#ifdef TKULZ_LEN13
#define TKULZ_HTABSZ 8192
#define TKULZ_HTABNB 13
#else
#define TKULZ_HTABSZ 4096
#define TKULZ_HTABNB 12
#endif

typedef struct TKuLZ_DecState_s TKuLZ_DecState;

struct TKuLZ_DecState_s {
	byte *cs;
	u32 win;
	byte pos;
	byte status;

	byte *ct;

	u16 htab_t[TKULZ_HTABSZ];
	u16 htab_l[TKULZ_HTABSZ];
	u16 htab_d[TKULZ_HTABSZ];
	byte hparm_t;
	byte hparm_l;
	byte hparm_d;

	int (*DecodeTagSym)(TKuLZ_DecState *ctx);
	int (*DecodeLitSym)(TKuLZ_DecState *ctx);
	int (*DecodeDistSym)(TKuLZ_DecState *ctx);
	u32 (*DecodeLitSym4x)(TKuLZ_DecState *ctx);
	u64 (*DecodeLitSym8x)(TKuLZ_DecState *ctx);
};

int TKuLZ_PeekBits(TKuLZ_DecState *ctx, int bits)
	{ return((ctx->win>>ctx->pos)&((1<<bits)-1)); }

#ifdef HAVE_MISAL
void TKuLZ_SkipBits(TKuLZ_DecState *ctx, int bits)
{
	byte *cs;
	int k;

	k=ctx->pos+bits;
	ctx->pos=k&7;
	cs=(ctx->cs)+(k>>3);
	ctx->win=get_u32le(cs);
	ctx->cs=cs;
}

int TKuLZ_ReadBits(TKuLZ_DecState *ctx, int bits)
{
	byte *cs;
	int p, b, k;

	p=ctx->pos;
	b=(ctx->win>>p)&((1<<bits)-1);
	k=p+bits;
	ctx->pos=k&7;
	cs=(ctx->cs)+(k>>3);
	ctx->win=get_u32le(cs);
	ctx->cs=cs;

	return(b);
}

int TKuLZ_Read8Bits(TKuLZ_DecState *ctx)
{
//	byte *cs;
	int b;
	b=(ctx->win>>ctx->pos)&255;
	ctx->cs++;
	ctx->win=get_u32le(ctx->cs);
//	cs=(ctx->cs)+1;
//	ctx->win=get_u32le(cs);
//	ctx->cs=cs;
	return(b);
}

int TKuLZ_Read7Bits(TKuLZ_DecState *ctx)
{
	return(TKuLZ_ReadBits(ctx, 7));
}

int TKuLZ_Read16Bits(TKuLZ_DecState *ctx)
{
	int b;
	b=(ctx->win>>ctx->pos)&65535;
	ctx->cs+=2;
	ctx->win=get_u32le(ctx->cs);
	return(b);
}

u32 TKuLZ_Read32Bits(TKuLZ_DecState *ctx)
{
	u32 b, b0, b1;
	b0=TKuLZ_Read16Bits(ctx);
	b1=TKuLZ_Read16Bits(ctx);
	b=b0|(b1<<16);
	return(b);
}

#else

void TKuLZ_SkipBits(TKuLZ_DecState *ctx, int bits)
{
	ctx->pos+=bits;
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
}

int TKuLZ_ReadBits(TKuLZ_DecState *ctx, int bits)
{
	int b;
	b=(ctx->win>>ctx->pos)&((1<<bits)-1);
	ctx->pos+=bits;
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
//	b=TKuLZ_PeekBits(ctx, bits);
//	TKuLZ_SkipBits(ctx, bits);
	return(b);
}

int TKuLZ_Read8Bits(TKuLZ_DecState *ctx)
{
	int b;
	b=(ctx->win>>ctx->pos)&255;
	ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24);
	return(b);
}

int TKuLZ_Read7Bits(TKuLZ_DecState *ctx)
{
	int b;
	b=(ctx->win>>ctx->pos)&127;
	ctx->pos+=7;
	if(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
	return(b);
}

int TKuLZ_Read16Bits(TKuLZ_DecState *ctx)
{
	int b;
	b=(ctx->win>>ctx->pos)&65535;
	ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24);
	ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24);
	return(b);
}

u32 TKuLZ_Read32Bits(TKuLZ_DecState *ctx)
{
	u32 b, b0, b1;
	b0=TKuLZ_Read16Bits(ctx);
	b1=TKuLZ_Read16Bits(ctx);
	b=b0|(b1<<16);
	return(b);
}
#endif

u64 TKuLZ_Read64Bits(TKuLZ_DecState *ctx)
{
	u64 b, b0, b1;
	b0=TKuLZ_Read32Bits(ctx);
	b1=TKuLZ_Read32Bits(ctx);
	b=b0|(b1<<32);
	return(b);
}

u32 TKuLZ_Read7x4Bits(TKuLZ_DecState *ctx)
{
	u32 b, b0, b1;
	b0=TKuLZ_ReadBits(ctx, 14);
	b1=TKuLZ_ReadBits(ctx, 14);
	b0=(b0&127)|((b0<<1)&0x7F00);
	b1=(b1&127)|((b1<<1)&0x7F00);
	b=b0|(b1<<16);
	return(b);
}

u64 TKuLZ_Read7x8Bits(TKuLZ_DecState *ctx)
{
	u64 b, b0, b1;
	b0=TKuLZ_Read7x4Bits(ctx);
	b1=TKuLZ_Read7x4Bits(ctx);
	b=b0|(b1<<32);
	return(b);
}

int TKuLZ_ReadPackedLengths(TKuLZ_DecState *ctx, byte *cls)
{
	byte *t, *te;
	int i, j, c, nz, zc, lc;
	
	t=cls; te=cls+256; lc=0;
	while(t<te)
	{
		c=TKuLZ_ReadBits(ctx, 4); zc=0; nz=0;
		if(c<14)
			{ *t++=c; lc=c; }
		else if(c==14)
			{ nz=TKuLZ_ReadBits(ctx, 4)+3; }
		else if(c==15)
		{
			j=TKuLZ_ReadBits(ctx, 2);
			if(j==0)
				{ nz=TKuLZ_ReadBits(ctx, 6)+19; }
			else if(j==1)
			{
				nz=TKuLZ_ReadBits(ctx, 6)+3; zc=lc;
				if(nz==3)
					{ nz=te-t; zc=0; }
			}
			else
				{ ctx->status=TKULZ_STATUS_BADHT; }
		}
		while(nz>0)
			{ *t++=zc; nz--; }
	}
	
	if(t>te)
		{ ctx->status=TKULZ_STATUS_BADHT; }
	return(0);
}

static const byte tkulz_trans4[16]={
	0x0,0x8,0x4,0xC, 0x2,0xA,0x6,0xE,
	0x1,0x9,0x5,0xD, 0x3,0xB,0x7,0xF};

static const byte tkulz_trans8[256]={
	0x00,0x80,0x40,0xC0, 0x20,0xA0,0x60,0xE0,
	0x10,0x90,0x50,0xD0, 0x30,0xB0,0x70,0xF0,
	0x08,0x88,0x48,0xC8, 0x28,0xA8,0x68,0xE8,
	0x18,0x98,0x58,0xD8, 0x38,0xB8,0x78,0xF8,
	0x04,0x84,0x44,0xC4, 0x24,0xA4,0x64,0xE4,
	0x14,0x94,0x54,0xD4, 0x34,0xB4,0x74,0xF4,
	0x0C,0x8C,0x4C,0xCC, 0x2C,0xAC,0x6C,0xEC,
	0x1C,0x9C,0x5C,0xDC, 0x3C,0xBC,0x7C,0xFC,

	0x02,0x82,0x42,0xC2, 0x22,0xA2,0x62,0xE2,
	0x12,0x92,0x52,0xD2, 0x32,0xB2,0x72,0xF2,
	0x0A,0x8A,0x4A,0xCA, 0x2A,0xAA,0x6A,0xEA,
	0x1A,0x9A,0x5A,0xDA, 0x3A,0xBA,0x7A,0xFA,
	0x06,0x86,0x46,0xC6, 0x26,0xA6,0x66,0xE6,
	0x16,0x96,0x56,0xD6, 0x36,0xB6,0x76,0xF6,
	0x0E,0x8E,0x4E,0xCE, 0x2E,0xAE,0x6E,0xEE,
	0x1E,0x9E,0x5E,0xDE, 0x3E,0xBE,0x7E,0xFE,

	0x01,0x81,0x41,0xC1, 0x21,0xA1,0x61,0xE1,
	0x11,0x91,0x51,0xD1, 0x31,0xB1,0x71,0xF1,
	0x09,0x89,0x49,0xC9, 0x29,0xA9,0x69,0xE9,
	0x19,0x99,0x59,0xD9, 0x39,0xB9,0x79,0xF9,
	0x05,0x85,0x45,0xC5, 0x25,0xA5,0x65,0xE5,
	0x15,0x95,0x55,0xD5, 0x35,0xB5,0x75,0xF5,
	0x0D,0x8D,0x4D,0xCD, 0x2D,0xAD,0x6D,0xED,
	0x1D,0x9D,0x5D,0xDD, 0x3D,0xBD,0x7D,0xFD,

	0x03,0x83,0x43,0xC3, 0x23,0xA3,0x63,0xE3,
	0x13,0x93,0x53,0xD3, 0x33,0xB3,0x73,0xF3,
	0x0B,0x8B,0x4B,0xCB, 0x2B,0xAB,0x6B,0xEB,
	0x1B,0x9B,0x5B,0xDB, 0x3B,0xBB,0x7B,0xFB,
	0x07,0x87,0x47,0xC7, 0x27,0xA7,0x67,0xE7,
	0x17,0x97,0x57,0xD7, 0x37,0xB7,0x77,0xF7,
	0x0F,0x8F,0x4F,0xCF, 0x2F,0xAF,0x6F,0xEF,
	0x1F,0x9F,0x5F,0xDF, 0x3F,0xBF,0x7F,0xFF
	};

byte TKuLZ_TransposeByte(byte v)
{
	return((tkulz_trans4[v&15]<<4)|tkulz_trans4[(v>>4)&15]);
}

u16 TKuLZ_TransposeWord(u16 v)
{
	int nv;
	nv=(tkulz_trans8[v&255]<<8)|tkulz_trans8[(v>>8)&255];
	return(nv);

#if 0
	int n0, n1, n2, n3, nv, nv1;
	n0=tkulz_trans4[(v    )&15];	n1=tkulz_trans4[(v>> 4)&15];
	n2=tkulz_trans4[(v>> 8)&15];	n3=tkulz_trans4[(v>>12)&15];
	nv=(n0<<12)|(n1<<8)|(n2<<4)|n3;
	nv1=(tkulz_trans8[v&255]<<8)|tkulz_trans8[(v>>8)&255];
	if(nv1!=nv)
	{
		printf("%04X %04X %04X\n", v, nv, nv1);
	}
	return(nv);
//	return((TKuLZ_TransposeByte(v&255)<<8)|TKuLZ_TransposeByte(v>>8));
#endif
}

void TKuLZ_SetupTableLengths(TKuLZ_DecState *ctx, u16 *htab, byte *cls)
{
	int c, l, tc, ntc;
	int i, j, k;

//	for(i=0; i<TKULZ_HTABSZ; i++)
//		htab[i]=0;
	
	c=0;
	for(l=1; l<(TKULZ_HTABNB+1); l++)
	{
		for(i=0; i<256; i++)
			if(cls[i]==l)
		{
			ntc=1<<(TKULZ_HTABNB-l);
			tc=c<<(TKULZ_HTABNB-l);
			for(j=0; j<ntc; j++)
			{
				k=TKuLZ_TransposeWord(tc+j)>>(16-TKULZ_HTABNB);
				htab[k]=(l<<8)|i;
			}
			c++;
		}
		c=c<<1;
	}
}

int TKuLZ_DecodeHuffSym(TKuLZ_DecState *ctx, u16 *htab)
{
	int b, c;
	b=(ctx->win>>ctx->pos)&(TKULZ_HTABSZ-1);
	c=htab[b];
//	if(!((c>>8)&15))
//		{ __debugbreak(); }
	TKuLZ_SkipBits(ctx, (c>>8)&15);
	return(c&255);
}

int TKuLZ_DecodeHuffSym2x(TKuLZ_DecState *ctx, u16 *htab)
{
#ifdef TKULZ_LEN13
	u32 s0, s1, c;
	s0=TKuLZ_DecodeHuffSym(ctx, htab);
	s1=TKuLZ_DecodeHuffSym(ctx, htab);
	c=s0|(s1<<8);
	return(c);
#else
	u32 b0, b1, c, c0, c1, l0, l1;
	b0=ctx->win>>ctx->pos;
	c0=htab[b0&(TKULZ_HTABSZ-1)];
	l0=(c0>>8)&15;
	b1=b0>>l0;
	c1=htab[b1&(TKULZ_HTABSZ-1)];
	l1=(c1>>8)&15;
	TKuLZ_SkipBits(ctx, l0+l1);
	c=(c0&255)|((c1&255)<<8);
	return(c);
#endif
}

u32 TKuLZ_DecodeHuffSym4x(TKuLZ_DecState *ctx, u16 *htab)
{

#ifdef HAVE_MISAL_64B
	u64 win, b0, b1, b2, b3;
	u32 c, c0, c1, c2, c3, l0, l1, l2, l3;
	win=get_u64le(ctx->cs);
	b0=win>>ctx->pos;
	c0=htab[b0&(TKULZ_HTABSZ-1)];
	l0=(c0>>8)&15;

	b1=b0>>l0;
	c1=htab[b1&(TKULZ_HTABSZ-1)];
	l1=(c1>>8)&15;

	b2=b1>>l1;
	c2=htab[b2&(TKULZ_HTABSZ-1)];
	l2=(c2>>8)&15;

	b3=b2>>l2;
	c3=htab[b3&(TKULZ_HTABSZ-1)];
	l3=(c3>>8)&15;
	
	TKuLZ_SkipBits(ctx, l0+l1+l2+l3);
	c=(c0&255)|((c1&255)<<8)|((c2&255)<<16)|((c3&255)<<24);
	return(c);
#endif

#ifndef HAVE_MISAL_64B

#if 1
	u32 s0, s1, c;
	s0=TKuLZ_DecodeHuffSym2x(ctx, htab);
	s1=TKuLZ_DecodeHuffSym2x(ctx, htab);
	c=s0|(s1<<16);
	return(c);
#endif

#if 0
	u32 s0, s1, s2, s3, c;
	s0=TKuLZ_DecodeHuffSym(ctx, htab);
	s1=TKuLZ_DecodeHuffSym(ctx, htab);
	s2=TKuLZ_DecodeHuffSym(ctx, htab);
	s3=TKuLZ_DecodeHuffSym(ctx, htab);
	c=s0|(s1<<8)|(s2<<16)|(s3<<24);
	return(c);
#endif

#endif
}

u64 TKuLZ_DecodeHuffSym8x(TKuLZ_DecState *ctx, u16 *htab)
{
	u64 s0, s1, c;
	s0=TKuLZ_DecodeHuffSym4x(ctx, htab);
	s1=TKuLZ_DecodeHuffSym4x(ctx, htab);
	c=s0|(s1<<32);
	return(c);
}

int TKuLZ_DecodeRiceSym(TKuLZ_DecState *ctx, byte rck)
{
	static const byte qtab[16]={
		0, 1, 0, 2,  0, 1, 0, 3,  0, 1, 0, 2,  0, 1, 0, 4 };
	int b, c, q, st;

	b=ctx->win>>ctx->pos;
	q=qtab[b&15];
	c=(q<<rck)|((b>>(q+1))&((1<<rck)-1));
	st=q+rck+1;
	if(q==4)
		{ c=(b>>4); st=12; }
	TKuLZ_SkipBits(ctx, st);
	return(c&255);
}

int TKuLZ_DecodeRice2Sym(TKuLZ_DecState *ctx, byte rck)
{
	static const byte qtab[16]={
		0, 1, 0, 2,  0, 1, 0, 3,  0, 1, 0, 2,  0, 1, 0, 4 };
	int b0, b1, c0, c1, c, q0, q1, k0, k1, l0, l1;

	k0=(rck   )&3;
	k1=(rck>>2)&3;
	b0=ctx->win>>ctx->pos;
	q0=qtab[b0&15];
	l0=q0+k0+1;
	b1=b0>>l0;
	q1=qtab[b1&15];
	c0=(q0<<k0)|((b0>>(q0+1))&((1<<k0)-1));
	c1=(q1<<k1)|((b1>>(q1+1))&((1<<k1)-1));
	l1=q1+k1+1;
	c=(c1<<4)|c0;
	TKuLZ_SkipBits(ctx, l0+l1);
	return(c&255);

#if 0
	int ch, cl, c;
	cl=TKuLZ_DecodeRiceSym(ctx, (rck>>0)&3);
	ch=TKuLZ_DecodeRiceSym(ctx, (rck>>2)&3);
	c=((ch&15)<<4)|(cl&15);
	return(c);
#endif
}

u32 TKuLZ_DecodeRiceSym4x(TKuLZ_DecState *ctx, int rck)
{
	u32 s0, s1, s2, s3, c;
	s0=TKuLZ_DecodeRiceSym(ctx, rck);
	s1=TKuLZ_DecodeRiceSym(ctx, rck);
	s2=TKuLZ_DecodeRiceSym(ctx, rck);
	s3=TKuLZ_DecodeRiceSym(ctx, rck);
	c=s0|(s1<<8)|(s2<<16)|(s3<<24);
	return(c);
}

u32 TKuLZ_DecodeRice2Sym4x(TKuLZ_DecState *ctx, int rck)
{
	u32 s0, s1, s2, s3, c;
	s0=TKuLZ_DecodeRice2Sym(ctx, rck);
	s1=TKuLZ_DecodeRice2Sym(ctx, rck);
	s2=TKuLZ_DecodeRice2Sym(ctx, rck);
	s3=TKuLZ_DecodeRice2Sym(ctx, rck);
	c=s0|(s1<<8)|(s2<<16)|(s3<<24);
	return(c);
}

u32 TKuLZ_ReadBits4x(TKuLZ_DecState *ctx, int rck)
{
	u32 s0, s1, s2, s3, c;
	s0=TKuLZ_ReadBits(ctx, rck);
	s1=TKuLZ_ReadBits(ctx, rck);
	s2=TKuLZ_ReadBits(ctx, rck);
	s3=TKuLZ_ReadBits(ctx, rck);
	c=s0|(s1<<8)|(s2<<16)|(s3<<24);
	return(c);
}

u64 TKuLZ_ReadBits8x(TKuLZ_DecState *ctx, int rck)
{
	u64 c;
	u32 s0, s1;
	s0=TKuLZ_ReadBits4x(ctx, rck);
	s1=TKuLZ_ReadBits4x(ctx, rck);
	c=s0|(((u64)s1)<<32);
	return(c);
}

int TKuLZ_DecodeTagSymHuff(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_t)); }
int TKuLZ_DecodeLitSymHuff(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_l)); }
int TKuLZ_DecodeDistSymHuff(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_d)); }
u32 TKuLZ_DecodeLitSym4xHuff(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym4x(ctx, ctx->htab_l)); }
u64 TKuLZ_DecodeLitSym8xHuff(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym8x(ctx, ctx->htab_l)); }

int TKuLZ_DecodeTagSymFixed(TKuLZ_DecState *ctx)
	{ return(TKuLZ_ReadBits(ctx, ctx->hparm_t)); }
int TKuLZ_DecodeLitSymFixed(TKuLZ_DecState *ctx)
	{ return(TKuLZ_ReadBits(ctx, ctx->hparm_l)); }
int TKuLZ_DecodeDistSymFixed(TKuLZ_DecState *ctx)
	{ return(TKuLZ_ReadBits(ctx, ctx->hparm_d)); }
u32 TKuLZ_DecodeLitSym4xFixed(TKuLZ_DecState *ctx)
	{ return(TKuLZ_ReadBits4x(ctx, ctx->hparm_l)); }
u64 TKuLZ_DecodeLitSym8xFixed(TKuLZ_DecState *ctx)
	{ return(TKuLZ_ReadBits8x(ctx, ctx->hparm_l)); }

int TKuLZ_DecodeTagSymRice(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRiceSym(ctx, ctx->hparm_t)); }
int TKuLZ_DecodeLitSymRice(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRiceSym(ctx, ctx->hparm_l)); }
int TKuLZ_DecodeDistSymRice(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRiceSym(ctx, ctx->hparm_d)); }
u32 TKuLZ_DecodeLitSym4xRice(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRiceSym4x(ctx, ctx->hparm_l)); }

int TKuLZ_DecodeTagSymRice2(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRice2Sym(ctx, ctx->hparm_t)); }
int TKuLZ_DecodeLitSymRice2(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRice2Sym(ctx, ctx->hparm_l)); }
int TKuLZ_DecodeDistSymRice2(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRice2Sym(ctx, ctx->hparm_d)); }
u32 TKuLZ_DecodeLitSym4xRice2(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeRice2Sym4x(ctx, ctx->hparm_l)); }

int TKuLZ_DecodeTagSym(TKuLZ_DecState *ctx)
	{ return(ctx->DecodeTagSym(ctx)); }
int TKuLZ_DecodeLitSym(TKuLZ_DecState *ctx)
	{ return(ctx->DecodeLitSym(ctx)); }
int TKuLZ_DecodeDistSym(TKuLZ_DecState *ctx)
	{ return(ctx->DecodeDistSym(ctx)); }
u32 TKuLZ_DecodeLitSym4x(TKuLZ_DecState *ctx)
	{ return(ctx->DecodeLitSym4x(ctx)); }
u64 TKuLZ_DecodeLitSym8x(TKuLZ_DecState *ctx)
	{ return(ctx->DecodeLitSym8x(ctx)); }

u64 TKuLZ_DecodeLitSym8xPair(TKuLZ_DecState *ctx)
{
	u64 c;
	u32 c0, c1;
	c0=TKuLZ_DecodeLitSym4x(ctx);
	c1=TKuLZ_DecodeLitSym4x(ctx);
	c=(((u64)c1)<<32)|c0;
	return(c);
}

int TKuLZ_DecodeDistance(TKuLZ_DecState *ctx)
{
	int pfx, eb, d;
	pfx=TKuLZ_DecodeDistSym(ctx);
	eb=pfx>>3; d=pfx&7;
	if(eb) { eb--; d=((8+d)<<eb)|TKuLZ_ReadBits(ctx, eb); }
	return(d);
}

// #ifdef TKELZ_HASH_SZ
#if 0
void TKuLZ_CopyMatch(TKuLZ_DecState *ctx, int len, int dist)
{
	byte *cs, *ct, *cte;
	int i, step;
	
	ct=ctx->ct; cs=ct-dist; cte=ct+len;
	TgvLz_MatchCopy2(ct, len, dist);
	ctx->ct=cte;
}
#endif

// #ifndef TKELZ_HASH_SZ
#if 1
void TKuLZ_CopyMatch(TKuLZ_DecState *ctx, int len, int dist)
{
	byte *cs, *ct, *cte;
	int i, step;
	
	ct=ctx->ct; cs=ct-dist; cte=ct+len;
	if(dist>=16)
	{
		if(len>16)
		{
			while(ct<cte)
				{ memcpy(ct, cs, 16); ct+=16; cs+=16; }
		}else
		{
			memcpy(ct, cs, 16);
		}
	}else
	{
		if(dist==1)
//		if(0)
		{
			i=cs[0];
			if(len>16)
			{
				ct[ 0]=i; ct[ 1]=i; ct[ 2]=i; ct[ 3]=i;
				ct[ 4]=i; ct[ 5]=i; ct[ 6]=i; ct[ 7]=i;
				ct[ 8]=i; ct[ 9]=i; ct[10]=i; ct[11]=i;
				ct[12]=i; ct[13]=i; ct[14]=i; ct[15]=i;
				while(ct<cte)
					{ memcpy(ct, cs, 16); ct+=16; }
			}else
			{
				while(ct<cte)
					{ *ct++=i; }
			}
//			memset(ct, cs[0], len);
		}else
		{
			while(ct<cte)
				{ *ct++=*cs++; }
		}
	}
	
	ctx->ct=cte;
}
#endif

void TKuLZ_DecodeLiterals(TKuLZ_DecState *ctx, int len)
{
	u64 lv;
	u32 v;
	int i;
	i=len;

#ifdef HAVE_MISAL

#ifdef HAVE_MISAL_64B
	while(i>=8)
	{
		lv=TKuLZ_DecodeLitSym8x(ctx);
		set_u64le(ctx->ct, lv);
		ctx->ct+=8;
		i-=8;
	}

	if(i>=4)
	{
		v=TKuLZ_DecodeLitSym4x(ctx);
		set_u32le(ctx->ct, v);
		ctx->ct+=4;
		i-=4;
	}
#else
	while(i>=4)
	{
		v=TKuLZ_DecodeLitSym4x(ctx);
		set_u32le(ctx->ct, v);
		ctx->ct+=4;
		i-=4;
	}
#endif

#endif
	
	while(i--)
		{ *ctx->ct++=TKuLZ_DecodeLitSym(ctx); }
}

void TKuLZ_DecodeRuns(TKuLZ_DecState *ctx)
{
	int tg, ltg, lll;
	int ll, ml, md, lml, lmd;
	int i, j, k;

	lml=0; lmd=0;
	while(1)
	{
		tg=TKuLZ_DecodeTagSym(ctx);
		ll=tg>>4; ml=(tg&15)+3;
		if(ll==15)
			{ ll=TKuLZ_DecodeDistance(ctx)+15; }
		if(ml==18)
			{ ml=TKuLZ_DecodeDistance(ctx)+18; }
		md=TKuLZ_DecodeDistance(ctx);
		if(!md)
		{
			if(ml==3)
				{ }
			else if(ml==4)
				{ ml=lml; md=lmd; }
			else if(ml==5)
				{ ml=lml; md=TKuLZ_DecodeDistance(ctx); }
			else if(ml==6)
				{ ml=TKuLZ_DecodeDistance(ctx)+3; md=lmd; }
			else
				{ ctx->status=TKULZ_STATUS_BADESC; break; }
		}
		ltg=tg; lll=ll;
		lml=ml;	lmd=md;
		if(ll)
			{ TKuLZ_DecodeLiterals(ctx, ll); }
		if(!md)
			break;
		TKuLZ_CopyMatch(ctx, ml, md);
	}
}

void TKuLZ_DecodeLzBlock(TKuLZ_DecState *ctx)
	{ TKuLZ_DecodeRuns(ctx); }

void TKuLZ_DecodeRawBlock(TKuLZ_DecState *ctx)
{	
	int i, tgl;
	tgl=TKuLZ_ReadBits(ctx, 2);
	i=TKuLZ_ReadBits(ctx, 12+(tgl*4))+1;
	ctx->pos=(ctx->pos+7)&(~7);
	while(i--)
		{ *ctx->ct++=TKuLZ_ReadBits(ctx, 8); }
}

void TKuLZ_DecodeTable(TKuLZ_DecState *ctx, int tn)
{
	byte cl[256];
	TKuLZ_ReadPackedLengths(ctx, cl);
	if(tn==0)
	{	TKuLZ_SetupTableLengths(ctx, ctx->htab_t, cl); 
		ctx->DecodeTagSym=TKuLZ_DecodeTagSymHuff;	}
	else if(tn==1)
	{	TKuLZ_SetupTableLengths(ctx, ctx->htab_l, cl);
		ctx->DecodeLitSym=TKuLZ_DecodeLitSymHuff;
		ctx->DecodeLitSym4x=TKuLZ_DecodeLitSym4xHuff;
		ctx->DecodeLitSym8x=TKuLZ_DecodeLitSym8xHuff;
	}
	else if(tn==2)
	{	TKuLZ_SetupTableLengths(ctx, ctx->htab_d, cl);
		ctx->DecodeDistSym=TKuLZ_DecodeDistSymHuff; }
}

void TKuLZ_DecodeFixedTable(TKuLZ_DecState *ctx)
{
	byte cl[256];
	int tn, ttag, tparm;

	tn=TKuLZ_ReadBits(ctx, 2);
	ttag=TKuLZ_ReadBits(ctx, 2);
	tparm=TKuLZ_ReadBits(ctx, 4);

	if(ttag==0)
	{
		if((tparm<1) || (tparm>8))
			ctx->status=TKULZ_STATUS_BADHT;
		
		if(tn==0)
		{
			ctx->hparm_t=tparm;
			ctx->DecodeTagSym=TKuLZ_DecodeTagSymFixed;
			if(tparm==8)
				ctx->DecodeTagSym=TKuLZ_Read8Bits;
			if(tparm==7)
				ctx->DecodeTagSym=TKuLZ_Read7Bits;
		}
		else if(tn==1)
		{
			ctx->hparm_l=tparm;
			ctx->DecodeLitSym=TKuLZ_DecodeLitSymFixed;
			ctx->DecodeLitSym4x=TKuLZ_DecodeLitSym4xFixed;
			ctx->DecodeLitSym8x=TKuLZ_DecodeLitSym8xFixed;
			if(tparm==8)
			{
				ctx->DecodeLitSym=TKuLZ_Read8Bits;
				ctx->DecodeLitSym4x=TKuLZ_Read32Bits;
				ctx->DecodeLitSym8x=TKuLZ_Read64Bits;
			}
			if(tparm==7)
			{
				ctx->DecodeLitSym=TKuLZ_Read7Bits;
				ctx->DecodeLitSym4x=TKuLZ_Read7x4Bits;
				ctx->DecodeLitSym8x=TKuLZ_Read7x8Bits;
			}
		}
		else if(tn==2)
		{
			ctx->hparm_d=tparm;
			ctx->DecodeDistSym=TKuLZ_DecodeDistSymFixed;
			if(tparm==8)
				ctx->DecodeDistSym=TKuLZ_Read8Bits;
			if(tparm==7)
				ctx->DecodeDistSym=TKuLZ_Read7Bits;
		}
	}else
		if(ttag==1)
	{
		if(tparm>=8)
			ctx->status=TKULZ_STATUS_BADHT;
		
		if(tn==0)
		{	ctx->hparm_t=tparm;
			ctx->DecodeTagSym=TKuLZ_DecodeTagSymRice;	}
		else if(tn==1)
		{	ctx->hparm_l=tparm;
			ctx->DecodeLitSym=TKuLZ_DecodeLitSymRice;
			ctx->DecodeLitSym4x=TKuLZ_DecodeLitSym4xRice;
			ctx->DecodeLitSym8x=TKuLZ_DecodeLitSym8xPair;
		}
		else if(tn==2)
		{	ctx->hparm_d=tparm;
			ctx->DecodeDistSym=TKuLZ_DecodeDistSymRice; }
	}else
		if(ttag==2)
	{
		if(tn==0)
		{	ctx->hparm_t=tparm;
			ctx->DecodeTagSym=TKuLZ_DecodeTagSymRice2;	}
		else if(tn==1)
		{	ctx->hparm_l=tparm;
			ctx->DecodeLitSym=TKuLZ_DecodeLitSymRice2;
			ctx->DecodeLitSym4x=TKuLZ_DecodeLitSym4xRice2;
			ctx->DecodeLitSym8x=TKuLZ_DecodeLitSym8xPair;
		}
		else if(tn==2)
		{	ctx->hparm_d=tparm;
			ctx->DecodeDistSym=TKuLZ_DecodeDistSymRice2; }
	}else
	{
		ctx->status=TKULZ_STATUS_BADHT;
	}
}

void TKuLZ_DecodeBlock(TKuLZ_DecState *ctx)
{
	int tag;
	
	tag=TKuLZ_ReadBits(ctx, 4);
	if(tag==0)
		{ ctx->status=TKULZ_STATUS_END; return; }
	if(tag==1)
		{ TKuLZ_DecodeRawBlock(ctx); return; }
	if(tag==2)
		{ TKuLZ_DecodeLzBlock(ctx); return; }
	if((tag>=4) && (tag<7))
		{ TKuLZ_DecodeTable(ctx, tag-4); return; }
	if(tag==8)
		{ TKuLZ_DecodeFixedTable(ctx); return; }
	if(tag==9)
		{ ctx->status=TKULZ_STATUS_END; TKuLZ_DecodeRawBlock(ctx); return; }
	if(tag==10)
		{ ctx->status=TKULZ_STATUS_END; TKuLZ_DecodeLzBlock(ctx); return; }
	ctx->status=TKULZ_STATUS_BADTAG;
}

void TKuLZ_SetupDecodeBuffers(TKuLZ_DecState *ctx,
	byte *dst, byte *src, int dsz, int ssz)
{
	ctx->cs=src;	ctx->ct=dst;
	ctx->pos=0;		ctx->status=0;
#ifdef HAVE_MISAL
	ctx->win=get_u32le(src);
#else
	TKuLZ_SkipBits(ctx, 16);
	TKuLZ_SkipBits(ctx, 16);
#endif
}

int TKuLZ_DecodeBuffer(TKuLZ_DecState *ctx,
	byte *dst, byte *src, int dsz, int ssz)
{
	TKuLZ_SetupDecodeBuffers(ctx, dst, src, dsz, ssz);
	while(!(ctx->status))
		{ TKuLZ_DecodeBlock(ctx); }
	if(ctx->status && (ctx->status!=1))
		printf("Error %d\n", ctx->status);
	return(ctx->ct-dst);
}

int TKuLZ_DecodeBufferNoCtx(
	byte *dst, byte *src, int dsz, int ssz)
{
	static TKuLZ_DecState *ctx=NULL;

	if(!ctx)
		ctx=malloc(sizeof(TKuLZ_DecState));

	TKuLZ_SetupDecodeBuffers(ctx, dst, src, dsz, ssz);
	while(!(ctx->status))
		{ TKuLZ_DecodeBlock(ctx); }
	if(ctx->status && (ctx->status!=1))
		printf("Error %d\n", ctx->status);
	return(ctx->ct-dst);
}

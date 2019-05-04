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
};

int TKuLZ_PeekBits(TKuLZ_DecState *ctx, int bits)
	{ return((ctx->win>>ctx->pos)&((1<<bits)-1)); }

void TKuLZ_SkipBits(TKuLZ_DecState *ctx, int bits)
{
	ctx->pos+=bits;
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
}

int TKuLZ_ReadBits(TKuLZ_DecState *ctx, int bits)
{
	int b;
	b=TKuLZ_PeekBits(ctx, bits);
	TKuLZ_SkipBits(ctx, bits);
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
}

byte TKuLZ_TransposeByte(byte v)
{
	static const byte trans4[16]={
		0x0,0x8,0x4,0xC, 0x2,0xA,0x6,0xE,
		0x1,0x9,0x5,0xD, 0x3,0xB,0x7,0xF};
	return((trans4[v&15]<<4)|trans4[(v>>4)&15]);
}

u16 TKuLZ_TransposeWord(u16 v)
	{ return((TKuLZ_TransposeByte(v&255)<<8)|TKuLZ_TransposeByte(v>>8)); }

void TKuLZ_SetupTableLengths(TKuLZ_DecState *ctx, u16 *htab, byte *cls)
{
	int c, l, tc, ntc;
	int i, j, k;

	for(i=0; i<TKULZ_HTABSZ; i++)
		htab[i]=0;
	
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
	if(!((c>>8)&15))
		{ __debugbreak(); }
	TKuLZ_SkipBits(ctx, (c>>8)&15);
	return(c&255);
}

int TKuLZ_DecodeTagSym(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_t)); }
int TKuLZ_DecodeLitSym(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_l)); }
int TKuLZ_DecodeDistSym(TKuLZ_DecState *ctx)
	{ return(TKuLZ_DecodeHuffSym(ctx, ctx->htab_d)); }

int TKuLZ_DecodeDistance(TKuLZ_DecState *ctx)
{
	int pfx, eb, d;
	pfx=TKuLZ_DecodeDistSym(ctx);
	eb=pfx>>3; d=pfx&7;
	if(eb) { eb--; d=((8+d)<<eb)|TKuLZ_ReadBits(ctx, eb); }
	return(d);
}

void TKuLZ_CopyMatch(TKuLZ_DecState *ctx, int len, int dist)
{
	byte *cs, *ct, *cte;
	int i;
	
	ct=ctx->ct; cs=ct-dist; cte=ct+len;
	if(dist>=16)
	{
		while(ct<cte)
			{ memcpy(ct, cs, 16); ct+=16; cs+=16; }
	}else
//		if(len>16)
		if(0)
	{
		for(i=0; i<16; i++)
			ct[i]=cs[i];
		while(ct<cte)
			{ memcpy(ct, cs, 16); ct+=16; cs+=16; }
	}else
	{
		while(ct<cte)
			{ *ct++=*cs++; }
	}
	
	ctx->ct=cte;
}

void TKuLZ_DecodeLiterals(TKuLZ_DecState *ctx, int len)
{
	int i;
	i=len;
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
	if(tn==0)		{ TKuLZ_SetupTableLengths(ctx, ctx->htab_t, cl); }
	else if(tn==1)	{ TKuLZ_SetupTableLengths(ctx, ctx->htab_l, cl); }
	else if(tn==2)	{ TKuLZ_SetupTableLengths(ctx, ctx->htab_d, cl); }
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
	TKuLZ_SkipBits(ctx, 16);
	TKuLZ_SkipBits(ctx, 16);
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

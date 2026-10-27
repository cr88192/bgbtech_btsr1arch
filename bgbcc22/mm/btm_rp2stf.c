/*
 * STF+AdRice post-compressor for RP2.
 *
 * Escape Magic: 10 00
 *
 * Uses 3 contexts: Tag, Length/Distance, and Literal.
 *
 * Mostly separates bytes by category and then uses the appropriate context.
 *
 * Uses an AdRice implementation that steps by a fraction of a bit.
 * The slower stepping was better for compression in this case.
 * Where, Rice involves splitting the value into a unary coded prefix (Q)
 * and a K bit suffix. In AdRice, the K value is adjusted based on Q.
 * Rather than directly incrementing/decrementing K, was more effective to
 * step with a partial bit here to slow the adaptation.
 *
 * STF: Swap Towards Front
 * As each symbol is encoded, it is swapped towards the front of the list.
 * This will typically swap symbols towards an approximation of an optimal order.
 *
 *
 * AdSkew+STF:
 *
 * Escape Magic: 50 00
 * Modified version of the scheme which uses a "Skew Coding" scheme rather
 * than AdRice.
 */

#ifndef RP2POST_RP2STF_C
#define RP2POST_RP2STF_C

#ifdef GFXEDIT_MINRP2_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	GfxEdit_DecodeRP2(obuf, ibuf, osz, isz)
#endif
#endif

#ifdef TGVLZ1_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	TgvLz_DecodeBufferRP2C(ibuf, obuf, isz, osz)
#endif
#endif

#define BTM_STFRK_ADBSHL	5
#define BTM_STFRK_ADBINIT	6

// #define BTM_STFRK_ADSKSHL	5
// #define BTM_STFRK_ADSKSHL	4
#define BTM_STFRK_ADSKSHL	3
// #define BTM_STFRK_ADSKSHL	2
#define BTM_STFRK_ADSKINIT	0

typedef struct BTM_StfRkCtx_s BTM_StfRkCtx;
struct BTM_StfRkCtx_s {
int (*ReadAdRice)(BTM_StfRkCtx *ctx, byte *rkf);
void (*WriteAdRice)(BTM_StfRkCtx *ctx, int val, byte *rkf);
byte *cst;
u32 win;
byte pos;
};

int BTM_StfRk_PeekBits(BTM_StfRkCtx *ctx, int bits)
{
	u32 v;
	v=*(u32 *)(ctx->cst);
	return((v>>ctx->pos)&((1<<bits)-1));
}

void BTM_StfRk_SkipBits(BTM_StfRkCtx *ctx, int bits)
{
	int p;

	p=ctx->pos+bits;
	ctx->cst+=p>>3;
	ctx->pos=p&7;
}

void BTM_StfRk_SetupReadBits(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->pos=0;
}

#if 1
static const byte btm_stfrk_riceqtab[256]={
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 8
};

int BTM_StfRk_ReadAdRiceB(BTM_StfRkCtx *ctx, byte *rkf)
{
	int b, q, r, v, kf, kfb, skb;
	int p;

	kfb=*rkf;
	kf=kfb>>BTM_STFRK_ADBSHL;
	b=BTM_StfRk_PeekBits(ctx, 16);
	q=btm_stfrk_riceqtab[b&0xFF];
	if(q==8)
	{
		kfb++;
		skb=16;
		v=(b>>8)&0xFF;
	}else
	{
		r=(b>>(q+1))&((1<<kf)-1);
		skb=q+kf+1;
		v=(q<<kf)|r;
		if(!q && kfb)
			{ kfb--; }
		else if(q>1)
			{ kfb++; }
	}

	BTM_StfRk_SkipBits(ctx, skb);
	*rkf=kfb;
	return(v);
}
#endif

int BTM_StfRk_ReadAdRiceSTF(BTM_StfRkCtx *ctx, byte *rkf, byte *stftab)
{
	int i0, i1, v0, v1;
	
//	i0=BTM_StfRk_ReadAdRiceB(ctx, rkf);
	i0=ctx->ReadAdRice(ctx, rkf);
	i1=(i0*15)>>4;
	v0=stftab[i0];	v1=stftab[i1];
	stftab[i0]=v1;	stftab[i1]=v0;
	return(v0);
}

/*
   (15:12)=Sym Len
   (11: 8)=Prefix Len
   ( 7: 0)=Base
 */

#if 0
const u16 btm_stfrk_skewtab[4*16]={
/* Skew 0 */
	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,
/* Skew 1 */
	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,
/* Skew 2 */
	0x6900, 0x7220, 0x6900, 0x9340,
	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,
	0x6900, 0x7220, 0x6900, 0xA380,
/* Skew 3 */
	0x5900, 0x6210, 0x5900, 0x8320,
	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,
	0x5900, 0x6210, 0x5900, 0xB480,
};

const u16 btm_stfrk_skewtab2[5*32]={
/* Skew 0 */
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
/* Skew 1 */
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
/* Skew 2 */
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
/* Skew 3 */
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
/* Skew 4 */
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xB540,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xC580,
};
#endif

const u16 btm_stfrk_skewtab3[8*64]={
/* Skew 0 */
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
	0x8900, 0x8180, 0x8900, 0x8180,	0x8900, 0x8180, 0x8900, 0x8180,
/* Skew 1 */
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
	0x7900, 0x8240, 0x7900, 0x9280,	0x7900, 0x8240, 0x7900, 0x9280,
/* Skew 2 */
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
	0x6900, 0x7220, 0x6900, 0x9340,	0x6900, 0x7220, 0x6900, 0xA380,
/* Skew 3 */
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xA440,
	0x5900, 0x6210, 0x5900, 0x8320,	0x5900, 0x6210, 0x5900, 0xB480,
/* Skew 4 */
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xB540,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xC580,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xB540,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0x9420,
	0x4900, 0x5208, 0x4900, 0x7310,	0x4900, 0x5208, 0x4900, 0xC580,
/* Skew 5 */
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0x8410,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0xA520,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0x8410,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0xC640,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0x8410,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0xA520,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0x8410,
	0x3900, 0x4204, 0x3900, 0x6308,	0x3900, 0x4204, 0x3900, 0xD680,
/* Skew 6 / 5+ */
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0x8410,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0xA520,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0x8410,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0xC640,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0x8410,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0xA520,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0x8410,
	0x3100, 0x4204, 0x3100, 0x6308,	0x3100, 0x4204, 0x3100, 0xD680,
/* Skew 7 / -1 */
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
	0x8900, 0x8980, 0x8900, 0x8980,	0x8900, 0x8980, 0x8900, 0x8980,
};

int BTM_StfRk_ReadAdSkew(BTM_StfRkCtx *ctx, byte *rkf)
{
	int b, q, r, v, kf, kfb, skb;
	int kfb_nz, kfb_nr;
	int p;

	kfb=*rkf;
	kf=kfb>>BTM_STFRK_ADSKSHL;
	b=BTM_StfRk_PeekBits(ctx, 16);
//	q=btm_stfrk_skewtab[(b&0xF)|(kf<<4)];
//	q=btm_stfrk_skewtab2[(b&0x1F)|(kf<<5)];
	q=btm_stfrk_skewtab3[(b&0x3F)|(kf<<6)];
	skb=(q>>12)&15;

	p=(q>>8)&7;
	r=(1<<(skb-p))-1;
	v=(q&0xFF)+((b>>p)&r);

	BTM_StfRk_SkipBits(ctx, skb);

#if 1
	kfb_nz=(~(kfb-1))>>8;
	kfb_nr=((((5<<BTM_STFRK_ADSKSHL)-2)-kfb)>>8)+1;
	p=((q>>11)&1)-1;
	kfb_nz&= p;		kfb_nr&=~p;
	r=kfb_nz+kfb_nr;
	kfb+=r;
//	if(q&0x0800)	{ kfb+=kfb_nr; }
//	else			{ kfb+=kfb_nz; }
#endif

#if 0
	if(q&0x0800)
		{ if((kfb+1)<(5<<BTM_STFRK_ADSKSHL)) kfb++; }
	else
		{ if(kfb>0) kfb--; }
#endif
	*rkf=kfb;
	return(v);
}

int BTM_StfRk_ReadAdSkewB(BTM_StfRkCtx *ctx, byte *rkf)
{
	int b, q, r, v, kf, kfb, skb;
	int p;

	b=BTM_StfRk_PeekBits(ctx, 16);
	kfb=*rkf;
	kf=(kfb>>BTM_STFRK_ADSKSHL)&7;
	q=btm_stfrk_skewtab3[(b&0x3F)|(kf<<6)];
	skb=(q>>12)&15;
	p=(q>>8)&7;
	r=(1<<(skb-p))-1;
	v=(q&0xFF)+((b>>p)&r);
	kfb+=((q>>11)&1)*2-1;
	*rkf=kfb;
	BTM_StfRk_SkipBits(ctx, skb);
	return(v);
}

void BTM_StfRk_WriteBits(BTM_StfRkCtx *ctx, int val, int bits)
{
	ctx->win|=(val&((1U<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->cst++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void BTM_StfRk_FlushBits(BTM_StfRkCtx *ctx)
{
	int p;
	p=ctx->pos;
	while(p>0)
	{
		*ctx->cst++=ctx->win;
		ctx->win>>=8;
		p-=8;
	}
	ctx->pos=0;
}

void BTM_StfRk_WriteAdRiceB(BTM_StfRkCtx *ctx, int val, byte *rkf)
{
	int q, kf, kfb;
	
	kfb=*rkf;
	kf=kfb>>BTM_STFRK_ADBSHL;

	q=val>>kf;

	if(q>=8)
	{
		kfb++; *rkf=kfb;
		BTM_StfRk_WriteBits(ctx, 0xFF, 8);
		BTM_StfRk_WriteBits(ctx, val, 8);
		return;
	}

	BTM_StfRk_WriteBits(ctx, (1<<q)-1, q+1);
	BTM_StfRk_WriteBits(ctx, val&((1<<kf)-1), kf);
	if(!q && kfb)
		{ kfb--; *rkf=kfb; }
	else if(q>1)
		{ kfb++; *rkf=kfb; }
}

void BTM_StfRk_WriteAdRiceSTF(BTM_StfRkCtx *ctx, int val,
	byte *rkf, byte *stfvtab, byte *stfitab)
{
	int i0, i1, v0, v1;
	
	i0=stfitab[val];
	i1=(i0*15)>>4;
	v0=stfvtab[i0];	v1=stfvtab[i1];
	stfvtab[i0]=v1;	stfvtab[i1]=v0;
	stfitab[v1]=i0;	stfitab[v0]=i1;
	
//	BTM_StfRk_WriteAdRiceB(ctx, i0, rkf);
	ctx->WriteAdRice(ctx, i0, rkf);
}

void BTM_StfRk_WriteAdSkewI(BTM_StfRkCtx *ctx, int val, byte *rkf, int flag)
{
	int q, kf, kfb, kd, b, n;
	
	kfb=*rkf;
	kf=(kfb>>BTM_STFRK_ADSKSHL)&7;

//	q=val>>kf;
	b=0;
	n=0;

	if((kf==0) || (kf==7))
	{
		kd=val<0x80;
		b=((val&0x7F)<<1)|(!kd);
		n=8;
		if(kf==7)
			kd=1;
	}else
		if(kf==1)
	{
		if(val<0x40)
			{ b=(val<<1)|0; n=7; kd=1; }
		else if(val<0x80)
			{ b=(val<<2)|1; n=8; kd=0; }
		else
			{ b=(val<<2)|3; n=9; kd=0; }
	}else
		if(kf==2)
	{
		if(val<0x20)
			{ b=(val<<1)|0; n=6; kd=1; }
		else if(val<0x40)
			{ b=(val<<2)|1; n=7; kd=0; }
		else if(val<0x80)
			{ b=(val<<3)|3; n=9; kd=0; }
		else
			{ b=(val<<3)|7; n=10; kd=0; }
	}else
		if(kf==3)
	{
		if(val<0x10)
			{ b=(val<<1)|0; n=5; kd=1; }
		else if(val<0x20)
			{ b=(val<<2)|1; n=6; kd=0; }
		else if(val<0x40)
			{ b=(val<<3)|3; n=8; kd=0; }
		else if(val<0x80)
			{ b=(val<<4)|7; n=10; kd=0; }
		else
			{ b=(val<<4)|15; n=11; kd=0; }
	}else
		if(kf==4)
	{
		if(val<0x08)
			{ b=(val<<1)|0; n=4; kd=1; }
		else if(val<0x10)
			{ b=(val<<2)|1; n=5; kd=0; }
		else if(val<0x20)
			{ b=(val<<3)|3; n=7; kd=0; }
		else if(val<0x40)
			{ b=(val<<4)|7; n=9; kd=0; }
		else if(val<0x80)
			{ b=(val<<5)|15; n=11; kd=0; }
		else
			{ b=(val<<5)|31; n=12; kd=0; }
	}else
		if((kf==5) || (kf==6))
	{
		if(val<0x04)
			{ b=(val<<1)|0; n=3; kd=1; }
		else if(val<0x08)
			{ b=(val<<2)|1; n=4; kd=0; }
		else if(val<0x10)
			{ b=(val<<3)|3; n=6; kd=0; }
		else if(val<0x20)
			{ b=(val<<4)|7; n=8; kd=0; }
		else if(val<0x40)
			{ b=(val<<5)|15; n=10; kd=0; }
		else if(val<0x80)
			{ b=(val<<6)|31; n=12; kd=0; }
		else
			{ b=(val<<6)|63; n=13; kd=0; }
		if(kf==6)
			kd=0;
	}

	BTM_StfRk_WriteBits(ctx, b, n);

	if(flag&1)
	{
		if(kd)	{ kfb++; }
		else	{ kfb--; }
	}else
	{
		if(kd)
			{ if((kfb+1)<(5<<BTM_STFRK_ADSKSHL)) kfb++; }
		else
			{ if(kfb>0) kfb--; }
	}
	*rkf=kfb;
}

void BTM_StfRk_WriteAdSkew(BTM_StfRkCtx *ctx, int val, byte *rkf)
{
	BTM_StfRk_WriteAdSkewI(ctx, val, rkf, 0);
}

void BTM_StfRk_WriteAdSkewB(BTM_StfRkCtx *ctx, int val, byte *rkf)
{
	BTM_StfRk_WriteAdSkewI(ctx, val, rkf, 1);
}

void BTM_StfRk_EncodeInit(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->win=0;
	ctx->pos=0;
}

void BTM_StfRk_DecodeInit(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->win=*(u32 *)buf;
	ctx->pos=0;
}

int BTM_StfRk_CheckPostRp2Blob(byte *ibuf)
{
	if((ibuf[0]==0x10) && (ibuf[1]==0x00))
		return(1);
	if((ibuf[0]==0x50) && (ibuf[1]==0x00))
		return(1);
	if((ibuf[0]==0x40) && (ibuf[1]==0x00))
		return(1);
	return(0);
}

int BTM_StfRk_EncodeBufferPostRp2I(
	byte *obuf, byte *ibuf, int ibsz, int flag)
{
	byte prbtab_t[256], prbidx_t[256];
	byte prbtab_d[256], prbidx_d[256];
	byte prbtab_l[256], prbidx_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u32 tag;
	int tsz, nr;
	int i, j, k;

	ctx=&t_ctx;

	ct=obuf;
	*ct++=0x10;
	*ct++=0x00;
	
	BTM_StfRk_EncodeInit(ctx, ct);
	ctx->WriteAdRice=BTM_StfRk_WriteAdRiceB;

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;	prbidx_t[i]=i;
		prbtab_d[i]=i;	prbidx_d[i]=i;
		prbtab_l[i]=i;	prbidx_l[i]=i;
	}

	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	if(flag&1)
	{
#if 0
		obuf[0]=0x50;
		ctx->WriteAdRice=BTM_StfRk_WriteAdSkew;
#endif
#if 1
		obuf[0]=0x40;
		ctx->WriteAdRice=BTM_StfRk_WriteAdSkewB;
//		ctx->WriteAdRice=BTM_StfRk_WriteAdSkew;
#endif
		rk_t=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
		rk_d=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
		rk_l=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
	}

	cs=ibuf;
	cse=ibuf+ibsz;
	while(cs<cse)
	{
		tag=*(u32 *)cs;
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))
		{
			nr=(tag>>5)&7;
			if(tag&0x100)		{ tsz=3; }
			else				{ tsz=2; }
		}
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }
		else if(!(tag&0x80))
		{
			if(tag&0x100)
				{ tsz=6; nr=(tag>>9)&7; }
			else
				{ tsz=2; nr=0; }
		}
		else
		{
			if(!(tag&0x100))
				{ tsz=5; nr=(tag>>9)&7; }
			else
				{ tsz=-1; }
		}
		
		if(tsz<1)
			return(-1);

		if((cs+(tsz+nr))>cse)
			return(-1);
		
		BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_t, prbtab_t, prbidx_t);
		for(i=1; i<tsz; i++)
			{ BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_d, prbtab_d, prbidx_d); }
		for(i=0; i<nr; i++)
			{ BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_l, prbtab_l, prbidx_l); }
	}
	BTM_StfRk_FlushBits(ctx);
	
//	printf("  rk_t=%d rk_d=%d rk_l=%d\n", rk_t>>5, rk_d>>5, rk_l>>5);
	
	return(ctx->cst-obuf);
}

int BTM_StfRk_EncodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	static byte *t_s0buf, *t_s1buf;
	byte *t0buf, *t1buf;
	int osz0, osz1, osz;
	
	if(!t_s0buf)
	{
		t_s0buf=malloc(15360);
		t_s1buf=malloc(15360);
	}
	
	if((ibsz*1.25)>15360)
	{
		t0buf=malloc(ibsz*1.25);
		t1buf=malloc(ibsz*1.25);
	}else
	{
		t0buf=t_s0buf;
		t1buf=t_s1buf;
	}
	
	osz0=BTM_StfRk_EncodeBufferPostRp2I(t0buf, ibuf, ibsz, 0);
	osz1=BTM_StfRk_EncodeBufferPostRp2I(t1buf, ibuf, ibsz, 1);
	
	if(osz0<osz1)
	{
		osz=osz0;
		memcpy(obuf, t0buf, osz);
	}else
	{
		osz=osz1;
		memcpy(obuf, t1buf, osz);
	}
	
	if(t0buf!=t_s0buf)
	{
		free(t0buf);
		free(t1buf);
	}
	
	return(osz);
	
//	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 0));
//	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 1));
}

int BTM_StfRk_EncodeBufferPostRp2Rice(byte *obuf, byte *ibuf, int ibsz)
{
	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 0));
//	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 1));
}

int BTM_StfRk_EncodeBufferPostRp2Skew(byte *obuf, byte *ibuf, int ibsz)
{
//	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 0));
	return(BTM_StfRk_EncodeBufferPostRp2I(obuf, ibuf, ibsz, 1));
}

int BTM_StfRk_DecodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[256];
	byte prbtab_d[256];
	byte prbtab_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u64 tag, tag1, tag2, tag3;
	int lc, tsz, tsi, nr, tg;
	int i, j, k;

//	if(((ibuf[0]!=0x10) &&
//		(ibuf[0]!=0x50) &&
//		(ibuf[0]!=0x40)) ||
//		(ibuf[1]!=0x00))
	if(!BTM_StfRk_CheckPostRp2Blob(ibuf))
		return(-1);

	ctx=&t_ctx;
	BTM_StfRk_DecodeInit(ctx, ibuf+2);
	ctx->ReadAdRice=BTM_StfRk_ReadAdRiceB;
	
	ct=obuf;

	tag=0; tag1=0; tag2=0; tag3=0;

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;
		prbtab_d[i]=i;
		prbtab_l[i]=i;
	}
	
	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	if((ibuf[0]==0x50) || (ibuf[0]==0x40))
	{
		ctx->ReadAdRice=BTM_StfRk_ReadAdSkew;
		if(ibuf[0]==0x40)
			ctx->ReadAdRice=BTM_StfRk_ReadAdSkewB;
		rk_t=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
		rk_d=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
		rk_l=BTM_STFRK_ADSKINIT<<BTM_STFRK_ADSKSHL;
	}

	while(1)
	{
		tg=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_t, prbtab_t);

		*ct++=tg;
		if(tg==0x1F)
			break;
		
		tag=tg;
		tsi=1;

		if(!(tg&0x01))
			{ tsz=2; }
		else if(!(tg&0x02))
			{ tsz=3; }
		else if(!(tg&0x04))
			{ tsz=4; }
		else if(!(tg&0x08))
			{ tsz=1; }
		else if(!(tg&0x10))
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			tsz=2;
			if(tag&0x0100)
				tsz=3;
		}
		else if(!(tg&0x20))
			{ tsz=1; }
		else if(!(tg&0x40))
			{ tsz=2; }
		else if(!(tg&0x80))
		{
			tsz=2;
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=6;
		}else
		{
			tsz=2;
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			if(!(tag&0x0100))
				tsz=5;
		}
						
		for(i=tsi; i<tsz; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
//			if(i<4)
				tag|=((u64)j)<<(i*8);
		}
		
		if(!tag)
			break;
		
		if(!(tag&0x0001))		{ nr=(tag>>1)&7; }
		else if(!(tag&0x0002))	{ nr=(tag>>2)&7; }
		else if(!(tag&0x0004))	{ nr=(tag>>3)&7; }
		else if(!(tag&0x0008))	{ nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x0010))	{ nr=(tag>>5)&7; }
		else if(!(tag&0x0020))	{ nr=(tag>>6)&3; }
		else if(!(tag&0x0040))	{ nr=(((tag>>7)&511)+1)*8; }	
		else if(!(tag&0x0080))	{ nr=(tag&0x0100)?((tag>>9)&7):0; }	
		else if(!(tag&0x0100))	{ nr=(tag>>9)&7; }	

		for(i=0; i<nr; i++)
			{ *ct++=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_l, prbtab_l); }

		tag3=tag2;
		tag2=tag1;
		tag1=tag;
	}
	return(ct-obuf);
}

int BTM_StfRk_DummyDecodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[256];
	byte prbtab_d[256];
	byte prbtab_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u32 tag, tag1, tag2, tag3;
	int lc, tsz, tsi, nr, tg;
	int i, j, k;

//	if(((ibuf[0]!=0x10) && (ibuf[0]!=0x50)) || (ibuf[1]!=0x00))
	if(!BTM_StfRk_CheckPostRp2Blob(ibuf))
		return(-1);

	ctx=&t_ctx;
	BTM_StfRk_DecodeInit(ctx, ibuf+2);
	ctx->ReadAdRice=BTM_StfRk_ReadAdRiceB;
	
	ct=obuf;

	tag=0; tag1=0; tag2=0; tag3=0;

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;
		prbtab_d[i]=i;
		prbtab_l[i]=i;
	}
	
	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	if((ibuf[0]==0x50) || (ibuf[0]==0x40))
	{
		ctx->ReadAdRice=BTM_StfRk_ReadAdSkew;
		if(ibuf[0]==0x40)
			ctx->ReadAdRice=BTM_StfRk_ReadAdSkewB;
		rk_t=0<<BTM_STFRK_ADBSHL;
		rk_d=0<<BTM_STFRK_ADBSHL;
		rk_l=0<<BTM_STFRK_ADBSHL;
	}

	while(1)
	{
		tg=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_t, prbtab_t);
//		*ct++=tg;
		if((*ct++)!=tg)
			break;

		if(tg==0x1F)
			break;
		
		tag=tg;
		tsi=1;

		if(!(tg&0x01))
			{ tsz=2; }
		else if(!(tg&0x02))
			{ tsz=3; }
		else if(!(tg&0x04))
			{ tsz=4; }
		else if(!(tg&0x08))
			{ tsz=1; }
		else if(!(tg&0x10))
		{
			tsz=1;
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=3;
		}
		else if(!(tg&0x20))
			{ tsz=1; }
		else if(!(tg&0x40))
			{ tsz=2; }
		else if(!(tg&0x80))
		{
			tsz=2;
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=6;
		}else
		{
			tsz=2;
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			tag|=j<<8;
			tsi=2;
			if(!(tag&0x0100))
				tsz=5;
		}
		
		for(i=tsi; i<tsz; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			if(i<4)
				tag|=j<<(i*8);
		}
		
		if(!tag)
			break;
		
		if(!(tag&0x0001))		{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x0002))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x0004))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x0008))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x0010))	{ tsz=1; nr=(tag>>5)&7; }
		else if(!(tag&0x0020))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x0040))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }	
		else if(!(tag&0x0080))	{ nr=(tag&0x0100)?((tag>>9)&7):0; }	
		else if(!(tag&0x0100))	{ nr=(tag>>9)&7; }	
		for(i=0; i<nr; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_l, prbtab_l);
//			*ct++=j;
			if((*ct++)!=j)
				break;
		}

		tag3=tag2;
		tag2=tag1;
		tag1=tag;
	}
	return(ct-obuf);
}

int BTM_StfRk_EncodeBufferPostRp2Test(byte *obuf, byte *ibuf, int ibsz)
{
	int osz, i2sz;
	osz=BTM_StfRk_EncodeBufferPostRp2(obuf, ibuf, ibsz);
	i2sz=BTM_StfRk_DummyDecodeBufferPostRp2(ibuf, obuf, osz);
	if(i2sz!=ibsz)
		return(-1);
	return(osz);
}

int PostRp2Stf_DecodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_StfRk_DecodeBufferPostRp2(obuf, ibuf, ibsz));
}

int PostRp2Stf_DecodeBufferRp2Full(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
#ifdef HAS_RP2STF_DECODERP2
	static byte *ts_buf;
	byte *tbuf;
	int tbsz, osz;
	
	if(ibsz<8192)
	{
		if(!ts_buf)
			ts_buf=malloc(32768);
		tbuf=ts_buf;
	}else
	{
		tbuf=malloc(ibsz*4);
	}
	
	tbsz=BTM_StfRk_DecodeBufferPostRp2(tbuf, ibuf, ibsz);
	if(tbsz<=0)
	{
		if(tbuf!=ts_buf)
			free(tbuf);
		return(tbsz);
	}
	
	osz=PostRp2Stf_DecodeRP2(obuf, tbuf, obsz, tbsz);
	if(tbuf!=ts_buf)
		free(tbuf);
	return(osz);
#else
	return(-1);
#endif
}

int PostRp2Stf_EncodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_StfRk_EncodeBufferPostRp2(obuf, ibuf, ibsz));
}

int PostRp2Stf_EncodeBufferPostRp2Test(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_StfRk_EncodeBufferPostRp2Test(obuf, ibuf, ibsz));
}

#endif

/*
Bitwise Arithmetic Coder
 */

#define BTLZA_BITARITH_LOWER 0x00000000
#define BTLZA_BITARITH_UPPER 0xFFFFFFFF
#define BTLZA_BITARITH_WBITS 8

// #define BT1H_RANGE_USEN4
// #define BT1H_RANGE_USEN4S8

// byte btic1h_range_transtab8[256];	//byte transpose table
// u16 btic1h_range_transtab9[512];	//byte transpose table

byte btic1h_range_weightadjust[512];		//weight adjustment tables

// void *btic1h_range_mutex=NULL;

#if 0
// int btic1h_range_ctxindex(int ctx, int mask)
{
//	int i;
//	i=((ctx>>16)*251)>>8;
//	return((ctx^(i<<16))&mask);
	return(ctx&mask);
}
#endif

#define btic1h_range_ctxindex(ctx, mask) ((ctx)&(mask))


void BTIC1H_Range_Lock()
{
}

void BTIC1H_Range_Unlock()
{
}

void BTIC1H_Range_Init()
{
	static int init=0;
	int i, j, k;

	if(init)return;
	init=1;

#if 0
	for(i=0; i<256; i++)
	{
		j=((i&1)<<7)|((i&2)<<5)|((i&4)<<3)|((i&8)<<1);
		j|=((i&16)>>1)|((i&32)>>3)|((i&64)>>5)|((i&128)>>7);
		btic1h_range_transtab8[i]=j;
	}

	for(i=0; i<512; i++)
	{
		j=((i&1)<<7)|((i&2)<<5)|((i&4)<<3)|((i&8)<<1);
		j|=((i&16)>>1)|((i&32)>>3)|((i&64)>>5)|((i&128)>>7);
		btic1h_range_transtab9[i]=j;
	}
#endif
	
	for(i=0; i<256; i++)
	{
		j=i+((256-i)>>5);
		k=i-(i>>5);

		if(j>251)j=251;
		if(k<4)k=4;

//		if(j>239)j=239;
//		if(k<16)k=16;

//		if(j>223)j=223;
//		if(k<32)k=32;

//		if(j>191)j=191;
//		if(k<64)k=64;

//		if(j>231)j=231;
//		if(k<24)k=24;
//		if(j>227)j=227;
//		if(k<28)k=28;

		btic1h_range_weightadjust[(0<<8)+i]=j;
		btic1h_range_weightadjust[(1<<8)+i]=k;
	}
//	btic1h_range_weightadjust[(0<<8)+255]=255;
//	btic1h_range_weightadjust[(1<<8)+0]=0;
}

void BTIC1H_Range_OutputByte(BTIC1H_Context *ctx, int i)
{
	*ctx->bs_ct++=i;
}

#if 0
void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
	while(!((ctx->rmin^ctx->rmax)>>24))
//	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		BTIC1H_Range_OutputByte(ctx, ctx->rmin>>24);
		ctx->rmin<<=8;
		ctx->rmax<<=8;
		ctx->rmax|=255;
	}
}
#endif

#if 0
void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
//	while(!((ctx->rmin^ctx->rmax)>>24))
	if(!((ctx->rmin^ctx->rmax)>>24))
	{
//		BTIC1H_Range_OutputByte(ctx, ctx->rmin>>24);
		*ctx->bs_ct++=ctx->rmin>>24;
		ctx->rmin<<=8;
		ctx->rmax<<=8;
		ctx->rmax|=255;
	}
}
#endif

#if 1
void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		*ctx->bs_ct++=ctx->rmin>>24;
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;

#if 1
		if(!((ctx->rmin^ctx->rmax)>>24))
		{
			*ctx->bs_ct++=ctx->rmin>>24;
			ctx->rmin<<=8;
			ctx->rmax=(ctx->rmax<<8)|255;
		}
#endif
	}
}
#endif

#if 0
force_inline void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
	int i, j, k;

	j=ctx->rmin^ctx->rmax;
//	j=ctx->rmax-ctx->rmin;
	i=(!(j>>24))+(!(j>>16));

	k=ctx->rmin>>16;
	*(u16 *)ctx->bs_ct=(k<<8)|(k>>8);
	ctx->bs_ct+=i;
	j=i<<3;
	ctx->rmin=ctx->rmin<<j;
	ctx->rmax=((ctx->rmax+1)<<j)-1;
}
#endif

#if 0
void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
	int i, j;

//	while(!((ctx->rmin^ctx->rmax)>>24))

	i=!((ctx->rmin^ctx->rmax)>>24);
	j=i<<3;

	*ctx->bs_ct=ctx->rmin>>24;
	ctx->bs_ct+=i;
	ctx->rmin<<=j;
	ctx->rmax<<=j;
	ctx->rmax|=(1<<j)-1;
}
#endif

#if 0
void BTIC1H_Range_NormalizeOutput(BTIC1H_Context *ctx)
{
	if(((ctx->rmin^ctx->rmax)>>24))
		return;

	*ctx->bs_ct++=ctx->rmin>>24;
	ctx->rmin<<=8;
	ctx->rmax<<=8;
	ctx->rmax|=255;

	if(((ctx->rmin^ctx->rmax)>>24))
		return;

	*ctx->bs_ct++=ctx->rmin>>24;
	ctx->rmin<<=8;
	ctx->rmax<<=8;
	ctx->rmax|=255;

	if(((ctx->rmin^ctx->rmax)>>24))
		return;

	*ctx->bs_ct++=ctx->rmin>>24;
	ctx->rmin<<=8;
	ctx->rmax<<=8;
	ctx->rmax|=255;

	if(((ctx->rmin^ctx->rmax)>>24))
		return;

	*ctx->bs_ct++=ctx->rmin>>24;
	ctx->rmin<<=8;
	ctx->rmax<<=8;
	ctx->rmax|=255;
}
#endif

void BTIC1H_Range_FlushWBits(BTIC1H_Context *ctx)
{
	while((ctx->rmin!=BTLZA_BITARITH_LOWER) ||
		(ctx->rmax!=BTLZA_BITARITH_UPPER))
	{
		BTIC1H_Range_OutputByte(ctx, ctx->rmin>>24);

		ctx->rmin<<=8;
		ctx->rmax<<=8;
		ctx->rmax|=255;
	}
}

void BTIC1H_Range_OutputFixedBit(BTIC1H_Context *ctx, int i)
{
	u32 r, r2, v;
	int j;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>1);

	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;
	BTIC1H_Range_NormalizeOutput(ctx);
}

void BTIC1H_Range_OutputBit(BTIC1H_Context *ctx,
	int i, u32 w)
{
	u32 r, r2, v;
	int j;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTIC1H_Range_NormalizeOutput(ctx);
}

#if 1
void BTIC1H_Range_OutputModelBit(BTIC1H_Context *ctx,
	int i, byte *mctx)
{
	u32 r, v, w;
	int j;

	r=ctx->rmax-ctx->rmin; w=*mctx;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	if(i)
	{
		ctx->rmin=v+1;
//		*mctx=btic1h_range_weightadjust[256|w];
		w=btic1h_range_weightadjust[256+w];
//		w=btic1h_range_weightadjust[256|w];
		*mctx=w;
	}else
	{
		ctx->rmax=v;
//		*mctx=btic1h_range_weightadjust[w];
		w=btic1h_range_weightadjust[w];
		*mctx=w;
	}

//	w=btic1h_range_weightadjust[(i<<8)+w];
//	*mctx=w;

	BTIC1H_Range_NormalizeOutput(ctx);
}
#endif

#if 0
void BTIC1H_Range_OutputModelBit(BTIC1H_Context *ctx,
	int i, byte *mctx)
{
	u32 r, v, w, m, n;
	int j, k;

	m=ctx->rmin; n=ctx->rmax;
	r=n-m; w=*mctx;
	v=m+(r>>BTLZA_BITARITH_WBITS)*w;

	j=i-1; k=~j;
	ctx->rmin=((v+1)&k)|(m&j);
	ctx->rmax=((n  )&k)|(v&j);
	w=btic1h_range_weightadjust[(i<<8)+w];
	*mctx=w;

	BTIC1H_Range_NormalizeOutput(ctx);
}
#endif

#if 1
force_inline void BTIC1H_Range_OutputModelBitN(BTIC1H_Context *ctx,
	int i, byte *mctx)
{
	u32 r, v, w, m, n;
	int j, k;

	m=ctx->rmin; n=ctx->rmax;
	r=n-m; w=*mctx;
	v=m+(r>>BTLZA_BITARITH_WBITS)*w;

	j=i-1; k=~j;
	ctx->rmin=((v+1)&k)|(m&j);
	ctx->rmax=((n  )&k)|(v&j);
	w=btic1h_range_weightadjust[(i<<8)+w];
	*mctx=w;

//	BTIC1H_Range_NormalizeOutput(ctx);
}
#endif

void BTIC1H_Range_OutputFixedBits(BTIC1H_Context *ctx,
	int v, int n)
{
	int i, j, k, l;

	i=n;
	while(i--)
		{ BTIC1H_Range_OutputFixedBit(ctx, (v>>i)&1); }
}

void BTIC1H_Range_OutputContextModelBits(BTIC1H_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=(v>>i)&1;
		BTIC1H_Range_OutputModelBit(ctx, l, mdl+btic1h_range_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
}

void BTIC1H_Range_OutputContextModelBitsN4(BTIC1H_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;

	while(i>=4)
	{
		l=(v>>(--i))&1;
		BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&k));
		j=(j<<1)|l;

		l=(v>>(--i))&1;
		BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&k));
		j=(j<<1)|l;

		l=(v>>(--i))&1;
		BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&k));
		j=(j<<1)|l;

		l=(v>>(--i))&1;
		BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&k));
		j=(j<<1)|l;

		BTIC1H_Range_NormalizeOutput(ctx);
	}

	if(i>0)
	{
		while(i--)
		{
			l=(v>>i)&1;
			BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&k));
			j=(j<<1)|l;
		}

		BTIC1H_Range_NormalizeOutput(ctx);
	}
	ctx->wctx=j;
}

int BTIC1H_Range_SetupEncode(BTIC1H_Context *ctx, byte *out, int sz)
{
	ctx->bs_ct=out;
	ctx->bs_cte=out+sz;
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
	return(0);
}

int BTIC1H_Range_SetupBeginEncode(BTIC1H_Context *ctx)
{
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
	return(0);
}

void BTIC1H_Range_EncodeSymbol8(BTIC1H_Context *ctx, int v,
	byte *mdl, int ctxmask)
{
	int i, j, k, l;

#ifndef BT1H_RANGE_USEN4S8
	j=(ctx->wctx<<8)|(v&255);

	BTIC1H_Range_OutputModelBit(ctx, (v>>7)&1, mdl+((j>>8)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>6)&1, mdl+((j>>7)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>5)&1, mdl+((j>>6)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>4)&1, mdl+((j>>5)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>3)&1, mdl+((j>>4)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>2)&1, mdl+((j>>3)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v>>1)&1, mdl+((j>>2)&ctxmask));
	BTIC1H_Range_OutputModelBit(ctx, (v   )&1, mdl+((j>>1)&ctxmask));

	ctx->wctx=j;
#endif

#ifdef BT1H_RANGE_USEN4S8
	j=(ctx->wctx<<8)|(v&255);

	BTIC1H_Range_OutputModelBitN(ctx, (v>>7)&1, mdl+((j>>8)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v>>6)&1, mdl+((j>>7)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v>>5)&1, mdl+((j>>6)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v>>4)&1, mdl+((j>>5)&ctxmask));
	BTIC1H_Range_NormalizeOutput(ctx);

	BTIC1H_Range_OutputModelBitN(ctx, (v>>3)&1, mdl+((j>>4)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v>>2)&1, mdl+((j>>3)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v>>1)&1, mdl+((j>>2)&ctxmask));
	BTIC1H_Range_OutputModelBitN(ctx, (v   )&1, mdl+((j>>1)&ctxmask));
	BTIC1H_Range_NormalizeOutput(ctx);

	ctx->wctx=j;
#endif

#if 0
	i=8; j=ctx->wctx; k=ctxmask; l=0;

//	l=(v>>(--i))&1;
	l=(v>>7)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v>>6)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v>>5)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v>>4)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
	BTIC1H_Range_NormalizeOutput(ctx);

//	l=(v>>(--i))&1;
	l=(v>>3)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v>>2)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v>>1)&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
//	l=(v>>(--i))&1;
	l=(v   )&1;
	BTIC1H_Range_OutputModelBitN(ctx, l, mdl+(j&ctxmask));
	j=(j<<1)|l;
	BTIC1H_Range_NormalizeOutput(ctx);

	ctx->wctx=j;
#endif

#if 0
	i=8; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=(v>>i)&1;
		BTIC1H_Range_OutputModelBit(ctx, l, mdl+btic1h_range_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
#endif
}

void BTIC1H_Range_ResetContextWeights(
	BTIC1H_Context *ctx)
{
	int i, j;

	j=1<<ctx->rc_ctx_raw;
	for(i=0; i<j; i++)
	{
//		ctx->rc_mdl_raw[i]=1<<(BTLZA_BITARITH_WBITS-1);
		ctx->rc_mdl_raw[i]=128;
		ctx->rc_mdl_rc[i]=128;
	}

	ctx->wctx=0;
}

void BTIC1H_Range_SetupContextBits(
	BTIC1H_Context *ctx, int bits)
{
	int i, j;

	BTIC1H_Range_Init();

	if(ctx->rc_mdl_raw && (ctx->rc_ctx_raw==bits))
	{
		BTIC1H_Range_ResetContextWeights(ctx);
		return;
	}

	if(ctx->rc_mdl_raw && (ctx->rc_ctx_raw!=bits))
	{
		free(ctx->rc_mdl_raw); ctx->rc_mdl_raw=NULL;
		free(ctx->rc_mdl_rc); ctx->rc_mdl_rc=NULL;
	}
	
	j=1<<bits;
	if(!ctx->rc_mdl_raw)
	{
		ctx->rc_mdl_raw=malloc(j);
		ctx->rc_mdl_rc=malloc(j);
	}
	ctx->rc_ctx_raw=bits;
	ctx->rc_msk_raw=(1<<bits)-1;
	ctx->wctx=0;

	BTIC1H_Range_ResetContextWeights(ctx);
}



void TKRA_DrawSpanB_ModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel		*ct, *cte, *src;
	tkra_zbuf2pixel		*ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
//	int txs, txt;
	int ix0, ix1;
	int idx0, trifl;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		pix=src[idx];
//		dpix=*ct;

		cval=tkra_rgba32upck64(pix);
//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte, *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
	int txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
	while(ct<cte)
	{
		txs=tpos>>16;
		txt=tpos>>48;
		
		ix0=tkra_morton16(txs+0, txt+0);
		ix1=tkra_morton16(txs+1, txt+0);
		ix2=tkra_morton16(txs+0, txt+1);
		ix3=tkra_morton16(txs+1, txt+1);
		ix0&=ymask;
		ix1&=ymask;
		ix2&=ymask;
		ix3&=ymask;
		
		pix0=src[ix0];
		pix1=src[ix1];
		pix2=src[ix2];
		pix3=src[ix3];

		pix0=tkra_rgba32upck64(pix0);
		pix1=tkra_rgba32upck64(pix1);
		pix2=tkra_rgba32upck64(pix2);
		pix3=tkra_rgba32upck64(pix3);

		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

//		dpix=*ct;

//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModUtx3MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	u64 *src;
	tkra_rast2pixel *ct, *cte;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u64 blk;
	u32 xmask, ymask;
	s32 z, zref;
	int ix0, ix1;
//	int txs, txt;
	int pix, dpix, clr, idx, idx0, trifl;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

//		dpix=*ct;

		cval=TKRA_CachedBlkUtx3(src, idx);
//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
	
		z=zpos;

//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModBlUtx3MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte;
	u64 *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref, txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		ix0=tkra_morton16(txs+0, txt+0)&ymask;
		ix1=tkra_morton16(txs+1, txt+0)&ymask;
		ix2=tkra_morton16(txs+0, txt+1)&ymask;
		ix3=tkra_morton16(txs+1, txt+1)&ymask;

		pix0=TKRA_CachedBlkUtx3(src, ix0);
		pix1=TKRA_CachedBlkUtx3(src, ix1);
		pix2=TKRA_CachedBlkUtx3(src, ix2);
		pix3=TKRA_CachedBlkUtx3(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

//		dpix=*ct;

//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel		*ct, *cte, *src;
	tkra_zbuf2pixel		*ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
//	int txs, txt;
	int ix0, ix1;
	int idx0, trifl;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		pix=src[idx];
//		dpix=*ct;

		cval=tkra_rgba32upck64(pix);
//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		pix=tkra_rgba32pck64(cval);
		*ct=pix;

		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte, *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
	int txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
	while(ct<cte)
	{
		txs=tpos>>16;
		txt=tpos>>48;
		
		ix0=tkra_morton16(txs+0, txt+0);
		ix1=tkra_morton16(txs+1, txt+0);
		ix2=tkra_morton16(txs+0, txt+1);
		ix3=tkra_morton16(txs+1, txt+1);
		ix0&=ymask;
		ix1&=ymask;
		ix2&=ymask;
		ix3&=ymask;
		
		pix0=src[ix0];
		pix1=src[ix1];
		pix2=src[ix2];
		pix3=src[ix3];

		pix0=tkra_rgba32upck64(pix0);
		pix1=tkra_rgba32upck64(pix1);
		pix2=tkra_rgba32upck64(pix2);
		pix3=tkra_rgba32upck64(pix3);

		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

//		dpix=*ct;

//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		pix=tkra_rgba32pck64(cval);
		*ct=pix;

		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModUtx3Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	u64 *src;
	tkra_rast2pixel *ct, *cte;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u64 blk;
	u32 xmask, ymask;
	s32 z, zref;
	int ix0, ix1;
//	int txs, txt;
	int pix, dpix, clr, idx, idx0, trifl;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

//		dpix=*ct;

		cval=TKRA_CachedBlkUtx3(src, idx);
//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
	
		pix=tkra_rgba32pck64(cval);
		*ct=pix;

		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_ModBlUtx3Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte;
	u64 *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref, txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		ix0=tkra_morton16(txs+0, txt+0)&ymask;
		ix1=tkra_morton16(txs+1, txt+0)&ymask;
		ix2=tkra_morton16(txs+0, txt+1)&ymask;
		ix3=tkra_morton16(txs+1, txt+1)&ymask;

		pix0=TKRA_CachedBlkUtx3(src, ix0);
		pix1=TKRA_CachedBlkUtx3(src, ix1);
		pix2=TKRA_CachedBlkUtx3(src, ix2);
		pix3=TKRA_CachedBlkUtx3(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

//		dpix=*ct;

//		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		pix=tkra_rgba32pck64(cval);
		*ct=pix;

		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}



void TKRA_DrawSpanB_AlphaModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel		*ct, *cte, *src;
	tkra_zbuf2pixel		*ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref, amod, anmod;
	u32 xmask, ymask;
	s32 z, zref;
//	int txs, txt;
	int ix0, ix1;
	int idx0, trifl;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgba32upck64(pix);
		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);


		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;


		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_AlphaModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte, *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref, amod, anmod;
	u32 xmask, ymask;
	s32 z, zref;
	int txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rast2pixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
	while(ct<cte)
	{
#ifdef __BJX2__
// #if 0
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		ix0=__int_pmortq(txt+0, txs+0);
		ix1=__int_pmortq(txt+0, txs+1);
		ix2=__int_pmortq(txt+1, txs+0);
		ix3=__int_pmortq(txt+1, txs+1);
		ix0=ix0&ymask;
		ix1=ix1&ymask;
		ix2=ix2&ymask;
		ix3=ix3&ymask;

		pix0=src[ix0];
		pix1=src[ix1];
		pix2=src[ix2];
		pix3=src[ix3];

		pix0=__int64_rgb32upck64(pix0);
		pix1=__int64_rgb32upck64(pix1);
		pix2=__int64_rgb32upck64(pix2);
		pix3=__int64_rgb32upck64(pix3);

//		pix0=__int64_blkutx2(pix0, ix0);
//		pix1=__int64_blkutx2(pix1, ix1);
//		pix2=__int64_blkutx2(pix2, ix2);
//		pix3=__int64_blkutx2(pix3, ix3);
		
		dpix=*ct;
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dval=__int64_rgb32upck64(dpix);
		cval=__int64_pmulhw(cval, cpos);
#else
		txs=tpos>>16;
		txt=tpos>>48;
		
		ix0=tkra_morton16(txs+0, txt+0);
		ix1=tkra_morton16(txs+1, txt+0);
		ix2=tkra_morton16(txs+0, txt+1);
		ix3=tkra_morton16(txs+1, txt+1);
		ix0&=ymask;
		ix1&=ymask;
		ix2&=ymask;
		ix3&=ymask;
		
		pix0=src[ix0];
		pix1=src[ix1];
		pix2=src[ix2];
		pix3=src[ix3];

		pix0=tkra_rgba32upck64(pix0);
		pix1=tkra_rgba32upck64(pix1);
		pix2=tkra_rgba32upck64(pix2);
		pix3=tkra_rgba32upck64(pix3);

		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
#endif

		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

#ifdef __BJX2__
		cval=__int64_pmulhw(cval, amod);
		dval=__int64_pmulhw(dval, anmod);
		cval=__int64_paddw(cval, dval);
#else
		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
#endif

		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
#ifdef __BJX2__
			pix=__int64_rgb32pck64(cval);
#else
			pix=tkra_rgba32pck64(cval);
#endif
			*ct=pix;
			*ctz=z;
		}

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_AlphaModUtx3MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	u64 *src;
	tkra_rast2pixel *ct, *cte;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref, amod, anmod;
	u64 blk;
	u32 xmask, ymask;
	s32 z, zref;
	int ix0, ix1;
//	int txs, txt;
	int pix, dpix, clr, idx, idx0, trifl;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		dpix=*ct;

		cval=TKRA_CachedBlkUtx3(src, idx);
		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;

		z=zpos;

//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpanB_AlphaModBlUtx3MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rast2pixel *ct, *cte;
	u64 *src;
	tkra_zbuf2pixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref, amod, anmod;
	u32 xmask, ymask;
	s32 z, zref, txs, txt;
	int idx0, trifl;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ix0=dstc-ctx->screen_rgb;
//	ix1=dstz-ctx->screen_zbuf;
	ct=ctx->screenb_rgb+ix0;
	ctz=ctx->screenb_zbuf+ix0;
	cte=ct+cnt;

	while(ct<cte)
	{
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		ix0=tkra_morton16(txs+0, txt+0)&ymask;
		ix1=tkra_morton16(txs+1, txt+0)&ymask;
		ix2=tkra_morton16(txs+0, txt+1)&ymask;
		ix3=tkra_morton16(txs+1, txt+1)&ymask;

		pix0=TKRA_CachedBlkUtx3(src, ix0);
		pix1=TKRA_CachedBlkUtx3(src, ix1);
		pix2=TKRA_CachedBlkUtx3(src, ix2);
		pix3=TKRA_CachedBlkUtx3(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		z=zpos;
//		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		if(z<=(*ctz))
		{
			pix=tkra_rgba32pck64(cval);
			*ct=pix;
			*ctz=z;
		}

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

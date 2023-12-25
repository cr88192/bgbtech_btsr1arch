// #ifndef __BJX2__
#if 1

u64 tkra_rgba_expand64(u32 a);

void TKRA_DrawSpan_BlendModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=ct[0];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);

		dval=tkra_rgbupck64(dpix);
		cval=Blend(cval, dval);	
		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_BlendModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	s32 z;
	int pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=Blend(cval, dval);	
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_BlendModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	u64 *src;
	tkra_rastpixel *ct, *cte;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u64 blk;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		dpix=ct[0];

//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);

		cval=tkra_pmuluhw(cval, cpos);

		dval=tkra_rgbupck64(dpix);
		cval=Blend(cval, dval);	
		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_BlendModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	s32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		ix0=tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask;
		ix1=tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask;
		ix2=tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask;
		ix3=tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		pix3=TKRA_CachedBlkUtx2(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=Blend(cval, dval);	
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
	}
}


void TKRA_DrawSpan_BlendModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
//	int txs, txt;
	int idx0, trifl;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
#if 1
		z=zpos>>16;
		if(z>(*ctz))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
#endif

		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		cval=Blend(cval, dval);					

		pix=tkra_rgbpck64(cval);
		*ct=pix;

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_BlendModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u32 xmask, ymask;
	s32 z, zref;
	int txs, txt;
	int idx0, trifl;
	int pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
#if 1
		z=zpos>>16;
		if(z>(*ctz))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
#endif

		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		pix0=src[tkra_morton16(txs+0, txt+0)&ymask];
		pix1=src[tkra_morton16(txs+1, txt+0)&ymask];
		pix2=src[tkra_morton16(txs+0, txt+1)&ymask];
		pix3=src[tkra_morton16(txs+1, txt+1)&ymask];

		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		cval=Blend(cval, dval);	
		
		pix=tkra_rgbpck64(cval);
		*ct=pix;

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_BlendModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, cref;
	u64 blk;
	u32 xmask, ymask;
	s32 z, zref;
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

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
#if 1
		z=zpos>>16;
		if(z>(*ctz))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
#endif

		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		dpix=*ct;

		cval=TKRA_CachedBlkUtx2(src, idx);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		cval=Blend(cval, dval);	
		
		pix=tkra_rgbpck64(cval);
		*ct=pix;

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_BlendModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
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

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];
	Blend=(tkra_blendfunc_t)(parm[TKRA_DS_BLEND]);
	ZaTest=(tkra_zatest_t)(parm[TKRA_DS_ZATEST]);

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;
//	cref=0x8000000000000000ULL;
	zref=0;
	cref=tkra_rgba_expand64(ctx->zat_cref);

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
#if 1
		z=zpos>>16;
		if(z>(*ctz))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
#endif

#ifndef TKRA_CHEAP_BILIN
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		ix0=tkra_morton16(txs+0, txt+0)&ymask;
		ix1=tkra_morton16(txs+1, txt+0)&ymask;
		ix2=tkra_morton16(txs+0, txt+1)&ymask;
		ix3=tkra_morton16(txs+1, txt+1)&ymask;

		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		pix3=TKRA_CachedBlkUtx2(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));
#endif

#ifdef TKRA_CHEAP_BILIN
		txs=(tpos>>16);		txt=(tpos>>48);
		ix0=tkra_morton16(txs+0, txt+0)&ymask;
		ix1=tkra_morton16(txs+1, txt+0)&ymask;
		ix2=tkra_morton16(txs+0, txt+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		cval=TKRA_InterpBilinear3Pt_64(pix0, pix1, pix2,
			(u16)tpos, (u16)(tpos>>32));
#endif

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		cval=Blend(cval, dval);	
		
		pix=tkra_rgbpck64(cval);
		*ct=pix;

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

#if 0
void TKRA_DrawSpan_BlendModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_BlendModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_BlendModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_BlendModBlUtx2MortZt(parm, dstc, dstz, cnt);
}
#endif


void TKRA_DrawSpan_BlendClampI(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt,
	void (*DrawSpan)(u64 *parm,
		tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt))
{
	u64	parm2[TKRA_DS_NPARM];

	TKRA_Context *ctx;
	tkra_rastpixel *ct;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep, tepos, tcpos, tcstep, tclpos;
	u64	cpos, cstep, cepos, ccpos;
	u64 zpos, zstep, zepos, zcpos;
	s32 xmask, ymask;
	int txs, txt, texs, text, tcxs, tcxt;
	int trifl, cnt1;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	tepos=tpos+(cnt*tstep);
	txs=(s16)(tpos>>16);	txt=(s16)(tpos>>48);
	texs=(s16)(tepos>>16);	text=(s16)(tepos>>48);

	if(!((txs|txt|texs|text)&(~xmask)))
	{
		/* Span Falls entirely within texture */
		DrawSpan(parm, dstc, dstz, cnt);
		return;
	}

	memcpy(parm2, parm, TKRA_DS_NPARM*sizeof(u64));

	ctx=(TKRA_Context *)(parm[TKRA_DS_CTX]);
	trifl=ctx->svctx->span_trifl;

	tcpos=tpos;
	tcstep=tstep;

	if(trifl&TKRA_TRFL_CLAMPS)
	{
		if((txs<=0) && (texs<=0))
		{
			tcpos &=0xFFFFFFFF00000000ULL;
			tcstep&=0xFFFFFFFF00000000ULL;
			txs=0; texs=0;
		}else if((txs>=xmask) && (texs>=xmask))
		{
			tcpos &=0xFFFFFFFF00000000ULL;
			tcstep&=0xFFFFFFFF00000000ULL;
			tcpos|=xmask;
			txs=xmask; texs=xmask;
		}
	}
	else
	{
		txs&=xmask;
		texs&=xmask;
	}

	if(trifl&TKRA_TRFL_CLAMPT)
	{
		if((txt<=0) && (text<=0))
		{
			tcpos &=0x00000000FFFFFFFFULL;
			tcstep&=0x00000000FFFFFFFFULL;
			txt=0; text=0;
		}else if((txs>=xmask) && (texs>=xmask))
		{
			tcpos &=0x00000000FFFFFFFFULL;
			tcstep&=0x00000000FFFFFFFFULL;
			tcpos|=((u64)xmask)<<48;
			txt=xmask; text=xmask;
		}
	}
	else
	{
		txt&=xmask;
		text&=xmask;
	}

	if(!((txs|txt|texs|text)&(~xmask)))
	{
		parm2[TKRA_DS_TPOS]=tcpos;
		parm2[TKRA_DS_TSTEP]=tcstep;

		/* Span does not cross a clamped edge. */
		DrawSpan(parm2, dstc, dstz, cnt);
		return;
	}
	
	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	if(cnt>32)
	{
		cnt1=cnt>>1;

///		parm2[TKRA_DS_TPOS]=0;
//		parm2[TKRA_DS_TSTEP]=0;
		TKRA_DrawSpan_BlendClampI(parm2,
			dstc, dstz, cnt1,
			DrawSpan);

		tcpos=tpos+tstep*cnt1;
		ccpos=cpos+cstep*cnt1;
		zcpos=zpos+zstep*cnt1;

		parm2[TKRA_DS_TPOS]=tcpos;
		parm2[TKRA_DS_CPOS]=ccpos;
		parm2[TKRA_DS_ZPOS]=zcpos;
		TKRA_DrawSpan_BlendClampI(parm2,
			dstc+cnt1, dstz+cnt1, cnt-cnt1,
			DrawSpan);
		return;
	}
	

//	if(trifl&TKRA_TRFL_CLAMPS)

	if((txs<0) || (txt<0))
	{
		cnt1=0;
		tcpos=tpos;
		ccpos=cpos;
		zcpos=zpos;
		while(cnt1<cnt)
		{
			tcxs=(s16)(tcpos>>16);	tcxt=(s16)(tcpos>>48);
			if((tcxs>=0) && (tcxt>=0))
				break;
			tcpos+=tstep;
			ccpos+=cstep;
			zcpos+=zstep;
			cnt1++;
		}
		
		tclpos=tpos;
		tcstep=tstep;
			
		if(txs<0)
		{
			tclpos&=0xFFFFFFFF00000000ULL;
			tcstep&=0xFFFFFFFF00000000ULL;
		}

		if(txt<0)
		{
			tclpos&=0x00000000FFFFFFFFULL;
			tcstep&=0x00000000FFFFFFFFULL;
		}
		
		
		if((cnt-cnt1)<4)
		{
			/* YOLO Clip */
			parm2[TKRA_DS_TPOS]=tclpos;
			parm2[TKRA_DS_TSTEP]=tcstep;
			DrawSpan(parm2, dstc, dstz, cnt);
			return;
		}

		if(cnt1>0)
		{
			/* Clip off lead-in */

			parm2[TKRA_DS_TPOS]=tclpos;
			parm2[TKRA_DS_TSTEP]=tcstep;
			DrawSpan(parm2, dstc, dstz, cnt1);

			parm2[TKRA_DS_TPOS]=tcpos;
			parm2[TKRA_DS_TSTEP]=tstep;
			parm2[TKRA_DS_CPOS]=ccpos;
			parm2[TKRA_DS_ZPOS]=zcpos;
			TKRA_DrawSpan_BlendClampI(parm2,
				dstc+cnt1, dstz+cnt1, cnt-cnt1,
				DrawSpan);
			return;
		}
		
		__debugbreak();
		return;
	}

//	if((texs>xmask) || (text>xmask))
	if(1)
	{
		cnt1=0;
		tcpos=tpos;
		tclpos=tpos;
		ccpos=cpos;
		zcpos=zpos;
		while(cnt1<cnt)
		{
			tcxs=(s16)(tcpos>>16);	tcxt=(s16)(tcpos>>48);
			if((tcxs>xmask) || (tcxt>xmask))
				break;
			tclpos=tcpos;
			tcpos+=tstep;
			ccpos+=cstep;
			zcpos+=zstep;
			cnt1++;
		}

		if(cnt1<4)
		{
			tcxs=(s16)(tcpos>>16);
			tcxt=(s16)(tcpos>>48);
			if(tcxs<0)		tcxs=0;
			if(tcxt<0)		tcxt=0;
			if(tcxs>xmask)	tcxs=xmask;
			if(tcxt>xmask)	tcxt=xmask;
			tcpos=(((u64)tcxt)<<48) | (((u64)tcxs)<<16);

			parm2[TKRA_DS_TPOS]=tpos;
			parm2[TKRA_DS_TSTEP]=0;
			DrawSpan(parm2, dstc, dstz, cnt1);
			return;
		}
		
		/* Draw Interior Part */

//		parm2[TKRA_DS_TPOS]=tpos;
//		parm2[TKRA_DS_TSTEP]=tstep;
		DrawSpan(parm2, dstc, dstz, cnt1);
		
		if((cnt-cnt1)<4)
			return;

//		parm2[TKRA_DS_TPOS]=tclpos;
		parm2[TKRA_DS_TPOS]=tcpos;
		parm2[TKRA_DS_TSTEP]=tstep;
		parm2[TKRA_DS_CPOS]=ccpos;
		parm2[TKRA_DS_ZPOS]=zcpos;
//		DrawSpan(parm2,
//			dstc+cnt1, dstz+cnt1, cnt-cnt1);
		TKRA_DrawSpan_BlendClampI(parm2,
			dstc+cnt1, dstz+cnt1, cnt-cnt1,
			DrawSpan);
		return;
	}
}




void TKRA_DrawSpan_BlendModTexMortClamp(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModTexMort);
}

void TKRA_DrawSpan_BlendModBlTexMortClamp(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModBlTexMort);
}


void TKRA_DrawSpan_BlendModUtx2MortClamp(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModUtx2Mort);
}

void TKRA_DrawSpan_BlendModBlUtx2MortClamp(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModBlUtx2Mort);
}


void TKRA_DrawSpan_BlendModTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModTexMortZt);
}

void TKRA_DrawSpan_BlendModBlTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModBlTexMortZt);
}


void TKRA_DrawSpan_BlendModUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModUtx2MortZt);
}

void TKRA_DrawSpan_BlendModBlUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_BlendModBlUtx2MortZt);
}

#endif

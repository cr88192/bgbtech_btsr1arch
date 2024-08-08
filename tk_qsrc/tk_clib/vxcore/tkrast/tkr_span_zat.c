

void TKRA_DrawSpan_ZatModTexMortZt(u64 *parm,
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
#if 0
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

#if 0
		if(trifl&(TKRA_TRFL_CLAMPS|TKRA_TRFL_CLAMPT))
		{
			if(idx!=idx0)
			{
				txs=(s16)(tpos>>16);
				txt=(s16)(tpos>>48);
				if((trifl&TKRA_TRFL_CLAMPS) && (txs&(~xmask)))
					txs=(txs<0)?0:xmask;
				if((trifl&TKRA_TRFL_CLAMPT) && (txt&(~xmask)))
					txt=(txt<0)?0:xmask;
				idx=tkra_morton16(txs, txt)&ymask;
			}
		}
#endif

//		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

		cval=Blend(cval, dval);	

		z=zpos>>16;
		if(!ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
				
//		cval=Blend(cval, dval);	
		pix=tkra_rgbpck64(cval);
		*ct=pix;
		*ctz=z;

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ZatModBlTexMortZt(u64 *parm,
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
#if 0
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

#if 1
		txs=(s16)(tpos>>16);
		txt=(s16)(tpos>>48);
		pix0=src[tkra_morton16(txs+0, txt+0)&ymask];
		pix1=src[tkra_morton16(txs+1, txt+0)&ymask];
		pix2=src[tkra_morton16(txs+0, txt+1)&ymask];
		pix3=src[tkra_morton16(txs+1, txt+1)&ymask];
#endif

#if 0
		if(trifl&(TKRA_TRFL_CLAMPS|TKRA_TRFL_CLAMPT))
		{
			idx0=tkra_morton16(txs, txt);
			idx=idx0&ymask;

			if(idx!=idx0)
			{
				if((trifl&TKRA_TRFL_CLAMPS) && (txs&(~xmask)))
					txs=(txs<0)?0:xmask;
				if((trifl&TKRA_TRFL_CLAMPT) && (txt&(~xmask)))
					txt=(txt<0)?0:xmask;
				idx=tkra_morton16(txs, txt)&ymask;
				pix0=src[tkra_morton16(txs, txt)&ymask];
				pix1=pix0; pix2=pix0; pix3=pix1;
			}
		}
#endif

//		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
//		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
//		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
//		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		cval=Blend(cval, dval);	

		z=zpos>>16;
		if(!ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}
				
//		cval=Blend(cval, dval);	
		
		pix=tkra_rgbpck64(cval);
		*ct=pix;
		*ctz=z;

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ZatModUtx2MortZt(u64 *parm,
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
#ifdef __BJX2__
		idx0=__int_pmortq(tpos>>48, tpos>>16);
		idx=idx0&ymask;
		blk=src[idx>>4];
		dpix=*ct;
		cval=__int64_blkutx2(blk, idx);
		dval=__int64_rgb5upck64(dpix);
		cval=__int64_pmulhw(cval, cpos);
#else
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		dpix=*ct;

		cval=TKRA_CachedBlkUtx2(src, idx);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
#endif
	
		cval=Blend(cval, dval);	

		z=zpos>>16;

#if 0
		if(!ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		{
			ctz++;
			ct++;
			tpos+=tstep;
			cpos+=cstep;
			zpos+=zstep;
			continue;
		}

//		cval=Blend(cval, dval);	
		
#ifdef __BJX2__
		pix=__int64_rgb5pck64(cval);
//		pix=tkra_rgbpck64(cval);
#else
		pix=tkra_rgbpck64(cval);
#endif
		*ct=pix;
		*ctz=z;
#endif

#if 1
		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		{
#ifdef __BJX2__
			pix=__int64_rgb5pck64(cval);
#else
			pix=tkra_rgbpck64(cval);
#endif
			*ct=pix;
			*ctz=z;
		}
#endif

		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ZatModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_Context *ctx;
	tkra_blendfunc_t	Blend;
	tkra_zatest_t		ZaTest;
	tkra_rastpixel *ct, *cte;
	u64 *src;
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

		pix0=src[ix0>>4];
		pix1=src[ix1>>4];
		pix2=src[ix2>>4];
		pix3=src[ix3>>4];

		pix0=__int64_blkutx2(pix0, ix0);
		pix1=__int64_blkutx2(pix1, ix1);
		pix2=__int64_blkutx2(pix2, ix2);
		pix3=__int64_blkutx2(pix3, ix3);
		dpix=*ct;
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dval=__int64_rgb5upck64(dpix);
		cval=__int64_pmulhw(cval, cpos);
#else
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

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
#endif
		
		cval=Blend(cval, dval);	

#if 1
		z=zpos>>16;
		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
		{
#ifdef __BJX2__
			pix=__int64_rgb5pck64(cval);
#else
			pix=tkra_rgbpck64(cval);
#endif
			*ct=pix;
			*ctz=z;
		}
				
#endif

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

#if 0
void TKRA_DrawSpan_ZatModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_ZatModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_ZatModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_ZatModBlUtx2MortZt(parm, dstc, dstz, cnt);
}
#endif


void TKRA_DrawSpan_ZatModTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_ZatModTexMortZt);
}

void TKRA_DrawSpan_ZatModBlTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_ZatModBlTexMortZt);
}


void TKRA_DrawSpan_ZatModUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_ZatModUtx2MortZt);
}

void TKRA_DrawSpan_ZatModBlUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_ZatModBlUtx2MortZt);
}



void TKRA_DrawSpanB_ZatModTexMortZt(u64 *parm,
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

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgba32upck64(pix);
		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);

//		cval&=~(0xFFULL<<48);
//		cval|=(0xFFULL<<56);

		cval=Blend(cval, dval);	

//		z=zpos>>16;
		z=zpos;
		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
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

void TKRA_DrawSpanB_ZatModBlTexMortZt(u64 *parm,
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

//	TKRA_DrawSpanB_ZatModTexMortZt(parm, dstc, dstz, cnt);
//	return;

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
//		txs=(s16)(tpos>>16);
//		txt=(s16)(tpos>>48);
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

//		pix0&=~(0xFFULL<<48);
//		pix1&=~(0xFFULL<<48);
//		pix2&=~(0xFFULL<<48);
//		pix3&=~(0xFFULL<<48);
//		pix0|=(0xFFULL<<56);
//		pix1|=(0xFFULL<<56);
//		pix2|=(0xFFULL<<56);
//		pix3|=(0xFFULL<<56);

		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		cval=Blend(cval, dval);	

//		z=zpos>>16;
		z=zpos;
		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
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

void TKRA_DrawSpanB_ZatModUtx3MortZt(u64 *parm,
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

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
	while(ct<cte)
	{
		idx0=tkra_morton16(tpos>>16, tpos>>48);
		idx=idx0&ymask;

		dpix=*ct;

		cval=TKRA_CachedBlkUtx3(src, idx);
		dval=tkra_rgba32upck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
	
		cval=Blend(cval, dval);	

//		z=zpos>>16;
		z=zpos;

		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
//		if(1)
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

void TKRA_DrawSpanB_ZatModBlUtx3MortZt(u64 *parm,
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

//	ct=dstc; cte=ct+cnt;
//	ctz=dstz;
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
		
		cval=Blend(cval, dval);	

//		z=zpos>>16;
		z=zpos;
		if(ZaTest(z, *ctz, zref, &z, cval, dval, cref, &cval))
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


void TKRA_DrawSpanB_ZatModTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
//	TKRA_DrawSpanB_ZatModTexMortZt(parm, dstc, dstz, cnt);
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpanB_ZatModTexMortZt);
}

void TKRA_DrawSpanB_ZatModBlTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
//	TKRA_DrawSpanB_ZatModBlTexMortZt(parm, dstc, dstz, cnt);
//	TKRA_DrawSpanB_ZatModTexMortZt(parm, dstc, dstz, cnt);
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpanB_ZatModBlTexMortZt);
}


void TKRA_DrawSpanB_ZatModUtx3MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpanB_ZatModUtx3MortZt);
}

void TKRA_DrawSpanB_ZatModBlUtx3MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpanB_ZatModBlUtx3MortZt);
}


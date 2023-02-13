void TKRA_DrawSpan_LmapModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_LmapModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_LmapModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_LmapModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_LmapModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	s32 z;
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

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=tkra_pmuluhw(cval, dval);
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_LmapModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
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

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
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

		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=tkra_pmuluhw(cval, dval);
		
		pix=tkra_rgbpck64(cval);
		*ct=pix;

		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_LmapModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u64 blk;
	u32 xmask, ymask;
	s32 z;
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

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		dpix=*ct;

		cval=TKRA_CachedBlkUtx2(src, idx);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=tkra_pmuluhw(cval, dval);
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_LmapModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
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
#endif

#ifdef TKRA_CHEAP_BILIN
		ix2=(tpos>>16);		ix3=(tpos>>48);
		ix0=tkra_morton16(ix2+0, ix3+0)&ymask;
		ix1=tkra_morton16(ix2+1, ix3+0)&ymask;
		ix2=tkra_morton16(ix2+0, ix3+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		cval=TKRA_InterpBilinear3Pt_64(pix0, pix1, pix2,
			(u16)tpos, (u16)(tpos>>32));
#endif

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		cval=tkra_pmuluhw(cval, dval);
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		ctz++;	ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_LmapModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_LmapModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_LmapModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_LmapModBlUtx2MortZt(parm, dstc, dstz, cnt);
}


void TKRA_DrawSpan_LmapModTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_LmapModTexMortZt);
}

void TKRA_DrawSpan_LmapModBlTexMortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_LmapModBlTexMortZt);
}


void TKRA_DrawSpan_LmapModUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_LmapModUtx2MortZt);
}

void TKRA_DrawSpan_LmapModBlUtx2MortClampZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_BlendClampI(parm, dstc, dstz, cnt, 
		TKRA_DrawSpan_LmapModBlUtx2MortZt);
}

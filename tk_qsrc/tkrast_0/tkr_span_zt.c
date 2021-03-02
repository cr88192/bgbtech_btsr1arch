#ifndef __BJX2__

void TKRA_DrawSpan_ModTexZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AlphaModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AtestModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AlphaModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AtestModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_Zbuf(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);
void TKRA_DrawSpan_ZbNul(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);

u64 TKRA_InterpBilinear(
	u16 px0, u16 px1, u16 px2, u16 px3,
	u16 xfrac, u16 yfrac);
u64 TKRA_InterpBilinear64(
	u64 px0, u64 px1, u64 px2, u64 px3,
	u16 xfrac, u16 yfrac);

void TKRA_DrawSpan_ModTexZbuf(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	pos, step;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	pos=parm[TKRA_DS_TPOS];
	step=parm[TKRA_DS_TSTEP];

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
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		pos+=step;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

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
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
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
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
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

void TKRA_DrawSpan_AtestModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

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

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if((z<=(*ctz)) && ((cval>>63)&1))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, clr, idx;

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
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));
		cval=tkra_pmuluhw(cval, cpos);
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

void TKRA_DrawSpan_AlphaModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
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
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
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

void TKRA_DrawSpan_AtestModBlTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, clr, idx;

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
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if((z<=(*ctz)) && ((cval>>63)&1))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

#if 1
void TKRA_DrawSpan_ModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, blk;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

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
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];
//		blk=src[idx>>4];

//		cval=tkra_rgbupck64(pix);
//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModUtx2MortZt(u64 *parm,
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
	u32 z;
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
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];
//		blk=src[idx>>4];
		dpix=*ct;

//		cval=tkra_rgbupck64(pix);
//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
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

void TKRA_DrawSpan_AtestModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, blk;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

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
		cval=TKRA_CachedBlkUtx2(src, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if((z<=(*ctz)) && ((cval>>63)&1))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}
#endif

#if 1
void TKRA_DrawSpan_ModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, clr, idx;

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

		cval=tkra_pmuluhw(cval, cpos);
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

void TKRA_DrawSpan_AlphaModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
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
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
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

void TKRA_DrawSpan_AtestModBlUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, clr, idx;

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

		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz) && ((cval>>63)&1))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}
#endif

void TKRA_DrawSpan_ZbNul(
	tkra_zbufpixel *dstz, int cnt,
	u64 izpos, u64 izstep)
{
}

void TKRA_DrawSpan_Zbuf(
	tkra_zbufpixel *dstz, int cnt,
	u64 izpos, u64 izstep)
{
	tkra_zbufpixel *ct, *cte;
	u64	zpos, zstep;
	int pix, idx;

	zpos=izpos; zstep=izstep;
	ct=dstz; cte=ct+cnt;
	while(ct<cte)
	{
		pix=zpos>>16;
		if(pix<(*ct))
			*ct=pix;
		ct++;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_ModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_AlphaModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_AlphaModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_AtestModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_AtestModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_ModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_ModBlUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_AlphaModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_AlphaModBlUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_AtestModBlUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_AtestModBlUtx2MortZt(parm, dstc, dstz, cnt);
}


u64 TKRA_InterpBilinear(
	u16 px0, u16 px1, u16 px2, u16 px3,
	u16 xfrac, u16 yfrac)
{
	u64 cv0, cv1, cv2, cv3, cv4, cv5, cv6;
	u64 cxf, cyf, cxnf, cynf;

	cv0=tkra_rgbupck64(px0);
	cv1=tkra_rgbupck64(px1);
	cv2=tkra_rgbupck64(px2);
	cv3=tkra_rgbupck64(px3);

	cxf=xfrac;	cxf|=cxf<<16;	cxf|=cxf<<32;
	cyf=yfrac;	cyf|=cyf<<16;	cyf|=cyf<<32;
	cxnf=~cxf;
	cynf=~cyf;

	cv4=tkra_pmuluhw(cv0, cxnf)+tkra_pmuluhw(cv1, cxf);
	cv5=tkra_pmuluhw(cv2, cxnf)+tkra_pmuluhw(cv3, cxf);
	cv6=tkra_pmuluhw(cv4, cynf)+tkra_pmuluhw(cv5, cyf);
	return(cv6);
}

u64 TKRA_InterpBilinear64(
	u64 px0, u64 px1, u64 px2, u64 px3,
	u16 xfrac, u16 yfrac)
{
	u64 cv4, cv5, cv6;
	u64 cxf, cyf, cxnf, cynf;

	cxf=xfrac;	cxf|=cxf<<16;	cxf|=cxf<<32;
	cyf=yfrac;	cyf|=cyf<<16;	cyf|=cyf<<32;
	cxnf=~cxf;
	cynf=~cyf;

	cv4=tkra_pmuluhw(px0, cxnf)+tkra_pmuluhw(px1, cxf);
	cv5=tkra_pmuluhw(px2, cxnf)+tkra_pmuluhw(px3, cxf);
	cv6=tkra_pmuluhw(cv4, cynf)+tkra_pmuluhw(cv5, cyf);
	return(cv6);
}

#endif

#ifndef __BJX2__

void BTMRA_DrawCol_DirClr(u64 *parm,
	btmra_rastpixel *dstc, btmra_zbufpixel *dstz, int cnt, int stride)
{
	btmra_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u64	cpos, cstep;
	u32 xmask, ymask;
	int pix, idx;

//	cpos=ctx->ds_cpos;
//	cstep=ctx->ds_cstep;
	cpos=parm[BTMRA_DS_CPOS];
	cstep=parm[BTMRA_DS_CSTEP];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		pix=src[idx];
		pix=
			((cpos>>33)&0x7C00) |
			((cpos>>22)&0x03E0) |
			((cpos>>11)&0x001F);
		*ct=pix;
		ct+=stride;
		cpos+=cstep;
	}
}

void BTMRA_DrawCol_DirTexMort(u64 *parm,
	btmra_rastpixel *dstc, btmra_zbufpixel *dstz, int cnt, int stride)
{
	btmra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u32 xmask, ymask;
	int pix, idx;

//	pos=ctx->ds_pos;
//	step=ctx->ds_step;
	tpos=parm[BTMRA_DS_TPOS];
	tstep=parm[BTMRA_DS_TSTEP];

//	src=ctx->tex_img;
	src=(btmra_rastpixel *)(parm[BTMRA_DS_TEXIMG]);
//	xmask=ctx->tex_xmask;
//	ymask=ctx->tex_ymask;

	xmask=parm[BTMRA_DS_XMASK];
	ymask=parm[BTMRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=btmra_morton8(((pos>>16)&xmask), ((pos>>48)&xmask));
//		idx=btmra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=btmra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		*ct=pix;
		ct+=stride;
		tpos+=tstep;
	}
}

void BTMRA_DrawCol_ModTexMort(u64 *parm,
	btmra_rastpixel *dstc, btmra_zbufpixel *dstz, int cnt, int stride)
{
	btmra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval;
	u32 xmask, ymask;
	int pix, clr, idx;

	tpos=parm[BTMRA_DS_TPOS];
	tstep=parm[BTMRA_DS_TSTEP];

	cpos=parm[BTMRA_DS_CPOS];
	cstep=parm[BTMRA_DS_CSTEP];

	src=(btmra_rastpixel *)(parm[BTMRA_DS_TEXIMG]);
	xmask=parm[BTMRA_DS_XMASK];
	ymask=parm[BTMRA_DS_YMASK];

	ct=dstc; cte=ct+(cnt*stride);
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=btmra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=btmra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=btmra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];

		cval=btmra_rgbupck64(pix);
		cval=btmra_pmuluhw(cval, cpos);
		pix=btmra_rgbpck64(cval);

		*ct=pix;
		ct+=stride;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void BTMRA_DrawCol_AlphaModTexMort(u64 *parm,
	btmra_rastpixel *dstc, btmra_zbufpixel *dstz, int cnt, int stride)
{
	btmra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[BTMRA_DS_TPOS];
	tstep=parm[BTMRA_DS_TSTEP];

	cpos=parm[BTMRA_DS_CPOS];
	cstep=parm[BTMRA_DS_CSTEP];

	src=(btmra_rastpixel *)(parm[BTMRA_DS_TEXIMG]);
	xmask=parm[BTMRA_DS_XMASK];
	ymask=parm[BTMRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=btmra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=btmra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=btmra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=ct[0];

		cval=btmra_rgbupck64(pix);
		cval=btmra_pmuluhw(cval, cpos);

		dval=btmra_rgbupck64(dpix);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=btmra_pmuluhw(cval, amod);
		dval=btmra_pmuluhw(dval, anmod);
		cval+=dval;

		pix=btmra_rgbpck64(cval);

		*ct=pix;
		ct+=stride;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void BTMRA_DrawCol_AtestModTexMort(u64 *parm,
	btmra_rastpixel *dstc, btmra_zbufpixel *dstz, int cnt, int stride)
{
	btmra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[BTMRA_DS_TPOS];
	tstep=parm[BTMRA_DS_TSTEP];

	cpos=parm[BTMRA_DS_CPOS];
	cstep=parm[BTMRA_DS_CSTEP];

	src=(btmra_rastpixel *)(parm[BTMRA_DS_TEXIMG]);
	xmask=parm[BTMRA_DS_XMASK];
	ymask=parm[BTMRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=btmra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=btmra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=btmra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
//		dpix=ct[0];

		cval=btmra_rgbupck64(pix);
		cval=btmra_pmuluhw(cval, cpos);
		pix=btmra_rgbpck64(cval);

//		dval=btmra_rgbupck64(dpix);

		if((cval>>63)&1)
			*ct=pix;

		ct+=stride;
		tpos+=tstep;
		cpos+=cstep;
	}
}

#endif

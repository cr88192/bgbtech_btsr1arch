u16 tkra_spanrcptab[1024];

int TKRA_InitSpanRcp(void)
{
	int i, j, k;
	
	for(i=2; i<1024; i++)
	{
		tkra_spanrcptab[i]=65536/i;
	}
	tkra_spanrcptab[0]=65535;
	tkra_spanrcptab[1]=65535;
	return(0);
}

int TKRA_SpanRcp(int cnt)
{
	if(cnt<1024)
		return(tkra_spanrcptab[cnt]);
	/* Placeholder */
	return(65536/cnt);
}

#ifdef __BJX2__
s64 __int32_dmuls(s32 a, s32 b);
u64 __int32_dmulu(u32 a, u32 b);
#define tkra_dmuls(a, b)	__int32_dmuls(a, b)
#define tkra_dmulu(a, b)	__int32_dmulu(a, b)
#else
#define tkra_dmuls(a, b)	(((s64)(a))*(b))
#define tkra_dmulu(a, b)	(((u64)(a))*(b))
#endif

u64 TKRA_ScaleTexStepRcp(u64 tdist, u16 rcp)
{
	u32 tdy, tdx;
	
	tdy=(tdist>>32);
	tdx=(tdist    );
//	tdy=(((u64)tdy)*rcp)>>16;
//	tdx=(((u64)tdx)*rcp)>>16;

	tdy=tkra_dmulu(tdy, rcp)>>16;
	tdx=tkra_dmulu(tdx, rcp)>>16;

	return((((u64)tdy)<<32)|tdx);
}

#ifdef __BJX2__
u64 TKRA_CalcTexStepRcp(u64 tdst, u64 tsrc, u16 rcp);

__asm {
TKRA_CalcTexStepRcp:
	SUB		R4, R5, R7
	SHAD.Q	R7, -32, R17	|	EXTS.L	R7, R16
								DMULS.L	R16, R6, R16
								DMULS.L	R17, R6, R17
	SHAD.Q	R16, -16, R16	|	SHAD.Q	R17, -16, R17
	MOVLD	R17, R16, R2
	RTSU
};

#else
u64 TKRA_CalcTexStepRcp(u64 tdst, u64 tsrc, u16 rcp)
{
	s32 tdy, tdx;

	tdy=(tdst>>32)-(tsrc>>32);
	tdx=(tdst    )-(tsrc    );
	
//	tdy=(tdist>>32);
//	tdx=(tdist    );
//	tdy=(((s64)tdy)*rcp)>>16;
//	tdx=(((s64)tdx)*rcp)>>16;
	tdy=tkra_dmuls(tdy, rcp)>>16;
	tdx=tkra_dmuls(tdx, rcp)>>16;

	return((((u64)tdy)<<32)|((u32)tdx));
}
#endif

#ifdef __BJX2__
// #if 0
u64 TKRA_ScaleClrStepRcp(u64 tdist, u16 rcp);
u64 TKRA_CalcClrStepRcp(u64 cdst, u64 csrc, u16 rcp);

__asm {
TKRA_ScaleClrStepRcp:
	PSHUF.W		R5, 0, R6		//(0000)
	PMULU.HW	R4, R6, R2
	RTSU

TKRA_CalcClrStepRcp:
	MOV		0xFFFEFFFEFFFEFFFE, R3
	AND		R4, R3, R4	|	AND		R5, R3, R5
	SHLD.Q	R4, -1, R4	|	SHLD.Q	R5, -1, R5
	SUB			R4, R5, R2
	PSHUF.W		R6, 0, R7		//(0000)
	PMULS.HW	R2, R7, R2
	SHLD.Q		R2, 1, R2
	RTSU
};
#else
u64 TKRA_ScaleClrStepRcp(u64 tdist, u16 rcp)
{
	u32 tdr, tdg, tdb, tda;
	
	tda=(u16)(tdist>>48);	tdr=(u16)(tdist>>32);
	tdg=(u16)(tdist>>16);	tdb=(u16)(tdist    );
	tda=(tda*rcp)>>16;		tdr=(tdr*rcp)>>16;
	tdg=(tdg*rcp)>>16;		tdb=(tdb*rcp)>>16;
	return(
		(((u64)tda)<<48)|
		(((u64)tdr)<<32)|
		(((u64)tdg)<<16)|
		(      tdb     ));
}

u64 TKRA_CalcClrStepRcp(u64 cdst, u64 csrc, u16 rcp)
{
	int tdr, tdg, tdb, tda;
	
	tda=((cdst>>48)&65535)-((csrc>>48)&65535);
	tdr=((cdst>>32)&65535)-((csrc>>32)&65535);
	tdg=((cdst>>16)&65535)-((csrc>>16)&65535);
	tdb=((cdst>> 0)&65535)-((csrc>> 0)&65535);
	
//	tda=(u16)(tdist>>48);	tdr=(u16)(tdist>>32);
//	tdg=(u16)(tdist>>16);	tdb=(u16)(tdist    );
	tda=(tda*rcp)>>16;		tdr=(tdr*rcp)>>16;
	tdg=(tdg*rcp)>>16;		tdb=(tdb*rcp)>>16;
	tda=(u16)tda;	tdr=(u16)tdr;
	tdg=(u16)tdg;	tdb=(u16)tdb;
	return(
		(((u64)tda)<<48)|
		(((u64)tdr)<<32)|
		(((u64)tdg)<<16)|
		(      tdb     ));
}

#endif

/*
Walk along a pair of edges, calling DrawSpan for each span.
Default Version (Tex+Color+Z)
 */
void TKRA_WalkEdges_Dfl(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt)
{
	void (*DrawSpan)(u64 *parm,
		tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

	u64	dsparm[TKRA_DS_NPARM];
	tkra_rastpixel *scr_cb, *cb0;
	tkra_zbufpixel *scr_zb, *zb0;
	tkra_rastpixel *cb;
	tkra_zbufpixel *zb;
	int xpos_l, xpos_r, xstep_l, xstep_r;
	int zpos_l, zpos_r, zstep_l, zstep_r;
	u64 tpos_l, tpos_r, tstep_l, tstep_r;
	u64 tstep_d, tstep_c;

	u64 cpos_l, cpos_r, cstep_l, cstep_r;
	u64 cstep_d, cstep_c;
	int zstep_d, zstep_c;
	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		y, ymax, x0, x1, x2, xcnt, xrcp;
	int		xshl, yshl;
	int		scr_xs, scr_ys;

	DrawSpan	= ctx->DrawSpan;
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;

//	clip_x0	= 0;
//	clip_x1	= scr_xs;
//	clip_y0	= 0;
//	clip_y1	= scr_ys;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

	if(cnt<=0)
		return;
	if(ytop>=clip_y1)
		return;
	ymax=ytop+cnt;
	if(ymax<=clip_y0)
		return;

	dsparm[TKRA_DS_CTX]=(u64)(ctx);
	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);

//	dsparm[TKRA_DS_XMASK]=(1<<ctx->tex_xshl)-1;
//	dsparm[TKRA_DS_YMASK]=(1<<ctx->tex_yshl)-1;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;
	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
//	dsparm[TKRA_DS_YMASK]=((1<<yshl)-1)<<xshl;
//	dsparm[TKRA_DS_YMASK]=(1<<yshl)-1;
	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;

	xpos_l	= edge_l[TKRA_ES_XPOS];
	xpos_r	= edge_r[TKRA_ES_XPOS];
	xstep_l	= edge_l[TKRA_ES_XSTEP];
	xstep_r	= edge_r[TKRA_ES_XSTEP];
	
	tpos_l	= edge_l[TKRA_ES_TPOS];
	tpos_r	= edge_r[TKRA_ES_TPOS];
	tstep_l	= edge_l[TKRA_ES_TSTEP];
	tstep_r	= edge_r[TKRA_ES_TSTEP];
	
	cpos_l	= edge_l[TKRA_ES_CPOS];
	cpos_r	= edge_r[TKRA_ES_CPOS];
	cstep_l	= edge_l[TKRA_ES_CSTEP];
	cstep_r	= edge_r[TKRA_ES_CSTEP];
	
	zpos_l	= edge_l[TKRA_ES_ZPOS];
	zpos_r	= edge_r[TKRA_ES_ZPOS];
	zstep_l	= edge_l[TKRA_ES_ZSTEP];
	zstep_r	= edge_r[TKRA_ES_ZSTEP];
	
	cb0=scr_cb+(ytop*scr_xs);
	zb0=scr_zb+(ytop*scr_xs);

	ymax=ytop+cnt;
	for(y=ytop; y<ymax; y++)
	{
		x0=(xpos_l      )>>16;
//		x1=xpos_r>>16;
		x1=(xpos_r+65535)>>16;
		xcnt=x1-x0;
//		if(xcnt<1)
		if((xcnt<1) || (y<clip_y0) || (y>=clip_y1))
		{
			xpos_l+=xstep_l;	xpos_r+=xstep_r;
			tpos_l+=tstep_l;	tpos_r+=tstep_r;
			cpos_l+=cstep_l;	cpos_r+=cstep_r;
			zpos_l+=zstep_l;	zpos_r+=zstep_r;
			cb0+=scr_xs;
			zb0+=scr_xs;
			continue;
		}

		xrcp=TKRA_SpanRcp(xcnt+1);
		
//		tstep_d=tpos_r-tpos_l;
//		tstep_c=TKRA_ScaleTexStepRcp(tstep_d, xrcp);
		tstep_c=TKRA_CalcTexStepRcp(tpos_r, tpos_l, xrcp);

//		cstep_d=cpos_r-cpos_l;
//		cstep_c=TKRA_ScaleClrStepRcp(cstep_d, xrcp);
		cstep_c=TKRA_CalcClrStepRcp(cpos_r, cpos_l, xrcp);

		dsparm[TKRA_DS_TPOS		]=tpos_l;
		dsparm[TKRA_DS_TSTEP	]=tstep_c;
		dsparm[TKRA_DS_CPOS		]=cpos_l;
		dsparm[TKRA_DS_CSTEP	]=cstep_c;

		zstep_d=zpos_r-zpos_l;
		zstep_c=tkra_dmuls(zstep_d, xrcp)>>16;
		dsparm[TKRA_DS_ZPOS		]=zpos_l;
		dsparm[TKRA_DS_ZSTEP	]=zstep_c;

		if((y>=clip_y0) && (y<clip_y1))
		{
			if(x0<clip_x0)
			{
				x2=clip_x0-x0;
				dsparm[TKRA_DS_TPOS		]+=tstep_c*x2;
				dsparm[TKRA_DS_CPOS		]+=cstep_c*x2;
				dsparm[TKRA_DS_ZPOS		]+=zstep_c*x2;
				x0+=x2;
			}
			
			if(x1>=clip_x1)
			{
				xcnt=clip_x1-x0;
			}
			
			cb=cb0+x0;
			zb=zb0+x0;
			if(xcnt>0)
			{
				DrawSpan(dsparm, cb, zb, xcnt);
			}
		}
		
		xpos_l+=xstep_l;	xpos_r+=xstep_r;
		tpos_l+=tstep_l;	tpos_r+=tstep_r;
		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		zpos_l+=zstep_l;	zpos_r+=zstep_r;
		cb0+=scr_xs;
		zb0+=scr_xs;
	}

	edge_l[TKRA_ES_XPOS] = xpos_l;
	edge_r[TKRA_ES_XPOS] = xpos_r;
	edge_l[TKRA_ES_TPOS] = tpos_l;
	edge_r[TKRA_ES_TPOS] = tpos_r;
	edge_l[TKRA_ES_CPOS] = cpos_l;
	edge_r[TKRA_ES_CPOS] = cpos_r;
	edge_l[TKRA_ES_ZPOS] = zpos_l;
	edge_r[TKRA_ES_ZPOS] = zpos_r;
}

/* Half-Resolution Z-Buffer */
void TKRA_WalkEdges_HZbuf(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt)
{
	void (*DrawSpan)(u64 *parm,
		tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
	void (*DrawSpanZ)(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);

	u64	dsparm[TKRA_DS_NPARM];
	tkra_rastpixel *scr_cb, *cb0;
	tkra_zbufpixel *scr_zb, *zb0;
	tkra_rastpixel *cb;
	tkra_zbufpixel *zb;
	int xpos_l, xpos_r, xstep_l, xstep_r;
	int zpos_l, zpos_r, zstep_l, zstep_r;
	u64 tpos_l, tpos_r, tstep_l, tstep_r;
	u64 tstep_d, tstep_c;

	u64 cpos_l, cpos_r, cstep_l, cstep_r;
	u64 cstep_d, cstep_c;
	int zstep_d, zstep_c;
	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		y, ymax, x0, x1, x2, xcnt, xrcp;
	int		xshl, yshl;
	int		scr_xs, scr_ys, scr_zxs;

	DrawSpan	= ctx->DrawSpanHZt;
	DrawSpanZ	= ctx->DrawSpanZb;
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;
	scr_zxs = scr_xs>>1;

//	clip_x0	= 0;
//	clip_x1	= scr_xs;
//	clip_y0	= 0;
//	clip_y1	= scr_ys;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

	if(cnt<=0)
		return;
	if(ytop>=clip_y1)
		return;
	ymax=ytop+cnt;
	if(ymax<=clip_y0)
		return;

	dsparm[TKRA_DS_CTX]=(u64)(ctx);
	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);

//	dsparm[TKRA_DS_XMASK]=(1<<ctx->tex_xshl)-1;
//	dsparm[TKRA_DS_YMASK]=(1<<ctx->tex_yshl)-1;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;
	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
//	dsparm[TKRA_DS_YMASK]=((1<<yshl)-1)<<xshl;
//	dsparm[TKRA_DS_YMASK]=(1<<yshl)-1;
	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;

	xpos_l	= edge_l[TKRA_ES_XPOS];
	xpos_r	= edge_r[TKRA_ES_XPOS];
	xstep_l	= edge_l[TKRA_ES_XSTEP];
	xstep_r	= edge_r[TKRA_ES_XSTEP];
	
	tpos_l	= edge_l[TKRA_ES_TPOS];
	tpos_r	= edge_r[TKRA_ES_TPOS];
	tstep_l	= edge_l[TKRA_ES_TSTEP];
	tstep_r	= edge_r[TKRA_ES_TSTEP];
	
	cpos_l	= edge_l[TKRA_ES_CPOS];
	cpos_r	= edge_r[TKRA_ES_CPOS];
	cstep_l	= edge_l[TKRA_ES_CSTEP];
	cstep_r	= edge_r[TKRA_ES_CSTEP];
	
	zpos_l	= edge_l[TKRA_ES_ZPOS];
	zpos_r	= edge_r[TKRA_ES_ZPOS];
	zstep_l	= edge_l[TKRA_ES_ZSTEP];
	zstep_r	= edge_r[TKRA_ES_ZSTEP];
	
	cb0=scr_cb+(ytop*scr_xs);
//	zb0=scr_zb+(ytop*scr_xs);
	zb0=scr_zb+((ytop>>1)*scr_zxs);

	ymax=ytop+cnt;
	for(y=ytop; y<ymax; y++)
	{
		x0=(xpos_l      )>>16;
		x1=(xpos_r+65535)>>16;
		xcnt=x1-x0;
		if((xcnt<1) || (y<clip_y0) || (y>=clip_y1))
		{
			xpos_l+=xstep_l;	xpos_r+=xstep_r;
			tpos_l+=tstep_l;	tpos_r+=tstep_r;
			cpos_l+=cstep_l;	cpos_r+=cstep_r;
			zpos_l+=zstep_l;	zpos_r+=zstep_r;
			cb0+=scr_xs;
			if(y&1)
			{
				zb0=zb0+scr_zxs;
			}

			continue;
		}

		xrcp=TKRA_SpanRcp(xcnt+1);
		
		tstep_c=TKRA_CalcTexStepRcp(tpos_r, tpos_l, xrcp);

		cstep_c=TKRA_CalcClrStepRcp(cpos_r, cpos_l, xrcp);

		dsparm[TKRA_DS_TPOS		]=tpos_l;
		dsparm[TKRA_DS_TSTEP	]=tstep_c;
		dsparm[TKRA_DS_CPOS		]=cpos_l;
		dsparm[TKRA_DS_CSTEP	]=cstep_c;

//		if(!(y&1))
//		if(y&1)
		if(1)
		{
			zstep_d=zpos_r-zpos_l;
			zstep_c=tkra_dmuls(zstep_d, xrcp)>>16;
			dsparm[TKRA_DS_ZPOS		]=zpos_l;
			dsparm[TKRA_DS_ZSTEP	]=zstep_c;
		}

//		if((y>=clip_y0) && (y<clip_y1))
		if(1)
		{
			if(x0<clip_x0)
			{
				x2=clip_x0-x0;
				dsparm[TKRA_DS_TPOS		]+=tstep_c*x2;
				dsparm[TKRA_DS_CPOS		]+=cstep_c*x2;
				dsparm[TKRA_DS_ZPOS		]+=zstep_c*x2;
				x0+=x2;
			}
			
//			dsparm[TKRA_DS_ZPOS]<<=1;
			
			if(x1>=clip_x1)
			{
				xcnt=clip_x1-x0;
			}

			zb=zb0+(x0>>1);

			if(!(y&1))
			{
//				DrawSpanZ(dsparm, zb, xcnt>>1);
				DrawSpanZ(zb, xcnt>>1,
//					dsparm[TKRA_DS_ZPOS],
//					dsparm[TKRA_DS_ZPOS]+(512<<16),
					dsparm[TKRA_DS_ZPOS]+(128<<16),
					zstep_c<<1);
//					dsparm[TKRA_DS_ZSTEP]);
			}
			
//			dsparm[TKRA_DS_ZPOS]-=256<<16;

			cb=cb0+x0;
			if(xcnt>0)
			{
				DrawSpan(dsparm, cb, zb, xcnt);
			}
		}
		
		xpos_l+=xstep_l;	xpos_r+=xstep_r;
		tpos_l+=tstep_l;	tpos_r+=tstep_r;
		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		zpos_l+=zstep_l;	zpos_r+=zstep_r;
		cb0+=scr_xs;
		if(y&1)
		{
			zb0=zb0+scr_zxs;
		}
	}

	edge_l[TKRA_ES_XPOS] = xpos_l;
	edge_r[TKRA_ES_XPOS] = xpos_r;
	edge_l[TKRA_ES_TPOS] = tpos_l;
	edge_r[TKRA_ES_TPOS] = tpos_r;
	edge_l[TKRA_ES_CPOS] = cpos_l;
	edge_r[TKRA_ES_CPOS] = cpos_r;
	edge_l[TKRA_ES_ZPOS] = zpos_l;
	edge_r[TKRA_ES_ZPOS] = zpos_r;
}

/*
Draw a Triangle
Given 3 vertices in Screen Space.
*/
void TKRA_WalkTriangle(TKRA_Context *ctx,
	u64 *vec0, u64 *vec1, u64 *vec2)
{
	u64	e1_parm[TKRA_ES_NPARM];
	u64	e2_parm[TKRA_ES_NPARM];
	u64	e3_parm[TKRA_ES_NPARM];
	void (*RasterWalkEdges)(TKRA_Context *ctx,
		int ytop, u64 *edge_l, u64 *edge_r, int cnt);
	u64 *tve0, *tve1, *tve2;
	int y0, y1, y2, y3;
	int y1cnt, y2cnt, y3cnt, y1rcp, y2rcp, y3rcp;
	u64 t0, t1, t2;

	int x1pos, x2pos, x3pos, x1step, x2step, x3step;
	int x1step_d, x1step_c;
	int x2step_d, x2step_c;
	int x3step_d, x3step_c;

	int z1pos, z2pos, z3pos, z1step, z2step, z3step;
	u64 t1pos, t2pos, t3pos, t1step, t2step, t3step;
	u64 t1step_d, t1step_c;
	u64 t2step_d, t2step_c;
	u64 t3step_d, t3step_c;
	u64 c1pos, c2pos, c3pos, c1step, c2step, c3step;
	u64 c1step_d, c1step_c, c2step_d, c2step_c, c3step_d, c3step_c;
	int z1step_d, z1step_c, z2step_d, z2step_c, z3step_d, z3step_c;
	int xshl, yshl;

	RasterWalkEdges = ctx->RasterWalkEdges;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;

	y0=vec0[TKRA_VX_YPOS];
	y1=vec1[TKRA_VX_YPOS];
	y2=vec2[TKRA_VX_YPOS];

	/* Sort vertices by Y position */
	if(y0<y1)
	{
		if(y1<y2)
			{	tve0=vec0;	tve1=vec1;	tve2=vec2;	}
		else if(y0<y2)
			{	tve0=vec0;	tve1=vec2;	tve2=vec1;	}
		else
			{	tve0=vec2;	tve1=vec0;	tve2=vec1;	}
	}else
	{
		if(y0<y2)	
			{	tve0=vec1;	tve1=vec0;	tve2=vec2;	}
		else if(y1<y2)
			{	tve0=vec1;	tve1=vec2;	tve2=vec0;	}
		else
			{	tve0=vec2;	tve1=vec1;	tve2=vec0;	}
	}

	y0=tve0[TKRA_VX_YPOS];
	y1=tve1[TKRA_VX_YPOS];
	y2=tve2[TKRA_VX_YPOS];
	
	if(y0>y1)
		{ __debugbreak(); }
	if(y1>y2)
		{ __debugbreak(); }
	
	y0=tve0[TKRA_VX_YPOS]>>16;
	y1=tve1[TKRA_VX_YPOS]>>16;
	y2=tve2[TKRA_VX_YPOS]>>16;
	y1cnt=y1-y0;
	y2cnt=y2-y0;
	y3cnt=y2-y1;
	y1rcp=TKRA_SpanRcp(y1cnt+1);
	y2rcp=TKRA_SpanRcp(y2cnt+1);
	y3rcp=TKRA_SpanRcp(y3cnt+1);

	t0=tve0[TKRA_VX_XPOS];
	t1=tve1[TKRA_VX_XPOS];
	t2=tve2[TKRA_VX_XPOS];
	x1step_d=t1-t0;
//	x1step_c=(((s64)x1step_d)*y1rcp)>>16;
	x1step_c=tkra_dmuls(x1step_d, y1rcp)>>16;
	x2step_d=t2-t0;
//	x2step_c=(((s64)x2step_d)*y2rcp)>>16;
	x2step_c=tkra_dmuls(x2step_d, y2rcp)>>16;
	x3step_d=t2-t1;
//	x3step_c=(((s64)x3step_d)*y3rcp)>>16;
	x3step_c=tkra_dmuls(x3step_d, y3rcp)>>16;

	t0=tve0[TKRA_VX_TPOS];
	t1=tve1[TKRA_VX_TPOS];
	t2=tve2[TKRA_VX_TPOS];
//	t1step_d=t1-t0;
//	t1step_c=TKRA_ScaleTexStepRcp(t1step_d, y1rcp);
	t1step_c=TKRA_CalcTexStepRcp(t1, t0, y1rcp);
//	t2step_d=t2-t0;
//	t2step_c=TKRA_ScaleTexStepRcp(t2step_d, y2rcp);
	t2step_c=TKRA_CalcTexStepRcp(t2, t0, y2rcp);
//	t3step_d=t2-t1;
//	t3step_c=TKRA_ScaleTexStepRcp(t3step_d, y3rcp);
	t3step_c=TKRA_CalcTexStepRcp(t2, t1, y3rcp);

	t0=tve0[TKRA_VX_CPOS];
	t1=tve1[TKRA_VX_CPOS];
	t2=tve2[TKRA_VX_CPOS];
//	c1step_d=t1-t0;
//	c1step_c=TKRA_ScaleClrStepRcp(c1step_d, y1rcp);
	c1step_c=TKRA_CalcClrStepRcp(t1, t0, y1rcp);
//	c2step_d=t2-t0;
//	c2step_c=TKRA_ScaleClrStepRcp(c2step_d, y2rcp);
	c2step_c=TKRA_CalcClrStepRcp(t2, t0, y2rcp);
//	c3step_d=t2-t1;
//	c3step_c=TKRA_ScaleClrStepRcp(c3step_d, y3rcp);
	c3step_c=TKRA_CalcClrStepRcp(t2, t1, y3rcp);

	t0=tve0[TKRA_VX_ZPOS];
	t1=tve1[TKRA_VX_ZPOS];
	t2=tve2[TKRA_VX_ZPOS];
	z1step_d=t1-t0;
//	z1step_c=(((s64)z1step_d)*y1rcp)>>16;
	z1step_c=tkra_dmuls(z1step_d, y1rcp)>>16;
	z2step_d=t2-t0;
//	z2step_c=(((s64)z2step_d)*y2rcp)>>16;
	z2step_c=tkra_dmuls(z2step_d, y2rcp)>>16;
	z3step_d=t2-t1;
//	z3step_c=(((s64)z3step_d)*y3rcp)>>16;
	z3step_c=tkra_dmuls(z3step_d, y3rcp)>>16;

	e1_parm[TKRA_ES_XPOS]=tve0[TKRA_VX_XPOS];
	e1_parm[TKRA_ES_TPOS]=tve0[TKRA_VX_TPOS];
	e1_parm[TKRA_ES_CPOS]=tve0[TKRA_VX_CPOS];
	e1_parm[TKRA_ES_ZPOS]=tve0[TKRA_VX_ZPOS];

	e2_parm[TKRA_ES_XPOS]=tve0[TKRA_VX_XPOS];
	e2_parm[TKRA_ES_TPOS]=tve0[TKRA_VX_TPOS];
	e2_parm[TKRA_ES_CPOS]=tve0[TKRA_VX_CPOS];
	e2_parm[TKRA_ES_ZPOS]=tve0[TKRA_VX_ZPOS];

	e3_parm[TKRA_ES_XPOS]=tve1[TKRA_VX_XPOS];
	e3_parm[TKRA_ES_TPOS]=tve1[TKRA_VX_TPOS];
	e3_parm[TKRA_ES_CPOS]=tve1[TKRA_VX_CPOS];
	e3_parm[TKRA_ES_ZPOS]=tve1[TKRA_VX_ZPOS];

	e1_parm[TKRA_ES_XSTEP]=x1step_c;	/* v1-v0 */
	e1_parm[TKRA_ES_TSTEP]=t1step_c;
	e1_parm[TKRA_ES_CSTEP]=c1step_c;
	e1_parm[TKRA_ES_ZSTEP]=z1step_c;

	e2_parm[TKRA_ES_XSTEP]=x2step_c;	/* v2-v0 */
	e2_parm[TKRA_ES_TSTEP]=t2step_c;
	e2_parm[TKRA_ES_CSTEP]=c2step_c;
	e2_parm[TKRA_ES_ZSTEP]=z2step_c;

	e3_parm[TKRA_ES_XSTEP]=x3step_c;	/* v2-v1 */
	e3_parm[TKRA_ES_TSTEP]=t3step_c;
	e3_parm[TKRA_ES_CSTEP]=c3step_c;
	e3_parm[TKRA_ES_ZSTEP]=z3step_c;

#if 0
	t0=((u32)(tve0[TKRA_VX_TPOS]))|((t0>>48)<<(32+xshl));
	t1=((u32)(tve1[TKRA_VX_TPOS]))|((t1>>48)<<(32+xshl));
	e1_parm[TKRA_ES_TPOS]=t0;
	e2_parm[TKRA_ES_TPOS]=t0;
	e3_parm[TKRA_ES_TPOS]=t1;

	t0=((u32)(t1step_c))|((t1step_c>>48)<<(32+xshl));
	t1=((u32)(t2step_c))|((t2step_c>>48)<<(32+xshl));
	t2=((u32)(t3step_c))|((t3step_c>>48)<<(32+xshl));
	e1_parm[TKRA_ES_TSTEP]=t0;
	e2_parm[TKRA_ES_TSTEP]=t1;
	e3_parm[TKRA_ES_TSTEP]=t2;
#endif

#if 0
//	if(y3cnt==0)
	if(0)
	{
//		if(tve1[TKRA_VX_XPOS]<tve2[TKRA_VX_XPOS])
		if(x1step_c<=x2step_c)
		{
			RasterWalkEdges(ctx, y0, e1_parm, e2_parm, y1-y0);
		}else
		{
			RasterWalkEdges(ctx, y0, e2_parm, e1_parm, y1-y0);
		}
		return;
	}
#endif

	/* Figure out which is left edge. */
//	if(tve1[TKRA_VX_XPOS]<tve2[TKRA_VX_XPOS])
	if(x1step_c<=x2step_c)
	{
		RasterWalkEdges(ctx, y0, e1_parm, e2_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e3_parm, e2_parm, y2-y1);
	}else
	{
		RasterWalkEdges(ctx, y0, e2_parm, e1_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e2_parm, e3_parm, y2-y1);
	}
	
}

int TKRA_SetupDrawEdgeForState(TKRA_Context *ctx)
{
	ctx->DrawSpan=TKRA_DrawSpan_ModTexMort;
	ctx->DrawSpan_Min=TKRA_DrawSpan_ModTexMort;
	ctx->DrawSpan_Mag=TKRA_DrawSpan_ModTexMort;
//	ctx->DrawSpan=TKRA_DrawSpan_ModTexMortZbuf;
//	ctx->DrawSpan=TKRA_DrawSpan_DirTexMort;
//	ctx->DrawSpan=TKRA_DrawSpan_DirClr;

//	ctx->DrawSpanHZt=TKRA_DrawSpan_ModTexMortHZt;
//	ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModTexMortHZt;
//	ctx->DrawSpanHZt=TKRA_DrawSpan_ModBlTexMortHZt;

//	ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModBlTexMortHZt;
	ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModTexMortHZt;

	ctx->DrawSpanHZt_Min=TKRA_DrawSpan_AlphaModTexMortHZt;
//	ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortHZt;
	ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModTexMortHZt;
	ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;

//	ctx->RasterWalkEdges=TKRA_WalkEdges_Dfl;
	ctx->RasterWalkEdges=TKRA_WalkEdges_HZbuf;
	return(0);
}

int TKRA_SetupDrawEdgeForTriFlag(TKRA_Context *ctx, int trifl)
{
	if(trifl&TKRA_TRFL_ALPHA)
	{
//		ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModBlTexMortHZt;
		ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModTexMortHZt;
		ctx->DrawSpanHZt_Min=TKRA_DrawSpan_AlphaModTexMortHZt;
//		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortHZt;
		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModTexMortHZt;
	}else
	{
		ctx->DrawSpanHZt=TKRA_DrawSpan_ModBlTexMortHZt;
//		ctx->DrawSpanHZt=TKRA_DrawSpan_ModTexMortHZt;
		ctx->DrawSpanHZt_Min=TKRA_DrawSpan_ModTexMortHZt;
//		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_ModBlTexMortHZt;
		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_ModTexMortHZt;
	}
	return(0);
}

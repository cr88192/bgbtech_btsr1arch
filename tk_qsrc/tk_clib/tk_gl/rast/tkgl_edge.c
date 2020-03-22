int TKGL_RasterSpanRcp(int cnt)
{
	/* Placeholder */
	return(65536/cnt);
}

u64 TKGL_ScaleTexStepRcp(u64 tdist, u16 rcp)
{
	u32 tdy, tdx;
	
	tdy=(tdist>>32);
	tdx=(tdist    );
	tdy=(((u64)tdy)*rcp)>>16;
	tdx=(((u64)tdx)*rcp)>>16;
	return((((u64)tdy)<<32)|tdx);
}

#ifdef _BGBCC
__asm {
TKGL_ScaleClrStepRcp:
	PSHUF.W		R5, 0, R6		//(0000)
	PMULU.HW	R4, R6, R2
	RTSU
};
#else
u64 TKGL_ScaleClrStepRcp(u64 tdist, u16 rcp)
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
#endif

/*
Walk along a pair of edges, calling DrawSpan for each span.
Default Version (Tex+Color+Z)
 */
void TKGL_RasterWalkEdges_Dfl(TKGL_RastContext *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt)
{
	void (*DrawSpan)(u64 *parm,
		tkgl_rastpixel *dstc, tkgl_zbufpixel *dstz, int cnt);

	u64	dsparm[TKGL_DS_NPARM];
	tkgl_rastpixel *scr_cb, *cb0;
	tkgl_zbufpixel *scr_zb, *zb0;
	tkgl_rastpixel *cb;
	tkgl_zbufpixel *zb;
	int xpos_l, xpos_r, xstep_l, xstep_r;
	int zpos_l, zpos_r, zstep_l, zstep_r;
	u64 tpos_l, tpos_r, tstep_l, tstep_r;
	u64 tstep_d, tstep_c;

	u64 cpos_l, cpos_r, cstep_l, cstep_r;
	u64 cstep_d, cstep_c;
	int zstep_d, zstep_c;
	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		y, ymax, x0, x1, x2, xcnt, xrcp;
	int		scr_xs, scr_ys;


	clip_x0	= 0;
	clip_x1	= scr_xs;

	clip_y0	= 0;
	clip_y1	= scr_ys;

	if(cnt<=0)
		return;
	if(ymax<=clip_y0)
		return;
	if(ytop>=clip_y1)
		return;
	

	DrawSpan	= ctx->DrawSpan;
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;
	
	dsparm[TKGL_DS_CTX]=(u64)(ctx);
	dsparm[TKGL_DS_TEXIMG]=(u64)(ctx->tex_img);

	dsparm[TKGL_DS_XMASK]=(1<<ctx->tex_xshl)-1;
	dsparm[TKGL_DS_YMASK]=(1<<ctx->tex_yshl)-1;

	xpos_l	= edge_l[TKGL_ES_XPOS];
	xpos_r	= edge_r[TKGL_ES_XPOS];
	xstep_l	= edge_l[TKGL_ES_XSTEP];
	xstep_r	= edge_r[TKGL_ES_XSTEP];
	
	tpos_l	= edge_l[TKGL_ES_TPOS];
	tpos_r	= edge_r[TKGL_ES_TPOS];
	tstep_l	= edge_l[TKGL_ES_TSTEP];
	tstep_r	= edge_r[TKGL_ES_TSTEP];
	
	cpos_l	= edge_l[TKGL_ES_CPOS];
	cpos_r	= edge_r[TKGL_ES_CPOS];
	cstep_l	= edge_l[TKGL_ES_CSTEP];
	cstep_r	= edge_r[TKGL_ES_CSTEP];
	
	zpos_l	= edge_l[TKGL_ES_ZPOS];
	zpos_r	= edge_r[TKGL_ES_ZPOS];
	zstep_l	= edge_l[TKGL_ES_ZSTEP];
	zstep_r	= edge_r[TKGL_ES_ZSTEP];
	
	cb0=scr_cb+(ytop*scr_xs);
	zb0=scr_zb+(ytop*scr_xs);

	ymax=ytop+cnt;
	for(y=ytop; y<ymax; y++)
	{
		x0=xpos_l>>16;
		x1=xpos_r>>16;
		xcnt=x1-x0;
		xrcp=TKGL_RasterSpanRcp(xcnt);
		
		tstep_d=tpos_r-tpos_l;
		tstep_c=TKGL_ScaleTexStepRcp(tstep_d, xrcp);

		cstep_d=cpos_r-cpos_l;
		cstep_c=TKGL_ScaleClrStepRcp(cstep_d, xrcp);

		zstep_d=zpos_r-zpos_l;
		zstep_c=(zstep_d*xrcp)>>16;

		dsparm[TKGL_DS_TPOS		]=tpos_l;
		dsparm[TKGL_DS_TSTEP	]=tstep_c;
		dsparm[TKGL_DS_CPOS		]=cpos_l;
		dsparm[TKGL_DS_CSTEP	]=cstep_c;
		dsparm[TKGL_DS_ZPOS		]=zpos_l;
		dsparm[TKGL_DS_ZSTEP	]=zstep_c;

		if((y>=clip_y0) && (y<clip_y1))
		{
			if(x0<clip_x0)
			{
				x2=clip_x0-x0;
				dsparm[TKGL_DS_TPOS		]+=tstep_c*x2;
				dsparm[TKGL_DS_CPOS		]+=cstep_c*x2;
				dsparm[TKGL_DS_ZPOS		]+=zstep_c*x2;
			}
			
			if(x1>=clip_x1)
			{
				xcnt=clip_x1-x0;
			}
			
			cb=cb0+x0;		zb=zb0+x0;
			if(xcnt>0)
			{
				DrawSpan(dsparm, cb, zb, xcnt);
			}
		}
		
		xpos_l+=xstep_l;	xpos_r+=xstep_r;
		tpos_l+=tstep_l;	tpos_r+=tstep_r;
		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		zpos_l+=zstep_l;	zpos_r+=zstep_r;
		cb0+=scr_xs;		zb0+=scr_xs;
	}

	edge_l[TKGL_ES_XPOS] = xpos_l;
	edge_r[TKGL_ES_XPOS] = xpos_r;
	edge_l[TKGL_ES_TPOS] = tpos_l;
	edge_r[TKGL_ES_TPOS] = tpos_r;
	edge_l[TKGL_ES_CPOS] = cpos_l;
	edge_r[TKGL_ES_CPOS] = cpos_r;
	edge_l[TKGL_ES_ZPOS] = zpos_l;
	edge_r[TKGL_ES_ZPOS] = zpos_r;
}

/*
Draw a Triangle
Given 3 vertices in Screen Space.
*/
void TKGL_RasterWalkTriangle(TKGL_RastContext *ctx,
	u64 *vec0, u64 *vec1, u64 *vec2)
{
	u64	e1_parm[TKGL_ES_NPARM];
	u64	e2_parm[TKGL_ES_NPARM];
	u64	e3_parm[TKGL_ES_NPARM];
	void (*RasterWalkEdges)(TKGL_RastContext *ctx,
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

	RasterWalkEdges = ctx->RasterWalkEdges;

	y0=vec0[TKGL_VX_YPOS];
	y1=vec1[TKGL_VX_YPOS];
	y2=vec2[TKGL_VX_YPOS];

	/* Sort vertices by Y position */
	if(y0<y1)
	{
		if(y1<y2)		{	tve0=vec0;	tve1=vec1;	tve2=vec2;	}
		else if(y0<y2)	{	tve0=vec0;	tve1=vec2;	tve2=vec1;	}
		else			{	tve0=vec2;	tve1=vec0;	tve2=vec1;	}
	}else
	{
		if(y0<y2)		{	tve0=vec1;	tve1=vec0;	tve2=vec2;	}
		else if(y1<y2)	{	tve0=vec1;	tve1=vec2;	tve2=vec0;	}
		else			{	tve0=vec2;	tve1=vec1;	tve2=vec0;	}
	}

	y0=tve0[TKGL_VX_YPOS]>>16;
	y1=tve1[TKGL_VX_YPOS]>>16;
	y2=tve2[TKGL_VX_YPOS]>>16;
	y1cnt=y1-y0;
	y2cnt=y2-y0;
	y3cnt=y2-y1;
	y1rcp=TKGL_RasterSpanRcp(y1cnt);
	y2rcp=TKGL_RasterSpanRcp(y2cnt);
	y3rcp=TKGL_RasterSpanRcp(y3cnt);

	t0=tve0[TKGL_VX_XPOS];
	t1=tve1[TKGL_VX_XPOS];
	t2=tve2[TKGL_VX_XPOS];
	x1step_d=t1-t0;
	x1step_c=(x1step_d*y1rcp)>>16;
	x2step_d=t2-t0;
	x2step_c=(x2step_d*y2rcp)>>16;
	x3step_d=t2-t1;
	x3step_c=(x3step_d*y3rcp)>>16;

	t0=tve0[TKGL_VX_TPOS];
	t1=tve1[TKGL_VX_TPOS];
	t2=tve2[TKGL_VX_TPOS];
	t1step_d=t1-t0;
	t1step_c=TKGL_ScaleTexStepRcp(t1step_d, y1rcp);
	t2step_d=t2-t0;
	t2step_c=TKGL_ScaleTexStepRcp(t2step_d, y2rcp);
	t3step_d=t2-t1;
	t3step_c=TKGL_ScaleTexStepRcp(t3step_d, y3rcp);

	t0=tve0[TKGL_VX_CPOS];
	t1=tve1[TKGL_VX_CPOS];
	t2=tve2[TKGL_VX_CPOS];
	c1step_d=t1-t0;
	c1step_c=TKGL_ScaleClrStepRcp(c1step_d, y1rcp);
	c2step_d=t2-t0;
	c2step_c=TKGL_ScaleClrStepRcp(c2step_d, y2rcp);
	c3step_d=t2-t1;
	c3step_c=TKGL_ScaleClrStepRcp(c3step_d, y3rcp);

	t0=tve0[TKGL_VX_ZPOS];
	t1=tve1[TKGL_VX_ZPOS];
	t2=tve2[TKGL_VX_ZPOS];
	z1step_d=y1-t0;
	z1step_c=(z1step_d*y1rcp)>>16;
	z2step_d=t2-t0;
	z2step_c=(z2step_d*y2rcp)>>16;
	z3step_d=t2-t1;
	z3step_c=(z3step_d*y3rcp)>>16;

	e1_parm[TKGL_ES_XPOS]=tve0[TKGL_VX_XPOS];
	e1_parm[TKGL_ES_TPOS]=tve0[TKGL_VX_TPOS];
	e1_parm[TKGL_ES_CPOS]=tve0[TKGL_VX_CPOS];
	e1_parm[TKGL_ES_ZPOS]=tve0[TKGL_VX_ZPOS];

	e2_parm[TKGL_ES_XPOS]=tve0[TKGL_VX_XPOS];
	e2_parm[TKGL_ES_TPOS]=tve0[TKGL_VX_TPOS];
	e2_parm[TKGL_ES_CPOS]=tve0[TKGL_VX_CPOS];
	e2_parm[TKGL_ES_ZPOS]=tve0[TKGL_VX_ZPOS];

	e3_parm[TKGL_ES_XPOS]=tve1[TKGL_VX_XPOS];
	e3_parm[TKGL_ES_TPOS]=tve1[TKGL_VX_TPOS];
	e3_parm[TKGL_ES_CPOS]=tve1[TKGL_VX_CPOS];
	e3_parm[TKGL_ES_ZPOS]=tve1[TKGL_VX_ZPOS];

	e1_parm[TKGL_ES_XSTEP]=x1step_c;
	e1_parm[TKGL_ES_TSTEP]=t1step_c;
	e1_parm[TKGL_ES_CSTEP]=c1step_c;
	e1_parm[TKGL_ES_ZSTEP]=z1step_c;

	e2_parm[TKGL_ES_XSTEP]=x2step_c;
	e2_parm[TKGL_ES_TSTEP]=t2step_c;
	e2_parm[TKGL_ES_CSTEP]=c2step_c;
	e2_parm[TKGL_ES_ZSTEP]=z2step_c;

	e3_parm[TKGL_ES_XSTEP]=x3step_c;
	e3_parm[TKGL_ES_TSTEP]=t3step_c;
	e3_parm[TKGL_ES_CSTEP]=c3step_c;
	e3_parm[TKGL_ES_ZSTEP]=z3step_c;

	/* Figure out which is left edge. */
	if(vec1[TKGL_VX_XPOS]<vec2[TKGL_VX_XPOS])
	{
		RasterWalkEdges(ctx, y0, e1_parm, e2_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e3_parm, e2_parm, y2-y1);
	}else
	{
		RasterWalkEdges(ctx, y0, e2_parm, e1_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e2_parm, e3_parm, y2-y1);
	}
	
}


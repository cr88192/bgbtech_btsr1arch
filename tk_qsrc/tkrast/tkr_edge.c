no_cull u16 tkra_spanrcptab[1024];

no_cull int TKRA_InitSpanRcp(void)
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
	int ix, shr;

	if(cnt<1024)
		return(tkra_spanrcptab[cnt]);
	/* Placeholder */

	ix=cnt; shr=0;
	while(ix>=1024)
		{ ix=ix>>1; shr++; }
	return(tkra_spanrcptab[ix]>>shr);
	
//	return(65536/cnt);
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
	RTS

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

//	tda=tda&0xFFF0;	tdr=tdr&0xFFF0;
//	tdg=tdg&0xFFF0;	tdb=tdb&0xFFF0;

	return(
		(((u64)tda)<<48)|
		(((u64)tdr)<<32)|
		(((u64)tdg)<<16)|
		(      tdb     ));
}

#endif

#ifndef __BJX2__
// #if 1
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
	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);
	dsparm[TKRA_DS_BLEND]=(u64)(ctx->Blend);
	dsparm[TKRA_DS_ZATEST]=(u64)(ctx->ZaTest);

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
//		x0=(xpos_l      )>>16;
		x0=xpos_l>>16;
		x1=xpos_r>>16;
//		x1=(xpos_r+65535)>>16;
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

//		xrcp=TKRA_SpanRcp(xcnt+1);
		xrcp=TKRA_SpanRcp(xcnt);
		
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
				xcnt=x1-x0;
			}
			
			if(x1>=clip_x1)
			{
				x1=clip_x1-1;
//				xcnt=clip_x1-x0;
				xcnt=x1-x0;
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
//		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		cpos_l=tkra_paddhw(cpos_l, cstep_l);
		cpos_r=tkra_paddhw(cpos_r, cstep_r);
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

// void TKRA_WalkEdges_Zbuf(TKRA_Context *ctx,
//	int ytop, u64 *edge_l, u64 *edge_r, int cnt)
// {
//	TKRA_WalkEdges_Dfl(ctx, ytop, edge_l, edge_r, cnt);
// }

#endif

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
	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);
	dsparm[TKRA_DS_BLEND]=(u64)(ctx->Blend);
	dsparm[TKRA_DS_ZATEST]=(u64)(ctx->ZaTest);

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
//		x1=(xpos_r+65535)>>16;
		x1=(xpos_r      )>>16;
		xcnt=x1-x0;
		if((xcnt<1) || (y<clip_y0) || (y>=clip_y1))
		{
			xpos_l+=xstep_l;	xpos_r+=xstep_r;
			tpos_l+=tstep_l;	tpos_r+=tstep_r;
//			cpos_l+=cstep_l;	cpos_r+=cstep_r;
			cpos_l=tkra_paddhw(cpos_l, cstep_l);
			cpos_r=tkra_paddhw(cpos_r, cstep_r);
			zpos_l+=zstep_l;	zpos_r+=zstep_r;
			cb0+=scr_xs;
			if(y&1)
			{
				zb0=zb0+scr_zxs;
			}

			continue;
		}

//		xrcp=TKRA_SpanRcp(xcnt+1);
		xrcp=TKRA_SpanRcp(xcnt);
		
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
				xcnt=x1-x0;
			}
			
//			dsparm[TKRA_DS_ZPOS]<<=1;
			
			if(x1>=clip_x1)
			{
				x1=clip_x1-1;
//				xcnt=clip_x1-x0;
				xcnt=x1-x0;
			}

			zb=zb0+(x0>>1);

			if(!(y&1))
			{
//				DrawSpanZ(dsparm, zb, xcnt>>1);
				DrawSpanZ(zb, xcnt>>1,
//					dsparm[TKRA_DS_ZPOS],
//					dsparm[TKRA_DS_ZPOS]+(512<<16),
//					dsparm[TKRA_DS_ZPOS]+(128<<16),
//					dsparm[TKRA_DS_ZPOS]+(16<<16),
					dsparm[TKRA_DS_ZPOS]+(4<<16),
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
//		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		cpos_l=tkra_paddhw(cpos_l, cstep_l);
		cpos_r=tkra_paddhw(cpos_r, cstep_r);
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

// #if 0
#ifdef __BJX2__
void TKRA_WalkEdges_Dfl(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);
void TKRA_WalkEdges_Zbuf(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);
void TKRA_WalkEdges_ZbufNZb(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);

void TKRA_WalkEdges_ZbufNcp(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);

__asm {
#if 0
TKRA_WalkEdges_Zbuf:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		x0
R17		x1
R18		xcnt
R19		Scratch

R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r
*/

//	clip_x0	= ctx->clip_x0;
//	clip_x1	= ctx->clip_x1;
//	clip_y0	= ctx->clip_y0;
//	clip_y1	= ctx->clip_y1;
	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13


	ADD		R5, R20, R21		//ymax = ytop+ycnt
	MOV		R5, R20				//ycur = ytop

	DMULS	R5, R13, R12		//scr_ofs = ycur*screen_xsize

	SHAD.Q	R22, -32, R2	//clip_x1
	SHAD.Q	R23, -32, R3	//clip_y1

//	if(ytop>=clip_y1)
//		return;
	CMPGT	R5, R3
	BF		.L_END

//	ymax=ytop+cnt;
//	if(ymax<=clip_y0)
//		return;
	CMPGT	R23, R21
	BF		.L_END

//	dsparm[TKRA_DS_CTX]=(u64)(ctx);
//	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);
//	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);

	LEA.Q	(SP, 32*8), R14
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)

	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)

//	xshl=ctx->tex_xshl;
//	yshl=ctx->tex_yshl;
//	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
//	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1
	SHAD	R1, R2, R19
	ADD		R19, -1, R19
	MOV.Q	R19, (R14, TKRA_DS_XMASK*8)
	
	ADD		R2, R3, R19
	SHAD	R1, R19, R19
	ADD		R19, -1, R19
	MOV.Q	R19, (R14, TKRA_DS_YMASK*8)


	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	SHAD.Q	R24, -48, R16		//		x0=(xzpos_l)>>48;
//	SHAD.Q	R26, -48, R17		//		x1=(xzpos_r)>>48;

	SHAD.Q	R26, -32, R17		//		x1=(xzpos_r)>>48;
	ADD		65535, R17
	SHAD.Q	R17, -16, R17		//		x1=(xzpos_r)>>48;

	SUB		R17, R16, R18		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	//clip_x1
	SHAD.Q	R23, -32, R1	//clip_y1

	ADDS.L	R18, -1, R19	//xcnt-1
	SUBS.L	R20, R23, R2	//y -clip_y0
	SUBS.L	R17, R22, R3	//x1-clip_x0
	SUBS.L	R1, R20, R1		//clip_y1-y
	SUBS.L	R0, R16, R0		//clip_x1-x0
	
	OR		R2, R3, R3
	OR		R0, R1, R1
	OR		R3, R19, R2
	OR		R2, R1, R2

//		x2=	(y -clip_y0) | (clip_y1- y) |
//			(x1-clip_x0) | (clip_x1-x0) |
//			(xcnt-1);

	
	CMPGE	0, R2
	BT		.L1

//		if(x2<0)
//		{
	
	ADD		R25, R24
	ADD		R27, R26
	ADD		R29, R28
	ADD		R31, R30
	ADD		R9 , R8
	ADD		R11, R10
	ADD		R13, R12
	ADD		1, R20

//			tpos_l+=tstep_l;	tpos_r+=tstep_r;
//			cpos_l+=cstep_l;	cpos_r+=cstep_r;
//			xzpos_l+=xzstep_l;	xzpos_r+=xzstep_r;
//			cb0=cb0+scr_xs;		zb0=zb0+scr_xs;

	BRA		.L0

//			continue;
//		}
	
	.L1:


//		xrcp=TKRA_SpanRcp(xcnt+1);
		ADDS.L	R18, 1, R19	//xcnt+1
		MOV		tkra_spanrcptab, R2
		MOVU.W	(R2, R19), R1

//		tstep_c=TKRA_CalcTexStepRcp(tpos_r, tpos_l, xrcp);

		SUB		R30, R28, R2	//tpos_r - tpos_l
		SHAD.Q	R2, -32, R3
		EXTS.L	R2, R2
		DMULS.L	R2, R1, R2
		DMULS.L	R3, R1, R3
		SHAD.Q	R2, -16, R2
		SHAD.Q	R3, -16, R3
		MOVLD	R3, R2, R4		//tstep_c

//		cstep_c=TKRA_CalcClrStepRcp(cpos_r, cpos_l, xrcp);

		MOV		0xFFFEFFFEFFFEFFFE, R3
		AND		R10, R3, R2
		AND		R8, R3, R3

		SHLD.Q	R2, -1, R2
		SHLD.Q	R3, -1, R3
		SUB			R2, R3, R2
		PSHUF.W		R1, 0, R19		//(0000)
		PMULS.HW	R2, R19, R2
		SHLD.Q		R2, 1, R5		//cstep_c

//		zstep_d=xzpos_r-xzpos_l;
//		zstep_c=tkra_dmuls(zstep_d, xrcp)>>16;
		SUBS.L	R26, R24, R19
		DMULS	R19, R1, R19
		SHAD.Q	R19, -16, R19

//		z0=xzpos_l;

//		dsparm[TKRA_DS_TPOS		]=tpos_l;
//		dsparm[TKRA_DS_TSTEP	]=tstep_c;
		MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
		MOV.Q	R4, (R14, TKRA_DS_TSTEP*8)

//		dsparm[TKRA_DS_CPOS		]=cpos_l;
//		dsparm[TKRA_DS_CSTEP	]=cstep_c;
		MOV.Q	R8, (R14, TKRA_DS_CPOS*8)
		MOV.Q	R5, (R14, TKRA_DS_CSTEP*8)

//		dsparm[TKRA_DS_ZPOS		]=z0;
//		dsparm[TKRA_DS_ZSTEP	]=zstep_c;
//		MOV.Q	R24, (R14, TKRA_DS_ZPOS*8)
//		MOV.Q	R19, (R14, TKRA_DS_ZSTEP*8)

		EXTS.L	R24, R2
		EXTS.L	R19, R3
		MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
		MOV.Q	R3, (R14, TKRA_DS_ZSTEP*8)

//		if(x0<clip_x0)
//		{
//		EXTU.L	R22, R1
//		CMPGE	R16, R1

//		CMPGT	R16, R22
		CMPGE	R16, R22
		BF		.L2

//			x2=clip_x0-x0;
//			z0+=zstep_c*x2;
//			dsparm[TKRA_DS_TPOS		]+=tstep_c*x2;
//			dsparm[TKRA_DS_CPOS		]+=cstep_c*x2;
//			dsparm[TKRA_DS_ZPOS		]=z0;
//			x0+=x2;
//			xcnt=x1-x0;

		SUBS.L	R22, R16, R1

		DMULS.L	R4, R1, R2
		ADD		R28, R2, R2
		MOV.Q	R2, (R14, TKRA_DS_TPOS*8)

		DMULS.L	R5, R1, R3
		ADD		R8, R3, R3
		MOV.Q	R3, (R14, TKRA_DS_CPOS*8)

		DMULS.L	R19, R1, R2
//		ADD		R24, R2, R2
		ADDS.L	R24, R2, R2
		MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)

//		MOV		R22, R16
		EXTS.L	R22, R16

//		}

		.L2:
		
		
//		if(x1>=clip_x1)
//		{
//			x1=clip_x1-1;
//			xcnt=x1-x0;
//		}

		SHAD.Q	R22, -32, R2
		CMPGT	R2, R17
		MOV?T	R2, R17

//		SUB		R17, R16, R18		//		xcnt=x1-x0;
		SUBS.L	R17, R16, R18		//		xcnt=x1-x0;

//		if(xcnt>0)
//		{
		CMPGT	0, R18
		BF		.L3


		MOV.X	R4, (SP, 4*8)
		MOV.X	R6, (SP, 6*8)
		MOV.X	R16, (SP, 16*8)
		MOV.X	R18, (SP, 18*8)
		MOV.X	R20, (SP, 20*8)
		MOV.X	R22, (SP, 22*8)

//			zb=zb0+x0;		cb=cb0+x0;
		MOV.Q	(R14, TKRA_DS_CTX*8), R19
		MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
		MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
		ADD		R12, R16, R3
		LEA.W	(R4, R3), R4
		LEA.W	(R5, R3), R5

		MOV.X	R4, (SP, 48*8)

//			DrawSpanZ(zb, xcnt, z0, zstep_c);
		MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZb), R3
		MOV		R18, R5
		MOV.X	(R14, TKRA_DS_ZPOS*8), R6
		JSR		R3

//			DrawSpan(dsparm, cb, zb, xcnt);
		MOV.X	(SP, 48*8), R6
		MOV		R14, R4
		MOV		R7, R5
		MOV.Q	(SP, 18*8), R7
		MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZt), R3
		JSR		R3


		MOV.X	(SP,  4*8), R4
		MOV.X	(SP,  6*8), R6
		MOV.X	(SP, 16*8), R16
		MOV.X	(SP, 18*8), R18
		MOV.X	(SP, 20*8), R20
		MOV.X	(SP, 22*8), R22
//		}

	.L3:

//		tpos_l+=tstep_l;	tpos_r+=tstep_r;
//		cpos_l+=cstep_l;	cpos_r+=cstep_r;
//		xzpos_l+=xzstep_l;	xzpos_r+=xzstep_r;
//		cb0=cb0+scr_xs;		zb0=zb0+scr_xs;

	ADD		R25, R24
	ADD		R27, R26
	ADD		R29, R28
	ADD		R31, R30
	ADD		R9 , R8
	ADD		R11, R10
	ADD		R13, R12
	ADD		1, R20

	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R16
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30
	ADD		512, SP
	JMP		R16
	NOP

	.L_END_F:
	RTS

#endif




#if 0
.ifarch has_xgpr

TKRA_WalkEdges_Zbuf:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

	MOV.X	R40, (SP, 40*8)
	MOV.X	R42, (SP, 42*8)
	MOV.X	R44, (SP, 44*8)
	MOV.X	R46, (SP, 46*8)

	MOV.X	R56, (SP, 56*8)
	MOV.X	R58, (SP, 58*8)
	MOV.X	R60, (SP, 60*8)
	MOV.X	R62, (SP, 62*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		-
R17		-
R18		-
R19		Scratch


R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r

R40		ycnt / ycur			(alt)
R41		ymax				(alt)
R42		clip_x1:clip_x0		(alt)
R43		clip_y1:clip_y0		(alt)
R44		ctx		/ tstep_c	(alt)
R45		ytop	/ cstep_c	(alt)
R46		edge_l				(alt)
R47		edge_r				(alt)

R56		x0
R57		x1
R58		xcnt
R59		ctx

R60		Scratch (Across Call)
R61		Scratch (Across Call)
R62		z_buf
R63		rgb_buf
*/

	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13

	ADD		R5, R20, R21	
	MOV		R5, R20			|	DMULS	R5, R13, R12
	SHAD.Q	R22, -32, R2	|	SHAD.Q	R23, -32, R3	

	CMPGT	R5, R3
	BF		.L_END

	CMPGT	R23, R21
	BF		.L_END

	LEA.Q	(SP, 32*8), R14
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	(R4, offsetof TKRA_Context_s Blend), R8
	MOV.Q	(R4, offsetof TKRA_Context_s ZaTest), R9
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)
	MOV.Q	R8, (R14, TKRA_DS_BLEND*8)
	MOV.Q	R9, (R14, TKRA_DS_ZATEST*8)

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1		|	ADD		R2, R3, R9
	SHAD	R1, R2, R8	|	SHAD	R1, R9, R9
	ADD		R8, -1, R8	|	ADD		R9, -1, R9
	MOV.Q	R8, (R14, TKRA_DS_XMASK*8)
	MOV.Q	R9, (R14, TKRA_DS_YMASK*8)

	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r

	MOV.Q	(R14, TKRA_DS_CTX*8), R59
	MOV		R59, R19
	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R62
	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R63


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	SHAD.Q	R24, -48, R56	|	SHAD.Q	R26, -48, R57

	SUB		R57, R56, R58		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	|	SHAD.Q	R23, -32, R1	
	SUBS.L	R20, R23, R2	|	SUBS.L	R57, R22, R3	
	SUBS.L	R1, R20, R1		|	SUBS.L	R0, R56, R0
	ADDS.L	R58, -1, R19	|	OR		R2, R3, R3
	OR		R0, R1, R1		|	OR		R3, R19, R2
	OR		R2, R1, R2
	
	CMPGE	0, R2
	BT		.L1
	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0	

	.L1:
	MOV		tkra_spanrcptab, R3
	SUB		R30, R28, R2	|	MOVU.W	(R3, R58), R1
	EXTS.L	R2, R5			|	SHAD.Q	R2, -32, R3
								DMULS.L	R5, R1, R2
								DMULS.L	R3, R1, R3
	/* Jumbo */					MOV		0xFFFEFFFEFFFEFFFE, R19
	SHAD.Q	R2, -16, R2		|	SHAD.Q	R3, -16, R3
								MOVLD	R3, R2, R4
	AND		R10, R19, R2	|	AND		R8, R19, R5

	SHLD.Q	R2, -1, R2		|	SHLD.Q	R5, -1, R3
	SUB			R2, R3, R2	|	PSHUF.W		R1, 0, R18
								PMULS.HW	R2, R18, R2

	SHLD.Q		R2, 1, R5	|	SUBS.L	R26, R24, R18
								DMULS.L	R18, R1, R18

								MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R4,  (R14, TKRA_DS_TSTEP*8)
	SHAD.Q	R18, -16, R18	|	MOV.Q	R8,  (R14, TKRA_DS_CPOS*8)
	EXTS.L	R24, R2			|	MOV.Q	R5,  (R14, TKRA_DS_CSTEP*8)
	EXTS.L	R18, R3			|	MOV.Q	R2,  (R14, TKRA_DS_ZPOS*8)
								MOV.Q	R3,  (R14, TKRA_DS_ZSTEP*8)

	CMPGT	R56, R22
	BF		.L2
								SUBS.L		R22, R56, R1
	SHAD.Q	R4, -32, R3		|	DMULS.L		R4, R1, R2
	PSHUF.W		R1, 0, R0	|	DMULS.L		R3, R1, R3
								MOVLD		R3, R2, R2
								PMULU.HW	R5, R0, R3
	ADD		R28, R2, R2		|	ADD			R8, R3, R3
								MOV.Q	R2, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R3, (R14, TKRA_DS_CPOS*8)
	EXTS.L	R22, R56		|	DMULS.L	R18, R1, R2
								ADDS.L	R24, R2, R2
								MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
	.L2:
	SHAD.Q	R22, -32, R2
	CMPGT	R2, R57
//	MOV?T	R2, R57
	CSELT	R2, R57, R57

	SUBS.L	R57, R56, R58

	CMPGT	0, R58
	BF		.L3

//	MOV.Q	(R14, TKRA_DS_CTX*8), R19

	MOVX	R4, R44
	MOVX	R6, R46

//	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
//	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
	MOVX	R62, R4
	ADD		R12, R56, R3

	MOVX	R20, R40
	MOVX	R22, R42

//	.break
//	MOV.Q	(R59, offsetof TKRA_Context_s DrawSpanZb), R3

//	MOV		R59, R19
	LEA.W	(R4, R3), R4
	LEA.W	(R5, R3), R5
	MOV.Q	(R59, offsetof TKRA_Context_s DrawSpanZb), R3

	MOV.X	R4, R60

	MOV.X	(R14, TKRA_DS_ZPOS*8), R6
	MOV		R58, R5
	JSR		R3

//	MOV		R59, R19
//	MOV.Q	(R14, TKRA_DS_CTX*8), R19
	MOV.X	R60, R6
	MOV		R14, R4
	MOV.Q	(R59, offsetof TKRA_Context_s DrawSpanZt), R3
	MOV		R7, R5
	MOV		R58, R7
	JSR		R3

	MOVX	R44, R4
	MOVX	R46, R6

	MOVX	R40, R20
	MOVX	R42, R22

	.L3:

	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R1
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30

	MOV.X	(SP, 40*8), R40
	MOV.X	(SP, 42*8), R42
	MOV.X	(SP, 44*8), R44
	MOV.X	(SP, 46*8), R46

	MOV.X	(SP, 56*8), R56
	MOV.X	(SP, 58*8), R58
	MOV.X	(SP, 60*8), R60
	MOV.X	(SP, 62*8), R62

	ADD		512, SP
	JMP		R1
	NOP
	NOP

	.L_END_F:
	RTS

.endif

#endif


#if 0
.ifarch has_xgpr

TKRA_WalkEdges_ZbufNZb:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

	MOV.X	R40, (SP, 40*8)
	MOV.X	R42, (SP, 42*8)
	MOV.X	R44, (SP, 44*8)
	MOV.X	R46, (SP, 46*8)

	MOV.X	R56, (SP, 56*8)
	MOV.X	R58, (SP, 58*8)
	MOV.X	R60, (SP, 60*8)
	MOV.X	R62, (SP, 62*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		-
R17		-
R18		-
R19		Scratch


R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r

R40		ycnt / ycur			(alt)
R41		ymax				(alt)
R42		clip_x1:clip_x0		(alt)
R43		clip_y1:clip_y0		(alt)
R44		ctx		/ tstep_c	(alt)
R45		ytop	/ cstep_c	(alt)
R46		edge_l				(alt)
R47		edge_r				(alt)

R56		x0
R57		x1
R58		xcnt
R59		ctx

R60		Scratch (Across Call)
R61		Scratch (Across Call)
R62		z_buf
R63		rgb_buf
*/

	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13

	ADD		R5, R20, R21	
	MOV		R5, R20			|	DMULS	R5, R13, R12
	SHAD.Q	R22, -32, R2	|	SHAD.Q	R23, -32, R3	

	CMPGT	R5, R3
	BF		.L_END

	CMPGT	R23, R21
	BF		.L_END

	LEA.Q	(SP, 32*8), R14
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	(R4, offsetof TKRA_Context_s Blend), R8
	MOV.Q	(R4, offsetof TKRA_Context_s ZaTest), R9
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)
	MOV.Q	R8, (R14, TKRA_DS_BLEND*8)
	MOV.Q	R9, (R14, TKRA_DS_ZATEST*8)

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1		|	ADD		R2, R3, R9
	SHAD	R1, R2, R8	|	SHAD	R1, R9, R9
	ADD		R8, -1, R8	|	ADD		R9, -1, R9
	MOV.Q	R8, (R14, TKRA_DS_XMASK*8)
	MOV.Q	R9, (R14, TKRA_DS_YMASK*8)

	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r

	MOV.Q	(R14, TKRA_DS_CTX*8), R59
	MOV		R59, R19
	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R62
	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R63


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	SHAD.Q	R24, -48, R56	|	SHAD.Q	R26, -48, R57

	SUB		R57, R56, R58		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	|	SHAD.Q	R23, -32, R1	
	SUBS.L	R20, R23, R2	|	SUBS.L	R57, R22, R3	
	SUBS.L	R1, R20, R1		|	SUBS.L	R0, R56, R0
	ADDS.L	R58, -1, R19	|	OR		R2, R3, R3
	OR		R0, R1, R1		|	OR		R3, R19, R2
	OR		R2, R1, R2
	
	CMPGE	0, R2
	BT		.L1
	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0	

	.L1:
	MOV		tkra_spanrcptab, R3
	SUB		R30, R28, R2	|	MOVU.W	(R3, R58), R1
	EXTS.L	R2, R5			|	SHAD.Q	R2, -32, R3
								DMULS.L	R5, R1, R2
								DMULS.L	R3, R1, R3
	/* Jumbo */					MOV		0xFFFEFFFEFFFEFFFE, R19
	SHAD.Q	R2, -16, R2		|	SHAD.Q	R3, -16, R3
								MOVLD	R3, R2, R4
	AND		R10, R19, R2	|	AND		R8, R19, R5

	SHLD.Q	R2, -1, R2		|	SHLD.Q	R5, -1, R3
	SUB			R2, R3, R2	|	PSHUF.W		R1, 0, R18
								PMULS.HW	R2, R18, R2

	SHLD.Q		R2, 1, R5	|	SUBS.L	R26, R24, R18
								DMULS.L	R18, R1, R18

								MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R4,  (R14, TKRA_DS_TSTEP*8)
	SHAD.Q	R18, -16, R18	|	MOV.Q	R8,  (R14, TKRA_DS_CPOS*8)
	EXTS.L	R24, R2			|	MOV.Q	R5,  (R14, TKRA_DS_CSTEP*8)
	EXTS.L	R18, R3			|	MOV.Q	R2,  (R14, TKRA_DS_ZPOS*8)
								MOV.Q	R3,  (R14, TKRA_DS_ZSTEP*8)

	CMPGT	R56, R22
	BF		.L2
								SUBS.L		R22, R56, R1
	SHAD.Q	R4, -32, R3		|	DMULS.L		R4, R1, R2
	PSHUF.W		R1, 0, R0	|	DMULS.L		R3, R1, R3
								MOVLD		R3, R2, R2
								PMULU.HW	R5, R0, R3
	ADD		R28, R2, R2		|	ADD			R8, R3, R3
								MOV.Q	R2, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R3, (R14, TKRA_DS_CPOS*8)
	EXTS.L	R22, R56		|	DMULS.L	R18, R1, R2
								ADDS.L	R24, R2, R2
								MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
	.L2:
	SHAD.Q	R22, -32, R2
	CMPGT	R2, R57
//	MOV?T	R2, R57
	CSELT	R2, R57, R57

	SUBS.L	R57, R56, R58

	CMPGT	0, R58
	BF		.L3

//	MOV.Q	(R14, TKRA_DS_CTX*8), R19

	MOVX	R4, R44
	MOVX	R6, R46

//	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
//	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
	MOVX	R62, R4
	ADD		R12, R56, R3

	MOVX	R20, R40
	MOVX	R22, R42

//	MOV		R59, R19
	LEA.W	(R4, R3), R6
	LEA.W	(R5, R3), R7
//	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZb), R3

//	MOV.X	R4, R60

//	MOV.X	(R14, TKRA_DS_ZPOS*8), R6
//	MOV		R58, R5
//	JSR		R3

//	MOV		R59, R19
//	MOV.Q	(R14, TKRA_DS_CTX*8), R19
//	MOV.X	R60, R6
	MOV		R14, R4
	MOV.Q	(R59, offsetof TKRA_Context_s DrawSpanZt), R3
	MOV		R7, R5
	MOV		R58, R7
	JSR		R3

	MOVX	R44, R4
	MOVX	R46, R6

	MOVX	R40, R20
	MOVX	R42, R22

	.L3:

	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R1
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30

	MOV.X	(SP, 40*8), R40
	MOV.X	(SP, 42*8), R42
	MOV.X	(SP, 44*8), R44
	MOV.X	(SP, 46*8), R46

	MOV.X	(SP, 56*8), R56
	MOV.X	(SP, 58*8), R58
	MOV.X	(SP, 60*8), R60
	MOV.X	(SP, 62*8), R62

	ADD		512, SP
	JMP		R1
	NOP
	NOP

	.L_END_F:
	RTS

.endif

#endif




#if 1
// .ifnarch has_xgpr

TKRA_WalkEdges_Zbuf:
TKRA_WalkEdges_ZbufNZb:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		x0
R17		x1
R18		xcnt
R19		Scratch

R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r
*/

	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13

	ADD		R5, R20, R21	
	MOV		R5, R20			|	DMULS	R5, R13, R12
	SHAD.Q	R22, -32, R2	|	SHAD.Q	R23, -32, R3	

	CMPGT	R5, R3
	BF		.L_END

	CMPGT	R23, R21
	BF		.L_END

	LEA.Q	(SP, 32*8), R14
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	(R4, offsetof TKRA_Context_s Blend), R8
	MOV.Q	(R4, offsetof TKRA_Context_s ZaTest), R9
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)
	MOV.Q	R8, (R14, TKRA_DS_BLEND*8)
	MOV.Q	R9, (R14, TKRA_DS_ZATEST*8)

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1		|	ADD		R2, R3, R9
	SHAD	R1, R2, R8	|	SHAD	R1, R9, R9
	ADD		R8, -1, R8	|	ADD		R9, -1, R9
	MOV.Q	R8, (R14, TKRA_DS_XMASK*8)
	MOV.Q	R9, (R14, TKRA_DS_YMASK*8)

	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	//		x0=(xzpos_l)>>48;
	//		x1=(xzpos_r)>>48;
	SHAD.Q	R24, -48, R16	|	SHAD.Q	R26, -48, R17

//	SHAD.Q	R26, -32, R17		//		x1=(xzpos_r)>>48;
//	ADD		65535, R17
//	SHAD.Q	R17, -16, R17		//		x1=(xzpos_r)>>48;

	SUB		R17, R16, R18		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	|	SHAD.Q	R23, -32, R1	
	SUBS.L	R20, R23, R2	|	SUBS.L	R17, R22, R3	
	SUBS.L	R1, R20, R1		|	SUBS.L	R0, R16, R0
	ADDS.L	R18, -1, R19	|	OR		R2, R3, R3
	OR		R0, R1, R1		|	OR		R3, R19, R2
	OR		R2, R1, R2
	
	CMPGE	0, R2
	BT		.L1
	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0	

	.L1:
	MOV		tkra_spanrcptab, R3
	ADDS.L	R18, 1, R19		
//	SUB		R30, R28, R2	|	MOVU.W	(R3, R19), R1
	SUB		R30, R28, R2	|	MOVU.W	(R3, R18), R1
	EXTS.L	R2, R5			|	SHAD.Q	R2, -32, R3
								DMULS.L	R5, R1, R2
								DMULS.L	R3, R1, R3
	SHAD.Q	R2, -16, R2		|	SHAD.Q	R3, -16, R3
								MOVLD	R3, R2, R4
	/* Jumbo */					MOV		0xFFFEFFFEFFFEFFFE, R3
	AND		R10, R3, R2		|	AND		R8, R3, R5
	SHLD.Q	R2, -1, R2		|	SHLD.Q	R5, -1, R3
	SUB			R2, R3, R2	|	PSHUF.W		R1, 0, R19
								PMULS.HW	R2, R19, R2
	SHLD.Q		R2, 1, R5	|	SUBS.L	R26, R24, R19
								DMULS.L	R19, R1, R19
								SHAD.Q	R19, -16, R19
	EXTS.L	R24, R2			|	EXTS.L	R19, R3

								MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R4,  (R14, TKRA_DS_TSTEP*8)
								MOV.Q	R8,  (R14, TKRA_DS_CPOS*8)
								MOV.Q	R5,  (R14, TKRA_DS_CSTEP*8)
								MOV.Q	R2,  (R14, TKRA_DS_ZPOS*8)
								MOV.Q	R3,  (R14, TKRA_DS_ZSTEP*8)

	CMPGT	R16, R22
	BF		.L2
								SUBS.L	R22, R16, R1
	SHAD.Q	R4, -32, R3		|	DMULS.L		R4, R1, R2
	PSHUF.W		R1, 0, R0	|	DMULS.L		R3, R1, R3
								MOVLD		R3, R2, R2
								PMULU.HW	R5, R0, R3
	ADD		R28, R2, R2		|	ADD			R8, R3, R3
								MOV.Q	R2, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R3, (R14, TKRA_DS_CPOS*8)
	EXTS.L	R22, R16		|	DMULS.L	R19, R1, R2
								ADDS.L	R24, R2, R2
								MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
	.L2:
	SHAD.Q	R22, -32, R2
	CMPGT	R2, R17
	MOV?T	R2, R17

	SUBS.L	R17, R16, R18

	CMPGT	0, R18
	BF		.L3

	MOV.X	R18, (SP, 18*8)

	MOV.Q	(R14, TKRA_DS_CTX*8), R19

	MOV.X	R4, (SP, 4*8)
	MOV.X	R6, (SP, 6*8)

	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
	ADD		R12, R16, R3

	MOV.X	R16, (SP, 16*8)
	MOV.X	R20, (SP, 20*8)
	MOV.X	R22, (SP, 22*8)

	LEA.W	(R4, R3), R4
	LEA.W	(R5, R3), R5
	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZb), R3
	MOV.X	R4, (SP, 48*8)
	MOV.X	(R14, TKRA_DS_ZPOS*8), R6
	MOV		R18, R5
	JSR		R3

	MOV.X	(SP, 48*8), R6
	MOV		R14, R4
	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZt), R3
	MOV		R7, R5
	MOV.Q	(SP, 18*8), R7
	JSR		R3

	MOV.X	(SP, 18*8), R18

	MOV.X	(SP,  4*8), R4
	MOV.X	(SP,  6*8), R6

	MOV.X	(SP, 16*8), R16
	MOV.X	(SP, 20*8), R20
	MOV.X	(SP, 22*8), R22

	.L3:

	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R16
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30

	ADD		512, SP
	JMP		R16
	NOP

	.L_END_F:
	RTS

// .endif

#endif

#if 1
TKRA_WalkEdges_ZbufNcp:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		x0
R17		x1
R18		xcnt
R19		Scratch

R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r
*/

	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13

	ADD		R5, R20, R21	
	MOV		R5, R20			|	DMULS	R5, R13, R12
	SHAD.Q	R22, -32, R2	|	SHAD.Q	R23, -32, R3	

	CMPGT	R5, R3
	BF		.L_END

	CMPGT	R23, R21
	BF		.L_END

	LEA.Q	(SP, 32*8), R14
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	(R4, offsetof TKRA_Context_s Blend), R8
	MOV.Q	(R4, offsetof TKRA_Context_s ZaTest), R9
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)
	MOV.Q	R8, (R14, TKRA_DS_BLEND*8)
	MOV.Q	R9, (R14, TKRA_DS_ZATEST*8)

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1		|	ADD		R2, R3, R9
	SHAD	R1, R2, R8	|	SHAD	R1, R9, R9
	ADD		R8, -1, R8	|	ADD		R9, -1, R9
	MOV.Q	R8, (R14, TKRA_DS_XMASK*8)
	MOV.Q	R9, (R14, TKRA_DS_YMASK*8)

	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	//		x0=(xzpos_l)>>48;
	//		x1=(xzpos_r)>>48;
	SHAD.Q	R24, -48, R16	|	SHAD.Q	R26, -48, R17

//	SHAD.Q	R26, -32, R17		//		x1=(xzpos_r)>>48;
//	ADD		65535, R17
//	SHAD.Q	R17, -16, R17		//		x1=(xzpos_r)>>48;

	SUB		R17, R16, R18		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	|	SHAD.Q	R23, -32, R1	
	SUBS.L	R20, R23, R2	|	SUBS.L	R17, R22, R3	
	SUBS.L	R1, R20, R1		|	SUBS.L	R0, R16, R0
	ADDS.L	R18, -1, R19	|	OR		R2, R3, R3
	OR		R0, R1, R1		|	OR		R3, R19, R2
	OR		R2, R1, R2

#if 0
	CMPGE	0, R2
	BT		.L1
	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0	
#endif

	.L1:
	MOV		tkra_spanrcptab, R3
	ADDS.L	R18, 1, R19		
//	SUB		R30, R28, R2	|	MOVU.W	(R3, R19), R1
	SUB		R30, R28, R2	|	MOVU.W	(R3, R18), R1
	EXTS.L	R2, R5			|	SHAD.Q	R2, -32, R3
								DMULS.L	R5, R1, R2
								DMULS.L	R3, R1, R3
	SHAD.Q	R2, -16, R2		|	SHAD.Q	R3, -16, R3
								MOVLD	R3, R2, R4
	/* Jumbo */					MOV		0xFFFEFFFEFFFEFFFE, R3
	AND		R10, R3, R2		|	AND		R8, R3, R5
	SHLD.Q	R2, -1, R2		|	SHLD.Q	R5, -1, R3
	SUB			R2, R3, R2	|	PSHUF.W		R1, 0, R19
								PMULS.HW	R2, R19, R2
	SHLD.Q		R2, 1, R5	|	SUBS.L	R26, R24, R19
								DMULS.L	R19, R1, R19
								SHAD.Q	R19, -16, R19
	EXTS.L	R24, R2			|	EXTS.L	R19, R3

								MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R4,  (R14, TKRA_DS_TSTEP*8)
								MOV.Q	R8,  (R14, TKRA_DS_CPOS*8)
								MOV.Q	R5,  (R14, TKRA_DS_CSTEP*8)
								MOV.Q	R2,  (R14, TKRA_DS_ZPOS*8)
								MOV.Q	R3,  (R14, TKRA_DS_ZSTEP*8)

#if 0
	CMPGT	R16, R22
	BF		.L2
								SUBS.L	R22, R16, R1
	SHAD.Q	R4, -32, R3		|	DMULS.L		R4, R1, R2
	PSHUF.W		R1, 0, R0	|	DMULS.L		R3, R1, R3
								MOVLD		R3, R2, R2
								PMULU.HW	R5, R0, R3
	ADD		R28, R2, R2		|	ADD			R8, R3, R3
								MOV.Q	R2, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R3, (R14, TKRA_DS_CPOS*8)
	EXTS.L	R22, R16		|	DMULS.L	R19, R1, R2
								ADDS.L	R24, R2, R2
								MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
#endif

	.L2:
//	SHAD.Q	R22, -32, R2
//	CMPGT	R2, R17
//	MOV?T	R2, R17

	SUBS.L	R17, R16, R18

	CMPGT	0, R18
	BF		.L3

	MOV.X	R18, (SP, 18*8)

	MOV.Q	(R14, TKRA_DS_CTX*8), R19

	MOV.X	R4, (SP, 4*8)
	MOV.X	R6, (SP, 6*8)

	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
	ADD		R12, R16, R3

	MOV.X	R16, (SP, 16*8)
	MOV.X	R20, (SP, 20*8)
	MOV.X	R22, (SP, 22*8)

	LEA.W	(R4, R3), R4
	LEA.W	(R5, R3), R5
	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZb), R3
	MOV.X	R4, (SP, 48*8)
	MOV.X	(R14, TKRA_DS_ZPOS*8), R6
	MOV		R18, R5
	JSR		R3

	MOV.X	(SP, 48*8), R6
	MOV		R14, R4
	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZt), R3
	MOV		R7, R5
	MOV.Q	(SP, 18*8), R7
	JSR		R3

	MOV.X	(SP, 18*8), R18

	MOV.X	(SP,  4*8), R4
	MOV.X	(SP,  6*8), R6

	MOV.X	(SP, 16*8), R16
	MOV.X	(SP, 20*8), R20
	MOV.X	(SP, 22*8), R22

	.L3:

	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R16
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30

	ADD		512, SP
	JMP		R16
	NOP

	.L_END_F:
	RTS
#endif

#if 1
TKRA_WalkEdges_Dfl:

	CMPGT	0, R20
	BF		.L_END_F

	SUB		512, SP
	MOV		LR , R16
	MOV.X	R8 , (SP, 8*8)
	MOV.X	R10, (SP, 10*8)
	MOV.X	R12, (SP, 12*8)
	MOV.Q	R14, (SP, 14*8)
	MOV.Q	R16, (SP, 15*8)

	MOV.X	R24, (SP, 24*8)
	MOV.X	R26, (SP, 26*8)
	MOV.X	R28, (SP, 28*8)
	MOV.X	R30, (SP, 30*8)

/*

R0		Scratch
R1		Scratch
R2		Scratch
R3		Scratch

R4		ctx			/ tstep_c
R5		ytop		/ cstep_c
R6		edge_l
R7		edge_r

R8		cpos_l
R9		cstep_l
R10		cpos_r
R11		cstep_r

R12		scr_offs
R13		scr_xs
R14		dsparm
R15		SP

R16		x0
R17		x1
R18		xcnt
R19		Scratch

R20		ycnt / ycur
R21		ymax
R22		clip_x1:clip_x0
R23		clip_y1:clip_y0

R24		xzpos_l
R25		xzstep_l
R26		xzpos_r
R27		xzstep_r

R28		tpos_l
R29		tstep_l
R30		tpos_r
R31		tstep_r
*/

	MOV.X	(R4, offsetof TKRA_Context_s clip_x0), R22
	MOV.L	(R4, offsetof TKRA_Context_s screen_xsize), R13

	ADD		R5, R20, R21	
	MOV		R5, R20			|	DMULS	R5, R13, R12
	SHAD.Q	R22, -32, R2	|	SHAD.Q	R23, -32, R3	

	CMPGT	R5, R3
	BF		.L_END

	CMPGT	R23, R21
	BF		.L_END

	LEA.Q	(SP, 32*8), R14
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img), R2
	MOV.Q	(R4, offsetof TKRA_Context_s tex_img_bcn), R3
	MOV.Q	(R4, offsetof TKRA_Context_s Blend), R8
	MOV.Q	(R4, offsetof TKRA_Context_s ZaTest), R9
	MOV.Q	R4, (R14, TKRA_DS_CTX*8)
	MOV.Q	R2, (R14, TKRA_DS_TEXIMG*8)
	MOV.Q	R3, (R14, TKRA_DS_TEXBCN*8)
	MOV.Q	R8, (R14, TKRA_DS_BLEND*8)
	MOV.Q	R9, (R14, TKRA_DS_ZATEST*8)

	MOVU.B	(R4, offsetof TKRA_Context_s tex_xshl), R2
	MOVU.B	(R4, offsetof TKRA_Context_s tex_yshl), R3
	MOV		1, R1		|	ADD		R2, R3, R9
	SHAD	R1, R2, R8	|	SHAD	R1, R9, R9
	ADD		R8, -1, R8	|	ADD		R9, -1, R9
	MOV.Q	R8, (R14, TKRA_DS_XMASK*8)
	MOV.Q	R9, (R14, TKRA_DS_YMASK*8)

	MOV.X	(R6, TKRA_ES_CPOS*8), R8	//cpos_l, cstep_l
	MOV.X	(R7, TKRA_ES_CPOS*8), R10	//cpos_r, cstep_r

	MOV.X	(R6, TKRA_ES_ZPOS*8), R24	//zpos_l, zstep_l
	MOV.X	(R7, TKRA_ES_ZPOS*8), R26	//zpos_r, zstep_r
	MOV.X	(R6, TKRA_ES_TPOS*8), R28	//tpos_l, tstep_l
	MOV.X	(R7, TKRA_ES_TPOS*8), R30	//tpos_r, tstep_r


	.L0:
	CMPGT	R20, R21		//(ymax>ycur)
	BF		.L4

	SHAD.Q	R24, -48, R16		//		x0=(xzpos_l)>>48;
	SHAD.Q	R26, -48, R17		//		x1=(xzpos_r)>>48;

//	SHAD.Q	R26, -32, R17		//		x1=(xzpos_r)>>48;
//	ADD		65535, R17
//	SHAD.Q	R17, -16, R17		//		x1=(xzpos_r)>>48;

	SUB		R17, R16, R18		//		xcnt=x1-x0;
	
	SHAD.Q	R22, -32, R0	|	SHAD.Q	R23, -32, R1	
	SUBS.L	R20, R23, R2	|	SUBS.L	R17, R22, R3	
	SUBS.L	R1, R20, R1		|	SUBS.L	R0, R16, R0
	ADDS.L	R18, -1, R19	|	OR		R2, R3, R3
	OR		R0, R1, R1		|	OR		R3, R19, R2
	OR		R2, R1, R2
	
	CMPGE	0, R2
	BT		.L1
	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0	

	.L1:
	ADDS.L	R18, 1, R19		|	MOV		tkra_spanrcptab, R3
//	SUB		R30, R28, R2	|	MOVU.W	(R3, R19), R1
	SUB		R30, R28, R2	|	MOVU.W	(R3, R18), R1
	EXTS.L	R2, R5			|	SHAD.Q	R2, -32, R3
								DMULS.L	R5, R1, R2
								DMULS.L	R3, R1, R3
	SHAD.Q	R2, -16, R2		|	SHAD.Q	R3, -16, R3
								MOVLD	R3, R2, R4
	/* Jumbo */					MOV		0xFFFEFFFEFFFEFFFE, R3
	AND		R10, R3, R2		|	AND		R8, R3, R5
	SHLD.Q	R2, -1, R2		|	SHLD.Q	R5, -1, R3
	SUB			R2, R3, R2	|	PSHUF.W		R1, 0, R19
								PMULS.HW	R2, R19, R2
	SHLD.Q		R2, 1, R5	|	SUBS.L	R26, R24, R19
								DMULS	R19, R1, R19
								SHAD.Q	R19, -16, R19
	EXTS.L	R24, R2			|	EXTS.L	R19, R3

								MOV.Q	R28, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R4,  (R14, TKRA_DS_TSTEP*8)
								MOV.Q	R8,  (R14, TKRA_DS_CPOS*8)
								MOV.Q	R5,  (R14, TKRA_DS_CSTEP*8)
								MOV.Q	R2,  (R14, TKRA_DS_ZPOS*8)
								MOV.Q	R3,  (R14, TKRA_DS_ZSTEP*8)

	CMPGT	R16, R22
	BF		.L2
								SUBS.L	R22, R16, R1
	SHAD.Q	R4, -32, R3		|	DMULS		R4, R1, R2
	PSHUF.W		R1, 0, R0	|	DMULS		R3, R1, R3
								MOVLD		R3, R2, R2
								PMULU.HW	R5, R0, R3
	ADD		R28, R2, R2		|	ADD			R8, R3, R3
								MOV.Q	R2, (R14, TKRA_DS_TPOS*8)
								MOV.Q	R3, (R14, TKRA_DS_CPOS*8)
	MOV		R22, R16		|	DMULS	R19, R1, R2
								ADDS.L	R24, R2, R2
								MOV.Q	R2, (R14, TKRA_DS_ZPOS*8)
	.L2:
	SHAD.Q	R22, -32, R2
	CMPGT	R2, R17
	MOV?T	R2, R17

	SUB		R17, R16, R18

	CMPGT	0, R18
	BF		.L3

	MOV.X	R4, (SP, 4*8)
	MOV.X	R6, (SP, 6*8)
	MOV.X	R16, (SP, 16*8)
	MOV.X	R18, (SP, 18*8)
	MOV.Q	(R14, TKRA_DS_CTX*8), R19
	MOV.X	R20, (SP, 20*8)
	MOV.Q	(R19, offsetof TKRA_Context_s screen_zbuf), R4
	MOV.X	R22, (SP, 22*8)
	MOV.Q	(R19, offsetof TKRA_Context_s screen_rgb), R5
	ADD		R12, R16, R3
//	LEA.W	(R4, R3), R4
//	LEA.W	(R5, R3), R5
	LEA.W	(R4, R3), R6
	LEA.W	(R5, R3), R7

//	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZb), R3
//	MOV.X	R4, (SP, 48*8)
//	MOV		R18, R5
//	MOV.X	(R14, TKRA_DS_ZPOS*8), R6
//	JSR		R3

//	MOV.X	(SP, 48*8), R6
	MOV		R14, R4
	MOV		R7, R5
	MOV.Q	(SP, 18*8), R7
//	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpanZt), R3
	MOV.Q	(R19, offsetof TKRA_Context_s DrawSpan), R3
	JSR		R3

	MOV.X	(SP,  4*8), R4
	MOV.X	(SP,  6*8), R6
	MOV.X	(SP, 16*8), R16
	MOV.X	(SP, 18*8), R18
	MOV.X	(SP, 20*8), R20
	MOV.X	(SP, 22*8), R22

	.L3:

	ADD		R25, R24	|	ADD		R27, R26
	ADD		R29, R28	|	ADD		R31, R30
	ADD		R9 , R8		|	ADD		R11, R10
	ADD		R13, R12	|	ADD		1, R20
	BRA		.L0

	.L4:

	MOV.X	R8, (R6, TKRA_ES_CPOS*8)	//cpos_l, cstep_l
	MOV.X	R10, (R7, TKRA_ES_CPOS*8)	//cpos_r, cstep_r

	MOV.X	R24, (R6, TKRA_ES_ZPOS*8)	//zpos_l, zstep_l
	MOV.X	R26, (R7, TKRA_ES_ZPOS*8)	//zpos_r, zstep_r
	MOV.X	R28, (R6, TKRA_ES_TPOS*8)	//tpos_l, tstep_l
	MOV.X	R30, (R7, TKRA_ES_TPOS*8)	//tpos_r, tstep_r

	.L_END:

	MOV.X	(SP,  8*8), R8
	MOV.X	(SP, 10*8), R10
	MOV.X	(SP, 12*8), R12
	MOV.Q	(SP, 14*8), R14
	MOV.Q	(SP, 15*8), R16
	MOV.X	(SP, 24*8), R24
	MOV.X	(SP, 26*8), R26
	MOV.X	(SP, 28*8), R28
	MOV.X	(SP, 30*8), R30
	ADD		512, SP
	JMP		R16
	NOP

	.L_END_F:
	RTS
#endif

};
#endif

/*
 * 0: Not Configured
 * 1: Permanently disable MMIO Edge-Walker (eg: Not Present)
 * 2: Use Edge Walker
 * 3: Temporarily disable Edge Walker (eg: Texture Incompatibilty)
 */
static int tkra_nommio;

#ifdef __BJX2__

void TKRA_Probe_WalkEdgesMMIO(TKRA_Context *ctx)
{
	volatile u64 *mmio;
	u64 tex, scrn, zbuf, sts;
	int n;

	if(tkra_nommio==1)
		return;

//	if(tkra_nommio==3)
//		tkra_nommio=2;

	if(tkra_nommio&2)
	{
		tkra_nommio&=~1;
		return;
	}

#if 0
	tex=(u64)(ctx->tex_img_bcn);
	if(tkra_nommio&2)
	{
		if(!tex)
			tkra_nommio|=1;
	
		if(tex&0xFFFFF0000000ULL)
		{
			tkra_nommio|=1;
		}
		
//		if(((tex>>57)&7)!=0)
//			tkra_nommio|=1;

		return;
	}
#endif

	scrn=(u64)(ctx->screen_rgb);
	zbuf=(u64)(ctx->screen_zbuf);
	
	if(scrn&0xFFFFF0000000ULL)
		tkra_nommio=1;
	if(zbuf&0xFFFFF0000000ULL)
		tkra_nommio=1;

	mmio=(volatile u64 *)0xFFFFF000C000ULL;

	mmio[0]=0x00F2; n=16;
	while(n--)
		sts=mmio[0];

	if((sts&0x00FA)!=0x00F0)
		tkra_nommio=1;

	if(tkra_nommio&1)
	{
//		__debugbreak();
		return;
	}

	tkra_nommio|=2;

	if(tex&0xFFFFF0000000ULL)
	{
		tkra_nommio|=1;
	}
}

void TKRA_WalkEdges_MMIO(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt)
{
	static byte blfntab[16]={
		0x6, 0x7, 0x9, 0xD, 0x8, 0xC, 0xA, 0xE, 0xB, 0xF
	};
	volatile u64 *mmio;
	u64 tex, scrn, zbuf, sts, mdfl;
	int clip_x0, clip_x1, clip_y0, clip_y1, ymax;
	
	tex=(u64)(ctx->tex_img_bcn);
	scrn=(u64)(ctx->screen_rgb);
	zbuf=(u64)(ctx->screen_zbuf);
	
	if(tkra_nommio&1)
	{
		TKRA_WalkEdges_Zbuf(ctx, ytop, edge_l, edge_r, cnt);
		return;
	}

	if(!tex || (tex&0xFFFFF0000000ULL) || (((tex>>57)&7)!=0))
	{
		TKRA_WalkEdges_Zbuf(ctx, ytop, edge_l, edge_r, cnt);
		return;
	}
	
	if(cnt<=0)
		return;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

	if(ytop>=clip_y1)
		return;
	ymax=ytop+cnt;
	if(ymax<=clip_y0)
		return;
	if(ymax>=clip_y1)
		cnt=clip_y1-ytop;

	mmio=(volatile u64 *)0xFFFFF000C000ULL;
	
	sts=mmio[0];
	while(sts&1)
	{
		sts=mmio[0];
	}

	mmio[3]=tex;

#if 1
	mmio[4]=edge_l[TKRA_ES_ZPOS];
	mmio[5]=edge_r[TKRA_ES_ZPOS];
	mmio[6]=edge_l[TKRA_ES_ZSTEP];
	mmio[7]=edge_r[TKRA_ES_ZSTEP];
#endif

	mmio[ 8]=edge_l[TKRA_ES_TPOS];
	mmio[ 9]=edge_r[TKRA_ES_TPOS];
	mmio[10]=edge_l[TKRA_ES_TSTEP];
	mmio[11]=edge_r[TKRA_ES_TSTEP];

	mmio[12]=edge_l[TKRA_ES_CPOS];
	mmio[13]=edge_r[TKRA_ES_CPOS];
	mmio[14]=edge_l[TKRA_ES_CSTEP];
	mmio[15]=edge_r[TKRA_ES_CSTEP];
	
	mmio[16]=
		(((u64)clip_x0)<< 0) |
		(((u64)clip_y0)<<16) |
		(((u64)clip_x1)<<32) |
		(((u64)clip_y1)<<48) ;

	mmio[1]=((u16)ytop) | (((u64)(ytop+cnt-1))<<16) |
		(((u64)(ctx->screen_xsize))<<32);
	mmio[2]=((u32)scrn) | (((u64)zbuf)<<32);

	mdfl=0x0021;
	if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
		mdfl|=0x10;
	if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
		mdfl|=0xC0;

	if(ctx->span_trifl&TKRA_TRFL_NOZWRITE)
		mdfl&=~0x80;

	mdfl|=
			(blfntab[ctx->blend_sfunc]<< 8) |
		(blfntab[ctx->blend_dfunc]<<12) ;

	mmio[0]=mdfl;

//	mmio[0]=
//		mdfl|((ctx->screen_xsize)<<16);
}
#endif

#ifndef __BJX2__
// #if 1
/* Z-Buffer */
void TKRA_WalkEdges_Zbuf(TKRA_Context *ctx,
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
//	int xpos_l, xpos_r, xstep_l, xstep_r;
//	int zpos_l, zpos_r, zstep_l, zstep_r;

	register	s64 xzpos_l, xzstep_l;
				s64 xzpos_r, xzstep_r;
	register	u64 tpos_l, tstep_l;
				u64 tpos_r, tstep_r;
	register	u64 cpos_l, cstep_l;
				u64 cpos_r, cstep_r;
	u64 tstep_d, tstep_c;
	u64 cstep_d, cstep_c;
	int zstep_d, zstep_c;
	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		y, ymax, x0, x1, x2, x3, z0, xcnt, xrcp;
	int		xshl, yshl;
	int		scr_xs, scr_ys, scr_zxs;


	if(cnt<=0)
		return;

	DrawSpan	= ctx->DrawSpanZt;
	DrawSpanZ	= ctx->DrawSpanZb;
	
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;
	scr_zxs = scr_xs;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

	if(ytop>=clip_y1)
		return;
	ymax=ytop+cnt;
	if(ymax<=clip_y0)
		return;

	dsparm[TKRA_DS_CTX]=(u64)(ctx);
	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);
	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);
	dsparm[TKRA_DS_BLEND]=(u64)(ctx->Blend);
	dsparm[TKRA_DS_ZATEST]=(u64)(ctx->ZaTest);

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;
	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;
	
	tpos_l	= edge_l[TKRA_ES_TPOS];
	tpos_r	= edge_r[TKRA_ES_TPOS];
	tstep_l	= edge_l[TKRA_ES_TSTEP];
	tstep_r	= edge_r[TKRA_ES_TSTEP];
	
	cpos_l	= edge_l[TKRA_ES_CPOS];
	cpos_r	= edge_r[TKRA_ES_CPOS];
	cstep_l	= edge_l[TKRA_ES_CSTEP];
	cstep_r	= edge_r[TKRA_ES_CSTEP];
	
	xzpos_l		= edge_l[TKRA_ES_ZPOS];
	xzpos_r		= edge_r[TKRA_ES_ZPOS];
	xzstep_l	= edge_l[TKRA_ES_ZSTEP];
	xzstep_r	= edge_r[TKRA_ES_ZSTEP];
	
	cb0=scr_cb+(ytop*scr_xs);
	zb0=scr_zb+(ytop*scr_xs);

	ymax=ytop+cnt;
	for(y=ytop; y<ymax; y++)
	{
		x0=(xzpos_l)>>48;
//		x0=(int)(xzpos_l>>48);
//		x1=(xzpos_r+0x0000FFFF00000000LL)>>48;
		x1=(xzpos_r)>>48;

		xcnt=x1-x0;

		x2=	(y -clip_y0) | (clip_y1- y) |
			(x1-clip_x0) | (clip_x1-x0) |
			(xcnt-1);

		if(x2<0)
		{
			tpos_l+=tstep_l;	tpos_r+=tstep_r;
			cpos_l+=cstep_l;	cpos_r+=cstep_r;
			xzpos_l+=xzstep_l;	xzpos_r+=xzstep_r;
			cb0=cb0+scr_xs;		zb0=zb0+scr_xs;
			continue;
		}

		xrcp=TKRA_SpanRcp(xcnt+1);
		tstep_c=TKRA_CalcTexStepRcp(tpos_r, tpos_l, xrcp);
		cstep_c=TKRA_CalcClrStepRcp(cpos_r, cpos_l, xrcp);
//		zstep_d=zpos_r-zpos_l;
		zstep_d=xzpos_r-xzpos_l;
//		zstep_d=(s32)(xzpos_r-xzpos_l);
//		zstep_d=((s32)xzpos_r)-((s32)xzpos_l);
		zstep_c=tkra_dmuls(zstep_d, xrcp)>>16;

		z0=xzpos_l;
		dsparm[TKRA_DS_TPOS		]=tpos_l;
		dsparm[TKRA_DS_TSTEP	]=tstep_c;
		dsparm[TKRA_DS_CPOS		]=cpos_l;
		dsparm[TKRA_DS_CSTEP	]=cstep_c;
		dsparm[TKRA_DS_ZPOS		]=z0;
		dsparm[TKRA_DS_ZSTEP	]=zstep_c;

		if(x0<clip_x0)
		{
			x2=clip_x0-x0;
			z0+=zstep_c*x2;
			dsparm[TKRA_DS_TPOS		]+=tstep_c*x2;
			dsparm[TKRA_DS_CPOS		]+=cstep_c*x2;
			dsparm[TKRA_DS_ZPOS		]=z0;
			x0+=x2;
			xcnt=x1-x0;
		}
		
		if(x1>=clip_x1)
		{
			x1=clip_x1-1;
			xcnt=x1-x0;
		}

		if(xcnt>0)
		{
			zb=zb0+x0;		cb=cb0+x0;
			DrawSpanZ(zb, xcnt, z0, zstep_c);
			DrawSpan(dsparm, cb, zb, xcnt);
		}

		tpos_l+=tstep_l;	tpos_r+=tstep_r;
		cpos_l+=cstep_l;	cpos_r+=cstep_r;
		xzpos_l+=xzstep_l;	xzpos_r+=xzstep_r;
		cb0=cb0+scr_xs;		zb0=zb0+scr_xs;
	}

	edge_l[TKRA_ES_TPOS] = tpos_l;
	edge_r[TKRA_ES_TPOS] = tpos_r;
	edge_l[TKRA_ES_CPOS] = cpos_l;
	edge_r[TKRA_ES_CPOS] = cpos_r;
	edge_l[TKRA_ES_ZPOS] = xzpos_l;
	edge_r[TKRA_ES_ZPOS] = xzpos_r;
}
#endif

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
	int y0, y1, y2, y3, y3m, y3n;
	int x0, x1, x2, x3, x3m, x3n;
	int y1cnt, y2cnt, y3cnt, y1rcp, y2rcp, y3rcp;
	u64 t0, t1, t2;
	s64 l1, l2, l3;

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
	
	if(!RasterWalkEdges)
		return;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;

	y0=vec0[TKRA_VX_YPOS];
	y1=vec1[TKRA_VX_YPOS];
	y2=vec2[TKRA_VX_YPOS];
	
#if 1
	y3m=y0;	y3n=y0;
	if(y1<y3m)y3m=y1;
	if(y1>y3n)y3n=y1;
	if(y2<y3m)y3m=y2;
	if(y2>y3n)y3n=y2;

	x0=vec0[TKRA_VX_XPOS];
	x1=vec1[TKRA_VX_XPOS];
	x2=vec2[TKRA_VX_XPOS];
	
	x3m=x0;	x3n=x0;
	if(x1<x3m)x3m=x1;
	if(x1>x3n)x3n=x1;
	if(x2<x3m)x3m=x2;
	if(x2>x3n)x3n=x2;
	
	y3m=y3m>>16;	y3n=y3n>>16;
	x3m=x3m>>16;	x3n=x3n>>16;
	
	if(	(x3m>ctx->clip_x0) && (x3n<ctx->clip_x1) &&
		(y3m>ctx->clip_y0) && (y3n<ctx->clip_y1))
	{
		RasterWalkEdges = ctx->RasterWalkEdgesNcp;
	}
#endif

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
	
//	y0=tve0[TKRA_VX_YPOS]>>16;
//	y1=tve1[TKRA_VX_YPOS]>>16;
//	y2=tve2[TKRA_VX_YPOS]>>16;

//	y0=(tve0[TKRA_VX_YPOS]+32767)>>16;
//	y0=(tve0[TKRA_VX_YPOS]+    0)>>16;
//	y0=(tve0[TKRA_VX_YPOS]+16383)>>16;
//	y1=(tve1[TKRA_VX_YPOS]+32767)>>16;
//	y2=(tve2[TKRA_VX_YPOS]+32767)>>16;
//	y2=(tve2[TKRA_VX_YPOS]+49151)>>16;
//	y2=(tve2[TKRA_VX_YPOS]+65535)>>16;

	y0=(tve0[TKRA_VX_YPOS]+32767)>>16;
	y1=(tve1[TKRA_VX_YPOS]+32767)>>16;
	y2=(tve2[TKRA_VX_YPOS]+32767)>>16;

//	y0=(tve0[TKRA_VX_YPOS]+16383)>>16;
//	y1=(tve1[TKRA_VX_YPOS]+32767)>>16;
//	y2=(tve2[TKRA_VX_YPOS]+49151)>>16;

	y1cnt=y1-y0;
	y2cnt=y2-y0;
	y3cnt=y2-y1;
	y1rcp=TKRA_SpanRcp(y1cnt+1);
	y2rcp=TKRA_SpanRcp(y2cnt+1);
	y3rcp=TKRA_SpanRcp(y3cnt+1);

	t0=tve0[TKRA_VX_XPOS];
	t1=tve1[TKRA_VX_XPOS];
	t2=tve2[TKRA_VX_XPOS];

#if 1
	x1step_d=t1-t0;
//	x1step_c=(((s64)x1step_d)*y1rcp)>>16;
//	x1step_c=tkra_dmuls(x1step_d, y1rcp)>>16;
	x1step_c=(tkra_dmuls(x1step_d, y1rcp)+32767)>>16;

//	y3=tkra_dmuls(x1step_d, y1rcp);
//	x1step_c=(y3+32767)>>16;

	x2step_d=t2-t0;
//	x2step_c=(((s64)x2step_d)*y2rcp)>>16;
//	x2step_c=tkra_dmuls(x2step_d, y2rcp)>>16;
	x2step_c=(tkra_dmuls(x2step_d, y2rcp)+32767)>>16;

	x3step_d=t2-t1;
//	x3step_c=(((s64)x3step_d)*y3rcp)>>16;
//	x3step_c=tkra_dmuls(x3step_d, y3rcp)>>16;
	x3step_c=(tkra_dmuls(x3step_d, y3rcp)+32767)>>16;
#endif

#if 0
	x1step_d=t1-t0;
	l3=tkra_dmuls(x1step_d, y1rcp);
	if(l3>=0)	l3+=49151;
		else	l3+=16383;
	x1step_c=l3>>16;

	x2step_d=t2-t0;
	l3=tkra_dmuls(x2step_d, y2rcp);
	if(l3>=0)	l3+=49151;
		else	l3+=16383;
	x2step_c=l3>>16;

	x3step_d=t2-t1;
	l3=tkra_dmuls(x3step_d, y3rcp);
	if(l3>=0)	l3+=49151;
		else	l3+=16383;
	x3step_c=l3>>16;

//	x1step_c=(tkra_dmuls(x1step_d, y1rcp)+32767)>>16;
//	x2step_c=(tkra_dmuls(x2step_d, y2rcp)+32767)>>16;
//	x3step_c=(tkra_dmuls(x3step_d, y3rcp)+32767)>>16;
#endif

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
//	e1_parm[TKRA_ES_ZSTEP]=z1step_c;
	e1_parm[TKRA_ES_ZSTEP]=((u32)z1step_c) | (((s64)x1step_c)<<32);

	e2_parm[TKRA_ES_XSTEP]=x2step_c;	/* v2-v0 */
	e2_parm[TKRA_ES_TSTEP]=t2step_c;
	e2_parm[TKRA_ES_CSTEP]=c2step_c;
//	e2_parm[TKRA_ES_ZSTEP]=z2step_c;
	e2_parm[TKRA_ES_ZSTEP]=((u32)z2step_c) | (((s64)x2step_c)<<32);

	e3_parm[TKRA_ES_XSTEP]=x3step_c;	/* v2-v1 */
	e3_parm[TKRA_ES_TSTEP]=t3step_c;
	e3_parm[TKRA_ES_CSTEP]=c3step_c;
//	e3_parm[TKRA_ES_ZSTEP]=z3step_c;
	e3_parm[TKRA_ES_ZSTEP]=((u32)z3step_c) | (((s64)x3step_c)<<32);

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
		e2_parm[TKRA_ES_XPOS]+=32768;
		RasterWalkEdges(ctx, y0, e1_parm, e2_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e3_parm, e2_parm, y2-y1);
	}else
	{
		e1_parm[TKRA_ES_XPOS]+=32768;
		e3_parm[TKRA_ES_XPOS]+=32768;

		RasterWalkEdges(ctx, y0, e2_parm, e1_parm, y1-y0);
		RasterWalkEdges(ctx, y1, e2_parm, e3_parm, y2-y1);
	}
	
}


#if 1
void TKRA_WalkQuad_FillEParm(TKRA_Context *ctx,
	u64 *vec0, u64 *vec1, u64 *e_parm)
{
	int x1step_d, x1step_c;
	u64 t1step_d, t1step_c;
	u64 c1step_d, c1step_c;
	int z1step_d, z1step_c;
	u64 t0, t1, t2;

	int y0, y1, y1cnt, y1rcp;

	y0=(vec0[TKRA_VX_YPOS]+32767)>>16;
	y1=(vec1[TKRA_VX_YPOS]+32767)>>16;

	if(y1==y0)
	{
		e_parm[TKRA_ES_XPOS]=vec0[TKRA_VX_XPOS];
		e_parm[TKRA_ES_TPOS]=vec0[TKRA_VX_TPOS];
		e_parm[TKRA_ES_CPOS]=vec0[TKRA_VX_CPOS];
		e_parm[TKRA_ES_ZPOS]=vec0[TKRA_VX_ZPOS];

		e_parm[TKRA_ES_XSTEP]=0;
		e_parm[TKRA_ES_TSTEP]=0;
		e_parm[TKRA_ES_CSTEP]=0;
		e_parm[TKRA_ES_ZSTEP]=0;
		
		return;
	}

	y1cnt=y1-y0;
	if(y1cnt<0)
		y1cnt=-y1cnt;
	
//	y1rcp=TKRA_SpanRcp(y1cnt+1);
	y1rcp=TKRA_SpanRcp(y1cnt+0);

	t0=vec0[TKRA_VX_XPOS];	t1=vec1[TKRA_VX_XPOS];
	x1step_d=t1-t0;
	x1step_c=(tkra_dmuls(x1step_d, y1rcp)+32767)>>16;

	t0=vec0[TKRA_VX_TPOS];	t1=vec1[TKRA_VX_TPOS];
	t1step_c=TKRA_CalcTexStepRcp(t1, t0, y1rcp);

	t0=vec0[TKRA_VX_CPOS];	t1=vec1[TKRA_VX_CPOS];
	c1step_c=TKRA_CalcClrStepRcp(t1, t0, y1rcp);

	t0=vec0[TKRA_VX_ZPOS];	t1=vec1[TKRA_VX_ZPOS];
	z1step_d=t1-t0;
	z1step_c=tkra_dmuls(z1step_d, y1rcp)>>16;

//	z1step_c = 0;

	e_parm[TKRA_ES_XPOS]=vec0[TKRA_VX_XPOS];
	e_parm[TKRA_ES_TPOS]=vec0[TKRA_VX_TPOS];
	e_parm[TKRA_ES_CPOS]=vec0[TKRA_VX_CPOS];
	e_parm[TKRA_ES_ZPOS]=vec0[TKRA_VX_ZPOS];

	e_parm[TKRA_ES_XSTEP]=x1step_c;
	e_parm[TKRA_ES_TSTEP]=t1step_c;
	e_parm[TKRA_ES_CSTEP]=c1step_c;
	e_parm[TKRA_ES_ZSTEP]=((u32)z1step_c) | (((s64)x1step_c)<<32);
}

/*
Draw a Quad
Given 4 vertices in Screen Space.
*/
void TKRA_WalkQuadB(TKRA_Context *ctx,
	u64 *vec0, u64 *vec1, u64 *vec2, u64 *vec3)
{
	u64	e1_parm[TKRA_ES_NPARM];
	u64	e2_parm[TKRA_ES_NPARM];
	u64	e3_parm[TKRA_ES_NPARM];
//	u64	e4_parm[TKRA_ES_NPARM];	

	u64	e5_parm[TKRA_ES_NPARM];
	u64	e6_parm[TKRA_ES_NPARM];
	u64	e7_parm[TKRA_ES_NPARM];
//	u64	e8_parm[TKRA_ES_NPARM];

	void (*RasterWalkEdges)(TKRA_Context *ctx,
		int ytop, u64 *edge_l, u64 *edge_r, int cnt);
	u64 *tve0, *tve1, *tve2, *tve3;
	u64 *tve4, *tve5, *tve6, *tve7;
	int y0, y1, y2, y3, y3m, y3n;
	int x0, x1, x2, x3, x3m, x3n;
	int y1cnt, y2cnt, y3cnt, y4cnt, y1rcp, y2rcp, y3rcp, y4rcp;
	u64 t0, t1, t2;
	s64 l1, l2, l3;
	int i;

//	int xshl, yshl;

	RasterWalkEdges = ctx->RasterWalkEdges;
	
	if(!RasterWalkEdges)
		return;

//	xshl=ctx->tex_xshl;
//	yshl=ctx->tex_yshl;

	y0=vec0[TKRA_VX_YPOS];
	y1=vec1[TKRA_VX_YPOS];
	y2=vec2[TKRA_VX_YPOS];
	y3=vec3[TKRA_VX_YPOS];

	x0=vec0[TKRA_VX_XPOS];
	x1=vec1[TKRA_VX_XPOS];
	x2=vec2[TKRA_VX_XPOS];
	x3=vec3[TKRA_VX_XPOS];
	
	y0=y0>>16;	y1=y1>>16;
	y2=y2>>16;	y3=y3>>16;
	x0=x0>>16;	x1=x1>>16;
	x2=x2>>16;	x3=x3>>16;
	
	i=0;
//	if((y0==y1) && (x0==x1))	i|=1;
//	if((y1==y2) && (x1==x2))	i|=2;
//	if((y2==y3) && (x2==x3))	i|=4;
//	if((y3==y0) && (x3==x0))	i|=8;

	i =((y0==y1)&(x0==x1))   ;
	i|=((y1==y2)&(x1==x2))<<1;
	i|=((y2==y3)&(x2==x3))<<2;
	i|=((y3==y0)&(x3==x0))<<3;
	
	if(i)
	{
		/* Deal with colinear points. */
		if(i==1)
			{ TKRA_WalkTriangle(ctx, vec0, vec2, vec3); }
		else if(i==2)
			{ TKRA_WalkTriangle(ctx, vec0, vec1, vec3); }
		else if(i==4)
			{ TKRA_WalkTriangle(ctx, vec0, vec1, vec2); }
		else if(i==8)
			{ TKRA_WalkTriangle(ctx, vec0, vec1, vec2); }
		return;
	}
	
#if 1
	y3m=y0;	y3n=y0;
	if(y1<y3m)y3m=y1;
	if(y1>y3n)y3n=y1;
	if(y2<y3m)y3m=y2;
	if(y2>y3n)y3n=y2;
	if(y3<y3m)y3m=y3;
	if(y3>y3n)y3n=y3;

	x3m=x0;	x3n=x0;
	if(x1<x3m)x3m=x1;
	if(x1>x3n)x3n=x1;
	if(x2<x3m)x3m=x2;
	if(x2>x3n)x3n=x2;
	if(x3<x3m)x3m=x3;
	if(x3>x3n)x3n=x3;
	
//	y3m=y3m>>16;	y3n=y3n>>16;
//	x3m=x3m>>16;	x3n=x3n>>16;
	
	if(	((x3m-4)>ctx->clip_x0) && ((x3n+4)<ctx->clip_x1) &&
		((y3m-4)>ctx->clip_y0) && ((y3n+4)<ctx->clip_y1))
	{
		RasterWalkEdges = ctx->RasterWalkEdgesNcp;
	}
#endif

	/* Find starting vertex with smallest Y position  */
	if(y0<=y1)
	{
		if(y0<=y2)
		{
			if(y0<=y3)
				{	tve0=vec0;	tve1=vec1;	tve2=vec2;	tve3=vec3;	}
			else
				{	tve0=vec3;	tve1=vec0;	tve2=vec1;	tve3=vec2;	}
		}else
		{
			if(y2<=y3)
				{	tve0=vec2;	tve1=vec3;	tve2=vec0;	tve3=vec1;	}
			else
				{	tve0=vec3;	tve1=vec0;	tve2=vec1;	tve3=vec2;	}
		}
	}else
	{
		if(y1<=y2)
		{
			if(y1<=y3)
				{	tve0=vec1;	tve1=vec2;	tve2=vec3;	tve3=vec0;	}
			else
				{	tve0=vec3;	tve1=vec0;	tve2=vec1;	tve3=vec2;	}
		}else
		{
			if(y2<=y3)
				{	tve0=vec2;	tve1=vec3;	tve2=vec0;	tve3=vec1;	}
			else
				{	tve0=vec3;	tve1=vec0;	tve2=vec1;	tve3=vec2;	}
		}
	}

	y0=tve0[TKRA_VX_YPOS];
	y1=tve1[TKRA_VX_YPOS];
	y2=tve2[TKRA_VX_YPOS];
	y3=tve3[TKRA_VX_YPOS];

#if 1
	if((y0==y1) || (y0==y3))
	{
		x0=tve0[TKRA_VX_XPOS];
		x1=tve1[TKRA_VX_XPOS];
		x2=tve2[TKRA_VX_XPOS];
		x3=tve3[TKRA_VX_XPOS];

		if((y0==y1) && (x1<x0))
		{
			tve4=tve0;	tve0=tve1;
			tve1=tve2;	tve2=tve3;
			tve3=tve4;
		}else
			if((y0==y3) && (x3<x0))
		{
			tve4=tve3;	tve3=tve2;
			tve2=tve1;	tve1=tve0;
			tve0=tve4;
		}
	}
#endif
	
//	if(y0>y1)
//		{ __debugbreak(); }
//	if(y1>y2)
//		{ __debugbreak(); }
	
	y0=(tve0[TKRA_VX_YPOS]+32767)>>16;
	y1=(tve1[TKRA_VX_YPOS]+32767)>>16;
	y2=(tve2[TKRA_VX_YPOS]+32767)>>16;
	y3=(tve3[TKRA_VX_YPOS]+32767)>>16;

	x0=tve0[TKRA_VX_XPOS];
	x1=tve1[TKRA_VX_XPOS];
	x2=tve2[TKRA_VX_XPOS];
	x3=tve3[TKRA_VX_XPOS];


	TKRA_WalkQuad_FillEParm(ctx, tve0, tve1, e1_parm);

	if(y2>=y1)
		TKRA_WalkQuad_FillEParm(ctx, tve1, tve2, e2_parm);
	if(y3>=y2)
		TKRA_WalkQuad_FillEParm(ctx, tve2, tve3, e3_parm);
//	TKRA_WalkQuad_FillEParm(ctx, tve3, tve0, e4_parm);

	TKRA_WalkQuad_FillEParm(ctx, tve0, tve3, e5_parm);

	if(y2>=y3)
		TKRA_WalkQuad_FillEParm(ctx, tve3, tve2, e6_parm);
	if(y1>=y2)
		TKRA_WalkQuad_FillEParm(ctx, tve2, tve1, e7_parm);
//	TKRA_WalkQuad_FillEParm(ctx, tve1, tve0, e8_parm);

	if(x3<x1)
	{
		/* Something weird here. */
		TKRA_WalkTriangle(ctx, vec0, vec1, vec2);
		TKRA_WalkTriangle(ctx, vec0, vec2, vec3);		
	}
	
	e5_parm[TKRA_ES_XPOS]+=32768;
	e6_parm[TKRA_ES_XPOS]+=32768;
	e7_parm[TKRA_ES_XPOS]+=32768;
	
	if((y0==y3) && (y1==y2))
	{
		RasterWalkEdges(ctx, y3, e1_parm, e6_parm, y1-y0);
	}else
		if(y1<y3)
	{
		if(y3<y2)
		{
			RasterWalkEdges(ctx, y0, e1_parm, e5_parm, y1-y0);
			RasterWalkEdges(ctx, y1, e2_parm, e5_parm, y3-y1);
			RasterWalkEdges(ctx, y3, e2_parm, e6_parm, y2-y3);
		}else
		{
			RasterWalkEdges(ctx, y0, e1_parm, e5_parm, y1-y0);
			RasterWalkEdges(ctx, y1, e2_parm, e5_parm, y2-y1);
			RasterWalkEdges(ctx, y2, e3_parm, e5_parm, y3-y2);
		}
	}else
	{
		if(y1<y2)
		{
			RasterWalkEdges(ctx, y0, e1_parm, e5_parm, y3-y0);
			RasterWalkEdges(ctx, y3, e1_parm, e6_parm, y1-y3);
			RasterWalkEdges(ctx, y1, e2_parm, e6_parm, y2-y1);
		}else
		{
			RasterWalkEdges(ctx, y0, e1_parm, e5_parm, y3-y0);
			RasterWalkEdges(ctx, y3, e1_parm, e6_parm, y2-y3);
			RasterWalkEdges(ctx, y2, e1_parm, e7_parm, y1-y2);
		}
	}
}
#endif

void TKRA_WalkQuad(TKRA_Context *ctx,
	u64 *vec0, u64 *vec1, u64 *vec2, u64 *vec3)
{
//	TKRA_WalkTriangle(ctx, vec0, vec1, vec2);
//	TKRA_WalkTriangle(ctx, vec0, vec2, vec3);

	TKRA_WalkQuadB(ctx, vec0, vec1, vec2, vec3);
}


#if 1
void TKRA_WalkPoint(TKRA_Context *ctx, u64 *vec0)
{
	void (*DrawSpan)(u64 *parm,
		tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
	void (*DrawSpanZ)(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);

	u64	dsparm[TKRA_DS_NPARM];
	tkra_rastpixel *scr_cb, *cb;
	tkra_zbufpixel *scr_zb, *zb;
	int x0, y0, z0;

	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		xshl, yshl;
	int		scr_xs, scr_ys;

	y0=vec0[TKRA_VX_YPOS]>>16;
	x0=vec0[TKRA_VX_XPOS]>>16;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

	if(	(x0>=clip_x1) ||
		(x0<=clip_x0) ||
		(y0>=clip_y1) ||
		(y0<=clip_y0) )
			return;

	DrawSpan	= ctx->DrawSpanZt;
	DrawSpanZ	= ctx->DrawSpanZb;
	
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;

	dsparm[TKRA_DS_CTX]=(u64)(ctx);
	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);
	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);
	dsparm[TKRA_DS_BLEND]=(u64)(ctx->Blend);
	dsparm[TKRA_DS_ZATEST]=(u64)(ctx->ZaTest);
	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;
	
	z0 = (int)(vec0[TKRA_VX_ZPOS]);
	dsparm[TKRA_DS_TPOS		]=vec0[TKRA_VX_TPOS];
	dsparm[TKRA_DS_TSTEP	]=0;
	dsparm[TKRA_DS_CPOS		]=vec0[TKRA_VX_CPOS];
	dsparm[TKRA_DS_CSTEP	]=0;
//	dsparm[TKRA_DS_ZPOS		]=vec0[TKRA_VX_ZPOS];
	dsparm[TKRA_DS_ZPOS		]=z0;
	dsparm[TKRA_DS_ZSTEP	]=0;

	cb=scr_cb+(y0*scr_xs+x0);
	zb=scr_zb+(y0*scr_xs+x0);
	DrawSpanZ(zb, 1, z0, 0);
	DrawSpan(dsparm, cb, zb, 1);
}
#endif

#if 1
void TKRA_WalkLine(TKRA_Context *ctx, u64 *vec0, u64 *vec1)
{
	void (*DrawSpan)(u64 *parm,
		tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
	void (*DrawSpanZ)(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);

	u64	dsparm[TKRA_DS_NPARM];
	tkra_rastpixel *scr_cb, *cb;
	tkra_zbufpixel *scr_zb, *zb;
	int x0, y0, z0, x1, y1, z1;
	int xc, yc, zc, xs, ys, zs, xd, yd, zd, xda, yda, xyd, xydrcp;
	int i, j, k;

	int clip_x0, clip_x1, clip_y0, clip_y1;

	int		xshl, yshl;
	int		scr_xs, scr_ys;

	x0=vec0[TKRA_VX_XPOS];
	y0=vec0[TKRA_VX_YPOS];
	z0 = (int)(vec0[TKRA_VX_ZPOS]);

	y1=vec1[TKRA_VX_YPOS];
	x1=vec1[TKRA_VX_XPOS];
	z1 = (int)(vec1[TKRA_VX_ZPOS]);
	
	xd=x1-x0;
	yd=y1-y0;
	zd=z1-z0;
//	xyd=(xd^(xd>>31))+(yd^(yd>>31));
	xda=xd^(xd>>31);
	yda=yd^(yd>>31);
	xyd=xda;
	if(yda>xyd)
		xyd=yda;

	xyd=(xyd>>16)+1;

	xydrcp=TKRA_SpanRcp(xyd);
	
	xc=x0;
	yc=y0;
	zc=z0;
	xs=xd/xyd;
	ys=yd/xyd;
	zs=zd/xyd;

//	xs=(xd*xydrcp)>>16;
//	ys=(yd*xydrcp)>>16;
//	zs=(zd*xydrcp)>>16;

	clip_x0	= ctx->clip_x0;
	clip_x1	= ctx->clip_x1;
	clip_y0	= ctx->clip_y0;
	clip_y1	= ctx->clip_y1;

//	if(	(x0>=clip_x1) ||
//		(x0<=clip_x0) ||
//		(y0>=clip_y1) ||
//		(y0<=clip_y0) )
//			return;

	DrawSpan	= ctx->DrawSpanZt;
	DrawSpanZ	= ctx->DrawSpanZb;
	
	scr_cb = ctx->screen_rgb;
	scr_zb = ctx->screen_zbuf;
	scr_xs = ctx->screen_xsize;
	scr_ys = ctx->screen_ysize;

	xshl=ctx->tex_xshl;
	yshl=ctx->tex_yshl;

	dsparm[TKRA_DS_CTX]=(u64)(ctx);
	dsparm[TKRA_DS_TEXIMG]=(u64)(ctx->tex_img);
	dsparm[TKRA_DS_TEXBCN]=(u64)(ctx->tex_img_bcn);
	dsparm[TKRA_DS_BLEND]=(u64)(ctx->Blend);
	dsparm[TKRA_DS_ZATEST]=(u64)(ctx->ZaTest);
	dsparm[TKRA_DS_XMASK]=(1<<xshl)-1;
	dsparm[TKRA_DS_YMASK]=(1<<(yshl+xshl))-1;
	
	dsparm[TKRA_DS_TPOS		]=vec0[TKRA_VX_TPOS];
	dsparm[TKRA_DS_TSTEP	]=0;
	dsparm[TKRA_DS_CPOS		]=vec0[TKRA_VX_CPOS];
	dsparm[TKRA_DS_CSTEP	]=0;
	dsparm[TKRA_DS_ZPOS		]=z0;
	dsparm[TKRA_DS_ZSTEP	]=0;

	for(i=0; i<xyd; i++)
	{
		x0=xc>>16;
		y0=yc>>16;

		if(	(x0>=clip_x1) ||
			(x0<=clip_x0) ||
			(y0>=clip_y1) ||
			(y0<=clip_y0) )
				continue;

		dsparm[TKRA_DS_ZPOS		]=zc;

		cb=scr_cb+(y0*scr_xs+x0);
		zb=scr_zb+(y0*scr_xs+x0);
		DrawSpanZ(zb, 1, z0, 0);
		DrawSpan(dsparm, cb, zb, 1);
		
		xc+=xs;
		yc+=ys;
		zc+=zs;
	}

	return(0);
}
#endif

int TKRA_SetupDrawEdgeForState(TKRA_Context *ctx)
{
#if 0
	ctx->DrawSpan=TKRA_DrawSpan_ModTexMort;
	ctx->DrawSpan_Min=TKRA_DrawSpan_ModTexMort;
	ctx->DrawSpan_Mag=TKRA_DrawSpan_ModTexMort;
//	ctx->DrawSpan=TKRA_DrawSpan_ModTexMortZbuf;

	ctx->DrawSpan=TKRA_DrawSpan_AlphaModTexMort;
	ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModTexMort;
	ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModTexMort;
//	ctx->DrawSpan=TKRA_DrawSpan_AlphaModTexMortZbuf;

//	ctx->DrawSpan=TKRA_DrawSpan_AtestModTexMort;
//	ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModTexMort;
//	ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModTexMort;

//	ctx->DrawSpan=TKRA_DrawSpan_DirTexMort;
//	ctx->DrawSpan_Min=TKRA_DrawSpan_DirTexMort;
//	ctx->DrawSpan_Mag=TKRA_DrawSpan_DirTexMort;

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

	ctx->DrawSpanZt=TKRA_DrawSpan_AlphaModTexMortZt;
	ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModTexMortZt;
	ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModTexMortZt;
#endif

//	if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
	if((ctx->stateflag1&TKRA_STFL1_DEPTHTEST) ||
		(ctx->bfn_flag&TKRA_TRFL_DOBLEND))
//	if(0)
	{
//		ctx->RasterWalkEdges=TKRA_WalkEdges_HZbuf;
		ctx->RasterWalkEdges=TKRA_WalkEdges_Zbuf;
		ctx->RasterWalkEdgesNcp=TKRA_WalkEdges_ZbufNcp;

#ifdef __BJX2__
//		if(ctx->DrawSpanZb == TKRA_DrawSpan_ZbNul)
//			ctx->RasterWalkEdges=TKRA_WalkEdges_ZbufNZb;
#endif

#ifdef __BJX2__
		TKRA_Probe_WalkEdgesMMIO(ctx);
		if(!(tkra_nommio&1))
		{
			ctx->RasterWalkEdges=TKRA_WalkEdges_MMIO;
			ctx->RasterWalkEdgesNcp=TKRA_WalkEdges_MMIO;
		}
#endif

//		ctx->RasterWalkEdgesNcp=ctx->RasterWalkEdges;

	}else
	{
		ctx->RasterWalkEdges=TKRA_WalkEdges_Dfl;
		ctx->RasterWalkEdgesNcp=ctx->RasterWalkEdges;

#ifdef __BJX2__
		TKRA_Probe_WalkEdgesMMIO(ctx);
		if(!(tkra_nommio&1))
			ctx->RasterWalkEdgesNcp=TKRA_WalkEdges_MMIO;
#endif
	}

//	ctx->RasterWalkEdges=TKRA_WalkEdges_Dfl;
//	ctx->RasterWalkEdges=TKRA_WalkEdges_HZbuf;
	return(0);
}

int TKRA_SetupDrawEdgeForTriFlag(TKRA_Context *ctx, int trifl)
{
	trifl|=ctx->tex_flag;
	trifl|=ctx->bfn_flag;

	if(trifl&TKRA_TRFL_NOALPHA)
		trifl&=~TKRA_TRFL_ALPHA;

	if(trifl&TKRA_TRFL_NOCMOD)
		trifl&=~TKRA_TRFL_ALPHA;
		
	if(		(trifl==ctx->span_trifl) &&
			(ctx->tex_cur==ctx->span_tex_cur))
		return(0);
	ctx->span_trifl=trifl;
	ctx->span_tex_cur=ctx->tex_cur;

	if((trifl&TKRA_TRFL_MINBL) && (trifl&TKRA_TRFL_MAGBL))
		trifl&=~TKRA_TRFL_MINBL;


	ctx->triflag=trifl;

	if(trifl&TKRA_TRFL_DOZABLEND)
	{
		ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

		if(trifl&TKRA_TRFL_MINBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModBlTexMortClampZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModBlTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModTexMortClampZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModTexMortZt;
			}
		}

		if(trifl&TKRA_TRFL_MAGBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModBlTexMortClampZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModBlTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModTexMortClampZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModTexMortZt;
			}
		}

		if(ctx->tex_cur->tex_img_bcn)
		{
			if(trifl&TKRA_TRFL_MINBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModBlUtx2MortClampZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModBlUtx2MortZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModUtx2MortClampZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ZatModUtx2MortZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_ZatModUtx2MortZt;
				}
			}

			if(trifl&TKRA_TRFL_MAGBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModBlUtx2MortClampZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModBlUtx2MortZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModUtx2MortClampZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ZatModUtx2MortZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ZatModUtx2MortZt;
				}
			}
		}
	}else

		if(trifl&TKRA_TRFL_DOBLEND)
//	if(	(trifl&TKRA_TRFL_DOBLEND) ||
//		(trifl&TKRA_TRFL_CLAMPS) ||
//		(trifl&TKRA_TRFL_CLAMPT))
	{
#if 0
		ctx->DrawSpan=TKRA_DrawSpan_BlendModTexMort;
		ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModTexMort;
		ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModTexMort;
//		ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlTexMort;

		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;

		ctx->DrawSpanZt=TKRA_DrawSpan_BlendModTexMortZt;
		ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModTexMortZt;
		ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModTexMortZt;
//		ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlTexMortZt;
#endif

		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;
//		ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

		if(trifl&TKRA_TRFL_NOZWRITE)
			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;


		if(trifl&TKRA_TRFL_MINBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModBlTexMortClamp;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModBlTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModTexMortClamp;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModTexMortZt;
			}
		}

		if(trifl&TKRA_TRFL_MAGBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlTexMortClamp;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModTexMortClamp;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModTexMortZt;
			}
		}

#if 0
		if(!(ctx->stateflag1&TKRA_STFL1_TEXTURE2D))
		{
			ctx->DrawSpan=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Min=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_DirClr;

			ctx->DrawSpanZt=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_DirClrZt;
		}else
#endif
			if(ctx->tex_cur->tex_img_bcn)
		{
#if 0
			ctx->DrawSpan=TKRA_DrawSpan_BlendModUtx2Mort;
			ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModUtx2Mort;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModUtx2Mort;
//			ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlUtx2Mort;

			ctx->DrawSpanZt=TKRA_DrawSpan_BlendModUtx2MortZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModUtx2MortZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModUtx2MortZt;
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlUtx2MortZt;
#endif

			if(trifl&TKRA_TRFL_MINBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModBlUtx2MortClamp;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModBlUtx2Mort;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModUtx2MortClamp;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_BlendModUtx2Mort;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_BlendModUtx2MortZt;
				}
			}

			if(trifl&TKRA_TRFL_MAGBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlUtx2MortClamp;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModBlUtx2Mort;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModUtx2MortClamp;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_BlendModUtx2Mort;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_BlendModUtx2MortZt;
				}
			}
		}
	}else

#if 1
	if(trifl&TKRA_TRFL_DOLMAP)
	{
		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;
		if(trifl&TKRA_TRFL_NOZWRITE)
			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

		if(trifl&TKRA_TRFL_MINBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModBlTexMortClampZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModBlTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModTexMortClampZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModTexMortZt;
			}
		}

		if(trifl&TKRA_TRFL_MAGBL)
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModBlTexMortClampZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModBlTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModBlTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModBlTexMortZt;
			}
		}else
		{
			if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModTexMortClampZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModTexMortClampZt;
			}else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModTexMortZt;
			}
		}

#if 0
		if(!(ctx->stateflag1&TKRA_STFL1_TEXTURE2D))
		{
			ctx->DrawSpan=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Min=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_DirClr;

			ctx->DrawSpanZt=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_DirClrZt;
		}else
#endif

		if(ctx->tex_cur->tex_img_bcn)
		{
			if(trifl&TKRA_TRFL_MINBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModBlUtx2MortClampZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModBlUtx2MortZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModUtx2MortClampZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_LmapModUtx2MortZt;
					ctx->DrawSpanZt_Min=TKRA_DrawSpan_LmapModUtx2MortZt;
				}
			}

			if(trifl&TKRA_TRFL_MAGBL)
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModBlUtx2MortClampZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModBlUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModBlUtx2MortZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModBlUtx2MortZt;
				}
			}else
			{
				if((trifl&TKRA_TRFL_CLAMPS) || (trifl&TKRA_TRFL_CLAMPT))
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModUtx2MortClampZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModUtx2MortClampZt;
				}else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_LmapModUtx2MortZt;
					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_LmapModUtx2MortZt;
				}
			}
		}
	}else
#endif

		if(trifl&TKRA_TRFL_ALPHA)
	{
//		ctx->DrawSpan=TKRA_DrawSpan_DirClr;
//		ctx->DrawSpan_Min=TKRA_DrawSpan_DirClr;
//		ctx->DrawSpan_Mag=TKRA_DrawSpan_DirClr;

		ctx->DrawSpan=TKRA_DrawSpan_AlphaModTexMort;
		ctx->DrawSpanZt=TKRA_DrawSpan_AlphaModTexMortZt;

#if 0
		ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModTexMort;
//		ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModBlTexMort;
		ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModTexMort;
//		ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModBlTexMort;

		if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
		{
			ctx->DrawSpan=TKRA_DrawSpan_AtestModTexMort;
			ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModTexMort;
//			ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlTexMort;
//			ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlTexMort;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModTexMort;
		}
#endif

		if(trifl&TKRA_TRFL_MINBL)
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlTexMortZt;
//				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModBlTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlTexMortZt;
			}
		}else
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModTexMortZt;
//				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModTexMortZt;
			}
		}

		if(trifl&TKRA_TRFL_MAGBL)
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlTexMort;
//				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModBlTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortZt;
			}
		}else
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModTexMort;
//				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModTexMortZt;
			}
		}

//		ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModBlTexMortHZt;
//		ctx->DrawSpanHZt=TKRA_DrawSpan_AlphaModTexMortHZt;
//		ctx->DrawSpanHZt_Min=TKRA_DrawSpan_AlphaModTexMortHZt;
//		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortHZt;
//		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_AlphaModTexMortHZt;

		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;
//		ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

		if(trifl&TKRA_TRFL_NOZWRITE)
			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

//		ctx->DrawSpanZt=TKRA_DrawSpan_AlphaModTexMortZt;
//		ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModTexMortZt;
//		ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModTexMortZt;
//		ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortZt;

//		if(trifl&TKRA_TRFL_MINBL)
//		{
//			ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlTexMortZt;
//		}

//		if(trifl&TKRA_TRFL_MAGBL)
//		{
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortZt;
//		}

		if(ctx->tex_cur->tex_img_bcn)
		{
#if 0
			ctx->DrawSpan=TKRA_DrawSpan_AlphaModUtx2Mort;
			ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModUtx2Mort;
//			ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModBlUtx2Mort;
//			ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModUtx2Mort;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModBlUtx2Mort;

			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan=TKRA_DrawSpan_AtestModUtx2Mort;
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModUtx2Mort;
//				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlUtx2Mort;
//				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModUtx2Mort;
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlUtx2Mort;
			}

			ctx->DrawSpanZt=TKRA_DrawSpan_AlphaModUtx2MortZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZt;
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZt;

// #ifdef __BJX2__
#if 1
			ctx->DrawSpanZt=TKRA_DrawSpan_AlphaModUtx2MortZb;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZb;
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZb;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZb;

			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;
#endif
#endif

			if(trifl&TKRA_TRFL_MINBL)
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlUtx2MortZb;
//					ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
				}
			}else
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModUtx2MortZb;
//					ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZb;
				}
			}

			if(trifl&TKRA_TRFL_MAGBL)
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlUtx2MortZb;
//					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
				}
			}else
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModUtx2MortZb;
//					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZb;
				}
			}

		}

	}else
	{
#if 0
		ctx->DrawSpan=TKRA_DrawSpan_ModTexMort;
//		ctx->DrawSpan_Min=TKRA_DrawSpan_ModTexMort;
		ctx->DrawSpan_Min=TKRA_DrawSpan_ModBlTexMort;
//		ctx->DrawSpan_Mag=TKRA_DrawSpan_ModTexMort;
		ctx->DrawSpan_Mag=TKRA_DrawSpan_ModBlTexMort;

		if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
		{
			ctx->DrawSpan=TKRA_DrawSpan_AtestModTexMort;
			ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModTexMort;
//			ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlTexMort;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlTexMort;
		}

		ctx->DrawSpanHZt=TKRA_DrawSpan_ModBlTexMortHZt;
//		ctx->DrawSpanHZt=TKRA_DrawSpan_ModTexMortHZt;
		ctx->DrawSpanHZt_Min=TKRA_DrawSpan_ModTexMortHZt;
//		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_ModBlTexMortHZt;
		ctx->DrawSpanHZt_Mag=TKRA_DrawSpan_ModTexMortHZt;

		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;

		ctx->DrawSpanZt=TKRA_DrawSpan_ModTexMortZt;
		ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModTexMortZt;
		ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModTexMortZt;
#endif

		ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;
		if(trifl&TKRA_TRFL_NOZWRITE)
			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

		if(trifl&TKRA_TRFL_MINBL)
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlTexMort;
//				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ModBlTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModBlTexMortZt;
			}
		}else
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModTexMort;
//				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModTexMortZt;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Min=TKRA_DrawSpan_ModTexMort;
				ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModTexMortZt;
			}
		}

		if(trifl&TKRA_TRFL_MAGBL)
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlTexMort;
//				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ModBlTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModBlTexMortZt;
			}
		}else
		{
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModTexMort;
//				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModTexMortZt;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModTexMortZt;
			}
			else
			{
				ctx->DrawSpan_Mag=TKRA_DrawSpan_ModTexMort;
				ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModTexMortZt;
			}
		}


		if(	!(ctx->stateflag1&TKRA_STFL1_TEXTURE2D) &&
			!(ctx->stateflag1&TKRA_STFL1_ALPHATEST))
		{
			ctx->DrawSpan=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Min=TKRA_DrawSpan_DirClr;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_DirClr;

			ctx->DrawSpanZt=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_DirClrZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_DirClrZt;
		}else
			if(ctx->tex_cur->tex_img_bcn)
		{
#if 0
//			ctx->DrawSpan=TKRA_DrawSpan_AlphaModUtx2Mort;
//			ctx->DrawSpan_Min=TKRA_DrawSpan_AlphaModUtx2Mort;
//			ctx->DrawSpan_Mag=TKRA_DrawSpan_AlphaModUtx2Mort;

			ctx->DrawSpan=TKRA_DrawSpan_ModUtx2Mort;
			ctx->DrawSpan_Min=TKRA_DrawSpan_ModUtx2Mort;
//			ctx->DrawSpan_Mag=TKRA_DrawSpan_ModUtx2Mort;
			ctx->DrawSpan_Mag=TKRA_DrawSpan_ModBlUtx2Mort;

			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
			{
				ctx->DrawSpan=TKRA_DrawSpan_AtestModUtx2Mort;
				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModUtx2Mort;
//				ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlUtx2Mort;
//				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModUtx2Mort;
				ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlUtx2Mort;
			}

			ctx->DrawSpanZt=TKRA_DrawSpan_ModUtx2MortZt;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModUtx2MortZt;
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModUtx2MortZt;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModBlUtx2MortZt;

// #ifdef __BJX2__
#if 1
			ctx->DrawSpanZt=TKRA_DrawSpan_ModUtx2MortZb;
			ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModUtx2MortZb;
//			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModUtx2MortZb;
			ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModBlUtx2MortZb;

			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;
#endif

#endif

			ctx->DrawSpan=TKRA_DrawSpan_ModUtx2Mort;
			ctx->DrawSpanZt=TKRA_DrawSpan_ModUtx2MortZb;
			ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;

			if(trifl&TKRA_TRFL_MINBL)
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModBlUtx2MortZb;
//					ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModBlUtx2MortZb;
				}
			}else
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_AtestModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_AtestModUtx2MortZb;
//					ctx->DrawSpanZt_Min=TKRA_DrawSpan_AlphaModUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Min=TKRA_DrawSpan_ModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModUtx2MortZt;
					else
						ctx->DrawSpanZt_Min=TKRA_DrawSpan_ModUtx2MortZb;
				}
			}

			if(trifl&TKRA_TRFL_MAGBL)
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModBlUtx2Mort;
//					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModBlUtx2MortZb;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModBlUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ModBlUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModBlUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModBlUtx2MortZb;
				}
			}else
			{
				if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_AtestModUtx2Mort;
//					ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AlphaModUtx2MortZb;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_AtestModUtx2MortZb;
				}
				else
				{
					ctx->DrawSpan_Mag=TKRA_DrawSpan_ModUtx2Mort;
					if(trifl&TKRA_TRFL_NOZWRITE)
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModUtx2MortZt;
					else
						ctx->DrawSpanZt_Mag=TKRA_DrawSpan_ModUtx2MortZb;
				}
			}
		}
	}

#if 0
// #ifdef __BJX2__
	if(ctx->DrawSpanZt == TKRA_DrawSpan_ModUtx2MortZt)
	{
		ctx->DrawSpanZt	= TKRA_DrawSpan_ModUtx2MortZb;
		ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;
	}
	
	if(ctx->DrawSpanZt == TKRA_DrawSpan_AlphaModUtx2MortZt)
	{
		ctx->DrawSpanZt	= TKRA_DrawSpan_AlphaModUtx2MortZb;
		ctx->DrawSpanZb = TKRA_DrawSpan_ZbNul;
	}
#endif

	return(0);
}

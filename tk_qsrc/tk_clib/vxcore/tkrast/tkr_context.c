void TKRA_DumpMatrix(tkra_mat4 matc, char *str);
void TKRA_DumpVec4(tkra_vec4f vec, char *str);

int TKRA_DrawPrimitiveIndexArrayObjI(
	TKRA_Context *ctx,
	TKRA_DrawPrimArrays	*parr,
	int mode,		int count);

void TKRA_UpdateTexImgI(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	tkra_rastpixel *buf, int xs, int ys, int mip, int flag);
void TKRA_UpdateTexImgUtx2I(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	u64 *buf, int xs, int ys, int mip, int flag);

TKRA_TexImage *TKRA_LookupTexImgI(TKRA_Context *ctx, int num);
TKRA_TexImage *TKRA_GetTexImgI(TKRA_Context *ctx, int num);
int TKRA_BindTexImgI(TKRA_Context *ctx, TKRA_TexImage *img);

int TKRA_QueryContextI(TKRA_Context *ctx, u64 param, void *iptr, void *optr);
int TKRA_ModifyContextI(TKRA_Context *ctx, u64 param, void *iptr, void *optr);
int TKRA_SetupScreenI(TKRA_Context *ctx, int xs, int ys, u64 attrib);
void TKRA_ClearI(TKRA_Context *ctx, unsigned int mask);

TKRA_ContextVt tkra_context_vt = {
NULL,			//Reserved 0
NULL,			//Reserved 1
NULL,			//Reserved 2
NULL,			//Reserved 3
TKRA_DrawPrimitiveIndexArrayObjI,
TKRA_UpdateTexImgI,
TKRA_UpdateTexImgUtx2I,
TKRA_LookupTexImgI,
TKRA_GetTexImgI,
TKRA_BindTexImgI,
TKRA_QueryContextI,
TKRA_ModifyContextI,
TKRA_SetupScreenI
};

TKRA_Context *TKRA_AllocContext()
{
	TKRA_Context *tmp;
	TKRA_SvContext	*svt;
	
	tmp=tkra_malloc_gbl(sizeof(TKRA_Context));
	svt=tkra_malloc_gbl(sizeof(TKRA_SvContext));
	memset(tmp, 0, sizeof(TKRA_Context));
	memset(svt, 0, sizeof(TKRA_SvContext));
	tmp->vt=&tkra_context_vt;
	tmp->vptr=&(tmp->t_vptr);

	tmp->svctx=svt;
	svt->clctx=tmp;
	
	tmp->magic1=0x12345678;
	tmp->magic2=0x12345678;
	tmp->size_context=sizeof(TKRA_Context);

	return(tmp);
}

void *TKRA_GetHalContextComGlue(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	TKRA_Context *tmp;
	
	if(((u32)apiname)!=TK_FCC_TKRA)
		return(NULL);
	
	tmp=TKRA_AllocContext();
	tmp->vt=TKGDI_GetWrapVTableForTask(task,
		&tkra_context_vt, sizeof(tkra_context_vt));
	return(tmp);
}

int TKRA_RegisterHalGetContext()
{
	TK_RegisterGetApiContextFn(TK_FCC_TKRA, TKRA_GetHalContextComGlue);
	return(0);
}

int TKRA_SyncScreenCacheMode(TKRA_Context *ctx, int md)
{
	u64 *ptr;

	u64 i0, i1, i2, i3, ik;
	int i, j, k;

	if(md==ctx->cachemode)
		return(0);

	if((md&0xE)==(ctx->cachemode&0xE))
	{
		if(md&1)
		{
			ctx->cachemode=md;
			return(1);
		}
		return(0);
	}

	if(!(ctx->cachemode&1))
	{
		ctx->cachemode=md;
		return(1);
	}
	
	ptr=(u64 *)(ctx->screen_sten);
	ik=0; j=0;
	for(i=0; i<512; i++)
	{
		i0=ptr[j+0];
		i1=ptr[j+2];
		i2=ptr[j+4];
		i3=ptr[j+6];
		i0=i0+i1;
		i2=i2+i3;
		i1=i0+i2;
		j+=8;
		ik=ik+i1;
//		ik+=i0+i1+i2+i3;
	}
	
	ctx->cachemode=md;
	return(1);
}

int TKRA_QueryContextI(TKRA_Context *ctx, u64 param, void *iptr, void *optr)
{
}

int TKRA_ModifyContextI(TKRA_Context *ctx, u64 param, void *iptr, void *optr)
{
	if(param==TKRA_CTXPARM_FINISH)
	{
		TKRA_DebugPrintStats(ctx);
	}

	if((param&TKRA_CTXPARM_MASK)==TKRA_CTXPARM_CLEAR)
	{
		TKRA_ClearI(ctx, param>>32);
	}
}

int TKRA_QueryContext(TKRA_Context *ctx, u64 param, void *iptr, void *optr)
{
	return(ctx->vt->QueryContext(ctx, param, iptr, optr));
}

int TKRA_ModifyContext(TKRA_Context *ctx, u64 param, void *iptr, void *optr)
{
	return(ctx->vt->ModifyContext(ctx, param, iptr, optr));
}

int TKRA_SetupScreenI(TKRA_Context *ctx, int xs, int ys, u64 attrib)
{
	byte *ptr;
	int scr_memsz;
	int ofs_rgb, ofs_zbuf, ofs_zbuf2, ofs_sten;
	int ofs_pad, ofs_pad_lo, frsz;
	int i, j, k;
	
	if(!(((long)ctx)>>16))
	{
		__debugbreak();
	}
	
	frsz=xs*ys;

	if(ctx->pixelfmt&TKRA_PIXFMT_DW)
		frsz*=2;

//	ofs_pad=4096-(frsz&8191);
//	if(ofs_pad<0)
//		ofs_pad+=8192;

//	ofs_pad=2048-(frsz&4095);
	ofs_pad=2720-(frsz&4095);
	if(ofs_pad<0)
		ofs_pad+=4096;

	ofs_pad=(ofs_pad+31)&(~31);
	ofs_pad_lo=ofs_pad>>1;

	scr_memsz = (xs+2)*16*2;
	scr_memsz = (scr_memsz+63)&(~63);

//	ofs_rgb = scr_memsz;
	ofs_rgb = scr_memsz + ofs_pad_lo;
//	scr_memsz += (xs+2)*(ys+4)*sizeof(tkra_rastpixel);
	scr_memsz += (frsz+ofs_pad)*sizeof(tkra_rastpixel);
	scr_memsz = (scr_memsz+63)&(~63);

//	ofs_zbuf = scr_memsz;
	ofs_zbuf = scr_memsz + ofs_pad_lo;
//	scr_memsz += (xs+2)*(ys+4)*sizeof(tkra_zbufpixel);
	scr_memsz += (frsz+ofs_pad)*sizeof(tkra_zbufpixel);
	scr_memsz = (scr_memsz+63)&(~63);

//	ofs_zbuf2 = scr_memsz;
	ofs_zbuf2 = scr_memsz + ofs_pad_lo;
//	scr_memsz += (xs+2)*(ys+4)*sizeof(tkra_zbufpixel);
	scr_memsz += (frsz+ofs_pad)*sizeof(tkra_zbufpixel);
	scr_memsz = (scr_memsz+63)&(~63);

//	ofs_sten = scr_memsz;
	ofs_sten = scr_memsz + ofs_pad_lo;
//	scr_memsz += (xs+2)*(ys+4)*sizeof(byte);
	scr_memsz += (frsz+ofs_pad)*sizeof(byte);

//	ptr=tkra_malloc(scr_memsz+31);
//	ptr=tkra_malloc_gbl(scr_memsz+31);
	ptr=tkra_malloc_phys(scr_memsz+31);
	ctx->screen_mem=ptr;
	
	k=(nlint)ptr;
	if(k&15)
		ptr+=(16-(k&15));
	
	ctx->screen_rgb=(void *)(ptr+ofs_rgb);
	ctx->screen_zbuf=(void *)(ptr+ofs_zbuf);
	ctx->screen_zbuf2=(void *)(ptr+ofs_zbuf2);
	ctx->screen_sten=(void *)(ptr+ofs_sten);

	if(ctx->pixelfmt&TKRA_PIXFMT_DW)
	{
		ctx->screenb_rgb=(void *)(ptr+ofs_rgb);
		ctx->screenb_zbuf=(void *)(ptr+ofs_zbuf);
		ctx->screenb_zbuf2=(void *)(ptr+ofs_zbuf2);
		ctx->screenb_sten=(void *)(ptr+ofs_sten);
	}

//	ctx->screen_rgb=tkra_malloc((xs+4)*(ys+16)*sizeof(tkra_rastpixel));
//	ctx->screen_zbuf=tkra_malloc((xs+4)*(ys+16)*sizeof(tkra_zbufpixel));
//	ctx->screen_zbuf=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(tkra_zbufpixel));
//	ctx->screen_sten=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(byte));
//	ctx->screen_sten=tkra_malloc((xs+2)*(ys+2)*sizeof(byte));

	ctx->screen_xsize=xs;
	ctx->screen_ysize=ys;

	ctx->clip_x0=0;
	ctx->clip_x1=xs;
	ctx->clip_y0=0;
	ctx->clip_y1=ys;

	ctx->scr_xscale=xs/2;
	ctx->scr_yscale=ys/2;
	ctx->scr_xcenter=xs/2;
	ctx->scr_ycenter=ys/2;

	ctx->trans_znear=0;
	ctx->trans_zfar=1;
	ctx->trans_zrange=1;

	TKRA_InitSpanRcp();
	TKRA_FillEParm_InitRcpTab();

	for(i=0; i<xs*ys; i++)
	{
//		((short *)(ctx->screen_rgb))[i]=0x7FFF;
		((short *)(ctx->screen_rgb))[i]=0x3CEF;
		((short *)(ctx->screen_zbuf))[i]=0x7FFF;
	}

	return(0);
}

int TKRA_SetupScreen(TKRA_Context *ctx, int xs, int ys)
{
	return(ctx->vt->SetupScreen(ctx, xs, ys, 0));
}

int TKRA_DebugPrintStats(TKRA_Context *ctx)
{
#if 0
	if(ctx->stat_base_tris)
	{
//		TKRA_DumpVec4(ctx->prj_xyzsc, "Clip: XyzSc:");
//		TKRA_DumpVec4(ctx->prj_xyzbi, "Clip: XyzBi:");

		tk_dbg_printf("clip: L=%d R=%d T=%d B=%d\n",
			(int)(ctx->scr_clip_l), (int)(ctx->scr_clip_r),
			(int)(ctx->scr_clip_t), (int)(ctx->scr_clip_b));

	//	tkra_prj_xyzsc=prj_xyzsc;
	//	tkra_prj_xyzbi=prj_xyzbi;

		tk_dbg_printf("Stat: Base=%d Frag=%d Draw=%d Reject=%d Blown=%d\n",
			ctx->stat_base_tris,
			ctx->stat_frag_tris,
			ctx->stat_draw_tris,
			ctx->stat_reject_tris,
			ctx->stat_blown_tris);

		tk_dbg_printf("Stat: Rejects: Frustum=%d MicroBase=%d "
				"MicroFrag=%d Backface=%d NegW=%d Zocc=%d\n",
			ctx->stat_frustum_tris,
			ctx->stat_microbase_tris,
			ctx->stat_microfrag_tris,
			ctx->stat_backface_tris,
			ctx->stat_negw_tris,
			ctx->stat_zocc_tris);
	}
#endif

	ctx->stat_base_tris=0;
	ctx->stat_frag_tris=0;
	ctx->stat_draw_tris=0;
	ctx->stat_reject_tris=0;
	ctx->stat_blown_tris=0;
	ctx->stat_drawpts1_tris=0;
	ctx->stat_drawpts3_tris=0;

	ctx->stat_frustum_tris=0;
//	ctx->stat_micro_tris=0;
	ctx->stat_microbase_tris=0;
	ctx->stat_microfrag_tris=0;
	ctx->stat_backface_tris=0;
	ctx->stat_negw_tris=0;
	ctx->stat_zocc_tris=0;
}

int TKRA_RecalcViewport(TKRA_Context *ctx)
{
	float	scr_xsc, scr_ysc, scr_zsc;
	float	scr_xc, scr_yc, scr_zc;
	float	zfr;
	

	scr_xsc=ctx->scr_xscale;
	scr_ysc=ctx->scr_yscale;
	scr_xc=ctx->scr_xcenter;
	scr_yc=ctx->scr_ycenter;
	
//	zfr = 24575;
	zfr = 28671;

	scr_zsc=ctx->trans_zrange;
	scr_zc=ctx->trans_znear;
//	ctx->prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 32767, 1);
//	ctx->prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 24575, 1);
//	ctx->prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 16383, 1);
//	ctx->prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 8191, 1);
	ctx->prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, zfr*scr_zsc, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 16383, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 8192, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, -8192, 1);
//	ctx->prj_xyzbi=tkra_mkvec4f(scr_xc, scr_yc, 0, 0);
	ctx->prj_xyzbi=tkra_mkvec4f(scr_xc, scr_yc, zfr*scr_zc, 0);

	ctx->scr_clip_l=scr_xc-scr_xsc;
	ctx->scr_clip_r=scr_xc+scr_xsc;
	ctx->scr_clip_t=scr_yc+scr_ysc;
	ctx->scr_clip_b=scr_yc-scr_ysc;
}

int TKRA_SetViewport(TKRA_Context *ctx, int x, int y, int xs, int ys)
{
	float			scr_xsc, scr_ysc;
	float			scr_xc, scr_yc;

	ctx->clip_x0=x;
	ctx->clip_x1=x+xs;
	ctx->clip_y0=y;
	ctx->clip_y1=y+ys;

	if(ctx->clip_x0<0)
		ctx->clip_x0=0;
	if(ctx->clip_x1<0)
		ctx->clip_x1=0;

	if(ctx->clip_y0<0)
		ctx->clip_y0=0;
	if(ctx->clip_y1<0)
		ctx->clip_y1=0;

	if(ctx->clip_x0>ctx->screen_xsize)
		ctx->clip_x0=ctx->screen_xsize;
	if(ctx->clip_x1>ctx->screen_xsize)
		ctx->clip_x1=ctx->screen_xsize;

	if(ctx->clip_y0>ctx->screen_ysize)
		ctx->clip_y0=ctx->screen_ysize;
	if(ctx->clip_y1>ctx->screen_ysize)
		ctx->clip_y1=ctx->screen_ysize;

	ctx->scr_xscale=xs/2;
	ctx->scr_yscale=ys/2;
	ctx->scr_xcenter=x+(xs/2);
	ctx->scr_ycenter=y+(ys/2);

	TKRA_RecalcViewport(ctx);

	return(0);
}

#ifdef __BJX2__
void tkra_memset_word(void *buf, u16 val, int cnt);
void tkra_memset_dword(void *buf, u32 val, int cnt);

__asm {
tkra_memset_word:
	PSHUF.W	R5, 0x00, R18
	MOV		R18, R19
	
	LEA.W	(R4, R6), R7

	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BF		.L1
	.L0:
	MOV.X	R18, (R4, 0)
	MOV.X	R18, (R4, 16)
	MOV.X	R18, (R4, 32)
	MOV.X	R18, (R4, 48)
	MOV		R2, R4
	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BT		.L0
	.L1:

	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BF		.L3
	.L2:
	MOV.Q	R18, (R4)
	MOV		R2, R4
	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BT		.L2
	.L3:

	CMPQGT	R4, R7
	BF		.L5
	.L4:
	MOV.W	R18, (R4)
	ADD		2, R4
	CMPQGT	R4, R7
	BT		.L4
	.L5:

	RTS

tkra_memset_dword:
	MOVLLD	R5, R5, R18
	MOV		R18, R19
	
	LEA.L	(R4, R6), R7

	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BF		.L1
	.L0:
	MOV.X	R18, (R4, 0)
	MOV.X	R18, (R4, 16)
	MOV.X	R18, (R4, 32)
	MOV.X	R18, (R4, 48)
	MOV		R2, R4
	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BT		.L0
	.L1:

	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BF		.L3
	.L2:
	MOV.Q	R18, (R4)
	MOV		R2, R4
	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BT		.L2
	.L3:

	CMPQGT	R4, R7
	BF		.L5
	.L4:
	MOV.W	R18, (R4)
	ADD		2, R4
	CMPQGT	R4, R7
	BT		.L4
	.L5:

	RTS
};

#else
void tkra_memset_word(void *buf, u16 px, int cnt)
{
	u64 px4;
	u16 *ct, *cte;
	
	ct=buf; cte=ct+cnt;
	px4=px|(px<<16); px4=px4|(px4<<32);
	while((ct+32)<=cte)
	{
		((u64 *)ct)[0]=px4;	((u64 *)ct)[1]=px4;
		((u64 *)ct)[2]=px4;	((u64 *)ct)[3]=px4;
		((u64 *)ct)[4]=px4;	((u64 *)ct)[5]=px4;
		((u64 *)ct)[6]=px4;	((u64 *)ct)[7]=px4;
		ct+=32;
	}
	while((ct+8)<=cte)
	{
		((u64 *)ct)[0]=px4;
		((u64 *)ct)[1]=px4;
		ct+=8;
	}
	while(ct<cte)
		{ *ct++=px; }
}

void tkra_memset_dword(void *buf, u32 px, int cnt)
{
	u64 px4;
	u32 *ct, *cte;
	
	ct=buf; cte=ct+cnt;
	px4=px; px4=px4|(px4<<32);
	while((ct+16)<=cte)
	{
		((u64 *)ct)[0]=px4;	((u64 *)ct)[1]=px4;
		((u64 *)ct)[2]=px4;	((u64 *)ct)[3]=px4;
		((u64 *)ct)[4]=px4;	((u64 *)ct)[5]=px4;
		((u64 *)ct)[6]=px4;	((u64 *)ct)[7]=px4;
		ct+=16;
	}
	while((ct+4)<=cte)
	{
		((u64 *)ct)[0]=px4;
		((u64 *)ct)[1]=px4;
		ct+=4;
	}
	while(ct<cte)
		{ *ct++=px; }
}

#endif

void TKRA_ClearI(TKRA_Context *ctx, unsigned int mask)
{
	tkra_rastpixel *rgb, *cct, *ccte;
	tkra_zbufpixel *zbuf, *zct, *zcte;

	tkra_rast2pixel *c2rgb, *c2ct;
	tkra_zbuf2pixel *z2buf, *z2ct;
	byte *sten;
	u64 px4;
	u32 px;
	int xs, ys;
	int i, j, k, n;
	
//	ctx=TKRA_GetCurrentContext();

	xs=ctx->screen_xsize;
	ys=ctx->screen_ysize;
	rgb=ctx->screen_rgb;
	zbuf=ctx->screen_zbuf;
	sten=ctx->screen_sten;

	if(mask&TKRA_GL_DEPTH_BUFFER_BIT)
	{
		/* Swap Z Buffers */
		zbuf=ctx->screen_zbuf2;
		zct=ctx->screen_zbuf;
		ctx->screen_zbuf=zbuf;
		ctx->screen_zbuf2=zct;

		z2buf=ctx->screenb_zbuf2;
		z2ct=ctx->screenb_zbuf;
		ctx->screenb_zbuf=z2buf;
		ctx->screenb_zbuf2=z2ct;
	}


	if(mask&TKRA_GL_COLOR_BUFFER_BIT)
	{
		if(ctx->pixelfmt&TKRA_PIXFMT_DW)
		{
			px=ctx->clear_rgba32;
			n=xs*ys;
			tkra_memset_dword(rgb, px, n);
		}else
		{
			px=ctx->clear_rgb5;
			n=xs*ys;
			tkra_memset_word(rgb, px, n);
		}
	}

	if(mask&TKRA_GL_DEPTH_BUFFER_BIT)
	{
		if(ctx->pixelfmt&TKRA_PIXFMT_DW)
		{
			px=ctx->clear_zbuf32;
			n=xs*ys;
			tkra_memset_dword(z2buf, px, n);
		}else
		{
			px=ctx->clear_zbuf;
			n=xs*ys;
			tkra_memset_word(zbuf, px, n);
		}
	}

#if 0
	if(mask&TKRA_GL_STENCIL_BUFFER_BIT)
	{
		px=0;
		n=(xs>>1)*(ys>>1);
		for(i=0; i<n; i++)
			{ sten[i]=px; }
	}
#endif
}

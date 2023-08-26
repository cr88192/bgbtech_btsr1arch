void TKRA_DumpMatrix(tkra_mat4 matc, char *str);
void TKRA_DumpVec4(tkra_vec4f vec, char *str);

TKRA_Context *TKRA_AllocContext()
{
	TKRA_Context *tmp;
	
	tmp=tkra_malloc(sizeof(TKRA_Context));
	memset(tmp, 0, sizeof(TKRA_Context));
	return(tmp);
}

int TKRA_SetupScreen(TKRA_Context *ctx, int xs, int ys)
{
	byte *ptr;
	int scr_memsz;
	int ofs_rgb, ofs_zbuf, ofs_zbuf2, ofs_sten;
	int ofs_pad, ofs_pad_lo, frsz;
	int i, j, k;
	
	frsz=xs*ys;

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
	ptr=tkra_malloc_phys(scr_memsz+31);
	ctx->screen_mem=ptr;
	
	k=(nlint)ptr;
	if(k&15)
		ptr+=(16-(k&15));
	
	ctx->screen_rgb=(void *)(ptr+ofs_rgb);
	ctx->screen_zbuf=(void *)(ptr+ofs_zbuf);
	ctx->screen_zbuf2=(void *)(ptr+ofs_zbuf2);
	ctx->screen_sten=(void *)(ptr+ofs_sten);

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
	
	for(i=0; i<xs*ys; i++)
	{
//		((short *)(ctx->screen_rgb))[i]=0x7FFF;
		((short *)(ctx->screen_rgb))[i]=0x3CEF;
		((short *)(ctx->screen_zbuf))[i]=0x7FFF;
	}

	return(0);
}

int TKRA_DebugPrintStats(TKRA_Context *ctx)
{
#if 1
	if(ctx->stat_base_tris)
	{
//		TKRA_DumpVec4(ctx->prj_xyzsc, "Clip: XyzSc:");
//		TKRA_DumpVec4(ctx->prj_xyzbi, "Clip: XyzBi:");

		printf("clip: L=%f R=%f T=%f B=%f\n",
			ctx->scr_clip_l, ctx->scr_clip_r,
			ctx->scr_clip_t, ctx->scr_clip_b);

	//	tkra_prj_xyzsc=prj_xyzsc;
	//	tkra_prj_xyzbi=prj_xyzbi;

		printf("Stat: Base=%d Frag=%d Draw=%d Reject=%d Blown=%d\n",
			ctx->stat_base_tris,
			ctx->stat_frag_tris,
			ctx->stat_draw_tris,
			ctx->stat_reject_tris,
			ctx->stat_blown_tris);

		printf("Stat: Rejects: Frustum=%d MicroBase=%d "
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


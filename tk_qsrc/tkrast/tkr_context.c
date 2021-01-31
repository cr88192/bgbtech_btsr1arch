
TKRA_Context *TKRA_AllocContext()
{
	TKRA_Context *tmp;
	
	tmp=tkra_malloc(sizeof(TKRA_Context));
	memset(tmp, 0, sizeof(TKRA_Context));
	return(tmp);
}

int TKRA_SetupScreen(TKRA_Context *ctx, int xs, int ys)
{
	int scr_memsz;
	int ofs_rgb, ofs_zbuf, ofs_sten;

	scr_memsz = (xs+2)*16*2;
	scr_memsz = (scr_memsz+15)&(~15);
	ofs_rgb = scr_memsz;
	scr_memsz += (xs+2)*(ys+4)*sizeof(tkra_rastpixel);
	scr_memsz = (scr_memsz+15)&(~15);
	ofs_zbuf = scr_memsz;
	scr_memsz += (xs+2)*(ys+4)*sizeof(tkra_zbufpixel);
	scr_memsz = (scr_memsz+15)&(~15);
	ofs_sten = scr_memsz;
	scr_memsz += (xs+2)*(ys+4)*sizeof(byte);

	ctx->screen_mem=tkra_malloc(scr_memsz);
	ctx->screen_rgb=(void *)(ctx->screen_mem+ofs_rgb);
	ctx->screen_zbuf=(void *)(ctx->screen_mem+ofs_zbuf);
	ctx->screen_sten=(void *)(ctx->screen_mem+ofs_sten);

//	ctx->screen_rgb=tkra_malloc((xs+4)*(ys+16)*sizeof(tkra_rastpixel));
//	ctx->screen_zbuf=tkra_malloc((xs+4)*(ys+16)*sizeof(tkra_zbufpixel));
//	ctx->screen_zbuf=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(tkra_zbufpixel));
//	ctx->screen_sten=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(byte));
//	ctx->screen_sten=tkra_malloc((xs+2)*(ys+2)*sizeof(byte));

	ctx->screen_xsize=xs;
	ctx->screen_ysize=ys;

	ctx->clip_x0=0;
	ctx->clip_x1=320;
	ctx->clip_y0=0;
	ctx->clip_y1=200;

	ctx->scr_xscale=160;
	ctx->scr_yscale=100;
	ctx->scr_xcenter=160;
	ctx->scr_ycenter=100;

	TKRA_InitSpanRcp();

	return(0);
}

int TKRA_SetViewport(TKRA_Context *ctx, int x, int y, int xs, int ys)
{
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
	return(0);
}


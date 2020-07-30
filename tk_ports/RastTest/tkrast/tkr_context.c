
TKRA_Context *TKRA_AllocContext()
{
	TKRA_Context *tmp;
	
	tmp=tkra_malloc(sizeof(TKRA_Context));
	memset(tmp, 0, sizeof(TKRA_Context));
	return(tmp);
}

int TKRA_SetupScreen(TKRA_Context *ctx, int xs, int ys)
{
	ctx->screen_rgb=tkra_malloc((xs+2)*(ys+2)*sizeof(tkra_rastpixel));
//	ctx->screen_zbuf=tkra_malloc((xs+2)*(ys+2)*sizeof(tkra_zbufpixel));
	ctx->screen_zbuf=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(tkra_zbufpixel));
	ctx->screen_sten=tkra_malloc(((xs/2)+2)*((ys/2)+2)*sizeof(byte));
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

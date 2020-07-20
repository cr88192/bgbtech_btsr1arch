
TKRA_Context *TKRA_AllocContext()
{
	TKRA_Context *tmp;
	
	tmp=tkra_malloc(sizeof(TKRA_Context));
	memset(tmp, 0, sizeof(TKRA_Context));
	return(tmp);
}

int TKRA_SetupScreen(TKRA_Context *ctx, int xs, int ys)
{
	ctx->screen_rgb=tkra_malloc(xs*ys*sizeof(tkra_rastpixel));
	ctx->screen_zbuf=tkra_malloc(xs*ys*sizeof(tkra_zbufpixel));
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

#include <tkgdi/tkgdi.h>

void *TK_DlGetApiContextA(u64 apiname, u64 subname);

static _tkgdi_context_t *tkgdi_current_context = NULL;

_tkgdi_context_t *tkgGetCurrentContext()
{
	_tkgdi_context_t *ctx;

//	tk_printf("tkgGetCurrentContext: 0\n");

	ctx=tkgdi_current_context;

//	__debugbreak();

	if(ctx)
	{
//		tk_printf("tkgGetCurrentContext: A\n");
//		__debugbreak();
	
		if(ctx->vt->magic0 != ((void *)0x12345678))
			{ __debugbreak(); }
		if(ctx->vt->magic1 != ((void *)0x12345678))
			{ __debugbreak(); }
		if(ctx->vt->magic2 != ((void *)0x12345678))
			{ __debugbreak(); }

		return(ctx);
	}
	
//	tk_printf("tkgGetCurrentContext: B\n");
//	__debugbreak();

	ctx=TK_DlGetApiContextA(TK_FCC_GDI, TK_FCC_GDI);
	tkgdi_current_context=ctx;

//	__debugbreak();
	
	if(!ctx || !ctx->vt)
		{ __debugbreak(); }
	if(ctx->vt->magic0 != ((void *)0x12345678))
		{ __debugbreak(); }
	if(ctx->vt->magic1 != ((void *)0x12345678))
		{ __debugbreak(); }
	if(ctx->vt->magic2 != ((void *)0x12345678))
		{ __debugbreak(); }
	
	return(ctx);
}

void *tkgGlobalAlloc(size_t size)
{
	return(tk_malloc(size));
}
  
TKGSTATUS tkgGlobalFree(void *obj)
{
	tk_free(obj);
}
  
TKGSTATUS tkgBlitSubImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
	_tkgdi_context_t *ctx;
	TKGDI_RRECT t_rect, t_drect;
	TKGDI_RRECT *rect;
	TKGDI_RRECT *drect;
	
	rect=&t_rect;
	drect=&t_drect;
	rect->xorg=xo_src;
	rect->yorg=yo_src;
	rect->xsize=xs_src;
	rect->ysize=ys_src;

	drect->xorg=xo_dev;
	drect->yorg=yo_dev;
	drect->xsize=xs_src;
	drect->ysize=ys_src;
	
	ctx=tkgGetCurrentContext();

	if(ctx->vt->magic3 == ((void *)0x12345678))
	{
		return(ctx->vt->BlitSubImageNew(ctx,
			dev,
			drect,
			info, data,
			rect));
	}

	return(ctx->vt->BlitSubImage(ctx,
		dev,
		xo_dev, yo_dev,
		info, data,
		xo_src, yo_src, xs_src, ys_src));

}

TKGSTATUS tkgBlitImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data)
{
	return(tkgBlitSubImage(
		dev, xo_dev, yo_dev, info, data,
		0, 0, info->biWidth, info->biHeight));
}

TKGSTATUS tkgQueryDisplay(
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->QueryDisplay(ctx, dev, parm, ifmt, ofmt));
}

TKGSTATUS tkgQueryCreateDisplay(
	TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->QueryDisplay(ctx, 0, TKGDI_FCC_crea, ifmt, ofmt));
}


TKGHDC tkgCreateDisplay(TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_context_t *ctx;
	tk_printf("tkgCreateDisplay: A\n");
	ctx=tkgGetCurrentContext();
	return(ctx->vt->CreateDisplay(ctx, 0, TKGDI_FCC_crea, info));
}

TKGHDC tkgCreateWindow(
	TKGHDC hDev,
	char *title,
	u64 style,
	int x_org,
	int y_org,
	TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_context_t *ctx;
	TKGDI_RECT tRect;
	TKGHDC hDc;
	
	if(hDev<=0)
		hDev=1;
	
	tk_printf("tkgCreateWindow: A\n");
	ctx=tkgGetCurrentContext();
	hDc=ctx->vt->CreateDisplay(ctx, hDev, TKGDI_FCC_crea, info);
	if(hDc<=0)
		return(hDc);
	
	if(x_org || y_org)
	{
		tRect.left=x_org;
		tRect.top=y_org;
		tRect.right=x_org+info->biWidth;
		tRect.bottom=y_org+info->biHeight;
		tkgModifyDisplay(hDc, TKGDI_FCC_move, &tRect, NULL);
	}
	
	if(title)
	{
		tkgModifyDisplay(hDc, TKGDI_FCC_text, title, NULL);
	}
	
	if(style)
	{
		tkgModifyDisplay(hDc, TKGDI_FCC_styl, &style, NULL);
	}
	
	return(hDc);
}

TKGSTATUS tkgDestroyDisplay(TKGHDC dev)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->DestroyDisplay(ctx));
}

TKGSTATUS tkgModifyDisplay(
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->ModifyDisplay(ctx, dev, parm, ifmt, ofmt));
}

TKGSTATUS tkgResizeDisplay(TKGHDC dev, TKGDI_BITMAPINFOHEADER *info)
{
	return(tkgModifyDisplay(dev, TKGDI_FCC_resz, info, NULL));
}

TKGSTATUS tkgDrawString(TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->DrawString(ctx, dev, xo_dev, yo_dev,
		text, font, mode));
}


TKGHSND tkgCreateAudioDevice(TKGHDC dev,
	TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->CreateAudioDevice(ctx, dev, clz, info));
}

TKGSTATUS tkgDestroyAudioDevice(TKGHSND dev)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->DestroyAudioDevice(ctx, dev));
}

TKGSTATUS tkgModifyAudioDevice(
	TKGHSND dev,	TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->ModifyAudioDevice(ctx, dev, parm, ifmt, ofmt));
}


TKGSTATUS tkgQueryAudioDevice(
	TKGHSND dev,	TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->QueryAudioDevice(ctx, dev, parm, ifmt, ofmt));
}

TKGSTATUS tkgWriteSamples(
	TKGHSND dev, void *buffer, int cnt, int ovcnt)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->WriteAudioSamples(ctx, dev, buffer, cnt, ovcnt));
}

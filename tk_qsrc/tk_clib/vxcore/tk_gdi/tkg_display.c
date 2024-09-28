#include <tkgdi/tkgdi.h>

void *TK_DlGetApiContextA(u64 apiname, u64 subname);

static _tkgdi_context_t *tkgdi_current_context = NULL;
static void *tkgdi_smallbuf_ifmt;		//small buffer for ifmt
static void *tkgdi_smallbuf_ofmt;		//small buffer for ofmt

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

		if(!tkgdi_smallbuf_ifmt)
		{
			tkgdi_smallbuf_ifmt=tkgGlobalAlloc(4096);
			tkgdi_smallbuf_ofmt=tkgGlobalAlloc(4096);
		}

		return(ctx);
	}
	
//	tk_printf("tkgGetCurrentContext: B\n");
//	__debugbreak();

	ctx=TK_DlGetApiContextA(TK_FCC_GDI, TK_FCC_GDI);
	tkgdi_current_context=ctx;

	tkgdi_smallbuf_ifmt=tkgGlobalAlloc(4096);
	tkgdi_smallbuf_ofmt=tkgGlobalAlloc(4096);

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
	void *ptr;
//	return(tk_malloc(size));

	ptr=tk_malloc_cat(size, TKMM_MCAT_GLOBAL);
	if(!ptr)
	{
		__debugbreak();
	}

	return(ptr);
}
  
TKGSTATUS tkgGlobalFree(void *obj)
{
	tk_free(obj);
}

int TKGDI_CopyBitmapInfo(
	TKGDI_BITMAPINFOHEADER *dst,
	TKGDI_BITMAPINFOHEADER *src)
{
	int szb, szs, clrused, opt;
	
	if(!src || !dst)
		return(0);
	
	szb=src->biSize;
	if(szb<40)
	{
		tk_printf("TKGDI_CopyBitmapInfo: "
			"Src BITMAPINFOHEADER too small, %d bytes\n", szb);
		szb=40;
	}
	
	if(szb>512)
	{
		tk_printf("TKGDI_CopyBitmapInfo: Reject, "
			"Src BITMAPINFOHEADER too big, %d bytes\n", szb);
		return(-1);
	}
	
	clrused=src->biClrUsed;
	if(!clrused && (src->biBitCount<=8) && (src->biBitCount>0))
	{
		clrused=1<<src->biBitCount;
	}
	
	opt=0;
	if(src->biCompression>=TKGDI_FCC_4xSpace)
	{
		opt=1024;
		if(src->biCompression==TKGDI_FCC_text)
			opt=0;
	}
	
	szs=szb+(clrused*4)+opt;
	
	memcpy(dst, src, szs);
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
	
// #ifdef __RISCV__
#if 0
//	__debugbreak();
	tk_printf("tkgBlitSubImage: Rect1 %d %d %d %d\n",
		xo_src, yo_src, xs_src, ys_src);
	tk_printf("tkgBlitSubImage: Rect2 %d %d %d %d\n",
		xo_dev, yo_dev, xs_src, ys_src);
#endif

	ctx=tkgGetCurrentContext();

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);

	if(ctx->vt->magic3 == ((void *)0x12345678))
	{
		return(ctx->vt->BlitSubImageNew(ctx,
			dev,
			drect,
//			info, data,
			tkgdi_smallbuf_ifmt, data,
			rect));
	}

	tk_printf("tkgBlitSubImage: Old Path\n");

	return(ctx->vt->BlitSubImage(ctx,
		dev,
		xo_dev, yo_dev,
//		info, data,
		tkgdi_smallbuf_ifmt, data,
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
	int res;
	ctx=tkgGetCurrentContext();

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, ifmt);
//	res=ctx->vt->QueryDisplay(ctx, 0, TKGDI_FCC_crea, ifmt, ofmt);
	res=ctx->vt->QueryDisplay(ctx, 0, TKGDI_FCC_crea,
		tkgdi_smallbuf_ifmt,
		tkgdi_smallbuf_ofmt);
	TKGDI_CopyBitmapInfo(ofmt, tkgdi_smallbuf_ofmt);
	return(res);
}

TKGSTATUS tkgPollEvent(TKGHDC dev, TKGDI_EVENT *imsg)
{
	return(tkgQueryDisplay(dev, TKGDI_FCC_poll, NULL, imsg));
}

TKGSTATUS tkgPushEvent(TKGHDC dev, TKGDI_EVENT *imsg)
{
	return(tkgQueryDisplay(dev, TKGDI_FCC_poll, imsg, NULL));
}

void *tkgTryMapFrameBuffer(TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_context_t *ctx;
	void *ptr;
	int res;

	ctx=tkgGetCurrentContext();

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);
	
	ptr=NULL;
	*(void **)tkgdi_smallbuf_ofmt=NULL;
//	res=ctx->vt->QueryDisplay(ctx, dev, TKGDI_FCC_mapf, info, &ptr);
	res=ctx->vt->QueryDisplay(ctx, dev, TKGDI_FCC_mapf,
		tkgdi_smallbuf_ifmt, tkgdi_smallbuf_ofmt);
	ptr=*(void **)tkgdi_smallbuf_ofmt;
	
	if(res<0)
		return(NULL);

	return(ptr);
}

TKGSTATUS tkgMapReleaseFrameBuffer(TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info, void *ptr)
{
	_tkgdi_context_t *ctx;
	void *tptr;
	int res;
	
	tptr=ptr;
	ctx=tkgGetCurrentContext();

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);
	*(void **)tkgdi_smallbuf_ofmt=ptr;

//	res=ctx->vt->QueryDisplay(ctx, dev, TKGDI_FCC_umap, info, &tptr);
	res=ctx->vt->QueryDisplay(ctx, dev, TKGDI_FCC_umap,
		tkgdi_smallbuf_ifmt,
		tkgdi_smallbuf_ofmt);
	return(res);
}

TKGSTATUS tkgMapFlipFrame(TKGHDC dev)
{
	_tkgdi_context_t *ctx;
	int res;
	
	ctx=tkgGetCurrentContext();
	res=ctx->vt->QueryDisplay(ctx, dev, TKGDI_FCC_flip, NULL, NULL);
	return(res);
}

TKGHDC tkgCreateDisplay(TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_context_t *ctx;
	tk_printf("tkgCreateDisplay: A\n");
	ctx=tkgGetCurrentContext();

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);

//	return(ctx->vt->CreateDisplay(ctx, 0, TKGDI_FCC_crea, info));
	return(ctx->vt->CreateDisplay(ctx, 0, TKGDI_FCC_crea,
		tkgdi_smallbuf_ifmt));
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

	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);

	tk_printf("tkgCreateWindow: A\n");
	ctx=tkgGetCurrentContext();
//	hDc=ctx->vt->CreateDisplay(ctx, hDev, TKGDI_FCC_crea, info);
	hDc=ctx->vt->CreateDisplay(ctx, hDev, TKGDI_FCC_crea,
		tkgdi_smallbuf_ifmt);
	if(hDc<=0)
		return(hDc);
	
	if(x_org || y_org)
	{
		tRect.left=x_org;
		tRect.top=y_org;
		tRect.right=x_org+info->biWidth;
		tRect.bottom=y_org+info->biHeight;
		memcpy(tkgdi_smallbuf_ifmt, &tRect, sizeof(TKGDI_RECT));
//		tkgModifyDisplay(hDc, TKGDI_FCC_move, &tRect, NULL);
		tkgModifyDisplay(hDc, TKGDI_FCC_move, tkgdi_smallbuf_ifmt, NULL);
	}
	
	if(title)
	{
		strcpy(tkgdi_smallbuf_ifmt, title);
//		tkgModifyDisplay(hDc, TKGDI_FCC_text, title, NULL);
		tkgModifyDisplay(hDc, TKGDI_FCC_text, tkgdi_smallbuf_ifmt, NULL);
	}
	
	if(style)
	{
		*(u64 *)tkgdi_smallbuf_ifmt=style;
//		tkgModifyDisplay(hDc, TKGDI_FCC_styl, &style, NULL);
		tkgModifyDisplay(hDc, TKGDI_FCC_styl, tkgdi_smallbuf_ifmt, NULL);
	}

	return(hDc);
}

TKGHDC tkgSetWindowTitle(
	TKGHDC hDev,
	char *title)
{
	_tkgdi_context_t *ctx;
	int rt;

	ctx=tkgGetCurrentContext();
	strcpy(tkgdi_smallbuf_ifmt, title);
	rt=tkgModifyDisplay(hDev, TKGDI_FCC_text, tkgdi_smallbuf_ifmt, NULL);
	return(rt);
}

TKGHDC tkgSetWindowStyle(
	TKGHDC hDev,
	u64 style)
{
	_tkgdi_context_t *ctx;
	int rt;

	ctx=tkgGetCurrentContext();

	*(u64 *)tkgdi_smallbuf_ifmt=style;
	rt=tkgModifyDisplay(hDev, TKGDI_FCC_styl, tkgdi_smallbuf_ifmt, NULL);

	return(rt);
}

TKGHDC tkgMoveWindow(
	TKGHDC hDev,
	int xorg, int yorg)
{
	TKGDI_RECT tRect;
	_tkgdi_context_t *ctx;
	int rt;

	ctx=tkgGetCurrentContext();

	tRect.left=xorg;
	tRect.top=yorg;
	tRect.right=xorg;
	tRect.bottom=yorg;
	memcpy(tkgdi_smallbuf_ifmt, &tRect, sizeof(TKGDI_RECT));
	rt=tkgModifyDisplay(hDev, TKGDI_FCC_move, tkgdi_smallbuf_ifmt, NULL);

	return(rt);
}

TKGSTATUS tkgDestroyDisplay(TKGHDC dev)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	return(ctx->vt->DestroyDisplay(ctx, dev));
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
	TKGDI_CopyBitmapInfo(tkgdi_smallbuf_ifmt, info);
//	return(tkgModifyDisplay(dev, TKGDI_FCC_resz, info, NULL));
	return(tkgModifyDisplay(dev, TKGDI_FCC_resz,
		tkgdi_smallbuf_ifmt, NULL));
}

TKGHFONT tkgResolveFont(TKGHDC dev, char *name,
	int c_xs, int c_ys, int style)
{
	TKGDI_FONT_COMMAND *fcmd;
	TKGDI_FONT_COMMAND *frsp;
	char *nambuf;
	
	fcmd=(TKGDI_FONT_COMMAND *)tkgdi_smallbuf_ifmt;
	nambuf=(char *)(fcmd+1);
	strcpy(nambuf, name);

	frsp=(TKGDI_FONT_COMMAND *)tkgdi_smallbuf_ofmt;

	fcmd->fcSize=sizeof(TKGDI_FONT_COMMAND);
	fcmd->fcCmd=1;
	fcmd->fcSizeX=c_xs;
	fcmd->fcSizeY=c_ys;
	fcmd->fcStyle=style;
	fcmd->sName=nambuf-((char *)fcmd);
	fcmd->sPath=0;
	
	tkgModifyDisplay(dev, TKGDI_FCC_fcmd,
		tkgdi_smallbuf_ifmt, tkgdi_smallbuf_ofmt);
	return(frsp->fcFont);
}


TKGSTATUS tkgFontGetCelBits(TKGHDC dev, TKGHFONT font,
	int codepoint, void *bits,
	int c_xs, int c_ys, int style)
{
	TKGDI_FONT_COMMAND *fcmd;
	TKGDI_FONT_COMMAND *frsp;
	char *nambuf, *bitsbuf;
	int celsz;
	
	celsz=(c_xs*c_ys+7)>>3;
	
	fcmd=(TKGDI_FONT_COMMAND *)tkgdi_smallbuf_ifmt;
	nambuf=(char *)(fcmd+1);
//	strcpy(nambuf, name);

	frsp=(TKGDI_FONT_COMMAND *)tkgdi_smallbuf_ofmt;
	bitsbuf=(char *)(frsp+1);

	fcmd->fcSize=sizeof(TKGDI_FONT_COMMAND);
	fcmd->fcCmd=2;
	fcmd->fcFont=font;
	fcmd->fcChar=codepoint;
	fcmd->fcSizeX=c_xs;
	fcmd->fcSizeY=c_ys;
	fcmd->fcStyle=style;
	fcmd->sName=nambuf-((char *)fcmd);
	fcmd->sPath=bitsbuf-((char *)frsp);

	frsp->sName=bitsbuf-((char *)frsp);
	
	tkgModifyDisplay(dev, TKGDI_FCC_fcmd,
		tkgdi_smallbuf_ifmt, tkgdi_smallbuf_ofmt);
	
	memcpy(bits, bitsbuf, celsz);
	
//	return(frsp->fcFont);
	return(1);
}

TKGSTATUS tkgDrawString(TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();
	
	strcpy(tkgdi_smallbuf_ifmt, text);
	
	return(ctx->vt->DrawString(ctx, dev, xo_dev, yo_dev,
//		text, font, mode));
		tkgdi_smallbuf_ifmt, font, mode));
}


TKGHSND tkgCreateAudioDevice(TKGHDC dev,
	TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();

	memcpy(tkgdi_smallbuf_ifmt, info, sizeof(TKGDI_WAVEFORMATEX));
//	return(ctx->vt->CreateAudioDevice(ctx, dev, clz, info));
	return(ctx->vt->CreateAudioDevice(ctx, dev, clz,
		tkgdi_smallbuf_ifmt));
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

TKGSTATUS tkgDeviceMidiCommand(
	TKGHSND dev,	TKGDI_MIDI_COMMAND *mcmd)
{
	_tkgdi_context_t *ctx;
	ctx=tkgGetCurrentContext();

	memcpy(tkgdi_smallbuf_ifmt, mcmd, sizeof(TKGDI_MIDI_COMMAND));

	return(ctx->vt->ModifyAudioDevice(ctx, dev,
		TKGDI_FCC_mcmd, tkgdi_smallbuf_ifmt, NULL));

//	return(tkgModifyAudioDevice(dev, TKGDI_FCC_mcmd, mcmd, NULL));
//	return(tkgModifyAudioDevice(dev,
//		TKGDI_FCC_mcmd, tkgdi_smallbuf_ifmt, NULL));
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

#include <tkgdi/tkgdi.h>

int tkgdi_isinit=0;

int TKGDI_Init()
{
	if(tkgdi_isinit)
		return(0);
	tkgdi_isinit=1;

	GfxDrv_Start();
	return(1);
}

TKGSTATUS TKGDI_BlitSubImage(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
}

TKGSTATUS TKGDI_QueryDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
}

TKGHDC TKGDI_CreateDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	TKGDI_BITMAPINFOHEADER *info)
{
	TKGDI_Init();
}

TKGSTATUS TKGDI_DestroyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
}

TKGSTATUS TKGDI_ModifyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt)
{
}

TKGSTATUS TKGDI_DrawString(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
}

TKGHSND TKGDI_CreateAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHDC dev, TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
}

TKGSTATUS TKGDI_DestroyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev)
{
}

TKGSTATUS TKGDI_ModifyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev, TKGFOURCC parm,
	void *info, void *rinfo)
{
}

TKGSTATUS TKGDI_QueryAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev, TKGFOURCC parm,
	void *info, void *rinfo)
{
}

TKGSTATUS TKGDI_WriteAudioSamples(
	_tkgdi_context_t *ctx,
	TKGHSND dev, void *buffer, int cnt, int ovcnt)
{
}

TKGSTATUS TKGDI_BlitSubImageNew(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_RRECT *drect,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	TKGDI_RRECT *rect)
{
}

_tkgdi_context_vtable_t tkgdi_w32_vt = {
NULL,
NULL,

(void *)0x12345678,

TKGDI_BlitSubImage,
TKGDI_QueryDisplay,
TKGDI_CreateDisplay,
TKGDI_DestroyDisplay,
TKGDI_ModifyDisplay,
TKGDI_DrawString,

(void *)0x12345678,

TKGDI_CreateAudioDevice,
TKGDI_DestroyAudioDevice,
TKGDI_ModifyAudioDevice,
TKGDI_QueryAudioDevice,
TKGDI_WriteAudioSamples,

(void *)0x12345678,

TKGDI_BlitSubImageNew

};


void *TKW_TkGdiGetApiContext(u64 apiname, u64 subname)
{
	_tkgdi_context_t tctx;
	_tkgdi_context_t *ctx;
	
	ctx=&tctx;
	ctx->vt=&tkgdi_w32_vt;
	ctx->sobj=ctx;
	
	return(ctx);
}

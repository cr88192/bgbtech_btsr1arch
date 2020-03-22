#include <windows.h>
#include <tkgl/bgbrasw.h>

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglCopyContext(
	HGLRC a0, HGLRC a1, UINT a2)
{
}

BGBRASW_API HGLRC GLDRV_APIENTRY GlDrv_wglCreateContext(HDC hdc)
{
	RASWGL_Context *ragl;
	BITMAPINFO dib, *pdib;
	void *pix;
	HBITMAP hbmp;
	HDC hdcDib;
	HGDIOBJ hdlSel;
//	HGLRC glrc;
	int xs, ys;
	
	xs=GetDeviceCaps(hdc, HORZRES);
	ys=GetDeviceCaps(hdc, VERTRES);

//	xs=1024; ys=768;
//	xs=640; ys=480;

//	ragl=RaGlCreateContext(xs, ys, 0);
	ragl=RASWGL_CreateContext(xs, ys, 0);

//	ragl=BGBRASW_AllocContext();
	
//	glrc=wglCreateContext(hdc);
//	CreateCompatibleBitmap(hdc, xs, ys);

	pdib=&dib;
	memset(pdib, 0, sizeof(BITMAPINFO));
	
	pdib->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	pdib->bmiHeader.biWidth         = xs;
	pdib->bmiHeader.biHeight        = ys;
	pdib->bmiHeader.biPlanes        = 1;
	pdib->bmiHeader.biBitCount      = 32;
	pdib->bmiHeader.biCompression   = BI_RGB;
	pdib->bmiHeader.biSizeImage     = 0;
	pdib->bmiHeader.biXPelsPerMeter = 0;
	pdib->bmiHeader.biYPelsPerMeter = 0;
	pdib->bmiHeader.biClrUsed       = 0;
	pdib->bmiHeader.biClrImportant  = 0;

	pix=NULL;
	hbmp=CreateDIBSection(hdc, pdib, DIB_RGB_COLORS, &pix, NULL, 0);

//	ragl->tgt_rgba=pix;
//	ragl->tgt_xs=xs;
//	ragl->tgt_ys=ys;

	ragl->os_hdc=hdc;
//	ragl->os_hglrc=glrc;
	ragl->os_hbmp=hbmp;
	ragl->os_rgba=pix;
	
	GdiFlush();
	memset(pix, 0, xs*ys*4);
	
	hdcDib=CreateCompatibleDC(hdc);
	ragl->os_hdcdib=hdcDib;
	
	hdlSel=SelectObject(hdcDib, hbmp);
	ragl->os_hdlsel=hdlSel;
	
	return((HGLRC)ragl);
}

BGBRASW_API HGLRC GLDRV_APIENTRY GlDrv_wglCreateLayerContext(
	HDC a0, int a1)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglDeleteContext(HGLRC a0)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglDescribeLayerPlane(
	HDC a0, int a1, int a2, UINT a3, LPLAYERPLANEDESCRIPTOR a4)
{
}

BGBRASW_API HGLRC GLDRV_APIENTRY GlDrv_wglGetCurrentContext(void)
{
	RASWGL_Context *ragl;
	ragl=RaGlGetContext();
	return((HGLRC)ragl);
}

BGBRASW_API HDC GLDRV_APIENTRY GlDrv_wglGetCurrentDC(void)
{
	RASWGL_Context *ragl;
	ragl=RaGlGetContext();
	return((HDC)(ragl->os_hdc));
}

BGBRASW_API int GLDRV_APIENTRY GlDrv_wglGetLayerPaletteEntries(
	HDC a0, int a1, int a2, int a3, COLORREF *a4)
{
}

BGBRASW_API PROC GLDRV_APIENTRY GlDrv_wglGetProcAddress(
	LPCSTR a0)
{
	return(NULL);
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglMakeCurrent(
	HDC hdc, HGLRC hglrc)
{
	RASWGL_Context *ragl;
	
	if(!hdc && !hglrc)
	{
		RaGlSetContext(NULL);
		return(1);
//		return(wglMakeCurrent(NULL, NULL));
	}
	
	ragl=(RASWGL_Context *)hglrc;
	RaGlSetContext(ragl);
	ragl->os_hdc=hdc;
	return(1);

//	return(wglMakeCurrent(hdc, ragl->os_hglrc));
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglRealizeLayerPalette(
	HDC a0, int a1, BOOL a2)
{
}

BGBRASW_API int GLDRV_APIENTRY GlDrv_wglSetLayerPaletteEntries(
	HDC a0, int a1, int a2, int a3, const COLORREF *a4)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglShareLists(
	HGLRC a0, HGLRC a1)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglSwapLayerBuffers(
	HDC a0, UINT a1)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglUseFontBitmapsA(
	HDC a0, DWORD a1, DWORD a2, DWORD a3)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglUseFontBitmapsW(
	HDC a0, DWORD a1, DWORD a2, DWORD a3)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglUseFontOutlinesA(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
{
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglUseFontOutlinesW(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
{
}


BGBRASW_API int GLDRV_APIENTRY GlDrv_wglChoosePixelFormat(
	HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
	PIXELFORMATDESCRIPTOR pfd2, *ppfd2;

	ppfd2=&pfd2;
	memcpy(ppfd2, ppfd, sizeof(PIXELFORMATDESCRIPTOR));
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->cColorBits=32;
	ppfd2->cDepthBits=0;
	ppfd2->cStencilBits=0;

	return(ChoosePixelFormat(hdc, ppfd2));
}

BGBRASW_API int GLDRV_APIENTRY GlDrv_wglGetPixelFormat(HDC hdc)
{
	return(GetPixelFormat(hdc));
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglSetPixelFormat(
	HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd)
{
	PIXELFORMATDESCRIPTOR pfd2, *ppfd2;

	ppfd2=&pfd2;
	memcpy(ppfd2, ppfd, sizeof(PIXELFORMATDESCRIPTOR));
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->cColorBits=32;
	ppfd2->cDepthBits=0;
	ppfd2->cStencilBits=0;

	return(SetPixelFormat(hdc, iPixelFormat, ppfd2));
}

BGBRASW_API int GLDRV_APIENTRY GlDrv_wglDescribePixelFormat(
	HDC hdc, INT iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
	return(DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd));
}

BGBRASW_API BOOL GLDRV_APIENTRY GlDrv_wglSwapBuffers(HDC hdc)
{
	RASWGL_Context *ragl;

	ragl=RaGlGetContext();
	if(!ragl)return(0);
	if(!ragl->os_rgba)return(0);
//	if(!ragl->ractx->tgt_rgba)return(0);

	RASWGL_Finish(ragl);

	GdiFlush();
#ifdef BGBRASW_SS_YUVA
	if(!ragl->ractx->tgt_ssyuva)return(0);
	BGBRASW_ConvUnpackYUVAToRGBA(
		ragl->os_rgba, ragl->ractx->tgt_ssyuva,
		ragl->ractx->tgt_lxs, ragl->ractx->tgt_lys,
		ragl->ractx->tgt_lxs, ragl->ractx->tgt_xs);
#else
	if(!ragl->ractx->tgt_rgba)return(0);
	memcpy(ragl->os_rgba, ragl->ractx->tgt_rgba,
		ragl->ractx->tgt_xs*ragl->ractx->tgt_ys*sizeof(u32));
#endif

#if 0
	BitBlt(ragl->os_hdc,
	    0, 0,
		ragl->ractx->tgt_xs,
		ragl->ractx->tgt_ys,
		ragl->os_hdcdib,
		0, 0,
		SRCCOPY);
#endif

#if 1
	BitBlt(ragl->os_hdc,
//	    ragl->ractx->tgt_xs-(ragl->viewport_x+ragl->viewport_xs),
		ragl->viewport_x,
		0,
//		-ragl->ractx->tgt_ys-(ragl->viewport_y+ragl->viewport_ys),
		ragl->viewport_xs,
		ragl->viewport_ys,
		ragl->os_hdcdib,
		0, ragl->ractx->tgt_lys-(ragl->viewport_y+ragl->viewport_ys),
		SRCCOPY);
#endif

//	return(SwapBuffers(hdc));
	return(1);
}

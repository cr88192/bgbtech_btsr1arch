#include <tk_core.h>
#include <tkgdi/tkgdi.h>

static u64 tkgdi_ptrmangle_key=0;

TKGSTATUS TKGDI_BlitSubImageNew(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_RRECT *drect,
//	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	TKGDI_RRECT *rect)
//	int xo_src, int yo_src, int xs_src, int ys_src)
{
	u16 pal_16b[256];
	_tkgdi_window_t *wctx;
	byte *bmct, *csb;
	u16 *cs, *ct, *imgtmp;
	u32 *pal;
	int xo_dev, yo_dev, xs_bmp, ys_bmp;
	int xo_src, yo_src, xs_src, ys_src;
	int xs, ys, mxs, mys, bxs, bys, nx, ny, flip;
	int xstr_src;
	int i, j, k;

	if(dev<=0)
		return(-1);
	
	xo_dev=drect->xorg;
	yo_dev=drect->yorg;
	
	xo_src=rect->xorg;
	yo_src=rect->yorg;
	xs_src=rect->xsize;
	ys_src=rect->ysize;
	
	if(xs_src<0)
		xs_src=-xs_src;
	if(ys_src<0)
		ys_src=-ys_src;
	
	flip=(info->biHeight>0);
	xs_bmp=info->biWidth;
	ys_bmp=info->biHeight;
	if(xs_bmp<0)
		xs_bmp=-xs_bmp;
	if(ys_bmp<0)
		ys_bmp=-ys_bmp;
	
//	return(-1);
		
	if(dev==1)
	{
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555_LFB)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=320-xo_dev;
			mys=200-yo_dev;
			
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_LfbRgb555(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}
			}

		}
	
//		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555)
		if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555) ||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_RGB555))
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=320-xo_dev;
			mys=200-yo_dev;
			
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_BlkRgb555(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 8)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex8(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 4)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex4(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 24)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkRgb888(xo_dev, yo_dev, xs, ys,
						data, 3, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 32)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkRgb888(xo_dev, yo_dev, xs, ys,
						data, 4, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}
			}else
				if(info->biCompression == TKGDI_BI_HDRU)
			{
				if(info->biBitCount == 32)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkRgb888H(xo_dev, yo_dev, xs, ys,
						data, 4, xo_src, yo_src,
						info->biWidth, info->biHeight,
						info->biClrUsed);
				}
			}

		}

//		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)
		if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)			||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)	||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_800x600_CC)	)
		{
			xs=xs_src;
			ys=ys_src;
			
//			mxs=640-xo_dev;
			mxs=tkgdi_vid_xsize-xo_dev;
//			mys=400-yo_dev;
			mys=tkgdi_vid_ysize-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}

				if((info->biBitCount == 8) ||
					(info->biBitCount == 4))
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex_CCE(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight,
						info->biBitCount);
				}

				if((info->biBitCount == 24) ||
					(info->biBitCount == 32))
				{
					TKGDI_BlitUpdate_BlkIndex_CCE(xo_dev, yo_dev, xs, ys,
						data, NULL, xo_src, yo_src,
						info->biWidth, info->biHeight,
						info->biBitCount);
				}
			}

			if(info->biCompression == TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}

#if 0
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=640-xo_dev;
			mys=200-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression==0)
			{
				TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}

			if(info->biCompression==TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}
#endif

		return(0);
	}
	
	if((dev>1) && (dev<tkgdi_n_windows))
	{
		wctx=tkgdi_windows[dev];
		
		if(!wctx || !wctx->buf_info)
		{
			tk_dbg_printf("TKGDI_BlitSubImage: Bad wctx=%p\n",
				wctx);
			return(-1);
		}

		xs=xs_src;
		ys=ys_src;
		
		bxs=wctx->buf_info->biWidth;
		bys=wctx->buf_info->biHeight;
		mxs=bxs-xo_dev;
		mys=bys-yo_dev;
		if(xs>mxs)
			xs=mxs;
		if(ys>mys)
			ys=mys;
			
		cs=data;
		ct=wctx->buf_data;
		csb=data;

		if(info->biCompression==TKGDI_BI_RGB)
		{
			if(info->biBitCount==16)
			{
				xstr_src=(xs_bmp+1)&(~1);

				if(flip)
				{
	//				cs+=(ys-1)*xs_src;
	//				cs+=(ys-1)*xs_bmp;
					cs+=(ys-1)*xstr_src;
					cs-=yo_src*xstr_src;
					cs+=xo_src;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						memcpy(ct, cs, xs*2);
	//					cs-=xs_src;
//						cs-=xs_bmp;
						cs-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					cs+=yo_src*xstr_src;
					cs+=xo_src;
			
					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						memcpy(ct, cs, xs*2);
	//					cs+=xs_src;
//						cs+=xs_bmp;
						cs+=xstr_src;
						ct+=bxs;
					}
				}
			}

			if(info->biBitCount==8)
			{
				pal=(u32 *)(((byte *)info)+info->biSize);
				TKGDI_CopyPixelSpan_SetupPalArrayF(pal_16b, pal, 256);

	//			xstr_src=(xs_src+3)&(~3);
				xstr_src=(xs_bmp+3)&(~3);

				if(flip)
				{
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
	//					memcpy(ct, cs, xs*2);
	//					for(j=0; j<xs; j++)
	//						ct[j]=pal_16b[csb[j]];
						TKGDI_CopyPixelSpan_CnvI8to15(ct, csb, xs, pal_16b);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
	//					memcpy(ct, cs, xs*2);
	//					for(j=0; j<xs; j++)
	//						ct[j]=pal_16b[csb[j]];
						TKGDI_CopyPixelSpan_CnvI8to15(ct, csb, xs, pal_16b);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}

			if(info->biBitCount==4)
			{
				pal=(u32 *)(((byte *)info)+info->biSize);
				TKGDI_CopyPixelSpan_SetupPalArrayF(pal_16b, pal, 16);

	//			xstr_src=((xs_src+7)&(~7))>>1;
				xstr_src=((xs_bmp+7)&(~7))>>1;

				if(flip)
				{
	//				csb+=((ys-1)*xs_src)>>1;
	//				csb+=(ys-1)*((xs_src+1)>>1);
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src>>1;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_CnvI4to15(ct, csb, xs, pal_16b);
	//					csb-=((xs_src+1)>>1);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src>>1;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_CnvI4to15(ct, csb, xs, pal_16b);
	//					csb+=((xs_src+1)>>1);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}

			if(info->biBitCount==24)
			{
				xstr_src=xs_bmp*3;
				xstr_src=(xstr_src+3)&(~3);

				if(flip)
				{
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src*3;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv24to15(ct, csb, xs);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src*3;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv24to15(ct, csb, xs);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}

			if(info->biBitCount==32)
			{
				xstr_src=xs_bmp<<2;

				if(flip)
				{
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src<<2;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv32to15(ct, csb, xs);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src<<2;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv32to15(ct, csb, xs);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}
		}else
			if(	(info->biCompression==TKGDI_FCC_hdr) ||
				(info->biCompression==TKGDI_FCC_hdru))
		{
			if(info->biBitCount==24)
			{
				xstr_src=xs_bmp*3;
				xstr_src=(xstr_src+3)&(~3);

				if(flip)
				{
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src*3;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv24Hto15(ct, csb, xs,
							info->biClrUsed);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src*3;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv24Hto15(ct, csb, xs,
							info->biClrUsed);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}

			if(info->biBitCount==32)
			{
				xstr_src=xs_bmp<<2;

				if(flip)
				{
					csb+=(ys-1)*xstr_src;
					csb-=yo_src*xstr_src;
					csb+=xo_src<<2;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv32Hto15(ct, csb, xs,
							info->biClrUsed);
						csb-=xstr_src;
						ct+=bxs;
					}
				}else
				{
					csb+=yo_src*xstr_src;
					csb+=xo_src<<2;

					ct+=yo_dev*bxs+xo_dev;
					for(i=0; i<ys; i++)
					{
						TKGDI_CopyPixelSpan_Cnv32Hto15(ct, csb, xs,
							info->biClrUsed);
						csb+=xstr_src;
						ct+=bxs;
					}
				}
			}
		}else
			if(	(info->biCompression==TKGDI_FCC_CRAM) ||
				(info->biCompression==TKGDI_FCC_UTX2))
		{
			xstr_src=(xs_bmp+1)&(~1);

			imgtmp=tk_malloc(xstr_src*ys_bmp*2);
			cs=imgtmp;

			if(info->biCompression==TKGDI_FCC_UTX2)
			{
				TKGDI_CopyPixelSpan_ImageCnvUTX2to15(
					imgtmp, data, xs_bmp, ys_bmp);
			}else
				if(info->biBitCount==8)
			{
				pal=(u32 *)(((byte *)info)+info->biSize);
				TKGDI_CopyPixelSpan_SetupPalArrayF(pal_16b, pal, 256);
				
				TKGDI_CopyPixelSpan_ImageCnvCRAM8to15(imgtmp, data,
					xs_bmp, ys_bmp, pal_16b);
			}else
				if(info->biBitCount==16)
			{
				TKGDI_CopyPixelSpan_ImageCnvCRAM16to15(
					imgtmp, data, xs_bmp, ys_bmp);
			}

			if(flip)
			{
				cs+=(ys-1)*xstr_src;
				cs-=yo_src*xstr_src;
				cs+=xo_src;

				ct+=yo_dev*bxs+xo_dev;
				for(i=0; i<ys; i++)
				{
					memcpy(ct, cs, xs*2);
					cs-=xstr_src;
					ct+=bxs;
				}
			}else
			{
				cs+=yo_src*xstr_src;
				cs+=xo_src;
		
				ct+=yo_dev*bxs+xo_dev;
				for(i=0; i<ys; i++)
				{
					memcpy(ct, cs, xs*2);
					cs+=xstr_src;
					ct+=bxs;
				}
			}
			tk_free(imgtmp);
		}
		
		nx=(xo_dev+xs+3)>>2;
		ny=(yo_dev+ys+3)>>2;

		for(i=(yo_dev>>2); i<ny; i++)
		{
			bmct=wctx->buf_dirty1+(i*wctx->size_bmxs);
			for(j=(xo_dev>>2); j<nx; j++)
			{
				if(!(j&7) && ((j+8)<=nx))
				{
					bmct[j>>3]=0xFF;
					j+=7;
					continue;
				}
				bmct[j>>3]|=1<<(j&7);
			}
		}
		wctx->dirty1=1;

		if(wctx->con)
			tkgdi_con_redrawbuffer(wctx->con);

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();

		return(0);
	}
	
	tk_dbg_printf("TKGDI_BlitSubImage: Bad hDc=%d max=%d\n",
		dev, tkgdi_n_windows);

	return(-1);
}

TKGSTATUS TKGDI_WindowMarkDirtyRect(
	_tkgdi_context_t *ctx,
	_tkgdi_window_t *wctx,
//	TKGDI_RRECT *rect)
	int xo_src, int yo_src, int xs_src, int ys_src)
{
//	_tkgdi_window_t *wctx;
	byte *bmct;
	u16 *cs, *ct;
	u32 *pal;
	int xo_dev, yo_dev;
//	int xo_src, yo_src, xs_src, ys_src;
	int xs, ys, mxs, mys, bxs, bys, nx, ny;
	int i, j, k;

//	if(dev<=0)
//		return(-1);
	
//	xo_src=rect->xorg;
//	yo_src=rect->yorg;
//	xs_src=rect->xsize;
//	ys_src=rect->ysize;
	
//	if(xs_src<0)
//		xs_src=-xs_src;
//	if(ys_src<0)
//		ys_src=-ys_src;
	
//	return(-1);

	bxs=wctx->buf_info->biWidth;
	bys=wctx->buf_info->biHeight;
	mxs=bxs-xo_src;
	mys=bys-yo_src;
	if(xs_src>mxs)
		xs_src=mxs;
	if(ys_src>mys)
		ys_src=mys;

	nx=(xo_src+xs_src+3)>>2;
	ny=(yo_src+ys_src+3)>>2;

	for(i=(yo_src>>2); i<=ny; i++)
	{
		bmct=wctx->buf_dirty1+(i*wctx->size_bmxs);
		for(j=(xo_src>>2); j<=nx; j++)
		{
			if(!(j&7) && ((j+8)<=nx))
			{
				bmct[j>>3]=0xFF;
				j+=7;
				continue;
			}
			bmct[j>>3]|=1<<(j&7);
		}
	}
	wctx->dirty1=1;

//	TKGDI_UpdateWindowCells(wctx);
	return(0);
}
		

TKGSTATUS TKGDI_BlitSubImageOld(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
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
	
	return(ctx->vt->BlitSubImageNew(ctx,
		dev,
		drect,
		info, data,
		rect));
}

int TKGDI_ModeForInputFormat(TKGDI_BITMAPINFOHEADER *ifmt)
{
	int ofmt_mode;

	if(!ifmt)
	{
		tk_dbg_printf("TKGDI_ModeForInputFormat: Got NULL\n");
		return(-1);
	}

	ofmt_mode=-1;

	tk_dbg_printf("TKGDI_ModeForInputFormat: W=%d H=%d bpp=%d\n",
		ifmt->biWidth, ifmt->biHeight, ifmt->biBitCount);

	if((ifmt->biBitCount == 16) || (ifmt->biBitCount == 15) ||
		(ifmt->biBitCount == 24) || (ifmt->biBitCount == 32))
	{

#if 0
		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
//			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555_LFB;		//Use hi-color
		}
#endif

		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
//			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555_LFB;		//Use hi-color
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_TEXT;		//Use 80x25 color cell
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 400) )
		{
//			ofmt_mode=TKGDI_SCRMODE_640x400_CC;	//Use 80x50 color cell
//			ofmt_mode=TKGDI_SCRMODE_640x400_RGB555;
			ofmt_mode=TKGDI_SCRMODE_640x400_PAL8B;
		}
		
		if(	(ifmt->biWidth		== 800) &&
			(ifmt->biHeight		== 600) )
		{
			ofmt_mode=TKGDI_SCRMODE_800x600_CC;	//Use 100x75 color cell
//			ofmt_mode=TKGDI_SCRMODE_800x600_PAL8B;	//Use 800x600 8bpp
		}
	}
	
	return(ofmt_mode);
}

TKGSTATUS TKGDI_QueryCreateDisplay(
	_tkgdi_context_t *ctx,
	TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt)
{
	int ofmt_mode;

	if(ifmt && ofmt)
	{
		return(-1);
	}

	ofmt_mode=tkgdi_vid_scrmode;

	if(ifmt)
	{
		ofmt_mode=TKGDI_ModeForInputFormat(ifmt);
	}

	if(ofmt)
	{
		ofmt->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
		ofmt->biWidth=640;
		ofmt->biHeight=200;
		ofmt->biBitCount=16;
		ofmt->biPlanes=1;
		ofmt->biCompression=0;
		ofmt->biSizeImage=ofmt->biWidth*ofmt->biHeight*2;
		ofmt->biXPelsPerMeter=72;
		ofmt->biYPelsPerMeter=72;
		ofmt->biClrUsed=0;
		ofmt->biClrImportant=0;

		if(ofmt_mode<0)
			return(0);

		if(ofmt_mode==TKGDI_SCRMODE_TEXT)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			ofmt->biWidth=320;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_RGB555)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x480_PAL8B)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=480;
			ofmt->biBitCount=8;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_PAL8B)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
			ofmt->biBitCount=8;
		}

		if(ofmt_mode==TKGDI_SCRMODE_800x600_PAL8B)
		{
			ofmt->biWidth=800;
			ofmt->biHeight=600;
			ofmt->biBitCount=8;
		}

		ofmt->biSizeImage=
			ofmt->biWidth*
			ofmt->biHeight*
			((ofmt->biBitCount+7)>>3);

		return(1);
	}
	
	return(0);
}

TKGSTATUS TKGDI_TryMapDisplayBuffer(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info,
	void **rfbuf)
{
	void *ptr;

	if(dev!=1)
	{
		*rfbuf=NULL;
		return(TKGDI_STATUS_FAIL);
	}
	
	if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555_LFB)
	{
		*rfbuf=NULL;

		if((info->biWidth!=320) || (info->biHeight!=(-200)))
		{
			tk_dbg_printf("TKGDI_TryMapDisplayBuffer: "
				"Fail, Resolution, %d x %d\n",
				info->biWidth, info->biHeight);
			return(TKGDI_STATUS_FAIL);
		}

		if((info->biBitCount!=15) && (info->biBitCount!=16))
		{
			tk_dbg_printf("TKGDI_TryMapDisplayBuffer: Fail, Bit Count\n");
			return(TKGDI_STATUS_FAIL);
		}
		
		if(tkgdi_blitupdate_getconbuf_sticky_cnt>0)
		{
			ptr=TKGDI_BlitUpdate_GetConbuf();
			ptr=TKGDI_BlitUpdate_GetConbuf();
		}
		
		ptr=TKGDI_BlitUpdate_GetConbufFast();

		if(!ptr)
			return(TKGDI_STATUS_FAIL);
		
//		if(((((long)ptr)>>44)&15)==15)
//		{
//			tk_dbg_printf("TKGDI_TryMapDisplayBuffer: Fail, MMIO\n");
//			return(TKGDI_STATUS_FAIL);
//		}

		*rfbuf=ptr;
		return(TKGDI_STATUS_OK);
	}

	*rfbuf=NULL;
	return(TKGDI_STATUS_FAIL);
}

TKGSTATUS TKGDI_UnmapDisplayBuffer(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *ifmt,
	void **rfbuf)
{
	if(dev!=1)
	{
		return(TKGDI_STATUS_FAIL);
	}

	return(TKGDI_STATUS_OK);
}

TKGSTATUS TKGDI_MapFlipFrame(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
	if(dev!=1)
	{
		return(TKGDI_STATUS_FAIL);
	}
	
	TKGDI_BlitUpdate_FlipConbuf();
	return(TKGDI_STATUS_OK);
}

u32 tkgdi_eventseq;

TKGSTATUS TKGDI_DevPushEvent(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_EVENT *imsg)
{
	TKGDI_EVENTBUF *tmsg, *tmcur;
	_tkgdi_window_t *wctx;
	int didx;
	int i, j, k;

	if(dev<=0)
		return(0);

	didx=dev&0xFFFFF;

//	if(dev>1)
	if(didx>1)
	{
//		i=dev;
		i=didx;

		wctx=tkgdi_window_vis[i];
		if(!wctx)
			return(0);

		if(	(wctx->magic0!=TKGDI_CTX_MAGIC) ||
			(wctx->magic1!=TKGDI_CTX_MAGIC) )
		{
			__debugbreak();
		}

		tmsg=TKGDI_AllocEventBuf();
		tmsg->ev=*imsg;
		
		tmsg->next=wctx->msgqueue;
		wctx->msgqueue=tmsg;

//		tk_dbg_printf("TKGDI_DevPushEvent: Reject Dev=%04X EV %08X %02X\n",
//			dev, imsg->fccMsg, imsg->wParm1);
		return(0);
	}

//	tk_dbg_printf("TKGDI_DevPushEvent: Send EV %08X %02X\n",
//		imsg->fccMsg, imsg->wParm1);
		
	if(!imsg->evSeqNum)
	{
		k=tkgdi_eventseq++;
		if(!k)
			k=tkgdi_eventseq++;
		imsg->evSeqNum=k;
	}

	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		wctx=tkgdi_window_vis[i];

		if(	(wctx->magic0!=TKGDI_CTX_MAGIC) ||
			(wctx->magic1!=TKGDI_CTX_MAGIC) )
		{
			__debugbreak();
		}

		if(imsg->fccMsg==TKGDI_FCC_keyb)
		{
			/* Keyboard events only go to the window with input focus. */
			if(i!=(tkgdi_n_window_vis-1))
				continue;
		}

		tmsg=TKGDI_AllocEventBuf();

		if(	(wctx->magic0!=TKGDI_CTX_MAGIC) ||
			(wctx->magic1!=TKGDI_CTX_MAGIC) )
		{
			__debugbreak();
		}

//		tmsg->ev=*imsg;
		memcpy((void *)(&(tmsg->ev)), (void *)imsg, sizeof(TKGDI_EVENT));
		tmsg->next=NULL;
		
//		tmsg->next=wctx->msgqueue;
//		wctx->msgqueue=tmsg;

		if(	(wctx->magic0!=TKGDI_CTX_MAGIC) ||
			(wctx->magic1!=TKGDI_CTX_MAGIC) )
		{
			__debugbreak();
		}

		tmcur=wctx->msgqueue;
		if(tmcur)
		{
			k=0;
			while(tmcur->next)
				{ tmcur=tmcur->next; k++; }
			
			if(k>99)
			{
				/* Too many events backlogged... */
				TKGDI_FreeEventBuf(tmsg);
			}else
			{
				tmcur->next=tmsg;
			}
		}else
		{
			wctx->msgqueue=tmsg;
		}
		
		if(wctx->con && wctx->con->conrowmask)
		{
//			tkgdi_con_redrawbuffer(wctx->con);
		}
	}
	return(0);
}

TKGSTATUS TKGDI_DevPollEvent(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_EVENT *imsg)
{
	_tkgdi_window_t *wctx;
	TKGDI_EVENTBUF *tmsg;
	TKGDI_RECT *rect;
	int didx, dtab;

	if(dev<1)
	{
		return(-1);
	}

	didx=dev&0xFFFFF;
	if(didx<1)
	{
		return(-1);
	}
	
//	wctx=tkgdi_windows[dev];
	wctx=tkgdi_windows[didx];
	if(!wctx)
	{
		return(-1);
	}

	dtab=(dev>>20)&15;
	if(dtab!=wctx->tabactive)
	{
		imsg->dev=0;
		imsg->fccMsg=0;
		return(0);
	}

	tmsg=wctx->msgqueue;
	if(!tmsg)
	{
		imsg->dev=0;
		imsg->fccMsg=0;
		return(0);
	}
	
//	*imsg=tmsg->ev;
	memcpy((void *)imsg, (void *)(&(tmsg->ev)), sizeof(TKGDI_EVENT));

//	tk_dbg_printf("TKGDI_DevPollEvent: Saw EV %08X %02X\n",
//		imsg->fccMsg, imsg->wParm1);

	wctx->msgqueue=tmsg->next;
	TKGDI_FreeEventBuf(tmsg);
	
	return(1);
}

TKGSTATUS TKGDI_QueryDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	int i;

	if(parm==TKGDI_FCC_crea)
	{
		return(TKGDI_QueryCreateDisplay(ctx, ifmt, ofmt));
	}

	if(parm==TKGDI_FCC_mapf)
	{
		return(TKGDI_TryMapDisplayBuffer(ctx, dev, ifmt, ofmt));
	}
	
	if(parm==TKGDI_FCC_umap)
	{
		return(TKGDI_UnmapDisplayBuffer(ctx, dev, ifmt, ofmt));
	}
	
	if(parm==TKGDI_FCC_flip)
	{
		return(TKGDI_MapFlipFrame(ctx, dev));
	}

	if(parm==TKGDI_FCC_poll)
	{
		if(ifmt)
		{
			i=TKGDI_DevPushEvent(ctx, dev, ifmt);
			return(i);
		}

		if(ofmt)
		{
			i=TKGDI_DevPollEvent(ctx, dev, ofmt);
			return(i);
		}
		
		return(0);
	}
	
	tk_dbg_printf("TKGDI_QueryDisplay: Fail, Bad Request\n");
	return(-1);
}

TKGHDC TKGDI_CreateDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_window_t *wctx;
	int tgt_mode, domodeset;
	int xs, ys, bxs, bys, cxs, cys;
	int i, j, k;

	tk_dbg_printf("TKGDI_CreateDisplay: A\n");

	if((tkgdi_vid_scrmode<0) || (tkgdi_vid_scrmode>16))
		tkgdi_vid_scrmode=0;

	tgt_mode=TKGDI_ModeForInputFormat(info);
	
	domodeset=0;
	if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
	{
		if(tgt_mode>0)
		{
			domodeset=1;
		}
	}
	
	if(tgt_mode==TKGDI_SCRMODE_TEXT)
	{
		if(tkgdi_vid_scrmode!=TKGDI_SCRMODE_TEXT)
		{
			domodeset=1;
		}
	}

	if((dev<=0) && domodeset)
	{
		/* Fullscreen modes. */

		tkgdi_blitupdate_getconbuf_sticky=0;
		tkgdi_blitupdate_getconbuf_sticky_cnt=64;

		if(tgt_mode==TKGDI_SCRMODE_TEXT)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0081;	//80x25 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_enable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0085;	//80x50 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			tkgdi_vid_scrmode=tgt_mode;

			((u32 *)0xFFFFF00BFF00UL)[0]=0x008F;	//100x75 color-cell
			tkgdi_vid_cellstride=4;
			tkgdi_vid_rowstride=100*4;

//			((u32 *)0xFFFFF00BFF00UL)[0]=0x008D;	//100x75 color-cell
//			tkgdi_vid_cellstride=8;
//			tkgdi_vid_rowstride=100*8;

			tkgdi_vid_xsize=800;
			tkgdi_vid_ysize=600;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0095;	//320x200x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=320;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_RGB555)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0095;	//640x400x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=2;
			tkgdi_vid_rowstride=160*2;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x480_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0297;	//640x480x8bpp
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC297;	//640x480x8bpp
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=160*1;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=480;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0097;	//640x400x8bpp
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC097;	//640x400x8bpp
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=160*1;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_800x600_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D009F;	//200x150 Pal8
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC09F;	//200x150 Pal8
			((u32 *)0xFFFFF00BFF00UL)[0]=0x002DC09F;	//200x150 Pal8
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=200*1;
			tkgdi_vid_xsize=800;
			tkgdi_vid_ysize=600;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_320x200_RGB555_LFB)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x08000495;	//320x200x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=2;
			tkgdi_vid_rowstride=80;
			tkgdi_vid_xsize=320;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tkgdi_vid_bxs=(tkgdi_vid_xsize+7)>>3;
			tkgdi_vid_bys=(tkgdi_vid_ysize+7)>>3;
			tk_con_disable();
			return(1);
		}

		tk_dbg_printf("TKGDI_CreateDisplay: Invalid Mode\n");
		return(TKGHDC_NULL);
	}

	if(dev<=1)
	{
		if(info->biCompression==TKGDI_FCC_text)
		{
			wctx=TKGDI_AllocNewConsoleWindow(ctx);
		}else
		{
			wctx=TKGDI_AllocNewWindow(ctx);
		}
		
		xs=info->biWidth;
		ys=info->biHeight;

		if(xs<0)
			xs=-xs;
		if(ys<0)
			ys=-ys;

		if(info->biCompression==TKGDI_FCC_text)
		{
			if((xs>0) && (ys)>0)
			{
				cxs=xs/80;
				cys=ys/25;
				if(cxs<4)	cxs=4;
				if(cys<6)	cys=6;
				if(cxs>8)	cxs=8;
				if(cys>8)	cys=8;
				
				if(cxs==5)
					cxs=4;
				if(cxs==7)
					cxs=6;

				if(cxs==4)
					cys=6;
				if(cxs==6)
					cys=8;
				if(cxs==8)
					cys=8;
				
				xs=cxs*80;
				ys=cys*25;
				
				wctx->con->cell_xs=cxs;
				wctx->con->cell_ys=cys;
				wctx->cell_xs=cxs;
				wctx->cell_ys=cys;
			}else
			{
				cxs=wctx->con->cell_xs;
				cys=wctx->con->cell_ys;
				xs=cxs*80;
				ys=cys*25;
				wctx->cell_xs=cxs;
				wctx->cell_ys=cys;
			}
		}

		bxs=(xs+3)>>2;
		bys=(ys+3)>>2;
		
		wctx->buf_info->biWidth=xs;
		wctx->buf_info->biHeight=ys;
		wctx->buf_info->biBitCount=16;
		
		wctx->size_bxs=bxs;
		wctx->size_bys=bys;
		wctx->size_x=bxs<<2;
		wctx->size_y=bys<<2;
		
		tk_dbg_printf("  xs=%d ys=%d\n", xs, ys);
		tk_dbg_printf("  bxs=%d bys=%d\n", bxs, bys);
		
		wctx->size_bmxs=(bxs+7)>>3;
		wctx->size_bmsz=wctx->size_bmxs*bys;
		
		wctx->buf_data=tk_malloc(xs*(ys+8)*2);
		wctx->buf_utx2=tk_malloc(bxs*(bys+2)*8);
//		wctx->buf_dirty1=tk_malloc(wctx->size_bmsz+512);
//		wctx->buf_dirty2=tk_malloc(wctx->size_bmsz+512);

		wctx->buf_dirty1=tk_malloc(wctx->size_bmsz+1024);
		wctx->buf_dirty2=tk_malloc(wctx->size_bmsz+1024);
		wctx->buf_dirty1+=512;
		wctx->buf_dirty2+=512;

		memset(wctx->buf_data, 0x55, xs*ys*2);
		memset(wctx->buf_dirty1, 0xFF, wctx->size_bmsz);
		memset(wctx->buf_dirty2, 0xFF, wctx->size_bmsz);
		
		wctx->dirty1=1;
		wctx->dirty2=0;

		if(tkgdi_comglue_curtask)
			wctx->owner_pid=tkgdi_comglue_curtask->pid;

		if(wctx->con)
			wctx->con->pixbuf=wctx->buf_data;

		i=tkgdi_n_window_vis++;
		tkgdi_window_vis[i]=wctx;

		if(wctx->con)
			tkgdi_con_redrawbuffer(wctx->con);

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();
		
		return(wctx->idx);
	}
}

static u16 tkgdi_rgbi2rgb555[16]={
	0x0000, /* 0, 0.000-00.00-000.0-0000 */
	0x0015, /* 1, 0.000-00.00-000.1-0101 */
	0x02A0, /* 2, 0.000-00.10-101.0-0000 */
	0x02B5, /* 3, 0.000-00.10-101.1-0101 */
	0x5400, /* 4, 0.101-01.00-000.0-0000 */
	0x5415, /* 5, 0.101-01.00-000.1-0101 */
	0x56A0, /* 6, 0.101-01.10-101.0-0000 */
	0x56B5, /* 7, 0.101-01.10-101.1-0101 */
	0x294A, /* 8, 0.010-10.01-010.0-1010 */
	0x295F, /* 9, 0.010-10.01-010.1-1111 */
	0x2BEA, /* A, 0.010-10.11-111.0-1010 */
	0x2BFF, /* B, 0.010-10.11-111.1-1111 */
	0x7D4A, /* C, 0.111-11.01-010.0-1010 */
	0x7D5F, /* D, 0.111-11.01-010.1-1111 */
	0x7FEA, /* E, 0.111-11.11-111.0-1010 */
	0x7FFF  /* F, 0.111-11.11-111.1-1111 */
};

int TKGDI_DrawCharBasic(int xo, int yo, int ch, long long mode)
{
	u32 *conbufa;
	u64 pix;
	u32 cxa;
	int xi, yi, ci;

	conbufa=(u32 *)0xFFFFF00A0000ULL;

	if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT		)	||
		(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC	)	)
	{
		if(((mode&255)==8) && !(xo&7) && !(yo&7))
		{
			xi=xo>>3;
			yi=yo>>3;
			ci=(yi*80+xi)*8;
		
			pix=TK_Con_GlyphForCodepoint(ch);
			cxa=0x80000000ULL |
				(tkgdi_rgbi2rgb555[(mode>>12)&15]<<15) |
				(tkgdi_rgbi2rgb555[(mode>> 8)&15]<< 0) ;
			conbufa[ci+0]=cxa;
			conbufa[ci+1]=0;
			conbufa[ci+2]=pix;
			conbufa[ci+3]=pix>>32;
			
			return(0);
		}
	}

	return(-1);
}

TKGSTATUS TKGDI_DrawString(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
	_tkgdi_window_t *wctx;
	_tkgdi_conparm	*con;
	TKGDI_RECT *rect;

	char *s;
	int ocx, ocy, cxs, cys;
	int cx, cy, sz, ch;
	int dcy, dcxm, dcxn;
	int didx, dtab;
	
	didx=dev&0xFFFF;
	dtab=(dev>>20)&15;

	if(didx>1)
	{
		wctx=tkgdi_windows[didx];
		if(!wctx)
		{
			return(-1);
		}
		
		con=wctx->contab[dtab];
		if(!con)
		{
			con=tk_malloc(sizeof(_tkgdi_conparm));
			memset(con, 0, sizeof(_tkgdi_conparm));
			tkgdi_con_init(con);
			wctx->contab[dtab]=con;
			con->cell_xs=wctx->cell_xs;
			con->cell_ys=wctx->cell_ys;
		}
		
		if(con)
		{
			ocx=con->x;
			ocy=con->y;

			cxs=con->cell_xs;
			cys=con->cell_ys;

			if((xo_dev>=0) && (yo_dev>=0))
			{
				con->x=xo_dev;
				con->y=yo_dev;
			}
			
			dcy=-1;
			dcxm=0;
			dcxn=0;

			s=text;
			while(*s)
			{
				ch=*s++;
				cx=con->x;
				cy=con->y;
				tkgdi_con_putc(con, ch);

				if(dtab==wctx->tabactive)
				{
#if 0
					TKGDI_WindowMarkDirtyRect(ctx, wctx,
						cx*cxs, cy*cys, cxs, cys);
					
					if(wctx->con->dirty&2)
					{
						TKGDI_WindowMarkDirtyRect(ctx, wctx,
							0, 0, wctx->size_x, wctx->size_y);
					}
#endif

					if(cy!=dcy)
					{
						if(dcy>=0)
						{
							TKGDI_WindowMarkDirtyRect(ctx, wctx,
								dcxm*cxs, dcy*cys, ((dcxn-dcxm)+1)*cxs, cys);
							
							if(wctx->con->dirty&2)
							{
								TKGDI_WindowMarkDirtyRect(ctx, wctx,
									0, 0, wctx->size_x, wctx->size_y);
							}
						}
						dcy=cy;
						dcxm=cx;
						dcxn=cx;
					}else
					{
						if(cx<dcxm)
							dcxm=cx;
						if(cx>dcxn)
							dcxn=cx;
					}
				}
			}

			if(dcy>=0)
			{
				TKGDI_WindowMarkDirtyRect(ctx, wctx,
					dcxm*cxs, dcy*cys, ((dcxn-dcxm)+1)*cxs, cys);
				
				if(wctx->con->dirty&2)
				{
					TKGDI_WindowMarkDirtyRect(ctx, wctx,
						0, 0, wctx->size_x, wctx->size_y);
				}
			}

			if((xo_dev>=0) && (yo_dev>=0))
			{
				con->x=ocx;
				con->y=ocy;
			}

			if(dtab==wctx->tabactive)
			{
				tkgdi_con_redrawbuffer(con);
				TKGDI_UpdateWindowCells(wctx);
			}
			
			return(0);
		}

		return(-1);
	}

	cx=xo_dev;
	cy=yo_dev;
	sz=mode&255;
	s=text;
	
	while(*s)
	{
		ch=*s++;
		
		if(ch=='\r')
		{
			cx=xo_dev;
			continue;
		}
		if(ch=='\n')
		{
			cx=xo_dev;
			cy+=sz;
			continue;
		}
		
		TKGDI_DrawCharBasic(cx, cy, ch, mode);
	}
	return(0);
}

TKGSTATUS TKGDI_DestroyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
	_tkgdi_window_t *wctx;
	TKGDI_RECT *rect;
	int i, j, k;

	if(dev<2)
		return(0);

	wctx=tkgdi_windows[dev];
	if(!wctx)
		return(0);

	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		if(tkgdi_window_vis[i]==wctx)
		{
			for(j=i; j<tkgdi_n_window_vis; j++)
				{ tkgdi_window_vis[j]=tkgdi_window_vis[j+1]; }
			tkgdi_n_window_vis--;
			break;
		}
	}
	
	tkgdi_windows[dev]=NULL;

	if(wctx->buf_data)
		tk_free(wctx->buf_data);
	if(wctx->buf_utx2)
		tk_free(wctx->buf_utx2);
	if(wctx->buf_dirty1)
		tk_free(wctx->buf_dirty1);
	if(wctx->buf_dirty2)
		tk_free(wctx->buf_dirty2);
	tk_free(wctx);

	TKGDI_ScreenMarkDirty();

	return(0);
}

TKGSTATUS TKGDI_ModifyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	_tkgdi_window_t *wctx;
	TKGDI_FONT_COMMAND *fcmd;
	TKGDI_FONT_COMMAND *frsp;
	TKGDI_RECT *rect;

	wctx=tkgdi_windows[dev];
	if(!wctx)
	{
		return(-1);
	}

	if(parm==TKGDI_FCC_move)
	{
		TKGDI_ScreenMarkDirty();

		rect=ifmt;
		wctx->base_x=rect->left;
		wctx->base_y=rect->top;
		return(1);
	}

	if(parm==TKGDI_FCC_text)
	{
		TKGDI_ScreenMarkDirty();

		if(ifmt)
		{
			wctx->title=strdup(ifmt);
		}else
		{
			wctx->title=NULL;
		}

//		rect=ifmt;
//		wctx->base_x=rect->left;
//		wctx->base_y=rect->top;
		return(1);
	}

	if(parm==TKGDI_FCC_styl)
	{
		TKGDI_ScreenMarkDirty();
		wctx->style=*(u64 *)(ifmt);
		return(1);
	}

	if(parm==TKGDI_FCC_fcmd)
	{
		fcmd=ifmt;
		frsp=ofmt;

		if(fcmd->fcCmd==1)
		{
			frsp->fcFont=1;
			return(1);
		}
		
		if(fcmd->fcCmd==2)
		{
			TKGDI_FetchUnifontSdfBits(
				((byte *)ofmt)+fcmd->sPath,
				fcmd->fcSizeX, fcmd->fcSizeY,
				fcmd->fcFont, fcmd->fcChar);
			return(1);
		}
		
		return(0);
	}

	return(0);
}


void	TKGDI_Snd_SetupOutputMode(int chan, int rate, int bits);
void	TKGDI_Snd_WriteModeSamples2(void *mixbuf, int nsamp, int nsamp2);
void	TKGDI_Snd_Submit(void);

int TK_Midi_ProbeDelayOff(void);

TKGHSND TKGDI_CreateAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHDC dev, TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
	TKGDI_Snd_SetupOutputMode(
		info->nChannels,
		info->nSamplesPerSec,
		info->wBitsPerSample);
	return(1);
}

TKGSTATUS TKGDI_DestroyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev)
{
	return(0);
}

int TK_Midi_SetFmRegisterData(int prg, int idx, u32 val);

TKGSTATUS TKGDI_ModifyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	TKGDI_MIDI_COMMAND *mcmd;
	TKGDI_MIDI_PATCHINFO *mpat;
	byte *mdat;
	int op, ch, d0, d1;

	if(parm==TKGDI_FCC_mcmd)
	{
		TK_Midi_Init();
	
		mcmd=ifmt;
		
		op=mcmd->op;
		ch=mcmd->ch;
		d0=mcmd->d0;
		d1=mcmd->d1;

		switch(mcmd->op)
		{
		case 0:
			TK_Midi_NoteOff(ch, d0, d1);
			break;
		case 1:
			TK_Midi_NoteOn(ch, d0, d1);
			break;
		case 2:
//			TK_Midi_PitchBlend(ch, d0<<6);
			TK_Midi_PitchBlend(ch, d0<<7);
			break;
		case 3:
			TK_Midi_Controller(ch,  d0, d1);
			break;
		case 4:
			TK_Midi_ProgramChange(ch,  d0);
			break;
			
		case 16:
			TK_Midi_SetFmRegisterData(d0, d1, mcmd->u0);
			break;
		case 17:
			if(d0==0)
			{
				TK_Midi_SilenceAll();
				break;
			}
			TK_Midi_SetMasterParam(d0, d1);
			break;

		default:
			break;
		}
	}
	
	if(parm==TKGDI_FCC_mpat)
	{
		mpat=ifmt;
		mdat=((byte *)ifmt)+mpat->cbSize;
		TK_Midi_UploadPatch(mpat, mdat);
	}

	return(0);
}

TKGSTATUS TKGDI_QueryAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *info,
	void *rinfo)
{
	return(0);
}

TKGSTATUS TKGDI_WriteAudioSamples(
	_tkgdi_context_t *ctx,
	TKGHSND dev, void *buffer, int cnt, int ovcnt)
{
	if(dev==1)
	{
		TK_Midi_ProbeDelayOff();
	
		if(ovcnt>0)
			TKGDI_Snd_WriteModeSamples2(buffer, cnt, ovcnt);
		TKGDI_Snd_Submit();
	}
	return(0);
}


void TKGDI_HalCleanupForTask(TKPE_TaskInfo *task)
{
	_tkgdi_window_t *wctx;
	int i, j, k;

//	if(dev<2)
//		return(0);

//	wctx=tkgdi_windows[dev];
//	if(!wctx)
//		return(0);

	for(i=2; i<tkgdi_n_windows; i++)
	{
		wctx=tkgdi_windows[i];
		if(!wctx)
			continue;
		if(wctx->owner_pid==task->pid)
		{
			TKGDI_DestroyDisplay(NULL, i);
		}
	}
	
	TK_Midi_SilenceAll();
	TKGDI_Snd_SilenceAll();

}

const _tkgdi_context_vtable_t tkgdi_context_vtable_vt = {
NULL,
NULL,

(void *)0x12345678,

TKGDI_BlitSubImageOld,
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

TKGDI_BlitSubImageNew,
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10

(void *)0x12345678

};

// _tkgdi_context_t tkgdi_context_dfl = {
// &tkgdi_context_vtable_vt
// };

_tkgdi_context_t *tkgdi_context_ctx[256];
TKPE_TaskInfo *tkgdi_context_task[256];
int tkgdi_n_contexts;

void *TKGDI_GetHalContext(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	_tkgdi_context_t *ctx;
	int i, j, k;
	
	tk_dbg_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_contexts; i++)
	{
		if(tkgdi_context_task[i]==task)
		{
			return(tkgdi_context_ctx[i]);
		}
	}
	
	ctx=tk_malloc(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
	ctx->vt=&tkgdi_context_vtable_vt;
	
	i=tkgdi_n_contexts++;
	tkgdi_context_task[i]=task;
	tkgdi_context_ctx[i]=ctx;
	
	return(ctx);
}

void tkgdi_comglue_dispatchfcn(void *obj, void *fcn, void *pret, void *args);

#ifndef __BJX2__
void tkgdi_comglue_dispatchfcn(void *obj, void *fcn, void *pret, void *args)
{
}
#endif

#ifdef __BJX2__
__asm {
tkgdi_comglue_dispatchfcn:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 192)
	MOV.Q	R5, (SP, 200)
	MOV.Q	R6, (SP, 208)
	MOV.Q	R7, (SP, 216)
	
	
	MOV		R7, R19
	MOV		R5, R3

	MOV.Q	(R19, 0), R5
	MOV.Q	(R19, 8), R6
	MOV.Q	(R19, 16), R7
	MOV.Q	(R19, 24), R20
	MOV.Q	(R19, 32), R21
	MOV.Q	(R19, 40), R22
	MOV.Q	(R19, 48), R23
	
	JSR		R3

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 208), R18
	NOP
	MOV.Q	R2, (R18)
	NOP
	NOP
	ADD		256, SP
	JMP 	R1
};
#endif

/*

VTable:
  If Vt[2]==0x12345678
    Use daisy-chained object method.
  If Vt[2]==0x789ABCDE
    Use daisy-chained VTable method (Vt[3]).

 */

void TKGDI_ComGlueDispatch(TKPE_TaskInfo *task,
	void *sObj, int idx, void *pret, void *args)
{
	void *obj1, **vt0, **vt, *fn;
	
	vt0=(*(void **)sObj);
	fn=vt0[2];
	vt=vt0[3];
//	if(fn==(void *)0x12345678)
//		vt=NULL;

//	if(vt)
	if(fn==((void *)0x789ABCDE))
	{
		vt=(void *)(((u64)vt)^tkgdi_ptrmangle_key);
		obj1=sObj;
		fn=vt[idx];
	}else if(fn==((void *)0x12345678))
	{
		obj1=((void **)sObj)[1];
		obj1=(void *)(((u64)obj1)^tkgdi_ptrmangle_key);
		vt=(void **)(*(void **)obj1);
		fn=vt[idx];
	}else
	{
		__debugbreak();
	}

	tkgdi_comglue_curtask=task;
	tkgdi_comglue_dispatchfcn(obj1, fn, pret, args);
}


void tkgdi_comglue_wrapcall3(void *obj);
void tkgdi_comglue_wrapcall4(void *obj);
void tkgdi_comglue_wrapcall5(void *obj);
void tkgdi_comglue_wrapcall6(void *obj);
void tkgdi_comglue_wrapcall7(void *obj);
void tkgdi_comglue_wrapcall8(void *obj);
void tkgdi_comglue_wrapcall9(void *obj);
void tkgdi_comglue_wrapcall10(void *obj);
void tkgdi_comglue_wrapcall11(void *obj);
void tkgdi_comglue_wrapcall12(void *obj);
void tkgdi_comglue_wrapcall13(void *obj);
void tkgdi_comglue_wrapcall14(void *obj);
void tkgdi_comglue_wrapcall15(void *obj);
void tkgdi_comglue_wrapcall16(void *obj);
void tkgdi_comglue_wrapcall17(void *obj);
void tkgdi_comglue_wrapcall18(void *obj);
void tkgdi_comglue_wrapcall19(void *obj);
void tkgdi_comglue_wrapcall20(void *obj);
void tkgdi_comglue_wrapcall21(void *obj);
void tkgdi_comglue_wrapcall22(void *obj);
void tkgdi_comglue_wrapcall23(void *obj);
void tkgdi_comglue_wrapcall24(void *obj);
void tkgdi_comglue_wrapcall25(void *obj);
void tkgdi_comglue_wrapcall26(void *obj);
void tkgdi_comglue_wrapcall27(void *obj);
void tkgdi_comglue_wrapcall28(void *obj);
void tkgdi_comglue_wrapcall29(void *obj);
void tkgdi_comglue_wrapcall30(void *obj);
void tkgdi_comglue_wrapcall31(void *obj);

void tkgdi_comglue_rv_wrapcall3(void *obj);
void tkgdi_comglue_rv_wrapcall4(void *obj);
void tkgdi_comglue_rv_wrapcall5(void *obj);
void tkgdi_comglue_rv_wrapcall6(void *obj);
void tkgdi_comglue_rv_wrapcall7(void *obj);
void tkgdi_comglue_rv_wrapcall8(void *obj);
void tkgdi_comglue_rv_wrapcall9(void *obj);
void tkgdi_comglue_rv_wrapcall10(void *obj);
void tkgdi_comglue_rv_wrapcall11(void *obj);
void tkgdi_comglue_rv_wrapcall12(void *obj);
void tkgdi_comglue_rv_wrapcall13(void *obj);
void tkgdi_comglue_rv_wrapcall14(void *obj);
void tkgdi_comglue_rv_wrapcall15(void *obj);
void tkgdi_comglue_rv_wrapcall16(void *obj);
void tkgdi_comglue_rv_wrapcall17(void *obj);
void tkgdi_comglue_rv_wrapcall18(void *obj);
void tkgdi_comglue_rv_wrapcall19(void *obj);
void tkgdi_comglue_rv_wrapcall20(void *obj);
void tkgdi_comglue_rv_wrapcall21(void *obj);
void tkgdi_comglue_rv_wrapcall22(void *obj);
void tkgdi_comglue_rv_wrapcall23(void *obj);
void tkgdi_comglue_rv_wrapcall24(void *obj);
void tkgdi_comglue_rv_wrapcall25(void *obj);
void tkgdi_comglue_rv_wrapcall26(void *obj);
void tkgdi_comglue_rv_wrapcall27(void *obj);
void tkgdi_comglue_rv_wrapcall28(void *obj);
void tkgdi_comglue_rv_wrapcall29(void *obj);
void tkgdi_comglue_rv_wrapcall30(void *obj);
void tkgdi_comglue_rv_wrapcall31(void *obj);

int tk_syscall_utxt(void *sobj, int umsg, void *pptr, void *args);
int tk_syscall_rv_utxt(void *sobj, int umsg, void *pptr, void *args);

#ifndef __BJX2__
void tkgdi_comglue_wrapcall3(void *obj) { }
void tkgdi_comglue_wrapcall4(void *obj) { }
void tkgdi_comglue_wrapcall5(void *obj) { }
void tkgdi_comglue_wrapcall6(void *obj) { }
void tkgdi_comglue_wrapcall7(void *obj) { }
void tkgdi_comglue_wrapcall8(void *obj) { }
void tkgdi_comglue_wrapcall9(void *obj) { }
void tkgdi_comglue_wrapcall10(void *obj) { }
void tkgdi_comglue_wrapcall11(void *obj) { }
void tkgdi_comglue_wrapcall12(void *obj) { }
void tkgdi_comglue_wrapcall13(void *obj) { }
void tkgdi_comglue_wrapcall14(void *obj) { }
void tkgdi_comglue_wrapcall15(void *obj) { }
void tkgdi_comglue_wrapcall16(void *obj) { }
void tkgdi_comglue_wrapcall17(void *obj) { }
void tkgdi_comglue_wrapcall18(void *obj) { }
void tkgdi_comglue_wrapcall19(void *obj) { }
void tkgdi_comglue_wrapcall20(void *obj) { }
void tkgdi_comglue_wrapcall21(void *obj) { }
void tkgdi_comglue_wrapcall22(void *obj) { }
void tkgdi_comglue_wrapcall23(void *obj) { }
void tkgdi_comglue_wrapcall24(void *obj) { }
void tkgdi_comglue_wrapcall25(void *obj) { }
void tkgdi_comglue_wrapcall26(void *obj) { }
void tkgdi_comglue_wrapcall27(void *obj) { }
void tkgdi_comglue_wrapcall28(void *obj) { }
void tkgdi_comglue_wrapcall29(void *obj) { }
void tkgdi_comglue_wrapcall30(void *obj) { }
void tkgdi_comglue_wrapcall31(void *obj) { }

void tkgdi_comglue_rv_wrapcall3(void *obj) { }
void tkgdi_comglue_rv_wrapcall4(void *obj) { }
void tkgdi_comglue_rv_wrapcall5(void *obj) { }
void tkgdi_comglue_rv_wrapcall6(void *obj) { }
void tkgdi_comglue_rv_wrapcall7(void *obj) { }
void tkgdi_comglue_rv_wrapcall8(void *obj) { }
void tkgdi_comglue_rv_wrapcall9(void *obj) { }
void tkgdi_comglue_rv_wrapcall10(void *obj) { }
void tkgdi_comglue_rv_wrapcall11(void *obj) { }
void tkgdi_comglue_rv_wrapcall12(void *obj) { }
void tkgdi_comglue_rv_wrapcall13(void *obj) { }
void tkgdi_comglue_rv_wrapcall14(void *obj) { }
void tkgdi_comglue_rv_wrapcall15(void *obj) { }
void tkgdi_comglue_rv_wrapcall16(void *obj) { }
void tkgdi_comglue_rv_wrapcall17(void *obj) { }
void tkgdi_comglue_rv_wrapcall18(void *obj) { }
void tkgdi_comglue_rv_wrapcall19(void *obj) { }
void tkgdi_comglue_rv_wrapcall20(void *obj) { }
void tkgdi_comglue_rv_wrapcall21(void *obj) { }
void tkgdi_comglue_rv_wrapcall22(void *obj) { }
void tkgdi_comglue_rv_wrapcall23(void *obj) { }
void tkgdi_comglue_rv_wrapcall24(void *obj) { }
void tkgdi_comglue_rv_wrapcall25(void *obj) { }
void tkgdi_comglue_rv_wrapcall26(void *obj) { }
void tkgdi_comglue_rv_wrapcall27(void *obj) { }
void tkgdi_comglue_rv_wrapcall28(void *obj) { }
void tkgdi_comglue_rv_wrapcall29(void *obj) { }
void tkgdi_comglue_rv_wrapcall30(void *obj) { }
void tkgdi_comglue_rv_wrapcall31(void *obj) { }
#endif

#ifdef __BJX2__

__asm {
.section .utext

.balign	16

tk_syscall_utxt:
	nop
	nop
	keybrk_xg2
	mov sp, r1
	mov.q r1, (sp)
	mov lr, r1
	mov.q r1, (sp, 8)

	mov		0, r20
	mov		0, r21
	mov		0, r22
	mov		0, r23

//	bsr		tk_sysc_xorhashregs
//	mov.q	r2, (sp, 16)
//	mov		r1, lr

	nop
	nop
	syscall	0
	nop
	nop
	nop
	nop
	nop
	keybrk_xg2
	nop
	keybrk_xg2
	nop
	nop
	mov.q (sp), r1
	cmpqeq r1, sp
	break?f
	mov lr, r16
	mov.q (sp, 8), r1
	cmpqeq r1, r16
	break?f

#if 0
	mov		r2, r18
	bsr		tk_sysc_xorhashregs
	mov.q	(sp, 16), r3

	cmpqeq	r2, r3
	break?f

	mov		r1, lr
	mov		r18, r2
#endif

	nop
	nop
	nop
	nop
	mov lr, r18
	mov.q (r18), r19
	nop
	nop
	
	rts
	nop
	nop
	nop
	nop
	brk
	brk

.balign 4

.fix32

tkgdi_comglue_wrapcall_gen:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 128)
	MOV.Q	R5, (SP, 136)
	MOV.Q	R6, (SP, 144)
	MOV.Q	R7, (SP, 152)
	MOV.Q	R20, (SP, 160)
	MOV.Q	R21, (SP, 168)
	MOV.Q	R22, (SP, 176)
	MOV.Q	R23, (SP, 184)
	
	MOV		R3, R5
	LEA.B	(SP, 112), R6
//	LEA.B	(SP, 128), R7
	LEA.B	(SP, 136), R7
	BSR		tk_syscall_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R2
	MOV.Q	(SP, 120), R3
	
	MOV		R1, R4
	MOV.Q	(R4, 0), R5		//Probe Load

	ADD		256, SP
	JMP 	R1

	NOP
	NOP
	NOP
	NOP

tkgdi_comglue_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall20:
	MOV		TK_UMSG_COMGLUE_VMT20, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall21:
	MOV		TK_UMSG_COMGLUE_VMT21, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall22:
	MOV		TK_UMSG_COMGLUE_VMT22, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall23:
	MOV		TK_UMSG_COMGLUE_VMT23, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall24:
	MOV		TK_UMSG_COMGLUE_VMT24, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall25:
	MOV		TK_UMSG_COMGLUE_VMT25, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall26:
	MOV		TK_UMSG_COMGLUE_VMT26, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall27:
	MOV		TK_UMSG_COMGLUE_VMT27, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall28:
	MOV		TK_UMSG_COMGLUE_VMT28, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall29:
	MOV		TK_UMSG_COMGLUE_VMT29, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall30:
	MOV		TK_UMSG_COMGLUE_VMT30, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall31:
	MOV		TK_UMSG_COMGLUE_VMT31, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall32:
	MOV		TK_UMSG_COMGLUE_VMT32, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall33:
	MOV		TK_UMSG_COMGLUE_VMT33, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall34:
	MOV		TK_UMSG_COMGLUE_VMT34, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall35:
	MOV		TK_UMSG_COMGLUE_VMT35, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall36:
	MOV		TK_UMSG_COMGLUE_VMT36, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall37:
	MOV		TK_UMSG_COMGLUE_VMT37, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall38:
	MOV		TK_UMSG_COMGLUE_VMT38, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall39:
	MOV		TK_UMSG_COMGLUE_VMT39, R3
	BRA		tkgdi_comglue_wrapcall_gen

	NOP
	NOP
	NOP
	NOP

.endfix32

};

__asm {

.balign 4

#if 1
.riscv
	nop
	nop

tk_syscall_rv_utxt:
	nop
	nop
	nop
	nop

	mov		-1, r17

	nop
	nop
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop
	nop
	nop
	nop

tkgdi_comglue_rv_wrapcall_gen:
	ADD		-256, SP
//	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R10, (SP, 128)
	MOV.Q	R11, (SP, 136)
	MOV.Q	R12, (SP, 144)
	MOV.Q	R13, (SP, 152)
	MOV.Q	R14, (SP, 160)
	MOV.Q	R15, (SP, 168)
	MOV.Q	R16, (SP, 176)
	MOV.Q	R17, (SP, 184)
	
	MOV		R5, R11
	ADD		SP, 112, R12
	ADD		SP, 136, R13
	BSR		tk_syscall_rv_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R10
	MOV.Q	(SP, 120), R11

	MOV		R1, R12
	MOV.Q	(R12, 0), R13		//Probe Load

	ADD		256, SP
	JMP 	R1

	NOP
	NOP
	NOP
	NOP

tkgdi_comglue_rv_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall20:
	MOV		TK_UMSG_COMGLUE_VMT20, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall21:
	MOV		TK_UMSG_COMGLUE_VMT21, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall22:
	MOV		TK_UMSG_COMGLUE_VMT22, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall23:
	MOV		TK_UMSG_COMGLUE_VMT23, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall24:
	MOV		TK_UMSG_COMGLUE_VMT24, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall25:
	MOV		TK_UMSG_COMGLUE_VMT25, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall26:
	MOV		TK_UMSG_COMGLUE_VMT26, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall27:
	MOV		TK_UMSG_COMGLUE_VMT27, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall28:
	MOV		TK_UMSG_COMGLUE_VMT28, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall29:
	MOV		TK_UMSG_COMGLUE_VMT29, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen


	NOP
	NOP
	NOP
	NOP

.endriscv
#endif
};

#endif


const _tkgdi_context_vtable_t tkgdi_context_vtable_gvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_wrapcall3,	//3
tkgdi_comglue_wrapcall4,
tkgdi_comglue_wrapcall5,
tkgdi_comglue_wrapcall6,
tkgdi_comglue_wrapcall7,
tkgdi_comglue_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_wrapcall10,	//10
tkgdi_comglue_wrapcall11,
tkgdi_comglue_wrapcall12,
tkgdi_comglue_wrapcall13,
tkgdi_comglue_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};

const _tkgdi_context_vtable_t tkgdi_context_vtable_grvvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_rv_wrapcall3,	//3
tkgdi_comglue_rv_wrapcall4,
tkgdi_comglue_rv_wrapcall5,
tkgdi_comglue_rv_wrapcall6,
tkgdi_comglue_rv_wrapcall7,
tkgdi_comglue_rv_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_rv_wrapcall10,	//10
tkgdi_comglue_rv_wrapcall11,
tkgdi_comglue_rv_wrapcall12,
tkgdi_comglue_rv_wrapcall13,
tkgdi_comglue_rv_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_rv_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};

void *tkgdi_transvt_orgvt[256];
void *tkgdi_transvt_trgvt_jx[256];
void *tkgdi_transvt_trgvt_rv[256];
int tkgdi_transvt_n_vt;

void *TKGDI_GetWrapVTableForTask(TKPE_TaskInfo *task,
	void *orgvt, int size)
{
	TKPE_TaskInfo *ctask;
	void **vt_o, **vt_jx, **vt_rv;
	void *fn, *ovt1;
	u64 uli;
	int i, j, k, n;

	ctask=TK_GetCurrentTask();

	if(ctask==task)
		return(orgvt);
	
	for(i=0; i<tkgdi_transvt_n_vt; i++)
	{
		if(tkgdi_transvt_orgvt[i]==orgvt)
		{
			if(TK_Task_SyscallTaskIsRiscV(task))
			{
				return(tkgdi_transvt_trgvt_rv[i]);
			}else
			{
				return(tkgdi_transvt_trgvt_jx[i]);
			}
		}
	}
	
	if(!tkgdi_ptrmangle_key)
	{
		tkgdi_ptrmangle_key=TK_GetRandom48ASLR();
	}
	
	ovt1=(void *)(((u64)orgvt)^tkgdi_ptrmangle_key);
	
	i=tkgdi_transvt_n_vt++;
	tkgdi_transvt_orgvt[i]=orgvt;
	
	n=size/sizeof(void *);
	vt_o=orgvt;
	vt_jx=tk_malloc(n*sizeof(void *));
	vt_rv=tk_malloc(n*sizeof(void *));
	
	memset(vt_jx, 0, n*sizeof(void *));
	memset(vt_rv, 0, n*sizeof(void *));

	vt_jx[2]=(void *)0x789ABCDE;
	vt_rv[2]=(void *)0x789ABCDE;
//	vt_jx[3]=orgvt;
//	vt_rv[3]=orgvt;
	vt_jx[3]=ovt1;
	vt_rv[3]=ovt1;

	tkgdi_transvt_trgvt_jx[i]=vt_jx;
	tkgdi_transvt_trgvt_rv[i]=vt_rv;
	
	for(j=4; j<n; j++)
	{
		fn=vt_o[j];
		if(!fn)
		{
			vt_jx[j]=NULL;
			vt_rv[j]=NULL;
			continue;
		}
		
		fn=NULL;
		switch(j)
		{
			case  4: fn=tkgdi_comglue_wrapcall4; break;
			case  5: fn=tkgdi_comglue_wrapcall5; break;
			case  6: fn=tkgdi_comglue_wrapcall6; break;
			case  7: fn=tkgdi_comglue_wrapcall7; break;

			case  8: fn=tkgdi_comglue_wrapcall8; break;
			case  9: fn=tkgdi_comglue_wrapcall9; break;

			case 10: fn=tkgdi_comglue_wrapcall10; break;
			case 11: fn=tkgdi_comglue_wrapcall11; break;
			case 12: fn=tkgdi_comglue_wrapcall12; break;
			case 13: fn=tkgdi_comglue_wrapcall13; break;
			case 14: fn=tkgdi_comglue_wrapcall14; break;
			case 15: fn=tkgdi_comglue_wrapcall15; break;
			case 16: fn=tkgdi_comglue_wrapcall16; break;
			case 17: fn=tkgdi_comglue_wrapcall17; break;
			case 18: fn=tkgdi_comglue_wrapcall18; break;
			case 19: fn=tkgdi_comglue_wrapcall19; break;

			case 20: fn=tkgdi_comglue_wrapcall20; break;
			case 21: fn=tkgdi_comglue_wrapcall21; break;
			case 22: fn=tkgdi_comglue_wrapcall22; break;
			case 23: fn=tkgdi_comglue_wrapcall23; break;
			case 24: fn=tkgdi_comglue_wrapcall24; break;
			case 25: fn=tkgdi_comglue_wrapcall25; break;
			case 26: fn=tkgdi_comglue_wrapcall26; break;
			case 27: fn=tkgdi_comglue_wrapcall27; break;
			case 28: fn=tkgdi_comglue_wrapcall28; break;
			case 29: fn=tkgdi_comglue_wrapcall29; break;
		}
		
		uli=(u64)fn;
		uli&=0x0000FFFFFFFFFFFEULL;
		uli|=0x0000000000000001ULL;
		fn=(void *)uli;
		
		vt_jx[j]=fn;

		fn=NULL;
		switch(j)
		{
			case  4: fn=tkgdi_comglue_rv_wrapcall4; break;
			case  5: fn=tkgdi_comglue_rv_wrapcall5; break;
			case  6: fn=tkgdi_comglue_rv_wrapcall6; break;
			case  7: fn=tkgdi_comglue_rv_wrapcall7; break;

			case  8: fn=tkgdi_comglue_rv_wrapcall8; break;
			case  9: fn=tkgdi_comglue_rv_wrapcall9; break;

			case 10: fn=tkgdi_comglue_rv_wrapcall10; break;
			case 11: fn=tkgdi_comglue_rv_wrapcall11; break;
			case 12: fn=tkgdi_comglue_rv_wrapcall12; break;
			case 13: fn=tkgdi_comglue_rv_wrapcall13; break;
			case 14: fn=tkgdi_comglue_rv_wrapcall14; break;
			case 15: fn=tkgdi_comglue_rv_wrapcall15; break;
			case 16: fn=tkgdi_comglue_rv_wrapcall16; break;
			case 17: fn=tkgdi_comglue_rv_wrapcall17; break;
			case 18: fn=tkgdi_comglue_rv_wrapcall18; break;
			case 19: fn=tkgdi_comglue_rv_wrapcall19; break;

			case 20: fn=tkgdi_comglue_rv_wrapcall20; break;
			case 21: fn=tkgdi_comglue_rv_wrapcall21; break;
			case 22: fn=tkgdi_comglue_rv_wrapcall22; break;
			case 23: fn=tkgdi_comglue_rv_wrapcall23; break;
			case 24: fn=tkgdi_comglue_rv_wrapcall24; break;
			case 25: fn=tkgdi_comglue_rv_wrapcall25; break;
			case 26: fn=tkgdi_comglue_rv_wrapcall26; break;
			case 27: fn=tkgdi_comglue_rv_wrapcall27; break;
			case 28: fn=tkgdi_comglue_rv_wrapcall28; break;
			case 29: fn=tkgdi_comglue_rv_wrapcall29; break;
		}
		
//		uli=(u64)fn;
//		uli&=0x0000FFFFFFFFFFFEULL;
//		uli|=0x0000000000000001ULL;
//		fn=(void *)uli;
		
		vt_rv[j]=fn;
	}

	if(TK_Task_SyscallTaskIsRiscV(task))
	{
		return(tkgdi_transvt_trgvt_rv[i]);
	}else
	{
		return(tkgdi_transvt_trgvt_jx[i]);
	}
}

_tkgdi_context_vtable_t *tkgdi_context_vtable_gvtc;
_tkgdi_context_vtable_t *tkgdi_context_vtable_grvvtc;

_tkgdi_context_t *tkgdi_gcontext_ctx[256];
TKPE_TaskInfo *tkgdi_gcontext_task[256];
int tkgdi_n_gcontexts;

void *TKGDI_GetHalContextComGlue(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	void **ppv, *pv;
	_tkgdi_context_t *ctx;
	_tkgdi_context_t *ctx2;
	TKPE_TaskInfo *ctask;
	u64 lv;
	int i, j, k, n;

	if(((u32)apiname)!=TK_FCC_GDI)
		return(NULL);

	tk_dbg_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_gcontexts; i++)
	{
		if(tkgdi_gcontext_task[i]==task)
		{
			return(tkgdi_gcontext_ctx[i]);
		}
	}

	ctask=TK_GetCurrentTask();

	ctx2=TKGDI_GetHalContext(task, apiname, subname);
	
	if(ctask==task)
		return(ctx2);
	
	if(!tkgdi_context_vtable_gvtc)
	{
		tkgdi_context_vtable_gvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		tkgdi_context_vtable_grvvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_gvtc,
			&tkgdi_context_vtable_gvt,
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_grvvtc,
			&tkgdi_context_vtable_grvvt,
			sizeof(_tkgdi_context_vtable_t));
			
		n=sizeof(_tkgdi_context_vtable_t)/sizeof(void *);
		ppv=(void **)tkgdi_context_vtable_gvtc;
		for(i=0; i<n; i++)
		{
			pv=ppv[i];
			lv=(u64)pv;
			if(!lv)
				continue;
			if(lv==0x12345678)
				continue;

			lv&=0x0000FFFFFFFFFFFEULL;
			lv|=0x0000000000000001ULL;
			ppv[i]=(void *)lv;
		}

		ppv=(void **)tkgdi_context_vtable_grvvtc;
		for(i=0; i<n; i++)
		{
			pv=ppv[i];
			lv=(u64)pv;
			if(!lv)
				continue;
			if(lv==0x12345678)
				continue;

			lv&=0x0000FFFFFFFFFFFEULL;
			lv|=0x0004000000000001ULL;
			ppv[i]=(void *)lv;
		}
	}

	if(!tkgdi_ptrmangle_key)
	{
		tkgdi_ptrmangle_key=TK_GetRandom48ASLR();
	}

	ctx=tk_malloc_usr(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
//	ctx->vt=&tkgdi_context_vtable_gvt;
	ctx->vt=tkgdi_context_vtable_gvtc;
	
	if(TK_Task_SyscallTaskIsRiscV(task))
	{
		ctx->vt=tkgdi_context_vtable_grvvtc;
	}
	
	ctx->sobj=TKMM_LVA_ExportSharedObjectPtr(ctx2);

	ctx->sobj=(void *)(((u64)ctx->sobj)^tkgdi_ptrmangle_key);

	i=tkgdi_n_gcontexts++;
	tkgdi_gcontext_task[i]=task;
	tkgdi_gcontext_ctx[i]=ctx;
	
	return(ctx);
}

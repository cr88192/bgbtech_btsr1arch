/**
 * Get image parameters from a compressed image buffer.
 *
 * The BITMAPINFOHEADER is to be filled with parameters extracted from the
 * compressed image. The cmpfmt argument will identify the image format.
 *
 * This may return the raw format of the image data, rather than a format
 * desirable for a buffer which can be used to hold the extracted pixels.
 */
TKGSTATUS tkgGetImageInfoFromBuffer(
	void *cmpbuf, int sz_cmpbuf, TKGFOURCC cmpfmt,
	TKGDI_BITMAPINFOHEADER *info)
{
	byte *cs;
	int xs, ys;

	if(cmpfmt==TKGDI_FCC_BMP)
	{
		cs=cmpbuf;
		if(*(u16 *)cs==TKGDI_TCC_BM)
		{
			info->biSize			=*(u32 *)(cs+0x0E);
			info->biWidth			=*(u32 *)(cs+0x12);
			info->biHeight			=*(u32 *)(cs+0x16);
			info->biPlanes			=*(u16 *)(cs+0x1A);
			info->biBitCount		=*(u16 *)(cs+0x1C);
			info->biCompression		=*(u32 *)(cs+0x1E);
			info->biSizeImage		=*(u32 *)(cs+0x22);
			info->biXPelsPerMeter	=*(u32 *)(cs+0x26);
			info->biYPelsPerMeter	=*(u32 *)(cs+0x2A);
			info->biClrUsed			=*(u32 *)(cs+0x2E);
			info->biClrImportant	=*(u32 *)(cs+0x32);
			return(TKGDI_STATUS_OK);
		}

		if(*(u32 *)cs==TKGDI_FCC_SpBMP)
		{
			info->biSize			=*(u32 *)(cs+0x10);
			info->biWidth			=*(u32 *)(cs+0x14);
			info->biHeight			=*(u32 *)(cs+0x18);
			info->biPlanes			=*(u16 *)(cs+0x1C);
			info->biBitCount		=*(u16 *)(cs+0x1E);
			info->biCompression		=*(u32 *)(cs+0x20);
			info->biSizeImage		=*(u32 *)(cs+0x24);
			info->biXPelsPerMeter	=*(u32 *)(cs+0x28);
			info->biYPelsPerMeter	=*(u32 *)(cs+0x2C);
			info->biClrUsed			=*(u32 *)(cs+0x30);
			info->biClrImportant	=*(u32 *)(cs+0x34);
			return(TKGDI_STATUS_OK);
		}

		return(TKGDI_STATUS_FAIL);
	}

	if(cmpfmt==TKGDI_FCC_QOI)
	{
		cs=cmpbuf;
		if((*(u16 *)cs)!=TKGDI_TCC_qo)
			return(TKGDI_STATUS_FAIL);
		if(	((*(u16 *)(cs+2))!=TKGDI_TCC_if) &&
			((*(u16 *)(cs+2))!=TKGDI_TCC_li) &&
			((*(u16 *)(cs+2))!=TKGDI_TCC_l4))
			return(TKGDI_STATUS_FAIL);

		xs=ntohl(*(u32 *)(cs+4));
		ys=ntohl(*(u32 *)(cs+8));

		info->biSize			=40;
		info->biWidth			=xs;
		info->biHeight			=-ys;
		info->biPlanes			=1;
		info->biBitCount		=32;
		info->biCompression		=TKGDI_BI_RGB;
		info->biSizeImage		=info->biWidth*info->biHeight*4;
		info->biXPelsPerMeter	=2835;
		info->biYPelsPerMeter	=2835;
		info->biClrUsed			=0;
		info->biClrImportant	=0;

		return(TKGDI_STATUS_OK);
	}

	return(TKGDI_STATUS_FAIL);
}

/**
 * Get image data from a compressed image buffer.
 *
 * The BITMAPINFOHEADER is to be filled with parameters compatible with the
 * compressed image. The cmpfmt argument will identify the image format.
 *
 * The BITMAPINFO header will describe the format of the destination image.
 * If the destination image is NULL, this will return a status for whether the
 * image could be extracted into the requested format.
 */
TKGSTATUS tkgGetImageDataFromBuffer(
	void *cmpbuf, int sz_cmpbuf, TKGFOURCC cmpfmt,
	void *destimage, TKGDI_BITMAPINFOHEADER *info)
{
	TKGDI_BITMAPINFOHEADER t_thdr;
	TKGDI_BITMAPINFOHEADER *thdr;
	cram_ctxinfo t_cramhdr;
	cram_ctxinfo *cramhdr;
	byte *cs, *ics, *tbuf, *dpal, *spal;
	int xs, ys, rt, xs1, ys1;

	if(cmpfmt==TKGDI_FCC_BMP)
	{
		cs=cmpbuf;
		if(	((*(u16 *)cs)!=TKGDI_TCC_BM) &&
			((*(u32 *)cs)!=TKGDI_FCC_SpBMP))
		{
			return(TKGDI_STATUS_FAIL);
		}
		
		thdr=&t_thdr;
		thdr->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
		tkgGetImageInfoFromBuffer(
			cmpbuf, sz_cmpbuf, cmpfmt, thdr);

		if(	(info->biWidth!=thdr->biWidth) ||
			(	(info->biHeight!=thdr->biHeight) &&
				(info->biHeight!=(-thdr->biHeight)))
			)
		{
			return(TKGDI_STATUS_NO);
		}
		
//		dpal=((byte *)info)+40;
		dpal=((byte *)info)+(info->biSize);
		
		if((*(u32 *)cs)==TKGDI_FCC_SpBMP)
		{
			spal=cs + 0x10 + (thdr->biSize);
			ics=cs+(*(u32 *)(cs+0x0C));
		}else
		{
//			spal=cs+0x36;
			spal=cs + 0x0E + (thdr->biSize);
			ics=cs+(*(u32 *)(cs+0x0A));
		}
		
		xs=info->biWidth;
		ys=info->biHeight;
		if(ys<0)ys=-ys;
		if(info->biHeight!=thdr->biHeight)
			ys=-ys;
		
		if(info->biCompression==TKGDI_BI_RGB)
		{
			if(thdr->biCompression==TKGDI_BI_RGB)
			{
				rt=TKGDI_CopyImageBuffer_ConvPal(destimage, ics, xs, ys,
					info->biBitCount, thdr->biBitCount, dpal, spal);
				if(rt<0)
					return(TKGDI_STATUS_NO);
				return(TKGDI_STATUS_YES);
			}

			if((thdr->biCompression==TKGDI_FCC_CRAM) &&
				(thdr->biBitCount>8))
			{
				/* RGB555 CRAM */
			
				cramhdr=&t_cramhdr;
				cramhdr->ihead=thdr;
				cramhdr->ohead=info;

				if((info->biBitCount==32) && (ys>0))
				{
					cram_decompress_inner_bgra(ics, destimage,
						xs*ys*4, xs*ys*4, cramhdr);
					return(TKGDI_STATUS_YES);
				}
				
				if((info->biBitCount==16) && (ys>0))
				{
					cram_decompress_inner_rgb555(ics, destimage,
						xs*ys*4, xs*ys*4, cramhdr);
					return(TKGDI_STATUS_YES);
				}
				
				ys1=ys;
				if(ys<0)
					ys1=-ys;
				tbuf=tk_malloc(xs*ys1*4);
				cram_decompress_inner_bgra(ics, (u32 *)tbuf,
					xs*ys1*4, xs*ys1*4, cramhdr);

				TKGDI_CopyImageBuffer_Conv(destimage, tbuf, xs, ys,
					info->biBitCount, 32);
				tk_free(tbuf);
				return(TKGDI_STATUS_YES);
			}

			if((thdr->biCompression==TKGDI_FCC_CRAM) &&
				(thdr->biBitCount<=8))
			{
				/* Indexed CRAM */
			
				cramhdr=&t_cramhdr;
				cramhdr->ihead=thdr;
				cramhdr->ohead=info;

				if((info->biBitCount==16) && (ys>0))
				{
					cram_decompress_inner8_rgb555(ics, destimage,
						xs*ys*4, xs*ys*4, cramhdr, spal);
					return(TKGDI_STATUS_YES);
				}
				
				ys1=ys;
				if(ys<0)
					ys1=-ys;
				tbuf=tk_malloc(xs*ys1*2);
				cram_decompress_inner8_rgb555(ics, (u16 *)tbuf,
					xs*ys1*4, xs*ys1*4, cramhdr, spal);

				TKGDI_CopyImageBuffer_Conv(destimage, tbuf, xs, ys,
					info->biBitCount, 16);
				tk_free(tbuf);
				return(TKGDI_STATUS_YES);
			}
		}

		return(TKGDI_STATUS_NO);
	}


	if(cmpfmt==TKGDI_FCC_QOI)
	{
		cs=cmpbuf;
		if((*(u16 *)cs)!=TKGDI_TCC_qo)
			return(TKGDI_STATUS_FAIL);
		if(	((*(u16 *)(cs+2))!=TKGDI_TCC_if) &&
			((*(u16 *)(cs+2))!=TKGDI_TCC_li) &&
			((*(u16 *)(cs+2))!=TKGDI_TCC_l4))
			return(TKGDI_STATUS_FAIL);

		xs=ntohl(*(u32 *)(cs+4));
		ys=ntohl(*(u32 *)(cs+8));

		thdr=&t_thdr;
		thdr->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
		tkgGetImageInfoFromBuffer(
			cmpbuf, sz_cmpbuf, cmpfmt, thdr);

		if(	(info->biWidth!=thdr->biWidth) ||
			(	(info->biHeight!=thdr->biHeight) &&
				(info->biHeight!=(-thdr->biHeight)))
			)
		{
			return(TKGDI_STATUS_NO);
		}

//		ics=cs+(*(u32 *)(cs+0x0A));
		
		xs=info->biWidth;
		ys=info->biHeight;
		if(ys<0)ys=-ys;
		if(info->biHeight!=thdr->biHeight)
			ys=-ys;
		if(info->biCompression==TKGDI_BI_RGB)
		{
			if(info->biBitCount==32)
			{
				xs1=xs; ys1=ys;
				TKG_QOILZ_DecImageBufferFlatRGBA32(
					destimage, cmpbuf, &xs1, &ys1);
				return(TKGDI_STATUS_YES);
			}

			if(info->biBitCount==24)
			{
				xs1=xs; ys1=ys;
				TKG_QOILZ_DecImageBufferFlatRGB24(
					destimage, cmpbuf, &xs1, &ys1);
				return(TKGDI_STATUS_YES);
			}

			if(	(info->biBitCount==16) ||
				(info->biBitCount==15))
			{
				xs1=xs; ys1=ys;
				TKG_QOILZ_DecImageBufferFlatRGB555(
					destimage, cmpbuf, &xs1, &ys1);
				return(TKGDI_STATUS_YES);
			}
		}

		return(TKGDI_STATUS_NO);
	}
		
	return(TKGDI_STATUS_NO);
}

TKGSTATUS tkgConvertImageBuffer(
	void *dstimg, void *srcimg,
	TKGDI_BITMAPINFOHEADER *dstinfo,
	TKGDI_BITMAPINFOHEADER *srcinfo)
{
	byte *spal, *dpal;
	int sxs, sys, dxs, dys, xs, ys, dbpp, sbpp;
	int i;

//	spal=((byte *)srcinfo)+40;
//	dpal=((byte *)dstinfo)+40;
	spal=((byte *)srcinfo)+srcinfo->biSize;
	dpal=((byte *)dstinfo)+dstinfo->biSize;
	
	dxs=dstinfo->biWidth;
	dys=dstinfo->biHeight;
	sxs=srcinfo->biWidth;
	sys=srcinfo->biHeight;
	dbpp=dstinfo->biBitCount;
	sbpp=srcinfo->biBitCount;
	xs=dxs;
	ys=dys;
	if(sxs!=dxs)
		return(TKGDI_STATUS_NO);
	if((sys!=dys) && (sys!=(-dys)))
		return(TKGDI_STATUS_NO);

	if(ys<0)
		ys=-ys;
	if(sys==-dys)
		ys=-ys;
	i=TKGDI_CopyImageBuffer_ConvPal(
		dstimg, srcimg,
		xs, ys, dbpp, sbpp, dpal, spal);
	if(i<0)
		return(TKGDI_STATUS_NO);

	return(TKGDI_STATUS_YES);
}

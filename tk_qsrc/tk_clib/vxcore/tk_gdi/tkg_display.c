#include <tkgdi.h>

int tkgdi_vid_frnum;
int tkgdi_vid_scrmode;

void I_FinishUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void I_FinishUpdate_ScanCopy_Flip(u16 *ics, u32 *ict, int blkn);

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkRgb555(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=!sbys;
		flip=!flip;
	}

//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xFFFFF00A0000ULL;
//	conbufb=conbufa+(80*61);

//	conbufb=(u32 *)0x0000080A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=sbuf+(sbyo*sbxs)+sbxo;
	ict=conbufa+((bym*80+bxm)*8);
	if(flip)
	{
		ics+=((dys>>4)-1)*(4*sbxs);
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			I_FinishUpdate_ScanCopy_Flip(ics16, ict, dxs>>2);
			ics-=4*sbxs;
		}else
		{
			I_FinishUpdate_ScanCopy(ics16, ict, dxs>>2);
			ics+=4*sbxs;
		}
		ict+=80*8;
	}
	
	return(0);
}


TKGSTATUS tkgBlitSubImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
	int xs, ys, mxs, mys;
	if(dev<=0)
		return(-1);
		
	if(dev==1)
	{
		xs=xs_src;
		ys=ys_src;
		
		mxs=320-xo_dev;
		mys=200-yo_dev;
		
		if(xs>mxs)
			xs=mxs;
		if(ys>mys)
			ys=mys;
	
		TKGDI_BlitUpdate_BlkRgb555(xo_dev, yo_dev, xs, ys,
			data, xo_src, yo_src, info->biWidth, info->biHeight);
	}
}

TKGSTATUS tkgBlitImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data)
{
	return(tkgBlitSubImage(dev, xo_dev, yo_dev, info, data,
		0, 0, info->biWidth, info->biHeight));
}

TKGSTATUS tkgQueryDisplay(TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt)
{
}

TKGHDC tkgCreateDisplay(TKGDI_BITMAPINFOHEADER *info)
{
}

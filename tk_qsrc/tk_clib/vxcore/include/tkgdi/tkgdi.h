#ifndef __TKGDI_TKGDI_H
#define __TKGDI_TKGDI_H

#include <tk_core.h>
#include <tkgdi/tkgdi_avi.h>

#define TKGHDC_NULL		0

typedef int TKGHDC;		//TKGDI Handle Display Context
typedef int TKGSTATUS;		//TKGDI Error Status

/* Blit an image to Display Device.
 * The info field gives the image format.
 *   Should be compatible with the target.
 *  xo_dev / yo_dev give the offsets on the device.
 */
TKGSTATUS tkgBlitImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data);

/* Blit an image to Display Device.
 * The info field gives the image format.
 *   Should be compatible with the target.
 * xo_dev / yo_dev
 *   Give the offsets on the device.
 * xo_src / yo_src / xs_src / ys_src:
 *   Give the offsets and size within the source image.
 */
TKGSTATUS tkgBlitSubImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src);

/* Query Primary Display Device:
 *   ifmt (if not Null): Requested Format
 *   ofmt (if not Null): Output Format
 * If ifmt is NULL, Fetch the current screen mode.
 * If ofmt is NULL, Return Status based on if ifmt is a valid mode.
 */
TKGSTATUS tkgQueryDisplay(TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt);

/*
 * Create a Display with the requested mode.
 * Returns TKGHDC_NULL if the display can't be created.
 * If running in a GUI, may create a basic window.
 */
TKGHDC tkgCreateDisplay(TKGDI_BITMAPINFOHEADER *info);

#endif

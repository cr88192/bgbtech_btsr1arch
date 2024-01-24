// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	The actual span/column drawing functions.
//	Here find the main potential for optimization,
//	 e.g. inline assembly, different algorithms.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: r_draw.c,v 1.4 1997/02/03 16:47:55 b1 Exp $";


#include "doomdef.h"

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "r_local.h"

// Needs access to LFB (guess what).
#include "v_video.h"

// State.
#include "doomstat.h"

// #include <stdint.h>

// ?
#define MAXWIDTH			1120
#define MAXHEIGHT			832

// status bar height at bottom of screen
#define SBARHEIGHT		32

//
// All drawing to the view buffer is accomplished in this file.
// The other refresh files only know about ccordinates,
//  not the architecture of the frame buffer.
// Conveniently, the frame buffer is a linear one,
//  and we need only the base address,
//  and the total size == width*height*depth/8.,
//


byte*		viewimage;
int		viewwidth;
int		scaledviewwidth;
int		viewheight;
int		viewwindowx;
int		viewwindowy;
dt_scrpix	*ylookup[MAXHEIGHT];
int		columnofs[MAXWIDTH];

u16		*ylookup_zb[MAXHEIGHT];

// Color tables for different players,
//  translate a limited part to another
//  (color ramps used for  suit colors).
//
byte		translations[3][256];


// #ifndef __BGBCC__
#if !defined(__BGBCC__) && !defined(__riscv)

int __int_clamp(int val, int min, int max)
{
	if(val<min)
		return(min);
	if(val>max)
		return(max);
	return(val);
}

int __hint_use_egpr()
{
	return(0);
}

#endif

//
// R_DrawColumn
// Source is the top of the column to scale.
//
global_register		lighttable_t	*dc_colormap;
global_register		int				dc_x;
global_register		int				dc_yl;
global_register		int				dc_yh;
global_register		fixed_t			dc_scale;
global_register		fixed_t			dc_iscale;
global_register		fixed_t			dc_texturemid;
global_register		fixed_t			dc_zdist;
global_register		int				dc_col;

byte			dc_isspr;

// first pixel in a column (possibly virtual)
global_register		byte			*dc_source;

// just for profiling
int			dccount;

u64		r_colmask[32];

int dc_czbuf_yl[512];
int dc_czbuf_yh[512];
fixed_t dc_czbuf_sc[512];

int r_nseenpolyobj;

void R_ClearCZBuf (void)
{
	int i, j, k;

	for(i=0; i<512; i++)
	{
		dc_czbuf_sc[i] = MAXINT;
	}

	if(r_usezbuff)
	{
		for(i=0; i<(SCREENWIDTH*SCREENHEIGHT); i++)
			screens_zbuf[i]=0xFFFF;
	}

	if(!r_nseenpolyobj)
	{
		r_usezbuff=0;
	}
	r_nseenpolyobj=0;
}

void R_CellMarkColumn (void)
{
	u64 m;
	int y, y0, y1;

	m=1LL<<((viewwindowx+dc_x)>>3);
	y1=(viewwindowy+dc_yh)>>3;
	y0=(viewwindowy+dc_yl)>>3;

	for(y=y0; y<=y1; y++)
		r_colmask[y]|=m;
}

//
// A column is a vertical slice/span from a wall texture that,
//  given the DOOM style restrictions on the view orientation,
//  will always have constant z depth.
// Thus a special case loop for very fast rendering can
//  be used. It has also been used with Wolfenstein 3D.
//
#if 0
void R_DrawColumn (void)
{
	int			count;
	dt_scrpix	*dest;
	fixed_t		frac;
	fixed_t		fracstep;

	count = dc_yh - dc_yl;

	// Zero length, column does not exceed a pixel.
	if (count < 0)
		return;

#ifdef RANGECHECK
	if ((unsigned)dc_x >= SCREENWIDTH
	|| dc_yl < 0
	|| dc_yh >= SCREENHEIGHT)
	I_Error ("R_DrawColumn: %i to %i at %i", dc_yl, dc_yh, dc_x);
#endif

//	R_CellMarkColumn();

	// Framebuffer destination address.
	// Use ylookup LUT to avoid multiply with ScreenWidth.
	// Use columnofs LUT for subwindows?
	dest = ylookup[dc_yl] + columnofs[dc_x];

	// Determine scaling,
	//  which is the only mapping to be done.
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	// Inner loop that does the actual texture mapping,
	//  e.g. a DDA-lile scaling.
	// This is as fast as it gets.
	do
	{
		// Re-map color indices from wall texture column
		//  using a lighting/special effects LUT.
		*dest = dc_colormap[dc_source[(frac>>FRACBITS)&127]];

		dest += SCREENWIDTH;
		frac += fracstep;

	} while (count--);
}
#endif


// UNUSED.
// Loop unrolled.
// #if 0
#ifndef __BGBCC__
void R_DrawColumn (void)
{
	int				count;
	register byte*			source;
	register dt_scrpix*		dest;
	register lighttable_t*	colormap;

//	register unsigned		frac;
//	register unsigned		fracstep;
//	unsigned		fracstep2;
//	unsigned		fracstep3;
//	unsigned		fracstep4;

//	register int	frac;
//	register int	fracstep;
	register unsigned	frac;
	register unsigned	fracstep;

	count = dc_yh - dc_yl + 1;

	if(count<=0)
		return;

	source = dc_source;
	if(!source)
		return;

	if(!dc_isspr)
	{
		if(dc_scale==dc_czbuf_sc[dc_x])
		{
			if(dc_yl < dc_czbuf_yl[dc_x])
				dc_czbuf_yl[dc_x] = dc_yl;
			if(dc_yh > dc_czbuf_yh[dc_x])
				dc_czbuf_yh[dc_x] = dc_yh;
		}else
			if(dc_scale<dc_czbuf_sc[dc_x])
		{
			dc_czbuf_yl[dc_x] = dc_yl;
			dc_czbuf_yh[dc_x] = dc_yh;
			dc_czbuf_sc[dc_x] = dc_scale;
		}else
		{
//			return;
		}
	}

//	else
//	{
//		return;
//	}

	colormap = dc_colormap;
	dest = ylookup[dc_yl] + columnofs[dc_x];

	fracstep = dc_iscale<<9;
	frac = (dc_texturemid + (dc_yl-centery)*dc_iscale)<<9;

//	fracstep2 = fracstep+fracstep;
//	fracstep3 = fracstep2+fracstep;
//	fracstep4 = fracstep3+fracstep;

	while (count >= 8)
	{
#if 0
		dest[0            ] = colormap[source[(frac          )>>25]];
		dest[SCREENWIDTH  ] = colormap[source[(frac+fracstep )>>25]];
		dest[SCREENWIDTH*2] = colormap[source[(frac+fracstep2)>>25]];
		dest[SCREENWIDTH*3] = colormap[source[(frac+fracstep3)>>25]];
		frac += fracstep4;

		dest[SCREENWIDTH*4] = colormap[source[(frac          )>>25]];
		dest[SCREENWIDTH*5] = colormap[source[(frac+fracstep )>>25]];
		dest[SCREENWIDTH*6] = colormap[source[(frac+fracstep2)>>25]];
		dest[SCREENWIDTH*7] = colormap[source[(frac+fracstep3)>>25]];
		frac += fracstep4;
#endif

#if 1
		dest[0            ] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH  ] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH*2] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH*3] = colormap[source[frac>>25]]; frac += fracstep;

		dest[SCREENWIDTH*4] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH*5] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH*6] = colormap[source[frac>>25]]; frac += fracstep;
		dest[SCREENWIDTH*7] = colormap[source[frac>>25]]; frac += fracstep;
#endif

		dest += SCREENWIDTH*8;
		count -= 8;
	}

	while (count > 0)
	{
		*dest = colormap[source[frac>>25]];
		dest += SCREENWIDTH;
		frac += fracstep;
		count--;
	}
}
#endif


#if 1
void R_DrawColumn_ZB (void)
{
	int			count;
	dt_scrpix	*dest;
	u16			*dest_zb;
	fixed_t		frac;
	fixed_t		fracstep;
	u16			z;
	int			i;

	count = dc_yh - dc_yl;

	// Zero length, column does not exceed a pixel.
	if (count < 0)
		return;

	dest = ylookup[dc_yl] + columnofs[dc_x];
	dest_zb = ylookup_zb[dc_yl] + columnofs[dc_x];

	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

//	z = dc_scale;
//	z = dc_iscale;
//	z = dc_zdist>>16;

	z = dc_zdist >> 13;
	z = r_int_clamp(z, 0, 65535);
	i = 0;

	do
	{
//		if((z<=(*dest_zb)) || dc_isspr)
		if(z<(*dest_zb))
		{
			*dest = dc_colormap[dc_source[(frac>>FRACBITS)&127]];
			*dest_zb = z;
		}else
		{
			i = -1;
		}
		dest += SCREENWIDTH;
		dest_zb += SCREENWIDTH;
		frac += fracstep;
	} while (count--);
}
#endif

// UNUSED.
// Loop unrolled.
#if 0
void R_DrawColumn_ZB (void)
{
	int				count;
	byte*			source;
	dt_scrpix*		dest;
	u16*			dest_zb;
	lighttable_t*	colormap;

	unsigned		frac;
	unsigned		fracstep;
	unsigned		fracstep2;
	unsigned		fracstep3;
	unsigned		fracstep4;
	u16 			z;

	count = dc_yh - dc_yl + 1;

	if(count<=0)
		return;

	source = dc_source;
	if(!source)
		return;

	colormap = dc_colormap;
	dest = ylookup[dc_yl] + columnofs[dc_x];
	dest_zb = ylookup_zb[dc_yl] + columnofs[dc_x];

	fracstep = dc_iscale<<9;
	frac = (dc_texturemid + (dc_yl-centery)*dc_iscale)<<9;

	fracstep2 = fracstep+fracstep;
	fracstep3 = fracstep2+fracstep;
	fracstep4 = fracstep3+fracstep;

	z = dc_scale;

	while (count >= 8)
	{
		if(z<=dest_zb[SCREENWIDTH*0])
		{
		dest[SCREENWIDTH*0] = colormap[source[(frac          )>>25]];
		dest[SCREENWIDTH*1] = colormap[source[(frac+fracstep )>>25]];
		dest[SCREENWIDTH*2] = colormap[source[(frac+fracstep2)>>25]];
		dest[SCREENWIDTH*3] = colormap[source[(frac+fracstep3)>>25]];

		frac += fracstep4;

		dest[SCREENWIDTH*4] = colormap[source[(frac          )>>25]];
		dest[SCREENWIDTH*5] = colormap[source[(frac+fracstep )>>25]];
		dest[SCREENWIDTH*6] = colormap[source[(frac+fracstep2)>>25]];
		dest[SCREENWIDTH*7] = colormap[source[(frac+fracstep3)>>25]];

		frac += fracstep4;
		dest += SCREENWIDTH*8;
		count -= 8;
	}

	while (count > 0)
	{
		if(z<=(*dest_zb))
		{
			*dest_zb = z;
			*dest = colormap[source[frac>>25]];
		}

		dest += SCREENWIDTH;
		frac += fracstep;
		count--;
	}
}
#endif

// #if 0
#ifndef __BGBCC__
void R_DrawColumnLow (void)
{
	int			count;
	dt_scrpix	*dest;
	dt_scrpix	*dest2;
	fixed_t		frac;
	fixed_t		fracstep;
	int			x1;

	count = dc_yh - dc_yl;

	// Zero length.
	if (count < 0)
		return;

#ifdef RANGECHECK
	if ((unsigned)dc_x >= SCREENWIDTH
	|| dc_yl < 0
	|| dc_yh >= SCREENHEIGHT)
	{

	I_Error ("R_DrawColumn: %i to %i at %i", dc_yl, dc_yh, dc_x);
	}
	//	dccount++;
#endif
	// Blocky mode, need to multiply by 2.
//	dc_x <<= 1;
	x1 = dc_x << 1;

//	R_CellMarkColumn();

//	dest = ylookup[dc_yl] + columnofs[dc_x];
//	dest2 = ylookup[dc_yl] + columnofs[dc_x+1];
	dest = ylookup[dc_yl] + columnofs[x1];
	dest2 = ylookup[dc_yl] + columnofs[x1+1];

	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	do
	{
		// Hack. Does not work corretly.
		*dest2 = *dest = dc_colormap[dc_source[(frac>>FRACBITS)&127]];
		dest += SCREENWIDTH;
		dest2 += SCREENWIDTH;
		frac += fracstep;
	} while (count--);
}
#endif

//
// Spectre/Invisibility.
//
#define FUZZTABLE		50
#define FUZZOFF	(SCREENWIDTH)


int	fuzzoffset[FUZZTABLE] =
{
	FUZZOFF,-FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
	FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
	FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,
	FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,
	FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,
	FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,
	FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF,FUZZOFF,-FUZZOFF,FUZZOFF
};

int	fuzzpos = 0;


//
// Framebuffer postprocessing.
// Creates a fuzzy image by copying pixels
//  from adjacent ones to left and right.
// Used with an all black colormap, this
//  could create the SHADOW effect,
//  i.e. spectres and invisible players.
//
void R_DrawFuzzColumn (void)
{
	static u64	fuzzseed;		//BGB
	u64			fuzzmask;
	int			count;
	dt_scrpix	*dest;
	fixed_t		frac;
	fixed_t		fracstep;
	int			px;
	int			fuzzofs;		//BGB

	// Adjust borders. Low...
	if (!dc_yl)
		dc_yl = 1;

	// .. and high.
	if (dc_yh == viewheight-1)
		dc_yh = viewheight - 2;

	count = dc_yh - dc_yl;

	// Zero length.
	if (count < 0)
		return;


	fuzzmask = fuzzseed >> 16;
	fuzzseed = (fuzzseed + 1) * 65521;
	fuzzmask += fuzzseed;

#ifdef RANGECHECK
	if ((unsigned)dc_x >= SCREENWIDTH
	|| dc_yl < 0 || dc_yh >= SCREENHEIGHT)
	{
	I_Error ("R_DrawFuzzColumn: %i to %i at %i",
		 dc_yl, dc_yh, dc_x);
	}
#endif

//	R_CellMarkColumn();

	// Keep till detailshift bug in blocky mode fixed,
	//  or blocky mode removed.
	/* WATCOM code
	if (detailshift)
	{
	if (dc_x & 1)
	{
		outpw (GC_INDEX,GC_READMAP+(2<<8) );
		outp (SC_INDEX+1,12);
	}
	else
	{
		outpw (GC_INDEX,GC_READMAP);
		outp (SC_INDEX+1,3);
	}
	dest = destview + dc_yl*80 + (dc_x>>1);
	}
	else
	{
	outpw (GC_INDEX,GC_READMAP+((dc_x&3)<<8) );
	outp (SC_INDEX+1,1<<(dc_x&3));
	dest = destview + dc_yl*80 + (dc_x>>2);
	}*/


	// Does not work with blocky mode.
	dest = ylookup[dc_yl] + columnofs[dc_x];

	// Looks familiar.
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	// Looks like an attempt at dithering,
	//  using the colormap #6 (of 0-31, a bit
	//  brighter than average).
	do
	{
		// Lookup framebuffer, and retrieve
		//  a pixel that is either one column
		//  left or right of the current one.
		// Add index from colormap to index.
//		*dest = colormaps[6*256+dest[fuzzoffset[fuzzpos]]];

		//BGB: using dynamic mask is faster than lookup table on BJX2
		fuzzofs = -FUZZOFF;
		if((fuzzmask>>fuzzpos)&1)
			fuzzofs = FUZZOFF;

//		px=dest[fuzzoffset[fuzzpos]];
		px=dest[fuzzofs];
//		px-=1<<10;
//		if(px<0x0210)
//			px=0x0210;

//		px=((px&0x7BDE)>>1) + ((px&0x739C)>>2);
		px= px - ((px&0x739C)>>1);
//		px= px - ((px&0x739C)>>2);
//		px= px - ((px&0x6318)>>3);

//		px-=0x0421;
//		if(px<0)
//			px=0;

		*dest = px;

		// Clamp table lookup index.
		if (++fuzzpos == FUZZTABLE)
			fuzzpos = 0;

		dest += SCREENWIDTH;

		frac += fracstep;
	} while (count--);
}


void R_DrawFuzzColumnLow (void)
{
	static u64	fuzzseed;		//BGB
	u64			fuzzmask;
	int			count;
	dt_scrpix	*dest;
	fixed_t		frac;
	fixed_t		fracstep;
	int			px;
	int			fuzzofs;		//BGB

	// Adjust borders. Low...
	if (!dc_yl)
		dc_yl = 1;

	// .. and high.
	if (dc_yh == viewheight-1)
		dc_yh = viewheight - 2;

	count = dc_yh - dc_yl;

	// Zero length.
	if (count < 0)
		return;


	fuzzmask = fuzzseed >> 16;
	fuzzseed = (fuzzseed + 1) * 65521;
	fuzzmask += fuzzseed;

	// Does not work with blocky mode.
	dest = ylookup[dc_yl] + columnofs[dc_x<<1];

	// Looks familiar.
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	// Looks like an attempt at dithering,
	//  using the colormap #6 (of 0-31, a bit
	//  brighter than average).
	do
	{
		//BGB: using dynamic mask is faster than lookup table on BJX2
		fuzzofs = -FUZZOFF;
		if((fuzzmask>>fuzzpos)&1)
			fuzzofs = FUZZOFF;

//		px=dest[fuzzoffset[fuzzpos]];
		px=dest[fuzzofs];

//		px-=1<<10;
//		if(px<0x0210)
//			px=0x0210;
		px= px - ((px&0x739C)>>1);

		dest[0] = px;
		dest[1] = px;

		// Clamp table lookup index.
		if (++fuzzpos == FUZZTABLE)
			fuzzpos = 0;

		dest += SCREENWIDTH;

		frac += fracstep;
	} while (count--);
}




//
// R_DrawTranslatedColumn
// Used to draw player sprites
//  with the green colorramp mapped to others.
// Could be used with different translation
//  tables, e.g. the lighter colored version
//  of the BaronOfHell, the HellKnight, uses
//  identical sprites, kinda brightened up.
//
byte*	dc_translation;
byte*	translationtables;

void R_DrawTranslatedColumn (void)
{
	int			count;
	dt_scrpix	*dest;
	fixed_t		frac;
	fixed_t		fracstep;

	count = dc_yh - dc_yl;
	if (count < 0)
		return;

#ifdef RANGECHECK
	if ((unsigned)dc_x >= SCREENWIDTH
	|| dc_yl < 0
	|| dc_yh >= SCREENHEIGHT)
	{
	I_Error ( "R_DrawColumn: %i to %i at %i",
		  dc_yl, dc_yh, dc_x);
	}

#endif

//	R_CellMarkColumn();

	// WATCOM VGA specific.
	/* Keep for fixing.
	if (detailshift)
	{
	if (dc_x & 1)
		outp (SC_INDEX+1,12);
	else
		outp (SC_INDEX+1,3);

	dest = destview + dc_yl*80 + (dc_x>>1);
	}
	else
	{
	outp (SC_INDEX+1,1<<(dc_x&3));

	dest = destview + dc_yl*80 + (dc_x>>2);
	}*/


	// FIXME. As above.
	dest = ylookup[dc_yl] + columnofs[dc_x];

	// Looks familiar.
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	// Here we do an additional index re-mapping.
	do
	{
		// Translation tables are used
		//  to map certain colorramps to other ones,
		//  used with PLAY sprites.
		// Thus the "green" ramp of the player 0 sprite
		//  is mapped to gray, red, black/indigo.
		*dest = dc_colormap[dc_translation[dc_source[frac>>FRACBITS]]];
		dest += SCREENWIDTH;

		frac += fracstep;
	} while (count--);
}




//
// R_InitTranslationTables
// Creates the translation tables to map
//  the green color ramp to gray, brown, red.
// Assumes a given structure of the PLAYPAL.
// Could be read from a lump instead.
//
void R_InitTranslationTables (void)
{
	int		i;

	translationtables = Z_Malloc (256*3+255, PU_STATIC, 0);
	translationtables = (byte *)(( (nlint)translationtables + 255 )& ~255);

	// translate just the 16 green colors
	for (i=0 ; i<256 ; i++)
	{
		if (i >= 0x70 && i<= 0x7f)
		{
			// map green ramp to gray, brown, red
			translationtables[i] = 0x60 + (i&0xf);
			translationtables [i+256] = 0x40 + (i&0xf);
			translationtables [i+512] = 0x20 + (i&0xf);
		}
		else
		{
			// Keep all other colors as is.
			translationtables[i] = translationtables[i+256]
			= translationtables[i+512] = i;
		}
	}
}




//
// R_DrawSpan
// With DOOM style restrictions on view orientation,
//  the floors and ceilings consist of horizontal slices
//  or spans with constant z depth.
// However, rotation around the world z axis is possible,
//  thus this mapping, while simpler and faster than
//  perspective correct texture mapping, has to traverse
//  the texture at an angle in all but a few cases.
// In consequence, flats are not stored by column (like walls),
//  and the inner loop has to step in texture space u and v.
//
global_register		int				ds_y;
global_register		int				ds_x1;
global_register		int				ds_x2;
global_register		fixed_t			ds_z;

global_register		lighttable_t	*ds_colormap;

global_register		fixed_t			ds_xfrac;
global_register		fixed_t			ds_yfrac;
global_register		fixed_t			ds_xstep;
global_register		fixed_t			ds_ystep;

// start of a 64*64 tile image
global_register		byte			*ds_source;

global_register		u16				ds_baseluma;
// global_register		u16				ds_luma;
global_register		u64				ds_color;
global_register		u16				dc_baseluma;
// global_register		u16				dc_luma;
global_register		u64				dc_color;

// just for profiling
int			dscount;


void R_CellMarkSpan (void)
{
	u64 m;
	int y, x0, x1;

	y=(viewwindowy+ds_y)>>3;

	x1=(viewwindowx+ds_x2)>>3;
	x0=(viewwindowx+ds_x1)>>3;

	m=1LL<<(x1+1);
	m-=1LL<<x0;

	r_colmask[y]|=m;
}

void R_CellMarkBox (int bx0, int bx1, int by0, int by1)
{
	u64 m;
	int y, y0, y1, x0, x1;

//	printf("R_CellMarkBox: (%d,%d), (%d,%d)\n", bx0, bx1, by0, by1);

	if(bx1<bx0)
		return;
	if(by1<by0)
		return;

	y0=by0>>3;
	y1=by1>>3;

	x0=(bx0)>>3;
	x1=(bx1)>>3;

	m=1LL<<(x1+1);
	m-=1LL<<x0;

	for(y=y0; y<=y1; y++)
		r_colmask[25-y]|=m;
}

//
// Draws the actual span.
#if 0
// #ifndef __BGBCC__
void R_DrawSpan (void)
{
	fixed_t		xfrac;
	fixed_t		yfrac;
	dt_scrpix	*dest;
	int			count;
	int			spot;

#ifdef RANGECHECK
	if (ds_x2 < ds_x1
	|| ds_x1<0
	|| ds_x2>=SCREENWIDTH
	|| (unsigned)ds_y>SCREENHEIGHT)
	{
	I_Error( "R_DrawSpan: %i to %i at %i",
		 ds_x1,ds_x2,ds_y);
	}
//	dscount++;
#endif

//	R_CellMarkSpan();

	xfrac = ds_xfrac;
	yfrac = ds_yfrac;

	dest = ylookup[ds_y] + columnofs[ds_x1];

	// We do not check for zero spans here?
	count = ds_x2 - ds_x1;

	do
	{
		// Current texture index in u,v.
		spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63);

		// Lookup pixel from flat texture tile,
		//  re-index using light/colormap.
		*dest++ = ds_colormap[ds_source[spot]];

		// Next step in u,v.
		xfrac += ds_xstep;
		yfrac += ds_ystep;

	} while (count--);
}
#endif

#if 0
//BGB: Loop Unrolled 2
void R_DrawSpan (void)
{
	fixed_t		xfrac;
	fixed_t		yfrac;
	dt_scrpix	*dest;
	int			count;
	int			spot;

	xfrac = ds_xfrac;
	yfrac = ds_yfrac;

	dest = ylookup[ds_y] + columnofs[ds_x1];
//	count = ds_x2 - ds_x1;
	count = ds_x2 - ds_x1 + 1;

	while(count>=4)
	{
		spot = ((yfrac>>10)&4032) + ((xfrac>>16)&63);
		dest[0] = ds_colormap[ds_source[spot]];
		xfrac += ds_xstep;		yfrac += ds_ystep;

		spot = ((yfrac>>10)&4032) + ((xfrac>>16)&63);
		dest[1] = ds_colormap[ds_source[spot]];
		xfrac += ds_xstep;		yfrac += ds_ystep;

		spot = ((yfrac>>10)&4032) + ((xfrac>>16)&63);
		dest[2] = ds_colormap[ds_source[spot]];
		xfrac += ds_xstep;		yfrac += ds_ystep;

		spot = ((yfrac>>10)&4032) + ((xfrac>>16)&63);
		dest[3] = ds_colormap[ds_source[spot]];
		xfrac += ds_xstep;		yfrac += ds_ystep;

		count -= 4;
		dest += 4;
	}

	while(count>0)
	{
		// Current texture index in u,v.
		spot = ((yfrac>>10)&4032) + ((xfrac>>16)&63);

		// Lookup pixel from flat texture tile,
		//  re-index using light/colormap.
		*dest++ = ds_colormap[ds_source[spot]];

		// Next step in u,v.
		xfrac += ds_xstep;
		yfrac += ds_ystep;
		count--;
	}
}
#endif


// UNUSED.
// Loop unrolled by 4.
// #if 0
#ifndef __BGBCC__
void R_DrawSpan (void)
{
	register unsigned	position, step;
	register byte			*source;
	register lighttable_t	*colormap;
	register dt_scrpix		*dest;

	unsigned	count;
	unsigned	spot;
	unsigned	value;
	unsigned	temp;
	unsigned	xtemp;
	unsigned	ytemp;

	position = ((ds_xfrac<<10)&0xffff0000) | ((ds_yfrac>>6)&0xffff);
	step = ((ds_xstep<<10)&0xffff0000) | ((ds_ystep>>6)&0xffff);

	source = ds_source;
	colormap = ds_colormap;
	dest = ylookup[ds_y] + columnofs[ds_x1];
	count = ds_x2 - ds_x1 + 1;

	while (count >= 4)
	{
		ytemp = position>>4;
		ytemp = ytemp & 4032;
		xtemp = position>>26;
		spot = xtemp | ytemp;
		position += step;
		dest[0] = colormap[source[spot]];

		ytemp = position>>4;
		ytemp = ytemp & 4032;
		xtemp = position>>26;
		spot = xtemp | ytemp;
		position += step;
		dest[1] = colormap[source[spot]];

		ytemp = position>>4;
		ytemp = ytemp & 4032;
		xtemp = position>>26;
		spot = xtemp | ytemp;
		position += step;
		dest[2] = colormap[source[spot]];

		ytemp = position>>4;
		ytemp = ytemp & 4032;
		xtemp = position>>26;
		spot = xtemp | ytemp;
		position += step;
		dest[3] = colormap[source[spot]];

		count -= 4;
		dest += 4;
	}
	while (count > 0)
	{
		ytemp = position>>4;
		ytemp = ytemp & 4032;
		xtemp = position>>26;
		spot = xtemp | ytemp;
		position += step;
		*dest++ = colormap[source[spot]];
		count--;
	}
}
#endif

#if 1
void R_DrawSpan_ZB (void)
{
	fixed_t		xfrac;
	fixed_t		yfrac;
	dt_scrpix	*dest;
	u16			*dest_zb;
	int			count;
	int			spot;
	int			tz;

	xfrac = ds_xfrac;
	yfrac = ds_yfrac;

	dest = ylookup[ds_y] + columnofs[ds_x1];
	dest_zb = ylookup_zb[ds_y] + columnofs[ds_x1];
	count = ds_x2 - ds_x1;

//	tz = ds_z >> 16;
	tz = ds_z >> 13;
	tz = r_int_clamp(tz, 0, 65535);

	do
	{
		// Current texture index in u,v.
		spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63);

		// Lookup pixel from flat texture tile,
		//  re-index using light/colormap.
		if(tz <= (*dest_zb))
		{
			*dest = ds_colormap[ds_source[spot]];
			*dest_zb = tz;
		}

		dest++;
		dest_zb++;

//		*dest++ = ds_colormap[ds_source[spot]];
//		*dest_zb++=ds_z;

		// Next step in u,v.
		xfrac += ds_xstep;
		yfrac += ds_ystep;

	} while (count--);
}
#endif

#ifdef __BGBCC__

void R_DrawSpanUtx (void);
void R_DrawSpanLowUtx (void);

void R_DrawColumnUtx (void);
void R_DrawColumnLowUtx (void);

__asm {
R_DrawSpanLowUtx:
	RTS

R_DrawSpanUtx:
	MOV.L		ds_x2, R4
	ADD			1, R4
	MOV.L		ds_x1, R3
	SUB			R4, R3, R7					// pixel count

	MOV			ylookup, R3
	MOV.L		ds_y, R2
	MOV.P		(R3, R2), R2
	MOV.L		ds_x1, R4
	MOV			columnofs, R3
	MOV.L		(R3, R4), R3
	LEA.W		(R2, R3), R5

	MOV.L		ds_xfrac, R16
	MOV.L		ds_yfrac, R17
	MOV.L		ds_xstep, R18
	MOV.L		ds_ystep, R19

	MOV.P		ds_source, R20
	MOV			4095, R21
	MOVLLD		R17, R16, R22
	MOVLLD		R19, R18, R23

	MOV.Q		ds_color, R6
//	MOVU.W		ds_luma, R2
//	PSHUF.W		R2, 0x00, R6

	LEA.W		(R5, R7), R4

//	MOV		0xFFFF5555AAAA5555, R6
//	MOV		0xFFFF000000000000, R2
//	OR		R2, R6

	.L0:	
	PMORT.L		R22, R2
	ADD			R23, R22
	AND			R2, R21, R3
	SHAD		R3, -4, R2
	MOV.Q		(R20, R2), R2
	BLKUTX2		R2, R3, R2
	PMULU.HW	R2, R6, R2
	RGB5PCK64	R2, R2
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R4
	BT			.L0

	RTSU


R_DrawColumnUtx:

	MOV.L		dc_yh, R4
	MOV.L		dc_yl, R20
	ADD			1, R4	
	SUB			R4, R20, R16

	CMPGT		0, R16
	BF			.done

	MOV			ylookup, R18
	MOV.L		dc_x, R7
	MOV			columnofs, R6
	MOV.L		dc_iscale, R5
	MOV.L		centery, R21
	MOV.L		dc_texturemid, R22

	SHLD.Q		R16, -1, R17
	MOV.P		(R18, R20), R23
	SUB			R21, R20, R21
	MOV.L		(R6, R7), R19

	MOV.L		dc_iscale, R7
	SHLD		R7, 9, R7
	MUL.L		R21, R5, R4
	SUB			R22, R4, R22
	LEA.W		(R23, R19), R23

	MOV		R7, R21
	SHLD	R22, 9, R18
	ADD		R18, R7, R19
	ADD		R19, R7, R20

	MOV.P	dc_source, R22	

	MOV.Q		dc_color, R7
//	MOVU.W		dc_luma, R2
//	PSHUF.W		R2, 0x00, R7

//	MOV		0xFFFF000000000000, R2
//	OR		R2, R7

	MOV		0x8000000000000000, R1

	MOVU.W		dc_col, R3
	AND			3, R3

#if 1
.L0:
	SHLD		R20, -25, R5
	SHLD		R20, -27, R6
	SHLD		R5, 2, R5
	OR			R3, R5
	
	MOV.Q		(R22, R6), R18
	BLKUTX2		R18, R5, R18
	PMULU.HW	R18, R7, R2
	RGB5PCK64	R2, R18

	TSTQ		R1, R2
	MOV.W?F		R18, (R23)

	ADD			R21, R20
	ADD			640, R23
	ADD			-1, R16
//	TEST		R16, R16
//	BF			.L0
	CMPGT		0, R16
	BT			.L0
#endif

.done:
	RTS

R_DrawColumnLowUtx:
	RTS

};

#else

const u16 pmorttab[256] = {
0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555 };


int tkra_morton8(int x, int y)
{
//	return(pmorttab[x]|(pmorttab[y]<<1));
	return(pmorttab[x&255]|(pmorttab[y&255]<<1));
}

int tkra_morton16(int x, int y)
{
//	return(pmorttab[x]|(pmorttab[y]<<1));
	return(
		(pmorttab[(x   )&255]    )|(pmorttab[(y   )&255]<< 1)|
		(pmorttab[(x>>8)&255]<<16)|(pmorttab[(y>>8)&255]<<17));
}


u64 tkra_pmuluhw(u64 a, u64 b)
{
	int ax, ay, az, aw;
	int bx, by, bz, bw;
	int cx, cy, cz, cw;
	u64 c;
	
	ax=(u16)(a>> 0);	ay=(u16)(a>>16);
	az=(u16)(a>>32);	aw=(u16)(a>>48);
	bx=(u16)(b>> 0);	by=(u16)(b>>16);
	bz=(u16)(b>>32);	bw=(u16)(b>>48);
	cx=(ax*bx)>>16;		cy=(ay*by)>>16;
	cz=(az*bz)>>16;		cw=(aw*bw)>>16;
	cx=(u16)cx;			cy=(u16)cy;
	cz=(u16)cz;			cw=(u16)cw;
	c=	(((u64)cx)<< 0) | (((u64)cy)<<16) |
		(((u64)cz)<<32) | (((u64)cw)<<48) ;
	return(c);
}

u64 tkra_rgbupck64(u16 a)
{
	int ax, ay, az, aw, av;
	u64 c;

//	av=(a&0x0001)|((a&0x0020)>>5)|((a&0x0400)>>10);
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=av<<13;
	ax=(a&0x001F)<<11;
	ay=(a&0x03E0)<< 6;
	az=(a&0x7C00)<< 1;
	
//	av|=av>>3;
	ax|=ax>>5;
	ay|=ay>>5;
	az|=az>>5;
	
	aw=(a&0x8000)?av:0xFFFF;
	c=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
	return(c);
	
}

u16 tkra_rgbpck64(u64 a)
{
	int av;
	u16 c;

	av=(a>>60)&15;
	if(av==15)
	{
		c=	((a>>33)&0x7C00) |
			((a>>22)&0x03E0) |
			((a>>11)&0x001F);
	}else
	{
		c=	((a>>33)&0x7800) |
			((a>>22)&0x03C0) |
			((a>>11)&0x001E) |
			0x8000;
		if(av&8)c|=0x0400;
		if(av&4)c|=0x0020;
		if(av&2)c|=0x0001;
	}
	return(c);
}

int TKRA_GetPixel444A3_Alpha(int a)
{
	int av;
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=(av<<5);
	return(av);
}

// int		 tkra_utx2_cachedindx[64];
// void	*tkra_utx2_cachedblka[64];
u64		tkra_utx2_cachedblkv[64];
u64		tkra_utx2_cachedpels[64][16];

u64 TKRA_CachedBlkUtx2(void *src, int ix)
{
	u64 *blka;
	u64 tca[4];
	int pxa, pxb, pxv;
	int axa, axb, axc;
	u64 blk;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	if(!src)
	{
//		__debugbreak();
		return(0);
	}

	ix0=ix&15;
	ix1=ix>>4;

	blka=src;
	blk=blka[ix1];

//	hxi=((ix1*251)>>8)&255;
//	hxi=(ix1^(ix1>>6))&63;

	hxi = ((blk*(65521ULL*65521ULL*65521ULL*251ULL))>>56)&63;
	
//	if(	(tkra_utx2_cachedindx[hxi]==ix1) &&
//		(tkra_utx2_cachedblka[hxi]==src))
	if(tkra_utx2_cachedblkv[hxi]==blk)
	{
		return(tkra_utx2_cachedpels[hxi][ix0]);
	}

	blka=src;
	blk=blka[ix1];

	pxa=(u16)(blk>> 0);
	pxb=(u16)(blk>>16);
	
//	pxa=0x5555;
//	pxb=0x7AAA;
	
	pxv=blk>>32;
	clra=tkra_rgbupck64(pxa);
	clrb=tkra_rgbupck64(pxb);

	clrc=tkra_pmuluhw(clra, 0xAAAAAAAAAAAAAAAAULL);
	clrd=tkra_pmuluhw(clrb, 0xAAAAAAAAAAAAAAAAULL);
	clrp=tkra_pmuluhw(clra, 0x5555555555555555ULL);
	clrq=tkra_pmuluhw(clrb, 0x5555555555555555ULL);
	clrc+=clrq;
	clrd+=clrp;

	tca[0]=clrb;
	tca[1]=clrd;
	tca[2]=clrc;
	tca[3]=clra;

	if((pxa&0x8000) && (pxb&0x8000))
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
		if(pxb&0x8000)
	{
		axa=TKRA_GetPixel444A3_Alpha(pxa);
		axb=TKRA_GetPixel444A3_Alpha(pxb);
		axa=axa<<8;
		axb=axb<<8;

		for(i=0; i<16; i++)
		{
			clrp= ((pxv>>(i*2+1))&1)?clra:clrb;
			axc = ((pxv>>(i*2+0))&1)?axa:axb;
			clrp&=0x0000FFFFFFFFFFFFULL;
			clrp|=((u64)axc)<<48;
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}

//	tkra_utx2_cachedindx[hxi]=ix1;
//	tkra_utx2_cachedblka[hxi]=src;
	tkra_utx2_cachedblkv[hxi]=blk;
	return(tkra_utx2_cachedpels[hxi][ix0]);
}

void R_DrawSpanUtx (void)
{
	fixed_t		xfrac;
	fixed_t		yfrac;
	dt_scrpix	*dest;
	u64			cval, blk, cpos;
	int			count;
	int			spot;
	int			pix, clr, idx;

	xfrac = ds_xfrac;
	yfrac = ds_yfrac;

	cpos = ds_color;
//	cpos = ds_luma;
//	cpos |= cpos<<16;
//	cpos |= cpos<<32;
//	cpos |= 0xFFFF000000000000ULL;
//	cpos = ~0;

	dest = ylookup[ds_y] + columnofs[ds_x1];

	// We do not check for zero spans here?
	count = ds_x2 - ds_x1;

	do
	{
		// Current texture index in u,v.
//		spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63);

		idx=tkra_morton16(xfrac>>16, yfrac>>16)&4095;

//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(ds_source, idx);
//		cval = ~0;
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);


		// Lookup pixel from flat texture tile,
		//  re-index using light/colormap.
//		*dest++ = ds_colormap[ds_source[spot]];
//		*dest++ = 0x5555;
//		if(!(pix&0x8000))
			*dest++ = pix;

		// Next step in u,v.
		xfrac += ds_xstep;
		yfrac += ds_ystep;

	} while (count--);
}

void R_DrawSpanLowUtx (void)
{
}

void R_DrawColumnUtx (void)
{
	int				count;
	register byte*			source;
	register dt_scrpix*		dest;
	register lighttable_t*	colormap;

	register unsigned	frac;
	register unsigned	fracstep;

	u64			cval, blk, cpos;
	int			pix, clr, idx;

	count = dc_yh - dc_yl + 1;

	if(count<=0)
		return;

	source = dc_source;
	if(!source)
		return;

	colormap = dc_colormap;
	dest = ylookup[dc_yl] + columnofs[dc_x];

	fracstep = dc_iscale<<9;
	frac = (dc_texturemid + (dc_yl-centery)*dc_iscale)<<9;

//	cpos = dc_luma;
	cpos = dc_color;
//	cpos |= cpos<<16;
//	cpos |= cpos<<32;
//	cpos |= 0xFFFF000000000000ULL;

	while (count > 0)
	{
		idx = frac>>25;
		idx = (idx<<2) | (dc_col&3);
	
		cval=TKRA_CachedBlkUtx2(dc_source, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

//		*dest = colormap[source[frac>>25]];
//		*dest = 0x5555;

//		if(!(pix&0x8000))
		if((cval>>63)&1)
			*dest = pix;

		dest += SCREENWIDTH;
		frac += fracstep;
		count--;
	}
}

void R_DrawColumnLowUtx (void)
{
}

#endif

// #if 0
#ifndef __BGBCC__

//
// Again..
//
void R_DrawSpanLow (void)
{
	fixed_t		xfrac;
	fixed_t		yfrac;
	dt_scrpix	*dest;
	int			count;
	int			spot;
	int			x1, x2;

#ifdef RANGECHECK
	if (ds_x2 < ds_x1
	|| ds_x1<0
	|| ds_x2>=SCREENWIDTH
	|| (unsigned)ds_y>SCREENHEIGHT)
	{
	I_Error( "R_DrawSpan: %i to %i at %i",
		 ds_x1,ds_x2,ds_y);
	}
//	dscount++;
#endif

//	return;

	xfrac = ds_xfrac;
	yfrac = ds_yfrac;

	// Blocky mode, need to multiply by 2.
//	ds_x1 <<= 1;
//	ds_x2 <<= 1;

	x1 = ds_x1 << 1;
	x2 = ds_x2 << 1;

//	if(x1 < 0) x1 = 0;
//	if(x2 >= SCREENWIDTH) x2 = SCREENWIDTH-1;

//	x1 = ds_x1;
//	x2 = ds_x2;

//	R_CellMarkSpan();

//	dest = ylookup[ds_y] + columnofs[ds_x1];
	dest = ylookup[ds_y] + columnofs[x1];


//	count = ds_x2 - ds_x1;
//	count = x2 - x1;
	count = (x2 - x1)>>1;
	do
	{
		spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63);
		// Lowres/blocky mode does it twice,
		//  while scale is adjusted appropriately.
		*dest++ = ds_colormap[ds_source[spot]];
		*dest++ = ds_colormap[ds_source[spot]];

		xfrac += ds_xstep;
		yfrac += ds_ystep;

	} while (count--);
}
#endif

//
// R_InitBuffer
// Creats lookup tables that avoid
//  multiplies and other hazzles
//  for getting the framebuffer address
//  of a pixel to draw.
//
void
R_InitBuffer
( int		width,
  int		height )
{
	int		i;

	//BGB: Clear to Zero
	for (i=0 ; i<MAXWIDTH ; i++)
		{ columnofs[i] = 0; }

	for (i=0 ; i<MAXHEIGHT ; i++)
	{
		ylookup[i] = 0;
		ylookup_zb[i] = 0;
	}

	if( (width >= MAXWIDTH) || (height >= MAXHEIGHT) )
		__debugbreak();

	// Handle resize,
	//  e.g. smaller view windows
	//  with border and/or status bar.
	viewwindowx = (SCREENWIDTH-width) >> 1;

	// Column offset. For windows.
	for (i=0 ; i<width ; i++)
		columnofs[i] = viewwindowx + i;

	// Samw with base row offset.
	if (width == SCREENWIDTH)
		viewwindowy = 0;
	else
		viewwindowy = (SCREENHEIGHT-SBARHEIGHT-height) >> 1;


	// Preclaculate all row offsets.
	for (i=0 ; i<height ; i++)
	{
		ylookup[i] = screens[0] + (i+viewwindowy)*SCREENWIDTH;

		ylookup_zb[i] = screens_zbuf + (i+viewwindowy)*SCREENWIDTH;
	}
}




//
// R_FillBackScreen
// Fills the back screen with a pattern
//  for variable screen sizes
// Also draws a beveled edge.
//
void R_FillBackScreen (void)
{
	byte*	src;
	dt_scrpix	*dest;
	dt_scrpix	*cmap;
	int		x;
	int		y;
	int		w;
	patch_t*	patch;

	// DOOM border patch.
//	char	name1[] = "FLOOR7_2";

	// DOOM II border patch.
//	char	name2[] = "GRNROCK";

	static const char	*name1 = "FLOOR7_2";
	static const char	*name2 = "GRNROCK";

	const char*	name;

	if (scaledviewwidth == 320)
		return;

	if ( gamemode == commercial)
		name = name2;
	else
		name = name1;

//	src = W_CacheLumpName (name, PU_CACHE);
	src = W_CacheFlatName (name, PU_CACHE);
	dest = screens[1];

//	cmap = ds_colormap;
	cmap = colormaps;

	for (y=0 ; y<SCREENHEIGHT-SBARHEIGHT ; y++)
	{
		if(sizeof(*src)==sizeof(*dest))
		{
			for (x=0 ; x<SCREENWIDTH/64 ; x++)
			{
				memcpy (dest, src+((y&63)<<6), 64*sizeof(dt_scrpix));
				dest += 64;
			}

			if (SCREENWIDTH&63)
			{
				memcpy (dest, src+((y&63)<<6),
					(SCREENWIDTH&63)*sizeof(dt_scrpix));
				dest += (SCREENWIDTH&63);
			}
		}else
		{
			for (x=0 ; x<SCREENWIDTH/64 ; x++)
			{
				for(w=0; w<64; w++)
//					dest[w]=(src+((y&63)<<6))[w];
					dest[w]=cmap[(src+((y&63)<<6))[w]];
				dest += 64;
			}

			if (SCREENWIDTH&63)
			{
				for(w=0; w<(SCREENWIDTH&63); w++)
//					dest[w]=(src+((y&63)<<6))[w];
					dest[w]=cmap[(src+((y&63)<<6))[w]];
				dest += (SCREENWIDTH&63);
			}
		}
	}

//	patch = W_CacheLumpName ("brdr_t",PU_CACHE);
	patch = W_CachePatchName ("brdr_t",PU_CACHE);

	for (x=0 ; x<scaledviewwidth ; x+=8)
		V_DrawPatch (viewwindowx+x,viewwindowy-8,1,patch);
//	patch = W_CacheLumpName ("brdr_b",PU_CACHE);
	patch = W_CachePatchName ("brdr_b",PU_CACHE);

	for (x=0 ; x<scaledviewwidth ; x+=8)
		V_DrawPatch (viewwindowx+x,viewwindowy+viewheight,1,patch);
//	patch = W_CacheLumpName ("brdr_l",PU_CACHE);
	patch = W_CachePatchName ("brdr_l",PU_CACHE);

	for (y=0 ; y<viewheight ; y+=8)
		V_DrawPatch (viewwindowx-8,viewwindowy+y,1,patch);
//	patch = W_CacheLumpName ("brdr_r",PU_CACHE);
	patch = W_CachePatchName ("brdr_r",PU_CACHE);

	for (y=0 ; y<viewheight ; y+=8)
	V_DrawPatch (viewwindowx+scaledviewwidth,viewwindowy+y,1,patch);


	// Draw beveled edge.
	V_DrawPatch (viewwindowx-8,
		 viewwindowy-8,
		 1,
//		 W_CacheLumpName ("brdr_tl",PU_CACHE));
		 W_CachePatchName ("brdr_tl",PU_CACHE));

	V_DrawPatch (viewwindowx+scaledviewwidth,
		 viewwindowy-8,
		 1,
//		 W_CacheLumpName ("brdr_tr",PU_CACHE));
		 W_CachePatchName ("brdr_tr",PU_CACHE));

	V_DrawPatch (viewwindowx-8,
		 viewwindowy+viewheight,
		 1,
//		 W_CacheLumpName ("brdr_bl",PU_CACHE));
		 W_CachePatchName ("brdr_bl",PU_CACHE));

	V_DrawPatch (viewwindowx+scaledviewwidth,
		 viewwindowy+viewheight,
		 1,
//		 W_CacheLumpName ("brdr_br",PU_CACHE));
		 W_CachePatchName ("brdr_br",PU_CACHE));
}


//
// Copy a screen buffer.
//
void
R_VideoErase
( unsigned	ofs,
  int		count )
{
  // LFB copy.
  // This might not be a good idea if memcpy
  //  is not optiomal, e.g. byte by byte on
  //  a 32bit CPU, as GNU GCC/Linux libc did
  //  at one point.
	memcpy (screens[0]+ofs, screens[1]+ofs, count*sizeof(dt_scrpix));
}


//
// R_DrawViewBorder
// Draws the border around the view
//  for different size windows?
//
void
V_MarkRect
( int		x,
  int		y,
  int		width,
  int		height );

void R_DrawViewBorder (void)
{
	int		top;
	int		side;
	int		ofs;
	int		i;

	if (scaledviewwidth == SCREENWIDTH)
	return;

	top = ((SCREENHEIGHT-SBARHEIGHT)-viewheight)/2;
	side = (SCREENWIDTH-scaledviewwidth)/2;

	// copy top and one line of left side
	R_VideoErase (0, top*SCREENWIDTH+side);

	// copy one line of right side and bottom
	ofs = (viewheight+top)*SCREENWIDTH-side;
	R_VideoErase (ofs, top*SCREENWIDTH+side);

	// copy sides using wraparound
	ofs = top*SCREENWIDTH + SCREENWIDTH-side;
	side <<= 1;

	for (i=1 ; i<viewheight ; i++)
	{
		R_VideoErase (ofs, side);
		ofs += SCREENWIDTH;
	}

	// ?
	V_MarkRect (0,0,SCREENWIDTH, SCREENHEIGHT-SBARHEIGHT);
}



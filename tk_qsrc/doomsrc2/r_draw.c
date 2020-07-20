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

 
#ifndef __BGBCC__

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

	char*	name;
	
	if (scaledviewwidth == 320)
		return;
	
	if ( gamemode == commercial)
		name = name2;
	else
		name = name1;
	
	src = W_CacheLumpName (name, PU_CACHE); 
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
	
	patch = W_CacheLumpName ("brdr_t",PU_CACHE);

	for (x=0 ; x<scaledviewwidth ; x+=8)
		V_DrawPatch (viewwindowx+x,viewwindowy-8,1,patch);
	patch = W_CacheLumpName ("brdr_b",PU_CACHE);

	for (x=0 ; x<scaledviewwidth ; x+=8)
		V_DrawPatch (viewwindowx+x,viewwindowy+viewheight,1,patch);
	patch = W_CacheLumpName ("brdr_l",PU_CACHE);

	for (y=0 ; y<viewheight ; y+=8)
		V_DrawPatch (viewwindowx-8,viewwindowy+y,1,patch);
	patch = W_CacheLumpName ("brdr_r",PU_CACHE);

	for (y=0 ; y<viewheight ; y+=8)
	V_DrawPatch (viewwindowx+scaledviewwidth,viewwindowy+y,1,patch);


	// Draw beveled edge. 
	V_DrawPatch (viewwindowx-8,
		 viewwindowy-8,
		 1,
		 W_CacheLumpName ("brdr_tl",PU_CACHE));
	
	V_DrawPatch (viewwindowx+scaledviewwidth,
		 viewwindowy-8,
		 1,
		 W_CacheLumpName ("brdr_tr",PU_CACHE));
	
	V_DrawPatch (viewwindowx-8,
		 viewwindowy+viewheight,
		 1,
		 W_CacheLumpName ("brdr_bl",PU_CACHE));
	
	V_DrawPatch (viewwindowx+scaledviewwidth,
		 viewwindowy+viewheight,
		 1,
		 W_CacheLumpName ("brdr_br",PU_CACHE));
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
 
 

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
// DESCRIPTION:
//	System specific interface stuff.
//
//-----------------------------------------------------------------------------


#ifndef __R_DRAW__
#define __R_DRAW__


#ifdef __GNUG__
#pragma interface
#endif

static int r_int_clamp(int v, int m, int n)
{
	if(v>=m)
	{
		if(v<=n)
			return(v);
		return(n);
	}
	return(m);
}

static int r_int_min(int m, int n)
{
	return((m<=n)?m:n);
}

static int r_int_max(int m, int n)
{
	return((m>n)?m:n);
}

global_register extern lighttable_t	*dc_colormap;
global_register extern int				dc_x;
global_register extern int				dc_yl;
global_register extern int				dc_yh;
global_register extern fixed_t			dc_scale;
global_register extern fixed_t			dc_iscale;
global_register extern fixed_t			dc_texturemid;
global_register	extern int				dc_col;
global_register	extern u16				dc_baseluma;
//global_register	extern u16				dc_luma;
global_register	extern u64				dc_color;

extern byte		dc_isspr;
extern fixed_t		dc_zdist;

// first pixel in a column
global_register extern byte			*dc_source;		


// The span blitting interface.
// Hook in assembler or system specific BLT
//  here.
void 	R_DrawColumn (void);
void 	R_DrawColumnLow (void);

void	R_DrawColumnUtx (void);
void	R_DrawColumnLowUtx (void);

void	R_DrawColumn_ZB (void);

// The Spectre/Invisibility effect.
void 	R_DrawFuzzColumn (void);
void 	R_DrawFuzzColumnLow (void);

// Draw with color translation tables,
//  for player sprite rendering,
//  Green/Red/Blue/Indigo shirts.
void	R_DrawTranslatedColumn (void);
void	R_DrawTranslatedColumnLow (void);

void
R_VideoErase
( unsigned	ofs,
  int		count );

global_register extern int				ds_y;
global_register extern int				ds_x1;
global_register extern int				ds_x2;

global_register extern lighttable_t	*ds_colormap;

global_register extern fixed_t			ds_xfrac;
global_register extern fixed_t			ds_yfrac;
global_register extern fixed_t			ds_xstep;
global_register extern fixed_t			ds_ystep;
global_register extern fixed_t			ds_z;

// start of a 64*64 tile image
global_register extern byte			*ds_source;		

global_register extern byte			*translationtables;
global_register extern byte			*dc_translation;

global_register extern u16				ds_baseluma;
// global_register extern u16				ds_luma;
global_register extern u64				ds_color;


// Span blitting for rows, floor/ceiling.
// No Sepctre effect needed.
void 	R_DrawSpan (void);
void	R_DrawSpanUtx (void);

void 	R_DrawSpan_ZB (void);

// Low resolution mode, 160x200?
void 	R_DrawSpanLow (void);
void	R_DrawSpanLowUtx (void);


void
R_InitBuffer
( int		width,
  int		height );


// Initialize color translation tables,
//  for player rendering etc.
void	R_InitTranslationTables (void);



// Rendering function.
void R_FillBackScreen (void);

// If the view size is not full screen, draws a border around it.
void R_DrawViewBorder (void);

void R_ClearCZBuf (void);



#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Gamma correction LUT stuff.
//	Functions to draw patches (by post) directly to screen.
//	Functions to blit a block to the screen.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: v_video.c,v 1.5 1997/02/03 22:45:13 b1 Exp $";


#include "i_system.h"
#include "r_local.h"

#include "doomdef.h"
#include "doomdata.h"

#include "m_bbox.h"
#include "m_swap.h"

#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"


// Each screen is [SCREENWIDTH*SCREENHEIGHT]; 
dt_scrpix		*screens[5];	
u16				*screens_zbuf;
dt_scrpix		*screen;
dt_scrpix		*screens_base;

int				dirtybox[4]; 



// Now where did these came from?
byte gammatable[5][256] =
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
     17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
     81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
     97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
     144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
     160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
     176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
     192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
     208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

    {2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,
     32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,
     56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,
     78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
     99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,
     130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
     146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,
     161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,
     175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,
     190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,
     205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,
     219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,
     233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,
     247,248,249,250,251,252,252,253,254,255},

    {4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,
     43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,
     70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,
     94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,
     144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,
     160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,
     174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,
     188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,
     202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,
     216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,
     229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,
     242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,
     255},

    {8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,
     57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,
     86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,
     108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,
     141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
     155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,
     169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,
     183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,
     195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,
     207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,
     219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,
     231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,
     242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,
     253,253,254,254,255},

    {16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,
     78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,
     107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
     142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
     156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,
     169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,
     182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,
     193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,
     204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,
     214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,
     224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,
     234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,
     243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,
     251,252,252,253,254,254,255,255}
};



int	usegamma;
			 
//
// V_MarkRect 
// 
void
V_MarkRect
( int		x,
  int		y,
  int		width,
  int		height ) 
{ 
    M_AddToBox (dirtybox, x, y); 
    M_AddToBox (dirtybox, x+width-1, y+height-1); 
} 
 

//
// V_CopyRect 
// 
void
V_CopyRect
( int		srcx,
  int		srcy,
  int		srcscrn,
  int		width,
  int		height,
  int		destx,
  int		desty,
  int		destscrn ) 
{ 
    dt_scrpix	*src;
    dt_scrpix	*dest; 
	 
#ifdef RANGECHECK 
    if (srcx<0
	||srcx+width >SCREENWIDTH
	|| srcy<0
	|| srcy+height>SCREENHEIGHT 
	||destx<0||destx+width >SCREENWIDTH
	|| desty<0
	|| desty+height>SCREENHEIGHT 
	|| (unsigned)srcscrn>4
	|| (unsigned)destscrn>4)
    {
	I_Error ("Bad V_CopyRect");
    }
#endif 
    V_MarkRect (destx, desty, width, height); 
	 
    src = screens[srcscrn]+SCREENWIDTH*srcy+srcx; 
    dest = screens[destscrn]+SCREENWIDTH*desty+destx; 

    for ( ; height>0 ; height--) 
    { 
		memcpy (dest, src, width*sizeof(dt_scrpix)); 
		src += SCREENWIDTH; 
		dest += SCREENWIDTH; 
    } 
} 
 

//
// V_DrawPatch
// Masks a column based masked pic to the screen. 
//

void V_DrawPatchCmap_Col(
	byte *src, dt_scrpix *dst,
	lighttable_t *tcol, int cnt);

void
V_DrawPatchCmap
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch,
  lighttable_t *tcol1) 
{ 
	register lighttable_t *tcol;
    register int		count;
    int		col; 
    column_t*	column; 
    dt_scrpix	*desttop;
    dt_scrpix	*scrp, *scrp_end;
    register dt_scrpix	*dest;
    dt_scrpix	*dest_end, *dest_e1;
    register byte*	source;
    register int i0, i1, i2, i3;
    int		w; 

	__hint_use_egpr();

	tcol = tcol1;

	if(!patch)
		return;
	 
//    y -= SHORT(patch->topoffset); 
    y -= (patch->topoffset); 
//    x -= SHORT(patch->leftoffset); 
    x -= (patch->leftoffset); 
#ifdef RANGECHECK 
    if (x<0
//	|| (x+SHORT(patch->width) >SCREENWIDTH)
	|| (x+(patch->width) >SCREENWIDTH)
	|| (y<0)
//	|| (y+SHORT(patch->height)>SCREENHEIGHT)
	|| (y+(patch->height)>SCREENHEIGHT) 
	|| ((unsigned)scrn>4))
//	|| ((unsigned)scrn>0))
    {
      fprintf( stderr, "Patch at %d,%d exceeds LFB\n", x,y );
      // No I_Error abort - what is up with TNT.WAD?
      fprintf( stderr, "V_DrawPatch: bad patch (ignored)\n");
      return;
    }
#endif 
 
    if (!scrn)
    {
//		V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 
		V_MarkRect (x, y, (patch->width), (patch->height)); 
//		R_CellMarkBox(
//			x, x+SHORT(patch->width),
//			y, y+SHORT(patch->height));
	}else
	{
//		return;
	}

    col = 0; 
    scrp = screens[scrn];
    if(!scrp)
		return;
    desttop = scrp+y*SCREENWIDTH+x; 
	
	scrp_end = scrp + SCREENWIDTH * SCREENHEIGHT;
	
//	__debugbreak();
	
//    w = SHORT(patch->width); 
    w = (patch->width); 

    for ( ; col<w ; x++, col++, desttop++)
    { 
//		column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 
		column = (column_t *)((byte *)patch + (patch->columnofs[col])); 
	 
		// step through the posts in a column 
		while (column->topdelta != 0xff ) 
		{ 
			source = (byte *)column + 3; 
			dest = desttop + column->topdelta*SCREENWIDTH; 
			count = column->length; 
			dest_end = dest + count*SCREENWIDTH; 

//			if(dest<scr)
//				break;

//			if(dest_end>scr_end)
//				break;

			if((dest<scrp) || (dest_end>scrp_end))
			{
				if(dest>=scrp)
				{
					dest_e1 = dest+4*SCREENWIDTH;
					while((count>=4) && (dest_e1<=scrp_end))
					{
						i0 = source[0];	i1 = source[1];
						i2 = source[2];	i3 = source[3];
						i0 = tcol[i0];	i1 = tcol[i1];
						i2 = tcol[i2];	i3 = tcol[i3];
						dest[0*SCREENWIDTH] = i0;
						dest[1*SCREENWIDTH] = i1;
						dest[2*SCREENWIDTH] = i2;
						dest[3*SCREENWIDTH] = i3;
						dest = dest_e1;
						dest_e1 += 4*SCREENWIDTH; 
						
						source+=4;
						count-=4;
					}
				}
			
				while (count--) 
				{ 
//					if(dest<scrp)
					if((dest<scrp) || (dest>scrp_end))
					{
						source++;
						dest += SCREENWIDTH; 
						continue;
					}
//					if(dest>scrp_end)
//						break;

	//				*dest = *source++; 
					*dest = tcol[*source++];
					dest += SCREENWIDTH; 
				}

				column = (column_t *)(
					(byte *)column + column->length + 4 ); 
				continue;
			}

// #ifndef __BGBCC__
#ifndef __BJX2__
//#if 1

#if 1
			while (count>=4) 
			{ 
				dest[0*SCREENWIDTH] = tcol[source[0]];
				dest[1*SCREENWIDTH] = tcol[source[1]];
				dest[2*SCREENWIDTH] = tcol[source[2]];
				dest[3*SCREENWIDTH] = tcol[source[3]];
				dest += 4*SCREENWIDTH; 
				
				source+=4;
				count-=4;
			}
#endif

#if 1
			while (count--) 
			{ 
//				*dest = *source++; 
				*dest = tcol[*source++];
				dest += SCREENWIDTH; 
			} 
#endif

#else

			V_DrawPatchCmap_Col(source, dest, tcol, count);

#endif

			column = (column_t *)(  (byte *)column + column->length 
						+ 4 ); 
		} 
    }			 
} 

void
V_DrawPatch
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch ) 
{
	if(!patch)
		return;

//	V_DrawPatchCmap(x, y,
//		scrn, patch,
//		colormaps);
	V_DrawPatchDirect(x, y,
		scrn, patch);
}

//
// V_DrawPatchFlipped 
// Masks a column based masked pic to the screen.
// Flips horizontally, e.g. to mirror face.
//
void
V_DrawPatchFlipped
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch ) 
{ 

    int		count;
    int		col; 
    column_t*	column; 
    lighttable_t *tcol;
    dt_scrpix	*desttop;
    dt_scrpix	*dest;
    byte*	source; 
    int		w; 

	if(!patch)
		return;
	 
//    y -= SHORT(patch->topoffset); 
    y -= (patch->topoffset); 
//    x -= SHORT(patch->leftoffset); 
    x -= (patch->leftoffset); 
#ifdef RANGECHECK 
    if (x<0
//	||x+SHORT(patch->width) >SCREENWIDTH
	||x+(patch->width) >SCREENWIDTH
	|| y<0
//	|| y+SHORT(patch->height)>SCREENHEIGHT 
	|| y+(patch->height)>SCREENHEIGHT 
	|| (unsigned)scrn>4)
    {
      fprintf( stderr, "Patch origin %d,%d exceeds LFB\n", x,y );
      I_Error ("Bad V_DrawPatch in V_DrawPatchFlipped");
    }
#endif 
 
    if (!scrn)
//		V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 
		V_MarkRect (x, y, (patch->width), (patch->height)); 

	tcol = colormaps;

    col = 0; 
    desttop = screens[scrn]+y*SCREENWIDTH+x; 
	 
//    w = SHORT(patch->width); 
    w = (patch->width); 

    for ( ; col<w ; x++, col++, desttop++) 
    { 
//		column = (column_t *)((byte *)patch + LONG(patch->columnofs[w-1-col])); 
		column = (column_t *)((byte *)patch + (patch->columnofs[w-1-col])); 
	 
		// step through the posts in a column 
		while (column->topdelta != 0xff ) 
		{ 
			source = (byte *)column + 3; 
			dest = desttop + column->topdelta*SCREENWIDTH; 
			count = column->length; 

#ifndef __BJX2__
			while (count--) 
			{ 
				*dest = *source++; 
				dest += SCREENWIDTH; 
			} 
#else
			V_DrawPatchCmap_Col(source, dest, tcol, count);
#endif

			column = (column_t *)(  (byte *)column + column->length 
						+ 4 ); 
		} 
    }			 
} 
 

extern int		n_colormaps_alt;

//
// V_DrawPatchDirect
// Draws directly to the screen on the pc. 
//
void
V_DrawPatchDirect
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch ) 
{
	lighttable_t	*tcol;
	int lump;

	if(n_colormaps_alt<=1)
	{
		V_DrawPatchCmap(x, y,
			scrn, patch,
			colormaps);
		return;
	}
	
	if(!patch)
		return;

	lump = W_GetNumForCache(patch);
    tcol = R_ColormapForLump (lump, 0);
    
    if(tcol)
    {
		V_DrawPatchCmap (x,y,scrn, patch, tcol); 
    }else
	{
//		V_DrawPatch (x,y,scrn, patch); 
		V_DrawPatchCmap(x, y,
			scrn, patch,
			colormaps);
	}

    /*
    int		count;
    int		col; 
    column_t*	column; 
    byte*	desttop;
    byte*	dest;
    byte*	source; 
    int		w; 
	 
//    y -= SHORT(patch->topoffset); 
//    x -= SHORT(patch->leftoffset); 

#ifdef RANGECHECK 
    if (x<0
	||x+SHORT(patch->width) >SCREENWIDTH
	|| y<0
	|| y+SHORT(patch->height)>SCREENHEIGHT 
	|| (unsigned)scrn>4)
    {
	I_Error ("Bad V_DrawPatchDirect");
    }
#endif 
 
    //	V_MarkRect (x, y, SHORT(patch->width), SHORT(patch->height)); 
    desttop = destscreen + y*SCREENWIDTH/4 + (x>>2); 
	 
    w = SHORT(patch->width); 
    for ( col = 0 ; col<w ; col++) 
    { 
	outp (SC_INDEX+1,1<<(x&3)); 
//	column = (column_t *)((byte *)patch + LONG(patch->columnofs[col])); 
 
	// step through the posts in a column 
	 
	while (column->topdelta != 0xff ) 
	{ 
	    source = (byte *)column + 3; 
	    dest = desttop + column->topdelta*SCREENWIDTH/4; 
	    count = column->length; 
 
	    while (count--) 
	    { 
		*dest = *source++; 
		dest += SCREENWIDTH/4; 
	    } 
	    column = (column_t *)(  (byte *)column + column->length 
				    + 4 ); 
	} 
	if ( ((++x)&3) == 0 ) 
	    desttop++;	// go to next byte, not next plane 
    }*/ 
} 
 

void
V_DrawPatchDirectName
( int		x,
  int		y,
  int		scrn,
  char		*name ) 
{
	patch_t			*patch;
	lighttable_t	*tcol;
	int				lump, size;
	
//	pat = W_CacheLumpName(skullName[whichSkull],PU_CACHE);
	lump = W_GetNumForName (name);

	if(lump < 0)
		return;
	
//    patch = W_CacheLumpNum (lump, PU_CACHE);
    patch = W_CachePatchNum (lump, PU_CACHE);
    tcol = R_ColormapForLump (lump, 0);
//    V_DrawPatchDirect(x, y, scrn, patch);

	size = W_LumpLength(lump);
	
	if(size == 64000)
	{
		if(tcol)
			V_DrawBlockCmap(0, 0, scrn, 320, 200, (byte *)patch, tcol);
		else
			V_DrawBlockCmap(0, 0, scrn, 320, 200, (byte *)patch, colormaps);
//			V_DrawBlock(0, 0, scrn, 320, 200, (byte *)patch);
		return;
	}

	if(tcol)
		V_DrawPatchCmap(x, y, scrn, patch, tcol);
	else
		V_DrawPatchDirect(x, y, scrn, patch);
}


//
// V_DrawBlock
// Draw a linear block of pixels into the view buffer.
//
void
V_DrawBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		src ) 
{ 
    dt_scrpix	*dest;
    int i;
	 
#ifdef RANGECHECK 
    if (x<0
	||x+width >SCREENWIDTH
	|| y<0
	|| y+height>SCREENHEIGHT 
	|| (unsigned)scrn>4 )
    {
	I_Error ("Bad V_DrawBlock");
    }
#endif 
 
    V_MarkRect (x, y, width, height); 
 
    dest = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    {
		if(sizeof(*src)==sizeof(*dest))
		{
			memcpy (dest, src, width*sizeof(dt_scrpix)); 
		}else
		{
			for(i=0; i<width; i++)
				dest[i]=src[i];
		}
		src += width; 
		dest += SCREENWIDTH; 
    } 
} 

void
V_DrawBlockCmap
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		src,
  dt_scrpix	*cmap) 
{ 
    dt_scrpix	*dest;
    int i;
	 
#ifdef RANGECHECK 
    if (x<0
	||x+width >SCREENWIDTH
	|| y<0
	|| y+height>SCREENHEIGHT 
	|| (unsigned)scrn>4 )
    {
	I_Error ("Bad V_DrawBlock");
    }
#endif 
 
    V_MarkRect (x, y, width, height); 
 
    dest = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    {
		if(sizeof(*src)==sizeof(*dest))
		{
			memcpy (dest, src, width*sizeof(dt_scrpix)); 
		}else
		{
			for(i=0; i<width; i++)
				dest[i]=cmap[src[i]];
		}
		src += width; 
		dest += SCREENWIDTH; 
    } 
} 
 

//
// V_DrawBlock
// Draw a linear block of pixels into the view buffer.
//
void
V_DrawBlockScr
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  dt_scrpix*	src ) 
{ 
    dt_scrpix	*dest;
    int i;
	 
#ifdef RANGECHECK 
    if (x<0
	||x+width >SCREENWIDTH
	|| y<0
	|| y+height>SCREENHEIGHT 
	|| (unsigned)scrn>4 )
    {
	I_Error ("Bad V_DrawBlock");
    }
#endif 
 
    V_MarkRect (x, y, width, height); 
 
    dest = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    {
		memcpy (dest, src, width*sizeof(dt_scrpix)); 
		src += width; 
		dest += SCREENWIDTH; 
    } 
} 
 


//
// V_GetBlock
// Gets a linear block of pixels from the view buffer.
//
void
V_GetBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		dest ) 
{ 
    dt_scrpix	*src;
    int i;
	 
#ifdef RANGECHECK 
    if (x<0
	||x+width >SCREENWIDTH
	|| y<0
	|| y+height>SCREENHEIGHT 
	|| (unsigned)scrn>4 )
    {
	I_Error ("Bad V_DrawBlock");
    }
#endif 
 
    src = screens[scrn] + y*SCREENWIDTH+x; 

    while (height--) 
    {
		if(sizeof(*src)==sizeof(*dest))
		{
			memcpy (dest, src, width*sizeof(*dest)); 
		}else
		{
			for(i=0; i<width; i++)
				dest[i]=src[i];
		}
		src += SCREENWIDTH; 
		dest += width; 
    } 
} 




//
// V_Init
// 
void V_Init (void) 
{ 
    int			i;
    dt_scrpix	*base;
		
    // stick these in low dos memory on PCs

    base = (dt_scrpix *)
//		I_AllocLow (SCREENWIDTH*SCREENHEIGHT*4*sizeof(dt_scrpix));
		I_AllocLow (SCREENWIDTH*SCREENHEIGHT*5*sizeof(dt_scrpix));


	screens_base = base;

    for (i=0 ; i<4 ; i++)
    {
		screens[i] = base + i*SCREENWIDTH*SCREENHEIGHT;
	}

    screens_zbuf = (u16 *)
		I_AllocLow (SCREENWIDTH*SCREENHEIGHT*sizeof(u16));
		
	screen = screens[0];
}

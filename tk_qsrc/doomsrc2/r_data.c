// Emacs style mode select	 -*- C++ -*- 
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
// GNU General Public License for more details.
//
// $Log:$
//
// Revision 1.3	1997/01/29 20:10
// DESCRIPTION:
//	Preparation of data for rendering,
//	generation of lookups, caching, retrieval by name.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: r_data.c,v 1.4 1997/02/03 16:47:55 b1 Exp $";

#include "i_system.h"
#include "i_video.h"

#include "z_zone.h"

#include "m_swap.h"

#include "w_wad.h"

#include "doomdef.h"
#include "r_local.h"
#include "p_local.h"

#include "doomstat.h"
#include "r_sky.h"

// #ifdef LINUX
// #include	<alloca.h>
// #endif


#include "r_data.h"

//
// Graphics.
// DOOM graphics for walls and sprites
// is stored in vertical runs of opaque pixels (posts).
// A column is composed of zero or more posts,
// a patch or sprite is composed of zero or more columns.
// 



//
// Texture definition.
// Each texture is composed of one or more patches,
// with patches being lumps stored in the WAD.
// The lumps are referenced by number, and patched
// into the rectangular texture space using origin
// and possibly other attributes.
//
typedef struct
{
	short	originx;
	short	originy;
	short	patch;
	short	stepdir;
	short	colormap;
} mappatch_t;


//
// Texture definition.
// A DOOM wall texture is a list of patches
// which are to be combined in a predefined order.
//
typedef struct
{
	char		name[8];
//	boolean		masked;	
	short		flags;	//BGB
	char		scalex;
	char		scaley;
	short		width;
	short		height;
//	void		**columndirectory;	// OBSOLETE
	int			columndirectory;
	short		patchcount;
	mappatch_t	patches[1];
} maptexture_t;


// A single patch from a texture definition,
//	basically a rectangular area within
//	the texture rectangle.
typedef struct
{
	// Block origin (allways UL),
	// which has allready accounted
	// for the internal origin of the patch.
	int		originx;	
	int		originy;
	int		patch;
} texpatch_t;


// A maptexturedef_t describes a rectangular texture,
//	which is composed of one or more mappatch_t structures
//	that arrange graphic patches.
typedef struct
{
	// Keep name for switch changing, etc.
	char	name[8];		
	short	width;
	short	height;
	short	next;		//BGB: next texture in hash chain
	
	// All the patches[patchcount]
	//	are drawn back to front into the cached texture.
	short	patchcount;
	texpatch_t	patches[1];		
	
} texture_t;



int		firstflat;
int		lastflat;
int		numflats;

int		firstpatch;
int		lastpatch;
int		numpatches;

int		firstspritelump;
int		lastspritelump;
int		numspritelumps;

int		numtextures;
texture_t**	textures;

short	texturehash[64];


int*			texturewidthmask;
// needed for texture pegging
fixed_t*		textureheight;		
int*			texturecompositesize;
short**			texturecolumnlump;
unsigned short**	texturecolumnofs;
byte**			texturecomposite;

int*			texutx_widthmask;
short**			texutx_columnlump;
short**			texutx_columnofs;
u64**			texutx_composite;
// int*			texutx_compositesize;

// for global animation
int*		flattranslation;
int*		texturetranslation;

// byte		**flattransptr;
// int		*flattransptrlump;


// needed for pre rendering
fixed_t*	spritewidth;	
fixed_t*	spriteoffset;
fixed_t*	spritetopoffset;

lighttable_t	*colormaps;

byte			cmap_luma[64];

lighttable_t	*colormaps_alt[8];
int				colormaps_aidx[8];
int				n_colormaps_alt;
u16				d_8to16table_alt[8][256];

lighttable_t	*colormaps_blend;
lighttable_t	*colormaps_blend_base;
int				colormaps_blend_ix;
int				colormaps_blend_len;
int				colormaps_blend_flash;

void		**patchcache;
void		**utxcache;

#ifndef BGBCC_FOURCC
#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif



//
// MAPTEXTURE_T CACHING
// When a texture is first needed,
//	it counts the number of composite columns
//	required in the texture and allocates space
//	for a column directory and any new columns.
// The directory will simply point inside other patches
//	if there is only one patch in a given column,
//	but any columns with multiple patches
//	will have new column_ts generated.
//



//
// R_DrawColumnInCache
// Clip and draw a column
//	from a patch into a cached post.
//
void
R_DrawColumnInCache
( column_t*	patch,
	byte*		cache,
	int		originy,
	int		cacheheight )
{
	int		count;
	int		position;
	byte*	source;
	byte*	dest;

#if 0
	cache[0]=0;
	cache[1]=cacheheight;
	
	cache[cacheheight+4]=0xff;
	cache[cacheheight+5]=0x00;
#endif

	dest = (byte *)cache + 3;
	
	while (patch->topdelta != 0xff)
	{
		source = (byte *)patch + 3;
		count = patch->length;
		position = originy + patch->topdelta;

		if (position < 0)
		{
			count += position;
			position = 0;
		}

		if (position + count > cacheheight)
			count = cacheheight - position;

		if (count > 0)
			memcpy (cache + position, source, count);
			
		patch = (column_t *)(	(byte *)patch + patch->length + 4); 
	}
}



//
// R_GenerateComposite
// Using the texture definition,
//	the composite texture is created from the patches,
//	and each column is cached.
//
void R_GenerateComposite (int texnum)
{
	byte		*block;
	byte		*tdest;
	texture_t*		texture;
	texpatch_t*		patch;	
	patch_t*		realpatch;
	int			x;
	int			x1;
	int			x2;
	int			i;
	column_t*		patchcol;
	short*		collump;
	unsigned short*	colofs;
	
	texture = textures[texnum];

	block = Z_Malloc (texturecompositesize[texnum],
		PU_STATIC, 
		&texturecomposite[texnum]);	

	collump = texturecolumnlump[texnum];
	colofs = texturecolumnofs[texnum];

#if 0
	//BGB: was this something I added?...
	for (x=0 ; x<texture->width ; x++)
	{
		if (collump[x] >= 0)
			continue;

#if 1
		tdest=block + colofs[x];
		tdest[0]=0x00;
		tdest[1]=texture->height;
		
		tdest[(texture->height)+4]=0xFF;
		tdest[(texture->height)+5]=0x00;
#endif
	}
#endif

	// Composite the columns together.
	patch = texture->patches;
		
	for (i=0 , patch = texture->patches;
		i<texture->patchcount;
		i++, patch++)
	{
//		realpatch = W_CacheLumpNum (patch->patch, PU_CACHE);
		realpatch = W_CachePatchNum (patch->patch, PU_CACHE);
		x1 = patch->originx;
//		x2 = x1 + SHORT(realpatch->width);
		x2 = x1 + (realpatch->width);

		if (x1<0)
			x = 0;
		else
			x = x1;
		
		if (x2 > texture->width)
			x2 = texture->width;

		for ( ; x<x2 ; x++)
		{
			// Column does not have multiple patches?
			if (collump[x] >= 0)
				continue;
			
			patchcol = (column_t *)((byte *)realpatch
//						+ LONG(realpatch->columnofs[x-x1]));
						+ (realpatch->columnofs[x-x1]));
			R_DrawColumnInCache (patchcol,
					 block + colofs[x],
					 patch->originy,
					 texture->height);
		}					
	}

	// Now that the texture has been built in column cache,
	//	it is purgable from zone memory.
	Z_ChangeTag (block, PU_CACHE);
}

void R_GenerateCompositeUtx (int texnum)
{
	u16			tblk[16];
	u64				*block, *ct;
	byte			*src;
	texture_t*		texture;
	texpatch_t*		patch;	
	patch_t*		realpatch;
	int			x, y, xs, ys, xs1, ys1, xs1b, ys1b;
	int			x1, x2, ofs;
	int			i, j, k;
	column_t	*pcol0, *pcol1, *pcol2, *pcol3;
	byte		*tcs0, *tcs1, *tcs2, *tcs3;
	short*		collump;
	unsigned short*	colofs;
	u64			blk;
	
	texture = textures[texnum];

	xs = texture->width;
	ys = texture->height;
	xs1 = xs>>2;
	ys1 = ys>>2;
	xs1b = (xs+3)>>2;
	ys1b = (ys+3)>>2;

	block = Z_Malloc (xs1b*ys1b*8,
		PU_STATIC, 
		&texutx_composite[texnum]);

#if 0	
	src = texturecomposite[texnum];

	if(!src)
	{
//		R_GenerateComposite(texnum);
	}
#endif

	ct = block;

	for(x=0; x<xs1b; x++)
	{
		texutx_columnofs[texnum][x] = ct - block;

#if 1
		tcs0 = R_GetColumn(texnum, x*4+0);
		tcs1 = R_GetColumn(texnum, x*4+1);
		tcs2 = R_GetColumn(texnum, x*4+2);
		tcs3 = R_GetColumn(texnum, x*4+3);
#endif

		for(y=0; y<ys1; y++)
		{
			tblk[ 0]=colormaps[tcs0[0]];	tblk[ 4]=colormaps[tcs0[1]];
			tblk[ 8]=colormaps[tcs0[2]];	tblk[12]=colormaps[tcs0[3]];
			tblk[ 1]=colormaps[tcs1[0]];	tblk[ 5]=colormaps[tcs1[1]];
			tblk[ 9]=colormaps[tcs1[2]];	tblk[13]=colormaps[tcs1[3]];
			tblk[ 2]=colormaps[tcs2[0]];	tblk[ 6]=colormaps[tcs2[1]];
			tblk[10]=colormaps[tcs2[2]];	tblk[14]=colormaps[tcs2[3]];
			tblk[ 3]=colormaps[tcs3[0]];	tblk[ 7]=colormaps[tcs3[1]];
			tblk[11]=colormaps[tcs3[2]];	tblk[15]=colormaps[tcs3[3]];
		
			blk = W_EncodeBlockUtx2(tblk);
			*ct++ = blk;
		
			tcs0+=4;	tcs1+=4;
			tcs2+=4;	tcs3+=4;
		}
		if(ys1<ys1b)
		{
			for(i=0; i<(ys&3); i++)
			{
				tblk[i*4+0]=colormaps[tcs0[i]];
				tblk[i*4+1]=colormaps[tcs1[i]];
				tblk[i*4+2]=colormaps[tcs2[i]];
				tblk[i*4+3]=colormaps[tcs3[i]];
			}
			for(; i<4; i++)
			{
				tblk[i*4+0]=tblk[(i-1)*4+0];
				tblk[i*4+1]=tblk[(i-1)*4+1];
				tblk[i*4+2]=tblk[(i-1)*4+2];
				tblk[i*4+3]=tblk[(i-1)*4+3];
			}

			blk = W_EncodeBlockUtx2(tblk);
			*ct++ = blk;
		}
	}

	Z_ChangeTag (block, PU_CACHE);
}

//
// R_GenerateLookup
//
void R_GenerateLookup (int texnum)
{
	texture_t*		texture;
	byte*		patchcount;	// patchcount[texture->width]
	texpatch_t*		patch;	
	patch_t*		realpatch;
	int			x;
	int			x1;
	int			x2;
	int			i;
	short*		collump;
	unsigned short*	colofs;
	
	texture = textures[texnum];

	// Composited texture not created yet.
	texturecomposite[texnum] = 0;
	
	texturecompositesize[texnum] = 0;
	collump = texturecolumnlump[texnum];
	colofs = texturecolumnofs[texnum];
	
	if(texture->width<0)
		__debugbreak();
	if(texture->height<0)
		__debugbreak();
	
	// Now count the number of columns
	//	that are covered by more than one patch.
	// Fill in the lump / offset, so columns
	//	with only a single patch are all done.
//	patchcount = (byte *)alloca (texture->width);
	patchcount = (byte *)malloc (texture->width);
	memset (patchcount, 0, texture->width);
	patch = texture->patches;
		
	for (i=0 , patch = texture->patches;
		i<texture->patchcount;
		i++, patch++)
	{
//		realpatch = W_CacheLumpNum (patch->patch, PU_CACHE);
		realpatch = W_CachePatchNum (patch->patch, PU_CACHE);
		x1 = patch->originx;
//		x2 = x1 + SHORT(realpatch->width);
		x2 = x1 + (realpatch->width);
		
		if (x1 < 0)
			x = 0;
		else
			x = x1;

		if (x2 > texture->width)
			x2 = texture->width;
		for ( ; x<x2 ; x++)
		{
			patchcount[x]++;
			collump[x] = patch->patch;
//			colofs[x] = LONG(realpatch->columnofs[x-x1])+3;
			colofs[x] = (realpatch->columnofs[x-x1])+3;
		}
	}
	
	for (x=0 ; x<texture->width ; x++)
	{
		if (!patchcount[x])
		{
			printf ("R_GenerateLookup: column without a patch (%s)\n",
				texture->name);
			return;
		}
		// I_Error ("R_GenerateLookup: column without a patch");
		
		if (patchcount[x] > 1)
		{
			// Use the cached block.
			collump[x] = -1;	
			colofs[x] = texturecompositesize[texnum];
			
			if (texturecompositesize[texnum] > 0x10000-texture->height)
			{
			I_Error ("R_GenerateLookup: texture %i is >64k",
				 texnum);
			}
			
			texturecompositesize[texnum] += texture->height;
//			texturecompositesize[texnum] += texture->height+6;
		}
	}

//	texturecompositesize[texnum] += texture->height+6;
}




//
// R_GetColumn
//
byte *R_GetColumn
(	int		tex,
	int		col )
{
	byte	*src;
	int		lump;
	int		ofs;
	
//	if(texturewidthmask[tex]<2)
//	{
//		__debugbreak();
//	}
	
	col &= texturewidthmask[tex];
	lump = texturecolumnlump[tex][col];
	ofs = texturecolumnofs[tex][col];
	
	if (lump > 0)
	{
		src = patchcache[lump];
		if(src)
			return(src+ofs);

//		return (byte *)W_CacheLumpNum(lump,PU_CACHE)+ofs;
		return (byte *)W_CachePatchNum(lump,PU_CACHE)+ofs;
	}

	src = texturecomposite[tex];
	if (src)
		return(src + ofs);
	R_GenerateComposite (tex);
	return texturecomposite[tex] + ofs;

#if 0
	if (!texturecomposite[tex])
	{
		R_GenerateComposite (tex);
//		return(NULL);
	}
	return texturecomposite[tex] + ofs;
#endif
}


u64 *R_GetColumnUtx
(	int		tex,
	int		col )
{
	u64		*src;
	int		lump;
	int		ofs;
		
	col &= texutx_widthmask[tex];
	lump = texutx_columnlump[tex][col];
	ofs = texutx_columnofs[tex][col];

#if 0
	if (lump > 0)
	{
		src = patchcache[lump];
		if(src)
			return(src+ofs);

//		return (byte *)W_CacheLumpNum(lump,PU_CACHE)+ofs;
		return (byte *)W_CachePatchNum(lump,PU_CACHE)+ofs;
	}
#endif

	src = texutx_composite[tex];
	if (src)
		return(src + ofs);
	R_GenerateCompositeUtx (tex);

	ofs = texutx_columnofs[tex][col];
	return texutx_composite[tex] + ofs;
}




//
// R_InitTextures
// Initializes the texture list
//	with the textures from the world map.
//
void R_InitTextures (void)
{
	maptexture_t*	mtexture;
	texture_t*		texture;
	mappatch_t*		mpatch;
	texpatch_t*		patch;

	int			i;
	int			j;
	int			k;

	int*		maptex;
	int*		maptex2;
	int*		maptex1;
	
	char		name[19];
	char*		names;
	char*		name_p;
	
	int*		patchlookup;
	
	int			totalwidth;
	int			nummappatches;
	int			offset;
	int			maxoff;
	int			maxoff2;
	int			numtextures1;
	int			numtextures2;

	int*		directory;
	
	int			temp1;
	int			temp2;
	int			temp3;

//	tk_puts("R_IT\n");

	// Load the patch names from pnames.lmp.
	name[8] = 0;	
	names = W_CacheLumpName ("PNAMES", PU_STATIC);
	
	if(!names)
	{
		I_Error("R_InitTextures: Failed to load PNAMES");
	}
	
	nummappatches = LONG ( *((int *)names) );
	name_p = names+4;
//	patchlookup = alloca (nummappatches*sizeof(*patchlookup));
	patchlookup = malloc (nummappatches*sizeof(int));
	
	for (i=0 ; i<nummappatches ; i++)
	{
		strncpy (name,name_p+i*8, 8);
		patchlookup[i] = W_CheckNumForName (name);
	}
	Z_Free (names);
	
	// Load the map texture definitions from textures.lmp.
	// The data is contained in one or two lumps,
	//	TEXTURE1 for shareware, plus TEXTURE2 for commercial.
	maptex = maptex1 = W_CacheLumpName ("TEXTURE1", PU_STATIC);
	numtextures1 = LONG(*maptex);
	maxoff = W_LumpLength (W_GetNumForName ("TEXTURE1"));
	directory = maptex+1;
	
	if (W_CheckNumForName ("TEXTURE2") != -1)
	{
		maptex2 = W_CacheLumpName ("TEXTURE2", PU_STATIC);
		numtextures2 = LONG(*maptex2);
		maxoff2 = W_LumpLength (W_GetNumForName ("TEXTURE2"));
	}
	else
	{
		maptex2 = NULL;
		numtextures2 = 0;
		maxoff2 = 0;
	}
	numtextures = numtextures1 + numtextures2;
	
	textures = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texturecolumnlump = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texturecolumnofs = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texturecomposite = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texturecompositesize = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texturewidthmask = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	textureheight = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);

#ifdef UTXWALLS
	texutx_columnlump = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texutx_columnofs = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texutx_composite = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
//	texutx_compositesize = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
	texutx_widthmask = Z_Malloc (numtextures*sizeof(void*), PU_STATIC, 0);
#endif

	totalwidth = 0;
	
	printf("\n");
	
	//	Really complex printing shit...
	temp1 = W_GetNumForName ("S_START");	// P_???????
	temp2 = W_GetNumForName ("S_END") - 1;
//	temp3 = ((temp2-temp1+63)/64) + ((numtextures+63)/64);
	temp3 = ((temp2-temp1+63)>>6) + ((numtextures+63)>>6);
	printf("[");
	for (i = 0; i < temp3; i++)
		printf(" ");
	printf("		 ]");
//	for (i = 0; i < temp3; i++)
//		printf("\x8");
//	printf("\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8");	

	printf("\r[");

	//BGB: keep hash chains for texture lookups.
	for(i=0; i < 64; i++)
		texturehash[i] = -1;
	
	for (i=0 ; i<numtextures ; i++, directory++)
	{
		if (!(i&63))
			printf (".");

		if (i == numtextures1)
		{
			// Start looking in second texture file.
			maptex = maptex2;
			maxoff = maxoff2;
			directory = maptex+1;
		}
			
		offset = LONG(*directory);

		if (offset > maxoff)
			I_Error ("R_InitTextures: bad texture directory");
		
		mtexture = (maptexture_t *) ( (byte *)maptex + offset);

		texture = textures[i] =
			Z_Malloc (sizeof(texture_t)
					+ sizeof(texpatch_t)*(SHORT(mtexture->patchcount)-1),
					PU_STATIC, 0);
		
		texture->width = SHORT(mtexture->width);
		texture->height = SHORT(mtexture->height);
		texture->patchcount = SHORT(mtexture->patchcount);

//		memcpy (texture->name, mtexture->name, sizeof(texture->name));

		w_strupr_n(texture->name, mtexture->name, sizeof(texture->name));
		
//		j = W_HashIndexForName(texture->name);
//		texture->next = texturehash[j];
//		texturehash[j] = i;
		
		mpatch = &mtexture->patches[0];
		patch = &texture->patches[0];

		for (j=0 ; j<texture->patchcount ; j++, mpatch++, patch++)
		{
			patch->originx = SHORT(mpatch->originx);
			patch->originy = SHORT(mpatch->originy);
			patch->patch = patchlookup[SHORT(mpatch->patch)];
			if (patch->patch == -1)
			{
			I_Error ("R_InitTextures: Missing patch in texture %s",
				 texture->name);
			}
		}		
//		texturecolumnlump[i] = Z_Malloc (texture->width*2, PU_STATIC,0);
//		texturecolumnofs[i] = Z_Malloc (texture->width*2, PU_STATIC,0);

		texturecolumnlump[i] = Z_Malloc (
			texture->width*sizeof(int), PU_STATIC,0);
		texturecolumnofs[i] = Z_Malloc (
			texture->width*sizeof(int), PU_STATIC,0);

#ifdef UTXWALLS
		texutx_columnlump[i] = Z_Malloc (
			((texture->width+3)/4)*sizeof(int), PU_STATIC, 0);
		texutx_columnofs[i] = Z_Malloc (
			((texture->width+3)/4)*sizeof(int), PU_STATIC, 0);
#endif

		j = 1;
		while (j*2 <= texture->width)
			j<<=1;

		texturewidthmask[i] = j-1;
		textureheight[i] = texture->height<<FRACBITS;

#ifdef UTXWALLS
		texutx_widthmask[i] = (j-1)>>2;
#endif
			
		totalwidth += texture->width;

		j = W_HashIndexForName(texture->name);
		texture->next = texturehash[j];
		texturehash[j] = i;
	}

	Z_Free (maptex1);
	if (maptex2)
		Z_Free (maptex2);
	
	// Precalculate whatever possible.	
	for (i=0 ; i<numtextures ; i++)
		R_GenerateLookup (i);
	
	// Create translation table for global animation.
	texturetranslation = Z_Malloc ((numtextures+1)*sizeof(int), PU_STATIC, 0);
	
	for (i=0 ; i<numtextures ; i++)
		texturetranslation[i] = i;
}

//
// R_InitFlats
//
void R_InitFlats (void)
{
	int		i;
	
	firstflat = W_GetNumForName ("F_START") + 1;
	lastflat = W_GetNumForName ("F_END") - 1;
	numflats = lastflat - firstflat + 1;
	
	// Create translation table for global animation.
//	flattranslation = Z_Malloc ((numflats+1)*4, PU_STATIC, 0);
	flattranslation = Z_Malloc ((numflats+1)*sizeof(int), PU_STATIC, 0);
//	flattransptr = Z_Malloc ((numflats+1)*sizeof(byte *), PU_STATIC, 0);
//	flattransptrlump = Z_Malloc ((numflats+1)*sizeof(int), PU_STATIC, 0);
	
	for (i=0 ; i<numflats ; i++)
		flattranslation[i] = i;
}

void R_DecodeColorCell8(byte *cs, byte *ct, int xs)
{
	int ca, cb, px;

	ca=cs[0];	cb=cs[1];
	px=((u16 *)cs)[1];
	
//	ca=0x55;
//	cb=0xAA;
//	px=0x5A5A;
	
	ct[0]=(px&0x0001)?ca:cb;
	ct[1]=(px&0x0002)?ca:cb;
	ct[2]=(px&0x0004)?ca:cb;
	ct[3]=(px&0x0008)?ca:cb;
	ct+=xs;
	ct[0]=(px&0x0010)?ca:cb;
	ct[1]=(px&0x0020)?ca:cb;
	ct[2]=(px&0x0040)?ca:cb;
	ct[3]=(px&0x0080)?ca:cb;
	ct+=xs;
	ct[0]=(px&0x0100)?ca:cb;
	ct[1]=(px&0x0200)?ca:cb;
	ct[2]=(px&0x0400)?ca:cb;
	ct[3]=(px&0x0800)?ca:cb;
	ct+=xs;
	ct[0]=(px&0x1000)?ca:cb;
	ct[1]=(px&0x2000)?ca:cb;
	ct[2]=(px&0x4000)?ca:cb;
	ct[3]=(px&0x8000)?ca:cb;
	ct+=xs;
}

void *W_CacheFlatNum(int flat, int tag)
{
	void *src, *buf;
	byte *cs, *cse, *cs1e, *ct;
	int lump, size, srci, flat2;

	if(flat<=0)
		return(NULL);
	if(flat>=numlumps)
		return(NULL);

	flat2 = flattranslation[flat];
//	lump = firstflat + flattranslation[flat];
	lump = firstflat + flat2;

	if(lump<=0)
		return(NULL);
	if(lump>=numlumps)
		return(NULL);

//	src = flattransptr[flat2];
//	srci = flattransptrlump[flat2];
	src = patchcache[lump];
	if(src)
		return(src);

	if(tag == PU_STATIC)
		tag = PU_FLAT;

	size = lumpinfo[lump].size;
	
	if(size == 1024)
	{
		if(!src)
//			src = Z_Malloc (4096, tag, &flattransptr[flat2]);
			src = Z_Malloc (4096, tag, &patchcache[lump]);
		buf = W_CacheLumpNum(lump, PU_CACHE);
//		flattransptrlump[flat2] = lump;
		
		cs = buf;	ct = src;	cse = cs + 1024;
		while(cs < cse)
		{
			cs1e=cs+64;
			while(cs<cs1e)
			{
				R_DecodeColorCell8(cs, ct, 64);
				cs+=4;
				ct+=4;
			}
			ct+=3*64;
		}

		return(src);
	}

	if(size == 4096)
	{
		if(!src)
//			src = Z_Malloc (4096, tag, &flattransptr[flat2]);
			src = Z_Malloc (4096, tag, &patchcache[lump]);
		buf = W_CacheLumpNum(lump, PU_CACHE);
		memcpy(src, buf, 4096);
		return(src);
	}

	__debugbreak();
	src = W_CacheLumpNum(lump, tag);
	return(src);
}

int W_UtxPixelGetY(u16 pix)
{
	int cr, cg, cb, cy;
	cr=(pix>>10)&31;
	cg=(pix>> 5)&31;
	cb=(pix>> 0)&31;
	cr=(cr<<3)|(cr>>2);
	cg=(cg<<3)|(cg>>2);
	cb=(cb<<3)|(cb>>2);
	cy=(8*cg+5*cr+3*cb)>>4;
	return(cy);
}

u64 W_EncodeBlockUtx2(u16 *pix)
{
	byte pxy[16];
	u64 blkv;
	u16 px, mpx, npx;
	u32 pxb;
	int mcy, ncy, acy, rcy, cy, al;
	int i, j, k;
	
	mcy=999; ncy=-999;	mpx=0; npx=0; al=0;
//	acy=0;
	for(i=0; i<16; i++)
	{
		px=pix[i];
		if(px&0x8000)
			{ al=1; continue; }
		cy=W_UtxPixelGetY(px);
		pxy[i]=cy;
//		acy+=cy;
		if(cy<mcy)	{ mcy=cy; mpx=px; }
		if(cy>ncy)	{ ncy=cy; npx=px; }
	}
//	acy>>=4;
	acy=(mcy+ncy)>>1;
	rcy=(4*4096)/((ncy-mcy)+1);

	pxb=0;
	for(i=0; i<16; i++)
	{
		cy = pxy[i];
		if(al)
		{
			px=pix[i];
			j=0;
			if(cy>acy)
				j |= 2;
			if(!(px&0x8000))
				j |= 1;
		}else
		{
			j = (((cy-acy)*rcy)>>12)+2;
			if(j < 0) j = 0;
			if(j > 3) j = 3;
		}
		pxb |= j<<(i*2);
	}
	
	mpx&=0x7FFF;
	npx&=0x7FFF;
	if(al)
	{
		mpx&=0x7BDE;
		npx&=0x7BDE;
		mpx|=0x8000;
		npx|=0x0421;
	}
	
	blkv = (((u64)pxb)<<32) | (((u64)mpx)<<16) | (((u64)npx)<<0);
	return(blkv);
}

void *W_CacheUtxFlatNum(int flat, int tag)
{
	static byte morttab4[16] = {
		0x00, 0x01, 0x04, 0x05,	0x10, 0x11, 0x14, 0x15,
		0x40, 0x41, 0x44, 0x45,	0x50, 0x51, 0x54, 0x55 };
	u16		tblk[16];
	byte *src, *buf;
	byte *cs, *cse, *cs1e, *ct;
	u64 blk;
	int lump, size, srci, flat2;
	int cx, cy, cz;

	if(flat<=0)
		return(NULL);
	if(flat>=numlumps)
		return(NULL);

	flat2 = flattranslation[flat];
//	lump = firstflat + flattranslation[flat];
	lump = firstflat + flat2;

	if(lump<=0)
		return(NULL);
	if(lump>=numlumps)
		return(NULL);

//	src = flattransptr[flat2];
//	srci = flattransptrlump[flat2];
	src = utxcache[lump];
	if(src)
		return(src);

	buf = W_CacheFlatNum (flat, PU_CACHE);
	src = Z_Malloc (2048, tag, &utxcache[lump]);

	for(cy=0; cy<16; cy++)
		for(cx=0; cx<16; cx++)
	{
		cs = buf + ((cy*4*64) + (cx*4));
		cz = (morttab4[cy]<<1) | morttab4[cx];

		tblk[ 0] = colormaps[cs[0*64+0]];
		tblk[ 1] = colormaps[cs[0*64+1]];
		tblk[ 4] = colormaps[cs[0*64+2]];
		tblk[ 5] = colormaps[cs[0*64+3]];

		tblk[ 2] = colormaps[cs[1*64+0]];
		tblk[ 3] = colormaps[cs[1*64+1]];
		tblk[ 6] = colormaps[cs[1*64+2]];
		tblk[ 7] = colormaps[cs[1*64+3]];

		tblk[ 8] = colormaps[cs[2*64+0]];
		tblk[ 9] = colormaps[cs[2*64+1]];
		tblk[12] = colormaps[cs[2*64+2]];
		tblk[13] = colormaps[cs[2*64+3]];

		tblk[10] = colormaps[cs[3*64+0]];
		tblk[11] = colormaps[cs[3*64+1]];
		tblk[14] = colormaps[cs[3*64+2]];
		tblk[15] = colormaps[cs[3*64+3]];
		
		blk = W_EncodeBlockUtx2(tblk);
		((u64 *)src)[cz] = blk;
	}

	utxcache[lump] = src;
	return(src);
}

void R_ColorCellPostDecode8(byte *cs, byte *ct)
{
	int ca, cb, px;

	ca=cs[0];
	cb=cs[1];
	px=cs[2];
		
	ct[0]=(px&0x01)?ca:cb;
	ct[1]=(px&0x02)?ca:cb;
	ct[2]=(px&0x04)?ca:cb;
	ct[3]=(px&0x08)?ca:cb;
	ct[4]=(px&0x10)?ca:cb;
	ct[5]=(px&0x20)?ca:cb;
	ct[6]=(px&0x40)?ca:cb;
	ct[7]=(px&0x80)?ca:cb;
}

int R_ColorCellPostDecodeC8(byte *cs, byte *ct, int len)
{
	int l;
	
	l=len;
	while(l>0)
	{
		R_ColorCellPostDecode8(cs, ct);
		cs+=3;	ct+=8;	l-=8;
	}
	return(0);
}

int R_ColorCellPostDecodeC16(byte *cs, byte *ct, int len)
{
	int l;
	
	l=len;
	while(l>0)
	{
		R_DecodeColorCell8(cs, ct, 4);
		cs+=4;	ct+=16;	l-=16;
	}
	return(0);
}

void *W_CachePatchNumUtx(int lump, int tag)
{
//	u16			tblk[128*4];
	u16			tblk[224*4];
	void		*src, *buf;
	byte		*csbuf, *ctbuf, *cs, *ct;
	u64			*blka;
	u64			blk;
	u32			*csofs;
	u16			*csofs16;
    patch_t		*spatch, *tpatch;
	column_t	*tcol;
    int			x, x1, col, xs, ys, xs1, ys1;
    int			i, j, k, l, y;

	if(lump<=0)
		return(NULL);
	if(lump>=numlumps)
		return(NULL);

	src = utxcache[lump];
	if(src)
		return(src);

	spatch = W_CachePatchNum(lump, PU_CACHE);
	if(!spatch)
		return(NULL);
	
	xs = spatch->width;
	ys = spatch->height;
	xs1 = (xs+3)>>2;
//	ys1 = (ys+3)>>2;
	ys1 = (ys+7)>>2;
	
	tpatch = Z_Malloc (sizeof(patch_t)+(xs1*ys1*8),
		PU_STATIC, &utxcache[lump]);
	*tpatch = *spatch;
	
	blka = (u64 *)(tpatch+1);
	
	for(x=0; x<xs1; x++)
	{
		for(i=0; i<ys1*16; i++)
			tblk[i]=0x8000;

		for(x1=0; x1<4; x1++)
		{
			col = x*4+x1;

			if(col >= spatch->width)
				break;

			tcol = (column_t *) ((byte *)spatch +
				(spatch->columnofs[col]));
			while(tcol->topdelta!=0xFF)
			{
				l = tcol->length;
				y = tcol->topdelta;
				for(i=0; i<l; i++)
					{ tblk[(y+i)*4+x1]=colormaps[((byte *)tcol)[3+i]]; }
//				tcol = ((byte *)tcol) + l + 4;
				tcol = (column_t *)((byte *)tcol + tcol->length + 4);
			}
		}
		
		for(y=0; y<ys1; y++)
		{
			blk = W_EncodeBlockUtx2(tblk + y*4*4);
			blka[x*ys1+y] = blk;
		}
	}
	
	return(tpatch);
}

void *W_CachePatchNum(int lump, int tag)
{
	void		*src, *buf;
	byte		*csbuf, *ctbuf, *cs, *ct;
	u32			*csofs;
	u16			*csofs16;
    patch_t		*spatch, *tpatch;
    int			size, size2, xs, ys, xs1, ys1, xs2, ys2, cctag;
    int			x, y, py, pl, ofs, is8, is16, israw, isofs16;
    int			isflat;

	if(lump<=0)
		return(NULL);
	if(lump>=numlumps)
		return(NULL);

	src = patchcache[lump];
	if(src)
		return(src);

	if(tag == PU_STATIC)
		tag = PU_PATCH;

	size = lumpinfo[lump].size;

	buf = W_CacheLumpNum(lump, PU_CACHE);

	if(!buf)
	{
		return(NULL);
	}

	csbuf = buf;
	spatch = (patch_t *)csbuf;
	xs=SHORT(spatch->width);
	ys=SHORT(spatch->height);

	size2= 8 + (xs+1)*4;
//	csofs = csbuf+8;
	csofs = (u32 *)(spatch->columnofs);
	csofs16 = (u16 *)csofs;

	for(x=0; x<xs; x++)
	{
//		ofs=spatch->columnofs[x];
		ofs=LONG(csofs[x]);
		if(ofs>>28)
			break;
	}
	
	if(SHORT(0x1234)!=0x1234)
		x=-1;

//	if(memcmp(buf, "CP8A", 4))
	if(x>=xs)
	{
		src = Z_Malloc (size+256, tag, &patchcache[lump]);
//		src = Z_Malloc (size*2+256, tag, &patchcache[lump]);
		memcpy(src, buf, size);
		return(src);
	}

//	__debugbreak();

	isofs16 = 0;
	isflat = 0;

	ofs = LONG(csofs[0]);
	if((ofs>>28)&0xC)
		isofs16 = 1;

	if(isofs16)
	{
		ofs = SHORT(csofs16[0]);
		if((ofs&0x3FFF)<(8+(xs*2)))
		{
			isflat = 1;
			if((ofs>>14)==1)	is8=1;
			if((ofs>>14)==2)	is16=1;
			if((ofs>>14)==3)	israw=1;
		}
	}

#if 1
	if(isflat)
	{
//		xs1=(xs+3)&(~3);	ys1=(ys+3)&(~3);
		xs2=(xs+3)>>2;		ys2=(ys+3)>>2;
		xs1=xs2<<2;			ys1=ys2<<2;
		size2=8+((xs+1)*4);
		size2+=xs1*(ys1+8);
	}else
	{
		size2=8+((xs+1)*4);
		for(x=0; x<xs; x++)
		{
			if(x>0)
			{
				if(spatch->columnofs[x]==spatch->columnofs[x-1])
					continue;
			}

			if(isofs16)
			{
	//			ofs=spatch->columnofs[x];
				ofs=SHORT(csofs16[x]);
				israw=0;	is16=0;		is8=0;
				if((ofs>>14)==1)	is8=1;
				if((ofs>>14)==2)	is16=1;
				if((ofs>>14)==3)	israw=1;
				ofs&=0x3FFF;
			}else
			{
	//			ofs=spatch->columnofs[x];
				ofs=LONG(csofs[x]);
				israw=0;	is16=0;		is8=0;
				if((ofs>>28)==1)	is8=1;
				if((ofs>>28)==2)	is16=1;
				if((ofs>>28)==3)	israw=1;
				ofs&=0x00FFFFFF;
			}

			cs=csbuf+ofs;

			while(1)
			{
				py=cs[0];
				pl=cs[1];
				if(py==0xFF)
					{ size2++; break; }
				if(is8)
					{ cs+=2+((pl+7)/8)*3; }
				else if(is16)
					{ cs+=2+((pl+15)/16)*4; }
				else if(israw)
					{ cs+=2+pl; }
				else
					{ cs+=4+pl; }
				size2+=pl+4;
			}
		}
	}
#endif	

//	size2 = ((int *)buf)[1];
	src = Z_Malloc (size2+256, tag, &patchcache[lump]);
//	src = Z_Malloc (size2*2+256, tag, &patchcache[lump]);

	csbuf = buf;
	ctbuf = src;

//	is16=!memcmp(buf, "CP16", 4);

//	memcpy(src, ctbuf+16, 8);

//	spatch = (patch_t *)(csbuf+8);
	spatch = (patch_t *)csbuf;
	tpatch = (patch_t *)ctbuf;

	memcpy(tpatch, spatch, sizeof(patch_t));
	
	tpatch->width=SHORT(spatch->width);
	tpatch->height=SHORT(spatch->height);
	tpatch->leftoffset=SHORT(spatch->leftoffset);
	tpatch->topoffset=SHORT(spatch->topoffset);


//	xs=SHORT(spatch->width);
//	cctag=LONG(patch->columnofs[xs]);

	if(isflat)
	{
		ofs=SHORT(csofs16[0]);
		ofs&=0x3FFF;
		cs=csbuf+ofs;

		ct=ctbuf+8+((xs+1)*4);
		ct+=3;

		for(x=0; x<xs2; x++)
		{
			for(y=0; y<ys2; y++)
			{
				R_DecodeColorCell8(cs, ct, ys1+8);
				cs+=4;
				ct+=4;
			}
			ct+=8;
			ct+=3*(ys1+8);
		}

		py=SHORT(csofs16[1])&255;
		pl=ys;

		ct=ctbuf+8+((xs+1)*4);
		for(x=0; x<xs; x++)
		{
			tpatch->columnofs[x]=ct-ctbuf;
			ct[0]=py;
			ct[1]=pl;
			ct[2]=ct[3];
			ct[pl+3]=ct[pl+2];
			ct[pl+4]=0xFF;
			ct[pl+5]=0xFF;
			ct[pl+6]=0xFF;
			ct[pl+7]=0xFF;
			ct+=ys1+8;
		}
	}else
	{
		ct=ctbuf+8+((xs+1)*4);
		for(x=0; x<xs; x++)
		{
			if(x>0)
			{
				if(spatch->columnofs[x]==spatch->columnofs[x-1])
				{
					tpatch->columnofs[x]=tpatch->columnofs[x-1];
					continue;
				}
			}

			if(isofs16)
			{
	//			ofs=spatch->columnofs[x];
				ofs=SHORT(csofs16[x]);
				israw=0;	is16=0;		is8=0;
				if((ofs>>14)==1)	is8=1;
				if((ofs>>14)==2)	is16=1;
				if((ofs>>14)==3)	israw=1;
				ofs&=0x3FFF;
			}else
			{
	//			ofs=spatch->columnofs[x];
				ofs=LONG(csofs[x]);
				israw=0;	is16=0;		is8=0;
				if((ofs>>28)==1)	is8=1;
				if((ofs>>28)==2)	is16=1;
				if((ofs>>28)==3)	israw=1;
//				if((ofs>>28)==0)	israw=3;
				ofs&=0x00FFFFFF;
			}

			cs=csbuf+ofs;
			tpatch->columnofs[x]=ct-ctbuf;

			while(1)
			{
				py=cs[0];
				pl=cs[1];
				if(py==0xFF)
				{
					*ct++=0xFF;
					break;
				}
				
				ct[0]=py;
				ct[1]=pl;
				
				if(is8)
				{
					R_ColorCellPostDecodeC8(cs+2, ct+3, pl);
					cs+=2+((pl+7)/8)*3;
				}
				else if(is16)
				{
					R_ColorCellPostDecodeC16(cs+2, ct+3, pl);
					cs+=2+((pl+15)/16)*4;
				}
				else if(israw)
				{
					memcpy(ct+3, cs+2, pl);
					cs+=2+pl;
				}else
				{
					memcpy(ct+3, cs+3, pl);
					cs+=4+pl;
				}

				ct[2]=ct[3];
				ct[pl+3]=ct[pl+2];
				
				ct+=pl+4;
			}
		}
	}

//	src = W_CacheLumpNum(lump, tag);
	return(src);
}

void *W_CachePatchName(const char *name, int tag)
{
	int lump;

	lump=W_GetNumForName (name);
	return(W_CachePatchNum(lump, tag));
}

void *W_CacheFlatName(const char *name, int tag)
{
	int lump;

	lump=W_GetNumForName (name);
	return(W_CacheFlatNum(lump-firstflat, tag));
}

//
// R_InitSpriteLumps
// Finds the width and hoffset of all sprites in the wad,
//	so the sprite does not need to be cached completely
//	just for having the header info ready during rendering.
//
void R_InitSpriteLumps (void)
{
	int		i, j, k;
	patch_t	*patch;
	
	firstspritelump = W_GetNumForName ("S_START") + 1;
	lastspritelump = W_GetNumForName ("S_END") - 1;
	
	j=W_CheckNumForNameBase(firstspritelump-1, "S_END");
	k=W_CheckNumForNameBase(firstspritelump-1, "S_START");
	while((j>0) && (k>0))
	{
		lastspritelump=j-1;
		firstspritelump=k+1;
		j=W_CheckNumForNameBase(firstspritelump-1, "S_END");
		k=W_CheckNumForNameBase(firstspritelump-1, "S_START");
	}
	
	numspritelumps = lastspritelump - firstspritelump + 1;
	spritewidth = Z_Malloc (
		(numspritelumps+16)*sizeof(fixed_t), PU_STATIC, 0);
	spriteoffset = Z_Malloc (
		(numspritelumps+16)*sizeof(fixed_t), PU_STATIC, 0);
	spritetopoffset = Z_Malloc (
		(numspritelumps+16)*sizeof(fixed_t), PU_STATIC, 0);
	
	for (i=0 ; i< numspritelumps ; i++)
	{
		if (!(i&63))
			printf (".");

//		patch = W_CacheLumpNum (firstspritelump+i, PU_CACHE);
		patch = W_CachePatchNum (firstspritelump+i, PU_CACHE);
//		spritewidth[i] = SHORT(patch->width)<<FRACBITS;
		spritewidth[i] = (patch->width)<<FRACBITS;
//		spriteoffset[i] = SHORT(patch->leftoffset)<<FRACBITS;
		spriteoffset[i] = (patch->leftoffset)<<FRACBITS;
//		spritetopoffset[i] = SHORT(patch->topoffset)<<FRACBITS;
		spritetopoffset[i] = (patch->topoffset)<<FRACBITS;
	}
}


unsigned short	d_8to16table[256];

int vid_flashblend;

//
// R_InitColormaps
//
void R_InitColormaps (void)
{
	byte *pal;
	byte *tbuf;
	lighttable_t *tcol;
	int	lump, length, blen, hdl, lump1, hdl1;
	int cr, cg, cb, cr2, cg2, cb2, dr, dg, db;
	int i, j, k, l, n;

	pal = W_CacheLumpName ("PLAYPAL", PU_CACHE);
	I_SetPalette (pal);

	// Load in the light tables, 
	//	256 byte align tables.
	lump = W_GetNumForName("COLORMAP"); 
	blen = W_LumpLength (lump);
	length = blen + 255; 
	
	hdl=W_LumpHandle(lump);

	lump1=lump;
	hdl1=W_LumpHandle(lump1);
	while(lump1>0)
	{
		lump1--;
		hdl1=W_LumpHandle(lump1);
		if(hdl1!=hdl)
			{ lump1++; break; }
	}
	
	tbuf=malloc(length);

	colormaps = Z_Malloc ((length+256)*sizeof(lighttable_t), PU_STATIC, 0); 
	colormaps = (lighttable_t *)( ((nlint)colormaps + 255)&(~0xff)); 

	colormaps_blend = Z_Malloc (
		(length+256)*sizeof(lighttable_t), PU_STATIC, 0); 
	colormaps_blend = (lighttable_t *)
		( ((nlint)colormaps_blend + 255)&(~0xff)); 
	colormaps_blend_ix = -1;
	colormaps_blend_flash = -1;
	colormaps_blend_base = NULL;
	colormaps_blend_len = length;

//lighttable_t	*colormaps_blend;
//int				colormaps_blend_ix;
//int				colormaps_blend_flash;

	colormaps_alt[0]=colormaps;
	colormaps_aidx[0]=lump1;
	for(j=0; j<256; j++)
		d_8to16table_alt[0][j]=d_8to16table[j];
	n_colormaps_alt=1;

//	W_ReadLump (lump,colormaps);
	W_ReadLump (lump,tbuf);
	
	for(i=0; i<blen; i++)
	{
//		colormaps[i] = tbuf[i];
		colormaps[i] = d_8to16table[tbuf[i]];
	}

	n=length/256;

	for(i=0; i<64; i++)
		cmap_luma[i] = 0;

//	for(i=0; i<64; i++)
	for(i=0; i<n; i++)
	{
		l = 0;
		for(j=0; j<256; j++)
		{
			k=tbuf[i*256+j];
			k=(2*pal[k*3+1]+pal[k*3+0]+pal[k*3+2])/4;
			l+=k;
		}
		cmap_luma[i] = l/256;
	}
	
	l=65536/cmap_luma[0];
	for(i=0; i<64; i++)
	{
		j = cmap_luma[i];
		j = (j * l) >> 8;
		if(j>255)
			j = 255;
		cmap_luma[i] = j;
	}

#if 1
//	for(i=0; i<64; i++)
	for(i=0; i<32; i++)
	{
		l=cmap_luma[i];
		for(j=0; j<256; j++)
		{
			k=colormaps[j];
			cr=(k>>10)&31;
			cg=(k>> 5)&31;
			cb=(k>> 0)&31;

			cr=(cr*l+127)>>8;
			cg=(cg*l+127)>>8;
			cb=(cb*l+127)>>8;
			if(cr>31)cr=31;
			if(cg>31)cg=31;
			if(cb>31)cb=31;

			k=colormaps[i*256+j];
			cr2=(k>>10)&31;
			cg2=(k>> 5)&31;
			cb2=(k>> 0)&31;
			
			dr=cr2-cr;
			dg=cg2-cg;
			db=cb2-cb;
			dr=dr^(dr>>31);
			dg=dg^(dg>>31);
			db=db^(db>>31);
			k=dr+dg+db;
			if(k>12)
			{
				cr=cr2;
				cg=cg2;
				cb=cb2;
			}else
			{
//				cr=(cr+cr2)>>1;
//				cg=(cg+cg2)>>1;
//				cb=(cb+cb2)>>1;
			}
						
			k=(cr<<10)|(cg<<5)|cb;
			colormaps[i*256+j] = k;
			
//			colormaps[i] = d_8to16table[tbuf[i]];
		}
	}
#endif

	lump = W_GetNumForNameBase(lump, "COLORMAP"); 
	while(lump>0)
	{
		I_SetPalette (W_CacheLumpNameBase (lump, "PLAYPAL",PU_CACHE));

		blen = W_LumpLength (lump);
		length = blen + 255; 

		hdl=W_LumpHandle(lump);

		lump1=lump;
		hdl1=W_LumpHandle(lump1);
		while(lump1>0)
		{
			lump1--;
			hdl1=W_LumpHandle(lump1);
			if(hdl1!=hdl)
				{ lump1++; break; }
		}
		
//		tbuf=malloc(length);

		tcol = Z_Malloc (length*sizeof(lighttable_t), PU_STATIC, 0); 
		tcol = (lighttable_t *)( ((nlint)tcol + 255)&~0xff); 

		colormaps_alt[n_colormaps_alt]=tcol;
		colormaps_aidx[n_colormaps_alt]=lump1;
		for(j=0; j<256; j++)
			d_8to16table_alt[n_colormaps_alt][j]=d_8to16table[j];
		n_colormaps_alt++;

	//	W_ReadLump (lump,colormaps);
		W_ReadLump (lump,tbuf);
		
		for(i=0; i<blen; i++)
		{
	//		tcol[i] = tbuf[i];
			tcol[i] = d_8to16table[tbuf[i]];
		}

		lump = W_GetNumForNameBase(lump, "COLORMAP"); 
	}
}

lighttable_t *R_ColormapForLumpI(int lump, int lvl)
{
	lighttable_t *tcol;
	int i, j, k;

	if(n_colormaps_alt<=1)
		return(NULL);
	if(lump>colormaps_aidx[0])
		return(NULL);
		
	for(i=0; i<n_colormaps_alt; i++)
	{
		if(lump>colormaps_aidx[i])
			break;
	}
	
	if(i<n_colormaps_alt)
	{
		tcol=colormaps_alt[i];
		return(tcol+(lvl*256));
	}
	
	return(NULL);

//	byte *tbuf;
}

lighttable_t *R_ColormapForLump(int lump, int lvl)
{
	lighttable_t *basecm;
	int i, j, k;

	return(R_ColormapForLumpI(lump, lvl));

#if 0
	if(!vid_flashblend)
	{
		return(R_ColormapForLumpI(lump, lvl));
	}

	basecm=R_ColormapForLumpI(lump, lvl);
	if(!basecm)
		basecm=colormaps;
		
	if(colormaps_blend_base==basecm)
	{
		if(vid_flashblend==colormaps_blend_flash)
			return(colormaps_blend);
	}
	
	colormaps_blend_base=basecm;
	colormaps_blend_flash=vid_flashblend;
	
	for(i=0; i<8192; i+=4)
	{
		*(u64 *)(colormaps_blend+i) =
			VID_BlendFlash4x(*(u64 *)(basecm+i), vid_flashblend);
	}

	return(colormaps_blend);
#endif
}

lighttable_t *R_ColormapRemapForBlend(lighttable_t *cmap)
{
	lighttable_t *basecm;
	int ofs;
	int i, j, k;

	if(!vid_flashblend)
	{
		return(cmap);
	}
	
	ofs=cmap-colormaps;
	if((ofs>=0) && (ofs<colormaps_blend_len))
	{
		basecm=colormaps;
		
		if(colormaps_blend_base==basecm)
		{
			if(vid_flashblend==colormaps_blend_flash)
				return(colormaps_blend+ofs);
		}
		
		colormaps_blend_base=basecm;
		colormaps_blend_flash=vid_flashblend;
		
		for(i=0; i<colormaps_blend_len; i+=4)
		{
			*(u64 *)(colormaps_blend+i) =
				VID_BlendFlash4x(*(u64 *)(basecm+i), vid_flashblend);
		}

		return(colormaps_blend+ofs);
	}
	
	return(cmap);
}


//
// R_InitData
// Locates all the lumps
//	that will be used by all views
// Must be called after W_Init.
//
void R_InitData (void)
{
//	tk_puts("TI ");
//	printf ("\nIT");
	R_InitTextures ();
	printf ("\nInitTextures");
	R_InitFlats ();
	printf ("\nInitFlats");
	R_InitSpriteLumps ();
	printf ("\nInitSprites");
	R_InitColormaps ();
	printf ("\nInitColormaps");
}



//
// R_FlatNumForName
// Retrieval, get a flat number for a flat name.
//
int R_FlatNumForName (char* name)
{
	int		i;
	char	namet[9];

	memcpy (namet, name, 8);
	namet[8] = 0;

//	if(!strcmp(name, "F_SKY"))
	if(!strcmp(namet, "F_SKY"))
		return(skyflatnum);

//	i = W_CheckNumForName (name);
	i = W_CheckNumForName (namet);

	if (i == -1)
	{
//		namet[8] = 0;
//		memcpy (namet, name,8);
//		I_Error ("R_FlatNumForName: %s not found",namet);
		return(skyflatnum);	//BGB: debug
	}
	return i - firstflat;
}



//
// R_CheckTextureNumForName
// Check whether texture is available.
// Filter out NoTexture indicator.
//
int	R_CheckTextureNumForName (char *name)
{
	char tname[9];
	int		i, h, n;

	// "NoTexture" marker.
	if (name[0] == '-')		
		return 0;
	
	w_strupr_n(tname, name, 8);
	h = W_HashIndexForName(tname);
	n = numtextures;

#if 1
	i = texturehash[h];
	while(i>=0)
//	while((i>=0) && (n>=0))
	{
		if (!memcmp (textures[i]->name, tname, 8) )
			return(i);
		i = textures[i]->next;

		if(n<0)
			break;
		n--;
	}
#endif

#if 1
	if(n<0)
	{
		for (i=0 ; i<numtextures ; i++)
		{
	//		if (!strnicmp (textures[i]->name, name, 8) )
			if (!memcmp (textures[i]->name, tname, 8) )
				return i;
		}
	}
#endif
		
	return -1;
}



//
// R_TextureNumForName
// Calls R_CheckTextureNumForName,
//	aborts with error message.
//
int	R_TextureNumForName (char* name)
{
	char	tname[9];
	int		i;
	
	memcpy(tname, name, 8);
	tname[8]=0;
	
	i = R_CheckTextureNumForName (tname);

	if (i==-1)
	{
//	I_Error ("R_TextureNumForName: %s not found",
//		 tname);
		return(0);
	}
	return i;
}




//
// R_PrecacheLevel
// Preloads all relevant graphics for the level.
//
int		flatmemory;
int		texturememory;
int		spritememory;

void R_PrecacheLevel (void)
{
	char*		flatpresent;
	char*		texturepresent;
	char*		spritepresent;

	int			i;
	int			j;
	int			k;
	int			lump;
	
	texture_t*		texture;
	thinker_t*		th;
	spriteframe_t*	sf;

	if (demoplayback)
		return;
	
	// Precache flats.
	flatpresent = malloc(numflats);
	memset (flatpresent,0,numflats);	

	for (i=0 ; i<numsectors ; i++)
	{
		flatpresent[sectors[i].floorpic] = 1;
		flatpresent[sectors[i].ceilingpic] = 1;
	}
	
	flatmemory = 0;

	for (i=0 ; i<numflats ; i++)
	{
		if (flatpresent[i])
		{
			lump = firstflat + i;
			flatmemory += lumpinfo[lump].size;
			W_CacheLumpNum(lump, PU_CACHE);
		}
	}
	
	// Precache textures.
	texturepresent = malloc(numtextures);
	memset (texturepresent,0, numtextures);
	
	for (i=0 ; i<numsides ; i++)
	{
		texturepresent[sides[i].toptexture] = 1;
		texturepresent[sides[i].midtexture] = 1;
		texturepresent[sides[i].bottomtexture] = 1;
	}

	// Sky texture is always present.
	// Note that F_SKY1 is the name used to
	//	indicate a sky floor/ceiling as a flat,
	//	while the sky texture is stored like
	//	a wall texture, with an episode dependend
	//	name.
	texturepresent[skytexture] = 1;
	
	texturememory = 0;
	for (i=0 ; i<numtextures ; i++)
	{
		if (!texturepresent[i])
			continue;

		texture = textures[i];
		
		for (j=0 ; j<texture->patchcount ; j++)
		{
			lump = texture->patches[j].patch;
			texturememory += lumpinfo[lump].size;
			W_CacheLumpNum(lump , PU_CACHE);
		}
	}
	
	// Precache sprites.
	spritepresent = malloc(numsprites);
	memset (spritepresent,0, numsprites);
	
	for (th = thinkercap.next ; th != &thinkercap ; th=th->next)
	{
		if (th->function.acp1 == (actionf_p1)P_MobjThinker)
			spritepresent[((mobj_t *)th)->sprite] = 1;
	}
	
	spritememory = 0;
	for (i=0 ; i<numsprites ; i++)
	{
		if (!spritepresent[i])
			continue;

		for (j=0 ; j<sprites[i].numframes ; j++)
		{
			sf = &sprites[i].spriteframes[j];

			for (k=0 ; k<8 ; k++)
			{
				lump = firstspritelump + sf->lump[k];
				spritememory += lumpinfo[lump].size;

				W_CacheLumpNum(lump , PU_CACHE);
			}
		}
	}
}





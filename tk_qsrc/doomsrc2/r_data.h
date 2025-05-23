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
//  Refresh module, data I/O, caching, retrieval of graphics
//  by name.
//
//-----------------------------------------------------------------------------


#ifndef __R_DATA__
#define __R_DATA__

#include "r_defs.h"
#include "r_state.h"

#ifdef __GNUG__
#pragma interface
#endif

// Retrieve column data for span blitting.
byte *R_GetColumn (int tex, int col);
u64 *R_GetColumnUtx (int tex, int col);


// I/O, setting up the stuff.
void R_InitData (void);
void R_PrecacheLevel (void);

u64 W_EncodeBlockUtx2(u16 *pix);


// Retrieval.
// Floor/ceiling opaque texture tiles,
// lookup by name. For animation?
int R_FlatNumForName (char* name);

void *W_CacheFlatNum(int flat, int tag);
void *W_CachePatchNum(int lump, int tag);
void *W_CachePatchName(const char *name, int tag);
void *W_CacheFlatName(const char *name, int tag);

void *W_CacheUtxFlatNum(int flat, int tag);
void *W_CachePatchNumUtx(int lump, int tag);


// Called by P_Ticker for switches and animations,
// returns the texture number for the texture name.
int R_TextureNumForName (char *name);
int R_CheckTextureNumForName (char *name);

lighttable_t *R_ColormapForLump(int lump, int lvl);
lighttable_t *R_ColormapRemapForBlend(lighttable_t *cmap);

void R_CheckSpriteLump (int lump);

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

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
// DESCRIPTION:
//	WAD I/O functions.
//
//-----------------------------------------------------------------------------


#ifndef __W_WAD__
#define __W_WAD__


#ifdef __GNUG__
#pragma interface
#endif


//
// TYPES
//
typedef struct
{
    // Should be "IWAD" or "PWAD".
    char		identification[4];		
    int			numlumps;
    int			infotableofs;
    
} wadinfo_t;


typedef struct
{
    int			filepos;
    int			size;
    char		name[8];
    
} filelump_t;

typedef struct
{
	int		filepos;
	int		csize;
	int		dsize;
	char	ety;		//Entry Type
	char	cmp;		//Compression
	short	chn;		//Chain (ExWAD)
	char	name[16];
} wad2lump_t;

//
// WADFILE I/O related stuff.
//
typedef struct
{
//    char	name[8];
    char	name[16];
    int		handle;
    int		position;
    int		size;
    int		chain;
    int		csize;
    char	ety;
    char	cmp;
} lumpinfo_t;


extern	void**		lumpcache;
extern	lumpinfo_t*	lumpinfo;
extern	int		numlumps;

void w_strupr (char *s);
void w_strupr_n (char *t, const char *s, int n);
int w_chkaccess(const char *name);

void    W_InitMultipleFiles (char** filenames);
void    W_Reload (void);

int W_HashIndexForName(char *s);

int	W_CheckNumForName (const char* name);
int W_CheckNumForNameBase (int base, const char* name);

int	W_GetNumForName (const char* name);
int W_GetNumForNameBase (int base, char *name);

int	W_LumpLength (int lump);
void    W_ReadLump (int lump, void *dest);

int W_LumpHandle (int lump);

int W_GetNumForCache (void *ptr);

void*	W_CacheLumpNum (int lump, int tag);
void*	W_CacheLumpName (const char* name, int tag);
void*	W_CacheLumpNameBase (int base, char* name, int tag);


void *W_CacheFlatNum(int flat, int tag);
void *W_CachePatchNum(int lump, int tag);
void *W_CachePatchName(const char *name, int tag);
void *W_CacheFlatName(const char *name, int tag);

void W_Profile (void);



#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

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
//      Zone Memory Allocation, perhaps NeXT ObjectiveC inspired.
//	Remark: this was the only stuff that, according
//	 to John Carmack, might have been useful for
//	 Quake.
//
//---------------------------------------------------------------------



#ifndef __Z_ZONE__
#define __Z_ZONE__

#include <stdio.h>

//
// ZONE MEMORY
// PU - purge tags.
// Tags < 100 are not overwritten until freed.
#define PU_STATIC		1	// static entire execution time
#define PU_SOUND		2	// static while playing
#define PU_MUSIC		3	// static while playing
#define PU_DAVE		4	// anything else Dave wants static

#define PU_PATCH		8	// static patch
#define PU_FLAT			9	// static flat
#define PU_STATICLUMP	10

// #define PU_PATCH		PU_STATIC	// static patch
// #define PU_FLAT			PU_STATIC	// static flat
// #define PU_STATICLUMP	PU_STATIC

#define PU_LEVEL		50	// static until level exited
#define PU_LEVSPEC		51      // a special thinker in a level
#define PU_LEVELMOBJ	52	// static until level exited

#define PU_LEVEL_VTX	53	// static until level exited
#define PU_LEVEL_SEGS	54	// static until level exited
#define PU_LEVEL_SUBS	55	// static until level exited
#define PU_LEVEL_SECT	56	// static until level exited
#define PU_LEVEL_NODE	57	// static until level exited
#define PU_LEVEL_LINES	58	// static until level exited
#define PU_LEVEL_SIDES	59	// static until level exited
#define PU_LEVEL_BMAP	60	// static until level exited
#define PU_LEVEL_GLINE	61	// static until level exited

// Tags >= 100 are purgable whenever needed.
#define PU_PURGELEVEL	100
#define PU_CACHE		101
#define PU_CACHELUMP	102


void	Z_Init (void);
void*	Z_Malloc (int size, int tag, void *ptr);
void    Z_Free (void *ptr);
void    Z_FreeTags (int lowtag, int hightag);
void    Z_DumpHeap (int lowtag, int hightag);
void    Z_FileDumpHeap (FILE *f);
void    Z_CheckHeap (void);
void    Z_ChangeTag2 (void *ptr, int tag);
int     Z_FreeMemory (void);


#if 0
typedef struct memblock_s
{
    int			size;	// including the header and possibly tiny fragments
    void**		user;	// NULL if a free block
    int			tag;	// purgelevel
    int			id;	// should be ZONEID
    struct memblock_s*	next;
    struct memblock_s*	prev;
} memblock_t;
#endif

#if 1
typedef struct memblock_s
{
    int			id;		// 00, should be ZONEID
    int			size;	// 04, including the header and possibly tiny fragments
    int			tag;	// 08, purgelevel
    int			pad_0;	// 0C, pad
    void**		user;	// 10, NULL if a free block
    void *		pad_1;	// 18, pad
    struct memblock_s*	next;	// 20, next pointer
    struct memblock_s*	prev;	// 28, previous pointer
} memblock_t;
#endif

//
// This is used to get the local FILE:LINE info from CPP
// prior to really call the function in question.
//
#define Z_ChangeTag(p,t) \
{ \
      if(p) { if (( (memblock_t *)( (byte *)(p) - sizeof(memblock_t)))->id!=0x1d4a11) \
	  I_Error("Z_CT at "__FILE__":%i",__LINE__); \
	  Z_ChangeTag2(p,t); } \
};



#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

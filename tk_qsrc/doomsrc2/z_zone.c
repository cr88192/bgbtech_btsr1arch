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
//	Zone Memory Allocation. Neat.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: z_zone.c,v 1.4 1997/02/03 16:47:58 b1 Exp $";

#include "z_zone.h"
#include "i_system.h"
#include "doomdef.h"


//
// ZONE MEMORY ALLOCATION
//
// There is never any space between memblocks,
//  and there will never be two contiguous free memblocks.
// The rover can be left pointing at a non-empty block.
//
// It is of no value to free a cachable block,
//  because it will get overwritten automatically if needed.
// 
 
#define ZONEID	0x1d4a11


typedef struct
{
	// total bytes malloced, including header
	int		size;

	// start / end cap for linked list
	memblock_t	blocklist;
	
	memblock_t*	rover;
	
} memzone_t;



memzone_t*	mainzone;

void __setmemtrap(void *ptr, int mode);

// #ifndef __BJX2__
#ifndef __TESTKERN__
void __setmemtrap(void *ptr, int mode)
{
	/* NO-OP */
}
#endif

//
// Z_ClearZone
//
void Z_ClearZone (memzone_t* zone)
{
	memblock_t*		block;
	
	// set the entire zone to one free block
	block = (memblock_t *)( (byte *)zone + sizeof(memzone_t) );
	zone->blocklist.next = block;
	zone->blocklist.prev = block;

	zone->blocklist.user = (void *)zone;
	zone->blocklist.tag = PU_STATIC;
	zone->blocklist.id = ZONEID;
	zone->rover = block;
	
	block->prev = block->next = &zone->blocklist;
	
	// NULL indicates a free block.
	block->user = NULL;	
//	block->id = ZONEID;
	block->id = 0;

	block->size = zone->size - sizeof(memzone_t);
}



//
// Z_Init
//
void Z_Init (void)
{
	memblock_t*	block;
	int		size;

	size = 0;
	mainzone = (memzone_t *)I_ZoneBase (&size);
	mainzone->size = size;

	// set the entire zone to one free block
	block = (memblock_t *)( (byte *)mainzone + sizeof(memzone_t) );
	mainzone->blocklist.next = block;
	mainzone->blocklist.prev = block;

	mainzone->blocklist.user = (void *)mainzone;
	mainzone->blocklist.tag = PU_STATIC;
	mainzone->blocklist.id = ZONEID;
	mainzone->rover = block;

	block->prev = block->next = &mainzone->blocklist;

	// NULL indicates a free block.
	block->user = NULL;
//	block->id = ZONEID;
	block->id = 0;
	
	block->size = mainzone->size - sizeof(memzone_t);

	if(((long)block)&15)
		__debugbreak();
}


//
// Z_Free
//
void Z_Free (void* ptr)
{
	memblock_t*		block;
	memblock_t*		other;
	
	if(!ptr)
		return;
	
	block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));
	
	if(((long)block)&15)
		__debugbreak();
	
	__setmemtrap(block, 2);

	if (block->id != ZONEID)
	{
		__debugbreak();
		I_Error ("Z_Free: freed a pointer without ZONEID");
	}
		
	if (block->user > (void **)0x100)
	{
		// smaller values are not pointers
		// Note: OS-dependend?
		
		// clear the user's mark
//		*block->user = 0;
		*(void **)(block->user) = NULL;
//		__debugbreak();
	}

	// mark as free
	block->user = NULL;	
	block->tag = 0;
	block->id = 0;
	
	other = block->prev;

//	if (!other->user)
	if (other->id == 0)
	{
		__setmemtrap(other, 2);

		// merge with previous free block
		other->size += block->size;
		other->next = block->next;
		other->next->prev = other;

		if (block == mainzone->rover)
			mainzone->rover = other;

		block = other;
	}
	
	other = block->next;

#if 0
	if ( other->prev != block)
	{
		__debugbreak();
//		printf ("ERROR: next block doesn't have proper back link\n");
	}

	if ( other->id && (other->id != ZONEID))
	{
		__debugbreak();
//		printf ("ERROR: next block bad ZONEID\n");
	}
#endif

//	if (!other->user)
	if (other->id == 0)
	{
		__setmemtrap(other, 2);

		// merge the next free block onto the end
		block->size += other->size;
		block->next = other->next;
		block->next->prev = block;

		if (other == mainzone->rover)
			mainzone->rover = block;
	}
}



//
// Z_Malloc
// You can pass a NULL user if the tag is < PU_PURGELEVEL.
//
#define MINFRAGMENT		64


void*
Z_Malloc
( int		size,
  int		tag,
  void*		user )
{
	int		extra, size1;
	memblock_t*	start;
	memblock_t* rover;
	memblock_t* newblock;
	memblock_t*	base;
	void		*ptr;

//	size *= 2;	//BGB: Debug

#if 0
//	if(
//		(tag==PU_STATIC) ||
//		(tag==PU_LEVSPEC) ||
//		(tag==PU_LEVELMOBJ) ||
//		(tag==PU_LEVEL_GLINE)
//		)
	if(
//		(tag!=PU_STATIC) &&
//		(tag!=PU_LEVSPEC) &&
		(tag!=PU_PATCH) &&
		(tag!=PU_FLAT) &&
		(tag!=PU_STATICLUMP) &&
		(tag!=PU_LEVELMOBJ) &&
		(tag!=PU_LEVEL_GLINE)
//		1
		)
	{
		size *= 2;	//BGB: Debug
	}
#endif

//	size = (size + 3) & ~3;
//	size = (size + 7) & ~7;
//	size = (size + 15) & (~15);
	size = (size + 31) & (~15);
//	size = (size + 47) & (~15);
	size1 = size;

	if(((long)size)&15)
		__debugbreak();

	// scan through the block list,
	// looking for the first free block
	// of sufficient size,
	// throwing out any purgable blocks along the way.

	// account for size of block header
	size += sizeof(memblock_t);
	
	// if there is a free block behind the rover,
	//  back up over them
	base = mainzone->rover;
	
	if (!base->prev->user)
		base = base->prev;
	
	rover = base;
	start = base->prev;
	
	do
	{
		if (rover == start)
		{
			// scanned all the way around the list
			I_Error ("Z_Malloc: failed on allocation of %i bytes", size);
		}

#if 1
		if (rover->id && (rover->id != ZONEID))		//BGB
		{
			Z_DumpHeap(0, 127);
			__debugbreak();
		}
#endif

		if (rover->user)
		{
			if (rover->tag < PU_PURGELEVEL)
			{
				// hit a block that can't be purged,
				//  so move base past it
				base = rover = rover->next;
			}
			else
			{
				// free the rover block (adding the size to base)

				// the rover can be the base block
				base = base->prev;
				Z_Free ((byte *)rover+sizeof(memblock_t));
				base = base->next;
				rover = base->next;
			}
		}
		else
			rover = rover->next;
	} while (base->user || base->size < size);


	// found a block big enough
	extra = base->size - size;
	
	if (extra >  MINFRAGMENT)
	{
		// there will be a free fragment after the allocated block
		newblock = (memblock_t *) ((byte *)base + size );
		newblock->size = extra;
		
		// NULL indicates free block.
		newblock->user = NULL;	
		newblock->tag = 0;
		newblock->prev = base;
		newblock->next = base->next;
		newblock->next->prev = newblock;
		newblock->id = 0;

		base->next = newblock;
		base->size = size;
	}
	
	if (user)
	{
		// mark as an in use block
		base->user = user;			
		*(void **)user = (void *) ((byte *)base + sizeof(memblock_t));
	}
	else
	{
		if (tag >= PU_PURGELEVEL)
			I_Error ("Z_Malloc: an owner is required for purgable blocks");

		// mark as in use, but unowned	
		base->user = (void *)2;		
	}
	base->tag = tag;

	// next allocation will start looking here
	mainzone->rover = base->next;	
	
	base->id = ZONEID;

	if(((long)base)&15)
		__debugbreak();

	__setmemtrap(base, 3);
	
	ptr = (void *) ((byte *)base + sizeof(memblock_t));
//	memset(ptr, 0, size1);
	return ptr;

//	return (void *) ((byte *)base + sizeof(memblock_t));
}



//
// Z_FreeTags
//
void
Z_FreeTags
( int		lowtag,
  int		hightag )
{
	memblock_t*	block;
	memblock_t*	next;
	
	for (block = mainzone->blocklist.next ;
	 block != &mainzone->blocklist ;
	 block = next)
	{
	// get link before freeing
	next = block->next;

	// free block?
	if (!block->user)
		continue;
	
	if (block->tag >= lowtag && block->tag <= hightag)
		Z_Free ( (byte *)block+sizeof(memblock_t));
	}
}



//
// Z_DumpHeap
// Note: TFileDumpHeap( stdout ) ?
//
void
Z_DumpHeap
( int		lowtag,
  int		hightag )
{
	memblock_t*	block;
	
	printf ("zone size: %i  location: %p\n",
		mainzone->size,mainzone);
	
	printf ("tag range: %i to %i\n",
		lowtag, hightag);
	
	for (block = mainzone->blocklist.next ; ; block = block->next)
	{
		if (block->id && (block->id != ZONEID))
		{
			printf ("ERROR: block bad ZONEID\n");
			__debugbreak();
		}

		if (block->tag >= lowtag && block->tag <= hightag)
			printf ("block:%p	size:%7i	user:%p	tag:%3i\n",
				block, block->size, block->user, block->tag);
			
		if (block->next == &mainzone->blocklist)
		{
			// all blocks have been hit
			break;
		}
		
		if ( (byte *)block + block->size != (byte *)block->next)
			printf ("ERROR: block size does not touch the next block\n");

		if ( block->next->prev != block)
			printf ("ERROR: next block doesn't have proper back link\n");

		if (!block->user && !block->next->user)
			printf ("ERROR: two consecutive free blocks\n");
	}
}


//
// Z_FileDumpHeap
//
void Z_FileDumpHeap (FILE* f)
{
	memblock_t*	block;
	
	fprintf (f,"zone size: %i  location: %p\n",mainzone->size,mainzone);
	
	for (block = mainzone->blocklist.next ; ; block = block->next)
	{
	fprintf (f,"block:%p	size:%7i	user:%p	tag:%3i\n",
		 block, block->size, block->user, block->tag);
		
	if (block->next == &mainzone->blocklist)
	{
		// all blocks have been hit
		break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
		fprintf (f,"ERROR: block size does not touch the next block\n");

	if ( block->next->prev != block)
		fprintf (f,"ERROR: next block doesn't have proper back link\n");

	if (!block->user && !block->next->user)
		fprintf (f,"ERROR: two consecutive free blocks\n");
	}
}



//
// Z_CheckHeap
//
void Z_CheckHeap (void)
{
	memblock_t*	block;
	
	for (block = mainzone->blocklist.next ; ; block = block->next)
	{
	if (block->next == &mainzone->blocklist)
	{
		// all blocks have been hit
		break;
	}

	if ( (byte *)block + block->size != (byte *)block->next)
		I_Error ("Z_CheckHeap: block size does not touch the next block\n");

	if ( block->next->prev != block)
		I_Error ("Z_CheckHeap: next block doesn't have proper back link\n");

	if (!block->user && !block->next->user)
		I_Error ("Z_CheckHeap: two consecutive free blocks\n");
	}
}




//
// Z_ChangeTag
//
void
Z_ChangeTag2
( void*		ptr,
  int		tag )
{
	if(!ptr)
		return;

	memblock_t*	block;
	memblock_t*	other;
	
	block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

	if(((long)block)&15)
		__debugbreak();

	if (block->id != ZONEID)
	{
		__debugbreak();
		I_Error ("Z_ChangeTag: freed a pointer without ZONEID");
	}

	other = block->next;

#if 1	//BGB: Debug
	if ( other->prev != block)
	{
		__debugbreak();
//		printf ("ERROR: next block doesn't have proper back link\n");
	}

	if ( other->id && (other->id != ZONEID))
	{
		__debugbreak();
//		printf ("ERROR: next block bad ZONEID\n");
	}
#endif

//	if (tag >= PU_PURGELEVEL && (unsigned)block->user < 0x100)
	if (tag >= PU_PURGELEVEL && ((nlint)block->user) < 0x100)
		I_Error ("Z_ChangeTag: an owner is required for purgable blocks");

	__setmemtrap(block, 2);

	block->tag = tag;

	__setmemtrap(block, 3);
}



//
// Z_FreeMemory
//
int Z_FreeMemory (void)
{
	memblock_t*		block;
	int			free;
	
	free = 0;
	
	for (block = mainzone->blocklist.next ;
	 block != &mainzone->blocklist;
	 block = block->next)
	{
	if (!block->user || block->tag >= PU_PURGELEVEL)
		free += block->size;
	}
	return free;
}


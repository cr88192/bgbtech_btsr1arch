/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#ifndef _z_zone_private
#define _z_zone_private

#include "develop.h"

#define MINFRAGMENT     64
#define DPMI_INT  0x31
#define MAXMEMORYSIZE   9000000

#define LEVELZONESIZE   250000

// memory storage data types

#define MEMORYPRETAG   (0x1a2b3c4d)
#define MEMORYPOSTTAG  (0x9f8e7d6b)

#if 0
typedef struct memblock_s
{
#if (MEMORYCORRUPTIONTEST==1)
	int     pretag;
#endif
	int     size;   // including the header and possibly tiny fragments
	void    **user; // NULL if a free block
	int     tag;    // purgelevel
	struct memblock_s   *next, *prev;
#if (MEMORYCORRUPTIONTEST==1)
	int     posttag;
#endif
} memblock_t;
#endif

#if 1
typedef struct memblock_s
{
#if (MEMORYCORRUPTIONTEST==1)
//	int     pretag;
	long     pretag;	//00
#endif
	int     size;   // 04/08, including the header and possibly tiny fragments
	int     tag;    // 08/0C, purgelevel
	void    **user; // 0C/10, NULL if a free block
	struct memblock_s	*next;	//10/18
	struct memblock_s	*prev;	//14/20
#if (MEMORYCORRUPTIONTEST==1)
//	int     posttag;
	long     posttag;			//18/28
#endif
} memblock_t;
#endif

typedef struct
{
	int			size;		// total bytes malloced, including header
	memblock_t	*rover;
	void		*pad0;
	void		*pad1;
	memblock_t	blocklist;	// start / end cap for linked list
} memzone_t;

#endif

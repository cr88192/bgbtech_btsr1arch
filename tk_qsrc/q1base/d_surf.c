/*
Copyright (C) 1996-1997 Id Software, Inc.

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
// d_surf.c: rasterization driver surface heap manager

#include "quakedef.h"
#include "d_local.h"
#include "r_local.h"

float           surfscale;
qboolean        r_cache_thrash;         // set if surface cache is thrashing

int                                     sc_size;
surfcache_t                     *sc_rover, *sc_base;

#define GUARDSIZE       4


int     D_SurfaceCacheForRes (int width, int height)
{
	int             size, pix;

	if (COM_CheckParm ("-surfcachesize"))
	{
		size = Q_atoi(com_argv[COM_CheckParm("-surfcachesize")+1]) * 1024;
		return size;
	}
	
	size = SURFCACHE_SIZE_AT_320X200;

	pix = width*height;
	if (pix > 64000)
		size += (pix-64000)*3;
	
	size = size * r_pixbytes;

//	size *= 2;
	size *= 6;

	return size;
}

void D_CheckCacheGuard (void)
{
	byte    *s;
	int             i;

	s = (byte *)sc_base + sc_size;
	for (i=0 ; i<GUARDSIZE ; i++)
		if (s[i] != (byte)i)
			Sys_Error ("D_CheckCacheGuard: failed");
}

void D_ClearCacheGuard (void)
{
	byte    *s;
	int             i;
	
	s = (byte *)sc_base + sc_size;
	for (i=0 ; i<GUARDSIZE ; i++)
		s[i] = (byte)i;
}


/*
================
D_InitCaches

================
*/
void D_InitCaches (void *buffer, int size)
{

	if (!msg_suppress_1)
		Con_Printf ("%ik surface cache\n", size/1024);

	sc_size = size - GUARDSIZE;
	sc_base = (surfcache_t *)buffer;
	sc_rover = sc_base;
	
	sc_base->next = NULL;
	sc_base->owner = NULL;
	sc_base->size = sc_size;
	
	D_ClearCacheGuard ();
}


/*
==================
D_FlushCaches
==================
*/
void D_FlushCaches (void)
{
	surfcache_t     *c;
	
	if (!sc_base)
		return;
		
	printf("D_FlushCaches\n");

	for (c = sc_base ; c ; c = c->next)
	{
		if (c->owner)
			*c->owner = NULL;
	}
	
	sc_rover = sc_base;
	sc_base->next = NULL;
	sc_base->owner = NULL;
	sc_base->size = sc_size;
}

/*
=================
D_SCAlloc
=================
*/
surfcache_t     *D_SCAlloc (int width, int size)
{
	surfcache_t             *newsc;
	qboolean                wrapped_this_time;
	int i, j, k, sz1;

	if ((width < 0) || (width > 256))
		Sys_Error ("D_SCAlloc: bad cache width %d\n", width);

//	if ((size <= 0) || (size > 0x10000))
	if ((size <= 0) || (size > 0x20000))
		Sys_Error ("D_SCAlloc: bad cache size %d\n", size);
	
//	size = (int)&((surfcache_t *)0)->data[size];
	size = (nlint)&((surfcache_t *)0)->data[size];
	size = (size + 3) & ~3;
	if (size > sc_size)
		Sys_Error ("D_SCAlloc: %i > cache size",size);

// if there is not size bytes after the rover, reset to the start
	wrapped_this_time = false;

	if ( !sc_rover || (byte *)sc_rover - (byte *)sc_base > sc_size - size)
	{
		if (sc_rover)
		{
			wrapped_this_time = true;
		}
		sc_rover = sc_base;
	}
		
// colect and free surfcache_t blocks until the rover block is large enough
	newsc = sc_rover;
	if (sc_rover->owner)
		*sc_rover->owner = NULL;
	
	while (newsc->size < size)
	{
	// free another
		sc_rover = sc_rover->next;
		if (!sc_rover)
			Sys_Error ("D_SCAlloc: hit the end of memory");
		if (sc_rover->owner)
			*sc_rover->owner = NULL;
			
		newsc->size += sc_rover->size;
		newsc->next = sc_rover->next;
	}

// create a fragment out of any leftovers
	if (newsc->size - size > 256)
	{
		sc_rover = (surfcache_t *)( (byte *)newsc + size);
		sc_rover->size = newsc->size - size;
		sc_rover->next = newsc->next;
		sc_rover->width = 0;
		sc_rover->owner = NULL;
		newsc->next = sc_rover;
		newsc->size = size;
	}
	else
		sc_rover = newsc->next;
	
	newsc->width = width;
// DEBUG
	if (width > 0)
	{
//		newsc->height = (size - sizeof(*newsc) + sizeof(newsc->data)) / width;
		newsc->height = (size - sizeof(*newsc) + sizeof(newsc->data)) /
			(width*r_pixbytes);
	}

	newsc->owner = NULL;              // should be set properly after return

	if (d_roverwrapped)
	{
		if (wrapped_this_time || (sc_rover >= d_initial_rover))
			r_cache_thrash = true;
	}
	else if (wrapped_this_time)
	{       
		d_roverwrapped = true;
	}

#ifdef _WIN32
	sz1=newsc->width*newsc->height*r_pixbytes;
	for(i=0; i<(sz1/2); i++)
	{
		((u16 *)(newsc->data))[i]=0x7FFF;
	}
#endif

D_CheckCacheGuard ();   // DEBUG
	return newsc;
}


/*
=================
D_SCDump
=================
*/
void D_SCDump (void)
{
	surfcache_t             *test;

	for (test = sc_base ; test ; test = test->next)
	{
		if (test == sc_rover)
			Sys_Printf ("ROVER:\n");
		printf ("%p : %i bytes     %i width\n",test, test->size, test->width);
	}
}

//=============================================================================

// if the num is not a power of 2, assume it will not repeat

int     MaskForNum (int num)
{
	if (num==128)
		return 127;
	if (num==64)
		return 63;
	if (num==32)
		return 31;
	if (num==16)
		return 15;
	return 255;
}

int D_log2 (int num)
{
	int     c;
	
	c = 0;
	
	while (num>>=1)
		c++;
	return c;
}

//=============================================================================

/*
================
D_CacheSurface
================
*/
surfcache_t *D_CacheSurface (msurface_t *surface, int miplevel)
{
	surfcache_t     *cache;
	int t;

//
// if the surface is animating or flashing, flush the cache
//
	r_drawsurf.texture = R_TextureAnimation (surface->texinfo->texture);
	r_drawsurf.lightadj[0] = d_lightstylevalue[surface->styles[0]];
	r_drawsurf.lightadj[1] = d_lightstylevalue[surface->styles[1]];
	r_drawsurf.lightadj[2] = d_lightstylevalue[surface->styles[2]];
	r_drawsurf.lightadj[3] = d_lightstylevalue[surface->styles[3]];
	
//
// see if the cache holds apropriate data
//
	cache = surface->cachespots[miplevel];

	if(cache && (r_lowfps>1))
		return cache;	//BGB debug

#if 1	//BGB Debug
	if (cache
			&& cache->texture == r_drawsurf.texture
			&& cache->lightadj[0] == r_drawsurf.lightadj[0]
			&& cache->lightadj[1] == r_drawsurf.lightadj[1]
			&& cache->lightadj[2] == r_drawsurf.lightadj[2]
			&& cache->lightadj[3] == r_drawsurf.lightadj[3]
			&& r_lowfps
			)
		return cache;
#endif

#if 0	//BGB perf hack
//	t = ((int) cache)>>4;
	if ( cache && (r_framecount & 3))
//	if ( cache && ((r_framecount^t) & 3))
		return cache;
#endif

#if 1	//BGB Debug
	if (cache
			&& !cache->dlight
			&& (surface->dlightframe != r_framecount)
//			&&  ((surface->dlightframe != r_framecount) || (r_framecount & 3))
			&& cache->texture == r_drawsurf.texture
			&& cache->lightadj[0] == r_drawsurf.lightadj[0]
			&& cache->lightadj[1] == r_drawsurf.lightadj[1]
			&& cache->lightadj[2] == r_drawsurf.lightadj[2]
			&& cache->lightadj[3] == r_drawsurf.lightadj[3]
			)
		return cache;
#endif

//
// determine shape of surface
//

	surfscale = 1.0 / (1<<miplevel);

//	t = (1<<miplevel);
//	surfscale = 1.0 / t;

	r_drawsurf.surfmip = miplevel;
	r_drawsurf.surfwidth = surface->extents[0] >> miplevel;
	r_drawsurf.rowbytes = r_drawsurf.surfwidth * r_pixbytes;
	r_drawsurf.surfheight = surface->extents[1] >> miplevel;
	
//
// allocate memory if needed
//
	if (!cache)     // if a texture just animated, don't reallocate it
	{
		t=	r_drawsurf.surfwidth *
			r_drawsurf.surfheight *
			r_pixbytes ;

#if 0	
		printf("D_CacheSurface width=%d height=%d pixbytes=%d\n",
			r_drawsurf.surfwidth,
			r_drawsurf.surfheight,
			r_pixbytes);
		if(t<=0)
		{
			printf("  %d %d %d\n",
				surface->extents[0],
				surface->extents[1],
				miplevel);
		}
#endif
	
		cache = D_SCAlloc (r_drawsurf.surfwidth,
						   r_drawsurf.surfwidth *
						   r_drawsurf.surfheight *
						   r_pixbytes);
		surface->cachespots[miplevel] = cache;
		cache->owner = &surface->cachespots[miplevel];
		cache->mipscale = surfscale;
	}
	
	if (surface->dlightframe == r_framecount)
		cache->dlight = 1;
	else
		cache->dlight = 0;

	r_drawsurf.surfdat = (pixel_t *)cache->data;
	
	cache->texture = r_drawsurf.texture;
	cache->lightadj[0] = r_drawsurf.lightadj[0];
	cache->lightadj[1] = r_drawsurf.lightadj[1];
	cache->lightadj[2] = r_drawsurf.lightadj[2];
	cache->lightadj[3] = r_drawsurf.lightadj[3];

//
// draw and light the surface texture
//
	r_drawsurf.surf = surface;

	c_surf++;
	R_DrawSurface ();

	return surface->cachespots[miplevel];
}



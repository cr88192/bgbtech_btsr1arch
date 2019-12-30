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
// r_surf.c: surface-related refresh code

#include "quakedef.h"
#include "r_local.h"

drawsurf_t	r_drawsurf;

int				lightleft, sourcesstep, blocksize, sourcetstep;
int				lightdelta, lightdeltastep;
int				lightright, lightleftstep, lightrightstep, blockdivshift;
unsigned		blockdivmask;
void			*prowdestbase;
unsigned char	*pbasesource;
int				surfrowbytes;	// used by ASM files
unsigned		*r_lightptr;
int				r_stepback;
int				r_lightwidth;
int				r_numhblocks, r_numvblocks;
unsigned char	*r_source, *r_sourcemax;


unsigned short *r_vid_colormap16;
//	r_vid_colormap16=vid.colormap16;



void R_DrawSurfaceBlock8_mip0 (void);
void R_DrawSurfaceBlock8_mip1 (void);
void R_DrawSurfaceBlock8_mip2 (void);
void R_DrawSurfaceBlock8_mip3 (void);

static void	(*surfmiptable[4])(void) = {
	R_DrawSurfaceBlock8_mip0,
	R_DrawSurfaceBlock8_mip1,
	R_DrawSurfaceBlock8_mip2,
	R_DrawSurfaceBlock8_mip3
};

void R_DrawSurfaceBlock16_mip0 (void);
void R_DrawSurfaceBlock16_mip1 (void);
void R_DrawSurfaceBlock16_mip2 (void);
void R_DrawSurfaceBlock16_mip3 (void);

static void	(*surfmiptable16[4])(void) = {
	R_DrawSurfaceBlock16_mip0,
	R_DrawSurfaceBlock16_mip1,
	R_DrawSurfaceBlock16_mip2,
	R_DrawSurfaceBlock16_mip3
};



unsigned		blocklights[18*18];

#ifndef __BGBCC__
void	__hint_use_egpr()
{
}
#endif

/*
===============
R_AddDynamicLights
===============
*/
void R_AddDynamicLights (void)
{
	msurface_t *surf;
	int			lnum;
	int			sd, td;
	float		dist, rad, minlight;
	vec3_t		impact, local;
	int			s, t;
	int			i;
	int			smax, tmax;
	mtexinfo_t	*tex;

	surf = r_drawsurf.surf;
	smax = (surf->extents[0]>>4)+1;
	tmax = (surf->extents[1]>>4)+1;
	tex = surf->texinfo;

	for (lnum=0 ; lnum<MAX_DLIGHTS ; lnum++)
	{
		if ( !(surf->dlightbits & (1<<lnum) ) )
			continue;		// not lit by this light

		rad = cl_dlights[lnum].radius;
		dist = DotProduct (cl_dlights[lnum].origin, surf->plane->normal) -
				surf->plane->dist;
		rad -= fabs(dist);
		minlight = cl_dlights[lnum].minlight;
		if (rad < minlight)
			continue;
		minlight = rad - minlight;

		for (i=0 ; i<3 ; i++)
		{
			impact[i] = cl_dlights[lnum].origin[i] -
					surf->plane->normal[i]*dist;
		}

		local[0] = DotProduct (impact, tex->vecs[0]) + tex->vecs[0][3];
		local[1] = DotProduct (impact, tex->vecs[1]) + tex->vecs[1][3];

		local[0] -= surf->texturemins[0];
		local[1] -= surf->texturemins[1];
		
		for (t = 0 ; t<tmax ; t++)
		{
			td = local[1] - t*16;
			if (td < 0)
				td = -td;
			for (s=0 ; s<smax ; s++)
			{
				sd = local[0] - s*16;
				if (sd < 0)
					sd = -sd;
				if (sd > td)
					dist = sd + (td>>1);
				else
					dist = td + (sd>>1);
				if (dist < minlight)
#ifdef QUAKE2
				{
					unsigned temp;
					temp = (rad - dist)*256;
					i = t*smax + s;
					if (!cl_dlights[lnum].dark)
						blocklights[i] += temp;
					else
					{
						if (blocklights[i] > temp)
							blocklights[i] -= temp;
						else
							blocklights[i] = 0;
					}
				}
#else
					blocklights[t*smax + s] += (rad - dist)*256;
#endif
			}
		}
	}
}

/*
===============
R_BuildLightMap

Combine and scale multiple lightmaps into the 8.8 format in blocklights
===============
*/
void R_BuildLightMap (void)
{
	int			smax, tmax;
	int			t;
	int			i, size;
	byte		*lightmap;
	unsigned	scale;
	int			maps;
	msurface_t	*surf;

	surf = r_drawsurf.surf;

	smax = (surf->extents[0]>>4)+1;
	tmax = (surf->extents[1]>>4)+1;
	size = smax*tmax;
	lightmap = surf->samples;

	if (r_fullbright.value || !cl.worldmodel->lightdata)
	{
		for (i=0 ; i<size ; i++)
			blocklights[i] = 0;
		return;
	}

// clear to ambient
	for (i=0 ; i<size ; i++)
		blocklights[i] = r_refdef.ambientlight<<8;


// add all the lightmaps
	if (lightmap)
		for (maps = 0 ; maps < MAXLIGHTMAPS && surf->styles[maps] != 255 ;
			 maps++)
		{
			scale = r_drawsurf.lightadj[maps];	// 8.8 fraction		
			for (i=0 ; i<size ; i++)
				blocklights[i] += lightmap[i] * scale;
			lightmap += size;	// skip to next lightmap
		}

// add all the dynamic lights
	if (surf->dlightframe == r_framecount)
		R_AddDynamicLights ();

// bound, invert, and shift
	for (i=0 ; i<size ; i++)
	{
		t = (255*256 - (int)blocklights[i]) >> (8 - VID_CBITS);

		if (t < (1 << 6))
			t = (1 << 6);

		blocklights[i] = t;
	}
}


/*
===============
R_TextureAnimation

Returns the proper texture for a given time and base texture
===============
*/
texture_t *R_TextureAnimation (texture_t *base)
{
	int		reletive;
	int		count;

#if 1
	if (currententity->frame)
	{
		if (base->alternate_anims)
			base = base->alternate_anims;
	}
#endif
	
	if (!base->anim_total)
		return base;

	reletive = (int)(cl.time*10) % base->anim_total;

	count = 0;	
	while (base->anim_min > reletive || base->anim_max <= reletive)
	{
		base = base->anim_next;
		if (!base)
			Sys_Error ("R_TextureAnimation: broken cycle");
		if (++count > 100)
			Sys_Error ("R_TextureAnimation: infinite cycle");
	}

	return base;
}


/*
===============
R_DrawSurface
===============
*/
void R_DrawSurface (void)
{
	unsigned char	*basetptr;
	int				smax, tmax, twidth;
	int				u;
	int				soffset, basetoffset, texwidth;
	int				horzblockstep;
	unsigned char	*pcolumndest;
	void			(*pblockdrawer)(void);
	int				mip;
	texture_t		*mt;

// calculate the lightings
	R_BuildLightMap ();
	
	surfrowbytes = r_drawsurf.rowbytes;

	mt = r_drawsurf.texture;
	
	mip = r_drawsurf.surfmip;
//	if(mip<0)mip=0;
//	if(mip>3)mip=3;

	r_source = ((byte *)mt) + mt->offsets[r_drawsurf.surfmip];
//	r_source = ((byte *)mt) + mt->offsets[mip];

//	r_source = ((byte *)mt);
//	r_source = ((byte *)mt)+sizeof(texture_t);
	
// the fractional light values should range from 0 to (VID_GRADES - 1) << 16
// from a source range of 0 - 255
	
	texwidth = mt->width >> r_drawsurf.surfmip;
//	texwidth = mt->width >> mip;

	blocksize = 16 >> r_drawsurf.surfmip;
//	blocksize = 16 >> mip;

	blockdivshift = 4 - r_drawsurf.surfmip;
//	blockdivshift = 4 - mip;

	blockdivmask = (1 << blockdivshift) - 1;
	
	r_lightwidth = (r_drawsurf.surf->extents[0]>>4)+1;

	r_numhblocks = r_drawsurf.surfwidth >> blockdivshift;
	r_numvblocks = r_drawsurf.surfheight >> blockdivshift;

//==============================

	if (r_pixbytes == 1)
	{
		pblockdrawer = surfmiptable[r_drawsurf.surfmip];
	// TODO: only needs to be set when there is a display settings change
		horzblockstep = blocksize;
	}
	else
	{
		pblockdrawer = surfmiptable16[r_drawsurf.surfmip];
//		pblockdrawer = R_DrawSurfaceBlock16;
	// TODO: only needs to be set when there is a display settings change
		horzblockstep = blocksize << 1;
	}

	smax = mt->width >> r_drawsurf.surfmip;
//	smax = mt->width >> mip;
	twidth = texwidth;
	tmax = mt->height >> r_drawsurf.surfmip;
//	tmax = mt->height >> mip;
	sourcetstep = texwidth;
	r_stepback = tmax * twidth;
//	r_stepback = tmax * smax;

	r_sourcemax = r_source + (tmax * smax);
//	r_sourcemax = r_source + (tmax * smax * r_pixbytes);

	r_vid_colormap16 = vid.colormap16;

	soffset = r_drawsurf.surf->texturemins[0];
	basetoffset = r_drawsurf.surf->texturemins[1];

// << 16 components are to guarantee positive values for %
	soffset = ((soffset >> r_drawsurf.surfmip) + (smax << 16)) % smax;
	
//	soffset=0;
	
	basetptr = &r_source[((((basetoffset >> r_drawsurf.surfmip) 
		+ (tmax << 16)) % tmax) * twidth)];

//	basetptr = r_source + (((((basetoffset >> r_drawsurf.surfmip) 
//		+ (tmax << 16)) % tmax) * twidth));

//	basetptr = r_source;

	pcolumndest = r_drawsurf.surfdat;

	for (u=0 ; u<r_numhblocks; u++)
	{
		r_lightptr = blocklights + u;

		prowdestbase = pcolumndest;

		pbasesource = basetptr + soffset;
//		pbasesource = basetptr;

//		if(pbasesource<r_source)
//			__debugbreak();
//		if(pbasesource>r_sourcemax)
//			__debugbreak();

		(*pblockdrawer)();

		soffset = soffset + blocksize;
		if (soffset >= smax)
			soffset = 0;

		pcolumndest += horzblockstep;
	}
}


//=============================================================================

// #if	!id386
#if 1

/*
================
R_DrawSurfaceBlock8_mip0
================
*/
void R_DrawSurfaceBlock8_mip0 (void)
{
	int				v, i, b, lightstep, lighttemp, light;
	unsigned char	pix, *psource, *prowdest;

	psource = pbasesource;
	prowdest = prowdestbase;

	for (v=0 ; v<r_numvblocks ; v++)
	{
	// FIXME: make these locals?
	// FIXME: use delta rather than both right and left, like ASM?
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 4;
		lightrightstep = (r_lightptr[1] - lightright) >> 4;

		for (i=0 ; i<16 ; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 4;

			light = lightright;

			for (b=15; b>=0; b--)
			{
				pix = psource[b];
				prowdest[b] = ((unsigned char *)vid.colormap)
						[(light & 0xFF00) + pix];
				light += lightstep;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes;
		}

		if (psource >= r_sourcemax)
			psource -= r_stepback;
	}
}


/*
================
R_DrawSurfaceBlock8_mip1
================
*/
void R_DrawSurfaceBlock8_mip1 (void)
{
	int				v, i, b, lightstep, lighttemp, light;
	unsigned char	pix, *psource, *prowdest;

	psource = pbasesource;
	prowdest = prowdestbase;

	for (v=0 ; v<r_numvblocks ; v++)
	{
	// FIXME: make these locals?
	// FIXME: use delta rather than both right and left, like ASM?
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 3;
		lightrightstep = (r_lightptr[1] - lightright) >> 3;

		for (i=0 ; i<8 ; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 3;

			light = lightright;

			for (b=7; b>=0; b--)
			{
				pix = psource[b];
				prowdest[b] = ((unsigned char *)vid.colormap)
						[(light & 0xFF00) + pix];
				light += lightstep;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes;
		}

		if (psource >= r_sourcemax)
			psource -= r_stepback;
	}
}


/*
================
R_DrawSurfaceBlock8_mip2
================
*/
void R_DrawSurfaceBlock8_mip2 (void)
{
	int				v, i, b, lightstep, lighttemp, light;
	unsigned char	pix, *psource, *prowdest;

	psource = pbasesource;
	prowdest = prowdestbase;

	for (v=0 ; v<r_numvblocks ; v++)
	{
	// FIXME: make these locals?
	// FIXME: use delta rather than both right and left, like ASM?
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 2;
		lightrightstep = (r_lightptr[1] - lightright) >> 2;

		for (i=0 ; i<4 ; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 2;

			light = lightright;

			for (b=3; b>=0; b--)
			{
				pix = psource[b];
				prowdest[b] = ((unsigned char *)vid.colormap)
						[(light & 0xFF00) + pix];
				light += lightstep;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes;
		}

		if (psource >= r_sourcemax)
			psource -= r_stepback;
	}
}


/*
================
R_DrawSurfaceBlock8_mip3
================
*/
void R_DrawSurfaceBlock8_mip3 (void)
{
	int				v, i, b, lightstep, lighttemp, light;
	unsigned char	pix, *psource, *prowdest;

	psource = pbasesource;
	prowdest = prowdestbase;

	for (v=0 ; v<r_numvblocks ; v++)
	{
	// FIXME: make these locals?
	// FIXME: use delta rather than both right and left, like ASM?
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 1;
		lightrightstep = (r_lightptr[1] - lightright) >> 1;

		for (i=0 ; i<2 ; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 1;

			light = lightright;

			for (b=1; b>=0; b--)
			{
				pix = psource[b];
				prowdest[b] = ((unsigned char *)vid.colormap)
						[(light & 0xFF00) + pix];
				light += lightstep;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes;
		}

		if (psource >= r_sourcemax)
			psource -= r_stepback;
	}
}


/*
================
R_DrawSurfaceBlock16

FIXME: make this work
================
*/
#if 0
void R_DrawSurfaceBlock16 (void)
{
	int				k;
	unsigned char	*psource;
	int				lighttemp, lightstep, light;
	unsigned short	*prowdest;

	prowdest = (unsigned short *)prowdestbase;

	for (k=0 ; k<blocksize ; k++)
	{
		unsigned short		*pdest;
		unsigned short		pix;
		int				b;

		psource = pbasesource;
		lighttemp = lightright - lightleft;
		lightstep = lighttemp >> blockdivshift;

		light = lightleft;
		pdest = prowdest;

		for (b=0; b<blocksize; b++)
		{
			pix = *psource;
			pix = vid.colormap16[(light & 0xFF00) + pix];
//			pix = 0x7FFF;
			*pdest = pix;
			psource += sourcesstep;
			pdest++;
			light += lightstep;
		}

		pbasesource += sourcetstep;
		lightright += lightrightstep;
		lightleft += lightleftstep;
//		prowdest = (unsigned short *)((long)prowdest + surfrowbytes);
		prowdest += surfrowbytes>>1;
	}

	prowdestbase = prowdest;
}
#endif

#if 1
void R_DrawSurfaceBlock16_mipN (int mip)
{
	int				v, i, b, l, d;
	int				lighttemp;

//	register int lightstep, light;
//	register unsigned char	*psource;
//	register unsigned short *prowdest;
//	register unsigned short *cmap16;
//	register int pix;

	int lightstep, light;
	unsigned char	*psource;
	unsigned short *prowdest;
	unsigned short *cmap16;
	int pix;

	int pixa, pixb, shr0, shp2, shp2n1;

//	__hint_use_egpr();

	psource = pbasesource;
	prowdest = prowdestbase;

#if 0
	if(psource<r_source)
	{
		__debugbreak();
	}
	if(psource>r_sourcemax)
	{
		__debugbreak();
	}
#endif

	shr0=4-mip;
	shp2=1<<shr0;
	shp2n1=shp2-1;
	cmap16=vid.colormap16;

	for (v=0 ; v<r_numvblocks ; v++)
	{
	// FIXME: make these locals?
	// FIXME: use delta rather than both right and left, like ASM?
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> shr0;
		lightrightstep = (r_lightptr[1] - lightright) >> shr0;

		for (i=0 ; i<shp2; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> shr0;

			light = lightright;

			for (b=shp2n1; b>=0; b--)
			{
				pix = psource[b];
//				prowdest[b] = vid.colormap16[(light & 0xFF00) + pix];
				prowdest[b] = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes/2;
		}

		if (psource >= r_sourcemax)
		{
			psource -= r_stepback;

//			__debugbreak();
//			psource = psource+(-r_stepback);
		}
	}
}

#if 1

#if 0
void R_DrawSurfaceBlock16_mip0 (void)
{
	int				v, i, l, d;
	int				lighttemp;

	register int lightstep, light;
	register unsigned char	*psource;
	register unsigned short *prowdest;
	register unsigned short *cmap16;
	register int pix;
//	register int pix0, pix1, pix2, pix3;
	register int b;

	int pixa, pixb;
//	 shr0, shp2, shp2n1;

	__hint_use_egpr();

	psource = pbasesource;
	prowdest = prowdestbase;

//	shr0=4;
//	shp2=16;
//	shp2n1=15;
	cmap16=vid.colormap16;

	for (v=0 ; v<r_numvblocks ; v++)
	{
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 4;
		lightrightstep = (r_lightptr[1] - lightright) >> 4;

		for (i=0 ; i<16; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 4;
			light = lightright;
			
//			light &= 0xFF00;
//			lightstep &= 0xFF00;

#if 0
			for (b=15; b>=0; b--)
			{
				pix = psource[b];
				prowdest[b] = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
			}
#endif

#if 1
//			for (b=15; b>=0; )
			for (b=15; b>0; )
			{
				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;	b--;

				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;	b--;

				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;	b--;

				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;	b--;
			}
#endif

#if 0
			for (b=15; b>0; )
			{
				pix0 = psource[b-0];
				pix1 = psource[b-1];
				pix2 = psource[b-2];
				pix3 = psource[b-3];

//				pix0 = cmap16[(light & 0xFF00) + pix0];
				pix0 += light & 0xFF00;
				light += lightstep;
				pix1 += light & 0xFF00;
				light += lightstep;
				pix2 += light & 0xFF00;
				light += lightstep;
				pix3 += light & 0xFF00;
				light += lightstep;

				pix0 = cmap16[pix0];
				pix1 = cmap16[pix1];
				pix2 = cmap16[pix2];
				pix3 = cmap16[pix3];
				prowdest[b-0] = pix0;
				prowdest[b-1] = pix1;
				prowdest[b-2] = pix2;
				prowdest[b-3] = pix3;

				b-=4;
			}
#endif
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
//			prowdest += surfrowbytes/2;
			prowdest += surfrowbytes >> 1;
		}

		if (psource >= r_sourcemax)
			{ psource -= r_stepback; }
	}
}
#endif

#if 0
void R_DrawSurfaceBlock16_mip1 (void)
{
	int				v, i, b, l, d;
	int				lighttemp;

	register int lightstep, light;
	register unsigned char	*psource;
	register unsigned short *prowdest;
	register unsigned short *cmap16;
	register int pix;

	int pixa, pixb;
//	int shr0, shp2, shp2n1;

	__hint_use_egpr();

	psource = pbasesource;
	prowdest = prowdestbase;

//	shr0=3;
//	shp2=8;
//	shp2n1=7;
	cmap16=vid.colormap16;

	for (v=0 ; v<r_numvblocks ; v++)
	{
		lightleft = r_lightptr[0];
		lightright = r_lightptr[1];
		r_lightptr += r_lightwidth;
		lightleftstep = (r_lightptr[0] - lightleft) >> 3;
		lightrightstep = (r_lightptr[1] - lightright) >> 3;

		for (i=0 ; i<8; i++)
		{
			lighttemp = lightleft - lightright;
			lightstep = lighttemp >> 3;

			light = lightright;

//			for (b=7; b>=0; )
			for (b=7; b>0; )
			{
				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;
				b--;

				pix = psource[b];
				pix = cmap16[(light & 0xFF00) + pix];
				light += lightstep;
				prowdest[b] = pix;
				b--;
			}
	
			psource += sourcetstep;
			lightright += lightrightstep;
			lightleft += lightleftstep;
			prowdest += surfrowbytes/2;
		}

		if (psource >= r_sourcemax)
			{ psource -= r_stepback; }
	}
}
#endif

#endif

//void R_DrawSurfaceBlock16_mip0 (void)
//	{ R_DrawSurfaceBlock16_mipN(0); }
//void R_DrawSurfaceBlock16_mip1 (void)
//	{ R_DrawSurfaceBlock16_mipN(1); }
void R_DrawSurfaceBlock16_mip2 (void)
	{ R_DrawSurfaceBlock16_mipN(2); }
void R_DrawSurfaceBlock16_mip3 (void)
	{ R_DrawSurfaceBlock16_mipN(3); }

//void R_DrawSurfaceBlock16_mip1 (void)
//	{ R_DrawSurfaceBlock16(); }
//void R_DrawSurfaceBlock16_mip2 (void)
//	{ R_DrawSurfaceBlock16(); }
//void R_DrawSurfaceBlock16_mip3 (void)
//	{ R_DrawSurfaceBlock16(); }

#endif

#endif


//============================================================================

/*
================
R_GenTurbTile
================
*/
void R_GenTurbTile (pixel_t *pbasetex, void *pdest)
{
	int		*turb;
	int		i, j, s, t;
	byte	*pd;
	
	turb = sintable + ((int)(cl.time*SPEED)&(CYCLE-1));
	pd = (byte *)pdest;

	for (i=0 ; i<TILE_SIZE ; i++)
	{
		for (j=0 ; j<TILE_SIZE ; j++)
		{	
			s = (((j << 16) + turb[i & (CYCLE-1)]) >> 16) & 63;
			t = (((i << 16) + turb[j & (CYCLE-1)]) >> 16) & 63;
			*pd++ = *(pbasetex + (t<<6) + s);
		}
	}
}


/*
================
R_GenTurbTile16
================
*/
void R_GenTurbTile16 (pixel_t *pbasetex, void *pdest)
{
	int				*turb;
	int				i, j, s, t;
	unsigned short	*pd;

	turb = sintable + ((int)(cl.time*SPEED)&(CYCLE-1));
	pd = (unsigned short *)pdest;

	for (i=0 ; i<TILE_SIZE ; i++)
	{
		for (j=0 ; j<TILE_SIZE ; j++)
		{	
			s = (((j << 16) + turb[i & (CYCLE-1)]) >> 16) & 63;
			t = (((i << 16) + turb[j & (CYCLE-1)]) >> 16) & 63;
			*pd++ = d_8to16table[*(pbasetex + (t<<6) + s)];
		}
	}
}


/*
================
R_GenTile
================
*/
void R_GenTile (msurface_t *psurf, void *pdest)
{
	if (psurf->flags & SURF_DRAWTURB)
	{
		if (r_pixbytes == 1)
		{
			R_GenTurbTile ((pixel_t *)
				((byte *)psurf->texinfo->texture + psurf->texinfo->texture->offsets[0]), pdest);
		}
		else
		{
			R_GenTurbTile16 ((pixel_t *)
				((byte *)psurf->texinfo->texture + psurf->texinfo->texture->offsets[0]), pdest);
		}
	}
	else if (psurf->flags & SURF_DRAWSKY)
	{
		if (r_pixbytes == 1)
		{
			R_GenSkyTile (pdest);
		}
		else
		{
			R_GenSkyTile16 (pdest);
		}
	}
	else
	{
		Sys_Error ("Unknown tile type");
	}
}


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
// d_edge.c

#include "quakedef.h"
#include "d_local.h"

static int	miplevel;

float		scale_for_mip;
extern int			screenwidth;
int			ubasestep, errorterm, erroradjustup, erroradjustdown;
int			vstartscan;

// FIXME: should go away
extern void			R_RotateBmodel (void);
extern void			R_TransformFrustum (void);

vec3_t		transformed_modelorg;
extern	int		r_lowfps;

#if !defined(_BGBCC) && !defined(__riscv)
int __int_clamp(int v, int min, int max)
{
	if(v<min)return(min);
	if(v>max)return(max);
	return(v);
}
#endif

// #ifndef _BGBCC
#ifndef __BJX2__
float __fpu_frcp_sf(float f)
{
	return(1.0/f);
}

float __fpu_fdiv_sf(float f, float g)
{
	return(f/g);
}

int D_SoftDivB(int a, int b)
{
//	return(D_SoftDivB(a, b));
	return(D_SoftDiv(a, b));
}

// void __hint_use_egpr()
// {
// }

#endif


/*
==============
D_DrawPoly

==============
*/
void D_DrawPoly (void)
{
// this driver takes spans, not polygons
}


/*
=============
D_MipLevelForScale
=============
*/
int D_MipLevelForScale (float scale)
{
	int		lmiplevel;

	if(r_lowfps>1)
//		scale *= 0.66;
		scale *= 0.414;

	if (scale >= d_scalemip[0] )
		lmiplevel = 0;
	else if (scale >= d_scalemip[1] )
		lmiplevel = 1;
	else if (scale >= d_scalemip[2] )
		lmiplevel = 2;
	else
		lmiplevel = 3;

	if (lmiplevel < d_minmip)
		lmiplevel = d_minmip;

	return lmiplevel;
}


/*
==============
D_DrawSolidSurface
==============
*/

// FIXME: clean this up

void D_DrawSolidSurface16 (surf_t *surf, int color);

void D_DrawSolidSurface (surf_t *surf, int color)
{
	espan_t	*span;
	byte	*pdest;
	int		u, u2, pix;
	
	if(r_pixbytes==2)
	{
		D_DrawSolidSurface16(surf,
			d_8to16table[color]);
		return;
	}
	
	pix = (color<<24) | (color<<16) | (color<<8) | color;
	for (span=surf->spans ; span ; span=span->pnext)
	{
		pdest = (byte *)d_viewbuffer + screenwidth*span->v;
		u = span->u;
		u2 = span->u + span->count - 1;
		((byte *)pdest)[u] = pix;

		if (u2 - u < 8)
		{
			for (u++ ; u <= u2 ; u++)
				((byte *)pdest)[u] = pix;
		}
		else
		{
			for (u++ ; u & 3 ; u++)
				((byte *)pdest)[u] = pix;

			u2 -= 4;
			for ( ; u <= u2 ; u+=4)
				*(int *)((byte *)pdest + u) = pix;
			u2 += 4;
			for ( ; u <= u2 ; u++)
				((byte *)pdest)[u] = pix;
		}
	}
}

void D_DrawSolidSurface16 (surf_t *surf, int color)
{
	espan_t	*span;
	u16		*pdest, *ct, *cte, *ct1e;
	int 	*ct32;
	u64 	*ct64;
	u64		pix64;
	int		u, u1, u2, pix;
	int		i;
	
//	pix = (color<<24) | (color<<16) | (color<<8) | color;
	pix = (color<<16) | color;
	pix64	= (((u64)((u32)pix))<<32)|((u32)pix);
	for (span=surf->spans ; span ; span=span->pnext)
	{
		pdest = (u16 *)d_viewbuffer + screenwidth*span->v;
		u = span->u;
		u2 = span->u + span->count - 1;
		pdest[u] = pix;

		if (u2 - u < 8)
		{
			for (u++ ; u <= u2 ; u++)
				pdest[u] = pix;
		}
		else
		{
#if 0
			for (u++ ; u & 3 ; u++)
				pdest[u] = pix;
			u2 -= 4;
			for ( ; u <= u2 ; u+=2)
				*(int *)(pdest + u) = pix;
			u2 += 4;
			for ( ; u <= u2 ; u++)
				pdest[u] = pix;
#endif

#if 1
//			ct = pdest;
			ct = (pdest + u + 1);
			cte = (pdest + u2);
//			u1 = (u2-u)&(~7);
//			ct32 = (int *)(pdest + u);
//			for ( i = 0; i < u1 ; i += 8)
			ct1e=cte-16;
//			while((ct+16)<cte)
			while(ct<ct1e)
			{
//				ct32 = (int *)ct;
//				ct32[0] = pix;	ct32[1] = pix;
//				ct32[2] = pix;	ct32[3] = pix;
//				ct32 += 4;

				ct64 = (u64 *)ct;
				ct64[0] = pix64;
				ct64[1] = pix64;
				ct64[2] = pix64;
				ct64[3] = pix64;

				ct += 16;
			}
//			ct = (u16 *)ct32;
//			u1 = (u2-u)&7;
//			for ( i = 0; i <= u1 ; i ++)
//				ct[u] = pix;

			ct1e=cte-4;
//			while((ct+4)<cte)
			while(ct<ct1e)
			{
				*(u64 *)ct=pix64;
				ct += 4;
			}

			while(ct<=cte)
				*ct++=pix;
#endif
		}
	}
}


// float __fpu_fdiv_sf(float x, float y);
// float __fpu_frcp_sf(float x);

/*
==============
D_CalcGradients
==============
*/
void D_CalcGradients (msurface_t *pface)
{
	mplane_t	*pplane;
	float		mipscale;
	vec3_t		p_temp1;
	vec3_t		p_saxis, p_taxis;
	float f0, f1, f2, f3;
	float		t;
	int j0, j1, j2, j3;
	int i;

	pplane = pface->plane;

//	i = 1 << miplevel;
//	mipscale = 1.0 / ((float)i);
//	mipscale = 1.0 / (float)(1 << miplevel);
//	mipscale = __fpu_frcp_sf((float)(1 << miplevel));
	mipscale = d_frcp_sf((float)(1 << miplevel));

//	tk_printf("mipscape = %f\n", mipscale);

	TransformVector (pface->texinfo->vecs[0], p_saxis);
	TransformVector (pface->texinfo->vecs[1], p_taxis);

//	tk_printf("p_saxis = ( %f %f %f )\n", p_saxis[0], p_saxis[1], p_saxis[2]);
//	tk_printf("p_taxis = ( %f %f %f )\n", p_taxis[0], p_taxis[1], p_taxis[2]);

	t = xscaleinv * mipscale;
	d_sdivzstepu = p_saxis[0] * t;
	d_tdivzstepu = p_taxis[0] * t;

	t = yscaleinv * mipscale;
	d_sdivzstepv = -p_saxis[1] * t;
	d_tdivzstepv = -p_taxis[1] * t;

	d_sdivzorigin = p_saxis[2] * mipscale - xcenter * d_sdivzstepu -
			ycenter * d_sdivzstepv;
	d_tdivzorigin = p_taxis[2] * mipscale - xcenter * d_tdivzstepu -
			ycenter * d_tdivzstepv;

//	tk_printf("%f %f   %f %f   %f %f\n",
//		d_sdivzstepu, d_tdivzstepu,
//		d_sdivzstepv, d_tdivzstepv,
//		d_sdivzorigin, d_tdivzorigin);

	VectorScale (transformed_modelorg, mipscale, p_temp1);
//	p_temp1[0]=0;
//	p_temp1[1]=0;
//	p_temp1[2]=0;

#if 0
	tk_printf("transformed_modelorg = ( %f %f %f )\n",
		transformed_modelorg[0],
		transformed_modelorg[1],
		transformed_modelorg[2]);
	tk_printf("p_temp1 = ( %f %f %f )\n", p_temp1[0], p_temp1[1], p_temp1[2]);
#endif

//	t = 0x10000*mipscale;
	t = mipscale * 65536.0;

#if 0
	f2 = DotProduct (p_temp1, p_saxis);
	f0 = f2 * 0x10000 + 0.5;
	f1 = pface->texinfo->vecs[0][3]*t;
	j0 = (pface->texturemins[0] << 16) >> miplevel;
	
	sadjust = ((fixed16_t)f0) - j0 + ((fixed16_t)f1);
#endif

#if 0
	sadjust = ((fixed16_t)(DotProduct (p_temp1, p_saxis) * 0x10000 + 0.5)) -
			((pface->texturemins[0] << 16) >> miplevel)
			+ pface->texinfo->vecs[0][3]*t;

	tadjust = ((fixed16_t)(DotProduct (p_temp1, p_taxis) * 0x10000 + 0.5)) -
			((pface->texturemins[1] << 16) >> miplevel)
			+ pface->texinfo->vecs[1][3]*t;
//	__debugbreak();
#endif

#if 1
	sadjust = ((fixed16_t)(DotProduct (p_temp1, p_saxis) * 0x10000 + 0.5)) -
			(pface->texturemins[0] << (16-miplevel))
			+ pface->texinfo->vecs[0][3]*t;

	tadjust = ((fixed16_t)(DotProduct (p_temp1, p_taxis) * 0x10000 + 0.5)) -
			(pface->texturemins[1] << (16-miplevel))
			+ pface->texinfo->vecs[1][3]*t;
#endif

//	f0 = ((fixed16_t)(DotProduct (p_temp1, p_saxis) * 0x10000 + 0.5)) -
//			((pface->texturemins[0] << 16) >> miplevel)
//			+ pface->texinfo->vecs[0][3]*t;

//	f1 = ((fixed16_t)(DotProduct (p_temp1, p_taxis) * 0x10000 + 0.5)) -
//			((pface->texturemins[1] << 16) >> miplevel)
//			+ pface->texinfo->vecs[1][3]*t;
//	sadjust = (int)f0;
//	tadjust = (int)f1;

//	sadjust = 0;

//	tk_printf("sadjust = %d\n", sadjust>>16);
//	tk_printf("tadjust = %d\n", tadjust>>16);

//	tk_printf("sadjust = %f\n", sadjust);
//	tk_printf("tadjust = %f\n", tadjust);

//
// -1 (-epsilon) so we never wander off the edge of the texture
//

//	bbextents = ((pface->extents[0] << 16) >> miplevel) - 1;
//	bbextentt = ((pface->extents[1] << 16) >> miplevel) - 1;

	bbextents = (pface->extents[0] << (16-miplevel)) - 1;
	bbextentt = (pface->extents[1] << (16-miplevel)) - 1;
	
//	tk_printf("bbextents = %d\n", bbextents>>16);
//	tk_printf("bbextentt = %d\n", bbextentt>>16);
}


/*
==============
D_DrawSurfaces
==============
*/
void D_DrawSurfaces (void)
{
	surf_t			*s;
	msurface_t		*pface;
	surfcache_t		*pcurrentcache;
	vec3_t			world_transformed_modelorg;
	vec3_t			local_modelorg;

	currententity = &cl_entities[0];
	TransformVector (modelorg, transformed_modelorg);
	VectorCopy (transformed_modelorg, world_transformed_modelorg);

//	return;

//	__debugbreak();

// TODO: could preset a lot of this at mode set time
	if (r_drawflat.value)
	{
		for (s = &surfaces[1] ; s<surface_p ; s++)
		{
			if (!s->spans)
				continue;

			d_zistepu = s->d_zistepu;
			d_zistepv = s->d_zistepv;
			d_ziorigin = s->d_ziorigin;

			D_DrawSolidSurface (s, (nlint)s->data & 0xFF);
			D_DrawZSpans (s->spans);
		}
	}
	else
	{
		for (s = &surfaces[1] ; s<surface_p ; s++)
		{
			if (!s->spans)
				continue;

			r_drawnpolycount++;

			d_zistepu = s->d_zistepu;
			d_zistepv = s->d_zistepv;
			d_ziorigin = s->d_ziorigin;

			if (s->flags & SURF_DRAWSKY)
			{
				if (!r_skymade)
				{
					R_MakeSky ();
				}

				if(r_pixbytes==2)
					D_DrawSkyScans16 (s->spans);
				else
					D_DrawSkyScans8 (s->spans);
				D_DrawZSpans (s->spans);
			}
			else if (s->flags & SURF_DRAWBACKGROUND)
			{
			// set up a gradient for the background surface that places it
			// effectively at infinity distance from the viewpoint
				d_zistepu = 0;
				d_zistepv = 0;
				d_ziorigin = -0.9;

				D_DrawSolidSurface (s, (int)r_clearcolor.value & 0xFF);
				D_DrawZSpans (s->spans);
			}
			else if (s->flags & SURF_DRAWTURB)
			{
				pface = s->data;
				miplevel = 0;
				cacheblock = (pixel_t *)
						((byte *)pface->texinfo->texture +
						pface->texinfo->texture->offsets[0]);
				cachewidth = 64;

				if (s->insubmodel)
				{
				// FIXME: we don't want to do all this for every polygon!
				// TODO: store once at start of frame
					currententity = s->entity;	//FIXME: make this passed in to
												// R_RotateBmodel ()
					VectorSubtract (r_origin, currententity->origin,
							local_modelorg);
					TransformVector (local_modelorg, transformed_modelorg);

					R_RotateBmodel ();	// FIXME: don't mess with the frustum,
										// make entity passed in
				}

				D_CalcGradients (pface);

#if 1
				if(r_pixbytes==2)
					Turbulent16 (s->spans);
				else
					Turbulent8 (s->spans);
				D_DrawZSpans (s->spans);
#endif

				if (s->insubmodel)
				{
				//
				// restore the old drawing state
				// FIXME: we don't want to do this every time!
				// TODO: speed up
				//
					currententity = &cl_entities[0];
					VectorCopy (world_transformed_modelorg,
								transformed_modelorg);
					VectorCopy (base_vpn, vpn);
					VectorCopy (base_vup, vup);
					VectorCopy (base_vright, vright);
					VectorCopy (base_modelorg, modelorg);
					R_TransformFrustum ();
				}
			}
			else
			{
//				continue;		//BGB: debug
			
				if (s->insubmodel)
//				if(0)
				{
				// FIXME: we don't want to do all this for every polygon!
				// TODO: store once at start of frame
					currententity = s->entity;	//FIXME: make this passed in to
												// R_RotateBmodel ()
					VectorSubtract (r_origin, currententity->origin, local_modelorg);
					TransformVector (local_modelorg, transformed_modelorg);

					R_RotateBmodel ();	// FIXME: don't mess with the frustum,
										// make entity passed in
				}

				pface = s->data;
				miplevel = D_MipLevelForScale (s->nearzi * scale_for_mip
				* pface->texinfo->mipadjust);

				if (s->insubmodel && r_lowfps)
					miplevel = 0;

			// FIXME: make this passed in to D_CacheSurface
				pcurrentcache = D_CacheSurface (pface, miplevel);

				cacheblock = (pixel_t *)pcurrentcache->data;
				cachewidth = pcurrentcache->width;

				D_CalcGradients (pface);

				(*d_drawspans) (s->spans);

				D_DrawZSpans (s->spans);

				if (s->insubmodel)
//				if(0)
				{
				//
				// restore the old drawing state
				// FIXME: we don't want to do this every time!
				// TODO: speed up
				//
					currententity = &cl_entities[0];
					VectorCopy (world_transformed_modelorg,
								transformed_modelorg);
					VectorCopy (base_vpn, vpn);
					VectorCopy (base_vup, vup);
					VectorCopy (base_vright, vright);
					VectorCopy (base_modelorg, modelorg);
					R_TransformFrustum ();
				}
			}
		}
	}
}


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

int			skytexturenum;

#ifndef GL_RGBA4
#define	GL_RGBA4	0
#endif


int		lightmap_bytes;		// 1, 2, or 4

int		lightmap_textures;

unsigned		blocklights[18*18];

#define	BLOCK_WIDTH		128
#define	BLOCK_HEIGHT	128

#define	MAX_LIGHTMAPS	64
int			active_lightmaps;

typedef struct glRect_s {
	unsigned char l,t,w,h;
} glRect_t;

glpoly_t	*lightmap_polys[MAX_LIGHTMAPS];
qboolean	lightmap_modified[MAX_LIGHTMAPS];
glRect_t	lightmap_rectchange[MAX_LIGHTMAPS];

int			allocated[MAX_LIGHTMAPS][BLOCK_WIDTH];

// the lightmap texture data needs to be kept in
// main memory so texsubimage can update properly
// byte		lightmaps[4*MAX_LIGHTMAPS*BLOCK_WIDTH*BLOCK_HEIGHT];
byte		*lightmaps = NULL;

// For gl_texsort 0
msurface_t  *skychain = NULL;
msurface_t  *waterchain = NULL;

void R_RenderDynamicLightmaps (msurface_t *fa);

vec3_t	shadevector;
float	shadelight, ambientlight;

qboolean	currententity_world;
qboolean	currententity_world_bmodel;

/*
===============
R_AddDynamicLights
===============
*/
void R_AddDynamicLights (msurface_t *surf)
{
	int			lnum;
	int			sd, td;
	float		dist, rad, minlight;
	vec3_t		impact, local;
	int			s, t;
	int			i;
	int			smax, tmax;
	mtexinfo_t	*tex;

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
					blocklights[t*smax + s] += (rad - dist)*256;
			}
		}
	}
}


#if 0
/*
===============
R_BuildLightMap

Combine and scale multiple lightmaps into the 8.8 format in blocklights
===============
*/
void R_BuildLightMap (msurface_t *surf, byte *dest, int stride)
{
	int			smax, tmax;
	int			t;
	int			i, j, size;
	byte		*lightmap;
	unsigned	scale;
	int			maps;
	int			lightadj[4];
	unsigned	*bl;

	surf->cached_dlight = (surf->dlightframe == r_framecount);

	smax = (surf->extents[0]>>4)+1;
	tmax = (surf->extents[1]>>4)+1;
	size = smax*tmax;
	lightmap = surf->samples;

// set to full bright if no light data
	if (r_fullbright.value || !cl.worldmodel->lightdata)
	{
		for (i=0 ; i<size ; i++)
			blocklights[i] = 255*256;
		goto store;
	}

// clear to no light
	for (i=0 ; i<size ; i++)
		blocklights[i] = 0;

// add all the lightmaps
	if (lightmap)
		for (maps = 0 ; maps < MAXLIGHTMAPS && surf->styles[maps] != 255 ;
			 maps++)
		{
			scale = d_lightstylevalue[surf->styles[maps]];
			surf->cached_light[maps] = scale;	// 8.8 fraction
			for (i=0 ; i<size ; i++)
				blocklights[i] += lightmap[i] * scale;
			lightmap += size;	// skip to next lightmap
		}

// add all the dynamic lights
	if (surf->dlightframe == r_framecount)
		R_AddDynamicLights (surf);

// bound, invert, and shift
store:
	switch (gl_lightmap_format)
	{
	case GL_RGBA:
		stride -= (smax<<2);
		bl = blocklights;
		for (i=0 ; i<tmax ; i++, dest += stride)
		{
			for (j=0 ; j<smax ; j++)
			{
				t = *bl++;
				t >>= 7;
				if (t > 255)
					t = 255;
				dest[3] = 255-t;
				dest += 4;
			}
		}
		break;
	case GL_ALPHA:
	case GL_LUMINANCE:
	case GL_INTENSITY:
		bl = blocklights;
		for (i=0 ; i<tmax ; i++, dest += stride)
		{
			for (j=0 ; j<smax ; j++)
			{
				t = *bl++;
				t >>= 7;
				if (t > 255)
					t = 255;
				dest[j] = 255-t;
			}
		}
		break;
	default:
		Sys_Error ("Bad lightmap format");
	}
}
#endif

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

	if (currententity->frame)
	{
		if (base->alternate_anims)
			base = base->alternate_anims;
	}
	
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
=============================================================

	BRUSH MODELS

=============================================================
*/


extern	int		solidskytexture;
extern	int		alphaskytexture;
extern	float	speedscale;		// for top sky and bottom sky

void DrawGLWaterPoly (glpoly_t *p);
void DrawGLWaterPolyLightmap (glpoly_t *p);

lpMTexFUNC qglMTexCoord2fSGIS = NULL;
lpSelTexFUNC qglSelectTextureSGIS = NULL;

qboolean mtexenabled = false;

void GL_SelectTexture (GLenum target);

void GL_DisableMultitexture(void) 
{
	if (mtexenabled) {
		qglDisable(GL_TEXTURE_2D);
		GL_SelectTexture(TEXTURE0_SGIS);
		mtexenabled = false;
	}
}

void GL_EnableMultitexture(void) 
{
	if (gl_mtexable) {
		GL_SelectTexture(TEXTURE1_SGIS);
		qglEnable(GL_TEXTURE_2D);
		mtexenabled = true;
	}
}

void R_DrawSequentialPoly (msurface_t *s)
{
}


qgl_hfloat	*gl_vatmp = NULL;
int			gl_szvatmp = 0;

/*
Check and (if needed) expand the size of a scratchpad buffer for vertex arrays.
Returns the buffer pointer.
 */
qgl_hfloat *GL_CheckExpandVaTemp(int size)
{
	if(size > gl_szvatmp)
	{
		if(size<16384)
			size = 16384;
		gl_vatmp = realloc(gl_vatmp, size);
		gl_szvatmp = size;
	}
	return(gl_vatmp);
}

float qgl_fastsin(float rad);
float qgl_fastcos(float rad);


/*
================
DrawGLWaterPoly

Warp the vertex coordinates
================
*/
#if 0
void DrawGLWaterPoly (glpoly_t *p)
{
	int		i;
	float	*v;
	float	s, t, os, ot;
	vec3_t	nv;

	return;

	GL_DisableMultitexture();

	qglBegin (GL_TRIANGLE_FAN);
	v = p->verts[0];
	for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
	{
		qglTexCoord2f (v[3], v[4]);

//		nv[0] = v[0] + 8*sin(v[1]*0.05+realtime)*sin(v[2]*0.05+realtime);
//		nv[1] = v[1] + 8*sin(v[0]*0.05+realtime)*sin(v[2]*0.05+realtime);
		nv[0] = v[0] + 8*
			qgl_fastsin(v[1]*0.05+realtime)*
			qgl_fastsin(v[2]*0.05+realtime);
		nv[1] = v[1] + 8*
			qgl_fastsin(v[0]*0.05+realtime)*
			qgl_fastsin(v[2]*0.05+realtime);
		nv[2] = v[2];

		qglVertex3fv (nv);
	}
	qglEnd ();
}
#endif

void DrawGLWaterPolyLightmap (glpoly_t *p)
{
#if 0
	int		i;
	float	*v;
	float	s, t, os, ot;
	vec3_t	nv;

//	if(r_vertex.value)
//		return;

	GL_DisableMultitexture();

	qglBegin (GL_TRIANGLE_FAN);
	v = p->verts[0];
	for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
	{
		qglTexCoord2f (v[5], v[6]);

//		nv[0] = v[0] + 8*sin(v[1]*0.05+realtime)*sin(v[2]*0.05+realtime);
		nv[0] = v[0] + 8*
			qgl_fastsin(v[1]*0.05+realtime)*
			qgl_fastsin(v[2]*0.05+realtime);
//		nv[1] = v[1] + 8*sin(v[0]*0.05+realtime)*sin(v[2]*0.05+realtime);
		nv[1] = v[1] + 8*
			qgl_fastsin(v[0]*0.05+realtime)*
			qgl_fastsin(v[2]*0.05+realtime);
		nv[2] = v[2];

		qglVertex3fv (nv);
	}
	qglEnd ();
#endif
}

/*
================
DrawGLPoly
================
*/
void DrawGLPoly (glpoly_t *p)
{
	int		i, j, step;
//	float	*v;
	qgl_hfloat	*v;
	float	f;

#if 1
	i = p->numverts * VERTEXSIZE * 4;
	GL_CheckExpandVaTemp(i);

	f = shadelight * (1.0/128);

	i = p->numverts * VERTEXSIZE * sizeof(qgl_hfloat);

	v = p->verts[0];
	step = VERTEXSIZE * sizeof(qgl_hfloat);
	memcpy(gl_vatmp, v, i);

	j=f*255;
	j=0xFF000000 | (j<<16) | (j<<8) | (j<<0);
	
	v = gl_vatmp;
	for(i=0; i<p->numverts; i++)
	{
		*(int *)(v+7) = j;
		v += VERTEXSIZE;
	}

	v = gl_vatmp;

	qglEnableClientState(GL_VERTEX_ARRAY);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglEnableClientState(GL_NORMAL_ARRAY);
	qglEnableClientState(GL_COLOR_ARRAY);

#ifdef QGL_HFLOAT
	qglVertexPointer(3, GL_HALF_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_HALF_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+6);
	qglDrawArrays(p->prim, 0, p->numverts);
#else
	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(p->prim, 0, p->numverts);
#endif

	qglDisableClientState(GL_VERTEX_ARRAY);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglDisableClientState(GL_NORMAL_ARRAY);
	qglDisableClientState(GL_COLOR_ARRAY);
#endif
}


qgl_hfloat	*gl_vadptris = NULL;
int			gl_szvadptris = 0;
int			gl_nvadptris = 0;

/*
Check and (if needed) expand the size of the delayed-poly-triangle list.
 */

qgl_hfloat *R_CheckExpandDelayPolyTris(int nvtx)
{
	int size, size1;
	
	size=(gl_nvadptris + nvtx) * VERTEXSIZE * sizeof(qgl_hfloat);

	if(size > gl_szvadptris)
	{
		size1 = gl_szvadptris;
		if(size1 < (4096 * 16 * sizeof(qgl_hfloat)))
			size1 = (4096 * 16 * sizeof(qgl_hfloat));
		while(size > size1)
			size1 = size1 + (size1 >> 1);
	
		gl_vadptris = realloc(gl_vadptris, size1);
		gl_szvadptris = size1;
	}
	return(gl_vatmp);
}

qgl_hfloat *R_GetExpandDelayPolyTris(int nvtx)
{
	qgl_hfloat *fv;
	R_CheckExpandDelayPolyTris(nvtx);
	
	fv = gl_vadptris + gl_nvadptris * VERTEXSIZE;
	gl_nvadptris += nvtx;
	return(fv);
}

void R_ResetDelayPolyTris(void)
{
	gl_nvadptris = 0;
}

int R_GetDelayPolyTrisSize(void)
{
	return(gl_nvadptris);
}

qgl_hfloat *R_GetDelayPolyTrisBase(void)
{
	return(gl_vadptris);
}

void DrawGLPolyVtx (glpoly_t *p)
{
	int		i, step, pnv;
//	float	*v, *v0, *v1, *v2, *ctv;
	qgl_hfloat	*v, *v0, *v1, *v2, *ctv;
	float f;

//	return;

#if 0
	v = p->verts[0];

	pnv = p->numverts;
	qglBegin(p->prim);
	for (i=0 ; i<pnv ; i++)
	{
		qglColor4ubv((byte *)(v+7));
		qglTexCoord2fv(v+3);
		qglVertex3fv(v);
		v += VERTEXSIZE;
	}
	qglEnd();
#endif

#if 0
//	qglBegin (GL_POLYGON);
	qglBegin (p->prim);
	v = p->verts[0];
	for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
	{
//		f = v[7];
//		qglColor4f(f,f,f,1.0);
		qglColor4ubv((byte *)(v+7));
		qglTexCoord2f (v[3], v[4]);
		qglVertex3fv (v);
	}
	qglEnd ();
#endif

#if 1
	qglEnableClientState(GL_VERTEX_ARRAY);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	qglEnableClientState(GL_COLOR_ARRAY);

	v = p->verts[0];
	step = VERTEXSIZE*4;

	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(p->prim, 0, p->numverts);

	qglDisableClientState(GL_VERTEX_ARRAY);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
	qglDisableClientState(GL_COLOR_ARRAY);
#endif

#if 0
	R_CheckExpandDelayPolyTris((p->numverts-2)*3);

	v = p->verts[0];

	v0 = v;
	v+= VERTEXSIZE;
	
	ctv = gl_vadptris + (gl_nvadptris * VERTEXSIZE);
	
	pnv = p->numverts;
	for (i=2 ; i<pnv ; i++)
	{
		v1 = v;
		v += VERTEXSIZE;
		v2 = v;

		memcpy(ctv, v0, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv += VERTEXSIZE;
		memcpy(ctv, v1, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv += VERTEXSIZE;
		memcpy(ctv, v2, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv += VERTEXSIZE;
		gl_nvadptris+=3;
	}
#endif
}

void DrawGLWaterPoly (glpoly_t *p)
{
	int		i, step, pnv;
//	float	*v, *v0, *v1, *v2, *ctv;
	qgl_hfloat	*v, *v0, *v1, *v2, *ctv;
	float f, ofs_s, ofs_t;

#if 1
	R_CheckExpandDelayPolyTris((p->numverts-2)*3);

	v = p->verts[0];

	v0 = v;
	v+= VERTEXSIZE;
	
	ctv = gl_vadptris + (gl_nvadptris * VERTEXSIZE);
	
	ofs_s = qgl_fastsin( realtime ) * 0.03125;
	ofs_t = qgl_fastcos( realtime ) * 0.03125;
	
	pnv = p->numverts;
	for (i=2 ; i<pnv ; i++)
	{
		v1 = v;
		v += VERTEXSIZE;
		v2 = v;

		memcpy(ctv, v0, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv[3]+=ofs_s;		ctv[4]+=ofs_t;
		ctv += VERTEXSIZE;

		memcpy(ctv, v1, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv[3]+=ofs_s;		ctv[4]+=ofs_t;
		ctv += VERTEXSIZE;

		memcpy(ctv, v2, VERTEXSIZE * sizeof(qgl_hfloat));
		ctv[3]+=ofs_s;		ctv[4]+=ofs_t;
		ctv += VERTEXSIZE;

		gl_nvadptris+=3;
	}
#endif
}

int R_RenderDelayPolyTris()
{
	qgl_hfloat *v;
	int step;

	if(gl_nvadptris<=0)
		return(0);

#if 1
	qglEnableClientState(GL_VERTEX_ARRAY);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglEnableClientState(GL_NORMAL_ARRAY);
	qglEnableClientState(GL_COLOR_ARRAY);

	v = gl_vadptris;
	step = VERTEXSIZE * sizeof(qgl_hfloat);

#ifdef QGL_HFLOAT
	qglVertexPointer(3, GL_HALF_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_HALF_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+6);
	qglDrawArrays(GL_TRIANGLES, 0, gl_nvadptris);
#else
	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(GL_TRIANGLES, 0, gl_nvadptris);
#endif

	qglDisableClientState(GL_VERTEX_ARRAY);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglDisableClientState(GL_NORMAL_ARRAY);
	qglDisableClientState(GL_COLOR_ARRAY);
#endif

	gl_nvadptris = 0;
	return(1);
}


/*
================
R_BlendLightmaps
================
*/
void R_BlendLightmaps (void)
{
}

/*
================
R_RenderBrushPoly
================
*/
void R_RenderBrushPoly (msurface_t *fa)
{
	texture_t	*t;
	byte		*base;
	int			maps;
	glRect_t	*theRect;
	float		f, g;
	int smax, tmax;

	if(!fa->polys)
		return;

	c_brush_polys++;

	if (fa->flags & SURF_DRAWSKY)
	{	// warp texture, no lightmaps
		EmitBothSkyLayers (fa);
		return;
	}
	
	if(currententity_world)
	{
		t=fa->texinfo->texture;
		if (t->anim_total)
			t = R_TextureAnimation (t);
	}else
	{
		t = R_TextureAnimation (fa->texinfo->texture);
	}
	GL_Bind (t->gl_texturenum);

//	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	qglShadeModel (GL_SMOOTH);
//		qglEnable(GL_BLEND);
//	qglHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	if (currententity_world &&
		!(fa->flags & (SURF_DRAWTURB | SURF_UNDERWATER)))
	{
		DrawGLPolyVtx (fa->polys);
		return;
	}

#if 0
	if (fa->flags & SURF_DRAWTURB)
//	if (fa->flags & (SURF_DRAWTURB | SURF_UNDERWATER))
	{	// warp texture, no lightmaps
//		qglColor4f(1,1,1,1);
		EmitWaterPolys (fa);
		return;
	}
#endif

#if 1
	if (fa->flags & (SURF_DRAWTURB | SURF_UNDERWATER))
	{
		if (fa->flags & SURF_DRAWTURB)
		{
			EmitWaterPolys (fa);
			return;
		}else
		{
			DrawGLWaterPoly (fa->polys);
			return;
		}
	}
#endif

#if 0
	if(!currententity_world)
	{
		f = shadelight * (1.0/128);
		qglColor4f(f,f,f,1);
	}
#endif

#if 0
	if (fa->flags & SURF_UNDERWATER)
	{
		DrawGLWaterPoly (fa->polys);
	}
	else
#endif

	if(currententity_world || currententity_world_bmodel)
	{
		DrawGLPolyVtx (fa->polys);
//		qglColor4f(1,1,1,1);
	}
	else
	{
		DrawGLPoly (fa->polys);
	}
}

/*
================
R_RenderDynamicLightmaps
Multitexture
================
*/
void R_RenderDynamicLightmaps (msurface_t *fa)
{
}

/*
================
R_MirrorChain
================
*/
void R_MirrorChain (msurface_t *s)
{
//	if (mirror)
//		return;
//	mirror = true;
//	mirror_plane = s->plane;
}


/*
================
R_DrawWaterSurfaces
================
*/
void R_DrawWaterSurfaces (void)
{
}

/*
================
DrawTextureChains
================
*/
void DrawTextureChains (void)
{
	int		i, n;
	msurface_t	*s;
	texture_t	*t;

	n = cl.worldmodel->numtextures;
	for (i=0 ; i<n ; i++)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		qglShadeModel (GL_SMOOTH);
		qglEnable(GL_BLEND);
		qglHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

		t = cl.worldmodel->textures[i];
		if (!t)
			continue;
		s = t->texturechain;
		if (!s)
			continue;
		if (i == skytexturenum)
		{
			R_DrawSkyChain (s);
		}
		else
		{
			for ( ; s ; s=s->texturechain)
				R_RenderBrushPoly (s);
			R_RenderDelayPolyTris();
		}

		t->texturechain = NULL;
	}
}

/*
=================
R_DrawBrushModel
=================
*/
void R_DrawBrushModel (entity_t *e)
{
	int			i, j, k;
	vec3_t		mins, maxs, morg, mtmp;
	int			numsurfaces;
	msurface_t	*psurf;
	float		dot;
	mplane_t	*pplane;
	model_t		*clmodel;
	qboolean	rotated;
	qboolean	dolight;
	
	float		f;

	currententity = e;
	currenttexture = -1;

	clmodel = e->model;

	currententity_world_bmodel =
		(clmodel->firstmodelsurface != 0);


	if (e->angles[0] || e->angles[1] || e->angles[2])
	{
		rotated = true;
		for (i=0 ; i<3 ; i++)
		{
			mins[i] = e->origin[i] - clmodel->radius;
			maxs[i] = e->origin[i] + clmodel->radius;
		}
	}
	else
	{
		rotated = false;
		VectorAdd (e->origin, clmodel->mins, mins);
		VectorAdd (e->origin, clmodel->maxs, maxs);
	}

	if (R_CullBox (mins, maxs))
		return;

	VectorAdd(mins, maxs, morg);
	VectorScale(morg, 0.5, morg);

	qglColor3f (1,1,1);
	memset (lightmap_polys, 0, sizeof(lightmap_polys));

	VectorSubtract (r_refdef.vieworg, e->origin, modelorg);
	if (rotated)
	{
		vec3_t	temp;
		vec3_t	forward, right, up;

		VectorCopy (modelorg, temp);
		AngleVectors (e->angles, forward, right, up);
		modelorg[0] = DotProduct (temp, forward);
		modelorg[1] = -DotProduct (temp, right);
		modelorg[2] = DotProduct (temp, up);
	}

	psurf = &clmodel->surfaces[clmodel->firstmodelsurface];

// calculate dynamic lighting for bmodel if it's not an
// instanced model
	if (clmodel->firstmodelsurface != 0 && !gl_flashblend.value)
	{
		for (k=0 ; k<MAX_DLIGHTS ; k++)
		{
			if ((cl_dlights[k].die < cl.time) ||
				(!cl_dlights[k].radius))
				continue;

			R_MarkLights (&cl_dlights[k], 1<<k,
				clmodel->nodes + clmodel->hulls[0].firstclipnode);
		}
	}

	qglPushMatrix ();
	e->angles[0] = -e->angles[0];	// stupid quake bug
	R_RotateForEntity (e);
	e->angles[0] = -e->angles[0];	// stupid quake bug

//	ambientlight = shadelight = R_LightPoint (morg);

	dolight =
		(currententity->ambientlight <= 0)		||
		(currententity->msg_origins[0][0] !=
			currententity->msg_origins[1][0])	||
		(currententity->msg_origins[0][1] !=
			currententity->msg_origins[1][1])	||
		(currententity->msg_origins[0][2] !=
			currententity->msg_origins[1][2]);

	if(dolight)
//	if(1)
	{
		f = R_LightPoint (morg);

#if 0
		for(i=0; i<9; i++)
		{
	//		VectorAdd(mins, maxs, morg);
	//		VectorScale(morg, 0.5, morg);
			mtmp[0] = (i&1) ? maxs[0] : mins[0];
			mtmp[1] = (i&2) ? maxs[1] : mins[1];
			mtmp[2] = (i&4) ? maxs[2] : mins[2];
			VectorAdd(mtmp, morg, mtmp);
			VectorScale(mtmp, 0.5, mtmp);
			f += R_LightPoint (mtmp);
		}
		f /= 9;
#endif
		
		ambientlight = shadelight = f;

		if (ambientlight <= 0)
			ambientlight = 1;
			
		currententity->ambientlight = ambientlight;
		currententity->shadelight = shadelight;
	}else
	{
		ambientlight = currententity->ambientlight;
		shadelight = currententity->shadelight;
	}

	//
	// draw texture
	//
	for (i=0 ; i<clmodel->nummodelsurfaces ; i++, psurf++)
	{
	// find which side of the node we are on
		pplane = psurf->plane;

		dot = DotProduct (modelorg, pplane->normal) - pplane->dist;

	// draw the polygon
		if (((psurf->flags & SURF_PLANEBACK) && (dot < -BACKFACE_EPSILON)) ||
			(!(psurf->flags & SURF_PLANEBACK) && (dot > BACKFACE_EPSILON)))
		{
//			if (gl_texsort.value)
				R_RenderBrushPoly (psurf);
//			else
//				R_DrawSequentialPoly (psurf);
		}

		R_RenderDelayPolyTris();
	}

//	R_RenderDelayPolyTris();

	R_BlendLightmaps ();

	qglPopMatrix ();
}

/*
=============================================================

	WORLD MODEL

=============================================================
*/

int r_totalsurfs;

/*
================
R_RecursiveWorldNode
================
*/
void R_RecursiveWorldNode (mnode_t *node)
{
	int			i, c, side, *pindex;
	vec3_t		acceptpt, rejectpt;
	mplane_t	*plane;
	msurface_t	*surf, **mark;
	mleaf_t		*pleaf;
	texture_t	*texture;
	double		d, dot;
	vec3_t		mins, maxs;

	if (node->visframe != r_visframecount)
		return;

	if (node->contents == CONTENTS_SOLID)
		return;		// solid

	if (R_CullBox (node->minmaxs, node->minmaxs+3))
		return;
	
// if a leaf node, draw stuff
	if (node->contents < 0)
	{
		pleaf = (mleaf_t *)node;

		mark = pleaf->firstmarksurface;
		c = pleaf->nummarksurfaces;

		if (c)
		{
			do
			{
				(*mark)->visframe = r_framecount;
				mark++;
			} while (--c);
		}

	// deal with model fragments in this leaf
		if (pleaf->efrags)
			R_StoreEfrags (&pleaf->efrags);

		return;
	}

// node is just a decision point, so go down the apropriate sides

// find which side of the node we are on
	plane = node->plane;

#if 0
	switch (plane->type)
	{
	case PLANE_X:
		dot = modelorg[0] - plane->dist;
		break;
	case PLANE_Y:
		dot = modelorg[1] - plane->dist;
		break;
	case PLANE_Z:
		dot = modelorg[2] - plane->dist;
		break;
	default:
		dot = DotProduct (modelorg, plane->normal) - plane->dist;
		break;
	}
#endif

	dot = DotProduct (modelorg, plane->normal) - plane->dist;
	side = dot < 0;

//	if (dot >= 0)
//		side = 0;
//	else
//		side = 1;

// recurse down the children, front side first
	R_RecursiveWorldNode (node->children[side]);

// draw stuff
	c = node->numsurfaces;

//	if (c)
	if (c>0)
	{
		r_totalsurfs += c;
	
		surf = cl.worldmodel->surfaces + node->firstsurface;

//		if (dot < 0 -BACKFACE_EPSILON)
//			side = SURF_PLANEBACK;
//		else if (dot > BACKFACE_EPSILON)
//			side = 0;

//		{
//			for ( ; c ; c--, surf++)
			while(c)
			{
				if (surf->visframe != r_framecount)
					{ surf++; c--; continue; }

				if (!surf->polys)
					{ surf++; c--; continue; }

				// don't backface underwater surfaces, because they warp
//				if ( !(surf->flags & SURF_UNDERWATER) && 
//					( (dot < 0) ^ !!(surf->flags & SURF_PLANEBACK)) )
//						continue;		// wrong side

				if ( !(surf->flags & SURF_UNDERWATER) )
				{
					if(((surf->flags>>1)&1)^(dot<0))
						{ surf++; c--; continue; }
				}
				
//				if(R_CullSphere(surf->porg, surf->porg[3]))
//					{ surf++; c--; continue; }

				if(R_CullBox(surf->minmaxs+0, surf->minmaxs+3))
					{ surf++; c--; continue; }

//				if((r_totalsurfs^c)&1)
//					{ surf++; c--; continue; }

//				surf->texturechain = surf->texinfo->texture->texturechain;
//				surf->texinfo->texture->texturechain = surf;

				texture = surf->texinfo->texture;
				surf->texturechain = texture->texturechain;
				texture->texturechain = surf;
				surf++;
				c--;
			}
//		}

	}

//	if(r_totalsurfs > 200)
//	if(r_totalsurfs > 400)
//		return;

// recurse down the back side
	R_RecursiveWorldNode (node->children[!side]);
}



/*
=============
R_DrawWorld
=============
*/
void R_DrawWorld (void)
{
	entity_t	ent;
	int			i;

	if(!r_drawworld.value)
		return;

	memset (&ent, 0, sizeof(ent));
	ent.model = cl.worldmodel;

	VectorCopy (r_refdef.vieworg, modelorg);

	currententity = &ent;
	currenttexture = -1;
	currententity_world = true;
	currententity_world_bmodel = false;

	qglColor3f (1,1,1);
	memset (lightmap_polys, 0, sizeof(lightmap_polys));
#ifdef QUAKE2
	R_ClearSkyBox ();
#endif

	r_totalsurfs = 0;

	R_RecursiveWorldNode (cl.worldmodel->nodes);

	DrawTextureChains ();

	R_BlendLightmaps ();

#ifdef QUAKE2
	R_DrawSkyBox ();
#endif

	currententity_world = false;
	currententity_world_bmodel = false;

}


/*
===============
R_MarkLeaves
===============
*/
void R_MarkLeaves (void)
{
	byte	*vis;
	mnode_t	*node;
	int		i;
//	byte	solid[4096];
//	static byte	solid[4096];
	static byte *solid = NULL;

//	if (r_oldviewleaf == r_viewleaf && !r_novis.value)
	if (r_oldviewleaf == r_viewleaf)
		return;
	
//	if (mirror)
//		return;

	if(!solid)
		solid = malloc(4096);

	r_visframecount++;
	r_oldviewleaf = r_viewleaf;

	if (r_novis.value)
	{
		vis = solid;
		memset (solid, 0xff, (cl.worldmodel->numleafs+7)>>3);
	}
	else
		vis = Mod_LeafPVS (r_viewleaf, cl.worldmodel);
		
	for (i=0 ; i<cl.worldmodel->numleafs ; i++)
	{
		if (vis[i>>3] & (1<<(i&7)))
		{
			node = (mnode_t *)&cl.worldmodel->leafs[i+1];
			do
			{
				if (node->visframe == r_visframecount)
					break;
				node->visframe = r_visframecount;
				node = node->parent;
			} while (node);
		}
	}
}



/*
=============================================================================

  LIGHTMAP ALLOCATION

=============================================================================
*/

// returns a texture number and the position inside it
int AllocBlock (int w, int h, int *x, int *y)
{
	int		i, j;
	int		best, best2;
	int		bestx;
	int		texnum;

	for (texnum=0 ; texnum<MAX_LIGHTMAPS ; texnum++)
	{
		best = BLOCK_HEIGHT;

		for (i=0 ; i<BLOCK_WIDTH-w ; i++)
		{
			best2 = 0;

			for (j=0 ; j<w ; j++)
			{
				if (allocated[texnum][i+j] >= best)
					break;
				if (allocated[texnum][i+j] > best2)
					best2 = allocated[texnum][i+j];
			}
			if (j == w)
			{	// this is a valid spot
				*x = i;
				*y = best = best2;
			}
		}

		if (best + h > BLOCK_HEIGHT)
			continue;

		for (i=0 ; i<w ; i++)
			allocated[texnum][*x + i] = best + h;

		return texnum;
	}

	Sys_Error ("AllocBlock: full");
}


mvertex_t	*r_pcurrentvertbase;
model_t		*currentmodel;

void BoundSurface (msurface_t *fa)
{
	int			i, j, k;
	int			lindex, lnumverts, s_axis, t_axis;
	float		dist, lastdist, lzi, scale, u, v, frac;
	unsigned	mask;
//	vec3_t		local, transformed;
	vec3_t		porg, ptmp;
//	vec3_t		pdx, pdy, pdz;
	vec3_t		mins, maxs;
	float		pdist, prad;
	
	medge_t		*pedges, *r_pedge;
	mplane_t	*pplane;
	int			vertpage, newverts, newpage, lastvert;
	int			luma, luma1;
	qboolean	visible;
	float		*vec, *vec1;
	float		s, t, f;
	glpoly_t	*poly;

// reconstruct the polygon
	pedges = currentmodel->edges;
	lnumverts = fa->numedges;
	vertpage = 0;
	
//	VectorZero(porg);
	porg[0]=0;
	porg[1]=0;
	porg[2]=0;

	mins[0]= 999999; 	mins[1]= 999999; 	mins[2]= 999999;
	maxs[0]=-999999; 	maxs[1]=-999999; 	maxs[2]=-999999;

	for (i=0 ; i<lnumverts ; i++)
	{
		lindex = currentmodel->surfedges[fa->firstedge + i];
		if (lindex > 0)
		{
			r_pedge = &pedges[lindex];
			vec = r_pcurrentvertbase[r_pedge->v[0]].position;
//			vec = loadmodel->vertexes[loadmodel->edges[lindex].v[0]].position;
		}
		else
		{
			r_pedge = &pedges[-lindex];
			vec = r_pcurrentvertbase[r_pedge->v[1]].position;
//			vec = loadmodel->vertexes[loadmodel->edges[lindex].v[1]].position;
		}
		
		VectorAdd(porg, vec, porg);
		
		if(vec[0]<mins[0])
			mins[0]=vec[0];
		if(vec[1]<mins[1])
			mins[1]=vec[1];
		if(vec[2]<mins[2])
			mins[2]=vec[2];

		if(vec[0]>maxs[0])
			maxs[0]=vec[0];
		if(vec[1]>maxs[1])
			maxs[1]=vec[1];
		if(vec[2]>maxs[2])
			maxs[2]=vec[2];
	}

	VectorScale(porg, 1.0/lnumverts, porg);

	prad = 0;
	for (i=0 ; i<lnumverts ; i++)
	{
		lindex = currentmodel->surfedges[fa->firstedge + i];
		if (lindex > 0)
		{
			r_pedge = &pedges[lindex];
			vec = r_pcurrentvertbase[r_pedge->v[0]].position;
//			vec = loadmodel->vertexes[loadmodel->edges[lindex].v[0]].position;
		}
		else
		{
			r_pedge = &pedges[-lindex];
			vec = r_pcurrentvertbase[r_pedge->v[1]].position;
//			vec = loadmodel->vertexes[loadmodel->edges[lindex].v[1]].position;
		}
		
		VectorSubtract(porg, vec, ptmp);
		pdist = Length(ptmp);
		
		if(pdist>prad)
			prad=pdist;
	}
	
	VectorCopy(mins, fa->minmaxs+0);
	VectorCopy(maxs, fa->minmaxs+3);

	fa->porg[0]=porg[0];
	fa->porg[1]=porg[1];
	fa->porg[2]=porg[2];
	fa->porg[3]=prad;
}

int	nColinElim;

/*
================
BuildSurfaceDisplayList
================
*/
void BuildSurfaceDisplayList (msurface_t *fa)
{
	int			i, j, k;
	int			lindex, lnumverts, s_axis, t_axis;
	float		dist, lastdist, lzi, scale, u, v, frac;
	unsigned	mask;
	vec3_t		local, transformed;
	vec3_t		porg, ptmp;
	vec3_t		pdx, pdy, pdz;
	float		pdist, prad;
	
	medge_t		*pedges, *r_pedge;
	mplane_t	*pplane;
	int			vertpage, newverts, newpage, lastvert;
	int			luma, luma1;
	qboolean	visible;
	float		*vec, *vec1;
	float		s, t, f;
	glpoly_t	*poly;

// reconstruct the polygon
	pedges = currentmodel->edges;
	lnumverts = fa->numedges;
	vertpage = 0;

	BoundSurface(fa);
	
	VectorCopy(fa->porg, porg);

	if(fa->porg[3] < 3)
	{
		fa->polys = NULL;
		return;
	}

#if 1
	k=0;
	if((fa->minmaxs[3]-fa->minmaxs[0])<1) k++;
	if((fa->minmaxs[4]-fa->minmaxs[1])<1) k++;
	if((fa->minmaxs[5]-fa->minmaxs[2])<1) k++;
	if(k>1)
	{
		fa->polys = NULL;
		return;
	}
#endif

	//
	// draw texture
	//
//	poly = Hunk_Alloc (sizeof(glpoly_t) + (lnumverts-4) * VERTEXSIZE*sizeof(float));
	poly = Hunk_Alloc (sizeof(glpoly_t) + (lnumverts-4) *
		VERTEXSIZE * sizeof(qgl_hfloat));
	poly->next = fa->polys;
	poly->flags = fa->flags;
	poly->prim = GL_POLYGON;
	fa->polys = poly;
	poly->numverts = lnumverts;


	for (i=0 ; i<lnumverts ; i++)
	{
		lindex = currentmodel->surfedges[fa->firstedge + i];

		if (lindex > 0)
		{
			r_pedge = &pedges[lindex];
			vec = r_pcurrentvertbase[r_pedge->v[0]].position;
			vec1 = r_pcurrentvertbase[r_pedge->v[1]].position;
			luma = r_pedge->luma[0];
			luma1 = r_pedge->luma[1];
		}
		else
		{
			r_pedge = &pedges[-lindex];
			vec = r_pcurrentvertbase[r_pedge->v[1]].position;
			vec1 = r_pcurrentvertbase[r_pedge->v[0]].position;
			luma = r_pedge->luma[1];
			luma1 = r_pedge->luma[0];
		}
		s = DotProduct (vec, fa->texinfo->vecs[0]) + fa->texinfo->vecs[0][3];
		s /= fa->texinfo->texture->width;

		t = DotProduct (vec, fa->texinfo->vecs[1]) + fa->texinfo->vecs[1][3];
		t /= fa->texinfo->texture->height;

		VectorCopy (vec, poly->verts[i]);
		poly->verts[i][3] = s;
		poly->verts[i][4] = t;

		poly->verts[i][5] = s;
		poly->verts[i][6] = t;

		if(luma>0)
		{
			j=luma;
			k=0xFF000000|(j<<16)|(j<<8)|j;
		}else
		{
			k=0xFFFFFFFFU;

//			tk_printf("Relight %d\n", lindex);

	#if 1
			VectorSubtract(vec1, vec, pdx);
			VectorSubtract(vec, porg, pdy);
			CrossProduct(pdx, pdy, pdz);
			VectorNormalizeFast(pdz);

	//		ptmp[0]=(vec[0]+porg[0])*0.5;
	//		ptmp[1]=(vec[1]+porg[1])*0.5;
	//		ptmp[2]=(vec[2]+porg[2])*0.5;

			ptmp[0]=(vec[0]*0.95)+(porg[0]*0.05);
			ptmp[1]=(vec[1]*0.95)+(porg[1]*0.05);
			ptmp[2]=(vec[2]*0.95)+(porg[2]*0.05);
			
	//		ptmp[0]+=pdz[0]*4;
	//		ptmp[1]+=pdz[1]*4;
	//		ptmp[2]+=pdz[2]*4;

	//		f = R_LightPoint(vec) / 255.0;
	//		f = R_LightPoint(ptmp) / 255.0;
	//		f = R_LightPointDir(ptmp, pdz) / 128.0;
			
	//		f = f*1.5;
	//		poly->verts[i][7] = 0.5;
	//		poly->verts[i][7] = f;

			j = R_LightPointDir(ptmp, pdz) * 2;
			
	//		j=f*255;
			if(j<1)j=1;
			if(j>255)j=255;
			k=0xFF000000|(j<<16)|(j<<8)|j;

			if (lindex > 0)
				{ r_pedge->luma[0]=j; }
			else
				{ r_pedge->luma[1]=j; }

	#endif
		}

#ifdef QGL_HFLOAT
		*(int *)((poly->verts[i])+6) = k;
#else
		((int *)(poly->verts[i]))[7] = k;
#endif
	}

	//
	// remove co-linear points - Ed
	//
	if (!gl_keeptjunctions.value && !(fa->flags & SURF_UNDERWATER) )
	{
		for (i = 0 ; i < lnumverts ; ++i)
		{
			vec3_t v1, v2, v3;
			qgl_hfloat *prev, *cur, *next;
			float f, d;

			prev = poly->verts[(i + lnumverts - 1) % lnumverts];
			cur = poly->verts[i];
			next = poly->verts[(i + 1) % lnumverts];

			VectorSubtract( cur, prev, v1 );
			VectorNormalize( v1 );
			VectorSubtract( next, prev, v2 );
			VectorNormalize( v2 );

			VectorSubtract( next, cur, v3 );
			d = VectorNormalize( v3 );

			// skip co-linear points
//			#define COLINEAR_EPSILON 0.001
			#define COLINEAR_EPSILON 0.01
//			#define COLINEAR_EPSILON 0.1
//			if ((fabs( v1[0] - v2[0] ) <= COLINEAR_EPSILON) &&
//				(fabs( v1[1] - v2[1] ) <= COLINEAR_EPSILON) && 
//				(fabs( v1[2] - v2[2] ) <= COLINEAR_EPSILON))

			if (((fabs( v1[0] - v2[0] ) <= COLINEAR_EPSILON) &&
				(fabs( v1[1] - v2[1] ) <= COLINEAR_EPSILON) && 
				(fabs( v1[2] - v2[2] ) <= COLINEAR_EPSILON)) ||
				(d<0.1))
			{
				int j;
				for (j = i + 1; j < lnumverts; ++j)
				{
					int k;
					for (k = 0; k < VERTEXSIZE; ++k)
						poly->verts[j - 1][k] = poly->verts[j][k];
				}
				--lnumverts;
				++nColinElim;
				// retry next vertex next time, which is now current vertex
				--i;
			}
		}
	}
	poly->numverts = lnumverts;

	if(lnumverts<3)
	{
		fa->polys = NULL;
		return;
	}

}

/*
========================
GL_CreateSurfaceLightmap
========================
*/
void GL_CreateSurfaceLightmap (msurface_t *surf)
{
}


/*
==================
GL_BuildLightmaps

Builds the lightmap texture
with all the surfaces from all brush models
==================
*/

extern qboolean isPermedia;

void GL_BuildLightmaps (void)
{
	int		i, j;
	model_t	*m;
//	extern qboolean isPermedia;

	memset (allocated, 0, sizeof(allocated));

	r_framecount = 1;		// no dlightcache

	if (!lightmap_textures)
	{
		lightmap_textures = texture_extension_number;
		texture_extension_number += MAX_LIGHTMAPS;
	}

	gl_lightmap_format = GL_LUMINANCE;
	lightmap_bytes = 1;

	for (j=1 ; j<MAX_MODELS ; j++)
	{
		m = cl.model_precache[j];
		if (!m)
			break;
		if (m->name[0] == '*')
			continue;
		r_pcurrentvertbase = m->vertexes;
		currentmodel = m;
		for (i=0 ; i<m->numsurfaces ; i++)
		{
			GL_CreateSurfaceLightmap (m->surfaces + i);
			if ( m->surfaces[i].flags & SURF_DRAWTURB )
				continue;
#ifndef QUAKE2
			if ( m->surfaces[i].flags & SURF_DRAWSKY )
				continue;
#endif
			BuildSurfaceDisplayList (m->surfaces + i);
		}
	}
}


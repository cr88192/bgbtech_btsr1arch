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

#if 0
/*
================
R_DrawSequentialPoly

Systems that have fast state and texture changes can
just do everything as it passes with no need to sort
================
*/
void R_DrawSequentialPoly (msurface_t *s)
{
	glpoly_t	*p;
	float		*v;
	int			i;
	texture_t	*t;

	//
	// normal lightmaped poly
	//
	if (! (s->flags & (SURF_DRAWSKY|SURF_DRAWTURB|SURF_UNDERWATER) ) )
	{
		p = s->polys;

		t = R_TextureAnimation (s->texinfo->texture);
		GL_Bind (t->gl_texturenum);
		qglBegin (GL_POLYGON);
		v = p->verts[0];
		for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
		{
			qglTexCoord2f (v[3], v[4]);
			qglVertex3fv (v);
		}
		qglEnd ();

		GL_Bind (lightmap_textures + s->lightmaptexturenum);
		qglEnable (GL_BLEND);
		qglBegin (GL_POLYGON);
		v = p->verts[0];
		for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
		{
			qglTexCoord2f (v[5], v[6]);
			qglVertex3fv (v);
		}
		qglEnd ();

		qglDisable (GL_BLEND);

		return;
	}

	//
	// subdivided water surface warp
	//
	if (s->flags & SURF_DRAWTURB)
	{
		GL_Bind (s->texinfo->texture->gl_texturenum);
		EmitWaterPolys (s);
		return;
	}

	//
	// subdivided sky warp
	//
	if (s->flags & SURF_DRAWSKY)
	{
		GL_Bind (solidskytexture);
		speedscale = realtime*8;
		speedscale -= (int)speedscale;

		EmitSkyPolys (s);

		qglEnable (GL_BLEND);
		qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GL_Bind (alphaskytexture);
		speedscale = realtime*16;
		speedscale -= (int)speedscale;
		EmitSkyPolys (s);
		if (gl_lightmap_format == GL_LUMINANCE)
			qglBlendFunc (GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

		qglDisable (GL_BLEND);
	}

	//
	// underwater warped with lightmap
	//
	p = s->polys;

	t = R_TextureAnimation (s->texinfo->texture);
	GL_Bind (t->gl_texturenum);
	DrawGLWaterPoly (p);

	GL_Bind (lightmap_textures + s->lightmaptexturenum);
	qglEnable (GL_BLEND);
	DrawGLWaterPolyLightmap (p);
	qglDisable (GL_BLEND);
}
#else
/*
================
R_DrawSequentialPoly

Systems that have fast state and texture changes can
just do everything as it passes with no need to sort
================
*/
void R_DrawSequentialPoly (msurface_t *s)
{
#if 0
	glpoly_t	*p;
	float		*v;
	int			i, ll;
	texture_t	*t;
	vec3_t		nv, dir;
	float		ss, ss2, length;
	float		s1, t1;
	float		f;
	glRect_t	*theRect;

	//
	// normal lightmaped poly
	//

	return;

	if (! (s->flags & (SURF_DRAWSKY|SURF_DRAWTURB|SURF_UNDERWATER) ) )
	{
		R_RenderDynamicLightmaps (s);
		
		if(r_vertex.value)
		{
			p = s->polys;

			t = R_TextureAnimation (s->texinfo->texture);
			GL_Bind (t->gl_texturenum);
			qglBegin (GL_POLYGON);
			v = p->verts[0];
			for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
			{
				ll=R_LightPoint(v);
				f=ll*(1.0/255.0);
				qglColor4f(f,f,f,1.0);
				qglTexCoord2f (v[3], v[4]);
				qglVertex3fv (v);
			}
			qglEnd ();

		}else
//		if (gl_mtexable)
		if (0)
		{
			p = s->polys;

			t = R_TextureAnimation (s->texinfo->texture);
			// Binds world to texture env 0
			GL_SelectTexture(TEXTURE0_SGIS);
			GL_Bind (t->gl_texturenum);
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			// Binds lightmap to texenv 1
			GL_EnableMultitexture(); // Same as SelectTexture (TEXTURE1)
			GL_Bind (lightmap_textures + s->lightmaptexturenum);
			i = s->lightmaptexturenum;
			if (lightmap_modified[i])
			{
				lightmap_modified[i] = false;
				theRect = &lightmap_rectchange[i];
				qglTexSubImage2D(GL_TEXTURE_2D, 0, 0, theRect->t, 
					BLOCK_WIDTH, theRect->h, gl_lightmap_format, GL_UNSIGNED_BYTE,
					lightmaps+(i* BLOCK_HEIGHT + theRect->t) *BLOCK_WIDTH*lightmap_bytes);
				theRect->l = BLOCK_WIDTH;
				theRect->t = BLOCK_HEIGHT;
				theRect->h = 0;
				theRect->w = 0;
			}
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			qglBegin(GL_POLYGON);
			v = p->verts[0];
			for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
			{
				qglMTexCoord2fSGIS (TEXTURE0_SGIS, v[3], v[4]);
				qglMTexCoord2fSGIS (TEXTURE1_SGIS, v[5], v[6]);
				qglVertex3fv (v);
			}
			qglEnd ();
			return;
		} else {
			p = s->polys;

			t = R_TextureAnimation (s->texinfo->texture);
			GL_Bind (t->gl_texturenum);
			qglBegin (GL_POLYGON);
			v = p->verts[0];
			for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
			{
				qglTexCoord2f (v[3], v[4]);
				qglVertex3fv (v);
			}
			qglEnd ();

			GL_Bind (lightmap_textures + s->lightmaptexturenum);
			qglEnable (GL_BLEND);
			qglBegin (GL_POLYGON);
			v = p->verts[0];
			for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
			{
				qglTexCoord2f (v[5], v[6]);
				qglVertex3fv (v);
			}
			qglEnd ();

			qglDisable (GL_BLEND);
		}

		return;
	}

	//
	// subdivided water surface warp
	//

	if (s->flags & SURF_DRAWTURB)
	{
		GL_DisableMultitexture();
		GL_Bind (s->texinfo->texture->gl_texturenum);
		EmitWaterPolys (s);
		return;
	}

	//
	// subdivided sky warp
	//
	if (s->flags & SURF_DRAWSKY)
	{
		GL_DisableMultitexture();
		GL_Bind (solidskytexture);
		speedscale = realtime*8;
		speedscale -= (int)speedscale & ~127;

		EmitSkyPolys (s);

		qglEnable (GL_BLEND);
		GL_Bind (alphaskytexture);
		speedscale = realtime*16;
		speedscale -= (int)speedscale & ~127;
		EmitSkyPolys (s);

		qglDisable (GL_BLEND);
		return;
	}

	//
	// underwater warped with lightmap
	//
	R_RenderDynamicLightmaps (s);
	if (gl_mtexable) {
		p = s->polys;

		t = R_TextureAnimation (s->texinfo->texture);
		GL_SelectTexture(TEXTURE0_SGIS);
		GL_Bind (t->gl_texturenum);
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		GL_EnableMultitexture();
		GL_Bind (lightmap_textures + s->lightmaptexturenum);
		i = s->lightmaptexturenum;
		if (lightmap_modified[i])
		{
			lightmap_modified[i] = false;
			theRect = &lightmap_rectchange[i];
			qglTexSubImage2D(GL_TEXTURE_2D, 0, 0, theRect->t, 
				BLOCK_WIDTH, theRect->h, gl_lightmap_format, GL_UNSIGNED_BYTE,
				lightmaps+(i* BLOCK_HEIGHT + theRect->t) *BLOCK_WIDTH*lightmap_bytes);
			theRect->l = BLOCK_WIDTH;
			theRect->t = BLOCK_HEIGHT;
			theRect->h = 0;
			theRect->w = 0;
		}
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		qglBegin (GL_TRIANGLE_FAN);
		v = p->verts[0];
		for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
		{
			qglMTexCoord2fSGIS (TEXTURE0_SGIS, v[3], v[4]);
			qglMTexCoord2fSGIS (TEXTURE1_SGIS, v[5], v[6]);

			nv[0] = v[0] + 8*sin(v[1]*0.05+realtime)*sin(v[2]*0.05+realtime);
			nv[1] = v[1] + 8*sin(v[0]*0.05+realtime)*sin(v[2]*0.05+realtime);
			nv[2] = v[2];

			qglVertex3fv (nv);
		}
		qglEnd ();

	} else {
		p = s->polys;

		t = R_TextureAnimation (s->texinfo->texture);
		GL_Bind (t->gl_texturenum);
		DrawGLWaterPoly (p);

		GL_Bind (lightmap_textures + s->lightmaptexturenum);
		qglEnable (GL_BLEND);
		DrawGLWaterPolyLightmap (p);
		qglDisable (GL_BLEND);
	}
#endif
}
#endif


float	*gl_vatmp = NULL;
int		gl_szvatmp = 0;

/*
Check and (if needed) expand the size of a scratchpad buffer for vertex arrays.
Returns the buffer pointer.
 */
void *GL_CheckExpandVaTemp(int size)
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
void DrawGLWaterPoly (glpoly_t *p)
{
	int		i;
	float	*v;
	float	s, t, os, ot;
	vec3_t	nv;

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

void DrawGLWaterPolyLightmap (glpoly_t *p)
{
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
}

/*
================
DrawGLPoly
================
*/
void DrawGLPoly (glpoly_t *p)
{
//	float	tmp[256*VERTEXSIZE];
	int		i, j, step;
	float	*v;
	float	f;

//	return;

#if 0
	f = shadelight * (1.0/128);
	qglColor4f(f,f,f,1.0);

//	qglBegin (GL_POLYGON);
	qglBegin (p->prim);
	v = p->verts[0];
	for (i=0 ; i<p->numverts ; i++, v+= VERTEXSIZE)
	{
		qglTexCoord2f (v[3], v[4]);
		qglVertex3fv (v);
	}
	qglEnd ();
#endif

#if 1
	i = p->numverts * VERTEXSIZE * 4;
	GL_CheckExpandVaTemp(i);

	f = shadelight * (1.0/128);

	i = p->numverts * VERTEXSIZE * 4;

	v = p->verts[0];
	step = VERTEXSIZE*4;
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

	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(p->prim, 0, p->numverts);

	qglDisableClientState(GL_VERTEX_ARRAY);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglDisableClientState(GL_NORMAL_ARRAY);
	qglDisableClientState(GL_COLOR_ARRAY);
#endif
}


float	*gl_vadptris = NULL;
int		gl_szvadptris = 0;
int		gl_nvadptris = 0;

/*
Check and (if needed) expand the size of the delayed-poly-triangle list.
 */

void *R_CheckExpandDelayPolyTris(int nvtx)
{
	int size, size1;
	
	size=(gl_nvadptris + nvtx) * VERTEXSIZE * sizeof(float);

	if(size > gl_szvadptris)
	{
		size1 = gl_szvadptris;
		if(size1 < 4096)
			size1 = 4096;
		while(size > size1)
			size1 = size1 + (size1 >> 1);
	
		gl_vadptris = realloc(gl_vadptris, size1);
		gl_szvadptris = size1;
	}
	return(gl_vatmp);
}

float *R_GetExpandDelayPolyTris(int nvtx)
{
	float *fv;
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

float *R_GetDelayPolyTrisBase(void)
{
	return(gl_vadptris);
}

void DrawGLPolyVtx (glpoly_t *p)
{
	int		i, step, pnv;
	float	*v, *v0, *v1, *v2, *ctv;
	float f;

//	return;

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

#if 0
	qglEnableClientState(GL_VERTEX_ARRAY);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglEnableClientState(GL_NORMAL_ARRAY);
	qglEnableClientState(GL_COLOR_ARRAY);

	v = p->verts[0];
	step = VERTEXSIZE*4;

	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(p->prim, 0, p->numverts);

	qglDisableClientState(GL_VERTEX_ARRAY);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglDisableClientState(GL_NORMAL_ARRAY);
	qglDisableClientState(GL_COLOR_ARRAY);
#endif

#if 1
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

		memcpy(ctv, v0, VERTEXSIZE * sizeof(float));
		ctv += VERTEXSIZE;
		memcpy(ctv, v1, VERTEXSIZE * sizeof(float));
		ctv += VERTEXSIZE;
		memcpy(ctv, v2, VERTEXSIZE * sizeof(float));
		ctv += VERTEXSIZE;
		gl_nvadptris+=3;

//		qglColor4ubv((byte *)(v+7));
//		qglTexCoord2f (v[3], v[4]);
//		qglVertex3fv (v);
	}
#endif

}

int R_RenderDelayPolyTris()
{
	float *v;
	int step;

	if(gl_nvadptris<=0)
		return(0);

#if 1
	qglEnableClientState(GL_VERTEX_ARRAY);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	qglEnableClientState(GL_NORMAL_ARRAY);
	qglEnableClientState(GL_COLOR_ARRAY);

	v = gl_vadptris;
	step = VERTEXSIZE*4;

	qglVertexPointer(3, GL_FLOAT, step, v+0);
	qglTexCoordPointer(2, GL_FLOAT, step, v+3);
	qglColorPointer(4,  GL_UNSIGNED_BYTE, step, v+7);
	qglDrawArrays(GL_TRIANGLES, 0, gl_nvadptris);

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
	int			i, j;
	glpoly_t	*p;
	float		*v;
	glRect_t	*theRect;

	if (r_fullbright.value)
		return;
	if (!gl_texsort.value)
		return;

	if(r_vertex.value)
		return;

	qglDepthMask (0);		// don't bother writing Z

	if (gl_lightmap_format == GL_LUMINANCE)
		qglBlendFunc (GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	else if (gl_lightmap_format == GL_INTENSITY)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		qglColor4f (0,0,0,1);
		qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (!r_lightmap.value)
	{
		qglEnable (GL_BLEND);
	}

	for (i=0 ; i<MAX_LIGHTMAPS ; i++)
	{
		p = lightmap_polys[i];
		if (!p)
			continue;
		GL_Bind(lightmap_textures+i);
		if (lightmap_modified[i])
		{
			lightmap_modified[i] = false;
			theRect = &lightmap_rectchange[i];
//			qglTexImage2D (GL_TEXTURE_2D, 0, lightmap_bytes
//			, BLOCK_WIDTH, BLOCK_HEIGHT, 0, 
//			gl_lightmap_format, GL_UNSIGNED_BYTE, lightmaps+i*BLOCK_WIDTH*BLOCK_HEIGHT*lightmap_bytes);
//			qglTexImage2D (GL_TEXTURE_2D, 0, lightmap_bytes
//				, BLOCK_WIDTH, theRect->h, 0, 
//				gl_lightmap_format, GL_UNSIGNED_BYTE, lightmaps+(i*BLOCK_HEIGHT+theRect->t)*BLOCK_WIDTH*lightmap_bytes);
			qglTexSubImage2D(GL_TEXTURE_2D, 0, 0, theRect->t, 
				BLOCK_WIDTH, theRect->h, gl_lightmap_format, GL_UNSIGNED_BYTE,
				lightmaps+(i* BLOCK_HEIGHT + theRect->t) *BLOCK_WIDTH*lightmap_bytes);
			theRect->l = BLOCK_WIDTH;
			theRect->t = BLOCK_HEIGHT;
			theRect->h = 0;
			theRect->w = 0;
		}
		for ( ; p ; p=p->chain)
		{
			if (p->flags & SURF_UNDERWATER)
				DrawGLWaterPolyLightmap (p);
			else
			{
				qglBegin (GL_POLYGON);
				v = p->verts[0];
				for (j=0 ; j<p->numverts ; j++, v+= VERTEXSIZE)
				{
					qglTexCoord2f (v[5], v[6]);
					qglVertex3fv (v);
				}
				qglEnd ();
			}
		}
	}

	qglDisable (GL_BLEND);
	if (gl_lightmap_format == GL_LUMINANCE)
		qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else if (gl_lightmap_format == GL_INTENSITY)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		qglColor4f (1,1,1,1);
	}

	qglDepthMask (1);		// back to normal Z buffering
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

	if(r_vertex.value)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		qglShadeModel (GL_SMOOTH);
//		qglEnable(GL_BLEND);
		qglHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

		if (currententity_world &&
			!(fa->flags & (SURF_DRAWTURB | SURF_UNDERWATER)))
		{
			DrawGLPolyVtx (fa->polys);
			return;
		}

	}

	if (fa->flags & SURF_DRAWTURB)
	{	// warp texture, no lightmaps
//		qglColor4f(1,1,1,1);
		EmitWaterPolys (fa);
		return;
	}

	if(!currententity_world)
	{
		f = shadelight * (1.0/128);
		qglColor4f(f,f,f,1);
	}

	if (fa->flags & SURF_UNDERWATER)
	{
		DrawGLWaterPoly (fa->polys);
	}
//	else if(r_vertex.value && currententity_world)
//	else if(r_vertex.value)
	else if(r_vertex.value &&
		(currententity_world || currententity->model->firstmodelsurface))
	{
		DrawGLPolyVtx (fa->polys);
		qglColor4f(1,1,1,1);
	}
	else
	{
		DrawGLPoly (fa->polys);
	}

	if (r_vertex.value)
	{
		return;
	}

	// add the poly to the proper lightmap chain

	fa->polys->chain = lightmap_polys[fa->lightmaptexturenum];
	lightmap_polys[fa->lightmaptexturenum] = fa->polys;

	// check for lightmap modification
	for (maps = 0 ; maps < MAXLIGHTMAPS && fa->styles[maps] != 255 ;
		 maps++)
		if (d_lightstylevalue[fa->styles[maps]] != fa->cached_light[maps])
			goto dynamic;

	if (fa->dlightframe == r_framecount	// dynamic this frame
		|| fa->cached_dlight)			// dynamic previously
	{
dynamic:
		if (r_dynamic.value)
		{
			lightmap_modified[fa->lightmaptexturenum] = true;
			theRect = &lightmap_rectchange[fa->lightmaptexturenum];
			if (fa->light_t < theRect->t) {
				if (theRect->h)
					theRect->h += theRect->t - fa->light_t;
				theRect->t = fa->light_t;
			}
			if (fa->light_s < theRect->l) {
				if (theRect->w)
					theRect->w += theRect->l - fa->light_s;
				theRect->l = fa->light_s;
			}
			smax = (fa->extents[0]>>4)+1;
			tmax = (fa->extents[1]>>4)+1;
			if ((theRect->w + theRect->l) < (fa->light_s + smax))
				theRect->w = (fa->light_s-theRect->l)+smax;
			if ((theRect->h + theRect->t) < (fa->light_t + tmax))
				theRect->h = (fa->light_t-theRect->t)+tmax;
			base = lightmaps + fa->lightmaptexturenum*lightmap_bytes*BLOCK_WIDTH*BLOCK_HEIGHT;
			base += fa->light_t * BLOCK_WIDTH * lightmap_bytes + fa->light_s * lightmap_bytes;
			R_BuildLightMap (fa, base, BLOCK_WIDTH*lightmap_bytes);
		}
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
	texture_t	*t;
	byte		*base;
	int			maps;
	glRect_t	*theRect;
	int smax, tmax;

	if (r_vertex.value)
		return;

	c_brush_polys++;

	if (fa->flags & ( SURF_DRAWSKY | SURF_DRAWTURB) )
		return;

	fa->polys->chain = lightmap_polys[fa->lightmaptexturenum];
	lightmap_polys[fa->lightmaptexturenum] = fa->polys;

	// check for lightmap modification
	for (maps = 0 ; maps < MAXLIGHTMAPS && fa->styles[maps] != 255 ;
		 maps++)
		if (d_lightstylevalue[fa->styles[maps]] != fa->cached_light[maps])
			goto dynamic;

	if (fa->dlightframe == r_framecount	// dynamic this frame
		|| fa->cached_dlight)			// dynamic previously
	{
dynamic:
		if (r_dynamic.value)
		{
			lightmap_modified[fa->lightmaptexturenum] = true;
			theRect = &lightmap_rectchange[fa->lightmaptexturenum];
			if (fa->light_t < theRect->t) {
				if (theRect->h)
					theRect->h += theRect->t - fa->light_t;
				theRect->t = fa->light_t;
			}
			if (fa->light_s < theRect->l) {
				if (theRect->w)
					theRect->w += theRect->l - fa->light_s;
				theRect->l = fa->light_s;
			}
			smax = (fa->extents[0]>>4)+1;
			tmax = (fa->extents[1]>>4)+1;
			if ((theRect->w + theRect->l) < (fa->light_s + smax))
				theRect->w = (fa->light_s-theRect->l)+smax;
			if ((theRect->h + theRect->t) < (fa->light_t + tmax))
				theRect->h = (fa->light_t-theRect->t)+tmax;
			base = lightmaps + fa->lightmaptexturenum*lightmap_bytes*BLOCK_WIDTH*BLOCK_HEIGHT;
			base += fa->light_t * BLOCK_WIDTH * lightmap_bytes + fa->light_s * lightmap_bytes;
			R_BuildLightMap (fa, base, BLOCK_WIDTH*lightmap_bytes);
		}
	}
}

/*
================
R_MirrorChain
================
*/
void R_MirrorChain (msurface_t *s)
{
	if (mirror)
		return;
	mirror = true;
	mirror_plane = s->plane;
}


#if 0
/*
================
R_DrawWaterSurfaces
================
*/
void R_DrawWaterSurfaces (void)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;

	if (r_wateralpha.value == 1.0)
		return;

	//
	// go back to the world matrix
	//
	qglLoadMatrixf (r_world_matrix);

	qglEnable (GL_BLEND);
	qglColor4f (1,1,1,r_wateralpha.value);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (i=0 ; i<cl.worldmodel->numtextures ; i++)
	{
		t = cl.worldmodel->textures[i];
		if (!t)
			continue;
		s = t->texturechain;
		if (!s)
			continue;
		if ( !(s->flags & SURF_DRAWTURB) )
			continue;

		// set modulate mode explicitly
		GL_Bind (t->gl_texturenum);

		for ( ; s ; s=s->texturechain)
			R_RenderBrushPoly (s);

		t->texturechain = NULL;
	}

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	qglColor4f (1,1,1,1);
	qglDisable (GL_BLEND);
}
#else
/*
================
R_DrawWaterSurfaces
================
*/
void R_DrawWaterSurfaces (void)
{
	int			i;
	msurface_t	*s;
	texture_t	*t;

	if (r_wateralpha.value == 1.0 && gl_texsort.value)
		return;

	//
	// go back to the world matrix
	//

	qglLoadMatrixf (r_world_matrix);

	if (r_wateralpha.value < 1.0) {
		qglEnable (GL_BLEND);
		qglColor4f (1,1,1,r_wateralpha.value);
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	if (!gl_texsort.value) {
		if (!waterchain)
			return;

		for ( s = waterchain ; s ; s=s->texturechain) {
			GL_Bind (s->texinfo->texture->gl_texturenum);
			EmitWaterPolys (s);
		}
		
		waterchain = NULL;
	} else {

		for (i=0 ; i<cl.worldmodel->numtextures ; i++)
		{
			t = cl.worldmodel->textures[i];
			if (!t)
				continue;
			s = t->texturechain;
			if (!s)
				continue;
			if ( !(s->flags & SURF_DRAWTURB ) )
				continue;

			// set modulate mode explicitly
			
			GL_Bind (t->gl_texturenum);

			for ( ; s ; s=s->texturechain)
				EmitWaterPolys (s);
			
			t->texturechain = NULL;
		}

	}

	if (r_wateralpha.value < 1.0) {
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		qglColor4f (1,1,1,1);
		qglDisable (GL_BLEND);
	}

}

#endif

/*
================
DrawTextureChains
================
*/
void DrawTextureChains (void)
{
	int		i;
	msurface_t	*s;
	texture_t	*t;

	if (!gl_texsort.value)
	{
		GL_DisableMultitexture();

		if (skychain) {
			R_DrawSkyChain(skychain);
			skychain = NULL;
		}

		return;
	} 

	for (i=0 ; i<cl.worldmodel->numtextures ; i++)
	{
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
//		else if (i == mirrortexturenum && r_mirroralpha.value != 1.0)
//		{
//			R_MirrorChain (s);
//			continue;
//		}
		else
		{
			if ((s->flags & SURF_DRAWTURB) && r_wateralpha.value != 1.0)
				continue;	// draw translucent water later
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
			if (gl_texsort.value)
				R_RenderBrushPoly (psurf);
			else
				R_DrawSequentialPoly (psurf);
		}
	}

	R_RenderDelayPolyTris();

	R_BlendLightmaps ();

	qglPopMatrix ();
}

/*
=============================================================

	WORLD MODEL

=============================================================
*/

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
	double		d, dot;
	vec3_t		mins, maxs;

	if (node->contents == CONTENTS_SOLID)
		return;		// solid

	if (node->visframe != r_visframecount)
		return;
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

	if (dot >= 0)
		side = 0;
	else
		side = 1;

// recurse down the children, front side first
	R_RecursiveWorldNode (node->children[side]);

// draw stuff
	c = node->numsurfaces;

	if (c)
	{
		surf = cl.worldmodel->surfaces + node->firstsurface;

		if (dot < 0 -BACKFACE_EPSILON)
			side = SURF_PLANEBACK;
		else if (dot > BACKFACE_EPSILON)
			side = 0;
		{
			for ( ; c ; c--, surf++)
			{
				if (surf->visframe != r_framecount)
					continue;

				// don't backface underwater surfaces, because they warp
				if ( !(surf->flags & SURF_UNDERWATER) && ( (dot < 0) ^ !!(surf->flags & SURF_PLANEBACK)) )
					continue;		// wrong side

				// if sorting by texture, just store it out
				if (gl_texsort.value)
				{
//					if (!mirror
//					|| surf->texinfo->texture != cl.worldmodel->textures[mirrortexturenum])
//					{
						surf->texturechain = surf->texinfo->texture->texturechain;
						surf->texinfo->texture->texturechain = surf;
//					}
				} else if (surf->flags & SURF_DRAWSKY) {
					surf->texturechain = skychain;
					skychain = surf;
				} else if (surf->flags & SURF_DRAWTURB) {
					surf->texturechain = waterchain;
					waterchain = surf;
				} else
				{
					R_DrawSequentialPoly (surf);
				}
			}
		}

	}

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

	qglColor3f (1,1,1);
	memset (lightmap_polys, 0, sizeof(lightmap_polys));
#ifdef QUAKE2
	R_ClearSkyBox ();
#endif

	R_RecursiveWorldNode (cl.worldmodel->nodes);

	DrawTextureChains ();

	R_BlendLightmaps ();

#ifdef QUAKE2
	R_DrawSkyBox ();
#endif

	currententity_world = false;

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
	byte	solid[4096];

	if (r_oldviewleaf == r_viewleaf && !r_novis.value)
		return;
	
	if (mirror)
		return;

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

	if(r_vertex.value)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		qglShadeModel (GL_SMOOTH);
	}

	//
	// draw texture
	//
	poly = Hunk_Alloc (sizeof(glpoly_t) + (lnumverts-4) * VERTEXSIZE*sizeof(float));
	poly->next = fa->polys;
	poly->flags = fa->flags;
	poly->prim = GL_POLYGON;
	fa->polys = poly;
	poly->numverts = lnumverts;

//	VectorZero(porg);
	porg[0]=0;
	porg[1]=0;
	porg[2]=0;

	for (i=0 ; i<lnumverts ; i++)
	{
		lindex = currentmodel->surfedges[fa->firstedge + i];
		if (lindex > 0)
		{
			r_pedge = &pedges[lindex];
			vec = r_pcurrentvertbase[r_pedge->v[0]].position;
//			vec1 = r_pcurrentvertbase[r_pedge->v[1]].position;
		}
		else
		{
			r_pedge = &pedges[-lindex];
			vec = r_pcurrentvertbase[r_pedge->v[1]].position;
//			vec1 = r_pcurrentvertbase[r_pedge->v[0]].position;
		}
		
		VectorAdd(porg, vec, porg);
	}
	VectorScale(porg, 1.0/lnumverts, porg);

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

		if(!r_vertex.value)
		{
			//
			// lightmap texture coordinates
			//
			s = DotProduct (vec, fa->texinfo->vecs[0]) +
				fa->texinfo->vecs[0][3];
			s -= fa->texturemins[0];
			s += fa->light_s*16;
			s += 8;
			s /= BLOCK_WIDTH*16; //fa->texinfo->texture->width;

			t = DotProduct (vec, fa->texinfo->vecs[1]) +
				fa->texinfo->vecs[1][3];
			t -= fa->texturemins[1];
			t += fa->light_t*16;
			t += 8;
			t /= BLOCK_HEIGHT*16; //fa->texinfo->texture->height;
		}

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
			VectorNormalize(pdz);

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

		((int *)(poly->verts[i]))[7] = k;
	}

	//
	// remove co-linear points - Ed
	//
	if (!gl_keeptjunctions.value && !(fa->flags & SURF_UNDERWATER) )
	{
		for (i = 0 ; i < lnumverts ; ++i)
		{
			vec3_t v1, v2;
			float *prev, *this, *next;
			float f;

			prev = poly->verts[(i + lnumverts - 1) % lnumverts];
			this = poly->verts[i];
			next = poly->verts[(i + 1) % lnumverts];

			VectorSubtract( this, prev, v1 );
			VectorNormalize( v1 );
			VectorSubtract( next, prev, v2 );
			VectorNormalize( v2 );

			// skip co-linear points
			#define COLINEAR_EPSILON 0.001
			if ((fabs( v1[0] - v2[0] ) <= COLINEAR_EPSILON) &&
				(fabs( v1[1] - v2[1] ) <= COLINEAR_EPSILON) && 
				(fabs( v1[2] - v2[2] ) <= COLINEAR_EPSILON))
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

}

/*
========================
GL_CreateSurfaceLightmap
========================
*/
void GL_CreateSurfaceLightmap (msurface_t *surf)
{
	int		smax, tmax, s, t, l, i;
	byte	*base;

 	if (r_vertex.value)
		return;

	if (surf->flags & (SURF_DRAWSKY|SURF_DRAWTURB))
		return;

	smax = (surf->extents[0]>>4)+1;
	tmax = (surf->extents[1]>>4)+1;

	surf->lightmaptexturenum = AllocBlock (smax, tmax, &surf->light_s, &surf->light_t);
	base = lightmaps + surf->lightmaptexturenum*lightmap_bytes*BLOCK_WIDTH*BLOCK_HEIGHT;
	base += (surf->light_t * BLOCK_WIDTH + surf->light_s) * lightmap_bytes;
	R_BuildLightMap (surf, base, BLOCK_WIDTH*lightmap_bytes);
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

// 	if (r_vertex.value)
//		return;

	r_framecount = 1;		// no dlightcache

 	if (!r_vertex.value)
 	{
		if(!lightmaps)
		{
			lightmaps = malloc(4*MAX_LIGHTMAPS*BLOCK_WIDTH*BLOCK_HEIGHT);
		}
	}

	if (!lightmap_textures)
	{
		lightmap_textures = texture_extension_number;
		texture_extension_number += MAX_LIGHTMAPS;
	}

	gl_lightmap_format = GL_LUMINANCE;
	// default differently on the Permedia
	if (isPermedia)
		gl_lightmap_format = GL_RGBA;

	if (COM_CheckParm ("-lm_1"))
		gl_lightmap_format = GL_LUMINANCE;
	if (COM_CheckParm ("-lm_a"))
		gl_lightmap_format = GL_ALPHA;
	if (COM_CheckParm ("-lm_i"))
		gl_lightmap_format = GL_INTENSITY;
	if (COM_CheckParm ("-lm_2"))
		gl_lightmap_format = GL_RGBA4;
	if (COM_CheckParm ("-lm_4"))
		gl_lightmap_format = GL_RGBA;

	switch (gl_lightmap_format)
	{
	case GL_RGBA:
		lightmap_bytes = 4;
		break;
	case GL_RGBA4:
		lightmap_bytes = 2;
		break;
	case GL_LUMINANCE:
	case GL_INTENSITY:
	case GL_ALPHA:
		lightmap_bytes = 1;
		break;
	}

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

 	if (r_vertex.value)
		return;


 	if (!gl_texsort.value)
 		GL_SelectTexture(TEXTURE1_SGIS);

	//
	// upload all lightmaps that were filled
	//
	for (i=0 ; i<MAX_LIGHTMAPS ; i++)
	{
		if (!allocated[i][0])
			break;		// no more used
		lightmap_modified[i] = false;
		lightmap_rectchange[i].l = BLOCK_WIDTH;
		lightmap_rectchange[i].t = BLOCK_HEIGHT;
		lightmap_rectchange[i].w = 0;
		lightmap_rectchange[i].h = 0;
		GL_Bind(lightmap_textures + i);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		qglTexImage2D (GL_TEXTURE_2D, 0, lightmap_bytes,
			BLOCK_WIDTH, BLOCK_HEIGHT, 0, 
			gl_lightmap_format, GL_UNSIGNED_BYTE,
			lightmaps+i*BLOCK_WIDTH*BLOCK_HEIGHT*lightmap_bytes);
	}

 	if (!gl_texsort.value)
 		GL_SelectTexture(TEXTURE0_SGIS);

}


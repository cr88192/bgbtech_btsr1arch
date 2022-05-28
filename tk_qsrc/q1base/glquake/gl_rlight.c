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
// r_light.c

#include "quakedef.h"

int	r_dlightframecount;


/*
==================
R_AnimateLight
==================
*/
void R_AnimateLight (void)
{
	int			i,j,k;
	for (j=0 ; j<MAX_LIGHTSTYLES ; j++)
		d_lightstylevalue[j] = 256;
}

/*
=============================================================================

DYNAMIC LIGHTS BLEND RENDERING

=============================================================================
*/

void AddLightBlend (float r, float g, float b, float a2)
{
	float	a;

	v_blend[3] = a = v_blend[3] + a2*(1-v_blend[3]);

	a2 = a2/a;

	v_blend[0] = v_blend[1]*(1-a2) + r*a2;
	v_blend[1] = v_blend[1]*(1-a2) + g*a2;
	v_blend[2] = v_blend[2]*(1-a2) + b*a2;
}

void R_RenderDlight (dlight_t *light)
{
	int		i, j;
	float	a;
	vec3_t	v;
	float	rad;

	rad = light->radius * 0.35;

	VectorSubtract (light->origin, r_origin, v);
	if (Length (v) < rad)
	{	// view is inside the dlight
		AddLightBlend (1, 0.5, 0, light->radius * 0.0003);
		return;
	}

	qglBegin (GL_TRIANGLE_FAN);
	qglColor3f (0.2,0.1,0.0);
	for (i=0 ; i<3 ; i++)
		v[i] = light->origin[i] - vpn[i]*rad;
	qglVertex3fv (v);
	qglColor3f (0,0,0);
	for (i=16 ; i>=0 ; i--)
	{
		a = i/16.0 * M_PI*2;
		for (j=0 ; j<3 ; j++)
		{
//			v[j] = light->origin[j] + vright[j]*cos(a)*rad
//				+ vup[j]*sin(a)*rad;
			v[j] = light->origin[j] + vright[j]*qgl_fastcos(a)*rad
				+ vup[j]*qgl_fastsin(a)*rad;
		}
		qglVertex3fv (v);
	}
	qglEnd ();
}

/*
=============
R_RenderDlights
=============
*/
void R_RenderDlights (void)
{
}


/*
=============================================================================

DYNAMIC LIGHTS

=============================================================================
*/

/*
=============
R_MarkLights
=============
*/
void R_MarkLights (dlight_t *light, int bit, mnode_t *node)
{
}


/*
=============
R_PushDlights
=============
*/
void R_PushDlights (void)
{
#if 0
	int		i;
	dlight_t	*l;

	if (gl_flashblend.value)
		return;

	r_dlightframecount = r_framecount + 1;	// because the count hasn't
											//  advanced yet for this frame
	l = cl_dlights;

	for (i=0 ; i<MAX_DLIGHTS ; i++, l++)
	{
		if (l->die < cl.time || !l->radius)
			continue;
		R_MarkLights ( l, 1<<i, cl.worldmodel->nodes );
	}
#endif
}


/*
=============================================================================

LIGHT SAMPLING

=============================================================================
*/

mplane_t		*lightplane;
vec3_t			lightspot;

int RecursiveLightPoint (mnode_t *node, vec3_t start, vec3_t end)
{
	int			r;
	float		front, back, frac;
	int			side;
	mplane_t	*plane;
	vec3_t		mid;
	msurface_t	*surf;
	int			s, t, ds, dt;
	int			i;
	mtexinfo_t	*tex;
	byte		*lightmap;
	unsigned	scale;
	int			maps, step;

	if (node->contents < 0)
		return -1;		// didn't hit anything
	
// calculate mid point

// FIXME: optimize for axial
	plane = node->plane;
	front = DotProduct (start, plane->normal) - plane->dist;
	back = DotProduct (end, plane->normal) - plane->dist;
//	front = _DotProduct (start, plane->normal) - plane->dist;
//	back = _DotProduct (end, plane->normal) - plane->dist;
	side = front < 0;
	
	if ( (back < 0) == side)
		return RecursiveLightPoint (node->children[side], start, end);
	
	frac = front / (front-back);
	mid[0] = start[0] + (end[0] - start[0])*frac;
	mid[1] = start[1] + (end[1] - start[1])*frac;
	mid[2] = start[2] + (end[2] - start[2])*frac;
	
// go down front side	
	r = RecursiveLightPoint (node->children[side], start, mid);
	if (r >= 0)
		return r;		// hit something
		
	if ( (back < 0) == side )
		return -1;		// didn't hit anuthing
		
// check for impact on this node
	VectorCopy (mid, lightspot);
	lightplane = plane;

	surf = cl.worldmodel->surfaces + node->firstsurface;
	for (i=0 ; i<node->numsurfaces ; i++, surf++)
	{
		if (surf->flags & SURF_DRAWTILED)
			continue;	// no lightmaps

		tex = surf->texinfo;
		
		s = DotProduct (mid, tex->vecs[0]) + tex->vecs[0][3];
		t = DotProduct (mid, tex->vecs[1]) + tex->vecs[1][3];
//		s = _DotProduct (mid, tex->vecs[0]) + tex->vecs[0][3];
//		t = _DotProduct (mid, tex->vecs[1]) + tex->vecs[1][3];

		if ((s < surf->texturemins[0]) ||
			(t < surf->texturemins[1]))
			continue;
		
		ds = s - surf->texturemins[0];
		dt = t - surf->texturemins[1];
		
		if ( (ds > surf->extents[0]) || (dt > surf->extents[1]) )
			continue;

		if (!surf->samples)
			return 0;

		ds >>= 4;
		dt >>= 4;

		lightmap = surf->samples;
		r = 0;
//		if (lightmap)
		if(1)
		{

			lightmap += dt * ((surf->extents[0]>>4)+1) + ds;
			step =	((surf->extents[0]>>4)+1) *
					((surf->extents[1]>>4)+1);

			for (maps = 0 ; maps < MAXLIGHTMAPS && surf->styles[maps] != 255 ;
					maps++)
			{
//				scale = d_lightstylevalue[surf->styles[maps]];
//				r += *lightmap * scale;
//				lightmap += ((surf->extents[0]>>4)+1) *
//						((surf->extents[1]>>4)+1);

				r += *lightmap * 256;
				lightmap += step;
			}
			
			r >>= 8;
		}
		
		return r;
	}

// go down back side
	return RecursiveLightPoint (node->children[!side], mid, end);
}

int R_LightPointOrg (vec3_t p)
{
	vec3_t		strt;
	vec3_t		end;
	int			r;
	
	if (!cl.worldmodel->lightdata)
		return 255;

//	return 255;

	strt[0] = p[0];
	strt[1] = p[1];
	strt[2] = p[2] + 2;
	
	end[0] = p[0];
	end[1] = p[1];
//	end[2] = p[2] - 2048;
	end[2] = p[2] - 512;
	
//	r = RecursiveLightPoint (cl.worldmodel->nodes, p, end);
	r = RecursiveLightPoint (cl.worldmodel->nodes, strt, end);
	
	if (r == -1)
		r = 0;

	return r;
}

int R_LightPointDirOrg (vec3_t p, vec3_t dir)
{
	vec3_t		strt;
	vec3_t		end;
	int			r;
	
	if (!cl.worldmodel->lightdata)
		return 255;

	strt[0] = p[0] + dir[0]*2;
	strt[1] = p[1] + dir[1]*2;
	strt[2] = p[2] + dir[2]*2;
	
//	end[0] = p[0] - dir[0]*2048;
//	end[1] = p[1] - dir[1]*2048;
//	end[2] = p[2] - dir[2]*2048;

	end[0] = p[0] - dir[0]*512;
	end[1] = p[1] - dir[1]*512;
	end[2] = p[2] - dir[2]*512;
	
//	r = RecursiveLightPoint (cl.worldmodel->nodes, p, end);
	r = RecursiveLightPoint (cl.worldmodel->nodes, strt, end);
	
	if (r == -1)
		r = 0;

	return r;
}

#if 1
//int R_LightPoint (vec3_t p)
//{
//	return(R_LightPointOrg(p));
//}

int R_LightPointDir (vec3_t p, vec3_t dir)
{
	int l, l1;
//	return(255);

	l=R_LightPointDirOrg(p, dir);
	l1=R_LightPoint(p);
	if(l1>l)
		l=(l+l1)>>1;
	return(l);

//	return(R_LightPointDirOrg(p, dir));
//	return(R_LightPoint(p));
}


// byte *lightcube;
int *lightcube;
byte lightcube_ready;

int R_LightPoint (vec3_t p)
{
	vec3_t p1, p2;
	int x, y, z, w;
	int i, j, k, k1, k2, k3, l, l1;
	
//	return(255);
	
	if(!lightcube)
	{
		lightcube=malloc(4096*sizeof(int));
//		lightcube=malloc(1024*sizeof(int));
		lightcube_ready = 0;
	}
	
	if(!lightcube_ready)
	{
		memset(lightcube, 0x00, 4096*sizeof(int));
//		memset(lightcube, 0x00, 1024*sizeof(int));
		lightcube_ready = 1;
	}
	
//	x=(p[0]/64)+16;
//	y=(p[1]/64)+16;
//	z=(p[2]/64)+16;
//	w=(z*32+y)*32+x;

//	x=((int)(p[0]/16))&255;
//	y=((int)(p[1]/16))&255;
//	z=((int)(p[2]/16))&255;

//	x=((int)(p[0]/32))&255;
//	y=((int)(p[1]/32))&255;
//	z=((int)(p[2]/32))&255;

	x=((int)(p[0]*(1.0/32)))&255;
	y=((int)(p[1]*(1.0/32)))&255;
	z=((int)(p[2]*(1.0/32)))&255;

	p1[0]=x*32+16;
	p1[1]=y*32+16;
	p1[2]=z*32+16;

	j=(x<<16)|(y<<8)|z;

//	w=(x*251+y)*251+z;
//	w=w*251; w=w*251;
//	w=((x+y+z)<<2)^(x^y);
//	w=((x+y+z)<<3)^(x^y);
	w=j^(j>>12);

	w=w*65521;
//	w=(w>>16)&4095;
	w=(w>>16)&1023;

//	j=(x<<16)|(y<<8)|z;

	k=lightcube[w];
	l=(k>>24)&255;
//	if(l<=0)
	if((k&0x00FFFFFF)!=j)
	{
		k1=lightcube[w|(1<<10)];
		if((k1&0x00FFFFFF)==j)
		{
			l=(k1>>24)&255;
			lightcube[w]=k1;
			lightcube[w|(1<<10)]=k;
			return(l);
		}

		k2=lightcube[w|(2<<10)];
		if((k2&0x00FFFFFF)==j)
		{
			l=(k2>>24)&255;
			lightcube[w]=k2;
			lightcube[w|(1<<10)]=k;
			lightcube[w|(2<<10)]=k1;
			return(l);
		}

		k3=lightcube[w|(3<<10)];
		if((k2&0x00FFFFFF)==j)
		{
			l=(k3>>24)&255;
			lightcube[w]=k3;
			lightcube[w|(1<<10)]=k;
			lightcube[w|(2<<10)]=k1;
			lightcube[w|(3<<10)]=k2;
			return(l);
		}

		l=127;

#if 1
		l=R_LightPointOrg(p1);

		if(l<=0)
		{
			l=R_LightPointOrg(p);
		}
		
		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[0]+=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}

		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[0]-=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}

		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[1]+=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}

		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[1]-=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}

		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[2]+=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}

		if(l<=0)
		{
			VectorCopy(p1, p2);
			p2[2]-=16;
			l1=R_LightPointOrg(p2);
			if(l1>l)
				l=l1;
		}
#endif

//		if(l<1)l=1;
		lightcube[w]=(l<<24)|j;
		lightcube[w|(1<<10)]=k;
		lightcube[w|(2<<10)]=k1;
		lightcube[w|(3<<10)]=k2;
	}

	return(l);
}

#endif

#if 0
byte *lightcube;
byte lightcube_ready = false;

void R_InitLightcube()
{
	vec3_t p;
	int l;
	int x, y, z, w;
	
	if(!lightcube)
	{
		lightcube=malloc(32*32*32);
		memset(lightcube, 0xFF, 32*32*32);
		lightcube_ready = false;
	}
	
	if(lightcube_ready)
		return;
	lightcube_ready = 1;
		
	for(x=0; x<32; x++)
		for(y=0; y<32; y++)
			for(z=0; z<32; z++)
	{
		p[0]=(x-16)*64;
		p[1]=(y-16)*64;
		p[2]=(z-16)*64;

		w=(z*32+y)*32+x;
		
		l = R_LightPointOrg(p);
		l = 64+(l>>3);
		lightcube[w] = l;
	}
}

int R_LightPoint (vec3_t p)
{
	int x, y, z, w;
	
	R_InitLightcube();
	
	x=(p[0]/64)+16;
	y=(p[1]/64)+16;
	z=(p[2]/64)+16;
	w=(z*32+y)*32+x;

	return(lightcube[w]);
}

int R_LightPointDir (vec3_t p, vec3_t dir)
{
	return(R_LightPoint(p));
}
#endif

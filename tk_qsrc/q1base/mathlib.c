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
// mathlib.c -- math primitives

#include <math.h>
#include "quakedef.h"

void Sys_Error (char *error, ...);

vec3_t vec3_origin = {0,0,0};
int nanmask = 255<<23;

/*-----------------------------------------------------------------*/

#define DEG2RAD( a ) ( a * M_PI ) / 180.0F

void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal )
{
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom = 1.0F / DotProduct( normal, normal );

	d = DotProduct( normal, p ) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

/*
** assumes "src" is normalized
*/
void PerpendicularVector( vec3_t dst, const vec3_t src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for ( pos = 0, i = 0; i < 3; i++ )
	{
		if ( fabs( src[i] ) < minelem )
		{
			pos = i;
			minelem = fabs( src[i] );
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	/*
	** project the point onto the plane defined by src
	*/
	ProjectPointOnPlane( dst, tempvec, src );

	/*
	** normalize the result
	*/
	VectorNormalize( dst );
}

#ifdef _WIN32
#pragma optimize( "", off )
#endif


void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees )
{
	float	m[3][3];
	float	im[3][3];
	float	zrot[3][3];
	float	tmpmat[3][3];
	float	rot[3][3];
	int	i;
	vec3_t vr, vup, vf;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector( vr, dir );
	CrossProduct( vr, vf, vup );

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy( im, m, sizeof( im ) );

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset( zrot, 0, sizeof( zrot ) );
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	zrot[0][0] = cos( DEG2RAD( degrees ) );
	zrot[0][1] = sin( DEG2RAD( degrees ) );
	zrot[1][0] = -sin( DEG2RAD( degrees ) );
	zrot[1][1] = cos( DEG2RAD( degrees ) );

	R_ConcatRotations( m, zrot, tmpmat );
	R_ConcatRotations( tmpmat, im, rot );

	for ( i = 0; i < 3; i++ )
	{
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}

#ifdef _WIN32
#pragma optimize( "", on )
#endif

/*-----------------------------------------------------------------*/


float	anglemod(float a)
{
#if 0
	if (a >= 0)
		a -= 360*(int)(a/360);
	else
		a += 360*( 1 + (int)(-a/360) );
#endif
	a = (360.0/65536) * ((int)(a*(65536/360.0)) & 65535);
	return a;
}

/*
==================
BOPS_Error

Split out like this for ASM to call.
==================
*/
void BOPS_Error (void)
{
	Sys_Error ("BoxOnPlaneSide:  Bad signbits");
}

#if 1

#ifdef __BJX2__
void _BoxCornersForSignbits (
	vec3_t emins, vec3_t emaxs,
	vec3_t cmins, vec3_t cmaxs,
	int signbits);
__asm {
_BoxCornersForSignbits:
#if 0
	MOV.L	(R4, 0), R16
	MOV.L	(R5, 0), R17
	TEST	1, R20
	CSELT	R16, R17, R18
	CSELT	R17, R16, R19
	MOV.L	R18, (R6, 0)
	MOV.L	R19, (R7, 0)

	MOV.L	(R4, 4), R16
	MOV.L	(R5, 4), R17
	TEST	2, R20
	CSELT	R16, R17, R18
	CSELT	R17, R16, R19
	MOV.L	R18, (R6, 4)
	MOV.L	R19, (R7, 4)
#endif

#if 1
	MOV.Q	(R4, 0), R16
	MOV.Q	(R5, 0), R17
	TEST	1, R20
	CSELT	R16, R17, R18	|	CSELT	R17, R16, R19
	TEST	2, R20
	CSELT	R16, R17, R22	|	CSELT	R17, R16, R23
	MOVHLD	R22, R18, R18	|	MOVHLD	R23, R19, R19
	MOV.Q	R18, (R6, 0)
	MOV.Q	R19, (R7, 0)
#endif

	MOV.L	(R4, 8), R16
	MOV.L	(R5, 8), R17
	TEST	4, R20
	CSELT	R16, R17, R18
	CSELT	R17, R16, R19
	MOV.L	R18, (R6, 8)
	MOV.L	R19, (R7, 8)
	
	RTSU
};

#else

void _BoxCornersForSignbits (
	vec3_t emins, vec3_t emaxs,
	vec3_t cmins, vec3_t cmaxs,
	int signbits)
{
	if (signbits & 1)
		{ cmaxs[0] = emins[0];	cmins[0] = emaxs[0]; }
	else
		{ cmins[0] = emins[0];	cmaxs[0] = emaxs[0]; }

	if (signbits & 2)
		{ cmaxs[1] = emins[1];	cmins[1] = emaxs[1]; }
	else
		{ cmins[1] = emins[1];	cmaxs[1] = emaxs[1]; }

	if (signbits & 4)
		{ cmaxs[2] = emins[2];	cmins[2] = emaxs[2]; }
	else
		{ cmins[2] = emins[2];	cmaxs[2] = emaxs[2]; }
}
#endif



#ifdef __BJX2__
// #if 0

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, mplane_t *p);

__asm {
BoxOnPlaneSide:
	MOVU.B		(R6, offsetof mplane_s signbits), R7
									MOV.Q		(R4, 0), R16
									MOV.Q		(R5, 0), R17
									TEST		1, R7
	CSELT		R16, R17, R18	|	CSELT		R17, R16, R19
									TEST		2, R7
	CSELT		R16, R17, R22	|	CSELT		R17, R16, R23
	MOVHLD		R22, R18, R20	|	MOVU.L		(R4, 8), R16
	MOVHLD		R23, R19, R22	|	MOVU.L		(R5, 8), R17
									TEST		4, R7
	CSELT		R16, R17, R21	|	CSELT		R17, R16, R23
	/* R21:R20 = cmins, R23:R22=cmaxs */
									MOV.Q		(R6, 0), R18
									MOV.Q		(R6, 8), R3
	/* R19:R18 = p->normal, unaligned */
	EXTU.L		R3, R19			|	FLDCFH		R3, R7
	/* R7 = p->dist */

	/* dist1 = _DotProduct (p->normal, cmaxs); */
	PMULX.F		R22, R18, R16
//	PMULX.FA	R22, R18, R16
	PADD.F		R16, R17, R3
//	PADD.FA		R16, R17, R3
	FLDCF		R3, R16
	FLDCFH		R3, R17
.ifarch has_fpvsf_sp
	FADDA		R16, R17, R4
.else
	FADD		R16, R17, R4
.endif
	/* R4 = dist1 */

	/* dist2 = _DotProduct (p->normal, cmins); */
	PMULX.F		R20, R18, R16
//	PMULX.FA	R20, R18, R16
	PADD.F		R16, R17, R3
//	PADD.FA		R16, R17, R3
	FLDCF		R3, R16
	FLDCFH		R3, R17
.ifarch has_fpvsf_sp
	FADDA		R16, R17, R5
.else
	FADD		R16, R17, R5
.endif
	/* R5 = dist2 */

	MOV			0, R2
	FCMPGT		R4, R7
	OR?F		R2, 1, R2
	FCMPGT		R5, R7
	OR?T		R2, 2, R2

	RTSU
};

#else

/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2
==================
*/

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, mplane_t *p)
{
	vec3_t	cmins, cmaxs;
	float	dist1, dist2;
	int		sides;

	_BoxCornersForSignbits(emins, emaxs, cmins, cmaxs, p->signbits);
	dist1 = _DotProduct (p->normal, cmaxs);
	dist2 = _DotProduct (p->normal, cmins);
	sides = 0;
	if (dist1 >= p->dist)
		sides = 1;
	if (dist2 < p->dist)
		sides |= 2;
	return sides;
}

#endif

#endif

//#if	!id386
// #if 1
#if 0

/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2
==================
*/

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, mplane_t *p)
{
//	vec3_t	corners[2];
	vec3_t	cmins, cmaxs;
	float	dist1, dist2;
	int		sides;

#if 0	// this is done by the BOX_ON_PLANE_SIDE macro before calling this
		// function
// fast axial cases
	if (p->type < 3)
	{
		if (p->dist <= emins[p->type])
			return 1;
		if (p->dist >= emaxs[p->type])
			return 2;
		return 3;
	}
#endif

#if 1
// general case
	switch (p->signbits)
	{
	case 0:
dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 1:
dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 2:
dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 3:
dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 4:
dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 5:
dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 6:
dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	case 7:
dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	default:
		dist1 = dist2 = 0;		// shut up compiler
		BOPS_Error ();
		break;
	}
#endif

#if 0
//	_BoxCornersForSignbits(emins, emaxs, corners[1], corners[0], p->signbits);
//	dist1 = _DotProduct (p->normal, corners[0]);
//	dist2 = _DotProduct (p->normal, corners[1]);

	_BoxCornersForSignbits(emins, emaxs, cmins, cmaxs, p->signbits);
	dist1 = _DotProduct (p->normal, cmaxs);
	dist2 = _DotProduct (p->normal, cmins);
#endif

#if 0
	int		i;
	vec3_t	corners[2];

	for (i=0 ; i<3 ; i++)
	{
		if (plane->normal[i] < 0)
		{
			corners[0][i] = emins[i];
			corners[1][i] = emaxs[i];
		}
		else
		{
			corners[1][i] = emins[i];
			corners[0][i] = emaxs[i];
		}
	}
	dist = DotProduct (plane->normal, corners[0]) - plane->dist;
	dist2 = DotProduct (plane->normal, corners[1]) - plane->dist;
	sides = 0;
	if (dist1 >= 0)
		sides = 1;
	if (dist2 < 0)
		sides |= 2;

#endif

	sides = 0;
	if (dist1 >= p->dist)
		sides = 1;
	if (dist2 < p->dist)
		sides |= 2;

#ifdef PARANOID
if (sides == 0)
	Sys_Error ("BoxOnPlaneSide: sides==0");
#endif

	return sides;
}

#endif


void AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	
	angle = angles[YAW] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;
	right[0] = (-1*sr*sp*cy+-1*cr*-sy);
	right[1] = (-1*sr*sp*sy+-1*cr*cy);
	right[2] = -1*sr*cp;
	up[0] = (cr*sp*cy+-sr*-sy);
	up[1] = (cr*sp*sy+-sr*cy);
	up[2] = cr*cp;
}

int VectorCompare (vec3_t v1, vec3_t v2)
{
	int		i;
	
	for (i=0 ; i<3 ; i++)
		if (v1[i] != v2[i])
			return 0;
			
	return 1;
}

void VectorMA (vec3_t veca, float scale, vec3_t vecb, vec3_t vecc)
{
	vecc[0] = veca[0] + scale*vecb[0];
	vecc[1] = veca[1] + scale*vecb[1];
	vecc[2] = veca[2] + scale*vecb[2];
}


#ifdef __BJX2__
// #if 0
vec_t _DotProduct (const vec3_t v1, const vec3_t v2);
void _VectorAdd (vec3_t veca, vec3_t vecb, vec3_t out);
void _VectorSubtract (vec3_t veca, vec3_t vecb, vec3_t out);
void _VectorCopy (vec3_t in, vec3_t out);

//vec_t _DotProductFa (const vec3_t v1, const vec3_t v2);
//void _VectorAddFa (vec3_t veca, vec3_t vecb, vec3_t out);
//void _VectorSubtractFa (vec3_t veca, vec3_t vecb, vec3_t out);

__asm {

_VectorAdd:
	MOV.Q		(R4, 0), R20
	MOVU.L		(R4, 8), R21
	MOV.Q		(R5, 0), R22
	MOVU.L		(R5, 8), R23
	PADDX.F		R20, R22, R16
//	PADDX.FA	R20, R22, R16
	MOV.Q		R16, (R6, 0)
	MOVU.L		R17, (R6, 8)
	RTSU

_VectorSubtract:
	MOV.Q		(R4, 0), R20
	MOVU.L		(R4, 8), R21
	MOV.Q		(R5, 0), R22
	MOVU.L		(R5, 8), R23
	PSUBX.F		R20, R22, R16
//	PSUBX.FA	R20, R22, R16
	MOV.Q		R16, (R6, 0)
	MOVU.L		R17, (R6, 8)
	RTSU

_DotProduct:
	MOV.Q		(R4, 0), R20
	MOVU.L		(R4, 8), R21
	MOV.Q		(R5, 0), R22
	MOVU.L		(R5, 8), R23

	PMULX.F		R20, R22, R16
//	PMULX.FA	R20, R22, R16
	PADD.F		R16, R17, R3
//	PADD.FA		R16, R17, R3
	FLDCF		R3, R18
	FLDCFH		R3, R19
.ifarch has_fpvsf_sp
	FADDA		R18, R19, R2
.else
	FADD		R18, R19, R2
.endif
	RTSU

_VectorCopy:
	MOV.Q	(R4, 0), R6
	MOVU.L	(R4, 8), R7
	MOV.Q	R6, (R5, 0)
	MOVU.L	R7, (R5, 8)
	RTSU

};
#else
vec_t _DotProduct (const vec3_t v1, const vec3_t v2)
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void _VectorSubtract (vec3_t veca, vec3_t vecb, vec3_t out)
{
	out[0] = veca[0]-vecb[0];
	out[1] = veca[1]-vecb[1];
	out[2] = veca[2]-vecb[2];
}

void _VectorAdd (vec3_t veca, vec3_t vecb, vec3_t out)
{
	out[0] = veca[0]+vecb[0];
	out[1] = veca[1]+vecb[1];
	out[2] = veca[2]+vecb[2];
}

void _VectorCopy (vec3_t in, vec3_t out)
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}
#endif

#ifdef __BJX2__
double Q_rsqrt_d( double number )
{
	long long i;
	double x2, y;
	const double threehalfs = 1.5;

	x2 = number * 0.5;
	y  = number;
	i  = __float64_getbits(y);
//	i  = * ( long * ) &y;
//	i  = 0x5f3759df - ( i >> 1 );
	i  = 0x5fe6eb3be0004a48ull - ( i >> 1 );
//	y  = * ( float * ) &i;
	y  = __float64_frombits(i);
	y  = y * ( threehalfs - ( x2 * y * y ) );
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

float Q_rsqrt( float number )
{
	unsigned int i;
	float x2, y;
	const double threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * ( unsigned int * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}

#else
double Q_rsqrt_d( double number )
{
	long long i;
	double x2, y;
	const double threehalfs = 1.5;

	x2 = number * 0.5;
	y  = number;
	memcpy(&i, &y, sizeof(double));
	i  = 0x5fe6eb3be0004a48ull - ( i >> 1 );
	memcpy(&y, &i, sizeof(double));
	y  = y * ( threehalfs - ( x2 * y * y ) );
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}

float Q_rsqrt( float number )
{
	unsigned int i;
	float x2, y;
	const double threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * ( unsigned int * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}
#endif

void CrossProduct (vec3_t v1, vec3_t v2, vec3_t cross)
{
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

double sqrt(double x);

vec_t Length(vec3_t v)
{
	int		i;
	float	length;
	
	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);		// FIXME

	return length;
}

float VectorNormalize (vec3_t v)
{
	float	length, ilength;

//	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = DotProduct(v, v);
	length = sqrt (length);		// FIXME
//	ilength = Q_rsqrt(length);

	if (length)
	{
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
		
	return length;

}

float __fpu_frcp_sf(float x);

float VectorNormalizeFast (vec3_t v)
{
	/* Optimized for BJX2... */
	double	length, ilength;
//	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = DotProduct(v, v);
	length = Q_sqrt_fast (length);
//	ilength = Q_rsqrt_d(length);
	ilength = __fpu_frcp_sf (length);
	if (length>0)
	{
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
	return length;
}

float VectorDistance (vec3_t sv, vec3_t ev)
{
	vec3_t	dv;
	float	len;

	VectorSubtract(ev, sv, dv);
	len = DotProduct (dv, dv);
	len = sqrt (len);
	return(len);
}

float VectorDistanceFast (vec3_t sv, vec3_t ev)
{
	vec3_t	dv;
	float	len;

	VectorSubtract(ev, sv, dv);
	len = DotProduct (dv, dv);
	len = Q_sqrt_fast (len);
	return(len);
}

void VectorInverse (vec3_t v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void VectorScale (vec3_t in, vec_t scale, vec3_t out)
{
	out[0] = in[0]*scale;
	out[1] = in[1]*scale;
	out[2] = in[2]*scale;
}


int Q_log2(int val)
{
	int answer=0;
	while (val>>=1)
		answer++;
	return answer;
}

#ifdef __BJX2__
double Q_sqrt_fast( double number )
{
	long long i, j;
	double x0, x1, y, x0r;
	x0 = number;
	x1 = number + 1;
	i  = __float64_getbits(x0);
	i  = 0x1ff8000000000000ull + ( i >> 1 );
	j  = 0x7fd0000000000000ull - i;
	y  = __float64_frombits(i);
	x0r  = __float64_frombits(j);
//	y  = y * ( x1 - x0r*y*y );
	x0r *= x0r;
	y  = y * ( 1.0 + ((x0 - y*y) * x0r) );
	return y;
}
#else
double Q_sqrt_fast( double number )
{
	long long i, j;
	double x0, x1, y, x0r;
	x0 = number;
	x1 = number + 1;
	memcpy(&i, &x0, sizeof(double));
	i  = 0x1ff8000000000000ull + ( i >> 1 );
	j  = 0x7fd0000000000000ull - i;
	memcpy(&y, &i, sizeof(double));
	memcpy(&x0r, &j, sizeof(double));
//	y  = __float64_frombits(i);
//	x0r  = __float64_frombits(j);
//	y  = y * ( x1 - x0r*y*y );
	x0r *= x0r;
	y  = y * ( 1.0 + ((x0 - y*y) * x0r) );
	return y;
}
#endif

void Q_BoxCenter(vec3_t mins, vec3_t maxs, vec3_t org)
{
	org[0] = (mins[0] + maxs[0])*0.5;
	org[1] = (mins[1] + maxs[1])*0.5;
	org[2] = (mins[2] + maxs[2])*0.5;
}

vec_t Q_BoxCenterRadius(vec3_t mins, vec3_t maxs, vec3_t org)
{
	float dx, dy, dz, d;

	org[0] = (mins[0] + maxs[0])*0.5;
	org[1] = (mins[1] + maxs[1])*0.5;
	org[2] = (mins[2] + maxs[2])*0.5;
	
	dx=maxs[0]-org[0];
	dy=maxs[1]-org[1];
	dz=maxs[2]-org[2];
	d=dx*dx+dy*dy+dz*dz;
	return(Q_sqrt_fast(d));
}

/*
================
R_ConcatRotations
================
*/
void R_ConcatRotations (float in1[3][3], float in2[3][3], float out[3][3])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
				in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
				in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
				in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
				in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
				in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
				in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
				in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
				in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
				in1[2][2] * in2[2][2];
}


/*
================
R_ConcatTransforms
================
*/
void R_ConcatTransforms (float in1[3][4], float in2[3][4], float out[3][4])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
				in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
				in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
				in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
				in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
				in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
				in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
				in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
				in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
				in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
				in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
				in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
				in1[2][2] * in2[2][3] + in1[2][3];
}


/*
===================
FloorDivMod

Returns mathematically correct (floor-based) quotient and remainder for
numer and denom, both of which should contain no fractional part. The
quotient must fit in 32 bits.
====================
*/

void FloorDivMod (double numer, double denom, int *quotient,
		int *rem)
{
	int		q, r;
	double	x;

#ifndef PARANOID
	if (denom <= 0.0)
		Sys_Error ("FloorDivMod: bad denominator %d\n", denom);

//	if ((floor(numer) != numer) || (floor(denom) != denom))
//		Sys_Error ("FloorDivMod: non-integer numer or denom %f %f\n",
//				numer, denom);
#endif

	if (numer >= 0.0)
	{

		x = floor(numer / denom);
		q = (int)x;
		r = (int)floor(numer - (x * denom));
	}
	else
	{
	//
	// perform operations with positive values, and fix mod to make floor-based
	//
		x = floor(-numer / denom);
		q = -(int)x;
		r = (int)floor(-numer - (x * denom));
		if (r != 0)
		{
			q--;
			r = (int)denom - r;
		}
	}

	*quotient = q;
	*rem = r;
}


/*
===================
GreatestCommonDivisor
====================
*/
int GreatestCommonDivisor (int i1, int i2)
{
	if (i1 > i2)
	{
		if (i2 == 0)
			return (i1);
		return GreatestCommonDivisor (i2, i1 % i2);
	}
	else
	{
		if (i1 == 0)
			return (i2);
		return GreatestCommonDivisor (i1, i2 % i1);
	}
}


//#if	!id386
#if 1

// TODO: move to nonintel.c

/*
===================
Invert24To16

Inverts an 8.24 value to a 16.16 value
====================
*/

fixed16_t Invert24To16(fixed16_t val)
{
	if (val < 256)
		return (0xFFFFFFFF);

	return (fixed16_t)
			(((double)0x10000 * (double)0x1000000 / (double)val) + 0.5);
}

#endif

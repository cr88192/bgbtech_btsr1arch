#include <math.h>


#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif


#ifndef LIBVECMATH_H
#define LIBVECMATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define INLINE
// #define INLINE inline

#ifndef M_PI
#define M_PI		3.1415926535897932384626433832795
#endif

#ifndef M_BIGVAL
#define M_BIGVAL		1000000000.0	//some large value
#define M_SMALLVAL		0.0000000001	//some small value
#endif

#define DEG(x)		((x)*(PD3D_M_PI/180.0))
#define MIN(x, y)	(((x)<(y))?(x):(y))
#define MAX(x, y)	(((x)>(y))?(x):(y))
#define SQR(x)		((x)*(x))

#define CLAMP(a, m, n)	MIN(MAX(a, m), n)


//macro varieties
#define V2F_COPY(a, b)		(b)[0]=(a)[0];	\
				(b)[1]=(a)[1];
#define V3F_COPY(a, b)		(b)[0]=(a)[0];	\
				(b)[1]=(a)[1];	\
				(b)[2]=(a)[2];
#define V4F_COPY(a, b)		(b)[0]=(a)[0];	\
				(b)[1]=(a)[1];	\
				(b)[2]=(a)[2];	\
				(b)[3]=(a)[3];

#define V2F_ADD(a, b, c)	(c)[0]=(a)[0]+(b)[0];	\
				(c)[1]=(a)[1]+(b)[1];
#define V3F_ADD(a, b, c)	(c)[0]=(a)[0]+(b)[0];	\
				(c)[1]=(a)[1]+(b)[1];	\
				(c)[2]=(a)[2]+(b)[2];
#define V4F_ADD(a, b, c)	(c)[0]=(a)[0]+(b)[0];	\
				(c)[1]=(a)[1]+(b)[1];	\
				(c)[2]=(a)[2]+(b)[2];	\
				(c)[3]=(a)[3]+(b)[3];

#define V2F_SUB(a, b, c)	(c)[0]=(a)[0]-(b)[0];	\
				(c)[1]=(a)[1]-(b)[1];
#define V3F_SUB(a, b, c)	(c)[0]=(a)[0]-(b)[0];	\
				(c)[1]=(a)[1]-(b)[1];	\
				(c)[2]=(a)[2]-(b)[2];
#define V4F_SUB(a, b, c)	(c)[0]=(a)[0]-(b)[0];	\
				(c)[1]=(a)[1]-(b)[1];	\
				(c)[2]=(a)[2]-(b)[2];	\
				(c)[3]=(a)[3]-(b)[3];

#define V2F_SCALE(a, b, c)	(c)[0]=(a)[0]*(b);	\
				(c)[1]=(a)[1]*(b);
#define V3F_SCALE(a, b, c)	(c)[0]=(a)[0]*(b);	\
				(c)[1]=(a)[1]*(b);	\
				(c)[2]=(a)[2]*(b);
#define V4F_SCALE(a, b, c)	(c)[0]=(a)[0]*(b);	\
				(c)[1]=(a)[1]*(b);	\
				(c)[2]=(a)[2]*(b);	\
				(c)[3]=(a)[3]*(b);

#define V2F_SCALEVEC(a, b, c)	(c)[0]=(a)[0]*(b)[0];	\
				(c)[1]=(a)[1]*(b)[1];
#define V3F_SCALEVEC(a, b, c)	(c)[0]=(a)[0]*(b)[0];	\
				(c)[1]=(a)[1]*(b)[1];	\
				(c)[2]=(a)[2]*(b)[2];
#define V4F_SCALEVEC(a, b, c)	(c)[0]=(a)[0]*(b)[0];	\
				(c)[1]=(a)[1]*(b)[1];	\
				(c)[2]=(a)[2]*(b)[2];	\
				(c)[3]=(a)[3]*(b)[3];

#define V2F_DOT(a, b)		((a)[0]*(b)[0]+(a)[1]*(b)[1])
#define V3F_DOT(a, b)		((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define V4F_DOT(a, b)		((a)[0]*(b)[0]+(a)[1]*(b)[1]+ \
				(a)[2]*(b)[2]+(a)[3]*(b)[3])

#define V2F_CROSS(a, b)		((a)[0]*(b)[1]-(a)[1]*(b)[0])
#define V3F_CROSS(a, b, c)	(c)[0]=((a)[1]*(b)[2])-((a)[2]*(b)[1]);	\
				(c)[1]=((a)[2]*(b)[0])-((a)[0]*(b)[2]);	\
				(c)[2]=((a)[0]*(b)[1])-((a)[1]*(b)[0]);

#define V2F_LEN(a)		(sqrt(V2F_DOT(a, a)))
#define V3F_LEN(a)		(sqrt(V3F_DOT(a, a)))
#define V4F_LEN(a)		(sqrt(V4F_DOT(a, a)))

#define V1F_DIST2(a, b)		(((a)-(b))*((a)-(b)))
#define V2F_DIST2(a, b)		(V1F_DIST2((a)[0], (b)[0])+ \
				V1F_DIST2((a)[1], (b)[1]))
#define V3F_DIST2(a, b)		(V1F_DIST2((a)[0], (b)[0])+ \
				V1F_DIST2((a)[1], (b)[1])+ \
				V1F_DIST2((a)[2], (b)[2]))
#define V4F_DIST2(a, b)		(V1F_DIST2((a)[0], (b)[0])+ \
				V1F_DIST2((a)[1], (b)[1])+ \
				V1F_DIST2((a)[2], (b)[2])+ \
				V1F_DIST2((a)[3], (b)[3]))

#define V2F_DIST(a, b)		(sqrt(V2F_DIST2(a, b)))
#define V3F_DIST(a, b)		(sqrt(V3F_DIST2(a, b)))
#define V4F_DIST(a, b)		(sqrt(V4F_DIST2(a, b)))

#define V2F_ZERO(a)		(a)[0]=0; (a)[1]=0;
#define V3F_ZERO(a)		(a)[0]=0; (a)[1]=0; (a)[2]=0;
#define V4F_ZERO(a)		(a)[0]=0; (a)[1]=0; (a)[2]=0; (a)[3]=0;
 
#define V2F_CONST(a, k)		(a)[0]=k; (a)[1]=k;
#define V3F_CONST(a, k)		(a)[0]=k; (a)[1]=k; (a)[2]=k;
#define V4F_CONST(a, k)		(a)[0]=k; (a)[1]=k; (a)[2]=k; (a)[3]=k;

#define V2F_ADDCONST(a, k)	(a)[0]+=k; (a)[1]+=k;
#define V3F_ADDCONST(a, k)	(a)[0]+=k; (a)[1]+=k; (a)[2]+=k;
#define V4F_ADDCONST(a, k)	(a)[0]+=k; (a)[1]+=k; (a)[2]+=k; (a)[3]+=k;

#define V2F_SUBCONST(a, k)	(a)[0]-=k; (a)[1]-=k;
#define V3F_SUBCONST(a, k)	(a)[0]-=k; (a)[1]-=k; (a)[2]-=k;
#define V4F_SUBCONST(a, k)	(a)[0]-=k; (a)[1]-=k; (a)[2]-=k; (a)[3]-=k;

#define V2F_ADDCONSTB(a, k)	(a)[0]+=k; (a)[1]+=k;
#define V3F_ADDCONSTB(a, k)	(a)[0]+=k; (a)[1]+=k; (a)[2]+=k;
#define V4F_ADDCONSTB(a, k)	(a)[0]+=k; (a)[1]+=k; (a)[2]+=k; (a)[3]+=k;


#define V2F_CONSTADD(a, k, c)	\
				(c)[0]=(a)[0]+(k);	\
				(c)[1]=(a)[1]+(k);
#define V3F_CONSTADD(a, k, c)	\
				(c)[0]=(a)[0]+(k);	\
				(c)[1]=(a)[1]+(k);	\
				(c)[2]=(a)[2]+(k);
#define V4F_CONSTADD(a, k, c)	\
				(c)[0]=(a)[0]+(k);	\
				(c)[1]=(a)[1]+(k);	\
				(c)[2]=(a)[2]+(k);	\
				(c)[3]=(a)[3]+(k);

#define V2F_CONSTSUB(a, k, c)	\
				(c)[0]=(a)[0]+(k);	\
				(c)[1]=(a)[1]-(k);
#define V3F_CONSTSUB(a, k, c)	\
				(c)[0]=(a)[0]-(k);	\
				(c)[1]=(a)[1]-(k);	\
				(c)[2]=(a)[2]-(k);
#define V4F_CONSTSUB(a, k, c)	\
				(c)[0]=(a)[0]-(k);	\
				(c)[1]=(a)[1]-(k);	\
				(c)[2]=(a)[2]-(k);	\
				(c)[3]=(a)[3]-(k);


#define V2F_ADDSCALE(a, b, c, d)	(d)[0]=(a)[0]+(b)[0]*(c);	\
					(d)[1]=(a)[1]+(b)[1]*(c);
#define V3F_ADDSCALE(a, b, c, d)	(d)[0]=(a)[0]+(b)[0]*(c);	\
					(d)[1]=(a)[1]+(b)[1]*(c);	\
					(d)[2]=(a)[2]+(b)[2]*(c);
#define V4F_ADDSCALE(a, b, c, d)	(d)[0]=(a)[0]+(b)[0]*(c);	\
					(d)[1]=(a)[1]+(b)[1]*(c);	\
					(d)[2]=(a)[2]+(b)[2]*(c);	\
					(d)[3]=(a)[3]+(b)[3]*(c);

#define V2F_SCALEADDSCALE(a, b, c, d, e)	\
					(e)[0]=(a)[0]*(b)+(c)[0]*(d);	\
					(e)[1]=(a)[1]*(b)+(c)[1]*(d);
#define V3F_SCALEADDSCALE(a, b, c, d, e)	\
					(e)[0]=(a)[0]*(b)+(c)[0]*(d);	\
					(e)[1]=(a)[1]*(b)+(c)[1]*(d);	\
					(e)[2]=(a)[2]*(b)+(c)[2]*(d);
#define V4F_SCALEADDSCALE(a, b, c, d, e)	\
					(e)[0]=(a)[0]*(b)+(c)[0]*(d);	\
					(e)[1]=(a)[1]*(b)+(c)[1]*(d);	\
					(e)[2]=(a)[2]*(b)+(c)[2]*(d);	\
					(e)[3]=(a)[3]*(B)+(c)[3]*(d);

#define V2F_ADDSCALEADDSCALE(a, b, c, d, e, f)	\
				(f)[0]=(a)[0]+(b)[0]*(c)+(d)[0]*(e);	\
				(f)[1]=(a)[1]+(b)[1]*(c)+(d)[1]*(e);
#define V3F_ADDSCALEADDSCALE(a, b, c, d, e, f)	\
				(f)[0]=(a)[0]+(b)[0]*(c)+(d)[0]*(e);	\
				(f)[1]=(a)[1]+(b)[1]*(c)+(d)[1]*(e);	\
				(f)[2]=(a)[2]+(b)[2]*(c)+(d)[2]*(e);
#define V4F_ADDSCALEADDSCALE(a, b, c, d, e, f)	\
				(f)[0]=(a)[0]+(b)[0]*(c)+(d)[0]*(e);	\
				(f)[1]=(a)[1]+(b)[1]*(c)+(d)[1]*(e);	\
				(f)[2]=(a)[2]+(b)[2]*(c)+(d)[2]*(e);	\
				(f)[3]=(a)[3]+(b)[3]*(c)+(d)[3]*(e);

/*
#define V3F_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])
#define V3D_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])
*/

#include <tkgl/vecmath_dot.h>

#define V2F_SUM(a)		((a)[0]+(a)[1])
#define V3F_SUM(a)		((a)[0]+(a)[1]+(a)[2])
#define V4F_SUM(a)		((a)[0]+(a)[1]+(a)[2]+(a)[3])

#define V2F_INITMINS(a)		V2F_CONST(a, PD3D_M_BIGVAL)
#define V2F_INITMAXS(a)		V2F_CONST(a, -PD3D_M_BIGVAL)
#define V3F_INITMINS(a)		V3F_CONST(a, PD3D_M_BIGVAL)
#define V3F_INITMAXS(a)		V3F_CONST(a, -PD3D_M_BIGVAL)
#define V4F_INITMINS(a)		V4F_CONST(a, PD3D_M_BIGVAL)
#define V4F_INITMAXS(a)		V4F_CONST(a, -PD3D_M_BIGVAL)

#define V2F_INITBOX(m, n)	V2F_INITMINS(m) V2F_INITMAXS(n)
#define V3F_INITBOX(m, n)	V3F_INITMINS(m) V3F_INITMAXS(n)
#define V4F_INITBOX(m, n)	V4F_INITMINS(m) V4F_INITMAXS(n)

#define V2F_MIN(a, b, c)	(c)[0]=MIN((a)[0], (b)[0]); \
				(c)[1]=MIN((a)[1], (b)[1]);
#define V3F_MIN(a, b, c)	(c)[0]=MIN((a)[0], (b)[0]); \
				(c)[1]=MIN((a)[1], (b)[1]); \
				(c)[2]=MIN((a)[2], (b)[2]);
#define V4F_MIN(a, b, c)	(c)[0]=MIN((a)[0], (b)[0]); \
				(c)[1]=MIN((a)[1], (b)[1]); \
				(c)[2]=MIN((a)[2], (b)[2]); \
				(c)[3]=MIN((a)[3], (b)[3]);
#define V2F_MAX(a, b, c)	(c)[0]=MAX((a)[0], (b)[0]); \
				(c)[1]=MAX((a)[1], (b)[1]);
#define V3F_MAX(a, b, c)	(c)[0]=MAX((a)[0], (b)[0]); \
				(c)[1]=MAX((a)[1], (b)[1]); \
				(c)[2]=MAX((a)[2], (b)[2]);
#define V4F_MAX(a, b, c)	(c)[0]=MAX((a)[0], (b)[0]); \
				(c)[1]=MAX((a)[1], (b)[1]); \
				(c)[2]=MAX((a)[2], (b)[2]); \
				(c)[3]=MAX((a)[3], (b)[3]);

#define V2F_SET(v, x, y)	(v)[0]=(x); (v)[1]=(y);
#define V3F_SET(v, x, y, z)	(v)[0]=(x); (v)[1]=(y); (v)[2]=(z);
#define V4F_SET(v, x, y, z, w)	(v)[0]=(x); (v)[1]=(y);	\
				(v)[2]=(z); (v)[3]=(w);


#ifndef __BSCC

static INLINE float frsqrt(float x)
{
	float xh, y;
	s32 i;
 
	xh=x*0.5F; y=x;
	i=*(s32 *)(&y);
	i=0x5F375A86-(i>>1);
	y=*(float *)(&i);
	y=y*(1.5F-(xh*y*y)); 
	return(y);
}

#define V2F_NORMALIZE_DEF
static INLINE float V2F_NORMALIZE(float *a, float *b)
	{float f, g; f=V2F_LEN(a); g=(f==0)?1:(1.0/f);
	V2F_SCALE(a, g, b); return(f);}

#define V3F_NORMALIZE_DEF
static INLINE float V3F_NORMALIZE(float *a, float *b)
	{float f, g; f=V3F_LEN(a); g=(f==0)?1:(1.0/f);
	V3F_SCALE(a, g, b); return(f);}

#define V4F_NORMALIZE_DEF
static INLINE float V4F_NORMALIZE(float *a, float *b)
	{float f, g; f=V4F_LEN(a); g=(f==0)?1:(1.0/f);
	V4F_SCALE(a, g, b); return(f);}

static INLINE double V2D_NORMALIZE(double *a, double *b)
	{double f, g; f=V2F_LEN(a); g=(f==0)?1:(1.0/f);
	V2F_SCALE(a, g, b); return(f);}
static INLINE double V3D_NORMALIZE(double *a, double *b)
	{double f, g; f=V3F_LEN(a); g=(f==0)?1:(1.0/f);
	V3F_SCALE(a, g, b); return(f);}
static INLINE double V4D_NORMALIZE(double *a, double *b)
	{double f, g; f=V4F_LEN(a); g=(f==0)?1:(1.0/f);
	V4F_SCALE(a, g, b); return(f);}

static INLINE float V3F_FRNORMALIZE(float *a, float *b)
{
	float f;
	f=frsqrt(V3F_DOT(a, a));
	V3F_SCALE(a, f, b);
	return(f);
}


VECMATH_API float *VecNF_AllocVec(int n);

static INLINE float *VEC2(float x, float y)
{
	float *v=VecNF_AllocVec(2);
	v[0]=x; v[1]=y;
	return(v);
}
static INLINE float *VEC3(float x, float y, float z)
{
	float *v=VecNF_AllocVec(3);
	v[0]=x; v[1]=y; v[2]=z;
	return(v);
}
static INLINE float *VEC4(float x, float y, float z, float w)
{
	float *v=VecNF_AllocVec(4);
	v[0]=x; v[1]=y; v[2]=z; v[3]=w;
	return(v);
}


static INLINE float *VA2(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(2);
	v[0]=v0[0]+v1[0]; v[1]=v0[1]+v1[1];
	return(v);
}
static INLINE float *VA3(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(3);
	v[0]=v0[0]+v1[0]; v[1]=v0[1]+v1[1]; v[2]=v0[2]+v1[2];
	return(v);
}
static INLINE float *VA4(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(4);
	v[0]=v0[0]+v1[0]; v[1]=v0[1]+v1[1];
	v[2]=v0[2]+v1[2]; v[3]=v0[3]+v1[3];
	return(v);
}

static INLINE float *VS2(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(2);
	v[0]=v0[0]-v1[0]; v[1]=v0[1]-v1[1];
	return(v);
}
static INLINE float *VS3(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(3);
	v[0]=v0[0]-v1[0]; v[1]=v0[1]-v1[1]; v[2]=v0[2]-v1[2];
	return(v);
}
static INLINE float *VS4(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(4);
	v[0]=v0[0]-v1[0]; v[1]=v0[1]-v1[1];
	v[2]=v0[2]-v1[2]; v[3]=v0[3]-v1[3];
	return(v);
}

static INLINE float *VM2(float *v0, float f)
{
	float *v=VecNF_AllocVec(2);
	v[0]=v0[0]*f; v[1]=v0[1]*f;
	return(v);
}
static INLINE float *VM3(float *v0, float f)
{
	float *v=VecNF_AllocVec(3);
	v[0]=v0[0]*f; v[1]=v0[1]*f; v[2]=v0[2]*f;
	return(v);
}
static INLINE float *VM4(float *v0, float f)
{
	float *v=VecNF_AllocVec(4);
	v[0]=v0[0]*f; v[1]=v0[1]*f;
	v[2]=v0[2]*f; v[3]=v0[3]*f;
	return(v);
}


static INLINE float VD2(float *v0, float *v1)
{
	return((v0[0]*v1[0])+(v0[1]*v1[1]));
}
static INLINE float VD3(float *v0, float *v1)
{
	return((v0[0]*v1[0])+(v0[1]*v1[1])+(v0[2]*v1[2]));
}
static INLINE float VD4(float *v0, float *v1)
{
	return((v0[0]*v1[0])+(v0[1]*v1[1])+(v0[2]*v1[2])+(v0[3]*v1[3]));
}


static INLINE float VX2(float *v0, float *v1)
{
	return((v0[0]*v1[1])-(v1[0]*v0[1]));
}

static INLINE float *VX3(float *v0, float *v1)
{
	float *v=VecNF_AllocVec(3);
	v[0]=(v0[1]*v1[2])-(v0[2]*v1[1]);
	v[1]=(v0[2]*v1[0])-(v0[0]*v1[2]);
	v[2]=(v0[0]*v1[1])-(v0[1]*v1[0]);
	return(v);
}

static INLINE float VL2(float *v)
{
	return(sqrt((v[0]*v[0])+(v[1]*v[1])));
}
static INLINE float VL3(float *v)
{
	return(sqrt((v[0]*v[0])+(v[1]*v[1])+(v[2]*v[2])));
}
static INLINE float VL4(float *v)
{
	return(sqrt((v[0]*v[0])+(v[1]*v[1])+(v[2]*v[2])+(v[3]*v[3])));
}

static INLINE void VC2(float *d, float *s)
{
	d[0]=s[0]; d[1]=s[1];
}
static INLINE void VC3(float *d, float *s)
{
	d[0]=s[0]; d[1]=s[1]; d[2]=s[2];
}
static INLINE void VC4(float *d, float *s)
{
	d[0]=s[0]; d[1]=s[1]; d[2]=s[2]; d[3]=s[3];
}

#endif

#ifdef __cplusplus
}
#endif

#endif

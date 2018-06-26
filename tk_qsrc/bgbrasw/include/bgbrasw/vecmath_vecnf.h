#ifndef VECNF_H
#define VECNF_H

// #include <bsde_conf.h>
#include <math.h>

void *VecNF_Ralloc(int sz);

#define gcralloc(sz)	VecNF_Ralloc(sz);

#if defined(MSVC)
#if defined(WIN64) || (_M_IX86_FP>=1)
#include <bgbrasw/vecmath_vecnf_msvc.h>
#else
#include <bgbrasw/vecmath_vecnf_raw.h>
#endif
#else
#include <bgbrasw/vecmath_vecnf_raw.h>
#endif

#undef gcralloc

static vec3 q_rot_v3f(quat a, vec3 b)
	{ return(qmul(qmul(a, b), qconj(a))); }
static vec4 q_rot_v4f(quat a, vec4 b)
	{ return(qmul(qmul(a, b), qconj(a))); }

static vec2 v2lerp(vec2 a, vec2 b, float t)
	{ return(v2add(v2scale(a, 1.0-t), v2scale(b, t))); }
static vec3 v3lerp(vec3 a, vec3 b, float t)
	{ return(v3add(v3scale(a, 1.0-t), v3scale(b, t))); }
static vec4 v4lerp(vec4 a, vec4 b, float t)
	{ return(v4add(v4scale(a, 1.0-t), v4scale(b, t))); }

static vec2 v2nlerp(vec2 a, vec2 b, float t)
	{ return(v2norm(v2add(v2scale(a, 1.0-t), v2scale(b, t)))); }
static vec3 v3nlerp(vec3 a, vec3 b, float t)
	{ return(v3norm(v3add(v3scale(a, 1.0-t), v3scale(b, t)))); }
static vec4 v4nlerp(vec4 a, vec4 b, float t)
	{ return(v4norm(v4add(v4scale(a, 1.0-t), v4scale(b, t)))); }

static quat qlerp(quat a, quat b, float t)
	{ return(qadd(qscale(a, 1.0-t), qscale(b, t))); }
static quat qnlerp(quat a, quat b, float t)
	{ return(qnorm(qadd(qscale(a, 1.0-t), qscale(b, t)))); }

static float qarg(quat q)
	{ return(acos(qreal(q)/qlen(q))); }

#define qsgn(q)		qnorm(q)

#define qln(q)		dtyQuatLn(q)
#define qexp(q)		dtyQuatExp(q)
#define qpow(p, q)	dtyQuatExpQuatQuat(p, q)
#define qpowsq(p, q)	dtyQuatExpRealQuat(p, q)
#define qpowqs(p, q)	dtyQuatExpQuatReal(p, q)

#define qslerp(p, q, t)	dtyQuatSlerp(p, q, t)

//#ifdef __cplusplus
#if 0

#if 0
vec2 operator=(vec2 &a, float *b)	{ a=vec2vf(b); return(a); }
vec3 operator=(vec3 &a, float *b)	{ a=vec3vf(b); return(a); }
vec4 operator=(vec4 &a, float *b)	{ a=vec4vf(b); return(a); }
quat operator=(quat &a, float *b)	{ a=quatvf(b); return(a); }

vec2 operator=(float *a, vec2 b)
	{ vfvec2(a, b); return(b); }
vec3 operator=(float *a, vec3 b)
	{ vfvec3(a, b); return(b); }
vec4 operator=(float *a, vec4 b)
	{ vfvec4(a, b); return(b); }
quat operator=(float *a, quat b)
	{ vfquat(a, b); return(b); }
#endif

//static vec2 operator+(vec2 a, vec2 b)	{ return(v2add(a, b)); }
//static vec2 operator-(vec2 a, vec2 b)	{ return(v2sub(a, b)); }
//static vec3 operator+(vec3 a, vec3 b)	{ return(v3add(a, b)); }
//static vec3 operator-(vec3 a, vec3 b)	{ return(v3sub(a, b)); }
static vec4 operator+(vec4 a, vec4 b)	{ return(v4add(a, b)); }
static vec4 operator-(vec4 a, vec4 b)	{ return(v4sub(a, b)); }
//static quat operator+(quat a, quat b)	{ return(qadd(a, b)); }
//static quat operator-(quat a, quat b)	{ return(qsub(a, b)); }

static float operator*(vec2 a, vec2 b)	{ return(v2dot(a, b)); }
static float operator*(vec3 a, vec3 b)	{ return(v3dot(a, b)); }
static float operator*(vec4 a, vec4 b)	{ return(v4dot(a, b)); }

static vec2 operator*(vec2 a, float b)	{ return(v2scale(a, b)); }
static vec3 operator*(vec3 a, float b)	{ return(v3scale(a, b)); }
static vec4 operator*(vec4 a, float b)	{ return(v4scale(a, b)); }
static quat operator*(quat a, float b)	{ return(qscale(a, b)); }

static vec2 operator*(float a, vec2 b)	{ return(v2scale(b, a)); }
static vec3 operator*(float a, vec3 b)	{ return(v3scale(b, a)); }
static vec4 operator*(float a, vec4 b)	{ return(v4scale(b, a)); }
static quat operator*(float a, quat b)	{ return(qscale(b, a)); }

static vec2 operator/(vec2 a, float b)	{ return(v2scale(a, 1.0/b)); }
static vec3 operator/(vec3 a, float b)	{ return(v3scale(a, 1.0/b)); }
static vec4 operator/(vec4 a, float b)	{ return(v4scale(a, 1.0/b)); }
static quat operator/(quat a, float b)	{ return(qscale(a, 1.0/b)); }

static vec2 operator/(float a, vec2 b)	{ return(v2scale(b, 1.0/a)); }
static vec3 operator/(float a, vec3 b)	{ return(v3scale(b, 1.0/a)); }
static vec4 operator/(float a, vec4 b)	{ return(v4scale(b, 1.0/a)); }
static quat operator/(float a, quat b)	{ return(qscale(b, 1.0/a)); }

static quat operator*(quat a, quat b)	{ return(qmul(a, b)); }
static quat operator/(quat a, quat b)	{ return(qdiv(a, b)); }
static quat operator|(quat a, quat b)	{ return(qrdiv(a, b)); }

static float operator%(vec2 a, vec2 b)	{ return(v2cross(a, b)); }
static vec3 operator%(vec3 a, vec3 b)	{ return(v3cross(a, b)); }
static vec4 operator%(vec4 a, vec4 b)	{ return(v4cross(a, b)); }
static quat operator%(quat a, quat b)	{ return(qcross(a, b)); }

static vec2 operator^(vec2 a, vec2 b)	{ return(v2mul(a, b)); }
static vec3 operator^(vec3 a, vec3 b)	{ return(v3mul(a, b)); }
static vec4 operator^(vec4 a, vec4 b)	{ return(v4mul(a, b)); }

static quat operator^(quat a, quat b)	{ return(qexp(a, b)); }
static quat operator^(quat a, float b)	{ return(qexpqs(a, b)); }
static quat operator^(float a, quat b)	{ return(qexpsq(a, b)); }

// static vec2 operator*(vec2 a, vec2 b)	{ return(v2mul(a, b)); }
// static vec2 operator/(vec2 a, vec2 b)	{ return(v2div(a, b)); }
// static vec3 operator*(vec3 a, vec3 b)	{ return(v3mul(a, b)); }
// static vec3 operator/(vec3 a, vec3 b)	{ return(v3div(a, b)); }
// static vec4 operator*(vec4 a, vec4 b)	{ return(v4mul(a, b)); }
// static vec4 operator/(vec4 a, vec4 b)	{ return(v4div(a, b)); }

#endif

#endif

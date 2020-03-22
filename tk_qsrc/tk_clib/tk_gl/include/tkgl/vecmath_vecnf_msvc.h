#include <xmmintrin.h>

#define vec2(x,y)	_mm_set_ps(0,0,y,x)
#define vec3(x,y,z)	_mm_set_ps(0,z,y,x)
#define vec4(x,y,z,w)	_mm_set_ps(w,z,y,x)
#define quat(w,x,y,z)	_mm_set_ps(w,z,y,x)

#define vec2vf(v)	_vec2vf(v)
#define vec3vf(v)	_vec3vf(v)
#define vec4vf(v)	_mm_loadu_ps(v)
#define quatvf(v)	_mm_loadu_ps(v)	//XYZW
#define quatvf2(v)	_quatvf2(v)	//WXYZ

#define pfvec2(v)	_vfvec(v)
#define pfvec3(v)	_vfvec(v)
#define pfvec4(v)	_vfvec(v)
#define pfquat(v)	_vfvec(v)	//XYZW
#define pfquat2(v)	_vf2vec(v)	//WXYZ

#define vfvec2(p,v)	_mm_storeu_ps(p,v)
#define vfvec3(p,v)	_mm_storeu_ps(p,v)
#define vfvec4(p,v)	_mm_storeu_ps(p,v)
#define vfquat(p,v)	_mm_storeu_ps(p,v)	//XYZW
#define vfquat2(p,v)	_vf4vec(p,v)		//WXYZ

#define v4shuf(a,sh)	_mm_shuffle_ps(a,a,sh)

#define v2add(a,b)	_mm_add_ps(a,b)
#define v2sub(a,b)	_mm_sub_ps(a,b)
#define v2mul(a,b)	_mm_mul_ps(a,b)
#define v2div(a,b)	_mm_div_ps(a,b)

#define v3add(a,b)	_mm_add_ps(a,b)
#define v3sub(a,b)	_mm_sub_ps(a,b)
#define v3mul(a,b)	_mm_mul_ps(a,b)
#define v3div(a,b)	_mm_div_ps(a,b)

#define v4add(a,b)	_mm_add_ps(a,b)
#define v4sub(a,b)	_mm_sub_ps(a,b)
#define v4mul(a,b)	_mm_mul_ps(a,b)
#define v4div(a,b)	_mm_div_ps(a,b)

#define qadd(a,b)	_mm_add_ps(a,b)
#define qsub(a,b)	_mm_sub_ps(a,b)

#define v2neg(a)	v2scale(a,-1)
#define v3neg(a)	v3scale(a,-1)
#define v4neg(a)	v4scale(a,-1)

#define qneg(a)		qscale(a,-1)
#define qconj(a)	v4mul(a,vec4(-1,-1,-1,1))

#define qdiv(a,b)	qmul(a,qrcp(b))
#define qrdiv(a,b)	qmul(qrcp(b),a)

#define v2dist2(a,b)	v2len2(v2sub(a,b))
#define v3dist2(a,b)	v3len2(v3sub(a,b))
#define v4dist2(a,b)	v4len2(v4sub(a,b))
#define qdist2(a,b)	qlen2(qsub(a,b))

#define v2dist(a,b)	v2len(v2sub(a,b))
#define v3dist(a,b)	v3len(v3sub(a,b))
#define v4dist(a,b)	v4len(v4sub(a,b))
#define qdist(a,b)	qlen(qsub(a,b))

#define v2eqp(a,b)	(v2dist2(a,b)==0)
#define v3eqp(a,b)	(v3dist2(a,b)==0)
#define v4eqp(a,b)	(v4dist2(a,b)==0)
#define qeqp(a,b)	(qdist2(a,b)==0)

#define v2neqp(a,b)	(v2dist2(a,b)!=0)
#define v3neqp(a,b)	(v3dist2(a,b)!=0)
#define v4neqp(a,b)	(v4dist2(a,b)!=0)
#define qneqp(a,b)	(qdist2(a,b)!=0)

#define v3xyz(a)	v4xyzw(a)
#define v3xzy(a)	v4xyzw(a)
#define v3yxz(a)	v4yxzw(a)
#define v3yzx(a)	v4yzxw(a)
#define v3zxy(a)	v4zxyw(a)
#define v3zyx(a)	v4zyxw(a)

#define v4xy(a)		v4xyzw(a)
#define v4xz(a)		v4xzyw(a)
#define v4xw(a)		v4xwyz(a)
#define v4yx(a)		v4yxwz(a)
#define v4yz(a)		v4yzxw(a)
#define v4yw(a)		v4ywxz(a)
#define v4zx(a)		v4zxyw(a)
#define v4zy(a)		v4zyxw(a)
#define v4zw(a)		v4zwyx(a)
#define v4wx(a)		v4wxyz(a)
#define v4wy(a)		v4wyxz(a)
#define v4wz(a)		v4wzyx(a)

#define v4xyz(a)	v4xyzw(a)
#define v4xyw(a)	v4xywz(a)
#define v4xzy(a)	v4xzyw(a)
#define v4xzw(a)	v4xzwy(a)
#define v4xwy(a)	v4xwyz(a)
#define v4xwz(a)	v4xwzy(a)
#define v4yxz(a)	v4yxzw(a)
#define v4yxw(a)	v4yxwz(a)
#define v4yzx(a)	v4yzxw(a)
#define v4yzw(a)	v4yzwx(a)
#define v4ywx(a)	v4ywxz(a)
#define v4ywz(a)	v4ywzx(a)
#define v4zxy(a)	v4zxyw(a)
#define v4zxw(a)	v4zxwy(a)
#define v4zyx(a)	v4zyxw(a)
#define v4zyw(a)	v4zywx(a)
#define v4zwx(a)	v4zwxy(a)
#define v4zwy(a)	v4zwyx(a)
#define v4wxy(a)	v4wxyz(a)
#define v4wxz(a)	v4wxzy(a)
#define v4wyx(a)	v4wyxz(a)
#define v4wyz(a)	v4wyzx(a)
#define v4wzx(a)	v4wzxy(a)
#define v4wzy(a)	v4wzyx(a)

#define v4xyzw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 1, 0))
#define v4xywz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 1, 0))
#define v4xzyw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 2, 0))
#define v4xzwy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0))
#define v4xwyz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 3, 0))
#define v4xwzy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 3, 0))
#define v4yxzw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1))
#define v4yxwz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1))
#define v4yzxw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1))
#define v4yzwx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 3, 2, 1))
#define v4ywxz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 0, 3, 1))
#define v4ywzx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 3, 1))
#define v4zxyw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))
#define v4zxwy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 0, 2))
#define v4zyxw_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 1, 2))
#define v4zywx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 3, 1, 2))
#define v4zwxy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 3, 2))
#define v4zwyx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 3, 2))
#define v4wxyz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 0, 3))
#define v4wxzy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 0, 3))
#define v4wyxz_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 0, 1, 3))
#define v4wyzx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 1, 3))
#define v4wzxy_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 2, 3))
#define v4wzyx_2(a)	_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 2, 3))

#define v2len(a)	sqrt(v2len2(a))
#define v3len(a)	sqrt(v3len2(a))
#define v4len(a)	sqrt(v4len2(a))
#define qlen(a)		sqrt(qlen2(a))


typedef __m128 vec2;
typedef __m128 vec4;
typedef vec4 vec3;
typedef vec4 quat;

static vec2 _vec2vf(float *x)
	{ return(_mm_set_ps(0, 0, x[1], x[0])); };
static vec3 _vec3vf(float *x)
	{ return(_mm_set_ps(0, x[2], x[1], x[0])); };
static vec4 _vec4vf(float *x)
	{ return(_mm_loadu_ps(x)); };
static quat _quatvf(float *x)
	{ return(_mm_set_ps(x[3], x[2], x[1], x[0])); };
static quat _quatvf2(float *x)
	{ return(_mm_set_ps(x[0], x[3], x[2], x[1])); };

#if 0
static float *_vfvec(vec4 v)
{
	float *p;
	p=gcralloc(4*sizeof(float));
	_mm_storeu_ps(p, v);
	return(p);
}

static float *_vf2vec(vec4 v)
{
	vec4 t;
	float *p;
	p=gcralloc(4*sizeof(float));
	t=_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3));
	_mm_storeu_ps(p, t);
	return(p);
}
#endif

static void _vf4vec(float *p, vec4 v)
{
	vec4 t;
	t=_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3));
	_mm_storeu_ps(p, t);
}


static float v2x(vec2 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[0]); };
static float v2y(vec2 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[1]); };

static float v3x(vec3 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[0]); };
static float v3y(vec3 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[1]); };
static float v3z(vec3 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[2]); };

static float v4x(vec4 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[0]); };
static float v4y(vec4 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[1]); };
static float v4z(vec4 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[2]); };
static float v4w(vec4 a)	{ float p[4]; _mm_storeu_ps(p, a); return(p[3]); };

static float qx(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[0]); };
static float qy(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[1]); };
static float qz(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[2]); };
static float qw(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[3]); };

static float qi(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[0]); };
static float qj(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[1]); };
static float qk(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[2]); };
static float qr(quat a)		{ float p[4]; _mm_storeu_ps(p, a); return(p[3]); };

static float qreal(quat a)
{
	float p[4];
	_mm_storeu_ps(p, a);
	return(p[3]);
}

static quat qvec(quat a)
{
	quat c, t;
	t=_mm_set_ps(0, 1, 1, 1);
	c=_mm_mul_ps(a, t);
	return(c);
}

static vec4 v4xyzw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 1, 0)); return(t); };
static vec4 v4xywz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 1, 0)); return(t); };
static vec4 v4xzyw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 2, 0)); return(t); };
static vec4 v4xzwy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0)); return(t); };
static vec4 v4xwyz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 3, 0)); return(t); };
static vec4 v4xwzy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 3, 0)); return(t); };
static vec4 v4yxzw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1)); return(t); };
static vec4 v4yxwz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1)); return(t); };
static vec4 v4yzxw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)); return(t); };
static vec4 v4yzwx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 3, 2, 1)); return(t); };
static vec4 v4ywxz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 0, 3, 1)); return(t); };
static vec4 v4ywzx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 3, 1)); return(t); };
static vec4 v4zxyw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)); return(t); };
static vec4 v4zxwy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 0, 2)); return(t); };
static vec4 v4zyxw(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 1, 2)); return(t); };
static vec4 v4zywx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 3, 1, 2)); return(t); };
static vec4 v4zwxy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 3, 2)); return(t); };
static vec4 v4zwyx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 3, 2)); return(t); };
static vec4 v4wxyz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 0, 3)); return(t); };
static vec4 v4wxzy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 0, 3)); return(t); };
static vec4 v4wyxz(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 0, 1, 3)); return(t); };
static vec4 v4wyzx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 1, 3)); return(t); };
static vec4 v4wzxy(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 2, 3)); return(t); };
static vec4 v4wzyx(vec4 a)	{ vec4 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 2, 3)); return(t); };

static vec2 v2xy(vec2 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 1, 0)); return(t); };
static vec2 v2yx(vec2 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1)); return(t); };

static vec2 v3xy(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 1, 0)); return(t); };
static vec2 v3xz(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 2, 0)); return(t); };
static vec2 v3yx(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1)); return(t); };
static vec2 v3yz(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 2, 1)); return(t); };
static vec2 v3zx(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 2)); return(t); };
static vec2 v3zy(vec3 a)	{ vec2 t; t=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 1, 2)); return(t); };


static vec2 v2scale(vec2 a, float f)
	{ return(v2mul(a, vec2(f, f))); }
static vec3 v3scale(vec3 a, float f)
	{ return(v3mul(a, vec3(f, f, f))); }
static vec4 v4scale(vec4 a, float f)
	{ return(v4mul(a, vec4(f, f, f, f))); }
static quat qscale(quat a, float f)
	{ return(v4mul(a, vec4(f, f, f, f))); }

static vec2 v2addscale(vec2 a, vec2 b, float f)
	{ return(v2add(a, v2mul(b, vec2(f, f)))); }
static vec3 v3addscale(vec3 a, vec3 b, float f)
	{ return(v3add(a, v3mul(b, vec3(f, f, f)))); }
static vec4 v4addscale(vec4 a, vec4 b, float f)
	{ return(v4add(a, v4mul(b, vec4(f, f, f, f)))); }
static quat qaddscale(quat a, quat b, float f)
	{ return(v4add(a, v4mul(b, vec4(f, f, f, f)))); }

static vec2 v2scaleaddscale(vec2 a, float f, vec2 b, float g)
	{ return(v2add(v2mul(a, vec2(f, f)), v2mul(b, vec2(g, g)))); }
static vec3 v3scaleaddscale(vec3 a, float f, vec3 b, float g)
	{ return(v3add(v3mul(a, vec3(f, f, f)), v3mul(b, vec3(g, g, g)))); }
static vec4 v4scaleaddscale(vec4 a, float f, vec4 b, float g)
	{ return(v4add(v4mul(a, vec4(f, f, f, f)), v4mul(b, vec4(g, g, g, g)))); }
static quat qscaleaddscale(quat a, float f, quat b, float g)
	{ return(v4add(v4mul(a, vec4(f, f, f, f)), v4mul(b, vec4(g, g, g, g)))); }

static vec2 v2scaleadd2(vec2 a, float f, vec2 b, float g)
	{ return(v2add(v2mul(a, vec2(f, f)), v2mul(b, vec2(g, g)))); }
static vec3 v3scaleadd2(vec3 a, float f, vec3 b, float g)
	{ return(v3add(v3mul(a, vec3(f, f, f)), v3mul(b, vec3(g, g, g)))); }
static vec4 v4scaleadd2(vec4 a, float f, vec4 b, float g)
	{ return(v4add(v4mul(a, vec4(f, f, f, f)), v4mul(b, vec4(g, g, g, g)))); }
static quat qscaleadd2(quat a, float f, quat b, float g)
	{ return(v4add(v4mul(a, vec4(f, f, f, f)), v4mul(b, vec4(g, g, g, g)))); }

static vec2 v2scaleadd3(vec2 a, float f, vec2 b, float g, vec2 c, float h)
{
	return(v2add(
		v2mul(a, vec2(f, f)),
		v2add(	v2mul(b, vec2(g, g)),
			v2mul(c, vec2(h, h)))
		));
}
static vec3 v3scaleadd3(vec3 a, float f, vec3 b, float g, vec3 c, float h)
{
	return(v3add(
		v3mul(a, vec3(f, f, f)),
		v3add(	v3mul(b, vec3(g, g, g)),
			v3mul(c, vec3(h, h, h)))
		));
}
static vec4 v4scaleadd3(vec4 a, float f, vec4 b, float g, vec4 c, float h)
{
	return(v4add(
		v4mul(a, vec4(f, f, f, f)),
		v4add(	v4mul(b, vec4(g, g, g, g)),
			v4mul(c, vec4(h, h, h, h)))
		));
}

static quat qscaleadd3(quat a, float f, quat b, float g, quat c, float h)
{
	return(v4add(
		v4mul(a, vec4(f, f, f, f)),
		v4add(	v4mul(b, vec4(g, g, g, g)),
			v4mul(c, vec4(h, h, h, h)))
		));
}

static vec2 v2scaleadd4(vec2 a, float f, vec2 b, float g, vec2 c, float h, vec2 d, float i)
{
	return(v2add(
		v2add(	v2mul(a, vec2(f, f)),
			v2mul(b, vec2(g, g))),
		v2add(	v2mul(c, vec2(h, h)),
			v2mul(d, vec2(i, i)))
		));
}

static vec3 v3scaleadd4(vec3 a, float f, vec3 b, float g, vec3 c, float h, vec3 d, float i)
{
	return(v3add(
		v3add(	v3mul(a, vec3(f, f, f)),
			v3mul(b, vec3(g, g, g))),
		v3add(	v3mul(c, vec3(h, h, h)),
			v3mul(d, vec3(i, i, i)))
		));
}

static vec4 v4scaleadd4(vec4 a, float f, vec4 b, float g, vec4 c, float h, vec4 d, float i)
{
	return(v4add(
		v4add(	v4mul(a, vec4(f, f, f, f)),
			v4mul(b, vec4(g, g, g, g))),
		v4add(	v4mul(c, vec4(h, h, h, h)),
			v4mul(d, vec4(i, i, i, i)))
		));
}

static quat qscaleadd4(quat a, float f, quat b, float g, quat c, float h, quat d, float i)
{
	return(v4add(
		v4add(	v4mul(a, vec4(f, f, f, f)),
			v4mul(b, vec4(g, g, g, g))),
		v4add(	v4mul(c, vec4(h, h, h, h)),
			v4mul(d, vec4(i, i, i, i)))
		));
}


static float v2cross(vec2 a, vec2 b)
	{ return(v2x(a)*v2y(b)-v2y(a)*v2x(b)); }

static vec3 v3cross(vec3 a, vec3 b)
{
	return(v4sub(
		v4mul(v4yzxw_2(a), v4zxyw_2(b)),
		v4mul(v4zxyw_2(a), v4yzxw_2(b))
		));
}

static vec4 v4cross(vec4 a, vec4 b)
{
	return(v4sub(
		v4mul(v4yzxw_2(a), v4zxyw_2(b)),
		v4mul(v4zxyw_2(a), v4yzxw_2(b))
		));
}

static quat qcross(quat a, quat b)
{
	return(v4sub(
		v4mul(v4yzxw_2(a), v4zxyw_2(b)),
		v4mul(v4zxyw_2(a), v4yzxw_2(b))
		));
}

static quat qmul(quat a, quat b)
{
	quat a0, a1, a2, a3;
	quat b0, b1, b2, b3;
	quat c0, c1, c2, c3;
	quat d0, d1, d2, d3;
	quat c;

	a0=_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3));
	b0=_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 1, 0));
	c0=_mm_mul_ps(a0, b0);

	a1=_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 1, 0));
	b1=_mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 3, 3, 3));
	c1=_mm_mul_ps(a1, b1);

	a2=_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 2, 1));
	b2=_mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 0, 2));
	c2=_mm_mul_ps(a2, b2);

	a3=_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 0, 2));
	b3=_mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 0, 2, 1));
	c3=_mm_mul_ps(a3, b3);

	d0=_mm_sub_ps(c0, c3);
	d1=_mm_add_ps(c1, c2);
	d2=_mm_set_ps(-1, 1, 1, 1);
	d3=_mm_mul_ps(d1, d2);

	c=_mm_add_ps(d0, d3);

//	c.x=(a.w*b.x)+(a.x*b.w)+(a.y*b.z)-(a.z*b.y);
//	c.y=(a.w*b.y)+(a.y*b.w)+(a.z*b.x)-(a.x*b.z);
//	c.z=(a.w*b.z)+(a.z*b.w)+(a.x*b.y)-(a.y*b.x);
//	c.w=(a.w*b.w)-(a.x*b.x)-(a.y*b.y)-(a.z*b.z);
	return(c);
}


static float v2dot(vec2 a, vec2 b)
{
	float p[4];
	_mm_storeu_ps(p, _mm_mul_ps(a, b));
	return(p[0]+p[1]);
}

static float v3dot(vec3 a, vec3 b)
{
	float p[4];
	_mm_storeu_ps(p, _mm_mul_ps(a, b));
	return(p[0]+p[1]+p[2]);
}

static float v4dot(vec4 a, vec4 b)
{
	float p[4];
	_mm_storeu_ps(p, _mm_mul_ps(a, b));
	return(p[0]+p[1]+p[2]+p[3]);
}

static float qdot(quat a, quat b)
{
	float p[4];
	_mm_storeu_ps(p, _mm_mul_ps(a, b));
	return(p[0]+p[1]+p[2]+p[3]);
}

static float v2len2(vec2 a)	{ return(v2dot(a, a)); }
static float v3len2(vec3 a)	{ return(v3dot(a, a)); }
static float v4len2(vec4 a)	{ return(v4dot(a, a)); }
static float qlen2(quat a)	{ return(qdot(a, a)); }

static vec2 v2norm(vec2 a)
{	vec2 c; float l;
	l=v2len(a); if(l!=0)l=1.0/l;
	c=v2scale(a, l);
	return(c);
}

static vec3 v3norm(vec3 a)
{	vec3 c; float l;
	l=v3len(a); if(l!=0)l=1.0/l;
	c=v3scale(a, l);
	return(c); }
static vec4 v4norm(vec4 a)
{	vec4 c; float l;
	l=v4len(a); if(l!=0)l=1.0/l;
	c=v4scale(a, l);
	return(c); }
static quat qnorm(quat a)
{	quat c; float l;
	l=qlen(a); if(l!=0)l=1.0/l;
	c=qscale(a, l);
	return(c); }


static quat qrcp(quat a)
{
	quat c; float f;

//	f=(a.w*a.w)+(a.x*a.x)+(a.y*a.y)+(a.z*a.z);
//	c.x=-a.x/f; c.y=-a.y/f; c.z=-a.z/f; c.w=a.w/f;

	f=qdot(a, a); if(f!=0)f=1.0/f;
	c=qscale(qconj(a), f);

	return(c);
}

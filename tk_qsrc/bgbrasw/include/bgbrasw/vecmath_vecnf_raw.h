#define vec2(x,y)	_vec2i(x,y)
#define vec3(x,y,z)	_vec3i(x,y,z)
#define vec4(x,y,z,w)	_vec4i(x,y,z,w)
#define quat(w,x,y,z)	_quati(w,x,y,z)

#define vec2vf(v)	_vec2vf(v)
#define vec3vf(v)	_vec3vf(v)
#define vec4vf(v)	_vec4vf(v)
#define quatvf(v)	_quatvf(v)	//XYZW
#define quatvf2(v)	_quatvf2(v)	//WXYZ

#define pfvec2(v)	_vfvec2(v)
#define pfvec3(v)	_vfvec4(v)
#define pfvec4(v)	_vfvec4(v)
#define pfquat(v)	_vfvec4(v)	//XYZW
#define pfquat2(v)	_vf2vec4(v)	//WXYZ

#define vfvec2(p,v)	_vfavec2(p,v)
#define vfvec3(p,v)	_vfavec3(p,v)
#define vfvec4(p,v)	_vfavec4(p,v)
#define vfquat(p,v)	_vfavec4(p,v)		//XYZW
#define vfquat2(p,v)	_vfa2vec4(p,v)		//WXYZ

#define v4shuf(a,sh)	_vf4_shuf(a,sh)

typedef struct {float x,y;} vec2;
typedef struct {float x,y,z,w;} vec4;
typedef vec4 vec3;
typedef vec4 quat;

static vec2 _vec2i(float x, float y)
	{ vec2 t; t.x=x; t.y=y; return(t); };
static vec3 _vec3i(float x, float y, float z)
	{ vec3 t; t.x=x; t.y=y; t.z=z; return(t); };
static vec4 _vec4i(float x, float y, float z, float w)
	{ vec4 t; t.x=x; t.y=y; t.z=z; t.w=w; return(t); };
static quat _quati(float w, float x, float y, float z)
	{ quat t; t.x=x; t.y=y; t.z=z; t.w=w; return(t); };

static vec2 _vec2vf(float *x)
	{ return(_vec2i(x[0], x[1])); };
static vec3 _vec3vf(float *x)
	{ return(_vec3i(x[0], x[1], x[2])); };
static vec4 _vec4vf(float *x)
	{ return(_vec4i(x[0], x[1], x[2], x[3])); };
static quat _quatvf(float *x)
	{ return(_vec4i(x[3], x[0], x[1], x[2])); };
static quat _quatvf2(float *x)
	{ return(_quati(x[0], x[1], x[2], x[3])); };

static float *_vfvec2(vec2 v)
{
	float *p;
	p=(float *)gcralloc(4*sizeof(float));
	p[0]=v.x; p[1]=v.y; p[2]=0; p[3]=0;
	return(p);
}

static float *_vfvec4(vec4 v)
{
	float *p;
	p=(float *)gcralloc(4*sizeof(float));
	p[0]=v.x; p[1]=v.y; p[2]=v.z; p[3]=v.w;
	return(p);
}

static float *_vf2vec4(vec4 v)
{
	float *p;
	p=(float *)gcralloc(4*sizeof(float));
	p[0]=v.w; p[1]=v.x; p[2]=v.y; p[3]=v.z;
	return(p);
}

static void _vfavec2(float *p, vec2 v)
	{ p[0]=v.x; p[1]=v.y; }
static void _vfavec3(float *p, vec4 v)
	{ p[0]=v.x; p[1]=v.y; p[2]=v.z; }
static void _vfavec4(float *p, vec4 v)
	{ p[0]=v.x; p[1]=v.y; p[2]=v.z; p[3]=v.w; }
static void _vfa2vec4(float *p, vec4 v)
	{ p[0]=v.w; p[1]=v.x; p[2]=v.y; p[3]=v.z; }

static float _vf4_getidx(vec4 v, int i)
{
	float f;
	switch(i)
	{
	case 0: f=v.x; break; case 1: f=v.y; break;
	case 2: f=v.z; break; case 3: f=v.w; break;
	default: f=0; break;
	}
	return(f);
}

static vec4 _vf4_shuf(vec4 v, int sh)
{
	vec4 b;
	b.x=_vf4_getidx(v, sh&3);
	b.y=_vf4_getidx(v, (sh>>2)&3);
	b.z=_vf4_getidx(v, (sh>>4)&3);
	b.w=_vf4_getidx(v, (sh>>6)&3);
	return(b);
}


static float v2x(vec2 a)	{ return(a.x); };
static float v2y(vec2 a)	{ return(a.y); };

static float v3x(vec3 a)	{ return(a.x); };
static float v3y(vec3 a)	{ return(a.y); };
static float v3z(vec3 a)	{ return(a.z); };

static float v4x(vec4 a)	{ return(a.x); };
static float v4y(vec4 a)	{ return(a.y); };
static float v4z(vec4 a)	{ return(a.z); };
static float v4w(vec4 a)	{ return(a.w); };

static vec2 v2xy(vec2 a)	{ vec2 t; t.x=a.x; t.y=a.y; return(t); };
static vec2 v2yx(vec2 a)	{ vec2 t; t.x=a.y; t.y=a.x; return(t); };

static vec2 v3xy(vec3 a)	{ vec2 t; t.x=a.x; t.y=a.y; return(t); };
static vec2 v3xz(vec3 a)	{ vec2 t; t.x=a.x; t.y=a.z; return(t); };
static vec2 v3yx(vec3 a)	{ vec2 t; t.x=a.y; t.y=a.x; return(t); };
static vec2 v3yz(vec3 a)	{ vec2 t; t.x=a.y; t.y=a.z; return(t); };
static vec2 v3zx(vec3 a)	{ vec2 t; t.x=a.z; t.y=a.x; return(t); };
static vec2 v3zy(vec3 a)	{ vec2 t; t.x=a.z; t.y=a.y; return(t); };

static vec3 v3xyz(vec3 a) { vec3 t; t.x=a.x; t.y=a.y; t.z=a.z; return(t); };
static vec3 v3xzy(vec3 a) { vec3 t; t.x=a.x; t.y=a.z; t.z=a.y; return(t); };
static vec3 v3yxz(vec3 a) { vec3 t; t.x=a.y; t.y=a.x; t.z=a.z; return(t); };
static vec3 v3yzx(vec3 a) { vec3 t; t.x=a.y; t.y=a.z; t.z=a.x; return(t); };
static vec3 v3zxy(vec3 a) { vec3 t; t.x=a.z; t.y=a.x; t.z=a.y; return(t); };
static vec3 v3zyx(vec3 a) { vec3 t; t.x=a.z; t.y=a.y; t.z=a.x; return(t); };

static vec2 v4xy(vec4 a)	{ vec2 t; t.x=a.x; t.y=a.y; return(t); };
static vec2 v4xz(vec4 a)	{ vec2 t; t.x=a.x; t.y=a.z; return(t); };
static vec2 v4xw(vec4 a)	{ vec2 t; t.x=a.x; t.y=a.w; return(t); };
static vec2 v4yx(vec4 a)	{ vec2 t; t.x=a.y; t.y=a.x; return(t); };
static vec2 v4yz(vec4 a)	{ vec2 t; t.x=a.y; t.y=a.z; return(t); };
static vec2 v4yw(vec4 a)	{ vec2 t; t.x=a.y; t.y=a.w; return(t); };
static vec2 v4zx(vec4 a)	{ vec2 t; t.x=a.z; t.y=a.x; return(t); };
static vec2 v4zy(vec4 a)	{ vec2 t; t.x=a.z; t.y=a.y; return(t); };
static vec2 v4zw(vec4 a)	{ vec2 t; t.x=a.z; t.y=a.w; return(t); };
static vec2 v4wx(vec4 a)	{ vec2 t; t.x=a.w; t.y=a.x; return(t); };
static vec2 v4wy(vec4 a)	{ vec2 t; t.x=a.w; t.y=a.y; return(t); };
static vec2 v4wz(vec4 a)	{ vec2 t; t.x=a.w; t.y=a.z; return(t); };

static vec3 v4xyz(vec4 a) { vec3 t; t.x=a.x; t.y=a.y; t.z=a.z; return(t); };
static vec3 v4xyw(vec4 a) { vec3 t; t.x=a.x; t.y=a.y; t.z=a.w; return(t); };
static vec3 v4xzy(vec4 a) { vec3 t; t.x=a.x; t.y=a.z; t.z=a.y; return(t); };
static vec3 v4xzw(vec4 a) { vec3 t; t.x=a.x; t.y=a.z; t.z=a.w; return(t); };
static vec3 v4xwy(vec4 a) { vec3 t; t.x=a.x; t.y=a.w; t.z=a.y; return(t); };
static vec3 v4xwz(vec4 a) { vec3 t; t.x=a.x; t.y=a.w; t.z=a.z; return(t); };
static vec3 v4yxz(vec4 a) { vec3 t; t.x=a.y; t.y=a.x; t.z=a.z; return(t); };
static vec3 v4yxw(vec4 a) { vec3 t; t.x=a.y; t.y=a.x; t.z=a.w; return(t); };
static vec3 v4yzx(vec4 a) { vec3 t; t.x=a.y; t.y=a.z; t.z=a.x; return(t); };
static vec3 v4yzw(vec4 a) { vec3 t; t.x=a.y; t.y=a.z; t.z=a.w; return(t); };
static vec3 v4ywx(vec4 a) { vec3 t; t.x=a.y; t.y=a.w; t.z=a.x; return(t); };
static vec3 v4ywz(vec4 a) { vec3 t; t.x=a.y; t.y=a.w; t.z=a.z; return(t); };
static vec3 v4zxy(vec4 a) { vec3 t; t.x=a.z; t.y=a.x; t.z=a.y; return(t); };
static vec3 v4zxw(vec4 a) { vec3 t; t.x=a.z; t.y=a.x; t.z=a.w; return(t); };
static vec3 v4zyx(vec4 a) { vec3 t; t.x=a.z; t.y=a.y; t.z=a.x; return(t); };
static vec3 v4zyw(vec4 a) { vec3 t; t.x=a.z; t.y=a.y; t.z=a.w; return(t); };
static vec3 v4zwx(vec4 a) { vec3 t; t.x=a.z; t.y=a.w; t.z=a.x; return(t); };
static vec3 v4zwy(vec4 a) { vec3 t; t.x=a.z; t.y=a.w; t.z=a.y; return(t); };
static vec3 v4wxy(vec4 a) { vec3 t; t.x=a.w; t.y=a.x; t.z=a.y; return(t); };
static vec3 v4wxz(vec4 a) { vec3 t; t.x=a.w; t.y=a.x; t.z=a.z; return(t); };
static vec3 v4wyx(vec4 a) { vec3 t; t.x=a.w; t.y=a.y; t.z=a.x; return(t); };
static vec3 v4wyz(vec4 a) { vec3 t; t.x=a.w; t.y=a.y; t.z=a.z; return(t); };
static vec3 v4wzx(vec4 a) { vec3 t; t.x=a.w; t.y=a.z; t.z=a.x; return(t); };
static vec3 v4wzy(vec4 a) { vec3 t; t.x=a.w; t.y=a.z; t.z=a.y; return(t); };

static vec4 v4xyzw(vec4 a) { vec4 t; t.x=a.x; t.y=a.y; t.z=a.z; t.w=a.w; return(t); };
static vec4 v4xywz(vec4 a) { vec4 t; t.x=a.x; t.y=a.y; t.z=a.w; t.w=a.z; return(t); };
static vec4 v4xzyw(vec4 a) { vec4 t; t.x=a.x; t.y=a.z; t.z=a.y; t.w=a.w; return(t); };
static vec4 v4xzwy(vec4 a) { vec4 t; t.x=a.x; t.y=a.z; t.z=a.w; t.w=a.y; return(t); };
static vec4 v4xwyz(vec4 a) { vec4 t; t.x=a.x; t.y=a.w; t.z=a.y; t.w=a.z; return(t); };
static vec4 v4xwzy(vec4 a) { vec4 t; t.x=a.x; t.y=a.w; t.z=a.z; t.w=a.y; return(t); };
static vec4 v4yxzw(vec4 a) { vec4 t; t.x=a.y; t.y=a.x; t.z=a.z; t.w=a.w; return(t); };
static vec4 v4yxwz(vec4 a) { vec4 t; t.x=a.y; t.y=a.x; t.z=a.w; t.w=a.z; return(t); };
static vec4 v4yzxw(vec4 a) { vec4 t; t.x=a.y; t.y=a.z; t.z=a.x; t.w=a.w; return(t); };
static vec4 v4yzwx(vec4 a) { vec4 t; t.x=a.y; t.y=a.z; t.z=a.w; t.w=a.x; return(t); };
static vec4 v4ywxz(vec4 a) { vec4 t; t.x=a.y; t.y=a.w; t.z=a.x; t.w=a.z; return(t); };
static vec4 v4ywzx(vec4 a) { vec4 t; t.x=a.y; t.y=a.w; t.z=a.z; t.w=a.x; return(t); };
static vec4 v4zxyw(vec4 a) { vec4 t; t.x=a.z; t.y=a.x; t.z=a.y; t.w=a.w; return(t); };
static vec4 v4zxwy(vec4 a) { vec4 t; t.x=a.z; t.y=a.x; t.z=a.w; t.w=a.y; return(t); };
static vec4 v4zyxw(vec4 a) { vec4 t; t.x=a.z; t.y=a.y; t.z=a.x; t.w=a.w; return(t); };
static vec4 v4zywx(vec4 a) { vec4 t; t.x=a.z; t.y=a.y; t.z=a.w; t.w=a.x; return(t); };
static vec4 v4zwxy(vec4 a) { vec4 t; t.x=a.z; t.y=a.w; t.z=a.x; t.w=a.y; return(t); };
static vec4 v4zwyx(vec4 a) { vec4 t; t.x=a.z; t.y=a.w; t.z=a.y; t.w=a.x; return(t); };
static vec4 v4wxyz(vec4 a) { vec4 t; t.x=a.w; t.y=a.x; t.z=a.y; t.w=a.z; return(t); };
static vec4 v4wxzy(vec4 a) { vec4 t; t.x=a.w; t.y=a.x; t.z=a.z; t.w=a.y; return(t); };
static vec4 v4wyxz(vec4 a) { vec4 t; t.x=a.w; t.y=a.y; t.z=a.x; t.w=a.z; return(t); };
static vec4 v4wyzx(vec4 a) { vec4 t; t.x=a.w; t.y=a.y; t.z=a.z; t.w=a.x; return(t); };
static vec4 v4wzxy(vec4 a) { vec4 t; t.x=a.w; t.y=a.z; t.z=a.x; t.w=a.y; return(t); };
static vec4 v4wzyx(vec4 a) { vec4 t; t.x=a.w; t.y=a.z; t.z=a.y; t.w=a.x; return(t); };

static float qx(vec4 a)	{ return(a.x); };
static float qy(vec4 a)	{ return(a.y); };
static float qz(vec4 a)	{ return(a.z); };
static float qw(vec4 a)	{ return(a.w); };

static float qi(vec4 a)	{ return(a.x); };
static float qj(vec4 a)	{ return(a.y); };
static float qk(vec4 a)	{ return(a.z); };
static float qr(vec4 a)	{ return(a.w); };

static float qreal(vec4 a)	{ return(a.w); };

static quat qvec(vec4 a)	{ a.w=0; return(a); };


static float v2dot(vec2 a, vec2 b)
	{ return(a.x*b.x+a.y*b.y); }
static float v3dot(vec3 a, vec3 b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z); }
static float v4dot(vec4 a, vec4 b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w); }
static float qdot(quat a, quat b)
	{ return(a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w); }

static float v2cross(vec2 a, vec2 b)
	{ return(a.x*b.y-a.y*b.x); }

static vec3 v3cross(vec3 a, vec3 b)
{
	vec3 c;
	c.x=(a.y*b.z)-(a.z*b.y);
	c.y=(a.z*b.x)-(a.x*b.z);
	c.z=(a.x*b.y)-(a.y*b.x);
	return(c);
}

static vec4 v4cross(vec4 a, vec4 b)
{
	vec4 c;
	c.x=(a.y*b.z)-(a.z*b.y);
	c.y=(a.z*b.x)-(a.x*b.z);
	c.z=(a.x*b.y)-(a.y*b.x);
	c.w=0;
	return(c);
}

static vec4 qcross(quat a, quat b)
	{ return(v4cross(a, b)); }

static float v2len(vec2 a)
	{ return(sqrt(a.x*a.x+a.y*a.y)); }
static float v3len(vec3 a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)); }
static float v4len(vec4 a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w)); }
static float qlen(quat a)
	{ return(sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w)); }

static vec2 v2norm(vec2 a)
{	vec2 c; float l;
	l=v2len(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l;
	return(c); }
static vec3 v3norm(vec3 a)
{	vec3 c; float l;
	l=v3len(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l;
	return(c); }
static vec4 v4norm(vec4 a)
{	vec4 c; float l;
	l=v4len(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l; c.w=a.w*l;
	return(c); }
static quat qnorm(quat a)
{	vec4 c; float l;
	l=v4len(a); if(l!=0)l=1.0/l;
	c.x=a.x*l; c.y=a.y*l; c.z=a.z*l; c.w=a.w*l;
	return(c); }

static vec2 v2neg(vec2 a)
	{ vec2 c; c.x=-a.x; c.y=-a.y; return(c); }
static vec3 v3neg(vec3 a)
	{ vec3 c; c.x=-a.x; c.y=-a.y; c.z=-a.z; return(c); }
static vec4 v4neg(vec4 a)
	{ vec4 c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=-a.w; return(c); }

static quat qneg(quat a)
	{ quat c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=-a.w; return(c); }
static quat qconj(quat a)
	{ quat c; c.x=-a.x; c.y=-a.y; c.z=-a.z; c.w=a.w; return(c); }

static quat qrcp(quat a)
{
	quat c; float f;
	f=(a.w*a.w)+(a.x*a.x)+(a.y*a.y)+(a.z*a.z);
	c.x=-a.x/f; c.y=-a.y/f; c.z=-a.z/f; c.w=a.w/f;
	return(c);
}


static vec2 v2add(vec2 a, vec2 b)
	{ vec2 c; c.x=a.x+b.x; c.y=a.y+b.y; return(c); }
static vec2 v2sub(vec2 a, vec2 b)
	{ vec2 c; c.x=a.x-b.x; c.y=a.y-b.y; return(c); }
static vec2 v2mul(vec2 a, vec2 b)
	{ vec2 c; c.x=a.x*b.x; c.y=a.y*b.y; return(c); }
static vec2 v2div(vec2 a, vec2 b)
	{ vec2 c; c.x=a.x/b.x; c.y=a.y/b.y; return(c); }

static vec3 v3add(vec3 a, vec3 b)
	{ vec3 c; c.x=a.x+b.x; c.y=a.y+b.y; c.z=a.z+b.z; return(c); }
static vec3 v3sub(vec3 a, vec3 b)
	{ vec3 c; c.x=a.x-b.x; c.y=a.y-b.y; c.z=a.z-b.z; return(c); }
static vec3 v3mul(vec3 a, vec3 b)
	{ vec3 c; c.x=a.x*b.x; c.y=a.y*b.y; c.z=a.z*b.z; return(c); }
static vec3 v3div(vec3 a, vec3 b)
	{ vec3 c; c.x=a.x/b.x; c.y=a.y/b.y; c.z=a.z/b.z; return(c); }

static vec4 v4add(vec4 a, vec4 b)
	{vec4 c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;c.w=a.w+b.w;return(c);}
static vec4 v4sub(vec4 a, vec4 b)
	{vec4 c;c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;c.w=a.w-b.w;return(c);}
static vec4 v4mul(vec4 a, vec4 b)
	{vec4 c;c.x=a.x*b.x;c.y=a.y*b.y;c.z=a.z*b.z;c.w=a.w*b.w;return(c);}
static vec4 v4div(vec4 a, vec4 b)
	{vec4 c;c.x=a.x/b.x;c.y=a.y/b.y;c.z=a.z/b.z;c.w=a.w/b.w;return(c);}

static quat qadd(quat a, quat b)
	{vec4 c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;c.w=a.w+b.w;return(c);}
static quat qsub(quat a, quat b)
	{vec4 c;c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;c.w=a.w-b.w;return(c);}

static quat qmul(quat a, quat b)
{
	quat c;
	c.x=(a.w*b.x)+(a.x*b.w)+(a.y*b.z)-(a.z*b.y);
	c.y=(a.w*b.y)+(a.y*b.w)+(a.z*b.x)-(a.x*b.z);
	c.z=(a.w*b.z)+(a.z*b.w)+(a.x*b.y)-(a.y*b.x);
	c.w=(a.w*b.w)-(a.x*b.x)-(a.y*b.y)-(a.z*b.z);
	return(c);
}

static quat qdiv(quat a, quat b)
	{ return(qmul(a, qrcp(b))); }
static quat qrdiv(quat a, quat b)
	{ return(qmul(qrcp(b), a)); }


static float v2dist(vec2 a, vec2 b) { return(v2len(v2sub(a, b))); }
static float v3dist(vec3 a, vec3 b) { return(v3len(v3sub(a, b))); }
static float v4dist(vec4 a, vec4 b) { return(v4len(v4sub(a, b))); }
static float qdist(quat a, quat b) { return(qlen(qsub(a, b))); }

static vec2 v2scale(vec2 a, float f)
	{ vec2 c; c.x=a.x*f; c.y=a.y*f; return(c); }
static vec3 v3scale(vec3 a, float f)
	{ vec3 c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; return(c); }
static vec4 v4scale(vec4 a, float f)
	{ vec4 c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; c.w=a.w*f; return(c); }
static quat qscale(quat a, float f)
	{ quat c; c.x=a.x*f; c.y=a.y*f; c.z=a.z*f; c.w=a.w*f; return(c); }


static int v2eqp(vec2 a, vec2 b)
	{ return((a.x==b.x) && (a.y==b.y)); }
static int v3eqp(vec3 a, vec3 b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z)); }
static int v4eqp(vec4 a, vec4 b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }
static int qeqp(quat a, quat b)
	{ return((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }

static int v2neqp(vec2 a, vec2 b)
	{ return((a.x!=b.x) || (a.y!=b.y)); }
static int v3neqp(vec3 a, vec3 b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z)); }
static int v4neqp(vec4 a, vec4 b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w)); }
static int qneqp(quat a, quat b)
	{ return((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w)); }


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

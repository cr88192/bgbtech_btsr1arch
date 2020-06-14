#ifdef __BGBCC__
typedef __vec4f	tkgl_vec4f;

#define tkgl_mkvec4f(x, y, z, w)	((__vec4f){x,y,z,w})

#define tkgl_v4fadd(a, b)	((a)+(b))
#define tkgl_v4fsub(a, b)	((a)-(b))
#define tkgl_v4fmul(a, b)	((a)*(b))

#else

typedef struct tkgl_vec4f_s tkgl_vec4f;

struct tkgl_vec4f_s {
float x;
float y;
float z;
float w;
};

static tkgl_vec4f tkgl_mkvec4f(float x, float y, float z, float w)
{
	tkgl_vec4f c;
	c.x=x;	c.y=y;
	c.z=z;	c.w=w;
	return(c);
}

static tkgl_vec4f tkgl_v4fadd(tkgl_vec4f a, tkgl_vec4f b)
{
	tkgl_vec4f c;
	c.x=a.x+b.x;	c.y=a.y+b.y;
	c.z=a.z+b.z;	c.w=b.w+b.w;
	return(c);
}

static tkgl_vec4f tkgl_v4fsub(tkgl_vec4f a, tkgl_vec4f b)
{
	tkgl_vec4f c;
	c.x=a.x-b.x;	c.y=a.y-b.y;
	c.z=a.z-b.z;	c.w=b.w-b.w;
	return(c);
}

static tkgl_vec4f tkgl_v4fmul(tkgl_vec4f a, tkgl_vec4f b)
{
	tkgl_vec4f c;
	c.x=a.x*b.x;	c.y=a.y*b.y;
	c.z=a.z*b.z;	c.w=b.w*b.w;
	return(c);
}

#endif

typedef struct {
	tkgl_vec4f	row0;
	tkgl_vec4f	row1;
	tkgl_vec4f	row2;
	tkgl_vec4f	row3;
}tkgl_mat4;
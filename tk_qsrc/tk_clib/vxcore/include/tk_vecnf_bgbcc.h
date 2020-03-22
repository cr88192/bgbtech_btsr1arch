typedef	__vec2f	vec2f;
typedef	__vec3f	vec3f;
typedef	__vec4f	vec4f;
typedef	__quatf	quatf;

typedef struct mat4f_s mat4f;

struct mat4f_s {
vec4f v0;
vec4f v1;
vec4f v2;
vec4f v3;
};

#define		v2fVec2(a, b)			((__vec2f) { a, b })
#define		v3fVec3(a, b, c)		((__vec3f) { a, b, c })
#define		v4fVec4(a, b, c, d)		((__vec4f) { a, b, c, d })

#define		v2fAdd(a, b)	((a)+(b))
#define		v2fSub(a, b)	((a)-(b))
#define		v2fMul(a, b)	((a)*(b))
#define		v2fDiv(a, b)	((a)/(b))

#define		v3fAdd(a, b)	((a)+(b))
#define		v3fSub(a, b)	((a)-(b))
#define		v3fMul(a, b)	((a)*(b))
#define		v3fDiv(a, b)	((a)/(b))

#define		v4fAdd(a, b)	((a)+(b))
#define		v4fSub(a, b)	((a)-(b))
#define		v4fMul(a, b)	((a)*(b))
#define		v4fDiv(a, b)	((a)/(b))

#define		vqfAdd(a, b)	((a)+(b))
#define		vqfSub(a, b)	((a)-(b))
#define		vqfMul(a, b)	((a)*(b))
#define		vqfDiv(a, b)	((a)/(b))

#define		v2fDot(a, b)	(__vnf_v2f_dot(a, b))
#define		v2fCross(a, b)	(__vnf_v2f_cross(a, b))
#define		v3fDot(a, b)	(__vnf_v3f_dot(a, b))
#define		v3fCross(a, b)	(__vnf_v3f_cross(a, b))
#define		v4fDot(a, b)	(__vnf_v4f_dot(a, b))
#define		v4fCross(a, b)	(__vnf_v4f_cross(a, b))


float		__vnf_v2f_dot(__vec2f a, __vec2f b);
float		__vnf_v2f_cross(__vec2f a, __vec2f b);

double		__vnf_v2d_dot(__vec2d a, __vec2d b);
double		__vnf_v2d_cross(__vec2d a, __vec2d b);

__vec3f		__vnf_v3f_cross(__vec3f a, __vec3f b);
float		__vnf_v3f_dot(__vec3f a, __vec3f b);

__vec4f		__vnf_v4f_cross(__vec4f a, __vec4f b);
float		__vnf_v4f_dot(__vec4f a, __vec4f b);

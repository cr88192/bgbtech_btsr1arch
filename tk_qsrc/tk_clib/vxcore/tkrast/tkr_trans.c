void TKRA_UnpackMatrix16fv(tkra_mat4 mat, float *fv);

int TKRA_EmitProjectedTrianglePts(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2);
int TKRA_EmitProjectedQuadPts(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3);

// #ifdef __BJX2__
#ifdef BJX2_SIMD

__vec4f		__vnf_v4fa_add(__vec4f a, __vec4f b);
__vec4f		__vnf_v4fa_sub(__vec4f a, __vec4f b);
__vec4f		__vnf_v4fa_mul(__vec4f a, __vec4f b);

#define tkra_v4fadd(a, b)	(__vnf_v4fa_add(a, b))
#define tkra_v4fsub(a, b)	(__vnf_v4fa_sub(a, b))
#define tkra_v4fmul(a, b)	(__vnf_v4fa_mul(a, b))

// #define tkra_v4fadd(a, b)	((a)+(b))
// #define tkra_v4fsub(a, b)	((a)-(b))
// #define tkra_v4fmul(a, b)	((a)*(b))

#define tkra_v2fadd(a, b)	((a)+(b))
#define tkra_v2fsub(a, b)	((a)-(b))
#define tkra_v2fmul(a, b)	((a)*(b))

//#define tkra_v4f_x(a)	((a)[0])
//#define tkra_v4f_y(a)	((a)[1])
//#define tkra_v4f_z(a)	((a)[2])
//#define tkra_v4f_w(a)	((a)[3])

//#define tkra_v2f_x(a)	((a)[0])
//#define tkra_v2f_y(a)	((a)[1])

#define tkra_v4f_x(a)	(a.x)
#define tkra_v4f_y(a)	(a.y)
#define tkra_v4f_z(a)	(a.z)
#define tkra_v4f_w(a)	(a.w)

#define tkra_v2f_x(a)	(a.x)
#define tkra_v2f_y(a)	(a.y)

// #define	tkra_mkvec4f(x, y, z, w)	((__vec4f) { x, y, z, w })
// #define	tkra_mkvec2f(x, y)			((__vec2f) { x, y })

#define tkra_v4f_xy(a)	tkra_mkvec2f(a.x, a.y)

//#define	tkra_mkvec4fv(p)			((__vec4f) { \
//	((float *)(p))[0], ((float *)(p))[1], \
//	((float *)(p))[2], ((float *)(p))[3] })
//#define	tkra_mkvec2fv(p)			((__vec2f) { \
//	((float *)(p))[0], ((float *)(p))[1] })

#define	tkra_mkvec4fv(p)			((__vec4f) (*(__unaligned __m128 *)(p)) )
#define	tkra_mkvec2fv(p)			((__vec2f) (*(__unaligned __m64 *)(p)) )

#define	tkra_upvec4fv(p, v)			((*(__unaligned __m128 *)(p))=(v))
#define	tkra_upvec2fv(p, v)			((*(__unaligned __m64 *)(p))=(v))

__m128 __m128_shufd(__m128 a, int b);
#define tkra_v4f_xxxx(a)	__m128_shufd(a, 0x00)
#define tkra_v4f_yyyy(a)	__m128_shufd(a, 0x55)
#define tkra_v4f_zzzz(a)	__m128_shufd(a, 0xAA)
#define tkra_v4f_wwww(a)	__m128_shufd(a, 0xFF)

//#define tkra_v4fdot(a, b)	(	((a)[0])*((b)[0])+ \
//								((a)[1])*((b)[1])+ \
//								((a)[2])*((b)[2])+ \
//								((a)[3])*((b)[3])	)

float		__vnf_v4f_dot(__vec4f a, __vec4f b);
float		__vnf_v3f_dot(__vec4f a, __vec4f b);
float		__vnf_v2f_dot(__vec2f a, __vec2f b);

float		__vnf_v4fa_dot(__vec4f a, __vec4f b);
float		__vnf_v3fa_dot(__vec4f a, __vec4f b);
float		__vnf_v2fa_dot(__vec2f a, __vec2f b);

// #define tkra_v4fdot(a, b)	__vnf_v4f_dot(a, b)
// #define tkra_v3fdot(a, b)	__vnf_v3f_dot(a, b)
// #define tkra_v2fdot(a, b)	__vnf_v2f_dot(a, b)

#define tkra_v4fdot(a, b)	__vnf_v4fa_dot(a, b)
#define tkra_v3fdot(a, b)	__vnf_v3fa_dot(a, b)
#define tkra_v2fdot(a, b)	__vnf_v2fa_dot(a, b)

// #define tkra_v4fdot(a, b)	((a)^(b))
// #define tkra_v2fdot(a, b)	((a)^(b))

float		__vnf_v2f_cross(__vec2f a, __vec2f b);
#define tkra_v2fcross(a, b)	__vnf_v2f_cross(a, b)

#define tkra_v4f_midpoint(a, b)		\
	(tkra_v4fmul(tkra_v4fadd(a, b), tkra_mkvec4f(0.5,0.5,0.5,0.5)))
#define tkra_v2f_midpoint(a, b)		\
	(tkra_v2fmul(tkra_v2fadd(a, b), tkra_mkvec2f(0.5,0.5)))

#define tkra_v4f_scale(a, b)		\
	(tkra_v4fmul(a, tkra_mkvec4f(b,b,b,b)))


float tkra_v4f_dist(tkra_vec4f a, tkra_vec4f b)
{
	tkra_vec4f vc, vd;
	vc=a-b;
	return(tkra_v4fdot(vc, vc));
}

float tkra_v2f_dist(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f vc, vd;
	vc=a-b;
	vd=vc*vc;
	return(vd.x+vd.y);
}

float tkra_v4f_dist_xy(tkra_vec4f a, tkra_vec4f b)
{
	float c, dx, dy;
	dx=a.x-b.x;		dy=a.y-b.y;
	c=(dx*dx)+(dy*dy);
	return(c);
}

tkra_vec2f tkra_mkvec2sfv(void *ptr);
tkra_vec4f tkra_mkvec4sfv(void *ptr);
tkra_vec4f tkra_mkvec3sfv1(void *ptr);

#ifdef __BJX2__
__asm {
tkra_mkvec2sfv:
	MOVU.L	(R4), R5
	SHLD.Q	R5, -16, R6	|	FLDCH	R5, R16
							FLDCH	R6, R17
	FSTCF	R16, R20
	FSTCF	R17, R21
	MOVLD	R21, R20, R2
	RTS

tkra_mkvec4sfv:
	MOV.Q	(R4), R5
	SHLD.Q	R5, -16, R6	|	FLDCH	R5, R16
	SHLD.Q	R5, -32, R7	|	FLDCH	R6, R17
	SHLD.Q	R5, -48, R6	|	FLDCH	R7, R18
							FLDCH	R6, R19
	FSTCF	R16, R20
	FSTCF	R17, R21
	FSTCF	R18, R22
	FSTCF	R19, R23
	MOVLD	R21, R20, R2
	MOVLD	R23, R22, R3

	RTSU

tkra_mkvec3sfv1:
	MOV.Q	(R4), R5
	SHLD.Q	R5, -16, R6	|	FLDCH	R5, R16
	SHLD.Q	R5, -32, R7	|	FLDCH	R6, R17
							FLDCH	R7, R18
							FLDCH	0x3C00, R19
	FSTCF	R16, R20
	FSTCF	R17, R21
	FSTCF	R18, R22
	FSTCF	R19, R23
	MOVLD	R21, R20, R2
	MOVLD	R23, R22, R3

	RTSU
};
#else
tkra_vec2f tkra_mkvec2sfv(void *ptr)
{
	float fa[4];
	fa[0]=((short float *)ptr)[0];
	fa[1]=((short float *)ptr)[1];
	return(tkra_mkvec2fv(fa));
}

tkra_vec4f tkra_mkvec4sfv(void *ptr)
{
	float fa[4];
	fa[0]=((short float *)ptr)[0];
	fa[1]=((short float *)ptr)[1];
	fa[2]=((short float *)ptr)[2];
	fa[3]=((short float *)ptr)[3];
	return(tkra_mkvec4fv(fa));
}

tkra_vec4f tkra_mkvec3sfv1(void *ptr)
{
	float fa[4];
	fa[0]=((short float *)ptr)[0];
	fa[1]=((short float *)ptr)[1];
	fa[2]=((short float *)ptr)[2];
	fa[3]=1.0;
	return(tkra_mkvec4fv(fa));
}

#endif


#else
tkra_vec4f tkra_v4fadd(tkra_vec4f a, tkra_vec4f b)
{
	tkra_vec4f c;
	c.x=a.x+b.x;	c.y=a.y+b.y;
	c.z=a.z+b.z;	c.w=a.w+b.w;
	return(c);
}

tkra_vec4f tkra_v4fsub(tkra_vec4f a, tkra_vec4f b)
{
	tkra_vec4f c;
	c.x=a.x-b.x;	c.y=a.y-b.y;
	c.z=a.z-b.z;	c.w=a.w-b.w;
	return(c);
}

tkra_vec4f tkra_v4fmul(tkra_vec4f a, tkra_vec4f b)
{
	tkra_vec4f c;
	c.x=a.x*b.x;	c.y=a.y*b.y;
	c.z=a.z*b.z;	c.w=a.w*b.w;
	return(c);
}

float tkra_v4fdot(tkra_vec4f a, tkra_vec4f b)
{
	float c;
	c=	(a.x*b.x) + (a.y*b.y) +
		(a.z*b.z) + (a.w*b.w);
	return(c);
}

float tkra_v3fdot(tkra_vec4f a, tkra_vec4f b)
{
	float c;
	c=	(a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	return(c);
}

float tkra_v2fdot(tkra_vec2f a, tkra_vec2f b)
{
	float c;
	c=	(a.x*b.x) + (a.y*b.y);
	return(c);
}

float tkra_v4f_dist(tkra_vec4f a, tkra_vec4f b)
{
	float c, dx, dy, dz, dw;
	dx=a.x-b.x;		dy=a.y-b.y;
	dz=a.z-b.z;		dw=a.w-b.w;
	c=(dx*dx)+(dy*dy)+(dz*dz)+(dw*dw);
	return(c);
}

tkra_vec4f tkra_v4f_scale(tkra_vec4f a, double b)
{
	tkra_vec4f c;
	c.x=a.x*b;	c.y=a.y*b;
	c.z=a.z*b;	c.w=a.w*b;
	return(c);
}

float tkra_v2f_dist(tkra_vec2f a, tkra_vec2f b)
{
	float c, dx, dy;
	dx=a.x-b.x;		dy=a.y-b.y;
	c=(dx*dx)+(dy*dy);
	return(c);
}

float tkra_v4f_dist_xy(tkra_vec4f a, tkra_vec4f b)
{
	float c, dx, dy;
	dx=a.x-b.x;		dy=a.y-b.y;
	c=(dx*dx)+(dy*dy);
	return(c);
}

tkra_vec2f tkra_v2fadd(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f c;
	c.x=a.x+b.x;	c.y=a.y+b.y;
	return(c);
}

tkra_vec2f tkra_v2fsub(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f c;
	c.x=a.x-b.x;	c.y=a.y-b.y;
	return(c);
}

tkra_vec2f tkra_v2fmul(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f c;
	c.x=a.x*b.x;	c.y=a.y*b.y;
	return(c);
}

float tkra_v2fcross(tkra_vec2f a, tkra_vec2f b)
{
	float c;
	c=(a.x*b.y)-(a.y*b.x);
	return(c);
}

#define tkra_v4f_x(a)	(a.x)
#define tkra_v4f_y(a)	(a.y)
#define tkra_v4f_z(a)	(a.z)
#define tkra_v4f_w(a)	(a.w)

#define tkra_v2f_x(a)	(a.x)
#define tkra_v2f_y(a)	(a.y)

#define tkra_v4f_xxxx(a)	tkra_mkvec4f(a.x, a.x, a.x, a.x)
#define tkra_v4f_yyyy(a)	tkra_mkvec4f(a.y, a.y, a.y, a.y)
#define tkra_v4f_zzzz(a)	tkra_mkvec4f(a.z, a.z, a.z, a.z)
#define tkra_v4f_wwww(a)	tkra_mkvec4f(a.w, a.w, a.w, a.w)

#define tkra_v4f_xy(a)	tkra_mkvec2f(a.x, a.y)

#if 0
tkra_vec4f tkra_mkvec4f(float x, float y, float z, float w)
{
	tkra_vec4f c;
	c.x=x;	c.y=y;
	c.z=z;	c.w=w;
	return(c);
}

tkra_vec2f tkra_mkvec2f(float x, float y)
{
	tkra_vec2f c;
	c.x=x;	c.y=y;
	return(c);
}
#endif

#define tkra_v4f_xy(a)	tkra_mkvec2f(a.x, a.y)

#define	tkra_mkvec4fv(p)			(tkra_mkvec4f( \
	((float *)(p))[0], ((float *)(p))[1], \
	((float *)(p))[2], ((float *)(p))[3] ))
#define	tkra_mkvec2fv(p)			(tkra_mkvec2f( \
	((float *)(p))[0], ((float *)(p))[1] ))

#define	tkra_upvec4fv(p, v)				\
	((float *)(p))[0]=tkra_v4f_x(v);	\
	((float *)(p))[1]=tkra_v4f_y(v);	\
	((float *)(p))[2]=tkra_v4f_z(v);	\
	((float *)(p))[3]=tkra_v4f_w(v)
	
#define	tkra_upvec2fv(p, v)				\
	((float *)(p))[0]=tkra_v2f_x(v);	\
	((float *)(p))[1]=tkra_v2f_y(v)

tkra_vec4f tkra_v4f_midpoint(tkra_vec4f a, tkra_vec4f b)
{
	tkra_vec4f c;
	c.x=(a.x+b.x)*0.5;	c.y=(a.y+b.y)*0.5;
	c.z=(a.z+b.z)*0.5;	c.w=(a.w+b.w)*0.5;
	return(c);
}

tkra_vec2f tkra_v2f_midpoint(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f c;
	c.x=(a.x+b.x)*0.5;	c.y=(a.y+b.y)*0.5;
	return(c);
}

float tkra_half2float(u16 iv)
{
	u32 v;
	int ex, fr;

	ex=(iv>>10)&31;
	fr=(iv&1023)<<(23-10);
	if(ex==31)
		{ v=0x7F800000U|fr; }
	else if(ex==0)
		{ v=fr; }
	else
		{ v=((ex+(127-15))<<23)|fr; }
	v|=(iv&0x8000)<<16;
	return(*(float *)(&v));
}

tkra_vec2f tkra_mkvec2sfv(void *ptr)
{
	tkra_vec2f c;
	c.x=tkra_half2float(((u16 *)ptr)[0]);
	c.y=tkra_half2float(((u16 *)ptr)[1]);
	return(c);
}

tkra_vec4f tkra_mkvec4sfv(void *ptr)
{
	tkra_vec4f c;
	c.x=tkra_half2float(((u16 *)ptr)[0]);
	c.y=tkra_half2float(((u16 *)ptr)[1]);
	c.z=tkra_half2float(((u16 *)ptr)[2]);
	c.w=tkra_half2float(((u16 *)ptr)[3]);
	return(c);
}

tkra_vec4f tkra_mkvec3sfv1(void *ptr)
{
	tkra_vec4f c;
	c.x=tkra_half2float(((u16 *)ptr)[0]);
	c.y=tkra_half2float(((u16 *)ptr)[1]);
	c.z=tkra_half2float(((u16 *)ptr)[2]);
	c.w=1.0;
	return(c);
}

#if 0
u32 tkra_rgba_midpoint(u32 a, u32 b)
{
	u32 c;
//	c=((a&0xFEFEFEFE)>>1)+((b&0xFEFEFEFE)>>1)+((a&b)&0x01010101);
	c=((a&0xFEFEFEFE)>>1)+((b&0xFEFEFEFE)>>1);
	return(c);
}


u64 tkra_rgba_expand64(u32 a)
{
	u64 c;
	c=	((a&0xFF000000ULL)<<32)|((a&0xFF000000ULL)<<24)|
		((a&0x00FF0000ULL)<<24)|((a&0x00FF0000ULL)<<16)|
		((a&0x0000FF00ULL)<<16)|((a&0x0000FF00ULL)<< 8)|
		((a&0x000000FFULL)<< 8)|((a&0x000000FFULL)<< 0);
	return(c);
}
#endif

#endif

#ifdef __BJX2__
// #ifdef BJX2_SIMD
// #if 0
u32 tkra_rgba_midpoint(u32 a, u32 b);
u64 tkra_rgba_expand64(u32 a);
u64 tkra_rgba32upck64(u32 a);
u32 tkra_rgba32pck64(u64 a);
u32 tkra_norm_midpoint(u32 a, u32 b);

__asm {
tkra_rgba_midpoint:
	MOV		0xFEFEFEFE, R3
	AND		R4, R3, R4	| AND		R5, R3, R5
	SHLD	R4, -1, R4	| SHLD		R5, -1, R5
	ADD		R4, R5, R2
	RTS

tkra_rgba_expand64:
	MOV				0xFF70FF70FF70FF70, R6
	MOV				0x0070007000700070, R7
	RGB32UPCK64		R4, R2
	AND				R6, R2
	OR				R7, R2
	RTS

tkra_rgba32upck64:
	RGB32UPCK64		R4, R2
	RTS
tkra_rgba32pck64:
	RGB32PCK64		R4, R2
	RTS

tkra_norm_midpoint:
	MOV		0xFEFEFEFE, R3
	MOV		0x80808080, R2
	AND		R4, R3, R4	| AND		R5, R3, R5
	AND		R4, R2, R16	| AND		R5, R2, R17
	SHLD	R4, -1, R6	| SHLD		R5, -1, R7
	OR		R16, R6		| OR		R17, R7
	ADD		R6, R7, R2
	RTS
};
#endif

#ifdef __XG3__
u32 tkra_rgba_midpoint(u32 a, u32 b);
u64 tkra_rgba_expand64(u32 a);
u64 tkra_rgba32upck64(u32 a);
u32 tkra_rgba32pck64(u64 a);
u32 tkra_norm_midpoint(u32 a, u32 b);

__asm {
tkra_rgba_midpoint:
	MOV		0xFEFEFEFE, R13
	AND		R10, R13, R10
	AND		R11, R13, R11
	SHLD	R10, -1, R10
	SHLD	R11, -1, R11
	ADD		R10, R11, R10
	RTS

tkra_rgba_expand64:
	MOV				0xFF70FF70FF70FF70, R16
	MOV				0x0070007000700070, R17
	RGB32UPCK64		R10, R10
	AND				R16, R10
	OR				R17, R10
	RTS

tkra_rgba32upck64:
	RGB32UPCK64		R10, R10
	RTS
tkra_rgba32pck64:
	RGB32PCK64		R10, R10
	RTS

tkra_norm_midpoint:
	MOV		0xFEFEFEFE, R13
	MOV		0x80808080, R12
	AND		R10, R13, R4
	AND		R11, R13, R5
	AND		R10, R12, R16
	AND		R11, R12, R17
	SHLD	R10, -1, R14
	SHLD	R11, -1, R15
	OR		R16, R14
	OR		R17, R15
	ADD		R14, R15, R10
	RTS
};
#endif

#if !defined(__BJX2__) && !defined(__XG3__)
u32 tkra_rgba_midpoint(u32 a, u32 b)
{
	u32 c;
//	c=((a&0xFEFEFEFE)>>1)+((b&0xFEFEFEFE)>>1)+((a&b)&0x01010101);
	c=((a&0xFEFEFEFEU)>>1)+((b&0xFEFEFEFEU)>>1);
	return(c);
}


u64 tkra_rgba_expand64(u32 a)
{
	u64 c;
	c=	((a&0xFF000000ULL)<<32)|((a&0xFF000000ULL)<<24)|
		((a&0x00FF0000ULL)<<24)|((a&0x00FF0000ULL)<<16)|
		((a&0x0000FF00ULL)<<16)|((a&0x0000FF00ULL)<< 8)|
		((a&0x000000FFULL)<< 8)|((a&0x000000FFULL)<< 0);
	
	c &= 0xFF70FF70FF70FF70ULL;
	c |= 0x0070007000700070ULL;
	

//	c=	((a&0xFF000000ULL)<<32)|
//		((a&0x00FF0000ULL)<<24)|
//		((a&0x0000FF00ULL)<<16)|
//		((a&0x000000FFULL)<< 8);
	return(c);
}

u64 tkra_rgba32upck64(u32 a)
{
	u64 c;
	c=	((a&0xFF000000ULL)<<32)|((a&0xFF000000ULL)<<24)|
		((a&0x00FF0000ULL)<<24)|((a&0x00FF0000ULL)<<16)|
		((a&0x0000FF00ULL)<<16)|((a&0x0000FF00ULL)<< 8)|
		((a&0x000000FFULL)<< 8)|((a&0x000000FFULL)<< 0);
	return(c);
}

u32 tkra_rgba32pck64(u64 a)
{
	u32 c;
	c=	((a>>32)&0xFF000000ULL) |
		((a>>24)&0x00FF0000ULL) |
		((a>>16)&0x0000FF00ULL) |
		((a>> 8)&0x000000FFULL) ;
	return(c);
}

u32 tkra_norm_midpoint(u32 a, u32 b)
{
	u32 c;
//	c=((a&0xFEFEFEFE)>>1)+((b&0xFEFEFEFE)>>1)+((a&b)&0x01010101);
	c=	(((a&0xFEFEFEFEU)>>1)|(a&0x80808080U))
		+
		(((b&0xFEFEFEFEU)>>1)|(b&0x80808080U));
	return(c);
}
#endif

double __fpu_frcp_sf(double x);
double __fpu_frcp_s(double x);
double tkra_frcp_fast(double x)
{
//	return(1.0/x);
//	return(__fpu_frcp_sf(x));
	return(__fpu_frcp_s(x));
}


#ifdef __BJX2__

tkra_vec4f tkra_v4f_bboxmins3(tkra_vec4f a, tkra_vec4f b, tkra_vec4f c);
tkra_vec4f tkra_v4f_bboxmaxs3(tkra_vec4f a, tkra_vec4f b, tkra_vec4f c);

tkra_vec4f tkra_v4f_bboxmins4(
	tkra_vec4f a, tkra_vec4f b,
	tkra_vec4f c, tkra_vec4f d);
tkra_vec4f tkra_v4f_bboxmaxs4(
	tkra_vec4f a, tkra_vec4f b,
	tkra_vec4f c, tkra_vec4f d);

float tkra_frcpabs(float x);

__asm {
tkra_v4f_bboxmins3:
	FLDCF	R4, R16
	FLDCF	R6, R17
	FLDCF	R20, R18
	FCMPGT	R16, R17
	CSELT	R16, R17, R22
	FCMPGT	R22, R18
	CSELT	R22, R18, R22

	FLDCFH	R4, R16
	FLDCFH	R6, R17
	FLDCFH	R20, R18
	FCMPGT	R16, R17
	CSELT	R16, R17, R23
	FCMPGT	R23, R18
	CSELT	R23, R18, R23

//	FSTCF	R22, R2
//	FSTCFH	R23, R2

	FSTCF	R22, R2
	FSTCF	R23, R3
	MOVLD	R3, R2, R2

	FLDCF	R5, R16
	FLDCF	R7, R17
	FLDCF	R21, R18
	FCMPGT	R16, R17
	CSELT	R16, R17, R22
	FCMPGT	R22, R18
	CSELT	R22, R18, R22

	FLDCFH	R5, R16
	FLDCFH	R7, R17
	FLDCFH	R21, R18
	FCMPGT	R16, R17
	CSELT	R16, R17, R23
	FCMPGT	R23, R18
	CSELT	R23, R18, R23

//	FSTCF	R22, R3
//	FSTCFH	R23, R3

	FSTCF	R22, R3
	FSTCF	R23, R1
	MOVLD	R1, R3, R3

	RTSU

tkra_v4f_bboxmaxs3:
	FLDCF	R4, R16
	FLDCF	R6, R17
	FLDCF	R20, R18
	FCMPGT	R16, R17
	CSELT	R17, R16, R22
	FCMPGT	R22, R18
	CSELT	R18, R22, R22

	FLDCFH	R4, R16
	FLDCFH	R6, R17
	FLDCFH	R20, R18
	FCMPGT	R16, R17
	CSELT	R17, R16, R23
	FCMPGT	R23, R18
	CSELT	R18, R23, R23

//	FSTCF	R22, R2
//	FSTCFH	R23, R2

	FSTCF	R22, R2
	FSTCF	R23, R3
	MOVLD	R3, R2, R2

	FLDCF	R5, R16
	FLDCF	R7, R17
	FLDCF	R21, R18
	FCMPGT	R16, R17
	CSELT	R17, R16, R22
	FCMPGT	R22, R18
	CSELT	R18, R22, R22

	FLDCFH	R5, R16
	FLDCFH	R7, R17
	FLDCFH	R21, R18
	FCMPGT	R16, R17
	CSELT	R17, R16, R23
	FCMPGT	R23, R18
	CSELT	R18, R23, R23

//	FSTCF	R22, R3
//	FSTCFH	R23, R3

	FSTCF	R22, R3
	FSTCF	R23, R1
	MOVLD	R1, R3, R3

	RTSU


tkra_v4f_bboxmins4:
	FLDCF	R4, R16
	FLDCF	R6, R17
	FLDCF	R20, R18
	FLDCF	R22, R19
	FCMPGT	R16, R17
	CSELT	R16, R17, R2
	FCMPGT	R2, R18
	CSELT	R2, R18, R2
	FCMPGT	R2, R19
	CSELT	R2, R19, R2

	FLDCFH	R4, R16
	FLDCFH	R6, R17
	FLDCFH	R20, R18
	FLDCFH	R22, R19
	FCMPGT	R16, R17
	CSELT	R16, R17, R3
	FCMPGT	R3, R18
	CSELT	R3, R18, R3
	FCMPGT	R3, R19
	CSELT	R3, R19, R3

	FSTCF	R2, R2
	FSTCF	R3, R3
	MOVLD	R3, R2, R1

	FLDCF	R5, R16
	FLDCF	R7, R17
	FLDCF	R21, R18
	FLDCF	R23, R19
	FCMPGT	R16, R17
	CSELT	R16, R17, R2
	FCMPGT	R2, R18
	CSELT	R2, R18, R2
	FCMPGT	R2, R19
	CSELT	R2, R19, R2

	FLDCFH	R5, R16
	FLDCFH	R7, R17
	FLDCFH	R21, R18
	FLDCFH	R23, R19
	FCMPGT	R16, R17
	CSELT	R16, R17, R3
	FCMPGT	R3, R18
	CSELT	R3, R18, R3
	FCMPGT	R3, R19
	CSELT	R3, R19, R3

	FSTCF	R2, R2
	FSTCF	R3, R3
	MOVLD	R3, R2, R3
	MOV		R1, R2

	RTSU

tkra_v4f_bboxmaxs4:
	FLDCF	R4, R16
	FLDCF	R6, R17
	FLDCF	R20, R18
	FLDCF	R22, R19
	FCMPGT	R16, R17
	CSELT	R17, R16, R2
	FCMPGT	R2, R18
	CSELT	R18, R2, R2
	FCMPGT	R2, R19
	CSELT	R19, R2, R2

	FLDCFH	R4, R16
	FLDCFH	R6, R17
	FLDCFH	R20, R18
	FLDCFH	R22, R19
	FCMPGT	R16, R17
	CSELT	R17, R16, R3
	FCMPGT	R3, R18
	CSELT	R18, R3, R3
	FCMPGT	R3, R19
	CSELT	R19, R3, R3

//	FSTCF	R22, R2
//	FSTCFH	R23, R2

	FSTCF	R2, R2
	FSTCF	R3, R3
	MOVLD	R3, R2, R1

	FLDCF	R5, R16
	FLDCF	R7, R17
	FLDCF	R21, R18
	FLDCF	R23, R19
	FCMPGT	R16, R17
	CSELT	R17, R16, R2
	FCMPGT	R2, R18
	CSELT	R18, R2, R2
	FCMPGT	R2, R19
	CSELT	R19, R2, R2

	FLDCFH	R5, R16
	FLDCFH	R7, R17
	FLDCFH	R21, R18
	FLDCFH	R23, R19
	FCMPGT	R16, R17
	CSELT	R17, R16, R3
	FCMPGT	R3, R18
	CSELT	R18, R3, R3
	FCMPGT	R3, R19
	CSELT	R19, R3, R3

	FSTCF	R2, R2
	FSTCF	R3, R3
	MOVLD	R3, R2, R3
	MOV		R1, R2

	RTSU


tkra_frcpabs:
//	FLDCH	0x3A00, R5
	FLDCH	0x1A00, R5
	FABS	R4, R4
	FADD	R5, R4
	BRA __fpu_frcp_sf

}

#else

tkra_vec4f tkra_v4f_bboxmins3(tkra_vec4f a, tkra_vec4f b, tkra_vec4f c)
{
	float f0, f1, f2;
	float mx, my, mz, mw;

	f0=tkra_v4f_x(a);	f1=tkra_v4f_x(b);	f2=tkra_v4f_x(c);
	mx=f0;
	if(f1<mx)mx=f1;
	if(f2<mx)mx=f2;

	f0=tkra_v4f_y(a);	f1=tkra_v4f_y(b);	f2=tkra_v4f_y(c);
	my=f0;
	if(f1<my)my=f1;
	if(f2<my)my=f2;

	f0=tkra_v4f_z(a);	f1=tkra_v4f_z(b);	f2=tkra_v4f_z(c);
	mz=f0;
	if(f1<mz)mz=f1;
	if(f2<mz)mz=f2;

	f0=tkra_v4f_w(a);	f1=tkra_v4f_w(b);	f2=tkra_v4f_w(c);
	mw=f0;
	if(f1<mw)mw=f1;
	if(f2<mw)mw=f2;

	return(tkra_mkvec4f(mx, my, mz, mw));
}

tkra_vec4f tkra_v4f_bboxmaxs3(tkra_vec4f a, tkra_vec4f b, tkra_vec4f c)
{
	float f0, f1, f2;
	float mx, my, mz, mw;

	f0=tkra_v4f_x(a);	f1=tkra_v4f_x(b);	f2=tkra_v4f_x(c);
	mx=f0;
	if(f1>mx)mx=f1;
	if(f2>mx)mx=f2;

	f0=tkra_v4f_y(a);	f1=tkra_v4f_y(b);	f2=tkra_v4f_y(c);
	my=f0;
	if(f1>my)my=f1;
	if(f2>my)my=f2;

	f0=tkra_v4f_z(a);	f1=tkra_v4f_z(b);	f2=tkra_v4f_z(c);
	mz=f0;
	if(f1>mz)mz=f1;
	if(f2>mz)mz=f2;

	f0=tkra_v4f_w(a);	f1=tkra_v4f_w(b);	f2=tkra_v4f_w(c);
	mw=f0;
	if(f1>mw)mw=f1;
	if(f2>mw)mw=f2;

	return(tkra_mkvec4f(mx, my, mz, mw));
}

tkra_vec4f tkra_v4f_bboxmins4(
	tkra_vec4f a, tkra_vec4f b,
	tkra_vec4f c, tkra_vec4f d)
{
	float f0, f1, f2, f3;
	float mx, my, mz, mw;

	f0=tkra_v4f_x(a);	f1=tkra_v4f_x(b);
	f2=tkra_v4f_x(c);	f3=tkra_v4f_x(d);
	mx=f0;
	if(f1<mx)mx=f1;
	if(f2<mx)mx=f2;
	if(f3<mx)mx=f3;

	f0=tkra_v4f_y(a);	f1=tkra_v4f_y(b);
	f2=tkra_v4f_y(c);	f3=tkra_v4f_y(d);
	my=f0;
	if(f1<my)my=f1;
	if(f2<my)my=f2;
	if(f3<my)my=f3;

	f0=tkra_v4f_z(a);	f1=tkra_v4f_z(b);
	f2=tkra_v4f_z(c);	f3=tkra_v4f_z(d);
	mz=f0;
	if(f1<mz)mz=f1;
	if(f2<mz)mz=f2;
	if(f3<mz)mz=f3;

	f0=tkra_v4f_w(a);	f1=tkra_v4f_w(b);
	f2=tkra_v4f_w(c);	f3=tkra_v4f_w(d);
	mw=f0;
	if(f1<mw)mw=f1;
	if(f2<mw)mw=f2;
	if(f3<mw)mw=f3;

	return(tkra_mkvec4f(mx, my, mz, mw));
}

tkra_vec4f tkra_v4f_bboxmaxs4(
	tkra_vec4f a, tkra_vec4f b,
	tkra_vec4f c, tkra_vec4f d)
{
	float f0, f1, f2, f3;
	float mx, my, mz, mw;

	f0=tkra_v4f_x(a);	f1=tkra_v4f_x(b);
	f2=tkra_v4f_x(c);	f3=tkra_v4f_x(d);
	mx=f0;
	if(f1>mx)mx=f1;
	if(f2>mx)mx=f2;
	if(f3>mx)mx=f3;

	f0=tkra_v4f_y(a);	f1=tkra_v4f_y(b);
	f2=tkra_v4f_y(c);	f3=tkra_v4f_y(d);
	my=f0;
	if(f1>my)my=f1;
	if(f2>my)my=f2;
	if(f3>my)my=f3;

	f0=tkra_v4f_z(a);	f1=tkra_v4f_z(b);
	f2=tkra_v4f_z(c);	f3=tkra_v4f_z(d);
	mz=f0;
	if(f1>mz)mz=f1;
	if(f2>mz)mz=f2;
	if(f3>mz)mz=f3;

	f0=tkra_v4f_w(a);	f1=tkra_v4f_w(b);
	f2=tkra_v4f_w(c);	f3=tkra_v4f_w(d);
	mw=f0;
	if(f1>mw)mw=f1;
	if(f2>mw)mw=f2;
	if(f3>mw)mw=f3;

	return(tkra_mkvec4f(mx, my, mz, mw));
}

float tkra_frcpabs(float x)
{
//	return(1.0/(fabs(x)+0.25));
//	return(1.0/(fabs(x)+0.001));
	return(tkra_frcp_fast(fabs(x)+0.001));
}

#endif


#ifdef __BJX2__

tkra_vec4f TKRA_ProjectVertex(tkra_vec4f vec, tkra_mat4 mat);

__asm {
TKRA_ProjectVertex:
	MOV.X		(R6,  0), R22
	MOVLD		R4, R4, R20		|	MOVLD	R4, R4, R21
	MOV.X		(R6, 16), R18
	PMULX.FA	R20, R22, R2
	MOVHD		R4, R4, R20		|	MOVHD	R4, R4, R21
	MOV.X		(R6, 32), R22
	PMULX.FA	R20, R18, R16
	PADDX.FA	R2, R16, R2
	MOVLD		R5, R5, R20		|	MOVLD	R5, R5, R21
	MOV.X		(R6, 48), R18
	PMULX.FA	R20, R22, R16
	MOVHD		R5, R5, R20		|	MOVHD	R5, R5, R21
	PADDX.FA	R2, R16, R2
	PMULX.FA	R20, R18, R16
	PADDX.FA	R2, R16, R2
	RTS
	NOP
};

#else

tkra_vec4f TKRA_ProjectVertex(tkra_vec4f vec, tkra_mat4 mat)
{
	register tkra_vec4f v0x, v0y, v0z, v0w;
	float f;

	v0x=tkra_v4f_xxxx(vec);
	v0y=tkra_v4f_yyyy(vec);
	v0z=tkra_v4f_zzzz(vec);
	v0w=tkra_v4f_wwww(vec);
		
	v0x=tkra_v4fmul(v0x, mat.row0);
	v0y=tkra_v4fmul(v0y, mat.row1);
	v0z=tkra_v4fmul(v0z, mat.row2);
	v0w=tkra_v4fmul(v0w, mat.row3);

	v0x=tkra_v4fadd(v0x, v0y);
	v0y=tkra_v4fadd(v0z, v0w);
	v0z=tkra_v4fadd(v0x, v0y);

	return(v0z);
}

#endif

tkra_vec4f TKRA_ProjectVector(tkra_vec4f vec, tkra_mat4 mat)
{
	register tkra_vec4f v0x, v0y, v0z, v0w;
	float f;

	v0x=tkra_v4f_xxxx(vec);
	v0y=tkra_v4f_yyyy(vec);
	v0z=tkra_v4f_zzzz(vec);
//	v0w=tkra_v4f_wwww(vec);
		
	v0x=tkra_v4fmul(v0x, mat.row0);
	v0y=tkra_v4fmul(v0y, mat.row1);
	v0z=tkra_v4fmul(v0z, mat.row2);
//	v0w=tkra_v4fmul(v0w, mat.row3);

	v0x=tkra_v4fadd(v0x, v0y);
//	v0y=tkra_v4fadd(v0z, v0w);
	v0z=tkra_v4fadd(v0x, v0z);

	return(v0z);
}

#ifdef __BJX2__
// #if 0
tkra_mat4 TKRA_MatrixTranspose(tkra_mat4 mata);

__asm {
TKRA_MatrixTranspose:
	MOV.X (R4,  0), R16
	MOV.X (R4, 16), R18
	MOV.X (R4, 32), R20
	MOV.X (R4, 48), R22
	
	MOVLLD	R18, R16, R6
	MOVLLD	R22, R20, R7
	MOV.X	R6, (R2, 0)
	MOVHHD	R18, R16, R6
	MOVHHD	R22, R20, R7
	MOV.X	R6, (R2, 16)

	MOVLLD	R19, R17, R6
	MOVLLD	R23, R21, R7
	MOV.X	R6, (R2, 32)
	MOVHHD	R19, R17, R6
	MOVHHD	R23, R21, R7
	MOV.X	R6, (R2, 48)

	RTS
	NOP
};

#else
tkra_mat4 TKRA_MatrixTranspose(tkra_mat4 mata)
{
	tkra_vec4f v0, v1, v2, v3;
	tkra_mat4 matc;
	float x, y, z, w;

#if 1
	v0=mata.row0;	v1=mata.row1;
	v2=mata.row2;	v3=mata.row3;
	
	x=tkra_v4f_x(v0);	y=tkra_v4f_x(v1);
	z=tkra_v4f_x(v2);	w=tkra_v4f_x(v3);
	matc.row0=tkra_mkvec4f( x, y, z, w );
	x=tkra_v4f_y(v0);	y=tkra_v4f_y(v1);
	z=tkra_v4f_y(v2);	w=tkra_v4f_y(v3);
	matc.row1=tkra_mkvec4f( x, y, z, w );
	x=tkra_v4f_z(v0);	y=tkra_v4f_z(v1);
	z=tkra_v4f_z(v2);	w=tkra_v4f_z(v3);
	matc.row2=tkra_mkvec4f( x, y, z, w );
	x=tkra_v4f_w(v0);	y=tkra_v4f_w(v1);
	z=tkra_v4f_w(v2);	w=tkra_v4f_w(v3);
	matc.row3=tkra_mkvec4f( x, y, z, w );
	return(matc);
#endif
	
#if 0
	matc.row0=tkra_mkvec4f(
		tkra_v4f_x(mata.row0),	tkra_v4f_x(mata.row1),
		tkra_v4f_x(mata.row2),	tkra_v4f_x(mata.row3) );
	matc.row1=tkra_mkvec4f(
		tkra_v4f_y(mata.row0),	tkra_v4f_y(mata.row1),
		tkra_v4f_y(mata.row2),	tkra_v4f_y(mata.row3) );
	matc.row2=tkra_mkvec4f(
		tkra_v4f_z(mata.row0),	tkra_v4f_z(mata.row1),
		tkra_v4f_z(mata.row2),	tkra_v4f_z(mata.row3) );
	matc.row3=tkra_mkvec4f(
		tkra_v4f_w(mata.row0),	tkra_v4f_w(mata.row1),
		tkra_v4f_w(mata.row2),	tkra_v4f_w(mata.row3) );
	return(matc);
#endif
}
#endif


#ifdef __BJX2__
// #if 0
tkra_mat4 TKRA_MatrixMultiply(tkra_mat4 mata, tkra_mat4 matb);

__asm {
TKRA_MatrixMultiply:
	MOV.X	(R4,  0), R6
	MOV.X	(R5,  0), R16
	MOVLLD	R6, R6, R18	
	MOVLLD	R6, R6, R19
	PMULX.F	R16, R18, R20
	MOV.X	(R5, 16), R16
	MOVHHD	R6, R6, R18
	MOVHHD	R6, R6, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 32), R16
	MOVLLD	R7, R7, R18
	MOVLLD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 48), R16
	MOVHHD	R7, R7, R18
	MOVHHD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	R20, (R2, 0)

	MOV.X	(R4, 16), R6
	MOV.X	(R5,  0), R16
	MOVLLD	R6, R6, R18
	MOVLLD	R6, R6, R19
	PMULX.F	R16, R18, R20
	MOV.X	(R5, 16), R16
	MOVHHD	R6, R6, R18
	MOVHHD	R6, R6, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 32), R16
	MOVLLD	R7, R7, R18
	MOVLLD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 48), R16
	MOVHHD	R7, R7, R18
	MOVHHD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	R20, (R2, 16)

	MOV.X	(R4, 32), R6
	MOV.X	(R5,  0), R16
	MOVLLD	R6, R6, R18
	MOVLLD	R6, R6, R19
	PMULX.F	R16, R18, R20
	MOV.X	(R5, 16), R16
	MOVHHD	R6, R6, R18
	MOVHHD	R6, R6, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 32), R16
	MOVLLD	R7, R7, R18
	MOVLLD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 48), R16
	MOVHHD	R7, R7, R18
	MOVHHD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	R20, (R2, 32)

	MOV.X	(R4, 48), R6
	MOV.X	(R5,  0), R16
	MOVLLD	R6, R6, R18
	MOVLLD	R6, R6, R19
	PMULX.F	R16, R18, R20
	MOV.X	(R5, 16), R16
	MOVHHD	R6, R6, R18
	MOVHHD	R6, R6, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 32), R16
	MOVLLD	R7, R7, R18
	MOVLLD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	(R5, 48), R16
	MOVHHD	R7, R7, R18
	MOVHHD	R7, R7, R19
	PMULX.F	R16, R18, R22
	PADDX.F	R20, R22, R20
	MOV.X	R20, (R2, 48)

	RTS
	NOP
};

#else

tkra_mat4 TKRA_MatrixMultiply(tkra_mat4 mata, tkra_mat4 matb)
{
//	float xyz0[16];
	tkra_mat4 matbt;
	tkra_mat4 matc;
	tkra_vec4f v0a, v1a, v2a, v3a;
	tkra_vec4f v0b, v1b, v2b, v3b;
	float f0, f1, f2, f3;

//	v0a=mata.row0;		v1a=mata.row1;
//	v2a=mata.row2;		v3a=mata.row3;

#if 0
	printf("%p   %08X %08X %08X %08X\n", &mata,
		((u32 *)(&mata))[0], ((u32 *)(&mata))[1],
		((u32 *)(&mata))[2], ((u32 *)(&mata))[3]);
#endif

	matbt=TKRA_MatrixTranspose(matb);

//	__debugbreak();

#if 0
	TKRA_UnpackMatrix16fv(matbt, xyz0);
	printf("TKRA_MatrixMultiply: matbt\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);
#endif

#if 1
//	printf("%p   %08X %08X %08X %08X\n", &mata,
//		((u32 *)(&mata))[0], ((u32 *)(&mata))[1],
//		((u32 *)(&mata))[2], ((u32 *)(&mata))[3]);

	v0a=mata.row0;		v1a=mata.row1;
	v2a=mata.row2;		v3a=mata.row3;
	v0b=matbt.row0;		v1b=matbt.row1;
	v2b=matbt.row2;		v3b=matbt.row3;

//	__debugbreak();

//	printf("%p   %08X %08X %08X %08X\n", &mata,
//		((u32 *)(&mata))[0], ((u32 *)(&mata))[1],
//		((u32 *)(&mata))[2], ((u32 *)(&mata))[3]);

	f0=tkra_v4fdot(v0a, v0b);	f1=tkra_v4fdot(v0a, v1b);
	f2=tkra_v4fdot(v0a, v2b);	f3=tkra_v4fdot(v0a, v3b);

//	__debugbreak();

//	printf("%f %f %f %f\n", f0, f1, f2, f3);

	matc.row0=tkra_mkvec4f( f0, f1, f2, f3 );
//	__debugbreak();
	
	f0=tkra_v4fdot(v1a, v0b);	f1=tkra_v4fdot(v1a, v1b);
	f2=tkra_v4fdot(v1a, v2b);	f3=tkra_v4fdot(v1a, v3b);
	matc.row1=tkra_mkvec4f( f0, f1, f2, f3 );
	f0=tkra_v4fdot(v2a, v0b);	f1=tkra_v4fdot(v2a, v1b);
	f2=tkra_v4fdot(v2a, v2b);	f3=tkra_v4fdot(v2a, v3b);
	matc.row2=tkra_mkvec4f( f0, f1, f2, f3 );
	f0=tkra_v4fdot(v3a, v0b);	f1=tkra_v4fdot(v3a, v1b);
	f2=tkra_v4fdot(v3a, v2b);	f3=tkra_v4fdot(v3a, v3b);
	matc.row3=tkra_mkvec4f( f0, f1, f2, f3 );

	return(matc);
#endif

#if 0
	matc.row0=tkra_mkvec4f(
		tkra_v4fdot(mata.row0, matbt.row0),
		tkra_v4fdot(mata.row0, matbt.row1),
		tkra_v4fdot(mata.row0, matbt.row2),
		tkra_v4fdot(mata.row0, matbt.row3) );
	matc.row1=tkra_mkvec4f(
		tkra_v4fdot(mata.row1, matbt.row0),
		tkra_v4fdot(mata.row1, matbt.row1),
		tkra_v4fdot(mata.row1, matbt.row2),
		tkra_v4fdot(mata.row1, matbt.row3) );
	matc.row2=tkra_mkvec4f(
		tkra_v4fdot(mata.row2, matbt.row0),
		tkra_v4fdot(mata.row2, matbt.row1),
		tkra_v4fdot(mata.row2, matbt.row2),
		tkra_v4fdot(mata.row2, matbt.row3) );
	matc.row3=tkra_mkvec4f(
		tkra_v4fdot(mata.row3, matbt.row0),
		tkra_v4fdot(mata.row3, matbt.row1),
		tkra_v4fdot(mata.row3, matbt.row2),
		tkra_v4fdot(mata.row3, matbt.row3) );
	return(matc);
#endif
}

#endif

#ifdef __BJX2__
// #if 0
tkra_mat4 TKRA_MatrixIdentify(void);

__asm {
TKRA_MatrixIdentify:
	MOV		0x00000000, R6
	MOV		0x3F800000, R7

	MOVLD	R6, R7, R16
	MOVLD	R6, R6, R17
	MOV.X	R16, (R2, 0)

	MOVLD	R7, R6, R16
	MOVLD	R6, R6, R17
	MOV.X	R16, (R2, 16)

	MOVLD	R6, R6, R16
	MOVLD	R6, R7, R17
	MOV.X	R16, (R2, 32)

	MOVLD	R6, R6, R16
	MOVLD	R7, R6, R17
	MOV.X	R16, (R2, 48)

	RTS
	NOP
};

#else
tkra_mat4 TKRA_MatrixIdentify(void)
{
	tkra_mat4 matc;
	
	matc.row0=tkra_mkvec4f( 1, 0, 0, 0 );
	matc.row1=tkra_mkvec4f( 0, 1, 0, 0 );
	matc.row2=tkra_mkvec4f( 0, 0, 1, 0 );
	matc.row3=tkra_mkvec4f( 0, 0, 0, 1 );
//	__debugbreak();
	return(matc);
}
#endif

#ifdef __BJX2__

tkra_mat4 TKRA_Matrix16fv(float *fv);
tkra_mat4 TKRA_Matrix16Tfv(float *fv);

__asm {
TKRA_Matrix16fv:
	MOV.Q	(R4,  0), R16
	MOV.Q	(R4,  8), R17
	MOV.Q	(R4, 16), R18
	MOV.Q	(R4, 24), R19
	MOV.Q	(R4, 32), R20
	MOV.Q	(R4, 40), R21
	MOV.Q	(R4, 48), R22
	MOV.Q	(R4, 56), R23
	MOV.X	R16, (R2,  0)
	MOV.X	R18, (R2, 16)
	MOV.X	R20, (R2, 32)
	MOV.X	R22, (R2, 48)
	RTS

TKRA_Matrix16Tfv:
	MOV.Q	(R4,  0), R16
	MOV.Q	(R4,  8), R17
	MOV.Q	(R4, 16), R18
	MOV.Q	(R4, 24), R19
	MOV.Q	(R4, 32), R20
	MOV.Q	(R4, 40), R21
	MOV.Q	(R4, 48), R22
	MOV.Q	(R4, 56), R23
	MOVLD	R18, R16, R6 |	MOVLD	R22, R20, R7
	MOV.X	R6, (R2,  0)
	MOVHD	R18, R16, R6 |	MOVHD	R22, R20, R7
	MOV.X	R6, (R2, 16)
	MOVLD	R19, R17, R6 |	MOVLD	R23, R21, R7
	MOV.X	R6, (R2, 32)
	MOVHD	R19, R17, R6 |	MOVHD	R23, R21, R7
	MOV.X	R6, (R2, 48)
	RTS
};

#else
tkra_mat4 TKRA_Matrix16fv(float *fv)
{
	tkra_mat4 matc;
//	matc.row0=tkra_mkvec4f( fv[ 0], fv[ 1], fv[ 2], fv[ 3] );
//	matc.row1=tkra_mkvec4f( fv[ 4], fv[ 5], fv[ 6], fv[ 7] );
//	matc.row2=tkra_mkvec4f( fv[ 8], fv[ 9], fv[10], fv[11] );
//	matc.row3=tkra_mkvec4f( fv[12], fv[13], fv[14], fv[15] );
	matc.row0=tkra_mkvec4fv( fv+ 0 );
	matc.row1=tkra_mkvec4fv( fv+ 4 );
	matc.row2=tkra_mkvec4fv( fv+ 8 );
	matc.row3=tkra_mkvec4fv( fv+12 );
//	__debugbreak();
	return(matc);
}

tkra_mat4 TKRA_Matrix16Tfv(float *fv)
{
	tkra_mat4 matc;
	matc.row0=tkra_mkvec4f( fv[ 0], fv[ 4], fv[ 8], fv[12] );
	matc.row1=tkra_mkvec4f( fv[ 1], fv[ 5], fv[ 9], fv[13] );
	matc.row2=tkra_mkvec4f( fv[ 2], fv[ 6], fv[10], fv[14] );
	matc.row3=tkra_mkvec4f( fv[ 3], fv[ 7], fv[11], fv[15] );
	return(matc);
}
#endif

tkra_mat4 TKRA_Matrix16dv(double *fv)
{
	tkra_mat4 matc;
	matc.row0=tkra_mkvec4f( fv[ 0], fv[ 1], fv[ 2], fv[ 3] );
	matc.row1=tkra_mkvec4f( fv[ 4], fv[ 5], fv[ 6], fv[ 7] );
	matc.row2=tkra_mkvec4f( fv[ 8], fv[ 9], fv[10], fv[11] );
	matc.row3=tkra_mkvec4f( fv[12], fv[13], fv[14], fv[15] );
	return(matc);
}

void TKRA_UnpackMatrix16fv(tkra_mat4 mat, float *fv)
{
	tkra_vec4f row;
	
	row=mat.row0;
	tkra_upvec4fv(fv+0, row);
	row=mat.row1;
	tkra_upvec4fv(fv+4, row);
	row=mat.row2;
	tkra_upvec4fv(fv+8, row);
	row=mat.row3;
	tkra_upvec4fv(fv+12, row);
}

#define TKRA_CheckZCullQuad_Bias	512

int TKRA_CheckZCullTriangle(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2)
{
	tkra_zbufpixel *zbuf;
	int ds0, ds1, ds2, ds3, dt0, dt1, dt2, dt3, dst;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst;
	int clip_mx, clip_nx, clip_my, clip_ny;

	return(0);

	if(!(ctx->stateflag1&TKRA_STFL1_DEPTHTEST))
		return(0);
	if(	(ctx->zat_zfunc!=TKRA_ZAT_LT) &&
		(ctx->zat_zfunc!=TKRA_ZAT_LE))
			return(0);

	zbuf=ctx->screen_zbuf;

	clip_mx=ctx->clip_x0;		clip_nx=ctx->clip_x1;
	clip_my=ctx->clip_y0;		clip_ny=ctx->clip_y1;

	tx0=(pv0.x>>16);		ty0=(pv0.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);
	tx0=(pv1.x>>16);		ty0=(pv1.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);
	tx0=(pv2.x>>16);		ty0=(pv2.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);

	dxy=ctx->screen_xsize*ctx->screen_ysize;
	
	tx0=(pv0.x>>16);		ty0=(pv0.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	
	dy0=(s16)(pv0.z>>16)-TKRA_CheckZCullQuad_Bias;

	if(zbuf[dx0]>dy0)
		return(0);

	tx0=(pv1.x>>16);		ty0=(pv1.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx1=(ty0*ctx->screen_xsize)+tx0;
	dy1=(s16)(pv1.z>>16)-TKRA_CheckZCullQuad_Bias;

	tx0=(pv2.x>>16);		ty0=(pv2.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx2=(ty0*ctx->screen_xsize)+tx0;
	dy2=(s16)(pv2.z>>16)-TKRA_CheckZCullQuad_Bias;

	if((zbuf[dx1]>dy1) || (zbuf[dx2]>dy2))
		return(0);

	dx2=(pv0.x>>16)+(pv1.x>>16)+(pv2.x>>16)+(pv1.x>>16);
	dy2=(pv0.y>>16)+(pv1.y>>16)+(pv2.y>>16)+(pv1.y>>16);
	ds2=(pv0.z>>16)+(pv1.z>>16)+(pv2.z>>16)+(pv1.z>>16);

	dx2=dx2>>2;
	dy2=dx2>>2;
	ds2=ds2>>2;

//	dx2=__int_clamp(dx2, clip_mx, clip_nx);
//	dy2=__int_clamp(dy2, clip_my, clip_ny);

	dx0=(dy2*ctx->screen_xsize)+(dx2);
	dy0=((s16)ds2)-TKRA_CheckZCullQuad_Bias;

	if(zbuf[dx0]>dy0)
		return(0);

#if 1
	tx0=((pv0.x>>16)+dx2)>>1;	ty0=((pv0.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	dy0=(((s16)(pv0.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	tx0=((pv2.x>>16)+dx2)>>1;	ty0=((pv2.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx1=(ty0*ctx->screen_xsize)+tx0;
	dy1=(((s16)(pv2.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	if((zbuf[dx0]>dy0) || (zbuf[dx1]>dy1))
		return(0);
#endif

#if 1
	tx0=((pv1.x>>16)+dx2)>>1;	ty0=((pv1.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	dy0=(((s16)(pv1.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

//	tx0=((pv3.x>>16)+dx2)>>1;	ty0=((pv3.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
//	dx1=(ty0*ctx->screen_xsize)+tx0;
//	dy1=(((s16)(pv3.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

//	if((zbuf[dx0]>dy0) || (zbuf[dx1]>dy1))
	if(zbuf[dx0]>dy0)
		return(0);
#endif

	return(1);	
}

int TKRA_EmitProjectedTriangleI(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2)
{
	u64	v1_parm[TKRA_VX_NPARM];
	u64	v2_parm[TKRA_VX_NPARM];
	u64	v3_parm[TKRA_VX_NPARM];
	tkra_zbufpixel *zbuf;
	TKRA_TexImage *img;
	TKRA_SvContext *sctx;

	u64 tl0, tl1;
	int is0, is1, is2, it0, it1, it2, mip, mag;
	int ds0, ds1, ds2, dt0, dt1, dt2, dst;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst, flipst2;
	int isb0, isb1, isb2, itb0, itb1, itb2, mip2, mag2;
	int clip_mx, clip_nx, clip_my, clip_ny;

	sctx=ctx->svctx;

	is0=pv0.s;		is1=pv1.s;		is2=pv2.s;
	it0=pv0.t;		it1=pv1.t;		it2=pv2.t;
	isb0=pv0.s2;	isb1=pv1.s2;	isb2=pv2.s2;
	itb0=pv0.t2;	itb1=pv1.t2;	itb2=pv2.t2;

	if((tkra_nommio&3)!=2)
	{
		if(TKRA_CheckZCullTriangle(ctx, pv0, pv1, pv2))
			return(-1);
	}

	flipst=0;

	mmip=sctx->tex_nmip;
	if(1)
	{
		dx0=pv1.x-pv0.x;	dx1=pv2.x-pv1.x;
		dy0=pv1.y-pv0.y;	dy1=pv2.y-pv1.y;
		dx0=dx0^(dx0>>31);	dx1=dx1^(dx1>>31);
		dy0=dy0^(dy0>>31);	dy1=dy1^(dy1>>31);
		dxy=dx0+dx1+dy0+dy1;

		dx2=(dx0+dx1)>>16;
		dy2=(dy0+dy1)>>16;
		dx3=dxy>>16;
		
		if((dx2<2) || (dy2<2))
			return;
		if(dx3<2)
			{ return; }
		if(dx3<6)
		{
			TKRA_EmitProjectedTrianglePts(ctx, pv0, pv1, pv2);
			return;
		}
		
		ds0=is1-is0;		ds1=is2-is1;
		dt0=it1-it0;		dt1=it2-it1;
		ds0=ds0^(ds0>>31);	ds1=ds1^(ds1>>31);
		dt0=dt0^(dt0>>31);	dt1=dt1^(dt1>>31);
		dst=ds0+ds1+dt0+dt1;
		
		mip=0;
		mag=(dst<=dxy);

#if 1
		while((dst>dxy) && (mip<(mmip-1)))
			{ dst>>=1;	mip++; }
#endif

		if(sctx->tex_flag&TKRA_TRFL_FLIPST)
			flipst=1;

		if((mip>0) && (sctx->tex_flag&TKRA_TRFL_HASMIP))
		{
			is0>>=mip;	is1>>=mip;	is2>>=mip;
			it0>>=mip;	it1>>=mip;	it2>>=mip;
			mag=0;

			img=sctx->tex_cur;

			shx=img->tex_xshl-mip;
			shy=img->tex_yshl-mip;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			sctx->tex_img=img->tex_img+img->tex_mipofs[mip];
			sctx->tex_img_bcn=img->tex_img_bcn+img->tex_mipofs_bcn[mip];
			sctx->tex_xshl=shx;
			sctx->tex_yshl=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn)
			{
				tl0=(u64)(sctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				if(img->tex_flag&TKRA_TRFL_PIXFMT_UTX3)
					tx0|=2<<9;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}else
		{
			img=sctx->tex_cur;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			sctx->tex_img=img->tex_img;
			sctx->tex_img_bcn=img->tex_img_bcn;
			sctx->tex_xshl=shx;
			sctx->tex_yshl=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn)
			{
				tl0=(u64)(sctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				if(img->tex_flag&TKRA_TRFL_PIXFMT_UTX3)
					tx0|=2<<9;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}
		
//		if(mip>0)
		if(!mag)
		{
			sctx->DrawSpan=sctx->DrawSpan_Min;
			sctx->DrawSpanHZt=sctx->DrawSpanHZt_Min;
			sctx->DrawSpanZt=sctx->DrawSpanZt_Min;
		}
		else
		{
			sctx->DrawSpan=sctx->DrawSpan_Mag;
			sctx->DrawSpanHZt=sctx->DrawSpanHZt_Mag;
			sctx->DrawSpanZt=sctx->DrawSpanZt_Mag;
		}
	}else
	{
		img=sctx->tex_cur;
		sctx->tex_img=img->tex_img;
		sctx->tex_img_bcn=img->tex_img_bcn;
		sctx->DrawSpan=sctx->DrawSpan_Mag;
		sctx->DrawSpanHZt=sctx->DrawSpanHZt_Mag;
		sctx->DrawSpanZt=sctx->DrawSpanZt_Mag;
	}

#if 1
	mmip=sctx->tex_nmip2;
	if(sctx->tex_cur2)
	{
#if 1
		dx0=pv1.x-pv0.x;	dx1=pv2.x-pv1.x;
		dy0=pv1.y-pv0.y;	dy1=pv2.y-pv1.y;
		dx0=dx0^(dx0>>31);	dx1=dx1^(dx1>>31);
		dy0=dy0^(dy0>>31);	dy1=dy1^(dy1>>31);
		dxy=dx0+dx1+dy0+dy1;

		ds0=isb1-isb0;		ds1=isb2-isb1;
		dt0=itb1-itb0;		dt1=itb2-itb1;
		ds0=ds0^(ds0>>31);	ds1=ds1^(ds1>>31);
		dt0=dt0^(dt0>>31);	dt1=dt1^(dt1>>31);
		dst=ds0+ds1+dt0+dt1;
#endif
		
		mip2=0;
		mag2=dst<=dxy;

#if 1
		while((dst>dxy) && (mip2<(mmip-1)))
			{ dst>>=1;	mip2++; }
#endif

		if(sctx->tex_flag2&TKRA_TRFL_FLIPST)
			flipst2=1;

		if((mip2>0) && (sctx->tex_flag&TKRA_TRFL_HASMIP))
		{
			isb0>>=mip2;	isb1>>=mip2;	isb2>>=mip2;
			itb0>>=mip2;	itb1>>=mip2;	itb2>>=mip2;
			mag=0;

			img=sctx->tex_cur2;

			shx=img->tex_xshl-mip2;
			shy=img->tex_yshl-mip2;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			sctx->tex_img2=img->tex_img+img->tex_mipofs[mip2];
			sctx->tex_img_bcn2=img->tex_img_bcn+img->tex_mipofs_bcn[mip2];
			sctx->tex_xshl2=shx;
			sctx->tex_yshl2=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn2)
			{
				tl0=(u64)(sctx->tex_img_bcn2);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn2=(void *)tl0;
			}
#endif
		}else
		{
			img=sctx->tex_cur2;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			sctx->tex_img2=img->tex_img;
			sctx->tex_img_bcn2=img->tex_img_bcn;
			sctx->tex_xshl2=shx;
			sctx->tex_yshl2=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn2)
			{
				tl0=(u64)(sctx->tex_img_bcn2);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn2=(void *)tl0;
			}
#endif
		}
	}
#endif

//	ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;

//	__debugbreak();

//	printf("pv0: %p  %d %d %d\n", &pv0, pv0.x, pv0.y, pv0.z);
//	printf("pv1: %p  %d %d %d\n", &pv1, pv1.x, pv1.y, pv1.z);
//	printf("pv2: %p  %d %d %d\n", &pv2, pv2.x, pv2.y, pv2.z);


	tx0=pv0.x;		ty0=pv0.y;		tz0=pv0.z;
	v1_parm[TKRA_VX_XPOS]=tx0;
	v1_parm[TKRA_VX_YPOS]=ty0;
	v1_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	tx0=pv1.x;		ty0=pv1.y;		tz0=pv1.z;
	v2_parm[TKRA_VX_XPOS]=tx0;
	v2_parm[TKRA_VX_YPOS]=ty0;
	v2_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	tx0=pv2.x;		ty0=pv2.y;		tz0=pv2.z;
	v3_parm[TKRA_VX_XPOS]=tx0;
	v3_parm[TKRA_VX_YPOS]=ty0;
	v3_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	if(flipst)
	{
		v1_parm[TKRA_VX_TPOS]=((u64)(u32)it0) | (((u64)((u32)is0))<<32);
		v2_parm[TKRA_VX_TPOS]=((u64)(u32)it1) | (((u64)((u32)is1))<<32);
		v3_parm[TKRA_VX_TPOS]=((u64)(u32)it2) | (((u64)((u32)is2))<<32);
	}else
	{
		v1_parm[TKRA_VX_TPOS]=((u64)(u32)is0) | (((u64)((u32)it0))<<32);
		v2_parm[TKRA_VX_TPOS]=((u64)(u32)is1) | (((u64)((u32)it1))<<32);
		v3_parm[TKRA_VX_TPOS]=((u64)(u32)is2) | (((u64)((u32)it2))<<32);
	}
	
	if(flipst2)
	{
		v1_parm[TKRA_VX_T2POS]=((u64)(u32)itb0) | (((u64)((u32)isb0))<<32);
		v2_parm[TKRA_VX_T2POS]=((u64)(u32)itb1) | (((u64)((u32)isb1))<<32);
		v3_parm[TKRA_VX_T2POS]=((u64)(u32)itb2) | (((u64)((u32)isb2))<<32);
	}else
	{
		v1_parm[TKRA_VX_T2POS]=((u64)(u32)isb0) | (((u64)((u32)itb0))<<32);
		v2_parm[TKRA_VX_T2POS]=((u64)(u32)isb1) | (((u64)((u32)itb1))<<32);
		v3_parm[TKRA_VX_T2POS]=((u64)(u32)isb2) | (((u64)((u32)itb2))<<32);
	}
	
	v1_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv0.rgb);
	v2_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv1.rgb);
	v3_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv2.rgb);

	if(ctx->stateflag1&TKRA_STFL1_FILL_LINE)
	{
		TKRA_WalkLine(ctx, v1_parm, v2_parm);
		TKRA_WalkLine(ctx, v2_parm, v3_parm);
		TKRA_WalkLine(ctx, v3_parm, v1_parm);
	}else
	{
		TKRA_WalkTriangle(ctx, v1_parm, v2_parm, v3_parm);
	}

	return(0);
}

// #define TKRA_CheckZCullQuad_Bias	512

int TKRA_CheckZCullQuad(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3)
{
	tkra_zbufpixel *zbuf;
	int ds0, ds1, ds2, ds3, dt0, dt1, dt2, dt3, dst;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst;
	int clip_mx, clip_nx, clip_my, clip_ny;

	return(0);

	if(!(ctx->stateflag1&TKRA_STFL1_DEPTHTEST))
		return(0);
	if(	(ctx->zat_zfunc!=TKRA_ZAT_LT) &&
		(ctx->zat_zfunc!=TKRA_ZAT_LE))
			return(0);
	
	zbuf=ctx->screen_zbuf;

	clip_mx=ctx->clip_x0;		clip_nx=ctx->clip_x1;
	clip_my=ctx->clip_y0;		clip_ny=ctx->clip_y1;

	tx0=(pv0.x>>16);		ty0=(pv0.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);
	tx0=(pv1.x>>16);		ty0=(pv1.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);
	tx0=(pv2.x>>16);		ty0=(pv2.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);
	tx0=(pv3.x>>16);		ty0=(pv3.y>>16);
	if(	(tx0<=clip_mx) || (ty0<=clip_my)	||
		(tx0>=clip_nx) || (ty0>=clip_ny)	)
			return(0);


	dxy=ctx->screen_xsize*ctx->screen_ysize;
	
	tx0=(pv0.x>>16);		ty0=(pv0.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	
	dy0=(s16)(pv0.z>>16)-TKRA_CheckZCullQuad_Bias;

	if(zbuf[dx0]>dy0)
		return(0);

	tx0=(pv3.x>>16);		ty0=(pv3.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx3=(ty0*ctx->screen_xsize)+tx0;
	dy3=(s16)(pv3.z>>16)-TKRA_CheckZCullQuad_Bias;

	if(zbuf[dx3]>dy3)
		return(0);

	tx0=(pv1.x>>16);		ty0=(pv1.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx1=(ty0*ctx->screen_xsize)+tx0;
	dy1=(s16)(pv1.z>>16)-TKRA_CheckZCullQuad_Bias;

	tx0=(pv2.x>>16);		ty0=(pv2.y>>16);
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx2=(ty0*ctx->screen_xsize)+tx0;
	dy2=(s16)(pv2.z>>16)-TKRA_CheckZCullQuad_Bias;

	if((zbuf[dx1]>dy1) || (zbuf[dx2]>dy2))
		return(0);

	dx2=(pv0.x>>16)+(pv1.x>>16)+(pv2.x>>16)+(pv3.x>>16);
	dy2=(pv0.y>>16)+(pv1.y>>16)+(pv2.y>>16)+(pv3.y>>16);
	ds2=(pv0.z>>16)+(pv1.z>>16)+(pv2.z>>16)+(pv3.z>>16);

	dx2=dx2>>2;
	dy2=dx2>>2;
	ds2=ds2>>2;

//	dx2=__int_clamp(dx2, clip_mx, clip_nx);
//	dy2=__int_clamp(dy2, clip_my, clip_ny);

	dx0=(dy2*ctx->screen_xsize)+(dx2);
	dy0=((s16)ds2)-TKRA_CheckZCullQuad_Bias;

	if(zbuf[dx0]>dy0)
		return(0);

#if 1
	tx0=((pv0.x>>16)+dx2)>>1;	ty0=((pv0.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	dy0=(((s16)(pv0.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	tx0=((pv2.x>>16)+dx2)>>1;	ty0=((pv2.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx1=(ty0*ctx->screen_xsize)+tx0;
	dy1=(((s16)(pv2.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	if((zbuf[dx0]>dy0) || (zbuf[dx1]>dy1))
		return(0);
#endif

#if 1
	tx0=((pv1.x>>16)+dx2)>>1;	ty0=((pv1.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx0=(ty0*ctx->screen_xsize)+tx0;
	dy0=(((s16)(pv1.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	tx0=((pv3.x>>16)+dx2)>>1;	ty0=((pv3.y>>16)+dy2)>>1;
//	tx0=__int_clamp(tx0, clip_mx, clip_nx);
//	ty0=__int_clamp(ty0, clip_my, clip_ny);
	dx1=(ty0*ctx->screen_xsize)+tx0;
	dy1=(((s16)(pv3.z>>16)+ds2)>>1)-TKRA_CheckZCullQuad_Bias;

	if((zbuf[dx0]>dy0) || (zbuf[dx1]>dy1))
		return(0);
#endif

	return(1);	
}

int TKRA_EmitProjectedQuadI(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3)
{
	u64	v1_parm[TKRA_VX_NPARM];
	u64	v2_parm[TKRA_VX_NPARM];
	u64	v3_parm[TKRA_VX_NPARM];
	u64	v4_parm[TKRA_VX_NPARM];
	TKRA_TexImage *img;
	TKRA_SvContext *sctx;

	u64 tl0, tl1;
	int is0, is1, is2, is3, it0, it1, it2, it3, mip, mag;
	int isb0, isb1, isb2, isb3, itb0, itb1, itb2, itb3, mip2, mag2;
	int ds0, ds1, ds2, ds3, dt0, dt1, dt2, dt3, dst;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst, flipst2;
	int clip_mx, clip_nx, clip_my, clip_ny;

	if((tkra_nommio&3)!=2)
	{
		if(TKRA_CheckZCullQuad(ctx, pv0, pv1, pv2, pv3))
			return(-1);
	}

	sctx=ctx->svctx;

	is0=pv0.s;		is1=pv1.s;		is2=pv2.s;		is3=pv3.s;
	it0=pv0.t;		it1=pv1.t;		it2=pv2.t;		it3=pv3.t;
	isb0=pv0.s2;	isb1=pv1.s2;	isb2=pv2.s2;	isb3=pv3.s2;
	itb0=pv0.t2;	itb1=pv1.t2;	itb2=pv2.t2;	itb3=pv3.t2;

	flipst=0;

	mmip=sctx->tex_nmip;
	if(1)
	{
#if 1
		dx0=pv1.x-pv0.x;	dx1=pv2.x-pv1.x;
		dy0=pv1.y-pv0.y;	dy1=pv2.y-pv1.y;
		dx0=dx0^(dx0>>31);	dx1=dx1^(dx1>>31);
		dy0=dy0^(dy0>>31);	dy1=dy1^(dy1>>31);
		dxy=dx0+dx1+dy0+dy1;

		ds0=is1-is0;		ds1=is2-is1;
		dt0=it1-it0;		dt1=it2-it1;
		ds0=ds0^(ds0>>31);	ds1=ds1^(ds1>>31);
		dt0=dt0^(dt0>>31);	dt1=dt1^(dt1>>31);
		dst=ds0+ds1+dt0+dt1;
#endif
		
		dx2=(dx0+dx1)>>16;
		dy2=(dy0+dy1)>>16;
		dx3=dxy>>16;
		
		if((dx2<2) || (dy2<2))
			return;
		if(dx3<3)
			{ return; }
		if(dx3<6)
		{
			TKRA_EmitProjectedQuadPts(ctx, pv0, pv1, pv2, pv3);
			return;
		}
		
		mip=0;
		mag=dst<=dxy;

#if 1
		while((dst>dxy) && (mip<(mmip-1)))
			{ dst>>=1;	mip++; }
#endif

		if(sctx->tex_flag&TKRA_TRFL_FLIPST)
			flipst=1;

		if((mip>0) && (sctx->tex_flag&TKRA_TRFL_HASMIP))
		{
			is0>>=mip;	is1>>=mip;	is2>>=mip;	is3>>=mip;
			it0>>=mip;	it1>>=mip;	it2>>=mip;	it3>>=mip;
			mag=0;

			img=sctx->tex_cur;

			shx=img->tex_xshl-mip;
			shy=img->tex_yshl-mip;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			sctx->tex_img=img->tex_img+img->tex_mipofs[mip];
			sctx->tex_img_bcn=img->tex_img_bcn+img->tex_mipofs_bcn[mip];
			sctx->tex_xshl=shx;
			sctx->tex_yshl=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn)
			{
				tl0=(u64)(sctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				if(img->tex_flag&TKRA_TRFL_PIXFMT_UTX3)
					tx0|=2<<9;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}else
		{
			img=sctx->tex_cur;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			sctx->tex_img=img->tex_img;
			sctx->tex_img_bcn=img->tex_img_bcn;
			sctx->tex_xshl=shx;
			sctx->tex_yshl=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn)
			{
				tl0=(u64)(sctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				if(img->tex_flag&TKRA_TRFL_PIXFMT_UTX3)
					tx0|=2<<9;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}
		
//		if(mip>0)
		if(!mag)
		{
			sctx->DrawSpan=sctx->DrawSpan_Min;
			sctx->DrawSpanHZt=sctx->DrawSpanHZt_Min;
			sctx->DrawSpanZt=sctx->DrawSpanZt_Min;
		}
		else
		{
			sctx->DrawSpan=sctx->DrawSpan_Mag;
			sctx->DrawSpanHZt=sctx->DrawSpanHZt_Mag;
			sctx->DrawSpanZt=sctx->DrawSpanZt_Mag;
		}
	}

#if 1
//	mmip=ctx->tex_mmip;
	mmip=sctx->tex_nmip2;
//	if(mmip>0)
//	if((mmip>0) && (ctx->tex_flag&TKRA_TRFL_HASMIP))
//	if(0)
	if(sctx->tex_cur2)
	{
#if 1
		dx0=pv1.x-pv0.x;	dx1=pv2.x-pv1.x;
		dy0=pv1.y-pv0.y;	dy1=pv2.y-pv1.y;
		dx0=dx0^(dx0>>31);	dx1=dx1^(dx1>>31);
		dy0=dy0^(dy0>>31);	dy1=dy1^(dy1>>31);
		dxy=dx0+dx1+dy0+dy1;

		ds0=isb1-isb0;		ds1=isb2-isb1;
		dt0=itb1-itb0;		dt1=itb2-itb1;
		ds0=ds0^(ds0>>31);	ds1=ds1^(ds1>>31);
		dt0=dt0^(dt0>>31);	dt1=dt1^(dt1>>31);
		dst=ds0+ds1+dt0+dt1;
#endif
		
		mip2=0;
		mag2=dst<=dxy;

#if 1
		while((dst>dxy) && (mip2<(mmip-1)))
			{ dst>>=1;	mip2++; }
#endif

		if(sctx->tex_flag2&TKRA_TRFL_FLIPST)
			flipst2=1;

		if((mip2>0) && (sctx->tex_flag&TKRA_TRFL_HASMIP))
		{
			isb0>>=mip;	isb1>>=mip;	isb2>>=mip;	isb3>>=mip;
			itb0>>=mip;	itb1>>=mip;	itb2>>=mip;	itb3>>=mip;
			mag=0;

			img=sctx->tex_cur2;

			shx=img->tex_xshl-mip2;
			shy=img->tex_yshl-mip2;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			sctx->tex_img2=img->tex_img+img->tex_mipofs[mip];
			sctx->tex_img_bcn2=img->tex_img_bcn+img->tex_mipofs_bcn[mip];
			sctx->tex_xshl2=shx;
			sctx->tex_yshl2=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn2)
			{
				tl0=(u64)(sctx->tex_img_bcn2);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn2=(void *)tl0;
			}
#endif
		}else
		{
			img=sctx->tex_cur2;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			sctx->tex_img2=img->tex_img;
			sctx->tex_img_bcn2=img->tex_img_bcn;
			sctx->tex_xshl2=shx;
			sctx->tex_yshl2=shy;

// #ifdef __BJX2__
#ifdef BJX2_MMIO
			if(sctx->tex_img_bcn2)
			{
				tl0=(u64)(sctx->tex_img_bcn2);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				sctx->tex_img_bcn2=(void *)tl0;
			}
#endif
		}
	}
#endif

	tx0=pv0.x;		ty0=pv0.y;		tz0=pv0.z;
	v1_parm[TKRA_VX_XPOS]=tx0;
	v1_parm[TKRA_VX_YPOS]=ty0;
	v1_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	tx0=pv1.x;		ty0=pv1.y;		tz0=pv1.z;
	v2_parm[TKRA_VX_XPOS]=tx0;
	v2_parm[TKRA_VX_YPOS]=ty0;
	v2_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	tx0=pv2.x;		ty0=pv2.y;		tz0=pv2.z;
	v3_parm[TKRA_VX_XPOS]=tx0;
	v3_parm[TKRA_VX_YPOS]=ty0;
	v3_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	tx0=pv3.x;		ty0=pv3.y;		tz0=pv3.z;
	v4_parm[TKRA_VX_XPOS]=tx0;
	v4_parm[TKRA_VX_YPOS]=ty0;
	v4_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

	if(flipst)
	{
		v1_parm[TKRA_VX_TPOS]=((u64)(u32)it0) | (((u64)((u32)is0))<<32);
		v2_parm[TKRA_VX_TPOS]=((u64)(u32)it1) | (((u64)((u32)is1))<<32);
		v3_parm[TKRA_VX_TPOS]=((u64)(u32)it2) | (((u64)((u32)is2))<<32);
		v4_parm[TKRA_VX_TPOS]=((u64)(u32)it3) | (((u64)((u32)is3))<<32);
	}else
	{
		v1_parm[TKRA_VX_TPOS]=((u64)(u32)is0) | (((u64)((u32)it0))<<32);
		v2_parm[TKRA_VX_TPOS]=((u64)(u32)is1) | (((u64)((u32)it1))<<32);
		v3_parm[TKRA_VX_TPOS]=((u64)(u32)is2) | (((u64)((u32)it2))<<32);
		v4_parm[TKRA_VX_TPOS]=((u64)(u32)is3) | (((u64)((u32)it3))<<32);
	}
	
	if(sctx->tex_cur2)
	{
		if(flipst2)
		{
			v1_parm[TKRA_VX_T2POS]=((u64)(u32)itb0) | (((u64)((u32)isb0))<<32);
			v2_parm[TKRA_VX_T2POS]=((u64)(u32)itb1) | (((u64)((u32)isb1))<<32);
			v3_parm[TKRA_VX_T2POS]=((u64)(u32)itb2) | (((u64)((u32)isb2))<<32);
			v4_parm[TKRA_VX_T2POS]=((u64)(u32)itb3) | (((u64)((u32)isb3))<<32);
		}else
		{
			v1_parm[TKRA_VX_T2POS]=((u64)(u32)isb0) | (((u64)((u32)itb0))<<32);
			v2_parm[TKRA_VX_T2POS]=((u64)(u32)isb1) | (((u64)((u32)itb1))<<32);
			v3_parm[TKRA_VX_T2POS]=((u64)(u32)isb2) | (((u64)((u32)itb2))<<32);
			v4_parm[TKRA_VX_T2POS]=((u64)(u32)isb3) | (((u64)((u32)itb3))<<32);
		}
	}

	v1_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv0.rgb);
	v2_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv1.rgb);
	v3_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv2.rgb);
	v4_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv3.rgb);

	if(ctx->stateflag1&TKRA_STFL1_FILL_LINE)
	{
		TKRA_WalkLine(ctx, v1_parm, v2_parm);
		TKRA_WalkLine(ctx, v2_parm, v3_parm);
		TKRA_WalkLine(ctx, v3_parm, v4_parm);
		TKRA_WalkLine(ctx, v4_parm, v1_parm);
	}else
	{
		TKRA_WalkQuad(ctx, v1_parm, v2_parm, v3_parm, v4_parm);
	}

	return(0);
}

#if 0
int TKRA_EmitProjectedQuad(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3)
{
	int rt1, rt2;

	rt1=TKRA_EmitProjectedTriangle(ctx, pv0, pv1, pv2);
	rt2=TKRA_EmitProjectedTriangle(ctx, pv0, pv2, pv3);
	if((rt1<0) && (rt2<0))
		return(rt1);
	
	return(0);
}
#endif

int TKRA_EmitProjectedPoint(
	TKRA_Context *ctx,
	tkra_projvertex pv0)
{
	u64	v1_parm[TKRA_VX_NPARM];
	TKRA_SvContext *sctx;
	TKRA_TexImage *img;

	int is0, it0;
	int tx0, ty0, tz0, shx, shy;

	sctx=ctx->svctx;
	if(1)
	{		
		img=sctx->tex_cur;
		shx=img->tex_xshl;
		shy=img->tex_yshl;
		sctx->tex_img=img->tex_img;
		sctx->tex_img_bcn=img->tex_img_bcn;
		sctx->tex_xshl=shx;
		sctx->tex_yshl=shy;
		
		sctx->DrawSpan=sctx->DrawSpan_Min;
		sctx->DrawSpanHZt=sctx->DrawSpanHZt_Min;
		sctx->DrawSpanZt=sctx->DrawSpanZt_Min;
	}

	is0=pv0.s;		it0=pv0.t;
	tx0=pv0.x;		ty0=pv0.y;		tz0=pv0.z;
	v1_parm[TKRA_VX_XPOS]=tx0;
	v1_parm[TKRA_VX_YPOS]=ty0;
	v1_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);
	v1_parm[TKRA_VX_TPOS]=((u64)(u32)is0) | (((u64)((u32)it0))<<32);	
	v1_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv0.rgb);
	TKRA_WalkPoint(ctx, v1_parm);

//	TKRA_WalkTriangle(ctx, v1_parm, v2_parm, v3_parm);
	return(0);
}

int TKRA_EmitProjectedTrianglePts(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2)
{
	TKRA_EmitProjectedPoint(ctx, pv0);
	TKRA_EmitProjectedPoint(ctx, pv1);
	TKRA_EmitProjectedPoint(ctx, pv2);
}

int TKRA_EmitProjectedQuadPts(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3)
{
	TKRA_EmitProjectedPoint(ctx, pv0);
	TKRA_EmitProjectedPoint(ctx, pv1);
	TKRA_EmitProjectedPoint(ctx, pv2);
	TKRA_EmitProjectedPoint(ctx, pv3);
}

void TKRA_TransProjVertexMidpointPersp(
	TKRA_Context *ctx,
	tkra_projvertex *pv0,
	tkra_projvertex *pv1,
	tkra_projvertex *pv4)
{
	TKRA_TexImage *img, *img2;
	float si, ti, zi, s, t, z;

	img=ctx->svctx->tex_cur;
	img2=ctx->svctx->tex_cur2;

	zi=(pv0->rcp_z+pv1->rcp_z)*0.5;
	si=(pv0->rcp_s+pv1->rcp_s)*0.5;
	ti=(pv0->rcp_t+pv1->rcp_t)*0.5;
	z=tkra_frcp_fast(zi);
//	z=1.0/zi;
	pv4->x=(((s64)pv0->x)+pv1->x)>>1;
	pv4->y=(((s64)pv0->y)+pv1->y)>>1;
	pv4->z=(((s64)pv0->z)+pv1->z)>>1;
//	pv4->z=TKRA_FinalProjectVertex_Float2Fixed32pN(z, 16);
	pv4->s=TKRA_FinalProjectVertex_Float2Fixed32pN(si*z, 16+img->tex_xshl);
	pv4->t=TKRA_FinalProjectVertex_Float2Fixed32pN(ti*z, 16+img->tex_yshl);
	pv4->rgb=tkra_rgba_midpoint(pv0->rgb, pv1->rgb);
	pv4->rcp_s=si;
	pv4->rcp_t=ti;
	pv4->rcp_z=zi;

	if(img2)
	{
		si=(pv0->rcp_s2+pv1->rcp_s2)*0.5;
		ti=(pv0->rcp_t2+pv1->rcp_t2)*0.5;
		pv4->s2=TKRA_FinalProjectVertex_Float2Fixed32pN(si*z,
			16+img2->tex_xshl);
		pv4->t2=TKRA_FinalProjectVertex_Float2Fixed32pN(ti*z,
			16+img2->tex_yshl);
		pv4->rcp_s2=si;
		pv4->rcp_t2=ti;
	}
}

TKRA_PrimSubDiv *TKRA_AllocPrimSubDiv(TKRA_Context *ctx)
{
	TKRA_PrimSubDiv *ptmp;
	int i;

	ptmp=ctx->svctx->subdiv_free;
	if(ptmp)
	{
		ctx->svctx->subdiv_free=ptmp->next;
		ptmp->next=NULL;
		return(ptmp);
	}
	
	ptmp=tkra_malloc(64*sizeof(TKRA_PrimSubDiv));
	for(i=0; i<63; i++)
	{
		ptmp->next=ctx->svctx->subdiv_free;
		ctx->svctx->subdiv_free=ptmp;
		ptmp++;
	}
	
	ptmp->next=NULL;
	return(ptmp);
}

void TKRA_FreePrimSubDiv(TKRA_Context *ctx, TKRA_PrimSubDiv *pcur)
{
	pcur->next=ctx->svctx->subdiv_free;
	ctx->svctx->subdiv_free=pcur;
}

TKRA_PrimSubDiv *TKRA_AddSubDivListQuad(
	TKRA_Context *ctx,
	TKRA_PrimSubDiv *plst,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3,
	int lvl)
{
	TKRA_PrimSubDiv *pcur;
	pcur=TKRA_AllocPrimSubDiv(ctx);
	pcur->pv0=pv0;	pcur->pv1=pv1;
	pcur->pv2=pv2;	pcur->pv3=pv3;
	pcur->lvl=lvl;
	pcur->pvt=4;
	pcur->next=plst;
	return(pcur);
}

TKRA_PrimSubDiv *TKRA_AddSubDivListTriangle(
	TKRA_Context *ctx,
	TKRA_PrimSubDiv *plst,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	int lvl)
{
	TKRA_PrimSubDiv *pcur;
	pcur=TKRA_AllocPrimSubDiv(ctx);
	pcur->pv0=pv0;	pcur->pv1=pv1;
	pcur->pv2=pv2;	pcur->pv3=pv2;
	pcur->lvl=lvl;
	pcur->pvt=3;
	pcur->next=plst;
	return(pcur);
}

int TKRA_EmitProjectedQuadI(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2,
	tkra_projvertex pv3);
int TKRA_EmitProjectedTriangleI(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2);

int TKRA_EmitProjectedTriangle(
	TKRA_Context *ctx,
	tkra_projvertex ipv0,
	tkra_projvertex ipv1,
	tkra_projvertex ipv2);

int TKRA_FixedDistXY(int x0, int y0, int x1, int y1)
{
	int dx, dy, d;
	dx=x0-x1;		dy=y0-y1;
	dx^=dx>>31;		dy^=dy>>31;
	d=dx+(dy>>1);
	if(dy>dx)
		d=dy+(dx>>1);
	return(d);
}

int TKRA_EmitProjectedSubDiv(
	TKRA_Context *ctx,
	TKRA_PrimSubDiv *pilst)
{
	TKRA_PrimSubDiv *plst, *pcur;
	tkra_projvertex pv0, pv1, pv2, pv3, pv4, pv5, pv6, pv7, pv8, pv9;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3;
	int dz0, dz1, dz2, dz3, dz4;
	int sdl0, sdl1, sdl2, sdl3, sdl4, sdl5, sdl6, sdl7;
	int sdi0, sdi1, sdi2, sdi3, sdi4, lv1;
	int sds0, sds1, sds2, sds3, subdivlim;

	plst=pilst;
	while(plst)
	{
		pcur=plst;
		plst=pcur->next;

		if(pcur->pvt==4)
		{
			pv0=pcur->pv0;
			pv1=pcur->pv1;
			pv2=pcur->pv2;
			pv3=pcur->pv3;

			sdl0=TKRA_FixedDistXY(pv0.x, pv0.y, pv1.x, pv1.y)>>16;
			sdl1=TKRA_FixedDistXY(pv1.x, pv1.y, pv2.x, pv2.y)>>16;
			sdl2=TKRA_FixedDistXY(pv2.x, pv2.y, pv3.x, pv3.y)>>16;
			sdl3=TKRA_FixedDistXY(pv3.x, pv3.y, pv0.x, pv0.y)>>16;

			dz0=pv0.z-pv1.z;	dz1=pv1.z-pv2.z;
			dz2=pv2.z-pv3.z;	dz3=pv3.z-pv0.z;
			dz0^=dz0>>31;		dz1^=dz1>>31;
			dz2^=dz2>>31;		dz3^=dz3>>31;
			
			dz4=(dz0+dz1+dz2+dz3)>>16;
			subdivlim=TKRA_PARAM_SCRQUADSUBDIV;
			if(dz4<256)
			{
				if(dz4<16)		{ subdivlim<<=4; }
				else			{ subdivlim<<=2; }
			}

			sdl4=sdl0+sdl1+sdl2+sdl3;
			if((sdl4<12) || ((sdl0<2) && (sdl2<2)) || ((sdl1<2) && (sdl3<2)))
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				continue;
			}

			lv1=pcur->lvl;

#if 1
			if(sdl0<2)
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv0, &pv1, &pv4);
				plst=TKRA_AddSubDivListTriangle(ctx, plst,
					pv4, pv2, pv3, lv1);
				continue;
			}
			if(sdl1<2)
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv1, &pv2, &pv5);
				plst=TKRA_AddSubDivListTriangle(ctx, plst,
					pv0, pv5, pv3, lv1);
				continue;
			}
			if(sdl2<2)
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv2, &pv3, &pv6);
				plst=TKRA_AddSubDivListTriangle(ctx, plst,
					pv0, pv1, pv6, lv1);
				continue;
			}
			if(sdl3<2)
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv3, &pv0, &pv7);
				plst=TKRA_AddSubDivListTriangle(ctx, plst,
					pv7, pv1, pv2, lv1);
				continue;
			}
#endif

//			sdi0=	(sdl0>TKRA_PARAM_SCRQUADSUBDIV) ;
//			sdi1=	(sdl1>TKRA_PARAM_SCRQUADSUBDIV) ;
//			sdi2=	(sdl2>TKRA_PARAM_SCRQUADSUBDIV) ;
//			sdi3=	(sdl3>TKRA_PARAM_SCRQUADSUBDIV) ;

			sdi0=(sdl0>subdivlim);	sdi1=(sdl1>subdivlim);
			sdi2=(sdl2>subdivlim);	sdi3=(sdl3>subdivlim);

			sdl5=sdl4>>3;
				
			if(sdi0&sdi1&sdi2&sdi3)
			{
				if((sdl0<sdl5)|(sdl2<sdl5))
					{ sdi0=0; sdi2=0; }
				if((sdl1<sdl5)|(sdl3<sdl5))
					{ sdi1=0; sdi3=0; }
			}

			lv1=pcur->lvl+1;

			if((lv1<4) && (sdi0|sdi1|sdi2|sdi3))
			{
				sdl6=TKRA_FixedDistXY(pv0.x, pv0.y, pv2.x, pv2.y)>>16;
				sdl7=TKRA_FixedDistXY(pv1.x, pv1.y, pv3.x, pv3.y)>>16;
				
				if((sdl6<sdl5) || (sdl7<sdl5))
				{
//					if(	(sdl6<TKRA_PARAM_SCRQUADSUBDIV) ||
//						(sdl7<TKRA_PARAM_SCRQUADSUBDIV) )
					if(	(sdl6<subdivlim) ||
						(sdl7<subdivlim) )
					{
						sdi0=0; sdi2=0;
						sdi1=0; sdi3=0;
					}else if(sdl6<sdl5)
					{
						TKRA_FreePrimSubDiv(ctx, pcur);
						plst=TKRA_AddSubDivListTriangle(ctx, plst,
							pv0, pv1, pv2, lv1);
						plst=TKRA_AddSubDivListTriangle(ctx, plst,
							pv0, pv2, pv3, lv1);
						continue;
					}else if(sdl7<sdl5)
					{
						TKRA_FreePrimSubDiv(ctx, pcur);
						plst=TKRA_AddSubDivListTriangle(ctx, plst,
							pv0, pv1, pv3, lv1);
						plst=TKRA_AddSubDivListTriangle(ctx, plst,
							pv1, pv2, pv3, lv1);
						continue;
					}
				}
			}

			if((lv1<4) && (sdi0|sdi1|sdi2|sdi3))
			{
				TKRA_TransProjVertexMidpointPersp(ctx, &pv0, &pv1, &pv4);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv1, &pv2, &pv5);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv2, &pv3, &pv6);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv3, &pv0, &pv7);

				if(sdi0|sdi2)
				{
					if(sdi1|sdi3)
					{
		//				TKRA_TransProjVertexMidpointPersp(ctx,
		//					&pv4, &pv6, &pv8);
		//				TKRA_TransProjVertexMidpointPersp(ctx,
		//					&pv5, &pv7, &pv9);
		//				TKRA_TransProjVertexMidpointPersp(ctx,
		//					&pv8, &pv9, &pv8);

						if((sdl0+sdl2)>(sdl1+sdl3))
						{
							TKRA_TransProjVertexMidpointPersp(ctx,
								&pv4, &pv6, &pv8);
						}
						else
						{
							TKRA_TransProjVertexMidpointPersp(ctx,
								&pv5, &pv7, &pv8);
						}

						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv0, pv4, pv8, pv7, lv1);
						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv4, pv1, pv5, pv8, lv1);
						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv5, pv2, pv6, pv8, lv1);
						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv6, pv3, pv7, pv8, lv1);
					}else
					{
						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv0, pv4, pv6, pv3, lv1);
						plst=TKRA_AddSubDivListQuad(ctx, plst,
							pv4, pv1, pv2, pv6, lv1);
					}
				}else
				{
					plst=TKRA_AddSubDivListQuad(ctx, plst,
						pv0, pv1, pv5, pv7, lv1);
					plst=TKRA_AddSubDivListQuad(ctx, plst,
						pv5, pv2, pv3, pv7, lv1);
				}
			}else
			{
				TKRA_EmitProjectedQuadI(ctx, pv0, pv1, pv2, pv3);
			}
			TKRA_FreePrimSubDiv(ctx, pcur);
			continue;
		}
		
		if(pcur->pvt==3)
		{
			pv0=pcur->pv0;
			pv1=pcur->pv1;
			pv2=pcur->pv2;

			sdl0=TKRA_FixedDistXY(pv0.x, pv0.y, pv1.x, pv1.y)>>16;
			sdl1=TKRA_FixedDistXY(pv1.x, pv1.y, pv2.x, pv2.y)>>16;
			sdl2=TKRA_FixedDistXY(pv2.x, pv2.y, pv0.x, pv0.y)>>16;

			dz0=pv0.z-pv1.z;	dz1=pv1.z-pv2.z;	dz2=pv2.z-pv0.z;
			dz0^=dz0>>31;		dz1^=dz1>>31;		dz2^=dz2>>31;
			
			dz4=(dz0+dz1+dz2)>>16;
			subdivlim=TKRA_PARAM_SCRTRISUBDIV;
			if(dz4<256)
			{
				if(dz4<16)		{ subdivlim<<=4; }
				else			{ subdivlim<<=2; }
			}

			sdl4=sdl0+sdl1+sdl2;
			if((sdl4<8) || (sdl0<2) || (sdl1<2) || (sdl2<2))
			{
				TKRA_FreePrimSubDiv(ctx, pcur);
				continue;
			}

//			sdi0=(sdl0>TKRA_PARAM_SCRTRISUBDIV);
//			sdi1=(sdl1>TKRA_PARAM_SCRTRISUBDIV);
//			sdi2=(sdl2>TKRA_PARAM_SCRTRISUBDIV);

			sdi0=(sdl0>subdivlim);
			sdi1=(sdl1>subdivlim);
			sdi2=(sdl2>subdivlim);

			sds0=sdl0<(sdl4>>2);
			sds1=sdl1<(sdl4>>2);
			sds2=sdl2<(sdl4>>2);

			lv1=pcur->lvl+1;

			if((lv1<4) && (sdi0|sdi1|sdi2))
			{
				TKRA_TransProjVertexMidpointPersp(ctx, &pv0, &pv1, &pv4);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv1, &pv2, &pv5);
				TKRA_TransProjVertexMidpointPersp(ctx, &pv2, &pv0, &pv6);

				if(sds0)
				{
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv5, pv2, pv6, lv1);

//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv0, pv1, pv6, lv1);
//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv6, pv1, pv5, lv1);
					plst=TKRA_AddSubDivListQuad(ctx, plst,
						pv0, pv1, pv5, pv6, lv1);
				}else
					if(sds1)
				{
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv0, pv4, pv6, lv1);

//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv4, pv1, pv2, lv1);
//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv4, pv2, pv6, lv1);
					plst=TKRA_AddSubDivListQuad(ctx, plst,
						pv4, pv1, pv2, pv6, lv1);
				}else
					if(sds2)
				{
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv4, pv1, pv5, lv1);

//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv5, pv2, pv4, lv1);
//					plst=TKRA_AddSubDivListTriangle(ctx, plst,
//						pv4, pv2, pv0, lv1);
					plst=TKRA_AddSubDivListQuad(ctx, plst,
						pv5, pv2, pv0, pv4, lv1);
				}else
				{
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv0, pv4, pv6, lv1);
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv4, pv1, pv5, lv1);
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv5, pv2, pv6, lv1);
					plst=TKRA_AddSubDivListTriangle(ctx, plst,
						pv4, pv5, pv6, lv1);
				}
			}else
			{
				TKRA_EmitProjectedTriangleI(ctx, pv0, pv1, pv2);
			}
			TKRA_FreePrimSubDiv(ctx, pcur);
			continue;
		}
	}
}

int TKRA_EmitProjectedQuad(
	TKRA_Context *ctx,
	tkra_projvertex ipv0,
	tkra_projvertex ipv1,
	tkra_projvertex ipv2,
	tkra_projvertex ipv3)
{
	TKRA_PrimSubDiv *plst, *pcur;
//	tkra_projvertex pv0, pv1, pv2, pv3, pv4, pv5, pv6, pv7, pv8;
	tkra_projvertex pv4;
//	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3;
	int sdl0, sdl1, sdl2, sdl3, sdl4, sdl5, sdl6, sdl7;
	int sdi0, sdi1, sdi2, sdi3, sdi4, lv1;

	sdl0=TKRA_FixedDistXY(ipv0.x, ipv0.y, ipv1.x, ipv1.y)>>16;
	sdl1=TKRA_FixedDistXY(ipv1.x, ipv1.y, ipv2.x, ipv2.y)>>16;
	sdl2=TKRA_FixedDistXY(ipv2.x, ipv2.y, ipv3.x, ipv3.y)>>16;
	sdl3=TKRA_FixedDistXY(ipv3.x, ipv3.y, ipv0.x, ipv0.y)>>16;

	sdl4=sdl0+sdl1+sdl2+sdl3;
	if((sdl4<12) || ((sdl0<2) && (sdl2<2)) || ((sdl1<2) && (sdl3<2)))
	{
		return;
	}
	
	if(sdl0<2)
	{
		TKRA_TransProjVertexMidpointPersp(ctx, &ipv0, &ipv1, &pv4);
		TKRA_EmitProjectedTriangle(ctx, pv4, ipv2, ipv3);
		return;
	}
	if(sdl1<2)
	{
		TKRA_TransProjVertexMidpointPersp(ctx, &ipv1, &ipv2, &pv4);
		TKRA_EmitProjectedTriangle(ctx, ipv0, pv4, ipv3);
		return;
	}
	if(sdl2<2)
	{
		TKRA_TransProjVertexMidpointPersp(ctx, &ipv2, &ipv3, &pv4);
		TKRA_EmitProjectedTriangle(ctx, ipv0, ipv1, pv4);
		return;
	}
	if(sdl3<2)
	{
		TKRA_TransProjVertexMidpointPersp(ctx, &ipv3, &ipv0, &pv4);
		TKRA_EmitProjectedTriangle(ctx, pv4, ipv1, ipv2);
		return;
	}

	sdi0=	(sdl0>TKRA_PARAM_SCRQUADSUBDIV) ;
	sdi1=	(sdl1>TKRA_PARAM_SCRQUADSUBDIV) ;
	sdi2=	(sdl2>TKRA_PARAM_SCRQUADSUBDIV) ;
	sdi3=	(sdl3>TKRA_PARAM_SCRQUADSUBDIV) ;
	sdi4=	sdi0|sdi1|sdi2|sdi3;
	if(!sdi4)
	{
		TKRA_EmitProjectedQuadI(ctx, ipv0, ipv1, ipv2, ipv3);
		return;
	}

	plst=TKRA_AddSubDivListQuad(ctx, NULL, ipv0, ipv1, ipv2, ipv3, 0);
	TKRA_EmitProjectedSubDiv(ctx, plst);
	return(0);
}

int TKRA_EmitProjectedTriangle(
	TKRA_Context *ctx,
	tkra_projvertex ipv0,
	tkra_projvertex ipv1,
	tkra_projvertex ipv2)
{
	TKRA_PrimSubDiv *plst, *pcur;
//	tkra_projvertex pv0, pv1, pv2, pv3, pv4, pv5, pv6, pv7, pv8;
	int sdl0, sdl1, sdl2, sdl4;
	int sds0, sds1, sds2;
	int sdi0, sdi1, sdi2, sdi3, sdi4;

	sdl0=TKRA_FixedDistXY(ipv0.x, ipv0.y, ipv1.x, ipv1.y)>>16;
	sdl1=TKRA_FixedDistXY(ipv1.x, ipv1.y, ipv2.x, ipv2.y)>>16;
	sdl2=TKRA_FixedDistXY(ipv2.x, ipv2.y, ipv0.x, ipv0.y)>>16;

	sdl4=sdl0+sdl1+sdl2;
	if((sdl4<12) || (sdl0<2) || (sdl1<2) || (sdl2<2))
	{
		return;
	}
	
	if(sdl4<=(TKRA_PARAM_SCRTRISUBDIV*3))
	{
		TKRA_EmitProjectedTriangleI(ctx, ipv0, ipv1, ipv2);
		return(0);
	}
	
	plst=TKRA_AddSubDivListTriangle(ctx, NULL, ipv0, ipv1, ipv2, 0);
	TKRA_EmitProjectedSubDiv(ctx, plst);
	return(0);
}

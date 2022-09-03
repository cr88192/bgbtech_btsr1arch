void TKRA_UnpackMatrix16fv(tkra_mat4 mat, float *fv);

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
//	vd=vc*vc;
//	return(vd.x+vd.y+vd.z+vd.w);
	return(tkra_v4fdot(vc, vc));
	
//	float c, dx, dy, dz, dw;
//	dx=a.x-b.x;		dy=a.y-b.y;
//	dz=a.z-b.z;		dw=a.w-b.w;
//	c=(dx*dx)+(dy*dy)+(dz*dz)+(dw*dw);
//	return(c);
}

float tkra_v2f_dist(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f vc, vd;
	vc=a-b;
	vd=vc*vc;
	return(vd.x+vd.y);

//	float c, dx, dy;
//	dx=a.x-b.x;		dy=a.y-b.y;
//	c=(dx*dx)+(dy*dy);
//	return(c);
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

__asm {
tkra_mkvec2sfv:
	MOVU.L	(R4), R5
	SHLD.Q	R5, -16, R6	|	FLDCH	R5, R16
							FLDCH	R6, R17
//	FSTCF	R16, R2
//	FSTCFH	R17, R2

	FSTCF	R16, R20
	FSTCF	R17, R21
//	FSTCF	R18, R22
//	FSTCF	R19, R23
	MOVLD	R21, R20, R2
//	MOVLD	R23, R22, R3

	RTS

tkra_mkvec4sfv:
	MOV.Q	(R4), R5
	SHLD.Q	R5, -16, R6	|	FLDCH	R5, R16
	SHLD.Q	R5, -32, R7	|	FLDCH	R6, R17
	SHLD.Q	R5, -48, R6	|	FLDCH	R7, R18
							FLDCH	R6, R19
//	FSTCF	R16, R2
//	FSTCFH	R17, R2
//	FSTCF	R18, R3
//	FSTCFH	R19, R3

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

#else
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
#endif

// #ifdef __BJX2__
#if 0
float tkra_v4f_dist_xy(tkra_vec4f a, tkra_vec4f b);
tkra_vec4f tkra_v4f_midpoint(tkra_vec4f a, tkra_vec4f b);
tkra_vec2f tkra_v2f_midpoint(tkra_vec2f a, tkra_vec2f b);

__asm {
tkra_v4f_dist:
	PSUBX.F		R4, R6, R18
	PMULX.F		R18, R18, R20
	PADD.F		R20, R21, R22
	FLDCF		R22, R2
	FLDCFH		R22, R3
	FADD		R2, R3, R2
	RTSU

tkra_v4f_dist_xy:
	PSUB.F		R4, R6, R18
	PMUL.F		R18, R18, R20
	FLDCF		R20, R2
	FLDCFH		R20, R3
	FADD		R2, R3, R2
	RTSU
};

#endif

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
//	if(fabs(x)<0.01)
//		return(1.0);
//	return(1.0/(x));

//	return(1.0/(fabs(x)+0.25));
	return(1.0/(fabs(x)+0.001));
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
	RTSU
};

#else

tkra_vec4f TKRA_ProjectVertex(tkra_vec4f vec, tkra_mat4 mat)
{
	register tkra_vec4f v0x, v0y, v0z, v0w;
	float f;
	
//	f=tkra_v4f_x(vec);
//	v0x=tkra_mkvec4f(f, f, f, f);
//	f=tkra_v4f_y(vec);
//	v0y=tkra_mkvec4f(f, f, f, f);
//	f=tkra_v4f_z(vec);
//	v0z=tkra_mkvec4f(f, f, f, f);
//	f=tkra_v4f_w(vec);
//	v0w=tkra_mkvec4f(f, f, f, f);

	v0x=tkra_v4f_xxxx(vec);
	v0y=tkra_v4f_yyyy(vec);
	v0z=tkra_v4f_zzzz(vec);
	v0w=tkra_v4f_wwww(vec);
	
//	TKRA_DumpVec4(vec, "TKRA_ProjectVertex: vec");
	
//	TKRA_DumpMatrix(mat, "TKRA_ProjectVertex: proj");
	
	v0x=tkra_v4fmul(v0x, mat.row0);
	v0y=tkra_v4fmul(v0y, mat.row1);
	v0z=tkra_v4fmul(v0z, mat.row2);
	v0w=tkra_v4fmul(v0w, mat.row3);

//	TKRA_DumpVec4(v0x, "TKRA_ProjectVertex: v0x");
//	TKRA_DumpVec4(v0y, "TKRA_ProjectVertex: v0y");
//	TKRA_DumpVec4(v0z, "TKRA_ProjectVertex: v0z");
//	TKRA_DumpVec4(v0w, "TKRA_ProjectVertex: v0w");

	v0x=tkra_v4fadd(v0x, v0y);
	v0y=tkra_v4fadd(v0z, v0w);
	v0z=tkra_v4fadd(v0x, v0y);

//	TKRA_DumpVec4(v0x, "TKRA_ProjectVertex: v0a");
//	TKRA_DumpVec4(v0y, "TKRA_ProjectVertex: v0b");
//	TKRA_DumpVec4(v0z, "TKRA_ProjectVertex: v0c");

	return(v0z);

//	return(tkra_v4fadd(
//		tkra_v4fadd(v0x, v0y),
//		tkra_v4fadd(v0z, v0w)) );
	
//	return(tkra_v4fadd(
//		tkra_v4fadd(
//			tkra_v4fmul(vec, mat.row0),
//			tkra_v4fmul(vec, mat.row1)),
//		tkra_v4fadd(
//			tkra_v4fmul(vec, mat.row2),
//			tkra_v4fmul(vec, mat.row3))));
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

tkra_mat4 TKRA_MatrixMultiply(tkra_mat4 mata, tkra_mat4 matb)
{
	float xyz0[16];
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

tkra_mat4 TKRA_MatrixIdentify(void)
{
	tkra_mat4 matc;
	
	matc.row0=tkra_mkvec4f( 1, 0, 0, 0 );
	matc.row1=tkra_mkvec4f( 0, 1, 0, 0 );
	matc.row2=tkra_mkvec4f( 0, 0, 1, 0 );
	matc.row3=tkra_mkvec4f( 0, 0, 0, 1 );
	return(matc);
}

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
//	fv[ 0]=tkra_v4f_x(row);		fv[ 1]=tkra_v4f_y(row);
//	fv[ 2]=tkra_v4f_z(row);		fv[ 3]=tkra_v4f_w(row);
	tkra_upvec4fv(fv+0, row);
//	__debugbreak();
	row=mat.row1;
//	fv[ 4]=tkra_v4f_x(row);		fv[ 5]=tkra_v4f_y(row);
//	fv[ 6]=tkra_v4f_z(row);		fv[ 7]=tkra_v4f_w(row);
	tkra_upvec4fv(fv+4, row);
	row=mat.row2;
//	fv[ 8]=tkra_v4f_x(row);		fv[ 9]=tkra_v4f_y(row);
//	fv[10]=tkra_v4f_z(row);		fv[11]=tkra_v4f_w(row);
	tkra_upvec4fv(fv+8, row);
	row=mat.row3;
//	fv[12]=tkra_v4f_x(row);		fv[13]=tkra_v4f_y(row);
//	fv[14]=tkra_v4f_z(row);		fv[15]=tkra_v4f_w(row);
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

int TKRA_EmitProjectedTriangle(
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

	u64 tl0, tl1;
	int is0, is1, is2, it0, it1, it2, mip, mag;
	int ds0, ds1, ds2, dt0, dt1, dt2, dst;
	int dx0, dx1, dx2, dy0, dy1, dy2, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst;
	int clip_mx, clip_nx, clip_my, clip_ny;


	is0=pv0.s;	is1=pv1.s;	is2=pv2.s;
	it0=pv0.t;	it1=pv1.t;	it2=pv2.t;

	if(TKRA_CheckZCullTriangle(ctx, pv0, pv1, pv2))
		return(-1);

#if 0
	if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
//	if(0)
	{
//		zbuf=ctx->screen_zbuf2;
		zbuf=ctx->screen_zbuf;

		clip_mx=ctx->clip_x0;		clip_nx=ctx->clip_x1;
		clip_my=ctx->clip_y0;		clip_ny=ctx->clip_y1;

		dxy=ctx->screen_xsize*ctx->screen_ysize;
		
		tx0=(pv0.x>>16);		ty0=(pv0.y>>16);
		tx0=__int_clamp(tx0, clip_mx, clip_nx);
		ty0=__int_clamp(ty0, clip_my, clip_ny);
		dx0=(ty0*ctx->screen_xsize)+tx0;
		
//		dx0=((pv0.y>>16)*ctx->screen_xsize)+(pv0.x>>16);
		dy0=(s16)(pv0.z>>16)-256;

//		if(dx0<0)dx0=0;
//		if(dx0>dxy)dx0=dxy;

		if(zbuf[dx0]<dy0)
		{
			tx0=(pv1.x>>16);		ty0=(pv1.y>>16);
			tx0=__int_clamp(tx0, clip_mx, clip_nx);
			ty0=__int_clamp(ty0, clip_my, clip_ny);
			dx1=(ty0*ctx->screen_xsize)+tx0;

			tx0=(pv2.x>>16);		ty0=(pv2.y>>16);
			tx0=__int_clamp(tx0, clip_mx, clip_nx);
			ty0=__int_clamp(ty0, clip_my, clip_ny);
			dx2=(ty0*ctx->screen_xsize)+tx0;

//			dx1=((pv1.y>>16)*ctx->screen_xsize)+(pv1.x>>16);
			dy1=(s16)(pv1.z>>16)-256;

//			dx2=((pv2.y>>16)*ctx->screen_xsize)+(pv2.x>>16);
			dy2=(s16)(pv2.z>>16)-256;

//			if(dx0<0)dx0=0;
//			if(dx1<0)dx1=0;
//			if(dx2<0)dx2=0;

//			if(dx0>dxy)dx0=dxy;
//			if(dx1>dxy)dx1=dxy;
//			if(dx2>dxy)dx2=dxy;

//			if((zbuf[dx0]<dy0) && (zbuf[dx1]<dy1) && (zbuf[dx2]<dy2))
			if((zbuf[dx1]<dy1) && (zbuf[dx2]<dy2))
			{
				dx2=(pv0.x>>16)+(pv1.x>>16)+(pv2.x>>16);
				dy2=(pv0.y>>16)+(pv1.y>>16)+(pv2.y>>16);
				ds2=(pv0.z>>16)+(pv1.z>>16)+(pv2.z>>16);
				dx2=(dx2*85)>>8;
				dy2=(dx2*85)>>8;
				ds2=(ds2*85)>>8;

				tx0=__int_clamp(dx2, clip_mx, clip_nx);
				ty0=__int_clamp(dy2, clip_my, clip_ny);
				dx0=(ty0*ctx->screen_xsize)+tx0;

//				dx0=(dy2*ctx->screen_xsize)+(dx2);
				dy0=((s16)ds2)-256;

//				if(dx0<0)dx0=0;
//				if(dx0>dxy)dx0=dxy;

				if(zbuf[dx0]<dy0)
					return(-1);
			}
		}
	}
#endif

	flipst=0;

//	mmip=ctx->tex_mmip;
	mmip=ctx->tex_nmip;
//	if(mmip>0)
//	if((mmip>0) && (ctx->tex_flag&TKRA_TRFL_HASMIP))
//	if(0)
	if(1)
	{
#if 0
		ds0=is1-is0;	ds1=is2-is1;	ds2=is0-is2;
		dt0=it1-it0;	dt1=it2-it1;	dt2=it0-it2;
		dx0=pv1.x-pv0.x;	dx1=pv2.x-pv1.x;	dx2=pv0.x-pv2.x;
		dy0=pv1.y-pv0.y;	dy1=pv2.y-pv1.y;	dy2=pv0.y-pv2.y;
		
		dx0=dx0^(dx0>>31);	dx1=dx1^(dx1>>31);	dx2=dx2^(dx2>>31);
		dy0=dy0^(dy0>>31);	dy1=dy1^(dy1>>31);	dy2=dy2^(dy2>>31);
		dxy=dx0+dx1+dx2+dy0+dy1+dy2;

		ds0=ds0^(ds0>>31);	ds1=ds1^(ds1>>31);	ds2=ds2^(ds2>>31);
		dt0=dt0^(dt0>>31);	dt1=dt1^(dt1>>31);	dt2=dt2^(dt2>>31);
		dst=ds0+ds1+ds2+dt0+dt1+dt2;
#endif

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
		
		mip=0;
//		mag=(dst*2)<dxy;
		mag=dst<=dxy;

#if 1
		while((dst>dxy) && (mip<mmip))
			{ dst>>=1;	mip++; }
#endif

		if(ctx->tex_flag&TKRA_TRFL_FLIPST)
			flipst=1;

//		if(ctx->tex_flag&TKRA_TRFL_HASMIP)
		if((mip>0) && (ctx->tex_flag&TKRA_TRFL_HASMIP))
//		if(0)
		{
			is0>>=mip;	is1>>=mip;	is2>>=mip;
			it0>>=mip;	it1>>=mip;	it2>>=mip;
			mag=0;

			img=ctx->tex_cur;

			shx=img->tex_xshl-mip;
			shy=img->tex_yshl-mip;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			ctx->tex_img=img->tex_img+img->tex_mipofs[mip];
			ctx->tex_img_bcn=img->tex_img_bcn+img->tex_mipofs_bcn[mip];
			ctx->tex_xshl=shx;
			ctx->tex_yshl=shy;

#ifdef __BJX2__
			if(ctx->tex_img_bcn)
			{
				tl0=(u64)(ctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				ctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}else
		{
			img=ctx->tex_cur;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			ctx->tex_img=img->tex_img;
			ctx->tex_img_bcn=img->tex_img_bcn;
			ctx->tex_xshl=shx;
			ctx->tex_yshl=shy;

#ifdef __BJX2__
			if(ctx->tex_img_bcn)
			{
				tl0=(u64)(ctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				ctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}
		
//		if(mip>0)
		if(!mag)
		{
			ctx->DrawSpan=ctx->DrawSpan_Min;
			ctx->DrawSpanHZt=ctx->DrawSpanHZt_Min;
			ctx->DrawSpanZt=ctx->DrawSpanZt_Min;
		}
		else
		{
			ctx->DrawSpan=ctx->DrawSpan_Mag;
			ctx->DrawSpanHZt=ctx->DrawSpanHZt_Mag;
			ctx->DrawSpanZt=ctx->DrawSpanZt_Mag;
		}
	}else
	{
		img=ctx->tex_cur;
		ctx->tex_img=img->tex_img;
		ctx->tex_img_bcn=img->tex_img_bcn;
		ctx->DrawSpan=ctx->DrawSpan_Mag;
		ctx->DrawSpanHZt=ctx->DrawSpanHZt_Mag;
		ctx->DrawSpanZt=ctx->DrawSpanZt_Mag;
	}

//	ctx->DrawSpanZb=TKRA_DrawSpan_Zbuf;

//	__debugbreak();

//	printf("pv0: %p  %d %d %d\n", &pv0, pv0.x, pv0.y, pv0.z);
//	printf("pv1: %p  %d %d %d\n", &pv1, pv1.x, pv1.y, pv1.z);
//	printf("pv2: %p  %d %d %d\n", &pv2, pv2.x, pv2.y, pv2.z);


//	v1_parm[TKRA_VX_XPOS]=pv0.x<<16;
//	v1_parm[TKRA_VX_YPOS]=pv0.y<<16;
//	v1_parm[TKRA_VX_ZPOS]=pv0.z<<16;

//	tx0=(pv0.x<<16);	ty0=(pv0.y<<16);	tz0=(pv0.z<<16);
//	tx0=(pv0.x<<12);	ty0=(pv0.y<<12);	tz0=(pv0.z<<12);
	tx0=pv0.x;		ty0=pv0.y;		tz0=pv0.z;
	v1_parm[TKRA_VX_XPOS]=tx0;
	v1_parm[TKRA_VX_YPOS]=ty0;
	v1_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

//	v2_parm[TKRA_VX_XPOS]=pv1.x<<16;
//	v2_parm[TKRA_VX_YPOS]=pv1.y<<16;
//	v2_parm[TKRA_VX_ZPOS]=pv1.z<<16;

//	tx0=(pv1.x<<16);	ty0=(pv1.y<<16);	tz0=(pv1.z<<16);
//	tx0=(pv1.x<<12);	ty0=(pv1.y<<12);	tz0=(pv1.z<<12);
	tx0=pv1.x;		ty0=pv1.y;		tz0=pv1.z;
	v2_parm[TKRA_VX_XPOS]=tx0;
	v2_parm[TKRA_VX_YPOS]=ty0;
	v2_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

//	__debugbreak();

//	v3_parm[TKRA_VX_XPOS]=pv2.x<<16;
//	v3_parm[TKRA_VX_YPOS]=pv2.y<<16;
//	v3_parm[TKRA_VX_ZPOS]=pv2.z<<16;

//	tx0=(pv2.x<<16);	ty0=(pv2.y<<16);	tz0=(pv2.z<<16);
//	tx0=(pv2.x<<12);	ty0=(pv2.y<<12);	tz0=(pv2.z<<12);
	tx0=pv2.x;		ty0=pv2.y;		tz0=pv2.z;
	v3_parm[TKRA_VX_XPOS]=tx0;
	v3_parm[TKRA_VX_YPOS]=ty0;
	v3_parm[TKRA_VX_ZPOS]=((u32)tz0) | (((s64)tx0)<<32);

//	printf("pv0: %d %d %d\n", pv0.x, pv0.y, pv0.z);
//	printf("pv1: %d %d %d\n", pv1.x, pv1.y, pv1.z);
//	printf("pv2: %d %d %d\n", pv2.x, pv2.y, pv2.z);

//	v1_parm[TKRA_VX_TPOS]=(((u64)pv0.s)<<16) | (((u64)pv0.t)<<48);
//	v2_parm[TKRA_VX_TPOS]=(((u64)pv1.s)<<16) | (((u64)pv1.t)<<48);
//	v3_parm[TKRA_VX_TPOS]=(((u64)pv2.s)<<16) | (((u64)pv2.t)<<48);

//	v1_parm[TKRA_VX_TPOS]=(((u64)((u16)is0))<<16) | (((u64)((u16)it0))<<48);
//	v2_parm[TKRA_VX_TPOS]=(((u64)((u16)is1))<<16) | (((u64)((u16)it1))<<48);
//	v3_parm[TKRA_VX_TPOS]=(((u64)((u16)is2))<<16) | (((u64)((u16)it2))<<48);

//	v1_parm[TKRA_VX_TPOS]=(((u64)((u16)is0))<<12) | (((u64)((u16)it0))<<44);
//	v2_parm[TKRA_VX_TPOS]=(((u64)((u16)is1))<<12) | (((u64)((u16)it1))<<44);
//	v3_parm[TKRA_VX_TPOS]=(((u64)((u16)is2))<<12) | (((u64)((u16)it2))<<44);

//	is0<<=12;	is1<<=12;	is2<<=12;
//	it0<<=12;	it1<<=12;	it2<<=12;

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

int TKRA_EmitProjectedQuad(
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

	u64 tl0, tl1;
	int is0, is1, is2, is3, it0, it1, it2, it3, mip, mag;
	int ds0, ds1, ds2, ds3, dt0, dt1, dt2, dt3, dst;
	int dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3, dxy, mmip;
	int tx0, ty0, tz0, shx, shy, flipst;
	int clip_mx, clip_nx, clip_my, clip_ny;

	if(TKRA_CheckZCullQuad(ctx, pv0, pv1, pv2, pv3))
		return(-1);

	is0=pv0.s;	is1=pv1.s;	is2=pv2.s;	is3=pv3.s;
	it0=pv0.t;	it1=pv1.t;	it2=pv2.t;	it3=pv3.t;

	flipst=0;

	mmip=ctx->tex_nmip;
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
		
		mip=0;
		mag=dst<=dxy;

#if 1
		while((dst>dxy) && (mip<mmip))
			{ dst>>=1;	mip++; }
#endif

		if(ctx->tex_flag&TKRA_TRFL_FLIPST)
			flipst=1;

		if((mip>0) && (ctx->tex_flag&TKRA_TRFL_HASMIP))
		{
			is0>>=mip;	is1>>=mip;	is2>>=mip;	is3>>=mip;
			it0>>=mip;	it1>>=mip;	it2>>=mip;	it3>>=mip;
			mag=0;

			img=ctx->tex_cur;

			shx=img->tex_xshl-mip;
			shy=img->tex_yshl-mip;
			if(shx<0)shx=0;
			if(shy<0)shy=0;

			ctx->tex_img=img->tex_img+img->tex_mipofs[mip];
			ctx->tex_img_bcn=img->tex_img_bcn+img->tex_mipofs_bcn[mip];
			ctx->tex_xshl=shx;
			ctx->tex_yshl=shy;

#ifdef __BJX2__
			if(ctx->tex_img_bcn)
			{
				tl0=(u64)(ctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				ctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}else
		{
			img=ctx->tex_cur;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			ctx->tex_img=img->tex_img;
			ctx->tex_img_bcn=img->tex_img_bcn;
			ctx->tex_xshl=shx;
			ctx->tex_yshl=shy;

#ifdef __BJX2__
			if(ctx->tex_img_bcn)
			{
				tl0=(u64)(ctx->tex_img_bcn);
				tx0=((shx+shy)<<4)|0;
				tl1=((u64)tx0)<<48;
				tl0=(tl0&0x0000FFFFFFFFFFFFULL)|tl1;
				ctx->tex_img_bcn=(void *)tl0;
			}
#endif
		}
		
//		if(mip>0)
		if(!mag)
		{
			ctx->DrawSpan=ctx->DrawSpan_Min;
			ctx->DrawSpanHZt=ctx->DrawSpanHZt_Min;
			ctx->DrawSpanZt=ctx->DrawSpanZt_Min;
		}
		else
		{
			ctx->DrawSpan=ctx->DrawSpan_Mag;
			ctx->DrawSpanHZt=ctx->DrawSpanHZt_Mag;
			ctx->DrawSpanZt=ctx->DrawSpanZt_Mag;
		}
	}

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
//		TKRA_WalkTriangle(ctx, v1_parm, v2_parm, v3_parm);
//		TKRA_WalkTriangle(ctx, v1_parm, v3_parm, v4_parm);
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
	TKRA_TexImage *img;

	int is0, it0;
	int tx0, ty0, tz0, shx, shy;

	if(1)
	{		
		img=ctx->tex_cur;
		shx=img->tex_xshl;
		shy=img->tex_yshl;
		ctx->tex_img=img->tex_img;
		ctx->tex_img_bcn=img->tex_img_bcn;
		ctx->tex_xshl=shx;
		ctx->tex_yshl=shy;
		
		ctx->DrawSpan=ctx->DrawSpan_Min;
		ctx->DrawSpanHZt=ctx->DrawSpanHZt_Min;
		ctx->DrawSpanZt=ctx->DrawSpanZt_Min;
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

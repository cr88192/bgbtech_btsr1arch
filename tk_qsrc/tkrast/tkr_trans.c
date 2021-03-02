void TKRA_UnpackMatrix16fv(tkra_mat4 mat, float *fv);

// #ifdef __BJX2__
#ifdef BJX2_SIMD

#define tkra_v4fadd(a, b)	((a)+(b))
#define tkra_v4fsub(a, b)	((a)-(b))
#define tkra_v4fmul(a, b)	((a)*(b))

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
float		__vnf_v2f_dot(__vec2f a, __vec2f b);
#define tkra_v4fdot(a, b)	__vnf_v4f_dot(a, b)
#define tkra_v2fdot(a, b)	__vnf_v2f_dot(a, b)

// #define tkra_v4fdot(a, b)	((a)^(b))
// #define tkra_v2fdot(a, b)	((a)^(b))

float		__vnf_v2f_cross(__vec2f a, __vec2f b);
#define tkra_v2fcross(a, b)	__vnf_v2f_cross(a, b)

#define tkra_v4f_midpoint(a, b)		\
	(tkra_v4fmul(tkra_v4fadd(a, b), tkra_mkvec4f(0.5,0.5,0.5,0.5)))
#define tkra_v2f_midpoint(a, b)		\
	(tkra_v2fmul(tkra_v2fadd(a, b), tkra_mkvec2f(0.5,0.5)))


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
	SHAD	R5, -16, R6	|	FLDCH	R5, R16
							FLDCH	R6, R17
//	FSTCF	R16, R2
//	FSTCFH	R17, R2

	FSTCF	R16, R20
	FSTCF	R17, R21
//	FSTCF	R18, R22
//	FSTCF	R19, R23
	MOVLD	R21, R20, R2
//	MOVLD	R23, R22, R3

	RTSU

tkra_mkvec4sfv:
	MOV.Q	(R4), R5
	SHAD	R5, -16, R6	|	FLDCH	R5, R16
	SHAD	R5, -32, R7	|	FLDCH	R6, R17
	SHAD	R5, -48, R6	|	FLDCH	R7, R18
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
	SHAD	R5, -16, R6	|	FLDCH	R5, R16
	SHAD	R5, -32, R7	|	FLDCH	R6, R17
							FLDCH	R7, R18
							FLDCH	0x7C00, R19

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
	PMULX.F		R20, R22, R2
	MOVHD		R4, R4, R20		|	MOVHD	R4, R4, R21
	MOV.X		(R6, 32), R22
	PMULX.F		R20, R18, R16
	PADDX.F		R2, R16, R2
	MOVLD		R5, R5, R20		|	MOVLD	R5, R5, R21
	MOV.X		(R6, 48), R18
	PMULX.F		R20, R22, R16
	MOVHD		R5, R5, R20		|	MOVHD	R5, R5, R21
	PADDX.F		R2, R16, R2
	PMULX.F		R20, R18, R16
	PADDX.F		R2, R16, R2
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

int TKRA_EmitProjectedTriangle(
	TKRA_Context *ctx,
	tkra_projvertex pv0,
	tkra_projvertex pv1,
	tkra_projvertex pv2)
{
	u64	v1_parm[TKRA_VX_NPARM];
	u64	v2_parm[TKRA_VX_NPARM];
	u64	v3_parm[TKRA_VX_NPARM];
	TKRA_TexImage *img;

	int is0, is1, is2, it0, it1, it2, mip, mag;
	int ds0, ds1, ds2, dt0, dt1, dt2, dst;
	int dx0, dx1, dx2, dy0, dy1, dy2, dxy, mmip;
	int tx0, ty0, tz0, shx, shy;

	is0=pv0.s;	is1=pv1.s;	is2=pv2.s;
	it0=pv0.t;	it1=pv1.t;	it2=pv2.t;

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
		}else
		{
			img=ctx->tex_cur;
			shx=img->tex_xshl;
			shy=img->tex_yshl;
			ctx->tex_img=img->tex_img;
			ctx->tex_img_bcn=img->tex_img_bcn;
			ctx->tex_xshl=shx;
			ctx->tex_yshl=shy;
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

	v1_parm[TKRA_VX_TPOS]=((u64)(u32)is0) | (((u64)((u32)it0))<<32);
	v2_parm[TKRA_VX_TPOS]=((u64)(u32)is1) | (((u64)((u32)it1))<<32);
	v3_parm[TKRA_VX_TPOS]=((u64)(u32)is2) | (((u64)((u32)it2))<<32);
	
	v1_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv0.rgb);
	v2_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv1.rgb);
	v3_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv2.rgb);

	TKRA_WalkTriangle(ctx, v1_parm, v2_parm, v3_parm);
	return(0);
}

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

float	__fpu_frcp_s(float x);

tkra_vec4f		tkra_prj_xyzsc;
tkra_vec4f		tkra_prj_xyzbi;

#ifdef __BJX2__
// #if 0

tkra_vec4f TKRA_ProjectVertexB(tkra_vec4f vec, tkra_mat4 mat);

__asm {
TKRA_ProjectVertexB:
	MOV.X		(R6,  0), R22
	MOVLD		R4, R4, R20		|	MOVLD	R4, R4, R21
	MOV.X		(R6, 16), R18
	PMULX.F		R20, R22, R2
	MOVHD		R4, R4, R20		|	MOVHD	R4, R4, R21
	MOV.X		(R6, 32), R22
	PMULX.F		R20, R18, R16
	PADDX.F		R2, R16, R2
	MOVLD		R5, R5, R20		|	MOVLD	R5, R5, R21
	MOV.X		(R6, 48), R18
	PMULX.F		R20, R22, R16
	MOVHD		R5, R5, R20		|	MOVHD	R5, R5, R21
	PADDX.F		R2, R16, R2
	PMULX.F		R20, R18, R16
	PADDX.F		R2, R16, R18

	FLDCFH		R19, R4
//	FLDCH		0x1A00, R5
	FLDCH		0x0A00, R5
	FABS		R4, R4
	FADD		R5, R4

//	MOV			0x7FF, R5	| MOV	0x3FF, R6
	MOV			0x7FE, R5	| MOV	0x3FF, R6
	SHLD.Q		R5, 52, R5	| MOV	0x400, R3
	SHLD.Q		R6, 52, R6	| SUB	R5, R4, R5
	SHLD.Q		R3, 52, R3	| FMUL	R5, R4, R7
	SUB			R6, R7, R7
	ADD			R7, R5, R2

	FMUL		R2, R4, R6
	FSUB		R3, R6, R7
	FMUL		R7, R2

//	FMUL		R2, R4, R6
//	FSUB		R3, R6, R7
//	FMUL		R7, R2

//	MOV			tkra_prj_xyzsc, R22
//	MOV			tkra_prj_xyzbi, R20
//	MOV.X		(R22,0), R22
//	MOV.X		(R20,0), R20

	MOV.X		tkra_prj_xyzsc, R22
	MOV.X		tkra_prj_xyzbi, R20
	
//	BREAK
	
	MOV			0x3F800000, R5	|	FSTCF		R2, R1
	MOVLD		R1, R1, R16		|	MOVLD		R5, R1, R17
	PMULX.F		R16, R22, R16
	PMULX.F		R18, R16, R18
	PADDX.F		R18, R20, R2
	RTSU
};

#else
tkra_vec4f TKRA_ProjectVertexB(tkra_vec4f vec, tkra_mat4 mat)
{
	tkra_vec4f		prj_xyzsc, prj_xyzbi;
	tkra_vec4f		v0xyzw, v0ww, v0p;
	float			f0, f1;

	v0xyzw=TKRA_ProjectVertex(vec, mat);
	f0=tkra_v4f_w(v0xyzw);
	f1=tkra_frcpabs(f0);

//	prj_xyzsc=tkra_prj_xyzsc;
//	prj_xyzbi=tkra_prj_xyzbi;
	v0ww=tkra_mkvec4f(f1, f1, f1, 1.0);
//	if(f0<0)
//		v0ww=tkra_mkvec4f(-f1, -f1, -f1, -1.0);
	
//	v0ww=tkra_v4fmul(v0ww, prj_xyzsc);
	v0ww=tkra_v4fmul(v0ww, tkra_prj_xyzsc);
//	v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), prj_xyzbi);
	v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), tkra_prj_xyzbi);

	return(v0p);
}
#endif

int TKRA_TransformProjectTriangle(
	TKRA_Context *ctx,
	tkra_trivertex iv0,	tkra_trivertex iv1,
	tkra_trivertex iv2,
//	tkra_mat4 xform,
//	tkra_mat4 xproj)
	tkra_mat4 prjmat)
{
	tkra_trivertex v0stk[64];
	tkra_trivertex v1stk[64];
	tkra_trivertex v2stk[64];
	int vstkpos;

	tkra_projvertex pv0;
	tkra_projvertex pv1;
	tkra_projvertex pv2;
	tkra_trivertex	v0, v1, v2;
	tkra_trivertex	v3, v4, v5;
	tkra_vec4f		v0xyzw, v0ww, v0p;
	tkra_vec4f		v1xyzw, v1ww, v1p;
	tkra_vec4f		v2xyzw, v2ww, v2p;
	tkra_vec2f		v0dxy, v1dxy, v2dxy;
	tkra_vec4f		prj_xyzsc, prj_xyzbi;

	TKRA_TexImage *img;

	float			scr_xsc, scr_ysc;
	float			scr_xc, scr_yc;
	float			scr_clip_l, scr_clip_r;
	float			scr_clip_t, scr_clip_b;
	float			txs, tys, txsn1, tysn1, xbi, ybi;
	float			f0, f1, f2, f3, f4, fdx, fdy;
	float			w0, w1, w2;
	int				i0, i1, i2, tfl;
	int				ecfl;
	byte			wasfrag;
	
	TKRA_SetupDrawBlend(ctx);

	v0stk[0]=iv0;
	v1stk[0]=iv1;
	v2stk[0]=iv2;
	vstkpos=1;
	ctx->stat_base_tris++;
	
	tfl=0;

	if((iv0.rgb&iv1.rgb&iv2.rgb&0xF0000000)!=0xF0000000)
		tfl|=1;
	
//	ctx->triflag=tfl;
	TKRA_SetupDrawEdgeForTriFlag(ctx, tfl);
	tfl=ctx->triflag;
	
	if(tfl&TKRA_TRFL_NOCMOD)
	{
		v0stk[0].rgb=0xFFFFFFFFU;
		v1stk[0].rgb=0xFFFFFFFFU;
		v2stk[0].rgb=0xFFFFFFFFU;
	}

#if 0
	scr_xsc=ctx->scr_xscale;
	scr_ysc=ctx->scr_yscale;
	scr_xc=ctx->scr_xcenter;
	scr_yc=ctx->scr_ycenter;
	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 32767, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 16383, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 8192, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, -8192, 1);
	prj_xyzbi=tkra_mkvec4f(scr_xc, scr_yc, 0, 0);

	scr_clip_l=scr_xc-scr_xsc;
	scr_clip_r=scr_xc+scr_xsc;
	scr_clip_t=scr_yc+scr_ysc;
	scr_clip_b=scr_yc-scr_ysc;
#endif
	
	prj_xyzsc=ctx->prj_xyzsc;
	prj_xyzbi=ctx->prj_xyzbi;
	
	tkra_prj_xyzsc=prj_xyzsc;
	tkra_prj_xyzbi=prj_xyzbi;
//	*(tkra_vec4f *)(&tkra_prj_xyzsc)=prj_xyzsc;
//	*(tkra_vec4f *)(&tkra_prj_xyzbi)=prj_xyzbi;
	
//	__debugbreak();

	scr_clip_l=ctx->scr_clip_l;
	scr_clip_r=ctx->scr_clip_r;
	scr_clip_t=ctx->scr_clip_t;
	scr_clip_b=ctx->scr_clip_b;


//	txs=(1<<(ctx->tex_xshl));
//	tys=(1<<(ctx->tex_yshl));

	img=ctx->tex_cur;
	txs=1<<img->tex_xshl;
	tys=1<<img->tex_yshl;
	txsn1=txs-1;
	tysn1=tys-1;
//	xbi=0.5;
//	xbi=-0.5;
//	xbi=-1.0;
	xbi=0.0;
//	ybi=0.5;
//	ybi=-0.5;
	ybi=-1.0;
//	ybi=-2.0;
//	ybi= 0.0;
	wasfrag=0;

	while(vstkpos>0)
	{
		vstkpos--;
		v0=v0stk[vstkpos];
		v1=v1stk[vstkpos];
		v2=v2stk[vstkpos];

// #if 0
#if 1
		v0p=TKRA_ProjectVertexB(v0.xyz, prjmat);
		v1p=TKRA_ProjectVertexB(v1.xyz, prjmat);
		v2p=TKRA_ProjectVertexB(v2.xyz, prjmat);
#endif

#if 0
// #if 1
		v0xyzw=TKRA_ProjectVertex(v0.xyz, prjmat);
		v1xyzw=TKRA_ProjectVertex(v1.xyz, prjmat);
		v2xyzw=TKRA_ProjectVertex(v2.xyz, prjmat);
		
		w0=tkra_v4f_w(v0xyzw);
		w1=tkra_v4f_w(v1xyzw);
		w2=tkra_v4f_w(v2xyzw);
		f0=tkra_frcpabs(w0);
		f1=tkra_frcpabs(w1);
		f2=tkra_frcpabs(w2);

		v0ww=tkra_mkvec4f(f0, f0, f0, 1.0);
		v1ww=tkra_mkvec4f(f1, f1, f1, 1.0);
		v2ww=tkra_mkvec4f(f2, f2, f2, 1.0);
		v0ww=tkra_v4fmul(v0ww, prj_xyzsc);
		v1ww=tkra_v4fmul(v1ww, prj_xyzsc);
		v2ww=tkra_v4fmul(v2ww, prj_xyzsc);

		v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), prj_xyzbi);
		v1p=tkra_v4fadd(tkra_v4fmul(v1xyzw, v1ww), prj_xyzbi);
		v2p=tkra_v4fadd(tkra_v4fmul(v2xyzw, v2ww), prj_xyzbi);
#endif

		v0ww=tkra_v4f_bboxmins3(v0p, v1p, v2p);
		v1ww=tkra_v4f_bboxmaxs3(v0p, v1p, v2p);
		
		ecfl=0;

//		if(tkra_v4f_z(v0ww)<(-32767))
//			continue;
//		if(tkra_v4f_z(v1ww)>( 32767))
//			continue;

		if(tkra_v4f_w(v1ww)<0.0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_negw_tris++;
			continue;
		}

		if(tkra_v4f_z(v1ww)<0.0)
		{
//			if(tkra_v4f_w(v0p)<0.0)

//			if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
//				continue;

//			if(tkra_v4f_z(v1ww)<(-32760))
//				continue;
		}

		if(tkra_v4f_x(v0ww)>scr_clip_r)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_x(v1ww)<scr_clip_l)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v0ww)>scr_clip_t)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v1ww)<scr_clip_b)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

#if 1
//		if(tkra_v4f_z(v0ww)<0.0)
//			{ ecfl|=1; }

		if((tkra_v4f_z(v0ww)<0.0) && (tkra_v4f_z(v1ww)>0.0))
			{ ecfl|=1; }
		if(	(tkra_v4f_x(v1ww)>scr_clip_r) ||
			(tkra_v4f_x(v0ww)<scr_clip_l) )
				{ ecfl|=2; }
		if(	(tkra_v4f_y(v1ww)>scr_clip_t) ||
			(tkra_v4f_y(v0ww)<scr_clip_b) )
				{ ecfl|=4; }
#endif

		/* Check if triangle is below size limit. */
		v0dxy=tkra_v2fsub(tkra_v4f_xy(v1p), tkra_v4f_xy(v0p));
		v1dxy=tkra_v2fsub(tkra_v4f_xy(v2p), tkra_v4f_xy(v1p));
		v2dxy=tkra_v2fsub(tkra_v4f_xy(v0p), tkra_v4f_xy(v2p));
		f0=tkra_v2fdot(v0dxy, v0dxy);
		f1=tkra_v2fdot(v1dxy, v1dxy);
		f2=tkra_v2fdot(v2dxy, v2dxy);
		
//		f3=tkra_v2fcross(v0dxy, v1dxy);
//		if(f3<=0)
//		if(f3>0)
//			continue;

		f3=f0+f1+f2;

//		if(f3 < (5*5*3))
//		if(f3 < (4*4*3))
//		if(f3 < (3*3*3))
//		if(f3 < (2*2*3))
		if(f3 < (3*3))
		{
//			ctx->stat_reject_tris++;
//			ctx->stat_micro_tris++;
//			if(wasfrag&1)
//				ctx->stat_microfrag_tris++;
//			else
//				ctx->stat_microbase_tris++;

//			if(f3 < (0.5*3))
			if(f3 < (0.75*3))
			{
				ctx->stat_microbase_tris++;
				continue;				
			}

			if(f3 < (2*3))
//			if(f3 < (2*2*3))
			{
				f0=tkra_v4f_x(v0p)+tkra_v4f_x(v1p)+tkra_v4f_x(v2p);
				f1=tkra_v4f_y(v0p)+tkra_v4f_y(v1p)+tkra_v4f_y(v2p);
				f2=tkra_v4f_z(v0p)+tkra_v4f_z(v1p)+tkra_v4f_z(v2p);
				i0=f0*(65536*0.3333);
				i1=f1*(65536*0.3333);
				i2=f2*(65536*0.3333);
				pv0.x=i0;
				pv0.y=i1;
				pv0.z=i2;
//				f0=	tkra_v2f_x(v0.st)+
//					tkra_v2f_x(v1.st)+
//					tkra_v2f_x(v2.st);
//				f1=	tkra_v2f_y(v0.st)+
//					tkra_v2f_y(v1.st)+
//					tkra_v2f_y(v2.st);
//				f0=f0*0.3333;
//				f1=f1*0.3333;

				v0dxy=tkra_v2fadd(tkra_v2fadd(v0.st, v1.st), v2.st);
				f0=tkra_v2f_x(v0dxy)*0.3333;
				f1=tkra_v2f_y(v0dxy)*0.3333;

				pv0.s=(f0*txs+xbi)*65536;
				pv0.t=(f1*tys+ybi)*65536;
				pv0.rgb=v0.rgb;

				ctx->stat_drawpts1_tris++;
				TKRA_EmitProjectedPoint(ctx, pv0);
				continue;
			}

//			f0=	tkra_v2f_x(v0.st)+
//				tkra_v2f_x(v1.st)+
//				tkra_v2f_x(v2.st);
//			f1=	tkra_v2f_y(v0.st)+
//				tkra_v2f_y(v1.st)+
//				tkra_v2f_y(v2.st);
//			f0=f0*0.3333;
//			f1=f1*0.3333;
			v0dxy=tkra_v2fadd(tkra_v2fadd(v0.st, v1.st), v2.st);
			f0=tkra_v2f_x(v0dxy)*0.3333;
			f1=tkra_v2f_y(v0dxy)*0.3333;

			i0=tkra_v4f_x(v0p)*65536;
			i1=tkra_v4f_y(v0p)*65536;
			i2=tkra_v4f_z(v0p)*65536;
			pv0.x=i0;	pv0.y=i1;	pv0.z=i2;
//			pv0.s=(tkra_v2f_x(v0.st)*txs+xbi)*65536;
//			pv0.t=(tkra_v2f_y(v0.st)*tys+ybi)*65536;
			pv0.s=(f0*txs+xbi)*65536;
			pv0.t=(f1*tys+ybi)*65536;
			pv0.rgb=v0.rgb;

			i0=tkra_v4f_x(v1p)*65536;
			i1=tkra_v4f_y(v1p)*65536;
			i2=tkra_v4f_z(v1p)*65536;
			pv1.x=i0;	pv1.y=i1;	pv1.z=i2;
			i0=tkra_v4f_x(v2p)*65536;
			i1=tkra_v4f_y(v2p)*65536;
			i2=tkra_v4f_z(v2p)*65536;
			pv2.x=i0;	pv2.y=i1;	pv2.z=i2;
//			pv1.s=(tkra_v2f_x(v1.st)*txs+xbi)*65536;
//			pv1.t=(tkra_v2f_y(v1.st)*tys+ybi)*65536;
//			pv2.s=(tkra_v2f_x(v2.st)*txs+xbi)*65536;
//			pv2.t=(tkra_v2f_y(v2.st)*tys+ybi)*65536;
			pv1.s=pv0.s;		pv1.t=pv0.t;
			pv2.s=pv0.s;		pv2.t=pv0.t;

			pv1.rgb=v1.rgb;
			pv2.rgb=v2.rgb;

			ctx->stat_drawpts3_tris++;
			TKRA_EmitProjectedTrianglePts(ctx, pv0, pv1, pv2);
			continue;
		}

#if 0
		if((f0<1) | (f1<1) | (f2<1))
		{
			ctx->stat_reject_tris++;
//			ctx->stat_micro_tris++;
			if(wasfrag&1)
				ctx->stat_microfrag_tris++;
			else
				ctx->stat_microbase_tris++;
			continue;
		}
#endif

//		f4=(128*128*3);
//		f4=(96*96*3);


#if 1
		f4=(72*72*3);
//		f4=(64*64*3);
//		f4=(48*48*3);
//		if(ecfl)
//			f4=(32*32*3);

		if(ecfl&4)
//			f4=(32*32*3);
			f4=(48*48*3);

		if(ecfl&1)
			f4=(32*32*3);
//			f4=(24*24*3);
#endif
	
//		if(f3>(64*64*3))
//		if(f3>(32*32*3))
//		if(f3>(128*128*3))
		if(f3>f4)
//		if(0)
		{
//			if((vstkpos+5)>=32)
			if((vstkpos+5)>=48)
//			if((vstkpos+5)>=64)
//			if((vstkpos+5)>=16)
//			if((vstkpos+5)>=24)
			{
				printf("blown: Wn: %f %f %f\n", w0, w1, w2);
				printf("blown: Fn: %f %f %f\n", f0, f1, f2);
			
				TKRA_DumpMatrix(prjmat, "PrjMat");
				TKRA_DumpVec4(iv0.xyz, "Iv0");
				TKRA_DumpVec4(iv1.xyz, "Iv1");
				TKRA_DumpVec4(iv2.xyz, "Iv2");
			
				ctx->stat_blown_tris++;
				return(-1);
			}

			/* Subdivide Triangle */
			v3.xyz=tkra_v4f_midpoint(v0.xyz, v1.xyz);
			v4.xyz=tkra_v4f_midpoint(v1.xyz, v2.xyz);
			v5.xyz=tkra_v4f_midpoint(v2.xyz, v0.xyz);
			v3.st=tkra_v2f_midpoint(v0.st, v1.st);
			v4.st=tkra_v2f_midpoint(v1.st, v2.st);
			v5.st=tkra_v2f_midpoint(v2.st, v0.st);
			v3.rgb=tkra_rgba_midpoint(v0.rgb, v1.rgb);
			v4.rgb=tkra_rgba_midpoint(v1.rgb, v2.rgb);
			v5.rgb=tkra_rgba_midpoint(v2.rgb, v0.rgb);

			v0stk[vstkpos]=v0;
			v1stk[vstkpos]=v3;
			v2stk[vstkpos]=v5;
			vstkpos++;
			v0stk[vstkpos]=v1;
			v1stk[vstkpos]=v4;
			v2stk[vstkpos]=v3;
			vstkpos++;
			v0stk[vstkpos]=v2;
			v1stk[vstkpos]=v5;
			v2stk[vstkpos]=v4;
			vstkpos++;
			v0stk[vstkpos]=v3;
			v1stk[vstkpos]=v4;
			v2stk[vstkpos]=v5;
			vstkpos++;

			ctx->stat_frag_tris+=4;
			wasfrag|=1;

			continue;
		}

#if 1
		if(ctx->stateflag1&TKRA_STFL1_CULLFACE)
		{
			f3=tkra_v2fcross(v0dxy, v1dxy);
			if(ctx->stateflag1&TKRA_STFL1_CULL_CW)
				f3=-f3;
//			if(f3<=0)
//			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3>0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3<0))
			{
				ctx->stat_reject_tris++;
				continue;
			}
//			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3<=0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3>=0))
			{
				ctx->stat_reject_tris++;
				continue;
			}
		}
#endif

//		if(tkra_v4f_z(v0ww)<(-32767))
//			continue;
//		if(tkra_v4f_z(v1ww)>( 32767))
//			continue;


		ctx->stat_draw_tris++;

		i0=tkra_v4f_x(v0p)*65536;
		i1=tkra_v4f_y(v0p)*65536;
		i2=tkra_v4f_z(v0p)*65536;
		pv0.x=i0;	pv0.y=i1;	pv0.z=i2;
		
		i0=tkra_v4f_x(v1p)*65536;
		i1=tkra_v4f_y(v1p)*65536;
		i2=tkra_v4f_z(v1p)*65536;
		pv1.x=i0;	pv1.y=i1;	pv1.z=i2;

		i0=tkra_v4f_x(v2p)*65536;
		i1=tkra_v4f_y(v2p)*65536;
		i2=tkra_v4f_z(v2p)*65536;
		pv2.x=i0;	pv2.y=i1;	pv2.z=i2;
		
//		pv0.s=tkra_v2f_x(v0.st)*txs*65536-32767;
//		pv0.t=tkra_v2f_y(v0.st)*tys*65536-32767;
//		pv1.s=tkra_v2f_x(v1.st)*txs*65536-32767;
//		pv1.t=tkra_v2f_y(v1.st)*tys*65536-32767;
//		pv2.s=tkra_v2f_x(v2.st)*txs*65536-32767;
//		pv2.t=tkra_v2f_y(v2.st)*tys*65536-32767;

//		pv0.s=tkra_v2f_x(v0.st)*txs*65536;
//		pv0.t=tkra_v2f_y(v0.st)*tys*65536;
//		pv1.s=tkra_v2f_x(v1.st)*txs*65536;
//		pv1.t=tkra_v2f_y(v1.st)*tys*65536;
//		pv2.s=tkra_v2f_x(v2.st)*txs*65536;
//		pv2.t=tkra_v2f_y(v2.st)*tys*65536;

		pv0.s=(tkra_v2f_x(v0.st)*txs+xbi)*65536;
		pv0.t=(tkra_v2f_y(v0.st)*tys+ybi)*65536;
		pv1.s=(tkra_v2f_x(v1.st)*txs+xbi)*65536;
		pv1.t=(tkra_v2f_y(v1.st)*tys+ybi)*65536;
		pv2.s=(tkra_v2f_x(v2.st)*txs+xbi)*65536;
		pv2.t=(tkra_v2f_y(v2.st)*tys+ybi)*65536;

		pv0.rgb=v0.rgb;
		pv1.rgb=v1.rgb;
		pv2.rgb=v2.rgb;

		TKRA_EmitProjectedTriangle(ctx, pv0, pv1, pv2);
	}
	
	return(0);
}

int TKRA_DrawTriangleArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int cnt)
{
	tkra_trivertex	v0, v1, v2;
	byte *cxyz, *cst, *crgb;
	float *xyz0, *st0;
	u32 *rgb0;
	int i, j, k;
	
	cxyz=xyz;
	cst=st;
	crgb=rgb;
	for(i=0; i<cnt; i++)
	{
		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v0.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v0.st=tkra_mkvec2f(st0[0], st0[1]);
		v0.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v1.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v1.st=tkra_mkvec2f(st0[0], st0[1]);
		v1.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v2.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v2.st=tkra_mkvec2f(st0[0], st0[1]);
		v2.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;
		
		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
//		TKRA_TransformProjectTriangle(ctx, v0, v1, v2,
//			ctx->mat_xform, ctx->mat_xproj);
	}
	return(0);
}

int tkra_clamp255(int v)
{
	if(v<0)v=0;
	if(v>255)v=255;
	return(v);
}

int tkra_clamp127s(int v)
{
	if(v<(-127))v=(-127);
	if(v>( 127))v=( 127);
	return(v);
}

tkra_vec4f tkra_getptr_v4f_2fv01(void *ptr)
{
	float *pf;		pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], 0, 1));
}
tkra_vec4f tkra_getptr_v4f_2sfv01(void *ptr)
{
	tkra_vec2f xy;
	xy=tkra_mkvec2sfv(ptr);
	return(tkra_mkvec4f(tkra_v2f_x(xy), tkra_v2f_y(xy), 0, 1));
}
tkra_vec4f tkra_getptr_v4f_2dv01(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], 0, 1));
}

tkra_vec4f tkra_getptr_v4f_2ssv01(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0), 0.0, 1.0));
}

tkra_vec4f tkra_getptr_v4f_2siv01(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0), 0.0, 1.0));
}

#ifdef __BJX2__
tkra_vec4f tkra_getptr_v4f_3fv1(void *ptr);
tkra_vec4f tkra_getptr_v4f_4fv(void *ptr);
__asm {
tkra_getptr_v4f_3fv1:
	MOV.Q	(R4, 0), R2
	MOV.Q	(R4, 8), R3
	MOV		0x3F800000, R5
	MOVLD	R5, R3, R3
	RTS
tkra_getptr_v4f_4fv:
	MOV.Q	(R4, 0), R2
	MOV.Q	(R4, 8), R3
	RTS
};
#else
tkra_vec4f tkra_getptr_v4f_3fv1(void *ptr)
{
	float *pf;		pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], 1));
}
tkra_vec4f tkra_getptr_v4f_4fv(void *ptr)
	{ return(tkra_mkvec4fv(ptr)); }
#endif

tkra_vec4f tkra_getptr_v4f_3sfv1(void *ptr)
	{ return(tkra_mkvec3sfv1(ptr)); }
tkra_vec4f tkra_getptr_v4f_3dv1(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], 1));
}

tkra_vec4f tkra_getptr_v4f_3ssv1(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0),
		pf[2]*(1.0/32767.0), 1.0));
}

tkra_vec4f tkra_getptr_v4f_3siv1(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0),
		pf[2]*(1.0/2147483648.0), 1.0));
}

tkra_vec4f tkra_getptr_v4f_4sfv(void *ptr)
	{ return(tkra_mkvec4sfv(ptr)); }

tkra_vec4f tkra_getptr_v4f_4dv(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], pf[3]));
}

tkra_vec4f tkra_getptr_v4f_4ssv(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0),
		pf[2]*(1.0/32767.0), pf[3]*(1.0/32767.0)));
}

tkra_vec4f tkra_getptr_v4f_4siv(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0),
		pf[2]*(1.0/2147483648.0), pf[3]*(1.0/2147483648.0)));
}

tkra_vec4f tkra_getptr_v4f_null(void *ptr)
{
	return(tkra_mkvec4f(0.0, 0.0, 0.0, 1.0));
}


#ifdef __BJX2__
tkra_vec2f tkra_getptr_v2f_2fv(void *ptr);
__asm {
tkra_getptr_v2f_2fv:
	MOV.Q	(R4), R2
	RTS
};
#else
tkra_vec2f tkra_getptr_v2f_2fv(void *ptr)
	{ return(tkra_mkvec2fv(ptr)); }
#endif

tkra_vec2f tkra_getptr_v2f_2sfv(void *ptr)
	{ return(tkra_mkvec2sfv(ptr)); }
tkra_vec2f tkra_getptr_v2f_2dv(void *ptr)
{
	double *pf;
	pf=ptr;
	return(tkra_mkvec2f(pf[0], pf[1]));
}

tkra_vec2f tkra_getptr_v2f_2ssv(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec2f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0)));
}

tkra_vec2f tkra_getptr_v2f_2siv(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec2f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0)));
}

tkra_vec2f tkra_getptr_v2f_null(void *ptr)
{
	return(tkra_mkvec2f(0, 0));
}

u32 tkra_getptr_rgba_3sbv(void *ptr)
	{ return(0xFF000000|(*(u32 *)ptr)); }
u32 tkra_getptr_rgba_4sbv(void *ptr)
	{ return(*(u32 *)ptr); }

u32 tkra_getptr_rgba_3ubv(void *ptr)
{
	u32 c;
	c=0xFF000000|(*(u32 *)ptr);
	c=	((c    )&0xFF00FF00) |
		((c>>16)&0x000000FF) |
		((c<<16)&0x00FF0000) ;
	return(c);
//	return(0xFF000000|(*(u32 *)ptr));
}

#ifdef __BJX2__
u32 tkra_getptr_rgba_4ubv(void *ptr);
__asm {
tkra_getptr_rgba_4ubv:
	MOVU.L	(R4), R2
	PSHUF.B R2, 0xC6, R2
	RTS
};
#else
u32 tkra_getptr_rgba_4ubv(void *ptr)
{
	u32 c;
	c=*(u32 *)ptr;
	c=	((c    )&0xFF00FF00) |
		((c>>16)&0x000000FF) |
		((c<<16)&0x00FF0000) ;
	return(c);
}
#endif

u32 tkra_getptr_rgba_3fv(void *ptr)
{
	int cr, cg, cb;
	float *pf;
	pf=ptr;
	cr=pf[0]*255;	cg=pf[1]*255;	cb=pf[2]*255;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4fv(void *ptr)
{
	int cr, cg, cb, ca;
	float *pf;
	pf=ptr;
	cr=pf[0]*255;	cg=pf[1]*255;	cb=pf[2]*255;	ca=pf[3]*255;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3ssv(void *ptr)
{
	int cr, cg, cb;		s16 *pf;	pf=ptr;
	cr=pf[0]/127;	cg=pf[1]/127;	cb=pf[2]/127;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4ssv(void *ptr)
{
	int cr, cg, cb, ca;		s16 *pf;	pf=ptr;
	cr=pf[0]/127;	cg=pf[1]/127;
	cb=pf[2]/127;	ca=pf[3]/127;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3usv(void *ptr)
{
	int cr, cg, cb;		u16 *pf;	pf=ptr;
	cr=pf[0]>>8;	cg=pf[1]>>8;	cb=pf[2]>>8;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4usv(void *ptr)
{
	int cr, cg, cb, ca;		u16 *pf;	pf=ptr;
	cr=pf[0]>>8;	cg=pf[1]>>8;	cb=pf[2]>>8;	ca=pf[3]>>8;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3siv(void *ptr)
{
	int cr, cg, cb;		s32 *pf;	pf=ptr;
	cr=pf[0]>>23;	cg=pf[1]>>23;	cb=pf[2]>>23;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4siv(void *ptr)
{
	int cr, cg, cb, ca;		s32 *pf;	pf=ptr;
	cr=pf[0]>>23;	cg=pf[1]>>23;	cb=pf[2]>>23;	ca=pf[3]>>23;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3uiv(void *ptr)
{
	int cr, cg, cb;		u32 *pf;	pf=ptr;
	cr=pf[0]>>24;	cg=pf[1]>>24;	cb=pf[2]>>24;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4uiv(void *ptr)
{
	int cr, cg, cb, ca;		u32 *pf;	pf=ptr;
	cr=pf[0]>>24;	cg=pf[1]>>24;	cb=pf[2]>>24;	ca=pf[3]>>24;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_null(void *ptr)
{
	return(0xFFFFFFFFU);
}

#ifdef __BJX2__
int tkra_getptr_int_ubv(void *ptr);
int tkra_getptr_int_usv(void *ptr);
int tkra_getptr_int_uiv(void *ptr);
int tkra_getptr_int_null(void *ptr);
__asm {
tkra_getptr_int_ubv:
	MOVU.B	(R4), R2
	RTS
tkra_getptr_int_usv:
	MOVU.W	(R4), R2
	RTS
tkra_getptr_int_uiv:
	MOVU.L	(R4), R2
	RTS
tkra_getptr_int_null:
	MOV		R4, R2
	RTS
};
#else
int tkra_getptr_int_ubv(void *ptr)
	{ return(*(byte *)ptr); }
int tkra_getptr_int_usv(void *ptr)
	{ return(*(u16 *)ptr); }
int tkra_getptr_int_uiv(void *ptr)
	{ return(*(u32 *)ptr); }
int tkra_getptr_int_null(void *ptr)
	{ return((nlint)ptr); }
#endif

void *TKRA_PrimitiveGetGetPtrXYZ(int xyz_n, int xyz_ty)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);

	switch(xyz_n)
	{
	case 2:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_2fv01;	break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_2dv01;	break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_2sfv01;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_2ssv01;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_2siv01;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;	break;
		}
		break;
	case 3:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_3fv1;	break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_3dv1;	break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_3sfv1;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_3ssv1;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_3siv1;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;	break;
		}
		break;
	case 4:
	default:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_4fv;		break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_4dv;		break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_4sfv;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_4ssv;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_4siv;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;		break;
		}
		break;
	}
	
	return(getptr_xyz);
}

void *TKRA_PrimitiveGetGetPtrST(int st_n, int st_ty)
{
	tkra_vec2f	(*getptr_st)(void *ptr);

	switch(st_ty)
	{
	case TKRA_FLOAT:	getptr_st=tkra_getptr_v2f_2fv;		break;
	case TKRA_DOUBLE:	getptr_st=tkra_getptr_v2f_2dv;		break;
	case TKRA_HFLOAT:	getptr_st=tkra_getptr_v2f_2sfv;		break;
	case TKRA_SHORT:	getptr_st=tkra_getptr_v2f_2ssv;		break;
	case TKRA_INT:		getptr_st=tkra_getptr_v2f_2siv;		break;
	default:			getptr_st=tkra_getptr_v2f_null;		break;
	}
	
	return(getptr_st);
}

void *TKRA_PrimitiveGetGetPtrRGB(int rgb_n, int rgb_ty)
{
	u32			(*getptr_rgb)(void *ptr);

	switch(rgb_n)
	{
	case 3:
		switch(rgb_ty)
		{
		case TKRA_BYTE:		getptr_rgb=tkra_getptr_rgba_3sbv;	break;
		case TKRA_UBYTE:	getptr_rgb=tkra_getptr_rgba_3ubv;	break;
		case TKRA_SHORT:	getptr_rgb=tkra_getptr_rgba_3ssv;	break;
		case TKRA_USHORT:	getptr_rgb=tkra_getptr_rgba_3usv;	break;
		case TKRA_FLOAT:	getptr_rgb=tkra_getptr_rgba_3fv;	break;
		default:			getptr_rgb=tkra_getptr_rgba_null;	break;
		}
		break;
	case 4:
	default:
		switch(rgb_ty)
		{
		case TKRA_BYTE:		getptr_rgb=tkra_getptr_rgba_4sbv;	break;
		case TKRA_UBYTE:	getptr_rgb=tkra_getptr_rgba_4ubv;	break;
		case TKRA_SHORT:	getptr_rgb=tkra_getptr_rgba_4ssv;	break;
		case TKRA_USHORT:	getptr_rgb=tkra_getptr_rgba_4usv;	break;
		case TKRA_FLOAT:	getptr_rgb=tkra_getptr_rgba_4fv;	break;
		default:			getptr_rgb=tkra_getptr_rgba_null;	break;
		}
		break;
	}
	
	return(getptr_rgb);
}

void *TKRA_PrimitiveGetGetPtrIndex(int idx_ty, int *rstr)
{
	int			(*getptr_idx)(void *ptr);
	int idx_str;

	switch(idx_ty)
	{
	case	TKRA_BYTE:	case	TKRA_UBYTE:
		getptr_idx=tkra_getptr_int_ubv; idx_str=1; break;
	case	TKRA_SHORT:	case	TKRA_USHORT:
		getptr_idx=tkra_getptr_int_usv; idx_str=2; break;
	case	TKRA_INT:	case	TKRA_UINT:
		getptr_idx=tkra_getptr_int_uiv; idx_str=4; break;
	default:
		getptr_idx=tkra_getptr_int_null; idx_str=1; break;
	}
	
	*rstr=idx_str;
	return(getptr_idx);
}

int TKRA_DrawPrimitive_TriangleComm(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int mode,		int count)
{
	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb;
	int idx_str;
	int i, j, k, cnt, ix;

	cxyz=xyz;
	cst=st;
	crgb=rgb;

	cnt=count/3;
	for(i=0; i<cnt; i++)
	{
		v0.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v0.st=tkra_getptr_v2f_2fv(cst);
		v0.st=*(tkra_vec2f *)cst;
		v0.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v0.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		v1.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v1.st=tkra_getptr_v2f_2fv(cst);
		v1.st=*(tkra_vec2f *)cst;
		v1.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v1.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v2.st=tkra_getptr_v2f_2fv(cst);
		v2.st=*(tkra_vec2f *)cst;
		v2.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v2.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
	}
	return(0);
}

int TKRA_DrawPrimitive_PolyComm(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int mode,		int count)
{
	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb;
	int idx_str;
	int i, j, k, cnt, ix;

	cxyz=xyz;
	cst=st;
	crgb=rgb;

	v0.xyz=tkra_getptr_v4f_3fv1(cxyz);
//	v0.st=tkra_getptr_v2f_2fv(cst);
	v0.st=*(tkra_vec2f *)cst;
	v0.rgb=tkra_getptr_rgba_4ubv(crgb);
//	v0.rgb=*(u32 *)crgb;
	cxyz+=xyz_str;
	cst+=st_str;
	crgb+=rgb_str;

	v1.xyz=tkra_getptr_v4f_3fv1(cxyz);
//	v1.st=tkra_getptr_v2f_2fv(cst);
	v1.st=*(tkra_vec2f *)cst;
	v1.rgb=tkra_getptr_rgba_4ubv(crgb);
//	v1.rgb=*(u32 *)crgb;
	cxyz+=xyz_str;
	cst+=st_str;
	crgb+=rgb_str;


	cnt=count-2;
	for(i=0; i<cnt; i++)
	{		
		v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v2.st=tkra_getptr_v2f_2fv(cst);
		v2.st=*(tkra_vec2f *)cst;
		v2.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v2.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
		v1=v2;
	}

	return(0);
}

int TKRA_DrawPrimitiveIndexArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_n,		int xyz_ty,		int xyz_str,
	void *st,		int st_n,		int st_ty,		int st_str,
	void *rgb,		int rgb_n,		int rgb_ty,		int rgb_str,
	void *idx,						int idx_ty,
	int mode,		int count)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);
	tkra_vec2f	(*getptr_st )(void *ptr);
	u32			(*getptr_rgb)(void *ptr);
	int			(*getptr_idx)(void *ptr);

	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb, *cix;
	int idx_str;
	int i, j, k, cnt, ix;
	
//	return;
	
	if(!xyz)xyz_ty=0;
	if(!st )st_ty=0;
	if(!rgb)rgb_ty=0;
	if(!idx)idx_ty=0;

#if 1
	if(	(xyz_ty==TKRA_FLOAT) &&
		(st_ty==TKRA_FLOAT) &&
		(rgb_ty==TKRA_UBYTE) &&
		(idx_ty==0) &&
		(xyz_n==3) &&
		(st_n==2) &&
		(rgb_n==4))
	{
		if(mode==TKRA_TRIANGLES)
		{
			TKRA_DrawPrimitive_TriangleComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}

		if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
		{
			TKRA_DrawPrimitive_PolyComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}
	}
#endif
	
	getptr_xyz=TKRA_PrimitiveGetGetPtrXYZ(xyz_n, xyz_ty);
	getptr_st =TKRA_PrimitiveGetGetPtrST(st_n, st_ty);
	getptr_rgb=TKRA_PrimitiveGetGetPtrRGB(rgb_n, rgb_ty);
	getptr_idx=TKRA_PrimitiveGetGetPtrIndex(idx_ty, &idx_str);

	cxyz=xyz;
	cst=st;
	crgb=rgb;
	cix=idx;

	if(mode==TKRA_TRIANGLES)
	{
		cnt=count/3;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*3+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*3+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*3+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
		}
		return(0);
	}

	if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_TRIANGLE_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			if(i&1)
			{
				TKRA_TransformProjectTriangle(ctx,
					v2, v1, v0, ctx->mat_tproj);
			}else
			{
				TKRA_TransformProjectTriangle(ctx,
					v0, v1, v2, ctx->mat_tproj);
			}
			
			v0=v1;
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_QUADS)
	{
		cnt=count>>2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*4+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
			TKRA_TransformProjectTriangle(ctx, v0, v2, v3, ctx->mat_tproj);
		}
		return(0);
	}

	if(mode==TKRA_QUAD_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

		cnt=(count-2)/2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*2+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*2+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v2, v1, ctx->mat_tproj);
			TKRA_TransformProjectTriangle(ctx, v1, v2, v3, ctx->mat_tproj);
			
			v0=v2;
			v1=v3;
		}
		return(0);
	}

	if(mode==TKRA_POINTS)
	{
		return(0);
	}

	if(	(mode==TKRA_LINES) ||
		(mode==TKRA_LINE_LOOP) ||
		(mode==TKRA_LINE_STRIP))
	{
		return(0);
	}

	return(-1);
}


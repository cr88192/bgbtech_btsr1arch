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

#define	tkra_mkvec4f(x, y, z, w)	((__vec4f) { x, y, z, w })
#define	tkra_mkvec2f(x, y)			((__vec2f) { x, y })

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

tkra_vec2f tkra_v2fmul(tkra_vec2f a, tkra_vec2f b)
{
	tkra_vec2f c;
	c.x=a.x*b.x;	c.y=a.y*b.y;
	return(c);
}

#define tkra_v4f_x(a)	(a.x)
#define tkra_v4f_y(a)	(a.y)
#define tkra_v4f_z(a)	(a.z)
#define tkra_v4f_w(a)	(a.w)

#define tkra_v2f_x(a)	(a.x)
#define tkra_v2f_y(a)	(a.y)

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
	RTSU
tkra_rgba_expand64:
	RGB32UPCK64		R4, R2
	RTSU
};

#else
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

	printf("%p   %08X %08X %08X %08X\n", &mata,
		((u32 *)(&mata))[0], ((u32 *)(&mata))[1],
		((u32 *)(&mata))[2], ((u32 *)(&mata))[3]);

	matbt=TKRA_MatrixTranspose(matb);

//	__debugbreak();

	TKRA_UnpackMatrix16fv(matbt, xyz0);
	printf("TKRA_MatrixMultiply: matbt\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);

#if 1
	printf("%p   %08X %08X %08X %08X\n", &mata,
		((u32 *)(&mata))[0], ((u32 *)(&mata))[1],
		((u32 *)(&mata))[2], ((u32 *)(&mata))[3]);

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

//	__debugbreak();

//	printf("pv0: %p  %d %d %d\n", &pv0, pv0.x, pv0.y, pv0.z);
//	printf("pv1: %p  %d %d %d\n", &pv1, pv1.x, pv1.y, pv1.z);
//	printf("pv2: %p  %d %d %d\n", &pv2, pv2.x, pv2.y, pv2.z);

	v1_parm[TKRA_VX_XPOS]=pv0.x<<16;
	v1_parm[TKRA_VX_YPOS]=pv0.y<<16;
	v1_parm[TKRA_VX_ZPOS]=pv0.z<<16;

	v2_parm[TKRA_VX_XPOS]=pv1.x<<16;
	v2_parm[TKRA_VX_YPOS]=pv1.y<<16;
	v2_parm[TKRA_VX_ZPOS]=pv1.z<<16;

//	__debugbreak();

	v3_parm[TKRA_VX_XPOS]=pv2.x<<16;
	v3_parm[TKRA_VX_YPOS]=pv2.y<<16;
	v3_parm[TKRA_VX_ZPOS]=pv2.z<<16;

//	printf("pv0: %d %d %d\n", pv0.x, pv0.y, pv0.z);
//	printf("pv1: %d %d %d\n", pv1.x, pv1.y, pv1.z);
//	printf("pv2: %d %d %d\n", pv2.x, pv2.y, pv2.z);


	v1_parm[TKRA_VX_TPOS]=(((u64)pv0.s)<<16) | (((u64)pv0.t)<<48);
	v2_parm[TKRA_VX_TPOS]=(((u64)pv1.s)<<16) | (((u64)pv1.t)<<48);
	v3_parm[TKRA_VX_TPOS]=(((u64)pv2.s)<<16) | (((u64)pv2.t)<<48);
	
	v1_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv0.rgb);
	v2_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv1.rgb);
	v3_parm[TKRA_VX_CPOS]=tkra_rgba_expand64(pv2.rgb);

	TKRA_WalkTriangle(ctx, v1_parm, v2_parm, v3_parm);
	return(0);
}


int TKRA_TransformProjectTriangle(
	TKRA_Context *ctx,
	tkra_trivertex iv0,	tkra_trivertex iv1,
	tkra_trivertex iv2,
//	tkra_mat4 xform,
//	tkra_mat4 xproj)
	tkra_mat4 prjmat)
{
	tkra_trivertex v0stk[32];
	tkra_trivertex v1stk[32];
	tkra_trivertex v2stk[32];
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
	float			scr_xsc, scr_ysc;
	float			scr_xc, scr_yc;
	float			txs, tys;
	tkra_vec4f		prj_xyzsc, prj_xyzbi;
	float			f0, f1, f2, f3, fdx, fdy;
	int				i0, i1, i2;
	
	v0stk[0]=iv0;
	v1stk[0]=iv1;
	v2stk[0]=iv2;
	vstkpos=1;

	scr_xsc=ctx->scr_xscale;
	scr_ysc=ctx->scr_yscale;
	scr_xc=ctx->scr_xcenter;
	scr_yc=ctx->scr_ycenter;
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 32767, 1);
	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, 8192, 1);
//	prj_xyzsc=tkra_mkvec4f(scr_xsc, scr_ysc, -8192, 1);
	prj_xyzbi=tkra_mkvec4f(scr_xc, scr_yc, 0, 0);
	
	txs=(1<<(ctx->tex_xshl));
	tys=(1<<(ctx->tex_yshl));
	
	while(vstkpos>0)
	{
		vstkpos--;
		v0=v0stk[vstkpos];
		v1=v1stk[vstkpos];
		v2=v2stk[vstkpos];

//		v0xyzw=TKRA_ProjectVertex(v0.xyz, xform);
//		v1xyzw=TKRA_ProjectVertex(v1.xyz, xform);
//		v2xyzw=TKRA_ProjectVertex(v2.xyz, xform);
//		v0xyzw=TKRA_ProjectVertex(v0xyzw, xproj);
//		v1xyzw=TKRA_ProjectVertex(v1xyzw, xproj);
//		v2xyzw=TKRA_ProjectVertex(v2xyzw, xproj);

		v0xyzw=TKRA_ProjectVertex(v0.xyz, prjmat);
		v1xyzw=TKRA_ProjectVertex(v1.xyz, prjmat);
		v2xyzw=TKRA_ProjectVertex(v2.xyz, prjmat);

//		f0=1.0/tkra_v4f_w(v0xyzw);
//		f1=1.0/tkra_v4f_w(v1xyzw);
//		f2=1.0/tkra_v4f_w(v2xyzw);
//		f0=1.0/fabs(tkra_v4f_w(v0xyzw));
//		f1=1.0/fabs(tkra_v4f_w(v1xyzw));
//		f2=1.0/fabs(tkra_v4f_w(v2xyzw));
		
		f0=tkra_v4f_w(v0xyzw);
		f1=tkra_v4f_w(v1xyzw);
		f2=tkra_v4f_w(v2xyzw);
		f0=1.0/(fabs(f0)+0.25);
		f1=1.0/(fabs(f1)+0.25);
		f2=1.0/(fabs(f2)+0.25);
//		f0=1.0/(fabs(f0)+0.10);
//		f1=1.0/(fabs(f1)+0.10);
//		f2=1.0/(fabs(f2)+0.10);
		
		v0ww=tkra_mkvec4f(f0, f0, f0, 1.0);
		v1ww=tkra_mkvec4f(f1, f1, f1, 1.0);
		v2ww=tkra_mkvec4f(f2, f2, f2, 1.0);
		v0ww=tkra_v4fmul(v0ww, prj_xyzsc);
		v1ww=tkra_v4fmul(v1ww, prj_xyzsc);
		v2ww=tkra_v4fmul(v2ww, prj_xyzsc);

//		TKRA_DumpVec4(v0ww, "TKRA_TransformProjectTriangle: v0ww");
//		TKRA_DumpVec4(v1ww, "TKRA_TransformProjectTriangle: v1ww");
//		TKRA_DumpVec4(v2ww, "TKRA_TransformProjectTriangle: v2ww");
		
		v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), prj_xyzbi);
		v1p=tkra_v4fadd(tkra_v4fmul(v1xyzw, v1ww), prj_xyzbi);
		v2p=tkra_v4fadd(tkra_v4fmul(v2xyzw, v2ww), prj_xyzbi);

//		TKRA_DumpVec4(v0p, "TKRA_TransformProjectTriangle: v0p");
//		TKRA_DumpVec4(v1p, "TKRA_TransformProjectTriangle: v1p");
//		TKRA_DumpVec4(v2p, "TKRA_TransformProjectTriangle: v2p");
		
		/* Check if triangle is below size limit. */
//		f0=tkra_v4f_dist_xy(v0p, v1p);
//		f1=tkra_v4f_dist_xy(v1p, v2p);
//		f2=tkra_v4f_dist_xy(v2p, v0p);

		v0dxy=tkra_v2fsub(tkra_v4f_xy(v1p), tkra_v4f_xy(v0p));
		v1dxy=tkra_v2fsub(tkra_v4f_xy(v2p), tkra_v4f_xy(v1p));
		v2dxy=tkra_v2fsub(tkra_v4f_xy(v0p), tkra_v4f_xy(v2p));
		f0=tkra_v2fdot(v0dxy, v0dxy);
		f1=tkra_v2fdot(v1dxy, v1dxy);
		f2=tkra_v2fdot(v2dxy, v2dxy);
		
		f3=tkra_v2fcross(v0dxy, v1dxy);
		if(f3<=0)
			continue;

		f3=f0+f1+f2;
		if(f3>(64*64*3))
//		if(f3>(32*32*3))
//		if(f3>(128*128*3))
//		if(0)
		{
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

			if((vstkpos+4)>=32)
				continue;

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

			continue;
		}
		
//		pv0.x=tkra_v4f_x(v0p);	pv0.y=tkra_v4f_y(v0p);	pv0.z=tkra_v4f_z(v0p);
//		pv1.x=tkra_v4f_x(v1p);	pv1.y=tkra_v4f_y(v1p);	pv1.z=tkra_v4f_z(v1p);
//		pv2.x=tkra_v4f_x(v2p);	pv2.y=tkra_v4f_y(v2p);	pv2.z=tkra_v4f_z(v2p);

		i0=tkra_v4f_x(v0p);	i1=tkra_v4f_y(v0p);	i2=tkra_v4f_z(v0p);
		pv0.x=i0;	pv0.y=i1;	pv0.z=i2;
//		printf("pv0: %d %d %d\n", i0, i1, i2);
		
		i0=tkra_v4f_x(v1p);	i1=tkra_v4f_y(v1p);	i2=tkra_v4f_z(v1p);
		pv1.x=i0;	pv1.y=i1;	pv1.z=i2;
//		printf("pv1: %d %d %d\n", i0, i1, i2);

		i0=tkra_v4f_x(v2p);	i1=tkra_v4f_y(v2p);	i2=tkra_v4f_z(v2p);
		pv2.x=i0;	pv2.y=i1;	pv2.z=i2;
//		printf("pv2: %d %d %d\n", i0, i1, i2);
		
		pv0.s=tkra_v2f_x(v0.st)*txs;	pv0.t=tkra_v2f_y(v0.st)*tys;
		pv1.s=tkra_v2f_x(v1.st)*txs;	pv1.t=tkra_v2f_y(v1.st)*tys;
		pv2.s=tkra_v2f_x(v2.st)*txs;	pv2.t=tkra_v2f_y(v2.st)*tys;
		
		pv0.rgb=v0.rgb;
		pv1.rgb=v1.rgb;
		pv2.rgb=v2.rgb;

//		printf("pv0: %p  %d %d %d\n", &pv0, pv0.x, pv0.y, pv0.z);
//		printf("pv1: %p  %d %d %d\n", &pv1, pv1.x, pv1.y, pv1.z);
//		printf("pv2: %p  %d %d %d\n", &pv2, pv2.x, pv2.y, pv2.z);
		
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


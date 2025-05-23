/*
Rasterizer State

Will use 16-bit RGB555 pixels as both source and destination.
*/


#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

#ifndef UINT_T
#define UINT_T
typedef unsigned int uint;
#endif

#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif

#if defined(__BJX2__)
#ifndef BJX2
#define BJX2
#endif
#endif

#ifdef BJX2
#define BJX2_SIMD
#define no_cull		__declspec(nocull)
#endif


#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM) || defined(BJX2)
#define LITTLEENDIAN
#endif
#endif

#if defined(X86_64) || defined(BJX2)
#define ARCH_IS64BIT
#endif


#ifdef ARCH_IS64BIT
typedef s64 nlint;
#else
typedef u32 nlint;
#endif


#if defined(_MSC_VER) && defined(BTEIFGL_DLL)
#ifndef BTRA_API
#define BTRA_API __declspec(dllexport)
#endif
#endif

#if defined(_MSC_VER) && defined(BTRA_DLL)
#ifndef BTRA_API
#define BTRA_API __declspec(dllexport)
#endif
#endif

#ifndef BTRA_API
#define BTRA_API
#endif

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifndef no_cull
#define no_cull
#endif

// #ifdef __GNUC__
// #define force_inline inline
// #define default_inline inline
// #endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_TAU
#define M_TAU 6.283185307179586476925286766559
#endif

#define BTRA_TWOCC(a, b)			((a)|((b)<<8))
#define BTRA_FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define		TKRA_CHEAP_BILIN

// #define		TKRA_PARAM_TRISUBDIV_DFL	(64*64*3)
// #define		TKRA_PARAM_TRISUBDIV_EDGE	(32*32*3)
// #define		TKRA_PARAM_TRISUBDIV_NEAR	(24*24*3)

// #define		TKRA_PARAM_TRISUBDIV_DFL	(64*64*3)
#define		TKRA_PARAM_TRISUBDIV_DFL	(48*48*3)
// #define		TKRA_PARAM_TRISUBDIV_DFL	(36*36*3)

// #define		TKRA_PARAM_TRISUBDIV_EDGE	(40*40*3)
// #define		TKRA_PARAM_TRISUBDIV_EDGE	(32*32*3)
#define		TKRA_PARAM_TRISUBDIV_EDGE	(24*24*3)

// #define		TKRA_PARAM_TRISUBDIV_NEAR	(24*24*3)
// #define		TKRA_PARAM_TRISUBDIV_NEAR	(16*16*3)
#define		TKRA_PARAM_TRISUBDIV_NEAR	(12*12*3)

// #define		TKRA_PARAM_QUADSUBDIV_DFL	(48*48*4)
// #define		TKRA_PARAM_QUADSUBDIV_EDGE	(40*40*4)
// #define		TKRA_PARAM_QUADSUBDIV_NEAR	(24*24*4)

// #define		TKRA_PARAM_QUADSUBDIV_DFL	(72*72*4)
// #define		TKRA_PARAM_QUADSUBDIV_DFL	(64*64*4)
#define		TKRA_PARAM_QUADSUBDIV_DFL	(60*60*4)
// #define		TKRA_PARAM_QUADSUBDIV_DFL	(56*56*4)
// #define		TKRA_PARAM_QUADSUBDIV_DFL	(40*40*4)

// #define		TKRA_PARAM_QUADSUBDIV_EDGE	(40*40*4)
#define		TKRA_PARAM_QUADSUBDIV_EDGE	(32*32*4)

// #define		TKRA_PARAM_QUADSUBDIV_NEAR	(24*24*4)
// #define		TKRA_PARAM_QUADSUBDIV_NEAR	(16*16*4)
#define		TKRA_PARAM_QUADSUBDIV_NEAR	(12*12*4)

/*
DrawSpan Parameter Array
*/
#define		TKRA_DS_CTX		0			//context
#define		TKRA_DS_TEXIMG	1			//texture image
#define		TKRA_DS_TPOS	2			//texture position
#define		TKRA_DS_TSTEP	3			//texture step
#define		TKRA_DS_XMASK	4			//texture position
#define		TKRA_DS_YMASK	5			//texture step
#define		TKRA_DS_CPOS	6			//color position
#define		TKRA_DS_CSTEP	7			//color step
#define		TKRA_DS_ZPOS	8			//color position
#define		TKRA_DS_ZSTEP	9			//color step
#define		TKRA_DS_TEXBCN	10			//texture image (BCn)
#define		TKRA_DS_BLEND	11			//Blend Function
#define		TKRA_DS_ZATEST	12			//Depth+Alpha Test
#define		TKRA_DS_NPARM	16			//number of drawspan params

/*
Edge Parameter Arrays
 */
#define		TKRA_ES_XPOS	0			//screen X position
#define		TKRA_ES_XSTEP	1			//screen X step
#define		TKRA_ES_TPOS	2			//texture position
#define		TKRA_ES_TSTEP	3			//texture step
#define		TKRA_ES_CPOS	4			//color position
#define		TKRA_ES_CSTEP	5			//color step
#define		TKRA_ES_ZPOS	6			//Z position
#define		TKRA_ES_ZSTEP	7			//Z step
// #define		TKRA_ES_NPARM	8			//number of edge params

#define		TKRA_ES_T2POS	8			//texture position
#define		TKRA_ES_T2STEP	9			//texture step

#define		TKRA_ES_NPARM	12			//number of edge params

/*
Vertex Parameter Arrays
 */
#define		TKRA_VX_XPOS	0			//screen X position
#define		TKRA_VX_YPOS	1			//screen Y position
#define		TKRA_VX_ZPOS	2			//screen Z position
#define		TKRA_VX_TPOS	3			//texture position
#define		TKRA_VX_CPOS	4			//color position
#define		TKRA_VX_T2POS	5			//texture position
#define		TKRA_VX_NPARM	8			//number of edge params

#define		TKRA_TRFL_ALPHA			0x0001	//Activity seen in alpha channel.
#define		TKRA_TRFL_HASMIP		0x0002	//Mipmaps Used
#define		TKRA_TRFL_NOALPHA		0x0004	//Ignore Alpha
#define		TKRA_TRFL_DOBLEND		0x0008	//Complex blend mode

#define		TKRA_TRFL_MINBL			0x0010	//
#define		TKRA_TRFL_MAGBL			0x0020	//
#define		TKRA_TRFL_CLAMPS		0x0040	//
#define		TKRA_TRFL_CLAMPT		0x0080	//
#define		TKRA_TRFL_NOZWRITE		0x0100	//No Z Write

#define		TKRA_TRFL_DOLMAP		0x0200	//Lightmap Blend
#define		TKRA_TRFL_NOCMOD		0x0400	//No Color Modulation
#define		TKRA_TRFL_DOZABLEND		0x0800	//Complex blend mode

#define		TKRA_TRFL_DOCMP			0x1000	//Do Compression
#define		TKRA_TRFL_FLIPST		0x2000	//Flip S/T Coords
#define		TKRA_TRFL_MORT			0x4000	//Morton Order
#define		TKRA_TRFL_ATEST			0x8000	//1-bit Alpha / Alpha Test

#define		TKRA_TRFL_PIXFMT_W		0x00000	//16-bit pixels
#define		TKRA_TRFL_PIXFMT_DW		0x10000	//32-bit pixels
#define		TKRA_TRFL_PIXFMT_QW		0x20000	//64-bit pixels
#define		TKRA_TRFL_PIXFMT_HDR	0x40000	//HDR / FP Pixels
#define		TKRA_TRFL_PIXFMT_UTX3	0x80000	//UTX3

#define		TKRA_PIXFMT_W			0x00	//16-bit pixels
#define		TKRA_PIXFMT_DW			0x01	//32-bit pixels
#define		TKRA_PIXFMT_QW			0x02	//64-bit pixels
#define		TKRA_PIXFMT_HDR			0x04	//HDR / FP Pixels
#define		TKRA_PIXFMT_UTX3		0x08	//UTX3

#define		TKRA_PIXFMT_RGBA5		0x00	//
#define		TKRA_PIXFMT_RGBA32		0x01	//
#define		TKRA_PIXFMT_RGBA_F8		0x05	//
#define		TKRA_PIXFMT_UTX3L		0x08	//UTX3
#define		TKRA_PIXFMT_UTX3H		0x0C	//UTX3

#define		TKRA_FMT_1SB			0x00
#define		TKRA_FMT_2SB			0x01
#define		TKRA_FMT_3SB			0x02
#define		TKRA_FMT_4SB			0x03
#define		TKRA_FMT_1UB			0x04
#define		TKRA_FMT_2UB			0x05
#define		TKRA_FMT_3UB			0x06
#define		TKRA_FMT_4UB			0x07
#define		TKRA_FMT_1SS			0x08
#define		TKRA_FMT_2SS			0x09
#define		TKRA_FMT_3SS			0x0A
#define		TKRA_FMT_4SS			0x0B
#define		TKRA_FMT_1US			0x0C
#define		TKRA_FMT_2US			0x0D
#define		TKRA_FMT_3US			0x0E
#define		TKRA_FMT_4US			0x0F
#define		TKRA_FMT_1SI			0x10
#define		TKRA_FMT_2SI			0x11
#define		TKRA_FMT_3SI			0x12
#define		TKRA_FMT_4SI			0x13
#define		TKRA_FMT_1UI			0x14
#define		TKRA_FMT_2UI			0x15
#define		TKRA_FMT_3UI			0x16
#define		TKRA_FMT_4UI			0x17
#define		TKRA_FMT_1F				0x18
#define		TKRA_FMT_2F				0x19
#define		TKRA_FMT_3F				0x1A
#define		TKRA_FMT_4F				0x1B
#define		TKRA_FMT_1D				0x1C
#define		TKRA_FMT_2D				0x1D
#define		TKRA_FMT_3D				0x1E
#define		TKRA_FMT_4D				0x1F
#define		TKRA_FMT_1H				0x20
#define		TKRA_FMT_2H				0x21
#define		TKRA_FMT_3H				0x22
#define		TKRA_FMT_4H				0x23


#define TKRA_BYTE								0x1400
#define TKRA_UNSIGNED_BYTE						0x1401
#define TKRA_UBYTE								0x1401
#define TKRA_SHORT								0x1402
#define TKRA_UNSIGNED_SHORT						0x1403
#define TKRA_USHORT								0x1403
#define TKRA_INT								0x1404
#define TKRA_UNSIGNED_INT						0x1405
#define TKRA_UINT								0x1405
#define TKRA_FLOAT								0x1406
#define TKRA_2_BYTES							0x1407
#define TKRA_3_BYTES							0x1408
#define TKRA_4_BYTES							0x1409
#define TKRA_DOUBLE								0x140A
#define	TKRA_HALF_FLOAT							0x140B
#define	TKRA_HFLOAT								0x140B

#define TKRA_GL_BYTE							0x1400
#define TKRA_GL_UNSIGNED_BYTE					0x1401
#define TKRA_GL_SHORT							0x1402
#define TKRA_GL_UNSIGNED_SHORT					0x1403
#define TKRA_GL_INT								0x1404
#define TKRA_GL_UNSIGNED_INT					0x1405
#define TKRA_GL_FLOAT							0x1406
#define TKRA_GL_2_BYTES							0x1407
#define TKRA_GL_3_BYTES							0x1408
#define TKRA_GL_4_BYTES							0x1409
#define TKRA_GL_DOUBLE							0x140A
#define	TKRA_GL_HALF_FLOAT						0x140B

/* Primitives */
#define TKRA_POINTS								0x0000
#define TKRA_LINES								0x0001
#define TKRA_LINE_LOOP							0x0002
#define TKRA_LINE_STRIP							0x0003
#define TKRA_TRIANGLES							0x0004
#define TKRA_TRIANGLE_STRIP						0x0005
#define TKRA_TRIANGLE_FAN						0x0006
#define TKRA_QUADS								0x0007
#define TKRA_QUAD_STRIP							0x0008
#define TKRA_POLYGON							0x0009

#define TKRA_GL_POINTS							0x0000
#define TKRA_GL_LINES							0x0001
#define TKRA_GL_LINE_LOOP						0x0002
#define TKRA_GL_LINE_STRIP						0x0003
#define TKRA_GL_TRIANGLES						0x0004
#define TKRA_GL_TRIANGLE_STRIP					0x0005
#define TKRA_GL_TRIANGLE_FAN					0x0006
#define TKRA_GL_QUADS							0x0007
#define TKRA_GL_QUAD_STRIP						0x0008
#define TKRA_GL_POLYGON							0x0009

#define TKRA_RGB								0x1907
#define TKRA_RGBA								0x1908
#define TKRA_BGR								0x80E0
#define TKRA_BGRA								0x80E1

#define TKRA_TEXTURE_2D							0x0DE1
#define TKRA_GL_TEXTURE_WRAP_S					0x2802
#define TKRA_GL_TEXTURE_WRAP_T					0x2803
#define TKRA_GL_TEXTURE_MAG_FILTER				0x2800
#define TKRA_GL_TEXTURE_MIN_FILTER				0x2801

#define TKRA_GL_NEAREST							0x2600
#define TKRA_GL_LINEAR							0x2601
#define TKRA_GL_NEAREST_MIPMAP_NEAREST			0x2700
#define TKRA_GL_NEAREST_MIPMAP_LINEAR			0x2702
#define TKRA_GL_LINEAR_MIPMAP_NEAREST			0x2701
#define TKRA_GL_LINEAR_MIPMAP_LINEAR			0x2703

#define TKRA_GL_REPEAT							0x2901
#define TKRA_GL_CLAMP							0x2900
#define TKRA_GL_CLAMP_TO_EDGE					0x812F

#define TKRA_MODELVIEW							0x1700
#define TKRA_PROJECTION							0x1701
#define TKRA_TEXTURE							0x1702

#define TKRA_GL_VENDOR							0x1F00
#define TKRA_GL_RENDERER						0x1F01
#define TKRA_GL_VERSION							0x1F02
#define TKRA_GL_EXTENSIONS						0x1F03

#define TKRA_GL_ZERO							0x0
#define TKRA_GL_ONE								0x1
#define TKRA_GL_SRC_COLOR						0x0300
#define TKRA_GL_ONE_MINUS_SRC_COLOR				0x0301
#define TKRA_GL_SRC_ALPHA						0x0302
#define TKRA_GL_ONE_MINUS_SRC_ALPHA				0x0303
#define TKRA_GL_DST_ALPHA						0x0304
#define TKRA_GL_ONE_MINUS_DST_ALPHA				0x0305
#define TKRA_GL_DST_COLOR						0x0306
#define TKRA_GL_ONE_MINUS_DST_COLOR				0x0307
#define TKRA_GL_SRC_ALPHA_SATURATE				0x0308
#define TKRA_GL_CONSTANT_COLOR					0x8001
#define TKRA_GL_ONE_MINUS_CONSTANT_COLOR		0x8002
#define TKRA_GL_CONSTANT_ALPHA					0x8003
#define TKRA_GL_ONE_MINUS_CONSTANT_ALPHA		0x8004

#define TKRA_GL_NEVER							0x0200
#define TKRA_GL_LESS							0x0201
#define TKRA_GL_EQUAL							0x0202
#define TKRA_GL_LEQUAL							0x0203
#define TKRA_GL_GREATER							0x0204
#define TKRA_GL_NOTEQUAL						0x0205
#define TKRA_GL_GEQUAL							0x0206
#define TKRA_GL_ALWAYS							0x0207
#define TKRA_GL_DEPTH_TEST						0x0B71
#define TKRA_GL_DEPTH_BITS						0x0D56
#define TKRA_GL_DEPTH_CLEAR_VALUE				0x0B73
#define TKRA_GL_DEPTH_FUNC						0x0B74
#define TKRA_GL_DEPTH_RANGE						0x0B70
#define TKRA_GL_DEPTH_WRITEMASK					0x0B72

#define TKRA_GL_ALPHA_TEST						0x0BC0

#define TKRA_GL_BLEND							0x0BE2
#define TKRA_GL_BLEND_SRC						0x0BE1
#define TKRA_GL_BLEND_DST						0x0BE0

#define TKRA_GL_CMPR_RGBA_S3TC_DXT5				0x83F3
#define TKRA_GL_CMPR_RGBA_S3TC_DXT1				0x83F1
#define TKRA_GL_CMPR_RGB_S3TC_DXT1				0x83F0

#define TKRA_GL_RGB_S3TC						0x83A0
#define TKRA_GL_RGBA_S3TC						0x83A1


#define TKRA_GL_CURRENT_BIT						0x00000001
#define TKRA_GL_POINT_BIT						0x00000002
#define TKRA_GL_LINE_BIT						0x00000004
#define TKRA_GL_POLYGON_BIT						0x00000008
#define TKRA_GL_POLYGON_STIPPLE_BIT				0x00000010
#define TKRA_GL_PIXEL_MODE_BIT					0x00000020
#define TKRA_GL_LIGHTING_BIT					0x00000040
#define TKRA_GL_FOG_BIT							0x00000080
#define TKRA_GL_DEPTH_BUFFER_BIT				0x00000100
#define TKRA_GL_ACCUM_BUFFER_BIT				0x00000200
#define TKRA_GL_STENCIL_BUFFER_BIT				0x00000400
#define TKRA_GL_VIEWPORT_BIT					0x00000800
#define TKRA_GL_TRANSFORM_BIT					0x00001000
#define TKRA_GL_ENABLE_BIT						0x00002000
#define TKRA_GL_COLOR_BUFFER_BIT				0x00004000
#define TKRA_GL_HINT_BIT						0x00008000
#define TKRA_GL_EVAL_BIT						0x00010000
#define TKRA_GL_LIST_BIT						0x00020000
#define TKRA_GL_TEXTURE_BIT						0x00040000
#define TKRA_GL_SCISSOR_BIT						0x00080000

#define TKRA_BLEND_ZERO							0x0
#define TKRA_BLEND_ONE							0x1
#define TKRA_BLEND_SRC_COLOR					0x2
#define TKRA_BLEND_ONE_MINUS_SRC_COLOR			0x3
#define TKRA_BLEND_SRC_ALPHA					0x4
#define TKRA_BLEND_ONE_MINUS_SRC_ALPHA			0x5
#define TKRA_BLEND_DST_ALPHA					0x6
#define TKRA_BLEND_ONE_MINUS_DST_ALPHA			0x7
#define TKRA_BLEND_DST_COLOR					0x8
#define TKRA_BLEND_ONE_MINUS_DST_COLOR			0x9

#define TKRA_ZAT_NV								0x0
#define TKRA_ZAT_AL								0x1
#define TKRA_ZAT_EQ								0x2
#define TKRA_ZAT_NE								0x3
#define TKRA_ZAT_LT								0x4
#define TKRA_ZAT_LE								0x5
#define TKRA_ZAT_GT								0x6
#define TKRA_ZAT_GE								0x7

#define TKRA_ZAT_NZ								0x8

#define TKRA_ZAT_NV_NZ							0x8
#define TKRA_ZAT_AL_NZ							0x9
#define TKRA_ZAT_EQ_NZ							0xA
#define TKRA_ZAT_NE_NZ							0xB
#define TKRA_ZAT_LT_NZ							0xC
#define TKRA_ZAT_LE_NZ							0xD
#define TKRA_ZAT_GT_NZ							0xE
#define TKRA_ZAT_GE_NZ							0xF

#define TKRA_STFL1_DEPTHTEST					0x00000001
#define TKRA_STFL1_NODEPTHWRITE					0x00000002
#define TKRA_STFL1_ALPHATEST					0x00000004
#define TKRA_STFL1_BLEND						0x00000008
#define TKRA_STFL1_CULLFACE						0x00000010
#define TKRA_STFL1_CULL_FT						0x00000020
#define TKRA_STFL1_CULL_BK						0x00000040
#define TKRA_STFL1_CULL_CW						0x00000080

#define TKRA_STFL1_VERTEX_ARRAY					0x00000100
#define TKRA_STFL1_NORMAL_ARRAY					0x00000200
#define TKRA_STFL1_COLOR_ARRAY					0x00000400
#define TKRA_STFL1_INDEX_ARRAY					0x00000800
#define TKRA_STFL1_TEXCOORD_ARRAY				0x00001000
#define TKRA_STFL1_EDGEFLAG_ARRAY				0x00002000

#define TKRA_STFL1_TEX_REPLACE					0x00004000

#define TKRA_STFL1_TEXTURE2D					0x00008000
#define TKRA_STFL1_FILL_LINE					0x00010000
#define TKRA_STFL1_NOSUBDIV						0x00020000
#define TKRA_STFL1_LIGHTING						0x00040000
#define TKRA_STFL1_FOG							0x00080000
#define TKRA_STFL1_USESHADER					0x00100000

#define TKRA_STFL1_DOSHADER_MASK				0x001C0000

#define TKRA_STFL1_STENCILTEST					0x00200000


#define TKRA_CTXPARM_FINISH						0x00010000
#define TKRA_CTXPARM_FLUSH						0x00010001
#define TKRA_CTXPARM_CLEAR						0x00010002

#define TKRA_CTXPARM_MASK						0x00FFFFFF


typedef unsigned short	tkra_rastpixel;
// typedef unsigned short	tkra_zbufpixel;
typedef signed short		tkra_zbufpixel;

typedef unsigned int		tkra_rast2pixel;
typedef signed int		tkra_zbuf2pixel;

typedef sbyte		tkra_s8;
typedef byte		tkra_u8;
typedef s16		tkra_s16;
typedef u16		tkra_u16;
typedef s32		tkra_s32;
typedef u32		tkra_u32;
typedef float		tkra_f32;
typedef double		tkra_f64;

typedef struct TKRA_Context_s		TKRA_Context;
typedef struct TKRA_SvContext_s	TKRA_SvContext;
typedef struct TKRA_ClContext_s	TKRA_ClContext;
typedef struct TKRA_ContextVt_s	TKRA_ContextVt;
typedef struct TKRA_TexImage_s		TKRA_TexImage;

// #ifdef __BJX2__
#ifdef BJX2_SIMD
typedef __vec4f tkra_vec4f;
typedef __vec2f tkra_vec2f;

#define	tkra_mkvec4f(x, y, z, w)	((__vec4f) { x, y, z, w })
#define	tkra_mkvec2f(x, y)			((__vec2f) { x, y })

#else
typedef struct {
	float x;
	float y;
	float z;
	float w;
}tkra_vec4f;

typedef struct {
	float x;
	float y;
}tkra_vec2f;

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

typedef struct {
	tkra_vec4f row0;
	tkra_vec4f row1;
	tkra_vec4f row2;
	tkra_vec4f row3;
}tkra_mat4;

typedef struct {
	tkra_vec4f	xyz;			//00
	tkra_vec4f	pv;				//10
	tkra_vec2f	st;				//20
	u32			rgb;			//28
	u32			nv;				//2C

	int			fl;				//30
	u32			mrgb;			//34, RGBA (Material)
//	int			pad1;			//38
//	int			pad2;			//3C

	tkra_vec2f	st2;			//38, Second Coords

//	tkra_vec4f	norm;			//40
	u64			attrib[8];		//40
	//80
}tkra_trivertex;

typedef struct {
	s32 x;		//00
	s32 y;		//04
	s32 z;		//08
	s32	s;		//0C
	s32 t;		//10
	u32 rgb;	//14
	//18
	s32	s2;		//18
	s32 t2;		//1C
	u64 attrib[4];
}tkra_projvertex;

typedef u64 (*tkra_blendfunc_t)(u64 sclr, u64 dclr);

typedef int (*tkra_zatest_t)(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod);

typedef struct TKRA_ShaderProg_s		TKRA_ShaderProg;
typedef struct TKRA_ShaderTrace_s		TKRA_ShaderTrace;
typedef struct TKRA_ShaderOp_s			TKRA_ShaderOp;
typedef struct TKRA_DrawPrimArrays_s	TKRA_DrawPrimArrays;


struct TKRA_ShaderProg_s
{
	TKRA_ShaderProg *next;
	u16 sdr_id;
	u64 vars_uniform[256];	//uniform variables
	u64 *wordcode_img[4];	//wordcode image (vertex)
};

#define TKRA_SHADER_MAXTROPS	16

struct TKRA_ShaderTrace_s
{
	TKRA_ShaderTrace *(*Run)(TKRA_Context *ctx, TKRA_ShaderTrace *tr);
	u32 sdr_idpc;			//Shader ID+PC
	byte n_ops;
	TKRA_ShaderTrace	*tr_next;		//Next Trace (PC Order)
	TKRA_ShaderTrace	*tr_jump;		//Jump Trace
	TKRA_ShaderOp		*ops[TKRA_SHADER_MAXTROPS];
};

struct TKRA_ShaderOp_s
{
	void (*Run)(TKRA_Context *ctx, TKRA_ShaderOp *op);
	byte nmid;
	byte fmid;
	byte rn;
	byte rs;
	byte rt;
	byte opfl;
	u32	imm;
};

#define TKRA_MAX_MULTITEX		8

struct TKRA_ContextVt_s
{
void *resv0;		//reserved 0
void *resv1;		//reserved 0
void *resv2;		//reserved 0
void *resv3;		//reserved 0

int (*DrawPrimitiveIndexArrayObj)(
	TKRA_Context *ctx,
	TKRA_DrawPrimArrays	*parr,
	int mode,		int count);

void (*UpdateTexImg)(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	tkra_rastpixel *buf,
	int xs, int ys, int mip, int flag);
void (*UpdateTexImgUtx2)(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	u64 *buf,
	int xs, int ys, int mip, int flag);

TKRA_TexImage *(*LookupTexImg)(TKRA_Context *ctx, int num);
TKRA_TexImage *(*GetTexImg)(TKRA_Context *ctx, int num);
int (*BindTexImg)(TKRA_Context *ctx, TKRA_TexImage *img);
int (*QueryContext)(TKRA_Context *ctx, u64 param, void *iptr, void *optr);
int (*ModifyContext)(TKRA_Context *ctx, u64 param, void *iptr, void *optr);
int (*SetupScreen)(TKRA_Context *ctx, int xs, int ys, u64 attrib);

};



struct TKRA_DrawPrimArrays_s
{
int		vptr_xyz_fmt;
int		vptr_xyz_str;
void 	*vptr_xyz_ptr;

int		vptr_st_fmt;	//ST format, Texture 0
int		vptr_st_str;	//ST stride, Texture 0
void 	*vptr_st_ptr;	//ST pointer, Texture 0

int		vptr_rgb_fmt;
int		vptr_rgb_str;
void 	*vptr_rgb_ptr;

int		vptr_nv_fmt;
int		vptr_nv_str;
void 	*vptr_nv_ptr;

int		vptr_ix_fmt;
int		vptr_ix_str;
void 	*vptr_ix_ptr;

int		vptr_sta_fmt[TKRA_MAX_MULTITEX];	//ST format, Multitexture
int		vptr_sta_str[TKRA_MAX_MULTITEX];	//ST stride, Multitexture
void 	*vptr_sta_ptr[TKRA_MAX_MULTITEX];	//ST pointer, Multitexture
};

#define TKRA_MAX_PROJSTACK		64
// #define TKRA_LIM_PROJSTACK		48
#define TKRA_LIM_PROJSTACK		(TKRA_MAX_PROJSTACK-8)

/* Server-side Local Context */
struct TKRA_SvContext_s
{
TKRA_Context  *clctx;			//client/shared context


void (*DrawSpan)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanHZt)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanZt)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void (*DrawSpan_Min)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanHZt_Min)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanZt_Min)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void (*DrawSpan_Mag)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanHZt_Mag)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanZt_Mag)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void (*DrawSpanZb)(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);
void (*RasterWalkEdges)(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);
void (*RasterWalkEdgesNcp)(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);


tkra_blendfunc_t	Blend;
tkra_zatest_t		ZaTest;

tkra_rastpixel *tex_img;			//bound texture image (RGB5A)
tkra_rastpixel *tex_img2;			//bound texture image (RGB5A)

tkra_rastpixel *texb_img;			//bound texture image (RGBA32)
tkra_rastpixel *texb_img2;			//bound texture image (RGBA32)

u32		*tex_img_bcn;				//texture images (block compressed)
u32		*tex_img_bcn2;				//texture images (block compressed)
byte	tex_xshl;
byte	tex_yshl;
byte	tex_mmip;
byte	tex_nmip;
int		triflag;
int		tex_flag;			//texture state flags
int		bfn_flag;			//blend state flags

byte	tex_xshl2;
byte	tex_yshl2;
byte	tex_mmip2;
byte	tex_nmip2;
int		tex_flag2;			//texture state flags

TKRA_TexImage *span_tex_cur;
int		span_trifl;

TKRA_TexImage *tex_cur;
TKRA_TexImage *tex_cur2;
TKRA_TexImage *tex_cur_mtx[TKRA_MAX_MULTITEX];


tkra_trivertex v0stk[TKRA_MAX_PROJSTACK];
tkra_trivertex v1stk[TKRA_MAX_PROJSTACK];
tkra_trivertex v2stk[TKRA_MAX_PROJSTACK];
tkra_trivertex v3stk[TKRA_MAX_PROJSTACK];



int tex_rov;
TKRA_TexImage *tex_list;
TKRA_TexImage *tex_hash[256];

TKRA_ShaderTrace *sdr_trcache[1024];
u16 sdr_trchain[1024];
u16 sdr_n_trace;
u16 sdr_trhash[256];

TKRA_ShaderProg		*sdr_progs;
TKRA_ShaderTrace	*sdr_tr_free;
TKRA_ShaderTrace	*sdr_tr_next;
TKRA_ShaderOp		*sdr_op_free;

TKRA_ShaderProg		*sdr_prog_cur;
TKRA_ShaderTrace	*sdr_tr_e_vtx;
};

/* Client-side Local Context */
struct TKRA_ClContext_s
{
u32 magic1;
u32 size;
};

struct TKRA_Context_s
{
TKRA_ContextVt	*vt;			//00, vtable
TKRA_SvContext  *svctx;			//08, server context

u32				magic1;			//10, magic number (0x12345678, sanity check)
u32				size_context;	//14, size of context struct

byte			*screen_mem;	//18, combined screen memory
tkra_rastpixel	*screen_rgb;	//20, display buffer (RGB)
tkra_zbufpixel	*screen_zbuf;	//28, display buffer (Z buffer)
tkra_zbufpixel	*screen_zbuf2;	//30, display buffer (Z buffer)
byte			*screen_sten;	//38, stencil buffer

int		screen_xsize;			//40
int		screen_ysize;			//44


int		scr_xscale;				//48
int		scr_yscale;				//4C
int		scr_xcenter;			//50
int		scr_ycenter;			//5C

tkra_vec4f		prj_xyzsc;		//60
tkra_vec4f		prj_xyzbi;		//70

tkra_rastpixel	clear_rgb5;		//80
tkra_zbufpixel	clear_zbuf;		//82
u32				clear_rgba;		//84

void	*ptr_align;				//88
int		clip_x0;				//90, Needs to be 64-bit aligned (SIMD, 0)
int		clip_x1;				//94, Needs to be 64-bit aligned
int		clip_y0;				//98, Needs to be 64-bit aligned
int		clip_y1;				//9C, Needs to be 64-bit aligned (SIMD, 3)

float			scr_clip_l;		//A0
float			scr_clip_r;		//A4
float			scr_clip_t;		//A8
float			scr_clip_b;		//AC

u64				stateflag1;		//B0

tkra_vec4f	(*VaGetPtr_xyz)(void *ptr);		//B8
tkra_vec2f	(*VaGetPtr_st )(void *ptr);		//C0
u32			(*VaGetPtr_rgb)(void *ptr);		//C8
int			(*VaGetPtr_idx)(void *ptr);		//D0


float		trans_znear;		//D8
float		trans_zfar;			//DC
float		trans_zrange;		//C0
u32			pad0;				//C4

tkra_mat4	mat_tproj;			//C8, modelview*projection

tkra_mat4	mat_xform;			//108, modelview
tkra_mat4	mat_xproj;			//148, projection

tkra_mat4	stk_xform[64];		//188, modelview
tkra_mat4	stk_xproj[8];		//projection
int			stkpos_xform;
int			stkpos_xproj;

byte		matmode;
byte		blend_sfunc;
byte		blend_dfunc;
byte		blend_sfunc2;

byte		blend_dfunc2;
byte		zat_alfunc;
byte		zat_zfunc;
byte		blend_isready;

byte		zat_stfunc;
byte		zat_stref;
byte		zat_stmask;
byte		zat_stclear;

byte		zat_sto_stfail;
byte		zat_sto_zfail;
byte		zat_sto_zpass;
byte		light_mask;

byte		cachemode;
byte		tex2d_active;
byte		bindsticky;
byte		pixelfmt;			//native pixel format

byte		resv_byte0;
byte		resv_byte1;
byte		resv_byte2;
byte		resv_byte3;

tkra_vec4f	light_model_ambient;
tkra_vec4f	light_ambient[8];
tkra_vec4f	light_diffuse[8];
tkra_vec4f	light_specular[8];
tkra_vec4f	light_position[8];
tkra_vec4f	light_spot_dir[8];
float		light_spot_exp[8];
float		light_spot_cutoff[8];
float		light_attn_const[8];
float		light_attn_linear[8];
float		light_attn_quadratic[8];

u32		zat_cref;

int		tkgl_usepgm_vtx;		//vertex shader
u64		*tkgl_sdr_uniform;		//uniform parameters
u64		*tkgl_sdr_local;		//local parameters
u64		*tkgl_sdr_attrib;		//local parameters

TKRA_DrawPrimArrays *vptr;		//draw arrays state

int		tkgl_begin_mode;
u32		tkgl_begin_rgba;
u32		tkgl_begin_norm;
float	tkgl_begin_st[2];
//float	tkgl_begin_nv[3];

float	*tkgl_begin_vtxa;
int		tkgl_begin_nvtx;
int		tkgl_begin_mvtx;

u64		tex2d_enamask;						//enabled textures

int		stat_base_tris;
int		stat_frag_tris;
int		stat_draw_tris;
int		stat_reject_tris;
int		stat_blown_tris;
int		stat_drawpts1_tris;
int		stat_drawpts3_tris;

int		stat_frustum_tris;
int		stat_microbase_tris;
int		stat_microfrag_tris;
int		stat_backface_tris;
int		stat_negw_tris;
int		stat_zocc_tris;

u32				magic2;			//magic number (0x12345678, sanity check)

int		boundtexid[TKRA_MAX_MULTITEX];		//bound texture IDs

TKRA_ClContext	*clctx;

tkra_rast2pixel	*screenb_rgb;	//display buffer (RGB, DWORD)
tkra_zbuf2pixel	*screenb_zbuf;	//display buffer (Z buffer)
tkra_zbuf2pixel	*screenb_zbuf2;	//display buffer (Z buffer)
byte			*screenb_sten;	//stencil buffer

tkra_rast2pixel	clear_rgba32;
tkra_zbuf2pixel	clear_zbuf32;


// unsorted past here

TKRA_DrawPrimArrays t_vptr;


};

struct TKRA_TexImage_s
{
TKRA_TexImage *next;				//00
TKRA_TexImage *chain;				//08
tkra_rastpixel *tex_img;			//10 texture images (RGB5A / RBGA32)
u32		*tex_img_bcn;				//18 texture images (block compressed)
int		tex_id;						//20 ID number
byte	tex_xshl;					//24 texture X size (log-2)
byte	tex_yshl;					//25 texture Y size (log-2)
byte	tex_mmip;					//26
byte	tex_nmip;					//27
int		tex_flag;					//28
int		pad0;						//2C
u64		pad1;						//30
void	*svptr;						//38, server-side pointer
int		tex_mipofs[16];				//40 mip offs, pixels
int		tex_mipofs_bcn[16];			//80 mip offs, block DWORDs
};

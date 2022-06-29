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
#define		TKRA_ES_NPARM	8			//number of edge params

/*
Vertex Parameter Arrays
 */
#define		TKRA_VX_XPOS	0			//screen X position
#define		TKRA_VX_YPOS	1			//screen Y position
#define		TKRA_VX_ZPOS	2			//screen Z position
#define		TKRA_VX_TPOS	3			//texture position
#define		TKRA_VX_CPOS	4			//color position
#define		TKRA_VX_NPARM	5			//number of edge params

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


typedef unsigned short	tkra_rastpixel;
// typedef unsigned short	tkra_zbufpixel;
typedef signed short		tkra_zbufpixel;

typedef sbyte		tkra_s8;
typedef byte		tkra_u8;
typedef s16		tkra_s16;
typedef u16		tkra_u16;
typedef s32		tkra_s32;
typedef u32		tkra_u32;
typedef float		tkra_f32;
typedef double		tkra_f64;

typedef struct TKRA_Context_s		TKRA_Context;
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
	tkra_vec4f	xyz;
	tkra_vec2f	st;
	u32			rgb;
}tkra_trivertex;

typedef struct {
	s32 x;
	s32 y;
	s32 z;
	s32	s;
	s32 t;

//	u16 x;
//	u16 y;
//	u16 z;
//	u16 w;
//	s16	s;
//	s16 t;
	u32 rgb;
}tkra_projvertex;

typedef u64 (*tkra_blendfunc_t)(u64 sclr, u64 dclr);

typedef int (*tkra_zatest_t)(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod);


struct TKRA_Context_s
{
byte			*screen_mem;	//combined screen memory
tkra_rastpixel	*screen_rgb;	//display buffer (RGB)
tkra_zbufpixel	*screen_zbuf;	//display buffer (Z buffer)
tkra_zbufpixel	*screen_zbuf2;	//display buffer (Z buffer)
byte			*screen_sten;	//stencil buffer
int		screen_xsize;
int		screen_ysize;


int		scr_xscale;
int		scr_yscale;
int		scr_xcenter;
int		scr_ycenter;

tkra_vec4f		prj_xyzsc;
tkra_vec4f		prj_xyzbi;

tkra_rastpixel	clear_rgb5;
tkra_zbufpixel	clear_zbuf;
u32				clear_rgba;

void	*ptr_align;
int		clip_x0;	//Needs to be 64-bit aligned (SIMD, 0)
int		clip_x1;	//Needs to be 64-bit aligned
int		clip_y0;	//Needs to be 64-bit aligned
int		clip_y1;	//Needs to be 64-bit aligned (SIMD, 3)

float			scr_clip_l, scr_clip_r;
float			scr_clip_t, scr_clip_b;


TKRA_TexImage *tex_cur;
tkra_rastpixel *tex_img;			//bound texture image
u32		*tex_img_bcn;				//texture images (block compressed)
byte	tex_xshl;
byte	tex_yshl;
byte	tex_mmip;
byte	tex_nmip;
int		triflag;
int		tex_flag;			//texture state flags
int		bfn_flag;			//blend state flags

TKRA_TexImage *span_tex_cur;
int		span_trifl;

u64		stateflag1;

tkra_rastpixel *dst_rgb;		//destination (RGB)
tkra_zbufpixel *dst_zbuf;		//destination (Zbuf)

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

tkra_vec4f	(*VaGetPtr_xyz)(void *ptr);
tkra_vec2f	(*VaGetPtr_st )(void *ptr);
u32			(*VaGetPtr_rgb)(void *ptr);
int			(*VaGetPtr_idx)(void *ptr);

int tex_rov;
TKRA_TexImage *tex_list;
TKRA_TexImage *tex_hash[256];


float		trans_znear;
float		trans_zfar;
float		trans_zrange;

tkra_mat4	mat_tproj;		//modelview*projection

tkra_mat4	mat_xform;		//modelview
tkra_mat4	mat_xproj;		//projection

tkra_mat4	stk_xform[64];		//modelview
tkra_mat4	stk_xproj[8];		//projection
int			stkpos_xform;
int			stkpos_xproj;
byte		matmode;
byte		blend_sfunc;
byte		blend_dfunc;

byte		zat_alfunc;
byte		zat_zfunc;
byte		blend_isready;

tkra_blendfunc_t	Blend;
tkra_zatest_t		ZaTest;

u32		zat_cref;

int		tkgl_vptr_xyz_nsz;
int		tkgl_vptr_xyz_ty;
int		tkgl_vptr_xyz_str;
void 	*tkgl_vptr_xyz_ptr;

int		tkgl_vptr_st_nsz;
int		tkgl_vptr_st_ty;
int		tkgl_vptr_st_str;
void 	*tkgl_vptr_st_ptr;

int		tkgl_vptr_rgb_nsz;
int		tkgl_vptr_rgb_ty;
int		tkgl_vptr_rgb_str;
void 	*tkgl_vptr_rgb_ptr;

int		tkgl_vptr_nv_nsz;
int		tkgl_vptr_nv_ty;
int		tkgl_vptr_nv_str;
void 	*tkgl_vptr_nv_ptr;

int		tkgl_begin_mode;
u32		tkgl_begin_rgba;
u32		tkgl_begin_norm;
float	tkgl_begin_st[2];
//float	tkgl_begin_nv[3];

float	*tkgl_begin_vtxa;
int		tkgl_begin_nvtx;
int		tkgl_begin_mvtx;

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
};

struct TKRA_TexImage_s
{
TKRA_TexImage *next;
TKRA_TexImage *chain;

tkra_rastpixel *tex_img;			//texture images
u32		*tex_img_bcn;				//texture images (block compressed)
int		tex_id;
int		tex_mipofs[16];				//mip offs, pixels
int		tex_mipofs_bcn[16];			//mip offs, block DWORDs
byte	tex_xshl;					//texture X size (log-2)
byte	tex_yshl;					//texture Y size (log-2)
byte	tex_mmip;
byte	tex_nmip;
int		tex_flag;
};

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
#define		TKRA_DS_NPARM	10			//number of drawspan params

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

typedef unsigned short	tkra_rastpixel;
typedef unsigned short	tkra_zbufpixel;

typedef struct TKRA_Context_s		TKRA_Context;
typedef struct TKRA_TexImage_s		TKRA_TexImage;

// #ifdef __BJX2__
#ifdef BJX2_SIMD
typedef __vec4f tkra_vec4f;
typedef __vec2f tkra_vec2f;
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
	u16 x;
	u16 y;
	u16 z;
	u16 w;
	u16	s;
	u16 t;
	u32 rgb;
}tkra_projvertex;


struct TKRA_Context_s
{
tkra_rastpixel *screen_rgb;		//display buffer (RGB)
tkra_zbufpixel *screen_zbuf;	//display buffer (Z buffer)
int		screen_xsize;
int		screen_ysize;


int		scr_xscale;
int		scr_yscale;
int		scr_xcenter;
int		scr_ycenter;

int 	clip_x0;
int		clip_x1;
int		clip_y0;
int		clip_y1;


TKRA_TexImage *tex_cur;
tkra_rastpixel *tex_img;			//bound texture image
int		tex_xshl;
int		tex_yshl;

tkra_rastpixel *dst_rgb;		//destination (RGB)
tkra_zbufpixel *dst_zbuf;		//destination (Zbuf)

void (*DrawSpan)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanHZt)(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void (*DrawSpanZb)(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);
void (*RasterWalkEdges)(TKRA_Context *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);

int tex_rov;
TKRA_TexImage *tex_list;
TKRA_TexImage *tex_hash[256];

tkra_mat4	mat_tproj;

tkra_mat4	mat_xform;
tkra_mat4	mat_xproj;
};

struct TKRA_TexImage_s
{
TKRA_TexImage *next;
TKRA_TexImage *chain;

tkra_rastpixel *tex_img;			//texture images
int		tex_id;
int		tex_mipofs[16];
byte	tex_xshl;					//texture X size (log-2)
byte	tex_yshl;					//texture Y size (log-2)
};

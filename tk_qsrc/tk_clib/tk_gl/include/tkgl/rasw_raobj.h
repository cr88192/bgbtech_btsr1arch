// #define BGBRASW_SS_YUVA

#ifdef _MSC_VER
#define BGBRASW_NOVECTOR	__pragma(loop(no_vector))
#else
#define BGBRASW_NOVECTOR
#endif

#define BGBRASW_MAKEPIXEL(r, g, b, a)	\
	(((a)<<24)|((r)<<16)|((g)<<8)|(b))
#define BGBRASW_PIXEL_RED(pix)		(((pix)>>16)&255)
#define BGBRASW_PIXEL_GREEN(pix)	(((pix)>> 8)&255)
#define BGBRASW_PIXEL_BLUE(pix)		(((pix)	)&255)
#define BGBRASW_PIXEL_ALPHA(pix)	(((pix)>>24)&255)

// #define BGBRASW_PIXEL_R(pix)		(((pix)>>16)&255)
// #define BGBRASW_PIXEL_G(pix)		(((pix)>> 8)&255)
// #define BGBRASW_PIXEL_B(pix)		(((pix)	)&255)
// #define BGBRASW_PIXEL_A(pix)		(((pix)>>24)&255)

#define BGBRASW_PIXEL_R(pix)		((int)(((pix)>>16)&255))
#define BGBRASW_PIXEL_G(pix)		((int)(((pix)>> 8)&255))
#define BGBRASW_PIXEL_B(pix)		((int)(((pix)    )&255))
#define BGBRASW_PIXEL_A(pix)		((int)(((pix)>>24)&255))

#if 0
#define BGBRASW_YYPIXEL_Y0(pix)		((int)(((pix)    )&255))
#define BGBRASW_YYPIXEL_Y1(pix)		((int)(((pix)>> 8)&255))
#define BGBRASW_YYPIXEL_Y2(pix)		((int)(((pix)>>16)&255))
#define BGBRASW_YYPIXEL_Y3(pix)		((int)(((pix)>>24)&255))
#define BGBRASW_YYPIXEL_U(pix)		((int)(((pix)>>32)&255))
#define BGBRASW_YYPIXEL_V(pix)		((int)(((pix)>>40)&255))
#define BGBRASW_YYPIXEL_A(pix)		((int)(((pix)>>48)&255))
#define BGBRASW_YYPIXEL_X(pix)		((int)(((pix)>>56)&255))
#endif

#if 1

#define BGBRASW_MAKEYYPIXEL(y0, y1, y2, y3, u, v, a, x)	\
	((((x)&240)<<24)|(((a)&240)<<20)|(((v)&240)<<16)|(((u)&240)<<12)| \
	(((y3)&240)<<8)|(((y2)&240)<<4)| \
	 ((y1)&240)|(((y0)&240)>>4))

#define BGBRASW_YYPIXEL_Y0(pix)		((int)(((pix)<< 4)&240))
#define BGBRASW_YYPIXEL_Y1(pix)		((int)(((pix)    )&240))
#define BGBRASW_YYPIXEL_Y2(pix)		((int)(((pix)>> 4)&240))
#define BGBRASW_YYPIXEL_Y3(pix)		((int)(((pix)>> 8)&240))
#define BGBRASW_YYPIXEL_U(pix)		((int)(((pix)>>12)&240))
#define BGBRASW_YYPIXEL_V(pix)		((int)(((pix)>>16)&240))
#define BGBRASW_YYPIXEL_A(pix)		((int)(((pix)>>20)&240))
#define BGBRASW_YYPIXEL_X(pix)		((int)(((pix)>>24)&240))
#define BGBRASW_YYPIXEL_X2(pix)		((int)(((pix)>>28)))
#endif

#define BGBRASW_MASK_CLR_ALPHA		0xFF000000UL
#define BGBRASW_MASK_CLR_RED		0x00FF0000UL
#define BGBRASW_MASK_CLR_GREEN		0x0000FF00UL
#define BGBRASW_MASK_CLR_BLUE		0x000000FFUL

#define BGBRASW_MASK_DEPTH			0xFFFFFF00UL
#define BGBRASW_MASK_STENCIL		0x000000FFUL

#define BGBRASW_MASK_STENCIL_INV	0xFFFFFF00UL

#define BGBRASW_MASK_YYCLR_ALPHA	0x0F000000UL

#define BGBRASW_CLAMP(val, min, max)	\
	((val)<(min))?(min):(((val)>(max))?(max):(val));

#define BGBRASW_PTYPE_NONE				0
#define BGBRASW_PTYPE_TRIANGLE			1
#define BGBRASW_PTYPE_BOX				2
#define BGBRASW_PTYPE_LINE				3
#define BGBRASW_PTYPE_QUAD				4

#define BGBRASW_PMODE_NONE				0
#define BGBRASW_PMODE_FLATBASIC			1		//flat color, replace
#define BGBRASW_PMODE_FLATINTERP		2		//interpolated color
#define BGBRASW_PMODE_FLATBASICZTEST	3		//color with Z test
#define BGBRASW_PMODE_FLATINTERPZTEST	4		//interp color, ZT
#define BGBRASW_PMODE_TEXBASIC			5		//textured, replace
#define BGBRASW_PMODE_TEXINTERP			6		//tex + interp color
#define BGBRASW_PMODE_TEXBASICZTEST		7		//tex + Z test
#define BGBRASW_PMODE_TEXINTERPZTEST	8		//tex + interp color + ZT
#define BGBRASW_PMODE_FLATINTERPTABS	9		//interp color, TABS
#define BGBRASW_PMODE_TEXINTERPTABS		10		//tex + interp, TABS

#define BGBRASW_ENABLE_ALPHA_TEST		0x00000001
#define BGBRASW_ENABLE_DEPTH_TEST		0x00000002
#define BGBRASW_ENABLE_TEXTURE_2D		0x00000004
#define BGBRASW_ENABLE_BLEND			0x00000008
#define BGBRASW_ENABLE_STENCIL_TEST		0x00000010
#define BGBRASW_ENABLE_SCISSOR_TEST		0x00000020
#define BGBRASW_ENABLE_CULL_FACE		0x00000040
#define BGBRASW_ENABLE_DEPTH_NOWRITE	0x00000080
#define BGBRASW_ENABLE_FOG				0x00000100
#define BGBRASW_ENABLE_LIGHTING			0x00000200
#define BGBRASW_ENABLE_NORMALIZE		0x00000400
#define BGBRASW_ENABLE_COLOR_MATERIAL	0x00000800
#define BGBRASW_ENABLE_TEXTURE_1D		0x00001000
#define BGBRASW_ENABLE_OFFSET_FILL		0x00002000

#define BGBRASW_TEXFLAG_OPAQUE			0x00000001

#define BGBRASW_CSENABLE_VERTEX_ARRAY	0x00000001
#define BGBRASW_CSENABLE_COORD_ARRAY	0x00000002
#define BGBRASW_CSENABLE_NORMAL_ARRAY	0x00000004
#define BGBRASW_CSENABLE_COLOR_ARRAY	0x00000008
#define BGBRASW_CSENABLE_INDEX_ARRAY	0x00000010
#define BGBRASW_CSENABLE_EDGEFL_ARRAY	0x00000020

#define BGBRASW_GL_NEVER						0x0200
#define BGBRASW_GL_LESS							0x0201
#define BGBRASW_GL_EQUAL						0x0202
#define BGBRASW_GL_LEQUAL						0x0203
#define BGBRASW_GL_GREATER						0x0204
#define BGBRASW_GL_NOTEQUAL						0x0205
#define BGBRASW_GL_GEQUAL						0x0206
#define BGBRASW_GL_ALWAYS						0x0207

#define BGBRASW_GL_ZERO							0x0
#define BGBRASW_GL_ONE							0x1
#define BGBRASW_GL_SRC_COLOR					0x0300
#define BGBRASW_GL_ONE_MINUS_SRC_COLOR			0x0301
#define BGBRASW_GL_SRC_ALPHA					0x0302
#define BGBRASW_GL_ONE_MINUS_SRC_ALPHA			0x0303
#define BGBRASW_GL_DST_ALPHA					0x0304
#define BGBRASW_GL_ONE_MINUS_DST_ALPHA			0x0305
#define BGBRASW_GL_DST_COLOR					0x0306
#define BGBRASW_GL_ONE_MINUS_DST_COLOR			0x0307
#define BGBRASW_GL_SRC_ALPHA_SATURATE			0x0308
#define BGBRASW_GL_CONSTANT_COLOR				0x8001
#define BGBRASW_GL_ONE_MINUS_CONSTANT_COLOR		0x8002
#define BGBRASW_GL_CONSTANT_ALPHA				0x8003
#define BGBRASW_GL_ONE_MINUS_CONSTANT_ALPHA		0x8004

#define BGBRASW_GL_NEAREST						0x2600
#define BGBRASW_GL_LINEAR						0x2601
#define BGBRASW_GL_NEAREST_MIPMAP_NEAREST		0x2700
#define BGBRASW_GL_NEAREST_MIPMAP_LINEAR		0x2702
#define BGBRASW_GL_LINEAR_MIPMAP_NEAREST		0x2701
#define BGBRASW_GL_LINEAR_MIPMAP_LINEAR			0x2703

#define BGBRASW_GL_KEEP							0x1E00
#define BGBRASW_GL_REPLACE						0x1E01
#define BGBRASW_GL_INCR							0x1E02
#define BGBRASW_GL_DECR							0x1E03
#define BGBRASW_GL_INCR_WRAP					0x8507
#define BGBRASW_GL_DECR_WRAP					0x8508

typedef u32 bgbrasw_pixel;
typedef u32 bgbrasw_zbuf;

// typedef u64 bgbrasw_pixyyuva;	//YYYY-UVAx
// typedef u64 bgbrasw_yypixel;	//YYYY-UVAx
typedef u32 bgbrasw_yypixel;	//YY,YY,UV,Ax

typedef struct BGBRASW_Context_s BGBRASW_Context;
typedef struct BGBRASW_Texture_s BGBRASW_Texture;
typedef struct BGBRASW_Primitive_s BGBRASW_Primitive;
typedef struct BGBRASW_PrimitiveSlab_s BGBRASW_PrimitiveSlab;

typedef struct BGBRASW_TestBlendData_s BGBRASW_TestBlendData;

typedef int (*BGBRASW_TestBlendFunc_ft)(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);

typedef void (*BGBRASW_StencilOpFunc_ft)(
	BGBRASW_TestBlendData *data,
	bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);

typedef void (*BGBRASW_BlendFunc_ft)(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa);

typedef void (*BGBRASW_DrawSpanFlatBasic_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel clr);
typedef void (*BGBRASW_DrawSpanFlatInterp_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1);
typedef void (*BGBRASW_DrawSpanFlatBasicZTest_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	int xo, int yo,
	bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1);

typedef void (*BGBRASW_DrawSpanFlatInterpZTest_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1);

typedef void (*BGBRASW_DrawSpanTextureBasic_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t);
typedef void (*BGBRASW_DrawSpanTextureInterp_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1);
typedef void (*BGBRASW_DrawSpanTextureBasicZTest_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1);
typedef void (*BGBRASW_DrawSpanTextureInterpZTest_ft)(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1);

typedef void (*BGBRASW_DrawSpanFlat_ft)(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1);

typedef void (*BGBRASW_DrawSpanTex_ft)(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1);

struct BGBRASW_Context_s {
BGBRASW_Context *pctx;			//parent context (if a child ctx)
BGBRASW_Context *pnext;			//parent context (if a child ctx)
BGBRASW_Context *child;			//parent context (if a child ctx)

bgbrasw_pixel *tgt_rgba;		//target RGBA
bgbrasw_zbuf *tgt_zbuf;			//target Z-Buffer (Depth24, Stencil8)
int tgt_xs, tgt_ys;				//target size
int tgt_lxs, tgt_lys;			//target logical size

#ifdef BGBRASW_SS_YUVA
bgbrasw_yypixel *tgt_ssyuva;	//subsampled YUVA
bgbrasw_zbuf *tgt_sszbuf;		//subsampled Z-Buffer
#endif

BGBRASW_PrimitiveSlab *prislab;	//triangle slabs
BGBRASW_Primitive *prifree;	//free triangles

BGBRASW_Texture *texfree;	//free textures
BGBRASW_Texture *texlist;	//textures list

BGBRASW_TestBlendData *tabs;		//active tabs
BGBRASW_TestBlendData *tabsfree;

BGBRASW_Texture *texhash[1024];	//textures hast lists
u32 tex_alloc[1024];
int tex_alcrov;

u32 *query_buf[1024];
u32 query_cur;

void *thread;
void *th_lock;
BGBRASW_Primitive *pridraw;		//primitive draw list
BGBRASW_Primitive *pridrawe;	//primitive draw list end
volatile int priseq;			//primitive sequence number
volatile int lpriseq;			//last seen primitive sequence
volatile int status;			//status flags
};

struct BGBRASW_Texture_s {
BGBRASW_Texture *next;			//next texture
BGBRASW_Texture *hnext;			//next texture (hash next)
bgbrasw_pixel *rgba[16];		//RGBA, Mip-Levels 0-16
byte *cmptx[16];				//Compressed, Mip-Levels 0-16
int xs, ys;						//texture size
int texid;						//texture ID number
int flags;						//texture flags
int tex_min_filter;				//texture min filter
int tex_mag_filter;				//texture mag filter
int cm;							//compressed texture type
};

struct BGBRASW_Primitive_s {
BGBRASW_Primitive *next;
BGBRASW_Texture *tex;			//bound texture
BGBRASW_TestBlendData *tabs;	//Test And Blend Settings
BGBRASW_Context *ctx;			//Owning context
int xy[4][2];					//XY (pixels)
bgbrasw_zbuf z[4];				//Depth/Stencil
int w[4];
int st[4][3];					//ST (pixels)
bgbrasw_pixel rgba[4];
int bbox[2][3];					//[MinX, MinY, MinZ], MaxX, MaxY
float xyzf[4][4];
byte type;
byte mode;
int seq;						//sequence number
};

struct BGBRASW_PrimitiveSlab_s {
BGBRASW_PrimitiveSlab *next;
BGBRASW_Primitive *first;
int cnt;
};

struct BGBRASW_TestBlendData_s {
BGBRASW_TestBlendData *next;
BGBRASW_Texture *tex;			//bound texture
BGBRASW_Context *ctx;			//parent context

bgbrasw_pixel ref_clr;			//reference color
bgbrasw_zbuf ref_z;				//reference Z
bgbrasw_zbuf ref_sten;			//reference stencil
bgbrasw_pixel mask_clr;			//mask color
bgbrasw_zbuf mask_z;			//mask depth
bgbrasw_zbuf mask_sten;			//mask stencil
u32 caps_enabled;
s32 query_id;

int blend_src;
int blend_dst;
int depth_func;
int alpha_func;
int stencil_func;

int stencil_op_sfail;
int stencil_op_dpfail;
int stencil_op_dppass;

BGBRASW_Texture *tmp_tex;	//temporary texture
int tmp_fracmip;			//temporary fractional mip
int refcount;				//reference count

BGBRASW_DrawSpanFlat_ft drawSpanFlat;
BGBRASW_DrawSpanTex_ft drawSpanTex_min;
BGBRASW_DrawSpanTex_ft drawSpanTex_mag;
BGBRASW_DrawSpanTex_ft drawSpanTex_mag2;

BGBRASW_TestBlendFunc_ft testAndBlend;

BGBRASW_TestBlendFunc_ft testAlpha;
BGBRASW_TestBlendFunc_ft testDepth;
BGBRASW_TestBlendFunc_ft testStencil;

BGBRASW_BlendFunc_ft blendSfunc;
BGBRASW_BlendFunc_ft blendDfunc;

BGBRASW_StencilOpFunc_ft stenOpSfail;
BGBRASW_StencilOpFunc_ft stenOpDpFail;
BGBRASW_StencilOpFunc_ft stenOpDpPass;

bgbrasw_pixel (*doBlend)(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst);

BGBRASW_DrawSpanFlatBasic_ft			drawSpanFlatBasic;
BGBRASW_DrawSpanFlatInterp_ft			drawSpanFlatInterp;
BGBRASW_DrawSpanFlatBasicZTest_ft		drawSpanFlatBasicZTest;
BGBRASW_DrawSpanFlatInterpZTest_ft		drawSpanFlatInterpZTest;
BGBRASW_DrawSpanTextureBasic_ft			drawSpanTextureBasic;
BGBRASW_DrawSpanTextureInterp_ft		drawSpanTextureInterp;
BGBRASW_DrawSpanTextureBasicZTest_ft	drawSpanTextureBasicZTest;
BGBRASW_DrawSpanTextureInterpZTest_ft	drawSpanTextureInterpZTest;

BGBRASW_DrawSpanTextureBasic_ft			drawSpanTextureBasic_mag;
BGBRASW_DrawSpanTextureInterp_ft		drawSpanTextureInterp_mag;
BGBRASW_DrawSpanTextureBasic_ft			drawSpanTextureBasic_mag2;
BGBRASW_DrawSpanTextureInterp_ft		drawSpanTextureInterp_mag2;

BGBRASW_DrawSpanTextureBasicZTest_ft	drawSpanTextureBasicZTest_mag;
BGBRASW_DrawSpanTextureInterpZTest_ft	drawSpanTextureInterpZTest_mag;
BGBRASW_DrawSpanTextureBasicZTest_ft	drawSpanTextureBasicZTest_mag2;
BGBRASW_DrawSpanTextureInterpZTest_ft	drawSpanTextureInterpZTest_mag2;
};


static bgbrasw_pixel bgbrasw_makePixel4f(
	float r, float g, float b, float a)
{
	int cr, cg, cb, ca;
	cr=r*255;	cg=g*255;
	cb=b*255;	ca=a*255;
	cr=BGBRASW_CLAMP(cr, 0, 255);
	cg=BGBRASW_CLAMP(cg, 0, 255);
	cb=BGBRASW_CLAMP(cb, 0, 255);
	ca=BGBRASW_CLAMP(ca, 0, 255);
	return(BGBRASW_MAKEPIXEL(cr, cg, cb, ca));
}

typedef struct {
byte *cs, *cse;
byte *ct, *cte;
u32 win;
int pos;
}BGBBTJ_BitStream;


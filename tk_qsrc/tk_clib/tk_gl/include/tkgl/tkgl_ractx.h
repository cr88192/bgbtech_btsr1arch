/*
Rasterizer State

Will use 16-bit RGB555 pixels as both source and destination.
*/

/*
DrawSpan Parameter Array
*/
#define		TKGL_DS_CTX		0			//context
#define		TKGL_DS_TEXIMG	1			//texture image
#define		TKGL_DS_TPOS	2			//texture position
#define		TKGL_DS_TSTEP	3			//texture step
#define		TKGL_DS_XMASK	4			//texture position
#define		TKGL_DS_YMASK	5			//texture step
#define		TKGL_DS_CPOS	6			//color position
#define		TKGL_DS_CSTEP	7			//color step
#define		TKGL_DS_ZPOS	8			//color position
#define		TKGL_DS_ZSTEP	9			//color step
#define		TKGL_DS_NPARM	10			//number of drawspan params

/*
Edge Parameter Arrays
 */
#define		TKGL_ES_XPOS	0			//screen X position
#define		TKGL_ES_XSTEP	1			//screen X step
#define		TKGL_ES_TPOS	2			//texture position
#define		TKGL_ES_TSTEP	3			//texture step
#define		TKGL_ES_CPOS	4			//color position
#define		TKGL_ES_CSTEP	5			//color step
#define		TKGL_ES_ZPOS	6			//Z position
#define		TKGL_ES_ZSTEP	7			//Z step
#define		TKGL_ES_NPARM	8			//number of edge params

/*
Vertex Parameter Arrays
 */
#define		TKGL_VX_XPOS	0			//screen X position
#define		TKGL_VX_YPOS	1			//screen Y position
#define		TKGL_VX_ZPOS	2			//screen Z position
#define		TKGL_VX_TPOS	3			//texture position
#define		TKGL_VX_CPOS	4			//color position


typedef unsigned short	tkgl_rastpixel;
typedef unsigned short	tkgl_zbufpixel;

typedef struct TKGL_RastContext_s		TKGL_RastContext;
typedef struct TKGL_RastTexImage_s		TKGL_RastTexImage;

struct TKGL_RastContext_s
{
tkgl_rastpixel *screen_rgb;		//display buffer (RGB)
tkgl_zbufpixel *screen_zbuf;	//display buffer (Z buffer)
int		screen_xsize;
int		screen_ysize;

tkgl_rastpixel *tex_img;			//bound texture image
int		tex_xshl;
int		tex_yshl;

tkgl_rastpixel *dst_rgb;		//destination (RGB)
tkgl_zbufpixel *dst_zbuf;		//destination (Zbuf)

void (*DrawSpan)(u64 *parm,
	tkgl_rastpixel *dstc, tkgl_zbufpixel *dstz, int cnt);
void (*RasterWalkEdges)(TKGL_RastContext *ctx,
	int ytop, u64 *edge_l, u64 *edge_r, int cnt);

#if 0
u64		ds_pos;					//texture position
u64		ds_step;				//texture step
u32		ds_xmask;				//texture position
u32		ds_ymask;				//texture step

u64		ds_cpos;				//color position
u64		ds_cstep;				//color step
#endif

};

struct TKGL_RastTexImage_s
{
tkgl_rastpixel *tex_img;			//texture images
int		tex_mipofs[16];
byte	tex_xshl;					//texture X size (log-2)
byte	tex_yshl;					//texture Y size (log-2)
};

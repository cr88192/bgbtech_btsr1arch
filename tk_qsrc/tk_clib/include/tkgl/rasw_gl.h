#ifdef BGBRASW_DLL

#if !defined(GLAPI)
#if defined(_MSC_VER)
#define GLAPI __declspec(dllexport)
#else
#define GLAPI
#endif
#endif

#if !defined(GLDRV_API)
#if defined(_MSC_VER)
#define GLDRV_API __declspec(dllexport)
#else
#define GLDRV_API
#endif
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT                     0x140B
#endif

#ifndef GL_COMPRESSED_RGBA_BPTC_UNORM
#define GL_COMPRESSED_RGBA_BPTC_UNORM     0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F
#endif

#define GLSTUB_APIFPTR APIENTRY
#define GLDRV_APIENTRY APIENTRY

#else

#if !defined(GLAPI)
#if defined(_MSC_VER)
#define GLAPI __declspec(dllimport)
#else
#define GLAPI
#endif
#endif

#if !defined(GLDRV_API)
#if defined(_MSC_VER)
#define GLDRV_API __declspec(dllimport)
#else
#define GLDRV_API
#endif
#endif

#define GLSTUB_APIFPTR
#define GLDRV_APIENTRY

#endif

typedef struct RASWGL_Context_s RASWGL_Context;

struct RASWGL_Context_s {
BGBRASW_Context *ractx;
BGBRASW_Texture *texture;
BGBRASW_TestBlendData *tabs;
int texid;
int glerror;

BGBRASW_Context *ractx_thread[16];
int ractx_n_thread;

bgbrasw_pixel ref_clr;
bgbrasw_zbuf ref_z;
bgbrasw_zbuf ref_sten;
bgbrasw_pixel mask_clr;
bgbrasw_zbuf mask_z;
bgbrasw_zbuf mask_sten;
int alpha_func;

bgbrasw_pixel clear_clr;
bgbrasw_zbuf clear_z;
u32 caps_enabled;
u32 lights_enabled;
s32 query_id;

float *xyzbuf;
float *stbuf;
float *nbuf;
float *clrbuf;
// float *nxbuf;
// float *nybuf;
int nvec, mvec;

float *trans0_xyzbuf;
float *trans0_stbuf;
float *trans0_nvbuf;
float *trans0_clrbuf;
int trans0_mvec;

float *trans1_xyzbuf;
float *trans1_stbuf;
float *trans1_nvbuf;
float *trans1_clrbuf;
int trans1_mvec;

float *transN_xyzbuf;
float *transN_stbuf;
float *transN_nvbuf;
float *transN_clrbuf;
int transN_mvec;

int prim;

int blend_src;
int blend_dst;
int lastblend_src;
int lastblend_dst;

float sttmp[2];
float ntmp[4];
float clr[4];
float nxtmp[4];
float nytmp[4];

float materialfv[8][4];

float lightf[16][8];
float lightfv[16][16][4];

int init;
int viewport_x, viewport_y;
int viewport_xs, viewport_ys;

int scissor_x, scissor_y;
int scissor_xs, scissor_ys;

int view_x0, view_y0;
int view_x1, view_y1;

float depth_near, depth_far;
float depth_clear;
int depth_func;
short cull_face;
short front_face;
short polygon_mode;
short shade_model;
short draw_buffer;
short read_buffer;

int stencil_func;
int stencil_op_sfail;
int stencil_op_dpfail;
int stencil_op_dppass;

// float matrixstack[16][16];
// int matrixstackpos=-1;
// float matrix[16];

float modelview_stack[16*16];
float projection_stack[16*16];
int modelview_stackpos;
int projection_stackpos;
float modelview_matrix[16];
float projection_matrix[16];

float modelview_projection_matrix[16];
float inv_modelview_projection_matrix[16];

float clip_plane[16*4];
u16 clip_enabled;

float *matrix;
float *matrixstack;
int *matrixstackpos;

float offset_factor, offset_units;

//ClientState / DrawArray
int csda_xyzsize, csda_xyztype, csda_xyzstep;
int csda_stsize, csda_sttype, csda_ststep;
int csda_normsize, csda_normtype, csda_normstep;
int csda_rgbsize, csda_rgbtype, csda_rgbstep;
void *csda_xyz, *csda_st;
void *csda_norm, *csda_rgb;
int csda_enabled;

void *os_hdc;
void *os_hglrc;
void *os_hbmp;
void *os_rgba;
void *os_hdcdib;
void *os_hdlsel;
};

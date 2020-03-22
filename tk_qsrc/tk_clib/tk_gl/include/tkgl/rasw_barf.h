/*
Big Array Render Fill
 */
 
typedef struct RASWGL_BARF_RenderArray_s RASWGL_BARF_RenderArray;
typedef struct RASWGL_BARF_RenderContext_s RASWGL_BARF_RenderContext;

typedef float raswgl_barf_float;
typedef u16 raswgl_barf_hfloat;

typedef float raswgl_barf_xyzfloat;
// typedef u16 raswgl_barf_stfloat;
typedef float raswgl_barf_stfloat;
//typedef u16 raswgl_barf_nvfloat;
// typedef u16 raswgl_barf_clrfloat;
typedef u32 raswgl_barf_nvfloat;
typedef byte raswgl_barf_clrfloat;

struct RASWGL_BARF_RenderArray_s {
int tex;
int texflags;
int texflags2;

int n_tris, m_tris;
raswgl_barf_xyzfloat *xyz;		//XYZ
raswgl_barf_stfloat *st;			//ST
raswgl_barf_nvfloat *norm;		//XYZ (Plane)
raswgl_barf_nvfloat *xnorm;		//XYZ (S-Vector)
raswgl_barf_nvfloat *ynorm;		//XYZ (T-Vector)
raswgl_barf_clrfloat *rgba;		//RGBA (base color)
raswgl_barf_clrfloat *srgba;		//RGBA (sun-light)
raswgl_barf_clrfloat *vrgba;		//RGBA (voxel-light)
raswgl_barf_clrfloat *trgba;		//RGBA (temp-light, combined)

//VBOs
int vbo;			//VBO ID
int sz_vbo;			//VBO size

//solid face VBO
int vbo_xyz;		//XYZ
int vbo_st;			//ST
int vbo_norm;		//normals (XYZ)
int vbo_xnorm;		//X normals (XYZ)
int vbo_ynorm;		//Y normals (XYZ)
int vbo_rgba;		//base color (RGBA)
int vbo_srgba;		//sun-light color (RGBA)
int vbo_vrgba;		//vox-light color (RGBA)
int vbo_trgba;		//temp light color (RGBA)
};

struct RASWGL_BARF_RenderContext_s {
int n_array, m_array;
RASWGL_BARF_RenderArray **array;
};

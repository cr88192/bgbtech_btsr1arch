#define btm_malloc		malloc
#define btm_free		free
#define btm_realloc		realloc

// #define bccx_split	bgbcc_split

#define bccx_strdup		bgbcc_strdup
#define bccx_strdup_i	bgbcc_strdup_2i
#define bccx_stridx_i	bgbcc_strtab_i

#define BTM_LoadFile	bgbcc_loadfile
#define BTM_StoreFile	bgbcc_storefile

#define BRM_VART16_SMALLLIT		0x1000ULL
#define BRM_VART16_SMALLSTRA	0x1001ULL
#define BRM_VART16_SMALLSTRB	0x1002ULL

#define BRM_PGMVAR_PTR			(0x0ULL<<60)
#define BRM_PGMVAR_FIXNUM		(0x4ULL<<60)
#define BRM_PGMVAR_FLONUM		(0x8ULL<<60)

#define BRM_PGMVAR_SMALLLIT		(0x1000ULL<<48)
#define BRM_PGMVAR_SMALLSTRA	(0x1001ULL<<48)
#define BRM_PGMVAR_SMALLSTRB	(0x1002ULL<<48)


#define	BRM_PGMVAR_NULL			(BRM_PGMVAR_SMALLLIT+0)
#define	BRM_PGMVAR_UNDEFINED	(BRM_PGMVAR_SMALLLIT+1)

#define BRM_PGMOPR_ADD			0
#define BRM_PGMOPR_SUB			1
#define BRM_PGMOPR_MUL			2
#define BRM_PGMOPR_DIV			3
#define BRM_PGMOPR_MOD			4
#define BRM_PGMOPR_AND			5
#define BRM_PGMOPR_OR			6
#define BRM_PGMOPR_XOR			7
#define BRM_PGMOPR_SHL			8
#define BRM_PGMOPR_SHR			9

#define BRM_PGMOPR_EQ			10
#define BRM_PGMOPR_NE			11
#define BRM_PGMOPR_GT			12
#define BRM_PGMOPR_LE			13
#define BRM_PGMOPR_LT			14
#define BRM_PGMOPR_GE			15

#define BRM_PGMOPR_NOT			16
#define BRM_PGMOPR_BITNOT		17


typedef struct BTM_BtModel_s BTM_BtModel;
typedef struct BTM_BtModelMesh_s BTM_BtModelMesh;
typedef struct BTM_BtModelBone_s BTM_BtModelBone;
typedef struct BTM_BtModelAnim_s BTM_BtModelAnim;

struct BTM_BtModel_s {
BTM_BtModel *next;
char *name;
BTM_BtModelMesh *mesh[256];
BTM_BtModelBone *bone[256];
BTM_BtModelAnim *anim[256];
float lod_dist[64];
byte lod_base[64];
byte lod_cnt[64];
int n_mesh;
int n_lod;
int n_bone;
int n_anim;

byte *slab[256];
int nslab, slabpos;
};

struct BTM_BtModelMesh_s {
float *v_xy;
float *v_st;
float *v_nv;
byte *v_bn;
u32 *v_cl;
u16 *tris;
float *tri_nv;
char *matname;
u32 baseclr;
int n_vtx;
int n_tri;

float bbox[6];
};

struct BTM_BtModelBone_s {
float baserot[4];
float baseorg[4];
char *name;
byte id_parent;
byte id_solid;
};

struct BTM_BtModelAnim_s {
BTM_BtModelAnim *next;
char *name;
u32 *frm_rot;
u32 *frm_org;
u32 *frm_scl;
byte n_frames;
byte n_bones;
byte framerate;
};



#define BTM_STATUS_CONVEX	1	//mesh is convex
#define BTM_STATUS_CONCAVE	2	//mesh is concave

typedef struct {
float x;
float y;
}btm_vec2f;

typedef struct {
float x;
float y;
float z;
}btm_vec3f;

typedef struct {
float x;
float y;
float z;
float w;
}btm_vec4f;

typedef struct BTM_SolidMesh_s BTM_SolidMesh;
typedef struct BTM_SolidSkel_s BTM_SolidSkel;

typedef struct BTM_CsgBrush_s BTM_CsgBrush;
typedef struct BTM_CsgNode_s BTM_CsgNode;
typedef struct BTM_CsgPoly_s BTM_CsgPoly;

struct BTM_SolidMesh_s {
BTM_SolidMesh *next;
char *fname;		//filename
char *name;			//name of mesh
float *tris;		//triangles
float *norm;		//triangle normals
u16 *rgb5;			//triangle RGB555
int nTris;			//number of triangles
int mTris;			//max number of triangles
int status;			//status flags

float bbox[6];
float scale[3];

float baseorg[4];
float baseclr[4];
int lastcalc;
int rcrov;
int texnum;

u64 clrmat;

// float texscale_s;
// float texscale_t;
char *usetex[8];
float texvec_n[32];
float texvec_s[32];
float texvec_t[32];
int n_texvec;

float lcamorg[4];

// textured mesh
float *v_xyz;		//vertex XYZ (VA)
u32 *v_cl;			//vertex RGBA (VA)
byte *v_bn;			//vertex bone (VA)
float *v_st;		//vertex ST (VA)
float *v_nv;		//vertex ST (VA)
int *t_vidx;		//triangle vertex indices
int n_vtx;
int n_tris;

BTM_SolidSkel *skel;

};

struct BTM_SolidSkel_s {
int bone_parent[256];
char *bone_name[256];
float bone_baserot[256*4];
float bone_baseorg[256*3];
float bone_relorg[256*3];
int n_bones;

BTM_BtModelAnim *anim;
};

struct BTM_CsgBrush_s {
BTM_CsgBrush *next;
float *planes;
int n_planes;
};

struct BTM_CsgNode_s {
BTM_CsgNode *ltree;
BTM_CsgNode *rtree;
BTM_CsgBrush *brush;
u64 clrmat;
float trans[16];
int opr;
};

struct BTM_CsgPoly_s {
BTM_CsgPoly *next;
u64 clrmat;
float bbox[6];
float *pts;
int npts;
};

typedef struct BTM_PgmExt_s	BTM_PgmExt;
typedef struct BTM_ProgCtx_s	BTM_ProgCtx;
typedef struct BTM_ConPgm_s	BTM_ConPgm;

struct BTM_PgmExt_s {
BTM_PgmExt *next;
u64 (*Run)(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 (*Infix)(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 tok, u64 vala, u64 valb);
u16 token;
u16 level;
};

struct BTM_ProgCtx_s {
int pgm_n_gbls;
int pgm_m_gbls;
int pgm_dyn_pos;
int gosub_retpos;
int nxt_line;
int cur_line;

u16 *pgm_gbl_vars;
u64 *pgm_gbl_vals;
u16 pgm_dyn_vars[256];
u64 pgm_dyn_vals[256];

char *pgm_print_stdout_ct;
};

struct BTM_ConPgm_s {
BTM_ConPgm	*next;
char		*name;

u16			*ltokb;			//line token buffer
u16			*ltoki;			//line token index
u16			*lbl_id;		//label IDs
u16			*lbl_ix;		//label line index
int			n_lines;
int			n_tokens;
s16			n_lbls;

int			cur_line;
int			nxt_line;
u64			retval;

int			gosub_retidx[16];
int			gosub_retdst[16];
byte		gosub_dynidx[16];
byte		gosub_retpos;

int			loop_retidx[16];
u64			loop_endval[16];
u64			loop_stpval[16];
byte		loop_retpos;
};

void QuatF_FromAngles(float *a, float *b);
void QuatF_FromAnglesB(float *a, float *b);
void QuatF_Identity(float *a);
float QuatF_Normalize(float *a, float *b);
void QuatF_From3Matrix(float *a, float *b);

BTM_SolidMesh *BTM_LoadMeshListBuffer(byte *ibuf, int ibsz, u32 fcc);

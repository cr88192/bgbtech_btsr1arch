#define BGBCC_FCC_FR2E		BGBCC_FOURCC('F', 'R', '2', 'E')
#define BGBCC_FCC_LITD		BGBCC_FOURCC('L', 'I', 'T', 'D')
#define BGBCC_FCC_GBLD		BGBCC_FOURCC('G', 'B', 'L', 'D')
#define BGBCC_FCC_strs		BGBCC_FOURCC('s', 't', 'r', 's')
#define BGBCC_FCC_strz		BGBCC_FOURCC('s', 't', 'r', 'z')
#define BGBCC_FCC_stri		BGBCC_FOURCC('s', 't', 'r', 'i')
#define BGBCC_FCC_vops		BGBCC_FOURCC('v', 'o', 'p', 's')
#define BGBCC_FCC_vtrs		BGBCC_FOURCC('v', 't', 'r', 's')

#define BGBCC_FCC_vosi		BGBCC_FOURCC('v', 'o', 's', 'i')
#define BGBCC_FCC_vtsi		BGBCC_FOURCC('v', 't', 's', 'i')

#define BGBCC_TCC_LI		BGBCC_TWOCC('L', 'I')
#define BGBCC_TCC_RI		BGBCC_TWOCC('R', 'I')
#define BGBCC_TCC_ST		BGBCC_TWOCC('S', 'T')
#define BGBCC_TCC_UN		BGBCC_TWOCC('U', 'N')
#define BGBCC_TCC_TY		BGBCC_TWOCC('T', 'Y')
#define BGBCC_TCC_FN		BGBCC_TWOCC('F', 'N')
#define BGBCC_TCC_GV		BGBCC_TWOCC('G', 'V')
#define BGBCC_TCC_SV		BGBCC_TWOCC('S', 'V')


typedef struct BGBCC_FR2E_ImgState_s BGBCC_FR2E_ImgState;

typedef struct BGBCC_FR2E_ImgVosiEnt_s BGBCC_FR2E_ImgVosiEnt;

struct BGBCC_FR2E_ImgVosiEnt_s {
int b_vops;
int b_vtrs;
int n_vops;
int n_vtrs;
int o_vops;
int o_vtrs;
int s_vops;
int s_vtrs;
int gblid;
int flags;
};

struct BGBCC_FR2E_ImgState_s {
byte *imgdat;		//image data
int sz_imgdat;		//size of image data

char *strtab;		//string table
int sz_strtab;
int msz_strtab;

int *stridx;		//string table index
int *strnxt;		//string table index (next)
int sz_stridx;
int msz_stridx;

int strhash[256];	//string table hash

byte *stridxd;		//string table index
int sz_stridxd;
int msz_stridxd;


byte *litdat;		//literal table data
int sz_litdat;
int msz_litdat;

byte *gbldat;		//global table data
int sz_gbldat;
int msz_gbldat;

byte *vopdat;		//opcode data
int sz_vopdat;
int msz_vopdat;

byte *vtrdat;		//trace data
int sz_vtrdat;
int msz_vtrdat;

byte *vosidat;		//opcode segment index
int sz_vosidat;
int msz_vosidat;

byte *vtsidat;		//trace segment index
int sz_vtsidat;
int msz_vtsidat;

BGBCC_CCXL_VirtOp **vop;
BGBCC_CCXL_VirtTr **vtr;
BGBCC_FR2E_ImgVosiEnt **vosi;

int n_vop;
int n_vtr;
int n_vosi;

BGBCC_CCXL_RegisterInfo **gbls;
BGBCC_CCXL_LiteralInfo **lits;
int n_gbls;
int n_lits;

int stat_vop_cnt;
int stat_vop_mskb;
int stat_vop_opb;
int stat_vop_tyb;
int stat_vop_regb;
int stat_vop_immb;

int stat_vopn_ty;
int stat_vopn_reg;
int stat_vopn_imm;

int vop_lmsk;

int vopm_hist[32];
ccxl_type typehist[32];
ccxl_register reghist[64];
u64 vopi_hist[32];

};

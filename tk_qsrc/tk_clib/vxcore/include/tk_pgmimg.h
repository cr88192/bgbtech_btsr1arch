typedef struct TKPE_ImageInfo_s	TKPE_ImageInfo;
typedef struct TKPE_TaskInfo_s		TKPE_TaskInfo;

typedef struct TK_EnvContext_s		TK_EnvContext;
typedef struct TKSH_CommandInfo_s	TKSH_CommandInfo;

struct TK_EnvContext_s {
TK_EnvContext *next;

char *cwd;

char *pathbuf;
char **pathlst;
int npathlst;

char *envbufs;
char *envbufe;
char *envbufc;
char **envlst_var;
char **envlst_val;
int nenvlst;
int menvlst;
};

struct TKPE_ImageInfo_s {
int pboix;
byte *imgbase;
void *imgname;
byte *bootptr;		//entry point for image
byte *bootgbr;		//pointer to start of data section
byte *bootgbre;		//pointer to end of data section

s64 rlc_imgbase;
s64 rlc_basedisp;
byte *ptr_reloc;
int sz_reloc;

int  gbr_rva;
int  gbr_szcpy;
int  gbr_sz;
};

struct TKPE_TaskInfo_s {
void *resv0_0;		//reserved pointer
void *resv0_1;		//reserved pointer
int (*SysCall)(void *sObj, int uMsg, void *vParm1, void *vParm2);
void *resv1;		//reserved function pointer
void *resv2;		//reserved function pointer
void *resv3;		//reserved function pointer
void *resv4;		//reserved function pointer
void *resv5;		//reserved function pointer
void *resv6;		//reserved function pointer
void *resv7;		//reserved function pointer
char **tlsptr;		//pointer to TLS data area

int pid;
void *baseptr;		//base pointer for main binary
void *bootptr;		//entry point for main binary
void *basegbr;		//GBR for main binary, set on program startup.
void *boottbr;		//main process TBR (threads), self-pointer for main thread.

void *boot_sps;		//Stack Start
void *boot_spe;		//Stack End
char **argv;
// char **env;

TK_EnvContext *envctx;

u64	ctx_regsave[64];	//saved registers (context switch)

char *tlsdat[2048];	//data for TLS

void *span_ptr[512];
int span_sz[512];
int n_span;
};

struct TKSH_CommandInfo_s {
TKSH_CommandInfo *next;
TKSH_CommandInfo *hnext;
char *name;
int (*Cmd)(char **args);
};

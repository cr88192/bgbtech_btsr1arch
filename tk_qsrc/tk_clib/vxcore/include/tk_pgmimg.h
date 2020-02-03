typedef struct TKPE_ImageInfo_s	TKPE_ImageInfo;
typedef struct TKPE_TaskInfo_s		TKPE_TaskInfo;

typedef struct TK_EnvContext_s		TK_EnvContext;

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
int  gbr_szcpy;
int  gbr_sz;
};

struct TKPE_TaskInfo_s {
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

char *tlsdat[2048];	//data for TLS

void *span_ptr[512];
int span_sz[512];
int n_span;
};

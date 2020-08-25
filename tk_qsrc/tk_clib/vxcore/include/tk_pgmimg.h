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
tk_kptr		resv0_0;		//reserved pointer
tk_kptr		resv0_1;		//reserved pointer
// int (*SysCall)(void *sObj, int uMsg, void *vParm1, void *vParm2);
tk_kptr		SysCall;
tk_kptr		resv1;		//reserved function pointer
tk_kptr		resv2;		//reserved function pointer
tk_kptr		resv3;		//reserved function pointer
tk_kptr		resv4;		//reserved function pointer
tk_kptr		resv5;		//reserved function pointer
tk_kptr		resv6;		//reserved function pointer
tk_kptr		resv7;		//reserved function pointer
// char **tlsptr;		//pointer to TLS data area
tk_kptr		tlsptr;		//pointer to TLS data area

int			pid;
int			pad1;
tk_kptr		baseptr;		//base pointer for main binary
tk_kptr		bootptr;		//entry point for main binary
tk_kptr		basegbr;		//GBR for main binary, set on program startup.
tk_kptr		boottbr;		//main process TBR (threads), self-pointer for main thread.

tk_kptr		boot_sps;		//Stack Start
tk_kptr		boot_spe;		//Stack End
// char **argv;
tk_kptr		argv;
// char **env;

// TK_EnvContext *envctx;
tk_kptr		envctx;

u64			ctx_regsave[64];	//saved registers (context switch)

u64			tlsdat[2048];	//data for TLS

tk_kptr		span_ptr[512];	//span	(mmap / VirtualAlloc)
int			span_sz[512];	//span	(mmap / VirtualAlloc)
int			n_span;			//span	(mmap / VirtualAlloc)
};

struct TKSH_CommandInfo_s {
TKSH_CommandInfo *next;
TKSH_CommandInfo *hnext;
char *name;
int (*Cmd)(char **args);
};

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
tk_kptr		resv0_0;	//00, reserved pointer
tk_kptr		resv0_1;	//08, reserved pointer
tk_kptr		SysCall;	//10, System Call (Entry Point)
tk_kptr		resv1;		//18, reserved function pointer
tk_kptr		resv2;		//20, reserved function pointer
tk_kptr		resv3;		//28, reserved function pointer
tk_kptr		eh_obj;		//30, Exception Object
tk_kptr		eh_vars;	//38, Exception Unwind Vars (Array Pointer)
tk_kptr		eh_oldlr;	//40, Saved LR (Exception Unwind)
tk_kptr		allocaptr;	//48, pointer to alloca mark
tk_kptr		tlsptr;		//50, pointer to TLS data area

int			pid;		//58
short		uid;		//5C
short		gid;		//5E

tk_kptr		baseptr;		//60, base pointer for main binary
tk_kptr		bootptr;		//68, entry point for main binary
tk_kptr		basegbr;		//70, GBR for main binary, set on program startup.
tk_kptr		boottbr;		//78, main process TBR (threads) | self-pointer.

tk_kptr		boot_sps;		//80, Stack Start
tk_kptr		boot_spe;		//88, Stack End
tk_kptr		argv;			//90, argument string
tk_kptr		envctx;			//98, Environment Context

tk_kptr		resv4;			//A0, reserved pointer
tk_kptr		resv5;			//A8, reserved pointer
tk_kptr		resv6;			//B0, reserved pointer
tk_kptr		resv7;			//B8, reserved pointer
tk_kptr		resv8;			//C0, reserved pointer
tk_kptr		resv9;			//C8, reserved pointer
tk_kptr		resv10;			//D0, reserved pointer
tk_kptr		resv11;			//D8, reserved pointer
tk_kptr		resv12;			//E0, reserved pointer
tk_kptr		resv13;			//E8, reserved pointer
tk_kptr		resv14;			//F0, reserved pointer
tk_kptr		resv15;			//F8, reserved pointer

//End of fixed area.

// u64			ctx_regsave[64];	//saved registers (context switch)
u64			ctx_regsave[128];	//saved registers (context switch)

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

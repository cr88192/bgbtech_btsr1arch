#define TKPE_REGSAVE_R0		0x00
#define TKPE_REGSAVE_R1		0x01
#define TKPE_REGSAVE_R2		0x02
#define TKPE_REGSAVE_R3		0x03
#define TKPE_REGSAVE_R4		0x04
#define TKPE_REGSAVE_R5		0x05
#define TKPE_REGSAVE_R6		0x06
#define TKPE_REGSAVE_R7		0x07
#define TKPE_REGSAVE_R8		0x08
#define TKPE_REGSAVE_R9		0x09
#define TKPE_REGSAVE_R10	0x0A
#define TKPE_REGSAVE_R11	0x0B
#define TKPE_REGSAVE_R12	0x0C
#define TKPE_REGSAVE_R13	0x0D
#define TKPE_REGSAVE_R14	0x0E
#define TKPE_REGSAVE_R15	0x0F

#define TKPE_REGSAVE_SSP	0x0F


#define TKPE_REGSAVE_GBR	0x40
#define TKPE_REGSAVE_LR		0x41
#define TKPE_REGSAVE_SPC	0x42
#define TKPE_REGSAVE_EXSR	0x43

#define TKPE_REGSAVE_TBR	0x44
#define TKPE_REGSAVE_TTB	0x45

#define TKPE_REGSAVE_KRR	0x48

typedef struct TKPE_ImageInfo_s		TKPE_ImageInfo;

typedef struct TKPE_TaskInfo_s			TKPE_TaskInfo;
typedef struct TKPE_TaskInfoUser_s		TKPE_TaskInfoUser;
typedef struct TKPE_TaskInfoKern_s		TKPE_TaskInfoKern;

typedef struct TK_EnvContext_s			TK_EnvContext;
typedef struct TKSH_CommandInfo_s		TKSH_CommandInfo;

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
int imgix;
int isdll;

char *imgname;		//image name, no path or extension
char *imgpath;		//full image path
byte *imgbase;
byte *bootptr;		//entry point for image
byte *bootgbr;		//pointer to start of data section
byte *bootgbre;		//pointer to end of data section

s64 rlc_imgbase;
s64 rlc_basedisp;
byte *ptr_reloc;
int sz_reloc;

int rva_imp;
int sz_imp;
int rva_exp;
int sz_exp;

int  gbr_rva;
int  gbr_szcpy;
int  gbr_sz;

int  tls_rva;
int  tls_dsize;
int  tls_key;
int  tls_rvaraw;
int  tls_szraw;

TKPE_ImageInfo *dll[256];
int n_dll;
};

#define TKPE_TASK_MAGIC		0x14263748

/*
The TaskInfoUser area will contain data which may be modified by the running program.
 */
struct TKPE_TaskInfoUser_s {
tk_kptr		resv_00;		//00, reserved pointer
tk_kptr		resv_08;		//08, reserved pointer
tk_kptr		resv_10;		//10, reserved pointer
tk_kptr		resv_18;		//18, reserved pointer
tk_kptr		resv_20;		//20, reserved pointer
tk_kptr		resv_28;		//28, reserved pointer
tk_kptr		eh_obj;			//30, Exception Object
tk_kptr		eh_vars;		//38, Exception Unwind Vars (Array Pointer)
tk_kptr		eh_oldlr;		//40, Saved LR (Exception Unwind)
tk_kptr		allocamrk;		//48, pointer to alloca mark
tk_kptr		tlsptr;			//50, pointer to TLS data area
tk_kptr		clib_gpa;		//58, C Library, GetProcAddress
tk_kptr		magic0;			//60, magic pointer
tk_kptr		resv_68;		//68, reserved pointer
tk_kptr		resv_70;		//70, reserved pointer
tk_kptr		resv_78;		//78, reserved pointer
tk_kptr		resv_80;		//80, reserved pointer
tk_kptr		resv_88;		//88, reserved pointer
tk_kptr		resv_90;		//90, reserved pointer
tk_kptr		resv_98;		//98, reserved pointer
tk_kptr		resv_A0;		//A0, reserved pointer
tk_kptr		resv_A8;		//A8, reserved pointer
tk_kptr		resv_B0;		//B0, reserved pointer
tk_kptr		resv_B8;		//B8, reserved pointer
tk_kptr		resv_C0;		//C0, reserved pointer
tk_kptr		resv_C8;		//C8, reserved pointer
tk_kptr		resv_D0;		//D0, reserved pointer
tk_kptr		resv_D8;		//D8, reserved pointer
tk_kptr		resv_E0;		//E0, reserved pointer
tk_kptr		resv_E8;		//E8, reserved pointer
tk_kptr		resv_F0;		//F0, reserved pointer
tk_kptr		resv_F8;		//F8, reserved pointer

tk_kptr		sharebuf[32];	//100, Shared Buffers

//End of fixed area.

u64			tlsdat[2048];	//data for TLS
};

/*
The TaskInfoKern area will contain data which is kernel only.
 */
struct TKPE_TaskInfoKern_s {
tk_kptr		resv_00;			//00, reserved pointer
tk_kptr		resv_08;			//08, reserved pointer
tk_kptr		resv_10;			//10, reserved pointer
tk_kptr		resv_18;			//18, reserved pointer
tk_kptr		usr_boot_sps;		//20, Stack Start (Bottom), User
tk_kptr		usr_boot_spe;		//28, Stack End (Top)
tk_kptr		krn_boot_sps;		//30, Stack Start (Bottom), Kernel
tk_kptr		krn_boot_spe;		//38, Stack End (Top)

u64			ctx_regsave[128];	//saved registers (context switch)
u64			ctx_stksave[128];	//top of stack

tk_kptr		magic0;				//40, magic key

tk_kptr		span_ptr[512];	//span	(mmap / VirtualAlloc)
int			span_sz[512];	//span	(mmap / VirtualAlloc)
int			n_span;			//span	(mmap / VirtualAlloc)


tk_kptr		mmap_ptr[512];	//mmap base address
int			mmap_len[512];	//mmap size
int			mmap_prot[512];	//mmap prot
int			mmap_flag[512];	//mmap flag
int			mmap_n_map;			//span	(mmap / VirtualAlloc)

tk_kptr		task_sysc_user;		//task that initiated syscall.
tk_kptr		task_join_ret;		//task join-on-return

// s64			result;			//return value (non-zero if terminated)
};

/*
The TaskInfo area will be Read-Only to the running program.
The program needs to be able to see these parameters, but should not be allowed to modify them.
 */
struct TKPE_TaskInfo_s {
tk_kptr		resv_00;		//00, reserved pointer
tk_kptr		resv_08;		//08, reserved pointer
tk_kptr		SysCall;		//10, System Call (Entry Point)
tk_kptr		resv_18;		//18, reserved function pointer
tk_kptr		resv_20;		//20, reserved function pointer
tk_kptr		resv_28;		//28, reserved function pointer
tk_kptr		krnlptr;		//30, pointer to kernel-only area
tk_kptr		ystatus;		//38, Sleep / Terminate Status
tk_kptr		usrptr;			//40, pointer to user-modifiable area
tk_kptr		allocaptr;		//48, pointer to alloca mark
tk_kptr		tlsptr;			//50, pointer to TLS data area

u16			pid;			//58, Process ID
u16			sch_id;			//5A, Scheduler ID
u16			uid;			//5C
u16			gid;			//5E

tk_kptr		baseptr;		//60, base pointer for main binary
tk_kptr		bootptr;		//68, entry point for main binary
tk_kptr		basegbr;		//70, GBR for main binary, set on program startup.
tk_kptr		boottbr;		//78, main process TBR (threads) | self-pointer.

tk_kptr		boot_sps;		//80, Stack Start (Bottom)
tk_kptr		boot_spe;		//88, Stack End (Top)
tk_kptr		argv;			//90, argument string (main thread), uptr (thread)
tk_kptr		envctx;			//98, Environment Context

tk_kptr		imgbaseptrs;	//A0, list of loaded image Base Pointers.
tk_kptr		imggbrptrs;		//A8, list of loaded image GBR Pointers
tk_kptr		imgtlsrvas;		//B0, list of loaded image TLS RVAs.
tk_kptr		resv_B8;		//B8, reserved pointer
tk_kptr		resv_C0;		//C0, reserved pointer
tk_kptr		resv_C8;		//C8, reserved pointer
tk_kptr		resv_D0;		//D0, reserved pointer
tk_kptr		resv_D8;		//D8, reserved pointer
tk_kptr		resv_E0;		//E0, reserved pointer
tk_kptr		resv_E8;		//E8, reserved pointer
tk_kptr		resv_F0;		//F0, reserved pointer
tk_kptr		resv_F8;		//F8, reserved pointer

//End of fixed area.

tk_kptr		magic0;			//100, key magic pointer

int			n_tlsv;			//number of allocated TLS vars
int			n_dllimg;		//number of loaded DLL images
int			m_imgptrs;		//Maximum Image Pointers

byte		status;			//Status, 0=Running
byte		prio;			//Priority Level
byte		qtick;			//Schedule Tick

tk_kptr		img_baseptrs[256];
tk_kptr		img_gbrptrs[256];
u32			img_tlsrvas[256];

};

struct TKSH_CommandInfo_s {
TKSH_CommandInfo *next;
TKSH_CommandInfo *hnext;
char *name;
int (*Cmd)(char **args);
};

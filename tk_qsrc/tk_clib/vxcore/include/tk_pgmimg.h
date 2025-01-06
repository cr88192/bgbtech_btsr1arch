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

#define TKPE_REGSAVE_R16	0x10
#define TKPE_REGSAVE_R17	0x11
#define TKPE_REGSAVE_R18	0x12
#define TKPE_REGSAVE_R19	0x13
#define TKPE_REGSAVE_R20	0x14
#define TKPE_REGSAVE_R21	0x15
#define TKPE_REGSAVE_R22	0x16
#define TKPE_REGSAVE_R23	0x17
#define TKPE_REGSAVE_R24	0x18
#define TKPE_REGSAVE_R25	0x19
#define TKPE_REGSAVE_R26	0x1A
#define TKPE_REGSAVE_R27	0x1B
#define TKPE_REGSAVE_R28	0x1C
#define TKPE_REGSAVE_R29	0x1D
#define TKPE_REGSAVE_R30	0x1E
#define TKPE_REGSAVE_R31	0x1F

#define TKPE_REGSAVE_SSP	0x0F


#define TKPE_REGSAVE_GBR_LO		0x20
#define TKPE_REGSAVE_LR_LO		0x21
#define TKPE_REGSAVE_SPC_LO		0x22
#define TKPE_REGSAVE_EXSR_LO	0x23


#define TKPE_REGSAVE_GBR	0x40
#define TKPE_REGSAVE_LR		0x41
#define TKPE_REGSAVE_SPC	0x42
#define TKPE_REGSAVE_EXSR	0x43

#define TKPE_REGSAVE_TBR	0x44
#define TKPE_REGSAVE_TTB	0x45

#define TKPE_REGSAVE_PCH	0x46
#define TKPE_REGSAVE_GBH	0x47

#define TKPE_REGSAVE_KRR	0x48


/* ELF Aux Vectors */
#define TKPE_ELF_AT_NULL				0
#define TKPE_ELF_AT_IGNORE				1
#define TKPE_ELF_AT_EXECFD				2
#define TKPE_ELF_AT_PHDR				3
#define TKPE_ELF_AT_PHENT				4
#define TKPE_ELF_AT_PHNUM				5
#define TKPE_ELF_AT_PAGESZ				6
#define TKPE_ELF_AT_BASE				7
#define TKPE_ELF_AT_FLAGS				8
#define TKPE_ELF_AT_ENTRY				9
#define TKPE_ELF_AT_NOTELF				10
#define TKPE_ELF_AT_UID					11
#define TKPE_ELF_AT_EUID				12
#define TKPE_ELF_AT_GID					13
#define TKPE_ELF_AT_EGID				14
#define TKPE_ELF_AT_PLATFORM			15
#define TKPE_ELF_AT_HWCAP				16
#define TKPE_ELF_AT_CLKTCK				17
#define TKPE_ELF_AT_FPUCW				18
#define TKPE_ELF_AT_DCACHEBSIZE			19
#define TKPE_ELF_AT_ICACHEBSIZE			20
#define TKPE_ELF_AT_UCACHEBSIZE			21
#define TKPE_ELF_AT_IGNOREPPC			22
#define	TKPE_ELF_AT_SECURE				23
#define TKPE_ELF_AT_BASE_PLATFORM		24
#define TKPE_ELF_AT_RANDOM				25
#define TKPE_ELF_AT_HWCAP2				26

#define TKPE_ELF_AT_EXECFN				31
#define TKPE_ELF_AT_SYSINFO				32
#define TKPE_ELF_AT_SYSINFO_EHDR		33
#define TKPE_ELF_AT_L1I_CACHESHAPE		34
#define TKPE_ELF_AT_L1D_CACHESHAPE		35
#define TKPE_ELF_AT_L2_CACHESHAPE		36
#define TKPE_ELF_AT_L3_CACHESHAPE		37

#define TKPE_ELF_AT_L1I_CACHESIZE		40
#define TKPE_ELF_AT_L1I_CACHEGEOMETRY	41
#define TKPE_ELF_AT_L1D_CACHESIZE		42
#define TKPE_ELF_AT_L1D_CACHEGEOMETRY	43
#define TKPE_ELF_AT_L2_CACHESIZE		44
#define TKPE_ELF_AT_L2_CACHEGEOMETRY	45
#define TKPE_ELF_AT_L3_CACHESIZE		46
#define TKPE_ELF_AT_L3_CACHEGEOMETRY	47

#define TKPE_ELF_AT_MINSIGSTKSZ			51


typedef struct TKPE_ImageInfo_s		TKPE_ImageInfo;

typedef struct TKPE_TaskInfo_s			TKPE_TaskInfo;
typedef struct TKPE_TaskInfoUser_s		TKPE_TaskInfoUser;
typedef struct TKPE_TaskInfoKern_s		TKPE_TaskInfoKern;
typedef struct TKPE_CreateTaskInfo_s	TKPE_CreateTaskInfo;

typedef struct TK_EnvContext_s			TK_EnvContext;
typedef struct TKSH_CommandInfo_s		TKSH_CommandInfo;

struct TK_EnvContext_s {
TK_EnvContext *next;

// char *cwd;

//char *pathbuf;
//char **pathlst;
//int npathlst;

u32 magic1;

char *envbufs;
char *envbufe;
char *envbufc;
char **envlst_var;
char **envlst_val;
short *envlst_chn;
int nenvlst;
int menvlst;

short envlst_hash[256];

u32 magic2;
};

struct TKPE_ImageInfo_s {
int pboix;
int imgix;
byte isdll;
byte iself;

char *imgname;		//image name, no path or extension
char *imgpath;		//full image path
byte *imgbase;
byte *bootptr;		//entry point for image
byte *bootgbr;		//pointer to start of data section
byte *bootgbre;		//pointer to end of data section

byte *realentry;		//real entry point
TKPE_ImageInfo *elf_interpimg;	//elf interpreter base
byte *elf_interpbase;	//elf interpreter base

byte *elf_phdr_ptr;
int elf_phdr_phoff;
short elf_phdr_phentsz;
short elf_phdr_phnum;

s64 rlc_imgbase;
s64 rlc_basedisp;
byte *ptr_reloc;
int sz_reloc;

s64 dllflags;

int rva_imp;
int sz_imp;
int rva_exp;
int sz_exp;

int rva_symtab;
int rva_strtab;
int sz_syment;

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
tk_kptr		gdictx;			//68, GDI Context
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
tk_kptr		syscmsgbuf[64];	//200, System Call Message Buffer

byte		kbfifo_dat[256];
byte		kbfifo_beg;
byte		kbfifo_end;

//End of fixed area.

u64			tlsdat[2048];	//data for TLS
u64			tls_freelistarray[256];
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

int			lclfd[512];

// s64			result;			//return value (non-zero if terminated)
};

/*
The TaskInfo area will be Read-Only to the running program.
The program needs to be able to see these parameters, but should not be allowed to modify them.
 */
struct TKPE_TaskInfo_s {
tk_kptr		resv_00;		//000, reserved pointer
tk_kptr		resv_08;		//008, reserved pointer
tk_kptr		SysCall;		//010, System Call (Entry Point)
tk_kptr		resv_18;		//018, reserved function pointer
tk_kptr		regsave;		//020, pointer to register save area
tk_kptr		resv_28;		//028, reserved pointer
tk_kptr		krnlptr;		//030, pointer to kernel-only area
tk_kptr		ystatus;		//038, Sleep / Terminate Status
tk_kptr		usrptr;			//040, pointer to user-modifiable area
tk_kptr		allocaptr;		//048, pointer to alloca mark
tk_kptr		tlsptr;			//050, pointer to TLS data area

u16			pid;			//058, Process ID
u16			sch_id;			//05A, Scheduler ID
u16			uid;			//05C
u16			gid;			//05E

tk_kptr		baseptr;		//060, base pointer for main binary
tk_kptr		bootptr;		//068, entry point for main binary
tk_kptr		basegbr;		//070, GBR for main binary, set on program startup.
tk_kptr		boottbr;		//078, main process TBR (threads) | self-pointer.

tk_kptr		boot_sps;		//080, Stack Start (Bottom)
tk_kptr		boot_spe;		//088, Stack End (Top)
tk_kptr		argv;			//090, argument string (main thread), uptr (thread)
tk_kptr		envctx;			//098, Environment Context

tk_kptr		imgbaseptrs;	//0A0, list of loaded image Base Pointers.
tk_kptr		imggbrptrs;		//0A8, list of loaded image GBR Pointers
tk_kptr		imgtlsrvas;		//0B0, list of loaded image TLS RVAs.
tk_kptr		resv_B8;		//0B8, reserved
tk_kptr		resv_C0;		//0C0, reserved pointer
tk_kptr		resv_C8;		//0C8, reserved pointer
tk_kptr		tls_freelist;	//0D0, thread local free list
tk_kptr		resv_D8;		//0D8, reserved pointer
tk_kptr		resv_E0;		//0E0, reserved pointer
tk_kptr		resv_E8;		//0E8, reserved pointer
tk_kptr		resv_F0;		//0F0, reserved pointer
tk_kptr		resv_F8;		//0F8, reserved pointer

tk_kptr		resv_100;		//100, reserved pointer
tk_kptr		resv_108;		//108, reserved pointer
tk_kptr		resv_110;		//110, reserved pointer
tk_kptr		resv_118;		//118, reserved pointer
tk_kptr		resv_120;		//120, reserved pointer
tk_kptr		resv_128;		//128, reserved pointer
tk_kptr		resv_130;		//130, reserved pointer
tk_kptr		resv_138;		//138, reserved pointer
tk_kptr		resv_140;		//140, reserved pointer
tk_kptr		resv_148;		//148, reserved pointer
tk_kptr		resv_150;		//150, reserved pointer
tk_kptr		resv_158;		//158, reserved pointer
tk_kptr		resv_160;		//160, reserved pointer
tk_kptr		resv_168;		//168, reserved pointer
tk_kptr		resv_170;		//170, reserved pointer
tk_kptr		resv_178;		//178, reserved pointer

tk_kptr		resv_180;		//180, reserved pointer
tk_kptr		resv_188;		//188, reserved pointer
tk_kptr		resv_190;		//190, reserved pointer
tk_kptr		resv_198;		//198, reserved pointer
tk_kptr		resv_1A0;		//1A0, reserved pointer
tk_kptr		resv_1A8;		//1A8, reserved pointer
tk_kptr		resv_1B0;		//1B0, reserved pointer
tk_kptr		resv_1B8;		//1B8, reserved pointer
tk_kptr		resv_1C0;		//1C0, reserved pointer
tk_kptr		resv_1C8;		//1C8, reserved pointer
tk_kptr		resv_1D0;		//1D0, reserved pointer
tk_kptr		resv_1D8;		//1D8, reserved pointer
tk_kptr		resv_1E0;		//1E0, reserved pointer
tk_kptr		resv_1E8;		//1E8, reserved pointer
tk_kptr		resv_1F0;		//1F0, reserved pointer
tk_kptr		resv_1F8;		//1F8, reserved pointer

//200, End of fixed area.

tk_kptr		magic0;			//key magic pointer

int			n_tlsv;			//number of allocated TLS vars
int			n_dllimg;		//number of loaded DLL images
int			m_imgptrs;		//Maximum Image Pointers

byte		status;			//Status, 0=Running
byte		prio;			//Priority Level
byte		qtick;			//Schedule Tick

byte		stdin_eof;		//EOF (for stdin redirect)

int			ttyid;			//terminal for console printing

int			redir_stdin;	//stdin redirect
int			redir_stdout;	//stdout redirect

int			next_redir_stdin;	//next stdin redirect (createproces)
int			next_redir_stdout;	//next stdout redirect

u64			us_lastsleep;	//last sleep time for task

tk_kptr		img_baseptrs[256];
tk_kptr		img_gbrptrs[256];
u32			img_tlsrvas[256];

u64 		img_elftls[256];

};

struct TKPE_CreateTaskInfo_s {
int szInfo;		//size of this structure.
int idTty;		//console ID, 0 for default.
int szStack;	//requested stack size (bytes), 0 for default
int pad0;		//
u64	flSched;	//scheduler flags
int rdStdin;	//redirect stdin (0 for console/terminal)
int rdStdout;	//redirect stdout (0 for console/terminal)
// char *strDoCmdLine;		//command-line (CreateProcess)
// char *strDoEnv;			//environment (CreateProcess)
};

struct TKSH_CommandInfo_s {
TKSH_CommandInfo *next;
TKSH_CommandInfo *hnext;
char *name;
char *desc;
int (*Cmd)(char **args);
};

typedef struct tk_hugeptr_s tk_hugeptr;

struct tk_hugeptr_s {
u64 addr_lo;
u64 addr_hi;
};

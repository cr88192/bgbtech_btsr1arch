#define X3PE_REGSAVE_R0		0x00
#define X3PE_REGSAVE_R1		0x01
#define X3PE_REGSAVE_R2		0x02
#define X3PE_REGSAVE_R3		0x03
#define X3PE_REGSAVE_R4		0x04
#define X3PE_REGSAVE_R5		0x05
#define X3PE_REGSAVE_R6		0x06
#define X3PE_REGSAVE_R7		0x07
#define X3PE_REGSAVE_R8		0x08
#define X3PE_REGSAVE_R9		0x09
#define X3PE_REGSAVE_R10	0x0A
#define X3PE_REGSAVE_R11	0x0B
#define X3PE_REGSAVE_R12	0x0C
#define X3PE_REGSAVE_R13	0x0D
#define X3PE_REGSAVE_R14	0x0E
#define X3PE_REGSAVE_R15	0x0F

#define X3PE_REGSAVE_R16	0x10
#define X3PE_REGSAVE_R17	0x11
#define X3PE_REGSAVE_R18	0x12
#define X3PE_REGSAVE_R19	0x13
#define X3PE_REGSAVE_R20	0x14
#define X3PE_REGSAVE_R21	0x15
#define X3PE_REGSAVE_R22	0x16
#define X3PE_REGSAVE_R23	0x17
#define X3PE_REGSAVE_R24	0x18
#define X3PE_REGSAVE_R25	0x19
#define X3PE_REGSAVE_R26	0x1A
#define X3PE_REGSAVE_R27	0x1B
#define X3PE_REGSAVE_R28	0x1C
#define X3PE_REGSAVE_R29	0x1D
#define X3PE_REGSAVE_R30	0x1E
#define X3PE_REGSAVE_R31	0x1F

#define X3PE_REGSAVE_SSP	0x0F


#define X3PE_REGSAVE_GBR	0x40
#define X3PE_REGSAVE_LR		0x41
#define X3PE_REGSAVE_SPC	0x42
#define X3PE_REGSAVE_EXSR	0x43

#define X3PE_REGSAVE_TBR	0x44
#define X3PE_REGSAVE_TTB	0x45

#define X3PE_REGSAVE_PCH	0x46
#define X3PE_REGSAVE_GBH	0x47

#define X3PE_REGSAVE_KRR	0x48


/* ELF Aux Vectors */
#define X3PE_ELF_AT_NULL				0
#define X3PE_ELF_AT_IGNORE				1
#define X3PE_ELF_AT_EXECFD				2
#define X3PE_ELF_AT_PHDR				3
#define X3PE_ELF_AT_PHENT				4
#define X3PE_ELF_AT_PHNUM				5
#define X3PE_ELF_AT_PAGESZ				6
#define X3PE_ELF_AT_BASE				7
#define X3PE_ELF_AT_FLAGS				8
#define X3PE_ELF_AT_ENTRY				9
#define X3PE_ELF_AT_NOTELF				10
#define X3PE_ELF_AT_UID					11
#define X3PE_ELF_AT_EUID				12
#define X3PE_ELF_AT_GID					13
#define X3PE_ELF_AT_EGID				14
#define X3PE_ELF_AT_PLATFORM			15
#define X3PE_ELF_AT_HWCAP				16
#define X3PE_ELF_AT_CLKTCK				17
#define X3PE_ELF_AT_FPUCW				18
#define X3PE_ELF_AT_DCACHEBSIZE			19
#define X3PE_ELF_AT_ICACHEBSIZE			20
#define X3PE_ELF_AT_UCACHEBSIZE			21
#define X3PE_ELF_AT_IGNOREPPC			22
#define	X3PE_ELF_AT_SECURE				23
#define X3PE_ELF_AT_BASE_PLATFORM		24
#define X3PE_ELF_AT_RANDOM				25
#define X3PE_ELF_AT_HWCAP2				26

#define X3PE_ELF_AT_EXECFN				31
#define X3PE_ELF_AT_SYSINFO				32
#define X3PE_ELF_AT_SYSINFO_EHDR		33
#define X3PE_ELF_AT_L1I_CACHESHAPE		34
#define X3PE_ELF_AT_L1D_CACHESHAPE		35
#define X3PE_ELF_AT_L2_CACHESHAPE		36
#define X3PE_ELF_AT_L3_CACHESHAPE		37

#define X3PE_ELF_AT_L1I_CACHESIZE		40
#define X3PE_ELF_AT_L1I_CACHEGEOMETRY	41
#define X3PE_ELF_AT_L1D_CACHESIZE		42
#define X3PE_ELF_AT_L1D_CACHEGEOMETRY	43
#define X3PE_ELF_AT_L2_CACHESIZE		44
#define X3PE_ELF_AT_L2_CACHEGEOMETRY	45
#define X3PE_ELF_AT_L3_CACHESIZE		46
#define X3PE_ELF_AT_L3_CACHEGEOMETRY	47

#define X3PE_ELF_AT_MINSIGSTKSZ			51


typedef struct X3PE_ImageInfo_s		X3PE_ImageInfo;

typedef struct X3PE_TaskInfo_s			X3PE_TaskInfo;
typedef struct X3PE_TaskInfoUser_s		X3PE_TaskInfoUser;
typedef struct X3PE_TaskInfoKern_s		X3PE_TaskInfoKern;
typedef struct X3PE_CreateTaskInfo_s	X3PE_CreateTaskInfo;

#if 0
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
#endif

struct X3PE_ImageInfo_s {
int pboix;
int imgix;
byte isdll;
byte iself;

char *imgname;		//image name, no path or extension
char *imgpath;		//full image path
x3vm_addr imgbase;
x3vm_addr bootptr;		//entry point for image
x3vm_addr bootgbr;		//pointer to start of data section
x3vm_addr bootgbre;		//pointer to end of data section

byte *realentry;		//real entry point
X3PE_ImageInfo *elf_interpimg;	//elf interpreter base
byte *elf_interpbase;	//elf interpreter base

byte *elf_phdr_ptr;
int elf_phdr_phoff;
short elf_phdr_phentsz;
short elf_phdr_phnum;

s64 rlc_imgbase;
s64 rlc_basedisp;
x3vm_addr ptr_reloc;
int sz_reloc;

s64 dllflags;

int rva_imp;
int sz_imp;
int rva_exp;
int sz_exp;

int rva_symtab;
int rva_strtab;
int sz_syment;

int imgsz;
int gbr_rva;
int gbr_szcpy;
int gbr_sz;

int  tls_rva;
int  tls_dsize;
int  tls_key;
int  tls_rvaraw;
int  tls_szraw;

X3PE_ImageInfo *dll[256];
int n_dll;

int *map_rva;
char **map_sym;
char *map_sch;
int *map_lnrva;
char **map_lnsym;
int map_nsym;
int map_nline;

int *stab_rva;
int *stab_tde;
char **stab_sym;
int map_nstab;
};

#define X3PE_TASK_MAGIC		0x14263748

/*
The TaskInfoUser area will contain data which may be modified by the running program.
 */
struct X3PE_TaskInfoUser_s {
u64		resv_00;		//00, reserved pointer
u64		resv_08;		//08, reserved pointer
u64		resv_10;		//10, reserved pointer
u64		resv_18;		//18, reserved pointer
u64		resv_20;		//20, reserved pointer
u64		resv_28;		//28, reserved pointer
u64		eh_obj;			//30, Exception Object
u64		eh_vars;		//38, Exception Unwind Vars (Array Pointer)
u64		eh_oldlr;		//40, Saved LR (Exception Unwind)
u64		allocamrk;		//48, pointer to alloca mark
u64		tlsptr;			//50, pointer to TLS data area
u64		clib_gpa;		//58, C Library, GetProcAddress
u64		magic0;			//60, magic pointer
u64		gdictx;			//68, GDI Context
u64		resv_70;		//70, reserved pointer
u64		resv_78;		//78, reserved pointer
u64		resv_80;		//80, reserved pointer
u64		resv_88;		//88, reserved pointer
u64		resv_90;		//90, reserved pointer
u64		resv_98;		//98, reserved pointer
u64		resv_A0;		//A0, reserved pointer
u64		resv_A8;		//A8, reserved pointer
u64		resv_B0;		//B0, reserved pointer
u64		resv_B8;		//B8, reserved pointer
u64		resv_C0;		//C0, reserved pointer
u64		resv_C8;		//C8, reserved pointer
u64		resv_D0;		//D0, reserved pointer
u64		resv_D8;		//D8, reserved pointer
u64		resv_E0;		//E0, reserved pointer
u64		resv_E8;		//E8, reserved pointer
u64		resv_F0;		//F0, reserved pointer
u64		resv_F8;		//F8, reserved pointer

u64		sharebuf[32];	//100, Shared Buffers
u64		syscmsgbuf[64];	//200, System Call Message Buffer

// u32		scstub[256];	//400, system-call stubs
// u64		scvtab[64];		//800, system call vtable (generic)

// byte		kbfifo_dat[256];
// byte		kbfifo_beg;
// byte		kbfifo_end;

//End of fixed area.

u64			tls_freelistarray[256];
u64			tlsdat[1024];	//data for TLS
};

/*
The TaskInfoKern area will contain data which is kernel only.
 */
struct X3PE_TaskInfoKern_s {
u64		resv_00;			//00, reserved pointer
u64		resv_08;			//08, reserved pointer
u64		resv_10;			//10, reserved pointer
u64		resv_18;			//18, reserved pointer
u64		usr_boot_sps;		//20, Stack Start (Bottom), User
u64		usr_boot_spe;		//28, Stack End (Top)
u64		krn_boot_sps;		//30, Stack Start (Bottom), Kernel
u64		krn_boot_spe;		//38, Stack End (Top)

u64		ctx_regsave[128];	//saved registers (context switch)
u64		ctx_stksave[128];	//top of stack

u64		magic0;				//40, magic key

u64		span_ptr[512];	//span	(mmap / VirtualAlloc)
int		span_sz[512];	//span	(mmap / VirtualAlloc)
int		n_span;			//span	(mmap / VirtualAlloc)


u64		mmap_ptr[512];	//mmap base address
int		mmap_len[512];	//mmap size
int		mmap_prot[512];	//mmap prot
int		mmap_flag[512];	//mmap flag
int		mmap_n_map;			//span	(mmap / VirtualAlloc)

u64		task_sysc_user;		//task that initiated syscall.
u64		task_join_ret;		//task join-on-return

int		lclfd[512];
};

/*
The TaskInfo area will be Read-Only to the running program.
The program needs to be able to see these parameters, but should not be allowed to modify them.
 */
struct X3PE_TaskInfo_s {
u64		resv_00;		//000, reserved pointer
u64		resv_08;		//008, reserved pointer
u64		SysCall;		//010, System Call (Entry Point)
u64		resv_18;		//018, reserved function pointer
u64		regsave;		//020, pointer to register save area
u64		selfptr;		//028, self-pointer (in user VAS)
u64		krnlptr;		//030, pointer to kernel-only area
u64		ystatus;		//038, Sleep / Terminate Status
u64		usrptr;			//040, pointer to user-modifiable area
u64		allocaptr;		//048, pointer to alloca mark
u64		tlsptr;			//050, pointer to TLS data area

u16		pid;			//058, Process ID
u16		sch_id;			//05A, Scheduler ID
u16		uid;			//05C
u16		gid;			//05E

u64		baseptr;		//060, base pointer for main binary
u64		bootptr;		//068, entry point for main binary
u64		basegbr;		//070, GBR for main binary, set on program startup.
u64		boottbr;		//078, main process TBR (threads) | self-pointer.

u64		boot_sps;		//080, Stack Start (Bottom)
u64		boot_spe;		//088, Stack End (Top)
u64		argv;			//090, argument string (main thread), uptr (thread)
u64		envctx;			//098, Environment Context

u64		imgbaseptrs;	//0A0, list of loaded image Base Pointers.
u64		imggbrptrs;		//0A8, list of loaded image GBR Pointers
u64		imgtlsrvas;		//0B0, list of loaded image TLS RVAs.
u64		resv_B8;		//0B8, reserved
u64		resv_C0;		//0C0, reserved pointer
u64		resv_C8;		//0C8, reserved pointer
u64		tls_freelist;	//0D0, thread local free list
u64		resv_D8;		//0D8, reserved pointer
u64		resv_E0;		//0E0, reserved pointer
u64		resv_E8;		//0E8, reserved pointer
u64		resv_F0;		//0F0, reserved pointer
u64		resv_F8;		//0F8, reserved pointer

u64		resv_100;		//100, reserved pointer
u64		resv_108;		//108, reserved pointer
u64		resv_110;		//110, reserved pointer
u64		resv_118;		//118, reserved pointer
u64		resv_120;		//120, reserved pointer
u64		resv_128;		//128, reserved pointer
u64		resv_130;		//130, reserved pointer
u64		resv_138;		//138, reserved pointer
u64		resv_140;		//140, reserved pointer
u64		resv_148;		//148, reserved pointer
u64		resv_150;		//150, reserved pointer
u64		resv_158;		//158, reserved pointer
u64		resv_160;		//160, reserved pointer
u64		resv_168;		//168, reserved pointer
u64		resv_170;		//170, reserved pointer
u64		resv_178;		//178, reserved pointer

u64		resv_180;		//180, reserved pointer
u64		resv_188;		//188, reserved pointer
u64		resv_190;		//190, reserved pointer
u64		resv_198;		//198, reserved pointer
u64		resv_1A0;		//1A0, reserved pointer
u64		resv_1A8;		//1A8, reserved pointer
u64		resv_1B0;		//1B0, reserved pointer
u64		resv_1B8;		//1B8, reserved pointer
u64		resv_1C0;		//1C0, reserved pointer
u64		resv_1C8;		//1C8, reserved pointer
u64		resv_1D0;		//1D0, reserved pointer
u64		resv_1D8;		//1D8, reserved pointer
u64		resv_1E0;		//1E0, reserved pointer
u64		resv_1E8;		//1E8, reserved pointer
u64		resv_1F0;		//1F0, reserved pointer
u64		resv_1F8;		//1F8, reserved pointer

//200, End of fixed area.

u64		magic0;			//key magic pointer

int		n_tlsv;			//number of allocated TLS vars
int		n_dllimg;		//number of loaded DLL images
int		m_imgptrs;		//Maximum Image Pointers

byte	status;			//Status, 0=Running
byte	prio;			//Priority Level
byte	qtick;			//Schedule Tick

byte	stdin_eof;		//EOF (for stdin redirect)

int		ttyid;			//terminal for console printing

int		redir_stdin;	//stdin redirect
int		redir_stdout;	//stdout redirect

int		next_redir_stdin;	//next stdin redirect (createproces)
int		next_redir_stdout;	//next stdout redirect

u64		us_lastsleep;	//last sleep time for task

u64		img_baseptrs[256];
u64		img_gbrptrs[256];
u32		img_tlsrvas[256];

u64 	img_elftls[256];

};

struct X3PE_CreateTaskInfo_s {
int szInfo;		//size of this structure.
int idTty;		//console ID, 0 for default.
int szStack;	//requested stack size (bytes), 0 for default
int pad0;		//
u64	flSched;	//scheduler flags
int rdStdin;	//redirect stdin (0 for console/terminal)
int rdStdout;	//redirect stdout (0 for console/terminal)
};

/*
XG3+RV64 Userland Only Interpreter.

Will likely assume a mixture of XG3 and RV64G.

Memory space will be essentially a page-table mapping into the host space.
Page size will be 16K, with a 46 bit address space.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <time.h>

#ifndef BTM_X3VMINTERP_H
#define BTM_X3VMINTERP_H


#ifndef BTM_BASETYPES
#define BTM_BASETYPES
typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef int8_t		sbyte;
typedef int16_t	s16;
typedef int32_t	s32;
typedef int64_t	s64;
#endif

typedef u64 x3vm_addr;

typedef struct X3VM_Context_s	X3VM_Context;
typedef struct X3VM_Trace_s	X3VM_Trace;
typedef struct X3VM_Opcode_s	X3VM_Opcode;
typedef struct X3VM_VfsGlue_s		X3VM_VfsGlue;
typedef struct X3VM_VfsGlueVt_s	X3VM_VfsGlueVt;

#define X3VM_REG_X0		0x00	//Zero
#define X3VM_REG_X1		0x01	//RA / LR
#define X3VM_REG_X2		0x02	//SP
#define X3VM_REG_X3		0x03	//GP / GBR
#define X3VM_REG_X4		0x04	//TP / TBR
#define X3VM_REG_X5		0x05
#define X3VM_REG_X6		0x06
#define X3VM_REG_X7		0x07

#define X3VM_REG_F0		0x20
#define X3VM_REG_F1		0x21
#define X3VM_REG_F2		0x22
#define X3VM_REG_F3		0x23
#define X3VM_REG_F4		0x24
#define X3VM_REG_F5		0x25
#define X3VM_REG_F6		0x26
#define X3VM_REG_F7		0x27

#define X3VM_REG_PC		0x40

#define X3VM_REG_SPC	0x42
#define X3VM_REG_TEA	0x43

#define X3VM_REG_BPC	0x60


#define X3VM_REG_ZR		X3VM_REG_X0
#define X3VM_REG_LR		X3VM_REG_X1
#define X3VM_REG_SP		X3VM_REG_X2
#define X3VM_REG_GBR	X3VM_REG_X3



#define X3VM_NMID_INVOP		0x00
#define X3VM_NMID_PREDT		0x02
#define X3VM_NMID_PREDF		0x03
#define X3VM_NMID_ADDW		0x04
#define X3VM_NMID_SUBW		0x05
#define X3VM_NMID_ADDWU		0x06
#define X3VM_NMID_SUBWU		0x07
#define X3VM_NMID_ADD		0x08
#define X3VM_NMID_SUB		0x09
#define X3VM_NMID_SLL		0x0A
#define X3VM_NMID_SRL		0x0B
#define X3VM_NMID_SRA		0x0C
#define X3VM_NMID_AND		0x0D
#define X3VM_NMID_OR		0x0E
#define X3VM_NMID_XOR		0x0F

#define X3VM_NMID_LB		0x10
#define X3VM_NMID_LH		0x11
#define X3VM_NMID_LW		0x12
#define X3VM_NMID_LD		0x13
#define X3VM_NMID_LBU		0x14
#define X3VM_NMID_LHU		0x15
#define X3VM_NMID_LWU		0x16
#define X3VM_NMID_LDP		0x17
#define X3VM_NMID_SB		0x18
#define X3VM_NMID_SH		0x19
#define X3VM_NMID_SW		0x1A
#define X3VM_NMID_SD		0x1B
#define X3VM_NMID_MULW		0x1C
#define X3VM_NMID_MULWU		0x1D
#define X3VM_NMID_LEA		0x1E
#define X3VM_NMID_SDP		0x1F

#define X3VM_NMID_MIN		0x20
#define X3VM_NMID_MAX		0x21
#define X3VM_NMID_SEQ		0x22
#define X3VM_NMID_SNE		0x23
#define X3VM_NMID_SLT		0x24
#define X3VM_NMID_SGE		0x25
#define X3VM_NMID_SLTU		0x26
#define X3VM_NMID_SGEU		0x27
#define X3VM_NMID_ADDX		0x28
#define X3VM_NMID_SUBX		0x29
#define X3VM_NMID_MULQ		0x2A
#define X3VM_NMID_MULQU		0x2B
#define X3VM_NMID_SLA		0x2C
#define X3VM_NMID_ANDX		0x2D
#define X3VM_NMID_ORX		0x2E
#define X3VM_NMID_XORX		0x2F

#define X3VM_NMID_JAL		0x30
#define X3VM_NMID_JALR		0x31
#define X3VM_NMID_BEQ		0x32
#define X3VM_NMID_BNE		0x33
#define X3VM_NMID_BLT		0x34
#define X3VM_NMID_BGE		0x35
#define X3VM_NMID_BLTU		0x36
#define X3VM_NMID_BGEU		0x37
#define X3VM_NMID_BTSTT		0x38
#define X3VM_NMID_BTSTF		0x39
#define X3VM_NMID_SLAX		0x3A
#define X3VM_NMID_SLLX		0x3B
#define X3VM_NMID_SLAW		0x3C
#define X3VM_NMID_SLLW		0x3D
#define X3VM_NMID_LI		0x3E
#define X3VM_NMID_SHORI		0x3F

#define X3VM_NMID_RSUB		0x40
#define X3VM_NMID_FLDCH		0x41
#define X3VM_NMID_SRLW		0x42
#define X3VM_NMID_SRAW		0x43
#define X3VM_NMID_FSD2W		0x44
#define X3VM_NMID_FSD2H		0x45
#define X3VM_NMID_FLW2D		0x46
#define X3VM_NMID_FLH2D		0x47
#define X3VM_NMID_PACK		0x48
#define X3VM_NMID_PACKU		0x49
#define X3VM_NMID_PACKUL	0x4A
#define X3VM_NMID_PACKLU	0x4B
#define X3VM_NMID_DMULW		0x4C
#define X3VM_NMID_DMULWU	0x4D
#define X3VM_NMID_DMULH		0x4E
#define X3VM_NMID_DMULHU	0x4F

#define X3VM_NMID_FADD		0x50
#define X3VM_NMID_FSUB		0x51
#define X3VM_NMID_FMUL		0x52
#define X3VM_NMID_FDIV		0x53
#define X3VM_NMID_FMAC		0x54
#define X3VM_NMID_FLH		0x55
#define X3VM_NMID_FLW		0x56
#define X3VM_NMID_FLD		0x57
#define X3VM_NMID_LEAB		0x58
#define X3VM_NMID_LEAH		0x59
#define X3VM_NMID_LEAW		0x5A
#define X3VM_NMID_LEAD		0x5B
#define X3VM_NMID_SH1ADD	0x5C
#define X3VM_NMID_FSH		0x5D
#define X3VM_NMID_FSW		0x5E
#define X3VM_NMID_FSD		0x5F

#define X3VM_NMID_LUI		0x60
#define X3VM_NMID_AUIPC		0x61
#define X3VM_NMID_SH2ADD	0x62
#define X3VM_NMID_SH3ADD	0x63
#define X3VM_NMID_DIVW		0x64
#define X3VM_NMID_DIVUW		0x65
#define X3VM_NMID_REMW		0x66
#define X3VM_NMID_REMUW		0x67
#define X3VM_NMID_DIV		0x68
#define X3VM_NMID_DIVU		0x69
#define X3VM_NMID_REM		0x6A
#define X3VM_NMID_REMU		0x6B
#define X3VM_NMID_EBREAK	0x6C
#define X3VM_NMID_ECALL		0x6D
#define X3VM_NMID_ADDUW		0x6E
#define X3VM_NMID_SHORI32	0x6F

#define X3VM_NMID_SGT		0x70
#define X3VM_NMID_FCVTDUS	0x71
#define X3VM_NMID_FMIN		0x72
#define X3VM_NMID_FMAX		0x73
#define X3VM_NMID_EXTSB		0x74
#define X3VM_NMID_EXTUB		0x75
#define X3VM_NMID_EXTSH		0x76
#define X3VM_NMID_EXTUH		0x77
#define X3VM_NMID_FCVTSD	0x78
#define X3VM_NMID_FCVTDS	0x79
#define X3VM_NMID_FCVTHD	0x7A
#define X3VM_NMID_FCVTDH	0x7B
#define X3VM_NMID_FCVTID	0x7C
#define X3VM_NMID_FCVTDI	0x7D
#define X3VM_NMID_FCVTIS	0x7E
#define X3VM_NMID_FCVTSI	0x7F

#define X3VM_NMID_PADDW		0x80
#define X3VM_NMID_PADDL		0x81
#define X3VM_NMID_PSUBW		0x82
#define X3VM_NMID_PSUBL		0x83
#define X3VM_NMID_PADDH		0x84
#define X3VM_NMID_PADDF		0x85
#define X3VM_NMID_PSUBH		0x86
#define X3VM_NMID_PSUBF		0x87
#define X3VM_NMID_PMULH		0x88
#define X3VM_NMID_PMULF		0x89
#define X3VM_NMID_PADDXF	0x8A
#define X3VM_NMID_PSUBXF	0x8B
#define X3VM_NMID_PMULXF	0x8C
#define X3VM_NMID_PADDXD	0x8D
#define X3VM_NMID_PSUBXD	0x8E
#define X3VM_NMID_PMULXD	0x8F

#define X3VM_NMID_PMULSW	0x90
#define X3VM_NMID_PMULUW	0x91
#define X3VM_NMID_FSQRT		0x92
#define X3VM_NMID_MULQH		0x93
#define X3VM_NMID_PMULSLW	0x94
#define X3VM_NMID_PMULULW	0x95
#define X3VM_NMID_PMULSHW	0x96
#define X3VM_NMID_PMULUHW	0x97
#define X3VM_NMID_STST		0x98
#define X3VM_NMID_SNTST		0x99
#define X3VM_NMID_MULQHSU	0x9A
#define X3VM_NMID_MULQHU	0x9B
#define X3VM_NMID_PSHUFB	0x9C
#define X3VM_NMID_PSHUFH	0x9D
#define X3VM_NMID_BITMOV	0x9E
#define X3VM_NMID_BITMOVS	0x9F

#define X3VM_NMID_FMADDS	0xA0
#define X3VM_NMID_FMSUBS	0xA1
#define X3VM_NMID_FMNADDS	0xA2
#define X3VM_NMID_FMNSUBS	0xA3
#define X3VM_NMID_FMADDD	0xA4
#define X3VM_NMID_FMSUBD	0xA5
#define X3VM_NMID_FMNADDD	0xA6
#define X3VM_NMID_FMNSUBD	0xA7
#define X3VM_NMID_FMADDH	0xA8
#define X3VM_NMID_FMSUBH	0xA9
#define X3VM_NMID_FMNADDH	0xAA
#define X3VM_NMID_FMNSUBH	0xAB
#define X3VM_NMID_PMINF		0xAC
#define X3VM_NMID_PMINH		0xAD
#define X3VM_NMID_PMAXF		0xAE
#define X3VM_NMID_PMAXH		0xAF

#define X3VM_NMID_PDIVF		0xB0
#define X3VM_NMID_PDIVH		0xB1
#define X3VM_NMID_FEQ		0xB2
#define X3VM_NMID_FLT		0xB3
#define X3VM_NMID_FGE		0xB4
#define X3VM_NMID_FEQF		0xB5
#define X3VM_NMID_FLTF		0xB6
#define X3VM_NMID_FGEF		0xB7
#define X3VM_NMID_FEQH		0xB8
#define X3VM_NMID_FLTH		0xB9
#define X3VM_NMID_FGEH		0xBA
#define X3VM_NMID_PCVTSH	0xBC
#define X3VM_NMID_PCVTHS	0xBD
#define X3VM_NMID_PSQRTF	0xBE
#define X3VM_NMID_PSQRTH	0xBF

#define X3VM_NMID_FSGNJ		0xC0
#define X3VM_NMID_FSGNJN	0xC1
#define X3VM_NMID_FSGNJX	0xC2
#define X3VM_NMID_FSGNJF	0xC3
#define X3VM_NMID_FSGNJNF	0xC4
#define X3VM_NMID_FSGNJXF	0xC5
#define X3VM_NMID_FSGNJH	0xC6
#define X3VM_NMID_FSGNJNH	0xC7
#define X3VM_NMID_FSGNJXH	0xC8
#define X3VM_NMID_NOP		0xC9
#define X3VM_NMID_CSRRW		0xCA
#define X3VM_NMID_CSRRS		0xCB
#define X3VM_NMID_CSRRC		0xCC
#define X3VM_NMID_CSRRWI	0xCD
#define X3VM_NMID_CSRRSI	0xCE
#define X3VM_NMID_CSRRCI	0xCF

// #define X3VM_NMID_FNEG		0xD0
// #define X3VM_NMID_FABS		0xD1



#define X3VM_FMID_INV		0x00
#define X3VM_FMID_NONE		0x01	//-
#define X3VM_FMID_J2RI		0x02	//JAL Disp, Rn
#define X3VM_FMID_J3RI		0x03	//Jcc Rs, Rt, Disp

#define X3VM_FMID_3R		0x04	//Rs, Rt, Rn
#define X3VM_FMID_3RI		0x05	//Rs, Imm, Rn
#define X3VM_FMID_2R		0x06	//Rs, Rn
#define X3VM_FMID_2RI		0x07	//Imm, Rn
#define X3VM_FMID_LDRI		0x08	//(Rs, Disp), Rn
#define X3VM_FMID_STRI		0x09	//Rn, (Rs, Disp)
#define X3VM_FMID_LDRR		0x0A	//(Rs, Idx*Sc), Rn
#define X3VM_FMID_STRR		0x0B	//Rn, (Rs, Idx*Sc)

#define X3VM_FMID_4R		0x0C	//Rs, Rt, Ru, Rn
#define X3VM_FMID_4RI		0x0D	//Rs, Rt, Imm, Rn

#define	X3VM_FAULT_MEMACC	0x8000
#define	X3VM_FAULT_BREAK	0x8004
#define	X3VM_FAULT_PAGEFLT	0x8007
#define	X3VM_FAULT_INVOP	0x800E

#define	X3VM_FAULT_BADCALL	0x801C

#define	X3VM_FAULT_ECALL	0xE000

#define	X3VM_OPFL_CTRLF		0x01
#define	X3VM_OPFL_OP16		0x02

#define	X3VM_OPFL_RN_P		0x10
#define	X3VM_OPFL_RS_P		0x20
#define	X3VM_OPFL_RT_P		0x40
#define	X3VM_OPFL_RNST_P	0x70

#define X3VM_MEMTLB_SIZE	1024
#define X3VM_MEMTLB_WAY		4

struct X3VM_Context_s {
u64 reg[128];
X3VM_Trace *tr_hash[4096];
X3VM_Trace *tr_next;
X3VM_Trace *tr_bra;
X3VM_Trace *tr_cur;

X3VM_Trace *tr_pred_lr0;
X3VM_Trace *tr_pred_lr1;
X3VM_Trace *tr_pred_lr2;

X3VM_Context	*vas_parent;	//Sub-context in shared VAS
X3VM_VfsGlue	*vfs;

void *mem_spgp_ptr;
u32 mem_spgp_pgn;

void *tlb_vpgptr0[X3VM_MEMTLB_SIZE];
void *tlb_vpgptr1[X3VM_MEMTLB_SIZE];
u32 tlb_vpgaddr0[X3VM_MEMTLB_SIZE];	//virtual page address, Addr(45:14)
u32 tlb_vpgaddr1[X3VM_MEMTLB_SIZE];	//virtual page address, Addr(45:14)
#if (X3VM_MEMTLB_WAY>2)
void *tlb_vpgptr2[X3VM_MEMTLB_SIZE];
void *tlb_vpgptr3[X3VM_MEMTLB_SIZE];
u32 tlb_vpgaddr2[X3VM_MEMTLB_SIZE];	//virtual page address, Addr(45:14)
u32 tlb_vpgaddr3[X3VM_MEMTLB_SIZE];	//virtual page address, Addr(45:14)
#endif

X3VM_Opcode *free_opc;
X3VM_Trace *free_tr;

X3VM_Trace *tr_cont;

X3VM_Trace *tr_log[256];

u64 timeref_usec;
u64 timeref_cyc;
u64 timeref_scale;		//32.32, cycles -> usec
u64 timeref_lastusec;

u64 totcyc;
u64 pagetab;
u64 rng_seed1;
u64 rng_seed2;
u64 rng_seed3;
u64 rng_seed4;
byte rng_mixto;

byte tr_logpos;
byte opt_faststack;		//make stack ops faster?

u16 aslr_useblock;
u16 pc_hibits;			//high order bits for PC.

u64 ex_reg[128];
int fault;

int stat_alloc_ops;
int stat_alloc_traces;

int stat_dec_traces;
int stat_dec_traceops;

int stat_dec_tracecnts[64];
};

#define X3VM_TRACE_MAXOPS	24
// #define X3VM_TRACE_MAXOPS	32

// #define X3VM_TRACE_MAXEXTOPS	X3VM_TRACE_MAXOPS
#define X3VM_TRACE_MAXEXTOPS	(X3VM_TRACE_MAXOPS*2)

struct X3VM_Trace_s {
X3VM_Trace *(*Run)(X3VM_Context *ctx, X3VM_Trace *tr);
X3VM_Trace *h_next;
X3VM_Trace *tr_next;	//next, by address
X3VM_Trace *tr_bra;		//next, branch
X3VM_Trace *tr_nobra;	//next, no-branch JAL
X3VM_Trace *tr_doext;	//trace-extension
u64 addr;
u64 addr_next;
u64 addr_nobra;			//addr, no-branch JAL
int n_ops;
X3VM_Opcode *ops[X3VM_TRACE_MAXOPS];
};

struct X3VM_Opcode_s {
void (*Run)(X3VM_Context *ctx, X3VM_Opcode *op);
X3VM_Opcode *subop;
s64 imm;
u64 addr;
u32 opw0;
u32 opw1;
u32 opw2;
byte nmid;
byte fmid;
byte rs;
byte rt;
byte ru;
byte rn;
byte sc;
byte opfl;
};

struct X3VM_VfsGlueVt_s {
void *pad0;
void *pad1;
void *pad2;
void *pad3;
void *(*vfsOpen)(X3VM_VfsGlue *ctx, char *name, char *mode);
void (*vfsClose)(X3VM_VfsGlue *ctx, void *fd);
s64 (*vfsSeek)(X3VM_VfsGlue *ctx, void *fd, s64 pos, int rel);
s64 (*vfsRead)(X3VM_VfsGlue *ctx, void *buf, s64 sz1, s64 sz2, void *fd);
s64 (*vfsWrite)(X3VM_VfsGlue *ctx, void *buf, s64 sz1, s64 sz2, void *fd);
};

struct X3VM_VfsGlue_s {
X3VM_VfsGlueVt *vt;
/* Generic provider-dependent variables */
void *uptr0;
void *uptr1;
void *uptr2;
void *uptr3;
/* Any other contents defined by provider. */
};

int X3VM_TraceDecodeForAddr(X3VM_Context *ctx,
	X3VM_Trace *tr, u64 addr, int usefl);
X3VM_Trace *X3VM_TraceGetForAddr(X3VM_Context *ctx, u64 addr, int fl);

X3VM_Trace *X3VM_AllocTrace(X3VM_Context *ctx);
X3VM_Opcode *X3VM_AllocOpcode(X3VM_Context *ctx);

u32 X3VM_RngRand32(X3VM_Context *ctx);
u64 X3VM_RngRand64(X3VM_Context *ctx);

char *x3vm_strdup(char *str);
int x3vm_strdup_i(char *str);

#endif

/*
BTSR VM:
Will use direct linking and assume a non-modifiable program space.

*/

#define BTSR1_REG_R0	0
#define BTSR1_REG_R1	1
#define BTSR1_REG_R2	2
#define BTSR1_REG_R3	3
#define BTSR1_REG_R4	4
#define BTSR1_REG_R5	5
#define BTSR1_REG_R6	6
#define BTSR1_REG_R7	7
#define BTSR1_REG_R8	8
#define BTSR1_REG_R9	9
#define BTSR1_REG_R10	10
#define BTSR1_REG_R11	11
#define BTSR1_REG_R12	12
#define BTSR1_REG_R13	13
#define BTSR1_REG_R14	14
#define BTSR1_REG_R15	15

#define BTSR1_REG_SP	15
#define BTSR1_REG_PC	16
#define BTSR1_REG_LR	17
#define BTSR1_REG_SR	18
#define BTSR1_REG_VBR	19
#define BTSR1_REG_DLR	20
#define BTSR1_REG_DHR	21
#define BTSR1_REG_GBR	22
#define BTSR1_REG_TBR	23

#define BTSR1_REG_TTB	24
#define BTSR1_REG_TEA	25
#define BTSR1_REG_MMCR	26
#define BTSR1_REG_EXSR	27

#define BTSR1_REG_DR	BTSR1_REG_DLR

#define BTSR1_REG_R0B	32
#define BTSR1_REG_R1B	33
#define BTSR1_REG_R2B	34
#define BTSR1_REG_R3B	35
#define BTSR1_REG_R4B	36
#define BTSR1_REG_R5B	37
#define BTSR1_REG_R6B	38
#define BTSR1_REG_R7B	39
#define BTSR1_REG_SPC	40
#define BTSR1_REG_SLR	41
#define BTSR1_REG_SSR	42
#define BTSR1_REG_SSP	43
#define BTSR1_REG_SDL	44
#define BTSR1_REG_SDH	45
#define BTSR1_REG_SGB	46
#define BTSR1_REG_STB	47

#define BTSR1_TR_MAXOP	32


#define BTSR1_FLT_INVADDR	0x8000		//invalid memory address
#define BTSR1_FLT_INV_MRD	0x8001		//invalid memory read
#define BTSR1_FLT_INV_MWR	0x8002		//invalid memory write
#define BTSR1_FLT_INV_MEX	0x8003		//invalid memory execute
#define BTSR1_FLT_BREAK		0x8004		//BREAK
#define BTSR1_FLT_SLEEP		0x8005		//SLEEP

#define BTSR1_FLT_PCMISH	0x8801		//PC doesn't match trace addr

#define BTSR1_FLT_TIMER		0xC001		//kHz timer

#define BTSR1_OPFL_CTRLF	0x01		//Control-Flow Opcode
#define BTSR1_OPFL_TWOWORD	0x02		//Uses two instruction words

#define BTSR1_NMID_NONE		0x00		//
#define BTSR1_NMID_MOVB		0x01		//
#define BTSR1_NMID_MOVW		0x02		//
#define BTSR1_NMID_MOVD		0x03		//
#define BTSR1_NMID_MOVQ		0x04		//
#define BTSR1_NMID_MOVUB	0x05		//
#define BTSR1_NMID_MOVUW	0x06		//
#define BTSR1_NMID_NOP		0x07		//
#define BTSR1_NMID_BRA		0x08		//
#define BTSR1_NMID_BSR		0x09		//
#define BTSR1_NMID_BT		0x0A		//
#define BTSR1_NMID_BF		0x0B		//
#define BTSR1_NMID_LEAB		0x0C		//
#define BTSR1_NMID_LEAW		0x0D		//
#define BTSR1_NMID_LEAD		0x0E		//
#define BTSR1_NMID_LEAQ		0x0F		//
#define BTSR1_NMID_ADD		0x10		//
#define BTSR1_NMID_SUB		0x11		//
#define BTSR1_NMID_ADC		0x12		//
#define BTSR1_NMID_SBB		0x13		//
#define BTSR1_NMID_TST		0x14		//
#define BTSR1_NMID_AND		0x15		//
#define BTSR1_NMID_OR		0x16		//
#define BTSR1_NMID_XOR		0x17		//
#define BTSR1_NMID_MOV		0x18		//
#define BTSR1_NMID_LDIZ		0x19		//
#define BTSR1_NMID_LDIN		0x1A		//
#define BTSR1_NMID_LDISH	0x1B		//
#define BTSR1_NMID_CMPEQ	0x1C		//
#define BTSR1_NMID_CMPHI	0x1D		//
#define BTSR1_NMID_CMPGT	0x1E		//
#define BTSR1_NMID_RTS		0x1F		//
#define BTSR1_NMID_EXTUB	0x20		//
#define BTSR1_NMID_EXTUW	0x21		//
#define BTSR1_NMID_EXTSB	0x22		//
#define BTSR1_NMID_EXTSW	0x23		//
#define BTSR1_NMID_CLRT		0x24		//
#define BTSR1_NMID_SETT		0x25		//
#define BTSR1_NMID_CLRS		0x26		//
#define BTSR1_NMID_SETS		0x27		//
#define BTSR1_NMID_NOTT		0x28		//
#define BTSR1_NMID_NOTS		0x29		//
#define BTSR1_NMID_LDISH16	0x2A		//
#define BTSR1_NMID_CMPGE	0x2B		//
#define BTSR1_NMID_PUSH		0x2C		//
#define BTSR1_NMID_POP		0x2D		//
#define BTSR1_NMID_CMPPL	0x2E		//
#define BTSR1_NMID_CMPPZ	0x2F		//
#define BTSR1_NMID_SHLL		0x30		//
#define BTSR1_NMID_SHLR		0x31		//
#define BTSR1_NMID_SHAR		0x32		//
#define BTSR1_NMID_CMPHS	0x33		//
#define BTSR1_NMID_BREAK	0x34		//
#define BTSR1_NMID_SLEEP	0x35		//
#define BTSR1_NMID_MULS		0x36		//
#define BTSR1_NMID_MULU		0x37		//
#define BTSR1_NMID_RTE		0x38		//
#define BTSR1_NMID_NOT		0x39		//
#define BTSR1_NMID_NEG		0x3A		//
#define BTSR1_NMID_NEGC		0x3B		//
#define BTSR1_NMID_ROTL		0x3C		//
#define BTSR1_NMID_ROTR		0x3D		//
#define BTSR1_NMID_ROTCL	0x3E		//
#define BTSR1_NMID_ROTCR	0x3F		//
#define BTSR1_NMID_RET		0x40		//
#define BTSR1_NMID_MOVT		0x41		//
#define BTSR1_NMID_LDISH12	0x42		//
#define BTSR1_NMID_LDISH20	0x43		//
#define BTSR1_NMID_SHAD		0x44		//
#define BTSR1_NMID_SHLD		0x45		//
#define BTSR1_NMID_SWAPB	0x46		//
#define BTSR1_NMID_SWAPW	0x47		//

#define BTSR1_NMID_FADD		0x48		//
#define BTSR1_NMID_FSUB		0x49		//
#define BTSR1_NMID_FMUL		0x4A		//
#define BTSR1_NMID_FDIV		0x4B		//
#define BTSR1_NMID_FCMPEQ	0x4C		//
#define BTSR1_NMID_FCMPGT	0x4D		//
#define BTSR1_NMID_FMOVS	0x4E		//
#define BTSR1_NMID_FMOVD	0x4F		//
#define BTSR1_NMID_FMOV		0x50		//
#define BTSR1_NMID_FNEG		0x51		//
#define BTSR1_NMID_FLDCF	0x52		//
#define BTSR1_NMID_FSTCF	0x53		//
#define BTSR1_NMID_FLDCI	0x54		//
#define BTSR1_NMID_FSTCI	0x55		//
#define BTSR1_NMID_FLDCH	0x56		//
#define BTSR1_NMID_FSTCH	0x57		//
#define BTSR1_NMID_FLDCD	0x58		//
#define BTSR1_NMID_FSTCD	0x59		//
#define BTSR1_NMID_FABS		0x5A		//

#define BTSR1_FMID_NONE			0x00		//?
#define BTSR1_FMID_REG			0x01		//Rn
#define BTSR1_FMID_REGREG		0x02		//Rm, Rn
#define BTSR1_FMID_Z			0x03		//-
#define BTSR1_FMID_IMM			0x04		//#imm
#define BTSR1_FMID_PCDISP		0x05		//(PC, disp)
#define BTSR1_FMID_LDREGREG		0x06		//(Rm), Rn
#define BTSR1_FMID_REGSTREG		0x07		//Rm, (Rn)
#define BTSR1_FMID_LDDRREGREG	0x08		//(Rm, DLR), Rn
#define BTSR1_FMID_REGSTDRREG	0x09		//Rm, (Rn, DLR)
#define BTSR1_FMID_LDREGDISPREG	0x0A		//(Rm, Disp), Rn
#define BTSR1_FMID_REGSTREGDISP	0x0B		//Rm, (Rn, Disp)
#define BTSR1_FMID_PCDR			0x0C		//(PC, DLR)
#define BTSR1_FMID_LDDRPCREG	0x0D		//(PC, DLR), Rn
#define BTSR1_FMID_REGSTDRPC	0x0E		//Rm, (PC, DLR)
#define BTSR1_FMID_IMMREG		0x0F		//#imm, Rn
#define BTSR1_FMID_LDDRABSREG	0x10		//(DLR), Rn
#define BTSR1_FMID_REGSTDRABS	0x11		//Rm, (DLR)
#define BTSR1_FMID_PCDR4		0x12		//(PC, DLR_i4)
#define BTSR1_FMID_LDDR4PCREG	0x13		//(PC, DLR_i4), Rn
#define BTSR1_FMID_REGSTDR4PC	0x14		//Rm, (PC, DLR_i4)
#define BTSR1_FMID_DR4REG		0x15		//DLR_i4, Rn
#define BTSR1_FMID_LDPCDISPREG	0x16		//(PC, Disp), Rn
#define BTSR1_FMID_REGSTPCDISP	0x17		//Rm, (PC, Disp)
#define BTSR1_FMID_REGDRREG		0x18		//Rm, DLR, Rn
#define BTSR1_FMID_PCREG		0x19		//(PC, Reg)

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef int btsr1_addr;


typedef struct BTSR1_Context_s BTSR1_Context;
typedef struct BTSR1_Opcode_s BTSR1_Opcode;
typedef struct BTSR1_Trace_s BTSR1_Trace;
typedef struct BTSR1_MemSpan_s BTSR1_MemSpan;

struct BTSR1_Context_s {
u32 regs[64];				//GPRs and CRs
u64 fpreg[16];				//FPRs

BTSR1_Trace *trhash[1024];
BTSR1_Trace *trcur;			//cached trace for interpreter loop.

BTSR1_MemSpan *span[32];	//memory spans, sorted by address
int n_span;

BTSR1_MemSpan *span_pr0;
BTSR1_MemSpan *span_pr1;

BTSR1_Opcode *free_op;
BTSR1_Trace *free_tr;

BTSR1_Trace *tr_cur;		//Current Trace (Runtime)
BTSR1_Trace *tr_rnxt;		//Trace lnknext (Runtime)
BTSR1_Trace *tr_rjmp;		//Trace jmpnext (Runtime)

u32 pclog[64];
byte pclogrov;

int status;

byte req_kill;				//VM has been requested to stop
byte kbbuf[256];			//debug keyboard buffer
byte kbrov;					//debug keyboard rover
byte kbirov;				//debug keyboard rover
byte kbirq;					//keyboard IRQ

int ttick_hk;				//timer ticks until IRQ
int ttick_rst;				//timer ticks reset
s64 tot_cyc;
s64 tot_ops;
s16 tgt_mhz;				//target MHz
u16 rcp_mhz;				//reciprocal MHz
};

struct BTSR1_Opcode_s {
u16 opn;		//Opcode Value
u16 opn2;		//Opcode Value
byte nmid;		//Opcode Number
byte fmid;		//Form ID
byte rn;		//Dest Register
byte rm;		//Source Register
byte fl;		//Opcodde Flags
s32 imm;		//Immediate

sbyte cyc;		//Clock Cycles
btsr1_addr pc;

void (*Run)(BTSR1_Context *ctx, BTSR1_Opcode *op);
};

struct BTSR1_Trace_s {
BTSR1_Opcode *ops[BTSR1_TR_MAXOP];
int n_ops;
int n_cyc;

btsr1_addr addr;
btsr1_addr addr_nxt;
btsr1_addr addr_jmp;
BTSR1_Trace *hnext;		//next in terms of hash
BTSR1_Trace *lnknext;	//next in terms of PC
BTSR1_Trace *jmpnext;	//next if ending in a branch

BTSR1_Trace *(*Run)(BTSR1_Context *ctx, BTSR1_Trace *tr);
};

struct BTSR1_MemSpan_s {
btsr1_addr addr_base;	//lower address
btsr1_addr addr_lim;	//upper address

char *name;				//helpful name
void *data;				//raw data pointer

int (*GetByte)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr);
int (*GetWord)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr);
s32 (*GetDWord)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr);

int (*SetByte)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val);
int (*SetWord)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val);
int (*SetDWord)(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, s32 val);
};


#define BTSR1_PtrGetSByteIxLe(ptr, ix)	(((sbyte *)(ptr))[ix])
#define BTSR1_PtrGetSWordIxLe(ptr, ix)	(((s16 *)(ptr))[ix])
#define BTSR1_PtrGetSDWordIxLe(ptr, ix)	(((s32 *)(ptr))[ix])

#define BTSR1_PtrSetByteIxLe(ptr, ix, val)		(((sbyte *)(ptr))[ix]=(val))
#define BTSR1_PtrSetWordIxLe(ptr, ix, val)		(((s16 *)(ptr))[ix]=(val))
#define BTSR1_PtrSetDWordIxLe(ptr, ix, val)		(((s32 *)(ptr))[ix]=(val))


#define BTSR1_PtrGetOfs(ptr, ix)		(((sbyte *)(ptr))+(ix))

#define BTSR1_PtrGetSByteOfsLe(ptr, ix)		(*(sbyte *)BTSR1_PtrGetOfs(ptr, ix))
#define BTSR1_PtrGetSWordOfsLe(ptr, ix)		(*(s16 *)BTSR1_PtrGetOfs(ptr, ix))
#define BTSR1_PtrGetSDWordOfsLe(ptr, ix)	(*(s32 *)BTSR1_PtrGetOfs(ptr, ix))

#define BTSR1_PtrSetByteOfsLe(ptr, ix, val)		\
	(*(sbyte *)BTSR1_PtrGetOfs(ptr, ix)=(val))
#define BTSR1_PtrSetWordOfsLe(ptr, ix, val)		\
	(*(s16 *)BTSR1_PtrGetOfs(ptr, ix)=(val))
#define BTSR1_PtrSetDWordOfsLe(ptr, ix, val)	\
	(*(s32 *)BTSR1_PtrGetOfs(ptr, ix)=(val))


BTSR1_Opcode *BTSR1_ContextAllocOpcode(BTSR1_Context *ctx);
BTSR1_Trace *BTSR1_ContextAllocTrace(BTSR1_Context *ctx);
BTSR1_Trace *BTSR1_GetTraceForAddr(BTSR1_Context *ctx, btsr1_addr addr);

BTSR1_MemSpan *BTSR1_MemAllocSpan(BTSR1_Context *ctx);

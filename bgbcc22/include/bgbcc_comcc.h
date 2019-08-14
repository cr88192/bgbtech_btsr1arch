#define BGBCC_COFF_MACH_SH3			0x01A2
#define BGBCC_COFF_MACH_SH3DSP		0x01A3
#define BGBCC_COFF_MACH_SH4			0x01A6

#define BGBCC_COFF_SCNT_NOPAD		0x00000008
#define BGBCC_COFF_SCNT_CODE		0x00000020
#define BGBCC_COFF_SCNT_IDATA		0x00000040
#define BGBCC_COFF_SCNT_UDATA		0x00000080

#define BGBCC_COFF_SCNT_GPREL		0x00008000
#define BGBCC_COFF_SCNT_MEM6BIT		0x00020000

#define BGBCC_COFF_SCNT_ALGN1B		0x00100000
#define BGBCC_COFF_SCNT_ALGN2B		0x00200000
#define BGBCC_COFF_SCNT_ALGN4B		0x00300000
#define BGBCC_COFF_SCNT_ALGN8B		0x00400000
#define BGBCC_COFF_SCNT_ALGN16B		0x00500000
#define BGBCC_COFF_SCNT_ALGN32B		0x00600000
#define BGBCC_COFF_SCNT_ALGN64B		0x00700000
#define BGBCC_COFF_SCNT_ALGN128B	0x00800000
#define BGBCC_COFF_SCNT_ALGN256B	0x00900000
#define BGBCC_COFF_SCNT_ALGN512B	0x00A00000
#define BGBCC_COFF_SCNT_ALGN1KB		0x00B00000
#define BGBCC_COFF_SCNT_ALGN2KB		0x00C00000
#define BGBCC_COFF_SCNT_ALGN4KB		0x00D00000
#define BGBCC_COFF_SCNT_ALGN8KB		0x00E00000

#define BGBCC_COFF_SCNT_NRLCOVF		0x01000000
#define BGBCC_COFF_SCNT_DISCARD		0x02000000
#define BGBCC_COFF_SCNT_NOCACHE		0x04000000
#define BGBCC_COFF_SCNT_NOPAGE		0x08000000
#define BGBCC_COFF_SCNT_SHARED		0x10000000
#define BGBCC_COFF_SCNT_EXECUTE		0x20000000
#define BGBCC_COFF_SCNT_READ		0x40000000
#define BGBCC_COFF_SCNT_WRITE		0x80000000

/*
Memory Models
Default:
	Text/Data/BSS range: 16MB
	Address Space: Full 32/64

Tiny16:
	Assume all addresses fit within a 64kB space.
	Address Space: 16 bit

Small24:
	Assume Text/Data/BSS range of 64kB.
	Data/BSS range is 24 bits,
	Address Space: 24 bit

Medium24:
	Assume Text/Data/BSS range of 16MB.
	Address Space: 24 bit

Medium32:
	Assume Text/Data/BSS range of 16MB.
	Address Space: 32 bit

Large32:
	Assume Text/Data/BSS range of 4GB.
	Address Space: 32 bit.
*/

#define BGBCC_MEMMDL_DEFAULT		0	//Default
#define BGBCC_MEMMDL_TINY16			1	//64KB
#define BGBCC_MEMMDL_SMALL24		2	//64KB+16MB, 16MB
#define BGBCC_MEMMDL_MEDIUM24		3	//16MB+16MB, 16MB
#define BGBCC_MEMMDL_MEDIUM32		4	//16MB+16MB, 4GB
#define BGBCC_MEMMDL_LARGE32		4	//4GB+4GB, 4GB

#define BGBCC_PSZX_MASK				0x0F	//
#define BGBCC_PSZX_UNK				0x00	//Unknown extension
#define BGBCC_PSZX_SX				0x01	//Sign Extended
#define BGBCC_PSZX_ZX				0x02	//Zero Extended
#define BGBCC_PSZX_QW				0x03	//Quadword
#define BGBCC_PSZX_SSX				0x04	//Small Sign Extended
#define BGBCC_PSZX_SZX				0x05	//Small Zero Extended


typedef struct BGBCC_CMG_Context_s BGBCC_CMG_Context;
typedef struct BGBCC_CMG_OpcodeArg_s BGBCC_CMG_OpcodeArg;
typedef struct BGBCC_CMG_VarSpan_s BGBCC_CMG_VarSpan;
typedef struct BGBCC_CMG_VarSpan2_s BGBCC_CMG_VarSpan2;

typedef struct BGBCC_CMG_EmitQueueOp_s BGBCC_CMG_EmitQueueOp;


struct BGBCC_CMG_Context_s {
char *sec_name[16];
byte *sec_buf[16];
byte *sec_end[16];
byte *sec_pos[16];
byte *sec_vpos[16];
u32 sec_rva[16];		//relative virtual address (image offset)
u32 sec_lva[16];		//logical virtual address
u32 sec_lsz[16];		//logical size
byte sec;
byte nsec;

byte *asm_buf[16];
byte *asm_end[16];
byte *asm_pos[16];
byte do_asm;

byte is_le;			//is little endian
byte use_bp;		//use frame pointer
byte need_farjmp;	//function needs far32 jumps
byte need_f16jmp;	//function needs far16 jumps
byte need_n12jmp;	//function needs at least 12-bit jumps

byte need_n16bsr;	//image exceeds 16-bit BSR
byte need_n20bsr;	//image exceeds 20-bit BSR
byte need_n24bsr;	//image exceeds 24-bit BSR

byte need_n16dat;	//image exceeds 16-bit BSR
byte need_n20dat;	//image exceeds 20-bit BSR
byte need_n24dat;	//image exceeds 24-bit BSR

byte is_pic;		//is PIC.
byte is_pbo;		//is using PBO addressing
byte use_fpr;		//uses floating point registers
byte use_dbr;		//uses fp double registers
byte is_vararg;		//function is varargs
byte is_simpass;	//is simulation pass
byte is_stable;		//function is stable
byte is_addr64;		//target uses a 64-bit ISA
byte is_rawasm;		//is raw assembler
byte is_leaf;		//function is a leaf function
byte is_rom;		//building a ROM image
byte is_betav;		//uses BetaVe tweaks.
byte is_mergece;	//merge CC0e/CC3e into CExx
byte is_tr_leaf;	//given trace is a leaf
byte is_addr_x32;	//target uses 32-bit addresses (on 64-bit ISA)

byte no_fpu;		//no hardware FPU instructions
byte no_ext32;		//no 32-bit instruction forms
byte fpu_soft;		//use SoftFPU design.
byte fpu_lite;		//use LiteFPU design.
byte fpu_gfp;		//use GprFPU design.

byte is_fixed32;	//uses only 32-bit instruction forms
byte op_is_wex2;	//encoding a WEX2 instruction (0/2/3).
byte no_ops48;		//Disable 48-bit opcodes.

byte has_shad;		//has SHAD/SHLD
byte has_movi20;	//has MOVI20 and friends
byte has_misalgn;	//has misaligned load/store
byte has_bjx1mov;	//has BJX1 MOV I-forms
byte has_bjx1jmp;	//has BJX1 Branch I-forms
byte has_bjx1ari;	//has BJX1 Arithmetic I-forms
byte has_bjx1breq;	//has BJX1 BREQ/BRNE/... I-forms
byte use_onlyimm;	//use only inline immediates
byte has_bjx1egpr;	//has BJX1-64 extended GPR ops
byte has_bjx1r3mov;	//has BJX1 Reg3 ops

byte has_addsl;		//has ADDS.L and similar
// byte has_jcmp;		//has JMPCC

byte use_egpr;		//enable use of extended GPRs
byte maxreg_gpr;	//current number of GPR register-slots
byte maxreg_gpr_lf;	//current number of GPR register-slots (leaf)

byte use_efpr;		//enable use of extended FPRs
byte maxreg_fpr;	//current number of GPR register-slots
byte maxreg_fpr_lf;	//current number of GPR register-slots (leaf)

byte use_memmdl;	//use memory model
byte csrv_skip;		//skip a CSRV operation

byte use_emitqueue;	//use emit queue

byte emit_isprobe;	//if we are simply probing an instruction

byte test_lclalign;	//local alignment-sensitivity test
byte use_padcross;	//use padding across cache lines

byte optmode;		//optimization mode
byte pred_tfmd;

int simfnsz;		//simulation's function size
int simfnmsz;		//simulation's min function size
int simfnnsz;		//simulation's max function size
int tr_trnum;		//trace number
int tr_opnum;		//trace op number

int fnsz_pro;		//size of prolog
int fnsz_epi;		//size of epilog
int fnsz_bod;		//size of function body
int fnsz_vtr[16];
int fnsz_lvtr[16];
int fnsz_dvtr[16];

int sim_txtsz;
int sim_datsz;
int sim_bsssz;
int simimgsz;

u32 *lbl_ofs;		//label offsets
u32 *rlc_ofs;		//reloc offsets
u32 *lbl_id;		//label IDs
u32 *rlc_id;		//reloc label IDs
byte *lbl_sec;		//label section
byte *rlc_sec;		//reloc section
byte *rlc_ty;		//reloc type
s32 *lbl_chn;		//label chains
int nlbl, mlbl;
int nrlc, mrlc;
int nvlbl;
u16 lblrov;			//labels (local)
u16 lbltrov;		//labels (temp)

s32 lbl_hash[1024];		//label chain hash
s32 lbl_simhash[64];	//label chain hash (simulation)

char **lbln_name;		//named label names
u32 *lbln_id;			//named label IDs
s16 *lbln_chn;			//named label chain
s16 lbln_hash[256];
int nlbln, mlbln;
int nvlbln;

u32 *lblstr_ofs;	//named label names
u32 *lblstr_id;		//named label IDs
int nlblstr, mlblstr;

int pos_pad_op0;
int pos_pad_op1;
int pos_pad_op2;
int pos_pad_op3;

u32		arch_pad_op32;		//padding ".text" for 32-bit pads
u16		arch_pad_op16;		//padding ".text" for 16-bit pads
byte	arch_pad_op8;		//padding ".text" for 8-bit pads
u16		arch_pecoff_mach;

u32		arch_gprmask_scra;		//arch scratch registers
u32		arch_gprmask_save;		//arch preserve registers

u32		arch_fprmask_scra;		//arch scratch FPRs
u32		arch_fprmask_save;		//arch preserve FPRs

// byte reg_idx[BGBCC_SH_MAX_CACHEVAR];
// byte reg_reg[BGBCC_SH_MAX_CACHEVAR];
// int reg_live;
// int reg_resv;
u32 reg_save;
// int reg_dirty;
u32 freg_save;
u32 reg_vsave;
u32 freg_vsave;

int cnt_set_fp32;
int cnt_set_fp64;

int cnt_set_dq0;
int cnt_set_dq1;

int stat_tot_dq0;
int stat_tot_dq1;
int stat_tot_dqi;

int stat_tot_imm;
int stat_tot_imm8;
int stat_tot_imm16;
int stat_tot_imm8r;
int stat_tot_imm32;

int stat_ovlbl8;

int stat_opc_tot;
int stat_opc_base16;
int stat_opc_ext8a;
int stat_opc_ext8e;
int stat_opc_extCe;
int stat_opc_extCC0;
int stat_opc_extCC3;
byte stat_opc_issfx;


int sim_voffs;		//est' offset between real PC and sim PC

s32 dfl_fpscr;		//default FPSCR state
s32 cur_fpscr;		//current FPSCR state

s32 cur_srmode;		//current SR mode
byte dfl_dq;
byte use_wexmd;		//WEX Profile Mode

s64 image_base;		//image base address
int gbr_rva;		//GBR base address

int ofs_s16tab[256];
int ofs_s32tab[256];
byte idx_s16tab[256];
byte idx_s32tab[256];

s16 const_s16tab[256];
s32 const_s32tab[256];
u32 const_s16tab_rlc[256];	//reloc label type+ID
u32 const_s32tab_rlc[256];	//reloc label type+ID
byte const_ns16;
byte const_ns32;
byte nofs_s16tab;
byte nofs_s32tab;

int jitfl;

const byte *jcachereg;
const byte *qcachereg;
const byte *pcachereg;

const byte *fcachereg;
const byte *dcachereg;


byte reg_pszx[32];		//register sign/zero extension

ccxl_register regalc_map[32];
byte regalc_ltcnt[32];	//lifetime count (who to evict)
byte regalc_utcnt[32];	//current use count (0=unused)
u32 regalc_save;		//register has been saved and may hold a value
u32 regalc_live;		//register is currently holding a value
u32 regalc_dirty;

ccxl_register fregalc_map[32];
byte fregalc_ltcnt[32];	//lifetime count (who to evict)
byte fregalc_utcnt[32];	//current use count (0=unused)
u16 fregalc_save;		//register has been saved and may hold a value
u16 fregalc_live;		//register is currently holding a value
u16 fregalc_dirty;

u32 sreg_live;			//scratch registers live (per 3AC operation)
u32 sreg_held;			//scratch registers live-held (multiple ops)
u32 sfreg_live;			//scratch registers live (per 3AC operation)
u32 sfreg_held;			//scratch registers live-held (multiple ops)

int iflvl_t;			//number of branches in true set
int iflvl_f;			//number of branches in false set

BGBCC_CMG_VarSpan **vspan;
short *vspan_chn;
short vspan_hash[64];
int vspan_num;
int vspan_max;
int vsp_rsv;
int vsp_tcnt;

int frm_size;			//allocated size of frame
int frm_offs_lcl;		//frame offset of local vars
int frm_offs_tmp;		//frame offset of temporaries
int frm_offs_fix;		//fixed-size automatic
int lbl_ret;			//label ID for function exit
int lbl_got;			//label ID for got
int frm_offs_retstr;	//offset of return struct
int frm_offs_save;		//offset of register-save area

int frm_offs_thisptr;	//offset of 'this' pointer

int lbl_rom_data_strt;
int lbl_rom_data_end;

int lbl_gbl_ptr;			//label ID for got

int wex_ofs_begin;			//beginning of wexifier segment

BGBCC_CMG_Context *next;
struct BGBCC_TransState_s *tctx;
FILE *cgen_log;

char *csrept;
int cnrept;

BGBCC_CMG_EmitQueueOp *eqfree;
BGBCC_CMG_EmitQueueOp *eqstrt, *eqend;

int *got_gblidx;
int got_n_gblidx;
int got_m_gblidx;
int t_got_gblidx[4096];

int *lvt16_lbl;
s64 *lvt16_val;
int lvt16_n_idx;
int lvt16_m_idx;
int t_lvt16_lbl[1024];
s64 t_lvt16_val[1024*2];

int *genlabel_srcpos;
int genlabel_limit;
int t_genlabel_srcpos[4096];

/* Epilog Caching */
int epihash_key[1024];
int epihash_lbl[1024];

/* Prolog Caching */
int eprhash_key[1024];
int eprhash_lbl[1024];


int opcnt_hi8[256];
int opcnt_3xx[256];
int n_opcnt_3xx;

int opcnt_f0xx[256];
int opcnt_f1xx[256];
int opcnt_f2xx[256];
int opcnt_f3xx[256];
int opcnt_f8xx[256];
int opcnt_opw1;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct BGBCC_CMG_OpcodeArg_s {
byte ty;		//operand type
byte sc;		//scale for Reg/RM forms (1,2,3,4 for mem-ref)
byte ireg;		//index for Reg/RM forms
byte breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
char *name;		//name
};

struct BGBCC_CMG_VarSpan2_s {
int bbeg;				//begin (vop)
int bend;				//end (vop)
int tbeg;				//begin (vop)
int tend;				//end (vop)
int cnt;				//total times used
};

struct BGBCC_CMG_VarSpan_s {
ccxl_register reg;		//register in question
int bbeg;				//begin (vop)
int bend;				//end (vop)
int tbeg;				//begin (vop)
int tend;				//end (vop)
int cnt;				//total times used
int flag;				//span flags

BGBCC_CMG_VarSpan2 seq[64];
int nseq;
};

/*
 * Used to buffer instructions during emit.
 */
struct BGBCC_CMG_EmitQueueOp_s {
BGBCC_CMG_EmitQueueOp *next;
u16 nmid;
byte fmid;
byte rn;		//Rn
byte rm;		//Rm or Rs
byte ro;		//Ro or Rt
int imm;
int lbl;
};

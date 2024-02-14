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

typedef struct BGBCC_CMG_RWadEntry_s BGBCC_CMG_RWadEntry;


struct BGBCC_CMG_Context_s {
char *sec_name[64];
byte *sec_buf[64];
byte *sec_end[64];
byte *sec_pos[64];
byte *sec_vpos[64];
u32 sec_rva[64];		//relative virtual address (image offset)
u32 sec_lva[64];		//logical virtual address
u32 sec_lsz[64];		//logical size
byte sec;
byte nsec;

byte *asm_buf[64];
byte *asm_end[64];
byte *asm_pos[64];
byte do_asm;

byte is_le;			//is little endian
byte use_bp;		//use frame pointer
byte need_farjmp;	//function needs far32 jumps
byte need_f16jmp;	//function needs far16 jumps
byte need_n12jmp;	//function needs at least 12-bit jumps

byte need_n16bsr;	//image exceeds 16-bit BSR
byte need_n20bsr;	//image exceeds 20-bit BSR
byte need_n23bsr;	//image exceeds 23-bit BSR
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
byte is_leaftiny;	//function is a tiny leaf function
byte is_rom;		//building a ROM image
byte is_betav;		//uses BetaVe tweaks.
byte is_mergece;	//merge CC0e/CC3e into CExx
byte is_tr_leaf;	//given trace is a leaf
byte is_addr_x32;	//target uses 32-bit addresses (on 64-bit ISA)

byte is_prolog;		//emitting prolog
byte is_epilog;		//emitting epilog

byte no_fpu;		//no hardware FPU instructions
byte no_ext32;		//no 32-bit instruction forms
byte fpu_soft;		//use SoftFPU design.
byte fpu_lite;		//use LiteFPU design.
byte fpu_gfp;		//use GprFPU design.

byte is_fixed32;	//uses only 32-bit instruction forms
byte op_is_wex2;	//encoding a WEX2 instruction (0/2/3).
byte no_ops48;		//Disable 48-bit opcodes.
byte op_wex_align;	//Need to align for WEX
byte is_align_wexj;	//need to align for WEX and Jumbo

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
byte has_jumbo;		//has ADDS.L and similar
byte has_pushx2;	//has PUSH.X/POP.X and similar
byte has_simdx2;	//has 128-bit SIMD ops
// byte pel_cmpr;
byte no_wexify;		//has 128-bit SIMD ops

byte has_alux;		//has 128-bit ALU ops
byte has_fpux;		//has 128-bit FPU ops
byte has_ops24;		//has 24-bit opcodes
byte has_bra48;		//has 24-bit opcodes
byte has_xgpr;		//Has R32..R63
byte has_fmovs;		//Has FMOV.S
byte has_fmovc;		//Has MOV.C
byte has_dmacl;		//Has MACx.L / DMACx.L
byte has_qmul;		//Has MUL.Q / DIVS.Q / ...
byte has_ldop;		//Has FPU Immediates
byte has_fpim;		//Has FPU Immediates
byte has_fpvsf;		//Has FPU Immediates
byte has_jcmp;		//Has Compare+Branch

byte abi_evenonly;	//Pass register pairs in Even Registers.
byte abi_noexcept;	//Disable Exceptions
byte abi_spillpad;	//Disable Exceptions

byte do_shuffle;


byte rov_sreg;		//rover for scratch registers
byte rov_rshuf;		//rover for register shuffle

byte use_egpr;		//enable use of extended GPRs
byte maxreg_gpr;	//current number of GPR register-slots
byte maxreg_gpr_lf;	//current number of GPR register-slots (leaf)

byte use_efpr;		//enable use of extended FPRs
byte maxreg_fpr;	//current number of GPR register-slots
byte maxreg_fpr_lf;	//current number of GPR register-slots (leaf)

byte lmaxreg_gpr;		//current number of GPR register-slots
byte lmaxreg_gpr_lf;	//current number of GPR register-slots (leaf)

byte use_memmdl;	//use memory model
byte csrv_skip;		//skip a CSRV operation

byte use_emitqueue;	//use emit queue

byte emit_isprobe;	//if we are simply probing an instruction
byte emit_riscv;	//emit RISC-V instructions.

byte test_lclalign;	//local alignment-sensitivity test
byte use_padcross;	//use padding across cache lines

byte state_alias;

byte optmode;		//optimization mode
byte pred_tfmd;
byte verbose;		//verbosity level

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

int breakrov;

u32 *lbl_ofs;		//label offsets
u32 *rlc_ofs;		//reloc offsets
u32 *lbl_id;		//label IDs
u32 *rlc_id;		//reloc label IDs
byte *lbl_sec;		//label section
byte *rlc_sec;		//reloc section
byte *rlc_ty;		//reloc type
s32 *rlc_chn;		//reloc chains
s32 *lbl_chn;		//label chains (lblid)
s32 *lbl_cho;		//label chains (sec, ofs)
int nlbl, mlbl;
int nrlc, mrlc;
int nvlbl;
u16 lblrov;			//labels (local)
u16 lbltrov;		//labels (temp)
u16 lblllnrov;		//labels (line number)

s32 lbl_hash[1024];		//label chain hash (lblid)
s32 lbl_simhash[64];	//label chain hash (simulation)

s32 lbl_hashb[1024];	//label chain hash (sec, ofs)
s32 lbl_simhashb[64];	//label chain hash (simulation)

s32 rlc_hash[1024];		//reloc chain hash (sec, offs)

char **lbln_name;		//named label names
u32 *lbln_id;			//named label IDs
s16 *lbln_chn;			//named label chain
s16 *lbln_ichn;			//named label chain (by ID)
s16 lbln_hash[256];		//hash by name
s16 lbln_ihash[256];	//hash by ID
int nlbln, mlbln;
int nvlbln;

u32 *lblstr_ofs;	//named label names
u32 *lblstr_id;		//named label IDs
int nlblstr, mlblstr;

int pos_pad_op0;
int pos_pad_op1;
int pos_pad_op2;
int pos_pad_op3;

int pos_pad_op4;
int pos_pad_op5;
int pos_pad_op6;
int pos_pad_op7;

int emit_hist_word0;
int emit_hist_word1;
int emit_hist_word2;
int emit_hist_word3;

u32		arch_pad_op32;		//padding ".text" for 32-bit pads
u16		arch_pad_op16;		//padding ".text" for 16-bit pads
byte	arch_pad_op8;		//padding ".text" for 8-bit pads
u16		arch_pecoff_mach;

u64		arch_gprmask_scra;		//arch scratch registers
u64		arch_gprmask_save;		//arch preserve registers

u64		arch_fprmask_scra;		//arch scratch FPRs
u64		arch_fprmask_save;		//arch preserve FPRs

// byte reg_idx[BGBCC_SH_MAX_CACHEVAR];
// byte reg_reg[BGBCC_SH_MAX_CACHEVAR];
// int reg_live;
// int reg_resv;
u64 reg_save;
// int reg_dirty;
u64 freg_save;
u64 reg_vsave;
u64 freg_vsave;

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

int stat_opc_7xx;
int stat_opc_9xx;

int stat_const_maskhit;
int stat_const_maskjumbo;
int stat_const_masktot;

int stat_const_jumbo64;
int stat_const_jumbo96;

int stat_const_jumbo64_imm33l;
int stat_const_jumbo64_imm32h;
int stat_const_jumbo64_imm32c;
int stat_const_jumbo64_f32;
int stat_const_jumbo64_2xf16;
int stat_const_jumbo64_4xf8;

int stat_const_jumbo96ph;

int stat_fpimm_totchk5;
int stat_fpimm_hitchk5;

int stat_fpimm_totchk10;
int stat_fpimm_hitchk10;

int stat_ldst_disp12s;
int stat_ldst_disp10s;
int stat_ldst_disp9u;
int stat_ldst_disp5u;

int stat_ldst_disp5ub;
int stat_ldst_disp9ub;
int stat_ldst_disp10sb;
int stat_ldst_disp12sb;

int stat_ldst_disptot;


int stat_ldst_pbotot;
int stat_ldst_pbotot_ix;
int stat_ldst_pbotot_9b;
int stat_ldst_pbotot_10b;
int stat_ldst_pbotot_33b;

int stat_func_tot;
int stat_func_leaf;
int stat_func_leaftiny;
int stat_func_maxrsv;
int stat_func_partrsv;

int stat_func_alias;

int stat_func_acc_vsprsv;
int stat_func_acc_vspan;
int stat_func_acc_vspval;
int stat_func_acc_vspmax;

int stat_imm3ri_imm9u;
int stat_imm3ri_imm9n;
int stat_imm3ri_imm5u;
int stat_imm3ri_imm5n;
int stat_imm3ri_imm9un;
int stat_imm3ri_immtot;

int stat_imm2ri_imm6u;
int stat_imm2ri_imm6n;
int stat_imm2ri_imm10u;
int stat_imm2ri_imm10n;
int stat_imm2ri_imm10un;
int stat_imm2ri_immtot;

int stat_imm2ri_hmtot;
int stat_imm2ri_hmiss;
int stat_imm2ri_hjmb;

int stat_imm3ri_hmtot;
int stat_imm3ri_hmiss;
int stat_imm3ri_hjmb;


int stat_fp16_isfpa;
int stat_fp16_tot;
int stat_fp16_hit5;
int stat_fp16_hit5b;

int stat_fp16_exp[32];
int stat_funarg_exp[32];
int stat_funarg_tot;

int stat_varcount_exp[64];
int stat_varcount_tot;

int stat_lbl_tot;
int stat_lbl_disp8;
int stat_lbl_disp12;
int stat_lbl_disp16;
int stat_lbl_disp20;
int stat_lbl_disp24;
int stat_lbl_disp33;

int stat_rlbl_tot;
int stat_rlbl_disp8;
int stat_rlbl_disp12;
int stat_rlbl_disp16;
int stat_rlbl_disp20;
int stat_rlbl_disp24;
int stat_rlbl_disp33;

int ofs_curbdl;

int sim_voffs;		//est' offset between real PC and sim PC
int cur_fcnbase;

s32 dfl_fpscr;		//default FPSCR state
s32 cur_fpscr;		//current FPSCR state

s32 cur_srmode;		//current SR mode
byte dfl_dq;
byte use_wexmd;		//WEX Profile Mode
u16 use_wexdbg;		//WEX Debug Options

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

const short *jcachereg;
const short *qcachereg;
const short *pcachereg;
const short *lcachereg;

const short *fcachereg;
const short *dcachereg;

const short *shufreg0;
const short *shufreg1;
const short *shufreg2;
const short *shufreg3;

u64 regs_excl;
u64 regs_excl_tiny;

u64 memcpy64_mask;
u64 memcpy_byte_mask[8];

byte reg_pszx[64];		//register sign/zero extension

int reg_heatstat[64];	//register heat map

ccxl_register regalc_map[64];
short regalc_ltcnt[64];	//lifetime count (who to evict)
short regalc_utcnt[64];	//current use count (0=unused)
u64 regalc_save;		//register has been saved and may hold a value
u64 regalc_live;		//register is currently holding a value
u64 regalc_dirty;		//register is dirty
u64 regalc_noval;		//register lacks backing value
u64 regalc_pair;		//register is paired
u64 regalc_gbldirty;	//register is dirty (global)

u64 regalc_isarg;		//register is holding a call argument

ccxl_register fregalc_map[64];
short fregalc_ltcnt[64];	//lifetime count (who to evict)
short fregalc_utcnt[64];	//current use count (0=unused)
u64 fregalc_save;		//register has been saved and may hold a value
u64 fregalc_live;		//register is currently holding a value
u64 fregalc_dirty;

u64 sreg_live;			//scratch registers live (per 3AC operation)
u64 sreg_held;			//scratch registers live-held (multiple ops)
u64 sfreg_live;			//scratch registers live (per 3AC operation)
u64 sfreg_held;			//scratch registers live-held (multiple ops)

int iflvl_t;			//number of branches in true set
int iflvl_f;			//number of branches in false set

byte	reg_stflag[512];
byte	arg_stflag[512];
byte	lcl_stflag[512];

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
int lbl_ret_zero;		//label ID for function exit (return zero)
int lbl_got;			//label ID for got
int frm_offs_retstr;	//offset of return struct
int frm_offs_save;		//offset of register-save area
int frm_offs_save_rsv;	//offset of register-save area

int frm_offs_thisptr;	//offset of 'this' pointer
int frm_offs_sectoken;	//offset of security token
int frm_offs_allocamrk;	//offset of alloca mark
int frm_offs_isrsaves;	//offset of saved registers (ISR)
int frm_offs_datahi;	//High mark for data area
int frm_offs_datalo;	//Low mark for data area

s64 frm_val_sectoken;	//value used for security token.

s64 stat_tot_frm_size;		//total stack-frame space for all funcs
int stat_cnt_frm_size;		//total stack-frame space for all funcs


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

BGBCC_CMG_RWadEntry *rwad_dir;
int rwad_ndirent;
int rwad_mdirent;

int rwad_lblhead;
int rwad_lbldir;
int rwad_lbltype;

u32 rwad_types[256];
u16 rwad_hash[128];

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

int *dbglln_srcpos;
int dbglln_limit;
// int t_dbglln_srcpos[4096];

/* Epilog Caching */
u64 epihash_key[1024];
int epihash_lbl[1024];
int epihash_rov;

/* Prolog Caching */
u64 eprhash_key[1024];
int eprhash_lbl[1024];
int eprhash_rov;


/* TLS Section. */

int tlsvar_lbl[512];
int tlsvar_ofs[512];
int tlsvar_sz[512];
int n_tlsvar;
int sz_tlsvar;
int tlsi_lbl;		//label for TLS Index



int opcnt_hi8[256];
int opcnt_3xx[256];
int n_opcnt_3xx;

int opcnt_f03xx[256];
int n_opcnt_f03xx;

int opcnt_f01xx[256];
int n_opcnt_f01xx;

// int opcnt_7xx[256];
// int opcnt_9xx[256];

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
s16 ireg;		//index for Reg/RM forms
s16 breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
char *name;		//name
};

struct BGBCC_CMG_VarSpan2_s {
int bbeg;				//begin (vop)
int bend;				//end (vop)
int tbeg;				//begin (trace)
int tend;				//end (trace)
int cnt;				//total times used
int flag;				//sub-span flags
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
s16 rn;		//Rn
s16 rm;		//Rm or Rs
s16 ro;		//Ro or Rt
s64 imm;
int lbl;
};

struct BGBCC_CMG_RWadEntry_s {
char name[17];
int lbl;
int csize;
int dsize;
byte ety;
byte cmp;
u16 flags;
u16 chain;
};

typedef struct
{
u32 magic;
int numlumps;
int diroffs;
int typeoffs;
} wad2head_t;

typedef struct
{
u32 fileoffs;
u32 csize;
u32 dsize;
byte ety;
byte cmp;
u16 chn;
char name[16];
} wad2lump_t;

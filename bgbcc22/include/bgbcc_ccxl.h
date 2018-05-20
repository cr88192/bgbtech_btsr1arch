//0-15=Core Primitive Types
//Types 16-255=Extended Core Types
//Types 256-4095=User Types (structs/unions/...)
#define CCXL_TY_I				0x00		//Int
#define CCXL_TY_L				0x01		//Long (64-bit)
#define CCXL_TY_F				0x02		//Float
#define CCXL_TY_D				0x03		//Double
#define CCXL_TY_P				0x04		//Pointer (abstract)
#define CCXL_TY_V				0x05		//Void
#define CCXL_TY_S				0x06		//Struct / Union
#define CCXL_TY_NL				0x07		//Native Long (32/64 bit)
#define CCXL_TY_SB				0x08		//Signed Byte
#define CCXL_TY_UB				0x09		//Unsigned Byte
#define CCXL_TY_SS				0x0A		//Signed Short
#define CCXL_TY_US				0x0B		//Unsigned Short
#define CCXL_TY_UI				0x0C		//Unsigned Int
#define CCXL_TY_UL				0x0D		//Unsigned Long (64-bit)
#define CCXL_TY_UNL				0x0E		//Native Long (32/64 bit)
#define CCXL_TY_UNDEF_I			0x0F		//Undefined (Implicit Int)
//Types 16-255=Extended Core Types
#define CCXL_TY_I128			0x10		//Int128
#define CCXL_TY_UI128			0x11		//Unsigned Int128
#define CCXL_TY_F128			0x12		//Float128
#define CCXL_TY_F16				0x13		//Float16
#define CCXL_TY_VARIANT			0x14		//Variant
#define CCXL_TY_VARARGS			0x15		//VarArgs
#define CCXL_TY_VALIST			0x16		//va_list

#define CCXL_VTY_PCHAR			0x00001008	//'char *'
#define CCXL_VTY_PWCHAR			0x0000100B	//'wchar_t *'


//Basic Type
#define CCXL_TY_BASEMASK		0x00000FFF	//base type or struct
#define CCXL_TY_PTRMASK			0x0000F000	//pointer level
#define CCXL_TY_PTRIDX1			0x00001000	//pointer level
#define CCXL_TY_PTRSHL			12			//pointer level shl
#define CCXL_TY_ARRMASK			0x0FFF0000	//small array size
#define CCXL_TY_ARRSHL			16			//array size shl

#define CCXL_TY_BASETYMAX		4096		//base type limit
#define CCXL_TY_BASEARRMAX		4096		//base array limit

#define CCXL_TY_TYTY_MASK		0xF0000000	//type-type mask
#define CCXL_TY_TYTY_BASIC		0x00000000	//type-type basic
#define CCXL_TY_TYTY_BASIC2		0x10000000	//type-type basic2
#define CCXL_TY_TYTY_OVF1		0x20000000	//type-type overflow
#define CCXL_TY_TYTY_BASIC3		0x30000000	//type-type basic3

#define CCXL_TYOVF_IDXMASK		0x0000FFFF	//type overflow index

//Basic2 Type
#define CCXL_TYB2_BASEMASK		0x0000003F	//base type or struct
#define CCXL_TYB2_PTRMASK		0x000000C0	//pointer level
#define CCXL_TYB2_PTRIDX1		0x00000040	//pointer level
#define CCXL_TYB2_ARRMASK		0x0FFFFF00	//small array size
#define CCXL_TYB2_PTRSHL		6			//pointer level shl
#define CCXL_TYB2_ARRSHL		8			//array size shl

#define CCXL_TYB2_BASETYMAX		64			//base type limit
#define CCXL_TYB2_BASEARRMAX	1048576		//base array limit

//Basic3 Type
#define CCXL_TYB3_BASEMASK		0x000FFFFF	//base type or struct
#define CCXL_TYB3_PTRMASK		0x00300000	//pointer level
#define CCXL_TYB3_PTRIDX1		0x00100000	//pointer level
#define CCXL_TYB3_ARRMASK		0x0FC00000	//small array size
#define CCXL_TYB3_PTRSHL		20			//pointer level shl
#define CCXL_TYB3_ARRSHL		22			//array size shl
#define CCXL_TYB3_BASETYMAX		1048576		//base type limit
#define CCXL_TYB3_BASEARRMAX	64			//base array limit


#define CCXL_REGTY_REGMASK			0xFF00000000000000ULL
#define CCXL_REGTY_TEMP				0x0000000000000000ULL
#define CCXL_REGTY_ARG				0x0100000000000000ULL
#define CCXL_REGTY_LOCAL			0x0200000000000000ULL
#define CCXL_REGTY_GLOBAL			0x0300000000000000ULL

#define CCXL_REGTY_IMM_INT			0x0400000000000000ULL
#define CCXL_REGTY_IMM_LONG			0x0500000000000000ULL
#define CCXL_REGTY_IMM_FLOAT		0x0600000000000000ULL
#define CCXL_REGTY_IMM_DOUBLE		0x0700000000000000ULL
#define CCXL_REGTY_IMM_STRING		0x0800000000000000ULL
#define CCXL_REGTY_IMM_WSTRING		0x0900000000000000ULL
#define CCXL_REGTY_IMM_LITERAL		0x0A00000000000000ULL
#define CCXL_REGTY_IMM_GBLADDR		0x0B00000000000000ULL
#define CCXL_REGTY_IMM_LONG_LVT		0x0C00000000000000ULL	//long in LVT
#define CCXL_REGTY_IMM_DOUBLE_LVT	0x0D00000000000000ULL	//double in LVT
#define CCXL_REGTY_IMM_I128_LVT		0x0E00000000000000ULL	//pair of indices
#define CCXL_REGTY_IMM_F128_LVT		0x0F00000000000000ULL	//pair of indices

#define CCXL_REGTY2_TYMASK			0xE000000000000000ULL
#define CCXL_REGTY2_IMM_LONG		0x2000000000000000ULL	//long(61b)
#define CCXL_REGTY2_IMM_DOUBLE		0x4000000000000000ULL	//double(61b)

//#define CCXL_REGID_REGMASK		0x000000000000FFFFULL
//#define CCXL_REGID_TYPEMASK		0x0000FFFFFFFF0000ULL
//#define CCXL_REGID_TYPESHIFT	16

#define CCXL_REGID_REGMASK		0x0000000000FFFFFFULL
#define CCXL_REGID_TYPEMASK		0x00FFFFFFFF000000ULL
#define CCXL_REGID_TYPESHIFT	24

#define CCXL_REGID_BASEMASK		0x0000000000000FFFULL	//basic register number
#define CCXL_REGID_SEQMASK		0x0000000000FFF000ULL	//register sequence

#define CCXL_REGINT_MASK		0x00000000FFFFFFFFULL	//int/float
#define CCXL_REGLONG_MASK		0x00FFFFFFFFFFFFFFULL	//long/double (non-LVT)

#define CCXL_REGINTPL_MASK		0x000000000FFFFFFFULL	//LVT (low half index)
#define CCXL_REGINTPH_MASK		0x00FFFFFFF0000000ULL	//LVT (high half index)
#define CCXL_REGINTPH_SHL		28

#define CCXL_REGID_REG_Z		0x0000000000FFFFFFULL	//placeholder void reg
#define CCXL_REGID_REG_DZ		0x0000000002FFFFFFULL	//multi-part regs

#define CCXL_REGID_REG_TYZ		0x0000000000FF0FFFULL	//type-only placeholder

#define CCXL_REGLONG2_MASK		0x1FFFFFFFFFFFFFFFULL	//long2/double2

#define CCXL_LITID_STRUCT		1
#define CCXL_LITID_UNION		2
#define CCXL_LITID_TYPEDEF		3
#define CCXL_LITID_FUNCTION		4
#define CCXL_LITID_GLOBALVAR	5
#define CCXL_LITID_ARGS			6
#define CCXL_LITID_LOCALS		7
#define CCXL_LITID_VAR			8
#define CCXL_LITID_LIST			9
#define CCXL_LITID_VALUE		10
#define CCXL_LITID_CLASS		11
#define CCXL_LITID_STATICVAR	12
#define CCXL_LITID_ASMFUNC		13
#define CCXL_LITID_ASMBLOB		14
#define CCXL_LITID_MANIFOBJ		15

#define CCXL_LITID_ARR1			16
#define CCXL_LITID_ARR2			17
#define CCXL_LITID_ARR4			18
#define CCXL_LITID_ARR8			19
#define CCXL_LITID_ARR16		20
#define CCXL_LITID_ASTD			21
#define CCXL_LITID_PROTOTYPE	22
#define CCXL_LITID_EXPLIST		23			//DLL export list

#define CCXL_STATUS_YES					1
#define CCXL_STATUS_NO					0
#define CCXL_STATUS_ERR_GENERIC			-1
#define CCXL_STATUS_ERR_LOOKUPFAIL		-2
#define CCXL_STATUS_ERR_BADOPCODE		-3
#define CCXL_STATUS_ERR_BADVALUE		-4
#define CCXL_STATUS_ERR_UNHANDLEDTYPE	-5
#define CCXL_STATUS_ERR_BADOPARGS		-6
#define CCXL_STATUS_ERR_CANTRELEASE		-7
#define CCXL_STATUS_ERR_CANTACQUIRE		-8
#define CCXL_STATUS_ERR_UNSUPPORTED		-9
#define CCXL_STATUS_ERR_UNIMPLEMENTED	-10

#define CCXL_CMD_VARVALUE				0x8001
#define CCXL_CMD_FUNCTION				0x8002
#define CCXL_CMD_METHOD					0x8003
#define CCXL_CMD_BODY					0x8004
#define CCXL_CMD_ARGS					0x8005
#define CCXL_CMD_LOCALS					0x8006
#define CCXL_CMD_VARARGS				0x8007
#define CCXL_CMD_PROTOTYPE				0x8008
#define CCXL_CMD_STRUCT					0x8009
#define CCXL_CMD_UNION					0x800A
#define CCXL_CMD_CLASS					0x800B
#define CCXL_CMD_EXTENDS				0x800C
#define CCXL_CMD_IMPLEMENTS				0x800D
#define CCXL_CMD_S_PROTOTYPE			0x800E
#define CCXL_CMD_LIST					0x800F
#define CCXL_CMD_STATICVARDECL			0x8010
#define CCXL_CMD_VARDECL				0x8011
#define CCXL_CMD_MANIFOBJ				0x8012
#define CCXL_CMD_TYPEDEF				0x8013

#define CCXL_ATTR_SIG					0x9001
#define CCXL_ATTR_FLAGS					0x9002
#define CCXL_ATTR_VALUE					0x9003
#define CCXL_ATTR_SRCTOK				0x9004
#define CCXL_ATTR_NAME					0x9005
#define CCXL_ATTR_TUIDX					0x9006

#define CCXL_TERR_GENERIC				0xA000
#define CCXL_TERR_STACK_OVERFLOW		0xA001
#define CCXL_TERR_STACK_UNDERFLOW		0xA002
#define CCXL_TERR_FORMAT_SANITY			0xA003

#define CCXL_TERR_CONV_PTRRANGELOSS		0xA004
#define CCXL_TERR_CONV_PTRSIZEDIFF		0xA005

#define CCXL_TERR_STATUS(st)			(0xA800+(st))

#define CCXL_BINOP_ADD			0x00
#define CCXL_BINOP_SUB			0x01
#define CCXL_BINOP_MUL			0x02
#define CCXL_BINOP_DIV			0x03
#define CCXL_BINOP_MOD			0x04
#define CCXL_BINOP_AND			0x05
#define CCXL_BINOP_OR			0x06
#define CCXL_BINOP_XOR			0x07
#define CCXL_BINOP_SHL			0x08
#define CCXL_BINOP_SHR			0x09
#define CCXL_BINOP_SHRR			0x0A
#define CCXL_BINOP_MULH			0x0B
#define CCXL_BINOP_UMULH		0x0C
#define CCXL_BINOP_UDIV			0x0D

#define CCXL_UNOP_MOV			0x00
#define CCXL_UNOP_NEG			0x01
#define CCXL_UNOP_NOT			0x02
#define CCXL_UNOP_LNOT			0x03
#define CCXL_UNOP_INC			0x04
#define CCXL_UNOP_DEC			0x05

#define CCXL_CMP_EQ				0x00
#define CCXL_CMP_NE				0x01
#define CCXL_CMP_LT				0x02
#define CCXL_CMP_GT				0x03
#define CCXL_CMP_LE				0x04
#define CCXL_CMP_GE				0x05
#define CCXL_CMP_AL				0x06
#define CCXL_CMP_NV				0x07
#define CCXL_CMP_TST			0x08		//if(x&y)
#define CCXL_CMP_NTST			0x09		//if(!(x&y))

#define CCXL_VOP_NONE				0x00
#define CCXL_VOP_DBGFN				0x01
#define CCXL_VOP_DBGLN				0x02
#define CCXL_VOP_LABEL				0x03
#define CCXL_VOP_JMP				0x04
#define CCXL_VOP_MOV				0x05
#define CCXL_VOP_JCMP_ZERO			0x06
#define CCXL_VOP_JCMP				0x07
#define CCXL_VOP_CALL				0x08
#define CCXL_VOP_CSRV				0x09
#define CCXL_VOP_RETDFL				0x0A
#define CCXL_VOP_RETV				0x0B
#define CCXL_VOP_RET				0x0C
#define CCXL_VOP_CONV				0x0D
#define CCXL_VOP_UNARY				0x0E
#define CCXL_VOP_BINARY				0x0F
#define CCXL_VOP_COMPARE			0x10
#define CCXL_VOP_LDIXIMM			0x11
#define CCXL_VOP_STIXIMM			0x12
#define CCXL_VOP_LDIX				0x13
#define CCXL_VOP_STIX				0x14
#define CCXL_VOP_LEAIMM				0x15
#define CCXL_VOP_LEA				0x16
#define CCXL_VOP_LDAVAR				0x17
#define CCXL_VOP_SIZEOFVAR			0x18
#define CCXL_VOP_DIFFPTR			0x19
#define CCXL_VOP_OFFSETOF			0x1A
#define CCXL_VOP_LOADSLOT			0x1B
#define CCXL_VOP_STORESLOT			0x1C
#define CCXL_VOP_LOADSLOTADDR		0x1D
#define CCXL_VOP_LOADSLOTADDRID		0x1E
#define CCXL_VOP_INITOBJ			0x1F
#define CCXL_VOP_DROPOBJ			0x20
#define CCXL_VOP_INITARR			0x21
#define CCXL_VOP_INITOBJARR			0x22
#define CCXL_VOP_LOADINITARR		0x23
#define CCXL_VOP_LOADINITOBJARR		0x24

#define CCXL_VOP_VA_START			0x25
#define CCXL_VOP_VA_END				0x26
#define CCXL_VOP_VA_ARG				0x27

#define CCXL_VOP_CSELCMP			0x28
#define CCXL_VOP_CSELCMP_Z			0x29

#define CCXL_LBL_GLOBALBASE			0x000000	//globals (main context)

#define CCXL_LBL_GENSYMBASE			0x800000	//gensyms (middle)
#define CCXL_LBL_GENSYM2BASE		0xC00000	//gensyms (backend, stable)
#define CCXL_LBL_GENSYM2BASE2		0xE00000	//gensyms (backend, temp)


typedef struct { u64 val; } ccxl_register;
typedef struct { u32 val; } ccxl_type;
typedef struct { u32 id; } ccxl_label;

typedef union {
	u32 ui;
	s32 si;
	u64 ul;
	s64 sl;
	f32 f;
	f64 d;
	char *str;
	struct {
		byte na;
		byte ca;
		ccxl_register *args;
	}call;
	struct {
		int gid;
		int fid;
	}obj;
}ccxl_value;

typedef int ccxl_status;
// typedef int ccxl_label;


typedef struct BGBCC_CCXL_RegisterInfo_s BGBCC_CCXL_RegisterInfo;
typedef struct BGBCC_CCXL_LiteralInfo_s BGBCC_CCXL_LiteralInfo;
typedef struct BGBCC_CCXL_TypeOverflow_s BGBCC_CCXL_TypeOverflow;
typedef struct BGBCC_CCXL_BackendFuncs_vt_s BGBCC_CCXL_BackendFuncs_vt;

typedef struct BGBCC_CCXL_VirtOp_s BGBCC_CCXL_VirtOp;
typedef struct BGBCC_CCXL_VirtTr_s BGBCC_CCXL_VirtTr;

struct BGBCC_CCXL_RegisterInfo_s {
BGBCC_CCXL_RegisterInfo *next;
BGBCC_CCXL_RegisterInfo *hashnext;		//next in lookup hash
char *pbname;			//scope path base name
char *name;				//assigned variable name
char *qname;			//assigned qualified name
char *sig;				//type signature
char *flagstr;			//flag string
ccxl_type type;			//assigned type
byte ucnt;				//use count
byte type_zb;			//type Z base
byte regcls;			//register class (backend)
s16 cseq;				//current sequence
int regtype;			//register type
int regid;				//register ID
int validx;				//value index
int srctok;				//source tokens
s64 flagsint;			//flags (integer)
ccxl_register value;	//literal value

byte *text;				//bytecode
int sz_text;			//sizeof bytecode

char *alc_fn;
int alc_ln;

BGBCC_CCXL_RegisterInfo *defv;		//define var
BGBCC_CCXL_RegisterInfo *defp;		//define parent

BGBCC_CCXL_RegisterInfo **fields;
BGBCC_CCXL_RegisterInfo **args;
BGBCC_CCXL_RegisterInfo **locals;
BGBCC_CCXL_RegisterInfo **regs;
// u32 *regs_tyseq;
int n_fields, m_fields;
int n_args, m_args;
int n_locals, m_locals;
int n_regs, m_regs;
int n_cargs;		//max number of called-function arguments
int n_eargs;		//end args
int n_vargs;		//virtual args

BGBCC_CCXL_VirtOp **vop;
BGBCC_CCXL_VirtTr **vtr;
int n_vop, m_vop;
int n_vtr, m_vtr;
// int s_vop;

char **goto_name;
ccxl_label *goto_lbl;
int n_goto, m_goto;

ccxl_register *listdata;
int n_listdata, m_listdata;
int gblid, regflags;
int fxsize;				//fixed-format size
int fxoffs;				//fixed-format offset (frame var offset)
int fxmoffs, fxnoffs;	//fixed min/max size
int fxmsize, fxnsize;	//fixed min/max size
int fxmalgn, fxnalgn;	//fixed min/max size
};

struct BGBCC_CCXL_LiteralInfo_s {
char *name;
char *sig;
int littype;		//literal type
int litid;			//literal ID
int hnext_name;		//next by hashed name

BGBCC_CCXL_LiteralInfo *parent;
BGBCC_CCXL_RegisterInfo *decl;
};

struct BGBCC_CCXL_TypeOverflow_s {
int base;
int asz[16];
byte pn, an;
};

struct BGBCC_CCXL_VirtOp_s {
byte opn;
byte opr;
ccxl_type type;
ccxl_type stype;
ccxl_register dst;
ccxl_register srca;
ccxl_register srcb;
ccxl_register srcc;
ccxl_register srcd;
ccxl_value imm;
};

struct BGBCC_CCXL_VirtTr_s {
int b_ops;			//first opcode
int n_ops;			//num opcodes
};



bccx_cxstate bgbcc_rcst_anytype;
bccx_cxstate bgbcc_rcst_arch;
bccx_cxstate bgbcc_rcst_args;
bccx_cxstate bgbcc_rcst_argdecls;
bccx_cxstate bgbcc_rcst_array;
bccx_cxstate bgbcc_rcst_asm_blob;
bccx_cxstate bgbcc_rcst_assign;
bccx_cxstate bgbcc_rcst_attr;
bccx_cxstate bgbcc_rcst_attribute;
bccx_cxstate bgbcc_rcst_attributes;

bccx_cxstate bgbcc_rcst_begin;
bccx_cxstate bgbcc_rcst_binary;
bccx_cxstate bgbcc_rcst_bits;
bccx_cxstate bgbcc_rcst_blockcomment;
bccx_cxstate bgbcc_rcst_body;
bccx_cxstate bgbcc_rcst_boolify;
bccx_cxstate bgbcc_rcst_break;

bccx_cxstate bgbcc_rcst_case;
bccx_cxstate bgbcc_rcst_case_default;
bccx_cxstate bgbcc_rcst_cast;
bccx_cxstate bgbcc_rcst_charstring;
bccx_cxstate bgbcc_rcst_class;
bccx_cxstate bgbcc_rcst_classdef;
bccx_cxstate bgbcc_rcst_comma;
bccx_cxstate bgbcc_rcst_complex;
bccx_cxstate bgbcc_rcst_cond;
bccx_cxstate bgbcc_rcst_constructor;
bccx_cxstate bgbcc_rcst_continue;

bccx_cxstate bgbcc_rcst_declspec;
bccx_cxstate bgbcc_rcst_def;
bccx_cxstate bgbcc_rcst_defs;
bccx_cxstate bgbcc_rcst_default;
bccx_cxstate bgbcc_rcst_defun;
bccx_cxstate bgbcc_rcst_doc_comment;
bccx_cxstate bgbcc_rcst_do_while;

bccx_cxstate bgbcc_rcst_else;
bccx_cxstate bgbcc_rcst_emptyindex;
bccx_cxstate bgbcc_rcst_enum;
bccx_cxstate bgbcc_rcst_enumdef;
bccx_cxstate bgbcc_rcst_expr;
bccx_cxstate bgbcc_rcst_extern2_lang;

bccx_cxstate bgbcc_rcst_flags;
bccx_cxstate bgbcc_rcst_float128;
bccx_cxstate bgbcc_rcst_fn;
bccx_cxstate bgbcc_rcst_for;
bccx_cxstate bgbcc_rcst_fproto;
bccx_cxstate bgbcc_rcst_func;
bccx_cxstate bgbcc_rcst_funcall;

bccx_cxstate bgbcc_rcst_gcc_asm;
bccx_cxstate bgbcc_rcst_getindex;
bccx_cxstate bgbcc_rcst_goto;

bccx_cxstate bgbcc_rcst_if;
bccx_cxstate bgbcc_rcst_imag;
bccx_cxstate bgbcc_rcst_impl;
bccx_cxstate bgbcc_rcst_import;
bccx_cxstate bgbcc_rcst_ind;
bccx_cxstate bgbcc_rcst_index;
bccx_cxstate bgbcc_rcst_init;
bccx_cxstate bgbcc_rcst_int;
bccx_cxstate bgbcc_rcst_int128;
bccx_cxstate bgbcc_rcst_interface;
bccx_cxstate bgbcc_rcst_iproto;

bccx_cxstate bgbcc_rcst_label;
bccx_cxstate bgbcc_rcst_lang;
bccx_cxstate bgbcc_rcst_left;
bccx_cxstate bgbcc_rcst_linecomment;
bccx_cxstate bgbcc_rcst_list;
bccx_cxstate bgbcc_rcst_ln;
bccx_cxstate bgbcc_rcst_loadindex;

bccx_cxstate bgbcc_rcst_methodcall;
bccx_cxstate bgbcc_rcst_module;
bccx_cxstate bgbcc_rcst_msvc_asm;

bccx_cxstate bgbcc_rcst_name;
bccx_cxstate bgbcc_rcst_namespace;
bccx_cxstate bgbcc_rcst_new;
bccx_cxstate bgbcc_rcst_null_statement;

bccx_cxstate bgbcc_rcst_objref;
bccx_cxstate bgbcc_rcst_offsetof;
bccx_cxstate bgbcc_rcst_op;

bccx_cxstate bgbcc_rcst_package;
bccx_cxstate bgbcc_rcst_postdec;
bccx_cxstate bgbcc_rcst_postinc;
bccx_cxstate bgbcc_rcst_pragma;
bccx_cxstate bgbcc_rcst_predec;
bccx_cxstate bgbcc_rcst_preinc;
bccx_cxstate bgbcc_rcst_property;
bccx_cxstate bgbcc_rcst_proto;

bccx_cxstate bgbcc_rcst_real;
bccx_cxstate bgbcc_rcst_ref;
bccx_cxstate bgbcc_rcst_rest;
bccx_cxstate bgbcc_rcst_return;
bccx_cxstate bgbcc_rcst_right;

bccx_cxstate bgbcc_rcst_sharpcall;
bccx_cxstate bgbcc_rcst_size;
bccx_cxstate bgbcc_rcst_sizeof;
bccx_cxstate bgbcc_rcst_sizeof_expr;
bccx_cxstate bgbcc_rcst_step;
bccx_cxstate bgbcc_rcst_string;
bccx_cxstate bgbcc_rcst_struct;
bccx_cxstate bgbcc_rcst_structs;
bccx_cxstate bgbcc_rcst_subarch;
bccx_cxstate bgbcc_rcst_super;
bccx_cxstate bgbcc_rcst_switch;
bccx_cxstate bgbcc_rcst_s_proto;

bccx_cxstate bgbcc_rcst_then;
bccx_cxstate bgbcc_rcst_throw;
bccx_cxstate bgbcc_rcst_tokens;
bccx_cxstate bgbcc_rcst_try_catch;
bccx_cxstate bgbcc_rcst_tyexpr;
bccx_cxstate bgbcc_rcst_type;
bccx_cxstate bgbcc_rcst_types;
bccx_cxstate bgbcc_rcst_tysuf;

bccx_cxstate bgbcc_rcst_unary;
bccx_cxstate bgbcc_rcst_union;
bccx_cxstate bgbcc_rcst_using;

bccx_cxstate bgbcc_rcst_value;
bccx_cxstate bgbcc_rcst_value_hi;
bccx_cxstate bgbcc_rcst_value_lo;
bccx_cxstate bgbcc_rcst_var;
bccx_cxstate bgbcc_rcst_vars;
bccx_cxstate bgbcc_rcst_vector_ref;

bccx_cxstate bgbcc_rcst_while;

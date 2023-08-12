/*
Codegen / 3AC stage stuff.

Local:
  (23:0): Index
  (55:24): Type

Local Space:
  0..3: Special
    0: ZR
    1: 'this'
  4..(4+Na-1): Arguments
  4+Na: Locals
  4+Na+Nl: Temporaries
  

Global:
  (23:0): Global Index
  (55:24): Type

SmallInt (Values that map into 32-bit range):
  (31:0): Literal Value
  (55:32): ShortType

LongInt (Values that exceed 32-bit range):
  (15: 0): Index (Low)
  (31:16): Index (High)
  (55:32): ShortType


Type:
  000..0FF: Built-In Types
  100..FFF: Complex/Structural Types


3AC IR OP, Serialized:
  UVLI fieldmask;
  UVLI opid;	//mask&01: op | op+opr
  SVLI dty;		//mask&02
  SVLI sty;		//mask&20 (CONV)
  UVLI vd;		//mask&04 (Dest)
  UVLI va;		//mask&08 (Src A)
  UVLI vb;		//mask&10 (Src B)
  UVLI vc;		//mask&40

IR will be organized into blocks:
  UVLI opcnt;	//number of ops in block
  UVLI lbl;		//label sequence
If lbl and opcnt are 0, this is the end of the function body.

va/vb/vc/vd will encode a value field:
  000: Local (Idx/Seq=Morton)
  010: Global (Linear)
  100: SmallIt (No Type, Linear)
  110: LongIt (No Type, Index=Morton)

Types will be expressed as:
  ty>0: Offset of signature in string table.
    The type for the signature string will be added to a table.
  ty<0: Index into a table of recently seen signature strings.


Functions will be expressed as "declaration blobs":
  Name, Sig, Flags
  Argument List;
  Locals List;
  Body.

  Decl Blob:
    UVLI fmask;		//0=Invalid
    SVLI name;		//(mask&01) offset into string table
    SVLI sig;		//(mask&02) offset into string table
    SVLI flagstr;	//(mask&04) offset into string table
    UVLI value;		//(mask&08) value field (variable)
    ArgList;		//(mask&10) argument list (proto/func)
    if(mask&20)		//body present (func)
		LocalList;	//local variables
		TempList;	//temporaries
		Body;		//function body

  Arg/Locals List:
    UVLI fmask;		//0=End of List
    SVLI name;		//(mask&01) offset into string table
    SVLI sig;		//(mask&02) offset into string table
    SVLI flagstr;	//(mask&04) offset into string table
    UVLI value;		//(mask&08) value field

 */

#define TKUCC_VALREG_VTYPE_MSK			0xFF00000000000000ULL
#define TKUCC_VALREG_VTYPE_LOCAL		0x0000000000000000ULL
#define TKUCC_VALREG_VTYPE_GLOBAL		0x0100000000000000ULL
#define TKUCC_VALREG_VTYPE_SMALLIT		0x0200000000000000ULL
#define TKUCC_VALREG_VTYPE_LONGIT		0x0300000000000000ULL

#define TKUCC_VALTYPE_BASE_MSK			0x00000FFF
#define TKUCC_VALTYPE_PTR_MSK			0x0000F000
#define TKUCC_VALTYPE_ARR_MSK			0x0FFF0000
#define TKUCC_VALTYPE_TYTY_MSK			0xF0000000

#define TKUCC_VALTYPE_TY_UNDEF			0x000		//Undefined type.
#define TKUCC_VALTYPE_TY_VOID			0x001
#define TKUCC_VALTYPE_TY_SB				0x002
#define TKUCC_VALTYPE_TY_UB				0x003
#define TKUCC_VALTYPE_TY_SS				0x004
#define TKUCC_VALTYPE_TY_US				0x005
#define TKUCC_VALTYPE_TY_SI				0x006
#define TKUCC_VALTYPE_TY_UI				0x007
#define TKUCC_VALTYPE_TY_SNL			0x008
#define TKUCC_VALTYPE_TY_UNL			0x009
#define TKUCC_VALTYPE_TY_SL				0x00A
#define TKUCC_VALTYPE_TY_UL				0x00B
#define TKUCC_VALTYPE_TY_SI128			0x00C
#define TKUCC_VALTYPE_TY_UI128			0x00D
#define TKUCC_VALTYPE_TY_VAR			0x00E
#define TKUCC_VALTYPE_TY_BF16			0x00F
#define TKUCC_VALTYPE_TY_F16			0x010
#define TKUCC_VALTYPE_TY_F				0x011
#define TKUCC_VALTYPE_TY_D				0x012
#define TKUCC_VALTYPE_TY_F128			0x013
#define TKUCC_VALTYPE_TY_VARARG			0x014



#define TKUCC_REGMASK_PRESERVE		0xFF00FF00FF007F00ULL
#define TKUCC_REGMASK_ARGUMENT		0x00F000F000F000F0ULL
#define TKUCC_REGMASK_SCRATCH		0x00FF00FF00FC00FCULL

#define TKUCC_BINOP_ADD		0x00
#define TKUCC_BINOP_SUB		0x01
#define TKUCC_BINOP_MUL		0x02
#define TKUCC_BINOP_DIV		0x03
#define TKUCC_BINOP_MOD		0x04
#define TKUCC_BINOP_AND		0x05
#define TKUCC_BINOP_OR		0x06
#define TKUCC_BINOP_XOR		0x07
#define TKUCC_BINOP_SHL		0x08
#define TKUCC_BINOP_SHR		0x09

#define TKUCC_BINOP_CMPEQ	0x0A
#define TKUCC_BINOP_CMPNE	0x0B
#define TKUCC_BINOP_CMPGT	0x0C
#define TKUCC_BINOP_CMPGE	0x0D
#define TKUCC_BINOP_CMPLT	0x0E
#define TKUCC_BINOP_CMPLE	0x0F


struct TKUCC_IROP_s {
TKUCC_IROP *next;
short cmd;
short opr;
int lbl;
tkucc_valtype dty;
tkucc_valtype sty;
tkucc_valreg va;
tkucc_valreg vb;
tkucc_valreg vc;
tkucc_valreg vd;
};

struct TKUCC_IRBB_s {
TKUCC_IRBB *next;
TKUCC_IROP *op;
int lbl;
};

struct TKUCC_IRDECL_s {
TKUCC_IRDECL *next;
TKUCC_IRDECL *ihnext;	//by index
TKUCC_IRDECL *nhnext;	//by name
char *name;
char *sig;
char *flagstr;

u64 flags;
tkucc_valtype ty;

int idx;	//index number (local or global)
int sz;		//storage size
int ofs;	//storage offset
int lbl;

TKUCC_IRDECL	*framevars;
};

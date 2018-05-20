#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// #include <bgbgc.h>
// #include <bgbdy.h>

#ifndef BGBCCC_H
#define BGBCCC_H

#include <bgbcc_conf.h>

#ifdef WIN32
#ifdef BGBCC_DLL
#define BGBCC_API __declspec(dllexport)
#else
#define BGBCC_API __declspec(dllimport)
#endif
#else
#define BGBCC_API
#endif

#ifdef BGBCC_DLL
#ifdef linux
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif
#endif

#ifdef WIN32
#define BGBCC_EXCEPTION		0xEBADB0F0
#endif

#ifndef BGBCC_UNDEFINED
#define BGBCC_UNDEFINED ((void *)(-1))
#endif

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

typedef struct _dyt_s *dyt;
typedef struct { u64 v; } dytf;

#ifndef UNDEFINED
#define UNDEFINED		((void *)(-1))
#endif

#define BGBDY_NULL		((void *)0)
#define BGBDY_TRUE		((void *)2)
#define BGBDY_FALSE		((void *)3)
#define BGBDY_AUXNULL	((void *)4)

#define DYTF_UNDEFINED	(dytfPtrF(UNDEFINED))
#define DYTF_NULL		(dytfPtrF((void *)0))
#define DYTF_TRUE		(dytfPtrF((void *)2))
#define DYTF_FALSE		(dytfPtrF((void *)3))
#define DYTF_AUXNULL	(dytfPtrF((void *)4))

#define BGBCC_FrCC_StubError(ctx)	\
	BGBCC_FrCC_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrCC_StubWarn(ctx)	\
	BGBCC_FrCC_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrCC_StubNote(ctx)	\
	BGBCC_FrCC_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_FrBC_StubError(ctx)	\
	BGBCC_FrBC_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubWarn(ctx)	\
	BGBCC_FrBC_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubNote(ctx)	\
	BGBCC_FrBC_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_FrCC_TagError(ctx, tag)	\
	BGBCC_FrCC_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrCC_TagWarn(ctx, tag)	\
	BGBCC_FrCC_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrCC_TagNote(ctx, tag)	\
	BGBCC_FrCC_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_TagError(ctx, tag)	\
	BGBCC_FrBC_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagWarn(ctx, tag)	\
	BGBCC_FrBC_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagNote(ctx, tag)	\
	BGBCC_FrBC_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, rr)	\
	BGBCC_FrBC_RegisterAllocTemporaryLLn(ctx, bty, rr, __FILE__, __LINE__)

#ifdef __cplusplus
extern "C" {
#endif

#define BGBCC_TYFL_PUBLIC			0x0001
#define BGBCC_TYFL_PRIVATE			0x0002
#define BGBCC_TYFL_PROTECTED		0x0004
#define BGBCC_TYFL_STATIC			0x0008
#define BGBCC_TYFL_FINAL			0x0010
#define BGBCC_TYFL_SUPER			0x0020
#define BGBCC_TYFL_SYNCHRONIZED		BGBCC_TYFL_SUPER
#define BGBCC_TYFL_VOLATILE			0x0040
#define BGBCC_TYFL_TRANSIENT		0x0080
#define BGBCC_TYFL_NATIVE			0x0100
#define BGBCC_TYFL_INTERFACE		0x0200
#define BGBCC_TYFL_ABSTRACT			0x0400
#define BGBCC_TYFL_STRICT			0x0800

#define BGBCC_TYFL_BYREF			0x1000	//BS2: pass-by-reference
#define BGBCC_TYFL_DELEGATE			0x2000	//BS2: pass-by-reference

#define BGBCC_TYFL_GETTER			0x4000
#define BGBCC_TYFL_SETTER			0x8000

#define BGBCC_TYFL_SIGNED			0x00010000
#define BGBCC_TYFL_UNSIGNED			0x00020000
#define BGBCC_TYFL_EXTERN			0x00040000
#define BGBCC_TYFL_CONST			0x00080000
#define BGBCC_TYFL_INLINE			0x00100000

#define BGBCC_TYFL_EVENT			0x00200000

#define BGBCC_TYFL_WIDE				0x00400000
// #define BGBCC_TYFL_FUNCTION			0x00800000	//is a function
// #define BGBCC_TYFL_CLASS			0x01000000	//is a class
#define BGBCC_TYFL_THREAD			0x00800000
#define BGBCC_TYFL_XCALL			0x01000000

#define BGBCC_TYFL_STDCALL			0x02000000	//'__stdcall'
#define BGBCC_TYFL_CDECL			0x04000000	//'__cdecl'
#define BGBCC_TYFL_PROXY			0x08000000	//'__proxy'

#define BGBCC_TYFL_LTLENDIAN		0x10000000	//'__littleendian'
#define BGBCC_TYFL_BIGENDIAN		0x20000000	//'__bigendian'

#define BGBCC_TYFL_EXPNAME			0x40000000

// #define BGBCC_TYFL_XCALL			0x10000000

// #define BGBCC_TYFL_WIN64			0x10000000	//'__win64'
// #define BGBCC_TYFL_AMD64			0x20000000	//'__amd64'
// #define BGBCC_TYFL_XCALL			0x40000000	//'__xcall'

// #define BGBCC_TYFL_PERSISTENT	BGBCC_TYFL_WIDE
// #define BGBCC_TYFL_CLASS			BGBCC_TYFL_XCALL

#define BGBCC_TYFL_WIN64			BGBCC_TYFL_STDCALL
#define BGBCC_TYFL_AMD64			BGBCC_TYFL_CDECL

#define BGBCC_TYFL_SEALED			BGBCC_TYFL_FINAL
#define BGBCC_TYFL_READONLY			BGBCC_TYFL_STRICT
#define BGBCC_TYFL_INTERNAL			BGBCC_TYFL_SIGNED

#define BGBCC_TYFL_NEW				BGBCC_TYFL_CONST
#define BGBCC_TYFL_OVERRIDE			BGBCC_TYFL_STRICT

#define BGBCC_TYFL_CONST2			BGBCC_TYFL_FINAL

#define BGBCC_TYFL_PACKED			BGBCC_TYFL_SIGNED
// #define BGBCC_TYFL_GC			BGBCC_TYFL_UNSIGNED
// #define BGBCC_TYFL_NOGC			BGBCC_TYFL_INLINE

#define BGBCC_TYFL_GC				BGBCC_TYFL_XCALL
#define BGBCC_TYFL_NOGC				BGBCC_TYFL_NATIVE

// #define BGBCC_TYFL_EVENT			BGBCC_TYFL_TRANSIENT
#define BGBCC_TYFL_UNSAFE			BGBCC_TYFL_NATIVE

#define BGBCC_TYFL_VIRTUAL			BGBCC_TYFL_VOLATILE
#define BGBCC_TYFL_DYNAMIC			BGBCC_TYFL_XCALL

//#define BGBCC_TYFL_GETSET			(BGBCC_TYFL_EVENT|BGBCC_TYFL_BYREF)
//#define BGBCC_TYFL_GETTER			(BGBCC_TYFL_GETSET|BGBCC_TYFL_STRICT)
//#define BGBCC_TYFL_SETTER			BGBCC_TYFL_GETSET

#define BGBCC_TYFL_DLLEXPORT		(1LL<<32)
#define BGBCC_TYFL_DLLIMPORT		(1LL<<33)

#define BGBCC_CTXFL_SAVENAMES		0x0001
#define BGBCC_CTXFL_ENDOFTRACE		0x0002
#define BGBCC_CTXFL_LASTOPRET		0x0004

#define BGBCC_REGFL_ACCESSED		0x0001		//global: was used by code
#define BGBCC_REGFL_CULL			0x0002		//global: omit from image
#define BGBCC_REGFL_INITIALIZED		0x0004		//was initialized
	//field: structs were initialized with this field present
	//struct: struct was initialized with data in-code
	//register: was initialized with an object (destroy on release)

#ifndef BTK_NAME
#define BTK_NAME	1
#define BTK_NUMBER	2
#define BTK_STRING	3
#define BTK_CHARSTRING	4
#define BTK_SEPERATOR	5
#define BTK_OPERATOR	6
#define BTK_BRACE	7
#define BTK_MISC	8
#define BTK_NULL	9
#define BTK_RESV_	10	//special, don't use ('\n')
#define BTK_BREAK	11	//linebreak, ...
#define BTK_COMMENT	12	//comment, ...

#define BTK_INT			13
#define BTK_FLOAT		14
#define BTK_STRING_OVF	15	//overlong string

#endif

#define BGBCC_LANG_C		1	//C
#define BGBCC_LANG_CPP		2	//C++
#define BGBCC_LANG_CS		3	//C#
#define BGBCC_LANG_JAVA		4	//Java
#define BGBCC_LANG_BS		5	//BGBScript / ECMAScript
#define BGBCC_LANG_CX		6	//C + extensions
#define BGBCC_LANG_MINIC	7	//Mini C
#define BGBCC_LANG_BS2		8	//BGBScript2

#define BGBCC_ARCH_DEFAULT	0	//any arch
#define BGBCC_ARCH_X86		1	//32-bit x86
#define BGBCC_ARCH_X64		2	//64-bit x86 / x86-64
#define BGBCC_ARCH_ARM		3	//ARM
#define BGBCC_ARCH_PPC		4	//PPC
#define BGBCC_ARCH_PPC64	5	//PPC64

#include <bgbcc_xml.h>
// #include <bgbcc_dytf.h>

#ifdef BGBCC_BGBMETA
#include <bgbcc_meta.h>
#include <bgbcc_exwad.h>
#include <bgbcc_link.h>
#endif

// #include <bgbcc_frbc.h>
// #include <bgbcc_frimg.h>
// #include <bgbcc_frjit.h>

#include <bgbcc_tokord.h>

typedef struct BGBCP_ParseItem_s BGBCP_ParseItem;
typedef struct BGBCP_ParseState_s BGBCP_ParseState;
typedef struct BGBPP_Def_s BGBPP_Def;

typedef struct BGBCP_ParseState_s BGBJP_ParseState;

typedef struct BGBCC_FrCC_State_s BGBCC_FrCC_State;
typedef struct BGBCC_FrCC_RegisterInfo_s BGBCC_FrCC_RegisterInfo;
typedef struct BGBCC_FrCC_LiteralInfo_s BGBCC_FrCC_LiteralInfo;
typedef struct BGBCC_FrCC_TypeOverflow_s BGBCC_FrCC_TypeOverflow;

//#include <bgbcc_svm.h>

struct BGBCP_ParseItem_s {
BGBCP_ParseItem *next;
char *name;

BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s);
};

struct BGBCP_ParseState_s {
int flags, lang, arch;
BCCX_Node *structs;
BCCX_Node *types;

BCCX_Node *e_structs;
BCCX_Node *e_types;

BGBPP_Def *ppdef[256];

BCCX_Node *struct_hash[256];
BCCX_Node *type_hash[1024];

BCCX_Node *reduce_tmp;

char *cur_ns;
char **cur_nsi;
char *cur_class;
};

typedef struct {
//elem lenv, llenv;	//lex env names
//elem tenv, ltenv;	//lex env types
//elem venv, lvenv;	//lex env values

//elem msstruct;		//module serialized structs

int lvl;	//block level, 0=toplevel, 1..=in function
int olvl;	//object level, 0=module, 1..=in object
int cap;	//set to indicate var capture
int tail;	//this expression is in tail position
int i_cx_p;	//infer: expression is clean (cleared to indicate dirty)
int i_cap;	//infer: guess as to whether or not there is var capture

char *cf_n;
BCCX_Node *cf_ty;

char **goto_name;
byte **goto_dest;
char **lbl_name;
byte **lbl_dest;
int n_goto, n_lbl;

char **contstack;
char **breakstack;
int contstackpos;
int breakstackpos;

char *ip;
char *ips;
char *ipe;

BCCX_Node *types;
BCCX_Node *structs;

char *cur_ns;
char *cur_struct;
int cur_idx;	//field
int cur_idx2;	//method
int cur_idx3;	//property

int lang, arch;

}BGBCC_State;

struct BGBPP_Def_s {
BGBPP_Def *next;
char *name;
char *value;
char **args;
char *t_args[8];
};

#if 0
struct BGBCC_FrCC_RegisterInfo_s {
BGBCC_FrCC_RegisterInfo *next;
BGBCC_FrCC_RegisterInfo *hashnext;		//next in lookup hash
char *pbname;			//scope path base name
char *name;				//assigned variable name
char *qname;			//assigned qualified name
char *sig;				//type signature
char *flagstr;			//flag string
frbc_type type;			//assigned type
int ucnt;				//use count
int regtype;			//register type
int regid;				//register ID
int validx;				//value index
s64 flagsint;			//flags (integer)
frbc_register value;	//literal value

byte *text;				//bytecode
int sz_text;			//sizeof bytecode

char *alc_fn;
int alc_ln;

BGBCC_FrCC_RegisterInfo *defv;		//define var
BGBCC_FrCC_RegisterInfo *defp;		//define parent

BGBCC_FrCC_RegisterInfo **fields;
BGBCC_FrCC_RegisterInfo **args;
BGBCC_FrCC_RegisterInfo **locals;
BGBCC_FrCC_RegisterInfo **regs;
int n_fields, m_fields;
int n_args, m_args;
int n_locals, m_locals;
int n_regs, m_regs;
int n_cargs;
int n_eargs;		//end args
int n_vargs;		//virtual args

frbc_register *listdata;
int n_listdata, m_listdata;
int gblid, regflags;
int fxsize, fxoffs;		//fixed-format size/offset
int fxmsize, fxnsize;	//fixed min/max size
int fxmalgn, fxnalgn;	//fixed min/max size
};

struct BGBCC_FrCC_LiteralInfo_s {
char *name;
char *sig;
int littype;	//literal type
int litid;		//literal ID

BGBCC_FrCC_LiteralInfo *parent;
BGBCC_FrCC_RegisterInfo *decl;
};

struct BGBCC_FrCC_TypeOverflow_s {
int base;
int asz[16];
byte pn, an;
};

struct BGBCC_FrCC_State_s {
int lvl;		//block level, 0=toplevel, 1..=in function
int olvl;		//object level, 0=module, 1..=in object
int cap;		//set to indicate var capture
int tail;		//this expression is in tail position
int i_cx_p;		//infer: expression is clean (cleared to indicate dirty)
int i_cap;		//infer: guess as to whether or not there is var capture

char *cf_n;
BCCX_Node *cf_ty;

char **goto_name;
int *goto_dest;
int *goto_type;
char **lbl_name;
int *lbl_dest;
int n_goto, n_lbl;
int m_goto, m_lbl;

char **contstack;
char **breakstack;
int contstackpos;
int breakstackpos;

char *ip;
char *ips;
char *ipe;

int *markstack;
frbc_register *regstack;
frbc_register *uregstack;
int markstackpos;
int regstackpos;
int uregstackpos;

BCCX_Node *types;
BCCX_Node *structs;
BCCX_Node *reduce_tmp;

BCCX_Node *cur_structdef;
char *cur_ns;
char *cur_struct;
int cur_idx;		//field
int cur_idx2;		//method
int cur_idx3;		//property
int cur_idx4;		//enum value

int lang, arch;
int ctxflags;

int n_error, n_warn, n_note;
char *lfn;	//Last File Name
int lln;	//Last Line Number
int reqlfn, gs_seq;

// BGBCC_FrCC_RegisterInfo *reg_args[256];
// BGBCC_FrCC_RegisterInfo *reg_locals[256];
// BGBCC_FrCC_RegisterInfo *reg_regs[256];
// BGBCC_FrCC_RegisterInfo **reg_args;
// BGBCC_FrCC_RegisterInfo **reg_locals;
// BGBCC_FrCC_RegisterInfo **reg_regs;
// int n_reg_args;
// int n_reg_locals;
// int n_reg_regs;

BGBCC_FrCC_RegisterInfo *cur_func;

// BGBCC_FrCC_RegisterInfo *reg_globals[4096];
// int idx_globals[4096];
BGBCC_FrCC_RegisterInfo *reg_globals[262144];
int idx_globals[262144];
int n_reg_globals;
int m_reg_globals;

BGBCC_FrCC_RegisterInfo *hash_globals[4096];	//global lookup hash
BGBCC_FrCC_RegisterInfo *usort_globals;			//unsorted globals

// BGBCC_FrCC_LiteralInfo *literals[4096];
// int idx_literals[4096];
BGBCC_FrCC_LiteralInfo *literals[65536];
int idx_literals[65536];
int n_literals, m_literals;

// BGBCC_FrCC_TypeOverflow *tyovf[1024];
BGBCC_FrCC_TypeOverflow *tyovf[16384];
int n_tyovf, m_tyovf;

u32 *ctab_lvt4;
u64 *ctab_lvt8;
int n_ctab_lvt4, m_ctab_lvt4;
int n_ctab_lvt8, m_ctab_lvt8;

int *cntstrn;
char **cntstrs;
int n_cntstrs, m_cntstrs;

char *strtab;
int sz_strtab, msz_strtab;

char *wstrtab;
int sz_wstrtab, msz_wstrtab;

BGBCC_FrCC_RegisterInfo *reginfo_free;

BGBCC_FrCC_LiteralInfo *cur_objstack[64];
BGBCC_FrCC_LiteralInfo *cur_obj;
int cur_objstackpos;
};
#endif


#ifdef BGBCC_BGBMETA
#include <bgbcc_autom.h>
#else
#include <bgbcc_auto.h>
#endif

#ifdef __cplusplus
}
#endif

#endif


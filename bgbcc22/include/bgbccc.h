#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

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

typedef u32 fourcc;

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

#ifdef _MSC_VER
#define BGBCC_DBGBREAK	__debugbreak();
#else
#ifdef __GNUC__
#define BGBCC_DBGBREAK	__builtin_trap();
#else
// #define BGBCC_DBGBREAK
#endif
#endif

#define BGBCC_CCXL_StubError(ctx)	\
	BGBCC_CCXL_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_CCXL_StubWarn(ctx)	\
	BGBCC_CCXL_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_CCXL_StubNote(ctx)	\
	BGBCC_CCXL_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_CCXL_SanityError(ctx)	\
	BGBCC_CCXL_SanityErrorLLn(ctx, __FILE__, __LINE__)

#define BGBCC_FrBC_StubError(ctx)	\
	BGBCC_FrBC_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubWarn(ctx)	\
	BGBCC_FrBC_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubNote(ctx)	\
	BGBCC_FrBC_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_CCXL_TagError(ctx, tag)	\
	BGBCC_CCXL_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_CCXL_TagWarn(ctx, tag)	\
	BGBCC_CCXL_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_CCXL_TagNote(ctx, tag)	\
	BGBCC_CCXL_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_TagError(ctx, tag)	\
	BGBCC_FrBC_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagWarn(ctx, tag)	\
	BGBCC_FrBC_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagNote(ctx, tag)	\
	BGBCC_FrBC_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, rr)	\
	BGBCC_FrBC_RegisterAllocTemporaryLLn(ctx, bty, rr, __FILE__, __LINE__)

#ifdef _MSC_VER
#ifndef BGBCC_GET_BITS
#define BGBCC_GET_BITS

#define BGBCC_GET_U16LE(p)	(*(u16 *)(p))
#define BGBCC_GET_U32LE(p)	(*(u32 *)(p))
#define BGBCC_GET_U64LE(p)	(*(u64 *)(p))

#define BGBCC_SET_U16LE(p, v)	((*(u16 *)(p))=(v))
#define BGBCC_SET_U32LE(p, v)	((*(u32 *)(p))=(v))
#define BGBCC_SET_U64LE(p, v)	((*(u64 *)(p))=(v))

#endif
#endif

#ifndef BGBCC_GET_BITS
#define BGBCC_GET_BITS
static u16 BGBCC_GET_U16LE(void *ptr)
{
	byte *p;
	p=(byte *)ptr;
	return(p[0]|(p[1]<<8));
}

static u32 BGBCC_GET_U32LE(void *ptr)
{
	byte *p;
	p=(byte *)ptr;
	return(p[0]|(p[1]<<8)|(p[2]<<16)|(p[3]<<24));
}

static u64 BGBCC_GET_U64LE(void *ptr)
{
	byte *p;
	u32 t0, t1;
	p=(byte *)ptr;
	t0=(p[0]|(p[1]<<8)|(p[2]<<16)|(p[3]<<24));
	t1=(p[4]|(p[5]<<8)|(p[6]<<16)|(p[7]<<24));
	return(t0|(((u64)t1)<<32));
}

static void BGBCC_SET_U16LE(void *ptr, u16 val)
{
	byte *p;
	p=(byte *)ptr;
	p[0]=val;
	p[1]=val>>8;
}

static void BGBCC_SET_U32LE(void *ptr, u32 val)
{
	byte *p;
	p=(byte *)ptr;
	p[0]=val;
	p[1]=val>>8;
	p[2]=val>>16;
	p[3]=val>>24;
}
#endif

static u16 BGBCC_GET_U16EN(void *ptr, int en)
{
	byte *p;
	p=(byte *)ptr;
	if(en)
		return(p[1]|(p[0]<<8));
	return(p[0]|(p[1]<<8));
}


#define BGBCC_ASC_MASK32	0x80808080UL
#define BGBCC_ASC_MASK64	0x8080808080808080ULL

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

#define BGBCC_TYFL_VIRTUAL			0x4000
#define BGBCC_TYFL_TYPEDEF			0x8000

// #define BGBCC_TYFL_GETTER			0x4000
// #define BGBCC_TYFL_SETTER			0x8000

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

// #define BGBCC_TYFL_SEALED			BGBCC_TYFL_FINAL
// #define BGBCC_TYFL_READONLY			BGBCC_TYFL_STRICT
// #define BGBCC_TYFL_INTERNAL			BGBCC_TYFL_SIGNED

// #define BGBCC_TYFL_NEW				BGBCC_TYFL_CONST
#define BGBCC_TYFL_OVERRIDE			BGBCC_TYFL_STRICT
// #define BGBCC_TYFL_ALIASPTR			BGBCC_TYFL_STRICT
// #define BGBCC_TYFL_RESTRICT			BGBCC_TYFL_STRICT

#define BGBCC_TYFL_CONST2			BGBCC_TYFL_FINAL

// #define BGBCC_TYFL_PACKED		BGBCC_TYFL_SIGNED
// #define BGBCC_TYFL_GC			BGBCC_TYFL_UNSIGNED
// #define BGBCC_TYFL_NOGC			BGBCC_TYFL_INLINE

// #define BGBCC_TYFL_GC				BGBCC_TYFL_XCALL
// #define BGBCC_TYFL_NOGC				BGBCC_TYFL_NATIVE

// #define BGBCC_TYFL_EVENT			BGBCC_TYFL_TRANSIENT
#define BGBCC_TYFL_UNSAFE			BGBCC_TYFL_NATIVE

// #define BGBCC_TYFL_VIRTUAL			BGBCC_TYFL_VOLATILE
#define BGBCC_TYFL_DYNAMIC			BGBCC_TYFL_XCALL

#define BGBCC_TYFL_REGISTER			BGBCC_TYFL_INLINE

//#define BGBCC_TYFL_GETSET			(BGBCC_TYFL_EVENT|BGBCC_TYFL_BYREF)
//#define BGBCC_TYFL_GETTER			(BGBCC_TYFL_GETSET|BGBCC_TYFL_STRICT)
//#define BGBCC_TYFL_SETTER			BGBCC_TYFL_GETSET

#define BGBCC_TYFL_PACKED			(1LL<<31)

#define BGBCC_TYFL_DLLEXPORT		(1LL<<32)
#define BGBCC_TYFL_DLLIMPORT		(1LL<<33)

#define BGBCC_TYFL_NONPOD			(1LL<<34)
#define BGBCC_TYFL_HASDATA			(1LL<<35)

#define BGBCC_TYFL_INTERRUPT		(1LL<<36)

#define BGBCC_TYFL_NEAR				(1LL<<37)
#define BGBCC_TYFL_FAR				(1LL<<38)

#define BGBCC_TYFL_SYSCALL			(1LL<<39)
#define BGBCC_TYFL_AUTO				(1LL<<40)

#define BGBCC_TYFL_HUGE				(1LL<<41)

#define BGBCC_TYFL_IFARCH			(1LL<<42)
#define BGBCC_TYFL_IFNARCH			(1LL<<43)

#define BGBCC_TYFL_NOCULL			(1LL<<44)
#define BGBCC_TYFL_FENVACC			(1LL<<45)

#define BGBCC_TYFL_RESTRICT			(1LL<<46)
#define BGBCC_TYFL_MAYALIAS			(1LL<<47)

#define BGBCC_TYFL_LOCAL			(1LL<<48)
#define BGBCC_TYFL_GLOBAL			(1LL<<49)

#define BGBCC_TYFL_TBRSAVE			(1LL<<50)


#define BGBCC_TYFL_DFL_MASK			\
	(BGBCC_TYFL_PUBLIC|BGBCC_TYFL_PRIVATE|BGBCC_TYFL_PROTECTED)

#define BGBCC_TYFL_DFL_COPY_MASK	\
	(BGBCC_TYFL_STATIC|BGBCC_TYFL_NATIVE|BGBCC_TYFL_VIRTUAL)



#define BGBCC_CTXFL_SAVENAMES		0x0001
#define BGBCC_CTXFL_ENDOFTRACE		0x0002
#define BGBCC_CTXFL_LASTOPRET		0x0004

#define BGBCC_REGFL_ACCESSED		0x00000001		//global: was used by code
#define BGBCC_REGFL_CULL			0x00000002		//global: omit from image
#define BGBCC_REGFL_INITIALIZED		0x00000004		//was initialized
	//field: structs were initialized with this field present
	//struct: struct was initialized with data in-code
	//register: was initialized with an object (destroy on release)

#define BGBCC_REGFL_RECTRACE		0x00000008		//hit by reachability trace

#define BGBCC_REGFL_DEMANDLOAD		0x00000010		//demand-load requested
#define BGBCC_REGFL_LOADED			0x00000020		//already loaded
#define BGBCC_REGFL_ALIASPTR		0x00000040		//may be aliased by a pointer

#define BGBCC_REGFL_VARCONV			0x00000080		//converted to/from variant

#define BGBCC_REGFL_TRYLEAF			0x00000100		//Maybe Leaf?
#define BGBCC_REGFL_NOTLEAF			0x00000200		//Not Leaf
#define BGBCC_REGFL_HASARRAY		0x00000400		//May Contain Arrays
#define BGBCC_REGFL_GOFAST			0x00000800		//Speed hint seen.
#define BGBCC_REGFL_ALLOCA			0x00001000		//Function uses alloca.

#define BGBCC_REGFL_LEAFTINY		0x00002000		//Maybe Tiny Leaf? (No Frame)
#define BGBCC_REGFL_NOTLEAFTINY		0x00004000		//Not Tiny Leaf
#define BGBCC_REGFL_HYBLEAFTINY		0x00008000		//Hybrid Tiny Leaf
#define BGBCC_REGFL_NOSCRATCHDYN	0x00010000		//Can't dynamically alloc
#define BGBCC_REGFL_IFACULL			0x00020000		//Culled via IfArch
#define BGBCC_REGFL_TEMPLOAD		0x00040000		//Temp Loaded from Stack
#define BGBCC_REGFL_HAS128			0x00080000		//Has 128-bit Types
#define BGBCC_REGFL_IMMLOAD			0x00100000		//Immediate value Loaded
#define BGBCC_REGFL_GBLSTORE		0x00200000		//Stores into Global

#define BGBCC_REGFL_HASLCLALIAS		0x00400000		//Has aliased locals.
#define BGBCC_REGFL_HASGBLALIAS		0x00800000		//Has aliased globals.

#define BGBCC_REGFL_CALLVARARG		0x01000000		//Calls a vararg func
#define BGBCC_REGFL_ISVARARG		0x02000000		//Is varargs

#define BGBCC_REGFL_TYPEACCESSED	0x04000000		//type was accessed


#define	BGBCC_MAXSTACKOBJ			0x080000	//maximum allowed stack frame
#define	BGBCC_MAXSTACKFRAME			0x080000	//maximum allowed stack frame

#define BGBCC_LOOPFL_CONTINUE		0x0001
#define BGBCC_LOOPFL_BREAK			0x0002
#define BGBCC_LOOPFL_MASK			0x0003

#define BGBCC_RSPFL_NONLOCAL		0x0001		//Not Local Variable
#define BGBCC_RSPFL_NONBASIC		0x0002		//Non-basic type
#define BGBCC_RSPFL_SINGLETRACE		0x0004		//Only spans a single trace
#define BGBCC_RSPFL_NONOVTRACE		0x0008		//No overlap within trace.
#define BGBCC_RSPFL_CROSSTRACE		0x0010		//Cross-trace spans seen.
#define BGBCC_RSPFL_GBLSTORE		0x0020		//Modified Global
#define BGBCC_RSPFL_ALIASPTR		0x0040		//Aliased by pointer

#define BGBCC_RSPFL_ISSOURCE		0x0100		//Is used as an input
#define BGBCC_RSPFL_ISCALLARG		0x0200		//Is a call argument (exclusive)

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
#define BTK_STRING_QQQ	16	//triple-quote

#endif

#define BGBCC_OPT_DEFAULT	0	//balance
#define BGBCC_OPT_SIZE		1	//smallest binary
#define BGBCC_OPT_SPEED		2	//fast binary, balance
#define BGBCC_OPT_SPEED2	3	//maximize speed
#define BGBCC_OPT_DEBUG		4

#if 0
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

#define BGBCC_ARCH_SH		6	//SH variants
#define BGBCC_ARCH_SH64		7	//64-bit SH variants

#define BGBCC_ARCH_SH_SH2	2	//SH2
#define BGBCC_ARCH_SH_SH4	4	//SH4
#endif

#if 1
#define BGBCC_TWOCC(a, b)			((a)|((b)<<8))
#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BGBCC_LANG_C		BGBCC_FOURCC('C', ' ', ' ', ' ')
#define BGBCC_LANG_CPP		BGBCC_FOURCC('C', 'P', 'P', ' ')
#define BGBCC_LANG_CS		BGBCC_FOURCC('C', 'S', 'R', 'P')
#define BGBCC_LANG_JAVA		BGBCC_FOURCC('J', 'A', 'V', 'A')
#define BGBCC_LANG_BS		BGBCC_FOURCC('B', 'S', ' ', ' ')
#define BGBCC_LANG_CX		BGBCC_FOURCC('C', 'A', 'U', 'X')
#define BGBCC_LANG_MINIC	BGBCC_FOURCC('M', 'I', 'N', 'C')
#define BGBCC_LANG_BS2		BGBCC_FOURCC('B', 'S', '2', ' ')

#define BGBCC_LANG_ASM		BGBCC_FOURCC('A', 'S', 'M', ' ')

#define BGBCC_ARCH_DEFAULT	0	//any arch
#define BGBCC_ARCH_X86		BGBCC_FOURCC('X', '8', '6', ' ')
#define BGBCC_ARCH_X64		BGBCC_FOURCC('X', '6', '4', ' ')
#define BGBCC_ARCH_ARM		BGBCC_FOURCC('A', 'R', 'M', ' ')
#define BGBCC_ARCH_PPC		BGBCC_FOURCC('P', 'P', 'C', ' ')
#define BGBCC_ARCH_PPC64	BGBCC_FOURCC('P', 'P', '6', '4')

#define BGBCC_ARCH_SH		BGBCC_FOURCC('S', 'H', ' ', ' ')
#define BGBCC_ARCH_SH64		BGBCC_FOURCC('S', 'H', '6', '4')

#define BGBCC_ARCH_SH_SH2	BGBCC_FOURCC('S', 'H', '2', ' ')
#define BGBCC_ARCH_SH_SH4	BGBCC_FOURCC('S', 'H', '4', ' ')
#define BGBCC_ARCH_SH_SH2L	BGBCC_FOURCC('S', 'H', '2', 'L')
#define BGBCC_ARCH_SH_SH4L	BGBCC_FOURCC('S', 'H', '4', 'L')
#define BGBCC_ARCH_SH_SH2B	BGBCC_FOURCC('S', 'H', '2', 'B')
#define BGBCC_ARCH_SH_SH4B	BGBCC_FOURCC('S', 'H', '4', 'B')

#define BGBCC_ARCH_SH_BX1L	BGBCC_FOURCC('B', 'X', '1', 'L')
#define BGBCC_ARCH_SH_BX1B	BGBCC_FOURCC('B', 'X', '1', 'B')
#define BGBCC_ARCH_SH_BX6L	BGBCC_FOURCC('B', 'X', '6', 'L')
#define BGBCC_ARCH_SH_BX6B	BGBCC_FOURCC('B', 'X', '6', 'B')

#define BGBCC_ARCH_SH_BX6M	BGBCC_FOURCC('B', 'X', '6', 'M')

#define BGBCC_ARCH_SH_BX6U	BGBCC_FOURCC('B', 'X', '6', 'U')
#define BGBCC_ARCH_SH_BX6V	BGBCC_FOURCC('B', 'X', '6', 'V')

#define BGBCC_ARCH_SH_BX6C	BGBCC_FOURCC('B', 'X', '6', 'C')


#define BGBCC_ARCH_BSR		BGBCC_FOURCC('B', 'S', 'R', ' ')
#define BGBCC_ARCH_BSR_BS1A	BGBCC_FOURCC('B', 'S', '1', 'A')

#define BGBCC_ARCH_BJX2			BGBCC_FOURCC('B', 'J', 'X', '2')
#define BGBCC_ARCH_BJX2_JX2A	BGBCC_FOURCC('J', 'X', '2', 'A')	//VL 64b
#define BGBCC_ARCH_BJX2_JX2B	BGBCC_FOURCC('J', 'X', '2', 'B')	//Fix32, 64b
#define BGBCC_ARCH_BJX2_JX2C	BGBCC_FOURCC('J', 'X', '2', 'C')	//VL 32b
#define BGBCC_ARCH_BJX2_JX2D	BGBCC_FOURCC('J', 'X', '2', 'D')	//Fix32, 32b
#define BGBCC_ARCH_BJX2_JX2E	BGBCC_FOURCC('J', 'X', '2', 'E')	//VL, Lite
#define BGBCC_ARCH_BJX2_JX2F	BGBCC_FOURCC('J', 'X', '2', 'F')	//VL, Lite

#define BGBCC_ARCH_BJX2_JX2G	BGBCC_FOURCC('J', 'X', '2', 'G')	//96b
#define BGBCC_ARCH_BJX2_JX2H	BGBCC_FOURCC('J', 'X', '2', 'H')	//
#define BGBCC_ARCH_BJX2_JX2I	BGBCC_FOURCC('J', 'X', '2', 'I')	//

#define BGBCC_ARCH_BJX2_X2RV	BGBCC_FOURCC('X', '2', 'R', 'V')	//XG2RV

#define BGBCC_ARCH_BJX2_XG2A	BGBCC_FOURCC('X', 'G', '2', 'A')

#define BGBCC_ARCH_XG2A		BGBCC_FOURCC('X', 'G', '2', 'A')

#define BGBCC_ARCH_RV32		BGBCC_FOURCC('R', 'V', '3', '2')
#define BGBCC_ARCH_RV64		BGBCC_FOURCC('R', 'V', '6', '4')


//output formats.
//format specifics depend on target.
#define BGBCC_IMGFMT_OBJ	BGBCC_FOURCC('O', 'B', 'J', ' ')
#define BGBCC_IMGFMT_EXE	BGBCC_FOURCC('E', 'X', 'E', ' ')
#define BGBCC_IMGFMT_DLL	BGBCC_FOURCC('D', 'L', 'L', ' ')
#define BGBCC_IMGFMT_ELXE	BGBCC_FOURCC('E', 'L', 'X', 'E')
#define BGBCC_IMGFMT_ELSO	BGBCC_FOURCC('E', 'L', 'S', 'O')
#define BGBCC_IMGFMT_ROM	BGBCC_FOURCC('R', 'O', 'M', ' ')
#define BGBCC_IMGFMT_SYS	BGBCC_FOURCC('S', 'Y', 'S', ' ')

#define BGBCC_IMGFMT_ASM	BGBCC_FOURCC('A', 'S', 'M', ' ')
#define BGBCC_IMGFMT_PP		BGBCC_FOURCC('P', 'P', ' ', ' ')

#define BGBCC_IMGFMT_RIL3	BGBCC_FOURCC('R', 'I', 'L', '3')
#define BGBCC_IMGFMT_FR2E	BGBCC_FOURCC('F', 'R', '2', 'E')

#define BGBCC_IMGFMT_WAD	BGBCC_FOURCC('W', 'A', 'D', ' ')
#define BGBCC_IMGFMT_WAD2	BGBCC_FOURCC('W', 'A', 'D', '2')
#define BGBCC_IMGFMT_RWAD	BGBCC_FOURCC('R', 'W', 'A', 'D')
#define BGBCC_IMGFMT_LUMP	BGBCC_FOURCC('L', 'U', 'M', 'P')

#define BGBCC_FMT_WAD		BGBCC_FOURCC('W', 'A', 'D', ' ')
#define BGBCC_FMT_WAD2		BGBCC_FOURCC('W', 'A', 'D', '2')
#define BGBCC_FMT_RWAD		BGBCC_FOURCC('R', 'W', 'A', 'D')
#define BGBCC_FMT_LUMP		BGBCC_FOURCC('L', 'U', 'M', 'P')

#define BGBCC_FMT_WAV		BGBCC_FOURCC('W', 'A', 'V', ' ')
#define BGBCC_FMT_BMP		BGBCC_FOURCC('B', 'M', 'P', ' ')
#define BGBCC_FMT_AVI		BGBCC_FOURCC('A', 'V', 'I', ' ')

#define BGBCC_FMT_DEF		BGBCC_FOURCC('D', 'E', 'F', ' ')
#define BGBCC_FMT_WDEF		BGBCC_FOURCC('W', 'D', 'E', 'F')

#define BGBCC_FMT_QOIF		BGBCC_FOURCC('Q', 'O', 'I', 'F')
#define BGBCC_FMT_LCIF		BGBCC_FOURCC('L', 'C', 'I', 'F')
#define BGBCC_FMT_UPIC		BGBCC_FOURCC('U', 'P', 'I', 'C')

#define BGBCC_FMT_CRAM		BGBCC_FOURCC('C', 'R', 'A', 'M')

#define BGBCC_FMT_RGL3		BGBCC_FOURCC('R', 'G', 'L', '3')
#define BGBCC_FMT_RGL4		BGBCC_FOURCC('R', 'G', 'L', '4')

#endif

/* flags for features the frontend has seen used. */
#define BGBCC_TYCSEEN_VARIANT	0x0001	//saw variant types
#define BGBCC_TYCSEEN_INT128	0x0002	//saw int128
#define BGBCC_TYCSEEN_FLOAT128	0x0004	//saw float128
#define BGBCC_TYCSEEN_FLOAT_FPU	0x0008	//saw FPU float/double types.
#define BGBCC_TYCSEEN_IDIV_VAR	0x0010	//saw integer division by variable.
#define BGBCC_TYCSEEN_IMOD_VAR	0x0020	//saw integer modulo by variable.
#define BGBCC_TYCSEEN_ISHL_VAR	0x0040	//saw int shift left, variable
#define BGBCC_TYCSEEN_ISHR_VAR	0x0080	//saw int shift right (log), variable
#define BGBCC_TYCSEEN_ISAR_VAR	0x0100	//saw int shift right (arith), variable
#define BGBCC_TYCSEEN_BITINT	0x0200	//saw _BitInt
#define BGBCC_TYCSEEN_BCD		0x0400	//saw BCD types

typedef union {
	struct {
		u32 la;
		u32 lb;
		u32 lc;
		u32 ld;
	};
	struct {
		u64 lo;
		u64 hi;
	};
}bgbcc_vint128;

typedef union {
	struct {
		u32 la;
		u32 lb;
		u32 lc;
		u32 ld;
	};
	struct {
		u64 lo;
		u64 hi;
	};
}bgbcc_vfloat128;

#define BGBCC_U64_LO32MASK	0x00000000FFFFFFFFULL
#define BGBCC_U64_LO32(a)	((a)&BGBCC_U64_LO32MASK)
#define BGBCC_U64_HI32(a)	((((u64)(a))>>32)&BGBCC_U64_LO32MASK)

#define BGBCC_BCCX2

#include <bgbcc_endian.h>
#include <bgbcc_thread.h>

#ifdef BGBCC_BCCX2
#include <bgbcc_ast2.h>
#else
#include <bgbcc_xml.h>
#endif

#ifdef BGBCC_BGBMETA
#include <bgbcc_meta.h>
#include <bgbcc_exwad.h>
#include <bgbcc_link.h>
#include <bgbcc_objelf.h>
#endif

#include <bgbcc_tokord.h>

#include <bgbcc_ccxl.h>
#include <bgbcc_ril3.h>
#include <bgbcc_fr2e.h>
#include <bgbcc_comcc.h>
#include <bgbcc_shcc.h>
#include <bgbcc_bsrcc.h>

typedef struct BGBCP_ParseItem_s BGBCP_ParseItem;
typedef struct BGBCP_ParseState_s BGBCP_ParseState;
typedef struct BGBPP_Def_s BGBPP_Def;

typedef struct BGBPP_PpiDef_s BGBPP_PpiDef;
typedef struct BGBPP_PpiFrame_s BGBPP_PpiFrame;

typedef struct BGBCP_ParseState_s BGBJP_ParseState;
typedef struct BGBCC_TransState_s BGBCC_TransState;


typedef struct TKuPI_DecState_s TKuPI_DecState;
typedef struct TKuPI_EncState_s TKuPI_EncState;


struct BGBCP_ParseItem_s {
BGBCP_ParseItem *next;
char *name;

BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s);
};

// #define BGBCC_GBLHASH_SZ	16384
#define BGBCC_GBLHASH_SZ	8192
// #define BGBCC_GBLHASH_SZ	4096

//#define BGBCC_PPDEF_SIZE	256
//#define BGBCC_PPDEF_MASK	255
//#define BGBCC_PPDEF_SIZE	512
//#define BGBCC_PPDEF_MASK	511
#define BGBCC_PPDEF_SIZE	1024
#define BGBCC_PPDEF_MASK	1023
//#define BGBCC_PPDEF_SIZE	2048
//#define BGBCC_PPDEF_MASK	2047

struct BGBCP_ParseState_s {
int flags;

fourcc lang;
fourcc arch;
fourcc subarch;
int tuidx;				//translation unit index
int blkidx;				//block index

byte expect_type;		//force detecting a type
byte in_func_body;		//parsing a function body
byte in_struct_body;	//parsing a structure
short cur_codepage;
byte	fenv_access;

s64	dfl_flags;

BCCX_Node *structs;
BCCX_Node *types;
BCCX_Node *toplevel;

BCCX_Node *e_structs;
BCCX_Node *e_types;

// BCAST_Node *p3_structs;
// BCAST_Node *p3_types;

// BGBPP_Def *ppdef[256];
BGBPP_Def *ppdef[BGBCC_PPDEF_SIZE];

BGBCP_ParseState *ppi_ctx;
BGBCP_ParseState *ppi_upctx;
BGBPP_PpiDef *ppi_toplevel;
BGBPP_PpiDef *ppi_freedef;
BGBPP_PpiFrame *ppi_callframe;
BGBPP_PpiFrame *ppi_freeframe;
byte ppi_break;
BCCX_Node *ppi_retval;

#ifdef BGBCC_BCCX2
int struct_hash_ix[256];
int type_hash_ix[1024];
#else
BCCX_Node *struct_hash[256];
BCCX_Node *type_hash[1024];
#endif

int n_enum_vars;
char *enum_names[4096];
int enum_vals[4096];
int enum_chain[4096];
int enum_hash[256];

BCCX_Node *reduce_tmp;

BCCX_Node *template_stack[16];
byte template_stackpos;

char *template_inststack_var[64];
BCCX_Node *template_inststack_val[64];
byte template_inst_stackpos;

char *cur_ns;
// char **cur_nsi;
char *cur_class;

char **cur_nsi;
int n_cur_nsi;
int m_cur_nsi;

BGBCC_CCXL_BackendFuncs_vt *back_vt;
};

struct BGBPP_Def_s {
BGBPP_Def *next;
char *name;
char *value;
char **args;
char *t_args[16];
};

struct BGBPP_PpiDef_s {
BGBPP_PpiDef *next;
char *name;
//char *value;
//char **args;
//char *t_args[16];
BCCX_Node *args;
BCCX_Node *body;
BCCX_Node *value;
};

struct BGBPP_PpiFrame_s {
BGBPP_PpiFrame *next;
//BCCX_Node *value;

BGBPP_PpiDef *vars;
};

struct BGBCC_TransState_s {
byte lvl;	//block level, 0=toplevel, 1..=in function
byte olvl;	//object level, 0=module, 1..=in object
byte cap;	//set to indicate var capture
byte tail;	//this expression is in tail position
byte i_cx_p;	//infer: expression is clean (cleared to indicate dirty)
byte i_cap;	//infer: guess as to whether or not there is var capture

char *cf_n;			//current function name
BCCX_Node *cf_ty;	//current function return type

//ccxl_label *goto_name;
//byte **goto_dest;
//ccxl_label *lbl_name;
//byte **lbl_dest;
//int n_goto, n_lbl;

// char **goto_name;
// ccxl_label *goto_lbl;
// int n_goto, m_goto;

int n_warn, n_error, n_note;

int tagw_lfn[1024];
int tagw_lln[1024];
int tagw_tag[1024];
short tagw_pix[1024];
byte tagw_pcn[1024];
int tagw_n_warn;
byte tagw_skip;

int tagw_parm[1024];
int tagw_n_parm;

ccxl_label *contstack;
ccxl_label *breakstack;
int contstackpos;
int breakstackpos;

// char *ip;
// char *ips;
// char *ipe;

byte *ril_ip;
byte *ril_ips;
byte *ril_ipe;

byte *ril_txwin;
u16 *ril_txhash;
u16 ril_txrov;
u16 ril_txrh;
u16 ril_psidx[64];
byte ril_pslen[64];
byte ril_pschn[64];		//chain (per length)
byte ril_pslix[256];	//length index
byte ril_psrov;

BGBCC_CCXL_VirtOp **vop;
BGBCC_CCXL_VirtTr **vtr;
int n_vop, m_vop;
int n_vtr, m_vtr;
int s_vop;

int *markstack;
ccxl_register *regstack;
ccxl_register *uregstack;
int markstackpos;
int regstackpos;
int uregstackpos;

BCCX_Node *types;
BCCX_Node *structs;

#ifdef BGBCC_BCCX2
int struct_hash_ix[256];
#else
BCCX_Node *struct_hash[256];
#endif

// BCCX_Node *dynobj;
// BCCX_Node *dynobj_e;

BCCX_Node *dynobj_stk[256];
int dynobj_stkpos;


BCCX_Node *static_init;
int static_init_seq;

char *cur_ns;
char *cur_struct;
int cur_idx;	//field
int cur_idx2;	//method
int cur_idx3;	//property
int cur_idx4;	//property

char *imp_ns[256];
int n_imp;

char *lambda_capref[256];
BCCX_Node *lambda_captype[256];
int n_lambda_capref;
byte do_lambda_capref;

char *vlcn_names[512];
BCCX_Node *vlcn_types[512];
int vlcn_npos;

char *infer_var_loopconst;

int vlcl_seq;
int vlcl_curseq;
int vlcl_stack[256];
int vlcl_stack_npos[256];
int vlcl_stackpos;

fourcc lang;
fourcc arch;			//major architecture
fourcc sub_arch;		//subset architecture
int tuidx;				//translation unit index

byte arch_sizeof_int;	//sizeof(int), 0/2/4
byte arch_sizeof_long;
byte arch_sizeof_ptr;
byte arch_sizeof_valist;

byte arch_endian;		//0=unknown, 1=little, 2=big
byte arch_unaligned;	//0=unknown, 1=unaligned safe, 2=aligned only

int arch_sizeof_regsave;

byte arch_demote_d2f;	//for arch, demote double to float
byte arch_align_max;	//don't use alignments larger than this
byte arch_align_objmin;	//minimum alignment for value-objects
byte arch_has_predops;	//arch has predicated instructions

byte arch_has_imac;		//arch has integer multiply-accumulate
byte arch_has_fmac;		//arch has floating point multiply-accumulate

byte opt_ptrcache;		//enable caching struct and pointer accesses.

byte ril3_norec;
byte cgif_no3ac;
byte ril3_noril;

byte ccxl_top_only;
byte ccxl_in_func;

byte ccxl_var_needsinit;
byte ccxl_isdef_methodproto;

byte pel_cmpr;			//PEL compression method

byte optmode;			//optimization mode
byte curprd;			//current predication mode
byte verbose;			//optimization mode

int loop_localstate;	

int trace_mult;

int ccxl_tyc_seen;

int gs_seq;
char *lfn;
int lfni;
int lln;

int *gs_srcpos;
int gs_srcmax;

char *fnidx_str[1024];
int fnidx_num;

char *archvar_name[1024];
s64 archvar_val[1024];
short archvar_chn[1024];
int archvar_num;
short archvar_hash[256];

int reqlfn;
// int reqlln;
int ctxflags;

byte regrov;

BCCX_Node *cur_structdef;
BCCX_Node *reduce_tmp;


ccxl_register loadslot_cache_dreg[256];
ccxl_register loadslot_cache_sreg[256];
ccxl_register loadslot_cache_ireg[256];
BGBCC_CCXL_LiteralInfo *loadslot_cache_st[256];
char *loadslot_cache_name[256];
byte loadslot_cache_srov;
byte loadslot_cache_erov;


BGBCC_CCXL_RegisterInfo *cur_func;
BGBCC_CCXL_LiteralInfo *cur_this;
BGBCC_CCXL_VirtOp *cur_vop;
BGBCC_CCXL_VirtTr *cur_vtr;

// BGBCC_CCXL_RegisterInfo *reg_globals[4096];
// int idx_globals[4096];
// BGBCC_CCXL_RegisterInfo *reg_globals[262144];
BGBCC_CCXL_RegisterInfo **reg_globals;
// int idx_globals[262144];
int n_reg_globals;
int m_reg_globals;

BGBCC_CCXL_RegisterInfo *hash_globals[BGBCC_GBLHASH_SZ];	//global lookup hash
// BGBCC_CCXL_RegisterInfo *hash_globals[4096];	//global lookup hash
// BGBCC_CCXL_RegisterInfo *hash_globals[1024];	//global lookup hash
BGBCC_CCXL_RegisterInfo *usort_globals;			//unsorted globals

// BGBCC_CCXL_LiteralInfo *literals[4096];
// int idx_literals[4096];
// BGBCC_CCXL_LiteralInfo *literals[65536];
// int idx_literals[65536];
BGBCC_CCXL_LiteralInfo **literals;
// int *idx_literals;
int n_literals, m_literals;

int hash_literals[BGBCC_GBLHASH_SZ];
// int hash_literals[4096];
int manif_literal;
int usort_literal;		//unsorted literals
int uname_literal;		//literal types lacking a name

int hash_literals_sig[256];

// BGBCC_CCXL_TypeOverflow *tyovf[1024];
// BGBCC_CCXL_TypeOverflow *tyovf[16384];
BGBCC_CCXL_TypeOverflow **tyovf;
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

BGBCC_CCXL_RegisterInfo *reginfo_free;

BGBCC_CCXL_LiteralInfo *cur_objstack[256];
BGBCC_CCXL_LiteralInfo *cur_obj;
int cur_objstackpos;

BGBCC_CCXL_VirtOp *virtop_free;
BGBCC_CCXL_VirtTr *virttr_free;

char *imgname;
char *imgbasename;
fourcc imgfmt;

char **optstrs;
int noptstrs;

BGBCC_CCXL_BackendFuncs_vt *back_vt;
void *uctx;		//context for backend
};

struct BGBCC_CCXL_BackendFuncs_vt_s {
char *tgtname;
BGBCC_CCXL_BackendFuncs_vt *next;

ccxl_status (*SetupParserForArch)(BGBCP_ParseState *ctx);
ccxl_status (*SetupContextForArch)(BGBCC_TransState *ctx);
ccxl_status (*FlattenImage)(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt);
ccxl_status (*EndFunction)(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj);
ccxl_status (*LoadBufferDLL)(BGBCC_TransState *ctx,
	byte *buf, int sz);
ccxl_status (*AddResourceData)(BGBCC_TransState *ctx,
	char *name, byte *buf, int sz, fourcc imgfmt);
};

#include <bgbcc_auto.h>

#ifdef __cplusplus
}
#endif

#endif


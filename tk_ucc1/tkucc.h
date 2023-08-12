#ifndef INC_TKUCC_H
#define INC_TKUCC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

typedef int8_t sbyte;
typedef uint8_t ubyte;
typedef uint8_t byte;

typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef ubyte BYTE;
typedef u16 WORD;
typedef u32 DWORD;
typedef u64 QWORD;

typedef float f32;
typedef double f64;

#if defined(_MSC_VER) || defined(__BGBCC__)
#ifndef TKUCC_DBGBREAK
#define TKUCC_DBGBREAK	__debugbreak();
#endif
#endif

#define TKUCC_TWOCC(a, b)			((a)|((b)<<8))
#define TKUCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define TKUCC_FCC_C					TKUCC_FOURCC('C', ' ', ' ', ' ')
#define TKUCC_FCC_S					TKUCC_FOURCC('S', ' ', ' ', ' ')
#define TKUCC_FCC_O					TKUCC_FOURCC('O', ' ', ' ', ' ')
#define TKUCC_FCC_A					TKUCC_FOURCC('A', ' ', ' ', ' ')

#define TKUCC_FCC_EXE				TKUCC_FOURCC('E', 'X', 'E', ' ')
#define TKUCC_FCC_DLL				TKUCC_FOURCC('D', 'L', 'L', ' ')


typedef struct TKUCC_LineBuf_s			TKUCC_LineBuf;
typedef struct TKUCC_AstNode_s			TKUCC_AstNode;
typedef struct TKUCC_PpDefine_s		TKUCC_PpDefine;
typedef struct TKUCC_ZoneAllocHead_s	TKUCC_ZoneAllocHead;
typedef struct TKUCC_ZoneStrHead_s		TKUCC_ZoneStrHead;
typedef struct TKUCC_MainContext_s		TKUCC_MainContext;

typedef struct TKUCC_LinkObjectLbl_s	TKUCC_LinkObjectLbl;
typedef struct TKUCC_LinkObjectSec_s	TKUCC_LinkObjectSec;
typedef struct TKUCC_LinkObject_s		TKUCC_LinkObject;

typedef struct TKUCC_AsmOpArg_s		TKUCC_AsmOpArg;
typedef struct TKUCC_AsmOp_s			TKUCC_AsmOp;

typedef struct tkucc_valtype_s			tkucc_valtype;
typedef struct tkucc_valreg_s			tkucc_valreg;
typedef struct TKUCC_IROP_s			TKUCC_IROP;
typedef struct TKUCC_IRBB_s			TKUCC_IRBB;
typedef struct TKUCC_IRDECL_s			TKUCC_IRDECL;

struct tkucc_valtype_s { u32 val; };
struct tkucc_valreg_s { u64 val; };


#include "tkucc_line.h"
#include "tkucc_cgtac.h"
#include "tkucc_asm.h"
#include "tkucc_woff.h"

#endif

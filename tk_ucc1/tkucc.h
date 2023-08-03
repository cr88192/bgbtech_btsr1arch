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

#if defined(_MSC_VER) || defined(__BGBCC__)
#ifndef TKUCC_DBGBREAK
#define TKUCC_DBGBREAK	__debugbreak();
#endif
#endif

#define TKUCC_TWOCC(a, b)			((a)|((b)<<8))
#define TKUCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#include "tkucc_line.h"
#include "tkucc_asm.h"
#include "tkucc_woff.h"

#endif

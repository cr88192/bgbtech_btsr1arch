#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#ifndef __x86_64__
#define __x86_64__
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#ifndef __i386__
#define __i386__
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#ifndef __arm__
#define __arm__
#endif
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#ifndef ARM64
#define ARM64
#endif
#endif


#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef _WIN64
#ifndef WIN64
#define WIN64
#endif
#endif

#ifdef _MSC_VER
#ifndef MSVC
#define MSVC
#endif
#endif

#ifndef BOOL_T
#define BOOL_T
typedef unsigned char bool;
#endif

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef U8_T
#define U8_T
typedef unsigned char u8;
#endif

#ifndef S8_T
#define S8_T
typedef signed char s8;
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

#ifndef SNLINT_T
#define SNLINT_T
#if defined(X86_64) || defined(ARM64)
typedef s64 snlint;
typedef u64 unlint;
typedef s64 nlint;
#else
#if defined(X86) || defined(ARM)
typedef s32 snlint;
typedef u32 unlint;
typedef u32 nlint;
#else
/* unknown, assume 64 bits for safety */
typedef s64 snlint;
typedef u64 unlint;
typedef s64 nlint;
#endif
#endif
#endif

#ifndef F32_T
#define F32_T
typedef float f32;
#endif

#ifndef F64_T
#define F64_T
typedef double f64;
#endif

#if defined(X86_64) || defined(ARM64)
#ifndef PTRBITS
#define PTRBITS	64
#endif
#ifndef PTRBITS_64
#define PTRBITS_64
#endif
#endif

#if defined(X86) || defined(ARM)
#ifndef PTRBITS
#define PTRBITS	32
#endif
#ifndef PTRBITS_32
#define PTRBITS_32
#endif
#endif

#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM) || defined(ARM64)
#define LITTLEENDIAN
#endif
#endif


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
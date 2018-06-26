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
#ifndef inline
#define inline __inline
#endif
#endif

#ifdef __ANDROID__
#ifndef ANDROID
#define ANDROID
#endif
#endif

#ifdef __native_client__
#ifndef NATIVECLIENT
#define NATIVECLIENT
#endif
#endif

#ifdef WIN32
#ifdef BGBRASW_DLL
#define BGBRASW_API __declspec(dllexport)
#else
#define BGBRASW_API __declspec(dllimport)
#endif
#else
#define BGBRASW_API
#endif


#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
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
#ifdef X86_64
typedef s64 snlint;
typedef u64 unlint;
typedef s64 nlint;
#else
#ifdef X86
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

#ifdef X86_64
#ifndef PTRBITS
#define PTRBITS	64
#endif
#ifndef PTRBITS_64
#define PTRBITS_64
#endif
#endif

#ifdef X86
#ifndef PTRBITS
#define PTRBITS	32
#endif
#ifndef PTRBITS_32
#define PTRBITS_32
#endif
#endif

#ifdef ARM
#ifndef PTRBITS
#define PTRBITS	32
#endif
#ifndef PTRBITS_32
#define PTRBITS_32
#endif
#endif

#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM)
#define LITTLEENDIAN
#endif
#endif

#if defined(ARM) || defined(ANDROID)
#define BGBGC_SHAVEMEM
#endif

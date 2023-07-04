/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stddef.h - assert header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __STDDEF_INCLUDED
#define __STDDEF_INCLUDED

#ifdef __BGBCC__
typedef __ptrdiff ptrdiff_t;
#else
// typedef int ptrdiff_t;
typedef long ptrdiff_t;
#endif

#if 0
#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
#if (defined(__OS2__) || defined(__32BIT__) || defined(__MVS__) \
    || defined(__CMS__))
typedef unsigned long size_t;
#elif (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC) \
    || defined(__WIN32__) || defined(__gnu_linux__))
typedef unsigned int size_t;
#endif
#endif
#endif

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
// typedef unsigned int size_t;
#ifdef __BGBCC__
typedef __usize_t size_t;
#else
typedef unsigned long size_t;
#endif
// typedef long size_t;
#endif

#ifndef __WCHAR_T_DEFINED
#define __WCHAR_T_DEFINED
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#endif
typedef unsigned short wchar_t;
#endif

#ifndef __ERRNO_T_DEFINED
#define __ERRNO_T_DEFINED
typedef int errno_t;
#endif

#define NULL ((void *)0)
// #define offsetof(x, y) (size_t)&(((x *)0)->y)
#define offsetof(x, y) __offsetof(x, y)

#ifdef __PDPCLIB_DLL
#define __PDPCLIB_API__ __declspec(dllexport)
#define __MAIN_FP__
#else
#define __PDPCLIB_API__
#endif

#ifdef __VX86__
#ifdef __WIN32__
#undef __WIN32__
#endif
#endif

#endif

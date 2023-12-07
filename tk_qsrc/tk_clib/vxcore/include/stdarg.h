/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stdarg.h - stdarg header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __STDARG_INCLUDED
#define __STDARG_INCLUDED

// #ifdef __GNUC__
#if defined(__GNUC__) || defined(__gnuc__) || defined(_BGBMETA)

//#ifndef __GNUC_VA_LIST
//#define __GNUC_VA_LIST
//typedef __builtin_va_list __gnuc_va_list;
//#endif

#ifndef _VALIST
#define _VALIST
// typedef __gnuc_va_list va_list;
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L
#define va_copy(d,s)    __builtin_va_copy(d,s)
#endif
#define __va_copy(d,s)  __builtin_va_copy(d,s)

#else /* __GNUC__ */

typedef char * va_list;

#define va_start(ap, parmN) ap = (char *)&parmN + 4
#define va_arg(ap, type) *(type *)(ap += sizeof(type), ap - sizeof(type))
#define va_end(ap) ap = 0

#endif

#endif

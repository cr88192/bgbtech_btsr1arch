/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stdlib.h - stdlib header file                                    */
/*                                                                   */
/*********************************************************************/

#ifndef __STDLIB_INCLUDED
#define __STDLIB_INCLUDED

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
// #if (defined(__OS2__) || defined(__32BIT__) || defined(__MVS__) \
//    || defined(__CMS__))
typedef unsigned long size_t;
//#elif (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC) \
 //   || defined(__WIN32__) || defined(__gnu_linux__))
//typedef unsigned int size_t;
//#endif
#endif

#ifndef __WCHAR_T_DEFINED
#define __WCHAR_T_DEFINED
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#endif
typedef unsigned short wchar_t;
#endif
typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;

#define NULL ((void *)0)
#define EXIT_SUCCESS 0
#if defined(__MVS__) || defined(__CMS__)
#define EXIT_FAILURE 12
#else
#define EXIT_FAILURE 1
#endif
#define RAND_MAX 32767
#define MB_CUR_MAX 1
#define __NATEXIT 32

extern void (*__userExit[__NATEXIT])(void);

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
#if defined(__MVS__) || defined(__CMS__)
void abort(void) __attribute__((noreturn));
void exit(int status) __attribute__((noreturn));
#else
void abort(void);
void exit(int status);
#endif
void qsort(void *, size_t, size_t,
                           int (*)(const void *, const void *));
void srand(unsigned int seed);
int rand(void);
double atof(const char *nptr);
double strtod(const char *nptr, char **endptr);
int atoi(const char *nptr);
long int atol(const char *nptr);
long int strtol(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);
int mblen(const char *s, size_t n);
int mbtowc(wchar_t *pwc, const char *s, size_t n);
int wctomb(char *s, wchar_t wchar);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);
int abs(int j);
div_t div(int numer, int denom);
long int labs(long int j);
ldiv_t ldiv(long int numer, long int denom);
int atexit(void (*func)(void));
char *getenv(const char *name);
int system(const char *string);
void *bsearch(const void *key, const void *base,
              size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

#ifdef __WATCOMC__
#pragma intrinsic (abs,labs,div,ldiv)
#endif

#ifdef __IBMC__
int _Builtin __abs(int j);
#define abs(j) (__abs((j)))
long int _Builtin __labs(long int j);
#define labs(j) (__labs((j)))
#endif

#endif

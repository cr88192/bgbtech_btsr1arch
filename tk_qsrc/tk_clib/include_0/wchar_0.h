/*
	wchar.h -- extended multibyte and wide character utilities

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/08/20	gwyn@arl.mil

	Implements subclause 7.24 of ISO/IEC 9899:1999 (E).

	Unfortunately, it is NOT QUITE CONFORMING, because it #includes
	<stddef.h> to obtain the definitions for wchar_t and size_t,
	<stdint.h> for WCHAR_MIN and WCHAR_MAX, <stdio.h> for FILE, and
	<time.h> for struct tm.  If you plan to include this header in
	a conforming C implementation, you must fix this.  (FILE should
	*not* be defined by this header, although it is needed in the
	declarations!)
*/

#if	!defined(_WCHAR_H) && !defined(_INC_WCHAR)	/* usual lock names */
#define	_WCHAR_H			/* idempotency lock (section 7.1.2) */
#define	_INC_WCHAR

#include	<stddef.h>		/* XXX -- for wchar_t and size_t */
#include	<stdio.h>		/* XXX -- for FILE (def of __Q8_FILE) */
#include	<time.h>		/* XXX -- for struct tm */

#include	<Q8defs.h>		/* defines the __Q8_* symbols */
#include	<stdint.h>		/* XXX -- for WCHAR_MIN and WCHAR_MAX */

#ifdef	__cplusplus
extern	"C"	{
#endif

/*
	7.24.1	Introduction
*/

#ifndef	__Q8_MBSTATE_T_DEFINED
#define	__Q8_MBSTATE_T_DEFINED		/* idempotency lock */
typedef __Q8_MBSTATE_T	mbstate_t;
#endif
#ifndef	__Q8_SIZE_T_DEFINED
#define	__Q8_SIZE_T_DEFINED		/* idempotency lock */
/* typedef __Q8_SIZE_T	size_t;		/* XXX -- conflicts with native hdrs */
#endif
#ifndef	__Q8_STRUCT_TM_DEFINED
#define	__Q8_STRUCT_TM_DEFINED	/* idempotency lock */
/* XXX -- need to work on this */
#endif
#ifndef	__Q8_WCHAR_T_DEFINED
#define	__Q8_WCHAR_T_DEFINED		/* idempotency lock */
/* typedef __Q8_WCHAR_T	wchar_t;	/* XXX -- conflicts with native hdrs */
#endif
#ifndef	__Q8_WINT_T_DEFINED
#define	__Q8_WINT_T_DEFINED		/* idempotency lock */
typedef __Q8_WINT_T	wint_t;
#endif

#ifndef	NULL				/* to protect against native defs */
#define	NULL		__Q8_NULL
#endif

#if 0	/* XXX -- these __Q8_* symbols are not yet defined */
#ifndef	WCHAR_MAX			/* to protect against native defs */
#define	WCHAR_MAX	__Q8_WCHAR_MAX
#endif

#ifndef	WCHAR_MIN			/* to protect against native defs */
#define	WCHAR_MIN	__Q8_WCHAR_MIN
#endif
#endif	/* XXX */

#ifndef	WEOF				/* to protect against native defs */
#define	WEOF		__Q8_WEOF
#endif

/*
	7.24.2	Formatted wide character input/output functions
*/

extern int	fwprintf __Q8_PARAMS((__Q8_FILE * __Q8_RESTRICT __stream,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format, ...));
extern int	fwscanf __Q8_PARAMS((__Q8_FILE * __Q8_RESTRICT __stream,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format, ...));
extern int	swprintf __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s, size_t __n,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format, ...));
extern int	swscanf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __s,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format, ...));
extern int	vfwprintf __Q8_PARAMS((__Q8_FILE * __Q8_RESTRICT __stream,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format,
			__va_list __arg));
extern int	vfwscanf __Q8_PARAMS((__Q8_FILE * __Q8_RESTRICT __stream,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format,
			__va_list __arg));
extern int	vswprintf __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s, size_t __n,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format,
			__va_list __arg));
extern int	vswscanf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __s,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format,
			__va_list __arg));
extern int	vwprintf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT
			__format, __va_list __arg));
extern int	vwscanf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT
			__format, __va_list __arg));
extern int	wprintf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT
			__format, ...));
extern int	wscanf __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT
			__format, ...));

/*
	7.24.3	Wide character input/output functions
*/

extern wint_t	fgetwc __Q8_PARAMS((__Q8_FILE * __stream));
extern wchar_t	*fgetws __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s, int __n,
			__Q8_FILE * __Q8_RESTRICT __stream));
extern wint_t	fputwc __Q8_PARAMS((wchar_t __c, __Q8_FILE * __stream));
extern int	fputws __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s,
			__Q8_FILE * __Q8_RESTRICT __stream));
extern int	fwide __Q8_PARAMS((__Q8_FILE * __stream, int __mode));
extern wint_t	getwc __Q8_PARAMS((__Q8_FILE * __stream));
extern wint_t	getwchar __Q8_PARAMS((void));
extern wint_t	putwc __Q8_PARAMS((wchar_t __c, __Q8_FILE * __stream));
extern wint_t	putwchar __Q8_PARAMS((wchar_t __c));
extern wint_t	ungetwc __Q8_PARAMS((wint_t __c, __Q8_FILE * __stream));

/*
	7.24.4	General wide string utilities
*/

extern double	wcstod __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr));
extern float	wcstof __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr));
#ifdef	__Q8_GT
extern __Q8_GT
		wcstold __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr));
#endif
extern long	wcstol __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr, int __base));
extern unsigned long
		wcstoul __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr, int __base));
#ifdef	__Q8_QT
extern __Q8_QT
		wcstoll __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr, int __base));
extern unsigned __Q8_QT
		wcstoull __Q8_PARAMS((__Q8_CONST wchar_t * __Q8_RESTRICT __nptr,
			wchar_t ** __Q8_RESTRICT __endptr, int __base));
#endif
extern wchar_t *wcscpy __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2));
extern wchar_t *wcsncpy __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2, size_t __n));
extern wchar_t *wmemcpy __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2, size_t __n));
extern wchar_t *wmemmove __Q8_PARAMS((wchar_t * __s1, __Q8_CONST wchar_t * __s2,
			size_t __n));
extern wchar_t *wcscat __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2));
extern wchar_t *wcsncat __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2, size_t __n));
extern int	wcscmp __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern int	wcscoll __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern int	wcsncmp __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2, size_t __n));
extern size_t	wcsxfrm __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2, size_t __n));
extern int	wmemcmp __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2, size_t __n));
extern wchar_t *wcschr __Q8_PARAMS((__Q8_CONST wchar_t * __s, wchar_t __c));
extern size_t	wcscspn __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern wchar_t *wcspbrk __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern wchar_t *wcsrchr __Q8_PARAMS((__Q8_CONST wchar_t * __s, wchar_t __c));
extern size_t	wcsspn __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern wchar_t *wcsstr __Q8_PARAMS((__Q8_CONST wchar_t * __s1,
			__Q8_CONST wchar_t * __s2));
extern wchar_t *wcstok __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s1,
			__Q8_CONST wchar_t * __Q8_RESTRICT __s2,
			wchar_t ** __Q8_RESTRICT __ptr));
extern wchar_t *wmemchr __Q8_PARAMS((__Q8_CONST wchar_t * __s, wchar_t __c,
			size_t __n));
extern size_t	wcslen __Q8_PARAMS((__Q8_CONST wchar_t * __s));
extern wchar_t *wmemset __Q8_PARAMS((wchar_t * __s, wchar_t __c, size_t __n));

/*
	7.24.5	Wide character time conversion functions
*/

extern size_t	wcsftime __Q8_PARAMS((wchar_t * __Q8_RESTRICT __s,
			size_t __maxsize,
			__Q8_CONST wchar_t * __Q8_RESTRICT __format,
			__Q8_CONST struct tm * __Q8_RESTRICT __timeptr));

/*
	7.24.6	Extended multibyte/wide character conversion utilities
*/

extern wint_t	btowc __Q8_PARAMS((int __c));
extern int	wctob __Q8_PARAMS((wint_t __c));
extern int	mbsinit __Q8_PARAMS((__Q8_CONST mbstate_t *__ps));
extern size_t	mbrlen __Q8_PARAMS((__Q8_CONST char * __Q8_RESTRICT __s,
			size_t __n, mbstate_t * __Q8_RESTRICT __ps));
extern size_t	mbrtowc __Q8_PARAMS((wchar_t * __Q8_RESTRICT __pwc,
			__Q8_CONST char * __Q8_RESTRICT __s, size_t __n,
			mbstate_t * __Q8_RESTRICT __ps));
extern size_t	wcrtomb __Q8_PARAMS((char * __Q8_RESTRICT __s, wchar_t __wc,
			mbstate_t * __Q8_RESTRICT __ps));
extern size_t	mbsrtowcs __Q8_PARAMS((wchar_t * __Q8_RESTRICT __dst,
			__Q8_CONST char ** __Q8_RESTRICT __src, size_t __len,
			mbstate_t * __Q8_RESTRICT __ps));
extern size_t	wcsrtombs __Q8_PARAMS((char * __Q8_RESTRICT __dst,
			__Q8_CONST wchar_t ** __Q8_RESTRICT __src, size_t __len,
			mbstate_t * __Q8_RESTRICT __ps));

#ifdef	__cplusplus
		}
#endif

#endif	/* !defined(_WCHAR_H) && !defined(_INC_WCHAR) */

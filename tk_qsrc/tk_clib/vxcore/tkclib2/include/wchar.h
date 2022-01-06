#ifndef STD_WCHAR_H
#define	STD_WCHAR_H

#include	<stddef.h>
#include	<stdio.h>
#include	<time.h>
#include	<stdint.h>

#ifdef	__cplusplus
extern	"C"	{
#endif

#ifndef	STD_MBSTATE_T
#define	STD_MBSTATE_T
typedef int	mbstate_t;
#endif

#ifndef	STD_WINT_T
#define	STD_WINT_T
typedef unsigned int	wint_t;
#endif

#ifndef	WEOF
#define	WEOF		0xFFFFFFFFU
#endif

extern int	fwprintf(FILE * restrict strm,
			const wchar_t * restrict fmt, ...);
extern int	fwscanf(FILE * restrict strm,
			const wchar_t * restrict fmt, ...);
extern int	swprintf(wchar_t * restrict __s, size_t __n,
			const wchar_t * restrict fmt, ...);
extern int	swscanf(const wchar_t * restrict __s,
			const wchar_t * restrict fmt, ...);
extern int	vfwprintf(FILE * restrict strm,
			const wchar_t * restrict fmt,
			__va_list arg);
extern int	vfwscanf(FILE * restrict strm,
			const wchar_t * restrict fmt,
			__va_list arg);
extern int	vswprintf(wchar_t * restrict __s, size_t __n,
			const wchar_t * restrict fmt,
			__va_list arg);
extern int	vswscanf(const wchar_t * restrict __s,
			const wchar_t * restrict fmt,
			__va_list arg);
extern int	vwprintf(const wchar_t * restrict
			fmt, __va_list arg);
extern int	vwscanf(const wchar_t * restrict
			fmt, __va_list arg);
extern int	wprintf(const wchar_t * restrict
			fmt, ...);
extern int	wscanf(const wchar_t * restrict
			fmt, ...);

extern wint_t	fgetwc(FILE * strm);
extern wchar_t	*fgetws(wchar_t * restrict __s, int __n,
			FILE * restrict strm);
extern wint_t	fputwc(wchar_t __c, FILE * strm);
extern int	fputws(const wchar_t * restrict __s,
			FILE * restrict strm);
extern int	fwide(FILE * strm, int __mode);
extern wint_t	getwc(FILE * strm);
extern wint_t	getwchar(void);
extern wint_t	putwc(wchar_t __c, FILE * strm);
extern wint_t	putwchar(wchar_t __c);
extern wint_t	ungetwc(wint_t __c, FILE * strm);

extern double	wcstod(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr);
extern float	wcstof(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr);

extern long double
		wcstold(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr);

extern long	wcstol(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr, int __base);
extern unsigned long
		wcstoul(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr, int __base);

extern long long
		wcstoll(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr, int __base);
extern unsigned long long
		wcstoull(const wchar_t * restrict __nptr,
			wchar_t ** restrict __endptr, int __base);

extern wchar_t *wcscpy(wchar_t * restrict __s1,
			const wchar_t * restrict __s2);
extern wchar_t *wcsncpy(wchar_t * restrict __s1,
			const wchar_t * restrict __s2, size_t __n);
extern wchar_t *wmemcpy(wchar_t * restrict __s1,
			const wchar_t * restrict __s2, size_t __n);
extern wchar_t *wmemmove(wchar_t * __s1, const wchar_t * __s2,
			size_t __n);
extern wchar_t *wcscat(wchar_t * restrict __s1,
			const wchar_t * restrict __s2);
extern wchar_t *wcsncat(wchar_t * restrict __s1,
			const wchar_t * restrict __s2, size_t __n);
extern int	wcscmp(const wchar_t * __s1,
			const wchar_t * __s2);
extern int	wcscoll(const wchar_t * __s1,
			const wchar_t * __s2);
extern int	wcsncmp(const wchar_t * __s1,
			const wchar_t * __s2, size_t __n);
extern size_t	wcsxfrm(wchar_t * restrict __s1,
			const wchar_t * restrict __s2, size_t __n);
extern int	wmemcmp(const wchar_t * __s1,
			const wchar_t * __s2, size_t __n);
extern wchar_t *wcschr(const wchar_t * __s, wchar_t __c);
extern size_t	wcscspn(const wchar_t * __s1,
			const wchar_t * __s2);
extern wchar_t *wcspbrk(const wchar_t * __s1,
			const wchar_t * __s2);
extern wchar_t *wcsrchr(const wchar_t * __s, wchar_t __c);
extern size_t	wcsspn(const wchar_t * __s1,
			const wchar_t * __s2);
extern wchar_t *wcsstr(const wchar_t * __s1,
			const wchar_t * __s2);
extern wchar_t *wcstok(wchar_t * restrict __s1,
			const wchar_t * restrict __s2,
			wchar_t ** restrict __ptr);
extern wchar_t *wmemchr(const wchar_t * __s, wchar_t __c,
			size_t __n);
extern size_t	wcslen(const wchar_t * __s);
extern wchar_t *wmemset(wchar_t * __s, wchar_t __c, size_t __n);

extern size_t	wcsftime(wchar_t * restrict __s,
			size_t __maxsize,
			const wchar_t * restrict fmt,
			const struct tm * restrict __timeptr);

extern wint_t	btowc(int __c);
extern int	wctob(wint_t __c);
extern int	mbsinit(const mbstate_t *__ps);
extern size_t	mbrlen(const char * restrict __s,
			size_t __n, mbstate_t * restrict __ps);
extern size_t	mbrtowc(wchar_t * restrict __pwc,
			const char * restrict __s, size_t __n,
			mbstate_t * restrict __ps);
extern size_t	wcrtomb(char * restrict __s, wchar_t __wc,
			mbstate_t * restrict __ps);
extern size_t	mbsrtowcs(wchar_t * restrict __dst,
			const char ** restrict __src, size_t __len,
			mbstate_t * restrict __ps);
extern size_t	wcsrtombs(char * restrict __dst,
			const wchar_t ** restrict __src, size_t __len,
			mbstate_t * restrict __ps);

errno_t wmemcpy_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src, rsize_t count );
errno_t wmemmove_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src, rsize_t count );

errno_t wcscpy_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src );

#ifdef	__cplusplus
		}
#endif

#endif

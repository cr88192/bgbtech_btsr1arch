/*
	wchar -- extended multibyte and wide character utilities

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.24 of ISO/IEC 9899:1999 (E).

	This is a minimal implementation for environments where
	internationalization is not considered important.

	It supports an encoding where all char codes are mapped
	to the *same* code values within a wchar_t or wint_t,
	so long as no other wchar_t codes are used by the program.

	This implementation is already compatible with Clive
	Feather's proposed changes detailed in WG14/N898 (DR#212).

	XXX -- UNTESTED -- XXX
*/

#include <vxcore.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <Q8defs.h>		/* defines several __Q8_* symbols */
#include <stdbool.h>
#include <wchar.h>

#ifndef	EILSEQ
#define	EILSEQ		ERANGE
#endif

#ifndef	MB_CUR_MAX
#define	MB_CUR_MAX	1
#endif

/*	helper functions:	*/

static char	x_format[BUFSIZ];
static char	x_s[BUFSIZ];

static bool
shrink(src, dst)
	register __Q8_CONST wchar_t * __Q8_RESTRICT	src;
	register char * __Q8_RESTRICT			dst;
	{
	register int					i;

	for ( i = 0; i < BUFSIZ; ++i )
		if ( (dst[i] = src[i]) == 0 )
			return true;

	errno = EDOM;
	return false;			/* doesn't fit into buffer */
	}

static bool
expand(src, dst)
	register __Q8_CONST char * __Q8_RESTRICT	src;
	register wchar_t * __Q8_RESTRICT		dst;
	{
	register int					i;

	for ( i = 0; i < BUFSIZ; ++i )
		if ( (dst[i] = src[i]) == 0 )
			return true;

	errno = EDOM;
	return false;			/* doesn't fit into buffer */
	}

/*
	7.24.2	Formatted wide character input/output functions
*/

/* VARARGS */
VXCORE_API int
fwprintf(__Q8_T(__Q8_FILE * __Q8_RESTRICT		stream)
	 __Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
	 __Q8_AList
	)
	__Q8_Dcl
	{
	__Q8_D(__Q8_FILE	*stream)
	__Q8_D(wchar_t		*format)
	__Q8_List(		ap)
	int			ret;	/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, __Q8_FILE *, stream)
	__Q8_I(ap, wchar_t *, format)

	ret = vfwprintf(stream, format, ap);

	__Q8_End( ap )
	return ret;
	}

/* VARARGS */
VXCORE_API int
fwscanf(__Q8_T(__Q8_FILE * __Q8_RESTRICT		stream)
	__Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
	__Q8_AList
       )__Q8_Dcl
	{
	__Q8_D(__Q8_FILE	*stream)
	__Q8_D(wchar_t		*format)
	__Q8_List(		ap)
	int			ret;	/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, __Q8_FILE *, stream)
	__Q8_I(ap, wchar_t *, format)

	ret = vfwscanf(stream, format, ap);

	__Q8_End( ap )
	return ret;
	}

/* VARARGS */
VXCORE_API int
snwprintf(__Q8_T(wchar_t * __Q8_RESTRICT			s)
	 __Q8_T(size_t					n)
	 __Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
	 __Q8_AList
	)
	__Q8_Dcl
	{
	__Q8_D(wchar_t	*s)
	__Q8_D(size_t	n)
	__Q8_D(wchar_t	*format)
	__Q8_List(	ap)
	int		ret;		/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, wchar_t *, s)
	__Q8_I(ap, size_t, n)
	__Q8_I(ap, wchar_t *, format)

	ret = vsnwprintf(s, n, format, ap);

	__Q8_End( ap )
	return ret;
	}

/* VARARGS */
VXCORE_API int
swscanf(__Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	s)
	__Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
	__Q8_AList
       )__Q8_Dcl
	{
	__Q8_D(__Q8_CONST wchar_t	*s)
	__Q8_D(wchar_t			*format)
	__Q8_List(			ap)
	int				ret;	/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, wchar_t *, s)
	__Q8_I(ap, wchar_t *, format)

	ret = vswscanf(s, format, ap);

	__Q8_End( ap )
	return ret;
	}

VXCORE_API int
vfwprintf(stream, format, arg)
	__Q8_FILE * __Q8_RESTRICT		stream;
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
	{
	if ( ! shrink(format, x_format) )
		return -1;

	return vfprintf(stream, x_format, arg);
	}

VXCORE_API int
vfwscanf(stream, format, arg)
	__Q8_FILE * __Q8_RESTRICT		stream;
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
{
	if ( ! shrink(format, x_format) )
		return -1;

//	return vfscanf(stream, x_format, arg);
}

VXCORE_API int
vsnwprintf(s, n, format, arg)
	wchar_t * __Q8_RESTRICT			s;
	size_t					n;
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
	{
	int	ret;			/* saved return value */

	if ( n > sizeof x_s )		/* too dangerous to proceed */
		{
		errno = EDOM;
		return -1;
		}

	if ( ! shrink(format, x_format) )
		return -1;

//	ret = vsnprintf(x_s, n, x_format, arg);
	ret = vsprintf(x_s, x_format, arg);

	if ( ! expand(x_s, s) )
		return -1;

	return ret;
	}

VXCORE_API int
vswscanf(s, format, arg)
	__Q8_CONST wchar_t * __Q8_RESTRICT	s;
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
	{
	int	ret;			/* saved return value */

	if ( ! shrink(format, x_format) )
		return -1;

//	ret = vsscanf(x_s, x_format, arg);

	if ( ! expand(x_s, s) )		/* Risky Business */
		return -1;

	return ret;
	}

VXCORE_API int
vwprintf(format, arg)
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
	{
	return vfwprintf(stdout, format, arg);
	}

VXCORE_API int
vwscanf(format, arg)
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	va_list					arg;
	{
	return vfwscanf(stdin, format, arg);
	}

/* VARARGS */
VXCORE_API int
wprintf(__Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
	__Q8_AList
       )__Q8_Dcl
	{
	__Q8_D(wchar_t		*format)
	__Q8_List(		ap)
	int			ret;	/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, wchar_t *, format)

	ret = vwprintf(format, ap);

	__Q8_End( ap )
	return ret;
	}

/* VARARGS */
VXCORE_API int
wscanf(__Q8_T(__Q8_CONST wchar_t * __Q8_RESTRICT	format)
       __Q8_AList
      )	__Q8_Dcl
	{
	__Q8_D(wchar_t		*format)
	__Q8_List(		ap)
	int			ret;	/* saved return value */

	__Q8_Start(ap, format)
	__Q8_I(ap, wchar_t *, format)

	ret = vwscanf(format, ap);

	__Q8_End( ap )
	return ret;
	}

/*
	7.24.3	Wide character input/output functions
*/

VXCORE_API wint_t
fgetwc(stream)
	__Q8_FILE	*stream;
	{
	int		ret;		/* saved return value */

	ret = getc(stream);		/* faster than fgetc */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API wchar_t *
fgetws(s, n, stream)
	wchar_t * __Q8_RESTRICT		s;
	int				n;
	__Q8_FILE * __Q8_RESTRICT	stream;
	{
	char				*ret;	/* saved return value */

	if ( n > sizeof x_s )		/* too dangerous to proceed */
		{
		errno = EDOM;
		return NULL;
		}

	ret = fgets(x_s, n, stream);

	if ( ! expand(x_s, s) )
		return NULL;

	return ret == x_s ? s : NULL;
	}

VXCORE_API wint_t
fputwc(c, stream)
	wchar_t		c;
	__Q8_FILE	*stream;
	{
	int		ret;		/* saved return value */

	ret = putc(c, stream);		/* faster than fputc */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API int
fputws(s, stream)
	const wchar_t * __Q8_RESTRICT		s;
	__Q8_FILE * __Q8_RESTRICT	stream;
	{
	int				ret;	/* saved return value */

	if ( ! shrink(s, x_s) )
		return -1;

	return fputs(x_s, stream);
	}

/*ARGSUSED*/
VXCORE_API int
fwide(stream, mode)
	__Q8_FILE	*stream;
	int		mode;
	{
	return -1;			/* limited to byte orientation */
	}

VXCORE_API wint_t
getwc(stream)
	__Q8_FILE	*stream;
	{
	int		ret;		/* saved return value */

	ret = getc(stream);		/* faster than fgetc */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API wint_t
getwchar()
	{
	int		ret;		/* saved return value */

	ret = getchar();		/* faster than fgetchar */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API wint_t
putwc(c, stream)
	wchar_t		c;
	__Q8_FILE	*stream;
	{
	int		ret;		/* saved return value */

	ret = putc(c, stream);		/* faster than fputc */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API wint_t
putwchar(c)
	wchar_t	c;
	{
	int	ret;			/* saved return value */

	ret = putchar(c);		/* faster than fputchar */
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

VXCORE_API wint_t
ungetwc(c, stream)
	wchar_t		c;
	__Q8_FILE	*stream;
	{
	int		ret;		/* saved return value */

	ret = ungetc(c, stream);
	return ret == EOF ? WEOF : ret;	/* EOF, WEOF values may differ */
	}

/*
	7.24.4	General wide string utilities
*/

VXCORE_API double
wcstod(nptr, endptr)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	{
	double					ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0.0;

	ret = strtod(x_s, &x_end);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}

VXCORE_API float
wcstof(nptr, endptr)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	{
	float					ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0.0;

//	ret = strtof(x_s, &x_end);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}

#if	!defined(__Q8_GT)
VXCORE_API __Q8_GT
wcstold(nptr, endptr)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	{
	__Q8_GT					ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0.0;

	ret = strtold(x_s, &x_end);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}
#endif	/* !defined(__Q8_GT) */

VXCORE_API long
wcstol(nptr, endptr, base)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	int					base;
	{
	long					ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0;

	ret = strtol(x_s, &x_end, base);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}

VXCORE_API unsigned long
wcstoul(nptr, endptr, base)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	int					base;
	{
	unsigned long				ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0;

	ret = strtoul(x_s, &x_end, base);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}

#if	!defined(__Q8_QT)
VXCORE_API __Q8_QT
wcstoll(nptr, endptr, base)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	int					base;
	{
	__Q8_QT					ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0;

	ret = strtoll(x_s, &x_end, base);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}

VXCORE_API unsigned __Q8_QT
wcstoull(nptr, endptr, base)
	__Q8_CONST wchar_t * __Q8_RESTRICT	nptr;
	wchar_t ** __Q8_RESTRICT		endptr;
	int					base;
	{
	unsigned __Q8_QT			ret;	/* saved return value */
	char					*x_end;

	if ( ! shrink(nptr, x_s) )
		return 0;

	ret = strtoull(x_s, &x_end, base);
	*endptr = (wchar_t *)nptr + (x_end - x_s);
	return ret;
	}
#endif	/* !defined(__Q8_QT) */

/* The following don't map via shrink/expand because that's much too slow. */

VXCORE_API wchar_t *
wcscpy(s1, s2)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	{
	wchar_t						*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL )
		return orig_s1;		/* robust */

	while ( (*s1++ = *s2++) != 0 )
		;

	return orig_s1;
	}

VXCORE_API wchar_t *
wcsncpy(s1, s2, n)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	register size_t					n;
	{
	wchar_t						*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL || n == 0 )
		return orig_s1;		/* robust */

	for ( ; n > 0; --n )
		if ( (*s1++ = *s2++) == 0 )
			{
			--n;		/* not really necessary */
			break;
			}

	for ( ; n > 0; --n )
		*s1++ = 0;

	return orig_s1;
	}

VXCORE_API wchar_t *
wmemcpy(s1, s2, n)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	register size_t					n;
	{
	wchar_t						*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL || n == 0 )
		return orig_s1;		/* robust */

	for ( ; n > 0; --n )
		*s1++ = *s2++;

	return orig_s1;
	}

VXCORE_API wchar_t *
wmemmove(s1, s2, n)
	register wchar_t		*s1;
	register __Q8_CONST wchar_t	*s2;
	register size_t			n;
	{
	wchar_t				*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL || n == 0 )
		return orig_s1;		/* robust */

	/* XXX -- The following test works only within a flat address space! */
	if ( s2 >= s1 )
		for ( ; n > 0; --n )
			*s1++ = *s2++;
	else	{
		s1 += n;
		s2 += n;

		for ( ; n > 0; --n )
			*--s1 = *--s2;
		}

	return orig_s1;
	}

VXCORE_API wchar_t *
wcscat(s1, s2)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	{
	wchar_t						*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL )
		return orig_s1;

	while ( *s1 != 0 )
		++s1;

	while ( (*s1++ = *s2++) != 0 )
		;

	return orig_s1;
	}

VXCORE_API wchar_t *
wcsncat(s1, s2, n)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	register size_t					n;
	{
	wchar_t						*orig_s1 = s1;

	if ( s1 == NULL || s2 == NULL || n == 0 )
		return orig_s1;		/* robust */

	while ( *s1 != 0 )
		++s1;

	for ( ; n > 0; --n )
		if ( (*s1++ = *s2++) == 0 )
			return orig_s1;

	*s1 = 0;			/* if orig. n == 0, this is redundant */
	return orig_s1;
	}

VXCORE_API int
wcscmp(s1, s2)
	register __Q8_CONST wchar_t	*s1;
	register __Q8_CONST wchar_t	*s2;
	{
	if ( s1 == s2 )
		return 0;		/* even for NULL pointers */

	if ( (s1 != NULL) != (s2 != NULL) )
		return s2 == NULL ? 1 : -1;	/* robust */

	for ( ; *s1 == *s2; ++s1, ++s2 )
		if ( *s1 == 0 )
			return 0;

	return *s1 - *s2;
	}

VXCORE_API int
wcscoll(s1, s2)
	register __Q8_CONST wchar_t	*s1;
	register __Q8_CONST wchar_t	*s2;
	{
	return wcscmp(s1, s2);
	}

VXCORE_API int
wcsncmp(s1, s2, n)
	register __Q8_CONST wchar_t	*s1;
	register __Q8_CONST wchar_t	*s2;
	size_t				n;
	{
	if ( n == 0 || s1 == s2 )
		return 0;		/* even for NULL pointers */

	if ( (s1 != NULL) != (s2 != NULL) )
		return s2 == NULL ? 1 : -1;	/* robust */

	for ( ; n > 0; ++s1, ++s2, --n )
		if ( *s1 != *s2 )
			return *s1 - *s2;
		else if ( *s1 == 0 )
			return 0;

	return 0;
	}

VXCORE_API size_t
wcsxfrm(s1, s2, n)
	register wchar_t * __Q8_RESTRICT		s1;
	register __Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	register size_t					n;
	{
	if ( s1 == NULL || s2 == NULL || n == 0 )
		{
		if ( s1 != NULL )
			*s1 = 0;

		return 0;		/* robust */
		}

	wcsncpy(s1, s2, n);
	return wcslen(s1);		/* not very efficient */
	}

VXCORE_API int
wmemcmp(s1, s2, n)
	register __Q8_CONST wchar_t	*s1;
	register __Q8_CONST wchar_t	*s2;
	size_t				n;
	{
	if ( n == 0 || s1 == s2 )
		return 0;		/* even for NULL pointers */

	if ( (s1 != NULL) != (s2 != NULL) )
		return s2 == NULL ? 1 : -1;	/* robust */

	for ( ; n > 0; ++s1, ++s2, --n )
		if ( *s1 != *s2 )
			return *s1 - *s2;

	return 0;
	}

VXCORE_API wchar_t *
wcschr(s, c)
	__Q8_CONST wchar_t	*s;
	wchar_t			c;
	{
	if ( s != NULL )
		for ( ; *s != c; ++s )
			if ( *s == 0 )
				return NULL;

	return (wchar_t *)s;
	}

VXCORE_API size_t
wcscspn(s1, s2)
	__Q8_CONST wchar_t		*s1;
	__Q8_CONST wchar_t		*s2;
	{
	register __Q8_CONST wchar_t	*s1p;
	register __Q8_CONST wchar_t	*s2p;

	if ( s1 == NULL || s2 == NULL )
		return wcslen(s1);	/* robust */

	for ( s1p = s1; *s1p != 0 ; ++s1p )
		for ( s2p = s2; *s2p != 0; ++s2p )
			if ( *s2p == *s1p )
				return s1p - s1;

	return s1p - s1;
	}

VXCORE_API wchar_t *
wcspbrk(s1, s2)
	register __Q8_CONST wchar_t	*s1;
	__Q8_CONST wchar_t		*s2;
	{
	register __Q8_CONST wchar_t	*s2p;

	if ( s1 == NULL || s2 == NULL )
		return NULL;		/* robust */

	for ( ; *s1 != 0 ; ++s1 )
		for ( s2p = s2; *s2p != 0; ++s2p )
			if ( *s2p == *s1 )
				return (wchar_t *)s1;

	return NULL;
	}

VXCORE_API wchar_t *
wcsrchr(s, c)
	register __Q8_CONST wchar_t	*s;
	register wchar_t		c;
	{
	register __Q8_CONST wchar_t	*ret = NULL;

	if ( s != NULL )
		/* do-while is necessary to handle the case c==0 */
		do
			if ( *s == c )
				ret = s;
		while ( *s++ != 0 );

	return (wchar_t *)ret;
	}

VXCORE_API size_t
wcsspn(s1, s2)
	__Q8_CONST wchar_t		*s1;
	__Q8_CONST wchar_t		*s2;
	{
	register __Q8_CONST wchar_t	*s1p;
	register __Q8_CONST wchar_t	*s2p;

	if ( s1 == NULL || s2 == NULL )
		return 0;		/* robust */

	for ( s1p = s1; *s1p != 0 ; ++s1p )
		for ( s2p = s2; *s2p != *s1p; ++s2p )
			if ( *s2p == 0 )
				return s1p - s1;

	return s1p - s1;
	}

/*
	My usual strstr() (based on Sunday's variation of Boyer-Moore)
	unfortunately requires an inordinately large shift table for
	wide characters.  In order not to trip you up should that
	occasion arise, I substituted a fine-tuned brute-force search.
	It's still much faster than most implementations one encounters.
*/

VXCORE_API wchar_t *
wcsstr(s1, s2)
	register __Q8_CONST wchar_t	*s1;
	__Q8_CONST wchar_t		*s2;
	{
	register __Q8_CONST wchar_t	*t;
	register __Q8_CONST wchar_t	*s2p = s2;

	if ( s2p == NULL || *s2p == 0 || s1 == NULL )
		return (wchar_t *)s1;	/* robust special cases */

	for ( ; *s1 != 0; ++s1 )
		{
		if ( *s2p == *s1 )	/* quick initial-match test */
			{
			/* Although it is tempting, we cannot omit the test
			   for 0 in the loop, because the match may occur
			   at the very end of s1[], in which case we'd keep
			   on going past the end of both arrays.
			*/
			for ( t = s1; *++s2p == *++t; )
				if ( *s2p == 0 )	/* match at end */
					return (wchar_t *)s1;

			if ( *s2p == 0 )
				return (wchar_t *)s1;	/* match elsewhere */

			s2p = s2;	/* reset pattern for next iteration */
			}
		}

	return NULL;			/* no match (and s2 isn't empty) */
	}

#if 0
VXCORE_API wchar_t *
wcstok(s1, s2, ptr)
	register wchar_t * __Q8_RESTRICT	s1;
	__Q8_CONST wchar_t * __Q8_RESTRICT	s2;
	wchar_t ** __Q8_RESTRICT		ptr;
	{
	register wchar_t			*etok;

	if ( s1 != NULL )
		*ptr = s1;		/* save for later continued use */
	else if ( (s1 = *ptr) == NULL )	/* try to continue scan */
		return NULL;		/* no more tokens */

	s1 += wcsspn(s1, s2);		/* skip over separators */

	if ( *s1 == 0 )
		return NULL;		/* no more tokens; preserve ptr */

	/* valid token starts at current s1 */

	if ( (etok = wcspbrk(s1, s2)) == NULL )	/* find end of token */
		*ptr = NULL;		/* this is the last token */
	else	{
		*etok = 0;		/* terminate token wide string */
		*ptr = etok + 1;	/* next time start just past token */
		}

	return s1;			/* start of valid token */
	}
#endif

VXCORE_API wchar_t *
wmemchr(s, c, n)
	register __Q8_CONST wchar_t	*s;
	register wchar_t		c;
	register size_t			n;
	{
	if ( s != NULL )
		for ( ; n > 0; ++s, --n )
			if ( *s == c )
				return (wchar_t *)s;

	return NULL;
	}

VXCORE_API size_t
wcslen(s)
	register __Q8_CONST wchar_t	*s;
	{
	__Q8_CONST wchar_t		*orig_s = s;

	if ( s == NULL )
		return 0;

	while ( *s != 0 )
		++s;

	return s - orig_s;
	}

VXCORE_API wchar_t *
wmemset(s, c, n)
	register wchar_t	*s;
	register wchar_t	c;
	register size_t		n;
	{
	wchar_t			*orig_s = s;

	if ( s != NULL )
		for ( ; n > 0; --n )
			*s++ = c;

	return orig_s;
	}

/*
	7.24.5	Wide character time conversion functions
*/

VXCORE_API size_t
wcsftime(s, maxsize, format, timeptr)
	wchar_t * __Q8_RESTRICT			s;
	size_t					maxsize;
	__Q8_CONST wchar_t * __Q8_RESTRICT	format;
	__Q8_CONST struct tm * __Q8_RESTRICT	timeptr;
	{
	int					ret;	/* saved return value */

	if ( maxsize > sizeof x_s )	/* too dangerous to proceed */
		{
		errno = EDOM;
		return 0;
		}

	if ( ! shrink(format, x_format) )
		return 0;

	ret = strftime(x_s, maxsize, x_format, timeptr);

	if ( ! expand(x_s, s) )
		return 0;

	return ret;
	}

/*
	7.24.6	Extended multibyte/wide character conversion utilities

	In this *minimal implementation*, all single-byte values are valid,
	except possibly for EOF on platforms where sizeof(char)==sizeof(int).
*/

VXCORE_API wint_t btowc(int c)
{
//	if ( c == EOF || (unsigned char)c != (unsigned int)c )	/* knothole */
//		return WEOF;

	if ((c==EOF) || (c<0) || (c>127))
		return WEOF;
	return((wint_t)c);
}

VXCORE_API int wctob(wint_t c)
{							/* knothole: */
//	if ( c == WEOF || (unsigned char)c != (unsigned __Q8_MT)c )
//		return EOF;
//	return (int)(unsigned char)c;

	if ((c==(wint_t)WEOF) || (c>127))
		return(EOF);
	return(c);
}

VXCORE_API int mbsinit(const mbstate_t *ps)
{
	return(1);			/* don't have shift states */
}

VXCORE_API size_t mbrlen(const char *s, size_t n, mbstate_t *ps)
{
	return(mbrtowc((wchar_t *)NULL, s, n, ps));	/* ps ignored */
}

VXCORE_API size_t mbrtowc(
	wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	unsigned char *us;
	wchar_t t;
	int i, n1;

	if (s==NULL)
	{
		pwc = NULL;
		s = (__Q8_CONST char *)"";
		n = 1;
	}

	if (n==0)return((size_t)-2);

	us=(unsigned char *)s;
	i=*us;
	if((i&0xF0)==0xF0)
		{ return((size_t)-1); }
	else if((i&0xF0)==0xE0)
	{
		if(n<3)return((size_t)-2);
		t=((i&15)<<12)|((us[1]&63)<<6)|(us[2]&63);
		n1=3;
	}else if((i&0xE0)==0xC0)
	{
		if(n<2)return((size_t)-2);
		t=((i&31)<<6)|(us[1]&63);
		n1=2;
	}else if((i&0xC0)==0x80)
		{ return((size_t)-1); }
	else { t=i; n1=1; }

	if(pwc)*pwc=t;
	return(t?n1:0);
}

VXCORE_API size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps)
{
	char dummy[4];
	unsigned char *us;
	int c, n;

	if(s==NULL) { s=dummy; }

	if(wc>65533)
	{
		errno=EILSEQ;
		return((size_t)-1);
	}

	us=(unsigned char *)s;
	if(wc>=2048)
	{
		us[0]=0xE0|((wc>>12)&15);
		us[1]=0x80|((wc>>6)&63);
		us[2]=0x80|(wc&63);
		n=3;
	}else if(wc>=128)
	{
		us[0]=0xC0|((wc>>6)&31);
		us[1]=0x80|(wc&63);
		n=2;
	}else { us[0]=wc; n=1; }

	return(n);
}

VXCORE_API size_t mbsrtowcs(
	wchar_t *dst, const char **src, size_t len, mbstate_t *ps)
{
	size_t ret;	/* from mbrtowc */
	size_t n = 0;	/* # converted */

	if ( src == NULL || *src == NULL )	/* undefined behavior */
		return(0);

	while (n<len)
	{
		ret = mbrtowc(dst, *src, (dst==NULL)?MB_CUR_MAX:(len-n), ps);
		if(!ret)break;
		if(ret==((size_t)-2))break;
		if(ret==((size_t)-1))break;
		dst++;
		if (*src!=NULL)*src+=ret;
		n+=ret;
	}

	if ((dst!=NULL) && (n<len) && (ret==0))
		*src = (char *)NULL;
	return(n);
}

VXCORE_API size_t wcsrtombs(
	char *dst, const wchar_t **src, size_t len, mbstate_t *ps)
{
	size_t ret;	/* from mbrtowc */
	size_t n = 0;	/* # converted */

	if((src==NULL) || (*src==NULL))	/* undefined behavior */
		return(0);

	while (n<len)
	{
		ret = wcrtomb(dst, **src, ps);
		if(!ret)break;
		if(ret==((size_t)-2))break;
		if(ret==((size_t)-1))break;

		dst++;
		if (*src!=NULL )
			*src+=ret;
		n+=ret;
	}

	if((dst!=NULL) && (n<len) && (ret==0))
		*src=(wchar_t *)NULL;
	return(n);
}

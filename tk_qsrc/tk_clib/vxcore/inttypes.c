/*
	inttypes -- format conversion of integer types
			(standard library functions declared by <inttypes.h>)

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.8.2 of ISO/IEC 9899:1999 (E).

	This particular implementation requires the matching <inttypes.h>.
	It also assumes that character codes for A..Z and a..z are in
	contiguous ascending order; this is true for ASCII but not EBCDIC.
*/

#include	<ctype.h>
#include	<errno.h>
#include	<stdlib.h>

#include	<stdbool.h>
#include	<inttypes.h>		/* defines various __Q8_* symbols */
#include	<wchar.h>
#include	<wctype.h>

/*	helper macros:	*/

/* convert digit character to number, in any base */
#define ToNumber(c)	(isdigit(c) ? (c) - '0' : \
			 isupper(c) ? (c) - 'A' + 10 : \
			 islower(c) ? (c) - 'a' + 10 : \
			 -1		/* "invalid" flag */ \
			)
#define ToWNumber(c)	(iswdigit(c) ? (c) - L'0' : \
			 iswupper(c) ? (c) - L'A' + 10 : \
			 iswlower(c) ? (c) - L'a' + 10 : \
			 -1		/* "invalid" flag */ \
			)

/* validate converted digit character for specific base */
#define valid(n, b)	((n) >= 0 && (n) < (b))

/*
	7.8.2	Functions for greatest-width integer types
*/

intmax_t
imaxabs(j)
	intmax_t	j;
	{
	return	j >= 0 ? j : -j;
	}

#if 0
imaxdiv_t imaxdiv(imaxdiv_t numer, imaxdiv_t denom)
{
	imaxdiv_t	result;

//	result.quot = numer / denom;
//	result.rem = numer % denom;

	result.quot = (int)numer / (int)denom;
	result.rem = (int)numer % (int)denom;

	return result;
}
#endif

#if 0
intmax_t
strtoimax(nptr, endptr, base)
	register const char *	nptr;
	char **			endptr;
	register int				base;
	{
	register uintmax_t	accum;	/* accumulates converted value */
	register int		n;	/* numeral from digit character */
	bool			minus;	/* set iff minus sign seen */
	bool			toobig;	/* set iff value overflows */

	if ( endptr != NULL )
		*endptr = (char *)nptr;	/* in case no conversion's performed */

	if ( base < 0 || base == 1 || base > 36 )
		{
		errno = EDOM;
		return 0;		/* unspecified behavior */
		}

	/* skip initial, possibly empty sequence of white-space characters */

	while ( isspace(*nptr) )
		++nptr;

	/* process subject sequence: */

	/* optional sign */
	if ( (minus = *nptr == '-') || *nptr == '+' )
		++nptr;

	if ( base == 0 )
		if ( *nptr == '0' )
			if ( nptr[1] == 'X' || nptr[1] == 'x' )
				base = 16;
			else
				base = 8;
		else
				base = 10;

	/* optional "0x" or "0X" for base 16 */

	if ( base == 16 && *nptr == '0' && (nptr[1] == 'X' || nptr[1] == 'x') )
		nptr += 2;		/* skip past this prefix */

	/* check whether there is at least one valid digit */

	n = ToNumber(*nptr);
	++nptr;

	if ( !valid(n, base) )
		return 0;		/* subject seq. not of expected form */

	accum = n;

	for ( toobig = false; n = ToNumber(*nptr), valid(n, base); ++nptr )
		if ( accum > INTMAX_MAX / base + 2 )	/* major wrap-around */
			toobig = true;	/* but keep scanning */
		else
			accum = base * accum + n;

	if ( endptr != NULL )
		*endptr = (char *)nptr;	/* points to first not-valid-digit */

#ifdef	__Q8_TC				/* twos-complement representation */
	if ( minus )
		{
		if ( accum > (uintmax_t)INTMAX_MAX + 1 )
			toobig = true;
		}
	else
#endif
	if ( accum > (uintmax_t)INTMAX_MAX )
		toobig = true;

	if ( toobig )
		{
		errno = ERANGE;
		return minus ? INTMAX_MIN : INTMAX_MAX;
		}
	else
		return (intmax_t)(minus ? -accum : accum);
	}
#endif

#if 0
uintmax_t
strtoumax(nptr, endptr, base)
	register const char *	nptr;
	char **			endptr;
	register int				base;
	{
	register uintmax_t	accum;	/* accumulates converted value */
	register uintmax_t	next;	/* for computing next value of accum */
	register int		n;	/* numeral from digit character */
	bool			minus;	/* set iff minus sign seen (yes!) */
	bool			toobig;	/* set iff value overflows */

	if ( endptr != NULL )
		*endptr = (char *)nptr;	/* in case no conversion's performed */

	if ( base < 0 || base == 1 || base > 36 )
		{
		errno = EDOM;
		return 0;		/* unspecified behavior */
		}

	/* skip initial, possibly empty sequence of white-space characters */

	while ( isspace(*nptr) )
		++nptr;

	/* process subject sequence: */

	/* optional sign (yes!) */

	if ( (minus = *nptr == '-') || *nptr == '+' )
		++nptr;

	if ( base == 0 )
		if ( *nptr == '0' )
			if ( nptr[1] == 'X' || nptr[1] == 'x' )
				base = 16;
			else
				base = 8;
		else
				base = 10;

	/* optional "0x" or "0X" for base 16 */

	if ( base == 16 && *nptr == '0' && (nptr[1] == 'X' || nptr[1] == 'x') )
		nptr += 2;		/* skip past this prefix */

	/* check whether there is at least one valid digit */

	n = ToNumber(*nptr);
	++nptr;

	if ( !valid(n, base) )
		return 0;		/* subject seq. not of expected form */

	accum = n;

	for ( toobig = false; n = ToNumber(*nptr), valid(n, base); ++nptr )
		if ( accum > UINTMAX_MAX / base + 1	/* major wrap-around */
		  || (next = base * accum + n) < accum	/* minor wrap-around */
		   )
			toobig = true;	/* but keep scanning */
		else
			accum = next;

	if ( endptr != NULL )
		*endptr = (char *)nptr;	/* points to first not-valid-digit */

	if ( toobig )
		{
		errno = ERANGE;
		return UINTMAX_MAX;
		}
	else
		return minus ? -accum : accum;	/* (yes!) */
	}
#endif

#if 0
intmax_t
wcstoimax(nptr, endptr, base)
	register const wchar_t *	nptr;
	wchar_t **				endptr;
	register int					base;
	{
	register uintmax_t	accum;	/* accumulates converted value */
	register int		n;	/* numeral from digit character */
	bool			minus;	/* set iff minus sign seen */
	bool			toobig;	/* set iff value overflows */

	if ( endptr != NULL )
		*endptr = (wchar_t *)nptr;	/* in case no conv performed */

	if ( base < 0 || base == 1 || base > 36 )
		{
		errno = EDOM;
		return 0;		/* unspecified behavior */
		}

	/* skip initial, possibly empty sequence of white-space w.characters */

	while ( iswspace(*nptr) )
		++nptr;

	/* process subject sequence: */

	/* optional sign */

	if ( (minus = *nptr == L'-') || *nptr == L'+' )
		++nptr;

	if ( base == 0 )
		if ( *nptr == L'0' )
			if ( nptr[1] == L'X' || nptr[1] == L'x' )
				base = 16;
			else
				base = 8;
		else
				base = 10;

	/* optional "0x" or "0X" for base 16 */

	if ( base == 16 && *nptr == L'0'
	  && (nptr[1] == L'X' || nptr[1] == L'x')
	   )
		nptr += 2;		/* skip past this prefix */

	/* check whether there is at least one valid digit */

	n = ToWNumber(*nptr);
	++nptr;

	if ( !valid(n, base) )
		return 0;		/* subject seq. not of expected form */

	accum = n;

	for ( toobig = false; n = ToWNumber(*nptr), valid(n, base); ++nptr )
		if ( accum > INTMAX_MAX / base + 2 )	/* major wrap-around */
			toobig = true;	/* but keep scanning */
		else
			accum = base * accum + n;

	if ( endptr != NULL )
		*endptr = (wchar_t *)nptr;	/* -> first not-valid-digit */

#ifdef	__Q8_TC				/* twos-complement representation */
	if ( minus )
		{
		if ( accum > (uintmax_t)INTMAX_MAX + 1 )
			toobig = true;
		}
	else
#endif
	if ( accum > (uintmax_t)INTMAX_MAX )
		toobig = true;

	if ( toobig )
		{
		errno = ERANGE;
		return minus ? INTMAX_MIN : INTMAX_MAX;
		}
	else
		return (intmax_t)(minus ? -accum : accum);
	}
#endif

#if 0
uintmax_t
wcstoumax(nptr, endptr, base)
	register const wchar_t *	nptr;
	wchar_t **				endptr;
	register int					base;
	{
	register uintmax_t	accum;	/* accumulates converted value */
	register uintmax_t	next;	/* for computing next value of accum */
	register int		n;	/* numeral from digit character */
	bool			minus;	/* set iff minus sign seen (yes!) */
	bool			toobig;	/* set iff value overflows */

	if ( endptr != NULL )
		*endptr = (wchar_t *)nptr;	/* in case no conv performed */

	if ( base < 0 || base == 1 || base > 36 )
		{
		errno = EDOM;
		return 0;		/* unspecified behavior */
		}

	/* skip initial, possibly empty sequence of white-space w.characters */

	while ( iswspace(*nptr) )
		++nptr;

	/* process subject sequence: */

	/* optional sign */

	if ( (minus = *nptr == L'-') || *nptr == L'+' )
		++nptr;

	if ( base == 0 )
		if ( *nptr == L'0' )
			if ( nptr[1] == L'X' || nptr[1] == L'x' )
				base = 16;
			else
				base = 8;
		else
				base = 10;

	/* optional "0x" or "0X" for base 16 */

	if ( base == 16 && *nptr == L'0'
	  && (nptr[1] == L'X' || nptr[1] == L'x')
	   )
		nptr += 2;		/* skip past this prefix */

	/* check whether there is at least one valid digit */

	n = ToWNumber(*nptr);
	++nptr;

	if ( !valid(n, base) )
		return 0;		/* subject seq. not of expected form */

	accum = n;

	for ( toobig = false; n = ToWNumber(*nptr), valid(n, base); ++nptr )
		if ( accum > UINTMAX_MAX / base + 1	/* major wrap-around */
		  || (next = base * accum + n) < accum	/* minor wrap-around */
		   )
			toobig = true;	/* but keep scanning */
		else
			accum = next;

	if ( endptr != NULL )
		*endptr = (wchar_t *)nptr;	/* -> first not-valid-digit */

	if ( toobig )
		{
		errno = ERANGE;
		return UINTMAX_MAX;
		}
	else
		return minus ? -accum : accum;	/* (yes!) */
	}
#endif

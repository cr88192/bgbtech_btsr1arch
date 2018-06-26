/*
	wctype.h -- wide character classification and mapping utilities

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/08/20	gwyn@arl.mil

	Implements subclause 7.25 of ISO/IEC 9899:1999 (E).
*/

#if	!defined(_WCTYPE_H) && !defined(_INC_WCTYPE)	/* usual lock names */
#define	_WCTYPE_H			/* idempotency lock (section 7.1.2) */
#define	_INC_WCTYPE

#include	<Q8defs.h>		/* defines the __Q8_* symbols */

#ifdef	__cplusplus
extern	"C"	{
#endif

/*
	7.25.1	Introduction
*/

#ifndef	__Q8_WINT_T_DEFINED
#define	__Q8_WINT_T_DEFINED		/* idempotency lock */
typedef __Q8_WINT_T	wint_t;
#endif
#ifndef	__Q8_WCTRANS_T_DEFINED
#define	__Q8_WCTRANS_T_DEFINED		/* idempotency lock */
typedef unsigned int	wctrans_t;	/* this type works nearly everywhere */
#endif
#ifndef	__Q8_WCTYPE_T_DEFINED
#define	__Q8_WCTYPE_T_DEFINED		/* idempotency lock */
typedef int		wctype_t;	/* this type works nearly everywhere */
#endif

#ifndef	WEOF				/* to protect against native defs */
#define	WEOF		__Q8_WEOF
#endif

/*
	7.25.2	Wide character classification utilities
*/

extern int	iswalnum __Q8_PARAMS((wint_t __wc));
extern int	iswalpha __Q8_PARAMS((wint_t __wc));
extern int	iswblank __Q8_PARAMS((wint_t __wc));
extern int	iswcntrl __Q8_PARAMS((wint_t __wc));
extern int	iswdigit __Q8_PARAMS((wint_t __wc));
extern int	iswgraph __Q8_PARAMS((wint_t __wc));
extern int	iswlower __Q8_PARAMS((wint_t __wc));
extern int	iswprint __Q8_PARAMS((wint_t __wc));
extern int	iswpunct __Q8_PARAMS((wint_t __wc));
extern int	iswspace __Q8_PARAMS((wint_t __wc));
extern int	iswupper __Q8_PARAMS((wint_t __wc));
extern int	iswxdigit __Q8_PARAMS((wint_t __wc));
extern int	iswctype __Q8_PARAMS((wint_t __wc, wctype_t __desc));
extern wctype_t	wctype __Q8_PARAMS((__Q8_CONST char *__property));

/*
	7.25.3	Wide character case mapping utilities
*/

extern wint_t	towlower __Q8_PARAMS((wint_t __wc));
extern wint_t	towupper __Q8_PARAMS((wint_t __wc));
extern wint_t	towctrans __Q8_PARAMS((wint_t __wc, wctrans_t __desc));
extern wctrans_t
		wctrans __Q8_PARAMS((__Q8_CONST char *__property));

#ifdef	__cplusplus
		}
#endif

#endif	/* !defined(_WCTYPE_H) && !defined(_INC_WCTYPE) */

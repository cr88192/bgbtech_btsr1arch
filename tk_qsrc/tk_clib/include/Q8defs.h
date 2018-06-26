/*
	Q8defs.h -- defines platform-specific parameters for standard headers

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Several of the standard headers provided by Doug Gwyn make use
	of these __Q8_* symbols.  If you want to define other names of
	the form __Q8_*, please coordinate them with gwyn@arl.mil, who
	claims "dibs" on (first-come ownership of) that name space.

	Aside:	Why "Q8"?  That was used as a system external symbol
	prefix in old CDC Fortran implementations, to avoid link-time
	name-space collisions with user-defined symbols, on the
	assumption that no user would ever think of using such a name.
*/

/* This header doesn't need an idempotency lock; there are no typedefs here. */

/* This header doesn't need C++ extern "C"; there are no declarations here. */

#define	__Q8_TEST		/* [undef to disable configuration checks] */

#define	_SYS_INT_TYPES_H		/* kludge to override Solaris header */
#define	_SYS_INT_LIMITS_H		/* ditto */

/*
	The following macros come in handy in many situations.
*/

/* splice arguments into one token */
#ifdef	__STDC__
#define	__Q8_1J(a,b)	a ## b
#define	__Q8_2J(a,b)	__Q8_1J(a,b)
#else
#define	__Q8_1J(a)	a
#define	__Q8_2J(a,b)	__Q8_1J(a)b
#endif

/* older versions of C didn't have certain type qualifiers */
#ifdef	__STDC__
#define	__Q8_CONST	const
#else
#define	__Q8_CONST
#endif

#if	__STDC_VERSION__ >= 199901
#define	__Q8_RESTRICT	restrict
#else
#define	__Q8_RESTRICT	/* nothing */
#endif

/* older versions of C don't support function prototypes */
#ifdef	__STDC__
#define	__Q8_PARAMS(a)	a
#else
#define	__Q8_PARAMS(a)	()
#endif
/* usage:	return_type func_name __Q8_PARAMS((param_decls)); */

/*
	The following is based on the MUVES "Va" (variable argument) package.

	created:	94/08/16	D A Gwyn

	The Va package provides portable support for functions taking
	a variable number of arguments.  It defines several macros
	that work together to hide the differences between the old
	UNIX <varargs.h> facility and the new Standard C <stdarg.h>.

	Rather than describing each Va package macro separately, it is
	best to give an example of the proper usage of the whole set of
	macros.  It should be easy to adapt this generic example to any
	specific requirement.

	The example is for a function named Advise that has two
	required arguments, the first being a printf-like format string
	and the second a flag that indicates (when true) that an extra
	"verbosity level" is provided as the third argument.  Remaining
	arguments are those, if any, associated with the format string.
	The Advise function prints the formatted result on the standard
	error output if and only if the verbosity level is given and is
	greater than 0.  It returns true unless it had trouble printing
	on the standard error output stream.

	Any code that wants to invoke the Advise function must include
	a proper declaration for it:

	#include	<Q8defs.h>		// includes Va package
	extern bool	Advise( __Q8_T(const char *) __Q8_T(bool) __Q8_Dots );
	// alternatively:
	// extern bool	Advise __Q8_PARAMS(( const char *, bool, ... ));

	The implementation of the Advise function might be:

	#include	<stdbool.h>
	#include	<stdio.h>
	#ifdef	__STDC__
	#include	<stdarg.h>
	#else
	#include	<varargs.h>
	#endif
	#include	<Q8defs.h>

	// VARARGS				// not VARARGS2
	bool
	Advise( __Q8_T( const char *format ) __Q8_T( bool verbose ) __Q8_AList )
		__Q8_Dcl
		{
		__Q8_D( const char *	format )
		__Q8_D( bool		verbose )
		__Q8_List(		ap )
		register int		verbosity;
		register bool		status;

		__Q8_Start( ap, verbose )
		__Q8_I( ap, char *, format )	// no "const" here
		__Q8_I( ap, bool, verbose )

		if ( verbose )
			verbosity = __Q8_Arg( ap, int );
		else
			verbosity = 0;

		if ( verbosity > 0 )
			status = vfprintf( stderr, format, ap ) > 0;
		else
			status = true;

		__Q8_End( ap )
		return status;
		}

	Note that several of these macros are reminiscent of the va_*
	macros in <varargs.h> or <stdarg.h>, but there are significant
	differences.  Proper usage of the "function-like" macros, in
	particular, does not require semicolons; this is intentional,
	in order to avoid warnings about "null statements" from certain
	compilers and "lint".  The easiest way to ensure correct usage
	is to copy the above example and then make changes to the copy
	as needed for the specific application.
*/

#ifdef	__STDC__

#define	__Q8_T(t)		t,
#define	__Q8_Dots		...
#define	__Q8_AList		...
#define	__Q8_D(d)		/* nothing */
#define	__Q8_Dcl		/* nothing */
#define	__Q8_List(ap)		va_list ap;
#define	__Q8_Start(ap, A0)	va_start(ap, A0);
#define	__Q8_I(ap, T, Ai)	/* nothing */
#define	__Q8_Arg(ap, T)		va_arg(ap, T)
#define	__Q8_End(ap)		va_end(ap);

#else	/* "classic" version of UNIX assumed */

#define	__Q8_T(t)		/* nothing */
#define	__Q8_Dots		/* nothing */
#define	__Q8_AList		va_alist
#define	__Q8_D(d)		d;
#define	__Q8_Dcl		va_dcl
#define	__Q8_List(ap)		va_list ap;
#define	__Q8_Start(ap, A0)	va_start(ap);
#define	__Q8_I(ap, T, Ai)	Ai = va_arg(ap, T);
#define	__Q8_Arg(ap, T)		va_arg(ap, T)
#define	__Q8_End(ap)		va_end(ap);

#endif

/*
	The following are used to define macros in multiple headers.
*/

/* NOTE: The following must be spelled *exactly* like EOF in <stdio.h>. */
#define	__Q8_EOF	(-1)
/* XXX -- not currently used in the headers I provide. */

/* NOTE: The following must be spelled *exactly* like NULL in <stddef.h> etc. */
#if	defined(_LP64) && !defined(__cplusplus)	/* Why? */
#define	__Q8_NULL	0L
#else
#define	__Q8_NULL	0
#endif

#define	__Q8_WEOF	((wint_t)-1)

/*
	The following are used to typedef types in multiple headers.
*/

#define	__Q8_MBSTATE_T	int		/* dummy; usually a struct */
#define	__Q8_WINT_T	int		/* this type works nearly everywhere */
#define	__Q8_FILE	FILE		/* must match <stdio.h> */
	/* XXX -- At present, __Q8_FILE requires #including <stdio.h>. */
/* XXX -- the following are not currently used in the headers I provide. */
#define	__Q8_SIZE_T	unsigned long	/* this type works nearly everywhere */
#define	__Q8_WCHAR_T	char		/* for minimal implementations */
/* XXX -- this is incomplete; these should be coupled with the following
  parameter definitions, and their limits should be computed and defined. */

/*
	The following defines parameters for supported types;
	they are used mainly by <stdint.h>.

	C:char  S:short  I:int  L:long  P:void*  Q:long long  M:max-width int.
	D:ptrdiff_t  A:sig_atomic_t  Z:size_t  W:wchar_t  X:wint_t
	G:long double
	followed by	W:width  T:type  U:is unsigned  S:suffix for constant
			F:printf/scanf length modifier

	TC:twos-complement	OC:ones-complement	SM:signed-magnitude
	SC:"signed char" or "char" if signed

	These values depend strongly on the architecture and compiler;
	the only combinations included here are the ones I use myself.
	Please add your particular combination and e-mail me the edits.

	XXX -- system parameters really ought to be in a separate header.
*/

#if defined(__BJX1_64__)

#define	__Q8_TC
#define	__Q8_SC	/* signed */ char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_LW	64
#define	__Q8_PW	64
#define	__Q8_DW	64
#define	__Q8_AW	32
#undef	__Q8_AU
#define	__Q8_ZW	32
#define	__Q8_WW	32
#undef	__Q8_WU
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#endif
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double


#elif	defined(__i386) || defined(__sh__)

#define	__Q8_TC
#define	__Q8_SC	/* signed */ char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_LW	32
#define	__Q8_PW	32
#define	__Q8_DW	32
#define	__Q8_AW	32
#undef	__Q8_AU
#define	__Q8_ZW	32
#define	__Q8_WW	32
#undef	__Q8_WU
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#endif
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double

#elif	defined(_M_IX86)		/* Intel x86, 32-bit, Microsoft */

#define	__Q8_TC
#define	__Q8_SC	signed char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_LW	32
#define	__Q8_PW	32
#define	__Q8_DW	32
#define	__Q8_AW	32
#undef	__Q8_AU
#define	__Q8_ZW	32
#define	__Q8_WW	32
#undef	__Q8_WU
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#define	__Q8_QW	64
#define	__Q8_QT	__int64
#define	__Q8_QS	i64
#define	__Q8_QF	"I64"
#endif
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double

#elif	defined(__sparc)		/* SPARC, Sun Solaris */

#define	__Q8_TC
#define	__Q8_SC	/* signed */ char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_AW	32
#undef	__Q8_AU
#ifdef	__sparcv9			/* 64-bit environment */
#define	__Q8_LW	64
#else					/* 32-bit environment */
#define	__Q8_LW	32
#endif
#define	__Q8_PW	__Q8_LW
#define	__Q8_DW	__Q8_LW
#define	__Q8_ZW	__Q8_LW
#define	__Q8_WW	__Q8_LW
#undef	__Q8_WU
#define	__Q8_XW	__Q8_LW
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#endif
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double

#elif	defined(_TMS320C6000)		/* TI TMS320C6xxx DSP, Code Composer */

#define	__Q8_TC
#define	__Q8_SC	signed char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_AW	32
#undef	__Q8_AU
#define	__Q8_LW	40
#define	__Q8_PW	32
#define	__Q8_DW	32
#define	__Q8_ZW	32
#define	__Q8_WW	8
#define	__Q8_WU			/* XXX -- check */
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#define	__Q8_MW	__Q8_LW
#define	__Q8_MT	long
#define	__Q8_MS L
#define	__Q8_MF "l"
#endif
#define	__Q8_GT	long double

#elif	defined(__vax)			/* DEC VAX-11 */

#define	__Q8_TC
#define	__Q8_SC	/* signed */ char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	32
#define	__Q8_LW	32
#define	__Q8_PW	32
#define	__Q8_DW	32
#define	__Q8_AW	32
#undef	__Q8_AU
#define	__Q8_ZW	32
#define	__Q8_WW	32
#undef	__Q8_WU
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#define	__Q8_MW	32
#define	__Q8_MT	long
#define	__Q8_MS	L
#undef	__Q8_GT		/* long double not supported */
#endif

#elif	defined(__pdp11)		/* DEC PDP-11 */

#define	__Q8_TC
#define	__Q8_SC	/* signed */ char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	16
#define	__Q8_LW	32
#define	__Q8_PW	16	/* (violates section 5.2.4.1) */
#define	__Q8_DW	16	/* (violates section 5.2.4.1) */
#define	__Q8_AW	16
#undef	__Q8_AU
#define	__Q8_ZW	16
#define	__Q8_WW	32
#undef	__Q8_WU
#define	__Q8_XW	32
#undef	__Q8_XU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#define	__Q8_GT	long double
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#define	__Q8_MW	32
#define	__Q8_MT	long
#define	__Q8_MS	L
#define	__Q8_MF	"l"
#undef	__Q8_GT		/* long double not supported */
#endif

#elif	defined(__ORCAC__)		/* Apple IIGS, large memory model */

/* #pragma	optimize	-1	/* enables all compiler optimizations */

#define	__Q8_TC
#define	__Q8_SC	signed char
#define	__Q8_CW	8
#define	__Q8_SW	16
#define	__Q8_IW	16
#define	__Q8_LW	32
#define	__Q8_PW	32
#define	__Q8_DW	32
#define	__Q8_AW	16
#undef	__Q8_AU
#define	__Q8_ZW	32		/* XXX -- check */
#define	__Q8_WW	32
#undef	__Q8_WU
#if	__STDC_VERSION__ >= 199901
#define	__Q8_CF	"hh"
#define	__Q8_QW	64
#define	__Q8_QT	long long
#define	__Q8_QS	LL
#define	__Q8_QF	"ll"
#define	__Q8_MW	__Q8_QW
#define	__Q8_MT	__Q8_QT
#define	__Q8_MS	__Q8_QS
#define	__Q8_MF	__Q8_QF
#else
#undef	__Q8_CF		/* no known length modifier for character type */
#define	__Q8_MW	32
#define	__Q8_MT	long
#define	__Q8_MS	L
#define	__Q8_MF	"l"
#endif
#define	__Q8_GT	long double

#else

#error	"Q8defs.h: unknown CPU type"

#endif

#if	defined(__Q8_TEST)

/*	sanity checks	*/

#if	defined(__Q8_TC)+defined(__Q8_OC)+defined(__Q8_SM) != 1
#error	"Q8defs.h: representation improperly defined"
#endif

#if	__Q8_CW < 8
#error	"Q8defs.h: __Q8_CW improperly defined"
#endif

#if	__Q8_SW < __Q8_CW || __Q8_SW < 16
#error	"Q8defs.h: __Q8_SW improperly defined"
#endif

#if	__Q8_IW < __Q8_SW
#error	"Q8defs.h: __Q8_IW improperly defined"
#endif

#if	__Q8_LW < __Q8_IW || __Q8_LW < 32 
#error	"Q8defs.h: __Q8_LW improperly defined"
#endif

#ifdef	__Q8_PW
#if	__Q8_PW < 16 || __Q8_PW < __Q8_CW || \
	__STDC_VERSION__ >= 199901 && __Q8_PW < 17
#error	"Q8defs.h: __Q8_PW improperly defined"
#endif
#endif

#ifdef	__Q8_DW
#if	__Q8_DW < 16 || __Q8_DW < __Q8_CW || \
	__STDC_VERSION__ >= 199901 && __Q8_DW < 17
#error	"Q8defs.h: __Q8_DW improperly defined"
#endif
#endif

#if	__Q8_AW < __Q8_CW
#error	"Q8defs.h: __Q8_AW improperly defined"
#endif

#if	__Q8_ZW < 16 || __Q8_ZW < __Q8_CW
#error	"Q8defs.h: __Q8_ZW improperly defined"
#endif

#if	__Q8_WW < __Q8_CW
#error	"Q8defs.h: __Q8_WW improperly defined"
#endif

#if	__Q8_XW < 16 || __Q8_XW < __Q8_CW
#error	"Q8defs.h: __Q8_XW improperly defined"
#endif

#ifdef	__Q8_QT
#if	__Q8_QW < __Q8_LW || __Q8_QW < 64
#error	"Q8defs.h: __Q8_QW improperly defined"
#endif
#elif	__STDC_VERSION__ >= 199901
#error	"Q8defs.h: __Q8_QT not defined"
#endif

#if	__Q8_MW < __Q8_LW || defined(__Q8_QT) && __Q8_MW < __Q8_QW
#error	"Q8defs.h: __Q8_MW improperly defined"
#endif

#ifndef	__Q8_MT
#error	"Q8defs.h: __Q8_MT not defined"
#endif

#endif	/* defined(__Q8_TEST) */

/*	limits for various types	*/

/* top signed values */
#define	__Q8_TI(w)	(~(~0<<(w)-1))
#define	__Q8_TL		(~(~0L<<__Q8_LW-1))
#define	__Q8_TQ		(~(~__Q8_2J(0,__Q8_QS)<<__Q8_QW-1))
#define	__Q8_TM		(~(~__Q8_2J(0,__Q8_MS)<<__Q8_MW-1))

/* upper unsigned values */
#define	__Q8_UI(w)	(~0U>>__Q8_IW-(w))
#define	__Q8_UL		(~0UL)
#define	__Q8_UQ		(~__Q8_2J(0U,__Q8_QS))
#define	__Q8_UM		(~__Q8_2J(0U,__Q8_MS))

/* bottom signed values */
#if	defined(__Q8_TC)
#define	__Q8_BI(w)	(-__Q8_TI(w)-1)
#define	__Q8_BL		(-__Q8_TL-1)
#define	__Q8_BQ		(-__Q8_TQ-1)
#define	__Q8_BM		(-__Q8_TM-1)
#else	/* defined(__Q8_OC) || defined(__Q8_SM) */
#define	__Q8_BI(w)	(-__Q8_TI(w))
#define	__Q8_BL		(-__Q8_TL)
#define	__Q8_BQ		(-__Q8_TQ)
#define	__Q8_BM		(-__Q8_TM)
#endif

#if	defined(__Q8_TEST)

/*	sanity checks	*/

#define	__Q8_X1		1
#define	__Q8_X2		2u
#if	__Q8_2J(__Q8_X1,__Q8_X2) != 12u
#error	"Q8defs.h: __Q8_2J macro is broken"
#endif

#if	__Q8_TI(__Q8_CW) <= 0 || __Q8_BI(__Q8_CW) >= 0 || \
	__Q8_TI(__Q8_SW) <= 0 || __Q8_BI(__Q8_SW) >= 0 || \
	__Q8_TI(__Q8_IW) <= 0 || __Q8_BI(__Q8_IW) >= 0 || \
	__Q8_TI(__Q8_CW) <= __Q8_BI(__Q8_CW) || \
	__Q8_TI(__Q8_SW) <= __Q8_BI(__Q8_SW) || \
	__Q8_TI(__Q8_IW) <= __Q8_BI(__Q8_IW)
#error	"Q8defs.h: __Q8_TI, __Q8_BI macros are broken"
#endif
#if	__Q8_UI(__Q8_CW) <= 0 || __Q8_UI(__Q8_CW) <= __Q8_TI(__Q8_CW) || \
	__Q8_UI(__Q8_SW) <= 0 || __Q8_UI(__Q8_SW) <= __Q8_TI(__Q8_SW) || \
	__Q8_UI(__Q8_IW) <= 0 || __Q8_UI(__Q8_IW) <= __Q8_TI(__Q8_IW)
#error	"Q8defs.h: __Q8_UI or __Q8_TI macro is broken"
#endif

#if	__Q8_TL <= 0 || __Q8_BL >= 0 || __Q8_TL <= __Q8_BL
#error	"Q8defs.h: __Q8_TL, __Q8_BL macros are broken"
#endif
#if	__Q8_UL <= 0 || __Q8_UL <= __Q8_TL
#error	"Q8defs.h: __Q8_UL or __Q8_TL macro is broken"
#endif

#ifdef	__Q8_QT
#if	__Q8_TQ <= 0 || __Q8_BQ >= 0 || __Q8_TQ <= __Q8_BQ
#error	"Q8defs.h: __Q8_TQ, __Q8_BQ macros are broken"
#endif
#if	__Q8_UQ <= 0 || __Q8_UQ <= __Q8_TQ
#if 0				/* XXX -- not working on Solaris */
#error	"Q8defs.h: __Q8_UQ or __Q8_TQ macro is broken"
#endif
#endif
#endif

#if	__Q8_TM <= 0 || __Q8_BM >= 0 || __Q8_TM <= __Q8_BM
#error	"Q8defs.h: __Q8_TM, __Q8_BM macros are broken"
#endif
#if	__Q8_UM <= 0 || __Q8_UM <= __Q8_TM
#if 0				/* XXX -- not working on Solaris */
#error	"Q8defs.h: __Q8_UM or __Q8_TM macro is broken"
#endif
#endif

#endif	/* defined(__Q8_TEST) */

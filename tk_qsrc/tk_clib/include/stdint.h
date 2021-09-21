/*
	stdint.h -- integer types

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.18 of ISO/IEC 9899:1999 (E) + TC1.

	This particular implementation assumes that widths of the standard
	integer types are powers of 2, and that there are no extended integer
	types with intermediate widths.

	This implementation is SELF-CONFIGURING, based on parameters defined
	in <Q8defs.h>.  If you plan to include this in a C implementation,
	it would be best to trim it down, with all the parameterized decisions
	already made.
*/

/* This header doesn't need an idempotency lock; there are no typedefs here. */

#ifndef STDINT_H
#define STDINT_H

#include	<Q8defs.h>		/* defines the __Q8_* symbols */

/* This header doesn't need C++ extern "C"; there are no declarations here. */

/*
	7.18.1.1	Exact-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.
*/

#if	__Q8_CW == 8 && defined(__Q8_SC)
typedef __Q8_SC			int8_t;
typedef unsigned char		uint8_t;
#endif
#if	__Q8_CW == 16 && defined(__Q8_SC)
typedef __Q8_SC			int16_t;
typedef unsigned char		uint16_t;
#elif	__Q8_SW == 16
typedef short			int16_t;
typedef unsigned short		uint16_t;
#endif
#if	__Q8_CW == 32 && defined(__Q8_SC)
typedef __Q8_SC			int32_t;
typedef unsigned char		uint32_t;
#elif	__Q8_SW == 32
typedef short			int32_t;
typedef unsigned short		uint32_t;
#elif	__Q8_IW == 32
typedef int			int32_t;
typedef unsigned int		uint32_t;
#elif	__Q8_LW == 32
typedef long			int32_t;
typedef unsigned long		uint32_t;
#endif
#if	__Q8_CW == 40 && defined(__Q8_SC)
typedef __Q8_SC			int40_t;
typedef unsigned char		uint40_t;
#elif	__Q8_SW == 40
typedef short			int40_t;
typedef unsigned short		uint40_t;
#elif	__Q8_IW == 40
typedef int			int40_t;
typedef unsigned int		uint40_t;
#elif	__Q8_LW == 40
typedef long			int40_t;
typedef unsigned long		uint40_t;
#endif

#if	__Q8_CW == 64 && defined(__Q8_SC)
typedef __Q8_SC			int64_t;
typedef unsigned char		uint64_t;
#elif	__Q8_SW == 64
typedef short			int64_t;
typedef unsigned short		uint64_t;
#elif	__Q8_IW == 64
typedef int			int64_t;
typedef unsigned int		uint64_t;
#elif	__Q8_LW == 64
typedef long			int64_t;
typedef unsigned long		uint64_t;
#elif	__Q8_QW == 64
typedef __Q8_QT			int64_t;
typedef unsigned __Q8_QT	uint64_t;
#endif

#if	__Q8_CW == 128 && defined(__Q8_SC)
typedef __Q8_SC			int128_t;
typedef unsigned char		uint128_t;
#elif	__Q8_SW == 128
typedef short				int128_t;
typedef unsigned short	uint128_t;
#elif	__Q8_IW == 128
typedef int				int128_t;
typedef unsigned int		uint128_t;
#elif	__Q8_LW == 128
typedef long				int128_t;
typedef unsigned long		uint128_t;
#elif	__Q8_QW == 128
typedef __Q8_QT			int128_t;
typedef unsigned __Q8_QT	uint128_t;
#elif	__Q8_OW == 128
typedef __Q8_OT			int128_t;
typedef unsigned __Q8_OT	uint128_t;
#endif

/*
	7.18.1.2	Minimum-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.

	Prefers type int over char or short char, due to a possible
	problem constructing suitable integer constants for 7.18.4.1.
*/

#if	__Q8_CW < __Q8_IW && defined(__Q8_SC)
typedef __Q8_SC			int_least8_t;
typedef unsigned char		uint_least8_t;
#elif	__Q8_SW < __Q8_IW
typedef short			int_least8_t;
typedef unsigned short		uint_least8_t;
#else
typedef int			int_least8_t;
typedef unsigned int		uint_least8_t;
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 16 && defined(__Q8_SC)
typedef __Q8_SC			int_least16_t;
typedef unsigned char		uint_least16_t;
#elif	__Q8_SW < __Q8_IW
typedef short			int_least16_t;
typedef unsigned short		uint_least16_t;
#else
typedef int			int_least16_t;
typedef unsigned int		uint_least16_t;
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 32 && defined(__Q8_SC)
typedef __Q8_SC			int_least32_t;
typedef unsigned char		uint_least32_t;
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 32
typedef short			int_least32_t;
typedef unsigned short		uint_least32_t;
#elif	__Q8_IW >= 32
typedef int			int_least32_t;
typedef unsigned int		uint_least32_t;
#else
typedef long			int_least32_t;
typedef unsigned long		uint_least32_t;
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 40 && defined(__Q8_SC)
typedef __Q8_SC			int_least40_t;
typedef unsigned char		uint_least40_t;
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 40
typedef short			int_least40_t;
typedef unsigned short		uint_least40_t;
#elif	__Q8_IW >= 40
typedef int			int_least40_t;
typedef unsigned int		uint_least40_t;
#elif	__Q8_LW >= 40
typedef long			int_least40_t;
typedef unsigned long		uint_least40_t;
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
typedef __Q8_QT			int_least40_t;
typedef unsigned __Q8_QT	uint_least40_t;
#endif

#if	__Q8_CW < __Q8_IW && __Q8_CW >= 64 && defined(__Q8_SC)
typedef __Q8_SC			int_least64_t;
typedef unsigned char		uint_least64_t;
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 64
typedef short			int_least64_t;
typedef unsigned short		uint_least64_t;
#elif	__Q8_IW >= 64
typedef int			int_least64_t;
typedef unsigned int		uint_least64_t;
#elif	__Q8_LW >= 64
typedef long			int_least64_t;
typedef unsigned long		uint_least64_t;
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
typedef __Q8_QT		int_least64_t;
typedef unsigned __Q8_QT	uint_least64_t;
#endif

#if	__Q8_CW < __Q8_IW && __Q8_CW >= 128 && defined(__Q8_SC)
typedef __Q8_SC			int_least128_t;
typedef unsigned char		uint_least128_t;
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 128
typedef short				int_least128_t;
typedef unsigned short	uint_least128_t;
#elif	__Q8_IW >= 128
typedef int				int_least128_t;
typedef unsigned int		uint_least128_t;
#elif	__Q8_LW >= 128
typedef long				int_least128_t;
typedef unsigned long		uint_least128_t;
#elif	__Q8_QW >= 128			/* (will be 0 if not defined) */
typedef __Q8_QT			int_least128_t;
typedef unsigned __Q8_QT	uint_least128_t;
#elif	__Q8_OW >= 128			/* (will be 0 if not defined) */
typedef __Q8_OT			int_least128_t;
typedef unsigned __Q8_OT	uint_least128_t;
#endif

/*
	7.18.1.3	Fastest minimum-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.

	"Fastest" is hard to determine; if "int" qualifies, it is chosen.
*/

typedef int			int_fast8_t;
typedef unsigned int		uint_fast8_t;
typedef int			int_fast16_t;
typedef unsigned int		uint_fast16_t;
#if	__Q8_IW >= 32
typedef int			int_fast32_t;
typedef unsigned int		uint_fast32_t;
#else
typedef long			int_fast32_t;
typedef unsigned long		uint_fast32_t;
#endif
#if	__Q8_IW >= 40
typedef int			int_fast40_t;
typedef unsigned int		uint_fast40_t;
#elif	__Q8_LW >= 40
typedef long			int_fast40_t;
typedef unsigned long		uint_fast40_t;
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
typedef __Q8_QT			int_fast40_t;
typedef unsigned __Q8_QT	uint_fast40_t;
#endif
#if	__Q8_IW >= 64
typedef int			int_fast64_t;
typedef unsigned int		uint_fast64_t;
#elif	__Q8_LW >= 64
typedef long			int_fast64_t;
typedef unsigned long		uint_fast64_t;
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
typedef __Q8_QT			int_fast64_t;
typedef unsigned __Q8_QT	uint_fast64_t;
#endif

/*
	7.18.1.4	Integer types capable of holding object pointers

	Theoretically, these tests might not work (e.g., the implementation
	can limit the interconvertible type to one that is not the smallest
	possible), but I'm unaware of any platform where they fail.
*/

#ifdef __BGBCC__

typedef __intptr		intptr_t;
typedef __uintptr		uintptr_t;

#else

#if	defined(__Q8_PW)
#if	__Q8_IW >= __Q8_PW
typedef int			intptr_t;
typedef unsigned int		uintptr_t;
#elif	__Q8_LW >= __Q8_PW
typedef long			intptr_t;
typedef unsigned long		uintptr_t;
#elif	__Q8_QW >= __Q8_PW
typedef __Q8_QT			intptr_t;
typedef unsigned __Q8_QT	uintptr_t;
#elif	__Q8_MW >= __Q8_PW
typedef __Q8_MT			intptr_t;
typedef unsigned __Q8_MT	uintptr_t;
#endif
#endif

#endif

/*
	7.18.1.5	Greatest-width integer types
*/

typedef __Q8_MT			intmax_t;
typedef unsigned __Q8_MT	uintmax_t;

#if	!defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/*
	7.18.2.1	Limits of exact-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.
*/

#if	__Q8_CW == 8 && defined(__Q8_SC)
#define	INT8_MAX		__Q8_TI(__Q8_CW)
#define	INT8_MIN		__Q8_BI(__Q8_CW)
#define	UINT8_MAX		__Q8_UI(__Q8_CW)
#endif
#if	__Q8_CW == 16 && defined(__Q8_SC)
#define	INT16_MAX		__Q8_TI(__Q8_CW)
#define	INT16_MIN		__Q8_BI(__Q8_CW)
#define	UINT16_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW == 16
#define	INT16_MAX		__Q8_TI(__Q8_SW)
#define	INT16_MIN		__Q8_BI(__Q8_SW)
#define	UINT16_MAX		__Q8_UI(__Q8_SW)
#endif
#if	__Q8_CW == 32 && defined(__Q8_SC)
#define	INT32_MAX		__Q8_TI(__Q8_CW)
#define	INT32_MIN		__Q8_BI(__Q8_CW)
#define	UINT32_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW == 32
#define	INT32_MAX		__Q8_TI(__Q8_SW)
#define	INT32_MIN		__Q8_BI(__Q8_SW)
#define	UINT32_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW == 32
#define	INT32_MAX		__Q8_TI(__Q8_IW)
#define	INT32_MIN		__Q8_BI(__Q8_IW)
#define	UINT32_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW == 32
#define	INT32_MAX		__Q8_TL
#define	INT32_MIN		__Q8_BL
#define	UINT32_MAX		__Q8_UL
#endif
#if	__Q8_CW == 40 && defined(__Q8_SC)
#define	INT40_MAX		__Q8_TI(__Q8_CW)
#define	INT40_MIN		__Q8_BI(__Q8_CW)
#define	UINT40_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW == 40
#define	INT40_MAX		__Q8_TI(__Q8_SW)
#define	INT40_MIN		__Q8_BI(__Q8_SW)
#define	UINT40_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW == 40
#define	INT40_MAX		__Q8_TI(__Q8_IW)
#define	INT40_MIN		__Q8_BI(__Q8_IW)
#define	UINT40_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW == 40
#define	INT40_MAX		__Q8_TL
#define	INT40_MIN		__Q8_BL
#define	UINT40_MAX		__Q8_UL
#endif
#if	__Q8_CW == 64 && defined(__Q8_SC)
#define	INT64_MAX		__Q8_TI(__Q8_CW)
#define	INT64_MIN		__Q8_BI(__Q8_CW)
#define	UINT64_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW == 64
#define	INT64_MAX		__Q8_TI(__Q8_SW)
#define	INT64_MIN		__Q8_BI(__Q8_SW)
#define	UINT64_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW == 64
#define	INT64_MAX		__Q8_TI(__Q8_IW)
#define	INT64_MIN		__Q8_BI(__Q8_IW)
#define	UINT64_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW == 64
#define	INT64_MAX		__Q8_TL
#define	INT64_MIN		__Q8_BL
#define	UINT64_MAX		__Q8_UL
#elif	__Q8_QW == 64
#define	INT64_MAX		__Q8_TQ
#define	INT64_MIN		__Q8_BQ
#define	UINT64_MAX		__Q8_UQ
#endif

/*
	7.18.2.2	Limits of minimum-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.
*/
#if	__Q8_CW < __Q8_IW && defined(__Q8_SC)
#define	INT_LEAST8_MAX		__Q8_TI(__Q8_CW)
#define	INT_LEAST8_MIN		__Q8_BI(__Q8_CW)
#define	UINT_LEAST8_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW < __Q8_IW
#define	INT_LEAST8_MAX		__Q8_TI(__Q8_SW)
#define	INT_LEAST8_MIN		__Q8_BI(__Q8_SW)
#define	UINT_LEAST8_MAX		__Q8_UI(__Q8_SW)
#else
#define	INT_LEAST8_MAX		__Q8_TI(__Q8_IW)
#define	INT_LEAST8_MIN		__Q8_BI(__Q8_IW)
#define	UINT_LEAST8_MAX		__Q8_UI(__Q8_IW)
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 16 && defined(__Q8_SC)
#define	INT_LEAST16_MAX		__Q8_TI(__Q8_CW)
#define	INT_LEAST16_MIN		__Q8_BI(__Q8_CW)
#define	UINT_LEAST16_MAX	__Q8_UI(__Q8_CW)
#elif	__Q8_SW < __Q8_IW
#define	INT_LEAST16_MAX		__Q8_TI(__Q8_SW)
#define	INT_LEAST16_MIN		__Q8_BI(__Q8_SW)
#define	UINT_LEAST16_MAX	__Q8_UI(__Q8_SW)
#else
#define	INT_LEAST16_MAX		__Q8_TI(__Q8_IW)
#define	INT_LEAST16_MIN		__Q8_BI(__Q8_IW)
#define	UINT_LEAST16_MAX	__Q8_UI(__Q8_IW)
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 32 && defined(__Q8_SC)
#define	INT_LEAST32_MAX		__Q8_TI(__Q8_CW)
#define	INT_LEAST32_MIN		__Q8_BI(__Q8_CW)
#define	UINT_LEAST32_MAX	__Q8_UI(__Q8_CW)
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 32
#define	INT_LEAST32_MAX		__Q8_TI(__Q8_SW)
#define	INT_LEAST32_MIN		__Q8_BI(__Q8_SW)
#define	UINT_LEAST32_MAX	__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= 32
#define	INT_LEAST32_MAX		__Q8_TI(__Q8_IW)
#define	INT_LEAST32_MIN		__Q8_BI(__Q8_IW)
#define	UINT_LEAST32_MAX	__Q8_UI(__Q8_IW)
#else
#define	INT_LEAST32_MAX		__Q8_TL
#define	INT_LEAST32_MIN		__Q8_BL
#define	UINT_LEAST32_MAX	__Q8_UL
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 40 && defined(__Q8_SC)
#define	INT_LEAST40_MAX		__Q8_TI(__Q8_CW)
#define	INT_LEAST40_MIN		__Q8_BI(__Q8_CW)
#define	UINT_LEAST40_MAX	__Q8_UI(__Q8_CW)
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 40
#define	INT_LEAST40_MAX		__Q8_TI(__Q8_SW)
#define	INT_LEAST40_MIN		__Q8_BI(__Q8_SW)
#define	UINT_LEAST40_MAX	__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= 40
#define	INT_LEAST40_MAX		__Q8_TI(__Q8_IW)
#define	INT_LEAST40_MIN		__Q8_BI(__Q8_IW)
#define	UINT_LEAST40_MAX	__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= 40
#define	INT_LEAST40_MAX		__Q8_TL
#define	INT_LEAST40_MIN		__Q8_BL
#define	UINT_LEAST40_MAX	__Q8_UL
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
#define	INT_LEAST40_MAX		__Q8_TQ
#define	INT_LEAST40_MIN		__Q8_BQ
#define	UINT_LEAST40_MAX	__Q8_UQ
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 64 && defined(__Q8_SC)
#define	INT_LEAST64_MAX		__Q8_TI(__Q8_CW)
#define	INT_LEAST64_MIN		__Q8_BI(__Q8_CW)
#define	UINT_LEAST64_MAX	__Q8_UI(__Q8_CW)
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 64
#define	INT_LEAST64_MAX		__Q8_TI(__Q8_SW)
#define	INT_LEAST64_MIN		__Q8_BI(__Q8_SW)
#define	UINT_LEAST64_MAX	__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= 64
#define	INT_LEAST64_MAX		__Q8_TI(__Q8_IW)
#define	INT_LEAST64_MIN		__Q8_BI(__Q8_IW)
#define	UINT_LEAST64_MAX	__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= 64
#define	INT_LEAST64_MAX		__Q8_TL
#define	INT_LEAST64_MIN		__Q8_BL
#define	UINT_LEAST64_MAX	__Q8_UL
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
#define	INT_LEAST64_MAX		__Q8_TQ
#define	INT_LEAST64_MIN		__Q8_BQ
#define	UINT_LEAST64_MAX	__Q8_UQ
#endif

/*
	7.18.2.3	Limits of fastest minimum-width integer types

	The optional 40-bit type is supported by the TMS320C6xxx DSP.
*/

#define	INT_FAST8_MAX		__Q8_TI(__Q8_IW)
#define	INT_FAST8_MIN		__Q8_BI(__Q8_IW)
#define	UINT_FAST8_MAX		__Q8_UI(__Q8_IW)
#define	INT_FAST16_MAX		__Q8_TI(__Q8_IW)
#define	INT_FAST16_MIN		__Q8_BI(__Q8_IW)
#define	UINT_FAST16_MAX		__Q8_UI(__Q8_IW)
#if	__Q8_IW >= 32
#define	INT_FAST32_MAX		__Q8_TI(__Q8_IW)
#define	INT_FAST32_MIN		__Q8_BI(__Q8_IW)
#define	UINT_FAST32_MAX		__Q8_UI(__Q8_IW)
#else
#define	INT_FAST32_MAX		__Q8_TL
#define	INT_FAST32_MIN		__Q8_BL
#define	UINT_FAST32_MAX		__Q8_UL
#endif
#if	__Q8_IW >= 40
#define	INT_FAST40_MAX		__Q8_TI(__Q8_IW)
#define	INT_FAST40_MIN		__Q8_BI(__Q8_IW)
#define	UINT_FAST40_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= 40
#define	INT_FAST40_MAX		__Q8_TL
#define	INT_FAST40_MIN		__Q8_BL
#define	UINT_FAST40_MAX		__Q8_UL
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
#define	INT_FAST40_MAX		__Q8_TQ
#define	INT_FAST40_MIN		__Q8_BQ
#define	UINT_FAST40_MAX		__Q8_UQ
#endif
#if	__Q8_IW >= 64
#define	INT_FAST64_MAX		__Q8_TI(__Q8_IW)
#define	INT_FAST64_MIN		__Q8_BI(__Q8_IW)
#define	UINT_FAST64_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= 64
#define	INT_FAST64_MAX		__Q8_TL
#define	INT_FAST64_MIN		__Q8_BL
#define	UINT_FAST64_MAX		__Q8_UL
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
#define	INT_FAST64_MAX		__Q8_TQ
#define	INT_FAST64_MIN		__Q8_BQ
#define	UINT_FAST64_MAX		__Q8_UQ
#endif

/*
	7.18.2.4	Limits of integer types
				capable of holding object pointers
*/

#if	defined(__Q8_PW)
#if	__Q8_IW >= __Q8_PW
#define	INTPTR_MAX		__Q8_TI(__Q8_IW)
#define	INTPTR_MIN		__Q8_BI(__Q8_IW)
#define	UINTPTR_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= __Q8_PW
#define	INTPTR_MAX		__Q8_TL
#define	INTPTR_MIN		__Q8_BL
#define	UINTPTR_MAX		__Q8_UL
#elif	__Q8_QW >= __Q8_PW
#define	INTPTR_MAX		__Q8_TQ
#define	INTPTR_MIN		__Q8_BQ
#define	UINTPTR_MAX		__Q8_UQ
#elif	__Q8_MW >= __Q8_PW
#define	INTPTR_MAX		__Q8_TM
#define	INTPTR_MIN		__Q8_BM
#define	UINTPTR_MAX		__Q8_UM
#endif
#endif

/*
	7.18.2.5	Limits of greatest-width integer types
*/

#define	INTMAX_MAX		__Q8_TM
#define	INTMAX_MIN		__Q8_BM
#define	UINTMAX_MAX		__Q8_UM

/*
	7.18.3		Limits of other integer types

	XXX --	These ought to be moved into <Q8defs.h> along with
		the corresponding type definitions.
*/

#if	__Q8_CW >= __Q8_DW && defined(__Q8_SC)
#define	PTRDIFF_MAX		__Q8_TI(__Q8_CW)
#define	PTRDIFF_MIN		__Q8_BI(__Q8_CW)
#elif	__Q8_SW >= __Q8_DW
#define	PTRDIFF_MAX		__Q8_TI(__Q8_SW)
#define	PTRDIFF_MIN		__Q8_BI(__Q8_SW)
#elif	__Q8_IW >= __Q8_DW
#define	PTRDIFF_MAX		__Q8_TI(__Q8_IW)
#define	PTRDIFF_MIN		__Q8_BI(__Q8_IW)
#elif	__Q8_LW >= __Q8_DW
#define	PTRDIFF_MAX		__Q8_TL
#define	PTRDIFF_MIN		__Q8_BL
#elif	__Q8_QW >= __Q8_DW
#define	PTRDIFF_MAX		__Q8_TQ
#define	PTRDIFF_MIN		__Q8_BQ
#else
#define	PTRDIFF_MAX		__Q8_TM
#define	PTRDIFF_MIN		__Q8_BM
#endif

#ifdef	__Q8_AU

#if	__Q8_CW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_UL
#elif	__Q8_QW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_UQ
#else
#define	SIG_ATOMIC_MAX		__Q8_UM
#endif

#define	SIG_ATOMIC_MIN		0

#else	/* !defined(__Q8_AU) */

#if	__Q8_CW >= __Q8_AW && defined(__Q8_SC)
#define	SIG_ATOMIC_MAX		__Q8_TI(__Q8_CW)
#define	SIG_ATOMIC_MIN		__Q8_BI(__Q8_CW)
#elif	__Q8_SW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_TI(__Q8_SW)
#define	SIG_ATOMIC_MIN		__Q8_BI(__Q8_SW)
#elif	__Q8_IW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_TI(__Q8_IW)
#define	SIG_ATOMIC_MIN		__Q8_BI(__Q8_IW)
#elif	__Q8_LW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_TL
#define	SIG_ATOMIC_MIN		__Q8_BL
#elif	__Q8_QW >= __Q8_AW
#define	SIG_ATOMIC_MAX		__Q8_TQ
#define	SIG_ATOMIC_MIN		__Q8_BQ
#else
#define	SIG_ATOMIC_MAX		__Q8_TM
#define	SIG_ATOMIC_MIN		__Q8_BM
#endif

#endif	/* defined(__Q8_AU) */

#if	__Q8_CW >= __Q8_ZW
#define	SIZE_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW >= __Q8_ZW
#define	SIZE_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= __Q8_ZW
#define	SIZE_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= __Q8_ZW
#define	SIZE_MAX		__Q8_UL
#elif	__Q8_QW >= __Q8_ZW
#define	SIZE_MAX		__Q8_UQ
#else
#define	SIZE_MAX		__Q8_UM
#endif

#ifdef	__Q8_WU

#if	__Q8_CW >= __Q8_WW
#define	WCHAR_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW >= __Q8_WW
#define	WCHAR_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= __Q8_WW
#define	WCHAR_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= __Q8_WW
#define	WCHAR_MAX		__Q8_UL
#elif	__Q8_QW >= __Q8_WW
#define	WCHAR_MAX		__Q8_UQ
#else
#define	WCHAR_MAX		__Q8_UM
#endif

#define	WCHAR_MIN		0

#else	/* !defined(__Q8_WU) */

#if	__Q8_CW >= __Q8_WW && defined(__Q8_SC)
#define	WCHAR_MAX		__Q8_TI(__Q8_CW)
#define	WCHAR_MIN		__Q8_BI(__Q8_CW)
#elif	__Q8_SW >= __Q8_WW
#define	WCHAR_MAX		__Q8_TI(__Q8_SW)
#define	WCHAR_MIN		__Q8_BI(__Q8_SW)
#elif	__Q8_IW >= __Q8_WW
#define	WCHAR_MAX		__Q8_TI(__Q8_IW)
#define	WCHAR_MIN		__Q8_BI(__Q8_IW)
#elif	__Q8_LW >= __Q8_WW
#define	WCHAR_MAX		__Q8_TL
#define	WCHAR_MIN		__Q8_BL
#elif	__Q8_QW >= __Q8_WW
#define	WCHAR_MAX		__Q8_TQ
#define	WCHAR_MIN		__Q8_BQ
#else
#define	WCHAR_MAX		__Q8_TM
#define	WCHAR_MIN		__Q8_BM
#endif

#endif	/* defined(__Q8_WU) */

#ifdef	__Q8_XU

#if	__Q8_CW >= __Q8_XW
#define	WINT_MAX		__Q8_UI(__Q8_CW)
#elif	__Q8_SW >= __Q8_XW
#define	WINT_MAX		__Q8_UI(__Q8_SW)
#elif	__Q8_IW >= __Q8_XW
#define	WINT_MAX		__Q8_UI(__Q8_IW)
#elif	__Q8_LW >= __Q8_XW
#define	WINT_MAX		__Q8_UL
#elif	__Q8_QW >= __Q8_XW
#define	WINT_MAX		__Q8_UQ
#else
#define	WINT_MAX		__Q8_UM
#endif

#define	WINT_MIN		0

#else	/* !defined(__Q8_XU) */

#if	__Q8_CW >= __Q8_XW && defined(__Q8_SC)
#define	WINT_MAX		__Q8_TI(__Q8_CW)
#define	WINT_MIN		__Q8_BI(__Q8_CW)
#elif	__Q8_SW >= __Q8_XW
#define	WINT_MAX		__Q8_TI(__Q8_SW)
#define	WINT_MIN		__Q8_BI(__Q8_SW)
#elif	__Q8_IW >= __Q8_XW
#define	WINT_MAX		__Q8_TI(__Q8_IW)
#define	WINT_MIN		__Q8_BI(__Q8_IW)
#elif	__Q8_LW >= __Q8_XW
#define	WINT_MAX		__Q8_TL
#define	WINT_MIN		__Q8_BL
#elif	__Q8_QW >= __Q8_XW
#define	WINT_MAX		__Q8_TQ
#define	WINT_MIN		__Q8_BQ
#else
#define	WINT_MAX		__Q8_TM
#define	WINT_MIN		__Q8_BM
#endif

#endif	/* defined(__Q8_XU) */

#endif	/* !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */

#if	!defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

/*
	7.18.4.1	Macros for minimum-width integer constants

	The optional 40-bit type is supported by the TMS320C6xxx DSP.

	This spec was changed in ISO/IEC 9899:1999 TC1; in ISO/IEC
	9899:1999 as initially published, the expansion was required
	to be an integer constant of precisely matching type, which
	is impossible to accomplish for the shorter types on most
	platforms, because C99 provides no standard way to designate
	an integer constant with width less than that of type int.
	TC1 changed this to require just an integer constant
	*expression* with *promoted* type.

	The trick used to get the right type is due to Clive Feather.
*/

#define	INT8_C(c)	(INT_LEAST8_MAX-INT_LEAST8_MAX+(c))
#define	UINT8_C(c)	(UINT_LEAST8_MAX-UINT_LEAST8_MAX+(c))
#define	INT16_C(c)	(INT_LEAST16_MAX-INT_LEAST16_MAX+(c))
#define	UINT16_C(c)	(UINT_LEAST16_MAX-UINT_LEAST16_MAX+(c))
#define	INT32_C(c)	(INT_LEAST32_MAX-INT_LEAST32_MAX+(c))
#define	UINT32_C(c)	(UINT_LEAST32_MAX-UINT_LEAST32_MAX+(c))
#ifdef	INT_LEAST40_MAX
#define	INT40_C(c)	(INT_LEAST40_MAX-INT_LEAST40_MAX+(c))
#define	UINT40_C(c)	(UINT_LEAST40_MAX-UINT_LEAST40_MAX+(c))
#endif
#ifdef	INT_LEAST64_MAX
#define	INT64_C(c)	(INT_LEAST64_MAX-INT_LEAST64_MAX+(c))
#define	UINT64_C(c)	(UINT_LEAST64_MAX-UINT_LEAST64_MAX+(c))
#endif

/*
	7.18.4.2	Macros for greatest-width integer constants
*/

#define	INTMAX_C(c)	(INTMAX_MAX-INTMAX_MAX+(c))
#define	UINTMAX_C(c)	(UINTMAX_MAX-UINTMAX_MAX+(c))

#endif	/* !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS) */

#endif

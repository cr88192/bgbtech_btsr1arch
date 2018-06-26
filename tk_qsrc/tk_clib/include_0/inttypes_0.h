/*
	inttypes.h -- format conversion of integer types

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.8 of ISO/IEC 9899:1999 (E).

	This particular implementation requires the matching <stdint.h>
	and inttypes.c.

	This implementation is SELF-CONFIGURING, based on parameters defined
	in <stdint.h>.  If you plan to include it in a C implementation,
	it would be best to trim it down, with all the parameterized actions
	already performed.
*/

#if	!defined(_INTTYPES_H) && !defined(_INC_INTTYPES)	/* usual */
#define	_INTTYPES_H			/* idempotency lock (section 7.1.2) */
#define	_INC_INTTYPES

#include	<stdint.h>		/* defines various __Q8_* symbols */

#ifdef	__cplusplus
extern	"C"	{
#endif

/*
	type defined by this header (only)
*/

typedef struct
	{
	intmax_t	quot;		/* member order must match function */
	intmax_t	rem;
	}			imaxdiv_t;

#if	!defined(__cplusplus) || defined(__STDC_FORMAT_MACROS)

/* figure out the type of wchar_t */
#ifdef	__Q8_WU

#if	__Q8_CW >= __Q8_WW
#define	__Q8_WT		unsigned char
#elif	__Q8_SW >= __Q8_WW
#define	__Q8_WT		unsigned short
#elif	__Q8_IW >= __Q8_WW
#define	__Q8_WT		unsigned int
#elif	__Q8_LW >= __Q8_WW
#define	__Q8_WT		unsigned long
#elif	__Q8_QW >= __Q8_WW
#define	__Q8_WT		unsigned __Q8_QT
#else
#define	__Q8_WT		unsigned __Q8_MT
#endif

#define	WCHAR_MIN		0

#else	/* !defined(__Q8_WU) */

#if	__Q8_CW >= __Q8_WW && defined(__Q8_SC)
#define	__Q8_WT		__Q8_SC
#elif	__Q8_SW >= __Q8_WW
#define	__Q8_WT		short
#elif	__Q8_IW >= __Q8_WW
#define	__Q8_WT		int
#elif	__Q8_LW >= __Q8_WW
#define	__Q8_WT		long
#elif	__Q8_QW >= __Q8_WW
#define	__Q8_WT		__Q8_QT
#else
#define	__Q8_WT		__Q8_MT
#endif

#endif	/* defined(__Q8_WU) */

/*
	7.8.1	Macros for format specifiers

	The optional 40-bit type is supported by the TMS320C6xxx DSP.

	Note that there was no length modifier for char type prior to C99.
*/

#if	__Q8_CW == 8 && defined(__Q8_SC)
#define	PRId8			"d"
#define	PRIi8			"i"
#define	PRIo8			"o"
#define	PRIu8			"u"
#define	PRIx8			"x"
#define	PRIX8			"X"
#ifdef	__Q8_CF
#define	SCNd8			__Q8_CF"d"
#define	SCNi8			__Q8_CF"i"
#define	SCNo8			__Q8_CF"o"
#define	SCNu8			__Q8_CF"u"
#define	SCNx8			__Q8_CF"x"
#endif
#endif
#if	__Q8_CW == 16 && defined(__Q8_SC)
#define	PRId16			"d"
#define	PRIi16			"i"
#define	PRIo16			"o"
#define	PRIu16			"u"
#define	PRIx16			"x"
#define	PRIX16			"X"
#ifdef	__Q8_CF
#define	SCNd16			__Q8_CF"d"
#define	SCNi16			__Q8_CF"i"
#define	SCNo16			__Q8_CF"o"
#define	SCNu16			__Q8_CF"u"
#define	SCNx16			__Q8_CF"x"
#endif
#elif	__Q8_SW == 16
#define	PRId16			"d"
#define	PRIi16			"i"
#define	PRIo16			"o"
#define	PRIu16			"u"
#define	PRIx16			"x"
#define	PRIX16			"X"
#define	SCNd16			"hd"
#define	SCNi16			"hi"
#define	SCNo16			"ho"
#define	SCNu16			"hu"
#define	SCNx16			"hx"
#endif
#if	__Q8_CW == 32 && defined(__Q8_SC)
#define	PRId32			"d"
#define	PRIi32			"i"
#define	PRIo32			"o"
#define	PRIu32			"u"
#define	PRIx32			"x"
#define	PRIX32			"X"
#ifdef	__Q8_CF
#define	SCNd32			__Q8_CF"d"
#define	SCNi32			__Q8_CF"i"
#define	SCNo32			__Q8_CF"o"
#define	SCNu32			__Q8_CF"u"
#define	SCNx32			__Q8_CF"x"
#endif
#elif	__Q8_SW == 32
#define	PRId32			"d"
#define	PRIi32			"i"
#define	PRIo32			"o"
#define	PRIu32			"u"
#define	PRIx32			"x"
#define	PRIX32			"X"
#define	SCNd32			"hd"
#define	SCNi32			"hi"
#define	SCNo32			"ho"
#define	SCNu32			"hu"
#define	SCNx32			"hx"
#elif	__Q8_IW == 32
#define	PRId32			"d"
#define	PRIi32			"i"
#define	PRIo32			"o"
#define	PRIu32			"u"
#define	PRIx32			"x"
#define	PRIX32			"X"
#define	SCNd32			"d"
#define	SCNi32			"i"
#define	SCNo32			"o"
#define	SCNu32			"u"
#define	SCNx32			"x"
#elif	__Q8_LW == 32
#define	PRId32			"ld"
#define	PRIi32			"li"
#define	PRIo32			"lo"
#define	PRIu32			"lu"
#define	PRIx32			"lx"
#define	PRIX32			"lX"
#define	SCNd32			"ld"
#define	SCNi32			"li"
#define	SCNo32			"lo"
#define	SCNu32			"lu"
#define	SCNx32			"lx"
#endif
#if	__Q8_CW == 40 && defined(__Q8_SC)
#define	PRId40			"d"
#define	PRIi40			"i"
#define	PRIo40			"o"
#define	PRIu40			"u"
#define	PRIx40			"x"
#define	PRIX40			"X"
#ifdef	__Q8_CF
#define	SCNd40			__Q8_CF"d"
#define	SCNi40			__Q8_CF"i"
#define	SCNo40			__Q8_CF"o"
#define	SCNu40			__Q8_CF"u"
#define	SCNx40			__Q8_CF"x"
#endif
#elif	__Q8_SW == 40
#define	PRId40			"d"
#define	PRIi40			"i"
#define	PRIo40			"o"
#define	PRIu40			"u"
#define	PRIx40			"x"
#define	PRIX40			"X"
#define	SCNd40			"hd"
#define	SCNi40			"hi"
#define	SCNo40			"ho"
#define	SCNu40			"hu"
#define	SCNx40			"hx"
#elif	__Q8_IW == 40
#define	PRId40			"d"
#define	PRIi40			"i"
#define	PRIo40			"o"
#define	PRIu40			"u"
#define	PRIx40			"x"
#define	PRIX40			"X"
#define	SCNd40			"d"
#define	SCNi40			"i"
#define	SCNo40			"o"
#define	SCNu40			"u"
#define	SCNx40			"x"
#elif	__Q8_LW == 40
#define	PRId40			"ld"
#define	PRIi40			"li"
#define	PRIo40			"lo"
#define	PRIu40			"lu"
#define	PRIx40			"lx"
#define	PRIX40			"lX"
#define	SCNd40			"ld"
#define	SCNi40			"li"
#define	SCNo40			"lo"
#define	SCNu40			"lu"
#define	SCNx40			"lx"
#endif
#if	__Q8_CW == 64 && defined(__Q8_SC)
#define	PRId64			"d"
#define	PRIi64			"i"
#define	PRIo64			"o"
#define	PRIu64			"u"
#define	PRIx64			"x"
#define	PRIX64			"X"
#ifdef	__Q8_CF
#define	SCNd64			__Q8_CF"d"
#define	SCNi64			__Q8_CF"i"
#define	SCNo64			__Q8_CF"o"
#define	SCNu64			__Q8_CF"u"
#define	SCNx64			__Q8_CF"x"
#endif
#elif	__Q8_SW == 64
#define	PRId64			"d"
#define	PRIi64			"i"
#define	PRIo64			"o"
#define	PRIu64			"u"
#define	PRIx64			"x"
#define	PRIX64			"X"
#define	SCNd64			"hd"
#define	SCNi64			"hi"
#define	SCNo64			"ho"
#define	SCNu64			"hu"
#define	SCNx64			"hx"
#elif	__Q8_IW == 64
#define	PRId64			"d"
#define	PRIi64			"i"
#define	PRIo64			"o"
#define	PRIu64			"u"
#define	PRIx64			"x"
#define	PRIX64			"X"
#define	SCNd64			"d"
#define	SCNi64			"i"
#define	SCNo64			"o"
#define	SCNu64			"u"
#define	SCNx64			"x"
#elif	__Q8_LW == 64
#define	PRId64			"ld"
#define	PRIi64			"li"
#define	PRIo64			"lo"
#define	PRIu64			"lu"
#define	PRIx64			"lx"
#define	PRIX64			"lX"
#define	SCNd64			"ld"
#define	SCNi64			"li"
#define	SCNo64			"lo"
#define	SCNu64			"lu"
#define	SCNx64			"lx"
#elif	__Q8_QW == 64
#define	PRId64			__Q8_QF"d"
#define	PRIi64			__Q8_QF"i"
#define	PRIo64			__Q8_QF"o"
#define	PRIu64			__Q8_QF"u"
#define	PRIx64			__Q8_QF"x"
#define	PRIX64			__Q8_QF"X"
#define	SCNd64			__Q8_QF"d"
#define	SCNi64			__Q8_QF"i"
#define	SCNo64			__Q8_QF"o"
#define	SCNu64			__Q8_QF"u"
#define	SCNx64			__Q8_QF"x"
#endif

#if	__Q8_CW < __Q8_IW && defined(__Q8_SC)
#define	PRIdLEAST8		"d"
#define	PRIiLEAST8		"i"
#define	PRIoLEAST8		"o"
#define	PRIuLEAST8		"u"
#define	PRIxLEAST8		"x"
#define	PRIXLEAST8		"X"
#ifdef	__Q8_CF
#define	SCNdLEAST8		__Q8_CF"d"
#define	SCNiLEAST8		__Q8_CF"i"
#define	SCNoLEAST8		__Q8_CF"o"
#define	SCNuLEAST8		__Q8_CF"u"
#define	SCNxLEAST8		__Q8_CF"x"
#endif
#elif	__Q8_SW < __Q8_IW
#define	PRIdLEAST8		"d"
#define	PRIiLEAST8		"i"
#define	PRIoLEAST8		"o"
#define	PRIuLEAST8		"u"
#define	PRIxLEAST8		"x"
#define	PRIXLEAST8		"X"
#define	SCNdLEAST8		"hd"
#define	SCNiLEAST8		"hi"
#define	SCNoLEAST8		"ho"
#define	SCNuLEAST8		"hu"
#define	SCNxLEAST8		"hx"
#else
#define	PRIdLEAST8		"d"
#define	PRIiLEAST8		"i"
#define	PRIoLEAST8		"o"
#define	PRIuLEAST8		"u"
#define	PRIxLEAST8		"x"
#define	PRIXLEAST8		"X"
#define	SCNdLEAST8		"d"
#define	SCNiLEAST8		"i"
#define	SCNoLEAST8		"o"
#define	SCNuLEAST8		"u"
#define	SCNxLEAST8		"x"
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 16 && defined(__Q8_SC)
#define	PRIdLEAST16		"d"
#define	PRIiLEAST16		"i"
#define	PRIoLEAST16		"o"
#define	PRIuLEAST16		"u"
#define	PRIxLEAST16		"x"
#define	PRIXLEAST16		"X"
#ifdef	__Q8_CF
#define	SCNdLEAST16		__Q8_CF"d"
#define	SCNiLEAST16		__Q8_CF"i"
#define	SCNoLEAST16		__Q8_CF"o"
#define	SCNuLEAST16		__Q8_CF"u"
#define	SCNxLEAST16		__Q8_CF"x"
#endif
#elif	__Q8_SW < __Q8_IW
#define	PRIdLEAST16		"d"
#define	PRIiLEAST16		"i"
#define	PRIoLEAST16		"o"
#define	PRIuLEAST16		"u"
#define	PRIxLEAST16		"x"
#define	PRIXLEAST16		"X"
#define	SCNdLEAST16		"hd"
#define	SCNiLEAST16		"hi"
#define	SCNoLEAST16		"ho"
#define	SCNuLEAST16		"hu"
#define	SCNxLEAST16		"hx"
#else
#define	PRIdLEAST16		"d"
#define	PRIiLEAST16		"i"
#define	PRIoLEAST16		"o"
#define	PRIuLEAST16		"u"
#define	PRIxLEAST16		"x"
#define	PRIXLEAST16		"X"
#define	SCNdLEAST16		"d"
#define	SCNiLEAST16		"i"
#define	SCNoLEAST16		"o"
#define	SCNuLEAST16		"u"
#define	SCNxLEAST16		"x"
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 32 && defined(__Q8_SC)
#define	PRIdLEAST32		"d"
#define	PRIiLEAST32		"i"
#define	PRIoLEAST32		"o"
#define	PRIuLEAST32		"u"
#define	PRIxLEAST32		"x"
#define	PRIXLEAST32		"X"
#ifdef	__Q8_CF
#define	SCNdLEAST32		__Q8_CF"d"
#define	SCNiLEAST32		__Q8_CF"i"
#define	SCNoLEAST32		__Q8_CF"o"
#define	SCNuLEAST32		__Q8_CF"u"
#define	SCNxLEAST32		__Q8_CF"x"
#endif
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 32
#define	PRIdLEAST32		"d"
#define	PRIiLEAST32		"i"
#define	PRIoLEAST32		"o"
#define	PRIuLEAST32		"u"
#define	PRIxLEAST32		"x"
#define	PRIXLEAST32		"X"
#define	SCNdLEAST32		"hd"
#define	SCNiLEAST32		"hi"
#define	SCNoLEAST32		"ho"
#define	SCNuLEAST32		"hu"
#define	SCNx32			"hx"
#elif	__Q8_IW >= 32
#define	PRIdLEAST32		"d"
#define	PRIiLEAST32		"i"
#define	PRIoLEAST32		"o"
#define	PRIuLEAST32		"u"
#define	PRIxLEAST32		"x"
#define	PRIXLEAST32		"X"
#define	SCNdLEAST32		"d"
#define	SCNiLEAST32		"i"
#define	SCNoLEAST32		"o"
#define	SCNuLEAST32		"u"
#define	SCNxLEAST32		"x"
#else
#define	PRIdLEAST32		"ld"
#define	PRIiLEAST32		"li"
#define	PRIoLEAST32		"lo"
#define	PRIuLEAST32		"lu"
#define	PRIxLEAST32		"lx"
#define	PRIXLEAST32		"lX"
#define	SCNdLEAST32		"ld"
#define	SCNiLEAST32		"li"
#define	SCNoLEAST32		"lo"
#define	SCNuLEAST32		"lu"
#define	SCNxLEAST32		"lx"
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 40 && defined(__Q8_SC)
#define	PRIdLEAST40		"d"
#define	PRIiLEAST40		"i"
#define	PRIoLEAST40		"o"
#define	PRIuLEAST40		"u"
#define	PRIxLEAST40		"x"
#define	PRIXLEAST40		"X"
#ifdef	__Q8_CF
#define	SCNdLEAST40		__Q8_CF"d"
#define	SCNiLEAST40		__Q8_CF"i"
#define	SCNoLEAST40		__Q8_CF"o"
#define	SCNuLEAST40		__Q8_CF"u"
#define	SCNxLEAST40		__Q8_CF"x"
#endif
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 40
#define	PRIdLEAST40		"d"
#define	PRIiLEAST40		"i"
#define	PRIoLEAST40		"o"
#define	PRIuLEAST40		"u"
#define	PRIxLEAST40		"x"
#define	PRIXLEAST40		"X"
#define	SCNdLEAST40		"hd"
#define	SCNiLEAST40		"hi"
#define	SCNoLEAST40		"ho"
#define	SCNuLEAST40		"hu"
#define	SCNxLEAST40		"hx"
#elif	__Q8_IW >= 40
#define	PRIdLEAST40		"d"
#define	PRIiLEAST40		"i"
#define	PRIoLEAST40		"o"
#define	PRIuLEAST40		"u"
#define	PRIxLEAST40		"x"
#define	PRIXLEAST40		"X"
#define	SCNdLEAST40		"d"
#define	SCNiLEAST40		"i"
#define	SCNoLEAST40		"o"
#define	SCNuLEAST40		"u"
#define	SCNxLEAST40		"x"
#elif	__Q8_LW >= 40
#define	PRIdLEAST40		"ld"
#define	PRIiLEAST40		"li"
#define	PRIoLEAST40		"lo"
#define	PRIuLEAST40		"lu"
#define	PRIxLEAST40		"lx"
#define	PRIXLEAST40		"lX"
#define	SCNdLEAST40		"ld"
#define	SCNiLEAST40		"li"
#define	SCNoLEAST40		"lo"
#define	SCNuLEAST40		"lu"
#define	SCNxLEAST40		"lx"
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
#define	PRIdLEAST40		__Q8_QF"d"
#define	PRIiLEAST40		__Q8_QF"i"
#define	PRIoLEAST40		__Q8_QF"o"
#define	PRIuLEAST40		__Q8_QF"u"
#define	PRIxLEAST40		__Q8_QF"x"
#define	PRIXLEAST40		__Q8_QF"X"
#define	SCNdLEAST40		__Q8_QF"d"
#define	SCNiLEAST40		__Q8_QF"i"
#define	SCNoLEAST40		__Q8_QF"o"
#define	SCNuLEAST40		__Q8_QF"u"
#define	SCNxLEAST40		__Q8_QF"x"
#endif
#if	__Q8_CW < __Q8_IW && __Q8_CW >= 64 && defined(__Q8_SC)
#define	PRIdLEAST64		"d"
#define	PRIiLEAST64		"i"
#define	PRIoLEAST64		"o"
#define	PRIuLEAST64		"u"
#define	PRIxLEAST64		"x"
#define	PRIXLEAST64		"X"
#ifdef	__Q8_CF
#define	SCNdLEAST64		__Q8_CF"d"
#define	SCNiLEAST64		__Q8_CF"i"
#define	SCNoLEAST64		__Q8_CF"o"
#define	SCNuLEAST64		__Q8_CF"u"
#define	SCNxLEAST64		__Q8_CF"x"
#endif
#elif	__Q8_SW < __Q8_IW && __Q8_SW >= 64
#define	PRIdLEAST64		"d"
#define	PRIiLEAST64		"i"
#define	PRIoLEAST64		"o"
#define	PRIuLEAST64		"u"
#define	PRIxLEAST64		"x"
#define	PRIXLEAST64		"X"
#define	SCNdLEAST64		"hd"
#define	SCNiLEAST64		"hi"
#define	SCNoLEAST64		"ho"
#define	SCNuLEAST64		"hu"
#define	SCNxLEAST64		"hx"
#elif	__Q8_IW >= 64
#define	PRIdLEAST64		"d"
#define	PRIiLEAST64		"i"
#define	PRIoLEAST64		"o"
#define	PRIuLEAST64		"u"
#define	PRIxLEAST64		"x"
#define	PRIXLEAST64		"X"
#define	SCNdLEAST64		"d"
#define	SCNiLEAST64		"i"
#define	SCNoLEAST64		"o"
#define	SCNuLEAST64		"u"
#define	SCNxLEAST64		"x"
#elif	__Q8_LW >= 64
#define	PRIdLEAST64		"ld"
#define	PRIiLEAST64		"li"
#define	PRIoLEAST64		"lo"
#define	PRIuLEAST64		"lu"
#define	PRIxLEAST64		"lx"
#define	PRIXLEAST64		"lX"
#define	SCNdLEAST64		"ld"
#define	SCNiLEAST64		"li"
#define	SCNoLEAST64		"lo"
#define	SCNuLEAST64		"lu"
#define	SCNxLEAST64		"lx"
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
#define	PRIdLEAST64		__Q8_QF"d"
#define	PRIiLEAST64		__Q8_QF"i"
#define	PRIoLEAST64		__Q8_QF"o"
#define	PRIuLEAST64		__Q8_QF"u"
#define	PRIxLEAST64		__Q8_QF"x"
#define	PRIXLEAST64		__Q8_QF"X"
#define	SCNdLEAST64		__Q8_QF"d"
#define	SCNiLEAST64		__Q8_QF"i"
#define	SCNoLEAST64		__Q8_QF"o"
#define	SCNuLEAST64		__Q8_QF"u"
#define	SCNxLEAST64		__Q8_QF"x"
#endif

#define	PRIdFAST8		"d"
#define	PRIiFAST8		"i"
#define	PRIoFAST8		"o"
#define	PRIuFAST8		"u"
#define	PRIxFAST8		"x"
#define	PRIXFAST8		"X"
#define	SCNdFAST8		"d"
#define	SCNiFAST8		"i"
#define	SCNoFAST8		"o"
#define	SCNuFAST8		"u"
#define	SCNxFAST8		"x"
#define	PRIdFAST16		"d"
#define	PRIiFAST16		"i"
#define	PRIoFAST16		"o"
#define	PRIuFAST16		"u"
#define	PRIxFAST16		"x"
#define	PRIXFAST16		"X"
#define	SCNdFAST16		"d"
#define	SCNiFAST16		"i"
#define	SCNoFAST16		"o"
#define	SCNuFAST16		"u"
#define	SCNxFAST16		"x"
#if	__Q8_IW >= 32
#define	PRIdFAST32		"d"
#define	PRIiFAST32		"i"
#define	PRIoFAST32		"o"
#define	PRIuFAST32		"u"
#define	PRIxFAST32		"x"
#define	PRIXFAST32		"X"
#define	SCNdFAST32		"d"
#define	SCNiFAST32		"i"
#define	SCNoFAST32		"o"
#define	SCNuFAST32		"u"
#define	SCNxFAST32		"x"
#else
#define	PRIdFAST32		"ld"
#define	PRIiFAST32		"li"
#define	PRIoFAST32		"lo"
#define	PRIuFAST32		"lu"
#define	PRIxFAST32		"lx"
#define	PRIXFAST32		"lX"
#define	SCNdFAST32		"ld"
#define	SCNiFAST32		"li"
#define	SCNoFAST32		"lo"
#define	SCNuFAST32		"lu"
#define	SCNxFAST32		"lx"
#endif
#if	__Q8_IW >= 40
#define	PRIdFAST40		"d"
#define	PRIiFAST40		"i"
#define	PRIoFAST40		"o"
#define	PRIuFAST40		"u"
#define	PRIxFAST40		"x"
#define	PRIXFAST40		"X"
#define	SCNdFAST40		"d"
#define	SCNiFAST40		"i"
#define	SCNoFAST40		"o"
#define	SCNuFAST40		"u"
#define	SCNxFAST40		"x"
#elif	__Q8_LW >= 40
#define	PRIdFAST40		"ld"
#define	PRIiFAST40		"li"
#define	PRIoFAST40		"lo"
#define	PRIuFAST40		"lu"
#define	PRIxFAST40		"lx"
#define	PRIXFAST40		"lX"
#define	SCNdFAST40		"ld"
#define	SCNiFAST40		"li"
#define	SCNoFAST40		"lo"
#define	SCNuFAST40		"lu"
#define	SCNxFAST40		"lx"
#elif	__Q8_QW >= 40			/* (will be 0 if not defined) */
#define	PRIdFAST40		__Q8_QF"d"
#define	PRIiFAST40		__Q8_QF"i"
#define	PRIoFAST40		__Q8_QF"o"
#define	PRIuFAST40		__Q8_QF"u"
#define	PRIxFAST40		__Q8_QF"x"
#define	PRIXFAST40		__Q8_QF"X"
#define	SCNdFAST40		__Q8_QF"d"
#define	SCNiFAST40		__Q8_QF"i"
#define	SCNoFAST40		__Q8_QF"o"
#define	SCNuFAST40		__Q8_QF"u"
#define	SCNxFAST40		__Q8_QF"x"
#endif
#if	__Q8_IW >= 64
#define	PRIdFAST64		"d"
#define	PRIiFAST64		"i"
#define	PRIoFAST64		"o"
#define	PRIuFAST64		"u"
#define	PRIxFAST64		"x"
#define	PRIXFAST64		"X"
#define	SCNdFAST64		"d"
#define	SCNiFAST64		"i"
#define	SCNoFAST64		"o"
#define	SCNuFAST64		"u"
#define	SCNxFAST64		"x"
#elif	__Q8_LW >= 64
#define	PRIdFAST64		"ld"
#define	PRIiFAST64		"li"
#define	PRIoFAST64		"lo"
#define	PRIuFAST64		"lu"
#define	PRIxFAST64		"lx"
#define	PRIXFAST64		"lX"
#define	SCNdFAST64		"ld"
#define	SCNiFAST64		"li"
#define	SCNoFAST64		"lo"
#define	SCNuFAST64		"lu"
#define	SCNxFAST64		"lx"
#elif	__Q8_QW >= 64			/* (will be 0 if not defined) */
#define	PRIdFAST64		__Q8_QF"d"
#define	PRIiFAST64		__Q8_QF"i"
#define	PRIoFAST64		__Q8_QF"o"
#define	PRIuFAST64		__Q8_QF"u"
#define	PRIxFAST64		__Q8_QF"x"
#define	PRIXFAST64		__Q8_QF"X"
#define	SCNdFAST64		__Q8_QF"d"
#define	SCNiFAST64		__Q8_QF"i"
#define	SCNoFAST64		__Q8_QF"o"
#define	SCNuFAST64		__Q8_QF"u"
#define	SCNxFAST64		__Q8_QF"x"
#endif

#define	PRIdMAX			__Q8_MF"d"
#define	PRIiMAX			__Q8_MF"i"
#define	PRIoMAX			__Q8_MF"o"
#define	PRIuMAX			__Q8_MF"u"
#define	PRIxMAX			__Q8_MF"x"
#define	PRIXMAX			__Q8_MF"X"
#define	SCNdMAX			__Q8_MF"d"
#define	SCNiMAX			__Q8_MF"i"
#define	SCNoMAX			__Q8_MF"o"
#define	SCNuMAX			__Q8_MF"u"
#define	SCNxMAX			__Q8_MF"x"

#if	defined(__Q8_PW)
#if	__Q8_IW >= __Q8_PW
#define	PRIdPTR			"d"
#define	PRIiPTR			"i"
#define	PRIoPTR			"o"
#define	PRIuPTR			"u"
#define	PRIxPTR			"x"
#define	PRIXPTR			"X"
#define	SCNdPTR			"d"
#define	SCNiPTR			"i"
#define	SCNoPTR			"o"
#define	SCNuPTR			"u"
#define	SCNxPTR			"x"
#elif	__Q8_LW >= __Q8_PW
#define	PRIdPTR			"ld"
#define	PRIiPTR			"li"
#define	PRIoPTR			"lo"
#define	PRIuPTR			"lu"
#define	PRIxPTR			"lx"
#define	PRIXPTR			"lX"
#define	SCNdPTR			"ld"
#define	SCNiPTR			"li"
#define	SCNoPTR			"lo"
#define	SCNuPTR			"lu"
#define	SCNxPTR			"lx"
#elif	__Q8_QW >= __Q8_PW
#define	PRIdPTR			__Q8_QF"d"
#define	PRIiPTR			__Q8_QF"i"
#define	PRIoPTR			__Q8_QF"o"
#define	PRIuPTR			__Q8_QF"u"
#define	PRIxPTR			__Q8_QF"x"
#define	PRIXPTR			__Q8_QF"X"
#define	SCNdPTR			__Q8_QF"d"
#define	SCNiPTR			__Q8_QF"i"
#define	SCNoPTR			__Q8_QF"o"
#define	SCNuPTR			__Q8_QF"u"
#define	SCNxPTR			__Q8_QF"x"
#elif	__Q8_MW >= __Q8_PW
#define	PRIdPTR			__Q8_MF"d"
#define	PRIiPTR			__Q8_MF"i"
#define	PRIoPTR			__Q8_MF"o"
#define	PRIuPTR			__Q8_MF"u"
#define	PRIxPTR			__Q8_MF"x"
#define	PRIXPTR			__Q8_MF"X"
#define	SCNdPTR			__Q8_MF"d"
#define	SCNiPTR			__Q8_MF"i"
#define	SCNoPTR			__Q8_MF"o"
#define	SCNuPTR			__Q8_MF"u"
#define	SCNxPTR			__Q8_MF"x"
#endif
#endif

#endif	/* !defined(__cplusplus) || defined(__STDC_FORMAT_MACROS) */

/*
	7.8.2	Functions for greatest-width integer types
*/

intmax_t	imaxabs __Q8_PARAMS((intmax_t __j));
imaxdiv_t	imaxdiv __Q8_PARAMS((intmax_t __numer, intmax_t __denom));
intmax_t	strtoimax __Q8_PARAMS((__Q8_CONST char *__Q8_RESTRICT __nptr,
			char **__Q8_RESTRICT __endptr, int __base));
uintmax_t	strtoumax __Q8_PARAMS((__Q8_CONST char *__Q8_RESTRICT __nptr,
			char **__Q8_RESTRICT __endptr, int __base));
intmax_t	wcstoimax __Q8_PARAMS((__Q8_CONST __Q8_WT *__Q8_RESTRICT __nptr,
			__Q8_WT **__Q8_RESTRICT __endptr, int __base));
uintmax_t	wcstoumax __Q8_PARAMS((__Q8_CONST __Q8_WT *__Q8_RESTRICT __nptr,
			__Q8_WT **__Q8_RESTRICT __endptr, int __base));

#ifdef	__cplusplus
		}
#endif

#endif	/* !defined(_INTTYPES_H) && !defined(_INC_INTTYPES) */

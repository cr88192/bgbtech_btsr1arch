#ifndef STD_STDINT_H
#define STD_STDINT_H

typedef signed char			int8_t;
typedef unsigned char			uint8_t;
typedef short					int16_t;
typedef unsigned short		uint16_t;
typedef int					int32_t;
typedef unsigned int			uint32_t;
typedef long long				int64_t;
typedef unsigned long long	uint64_t;
typedef __int128				int128_t;
typedef unsigned __int128		uint128_t;

typedef signed char			int_least8_t;
typedef unsigned char			uint_least8_t;
typedef short					int_least16_t;
typedef unsigned short		uint_least16_t;
typedef int					int_least32_t;
typedef unsigned int			uint_least32_t;
typedef long long				int_least64_t;
typedef unsigned long long	uint_least64_t;

typedef __int128				int_least128_t;
typedef unsigned __int128		uint_least128_t;

typedef int					int_fast8_t;
typedef unsigned int			uint_fast8_t;
typedef int					int_fast16_t;
typedef unsigned int			uint_fast16_t;
typedef int					int_fast32_t;
typedef unsigned int			uint_fast32_t;
typedef long long				int_fast64_t;
typedef unsigned long long	uint_fast64_t;

typedef __intptr				intptr_t;
typedef __uintptr				uintptr_t;

typedef __int128				intmax_t;
typedef unsigned __int128		uintmax_t;

#if	!defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

#define	INT8_MAX		(         127 )
#define	INT8_MIN		(        -128 )
#define	UINT8_MAX		(         255 )
#define	INT16_MAX		(       32767 )
#define	INT16_MIN		(      -32768 )
#define	UINT16_MAX		(       65535 )
#define	INT32_MAX		(  2147483647 )
#define	INT32_MIN		( -2147483648 )
#define	UINT32_MAX		(  4294967295U)

#define	INT64_MAX		(0x7FFFFFFFFFFFFFFFLL )
#define	INT64_MIN		(0x8000000000000000LL )
#define	UINT64_MAX		(0xFFFFFFFFFFFFFFFFULL)

#define	INT128_MAX		(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFi128 )
#define	INT128_MIN		(0x80000000000000000000000000000000i128 )
#define	UINT128_MAX		(0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFui128)


#define	INT_LEAST8_MAX		(         127 )
#define	INT_LEAST8_MIN		(        -128 )
#define	UINT_LEAST8_MAX		(         255 )
#define	INT_LEAST16_MAX		(       32767 )
#define	INT_LEAST16_MIN		(      -32768 )
#define	UINT_LEAST16_MAX	(       65535 )
#define	INT_LEAST32_MAX		(  2147483647 )
#define	INT_LEAST32_MIN		( -2147483648 )
#define	UINT_LEAST32_MAX	(  4294967295U)

#define	INT_LEAST64_MAX		(0x7FFFFFFFFFFFFFFFLL )
#define	INT_LEAST64_MIN		(0x8000000000000000LL )
#define	UINT_LEAST64_MAX	(0xFFFFFFFFFFFFFFFFULL)


#define	INT_FAST8_MAX		(  2147483647 )
#define	INT_FAST8_MIN		( -2147483648 )
#define	UINT_FAST8_MAX		(  4294967295U)
#define	INT_FAST16_MAX		(  2147483647 )
#define	INT_FAST16_MIN		( -2147483648 )
#define	UINT_FAST16_MAX		(  4294967295U)
#define	INT_FAST32_MAX		(  2147483647 )
#define	INT_FAST32_MIN		( -2147483648 )
#define	UINT_FAST32_MAX		(  4294967295U)

#define	INT_FAST64_MAX		(0x7FFFFFFFFFFFFFFFLL )
#define	INT_FAST64_MIN		(0x8000000000000000LL )
#define	UINT_FAST64_MAX		(0xFFFFFFFFFFFFFFFFULL)

#define	INTPTR_MAX		INT64_MAX
#define	INTPTR_MIN		INT64_MIN
#define	UINTPTR_MAX		UINT64_MAX

#define	INTMAX_MAX		INT128_MAX
#define	INTMAX_MIN		INT128_MIN
#define	UINTMAX_MAX		UINT128_MAX

#define	PTRDIFF_MAX		INT64_MAX
#define	PTRDIFF_MIN		INT64_MIN

#define	SIZE_MAX		UINT64_MAX

#define	WCHAR_MAX		65535
#define	WCHAR_MIN		0

#define	WINT_MAX		UINT32_MAX
#define	WINT_MIN		0

#endif

#if	!defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#define	INT8_C(c)		(INT_LEAST8_MAX-INT_LEAST8_MAX+(c))
#define	UINT8_C(c)		(UINT_LEAST8_MAX-UINT_LEAST8_MAX+(c))
#define	INT16_C(c)		(INT_LEAST16_MAX-INT_LEAST16_MAX+(c))
#define	UINT16_C(c)		(UINT_LEAST16_MAX-UINT_LEAST16_MAX+(c))
#define	INT32_C(c)		(INT_LEAST32_MAX-INT_LEAST32_MAX+(c))
#define	UINT32_C(c)		(UINT_LEAST32_MAX-UINT_LEAST32_MAX+(c))
#define	INT64_C(c)		(INT_LEAST64_MAX-INT_LEAST64_MAX+(c))
#define	UINT64_C(c)		(UINT_LEAST64_MAX-UINT_LEAST64_MAX+(c))

#define	INTMAX_C(c)		(INTMAX_MAX-INTMAX_MAX+(c))
#define	UINTMAX_C(c)	(UINTMAX_MAX-UINTMAX_MAX+(c))

#endif

#endif

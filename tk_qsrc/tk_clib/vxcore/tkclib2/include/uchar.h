#ifndef STD_UCHAR_H
#define STD_UCHAR_H

#include <stdint.h>
#include <wchar.h>

#define __STDC_UTF_16__
#define __STDC_UTF_32__ 

typedef uint_least16_t		char16_t;
typedef uint_least32_t		char32_t;

size_t c16rtomb(
	char *restrict s,
	char16_t wc,
	mbstate_t *restrict ps);

size_t c32rtomb(
	char *restrict s,
	char32_t wc,
	mbstate_t *restrict ps);

size_t mbrtoc16(
	char16_t *restrict pwc,
	const char *restrict s,
	size_t n,
	mbstate_t *restrict ps);

size_t mbrtoc32(
	char32_t *restrict pwc,
	const char *restrict s,
	size_t n,
	mbstate_t *restrict ps);

#endif
#include "uchar.h"

size_t c32rtomb(
	char *restrict s,
	char32_t wc,
	mbstate_t *restrict ps)
{
	unsigned char *us;
	int i;

	us = (unsigned char *)s;

	if(wc<0x0080)
	{
		*us = wc;
		return (1);
	}

	if(wc<0x0800)
	{
		us[0] = 0xC0 | ((wc>>6)&0x1F);
		us[1] = 0x80 | ((wc>>0)&0x3F);
		return (2);
	}

	if(wc<0x10000)
	{
		us[0] = 0xE0 | ((wc>>12)&0x0F);
		us[1] = 0x80 | ((wc>> 6)&0x3F);
		us[2] = 0x80 | ((wc>> 0)&0x3F);
		return (3);
	}

	if(1)
	{
		us[0] = 0xF0 | ((wc>>18)&0x07);
		us[1] = 0x80 | ((wc>>12)&0x3F);
		us[2] = 0x80 | ((wc>> 6)&0x3F);
		us[3] = 0x80 | ((wc>> 0)&0x3F);
		return (4);
	}
}

size_t c16rtomb(
	char *restrict s,
	char16_t wc,
	mbstate_t *restrict ps)
{
	return(c32rtomb(s, wc, ps));
}

size_t mbrtoc32(
	char32_t *restrict pwc,
	const char *restrict s,
	size_t n,
	mbstate_t *restrict ps)
{
}

size_t mbrtoc16(
	char16_t *restrict pwc,
	const char *restrict s,
	size_t n,
	mbstate_t *restrict ps)
{
	char32_t tc;
	int i;

	i=mbrtoc32(&tc, s, n, ps);
	*pwc=tc;
	return(i);
}

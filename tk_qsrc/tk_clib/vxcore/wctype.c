/*
	wctype -- wide character classification and mapping utilities

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.25 of ISO/IEC 9899:1999 (E).

	This particular implementation requires the matching <wctype.h>.

	This is a minimal implementation for environments where
	internationalization is not considered important.

	It supports an encoding where all char codes are mapped
	to the *same* code values within a wchar_t or wint_t,
	so long as no other wchar_t codes are used by the program.

	XXX -- UNTESTED -- XXX
*/

#include <vxcore.h>

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <wctype.h>

/*
	7.25.2	Wide character classification utilities
*/

VXCORE_API int iswalnum(wint_t wc)
{
	return isalnum((int)(unsigned char)wc);
}

VXCORE_API int iswalpha(wint_t wc)
{
	return isalpha((int)(unsigned char)wc);
}

VXCORE_API int iswblank(wint_t wc)
{
	return(wc==' ');
//	return isblank((int)(unsigned char)wc);
}

VXCORE_API int iswcntrl(wint_t wc)
{
	return iscntrl((int)(unsigned char)wc);
}

VXCORE_API int iswdigit(wint_t wc)
{
	return isdigit((int)(unsigned char)wc);
}

VXCORE_API int iswgraph(wint_t wc)
{
	return isgraph((int)(unsigned char)wc);
}

VXCORE_API int iswlower(wint_t wc)
{
	return islower((int)(unsigned char)wc);
}

VXCORE_API int iswprint(wint_t wc)
{
	return isprint((int)(unsigned char)wc);
}

VXCORE_API int iswpunct(wint_t wc)
{
	return ispunct((int)(unsigned char)wc);
}

VXCORE_API int iswspace(wint_t wc)
{
	return isspace((int)(unsigned char)wc);
}

VXCORE_API int iswupper(wint_t wc)
{
	return isupper((int)(unsigned char)wc);
}

VXCORE_API int iswxdigit(wint_t wc)
{
	return isxdigit((int)(unsigned char)wc);
}

/*
	wctype() usually encodes properties as ORed bit combinations;
	for this *minimal implementation* I can take a lazy approach.	
*/

static struct
{
	const char	*name;
	int		(*func)(int);
	}	cmap[] =
	{
	"alnum",	iswalnum,
	"alpha",	iswalpha,
	"blank",	iswblank,
	"cntrl",	iswcntrl,
	"digit",	iswdigit,
	"graph",	iswgraph,
	"lower",	iswlower,
	"print",	iswprint,
	"punct",	iswpunct,
	"space",	iswspace,
	"upper",	iswupper,
	"xdigit",	iswxdigit
};
#define	NCMAP	(sizeof cmap / sizeof cmap[0])

VXCORE_API int iswctype(wint_t wc, wctype_t desc)
{
	register int i;

	if ( desc <= 0 || desc > NCMAP )	/* undefined behavior */
	{
		errno = EDOM;	
		return 0;		/* a form of not having the property */
	}

	return cmap[desc-1].func(wc);
}

VXCORE_API wctype_t wctype(const char *property) /* returns map index + 1 */
{
	register int i;

	for ( i = 0; i < NCMAP; ++i )
		if ( strcmp(property, cmap[i].name) == 0 )
			return i+1;

	return 0;			/* indicates "not valid" */
}

/*
	7.25.3	Wide character case mapping utilities
*/

VXCORE_API wint_t towlower(wint_t wc)
{
	return tolower((int)(unsigned char)wc);
}

VXCORE_API wint_t towupper(wint_t wc)
{
	return toupper((int)(unsigned char)wc);
}

/*
	For this *minimal implementation* I can take a lazy approach.	
*/

static struct
{
	const char	*name;
	int		(*func)(int);
	}	tmap[] =
	{
	"lower",	towlower,
	"upper",	towupper
};
#define	NTMAP	(sizeof tmap / sizeof tmap[0])

VXCORE_API wint_t iswctrans(wint_t wc, wctrans_t desc)
{
	register int	i;

	if ( desc <= 0 || desc > NTMAP )	/* undefined behavior */
	{
		errno = EDOM;	
		return 0;		/* a form of not having the property */
	}

	return tmap[desc-1].func(wc);
}

VXCORE_API wctrans_t wctrans(const char	*property) /* returns map index + 1 */
{
	register int	i;

	for ( i = 0; i < NTMAP; ++i )
		if ( strcmp(property, tmap[i].name) == 0 )
			return i+1;

	return 0;			/* indicates "not valid" */
}

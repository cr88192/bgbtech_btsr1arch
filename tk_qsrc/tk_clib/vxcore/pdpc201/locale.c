/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  locale.c - implementation of stuff in locale.h                   */
/*                                                                   */
/*********************************************************************/

/*
 * BGB: Adds some logic to make this more than just a stub...
 */

#include "locale.h"
#include "limits.h"
#include "string.h"
#include "stddef.h"

#include "tk_locale.h"

static struct lconv thislocale = {
    ".",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX
};

struct {
char *name;
int flags;
} locale_list[] = {
{ "C", TK_LOCALE_FLAG_CP1252 },
{ "POSIX", TK_LOCALE_FLAG_CP1252 },
{ ".UTF8", TK_LOCALE_FLAG_UTF8 },
{ "EN_US.UTF8", TK_LOCALE_FLAG_UTF8 },
{ NULL, 0 }
};

short locale_id;
int locale_flags;

__PDPCLIB_API__ char *setlocale(int category, const char *locale)
{
	int i;
//    (void)category;
    if (locale == NULL)
    {
        return ("C");
    }
    else if ((strcmp(locale, "C") == 0)
             || (strcmp(locale, "") == 0))
    {
		locale_id = 0;
        return ("C");
    }
    else
    {
		for(i=0; locale_list[i].name; i++)
		{
			if(!stricmp(locale_list[i].name, locale))
			{
				locale_id = i;
				locale_flags = locale_list[i].flags;
				return(locale_list[i].name);
			}
		}
    
        return (NULL);
    }
}

__PDPCLIB_API__ struct lconv *localeconv(void)
{
    return (&thislocale);
}

__PDPCLIB_API__ int isalnum2(int c)
{
//    return (__isbuf[c] & 0x0001U);
}

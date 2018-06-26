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

#include "locale.h"
#include "limits.h"
#include "string.h"
#include "stddef.h"

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

__PDPCLIB_API__ char *setlocale(int category, const char *locale)
{
    (void)category;
    if (locale == NULL)
    {
        return ("C");
    }
    else if ((strcmp(locale, "C") == 0)
             || (strcmp(locale, "") == 0))
    {
        return ("C");
    }
    else
    {
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

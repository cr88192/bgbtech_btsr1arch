#include <locale.h>

struct lconv std_lconv_c = {
	".",	"",		"",		"",
	"",		"",		"",		"",
	"",		"",
	CHAR_MAX,	CHAR_MAX,	CHAR_MAX,	CHAR_MAX,
	CHAR_MAX,	CHAR_MAX,	CHAR_MAX,	CHAR_MAX
};

char *setlocale(int category, const char *locale)
{
	return("C");
}

struct lconv *localeconv(void)
{
    return (&std_lconv_c);
}


#ifndef	STD_WCTYPE_H
#define	STD_WCTYPE_H

#ifdef	__cplusplus
extern	"C"	{
#endif

#ifndef	STD_WINT_T
#define	STD_WINT_T
typedef unsigned int	wint_t;
#endif

#ifndef	STD_WCTRANS_T
#define	STD_WCTRANS_T
typedef unsigned int	wctrans_t;
#endif

#ifndef	STD_WCTYPE_T
#define	STD_WCTYPE_T
typedef int			wctype_t;
#endif

#ifndef	WEOF
#define	WEOF		0xFFFFFFFFU
#endif

extern int			iswalnum(wint_t wc);
extern int			iswalpha(wint_t wc);
extern int			iswblank(wint_t wc);
extern int			iswcntrl(wint_t wc);
extern int			iswdigit(wint_t wc);
extern int			iswgraph(wint_t wc);
extern int			iswlower(wint_t wc);
extern int			iswprint(wint_t wc);
extern int			iswpunct(wint_t wc);
extern int			iswspace(wint_t wc);
extern int			iswupper(wint_t wc);
extern int			iswxdigit(wint_t wc);
extern int			iswctype(wint_t wc, wctype_t desc);
extern wctype_t	wctype(const char *prop);

extern wint_t		towlower(wint_t wc);
extern wint_t		towupper(wint_t wc);
extern wint_t		towctrans(wint_t wc, wctrans_t desc);
extern wctrans_t	wctrans(const char *prop);

#ifdef	__cplusplus
}
#endif

#endif

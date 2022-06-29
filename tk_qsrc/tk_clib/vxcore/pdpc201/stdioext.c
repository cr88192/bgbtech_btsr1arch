#include "stdio.h"
#include "uchar.h"

wint_t	fgetwc (FILE *stream)
{
	int i, j, k, l;
	
	i=fgetc(stream);
	if((i<0) || (i==EOF))
		return(WEOF);
	
	if(i<0x80)
		{ return(i); }

	if(i<0xC0)
	{
		/* Malformed */
		errno = EILSEQ;
		return(WEOF);
	}

	if(i<0xE0)
	{
		j=fgetc(stream);
		if((j<0) || (j==EOF))
			return(WEOF);
		
		if((j<0x80) || (j>=0xC0))
		{
			errno = EILSEQ;
			return(WEOF);
		}
		
		k=((i&0x1F)<<6)|(j&0x3F);
		return(k);
	}

	if(i<0xF0)
	{
		j=fgetc(stream);
		if((j<0) || (j==EOF))
			return(WEOF);

		k=fgetc(stream);
		if((k<0) || (k==EOF))
			return(WEOF);
		
		if(	(j<0x80) || (j>=0xC0) ||
			(k<0x80) || (k>=0xC0))
		{
			errno = EILSEQ;
			return(WEOF);
		}
		
		k=((i&0x0F)<<12)|((j&0x3F)<<6)|(k&0x3F);
		return(k);
	}

	if(i<0xF8)
	{
		j=fgetc(stream);
		if((j<0) || (j==EOF))
			return(WEOF);

		k=fgetc(stream);
		if((k<0) || (k==EOF))
			return(WEOF);

		l=fgetc(stream);
		if((l<0) || (l==EOF))
			return(WEOF);
		
		if(	(j<0x80) || (j>=0xC0) ||
			(k<0x80) || (k>=0xC0) ||
			(l<0x80) || (l>=0xC0))
		{
			errno = EILSEQ;
			return(WEOF);
		}
		
		k=((i&0x07)<<18)|((j&0x3F)<<12)|((k&0x3F)<<6)|(l&0x3F);
		return(k);
	}

	errno = EILSEQ;
	return(WEOF);
}

wchar_t *fgetws (wchar_t * dstr, int n, FILE * stream)
{
	wchar_t *ct, *cte;
	wint_t wc;
	
	ct=dstr;
	cte=dstr+n;

	while(!feof(str) && (ct<cte))
	{
		wc = fgetwc (strem);
		if(wc == WEOF)
			break;

		*ct++=wc;
		if(wc == '\n')
			break;
	}

	*ct=0;
	return(dstr);
}

wint_t	fputwc (wchar_t wc, FILE *str)
{
	if(wc<0x80)
	{
		return(fputc(wc, str));
	}

	if(wc<0x800)
	{
		fputc(0xC0|((wc>>6)&0x1F), str);
		fputc(0x80|((wc   )&0x3F), str);
		return(wc);
	}

	if(wc<0x10000)
	{
		fputc(0xE0|((wc>>12)&0x0F), str);
		fputc(0x80|((wc>> 6)&0x3F), str);
		fputc(0x80|((wc    )&0x3F), str);
		return(wc);
	}

	if(wc<0x200000)
	{
		fputc(0xF0|((wc>>18)&0x07), str)
		fputc(0x80|((wc>>12)&0x3F), str)
		fputc(0x80|((wc>> 6)&0x3F), str)
		fputc(0x80|((wc    )&0x3F), str)
		return(wc);
	}
	
	return(WEOF);
}

int	fputws (const wchar_t *str, FILE *stream)
{
	wchar_t *cs;
	int rs;
	
	cs=str;
	while(*cs)
	{
		rs=fputwc(*cs++, stream);
		if(rs==WEOF)
			return(-1);
	}
	return(0);
}

int	fwide (FILE *stream, int mode)
{
	return(0);
}


wint_t	getwc (FILE *stream)
{
	return(fgetwc(stream));
}

wint_t	getwchar (void)
{
	return(fgetwc(stdin));
}

wint_t	putwc (wchar_t wc, FILE *stream)
{
	return(fputwc(wc, stream));
}

wint_t	putwchar (wchar_t wc)
{
	return(fputwc(wc, stdout));
}

wint_t	ungetwc (wint_t wc, FILE *stream)
{
	if(wc<0x80)
	{
		return(ungetc(wc, str));
	}
	
	return(WEOF);
}


int	vswprintf (wchar_t * dstr, size_t max,
	const wchar_t * fmt, va_list args)
{
	char ntb[160];
	wchar_t *cs, *ct, *cte, *cs1;
	char *ncs;
	char pcfill;
	__int128 llv;
	s64 lv;
	int i, v, w, ll, ch, cn;
	
	ct=dstr;
	cte=dstr+max;
	cs=fmt;
	while(*cs && (ct<cte))
	{
		if(*cs!='%')
		{
			if(dstr)
				*ct=*cs;
			ct++; cs++;
			continue;
		}

		if(cs[1]=='%')
		{
			if(dstr)
				*ct=*cs;
			ct++; cs+=2;
			continue;
		}
		
		cs++;

		if(*cs=='0')
		{
			pcfill='0';
			cs++;
		}else
		{
			pcfill=' ';
		}

		w=0;
		if((*cs>='0') && (*cs<='9'))
		{
			while((*cs>='0') && (*cs<='9'))
				w=(w*10)+((*cs++)-'0');
		}
		
		ll=0;
		if(cs[0]=='l')
		{
			if(cs[1]=='l')
			{
				ll=2;
				cs+=2;
			}else
			{
				ll=1;
				cs++;
			}
		}else
			if(cs[0]=='L')
		{
			ll=3;
			cs++;
		}
		
		ch=*cs++;
		switch(ch)
		{
		case 'd':
			if(ll&2)
				llv=va_arg(lst, long long);
			else
				llv=va_arg(lst, int);
			xlltoa_cnt(llv, ntb, 10, w);
			ncs=ntb;
			while(*ncs)
			{
				if(dstr)
					*ct=*ncs;
				ct++; ncs++;
			}
			break;
		case 'X':
			if(ll&2)
				llv=va_arg(lst, long long);
			else
				llv=va_arg(lst, int);
			xlltoa_cnt(llv, ntb, 64|16, w);
			ncs=ntb;
			while(*ncs)
			{
				if(dstr)
					*ct=*ncs;
				ct++; ncs++;
			}
			break;
		case 'x':
			if(ll&2)
				llv=va_arg(lst, long long);
			else
				llv=va_arg(lst, int);
			xlltoa_cnt(llv, ntb, 16, w);
			ncs=ntb;
			while(*ncs)
			{
				if(dstr)
					*ct=*ncs;
				ct++; ncs++;
			}
			break;

		case 's':
			cn=w;
			if(ll)
			{
				cs1=va_arg(lst, wchar_t *);
				while(*cs1)
				{
					if(dstr)
						*ct=*cs1;
					ct++; cs1++;
					cn--;
				}
			}else
			{
				ncs=va_arg(lst, char_t *);
				while(*ncs)
				{
					if(dstr)
						*ct=*ncs;
					ct++; ncs++;
					cn--;
				}
			}
			
			while(cn>0)
			{
				if(dstr)
					*ct=pcfill;
				ct++;
			}
			break;

		case 'c':
			lv=va_arg(lst, int);
			if(dstr)
				*ct=lv;
			ct++;
			break;

		case 'p':
			lv=va_arg(lst, long);
			xlltoa_cnt(lv, ntb, 16, 16);
			ncs=ntb;
			while(*ncs)
			{
				if(dstr)
					*ct=*ncs;
				ct++; ncs++;
			}
			break;
		}
	}
	
	if(ct>=cte)
		return(-1);
	
	return(ct-dstr);
}

int	vfwprintf (FILE * stream, const wchar_t *  fmt, va_list args)
{
	wchar_t tbuf[512];
	wchar_t *tb;
	int i;
	
	tb=tbuf;
	i=vswprintf(tb, 511, fmt, args);

	if(i<0)
	{
		i=vswprintf(NULL, 511, fmt, args);
		if(i<0)
			return(i);
		tb=malloc((i+17)*sizeof(wchar_t));
		i=vswprintf(tb, i+16, fmt, args);
	}

	fputws(tb, fmt);

	if(tb!=tbuf)
			free(tb);

	return(i);
}

int	vwprintf (const wchar_t * fmt, va_list args)
{
	return(vfwprintf(stdout, fmt, args));
}

int	fwprintf (FILE * stream, const wchar_t *  fmt, ...)
{
	va_list lst;
	int i;
	va_start(lst, fmt);
	i=vfwprintf(stream, fmt, lst);
	va_end(lst);
	return(i);
}

int	swprintf (wchar_t * dstr, size_t n, const wchar_t *  fmt, ...)
{
	va_list lst;
	int i;
	va_start(lst, fmt);
	i=vswprintf(dstr, n, fmt, lst);
	va_end(lst);
	return(i);
}

int	wprintf (const wchar_t * fmt, ...)
{
	va_list lst;
	int i;
	va_start(lst, fmt);
	i=vwprintf(fmt, lst);
	va_end(lst);
	return(i);
}


extern int	vswscanf (
	const wchar_t * sstr,
	const wchar_t * fmt, va_list args)
{
}

extern int	vfwscanf (FILE * stream,
			const wchar_t *  fmt,
			va_list args));
extern int	vwscanf (const wchar_t * 
			fmt, va_list args));


extern int	swscanf (const wchar_t * sstr,
			const wchar_t *  fmt, ...)
{
}

extern int	fwscanf (FILE * stream, const wchar_t *  fmt, ...)
{
}

extern int	wscanf (const wchar_t * fmt, ...)
{
}


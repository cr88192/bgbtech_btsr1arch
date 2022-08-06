#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <tkclib2/ci_stdio.h>

void _vvprintf_putc(int v, _stdio_printfstate_t *stv)
{
	if(stv->is_wcs)
	{
		/* wchar_t / UTF-16 */
	
		if(stv->cts)
		{
			if(stv->ct<stv->cte)
				{ *(short)(stv->ct)=v; }
			stv->ct+=2;
			return;
		}
		
		if(stv->fd)
		{
			fputc((v   )&255, stv->fd);
			fputc((v>>8)&255, stv->fd);
			stv->ct+=2;
			return;
		}

		/* Simply count output characters in this case. */
		stv->ct+=2;
		return;
	}

	if(stv->cts)
	{
		if(stv->ct<stv->cte)
			*stv->ct=v;
		stv->ct++;
		return;
	}
	
	if(stv->fd)
	{
		fputc(v, stv->fd);
		stv->ct++;
		return;
	}

	/* Simply count output characters in this case. */
	stv->ct++;
	return;
}

void _vvprintf_putwc(int v, _stdio_printfstate_t *stv)
{
	inr v1;

	if(v<128)
	{
		_vvprintf_putc(v, stv);
		return;
	}
	
	if(stv->is_wcs)
	{
		if(v<0x10000)
		{
			_vvprintf_putc(v, stv);
			return;
		}

		if(v<0x110000)
		{
			v1=v-0x10000;
			_vvprintf_putc(0xD800|((v>>10)&1023), stv);
			_vvprintf_putc(0xDC00|((v>> 0)&1023), stv);
			return;
		}

		return;
	}
	
	
	if(v<0x0800)
	{
		_vvprintf_putc(0xC0|(v>>6), stv);
		_vvprintf_putc(0x80|(v&0x3F), stv);
		return;
	}
	if(v<0x10000)
	{
		_vvprintf_putc(0xE0|((v   )>>12), stv);
		_vvprintf_putc(0x80|((v>>6)&0x3F), stv);
		_vvprintf_putc(0x80|((v   )&0x3F), stv);
		return;
	}
}

void _vvprintf_putlli(
	__uint128 val,
	int radix,
	int width,
	int fill,
	_stdio_printfstate_t *stv)
{
	char stk[64];
	char *sct;
	__uint128 v;
	int w, r;
	
	sct=stk+63;
	*(--sct)=0;
	
	v=val; w=width;
	while(v!=0)
	{
		w--;
		r=v%radix;
		v=v/radix;
		
		if(r<10)
		{
			*(--sct)='0'+r;
		}else
		{
			*(--sct)='A'+(r-10);
		}
	}
	
	while(w>0)
		{ w--; *(--sct)=fill; }
	
	while(*sct)
		{ _vvprintf_putc(*sct++, stv); }
}

void _vvprintf_readspec(unsigned char **rcs,
	_stdio_printfstate_t *stv)
{
	unsigned char *cs;
	int i, j, k, c;

	stv->fmt_fill=' ';
	stv->fmt_sgn=0;
	stv->fmt_left=0;
	stv->fmt_width=0;
	stv->fmt_prec=0;
	stv->fmt_lmod=0;
	stv->fmt_conv=0;

	cs=*rcs;
	cs++;		//'%'

	c=*cs;
	if((c=='d') || (c=='s'))
	{
		stv->fmt_conv=*cs++;	
		*rcs=cs;
		return;
	}

	while(*cs)
	{
		if(*cs=='+')
			{ stv->fmt_sgn=1; cs++; continue; }
		if(*cs=='-')
			{ stv->fmt_left=1; cs++; continue; }
		if(*cs=='0')
			{ stv->fmt_fill='0'; cs++; continue; }
		if(*cs==' ')
			{ stv->fmt_fill=' '; cs++; continue; }
		break;
	}
	
	i=0;
	while((*cs>='0') && *cs<='9')
		{ i=(i*10)+((*cs++)-'0'); }
	stv->fmt_width=i;
	
	if(*cs=='.')
	{
		cs++;
		i=0;
		while((*cs>='0') && *cs<='9')
			{ i=(i*10)+((*cs++)-'0'); }
		stv->fmt_prec=i;
	}
	
	c=*cs;
	if(c=='l')
	{
		if(cs[1]=='l')
		{
			cs+=2;
			stv->fmt_lmod='L';
		}else
		{
			cs++;
			stv->fmt_lmod='l';
		}
	}else
		if(c=='L')
	{
		cs++;
		stv->fmt_lmod='L';
	}else
		if(c=='j')
	{
		cs++;
		stv->fmt_lmod='Q';
	}

//	c=*cs;
	stv->fmt_conv=*cs++;
	
	*rcs=cs;
}

void _vvprintf_fmt_e(double val, int prec, _stdio_printfstate_t *stv)
{
	double f, g;
	long long li, lj;
	int prc, c;
	int i, j, k;
	
	g=f;
	if(f<0)
		g=-f;
	k=0;
	if(g>=10.0)
	{
		while(g>=1000.0)
			{ f=f*0.001; g=g*0.001; k+=3; }
		while(g>=10.0)
			{ f=f*0.1; g=g*0.1; k++; }
	}else if((g<1.0) && (g!=0.0))
	{
		while(g<0.001)
			{ f=f*1000.0; g=g*1000.0; k-=3; }
		while(g<1.0)
			{ f=f*10.0; g=g*10.0; k--; }
	}
	
	li=((long long)f);
	lj=(long long)((f-li)*10000000000000000.0);
	
	c=10; prc=prec;
	if(prc<=0)prc=6;
	c=16-prc;
	while(c>=3)
		{ c--; lj=lj/1000; }
	while(c>0)
		{ c--; lj=lj/10; }

	if(li<0)
	{
		_vvprintf_putc('-', stv);
		li=-li;
	}

	_vvprintf_putlli(li, 10,
		1, stv->fmt_fill, stv);
	_vvprintf_putc('.', stv);
	_vvprintf_putlli(lj, 10, prc, '0', stv);
	_vvprintf_putc('e', stv);
	if(k<0)
		{	_vvprintf_putc('-', stv);	k=-k;	}
	_vvprintf_putlli(k, 10, 2, '0', stv);
}

void _vvprintf_fmt_f(
	double val, int width, int prec, 
	_stdio_printfstate_t *stv)
{
	double f, g;
	long long li, lj;
	int prc, c;
	int i, j, k;
	
	li=((long)f);
	lj=(long)((f-li)*10000000000000000.0);
	
	c=10; prc=prec;
	if(prc<=0)prc=6;
	c=16-prc;
	while(c>=3)
		{ c--; lj=lj/1000; }
	while(c>0)
		{ c--; lj=lj/10; }

	if(li<0)
	{
		_vvprintf_putc('-', stv);
		li=-li;
	}

	_vvprintf_putlli(li, 10,
		width, stv->fmt_fill, stv);
	_vvprintf_putc('.', stv);
	_vvprintf_putlli(lj, 10, prc, '0', stv);
}

int _vvprintf(
	_stdio_printfstate_t *stv,
	const char *format, __va_list arg)
{
	__int128	li, lj;
	double f, g;
	unsigned char *cs, *cs2;
	wchar_t *wcs2;
	int c, prc, rdx;
	int i, j, k;

	cs=(unsigned char *)format;
	while(*cs)
	{
		c=cs[0];

		if(c!='%')
		{
			cs++;
			_vvprintf_putc(c, stv);
			continue;
		}
		
		if(cs[1]=='%')
		{
			cs+=2;
			_vvprintf_putc(c, stv);
			continue;
		}
		
		_vvprintf_readspec(&cs, stv);
		
		switch(stv->fmt_conv)
		{
			case 'd':	case 'i':
				switch(stv->lmod)
				{
				case 'l':	li=va_arg(arg, long);		break;
				case 'L':	li=va_arg(arg, long long);	break;
				case 'Q':	li=va_arg(arg, __int128);	break;
				default:	li=va_arg(arg, int);		break;
				}

				if(li<0)
				{
					_vvprintf_putc('-', stv);
					li=-li;
				}

				_vvprintf_putlli(li, 10,
					stv->fmt_width, stv->fmt_fill, stv);
				break;

			case 'o':	case 'u':
			case 'x':	case 'X':
				switch(stv->lmod)
				{
				case 'l':	li=va_arg(arg, unsigned long);			break;
				case 'L':	li=va_arg(arg, unsigned long long);	break;
				case 'Q':	li=va_arg(arg, unsigned __int128);		break;
				default:	li=va_arg(arg, unsigned int);			break;
				}
				rdx=10;
				if(stv->fmt_conv=='o')rdx=8;
				if(stv->fmt_conv=='u')rdx=10;
				if(stv->fmt_conv=='x')rdx=16;
				if(stv->fmt_conv=='X')rdx=16;

				_vvprintf_putlli(li, rdx,
					stv->fmt_width, stv->fmt_fill, stv);
				break;

			case 'e':	case 'E':
				f=va_arg(arg, double);
				_vvprintf_fmt_e(f, stv->fmt_prec, stv);
				break;

			case 'f':	case 'F':
				f=va_arg(arg, double);
				_vvprintf_fmt_f(f, stv->fmt_width, stv->fmt_prec, stv);
				break;

			case 'g':	case 'G':
				f=va_arg(arg, double);

				g=f;
				if(f<0)
					g=-f;
				
				if((g>=10000.0) || ((g<0.0001) && (g!=0.0)))
				{
					_vvprintf_fmt_e(f, stv->fmt_prec, stv);
					break;
				}				
				_vvprintf_fmt_f(f, stv->fmt_width, stv->fmt_prec, stv);
				break;

			case 'a':	case 'A':
				f=va_arg(arg, double);
				break;

			case 'c':
				i=va_arg(arg, int);
				if(stv->fmt_lmod=='l')
				{
					_vvprintf_putwc(i, stv);
					break;
				}
				_vvprintf_putc(i, stv);
				break;

			case 's':
				if(stv->fmt_lmod=='l')
				{
					wcs2=va_arg(arg, wchar_t *);
					while(*wcs2)
						_vvprintf_putwc(*wcs2++, stv);
					break;
				}

				cs2=va_arg(arg, char *);
				while(*cs2)
					_vvprintf_putc(*cs2++, stv);
				break;

			case 'p':
				li=va_arg(arg, unsigned long);
				k=16;
				if(sizeof(void *)==4)
					k=8;
				_vvprintf_putlli(li, k, 16, '0', stv);
				break;

			case 'n':
				cs2=va_arg(arg, int *);
				k=(stv->ct-stv->cts);
				if(stv->is_wcs)
					k=k>>1;
				*(int *)cs2=k;
				break;
		}
	}

	if((stv->ct+1)<stv->cte)
	{
		stv->ct[0]=0;
		stv->ct[1]=0;
	}else
		if(stv->ct<stv->cte)
	{
		*stv->ct=0;
	}

	k=(stv->ct-stv->cts);
	if(stv->is_wcs)
		k=k>>1;
	return(k);
}

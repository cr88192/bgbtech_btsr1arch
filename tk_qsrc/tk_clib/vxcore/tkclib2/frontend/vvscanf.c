#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <tkclib2/ci_stdio.h>

int _vvscanf_getc(_stdio_printfstate_t *stv)
{
	int v;
	if(stv->cts)
	{
		v=*stv->ct++;
		return(v);
	}
	
	if(stv->fd)
	{
		v=fgetc(stv->fd);
		stv->ct++;
		return(v);
	}
	
	return(-1);
}

void _vvscanf_ugetc(int v, _stdio_printfstate_t *stv)
{
	if(stv->cts)
	{
		stv->ct--;
		return;
	}
	
	if(stv->fd)
	{
		ungetc(v, stv->fd);
		stv->ct--;
		return;
	}
}

__int128 _vvscanf_readint(_stdio_printfstate_t *stv, int radix)
{
	__int128 li;
	int rc, neg, rdx, r;
	
	rc=-1;
	neg=0;

	if(rc<0)
		rc=_vvscanf_getc(stv);
	while((rc>0) && (rc<=' '))
		rc=_vvscanf_getc(stv);
	if(rc=='-')
		{ neg=1; rc=-1; }

	if(rc<0)
		rc=_vvscanf_getc(stv);
	
	rdx=radix;
	if(rdx<=0)
	{
		if(rc=='0')
		{
			rc=_vvscanf_getc(stv);
			if((rc=='x') || (rc=='X'))
			{
				rdx=16; rc=-1;
			}else
			{
				rdx=8;
			}
		}else
		{
			rdx=10;
		}
	}

	if(rc<0)
		rc=_vvscanf_getc(stv);
	
	while(rc>0)
	{
		r=999;
		if((rc>='0') && (rc<='9'))
			{ r=rc-'0'; }
		else if((rc>='A') && (rc<='Z'))
			{ r=10+(rc-'A'); }
		else if((rc>='a') && (rc<='z'))
			{ r=10+(rc-'A'); }
		if(r>=rdx)
			break;
		li=(li*rdx)+r;
		rc=_vvscanf_getc(stv);
	}

	if(rc>0)
		{ _vvscanf_ugetc(rc, stv); rc=-1; }
	
	if(neg)
		{ li=-li; }
	return(li);
}

double _vvscanf_readfloat(_stdio_printfstate_t *stv)
{
	double f, sc;
	int rc, neg, rdx, r, exneg, exv;
	
	rc=-1;
	neg=0;

	if(rc<0)
		rc=_vvscanf_getc(stv);
	while((rc>0) && (rc<=' '))
		rc=_vvscanf_getc(stv);
	if(rc=='-')
		{ neg=1; rc=-1; }

	if(rc<0)
		rc=_vvscanf_getc(stv);
		
	while(rc>0)
	{
		r=999;
		if((rc>='0') && (rc<='9'))
			{ r=rc-'0'; }
		else
			break;
		f=(f*10)+r;
		rc=_vvscanf_getc(stv);
	}
	
	if(rc=='.')
	{
		rc=_vvscanf_getc(stv);
		sc=0.1;

		while(rc>0)
		{
			if((rc>='0') && (rc<='9'))
				{ r=rc-'0'; }
			else
				break;
			f=f+(r*sc);
			sc=sc*0.1;
			rc=_vvscanf_getc(stv);
		}
	}

	if((rc=='e') || (rc=='E'))
	{
		rc=_vvscanf_getc(stv);
		
		exneg=0;
		if(rc=='-')
			{ exneg=1; rc=_vvscanf_getc(stv); }
		else if(rc=='+')
			rc=_vvscanf_getc(stv);
		
		exv=0;

		while((rc>='0') && (rc<='9'))
		{
			exv=(exv*10)+(rc-'0');
			rc=_vvscanf_getc(stv);
		}
		
		if(exneg)
		{
			while(exv>=3)
				{ f=f*0.001; exv-=3; }
			while(exv>0)
				{ f=f*0.1; exv--; }
		}else
		{
			while(exv>=3)
				{ f=f*1000; exv-=3; }
			while(exv>0)
				{ f=f*10; exv--; }
		}
	}

	if(rc>0)
		{ _vvscanf_ugetc(rc, stv); rc=-1; }
	
	if(neg)
		{ f=-f; }
	return(f);
}

int _vvscanf(
	_stdio_printfstate_t *stv,
	const char *format, __va_list arg)
{
	__int128	li, lj;
	double f, g;
	unsigned char *cs, *cs2;
	wchar_t *wcs2;
	void *ptr;
	int c, rc, prc, rdx, narg;
	int i, j, k;

	rc=-1; narg=0;
	cs=(unsigned char *)format;
	while(*cs)
	{
		c=cs[0];
		
		if(c<=' ')
		{
			cs++;
			if(rc<0)
				rc=_vvscanf_getc(stv);
			while((rc>0) && (rc<=' '))
				rc=_vvscanf_getc(stv);
			continue;
		}

		if(c!='%')
		{
			if(rc<0)
				rc=_vvscanf_getc(stv);
			if(rc!=c)
				break;
			rc=-1;
			cs++;
			continue;
		}
		_vvprintf_readspec(&cs, stv);

		if(rc>0)
			{ _vvscanf_ugetc(rc, stv); rc=-1; }
		
		switch(stv->fmt_conv)
		{
		case 'd':
		case 'i':
		case 'u':
		case 'o':
		case 'x':
		case 'X':
			rdx=0;
			if(stv->fmt_conv=='o')
				rdx=8;
			if((stv->fmt_conv=='x') || (stv->fmt_conv=='X'))
				rdx=16;
			
			li=_vvscanf_readint(stv, rdx);
			ptr=va_arg(arg, void *);
			switch(stv->fmt_lmod)
			{
				case 'l':	*(long *)ptr=li; break;
				case 'L':	*(long long *)ptr=li; break;
				case 'Q':	*(__int128 *)ptr=li; break;
				default:	*(int *)ptr=li; break;
			}
			narg++;
			break;
		case 'f':	case 'e':
		case 'g':	case 'E':
		case 'a':
			f=_vvscanf_readfloat(stv);
			ptr=va_arg(arg, void *);
			switch(stv->fmt_lmod)
			{
				case 'l':	*(double *)ptr=f; break;
				case 'L':	*(long double *)ptr=f; break;
				default:	*(float *)ptr=f; break;
			}
			narg++;
			break;

		case 'p':
			li=_vvscanf_readint(stv, 16);
			ptr=va_arg(arg, void *);
			*(long *)ptr=li;
			narg++;
			break;

		case 'c':
			if(rc<0)
				rc=_vvscanf_getc(stv);
			ptr=va_arg(arg, void *);
			*(int *)ptr=rc;
			rc=-1;
			narg++;
			break;

		case 's':
			cs2=va_arg(arg, void *);
			
			if(rc<0)
				rc=_vvscanf_getc(stv);
			while(rc>' ')
			{
				*cs2++=rc;
				rc=_vvscanf_getc(stv);
			}
			narg++;
			break;

		case 'n':
			ptr=va_arg(arg, void *);
			*(int *)ptr=(stv->ct-stv->cts);
			narg++;
			break;

		default:
			break;
		}
	}

	if(rc>0)
		{ _vvscanf_ugetc(rc, stv); rc=-1; }
	return(narg);
}

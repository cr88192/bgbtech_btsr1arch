#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <tkclib2/ci_stdio.h>

void *_malloc_cat(size_t size, int cat)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->malloc_cat(ctx, size, cat));
}

void *malloc(size_t size)
{
	return(_malloc_cat(size, 0));
}

void *calloc(size_t nmemb, size_t size)
{
	void *ptr;
	size_t sz1;
	
	sz1=nmemb*size;
	ptr=malloc(sz1);
	memset(ptr, 0, sz1);
	return(ptr);
}

void *realloc(void *ptr, size_t size)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->realloc(ctx, ptr, size));
}

void free(void *ptr)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	ctx->vt->free(ctx, ptr);
}

void abort(void)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	ctx->vt->abort(ctx);
}

void exit(int status)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	ctx->vt->exit(ctx, status);
}


static uint64_t _rand_seed;

void srand(unsigned int seed)
{
	_rand_seed = seed;
	_rand_seed = _rand_seed * 0x0000FC4BA2F7ACABULL + 1;
}

int rand(void)
{
	int ret;
	_rand_seed = _rand_seed * 0x0000FC4BA2F7ACABULL + 1;
	ret = (int)((_rand_seed >> 48) & 0x7FFF);
	return (ret);
}

int rand_r(unsigned int *seedp)
{
	unsigned int seed;
	int ret;
	seed=*seedp;
	seed=seed*65521+1;
	ret = (seed >> 16) & 0x7FFF;
	*seedp=seed;
	return (ret);
}

void _memswap(void *ptr1, void *ptr2, size_t size)
{
	uint64_t *csa, *csb;
	char *cs, *ct;
	uint64_t va, vb, vc, cd;
	int i, n;
	
	csa=ptr1;
	csb=ptr2;
	n=size;
	while(n>=16)
	{
		va=csa[0];		vb=csa[1];
		vc=csb[0];		vd=csb[1];
		n-=16;
		csa[0]=vc;		csa[1]=vd;
		cs)[0]=va;		csb[1]=vb;
		csa+=2;			csb+=2;
	}
	if(n>=8)
	{
		va=csa[0];		vb=csb[0];
		csa[0]=vb;		csb[0]=va;
		n-=8;
		csa++;			csb++;
	}
	if(n)
	{
		cs=(char *)csa;
		ct=(char *)csb;
		while(n--)
		{
			i=cs[0]; cs[0]=ct[0]; ct[0]=i;
			cs++; ct++;
		}
	}
}

void qsort(void *base,
			size_t nmemb,
			size_t size,
			int (*compar)(const void *a, const void *b))
{
	char *base2;
	size_t i,a,b,c;

	base2 = (char *)base;

	while (nmemb > 1)
	{
		a = 0;
		b = nmemb-1;
		c = (a+b)/2;
		while(1)
		{
			while ((*compar)(base2+(size*c), base2+(size*a)) > 0) 
				{ a++; }
			while ((*compar)(base2+(size*c), base2+(size*b)) < 0)
				{ b--; }
			if (a >= b)
				{ break; }
			_memswap(base2+(a*size), base2+(b*size), size);
			if (c == a)
				{ c = b; }
			else if (c == b)				
				{ c = a; }
			a++;
			b--;
		}
		b++;
		if (b < (nmemb-b))
		{
			qsort(base2,b,size,compar);
			base2+=(size*b);
			nmemb=nmemb-b;
		}
		else
		{
			qsort(base2+(size*b), nmemb-b, size, compar);
			nmemb=b;
		}
	}
	return;
}


double strtod(const char *nptr, char **endptr)
{
	double f, sc;
	int neg, c, r;
	
	neg=0;
	while(*nptr && (*nptr<=' '))
		{ nptr++; }
	if(*nptr=='-')
		{ neg=1; nptr++; }
	
	f=0; c=*nptr;
	while(c && (c>='0') && (c<='9'))
	{
		r=c-'0';
		c=nptr[1];
		nptr++;
		f=(f*10)+r;
	}
	
	if(c=='.')
	{
		c=nptr[1];
		nptr++;
		sc=0.1;

		while(c && (c>='0') && (c<='9'))
		{
			r=c-'0';
			c=nptr[1];
			nptr++;
			f=f+(r*sc);
			sc=sc*0.1;
		}
	}
	
	if(neg)
		f=-f;

	if(endptr)
		*endptr=nptr;
	return(f);
}

unsigned long long int strtoull(
	const char *nptr, char **endptr, int base)
{
	unsigned long long li;
	int c, r;

	if(base==0)
	{
		if(nptr=='0')
		{
			if((nptr[1]=='x') || (nptr[1]=='X'))
				{ nptr+=2; base=16; }
			else
				{ nptr++; base=8; }
		}else
		{
			base=10;
		}
	}
	
	if(base==10)
	{
		c=*nptr;
		li=0;
		while(c && (c>='0') && (c<='9'))
		{
			r=c-'0';
			c=nptr[1];
			nptr++;
			li=(li*10)+r;
		}
		if(endptr)
			*endptr=nptr;
		return(li);
	}

	c=*nptr;
	li=0;
	while(c)
	{
		if((c>='0') && (c<='9'))
			{ r=c-'0'; }
		else if((c>='A') && (c<='Z'))
			{ r=10+(c-'A'); }
		else if((c>='a') && (c<='z'))
			{ r=10+(c-'a'); }
		else
			{ r=99; }
		if(r>=base)
			break;
	
		c=nptr[1];
		nptr++;
		li=(li*base)+r;
	}
	if(endptr)
		*endptr=nptr;
	return(li);
}

unsigned long int strtoul(const char *nptr, char **endptr, int base)
{
	return(strtoull(nptr, endptr, base));
}

long int strtol(const char *nptr, char **endptr, int base)
{
	unsigned long y;
	long x;
	int neg = 0;

	while (isspace((unsigned char)*nptr))
		{ nptr++; }
	if (*nptr == '-')
		{ neg = 1; nptr++; }
	else if (*nptr == '+')
		{ nptr++; }
	y = strtoul(nptr, endptr, base);
	if (neg)
		{ x = -y; }
	else
		{ x = y; }
	return (x);
}

double atof(const char *nptr)
{
	return (strtod(nptr, (char **)NULL));
}

int atoi(const char *nptr)
{
	return ((int)strtol(nptr, (char **)NULL, 10));
}

long int atol(const char *nptr)
{
	return (strtol(nptr, (char **)NULL, 10));
}

int mblen(const char *s, size_t n)
{
	int i0, i1, i2;

	if (!s)
	{
		return (0);
	}

	if (n >= 1)
	{
#if 1
//		if(locale_flags&TK_LOCALE_FLAG_UTF8)
		if(1)
		{
			if(!(*s))
				return(0);
			i0=*(const unsigned char *)s;
			if(i0<0x80)
				return(1);

			if(i0<0xC0)
				return(-1);

			if(i0<0xE0)
			{
				if(n>=2)
				{
					i1=*(const unsigned char *)(s+1);
					if((i1&0xC0)!=0x80)
						return(-1);
					return(2);
				}
				return(-1);
			}

			if(i0<0xF0)
			{
				if(n>=3)
				{
					i1=*(const unsigned char *)(s+1);
					i2=*(const unsigned char *)(s+2);
					if((i1&0xC0)!=0x80)
						return(-1);
					if((i2&0xC0)!=0x80)
						return(-1);
					return(3);
				}
				return(-1);
			}

			return(-1);
		}
#endif

		return (1);
	}

	return (-1);
}

int mbtowc(wchar_t *pwc, const char *s, size_t n);
int wctomb(char *s, wchar_t wchar);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);

int abs(int j)
{
	if(j<0)
		j=-j;
	return(j);
}

long int labs(long int j)
{
	if(j<0)
		j=-j;
	return(j);
}

div_t div(int numer, int denom)
{
	div_t d;
	int q, r;
	
	q=numer/denom;
	r=numer-(q*denom);
	d.quot=q;
	d.rem=r;
	return(d);
}

ldiv_t ldiv(long int numer, long int denom)
{
	ldiv_t d;
	long q, r;
	q=numer/denom;
	r=numer-(q*denom);
	d.quot=q;
	d.rem=r;
	return(d);
}

void *bsearch(const void *key, const void *base,
              size_t nmemb, size_t size,
              int (*compar)(const void *a, const void *b))
{
	size_t mid;
	int res;
	const char *base2;
	const char *ptr;

	base2=base;
	while (nmemb > 0)
	{
		mid = nmemb / 2;
		ptr = base2 + (mid * size);
		res = compar(ptr, key);
		if (res == 0)
			{ return ((void *)ptr); }
		else if (res < 0)
		{
			nmemb = nmemb - mid - 1;
			base2 = (ptr + size);
		}
		else
			{ nmemb = mid; }
	}
	return (NULL);
}


int atexit(void (*func)(void));
char *getenv(const char *name);
int system(const char *string);

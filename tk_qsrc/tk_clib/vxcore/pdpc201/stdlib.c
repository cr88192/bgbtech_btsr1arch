/*********************************************************************/
/*																	*/
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																	*/
/*********************************************************************/
/*********************************************************************/
/*																	*/
/*  stdlib.c - implementation of stuff in stdlib.h					*/
/*																	*/
/*********************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "string.h"
#include "ctype.h"
#include "stddef.h"

#include "locale.h"
#include "tk_locale.h"

/* PDOS and MSDOS use the same interface most of the time */
#if defined(__PDOS__)
#define __MSDOS__
#endif

#undef __MSDOS__


#ifdef __OS2__
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#if defined(__MVS__) || defined(__CMS__)
#include "mvssupa.h"
#endif

#ifdef __MVS__
extern int __tso;
#endif

#if USE_MEMMGR
#include "__memmgr.h"
#define MAX_CHUNK 30000000 /* maximum size we will store in memmgr */
	/* Note that you can set MAX_CHUNK to less than REQ_CHUNK */
	/* But don't do this until MVS/380 has been fixed */
#define REQ_CHUNK 30000000 /* size that we request from OS */
void *__lastsup = NULL; /* last thing supplied to memmgr */
#endif

void __allocmem(size_t size, void **ptr);
void __freemem(void *ptr);
unsigned char *__envptr;
void __exec(char *cmd, void *env);



void (*__userExit[__NATEXIT])(void);


#if 0
__PDPCLIB_API__ void *malloc(size_t size)
{
	void *ptr;
	__allocmem(size, &ptr);
	return (ptr);
}


__PDPCLIB_API__ void free(void *ptr)
{
	if (ptr != NULL)
	{
		__freemem(ptr);
	}
	return;
}

__PDPCLIB_API__ void *realloc(void *ptr, size_t size)
{
	char *newptr;
	size_t oldsize;

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}

	newptr = malloc(size);
	if (newptr == NULL)
	{
		return (NULL);
	}
	if (ptr != NULL)
	{
		oldsize = *((size_t *)ptr - 1);
		if (oldsize < size)
		{
			size = oldsize;
		}
		memcpy(newptr, ptr, size);
		free(ptr);
	}
	return (newptr);
}
#endif



#if 1

void *(*_malloc_fptr)(size_t size, int cat);
void (*_free_fptr)(void *ptr);
void *(*_realloc_fptr)(void *ptr, size_t size);
size_t (*_msize_fptr)(void *ptr);

__PDPCLIB_API__ void **_getmallocptr(void)
	{ return(&_malloc_fptr); }
__PDPCLIB_API__ void **_getfreecptr(void)
	{ return(&_free_fptr); }
__PDPCLIB_API__ void **_getreallocptr(void)
	{ return(&_realloc_fptr); }
__PDPCLIB_API__ void **_getmsizeptr(void)
	{ return(&_msize_fptr); }

// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__TK_CLIB_DLLSTUB__)
// #if 0

__PDPCLIB_API__ void *malloc(size_t size);
__PDPCLIB_API__ void *_malloc_cat(size_t size, int cat);
__PDPCLIB_API__ size_t _msize(void *ptr);
__PDPCLIB_API__ size_t malloc_usable_size(void *ptr);
__PDPCLIB_API__ void free(void *ptr);
__PDPCLIB_API__ void *realloc(void *ptr, size_t size);

__asm {
malloc:
	MOV		0, R5
	MOV.Q	_malloc_fptr, R3
	JMP		R3

_malloc_cat:
	MOV.Q	_malloc_fptr, R3
	JMP		R3

_msize:
	MOV.Q	_msize_fptr, R3
	JMP		R3

malloc_usable_size:
	MOV.Q	_msize_fptr, R3
	JMP		R3

free:
	MOV.Q	_free_fptr, R3
	JMP		R3

realloc:
	MOV.Q	_realloc_fptr, R3
	JMP		R3
};

#else

__PDPCLIB_API__ void *malloc(size_t size)
{
	stdio_doinit();
	return(_malloc_fptr(size, 0));
}

__PDPCLIB_API__ void *_malloc_cat(size_t size, int cat)
{
	stdio_doinit();
	return(_malloc_fptr(size, cat));
}

__PDPCLIB_API__ size_t _msize(void *ptr)
{
	return(_msize_fptr(ptr));
}

__PDPCLIB_API__ size_t malloc_usable_size(void *ptr)
{
	return(_msize_fptr(ptr));
}

__PDPCLIB_API__ void free(void *ptr)
{
	_free_fptr(ptr);
}

__PDPCLIB_API__ void *realloc(void *ptr, size_t size)
{
	stdio_doinit();
	return(_realloc_fptr(ptr, size));
}

#endif

#ifndef __TK_CLIB_DLLSTUB__

void *malloc_dfl(size_t size, int cat)
{
	void *ptr;
	__allocmem(size, &ptr);
	return (ptr);
}


void free_dfl(void *ptr)
{
	if (ptr != NULL)
	{
		__freemem(ptr);
	}
	return;
}

void *realloc_dfl(void *ptr, size_t size)
{
	char *newptr;
	size_t oldsize;

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}

	newptr = malloc(size);
	if (newptr == NULL)
	{
		return (NULL);
	}
	if (ptr != NULL)
	{
		oldsize = *((size_t *)ptr - 1);
		if (oldsize < size)
		{
			size = oldsize;
		}
		memcpy(newptr, ptr, size);
		free(ptr);
	}
	return (newptr);
}
#endif

#endif

__PDPCLIB_API__ void *calloc(size_t nmemb, size_t size)
{
	void *ptr;
	size_t total;

	if (nmemb == 1)
	{
		total = size;
	}
	else if (size == 1)
	{
		total = nmemb;
	}
	else
	{
		total = nmemb * size;
	}
	ptr = malloc(total);
	if (ptr != NULL)
	{
//		memset(ptr, '\0', total);
		memset(ptr, 0, total);
	}
	return (ptr);
}


__PDPCLIB_API__ void abort(void)
{
	raise(SIGABRT);
	exit(EXIT_FAILURE);
}

#if !defined(__EMX__) && !defined(__GCC__) && !defined(__WIN32__) \
  && !defined(__gnu_linux__)
void __exit(int status);
#else
void __exit(int status) __attribute__((noreturn));
#endif

__PDPCLIB_API__ void exit(int status)
{
	__exit(status);
#if !defined(__EMX__) && !defined(__GCC__) && !defined(__WIN32__) \
  && !defined(__gnu_linux__)
	return;
#endif
}

/* This qsort routine was obtained from libnix (also public domain),
 * and then reformatted.
 *
 * This qsort function does a little trick:
 * To reduce stackspace it iterates the larger interval instead of doing
 * the recursion on both intervals. 
 * So stackspace is limited to 32*stack_for_1_iteration = 
 * 32*4*(4 arguments+1 returnaddress+11 stored registers) = 2048 Bytes,
 * which is small enough for everybodys use.
 * (And this is the worst case if you own 4GB and sort an array of chars.)
 * Sparing the function calling overhead does improve performance, too.
 */

__PDPCLIB_API__ void qsort(void *base,
			size_t nmemb,
			size_t size,
			int (*compar)(const void *, const void *))
{
	char *base2 = (char *)base;
	size_t i,a,b,c;
  
	while (nmemb > 1)
	{
		a = 0;
		b = nmemb-1;
		c = (a+b)/2; /* Middle element */
		for (;;)
		{
			while ((*compar)(&base2[size*c],&base2[size*a]) > 0) 
			{
				a++; /* Look for one >= middle */
			}
			while ((*compar)(&base2[size*c],&base2[size*b]) < 0)
			{
				b--; /* Look for one <= middle */
			}
			if (a >= b)
			{
				break; /* We found no pair */
			}
			for (i=0; i<size; i++) /* swap them */
			{
				char tmp=base2[size*a+i];

				base2[size*a+i]=base2[size*b+i];
				base2[size*b+i]=tmp;
			}
			if (c == a) /* Keep track of middle element */
			{
				c = b;
			}
			else if (c == b)				
			{
				c = a;
			}
			a++; /* These two are already sorted */
			b--;
		} /* a points to first element of right interval now 
			 (b to last of left) */
		b++;
		if (b < nmemb-b) /* do recursion on smaller interval and 
							iteration on larger one */
		{
			qsort(base2,b,size,compar);
			base2=&base2[size*b];
			nmemb=nmemb-b;
		}
		else
		{
			qsort(&base2[size*b],nmemb-b,size,compar);
			nmemb=b;
		}
	}
	return;
}


// static unsigned long myseed = 1;
static unsigned long long myseed = 1;

__PDPCLIB_API__ void srand(unsigned int seed)
{
	myseed = seed;
	return;
}

__PDPCLIB_API__ int rand(void)
{
	int ret;

//	myseed = myseed * 1103515245UL + 12345;
//	myseed = myseed * 65521UL + 1;
//	myseed = myseed * 0x0000F7A83CF9E588ULL + 1;
	myseed = myseed * 0x0000FC4BA2F7ACABULL + 1;
//	ret = (int)((myseed >> 16) & 0x8fff);
//	ret = (int)((myseed >> 16) & 0x7fff);
	ret = (int)((myseed >> 48) & 0x7fff);
	return (ret);
}

__PDPCLIB_API__ double atof(const char *nptr)
{
	return (strtod(nptr, (char **)NULL));
}

__PDPCLIB_API__ double strtod(const char *nptr, char **endptr)
{
	double x = 0.0;
	double xs= 1.0;
	double es = 1.0;
	double xf = 0.0;
	double xd = 1.0;

	while( isspace( (unsigned char)*nptr ) ) ++nptr;
	if(*nptr == '-')
	{
		xs = -1;
		nptr++;
	}
	else if(*nptr == '+')
	{
		nptr++;
	}


	while (1)
	{
		if (isdigit((unsigned char)*nptr))
		{
			x = x * 10 + (*nptr - '0');
			nptr++;
		}
		else
		{
			x = x * xs;
			break;
		}
	}
	if (*nptr == '.')
	{
		nptr++;
		while (1)
		{
			if (isdigit((unsigned char)*nptr))
			{
				xf = xf * 10 + (*nptr - '0');
				xd = xd * 10;
			}
			else
			{
				x = x + xs * (xf / xd);
				break;
			}
			nptr++;
		}
	}
	if ((*nptr == 'e') || (*nptr == 'E'))
	{
		nptr++;
		if (*nptr == '-')
		{
			es = -1;
			nptr++;
		}
		xd = 1;
		xf = 0;
		while (1)
		{
			if (isdigit((unsigned char)*nptr))
			{
				xf = xf * 10 + (*nptr - '0');
				nptr++;
			}
			else
			{
				while (xf > 0)
				{
					xd *= 10;
					xf--;
				}
				if (es < 0.0)
				{
					x = x / xd;
				}
				else
				{
					x = x * xd;
				}
				break;
			}
		}
	}
	if (endptr != NULL)
	{
		*endptr = (char *)nptr;
	}
	return (x);
}

__PDPCLIB_API__ int atoi(const char *nptr)
{
	return ((int)strtol(nptr, (char **)NULL, 10));
}

__PDPCLIB_API__ long int atol(const char *nptr)
{
	return (strtol(nptr, (char **)NULL, 10));
}

/* this logic is also in vvscanf - if you update this, update
	that one too */

__PDPCLIB_API__ unsigned long int strtoul(
	const char *nptr, char **endptr, int base)
{
	unsigned long x = 0;
	int undecided = 0;

	if (base == 0)
	{
		undecided = 1;
	}
	while (isspace((unsigned char)*nptr))
	{
		nptr++;
	}
	while (1)
	{
		if (isdigit((unsigned char)*nptr))
		{
			if (base == 0)
			{
				if (*nptr == '0')
				{
					base = 8;
				}
				else
				{
					base = 10;
					undecided = 0;
				}
			}
			x = x * base + (*nptr - '0');
			nptr++;
		}
		else if (isalpha((unsigned char)*nptr))
		{
			if ((*nptr == 'X') || (*nptr == 'x'))
			{
				if ((base == 0) || ((base == 8) && undecided))
				{
					base = 16;
					undecided = 0;
					nptr++;
				}
				else if (base == 16)
				{
					/* hex values are allowed to have an optional 0x */
					nptr++;
				}
				else
				{
					break;
				}
			}
			else if (base <= 10)
			{
				break;
			}
			else
			{
				x = x * base + (toupper((unsigned char)*nptr) - 'A') + 10;
				nptr++;
			}
		}
		else
		{
			break;
		}
	}
	if (endptr != NULL)
	{
		*endptr = (char *)nptr;
	}
	return (x);
}

__PDPCLIB_API__ long int strtol(const char *nptr, char **endptr, int base)
{
	unsigned long y;
	long x;
	int neg = 0;

	while (isspace((unsigned char)*nptr))
	{
		nptr++;
	}
	if (*nptr == '-')
	{
		neg = 1;
		nptr++;
	}
	else if (*nptr == '+')
	{
		nptr++;
	}
	y = strtoul(nptr, endptr, base);
	if (neg)
	{
		x = (long)-y;
	}
	else
	{
		x = (long)y;
	}
	return (x);
}

__PDPCLIB_API__ int mblen(const char *s, size_t n)
{
	int i0, i1, i2;
	if (s == NULL)
	{
		return (0);
	}
//	if (n == 1)
	if (n >= 1)
	{
#if 1
		if(locale_flags&TK_LOCALE_FLAG_UTF8)
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

static short tk_cp1252rm0[32]={
	0x20AC,0x0081,0x201A,0x0192,	0x201E,0x2026,0x2020,0x2021,
	0x02C6,0x2030,0x0160,0x2039,	0x0152,0x008D,0x017D,0x008F,
	0x0090,0x2018,0x2019,0x201C,	0x201D,0x2022,0x2013,0x2014,
	0x02DC,0x2122,0x0161,0x203A,	0x0153,0x009D,0x017E,0x0178 };

__PDPCLIB_API__ int mbtowc(wchar_t *pwc, const char *s, size_t n)
{
	const unsigned char *us;
	int i0, i1, i2;
	int j;

	if (s == NULL)
	{
		return (0);
	}
//	if (n == 1)
	if (n >= 1)
	{
		if(!pwc)
			return (-1);

		us=(const unsigned char *)s;
		i0=*us;
		if(i0<=0x80)
		{
			*pwc=i0;
			return(1);
		}

		if(locale_flags&TK_LOCALE_FLAG_CP1252)
		{
			if((i0>=0xA0) && (i0<=0xFF))
			{
				*pwc=i0;
				return(1);
			}

			if((i0>=0x80) && (i0<=0x9F))
			{
				*pwc=tk_cp1252rm0[i0-0x80];
				return(1);
			}

			return (-1);
		}

#if 1
		if(locale_flags&TK_LOCALE_FLAG_UTF8)
		{
			if(i0<=0xC0)
				{ return(-1); }
			if(i0<=0xE0)
			{
				i1=us[1];
				j=((i0&0x1F)<<6)|(i1&0x3F);
				*pwc=j;
				return(2);
			}
			if(i0<=0xF0)
			{
				i1=us[1];
				i2=us[2];
				j=((i0&0x0F)<<12)|((i1&0x3F)<<6)|(i2&0x3F);
				*pwc=j;
				return(3);
			}
		}
	
//		if (pwc != NULL)
//		{
//			*pwc = *s;
//		}
//		return (1);
#endif

	}
	else
	{
		return (-1);
	}
}

__PDPCLIB_API__ int wctomb(char *s, wchar_t wc)
{
	unsigned char *us;
	int i;

	if (s != NULL)
	{
		us = (unsigned char *)s;

		if(locale_flags&TK_LOCALE_FLAG_CP1252)
		{
			if(wc<0x0080)
			{
				*us = wc;
				return (1);
			}

			if((wc>=0x00A0) && (wc<0x0100))
			{
				*us = wc;
				return (1);
			}
			
			for(i=0; i<32; i++)
				if(wc==tk_cp1252rm0[i])
					break;
			if(i<32)
			{
				*us = 0x80+i;
				return (1);
			}

			*us = 0x8F;
			return (1);
			
//			return(0);
		}

#if 1
		if(locale_flags&TK_LOCALE_FLAG_UTF8)
		{
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

			if(1)
			{
				us[0] = 0xE0 | ((wc>>12)&0x0F);
				us[1] = 0x80 | ((wc>> 6)&0x3F);
				us[2] = 0x80 | ((wc>> 0)&0x3F);
				return (3);
			}
		}
#endif

	}
	else
	{
		return (0);
	}
}

__PDPCLIB_API__ size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n)
{
	const unsigned char *cs;
	wchar_t *ct, *cte;
	int i;
	
	cs = (const unsigned char *)s;
	ct = pwcs;
	cte = ct + (n-1);
	
	while(*cs && (ct<cte))
	{
		i=mbtowc(ct, cs, 9);
		if(i<=0)
			break;
		cs+=i;
		ct++;
	}
	*ct=0;
	return(ct-pwcs);

//	strncpy((char *)pwcs, s, n);
//	if (strlen(s) >= n)
//	{
//		return (n);
//	}
//	return (strlen((char *)pwcs));
}

__PDPCLIB_API__ size_t wcstombs(char *s, const wchar_t *pwcs, size_t n)
{
	char tb[4];
	unsigned char *ct, *cte;
	const wchar_t *cs;
	int i, l;
	
	if(!s)
	{
		cs = pwcs; l=0;
		while(*cs)
		{
			i=wctomb(tb, *cs++);
			if(i<=0)
				break;
			l+=i;
		}
		return(l);
	}
	
	cs = pwcs;
	ct = (unsigned char *)s;
	cte = ct + (n-1);

	while(*cs && (ct<cte))
	{
		i=wctomb(ct, *cs++);
		if(i<=0)
			break;
		ct+=i;
	}

	*ct=0;
	return(ct-s);


//	strncpy(s, (const char *)pwcs, n);
//	if (strlen((const char *)pwcs) >= n)
//	{
//		return (n);
//	}
//	return (strlen(s));
}

#ifdef abs
#undef abs
#endif

#ifdef labs
#undef labs
#endif

#ifdef __BJX2__
// #if 0

__PDPCLIB_API__ int abs(int j);
__PDPCLIB_API__ long int labs(long int j);

__asm {
abs:
	MOV		R4, R2
	CMPGT	0, R4
	NEG?F	R2, R2
	RTS

labs:
	MOV		R4, R2
	CMPQGT	0, R4
	NEG?F	R2, R2
	RTS
};

#else

__PDPCLIB_API__ int abs(int j)
{
	if (j < 0)
	{
		j = -j;
	}
	return (j);
}

__PDPCLIB_API__ long int labs(long int j)
{
	if (j < 0)
	{
		j = -j;
	}
	return (j);
}

#endif

__PDPCLIB_API__ div_t div(int numer, int denom)
{
	div_t x;

	x.quot = numer / denom;
	x.rem = numer % denom;
	return (x);
}

__PDPCLIB_API__ ldiv_t ldiv(long int numer, long int denom)
{
	ldiv_t x;

	x.quot = numer / denom;
	x.rem = numer % denom;
	return (x);
}

__PDPCLIB_API__ int atexit(void (*func)(void))
{
	int x;

	for (x = 0; x < __NATEXIT; x++)
	{
		if (__userExit[x] == 0)
		{
			__userExit[x] = func;
			return (0);
		}
	}
	return (-1);
}

__PDPCLIB_API__ char *getenv(const char *name)
{
#ifdef __OS2__
	PSZ result;

	if (DosScanEnv((void *)name, (void *)&result) == 0)
	{
		return ((char *)result);
	}
#endif
#if defined(__MSDOS__) || defined(__WIN32__)
	char *env;
	size_t lenn;

#ifdef __WIN32__
	env = GetEnvironmentStrings();
#else
	env = (char *)__envptr;
#endif
	lenn = strlen(name);
	while (*env != '\0')
	{
		if (strncmp(env, name, lenn) == 0)
		{
			if (env[lenn] == '=')
			{
				return (&env[lenn + 1]);
			}
		}
		env = env + strlen(env) + 1;
	}
#endif
	return (NULL);
}

/* The following code was taken from Paul Markham's "EXEC" program,
	and adapted to create a system() function.  The code is all
	public domain */

__PDPCLIB_API__ int system(const char *string)
{
#ifdef __OS2__
	char err_obj[100];
	APIRET rc;
	RESULTCODES results;

	if (string == NULL)
	{
		return (1);
	}
	rc = DosExecPgm(err_obj, sizeof err_obj, EXEC_SYNC,
					(PSZ)string, NULL, &results, (PSZ)string);
	if (rc != 0)
	{
		return (rc);
	}
	return ((int)results.codeResult);
#endif
#ifdef __WIN32__
	BOOL rc;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	DWORD ExitCode;

	memset(&si, 0, sizeof si);
	si.cb = sizeof si;
	memset(&pi, 0, sizeof pi);
	rc = CreateProcess(NULL,
						(char *)string,
						NULL,
						NULL,
						FALSE,
						0,
						NULL,
						NULL,
						&si,
						&pi);
	if (!rc)
	{
		return (GetLastError());
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &ExitCode);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return (ExitCode);
#endif
#if defined(__MSDOS__) && !defined(__VX86__)
	static unsigned char cmdt[140];
	static struct {
		int env;
		unsigned char *cmdtail;
		char *fcb1;
		char *fcb2;
	} parmblock = { 0, cmdt, NULL, NULL };
	size_t len;
	char *cmd;

	if (string == NULL)
	{
		return (1);
	}
	len = strlen(string);
	cmdt[0] = (unsigned char)(len + 3);
	memcpy(&cmdt[1], "/c ", 3);
	memcpy(&cmdt[4], string, len);
	memcpy(&cmdt[len + 4], "\r", 2);
	cmd = getenv("COMSPEC");
	if (cmd == NULL)
	{
		cmd = "\\command.com";
	}
	__exec(cmd, &parmblock);
	return (0);
#endif
#if defined(__MVS__)
	char pgm[9];
	size_t pgm_len;
	size_t cnt;
	char *p;
	
	p = strchr(string, ' ');
	if (p == NULL)
	{
		p = strchr(string, '\0');
	}
	
	pgm_len = p - string;
	/* don't allow a program name greater than 8 */
	
	if (pgm_len > 8)
	{
		return (-1);
	}
	memcpy(pgm, string, pgm_len);
	pgm[pgm_len] = '\0';
	
	/* uppercase the program name */
	for (cnt = 0; cnt < pgm_len; cnt++)
	{
		pgm[cnt] = toupper((unsigned char)pgm[cnt]);
	}
	
	/* point to parms */
	if (*p != '\0')
	{
		p++;
	}
	
	/* all parms now available */
	/* we use 1 = batch or 2 = tso */
	return (__system(__tso ? 2: 1, pgm_len, pgm, strlen(p), p));
#endif
#if defined(__VX86__)
	__exec(string, NULL);
	return(0);
#endif
#if defined(__CMS__)
	/* not implemented yet */
	return (0);
#endif
}

__PDPCLIB_API__ void *bsearch(const void *key, const void *base,
			  size_t nmemb, size_t size,
			  int (*compar)(const void *, const void *))
{
	size_t try;
	int res;
	const void *ptr;

	while (nmemb > 0)
	{
		try = nmemb / 2;
		ptr = (void *)((char *)base + try * size);
		res = compar(ptr, key);
		if (res == 0)
		{
			return ((void *)ptr);
		}
		else if (res < 0)
		{
			nmemb = nmemb - try - 1;
			base = (const void *)((const char *)ptr + size);
		}
		else
		{
			nmemb = try;
		}
	}
	return (NULL);
}

__PDPCLIB_API__ char *strdup(char *str)
{
	char *s1;
	int l;
	
	l=strlen(str);
	s1=malloc(l+1);
	memcpy(s1, str, l+1);
	return(s1);
}

__PDPCLIB_API__ char *strndup(char *str, size_t n)
{
	char *s1;
	int l;

	l=strlen(str);
	if(l<n)n=l;
	s1=malloc(n+1);
	memcpy(s1, str, n);
	s1[n]=0;
	return(s1);
}

#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "string.h"
#include "ctype.h"
#include "stddef.h"

#include "wchar.h"

#include "locale.h"
#include "tk_locale.h"


static const char *itoa_radix="0123456789abcdefghijklmnopqrstuvwxyz";

char *lltoa(long long value, char *str, int base)
{
	char tb[128];
	char *s, *t;
	unsigned long long uv;
	int sg, v, v1;

	if(base==10)
	{
		v=value; sg=0;
		if(v<0)
			{ v=-v; sg=1; }
		s=tb;
		while(v || (s==tb))
		{
			v1=v/10;
			*s++=itoa_radix[v-(v1*10)];
			v=v1;
		}
		if(sg)*s++='-';
		
		t=str;
		while(s!=tb)
			*t++=*(--s);
		*t++=0;
		return(str);
	}

	uv=value;
	s=tb;
	while(uv || (s==tb))
	{
		v1=uv/base;
		*s++=itoa_radix[v-(v1*base)];
		v=v1;
	}
	
	t=str;
	while(s!=tb)
		*t++=*(--s);
	*t++=0;
	return(str);
}

char *itoa(int value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *ltoa(long value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *ultoa(unsigned long value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *strupr (char *str)
{
	char *s;
	s=str;
	while (*s) { *s = toupper(*s); s++; }
	return(str);
}

int unlink(const char *path)
{
	return(tk_unlink(path));
}

char *getcwd(char *buf, size_t size)
{
	return(TK_Env_GetCwd(buf, size));
}

#if 1
void MemLzCpy_MatchCopy2(uint8_t *dst, long sz, long d)
{
	uint8_t *cs, *ct, *cte;
	uint64_t v;
	
	if(d<8)
	{
		if(d==1)
		{
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(uint16_t *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(uint32_t *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(uint64_t *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
	}else
		if(sz<=32)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
		((uint64_t *)dst)[2]=((uint64_t *)cs)[2];
		((uint64_t *)dst)[3]=((uint64_t *)cs)[3];
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			((uint64_t *)ct)[0]=((uint64_t *)cs)[0];
			((uint64_t *)ct)[1]=((uint64_t *)cs)[1];
			ct+=16; cs+=16;
		}
	}
}
#endif

#if 1
void *_memlzcpy(void *dst, void *src, size_t n)
{
	long d;
	
	d=((char *)dst)-((char *)src);
	if(d<=0)
	{
		/* Copying backwards, use normal copy. */
		if((-d)>=n)
			{ memcpy(dst, src, n); }
		else if(d!=0)
			{ memmove(dst, src, n); }
	}else if(d>=n)
	{
		/* No overlap, use memcpy. */
		memcpy(dst, src, n);
	}else
	{
		MemLzCpy_MatchCopy2(dst, n, d);
	}
	return(dst);
}
#endif


constraint_handler_t global_constraint_handler;

constraint_handler_t set_constraint_handler_s( constraint_handler_t handler )
{
	constraint_handler_t old;
	
	if(!handler)
		handler = ignore_handler_s;
	
	old = global_constraint_handler;
	global_constraint_handler = handler;
	return(old);
}

errno_t call_constraint_handler_s( const char * msg, void *ptr, errno_t error)
{
	global_constraint_handler(msg, ptr, error);
	return(error);
}

void abort_handler_s( const char * msg, void *ptr, errno_t error)
{
	fprintf(stderr, "abort_handler_s called: %s %p %d\n",
		msg, ptr, error);
	abort();
}

void ignore_handler_s( const char * msg, void *ptr, errno_t error)
{
	/* does nothing. */
}


size_t strnlen_s( const char *str, size_t strsz )
{
	const char *p, *pe;
	int n;

	if(!str)
		return(0);

	p = str;	pe = str + strsz;	n = 0;
	while(*p && p<pe) { p++; n++; }
	return(n);
}

errno_t memcpy_s(
	void *restrict dest, rsize_t destsz,
	const void *restrict src, rsize_t count )
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(count > destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	memcpy(dest, src, count);
	return(0);
}

errno_t memmove_s(void *dest, rsize_t destsz, const void *src, rsize_t count)
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(count > destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	memmove(dest, src, count);
	return(0);
}

errno_t wmemcpy_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src, rsize_t count )
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(count > destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	memcpy(dest, src, count * sizeof(wchar_t));
	return(0);
}

errno_t wmemmove_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src, rsize_t count )
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(count > destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	memmove(dest, src, count * sizeof(wchar_t));
	return(0);
}

errno_t strcat_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src)
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if((strlen(dest) + strlen(src)) >= destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	strcat(dest, src);
	return(0);
}

errno_t strcpy_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src)
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(strlen(src) >= destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	strcpy(dest, src);
	return(0);
}

errno_t strncat_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src, rsize_t count)
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if((strlen(dest) + strnlen_s(src, count)) >= destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	strncat(dest, src, count);
	return(0);
}

errno_t strncpy_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src, rsize_t count)
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(strnlen_s(src, count) >= destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	strncpy(dest, src, count);
	return(0);
}

errno_t wcscpy_s(
	wchar_t *restrict dest, rsize_t destsz,
	const wchar_t *restrict src )
{
	if(!dest || !src)
		{ return(call_constraint_handler_s("null argument", NULL, EFAULT)); }
	if(strlen(src) >= destsz)
		{ return(call_constraint_handler_s("bad size argument", ERANGE)); }
	wcscpy(dest, src);
	return(0);
}

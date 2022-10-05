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
static const char *itoa_radix_b="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

char *xlltoa_cnt(__int128 value, char *str, int base, int cnt)
{
	char tb[160];
	char *s, *t, *rdx;
//	unsigned long long uv;
	unsigned __int128 uv;
	int sg, v, v1, c;
	
	rdx=itoa_radix;
	if(base&64)
		rdx=itoa_radix_b;
	base&=63;

	if(base==10)
	{
		v=value; sg=0;
		if(v<0)
			{ v=-v; sg=1; }
		s=tb; c=cnt;
		while(v || (c>0) || (s==tb))
		{
			v1=v/10;
			*s++=rdx[v-(v1*10)];
			v=v1;
			c--;
		}
		if(sg)*s++='-';
		
		t=str;
		while(s!=tb)
			*t++=*(--s);
		*t++=0;
		return(str);
	}

	uv=value;
	s=tb; c=cnt;
	while(uv || (c>0) || (s==tb))
	{
		v1=uv/base;
		*s++=rdx[v-(v1*base)];
		v=v1;
		c--;
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

void MemLzCpy_MatchCopy2(uint8_t *dst, long sz, long d);

#if 1
uint8_t *MemLzCpy_MatchCopy2(uint8_t *dst, long sz, long d)
{
	uint8_t *cs, *ct, *cte;
	uint64_t *ctl, *csl;
	uint64_t v, v0, v1, v2, v3;
	int step;
	
	if(d<8)
	{
		if(d==1)
		{
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);
			step=8;
		}else
			if(d==2)
		{
			v=*(uint16_t *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);
			step=8;
		}else
			if(d==4)
		{
			v=*(uint32_t *)(dst-d);
			v=v|(v<<32);
			step=8;
		}else
			if(d==8)
		{
			v=*(uint64_t *)(dst-d);
			step=8;
		}else
		{
			v=*(uint64_t *)(dst-d);
			step=d;
			if(step<=4)
			{
				*(uint64_t *)dst=v;
				v=*(uint64_t *)(dst-d);
				step+=step;
			}
		}

		ct=dst; cte=dst+sz;
		if(step==8)
		{
#if 1
			while((ct+64)<cte)
			{
				ctl=(uint64_t *)ct;
				ctl[0]=v;	ctl[1]=v;
				ctl[2]=v;	ctl[3]=v;
				ctl[4]=v;	ctl[5]=v;
				ctl[6]=v;	ctl[7]=v;
				ct+=64;
			}
#endif

			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
		{
#if 1
			while((ct+64)<cte)
			{
				*(uint64_t *)ct=v;	ct+=step;
				*(uint64_t *)ct=v;	ct+=step;
				*(uint64_t *)ct=v;	ct+=step;
				*(uint64_t *)ct=v;	ct+=step;
				*(uint64_t *)ct=v;	ct+=step;
				*(uint64_t *)ct=v;	ct+=step;
			}
#endif

			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=step;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
		ct=dst+16;
	}else
		if(sz<=32)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
		((uint64_t *)dst)[2]=((uint64_t *)cs)[2];
		((uint64_t *)dst)[3]=((uint64_t *)cs)[3];
		ct=dst+32;
	}else
		if(d<32)
//		if(1)
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;

#if 1
		while((ct+64)<cte)
		{
			csl=(uint64_t *)cs;
			ctl=(uint64_t *)ct;
			ctl[0]=csl[0];	ctl[1]=csl[1];
			ctl[2]=csl[2];	ctl[3]=csl[3];
			ctl[4]=csl[4];	ctl[5]=csl[5];
			ctl[6]=csl[6];	ctl[7]=csl[7];
			ct+=64; cs+=64;
		}
#endif

		while(ct<cte)
		{
			((uint64_t *)ct)[0]=((uint64_t *)cs)[0];
			((uint64_t *)ct)[1]=((uint64_t *)cs)[1];
			ct+=16; cs+=16;
		}
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;

		while((ct+64)<cte)
		{
			csl=(uint64_t *)cs;
			ctl=(uint64_t *)ct;
			
			v0=csl[0];	v1=csl[1];
			v2=csl[2];	v3=csl[3];
			ctl[0]=v0;	ctl[1]=v1;
			ctl[2]=v2;	ctl[3]=v3;

			v0=csl[4];	v1=csl[5];
			v2=csl[6];	v3=csl[7];
			ctl[4]=v0;	ctl[5]=v1;
			ctl[6]=v2;	ctl[7]=v3;
			
			ct+=64; cs+=64;
		}

		while(ct<cte)
		{
			v0=((uint64_t *)cs)[0];	v1=((uint64_t *)cs)[1];
			((uint64_t *)ct)[0]=v0;	((uint64_t *)ct)[1]=v1;
			ct+=16; cs+=16;
		}
	}
	return(ct);
}

uint8_t *MemLzCpy_MatchCopy2S(uint8_t *dst, long sz, long d)
{
	uint8_t *cs, *ct, *cte;

	cs=dst-d;
	ct=dst; cte=dst+sz;

	while((ct+4)<=cte)
	{	*ct++=*cs++;	*ct++=*cs++;
		*ct++=*cs++;	*ct++=*cs++;	}

	while(ct<cte)
		{ *ct++=*cs++; }
	return(ct);
}

#endif

#if 1
/** Do an "LZ style" copy.
  * This version will copy an exact number of bytes.
  */
void *_memlzcpy(void *dst, void *src, size_t n)
{
	void *ct1, *cte;
	long d, b, n1, n1f;
	
	cte=((uint8_t *)dst)+n;
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
		b=(long)src;
		if((d<=8) && !(b&15) && !(n&15))
		{
			/* If aligned, try to turn it into a memset. */
			if(d==1)
			{
				memset(src, *(uint8_t *)src, n+1);
				return(cte);
			}else
				if(d==2)
			{
				_memset16(src, *(uint16_t *)(src), (n+3)>>1);
				return(cte);
			}
			else
				if(d==4)
			{
				_memset32(src, *(uint32_t *)(src), (n+7)>>2);
				return(cte);
			}else
				if(d==8)
			{
				_memset64(src, *(uint64_t *)(src), (n+15)>>3);
				return(cte);
			}
		}

		if(1)
		{
			n1=n&(~15);
			
			if((d<8) && (d&(d-1)))
			{
				/* NPOT step, so use relative margin. */
				n1=n-16;
				if(n1<0)
					n1=0;
			}

			ct1=dst;

			if(n1>0)
			{
				/* Copy bulk portion. */
				ct1=MemLzCpy_MatchCopy2(dst, n1, d);
			}

//			n1f=n-(((char *)ct1)-((char *)dst));
			n1f=cte-ct1;
			
//			if(n1f<0)
//				{ __debugbreak(); }
			
			if(n1f>0)
			{
				/* Copy remainder, safer byte-for-byte copy. */
				ct1=MemLzCpy_MatchCopy2S(ct1, n1f, d);
			}
			return(cte);
		}
	}
	return(cte);
}

/** Do an "LZ style" copy.
  * Fast version may run past the end by a small amoumt.
  */
void *_memlzcpyf(void *dst, void *src, size_t n)
{
	void *cte;
	uint64_t v0, v1;
	long d, b;
	
	cte=((uint8_t *)dst)+n;
	d=((char *)dst)-((char *)src);
	if(d<=0)
	{
		/* Copying backwards, use normal copy. */
		if(n<=16)
		{
			/* Small copy. */
			v0=((uint64_t *)src)[0];
			v1=((uint64_t *)src)[1];
			((uint64_t *)dst)[0]=v0;
			((uint64_t *)dst)[1]=v1;
		}else
			if((-d)>=n)
		{
			memcpy(dst, src, n);
		}
		else if(d!=0)
		{
			memmove(dst, src, n);
		}
	}else if(d>=n)
	{
		/* No overlap, use memcpy. */
		if(n<=16)
		{
			/* Small copy. */
			v0=((uint64_t *)src)[0];
			v1=((uint64_t *)src)[1];
			((uint64_t *)dst)[0]=v0;
			((uint64_t *)dst)[1]=v1;
		}else
		{
			memcpy(dst, src, n);
		}
	}else
	{
		b=(long)src;
//		if(d<8)
//		if((d<8) && !(b&15))
		if((d<=8) && !(b&15) && !(n&15))
//		if(0)
		{
			/* If aligned, try to turn it into a memset. */
			if(d==1)
			{
				memset(src, *(uint8_t *)src, n+1);
//				return(cte);
			}else
#if 1
				if(d==2)
			{
				_memset16(src, *(uint16_t *)(src), (n+3)>>1);
//				return(cte);
			}
			else
#endif
#if 1
				if(d==4)
			{
				_memset32(src, *(uint32_t *)(src), (n+7)>>2);
			}else
#endif
#if 1
				if(d==8)
			{
				_memset64(src, *(uint64_t *)(src), (n+15)>>3);
			}else
#endif
			{
				MemLzCpy_MatchCopy2(dst, n, d);
			}
		}else
		{
			MemLzCpy_MatchCopy2(dst, n, d);
		}
	}
	return(cte);
}
#endif

#if 1
int _memlzcmp(void *dst, void *src, size_t n)
{
	uint8_t *cs, *ct, *cse;
	
	cs=src;
	ct=dst;
	cse=cs+n;
	
	while((cs+8)<=cse)
	{
		if((*(uint64_t *)cs)!=(*(uint64_t *)ct))
			break;
		cs+=8;	ct+=8;
	}

	while(cs<cse)
	{
		if(*cs!=*ct)
			break;
		cs++;	ct++;
	}
	return(cs-((uint8_t *)src));
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

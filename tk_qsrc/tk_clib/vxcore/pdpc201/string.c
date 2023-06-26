/*********************************************************************/
/*																   */
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																   */
/*********************************************************************/
/*********************************************************************/
/*																   */
/*  string.c - implementation of routines in string.h				*/
/*																   */
/*********************************************************************/

#include "stdio.h"
#include "string.h"
#include "stddef.h"
#include "errno.h"
#include "stdint.h"
#include "ctype.h"

#ifdef memmove
#undef memmove
#endif
__PDPCLIB_API__ void *memmove(void *s1, const void *s2, size_t n)
{
//	char *p = s1;
//	const char *cs2 = s2;
//	const char *cs2e = s2 + n;
	char *p;
	const char *cs2, *cs2s;
	const char *cs2e, *cs2e1;
	size_t x, d;

	p = s1;
	cs2 = s2;
	cs2e = s2 + n;

	if (p <= cs2)
	{
		d = (cs2-p);
		if(d >= n)
		{
			memcpy(s1, s2, n);
			return(s1);
		}

		if((n > d) && (d >= 8))
		{
			cs2e1 = cs2e - 8;
			while(cs2<cs2e1)
			{
				*(long long *)p = *(long long *)cs2;
				p+=8; cs2+=8;
			}
		}

		while(cs2<cs2e)
		{
			*p = *cs2;
			p++; cs2++;
		}

#if 0
		for (x=0; x < n; x++)
		{
			*p = *cs2;
			p++;
			cs2++;
		}
#endif
	}
	else
	{
//		p = s1;
//		cs2 = s2;
//		cs2e = s2 + n;

		d = (p-cs2);
		if(d >= n)
		{
			memcpy(s1, s2, n);
			return(s1);
		}
		
		cs2s = cs2;
		p += n;
		cs2 += n;

		if((n > d) && (d >= 8))
		{
			cs2e1 = cs2s + 8;
			while(cs2 >= cs2e1)
			{
				p-=8; cs2-=8;
				*(long long *)p = *(long long *)cs2;
			}
		}
		
		while(cs2 > cs2s)
		{
			p--; cs2--;
			*p = *cs2;
		}

#if 0
		if (n != 0)
		{
			for (x=n-1; x > 0; x--)
			{
				*(p+x) = *(cs2+x);
			}
			*(p+x) = *(cs2+x);
		}
#endif
	}

	return (s1);
}

#ifdef strcpy
#undef strcpy
#endif
__PDPCLIB_API__ char *strcpy(char *s1, const char *s2)
{
	char *p = s1;

	while ((*p++ = *s2++) != '\0') ;
	return (s1);
}

#ifdef strncpy
#undef strncpy
#endif
__PDPCLIB_API__ char *strncpy(char *s1, const char *s2, size_t n)
{
	char *p = s1;
	size_t x;

	for (x=0; x < n; x++)
	{
		*p = *s2;
		if (*s2 == '\0') break;
		p++;
		s2++;
	}
	for (; x < n; x++)
	{
		*p++ = '\0';
	}
	return (s1);
}

#ifdef strcat
#undef strcat
#endif

// #ifndef __BJX2__
#if !defined(__BJX2__) || defined(__ADDR_X96__)
// #if 1

__PDPCLIB_API__ char *strcat(char *s1, const char *s2)
{
	char *p = s1;

	while (*p != '\0') p++;
	while ((*p = *s2) != '\0')
	{
		p++;
		s2++;
	}
	return (s1);
}
#endif

#ifdef strncat
#undef strncat
#endif
__PDPCLIB_API__ char *strncat(char *s1, const char *s2, size_t n)
{
	char *p = s1;
	size_t x = 0;

	while (*p != '\0') p++;
	while ((*s2 != '\0') && (x < n))
	{
		*p = *s2;
		p++;
		s2++;
		x++;
	}
	*p = '\0';
	return (s1);
}

#ifdef memcmp
#undef memcmp
#endif

#if 0
__PDPCLIB_API__ int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	size_t x = 0;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (x < n)
	{
		if (p1[x] < p2[x]) return (-1);
		else if (p1[x] > p2[x]) return (1);
		x++;
	}
	return (0);
}
#endif

#if 1
__PDPCLIB_API__ int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	unsigned long long v1, v2;
	int u1, u2, x;
//	size_t x = 0;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	x = 0;

#if 1
	while ((x+8) <= n)
	{
//		v1=*(unsigned long long *)p1;
//		v2=*(unsigned long long *)p2;
		v1=*(unsigned long long *)(p1+x);
		v2=*(unsigned long long *)(p2+x);
		if(v1!=v2)
			break;
		x+=8;
	}
#endif

	while (x < n)
	{
		u1=p1[x];	u2=p2[x];
		if(u1 != u2)
			return(((u1-u2)>>31)*2+1);
//		if (u1<u2) return (-1);
//		else if (u1 > u2) return (1);
		x++;
	}
	return (0);
}
#endif

#ifdef strcmp
#undef strcmp
#endif

// #if 0
#if !defined(__BJX2__) || defined(__ADDR_X96__)
// #if 1
__PDPCLIB_API__ int strcmp(const char *s1, const char *s2)
{
	const unsigned char *p1;
	const unsigned char *p2;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (*p1 != '\0')
	{
		if (*p1 < *p2) return (-1);
		else if (*p1 > *p2) return (1);
		p1++;
		p2++;
	}
	if (*p2 == '\0') return (0);
	else return (-1);
}
#endif

#if 0
__PDPCLIB_API__ int strcmp(const char *s1, const char *s2)
{
	const unsigned char *p1;
	const unsigned char *p2;
//	uint64_t l1, l2;
	uint32_t i1, i2;
	int ze;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;

#if 0
	while(1)
	{
		l1=*(uint64_t *)p1;
		l2=*(uint64_t *)p2;
		
		if(l1==l2)
		{
			ze=	(!(l1&0x00000000000000FFULL)) |
				(!(l1&0x000000000000FF00ULL)) |
				(!(l1&0x0000000000FF0000ULL)) |
				(!(l1&0x00000000FF000000ULL)) |
				(!(l1&0x000000FF00000000ULL)) |
				(!(l1&0x0000FF0000000000ULL)) |
				(!(l1&0x00FF000000000000ULL)) |
				(!(l1&0xFF00000000000000ULL)) ;
			if(ze)
				return(0);
			p1+=8;
			p2+=8;
			continue;
		}
		break;
	}
#endif

#if 1
	while(1)
	{
		i1=*(uint32_t *)p1;
		i2=*(uint32_t *)p2;
		
		if(i1==i2)
		{
			ze=	(!(l1&0x000000FFU)) |
				(!(l1&0x0000FF00U)) |
				(!(l1&0x00FF0000U)) |
				(!(l1&0xFF000000U)) ;
			if(ze)
				break;
//				return(0);
			p1+=4;
			p2+=4;
			continue;
		}
		break;
	}
#endif
	
	i1=*p1++; i2=*p2++;
	while((i1==i2) && (i1!=0))
		{ i1=*p1++; i2=*p2++; }
	ze=0;
	if(i1<i2)ze=-1;
	if(i1>i2)ze= 1;
	return(ze);

//	while (*p1 != '\0')
//	{
//		if (*p1 < *p2) return (-1);
//		else if (*p1 > *p2) return (1);
//		p1++;
//		p2++;
//	}
//	if (*p2 == '\0') return (0);
//	else return (-1);
}
#endif

#ifdef strcoll
#undef strcoll
#endif
__PDPCLIB_API__ int strcoll(const char *s1, const char *s2)
{
	return (strcmp(s1, s2));
}

#ifdef strncmp
#undef strncmp
#endif

// #ifndef __BJX2__
#if !defined(__BJX2__) || defined(__ADDR_X96__)
// #if 1
__PDPCLIB_API__ int strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	size_t x = 0;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (x < n)
	{
		if (p1[x] < p2[x]) return (-1);
		else if (p1[x] > p2[x]) return (1);
		else if (p1[x] == '\0') return (0);
		x++;
	}
	return (0);
}
#endif

#ifdef strxfrm
#undef strxfrm
#endif
__PDPCLIB_API__ size_t strxfrm(char *s1, const char *s2, size_t n)
{
	size_t oldlen;

	oldlen = strlen(s2);
	if (oldlen < n)
	{
		memcpy(s1, s2, oldlen);
		s1[oldlen] = '\0';
	}
	return (oldlen);
}

#ifdef memchr
#undef memchr
#endif
__PDPCLIB_API__ void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p;
	size_t x = 0;

	p = (const unsigned char *)s;
	while (x < n)
	{
		if (*p == (unsigned char)c) return ((void *)p);
		p++;
		x++;
	}
	return (NULL);
}

#ifdef strchr
#undef strchr
#endif
__PDPCLIB_API__ char *strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c) return ((char *)s);
		s++;
	}
	if (c == '\0') return ((char *)s);
	return (NULL);
}

#ifdef strcspn
#undef strcspn
#endif
__PDPCLIB_API__ size_t strcspn(const char *s1, const char *s2)
{
	const char *p1;
	const char *p2;

	p1 = s1;
	while (*p1 != '\0')
	{
		p2 = s2;
		while (*p2 != '\0')
		{
			if (*p1 == *p2) return ((size_t)(p1 - s1));
			p2++;
		}
		p1++;
	}
	return ((size_t)(p1 - s1));
}

#ifdef strpbrk
#undef strpbrk
#endif
__PDPCLIB_API__ char *strpbrk(const char *s1, const char *s2)
{
	const char *p1;
	const char *p2;

	p1 = s1;
	while (*p1 != '\0')
	{
		p2 = s2;
		while (*p2 != '\0')
		{
			if (*p1 == *p2) return ((char *)p1);
			p2++;
		}
		p1++;
	}
	return (NULL);
}

#ifdef strrchr
#undef strrchr
#endif
__PDPCLIB_API__ char *strrchr(const char *s, int c)
{
	const char *p;

	p = s + strlen(s);
	while (p >= s)
	{
		if (*p == (char)c) return ((char *)p);
		p--;
	}
	return (NULL);
}

#ifdef strspn
#undef strspn
#endif
__PDPCLIB_API__ size_t strspn(const char *s1, const char *s2)
{
	const char *p1;
	const char *p2;

	p1 = s1;
	while (*p1 != '\0')
	{
		p2 = s2;
		while (*p2 != '\0')
		{
			if (*p1 == *p2) break;
			p2++;
		}
		if (*p2 == '\0') return ((size_t)(p1 - s1));
		p1++;
	}
	return ((size_t)(p1 - s1));
}


/* strstr by Frank Adam */
/* modified by Paul Edwards */

#ifdef strstr
#undef strstr
#endif
__PDPCLIB_API__ char *strstr(const char *s1, const char *s2)
{
	const char *p = s1, *p1, *p2 = s2;

	while (*p)
	{
		if (*p == *s2)
		{
			p1 = p;
			p2 = s2;
			while ((*p2 != '\0') && (*p1 == *p2))
			{
				p1++;
				p2++;
			}
			if (*p2 == '\0')
			{
				return (char *)p;
			}
		}
		p++;
	}
	return NULL;
}

#ifdef strtok
#undef strtok
#endif
__PDPCLIB_API__ char *strtok(char *s1, const char *s2)
{
	static char *old = NULL;
	char *p;
	size_t len;
	size_t remain;

	if (s1 != NULL) old = s1;
	if (old == NULL) return (NULL);
	p = old;
	len = strspn(p, s2);
	remain = strlen(p);
	if (remain <= len) { old = NULL; return (NULL); }
	p += len;
	len = strcspn(p, s2);
	remain = strlen(p);
	if (remain <= len) { old = NULL; return (p); }
	*(p + len) = '\0';
	old = p + len + 1;
	return (p);
}

#ifdef memset
#undef memset
#endif

#if 0
__PDPCLIB_API__ void *memset(void *s, int c, size_t n)
{
	size_t x = 0;

	for (x = 0; x < n; x++)
	{
		*((char *)s + x) = (unsigned char)c;
	}
	return (s);
}
#endif

// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
// #if 0
void memset_movx(void *s, int c, size_t n);
void memset_filx(void *s, uint64_t c, size_t n);

__asm {
memset_movx:

.ifarch bjx2_movx
	PSHUF.B		R5, 0, R7
	PSHUF.W		R7, 0, R20
	MOV		R20, R22	|	MOV		R20, R23
	MOV		R20, R21	|	CMPGT	32, R6
	BF		.L1
	.L0:
	MOV.X	R20, (R4,  0)
	ADD		-32, R6	
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMPGT	32, R6
	BT		.L0
	.L1:
.else
	EXTU.B		R5, R20
	SHLD.Q		R5, 8, R7
	OR			R7, R20
	SHLD.Q		R5, 16, R7
	OR			R7, R20
	SHLD.Q		R5, 32, R7
	OR			R7, R20
.endif

.ifarch bjx2_wex3p
	CMPGE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	R20, (R4)
	ADD		8, R4		|	CMPGE	8, R6
	BT		.L2
	.L3:
.else
	CMPGE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6
	MOV.Q	R20, (R4)
	ADD		8, R4
	CMPGE	8, R6
	BT		.L2
	.L3:
.endif

	RTSU


memset_filx:

.ifarch bjx2_movx
	MOV		R5,  R20
	MOV		R20, R22	|	MOV		R20, R23
	MOV		R20, R21	|	CMPGT	32, R6
	BF		.L1
	.L0:
	MOV.X	R20, (R4,  0)
	ADD		-32, R6	
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMPGT	32, R6
	BT		.L0
	.L1:
.else
	MOV		R5,  R20
.endif

.ifarch bjx2_wex3p
	CMPGE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	R20, (R4)
	ADD		8, R4		|	CMPGE	8, R6
	BT		.L2
	.L3:
.else
	CMPGE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6
	MOV.Q	R20, (R4)
	ADD		8, R4
	CMPGE	8, R6
	BT		.L2
	.L3:
.endif

	RTSU
}
#endif

#if 1
__PDPCLIB_API__ void *memset(void *s, int c, size_t n)
{
	unsigned char *ct, *cte, *cte0;
#if defined(__ADDR_X96__)
	__int128 xv1;
#endif
	unsigned long long v1;
	int v, a, n1;

#if defined(__ADDR_X96__)
	xv1 = (__int128)s;
	if(xv1!=(long)xv1)
		__debugbreak();
#endif

//#ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
// #if 0
//	v=((int)s)|n;
	a=(int)(((long)s)|n);
//	if(!(a&7))
	if(!(a&15))
	{
		memset_movx(s, c, n);
		return(s);
	}
#endif

// #ifdef __BJX2__
#if 0
//	if(n>=256)
	if(n>=1024)
	{
		v1=c; v1|=(v1<<8); v1|=(v1<<16); v1|=(v1<<32);
		ct=s; cte=ct+n;

		a=(int)((long)s);
		if(a&15)
		{
			((unsigned long long *)ct)[0]=v1;
			((unsigned long long *)ct)[1]=v1;
			ct+=16-(a&15);
		}

		n1=cte-ct;
		if(!(n1&7))
		{
			memset_movx(ct, c, n1);
			return(s);
		}
		
		n1=n1&(~7);
		memset_movx(ct, c, n1);
		ct+=n1;
		while(ct<cte)
			{ *ct++=c; }

		return (s);
	}else
	{
//		v=c; v|=(v<<8); v|=(v<<16);
		v1=c; v1|=(v1<<8); v1|=(v1<<16); v1|=(v1<<32);
		v=v1;
		ct=s; cte=s+n;

#if 1
		cte0=cte-16;
		while(ct<=cte0)
		{
			((unsigned long long *)ct)[0]=v1;
			((unsigned long long *)ct)[1]=v1;
//			((int *)ct)[0]=v;
//			((int *)ct)[1]=v;
//			((int *)ct)[2]=v;
//			((int *)ct)[3]=v;
			ct+=16;
		}
#endif

#if 1
		cte0=cte-4;
		while(ct<=cte0)
			{ *(int *)ct=v; ct+=4; }
#endif

		while(ct<cte)
			{ *ct++=c; }

		return (s);
	}
#endif

#if 1

	v=c; v|=(v<<8); v|=(v<<16);
	ct=s; cte=s+n;

#if defined(__ADDR_X96__)
	xv1 = (__int128)cte;
	if(xv1!=(long)xv1)
		__debugbreak();
#endif

#if 1
	v1=(((unsigned long long)v)<<32)|((unsigned int)v);
	cte0=cte-16;
	while(ct<=cte0)
	{
		((unsigned long long *)ct)[0]=v1;
		((unsigned long long *)ct)[1]=v1;
		ct+=16;
	}
#endif

#if 0
//	cte0=s+(n&(~15));
//	while(ct<cte0)
//	while((ct+16)<=cte)
	cte0=cte-16;
	while(ct<=cte0)
	{
		((int *)ct)[0]=v;
		((int *)ct)[1]=v;
		((int *)ct)[2]=v;
		((int *)ct)[3]=v;
		ct+=16;
	}
#endif

#if 1
//	cte0=s+(n&(~3));
//	while(ct<cte0)
//	while((ct+4)<=cte)
	cte0=cte-4;
	while(ct<=cte0)
		{ *(int *)ct=v; ct+=4; }
#endif

	while(ct<cte)
		{ *ct++=c; }

	return (s);

#endif
}
#endif

__PDPCLIB_API__ void *_memset64(void *s, uint64_t v, size_t n)
{
	uint64_t *ct, *cte, *cte0;
	uint64_t v1;
	int a, n1;

#if defined(__BJX2__) && !defined(__ADDR_X96__)
	n1=n<<3;
	a=(int)(((long)s)|n1);
	if(!(a&15))
	{
		memset_filx(s, v, n1);
		return(s);
	}
#endif

	ct=s; cte=ct+n;
	while((ct+8)<=cte)
	{
		ct[0]=v;	ct[1]=v;
		ct[2]=v;	ct[3]=v;
		ct[4]=v;	ct[5]=v;
		ct[6]=v;	ct[7]=v;
		ct+=8;
	}

	while(ct<cte)
		{ *ct++=v; }
	return(s);
}

__PDPCLIB_API__ void *_memset32(void *s, uint32_t v, size_t n)
{
	uint32_t *ct, *cte, *cte0;
	uint64_t v1;
	int a, n1;

	v1=(((uint64_t)v)<<32)|(v);

#if 1
#if defined(__BJX2__) && !defined(__ADDR_X96__)
	n1=n<<2;
	a=(int)(((long)s)|n1);
	if(!(a&15))
	{
		memset_filx(s, v1, n1);
		return(s);
	}
#endif

	if(!(n&1))
	{
		return(_memset64(s, v1, n>>1));
	}
#endif

	ct=s; cte=ct+n;

#if 1
	while((ct+8)<=cte)
	{
		((uint64_t *)ct)[0]=v1;
		((uint64_t *)ct)[1]=v1;
		((uint64_t *)ct)[2]=v1;
		((uint64_t *)ct)[3]=v1;
		ct+=8;
	}
#endif

	while(ct<cte)
		{ *ct++=v; }
	return(s);
}

__PDPCLIB_API__ void *_memset16(void *s, uint16_t v, size_t n)
{
	uint16_t *ct, *cte, *cte0;
	uint64_t *ctl;
	uint64_t v1;
	int a, n1;

	v1=(((uint64_t)v)<<16)|(v);
	v1=(v1<<32)|v1;

#if 1
#if defined(__BJX2__) && !defined(__ADDR_X96__)
	n1=n<<1;
	a=(int)(((long)s)|n1);
	if(!(a&15))
	{
		memset_filx(s, v1, n1);
		return(s);
	}
#endif

	if(!(n&1))
	{
		return(_memset32(s, v1, n>>1));
	}
#endif

	ct=s; cte=ct+n;

#if 1
	while((ct+16)<=cte)
	{
		ctl=(uint64_t *)ct;
		ctl[0]=v1;	ctl[1]=v1;
		ctl[2]=v1;	ctl[3]=v1;
		ct+=16;
	}
#endif

	while(ct<cte)
		{ *ct++=v; }
	return(s);
}


#ifdef strerror
#undef strerror
#endif
__PDPCLIB_API__ char *strerror(int errnum)
{

#ifndef __VX86__
	if (errnum == 0) return ("No error has occurred\n");
	else return ("An error has occurred\n");
#else
	char *s;

	switch(errnum)
	{
	case 0: s="No error"; break;

	case EPERM: s="Operation not permitted"; break;
	case ENOFILE: s="No such file or directory"; break;
	case ESRCH: s="No such process"; break;
	case EINTR: s="Interrupted function call"; break;
	case EIO: s="Input/output error"; break;
	case ENXIO: s="No such device or address"; break;
	case E2BIG: s="Arg list too long"; break;
	case ENOEXEC: s="Exec format error"; break;
	case EBADF: s="Bad file descriptor"; break;
	case ECHILD: s="No child processes"; break;
	case EAGAIN: s="Resource temporarily unavailable"; break;
	case ENOMEM: s="Not enough space"; break;
	case EACCES: s="Permission denied"; break;
	case EFAULT: s="Bad address"; break;
	case EBUSY: s="Resource device"; break;
	case EEXIST: s="File exists"; break;
	case EXDEV: s="Improper link"; break;
	case ENODEV: s="No such device"; break;
	case ENOTDIR: s="Not a directory"; break;
	case EISDIR: s="Is a directory"; break;
	case EINVAL: s="Invalid argument"; break;
	case ENFILE: s="Too many open files in system"; break;
	case EMFILE: s="Too many open files"; break;
	case ENOTTY: s="Inappropriate I/O control operation"; break;
	case EFBIG: s="File too large"; break;
	case ENOSPC: s="No space left on device"; break;
	case ESPIPE: s="Invalid seek"; break;
	case EROFS: s="Read-only file system"; break;
	case EMLINK: s="Too many links"; break;
	case EPIPE: s="Broken pipe"; break;
	case EDOM: s="Domain error"; break;
	case ERANGE: s="Result too large or small"; break;
	case EDEADLOCK: s="Resource deadlock avoided"; break;
	case ENAMETOOLONG: s="Filename too long"; break;
	case ENOLCK: s="No locks available"; break;
	case ENOSYS: s="Function not implemented"; break;
	case ENOTEMPTY: s="Directory not empty"; break;
	case EILSEQ: s="Illegal byte sequence"; break;

	default: s="An error has occurred"; break;
	}
	return(s);
#endif
}


// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
// #if 0

__PDPCLIB_API__ size_t strlen(const char *s);
__PDPCLIB_API__ int strcmp(const char *s1, const char *s2);
__PDPCLIB_API__ int strncmp(const char *s1, const char *s2, size_t n);
__PDPCLIB_API__ char *strcat(char *s1, const char *s2);

__asm {
strlen:
	MOV		R4, R6
	MOV		0, R5
	
	.L0:
	MOV.Q		(R4), R7
	PSCHEQ.B	R7, R5, R1
	BT			.L1
	ADD			8, R4
	BRA			.L0
	.L1:
	SUB			R4, R6, R2
	ADD			R1, R2
	RTS
	
strcmp:
//	ADD			R4, 8, R18	|	MOV.Q		(R4), R6
//	ADD			R5, 8, R19	|	MOV.Q		(R5), R7
	MOV.Q		(R4), R6
	MOV.Q		(R5), R7
	MOV			0, R2

#if 1
	MOV.Q		(R4, 8), R16
	MOV.Q		(R5, 8), R17

	PSCHNE.B	R6, R7, R1
	BT			.L1
	PSCHEQ.B	R7, R2, R1
	BT			.L2

	ADD			8, R4		|	ADD			8, R5
	MOV			R16, R6		|	MOV			R17, R7

	MOV.Q		(R4, 8), R16
	MOV.Q		(R5, 8), R17

	PSCHNE.B	R6, R7, R1
	BT			.L1
	PSCHEQ.B	R7, R2, R1
	BT			.L2

#if 1
	ADD			8, R4		|	ADD			8, R5
	MOV			R16, R6		|	MOV			R17, R7

	MOV.Q		(R4, 8), R16
	MOV.Q		(R5, 8), R17

	PSCHNE.B	R6, R7, R1
	BT			.L1
	PSCHEQ.B	R7, R2, R1
	BT			.L2
#endif

#if 1
	ADD			8, R4		|	ADD			8, R5
	MOV			R16, R6		|	MOV			R17, R7

	MOV.Q		(R4, 8), R16
	MOV.Q		(R5, 8), R17

	PSCHNE.B	R6, R7, R1
	BT			.L1
	PSCHEQ.B	R7, R2, R1
	BT			.L2
#endif

//	MOV.Q		(R4, 8), R6
//	MOV.Q		(R5, 8), R7
	MOV			R16, R6		|	MOV			R17, R7
	ADD			8, R4		|	ADD			8, R5
#endif

#if 1
	.L0:

//	MOV.Q		(R4), R6
//	MOV.Q		(R5), R7
	PSCHNE.B	R6, R7, R1
	BT			.L1
	PSCHEQ.B	R7, R2, R1
	BT			.L2
	MOV.Q		(R4, 8), R6
	MOV.Q		(R5, 8), R7
	ADD			8, R4		|	ADD			8, R5
//	MOV			R18, R4		|	MOV.Q		(R18), R6
//	MOV			R19, R5		|	MOV.Q		(R19), R7
//	ADD			R4, 8, R18	|	ADD			R5, 8, R19
	BRA			.L0
#endif

#if 0
	.L0:
	MOV.Q		(R4, 0), R16
	MOV.Q		(R5, 0), R17
	MOV.Q		(R4, 8), R18
	MOV.Q		(R5, 8), R19

	PSCHNE.B	R16, R17, R1
	BT			.L1
	PSCHEQ.B	R17, R2, R1
	BT			.L2

	PSCHNE.B	R18, R19, R1
	BT			.L1B
	PSCHEQ.B	R19, R2, R1
	BT			.L2

	ADD			16, R4		|	ADD			16, R5
	BRA			.L0
#endif

#if 0
	.L1B:
	ADD			8, R4		|	ADD			8, R5
	MOV			R19, R17

	.L1:
	PSCHEQ.B	R17, R2, R3
	CMPGT		R3, R1
	BT			.L2
	ADD			R1, R4		|	ADD			R1, R5
	MOV.B		(R4), R6
	MOV.B		(R5), R7
#endif


#if 1
	.L1:
	PSCHEQ.B	R7, R2, R3
	CMPGT		R3, R1
	BT			.L2
	ADD			R1, R4		|	ADD			R1, R5
	MOV.B		(R4), R6
	MOV.B		(R5), R7
#endif

#if 0
	.L1:
	MOV.B		(R4), R6
	MOV.B		(R5), R7
	CMPQEQ		R6, R7
	BF			.L3
	TST			R6, R6
	BT			.L2
	ADD			1, R4		|	ADD			1, R5
	BRA			.L1

#endif

	.L3:
	CMPGT		R7, R6
	MOV?T		1, R2
	MOV?F		-1, R2	
	RTS

	.L2:
//	MOV			0, R2
	RTS


strncmp:
	MOV			0, R2
	.L0:
	MOV.Q		(R4), R16
	MOV.Q		(R5), R17
	PSCHNE.B	R16, R17, R1
	BT			.L1

	PSCHEQ.B	R17, R2, R1
	BT			.L2
	ADD			-8, R6
	ADD			8, R4	|	ADD			8, R5
	CMPGT		0, R6
	BT			.L0
	BRA			.L2

#if 1
	.L1:
	PSCHEQ.B	R7, R2, R3
	CMPGT		R3, R1
	BT			.L2
//	CMPGT		R6, R1
	CMPGE		R6, R1
	BT			.L2

	ADD			R1, R4
	ADD			R1, R5
	MOV.B		(R4), R6
	MOV.B		(R5), R7
#endif

	.L3:
	CMPGT		R7, R6
	MOV?T		1, R2
	MOV?F		-1, R2	

	.L2:
	RTS

#if 1
strcat:
	MOV		R4, R2
	MOV		0, R3
	
	.L0:
	MOV.Q		(R4), R7
	PSCHEQ.B	R7, R3, R1
	BT			.L1
	ADD			8, R4
	BRA			.L0
	.L1:
	ADD			R1, R4

	.L2:
	MOV.Q		(R5), R7
	PSCHEQ.B	R7, R3, R1
	BT			.L3
	MOV.Q		R7, (R4)
	ADD			8, R4	|	ADD			8, R5
	BRA			.L2

	.L3:
	MOV.B		(R5), R7
	MOV.B		R7, (R4)
	ADD			1, R4	|	ADD			1, R5
	TST			R7, R7
	BF			.L3

	RTS
#endif
};
#endif


#ifdef __BJX2__
int _strcmp_util_pack8zp(uint64_t v0);
int _strcmp_util_cmppack8(uint64_t v0, uint64_t v1);

__asm {
_strcmp_util_pack8zp:
	MOV			0, R2
	PSCHEQ.B	R4, R2, R1
	MOV?T		1, R2
	RTS

_strcmp_util_cmppack8:
	MOV			0, R2

	PSCHNE.B	R4, R5, R1
	BF			.L2
	
	PSCHEQ.B	R4, R2, R3
	CMPGT		R3, R1
	BT			.L2

	SHLD		R1, 3, R3
	NEG			R3, R3
	SHLD.Q		R4, R3, R6	|	SHLD.Q		R5, R3, R7
	EXTU.B		R6, R6		|	EXTU.B		R7, R7

	CMPGT		R7, R6
	MOV?T		1, R2
	MOV?F		-1, R2	

	.L2:
	RTS

};

#endif

// #ifndef __BJX2__
#if !defined(__BJX2__) || defined(__ADDR_X96__)
// #if 1

#ifdef strlen
#undef strlen
#endif
__PDPCLIB_API__ size_t strlen(const char *s)
{
	const char *p;
//	char *p;
	int n;

	p = s;
	n=0;
//	while (*p != '\0') p++;
//	while(*p)p++;
	while(*p) { p++; n++; }

	return(n);
//	return ((size_t)(p - s));
}

#endif

int _string_is_nonascii(const char *s)
{
	const unsigned char *p;
	uint64_t v;
	int c;

#ifdef __BJX2__
	p = (const unsigned char *)s;
	v=*(uint64_t *)p;
	while(!_strcmp_util_pack8zp(v))
	{
		if(v&0x8080808080808080ULL)
			return(1);
		p+=8;
		v=*(uint64_t *)p;
	}

	if(!(v&0x8080808080808080ULL))
		return(0);
#endif

	c=*p;
	while(c)
	{
		if(c&0x80)
			return(1);
		p++;
		c=*p;
	}
	return(0);
}

const unsigned char *_string_readchar_u8(const unsigned char *p, int *rc)
{
	int c0, c;
	
	c0=*p;
	if(!(c0&0x80))
	{
		*rc=c0;
		return(p+1);
	}
	if(!(c0&0x40))
	{
		/* BAD */
		*rc=c0;
		return(p+1);
	}

	if(!(c0&0x20))
	{
		c=((c0&0x1F)<<6)|(p[1]&0x3F);
		*rc=c;
		return(p+2);
	}

	if(!(c0&0x10))
	{
		c=	((c0&0x0F)<<12)|
			((p[1]&0x3F)<<6)|
			(p[2]&0x3F);
		*rc=c;
		return(p+3);
	}

	if(!(c0&0x08))
	{
		c=	((c0&0x07)<<18)|
			((p[1]&0x3F)<<12)|
			((p[2]&0x3F)<<6)|
			(p[3]&0x3F);
		*rc=c;
		return(p+4);
	}

	if(!(c0&0x04))
	{
		c=	((c0  &0x03)<<24)|
			((p[1]&0x3F)<<18)|
			((p[2]&0x3F)<<12)|
			((p[3]&0x3F)<< 6)|
			((p[4]&0x3F)    );
		*rc=c;
		return(p+5);
	}

	if(!(c0&0x02))
	{
		c=	((c0  &0x01)<<30)|
			((p[1]&0x3F)<<24)|
			((p[2]&0x3F)<<18)|
			((p[3]&0x3F)<<12)|
			((p[4]&0x3F)<< 6)|
			((p[5]&0x3F)    );
		*rc=c;
		return(p+6);
	}
}

__PDPCLIB_API__ int _stricmp_u8(const char *s1, const char *s2)
{
	const unsigned char *p1;
	const unsigned char *p2;
	int c1, c2, rl;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;

	c1=*p1;
	c2=*p2;
	while(c1)
	{
		if(c1&0x80)
			{ p1=_string_readchar_u8(p1, &c1); }
		else
			{ p1++; }
		if(c2&0x80)
			{ p2=_string_readchar_u8(p2, &c2); }
		else
			{ p2++; }
	
		c1=tolower(c1);
		c2=tolower(c2);
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		c1=*p1;
		c2=*p2;
	}

	if(!c2)
		return (0);
	return (-1);
}

__PDPCLIB_API__ int _strnicmp_u8(const char *s1, const char *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	int c1, c2, rl, x;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;

	c1=*p1;
	c2=*p2;
	x=0;
	while(c1 && (x<n))
	{
		if(c1&0x80)
			{ p1=_string_readchar_u8(p1, &c1); }
		else
			{ p1++; }
		if(c2&0x80)
			{ p2=_string_readchar_u8(p2, &c2); }
		else
			{ p2++; }
	
		c1=tolower(c1);
		c2=tolower(c2);
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		c1=*p1;
		x++;
		c2=*p2;
	}

	if(!c2)
		return (0);
	return (-1);
}

__PDPCLIB_API__ int _stricmp(const char *s1, const char *s2)
{
	const unsigned char *p1;
	const unsigned char *p2;
	uint64_t	v1, v2;
	int c1, c2, rl;

	if(_locale_is_utf8())
	{
		if(_string_is_nonascii(s1) || _string_is_nonascii(s2))
		{
			return(_stricmp_u8(s1, s2));
		}
	}

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;

#ifdef __BJX2__
// #if 0
//	__cytpe_init();

	v1=*(uint64_t *)p1;
	v2=*(uint64_t *)p2;
	rl=0;
	while(!_strcmp_util_pack8zp(v1))
	{
//		v1=_toupper_8x(v1);
//		v2=_toupper_8x(v2);
		v1=_tolower_8x(v1);
		v2=_tolower_8x(v2);
		rl=_strcmp_util_cmppack8(v1, v2);
		if(rl)break;
		p1+=8;
		p2+=8;
		v1=*(uint64_t *)p1;
		v2=*(uint64_t *)p2;
	}
	if(rl)
		return(rl);

//	v1=_toupper_8x(v1);
//	v2=_toupper_8x(v2);
	v1=_tolower_8x(v1);
	v2=_tolower_8x(v2);
	rl=_strcmp_util_cmppack8(v1, v2);
	return(rl);
#endif

	while (*p1 != '\0')
	{
//		c1=toupper(*p1);
//		c2=toupper(*p2);
		c1=tolower(*p1);
		c2=tolower(*p2);
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		p1++;
		p2++;
	}
	if (*p2 == '\0')
	{
#if 0
		if(rl)
		{
			printf("stricmp: 0 %016llX %016llX\n", v1, v2);
			__debugbreak();
		}
#endif
	
		return (0);
	}
	else return (-1);
}

__PDPCLIB_API__ int _strnicmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	uint64_t	v1, v2, v3;
	int c1, c2, rl;
	size_t x;

	if(_locale_is_utf8())
	{
		if(_string_is_nonascii(s1) || _string_is_nonascii(s2))
		{
			return(_strnicmp_u8(s1, s2, n));
		}
	}

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	x = 0;

#ifdef __BJX2__
//#if 0
//	if(n>=16)
	if(1)
	{
//		__cytpe_init();

		v1=*(uint64_t *)p1;
		v2=*(uint64_t *)p2;
		rl=0; x=0;
		while(!_strcmp_util_pack8zp(v1) && ((x+8)<=n))
		{
//			v1=_toupper_8x(v1);
//			v2=_toupper_8x(v2);
			v1=_tolower_8x(v1);
			v2=_tolower_8x(v2);
			rl=_strcmp_util_cmppack8(v1, v2);
			if(rl)
				return(rl);
			p1+=8;	p2+=8;	x+=8;
			v1=*(uint64_t *)p1;
			v2=*(uint64_t *)p2;
		}

		c1=n-x;
		if(c1<=0)
			return(0);
		
		if(c1<8)
		{
			v3=(1LL<<(c1*3))-1;
			v1&=v3;
			v2&=v3;
		}

//		v1=_toupper_8x(v1);
//		v2=_toupper_8x(v2);
		v1=_tolower_8x(v1);
		v2=_tolower_8x(v2);
		rl=_strcmp_util_cmppack8(v1, v2);
		return(rl);
		
//		if(rl)
//			return(rl);
	}
#endif

	while (x < n)
	{
//		c1 = toupper(p1[x]);
//		c2 = toupper(p2[x]);
		c1 = tolower(p1[x]);
		c2 = tolower(p2[x]);
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		else if (p1[x] == '\0') return (0);
		x++;
	}
	return (0);
}


#ifndef USE_ASSEMBLER
#ifdef memcpy
#undef memcpy
#endif

#if 0
// #ifndef __32BIT__
#if 0
__PDPCLIB_API__ void *memcpy(void *s1, const void *s2, size_t n)
{
//	register const unsigned char *f = s2;
//	register const unsigned char *fe;
//	register unsigned char *t = s1;
	const char *f, *fe;
	char *t;
	
	f=s2; t=s1;
	fe = f + n;
//	while (f != fe)
	while(f<fe)
		{ *t++ = *f++; }
	return (s1);
}
#else
__PDPCLIB_API__ void *memcpy(void *s1, const void *s2, size_t n)
{
	register unsigned int *p = (unsigned int *)s1;
	register unsigned int *cs2 = (unsigned int *)s2;
	register unsigned int *endi;
	register long long li0, li1;

//	n=(int)n;
	if(n!=((int)n))
		__debugbreak();

	endi = (unsigned int *)((char *)p + (n & ~0x03));

#if 1
	while ((p+4) <= endi)
	{
		li0 = ((long long *)cs2)[0];
		li1 = ((long long *)cs2)[1];
		((long long *)p)[0] = li0;
		((long long *)p)[1] = li1;

//		((long long *)p)[0] = ((long long *)cs2)[0];
//		((long long *)p)[1] = ((long long *)cs2)[1];
//		p[0] = cs2[0];
//		p[1] = cs2[1];
//		p[2] = cs2[2];
//		p[3] = cs2[3];
		p+=4; cs2+=4;
//		__debugbreak();
	}
#endif

//	while (p != endi)
	while (p < endi)
	{
		*p++ = *cs2++;
	}
	switch (n & 0x03)
	{
		case 0:
			break;
		case 1:
			*(char *)p = *(char *)cs2;
			break;
		case 2:
			*(char *)p = *(char *)cs2;
			p = (unsigned int *)((char *)p + 1);
			cs2 = (unsigned int *)((char *)cs2 + 1);
			*(char *)p = *(char *)cs2;
			break;
		case 3:
			*(char *)p = *(char *)cs2;
			p = (unsigned int *)((char *)p + 1);
			cs2 = (unsigned int *)((char *)cs2 + 1);
			*(char *)p = *(char *)cs2;
			p = (unsigned int *)((char *)p + 1);
			cs2 = (unsigned int *)((char *)cs2 + 1);
			*(char *)p = *(char *)cs2;
			break;
	}
	return (s1);
}

__PDPCLIB_API__ void *_memcpyf(void *s1, void *s2, size_t n)
{
	return(memcpy(s1, s2, n));
}
#endif /* 32BIT */
#endif /* USE_ASSEMBLER */
#endif

// #if 0
#if defined(__TK_CLIB_DLLSTUB__) || defined(__riscv)

__PDPCLIB_API__ void *memcpy(void *s1, const void *s2, size_t n)
{
	register long long *p = (long long *)s1;
	register long long *cs2 = (long long *)s2;
	register long long *endi;

//	n=(int)n;
//	if(n!=((int)n))
//		__debugbreak();

	endi = (long long *)((char *)p + (n & (~0x07)));

#if 1
	while (p < endi)
	{
		*p++ = *cs2++;
	}
#endif

	switch (n & 0x07)
	{
		case 0:
			break;
		case 1:
			*(char *)p = *(char *)cs2;
			break;
		case 2:
			*(short *)p = *(short *)cs2;
			break;
		case 3:
			((short *)p)[0] = ((short *)cs2)[0];
			((char *)p)[2] = ((char *)cs2)[2];
			break;
		case 4:
			*(int *)p = *(int *)cs2;
			break;
		case 5:
			((int *)p)[0] = ((int *)cs2)[0];
			((char *)p)[4] = ((char *)cs2)[4];
			break;
		case 6:
			((int *)p)[0] = ((int *)cs2)[0];
			((short *)p)[2] = ((short *)cs2)[2];
			break;
		case 7:
			((int *)p)[0] = ((int *)cs2)[0];
			((short *)p)[2] = ((short *)cs2)[2];
			((char *)p)[7] = ((char *)cs2)[7];
			break;
		default:
			break;
	}
	return (s1);
}

__PDPCLIB_API__ void *_memcpyf(void *s1, void *s2, size_t n)
{
	return(memcpy(s1, s2, n));
}
#endif


// #ifdef __riscv
#if 0
__PDPCLIB_API__ void *memcpy(void *s1, const void *s2, size_t n)
{
	u64 *cs, *ct, *cte;
	byte *csb, *ctb, *cteb;

	ct=(u64 *)s1; cs=(u64 *)s2; cte=(u64 *)(((byte *)s1)+(n&(~7)));
	
	while(ct<cte)
	{
		*ct=*cs;
		ct++; cs++;
	}
	
	if(n&7)
	{
		csb=(byte *)cs;
		ctb=(byte *)ct;
		cteb=((byte *)s1)+n;
		while(ctb<cteb)
		{
			*ctb=*csb;
			ctb++;	csb++;
		}
	}
	
	return(ct);
}

__PDPCLIB_API__ void *_memcpyf(void *s1, void *s2, size_t n)
{
	return(memcpy(s1, s2, n));
}
#endif

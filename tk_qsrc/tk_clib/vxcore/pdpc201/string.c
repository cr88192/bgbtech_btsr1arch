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

#ifdef memmove
#undef memmove
#endif
__PDPCLIB_API__ void *memmove(void *s1, const void *s2, size_t n)
{
	char *p = s1;
	const char *cs2 = s2;
	size_t x;

	if (p <= cs2)
	{
		for (x=0; x < n; x++)
		{
			*p = *cs2;
			p++;
			cs2++;
		}
	}
	else
	{
		if (n != 0)
		{
			for (x=n-1; x > 0; x--)
			{
				*(p+x) = *(cs2+x);
			}
			*(p+x) = *(cs2+x);
		}
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

#ifdef strcmp
#undef strcmp
#endif
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

#if 1
__PDPCLIB_API__ void *memset(void *s, int c, size_t n)
{
	unsigned char *ct, *cte, *cte0;
	int v;

	v=c; v|=(v<<8); v|=(v<<16);
	ct=s; cte=s+n;

	cte0=s+(n&(~15));
	while(ct<cte0)
	{
		((int *)ct)[0]=v;
		((int *)ct)[1]=v;
		((int *)ct)[2]=v;
		((int *)ct)[3]=v;
		ct+=16;
	}

	cte0=s+(n&(~3));
	while(ct<cte0)
		{ *(int *)ct=v; ct+=4; }

	while(ct<cte)
		{ *ct++=c; }

	return (s);
}
#endif

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

__PDPCLIB_API__ int stricmp(const char *s1, const char *s2)
{
	const unsigned char *p1;
	const unsigned char *p2;
	int c1, c2;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (*p1 != '\0')
	{
		c1=toupper(*p1);
		c2=toupper(*p2);
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		p1++;
		p2++;
	}
	if (*p2 == '\0') return (0);
	else return (-1);
}

__PDPCLIB_API__ int strnicmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	int c1, c2;
	size_t x = 0;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (x < n)
	{
		c1 = toupper(p1[x]);
		c2 = toupper(p2[x]);
		if (c1 < c2)
			return (-1);
		else if (toupper(p1[x]) > toupper(p2[x]))
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

//	n=(int)n;
	if(n!=((int)n))
		__debugbreak();

	endi = (unsigned int *)((char *)p + (n & ~0x03));
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
#endif /* 32BIT */
#endif /* USE_ASSEMBLER */

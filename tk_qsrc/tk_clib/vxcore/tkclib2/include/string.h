#ifndef STD_STRING_H
#define STD_STRING_H

#include <stddef.h>

void *memcpy(void *s1, const void *s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strcoll(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strxfrm(char *s1, const char *s2, size_t n);
void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);
size_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strrchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
char *strstr(const char *s1, const char *s2);
char *strtok(char *s1, const char *s2);
void *memset(void *s, int c, size_t n);
char *strerror(int errnum);
size_t strlen(const char *s);

char *strdup(char *str);
char *strndup(char *str, size_t n);

errno_t memcpy_s(
	void *restrict dest, rsize_t destsz,
	const void *restrict src, rsize_t count );
errno_t memmove_s(
	void *dest, rsize_t destsz, const void *src, rsize_t count);

errno_t strcat_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src);
errno_t strcpy_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src);
size_t strnlen_s( const char *str, size_t strsz );

errno_t strncat_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src, rsize_t count);
errno_t strncpy_s(
	char *restrict dest, rsize_t destsz,
	const char *restrict src, rsize_t count);

#endif

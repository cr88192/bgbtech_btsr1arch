#ifndef STD_STDLIB_H
#define STD_STDLIB_H

#ifndef STD_SIZE_T
#define STD_SIZE_T
typedef unsigned long size_t;
#endif

#ifndef STD_ERRNO_T
#define STD_ERRNO_T
typedef int errno_t;
#endif

#ifndef STD_WCHAR_T
#define STD_WCHAR_T
typedef unsigned short wchar_t;
#endif

typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;

typedef void (*constraint_handler_t)(
	const char *msg, void *ptr, errno_t error);

#define NULL ((void *)0)

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 32767
#define MB_CUR_MAX 1

void *malloc(size_t size);
void *_malloc_cat(size_t size, int cat);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void abort(void);
void exit(int status);
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
void srand(unsigned int seed);
int rand(void);
double atof(const char *nptr);
double strtod(const char *nptr, char **endptr);
int atoi(const char *nptr);
long int atol(const char *nptr);
long int strtol(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);
int mblen(const char *s, size_t n);
int mbtowc(wchar_t *pwc, const char *s, size_t n);
int wctomb(char *s, wchar_t wchar);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);
int abs(int j);
div_t div(int numer, int denom);
long int labs(long int j);
ldiv_t ldiv(long int numer, long int denom);
int atexit(void (*func)(void));
char *getenv(const char *name);
int system(const char *string);
void *bsearch(const void *key, const void *base,
              size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

#define		min(a, b)	(((a)<(b))?(a):(b))
#define		max(a, b)	(((a)>(b))?(a):(b))

constraint_handler_t set_constraint_handler_s( constraint_handler_t handler );
errno_t call_constraint_handler_s(const char * msg, void *ptr, errno_t error);

void abort_handler_s( const char * msg, void *ptr, errno_t error);
void ignore_handler_s( const char * msg, void *ptr, errno_t error);

#endif

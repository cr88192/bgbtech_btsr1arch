#ifndef STD_CI_STDIO_H
#define STD_CI_STDIO_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>

typedef struct _stdio_ctxvtable_s	_stdio_ctxvtable_t;
typedef struct _stdio_context_s	_stdio_context_t;
typedef struct _stdio_vtable_s		_stdio_vtable_t;
typedef struct _stdio_iobuf_s		_stdio_iobuf_t;

struct _stdio_ctxvtable_s {
FILE *(*fopen)(_stdio_context_t *ctx,
	const char *filename, const char *mode);
int (*remove)(_stdio_context_t *ctx,
	const char *filename);
int (*rename)(_stdio_context_t *ctx,
	const char *oldname, const char *newname);
FILE **(*get_stream_fd)(_stdio_context_t *ctx, int fd);

void *(*malloc_cat)(_stdio_context_t *ctx, size_t size, int cat);
void *(*realloc)(_stdio_context_t *ctx, void *ptr, size_t size);
void (*free)(_stdio_context_t *ctx, void *ptr);
void (*abort)(_stdio_context_t *ctx);
void (*exit)(_stdio_context_t *ctx, int status);

signal_t (*signal)(_stdio_context_t *ctx, int sig, signal_t func);
int (*raise)(_stdio_context_t *ctx, int sig);
clock_t (*clock)(_stdio_context_t *ctx);

};


struct _stdio_context_s {
_stdio_ctxvtable_t *vt;

errno_t		err_no;
signal_t	sigtab[16];

FILE *openfile[FOPEN_MAX];
};

struct _stdio_vtable_s {
int (*fclose)(FILE *stream);
int (*fflush)(FILE *stream);
int (*feof)(FILE *stream);
long (*ftell)(FILE *stream);
int (*fseek)(FILE *stream, long offset, int whence);

size_t (*fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t (*fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int (*fgetc)(FILE *stream);
int (*fputc)(int c, FILE *stream);
char *(*fgets)(char *s, int n, FILE *stream);
int (*fputs)(const char *s, FILE *stream);

int (*ungetc)(int c, FILE *stream);

int (*ferror((FILE *stream);
void (*clearerr)(FILE *stream);

int (*setvbuf)(FILE *stream, char *buf, int mode, size_t size);
FILE *(*freopen)(const char *filename, const char *mode, FILE *stream);
};

struct _stdio_iobuf_s {
struct _stdio_vtable_s *vt;
_stdio_context_t *ctx;
TK_FILE	*tkfd;
int ifd;
long long ugch;
};


typedef struct _stdio_printfstate_s _stdio_printfstate_t;

struct _stdio_printfstate_s {
unsigned char	*cs;
unsigned char	*cts;
unsigned char	*cte;
unsigned char	*ct;
FILE	*fd;
char	is_wcs;		//

char	fmt_fill;
char	fmt_sgn;
char	fmt_left;
short	fmt_width;
short	fmt_prec;
char	fmt_lmod;
char	fmt_conv;
};

void _vvprintf_readspec(unsigned char **rcs,
	_stdio_printfstate_t *stv);

int _vvprintf(
	_stdio_printfstate_t *stv,
	const char *format, __va_list arg);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <tkclib2/ci_stdio.h>

_stdio_context_t *_stdio_globalcontext_ptr;

_stdio_context_t *_stdio_getcontext(void)
{
	return(_stdio_globalcontext_ptr);
}

FILE **__get_stream_fd(int fd)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->get_stream_fd(ctx, fd));
}

int *__get_errno()
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(&(ctx->err_no));
}

FILE *fopen(const char *filename, const char *mode)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->fopen(ctx, filename, mode));
}

int remove(const char *filename)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->remove(ctx, filename));
}

int rename(const char *oldname, const char *newname)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->rename(ctx, oldname, newname));
}

void perror(const char *s)
{
	if(s)
		{ printf("%s: %s\n", s, strerror(errno)); }
	else
		{ printf("%s\n", strerror(errno)); }
}

int fclose(FILE *stream)
	{ return(stream->vt->fclose(stream)); }
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	{ return(stream->vt->fread(ptr, size, nmemb, stream)); }
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	{ return(stream->vt->fwrite(ptr, size, nmemb, stream)); }
long int ftell(FILE *stream)
	{ return(stream->vt->ftell(stream)); }
int fseek(FILE *stream, long int offset, int whence)
	{ return(stream->vt->fseek(stream, offset, whence)); }

int fgetc(FILE *stream)
	{ return(stream->vt->fgetc(stream)); }
int fputc(int c, FILE *stream)
	{ return(stream->vt->fputc(stream)); }

int ungetc(int c, FILE *stream)
	{ return(stream->vt->ungetc(stream)); }

char *fgets(char *s, int n, FILE *stream)
	{ return(stream->vt->fgets(s, n, stream)); }
int fputs(const char *s, FILE *stream)
	{ return(stream->vt->fputs(s, stream)); }

int feof(FILE *stream)
	{ return(stream->vt->feof(stream)); }
int ferror(FILE *stream)
	{ return(stream->vt->ferror(stream)); }

void clearerr(FILE *stream)
	{ return(stream->vt->clearerr(stream)); }
int fflush(FILE *stream)
	{ return(stream->vt->fflush(stream)); }

FILE *freopen(const char *filename, const char *mode, FILE *stream)
	{ return(stream->vt->freopen(filename, mode, stream)); }

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
	{ return(stream->vt->setvbuf(stream, buf, mode, size)); }

int setbuf(FILE *stream, char *buf)
	{ return(setvbuf(stream, buf, 0, BUFSIZ)); }

int fsetpos(FILE *stream, const fpos_t *pos)
{
	fseek(stream, *pos, SEEK_SET);
	return(0);
}

int fgetpos(FILE *stream, fpos_t *pos)
{
	*pos=ftell(stream);
	return(0);
}

void rewind(FILE *stream)
	{ fseek(stream, 0, SEEK_SET); }

char *gets(char *s)
	{ return(fgets(s, BUFSIZ, stdin)); }
int puts(const char *s)
	{ return(fputs(s, stdout)); }

int getchar(void)
	{ return(fgetc(stdin)); }
int putchar(int c)
	{ return(fputc(c, stdout)); }
int getc(FILE *stream)
	{ return(fgetc(stream)); }
int putc(int c, FILE *stream)
	{ return(fputc(c, stream)); }


int vsnprintf(char *dst, size_t nlim, const char *format, __va_list arg)
{
	_stdio_printfstate_t t_stv;
	_stdio_printfstate_t *stv;

	stv=&t_stv;
	memset(stv, 0, sizeof(_stdio_printfstate_t));

	stv->cts=dst;
	stv->cte=dst+nlim;
	stv->ct=dst;

	return(_vvprintf(stv, format, arg));
}

int vsprintf(char *s, const char *format, __va_list arg)
{
	return(vsnprintf(s, 1<<24, format, arg));
}

int vfprintf(FILE *stream, const char *format, __va_list arg)
{
	_stdio_printfstate_t t_stv;
	_stdio_printfstate_t *stv;

	stv=&t_stv;
	memset(stv, 0, sizeof(_stdio_printfstate_t));

	stv->fd=stream;
	return(_vvprintf(stv, format, arg));
}

int sprintf(char *s, const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vsprintf(s, format, lst);
	va_end(lst);
	return(ret);
}

int snprintf(char *s, size_t n, const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vsnprintf(s, n, format, lst);
	va_end(lst);
	return(ret);
}

int printf(const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vfprintf(stdout, format, lst);
	va_end(lst);
	return(ret);
}

int fprintf(FILE *stream, const char *format, ...);
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vfprintf(stream, format, lst);
	va_end(lst);
	return(ret);
}


int vsscanf(const char *s, const char *format, __va_list arg)
{
	_stdio_printfstate_t t_stv;
	_stdio_printfstate_t *stv;

	stv=&t_stv;
	memset(stv, 0, sizeof(_stdio_printfstate_t));

	stv->cts=s;
	stv->cte=dst+(1<<20);
	stv->ct=s;

	return(_vvscanf(stv, format, arg));
}

int vfscanf(FILE *stream, const char *format, __va_list arg)
{
	_stdio_printfstate_t t_stv;
	_stdio_printfstate_t *stv;

	stv=&t_stv;
	memset(stv, 0, sizeof(_stdio_printfstate_t));
	stv->fdd=stream;
	return(_vvscanf(stv, format, arg));
}

int sscanf(const char *s, const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vsscanf(s, format, lst);
	va_end(lst);
	return(ret);
}

int vscanf(const char *format, __va_list arg)
{
	return(vfscanf(stdin, format, arg));
}

int fscanf(FILE *stream, const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vfscanf(stream, format, lst);
	va_end(lst);
	return(ret);
}

int scanf(const char *format, ...)
{
	va_list lst;
	int ret;
	va_start(lst, format);
	ret=vfscanf(stdin, format, lst);
	va_end(lst);
	return(ret);
}


char *tmpnam(char *s)
{
	static byte rov;
	char tb[16];
	
	strcpy(tb, "zzzzz000.zzz");
	tb[5]='0'+(rov/100);
	tb[6]='0'+((rov/10)%10);
	tb[7]='0'+(rov%10);
	strcpy(s, tb);
	return(s);
}

FILE *tmpfile(void)
{
	char tb[128];
	tmpnam(tb);
	return (fopen(tb, "wb+"));
}

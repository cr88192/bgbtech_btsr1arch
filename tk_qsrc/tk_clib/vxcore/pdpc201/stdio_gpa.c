/*
Written 2021 by Brendan G Bohannon.

This is a vtable based wrapper for the stdio.h functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <tk_libcgpa.h>

void __init_stdin(void);

#define		stdio_doinit()		if(!tk_stdio_vtp)__init_stdin()

struct tk_stdio_vt *tk_stdio_vtp;

void *(*_malloc_fptr)(size_t size, int cat);
void (*_free_fptr)(void *ptr);
void *(*_realloc_fptr)(void *ptr, size_t size);
size_t (*_msize_fptr)(void *ptr);

void *(*_mgetbase_fptr)(void *ptr);

int (*_mgettag_fptr)(void *ptr);
int (*_msettag_fptr)(void *ptr, int tag);
int (*_mgetzone_fptr)(void *ptr);
int (*_msetzone_fptr)(void *ptr, int ztag);
int (*_mfreezone_fptr)(int ztag, int zmask);

FILE *__stdin_p;
FILE *__stdout_p;
FILE *__stderr_p;

__PDPCLIB_API__ FILE **__get_stdin()
{
	stdio_doinit();
	return(&__stdin_p);
}
__PDPCLIB_API__ FILE **__get_stdout()
{
	stdio_doinit();
	return(&__stdout_p);
}
__PDPCLIB_API__ FILE **__get_stderr()
{
	stdio_doinit();
	return(&__stderr_p);
}

int *(*__get_errno_fp)();
__PDPCLIB_API__ int *__get_errno()
{
	stdio_doinit();
	return(__get_errno_fp());
}

long __multicall(int callnum, void *retptr, void **args)
{
	long (*multicall_fp)(int callnum, void *retptr, void **args);
	long ret;
	stdio_doinit();
	
	multicall_fp=tk_stdio_vtp->multicall_fp;
	ret=multicall_fp(callnum, retptr, args);
	return(ret);
}

void __init_stdin(void)
{
	if(tk_stdio_vtp)
		return;

	TkClGetProcAddressCn((void **)(&tk_stdio_vtp), "stdio_vt");
	if(!tk_stdio_vtp)
	{
		__debugbreak();
		return;
	}

	if(tk_stdio_vtp->key1a!=_TK_STDIO_KEY1A)
	{
		__debugbreak();
	}

	__stdin_p=fopen("$STDIN", NULL);
	__stdout_p=fopen("$STDOUT", NULL);
	__stderr_p=fopen("$STDERR", NULL);
	
	_malloc_fptr=tk_stdio_vtp->malloc_fp;
	_free_fptr=tk_stdio_vtp->free_fp;
	_realloc_fptr=tk_stdio_vtp->realloc_fp;
	_msize_fptr=tk_stdio_vtp->msize_fp;

	_mgetbase_fptr=tk_stdio_vtp->mgetbase_fp;
	_mfreezone_fptr=tk_stdio_vtp->mfreezone_fp;

	_mgettag_fptr=tk_stdio_vtp->mgettag_fp;
	_msettag_fptr=tk_stdio_vtp->msettag_fp;
	_mgetzone_fptr=tk_stdio_vtp->mgetzone_fp;
	_msetzone_fptr=tk_stdio_vtp->msetzone_fp;
	
	__get_errno_fp=tk_stdio_vtp->geterrno_fp;
}

FILE *fopen(const char *filename, const char *mode)
{
	FILE *(*fopen_fp)(const char *filename, const char *mode);
	stdio_doinit();
	fopen_fp=tk_stdio_vtp->fopen_fp;
	return(fopen_fp(filename, mode));
}

int fclose(FILE *stream)
{
	int (*fclose_fp)(FILE *stream);
	fclose_fp=tk_stdio_vtp->fclose_fp;
	return(fclose_fp(stream));
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t (*fread_fp)(void *ptr, size_t size, size_t nmemb, FILE *stream);
	fread_fp=tk_stdio_vtp->fread_fp;
	return(fread_fp(ptr, size, nmemb, stream));
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t (*fwrite_fp)(const void *ptr,
		size_t size, size_t nmemb, FILE *stream);
	fwrite_fp=tk_stdio_vtp->fwrite_fp;
	return(fwrite_fp(ptr, size, nmemb, stream));
}

int fgetc(FILE *stream)
{
	int (*fgetc_fp)(FILE *stream);
	fgetc_fp=tk_stdio_vtp->fgetc_fp;
	return(fgetc_fp(stream));
}

int fputc(int c, FILE *stream)
{
	int (*fputc_fp)(int c, FILE *stream);
	fputc_fp=tk_stdio_vtp->fputc_fp;
	return(fputc_fp(c, stream));
}

char *fgets(char *s, int n, FILE *stream)
{
	char *(*fgets_fp)(char *s, int n, FILE *stream);
//	stdio_doinit();
	fgets_fp=tk_stdio_vtp->fgets_fp;
	return(fgets_fp(s, n, stream));
}

int fputs(const char *s, FILE *stream)
{
	int (*fputs_fp)(const char *s, FILE *stream);
//	stdio_doinit();
	fputs_fp=tk_stdio_vtp->fputs_fp;
	return(fputs_fp(s, stream));
}

int ungetc(int c, FILE *stream)
{
	int (*ungetc_fp)(int c, FILE *stream);
	ungetc_fp=tk_stdio_vtp->ungetc_fp;
	return(ungetc_fp(c, stream));
}

int fseek(FILE *stream, long int offset, int whence)
{
	int (*fseek_fp)(FILE *stream, long int offset, int whence);
	fseek_fp=tk_stdio_vtp->fseek_fp;
	return(fseek_fp(stream, offset, whence));
}

int feof(FILE *stream)
{
	int (*feof_fp)(FILE *stream);
	feof_fp=tk_stdio_vtp->feof_fp;
	return(feof_fp(stream));
}

long int ftell(FILE *stream)
{
	long int (*ftell_fp)(FILE *stream);
	ftell_fp=tk_stdio_vtp->ftell_fp;
	return(ftell_fp(stream));
}

int fflush(FILE *stream)
{
	int (*fflush_fp)(FILE *stream);
	fflush_fp=tk_stdio_vtp->fflush_fp;
	return(fflush_fp(stream));
}

int getc(FILE *stream)
	{ return(fgetc(stream)); }

int putc(int c, FILE *stream)
	{ return(fputc(c, stream)); }

char *tmpnam(char *s)
{
	char *(*tmpnam_fp)(char *s);
	stdio_doinit();
	tmpnam_fp=tk_stdio_vtp->tmpnam_fp;
	return(tmpnam_fp(s));
}

FILE *tmpfile(void)
{
	char tbuf[L_tmpnam];
	tmpnam(tbuf);
	return(fopen(tbuf, "w+b"));
}


char *gets(char *s)
{
	return(fgets(s, 79, stdin));
}

int puts(const char *s)
{
	return(fputs(s, stdout));
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
	FILE *(*freopen_fp)(const char *filename,
		const char *mode, FILE *stream);
	freopen_fp=tk_stdio_vtp->freopen_fp;
	return(freopen_fp(filename, mode, stream));
}

void clearerr(FILE *stream)
{
	void (*clearerr_fp)(FILE *stream);
	clearerr_fp=tk_stdio_vtp->clearerr_fp;
	clearerr_fp(stream);
}

void perror(const char *s)
{
	void (*perror_fp)(const char *s);
	stdio_doinit();
	perror_fp=tk_stdio_vtp->perror_fp;
	perror_fp(s);
}

int ferror(FILE *stream)
{
	int (*ferror_fp)(FILE *stream);
	ferror_fp=tk_stdio_vtp->ferror_fp;
	return(ferror_fp(stream));
}


int remove(const char *filename)
{
	int (*remove_fp)(const char *s);
	stdio_doinit();
	remove_fp=tk_stdio_vtp->remove_fp;
	return(remove_fp(filename));
}

int rename(const char *oldname, const char *newname)
{
	int (*rename_fp)(
		const char *oldname,
		const char *newname,
		const char *mode);
	stdio_doinit();
	rename_fp=tk_stdio_vtp->rename_fp;
	return(rename_fp(oldname, newname, "r"));
}

int link(const char *oldname, const char *newname)
{
	int (*rename_fp)(
		const char *oldname,
		const char *newname,
		const char *mode);
	stdio_doinit();
	rename_fp=tk_stdio_vtp->rename_fp;
	return(rename_fp(oldname, newname, "l"));
}

int symlink(const char *oldname, const char *newname)
{
	int (*rename_fp)(
		const char *oldname,
		const char *newname,
		const char *mode);
	stdio_doinit();
	rename_fp=tk_stdio_vtp->rename_fp;
	return(rename_fp(oldname, newname, "S"));
}

int fsetpos(FILE *stream, const fpos_t *pos)
{
	fseek(stream, *pos, SEEK_SET);
	return (0);
}

int fgetpos(FILE *stream, fpos_t *pos)
{
	*pos=ftell(stream);
	return(0);
}

void rewind(FILE *stream)
{
	fseek(stream, 0, 0);
}

int getchar(void)
	{ return(fgetc(stdin)); }

int putchar(int c)
	{ return(fputc(c, stdout)); }


int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	int (*setvbuf_fp)(FILE *stream, char *buf, int mode, size_t size);
	setvbuf_fp=tk_stdio_vtp->setvbuf_fp;
	return(setvbuf_fp(stream, buf, mode, size));
}

void *TkClGetInterface(u64 qwMajor, u64 qwMinor)
{
	int (*getiface_fp)(u64 major, u64 minor);
	getiface_fp=tk_stdio_vtp->getinterface_fp;
	if(!getiface_fp)
		return(NULL);
	return(getiface_fp(qwMajor, qwMinor));
}

void *TkClSetInterface(u64 qwMajor, u64 qwMinor, void *vtab, u64 flag)
{
	int (*setiface_fp)(u64 major, u64 minor);
	setiface_fp=tk_stdio_vtp->setinterface_fp;
	if(!setiface_fp)
		return(NULL);
	return(setiface_fp(qwMajor, qwMinor, vtab, flag));
}

int setbuf(FILE *stream, char *buf)
{
	int ret;

	if (buf == NULL)
	{
		ret = setvbuf(stream, NULL, _IONBF, 0);
	}
	else
	{
		ret = setvbuf(stream, buf, _IOFBF, BUFSIZ);
	}
	return (ret);
}


int vfprintf(FILE *stream, const char *format, va_list arg)
{
	int (*vfprintf_fp)(FILE *stream, const char *format, va_list arg);
	vfprintf_fp=tk_stdio_vtp->vfprintf_fp;
	return(vfprintf_fp(stream, format, arg));
}

int vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
	int (*vsnprintf_fp)(char *s, size_t n, const char *format, va_list arg);
	stdio_doinit();
	vsnprintf_fp=tk_stdio_vtp->vsnprintf_fp;
	return(vsnprintf_fp(s, n, format, arg));
}

int vsprintf(char *s, const char *format, va_list arg)
{
	return(vsnprintf(s, 99999999, format, arg));
}

int fprintf(FILE *stream, const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vfprintf(stream, format, args);
	va_end(args);
	return(ret);
}

int snprintf(char *s, size_t n, const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vsnprintf(s, n, format, args);
	va_end(args);
	return(ret);
}

int sprintf(char *s, const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vsprintf(s, format, args);
	va_end(args);
	return(ret);
}


int printf(const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vfprintf(stdout, format, args);
	va_end(args);
	return(ret);
}

int vfscanf(FILE *stream, const char *format, va_list arg)
{
	int (*vfscanf_fp)(FILE *stream, const char *format, va_list arg);
	vfscanf_fp=tk_stdio_vtp->vfscanf_fp;
	return(vfscanf_fp(stream, format, arg));
}

int vsscanf(const char *s, const char *format, va_list arg)
{
	int (*vsscanf_fp)(const char *s, const char *format, va_list arg);
	stdio_doinit();
	vsscanf_fp=tk_stdio_vtp->vsscanf_fp;
	return(vsscanf_fp(s, format, arg));
}

int vscanf(const char *format, va_list arg)
{
	return(vfscanf(stdin, format, arg));
}

int fscanf(FILE *stream, const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vfscanf(stream, format, args);
	va_end(args);
	return(ret);
}

int scanf(const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vfscanf(stdin, format, args);
	va_end(args);
	return(ret);
}

int sscanf(const char *s, const char *format, ...)
{
	va_list args;
	int ret;
	va_start(args, format);
	ret=vsscanf(s, format, args);
	va_end(args);
	return(ret);
}

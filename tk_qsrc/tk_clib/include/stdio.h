/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stdio.h - stdio header file                                      */
/*                                                                   */
/*********************************************************************/

#ifndef __STDIO_INCLUDED
#define __STDIO_INCLUDED

// #include <stdarg.h>

#define __TESTKERN__

#if 0
#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
#if (defined(__OS2__) || defined(__32BIT__) || defined(__MVS__) \
    || defined(__CMS__))
typedef unsigned long size_t;
#elif (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC) \
    || defined(__WIN32__) || defined(__gnu_linux__) \
    || defined(_WIN32) || defined(linux))
typedef unsigned int size_t;
#endif
#endif
#endif

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
// typedef unsigned int size_t;
// typedef long size_t;
// typedef unsigned long size_t;
#ifdef __BGBCC__
typedef __usize_t size_t;
#else
typedef unsigned long size_t;
#endif
#endif


#if defined(__GNUC__) || defined(__gnuc__) || defined(_BGBMETA)

//#ifndef __GNUC_VA_LIST
//#define __GNUC_VA_LIST
//typedef __builtin_va_list __gnuc_va_list;
//#endif

typedef __builtin_va_list __va_list;
#else

typedef char *__va_list;

#endif


/*
    What we have is an internal buffer, which is 8 characters
    longer than the actually used buffer.  E.g. say BUFSIZ is
    512 bytes, then we actually allocate 520 bytes.  The first
    2 characters will be junk, the next 2 characters set to NUL,
    for protection against some backward-compares.  The fourth-last
    character is set to '\n', to protect against overscan.  The
    last 3 characters will be junk, to protect against memory
    violation.  intBuffer is the internal buffer, but everyone refers
    to fbuf, which is actually set to the &intBuffer[4].  Also,
    szfbuf is the size of the "visible" buffer, not the internal
    buffer.  The reason for the 2 junk characters at the beginning
    is to align the buffer on a 4-byte boundary.
    
    Here is what memory would look like after an fwrite of "ABC"
    to an MVS LRECL=80, RECFM=F dataset:
    
    intbuffer: x'50000'
    fbuf:      x'50004'
    upto:      x'50007'
    endbuf:    x'58004'
    
    x'50004' = x'C1'
    x'50005' = x'C2'
    x'50006' = x'C3'
*/

#ifndef _FILE_MAGIC
#define _FILE_MAGIC	0x1414237
#endif

#ifndef _FILE_DEFINED /* Also in stdio.h */
#define	_FILE_DEFINED
typedef struct _iobuf FILE;
#endif /* Not _FILE_DEFINED */

struct _iobuf
{
#if (defined(__OS2__) || defined(__32BIT__))
    unsigned long hfile;  /* OS/2 file handle */
#elif (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC))
    int hfile; /* dos file handle */
#elif defined(__WIN32__)
    void *hfile;
#elif defined(__gnu_linux__)
    int hfile;
#else
    int hfile;
#endif

	int magic1;

#if (defined(__MVS__) || defined(__CMS__))
    void *hfile;
    void *asmbuf;
    int recfm;
    int style;
    int lrecl;
    int blksize;
    char ddname[9];
    char pdsmem[9];
    int reallyu;    /* 1 = this is really a RECFM=U file */
    int reallyt;    /* 1 = this is really a text file */
    int dynal;      /* 1 = this file was dynamically allocated */
#endif

    char quickBin;  /* 1 = do DosRead NOW!!!! */
    char quickText; /* 1 = quick text mode */
	char textMode; /* 1 = text mode, 0 = binary mode */
    int intFno;   /* internal file number */
    unsigned long bufStartR; /* buffer start represents, e.g. if we
        have read in 3 buffers, each of 512 bytes, and we are
        currently reading from the 3rd buffer, then the first
        character in the buffer would be 1024, so that is what is
        put in bufStartR. */
    char *fbuf;     /* file buffer - this is what all the routines
                       look at. */
    size_t szfbuf;  /* size of file buffer (the one that the routines
                       see, and the user allocates, and what is actually
                       read in from disk) */
    char *upto;     /* what character is next to read from buffer */
    char *endbuf;   /* pointer PAST last character in buffer, ie it
                       points to the '\n' in the internal buffer */
    int errorInd;   /* whether an error has occurred on this file */
    int eofInd;     /* whether EOF has been reached on this file */
    int ungetCh;    /* character pushed back, -1 if none */
    int bufTech;    /* buffering technique, _IOFBF etc */
    char *intBuffer; /* internal buffer */
    int noNl;       /* When doing gets, we don't copy NL */
    int mode;       /* __WRITE_MODE or __READ_MODE */
    int update;     /* Is file update (read + write)? */
    int theirBuffer; /* Is the buffer supplied by them? */
    int permfile;   /* Is this stdin/stdout/stderr? */
    int isopen;     /* Is this file open? */
    char modeStr[4]; /* 2nd parameter to fopen */

	int magic2;
};

typedef unsigned long fpos_t;

#define NULL ((void *)0)
#define FILENAME_MAX 260
#define FOPEN_MAX 256
#define _IOFBF 1
#define _IOLBF 2
#define _IONBF 3

/*#define BUFSIZ 409600*/
/* #define BUFSIZ 8192 */
/*#define BUFSIZ 5120*/

#if defined(__MVS__)
/* set it to maximum possible LRECL to simplify processing */
/* also add in room for a RDW and dword align it just to be
   on the safe side */
#define BUFSIZ 32768
#elif defined(__CMS__)
/* similar considerations for CMS as MVS */
#define BUFSIZ 65544
#else
#define BUFSIZ 6144
#endif
/* #define BUFSIZ 10 */
/* #define BUFSIZ 512 */
#define EOF -1
#define L_tmpnam FILENAME_MAX
#define TMP_MAX 25
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define __NFILE (FOPEN_MAX - 3)
#define __WRITE_MODE 1
#define __READ_MODE 2

#if 0
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

extern FILE *__userFiles[__NFILE];
#endif

FILE **__get_stdin();
FILE **__get_stdout();
FILE **__get_stderr();

#define stdin (*(__get_stdin()))
#define stdout (*(__get_stdout()))
#define stderr (*(__get_stderr()))

int printf(const char *format, ...);
FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, __va_list arg);
int remove(const char *filename);
int rename(const char *old, const char *new);
int sprintf(char *s, const char *format, ...);
int snprintf(char *s, size_t n, const char *format, ...);
int vsprintf(char *s, const char *format, __va_list arg);
int vsnprintf(char *s, size_t n, const char *format, __va_list arg);
char *fgets(char *s, int n, FILE *stream);
int ungetc(int c, FILE *stream);
int fgetc(FILE *stream);
int fseek(FILE *stream, long int offset, int whence);
long int ftell(FILE *stream);
int fsetpos(FILE *stream, const fpos_t *pos);
int fgetpos(FILE *stream, fpos_t *pos);
void rewind(FILE *stream);
void clearerr(FILE *stream);
void perror(const char *s);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
int setbuf(FILE *stream, char *buf);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fflush(FILE *stream);
char *tmpnam(char *s);
FILE *tmpfile(void);
int fscanf(FILE *stream, const char *format, ...);
int vfscanf(FILE *stream, const char *format, __va_list arg);
int scanf(const char *format, ...);
int vscanf(const char *format, __va_list arg);
int sscanf(const char *s, const char *format, ...);
int vsscanf(const char *s, const char *format, __va_list arg);
char *gets(char *s);
int puts(const char *s);

// #ifndef __POWERC
int getchar(void);
int putchar(int c);
int getc(FILE *stream);
int putc(int c, FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
// #endif

// #define getchar() (getc(stdin))
// #define putchar(c) (putc((c), stdout))
// #define getc(stream) (fgetc((stream)))
// #define putc(c, stream) (fputc((c), (stream)))
// #define feof(stream) ((stream)->eofInd)
// #define ferror(stream) ((stream)->errorInd)

#endif



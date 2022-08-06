#ifndef STD_STDIO_H
#define STD_STDIO_H

#ifndef STD_SIZE_T
#define STD_SIZE_T
typedef unsigned long size_t;
#endif

typedef __builtin_va_list __va_list;


#ifndef STD_FILE_T
#define	STD_FILE_T
typedef struct _stdio_iobuf_s FILE;
#endif

typedef unsigned long fpos_t;

#define NULL ((void *)0)
#define FILENAME_MAX 260
#define FOPEN_MAX 256
#define _IOFBF 1
#define _IOLBF 2
#define _IONBF 3

#define BUFSIZ 6144

#define EOF -1
#define L_tmpnam FILENAME_MAX
#define TMP_MAX 25
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

FILE **__get_stream_fd(int fd);

#define stdin (*(__get_stream_fd(0)))
#define stdout (*(__get_stream_fd(1)))
#define stderr (*(__get_stream_fd(2)))

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

int getchar(void);
int putchar(int c);
int getc(FILE *stream);
int putc(int c, FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);

#endif



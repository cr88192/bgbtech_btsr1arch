#ifndef __TK_LIBCGPA_H
#define __TK_LIBCGPA_H

#include <tk_core.h>

struct tk_stdio_vt {
void *fopen_fp;
void *fclose_fp;
void *fread_fp;
void *fwrite_fp;

void *fgetc_fp;
void *fputc_fp;
void *fgets_fp;
void *fputs_fp;

void *ungetc_fp;
void *fseek_fp;
void *ftell_fp;
void *fflush_fp;

void *feof_fp;
void *freopen_fp;
void *remove_fp;
void *rename_fp;

void *vfprintf_fp;
void *vsnprintf_fp;
void *vfscanf_fp;
void *vsscanf_fp;

void *clearerr_fp;
void *perror_fp;
void *setvbuf_fp;
void *tmpnam_fp;

void *ferror_fp;

void *malloc_fp;
void *free_fp;
void *realloc_fp;
void *msize_fp;
};

#endif

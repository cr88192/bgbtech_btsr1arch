#ifndef __TK_LIBCGPA_H
#define __TK_LIBCGPA_H

#include <tk_core.h>

#define		_TK_STDIO_KEY1A		((void *)0x314159)

struct tk_stdio_vt {
void *spec0;			//00
void *spec1;			//01
void *spec2;			//02
void *spec3;			//03

void *fopen_fp;			//04
void *fclose_fp;		//05
void *fread_fp;			//06
void *fwrite_fp;		//07

void *fgetc_fp;			//08
void *fputc_fp;			//09
void *fgets_fp;			//0A
void *fputs_fp;			//0B

void *ungetc_fp;		//0C
void *fseek_fp;			//0D
void *ftell_fp;			//0E
void *fflush_fp;		//0F

void *feof_fp;			//10
void *freopen_fp;		//11
void *remove_fp;		//12
void *rename_fp;		//13

void *vfprintf_fp;		//14
void *vsnprintf_fp;		//15
void *vfscanf_fp;		//16
void *vsscanf_fp;		//17

void *clearerr_fp;		//18
void *perror_fp;		//19
void *setvbuf_fp;		//1A
void *tmpnam_fp;		//1B

void *ferror_fp;		//1C
void *key1a;			//1D
void *mgetbase_fp;		//1E
void *mfreezone_fp;		//1F

void *malloc_fp;		//20
void *free_fp;			//21
void *realloc_fp;		//22
void *msize_fp;			//23

void *mgettag_fp;		//24
void *msettag_fp;		//25
void *mgetzone_fp;		//26
void *msetzone_fp;		//27

void *opendir_fp;		//28
void *closedir_fp;		//29
void *readdir_fp;		//2A
void *geterrno_fp;		//2B

void *mmap_fp;			//2C
void *munmap_fp;		//2D
void *msync_fp;			//2E
void *mprotect_fp;		//2F

void *setlocale_fp;		//30
void *localeconv_fp;	//31
void *signal_fp;		//32
void *raise_fp;			//33

void *dlopen_fp;		//34
void *dlclose_fp;		//35
void *dlsym_fp;			//36
void *dlerror_fp;		//37

void *open_fp;			//38
void *close_fp;			//39
void *read_fp;			//3A
void *write_fp;			//3B

void *lseek_fp;			//3C
void *ioctl_fp;			//3D
void *sendto_fp;		//3E
void *recvfrom_fp;		//3F

void *bind_fp;			//40
void *accept_fp;		//41
void *connect_fp;		//42
void *multicall_fp;		//43

void *exita_fp;			//44
void *gettimeus_fp;		//45

void *getinterface_fp;	//46
void *setinterface_fp;	//47
void *resv2;
void *resv3;
void *resv4;
void *resv5;
void *resv6;
void *resv7;
void *resv8;
void *resv9;
void *resv10;
void *resv11;
};

#endif

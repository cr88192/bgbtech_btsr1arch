// #ifdef ARCH_SH4
#if defined(ARCH_SH4) || defined(ARCH_BJX1) || defined(ARCH_BJX1_64)

#ifndef ARCH_HAS_FPU
#define ARCH_HAS_FPU
#endif

#ifndef ARCH_HAS_MMU
#define ARCH_HAS_MMU
#endif

#endif

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile u32 vol_u32;
typedef volatile u64 vol_u64;

#define	bool				_Bool
#define	true				1
#define	false				0

// #define GPIO_BASE 0xABCD0000
// #define SPI_BASE 0xABCD0040
// #define UART_BASE 0xABCD0100
// #define AIC_BASE 0xABCD0200

// #define GPIO_BASE	0xA000E100
// #define SPI_BASE	0xA000E030
// #define UART_BASE	0xA000E010
// #define AIC_BASE	0xA000E200

#define GPIO_BASE	0xF000E100
#define SPI_BASE	0xF000E030
#define UART_BASE	0xF000E010
#define AIC_BASE	0xF000E200

#define PS2_BASE	0xF000E040

#define UART_RX		(UART_BASE+0x00)
#define UART_TX		(UART_BASE+0x04)
#define UART_STAT	(UART_BASE+0x08)
#define UART_CTRL	(UART_BASE+0x0C)

#define SPI_CTRL	(SPI_BASE+0x00)
#define SPI_DATA	(SPI_BASE+0x04)
#define SPI_QDATA	(SPI_BASE+0x08)

#define PS2KB_RX	(PS2_BASE+0x00)
#define PS2KB_TX	(PS2_BASE+0x04)
#define PS2KB_STAT	(PS2_BASE+0x08)
#define PS2KB_CTRL	(PS2_BASE+0x0C)
#define PS2MS_RX	(PS2_BASE+0x10)
#define PS2MS_TX	(PS2_BASE+0x14)
#define PS2MS_STAT	(PS2_BASE+0x18)
#define PS2MS_CTRL	(PS2_BASE+0x1C)

#define AIC_CTRL0			(AIC_BASE+0x00)
#define AIC_BRKADD			(AIC_BASE+0x04)
#define AIC_ILEVELS			(AIC_BASE+0x08)
#define AIC_CTRL1			(AIC_BASE+0x0C)
#define AIC_PIT_THROTTLE	(AIC_BASE+0x10)
#define AIC_PIT_COUNTER		(AIC_BASE+0x14)
#define AIC_CLK_PERIOD		(AIC_BASE+0x18)
#define AIC_IGNORE0			(AIC_BASE+0x1C)
#define AIC_RTC_SEC_HI		(AIC_BASE+0x20)
#define AIC_RTC_SEC_LO		(AIC_BASE+0x24)
#define AIC_RTC_NSEC		(AIC_BASE+0x28)

#define P_UART_RX	(*(vol_u32 *)UART_RX)
#define P_UART_TX	(*(vol_u32 *)UART_TX)
#define P_UART_STAT	(*(vol_u32 *)UART_STAT)
#define P_UART_CTRL	(*(vol_u32 *)UART_CTRL)

#define	P_MMIO_DEBUG_RX		P_UART_RX
#define	P_MMIO_DEBUG_TX		P_UART_TX
#define	P_MMIO_DEBUG_STS	P_UART_STAT

#define P_SPI_CTRL	(*(vol_u32 *)SPI_CTRL)
#define P_SPI_DATA	(*(vol_u32 *)SPI_DATA)
#define P_SPI_QDATA	(*(vol_u64 *)SPI_QDATA)

#define P_AIC_RTC_SEC_HI	(*(vol_u32 *)AIC_RTC_SEC_HI)
#define P_AIC_RTC_SEC_LO	(*(vol_u32 *)AIC_RTC_SEC_LO)
#define P_AIC_RTC_NSEC		(*(vol_u32 *)AIC_RTC_NSEC)

#define P_PS2KB_RX		(*(vol_u32 *)PS2KB_RX)
#define P_PS2KB_TX		(*(vol_u32 *)PS2KB_TX)
#define P_PS2KB_STAT	(*(vol_u32 *)PS2KB_STAT)
#define P_PS2KB_CTRL	(*(vol_u32 *)PS2KB_CTRL)

#define P_PS2MS_RX		(*(vol_u32 *)PS2MS_RX)
#define P_PS2MS_TX		(*(vol_u32 *)PS2MS_TX)
#define P_PS2MS_STAT	(*(vol_u32 *)PS2MS_STAT)
#define P_PS2MS_CTRL	(*(vol_u32 *)PS2MS_CTRL)

#define MMREG_BASE 0xFF000000
#define MMREG_PTEH	(MMREG_BASE+0x00)
#define MMREG_PTEL	(MMREG_BASE+0x04)
#define MMREG_TTB	(MMREG_BASE+0x08)
#define MMREG_TEA	(MMREG_BASE+0x0C)
#define MMREG_MMUCR	(MMREG_BASE+0x10)

#define P_MMREG_PTEH	(*(vol_u32 *)MMREG_PTEH)
#define P_MMREG_PTEL	(*(vol_u32 *)MMREG_PTEL)
#define P_MMREG_TTB		(*(vol_u32 *)MMREG_TTB)
#define P_MMREG_TEA		(*(vol_u32 *)MMREG_TEA)
#define P_MMREG_MMUCR	(*(vol_u32 *)MMREG_MMUCR)

// #ifdef ARCH_SH4
// #define TKMM_PAGEBASE	0x90000000
// #define TKMM_PAGEEND	0x98000000
//#else
//#define TKMM_PAGEBASE	0x11000000
//#define TKMM_PAGEEND	0x18000000
//#endif

// #define TKMM_PAGEBASE	0x10000000
// #define TKMM_PAGEEND	0x18000000

// #define TKMM_PAGEBASE	0x04000000
// #define TKMM_PAGEBASE	0x03000000
// #define TKMM_PAGEBASE	0x02000000
#define TKMM_PAGEBASE	0x01400000
// #define TKMM_PAGEEND	0x18000000
#define TKMM_PAGEEND	0x08000000

// #define INITRD_ADDR	0x1003F010
// #define INITRD_SIZE	0x1003F014
// #define KINIT_ADDR	0x1003F100

// #define P_INITRD_ADDR	(*(vol_u32 *)INITRD_ADDR)
// #define P_INITRD_SIZE	(*(vol_u32 *)INITRD_SIZE)
// #define P_KINIT_ADDR	((char *)KINIT_ADDR)

// #define P_INITRD_ABSADDR	((void *)(0x10000000+P_INITRD_ADDR))

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

#endif

#ifndef __cplusplus

#ifndef _BOOL_T
#define _BOOL_T
// typedef unsigned char tk_bool;
// #define bool tk_bool
#endif

#ifndef true
#define true 1
#define false 0
#endif

#endif

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

typedef struct TKMM_MemLnkObj_s TKMM_MemLnkObj;

struct TKMM_MemLnkObj_s {
TKMM_MemLnkObj *cnext;	//next object in chunk
TKMM_MemLnkObj *cprev;	//previous object in chunk
u16 ix;					//size index value
u16 dty_tag;			//type tag
byte fl;				//object flags
byte check;				//check value
byte refcnt;			//reference count
byte pad;
// int flag;
// u32 size;
//double data[1];	//start of data
u32 data[1];	//start of data
};

#define TKMM_OFFS_DATA	((int)(((TKMM_MemLnkObj *)0)->data))

typedef struct TK_FILE_VT_s TK_FILE_VT;
typedef struct TK_FILE_s TK_FILE;
typedef struct TK_FSTAT_s TK_FSTAT;
// typedef struct TK_DIR_s TK_DIR;
typedef struct TK_FILE_s TK_DIR;
typedef struct TK_MOUNT_s TK_MOUNT;
typedef struct TK_DIRENT_s TK_DIRENT;
typedef struct TK_DATETIME_s TK_DATETIME;

struct TK_FILE_VT_s {
char *fsname;
TK_FILE_VT *next;

TK_MOUNT *(*mount)(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);

TK_FILE *(*fopen)(TK_MOUNT *mnt, char *name, char *mode);
TK_DIR *(*opendir)(TK_MOUNT *mnt, char *name);
int (*unlink)(TK_MOUNT *mnt, char *name);
int (*rename)(TK_MOUNT *mnt, char *oldfn, char *newfn);
int (*fstat)(TK_MOUNT *mnt, char *name, TK_FSTAT *st);

/* FILE Ops */
int (*fread)(void *buf, int sz1, int sz2, TK_FILE *fd);
int (*fwrite)(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 (*fseek)(TK_FILE *fd, s64 ofs, int rel);
s64 (*ftell)(TK_FILE *fd);
int (*fclose)(TK_FILE *fd);
int (*fgetc)(TK_FILE *fd);
int (*fputc)(int ch, TK_FILE *fd);
int (*fioctl)(TK_FILE *fd, int cmd, void *ptr);

/* DIR ops */
TK_DIRENT *(*readdir)(TK_DIR *dir);
int (*closedir)(TK_DIR *dir);
};

struct TK_FILE_s {
TK_FILE_VT *vt;
void *udata0;		//user-data (VFS driver)
void *udata1;		//user-data (VFS driver)
void *udata2;		//user-data (VFS driver)
void *udata3;		//user-data (VFS driver)
byte *ram_base;		//ram base (ramdisk)
byte *ram_end;		//ram end (ramdisk)
byte *ram_ofs;		//ram offset (ramdisk)
int ifd;			//file handle
u32 ofs;			//current file offset
u32 size;			//file size
};

struct TK_MOUNT_s {
TK_FILE_VT *vt;
TK_MOUNT *next;
char *src;
char *tgt;
int szSrc;
void *udata0;
void *udata1;
};

struct TK_DIRENT_s {
int d_ino;
int d_off;
u16 d_reclen;
byte d_type;
char d_name[256];

/* Stat Stuff */
char st_link[256];
u32 st_mode;
u16 st_uid;
u16 st_gid;
u32 st_ctime;
u32 st_mtime;
u32 st_size;
};

struct TK_DATETIME_s {
s16 year;
byte mon;
byte day;
byte hour;
byte min;
byte sec;
byte msc4;		//multiple of ~4 milliseconds
u16 usc4;		//microseconds, range=4096
};

#ifndef TK_APIEXPORT
#ifdef __BGBCC__
#define TK_APIEXPORT	__declspec(dllexport)
#else
#define TK_APIEXPORT
#endif
#endif

#include <tk_fatfs.h>
// #include <tk_dummyavi.h>
#include <tk_varobj.h>
#include <tk_keys.h>
#include <tk_romfcn.h>
#include <tk_pgmimg.h>

u32 __float32_getbits(float f);
float __float32_frombits(u32 f);
u64 __float64_getbits(double f);
double __float64_frombits(u64 f);

void *TKMM_Malloc(int sz);
int TKMM_Free(void *ptr);
char *TKMM_LVA_Strdup(char *str);

// void printf(char *str, ...);

// void *malloc(int sz);
// int free(void *ptr);

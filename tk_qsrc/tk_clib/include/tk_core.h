#ifndef __TK_CORE_H__
#define __TK_CORE_H__


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

typedef unsigned __int128 u128;
typedef signed __int128 s128;

typedef volatile u32 vol_u32;
typedef volatile u64 vol_u64;

typedef u64 tk_kernptr;
typedef u64 tk_kptr;

#define	bool				_Bool
#define	true				1
#define	false				0

#ifdef __BJX2__
#define TK_NOCULL	__declspec(nocull)
#else
#define TK_NOCULL
#endif

// #define GPIO_BASE 0xABCD0000
// #define SPI_BASE 0xABCD0040
// #define UART_BASE 0xABCD0100
// #define AIC_BASE 0xABCD0200

// #define GPIO_BASE	0xA000E100
// #define SPI_BASE	0xA000E030
// #define UART_BASE	0xA000E010
// #define AIC_BASE	0xA000E200

// #ifdef __ADDR_X48__
#if 1
#define MMIO_BASE	0xFFFFF0000000ULL
#define MMIO_END	0xFFFFFFFFFFF0ULL
#else
#define MMIO_BASE	0xF0000000ULL
#define MMIO_END	0xFFFFFFF0ULL
#endif

#ifndef RIFF_MAKETAG
#define RIFF_MAKETAG(a, b, c, d)	((a)+((b)<<8)+((c)<<16)+((d)<<24))
#endif

#define TK_FCC_GDI	RIFF_MAKETAG('G','D','I',' ')
#define TK_FCC_TKRA	RIFF_MAKETAG('T','K','R','A')

// #define GPIO_BASE	0xF000E100ULL
// #define SPI_BASE	0xF000E030ULL
// #define UART_BASE	0xF000E010ULL
// #define AIC_BASE	0xF000E200ULL
// #define PS2_BASE	0xF000E040ULL

#define GPIO_BASE	(MMIO_BASE+0x000E100ULL)
#define SPI_BASE	(MMIO_BASE+0x000E030ULL)
#define UART_BASE	(MMIO_BASE+0x000E010ULL)
#define AIC_BASE	(MMIO_BASE+0x000E200ULL)
#define PS2_BASE	(MMIO_BASE+0x000E040ULL)

#define UART_RX		(UART_BASE+0x00ULL)
#define UART_TX		(UART_BASE+0x04ULL)
#define UART_STAT	(UART_BASE+0x08ULL)
#define UART_CTRL	(UART_BASE+0x0CULL)

#define SPI_CTRL	(SPI_BASE+0x00ULL)
#define SPI_DATA	(SPI_BASE+0x04ULL)
#define SPI_QDATA	(SPI_BASE+0x08ULL)

#define PS2KB_RX	(PS2_BASE+0x00ULL)
#define PS2KB_TX	(PS2_BASE+0x04ULL)
#define PS2KB_STAT	(PS2_BASE+0x08ULL)
#define PS2KB_CTRL	(PS2_BASE+0x0CULL)
#define PS2MS_RX	(PS2_BASE+0x10ULL)
#define PS2MS_TX	(PS2_BASE+0x14ULL)
#define PS2MS_STAT	(PS2_BASE+0x18ULL)
#define PS2MS_CTRL	(PS2_BASE+0x1CULL)

#define AIC_CTRL0			(AIC_BASE+0x00ULL)
#define AIC_BRKADD			(AIC_BASE+0x04ULL)
#define AIC_ILEVELS			(AIC_BASE+0x08ULL)
#define AIC_CTRL1			(AIC_BASE+0x0CULL)
#define AIC_PIT_THROTTLE	(AIC_BASE+0x10ULL)
#define AIC_PIT_COUNTER		(AIC_BASE+0x14ULL)
#define AIC_CLK_PERIOD		(AIC_BASE+0x18ULL)
#define AIC_IGNORE0			(AIC_BASE+0x1CULL)
#define AIC_RTC_SEC_HI		(AIC_BASE+0x20ULL)
#define AIC_RTC_SEC_LO		(AIC_BASE+0x24ULL)
#define AIC_RTC_NSEC		(AIC_BASE+0x28ULL)

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

#if 0

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

#endif

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
#define TKMM_PAGEBASE	0x01400000ULL
// #define TKMM_PAGEEND	0x18000000
#define TKMM_PAGEEND	0x08000000ULL

#define TKMM_RAMBASE	0x01000000ULL

#define TKMM_EXOSTART	0x18000000ULL	//Execute Only Start

#define TKMM_VALOSTART	0x20000000ULL	//Virtual Address Low Start
#define TKMM_VALOEND	0x80000000ULL	//Virtual Address Low End

#define TKMM_VAS_START_LO		TKMM_VALOSTART
#define TKMM_VAS_END_LO			TKMM_VALOEND

#define TKMM_VAS_START_HI		0x000100000000ULL
#define TKMM_VAS_END_HI			0x7FFF00000000ULL

// #define TKMM_VAS_START_HI		0x40000000ULL
// #define TKMM_VAS_END_HI			0x80000000ULL

#if 1

#define TKMM_PAGEBITS		14
#define TKMM_PAGEMASK		16383
// #define TKMM_MAXMMLISTSZ	65536
#define TKMM_MAXMMLISTSZ	(65536-256)
#define TKMM_BRKBITS		20

#else

#if 0

#define TKMM_PAGEBITS		12
#define TKMM_PAGEMASK		4095
#define TKMM_MAXMMLISTSZ	65536
#define TKMM_BRKBITS		20

#else

#define TKMM_PAGEBITS		16
#define TKMM_PAGEMASK		65535
#define TKMM_MAXMMLISTSZ	(262144-256)
#define TKMM_BRKBITS		20

#endif

#endif

#define TKMM_MAXMMCELLBITS	10

#define TKMM_MAXMMSEGBLKBITS	(TKMM_BRKBITS-TKMM_MAXMMCELLBITS)
#define TKMM_MAXMMCELLSZ		(1<<TKMM_MAXMMCELLBITS)
#define TKMM_MAXMMSEGBLK		(1<<TKMM_MAXMMSEGBLKBITS)

#define TKMM_MCAT_DFL		0
#define TKMM_MCAT_URO		1
#define TKMM_MCAT_KRN_RW	2
#define TKMM_MCAT_USR_RW	3
#define TKMM_MCAT_USR_RWX	4

#define TKMM_MCAT_PHYSDFL	6
#define TKMM_MCAT_GLOBAL	7

#define TKMM_PROT_NONE		0x0000
#define TKMM_PROT_READ		0x0001
#define TKMM_PROT_WRITE		0x0002
#define TKMM_PROT_EXEC		0x0004

#define TKMM_PROT_RW		0x0003
#define TKMM_PROT_RWX		0x0007

#define TKMM_PROT_NOCACHE	0x0010
#define TKMM_PROT_NOUSER	0x0020

#define TKMM_PROT_USR_SXO	(TKMM_PROT_EXEC|TKMM_PROT_NOCACHE)
#define TKMM_PROT_USR_RO	(TKMM_PROT_READ|TKMM_PROT_NOCACHE)

#define TKMM_MAP_SHARED		0x0001
#define TKMM_MAP_PRIVATE	0x0002
#define TKMM_MAP_ANONYMOUS	0x0004
#define TKMM_MAP_FIXED		0x0008
#define TKMM_MAP_GROWSDOWN	0x0010
#define TKMM_MAP_NORESERVE	0x0020
#define TKMM_MAP_32BIT		0x0040
#define TKMM_MAP_DIRECT		0x0080

#define TKMM_MAP_PHYSICAL	0x0100
#define TKMM_MAP_GLOBAL		0x0200

// #define INITRD_ADDR	0x1003F010
// #define INITRD_SIZE	0x1003F014
// #define KINIT_ADDR	0x1003F100

// #define P_INITRD_ADDR	(*(vol_u32 *)INITRD_ADDR)
// #define P_INITRD_SIZE	(*(vol_u32 *)INITRD_SIZE)
// #define P_KINIT_ADDR	((char *)KINIT_ADDR)

// #define P_INITRD_ABSADDR	((void *)(0x10000000+P_INITRD_ADDR))

#define TKVF_O_CREAT	0x0002
#define TKVF_O_TRUNC	0x0040

#define TKVF_O_RDONLY	0x2000
#define TKVF_O_RDWR		0xA000
#define TKVF_O_SEARCH	0x4000
#define TKVF_O_WRONLY	0x8000


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
typedef unsigned long size_t;
#endif

#ifndef __OFF_T_DEFINED
#define __OFF_T_DEFINED
typedef long long off_t;
#endif

typedef struct TKMM_MemLnkObj_s TKMM_MemLnkObj;
typedef struct TKMM_MemLnkSeg_s TKMM_MemLnkSeg;
typedef struct TKMM_MemCelChk_s TKMM_MemCelChk;

struct TKMM_MemLnkObj_s {
u16 ttag;			//00, type tag
u16 ztag;			//02, zone tag
byte fl;			//04, object flags
byte check;			//05, check value
byte ix;			//06, size index
byte cat;			//07, category
u64		pad_resv;
// int flag;
// u32 size;
//double data[1];	//start of data
u64 data[1];	//start of data
};

struct TKMM_MemLnkSeg_s {
// byte	*blk[TKMM_MAXMMSEGBLK+4];
int		blk[TKMM_MAXMMSEGBLK+4];
int		nblk;

u64		data[1];
};

//65248, 2039, 62939
//7864, 62912+1966
//7872, 62976+1968
//7904, 63232+1976

// #define TKMM_MAXMMCELLS		4096
#define TKMM_MAXMMCELLS		7904

struct TKMM_MemCelChk_s {
u64		data[TKMM_MAXMMCELLS];
int		magic1;
u32		bmp[(TKMM_MAXMMCELLS/16)+1];
int		magic2;
int		rov;
short	freelst[(TKMM_MAXMMCELLSZ/8)+4];
int		magic3;
};


// #define TKMM_OFFS_DATA	((int)(((TKMM_MemLnkObj *)0)->data))
// #define TKMM_OFFS_DATA	((long)(((TKMM_MemLnkObj *)0)->data))

#ifdef __BGBCC__
#define TKMM_OFFS_DATA	__offsetof(TKMM_MemLnkObj, data)
#else
#define TKMM_OFFS_DATA	((long)(((TKMM_MemLnkObj *)0)->data))
#endif

typedef struct TK_FILE_VT_s TK_FILE_VT;
typedef struct TK_FILE_s TK_FILE;
typedef struct TK_FSTAT_s TK_FSTAT;
// typedef struct TK_DIR_s TK_DIR;
typedef struct TK_FILE_s TK_DIR;
typedef struct TK_MOUNT_s TK_MOUNT;
typedef struct TK_DIRENT_s TK_DIRENT;
typedef struct TK_DATETIME_s TK_DATETIME;
// typedef struct TK_DEVFSDEV_s TK_DEVFSDEV;
typedef struct TK_MOUNT_s TK_DEVFSDEV;
typedef struct TK_BLKDEVINFO_s TK_BLKDEVINFO;

typedef struct TK_USERINFO_s TK_USERINFO;


struct TK_FILE_VT_s {
char *fsname;
TK_FILE_VT *next;

TK_MOUNT *(*mount)(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);

TK_FILE *(*fopen)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path, char *mode);
TK_DIR *(*opendir)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path);
int (*unlink)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path);
int (*rename)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *oldfn, char *newfn, char *mode);
int (*fstat)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path, TK_FSTAT *st);

int (*mkdir)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path, char *mode);
int (*rmdir)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path);
int (*fsctl)(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *path, int cmd, void *ptr);

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

/* Socket/Device Ops */
int (*fsend)(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
int (*frecv)(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
};

#define TK_FILE_SZTFDA	64

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
int ipos;			//directory position
s64 ofs;			//current file offset
s64 size;			//file size
int flags;			//Access Flags
byte tfda[TK_FILE_SZTFDA];		//temp array (user data)
};

struct TK_MOUNT_s {
TK_FILE_VT *vt;
TK_MOUNT *next;
char *src;
char *tgt;
int szSrc;
int magic1;
void *udata0;
void *udata1;

byte budata[128];
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

struct TK_USERINFO_s {
u16 uid;
u16 gid;
u32 mode;
};

#if 0
struct TK_DEVFSDEV_s {
TK_FILE_VT *vt;
TK_DEVFSDEV *next;
TK_DIRENT de;
};
#endif

struct TK_BLKDEVINFO_s {
u32 lba_ofs;
u32 lba_sz;

void **buf_ptrs;		//buffer pointers
int buf_max;			//max allocated pointers.
};

typedef struct TK_VFB_RECT_s TK_VFB_RECT;
struct TK_VFB_RECT_s {
short sin_family;	//sockaddr
short sin_port;		//sockaddr
short xofs;			//X offset
short yofs;			//Y offset
short xsize;		//X size
short ysize;		//Y size
short xstr;			//X stride (pixels)
short ystr;			//Y stride (pixels)
};

typedef struct TK_BIGINT_s TK_BIGINT;
struct TK_BIGINT_s {
u64 val[6];
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
#include <tk_wadimg.h>


#ifdef __BJX2__
#define TK_GET_TBR		__arch_tbr
#define TK_SET_TBR(x)	__arch_tbr=(x)
extern volatile u64 __arch_tbr;
#else
#define TK_GET_TBR		_arch_gettbr()
#define TK_SET_TBR(x)	_arch_settbr(x)
u64 _arch_gettbr();
u64 _arch_settbr(u64 v);
#endif

void __debugbreak(void);

u32 __float32_getbits(float f);
float __float32_frombits(u32 f);
u64 __float64_getbits(double f);
double __float64_frombits(u64 f);

void *TKMM_Malloc(int sz);
int TKMM_Free(void *ptr);
void *TKMM_MallocCat(int sz, int cat);

char *TKMM_LVA_Strdup(char *str);

u16 TK_GetRandom16ASLR();
u64 TK_GetRandom48ASLR();
u64 TK_GetRandom();

s64 TK_GetTimeUs(void);


s64 TK_VMem_VaVirtualAlloc(s64 addr, s64 size, int flProt, int flMap);
int TK_VMem_VaVirtualFree(s64 vaddr, s64 size);
int TK_VMem_CheckAddrIsVirtual(s64 addr);
int TK_VMem_CheckAddrIsLowVirtual(s64 addr);
int TK_VMem_CheckAddrIsPhysPage(s64 addr);
int TK_VMem_CheckPtrIsVirtual(void *ptr);
int TK_VMem_MProtectPages(u64 addr, size_t len, int prot);
int TK_VMem_VaFreePages(s64 vaddr, int cnt);

int TK_TaskAddPageAlloc(TKPE_TaskInfo *task, void *base, int size);

int TKMM_MMList_Init(void);
void *TKMM_MMList_AllocBrk(int sz);
void *TKMM_MMList_AllocBrkCat(int sz, int cat);

TKMM_MemLnkObj *TKMM_MMList_GetPtrLnkObj(void *ptr);
int TKMM_MMCell_GetLnkObjCellSize(TKMM_MemLnkObj *obj, void *ptr);
int TKMM_FxiToSize(int ix);
int TKMM_MMList_FreeLnkObj(TKMM_MemLnkObj *obj);
int TKMM_MMList_WalkHeapObjects(
	void *ctx, int (*func)(void *ctx, TKMM_MemLnkObj *obj));

void *TKMM_MMList_Malloc(int sz);
void *TKMM_MMList_MallocCat(int sz, int cat);

u64 *TKMM_MMCell_GetLnkObjCellHeadPtr(TKMM_MemLnkObj *obj, void *ptr);
int TKMM_MMCell_FreeLnkObjCellPtr(TKMM_MemLnkObj *obj, void *ptr);

void TK_SetCurrentTask(TKPE_TaskInfo *task);
TKPE_TaskInfo *TK_GetCurrentTask();
TK_EnvContext *TK_GetCurrentEnvContext();
TKPE_TaskInfoKern *TK_GetCurrentTaskInfoKern();
int TK_TaskFreeAllPageAlloc(TKPE_TaskInfo *task);
void *TK_AllocNewTask();
int TK_SchedAddTask(TKPE_TaskInfo *newtask);

int TK_SpawnNewThread(void *func, void *uptr);
TKPE_TaskInfo *TK_SpawnNewThreadB(TKPE_TaskInfo *btask, void *func, void *uptr);
s64 TK_GetThreadStatusB(int pid);
s64 TK_GetThreadStatusA(int pid);
void TK_SuspendThreadB(int pid, s64 res);
int TK_YieldCurrentThreadB(s64 res);
void TK_YieldCurrentThreadA(s64 res);
void TK_YieldCurrentThread(void);
void *TK_WithKrrSetuidB(void *func, u64 key);

int TK_AllocNewTlsGbl();
int TK_AllocNewTlsB(TKPE_TaskInfo *task);
int TK_AllocNewTlsA(void);
s64 TK_TlsGetB(TKPE_TaskInfo *task, int key);
s64 TK_TlsSetB(TKPE_TaskInfo *task, int key, s64 val);
s64 TK_TlsGet(int key);
s64 TK_TlsSet(int key, s64 val);

int TK_TaskGetCwd(TKPE_TaskInfo *task, char *buf, int sz);
int TK_TaskSetCwd(TKPE_TaskInfo *task, char *buf);

tk_kptr TK_PboGbrGetB(TKPE_TaskInfo *task, int key);
tk_kptr TK_PboGbrSetB(TKPE_TaskInfo *task, int key, tk_kptr val);
tk_kptr TK_PboImgBaseGetB(TKPE_TaskInfo *task, int key);
tk_kptr TK_PboImgBaseSetB(TKPE_TaskInfo *task, int key, tk_kptr val);

int TK_Env_SetCwd(char *cwd);
char *TK_Env_GetCwd(char *buf, int sz);
int TK_Env_GetPathList(char ***rlst, int *rnlst);
int TK_Env_SetPath(char *cwd);
int TK_Env_GetEnvVar(char *varn, char *buf, int sz);
int TK_Env_SetEnvVar(char *varn, char *varv);

int TK_EnvCtx_GetEnvVarIdx(TK_EnvContext *ctx, int idx,
	char *bufn, char *bufv, int szn, int szv);

TK_EnvContext *TK_EnvCtx_AllocContext(void);
void TK_EnvCtx_FreeContext(TK_EnvContext *ctx);
TK_EnvContext *TK_EnvCtx_CloneContext(TK_EnvContext *ctx);
int TK_EnvCtx_SetCwd(TK_EnvContext *ctx, char *cwd);
char *TK_EnvCtx_GetCwd(TK_EnvContext *ctx, char *buf, int sz);
int TK_EnvCtx_GetPathList(TK_EnvContext *ctx, char ***rlst, int *rnlst);
int TK_EnvCtx_SplitVar(char *str, char *bvar, char **rval);
int TK_EnvCtx_SetPath(TK_EnvContext *ctx, char *path);
int TK_EnvCtx_GetEnvVar(TK_EnvContext *ctx, char *varn, char *bufv, int sz);
int TK_EnvCtx_SetEnvVar(TK_EnvContext *ctx, char *varn, char *varv);
int TK_EnvCtx_UpdateForSet(TK_EnvContext *ctx, char *estr);
int TK_EnvCtx_GetEnvListBuffer(TK_EnvContext *ctx, void *buf, int szbuf);
int TK_EnvCtx_InitForEnv(TK_EnvContext *ctx, char **envv);

void tk_puts(char *msg);
void tk_puts_n(char *msg, int n);
bool tk_iskernel();
int tk_issyscall();

void tk_sprintf(char *dst, char *str, ...);

int tk_vf_register(TK_FILE_VT *fsty);
int tk_vf_addmount(TK_MOUNT *mnt);
TK_MOUNT *tk_vf_nextmount(TK_MOUNT *mnt);
TK_MOUNT *tk_vf_firstmount(void);

int TK_FindFreeObjHandle(TKPE_TaskInfo *task);
int TK_FreeObjHandle(TKPE_TaskInfo *task, int ix);
int TK_LookupHandleForPtr(TKPE_TaskInfo *task, void *ptr);
int TK_GetHandleForPtr(TKPE_TaskInfo *task, void *ptr);
void *TK_GetPtrForHandle(TKPE_TaskInfo *task, int ix);
int TK_CheckUserAccess(TK_USERINFO *usr, TK_USERINFO *tgt);

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz);
int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);


int tk_vfile_init();
TK_FILE *tk_alloc_file();
TK_DIR *tk_alloc_dir();
void tk_free_file(TK_FILE *tmp);
void tk_free_dir(TK_DIR *tmp);
TK_MOUNT *tk_alloc_mount();
void tk_free_mount(TK_MOUNT *tmp);
int tk_fmount(
	char *devfn, char *mntfn, char *fsty,
	char *mode, char **opts);
int tk_unlink2(TK_USERINFO *usri, char *name);
int tk_unlink(char *name);
int tk_rmdir2(TK_USERINFO *usri, char *name);
int tk_rmdir(char *name);
int tk_mkdir2(TK_USERINFO *usri, char *name, char *mode);
int tk_mkdir(char *name, char *mode);
int tk_rename2(TK_USERINFO *usri, char *oldname, char *newname, char *mode);
int tk_rename(char *oldname, char *newname, char *mode);
int tk_fcopy2(TK_USERINFO *usri, char *oldname, char *newname);
int tk_fcopy(char *oldname, char *newname);
int tk_fstat2(TK_USERINFO *usri, char *name, TK_FSTAT *st);
int tk_fstat(char *name, TK_FSTAT *st);
int tk_fsctl2(TK_USERINFO *usri, char *name, int cmd, void *ptr);
int tk_fsctl(char *name, int cmd, void *ptr);
TK_FILE *tk_fopen2(TK_USERINFO *usri, char *name, char *mode);
TK_FILE *tk_fopen(char *name, char *mode);

int tk_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_ftell(TK_FILE *fd);
int tk_fclose(TK_FILE *fd);
int tk_fgetc(TK_FILE *fd);
int tk_fputc(int ch, TK_FILE *fd);
int tk_fputs(char *str, TK_FILE *fd);
int tk_fioctl(TK_FILE *fd, int cmd, void *ptr);
int tk_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
int tk_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);

int TK_InitUserInfoForTask(TKPE_TaskInfo *task, TK_USERINFO *usri);
int tk_hfmount(TKPE_TaskInfo *task,
	char *devfn, char *mntfn, char *fsty,
	char *mode, char **opts);
int tk_hfstat(TKPE_TaskInfo *task, char *name, TK_FSTAT *st);
int tk_hfsctl(TKPE_TaskInfo *task, char *name, int cmd, void *ptr);
int tk_hunlink(TKPE_TaskInfo *task, char *name);
int tk_hrmdir(TKPE_TaskInfo *task, char *name);
int tk_hmkdir(TKPE_TaskInfo *task, char *name, char *mode);
int tk_hrename(TKPE_TaskInfo *task,
	char *oldname, char *newname, char *mode);
int tk_hfopen(TKPE_TaskInfo *task, char *name, char *mode);
int tk_hread(TKPE_TaskInfo *task, int iHdl, void *pBuf, int szBuf);
int tk_hwrite(TKPE_TaskInfo *task, int iHdl, void *pBuf, int szBuf);
s64 tk_hseek(TKPE_TaskInfo *task, int iHdl, s64 lOffs, int iMod);
int tk_hclose(TKPE_TaskInfo *task, int iHdl);
s64 tk_hioctl(TKPE_TaskInfo *task, int iHdl, int iCmd, void *ptr);
s64 tk_hsend(TKPE_TaskInfo *task, int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
s64 tk_hrecv(TKPE_TaskInfo *task, int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);

TK_DIR *tk_opendir2(TK_USERINFO *usri, char *name);
TK_DIR *tk_opendir(char *name);
TK_DIRENT *tk_readdir(TK_DIR *fd);
int tk_closedir(TK_DIR *fd);
int tk_hopendir(TKPE_TaskInfo *task, char *name);
int tk_hreaddir(TKPE_TaskInfo *task, int iHdl,
	void *pDe, int szDe, int nDe);
int tk_hclosedir(TKPE_TaskInfo *task, int iHdl);

void *tk_malloc(int sz);
void *tk_malloc_cat(int sz, int cat);
int tk_free(void *ptr);
int tk_msize(void *ptr);
void *tk_realloc(void *ptr, int sz);

int tk_mdecref(void *ptr);
int tk_mgettag(void *ptr);
int tk_msettag(void *ptr, int tag);
int tk_mgetzone(void *ptr);
int tk_msetzone(void *ptr, int tag);
void *tk_mgetbase(void *ptr);
int tk_mfreezone(int ztag, int zmask);
char *tk_strdup_in(char *str);

void tk_printf(char *str, ...);
void tk_dbg_printf(char *str, ...);


void *tk_mmap(void *addr, size_t len, int prot, int flags,
	int fd, off_t offs);
int tk_munmap(void *addr, size_t len);
int tk_mprotect(void *addr, size_t len, int prot);
int tk_msync(void *addr, size_t len, int flags);
int tk_mlock(void *addr, size_t len);
int tk_munlock(void *addr, size_t len);
int tk_mlockall(int flags);
int tk_munlockall(void);




// void printf(char *str, ...);

// void *malloc(int sz);
// int free(void *ptr);

#endif

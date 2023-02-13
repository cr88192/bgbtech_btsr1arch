#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

// typedef unsigned __int128 u128;
// typedef signed __int128 s128;

typedef volatile u32 vol_u32;
typedef volatile u64 vol_u64;

typedef u64 tk_kernptr;
typedef u64 tk_kptr;

#ifndef bool
#define bool byte
#define true 1
#define false 0
#endif

#ifndef TK_APIEXPORT
#ifdef TKW_ISDLL
#define TK_APIEXPORT __declspec(dllexport)
#else
#define TK_APIEXPORT __declspec(dllimport)
#endif
#endif


#ifndef RIFF_MAKETAG
#define RIFF_MAKETAG(a, b, c, d)	((a)+((b)<<8)+((c)<<16)+((d)<<24))
#endif

#define TK_FCC_GDI	RIFF_MAKETAG('G','D','I',' ')
#define TK_FCC_TKRA	RIFF_MAKETAG('T','K','R','A')


#define TKMM_MCAT_DFL		0
#define TKMM_MCAT_URO		1
#define TKMM_MCAT_KRN_RW	2
#define TKMM_MCAT_USR_RW	3
#define TKMM_MCAT_USR_RWX	4

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

#define TKVF_O_CREAT	0x0002
#define TKVF_O_TRUNC	0x0040

#define TKVF_O_RDONLY	0x2000
#define TKVF_O_RDWR		0xA000
#define TKVF_O_SEARCH	0x4000
#define TKVF_O_WRONLY	0x8000


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

TK_APIEXPORT void tk_puts(char *msg);
TK_APIEXPORT int tk_printf(char *str, ...);

TK_APIEXPORT int tk_iskernel();

TK_APIEXPORT void *tk_malloc(size_t sz);
TK_APIEXPORT void *tk_malloc_krn(size_t sz);
TK_APIEXPORT void *tk_malloc_usr(size_t sz);
TK_APIEXPORT void tk_free(void *ptr);
TK_APIEXPORT char *tk_rstrdup(char *str);

int tk_vf_register(TK_FILE_VT *fsty);
int tk_vf_addmount(TK_MOUNT *mnt);
TK_MOUNT *tk_vf_nextmount(TK_MOUNT *mnt);
TK_MOUNT *tk_vf_firstmount(void);

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


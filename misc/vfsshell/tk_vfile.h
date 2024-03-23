#define TKVF_O_CREAT	0x0002
#define TKVF_O_TRUNC	0x0040

#define TKVF_O_RDONLY	0x2000
#define TKVF_O_RDWR		0xA000
#define TKVF_O_SEARCH	0x4000
#define TKVF_O_WRONLY	0x8000


#define TK_UMSG_VFOPEN			0x1020
#define TK_UMSG_VFOPENDIR		0x1021
#define TK_UMSG_VFUNLINK		0x1022
#define TK_UMSG_VFRENAME		0x1023
#define TK_UMSG_VFSTAT			0x1024
#define TK_UMSG_VFMOUNT			0x1025
#define TK_UMSG_VFREADDIR		0x1026
#define TK_UMSG_VFCLOSEDIR		0x1027
#define TK_UMSG_VFREAD			0x1028
#define TK_UMSG_VFWRITE			0x1029
#define TK_UMSG_VFSEEK			0x102A
#define TK_UMSG_VFCLOSE			0x102B
#define TK_UMSG_VFIOCTL			0x102C
#define TK_UMSG_VFSCTL			0x102D
#define TK_UMSG_VFMKDIR			0x102E
#define TK_UMSG_VFRMDIR			0x102F
#define TK_UMSG_VFSEND			0x1030
#define TK_UMSG_VFRECV			0x1031


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

#define	bool				_Bool
#define	true				1
#define	false				0




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

typedef struct TKPE_TaskInfo_s TKPE_TaskInfo;

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


typedef union TK_SysArg_s TK_SysArg;
union TK_SysArg_s {
s32 i;
u32 ui;
s64 l;
u64 ul;
void *p;
};

struct TKPE_TaskInfo_s {
int uid;
int gid;
};

void *tk_malloc(size_t sz);
void tk_free(void *ptr);
void *tk_realloc(void *ptr, size_t sz);

TK_FILE *tk_fopen2(TK_USERINFO *usri, char *name, char *mode);
TK_FILE *tk_fopen(char *name, char *mode);

s64 tk_fseek(TK_FILE *fd, s64 ofs, int rel);


TK_DIR *tk_opendir(char *name);
TK_DIRENT *tk_readdir(TK_DIR *fd);
int tk_closedir(TK_DIR *fd);

TK_MOUNT *tk_mount_wad4temp(char *path);

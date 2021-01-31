#ifndef __TK_SYS_STAT_H
#define __TK_SYS_STAT_H

#include <sys/types.h>

#define S_IFMT		0x00017000
#define S_IFSOCK	0x00014000
#define S_IFLNK		0x00012000
#define S_IFBLK		0x00006000
#define S_IFREG		0x00010000
#define S_IFDIR		0x00004000
#define S_IFCHR		0x00002000
#define S_IFIFO		0x00001000

#define S_ISUID		0x00000800
#define S_ISGID		0x00000400
// #define TKFAT_EMODE_STICKY		0x00000200

#define S_IRWXU		0x000001C0
#define S_IRWXG		0x00000038
#define S_IRWXO		0x00000007

#define S_IRUSR		0x00000100
#define S_IWUSR		0x00000080
#define S_IXUSR		0x00000040
#define S_IRGRP		0x00000020
#define S_IWGRP		0x00000010
#define S_IXGRP		0x00000008
#define S_IROTH		0x00000004
#define S_IWOTH		0x00000002
#define S_IXOTH		0x00000001

#define S_ISBLK(m)		(((m)&S_IFMT)==S_IFBLK)
#define S_ISCHR(m)		(((m)&S_IFMT)==S_IFCHR)
#define S_ISDIR(m)		(((m)&S_IFMT)==S_IFDIR)
#define S_ISFIFO(m)		(((m)&S_IFMT)==S_IFIFO)
#define S_ISREG(m)		(((m)&S_IFMT)==S_IFREG)
#define S_ISLNK(m)		(((m)&S_IFMT)==S_IFLNK)
#define S_ISSOCK(m)		(((m)&S_IFMT)==S_IFSOCK)


struct stat {
dev_t		st_dev;
ino_t		st_ino;
mode_t		st_mode;
nlink_t		st_nlink;
uid_t		st_uid;
gid_t		st_gid;
dev_t		st_rdev;
off_t		st_size;
time_t		st_atime;
time_t		st_mtime;
time_t		st_ctime;
blksize_t	st_blksize;
blkcnt_t	st_blocks;
};

int chmod(const char *path, mode_t mode);
int fchmod(int fd, mode_t mode);
int fstat(int fd, struct stat *st);
int lstat(const char * restrict path, struct stat * restrict st);
int mkdir(const char *path, mode_t mode);
int mkfifo(const char *path, mode_t mode);
int mknod(const char *path, mode_t mode, dev_t dev);
int stat(const char * restrict path, struct stat * restrict st);
mode_t umask(mode_t mode);

#endif

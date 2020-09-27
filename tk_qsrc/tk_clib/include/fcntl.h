#ifndef __TK_FCNTL_H
#define __TK_FCNTL_H

#include <unistd.h>

#define O_CLOEXEC	0x0001
#define O_CREAT		0x0002
#define O_DIRECTORY	0x0004
#define O_EXCL		0x0008
#define O_NOCTTY	0x0010
#define O_NOFOLLOW	0x0020
#define O_TRUNC		0x0040
#define O_TTY_INIT	0x0080
#define O_APPEND	0x0100
#define O_DSYNC		0x0200
#define O_NONBLOCK	0x0400
#define O_RSYNC		0x0800
#define O_SYNC		0x0200
#define O_ACCMODE	0xF000
#define O_EXEC		0x1000
#define O_RDONLY	0x2000
#define O_RDWR		0xA000
#define O_SEARCH	0x4000
#define O_WRONLY	0x8000

typedef int mode_t;

int creat(const char *path, mode_t mode);
int fcntl(int fd, int cmd, void *ptr=0);
int open(const char *path, int flags, mode_t mode=0);
int openat(int dirfd, const char *path, int flags, mode_t mode=0);
// int posix_fadvise(int, off_t, off_t, int);
// int posix_fallocate(int, off_t, off_t);

#endif

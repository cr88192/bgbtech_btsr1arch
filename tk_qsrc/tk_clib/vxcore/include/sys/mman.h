#ifndef __TK_SYS_MMAN_H
#define __TK_SYS_MMAN_H

#include <sys/types.h>

#define PROT_NONE		0x0000
#define PROT_READ		0x0001
#define PROT_WRITE		0x0002
#define PROT_EXEC		0x0004

// #define PROT_NOCACHE		0x0010
// #define PROT_NOUSER		0x0020

// #define PROT_USR_SXO		(PROT_EXEC|PROT_NOCACHE)
// #define PROT_USR_RO		(PROT_READ|PROT_NOCACHE)


#define MAP_SHARED		0x0001
#define MAP_PRIVATE		0x0002
#define MAP_ANONYMOUS	0x0004
#define MAP_FIXED		0x0008
#define MAP_GROWSDOWN	0x0010
#define MAP_NORESERVE	0x0020
#define MAP_32BIT		0x0040

struct posix_typed_mem_info {
size_t  posix_tmi_length;
};

int		mlock(const void *addr, size_t len);
int		mlockall(int flags);
void	*mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offs);
int		mprotect(void *addr, size_t len, int prot);
int		msync(void *addr, size_t len, int flags);
int		munlock(const void *addr, size_t len);
int		munlockall(void);
int		munmap(void *addr, size_t len);

int		shm_open(const char *name, int flag, mode_t mode);
int		shm_unlink(const char *name);

int		posix_madvise(void *, size_t, int);
int		posix_mem_offset(
			const void *restrict, size_t, off_t *restrict,
		   size_t *restrict, int *restrict);
int		posix_typed_mem_get_info(int, struct posix_typed_mem_info *);
int		posix_typed_mem_open(const char *, int, int);

#endif

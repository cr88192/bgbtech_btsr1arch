/*
Written 2009 by Brendan G Bohannon and hereby released into the public domain.
*/

// #include <sys/cdefs.h>
#include <sys/types.h>

#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

/*
Warning:
These particular values for these particular flags are unique to my
implementation, which is internally using a hybrid VirtualAlloc-like
facility.
*/

#define PROT_NONE		0x00000000	//POSIX: no access
#define PROT_READ		0x00000001	//POSIX: can read
#define PROT_WRITE		0x00000002	//POSIX: can write
#define PROT_EXEC		0x00000004	//POSIX: can execute
#define PROT_WRITECOPY		0x00000008	//VA: copy-on-write

#define MAP_FIXED		0x00000010	//POSIX: interp addr exactly
#define MAP_ANONYMOUS		0x00000020	//common: don't use file.
#define MAP_NORESERVE		0x00000040	//common: don't alloc swap space
#define MAP_STACK		0x00000080	//common: make usable as stack
#define MAP_GUARD		0x00000100	//VA: guard pages
#define MAP_NOCACHE		0x00000200	//VA: don't cache
#define MAP_WRITECOMBINE	0x00000400	//VA: combine writes
#define MAP_NOSYNC		0x00000800	//file need not by synced
#define MAP_COMMIT		0x00001000	//VA: allocate memory as well
#define MAP_RESERVE		0x00002000	//VA: reserve address space
#define MAP_DECOMMIT		0x00004000	//VA: dealloc backing memory
#define MAP_RELEASE		0x00008000	//VA: unmap region
#define MAP_FREE		0x00010000	//VQ: memory is unused
#define MAP_PRIVATE		0x00020000	//VQ: process local
#define MAP_SHARED		0x00040000	//POSIX: changes are shared
#define MAP_RESET		0x00080000	//VA: deinit range
#define MAP_TOP_DOWN		0x00100000	//VA: high-low
#define MAP_WRITE_WATCH		0x00200000	//VA: keep track of writes
#define MAP_PHYSICAL		0x00400000	//VA: allocate phys memory
						//BGB: use linear host memory

#define MAP_MAPPED		MAP_SHARED	//VQ: mapped into a section
#define MAP_32BIT		MAP_FREE	//BGB: alloc in low 2GB

#define MAP_FILE		0		//Ignored
#define MAP_ANON		MAP_ANONYMOUS	//alias


#define MAP_FAILED		((void *)(-1))


#define MS_SYNC			0x0000
#define MS_ASYNC		0x0001
#define MS_INVALIDATE		0x0002

#define MCL_CURRENT		0x0001
#define MCL_FUTURE		0x0002

struct posix_typed_mem_info;

int    mlock(const void *, size_t);
int    mlockall(int);
void  *mmap(void *, size_t, int, int, int, off_t);
int    mprotect(void *, size_t, int);
int    msync(void *, size_t, int);
int    munlock(const void *, size_t);
int    munlockall(void);
int    munmap(void *, size_t);
int    posix_madvise(void *, size_t, int);
int    posix_mem_offset(const void *, size_t, off_t *, size_t *, int *);
int    posix_typed_mem_get_info(int, struct posix_typed_mem_info *);
int    posix_typed_mem_open(const char *, int, int);
int    shm_open(const char *, int, mode_t);
int    shm_unlink(const char *);

#endif

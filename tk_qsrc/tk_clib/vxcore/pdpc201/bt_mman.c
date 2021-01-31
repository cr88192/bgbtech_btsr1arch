#include <sys/mman.h>

int		mlock(const void *addr, size_t len)
	{ return(tk_mlock(addr, len)); }
int		munlock(const void *addr, size_t len)
	{ return(tk_munlock(addr, len)); }
int		mlockall(int flags)
	{ return(tk_mlockall(flags)); }
int		munlockall(void)
	{ return(tk_munlockall()); }

void	*mmap(void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
	{ return(tk_mmap(addr, len, prot, flags, fd, offs)); }
int		munmap(void *addr, size_t len)
	{ return(tk_munmap(addr, len)); }
int		mprotect(void *addr, size_t len, int prot)
	{ return(tk_mprotect(addr, len, prot)); }
int		msync(void *addr, size_t len, int flags)
	{ return(tk_msync(addr, len, flags)); }

int		shm_open(const char *name, int flag, mode_t mode)
{
	char tb[256];
	int fd;
	while(*name=='/')
		name++;
	sprintf(tb, "/tmp/shm/%s", name);
	fd=open(tb, flag, mode);
	return(fd);
}

int		shm_unlink(const char *name)
{
	char tb[256];
	while(*name=='/')
		name++;
	sprintf(tb, "/tmp/shm/%s", name);
	unlink(tb);
	return(0);
}

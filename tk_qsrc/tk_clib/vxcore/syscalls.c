#include <vxcore.h>

void _printstr(char *s);
void *_allocmem(long sz);
void _freemem(void *p);

// int _read(int fd, void *buf, unsigned int sz);
// int _write(int fd, void *buf, unsigned int sz);
// int _open(char *name, int flag, int mode);
// void _close(int fd);
// int _lseek(int fd, int offs, int mode);
int _fork();
void _exit(int ret);
// void _unlink(char *name);

// void _unlink(int fd);
// s64 _llseek(int fd, s64 offs, int mode);

// int _eof(int fd);
// long _tell(int fd);

VXCORE_API char **_vx_argv=NULL;
VXCORE_API char **_vx_env=NULL;

// VXCORE_API void __chkstk(void)
//		{ }

VXCORE_API char **vxGetArgv()
	{ return(_vx_argv); }
VXCORE_API char **vxGetEnv()
	{ return(_vx_env); }

VXCORE_API char *GetCommandLine()
{
	static char buf[4096];
	char *s, *t;
	int i;

	if(_vx_argv)
	{
		t=buf;
		for(i=0; _vx_argv[i]; i++)
		{
			s=_vx_argv[i];
			*t++='\"';
			while(*s)*t++=*s++;
			*t++='\"';
			if(_vx_argv[i+1])
				*t++=' ';
		}
		*t++=0;
		return(buf);
	}else
	{
//		_printstr(s);
		return("dummy.exe");
	}
}

VXCORE_API void vx_printstr(char *s)
	{ _printstr(s); }

VXCORE_API void *vx_malloc(long sz)
	{ return(_allocmem(sz)); }

VXCORE_API void vx_free(void *p)
	{ _freemem(p); }

VXCORE_API int read(int fd, void *buf, unsigned int sz)
	{ return(_read(fd, buf, sz)); }

VXCORE_API int write(int fd, const void *buf, unsigned int sz)
	{ return(_write(fd, buf, sz)); }

VXCORE_API int open(const char *name, int flag, int mode)
	{ return(_open(name, flag, mode)); }

VXCORE_API int close(int fd)
	{ return(_close(fd)); }

VXCORE_API long lseek(int fd, long offs, int mode)
	{ return(_lseek(fd, offs, mode)); }

// VXCORE_API void unlink(int fd)
// 	{ _unlink(fd); }

VXCORE_API int eof(int fd)
	{ return(_eof(fd)); }
VXCORE_API long tell(int fd)
	{ return(_tell(fd)); }
VXCORE_API int unlink(const char *name)
	{ _unlink(name); }

VXCORE_API int creat(const char *name, int mode)
	{ return(open(name, O_WRONLY|O_CREAT|O_TRUNC, mode)); }

long filelength(int fd)
{
	long s, t;

	t=tell(fd);
	s=lseek(fd, 0, 2);
	lseek(fd, t, 0);
	return(s);
}

int isatty(int fd)
	{ return(0); }

int access (const char*, int);
int chsize (int, long );

int dup (int);
int dup2 (int, int);

// int setmode (int, int);
// int sopen (const char*, int, int, ...);
// int umask (int);

int _IntTab(int idx, int64_t *args, int nargs);
VXCORE_API int vxIntTab(int idx, int64_t *args, int nargs)
	{ return(_IntTab(idx, args, nargs)); }

int64_t llseek(int fd, int64_t offs, int mode)
{
	int64_t args[16];

	args[0]=fd;
	args[1]=offs;
	args[2]=mode;

	vxIntTab(VXSYSCALL_LLSEEK, args, 3);
	return(args[0]);
}

int ioctl(int fd, int req, char *argp)
{
	int64_t args[16];

	args[0]=fd;
	args[1]=req;
	args[2]=(int64_t)argp;

	vxIntTab(VXSYSCALL_IOCTL, args, 3);
	return(args[0]);
}

VXCORE_API int fork()
	{ return(_fork()); }
VXCORE_API void vx_exit(int ret)
	{ _exit(ret); }

// s64 llseek(int fd, s64 offs, int mode);


VXCORE_API void vx_remove(char *name)
	{ _unlink(name); }

VXCORE_API void vx_rename(char *old, char *new)
	{ }


VXCORE_API void vx_datetime(void *ptr)
{
}

VXCORE_API void vx_system(char *cmd)
{
}

void *_LoadLibrary(char *name);
void *_GetProcAddress(void *hdl, char *name);
void *_GetModuleHandle(char *name);

VXCORE_API int dlclose(void *hdl)
	{ return(0); }

VXCORE_API char *dlerror(void)
	{ return(NULL); }

VXCORE_API void *dlopen(const char *name, int flag)
{
	void *p;

	if(flag&RTLD_NOLOAD)
	{
		p=_GetModuleHandle(name);
		return(p);
	}
	p=_LoadLibrary(name);
	return(p);
}

VXCORE_API void *dlsym(void *hdl, const char *name)
	{ return(_GetProcAddress(hdl, name)); }

VXCORE_API void *VxLoadLibrary(char *name)
	{ return(_LoadLibrary(name)); }
VXCORE_API void *VxGetModuleHandle(const char *name)
	{ return(_GetModuleHandle(name)); }
VXCORE_API void *VxGetProcAddress(void *hdl, const char *name)
	{ return(_GetProcAddress(hdl, name)); }


//readdir interface

void *vx_opendir(char *name)
{
	int64_t args[16];
	args[0]=(int64_t)name;
	vxIntTab(VXSYSCALL_OPENDIR, args, 1);
	return((void *)(args[0]));
}

void vx_closedir(void *hdl)
{
	int64_t args[16];
	args[0]=(int64_t)hdl;
	vxIntTab(VXSYSCALL_CLOSEDIR, args, 1);
}

char *vx_readdir(void *hdl)
{
	int64_t args[16];
	args[0]=(int64_t)hdl;
	vxIntTab(VXSYSCALL_READDIR, args, 1);
	return((char *)(args[0]));
}

void vx_rewinddir(void *hdl)
{
	int64_t args[16];
	args[0]=(int64_t)hdl;
	vxIntTab(VXSYSCALL_REWINDDIR, args, 1);
}

VXCORE_API DIR* __cdecl opendir(const char *name)
{
	DIR *tmp;
	void *p;

	p=vx_opendir(name);
	if(!p)return(NULL);

	tmp=vx_malloc(sizeof(DIR)+strlen(name)+1);
	strcpy(tmp->dd_name, name);
	tmp->dd_handle=(long)p;
	tmp->dd_stat=0;
	return(tmp);
}

VXCORE_API struct dirent* __cdecl readdir(DIR *dir)
{
	struct dirent *de;
	char *s;

	de=&(dir->dd_dir);
	s=vx_readdir((void *)dir->dd_handle);
	if(!s)
	{
		dir->dd_stat=-1;
		return(NULL);
	}

	dir->dd_stat++;
	strcpy(de->d_name, s);
	return(de);
}

VXCORE_API int __cdecl closedir(DIR *dir)
{
	vx_closedir((void *)dir->dd_handle);
	vx_free(dir);
}

VXCORE_API void __cdecl rewinddir(DIR *dir)
{
	vx_rewinddir((void *)dir->dd_handle);
	dir->dd_stat=0;
}

VXCORE_API long __cdecl telldir(DIR *dir)
	{ return(dir->dd_stat); }

VXCORE_API void __cdecl seekdir(DIR *dir, long loc)
{
	int i;

	i=loc;
	rewinddir(dir);
	while(i>0) { readdir(dir); i--; }
}

VXCORE_API int mlock(const void *addr, size_t len)
	{ return(-1); }
VXCORE_API int mlockall(int flag)
	{ return(-1); }
VXCORE_API int munlock(const void *addr, size_t len)
	{ return(-1); }
VXCORE_API int munlockall(void)
	{ return(-1); }

VXCORE_API void *mmap(void *addr, size_t len, int prot, int flags, int filedes, off_t off)
{
	int64_t args[16];

	args[0]=(int64_t)addr;
	args[1]=(int64_t)len;
	args[2]=(int64_t)prot;
	args[3]=(int64_t)flags;
	args[4]=(int64_t)filedes;
	args[5]=(int64_t)off;

	vxIntTab(VXSYSCALL_MMAP, args, 6);
	return((void *)(args[0]));
}

VXCORE_API int mprotect(void *addr, size_t len, int prot)
{
	int64_t args[16];

	args[0]=(int64_t)addr;
	args[1]=(int64_t)len;
	args[2]=(int64_t)prot;
	vxIntTab(VXSYSCALL_MPROTECT, args, 3);
	return(args[0]);
}

VXCORE_API int msync(void *addr, size_t len, int flags)
{
	int64_t args[16];

	args[0]=(int64_t)addr;
	args[1]=(int64_t)len;
	args[2]=(int64_t)flags;
	vxIntTab(VXSYSCALL_MSYNC, args, 3);
	return(args[0]);
}

VXCORE_API int munmap(void *addr, size_t len)
{
	int64_t args[16];

	args[0]=(int64_t)addr;
	args[1]=(int64_t)len;
	vxIntTab(VXSYSCALL_MUNMAP, args, 2);
	return(args[0]);
}

VXCORE_API int vx_timens(int64_t *sec, int32_t *nsec)
{
	int64_t args[16];

	args[0]=(int64_t)sec;
	args[1]=(int64_t)nsec;
	vxIntTab(VXSYSCALL_TIMENS, args, 2);
	return(args[0]);
}

VXCORE_API int vx_nsleep(int64_t val)
{
	int64_t args[16];

	args[0]=(int64_t)val;
	vxIntTab(VXSYSCALL_NSLEEP, args, 1);
	return(args[0]);
}

VXCORE_API int VxStubFault(const char *file, int line, const char *func)
{
	vx_printstr("VxStubFault: ");
	vx_printstr(func);
	vx_printstr("\n");
	vx_exit(-1);
}

VXCORE_API int VxStubWarning(const char *file, int line, const char *func)
{
	vx_printstr("VxStubWarning: ");
	vx_printstr(func);
	vx_printstr("\n");
//	vx_exit(-1);
}

VXCORE_API int VxGetCurrentPID(void)
	{ return(0); }

//VXCORE_API int usleep(useconds_t useconds)
VXCORE_API int usleep(int64_t val)
{
	vx_nsleep(val*1000LL);
	return(0);
}

VXCORE_API int shm_open(const char *path, int flag, mode_t mode)
	{ return(open(path, flag|O_SHM, mode)); }

VXCORE_API int shm_unlink(const char *path)
{
	vx_remove(path);
	return(0);
}

size_t vx_recvfrom(int fd, void *msg, size_t msglen, int flags,
	struct sockaddr *addr, size_t *szaddr)
{
	int64_t args[16];

	args[0]=(int64_t)fd;
	args[1]=(int64_t)msg;
	args[2]=(int64_t)msglen;
	args[3]=(int64_t)flags;
	args[4]=(int64_t)addr;
	args[5]=(int64_t)szaddr;
	vxIntTab(VXSYSCALL_RECVFROM, args, 6);
	return(args[0]);
}

size_t vx_sendto(int sock, const void *msg, size_t msglen, int flags,
	const struct sockaddr *dest, size_t szdest)
{
	int64_t args[16];

	args[0]=(int64_t)sock;
	args[1]=(int64_t)msg;
	args[2]=(int64_t)msglen;
	args[3]=(int64_t)flags;
	args[4]=(int64_t)dest;
	args[5]=(int64_t)szdest;
	vxIntTab(VXSYSCALL_SENDTO, args, 6);
	return(args[0]);
}

int vx_bind(int sock, const struct sockaddr *addr, size_t addrlen)
{
	int64_t args[16];

	args[0]=(int64_t)sock;
	args[1]=(int64_t)addr;
	args[2]=(int64_t)addrlen;
	vxIntTab(VXSYSCALL_BIND, args, 3);
	return(args[0]);
}

int vx_socket(int domain, int type, int protocol)
{
	int64_t args[16];

	args[0]=(int64_t)domain;
	args[1]=(int64_t)type;
	args[2]=(int64_t)protocol;
	vxIntTab(VXSYSCALL_SOCKET, args, 3);
	return(args[0]);
}

int vx_connect(int fd, const struct sockaddr *addr, size_t addrlen)
{
	int64_t args[16];

	args[0]=(int64_t)fd;
	args[1]=(int64_t)addr;
	args[2]=(int64_t)addrlen;
	vxIntTab(VXSYSCALL_CONNECT, args, 3);
	return(args[0]);
}

int vx_accept(int fd, struct sockaddr *addr, size_t *addrlen)
{
	int64_t args[16];

	args[0]=(int64_t)fd;
	args[1]=(int64_t)addr;
	args[2]=(int64_t)addrlen;
	vxIntTab(VXSYSCALL_ACCEPT, args, 3);
	return(args[0]);
}

int vx_listen(int fd, int backlog)
{
	int64_t args[16];

	args[0]=(int64_t)fd;
	args[1]=(int64_t)backlog;
	vxIntTab(VXSYSCALL_LISTEN, args, 3);
	return(args[0]);
}

VXCORE_API int accept(int sock, struct sockaddr *addr, socklen_t *szaddr)
	{ return(vx_accept(sock, addr, szaddr)); }
VXCORE_API int bind(int sock, const struct sockaddr *addr, socklen_t szaddr)
	{ return(vx_bind(sock, addr, szaddr)); }
VXCORE_API int connect(int sock, const struct sockaddr *addr, socklen_t szaddr)
	{ return(vx_connect(sock, addr, szaddr)); }
VXCORE_API int listen(int sock, int backlog)
	{ return(vx_listen(sock, backlog)); }
VXCORE_API ssize_t recv(int sock, void *msg, size_t szmsg, int flags)
	{ return(vx_recvfrom(sock, msg, szmsg, flags, NULL, NULL)); }
VXCORE_API ssize_t recvfrom(int sock, void *msg, size_t szmsg, int flags,
		struct sockaddr *addr, socklen_t *szaddr)
	{ return(vx_recvfrom(sock, msg, szmsg, flags, addr, szaddr)); }
VXCORE_API ssize_t send(int sock, const void *msg, size_t szmsg, int flag)
	{ return(vx_sendto(sock, msg, szmsg, flag, NULL, 0)); }
VXCORE_API ssize_t sendto(int sock, const void *msg, size_t szmsg, int flags,
		const struct sockaddr *addr, socklen_t szaddr)
	{ return(vx_sendto(sock, msg, szmsg, flags, addr, szaddr)); }
VXCORE_API int socket(int dom, int type, int proto)
	{ return(vx_socket(dom, type, proto)); }

ssize_t recvmsg(int, struct msghdr *, int);
ssize_t sendmsg(int, const struct msghdr *, int);

int getpeername(int, struct sockaddr *, socklen_t *);
int getsockname(int, struct sockaddr *, socklen_t *);
int getsockopt(int, int, int, void *, socklen_t *);

int setsockopt(int, int, int, const void *, socklen_t);
int shutdown(int, int);

int sockatmark(int);
int socketpair(int, int, int, int[2]);

VXCORE_API void *vxNativeSwizBuf(void *buf, long sz)
{
	//FIX: do something
	return(buf);
}

VXCORE_API void *vxNativeSwizData(void *buf, char *sig)
{
	//FIX: do something
	return(buf);
}

VXCORE_API void *vxNativeSwizBufRd(void *buf, long sz)
{
	//FIX: do something
	return(buf);
}

VXCORE_API void *vxNativeSwizDataRd(void *buf, char *sig)
{
	//FIX: do something
	return(buf);
}

VXCORE_API void vxNativeUnSwiz(void *hdl)
{
}

VXCORE_API void vxNativeDropSwiz(void *hdl)
{
}


void *_NativeCall(char *name, void *args);
void *_NativeCall2(char *name, char *sig, void *args);

VXCORE_API void *vxNativeCall(char *name, char *sig, void *args)
{
	void *p;
	if(!sig)p=_NativeCall(name, args);
	else	p=_NativeCall2(name, sig, args);
	if(!p)errno=ENOSYS;
	return(p);
}

VXCORE_API int vxNativeCalli(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(-1);
 return(*(int *)p);
}

VXCORE_API long long vxNativeCalll(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(-1);
 return(*(long long *)p);
}

VXCORE_API float vxNativeCallf(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(-1);
 return(*(float *)p);
}

VXCORE_API double vxNativeCalld(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(-1);
 return(*(double *)p);
}

VXCORE_API void *vxNativeCallp(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(NULL);
 return(*(void **)p);
}

VXCORE_API void *vxNativeCallr(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
 if(!p) return(NULL);
 return(*(void **)p);
}

VXCORE_API void vxNativeCallv(char *name, char *sig, ...)
{
 void *p; p=(void *)((&sig)+1);
 p=vxNativeCall(name, sig, p);
// if(!p) return(-1);
}

char *_MetaGetKey(char *var);
int _MetaSetKey(char *var, char *val);
VXCORE_API char *vxMetaGetKey(char *var)
	{ return(_MetaGetKey(var)); }
VXCORE_API int vxMetaSetKey(char *var, char *val)
	{ return(_MetaSetKey(var, val)); }

VXCORE_API int DevIoCtrl(
	int hDevice, int dwIoControlCode,
	void *lpInBuffer, int nInBufferSize,
	void *lpOutBuffer, int nOutBufferSize,
	int *lpBytesReturned, void *lpOverlapped)
{
	int64_t args[16];

	args[0]=hDevice;
	args[1]=dwIoControlCode;
	args[2]=(int64_t)lpInBuffer;
	args[3]=nInBufferSize;
	args[4]=(int64_t)lpOutBuffer;
	args[5]=nOutBufferSize;
	args[6]=(int64_t)lpBytesReturned;
	args[7]=(int64_t)lpOverlapped;

	vxIntTab(VXSYSCALL_DEVIOCTRL, args, 8);
	return(args[0]);
}

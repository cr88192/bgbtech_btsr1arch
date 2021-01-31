byte *tkmm_mmap_bufs[512];
byte *tkmm_mmap_bufe[512];
int tkmm_mmap_prot[512];
int tkmm_mmap_flag[512];
int tkmm_mmap_n_map=0;

int tk_mlock2(TKPE_TaskInfo *task, void *addr, size_t len)
{
}

int tk_munlock2(TKPE_TaskInfo *task, void *addr, size_t len)
{
}

int tk_mlockall2(TKPE_TaskInfo *task, int flags)
{
}

int tk_munlockall2(TKPE_TaskInfo *task)
{
}

int TK_MMap_AllocIndex(void)
{
	int i;
	
	for(i=0; i<tkmm_mmap_n_map; i++)
		if(!tkmm_mmap_bufs[i])
			return(i);
	i=tkmm_mmap_n_map++;
	return(i);
}

int TK_MMap_VaPageFree(void *ptr, int len)
{
	s64 addr;
	int cnt;
	
	addr=(s64)ptr;
	cnt=len>>TKMM_PAGEBITS;
	if(TK_VMem_CheckAddrIsVirtual(addr))
	{
		TK_VMem_VaFreePages(addr, cnt);
		return(0);
	}else
	{
		TKMM_PageFree(ptr, len);
		return(0);
	}
}

void *tk_mmap2(TKPE_TaskInfo *task,
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
	byte *ptr;
	int ix;

	if(flags&TKMM_MAP_ANONYMOUS)
	{
		if(addr)
		{
		}
	
		ptr=TKMM_PageAlloc(len);
		
//		ix=tkmm_mmap_n_map++;
		ix=TK_MMap_AllocIndex();
		tkmm_mmap_bufs[ix]=ptr;
		tkmm_mmap_bufe[ix]=ptr+len;
		tkmm_mmap_prot[ix]=prot;
		tkmm_mmap_flag[ix]=flags;
		
		return(ptr);
	}
}

int tk_munmap2(TKPE_TaskInfo *task, void *addr, size_t len)
{
	byte *ptrs, *ptre, *bufs, *bufe;
	size_t len1;
	int i, j, k;
	
	ptrs=addr; ptre=ptrs+len;
	for(i=0; i<tkmm_mmap_n_map; i++)
	{
		/* Simple Case, 1:1 mapping. */
		if((ptrs==tkmm_mmap_bufs[i]) && (ptre==tkmm_mmap_bufe[i]))
		{
			TKMM_PageFree(ptrs, len);
			tkmm_mmap_bufs[i]=NULL;
			tkmm_mmap_bufe[i]=NULL;
			return(1);
		}

		if((ptrs>tkmm_mmap_bufs[i]) && (ptre<tkmm_mmap_bufe[i]))
		{
			/* Hacks part out of the middle. */
			j=TK_MMap_AllocIndex();
			TKMM_PageFree(ptrs, len);
			tkmm_mmap_bufs[i]=bufs;
			tkmm_mmap_bufe[i]=ptrs;
			tkmm_mmap_bufs[j]=ptre;
			tkmm_mmap_bufe[j]=bufe;
			return(1);
		}
	}

	for(i=0; i<tkmm_mmap_n_map; i++)
	{
		bufs=tkmm_mmap_bufs[i];
		bufe=tkmm_mmap_bufe[i];
		len1=bufe-bufs;

		if((ptrs<=bufs) && (ptre>=bufe))
		{
			/* Removes whole mapping */
			TK_MMap_VaPageFree(bufs, len1);
			tkmm_mmap_bufs[i]=NULL;
			tkmm_mmap_bufe[i]=NULL;
		}

		if((ptrs<=bufs) && (ptre<=bufe))
		{
			/* Removes start of mapping. */
			len1=ptre-bufs;
			TK_MMap_VaPageFree(bufs, len1);
			tkmm_mmap_bufs[i]=ptre;
			tkmm_mmap_bufe[i]=bufe;
		}

		if((ptrs>bufs) && (ptre>=bufe))
		{
			/* Removes end of mapping. */
			len1=bufe-ptrs;
			TK_MMap_VaPageFree(ptrs, len1);
			tkmm_mmap_bufs[i]=bufs;
			tkmm_mmap_bufe[i]=ptrs;
		}
	}
	
	return(-1);
}

int tk_mprotect2(TKPE_TaskInfo *task, void *addr, size_t len, int prot)
{
}

int tk_msync2(TKPE_TaskInfo *task, void *addr, size_t len, int flags)
{
}


void *TKMM_MmapV(
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
	TK_SysArg ar[8];
	void *p;
	
//	tk_printf("TKMM_MmapV: sz=%d\n", sz);
	
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	ar[2].i=prot;
	ar[3].i=flags;
	ar[4].i=fd;
	ar[5].l=offs;
	tk_syscall(NULL, TK_UMSG_MMAP, &p, ar);

//	tk_printf("TKMM_MmapV: Vp=%p, p=%p\n", &p, p);

	return(p);
}

void *TKMM_MunmapV(
	void *addr, size_t len)
{
	TK_SysArg ar[8];
	void *p;
	
//	tk_printf("TKMM_MmapV: sz=%d\n", sz);
	
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	tk_syscall(NULL, TK_UMSG_MUNMAP, &p, ar);

//	tk_printf("TKMM_MmapV: Vp=%p, p=%p\n", &p, p);

	return(p);
}

void *TKMM_MProtectV(
	void *addr, size_t len, int prot)
{
	TK_SysArg ar[8];
	void *p;
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	ar[2].i=prot;
	tk_syscall(NULL, TK_UMSG_MPROTECT, &p, ar);
	return(p);
}

void *TKMM_MSyncV(
	void *addr, size_t len, int prot)
{
	TK_SysArg ar[8];
	void *p;
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	ar[2].i=prot;
	tk_syscall(NULL, TK_UMSG_MSYNC, &p, ar);
	return(p);
}


void *TKMM_MmapL(
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
	return(tk_mmap2(NULL, addr, len, prot, flags, fd, offs));
}

void *TKMM_MunmapL(
	void *addr, size_t len)
{
	return(tk_munmap2(NULL, addr, len));
}

void *TKMM_MProtectL(
	void *addr, size_t len, int prot)
{
	return(tk_mprotect2(NULL, addr, len, prot));
}

void *TKMM_MSyncL(
	void *addr, size_t len, int flag)
{
	return(tk_msync2(NULL, addr, len, flag));
}

void *(*TKMM_MmapF)(
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs);
int (*TKMM_MunmapF)(void *addr, size_t len);
int (*TKMM_MProtectF)(void *addr, size_t len, int prot);
int (*TKMM_MSyncF)(void *addr, size_t len, int flag);


void TKMM_InitMMap()
{
	int i;

#ifndef __TK_CLIB_ONLY__
	if(!tk_iskernel())
	{
		TKMM_MmapF		=TKMM_MmapV;
		TKMM_MunmapF	=TKMM_MunmapV;
		TKMM_MProtectF	=TKMM_MProtectV;
		TKMM_MSyncF		=TKMM_MSyncV;
	}else
	{
		TKMM_MmapF		=TKMM_MmapL;
		TKMM_MunmapF	=TKMM_MunmapL;
		TKMM_MProtectF	=TKMM_MProtectL;
		TKMM_MSyncF		=TKMM_MSyncL;
	}
#else
	TKMM_MmapF		=TKMM_MmapV;
	TKMM_MunmapF	=TKMM_MunmapV;
	TKMM_MProtectF	=TKMM_MProtectV;
	TKMM_MSyncF		=TKMM_MSyncV;
#endif
}


void *tk_mmap(void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
	return(TKMM_MmapF(addr, len, prot, flags, fd, offs));
}


int tk_munmap(void *addr, size_t len)
{
	return(TKMM_MunmapF(addr, len));
}

int tk_mprotect(void *addr, size_t len, int prot)
{
	return(TKMM_MProtectF(addr, len, prot));
}

int tk_msync(void *addr, size_t len, int flags)
{
	return(TKMM_MSyncF(addr, len, flags));
}


int tk_mlock(void *addr, size_t len)
{
}

int tk_munlock(void *addr, size_t len)
{
}

int tk_mlockall(int flags)
{
}

int tk_munlockall(void)
{
}


#if 0
int tk_shm_open(const char *name, int flag, mode_t mode)
{
}

int tk_shm_unlink(const char *name)
{
}
#endif

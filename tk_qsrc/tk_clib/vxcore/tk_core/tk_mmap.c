#ifndef __TK_CLIB_ONLY__

// byte *tkmm_mmap_bufs[512];
// byte *tkmm_mmap_bufe[512];
// int tkmm_mmap_prot[512];
// int tkmm_mmap_flag[512];
// int tkmm_mmap_n_map=0;

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
	TKPE_TaskInfoKern *krnl;
	int i;
	
	krnl=TK_GetCurrentTaskInfoKern();
	
//	for(i=0; i<tkmm_mmap_n_map; i++)
//		if(!tkmm_mmap_bufs[i])
//			return(i);
//	i=tkmm_mmap_n_map++;

	for(i=0; i<krnl->mmap_n_map; i++)
		if(!krnl->mmap_ptr[i])
			return(i);
	i=krnl->mmap_n_map++;

	if(i>=512)
	{
		__debugbreak();
	}

	return(i);
}

int TK_MMap_VaPageFree(void *ptr, int len)
{
	s64 addr;
	int cnt;
	
	addr=(s64)ptr;
	cnt=len>>TKMM_PAGEBITS;
	if(	TK_VMem_CheckAddrIsVirtual(addr) ||
		TK_VMem_CheckAddrIsLowVirtual(addr))
	{
		TK_VMem_VaFreePages(addr, cnt);
		return(0);
	}else if(TK_VMem_CheckAddrIsPhysPage(addr))
	{
		TKMM_PageFree(ptr, len);
		return(0);
	}
}

void *tk_mmap2(TKPE_TaskInfo *task,
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
//	TKPE_TaskInfo *task;
	TKPE_TaskInfoKern *krnl;
	byte *ptr;
	int ix;

	if(!task)
	{
		if(!TK_GetCurrentTask())
			return(TKMM_PageAlloc(len));
		__debugbreak();
	}
	
	len=(len+((1<<TKMM_PAGEBITS)-1))&(~((1<<TKMM_PAGEBITS)-1));

	/*
	 * The mmap/mporotect interface will disallow NOCACHE and NOUSER.
	 * These flags will be considered as supervisor-only features.
	 */
	prot&=~(TKMM_PROT_NOCACHE|TKMM_PROT_NOUSER);

//	if(!task)
//		task=TK_GetCurrentTask();
	krnl=TK_GetTaskInfoKern(task);
//	krnl=TK_GetCurrentTaskInfoKern();
	
//	if(flags&TKMM_MAP_ANONYMOUS)
	if((flags&TKMM_MAP_ANONYMOUS) || (fd<0))
	{
		if(addr)
		{
			return(NULL);
		}
	
//		ptr=TKMM_PageAlloc(len);
//		ptr=TKMM_PageAllocUsc(len);
		ptr=(byte *)TK_VMem_VaVirtualAlloc((u64)addr, len, prot, flags);

		if(!ptr)
		{
			ptr=TKMM_PageAlloc(len);
		}

		TK_VMem_MProtectPages((u64)ptr, len, prot);
	
		TK_TaskAddPageAlloc(task, ptr, len);

//		ix=tkmm_mmap_n_map++;
		ix=TK_MMap_AllocIndex();
//		tkmm_mmap_bufs[ix]=ptr;
//		tkmm_mmap_bufe[ix]=ptr+len;
//		tkmm_mmap_prot[ix]=prot;
//		tkmm_mmap_flag[ix]=flags;

		krnl->mmap_ptr[ix]=(tk_kptr)ptr;
		krnl->mmap_len[ix]=len;
		krnl->mmap_prot[ix]=prot;
		krnl->mmap_flag[ix]=flags;
		
		return(ptr);
	}

	return(NULL);
}

int tk_munmap2(TKPE_TaskInfo *task, void *addr, size_t len)
{
	byte *ptrs, *ptre, *bufs, *bufe;
	TKPE_TaskInfoKern *krnl;
	size_t len1;
	int i, j, k;
	
	addr=(void *)(((long)addr)&0x0000FFFFFFFFFFFFULL);
	len=(int)len;

	len=(len+((1<<TKMM_PAGEBITS)-1))&(~((1<<TKMM_PAGEBITS)-1));

	tk_dbg_printf("tk_munmap2: %p %p\n", addr, len);
	
//	if(!task)
//		task=TK_GetCurrentTask();
	krnl=TK_GetTaskInfoKern(task);
//	krnl=TK_GetCurrentTaskInfoKern();
	
	ptrs=addr; ptre=ptrs+len;
//	for(i=0; i<tkmm_mmap_n_map; i++)
	for(i=0; i<krnl->mmap_n_map; i++)
	{
		bufs=(byte *)(krnl->mmap_ptr[i]);
		bufe=bufs+krnl->mmap_len[i];

		/* Simple Case, 1:1 mapping. */
//		if((ptrs==tkmm_mmap_bufs[i]) && (ptre==tkmm_mmap_bufe[i]))
		if((ptrs==bufs) && (ptre==bufe))
		{
//			TKMM_PageFree(ptrs, len);
			TK_MMap_VaPageFree(ptrs, len);
			krnl->mmap_ptr[i]=0;
			krnl->mmap_len[i]=0;
			
//			tkmm_mmap_bufs[i]=NULL;
//			tkmm_mmap_bufe[i]=NULL;
			return(1);
		}

//		if((ptrs>tkmm_mmap_bufs[i]) && (ptre<tkmm_mmap_bufe[i]))
		if((ptrs>bufs) && (ptre<bufe))
		{
			/* Hacks part out of the middle. */
			j=TK_MMap_AllocIndex();
//			TKMM_PageFree(ptrs, len);
			TK_MMap_VaPageFree(ptrs, len);
//			tkmm_mmap_bufs[i]=bufs;
//			tkmm_mmap_bufe[i]=ptrs;
//			tkmm_mmap_bufs[j]=ptre;
//			tkmm_mmap_bufe[j]=bufe;

			krnl->mmap_ptr[i]=(tk_kptr)bufs;
			krnl->mmap_len[i]=ptrs-bufs;
			krnl->mmap_ptr[j]=(tk_kptr)ptre;
			krnl->mmap_len[j]=bufe-ptre;

			return(1);
		}
	}

//	for(i=0; i<tkmm_mmap_n_map; i++)
	for(i=0; i<krnl->mmap_n_map; i++)
	{
		bufs=(byte *)(krnl->mmap_ptr[i]);
		bufe=bufs+krnl->mmap_len[i];
//		bufs=tkmm_mmap_bufs[i];
//		bufe=tkmm_mmap_bufe[i];
		len1=bufe-bufs;

		tk_dbg_printf("tk_munmap2: A:(%p %p) %p R:(%p %p)\n",
			ptrs, ptre, len1,
			bufs, bufe);

		if((ptrs<=bufs) && (ptre>=bufe))
		{
			/* Removes whole mapping */
			TK_MMap_VaPageFree(bufs, len1);
			krnl->mmap_ptr[i]=0;
			krnl->mmap_len[i]=0;
//			tkmm_mmap_bufs[i]=NULL;
//			tkmm_mmap_bufe[i]=NULL;
		}

		if((ptrs<=bufs) && (ptre<=bufe) && (ptre>bufs))
		{
			/* Removes start of mapping. */
			len1=ptre-bufs;
			TK_MMap_VaPageFree(bufs, len1);
//			tkmm_mmap_bufs[i]=ptre;
//			tkmm_mmap_bufe[i]=bufe;
			krnl->mmap_ptr[i]=(tk_kptr)ptre;
			krnl->mmap_len[i]=bufe-ptre;
		}

		if((ptrs>bufs) && (ptre>=bufe) && (ptrs<bufe))
		{
			/* Removes end of mapping. */
			len1=bufe-ptrs;
			TK_MMap_VaPageFree(ptrs, len1);
//			tkmm_mmap_bufs[i]=bufs;
//			tkmm_mmap_bufe[i]=ptrs;
			krnl->mmap_ptr[i]=(tk_kptr)bufs;
			krnl->mmap_len[i]=ptrs-bufs;
		}
	}
	
	return(-1);
}

int tk_mprotect2(TKPE_TaskInfo *task, void *addr, size_t len, int prot)
{
	prot&=~(TKMM_PROT_NOCACHE|TKMM_PROT_NOUSER);

	/* FIXME: This only works with a single address space. */
	TK_VMem_MProtectPages((u64)addr, len, prot);
}

int tk_msync2(TKPE_TaskInfo *task, void *addr, size_t len, int flags)
{
}

#endif


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

int TKMM_MunmapV(
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

	return((long)p);
}

int TKMM_MProtectV(
	void *addr, size_t len, int prot)
{
	TK_SysArg ar[8];
	void *p;
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	ar[2].i=prot;
	tk_syscall(NULL, TK_UMSG_MPROTECT, &p, ar);
	return((long)p);
}

int TKMM_MSyncV(
	void *addr, size_t len, int prot)
{
	TK_SysArg ar[8];
	void *p;
	p=NULL;
	ar[0].p=addr;
	ar[1].l=len;
	ar[2].i=prot;
	tk_syscall(NULL, TK_UMSG_MSYNC, &p, ar);
	return((long)p);
}


#ifndef __TK_CLIB_ONLY__
void *TKMM_MmapL(
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs)
{
//	prot&=~(TKMM_PROT_NOCACHE|TKMM_PROT_NOUSER);
	return(tk_mmap2(TK_GetCurrentTask(), addr, len, prot, flags, fd, offs));
}

void *TKMM_MunmapL(
	void *addr, size_t len)
{
	return((void *)tk_munmap2(TK_GetCurrentTask(), addr, len));
}

void *TKMM_MProtectL(
	void *addr, size_t len, int prot)
{
//	prot&=~(TKMM_PROT_NOCACHE|TKMM_PROT_NOUSER);
	return((void *)tk_mprotect2(TK_GetCurrentTask(), addr, len, prot));
}

void *TKMM_MSyncL(
	void *addr, size_t len, int flag)
{
	return((void *)tk_msync2(TK_GetCurrentTask(), addr, len, flag));
}
#endif

void *(*TKMM_MmapF)(
	void *addr, size_t len, int prot, int flags,
	int fd, off_t offs);
int (*TKMM_MunmapF)(void *addr, size_t len);
int (*TKMM_MProtectF)(void *addr, size_t len, int prot);
int (*TKMM_MSyncF)(void *addr, size_t len, int flag);


void TKMM_InitMMap()
{
	int i;

	if(TKMM_MmapF)
		return;

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
	TKMM_InitMMap();
//	return(TKMM_MmapF(addr, len, prot, flags, fd, offs));

#ifndef __TK_CLIB_ONLY__
	if(tk_iskerneltask() || tk_issyscall())
		{ return(TKMM_MmapL(addr, len, prot, flags, fd, offs)); }
	return(TKMM_MmapV(addr, len, prot, flags, fd, offs));
#else
	return(TKMM_MmapV(addr, len, prot, flags, fd, offs));
#endif
}


int tk_munmap(void *addr, size_t len)
{
	TKMM_InitMMap();
//	return(TKMM_MunmapF(addr, len));

#ifndef __TK_CLIB_ONLY__
	if(tk_iskerneltask() || tk_issyscall())
		{ return(TKMM_MunmapL(addr, len)); }
	return(TKMM_MunmapV(addr, len));
#else
	return(TKMM_MunmapV(addr, len));
#endif
}

int tk_mprotect(void *addr, size_t len, int prot)
{
	TKMM_InitMMap();
//	return(TKMM_MProtectF(addr, len, prot));

#ifndef __TK_CLIB_ONLY__
	if(tk_iskerneltask() || tk_issyscall())
		{ return(TKMM_MProtectL(addr, len, prot)); }
	return(TKMM_MProtectV(addr, len, prot));
#else
	return(TKMM_MProtectV(addr, len, prot));
#endif
}

int tk_msync(void *addr, size_t len, int flags)
{
	TKMM_InitMMap();
//	return(TKMM_MSyncF(addr, len, flags));

#ifndef __TK_CLIB_ONLY__
	if(tk_iskerneltask() || tk_issyscall())
		{ return(TKMM_MSyncL(addr, len, flags)); }
	return(TKMM_MSyncV(addr, len, flags));
#else
	return(TKMM_MSyncV(addr, len, flags));
#endif
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

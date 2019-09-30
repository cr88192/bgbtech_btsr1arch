byte *tk_ird_imgbuf=NULL;

u32 tkmm_pagebase, tkmm_pageend;
//static int tkmm_initvar=0;

byte tkmm_pagebmp[4096];
int tkmm_maxpage;
int tkmm_pagerov;

void *TKMM_MMList_Malloc(int sz);

int TKMM_FindFreePages(int n)
{
	int i0, i1;
	int i, j, m;
	
	if(n<=0)
		return(-1);
	
	i=tkmm_pagerov;
	m=tkmm_maxpage;
	while(i<m)
	{
		if(tkmm_pagebmp[i>>3]&(1<<(i&7)))
		{
			while((tkmm_pagebmp[i>>3]==0xFF) && (i<m))
			{
				i=(i+8)&(~7);
				continue;
			}
			while((tkmm_pagebmp[i>>3]&(1<<(i&7))) && (i<m))
				i++;
			continue;
		}
		i0=i; i1=i0+n;
		if(i1>m)break;
		while(!(tkmm_pagebmp[i>>3]&(1<<(i&7))) && (i<i1))
			i++;
		if(i>=i1)
		{
			tkmm_pagerov=i1;
			return(i0);
		}
	}
	
	if(tkmm_pagerov!=0)
	{
		tkmm_pagerov=0;
		i=TKMM_FindFreePages(n);
		return(i);
	}

	return(-1);
}

int TKMM_AllocPages(int n)
{
	int i, j, k;
	
	i=TKMM_FindFreePages(n);
	if(i<0)return(-1);
	
	j=i; k=j+n;
	while(j<k)
	{
		tkmm_pagebmp[j>>3]|=(1<<(j&7));
		j++;
	}
	return(i);
}

int TKMM_FreePages(int b, int n)
{
	int i;
	
	for(i=b; i<(b+n); i++)
	{
		tkmm_pagebmp[i>>3]&=~(1<<(i&7));
	}
	return(0);
}

void *TKMM_PageToPointer(int n)
{
	byte *ptr;
	ptr=((byte *)TKMM_PAGEBASE)+(n<<12);
//	__debugbreak();
	return(ptr);
}

int TKMM_PointerToPage(void *ptr)
{
	int d;
	
	d=((byte *)ptr)-((byte *)TKMM_PAGEBASE);
	return(d>>12);
}

void *(*TKMM_PageAlloc_f)(int sz);
int (*TKMM_PageFree_f)(void *ptr, int sz);

#if 1
void *TKMM_PageAlloc(int sz)
	{ return(TKMM_PageAlloc_f(sz)); }
int TKMM_PageFree(void *ptr, int sz)
	{ return(TKMM_PageFree_f(ptr, sz)); }
#endif

void *TKMM_PageAllocL(int sz)
{
	void *p;
	int pg;
	
	pg=TKMM_AllocPages((sz+4095)>>12);
	if(pg<0)return(NULL);
	p=TKMM_PageToPointer(pg);
	return(p);
}

int TKMM_PageFreeL(void *ptr, int sz)
{
	void *p;
	int pg;

	pg=TKMM_PointerToPage(ptr);
	TKMM_FreePages(pg, (sz+4095)>>12);
	return(0);
	
//	pg=TKMM_AllocPages((sz+4095)>>12);
//	if(pg<0)return(NULL);
//	p=TKMM_PageToPointer(pg);
//	return(p);
}

#if 0
void *TKMM_PageAlloc(int sz)
	{ return(TKMM_PageAllocL(sz)); }
int TKMM_PageFree(void *ptr, int sz)
	{ return(TKMM_PageFreeL(ptr, sz)); }
#endif

void *tk_getsavedvbr(void);
int tk_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2);

bool tk_iskernel(void)
{
	if(tk_getsavedvbr())
		return(0);
	return(1);
}


void *TKMM_PageAllocV(int sz)
{
	TK_SysArg ar[4];
	void *p;
	
	ar[0].i=sz;
	tk_syscall(NULL, TK_UMSG_PAGEALLOC, &p, ar);
	return(p);
}

int TKMM_PageFreeV(void *ptr, int sz)
{
	TK_SysArg ar[4];
	void *p;
	
	ar[0].p=ptr;
	ar[1].i=sz;
	tk_syscall(NULL, TK_UMSG_PAGEFREE, &p, ar);
	return(p);
}

void TK_ExitV(int res)
{
	TK_SysArg ar[4];
	void *p;
	
	ar[0].i=res;
	tk_syscall(NULL, TK_UMSG_PGMEXIT, &p, ar);
	return(p);
}



void TKMM_Init()
{
	static int init=0;
	int i;

	if(init)return;
	init=1;

	TKMM_PageAlloc_f=TKMM_PageAllocL;
	TKMM_PageFree_f=TKMM_PageFreeL;
	
//	if(tk_getsavedvbr())
	if(!tk_iskernel())
//	if(0)
	{
		TKMM_PageAlloc_f=TKMM_PageAllocV;
		TKMM_PageFree_f=TKMM_PageFreeV;
	}

//	tk_ird_init();
	
	tkmm_pagebase=TKMM_PAGEBASE;
	tkmm_pageend=TKMM_PAGEEND;
	
	if(tk_ird_imgbuf)
	{
		if(	(((u32)tk_ird_imgbuf)<tkmm_pageend) &&
			(((u32)tk_ird_imgbuf)>tkmm_pagebase))
		{
			tkmm_pageend=(u32)tk_ird_imgbuf;
		}
	}
	
	tkmm_maxpage=(tkmm_pageend-tkmm_pagebase)>>12;
	
//	printf("TKMM_Init: heap=%dMB\n",
//		(tkmm_pageend-tkmm_pagebase)>>20);
	
	TKMM_MMList_Init();
}

void *TKMM_Malloc(int sz)
{
	TKMM_MemLnkObj *obj;
	void *ptr;
	int pg, np;
	
	if(sz<65536)
	{
		ptr=TKMM_MMList_Malloc(sz);

		if(!ptr)
			tk_printf("TKMM_Malloc: failed A %d\n", sz);

		return(ptr);
	}

#if 0
	np=(sz+sizeof(TKMM_MemLnkObj)+4095)>>12;
	pg=TKMM_AllocPages(np);
	if(pg<0)
	{
		tk_printf("TKMM_Malloc: failed B %d\n", sz);
		return(NULL);
	}
	ptr=TKMM_PageToPointer(pg);
#endif

#if 1
	np=(sz+sizeof(TKMM_MemLnkObj)+4095)>>12;
//	ptr=TKMM_PageAlloc(sz+sizeof(TKMM_MemLnkObj));
	ptr=TKMM_PageAlloc(np<<12);
#endif

//	__debugbreak();
	
	if(!ptr)
	{
		tk_printf("TKMM_Malloc: failed C %d\n", sz);
		return(NULL);
	}
	
//	ptr=TKMM_PageAlloc(sz+sizeof(TKMM_MemLnkObj));
	obj=ptr;
	obj->cnext=NULL;
	obj->cprev=NULL;
	obj->ix=np;
	obj->fl=5;

//	__debugbreak();

	if(((s64)ptr)&0xFFF)
		__debugbreak();

	tk_printf("TKMM_Malloc: pass D %X %X %d\n",
		obj, obj->data, sz);

	return((byte *)obj->data);
}

int TKMM_Free(void *ptr)
{
	TKMM_MemLnkObj *obj;
	int b, n;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
//	obj->fl|=1;

	if(obj->fl&4)
	{
//		b=TKMM_PointerToPage(obj);
//		TKMM_FreePages(b, obj->ix);
		TKMM_PageFree(obj, obj->ix<<12);
		return(0);
	}
	
	TKMM_MMList_FreeLnkObj(obj);
	return(0);
}

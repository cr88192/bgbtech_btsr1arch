#ifndef __TK_CLIB_ONLY__

byte *tk_ird_imgbuf=NULL;

u32 tkmm_pagebase, tkmm_pageend;
//static int tkmm_initvar=0;

byte tkmm_pagebmp[16384];
int tkmm_maxpage;
int tkmm_pagerov;

#ifndef FOURCC
#define FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif

#define BOOTPARM_MAGIC1		FOURCC('B', 'O', 'O', 'T')
#define BOOTPARM_MAGIC2		FOURCC('P', 'A', 'R', 'M')
#define BOOTPARM_MAGIC3		FOURCC('M', 'R', 'A', 'P')
#define BOOTPARM_MAGIC4		FOURCC('T', 'O', 'O', 'B')


struct BootParm_s {
u32 magic1;		//00
u32 magic2;		//04
u32 ramkib;		//08
u32 conctx;		//0C

u32 resv1;		//10
u32 resv2;		//14
u32 resv3;		//18
u32 resv4;		//1C

u32 resv5;		//20
u32 resv6;		//24
u32 resv7;		//28
u32 resv8;		//2C

u32 resv9;		//30
u32 resv10;		//34
u32 magic3;		//38
u32 magic4;		//3C
};

struct BootParm_s t_bootparm;
struct BootParm_s *bootparm;

int	tkmm_ramkib;
int	tkmm_rampage;


int TKMM_MMList_Init(void);
int TKMM_MMList_Free(void *ptr);
void *TKMM_MMList_MallocCat(int sz, int cat);
void *TKMM_MMList_MallocURo(int sz);
void *TKMM_MMList_Malloc(int sz);


int TKMM_InitBootParm()
{
	u32 *css, *cse, *cs;
	
	css=(u32 *)0x0000C000UL;
	cse=(u32 *)0x0000DFFCUL;
	
	bootparm=&t_bootparm;
	
	cs=css;
	while(cs<cse)
	{
		if(	(cs[ 0]==BOOTPARM_MAGIC1) && (cs[ 1]==BOOTPARM_MAGIC2) &&
			(cs[14]==BOOTPARM_MAGIC3) && (cs[15]==BOOTPARM_MAGIC4))
		{
			memcpy(bootparm, cs, sizeof(struct BootParm_s));
			break;
		}
		cs++;
	}
	
	if(bootparm->magic1==BOOTPARM_MAGIC1)
	{
		tkmm_ramkib=bootparm->ramkib;
		tkmm_rampage=(((u64)tkmm_ramkib)<<10)>>TKMM_PAGEBITS;
	}else
	{
		tkmm_ramkib=131072;
		tkmm_rampage=(((u64)tkmm_ramkib)<<10)>>TKMM_PAGEBITS;
	}
	return(0);
}

void *TKMM_MMList_Malloc(int sz);
u64 *TKMM_MMCell_GetLnkObjCellHeadPtr(TKMM_MemLnkObj *obj, void *ptr);
int TKMM_MMList_FreeLnkObj(TKMM_MemLnkObj *obj);
int TKMM_MMCell_FreeLnkObjCellPtr(TKMM_MemLnkObj *obj, void *ptr);

// static int tkmm_findfree_rec=0;

int TKMM_FindFreePage(void)
{
	int i0, i1;
	int i, j, m;
	
	i=tkmm_pagerov;
	m=tkmm_maxpage;
	while(i<m)
	{
		j=tkmm_pagebmp[i>>3];
		if(j==0xFF)
			{ i=(i+8)&(~7); continue; }
		if(j&(1<<(i&7)))
			{ i++; continue; }
		tkmm_pagerov=i+1;
		return(i);
	}
	
	if(tkmm_pagerov!=0)
	{
		tkmm_pagerov=0;
		i=TKMM_FindFreePage();
		return(i);
	}

	return(-1);
}

int TKMM_AllocPage(void)
{
	byte *bm;
	int i0, i1;
	int i, j, m;
	
	bm=tkmm_pagebmp;
	i=tkmm_pagerov;
	m=tkmm_maxpage;
	while(i<m)
	{
		j=bm[i>>3];
		if(j==0xFF)
			{ i=(i+8)&(~7); continue; }
		if(j&(1<<(i&7)))
			{ i++; continue; }
		j|=1<<(i&7);
		bm[i>>3]=j;
		break;
	}
	
	if(i<m)
	{
		tkmm_pagerov=i+1;
//		__debugbreak();
		return(i);
	}
	
	if(tkmm_pagerov!=0)
	{
		tkmm_pagerov=0;
		i=TKMM_AllocPage();
		return(i);
	}

	return(-1);
}

int TKMM_FindFreePages(int n)
{
	byte *bm;
	int i0, i1;
	int i, j, m;
	
	if(n<=0)
		return(-1);
	
//	if(n==1)
//		return(TKMM_FindFreePage());
	
	bm=tkmm_pagebmp;
	i=tkmm_pagerov;
	m=tkmm_maxpage;
	while(i<m)
	{
		if(bm[i>>3]&(1<<(i&7)))
		{
			while((bm[i>>3]==0xFF) && (i<m))
			{
				i=(i+8)&(~7);
				continue;
			}
			while((bm[i>>3]&(1<<(i&7))) && (i<m))
				i++;
			continue;
		}
		i0=i; i1=i0+n;
		if(i1>m)break;
		while(!(bm[i>>3]&(1<<(i&7))) && (i<i1))
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
	byte *bm;
	int i, j, k;
	
	if(n<=0)
	{
		tk_printf("TKMM_AllocPages: Bad Page Count, n=%d\n", n);
		return(-1);
	}

#if 1
	if(n==1)
	{
		i=TKMM_AllocPage();
		if(i<0)
		{
			tk_printf("TKMM_AllocPages: Out Of Memory, n=%d\n", n);
			return(-1);
		}

//		tk_printf("TKMM_AllocPages: %X\n", i);
		return(i);
	}
#endif
	
	i=TKMM_FindFreePages(n);
	if(i<0)
	{
		tk_printf("TKMM_AllocPages: Out Of Memory, n=%d\n", n);
		return(-1);
	}

	if((i+n)>tkmm_maxpage)
	{
		__debugbreak();
	}
	
	bm=tkmm_pagebmp;
	j=i; k=j+n;
	while(j<k)
	{
		bm[j>>3]|=(1<<(j&7));
		j++;
	}

//	tk_printf("TKMM_AllocPages: %X..%X\n", i, k-1);

	return(i);
}

int TKMM_AllocPagesZeroed(int n)
{
	byte *ptr;
	int i0;
	
	i0=TKMM_AllocPages(n);
	if(i0<0)
		return(i0);

	ptr=((byte *)TKMM_PAGEBASE)+(i0<<TKMM_PAGEBITS);
	memset(ptr, 0, n<<TKMM_PAGEBITS);

	return(i0);
}

int TKMM_FreePages(int b, int n)
{
	int i;
	
	if((b<0) || (n<=0))
		return(-1);
		
	if((b+n)>tkmm_maxpage)
	{
		__debugbreak();
	}
	
	for(i=b; i<(b+n); i++)
	{
		tkmm_pagebmp[i>>3]&=~(1<<(i&7));
	}
	
	if(b<tkmm_pagerov)
		tkmm_pagerov=b;
	
	return(0);
}

void *TKMM_PageToPointer(int n)
{
	byte *ptr;
	ptr=((byte *)TKMM_PAGEBASE)+(n<<TKMM_PAGEBITS);
//	__debugbreak();
	return(ptr);
}

int TKMM_PointerToPage(void *ptr)
{
	int d;
	
	d=((byte *)ptr)-((byte *)TKMM_PAGEBASE);
	return(d>>TKMM_PAGEBITS);
}

void *TKMM_PageAllocL(int sz)
{
	void *p;
	int pg;
	
	pg=TKMM_AllocPages((sz+TKMM_PAGEMASK)>>TKMM_PAGEBITS);
	if(pg<0)
	{
		tk_printf("TKMM_PageAlloc: Failed Alloc %d\n", sz);
		return(NULL);
	}
	p=TKMM_PageToPointer(pg);
	return(p);
}

int TKMM_PageFreeL(void *ptr, int sz)
{
	void *p;
	int pg;

	pg=TKMM_PointerToPage(ptr);
	TKMM_FreePages(pg, (sz+TKMM_PAGEMASK)>>TKMM_PAGEBITS);
	return(0);
	
//	pg=TKMM_AllocPages((sz+TKMM_PAGEMASK)>>TKMM_PAGEBITS);
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

#endif

void *(*TKMM_PageAlloc_f)(int sz);
int (*TKMM_PageFree_f)(void *ptr, int sz);

#if 1
void *TKMM_PageAlloc(int sz)
	{ return(TKMM_PageAlloc_f(sz)); }
int TKMM_PageFree(void *ptr, int sz)
	{ return(TKMM_PageFree_f(ptr, sz)); }
#endif

extern volatile u64 __arch_tbr;

void *tk_getsavedvbr(void);
// int tk_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2);

int tk_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	int (*SysCall)(void *sObj, int uMsg, void *vParm1, void *vParm2);
	TKPE_TaskInfo *task;

	task=__arch_tbr;
	if(!task)
		__debugbreak();
//	if(!task->SysCall)
//		__debugbreak();
//	return(task->SysCall(sObj, uMsg, vParm1, vParm2));

	SysCall=(void *)(task->SysCall);
	if(!SysCall)
		__debugbreak();
	return(SysCall(sObj, uMsg, vParm1, vParm2));
}

bool tk_iskernel(void)
{
#ifndef __TK_CLIB_ONLY__
	if(tk_getsavedvbr())
		return(0);
	return(1);
#else
	return(0);
#endif
}


void *TKMM_PageAllocV(int sz)
{
	TK_SysArg ar[4];
	void *p;
	
//	tk_printf("TKMM_PageAllocV: sz=%d\n", sz);
	
	p=NULL;
	ar[0].i=sz;
	tk_syscall(NULL, TK_UMSG_PAGEALLOC, &p, ar);

//	tk_printf("TKMM_PageAllocV: Vp=%p, p=%p\n", &p, p);

	return(p);
}

int TKMM_PageFreeV(void *ptr, int sz)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].p=ptr;
	ar[1].i=sz;
	i=tk_syscall(NULL, TK_UMSG_PAGEFREE, &p, ar);
	return(i);
}

void TK_ExitV(int res)
{
	TK_SysArg ar[4];
	void *p;
	
	ar[0].i=res;
	tk_syscall(NULL, TK_UMSG_PGMEXIT, &p, ar);
}


static int tkmm_is_init=0;

void TKMM_Init()
{
	int i;

	if(tkmm_is_init)return;
	tkmm_is_init=1;

#ifndef __TK_CLIB_ONLY__
//	if(tk_getsavedvbr())
	if(!tk_iskernel())
//	if(0)
	{
		TKMM_PageAlloc_f=TKMM_PageAllocV;
		TKMM_PageFree_f=TKMM_PageFreeV;
	}else
	{
		TKMM_PageAlloc_f=TKMM_PageAllocL;
		TKMM_PageFree_f=TKMM_PageFreeL;

//		tk_ird_init();

		TKMM_InitBootParm();

//		tkmm_pagebase=TKMM_PAGEBASE;
//		tkmm_pageend=TKMM_PAGEEND;

		tkmm_pagebase=TKMM_PAGEBASE;
		tkmm_pageend=TKMM_PAGEBASE+(tkmm_rampage<<TKMM_PAGEBITS);

		memset(tkmm_pagebmp, 0, 16384);
		
		if(!TKMM_PageAlloc_f)
		{
			__debugbreak();
		}

#if 0
		if(tk_ird_imgbuf)
		{
			if(	(((u32)tk_ird_imgbuf)<tkmm_pageend) &&
				(((u32)tk_ird_imgbuf)>tkmm_pagebase))
			{
				tkmm_pageend=(u32)tk_ird_imgbuf;
			}
		}
#endif
	
		tkmm_maxpage=(tkmm_pageend-tkmm_pagebase)>>TKMM_PAGEBITS;
		tk_printf("TKMM_Heap %dkB\n", tkmm_maxpage<<(TKMM_PAGEBITS-10));
	}
#else
	TKMM_PageAlloc_f=TKMM_PageAllocV;
	TKMM_PageFree_f=TKMM_PageFreeV;
#endif
	
//	printf("TKMM_Init: heap=%dMB\n",
//		(tkmm_pageend-tkmm_pagebase)>>20);
	
	TKMM_MMList_Init();
	
	TKMM_LVA_ArrayInit();
}

void *TKMM_Malloc(int sz)
{
	TKMM_MemLnkObj *obj;
	void *ptr, *ptr1;
	int pg, np, np1;
	
	if(!TKMM_PageAlloc_f)
		__debugbreak();

//	tk_printf("TKMM_Malloc: sz=%d\n", sz);

#if 0
	if(sz<TKMM_MAXMMCELLSZ)
	{
		ptr=TKMM_MMCell_Malloc(sz);
		return(ptr);
	}
#endif

//	if(sz<65536)
	if(sz<TKMM_MAXMMLISTSZ)
	{
		ptr=TKMM_MMList_Malloc(sz);

		if(!ptr)
			tk_printf("TKMM_Malloc: failed A %d\n", sz);

		return(ptr);
	}

#if 0
	np=(sz+sizeof(TKMM_MemLnkObj)+TKMM_PAGEMASK)>>TKMM_PAGEBITS;
	pg=TKMM_AllocPages(np);
	if(pg<0)
	{
		tk_printf("TKMM_Malloc: failed B %d\n", sz);
		return(NULL);
	}
	ptr=TKMM_PageToPointer(pg);
#endif

#if 0
	np=(sz+sizeof(TKMM_MemLnkObj)+TKMM_PAGEMASK)>>TKMM_PAGEBITS;
//	ptr=TKMM_PageAlloc(sz+sizeof(TKMM_MemLnkObj));
	ptr=TKMM_PageAlloc(np<<TKMM_PAGEBITS);
	
//	memset(ptr, 0, np<<TKMM_PAGEBITS);

	if(!TKMM_PageAlloc_f)
	{
		ptr1=ptr;
		np1=np;
//		tk_printf("%p %X\n", ptr, np);
		__debugbreak();
	}

#endif

	ptr=TKMM_MMList_AllocBrk(sz+sizeof(TKMM_MemLnkObj)+15);
	np=(sz+sizeof(TKMM_MemLnkObj)+TKMM_PAGEMASK)>>TKMM_PAGEBITS;

//	__debugbreak();
	
	if(!ptr)
	{
		tk_printf("TKMM_Malloc: failed C %d\n", sz);
		return(NULL);
	}
	
//	ptr=TKMM_PageAlloc(sz+sizeof(TKMM_MemLnkObj));
	obj=ptr;

	if(!obj)
		{ __debugbreak(); }

//	obj->cnext=NULL;
//	obj->cprev=NULL;
	obj->ix=np;
	obj->fl=5;
	obj->cat=0;

//	__debugbreak();

	if(((s64)ptr)&TKMM_PAGEMASK)
		__debugbreak();

//	tk_printf("TKMM_Malloc: pass D %X %X %d\n",
//		obj, obj->data, sz);

//	if(!TKMM_PageAlloc_f)
//		__debugbreak();

	return((byte *)(obj->data));
}

int TKMM_Free(void *ptr)
{
	TKMM_MemLnkObj *obj;
	int b, n;

	if(!ptr)return(-1);
//	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
//	obj->fl|=1;

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
	{
		return(-1);
	}

//	return(-1);

	if(obj->fl&8)
	{
		TKMM_MMCell_FreeLnkObjCellPtr(obj, ptr);
		return(0);
	}
	
	if(obj->fl&4)
	{
//		b=TKMM_PointerToPage(obj);
//		TKMM_FreePages(b, obj->ix);
		TKMM_PageFree(obj, obj->ix<<TKMM_PAGEBITS);
		return(0);
	}

	TKMM_MMList_FreeLnkObj(obj);
	return(0);
}

int TKMM_GetSize(void *ptr)
{
	TKMM_MemLnkObj *obj;
	int b, n, sz1;

	if(!ptr)
		return(-1);

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
	{
		return(-1);
	}

	if(obj->fl&8)
	{
		sz1=TKMM_MMCell_GetLnkObjCellSize(obj, ptr);
		return(sz1);
	}
	
	if(obj->fl&4)
	{
		sz1=obj->ix<<TKMM_PAGEBITS;
	}else
	{
		sz1=TKMM_FxiToSize(obj->ix);
	}
	return(sz1);
}

int TKMM_IncRef(void *ptr)
{
	TKMM_MemLnkObj *obj;
	u64	*pv;
	u64 v;
	int i, b, n, sz1;

	if(!ptr)
		return(-1);

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
		{ return(-1); }

	if(obj->fl&8)
	{
		pv=TKMM_MMCell_GetLnkObjCellHeadPtr(obj, ptr);
		v=*pv;
		i=(v>>16)&255;
		if(i<255)i++;
		v=v&(~0x00FF0000ULL);
		v|=i<<16;
		*pv=v;
		return(0);
	}

	i=obj->refcnt;
	if(i<255)i++;
	obj->refcnt=i;
	return(0);
}

int TKMM_DecRef(void *ptr)
{
	TKMM_MemLnkObj *obj;
	u64	*pv;
	u64 v;
	int i, b, n, sz1;

	if(!ptr)
		return(-1);

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
		{ return(-1); }

	if(obj->fl&8)
	{
		pv=TKMM_MMCell_GetLnkObjCellHeadPtr(obj, ptr);
		v=*pv;
		i=(v>>16)&255;
		if(i<255)i--;
		v=v&(~0x00FF0000ULL);
		v|=i<<16;
		*pv=v;
		if(i<=0)
			{ TKMM_MMCell_FreeLnkObjCellPtr(obj, ptr); }
		return(0);
	}

	i=obj->refcnt;
	if(i<255)i--;
	obj->refcnt=i;
	
	if(i<=0)
	{
		TKMM_Free(ptr);
		return(1);
	}
	return(0);
}

int TKMM_GetTag(void *ptr)
{
	TKMM_MemLnkObj *obj;
	u64	*pv;
	u64 v;
	int i, b, n, sz1;

	if(!ptr)
		return(-1);

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
		{ return(-1); }

	if(obj->fl&8)
	{
		pv=TKMM_MMCell_GetLnkObjCellHeadPtr(obj, ptr);
		v=*pv;
		return(v&0xFFFF);
	}

	return(obj->dty_tag);
}

int TKMM_SetTag(void *ptr, int tag)
{
	TKMM_MemLnkObj *obj;
	u64	*pv;
	u64 v;
	int i, b, n, sz1;

	if(!ptr)
		return(-1);

	obj=TKMM_MMList_GetPtrLnkObj(ptr);
	if(!obj)
		{ return(-1); }

	if(obj->fl&8)
	{
		pv=TKMM_MMCell_GetLnkObjCellHeadPtr(obj, ptr);
		v=*pv;
		v=v&(~0x0000FFFFULL);
		v|=tag&0xFFFF;
		*pv=v;
		return(0);
	}

	obj->dty_tag=tag;
	return(0);
}

#ifndef __TK_CLIB_ONLY__
void *TKMM_MallocCat(int sz, int cat)
{
	TKMM_MemLnkObj *obj;
	void *ptr, *ptr1;
	int pg, np, np1;
	
	if(sz<TKMM_MAXMMLISTSZ)
	{
		ptr=TKMM_MMList_MallocCat(sz, cat);

		if(!ptr)
			tk_printf("TKMM_MallocURo: failed A %d\n", sz);

		return(ptr);
	}

	ptr=TKMM_MMList_AllocBrkCat(sz+sizeof(TKMM_MemLnkObj), cat);
	np=(sz+sizeof(TKMM_MemLnkObj)+TKMM_PAGEMASK)>>TKMM_PAGEBITS;

	if(!ptr)
	{
		tk_printf("TKMM_MallocURo: failed C %d\n", sz);
		return(NULL);
	}
	
	obj=ptr;

	if(!obj)
		{ __debugbreak(); }

//	obj->cnext=NULL;
//	obj->cprev=NULL;
	obj->ix=np;
	obj->fl=5;
	obj->cat=cat;

	if(((s64)ptr)&TKMM_PAGEMASK)
		__debugbreak();

	return((byte *)(obj->data));
}

void *TKMM_MallocURo(int sz)
{
	return(TKMM_MallocCat(sz, 1));
}

void *TKMM_MallocKrn(int sz)
{
	return(TKMM_MallocCat(sz, 2));
}

void *TKMM_MallocUsr(int sz)
{
	return(TKMM_MallocCat(sz, 3));
}

#endif

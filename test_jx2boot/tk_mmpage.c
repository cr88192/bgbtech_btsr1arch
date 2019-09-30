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
	return(ptr);
}

int TKMM_PointerToPage(void *ptr)
{
	int d;
	
	d=((byte *)ptr)-((byte *)TKMM_PAGEBASE);
	return(d>>12);
}

void *TKMM_PageAlloc(int sz)
{
	void *p;
	int pg;
	
	pg=TKMM_AllocPages((sz+4095)>>12);
	if(pg<0)return(NULL);
	p=TKMM_PageToPointer(pg);
	return(p);
}

int TKMM_PageFree(void *ptr, int sz)
{
	void *p;
	int pg;

	pg=TKMM_PointerToPage(ptr);
	TKMM_FreePages(pg, (sz+4095)>>12);
	return(0);
	
//	pg=TKMM_AllocPages((sz+4095)>>12);
//	if(pg<0)return(NULL);
//	p=TKMM_PageToPointer(pg);
	return(p);
}


void TKMM_Init()
{
	static int init=0;
	int i;

	if(init)return;
	init=1;
		
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
			printf("TKMM_Malloc: failed A %d\n", sz);

		return(ptr);
	}

	np=(sz+sizeof(TKMM_MemLnkObj)+4095)>>12;
	pg=TKMM_AllocPages(np);
	if(pg<0)
	{
		printf("TKMM_Malloc: failed B %d\n", sz);
		return(NULL);
	}
	ptr=TKMM_PageToPointer(pg);
	
//	__debugbreak();
	
	if(!ptr)
	{
		printf("TKMM_Malloc: failed C %d\n", sz);
		return(NULL);
	}
	
//	ptr=TKMM_PageAlloc(sz+sizeof(TKMM_MemLnkObj));
	obj=ptr;
	obj->cnext=NULL;
	obj->cprev=NULL;
	obj->ix=np;
	obj->fl=5;

//	__debugbreak();

	printf("TKMM_Malloc: pass D %X %X %d\n",
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
		b=TKMM_PointerToPage(obj);
		TKMM_FreePages(b, obj->ix);
		return(0);
	}
	
	TKMM_MMList_FreeLnkObj(obj);
	return(0);
}

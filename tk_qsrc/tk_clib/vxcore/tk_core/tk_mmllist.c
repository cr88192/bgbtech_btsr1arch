void *(*TKMM_PageAlloc_f)(int sz);
int (*TKMM_PageFree_f)(void *ptr, int sz);

TKMM_MemLnkObj *tkmm_mmlist_freelist[256];

byte *tkmm_mmlist_vrm_brkbuf[1024];
byte *tkmm_mmlist_vrm_brkend[1024];
int tkmm_mmlist_vrm_brkflg[1024];
int tkmm_mmlist_n_vrm=0;

byte *tkmm_mmlist_brkbuf=NULL;
byte *tkmm_mmlist_brkend;
byte *tkmm_mmlist_brkpos;


void __setmemtrap(void *ptr, int mode);

#ifndef __BJX2__
void __setmemtrap(void *ptr, int mode)
{
	/* NO-OP */
}
#endif

void *TKMM_MMList_AllocBrk(int sz)
{
	TKMM_MemLnkSeg *seg;
	byte *ptr;
	int i;

//	if(sz>=65536)
	if(sz>=TKMM_MAXMMLISTSZ)
	{
//		tk_puts("TKMM_MMList_AllocBrk A\n");
//		tk_printf("TKMM_MMList_AllocBrk A %d\n", sz);
		ptr=TKMM_PageAlloc(sz);
		
		if(!ptr)
		{
//			tk_puts("TKMM_MMList_AllocBrk A: Fail\n");
			return(NULL);
		}

//		tk_printf("TKMM_MMList_AllocBrk A-1 %p..%p\n", ptr, ptr+sz);
		
		memset(ptr, 0, sz);

		i=tkmm_mmlist_n_vrm++;
		if(i>=1024)
		{
			__debugbreak();
		}

		while((i>0) && (tkmm_mmlist_vrm_brkbuf[i-1]>ptr))
		{
			tkmm_mmlist_vrm_brkbuf[i]=tkmm_mmlist_vrm_brkbuf[i-1];
			tkmm_mmlist_vrm_brkend[i]=tkmm_mmlist_vrm_brkend[i-1];
			tkmm_mmlist_vrm_brkflg[i]=tkmm_mmlist_vrm_brkflg[i-1];
			i--;
		}

		tkmm_mmlist_vrm_brkbuf[i]=ptr;
		tkmm_mmlist_vrm_brkend[i]=ptr+sz;
		tkmm_mmlist_vrm_brkflg[i]=1;

		return(ptr);
	}
	
	sz=(sz+15)&(~15);
	
	if(tkmm_mmlist_brkbuf && ((tkmm_mmlist_brkpos+sz)>tkmm_mmlist_brkend))
	{
		tk_puts("TKMM_MMList_AllocBrk B\n");
		tkmm_mmlist_brkbuf=NULL;
	}
	
	if(!tkmm_mmlist_brkbuf)
	{
		tk_puts("TKMM_MMList_AllocBrk C\n");
//		tkmm_mmlist_brkbuf=TKMM_PageAlloc(1<<20);
		tkmm_mmlist_brkbuf=TKMM_PageAlloc(1<<TKMM_BRKBITS);
//		tkmm_mmlist_brkbuf=TKMM_PageAlloc(1<<18);
		
		if(!tkmm_mmlist_brkbuf)
		{
			tk_puts("TKMM_MMList_AllocBrk C: Fail\n");
			return(NULL);
		}
		
//		memset(tkmm_mmlist_brkbuf, 0, 1<<20);

//		tkmm_mmlist_brkend=tkmm_mmlist_brkbuf+(1<<20);
		tkmm_mmlist_brkend=tkmm_mmlist_brkbuf+(1<<TKMM_BRKBITS);
//		tkmm_mmlist_brkend=tkmm_mmlist_brkbuf+(1<<18);
//		tkmm_mmlist_brkpos=tkmm_mmlist_brkbuf;

		seg=(TKMM_MemLnkSeg *)tkmm_mmlist_brkbuf;
//		tkmm_mmlist_brkpos=(byte *)(seg->data);
//		i=tkmm_mmlist_brkpos-tkmm_mmlist_brkbuf;

		i=(byte *)(seg->data)-tkmm_mmlist_brkbuf;
		i=(i+15)&(~15);
		tkmm_mmlist_brkpos=tkmm_mmlist_brkbuf+i;
		
		seg->nblk=0;

		i=tkmm_mmlist_n_vrm++;
		if(i>=1024)
		{
			__debugbreak();
		}

		while((i>0) && (tkmm_mmlist_vrm_brkbuf[i-1]>ptr))
		{
			tkmm_mmlist_vrm_brkbuf[i]=tkmm_mmlist_vrm_brkbuf[i-1];
			tkmm_mmlist_vrm_brkend[i]=tkmm_mmlist_vrm_brkend[i-1];
			tkmm_mmlist_vrm_brkflg[i]=tkmm_mmlist_vrm_brkflg[i-1];
			i--;
		}

		tkmm_mmlist_vrm_brkbuf[i]=tkmm_mmlist_brkbuf;
		tkmm_mmlist_vrm_brkend[i]=tkmm_mmlist_brkend;
		tkmm_mmlist_vrm_brkflg[i]=0;
	}

//	tk_puts("TKMM_MMList_AllocBrk D\n");

	seg=(TKMM_MemLnkSeg *)tkmm_mmlist_brkbuf;
	
	ptr=tkmm_mmlist_brkpos;
	tkmm_mmlist_brkpos=ptr+sz;

#if 1
	i=seg->nblk++;
	if(i>=TKMM_MAXMMSEGBLK)
		{ __debugbreak(); }
//	seg->blk[i]=ptr;
	seg->blk[i]=((byte *)ptr)-((byte *)seg);
#endif

	return(ptr);
}

/** Convert size into a 5.3 minifloat, Round Up */
int TKMM_SizeToFxiU(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
//	while((fr>>3)>1)
	while(fr>=16)
		{ fr=(fr+1)>>1; ex++; }
	i=(fr&7)|(ex<<3);

	if(i&(~255))
		__debugbreak();

	return(i);
}

/** Convert size into a 5.3 minifloat, Round Down */
int TKMM_SizeToFxiD(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
//	while((fr>>3)>1)
	while(fr>=16)
		{ fr=fr>>1; ex++; }
	i=(fr&7)|(ex<<3);
	
	if(i&(~255))
		__debugbreak();
	
	return(i);
}

/** Convert Fxi to size */
int TKMM_FxiToSize(int ix)
{
	int ex, fr;
	int sz;
	
	if(ix<8)return(ix);
	fr=(ix&7)|8; ex=(ix>>3)&31;
	sz=fr<<ex;
	return(sz);
}

TKMM_MemLnkObj *TKMM_MMList_AllocObj(int sz)
{
	TKMM_MemLnkObj *obj, *obj1;
	byte *p1, *p2;
	int ix, ix1;
	int sz1, sz2;

	if(!TKMM_PageAlloc_f)
		__debugbreak();

	if(sz<=0)return(NULL);
//	if(sz<256)sz=256;
	if(sz<64)sz=64;
	
//	putc('<');
//	sz=(sz+TKMM_OFFS_DATA+7)&(~7);
	sz=(sz+TKMM_OFFS_DATA+15)&(~7);
//	sz=(sz+TKMM_OFFS_DATA+31)&(~7);

	ix=TKMM_SizeToFxiU(sz);
//	printf("%d", ix);

	if(ix&(~255))
		__debugbreak();

	obj=tkmm_mmlist_freelist[ix];
	if(obj)
	{
//		putc('A');
		tkmm_mmlist_freelist[ix]=*(TKMM_MemLnkObj **)(obj->data);
//		obj->ix=0;
		obj->fl&=~1;
//		putc('>');
//		return((byte *)obj->data);
		return(obj);
	}

//	putc('B');

#if 1
	sz1=TKMM_FxiToSize(ix);
	p1=TKMM_MMList_AllocBrk(sz1);

	if(!p1)
	{
		tk_printf("TKMM_MMList_Malloc: BRK Failed %d\n", sz1);
		__debugbreak();
		return(NULL);
	}

	p2=p1+sz1;
	obj=(TKMM_MemLnkObj *)p1;
//	sz2=p2-((byte *)obj1->data);
	sz2=p2-((byte *)obj->data);
	obj->ix=TKMM_SizeToFxiD(sz2);
	obj->fl=3;
	obj->check=0x5A;
	obj->cnext=(TKMM_MemLnkObj *)p2;
	obj->cprev=NULL;

	obj->fl&=~1;

	__setmemtrap(obj, 3);

//	return((byte *)obj->data);
	return(obj);
#endif

#if 0
	ix1=ix;
	while(!obj && (ix1<255))
		{ ix1++; obj=tkmm_mmlist_freelist[ix1]; }

	if(!obj)
	{
//		putc('C');

		sz1=TKMM_FxiToSize(ix);
		sz1=(sz1+16+65535)&(~65535);
		
		p1=TKMM_PageAlloc(sz1);
		if(!p1)
		{
//			putc('!');
//			putc('>');
			return(NULL);
		}
		p2=p1+sz1;
		obj=(TKMM_MemLnkObj *)p1;
		sz2=p2-((byte *)obj1->data);
		obj->ix=TKMM_SizeToFxiD(sz2);
		obj->fl=3;
		obj->check=0x5A;
		obj->cnext=(TKMM_MemLnkObj *)p2;
		obj->cprev=NULL;
	}

//	putc('D');

	sz1=TKMM_FxiToSize(ix);
//	printf("%d", ix);
	sz1=(sz1+7)&(~7);
	p1=((byte *)obj->data)+sz1;
	p2=(byte *)obj->cnext;
	if((p2-p1)>256)
	{
//		putc('E');

		obj1=(TKMM_MemLnkObj *)p1;
		if(!(obj->fl&2))
			{ obj->cnext->cprev=obj1; }
		obj1->cnext=obj->cnext;
		obj1->cprev=obj;
		obj->cnext=obj1;
		obj1->fl=obj->fl;
		obj->fl&=~2;
		obj->ix=ix;
		obj->check=0x5A;
		
		sz2=p2-((byte *)obj1->data);
		obj1->ix=TKMM_SizeToFxiD(sz2);
		
		*(TKMM_MemLnkObj **)(obj1->data)=tkmm_mmlist_freelist[obj1->ix];
		tkmm_mmlist_freelist[obj1->ix]=obj1;
	}

//	putc('F');

	obj->fl&=~1;
//	putc('>');
	return((byte *)obj->data);
#endif
}

void *TKMM_MMList_Malloc(int sz)
{
	TKMM_MemLnkObj *obj;
	obj=TKMM_MMList_AllocObj(sz);
	return((byte *)obj->data);
}


int TKMM_MMList_FreeLnkObj(TKMM_MemLnkObj *obj)
{
	__setmemtrap(obj, 2);

	if(obj->check!=0x5A)
	{
		tk_puts("TKMM_MMList_FreeLnkObj: Check Value Fail\n");
		return(-1);
	}

	if((obj->ix)&(~255))
	{
		__debugbreak();
		tk_puts("TKMM_MMList_FreeLnkObj: List Index Fail\n");
		return(-1);
	}

	obj->fl|=1;
	*(TKMM_MemLnkObj **)(obj->data)=tkmm_mmlist_freelist[obj->ix];
	tkmm_mmlist_freelist[obj->ix]=obj;
	return(0);
}

int TKMM_MMList_Free(void *ptr)
{
	TKMM_MemLnkObj *obj;

	if(!ptr)return(-1);

	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
	__setmemtrap(obj, 2);

	obj->fl|=1;
	
	TKMM_MMList_FreeLnkObj(obj);
	return(0);
}

int TKMM_MMList_Init(void)
{
	int i;

	for(i=0; i<256; i++)
		tkmm_mmlist_freelist[i]=NULL;
	return(0);
}

int TKMM_MMList_GetPtrHeapIndex(void *ptr)
{
	byte *vss, *vse;
	byte *p;
	int i, vrn;
	
	p=ptr;
	vrn=tkmm_mmlist_n_vrm;
	for(i=0; i<vrn; i++)
	{
		vss=tkmm_mmlist_vrm_brkbuf[i];
		vse=tkmm_mmlist_vrm_brkend[i];
		if((p>=vss) && (p<vse))
			return(i);
	}
	
	return(-1);
}

TKMM_MemLnkObj *TKMM_MMList_GetPtrLnkObj(void *ptr)
{
	TKMM_MemLnkSeg *seg;
	TKMM_MemLnkObj *obj;
	byte *vss, *vse, *p;
	int i, ns, nb, sz1;
	
	ns=TKMM_MMList_GetPtrHeapIndex(ptr);
	if(ns<0)
		return(NULL);

	i=tkmm_mmlist_vrm_brkflg[ns];
	if(i&1)
	{
		obj=(TKMM_MemLnkObj *)(tkmm_mmlist_vrm_brkbuf[ns]);
		return(obj);
	}

	seg=(TKMM_MemLnkSeg *)(tkmm_mmlist_vrm_brkbuf[ns]);
	p=ptr;
	
	nb=seg->nblk;
	for(i=0; i<nb; i++)
	{
//		obj=seg->blk[i];
		obj=(TKMM_MemLnkObj *)((byte *)seg+seg->blk[i]);
		sz1=TKMM_FxiToSize(obj->ix);
		vss=(byte *)obj;
		vse=((byte *)(obj->data))+sz1;
		
		if((p>=vss) && (p<vse))
			return(obj);
	}
	
	return(NULL);
}

int TKMM_MMList_CheckPtrIsHeap(void *ptr)
{
	return(TKMM_MMList_GetPtrHeapIndex(ptr)>=0);
}

int TKMM_MMList_GetTag(void *ptr)
{
	TKMM_MemLnkObj *obj;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
	return(obj->dty_tag);
}


int TKMM_MMList_SetTag(void *ptr, int tag)
{
	TKMM_MemLnkObj *obj;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
	__setmemtrap(obj, 2);
	obj->dty_tag=tag;
	__setmemtrap(obj, 3);
	return(0);
}

int TKMM_MMList_GetSize(void *ptr)
{
	TKMM_MemLnkObj *obj;
	int sz1;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);

	if(obj->fl&4)
	{
		sz1=obj->ix<<TKMM_PAGEBITS;
	}else
	{
		sz1=TKMM_FxiToSize(obj->ix);
	}
	return(sz1);
}

void *tk_malloc(int sz)
{
//	if(!TKMM_PageAlloc_f)
//		__debugbreak();

//	return(TKMM_MMList_Malloc(sz));
	return(TKMM_Malloc(sz));
}

int tk_free(void *ptr)
{
//	return(TKMM_MMList_Free(ptr));
	return(TKMM_Free(ptr));
}

int tk_msize(void *ptr)
{
	return(TKMM_GetSize(ptr));
}

void *tk_realloc(void *ptr, int sz)
{
	int osz;
	void *ptr1;
	
	if(!ptr)
	{
		return(tk_malloc(sz));
	}
	
//	osz=TKMM_MMList_GetSize(ptr);
	osz=tk_msize(ptr);
	if(osz>=sz)
		return(ptr);
	
	ptr1=tk_malloc(sz);
	memcpy(ptr1, ptr, osz);
	tk_free(ptr);
	return(ptr1);
}


char *tk_strdup_in(char *str)
{
	return(TKMM_LVA_Strdup(str));
}
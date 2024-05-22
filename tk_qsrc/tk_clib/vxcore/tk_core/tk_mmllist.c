void *(*TKMM_PageAlloc_f)(int sz);
int (*TKMM_PageFree_f)(void *ptr, int sz);

// TKMM_MemLnkObj *tkmm_mmlist_freelist[256];
// TKMM_MemLnkObj *tkmm_mmlist_freelist_uro[256];

// TKMM_MemLnkObj *tkmm_mmlist_freelist_cat[8][256];
TKMM_MemLnkObj *tkmm_mmlist_freelist_cat[8*256];

byte *tkmm_mmlist_vrm_brkbuf[1024];
byte *tkmm_mmlist_vrm_brkend[1024];
int tkmm_mmlist_vrm_brkflg[1024];
int tkmm_mmlist_n_vrm=0;

// byte *tkmm_mmlist_brkbuf=NULL;
// byte *tkmm_mmlist_brkend;
// byte *tkmm_mmlist_brkpos;
// byte *tkmm_mmlist_brkbuf_uro=NULL;
// byte *tkmm_mmlist_brkend_uro;
// byte *tkmm_mmlist_brkpos_uro;

byte *tkmm_mmlist_brkbuf_c[8];
byte *tkmm_mmlist_brkend_c[8];
byte *tkmm_mmlist_brkpos_c[8];
int tkmm_mmlist_brkpos_init=0;

int tk_mprotect(void *addr, size_t len, int prot);

void __setmemtrap(void *ptr, int mode);

#ifndef __BJX2__
void __setmemtrap(void *ptr, int mode)
{
	/* NO-OP */
}
#endif

void TKMM_MMList_AddVrmBrk(byte *ptr, int sz, int flag)
{
	int i;

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
	tkmm_mmlist_vrm_brkflg[i]=flag;
}

#if 0
void *TKMM_MMList_AllocBrk(int sz)
{
	TKMM_MemLnkSeg *seg;
	byte *ptr;
	int i;

//	if(sz>=65536)
	if(sz>=TKMM_MAXMMLISTSZ)
	{
//		tk_puts("TKMM_MMList_AllocBrk A\n");
//		tk_dbg_printf("TKMM_MMList_AllocBrk A %d\n", sz);
		ptr=TKMM_PageAlloc(sz);
		
		if(!ptr)
		{
//			tk_puts("TKMM_MMList_AllocBrk A: Fail\n");
			return(NULL);
		}

//		tk_dbg_printf("TKMM_MMList_AllocBrk A-1 %p..%p\n", ptr, ptr+sz);
		
		memset(ptr, 0, sz);

		TKMM_MMList_AddVrmBrk(ptr, sz, 1);

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
		memset(tkmm_mmlist_brkbuf, 0, 1<<TKMM_BRKBITS);

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

		TKMM_MMList_AddVrmBrk(tkmm_mmlist_brkbuf, 1<<TKMM_BRKBITS, 0);
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
#endif

// #ifndef __TK_CLIB_ONLY__
#if 1

void TKMM_MMList_MProtectCat(byte *ptr, int sz, int cat)
{
	if(!ptr)
		return;

#ifdef __TK_CLIB_ONLY__
	if(!cat)
	{
		tk_mprotect(ptr, sz,
			TKMM_PROT_READ|TKMM_PROT_WRITE);
		return;
	}
	if(cat==4)
	{
		tk_mprotect(ptr, sz,
			TKMM_PROT_READ|TKMM_PROT_WRITE|TKMM_PROT_EXEC);
		return;
	}
	return;
#else
	if(!cat)
	{
		if(tk_iskernel())
		{
			TK_VMem_MProtectPages((u64)ptr, sz,
				TKMM_PROT_NOUSER|TKMM_PROT_READ|TKMM_PROT_WRITE);
		}else
		{
//			tk_mprotect((u64)ptr, sz,
//				TKMM_PROT_READ|TKMM_PROT_WRITE);
		}
		return;
	}
	if(cat==4)
	{
		if(tk_iskernel())
		{
			TK_VMem_MProtectPages((u64)ptr, sz,
				TKMM_PROT_NOUSER|
				TKMM_PROT_READ|TKMM_PROT_WRITE|
				TKMM_PROT_EXEC);
		}else
		{
			tk_mprotect((void *)ptr, sz,
				TKMM_PROT_READ|TKMM_PROT_WRITE|TKMM_PROT_EXEC);
		}
		return;
	}
#endif

#ifndef __TK_CLIB_ONLY__
	if(cat==1)
	{
		TK_VMem_MProtectPages((u64)ptr, sz, TKMM_PROT_USR_RO);
		return;
	}
	if(cat==2)
	{
		TK_VMem_MProtectPages((u64)ptr, sz,
			TKMM_PROT_NOUSER|TKMM_PROT_READ|TKMM_PROT_WRITE);
		return;
	}
	if(cat==3)
	{
		TK_VMem_MProtectPages((u64)ptr, sz,
			TKMM_PROT_READ|TKMM_PROT_WRITE);
		return;
	}
#endif
}

void *TKMM_MMList_AllocBrkCat(int sz, int cat)
{
	TKMM_MemLnkSeg *seg;
	byte *ptr, *brkbuf, *brkpos, *brkend;
	int i;

	if(cat&(~7))
		__debugbreak();

	if(sz>=TKMM_MAXMMLISTSZ)
	{
		if(cat==TKMM_MCAT_PHYSDFL)
		{
#ifndef __TK_CLIB_ONLY__
			if(tk_iskernel())
			{
				ptr=TKMM_PageAlloc(sz+256);
			}else
			{
				ptr=tk_mmap(NULL, sz+256, TKMM_PROT_RWX,
					TKMM_MAP_PHYSICAL|TKMM_MAP_PRIVATE|TKMM_MAP_ANONYMOUS,
					-1, 0);
			}
#else
			ptr=tk_mmap(NULL, sz+256, TKMM_PROT_RWX,
				TKMM_MAP_PHYSICAL|TKMM_MAP_PRIVATE|TKMM_MAP_ANONYMOUS,
				-1, 0);
#endif

			if(ptr)
			{
				memset(ptr, 0, sz);
				return(ptr);
			}
		}
	
		if(cat==TKMM_MCAT_GLOBAL)
		{
			ptr=tk_mmap(NULL, sz+256, TKMM_PROT_RWX,
				TKMM_MAP_GLOBAL|TKMM_MAP_SHARED|TKMM_MAP_ANONYMOUS,
				-1, 0);

			if(ptr)
			{
				memset(ptr, 0, sz);
				return(ptr);
			}
		}
	
//		tk_dbg_printf("TKMM_MMList_AllocBrk A, cat=%d\n", cat);

//		if((cat==0) || (cat==4))
		if(cat==0)
			ptr=TKMM_PageAllocUsc(sz+256);
		else
			ptr=TKMM_PageAlloc(sz+256);

		if(!ptr)
		{
			tk_puts("TKMM_MMList_AllocBrkCat Fail\n");
			return(NULL);
		}

		TKMM_MMList_MProtectCat(ptr, sz, cat);
		
		memset(ptr, 0, sz);

		TKMM_MMList_AddVrmBrk(ptr, sz, 1);

		return(ptr);
	}


	if(tkmm_mmlist_brkpos_init!=0x1234)
	{
		for(i=0; i<8; i++)
		{
			brkbuf=tkmm_mmlist_brkbuf_c[i];
			brkpos=tkmm_mmlist_brkpos_c[i];
			brkend=tkmm_mmlist_brkend_c[i];
//			__debugbreak();
			if(brkbuf || brkpos || brkend)
			{
//				__debugbreak();
				tk_dbg_printf("TKMM_MMList_AllocBrkCat: Non-Zero init "
						"%d %016llX %016llX %016llX\n",
					i, brkbuf, brkpos, brkend);
			}
//			__debugbreak();

			tkmm_mmlist_brkbuf_c[i]=NULL;
			tkmm_mmlist_brkpos_c[i]=NULL;
			tkmm_mmlist_brkend_c[i]=NULL;
		}
	
		tkmm_mmlist_brkpos_init=0x1234;
	}

//	sz=(sz+15)&(~15);
	sz=(sz+31)&(~15);
	
	brkbuf=tkmm_mmlist_brkbuf_c[cat];
	brkpos=tkmm_mmlist_brkpos_c[cat];
	brkend=tkmm_mmlist_brkend_c[cat];

	if(((long)(brkpos))&15)
		__debugbreak();
	
	if(brkbuf &&
		((brkpos+sz)>brkend))
	{
		tkmm_mmlist_brkbuf_c[cat]=NULL;
		brkbuf=NULL;
	}
	
	if(!brkbuf)
	{
		tk_dbg_printf("TKMM_MMList_AllocBrk C, cat=%d\n", cat);
//		if((cat==0) || (cat==4))
		if(cat==0)
			{ ptr=TKMM_PageAllocUsc((1<<TKMM_BRKBITS)+256); }
		else
			if(cat==TKMM_MCAT_PHYSDFL)
		{
#ifndef __TK_CLIB_ONLY__
			if(tk_iskernel())
			{
				ptr=TKMM_PageAlloc((1<<TKMM_BRKBITS)+256);
			}else
			{
				ptr=tk_mmap(NULL, (1<<TKMM_BRKBITS)+256, TKMM_PROT_RWX,
					TKMM_MAP_PHYSICAL|TKMM_MAP_PRIVATE|TKMM_MAP_ANONYMOUS,
					-1, 0);
			}
#else
			ptr=tk_mmap(NULL, (1<<TKMM_BRKBITS)+256, TKMM_PROT_RWX,
				TKMM_MAP_PHYSICAL|TKMM_MAP_PRIVATE|TKMM_MAP_ANONYMOUS,
				-1, 0);
#endif
		}else
			if(cat==TKMM_MCAT_GLOBAL)
		{
			ptr=tk_mmap(NULL, (1<<TKMM_BRKBITS)+256, TKMM_PROT_RWX,
				TKMM_MAP_GLOBAL|TKMM_MAP_SHARED|TKMM_MAP_ANONYMOUS,
				-1, 0);
		}
		else
		{
			ptr=TKMM_PageAlloc((1<<TKMM_BRKBITS)+256);
		}
		
		if(!ptr)
		{
			ptr=TKMM_PageAlloc((1<<TKMM_BRKBITS)+256);
		}

		tkmm_mmlist_brkbuf_c[cat]=ptr;
		brkbuf=ptr;

		if(!brkbuf)
		{
			tk_puts("TKMM_MMList_AllocBrk C: Fail\n");
			return(NULL);
		}

		if(((long)(brkbuf))&15)
			__debugbreak();
		
		TKMM_MMList_MProtectCat(
			brkbuf,
			1<<TKMM_BRKBITS,
			cat);
		
		memset(brkbuf, 0, 1<<TKMM_BRKBITS);

//		tkmm_mmlist_brkend_c[cat]=tkmm_mmlist_brkbuf_c[cat]+(1<<TKMM_BRKBITS);
//		tkmm_mmlist_brkend_c[cat]=tkmm_mmlist_brkbuf_c[cat]+
//			(1<<TKMM_BRKBITS)-4096;

		brkend=brkbuf+(1<<TKMM_BRKBITS)-4096;
		tkmm_mmlist_brkend_c[cat]=brkend;

		seg=(TKMM_MemLnkSeg *)(brkbuf);
		i=(byte *)(seg->data)-(brkbuf);
//		i=(i+15)&(~15);
		i=(i+31)&(~15);
		brkpos=brkbuf+i;
		tkmm_mmlist_brkpos_c[cat]=brkpos;
		
		if(!i)
			__debugbreak();
		
		if((long)(brkpos)&15)
			__debugbreak();
		
		seg->nblk=0;

		TKMM_MMList_AddVrmBrk(brkbuf, 1<<TKMM_BRKBITS, 0);
	}

//	tk_puts("TKMM_MMList_AllocBrk D\n");

	seg=(TKMM_MemLnkSeg *)(brkbuf);
	
	ptr=brkpos;
	brkpos=ptr+sz;
	tkmm_mmlist_brkpos_c[cat]=brkpos;

	if(((long)(ptr))&15)
		__debugbreak();

	if(((long)(brkpos))&15)
		__debugbreak();

#if 1
	i=seg->nblk++;
	if(i>=TKMM_MAXMMSEGBLK)
		{ __debugbreak(); }
//	seg->blk[i]=ptr;
	seg->blk[i]=((byte *)ptr)-((byte *)seg);
#endif

//	i=tkmm_mmlist_brkpos_c[cat]-ptr;
//	if(i<sz)
//		__debugbreak();

	return(ptr);
}

void *TKMM_MMList_AllocBrk(int sz)
{
	return(TKMM_MMList_AllocBrkCat(sz, 0));
}


#endif

#if 0
void *TKMM_MMList_AllocBrkCat(int sz, int cat)
{
	if(cat==0)
		return(TKMM_MMList_AllocBrk(sz));
#ifndef __TK_CLIB_ONLY__
	if(cat==1)
		return(TKMM_MMList_AllocBrkURo(sz));
#endif
	return(NULL);
}
#endif

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

TKMM_MemLnkObj *TKMM_MMList_AllocObjCat(int sz, int cat)
{
	TKMM_MemLnkObj *obj, *obj1;
	TKMM_MemLnkObj **freelist;
	byte *p1, *p2;
	int ix, ix1;
	int isz, sz1, sz2;

	if(!TKMM_PageAlloc_f)
	{
		TKMM_Init();

		if(!TKMM_PageAlloc_f)
			__debugbreak();
	}

	isz=sz;

	if(sz<=0)return(NULL);
//	if(sz<256)sz=256;
	if(sz<64)sz=64;
	
//	putc('<');
//	sz=(sz+TKMM_OFFS_DATA+7)&(~7);
//	sz=(sz+TKMM_OFFS_DATA+15)&(~7);
//	sz=(sz+TKMM_OFFS_DATA+31)&(~7);
//	sz=(sz+TKMM_OFFS_DATA+31)&(~15);
//	sz2=(sz+sizeof(TKMM_MemLnkObj)+31)&(~15);
	sz2=(sz+sizeof(TKMM_MemLnkObj)+15)&(~15);
	sz1=sz+sizeof(TKMM_MemLnkObj);

	if(sz2<sz1)
		__debugbreak();
	if(sz1<sz)
		__debugbreak();
	
	sz=sz2;

	if(!TKMM_OFFS_DATA)
		__debugbreak();
	if(sz&15)
		__debugbreak();

	if(TKMM_OFFS_DATA&15)
		__debugbreak();

	ix=TKMM_SizeToFxiU(sz);
//	tk_dbg_printf("AllocObjCat ix=%d cat=%d\n", ix, cat);

	if(ix&(~255))
		__debugbreak();
		
	if(cat&(~7))
		__debugbreak();

//	freelist=tkmm_mmlist_freelist;
//	freelist=tkmm_mmlist_freelist_cat[cat];
//	freelist=tkmm_mmlist_freelist_cat+(cat*256);
	freelist=tkmm_mmlist_freelist_cat+(cat<<8);
//	if(cat==1)
//		freelist=tkmm_mmlist_freelist_uro;

//	obj=tkmm_mmlist_freelist[ix];
	obj=freelist[ix];
	if(obj)
	{
		if(((long)obj)&15)
			__debugbreak();
	
		if(obj->check!=0x5A)
			__debugbreak();

		if(obj->ix!=ix)
			__debugbreak();
	
//		putc('A');
		freelist[ix]=*(TKMM_MemLnkObj **)(obj->data);
//		obj->ix=0;
		obj->fl&=~1;
//		putc('>');
//		return((byte *)obj->data);
		return(obj);
	}

//	putc('B');

#if 1
	sz1=TKMM_FxiToSize(ix);
	
	if(sz1<sz)
		__debugbreak();
	
//	p1=TKMM_MMList_AllocBrk(sz1);
	p1=TKMM_MMList_AllocBrkCat(sz1, cat);

	if(!p1)
	{
		tk_printf("TKMM_MMList_AllocObjCat: BRK Failed %d\n", sz1);
		__debugbreak();
		return(NULL);
	}

	if(((long)obj)&15)
		__debugbreak();

	p2=p1+sz1;
	obj=(TKMM_MemLnkObj *)p1;
//	sz2=p2-((byte *)obj1->data);
//	sz2=p2-((byte *)obj->data);
//	obj->ix=TKMM_SizeToFxiD(sz2);
	obj->ix=ix;
	obj->fl=3;
	obj->check=0x5A;
	obj->cat=cat;
//	obj->cnext=(TKMM_MemLnkObj *)p2;
//	obj->cnext=NULL;
//	obj->cprev=NULL;

	obj->fl&=~1;

//	__setmemtrap(obj, 3);

//	tk_dbg_printf("TKMM_MMList_AllocObjCat: Data=%p..%p, isz=%d, sz1=%d\n",
//		(byte *)obj->data, ((byte *)obj->data)+isz, isz, sz1);

//	return((byte *)obj->data);
	return(obj);
#endif
}

TKMM_MemLnkObj *TKMM_MMList_AllocObj(int sz)
{
	return(TKMM_MMList_AllocObjCat(sz, 0));
}

void *TKMM_MMList_Malloc(int sz)
{
	TKMM_MemLnkObj *obj;
	void *ptr;
	obj=TKMM_MMList_AllocObjCat(sz, 0);
	ptr=(byte *)obj->data;
	__setmemtrap(obj, 3);
	
	if(((void *)obj)==ptr)
		__debugbreak();
	
	return(ptr);
}

void *TKMM_MMList_MallocURo(int sz)
{
	TKMM_MemLnkObj *obj;
	void *ptr;
	obj=TKMM_MMList_AllocObjCat(sz, 1);
	ptr=(byte *)obj->data;
	__setmemtrap(obj, 3);
	if(((void *)obj)==ptr)
		__debugbreak();
	return(ptr);
}

void *TKMM_MMList_MallocCat(int sz, int cat)
{
	TKMM_MemLnkObj *obj;
	void *ptr;
	obj=TKMM_MMList_AllocObjCat(sz, cat);
	ptr=(byte *)obj->data;
	__setmemtrap(obj, 3);
	if(((void *)obj)==ptr)
		__debugbreak();
	return(ptr);
}


int TKMM_MMList_FreeLnkObj(TKMM_MemLnkObj *obj)
{
	TKMM_MemLnkObj **freelist;

	__setmemtrap(obj, 2);

	if(obj->check!=0x5A)
	{
		tk_puts("TKMM_MMList_FreeLnkObj: Check Value Fail\n");
		__debugbreak();
		return(-1);
	}

	if((obj->ix)&(~255))
	{
		__debugbreak();
		tk_puts("TKMM_MMList_FreeLnkObj: List Index Fail\n");
		return(-1);
	}

	if((obj->cat)&(~7))
		__debugbreak();

//	freelist=tkmm_mmlist_freelist;
//	freelist=tkmm_mmlist_freelist_cat[obj->cat];
//	freelist=tkmm_mmlist_freelist_cat+(obj->cat*256);
	freelist=tkmm_mmlist_freelist_cat+(obj->cat<<8);
//	if(obj->cat==1)
//		freelist=tkmm_mmlist_freelist_uro;

	obj->fl|=1;
//	*(TKMM_MemLnkObj **)(obj->data)=tkmm_mmlist_freelist[obj->ix];
//	tkmm_mmlist_freelist[obj->ix]=obj;
	*(TKMM_MemLnkObj **)(obj->data)=freelist[obj->ix];
	freelist[obj->ix]=obj;
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
	int i, j;

//	for(i=0; i<256; i++)
//		tkmm_mmlist_freelist[i]=NULL;

	for(j=0; j<8; j++)
		for(i=0; i<256; i++)
	{
//		tkmm_mmlist_freelist_cat[j][i]=NULL;
//		tkmm_mmlist_freelist_cat[j*256+i]=NULL;
		tkmm_mmlist_freelist_cat[(j<<8)+i]=NULL;
	}
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

int TKMM_MMList_WalkHeapObjects(
	void *ctx, int (*func)(void *ctx, TKMM_MemLnkObj *obj))
{
	TKMM_MemLnkSeg *seg;
	TKMM_MemLnkObj *obj;
	byte *vss, *vse;
	byte *p;
	int i, j, k, vrn, nb;
	
//	p=ptr;
	vrn=tkmm_mmlist_n_vrm;
	for(i=0; i<vrn; i++)
	{
		j=tkmm_mmlist_vrm_brkflg[i];
		if(j&1)
		{
			obj=(TKMM_MemLnkObj *)(tkmm_mmlist_vrm_brkbuf[i]);
			func(ctx, obj);
			continue;
		}

		seg=(TKMM_MemLnkSeg *)(tkmm_mmlist_vrm_brkbuf[i]);

		nb=seg->nblk;
		for(j=0; j<nb; j++)
		{
			obj=(TKMM_MemLnkObj *)(((byte *)seg)+seg->blk[j]);
			func(ctx, obj);

//			sz1=TKMM_FxiToSize(obj->ix);
//			vss=(byte *)obj;
//			vse=((byte *)(obj->data))+sz1;
			
//			if((p>=vss) && (p<vse))
//				return(obj);
		}

//		vss=tkmm_mmlist_vrm_brkbuf[i];
//		vse=tkmm_mmlist_vrm_brkend[i];
//		if((p>=vss) && (p<vse))
//			return(i);
	}
	
	return(-1);
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
	return(obj->ttag);
}


int TKMM_MMList_SetTag(void *ptr, int tag)
{
	TKMM_MemLnkObj *obj;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
	__setmemtrap(obj, 2);
	obj->ttag=tag;
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

void *tk_malloc_cat(int sz, int cat)
{
// #ifndef __TK_CLIB_ONLY__
#if 1
	if(cat)
		return(TKMM_MallocCat(sz, cat));
	else
		return(TKMM_Malloc(sz));
#else
//	return(TKMM_MallocCat(sz, cat));
	return(TKMM_Malloc(sz));
#endif
}

void *tk_malloc_krn(int sz)
{
	void *ptr;
	ptr=TKMM_MallocCat(sz, TKMM_MCAT_KRN_RW);
	ptr=tk_ptrsetbound1(ptr, sz);
	return(ptr);
}

void *tk_malloc_usr(int sz)
{
	void *ptr;
	ptr=TKMM_MallocCat(sz, TKMM_MCAT_USR_RW);
	ptr=tk_ptrsetbound1(ptr, sz);
	return(ptr);

//	return(TKMM_MallocCat(sz, TKMM_MCAT_USR_RW));
}

void *tk_malloc_wxe(int sz)
{
//	if(!TKMM_PageAlloc_f)
//		__debugbreak();

//	return(TKMM_MMList_Malloc(sz));
//	return(TKMM_Malloc(sz));
	return(TKMM_MallocCat(sz, TKMM_MCAT_USR_RWX));
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

int TKMM_IncRef(void *ptr);
int TKMM_DecRef(void *ptr);

int tk_mincref(void *ptr)
{
	return(TKMM_IncRef(ptr));
}

int tk_mdecref(void *ptr)
{
	return(TKMM_DecRef(ptr));
}

int tk_mgettag(void *ptr)
{
	return(TKMM_GetTag(ptr));
}

int tk_msettag(void *ptr, int tag)
{
	return(TKMM_SetTag(ptr, tag));
}

int tk_mgetzone(void *ptr)
{
	return(TKMM_GetZoneTag(ptr));
}

int tk_msetzone(void *ptr, int tag)
{
	return(TKMM_SetZoneTag(ptr, tag));
}

void *tk_mgetbase(void *ptr)
{
	return(TKMM_GetBase(ptr));
}

int tk_mfreezone(int ztag, int zmask)
{
	return(TKMM_FreeZone(ztag, zmask));
}

char *tk_strdup_in(char *str)
{
	return(TKMM_LVA_Strdup(str));
}
TKMM_MemLnkObj *tkmm_mmlist_freelist[256];

byte *tkmm_mmlist_brkbuf=NULL;
byte *tkmm_mmlist_brkend;
byte *tkmm_mmlist_brkpos;

void *TKMM_MMList_AllocBrk(int sz)
{
	byte *ptr;

	if(sz>=65536)
	{
		tk_puts("TKMM_MMList_AllocBrk A\n");
		ptr=TKMM_PageAlloc(sz);
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
		tkmm_mmlist_brkbuf=TKMM_PageAlloc(1<<20);
		tkmm_mmlist_brkend=tkmm_mmlist_brkbuf+(1<<20);
		tkmm_mmlist_brkpos=tkmm_mmlist_brkbuf;
	}

	tk_puts("TKMM_MMList_AllocBrk D\n");
	
	ptr=tkmm_mmlist_brkpos;
	tkmm_mmlist_brkpos=ptr+sz;
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
	return(i);
}

/** Convert Fxi to size */
int TKMM_FxiToSize(int ix)
{
	int ex, fr;
	int sz;
	
	if(ix<8)return(ix);
	fr=(ix&7)|8; ex=ix>>3;
	sz=fr<<ex;
	return(sz);
}

void *TKMM_MMList_Malloc(int sz)
{
	TKMM_MemLnkObj *obj, *obj1;
	byte *p1, *p2;
	int ix, ix1;
	int sz1, sz2;

	if(sz<=0)return(NULL);
//	if(sz<256)sz=256;
	if(sz<64)sz=64;
	
//	putc('<');
//	sz=(sz+TKMM_OFFS_DATA+7)&(~7);
	sz=(sz+TKMM_OFFS_DATA+15)&(~7);

	ix=TKMM_SizeToFxiU(sz);
//	printf("%d", ix);
	obj=tkmm_mmlist_freelist[ix];
	if(obj)
	{
//		putc('A');
		tkmm_mmlist_freelist[ix]=*(TKMM_MemLnkObj **)(obj->data);
//		obj->ix=0;
		obj->fl&=~1;
//		putc('>');
		return((byte *)obj->data);
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
	sz2=p2-((byte *)obj1->data);
	obj->ix=TKMM_SizeToFxiD(sz2);
	obj->fl=3;
	obj->check=0x5A;
	obj->cnext=(TKMM_MemLnkObj *)p2;
	obj->cprev=NULL;

	obj->fl&=~1;
	return((byte *)obj->data);
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

int TKMM_MMList_FreeLnkObj(TKMM_MemLnkObj *obj)
{
	if(obj->check!=0x5A)
	{
		tk_puts("TKMM_MMList_FreeLnkObj: Check Value Fail\n");
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
	obj->fl|=1;
	
	TKMM_MMList_FreeLnkObj(obj);
}

int TKMM_MMList_Init(void)
{
	int i;

	for(i=0; i<256; i++)
		tkmm_mmlist_freelist[i]=NULL;
}

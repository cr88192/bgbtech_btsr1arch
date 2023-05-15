TKMM_MemLnkObj *TKMM_MMList_AllocObj(int sz);

TKMM_MemCelChk	*tkmm_mmcell_chunk[1024];
int				tkmm_mmcell_nchunk=0;

int TKMM_MMCell_AllocNewChunk(void)
{
	TKMM_MemLnkObj *obj;
	TKMM_MemCelChk *chk;
	int i, n;

//	chk=TKMM_Malloc(sizeof(TKMM_MemCelChk));

//	tk_printf("TKMM_MMCell_AllocNewChunk: sz=%d\n", sizeof(TKMM_MemCelChk));

	obj=TKMM_MMList_AllocObj(sizeof(TKMM_MemCelChk));
	chk=(TKMM_MemCelChk *)(obj->data);
	obj->fl|=8;
	
	memset(chk, 0, sizeof(TKMM_MemCelChk));
	
	n=TKMM_MAXMMCELLSZ/8;
	for(i=0; i<n; i++)
		chk->freelst[i]=-1;
	
	chk->magic1=0x12345678;
	chk->magic2=0x12345678;
	chk->magic3=0x12345678;
	
	i=tkmm_mmcell_nchunk++;
	tkmm_mmcell_chunk[i]=chk;
	return(i);
}

u32 TKMM_MMCell_GetChunkBmpBits(TKMM_MemCelChk *chk, int pos)
{
	u64 lv;
	u32 v0, v1, v;
	int pb, pf;
	
	pb=pos>>4; pf=pos&15;
	v0=chk->bmp[pb+0];
	v1=chk->bmp[pb+1];
	lv=(((u64)v1)<<32)|v0;
	v=(u32)(lv>>(pf*2));
	return(v);
}

int TKMM_MMCell_GetChunkBmpIndex(TKMM_MemCelChk *chk, int pos)
{
	u32 v;
	int pb, pf;
	
	pb=pos>>4; pf=pos&15;
	v=chk->bmp[pb];
	v=(v>>(pf*2))&3;
	return(v);
}

int TKMM_MMCell_SetChunkBmpIndex(TKMM_MemCelChk *chk, int pos, int val)
{
	u32 v;
	int pb, pf;
	
	pb=pos>>4; pf=pos&15;
	v=chk->bmp[pb];
	v&=~(  3<<(pf*2));
	v|= (val<<(pf*2));
	chk->bmp[pb]=v;
	return(0);
}

int TKMM_MMCell_ChunkFindFreeSpan(TKMM_MemCelChk *chk, int pos, int len)
{
	u32 v, v1;
	int p, pe, p1;
	
	p=pos;
	while(p<TKMM_MAXMMCELLS)
	{
		v=TKMM_MMCell_GetChunkBmpBits(chk, p);
		if(v&3)
		{
			if(v==0xFFFFFFFFU)
				{ p+=16; continue; }
			if(((u16)v)==0xFFFF)
				{ p+=8; continue; }
			if(((byte)v)==0xFF)
				{ p+=4; continue; }
			p++;
		}else
		{
			pe=p+len;
			if(pe>TKMM_MAXMMCELLS)
				break;
			p1=p+1;
			while(p1<pe)
			{
				v1=TKMM_MMCell_GetChunkBmpBits(chk, p1);
				if(!v1)
					{ p1+=16; continue; }
				if(!((u16)v1))
					{ p1+=8; continue; }
				if(!((byte)v1))
					{ p1+=4; continue; }
				if(v1&3)
					break;
				p1++;
			}
			if(p1>=pe)
			{
				return(p);
			}
			p=p1;
		}
	}
	return(-1);
}

int TKMM_MMCell_ChunkSetSpanUsed(TKMM_MemCelChk *chk, int pos, int len)
{
	int i;
	
	for(i=0; i<len; i++)
		TKMM_MMCell_SetChunkBmpIndex(chk, pos+i, 3);
	TKMM_MMCell_SetChunkBmpIndex(chk, pos, 2);
	return(0);
}


int TKMM_MMCell_ChunkSetSpanFree(TKMM_MemCelChk *chk, int pos, int len)
{
	int i;
	
	for(i=0; i<len; i++)
		TKMM_MMCell_SetChunkBmpIndex(chk, pos+i, 0);
	return(0);
}

int TKMM_MMCell_ChunkSetObjFree(TKMM_MemCelChk *chk, int pos)
{
	int p, v;

	p=pos;

	v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
	if(v!=2)
		return(-1);
	TKMM_MMCell_SetChunkBmpIndex(chk, p, 0);
	p++;
	
	while(p<TKMM_MAXMMCELLS)
	{
		v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
		if(v!=3)break;
		TKMM_MMCell_SetChunkBmpIndex(chk, p, 0);
		p++;
	}
	return(0);
}

int TKMM_MMCell_ChunkSetObjListFree(TKMM_MemCelChk *chk, int pos)
{
	int p, n;

	while(p>=0)
	{
		n=(s16)(chk->data[p]);
		TKMM_MMCell_ChunkSetObjFree(chk, p);
		p=n;
	}
	return(0);
}

int TKMM_MMCell_ChunkFindObjBase(TKMM_MemCelChk *chk, int pos)
{
	int p, v;

	p=pos;

	v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
	while((p>0) && (v==3))
	{
		p--;
		v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
	}
	return(p);
}

int TKMM_MMCell_ChunkGetObjLen(TKMM_MemCelChk *chk, int pos)
{
	int p, v;

	p=pos;
	v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
	if((v!=2) && (v!=1))
		return(-1);
	p++;	
	while(p<TKMM_MAXMMCELLS)
	{
		v=TKMM_MMCell_GetChunkBmpIndex(chk, p);
		if(v!=3)break;
		p++;
	}
	return(p-pos);
}

int TKMM_MMCell_ChunkTryAllocSpanFlO(TKMM_MemCelChk *chk, int len)
{
	int p, b;
	int i, n;
	
	b=chk->freelst[len];
	if(b>=0)
	{
		n=(s16)(chk->data[b]);
		chk->freelst[len]=n;
		return(b);
	}
	return(-1);
}

int TKMM_MMCell_ChunkTryAllocSpan(TKMM_MemCelChk *chk, int len)
{
	int p, b;
	int i, n;
	
	b=chk->freelst[len];
	if(b>=0)
	{
		n=(s16)(chk->data[b]);
		chk->freelst[len]=n;
		return(b);
	}
	
	p=chk->rov;
	b=TKMM_MMCell_ChunkFindFreeSpan(chk, p, len);
	if(b>=0)
	{
		TKMM_MMCell_ChunkSetSpanUsed(chk, b, len);
		chk->rov=b+len;
		return(b);
	}

	n=TKMM_MAXMMCELLSZ/8;
	for(i=0; i<n; i++)
	{
		TKMM_MMCell_ChunkSetObjListFree(chk, chk->freelst[i]);
		chk->freelst[i]=-1;
	}

	b=TKMM_MMCell_ChunkFindFreeSpan(chk, 0, len);
	if(b>=0)
	{
		TKMM_MMCell_ChunkSetSpanUsed(chk, b, len);
		chk->rov=b+len;
		return(b);
	}
	
	return(-1);
}

void *TKMM_MMCell_Malloc(int sz)
{
	TKMM_MemCelChk *chk;
	int b;
	int i, n;
	
//	n=(sz+7)/8;
//	n=(sz+15)/8;
//	n=(sz+23)/8;
	n=((sz+47)/16)*2;
	
//	n*=2;
//	n+=2;

	for(i=0; i<tkmm_mmcell_nchunk; i++)
	{
		chk=tkmm_mmcell_chunk[i];
		b=TKMM_MMCell_ChunkTryAllocSpanFlO(chk, n);
		if(b>=0)
//			{ return(chk->data+b+1); }
			{ return(chk->data+b+2); }
	}
	
	for(i=0; i<tkmm_mmcell_nchunk; i++)
	{
		chk=tkmm_mmcell_chunk[i];
		b=TKMM_MMCell_ChunkTryAllocSpan(chk, n);
		if(b>=0)
//			{ return(chk->data+b+1); }
			{ return(chk->data+b+2); }
	}
	
	i=TKMM_MMCell_AllocNewChunk();

	chk=tkmm_mmcell_chunk[i];
	b=TKMM_MMCell_ChunkTryAllocSpan(chk, n);
	if(b>=0)
//		{ return(chk->data+b+1); }
		{ return(chk->data+b+2); }
	
	__debugbreak();
}

int TKMM_MMCell_FreeLnkObjCellPtr(TKMM_MemLnkObj *obj, void *ptr)
{
	TKMM_MemCelChk *chk;
	int ofs, p, v, l;

	chk=(TKMM_MemCelChk *)(obj->data);
	ofs=((byte *)ptr)-((byte *)(chk->data));
	p=ofs>>3;

	if(chk->magic1!=0x12345678)
		__debugbreak();
	if(chk->magic2!=0x12345678)
		__debugbreak();
	if(chk->magic3!=0x12345678)
		__debugbreak();

	p=TKMM_MMCell_ChunkFindObjBase(chk, p);
	if(p<0)
		return(-1);
	l=TKMM_MMCell_ChunkGetObjLen(chk, p);
	if(l<0)
		return(-1);
	
	chk->data[p]=chk->freelst[l];
	chk->freelst[l]=p;
	return(1);
}


int TKMM_MMCell_GetLnkObjCellSize(TKMM_MemLnkObj *obj, void *ptr)
{
	TKMM_MemCelChk *chk;
	int ofs, p, v, l;

	chk=(TKMM_MemCelChk *)(obj->data);
	ofs=((byte *)ptr)-((byte *)(chk->data));
	p=ofs>>3;

	if(chk->magic1!=0x12345678)
		__debugbreak();
	if(chk->magic2!=0x12345678)
		__debugbreak();
	if(chk->magic3!=0x12345678)
		__debugbreak();

	p=TKMM_MMCell_ChunkFindObjBase(chk, p);
	if(p<0)
		return(-1);
	l=TKMM_MMCell_ChunkGetObjLen(chk, p);
	if(l<0)
		return(-1);
	
//	return(l<<3);
	return((l<<3)-8);
}

u64 *TKMM_MMCell_GetLnkObjCellHeadPtr(TKMM_MemLnkObj *obj, void *ptr)
{
	TKMM_MemCelChk *chk;
	int ofs, p, v, l;

	chk=(TKMM_MemCelChk *)(obj->data);
	ofs=((byte *)ptr)-((byte *)(chk->data));
	p=ofs>>3;

	if(chk->magic1!=0x12345678)
		__debugbreak();
	if(chk->magic2!=0x12345678)
		__debugbreak();
	if(chk->magic3!=0x12345678)
		__debugbreak();

	p=TKMM_MMCell_ChunkFindObjBase(chk, p);
	if(p<0)
		return(-1);
	return(chk->data+p);
}

/** Convert size into a 5.3 minifloat, Round Up */
int TKMM_SizeToFxiBnd(int sz)
{
	int fr, ex;
	int i;
	
	if(sz<16)
		return(sz);
	
	fr=sz;
	ex=1;
	while(fr>=16)
		{ fr=(fr+1)>>1; ex++; }
	i=(fr&7)|(ex<<3);

	if(i&(~255))
		__debugbreak();

	return(i);
}


void *tk_ptrsetbound1(void *ptr, int size);

void *tk_ptrsetbound2(void *ptr, int lobnd, int hibnd)
{
	int sz1;
	void *p;
	u64 v;
	int ix, b;

	if(lobnd>=0)
	{
		return(tk_ptrsetbound1(ptr, hibnd));
	}

	sz1=hibnd-lobnd;

	if(!ptr)
		return(ptr);

	v=(long)ptr;
	if(v>>48)
		return(ptr);

	ix=TKMM_SizeToFxiBnd(sz1);
	b=(-lobnd)>>((((ix+3)>>3)&31)-1);
	ix=0x3000|((ix+3)+((b+1)<<8));

//	v=(long)ptr;
	v=(v&0x0000FFFFFFFFFFFFULL)|(((u64)ix)<<48);
	p=(void *)v;
	return(p);
}

void *tk_ptrsetbound1(void *ptr, int size)
{
	void *p;
	u64 v;
	int ix;

	if(!ptr)
		return(ptr);

	v=(long)ptr;
	if(v>>48)
		return(ptr);

	ix=TKMM_SizeToFxiBnd(size);
	ix=0x3000|((ix+3)+(1<<8));
	
//	v=(long)ptr;
	v=(v&0x0000FFFFFFFFFFFFULL)|(((u64)ix)<<48);
	p=(void *)v;
	return(p);
}

void *tk_ptrstriptag(void *ptr)
{
	void *p;
	u64 v;

	if(!ptr)
		return(ptr);

	v=(long)ptr;
	v=(v&0x0000FFFFFFFFFFFFULL);
	p=(void *)v;
	return(p);
}

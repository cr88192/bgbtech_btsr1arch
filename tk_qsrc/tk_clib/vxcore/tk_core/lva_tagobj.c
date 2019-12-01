/*

High 4 Bits: 0 (Object Pointer)

Object Pointer:
  Low 48 bits: Address
  Bits 48-59: Type ID

Type ID:
     0..1023: Primitive/Named Object Types
  1024..2047: Reserved
  2048..4095: Dynamic Class/Interface Types

 */



LVA_TagInfo *tkmm_lva_taghash[1024];

void TKMM_LVA_TagInit(void)
{
	LVA_TagInfo *inf;

	if(!tkmm_lva_taghash[0])
	{
		inf=TKMM_MMList_Malloc(sizeof(LVA_TagInfo));
		inf->name=NULL;
		inf->idx=0;
		tkmm_lva_taghash[0]=inf;
	}
}

LVA_TagInfo *TKMM_LVA_GetTagInfoForName(char *name)
{
	LVA_TagInfo *inf;
	int h, n;
	int i;
	
	h=TKMM_LVA_HashName(name);	n=1024;
	while(n--)
	{
		i=(h>>16)&1023;
		inf=tkmm_lva_taghash[i];
		if(!inf)
		{
			inf=TKMM_MMList_Malloc(sizeof(LVA_TagInfo));
			inf->name=TKMM_LVA_Strdup(name);
			inf->idx=i;
			tkmm_lva_taghash[i]=inf;
			return(inf);
		}
		
		if(!strcmp(inf->name, name))
			return(inf);
		h=h*65521+1;
	}
	return(NULL);
}

int TKMM_LVA_GetTagIndexForName(char *name)
{
	LVA_TagInfo *inf;
	
	inf=TKMM_LVA_GetTagInfoForName(name);
	return(inf->idx);
}

char *TKMM_LVA_GetTagNameForIndex(int idx)
{
	LVA_TagInfo *inf;
	
	inf=tkmm_lva_taghash[idx&1023];
	if(!inf)
		return(NULL);
	return(inf->name);
}

void *TKMM_LVA_TyMalloc(char *tyn, int sz)
{
	void *obj;
	int tyi;

	TKMM_LVA_TagInit();

	tyi=TKMM_LVA_GetTagIndexForName(tyn);
	obj=TKMM_MMList_Malloc(sz);
	TKMM_MMList_SetTag(obj, tyi);
	return(obj);
}

void *TKMM_LVA_TagMalloc(int tyi, int sz)
{
	void *obj;

	obj=TKMM_MMList_Malloc(sz);
	TKMM_MMList_SetTag(obj, tyi);
	return(obj);
}

void *TKMM_LVA_ReAlloc(void *ptr, int sz)
{
	void *obj;
	int sz1;

	if(!ptr)
	{
		obj=TKMM_MMList_Malloc(sz);
		return(obj);
	}
	
	sz1=TKMM_MMList_GetSize(ptr);
	if(sz<=sz1)
	{
		return(ptr);
	}
	
	obj=TKMM_MMList_Malloc(sz);
	memcpy(obj, ptr, sz1);
	TKMM_MMList_Free(ptr);
	
	return(obj);
}

char *TKMM_LVA_GetPtrTypeName(void *ptr)
{
	int tyi;
	
	if(!TKMM_MMList_CheckPtrIsHeap(ptr))
		return(NULL);

	tyi=TKMM_MMList_GetTag(ptr);
	return(TKMM_LVA_GetTagNameForIndex(ptr));
}

int TKMM_LVA_GetPtrTypeTag(void *ptr)
{
	int tyi;
	
	if(!TKMM_MMList_CheckPtrIsHeap(ptr))
		return(0);

	tyi=TKMM_MMList_GetTag(ptr);
	return(tyi);
}

int TKMM_LVA_ObjRefP(u64 val)
{
	if((val>>60)==0)
		return(1);
	return(0);
}

int TKMM_LVA_FixnumP(u64 val)
{
	if((val>>62)==1)
		return(1);
	return(0);
}

int TKMM_LVA_FlonumP(u64 val)
{
	if((val>>62)==2)
		return(1);
	return(0);
}

u64 TKMM_LVA_WrapFixnum(s64 val)
{
	u64 v;
	v=((s64)val);
	v&=0x3FFFFFFFFFFFFFFFULL;
	v|=0x4000000000000000ULL;
	return(v);
}

u64 TKMM_LVA_WrapFlonum(double val)
{
//	double f;
	u64 v;
	
//	f=val;
//	v=(*(u64 *)(&f));
	v=__float64_getbits(val);
	v=v>>2;
	v|=0x8000000000000000ULL;
	return(v);
}

s64 TKMM_LVA_UnwrapFixnum(u64 vv)
{
	return(((s64)(vv<<2))>>2);
}

double TKMM_LVA_UnwrapFlonum(u64 vv)
{
	u64 v;
	v=vv<<2;
	return(__float64_frombits(v));
}

int TKMM_LVA_GetObjTag(u64 val)
{
	return(((int)(val>>48))&4095);
}


u64 TKMM_LVA_WrapPointer(void *val)
{
	u64 v;

// #ifdef __BJX1_64__
#ifdef __ADDR_64__
	v=(u64)val;
	v=v<<16;
	v=v>>16;
	v|=TKMM_LVA_GetPtrTypeTag(val);
	return(v);
#else
	v=(u64)((u32)val);
	v|=TKMM_LVA_GetPtrTypeTag(val);
	return(v);
#endif
}

void *TKMM_LVA_UnwrapPointer(u64 val)
{
	return((void *)(val&LVA_POINTER_MASK));
}

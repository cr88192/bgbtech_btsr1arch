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



// LVA_TagInfo *tkmm_lva_taghash[1024];

LVA_TagInfo *tkmm_lva_tagarr[1024];
short tkmm_lva_taghash[64];
int tkmm_lva_ntag=0;

void TKMM_LVA_TagInit(void)
{
	LVA_TagInfo *inf;
	int i;

	if(tkmm_lva_ntag)
		return;

#if 0
	if(!tkmm_lva_taghash[0])
	{
		inf=TKMM_MMList_Malloc(sizeof(LVA_TagInfo));
		inf->name=NULL;
		inf->idx=0;
		tkmm_lva_taghash[0]=inf;
	}
#endif

	for(i=0; i<64; i++)
		tkmm_lva_taghash[i]=0;

	i=tkmm_lva_ntag++;

	inf=TKMM_MMList_Malloc(sizeof(LVA_TagInfo));
	inf->name=NULL;
	inf->idx=i;
	tkmm_lva_tagarr[i]=inf;
}

LVA_TagInfo *TKMM_LVA_GetTagInfoForName(char *name)
{
	LVA_TagInfo *inf;
	int h, c, n;
	int i;

	h=TKMM_LVA_HashName(name)&63;
	c=tkmm_lva_taghash[h];
	while(c>0)
	{
		inf=tkmm_lva_tagarr[c];
		if(!inf || !inf->name)
			break;
		if(!strcmp(inf->name, name))
			return(inf);
		c=inf->chain;
	}

	i=tkmm_lva_ntag++;
	inf=TKMM_MMList_Malloc(sizeof(LVA_TagInfo));
	inf->name=TKMM_LVA_Strdup(name);
	inf->idx=i;
	inf->chain=tkmm_lva_taghash[h];
	tkmm_lva_taghash[h]=i;
	tkmm_lva_tagarr[i]=inf;
	return(inf);

#if 0
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
#endif

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
	
	inf=tkmm_lva_tagarr[idx&1023];
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

int __lvo_fixnump(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>62)==1);
}

int __lvo_flonump(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>62)==2);
}

int __lvo_stringp(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>48)==LVA_LVATY_STRING);
}

int __lvo_wstringp(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>48)==LVA_LVATY_WSTRING);
}

int __lvo_tagarrp(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>48)==LVA_LVATY_TAGARRAY);
}

int __lvo_tagobjp(tk_lva_object obj)
{
	u64 objv;
	objv=__object_getbits(obj);
	return((objv>>48)==LVA_LVATY_TAGOBJ);
}


void *__lvo_alloc_wxe(int sz)
{
//	if(!TKMM_PageAlloc_f)
//		__debugbreak();

//	return(TKMM_MMList_Malloc(sz));
	return(TKMM_Malloc(sz));
}

void TK_FlushCacheL1D_INVDC(void *ptr);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);

void *__snipe_dc(void *addr);
void *__snipe_ic(void *addr);

void *__lvo_makelambda(void *obj, void *fn)
{
	void *objb;
	void (*fnb)();
	u64 obja, fna;
	
	obja=(u64)obj;
	fna=(u64)fn;

#if 1
	((u16 *)obj)[ 0]=0xFE00|((obja>>56)&  255);
	((u16 *)obj)[ 1]=0x0000|((obja>>40)&65535);
	((u16 *)obj)[ 2]=0xFE00|((obja>>32)&  255);
	((u16 *)obj)[ 3]=0x0000|((obja>>16)&65535);
	((u16 *)obj)[ 4]=0xF803;
	((u16 *)obj)[ 5]=0x0000|((obja>> 0)&65535);

	((u16 *)obj)[ 6]=0xFE00|((fna>>56)&  255);
	((u16 *)obj)[ 7]=0x0000|((fna>>40)&65535);
	((u16 *)obj)[ 8]=0xFE00|((fna>>32)&  255);
	((u16 *)obj)[ 9]=0x0000|((fna>>16)&65535);
	((u16 *)obj)[10]=0xF801;
	((u16 *)obj)[11]=0x0000|((fna>> 0)&65535);

	((u16 *)obj)[12]=0xF020;
	((u16 *)obj)[13]=0x3010;

	((u16 *)obj)[14]=0x3030;
	((u16 *)obj)[15]=0x3030;
#endif

#if 0
	((u16 *)obj)[0]=0xF110;
	((u16 *)obj)[1]=0xB014;

	((u16 *)obj)[2]=0xFE00|((obja>>56)&  255);
	((u16 *)obj)[3]=0x0000|((obja>>40)&65535);
	((u16 *)obj)[4]=0xFE00|((obja>>32)&  255);
	((u16 *)obj)[5]=0x0000|((obja>>16)&65535);
	((u16 *)obj)[6]=0xF803;
	((u16 *)obj)[7]=0x0000|((obja>> 0)&65535);

	((u16 *)obj)[8]=0xF020;
	((u16 *)obj)[9]=0x3010;

	((u16 *)obj)[10]=0x3030;
	((u16 *)obj)[11]=0x3030;
	
	((u64 *)obj)[3]=fn;
#endif

	objb=__snipe_dc(((byte *)obj)-8);
	fna =((int *)objb)[ 0];
	fna+=((int *)objb)[ 2];
	fna+=((int *)objb)[ 4];
	fna+=((int *)objb)[ 6];
	fna+=((int *)objb)[ 8];
	fna+=((int *)objb)[10];

	fnb=__snipe_ic(((byte *)obj)-8);	fnb();
	fnb=__snipe_ic(((byte *)obj)+0);	fnb();
	fnb=__snipe_ic(((byte *)obj)+8);	fnb();
	fnb=__snipe_ic(((byte *)obj)+16);	fnb();
	fnb=__snipe_ic(((byte *)obj)+24);	fnb();
	fnb=__snipe_ic(((byte *)obj)+32);	fnb();
	

//	TK_FlushCacheL1D();

//	TK_FlushCacheL1D_INVDC(NULL);
//	TK_FlushCacheL1D_ReadBuf(obj, 32);
//	TK_FlushCacheL1D_INVIC(NULL);

	return(obj);
}

void __lvo_freelambda(void *obj)
{
	tk_free(obj);
}

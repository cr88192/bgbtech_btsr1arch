int tkmm_lvatyi_tagarray;
int tkmm_lvatyi_variant;
int tkmm_lvatyi_pointer;
int tkmm_lvatyi_byte;
int tkmm_lvatyi_sbyte;
int tkmm_lvatyi_short;
int tkmm_lvatyi_ushort;
int tkmm_lvatyi_int;
int tkmm_lvatyi_uint;
int tkmm_lvatyi_long;
int tkmm_lvatyi_ulong;
int tkmm_lvatyi_float;
int tkmm_lvatyi_double;

int tkmm_lvatyi_int128;
int tkmm_lvatyi_float128;

int tkmm_lvatyi_string;
int tkmm_lvatyi_wstring;
int tkmm_lvatyi_ustring;
int tkmm_lvatyi_classobj;

int tkmm_lvatyi_fixnum;
int tkmm_lvatyi_flonum;
int tkmm_lvatyi_tagobj;
int tkmm_lvatyi_cons;
int tkmm_lvatyi_bigint;

int tkmm_lvatyi_arrmt[16];
int tkmm_lvatyi_arrmsc[16];

LVA_ClassInfo	*tkmm_lva_clsinfo;
LVA_VTableBasic *tkmm_lva_clsvt;

extern int tkmm_lva_ntag;

void TKMM_LVA_InitTagOpr();
u64 __object_getbits(tk_lva_variant obj);


void TKMM_LVA_ArrayInit(void)
{
	byte *ptr, *ct;
	char *sig;

	if(tkmm_lva_ntag)
		return;
	
	TKMM_LVA_TagInit();

	tkmm_lvatyi_tagarray	=TKMM_LVA_GetTagIndexForName("_TagArray");	//01
	tkmm_lvatyi_variant		=TKMM_LVA_GetTagIndexForName("_Variant");	//02
	tkmm_lvatyi_string		=TKMM_LVA_GetTagIndexForName("_String");	//03
	tkmm_lvatyi_wstring		=TKMM_LVA_GetTagIndexForName("_WString");	//04
	tkmm_lvatyi_ustring		=TKMM_LVA_GetTagIndexForName("_UString");	//05
	tkmm_lvatyi_classobj	=TKMM_LVA_GetTagIndexForName("_ClassObj");	//06
	tkmm_lvatyi_pointer		=TKMM_LVA_GetTagIndexForName("_Pointer");	//07
	tkmm_lvatyi_byte		=TKMM_LVA_GetTagIndexForName("_Byte");		//08
	tkmm_lvatyi_sbyte		=TKMM_LVA_GetTagIndexForName("_SByte");		//09
	tkmm_lvatyi_short		=TKMM_LVA_GetTagIndexForName("_Short");		//0A
	tkmm_lvatyi_ushort		=TKMM_LVA_GetTagIndexForName("_UShort");	//0B
	tkmm_lvatyi_int			=TKMM_LVA_GetTagIndexForName("_Int");		//0C
	tkmm_lvatyi_uint		=TKMM_LVA_GetTagIndexForName("_UInt");		//0D
	tkmm_lvatyi_long		=TKMM_LVA_GetTagIndexForName("_Long");		//0E
	tkmm_lvatyi_ulong		=TKMM_LVA_GetTagIndexForName("_ULong");		//0F
	tkmm_lvatyi_float		=TKMM_LVA_GetTagIndexForName("_Float");		//10
	tkmm_lvatyi_double		=TKMM_LVA_GetTagIndexForName("_Double");	//11
	tkmm_lvatyi_int128		=TKMM_LVA_GetTagIndexForName("_Int128");	//12
	tkmm_lvatyi_float128	=TKMM_LVA_GetTagIndexForName("_Float128");	//13
	tkmm_lvatyi_fixnum		=TKMM_LVA_GetTagIndexForName("_Fixnum");	//14
	tkmm_lvatyi_flonum		=TKMM_LVA_GetTagIndexForName("_Flonum");	//15
	tkmm_lvatyi_tagobj		=TKMM_LVA_GetTagIndexForName("_TagObj");	//16
	tkmm_lvatyi_cons		=TKMM_LVA_GetTagIndexForName("_Cons");		//17
	tkmm_lvatyi_bigint		=TKMM_LVA_GetTagIndexForName("_BigInt");	//18

	tkmm_lvatyi_arrmt[0]=tkmm_lvatyi_int;
	tkmm_lvatyi_arrmt[1]=tkmm_lvatyi_long;
	tkmm_lvatyi_arrmt[2]=tkmm_lvatyi_float;
	tkmm_lvatyi_arrmt[3]=tkmm_lvatyi_double;
	tkmm_lvatyi_arrmt[4]=tkmm_lvatyi_pointer;
	tkmm_lvatyi_arrmt[5]=tkmm_lvatyi_variant;
	tkmm_lvatyi_arrmt[6]=0;
	tkmm_lvatyi_arrmt[7]=tkmm_lvatyi_long;
	tkmm_lvatyi_arrmt[8]=tkmm_lvatyi_sbyte;
	tkmm_lvatyi_arrmt[9]=tkmm_lvatyi_byte;
	tkmm_lvatyi_arrmt[10]=tkmm_lvatyi_short;
	tkmm_lvatyi_arrmt[11]=tkmm_lvatyi_ushort;
	tkmm_lvatyi_arrmt[12]=tkmm_lvatyi_uint;
	tkmm_lvatyi_arrmt[13]=tkmm_lvatyi_ulong;
	tkmm_lvatyi_arrmt[14]=tkmm_lvatyi_ulong;
	tkmm_lvatyi_arrmt[15]=0;
	
	tkmm_lvatyi_arrmsc[ 0]=2;
	tkmm_lvatyi_arrmsc[ 1]=3;
	tkmm_lvatyi_arrmsc[ 2]=2;
	tkmm_lvatyi_arrmsc[ 3]=3;
#ifdef __ADDR_X96__
	tkmm_lvatyi_arrmsc[ 4]=4;
	tkmm_lvatyi_arrmsc[ 5]=4;
#else
	tkmm_lvatyi_arrmsc[ 4]=3;
	tkmm_lvatyi_arrmsc[ 5]=3;
#endif
	tkmm_lvatyi_arrmsc[ 6]=2;
	tkmm_lvatyi_arrmsc[ 7]=3;
	tkmm_lvatyi_arrmsc[ 8]=0;
	tkmm_lvatyi_arrmsc[ 9]=0;
	tkmm_lvatyi_arrmsc[10]=1;
	tkmm_lvatyi_arrmsc[11]=1;
	tkmm_lvatyi_arrmsc[12]=2;
	tkmm_lvatyi_arrmsc[13]=3;
	tkmm_lvatyi_arrmsc[14]=3;
	tkmm_lvatyi_arrmsc[15]=3;
	
	ptr=tk_malloc(
		sizeof(LVA_VTableBasic)+
		sizeof(LVA_ClassInfo)+
		32);
	ct=ptr;

	tkmm_lva_clsvt=(LVA_VTableBasic *)(ct);
	ct+=sizeof(LVA_VTableBasic);
	tkmm_lva_clsinfo=(LVA_ClassInfo *)(ct);
	ct+=sizeof(LVA_ClassInfo);
	sig=(char *)ct;

	tkmm_lva_clsinfo->self=((byte *)tkmm_lva_clsinfo)-ptr;
	tkmm_lva_clsinfo->qname=((byte *)sig)-ptr;
	strcpy(sig, "UTagArray;");

	tkmm_lva_clsvt->clazz=tkmm_lva_clsinfo;
	
	TKMM_LVA_InitTagOpr();
}

int TKMM_LVA_ObjrefGetTypeTag(u64 val)
{
	int tg;

	if((val>>60)==0)
	{
		tg=val>>48;
		if(tg)
			return(tg);
		tg=TKMM_LVA_GetPtrTypeTag((void *)val);
		return(tg);
	}

	if((val>>60)==1)
	{
		return(tkmm_lvatyi_fixnum);
	}

	if((val>>60)==2)
	{
		return(tkmm_lvatyi_flonum);
	}
	
	return(0);
}

LVA_TagArray *TKMM_LVA_NewVarArray(int n)
{
	return(TKMM_LVA_NewTagArray(n, 5));
}

LVA_TagArray *TKMM_LVA_NewTagArray(int n, int mt)
{
	LVA_TagArray *arr;
	int sz;

	TKMM_LVA_ArrayInit();
	
	sz=n<<tkmm_lvatyi_arrmsc[mt];
	
	arr=TKMM_LVA_TagMalloc(tkmm_lvatyi_tagarray, sizeof(LVA_TagArray)+sz);
//	arr->p.vt=tkmm_lva_clsvt;
//	arr->p.dptr=arr->p.data;
//	arr->p.size=n;
//	arr->p.tty=tkmm_lvatyi_arrmt[mt];
//	arr->p.pad=0;

	arr->vt=tkmm_lva_clsvt;
	arr->data=arr->t_data;
	arr->size=n;
	arr->tty=tkmm_lvatyi_arrmt[mt];
	arr->base=0;

	return(arr);
}

tk_lva_object TKMM_LVA_WrapVarArray(LVA_TagArray *arr)
{
	u64 b;
	
	if((arr->size<4096) && (arr->base==0))
	{
		b=((long)(arr->data))&0x0000FFFFFFFFFFFFULL;
		b|=(((long)(arr->size))<<48);
		b|=2LL<<60;
		return(tk_lva_object_frombits(b));
	}
	
	b=((long)arr)&0x0000FFFFFFFFFFFFULL;
	b|=12LL<<60;
//	b|=((u64)tkmm_lvatyi_tagarray)<<48;
	return(tk_lva_object_frombits(b));
}


void *TKMM_LVA_GetArrayIndexPtr(tk_lva_object obj, int idx, int sc)
{
	LVA_TagArray *arr;
	byte *arrd;
	u64 obits;
	u32 bsi, ix2;
	int bti;

	obits=tk_lva_object_getbits(obj);

	if((obits>>60)==2)
	{
		bsi=(obits>>48)&4095;
		arrd=(byte *)(obits);
		if(idx>=bsi)
			{ __debugbreak(); }
		return(arrd+(idx<<sc));
	}

	if((obits>>60)==12)
	{
		bsi=(obits>>48)&4095;
		arr=(LVA_TagArray *)obits;

		arrd=(byte *)(arr->data);
		ix2=arr->base+bsi+idx;
		if(ix2>=arr->size)
			{ __debugbreak(); }
		return(arrd+(ix2<<sc));
		
#if 0
		if(arr->p.size>=0)
		{
			arrd=(byte *)(arr->data);
			ix2=bsi+idx;
			if(ix2>=arr->p.size)
			{
				__debugbreak();
				return(NULL);
			}
			return(arrd+(ix2<<sc));
		}else
		{
			arrd=(byte *)(arr->n.data_ty);
			ix2=arr->n.base+bsi+idx;
			if(ix2>=(-arr->n.size))
			{
				__debugbreak();
				return(NULL);
			}
			return(arrd+(ix2<<sc));
		}
#endif
	}

	if((obits>>60)==0)
	{
		bti=(obits>>48)&4095;
		bsi=0;
		
//		if(!bti && !idx)
//		{
//			//yeah...
//			return((void *)obits);
//		}
		
//		if(bti!=tkmm_lvatyi_tagarray)
//		{
//			__debugbreak();
//			return(NULL);
//		}

		arr=(LVA_TagArray *)obits;
		arrd=(byte *)(arr->data);
		ix2=arr->base+bsi+idx;
		if(ix2>=arr->size)
			{ __debugbreak(); }
		return(arrd+(ix2<<sc));

#if 0
		if(arr->p.size>=0)
		{
			arrd=(byte *)(arr->data);
			if(idx>=arr->p.size)
			{
				__debugbreak();
				return(NULL);
			}
			return(arrd+(ix2<<sc));
		}else
		{
			arrd=(byte *)(arr->n.data_ty);
			ix2=arr->n.base+idx;
			if(ix2>=(-arr->n.size))
			{
				__debugbreak();
				return(NULL);
			}
			return(arrd+(ix2<<sc));
		}
#endif
	}

	__debugbreak();
	return(NULL);
}

int __lvo_get_length(tk_lva_object obj)
{
	LVA_TagArray *arr;
	u64 obits;
	int bsi, bti;

	obits=tk_lva_object_getbits(obj);

	if((obits>>60)==12)
	{
		bsi=(obits>>48)&4095;
		arr=(LVA_TagArray *)obits;
		return(arr->size-bsi);
	}

	if((obits>>60)==0)
	{
		bti=(obits>>48)&4095;
		
		if(bti==tkmm_lvatyi_tagarray)
		{
			arr=(LVA_TagArray *)obits;
			return(arr->size);
		}
	}
		
	__debugbreak();
	return(0);
}

tk_lva_object __lvo_newvararray_0(void)
{
	return(tk_lva_object_null);
}

tk_lva_object __lvo_newvararray_1(tk_lva_object a0)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(1);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_2(tk_lva_object a0, tk_lva_object a1)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(2);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_3(tk_lva_object a0, tk_lva_object a1, tk_lva_object a2)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(3);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_4(
	tk_lva_object a0, tk_lva_object a1, tk_lva_object a2, tk_lva_object a3)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(4);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_5(
	tk_lva_object a0, tk_lva_object a1, tk_lva_object a2, tk_lva_object a3,
	tk_lva_object a4)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(5);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_6(
	tk_lva_object a0, tk_lva_object a1, tk_lva_object a2, tk_lva_object a3,
	tk_lva_object a4, tk_lva_object a5)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(6);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_7(
	tk_lva_object a0, tk_lva_object a1, tk_lva_object a2, tk_lva_object a3,
	tk_lva_object a4, tk_lva_object a5, tk_lva_object a6)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(7);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_8(
	tk_lva_object a0, tk_lva_object a1, tk_lva_object a2, tk_lva_object a3,
	tk_lva_object a4, tk_lva_object a5, tk_lva_object a6, tk_lva_object a7)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(8);
	arrd=(tk_lva_object *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_vn(
	tk_lva_object *ra, int n)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;
	int i;

	arr=TKMM_LVA_NewVarArray(n);
	arrd=(tk_lva_object *)(arr->data);
	for(i=0; i<n; i++)
		arrd[i]=ra[i];
	return(TKMM_LVA_WrapVarArray(arr));
}

void *__lvo_arraygetptr(tk_lva_object obj)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, 0, 0));
}

void *__lvo_loadindexaddr_b(tk_lva_object obj, int idx)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, idx, 0));
}

void *__lvo_loadindexaddr_s(tk_lva_object obj, int idx)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, idx, 1));
}

void *__lvo_loadindexaddr_i(tk_lva_object obj, int idx)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, idx, 2));
}

void *__lvo_loadindexaddr_l(tk_lva_object obj, int idx)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

void *__lvo_loadindexaddr_p(tk_lva_object obj, int idx)
{
	return(TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}


s32 __lvo_loadindex_i(tk_lva_object obj, int idx)
{
	return(*(s32 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2));
}

s64 __lvo_loadindex_l(tk_lva_object obj, int idx)
{
	return(*(s64 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

float __lvo_loadindex_f(tk_lva_object obj, int idx)
{
	return(*(float *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2));
}

double __lvo_loadindex_d(tk_lva_object obj, int idx)
{
	return(*(double *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

void *__lvo_loadindex_p(tk_lva_object obj, int idx)
{
#ifdef __ADDR_X96__
	return(*(void **)TKMM_LVA_GetArrayIndexPtr(obj, idx, 4));
#else
	return(*(void **)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
#endif
}

tk_lva_object __lvo_loadindex_v(tk_lva_object obj, int idx)
{
#ifdef __ADDR_X96__
	return(*(tk_lva_object *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 4));
#else
	return(*(tk_lva_object *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
#endif
}

int __lvo_loadindex_sb(tk_lva_object obj, int idx)
{
	return(*(sbyte *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 0));
}

int __lvo_loadindex_ub(tk_lva_object obj, int idx)
{
	return(*(byte *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 0));
}

int __lvo_loadindex_ss(tk_lva_object obj, int idx)
{
	return(*(s16 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 1));
}

int __lvo_loadindex_us(tk_lva_object obj, int idx)
{
	return(*(u16 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 1));
}

u32 __lvo_loadindex_ui(tk_lva_object obj, int idx)
{
	return(*(u32 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2));
}

u64 __lvo_loadindex_ul(tk_lva_object obj, int idx)
{
	return(*(u64 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

tk_lva_variant __lvo_loadindex_va(tk_lva_object obj, int idx)
{
#ifdef __ADDR_X96__
	return(*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 4));
#else
	return(*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
#endif
}

#ifdef __BGBCC__
__m128 __lvo_loadindex_x(tk_lva_object obj, int idx)
{
	return(*(__m128 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 4));
}
#endif

void __lvo_storeindex_i(tk_lva_object obj, int idx, int val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 2);
	*(u32 *)ptr=val;
//	*(u32 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2)=val;
}

void __lvo_storeindex_l(tk_lva_object obj, int idx, long long val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
	*(u64 *)ptr=val;
//	*(u64 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_f(tk_lva_object obj, int idx, float val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 2);
	*(float *)ptr=val;
//	*(float *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2)=val;
}

void __lvo_storeindex_d(tk_lva_object obj, int idx, double val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
	*(double *)ptr=val;
//	*(double *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_p(tk_lva_object obj, int idx, void *val)
{
	void *ptr;

#ifdef __ADDR_X96__
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 4);
#else
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
#endif

	*(void **)ptr=val;
//	*(void **)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_v(tk_lva_object obj, int idx, tk_lva_object val)
{
	void *ptr;
#ifdef __ADDR_X96__
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 4);
#else
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
#endif
	*(tk_lva_object *)ptr=val;
}

void __lvo_storeindex_b(tk_lva_object obj, int idx, int val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 0);
	*(byte *)ptr=val;
//	*(byte *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 0)=val;
}

void __lvo_storeindex_s(tk_lva_object obj, int idx, int val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 1);
	*(short *)ptr=val;
//	*(short *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 1)=val;
}

void __lvo_storeindex_va(tk_lva_object obj, int idx, tk_lva_variant val)
{
	void *ptr;
#ifdef __ADDR_X96__
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 4);
#else
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
#endif
	*(tk_lva_variant *)ptr=val;
//	*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

#ifdef __BGBCC__
void __lvo_storeindex_x(tk_lva_object obj, int idx, __m128 val)
{
	void *ptr;
	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 4);
	*(__m128 *)ptr=val;
//	*(short *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 1)=val;
}
#endif

tk_lva_variant __lvo_loadindex_var(tk_lva_object obj, int idx)
{
	LVA_TagArray *arr;
	byte *arrd;
	u64 objv, obits;
	s64 li;
	double lf;
	int tg4, tg12, tgc, cv;
	u32 bsi, ix2;
	int bti;

	objv=tk_lva_object_getbits(obj);
	if((objv>>60)==12)
	{
		bsi=(objv>>48)&4095;
		arr=(LVA_TagArray *)objv;
	}else if((objv>>60)==0)
	{
		bsi=0;
		arr=(LVA_TagArray *)objv;
	}

	arrd=(byte *)(arr->data);
	ix2=arr->base+bsi+idx;
	if(ix2>=arr->size)
		{ __debugbreak(); }

	li=0; obits=0; cv=0;
	switch(arr->tty)
	{
	case LVA_LVATY_VARIANT:
	case LVA_LVATY_STRING:
	case LVA_LVATY_WSTRING:
	case LVA_LVATY_USTRING:
	case LVA_LVATY_CLASSOBJ:
	case LVA_LVATY_POINTER:
	case LVA_LVATY_FIXNUM:
	case LVA_LVATY_FLONUM:
	case LVA_LVATY_TAGOBJ:
		obits=((u64 *)arrd)[ix2];
		break;
	case LVA_LVATY_BYTE:	li=((byte *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_SBYTE:	li=((sbyte *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_SHORT:	li=((s16 *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_USHORT:	li=((u16 *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_INT:		li=((s32 *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_UINT:	li=((u32 *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_LONG:	li=((s64 *)arrd)[ix2]; cv=1; break;
	case LVA_LVATY_ULONG:	li=((u64 *)arrd)[ix2]; cv=1; break;

	case LVA_LVATY_FLOAT:	lf=((float *)arrd)[ix2]; cv=2; break;
	case LVA_LVATY_DOUBLE:	lf=((double *)arrd)[ix2]; cv=2; break;
	}

	if(cv==1)
		obits = __lva_conv_fromi64(li);
	if(cv==2)
		obits = __lva_conv_fromf64(li);
	return(tk_lva_object_frombits(obits));
}

void __lvo_storeindex_var(tk_lva_object obj, int idx, tk_lva_variant val)
{
	LVA_TagArray *arr;
	byte *arrd;
	u64 objv, obits;
	s64 li;
	double lf;
	int tg4, tg12, tgc, cv;
	u32 bsi, ix2;
	int bti;

	objv=tk_lva_object_getbits(obj);
	obits=tk_lva_object_getbits(val);

	li=__lva_conv_toi64(obits);
	lf=__lva_conv_tof64(obits);

	if((objv>>60)==12)
	{
		bsi=(obits>>48)&4095;
		arr=(LVA_TagArray *)objv;
	}else if((objv>>60)==0)
	{
		bsi=0;
		arr=(LVA_TagArray *)objv;
	}

	arrd=(byte *)(arr->data);
	ix2=arr->base+bsi+idx;
	if(ix2>=arr->size)
		{ __debugbreak(); }

	switch(arr->tty)
	{
	case LVA_LVATY_VARIANT:
	case LVA_LVATY_STRING:
	case LVA_LVATY_WSTRING:
	case LVA_LVATY_USTRING:
	case LVA_LVATY_CLASSOBJ:
	case LVA_LVATY_POINTER:
	case LVA_LVATY_FIXNUM:
	case LVA_LVATY_FLONUM:
	case LVA_LVATY_TAGOBJ:
		((u64 *)arrd)[ix2]=obits;
		break;
	case LVA_LVATY_BYTE:	((byte *)arrd)[ix2]=li; break;
	case LVA_LVATY_SBYTE:	((sbyte *)arrd)[ix2]=li; break;
	case LVA_LVATY_SHORT:	((s16 *)arrd)[ix2]=li; break;
	case LVA_LVATY_USHORT:	((u16 *)arrd)[ix2]=li; break;
	case LVA_LVATY_INT:		((s32 *)arrd)[ix2]=li; break;
	case LVA_LVATY_UINT:	((u32 *)arrd)[ix2]=li; break;
	case LVA_LVATY_LONG:	((s64 *)arrd)[ix2]=li; break;
	case LVA_LVATY_ULONG:	((u64 *)arrd)[ix2]=li; break;

	case LVA_LVATY_FLOAT:	((float *)arrd)[ix2]=lf; break;
	case LVA_LVATY_DOUBLE:	((double *)arrd)[ix2]=lf; break;
	}
}

int __lvo_loadindex_str(tk_lva_variant obj, int idx)
{
	u16 *pw;
	byte *pb;
	u64 objv;
	int tg;
	
	objv=__object_getbits(obj);

	tg=(objv>>48)&65535;

	if(tg==tkmm_lvatyi_string)
	{
		pb=(byte *)objv;
		return(pb[idx]);
	}

	if(tg==tkmm_lvatyi_wstring)
	{
		pw=(u16 *)objv;
		return(pw[idx]);
	}

	__debugbreak();
}

void __lvo_storeindex_str(tk_lva_variant obj, int idx, int val)
{
	__debugbreak();
//	void *ptr;
//	ptr=TKMM_LVA_GetArrayIndexPtr(obj, idx, 3);
//	*(tk_lva_variant *)ptr=val;
//	*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}



tk_lva_object __lvo_newarray_sb_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_ub_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_ss_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_us_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_si_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_ui_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_sl_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_ul_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_f_0(void)
	{ return(tk_lva_object_null); }
tk_lva_object __lvo_newarray_d_0(void)
	{ return(tk_lva_object_null); }


tk_lva_object __lvo_newarray_sb_1(int a0)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_2(int a0, int a1)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 8);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_1(int a0)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_2(int a0, int a1)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 9);
	arrd=(byte *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_1(int a0)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_2(int a0, int a1)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 10);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}


tk_lva_object __lvo_newarray_us_1(int a0)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_2(int a0, int a1)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 11);
	arrd=(short *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}


tk_lva_object __lvo_newarray_si_1(int a0)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_2(int a0, int a1)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 0);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_1(int a0)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_2(int a0, int a1)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_6(int a0, int a1, int a2, int a3,
	int a4, int a5)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_7(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_8(int a0, int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 12);
	arrd=(int *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_1(s64 a0)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_2(s64 a0, s64 a1)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_3(s64 a0, s64 a1, s64 a2)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_4(
	s64 a0, s64 a1, s64 a2, s64 a3)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_5(
	s64 a0, s64 a1, s64 a2, s64 a3,
	s64 a4)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_6(
	s64 a0, s64 a1, s64 a2, s64 a3,
	s64 a4, s64 a5)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_7(
	s64 a0, s64 a1, s64 a2, s64 a3,
	s64 a4, s64 a5, s64 a6)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_8(
	s64 a0, s64 a1, s64 a2, s64 a3,
	s64 a4, s64 a5, s64 a6, s64 a7)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 1);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_1(u64 a0)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(1, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_2(u64 a0, u64 a1)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_3(u64 a0, u64 a1, u64 a2)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_4(
	u64 a0, u64 a1, u64 a2, u64 a3)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_5(
	u64 a0, u64 a1, u64 a2, u64 a3,
	u64 a4)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_6(
	u64 a0, u64 a1, u64 a2, u64 a3,
	u64 a4, u64 a5)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(6, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_7(
	u64 a0, u64 a1, u64 a2, u64 a3,
	u64 a4, u64 a5, u64 a6)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(7, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_8(
	u64 a0, u64 a1, u64 a2, u64 a3,
	u64 a4, u64 a5, u64 a6, u64 a7)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 13);
	arrd=(s64 *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}


tk_lva_object __lvo_newarray_f_1(float a0)
{
}

tk_lva_object __lvo_newarray_f_2(float a0, float a1)
{
}

tk_lva_object __lvo_newarray_f_3(float a0, float a1, float a2)
{
}

tk_lva_object __lvo_newarray_f_4(
	float a0, float a1, float a2, float a3)
{
}

tk_lva_object __lvo_newarray_f_5(
	float a0, float a1, float a2, float a3,
	float a4)
{
}

tk_lva_object __lvo_newarray_f_6(
	float a0, float a1, float a2, float a3,
	float a4, float a5)
{
}

tk_lva_object __lvo_newarray_f_7(
	float a0, float a1, float a2, float a3,
	float a4, float a5, float a6)
{
}

tk_lva_object __lvo_newarray_f_8(
	float a0, float a1, float a2, float a3,
	float a4, float a5, float a6, float a7)
{
	LVA_TagArray *arr;
	float	*arrd;
	arr=TKMM_LVA_NewTagArray(8, 2);
	arrd=(float *)(arr->data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	return(TKMM_LVA_WrapVarArray(arr));
}


tk_lva_object __lvo_newarray_d_1(double a0)
{
}

tk_lva_object __lvo_newarray_d_2(double a0, double a1)
{
}

tk_lva_object __lvo_newarray_d_3(double a0, double a1, double a2)
{
}

tk_lva_object __lvo_newarray_d_4(double a0, double a1, double a2, double a3)
{
}

tk_lva_object __lvo_newarray_d_5(double a0, double a1, double a2, double a3,
	double a4)
{
}

tk_lva_object __lvo_newarray_d_6(double a0, double a1, double a2, double a3,
	double a4, double a5)
{
}

tk_lva_object __lvo_newarray_d_7(double a0, double a1, double a2, double a3,
	double a4, double a5, double a6)
{
}

tk_lva_object __lvo_newarray_d_8(double a0, double a1, double a2, double a3,
	double a4, double a5, double a6, double a7)
{
}

tk_lva_object __lvo_newarray_sb_n(int n, ...)
{
}
tk_lva_object __lvo_newarray_ub_n(int n, ...)
{
}
tk_lva_object __lvo_newarray_ss_n(int n, ...)
{
}
tk_lva_object __lvo_newarray_us_n(int n, ...)
{
}
tk_lva_object __lvo_newarray_si_n(int n, ...)
{
}
tk_lva_object __lvo_newarray_ui_n(int n, ...)
{
}

tk_lva_object __lvo_newarray_sl_n(int n, ...)
{
}

tk_lva_object __lvo_newarray_ul_n(int n, ...)
{
}

tk_lva_object __lvo_newarray_f_n(int n, ...)
{
}

tk_lva_object __lvo_newarray_d_n(int n, ...)
{
}


tk_lva_object __lvo_newarray_sb_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 8);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 9);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 10);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 11);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 0);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 12);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 1);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 13);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_f_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 2);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_d_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 3);
	return(TKMM_LVA_WrapVarArray(arr));
}


tk_lva_object __lvo_newarray_ptr_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 4);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_var_sz1(int sz)
{
	LVA_TagArray *arr;
	arr=TKMM_LVA_NewTagArray(sz, 5);
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sig_sz1(char *sig, int sz)
{
}

void *__operator_new(int sz)
{
	return(tk_malloc(sz));
}


tk_lva_object TKMM_LVA_CreateShareBuffer(void *pbuf, int sz)
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *taskusr;
	u64 pbase, pbsz, tobj;
	int i;

	task=TK_GetCurrentTask();
	taskusr=(TKPE_TaskInfoUser *)task->usrptr;
	
	for(i=0; i<32; i++)
	{
		if(!taskusr->sharebuf[i])
		{
			pbase=(u64)pbuf;
			pbsz=(sz+255)>>8;
			pbase&=0x0000FFFFFFFFFFFFULL;
			pbase|=pbsz<<48;
			taskusr->sharebuf[i]=pbase;
			
			tobj=0x1C00000000000000ULL;
			tobj|=(task->pid<<16)|i;
			return(tk_lva_object_frombits(tobj));
		}
	}
	
	return(tk_lva_object_null);
}

int TKMM_LVA_DestroyShareBuffer(tk_lva_object obj)
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *taskusr;
	u64 pbase, pbsz, tobj;
	int i;

	task=TK_GetCurrentTask();
	taskusr=(TKPE_TaskInfoUser *)task->usrptr;

	tobj=tk_lva_object_getbits(obj);
	if(((u16)(tobj>>16))!=task->pid)
		__debugbreak();

	i=tobj&0xFF;
	taskusr->sharebuf[i]=0;
	
	return(0);
}

void *TKMM_LVA_MapShareBufferRead(tk_lva_object obj)
{
}

void *TKMM_LVA_MapShareBufferWrite(tk_lva_object obj)
{
}

void *TKMM_LVA_MapShareBufferModify(tk_lva_object obj)
{
}

int TKMM_LVA_UnmapShareBuffer(void *ptr)
{
}


void *TKMM_LVA_ExportSharedObjectPtr(void *ptr)
{
	/* TODO: Register object pointer to return to exporter. */
	return(ptr);
}


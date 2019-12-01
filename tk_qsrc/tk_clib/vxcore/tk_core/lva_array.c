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

int tkmm_lvatyi_string;
int tkmm_lvatyi_classobj;

int tkmm_lvatyi_arrmt[16];
int tkmm_lvatyi_arrmsc[16];

void TKMM_LVA_ArrayInit(void)
{
	if(!tkmm_lvatyi_tagarray)
	{
		TKMM_LVA_TagInit();
	
		tkmm_lvatyi_tagarray=TKMM_LVA_GetTagIndexForName("_TagArray");
		tkmm_lvatyi_variant	=TKMM_LVA_GetTagIndexForName("_Variant");
		tkmm_lvatyi_pointer	=TKMM_LVA_GetTagIndexForName("_Pointer");
		tkmm_lvatyi_byte	=TKMM_LVA_GetTagIndexForName("_Byte");
		tkmm_lvatyi_sbyte	=TKMM_LVA_GetTagIndexForName("_SByte");
		tkmm_lvatyi_short	=TKMM_LVA_GetTagIndexForName("_Short");
		tkmm_lvatyi_ushort	=TKMM_LVA_GetTagIndexForName("_UShort");
		tkmm_lvatyi_int		=TKMM_LVA_GetTagIndexForName("_Int");
		tkmm_lvatyi_uint	=TKMM_LVA_GetTagIndexForName("_UInt");
		tkmm_lvatyi_long	=TKMM_LVA_GetTagIndexForName("_Long");
		tkmm_lvatyi_ulong	=TKMM_LVA_GetTagIndexForName("_ULong");
		tkmm_lvatyi_float	=TKMM_LVA_GetTagIndexForName("_Float");
		tkmm_lvatyi_double	=TKMM_LVA_GetTagIndexForName("_Double");

		tkmm_lvatyi_string	=TKMM_LVA_GetTagIndexForName("_String");
		tkmm_lvatyi_classobj=TKMM_LVA_GetTagIndexForName("_ClassObj");
		
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
		tkmm_lvatyi_arrmsc[ 4]=3;
		tkmm_lvatyi_arrmsc[ 5]=3;
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
	}
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
	
	arr=TKMM_LVA_TyMalloc(tkmm_lvatyi_tagarray, sizeof(LVA_TagArray)+sz);
	arr->p.size=n;
	arr->p.tty=tkmm_lvatyi_arrmt[mt];
	arr->p.pad=0;
	return(arr);
}

tk_lva_object TKMM_LVA_WrapVarArray(LVA_TagArray *arr)
{
	u64 b;
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
	if((obits>>60)==12)
	{
		bsi=(obits>>48)&4095;
		arr=(LVA_TagArray *)obits;
		
		if(arr->p.size>=0)
		{
			arrd=(byte *)(arr->p.data);
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
	}

	if((obits>>60)==0)
	{
		bti=(obits>>48)&4095;
		
		if(bti!=tkmm_lvatyi_tagarray)
		{
			__debugbreak();
			return(NULL);
		}

		arr=(LVA_TagArray *)obits;
		
		if(arr->p.size>=0)
		{
			arrd=(byte *)(arr->p.data);
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
		
		if(arr->p.size>=0)
			return(arr->p.size-bsi);
		else
			return((-arr->n.size)-arr->n.base-bsi);
	}

	if((obits>>60)==0)
	{
		bti=(obits>>48)&4095;
		
		if(bti==tkmm_lvatyi_tagarray)
		{
			arr=(LVA_TagArray *)obits;
			
			if(arr->p.size>=0)
				return(arr->p.size);
			else
				return((-arr->n.size)-arr->n.base);
		}
	}
		
	__debugbreak();
	return(NULL);
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
	arrd=(tk_lva_object *)(arr->p.data);
	arrd[0]=a0;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_2(tk_lva_object a0, tk_lva_object a1)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(2);
	arrd=(tk_lva_object *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newvararray_3(tk_lva_object a0, tk_lva_object a1, tk_lva_object a2)
{
	LVA_TagArray *arr;
	tk_lva_object	*arrd;

	arr=TKMM_LVA_NewVarArray(3);
	arrd=(tk_lva_object *)(arr->p.data);
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
	arrd=(tk_lva_object *)(arr->p.data);
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
	arrd=(tk_lva_object *)(arr->p.data);
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
	arrd=(tk_lva_object *)(arr->p.data);
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
	arrd=(tk_lva_object *)(arr->p.data);
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
	arrd=(tk_lva_object *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	arrd[4]=a4;		arrd[5]=a5;
	arrd[6]=a6;		arrd[7]=a7;
	
	return(TKMM_LVA_WrapVarArray(arr));
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
	return(*(void **)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

tk_lva_object __lvo_loadindex_v(tk_lva_object obj, int idx)
{
	return(*(tk_lva_object *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
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
	return(*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

void __lvo_storeindex_i(tk_lva_object obj, int idx, int val)
{
	*(u32 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2)=val;
}

void __lvo_storeindex_l(tk_lva_object obj, int idx, long long val)
{
	*(u64 *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_f(tk_lva_object obj, int idx, float val)
{
	*(float *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 2)=val;
}

void __lvo_storeindex_d(tk_lva_object obj, int idx, double val)
{
	*(double *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_p(tk_lva_object obj, int idx, void *val)
{
	*(void **)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_v(tk_lva_object obj, int idx, tk_lva_object val)
{
	*(tk_lva_object *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
}

void __lvo_storeindex_b(tk_lva_object obj, int idx, int val)
{
	*(byte *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 0)=val;
}

void __lvo_storeindex_s(tk_lva_object obj, int idx, int val)
{
	*(short *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 1)=val;
}

void __lvo_storeindex_va(tk_lva_object obj, int idx, tk_lva_variant val)
{
	*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3)=val;
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
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_2(int a0, int a1)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 8);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 8);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 8);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sb_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 8);
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_2(int a0, int a1)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 9);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 9);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 9);
	arrd=(byte *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ub_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	byte	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 9);
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(byte *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_2(int a0, int a1)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 10);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 10);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 10);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ss_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 10);
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_2(int a0, int a1)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 11);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 11);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 11);
	arrd=(short *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_us_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	short	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 11);
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(short *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_2(int a0, int a1)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 0);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 0);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 0);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_si_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 0);
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_2(int a0, int a1)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 12);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_3(int a0, int a1, int a2)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 12);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_4(int a0, int a1, int a2, int a3)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(4, 12);
	arrd=(int *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	arrd[2]=a2;		arrd[3]=a3;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ui_5(int a0, int a1, int a2, int a3, int a4)
{
	LVA_TagArray *arr;
	int	*arrd;
	arr=TKMM_LVA_NewTagArray(5, 12);
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(int *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_2(s64 a0, s64 a1)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 1);
	arrd=(s64 *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_sl_3(s64 a0, s64 a1, s64 a2)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 1);
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
	arrd[0]=a0;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_2(u64 a0, u64 a1)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(2, 13);
	arrd=(s64 *)(arr->p.data);
	arrd[0]=a0;		arrd[1]=a1;
	return(TKMM_LVA_WrapVarArray(arr));
}

tk_lva_object __lvo_newarray_ul_3(u64 a0, u64 a1, u64 a2)
{
	LVA_TagArray *arr;
	s64	*arrd;
	arr=TKMM_LVA_NewTagArray(3, 13);
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(s64 *)(arr->p.data);
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
	arrd=(float *)(arr->p.data);
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


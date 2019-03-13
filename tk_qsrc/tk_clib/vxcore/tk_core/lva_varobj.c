char **tkmm_lva_syms_name;
u16 *tkmm_lva_syms_chn;
int tkmm_lva_syms_hash[256];
int tkmm_lva_syms_nsym;

int TKMM_LVA_VarObjKeyForSymbol(char *name)
{
	int h;
	int i, j, k;

	if(!name)
		return(0);

	if(!tkmm_lva_syms_name)
	{
		tkmm_lva_syms_name=TKMM_LVA_TyMalloc("_MiscBuf/Ptr",
			4096*sizeof(void *));
		tkmm_lva_syms_chn=TKMM_LVA_TyMalloc("_MiscBuf/Short",
			4096*sizeof(short));
		tkmm_lva_syms_nsym=1;

		for(i=0; i<256; i++)
			{ tkmm_lva_syms_hash[i]=0; }
	}
	
	h=TKMM_LVA_HashName(name);
	h=(h>>16)&255;
	
	i=tkmm_lva_syms_hash[h];
	while(i)
	{
		if(!strcmp(tkmm_lva_syms_name[i], name))
			return(i);
		i=tkmm_lva_syms_chn[i];
	}
	
	i=tkmm_lva_syms_nsym++;
	tkmm_lva_syms_name[i]=TKMM_LVA_Strdup(name);
	tkmm_lva_syms_chn[i]=tkmm_lva_syms_hash[h];
	tkmm_lva_syms_hash[h]=i;
	
	return(i);
}

LVA_FieldInfo *TKMM_LVA_VarObjLookupFieldKey(LVA_VarObject *obj, int key)
{
}

void *TKMM_LVA_CinfTransPtr(LVA_ClassInfo *cinf, u32 ptr)
{
	s64 adj;
	if(!ptr)
		return(NULL);
	
	adj=((u64)cinf)-(cinf->self);
	return((void *)(adj+ptr));
}

int TKMM_LVA_ObjLookupField(LVA_ObjectBasic *obj, char *name,
	LVA_FieldInfo *ofi)
{
	LVA_ClassInfo *cinf;
//	LVA_FieldInfo **afi;
	u32 *afi;
	LVA_FieldInfoD *fi;
	s64 adj;
	char *sn, *fn;
	int i, j;
	
	cinf=obj->vt->clazz;
	while(cinf)
	{
		adj=((u64)cinf)-(cinf->self);
//		afi=cinf->fields;
//		afi=TKMM_LVA_CinfTransPtr(cinf, cinf->fields);
		afi=(void *)(adj+cinf->fields);
		for(i=0; afi[i]; i++)
		{
//			fi=afi[i];
//			fi=TKMM_LVA_CinfTransPtr(cinf, afi[i]);
			fi=(void *)(adj+afi[i]);
			fn=(void *)(adj+fi->name);
			if(!strcmp(fn, name))
			{
				ofi->name=TKMM_LVA_CinfTransPtr(cinf, fi->name);
				ofi->sig=TKMM_LVA_CinfTransPtr(cinf, fi->sig);
				ofi->flags=fi->flags;
				ofi->offset=fi->offset;
				return(1);
			}
		}

//		cinf=cinf->super;
		cinf=TKMM_LVA_CinfTransPtr(cinf, cinf->super);
	}
	
	return(0);
}

u64 TKMM_LVA_TryGetSigPtrVar(void *ptr, char *sig)
{
	u64 v;
	
	v=0;
	switch(*sig)
	{
	case 'a':	case 'b':	case 'c':
		v=TKMM_LVA_WrapFixnum(*(sbyte *)ptr);			break;
	case 'd':	case 'e':
		v=TKMM_LVA_WrapFlonum(*(double *)ptr);			break;
	case 'f':	v=TKMM_LVA_WrapFlonum(*(float *)ptr);	break;
	case 'g':	break;
	case 'h':	v=TKMM_LVA_WrapFixnum(*(byte *)ptr);	break;
	case 'i':	v=TKMM_LVA_WrapFixnum(*(s32 *)ptr);		break;
	case 'j':	v=TKMM_LVA_WrapFixnum(*(u32 *)ptr);		break;

	case 'l':	v=TKMM_LVA_WrapFixnum(*(s64 *)ptr);		break;
	case 'm':	v=TKMM_LVA_WrapFixnum(*(u64 *)ptr);		break;
	case 'n':	break;
	case 'o':	break;
	case 'p':	v=TKMM_LVA_WrapFixnum(*(s64 *)ptr);		break;
	case 'q':	break;
	case 'r':	v=*(u64 *)ptr;							break;
	case 's':	v=TKMM_LVA_WrapFixnum(*(s16 *)ptr);		break;
	case 't':	v=TKMM_LVA_WrapFixnum(*(u16 *)ptr);		break;
	case 'u':	break;
	case 'v':	break;
	case 'w':	v=TKMM_LVA_WrapFixnum(*(u16 *)ptr);		break;
	case 'x':	v=TKMM_LVA_WrapFixnum(*(s64 *)ptr);		break;
	case 'y':	v=TKMM_LVA_WrapFixnum(*(u64 *)ptr);		break;
	case 'z':	break;
	
	case 'L':	case 'Q':
		v=*(u64 *)ptr;
		break;
		
	case 'C':
		switch(sig[1])
		{
		case 'o':	case 's':
			v=*(u64 *)ptr;
			break;
		default:
			break;
		}
		break;

	case 'P':
		switch(sig[1])
		{
		case 'c':
			v=TKMM_LVA_WrapString(*(char **)ptr);
			break;
		default:
			v=TKMM_LVA_WrapPointer(*(char **)ptr);
			break;
		}
		break;
	}
	return(0);
}


void TKMM_LVA_TrySetSigPtrVar(void *ptr, char *sig, u64 val)
{
	u64 v;
	
	v=0;
	switch(*sig)
	{
	case 'a':	case 'b':	case 'c':
		*(sbyte *)ptr=TKMM_LVA_UnwrapFixnum(val);			break;
	case 'd':	case 'e':
		*(double *)ptr=TKMM_LVA_UnwrapFlonum(val);			break;
	case 'f':	*(float *)ptr=TKMM_LVA_UnwrapFlonum(val);	break;
	case 'g':	break;
	case 'h':	(*(byte *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'i':	(*(s32 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'j':	(*(u32 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;

	case 'l':	(*(s64 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'm':	(*(u64 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'n':	break;
	case 'o':	break;
	case 'p':	(*(s64 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'q':	break;
	case 'r':	*(u64 *)ptr=val;							break;
	case 's':	(*(s16 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 't':	(*(u16 *)ptr)=TKMM_LVA_UnwrapFixnum(val);	break;
	case 'u':	break;
	case 'v':	break;
	case 'w':	(*(u16 *)ptr)=TKMM_LVA_UnwrapFixnum(val);		break;
	case 'x':	(*(s64 *)ptr)=TKMM_LVA_UnwrapFixnum(val);		break;
	case 'y':	(*(u64 *)ptr)=TKMM_LVA_UnwrapFixnum(val);		break;
	case 'z':	break;
	
	case 'L':	case 'Q':
		*(u64 *)ptr=val;
		break;
		
	case 'C':
		switch(sig[1])
		{
		case 'o':	case 's':
			*(u64 *)ptr=val;
			break;
		default:
			break;
		}
		break;

	case 'P':
		switch(sig[1])
		{
		case 'c':
			(*(char **)ptr)=TKMM_LVA_UnwrapPointer(val);
			break;
		default:
			(*(char **)ptr)=TKMM_LVA_UnwrapPointer(val);
			break;
		}
		break;
	}
	return(0);
}

u64 TKMM_LVA_TryObjGetFieldVar(LVA_ObjectBasic *obj, char *name)
{
	LVA_FieldInfo tfi;
	byte *ptr;
	int i;

	i=TKMM_LVA_ObjLookupField(obj, name, &tfi);
	if(i<=0)
		{ return(LVA_MAGIC_UNDEFINED); }
	
	ptr=((byte *)obj)+tfi.offset;
	return(TKMM_LVA_TryGetSigPtrVar(ptr, tfi.sig));
}

int TKMM_LVA_TryObjSetFieldVar(LVA_ObjectBasic *obj, char *name, u64 val)
{
	LVA_FieldInfo tfi;
	byte *ptr;
	int i;

	i=TKMM_LVA_ObjLookupField(obj, name, &tfi);
	if(i<=0)
		{ return(0); }
	
	ptr=((byte *)obj)+tfi.offset;
	TKMM_LVA_TrySetSigPtrVar(ptr, tfi.sig, val);
	return(1);
}

u64 TKMM_LVA_VarObjFetchKey(LVA_VarObject *obj, int key)
{
	u16 *keys;		//dynamic alloc keys
	u64 *vals;		//dynamic alloc values
	u64 kv;
	int k_m, k_n, k_c, k_cv;
	int nk;
	
	nk=obj->nkey;
	keys=obj->keys;
	vals=obj->vals;

	k_m=0; k_n=nk;
	while(k_n>k_m)
	{
		k_c=(k_m+k_n)>>1;
		k_cv=keys[k_c];
		if(k_cv>key)
			{ k_n=k_c; }
		else if((k_cv<key) && (k_c!=k_m))
			{ k_m=k_c; }
		else
			{ break; }
	}

	kv=vals[k_c];
	if(k_cv==key)
		return(kv);
	return(0);
}

int TKMM_LVA_VarObjSetKey(LVA_VarObject *obj, int key, u64 val)
{
	u16 *keys;		//dynamic alloc keys
	u64 *vals;		//dynamic alloc values
	u64 kv;
	int k_m, k_n, k_c, k_cv;
	int nk, mk, mk1;
	int i, j, k;
	
	nk=obj->nkey;
	keys=obj->keys;
	vals=obj->vals;

	k_m=0; k_n=nk;
	while(k_n>k_m)
	{
		k_c=(k_m+k_n)>>1;
		k_cv=keys[k_c];
		if(k_cv>key)
			{ k_n=k_c; }
		else if((k_cv<key) && (k_c!=k_m))
			{ k_m=k_c; }
		else
			{ break; }
	}

	if(k_cv==key)
	{
		vals[k_c]=val;
		return(1);
	}
	
	mk=obj->mkey;
	if(mk)
	{
		if((nk+1)>=mk)
		{
			mk1=mk+(mk>>1);
			obj->keys=TKMM_LVA_ReAlloc(obj->keys, mk1*sizeof(u16));
			obj->vals=TKMM_LVA_ReAlloc(obj->vals, mk1*sizeof(u64));
			obj->mkey=mk1;

			keys=obj->keys;
			vals=obj->vals;
		}
	}else
	{
		if((nk+1)>=16)
		{
			mk1=64;
			obj->keys=TKMM_LVA_TyMalloc("_MiscBuf/Short", mk1*sizeof(u16));
			obj->vals=TKMM_LVA_TyMalloc("_MiscBuf/Variant", mk1*sizeof(u64));
			obj->mkey=mk1;
			memcpy(obj->keys, keys, 16*sizeof(u16));
			memcpy(obj->vals, vals, 16*sizeof(u64));
			
			keys=obj->keys;
			vals=obj->vals;
		}
	}
	
	for(i=nk; i>0; i++)
	{
		if(keys[i-1]<key)
			break;
		keys[i]=keys[i-1];
		vals[i]=vals[i-1];
	}
	keys[i]=key;
	vals[i]=val;
	obj->mkey=nk+1;
	
	return(1);
}


__variant  __lvo_getslot(__object obj, char *name)
{
	LVA_ObjectBasic *ob;
	u64 objv;
	u64 v;
	int tag, key;

	objv=__object_getbits(obj);
	if(TKMM_LVA_ObjRefP(obj))
	{
		tag=TKMM_LVA_GetObjTag(objv);
		if(tag==tkmm_lvatyi_classobj)
		{
			ob=(void *)objv;
			v=TKMM_LVA_TryObjGetFieldVar(ob, name);
			if(v!=LVA_MAGIC_UNDEFINED)
				{ return(__object_frombits(v)); }
			
			if(ob->vo)
			{
				key=TKMM_LVA_VarObjKeyForSymbol(name);
				v=TKMM_LVA_VarObjFetchKey(ob->vo, key);
				return(__object_frombits(v));
			}
			
			v=LVA_MAGIC_UNDEFINED;
			return(__object_frombits(v));
		}
	}
}

void __lvo_setslot(__object obj, char *name, __variant val)
{
	LVA_ObjectBasic *ob;
	u64 objv, valv;
	int tag, key;

	objv=__object_getbits(obj);
	if(TKMM_LVA_ObjRefP(obj))
	{
		tag=TKMM_LVA_GetObjTag(objv);
		if(tag==tkmm_lvatyi_classobj)
		{
			valv=__object_getbits(val);

			ob=(void *)objv;
			if(TKMM_LVA_TryObjSetFieldVar(ob, name, valv)>0)
				return;
			
			if(ob->vo)
			{
				key=TKMM_LVA_VarObjKeyForSymbol(name);
				TKMM_LVA_VarObjSetKey(ob->vo, key, valv);
				return;
			}
			
			return;
		}
	}
}

__variant __lvo_methodcall(__object obj, char *name, __object args)
{
}


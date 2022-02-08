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

int TKMM_LVA_TryGetSigSize(char *sig)
{
	static const byte szarr_base[96] =
	{
		0, 0,   0, 0,  0,  0,  0,  0,  /* 20-27 */
		0, 0,   0, 0,  0,  0,  0,  0,  /* 28-2F */
		0, 0,   0, 0,  0,  0,  0,  0,  /* 30-37, '0'..'7' */
		0, 0,   0, 0,  0,  0,  0,  0,  /* 38-3F */
		0, 0,   0, 0,  0,  0,  0,  0,  /* 40-47, 'A'..'G' */
		0, 0,   0, 0,  8,  0,  0,  0,  /* 48-4F, 'H'..'O' */
		0, 8,   0, 0,  0,  0,  0,  0,  /* 50-57, 'P'..'W' */
		0, 0,   0, 0,  0,  0,  0,  0,  /* 58-5F, 'X'..'Z' */
		0, 1,   1, 1,  8, 16,  4, 16,  /* 60-67, 'a'..'g' */
		1, 4,   4, 2,  8,  8, 16, 16,  /* 68-6F, 'h'..'o' */
		8, 8,   8, 2,  2,  2,  1,  2,  /* 70-77, 'p'..'w' */
		8, 8, 128, 0,  0,  0,  0,  0   /* 78-7F, 'x'..'z' */
	};

	static const byte szarr_c[96] =
	{
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 20-27 */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 28-2F */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 30-37, '0'..'7' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 38-3F */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 40-47, 'A'..'G' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 48-4F, 'H'..'O' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 50-57, 'P'..'W' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 58-5F, 'X'..'Z' */
		 0,  8, 16, 16, 16, 32,  8, 32,  /* 60-67, 'a'..'g' */
		16, 16, 16,  4,  8,  8, 16,  8,  /* 68-6F, 'h'..'o' */
		16, 16,  8,  8,  8,  4,  8,  8,  /* 70-77, 'p'..'w' */
		 8,  8,  8,  0,  0,  0,  0,  0   /* 78-7F, 'x'..'z' */
	};

	static const byte szarr_d[96] =
	{
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 20-27 */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 28-2F */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 30-37, '0'..'7' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 38-3F */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 40-47, 'A'..'G' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 48-4F, 'H'..'O' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 50-57, 'P'..'W' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 58-5F, 'X'..'Z' */
		 0,  8,  0,  0,  0,  0,  0,  0,  /* 60-67, 'a'..'g' */
		 0,  8,  8,  0,  0,  0,  8,  0,  /* 68-6F, 'h'..'o' */
		 0,  0,  0,  0,  0,  0,  0,  0,  /* 70-77, 'p'..'w' */
		 0,  0,  0,  0,  0,  0,  0,  0   /* 78-7F, 'x'..'z' */
	};

	int sz, c0, c1;

	c0=sig[0];
	c1=sig[1];
	sz=szarr_base[c0-' '];
	if(sz>0)
		return(sz);
	if(c0=='P')
		return(sizeof(void *));

	if(c0=='C')
	{
		sz=szarr_c[c1-' '];
		return(sz);
	}

	if(c0=='D')
	{
		sz=szarr_d[c1-' '];
		return(sz);
	}

	if(c0=='G')
	{
		sz=szarr_base[c1-' '];
		return(sz);
	}

	__debugbreak();
	return(0);

#if 0
	sz=0;
	switch(*sig)
	{
	case 'a':		sz=1;	break;
	case 'b':		sz=1;	break;
	case 'c':		sz=1;	break;
	case 'd':		sz=8;	break;
	case 'e':		sz=16;	break;
	case 'f':		sz=4;	break;
	case 'g':		sz=16;	break;
	case 'h':		sz=1;	break;
	case 'i':		sz=4;	break;
	case 'j':		sz=4;	break;
	case 'k':		sz=2;	break;
	case 'l':		sz=8;	break;
	case 'm':		sz=8;	break;
	case 'n':		sz=16;	break;
	case 'o':		sz=16;	break;
	case 'p':		sz=8;	break;
	case 'q':		sz=8;	break;
	case 'r':		sz=8;	break;
	case 's':		sz=2;	break;
	case 't':		sz=2;	break;
	case 'u':		sz=2;	break;
	case 'v':		sz=1;	break;
	case 'w':		sz=2;	break;
	case 'x':		sz=8;	break;
	case 'y':		sz=8;	break;
	case 'z':		sz=128;	break;

	case 'L':		sz=8;	break;
	case 'Q':		sz=8;	break;
		
	case 'C':
		switch(sig[1])
		{
		case 'a':	sz=8;	break;
		case 'b':	sz=16;	break;
		case 'c':	sz=16;	break;
		case 'd':	sz=16;	break;
		case 'e':	sz=32;	break;
		case 'f':	sz=8;	break;
		case 'g':	sz=32;	break;
		case 'h':	sz=16;	break;
		case 'i':	sz=16;	break;
		case 'j':	sz=16;	break;
		case 'k':	sz=4;	break;

		case 'o':	sz=8;	break;
		case 's':	sz=8;	break;
		default:			break;
		}
		break;

	case 'P':								sz=8;	break;
	}
	return(0);
#endif
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

int TKMM_LVA_TryGetSigTty(char *sig)
{
	int sz;
	
	sz=0;
	switch(*sig)
	{
	case 'a':			sz=8;	break;
	case 'b':			sz=8;	break;
	case 'c':			sz=8;	break;
	case 'd':			sz=3;	break;
	case 'e':			sz=6;	break;
	case 'f':			sz=2;	break;
	case 'g':			sz=6;	break;
	case 'h':			sz=9;	break;
	case 'i':			sz=0;	break;
	case 'j':			sz=12;	break;
	case 'l':			sz=7;	break;
	case 'm':			sz=13;	break;
	case 'n':			sz=6;	break;
	case 'o':			sz=6;	break;
	case 'p':			sz=4;	break;
	case 'q':			sz=4;	break;
	case 'r':			sz=7;	break;
	case 's':			sz=10;	break;
	case 't':			sz=11;	break;
	case 'u':			sz=0;	break;
	case 'v':			sz=0;	break;
	case 'w':			sz=11;	break;
	case 'x':			sz=7;	break;
	case 'y':			sz=13;	break;
	case 'z':			sz=0;	break;
	case 'L':			sz=5;	break;
	case 'Q':			sz=5;	break;
		
	case 'C':
		switch(sig[1])
		{
		case 'o':	case 's':				sz=8;	break;
		default:							break;
		}
		break;
	case 'P':								sz=8;	break;
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
//	return(0);
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

// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
u64 TKMM_LVA_VarObjFetchKeyA(LVA_VarObject *obj, int key);
u64 TKMM_LVA_VarObjSetKeyA(LVA_VarObject *obj, int key, u64 val);

__asm {
TKMM_LVA_VarObjFetchKeyA:
	MOVU.W		(R4, 16), R3
	MOV.Q		(R4, 0), R6
	MOV.Q		(R4, 8), R7	
	CMPGT		16, R3
	BT			TKMM_LVA_VarObjFetchKey
	
	MOV.X		(R6), R16
	PSHUF.W		R5, 0x00, R5

//	MOV.Q		(R6, 0), R16
//	MOV.Q		(R6, 8), R17
	PSCHEQ.W	R16, R5, R3
	BT			.Found
	PSCHEQ.W	R17, R5, R3
	BT			.Found1
	
	ADD			16, R6
	ADD			64, R7

	MOV.X		(R6), R16
//	MOV.Q		(R6, 0), R16
//	MOV.Q		(R6, 8), R17
	PSCHEQ.W	R16, R5, R3
	BT			.Found
	PSCHEQ.W	R17, R5, R3
	BT			.Found1
	
	BRA			.NotFound
	
	.Found1:
//	ADD			32, R7
	ADD			4, R3
	.Found:
	MOV.Q		(R7, R3), R2
	RTS
	
	.NotFound:
	BRA			TKMM_LVA_VarObjFetchKey
//	MOV			1, R2
//	RTS

TKMM_LVA_VarObjSetKeyA:
	MOVU.W		(R4, 16), R3
	MOV.Q		(R4, 0), R18
	MOV.Q		(R4, 8), R19
	CMPGT		16, R3
	BT			TKMM_LVA_VarObjSetKey
	
	MOV.X		(R18), R16
	PSHUF.W		R5, 0x00, R5

//	MOV.Q		(R18, 0), R16
//	MOV.Q		(R18, 8), R17
	PSCHEQ.W	R16, R5, R3
	BT			.Found
	PSCHEQ.W	R17, R5, R3
	BT			.Found1
	
	ADD			16, R18
	ADD			64, R19

	MOV.X		(R18), R16
//	MOV.Q		(R18, 0), R16
//	MOV.Q		(R18, 8), R17
	PSCHEQ.W	R16, R5, R3
	BT			.Found
	PSCHEQ.W	R17, R5, R3
	BT			.Found1
	
	BRA			.NotFound
	
	.Found1:
	ADD			4, R3
	.Found:
	MOV.Q		R7, (R19, R3)
	RTS
	
	.NotFound:
	BRA			TKMM_LVA_VarObjSetKey
//	MOV			-1, R2
//	RTS
};

#endif

u64 TKMM_LVA_VarObjFetchKeyR(LVA_VarObject *obj, int key,
	LVA_VarObject **dsarr, int *rndsa)
{
	LVA_VarObject *obj1;
	u16 *keys;		//dynamic alloc keys
	u64 *vals;		//dynamic alloc values
	char *skey, *skey1;
	u64 kv;
	int k_m, k_n, k_c, k_cv;
	int i, nk, ndsa;
	
	ndsa=*rndsa;
	for(i=0; i<ndsa; i++)
		if(obj==dsarr[i])
			return(1);
	dsarr[ndsa++]=obj;
	*rndsa=ndsa;
	
	nk=obj->nkey;
	keys=obj->keys;
	vals=obj->vals;

#if 0
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
#endif

#if 1
	for(k_c=0; k_c<nk; k_c++)
	{
		k_cv=keys[k_c];
		if(k_cv==key)
			break;
	}
#endif

	kv=vals[k_c];
	if(k_cv==key)
		return(kv);

	skey=tkmm_lva_syms_name[key];

#if 1
	for(k_c=0; k_c<nk; k_c++)
	{
		k_cv=keys[k_c];
		skey1=tkmm_lva_syms_name[k_cv];
//		if(*skey1=='.')
//		if((skey1[0]=='_') && (skey1[1]!='_'))
//		if(*skey1=='$')
		if((skey1[0]=='$') && (skey1[1]!='$'))
		{
			kv=vals[k_c];
			if(!strcmp(skey1+1, skey))
				return(kv);
			obj1=(LVA_VarObject *)kv;
			kv=TKMM_LVA_VarObjFetchKeyR(obj1, key, dsarr, rndsa);
			if(kv!=1)
				return(kv);
		}
//		if(k_cv==key)
//			break;
	}
#endif

	return(1);
}

__declspec(nocull)
u64 TKMM_LVA_VarObjFetchKey(LVA_VarObject *obj, int key)
{
	LVA_VarObject *dsarr[256];
	int n_dsarr;
	n_dsarr=0;
	return(TKMM_LVA_VarObjFetchKeyR(obj, key, dsarr, &n_dsarr));
}


int TKMM_LVA_VarObjTrySetKeyR(LVA_VarObject *obj, int key, u64 val,
	LVA_VarObject **dsarr, int *rndsa)
{
	LVA_VarObject *obj1;
	u16		*keys;		//dynamic alloc keys
	u64		*vals;		//dynamic alloc values
	char	*skey;
	char	*skey1;
	u64 kv;
	int k_m, k_n, k_c, k_cv;
	int nk, mk, mk1, ndsa;
	int i, j, k;

	ndsa=*rndsa;
	for(i=0; i<ndsa; i++)
		if(obj==dsarr[i])
			return(1);
	dsarr[ndsa++]=obj;
	*rndsa=ndsa;

	nk=obj->nkey;
	keys=obj->keys;
	vals=obj->vals;

#if 0
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
#endif

#if 1
	for(k_c=0; k_c<nk; k_c++)
	{
		k_cv=keys[k_c];
		if(k_cv==key)
			break;
	}
#endif

	if(k_cv==key)
	{
		vals[k_c]=val;
		return(1);
	}

	skey=tkmm_lva_syms_name[key];

#if 1
	for(k_c=0; k_c<nk; k_c++)
	{
		k_cv=keys[k_c];
		skey1=tkmm_lva_syms_name[k_cv];
//		if((skey1[0]=='_') && (skey1[1]!='_'))
		if((skey1[0]=='$') && (skey1[1]!='$'))
		{
			kv=vals[k_c];
			if(!strcmp(skey1+1, skey))
				return(kv);
			obj1=(LVA_VarObject *)kv;
			j=TKMM_LVA_VarObjTrySetKeyR(obj1, key, val, dsarr, rndsa);
			if(j>0)
				return(j);
		}
//		if(k_cv==key)
//			break;
	}
#endif

	return(0);
}

__declspec(nocull)
int TKMM_LVA_VarObjSetKey(LVA_VarObject *obj, int key, u64 val)
{
	LVA_VarObject *dsarr[256];
	u16 *keys;		//dynamic alloc keys
	u64 *vals;		//dynamic alloc values
	int n_dsarr;
	u64 kv;
	int k_m, k_n, k_c, k_cv;
	int nk, mk, mk1;
	int i, j, k;

	n_dsarr=0;
	j=TKMM_LVA_VarObjTrySetKeyR(obj, key, val, dsarr, &n_dsarr);
	if(j>0)
		return(j);

	nk=obj->nkey;
	keys=obj->keys;
	vals=obj->vals;

#if 0

#if 0
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
#endif

#if 1
	for(k_c=0; k_c<nk; k_c++)
	{
		k_cv=keys[k_c];
		if(k_cv==key)
			break;
	}
#endif

	if(k_cv==key)
	{
		vals[k_c]=val;
		return(1);
	}

#endif

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
	
	for(i=nk; i>0; i--)
	{
		if(keys[i-1]<key)
			break;
		keys[i]=keys[i-1];
		vals[i]=vals[i-1];
	}
	keys[i]=key;
	vals[i]=val;
	obj->nkey=nk+1;
	
	return(1);
}

// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
#define LVO_GETSLOT_CST_GEN	__lvo_getslot_cstgen
#define LVO_SETSLOT_CST_GEN	__lvo_setslot_cstgen
__declspec(nocull) __variant
	__lvo_getslot_cstgen(__object obj, char *name, u16 *rfid);
__declspec(nocull) void
	__lvo_setslot_cstgen(__object obj, char *name, u16 *rfid, __variant val);
#endif

LVA_VarObject *TKMM_LVA_AllocVarObj(void)
{
	void *p;
	LVA_VarObject *ov;
	
//	TKMM_LVA_ArrayInit();
//	p=LVO_GETSLOT_CST_GEN;
//	p=LVO_SETSLOT_CST_GEN;
	
	ov=TKMM_LVA_TagMalloc(tkmm_lvatyi_tagobj, sizeof(LVA_VarObject));

	ov->keys=ov->fx_key;
	ov->vals=ov->fx_val;

	ov->nkey=0;
	ov->mkey=0;

	return(ov);
}

__object __lvo_emptyobject(void)
{
	LVA_VarObject *ov;
	u64 v;	

	ov=TKMM_LVA_AllocVarObj();
	v=((u64)ov)|(((u64)tkmm_lvatyi_tagobj)<<48);
	return(__object_frombits(v));
}

// #ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
// #define LVO_GETSLOT_CST_GEN	__lvo_getslot_cstgen
// #define LVO_SETSLOT_CST_GEN	__lvo_setslot_cstgen

__declspec(nocull) __variant
	__lvo_getslot_cst(__object obj, char *name, u16 *rfid);
__declspec(nocull) void
	__lvo_setslot_cst(__object obj, char *name, u16 *rfid, __variant val);

// __variant  __lvo_getslot_cstgen(__object obj, char *name, u16 *rfid);
// void __lvo_setslot_cstgen(__object obj, char *name, u16 *rfid, __variant val);

__asm {
__lvo_getslot_cst:
	SHLD.Q	R4, -48, R17
	MOVU.W	(R6), R16
	CMPEQ	LVA_LVATY_TAGOBJ, R17
	BF		.Generic
	TEST	R16, R16
	BT		.Generic
	
	MOV		R16, R5
	BRA		TKMM_LVA_VarObjFetchKeyA
	
	.Generic:
	BRA		__lvo_getslot_cstgen

__lvo_setslot_cst:
	SHLD.Q	R4, -48, R17
	MOVU.W	(R6), R16
	CMPEQ	LVA_LVATY_TAGOBJ, R17
	BF		.Generic
	TEST	R16, R16
	BT		.Generic
	
	MOV		R16, R5
	MOV		R7, R6
	BRA		TKMM_LVA_VarObjSetKeyA
	
	.Generic:
	BRA		__lvo_setslot_cstgen
};

#else

#define LVO_GETSLOT_CST_GEN	__lvo_getslot_cst
#define LVO_SETSLOT_CST_GEN	__lvo_setslot_cst

#endif

//__variant  __lvo_getslot_cst(__object obj, char *name, u16 *rfid)
__declspec(nocull) __variant
	LVO_GETSLOT_CST_GEN(__object obj, char *name, u16 *rfid)
{
	LVA_ObjectBasic *ob;
	LVA_VarObject *ov;
	u64 objv;
	u64 v;
	int tag, key;

	objv=__object_getbits(obj);
//	if(TKMM_LVA_ObjRefP(objv))
	if(1)
	{
//		tag=TKMM_LVA_GetObjTag(objv);
		tag=(objv>>48);

		if(tag==tkmm_lvatyi_tagobj)
		{
			ov=(void *)objv;
			key=*rfid;
			if(key<=0)
			{
				key=TKMM_LVA_VarObjKeyForSymbol(name);
				*rfid=key;
			}
//#ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
			v=TKMM_LVA_VarObjFetchKeyA(ov, key);
#else
			v=TKMM_LVA_VarObjFetchKey(ov, key);
#endif
//			__debugbreak();
			return(__object_frombits(v));
		}

		if(tag==tkmm_lvatyi_classobj)
		{
			ob=(void *)objv;
			v=TKMM_LVA_TryObjGetFieldVar(ob, name);
			if(v!=LVA_MAGIC_UNDEFINED)
				{ return(__object_frombits(v)); }
			
			if(ob->vo)
			{
				key=*rfid;
				if(key<=0)
				{
					key=TKMM_LVA_VarObjKeyForSymbol(name);
					*rfid=key;
				}

				v=TKMM_LVA_VarObjFetchKey(ob->vo, key);
				return(__object_frombits(v));
			}
			
			v=LVA_MAGIC_UNDEFINED;
			return(__object_frombits(v));
		}
	}
}

//void __lvo_setslot_cst(__object obj, char *name, u16 *rfid, __variant val)
__declspec(nocull) void
	LVO_SETSLOT_CST_GEN(__object obj, char *name, u16 *rfid, __variant val)
{
	LVA_ObjectBasic *ob;
	LVA_VarObject *ov;
	u64 objv, valv;
	int tag, key;

//	__debugbreak();

	objv=__object_getbits(obj);

//	if(TKMM_LVA_ObjRefP(objv))
	if(1)
	{
//		tag=TKMM_LVA_GetObjTag(objv);
		tag=(objv>>48);

		if(tag==tkmm_lvatyi_tagobj)
		{
			valv=__object_getbits(val);
			ov=(void *)objv;
			key=*rfid;
			if(key<=0)
			{
				key=TKMM_LVA_VarObjKeyForSymbol(name);
				*rfid=key;
			}
//#ifdef __BJX2__
#if defined(__BJX2__) && !defined(__ADDR_X96__)
			TKMM_LVA_VarObjSetKeyA(ov, key, valv);
#else
			TKMM_LVA_VarObjSetKey(ov, key, valv);
#endif
//			__debugbreak();
			return;
		}

		if(tag==tkmm_lvatyi_classobj)
		{
			valv=__object_getbits(val);

			ob=(void *)objv;
			if(TKMM_LVA_TryObjSetFieldVar(ob, name, valv)>0)
				return;
			
			if(ob->vo)
			{
				key=*rfid;
				if(key<=0)
				{
					key=TKMM_LVA_VarObjKeyForSymbol(name);
					*rfid=key;
				}
				TKMM_LVA_VarObjSetKey(ob->vo, key, valv);
				return;
			}
			
			return;
		}
	}
}

__variant  __lvo_getslot(__object obj, char *name)
{
	LVA_ObjectBasic *ob;
	LVA_VarObject *ov;
	u64 objv;
	u64 v;
	int tag, key;

	objv=__object_getbits(obj);
	if(TKMM_LVA_ObjRefP(objv))
	{
		tag=TKMM_LVA_GetObjTag(objv);

		if(tag==tkmm_lvatyi_tagobj)
		{
			ov=(void *)objv;
			key=TKMM_LVA_VarObjKeyForSymbol(name);
			v=TKMM_LVA_VarObjFetchKey(ov, key);
//			__debugbreak();
			return(__object_frombits(v));
		}

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
	LVA_VarObject *ov;
	u64 objv, valv;
	int tag, key;

//	__debugbreak();

	objv=__object_getbits(obj);

	if(TKMM_LVA_ObjRefP(objv))
	{
		tag=TKMM_LVA_GetObjTag(objv);

		if(tag==tkmm_lvatyi_tagobj)
		{
			valv=__object_getbits(val);
			ov=(void *)objv;
			key=TKMM_LVA_VarObjKeyForSymbol(name);
			TKMM_LVA_VarObjSetKey(ov, key, valv);
//			__debugbreak();
			return;
		}

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

void *__lva_unwrap(__variant obj)
{
}


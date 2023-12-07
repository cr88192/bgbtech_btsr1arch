LVA_Cons	*tkmm_lva_freecons;
void		*tkmm_lva_freebigint;
void		*tkmm_lva_freebigfloat;

LVA_TagOperator *tkmm_lva_tagbinop;
LVA_TagOperator *tkmm_lva_tagbinop_hash[64];

void TKMM_LVA_RegisterBinOp(int lty, int rty, int opr, void *run)
{
	LVA_TagOperator *tmp;
	u64 ident;
	int hash;
	
	ident=((u64)lty) | (((u64)rty)<<16) | (((u64)opr)<<32);
	hash=((ident * 281281747415761ULL)>>48)&63;;
	
//	hash=((((lty*65521+rty)*65521+opr)*251)>>8)&63;
	
	tmp=tk_malloc(sizeof(LVA_TagOperator));
	tmp->run=run;
	tmp->ident=ident;
//	tmp->lty=lty;
//	tmp->rty=rty;
//	tmp->opr=opr;
	
	tmp->next=tkmm_lva_tagbinop;
	tkmm_lva_tagbinop=tmp;
	tmp->hnext=tkmm_lva_tagbinop_hash[hash];
	tkmm_lva_tagbinop_hash[hash]=tmp;
}

u64 TKMM_LVA_BinaryOpGeneric(u64 vala, u64 valb, int opr)
{
	u64 (*Run)(u64 vala, u64 valb);
	LVA_TagOperator *cur;
	u64 ident;
	int lty, rty, hash;

	lty=TKMM_LVA_ObjrefGetTypeTag(vala);
	rty=TKMM_LVA_ObjrefGetTypeTag(valb);
//	hash=((((lty*65521+rty)*65521+opr)*251)>>8)&63;

	ident=((u64)lty) | (((u64)rty)<<16) | (((u64)opr)<<32);
	hash=((ident * 281281747415761ULL)>>48)&63;
	
	cur=tkmm_lva_tagbinop_hash[hash];
	while(cur)
	{
//		if((cur->lty==tga) && (cur->rty==tgb) && (cur->opr==opr))
		if(cur->ident==ident)
			break;
		cur=cur->next;
	}
	
	if(cur)
	{
		Run=cur->run;
		return(Run(vala, valb));
	}

	return(LVA_MAGIC_UNDEFINED);
}

u64 TKMM_LVA_UnaryOpGeneric(u64 vala, int opr)
{
	return(TKMM_LVA_BinaryOpGeneric(vala, 0, opr));
}



LVA_Cons *TKMM_LVA_AllocCons()
{
	LVA_Cons *tmp;
	
	tmp=tkmm_lva_freecons;
	if(tmp)
	{
		tkmm_lva_freecons=(LVA_Cons *)(tmp->cdr);
		return(tmp);
	}

	tmp=TKMM_LVA_TagMalloc(tkmm_lvatyi_cons, sizeof(LVA_Cons));
	return(tmp);

//	return(TKMM_LVA_NewTagArray(n, 5));
}

LVA_Cons *TKMM_LVA_NewCons(u64 car, u64 cdr)
{
	LVA_Cons *tmp;
	
	tmp=tkmm_lva_freecons;
	if(tmp)
	{
		tkmm_lva_freecons=(LVA_Cons *)(tmp->cdr);
		tmp->car=car;
		tmp->cdr=cdr;
		return(tmp);
	}

	tmp=TKMM_LVA_TagMalloc(tkmm_lvatyi_cons, sizeof(LVA_Cons));
	tmp->car=car;
	tmp->cdr=cdr;
	return(tmp);
}

void TKMM_LVA_FreeCons(LVA_Cons *tmp)
{
	tmp->cdr=(u64)tkmm_lva_freecons;
	tkmm_lva_freecons=tmp;
}

void TKMM_LVA_FreeConsList(LVA_Cons *lst)
{
	LVA_Cons *cur, *nxt;

	cur=lst;
	while(cur)
	{
		nxt=(LVA_Cons *)(cur->cdr);
		cur->cdr=(u64)tkmm_lva_freecons;
		tkmm_lva_freecons=cur;
		cur=nxt;
	}
}

int TKMM_LVA_GetConsListLength(LVA_Cons *lst)
{
	LVA_Cons *cur;
	int n;

	cur=lst; n=0;
	while(cur)
	{
		n++;
		cur=(LVA_Cons *)(cur->cdr);
	}
	return(n);
}

tk_lva_object __lvo_newvararray_nlist(tk_lva_variant vlst)
{
	LVA_TagArray *arr;
	LVA_Cons *lst, *cur;
	u64 *arrd;
	int i, na;
	
	lst=(LVA_Cons *)(__object_getbits(vlst));
	na=TKMM_LVA_GetConsListLength(lst);
	
	arr=TKMM_LVA_NewTagArray(na, 5);
	arrd=arr->data;

	cur=lst;
	for(i=0; i<na; i++)
	{
		arrd[i]=cur->car;
		cur=(LVA_Cons *)(cur->cdr);
	}
	
	TKMM_LVA_FreeConsList(lst);
	
	return(TKMM_LVA_WrapVarArray(arr));
}

void *TKMM_LVA_AllocBigInt()
{
	void *tmp;
	
	tmp=tkmm_lva_freebigint;
	if(tmp)
	{
		tkmm_lva_freebigint=*(void **)tmp;
		return(tmp);
	}

	tmp=TKMM_LVA_TagMalloc(LVA_LVATY_BIGINT, 48);
	return(tmp);
}

u64 TKMM_LVA_NewBigInt(__int128 val)
{
	void *ptr;
	__int128 valsx;
	u64 vptr;
	
	valsx=0;
	if(val<0)
		valsx=-1;
	
	ptr=TKMM_LVA_AllocBigInt();
//	*(__int128 *)ptr=val;
	((__int128 *)ptr)[0]=val;
	((__int128 *)ptr)[1]=valsx;
	((__int128 *)ptr)[2]=valsx;
	vptr=(u64)ptr;
	vptr|=((u64)LVA_LVATY_BIGINT)<<48;
	return(vptr);
}

#ifdef __BJX2__
u64 TKMM_LVA_NewBigInt3(__uint128 vala, __uint128 valb, __uint128 valc)
{
	void *ptr;
	u64 vptr;
	ptr=TKMM_LVA_AllocBigInt();
	((__int128 *)ptr)[0]=vala;
	((__int128 *)ptr)[1]=valb;
	((__int128 *)ptr)[2]=valc;
	vptr=(u64)ptr;
	vptr|=((u64)LVA_LVATY_BIGINT)<<48;
	return(vptr);
}
#endif

u64 TKMM_LVA_NewBigInt3v(void *pval)
{
	void *ptr;
	u64 vptr;
	ptr=TKMM_LVA_AllocBigInt();
	memcpy(ptr, pval, 48);
//	((__int128 *)ptr)[0]=pval[0];
//	((__int128 *)ptr)[1]=pval[1];
//	((__int128 *)ptr)[2]=pval[2];
	vptr=(u64)ptr;
	vptr|=((u64)LVA_LVATY_BIGINT)<<48;
	return(vptr);
}

void TKMM_LVA_UnwrapBigIntPv(void *pval, u64 val)
{
	int tag;
	s64 iv, ivx;

	if((val>>62)==1)
	{
		iv=val<<2;
		iv=iv>>2;
		ivx=iv>>63;
		((u64 *)pval)[0]=iv;
		((u64 *)pval)[1]=ivx;
		((u64 *)pval)[2]=ivx;
		((u64 *)pval)[3]=ivx;
		((u64 *)pval)[4]=ivx;
		((u64 *)pval)[5]=ivx;
		return;
	}

	tag=val>>48;
	if(tag==LVA_LVATY_BIGINT)
	{
		memcpy(pval, (void *)val, 48);
		return;
	}
	
	return;
}


u64	tkmm_lva_opradd_bigi(u64 lva, u64 lvb)
{
	u64 tlvc[6];
	tkmm_bigint_add((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));
//	__int128 va, vb, vc;
//	va=*(__int128 *)lva;
//	vb=*(__int128 *)lvb;
//	vc=va+vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_oprsub_bigi(u64 lva, u64 lvb)
{
	u64 tlvc[6];
	tkmm_bigint_add((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=*(__int128 *)lva;
//	vb=*(__int128 *)lvb;
//	vc=va-vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_opradd_bigig(u64 lva, u64 lvb)
{
	u64 tlva[6], tlvb[6], tlvc[6];
	TKMM_LVA_UnwrapBigIntPv(tlva, lva);
	TKMM_LVA_UnwrapBigIntPv(tlvb, lvb);
	tkmm_bigint_add((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=__lva_conv_toi128(lva);
//	vb=__lva_conv_toi128(lvb);
//	vc=va+vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_oprsub_bigig(u64 lva, u64 lvb)
{
	u64 tlva[6], tlvb[6], tlvc[6];
	TKMM_LVA_UnwrapBigIntPv(tlva, lva);
	TKMM_LVA_UnwrapBigIntPv(tlvb, lvb);
	tkmm_bigint_sub((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=__lva_conv_toi128(lva);
//	vb=__lva_conv_toi128(lvb);
//	vc=va-vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_oprmul_bigig(u64 lva, u64 lvb)
{
#ifdef __BGBCC__
	__int128 va, vb, vc;
	va=__lva_conv_toi128(lva);
	vb=__lva_conv_toi128(lvb);
	vc=va*vb;
	return(TKMM_LVA_NewBigInt(vc));
#endif
}

u64	tkmm_lva_oprdiv_bigig(u64 lva, u64 lvb)
{
#ifdef __BGBCC__
	__int128 va, vb, vc;
	va=__lva_conv_toi128(lva);
	vb=__lva_conv_toi128(lvb);
	vc=va/vb;
	return(TKMM_LVA_NewBigInt(vc));
#endif
}

u64	tkmm_lva_oprmod_bigig(u64 lva, u64 lvb)
{
#ifdef __BGBCC__
	__int128 va, vb, vc;
	va=__lva_conv_toi128(lva);
	vb=__lva_conv_toi128(lvb);
	vc=va%vb;
	return(TKMM_LVA_NewBigInt(vc));
#endif
}

u64	tkmm_lva_oprand_bigig(u64 lva, u64 lvb)
{
	u64 tlva[6], tlvb[6], tlvc[6];
	TKMM_LVA_UnwrapBigIntPv(tlva, lva);
	TKMM_LVA_UnwrapBigIntPv(tlvb, lvb);
	tkmm_bigint_and((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=__lva_conv_toi128(lva);
//	vb=__lva_conv_toi128(lvb);
//	vc=va&vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_opror_bigig(u64 lva, u64 lvb)
{
	u64 tlva[6], tlvb[6], tlvc[6];
	TKMM_LVA_UnwrapBigIntPv(tlva, lva);
	TKMM_LVA_UnwrapBigIntPv(tlvb, lvb);
	tkmm_bigint_or((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=__lva_conv_toi128(lva);
//	vb=__lva_conv_toi128(lvb);
//	vc=va|vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

u64	tkmm_lva_oprxor_bigig(u64 lva, u64 lvb)
{
	u64 tlva[6], tlvb[6], tlvc[6];
	TKMM_LVA_UnwrapBigIntPv(tlva, lva);
	TKMM_LVA_UnwrapBigIntPv(tlvb, lvb);
	tkmm_bigint_xor((void *)lva, (void *)lvb, tlvc);
	return(TKMM_LVA_NewBigInt3v(tlvc));

//	__int128 va, vb, vc;
//	va=__lva_conv_toi128(lva);
//	vb=__lva_conv_toi128(lvb);
//	vc=va^vb;
//	return(TKMM_LVA_NewBigInt(vc));
}

LVA_VarObject *TKMM_LVA_AllocVarObj(void);

void TKMM_LVA_InitTagOpr()
{
	void *p;
	
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_ADD,
		tkmm_lva_opradd_bigi);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_SUB,
		tkmm_lva_oprsub_bigi);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_MUL,
		tkmm_lva_oprmul_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_DIV,
		tkmm_lva_oprdiv_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_MOD,
		tkmm_lva_oprmod_bigig);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_AND,
		tkmm_lva_oprand_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_OR,
		tkmm_lva_opror_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_BIGINT, LVA_OPR_XOR,
		tkmm_lva_oprxor_bigig);


	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_ADD,
		tkmm_lva_opradd_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_SUB,
		tkmm_lva_oprsub_bigig);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_MUL,
		tkmm_lva_oprmul_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_DIV,
		tkmm_lva_oprdiv_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_MOD,
		tkmm_lva_oprmod_bigig);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_AND,
		tkmm_lva_oprand_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_OR,
		tkmm_lva_opror_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_BIGINT, LVA_LVATY_FIXNUM, LVA_OPR_XOR,
		tkmm_lva_oprxor_bigig);


	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_ADD,
		tkmm_lva_opradd_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_SUB,
		tkmm_lva_oprsub_bigig);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_MUL,
		tkmm_lva_oprmul_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_DIV,
		tkmm_lva_oprdiv_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_MOD,
		tkmm_lva_oprmod_bigig);

	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_AND,
		tkmm_lva_oprand_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_OR,
		tkmm_lva_opror_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_BIGINT, LVA_OPR_XOR,
		tkmm_lva_oprxor_bigig);

#if 0
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_FIXNUM, LVA_OPR_ADD,
		tkmm_lva_opradd_bigig);
	TKMM_LVA_RegisterBinOp(
		LVA_LVATY_FIXNUM, LVA_LVATY_FIXNUM, LVA_OPR_SUB,
		tkmm_lva_oprsub_bigig);
#endif
}

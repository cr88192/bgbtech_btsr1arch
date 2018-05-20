#include <bgbccc.h>

dytf *bgbgc_conschunk[1024];
int bgbgc_conschunk_num=0;
int bgbgc_conschunk_idx=0;

dytf *bgbgc_consfree=NULL;

BGBCC_API dytf *BGBCC_AllocCons()
{
	dytf *p;
	
	if(bgbgc_consfree)
	{
		p=bgbgc_consfree;
		bgbgc_consfree=*(dytf **)p;
		return(p);
	}
	
	if(bgbgc_conschunk_idx>=65536)
	{
		bgbgc_conschunk_num++;
		bgbgc_conschunk_idx=0;
	}
	
	if(!bgbgc_conschunk[bgbgc_conschunk_num])
	{
		bgbgc_conschunk[bgbgc_conschunk_num]=
			malloc((1<<16)*2*sizeof(dytf));
		bgbgc_conschunk_idx=0;
	}
	
	p=bgbgc_conschunk[bgbgc_conschunk_num]+
		(2*bgbgc_conschunk_idx);
	bgbgc_conschunk_idx++;
	return(p);
}

BGBCC_API void BGBCC_FreeCons(dytf *ptr)
{
	*(dytf **)ptr=bgbgc_consfree;
	bgbgc_consfree=ptr;
}

BGBCC_API bool BGBCC_ConsP(void *ptr)
{
	dytf *p, *ps, *pe;
	int i;
	
	p=(dytf *)ptr;
	for(i=0; i<=bgbgc_conschunk_num; i++)
	{
		ps=bgbgc_conschunk[i];
		if(!ps)continue;
		pe=ps+((1<<16)*2);
		if((p>=ps) && (p<pe))
			return(true);
	}
	return(false);
}

BGBCC_API dytf dytfCons(dytf a, dytf b)
{
	dytf *p;

	p=BGBCC_AllocCons();
	p[0]=a;	p[1]=b;
	return(dytfPtrF(p));
}

BGBCC_API bool dytfConsP(dytf p)
{
	return(BGBCC_ConsP(dytfPtrvF(p)));
}

BGBCC_API dytf dytfCar(dytf p)
{
	dytf *q;
//	if(dytfEqqP(p, DYTF_NULL) || dytfEqqP(p, DYTF_UNDEFINED))
	if(dytfNullP(p) || dytfUndefinedP(p))
		return(DYTF_UNDEFINED);
	q=(dytf *)dytfPtrvF(p);
	return(q[0]);
}

BGBCC_API dytf dytfCdr(dytf p)
{
	dytf *q;
//	if(dytfEqqP(p, DYTF_NULL) || dytfEqqP(p, DYTF_UNDEFINED))
	if(dytfNullP(p) || dytfUndefinedP(p))
		return(DYTF_UNDEFINED);
	q=(dytf *)dytfPtrvF(p);
	return(q[1]);
}

BGBCC_API dytf dytfCarF(dytf p)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	return(q[0]);
}

BGBCC_API dytf dytfCdrF(dytf p)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	return(q[1]);
}

BGBCC_API void dytfSetCar(dytf p, dytf v)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	q[0]=v;
}

BGBCC_API void dytfSetCdr(dytf p, dytf v)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	q[1]=v;
}

BGBCC_API void dytfSetCarF(dytf p, dytf v)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	q[0]=v;
}

BGBCC_API void dytfSetCdrF(dytf p, dytf v)
{
	dytf *q;
	q=(dytf *)dytfPtrvF(p);
	q[1]=v;
}

BGBCC_API bool dytfListP(dytf lst)
{
	dytf cur;

	cur=lst;
	while(dytfConsP(cur))
		cur=dytfCdr(cur);
	return(dytfEqqP(cur, DYTF_NULL));
}

BGBCC_API int dytfListLength(dytf lst)
{
	dytf cur;
	int i;

	cur=lst; i=0;
	while(dytfConsP(cur))
		 { i++; cur=dytfCdr(cur); }
	return(i);
}

BGBCC_API void dytfFreeCons(dytf p)
{
	BGBCC_FreeCons(dytfPtrvF(p));
}

BGBCC_API void dytfFreeList(dytf p)
{
	dytf c, n;

	c=p;
	while(dytfConsP(c))
	{
		n=dytfCdrF(c);
		dytfFreeCons(c);
		c=n;
	}
}

BGBCC_API void dytfFreeListF(dytf p)
{
	dytf c, n;

	c=p;
	while(!dytfNullP(c))
	{
		n=dytfCdrF(c);
		dytfFreeCons(c);
		c=n;
	}
}

BGBCC_API void dytfFreeListR(dytf p)
{
	dytf c, n;

	c=p;
	while(dytfConsP(c))
	{
		n=dytfCdrF(c);
		dytfFreeListR(dytfCarF(c));
		dytfFreeCons(c);
		c=n;
	}
}

BGBCC_API bool dytfFormIsP(dytf lst, char *str)
{
	if(!dytfConsP(lst))return(false);
	if(dytfEqqP(dytfCarF(lst), dytfSymbol(str)))
		return(true);
	return(false);
}


BGBCC_API dytf dytfCaar(dytf p) { return(dytfCar(dytfCar(p))); }
BGBCC_API dytf dytfCdar(dytf p) { return(dytfCdr(dytfCar(p))); }
BGBCC_API dytf dytfCadr(dytf p) { return(dytfCar(dytfCdr(p))); }
BGBCC_API dytf dytfCddr(dytf p) { return(dytfCdr(dytfCdr(p))); }
BGBCC_API dytf dytfCaaar(dytf p) { return(dytfCar(dytfCaar(p))); }
BGBCC_API dytf dytfCdaar(dytf p) { return(dytfCdr(dytfCaar(p))); }
BGBCC_API dytf dytfCadar(dytf p) { return(dytfCar(dytfCdar(p))); }
BGBCC_API dytf dytfCddar(dytf p) { return(dytfCdr(dytfCdar(p))); }
BGBCC_API dytf dytfCaadr(dytf p) { return(dytfCar(dytfCadr(p))); }
BGBCC_API dytf dytfCdadr(dytf p) { return(dytfCdr(dytfCadr(p))); }
BGBCC_API dytf dytfCaddr(dytf p) { return(dytfCar(dytfCddr(p))); }
BGBCC_API dytf dytfCdddr(dytf p) { return(dytfCdr(dytfCddr(p))); }
BGBCC_API dytf dytfCaaaar(dytf p) { return(dytfCar(dytfCaaar(p))); }
BGBCC_API dytf dytfCdaaar(dytf p) { return(dytfCdr(dytfCaaar(p))); }
BGBCC_API dytf dytfCadaar(dytf p) { return(dytfCar(dytfCdaar(p))); }
BGBCC_API dytf dytfCddaar(dytf p) { return(dytfCdr(dytfCdaar(p))); }
BGBCC_API dytf dytfCaadar(dytf p) { return(dytfCar(dytfCadar(p))); }
BGBCC_API dytf dytfCdadar(dytf p) { return(dytfCdr(dytfCadar(p))); }
BGBCC_API dytf dytfCaddar(dytf p) { return(dytfCar(dytfCddar(p))); }
BGBCC_API dytf dytfCdddar(dytf p) { return(dytfCdr(dytfCddar(p))); }
BGBCC_API dytf dytfCaaadr(dytf p) { return(dytfCar(dytfCaadr(p))); }
BGBCC_API dytf dytfCdaadr(dytf p) { return(dytfCdr(dytfCaadr(p))); }
BGBCC_API dytf dytfCadadr(dytf p) { return(dytfCar(dytfCdadr(p))); }
BGBCC_API dytf dytfCddadr(dytf p) { return(dytfCdr(dytfCdadr(p))); }
BGBCC_API dytf dytfCaaddr(dytf p) { return(dytfCar(dytfCaddr(p))); }
BGBCC_API dytf dytfCdaddr(dytf p) { return(dytfCdr(dytfCaddr(p))); }
BGBCC_API dytf dytfCadddr(dytf p) { return(dytfCar(dytfCdddr(p))); }
BGBCC_API dytf dytfCddddr(dytf p) { return(dytfCdr(dytfCdddr(p))); }

BGBCC_API dytf dytfCaaaaar(dytf p) { return(dytfCar(dytfCaaaar(p))); }
BGBCC_API dytf dytfCdaaaar(dytf p) { return(dytfCdr(dytfCaaaar(p))); }
BGBCC_API dytf dytfCadaaar(dytf p) { return(dytfCar(dytfCdaaar(p))); }
BGBCC_API dytf dytfCddaaar(dytf p) { return(dytfCdr(dytfCdaaar(p))); }
BGBCC_API dytf dytfCaadaar(dytf p) { return(dytfCar(dytfCadaar(p))); }
BGBCC_API dytf dytfCdadaar(dytf p) { return(dytfCdr(dytfCadaar(p))); }
BGBCC_API dytf dytfCaddaar(dytf p) { return(dytfCar(dytfCddaar(p))); }
BGBCC_API dytf dytfCdddaar(dytf p) { return(dytfCdr(dytfCddaar(p))); }
BGBCC_API dytf dytfCaaadar(dytf p) { return(dytfCar(dytfCaadar(p))); }
BGBCC_API dytf dytfCdaadar(dytf p) { return(dytfCdr(dytfCaadar(p))); }
BGBCC_API dytf dytfCadadar(dytf p) { return(dytfCar(dytfCdadar(p))); }
BGBCC_API dytf dytfCddadar(dytf p) { return(dytfCdr(dytfCdadar(p))); }
BGBCC_API dytf dytfCaaddar(dytf p) { return(dytfCar(dytfCaddar(p))); }
BGBCC_API dytf dytfCdaddar(dytf p) { return(dytfCdr(dytfCaddar(p))); }
BGBCC_API dytf dytfCadddar(dytf p) { return(dytfCar(dytfCdddar(p))); }
BGBCC_API dytf dytfCddddar(dytf p) { return(dytfCdr(dytfCdddar(p))); }
BGBCC_API dytf dytfCaaaadr(dytf p) { return(dytfCar(dytfCaaadr(p))); }
BGBCC_API dytf dytfCdaaadr(dytf p) { return(dytfCdr(dytfCaaadr(p))); }
BGBCC_API dytf dytfCadaadr(dytf p) { return(dytfCar(dytfCdaadr(p))); }
BGBCC_API dytf dytfCddaadr(dytf p) { return(dytfCdr(dytfCdaadr(p))); }
BGBCC_API dytf dytfCaadadr(dytf p) { return(dytfCar(dytfCadadr(p))); }
BGBCC_API dytf dytfCdadadr(dytf p) { return(dytfCdr(dytfCadadr(p))); }
BGBCC_API dytf dytfCaddadr(dytf p) { return(dytfCar(dytfCddadr(p))); }
BGBCC_API dytf dytfCdddadr(dytf p) { return(dytfCdr(dytfCddadr(p))); }
BGBCC_API dytf dytfCaaaddr(dytf p) { return(dytfCar(dytfCaaddr(p))); }
BGBCC_API dytf dytfCdaaddr(dytf p) { return(dytfCdr(dytfCaaddr(p))); }
BGBCC_API dytf dytfCadaddr(dytf p) { return(dytfCar(dytfCdaddr(p))); }
BGBCC_API dytf dytfCddaddr(dytf p) { return(dytfCdr(dytfCdaddr(p))); }
BGBCC_API dytf dytfCaadddr(dytf p) { return(dytfCar(dytfCadddr(p))); }
BGBCC_API dytf dytfCdadddr(dytf p) { return(dytfCdr(dytfCadddr(p))); }
BGBCC_API dytf dytfCaddddr(dytf p) { return(dytfCar(dytfCddddr(p))); }
BGBCC_API dytf dytfCdddddr(dytf p) { return(dytfCdr(dytfCddddr(p))); }

BGBCC_API dytf dytfList1(dytf a)
	{ return(dytfCons(a, DYTF_NULL)); }
BGBCC_API dytf dytfList2(dytf a, dytf b)
	{ return(dytfCons(a, dytfList1(b))); }
BGBCC_API dytf dytfList3(dytf a, dytf b, dytf c)
	{ return(dytfCons(a, dytfList2(b, c))); }
BGBCC_API dytf dytfList4(dytf a, dytf b, dytf c, dytf d)
	{ return(dytfCons(a, dytfList3(b, c, d))); }
BGBCC_API dytf dytfList5(dytf a, dytf b, dytf c, dytf d, dytf e)
	{ return(dytfCons(a, dytfList4(b, c, d, e))); }
BGBCC_API dytf dytfList6(dytf a, dytf b, dytf c, dytf d, dytf e, dytf f)
	{ return(dytfCons(a, dytfList5(b, c, d, e, f))); }

BGBCC_API dytf dytfList1s(char *a)
	{ return(dytfCons(dytfSymbol(a), DYTF_NULL)); }
BGBCC_API dytf dytfList2s(char *a, dytf b)
	{ return(dytfCons(dytfSymbol(a), dytfList1(b))); }
BGBCC_API dytf dytfList3s(char *a, dytf b, dytf c)
	{ return(dytfCons(dytfSymbol(a), dytfList2(b, c))); }
BGBCC_API dytf dytfList4s(char *a, dytf b, dytf c, dytf d)
	{ return(dytfCons(dytfSymbol(a), dytfList3(b, c, d))); }
BGBCC_API dytf dytfList5s(char *a, dytf b, dytf c, dytf d, dytf e)
	{ return(dytfCons(dytfSymbol(a), dytfList4(b, c, d, e))); }
BGBCC_API dytf dytfList6s(char *a, dytf b, dytf c, dytf d, dytf e, dytf f)
	{ return(dytfCons(dytfSymbol(a), dytfList5(b, c, d, e, f))); }

BGBCC_API dytf dytfCons2(dytf a, dytf b, dytf c)
	{ return(dytfCons(a, dytfCons(b, c))); }
BGBCC_API dytf dytfCons3(dytf a, dytf b, dytf c, dytf d)
	{ return(dytfCons(a, dytfCons2(b, c, d))); }
BGBCC_API dytf dytfCons4(dytf a, dytf b, dytf c, dytf d, dytf e)
	{ return(dytfCons(a, dytfCons3(b, c, d, e))); }

BGBCC_API dytf dytfConss(char *a, dytf b)
	{ return(dytfCons(dytfSymbol(a), b)); }
BGBCC_API dytf dytfCons2s(char *a, dytf b, dytf c)
	{ return(dytfCons(dytfSymbol(a), dytfCons(b, c))); }


BGBCC_API dytf dytfReverse(dytf lst)
{
	dytf c, l, n;

	c=lst; l=DYTF_NULL;
	while(dytfConsP(c))
	{
		l=dytfCons(dytfCarF(c), l);
		c=dytfCdrF(c);
	}
	return(l);
}

BGBCC_API dytf dytfNReverse(dytf lst)
{
	dytf c, l, n;

	c=lst; l=DYTF_NULL;
	while(dytfConsP(c))
	{
		n=dytfCdrF(c);
		dytfSetCdr(c, l);
		l=c; c=n;
	}
	return(l);
}

BGBCC_API dytf dytfNAppend(dytf la, dytf lb)
{
	dytf c, l, n;

	if(dytfNullP(la))
		return(lb);
	if(!dytfConsP(la))
		return(lb);

	c=la; l=DYTF_NULL;
	while(dytfConsP(c))
		{ l=c; c=dytfCdrF(c); }
	if(dytfNullP(l))
		return(lb);
	dytfSetCdr(l, lb);
	return(la);
}

#if 0
// BGBCC_API dytf dytfListToArray(dytf lst)
{
	dytf c, l;
	int i;

	c=lst; i=0;
	while(dytfConsP(c)) { i++; c=dytfCdrF(c); }

	l=dytfWrap(dyarray(i));

	c=lst; i=0;
	while(dytfConsP(c))
	{
		dytfArraySetIdx(l, i, dytfCarF(c));
		i++;
		c=dytfCdrF(c);
	}

	return(l);
}
#endif

BGBCC_API dytf dytfListCopy(dytf lst)
{
	dytf cur, tlst;

	cur=lst; tlst=DYTF_NULL;
	while(dytfConsP(cur))
	{
		tlst=dytfCons(dytfCarF(cur), tlst);
		cur=dytfCdrF(cur);
	}

	tlst=dytfNReverse(tlst);
	return(tlst);
}

BGBCC_API dytf dytfListCopyR(dytf lst)
{
	dytf cur, tlst;

	if(!dytfConsP(lst))
		return(lst);

	cur=lst; tlst=DYTF_NULL;
	while(dytfConsP(cur))
	{
//		gcSet(tlst, dycons(dyListCopyR(dycar(cur)), tlst));
		tlst=dytfCons(dytfListCopyR(dytfCarF(cur)), tlst);
		cur=dytfCdrF(cur);
	}

//	gcSet(tlst, dynreverse(tlst));
	tlst=dytfNReverse(tlst);
	return(tlst);
}

BGBCC_API dytf dytfAppend(dytf la, dytf lb)
{
	dytf c, l, n;

	if(dytfNullP(la))return(dytfListCopy(lb));
	if(dytfNullP(lb))return(dytfListCopy(la));
	return(dytfNAppend(dytfListCopy(la), dytfListCopy(lb)));
}

BGBCC_API bool dytfListEqualP(dytf la, dytf lb)
{
	dytf ca, cb;

	if(dytfEqqP(la, lb))
		return(true);
	if(!dytfConsP(la) || !dytfConsP(lb))
		return(false);

	ca=la; cb=lb;
	while(dytfConsP(ca) && dytfConsP(cb))
	{
		if(!dytfListEqualP(dytfCarF(ca), dytfCarF(cb)))
			return(false);
		ca=dytfCdrF(ca); cb=dytfCdrF(cb);
	}
	if(dytfNEqqP(ca, cb))
		return(false);
	return(true);
}

BGBCC_API bool dytfListEqvP(dytf la, dytf lb)
{
	dytf ca, cb;

	if(dytfEqqP(la, lb))
		return(true);
	if(!dytfConsP(la) || !dytfConsP(lb))
		return(dytfEqvP(la, lb));

	ca=la; cb=lb;
	while(dytfConsP(ca) && dytfConsP(cb))
	{
		if(!dytfEqvP(dytfCarF(ca), dytfCarF(cb)))
			return(false);
		ca=dytfCdrF(ca); cb=dytfCdrF(cb);
	}
	return(dytfEqvP(ca, cb));
}

static void *dytfPtrvF(dytf a)
{
	return((void *)((nlint)(a.v)));
}

static dytf dytfPtrF(void *p)
{
	dytf l;
	l.v=(u64)((nlint)p);
	return(l);
}

static bool dytfPtrP(dytf a)
{
//	int t;
//	t=(int)((a.v>>60)&15);
//	return((0x8001>>t)&1);
	return((bool)((0x8001>>((int)((a.v>>60)&15)))&1));
}

static bool dytfNullP(dytf a)
{
	u64 l;
	l=*(u64 *)(&a);
	if(!l)return(true);
	return(false);
}

static bool dytfUndefinedP(dytf a)
{
	u64 l;
	l=*(u64 *)(&a);
	if(l==((u64)((nlint)UNDEFINED)))
		return(true);
	return(false);
}

static bool dytfNullOrUndefinedP(dytf a)
{
	u64 l;
	l=a.v;
	if(!l)return(true);
	if(l==((u64)((nlint)UNDEFINED)))
		return(true);
	return(false);
}


static bool dytfFixintP(dytf a)
{
	u64 t, u, v;
	if((0x4002>>((int)((a.v>>60)&15)))&1)
	{
		t=(u64)((s64)((s32)(a.v)));
		u=t&0x0FFFFFFF00000000ULL;
		v=a.v&0x0FFFFFFF00000000ULL;
		return(u==v);
	}
	return(false);

//	int t;
//	t=(int)((a.v>>60)&15);
//	return((0x6006>>t)&1);
//	return((bool)((0x6006>>((int)((a.v>>60)&15)))&1));
}

static bool dytfFixlongP(dytf a)
{
	return((bool)((0x6006>>((int)((a.v>>60)&15)))&1));
}

static bool dytfFixnumP(dytf a)
{
	return((bool)((0x6006>>((int)((a.v>>60)&15)))&1));
}

static bool dytfEqqP(dytf a, dytf b)
{
	return(a.v==b.v);
}

static bool dytfNEqqP(dytf a, dytf b)
{
	return(a.v!=b.v);
}

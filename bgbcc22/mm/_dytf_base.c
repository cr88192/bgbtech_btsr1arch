#include <bgbccc.h>

/*
Replacement for dytf.

64-bit value, with high 4 bits as a tag.

High 4:
	0/15=Pointer;
	1/2/13/14=Fixnum;
	3/4/11/12=Flonum1;
	// 5/10=Flonum2;
	6="Spaces";
	7=Tag2;
	8=Flonum2
	9=Resv

Pointer: 60 bit object-pointer.
Fixnum: 62 bit integer.
Flonum1: double encoded directly (values where -256<exp<256 can be encoded exactly)
Flonum2: double with 3 bits shaved off.
Tag2: 4/12/28-more tag bits, with 56/48/32-bit spaces.
"Spaces": related to how tagged-values are implemented on x86-64 via the "magic pointer" system, but is mostly N/A for x86.

possibly coded via tag2: int32 (sub-case), float32 (sub-case), char32, ...

*/

BGBCC_API dytf dytfWrap(dyt a)
{
	u64 l;
	double f;

#if 0
	if(BGBDY_IsFixnumFP((byte *)a))
	{
		f=BGBDY_UnwrapIntFast(a);
		return(dytfInt((s64)f));
	}
	if(BGBDY_IsFlonumFP((byte *)a))
	{
		f=BGBDY_UnwrapMiniFloat(a);
		return(dytfReal(f));
	}
#endif

#ifdef X86
	l=((u32)a);
	return(*(dytf *)(&l));
//	return((dytf)l);
#else
#ifdef X86_64
#if 0
	if(gcllSpaceP(a))
	{
		l=gcllSpaceAbsoluteOffset((void *)a);
		l=0x6000000000000000ULL | l;
		return(*(dytf *)(&l));
	}
#endif

	if(((s64)a)>=0)
	{
		l=((u64)a)&0x0FFFFFFFFFFFFFFFULL;
	}
	else
	{
		l=0xF000000000000000ULL | ((u64)a)&0x0FFFFFFFFFFFFFFFULL;
	}
	return(*(dytf *)(&l));
#else
	l=0x0000000000000000ULL;
	return(*(dytf *)(&l));
#endif
#endif
}

BGBCC_API dyt dytfUnwrap(dytf a)
{
	u64 l;
	s64 v;
	dyt p;
	double f;

	l=*(u64 *)(&a);
//	l=a;

#if 0
//	if(dytfRealP(a))
	if(dytfFlonumP(a))
	{
		f=dytfRealv(a);
		return(BGBDY_WrapFlonum(f));
	}
#endif

	switch(((int)(l>>60))&15)
	{
	case 0: case 15:
		p=(dyt)((nlint)l);
		break;
	case 1: case 2:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
		v=(s64)l-0x1000000000000000LL;
//		p=BGBDY_WrapInt64(v);
		break;
	case 13: case 14:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
//		v|=(-1LL)<<61;
		v=((s64)l)-0xF000000000000000LL;
//		p=BGBDY_WrapInt64(v);
		break;

	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
//		p=BGBDY_WrapFlonum(f);
		break;
	case 8:
		l=l<<4;
		f=*(double *)(&l);
//		p=BGBDY_WrapFlonum(f);
		break;

	case 6:
		v=l&0x0FFFFFFFFFFFFFFFULL;
//		p=gcllSpaceWrapAbsolute(v);
		break;
#if 0
	case 7:
		switch(((int)(l>>56))&15)
		{
		case 0: p=dywvec2(dytfPackVec2v(a)); break;
		case 1: p=dywvec3(dytfPackVec3v(a)); break;
		case 2: p=dywvec4(dytfPackVec4v(a)); break;
		case 3: p=dywquat(dytfPackQuatv(a)); break;

		case 8: case 9: case 11: case 12:
			switch(((int)(l>>48))&0x3FF)
			{
			case 0x000: p=dywvec2(dytfPackVec2v(a)); break;
			case 0x001: p=dywvec3(dytfPackVec3v(a)); break;
			case 0x002: p=dywvec4(dytfPackVec4v(a)); break;
			case 0x003: p=dywquat(dytfPackQuatv(a)); break;
			default:
				*(int *)-1=-1;
				p=UNDEFINED;
				break;
			}
			break;
		default:
			*(int *)-1=-1;
			p=UNDEFINED;
			break;
		}
		break;
#endif
	default:
		*(int *)-1=-1;
		p=UNDEFINED;
		break;
	}
	return(p);
}


BGBCC_API int dytfFlonumP(dytf a)
{
	static int bits=0x1918;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	return((bits>>t)&1);
}

BGBCC_API bool dytfFixrealP(dytf a)
{
	static int bits=0x791E;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	return((bits>>t)&1);
}

BGBCC_API int dytfRealP(dytf a)
{
//	static int bits=0x1918;
//	u64 l;
//	int t, r;
	char *s;

//	l=*(u64 *)(&a);
//	t=(int)((l>>60)&15);
//	return((bits>>t)&1);

	if(dytfFixrealP(a))
		return(true);
	if(dytfPtrP(a))
	{
		s=dytfgettype(a);
		if(!strcmp(s, "_int_t"))
			return(true);
		if(!strcmp(s, "_long_t"))
			return(true);
		if(!strcmp(s, "_float_t"))
			return(true);
		if(!strcmp(s, "_double_t"))
			return(true);
	}
	return(false);
#if 0
	switch(t)
	{
	case 3: case 4:
	case 11: case 12:
		r=1; break;
//	case 5: case 10:
	case 8:
		r=1; break;
	default: r=0; break;
	}
	return(r);
#endif
}

BGBCC_API int dytfFloatP(dytf a)
{
	if(dytfFlonumP(a))
		return(true);
	return(dytftypep(a, "_float_t"));
}

BGBCC_API int dytfDoubleP(dytf a)
{
	if(dytfFlonumP(a))
		return(true);
	return(dytftypep(a, "_double_t"));
}

BGBCC_API dytf dytfReal(double a)
{
	u64 l;
	int t;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);

	if(!((t-3)&6))
		{ return(*(dytf *)(&l)); }

	switch(t)
	{
	case 3: case 4:
	case 11: case 12:
		break;
	default:
		l=(l+8)>>4;
		l=l&0x0FFFFFFFFFFFFFFFULL;
		l=l|0x8000000000000000ULL;		
		break;
#if 0
	case 0: case 1: case 2:
	case 5: case 6: case 7:
		l=(l+4)>>3;
		l=l|0x5000000000000000ULL;
		break;
	case 8: case 9: case 10:
	case 13: case 14: case 15:
		l=(l+4)>>3;
		l=l&0x0FFFFFFFFFFFFFFFULL;
		l=l|0xA000000000000000ULL;
		break;
	default:
		break;
#endif
	}
	return(*(dytf *)(&l));
}

BGBCC_API dytf dytfFloat(double a)
	{ return(dytfReal(a)); }
BGBCC_API dytf dytfDouble(double a)
	{ return(dytfReal(a)); }

BGBCC_API double dytfRealv(dytf a)
{
	u64 l;
	s64 v;
	int t;
	dyt p;
	double f;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);

	//speculative micro-optimization
	if(!((t-3)&6))
		{ return(*(double *)(&a)); }

	switch(t)
	{
	case 1: case 2:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
		v=(s64)l-0x1000000000000000LL;
		f=v;
		break;
	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
		break;
	case 8:
		l=l<<4;
		f=*(double *)(&l);
		break;

#if 0
	case 5:
		l=l<<3;
		f=*(double *)(&l);
		break;
	case 10:
		l=(l<<3)|0x8000000000000000ULL;
		f=*(double *)(&l);
		break;
#endif
	case 13: case 14:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
//		v|=(-1LL)<<61;
		v=((s64)l)-0xF000000000000000LL;
		f=v;
		break;
	default:
		p=0;
//		p=dytfUnwrap(a);
//		f=dyrealv(p);
		break;
	}
	return(f);
}

BGBCC_API double dytfFloatv(dytf a)
	{ return(dytfRealv(a)); }
BGBCC_API double dytfDoublev(dytf a)
	{ return(dytfRealv(a)); }

BGBCC_API double dytfFloatvF(dytf a)
{
	u64 l;
	s64 v;
	int t;
	double f;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);

	//speculative micro-optimization
	if(!((t-3)&6))
		{ return(*(double *)(&a)); }

	switch(t)
	{
	case 0:
		v=*(f32 *)((nlint)l);
		break;
	case 1: case 2:
		v=(s64)l-0x1000000000000000LL;
		f=v;
		break;
	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
		break;
	case 8:
		l=l<<4;
		f=*(double *)(&l);
		break;
	case 13: case 14:
		v=((s64)l)-0xF000000000000000LL;
		f=v;
		break;
	default:
		f=0;
		break;
	}
	return(f);
//	return(dytfRealv(a));
}
BGBCC_API double dytfDoublevF(dytf a)
{
	u64 l;
	s64 v;
	int t;
	double f;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);

	//speculative micro-optimization
	if(!((t-3)&6))
		{ return(*(double *)(&a)); }

	switch(t)
	{
	case 0:
		v=*(f64 *)((nlint)l);
		break;
	case 1: case 2:
		v=(s64)l-0x1000000000000000LL;
		f=v;
		break;
	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
		break;
	case 8:
		l=l<<4;
		f=*(double *)(&l);
		break;
	case 13: case 14:
		v=((s64)l)-0xF000000000000000LL;
		f=v;
		break;
	default:
		f=0;
		break;
	}
	return(f);
//	return(dytfRealv(a));
}

#if 0
// BGBCC_API int dytfFixintP(dytf a)
{
	static int bits=0x6006;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	return((bits>>t)&1);

#if 0
	switch(t)
	{
	case 1: case 2:
	case 13: case 14:
		r=1; break;
	default: r=0; break;
	}
	return(r);
#endif
}
#endif

BGBCC_API bool dytfIntP(dytf a)
{
	static int bits=0x6006;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	if((bits>>t)&1)
		return(true);
	return(dytftypep(a, "_int_t"));

#if 0
	switch(t)
	{
	case 1: case 2:
	case 13: case 14:
		r=1; break;
	default: r=0; break;
	}
	return(r);
#endif
}

BGBCC_API bool dytfLongP(dytf a)
{
	static int bits=0x6006;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	if((bits>>t)&1)
		return(true);
	return(dytftypep(a, "_long_t"));

#if 0
	switch(t)
	{
	case 1: case 2:
	case 13: case 14:
		r=1; break;
	default: r=0; break;
	}
	return(r);
#endif
}

BGBCC_API dytf dytfInt(s64 v)
{
	u64 t;

	if(v<0)
	{
//		if(v<=(-((1LL)<<61)))
//			return(dytfWrap(dylong(v)));
		t=0xF000000000000000ULL+(u64)v;
		return(*(dytf *)(&t));
	}else
	{
//		if(v>=((1LL)<<61))
//			return(dytfWrap(dylong(v)));
		t=(u64)v;
//		t=t^0x1000000000000000ULL;
		t=t+0x1000000000000000ULL;
		return(*(dytf *)(&t));
	}
}

BGBCC_API dytf dytfLong(s64 v)
	{ return(dytfInt(v)); }

BGBCC_API s64 dytfIntvF(dytf a)
{
	u64 l;
	s64 v;
	int t;
	dyt p;
	double f;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	
	if((t==1) || (t==14))
	{
		v=((s64)l)^0x1000000000000000LL;
		return(v);
	}
	
	switch(t)
	{
	case 1: case 2:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
		v=((s64)l)-0x1000000000000000LL;
		break;
	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
		v=(s64)f;
		break;
	case 5:
		l=l<<3;
		f=*(double *)(&l);
		v=(s64)f;
		break;
	case 10:
		l=(l<<3)|0x8000000000000000ULL;
		f=*(double *)(&l);
		v=(s64)f;
		break;
	case 13: case 14:
//		v=l&0x1FFFFFFFFFFFFFFFULL;
//		v|=(-1LL)<<61;
		v=((s64)l)-0xF000000000000000LL;
		break;
	default:
//		p=dytfUnwrap(a);
//		v=dylongv(p);
		v=0;
		break;
	}
	return(v);
}

BGBCC_API s64 dytfIntv(dytf a)
	{ return(dytfIntvF(a)); }

BGBCC_API s64 dytfLongv(dytf a)
	{ return(dytfIntvF(a)); }

BGBCC_API s64 dytfLongvF(dytf a)
{
	u64 l;
	s64 v;
	int t;
	s64 *p;
	double f;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	
	switch(t)
	{
	case 0:
		v=*(s64 *)((nlint)l);
		break;
	case 1: case 2:
		v=((s64)l)-0x1000000000000000LL;
		break;
	case 3: case 4:
	case 11: case 12:
		f=*(double *)(&a);
		v=(s64)f;
		break;
	case 5:
		l=l<<3;
		f=*(double *)(&l);
		v=(s64)f;
		break;
	case 10:
		l=(l<<3)|0x8000000000000000ULL;
		f=*(double *)(&l);
		v=(s64)f;
		break;
	case 13: case 14:
		v=((s64)l)-0xF000000000000000LL;
		break;
	default:
		v=0;
		break;
	}
	return(v);
}

BGBCC_API dytf dytfInt32F(s32 v)
{
	u64 t;

	t=(u64)((s64)v);
	t=t^0x1000000000000000ULL;
	return(*(dytf *)(&t));

#if 0
	if(v<0)
	{
		t=0xEFFFFFFF00000000ULL|((u32)v);
		return(*(dytf *)(&t));
	}else
	{
		t=0x1000000000000000ULL|((u32)v);
		return(*(dytf *)(&t));
	}
#endif
}

BGBCC_API s32 dytfInt32vF(dytf a)
{
	return((s32)(*(u64 *)(&a)));
}

BGBCC_API dytf dytfAddInt32FC(dytf a, s32 b)
{
	u64 t;
	s32 v;
	v=((s32)(*(u64 *)(&a)))+b;
	t=(u64)((s64)v);
	t=t^0x1000000000000000ULL;
	return(*(dytf *)(&t));
}

BGBCC_API dytf dytfSubInt32FC(dytf a, s32 b)
{
	u64 t;
	s32 v;
	v=((s32)(*(u64 *)(&a)))-b;
	t=(u64)((s64)v);
	t=t^0x1000000000000000ULL;
	return(*(dytf *)(&t));
}

BGBCC_API dytf dytfMulInt32FC(dytf a, s32 b)
{
	u64 t;
	s32 v;
	v=((s32)(*(u64 *)(&a)))*b;
	t=(u64)((s64)v);
	t=t^0x1000000000000000ULL;
	return(*(dytf *)(&t));
}

BGBCC_API dytf dytfDivInt32FC(dytf a, s32 b)
{
	u64 t;
	s32 v;
	v=((s32)(*(u64 *)(&a)))/b;
	t=(u64)((s64)v);
	t=t^0x1000000000000000ULL;
	return(*(dytf *)(&t));
}


BGBCC_API bool dytfInt32P(dytf a)
{
	u64 t, u, v;
	if((0x4002>>((int)((a.v>>60)&15)))&1)
	{
		t=(u64)((s64)((s32)(a.v)));
		u=t&0x0FFFFFFF00000000ULL;
		v=a.v&0x0FFFFFFF00000000ULL;
		return(u==v);
	}
	if(dytftypep(a, "_int_t"))
		return(true);
	return(false);
}

BGBCC_API double dytfRealvF(dytf a)
	{ return(dytfRealv(a)); }

#if 0
// BGBCC_API double dytfRealvF(dytf a)
	{ return(*(double *)(&a)); }

// BGBCC_API void *dytfPtrvF(dytf a)
{
	return((void *)((nlint)(*(u64 *)(&a))));
}

// BGBCC_API dytf dytfPtrF(void *p)
{
	u64 l;
	
	l=(u64)((nlint)p);
	return(*(dytf *)(&l));
}

#endif

BGBCC_API void *dytfPtrv(dytf a)
{
	if(!dytfPtrP(a))
		return(UNDEFINED);
	return((void *)((nlint)(*(u64 *)(&a))));
}

#if 0
// BGBCC_API int dytfPtrP(dytf a)
{
	static int bits=0x8001;
	u64 l;
	int t, r;

	l=*(u64 *)(&a);
	t=(int)((l>>60)&15);
	return((bits>>t)&1);
}
#endif

BGBCC_API dytf dytfString(char *str)
	{ return(dytfPtrF(bgbcc_strdup(str))); }
BGBCC_API dytf dytfSymbol(char *str)
	{ return(dytfPtrF(bgbcc_strdup_symbol(str))); }
BGBCC_API dytf dytfKeyword(char *str)
	{ return(dytfPtrF(bgbcc_strdup_keyword(str))); }

// BGBCC_API char *dytfStringv(dytf a)
//	{ return(dystringv(dytfUnwrap(a))); }
// BGBCC_API char *dytfSymbolv(dytf a)
// 	{ return(dysymbolv(dytfUnwrap(a))); }
// BGBCC_API char *dytfKeywordv(dytf a)
//	{ return(dysymbolv(dytfUnwrap(a))); }

BGBCC_API char *dytfStringv(dytf a)
	{ return((char *)dytfPtrvF(a)); }
BGBCC_API char *dytfSymbolv(dytf a)
	{ return((char *)dytfPtrvF(a)); }
BGBCC_API char *dytfKeywordv(dytf a)
	{ return((char *)dytfPtrvF(a)); }

BGBCC_API char *dytfStringvF(dytf a)
	{ return((char *)dytfPtrvF(a)); }
BGBCC_API char *dytfSymbolvF(dytf a)
	{ return((char *)dytfPtrvF(a)); }
BGBCC_API char *dytfKeywordvF(dytf a)
	{ return((char *)dytfPtrvF(a)); }

BGBCC_API bool dytfStringP(dytf a)
	{ return(bgbcc_strdup_strp((char *)dytfUnwrap(a))); }
BGBCC_API bool dytfSymbolP(dytf a)
	{ return(bgbcc_strdup_symp((char *)dytfUnwrap(a))); }
BGBCC_API bool dytfKeywordP(dytf a)
	{ return(bgbcc_strdup_keyp((char *)dytfUnwrap(a))); }

#if 0
// BGBCC_API bool dytfNullP(dytf a)
{
	u64 l;
	l=*(u64 *)(&a);
	if(!l)return(true);
	return(false);
}

// BGBCC_API bool dytfUndefinedP(dytf a)
{
	u64 l;
	l=*(u64 *)(&a);
//	if(((void *)((nlint)l))==UNDEFINED)
//	if(dytfEqqP(a, DYTF_UNDEFINED))
	if(l==((u64)((nlint)UNDEFINED)))
		return(true);
	return(false);
}
#endif

BGBCC_API char *dytfgettype(dytf a)
{
	void *p;

	if(dytfPtrP(a))
	{
		p=dytfPtrvF(a);
		if(BGBCC_ConsP(p))
			return("_cons_t");
		return(bgbcc_malloc_gettype(p));
	}

//	if(dytfRealP(a))
	if(dytfFlonumP(a))
		return("_double_t");
//	if(dytfIntP(a))
	if(dytfFixintP(a))
		return("_fixint_t");
	if(dytfFixnumP(a))
		return("_fixlong_t");
	return(bgbcc_malloc_gettype(dytfUnwrap(a)));
}

BGBCC_API int dytftypep(dytf a, char *ty)
{
	char *ty1;

	ty1=dytfgettype(a);

	if(!ty && !ty1)return(1);
	if(!ty || !ty1)return(0);

	if(!strcmp(ty1, ty))return(1);
	return(0);
}

#if 0
// BGBCC_API bool dytfEqqP(dytf a, dytf b)
{
	if((*(u64 *)(&a))==(*(u64 *)(&b)))return(true);
	return(false);
}

// BGBCC_API bool dytfNEqqP(dytf a, dytf b)
{
	if((*(u64 *)(&a))!=(*(u64 *)(&b)))return(true);
	return(false);
}
#endif


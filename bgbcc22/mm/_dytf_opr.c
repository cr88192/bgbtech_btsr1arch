#include <bgbccc.h>

BGBCC_API dytf dytfAdd(dytf a, dytf b)
{
	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealvF(a)+dytfRealvF(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfReal(dytfRealvF(a)+dytfRealv(b))); }
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntvF(a)+dytfIntvF(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealv(a)+dytfRealvF(b))); }
	}
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyadd(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfSub(dytf a, dytf b)
{
	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealvF(a)-dytfRealvF(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfReal(dytfRealvF(a)-dytfRealv(b))); }
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)-dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealv(a)-dytfRealvF(b))); }
	}
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dysub(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfMul(dytf a, dytf b)
{
	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealvF(a)*dytfRealvF(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfReal(dytfRealvF(a)*dytfRealv(b))); }
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)*dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfReal(dytfRealv(a)*dytfRealvF(b))); }
	}
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dymul(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfDiv(dytf a, dytf b)
{
	dytf c;

	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
		{
			c=dytfReal(dytfRealvF(a)/dytfRealvF(b));
			return(c);
		}else if(dytfFixnumP(b))
		{
			c=dytfReal(dytfRealvF(a)/dytfRealv(b));
			return(c);
		}
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
		{
			c=dytfInt(dytfIntv(a)/dytfIntv(b));
			return(c);
		}else if(dytfFlonumP(b))
		{
			c=dytfReal(dytfRealv(a)/dytfRealvF(b));
			return(c);
		}
	}

	return(DYTF_UNDEFINED);
//	c=dytfWrap(dydiv(dytfUnwrap(a), dytfUnwrap(b)));
//	return(c);
}

BGBCC_API dytf dytfMod(dytf a, dytf b)
{
	dytf c;

	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
		{
			c=dytfReal(fmod(dytfRealvF(a), dytfRealvF(b)));
			return(c);
		}else if(dytfFixnumP(b))
		{
			c=dytfReal(fmod(dytfRealvF(a), dytfRealv(b)));
			return(c);
		}
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
		{
			c=dytfInt(dytfIntv(a)%dytfIntv(b));
			return(c);
		}else if(dytfFlonumP(b))
		{
			c=dytfReal(fmod(dytfRealv(a), dytfRealvF(b)));
			return(c);
		}
	}

	return(DYTF_UNDEFINED);
//	c=dytfWrap(dymod(dytfUnwrap(a), dytfUnwrap(b)));
//	return(c);
}

BGBCC_API dytf dytfAnd(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)&dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfInt(dytfIntv(a)&dytfIntv(b))); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfIntv(a)&dytfIntv(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)&dytfIntv(b))); }
	}

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyand(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfOr(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)|dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfInt(dytfIntv(a)|dytfIntv(b))); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfIntv(a)|dytfIntv(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)|dytfIntv(b))); }
	}

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyor(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfXor(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)^dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfInt(dytfIntv(a)^dytfIntv(b))); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfIntv(a)^dytfIntv(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)^dytfIntv(b))); }
	}

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyxor(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfShl(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)<<dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfInt(dytfIntv(a)<<dytfIntv(b))); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfIntv(a)<<dytfIntv(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)<<dytfIntv(b))); }
	}

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyshl(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfShr(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)>>dytfIntv(b))); }
		else if(dytfFlonumP(b))
			{ return(dytfInt(dytfIntv(a)>>dytfIntv(b))); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfReal(dytfIntv(a)>>dytfIntv(b))); }
		else if(dytfFixnumP(b))
			{ return(dytfInt(dytfIntv(a)>>dytfIntv(b))); }
	}

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyshr(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API dytf dytfExp(dytf a, dytf b)
{
	dytf c;

	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
		{
			c=dytfReal(pow(dytfRealvF(a), dytfRealvF(b)));
			return(c);
		}else if(dytfFixnumP(b))
		{
			c=dytfReal(pow(dytfRealvF(a), dytfRealv(b)));
			return(c);
		}
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
		{
			c=dytfInt(pow(dytfIntv(a), dytfIntv(b)));
			return(c);
		}else if(dytfFlonumP(b))
		{
			c=dytfReal(pow(dytfRealv(a), dytfRealvF(b)));
			return(c);
		}
	}

	return(DYTF_UNDEFINED);
//	c=dytfWrap(dyexp(dytfUnwrap(a), dytfUnwrap(b)));
//	return(c);
}

BGBCC_API dytf dytfNeg(dytf a)
{
	if(dytfFlonumP(a))
		{ return(dytfReal(-dytfRealvF(a))); }
	else if(dytfFixnumP(a))
		{ return(dytfInt(-dytfIntv(a))); }

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyneg(dytfUnwrap(a))));
}

BGBCC_API dytf dytfNot(dytf a)
{
	if(dytfFlonumP(a))
		{ return(dytfReal(~dytfIntv(a))); }
	else if(dytfFixnumP(a))
		{ return(dytfInt(~dytfIntv(a))); }

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dynot(dytfUnwrap(a))));
}

BGBCC_API dytf dytfLNot(dytf a)
{
	if(dytfFlonumP(a))
		{ return(dytfReal(!dytfIntv(a))); }
	else if(dytfFixnumP(a))
		{ return(dytfInt(!dytfIntv(a))); }

	return(DYTF_UNDEFINED);
//	return(dytfWrap(dylnot(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUMul(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyumul(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUDiv(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyudiv(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUMod(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyumod(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUAnd(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyuand(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUOr(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyuor(dytfUnwrap(a))));
}

BGBCC_API dytf dytfUXor(dytf a)
{
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyuxor(dytfUnwrap(a))));
}

BGBCC_API dytf dytfSin(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(sin(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dySin(dytfUnwrap(a))));
}

BGBCC_API dytf dytfCos(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(cos(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyCos(dytfUnwrap(a))));
}

BGBCC_API dytf dytfTan(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(tan(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyTan(dytfUnwrap(a))));
}

BGBCC_API dytf dytfASin(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(asin(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyASin(dytfUnwrap(a))));
}

BGBCC_API dytf dytfACos(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(acos(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyACos(dytfUnwrap(a))));
}

BGBCC_API dytf dytfATan(dytf a)
{
	if(dytfFixrealP(a))
		{ return(dytfReal(atan(dytfRealv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyATan(dytfUnwrap(a))));
}

BGBCC_API dytf dytfATan2(dytf a, dytf b)
{
	if(dytfFixrealP(a) && dytfFixrealP(a))
		{ return(dytfReal(atan2(dytfRealv(a), dytfRealv(b)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dyATan2(dytfUnwrap(a), dytfUnwrap(b))));
}

BGBCC_API bool dytfEqP(dytf a, dytf b)
{
	if(a.v==b.v)return(true);
//	if((*(u64 *)(&a))==(*(u64 *)(&b)))return(true);

	if(dytfFixnumP(a) && dytfFixnumP(b))
		return(dytfIntvF(a) == dytfIntvF(b));
	if(dytfFlonumP(a) && dytfFlonumP(b))
		return(dytfRealvF(a) == dytfRealvF(b));
	return(false);
//	return(dyeqp(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfNEqP(dytf a, dytf b)
{
	return(!dytfEqP(a, b));
}

BGBCC_API bool dytfEqvP(dytf a, dytf b)
{
	if(a.v==b.v)return(true);
//	if((*(u64 *)(&a))==(*(u64 *)(&b)))return(true);

	if(dytfFlonumP(a) && dytfFlonumP(b))
		return(dytfRealvF(a) == dytfRealvF(b));
//	if(dytfFixnumP(a) && dytfFixnumP(b))
//		return(dytfIntv(a) == dytfIntv(b));
	return(false);
//	return(dyeqvp(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfNEqvP(dytf a, dytf b)
{
	return(!dytfEqvP(a, b));
}


BGBCC_API bool dytfLtP(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfIntv(a)<dytfIntv(b)); }
		else if(dytfFlonumP(b))
			{ return(dytfRealv(a)<dytfRealvF(b)); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfRealvF(a)<dytfRealvF(b)); }
		else if(dytfFixnumP(b))
			{ return(dytfRealvF(a)<dytfRealv(b)); }
	} 
	return(false);
//	return(dyltp(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfGtP(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfIntv(a)>dytfIntv(b)); }
		else if(dytfFlonumP(b))
			{ return(dytfRealv(a)>dytfRealvF(b)); }
	}else
	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfRealvF(a)>dytfRealvF(b)); }
		else if(dytfFixnumP(b))
			{ return(dytfRealvF(a)>dytfRealv(b)); }
	} 
	return(false);
//	return(dygtp(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfLeP(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfIntv(a)<=dytfIntv(b)); }
		else if(dytfFlonumP(b))
			{ return(dytfRealv(a)<=dytfRealvF(b)); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfRealvF(a)<=dytfRealvF(b)); }
		else if(dytfFixnumP(b))
			{ return(dytfRealvF(a)<=dytfRealv(b)); }
	}if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfIntv(a)<=dytfIntv(b)); }
		else if(dytfFlonumP(b))
			{ return(dytfRealv(a)<=dytfRealvF(b)); }
	}
	return(false);
//	return(dylep(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfGeP(dytf a, dytf b)
{
	if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(dytfIntv(a)>=dytfIntv(b)); }
		else if(dytfFlonumP(b))
			{ return(dytfRealv(a)>=dytfRealvF(b)); }
	}else if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(dytfRealvF(a)>=dytfRealvF(b)); }
		else if(dytfFixnumP(b))
			{ return(dytfRealvF(a)>=dytfRealv(b)); }
	}
	return(false);
//	return(dygep(dytfUnwrap(a), dytfUnwrap(b)));
}

BGBCC_API bool dytfUnP(dytf a, dytf b)
{
	if(dytfFlonumP(a))
	{
		if(dytfFlonumP(b))
			{ return(!(dytfRealvF(a)>=dytfRealvF(b))&&
				!(dytfRealvF(a)<=dytfRealvF(b))); }
		else if(dytfFixnumP(b))
			{ return(!(dytfRealvF(a)>=dytfRealvF(b))&&
				!(dytfRealvF(a)<=dytfRealvF(b))); }
	}else if(dytfFixnumP(a))
	{
		if(dytfFixnumP(b))
			{ return(0); }
		else if(dytfFlonumP(b))
			{ return(!(dytfRealvF(a)>=dytfRealvF(b))&&
				!(dytfRealvF(a)<=dytfRealvF(b))); }
	}
	return(false);
//	return(dyunp(dytfUnwrap(a), dytfUnwrap(b)));
}

static double dytf_sqr(double x) { return(x*x); }
BGBCC_API dytf dytfSqr(dytf a)
{
	if(dytfFlonumP(a))
		{ return(dytfReal(dytf_sqr(dytfRealvF(a)))); }
	else if(dytfFixnumP(a))
		{ return(dytfInt(dytf_sqr(dytfIntv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dysqr(dytfUnwrap(a))));
}

BGBCC_API dytf dytfSqrt(dytf a)
{
	if(dytfFlonumP(a))
		{ return(dytfReal(sqrt(dytfRealvF(a)))); }
	else if(dytfFixnumP(a))
		{ return(dytfReal(sqrt(dytfIntv(a)))); }
	return(DYTF_UNDEFINED);
//	return(dytfWrap(dysqrt(dytfUnwrap(a))));
}

BGBCC_API dytf dytfSSqr(dytf a)
{
//	if(dytfFlonumP(a))
//		{ return(dytfReal(dytf_sqr(dytfRealvF(a)))); }
//	else if(dytfFixnumP(a))
//		{ return(dytfInt(dytf_sqr(dytfIntv(a)))); }
//	return(dytfWrap(dyssqr(dytfUnwrap(a))));
	return(DYTF_UNDEFINED);
}

BGBCC_API dytf dytfSSqrt(dytf a)
{
//	if(dytfFlonumP(a))
//		{ return(dytfReal(sqrt(dytfRealvF(a)))); }
//	else if(dytfFixnumP(a))
//		{ return(dytfReal(sqrt(dytfIntv(a)))); }
//	return(dytfWrap(dyssqrt(dytfUnwrap(a))));
	return(DYTF_UNDEFINED);
}

BGBCC_API dytf dytfInc(dytf a)
{
//	return(dytfWrap(dyinc(dytfUnwrap(a))));
	return(DYTF_UNDEFINED);
}

BGBCC_API dytf dytfDec(dytf a)
{
//	return(dytfWrap(dydec(dytfUnwrap(a))));
	return(DYTF_UNDEFINED);
}

BGBCC_API int dytfTrueP(dytf a)
{
	u64 l;
	l=a.v;

	if(!l)return(false);
	if(l==((u64)((nlint)UNDEFINED)))		return(false);
	if(l==((u64)((nlint)BGBDY_FALSE)))		return(false);
	if(l==((u64)((nlint)BGBDY_AUXNULL)))	return(false);
	
	if(dytfFixrealP(a))
		{ return(dytfRealv(a)!=0); }
	
	return(true);
}

BGBCC_API int dytfFalseP(dytf a)
{
	return(!dytfTrueP(a));
//	return(dyfalsep(dytfUnwrap(a)));
}

BGBCC_API dytf dytfBool(int a)
{
	if(a)	return(DYTF_TRUE);
	return(DYTF_FALSE);
//	return(dytfWrap(dybool(a)));
//	return(DYTF_UNDEFINED);
}

BGBCC_API dytf dytfChar(int a)
{
	u64 t;
	s32 v;
	t=(u64)((u32)v);
	t=t|0x7C00000300000000ULL;
	return(*(dytf *)(&t));
//	return(dytfWrap(dychar(a)));
}

BGBCC_API bool dytfCharP(dytf a)
{
	return((a.v&0xFFFFFFFF00000000ULL)==0x7C00000300000000ULL);
}

BGBCC_API int dytfCharvF(dytf a)
{
	return((int)a.v);
}

BGBCC_API dytf dytfUnaryS(char *opr, dytf a)
{
//	return(dytfWrap(dyUnaryS(opr, dytfUnwrap(a))));
	return(DYTF_UNDEFINED);
}

BGBCC_API dytf dytfBinaryS(char *opr, dytf a, dytf b)
{
//	return(dytfWrap(dyBinaryS(opr, dytfUnwrap(a), dytfUnwrap(b))));
	return(DYTF_UNDEFINED);
}

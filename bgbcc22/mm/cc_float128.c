bgbcc_vfloat128 BGBCC_Float128_FromDouble(double val)
{
	bgbcc_vfloat128 c;
	u64 dxlo, dxhi, v;
	int dpct, dpshr;

	*((double *)(&v))=val;

	if(!(v&0x7FF0000000000000ULL))
	{
		c.hi=0;
		c.lo=0;
		return(c);
	}

	dxhi=((v>>4)&0x07FFFFFFFFFFFFFFULL)+
		(  0x3C00000000000000ULL)+
		(v&0x8000000000000000ULL);
	dxlo=v<<60;
	
	c.hi=dxhi;
	c.lo=dxlo;
	return(c);
}

double BGBCC_Float128_ToDouble(bgbcc_vfloat128 val)
{
	u64 v, hi, lo;
	
	hi=val.hi;
	lo=val.lo;

	if((((hi>>58)&15)==0)||(((hi>>58)&15)==15))
	{
		v=	(hi&0xC000000000000000ULL)|
			((hi<<4)&0x3FFFFFFFFFFFFFFFULL)|
	//		(lo>>60);
//			((lo+0x07FFFFFFFFFFFFFFULL)>>60);
			((lo+0x0800000000000000ULL)>>60);
		return(*(double *)(&v));
	}
	
	if(hi&0x4000000000000000ULL)
	{
		v=(hi&0x8000000000000000ULL)|
			0x7FF0000000000000ULL;
		return(*(double *)(&v));
	}

	return(0.0);
}

double BGBCC_Float128_ToDoubleB(bgbcc_vfloat128 val)
{
	u64 v, hi, lo;
	
	hi=val.hi;
	lo=val.lo;

	if((((hi>>58)&15)==0)||(((hi>>58)&15)==15))
	{
		v=	(hi&0xC000000000000000ULL)|
			((hi<<4)&0x3FFFFFFFFFFFFFFFULL);
		return(*(double *)(&v));
	}
	
	if(hi&0x4000000000000000ULL)
	{
		v=(hi&0x8000000000000000ULL)|
			0x7FF0000000000000ULL;
		return(*(double *)(&v));
	}

	return(0.0);
}

bgbcc_vfloat128 BGBCC_Float128_FromDoubleBits(u64 v)
{
	bgbcc_vfloat128 c;
	u64 dxlo, dxhi;
	int dpct, dpshr;

	if(!(v&0x7FF0000000000000ULL))
	{
		c.hi=0;
		c.lo=0;
		return(c);
	}

	dxhi=((v>>4)&0x07FFFFFFFFFFFFFFULL)+
		(  0x3C00000000000000ULL)+
		(v&0x8000000000000000ULL);
	dxlo=v<<60;
	
	c.hi=dxhi;
	c.lo=dxlo;
	return(c);
}

u64 BGBCC_Float128_ToDoubleBits(bgbcc_vfloat128 val)
{
	u64 v, hi, lo;
	
	hi=val.hi;
	lo=val.lo;

	if((((hi>>58)&15)==0)||(((hi>>58)&15)==15))
	{
		v=	(hi&0xC000000000000000ULL)|
			((hi<<4)&0x3FFFFFFFFFFFFFFFULL)|
			((lo+0x0800000000000000ULL)>>60);
		return(v);
	}
	
	if(hi&0x4000000000000000ULL)
	{
		v=(hi&0x8000000000000000ULL)|
			0x7FF0000000000000ULL;
		return(v);
	}

	return(0);
}


bgbcc_vfloat128 BGBCC_Float128_FromInt128(bgbcc_vint128 val)
{
	bgbcc_vint128 frc;
	bgbcc_vfloat128 c;
	int exc, sga;

	if(!val.hi && !val.lo)
		{ c.hi=0; c.lo=0; return(c); }

	frc=val; exc=16383+112; sga=0;

	if(val.hi&0x8000000000000000ULL)
		{ frc=BGBCC_Int128_Neg(frc); sga=1; }

	while(frc.hi&0xFFFE000000000000ULL)
		{ frc=BGBCC_Int128_Shr(frc, 1); exc++; }
	while(!(frc.hi&0x0001000000000000ULL))
		{ frc=BGBCC_Int128_Shl(frc, 1); exc--; }

	c=BGBCC_Float128_ComposeSgExFr(sga, exc, frc);
	return(c);
}


bgbcc_vint128 BGBCC_Float128_GetMantissa(bgbcc_vfloat128 a)
{
	bgbcc_vint128 c;
	
	c.hi=(a.hi&0x0000FFFFFFFFFFFFULL)|0x0001000000000000ULL;
	c.lo=a.lo;
	return(c);
}

int BGBCC_Float128_GetExponent(bgbcc_vfloat128 a)
{
	return((a.hi>>48)&0x7FFF);
}

int BGBCC_Float128_GetSign(bgbcc_vfloat128 a)
{
	return((a.hi>>63)&1);
}

bgbcc_vfloat128 BGBCC_Float128_ComposeSgExFr(
	int sg, int ex, bgbcc_vint128 fr)
{
	bgbcc_vfloat128 c;
	
	c.hi=	(((u64)sg)<<63) |
			(((u64)ex)<<48) |
			(fr.hi&0x0000FFFFFFFFFFFFULL);
	c.lo=fr.lo;
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_AddPI(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 c;
	bgbcc_vint128 fra;
	bgbcc_vint128 frb;
	bgbcc_vint128 frc;
	byte sga, sgb;
	int exa, exb, exc, dsh;
	
	sga=BGBCC_Float128_GetSign(a);
	sgb=BGBCC_Float128_GetSign(b);
	exa=BGBCC_Float128_GetExponent(a);
	exb=BGBCC_Float128_GetExponent(b);
	
	fra=BGBCC_Float128_GetMantissa(a);
	frb=BGBCC_Float128_GetMantissa(b);
	
	
	dsh=exa-exb;
	if(dsh>=110)
		return(a);
	
	exc=exa;

	frb=BGBCC_Int128_Shr(frb, dsh);
	frc=BGBCC_Int128_Add(fra, frb);
	while(frc.hi&0xFFFE000000000000ULL)
		{ frc=BGBCC_Int128_Shr(frc, 1); exc++; }

	c=BGBCC_Float128_ComposeSgExFr(sga, exc, frc);
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_AddSI(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 c;
	bgbcc_vint128 fra;
	bgbcc_vint128 frb;
	bgbcc_vint128 frc;
	byte sga, sgb;
	int exa, exb, exc, dsh;
	
	sga=BGBCC_Float128_GetSign(a);
	sgb=BGBCC_Float128_GetSign(b);
	exa=BGBCC_Float128_GetExponent(a);
	exb=BGBCC_Float128_GetExponent(b);
	
	fra=BGBCC_Float128_GetMantissa(a);
	frb=BGBCC_Float128_GetMantissa(b);
	
	
	dsh=exa-exb;
	if(dsh>=112)
		return(a);
	
	exc=exa;

	frb=BGBCC_Int128_Shr(frb, dsh);
	frc=BGBCC_Int128_Sub(fra, frb);
	
	if(!frc.hi && !frc.lo)
		{ c.hi=0; c.lo=0; return(c); }
	
	while(!(frc.hi&0x0001000000000000ULL))
		{ frc=BGBCC_Int128_Shl(frc, 1); exc--; }

	c=BGBCC_Float128_ComposeSgExFr(sga, exc, frc);
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_Add(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 c;
	bgbcc_vint128 fra;
	bgbcc_vint128 frb;
	byte sga, sgb;
	int exa, exb, flip;
	
	sga=BGBCC_Float128_GetSign(a);
	sgb=BGBCC_Float128_GetSign(b);
	exa=BGBCC_Float128_GetExponent(a);
	exb=BGBCC_Float128_GetExponent(b);
	
	if(exa>exb)
		{ flip=0; }
	else if(exa<exb)
		{ flip=1; }
	else
	{
		fra=BGBCC_Float128_GetMantissa(a);
		frb=BGBCC_Float128_GetMantissa(b);
		flip=BGBCC_Int128_CmpGt(frb, fra);
	}

	if(flip)
	{
		if(sga^sgb)
			{ return(BGBCC_Float128_AddSI(b, a)); }
		else
			{ return(BGBCC_Float128_AddPI(b, a)); }
	}else
	{
		if(sga^sgb)
			{ return(BGBCC_Float128_AddSI(a, b)); }
		else
			{ return(BGBCC_Float128_AddPI(a, b)); }
	}
}

bgbcc_vfloat128 BGBCC_Float128_Sub(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 b2, c;
	
	b2=BGBCC_Float128_Neg(b);
	c=BGBCC_Float128_Add(a, b2);
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_Mul(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 c;
	bgbcc_vint128 fra, frb;
	bgbcc_vint128 frc, frd;
	byte sga, sgb, sgc;
	int exa, exb, exc, dsh;
	
	sga=BGBCC_Float128_GetSign(a);
	sgb=BGBCC_Float128_GetSign(b);
	exa=BGBCC_Float128_GetExponent(a);
	exb=BGBCC_Float128_GetExponent(b);
	
	if(!exa || !exb)
		{ c.hi=0; c.lo=0; return(c); }
	
	fra=BGBCC_Float128_GetMantissa(a);
	frb=BGBCC_Float128_GetMantissa(b);
	
	sgc=sga^sgb;
	exc=exa+exb-16383;
	if(exc<=0)
		{ c.hi=0; c.lo=0; return(c); }
	if(exc>=32767)
	{
		c.hi=0x7FFF000000000000ULL|(((u64)sgc)<<63);
		c.lo=0x0000000000000000ULL;
		return(c);
	}
	
//	frc=BGBCC_Int128_MulHA(fra, frb);
	BGBCC_Int128_MulW(fra, frb, &frd, &frc);

	frc=BGBCC_Int128_Shl(frc, 16);
//	frc.lo|=frd.hi>>48;
	
	while(frc.hi&0xFFFE000000000000ULL)
		{ frc=BGBCC_Int128_Shr(frc, 1); exc++; }

#if 0	
	dsh=exa-exb;
	if(dsh>=112)
		return(a);
	
	exc=exa;

	frb=BGBCC_Int128_Shr(frb, dsh);
	frc=BGBCC_Int128_Sub(fra, frb);
	while(!(frc.hi&0x0001000000000000ULL))
		{ frc=BGBCC_Int128_Shl(frc, 1); exc--; }
#endif

	c=BGBCC_Float128_ComposeSgExFr(sgc, exc, frc);
	return(c);
}


bgbcc_vfloat128 BGBCC_Float128_Neg(bgbcc_vfloat128 a)
{
	bgbcc_vfloat128 c;
	
	c.hi=a.hi^0x8000000000000000ULL;
	c.lo=a.lo;
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_RcpA(bgbcc_vfloat128 a)
{
	bgbcc_vfloat128 c;
	
	c.hi=0x7FFF000000000000ULL-a.hi;
	c.lo=~a.lo;
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_Rcp(bgbcc_vfloat128 a)
{
	bgbcc_vfloat128 c, d;
	bgbcc_vint128 ta, tb, tc;
	
	if(a.hi&0x8000000000000000ULL)
	{
		c=BGBCC_Float128_Neg(a);
		d=BGBCC_Float128_Rcp(c);
		c=BGBCC_Float128_Neg(d);
		return(c);
	}
	
	c=BGBCC_Float128_RcpA(a);
	d=BGBCC_Float128_Mul(a, c);

	while((d.hi!=0x3FFF000000000000ULL) ||
		(d.lo&0xFFFFFFFFFFFFFFFCULL))
	{
		ta.hi=c.hi;
		ta.lo=c.lo;
		
		tb.hi=d.hi-0x3FFF000000000000ULL;
		tb.lo=d.lo;
		
		tb=BGBCC_Int128_Shr(tb, 1);
		
		tc=BGBCC_Int128_Sub(ta, tb);
		c.hi=tc.hi;
		c.lo=tc.lo;

		d=BGBCC_Float128_Mul(a, c);
	}
	
	return(c);
}

bgbcc_vfloat128 BGBCC_Float128_Div(
	bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	bgbcc_vfloat128 c, b2;
	
	b2=BGBCC_Float128_Rcp(b);
	c=BGBCC_Float128_Mul(a, b2);
	return(c);
}

int BGBCC_Float128_CmpEqD(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	double f, g;
	
	f=BGBCC_Float128_ToDoubleB(a);
	g=BGBCC_Float128_ToDoubleB(b);
	return(f==g);
}

int BGBCC_Float128_CmpEq(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	if(a.hi!=b.hi)
		return(0);
	if(a.lo!=b.lo)
		return(0);
	return(1);
}

int BGBCC_Float128_CmpNe(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
	{ return(!BGBCC_Float128_CmpEq(a, b)); }

int BGBCC_Float128_CmpGt(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
{
	byte sga, sgb, tt, tf;

	sga=BGBCC_Float128_GetSign(a);
	sgb=BGBCC_Float128_GetSign(b);

	tt=!sga;
	tf= sga;
	
	if(sga^sgb)
		return(tt);

	if(a.hi>b.hi)
		return(tt);
	if(a.hi<b.hi)
		return(tf);
	if(a.lo>b.lo)
		return(tt);
	return(tf);
}

int BGBCC_Float128_CmpLe(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
	{ return(!BGBCC_Float128_CmpGt(a, b)); }
int BGBCC_Float128_CmpLt(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
	{ return(BGBCC_Float128_CmpGt(b, a)); }
int BGBCC_Float128_CmpGe(bgbcc_vfloat128 a, bgbcc_vfloat128 b)
	{ return(!BGBCC_Float128_CmpGt(b, a)); }

bgbcc_vfloat128 BGBCC_Float128_FromString(char *str)
{
	static bgbcc_vfloat128 sc10, sc01, sc1000, sc0001;
	static int init=0;
	bgbcc_vfloat128 a, b, c;
	char *s;
	int sg, sg1, i;
	
	if(!init)
	{
		sc10=BGBCC_Float128_FromDouble(10.0);
		sc1000=BGBCC_Float128_FromDouble(1000.0);
	//	sc01=BGBCC_Float128_FromDouble(0.1);
	//	sc01=BGBCC_Float128_FromDouble(0.09);
		sc01=BGBCC_Float128_Rcp(sc10);
		sc0001=BGBCC_Float128_Rcp(sc1000);
		init=1;
	}

//	a=BGBCC_Float128_FromDouble(0.0);
	a.hi=0; a.lo=0;
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	while(*s)
	{
		if((*s>='0') && (*s<='9'))
		{
			if((s[1]>='0') && (s[1]<='9') &&
				(s[2]>='0') && (s[2]<='9'))
			{
				i=((s[0]-'0')*100)+((s[1]-'0')*10)+(s[2]-'0');
				b=BGBCC_Float128_FromDouble(i);
				a=BGBCC_Float128_Mul(a, sc1000);
				a=BGBCC_Float128_Add(a, b);
				s+=3;
				continue;
			}
		
			b=BGBCC_Float128_FromDouble(*s-'0');
			a=BGBCC_Float128_Mul(a, sc10);
			a=BGBCC_Float128_Add(a, b);
			s++;
			continue;
		}

		if(*s=='.')
			{ break; }
		if((*s=='_') || (*s=='`'))
			{ s++; continue; }
		break;
	}
	
	if(*s=='.')
//	if(0)
	{
		s++;
//		c=sc01;
//		c=BGBCC_Float128_FromDouble(1.0);
		c.hi=0x3FFF000000000000ULL;
		c.lo=0x0000000000000000ULL;

		while(*s)
		{
			if((*s>='0') && (*s<='9'))
			{
#if 1
				if((s[1]>='0') && (s[1]<='9') &&
					(s[2]>='0') && (s[2]<='9'))
				{
					i=((s[0]-'0')*100)+((s[1]-'0')*10)+(s[2]-'0');
					b=BGBCC_Float128_FromDouble(i);
					c=BGBCC_Float128_Mul(c, sc0001);
					b=BGBCC_Float128_Mul(b, c);
					a=BGBCC_Float128_Add(a, b);
					s+=3;
					continue;
				}
#endif

				b=BGBCC_Float128_FromDouble(*s-'0');
				c=BGBCC_Float128_Mul(c, sc01);
				b=BGBCC_Float128_Mul(b, c);
				a=BGBCC_Float128_Add(a, b);
				s++;
				continue;
			}

			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			break;
		}
	}

	if((*s=='e') || (*s=='E'))
	{
		s++;
		
		sg1=0;
		if(*s=='-')
			{ s++; sg1=1; }
		i=0;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
			{
				i=(i*10)+((*s++)-'0');
				continue;
			}
			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			break;
		}
		if(sg1)
		{
			while(i>=3)
				{ a=BGBCC_Float128_Mul(a, sc0001); i-=3; }
			while(i--)
				{ a=BGBCC_Float128_Mul(a, sc01); }
		}else
		{
			while(i>=3)
				{ a=BGBCC_Float128_Mul(a, sc1000); i-=3; }
			while(i--)
				{ a=BGBCC_Float128_Mul(a, sc10); }
		}
	}
	
	if(sg)
		{ a=BGBCC_Float128_Neg(a); }
	return(a);
}

int BGBCC_Float128_Init_QI()
{
	bgbcc_vint128 qi_a, qi_b, qi_c, qi_d, qi_e, qi_f, qi_g, qi_h;
	bgbcc_vint128 qj_a, qj_b, qj_c, qj_d;
	
	qi_a.hi=0x0123456789ABCDEFULL;
	qi_a.lo=0x0123456789ABCDEFULL;

	qi_b.hi=0xFEDCBA9876543210ULL;
	qi_b.lo=0xFEDCBA9876543210ULL;
	
	qi_c.hi=0x02468ACF13579BDEULL;
	qi_c.lo=0x02468ACF13579BDEULL;
	
	qi_d.hi=0x0000000000000000ULL;
	qi_d.lo=0x0000000000000000ULL;
	
	qi_e.hi=0xFDB97530ECA86421ULL;
	qi_e.lo=0xFDB97530ECA86420ULL;

	qi_f.hi=0x00FEDCBA98765432ULL;
	qi_f.lo=0x10FEDCBA98765432ULL;

	qi_g.hi=0x23456789ABCDEF01ULL;
	qi_g.lo=0x23456789ABCDEF00ULL;

	qi_h.hi=0xFFFEDCBA98765432ULL;
	qi_h.lo=0x10FEDCBA98765432ULL;

	
	qj_a=BGBCC_Int128_Add(qi_a, qi_a);
	qj_b=BGBCC_Int128_Sub(qi_a, qi_a);

//	qj_c=BGBCC_Int128_Add(qi_b, qi_c);

	qj_c=BGBCC_Int128_Add(qi_b, qi_b);
	
	if(!BGBCC_Int128_CmpEq(qi_c, qj_a))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Int128_CmpEq(qi_d, qj_b))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Int128_CmpEq(qi_e, qj_c))
		{ BGBCC_DBGBREAK }

	qj_a=BGBCC_Int128_Shl(qi_a, 8);
	qj_b=BGBCC_Int128_Sar(qi_b, 8);
	qj_c=BGBCC_Int128_Shr(qi_b, 8);

	if(!BGBCC_Int128_CmpEq(qi_g, qj_a))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Int128_CmpEq(qi_h, qj_b))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Int128_CmpEq(qi_f, qj_c))
		{ BGBCC_DBGBREAK }

	return(0);
}

int BGBCC_Float128_Init_QF()
{
	bgbcc_vfloat128 qf_a, qf_b, qf_c, qf_d, qf_e, qf_f;
	bgbcc_vfloat128 qg_a, qg_b, qg_c, qg_d, qg_e;
	double f, g, h;

	qf_a=BGBCC_Float128_FromDouble( 3.0);
	qf_b=BGBCC_Float128_FromDouble( 5.0);
	qf_c=BGBCC_Float128_FromDouble( 8.0);
	qf_d=BGBCC_Float128_FromDouble(-2.0);
	qf_e=BGBCC_Float128_FromDouble(15.0);
	qf_f=BGBCC_Float128_FromDouble(3.0/5.0);

	qg_a=BGBCC_Float128_Add(qf_a, qf_b);
	qg_b=BGBCC_Float128_Sub(qf_a, qf_b);
	qg_c=BGBCC_Float128_Mul(qf_a, qf_b);
	qg_d=BGBCC_Float128_Div(qf_a, qf_b);
	
	if(!BGBCC_Float128_CmpEq(qg_a, qf_c))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Float128_CmpEq(qg_b, qf_d))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Float128_CmpEq(qg_c, qf_e))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Float128_CmpEqD(qg_d, qf_f))
		{ BGBCC_DBGBREAK }


	qf_a=BGBCC_Float128_FromDouble(3.1415926535897932384626433832795);
	qf_b=BGBCC_Float128_FromDouble(2.7182818284590452353602874713527);
	qf_c=BGBCC_Float128_FromDouble(5.8598744820488384738229308546322);
	qf_d=BGBCC_Float128_FromDouble(8.5397342226735670654635508695467);

	qg_a=BGBCC_Float128_Add(qf_a, qf_b);
	qg_b=BGBCC_Float128_Mul(qf_a, qf_b);

	if(!BGBCC_Float128_CmpEqD(qg_a, qf_c))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_Float128_CmpEqD(qg_b, qf_d))
		{ BGBCC_DBGBREAK }


	g=3.1415926535897932384626433832795;
	qg_e=BGBCC_Float128_FromDouble(g);
	f=BGBCC_Float128_ToDouble(qg_e);
	if(f!=g)
		{ BGBCC_DBGBREAK }

	qg_e=BGBCC_Float128_FromString("3.1415926535897932384626433832795");
//	qg_e=BGBCC_Float128_FromString("3.14");
	f=BGBCC_Float128_ToDouble(qg_e);
	g=3.1415926535897932384626433832795;
	if(f!=g)
		{ BGBCC_DBGBREAK }

//	BGBCC_DBGBREAK

	return(0);
}

int BGBCC_Float128_Init()
{
	BGBCC_Float128_Init_QI();
	BGBCC_Float128_Init_QF();

	return(0);
}

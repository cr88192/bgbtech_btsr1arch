bgbcc_vint128 BGBCC_Int128_Add(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;
	u64 pa, pb, pc, pd;
	
	pa=((u64)a.la)+((u64)b.la);
	pb=((u64)a.lb)+((u64)b.lb);
	pc=((u64)a.lc)+((u64)b.lc);
	pd=((u64)a.ld)+((u64)b.ld);
	
	c.lo=(pb<<32)+pa;
	c.hi=(pd<<32)+pc+((pb+(pa>>32))>>32);
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Neg(bgbcc_vint128 a)
{
	bgbcc_vint128 c;
	
	u64 pa, pb, pc, pd;

	pa=(~a.la)+1ULL;
	pb=(~a.lb)+(pa>>32);
	pc=(~a.lc)+(pb>>32);
	pd=(~a.ld)+(pc>>32);
	
	c.la=pa;
	c.lb=pb;
	c.lc=pc;
	c.ld=pd;
	return(c);
	
//	c.lo=(~a.lo)+1;
//	c.hi=(~a.hi)+ci;
}

bgbcc_vint128 BGBCC_Int128_Not(bgbcc_vint128 a)
{
	bgbcc_vint128 c;

	c.lo=~a.lo;
	c.hi=~a.hi;
	return(c);
}

int BGBCC_Int128_LNot(bgbcc_vint128 a)
{
	if(!a.lo && !a.hi)
		return(1);
	return(0);
}

bgbcc_vint128 BGBCC_Int128_Sub(bgbcc_vint128 a, bgbcc_vint128 b)
{
	return(BGBCC_Int128_Add(a, BGBCC_Int128_Neg(b)));
}

int BGBCC_Int64_AdcW(u64 a, u64 b, u64 *rc, byte ci)
{
	u64 pa, pb;

	pa=BGBCC_U64_LO32(a)+BGBCC_U64_LO32(b)+ci;
	pb=BGBCC_U64_HI32(a)+BGBCC_U64_HI32(b)+(pa>>32);
	*rc=BGBCC_U64_LO32(pa)+(pb<<32);
	return(pb>>32);
}

int BGBCC_Int64_MuluW(u64 a, u64 b, u64 *rc, u64 *rd)
{
	u64 pa, pb, pc, pd;
	u64 qa, qb, qc, qd;
	
	pa=BGBCC_U64_LO32(a)*BGBCC_U64_LO32(b);		//0
	pb=BGBCC_U64_LO32(a)*BGBCC_U64_HI32(b);		//1
	pc=BGBCC_U64_HI32(a)*BGBCC_U64_LO32(b);		//1
	pd=BGBCC_U64_HI32(a)*BGBCC_U64_HI32(b);		//2
	
	qa=BGBCC_U64_LO32(pa);
	qb=BGBCC_U64_HI32(pa)+
		BGBCC_U64_LO32(pb)+
		BGBCC_U64_LO32(pc);
	qc=BGBCC_U64_HI32(pb)+BGBCC_U64_HI32(pc)+BGBCC_U64_LO32(pd);
	qd=BGBCC_U64_HI32(pd);
	
	qc+=qb>>32;
	qd+=qc>>32;
	
	*rc=BGBCC_U64_LO32(qa)+(BGBCC_U64_LO32(qb)<<32);
	*rd=BGBCC_U64_LO32(qc)+(BGBCC_U64_LO32(qd)<<32);
	return(0);
}

int BGBCC_Int128_MulW(bgbcc_vint128 a, bgbcc_vint128 b,
	bgbcc_vint128 *rc, bgbcc_vint128 *rd)
{
	bgbcc_vint128 c, d;

	u64	paa, pab, pba, pbb;
	u64 pca, pcb, pda, pdb;
	u64	qaa, qab, qba, qbb;
	u64 qca, qcb, qda, qdb;

	u64	saa, sab, sba, sbb;

	int cb, cc;

	BGBCC_Int64_MuluW(a.lo, b.lo, &paa, &pab);
	BGBCC_Int64_MuluW(a.lo, b.hi, &pba, &pbb);
	BGBCC_Int64_MuluW(a.hi, b.lo, &pca, &pcb);
	BGBCC_Int64_MuluW(a.hi, b.hi, &pda, &pdb);

//	qaa=BGBCC_U64_LO32(paa);
	qab=BGBCC_U64_LO32(pab)+
		BGBCC_U64_LO32(pba)+
		BGBCC_U64_LO32(pca);
	qba=BGBCC_U64_LO32(pbb)+
		BGBCC_U64_LO32(pcb)+
		BGBCC_U64_LO32(pda);
//	qbb=BGBCC_U64_LO32(pdb);

//	qca=BGBCC_U64_HI32(paa);
	qcb=BGBCC_U64_HI32(pab)+
		BGBCC_U64_HI32(pba)+
		BGBCC_U64_HI32(pca);
	qda=BGBCC_U64_HI32(pbb)+
		BGBCC_U64_HI32(pcb)+
		BGBCC_U64_HI32(pda);
//	qdb=BGBCC_U64_HI32(pdb);
	
	saa=paa;
	sab=qab+(qcb<<32);
	sba=qba+(qda<<32);
//	sbb=qbb+(qdb<<32);
	sbb=pdb;
	
	sba+=(qcb+(qab>>32))>>32;
	sbb+=(qda+(qba>>32))>>32;
	
	c.lo=saa;
	c.hi=sab;
	d.lo=sba;
	d.hi=sbb;

	*rc=c;
	*rd=d;

	return(0);
//	return(c);
}

bgbcc_vint128 BGBCC_Int128_Mul(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;

	u64	paa, pab, pba, pbb;
	u64 pca, pcb, pda, pdb;

	BGBCC_Int64_MuluW(a.lo, b.lo, &paa, &pab);
	BGBCC_Int64_MuluW(a.lo, b.hi, &pba, &pbb);
	BGBCC_Int64_MuluW(a.hi, b.lo, &pca, &pcb);
		
	c.lo=paa;
	c.hi=pab+pba+pca;
	return(c);
}

bgbcc_vint128 BGBCC_Int128_MulHA(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;

	u64	paa, pab, pba, pbb;
	u64 pca, pcb, pda, pdb;
	u64 qa, qb;
	int ci, cj;

	BGBCC_Int64_MuluW(a.lo, b.hi, &pba, &pbb);
	BGBCC_Int64_MuluW(a.hi, b.lo, &pca, &pcb);
	BGBCC_Int64_MuluW(a.hi, b.hi, &pda, &pdb);	
	ci=BGBCC_Int64_AdcW(pbb, pcb, &qa, 0);
	cj=BGBCC_Int64_AdcW(qa, pda, &qb, 0);
	c.lo=qb;
	c.hi=pdb+ci+cj;
	return(c);
}

bgbcc_vint128 BGBCC_Int128_And(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;
	c.lo=a.lo&b.lo;
	c.hi=a.hi&b.hi;
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Or(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;
	c.lo=a.lo|b.lo;
	c.hi=a.hi|b.hi;
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Xor(bgbcc_vint128 a, bgbcc_vint128 b)
{
	bgbcc_vint128 c;
	c.lo=a.lo^b.lo;
	c.hi=a.hi^b.hi;
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Shl(bgbcc_vint128 a, int shl)
{
	bgbcc_vint128 c;

	shl&=127;
	if(shl>0)
	{
		if(shl>=64)
		{
			c.hi=a.lo<<(shl-64);
			c.lo=0;
		}else
		{
			c.lo=a.lo<<shl;
			c.hi=(a.hi<<shl)|(a.lo>>(64-shl));
		}
	}else
	{
		c.lo=a.lo;
		c.hi=a.hi;
	}
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Shr(bgbcc_vint128 a, int shl)
{
	bgbcc_vint128 c;

	shl&=127;
	if(shl>0)
	{
		if(shl>=64)
		{
			c.lo=a.hi>>(shl-64);
			c.hi=0;
		}else
		{
			c.lo=(a.lo>>shl)|(a.hi<<(64-shl));
			c.hi=(a.hi>>shl);
		}
	}else
	{
		c.lo=a.lo;
		c.hi=a.hi;
	}
	return(c);
}

bgbcc_vint128 BGBCC_Int128_Sar(bgbcc_vint128 a, int shl)
{
	bgbcc_vint128 c;
	u64 sx;

	shl&=127;
	if(shl>0)
	{
//		sx=~((a.hi>>63)-1);
		sx=-(a.hi>>63);

		if(shl>=64)
		{
			if(shl>64)
			{
				c.lo=(a.hi>>(shl-64))|(sx<<(128-shl));
				c.hi=sx;
			}else
			{
				c.lo=a.hi;
				c.hi=sx;
			}
		}else
		{
			c.lo=(a.lo>>shl)|(a.hi<<(64-shl));
			c.hi=(a.hi>>shl)|(sx<<(64-shl));
		}
	}else
	{
		c.lo=a.lo;
		c.hi=a.hi;
	}
	return(c);
}

int BGBCC_Int128_CmpEq(bgbcc_vint128 a, bgbcc_vint128 b)
{
	if(a.hi!=b.hi)
		return(0);
	if(a.lo!=b.lo)
		return(0);
	return(1);
}

int BGBCC_Int128_CmpGt(bgbcc_vint128 a, bgbcc_vint128 b)
{
	if(((s64)(a.hi))>((s64)(b.hi)))
		return(1);
	if(((s64)(a.hi))<((s64)(b.hi)))
		return(0);
	if(a.lo>b.lo)
		return(1);
	return(0);
}

int BGBCC_Int128_CmpHi(bgbcc_vint128 a, bgbcc_vint128 b)
{
	if(a.hi>b.hi)
		return(1);
	if(a.hi<b.hi)
		return(0);
	if(a.lo>b.lo)
		return(1);
	return(0);
}

int BGBCC_Int128_CmpNe(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(!BGBCC_Int128_CmpEq(a, b)); }

int BGBCC_Int128_CmpLe(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(!BGBCC_Int128_CmpGt(a, b)); }
int BGBCC_Int128_CmpLt(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(BGBCC_Int128_CmpGt(b, a)); }
int BGBCC_Int128_CmpGe(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(!BGBCC_Int128_CmpGt(b, a)); }

int BGBCC_Int128_CmpA(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(BGBCC_Int128_CmpHi(a, b)); }
int BGBCC_Int128_CmpBe(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(!BGBCC_Int128_CmpHi(a, b)); }
int BGBCC_Int128_CmpB(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(BGBCC_Int128_CmpHi(b, a)); }
int BGBCC_Int128_CmpAe(bgbcc_vint128 a, bgbcc_vint128 b)
	{ return(!BGBCC_Int128_CmpHi(b, a)); }


int BGBCC_Int128_CheckSx64P(bgbcc_vint128 a)
{
	if(!a.hi)
	{
		if(a.lo&0x8000000000000000ULL)
			return(0);
		return(1);
	}

	if(a.hi==((u64)(-1)))
	{
		if(a.lo&0x8000000000000000ULL)
			return(1);
		return(0);
	}

	return(0);
}

#ifdef __BGBCC__

#define XLF_GETBITS(x)	((__uint128)((__m128)(x)))
#define XLF_FROMBITS(x)	((__float128)((__m128)(x)))

#define XDB_GETBITS(x)	((__uint64)((__m64)(x)))
#define XDB_FROMBITS(x)	((__float64)((__m64)(x)))

// #ifdef __BJX2__
#if 0

__float128 __xlf_add(__float128 x, __float128 y);
__float128 __xlf_sub(__float128 x, __float128 y);
__float128 __xlf_mul(__float128 x, __float128 y);
__float128 __xlf_div(__float128 x, __float128 y);

__float128 __xlf_neg(__float128 x);

int __xlf_cmp_eq(__float128 a, __float128 b);
int __xlf_cmp_ne(__float128 a, __float128 b);
int __xlf_cmp_gt(__float128 a, __float128 b);
int __xlf_cmp_ge(__float128 a, __float128 b);

__asm {
__xlf_neg:
	MOV		0x8000000000000000, R1
	MOV		R4, R2
	XOR		R5, R1, R3
	RTS
};

__asm {
.extern	__xli_umul_hi
.extern	__xli_shl

__xlf_mul:
	ADD		-96, SP
	MOV		LR, R1
	MOV.X	R26, (SP, 80)
	MOV.X	R24, (SP, 64)
	MOV.Q	R1 , (SP, 56)
	MOV.Q	R14, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8 , (SP,  0)

	SHAD.Q	R5, -48, R16
	SHAD.Q	R7, -48, R17
	XOR		R16, R17, R19
	AND		0x7FFF, R16
	AND		0x7FFF, R17
	AND		0x8000, R18
	CMPGT	0, R16
	BF		.RetZero
	CMPGT	0, R17
	BF		.RetZero
	ADD		R16, R17, R14
	ADD		-16383, R14
	CMPGT	0, R14
	BF		.RetZero
	OR		R19, R14
	
	/*
	 * R14: Sign+Exponent
	 */
	
	MOV		0x0000FFFFFFFFFFFF, R8
	MOV		0x0001000000000000, R9
	MOV		0x0000000200000000, R10
	AND		R8, R5
	AND		R8, R7
	OR		R9, R5
	OR		R9, R7

	BSR		__xli_umul_hi

	MOV		R2, R4
	MOV		R3, R5
	MOV		16, R6

	TEST	R10, R5
	MOV?F	15, R6
	ADD?F	R14, 1, R14

	BSR		__xli_shl

	AND		R8, R5
	AND		R8, R7
	SHLD.Q	R19, 48, R20
	OR		R20, R3
	BRA		.Ret
	
	.RetZero:
	MOV		0, R2
	MOV		0, R3

	.Ret:
	MOV.X	(SP, 80), R26
	MOV.X	(SP, 64), R24
	MOV.Q	(SP, 56), R1
	MOV.Q	(SP, 48), R14
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 16), R10
	MOV.X	(SP,  0), R8
	ADD		96, SP
	JMP		R1
};

__asm {
.extern	__xli_shar
__xlf_sub:
	MOV		0x8000000000000000, R1
	XOR		R1, R7
	BRA		__xlf_add

__xlf_add:
	ADD		-128, SP
	MOV		LR, R1
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R1 , (SP,  56)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	/*
	 * R8: Exp_A
	 * R9: Exp_B
	 * R10: Exp_Max
	 * R11: Temp1
	 * R12: Exp_Max-Exp_A
	 * R13: Exp_Max-Exp_B
	 * R14: Sign
	 *
	 * R24: A_Lo
	 * R25: A_Hi
	 * R26: B_Lo
	 * R27: B_Hi
	 * R28: Temp2
	 * R29: Temp3
	 * R30: FraC_Lo
	 * R31: FraC_Hi
	 */

	MOV		R4, R24
	MOV		R5, R25
	MOV		R6, R26
	MOV		R7, R27

	SHAD.Q	R5, -48, R8
	SHAD.Q	R7, -48, R9
//	XOR		R8, R9, R10
	AND		0x7FFF, R8
	AND		0x7FFF, R9
//	AND		0x8000, R10

	CMPGT	R9, R8
	CSELT	R8, R9, R10

	MOV		0x0000FFFFFFFFFFFF, R28
	MOV		0x0001000000000000, R29
	MOV		0x8000000000000000, R11

	SUB		R10, R8, R12
	SUB		R10, R9, R13
	
	MOV		R24, R4
	AND		R25, R28, R5
	OR		R29, R5
	MOV		R12, R6
	
	TEST	R11, R25
	NOT?F	R4, R4
	NOT?F	R5, R5
	BSR		__xli_shar
	MOV		R2, R30
	MOV		R3, R31


	MOV		R26, R4
	AND		R27, R28, R5
	OR		R29, R5
	MOV		R13, R6
	
	TEST	R11, R27
	NOT?F	R4, R4
	NOT?F	R5, R5
	BSR		__xli_shar

	CLRT
	ADDC	R2, R30
	ADDC	R3, R31

	MOV		0x0000, R14
	TEST	R11, R31
	NOT?F	R30, R30
	NOT?F	R31, R31
	MOV?F	0x8000, R14
	
	OR		R30, R31, R6
	TEST	R6, R6
	BT		.RetZero

	MOV		0xFFFE000000000000, R7
	TEST	R7, R31
	BT		.L1
	.L0:
	SHLD.Q	R30, -1, R30
	TEST	1, R31
	OR?F	R11, R30
	SHLD.Q	R31, -1, R31
	ADD		1, R10
	TEST	R7, R31
	BF		.L0
	.L1:

	TEST	R29, R31
	BF		.L3
	.L2:
	ADDC	R30, R30
	ADDC	R31, R31
	ADD		-1, R10
	TEST	R29, R31
	BT		.L2
	.L3:
	
	CMPGT	0, R10
	BF		.RetZero
	
	OR		R14, R10
	SHLD.Q	R10, 48, R7
	
	MOV		R30, R2
	AND		R31, R28, R3
	OR		R7, R3

	.Ret:
	MOV.X	(SP, 112), R30
	MOV.X	(SP,  96), R28
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  64), R24
	MOV.Q	(SP,  56), R1
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  32), R12
	MOV.X	(SP,  16), R10
	MOV.X	(SP,   0), R8
	ADD		128, SP
	JMP		R1
	.RetZero:
	MOV		0, R2
	MOV		0, R3
	BRA		.Ret
};

__asm {
__xlf_cmp_eq:
	CMPQEQ	R5, R7
	BF		.False
	CMPQEQ	R4, R6
	BF		.False

	/* Bit patterns are equal, check for NaNs */
	SHLD.Q	R5, -48, R16
	SHLD.Q	R7, -48, R17
	MOV	0x7FFF, R1
	MOV	0x0000FFFFFFFFFFFF, R18
	AND	R1, R16
	AND	R1, R17
	CMPEQ	R16, R1
	BF .L0
	TSTQ	R18, R5
	BF .False
	.L0:
	CMPEQ	R17, R1
	BF .L1
	TSTQ	R18, R7
	BF .False
	.L1:

	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
};

__asm {
__xlf_cmp_ne:
	CMPQEQ	R5, R7
	BF		.True
	CMPQEQ	R4, R6
	BF		.True
	MOV		0, R2
	RTS
	.True:
	MOV		1, R2
	RTS
};

__asm {
__xlf_cmp_gt:
	MOV		0x8000000000000000, R3
	TSTQ	R5, R3
	BF		.N0
	BRA		.P0
	.N0:
	TSTQ	R7, R3
	BF		.NN
	BRA		.False
	.P0:
	TSTQ	R7, R3
	BT		.PP
	BRA		.True
	.PP:
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R6, R4
	BF		.False
	BRA		.True
	.NN:
	CMPQHI	R7, R5
	BT		.False
	CMPQHI	R4, R6
	BF		.False
	BRA		.True
	.False:
	MOV		0, R2
	RTS
	.True:
	MOV		1, R2
	RTS
};

__asm {
__xlf_cmp_ge:
	MOV		0x8000000000000000, R3
	TSTQ	R5, R3
	BF		.N0
	BRA		.P0
	.N0:
	TSTQ	R7, R3
	BF		.NN
	BRA		.False
	.P0:
	TSTQ	R7, R3
	BT		.PP
	BRA		.True
	.PP:
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R4, R6
	BT		.False
	BRA		.True
	.NN:
	CMPQHI	R7, R5
	BT		.False
	CMPQHI	R6, R4
	BT		.False
	BRA		.True
	.False:
	MOV		0, R2
	RTS
	.True:
	MOV		1, R2
	RTS
};

double __xlf_todbl(__float128 x);
__float128 __xlf_fromdbl(double x);
long long __xlf_toint(__float128 x);
__float128 __xlf_fromint(long long x);

__asm {
__xlf_todbl:
	MOV		-60, R16
	MOV		0x3C00000000000000, R17
	MOV		0x07FE000000000000, R18
	MOV		0x7FFF000000000000, R19
	MOV		0x000FFFFFFFFFFFFF, R20
	MOV		0x8000000000000000, R21
	SHLD.X	R4, R16, R2
	SUB		R5, R17, R3
	AND		R5, R21, R23
	AND		R20, R2
	AND		R19, R3
	CMPQHI	R18, R3
	BT		.RangeFail
	SHLD.Q	R3, 4, R3
	OR		R3, R2
	OR		R23, R2
	RTS
	.RangeFail:
	MOV		0x4000000000000000, R22
	TSTQ	R22, R3
	BF		.Inf
	MOV		0, R2
	RTS
	.Inf:
	MOV		0x7FF0000000000000, R2
	OR		R23, R2
	RTS
};

__asm {
__xlf_fromdbl:
	MOV		60, R16
	MOV		0x3C00000000000000, R17
	MOV		0x07FF000000000000, R18
	MOV		0x7FFF000000000000, R19
	MOV		0x000FFFFFFFFFFFFF, R20
	MOV		0x8000000000000000, R21
	MOV		R4, R2
	MOV		0, R3
	AND		R4, R21, R23
	SHLD.X	R2, R16, R2
	AND		R18, R3
	TSTQ	R18, R3
	ADD?F	R17, R3
	OR		R23, R3
	RTS
};

__asm {
__xlf_toint:
	MOV		LR, R1
	BSR		__xlf_todbl
	FSTCI	R2, R2
	JMP		R1
};

__asm {
__xlf_fromint:
	FLDCI	R4, R4
//	JMP		__xlf_fromdbl
	BRA		__xlf_fromdbl
};

#else

double __xlf_todbl(__float128 x)
{
	__uint128 a, b, c, cs, fra;
	__uint64 lc;
	int exa, exb, sga, rup;

	a=XLF_GETBITS(x);
	exa=(a>>112)&32767;
	exb=exa-(16383-1023);
	sga=(a>>127)&1;
	fra=a&0x0000_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFFUI128;
	if(exa)		fra|=0x0001_0000_0000_0000_0000_0000_0000_0000UI128;
	
	if(exb<0)
	{
		if(exb<=(-52))
			return(0);
		fra=fra>>(-exb);
		exb=0;
	}else
		if(exb>=2047)
	{
		lc=0x7FF0000000000000ULL|(((u64)sga)<<63);
		return(XDB_FROMBITS(lc));
	}
	
	rup=(fra>>59)&1;
	
	b=((fra>>60)&0x000FFFFFFFFFFFFFULL)|(((u64)exb)<<52)|(((u64)sga)<<63);
	if(rup)		b=b+1;

	lc=b;
	return(XDB_FROMBITS(lc));
}

__float128 __xlf_fromdbl(double x)
{
	__uint128 a, b, c, cs;
	__uint128 fra, frb, frc, frcm, frhb, frsg;
	__float128 z;
	int exa, exb, exc, sga, sgb, sgc;

	a=XDB_GETBITS(x);
	exa=(a>>52)&2047;
	sga=(a>>63)&1;

	if(a==0)
	{
		c=0;
		z=XLF_FROMBITS(c);
		return(z);
	}
	
	if(exa==0)
	{
		frb=a&0x000FFFFFFFFFFFFFUI128;
		exb=15360;
		while(!(frb>>52))
			{ frb<<=1; exb--; }
		frb&=0x000FFFFFFFFFFFFFUI128;
		b=(frb<<60)|(((__uint128)exb)<<112)|(((__uint128)sga)<<112);
		z=XLF_FROMBITS(b);
		return(z);
	}
	
	if(exa==2047)
	{
		b=((a&0x7FFFFFFFFFFFFFFFUI128)<<60);
		b|=0x3FFF0000000000000000000000000000UI128;
		b|=((a&0x8000000000000000UI128)<<64);
		z=XLF_FROMBITS(b);
		return(z);
	}
	
	b=(a&0x7FFFFFFFFFFFFFFFUI128)<<60;
	b+=0x3C000000000000000000000000000000UI128;
	b|=((a&0x8000000000000000UI128)<<64);

	z=XLF_FROMBITS(b);
	return(z);
}

__int128 __xlf_toint128(__float128 x)
{
	__uint128 a, b, c, cs;
	__uint128 fra, frb, frc, frcm, frhb, frsg;
	__float128 z;
	int exa, exb, exc, sga, sgb, sgc;

	a=XLF_GETBITS(x);
	exa=(a>>112)&32767;
	sga=(a>>127)&1;

	frcm=0x0000_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFFUI128;
	frhb=0x0001_0000_0000_0000_0000_0000_0000_0000UI128;
	frsg=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;
	fra=a&frcm;
	if(exa)	fra|=frhb;

	if(exa<16383)
	{
		c=0;
	}else
		if(exa<16495)
	{
		c=fra>>(112-(exa-16383));
	}else
		if(exa<16510)
	{
		c=fra<<(exa-16495);
	}else
	{
		c=~frsg;
	}
	
	if(sga)
	{
		c=(~c)+1;
	}
	
	return(c);
}

__float128 __xlf_from_frac(__uint128 frc, int exc, int sgc)
{
	__uint128 c, frcm, frhb, frsg;
	__float128 z;
	int j, rup;

	if((!frc) || (exc<=(-124)))
	{
		c=0;
		z=XLF_FROMBITS(c);
		return(z);
	}

	if((frc>>127)&1)
	{
		frc=~frc;
		sgc=!sgc;
	}

	frcm=0x0000_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFFUI128;
//	frhb=0x0001_0000_0000_0000_0000_0000_0000_0000UI128;
	frsg=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;

	j=(int)(frc>>120);
	if(j>=32)
	{
		while((frc>>125)!=0)
			{ frc=frc>>1; exc++; }
	}else
		if(j<16)
	{
		if(!j)
		{
			if((frc>>60)==0)
				{ frc=frc<<64; exc-=64; }
			while(!((int)(frc>>108)))
				{ frc=frc<<16; exc-=16; }
			while(!((int)(frc>>120)))
				{ frc=frc<<4; exc-=4; }
		}
		while(!((int)(frc>>124)))
			{ frc=frc<<1; exc--; }
	}

	if(exc<0)
	{
		if(exc<=(-112))
		{
			c=0;
			z=XLF_FROMBITS(c);
			return(z);
		}
		
		frc=frc>>(-exc);
		exc=0;
		
		c=frc;
		if(sgc)		c|=frsg;

		z=XLF_FROMBITS(c);
		return(z);
	}
	
	if(exc>=32767)
	{
		frc=0;
		exc=32767;
	}
	
	rup=(frc>>11)&1;
	frc>>=12;

	c=(frc&frcm)|(((__uint128)exc)<<112);
	if(sgc)		c|=frsg;
	if(rup)		c=c+1;

	z=XLF_FROMBITS(c);
	return(z);
}

__float128 __xlf_fromint128(__int128 li)
{
	__uint128 c;
	__uint128 frc;
	__float128 z;
	int exc, sgc;

	if(li==0)
	{
		c=0;
		z=XLF_FROMBITS(c);
		return(z);
	}

	frc=li;
	sgc=0;
	exc=16383+124;
	
	if(li<0)
	{
		frc=-li;
		sgc=1;
	}

	return(__xlf_from_frac(frc, exc, sgc));
}

long long __xlf_toint(__float128 x)
{
	return(__xlf_toint128(x));
}

__float128 __xlf_fromint(long long x)
{
	return(__xlf_fromint128(x));
}

__float128 __xlf_add(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;
	__uint128 fra, frb, frb1, frc, frcm, frhb, frsg;
	__float128 z;
	int exa, exb, exc, sga, sgb, sgc, shl;

	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);

	exa=(a>>112)&32767;
	exb=(b>>112)&32767;
	sga=(a>>127)&1;
	sgb=(b>>127)&1;

	frcm=0x0000_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFFUI128;
	frhb=0x0001_0000_0000_0000_0000_0000_0000_0000UI128;
	frsg=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;
	fra=a&frcm;
	frb=b&frcm;
	if(exa)	fra|=frhb;
	if(exb)	frb|=frhb;
	
	fra=fra<<12;
	frb=frb<<12;
	
	if((exb>exa) || ((exb==exa) && (frb>fra)))
	{
		frc=fra; fra=frb; frb=frc;
		exc=exa; exa=exb; exb=exc;
		sgc=sga; sga=sgb; sgb=sgc;
	}

	shl=exa-exb;
	frb1=frb>>shl;

	if(!(sga^sgb))
	{
		frc=fra+frb1;
		exc=exa;
		sgc=sga;
	}else
	{
		frc=fra-frb1;
		exc=exa;
		sgc=sga;
		
		if(frc==0)
		{
			z=XLF_FROMBITS(frc);
			return(z);
		}
	}
	
	return(__xlf_from_frac(frc, exc, sgc));
}

__float128 __xlf_sub(__float128 x, __float128 y)
	{ return(__xlf_add(x, -y)); }

__uint128 __xli_mul128lo(__uint128 a, __uint128 b)
{
	__uint128 c;
	c=a*b;
	return(c);
}

int __xli_dmul128u(__uint128 a, __uint128 b, __uint128 *rcl, __uint128 *rch);

#if 0
int __xli_dmul128u(__uint128 a, __uint128 b, __uint128 *rcl, __uint128 *rch)
{
	__uint128 ch, cl, cm, hh, hl, lh, ll, ah, al, bh, bl;
	int cms;

	ah=a>>64;	al=(u64)a;
	bh=b>>64;	bl=(u64)b;

	hh=ah*bh;	hl=ah*bl;
	lh=al*bh;	ll=al*bl;
	cm=hl+lh;
	cms=cm<hl;
	
	ch=hh+(cm>>64);
	cl=ll+(cm<<64);
	if(cl<ll)	ch+=1UI128;
	if(cms)		ch+=1UI128<<64;
	*rcl=cl;	*rch=ch;
	return(0);
}
#endif

__float128 __xlf_mul(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;
	__uint128 fra, frb, frc, frch, frcl, frcm, frhb, frsg;
	__float128 z;
	int exa, exb, exc, sga, sgb, sgc, rndh;

	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);

	exa=(a>>112)&32767;
	exb=(b>>112)&32767;
	sga=(a>>127)&1;
	sgb=(b>>127)&1;

	if((a==0) || (b==0))
	{
//		__debugbreak();

		c=0;
		z=XLF_FROMBITS(c);
		return(z);
	}

	frcm=0x0000_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFFUI128;
	frhb=0x0001_0000_0000_0000_0000_0000_0000_0000UI128;
	frsg=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;
	fra=a&frcm;
	frb=b&frcm;
	if(exa)	fra|=frhb;
	if(exb)	frb|=frhb;

	__xli_dmul128u(fra, frb, &frcl, &frch);
//	frch=(frch<<16)|(frcl>>112);
//	frcl=(frcl<<16);
	frch=(frch<<28)|(frcl>>100);
	frcl=(frcl<<28);

//	__debugbreak();

	frc=frch;
	
	exc=exa+exb-16383;
	sgc=sga^sgb;

	return(__xlf_from_frac(frc, exc, sgc));
}

// __float128 __xlf_div(__float128 x, __float128 y);

int __xlf_dbgprint_lln(__float128 x, char *lfn, int lln)
{
	__uint128 a;

	a=XLF_GETBITS(x);
	printf("%s:%d %016llX:%016llX\n",
		lfn, lln,
		(long long)(a>>64), (long long)a);
	return(0);
}

#define __xlf_dbgprint(x)	__xlf_dbgprint_lln(x, __FILE__, __LINE__)


#if defined(__BJX2__) || defined(__RISCV__)
// #if 0

__float128 __xlf_neg(__float128 x);

#ifdef __BJX2__

__asm {
__xlf_neg:
	MOV		0x8000000000000000, R1
	MOV		R4, R2
	XOR		R5, R1, R3
	RTS
};

#endif

#ifdef __RISCV__

__asm {
__xlf_neg:
	MOV		0x8000000000000000, R17
	XOR		R11, R17, R11
	RTS
};

#endif

#else
__float128 __xlf_neg(__float128 x)
{
	__uint128 b, c, cs;
	__float128 y;

	b=XLF_GETBITS(x);
	
	cs=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;
	c=b^cs;
	
//	__debugbreak();
	
	y=XLF_FROMBITS(c);
	return(y);
}
#endif

int __xlf_cmp_eq(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;
	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);
	return(a==b);
}

int __xlf_cmp_ne(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;
	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);
	return(a!=b);
}

int __xlf_cmp_gt(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;

	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);
	cs=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;

	if(!(a&cs))
	{
		if(!(b&cs))
		{
			return(a>b);
		}else
		{
			return(1);
		}
	}else
	{
		if(!(b&cs))
		{
			return(0);
		}else
		{
			return(a<b);
		}
	}
}

int __xlf_cmp_ge(__float128 x, __float128 y)
{
	__uint128 a, b, c, cs;

	a=XLF_GETBITS(x);
	b=XLF_GETBITS(y);
	cs=0x8000_0000_0000_0000_0000_0000_0000UI128;

	if(!(a&cs))
	{
		if(!(b&cs))
		{
			return(a>=b);
		}else
		{
			return(1);
		}
	}else
	{
		if(!(b&cs))
		{
			return(0);
		}else
		{
			return(a<=b);
		}
	}
}



#endif


#if 1

__float128 __xlf_rcp(__float128 x)
{
	__float128 y, z, c2, c3s8, c5s8;
	__uint128 b, c, cs, p, q, r;
	int sg;
	int i;
	
	b=XLF_GETBITS(x);
	
	sg=0;
	cs=0x8000_0000_0000_0000_0000_0000_0000_0000UI128;
	if(b&cs)
	{
//		__debugbreak();
		sg=1;
//		b^=cs;
		x=-x;
		b=XLF_GETBITS(x);
	}
	
	c=0x7FFE_0000_0000_0000_0000_0000_0000_0000UI128;
	p=c-b;

	y=XLF_FROMBITS(p);
//	__xlf_dbgprint(y);

#if 1
	c=0x3FFF_0000_0000_0000_0000_0000_0000_0000UI128;
	z=x*XLF_FROMBITS(p);

//	__xlf_dbgprint(z);

	q=XLF_GETBITS(z);
	r=((__int128)(c-q))>>1;

//	__debugbreak();
//	__xlf_dbgprint(XLF_FROMBITS(r));

	p=p+r;
	y=XLF_FROMBITS(p);
#endif

//	__xlf_dbgprint(y);

//	__debugbreak();

	c2=2.0;

#if 0
	c3s8=0.375;
	c5s8=0.625;

	__debugbreak();

	z=(c2-x*y)*c3s8+c5s8;
	y=y*z;
#endif

//	__debugbreak();

#if 0
	z=x*y;
	printf("W "); __xlf_dbgprint(z);
	printf("2 "); __xlf_dbgprint(c2);
	z=(c2-z);
	y=y*z;
	printf("Z "); __xlf_dbgprint(z);
	printf("Y "); __xlf_dbgprint(y);
#endif

	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);

//	__xlf_dbgprint(y);

#if 0
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
#endif

//	__xlf_dbgprint(y);

#if 0
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
	y=y*(c2-x*y);
#endif

//	__xlf_dbgprint(y);

	if(sg)
		y=-y;

//	__xlf_dbgprint(y);

	return(y);

#if 0
	c=0x3FFF_0000_0000_0000_0000_0000_0000_0000UI128;
	for(i=0; i<16; i++)
	{
		y=x*XLF_FROMBITS(p);
		q=XLF_GETBITS(y);
		p+=(c-q)>>1;
	}

	if(sg)
		{ p^=cs; }

	y=XLF_FROMBITS(p);
	return(y);
#endif
}

__float128 __xlf_div(__float128 x, __float128 y)
{
	return(x*__xlf_rcp(y));
}

#endif

#endif

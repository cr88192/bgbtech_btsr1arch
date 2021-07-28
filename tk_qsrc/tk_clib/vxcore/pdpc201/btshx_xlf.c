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
	
__xlf_toint:
	MOV		LR, R1
	BSR		__xlf_todbl
	FSTCI	R2, R2
	JMP		R1
__xlf_fromint:
	FLDCI	R4, R4
	JMP		__xlf_fromdbl

};


#if 1

#define XLF_GETBITS(x)	((__uint128)((__m128)(x)))
#define XLF_FROMBITS(x)	((__float128)((__m128)(x)))

__float128 __xlf_rcp(__float128 x)
{
	__float128 y;
	__uint128 b, c, cs, p, q, r;
	int sg;
	int i;
	
	b=XLF_GETBITS(x);
	
	sg=0;
	cs=0x8000_0000_0000_0000UI128;
	if(b&cs)
		{ sg=1; b^=cs; }
	
	c=0x7FFE_0000_0000_0000UI128;
	p=c-b;
	
	c=0x3FFF_0000_0000_0000UI128;
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
}

__float128 __xlf_div(__float128 x, __float128 y)
{
	return(x*__xlf_rcp(y));
}

#endif

__int128 __xli_add (__int128 a, __int128 b);
__int128 __xli_sub (__int128 a, __int128 b);
__int128 __xli_smul(__int128 a, __int128 b);
__int128 __xli_sdiv(__int128 a, __int128 b);
__int128 __xli_smod(__int128 a, __int128 b);
__int128 __xli_and (__int128 a, __int128 b);
__int128 __xli_or  (__int128 a, __int128 b);
__int128 __xli_xor (__int128 a, __int128 b);
__int128 __xli_shl (__int128 a, __int128 b);
__int128 __xli_shlr(__int128 a, __int128 b);
__int128 __xli_shar(__int128 a, __int128 b);

__int128 __xli_neg (__int128 a);
__int128 __xli_not (__int128 a);

int __xli_cmp_eq(__int128 a, __int128 b);
int __xli_cmp_ne(__int128 a, __int128 b);
int __xli_cmp_gt(__int128 a, __int128 b);
int __xli_cmp_ge(__int128 a, __int128 b);
int __xli_cmp_hi(__int128 a, __int128 b);
int __xli_cmp_he(__int128 a, __int128 b);

int __xli_cmp_tst(__int128 a, __int128 b);
int __xli_cmp_ntst(__int128 a, __int128 b);

__asm {

__xli_add:
.ifarch has_alux
	ADDX	R4, R6, R2
	RTS
.else
	MOV		R6, R2
	MOV		R7, R3
	CLRT
	ADDC	R4, R2
	ADDC	R5, R3
	RTS
.endif

__xli_sub:
.ifarch has_alux
	SUBX	R4, R6, R2
	RTS
.else
	MOV		R6, R2
	MOV		R7, R3
	CLRT
	SUBC	R4, R2
	SUBC	R5, R3
	RTS
.endif

__xli_and:
	AND		R4, R6, R2
	AND		R5, R7, R3
	RTS

__xli_or:
	OR		R4, R6, R2
	OR		R5, R7, R3
	RTS

__xli_xor:
	XOR		R4, R6, R2
	XOR		R5, R7, R3
	RTS

__xli_neg:
	CLRT
	MOV		0, R2
	MOV		0, R3
	SUBC	R4, R2
	SUBC	R5, R3
	RTSU

__xli_not:
	NOT		R4, R2
	NOT		R5, R3
	RTS

__xli_shl:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BT			.L1
	ADD			R4, -64, R7
	SHLD.Q		R4, R6, R2
	SHLD.Q		R5, R6, R3
	SHLD.Q		R4, R7, R16
	OR			R16, R3
	RTS
	.L0:
	SUB			R6, 64, R7
	SHLD.Q		R4, R7, R3
	MOV			0, R2
	RTS
	.L1:
	MOV			R4, R2
	MOV			R5, R3
	RTS

__xli_shlr:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BT			.L1
	ADD			R4, -64, R7
	NEG			R6, R6
	NEG			R7, R7
	SHLD.Q		R4, R6, R2
	SHLD.Q		R5, R6, R3
	SHLD.Q		R5, R7, R16
	OR			R16, R2
	RTS
	.L0:
	SUB			R6, 64, R7
	NEG			R7, R7
	SHLD.Q		R5, R7, R2
	MOV			0, R3
	RTS
	.L1:
	MOV			R4, R2
	MOV			R5, R3
	RTS

__xli_shar:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BT			.L1
	ADD			R4, -64, R7
	NEG			R6, R6
	NEG			R7, R7
	SHLD.Q		R4, R6, R2
	SHAD.Q		R5, R6, R3
	SHLD.Q		R5, R7, R16
	OR			R16, R2
	RTS
	.L0:
	SUB			R6, 64, R7
	NEG			R7, R7
	SHAD.Q		R5, R7, R2
	SHAD.Q		R5, -63, R3
	RTS
	.L1:
	MOV			R4, R2
	MOV			R5, R3
	RTS


__xli_umuldq:
	SHLD.Q	R4, -32, R6
	SHLD.Q	R5, -32, R7

	DMULU	R4, R5, R16
	DMULU	R4, R7, R17
	DMULU	R6, R5, R18
	DMULU	R6, R7, R19

	SHLD.Q	R17,  32, R20
	SHLD.Q	R17, -32, R21
	SHLD.Q	R18,  32, R22
	SHLD.Q	R18, -32, R23

	MOV		R16, R2
	MOV		R19, R3
	CLRT
	ADDC	R20, R2
	ADDC	R21, R3
	CLRT
	ADDC	R22, R2
	ADDC	R23, R3
	RTSU

#if 1
__xli_smul:
	ADD		-96, SP
	MOV		LR, R1
	MOV.Q	R1 , (SP, 56)
	MOV.Q	R14, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8 , (SP,  0)

	MOV		R4, R8
	MOV		R5, R9
	MOV		R6, R10
	MOV		R7, R11

	MOV		R8 , R4
	MOV		R10, R5
	BSR		__xli_umuldq

	DMULU	R8, R11, R16
	DMULU	R9, R10, R17
	ADD		R16, R17, R18
	ADD		R18, R3

	MOV.Q	(SP, 56), R1
	MOV.Q	(SP, 48), R14
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 16), R10
	MOV.X	(SP,  0), R8
	ADD		96, SP
	JMP		R1
#endif

#if 1
__xli_umul_hi:
	ADD		-96, SP
	MOV		LR, R1
	MOV.X	R26, (SP, 80)
	MOV.X	R24, (SP, 64)
	MOV.Q	R1 , (SP, 56)
	MOV.Q	R14, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8 , (SP,  0)

	MOV		R4, R8
	MOV		R5, R9
	MOV		R6, R10
	MOV		R7, R11

	MOV		R8 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
	MOV		R2, R24
	MOV		R3, R25

	MOV		R8 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
	MOV		R2, R24
	MOV		R3, R25

	MOV		R9 , R4
	MOV		R11, R5
	BSR		__xli_umuldq
	MOV		R2, R26
	MOV		R3, R27

	MOV		R9 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
	CLRT
	MOV		0 , R1
	ADDC	R2, R25
	ADDC	R3, R26
	ADDC	R1, R27

	MOV		R8 , R4
	MOV		R11, R5
	BSR		__xli_umuldq
	CLRT
	MOV		0 , R1
	ADDC	R2, R25
	ADDC	R3, R26
	ADDC	R1, R27

	MOV		R26, R2
	MOV		R27, R3

	MOV.X	(SP, 80), R26
	MOV.X	(SP, 64), R24
	MOV.Q	(SP, 56), R1
	MOV.Q	(SP, 48), R14
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 16), R10
	MOV.X	(SP,  0), R8
	ADD		96, SP
	JMP		R1
//	RTSU
#endif


#if 0
__xli_smul:
	ADD		-96, SP
	MOV		LR, R1
	MOV.X	R26, (SP, 80)
	MOV.X	R24, (SP, 64)
	MOV.Q	R1 , (SP, 56)
	MOV.Q	R14, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8 , (SP,  0)

	MOV		R4, R8
	MOV		R5, R9
	MOV		R6, R10
	MOV		R7, R11

	MOV		R8 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
//	MOV		R2, R24
//	MOV		R3, R25

	DMULU	R8, R11, R16
	DMULU	R9, R10, R17
	ADD		R16, R17, R18
	ADD		R18, R3

#if 0
	MOV		R8 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
	MOV		R2, R24
	MOV		R3, R25

	MOV		R9 , R4
	MOV		R11, R5
	BSR		__xli_umuldq
	MOV		R2, R26
	MOV		R3, R27

	MOV		R9 , R4
	MOV		R10, R5
	BSR		__xli_umuldq
	CLRT
	MOV		0 , R1
	ADDC	R2, R25
	ADDC	R3, R26
	ADDC	R1, R27

	MOV		R8 , R4
	MOV		R11, R5
	BSR		__xli_umuldq
	CLRT
	MOV		0 , R1
	ADDC	R2, R25
	ADDC	R3, R26
	ADDC	R1, R27

	MOV		R24, R2
	MOV		R25, R3
#endif

	MOV.X	(SP, 80), R26
	MOV.X	(SP, 64), R24
	MOV.Q	(SP, 56), R1
	MOV.Q	(SP, 48), R14
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 16), R10
	MOV.X	(SP,  0), R8
	ADD		96, SP
	JMP		R1
//	RTSU
#endif

// __xli_sdiv:


__xli_cmp_eq:
	CMPQEQ	R5, R7
	BF		.False
	CMPQEQ	R4, R6
	BF		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS

__xli_cmp_ne:
	CMPQEQ	R5, R7
	BF		.True
	CMPQEQ	R4, R6
	BF		.True
	MOV		0, R2
	RTS
	.True:
	MOV		1, R2
	RTS

__xli_cmp_gt:
	CMPQGT	R5, R7
	BT		.False
	CMPQGT	R6, R4
	BF		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS

__xli_cmp_ge:
	CMPQGT	R5, R7
	BT		.False
	CMPQGT	R4, R6
	BT		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS

__xli_cmp_hi:
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R6, R4
	BF		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
__xli_cmp_he:
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R4, R6
	BT		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS

__xli_cmp_tst:
	TSTQ	R5, R7
	BF		.True
	TSTQ	R4, R6
	BF		.True
	MOV		0, R2
	RTS
	.True:
	MOV		1, R2
	RTS

__xli_cmp_ntst:
	TSTQ	R5, R7
	BF		.False
	TSTQ	R4, R6
	BF		.False
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS

};


#if 1
static int _fcn_clz128(__uint128 v)
{
	const __uint128 m1 =0x80000000000000000000000000000000ULL;
	const __uint128 m8 =0xFF000000000000000000000000000000ULL;
	const __uint128 m16=0xFFFF0000000000000000000000000000ULL;
	const __uint128 m32=0xFFFFFFFF000000000000000000000000ULL;
	const __uint128 m64=0xFFFFFFFFFFFFFFFF0000000000000000ULL;
	__uint128 c;
	int n;

	c=v; n=0;
	if(!(c&m64))
		{ n+=64; c<<=64; }
	if(!(c&m32))
		{ n+=32; c<<=32; }
	if(!(c&m16))
		{ n+=16; c<<=16; }
	while(!(c&m8))
		{ n+=8; c<<=8; }
	while(!(c&m1))
		{ n++; c+=c; }
	return(n);
}
#endif

#if 1
__uint128 __xli_udiv(__uint128 n, __uint128 d)
{
	__uint128 q, r;
	int s, c;
//	int sr;
	byte sr;

//	if(!d || !n)
	if((d==0) || (n==0))
		return(0);
	sr=(byte)(_fcn_clz128(d)-_fcn_clz128(n));

	if(sr>=127)
	{
		if(sr==127)
			return(n);
		return(0);
	}

	sr++;
	q=n<<(128-sr); r=n>>sr; c=0;
	while(sr--)
	{
		r=(r<<1)|(q>>127);
		q=(q<<1)|c;
		s=((__int128)(d-r-1))>>127;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	return(q);
}
#endif

#if 1
__int128 __xli_sdiv(__int128 a, __int128 b)
{
	__int128	sga, sgb;
	sga=a>>127;		sgb=b>>127;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__xli_udiv(a, b)^sga)-sga);
}
#endif

#if 1
__int128 __xli_smod(__int128 a, __int128 b)
{
	__int128	c, d;
	
	c=__xli_sdiv(a, b);
	d=a-(c*b);
	return(d);
	
//	sga=a>>127;		sgb=b>>127;
//	a=(a^sga)-sga;	b=(b^sgb)-sgb;
//	sga^=sgb;
//	return((__xli_udiv(a, b)^sga)-sga);
}
#endif

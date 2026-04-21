#ifdef __BGBCC__

#ifdef __BJX2__

__int128 __xli_add (__int128 a, __int128 b);
__int128 __xli_sub (__int128 a, __int128 b);
__int128 __xli_smul(__int128 a, __int128 b);
__int128 __xli_umul(__int128 a, __int128 b);
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

};

__asm {

__xli_sub:
.ifarch has_alux
	SUBX	R4, R6, R2
	RTS
.else
//	MOV		R6, R2
//	MOV		R7, R3
//	CLRT
//	SUBC	R4, R2
//	SUBC	R5, R3

	MOV		R4, R2
	MOV		R5, R3
	CLRT
	SUBC	R6, R2
	SUBC	R7, R3
	RTS
.endif

};

__asm {
__xli_and:
	AND		R4, R6, R2
	AND		R5, R7, R3
	RTS
};

__asm {
__xli_or:
	OR		R4, R6, R2
	OR		R5, R7, R3
	RTS
};

__asm {
__xli_xor:
	XOR		R4, R6, R2
	XOR		R5, R7, R3
	RTS
};

__asm {
__xli_neg:
	CLRT
	MOV		0, R2
	MOV		0, R3
	SUBC	R4, R2
	SUBC	R5, R3
	RTSU
};

__asm {
__xli_not:
	NOT		R4, R2
	NOT		R5, R3
	RTS
};

__asm {
__xli_shl:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BF			.L1
	ADD			R6, -64, R7
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
};

__asm {
__xli_shlr:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BF			.L1
	ADD			R6, -64, R7
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
};

__asm {
__xli_shar:
	CMPGE		64, R6
	BT			.L0
	CMPGT		0, R6
	BF			.L1
	ADD			R6, -64, R7
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
};

__asm {
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

	CLRT
	ADDC	R22, R20
	ADDC	R23, R21

	MOV		R16, R2
	MOV		R19, R3
	CLRT
	ADDC	R20, R2
	ADDC	R21, R3
//	CLRT
//	ADDC	R22, R2
//	ADDC	R23, R3
	
//	BREAK
	
	RTSU
};

__asm {
#if 1
.extern	__xli_umuldq
__xli_umul:
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

	MOV		R2, R12
	MOV		R3, R13

	MOV		R8, R4
	MOV		R11, R5
	BSR		__xli_umuldq
	MOV		R2, R14

	MOV		R9, R4
	MOV		R10, R5
	BSR		__xli_umuldq
	ADD		R2, R14
	
	MOV		R12, R2
	ADD		R13, R14, R3

//	DMULU	R8, R11, R16
//	DMULU	R9, R10, R17
//	ADD		R16, R17, R18
//	ADD		R18, R3

//	BREAK

	MOV.Q	(SP, 56), R1
	MOV.Q	(SP, 48), R14
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 16), R10
	MOV.X	(SP,  0), R8
	ADD		96, SP
	JMP		R1
#endif
};

__asm {
#if 1
.extern	__xli_umuldq
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
};

__asm {
#if 0
.extern	__xli_umuldq
__xli_umul:
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

};

__asm {
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
};

__asm {
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
};

__asm {
__xli_cmp_gt:
	CMPQGT	R7, R5
	BT		.True
	CMPQGT	R5, R7
	BT		.False
	CMPQHI	R6, R4
	BF		.False
	.True:
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
};

__asm {
__xli_cmp_ge:
	CMPQGT	R7, R5
	BT		.True
	CMPQGT	R5, R7
	BT		.False
	CMPQHI	R4, R6
	BT		.False
	.True:
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
};

__asm {
__xli_cmp_hi:
	CMPQHI	R7, R5
	BT		.True
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R6, R4
	BF		.False
	.True:
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
};

__asm {
__xli_cmp_he:
	CMPQHI	R7, R5
	BT		.True
	CMPQHI	R5, R7
	BT		.False
	CMPQHI	R4, R6
	BT		.False
	.True:
	MOV		1, R2
	RTS
	.False:
	MOV		0, R2
	RTS
};

__asm {
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
};

__asm {
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

#endif



#ifdef __RISCV__

__int128 __xli_add (__int128 a, __int128 b);
__int128 __xli_sub (__int128 a, __int128 b);
__int128 __xli_smul(__int128 a, __int128 b);
__int128 __xli_umul(__int128 a, __int128 b);
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

.style att

__xli_add:
	ADD		R10, R12, R14
	ADD		R11, R13, R15
	SLTU	R14, R10, R16
	ADD		R15, R16, R15
	MOV		R14, R10
	MOV		R15, R11
	RTS

__xli_sub:
	NOT		R12, R16
	NOT		R13, R17

	ADD		R10, R16, R14
	ADD		R11, R17, R15
	SLTU	R14, R10, R16
	ADD		R15, R16, R15

	ADD		R14, 1, R10
	ADD		R15, 0, R11
	SLTU	R10, R14, R16
	ADD		R11, R16, R11

//	MOV		R14, R10
//	MOV		R15, R11
	RTS

#if 0
	NOT		R12, R14
	NOT		R13, R15
	ADD		R14, 1, R12
	ADD		R15, 0, R13
	SLTU	R12, R14, R16
	ADD		R13, R16, R13
	BRA		__xli_add
#endif

__xli_neg:
	NOT		R10, R12
	NOT		R11, R13
	ADD		R12, 1, R10
	ADD		R13, 0, R11
	SLTU	R10, R12, R16
	ADD		R11, R16, R11
	RTS

__xli_not:
	NOT		R10, R10
	NOT		R11, R11
	RTS

__xli_lnot:
	MOV		0, R12
	MOV		0, R13
	BRA		__xli_cmp_eq

__xli_and:
	AND		R10, R12, R10
	AND		R11, R13, R11
	RTS

__xli_or:
	OR		R10, R12, R10
	OR		R11, R13, R11
	RTS

__xli_xor:
	XOR		R10, R12, R10
	XOR		R11, R13, R11
	RTS

__xli_shl:
	BRGE	R12, R0, .L0
	AND		R12, 127, R12
	AND		R12, 63, R13
	MOV		64, R6
	SUB		R6, R13, R7

	BRLE	R12, R6, .L2

	SHLD.Q	R10, R13, R14
	SHLD.Q	R11, R13, R15
	SHLR.Q	R10, R7, R16
	OR		R15, R16, R15

	MOV		R14, R10
	MOV		R15, R11
	RTS

	.L0:
	BRGT	R12, R0, .L1
	RTS
	.L1:
	NEG		R12, R12
	BRA		__xli_shlr

	.L2:
	SHLD.Q	R10, R13, R11
	MOV		0, R10
	RTS
	

__xli_shlr:
	BRGE	R12, R0, .L0
	AND		R12, 127, R12
	AND		R12, 63, R13
	MOV		64, R6
	SUB		R6, R13, R7

	BRLE	R12, R6, .L2

	SHLR.Q	R10, R12, R14
	SHLR.Q	R11, R12, R15
	SHLD.Q	R11, R7, R16
	OR		R14, R16, R14
	MOV		R14, R10
	MOV		R15, R11
	RTS

	.L0:
	BRGT	R12, R0, .L1
	RTS
	.L1:
	NEG		R12, R12
	BRA		__xli_shl

	.L2:
	SHLR.Q	R11, R13, R10
	MOV		0, R11
	RTS


__xli_shar:
	BRGE	R12, R0, .L0

	AND		R12, 127, R12
	AND		R12, 63, R13
	MOV		64, R6
	SUB		R6, R13, R7

	BRLE	R12, R6, .L2

	SHLR.Q	R10, R12, R14
	SHAR.Q	R11, R12, R15
	SHLD.Q	R11, R7, R16
	OR		R14, R16, R14

	MOV		R14, R10
	MOV		R15, R11
	RTS

	.L0:
	BRGT	R12, R0, .L1
	RTS
	.L1:
	NEG		R12, R12
	BRA		__xli_shl

	.L2:
	SHAR.Q	R11, R13, R10
	SHAR.Q	R10, 63, R11
	RTS


__xli_smul:
__xli_umul:
	mulu.q	r10, r12, r28
	mulhu.q	r10, r12, r29
	mulu.q	r10, r13, r30
	mulu.q	r11, r12, r31

	add		r30, r31, r14
	mov		r28, r10
	add		r14, r29, r11

	rts
	nop

__xli_dmul128u:
	mulu.q		r10, r12, r28
	mulhu.q		r10, r12, r29
	mulu.q		r10, r13, r30
	mulu.q		r11, r12, r31

	add			r30, r31, r16
	add			r16, r29, r17

	mov.q		r28, (r14, 0)
	mov.q		r17, (r14, 8)

	mulu.q		r11, r13, r28
	mulhu.q		r11, r13, r29
	mulhu.q		r10, r13, r30
	mulhu.q		r11, r12, r31
	
	add			r30, r31, r16
	add			r16, r28, r17
	sltu		r16, r30, r6
	sltu		r17, r16, r7
	add			r29, r6, r29
	add			r29, r7, r29

	mov.q		r17, (r15, 0)
	mov.q		r29, (r15, 8)

	rts
	nop

#if 0
__xli_sdiv:
	rts
	nop

__xli_smod:
	rts
	nop
#endif

__xli_cmp_rt_T:
	mov		1, r10
	mov		0, r11
	rts
	nop
__xli_cmp_rt_F:
	mov		0, r10
	mov		0, r11
	rts
	nop

.global __xli_cmp_eq
__xli_cmp_eq:
	brne	r11, r13, __xli_cmp_rt_F
	brne	r10, r12, __xli_cmp_rt_F
	mov		1, r10
	mov		0, r11
	rts
	nop

.global __xli_cmp_ne
__xli_cmp_ne:
	brne	r11, r13, __xli_cmp_rt_T
	brne	r10, r12, __xli_cmp_rt_T
	mov		0, r10
	mov		0, r11
	rts
	nop

.global __xli_cmp_gt
__xli_cmp_gt:
	brlt	r11, r13, __xli_cmp_rt_T
	brgt	r11, r13, __xli_cmp_rt_F
	brlt	r10, r12, __xli_cmp_rt_T
	mov		0, r10
//	mov		0, r11
	rts
	nop
.global __xli_cmp_ge
__xli_cmp_ge:
	brlt	r11, r13, __xli_cmp_rt_T
	brgt	r11, r13, __xli_cmp_rt_F
	brle	r10, r12, __xli_cmp_rt_T
	mov		0, r10
//	mov		0, r11
	rts
	nop


.global __xli_cmp_hi
__xli_cmp_hi:
	brltu	r11, r13, __xli_cmp_rt_T
	brgtu	r11, r13, __xli_cmp_rt_F
	brltu	r10, r12, __xli_cmp_rt_T
	mov		0, r10
	rts
	nop
.global __xli_cmp_he
__xli_cmp_he:
	brltu	r11, r13, __xli_cmp_rt_T
	brleu	r10, r12, __xli_cmp_rt_T
	mov		0, r10
	rts
	nop

__xli_cmp_tst:
	and		r10, r12, r14
	and		r11, r13, r15
	brne	r15, r0,__xli_cmp_rt_T
	brne	r14, r0, __xli_cmp_rt_T
	mov		0, r10
	rts
	nop

__xli_cmp_ntst:
	and		r10, r12, r14
	and		r11, r13, r15
	brne	r15, r0,__xli_cmp_rt_F
	brne	r14, r0, __xli_cmp_rt_F
	mov		1, r10
	rts
	nop
};
#endif

#if 1
static int _fcn_clz128(__uint128 v)
{
	const __uint128 m1 =0x80000000000000000000000000000000UI128;
	const __uint128 m8 =0xFF000000000000000000000000000000UI128;
	const __uint128 m16=0xFFFF0000000000000000000000000000UI128;
	const __uint128 m32=0xFFFFFFFF000000000000000000000000UI128;
	const __uint128 m64=0xFFFFFFFFFFFFFFFF0000000000000000UI128;
	__uint128 c;
	int n;

	if(v==0)
		return(128);

	c=v; n=0;
	if(!(c&m64))
		{ n+=64; c<<=64; }
	if(!(c&m32))
		{ n+=32; c<<=32; }
	if(!(c&m16))
		{ n+=16; c<<=16; }
//	__debugbreak();
	while(!(c&m8))
		{ n+=8; c<<=8; }
//	__debugbreak();
	while(!(c&m1))
		{ n++; c+=c; }
//	__debugbreak();
	return(n);
}
#endif


u64 __udivdi3(u64 n, u64 d);

#if 1
__uint128 __xli_udiv(__uint128 n, __uint128 d)
{
	__uint128 q, r;
//	int s, c;
	__int128 s, c;
//	int sr;
	byte sr;

//	if(!d || !n)
	if((d==0) || (n==0))
		return(0);
	
	if(d>n)
		return(0);

	if(((u64)n==n) && ((u64)d==d))
	{
		q=__udivdi3(n, d);
		return(q);
	}

#if 1
	if(!(d&(d-1)))
	{
		q=n;
		r=d;
		while(!(r&1))
		{
			q=q>>1;
			r=r>>1;
		}
		return(q);
	}
#endif

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
__int128 __xli_umod(__uint128 a, __uint128 b)
{
	__uint128	c, d;
	
#if 1
	if(!(b&(b-1)))
	{
		d=a&(b-1);
		return(d);
	}
#endif

	c=__xli_udiv(a, b);
	d=a-(c*b);
	
	if(d>=b)
		__debugbreak();
	
	return(d);
}
#endif

#if 1
__int128 __xli_sdiv(__int128 a, __int128 b)
{
	__uint128	av, bv, cv;
	int			sg;
	
	sg=0;
	av=a;
	bv=b;
	
	if(a<0)
		{ av=-a; sg=!sg; }
	if(b<0)
		{ bv=-b; sg=!sg; }
	cv=__xli_udiv(av, bv);
	if(sg)
		cv=-cv;
	return(cv);
	
//	__int128	sga, sgb;
//	sga=a>>127;		sgb=b>>127;
//	a=(a^sga)-sga;	b=(b^sgb)-sgb;
//	sga^=sgb;
//	return((__xli_udiv(a, b)^sga)-sga);
}
#endif

#if 1
__int128 __xli_smod(__int128 a, __int128 b)
{
	__int128	c, d;
	
	c=__xli_sdiv(a, b);
	d=a-(c*b);

//	__debugbreak();

	return(d);
}
#endif

#endif

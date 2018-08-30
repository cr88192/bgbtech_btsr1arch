/*
Count Leading Zeroes
  R4=Input
  R2=Output
Stomps: R2, R3, R4, R5
 */
_fcn_clz32:
	mov #0, r2

	shld r4, #16, r3
	add r2, 16, r5
	tst #0xFFFF0000, r4
	cselt r3, r4, r4
	cselt r5, r2, r2

	shld r4, #8, r3
	add r2, 8, r5
	tst #0xFF000000, r4
	cselt r3, r4, r4
	cselt r5, r2, r2

	shld r4, #4, r3
	add r2, 4, r5
	tst #0xF0000000, r4
	cselt r3, r4, r4
	cselt r5, r2, r2

	shld r4, #2, r3
	add r2, 2, r5
	tst #0xC0000000, r4
	cselt r3, r4, r4
	cselt r5, r2, r2

	shld r4, #1, r3
	add r2, 1, r5
	tst #0x80000000, r4
	cselt r3, r4, r4
	cselt r5, r2, r2

	rts

#if 1
// u32 __udivsi3(u32 n, u32 d)
.global	__udivsi3
__udivsi3:
	push	lr
	push	r14
	push	r13
	push	r12

	tst		r4, r4
	bt		.Ret_Zero
	tst		r5, r5
	bt		.Ret_Zero

	extu.l	r4, r12
	extu.l	r5, r13
	
	/*
		R12=n
		R13=d
		R14=sr
	 */

//	mov		r13, r4
//	bsr		_fcn_clz32
//	mov		r2, r14

	clz		r13, r14

//	mov		r12, r4
//	bsr		_fcn_clz32

	clz		r12, r2

	sub		r2, r14
	extu.b	r14

	cmpgt #30, r14
	bf .L0
	cmpeq #31, r14
	bt .Ret_R12
	bra .Ret_Zero
	.L0:

	add #1, r14

	mov #32, r2
	sub r2, r14, r2
	shld r12, r2, r4

	neg r14, r2
	shld r12, r2, r5

	mov #0, r6
	
	/*
		R2=scratch
		R3=scratch
		R4=q
		R5=r
		R6=c
		R7=s

		R12=n
		R13=d
		R14=sr
	 */
	
	.L1:
	add		#-1, r14

	shll1	r5
	shld	r4, #-31, r2
	shll1	r4
	or		r2, r5
	or		r6, r4

	sub		r13, r5, r2
	add		#-1, r2
	shad	r2, #-31, r7
	and		r7, #1, r6

	and		r13, r7, r2
	sub		r2, r5

	tst		r14, r14
	bf		.L1

	.L2:

	shll1	r4
	or		r6, r4

	extu.l	r4, r2

	pop r12
	pop r13
	pop r14
	ret

.Ret_Zero:
	mov #0, r2
	pop r12
	pop r13
	pop r14
	ret

.Ret_R12:
	mov r12, r2
	pop r12
	pop r13
	pop r14
	ret
#endif

#if 1
// s32 __sdivsi3(s32 a, s32 b)
.global	__sdivsi3

__sdivsi3:
	push lr
	push r14
	shad r4, #-31, r6
	shad r5, #-31, r7
	xor r4, r6, r2
	xor r5, r7, r3
	sub r2, r6, r4
	sub r3, r7, r5
	xor r7, r6, r14
	bsr __udivsi3
	xor r2, r14, r3
	sub r3, r14, r2
	pop r14
	ret

#endif

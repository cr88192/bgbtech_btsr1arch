#if 1
/*
Count Leading Zeroes
  R4=Input
  R2=Output
Stomps: R2, R3, R4, R5
 */
_fcn_clz32:
	mov #0, r2				| mov #-65536, r0
	shld r4, #16, r3		| add r2, 16, r5
	tst r0, r4
//	tst #0xFFFF0000, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld r4, #8, r3			| add r2, 8, r5
	tst #0xFF000000, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld r4, #4, r3			| add r2, 4, r5
	tst #0xF0000000, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld r4, #2, r3			| add r2, 2, r5
	tst #0xC0000000, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld r4, #1, r3			| add r2, 1, r5
	tst #0x80000000, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	rts
#endif

#if 1
/*
Unsigned Divide, Preserve R21 and R22
 */
// u32 __udivsi3(u32 n, u32 d)
.global	__udivsi3
__udivsi3:
//	push	lr
	mov		lr, r23

	tst		r4, r4
	bt		.Ret_Zero
	tst		r5, r5
	bt		.Ret_Zero

	extu.l	r4, r16		| extu.l	r5, r17
	
	/*
		R16=n
		R17=d
		R18=sr
		
		R21=__sdivsi3 . sign_mask
		R22=__sdivsi3 . lr
		R23=lr
	 */


//	clz		r17, r18
	mov		r17, r4
	bsr		_fcn_clz32
	mov		r2, r18

//	clz		r16, r2
	mov		r16, r4
	bsr		_fcn_clz32

	sub		r2, r18
	extu.b	r18

	cmpgt #30, r18
	bf .L0
	cmpeq #31, r18
	bt .Ret_R16
	bra .Ret_Zero
	.L0:

	add #1, r18			| mov #32, r2
	neg r18, r3			| sub r18, r2
	shld r16, r2, r4	| shld r16, r3, r5
	mov #0, r6
	
	/*
		R2=scratch
		R3=scratch
		R4=q
		R5=r
		R6=c
		R7=s

		R16=n
		R17=d
		R18=sr

		R21=__sdivsi3 . sign_mask
		R22=__sdivsi3 . lr
		R23=lr
	 */

#if 0
	tst		#-4, r18
	bt		.L1
	.L4:
	shll1	r5			| shld		r4, #-31, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpgt		r5, r17
	movnt	r6			| sub		r5, r17, r7
	add		#-3, r18	| cselt	r5, r7, r5
	shll1	r5			| shld		r4, #-31, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpgt		r5, r17
	movnt	r6			| sub		r5, r17, r7
	cselt	r5, r7, r5
	shll1	r5			| shld		r4, #-31, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpgt		r5, r17
	movnt	r6			| sub		r5, r17, r7
	cselt	r5, r7, r5	| tst		#-4, r18
	bf		.L4
#endif

	.L1:
	add		#-1, r18		| shll1		r5
	shld	r4, #-31, r2	| shll1		r4
	or		r2, r5			| or		r6, r4
	cmpgt	r5, r17
	movnt	r6				| sub		r5, r17, r7
	cselt	r5, r7, r5		| tst		r18, r18
	bf		.L1

	.L2:
	shll1	r4
	or		r6, r4		| mov		r23, lr
	extu.l	r4, r2		| rts

.Ret_Zero:
	mov		#0, r2		| mov		r23, lr
	rts

.Ret_R16:
	mov		r16, r2		| mov		r23, lr
	rts
#endif

#if 1
// s32 __sdivsi3(s32 a, s32 b)
.global	__sdivsi3

__sdivsi3:
//	push lr
	mov 	lr, r22
//	push r14
	shad r4, #-31, r6	| shad r5, #-31, r7
	xor r4, r6, r2		| xor r5, r7, r3
	sub r2, r6, r4		| sub r3, r7, r5
	xor r7, r6, r21
	bsr __udivsi3
	xor r2, r21, r3
	sub r3, r21, r2

//	pop r14
	mov		r22, lr
	rts

//	ret

#endif

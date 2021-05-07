#if 1
/*
Count Leading Zeroes
  R4=Input
  R2=Output
Stomps: R2, R3, R4, R5
 */
_fcn_clz64:
	mov #0, r2

	shld.q r4, #32, r3		| add r2, 32, r5
	mov #0xFFFFFFFF00000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld.q r4, #16, r3		| add r2, 16, r5
	mov #0xFFFF000000000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld.q r4, #8, r3		| add r2, 8, r5
	mov #0xFF00000000000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld.q r4, #4, r3		| add r2, 4, r5
	mov #0xF000000000000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld.q r4, #2, r3		| add r2, 2, r5
	mov #0xC000000000000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	shld.q r4, #1, r3		| add r2, 1, r5
	mov #0x8000000000000000, r0
	tstq r0, r4
	cselt r3, r4, r4		| cselt r5, r2, r2
	rts
#endif


#if 1
// u64 __udivdi3(u64 n, u64 d)

.global __udivdi3
__udivdi3:

//	u64 q, r;
//	int s, c;
//	int sr;
//	byte sr;

	mov		lr, r23

//	if(!d || !n)
//		return(0);

	tstq	r4, r4
	bt		.Ret_Zero
	tstq	r5, r5
	bt		.Ret_Zero

	mov	r4, r16		| mov	r5, r17
	
	/*
		R16=n
		R17=d
		R18=sr
		
		R21=__sdivdi3 . sign_mask
		R22=__sdivdi3 . lr
		R23=lr
	 */

//	sr=_fcn_clz(d)-_fcn_clz(n);
//	sr=(byte)(_fcn_clz64(d)-_fcn_clz64(n));

	clzq	r17, r18
//	mov		r17, r4
//	bsr		_fcn_clz64
//	mov		r2, r18

	clzq	r16, r2
//	mov		r16, r4
//	bsr		_fcn_clz64

	sub		r2, r18
	extu.b	r18


//	if(sr>=63)
//	{
//		if(sr==63)
//			return(n);
//		return(0);
//	}

	cmpgt #62, r18
	bf .L0
	cmpeq #63, r18
	bt .Ret_R16
	bra .Ret_Zero
	.L0:


//	sr++;
//	q=n<<(64-sr); r=n>>sr; c=0;

	add #1, r18			| mov #64, r2
	neg r18, r3			| sub r18, r2
	shld.q r16, r2, r4	| shld.q r16, r3, r5
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

		R21=__sdivdi3 . sign_mask
		R22=__sdivdi3 . lr
		R23=lr
	 */

//	while(sr--)
//	{
//		r=(r<<1)|(q>>63);
//		q=(q<<1)|c;
//		s=((s64)(d-r-1))>>63;
//		c=s&1;
//		r-=d&s;
//	}
//	q=(q<<1)|c;
//	return(q);

#if 1
	tst		#-4, r18
	bt		.L1
	.L4:
	shll1	r5			| shld.q	r4, #-63, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpqgt	r5, r17
	movnt	r6			| sub		r5, r17, r7
	add		#-3, r18	| cselt		r5, r7, r5
	shll1	r5			| shld.q	r4, #-63, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpqgt	r5, r17
	movnt	r6			| sub		r5, r17, r7
	cselt	r5, r7, r5
	shll1	r5			| shld.q	r4, #-63, r2
	shll1	r4			| or		r2, r5
	or		r6, r4		| cmpqgt	r5, r17
	movnt	r6			| sub		r5, r17, r7
	cselt	r5, r7, r5	| tst		#-4, r18
	bf		.L4
#endif

	.L1:
	add		#-1, r18		| shll1		r5
	shld.q	r4, #-63, r2
	shll1	r4
	or		r2, r5			| or		r6, r4
	cmpqgt	r5, r17
	movnt	r6				| sub		r5, r17, r7
	cselt	r5, r7, r5		| tst		r18, r18
	bf		.L1

	.L2:
	shll1	r4
	or		r6, r4
//	mov		r23, lr
	mov		r4, r2
//	rts
//	or		r6, r4, r2
	jmp		r23

.Ret_Zero:
	mov		#0, r2
	jmp		r23
//	mov		r23, lr
//	rts

.Ret_R16:
	mov		r16, r2
	jmp		r23
//	mov		r23, lr
//	rts

#endif


#if 1
// s64 __sdivdi3(s64 a, s64 b)
.global	__sdivdi3

__sdivdi3:
	mov 	lr, r22
	shad.q	r4, #-63, r6	|	shad.q r5, #-63, r7
	xor		r4, r6, r2		|	xor r5, r7, r3
	sub		r2, r6, r4		|	sub r3, r7, r5
	xor		r7, r6, r21
	bsr		__udivdi3
	xor		r2, r21, r3
	sub		r3, r21, r2
	jmp		r22
//	mov		r22, lr
//	rts
#endif

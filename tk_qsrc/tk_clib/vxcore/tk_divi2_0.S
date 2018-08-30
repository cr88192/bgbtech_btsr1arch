//static int _fcn_clz32(u32 v)
//{
//	const u32 m1=0x80000000U;
//	const u32 m8=0xFF000000U;
//	const u32 m16=0xFFFF0000U;
//	u32 c;
//	int n;
//	c=v; n=0;
//	if(!(c&m16))
//		{ n+=16; c<<=16; }
//	while(!(c&m8))
//		{ n+=8; c<<=8; }
//	while(!(c&m1))
//		{ n++; c+=c; }
//	return(n);
//}

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

// u32 __udivsi3(u32 n, u32 d)
// {
//    u32 q, r;
//    int s, c;
//    byte sr;
//    if(!d || !n)
//		return(0);
//    sr=(byte)(_fcn_clz32(d)-_fcn_clz32(n));
//    if(sr>=31)
//    {
//		if(sr==31)
//			return(n);
//		return(0);
//	}
//	sr++;
//	q=n<<(32-sr); r=n>>sr; c=0;
//	while(sr--)
//	{
//		r=(r<<1)|(q>>31);
//		q=(q<<1)|c;
//		s=((int)(d-r-1))>>31;
//		c=s&1;
//		r-=d&s;
//	}
//	q=(q<<1)|c;
//	q=(u32)q;
//	return(q);
// }

#if 1

.global	__udivsi3
__udivsi3:
	push	lr
	push	r14
	push	r13
	push	r12

//	if(!d || !n)
//		return(0);

	tst		r4, r4
	bt		.Ret_Zero
	tst		r5, r5
	bt		.Ret_Zero

//	mov		r4, r12
//	mov		r5, r13
	extu.l	r4, r12
	extu.l	r5, r13
	
	/*
		R12=n
		R13=d
		R14=sr
	 */

//	sr=(byte)(_fcn_clz32(d)-_fcn_clz32(n));
	mov		r13, r4
	bsr		_fcn_clz32
	mov		r2, r14

	mov		r12, r4
	bsr		_fcn_clz32
//	sub		r14, r2, r14
	sub		r2, r14
	extu.b	r14

//	if(sr>=31)
//	{
//		if(sr==31)
//			return(n);
//		return(0);
//	}

	cmpgt #30, r14
	bf .L0
	cmpeq #31, r14
	bt .Ret_R12
	bra .Ret_Zero
	.L0:

//	sr++;
	add #1, r14

//	q=n<<(32-sr);
	mov #32, r2
	sub r2, r14, r2
	shld r12, r2, r4

//	r=n>>sr;
	neg r14, r2
	shld r12, r2, r5

//	c=0;
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
	
//	while(sr--)
//	{

	.L1:
	tst		r14, r14
	bt		.L2
	add		#-1, r14

//		r=(r<<1)|(q>>31);
	shll1	r5
	shld	r4, #-31, r2
	or		r5, r2, r5
//		q=(q<<1)|c;
	shll1	r4
	or		r6, r4

//		s=((int)(d-r-1))>>31;
	sub		r13, r5, r2
	add		#-1, r2
	shad	r2, #-31, r7

//		c=s&1;
	and		r7, #1, r6

//		r-=d&s;
	and		r13, r7, r2
	sub		r2, r5

//	}
	bra .L1
	.L2:

//	q=(q<<1)|c;
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

// s32 __sdivsi3(s32 a, s32 b)
// {
//	s32 sga, sgb;
//	sga=a>>31;		sgb=b>>31;
//	a=(a^sga)-sga;	b=(b^sgb)-sgb;
//	sga^=sgb;
//	return((__udivsi3(a, b)^sga)-sga);
// }
#endif

#if 1
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

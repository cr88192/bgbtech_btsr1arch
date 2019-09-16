/*
Run basic ISA-level sanity checks.

This logic does not follow the C ABI, and may stomp most of the registers.
This is assumed to be called prior to transferring control to C land.
 */

#define X(n)	n

_checksane_fail:
	break
	bra _checksane_fail
	break

.global X(tk_checksane_asm)
X(tk_checksane_asm):
	push lr

/* Stage 1, Basic ALU */
	mov	0x12345678, r4
	mov	0x1234CDEF, r5
	mov	0x24692467, r8
	mov	0xFFFF8889, r9
	exts.l	r9, r10

	cmp/eq r4, r5
	bt _checksane_fail

	cmp/gt r4, r5
	bf _checksane_fail

	cmp/gt r4, r9
	bt _checksane_fail

	cmp/hi r4, r9
	bf _checksane_fail

	cmp/ge r4, r5
	bf _checksane_fail

	cmp/ge r4, r9
	bt _checksane_fail

	cmp/ge r4, r4
	bf _checksane_fail

	movt	r12
	cmp/eq #1, r12
	bf _checksane_fail

	movnt	r12
	cmp/eq #0, r12
	bf _checksane_fail

	mov 0xEDCBA987, r12

	tst r4, r8
	bt _checksane_fail

	tst r4, r12
	bf _checksane_fail


	add	r4, r5, r6
	mov r4, r7
	add r5, r7
	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r8
	bf _checksane_fail

	add	r4, r5, r18
	cmp/eq r18, r7
	bf _checksane_fail

	sub	r4, r5, r6
	mov r4, r7
	sub r5, r7
	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r9
	bf _checksane_fail
	cmpq/eq r6, r10
	bf _checksane_fail

	adds.l	r4, r5, r6
	addu.l	r4, r5, r7

	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r8
	bf _checksane_fail

	subs.l	r4, r5, r6
	subu.l	r4, r5, r7

	cmpq/eq r6, r10
	bf _checksane_fail
	cmpq/eq r9, r9
	bf _checksane_fail


	mov	0x12344468, r8
	mov	0x1234DFFF, r9
	mov	0x00009B97, r10

	and	r4, r5, r6
	mov r4, r7
	and r5, r7
	cmp/eq r6, r7
	bf _checksane_fail

	cmp/eq r6, r8
	bf _checksane_fail

	or	r4, r5, r6
	mov r4, r7
	or r5, r7
	cmp/eq r6, r7
	bf _checksane_fail

	cmp/eq r6, r9
	bf _checksane_fail


	xor	r4, r5, r6
	mov r4, r7
	xor r5, r7
	cmp/eq r6, r7
	bf _checksane_fail

	cmp/eq r6, r10
	bf _checksane_fail


	mov	0xEDCBA987, r11

	not r4, r6
	mov r4, r7
	not r7

	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r11
	bf _checksane_fail

	mov	0xEDCBA988, r12

	neg r4, r6
	mov r4, r7
	neg r7

	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r12
	bf _checksane_fail

	neg r4, r16
	mov r4, r17
	neg r17

	cmp/eq r16, r17
	bf _checksane_fail
	cmp/eq r16, r12
	bf _checksane_fail


	mov	0xE87AD208, r8
	mov	0x014B6F5A, r9

	mulu	r4, r5
	mov r0, r6
	mov r1, r7
	
	cmp/eq r6, r8
	bf _checksane_fail
	
	cmp/eq r7, r9
	bf _checksane_fail

	mov	0x75C123C0, r10
	mov	0x10882A3D, r11
	mov	0xFE53D3C5, r12

	mulu	r4, r8
	mov r0, r6
	mov r1, r7
	cmp/eq r6, r10
	bf _checksane_fail
	cmp/eq r7, r11
	bf _checksane_fail

	muls	r4, r8
	mov r0, r6
	mov r1, r7
	cmp/eq r6, r10
	bf _checksane_fail
	cmp/eq r7, r12
	bf _checksane_fail


	mov	0x1234CE58, r8
	mov	0x1234CD86, r9

	add r5, #0x69, r6
	mov r5, r7
	add #0x69, r7

	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r8
	bf _checksane_fail

	add r5, #0x69, r16
	cmp/eq r16, r7
	bf _checksane_fail

	add r5, #-0x69, r6
	mov r5, r7
	add #-0x69, r7

	cmp/eq r6, r7
	bf _checksane_fail
	cmp/eq r6, r9
	bf _checksane_fail

	mov #0x69, r6
	cmp/eq #0x69, r6
	bf _checksane_fail
	cmp/eq #0x73, r6
	bt _checksane_fail
	cmp/gt #0x73, r6
	bt _checksane_fail
	
	tst #0x31, r6
	bt _checksane_fail
	tst #0x96, r6
	bf _checksane_fail

	mov #0x6, r6
	cmp/eq #0x6, r6
	bf _checksane_fail
	cmp/eq #0x9, r6
	bt _checksane_fail

	cmp/gt #0x5, r6
	bf _checksane_fail
	cmp/gt #0x6, r6
	bt _checksane_fail
	cmp/gt #0x7, r6
	bt _checksane_fail

	cmp/ge #0x5, r6
	bf _checksane_fail
	cmp/ge #0x6, r6
	bf _checksane_fail
	cmp/ge #0x7, r6
	bt _checksane_fail

	mov #-0x6, r6
	cmp/eq #-0x6, r6
	bf _checksane_fail
	cmp/eq #-0x9, r6
	bt _checksane_fail


	mov	0x23456780, r8
	mov	0x01234567, r9

	shad  r4, #4, r6
	shad  r4, #-4, r7

	cmpq/eq r6, r8
	bf _checksane_fail
	cmpq/eq r7, r9
	bf _checksane_fail


	mov	0x68ACF000, r8
	mov	0x00091A2B, r9

	shad  r4, #9, r6
	shad  r4, #-9, r7

	cmpq/eq r6, r8
	bf _checksane_fail
	
	cmpq/eq r7, r9
	bf _checksane_fail

	mov #9, r22
	shad  r4, r22, r6
	cmpq/eq r6, r8
	bf _checksane_fail

	mov	0x2468ACF000, r10
	mov	0xE87AD208, r11
	mov	0x01D0F5A4, r12
	mov	0xFFD0F5A4, r13
	mov 0xC00000000, r14

	cmp/eq r8, r10
	bf _checksane_fail

	cmpq/eq r8, r10
	bt _checksane_fail

	cmp/gt r8, r11
	bt _checksane_fail

	cmp/hi r8, r11
	bf _checksane_fail

//	cmpq/eq #0xE87AD208, r11
//	bf _checksane_fail
	
	tst r14, r14
	bf _checksane_fail

	tstq r14, r14
	bt _checksane_fail

	shad.q  r4, #9, r6
	cmpq/eq r6, r10
	bf _checksane_fail

	shld  r11, #-7, r6
	cmp/eq r6, r12
	bf _checksane_fail

	shad  r11, #-7, r6
	cmp/eq r6, r13
	bf _checksane_fail

	mov #-7, r2
	mov #7, r3

	shad  r11, r2, r6
	shar  r11, r3, r7

	cmp/eq r6, r13
	bf _checksane_fail
	cmp/eq r7, r13
	bf _checksane_fail

	shad.q  r4, #31, r7

	mov r4, r6
	shll1 r6
	shll2 r6
	shll4 r6
	shll8 r6
	shll16 r6

	cmpq/eq r6, r4
	bt _checksane_fail
	cmpq/eq r6, r7
	bf _checksane_fail

	shlr1 r6
	shlr2 r6
	shlr4 r6
	shlr8 r6
	shlr16 r6

	cmpq/eq r6, r4
	bf _checksane_fail


	push r4
	push r5
	pop r7
	pop r6

	cmp/eq r4, r6
	bf _checksane_fail
	cmp/eq r5, r7
	bf _checksane_fail

	push r4
	pop lr
	mov lr, r6
	cmp/eq r4, r6
	bf _checksane_fail

	mov r5, lr
	mov lr, r6
	cmp/eq r5, r6
	bf _checksane_fail

	cmp/eq r5, r6
	cselt	r13, r5, r19
	cmp/eq r13, r19
	bf _checksane_fail

	cmp/eq r12, r19
	cselt	r5, r12, r19
	cmp/eq r12, r19


	bra	.L0
	.L2:
	break
	.L0:
//	bra .L1
	bra .L4
	bra .L2
	.L4:
	bsr .L1
	.L3:
	rts
	.L1:
	bsr .L3


	mov	0x12345678, r4
	mov	0x1234CDEF, r5
	mov	0x24692467, r8
	mov	0xFFFF8889, r9
	
	exts.l	r9, r10
	extu.l	r9, r11

	cmpq/eq r9, r10
	bt _checksane_fail
	cmpq/eq r9, r11
	bf _checksane_fail
	cmpq/eq r10, r11
	bt _checksane_fail

	exts.b	r5, r6
	extu.b	r5, r7

	cmpq/eq #-0x11, r6
	bf _checksane_fail
	cmpq/eq #0xEF, r7
	bf _checksane_fail

	exts.w	r5, r6
	extu.w	r5, r7

	cmpq/eq #-0x3211, r6
	bf _checksane_fail
	cmpq/eq #0xCDEF, r7
	bf _checksane_fail
	
	mov .L6, r6
	jmp r6
	break
	.L6:
	bra .L8
	break
	.L8:
	mov #2, r6
	braf r6
	break
	break
	bra .L7
	break
	break
	.L7:

#if 1
	mov	0xE87AD208, r8
	exts.l r8, r9
	mov	0xFFD0F5A4E87AD208, r10
	mov	0xFFFFFFFFFFD0F5A4, r11
	mov	0xE87AD208E87AD208, r14

	push r8
	push r8
	push r10	
	mov.l (sp, 0), r6
	movu.l (sp, 0), r7

	cmpq/eq r6, r7
	bt _checksane_fail
	cmpq/eq r6, r9
	bf _checksane_fail
	cmpq/eq r7, r8
	bf _checksane_fail
	cmpq/eq r8, r10
	bt _checksane_fail

	mov.q (sp, 0), r6
	cmpq/eq r6, r10
	bf _checksane_fail

	exts.b r8, r12
	exts.w r8, r13
	mov.b (sp, 0), r6
	mov.w (sp, 0), r7
	cmpq/eq r6, r12
	bf _checksane_fail
	cmpq/eq r7, r13
	bf _checksane_fail

	extu.b r8, r12
	extu.w r8, r13
	movu.b (sp, 0), r6
	movu.w (sp, 0), r7
	cmpq/eq r6, r12
	bf _checksane_fail
	cmpq/eq r7, r13
	bf _checksane_fail

	mov sp, r3
	mov.l (sp, 4), r6
	mov.l (r3, 4), r7

	cmpq/eq r6, r7
	bf _checksane_fail
	cmpq/eq r7, r11
	bf _checksane_fail

	mov.l r10, (sp, 12)
	mov.q (sp, 8), r6
	cmpq/eq r6, r14
	bf _checksane_fail
	

	pop r2
	pop r2
	pop r2
#endif

	cmpeq r4, r5
		break?t
	cmpeq r4, r4
		break?f

	mov #28, r18
	add #3, r18
	cmpeq #31, r18
	bf _checksane_fail

	neg r18, r19
	cmpeq #-31, r19
	bf _checksane_fail

#if 0
	mov	0x8889, r8
	mov	0xFFFF8889, r9
	mov	0x00FF8889, r10
	mov 0, r11

	clz r8, r6
	clz r9, r7

	cmp/eq #16, r6
	bf _checksane_fail
	cmp/eq #0, r7
	bf _checksane_fail

	clz r10, r6
	clz r11, r7

	cmp/eq #8, r6
	bf _checksane_fail
	cmp/eq #32, r7
	bf _checksane_fail
#endif

	pop lr
	rts


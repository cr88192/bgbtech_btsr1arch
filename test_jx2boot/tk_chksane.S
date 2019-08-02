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
	
	add	r4, r5, r6
	mov r4, r7
	add r5, r7
	cmp/eq r6, r7
	bf _checksane_fail

	cmp/eq r6, r8
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


	mov	0xE87AD208, r8
	mov	0x014B6F5A, r9

	dmulu	r4, r5
	mov r0, r6
	mov r1, r7
	
	cmp/eq r6, r8
	bf _checksane_fail
	
	cmp/eq r7, r9
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


	mov	0x68ACF000, r8
	mov	0x00091A2B, r9

	shad  r4, #9, r6
	shad  r4, #-9, r7

	cmpq/eq r6, r8
	bf _checksane_fail
	
	cmpq/eq r7, r9
	bf _checksane_fail

	mov	0x2468ACF000, r10
	mov	0xE87AD208, r11
	mov	0x01D0F5A4, r12
	mov	0xFFD0F5A4, r13

	cmp/eq r8, r10
	bf _checksane_fail

	cmpq/eq r8, r10
	bt _checksane_fail

	cmp/gt r8, r11
	bt _checksane_fail

	cmp/hi r8, r11
	bf _checksane_fail


	shad.q  r4, #9, r6
	cmpq/eq r6, r10
	bf _checksane_fail

	shld  r11, #-7, r6
	cmp/eq r6, r12
	bf _checksane_fail

	shad  r11, #-7, r6
	cmp/eq r6, r13
	bf _checksane_fail

	push r4
	push r5
	pop r7
	pop r6

	cmp/eq r4, r6
	bf _checksane_fail
	cmp/eq r5, r7
	bf _checksane_fail

	bra	.L0
	.L2:
	break
	.L0:
	bra .L1
	bra .L2
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
	

	pop lr
	rts


_checksane_fail_mem:
	break
	break
	bra _checksane_fail_mem
	break

_checksane_ldrts_asm:
	mov		lr, r1
	mov		.val0, r3
	movu.l	(r3), r2
//	nop
//	nop
//	rts
	jmp		r1
	
//	break
//	break
//	break
//	nop
//	nop
//	rts

.val0: 	.long	0xE87AD268

.global X(tk_checksane_mem_asm)
X(tk_checksane_mem_asm):
//	push lr

	add		-8, sp
	mov		lr, r1
	mov.q	r1, (sp)

#if 1
	mov	0xE87AD208, r8
	exts.l r8, r9
	mov	0xFFD0F5A4E87AD208, r10
	mov	0xFFFFFFFFFFD0F5A4, r11
	mov	0xE87AD208E87AD208, r14

	add		-128, sp

	mov.q	r14,  (sp, 48)
	mov.q	r11,  (sp, 40)
	mov.q	r10,  (sp, 32)

	mov.q	r8,  (sp, 16)
	mov.q	r8,  (sp,  8)
	mov.q	r10, (sp,  0)

#ifndef JX2UC

	.L0:

	mov.x	(sp, 32), r16
	cmpqeq	r16, r10
	break?f
	cmpqeq	r17, r11
	break?f

	.L1:

	mov.x	(sp, 40), r18
	cmpqeq	r18, r11
	break?f
	cmpqeq	r19, r14
	break?f

	mov.x	r16,  (sp, 56)
	mov.x	r18,  (sp, 72)
	mov.x	(sp, 64), r20

	.L2:

	cmpqeq	r20, r11
	break?f
	cmpqeq	r21, r11
	break?f

	mov.x	(sp, 64), r6

	.L3:

	cmpqeq	r6, r11
	break?f
	cmpqeq	r7, r11
	break?f

#endif


//	push r8
//	push r8
//	push r10	
	mov.l (sp, 0), r6
	movu.l (sp, 0), r7

	.L4:

	cmpq/eq r6, r7
	bt _checksane_fail_mem
	cmpq/eq r6, r9
	bf _checksane_fail_mem
	cmpq/eq r7, r8
	bf _checksane_fail_mem
	cmpq/eq r8, r10
	bt _checksane_fail_mem

	mov.q (sp, 0), r6
	cmpq/eq r6, r10
	bf _checksane_fail_mem

	movd.l (sp, 0), r6
	cmp/eq r6, r8
	bf _checksane_fail_mem

	.L5:

	exts.b r8, r12
	exts.w r8, r13
	mov.b (sp, 0), r6
	mov.w (sp, 0), r7
	cmpq/eq r6, r12
	bf _checksane_fail_mem
	cmpq/eq r7, r13
	bf _checksane_fail_mem

	extu.b r8, r12
	extu.w r8, r13
	movu.b (sp, 0), r6
	movu.w (sp, 0), r7
	cmpq/eq r6, r12
	bf _checksane_fail_mem
	cmpq/eq r7, r13
	bf _checksane_fail_mem

	mov sp, r3
	mov.l (sp, 4), r6
	mov.l (r3, 4), r7

	cmpq/eq r6, r7
	bf _checksane_fail_mem
	cmpq/eq r7, r11
	bf _checksane_fail_mem

	.L6:

	mov.l r10, (sp, 12)
	mov.q (sp, 8), r6
	cmpq/eq r6, r14
	bf _checksane_fail_mem

//	mov	0xE87AD208, r8
	mov	0xE87AD268, r11
	lea.q (r8, 96), r6

	cmpq/eq r6, r11
	bf _checksane_fail_mem

	bsr _checksane_ldrts_asm
//	break
	cmpq/eq r2, r11
	break?f
//	bf _checksane_fail_mem

	add	128, sp

//	pop r2
//	pop r2
//	pop r2
#endif

#if 0
//	push.x	r10
//	push	r11
//	push	r10

//	pop.x	r6
//	pop		r6
//	pop		r7

	cmpq/eq r6, r10
	bf _checksane_fail_mem

	cmpq/eq r7, r11
	bf _checksane_fail_mem
#endif

//	cmpeq r4, r5
//		break?t
//	cmpeq r4, r4
//		break?f

//	pop lr
//	rts

	mov.q	(sp), r1
	add		8, sp
	jmp		r1

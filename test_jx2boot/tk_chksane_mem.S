_checksane_fail_mem:
	break
	break
	bra _checksane_fail_mem
	break

.global X(tk_checksane_mem_asm)
X(tk_checksane_mem_asm):
	push lr

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

	mov.l r10, (sp, 12)
	mov.q (sp, 8), r6
	cmpq/eq r6, r14
	bf _checksane_fail_mem
	

	pop r2
	pop r2
	pop r2
#endif

	cmpeq r4, r5
		break?t
	cmpeq r4, r4
		break?f

	pop lr
	rts

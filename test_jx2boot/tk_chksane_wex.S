_checksane_wex_fail:
	break
	bra _checksane_wex_fail
	break


.global X(tk_checksane_wex_asm)
X(tk_checksane_wex_asm):
//	push lr
	add		-8, sp
	mov		lr, r1
	mov.q	r1, (sp)
	
	add #-24, sp

	.INVIC:

//	mov #-1, r0
//	invic r0
	nop
	nop
	nop

	.WEXMD:

	wexmd #1
	nop
	nop
	nop
	nop

	.L0A:

	mov #0x1234, r4 | mov #0x5678, r5
	add r4, r5, r2 | sub r5, r4, r3

	.L0B:

	mov 0x68AC, r6
	mov 0x4444, r7

	.L0C:
	
	cmpeq r2, r6
//	bf _checksane_wex_fail
	break?f

	.L0D:

	cmpeq r3, r7
//	bf _checksane_wex_fail
	break?f
	
	.L1:
	
	and r4, 0x7F, r2	| xor r5, 0x78, r3

	mov 0x0034, r8
	mov 0x5600, r9
	
	cmpeq r2, r8
//	bf _checksane_wex_fail
	break?f

	cmpeq r3, r9
//	bf _checksane_wex_fail
	break?f

	.L2:

	mov #0x1234, r4			| mov #0x89AB, r5
	ldish16 #0x5678, r4		| ldish16 #0xCDEF, r5
	add r4, r5, r2			| sub r5, r4, r3

	mov 0x9BE02467, r6
	mov 0x77777777, r7
	
	cmpeq r2, r6
//	bf _checksane_wex_fail
	break?f

	cmpeq r3, r7
//	bf _checksane_wex_fail
	break?f

	.L2B:

	mov #0x1234, r0			| mov #0x89AB, r1
	ldish16 #0x5678, r0		| ldish16 #0xCDEF, r1
	add r0, r1, r2			| sub r1, r0, r3

	cmpeq r2, r6
	break?f

	cmpeq r3, r7
	break?f

	mov #0x5678, r4
	mov #0xCDEF, r5

	shld r0, 16, r0		| shld r1, 16, r1
	shld r0, -16, r0	| shld r1, -16, r1

	cmpeq r0, r4
	break?f
	cmpeq r1, r5
	break?f


	.L3:

	mov #0x89AB, r1
	mov.l r1, (sp, 12)

	mov #0x1234, r0			| movd.l (sp, 12), r20
	ldish16 #0x5678, r0
	or r20, r0, r19
	mov 0x29530, r0

	mov 0x1234DFFB, r8

	
	cmpeq r19, r8
//	bf _checksane_wex_fail
	break?f

	wexmd #2

	mov #0x1234, r4		| mov #0x5678, r5	| mov #0x89AB, r6
	add r4, r5, r2		| sub r5, r4, r3	| xor r6, r5, r7

	mov 0x68AC, r8
	mov 0x4444, r9
	mov 0xDFD3, r10
	
	cmpeq r2, r8
//	bf _checksane_wex_fail
	break?f

	cmpeq r3, r9
//	bf _checksane_wex_fail
	break?f

	cmpeq r7, r10
//	bf _checksane_wex_fail
	break?f
	
	.L4:
	
	mov 0x1234, r8
	mov 0xABCD, r9
	mov 0x0C374FA4, r10
	mov 0xFA034FA4, r11
	
	mulu.w		r8, r9, r4
	muls.w		r8, r9, r5

	cmpeq r4, r10
	break?f

	cmpeq r5, r11
	break?f

	.L5:

	
	wexmd #0

	add #24, sp
//	pop lr
//	rts

	mov.q	(sp), r1
	add		8, sp
	jmp		r1

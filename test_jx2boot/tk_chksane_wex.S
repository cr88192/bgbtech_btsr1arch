_checksane_wex_fail:
	break
	bra _checksane_wex_fail
	break


.global X(tk_checksane_wex_asm)
X(tk_checksane_wex_asm):
	push lr

	mov #-1, r0
	invic r0

	wexmd #1
	nop
	nop
	nop
	nop

	mov #0x1234, r4 | mov #0x5678, r5
	add r4, r5, r2 | sub r5, r4, r3

	mov 0x68AC, r6
	mov 0x4444, r7
	
	cmpeq r2, r6
	bf _checksane_wex_fail

	cmpeq r3, r7
	bf _checksane_wex_fail
	
	and r4, 0x7F, r2	| xor r5, 0x78, r3

	mov 0x0034, r8
	mov 0x5600, r9
	
	cmpeq r2, r8
	bf _checksane_wex_fail

	cmpeq r3, r9
	bf _checksane_wex_fail
	
	wexmd #0

	pop lr
	rts

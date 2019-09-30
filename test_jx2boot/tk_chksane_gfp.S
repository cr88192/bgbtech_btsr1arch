_checksane_gfp_fail:
	break
	bra _checksane_fail
	break


.global X(tk_checksane_gfp_asm)
X(tk_checksane_gfp_asm):
	push lr

	mov	#0x400921f9f01b866e, r4		//3.14159
	mov	#0x401921f9f01b866e, r5		//6.28318
	mov #0x4000000000000000, r6		//2.0
	mov #0x3ff243f3e0370cdc, r7		//1.14159
	
	fadd r4, r4, r8
	fsub r4, r6, r9
	fmul r4, r6, r10
	
	cmpqeq r8, r5
	bf _checksane_gfp_fail
	
	cmpqeq r9, r7
	bf _checksane_gfp_fail

	cmpqeq r10, r5
	bf _checksane_gfp_fail


	fcmpeq r8, r5
	bf _checksane_gfp_fail

	fcmpeq r8, r7
	bt _checksane_gfp_fail

	fcmpgt r4, r4
	bt _checksane_gfp_fail

	fcmpgt r4, r5
	bf _checksane_gfp_fail


	fneg r4, r11
	
	fcmpeq r11, r4
	bt _checksane_gfp_fail

	fcmpgt r11, r4
	bf _checksane_gfp_fail


	/* Verify that FCMPEQ returns false for NaN */
	mov #0x7FFD000000000000, r19
	fcmpeq r19, r19
	bt _checksane_gfp_fail


	mov #0x40490fd0, r18
	mov #0x400921fa00000000, r19

	fstcf r4, r8
	fldcf r8, r9

	cmpeq r8, r18
	bf _checksane_gfp_fail

	cmpqeq r9, r19
	bf _checksane_gfp_fail

	mov #1234567, r20
	mov #0x4132d68700000000, r21
	
	fldci r20, r8
	fstci r8, r9

	cmpqeq r8, r21
	bf _checksane_gfp_fail

	cmpqeq r9, r20
	bf _checksane_gfp_fail

	pop lr
	rts

_checksane_gfp_fail:
	break
	bra _checksane_gfp_fail
	break


.global X(tk_checksane_gfp_asm)
X(tk_checksane_gfp_asm):
//	push lr

	add		-48, sp
	mov		lr, r1
	mov.q	r1, (sp, 40)


	mov	#0x400921f9f01b866e, r4		//3.14159
	mov	#0x401921f9f01b866e, r5		//6.28318
	mov #0x4000000000000000, r6		//2.0
	mov #0x3ff243f3e0370cdc, r7		//1.14159

	mov	#0xC00921f9f01b866e, r12	//-3.14159
	mov	#0xC01921f9f01b866e, r13	//-6.28318


	mov		r4, r8
	fadd	r4, r8

	mov		r4, r9
	fsub	r6, r9

	mov		r4, r10
	fmul	r6, r10

	cmpqeq r8, r5
//	bf _checksane_gfp_fail
	.A0_0:
	break?f
	
	cmpqeq r9, r7
//	bf _checksane_gfp_fail
	.A0_1:
	break?f

	cmpqeq r10, r5
//	bf _checksane_gfp_fail
	.A0_2:
	break?f

	/* Verify FPU Op */
	fadd r4, r4, r8
	fsub r4, r6, r9
	fmul r4, r6, r10
	
	cmpqeq r8, r5
	.A0_3:
	break?f
	
	cmpqeq r9, r7
	.A0_4:
	break?f

	cmpqeq r10, r5
	.A0_5:
	break?f

	/* FPU op mixed with mem-access */
	ADD		SP, -512, R2
	MOV.Q	(R2, 0), R3
	fadd r4, r4, r8
	MOV.Q	(R2, 32), R3
	fsub r4, r6, r9
	MOV.Q	(R2, 48), R3
	fmul r4, r6, r10
	
	.A0_5B:
	cmpqeq r8, r5
	break?f
	cmpqeq r9, r7
	break?f
	cmpqeq r10, r5
	break?f


	fcmpeq r8, r5
	.A0_6:
	break?f

	fcmpeq r8, r7
	.A0_7:
	break?t

	fcmpgt r4, r4
	.A0_8:
	break?t

	fcmpgt r4, r5
	.A0_9:
	break?f

	fcmpgt r5, r4
	.A0_10:
	break?t


	fneg r4, r11
	
	fcmpeq r11, r4
//	bt _checksane_gfp_fail
	.A0_11:
	break?t

	fcmpgt r11, r4
//	bf _checksane_gfp_fail
	.A0_12:
	break?f


	fcmpgt r12, r13
//	bt _checksane_gfp_fail
	.A0_13:
	break?t

	fcmpgt r13, r12
//	bf _checksane_gfp_fail
	.A0_14:
	break?f


	fstci	r4, r8
	fstci	r5, r9
	fstci	r6, r10

	cmpeq	#3, r8
//	bf _checksane_gfp_fail
	.A1_0:
	break?f

	cmpeq	#6, r9
//	bf _checksane_gfp_fail
	.A1_1:
	break?f
	cmpeq	#2, r10
//	bf _checksane_gfp_fail
	.A1_2:
	break?f


	/* Verify that FCMPEQ returns false for NaN */
	mov #0x7FFD000000000000, r19
	fcmpeq r19, r19
//	bt _checksane_gfp_fail
	.A1_3:
	break?t


	/* Verify Single<->Double Convert */
	mov #0x40490fd0, r18
	mov #0x400921fa00000000, r19
	mov #0x40490fd000000000, r20

	fstcf r4, r8
	fldcf r8, r9
	fldcfh r20, r10

	cmpeq r8, r18
//	bf _checksane_gfp_fail
	.A1_4:
	break?f

	cmpqeq r9, r19
//	bf _checksane_gfp_fail
	.A1_5:
	break?f

	cmpqeq r9, r10
	.A1_5B:
	break?f

	mov #1234567, r20
	mov #0x4132d68700000000, r21
	mov #-1234567, r22
	mov #0xC132d68700000000, r23
	
	fldci r20, r8
	fstci r8, r9

	cmpqeq r8, r21
//	bf _checksane_gfp_fail
	.A1_6:
	break?f

	cmpqeq r9, r20
//	bf _checksane_gfp_fail
	.A1_7:
	break?f


#if 1

.ifarch bjx2_fmovs
	fmov.s	r21, (sp)
	fmov.s	r21, (sp, 4)

	fmov.s	(sp), r8
	fmov.s	(sp, 4), r9

	cmpqeq r8, r21
	break?f

	cmpqeq r9, r21
	break?f
.endif

#endif

#if 1

.ifarch bjx2_fmovs
	mov		3, r2

	fmov.s	r23, (sp, r2)
	fmov.s	(sp, r2), r8

	cmpqeq r8, r23
	break?f
	
//	break
.endif

#endif


#if 1
	fldci r22, r8
	cmpqeq r8, r23
//	bf _checksane_gfp_fail
	.A1_8:
	break?f
#endif

#if 1
	fstci r8, r11
	cmpqeq r11, r22
//	bf _checksane_gfp_fail
	.A1_9:
	break?f
#endif

#if 1
	mov #0xC0BB3C0000000000, r24
	mov #-6972, r25

	fstci r24, r12
	cmpqeq r12, r25
//	bf _checksane_gfp_fail
	.A1_10:
	break?f
#endif

//	pop lr
//	rts

	mov.q	(sp, 40), r1
	add		48, sp
	jmp		r1

#if 1
/*
R4=src
R5=lsrc
R6=rdsta
R7=rdstb
*/

// void	TK_EncBlock16P(u16 *src, u16 *lsrc, u32 *rdsta, u32 *rdstb);

TK_EncBlock16P:

	/* Load the pixels for the block. */
	mov.q @(r4,    0), r18
	mov.q @(r4,  640), r19
	mov.q @(r4, 1280), r20
	mov.q @(r4, 1920), r21

	mov.q @(r5,    0), r22

	xor r2, r2		//i
	cmpqeq r18, r22
//	movnt r2

	/* Branch Early, we already know block differs. */
	bf .L0

	/* Load remaining pixels to compare against. */
	mov.q @(r5,  640), r23
	mov.q @(r5, 1280), r16
	mov.q @(r5, 1920), r17

	/* Check if any pixels differ. */
	cmpqeq r19, r23
//	movnt r3
//	movt r2
	movnt r2
//	or r3, r2
//	addc r2, r2

	cmpqeq r20, r16
	movnt r3
	or r3, r2
//	addc r2, r2

	cmpqeq r21, r17
	movnt r3
	or r3, r2
//	addc r2, r2

	tst r2, r2
	bf .L0
//	bt .L0
	
	/* Nothing changed, return now. */
	xor r2, r2
	rts
//	nop

/*
	At this point, we know we need to actually encode the block.
	R16=cmin
	R17=cmax
	R18=lpx0
	R19=lpx1
	R20=lpx2
	R21=lpx3
 */

	.L0:

	/* Copy updated pixels to lsrc. */
	mov.q r18, @(r5,    0)
	mov.q r19, @(r5,  640)
	mov.q r20, @(r5, 1280)
	mov.q r21, @(r5, 1920)

	/*
	Find lightest and darkest pixels.
	Shortcut is to only check half the pixels.
	Other pixels assumed to be in-between the ones checked.
	Should work correctly most of the time.
	*/

	mov #65535, r16
	mov #-1, r17

	shld r18, #-16, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r18, #-48, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


	extu.w r19, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r19, #-32, r2
	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


	shld r20, #-16, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r20, #-48, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


	extu.w r21, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r21, #-32, r2
	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	/*
	Now time to start encoding pixel bits for individual pixels.
	
	  At this point:
	  R0=DLR
	  R1=DHR, Zero
	  R2=scratch
	  R3=px2
	  R4=calo
	  R5=cahi
	  R16=cmin
	  R17=cmax
	  R18=lpx0
	  R19=lpx1
	  R20=lpx2
	  R21=lpx3
	  R22=cavg
	  R23=-
	*/

	add r16, r17, r22
	shad r22, #-1, r22
//	shlr1 r22

	add r16, r22, r4
	add r17, r22, r5
//	shlr1 r4
//	shlr1 r5

	shad r4, #-1, r4
	shad r5, #-1, r5


	xor r1, r1

	/* lpx0 */
	
	extu.w r18, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld r18, #-16, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r18, #-32, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r18, #-48, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3


	/* lpx1 */

	extu.w r19, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld r19, #-16, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r19, #-32, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r19, #-48, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3


	/* lpx2 */

	extu.w r20, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld r20, #-16, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r20, #-32, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r20, #-48, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3


	/* lpx3 */

	extu.w r21, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld r21, #-16, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r21, #-32, r2
	extu.w r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3

	shld.q r21, #-48, r2
	cmpgt r22, r2
	addc r3, r3
	cmpgt r5, r2
	addc r3, r3
	cmpgt r2, r4
	addc r1, r3


	mov r3, r2
	shlr1 r2
	not r2
	and #0x55555555, r2
	xor r2, r3

	shld r16, #-1, r16
	shld r17, #-1, r17
	add r16, #0x18000, r2
	shll16 r2
	shlr1 r2
	or r17, r2
	
//	xor r3, r3
	
	mov.l r2, @r6
	nop
	nop
	mov.l r3, @r7

	mov #1, r2
	rts

#endif

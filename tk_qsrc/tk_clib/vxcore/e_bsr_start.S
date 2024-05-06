// #define X(n)	_##n
#define X(n)	n
//#define STACK_BASE	0x0000DFFC
// #define STACK_BASE_ISR	0x0000DFF0
// #define STACK_BASE_ISR	0x0000DFC0
// #define STACK_BASE_USR	0x0000DF00

// #define STACK_BASE_USR	0x010FFFF0
// #define STACK_BASE_SYS	0x0101FFF0

#define STACK_BASE_USR	0x010FFFC0
#define STACK_BASE_SYS	0x0101FFC0
#define STACK_BASE_ISR	0x0100FFC0

.section .data

.balign 8
__tk_saved_vbr:
	.quad 0 
	.quad 0 

.section .text
.align 2
.global _start
// .global start
.global _vector_table
.extern X(__start)
.extern X(timer_ticks)

.extern X(__bss_start)
.extern X(_end)

// start:
_start:
//	mov STACK_BASE, sp
//	mov STACK_BASE_USR, sp

	mov		vbr, r2
//	mov.q	r2, __tk_saved_vbr

//	xor		r2, r2

	mov		__tk_saved_vbr, r3
	mov.q	r2, @r3
	
//	break

	mov		STACK_BASE_USR, r5
	mov		STACK_BASE_ISR, r6

	mov		sr, r7
	or 		#0x00C0, r7

	mov		isr_table, r4
.ifarch bjx2_xg2
	mov		0x0080000000000000, r16
	or		r16, r4
.endif
	tst		r2, r2
	nop
	mov?t	r4, vbr
	mov?t	r5, sp
	mov?t	r6, ssp
	mov?t	r7, sr
	

//	break?f

//	bsr		copy_data

#if 0
	mov		__tk_saved_vbr, r3
	mov.q	@r3, r2
	tst		r2, r2
	bf		.NoZero
	
	/* Skip .bss zeroing if loaded via program loader.
	 * It will have already zeroed it, and maybe added tripwires.
	 */
	bsr		zero_bss
	
	.NoZero:
#endif

//	mov		STACK_BASE_ISR, r0
//	mov		r0, ssp

//	mov		isr_table, r0
//	mov		r0, vbr

//	mov		isr_table, r4
//	tst		r2, r2
//	mov?t	r4, vbr

	bsr X(__start)
	bra _exit

_exit:
	break
	mov 0, r4
	mov 0x1003, r5
	mov 0, r6
	mov sp, r7
	syscall #0
	break
//	mov #-1, r0
//	jmp r0
//	nop

zero_bss:
	mov __bss_start, r2
	mov _end, r3
	mov #0, r4
.L0:
	mov.l r4, @-r3
	cmp/hi r2, r3
	bt .L0
	rts
	bra _exit

#if 0
copy_data:
	mov __rom_data_start, r2
	mov __rom_data_end, r3
	mov __data_start, r1
.L1:
	mov.l @r2, r4
	mov.l r4, @r1
	add #4, r1
	add #4, r2
	cmp/hi r2, r3
	bt .L1
	rts
#endif

isr_except:
	break
//	rte

	nop
	nop
	nop
	break
	nop

isr_inter:

#ifndef __USRONLY__
//	mov	exsr, r4
//	break
//	bsr __isr_interrupt
//	rte
	bra __isr_interrupt
#endif

	nop
	nop
	nop
	break
	nop


isr_tlbfault:
#ifndef __USRONLY__
//	break
	bra __isr_tlbfault
#endif

	nop
	nop
	nop
	break
	nop

#if 0
	mov		__tk_saved_vbr, r3
	mov.q	@r3, r2
//	lea.b	@(r2, 24), r3
	add		r2, #24, r3
	jmp		r3
#endif

isr_syscall:
//	break

#ifndef __USRONLY__
	mov		0x123456789ABCDEF, r16	//96-bit pad

	//adjust return point to be somewhere after syscall op
	mov		spc, r16
	add		8, r16
	mov		r16, spc

	bra __isr_syscall
#endif

	nop
	nop
	nop
	break
	nop

#if 0
	mov sp, r16
	mov STACK_BASE_SYS, sp
//	push r16
//	push gbr
//	push lr

	mov gbr, r17
	mov lr, r18

	add -24, sp
	mov.q r16, (sp, 16)
	mov.q r17, (sp, 8)
	mov.q r18, (sp, 0)

//	break

	bsr X(tk_isr_syscall)

	mov.q (sp, 16), r16
	mov.q (sp, 8), r17
	mov.q (sp, 0), r18
	add 24, sp

	mov r17, gbr
	mov r18, lr
	
//	pop lr
//	pop gbr
//	pop r16
	mov r16, sp

//	break
	
	rte
#endif

.balign 128

isr_table:
	bra8b _start
	bra8b isr_except
	bra8b isr_inter
	bra8b isr_tlbfault
	bra8b isr_syscall
//	bra8b X(__isr_syscall)

	nop4b
	nop4b
	nop4b
	nop4b

.global X(tk_getsavedvbr)
X(tk_getsavedvbr):
	mov		__tk_saved_vbr, r3
	mov.q	@r3, r2
	mov		0, r3
//	mov.q	__tk_saved_vbr, r2

//	tstq	r2, r2
//	break?f

	nop
	nop

	nop
	nop

//	xor r2, r2
	rts
	
	nop
	nop

#if 0
.global X(tk_syscall)
X(tk_syscall):
//	push lr
	syscall	#0
	nop
	nop
	nop
	
//	break
	
//	pop lr
	rts
#endif

#if 1
.global X(tk_syscall2)
X(tk_syscall2):
//	push lr

	syscall	#0
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
//	break
	
//	pop lr
	rts

.global X(tk_syscall2_rtuser)
X(tk_syscall2_rtuser):
	
	mov		0x12345678, R2
	mov		0x89ABCDEF, R7
	mov		0x1234CDEF, R16

	syscall	#1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
//	break
	
	rts
#endif

/*
	umullq: Unsigned Multiply, Long to Quad
	smullq: Signed Multiply, Long to Quad
*/

.global X(__umullq)
X(__umullq):
	dmulu	r4, r5, r2
	rts

#if 0
	dmulu r4, r5
	mov dhr, r3
	mov dlr, r2
	shll32 r3
	add r3, r2
	rts
#endif

.global X(__smullq)
X(__smullq):
	dmuls	r4, r5, r2
	rts

#if 0
	dmuls r4, r5
	mov dhr, r3
	mov dlr, r2
	shll32 r3
	add r3, r2
	rts
#endif

/*
	umulsq: Unsigned Multiply, Single Quad
	smulsq: Signed Multiply, Single Quad
*/
	
.global X(__umulsq)
.global X(__smulsq)
X(__umulsq):
X(__smulsq):
#if 1
	shld.q	r4, -32, r6
	shld.q	r5, -32, r7
	dmulu	r4, r5, r2

	dmulu	r4, r7, r16
	dmulu	r6, r5, r17
	add		r16, r17, r18
	shld.q	r18, 32, r3
	add		r3, r2

	rtsu
#endif

#if 0
	mov r4, r6
	mov r5, r7
	shlr32 r6
	shlr32 r7
	
	dmulu r4, r5
	mov dhr, r3
	mov dlr, r2

	dmulu r4, r7
	add dlr, r3
	dmulu r6, r5
	add dlr, r3

//	break

	shll32 r3
	add r3, r2

//	break

	rts
#endif

/*
	umuldq: Unsigned Multiply, Double Quad
*/
.global X(__umuldq)
X(__umuldq):
#if 1
	shld.q	r4, -32, r6
	shld.q	r5, -32, r7

	dmulu	r4, r5, r16
	dmulu	r4, r7, r17
	dmulu	r6, r5, r18
	dmulu	r6, r7, r19

	shld.q	r17, 32, r20
	shld.q	r17, -32, r21
	shld.q	r18, 32, r22
	shld.q	r18, -32, r23

	mov		r16, r2
	mov		r19, r3
	clrt
	addc	r20, r2
	addc	r21, r3
	clrt
	addc	r22, r2
	addc	r23, r3

	rtsu
#endif

#if 0
	mov r4, r6
	mov r5, r7
	shlr32 r6
	shlr32 r7
	
	dmulu r4, r5
	mov dhr, r3
	mov dlr, r2

	dmulu r4, r7
	add dlr, r3
	dmulu r6, r5
	add dlr, r3

//	break
	mov r3, r4
	shll32 r4
	add r4, r2

	shlr32 r3
	dmulu r6, r7
	add dlr, r3

//	break

	rts
#endif

/*
	smuldq: Signed Multiply, Double Quad
*/
.global X(__smuldq)
X(__smuldq):
#if 1
	shad.q	r4, -32, r6
	shad.q	r5, -32, r7

	dmulu	r4, r5, r16
	dmulu	r4, r7, r17
	dmulu	r6, r5, r18
	dmuls	r6, r7, r19

	shld.q	r17, 32, r20
	shld.q	r17, -32, r21
	shld.q	r18, 32, r22
	shld.q	r18, -32, r23

	mov		r16, r2
	mov		r19, r3
	clrt
	addc	r20, r2
	addc	r21, r3
	clrt
	addc	r22, r2
	addc	r23, r3

	rtsu
#endif

#if 0
	mov r4, r6
	mov r5, r7
	shlr32 r6
	shlr32 r7
	
	dmulu r4, r5
	mov dhr, r3
	mov dlr, r2

	dmulu r4, r7
	add dlr, r3
	dmulu r6, r5
	add dlr, r3

//	break
	mov r3, r4
	shll32 r4
	add r4, r2

	shlr32 r3
	dmuls r6, r7
	add dlr, r3

//	break

	rts
#endif


#if 0
.ifarch seen_idiv_var

divx3:
	rotcl r0
	div1
	rotcl r0
	div1
	rotcl r0
	div1
	rts
udiv_lgdiv:		/* large divisor */
//	push r5
	xor r4,r0
	.rept 4
	rotcl r0
	bsr divx3
	div1
	.endr
	rotcl r0
//	pop r5
//	pop r4
	jmp r1

div7:
	div1
div6:
	div1
	div1
	div1
	div1
	div1
	div1
	rts

div_xtrct_r4r0:
	mov r4, r2
	shll16 r2
	shlr16 r0
	or r2, r0
	rts
div_xtrct_r0r4:
	mov r0, r2
	shll16 r2
	shlr16 r4
	or r2, r4
	rts

.global X(__udivsi3_asm)
X(__udivsi3_asm):
	mov pr, r1
//	push r4
	extu.w r5, r0
	cmp/eq r5, r0
	swap.w r4, r0
	shlr16 r4
	div0
	bf udiv_lgdiv

	/* small divisor route */
//	push r5
	shll16 r5
	div1
	bsr div6
	div1
	div1
	bsr div6
	div1
	bsr div_xtrct_r4r0
	bsr div_xtrct_r0r4
	bsr div7
	swap.w r4
	div1
	bsr div7
	div1
	bsr div_xtrct_r4r0
	swap.w r0
	rotcl r0
//	pop r5
//	pop r4
	jmp r1

.endif
#endif

.ifarch seen_ishr_var
.global X(__lshrsi3)
X(__lshrsi3):
	mov r5, r2
	not r2
	and #31, r2
	mov lshr_list, r1
	lea.w (r1, r2), r1
	mov r4, r0
	jmp r1

lshr_list:
	shlr1 r0 /* 31 */
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0 /* 24 */
	shlr1 r0 /* 23 */
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0 /* 16 */
	shlr1 r0 /* 15 */
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0 /* 8 */
	shlr1 r0 /* 7 */
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0
	shlr1 r0 /* 1 */
	rts
.endif

.ifarch seen_isar_var
.global X(__ashrsi3)
X(__ashrsi3):
	mov r5, r2
	not r2
	and #31, r2
	mov ashr_list, r1
	lea.w (r1, r2), r1
	mov r4, r0
	jmp r1

ashr_list:
	shar1 r0 /* 31 */
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0 /* 24 */
	shar1 r0 /* 23 */
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0 /* 16 */
	shar1 r0 /* 15 */
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0 /* 8 */
	shar1 r0 /* 7 */
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0
	shar1 r0 /* 1 */
	rts
.endif

.ifarch seen_ishl_var
.global X(__ashlsi3)
X(__ashlsi3):
	mov r5, r2
	not r2
	and #31, r2
	mov ashl_list, r1
	lea.w (r1, r2), r1
	mov r4, r0
//	break
	jmp r1

ashl_list:
	shll1 r0 /* 31 */
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0 /* 24 */
	shll1 r0 /* 23 */
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0 /* 16 */
	shll1 r0 /* 15 */
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0 /* 8 */
	shll1 r0 /* 7 */
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0
	shll1 r0 /* 1 */
	rts
.endif

//void __tk_farcall(void *fptr, void *gbr, void *newstack, void *tbr)
.global X(__tk_farcall)
X(__tk_farcall):
	invic	#-1
	mov		r7, tbr
	mov		r5, gbr
	mov		r6, sp
	jmp		r4


.global X(__longj_sys)
X(__longj_sys):

#if 0
	mov.q  @(r4, 32), r1
	mov r1, tbr

	mov.q  @(r4, 40), r1
	mov r1, ttb

//	mov.q  @(r4, 48), r1
//	mov r1, mmcr

//	mov.q  @(r4, 128), r1
//	mov r1, vbr
#endif

//	mov.q  @(r4, 56), r1
//	mov r1, krr


	mov.q  @(r4, 544), r1
	mov r1, tbr

	mov.q  @(r4, 552), r1
	mov r1, ttb

	mov.q  @(r4, 576), r1
	mov r1, krr


	mov		0, r2
	invic	r2
	invdc	r2

.global X(__longj)
X(__longj):
//	mov r4, r1
	mov r5, r2

	test	r2, r2
	mov?t	1, r2

//	mov.q  @(16,r4), r0
//	lds r0, pr

#if 0
	mov.q  @(r4, 0), r1
	mov r1, lr

	mov.q  @(r4, 8), r1
	mov r1, gbr

	mov.q  @(r4, 16), r1
	mov r1, sr
#endif

#if 1
	mov.q  @(r4, 512), r1
	mov r1, gbr

	mov.q  @(r4, 520), r1
	mov r1, lr

//	mov.q  @(r4, 16), r1
//	mov r1, sr
#endif

//	mov.q  @(r4, 24), r1
//	mov r1, ssp

//	mov		STACK_BASE_ISR, r0
//	mov		r0, ssp

#if 0
	mov.q  @(r4, 32), r1
	mov r1, tbr

	mov.q  @(r4, 40), r1
	mov r1, ttb

	mov.q  @(r4, 48), r1
	mov r1, mmcr

	mov.q  @(r4, 128), r1
	mov r1, vbr
#endif

//	mov.q  @(r4, 56), r1
//	mov r1, krr

.ifarch has_xgpr

	mov.q	( 24,r4), r3
	mov.q	( 40,r4), r5

	mov.x	( 48,r4), r6
	mov.x	( 64,r4), r8
	mov.x	( 80,r4), r10
	mov.x	( 96,r4), r12

	mov.q	(112,r4), r14
	mov.q	(120,r4), r15

//	mov.x	(128,r4), r16
	mov.x	(144,r4), r18
	mov.x	(160,r4), r20
	mov.x	(176,r4), r22
	mov.x	(192,r4), r24
	mov.x	(208,r4), r26
	mov.x	(224,r4), r28
	mov.x	(240,r4), r30

	mov.x	(256,r4), r32
	mov.x	(272,r4), r34
	mov.x	(288,r4), r36
	mov.x	(304,r4), r38
	mov.x	(320,r4), r40
	mov.x	(336,r4), r42
	mov.x	(352,r4), r44
	mov.x	(368,r4), r46

	mov.x	(384,r4), r48
	mov.x	(400,r4), r50
	mov.x	(416,r4), r52
	mov.x	(432,r4), r54
	mov.x	(448,r4), r56
	mov.x	(464,r4), r58
	mov.x	(480,r4), r60
	mov.x	(496,r4), r62

.else

//	mov.q  @(  0,r4), r0
//	mov.q  @(  8,r4), r1
//	
	mov.q  @( 24,r4), r3
//	
	mov.q  @( 40,r4), r5
	mov.q  @( 48,r4), r6
	mov.q  @( 56,r4), r7
	mov.q  @( 64,r4), r8
	mov.q  @( 72,r4), r9
	mov.q  @( 80,r4), r10
	mov.q  @( 88,r4), r11
	mov.q  @( 96,r4), r12
	mov.q  @(104,r4), r13
	mov.q  @(112,r4), r14
	mov.q  @(120,r4), r15

	mov.q  @(128,r4), r16
	mov.q  @(136,r4), r17
	mov.q  @(144,r4), r18
	mov.q  @(152,r4), r19
	mov.q  @(160,r4), r20
	mov.q  @(168,r4), r21
	mov.q  @(176,r4), r22
	mov.q  @(184,r4), r23
	mov.q  @(192,r4), r24
	mov.q  @(200,r4), r25
	mov.q  @(208,r4), r26
	mov.q  @(216,r4), r27
	mov.q  @(224,r4), r28
	mov.q  @(232,r4), r29
	mov.q  @(240,r4), r30
	mov.q  @(248,r4), r31

.endif

	mov.x	(r4, 0x300), r16
	mov.x	(r4, 0x310), r18
	mov.x	(r4, 0x320), r20
	mov.x	(r4, 0x330), r22
	mov.x	r16, (r15, 0x000)
	mov.x	r18, (r15, 0x010)
	mov.x	r20, (r15, 0x020)
	mov.x	r22, (r15, 0x030)
	mov.x	(r4, 0x340), r16
	mov.x	(r4, 0x350), r18
	mov.x	(r4, 0x360), r20
	mov.x	(r4, 0x370), r22
	mov.x	r16, (r15, 0x040)
	mov.x	r18, (r15, 0x050)
	mov.x	r20, (r15, 0x060)
	mov.x	r22, (r15, 0x070)
	mov.x	(r4, 0x380), r16
	mov.x	(r4, 0x390), r18
	mov.x	(r4, 0x3A0), r20
	mov.x	(r4, 0x3B0), r22
	mov.x	r16, (r15, 0x080)
	mov.x	r18, (r15, 0x090)
	mov.x	r20, (r15, 0x0A0)
	mov.x	r22, (r15, 0x0B0)
	mov.x	(r4, 0x3C0), r16
	mov.x	(r4, 0x3D0), r18
	mov.x	(r4, 0x3E0), r20
	mov.x	(r4, 0x3F0), r22
	mov.x	r16, (r15, 0x0C0)
	mov.x	r18, (r15, 0x0D0)
	mov.x	r20, (r15, 0x0E0)
	mov.x	r22, (r15, 0x0F0)

//	break


//	mov r2, r0
	rts
	nop


.global X(__setj_sys)
X(__setj_sys):

#if 0
	mov ssp, r1
	mov.q  r1, @(r4, 24)

	mov tbr, r1
	mov.q  r1, @(r4, 32)

	mov ttb, r1
	mov.q  r1, @(r4, 40)

	mov mmcr, r1
	mov.q  r1, @(r4, 48)

	mov vbr, r1
	mov.q  r1, @(r4, 128)
#endif

//	mov krr, r1
//	mov.q  r1, @(r4, 56)


	mov tbr, r1
	mov.q  r1, @(r4, 544)

	mov ttb, r1
	mov.q  r1, @(r4, 552)

	mov krr, r1
	mov.q  r1, @(r4, 576)

.global X(__setj)
X(__setj):

.ifarch has_xgpr

//	mov.q  r0, @(  0,r4)
//	mov.q  r1, @(  8,r4)

//	mov.x  r2, @( 16,r4)
//	mov.x  r4, @( 32,r4)
//	mov.x  r6, @( 48,r4)
	mov.x  r8, @( 64,r4)
	mov.x r10, @( 80,r4)
	mov.x r12, @( 96,r4)

	mov.q r14, @(112,r4)
	mov.q r15, @(120,r4)

//	mov.x r16, @(128,r4)
	mov.x r18, @(144,r4)
	mov.x r20, @(160,r4)
	mov.x r22, @(176,r4)
	mov.x r24, @(192,r4)
	mov.x r26, @(208,r4)
	mov.x r28, @(224,r4)
	mov.x r30, @(240,r4)

	mov.x r32, @(256,r4)
	mov.x r34, @(272,r4)
	mov.x r36, @(288,r4)
	mov.x r38, @(304,r4)
	mov.x r40, @(320,r4)
	mov.x r42, @(336,r4)
	mov.x r44, @(352,r4)
	mov.x r46, @(368,r4)

	mov.x r48, @(384,r4)
	mov.x r50, @(400,r4)
	mov.x r52, @(416,r4)
	mov.x r54, @(432,r4)
	mov.x r56, @(448,r4)
	mov.x r58, @(464,r4)
	mov.x r60, @(480,r4)
	mov.x r62, @(496,r4)

.else

//	mov.q  r0, @(  0,r4)
//	mov.q  r1, @(  8,r4)
//	mov.q  r2, @( 16,r4)
//	mov.q  r3, @( 24,r4)
//	mov.q  r4, @( 32,r4)
//	mov.q  r5, @( 40,r4)
//	mov.q  r6, @( 48,r4)
//	mov.q  r7, @( 56,r4)
	mov.q  r8, @( 64,r4)
	mov.q  r9, @( 72,r4)
	mov.q r10, @( 80,r4)
	mov.q r11, @( 88,r4)
	mov.q r12, @( 96,r4)
	mov.q r13, @(104,r4)
	mov.q r14, @(112,r4)
	mov.q r15, @(120,r4)

	mov.q r16, @(128,r4)
	mov.q r17, @(136,r4)
	mov.q r18, @(144,r4)
	mov.q r19, @(152,r4)
	mov.q r20, @(160,r4)
	mov.q r21, @(168,r4)
	mov.q r22, @(176,r4)
	mov.q r23, @(184,r4)
	mov.q r24, @(192,r4)
	mov.q r25, @(200,r4)
	mov.q r26, @(208,r4)
	mov.q r27, @(216,r4)
	mov.q r28, @(224,r4)
	mov.q r29, @(232,r4)
	mov.q r30, @(240,r4)
	mov.q r31, @(248,r4)

.endif

	mov gbr, r1
	mov.q  r1, @(r4, 512)

	mov lr, r1
	mov.q  r1, @(r4, 520)

	mov.x	(r15, 0x000), r16
	mov.x	(r15, 0x010), r18
	mov.x	(r15, 0x020), r20
	mov.x	(r15, 0x030), r22
	mov.x	r16, (r4, 0x300)
	mov.x	r18, (r4, 0x310)
	mov.x	r20, (r4, 0x320)
	mov.x	r22, (r4, 0x330)
	mov.x	(r15, 0x040), r16
	mov.x	(r15, 0x050), r18
	mov.x	(r15, 0x060), r20
	mov.x	(r15, 0x070), r22
	mov.x	r16, (r4, 0x340)
	mov.x	r18, (r4, 0x350)
	mov.x	r20, (r4, 0x370)
	mov.x	r22, (r4, 0x370)
	mov.x	(r15, 0x080), r16
	mov.x	(r15, 0x090), r18
	mov.x	(r15, 0x0A0), r20
	mov.x	(r15, 0x0B0), r22
	mov.x	r16, (r4, 0x380)
	mov.x	r18, (r4, 0x390)
	mov.x	r20, (r4, 0x3A0)
	mov.x	r22, (r4, 0x3B0)
	mov.x	(r15, 0x0C0), r16
	mov.x	(r15, 0x0D0), r18
	mov.x	(r15, 0x0E0), r20
	mov.x	(r15, 0x0F0), r22
	mov.x	r16, (r4, 0x3C0)
	mov.x	r18, (r4, 0x3D0)
	mov.x	r20, (r4, 0x3E0)
	mov.x	r22, (r4, 0x3F0)

#if 0
//	sts pr, r1
//	mov #16, r0
//	mov.q  r1, @(r0,r4)

	mov lr, r1
	mov.q  r1, @(r4, 0)

	mov gbr, r1
	mov.q  r1, @(r4, 8)

	mov sr, r1
	mov.q  r1, @(r4, 16)
#endif

#if 0
	mov ssp, r1
	mov.q  r1, @(r4, 24)

	mov tbr, r1
	mov.q  r1, @(r4, 32)

	mov ttb, r1
	mov.q  r1, @(r4, 40)

	mov mmcr, r1
	mov.q  r1, @(r4, 48)

	mov vbr, r1
	mov.q  r1, @(r4, 128)
#endif

//	mov krr, r1
//	mov.q  r1, @(r4, 56)
	
//	mov #0, r0
	mov #0, r2
	rts
	nop


.global X(__va64_saveargs)
X(__va64_saveargs):
	mov.x r4, (r3, 0)
	mov.x r6, (r3, 16)
	mov.x r20, (r3, 32)
	mov.x r22, (r3, 48)
.ifarch abi_is_xgpr
	mov.x r36, (r3, 64)
	mov.x r38, (r3, 80)
	mov.x r52, (r3, 96)
	mov.x r54, (r3, 112)
.endif
	rts


// .global X(__va64_arg_l)
// X(__va64_arg_l):

.global X(__va64_arg_i)
X(__va64_arg_i):
//	mov.l @(r4, 96), r2
	mov.l @(r4, 128), r2
.ifarch abi_is_xgpr
	mov #128, r3
.else
	mov #64, r3
.endif
	cmp/gt r2, r3
	bf __va64_arg_i.L0
	
	mov r2, r3
	add r4, r3
	mov.q @r3, r6
	add #8, r2
//	mov.l  r2, @(r4, 96)
	mov.l  r2, @(r4, 128)
	mov r6, r2
	rts
	nop

__va64_arg_i.L0:
//	mov.q @(r4, 112), r3
	mov.q @(r4, 144), r3
	mov.l @r3+, r6
//	mov.q  r3, @(r4, 112)
	mov.q  r3, @(r4, 144)
	mov r6, r2
//	break
	rts
	nop

#if 1
.global X(__va64_arg_l)
X(__va64_arg_l):
//	mov.l @(r4, 96), r2
	mov.l @(r4, 128), r2
.ifarch abi_is_xgpr
	mov #128, r3
.else
	mov #64, r3
.endif
	cmp/gt r2, r3
	bf __va64_arg_l.L0	

	mov r2, r3
	add r4, r3
	mov.q @r3, r6
	add #8, r2
//	mov.l  r2, @(r4, 96)
	mov.l  r2, @(r4, 128)
	mov r6, r2
	rts
	nop
__va64_arg_l.L0:
//	mov.q @(r4, 112), r3
	mov.q @(r4, 144), r3
	add #7, r3
	and #-8, r3
	mov.q @r3+, r6
//	mov.q  r3, @(r4, 112)
	mov.q  r3, @(r4, 144)
	mov r6, r2
	rts
	nop
#endif


.global X(__va64_arg_x)
X(__va64_arg_x):
//	mov.l @(r4, 96), r2
	mov.l @(r4, 128), r2
.ifarch abi_evenonly
	test	#8, r2
	add?f	#8, r2
.endif
.ifarch abi_is_xgpr
	mov		#120, r3
.else
	mov		#56, r3
.endif
	cmp/gt	r2, r3
	bf		__va64_arg_x.L0
	mov		r2, r3
	add		r4, r3
//	mov.q	@r3, r6
//	add		#8, r3
//	mov.q	@r3, r7
//	add		#8, r3
	mov.q	@(r3, 0), r6
	mov.q	@(r3, 8), r7
	add		#16, r2
//	mov.l	r2, @(r4, 96)
	mov.l	r2, @(r4, 128)
	mov		r6, r2
	mov		r7, r3
	rts
	nop
__va64_arg_x.L0:
//	mov.q @(r4, 112), r3
	mov.q @(r4, 144), r3
	add #7, r3
	and #-8, r3
	mov.q @r3+, r6
	mov.q @r3+, r7
//	mov.q  r3, @(r4, 112)
	mov.q  r3, @(r4, 144)
	mov r6, r2
	mov r7, r3
	rts
	nop

#if 0
.ifnarch bjx1_nofpu

.global X(__va64_arg_f)
X(__va64_arg_f):
	mov.l @(r4, 104), r2
	mov #96, r3
	cmp/gt r2, r3
	bf __va64_arg_f.L0

	mov r4, r3
	add r2, r3
	fmov.d @r3, fr2
	add #8, r2
	mov r2, r1
	mov.l  r1, @(r4, 104)
	rts
	nop
__va64_arg_f.L0:
	mov.q @(r4, 112), r3
	fmov.s @r3, fr2
	add #4, r3
	mov.q  r3, @(r4, 112)
	rts
	nop

.global X(__va64_arg_d)
X(__va64_arg_d):
	mov.l @(r4, 104), r2
	add #7, r2
	mov #-8, r1
	and r1, r2
	
	mov #96, r1
	cmp/gt r2, r1
	bf __va64_arg_d.L0

	mov r4, r3
	add r2, r3
	fmov.d @r3, fr2

	add #8, r2
	mov r2, r1
	mov.l  r1, @(r4, 104)
	rts
	nop
__va64_arg_d.L0:
	mov.q @(r4, 112), r3
	fmov.d @r3, fr2
	add #8, r3
	mov.q  r3, @(r4, 112)
	rts
	nop

#if 0
.global X(__ldhf16)
X(__ldhf16):
	mov r4, dlr
//	fldch fr0
	fldch fr2
	rts

.global X(__sthf16)
X(__sthf16):
	fstch fr4
	mov dlr, r2
	rts
#endif

.endif
#endif

.ifarch abi_is_xgpr

.global X(__memcpy32)
X(__memcpy32):
	mov		0, r18
	shad	r20, -2, r19
	.L0:
	xmov.l	(r6, r18), r2
	xmov.l	r2, (r4, r18)
	add		1, r18
	cmpqgt	r18, r19
	bt .L0
	rts

.global X(__memcpy64)
X(__memcpy64):
	mov		0, r18
	shad	r20, -3, r19
	.L0:
	xmov.q	(r6, r18), r2
	xmov.q	r2, (r4, r18)
	add		1, r18
	cmpqgt	r18, r19
	bt .L0
	rts

.global X(__memcpy128)
X(__memcpy128):
	mov		0, r18
	shad	r20, -3, r19
	.L0:
	add		r18, 1, r21
	xmov.q	(r6, r18), r2
	xmov.q	(r6, r21), r3
	xmov.q	r2, (r4, r18)
	xmov.q	r3, (r4, r21)
	add		2, r18
	cmpqgt	r18, r19
	bt .L0
	rts

.global X(_memcpyf)
X(_memcpyf):

.global X(memcpy)
X(memcpy):

.ifarch abi_boundschk
	add			r6, -1, r7
	bndchk.b	0, r4
	bndchk.b	0, r5
	bndchk.b	r7, r4
	bndchk.b	r7, r5
.endif

	or		r4, r6, r3
	or		r3, r20, r3
	test	7, r3
	bt		__memcpy128

	mov		0, r18
	shad	r20, -3, r19
	.L0:
	xmov.q	(r6, r18), r2
	xmov.q	r2, (r4, r18)
	add		1, r18
	cmpqgt	r18, r19
	bt .L0

	and		r20, 7, r3
	test	r3, r3
	bt		.end
	
	shad	r18, 3, r18
	xlea.b	(r6, r18), r6
	xlea.b	(r4, r18), r4

	mov		0, r18
	.L1:
	xmov.b	(r6, r18), r2
	xmov.b	r2, (r4, r18)
	add		1, r18
	cmpqgt	r18, r3
	bt .L1

	.end:
	rts

.global X(memcpy_movx)
X(memcpy_movx):
	or		r4, r6, r3
	or		r3, r20, r3
	test	7, r3
	bt		__memcpy128
	bra		memcpy

.else

.global X(__memcpy32)
X(__memcpy32):

#if 1
	mov r5, r3
	add r6, r3
__memcpy32.L0:
	mov.l @r5, r1
	mov.l r1, @r4
	add #4, r5
	add #4, r4
	cmpqhi r5, r3
	bt __memcpy32.L0
//	break
	rts
#endif

#if 0
	mov.q @(r5,  0), r0
	mov.q @(r5,  8), r1
	mov.q @(r5, 16), r2
	mov.q @(r5, 24), r3
	mov.q r0, @(r4,  0)
	mov.q r1, @(r4,  8)
	mov.q r2, @(r4, 16)
	mov.q r3, @(r4, 24)
	rts
#endif

.global X(__memcpy64)
X(__memcpy64):

#if 1
	mov r5, r3
	add r6, r3
__memcpy64.L0:
	mov.q @r5, r1
	mov.q r1, @r4
	add #8, r5
	add #8, r4
	cmpqhi r5, r3
	bt __memcpy64.L0
//	break
	rts
#endif

.global X(__memcpy128)
X(__memcpy128):

#if 1
	mov r5, r3
	add r6, r3
__memcpy128.L0:
	mov.q @(r5, 0), r0
	mov.q @(r5, 8), r1
	mov.q r0, @(r4, 0)
	mov.q r1, @(r4, 8)
	add #16, r5
	add #16, r4
	cmpqhi r5, r3
	bt __memcpy128.L0
//	break
	rts
#endif

.global X(__memcpy8_16)
X(__memcpy8_16):
	mov.q @(r5, 0), r0
	mov.q @(r5, 8), r1
	mov.q r0, @(r4, 0)
	mov.q r1, @(r4, 8)
	rts

.ifarch bjx2_movx
memcpy_movx:
	MOV		R4, R2

#if 1
	OR		R4, R5, R7
	TEST	15, R7
	BT		.L0A

	TEST	7, R7
	BT		.L1

	CMPGE	40, R6
	BF		.L1

	XOR		R4, R5, R7
	TEST	8, R7
	BF		.L0A

//	AND		R4, 15, R16
//	AND		R5, 15, R17
//	CMPEQ	R16, R17
//	BF		.L0A
	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5
	ADD		8, R4
	MOV.Q	R20, (R4,  0)

	.L0A:
#endif

#if 1
	CMPGE	64, R6
	BF		.L1B
	.L0B:
	MOV.X	(R5,  0), R16
	MOV.X	(R5, 16), R18
	MOV.X	(R5, 32), R20
	MOV.X	(R5, 48), R22
	ADD		64, R5		|	ADD		-64, R6
	MOV.X	R16, (R4,  0)
	MOV.X	R18, (R4, 16)
	MOV.X	R20, (R4, 32)
	MOV.X	R22, (R4, 48)
	ADD		64, R4		|	CMPGE	64, R6
	BT		.L0B
	.L1B:
#endif

	CMPGE	32, R6
	BF		.L1
	.L0:
	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22
	ADD		32, R5		|	ADD		-32, R6
	MOV.X	R20, (R4,  0)
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMPGE	32, R6
	BT		.L0
	.L1:

	CMPGE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5
	MOV.Q	R20, (R4,  0)
	ADD		8, R4		|	CMPGE	8, R6
	BT		.L2
	.L3:

#if 1
	TEST	R6, R6
//	RTSU?T
	BT		.L5

	CMPGE		4, R6
	MOV.L?T		(R5), R3
	MOV.L?T		R3, (R4)
	ADD?T		4, R4
	ADD?T		4, R5
	ADD?T		-4, R6

	CMPGE		2, R6
	MOV.W?T		(R5), R3
	MOV.W?T		R3, (R4)
	ADD?T		2, R4
	ADD?T		2, R5
	ADD?T		-2, R6

	CMPGE		1, R6
	MOV.B?T		(R5), R3
	MOV.B?T		R3, (R4)

	.L5:
#endif

	RTS


memcpy_movx_f:
	MOV		R4, R2

#if 1
	OR		R4, R5, R7
	TEST	15, R7
	BT		.L0A

	CMPGE		40, R6
	BF			.L1

	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5		|	MOV.Q	R20, (R4,  0)
	ADD		8, R4

	.L0A:
#endif

	CMPGT	32, R6
	BF		.L1
	.L0:
	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22
	ADD		32, R5		|	ADD		-32, R6
	MOV.X	R20, (R4,  0)
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMPGT	32, R6
	BT		.L0
	.L1:

	CMPGE	0, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5		|	MOV.Q	R20, (R4,  0)
	ADD		8, R4		|	CMPGE	0, R6
	BT		.L2
	.L3:

	RTS
.endif

.ifnarch abi_is_xgpr

.global X(memcpy)
X(memcpy):
#if 1
.ifarch bjx2_movx
	OR		r4, r5,	r7
//	OR		r7, r6,	r7
	TEST	#7, r7
	BT		memcpy_movx
.endif
#endif

.ifarch bjx2_wex3p
	cmpge	#32, r6
	bf		.L1
	.L0:
	mov.q	(r5,  0), r20
	mov.q	(r5,  8), r21
	mov.q	(r5, 16), r22
	mov.q	(r5, 24), r23
	mov.q	r20, (r4,  0)
	mov.q	r21, (r4,  8)
	add		#32, r5		| mov.q		r22, (r4, 16)
	add		#-32, r6	| mov.q		r23, (r4, 24)
	add		#32, r4		| cmpge		32, r6
	bt		.L0
	.L1:

	cmpge	#8, r6
	bf		.L3
	.L2:
	add		#-8, r6		| mov.q		(r5), r2
	add		#8, r5		| mov.q		r2, (r4)
	add		#8, r4		| cmpge		#8, r6
	bt		.L2
	.L3:
.else
	cmp/ge	#32, r6
	bf		.L1
	.L0:
	mov.q	(r5,  0), r20
	mov.q	(r5,  8), r21
	mov.q	(r5, 16), r22
	mov.q	(r5, 24), r23
	mov.q	r20, (r4,  0)
	mov.q	r21, (r4,  8)
	mov.q	r22, (r4, 16)
	mov.q	r23, (r4, 24)
	add		#32, r4
	add		#32, r5
	add		#-32, r6
	cmp/ge	#32, r6
	bt		.L0
	.L1:

	cmp/ge	#8, r6
	bf		.L3
	.L2:
	mov.q	(r5), r2
	mov.q	r2, (r4)
	add		#8, r4
	add		#8, r5
	add		#-8, r6
	cmp/ge	#8, r6
	bt		.L2
	.L3:
.endif

#if 0
	cmp/ge #16, r6
	mov.q?t		(r5,  0), r2
	mov.q?t		(r5,  8), r3
	mov.q?t		r2, (r4,  0)
	mov.q?t		r3, (r4,  8)
	add?t		#16, r4
	add?t		#16, r5
	add?t		#-16, r6

	cmp/ge #8, r6
	mov.q?t		(r5), r2
	mov.q?t		r2, (r4)
	add?t		#8, r4
	add?t		#8, r5
	add?t		#-8, r6
#endif

#if 1
	cmpge		#4, r6
	mov.l?t		(r5), r2
	mov.l?t		r2, (r4)
	add?t		#4, r4
	add?t		#4, r5
	add?t		#-4, r6

	cmpge		#2, r6
	mov.w?t		(r5), r2
	mov.w?t		r2, (r4)
	add?t		#2, r4
	add?t		#2, r5
	add?t		#-2, r6

	cmpge		#1, r6
	mov.b?t		(r5), r2
	mov.b?t		r2, (r4)
#endif
	rtsu



.global X(_memcpyf)
X(_memcpyf):
#if 1
.ifarch bjx2_movx
	OR		r4, r5,	r7
//	OR		r7, r6,	r7
	TEST	#7, r7
	BT		memcpy_movx_f
.endif
#endif

	mov		r4, r2

.ifarch bjx2_wex3p
	cmpge	#32, r6
	bf		.L1
	.L0:
	mov.q	(r5,  0), r20
	mov.q	(r5,  8), r21
	mov.q	(r5, 16), r22
	mov.q	(r5, 24), r23
	mov.q	r20, (r4,  0)
	mov.q	r21, (r4,  8)
	add		#32, r5		| mov.q		r22, (r4, 16)
	add		#-32, r6	| mov.q		r23, (r4, 24)
	add		#32, r4		| cmpge		32, r6
	bt		.L0
	.L1:

	cmpge	#0, r6
	bf		.L3
	.L2:
	add		#-8, r6		| mov.q		(r5), r3
	add		#8, r5		| mov.q		r3, (r4)
	add		#8, r4		| cmpge		#0, r6
	bt		.L2
	.L3:
.else
	cmp/ge	#32, r6
	bf		.L1
	.L0:
	mov.q	(r5,  0), r20
	mov.q	(r5,  8), r21
	mov.q	(r5, 16), r22
	mov.q	(r5, 24), r23
	mov.q	r20, (r4,  0)
	mov.q	r21, (r4,  8)
	mov.q	r22, (r4, 16)
	mov.q	r23, (r4, 24)
	add		#32, r4
	add		#32, r5
	add		#-32, r6
	cmp/ge	#32, r6
	bt		.L0
	.L1:

	cmp/ge	#0, r6
	bf		.L3
	.L2:
	mov.q	(r5), r3
	mov.q	r3, (r4)
	add		#8, r4
	add		#8, r5
	add		#-8, r6
	cmp/ge	#0, r6
	bt		.L2
	.L3:
.endif

	rtsu

.endif


.endif


.global X(sleep_0)
X(sleep_0):
	sleep
	rts

#if 1

.ifnarch bjx1_nofpu

.ifarch bjx1_fpu_gfp

.ifarch bjx2_wex

/*
	Stomps r2, r3, r5, r6, r7
 */
.global X(__fpu_frcp)
X(__fpu_frcp):
	mov #0, r3
	fcmp/gt r4, r3

#if 0
	bf .L0
	mov lr, r17
	fneg r4
	bsr __fpu_frcp
//	fneg r2, r2		| jmp r17
	mov r17, lr
	fneg r2
	rts
#endif

	nop
	fneg?t r4

	.L0:
.ifarch has_fpvsf_sp
//	mov #0x7FF, r5		| mov #0x3FF, r6
	mov #0x7FE, r5		| mov #0x3FF, r6
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2
.else
//	mov #0x7FF, r5		| mov #0x3FF, r6
	mov #0x7FE, r5		| mov #0x3FF, r6
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2
.endif

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fneg?t r2

	rtsu

.global X(__fpu_frcp_s)
X(__fpu_frcp_s):
	mov #0, r3
	fcmp/gt r4, r3

#if 0
	bf .L0
	mov lr, r17
	fneg r4
	bsr __fpu_frcp_s
//	fneg r2, r2		| jmp r17
	mov r17, lr
	fneg r2
	rts
#endif

	nop
	fneg?t r4

	.L0:
//	mov #0x7FF, r5		| mov #0x3FF, r6
	mov #0x7FE, r5		| mov #0x3FF, r6
.ifarch has_fpvsf_sp
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2
.else
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2
.endif
	fneg?t r2
	
	rtsu

.global X(__fpu_fdiv)
X(__fpu_fdiv):
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp
	fmul r18, r2
	mov r16, lr
	rts
.global X(__fpu_fdiv_s)
X(__fpu_fdiv_s):
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_s
	fmul r18, r2
	mov r16, lr
	rts


.global X(__fpu_frcp_sf)
X(__fpu_frcp_sf):
	cmpqgt #0, r4
	fneg?f r4

//	mov #0x7FF, r5		| mov #0x3FF, r6
	mov #0x7FE, r5		| mov #0x3FF, r6
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r2, r7, r2

	fneg?f r2
	
	rtsu

.global X(__fpu_fdiv_sf)
X(__fpu_fdiv_sf):
	cmpqgt #0, r5
//	nop
	fneg?f r5
	mov		#0x4000000000000000, r1
//	mov		#0x7FF0000000000000, r3
	mov		#0x7FE0000000000000, r3
	mov		#0x3FF0000000000000, r6
	sub		r3, r5, r3
	fmula	r3, r5, r7
	sub		r6, r7, r7
	add		r7, r3, r2

	fmula	r2, r5, r6
	fsuba	r1, r6, r7
	fmula	r2, r7, r2

	fneg?f	r2
	fmul	r2, r4, r2	
	rtsu

.else

/*
	Stomps r2, r3, r5, r6, r7
 */
.global X(__fpu_frcp)
X(__fpu_frcp):
	mov #0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp
	fneg r2
	jmp r20
//	ret

	.L0:
//	mov r4, r3
//	mov r4, r6
	mov #0x7FF0000000000000, r5
//	sub r6, r5
	sub r4, r5
	mov #0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov #0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2
	
	rts

.global X(__fpu_frcp_s)
X(__fpu_frcp_s):
	mov #0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	jmp r20
//	ret

	.L0:
	mov #0x7FF0000000000000, r5
	sub r4, r5
	mov #0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov #0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

//	fmul r2, r5, r6
//	fsub r3, r6, r7
//	fmul r7, r2

//	fmul r2, r5, r6
//	fsub r3, r6, r7
//	fmul r7, r2
	
	rts

.global X(__fpu_fdiv)
X(__fpu_fdiv):
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp
	fmul r18, r2
	mov r16, lr
	rts

.global X(__fpu_fdiv_s)
X(__fpu_fdiv_s):
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_s
	fmul r18, r2
	mov r16, lr
	rts


.global X(__fpu_frcp_sf)
X(__fpu_frcp_sf):
	mov #0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	jmp r20
//	ret

	.L0:
	mov #0x7FF0000000000000, r5
	sub r4, r5
	mov #0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov #0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2
	
	rts

.global X(__fpu_fdiv_sf)
X(__fpu_fdiv_sf):
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_sf
	fmul r18, r2
	mov r16, lr
	rts

.endif

.else

/* DR4=rv, DR5=a */
.global X(__fpu_frcp_b0)
X(__fpu_frcp_b0):
	fmul dr4, dr5, dr7			//rp=rv*a
	fmov dr7, r7				//rpb=rp
	mov #0x3FF0000000000000, r6
	sub r7, r6

	fmov dr4, r5				//rvb=rv
	add r6, r5					
	fmov r5, dr4
//	break
	rts

#define FRCP_SEG					\
	fmul dr4, dr5, dr7;				\
	fmov dr7, r7;					\
	mov #0x3FF0000000000000, r6;	\
	sub r7, r6;						\
	fmov dr4, r5;					\
	add r6, r5;						\
	fmov r5, dr4

#define FRCP_SEG2					\
	fmov r5, dr4;					\
	fmul dr4, dr5, dr7;				\
	fmov dr7, r7;					\
	sub r6, r7, r7;					\
	add r7, r5;

#if 0
.global X(__fpu_frcp_b2)
X(__fpu_frcp_b2):
	mov #0x3FF0000000000000, r6
	fmov dr4, r5;

	FRCP_SEG2
	FRCP_SEG2
	FRCP_SEG2
	FRCP_SEG2
	fmov r5, dr4
	rts

.global X(__fpu_frcp_b3)
X(__fpu_frcp_b3):
	mov #0x3FF0000000000000, r6
	fmov dr4, r5;

	FRCP_SEG2
//	FRCP_SEG2
//	FRCP_SEG2
//	FRCP_SEG2
//	fmov r5, dr4
	fmov r5, dr2
//	fmov dr4, dr2
	
	mov #0x4000, r0
	fldch dr3

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
//	fmul dr2, dr7, dr2
	fmul dr7, dr2

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
//	fmul dr2, dr7, dr2
	fmul dr7, dr2

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
//	fmul dr2, dr7, dr2
	fmul dr7, dr2

//	fmov dr4, dr2
	
	rts
#endif

.global X(__fpu_frcp)
X(__fpu_frcp):
	mov #0x0000, r0
	fldch dr3
	fcmp/gt dr4, dr3
	bf .L0
//	push lr
	mov lr, r20
	fneg dr4
	bsr __fpu_frcp
	fneg dr2
	jmp r20
//	ret

	.L0:
	fmov dr4, dr5
	fmov dr4, r6
	mov #0x7FF0000000000000, r5
	sub r6, r5
	mov #0x3FF0000000000000, r6

	fmov r5, dr4
	fmul dr4, dr5, dr7
	fmov dr7, r7
	sub r6, r7, r7
	add r7, r5

//	fmov r5, dr4
//	fmul dr4, dr5, dr7
//	fmov dr7, r7
//	sub r6, r7, r7
//	add r7, r5

	fmov r5, dr2
	
	mov #0x4000, r0
	fldch dr3

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
	fmul dr7, dr2

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
	fmul dr7, dr2

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
	fmul dr7, dr2

	fmul dr2, dr5, dr6
	fsub dr3, dr6, dr7
	fmul dr7, dr2
	
	rts

.endif

.else

.global X(__fpu_frcp)
X(__fpu_frcp):
	rts

.endif

#endif

#if 0
.global X(strcmp)
X(strcmp):

	xor r2, r2

#if 1
.L0:
	mov.l @r4, r6
	mov.l @r5, r7
	cmpeq r6, r7
	bf .L1
.ifarch bjx2_wex3
	mov		0xFF00, r16		| tst #0xFF, r6
	shad	r16, 8, r17		| movt r3			| tst r16, r6
	shad	r17, 8, r16		| addc	r3, r3		| tst r17, r6
	addc	r3, r3								| tst r16, r6
	addc r3, r3
.else
	tst #0x000000FF, r6
	movt r3
	tst #0x0000FF00, r6
	addc r3, r3
	tst #0x00FF0000, r6
	addc r3, r3
	tst #0xFF000000, r6
	addc r3, r3
.endif
	
	tst r3, r3
	bf .L2
	
	add #4, r4
	add #4, r5
	bra .L0
#endif

#if 0
.L0:
	mov.q @r4, r6
	mov.q @r5, r7
	cmpqeq r6, r7
	bf .L1
.ifarch bjx2_wex3
	mov		0xFF00, r16				| tstq #0xFF, r6	//0000 0000 0000 00FF
	shad.q	r16, 8, r17	| movt r3		| tstq r16, r6	//0000 0000 0000 FF00
	shad.q	r17, 8, r16	| addc	r3, r3	| tstq r17, r6	//0000 0000 00FF 0000
	shad.q	r16, 8, r17	| addc	r3, r3	| tstq r16, r6	//0000 0000 FF00 0000
	shad.q	r17, 8, r16	| addc	r3, r3	| tstq r17, r6	//0000 00FF 0000 0000
	shad.q	r16, 8, r17	| addc	r3, r3	| tstq r16, r6	//0000 FF00 0000 0000
	shad.q	r17, 8, r16	| addc	r3, r3	| tstq r17, r6	//00FF 0000 0000 0000
	shad.q	r16, 8, r17	| addc	r3, r3	| tstq r16, r6	//FF00 0000 0000 0000
	addc r3, r3
.else
	mov		0xFF, r16
	shad.q	r16, 8, r17
	tstq	r16, r6
	movt r3
	shad.q	r17, 8, r16
	tstq r17, r6
	addc r3, r3
	shad.q	r16, 8, r17
	tstq r16, r6
	addc r3, r3
	shad.q	r17, 8, r16
	tstq r17, r6
	addc r3, r3
	shad.q	r16, 8, r17
	tstq r16, r6
	addc r3, r3
	shad.q	r17, 8, r16
	tstq r17, r6
	addc r3, r3
	shad.q	r16, 8, r17
	tstq r16, r6
	addc r3, r3
	tstq r17, r6
	addc r3, r3
.endif
	
	tst r3, r3
	bf .L2
	
	add #8, r4
	add #8, r5
	bra .L0
#endif

.L1:
	mov.b @r4, r6
	mov.b @r5, r7
	cmpeq r6, r7
	bf .L3
	tst r6, r6
	bt .L2
	add #1, r4
	add #1, r5
	bra .L1
.L3:
	cmpgt r7, r6
	mov?t #1, r2
	mov?f #-1, r2

.L2:
	rts
#endif

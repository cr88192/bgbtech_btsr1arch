// #define X(n)	_##n
#define X(n)	n
//#define STACK_BASE	0x0000DFFC
#define STACK_BASE_ISR	0x0000DFF0
// #define STACK_BASE_USR	0x0000DF00

#define STACK_BASE_USR	0x010FFFF0
#define STACK_BASE_SYS	0x0101FFF0

.section .data

__tk_saved_vbr: .quad 0 

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
	tst		r2, r2
	nop
	mov?t	r4, vbr
	mov?t	r5, sp
	mov?t	r6, ssp
	mov?t	r7, sr
	

//	break?f

//	bsr		copy_data

	bsr		zero_bss

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

isr_inter:
//	mov	exsr, r4
//	break
//	bsr __isr_interrupt
//	rte
	bra __isr_interrupt

isr_tlbfault:
	mov		__tk_saved_vbr, r3
	mov.q	@r3, r2
//	lea.b	@(r2, 24), r3
	add		r2, #24, r3
	jmp		r3

isr_syscall:
	mov sp, r16
	mov STACK_BASE_SYS, sp
	push r16
	push gbr
	push lr

//	break

	bsr X(tk_isr_syscall)
	
	pop lr
	pop gbr
	pop r16
	mov r16, sp

//	break
	
	rte

isr_table:
	bra8b _start
	bra8b isr_except
	bra8b isr_inter
	bra8b isr_tlbfault
	bra8b isr_syscall
//	bra8b X(__isr_syscall)


.global X(tk_getsavedvbr)
X(tk_getsavedvbr):
//	mov		__tk_saved_vbr, r3
//	mov.q	@r3, r2
	mov.q	__tk_saved_vbr, r2
	rts

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

/*
	umullq: Unsigned Multiply, Long to Quad
	smullq: Signed Multiply, Long to Quad
*/

.global X(__umullq)
X(__umullq):
	dmulu r4, r5
	mov dhr, r3
	mov dlr, r2
	shll32 r3
	add r3, r2
	rts

.global X(__smullq)
X(__smullq):
	dmuls r4, r5
	mov dhr, r3
	mov dlr, r2
	shll32 r3
	add r3, r2
	rts

/*
	umulsq: Unsigned Multiply, Single Quad
	smulsq: Signed Multiply, Single Quad
*/
	
.global X(__umulsq)
.global X(__smulsq)
X(__umulsq):
X(__smulsq):
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

/*
	umuldq: Unsigned Multiply, Double Quad
*/
.global X(__umuldq)
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

/*
	smuldq: Signed Multiply, Double Quad
*/
.global X(__smuldq)
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


.global X(__longj)
X(__longj):
//	mov r4, r1
	mov r5, r2

//	mov.q  @(16,r4), r0
//	lds r0, pr

	mov.q  @(r4, 0), r1
	mov r1, lr

	mov.q  @(r4, 8), r1
	mov r1, gbr

	mov.q  @(r4, 16), r1
	mov r1, sr

	mov.q  @(r4, 24), r1
	mov r1, ssp

//	mov		STACK_BASE_ISR, r0
//	mov		r0, ssp

	mov.q  @(r4, 32), r1
	mov r1, tbr

	mov.q  @(r4, 40), r1
	mov r1, ttb

	mov.q  @(r4, 48), r1
	mov r1, mmcr

	mov.q  @(r4, 56), r1
	mov r1, krr

	mov.q  @(r4, 128), r1
	mov r1, vbr

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

//	break


//	mov r2, r0
	rts
	nop

.global X(__setj)
X(__setj):

	mov.q  r0, @(  0,r4)
	mov.q  r1, @(  8,r4)
	mov.q  r2, @( 16,r4)
	mov.q  r3, @( 24,r4)
	mov.q  r4, @( 32,r4)
	mov.q  r5, @( 40,r4)
	mov.q  r6, @( 48,r4)
	mov.q  r7, @( 56,r4)
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
	
//	sts pr, r1
//	mov #16, r0
//	mov.q  r1, @(r0,r4)

	mov lr, r1
	mov.q  r1, @(r4, 0)

	mov gbr, r1
	mov.q  r1, @(r4, 8)

	mov sr, r1
	mov.q  r1, @(r4, 16)

	mov ssp, r1
	mov.q  r1, @(r4, 24)

	mov tbr, r1
	mov.q  r1, @(r4, 32)

	mov ttb, r1
	mov.q  r1, @(r4, 40)

	mov mmcr, r1
	mov.q  r1, @(r4, 48)

	mov krr, r1
	mov.q  r1, @(r4, 56)

	mov vbr, r1
	mov.q  r1, @(r4, 128)
	
//	mov #0, r0
	mov #0, r2
	rts
	nop


.global X(__va64_arg_i)
X(__va64_arg_i):
	mov.l @(r4, 96), r2
	mov #64, r3
	cmp/gt r2, r3
	bf __va64_arg_i.L0
	
	mov r2, r3
	add r4, r3
	mov.q @r3, r6
	add #8, r2
	mov.l  r2, @(r4, 96)
	mov r6, r2
	rts
	nop

__va64_arg_i.L0:
	mov.q @(r4, 112), r3
	mov.l @r3+, r6
	mov.q  r3, @(r4, 112)
	mov r6, r2
//	break
	rts
	nop

.global X(__va64_arg_l)
X(__va64_arg_l):
	mov.l @(r4, 96), r2
	mov #64, r3
	cmp/gt r2, r3
	bf __va64_arg_l.L0	

	mov r2, r3
	add r4, r3
	mov.q @r3, r6
	add #8, r2
	mov.l  r2, @(r4, 96)
	mov r6, r2
	rts
	nop
__va64_arg_l.L0:
	mov.q @(r4, 112), r3
	add #7, r3
	and #-8, r3
	mov.q @r3+, r6
	mov.q  r3, @(r4, 112)
	mov r6, r2
	rts
	nop

.global X(__va64_arg_x)
X(__va64_arg_x):
	mov.l @(r4, 96), r2
	mov #56, r3
	cmp/gt r2, r3
	bf __va64_arg_x.L0
	mov r2, r3
	add r4, r3
	mov.q @r3, r6
	add #8, r3
	mov.q @r3, r7
	add #8, r3
	mov.l  r3, @(r4, 96)
	mov r6, r2
	mov r7, r3
	rts
	nop
__va64_arg_x.L0:
	mov.q @(r4, 112), r3
	add #7, r3
	and #-8, r3
	mov.q @r3+, r6
	mov.q @r3+, r7
	mov.q  r3, @(r4, 112)
	mov r6, r2
	mov r7, r3
	rts
	nop

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

.endif

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
	cmp/hi r5, r3
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
	cmp/hi r5, r3
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
	cmp/hi r5, r3
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

.global X(memcpy)
X(memcpy):
.ifarch bjx2_wex
	cmp/ge	#32, r6
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
	add		#32, r4		| cmp/ge	#32, r6
	bt		.L0
	.L1:

	cmp/ge	#8, r6
	bf		.L3
	.L2:
	add		#-8, r6		| mov.q		(r5), r2
	add		#8, r5		| mov.q		r2, (r4)
	add		#8, r4		| cmp/ge	#8, r6
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
	cmp/ge #4, r6
	mov.l?t		(r5), r2
	mov.l?t		r2, (r4)
	add?t		#4, r4
	add?t		#4, r5
	add?t		#-4, r6

	cmp/ge #2, r6
	mov.w?t		(r5), r2
	mov.w?t		r2, (r4)
	add?t		#2, r4
	add?t		#2, r5
	add?t		#-2, r6

	cmp/ge #1, r6
	mov.b?t		(r5), r2
	mov.b?t		r2, (r4)
#endif
	rtsu


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
	mov #0x7FF, r5		| mov #0x3FF, r6
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
	mov #0x7FF, r5		| mov #0x3FF, r6
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

	mov #0x7FF, r5		| mov #0x3FF, r6
	shld.q r5, #52, r5	| mov #0x400, r3
	shld.q r6, #52, r6	| sub r5, r4, r5
	shld.q r3, #52, r3	| fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fneg?f r2
	
	rtsu

.global X(__fpu_fdiv_sf)
X(__fpu_fdiv_sf):
	cmpqgt #0, r5
//	nop
	fneg?f r5
	mov		#0x4000000000000000, r1
	mov		#0x7FF0000000000000, r3
	mov		#0x3FF0000000000000, r6
	sub		r3, r5, r3
	fmul	r3, r5, r7
	sub		r6, r7, r7
	add		r7, r3, r2

	fmul	r2, r5, r6
	fsub	r1, r6, r7
	fmul	r7, r2

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
	push lr
	fneg r4
	bsr __fpu_frcp
	fneg r2
	ret

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
	push lr
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	ret

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
	push lr
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	ret

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
	push lr
	fneg dr4
	bsr __fpu_frcp
	fneg dr2
	ret

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

#if 1
.global X(strcmp)
X(strcmp):

	xor r2, r2

.L0:
	mov.l @r4, r6
	mov.l @r5, r7
	cmpeq r6, r7
	bf .L1
	
	tst #0x000000FF, r6
	movt r3
	tst #0x0000FF00, r6
	addc r3, r3
	tst #0x00FF0000, r6
	addc r3, r3
	tst #0xFF000000, r6
	addc r3, r3
	
	tst r3, r3
	bf .L2
	
	add #4, r4
	add #4, r5
	bra .L0

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

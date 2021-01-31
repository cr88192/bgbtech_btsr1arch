// #define X(n)	_##n
#define X(n)	n
//#define STACK_BASE	0x0000DFFC
// #define STACK_BASE_ISR	0x0000DFC0
#define STACK_BASE_ISR	0x0000DFF0
// #define STACK_BASE_USR	0x0000DF00

// #define STACK_BASE_USR	0x010FFFF0
#define STACK_BASE_USR	0x010FFFC0

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
	mov STACK_BASE_USR, sp
	
	xor r1, r1
	mov r1, vbr
	mov r1, gbr
	mov r1, tbr
	mov r1, mmcr
//	mov r1, sr

	bsr tk_checksane_asm

	cpuid #1
	test #15, r0
	bf _iscore2

	bsr copy_data

	bsr zero_bss

	mov STACK_BASE_ISR, r0
	mov r0, ssp
//	mov isr_table, r0
//	mov r0, vbr

	bsr X(__start)
	bra _exit

_exit:
	break
	bra _exit
//	mov #-1, r0
//	jmp r0
//	nop

_iscore2:
	mov		core2_isr_table, r2
	mov		r2, vbr
	.L0:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bra .L0

#if 0
	mov #999999, r2
	.L0:
	nop
	nop
	nop
	add		#-1, r2
	cmpgt	#0, r2
	bt		.L0
	
	mov		#0xF00BFF00, r2
	mov.l	(r2, 0x24), r3
	test	r3, r3
	bf		.L1
	bra _iscore2
	.L1:
	jmp		r3
#endif

core2_isr_table:
	bra8b _iscore2
	bra8b _iscore2__isr
	bra8b _iscore2__isr
	bra8b _iscore2__isr

_iscore2__isr:
	mov		exsr, r7
	mov		0xF0FF, r4
	and		r7, r4, r6
	cmpeq	0x80AA, r6
	bt		_iscore2__isr_boot
	rte
	nop
_iscore2__isr_boot:
//	shad.q	r7, -16, r2
	mov		tea, r2
	test	r2, r2
	bt		_iscore2
	jmp		r2

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

#if 1
copy_data:
	mov __rom_data_start, r2
	mov __rom_data_end, r3
	mov __data_start, r5
.L1:
	mov.l @r2, r4
	mov.l r4, @r5
	add #4, r5
	add #4, r2
	cmp/hi r2, r3
	bt .L1
	rts
#endif

__isr_except:
	break
//	rte
	nop
	nop
	nop

__isr_inter:
	rte
	nop
	nop
	nop

#if 0
__isr_mmu:
	rte
	nop
	nop
	nop

__isr_syscall:
	rte
	nop
	nop
	nop

isr_table:
	bra8b _start
	bra8b __isr_except
	bra8b __isr_inter
#endif

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

#if 0
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
#endif


.global X(__longj)
X(__longj):
	mov r4, r1
	mov r5, r2

	mov.q  @(16,r4), r0
	lds r0, pr

	mov.q  @(  0,r4), r0
	mov.q  @(  8,r4), r1
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

//	mov r2, r0
	rts
	nop

.global X(__setj)
X(__setj):

	mov.q  r0,@(  0,r4)
	mov.q  r1,@(  8,r4)
	mov.q  r2,@( 16,r4)
	mov.q  r3,@( 24,r4)
	mov.q  r4,@( 32,r4)
	mov.q  r5,@( 40,r4)
	mov.q  r6,@( 48,r4)
	mov.q  r7,@( 56,r4)
	mov.q  r8,@( 64,r4)
	mov.q  r9,@( 72,r4)
	mov.q r10,@( 80,r4)
	mov.q r11,@( 88,r4)
	mov.q r12,@( 96,r4)
	mov.q r13,@(104,r4)
	mov.q r14,@(112,r4)
	mov.q r15,@(120,r4)

	mov.q r16,@(128,r4)
	mov.q r17,@(136,r4)
	mov.q r18,@(144,r4)
	mov.q r19,@(152,r4)
	mov.q r20,@(160,r4)
	mov.q r21,@(168,r4)
	mov.q r22,@(176,r4)
	mov.q r23,@(184,r4)
	mov.q r24,@(192,r4)
	mov.q r25,@(200,r4)
	mov.q r26,@(208,r4)
	mov.q r27,@(216,r4)
	mov.q r28,@(224,r4)
	mov.q r29,@(232,r4)
	mov.q r30,@(240,r4)
	mov.q r31,@(248,r4)
	
	sts pr,r1
	mov #16, r0
	mov.q  r1,@(r0,r4)
	
//	mov #0, r0
	mov #0, r2
	rts
	nop


.global X(__va64_arg_i)
X(__va64_arg_i):
	mov.l @(r4, 96), r2
	mov #64, r1
	cmp/gt r2, r1
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
	cmp/gt r0, r3
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

#if 0
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
	mov r2, r3
	mov.l  r3, @(r4, 104)
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
	mov #-8, r3
	and r3, r2
	
	mov #96, r3
	cmp/gt r2, r3
	bf __va64_arg_d.L0

	mov r4, r3
	add r2, r3
	fmov.d @r3, fr2

	add #8, r2
	mov r2, r3
	mov.l  r3, @(r4, 104)
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
	fldch fr2
	rts

.global X(__sthf16)
X(__sthf16):
	fstch fr4
	mov dlr, r2
	rts
#endif

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


.ifarch bjx2_movx
memcpy_movx:
	CMP/GE	32, R6
	BF		.L1
	.L0:
	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22
	ADD		32, R5		|	ADD		-32, R6
	MOV.X	R20, (R4,  0)
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMP/GE	32, R6
	BT		.L0
	.L1:

	CMP/GE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5		|	MOV.Q	R20, (R4,  0)
	ADD		8, R4		|	CMP/GE	8, R6
	BT		.L2
	.L3:

#if 1
	TEST	R6, R6
	RTSU?T
//	BT		.L5

	CMP/GE		4, R6
	MOV.L?T		(R5), R2
	MOV.L?T		R2, (R4)
	ADD?T		4, R4
	ADD?T		4, R5
	ADD?T		-4, R6

	CMP/GE		2, R6
	MOV.W?T		(R5), R2
	MOV.W?T		R2, (R4)
	ADD?T		2, R4
	ADD?T		2, R5
	ADD?T		-2, R6

	CMP/GE		1, R6
	MOV.B?T		(R5), R2
	MOV.B?T		R2, (R4)

//	.L5:
#endif

	RTSU
.endif

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

.global X(__do_ldtlb)
X(__do_ldtlb):
	mov r4, r0
	mov r5, r1
	ldtlb
	rts

.global X(__do_rethrow_fault)
X(__do_rethrow_fault):
	mov STACK_BASE_ISR, sp
#if 0
//	sub #128, sp
//	pop r0
//	pop r1
//	pop r2
//	pop r3
//	pop r4
//	pop r5
//	pop r6
//	pop r7
//	pop r16
//	pop r17
//	pop r18
//	pop r19
//	pop r20
//	pop r21
//	pop r22
//	pop r23
#endif

	sub 	128, sp
	mov.q	(sp,   0), r0
	mov.q	(sp,   8), r1
	mov.q	(sp,  16), r2
	mov.q	(sp,  24), r3
	mov.q	(sp,  32), r4
	mov.q	(sp,  40), r5
	mov.q	(sp,  48), r6
	mov.q	(sp,  56), r7
	mov.q	(sp,  64), r16
	mov.q	(sp,  72), r17
	mov.q	(sp,  80), r18
	mov.q	(sp,  88), r19
	mov.q	(sp,  96), r20
	mov.q	(sp, 104), r21
	mov.q	(sp, 112), r22
	mov.q	(sp, 120), r23
	add		128, sp

	mov vbr, r2
	add #8, r2
	jmp r2

#include "tk_divi2.S"

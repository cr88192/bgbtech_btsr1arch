// #define X(n)	_##n
#define X(n)	n
//#define STACK_BASE	0x0000DFFC
#define STACK_BASE_ISR	0x0000DFF0
// #define STACK_BASE_USR	0x0000DF00

#define STACK_BASE_USR	0x010FFFF0

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

__isr_except:
	break
//	rte

__isr_inter:
	rte

__isr_mmu:
	rte

__isr_syscall:
	rte

isr_table:
	bra8b _start
	bra8b __isr_except
	bra8b __isr_inter

.global X(__umulsq)
.global X(__smulsq)
X(__umulsq):
X(__smulsq):
	mov r4, r6
	mov r5, r7
	shlr32 r6
	shlr32 r7
	
	dmulu r4, r5
	mov dhr, r1
	mov dlr, r0

	dmulu r4, r7
	add dlr, r1
	dmulu r6, r5
	add dlr, r1

//	break

	shll32 r1
	add r1, r0

//	break

	rts


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

	mov r2, r0
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
	
	mov #0, r0
	rts
	nop


.global X(__va64_arg_i)
X(__va64_arg_i):
	mov #96, r0
	mov.l @(r4, r0), r0
	mov #64, r1
	cmp/gt r0, r1
	bf __va64_arg_i.L0
	
	mov.q @(r4, r0), r6
	add #8, r0
//	mov.l  r0, @(r4, 96)
	mov r0, r1
	mov #96, r0
	mov.l  r1, @(r4, r0)
	mov r6, r0
	rts
	nop
__va64_arg_i.L0:
	mov #112, r0
	mov.q @(r4, r0), r1
	mov.l @r1+, r6
	mov.q  r1, @(r4, r0)
	mov r6, r0
	rts
	nop

.global X(__va64_arg_l)
X(__va64_arg_l):
	mov #96, r0
	mov.l @(r4, r0), r0
	mov #64, r1
	cmp/gt r0, r1
	bf __va64_arg_l.L0	
	mov.q @(r4, r0), r6
	add #8, r0
	mov r0, r1
	mov #96, r0
	mov.l  r1, @(r4, r0)
	mov r6, r0
	rts
	nop
__va64_arg_l.L0:
	mov #112, r0
	mov.q @(r4, r0), r1
	add #7, r1
	mov #-8, r0
	and r0, r1
	mov.q @r1+, r6
	mov.q  r1, @(r4, r0)
	mov r6, r0
	rts
	nop

.global X(__va64_arg_x)
X(__va64_arg_x):
	mov #96, r0
	mov.l @(r4, r0), r0
	mov #56, r1
	cmp/gt r0, r1
	bf __va64_arg_x.L0
	mov.q @(r4, r0), r6
	add #8, r0
	mov.q @(r4, r0), r7
	add #8, r0
	mov r0, r1
	mov #96, r0
	mov.l  r1, @(r4, r0)
	mov r6, r0
	mov r7, r1
	rts
	nop
__va64_arg_x.L0:
	mov #112, r0
	mov.q @(r4, r0), r1
	add #7, r1
	mov #-8, r0
	and r0, r1
	mov.q @r1+, r6
	mov.q @r1+, r7
	mov.q  r1, @(r4, r0)
	mov r6, r0
	mov r7, r1
	rts
	nop

.global X(__va64_arg_f)
X(__va64_arg_f):
	mov #104, r0
	mov.l @(r4, r0), r0
	mov #96, r1
	cmp/gt r0, r1
	bf __va64_arg_f.L0

	fmov.d @(r4, r0), fr0
	add #8, r0
	mov r0, r1
	mov #104, r0
	mov.l  r1, @(r4, r0)
	rts
	nop
__va64_arg_f.L0:
	mov #112, r0
	mov.q @(r4, r0), r1
	fmov.s @r1, fr0
	add #4, r1
	mov.q  r1, @(r4, r0)
	rts
	nop

.global X(__va64_arg_d)
X(__va64_arg_d):
	mov #104, r0
	mov.l @(r4, r0), r0
	
	add #7, r0
	mov #-8, r1
	and r1, r0
	
	mov #96, r1
	cmp/gt r0, r1
	bf __va64_arg_d.L0

	add r4, r0
	fmov.d @r0, fr0
	add #8, r0
	sub r4, r0
	mov r0, r1
	mov #104, r0
	mov.l  r1, @(r4, r0)
	rts
	nop
__va64_arg_d.L0:
	mov #112, r0
	mov.q @(r4, r0), r1
	fmov.d @r1, fr1
	add #8, r1
	mov.q  r1, @(r4, r0)
	rts
	nop

.global X(__ldhf16)
X(__ldhf16):
	mov r4, dlr
	fldch fr0
	rts

.global X(__sthf16)
X(__sthf16):
	fstch fr4
	mov dlr, r0
	rts

.global X(__memcpy32)
X(__memcpy32):
	mov.q @(r5,  0), r0
	mov.q @(r5,  8), r1
	mov.q @(r5, 16), r2
	mov.q @(r5, 24), r3
	mov.q r0, @(r4,  0)
	mov.q r1, @(r4,  8)
	mov.q r2, @(r4, 16)
	mov.q r3, @(r4, 24)
	rts

.global X(__memcpy8_16)
X(__memcpy8_16):
	mov.q @(r5, 0), r0
	mov.q @(r5, 8), r1
	mov.q r0, @(r4, 0)
	mov.q r1, @(r4, 8)
	rts

.global X(sleep_0)
X(sleep_0):
	sleep
	rts


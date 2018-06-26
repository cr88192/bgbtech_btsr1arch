#ifdef ARCH_SH4
#define X(n)	n
#define STACK_BASE	0x8FFFFFF8
#else
#define X(n)	_##n
#define STACK_BASE	0x00003FFC
#endif

.section .bss
.global X(timer_ticks)
X(timer_ticks): .long 0

.section .text
.align 2
.global _start
.global start
.global _vector_table
.extern X(__start)
.extern X(timer_ticks)

.extern X(__bss_start)
.extern X(_end)

start:
_start:

	mov STACK_BASE, sp

	bsr zero_bss
	nop

	bsr X(__start)


	bra _exit
	nop

zero_bss:
	mov.l bss_strt, r2
	mov.l bss_end, r3
#ifdef __BJX1_64__
	extu.l r2
	extu.l r3
#endif
	mov #0, r4
.L0:
	mov.l r4, @-r3
//	cmp/hs r2, r3
	cmp/hi r2, r3
	bt .L0
	nop
	rts
	nop
	bra _exit

// .align 4
// bss_strt: .long __bss_start
// bss_end: .long _end
// stack_base: .long STACK_BASE


_loop:
	bra _loop
	nop

_exit:
	mov #-1, r0
	jsr @r0
	nop

_gdb_unhandled_isr:
	mov.l start1_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_nmi_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _loop
	nop

_gdb_illegalinst_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_addresserr_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_ihandler_emac:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _loop
	nop

_gdb_trapa32_isr:
	rte
	nop

_gdb_trapa33_isr:
	rte
	nop

_gdb_trapa34_isr:
	rte
	nop

_gdb_pit_isr:
	push r0
	push r1
//	mov.l ticks, r0
	mov timer_ticks, r0
	mov.l @r0, r1
	add #1, r1
	mov.l r1, @r0
	pop r1
	pop r0
	rte
	nop

// .align 4
// ticks: .long X(timer_ticks)

_gdb_ihandler_uart1:
	rte
	nop
_gdb_ihandler_uart0:
	rte
	nop
_gdb_ihandler_uart2:
	rte
	nop

_gdb_ihandler_inttmr:
	rte
	nop

.align 4
// pio_addr:	 .long 0xABCD0000
// start_leds:   .long 0x000000ff
// start1_leds:   .long 0x0000004f
// start2_leds:   .long 0x0000004e

//.long 0x5A5A5A5A
//a_tk_main: .long X(__start)
//.long 0x5B5B5B5B
//isr_table: .long _vector_table
//.long 0x5C5C5C5C

.global X(sleep_0)
X(sleep_0):
	sleep
	rts

#if 0
.global X(__udivsi3_i4i)
.global X(__udivsi3)
.global X(__udivdi3)
X(__udivsi3_i4i):
X(__udivsi3):
X(__udivdi3):
	sts pr,r1
	mov.l r4,@-r15
	extu.w r5,r0
	cmp/eq r5,r0
	swap.w r4,r0
	shlr16 r4
	bf/s udiv_lgdiv
	div0u
	mov.l r5,@-r15
	shll16 r5
sdiv_smdiv:
	div1 r5,r4
	bsr div6
	div1 r5,r4
	div1 r5,r4
	bsr div6
	div1 r5,r4
	xtrct r4,r0
	xtrct r0,r4
	bsr div7
	swap.w r4,r4
	div1 r5,r4
	bsr div7
	div1 r5,r4
	xtrct r4,r0
	mov.l @r15+,r5
	swap.w r0,r0
	mov.l @r15+,r4
	jmp @r1
	rotcl r0
div7:
	div1 r5,r4
div6:
	div1 r5,r4
	div1 r5,r4
	div1 r5,r4
	div1 r5,r4
	div1 r5,r4
	rts
	div1 r5,r4
divx3:
	rotcl r0
	div1 r5,r4
	rotcl r0
	div1 r5,r4
	rotcl r0
	rts
	div1 r5,r4

udiv_lgdiv:
	mov.l r5,@-r15
sdiv_lgdiv:
	xor r4,r0
	.rept 4
	rotcl r0
	bsr divx3
	div1 r5,r4
	.endr
	mov.l @r15+,r5
	mov.l @r15+,r4
	jmp @r1
	rotcl r0

.global	X(__sdivsi3_i4i)
.global X(__sdivsi3_i4)
.global X(__sdivsi3)
X(__sdivsi3_i4i):
X(__sdivsi3_i4):
X(__sdivsi3):
	mov.l r4,@-r15
	cmp/pz r5
	mov.l r5,@-r15
	bt/s sdiv_pos_divisor
	cmp/pz r4
	neg r5,r5
	extu.w r5,r0
	bt/s sdiv_neg_result
	cmp/eq r5,r0
	neg r4,r4
sdiv_pos_result:
	swap.w r4,r0
	bra sdiv_chkdiv
	sts pr,r1
sdiv_pos_divisor:
	extu.w r5,r0
	bt/s sdiv_pos_result
	cmp/eq r5,r0
	neg r4,r4
sdiv_neg_result:
	mova sdiv_res_neg,r0
	mov r0,r1
	swap.w r4,r0
	lds r2,macl
	sts pr,r2
sdiv_chkdiv:
	shlr16 r4
	bf/s sdiv_lgdiv
	div0u
	bra sdiv_smdiv
	shll16 r5
	.balign 4
sdiv_res_neg:
	neg r0,r0
	jmp @r2
	sts macl,r2
#endif

#ifdef ARCH_SH4


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
#endif


.global X(__va64_arg_i)
X(__va64_arg_i):
	isetmd.dq
	mov #96, r0
	mov.l @(r4, r0), r0
//#ifdef __BJX1_64E__
.ifarch bjx1_egpr
	mov #64, r1
.else
//#else
	mov #32, r1
.endif
//#endif
//	cmp/ge r1, r0
//	bt __va64_arg_i.L0
	cmp/gt r0, r1
	bf __va64_arg_i.L0
	
//	add r4, r0
//	mov.q @r0, r6
	
	mov.q @(r4, r0), r6
	add #8, r0
//	mov.l  r0, @(r4, 96)
	mov r0, r1
	mov #96, r0
	mov.l  r1, @(r4, r0)

//	brk

	mov r6, r0
	rts
	nop
__va64_arg_i.L0:
	isetmd.dq
	mov #112, r0
	mov.q @(r4, r0), r1
	mov.l @r1+, r6
//	mov.q @r1+, r6
	mov.q  r1, @(r4, r0)
	mov r6, r0

//	brk

	rts
	nop

.global X(__va64_arg_l)
X(__va64_arg_l):
	isetmd.dq
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
	isetmd.dq
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
	isetmd.dq
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
	isetmd.dq
	mov #112, r0
	mov.q @(r4, r0), r1
	fmov.s @r1+, fr0
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
	fmov.s @r1+, fr1
	fmov.s @r1+, fr0
	mov.q  r1, @(r4, r0)
	rts
	nop
#endif

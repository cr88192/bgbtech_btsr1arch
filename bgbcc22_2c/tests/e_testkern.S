// #ifdef ARCH_SH4
#if 1
#define X(n)	n
#define STACK_BASE	0x8FFFFFFC
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
	nop
	mov.l start_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1

	mov.l stack_base, sp

	bsr zero_bss
	nop

	mov.l isr_table, r0
	ldc r0, vbr
	
	mov #0, r0
	ldc r0, sr

	mov.l a_tk_main, r0
	jsr @r0
	nop

//	bra _loop
	bra _exit
	nop

zero_bss:
	mov.l bss_strt, r2
	mov.l bss_end, r3
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

.align 4
bss_strt: .long __bss_start
bss_end: .long _end
stack_base: .long STACK_BASE


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
	mov.l r0, @-sp
	mov.l r1, @-sp
	mov.l ticks, r0
	mov.l @r0, r1
	add #1, r1
	mov.l r1, @r0
	mov.l @sp+, r1
	mov.l @sp+, r0
	rte
	nop

.align 4
ticks: .long X(timer_ticks)

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
pio_addr:	 .long 0xABCD0000
start_leds:   .long 0x000000ff
start1_leds:   .long 0x0000004f
start2_leds:   .long 0x0000004e

.long 0x5A5A5A5A
a_tk_main: .long X(__start)
.long 0x5B5B5B5B
isr_table: .long _vector_table
.long 0x5C5C5C5C

.global X(sleep_0)
X(sleep_0):
	sleep
	rts

#if 1
.global X(__udivsi3_i4i)
X(__udivsi3_i4i):
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

.global X(__sremsi3)
X(__sremsi3):
	sts.l pr, @-r15
	mov.l r4,@-r15
	mov.l r5,@-r15
	bsr __sdivsi3
	nop
	mov.l @r15+, r5
	mov.l @r15+, r4
	dmuls.l r0, r5
	sts macl,r5
	mov r4, r0
	sub r5, r0
	lds.l @r15+, pr
	rts
	nop
	

#endif

#if 1
.global X(__va_start_1)
X(__va_start_1):
	mov.l  r5, @(r4,  4)
	mov.l  r6, @(r4,  8)
	mov.l  r7, @(r4, 12)
	mov.l fr4, @(r4, 16)
	mov.l fr5, @(r4, 20)
	mov.l fr6, @(r4, 24)
	mov.l fr7, @(r4, 28)
	mov.l fr8, @(r4, 32)
	mov.l fr9, @(r4, 36)
	mov.l fr10, @(r4, 40)
	mov.l fr11, @(r4, 44)
	
	mov #4, r0
	mov.l  r0, @(r4,  48)
	mov #0, r0
	mov.l  r0, @(r4,  52)

	rts
	nop

.global X(__va_arg_i)
X(__va_arg_i):
	mov.l @(r4, 48), r0
	mov #16, r1
	cmp/ge r1, r0
	bt __va_arg_i.L0
	
	mov.l @(r4, r0), r6
	add #4, r0
	mov.l  r0, @(r4,  48)
	mov r6, r0
	rts
//	bra _exit
	nop
__va_arg_i.L0:
	mov.l @(r4, 56), r0
	mov.l @r0+, r6
	mov.l  r0, @(r4, 56)
	mov r6, r0
	rts
	nop

.global X(__va_arg_l)
X(__va_arg_l):
	mov.l @(r4, 48), r0
	mov #12, r1
	cmp/ge r1, r0
	bt __va_arg_l.L0

	mov.l @(r4, r0), r6
	add #4, r0
	mov.l @(r4, r0), r7
	add #4, r0
	mov.l  r0, @(r4, 48)
	mov r6, r0
	mov r7, r1
	rts
	nop
__va_arg_l.L0:
	mov.l @(r4, 56), r0
	mov.l @r0+, r6
	mov.l @r0+, r7
	mov.l  r0, @(r4, 56)
	mov r6, r0
	mov r7, r1
	rts
	nop

.global X(__va_arg_f)
X(__va_arg_f):
	mov.l @(r4, 52), r0
	mov #48, r1
	cmp/ge r1, r0
	bt __va_arg_f.L0

	fmov.s @(r4, r0), fr0
	add #4, r0
	mov.l  r0, @(r4, 52)
	rts
	nop
__va_arg_f.L0:
	mov.l @(r4, 56), r0
	fmov.s @r0+, fr0
	mov.l  r0, @(r4, 56)
	rts
	nop

.global X(__va_arg_d)
X(__va_arg_d):
	mov.l @(r4, 52), r0
	
	add #7, r0
	and #0xF8, r0
	
	mov #48, r1
	cmp/ge r1, r0
	bt __va_arg_d.L0

	add r4, r0
	fmov.s @r0+, fr0
	fmov.s @r0+, fr1
	sub r4, r0
	mov.l  r0, @(r4, 52)
	rts
	nop
__va_arg_d.L0:
	mov.l @(r4, 56), r0
	fmov.s @r0+, fr0
	fmov.s @r0+, fr1
	mov.l  r0, @(r4, 56)
	rts
	nop

#endif

#ifdef ARCH_SH4
.global	X(__sqrt_d)
X(__sqrt_d):
	fmov dr4, dr0
	fsqrt dr0
	rts
	nop

.global	X(__tk_trapdebug)
X(__tk_trapdebug):
	rts
	fmov fr15, fr15

#endif

#if 1
.global	X(__movmem_i4_even)
.global	X(__movmem_i4_odd)

X(__movmem_i4_even):
	mov.l	@r5+,r0
	bra	L_movmem_start_even
	mov.l	@r5+,r1

X(__movmem_i4_odd):
	mov.l	@r5+,r1
	add	#-4,r4
	mov.l	@r5+,r2
	mov.l	@r5+,r3
	mov.l	r1,@(4,r4)
	mov.l	r2,@(8,r4)

L_movmem_loop:
	mov.l	r3,@(12,r4)
	dt	r6
	mov.l	@r5+,r0
	bt/s	L_movmem_2mod4_end
	mov.l	@r5+,r1
	add	#16,r4
L_movmem_start_even:
	mov.l	@r5+,r2
	mov.l	@r5+,r3
	mov.l	r0,@r4
	dt	r6
	mov.l	r1,@(4,r4)
	bf/s	L_movmem_loop
	mov.l	r2,@(8,r4)
	rts
	mov.l	r3,@(12,r4)

L_movmem_2mod4_end:
	mov.l	r0,@(16,r4)
	rts
	mov.l	r1,@(20,r4)
#endif

.global X(__longj)
X(__longj):
	mov r4, r1
	mov r5, r2
	add #64, r1

//	mov.l  @(64,r4), r0
	mov.l  @(0,r1), r0
	lds r0, pr

	mov.l  @( 0,r4), r0
	mov.l  @( 4,r4), r1
//	mov.l  @( 8,r4), r2
	mov.l  @(12,r4), r3
//	mov.l  @(16,r4), r4
	mov.l  @(20,r4), r5
	mov.l  @(24,r4), r6
	mov.l  @(28,r4), r7
	mov.l  @(32,r4), r8
	mov.l  @(36,r4), r9
	mov.l  @(40,r4), r10
	mov.l  @(44,r4), r11
	mov.l  @(48,r4), r12
	mov.l  @(52,r4), r13
	mov.l  @(56,r4), r14
	mov.l  @(60,r4), r15
	
	mov r2, r0
	rts
	nop

.global X(__setj)
X(__setj):
	mov.l  r0,@( 0,r4)
	mov.l  r1,@( 4,r4)
	mov.l  r2,@( 8,r4)
	mov.l  r3,@(12,r4)
	mov.l  r4,@(16,r4)
	mov.l  r5,@(20,r4)
	mov.l  r6,@(24,r4)
	mov.l  r7,@(28,r4)
	mov.l  r8,@(32,r4)
	mov.l  r9,@(36,r4)
	mov.l r10,@(40,r4)
	mov.l r11,@(44,r4)
	mov.l r12,@(48,r4)
	mov.l r13,@(52,r4)
	mov.l r14,@(56,r4)
	mov.l r15,@(60,r4)
	
	mov r4, r1
	add #64, r1
	
	sts pr,r0
//	mov.l  r0,@(64,r4)
	mov.l  r0,@(0,r1)
	
	mov #0, r0
	rts
	nop

#ifdef ARCH_SH2

.global X(__ashrsi3)
X(__ashrsi3):
	mov r5, r0
.global X(__ashrsi3_r0)
X(__ashrsi3_r0):
	mov.l r4, @-sp
	not r0, r0
	and	#31,r0
	shal r0
	mov r0, r4
	mova shar_list, r0
	add r0, r4
	mov.l @sp+, r0
	jmp @r4
	nop

/* .global X(__ashrsi3)
X(__ashrsi3):
	not r5, r5
	mov	#31,r0
	and	r0,r5
	shal r5
	mova shar_list,r0
	add r0, r5
	mov r4, r0
	jmp @r5
	nop
	*/
.align 2
shar_list:
	shar	r0	/* 31 */
	shar	r0	/* 30 */
	shar	r0	/* 29 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 25 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 21 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 17 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 13 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 9 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 5 */
	shar	r0
	shar	r0
	shar	r0
	shar	r0	/* 1 */
	rts
	nop

.global X(__lshrsi3)
X(__lshrsi3):
	mov r5, r0
.global X(__lshrsi3_r0)
X(__lshrsi3_r0):
	mov.l r4, @-sp
	not r0, r0
	and	#31,r0
	shal r0
	mov r0, r4
	mova shlr_list, r0
	add r0, r4
	mov.l @sp+, r0
	jmp @r4
	nop
.align 2
shlr_list:
	shlr	r0	/* 31 */
	shlr	r0	/* 30 */
	shlr	r0	/* 29 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 25 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 21 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 17 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 13 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 9 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 5 */
	shlr	r0
	shlr	r0
	shlr	r0
	shlr	r0	/* 1 */
	rts
	nop

.global X(__ashlsi3)
X(__ashlsi3):
	mov r5, r0
.global X(__ashlsi3_r0)
X(__ashlsi3_r0):
	mov.l r4, @-sp
	not r0, r0
	and	#31,r0
	shal r0
	mov r0, r4
	mova shll_list,r0
	add r0, r4
//	mov r4, r0
	mov.l @sp+, r0
	jmp @r4
	nop
.align 2
shll_list:
	shll	r0	/* 31 */
	shll	r0	/* 30 */
	shll	r0	/* 29 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 25 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 21 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 17 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 13 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 9 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 5 */
	shll	r0
	shll	r0
	shll	r0
	shll	r0	/* 1 */
	rts
	nop

.global X(__ashiftrt_r4_31)
.global X(__ashiftrt_r4_30)
.global X(__ashiftrt_r4_29)
.global X(__ashiftrt_r4_28)
.global X(__ashiftrt_r4_27)
.global X(__ashiftrt_r4_26)
.global X(__ashiftrt_r4_25)
.global X(__ashiftrt_r4_24)
.global X(__ashiftrt_r4_23)
.global X(__ashiftrt_r4_22)
.global X(__ashiftrt_r4_21)
.global X(__ashiftrt_r4_20)
.global X(__ashiftrt_r4_19)
.global X(__ashiftrt_r4_18)
.global X(__ashiftrt_r4_17)
.global X(__ashiftrt_r4_16)
.global X(__ashiftrt_r4_15)
.global X(__ashiftrt_r4_14)
.global X(__ashiftrt_r4_13)
.global X(__ashiftrt_r4_12)
.global X(__ashiftrt_r4_11)
.global X(__ashiftrt_r4_10)
.global X(__ashiftrt_r4_9)
.global X(__ashiftrt_r4_8)
.global X(__ashiftrt_r4_7)
.global X(__ashiftrt_r4_6)
.global X(__ashiftrt_r4_5)
.global X(__ashiftrt_r4_4)
.global X(__ashiftrt_r4_3)
.global X(__ashiftrt_r4_2)
.global X(__ashiftrt_r4_1)
.global X(__ashiftrt_r4_0)

X(__ashiftrt_r4_31):	shar r4
X(__ashiftrt_r4_30):	shar r4
X(__ashiftrt_r4_29):	shar r4
X(__ashiftrt_r4_28):	shar r4
X(__ashiftrt_r4_27):	shar r4
X(__ashiftrt_r4_26):	shar r4
X(__ashiftrt_r4_25):	shar r4
X(__ashiftrt_r4_24):	shar r4
X(__ashiftrt_r4_23):	shar r4
X(__ashiftrt_r4_22):	shar r4
X(__ashiftrt_r4_21):	shar r4
X(__ashiftrt_r4_20):	shar r4
X(__ashiftrt_r4_19):	shar r4
X(__ashiftrt_r4_18):	shar r4
X(__ashiftrt_r4_17):	shar r4
X(__ashiftrt_r4_16):	shar r4
X(__ashiftrt_r4_15):	shar r4
X(__ashiftrt_r4_14):	shar r4
X(__ashiftrt_r4_13):	shar r4
X(__ashiftrt_r4_12):	shar r4
X(__ashiftrt_r4_11):	shar r4
X(__ashiftrt_r4_10):	shar r4
X(__ashiftrt_r4_9):		shar r4
X(__ashiftrt_r4_8):		shar r4
X(__ashiftrt_r4_7):		shar r4
X(__ashiftrt_r4_6):		shar r4
X(__ashiftrt_r4_5):		shar r4
X(__ashiftrt_r4_4):		shar r4
X(__ashiftrt_r4_3):		shar r4
X(__ashiftrt_r4_2):		shar r4
X(__ashiftrt_r4_1):		shar r4
X(__ashiftrt_r4_0):		rts
						nop

#endif

#if 1
.global X(__memcpy32_4)
.global X(__memcpy32_8)
.global X(__memcpy32_12)
.global X(__memcpy32_16)
.global X(__memcpy32_20)
.global X(__memcpy32_24)
.global X(__memcpy32_28)
.global X(__memcpy32_32)
.global X(__memcpy32_36)
.global X(__memcpy32_40)
.global X(__memcpy32_44)
.global X(__memcpy32_48)
.global X(__memcpy32_52)
.global X(__memcpy32_56)
.global X(__memcpy32_60)
.global X(__memcpy32_64)

.global X(__memcpy32)

X(__memcpy32_4):
	mov.l @r5, r0
	mov.l r0, @r4
	rts
	nop
X(__memcpy32_8):
	mov.l @(r5,0), r0
	mov.l @(r5,4), r1
	mov.l r0, @(r4, 0)
	mov.l r1, @(r4, 4)
	rts
	nop

X(__memcpy32_12):
	mov.l @(r5,0), r0
	mov.l @(r5,4), r1
	mov.l @(r5,8), r2
	mov.l r0, @(r4, 0)
	mov.l r1, @(r4, 4)
	mov.l r2, @(r4, 8)
	rts
	nop

X(__memcpy32_16):
	mov.l @(r5, 0), r0
	mov.l @(r5, 4), r1
	mov.l @(r5, 8), r2
	mov.l @(r5,12), r3
	mov.l r0, @(r4,  0)
	mov.l r1, @(r4,  4)
	mov.l r2, @(r4,  8)
	mov.l r3, @(r4, 12)
	rts
	nop

X(__memcpy32_20):
	mov.l @(r5, 0), r0
	mov.l @(r5, 4), r1
	mov.l @(r5, 8), r2
	mov.l @(r5,12), r3
	mov.l r0, @(r4,  0)
	mov.l r1, @(r4,  4)
	mov.l r2, @(r4,  8)
	mov.l r3, @(r4, 12)
	mov.l @(r5, 16), r0
	mov.l r0, @(r4, 16)
	rts
	nop
X(__memcpy32_24):
	mov.l @(r5, 0), r0
	mov.l @(r5, 4), r1
	mov.l @(r5, 8), r2
	mov.l @(r5,12), r3
	mov.l r0, @(r4,  0)
	mov.l r1, @(r4,  4)
	mov.l r2, @(r4,  8)
	mov.l r3, @(r4, 12)
	mov.l @(r5, 16), r0
	mov.l @(r5, 20), r1
	mov.l r0, @(r4, 16)
	mov.l r1, @(r4, 20)
	rts
	nop

X(__memcpy32_28):
	mov.l @(r5, 0), r0
	mov.l @(r5, 4), r1
	mov.l @(r5, 8), r2
	mov.l @(r5,12), r3
	mov.l r0, @(r4,  0)
	mov.l r1, @(r4,  4)
	mov.l r2, @(r4,  8)
	mov.l r3, @(r4, 12)
	mov.l @(r5, 16), r0
	mov.l @(r5, 20), r1
	mov.l @(r5, 24), r2
	mov.l r0, @(r4, 16)
	mov.l r1, @(r4, 20)
	mov.l r2, @(r4, 24)
	rts
	nop

X(__memcpy32_32):
	mov.l @(r5,  0), r0
	mov.l @(r5,  4), r1
	mov.l @(r5,  8), r2
	mov.l @(r5, 12), r3
	mov.l r0, @(r4,  0)
	mov.l r1, @(r4,  4)
	mov.l r2, @(r4,  8)
	mov.l r3, @(r4, 12)
	mov.l @(r5, 16), r0
	mov.l @(r5, 20), r1
	mov.l @(r5, 24), r2
	mov.l @(r5, 28), r3
	mov.l r0, @(r4, 16)
	mov.l r1, @(r4, 20)
	mov.l r2, @(r4, 24)
	mov.l r3, @(r4, 28)
	rts
	nop

X(__memcpy32_36):
	sts.l pr, @-r15
	bsr __memcpy32_16
	nop
	mov 16, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_20
	lds.l @r15+, pr
X(__memcpy32_40):
	sts.l pr, @-r15
	bsr __memcpy32_16
	nop
	mov 16, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_24
	lds.l @r15+, pr
X(__memcpy32_44):
	sts.l pr, @-r15
	bsr __memcpy32_16
	nop
	mov 16, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_28
	lds.l @r15+, pr
X(__memcpy32_48):
	sts.l pr, @-r15
	bsr __memcpy32_16
	nop
	mov 16, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_32
	lds.l @r15+, pr
X(__memcpy32_52):
	sts.l pr, @-r15
	bsr __memcpy32_20
	nop
	mov 20, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_32
	lds.l @r15+, pr
X(__memcpy32_56):
	sts.l pr, @-r15
	bsr __memcpy32_24
	nop
	mov 24, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_32
	lds.l @r15+, pr
X(__memcpy32_60):
	sts.l pr, @-r15
	bsr __memcpy32_28
	nop
	mov 28, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_32
	lds.l @r15+, pr
X(__memcpy32_64):
	sts.l pr, @-r15
	bsr __memcpy32_20
	nop
	mov 32, r0
	add r0, r4
	add r0, r5
	bra __memcpy32_32
	lds.l @r15+, pr

/* Memory copy for 32-bit aligned data in 32-bit words. */
X(__memcpy32):
	sts.l pr, @-r15

	add r4, r6

	/* Copy 64-byte chunks */
	mov r6, r7
	mov 64, r0
	sub r0, r7	
	cmp/gt r4, r7
	__memcpy32.l0_64:
	bf __memcpy32.l1_64
	bsr __memcpy32_64
	mov 64, r0
	add r0, r4
	add r0, r5
	bra __memcpy32.l0_64
	cmp/gt r4, r7
	__memcpy32.l1_64:

	/* Copy 16-byte chunks */
	mov r6, r7
	mov 16, r0
	sub r0, r7	
	cmp/gt r4, r7
	__memcpy32.l0_16:
	bf __memcpy32.l1_16
	bsr __memcpy32_16
	mov 16, r0
	add r0, r4
	add r0, r5
	bra __memcpy32.l0_16
	cmp/gt r4, r7
	__memcpy32.l1_16:

	/* Copy 4-byte chunks */
	mov 16, r1
	cmp/gt r4, r6
	__memcpy32.l0_4:
	bf __memcpy32.l1_4
	mov.l @r5, r0
	mov.l r0, @r5
	add r1, r4
	add r1, r5
	bra __memcpy32.l0_4
	cmp/gt r4, r6
	__memcpy32.l1_4:

	lds.l @r15+, pr
	rts
	nop

#endif

#if 0
.global X(strcpy)
X(strcpy):
	strcpy.L0:
	mov.b @r5+, r3
	mov.b r3, @r4
	add #1, r4
	tst r3, r3
	bf strcpy.L0
	rts
	nop
#endif

#if 0
.global X(strcmp)
X(strcmp):
//	sts.l pr, @-r15

	strcmp.L1:
	mov.b @r4+, r2
	mov.b @r5+, r3
	tst r2, r2
	bt strcmp.L0
	tst r3, r3
	bt strcmp.L0
	cmp/eq r2, r3
	bt strcmp.L1

	strcmp.L0:
	mov r2, r0
	sub r3, r0

//	lds.l @r15+, pr
	rts
	nop

#endif

/* .section .vect */
.align 4
_vector_table:

.long  _start					/* 0x00: power-on reset */
.long  0xFFFC					/* 0x01 */
.long  _start					/* 0x02: manual reset */
.long  0xFFFC					/* 0x03 */
.long _gdb_illegalinst_isr  	/* 0x04: general illegal instruction */
.long _gdb_unhandled_isr		/* 0x05: (reserved) */
.long _gdb_illegalinst_isr		/* 0x06: slot illegal instruction */
.long _gdb_unhandled_isr		/* 0x07: (reserved) */
.long _gdb_unhandled_isr		/* 0x08: (reserved) */
.long _gdb_addresserr_isr		/* 0x09: CPU address error */
.long _gdb_addresserr_isr		/* 0x0A: DMAC/DTC address error */
.long _gdb_nmi_isr				/* 0x0B: NMI */
.long _gdb_unhandled_isr		/* 0x0C: UBC */
.long _gdb_unhandled_isr		/* 0x0D: (reserved) */
.long _gdb_unhandled_isr		/* 0x0E: (reserved) */
.long _gdb_unhandled_isr		/* 0x0F: (reserved) */
.long _gdb_pit_isr	 			/* 0x10: PIT */
.long _gdb_ihandler_emac		/* 0x11: (EMAC interface) */
.long _gdb_ihandler_uart0		/* 0x12: (UART0 Console) */
.long _gdb_ihandler_uart2		/* 0x13: (UART2 Console) */
.long _gdb_unhandled_isr		/* 0x14: (reserved) */

.long _gdb_unhandled_isr		/* 0x15: (reserved) */
.long _gdb_unhandled_isr		/* 0x16: (reserved) */
.long _gdb_ihandler_uart1		/* 0x17: (UART1 Console) */
.long _gdb_unhandled_isr		/* 0x18: (reserved) */
.long _gdb_ihandler_inttmr		/* 0x19: (when AIC countdown reach 0) */
.long _gdb_unhandled_isr		/* 0x1A: (reserved) */
.long _gdb_unhandled_isr		/* 0x1B: (reserved) */
.long _gdb_unhandled_isr		/* 0x1C: (reserved) */
.long _gdb_unhandled_isr		/* 0x1D: (reserved) */
.long _gdb_unhandled_isr		/* 0x1E: (reserved) */

.long _gdb_unhandled_isr		/* 0x1F: (reserved) */
.long _gdb_trapa32_isr			/* 0x20: trap 32 instruction */
.long _gdb_trapa33_isr			/* 0x21: trap 33 instruction */
.long _gdb_trapa34_isr			/* 0x22: trap 34 instruction */
.long _gdb_unhandled_isr		/* 0x23 */
.long _gdb_unhandled_isr		/* 0x24 */
.long _gdb_unhandled_isr		/* 0x25 */
.long _gdb_unhandled_isr		/* 0x26 */
.long _gdb_unhandled_isr		/* 0x27 */

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr


.section .data
.weak X(__fpscr_values)
.global X(__fpscr_values)
X(__fpscr_values):
	.long   0
	.long   0x80000

.end

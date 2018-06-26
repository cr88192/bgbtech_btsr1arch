#ifdef ARCH_SH4
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
.extern X(tk_main)
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
a_tk_main: .long X(tk_main)
.long 0x5B5B5B5B
isr_table: .long _vector_table
.long 0x5C5C5C5C

.global X(sleep_0)
X(sleep_0):
	sleep
	rts


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

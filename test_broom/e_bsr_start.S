// #define X(n)	_##n
#define X(n)	n
//#define STACK_BASE	0x0000DFFC
#define STACK_BASE_ISR	0x0000DFF0
#define STACK_BASE_USR	0x0000DF00

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
	mov isr_table, r0
	mov r0, vbr

	bsr X(__start)
	bra _exit

_exit:
	mov #-1, r0
	jmp r0
	nop

zero_bss:
//	mov.l bss_strt, r2
//	mov.l bss_end, r3
	mov __bss_start, r2
	mov _end, r3
	mov #0, r4
.L0:
	mov.l r4, @-r3
	cmp/hi r2, r3
	bt .L0
	rts
	bra _exit

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

isr_except:
	break
//	rte

isr_inter:
	rte

isr_table:
	bra8b _start
	bra8b isr_except
	bra8b isr_inter

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

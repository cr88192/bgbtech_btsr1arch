#define X(n)	n
#define STACK_BASE_ISR	0x0000DFF0
#define STACK_BASE_USR	0x010FFFC0

.section .text
.align 4

.global _start
.global _vector_table
.extern X(__start)
.extern X(timer_ticks)

.extern X(__bss_start)
.extern X(_end)

.extern X(tk_checksane_asm)

__label_0:
	nop
	nop

_start:
	mov STACK_BASE_USR, r10
	mov	r10, sp
	
	xor r10, r10
	mov r10, vbr
	mov r10, gbr
	mov r10, tbr
	mov r10, mmcr

	mov		0x0004000000000000, r10
	mov		r10, vbr

//	cpuid #1
//	test #15, r0
//	bf _iscore2

	csrrw	r0, 0xFC1, x10
	and		r10, 15, r11
	bne		r11, r0, _iscore2


//	mov		__data_start, r10
	mov		0xC000, r10
	mov		r10, gbr


//	bsr tk_checksane_asm

	bsr copy_data
	bsr zero_bss

	mov STACK_BASE_ISR, r10
	mov r10, ssp

	bsr X(__start)
	bra _exit

_exit:
	break
	bra _exit

_iscore2:
	mov		core2_isr_table, r10
	mov		r10, vbr
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

.fix32_off


.fix32_on
.align 16

core2_isr_table:
	bra _iscore2
	nop
	bra _iscore2__isr
	nop
	bra _iscore2__isr
	nop
	bra _iscore2__isr
	nop

_iscore2__isr:
	mov		exsr, r17
	mov		0xF0FF, r14
	and		r17, r14, r16
	mov		0x80AA, r15
	beq		r15, r16, _iscore2__isr_boot
	mov		0xB000, r15
	beq		r15, r16, _iscore2__isr_boot
	rte
	nop
_iscore2__isr_boot:
	mov		tea, r12
	beq		r12, r0, _iscore2
	jmp		r12


zero_bss:
	mov		__bss_start, r10
	mov		_end, r13
	mov 	0, r11
	sub		r13, r10, r12
	bra		memset
//	break

copy_data:
	mov		__rom_data_start, r11
	mov		__rom_data_end, r13
//	mov		__data_start, r10
	mov		gbr, r10
	sub		r13, r11, r12
//	bra		memcpy
	bra		__memcpy128
//	break

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

.global __va64_saveargs
__va64_saveargs:
	mov.q	r10, (r6,  0)
	mov.q	r11, (r6,  8)
	mov.q	r12, (r6, 16)
	mov.q	r13, (r6, 24)
	mov.q	r14, (r6, 32)
	mov.q	r15, (r6, 40)
	mov.q	r16, (r6, 48)
	mov.q	r17, (r6, 56)
	mov.q	r7, (r6, 128)
	mov.q	r0, (r6, 136)
	mov.q	r2, (r6, 144)
	rts
	nop

.global __va64_arg_i
.global __va64_arg_l
__va64_arg_i:
__va64_arg_l:
	mov.l	(r10, 128), r28
	mov		64, r29
//	cmp/gt	r28, r29
//	bf		__va64_arg_i.L0
	brge	r29, r28, __va64_arg_i.L0
	
	mov		r28, r29
	add		r10, r29
	mov.q	(r29), r30
	add		8, r28
	mov.l	r28, (r10, 128)
	mov		r30, r10
	rts
	nop

__va64_arg_i.L0:
	mov.q	(r10, 144), r31
	mov.q	(r31), r30
	add		8, r31
	mov.q	r31, (r10, 144)
	mov		r30, r10
	
	break
	rts
	nop

.global __va64_arg_x
__va64_arg_x:
	mov.l	(r10, 128), r28
// .ifarch abi_evenonly
//	test	8, r28
//	add?f	8, r28
// .endif
	mov		56, r29
//	cmp/gt	r28, r29
//	bf		__va64_arg_i.L0
	brge	r29, r28, __va64_arg_x.L0

	mov		r28, r29
	add		r10, r29
	mov.q	(r29, 0), r30
	mov.q	(r29, 8), r31
	add		16, r28
	mov.l	r28, (r10, 128)
	mov		r30, r10
	mov		r31, r11
	rts
	nop

__va64_arg_x.L0:
	mov.q	(r4, 144), r29
	add		7, r29
	and		-8, r29
	mov.q	(r29, 0), r30
	mov.q	(r29, 8), r31
	add		16, r29
	mov.q	r29, (r10, 144)
	mov		r30, r10
	mov		r31, r11
	rts
	nop

sleep_0:
	sleep
	rts


__memcpy128:
	add		r11, r12, r15
//  bgt		r0, r12, .L0
	bgt		r11, r15, .L0
    bra		.L1
	.L0:
	mov.q	(r11, 0), r16
	mov.q	(r11, 8), r17
//	add		r12, -16, r12
    add		r11, 16, r11
    mov.q	r16, (r10, 0)
	mov.q	r17, (r10, 8)
    add		r10, 16, r10
//	bgt		r0, r12, .L0
	bgt		r11, r15, .L0
    .L1:
    rts

memcpy:
	and		r12, 15, r13
	beq		r13, r0, __memcpy128
	
	sub		r12, r13, r16
	add		r10, r16, r14
	add		r11, r16, r15

	.L0:
	mov.b	(r15, 0), r17
	add		r13, -1, r13
	add		r15, 1, r15
	mov.b	r17, (r14, 0)
	add		r14, 1, r14
	bgt		r0, r13, .L0
	
	mov		r16, r12
	bra		__memcpy128

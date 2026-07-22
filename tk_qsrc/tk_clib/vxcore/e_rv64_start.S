#define STACK_BASE_USR	0x010FFFC0
#define STACK_BASE_SYS	0x0101FFC0
#define STACK_BASE_ISR	0x0100FFC0

.section .data

.balign 8

.quad	0x123

__tk_saved_vbr:
	.quad 0
//	.quad 0

.quad	0x456

.section .text

.global _start
_start:

#ifdef __USRONLY__

	bsr __start
	bra __exit

#else

	mov		vbr, r10
	mov		0x0000FFFFFFFFFFFE, r7
	and		r10, r7, r10

	mov		__tk_saved_vbr, r11
	mov.q	r10, (r11)
	
	mov		STACK_BASE_USR, r15
	mov		STACK_BASE_ISR, r16

	mov		sr, r17
	or 		#0x00C0, r17

	mov		0x000C000000000001, r28
	mov		_isr_table, r14
	or		r14, r28, r14

	bne		r10, r0, .nonboot

	mov		r14, vbr
	mov		r15, sp
	mov		r16, ssp
	mov		r17, sr

	.nonboot:
	bsr __start
	bra __exit

#endif

.global tk_getsavedvbr
tk_getsavedvbr:
	mov		__tk_saved_vbr, a0
	add		a0, -8, a1
	mov.q	(a1, 0), a2
	mov.q	(a0, 8), a3
	mov.q	(a0), a0

	mov		0x123, a4
	mov		0x456, a5
	bne		a4, a2, .fail
	bne		a5, a3, .fail

	rts
	
	.fail:
	break
	rts

.global sleep_0
sleep_0:
	sleep
	rts

.global _arch_gettbr
.global _arch_settbr
_arch_gettbr:
	mov	tbr, a0
	rts
_arch_settbr:
	mov	a0, tbr
	rts

.balign 128

_isr_table:
	bra8b _start
	bra8b _isr_except
	bra8b _isr_inter
	bra8b _isr_tlbfault
	bra8b _isr_syscall
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b

	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b
	nop4b

_isr_except:
	break

.extern __isr_interrupt
.extern __isr_tlbfault
.extern __isr_syscall

_isr_inter:
	bra __isr_interrupt
_isr_tlbfault:
	bra __isr_tlbfault
_isr_syscall:
	bra __isr_syscall


.global tk_syscall2
.global tk_syscall2_rtuser

tk_syscall2:
	mov	0, r14
	mov	0, r15
	mov	0, r16
	mov	-1, r17

	mov	0, x5
	mov x5, r0u
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	rts
	nop

tk_syscall2_rtuser:
	mov	1, x5
	mov x5, r0u
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	rts
	nop


.section .utext

.global tk_syscall_utxt
.global tk_syscall_rv_utxt

tk_syscall_utxt:
	break
	rts
	nop
	nop

tk_syscall_rv_utxt:
	mov	0, r14
	mov	0, r15
	mov	0, r16
	mov	-1, r17

	mov	0, x5
	mov x5, r0u
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	rts
	nop

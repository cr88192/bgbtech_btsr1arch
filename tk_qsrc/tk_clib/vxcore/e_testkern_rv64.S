.globl	_start
.globl	_arch_gettbr
.globl	_arch_settbr
.globl __longj
.globl __debugbreak
.globl tk_getsavedvbr

_start:
	.option push
	.option norelax
//1:	auipc gp, %pcrel_hi(__global_pointer$)
//	addi  gp, gp, %pcrel_lo(1b)
	la gp, __global_pointer$
	.option pop

	call	__basetests

    call	__start
	li		a0, 0
	call	__exit

_arch_gettbr:
	mv		a0, x4
	jr		ra

_arch_settbr:
	mv		x4, a0
	jr		ra

tk_getsavedvbr:
	li		a0, 0
	jr		ra

__longj:


__debugbreak:
	ebreak

.globl __halt
__halt:
	jr	ra

.globl __object_getbits
__object_getbits:
	jr	ra
	
.globl __start_init
__start_init:
	jr	ra


__basetests:
	li		a0, 0
	addi	a0, a0, 123
	li		a1, 123
	bne		a0, a1, __debugbreak
	
	lw		a2, __baseimm0
	li		a3, 0x12345678
	bne		a2, a3, __debugbreak

	beq		a0, a3, __debugbreak

	lw		a4, __baseimm1
	li		a5, 0xFFFFFFFFC2345678
	bne		a4, a5, __debugbreak
	
	jr	ra

__baseimm0:	.dword	0x12345678
__baseimm1:	.dword	0xC2345678


.globl sleep_0
sleep_0:
	jr	ra

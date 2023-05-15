.globl	_start
.globl	_arch_gettbr
.globl __longj
.globl __debugbreak

_start:
	.option push
	.option norelax
1:	auipc gp, %pcrel_hi(__global_pointer$)
	addi  gp, gp, %pcrel_lo(1b)
	.option pop

    call	__start
	li		a0, 0
	call	__exit

_arch_gettbr:
	mv		a0, x4
	jr		ra

_arch_settbr:
	mv		x4, a0
	jr		ra

__longj:


__debugbreak:

.globl __halt
__halt:
	jr	ra

.globl __object_getbits
__object_getbits:
	jr	ra
	
.globl __start_init
__start_init:
	jr	ra

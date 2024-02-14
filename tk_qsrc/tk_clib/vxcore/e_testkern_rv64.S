.globl	_start
.globl	_arch_gettbr
.globl	_arch_settbr
.globl __longj
.globl __setj
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
	nop

_arch_settbr:
	mv		x4, a0
	jr		ra
	nop

tk_getsavedvbr:
	li		a0, 0
	jr		ra
	nop

__setj:
	sd	ra,  8(a0)
	sd	sp, 16(a0)
	sd	gp, 24(a0)
	sd	x8, 64(a0)
	sd	x9, 72(a0)

	sd	x18, 144(a0)
	sd	x19, 152(a0)
	sd	x20, 160(a0)
	sd	x21, 168(a0)
	sd	x22, 176(a0)
	sd	x23, 184(a0)
	sd	x24, 192(a0)
	sd	x25, 200(a0)
	sd	x26, 208(a0)
	sd	x27, 216(a0)
	
	li	a0, 0
	jr	ra
	nop

__longj:
	ld	ra,  8(a0)
	ld	sp, 16(a0)
	ld	gp, 24(a0)
	ld	x8, 64(a0)
	ld	x9, 72(a0)

	ld	x18, 144(a0)
	ld	x19, 152(a0)
	ld	x20, 160(a0)
	ld	x21, 168(a0)
	ld	x22, 176(a0)
	ld	x23, 184(a0)
	ld	x24, 192(a0)
	ld	x25, 200(a0)
	ld	x26, 208(a0)
	ld	x27, 216(a0)
	
	mv	a0, a1

	jr	ra
	nop
	nop


__debugbreak:
	ebreak
	ebreak
	nop
	nop

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
//	bne		a0, a1, __debugbreak

	beq		a0, a1, __basetests_L0
	ebreak
	__basetests_L0:
	
	lw		a2, __baseimm0
	li		a3, 0x12345678
//	bne		a2, a3, __debugbreak

	beq		a2, a3, __basetests_L1
	ebreak
	__basetests_L1:

//	beq		a0, a3, __debugbreak
	bne		a0, a3, __basetests_L2
	ebreak
	__basetests_L2:

	lw		a4, __baseimm1
	li		a5, 0xFFFFFFFFC2345678
//	bne		a4, a5, __debugbreak

	beq		a4, a5, __basetests_L3
	ebreak
	__basetests_L3:
	
	jr	ra
	nop
	nop

__baseimm0:	.dword	0x12345678
__baseimm1:	.dword	0xC2345678


.globl sleep_0
sleep_0:
	jr	ra
	nop
	nop

.globl __hint_use_egpr
__hint_use_egpr:
	jr	ra
	nop
	nop

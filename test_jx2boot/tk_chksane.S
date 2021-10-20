/*
Run basic ISA-level sanity checks.

This logic does not follow the C ABI, and may stomp most of the registers.
This is assumed to be called prior to transferring control to C land.
 */

#define X(n)	n

#ifndef JX2UC
#include "tk_chksane_gfp.S"
#include "tk_chksane_wex.S"
#include "tk_chksane_xgpr.S"
#include "tk_chksane_rvi.S"
#endif

#include "tk_chksane_mem.S"

_checksane_fail:
	break
	break
	bra _checksane_fail
	break

_checksane_fail_alu:
	break
	break
	bra _checksane_fail_alu
	break

_checksane_fail_bra:
	break
	break
	bra _checksane_fail_bra
	break

_checksane_fail_logic:
	break
	break
	bra _checksane_fail_logic
	break

_checksane_fail_shad:
	break
	break
	bra _checksane_fail_shad
	break

_checksane_fail_mul:
	break
	break
	bra _checksane_fail_mul
	break

_checksane_fail_alu2a:
	break
	break
	bra _checksane_fail_alu2a
	break

_checksane_fail_alu2b:
	break
	break
	bra _checksane_fail_alu2b
	break

_checksane_fail_alu2c:
	break
	break
	bra _checksane_fail_alu2c
	break

_checksane_fail_alu2d:
	break
	break
	bra _checksane_fail_alu2d
	break

_checksane_fail_alu2e:
	break
	break
	bra _checksane_fail_alu2e
	break

_checksane_fail_alu2f:
	break
	break
	bra _checksane_fail_alu2f
	break
	break

_checksane_fail_alu3:
	break
	break
	bra _checksane_fail_alu3
	break
	break


_checksane_fail_alu4:
	break
	break
	bra _checksane_fail_alu4
	break


	break
	break
	break

.global X(tk_checksane_asm)
X(tk_checksane_asm):

//	rts

//	push lr
	add		-8, sp
	mov		lr, r1
	mov.q	r1, (sp)

	.L0A00:

/* Stage 1, Basic ALU */
	mov	0x12345678, r4
	mov	0x1234CDEF, r5
	mov	0x24692467, r8
	mov	0xFFFF8889, r9
	mov	0xFFFFEEEE, r11
	exts.l	r9, r10

	cmp/eq r4, r5
	bt _checksane_fail_alu

	cmp/gt r4, r5
	bf _checksane_fail_alu

	cmp/gt r4, r9
	bt _checksane_fail_alu

	cmp/gt r9, r4
	bf _checksane_fail_alu

	cmp/gt r9, r11
	bf _checksane_fail_alu

	cmp/gt r11, r9
	bt _checksane_fail_alu

	.L0A0:
	cmp/hi r4, r9
//	bf _checksane_fail
	break?f
	nop

	.L0A1:
	cmp/ge r4, r5
//	bf _checksane_fail
	break?f
	nop

	.L0A2:
	cmp/ge r4, r9
//	bt _checksane_fail
	break?t
	nop

	.L0A3:
	cmp/ge r4, r4
//	bf _checksane_fail
	break?f
	nop

	.L0A4:
	movt	r12
	cmp/eq #1, r12
//	bf _checksane_fail
	break?f
	nop

	.L0A5:
	movnt	r12
	cmp/eq #0, r12
//	bf _checksane_fail
	break?f
	nop

	.L0A6:
	tst		r12, r12
//	nop
	movnt	r12
	cmp/eq #0, r12
//	bf _checksane_fail
	break?f
	nop

	mov	r9, r21
	mov r21, r12

	cmp/eq r21, r9
	bf _checksane_fail_alu
	cmp/eq r12, r9
	bf _checksane_fail_alu


	mov 0xEDCBA987, r12

	.L0B0:
	tst r4, r8
//	bt _checksane_fail
	break?t
	nop

	.L0B1:
	tst r4, r12
//	bf _checksane_fail
	break?f
	nop


	add	r4, r5, r6
	mov r4, r7
	add r5, r7
	cmp/eq r6, r7
	bf _checksane_fail_alu
	cmp/eq r6, r8
	bf _checksane_fail_alu

	add	r4, r5, r18
	cmp/eq r18, r7
	bf _checksane_fail_alu

	sub	r4, r5, r6
	mov r4, r7
	sub r5, r7
	cmp/eq r6, r7
	bf _checksane_fail_alu
	cmp/eq r6, r9
	bf _checksane_fail_alu
	cmpq/eq r6, r10
	bf _checksane_fail_alu

	adds.l	r4, r5, r6
	addu.l	r4, r5, r7

	cmp/eq r6, r7
	bf _checksane_fail_alu
	cmp/eq r6, r8
	bf _checksane_fail_alu

	subs.l	r4, r5, r6
	subu.l	r4, r5, r7

	cmpq/eq r6, r10
	bf _checksane_fail_alu
	cmpq/eq r9, r9
	bf _checksane_fail_alu


	mov	0x12344468, r8
	mov	0x1234DFFF, r9
	mov	0x00009B97, r10

	and	r4, r5, r6
	mov r4, r7
	and r5, r7
	cmp/eq r6, r7
	bf _checksane_fail_alu

	cmp/eq r6, r8
	bf _checksane_fail_alu

	or	r4, r5, r6
	mov r4, r7
	or r5, r7
	cmp/eq r6, r7
	bf _checksane_fail_alu

	cmp/eq r6, r9
	bf _checksane_fail_alu


	xor	r4, r5, r6
	mov r4, r7
	xor r5, r7
	cmp/eq r6, r7
	bf _checksane_fail_alu

	cmp/eq r6, r10
	bf _checksane_fail_alu


	mov	0xEDCBA987, r11

	not r4, r6
	mov r4, r7
	not r7

	cmp/eq r6, r7
	bf _checksane_fail_alu
	cmp/eq r6, r11
	bf _checksane_fail_alu

	mov	0xEDCBA988, r12

	neg r4, r6
	mov r4, r7
	neg r7

	cmp/eq r6, r7
	bf _checksane_fail_alu
	cmp/eq r6, r12
	bf _checksane_fail_alu

	neg r4, r16
	mov r4, r17
	neg r17

	cmp/eq r16, r17
	bf _checksane_fail_alu
	cmp/eq r16, r12
	bf _checksane_fail_alu


	mov	0xE87AD208, r8
	mov	0x014B6F5A, r9

	mov	0x014B6F5AE87AD208, r10
	mov	0x10882A3D75C123C0, r11
	mov	0xFE53D3C575C123C0, r12

#if 0
	mulu	r4, r5
	mov r0, r6
	mov r1, r7

	.L0C0:
	cmp/eq r6, r8
//	bf _checksane_fail_mul
	break?f
	
	.L0C1:
	cmp/eq r7, r9
//	bf _checksane_fail_mul
	break?f
#endif

#if 1
	dmulu	r4, r5, r6

	.L0C0:
	cmpq/eq r6, r10
	break?f

	dmulu	r4, r8, r6

	.L0C1:
	cmpq/eq r6, r11
	break?f

	dmuls	r4, r8, r6
	.L0C2:
	cmpq/eq r6, r12
	break?f
#endif

#if 1
	mov 0x0004CB2F, R10
	mov 0xFFFBDA2C, R11
	mov 0xFFFFFFEC1DEDF214, R12

	dmuls	r10, r11, r6
	.L0C3:
	cmpq/eq r6, r12
	break?f
#endif

#if 1
	mov	0x00000000E50C6E30, r10
	mov	0xFFFFFFFFE50C6E30, r11

	.L0C4:
	mulu	r4, r9, r6
	cmpq/eq r6, r10
	break?f

	.L0C5:
	muls	r4, r9, r7
	cmpq/eq r7, r11
	break?f
#endif


	mov	0x75C123C0, r10
	mov	0x10882A3D, r11
	mov	0xFE53D3C5, r12

#if 0
	.L0C2:
	mulu	r4, r8
	mov r0, r6
	mov r1, r7
	cmp/eq r6, r10
//	bf _checksane_fail_mul
	break?f
	cmp/eq r7, r11
//	bf _checksane_fail_mul
	break?f
#endif

#if 0
	.L0C3:
	muls	r4, r8
	mov r0, r6
	mov r1, r7
	cmp/eq r6, r10
//	bf _checksane_fail_mul
	break?f
	cmp/eq r7, r12
//	bf _checksane_fail_mul
	break?f
#endif


	mov	0x1234CE58, r8
	mov	0x1234CD86, r9

	add r5, #0x69, r6
	mov r5, r7
	add #0x69, r7

	cmp/eq r6, r7
	bf _checksane_fail_alu2a
	cmp/eq r6, r8
	bf _checksane_fail_alu2a

	add r5, #0x69, r16
	cmp/eq r16, r7
	bf _checksane_fail_alu2a

	add r5, #-0x69, r6
	mov r5, r7
	add #-0x69, r7

	cmp/eq r6, r7
	bf _checksane_fail_alu2a
	cmp/eq r6, r9
	bf _checksane_fail_alu2a

	mov #0x69, r6
	cmp/eq #0x69, r6
	bf _checksane_fail_alu2b
	cmp/eq #0x73, r6
	bt _checksane_fail_alu2b
	cmp/gt #0x73, r6
	bt _checksane_fail_alu2b
	
	tst #0x31, r6
	bt _checksane_fail_alu2b
	tst #0x96, r6
	bf _checksane_fail_alu2b


	mov #0x6, r6
	cmp/eq #0x6, r6
	bf _checksane_fail_alu2c
	cmp/eq #0x9, r6
	bt _checksane_fail_alu2c

	cmp/gt #0x5, r6
	bf _checksane_fail_alu2c
	cmp/gt #0x6, r6
	bt _checksane_fail_alu2c
	cmp/gt #0x7, r6
	bt _checksane_fail_alu2c

	cmp/ge #0x5, r6
	bf _checksane_fail_alu2d
	cmp/ge #0x6, r6
	bf _checksane_fail_alu2d
	cmp/ge #0x7, r6
	bt _checksane_fail_alu2d

	mov #-0x6, r6
	cmp/eq #-0x6, r6
	bf _checksane_fail_alu2e
	cmp/eq #-0x9, r6
	bt _checksane_fail_alu2f


	mov	0x23456780, r8
	mov	0x01234567, r9

	shad  r4, #4, r6
	shad  r4, #-4, r7

	cmpq/eq r6, r8
//	bf _checksane_fail_shad
	break?f
	cmpq/eq r7, r9
//	bf _checksane_fail_shad
	break?f


	mov	0x68ACF000, r8
	mov	0x00091A2B, r9

	shad  r4, #9, r6
	shad  r4, #-9, r7

	cmpq/eq r6, r8
//	bf _checksane_fail_shad
	break?f
	
	cmpq/eq r7, r9
//	bf _checksane_fail_shad
	break?f

	mov #9, r22
	shad  r4, r22, r6
	cmpq/eq r6, r8
//	bf _checksane_fail_shad
	break?f

	mov	0x2468ACF000, r10
	mov	0xE87AD208, r11
	mov	0x01D0F5A4, r12
	mov	0xFFD0F5A4, r13
	mov 0xC00000000, r14

	cmp/eq r8, r10
//	bf _checksane_fail_shad
	break?f

	cmpq/eq r8, r10
//	bt _checksane_fail_shad
	break?t

	cmp/gt r8, r11
//	bt _checksane_fail_shad
	break?t

	cmp/hi r8, r11
//	bf _checksane_fail_shad
	break?f

//	cmpq/eq #0xE87AD208, r11
//	bf _checksane_fail
	
	tst r14, r14
	bf _checksane_fail_logic

	tstq r14, r14
	bt _checksane_fail_logic

	shad.q  r4, #9, r6
	cmpq/eq r6, r10
	bf _checksane_fail_shad

	shld  r11, #-7, r6
	cmp/eq r6, r12
	bf _checksane_fail_shad

	shad  r11, #-7, r6
	cmp/eq r6, r13
	bf _checksane_fail_shad

	mov #-7, r2
	mov #7, r3

	shad  r11, r2, r6
	shar  r11, r3, r7

	cmp/eq r6, r13
	bf _checksane_fail_shad
	cmp/eq r7, r13
	bf _checksane_fail_shad

	mov #-7, r2
	mov #7, r3

	shld  r11, r2, r6
	shlr  r11, r3, r7
	cmp/eq r6, r12
	bf _checksane_fail_shad
	cmp/eq r7, r12
	bf _checksane_fail_shad


	mov	0xE87AD208, r11
	mov	0x00000000000AD208, r16
	mov	0xFFFFFFFFFFFAD208, r17
	mov	0x00000000AD208000, r18
	mov	0xFFFFFFFFAD208000, r19

#if 1
	shad.q r16, 0, r6
	shad.q r17, 0, r7

	cmpqeq r6, r16
	bf _checksane_fail_shad
	cmpqeq r7, r17
	bf _checksane_fail_shad

	shld.q r16, 0, r6
	shld.q r17, 0, r7

	cmpqeq r6, r16
	bf _checksane_fail_shad
	cmpqeq r7, r17
	bf _checksane_fail_shad
#endif

#if 1
	shad	r16, 0, r6
	shad	r17, 0, r7

	cmpqeq r6, r16
	bf _checksane_fail_shad
	cmpqeq r7, r17
	bf _checksane_fail_shad

	shld	r11, 0, r6
	cmpqeq	r6, r11
	bf _checksane_fail_shad

#endif

	shld.q r11, 12, r6
	shld r6, -12, r7

	cmpqeq r7, r16
	bf _checksane_fail_shad

//	shad r11, 12, r6
	shad r6, -12, r7

	cmpqeq r7, r17
	bf _checksane_fail_shad

	shld r11, 12, r6
	shad r11, 12, r7

	cmpqeq r6, r18
	bf _checksane_fail_shad

	cmpqeq r7, r19
	bf _checksane_fail_shad

	shad.q  r4, #31, r7

	mov r4, r6
	shll1 r6
	shll2 r6
	shll4 r6
	shll8 r6
	shll16 r6

	cmpq/eq r6, r4
	bt _checksane_fail_shad
	cmpq/eq r6, r7
	bf _checksane_fail_shad

	shlr1 r6
	shlr2 r6
	shlr4 r6
	shlr8 r6
	shlr16 r6

	cmpq/eq r6, r4
	bf _checksane_fail_shad

#if 1
	shld	r13, #-31, r6
	shad	r13, #-31, r7

	cmpeq	#1, r6
	bf		_checksane_fail_shad
	cmpeq	#-1, r7
	bf		_checksane_fail_shad
#endif

#if 0
//	push r4
//	push r5
//	pop r7
//	pop r6

	cmp/eq r4, r6
	bf _checksane_fail_bra
	cmp/eq r5, r7
	bf _checksane_fail_bra

//	push r4
//	pop lr
	mov lr, r6
	cmp/eq r4, r6
	bf _checksane_fail_bra
#endif

	mov r5, lr
	mov lr, r6
	cmp/eq r5, r6
	bf _checksane_fail_bra

	cmp/eq r5, r6
	cselt	r13, r5, r19
	cmp/eq r13, r19
	bf _checksane_fail_alu3

	cmp/eq r12, r19
	cselt	r5, r12, r19
	cmp/eq r12, r19


	bra	.L0
	.L2:
	break
	.L0:
//	bra .L1
	bra .L4
	bra .L2
	.L4:
	bsr .L1
	.L3:
	rts
	.L1:
	bsr .L3
	nop
	nop


	mov	0x12345678, r4
	mov	0x1234CDEF, r5
	mov	0x24692467, r8
	mov	0xFFFF8889, r9
	
	exts.l	r9, r10
	extu.l	r9, r11

	cmpq/eq r9, r10
	bt _checksane_fail_alu3
	cmpq/eq r9, r11
	bf _checksane_fail_alu3
	cmpq/eq r10, r11
	bt _checksane_fail_alu3

	exts.b	r5, r6
	extu.b	r5, r7

	cmpq/eq #-0x11, r6
	bf _checksane_fail_alu3
	cmpq/eq #0xEF, r7
	bf _checksane_fail_alu3

	exts.w	r5, r6
	extu.w	r5, r7

	cmpq/eq #-0x3211, r6
	bf _checksane_fail_alu3
	cmpq/eq #0xCDEF, r7
	bf _checksane_fail_alu3
	
	mov .L7_1, r1
	mov	r1, lr
	mov .L6, r6
	jmp r6
	break
	.L6:
	bra .L8
	break
	.L8:
	mov #2, r6
	braf r6
	break
	break
	bra .L7
	break
	break
	.L7:
	rtsu
	break
	.L7_1:
	mov #1, r6
	bra.l r6
	break
	break
	bra .L7_2
	break
	break
	.L7_2:

//	bsr tk_checksane_mem_asm

	.L8_0:
	mov #28, r18
	add #3, r18
	cmpeq #31, r18
//	bf _checksane_fail_alu4
	break?f

	.L8_1:
	neg r18, r19
	cmpeq #-31, r19
//	bf _checksane_fail_alu4
	break?f

	.L8_2:
	add #-3, r18
	cmpeq #28, r18
//	bf _checksane_fail_alu4
	break?f


	.L8_3:
	xor r19, r19
	tst	r19, r19
//	bf _checksane_fail_alu4
	break?f

	.L8_4:
	mov #3, r21
	mov #0x8000, r20
	tst	r20, r20
//	bt _checksane_fail_alu4
	break?t

	.L8_5:
	tst		#-4, r21
//	bf _checksane_fail_alu4
	break?f

	.L8_6:
	tst		#-4, r20
//	bt _checksane_fail_alu4
	break?t

	mov	0x08889, r8
	mov 0x11112, r9
	mov 0x11113, r10
	
	mov r8, r18
	addc r8, r18

	.L8_7:
	cmpeq r9, r18
//	bf _checksane_fail_alu4
	break?f

	mov r8, r18
	addc r8, r18

	.L8_8:
	cmpeq r10, r18
//	bf _checksane_fail_alu4
	break?f

	xor r6, r6
	cmpeq r8, r8
	addc r6, r6
	cmpgt r8, r8
	addc r6, r6
	cmpeq r8, r8
	addc r6, r6
	cmpgt r8, r8
	addc r6, r6

	.L8_9:
//	mov 0xA, r7
//	cmpeq r7, r6
	cmpeq 0xA, r6
//	bf _checksane_fail_alu4
	break?f

//	break

#if 1
	mov	0x8889, r8
	mov	0xFFFF8889, r9
	mov	0x00FF8889, r10
	mov 0, r11

	clz r8, r6
	clz r9, r7

	.CLZ_0A:
	cmp/eq #16, r6
//	bf _checksane_fail
	break?f

	.CLZ_0B:
	cmp/eq #0, r7
//	bf _checksane_fail
	break?f

	clz r10, r6
	clz r11, r7

	.CLZ_1A:
	cmp/eq #8, r6
//	bf _checksane_fail
	break?f

	.CLZ_1B:
	cmp/eq #32, r7
//	bf _checksane_fail
	break?f

	mov	0x00000089, r12
	mov	0x00000389, r13
	clz r12, r6
	clz r13, r7

	.CLZ_2A:
	cmp/eq #24, r6
//	bf _checksane_fail
	break?f

	.CLZ_2B:
	cmp/eq #22, r7
//	bf _checksane_fail
	break?f


	clzq r8, r6
	clzq r9, r7

	.CLZ_3A:
	cmp/eq #48, r6
//	bf _checksane_fail
	break?f

	.CLZ_3B:
	cmp/eq #32, r7
//	bf _checksane_fail
	break?f

	clzq r10, r6
	clzq r11, r7

	cmp/eq #40, r6
//	bf _checksane_fail
	break?f
	cmp/eq #64, r7
//	bf _checksane_fail
	break?f

	clzq r12, r6
	clzq r13, r7

	.CLZ_4A:
	cmp/eq #56, r6
//	bf _checksane_fail
	break?f

	.CLZ_4B:
	cmp/eq #54, r7
//	bf _checksane_fail
	break?f
#endif

	mov		#0x12345678, r4
	mov		#0x5678ABCD, r5
	mov		#0x123456789ABCDEF0, r6
	jldi	#0x123456789ABCDEF0, r7
//	break
	mov.l tk_checksane_const, r8
	mov.l tk_checksane_const2, r9

	.L9_0:

	cmpqeq r4, r8
//	bf _checksane_fail_alu4
	break?f

	.L9_1:

	cmpqeq r5, r9
//	bf _checksane_fail_alu4
	break?f
	
	mov.q tk_checksane_const3, r10

	.L9_2:

	cmpqeq r6, r10
//	bf _checksane_fail_alu4
	break?f

	.L9_3:

	cmpqeq r6, r7
//	bf _checksane_fail_alu4
	break?f

	bsr X(tk_checksane_mem_asm)

#ifndef JX2UC

	bsr X(tk_checksane_gfp_asm)

	bsr X(tk_checksane_wex_asm)

.ifarch has_xgpr
	bsr X(tk_checksane_xgpr_asm)
.endif

	bsr X(tk_checksane_rvi_asm)

#endif

	mov #-1, r2
	invic r2

	mov.q	(sp), r1
	add		8, sp
	jmp		r1

//	pop lr
//	rts

.section .rodata

tk_checksane_const: .long 0x12345678
nop
tk_checksane_const2: .long 0x5678ABCD

tk_checksane_const3: .quad 0x123456789ABCDEF0

.section .text

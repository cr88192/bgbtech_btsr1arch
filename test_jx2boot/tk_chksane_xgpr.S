.ifarch has_xgpr

#define CHKSANE_EN_XGPR

.global X(tk_checksane_xgpr_asm)
X(tk_checksane_xgpr_asm):

	add	-64, sp

	mov	0x12345678, r2
	mov	0x1234CDEF, r3

	mov	0x12345678, r36
	mov	0x1234CDEF, r37
	mov	0x24692467, r40
	mov	0xFFFF8889, r41
	mov	0xFFFFEEEE, r43
	exts.l	r41, r42

	cmp/eq r36, r37
	break?t

	cmp/gt r36, r37
	break?f

	cmp/gt r36, r41
	break?t

	cmp/gt r41, r36
	break?f

	cmp/gt r41, r43
	break?f

	cmp/gt r43, r41
	break?t


	cmp/eq r2, r37
	break?t
	cmp/gt r2, r37
	break?f

	cmp/eq r36, r3
	break?t
	cmp/gt r36, r3
	break?f
	
	add		r36, r37, r4
	add		r36, r3, r5
	add		r2, r37, r6

	cmp/eq	r4, r40
	break?f
	cmp/eq	r5, r40
	break?f
	cmp/eq	r6, r40
	break?f

	mov.q	r36, (sp, 16)
	mov.q	r37, (sp, 24)
	mov.q	(sp, 16), r44
	mov.q	(sp, 24), r45

	cmp/eq	r36, r44
	break?f
	cmp/eq	r37, r45
	break?f

	mov.q?t	r36, (sp, 8)
	mov.q?f	r37, (sp, 8)
	mov.q	(sp, 8), r46

	cmp/eq	r36, r46
	break?f

//	break

	add	64, sp

	rts

.endif

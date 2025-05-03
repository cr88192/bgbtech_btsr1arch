#ifdef __BJX2__

u64 __umulsq(u64 a, u64 b);
s64 __smulsq(s64 a, s64 b);

__int128 __umuldq(u64 a, u64 b);
__int128 __smuldq(s64 a, s64 b);

__asm {
/*
	umulsq: Unsigned Multiply, Single Quad
	smulsq: Signed Multiply, Single Quad
*/

.global __umulsq
.global __smulsq
__umulsq:
__smulsq:
	shld.q	r4, -32, r6
	shld.q	r5, -32, r7
	dmulu	r4, r5, r2

	dmulu	r4, r7, r16
	dmulu	r6, r5, r17
	add		r16, r17, r18
	shld.q	r18, 32, r3
	add		r3, r2

	rtsu
};

__asm {
/*
	umuldq: Unsigned Multiply, Double Quad
*/
.global __umuldq
__umuldq:
	shld.q	r4, -32, r6
	shld.q	r5, -32, r7

	dmulu	r4, r5, r16
	dmulu	r4, r7, r17
	dmulu	r6, r5, r18
	dmulu	r6, r7, r19

	shld.q	r17, 32, r20
	shld.q	r17, -32, r21
	shld.q	r18, 32, r22
	shld.q	r18, -32, r23

	mov		r16, r2
	mov		r19, r3
	clrt
	addc	r20, r2
	addc	r21, r3
	clrt
	addc	r22, r2
	addc	r23, r3

	rtsu

/*
	smuldq: Signed Multiply, Double Quad
*/
.global __smuldq
__smuldq:
	shad.q	r4, -32, r6
	shad.q	r5, -32, r7

	dmulu	r4, r5, r16
	dmulu	r4, r7, r17
	dmulu	r6, r5, r18
	dmuls	r6, r7, r19

	shld.q	r17, 32, r20
	shld.q	r17, -32, r21
	shld.q	r18, 32, r22
	shld.q	r18, -32, r23

	mov		r16, r2
	mov		r19, r3
	clrt
	addc	r20, r2
	addc	r21, r3
	clrt
	addc	r22, r2
	addc	r23, r3

	rtsu
};

double __fpu_frcp(double x);
double __fpu_frcp_s(double x);
double __fpu_frcp_sf(double x);

__asm {
.ifarch bjx2_wex

/*
	Stomps r2, r3, r5, r6, r7
 */
.global __fpu_frcp
__fpu_frcp:
	mov 0, r3
	fcmp/gt r4, r3

	nop
	fneg?t r4

	.L0:
.ifarch has_fpvsf_sp
//	mov 0x7FF, r5		| mov 0x3FF, r6
	mov 0x7FE, r5		| mov 0x3FF, r6
	shld.q r5, 52, r5	| mov 0x400, r3
	shld.q r6, 52, r6	| sub r5, r4, r5
	shld.q r3, 52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2
.else
//	mov 0x7FF, r5		| mov 0x3FF, r6
	mov 0x7FE, r5		| mov 0x3FF, r6
	shld.q r5, 52, r5	| mov 0x400, r3
	shld.q r6, 52, r6	| sub r5, r4, r5
	shld.q r3, 52, r3	| fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2
.endif

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fneg?t r2

	rtsu

.global __fpu_frcp_s
__fpu_frcp_s:
	mov 0, r3
	fcmp/gt r4, r3

	nop
	fneg?t r4

	.L0:
//	mov 0x7FF, r5		| mov 0x3FF, r6
	mov 0x7FE, r5		| mov 0x3FF, r6
.ifarch has_fpvsf_sp
	shld.q r5, 52, r5	| mov 0x400, r3
	shld.q r6, 52, r6	| sub r5, r4, r5
	shld.q r3, 52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r7, r2
.else
	shld.q r5, 52, r5	| mov 0x400, r3
	shld.q r6, 52, r6	| sub r5, r4, r5
	shld.q r3, 52, r3	| fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r4, r6
	fsub r3, r6, r7
	fmul r7, r2
.endif
	fneg?t r2
	
	rtsu

.global __fpu_fdiv
__fpu_fdiv:
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp
	fmul r18, r2
	mov r16, lr
	rts

.global __fpu_fdiv_s
__fpu_fdiv_s:
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_s
	fmul r18, r2
	mov r16, lr
	rts


.global __fpu_frcp_sf
__fpu_frcp_sf:
	cmpqgt 0, r4
	fneg?f r4

//	mov 0x7FF, r5		| mov 0x3FF, r6
	mov 0x7FE, r5		| mov 0x3FF, r6
	shld.q r5, 52, r5	| mov 0x400, r3
	shld.q r6, 52, r6	| sub r5, r4, r5
	shld.q r3, 52, r3	| fmula r5, r4, r7
	sub r6, r7, r7
	add r7, r5, r2

	fmula r2, r4, r6
	fsuba r3, r6, r7
	fmula r2, r7, r2

	fneg?f r2
	
	rtsu

.global __fpu_fdiv_sf
__fpu_fdiv_sf:
	cmpqgt 0, r5
//	nop
	fneg?f r5
	mov		0x4000000000000000, r1
//	mov		0x7FF0000000000000, r3
	mov		0x7FE0000000000000, r3
	mov		0x3FF0000000000000, r6
	sub		r3, r5, r3
	fmula	r3, r5, r7
	sub		r6, r7, r7
	add		r7, r3, r2

	fmula	r2, r5, r6
	fsuba	r1, r6, r7
	fmula	r2, r7, r2

	fneg?f	r2
	fmul	r2, r4, r2	
	rtsu

.endif
};

__asm {
.ifnarch bjx2_wex
/*
	Stomps r2, r3, r5, r6, r7
 */
.global __fpu_frcp
__fpu_frcp:
	mov 0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp
	fneg r2
	jmp r20
//	ret

	.L0:
//	mov r4, r3
//	mov r4, r6
	mov 0x7FF0000000000000, r5
//	sub r6, r5
	sub r4, r5
	mov 0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov 0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2
	
	rts

.global __fpu_frcp_s
__fpu_frcp_s:
	mov 0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	jmp r20
//	ret

	.L0:
	mov 0x7FF0000000000000, r5
	sub r4, r5
	mov 0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov 0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2

//	fmul r2, r5, r6
//	fsub r3, r6, r7
//	fmul r7, r2

//	fmul r2, r5, r6
//	fsub r3, r6, r7
//	fmul r7, r2
	
	rts

.global __fpu_fdiv
__fpu_fdiv:
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp
	fmul r18, r2
	mov r16, lr
	rts

.global __fpu_fdiv_s
__fpu_fdiv_s:
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_s
	fmul r18, r2
	mov r16, lr
	rts


.global __fpu_frcp_sf
__fpu_frcp_sf:
	mov 0x0000, r0
	fldch r0, r3
	fcmp/gt r4, r3
	bf .L0
//	push lr
	mov lr, r20
	fneg r4
	bsr __fpu_frcp_s
	fneg r2
	jmp r20
//	ret

	.L0:
	mov 0x7FF0000000000000, r5
	sub r4, r5
	mov 0x3FF0000000000000, r6

	fmul r5, r4, r7
	sub r6, r7, r7
	add r7, r5

	mov r5, r2
	mov r4, r5
	
	mov 0x4000, r0
	fldch r0, r3

	fmul r2, r5, r6
	fsub r3, r6, r7
	fmul r7, r2
	
	rts

.global __fpu_fdiv_sf
__fpu_fdiv_sf:
	mov lr, r16
	mov r4, r18
	mov r5, r4
	bsr __fpu_frcp_sf
	fmul r18, r2
	mov r16, lr
	rts

.endif
};

#endif

u32 __moddi3(u32 a, u32 b)
{
	u32 q, r;
	
	q=a/b;
	r=a-(q*b);
	return(r);
}

u32 __umodsi3(u32 a, u32 b)
{
	u32 q, r;
	
	q=a/b;
	r=a-(q*b);

//	if(((s32)r)<0)
//		{ *(int *)-1=-1; }
//	if(r>=b)
//		{ *(int *)-1=-1; }

	return(r);
}

s32 __smodsi3(s32 a, s32 b)
{
	s32 q, r;
	
	q=a/b;
	r=a-(q*b);
	return(r);
}

#if 1
u64 __udivdi3(u64 n, u64 d);
s64 __sdivdi3(s64 a, s64 b);


u64 __udivsq(u64 a, u64 b)
{

	return(__udivdi3(a, b));

#if 0
	u64_obj_t c, d, e;
	u64 v, t;

#if 1
	if((((u32)a)==a) && (((u32)b)==b))
	{
		return(((u32)a)/((u32)b));
	}
#endif

	if((b&(b-1))==0)
	{
		v=a;	t=b;
		while(t>1)
			{ v=v>>1; t=t>>1; }
		return(v);
	}
	
	*(u64 *)(&c)=a;
	*(u64 *)(&d)=b;
	e=__udivlli(c, d);
//	*(u64_obj_t *)(&v)=__udivlli(c, d);
	v=*(u64 *)(&e);
	return(v);
#endif
}

s64 __sdivsq(s64 a, s64 b)
{
	return(__sdivdi3(a, b));

#if 0
#if 1
	if((((s32)a)==a) && (((s32)b)==b))
	{
		return(((s32)a)/((s32)b));
	}
#endif
	
	return(__sdivlli(a, b));
#endif
}

u64 __umodsq(u64 a, u64 b)
{
	u64 c, d;

#if 1
	if((((u32)a)==a) && (((u32)b)==b))
	{
		if(((u32)(b&(b-1)))==0)
			return(((u32)a)&((u32)(b-1)));
	
		return(((u32)a)%((u32)b));
	}
#endif

	if((b&(b-1))==0)
	{
//		__debugbreak();
		return(a&((u32)(b-1)));
	}

	c=a/b;
	d=a-(c*b);

	if(((s64)d)<0)
		d+=b;

	if(d>=b)
	{
		d-=b;
		if(d>=b)
		{
			tk_printf("a/b = %X_%X / %X_%X\n",
				(u32)(a>>32), (u32)a, (u32)(b>>32), (u32)b);
			tk_printf("c,d = %X_%X , %X_%X\n",
				(u32)(c>>32), (u32)c, (u32)(d>>32), (u32)d);
		
			__debugbreak();
			return(0);
		}
//		while(d>=b)
//			d-=b;
	}
	return(d);
}

s64 __smodsq(s64 a, s64 b)
{
	s64 c, d;

	c=__sdivsq(a, b);
	d=c*b;
	return(a-d);

#if 0
	if(b<0)
	{
//		c=__sdivsq(a, -b)
//		return(-__smodsq(a, -b));
	}

	if(a<0)
		{ return(-__umodsq(-a, b)); }
	else
		{ return(__umodsq(a, b)); }
#endif

}
#endif



#ifdef __BJX2__
__asm {

.global __va64_saveargs
__va64_saveargs:
	mov.x	r4, (r3, 0)
	mov.x	r6, (r3, 16)
	mov.x	r20, (r3, 32)
	mov.x	r22, (r3, 48)
.ifarch abi_is_xgarg
	mov.x	r36, (r3, 64)
	mov.x	r38, (r3, 80)
	mov.x	r52, (r3, 96)
	mov.x	r54, (r3, 112)
.endif
	rts
	nop

.global __va64_arg_i
__va64_arg_i:
	mov.l	(r4, 128), r2
.ifarch abi_is_xgarg
	mov		128, r3
.else
	mov		64, r3
.endif
	cmp/gt	r2, r3
	bf		__va64_arg_i.L0
	
	mov		r2, r3
	add		r4, r3
	mov.q	(r3), r6
	add		8, r2
	mov.l	r2, (r4, 128)
	mov		r6, r2
	rts
	nop

__va64_arg_i.L0:
	mov.q	(r4, 144), r3
	mov.l	(r3), r6
	add		4, r3
	mov.q	r3, (r4, 144)
	mov		r6, r2
	rts
	nop

#if 1
.global __va64_arg_l
__va64_arg_l:
	mov.l	(r4, 128), r2
.ifarch abi_is_xgarg
	mov		128, r3
.else
	mov		64, r3
.endif
	cmp/gt	r2, r3
	bf		__va64_arg_l.L0	

	mov		r2, r3
	add		r4, r3
	mov.q	(r3), r6
	add		8, r2
	mov.l	r2, (r4, 128)
	mov		r6, r2
	rts
	nop
__va64_arg_l.L0:
	mov.q	(r4, 144), r3
	add		7, r3
	and		-8, r3
	mov.q	(r3), r6
	add		8, r3
	mov.q	r3, (r4, 144)
	mov		r6, r2
	rts
	nop
#endif


.global __va64_arg_x
__va64_arg_x:
	mov.l	(r4, 128), r2
.ifarch abi_evenonly
	test	8, r2
	add?f	8, r2
.endif
.ifarch abi_is_xgarg
	mov		120, r3
.else
	mov		56, r3
.endif
	cmp/gt	r2, r3
	bf		__va64_arg_x.L0
	mov		r2, r3
	add		r4, r3
	mov.q	(r3, 0), r6
	mov.q	(r3, 8), r7
	add		16, r2
	mov.l	r2, (r4, 128)
	mov		r6, r2
	mov		r7, r3
	rts
	nop
__va64_arg_x.L0:
	mov.q	(r4, 144), r3
	add		7, r3
	and		-8, r3
	mov.q	(r3), r6
	add		8, r3
	mov.q	(r3), r7
	add		8, r3
	mov.q	r3, (r4, 144)
	mov		r6, r2
	mov		r7, r3
	rts
	nop

};

#endif

#ifdef __RISCV__

__asm {

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
	
//	mov		r28, r29
//	add		r10, r29
	add		r10, r28, r29
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

//	mov		r28, r29
//	add		r10, r29
	add		r10, r28, r29
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
	
.global _arch_gettbr
_arch_gettbr:
	mov		r4, r10
	rts
	nop


__fpu_fdiv:
	fdiv	x10, x11, x10
	rts
	nop

__xlf_todbl:
	rts
	nop

.global _start
.extern __start

_start:
	bsr __start
	bra _exit
	nop
	nop

_exit:
	break
	mov 0, r4
	mov 0x1003, r5
	mov 0, r6
	mov sp, r7
//	syscall 0
	syscall
	break

};

#endif

#if 1
// #if 0

.ifarch has_qmul

.global	__udivsi3_i
.global	__sdivsi3

__udivsi3_i:
	divu.q	r4, r5, r2
	rts

__sdivsi3:
	divs.q	r4, r5, r2
	rts

.else

.ifarch bjx2_wex
#include "tk_divi2_wx.S"
.else
#include "tk_divi2_sc.S"
.endif

.endif

#else
#include "tk_divi2_sc.S"
#endif
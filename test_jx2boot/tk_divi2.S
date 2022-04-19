#ifdef USEDIV

.global	__udivsi3
.global	__sdivsi3

__udivsi3:
	DIVU.Q	R4, R5, R2
	RTS

__sdivsi3:
	DIVS.Q	R4, R5, R2
	RTS

#else

#if 0
.ifarch bjx2_wex
#include "tk_divi2_wx.S"
.else
#include "tk_divi2_sc.S"
.endif
#else
#include "tk_divi2_sc.S"
#endif

#endif
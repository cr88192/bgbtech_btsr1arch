#if 1
.ifarch bjx2_wex
#include "tk_divi2_wx.S"
.else
#include "tk_divi2_sc.S"
.endif
#else
#include "tk_divi2_sc.S"
#endif
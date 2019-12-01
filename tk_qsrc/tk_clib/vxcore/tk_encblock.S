#if 0
#include "tk_encblock_sc.S"
#else
.ifarch bjx2_wex
#include "tk_encblock_wx.S"
.else
#include "tk_encblock_sc.S"
.endif
#endif

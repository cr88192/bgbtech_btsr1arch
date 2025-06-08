#include "x3vm_interp.h"
#include "x3vm_pgmimg.h"
#include "x3vm_romfcn.h"

#include "tkgdi/tkgdi.h"


#include "x3vm_fp16.c"

#include "x3vm_mem.c"

#include "x3vm_opc_arith.c"
#include "x3vm_opc_simd.c"
#include "x3vm_opc_mem.c"

#include "x3vm_dec_x3op.c"
#include "x3vm_dec_rvop.c"
#include "x3vm_trace.c"

#include "x3vm_petask.c"
#include "x3vm_peload_pbo.c"

#include "x3vm_syscall.c"

#include "x3vm_interp.c"

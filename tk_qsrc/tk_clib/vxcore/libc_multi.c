/*
The LIBC version will only include application-related logic.
Everything else will assume the use of system calls.

 */

#ifndef __PDPCLIB_API__
#define __PDPCLIB_API__
#endif

#ifndef __TK_CLIB_ONLY__
#define __TK_CLIB_ONLY__
#endif


#include <tk_core.h>
#include <tkgdi/tkgdi.h>

#include "pdpc201/stdio.c"
#include "pdpc201/string.c"
#include "pdpc201/stdlib.c"
#include "pdpc201/start.c"
#include "pdpc201/time.c"
#include "pdpc201/errno.c"
#include "pdpc201/assert.c"
#include "pdpc201/signal.c"
#include "pdpc201/locale.c"
#include "pdpc201/ctype.c"
#include "pdpc201/setjmp.c"
#include "pdpc201/math.c"
#include "pdpc201/math2.c"
#include "pdpc201/threads.c"
// #include "pdpc201/__memmgr.c"

#include "pdpc201/complex.c"
#include "pdpc201/stdlibext.c"

#include <tk_core.h>

#include "tk_core/tk_mmpage.c"
#include "tk_core/tk_mmcell.c"
#include "tk_core/tk_mmllist.c"

#include "tk_core/tk_mmap.c"

// #include "tk_core/tk_spi.c"
#include "tk_core/tk_epoch.c"
// #include "tk_core/tk_fat.c"
#include "tk_core/tk_vfile.c"
#include "tk_core/tk_divi.c"
#include "tk_core/tk_puts.c"
// #include "tk_core/tk_msgbuf.c"
// #include "tk_core/tk_conprn.c"
// #include "tk_core/tk_ps2keyb.c"
// #include "tk_core/tk_peload.c"
// #include "tk_core/tk_peload_pbo.c"
// #include "tk_core/tk_syscall.c"

#include "tk_core/tk_env.c"
#include "tk_core/tk_envctx.c"
// #include "tk_core/tk_shcmd.c"
#include "tk_core/tk_irq.c"

#include "tk_core/tku_bigint.c"

#include "tk_core/lva_array.c"
#include "tk_core/lva_strdup.c"
#include "tk_core/lva_tagobj.c"
#include "tk_core/lva_tagopr.c"
#include "tk_core/lva_varobj.c"

#include "tk_core/tk_rwad.c"

#include "pdpc201/btshx_supa.c"
#include "pdpc201/btshx_supdll.c"

#include "pdpc201/btshx_lva.c"
#include "pdpc201/btshx_xli.c"
#include "pdpc201/btshx_xlf.c"
#include "pdpc201/btshx_softfpu.c"
#include "pdpc201/btshx_fpusup.c"

#include "pdpc201/btrv_softfp.c"

#include "pdpc201/btshx_vnf.c"
#include "pdpc201/btshx_alloca.c"

#include "pdpc201/bt_mman.c"
#include "pdpc201/bt_readdir.c"
#include "pdpc201/bt_dlfcn.c"

#include "pdpc201/bt_htonl.c"
#include "pdpc201/bt_libcgpa.c"

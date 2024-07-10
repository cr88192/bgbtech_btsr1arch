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

#ifndef __TK_CLIB_DLLSTUB__
#define __TK_CLIB_DLLSTUB__
#endif

#include <tk_core.h>

#include "pdpc201/bt_libcgpa.c"
#include "pdpc201/stdio_gpa.c"

#include "pdpc201/string.c"
#include "pdpc201/stdlib.c"
// #include "pdpc201/start.c"
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

#include "pdpc201/complex.c"
#include "pdpc201/stdlibext.c"

#include <tk_core.h>

#include "tk_core/tk_divi.c"
// #include "tk_core/tku_bigint.c"

#include "pdpc201/btshx_alloca.c"

#include "pdpc201/btshx_supdll.c"

#if 0

#include "tk_core/lva_array.c"
#include "tk_core/lva_strdup.c"
#include "tk_core/lva_tagobj.c"
#include "tk_core/lva_tagopr.c"
#include "tk_core/lva_varobj.c"

#include "tk_core/tk_rwad.c"

// #include "pdpc201/btshx_supa.c"
// #include "pdpc201/btshx_lva.c"
// #include "pdpc201/btshx_supdll.c"
#include "pdpc201/btshx_xli.c"
#include "pdpc201/btshx_xlf.c"
#include "pdpc201/btshx_softfpu.c"
#include "pdpc201/btshx_fpusup.c"

#include "pdpc201/btshx_vnf.c"
// #include "pdpc201/btshx_alloca.c"

#include "pdpc201/bt_mman.c"
#include "pdpc201/bt_readdir.c"

#include "pdpc201/bt_htonl.c"

#endif

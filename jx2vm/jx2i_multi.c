#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include <math.h>

// #ifdef _M_X64
#if defined(_M_X64) || defined(__x86_64__)
#define BTESH2_HASJIT
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include "jx2i_interp.h"

#include "jx2r_fatfs.h"

#include "gfxdrv_keys.h"
#include "gfxdrv_w32.c"
#include "gfxdrv_input.c"

#include "sound_w32.c"

#include "jx2r_sdfat.c"
#include "jx2r_spimmc.c"

#include "jx2r_snd_sblk.c"

#include "jx2i_gfxcon.c"

#include "jx2i_mem.c"
#include "jx2i_ldrom.c"


#ifdef BTESH2_HASJIT
#include "bteuax64/uax_multi.c"
//#include "btesh2_jit_opmov.c"
//#include "btesh2_jit_opbra.c"
//#include "btesh2_jitjq_mov.c"
//#include "btesh2_jitjq_bra.c"
#include "jx2i_jit_opari.c"
#include "jx2i_jit_opbra.c"
#include "jx2i_jit_opmov.c"
#include "jx2i_jit.c"
#else
#include "jx2i_nojit.c"
#endif


#include "jx2i_opc_arith.c"
#include "jx2i_opc_bra.c"
#include "jx2i_opc_mov.c"
#include "jx2i_opc_cmov.c"
#include "jx2i_opc_fpari.c"
#include "jx2i_opc_fphfv.c"

#include "jx2i_dec_op.c"
#include "jx2i_dec_op_f0.c"
#include "jx2i_dec_op_f1.c"
#include "jx2i_dec_op_f8.c"
#include "jx2i_dec_op_fc.c"

#include "jx2i_dec_tr.c"
#include "jx2i_interp.c"

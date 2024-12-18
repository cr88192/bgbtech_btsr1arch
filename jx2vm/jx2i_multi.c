/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include <math.h>

#ifdef linux
#include <unistd.h>
#endif

#include "jx2i_conf.h"

// #ifdef _M_X64
// #if defined(_M_X64) || defined(__x86_64__)
#if defined(_M_X64)
#define BTESH2_HASJIT_X64
#endif

#if defined(ARM)
#define BTESH2_HASJIT_ARM
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include "jx2i_interp.h"

#include "jx2r_fatfs.h"
#include "jx2r_tkdfs.h"

#include "gfxdrv_keys.h"
#include "gfxdrv_w32.c"
#include "gfxdrv_input.c"

#include "sound_w32.c"

#include "jx2r_tkdfs.c"
#include "jx2r_sdfat.c"
#include "jx2r_spimmc.c"

#include "jx2r_snd_sblk.c"

#include "jx2i_gfxcon.c"

#include "jx2i_mem.c"
#include "jx2i_ldrom.c"

#include "jx2r_edgewalk.c"

void BJX2_Op_MOVW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op);

#if defined(BTESH2_HASJIT_X64)
#include "bteuax64/uax_multi.c"
//#include "btesh2_jit_opmov.c"
//#include "btesh2_jit_opbra.c"
//#include "btesh2_jitjq_mov.c"
//#include "btesh2_jitjq_bra.c"
#include "jx2i_jit_opari.c"
#include "jx2i_jit_opbra.c"
#include "jx2i_jit_opmov.c"
#include "jx2i_jit.c"
#elif defined(BTESH2_HASJIT_ARM)
#include "bteua_a32/ua32_multi.c"
#include "jx2i_ja32_jit.c"
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
#include "jx2i_dec_op_f2.c"
#include "jx2i_dec_op_f8.c"
#include "jx2i_dec_op_fc.c"
#include "jx2i_dec_op_rvi.c"
#include "jx2i_dec_op_rvc.c"

#include "jx2i_dec_tr.c"
#include "jx2i_interp.c"
#include "jx2i_gfxdbg.c"

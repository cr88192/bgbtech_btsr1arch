#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include <math.h>

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

#include "jx2i_opc_arith.c"
#include "jx2i_opc_bra.c"
#include "jx2i_opc_mov.c"
#include "jx2i_opc_fpari.c"

#include "jx2i_dec_op.c"
#include "jx2i_dec_op_f0.c"
#include "jx2i_dec_op_f1.c"
#include "jx2i_dec_op_f8.c"
#include "jx2i_dec_op_fc.c"

#include "jx2i_dec_tr.c"
#include "jx2i_interp.c"
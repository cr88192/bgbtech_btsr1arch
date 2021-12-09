#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bt4b_common.h"
#include "tk_baseavi.h"
#include "gfxdrv_keys.h"


#include "bt4b_grad16.c"
#include "bt4b_blkdec16.c"
#include "bt4b_blkdec.c"
#include "bt4b_bitread.c"
#include "bt4b_decode.c"

#include "tkulzd.c"

#include "btp0a_decode.c"
#include "bt5b_decode.c"

#include "snd_btac1c.c"

#include "tk_ramavi.c"
// #include "tk_codec_cram.c"

#ifdef _WIN32
#include "gfxdrv_w32.c"
#include "sound_w32.c"
#include "gfxdrv_input.c"
#endif

#ifdef __BJX2__
#include "i_snd_btsh4.c"
#endif

#include "pv_main.c"

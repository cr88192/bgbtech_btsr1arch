/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <math.h>
#include <time.h>

#define BTIC1H_VFWDRV

#define printf btjpg_printf

void btjpg_printf(char *str, ...);

#include "bt1h_common.h"

#include "bt1h_shim_s2tc.c"
#include "bt1h_shim_bc7.c"

#include "bt1h_thread.c"
#include "bt1h_thwork.c"

#include "bt1h_rice.c"
#include "bt1h_decrice.c"
#include "bt1h_bitrange.c"
#include "bt1h_rangedec.c"
#include "bt1h_rangeenc.c"

#include "bt1h_blkenc.c"
#include "bt1h_blkdec.c"
#include "bt1h_encmisc.c"
#include "bt1h_encode.c"
#include "bt1h_decode.c"


#include "bt4b_common.h"

#include "bt4b_grad16.c"

#include "bt4b_blkenc.c"
#include "bt4b_bitwrite.c"
#include "bt4b_encode.c"

#include "bt4b_blkdec.c"
#include "bt4b_bitread.c"
#include "bt4b_decode.c"
#include "bt4b_shim_bc7.c"
#include "bt4b_shim_s2tc.c"


#include "snd_btac1c.c"

#include "btjdrv_util.c"
#include "btjdrv_base.c"
#include "btjdrv_codec.c"
#include "btjdrv_codec_bt1h.c"
#include "btjdrv_codec_bt4b.c"
#include "btjvfw_driverproc.c"

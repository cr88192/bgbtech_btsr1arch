#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include "bsr1_interp.h"

#include "bsr1_mem.c"
#include "bsr1_ldrom.c"

#include "bsr1_opc_arith.c"
#include "bsr1_opc_bra.c"
#include "bsr1_opc_mov.c"
#include "bsr1_opc_fpari.c"

#include "bsr1_dec_op.c"
#include "bsr1_dec_tr.c"
#include "bsr1_interp.c"

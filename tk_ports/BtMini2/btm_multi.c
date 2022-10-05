/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>


#include "tkrast/tkr_multi.c"

#include "btm_engine.h"

#ifdef _WIN32
#include "gfxdrv_w32.c"
#include "gfxdrv_input.c"
#include "sound_w32.c"
#endif

#include "gfxdrv_keys.h"

#define TGVLZ_NOMAIN
#include "tgvlz1.c"

#include "bt1h_targa.c"

#include "cca_node.c"
#include "cca_print.c"
#include "cca_parse.c"
#include "cca_abxe.c"

#include "btm_thread.c"
#include "btm_thread_bjx2.c"

#include "btm_pgl.c"

#include "btm_voxtype.c"

// #include "btra_span.c"
// #include "btra_col.c"
// #include "btra_wall.c"

// #include "rat_loadply.c"
// #include "tkrast/tkr_multi.c"

#include "btra_ray2.c"

#include "btm_world.c"
#include "btm_phys.c"
#include "btm_region.c"
#include "btm_tgen.c"
#include "btm_light.c"
#include "btm_mob.c"
#include "btm_inven.c"
#include "btm_delta.c"

#include "btm_wad4.c"

#include "btm_loadmod.c"
#include "btm_sndmix.c"

#include "btm_menu.c"
#include "btm_console.c"

#include "btm_drawtext.c"

#include "btm_main.c"

#ifndef __PDPCLIB_API__
#define __PDPCLIB_API__
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

#include "pdpc201/stdlibext.c"
#include "pdpc201/complex.c"

#include <tk_core.h>

#include "tk_core/tk_mmpage.c"
#include "tk_core/tk_mmcell.c"
#include "tk_core/tk_mmllist.c"

#include "tk_core/tk_mmap.c"

#include "tk_core/tk_hugeptr.c"
#include "tk_core/tk_blkdev.c"

#include "tk_core/tk_spi.c"
#include "tk_core/tk_epoch.c"
#include "tk_core/tk_fat.c"
#include "tk_core/tk_devfs.c"
#include "tk_core/tk_bdev_ram.c"
#include "tk_core/tk_bdev_spi.c"
#include "tk_core/tk_bdev_vfb.c"
#include "tk_core/tk_vmem.c"

#include "tk_core/tk_wad4.c"
#include "tk_core/tk_vf_wad4.c"

// #include "tk_vf_fat.c"
// #include "tk_vf_sysc.c"

#include "tk_core/tk_vfile.c"
#include "tk_core/tk_divi.c"
#include "tk_core/tk_puts.c"
#include "tk_core/tk_msgbuf.c"
#include "tk_core/tk_conprn.c"
#include "tk_core/tk_ps2keyb.c"
#include "tk_core/tk_usb.c"
#include "tk_core/tk_peload.c"
#include "tk_core/tk_peload_pbo.c"
#include "tk_core/tk_elfload.c"
#include "tk_core/tk_syscall.c"

#include "tk_core/tk_env.c"
#include "tk_core/tk_envctx.c"

// #ifdef __BGBCC__
#include "tk_core/tk_shcmd.c"
// #endif

#include "tk_core/tk_irq.c"

#include "tk_core/tku_bigint.c"

// #include "tk_core/tk_fpusup_rv.c"
#include "pdpc201/btrv_softfp.c"

#include "tk_core/lva_array.c"
#include "tk_core/lva_strdup.c"
#include "tk_core/lva_tagobj.c"
#include "tk_core/lva_tagopr.c"
#include "tk_core/lva_varobj.c"

#include "tk_core/tk_rwad.c"

#include "tk_core/tk_sockmsg.c"

#include "tk_core/tk_regcache.c"
#include "tk_core/tk_tkdfs.c"

// #ifdef __BGBCC__
#if 1
#include "tk_core/tkgdi_blitupdate.c"
#include "tk_core/tkgdi_pixspan.c"
#include "tk_core/tkgdi_drawcon.c"
#include "tk_core/tkgdi_winman.c"
#include "tk_core/tkgdi_load_gtf.c"
#include "tk_core/tkgdi_load_upic.c"
#include "tk_core/tk_hal_tkgdi.c"

#include "tk_core/tk_hal_snd.c"
#include "tk_core/tk_hal_midi.c"
#endif

#include "pdpc201/btshx_supa.c"
#include "pdpc201/btshx_supdll.c"

// #ifdef __BGBCC__
#include "pdpc201/btshx_lva.c"
#include "pdpc201/btshx_xli.c"
#include "pdpc201/btshx_xlf.c"
// #endif

#include "pdpc201/btshx_softfpu.c"
#include "pdpc201/btshx_fpusup.c"
// #include "pdpc201/btshx_fpusup.c"

#ifdef __BGBCC__
#include "tk_core/tk_bcd.c"
#endif

#include "pdpc201/strings.c"

#include "pdpc201/btshx_vnf.c"
#include "pdpc201/btshx_alloca.c"

#ifdef __BGBCC__
#include "pdpc201/btshx_lvax.c"
#endif

#include "pdpc201/bt_mman.c"
#include "pdpc201/bt_readdir.c"
#include "pdpc201/bt_dlfcn.c"

#include "pdpc201/bt_htonl.c"

#include "pdpc201/bt_libcgpa.c"

#ifdef __BGBCC__
#include "tk_core/tk_neuron.c"
#endif

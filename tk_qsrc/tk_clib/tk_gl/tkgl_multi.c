#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#include <tkgl/bgbrasw.h>
#include <tkgl/rasw_glext.h>

#include "rast/tkgl_edge.c"
#include "rast/tkgl_span.c"
#include "rast/tkgl_teximg.c"
#include "rast/tkgl_trans.c"

#include "tkgl_api/ragl_begin.c"
#include "tkgl_api/ragl_clip.c"
#include "tkgl_api/ragl_gldrv.c"
#include "tkgl_api/ragl_glw32.c"
#include "tkgl_api/ragl_gpafuncs.c"
#include "tkgl_api/ragl_light.c"
#include "tkgl_api/ragl_raswprim.c"
#include "tkgl_api/ragl_teximage.c"
#include "tkgl_api/ragl_transform.c"
#include "tkgl_api/ragl_vertexarray.c"

#include "vecmath/math_clip.c"
#include "vecmath/math_hullf.c"
#include "vecmath/math_m3.c"
#include "vecmath/math_m4.c"
#include "vecmath/math_mf.c"
#include "vecmath/math_n.c"
#include "vecmath/math_plane.c"
#include "vecmath/math_quat.c"
#include "vecmath/math_solid.c"
#include "vecmath/math_v3f.c"
#include "vecmath/math_vecnf.c"
#include "vecmath/math_vnf.c"
#include "vecmath/mathlib_d.c"
#include "vecmath/mathlib_f.c"

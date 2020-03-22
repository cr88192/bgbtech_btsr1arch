TKGL_APISRCS = \
	tkgl_api/ragl_barfarray.c	\
	tkgl_api/ragl_begin.c		\
	tkgl_api/ragl_clip.c		\
	tkgl_api/ragl_gldrv.c		\
	tkgl_api/ragl_gpafuncs.c	\
	tkgl_api/ragl_light.c		\
	tkgl_api/ragl_raswprim.c	\
	tkgl_api/ragl_teximage.c	\
	tkgl_api/ragl_transform.c	\
	tkgl_api/ragl_vertexarray.c	\
	\
	tkgl_api/ragl_glw32.c		\
	\
	vecmath/math_clip.c			\
	vecmath/math_hullf.c		\
	vecmath/math_m3.c			\
	vecmath/math_m4.c			\
	vecmath/math_mf.c			\
	vecmath/math_n.c			\
	vecmath/math_plane.c		\
	vecmath/math_quat.c			\
	vecmath/math_solid.c		\
	vecmath/math_v3f.c			\
	vecmath/math_vecnf.c		\
	vecmath/math_vnf.c			\
	vecmath/mathlib_d.c			\
	vecmath/mathlib_f.c			\


TKGL_SRCS = \
	$(TKGL_APISRCS)

TKGL_MULTISRCS = \
	tkgl_multi.c

TKGL_HDRS = \
	include/tkgl/tkgl_auto.h

libs: $(DLLPF)tkgl$(DLLSUF)

include/tkgl/tkgl_auto.h: autohead$(BINSUF) $(TKGL_SRCS)
	./autohead$(BINSUF) -api TKGL_API $(TKGL_SRCS) > include/tkgl/tkgl_auto.h

autohead$(BINSUF): vecmath/autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) vecmath/autohead.c

$(DLLPF)tkgl$(DLLSUF): $(TKGL_HDRS) $(TKGL_SRCS) $(TKGL_MULTISRCS)
	$(CC_OUTDLL)$(DLLPF)tkgl$(DLLSUF) $(TKGL_MULTISRCS) \
	$(DLL_CFLAGS) $(DLL_LDFLAGS)


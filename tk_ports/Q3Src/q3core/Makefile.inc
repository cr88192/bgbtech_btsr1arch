all: includes libs apps FORCE

FORCE:

BUILD=.

includes: FORCE
	cp -rf qcommon/*.h		$(BUILD)/include
	cp -rf client/*.h		$(BUILD)/include
	cp -rf server/*.h		$(BUILD)/include
	cp -rf game/*.h			$(BUILD)/include

libs: qagame$(ARCH)$(DLLSUF) cgame$(ARCH)$(DLLSUF) ui$(ARCH)$(DLLSUF) 

apps: quake3$(ARCH)$(EXESUF) quake3$(ARCH)_tkra$(EXESUF) FORCE
# apps: quake3$(ARCH)$(EXESUF) FORCE

meta: FORCE

QUAKE3_CM_SRCS = \
	qcommon/cm_load.c \
	qcommon/cm_patch.c \
	qcommon/cm_polylib.c \
	qcommon/cm_test.c \
	qcommon/cm_trace.c \
	qcommon/cmd.c \
	qcommon/common.c \
	qcommon/cvar.c \
	qcommon/files.c \
	qcommon/huffman.c \
	qcommon/md4.c \
	qcommon/msg.c \
	qcommon/net_chan.c \
	\
	qcommon/unzip.c \
	qcommon/vm.c \
	qcommon/vm_interpreted.c \
	\
	game/q_math.c \
	game/q_shared.c \

#	qcommon/vm_x86.c \

QUAKE3_CL_SRCS = \
	client/cl_cgame.c \
	client/cl_cin.c \
	client/cl_console.c \
	client/cl_input.c \
	client/cl_keys.c \
	client/cl_main.c \
	client/cl_net_chan.c \
	client/cl_parse.c \
	client/cl_scrn.c \
	client/cl_ui.c \
	\
	client/snd_adpcm.c \
	client/snd_dma.c \
	client/snd_mem.c \
	client/snd_mix.c \
	client/snd_wavelet.c \

QUAKE3_SV_SRCS = \
	server/sv_bot.c \
	server/sv_ccmds.c \
	server/sv_client.c \
	server/sv_game.c \
	server/sv_init.c \
	server/sv_main.c \
	server/sv_net_chan.c \
	server/sv_snapshot.c \
	server/sv_world.c \

QUAKE3_BE_SRCS = \
	botlib/be_aas_bspq3.c \
	botlib/be_aas_cluster.c \
	botlib/be_aas_debug.c \
	botlib/be_aas_entity.c \
	botlib/be_aas_file.c \
	botlib/be_aas_main.c \
	botlib/be_aas_move.c \
	botlib/be_aas_optimize.c \
	botlib/be_aas_reach.c \
	botlib/be_aas_route.c \
	botlib/be_aas_routealt.c \
	botlib/be_aas_sample.c \
	botlib/be_ai_char.c \
	botlib/be_ai_chat.c \
	botlib/be_ai_gen.c \
	botlib/be_ai_goal.c \
	botlib/be_ai_move.c \
	botlib/be_ai_weap.c \
	botlib/be_ai_weight.c \
	botlib/be_ea.c \
	botlib/be_interface.c \
	botlib/l_crc.c \
	botlib/l_libvar.c \
	botlib/l_log.c \
	botlib/l_memory.c \
	botlib/l_precomp.c \
	botlib/l_script.c \
	botlib/l_struct.c \

QUAKE3_TR_SRCS = \
	renderer/tr_animation.c \
	renderer/tr_backend.c \
	renderer/tr_bsp.c \
	renderer/tr_cmds.c \
	renderer/tr_curve.c \
	renderer/tr_flares.c \
	renderer/tr_font.c \
	renderer/tr_image.c \
	renderer/tr_init.c \
	renderer/tr_light.c \
	renderer/tr_main.c \
	renderer/tr_marks.c \
	renderer/tr_mesh.c \
	renderer/tr_model.c \
	renderer/tr_noise.c \
	renderer/tr_scene.c \
	renderer/tr_shade.c \
	renderer/tr_shade_calc.c \
	renderer/tr_shader.c \
	renderer/tr_shadows.c \
	renderer/tr_sky.c \
	renderer/tr_surface.c \
	renderer/tr_world.c \

QUAKE3_JPEG6_SRCS = \
	jpeg-6/jcapimin.c \
	jpeg-6/jccoefct.c \
	jpeg-6/jccolor.c \
	jpeg-6/jcdctmgr.c \
	jpeg-6/jchuff.c \
	jpeg-6/jcinit.c \
	jpeg-6/jcmainct.c \
	jpeg-6/jcmarker.c \
	jpeg-6/jcmaster.c \
	jpeg-6/jcomapi.c \
	jpeg-6/jcparam.c \
	jpeg-6/jcphuff.c \
	jpeg-6/jcprepct.c \
	jpeg-6/jcsample.c \
	jpeg-6/jctrans.c \
	jpeg-6/jdapimin.c \
	jpeg-6/jdapistd.c \
	jpeg-6/jdatadst.c \
	jpeg-6/jdatasrc.c \
	jpeg-6/jdcoefct.c \
	jpeg-6/jdcolor.c \
	jpeg-6/jddctmgr.c \
	jpeg-6/jdhuff.c \
	jpeg-6/jdinput.c \
	jpeg-6/jdmainct.c \
	jpeg-6/jdmarker.c \
	jpeg-6/jdmaster.c \
	jpeg-6/jdpostct.c \
	jpeg-6/jdsample.c \
	jpeg-6/jdtrans.c \
	jpeg-6/jerror.c \
	jpeg-6/jfdctflt.c \
	jpeg-6/jidctflt.c \
	jpeg-6/jmemmgr.c \
	jpeg-6/jmemnobs.c \
	jpeg-6/jutils.c \

QUAKE3_FT2_SRCS = \
	ft2/ahangles.c \
	ft2/ahglobal.c \
	ft2/ahglyph.c \
	ft2/ahhint.c \
	ft2/ahmodule.c \
	ft2/ahoptim.c \
	ft2/ftcalc.c \
	ft2/ftdebug.c \
	ft2/ftextend.c \
	ft2/ftglyph.c \
	ft2/ftgrays.c \
	ft2/ftinit.c \
	ft2/ftlist.c \
	ft2/ftmm.c \
	ft2/ftnames.c \
	ft2/ftobjs.c \
	ft2/ftoutln.c \
	ft2/ftraster.c \
	ft2/ftrend1.c \
	ft2/ftsmooth.c \
	ft2/ftstream.c \
	ft2/ftsystem.c \
	ft2/sfdriver.c \
	ft2/sfobjs.c \
	ft2/ttcmap.c \
	ft2/ttdriver.c \
	ft2/ttgload.c \
	ft2/ttinterp.c \
	ft2/ttload.c \
	ft2/ttobjs.c \
	ft2/ttpload.c \
	ft2/ttpost.c \
	ft2/ttsbit.c \

QUAKE3_WIN_SRCS = \
	win32/win_gamma.c \
	win32/win_glimp.c \
	win32/win_glimp_tkra.c \
	win32/win_input.c \
	win32/win_main.c \
	win32/win_net.c \
	win32/win_qgl.c \
	win32/win_shared.c \
	win32/win_snd.c \
	win32/win_syscon.c \
	win32/win_wndproc.c \

QUAKE3_SRCS = \
	$(QUAKE3_CM_SRCS) \
	$(QUAKE3_CL_SRCS) \
	$(QUAKE3_SV_SRCS) \
	$(QUAKE3_BE_SRCS) \
	$(QUAKE3_TR_SRCS) \
	$(QUAKE3_JPEG6_SRCS) \
	\
	$(QUAKE3_OS_SRCS)

QUAKE3_BTSH_SRCS = \
	bjx2tk/tksh_glimp.c \
	bjx2tk/tksh_input.c \
	bjx2tk/tksh_main.c \
	bjx2tk/tksh_net.c \
	bjx2tk/tksh_qgl.c \
	bjx2tk/tksh_snddma.c \

#	bjx2tk/tksh_client.c \

TKRA_SRCS = \
	tkrast/tkr_blend.c			\
	tkrast/tkr_blend_bjx2.c		\
	tkrast/tkr_zatest.c			\
	tkrast/tkr_context.c		\
	tkrast/tkr_edge.c			\
	tkrast/tkr_glbegin.c		\
	tkrast/tkr_glenable.c		\
	tkrast/tkr_glgpa.c			\
	tkrast/tkr_glmatrix.c		\
	tkrast/tkr_gltex.c			\
	tkrast/tkr_matrix.c			\
	tkrast/tkr_multi.c			\
	tkrast/tkr_span.c			\
	tkrast/tkr_span_bjx2.c		\
	tkrast/tkr_span_zt.c		\
	tkrast/tkr_span_zt_bjx2.c	\
	tkrast/tkr_span_bf.c		\
	tkrast/tkr_span_bf_bjx2.c	\
	tkrast/tkr_span_hzt.c		\
	tkrast/tkr_teximg.c			\
	tkrast/tkr_trans.c			\

QUAKE3_TKRA_SRCS = \
	$(QUAKE3_CM_SRCS) \
	$(QUAKE3_CL_SRCS) \
	$(QUAKE3_SV_SRCS) \
	$(QUAKE3_BE_SRCS) \
	$(QUAKE3_TR_SRCS) \
	$(QUAKE3_JPEG6_SRCS) \
	\
	tkrast/tkr_multi.c			\
	\
	$(QUAKE3_OS_SRCS)

QUAKE3_H = \
	game/bg_public.h \
	cgame/cg_public.h \
	client/client.h \
	qcommon/cm_local.h \
	qcommon/cm_patch.h \
	qcommon/cm_polylib.h \
	qcommon/cm_public.h \
	game/g_public.h \
	ui/keycodes.h \
	client/keys.h \
	game/q_shared.h \
	qcommon/qcommon.h \
	qcommon/qfiles.h \
	renderer/qgl.h \
	win32/resource.h \
	server/server.h \
	client/snd_local.h \
	client/snd_public.h \
	game/surfaceflags.h \
	renderer/tr_local.h \
	renderer/tr_public.h \
	cgame/tr_types.h \
	ui/ui_public.h \
	qcommon/unzip.h \
	qcommon/vm_local.h \
	win32/win_local.h \

QUAKE3_UI_SRCS = \
	ui/ui_atoms.c \
	ui/ui_gameinfo.c \
	ui/ui_main.c \
	ui/ui_players.c \
	ui/ui_shared.c \
	ui/ui_syscalls.c \
	ui/ui_util.c \
	\
	game/bg_lib.c \
	game/bg_misc.c \
	game/q_math.c \
	game/q_shared.c \
	ui/ui.def \
	\

QUAKE3_Q3UI_SRCS =				\
	q3_ui/ui_addbots.c			\
	q3_ui/ui_atoms.c			\
	q3_ui/ui_cdkey.c			\
	q3_ui/ui_cinematics.c		\
	q3_ui/ui_confirm.c			\
	q3_ui/ui_connect.c			\
	q3_ui/ui_controls2.c		\
	q3_ui/ui_credits.c			\
	q3_ui/ui_demo2.c			\
	q3_ui/ui_display.c			\
	q3_ui/ui_gameinfo.c			\
	q3_ui/ui_ingame.c			\
	q3_ui/ui_loadconfig.c		\
	q3_ui/ui_main.c				\
	q3_ui/ui_menu.c				\
	q3_ui/ui_mfield.c			\
	q3_ui/ui_mods.c				\
	q3_ui/ui_network.c			\
	q3_ui/ui_options.c			\
	q3_ui/ui_playermodel.c		\
	q3_ui/ui_players.c			\
	q3_ui/ui_playersettings.c	\
	q3_ui/ui_preferences.c		\
	q3_ui/ui_qmenu.c			\
	q3_ui/ui_removebots.c		\
	q3_ui/ui_saveconfig.c		\
	q3_ui/ui_serverinfo.c		\
	q3_ui/ui_servers2.c			\
	q3_ui/ui_setup.c			\
	q3_ui/ui_sound.c			\
	q3_ui/ui_sparena.c			\
	q3_ui/ui_specifyserver.c	\
	q3_ui/ui_splevel.c			\
	q3_ui/ui_sppostgame.c		\
	q3_ui/ui_spreset.c			\
	q3_ui/ui_spskill.c			\
	q3_ui/ui_syscalls.c			\
	q3_ui/ui_startserver.c		\
	q3_ui/ui_team.c				\
	q3_ui/ui_teamorders.c		\
	q3_ui/ui_video.c			\
	\
	game/bg_lib.c \
	game/bg_misc.c \
	game/q_math.c \
	game/q_shared.c \
	q3_ui/ui.def \
	\

#	q3_ui/ui_specifyleague.c	\
#	q3_ui/ui_login.c			\
#	q3_ui/ui_rankings.c			\
#	q3_ui/ui_rankstatus.c		\
#	q3_ui/ui_signup.c			\

QUAKE3_GAME_SRCS = \
	game/ai_chat.c \
	game/ai_cmd.c \
	game/ai_dmnet.c \
	game/ai_dmq3.c \
	game/ai_main.c \
	game/ai_team.c \
	game/ai_vcmd.c \
	game/bg_lib.c \
	game/bg_misc.c \
	game/bg_pmove.c \
	game/bg_slidemove.c \
	game/g_active.c \
	game/g_arenas.c \
	game/g_bot.c \
	game/g_client.c \
	game/g_cmds.c \
	game/g_combat.c \
	game/g_items.c \
	game/g_main.c \
	game/g_mem.c \
	game/g_misc.c \
	game/g_missile.c \
	game/g_mover.c \
	game/g_session.c \
	game/g_spawn.c \
	game/g_svcmds.c \
	game/g_syscalls.c \
	game/g_target.c \
	game/g_team.c \
	game/g_trigger.c \
	game/g_utils.c \
	game/g_weapon.c \
	game/q_math.c \
	game/q_shared.c \
	\
	game/game.def \

QUAKE3_CGAME_SRCS = \
	game/bg_misc.c \
	game/bg_pmove.c \
	game/bg_slidemove.c \
	game/bg_lib.c \
	game/q_math.c \
	game/q_shared.c \
	\
	cgame/cg_consolecmds.c \
	cgame/cg_draw.c \
	cgame/cg_drawtools.c \
	cgame/cg_effects.c \
	cgame/cg_ents.c \
	cgame/cg_event.c \
	cgame/cg_info.c \
	cgame/cg_localents.c \
	cgame/cg_main.c \
	cgame/cg_marks.c \
	cgame/cg_players.c \
	cgame/cg_playerstate.c \
	cgame/cg_predict.c \
	cgame/cg_scoreboard.c \
	cgame/cg_servercmds.c \
	cgame/cg_snapshot.c \
	cgame/cg_syscalls.c \
	cgame/cg_view.c \
	cgame/cg_weapons.c \
	\
	cgame/cgame.def \

#	game/g_rankings.c \

autohead$(EXESUF): autohead.c
	$(CC_OUTEXE)autohead$(EXESUF) autohead.c

quake3_auto.h: autohead$(EXESUF) $(QUAKE3_SRCS)
	./autohead$(EXESUF) -api QUAKE3_API $(QUAKE3_SRCS) > quake3_auto.h

#		qcommon/q2cm_auto.h client/q2cl_auto.h server/q2sv_auto.h

qcommon/q2cm_auto.h: autohead$(EXESUF) $(QUAKE2_CM_SRCS)
	./autohead$(EXESUF) -api QUAKE2_API $(QUAKE2_CM_SRCS) > qcommon/q2cm_auto.h
client/q2cl_auto.h: autohead$(EXESUF) $(QUAKE2_CL_SRCS)
	./autohead$(EXESUF) -api QUAKE2_API $(QUAKE2_CL_SRCS) > client/q2cl_auto.h
server/q2sv_auto.h: autohead$(EXESUF) $(QUAKE2_SV_SRCS)
	./autohead$(EXESUF) -api QUAKE2_API $(QUAKE2_SV_SRCS) > server/q2sv_auto.h

quake3$(ARCH)$(EXESUF): quake3_auto.h $(QUAKE3_SRCS)
	$(CC_OUTEXE)quake3$(ARCH)$(EXESUF) $(CFLAGS) $(QUAKE3_SRCS) $(LDFLAGS)

quake3$(ARCH)_tkra$(EXESUF): quake3_auto.h $(QUAKE3_SRCS) $(TKRA_SRCS)
	$(CC_OUTEXE)quake3$(ARCH)_tkra$(EXESUF) $(CFLAGS) $(QUAKE3_TKRA_SRCS) \
		$(TKRA_LDFLAGS)

ui$(ARCH)$(DLLSUF): quake3_auto.h $(QUAKE3_Q3UI_SRCS)
	$(CC_OUTDLL)ui$(ARCH)$(DLLSUF) $(CFLAGS) $(QUAKE3_Q3UI_SRCS) \
		$(UI_DLL_LDFLAGS)

qagame$(ARCH)$(DLLSUF): quake3_auto.h $(QUAKE3_GAME_SRCS)
	$(CC_OUTDLL)qagame$(ARCH)$(DLLSUF) $(CFLAGS) $(QUAKE3_GAME_SRCS) \
		$(GAME_DLL_LDFLAGS)

cgame$(ARCH)$(DLLSUF): quake3_auto.h $(QUAKE3_CGAME_SRCS)
	$(CC_OUTDLL)cgame$(ARCH)$(DLLSUF) $(CFLAGS) $(QUAKE3_CGAME_SRCS) \
		$(CGAME_DLL_LDFLAGS)

ref_gl$(DLLSUF): FORCE
	make -C ref_gl -f Makefile$(MKSUF)
	cp ref_gl/ref_gl$(DLLSUF) .
#	cp ref_gl/ref_gl.lib .

# game$(DLLSUF): FORCE
#	make -C game -f Makefile$(MKSUF)
#	make -C gameui -f Makefile$(MKSUF)
#	cp game/*$(DLLSUF) ../baseq2
#	cp game/*.lib .
#	cp gameui/*$(DLLSUF) ../baseq2
#	cp gameui/*.lib .

FORCE:

clean:

rem tksys/instrum.wad=../tk_qsrc/tk_clib/instrum.wd1 

rem jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_qsrc/tk_clib/tkshell.sys autoexec.pf=../tk_qsrc/tk_clib/tkautoexec.pf tkusrimg.wd4=../tk_qsrc/tk_clib/tkusrimg.wd4 doom/doom.exe=../tk_qsrc/doomsrc2/doom_bjx2.exe doom/doom_rv.elf=../tk_qsrc/doomsrc2/doom_rv64.elf doom/doom_xrv.exe=../tk_qsrc/doomsrc2/doom_xrv.exe doom/doomu.wad=../tk_qsrc/doomsrc2/doom.wad quake/quake.exe=../tk_qsrc/quake_b64b.exe quake/glquake.exe=../tk_qsrc/glquake_tkra_b64b.exe quake/id1/pak0lz.wad=../tk_qsrc/id1/pak0lz.wad quake/id1/pak1lz.wad=../tk_qsrc/id1/pak1lz.wad quake/id1/pak2lz.wad=../tk_qsrc/id1/pak2lz.wad dhry.exe=../tk_qsrc/dhry_b64b.exe hexen/hexen.exe=../tk_qsrc/HexSrc/hexen_bjx2.exe hexen/hexen.wad=../tk_qsrc/HexSrc/hexen.wad --rd_map ../tk_qsrc/tk_clib/vxcore/tkshell.sys.map %1 %2 %3 %4 %5 

jx2vm ../tst_jx2boot.bin --rd_add_fl runmulti0_addlist.txt --rd_map ../tk_qsrc/tk_clib/vxcore/tkshell.sys.map %1 %2 %3 %4 %5 

rem cl /Fetst_bt1h.exe bt1h_main.c /Zi /O2
cl /Fetst_bt1h.exe bt1h_main.c /Zi /O2 /arch:SSE2
rem cl /Fetst_bt1h.exe bt1h_main.c /Zi

cl /Febt1hvfw.dll bt1h_vfwdrv.c /Zi /O2 /arch:SSE2 /LD winmm.lib user32.lib driverproc.def
rem cl /Febt1hvfw.dll bt1h_vfwdrv.c /Zi /arch:SSE2 /LD winmm.lib user32.lib driverproc.def

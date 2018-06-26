rem C:\Windows\SysWOW64\rundll32.exe SETUPAPI.DLL,InstallHinfSection DefaultInstall 0 F:\bgb.proj.robowasp\tst_btic1h\bt1hvfw.inf
copy bt1hdrv_conf.txt %HOME%

C:\Windows\SysWOW64\rundll32.exe SETUPAPI.DLL,InstallHinfSection DefaultInstall 0 %CD%\bt1hvfw.inf
copy bt1hdrv_conf.txt %HOME%

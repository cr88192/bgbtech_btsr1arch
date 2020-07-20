@echo off
jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/RastTest/tstrat_bjx2.exe Street256.tga=../tk_ports/RastTest/Street128.tga --rd_map ../tk_ports/RastTest/tstrat_bjx2.exe.map %1 %2 %3 %4 %5 

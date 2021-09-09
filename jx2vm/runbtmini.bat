@echo off
jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/BtMini1/src/tst_btm_bjx2.exe sky1b1.dds=../tk_ports/BtMini1/src/sky1b1.dds atlas0_2a.dds=../tk_ports/BtMini1/src/atlas0_2a.dds --rd_map ../tk_ports/BtMini1/src/tst_btm_bjx2.exe.map %1 %2 %3 %4 %5 

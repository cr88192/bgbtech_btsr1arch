@echo off
jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/PlayVid/playvid_bjx2.exe TestOut_CV0.avi=../tk_ports/PlayVid/BadApple_CV9.avi --rd_map ../tk_ports/PlayVid/playvid_bjx2.exe.map %1 %2 %3 %4 %5 
rem jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/PlayVid/playvid_bjx2.exe TestOut_CV0.avi=../tk_ports/PlayVid/Shadows-LindseyStirling_CV0.avi --rd_map ../tk_ports/PlayVid/playvid_bjx2.exe.map %1 %2 %3 %4 %5 
rem jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/PlayVid/playvid_bjx2.exe TestOut_CV0.avi=../tk_ports/PlayVid/LindseyStirling_TakeFlight_CV1.avi --rd_map ../tk_ports/PlayVid/playvid_bjx2.exe.map %1 %2 %3 %4 %5 
rem jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/PlayVid/playvid_bjx2.exe TestOut_CV0.avi=../tk_ports/PlayVid/TestOut_CV1.avi --rd_map ../tk_ports/PlayVid/playvid_bjx2.exe.map %1 %2 %3 %4 %5 
rem jx2vm ../tst_jx2boot.bin --rd_add bootload.sys=../tk_ports/PlayVid/playvid_bjx2.exe TestOut_CV0.avi=../tk_ports/PlayVid/BadApple3D_CV2.avi --rd_map ../tk_ports/PlayVid/playvid_bjx2.exe.map %1 %2 %3 %4 %5 

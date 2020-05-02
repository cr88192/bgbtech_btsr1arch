#!/bin/bash
verilator -Wall --cc ModTxtNtW.v --exe sim_fbtst0.cpp
make -C obj_dir -f VModTxtNtW.mk
obj_dir/VModTxtNtW ../DE3D0.tga
clang -o vgadec0 vgadec0.c -lm
./vgadec0 sim_fbtst0_pwm.dat

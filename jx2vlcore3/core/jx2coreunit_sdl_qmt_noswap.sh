#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnit.v --exe sim_coreunit0.cpp -CFLAGS "-DUSE_SDL -O3 -DNOSWAP -DDOQMT" -LDFLAGS "-lSDL" -Djx2_xc7a200
make -C obj_dir -f VCoreUnit.mk
obj_dir/VCoreUnit $1 $2 $3 $4

#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnit.v --exe sim_coreunit0.cpp -CFLAGS "-DUSE_SDL -O3" -LDFLAGS "-lSDL"
make -C obj_dir -f VCoreUnit.mk
obj_dir/VCoreUnit $1 $2 $3 $4

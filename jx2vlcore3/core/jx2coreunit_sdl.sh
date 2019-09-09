#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnit.v --exe sim_coreunit0.cpp -CFLAGS "-DUSE_SDL -g" -LDFLAGS "-lSDL"
make -C obj_dir -f VCoreUnit.mk
obj_dir/VCoreUnit

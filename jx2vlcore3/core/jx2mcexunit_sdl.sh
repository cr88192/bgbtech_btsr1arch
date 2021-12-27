#!/bin/sh
rm -rf obj_dir/*
verilator --cc minicore/McExUnit.v --exe sim_exunit0mc.cpp -CFLAGS "-DUSE_SDL -O3" -LDFLAGS "-lSDL"
make -C obj_dir -f VMcExUnit.mk
obj_dir/VMcExUnit

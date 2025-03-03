#!/bin/sh
rm -rf obj_dir/*
verilator --cc ExUnit.v --exe sim_exunit0.cpp -CFLAGS "-DUSE_SDL -O3 -g -fwrapv -fno-strict-aliasing -DUSE_ROM_RBD" -LDFLAGS "-lSDL" -Djx2_xc7s50
make -C obj_dir -f VExUnit.mk
obj_dir/VExUnit

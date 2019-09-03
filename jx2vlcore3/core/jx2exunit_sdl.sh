#!/bin/sh
rm -rf obj_dir/*
verilator --cc ExUnit.v --exe sim_exunit0.cpp -CFLAGS "-DUSE_SDL -g" -LDFLAGS "-lSDL"
make -C obj_dir -f VExUnit.mk
obj_dir/VExUnit

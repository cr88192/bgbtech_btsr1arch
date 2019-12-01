#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2ExUnit.v --exe sim_exunit0.cpp
make -C obj_dir -f VJx2ExUnit.mk
obj_dir/VJx2ExUnit

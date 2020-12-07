#!/bin/sh
rm -rf obj_dir/*
verilator --cc FpuExOp.v --exe sim_fpu0.cpp
make -C obj_dir -f VFpuExOp.mk
obj_dir/VFpuExOp

#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2FpuExOp.v --exe sim_fpu0.cpp
make -C obj_dir -f VJx2FpuExOp.mk
obj_dir/VJx2FpuExOp

#!/bin/sh
rm -rf obj_dir/*
verilator --cc FpuVec4SF.v --exe sim_fpuv4sf.cpp
make -C obj_dir -f VFpuVec4SF.mk
obj_dir/VFpuVec4SF

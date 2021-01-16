#!/bin/sh
rm -rf obj_dir/*
verilator --cc FpuExOpW.v --exe sim_fpuw0.cpp
make -C obj_dir -f VFpuExOpW.mk
obj_dir/VFpuExOpW

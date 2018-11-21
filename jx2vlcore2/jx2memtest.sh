#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2MemCache.v --exe sim_tstmem0.cpp
make -C obj_dir -f VJx2MemCache.mk
obj_dir/VJx2MemCache

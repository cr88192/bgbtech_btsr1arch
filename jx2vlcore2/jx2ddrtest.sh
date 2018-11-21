#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2ModDdr.v --exe sim_ddr0.cpp
make -C obj_dir -f VJx2ModDdr.mk
obj_dir/VJx2ModDdr

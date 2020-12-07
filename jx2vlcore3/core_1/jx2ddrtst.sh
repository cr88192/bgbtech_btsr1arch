#!/bin/sh
rm -rf obj_dir/*
verilator --cc MmiModDdr3.v --exe sim_ddr0.cpp
make -C obj_dir -f VMmiModDdr3.mk
obj_dir/VMmiModDdr3

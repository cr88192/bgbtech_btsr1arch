#!/bin/sh
rm -rf obj_dir/*
verilator --cc ExALU.v --exe sim_alu0.cpp
make -C obj_dir -f VExALU.mk
obj_dir/VExALU

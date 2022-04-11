#!/bin/sh
rm -rf obj_dir/*
verilator --cc ExOpSloMulDiv.v --exe sim_muldiv0.cpp
make -C obj_dir -f VExOpSloMulDiv.mk
obj_dir/VExOpSloMulDiv

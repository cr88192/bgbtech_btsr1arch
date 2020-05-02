#!/bin/sh
rm -rf obj_dir/*
verilator --cc MemL2Dc.v --exe sim_l2dc0.cpp
make -C obj_dir -f VMemL2Dc.mk
obj_dir/VMemL2Dc

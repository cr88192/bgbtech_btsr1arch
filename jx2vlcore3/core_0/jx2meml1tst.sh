#!/bin/sh
rm -rf obj_dir/*
verilator --cc MemL1A.v --exe sim_tstl1a.cpp
make -C obj_dir -f VMemL1A.mk
obj_dir/VMemL1A

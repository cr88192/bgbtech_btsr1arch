#!/bin/sh
rm -rf obj_dir/*
verilator --cc ringbus/RbiMemL1A.v --exe sim_tstl1a_rb.cpp
make -C obj_dir -f VRbiMemL1A.mk
obj_dir/VRbiMemL1A

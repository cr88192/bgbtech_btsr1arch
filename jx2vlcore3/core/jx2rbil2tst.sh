#!/bin/sh
rm -rf obj_dir/*
verilator --cc ringbus/RbiMemL2Dc.v --exe sim_l2dc_rb0.cpp
make -C obj_dir -f VRbiMemL2Dc.mk
obj_dir/VRbiMemL2Dc

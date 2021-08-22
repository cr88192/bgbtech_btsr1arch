#!/bin/sh
rm -rf obj_dir/*
verilator --cc ringbus/RbiMemL2DcWa.v --exe sim_l2dc_rbw0.cpp
make -C obj_dir -f VRbiMemL2DcWa.mk
obj_dir/VRbiMemL2DcWa

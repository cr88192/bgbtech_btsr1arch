#!/bin/sh
rm -rf obj_dir/*
verilator --cc ringbus/RbiMemEwDs.v --exe sim_mem_eds.cpp
make -C obj_dir -f VRbiMemEwDs.mk
obj_dir/VRbiMemEwDs

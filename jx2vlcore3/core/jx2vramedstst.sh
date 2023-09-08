#!/bin/sh
rm -rf obj_dir/*
verilator --cc ringbus/RbiMemVramA.v --exe sim_vram_eds.cpp
make -C obj_dir -f VRbiMemVramA.mk
obj_dir/VRbiMemVramA

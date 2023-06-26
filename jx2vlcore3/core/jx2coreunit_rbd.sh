#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnit.v --exe sim_coreunit0.cpp -Djx2_xc7s50
make -C obj_dir -f VCoreUnit.mk
obj_dir/VCoreUnit

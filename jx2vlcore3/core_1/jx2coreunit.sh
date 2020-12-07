#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnit.v --exe sim_coreunit0.cpp
make -C obj_dir -f VCoreUnit.mk
obj_dir/VCoreUnit

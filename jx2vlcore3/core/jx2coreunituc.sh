#!/bin/sh
rm -rf obj_dir/*
verilator --cc CoreUnitUc.v --exe sim_coreunituc0.cpp
make -C obj_dir -f VCoreUnitUc.mk
obj_dir/VCoreUnitUc

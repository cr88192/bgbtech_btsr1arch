#!/bin/sh
rm -rf obj_dir/*
verilator --cc ModAudPcm.v --exe sim_audpcm.cpp
make -C obj_dir -f VModAudPcm.mk
obj_dir/VModAudPcm

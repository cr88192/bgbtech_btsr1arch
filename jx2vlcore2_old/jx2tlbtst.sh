#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2MmuTlb.v --exe sim_tlbtst0.cpp
make -C obj_dir -f VJx2MmuTlb.mk
obj_dir/VJx2MmuTlb

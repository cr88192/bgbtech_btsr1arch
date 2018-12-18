#!/bin/sh
rm -rf obj_dir/*
verilator --cc Jx2FphfVecOp.v --exe sim_fphf.cpp
make -C obj_dir -f VJx2FphfVecOp.mk
obj_dir/VJx2FphfVecOp

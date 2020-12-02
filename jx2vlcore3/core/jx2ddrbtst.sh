#!/bin/sh
rm -rf obj_dir/*
verilator --cc MmiModDdrB.v --exe sim_ddr1.cpp
make -C obj_dir -f VMmiModDdrB.mk
obj_dir/VMmiModDdrB

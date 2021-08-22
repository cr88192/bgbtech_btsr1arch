#!/bin/sh
rm -rf obj_dir/*
verilator --cc MmiModDdrWa.v --exe sim_ddr0wa.cpp
make -C obj_dir -f VMmiModDdrWa.mk
obj_dir/VMmiModDdrWa

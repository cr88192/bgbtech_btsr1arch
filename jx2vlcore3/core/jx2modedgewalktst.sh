#!/bin/sh
rm -rf obj_dir/*
verilator --cc ModEdgeWalk.v --exe sim_modedgewalk.cpp
make -C obj_dir -f VModEdgeWalk.mk
obj_dir/VModEdgeWalk

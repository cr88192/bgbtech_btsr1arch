// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _VJx2FpuExOp__Syms_H_
#define _VJx2FpuExOp__Syms_H_

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "VJx2FpuExOp.h"
#include "VJx2FpuExOp___024unit.h"

// SYMS CLASS
class VJx2FpuExOp__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    VJx2FpuExOp*                   TOPp;
    
    // CREATORS
    VJx2FpuExOp__Syms(VJx2FpuExOp* topp, const char* namep);
    ~VJx2FpuExOp__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(64);

#endif // guard

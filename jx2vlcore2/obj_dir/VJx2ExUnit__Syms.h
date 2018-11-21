// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _VJx2ExUnit__Syms_H_
#define _VJx2ExUnit__Syms_H_

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "VJx2ExUnit.h"
#include "VJx2ExUnit___024unit.h"

// SYMS CLASS
class VJx2ExUnit__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    VJx2ExUnit*                    TOPp;
    
    // CREATORS
    VJx2ExUnit__Syms(VJx2ExUnit* topp, const char* namep);
    ~VJx2ExUnit__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(64);

#endif // guard

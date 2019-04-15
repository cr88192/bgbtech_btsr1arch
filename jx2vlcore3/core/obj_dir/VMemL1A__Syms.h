// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _VMemL1A__Syms_H_
#define _VMemL1A__Syms_H_

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "VMemL1A.h"
#include "VMemL1A___024unit.h"

// SYMS CLASS
class VMemL1A__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    VMemL1A*                       TOPp;
    
    // CREATORS
    VMemL1A__Syms(VMemL1A* topp, const char* namep);
    ~VMemL1A__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(64);

#endif // guard

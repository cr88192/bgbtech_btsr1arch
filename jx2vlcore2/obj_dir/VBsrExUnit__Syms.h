// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _VBsrExUnit__Syms_H_
#define _VBsrExUnit__Syms_H_

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "VBsrExUnit.h"
#include "VBsrExUnit___024unit.h"

// SYMS CLASS
class VBsrExUnit__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    VBsrExUnit*                    TOPp;
    
    // CREATORS
    VBsrExUnit__Syms(VBsrExUnit* topp, const char* namep);
    ~VBsrExUnit__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(64);

#endif // guard

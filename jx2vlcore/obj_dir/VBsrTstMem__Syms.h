// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _VBsrTstMem__Syms_H_
#define _VBsrTstMem__Syms_H_

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "VBsrTstMem.h"
#include "VBsrTstMem___024unit.h"

// SYMS CLASS
class VBsrTstMem__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    VBsrTstMem*                    TOPp;
    
    // CREATORS
    VBsrTstMem__Syms(VBsrTstMem* topp, const char* namep);
    ~VBsrTstMem__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(64);

#endif // guard

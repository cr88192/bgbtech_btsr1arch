// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VBsrTstMem.h for the primary calling header

#ifndef _VBsrTstMem___024unit_H_
#define _VBsrTstMem___024unit_H_

#include "verilated_heavy.h"

class VBsrTstMem__Syms;

//----------

VL_MODULE(VBsrTstMem___024unit) {
  public:
    
    // PORTS
    
    // LOCAL SIGNALS
    
    // LOCAL VARIABLES
    
    // INTERNAL VARIABLES
  private:
    VBsrTstMem__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VBsrTstMem___024unit);  ///< Copying not allowed
  public:
    VBsrTstMem___024unit(const char* name="TOP");
    ~VBsrTstMem___024unit();
    
    // API METHODS
    
    // INTERNAL METHODS
    void __Vconfigure(VBsrTstMem__Syms* symsp, bool first);
  private:
    void _ctor_var_reset();
} VL_ATTR_ALIGNED(128);

#endif // guard

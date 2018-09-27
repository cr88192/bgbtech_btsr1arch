// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VBsrExUnit.h for the primary calling header

#ifndef _VBsrExUnit___024unit_H_
#define _VBsrExUnit___024unit_H_

#include "verilated_heavy.h"

class VBsrExUnit__Syms;

//----------

VL_MODULE(VBsrExUnit___024unit) {
  public:
    
    // PORTS
    
    // LOCAL SIGNALS
    
    // LOCAL VARIABLES
    
    // INTERNAL VARIABLES
  private:
    VBsrExUnit__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VBsrExUnit___024unit);  ///< Copying not allowed
  public:
    VBsrExUnit___024unit(const char* name="TOP");
    ~VBsrExUnit___024unit();
    
    // API METHODS
    
    // INTERNAL METHODS
    void __Vconfigure(VBsrExUnit__Syms* symsp, bool first);
  private:
    void _ctor_var_reset();
} VL_ATTR_ALIGNED(128);

#endif // guard

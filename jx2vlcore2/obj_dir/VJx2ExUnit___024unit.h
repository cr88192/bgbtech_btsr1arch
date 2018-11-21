// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VJx2ExUnit.h for the primary calling header

#ifndef _VJx2ExUnit___024unit_H_
#define _VJx2ExUnit___024unit_H_

#include "verilated_heavy.h"

class VJx2ExUnit__Syms;

//----------

VL_MODULE(VJx2ExUnit___024unit) {
  public:
    
    // PORTS
    
    // LOCAL SIGNALS
    
    // LOCAL VARIABLES
    
    // INTERNAL VARIABLES
  private:
    VJx2ExUnit__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VJx2ExUnit___024unit);  ///< Copying not allowed
  public:
    VJx2ExUnit___024unit(const char* name="TOP");
    ~VJx2ExUnit___024unit();
    
    // API METHODS
    
    // INTERNAL METHODS
    void __Vconfigure(VJx2ExUnit__Syms* symsp, bool first);
  private:
    void _ctor_var_reset();
} VL_ATTR_ALIGNED(128);

#endif // guard

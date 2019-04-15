// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VMemL1A.h for the primary calling header

#ifndef _VMemL1A___024unit_H_
#define _VMemL1A___024unit_H_

#include "verilated_heavy.h"

class VMemL1A__Syms;

//----------

VL_MODULE(VMemL1A___024unit) {
  public:
    
    // PORTS
    
    // LOCAL SIGNALS
    
    // LOCAL VARIABLES
    
    // INTERNAL VARIABLES
  private:
    VMemL1A__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VMemL1A___024unit);  ///< Copying not allowed
  public:
    VMemL1A___024unit(const char* name="TOP");
    ~VMemL1A___024unit();
    
    // API METHODS
    
    // INTERNAL METHODS
    void __Vconfigure(VMemL1A__Syms* symsp, bool first);
  private:
    void _ctor_var_reset();
} VL_ATTR_ALIGNED(128);

#endif // guard

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VJx2FpuExOp.h for the primary calling header

#ifndef _VJx2FpuExOp___024unit_H_
#define _VJx2FpuExOp___024unit_H_

#include "verilated_heavy.h"

class VJx2FpuExOp__Syms;

//----------

VL_MODULE(VJx2FpuExOp___024unit) {
  public:
    
    // PORTS
    
    // LOCAL SIGNALS
    
    // LOCAL VARIABLES
    
    // INTERNAL VARIABLES
  private:
    VJx2FpuExOp__Syms* __VlSymsp;  // Symbol table
  public:
    
    // PARAMETERS
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VJx2FpuExOp___024unit);  ///< Copying not allowed
  public:
    VJx2FpuExOp___024unit(const char* name="TOP");
    ~VJx2FpuExOp___024unit();
    
    // API METHODS
    
    // INTERNAL METHODS
    void __Vconfigure(VJx2FpuExOp__Syms* symsp, bool first);
  private:
    void _ctor_var_reset();
} VL_ATTR_ALIGNED(128);

#endif // guard

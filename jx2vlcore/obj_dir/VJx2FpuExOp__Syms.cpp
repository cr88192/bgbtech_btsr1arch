// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "VJx2FpuExOp__Syms.h"
#include "VJx2FpuExOp.h"
#include "VJx2FpuExOp___024unit.h"

// FUNCTIONS
VJx2FpuExOp__Syms::VJx2FpuExOp__Syms(VJx2FpuExOp* topp, const char* namep)
	// Setup locals
	: __Vm_namep(namep)
	, __Vm_didInit(false)
	// Setup submodule names
{
    // Pointer to top level
    TOPp = topp;
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOPp->__Vconfigure(this, true);
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "VJx2ExUnit__Syms.h"
#include "VJx2ExUnit.h"
#include "VJx2ExUnit___024unit.h"

// FUNCTIONS
VJx2ExUnit__Syms::VJx2ExUnit__Syms(VJx2ExUnit* topp, const char* namep)
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

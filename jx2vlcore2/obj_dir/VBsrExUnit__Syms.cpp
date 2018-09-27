// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "VBsrExUnit__Syms.h"
#include "VBsrExUnit.h"
#include "VBsrExUnit___024unit.h"

// FUNCTIONS
VBsrExUnit__Syms::VBsrExUnit__Syms(VBsrExUnit* topp, const char* namep)
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

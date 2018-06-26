// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "VBsrTstMem__Syms.h"
#include "VBsrTstMem.h"
#include "VBsrTstMem___024unit.h"

// FUNCTIONS
VBsrTstMem__Syms::VBsrTstMem__Syms(VBsrTstMem* topp, const char* namep)
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

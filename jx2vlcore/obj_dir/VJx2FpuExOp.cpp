// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VJx2FpuExOp.h for the primary calling header

#include "VJx2FpuExOp.h"       // For This
#include "VJx2FpuExOp__Syms.h"


//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(VJx2FpuExOp) {
    VJx2FpuExOp__Syms* __restrict vlSymsp = __VlSymsp = new VJx2FpuExOp__Syms(this, name());
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VJx2FpuExOp::__Vconfigure(VJx2FpuExOp__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

VJx2FpuExOp::~VJx2FpuExOp() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void VJx2FpuExOp::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VJx2FpuExOp::eval\n"); );
    VJx2FpuExOp__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
	VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
	_eval(vlSymsp);
	if (VL_UNLIKELY(++__VclockLoop > 100)) {
	    // About to fail, so enable debug to see what's not settling.
	    // Note you must run make with OPT=-DVL_DEBUG for debug prints.
	    int __Vsaved_debug = Verilated::debug();
	    Verilated::debug(1);
	    __Vchange = _change_request(vlSymsp);
	    Verilated::debug(__Vsaved_debug);
	    VL_FATAL_MT(__FILE__,__LINE__,__FILE__,"Verilated model didn't converge");
	} else {
	    __Vchange = _change_request(vlSymsp);
	}
    } while (VL_UNLIKELY(__Vchange));
}

void VJx2FpuExOp::_eval_initial_loop(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
	_eval_settle(vlSymsp);
	_eval(vlSymsp);
	if (VL_UNLIKELY(++__VclockLoop > 100)) {
	    // About to fail, so enable debug to see what's not settling.
	    // Note you must run make with OPT=-DVL_DEBUG for debug prints.
	    int __Vsaved_debug = Verilated::debug();
	    Verilated::debug(1);
	    __Vchange = _change_request(vlSymsp);
	    Verilated::debug(__Vsaved_debug);
	    VL_FATAL_MT(__FILE__,__LINE__,__FILE__,"Verilated model didn't DC converge");
	} else {
	    __Vchange = _change_request(vlSymsp);
	}
    } while (VL_UNLIKELY(__Vchange));
}

//--------------------
// Internal Methods

VL_INLINE_OPT void VJx2FpuExOp::_combo__TOP__1(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_combo__TOP__1\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->Jx2FpuExOp__DOT__regRcpValRn = ((0x53U 
					     == (IData)(vlTOPp->opCmd))
					     ? vlTOPp->regValRm
					     : vlTOPp->regValRn);
    vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp = ((0x50U 
					     == (IData)(vlTOPp->opCmd))
					     ? 1U : 
					    ((0x51U 
					      == (IData)(vlTOPp->opCmd))
					      ? 2U : 
					     ((0x5aU 
					       == (IData)(vlTOPp->opCmd))
					       ? 3U
					       : 0U)));
}

void VJx2FpuExOp::_settle__TOP__2(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_settle__TOP__2\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->Jx2FpuExOp__DOT__regRcpValRn = ((0x53U 
					     == (IData)(vlTOPp->opCmd))
					     ? vlTOPp->regValRm
					     : vlTOPp->regValRn);
    vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp = ((0x50U 
					     == (IData)(vlTOPp->opCmd))
					     ? 1U : 
					    ((0x51U 
					      == (IData)(vlTOPp->opCmd))
					      ? 2U : 
					     ((0x5aU 
					       == (IData)(vlTOPp->opCmd))
					       ? 3U
					       : 0U)));
    vlTOPp->Jx2FpuExOp__DOT__tRegAddRn = ((0x5aU == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->ctlInDlr
					   : vlTOPp->regValRn);
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B 
	= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3;
    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
		       >> 0x3fU)))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3)));
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
	    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
	       >> 1U);
    } else {
	if ((0U != (0x1ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				      >> 0x36U))))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3;
	} else {
	    if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					 >> 0x2eU))))) {
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
		    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
				 - (IData)(8U)));
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
		    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
		       << 8U);
	    } else {
		if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					     >> 0x26U))))) {
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
			= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
				     - (IData)(0x10U)));
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
			= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
			   << 0x10U);
		} else {
		    if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
						 >> 0x1eU))))) {
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
			    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					 - (IData)(0x18U)));
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
			    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
			       << 0x18U);
		    } else {
			if ((0U != (0xffU & (IData)(
						    (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
						     >> 0x16U))))) {
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
				= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					     - (IData)(0x20U)));
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
				= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				   << 0x20U);
			} else {
			    if ((0U != (0xffU & (IData)(
							(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
							 >> 0xeU))))) {
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
				    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
						 - (IData)(0x28U)));
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
				    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				       << 0x28U);
			    } else {
				if ((0U != (0xffU & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
							     >> 6U))))) {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					    - (IData)(0x30U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					   << 0x30U);
				} else {
				    if ((0U != (0x3fU 
						& (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3)))) {
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
					    = (0xfffU 
					       & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
						  - (IData)(0x38U)));
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
					    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					       << 0x38U);
				    } else {
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B = 0U;
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B = 0U;
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B = VL_ULL(0);
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    // ALWAYS at Jx2FpuAdd.v:98
    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		       >> 0x3eU)))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4;
    } else {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			   >> 0x3dU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
		= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
			     - (IData)(1U)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
		= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		   << 1U);
	} else {
	    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			       >> 0x3cU)))) {
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
		    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
		    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
				 - (IData)(2U)));
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
		    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		       << 2U);
	    } else {
		if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				   >> 0x3bU)))) {
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
			= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
			= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
				     - (IData)(3U)));
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
			= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			   << 3U);
		} else {
		    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				       >> 0x3aU)))) {
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
			    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
			    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					 - (IData)(4U)));
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
			    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			       << 4U);
		    } else {
			if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   >> 0x39U)))) {
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
				= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
				= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					     - (IData)(5U)));
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				   << 5U);
			} else {
			    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					       >> 0x38U)))) {
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
				    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
				    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
						 - (IData)(6U)));
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				       << 6U);
			    } else {
				if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
						   >> 0x37U)))) {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					    - (IData)(7U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   << 7U);
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					    - (IData)(8U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   << 8U);
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if ((0x800U & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B = VL_ULL(0);
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tValC4 = VL_ULL(0);
    } else {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tValC4 
	    = ((((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B)) 
		 << 0x3fU) | (((QData)((IData)((0x7ffU 
						& (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B)))) 
			       << 0x34U) | (VL_ULL(0xfffffffffffff) 
					    & (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					       >> 0xaU)))) 
	       + ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				 >> 9U))) ? VL_ULL(1)
		   : VL_ULL(0)));
    }
    vlTOPp->Jx2FpuExOp__DOT__tRegMulRn = ((0x52U == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->regValRn
					   : ((0x53U 
					       == (IData)(vlTOPp->opCmd))
					       ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						   ? vlTOPp->regValRn
						   : vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp)
					       : vlTOPp->regValRn));
    vlTOPp->Jx2FpuExOp__DOT__tRegMulRm = ((0x52U == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->regValRm
					   : ((0x53U 
					       == (IData)(vlTOPp->opCmd))
					       ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						   ? vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp
						   : vlTOPp->regValRm)
					       : vlTOPp->regValRm));
    vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp = ((0x52U 
					     == (IData)(vlTOPp->opCmd))
					     ? 1U : 
					    ((0x53U 
					      == (IData)(vlTOPp->opCmd))
					      ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						  ? 1U
						  : 
						 ((IData)(vlTOPp->Jx2FpuExOp__DOT__tRegDivStrobe)
						   ? 0U
						   : 3U))
					      : 0U));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_P 
	= ((QData)((IData)((0xfffffffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_AC 
						  >> 8U))))) 
	   + (QData)((IData)((0xfffffffU & (IData)(
						   (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CA 
						    >> 8U))))));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_R 
	= ((QData)((IData)((0xfffffffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BB 
						  >> 8U))))) 
	   + (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CC 
	      << 0x1cU));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_S 
	= ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_P 
	    + vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_Q) 
	   + vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_R);
    // ALWAYS at Jx2FpuMul.v:127
    if ((0x800U & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B = VL_ULL(0);
    } else {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
			   >> 0x3dU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B 
		= (0xfffU & ((IData)(1U) + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B 
		= (VL_ULL(0x3fffffffffffff) & ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
						>> 9U) 
					       + ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
							      >> 8U)))
						   ? VL_ULL(1)
						   : VL_ULL(0))));
	} else {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B 
		= (VL_ULL(0x3fffffffffffff) & ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
						>> 8U) 
					       + ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
							      >> 7U)))
						   ? VL_ULL(1)
						   : VL_ULL(0))));
	}
    }
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tValC4 = 
	(((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B)) 
	  << 0x3fU) | (((QData)((IData)((0x7ffU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B)))) 
			<< 0x34U) | (VL_ULL(0xfffffffffffff) 
				     & vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B)));
    // ALWAYS at Jx2FpuRcpA.v:26
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1 = (1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P 
	= ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
			  >> 0x33U))) ? ((1U & (IData)(
						       (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							>> 0x32U)))
					  ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1fbf7f0U
						    : 0x3efbefcU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x5db22d1U
						    : 0x7bdef7cU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x997ef59U
						    : 0xb68eb04U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0xd30a942U
						    : 0xeeeeeefU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x10a37eedU
						    : 0x124e1a09U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x13ee7ee8U
						    : 0x158469eeU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x170f9526U
						    : 0x188fb824U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1a0487eeU
						    : 0x1b6db6dbU))))
					  : ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1ccaf478U
						    : 0x1e1bed62U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1f604b28U
						    : 0x2097b426U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x21c1cb5dU
						    : 0x22de304dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x23ec7ec8U
						    : 0x24ec4ec5U)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x25dd3432U
						    : 0x26bebebfU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x279079aaU
						    : 0x2851eb85U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x290295fbU
						    : 0x29a1f58dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2a2f8152U
						    : 0x2aaaaaabU)))))
	    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
			      >> 0x32U))) ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2b12dcf8U
						    : 0x2b677d47U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2ba7e9faU
						    : 0x2bd37a6fU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2be97e98U
						    : 0x2be93e94U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2bd1fa3fU
						    : 0x2ba2e8baU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2b5b37e8U
						    : 0x2afa0be8U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2a7e7e7eU
						    : 0x29e79e7aU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x29346f09U
						    : 0x2863e706U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2774f033U
						    : 0x26666666U))))
	        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				  >> 0x31U))) ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x253716afU
						    : 0x23e5be5cU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x227109f9U
						    : 0x20d79436U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1f17e4b1U
						    : 0x1d306eb4U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1b1f8fc8U
						    : 0x18e38e39U)))
		    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				      >> 0x30U))) ? 
		       ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				       >> 0x2fU))) ? 
			((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					>> 0x2eU)))
			  ? 0x167a9770U : 0x13e2be2cU)
			 : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					   >> 0x2eU)))
			     ? 0x1119f894U : 0xe1e1e1eU))
		        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					  >> 0x2fU)))
			    ? ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					      >> 0x2eU)))
			        ? 0xaece541U : 0x783e0f8U)
			    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					      >> 0x2eU)))
			        ? 0x3e07e08U : 0U))))));
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1 
	= ((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P)) 
	   << 0x14U);
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn 
	= (((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1)) 
	    << 0x3fU) | (VL_ULL(0x7fffffffffffffff) 
			 & ((~ vlTOPp->Jx2FpuExOp__DOT__regRcpValRn) 
			    + (VL_ULL(0x7fd0000000000000) 
			       | (VL_ULL(0xfffffffffffff) 
				  & (~ vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1))))));
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1 = 
	(0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK = 0U;
    if (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK 
	    = ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5)
	        ? 1U : 2U);
    }
    // ALWAYS at Jx2FpuAdd.v:98
    if ((3U == (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp))) {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraJ2 
			   >> 0x3fU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 = 1U;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 = 0x43eU;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
		= VL_NEGATE_Q(vlTOPp->Jx2FpuExOp__DOT__tRegAddRn);
	} else {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 = 0U;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 = 0x43eU;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
		= vlTOPp->Jx2FpuExOp__DOT__tRegAddRn;
	}
    } else {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
	    = (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2) 
		== (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2))
	        ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		   + vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2)
	        : (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		   - vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2));
    }
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1 = 
	(1U & ((IData)((vlTOPp->regValRm >> 0x3fU)) 
	       ^ ((IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp) 
		  >> 1U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1 = 
	(0x7ffU & (IData)((vlTOPp->regValRm >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1)))) 
	  << 0x3eU) | (VL_ULL(0x3ffffffffffffc00) & 
		       (vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
			<< 0xaU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1)))) 
	  << 0x3eU) | (VL_ULL(0x3ffffffffffffc00) & 
		       (vlTOPp->regValRm << 0xaU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D 
	= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1) 
		     - (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D 
	= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1) 
		     - (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1D 
	= ((0x34U >= (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D))
	    ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1 
	       >> (0x3fU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D)))
	    : VL_ULL(0));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1D 
	= ((0x34U >= (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D))
	    ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1 
	       >> (0x3fU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D)))
	    : VL_ULL(0));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1 = 
	(1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRn 
		       >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1 = 
	(1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
		       >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC1 = 
	((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1) 
	 ^ (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1));
    // ALWAYS at Jx2FpuRcpA.v:26
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tSgnB1 
	= (1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			 >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P 
	= ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			  >> 0x33U))) ? ((1U & (IData)(
						       (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							>> 0x32U)))
					  ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1fbf7f0U
						    : 0x3efbefcU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x5db22d1U
						    : 0x7bdef7cU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x997ef59U
						    : 0xb68eb04U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0xd30a942U
						    : 0xeeeeeefU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x10a37eedU
						    : 0x124e1a09U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x13ee7ee8U
						    : 0x158469eeU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x170f9526U
						    : 0x188fb824U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1a0487eeU
						    : 0x1b6db6dbU))))
					  : ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1ccaf478U
						    : 0x1e1bed62U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1f604b28U
						    : 0x2097b426U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x21c1cb5dU
						    : 0x22de304dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x23ec7ec8U
						    : 0x24ec4ec5U)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x25dd3432U
						    : 0x26bebebfU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x279079aaU
						    : 0x2851eb85U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x290295fbU
						    : 0x29a1f58dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2a2f8152U
						    : 0x2aaaaaabU)))))
	    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			      >> 0x32U))) ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2b12dcf8U
						    : 0x2b677d47U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2ba7e9faU
						    : 0x2bd37a6fU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2be97e98U
						    : 0x2be93e94U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2bd1fa3fU
						    : 0x2ba2e8baU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2b5b37e8U
						    : 0x2afa0be8U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2a7e7e7eU
						    : 0x29e79e7aU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x29346f09U
						    : 0x2863e706U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2774f033U
						    : 0x26666666U))))
	        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				  >> 0x31U))) ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x253716afU
						    : 0x23e5be5cU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x227109f9U
						    : 0x20d79436U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1f17e4b1U
						    : 0x1d306eb4U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1b1f8fc8U
						    : 0x18e38e39U)))
		    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				      >> 0x30U))) ? 
		       ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				       >> 0x2fU))) ? 
			((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					>> 0x2eU)))
			  ? 0x167a9770U : 0x13e2be2cU)
			 : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					   >> 0x2eU)))
			     ? 0x1119f894U : 0xe1e1e1eU))
		        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					  >> 0x2fU)))
			    ? ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					      >> 0x2eU)))
			        ? 0xaece541U : 0x783e0f8U)
			    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					      >> 0x2eU)))
			        ? 0x3e07e08U : 0U))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1 
	= ((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P)) 
	   << 0x14U);
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRn 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1)))) 
	  << 0x34U) | (VL_ULL(0xfffffffffffff) & vlTOPp->Jx2FpuExOp__DOT__tRegMulRn));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1)))) 
	  << 0x34U) | (VL_ULL(0xfffffffffffff) & vlTOPp->Jx2FpuExOp__DOT__tRegMulRm));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC1 = 
	(0xfffU & (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1) 
		    + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1)) 
		   - (IData)(0x3ffU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1)))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BB 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x12U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x12U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x12U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CA 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CB 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x12U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5 
	= (0x3ffU & ((0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
				       >> 0xdU))) * 
		     (0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
				       >> 0x1fU)))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5 
	= (0x3ffU & ((0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
				       >> 0x1fU))) 
		     * (0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
					 >> 0xdU)))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_Q 
	= (((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BC 
	     << 0xaU) | (QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5))) 
	   + ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CB 
	       << 0xaU) | (QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5))));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1 = vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK = 0U;
    if ((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK 
	    = (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5) 
		== (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp))
	        ? 1U : 2U);
    }
    // ALWAYS at Jx2FpuExOp.v:162
    vlTOPp->Jx2FpuExOp__DOT__tCtlOutDlr = vlTOPp->ctlInDlr;
    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal = VL_ULL(0);
    vlTOPp->Jx2FpuExOp__DOT__tRegOutId = 0x5fU;
    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 0U;
    if ((1U & (~ ((IData)(vlTOPp->opCmd) >> 7U)))) {
	if ((0x40U & (IData)(vlTOPp->opCmd))) {
	    if ((1U & (~ ((IData)(vlTOPp->opCmd) >> 5U)))) {
		if ((0x10U & (IData)(vlTOPp->opCmd))) {
		    if ((8U & (IData)(vlTOPp->opCmd))) {
			if ((1U & (~ ((IData)(vlTOPp->opCmd) 
				      >> 2U)))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->ctlInDlr 
									>> 0xeU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->ctlInDlr 
									    >> 0xdU)))
								 ? 0x7fU
								 : 0U))) 
					       << 0x37U) 
					      | (((QData)((IData)(
								  (7U 
								   & (IData)(
									     (vlTOPp->ctlInDlr 
									      >> 0xaU))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x3ffU 
								     & (IData)(vlTOPp->ctlInDlr)))) 
						    << 0x2aU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->ctlInDlr;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->ctlInDlr 
									>> 0x1eU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->ctlInDlr 
									    >> 0x1dU)))
								 ? 0xfU
								 : 0U))) 
					       << 0x3aU) 
					      | (((QData)((IData)(
								  (0x3fU 
								   & (IData)(
									     (vlTOPp->ctlInDlr 
									      >> 0x17U))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x7fffffU 
								     & (IData)(vlTOPp->ctlInDlr)))) 
						    << 0x1dU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->opCmd))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    if ((1U & (~ ((IData)(vlTOPp->regIdIxt) 
						  >> 7U)))) {
					if ((1U & (~ 
						   ((IData)(vlTOPp->regIdIxt) 
						    >> 6U)))) {
					    if ((1U 
						 & (~ 
						    ((IData)(vlTOPp->regIdIxt) 
						     >> 5U)))) {
						if (
						    (1U 
						     & (~ 
							((IData)(vlTOPp->regIdIxt) 
							 >> 4U)))) {
						    if (
							(1U 
							 & (~ 
							    ((IData)(vlTOPp->regIdIxt) 
							     >> 3U)))) {
							if (
							    (1U 
							     & (~ 
								((IData)(vlTOPp->regIdIxt) 
								 >> 2U)))) {
							    if (
								(2U 
								 & (IData)(vlTOPp->regIdIxt))) {
								if (
								    (1U 
								     & (IData)(vlTOPp->regIdIxt))) {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(((QData)((IData)(
										(1U 
										& (IData)(
										(vlTOPp->regValRn 
										>> 0x3fU))))) 
									  << 0x3fU) 
									 | (VL_ULL(0x7fffffffffffffff) 
									    & (VL_ULL(0x1ff8000000000000) 
									       + 
									       (VL_ULL(0x3fffffffffffffff) 
										& (vlTOPp->regValRn 
										>> 1U)))));
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								} else {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								}
							    } else {
								if (
								    (1U 
								     & (IData)(vlTOPp->regIdIxt))) {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(VL_ULL(0x7fffffffffffffff) 
									 & vlTOPp->regValRn);
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								} else {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(((QData)((IData)(
										(1U 
										& (~ (IData)(
										(vlTOPp->regValRn 
										>> 0x3fU)))))) 
									  << 0x3fU) 
									 | (VL_ULL(0x7fffffffffffffff) 
									    & vlTOPp->regValRn));
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->regValRm;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= (((1U == (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK)) 
					    & (IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable))
					    ? 1U : 2U);
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->opCmd))) {
			if ((4U & (IData)(vlTOPp->opCmd))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->memDataLd;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->memDataLd 
									>> 0x1eU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->memDataLd 
									    >> 0x1dU)))
								 ? 0xfU
								 : 0U))) 
					       << 0x3aU) 
					      | (((QData)((IData)(
								  (0x3fU 
								   & (IData)(
									     (vlTOPp->memDataLd 
									      >> 0x17U))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x7fffffU 
								     & (IData)(vlTOPp->memDataLd)))) 
						    << 0x1dU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    vlTOPp->ctlOutDlr = vlTOPp->Jx2FpuExOp__DOT__tCtlOutDlr;
    vlTOPp->regOutVal = vlTOPp->Jx2FpuExOp__DOT__tRegOutVal;
    vlTOPp->regOutId = vlTOPp->Jx2FpuExOp__DOT__tRegOutId;
    vlTOPp->regOutOK = vlTOPp->Jx2FpuExOp__DOT__tRegOutOK;
}

VL_INLINE_OPT void VJx2FpuExOp::_sequent__TOP__3(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_sequent__TOP__3\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__tMulRcpStable,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed0,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4,1,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2,0,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3,1,0);
    VL_SIG8(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2,1,0);
    VL_SIG16(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3,11,0);
    VL_SIG16(__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2,11,0);
    VL_SIG64(__Vdly__Jx2FpuExOp__DOT__tMulDivRcp,63,0);
    // Body
    __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4 = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4;
    __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3 = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3;
    __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2 = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3;
    __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2 = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2;
    __Vdly__Jx2FpuExOp__DOT__tMulRcpSeed = vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed;
    __Vdly__Jx2FpuExOp__DOT__tMulRcpSeed0 = vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed0;
    __Vdly__Jx2FpuExOp__DOT__tMulDivRcp = vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp;
    __Vdly__Jx2FpuExOp__DOT__tMulRcpStable = vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable;
    // ALWAYS at Jx2FpuAdd.v:291
    if (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1) {
	if (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1) 
	     >= (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2 
		= (1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
				 >> 0x3fU)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1D;
	} else {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2 
		= (1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
				 >> 0x3fU)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1D;
	}
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraJ2 
	    = vlTOPp->Jx2FpuExOp__DOT__tRegAddRn;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tValC4;
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3;
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2;
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1;
    } else {
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2 = 0U;
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3 = 0U;
	__Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4 = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5 = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo = VL_ULL(0);
    }
    // ALWAYS at Jx2FpuExOp.v:302
    if ((0x53U == (IData)(vlTOPp->opCmd))) {
	if (VL_UNLIKELY(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)) {
	    __Vdly__Jx2FpuExOp__DOT__tMulDivRcp = vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp;
	    VL_WRITEF("FPU FDIV: Stable=%x\n",64,vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp);
	} else {
	    if (VL_LIKELY(vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed)) {
		if (VL_UNLIKELY((1U == (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK)))) {
		    __Vdly__Jx2FpuExOp__DOT__tMulDivRcp 
			= (vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp 
			   + (VL_ULL(0x3ff0000000000000) 
			      - vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo));
		    VL_WRITEF("FPU FDIV: Iter=%x Delta=%x\n",
			      64,vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp,
			      64,(VL_ULL(0x3ff0000000000000) 
				  - vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo));
		    if ((VL_ULL(0) == (VL_ULL(0x7ffffffffffff) 
				       & ((VL_ULL(0x3ff0000000000000) 
					   - vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo) 
					  >> 2U)))) {
			__Vdly__Jx2FpuExOp__DOT__tMulRcpStable = 1U;
		    }
		    vlTOPp->Jx2FpuExOp__DOT__tRegDivStrobe = 1U;
		    if ((VL_ULL(0x7ffffffffffff) == 
			 (VL_ULL(0x7ffffffffffff) & 
			  ((VL_ULL(0x3ff0000000000000) 
			    - vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo) 
			   >> 2U)))) {
			__Vdly__Jx2FpuExOp__DOT__tMulRcpStable = 1U;
		    }
		} else {
		    vlTOPp->Jx2FpuExOp__DOT__tRegDivStrobe = 0U;
		}
	    } else {
		VL_WRITEF("FPU FDIV: Seed=%x\n",64,
			  vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn);
		__Vdly__Jx2FpuExOp__DOT__tMulDivRcp 
		    = vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn;
		__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed 
		    = vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed0;
		vlTOPp->Jx2FpuExOp__DOT__tRegDivStrobe = 1U;
		__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed0 = 1U;
	    }
	}
    } else {
	__Vdly__Jx2FpuExOp__DOT__tMulRcpStable = 0U;
	__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed = 0U;
	__Vdly__Jx2FpuExOp__DOT__tMulRcpSeed0 = 0U;
    }
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4 = __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4;
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3 = __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3;
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2 = __Vdly__Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2;
    vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed = __Vdly__Jx2FpuExOp__DOT__tMulRcpSeed;
    vlTOPp->Jx2FpuExOp__DOT__tMulRcpSeed0 = __Vdly__Jx2FpuExOp__DOT__tMulRcpSeed0;
    vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp = __Vdly__Jx2FpuExOp__DOT__tMulDivRcp;
    vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable = __Vdly__Jx2FpuExOp__DOT__tMulRcpStable;
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B 
	= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3;
    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
		       >> 0x3fU)))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3)));
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
	    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
	       >> 1U);
    } else {
	if ((0U != (0x1ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				      >> 0x36U))))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3;
	} else {
	    if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					 >> 0x2eU))))) {
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
		    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
				 - (IData)(8U)));
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
		    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
		       << 8U);
	    } else {
		if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					     >> 0x26U))))) {
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
			= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
				     - (IData)(0x10U)));
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
			= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
			   << 0x10U);
		} else {
		    if ((0U != (0xffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
						 >> 0x1eU))))) {
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
			    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					 - (IData)(0x18U)));
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
			    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
			       << 0x18U);
		    } else {
			if ((0U != (0xffU & (IData)(
						    (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
						     >> 0x16U))))) {
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
				= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					     - (IData)(0x20U)));
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
				= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				   << 0x20U);
			} else {
			    if ((0U != (0xffU & (IData)(
							(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
							 >> 0xeU))))) {
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
				    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
						 - (IData)(0x28U)));
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
				    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
				       << 0x28U);
			    } else {
				if ((0U != (0xffU & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
							     >> 6U))))) {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
					    - (IData)(0x30U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					   << 0x30U);
				} else {
				    if ((0U != (0x3fU 
						& (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3)))) {
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B 
					    = (0xfffU 
					       & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3) 
						  - (IData)(0x38U)));
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B 
					    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 
					       << 0x38U);
				    } else {
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B = 0U;
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B = 0U;
					vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B = VL_ULL(0);
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    // ALWAYS at Jx2FpuAdd.v:98
    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		       >> 0x3eU)))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4;
    } else {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			   >> 0x3dU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
		= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
			     - (IData)(1U)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
		= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		   << 1U);
	} else {
	    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			       >> 0x3cU)))) {
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
		    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
		    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
				 - (IData)(2U)));
		vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
		    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
		       << 2U);
	    } else {
		if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				   >> 0x3bU)))) {
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
			= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
			= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
				     - (IData)(3U)));
		    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
			= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			   << 3U);
		} else {
		    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				       >> 0x3aU)))) {
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
			    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
			    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					 - (IData)(4U)));
			vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
			    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
			       << 4U);
		    } else {
			if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   >> 0x39U)))) {
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
				= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
				= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					     - (IData)(5U)));
			    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				   << 5U);
			} else {
			    if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					       >> 0x38U)))) {
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
				    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
				    = (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
						 - (IData)(6U)));
				vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				    = (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
				       << 6U);
			    } else {
				if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
						   >> 0x37U)))) {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					    - (IData)(7U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   << 7U);
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4;
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B 
					= (0xfffU & 
					   ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4) 
					    - (IData)(8U)));
				    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					= (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 
					   << 8U);
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if ((0x800U & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B = VL_ULL(0);
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tValC4 = VL_ULL(0);
    } else {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tValC4 
	    = ((((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B)) 
		 << 0x3fU) | (((QData)((IData)((0x7ffU 
						& (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B)))) 
			       << 0x34U) | (VL_ULL(0xfffffffffffff) 
					    & (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
					       >> 0xaU)))) 
	       + ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B 
				 >> 9U))) ? VL_ULL(1)
		   : VL_ULL(0)));
    }
    // ALWAYS at Jx2FpuMul.v:275
    if ((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_AC 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AC;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BB 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BB;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BC 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BC;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CA 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CA;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CB 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CB;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CC 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CC;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_P 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_P;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_Q 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_Q;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_R 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_R;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_S;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tValC4;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC1;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC1;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1;
    } else {
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2 = 0U;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3 = 0U;
	__Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4 = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5 = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo = VL_ULL(0);
    }
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2 = __Vdly__Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2;
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_P 
	= ((QData)((IData)((0xfffffffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_AC 
						  >> 8U))))) 
	   + (QData)((IData)((0xfffffffU & (IData)(
						   (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CA 
						    >> 8U))))));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_R 
	= ((QData)((IData)((0xfffffffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BB 
						  >> 8U))))) 
	   + (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CC 
	      << 0x1cU));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_S 
	= ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_P 
	    + vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_Q) 
	   + vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_R);
    // ALWAYS at Jx2FpuMul.v:127
    if ((0x800U & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B = 0U;
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B = VL_ULL(0);
    } else {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
			   >> 0x3dU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B 
		= (0xfffU & ((IData)(1U) + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4)));
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B 
		= (VL_ULL(0x3fffffffffffff) & ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
						>> 9U) 
					       + ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
							      >> 8U)))
						   ? VL_ULL(1)
						   : VL_ULL(0))));
	} else {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B 
		= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B 
		= (VL_ULL(0x3fffffffffffff) & ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
						>> 8U) 
					       + ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S 
							      >> 7U)))
						   ? VL_ULL(1)
						   : VL_ULL(0))));
	}
    }
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tValC4 = 
	(((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B)) 
	  << 0x3fU) | (((QData)((IData)((0x7ffU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B)))) 
			<< 0x34U) | (VL_ULL(0xfffffffffffff) 
				     & vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B)));
}

VL_INLINE_OPT void VJx2FpuExOp::_combo__TOP__4(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_combo__TOP__4\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->Jx2FpuExOp__DOT__tRegAddRn = ((0x5aU == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->ctlInDlr
					   : vlTOPp->regValRn);
    // ALWAYS at Jx2FpuRcpA.v:26
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1 = (1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P 
	= ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
			  >> 0x33U))) ? ((1U & (IData)(
						       (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							>> 0x32U)))
					  ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1fbf7f0U
						    : 0x3efbefcU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x5db22d1U
						    : 0x7bdef7cU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x997ef59U
						    : 0xb68eb04U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0xd30a942U
						    : 0xeeeeeefU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x10a37eedU
						    : 0x124e1a09U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x13ee7ee8U
						    : 0x158469eeU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x170f9526U
						    : 0x188fb824U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1a0487eeU
						    : 0x1b6db6dbU))))
					  : ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1ccaf478U
						    : 0x1e1bed62U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1f604b28U
						    : 0x2097b426U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x21c1cb5dU
						    : 0x22de304dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x23ec7ec8U
						    : 0x24ec4ec5U)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x25dd3432U
						    : 0x26bebebfU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x279079aaU
						    : 0x2851eb85U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x290295fbU
						    : 0x29a1f58dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2a2f8152U
						    : 0x2aaaaaabU)))))
	    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
			      >> 0x32U))) ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2b12dcf8U
						    : 0x2b677d47U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2ba7e9faU
						    : 0x2bd37a6fU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2be97e98U
						    : 0x2be93e94U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2bd1fa3fU
						    : 0x2ba2e8baU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2b5b37e8U
						    : 0x2afa0be8U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2a7e7e7eU
						    : 0x29e79e7aU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x29346f09U
						    : 0x2863e706U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x2774f033U
						    : 0x26666666U))))
	        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				  >> 0x31U))) ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x253716afU
						    : 0x23e5be5cU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x227109f9U
						    : 0x20d79436U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1f17e4b1U
						    : 0x1d306eb4U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
							       >> 0x2eU)))
						    ? 0x1b1f8fc8U
						    : 0x18e38e39U)))
		    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				      >> 0x30U))) ? 
		       ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
				       >> 0x2fU))) ? 
			((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					>> 0x2eU)))
			  ? 0x167a9770U : 0x13e2be2cU)
			 : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					   >> 0x2eU)))
			     ? 0x1119f894U : 0xe1e1e1eU))
		        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					  >> 0x2fU)))
			    ? ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					      >> 0x2eU)))
			        ? 0xaece541U : 0x783e0f8U)
			    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__regRcpValRn 
					      >> 0x2eU)))
			        ? 0x3e07e08U : 0U))))));
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1 
	= ((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P)) 
	   << 0x14U);
    vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn 
	= (((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1)) 
	    << 0x3fU) | (VL_ULL(0x7fffffffffffffff) 
			 & ((~ vlTOPp->Jx2FpuExOp__DOT__regRcpValRn) 
			    + (VL_ULL(0x7fd0000000000000) 
			       | (VL_ULL(0xfffffffffffff) 
				  & (~ vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1))))));
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1 = 
	(0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK = 0U;
    if (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK 
	    = ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5)
	        ? 1U : 2U);
    }
    vlTOPp->Jx2FpuExOp__DOT__tRegMulRn = ((0x52U == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->regValRn
					   : ((0x53U 
					       == (IData)(vlTOPp->opCmd))
					       ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						   ? vlTOPp->regValRn
						   : vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp)
					       : vlTOPp->regValRn));
    vlTOPp->Jx2FpuExOp__DOT__tRegMulRm = ((0x52U == (IData)(vlTOPp->opCmd))
					   ? vlTOPp->regValRm
					   : ((0x53U 
					       == (IData)(vlTOPp->opCmd))
					       ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						   ? vlTOPp->Jx2FpuExOp__DOT__tMulDivRcp
						   : vlTOPp->regValRm)
					       : vlTOPp->regValRm));
    vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp = ((0x52U 
					     == (IData)(vlTOPp->opCmd))
					     ? 1U : 
					    ((0x53U 
					      == (IData)(vlTOPp->opCmd))
					      ? ((IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable)
						  ? 1U
						  : 
						 ((IData)(vlTOPp->Jx2FpuExOp__DOT__tRegDivStrobe)
						   ? 0U
						   : 3U))
					      : 0U));
    // ALWAYS at Jx2FpuAdd.v:98
    if ((3U == (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp))) {
	if ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraJ2 
			   >> 0x3fU)))) {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 = 1U;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 = 0x43eU;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
		= VL_NEGATE_Q(vlTOPp->Jx2FpuExOp__DOT__tRegAddRn);
	} else {
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 = 0U;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 = 0x43eU;
	    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
		= vlTOPp->Jx2FpuExOp__DOT__tRegAddRn;
	}
    } else {
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 
	    = vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2;
	vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 
	    = (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2) 
		== (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2))
	        ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		   + vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2)
	        : (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 
		   - vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2));
    }
    // ALWAYS at Jx2FpuAdd.v:98
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1 = 
	(1U & ((IData)((vlTOPp->regValRm >> 0x3fU)) 
	       ^ ((IData)(vlTOPp->Jx2FpuExOp__DOT__tRegAddExOp) 
		  >> 1U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1 = 
	(0x7ffU & (IData)((vlTOPp->regValRm >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1)))) 
	  << 0x3eU) | (VL_ULL(0x3ffffffffffffc00) & 
		       (vlTOPp->Jx2FpuExOp__DOT__tRegAddRn 
			<< 0xaU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1)))) 
	  << 0x3eU) | (VL_ULL(0x3ffffffffffffc00) & 
		       (vlTOPp->regValRm << 0xaU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D 
	= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1) 
		     - (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D 
	= (0xfffU & ((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1) 
		     - (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1D 
	= ((0x34U >= (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D))
	    ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1 
	       >> (0x3fU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D)))
	    : VL_ULL(0));
    vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1D 
	= ((0x34U >= (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D))
	    ? (vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1 
	       >> (0x3fU & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D)))
	    : VL_ULL(0));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1 = 
	(1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRn 
		       >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1 = 
	(1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
		       >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC1 = 
	((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1) 
	 ^ (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1));
    // ALWAYS at Jx2FpuRcpA.v:26
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tSgnB1 
	= (1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			 >> 0x3fU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P 
	= ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			  >> 0x33U))) ? ((1U & (IData)(
						       (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							>> 0x32U)))
					  ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1fbf7f0U
						    : 0x3efbefcU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x5db22d1U
						    : 0x7bdef7cU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x997ef59U
						    : 0xb68eb04U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0xd30a942U
						    : 0xeeeeeefU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x10a37eedU
						    : 0x124e1a09U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x13ee7ee8U
						    : 0x158469eeU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x170f9526U
						    : 0x188fb824U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1a0487eeU
						    : 0x1b6db6dbU))))
					  : ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1ccaf478U
						    : 0x1e1bed62U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1f604b28U
						    : 0x2097b426U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x21c1cb5dU
						    : 0x22de304dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x23ec7ec8U
						    : 0x24ec4ec5U)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x25dd3432U
						    : 0x26bebebfU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x279079aaU
						    : 0x2851eb85U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x290295fbU
						    : 0x29a1f58dU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2a2f8152U
						    : 0x2aaaaaabU)))))
	    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			      >> 0x32U))) ? ((1U & (IData)(
							   (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							    >> 0x31U)))
					      ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2b12dcf8U
						    : 0x2b677d47U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2ba7e9faU
						    : 0x2bd37a6fU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2be97e98U
						    : 0x2be93e94U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2bd1fa3fU
						    : 0x2ba2e8baU)))
					      : ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2b5b37e8U
						    : 0x2afa0be8U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2a7e7e7eU
						    : 0x29e79e7aU))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x29346f09U
						    : 0x2863e706U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x2774f033U
						    : 0x26666666U))))
	        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				  >> 0x31U))) ? ((1U 
						  & (IData)(
							    (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							     >> 0x30U)))
						  ? 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x253716afU
						    : 0x23e5be5cU)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x227109f9U
						    : 0x20d79436U))
						  : 
						 ((1U 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							      >> 0x2fU)))
						   ? 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1f17e4b1U
						    : 0x1d306eb4U)
						   : 
						  ((1U 
						    & (IData)(
							      (vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
							       >> 0x2eU)))
						    ? 0x1b1f8fc8U
						    : 0x18e38e39U)))
		    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				      >> 0x30U))) ? 
		       ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
				       >> 0x2fU))) ? 
			((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					>> 0x2eU)))
			  ? 0x167a9770U : 0x13e2be2cU)
			 : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					   >> 0x2eU)))
			     ? 0x1119f894U : 0xe1e1e1eU))
		        : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					  >> 0x2fU)))
			    ? ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					      >> 0x2eU)))
			        ? 0xaece541U : 0x783e0f8U)
			    : ((1U & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
					      >> 0x2eU)))
			        ? 0x3e07e08U : 0U))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1 
	= ((QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P)) 
	   << 0x14U);
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRn 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1 = 
	(0x7ffU & (IData)((vlTOPp->Jx2FpuExOp__DOT__tRegMulRm 
			   >> 0x34U)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1)))) 
	  << 0x34U) | (VL_ULL(0xfffffffffffff) & vlTOPp->Jx2FpuExOp__DOT__tRegMulRn));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 = 
	(((QData)((IData)((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1)))) 
	  << 0x34U) | (VL_ULL(0xfffffffffffff) & vlTOPp->Jx2FpuExOp__DOT__tRegMulRm));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC1 = 
	(0xfffU & (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1) 
		    + (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1)) 
		   - (IData)(0x3ffU)));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1)))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BB 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x12U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x12U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x12U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CA 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CB 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x12U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CC 
	= (VL_ULL(0xfffffffff) & ((QData)((IData)((0x3ffffU 
						   & (IData)(
							     (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
							      >> 0x24U))))) 
				  * (QData)((IData)(
						    (0x3ffffU 
						     & (IData)(
							       (vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
								>> 0x24U)))))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5 
	= (0x3ffU & ((0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
				       >> 0xdU))) * 
		     (0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
				       >> 0x1fU)))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5 
	= (0x3ffU & ((0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 
				       >> 0x1fU))) 
		     * (0x1fU & (IData)((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 
					 >> 0xdU)))));
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_Q 
	= (((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BC 
	     << 0xaU) | (QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5))) 
	   + ((vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CB 
	       << 0xaU) | (QData)((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5))));
    // ALWAYS at Jx2FpuMul.v:127
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1 = vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp;
    vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK = 0U;
    if ((0U != (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1))) {
	vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK 
	    = (((IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5) 
		== (IData)(vlTOPp->Jx2FpuExOp__DOT__tRegMulExOp))
	        ? 1U : 2U);
    }
    // ALWAYS at Jx2FpuExOp.v:162
    vlTOPp->Jx2FpuExOp__DOT__tCtlOutDlr = vlTOPp->ctlInDlr;
    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal = VL_ULL(0);
    vlTOPp->Jx2FpuExOp__DOT__tRegOutId = 0x5fU;
    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 0U;
    if ((1U & (~ ((IData)(vlTOPp->opCmd) >> 7U)))) {
	if ((0x40U & (IData)(vlTOPp->opCmd))) {
	    if ((1U & (~ ((IData)(vlTOPp->opCmd) >> 5U)))) {
		if ((0x10U & (IData)(vlTOPp->opCmd))) {
		    if ((8U & (IData)(vlTOPp->opCmd))) {
			if ((1U & (~ ((IData)(vlTOPp->opCmd) 
				      >> 2U)))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->ctlInDlr 
									>> 0xeU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->ctlInDlr 
									    >> 0xdU)))
								 ? 0x7fU
								 : 0U))) 
					       << 0x37U) 
					      | (((QData)((IData)(
								  (7U 
								   & (IData)(
									     (vlTOPp->ctlInDlr 
									      >> 0xaU))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x3ffU 
								     & (IData)(vlTOPp->ctlInDlr)))) 
						    << 0x2aU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->ctlInDlr;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->ctlInDlr 
									>> 0x1eU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->ctlInDlr 
									    >> 0x1dU)))
								 ? 0xfU
								 : 0U))) 
					       << 0x3aU) 
					      | (((QData)((IData)(
								  (0x3fU 
								   & (IData)(
									     (vlTOPp->ctlInDlr 
									      >> 0x17U))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x7fffffU 
								     & (IData)(vlTOPp->ctlInDlr)))) 
						    << 0x1dU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->opCmd))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    if ((1U & (~ ((IData)(vlTOPp->regIdIxt) 
						  >> 7U)))) {
					if ((1U & (~ 
						   ((IData)(vlTOPp->regIdIxt) 
						    >> 6U)))) {
					    if ((1U 
						 & (~ 
						    ((IData)(vlTOPp->regIdIxt) 
						     >> 5U)))) {
						if (
						    (1U 
						     & (~ 
							((IData)(vlTOPp->regIdIxt) 
							 >> 4U)))) {
						    if (
							(1U 
							 & (~ 
							    ((IData)(vlTOPp->regIdIxt) 
							     >> 3U)))) {
							if (
							    (1U 
							     & (~ 
								((IData)(vlTOPp->regIdIxt) 
								 >> 2U)))) {
							    if (
								(2U 
								 & (IData)(vlTOPp->regIdIxt))) {
								if (
								    (1U 
								     & (IData)(vlTOPp->regIdIxt))) {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(((QData)((IData)(
										(1U 
										& (IData)(
										(vlTOPp->regValRn 
										>> 0x3fU))))) 
									  << 0x3fU) 
									 | (VL_ULL(0x7fffffffffffffff) 
									    & (VL_ULL(0x1ff8000000000000) 
									       + 
									       (VL_ULL(0x3fffffffffffffff) 
										& (vlTOPp->regValRn 
										>> 1U)))));
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								} else {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= vlTOPp->Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								}
							    } else {
								if (
								    (1U 
								     & (IData)(vlTOPp->regIdIxt))) {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(VL_ULL(0x7fffffffffffffff) 
									 & vlTOPp->regValRn);
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								} else {
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
									= 
									(((QData)((IData)(
										(1U 
										& (~ (IData)(
										(vlTOPp->regValRn 
										>> 0x3fU)))))) 
									  << 0x3fU) 
									 | (VL_ULL(0x7fffffffffffffff) 
									    & vlTOPp->regValRn));
								    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
									= vlTOPp->regIdRn;
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK = 1U;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->regValRm;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= (((1U == (IData)(vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK)) 
					    & (IData)(vlTOPp->Jx2FpuExOp__DOT__tMulRcpStable))
					    ? 1U : 2U);
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				} else {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutOK 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->opCmd))) {
			if ((4U & (IData)(vlTOPp->opCmd))) {
			    if ((2U & (IData)(vlTOPp->opCmd))) {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= vlTOPp->memDataLd;
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->opCmd))) {
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutVal 
					= (((QData)((IData)(
							    (3U 
							     & (IData)(
								       (vlTOPp->memDataLd 
									>> 0x1eU))))) 
					    << 0x3eU) 
					   | (((QData)((IData)(
							       ((1U 
								 & (IData)(
									   (vlTOPp->memDataLd 
									    >> 0x1dU)))
								 ? 0xfU
								 : 0U))) 
					       << 0x3aU) 
					      | (((QData)((IData)(
								  (0x3fU 
								   & (IData)(
									     (vlTOPp->memDataLd 
									      >> 0x17U))))) 
						  << 0x34U) 
						 | ((QData)((IData)(
								    (0x7fffffU 
								     & (IData)(vlTOPp->memDataLd)))) 
						    << 0x1dU))));
				    vlTOPp->Jx2FpuExOp__DOT__tRegOutId 
					= vlTOPp->regIdRn;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    vlTOPp->ctlOutDlr = vlTOPp->Jx2FpuExOp__DOT__tCtlOutDlr;
    vlTOPp->regOutVal = vlTOPp->Jx2FpuExOp__DOT__tRegOutVal;
    vlTOPp->regOutId = vlTOPp->Jx2FpuExOp__DOT__tRegOutId;
    vlTOPp->regOutOK = vlTOPp->Jx2FpuExOp__DOT__tRegOutOK;
}

void VJx2FpuExOp::_eval(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_eval\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__3(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void VJx2FpuExOp::_eval_initial(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_eval_initial\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VJx2FpuExOp::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::final\n"); );
    // Variables
    VJx2FpuExOp__Syms* __restrict vlSymsp = this->__VlSymsp;
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VJx2FpuExOp::_eval_settle(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_eval_settle\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

VL_INLINE_OPT QData VJx2FpuExOp::_change_request(VJx2FpuExOp__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_change_request\n"); );
    VJx2FpuExOp* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VJx2FpuExOp::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clock & 0xfeU))) {
	Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((reset & 0xfeU))) {
	Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((regIdRm & 0x80U))) {
	Verilated::overWidthError("regIdRm");}
    if (VL_UNLIKELY((regIdRn & 0x80U))) {
	Verilated::overWidthError("regIdRn");}
    if (VL_UNLIKELY((memDataOK & 0xfcU))) {
	Verilated::overWidthError("memDataOK");}
}
#endif // VL_DEBUG

void VJx2FpuExOp::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VJx2FpuExOp::_ctor_var_reset\n"); );
    // Body
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    opCmd = VL_RAND_RESET_I(8);
    regIdIxt = VL_RAND_RESET_I(8);
    regIdRm = VL_RAND_RESET_I(7);
    regIdRn = VL_RAND_RESET_I(7);
    regValRm = VL_RAND_RESET_Q(64);
    regValRn = VL_RAND_RESET_Q(64);
    regOutVal = VL_RAND_RESET_Q(64);
    regOutId = VL_RAND_RESET_I(7);
    regOutOK = VL_RAND_RESET_I(2);
    memDataLd = VL_RAND_RESET_Q(64);
    memDataOK = VL_RAND_RESET_I(2);
    ctlInDlr = VL_RAND_RESET_Q(64);
    ctlOutDlr = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegOutVal = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegOutId = VL_RAND_RESET_I(7);
    Jx2FpuExOp__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__tCtlOutDlr = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegAddRn = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegAddExOp = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__tRegMulRm = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegMulRn = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tMulDivRcp = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tMulRcpStable = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__tMulRcpSeed = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__tMulRcpSeed0 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__tRegMulExOp = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__regRcpValRn = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__tRegDivStrobe = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1D = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1D = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraJ2 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4 = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tValC4 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1 = VL_RAND_RESET_I(11);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC1 = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1 = VL_RAND_RESET_Q(54);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1 = VL_RAND_RESET_Q(54);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BB = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CA = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CB = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5 = VL_RAND_RESET_I(10);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5 = VL_RAND_RESET_I(10);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2 = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_AC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BB = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CA = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CB = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CC = VL_RAND_RESET_Q(36);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_P = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_Q = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_R = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3 = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_P = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_Q = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_R = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_S = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4 = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B = VL_RAND_RESET_I(12);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B = VL_RAND_RESET_Q(54);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tValC4 = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1 = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2 = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3 = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4 = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5 = VL_RAND_RESET_I(2);
    Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tSgnB1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P = VL_RAND_RESET_I(32);
    Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1 = VL_RAND_RESET_Q(52);
    Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn = VL_RAND_RESET_Q(64);
    Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1 = VL_RAND_RESET_I(1);
    Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P = VL_RAND_RESET_I(32);
    Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1 = VL_RAND_RESET_Q(52);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

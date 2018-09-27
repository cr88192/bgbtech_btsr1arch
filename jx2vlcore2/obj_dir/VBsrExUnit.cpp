// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VBsrExUnit.h for the primary calling header

#include "VBsrExUnit.h"        // For This
#include "VBsrExUnit__Syms.h"


//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(VBsrExUnit) {
    VBsrExUnit__Syms* __restrict vlSymsp = __VlSymsp = new VBsrExUnit__Syms(this, name());
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VBsrExUnit::__Vconfigure(VBsrExUnit__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

VBsrExUnit::~VBsrExUnit() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void VBsrExUnit::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VBsrExUnit::eval\n"); );
    VBsrExUnit__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void VBsrExUnit::_eval_initial_loop(VBsrExUnit__Syms* __restrict vlSymsp) {
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

void VBsrExUnit::_settle__TOP__1(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_settle__TOP__1\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp3,127,0,4);
    // Body
    vlTOPp->memOutData[0U] = 0U;
    vlTOPp->memOutData[1U] = 0U;
    vlTOPp->memOutData[2U] = 0U;
    vlTOPp->memOutData[3U] = 0U;
    vlTOPp->memAddr = 0U;
    vlTOPp->memOpm = 0U;
    vlTOPp->mmioOutData = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOutData;
    vlTOPp->mmioAddr = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioAddr;
    vlTOPp->mmioOpm = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOpm;
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPq = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp;
    // ALWAYS at BsrIcTileB.v:51
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkHi = 
	((1U & vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr) 
	 != (1U & (vlTOPp->BsrExUnit__DOT__icRegInPc 
		   >> 3U)));
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx 
	= (((IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkHi) 
	    << 2U) | (3U & (vlTOPp->BsrExUnit__DOT__icRegInPc 
			    >> 1U)));
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkMiss 
	= (1U & (((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr 
		   != (0x1ffffU & (vlTOPp->BsrExUnit__DOT__icRegInPc 
				   >> 3U))) & (((IData)(1U) 
						+ vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr) 
					       != (0x1ffffU 
						   & (vlTOPp->BsrExUnit__DOT__icRegInPc 
						      >> 3U)))) 
		 | (~ (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkReady))));
    if (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkMiss) {
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK = 2U;
    } else {
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK = 1U;
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcVal 
	    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
	        ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[4U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U])
		    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U]))
	        : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U])
		    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[0U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[0U])));
    }
    // ALWAYS at BsrDecOp.v:56
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegO_Dfl 
	= (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
		   >> 8U));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl 
	= (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
		   >> 4U));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl 
	= (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr = 
	(0x10U | (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
			  >> 4U)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Cr = 
	(0x10U | (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr = 
	(0x20U | (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
			  >> 4U)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Sr = 
	(0x20U | (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 1U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x3fU;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
    if (((((((((0U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
	       | (0x100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	      | (0x200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	     | (0x300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	    | (0x400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	   | (0x500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	  | (0x600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	 | (0x700U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
	if ((0U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
	} else {
	    if ((0x100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
	    } else {
		if ((0x200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
		} else {
		    if ((0x300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
		    } else {
			if ((0x400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
			} else {
			    if ((0x500U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			    } else {
				if ((0x600U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
				}
			    }
			}
		    }
		}
	    }
	}
    } else {
	if (((((((((0x800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
		   | (0x900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		  | (0xa00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		 | (0xb00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		| (0xc00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	       | (0xd00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	      | (0xe00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	     | (0xf00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
	    if ((0x800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
	    } else {
		if ((0x900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
		} else {
		    if ((0xa00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
		    } else {
			if ((0xb00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
			} else {
			    if ((0xc00U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
			    } else {
				if ((0xd00U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				} else {
				    if ((0xe00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
				    }
				}
			    }
			}
		    }
		}
	    }
	} else {
	    if (((((((((0x1000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
		       | (0x1100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		      | (0x1200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		     | (0x1300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		    | (0x1400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		   | (0x1500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		  | (0x1600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		 | (0x1700U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
		if ((0x1000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		} else {
		    if ((0x1100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x11U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		    } else {
			if ((0x1200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x12U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			} else {
			    if ((0x1300U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x13U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			    } else {
				if ((0x1400U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x14U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				} else {
				    if ((0x1500U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x15U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    } else {
					if ((0x1600U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x16U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					} else {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x17U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					}
				    }
				}
			    }
			}
		    }
		}
	    } else {
		if (((((((((0x1800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
			   | (0x1900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			  | (0x1a00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			 | (0x1b00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			| (0x1c00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		       | (0x1d00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		      | (0x1e00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		     | (0x1f00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
		    if ((0x1800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		    } else {
			if ((0x1900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x19U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			} else {
			    if ((0x1a00U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
			    } else {
				if ((0x1b00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
				} else {
				    if ((0x1c00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    } else {
					if ((0x1d00U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1dU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					} else {
					    if ((0x1e00U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					    } else {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x18U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					    }
					}
				    }
				}
			    }
			}
		    }
		} else {
		    if (((((((((0x2000U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
			       | (0x2100U == (0xff00U 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			      | (0x2200U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			     | (0x2300U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			    | (0x2400U == (0xff00U 
					   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			   | (0x2500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			  | (0x2600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			 | (0x2800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
			if ((0x2000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			} else {
			    if ((0x2100U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			    } else {
				if ((0x2200U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				} else {
				    if ((0x2300U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				    } else {
					if ((0x2400U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 6U;
					} else {
					    if ((0x2500U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 7U;
					    } else {
						if (
						    (0x2600U 
						     == 
						     (0xff00U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1bU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 6U;
						} else {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1eU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
						}
					    }
					}
				    }
				}
			    }
			}
		    } else {
			if (((((((((0x2900U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				   | (0x2a00U == (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				  | (0x2b00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				 | (0x2c00U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				| (0x2d00U == (0xff00U 
					       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			       | (0x2e00U == (0xff00U 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			      | (0x2f00U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			     | (0x3000U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    if ((0x2900U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1eU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
			    } else {
				if ((0x2a00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1fU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				} else {
				    if ((0x2b00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1fU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
				    } else {
					if ((0x2c00U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
					} else {
					    if ((0x2d00U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xfU;
					    } else {
						if (
						    (0x2e00U 
						     == 
						     (0xff00U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
						} else {
						    if (
							(0x2f00U 
							 == 
							 (0xff00U 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1fU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
						    } else {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    }
						}
					    }
					}
				    }
				}
			    }
			} else {
			    if (((((((((0x3010U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)) 
				       | (0x3020U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				      | (0x3030U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				     | (0x3040U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				    | (0x3050U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				   | (0x3060U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				  | (0x3070U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				 | (0x3080U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				if ((0x3010U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
				} else {
				    if ((0x3020U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
				    } else {
					if ((0x3030U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 3U;
					} else {
					    if ((0x3040U 
						 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
					    } else {
						if (
						    (0x3050U 
						     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 5U;
						} else {
						    if (
							(0x3060U 
							 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 6U;
						    } else {
							if (
							    (0x3070U 
							     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 7U;
							} else {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
							}
						    }
						}
					    }
					}
				    }
				}
			    } else {
				if (((((((((0x3090U 
					    == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)) 
					   | (0x30a0U 
					      == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					  | (0x30b0U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					 | (0x30c0U 
					    == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					| (0x3002U 
					   == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				       | (0x3012U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				      | (0x3022U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				     | (0x3032U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    if ((0x3090U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 9U;
				    } else {
					if ((0x30a0U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x45U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0U;
					} else {
					    if ((0x30b0U 
						 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x45U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0xffffU;
					    } else {
						if (
						    (0x30c0U 
						     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0xcU;
						} else {
						    if (
							(0x3002U 
							 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
						    } else {
							if (
							    (0x3012U 
							     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							} else {
							    if (
								(0x3022U 
								 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							    } else {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    if (((((((((0x3004U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					       | (0x3005U 
						  == 
						  (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					      | (0x3006U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					     | (0x3007U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					    | (0x3008U 
					       == (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					   | (0x3009U 
					      == (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					  | (0x300aU 
					     == (0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					 | (0x300bU 
					    == (0xff0fU 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
					if ((0x3004U 
					     == (0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					} else {
					    if ((0x3005U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					    } else {
						if (
						    (0x3006U 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						} else {
						    if (
							(0x3007U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						    } else {
							if (
							    (0x3008U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x24U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
							} else {
							    if (
								(0x3009U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x24U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
							    } else {
								if (
								    (0x300aU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x26U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
								} else {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x26U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
								}
							    }
							}
						    }
						}
					    }
					}
				    } else {
					if ((((((((
						   (0x300cU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
						   | (0x300dU 
						      == 
						      (0xff0fU 
						       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						  | (0x300eU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						 | (0x300fU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						| (0x3100U 
						   == 
						   (0xff0fU 
						    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					       | (0x3101U 
						  == 
						  (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					      | (0x3102U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					     | (0x3103U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
					    if ((0x300cU 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					    } else {
						if (
						    (0x300dU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						} else {
						    if (
							(0x300eU 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						    } else {
							if (
							    (0x300fU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
							} else {
							    if (
								(0x3100U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x3101U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x11U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								} else {
								    if (
									(0x3102U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x12U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								    } else {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x13U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								    }
								}
							    }
							}
						    }
						}
					    }
					} else {
					    if ((((
						   (((((0x3104U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
						       | (0x3105U 
							  == 
							  (0xff0fU 
							   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						      | (0x3106U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						     | (0x3107U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						    | (0x3108U 
						       == 
						       (0xff0fU 
							& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						   | (0x3109U 
						      == 
						      (0xff0fU 
						       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						  | (0x310aU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						 | (0x310bU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
						if (
						    (0x3104U 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x14U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						} else {
						    if (
							(0x3105U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x15U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						    } else {
							if (
							    (0x3106U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x16U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							} else {
							    if (
								(0x3107U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x17U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x3108U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3109U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1fU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x310aU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
									} else {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
									}
								    }
								}
							    }
							}
						    }
						}
					    } else {
						if (
						    ((((((((0x310cU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
							   | (0x310dU 
							      == 
							      (0xff0fU 
							       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							  | (0x310eU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							 | (0x310fU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							| (0x3200U 
							   == 
							   (0xff0fU 
							    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						       | (0x3201U 
							  == 
							  (0xff0fU 
							   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						      | (0x3202U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						     | (0x3203U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
						    if (
							(0x310cU 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						    } else {
							if (
							    (0x310dU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1dU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							} else {
							    if (
								(0x310eU 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x310fU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
								} else {
								    if (
									(0x3200U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x28U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3201U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x29U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									} else {
									    if (
										(0x3202U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    }
									}
								    }
								}
							    }
							}
						    }
						} else {
						    if (
							((((((((0x3204U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
							       | (0x3205U 
								  == 
								  (0xff0fU 
								   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							      | (0x3206U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							     | (0x3207U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							    | (0x3208U 
							       == 
							       (0xff0fU 
								& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							   | (0x3209U 
							      == 
							      (0xff0fU 
							       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							  | (0x320aU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							 | (0x320bU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
							if (
							    (0x3204U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2cU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							} else {
							    if (
								(0x3205U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2dU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							    } else {
								if (
								    (0x3206U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2eU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3207U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2fU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3208U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1aU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
									} else {
									    if (
										(0x3209U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
									    } else {
										if (
										(0x320aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
										}
									    }
									}
								    }
								}
							    }
							}
						    } else {
							if (
							    ((((((((0x320cU 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
								   | (0x320dU 
								      == 
								      (0xff0fU 
								       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								  | (0x320eU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								 | (0x320fU 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								| (0x3300U 
								   == 
								   (0xff0fU 
								    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							       | (0x3301U 
								  == 
								  (0xff0fU 
								   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							      | (0x3302U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							     | (0x3304U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
							    if (
								(0x320cU 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
							    } else {
								if (
								    (0x320dU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
								} else {
								    if (
									(0x320eU 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
								    } else {
									if (
									    (0x320fU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
									} else {
									    if (
										(0x3300U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x30U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										if (
										(0x3301U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x31U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x3302U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x32U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x34U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										}
										}
									    }
									}
								    }
								}
							    }
							} else {
							    if (
								((((((((0x3305U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
								       | (0x3306U 
									  == 
									  (0xff0fU 
									   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								      | (0x3307U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								     | (0x3308U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								    | (0x3309U 
								       == 
								       (0xff0fU 
									& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								   | (0x330aU 
								      == 
								      (0xff0fU 
								       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								  | (0x3400U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								 | (0x3401U 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
								if (
								    (0x3305U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x35U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3306U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x36U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3307U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x37U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									} else {
									    if (
										(0x3308U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x38U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										if (
										(0x3309U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x39U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x330aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x3400U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										}
										}
										}
									    }
									}
								    }
								}
							    } else {
								if (
								    ((((((((0x3402U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
									   | (0x3403U 
									      == 
									      (0xff0fU 
									       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									  | (0x3404U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									 | (0x3405U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									| (0x3406U 
									   == 
									   (0xff0fU 
									    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								       | (0x3407U 
									  == 
									  (0xff0fU 
									   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								      | (0x3408U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								     | (0x3409U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
								    if (
									(0x3402U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
								    } else {
									if (
									    (0x3403U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
									} else {
									    if (
										(0x3404U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
									    } else {
										if (
										(0x3405U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										} else {
										if (
										(0x3406U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
										} else {
										if (
										(0x3407U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
										} else {
										if (
										(0x3408U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										}
										}
										}
										}
									    }
									}
								    }
								} else {
								    if (
									((((((((0x340aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
									       | (0x340bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									      | (0x340cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									     | (0x340dU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									    | (0x340eU 
									       == 
									       (0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									   | (0x340fU 
									      == 
									      (0xff0fU 
									       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									  | (0x3500U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									 | (0x3501U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
									if (
									    (0x340aU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
									} else {
									    if (
										(0x340bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
									    } else {
										if (
										(0x340cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340dU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340eU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340fU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x33U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x1fU;
										} else {
										if (
										(0x3500U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										}
										}
										}
										}
										}
									    }
									}
								    } else {
									if (
									    ((((((((0x3502U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x3503U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x3508U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x3509U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x350aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									       | (0x350bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									      | (0x360cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									     | (0x3f00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
									    if (
										(0x3502U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
									    } else {
										if (
										(0x3503U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x3508U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x3509U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x350aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
										} else {
										if (
										(0x350bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x360cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										}
										}
										}
										}
										}
									    }
									} else {
									    if (
										((((((((0x4000U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x4100U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4200U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4300U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4400U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4500U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4600U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4700U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x4000U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4100U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4200U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4300U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4400U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4500U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4600U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										}
										}
										}
										}
										}
										}
										}
									    } else {
										if (
										((((((((0x4800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x4900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4d00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4e00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4f00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x4800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										if (
										(0x4900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 7U;
										} else {
										if (
										(0x4a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										if (
										(0x4b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 7U;
										} else {
										if (
										(0x4c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x4d00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x4e00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 3U;
										}
										}
										}
										}
										}
										}
										}
										} else {
										if (
										((((((((0x5800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x5900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x8000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0xa000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0xb000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x5800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x40U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x41U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x42U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x43U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x44U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x8000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0x800U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										}
										} else {
										if (
										(0xa000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x16U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x17U;
										}
										}
										}
										}
										}
										}
										}
										} else {
										if (
										(0xc000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 5U;
										} else {
										if (
										(0xe000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 5U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 5U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 1U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0U;
										}
										}
										}
										}
									    }
									}
								    }
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0xfU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x28U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((0x10U & ((IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty) 
					 << 2U)) | 
			       (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0xfU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x20U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((0x10U & ((IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty) 
					 << 2U)) | 
			       (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			} else {
			    if ((7U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    } else {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				    = (0x38U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			    }
			}
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x30U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))
			        ? 0x17U : 0x16U);
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))
			        ? 0x17U : 0x16U);
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (8U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty;
		    }
		}
	    }
	} else {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffff000U | (0xfffU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x28U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    } else {
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x14U;
			}
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= (0x38U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			}
		    }
		}
	    }
	}
    } else {
	if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffffff0U | (0xfU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x2aU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (7U & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
					 >> 8U));
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= (8U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			}
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x22U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (7U & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
					 >> 8U));
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty;
			}
		    }
		}
	    }
	} else {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xffffff00U | (0xffU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegO_Dfl;
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Sr;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Cr;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
				}
			    }
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			}
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
		    } else {
			if ((4U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			} else {
			    if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    } else {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    }
			}
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx;
		    }
		} else {
		    if (VL_LIKELY((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx;
		    } else {
			VL_WRITEF("BsrDecOp: Inv %x\n",
				  16,vlTOPp->BsrExUnit__DOT__decIstrWord);
		    }
		}
	    }
	}
    }
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__icMemPcOK = 2U;
    vlTOPp->BsrExUnit__DOT__dcMemPcOK = 2U;
    vlTOPp->BsrExUnit__DOT__icMemPcData[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrExUnit__DOT__memRegInAddr = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInOpm = 0U;
    if ((0U != (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm))) {
	vlTOPp->BsrExUnit__DOT__icMemPcData[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
	vlTOPp->BsrExUnit__DOT__icMemPcOK = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK;
	vlTOPp->BsrExUnit__DOT__memRegInAddr = vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcAddr;
	vlTOPp->BsrExUnit__DOT__memRegInData[0U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[1U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[2U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[3U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInOpm = vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm;
    } else {
	if ((0U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm))) {
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[1U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[2U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[3U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcOK = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK;
	    vlTOPp->BsrExUnit__DOT__memRegInAddr = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr;
	    vlTOPp->BsrExUnit__DOT__memRegInData[0U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[1U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[2U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[3U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U];
	    vlTOPp->BsrExUnit__DOT__memRegInOpm = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm;
	} else {
	    vlTOPp->BsrExUnit__DOT__icMemPcOK = 0U;
	    vlTOPp->BsrExUnit__DOT__dcMemPcOK = 0U;
	}
    }
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutVbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr;
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutGbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr;
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutTbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr;
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqA = 
	((vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA 
	  + (0xffff0000U & ((IData)((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB))) 
			    << 0x10U))) + (0xffff0000U 
					   & ((IData)((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC))) 
					      << 0x10U)));
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqB = (IData)(
							   ((((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA)) 
								+ 
								((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB)) 
								 << 0x10U)) 
							       + 
							       ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC)) 
								<< 0x10U)) 
							      + 
							      ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpD)) 
							       << 0x20U)) 
							     + 
							     ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpE)) 
							      << 0x20U)) 
							    >> 0x20U));
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__exRegNextPc = ((IData)(2U) 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPc);
    vlTOPp->BsrExUnit__DOT__exCtlInPc = vlTOPp->BsrExUnit__DOT__exRegNextPc;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam 
	= ((0xc000U <= vlTOPp->BsrExUnit__DOT__memRegInAddr) 
	   & (0xe000U > vlTOPp->BsrExUnit__DOT__memRegInAddr));
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc = vlTOPp->BsrExUnit__DOT__exCtlInPc;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDhr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId = 0x3fU;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 1U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt = (
						   (0x80000000U 
						    & vlTOPp->BsrExUnit__DOT__exRegValRn)
						    ? 0xffffffffU
						    : 0U);
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt = (0xfU 
						   & ((IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt) 
						      >> 4U));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty = (7U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaRbVal = 
	((8U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
	  ? vlTOPp->BsrExUnit__DOT__exRegValRm : vlTOPp->BsrExUnit__DOT__exRegValRn);
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal = 
	((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	  ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	      ? ((0xfffffff0U & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
				 << 4U)) | (0xfU & vlTOPp->BsrExUnit__DOT__exImmValRi))
	      : vlTOPp->BsrExUnit__DOT__exImmValRi)
	  : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	      ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr
	      : 0U));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal2 
	= ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	        ? (0xfffffff8U & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
				  << 3U)) : (0xfffffffcU 
					     & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
						<< 2U)))
	    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	        ? (0xfffffffeU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
				  << 1U)) : vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr = 
	(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaRbVal 
	 + vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal2);
    if ((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
	vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
	vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
	if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
	    VL_WRITEF("BsrExOp: Missing %x\n",8,vlTOPp->BsrExUnit__DOT__exOpCmd);
	}
    } else {
	if ((0x40U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
	    if ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
		    VL_WRITEF("BsrExOp: Missing %x\n",
			      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
		}
		vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
		vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
	    } else {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
			VL_WRITEF("BsrExOp: Missing %x\n",
				  8,vlTOPp->BsrExUnit__DOT__exOpCmd);
		    }
		    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
		    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
			    VL_WRITEF("BsrExOp: Missing %x\n",
				      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
			}
			vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
			vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if (VL_UNLIKELY((1U 
						 != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
				    VL_WRITEF("BsrExOp: Missing %x\n",
					      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
				}
				vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= ((0xffff0000U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
					       << 0x10U)) 
					   | (0xffffU 
					      & vlTOPp->BsrExUnit__DOT__exImmValRi));
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   ^ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   | vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   - vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		}
	    }
	} else {
	    if ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xffff0000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x10U)) 
						      | (0xffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 0x10U)))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xff000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x18U)) 
						      | (0xffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 8U))))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xf0000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1cU)) 
						      | (0xfffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 4U)))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xc0000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1eU)) 
						      | (0x3fffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 2U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     ((0x80000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1fU)) 
						      | (0x7fffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 1U)))
						      : 0U)))));
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 0x10U))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 8U)))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 4U))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0x3fffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 2U)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     (0x7fffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 1U))
						      : 0U)))));
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffff0000U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 0x10U))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffff00U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 8U)))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffff0U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 4U))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffffcU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 2U)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     (0xfffffffeU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 1U))
						      : 0U)))));
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & vlTOPp->BsrExUnit__DOT__exRegValRn) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0x7fffffffU 
					   & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					      >> 1U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xfffffffeU 
					   & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					      << 1U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr 
					       << 0x1fU)) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0xfffffffeU 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 1U)) 
					   | (1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 0x1fU)) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0xfffffffeU 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 1U)) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((~ vlTOPp->BsrExUnit__DOT__exRegValRn) 
					   + (1U & 
					      (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= VL_NEGATE_I(vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (~ vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((((0x8000U 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn)
					      ? 0xffffU
					      : 0U) 
					    << 0x10U) 
					   | (0xffffU 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((((0x80U 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn)
					      ? 0xffffffU
					      : 0U) 
					    << 8U) 
					   | (0xffU 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xffffU 
					   & vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xffU & vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if ((1U & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exRegValRn;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				} else {
				    if ((1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exRegValRn;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr 
					= vlTOPp->BsrExUnit__DOT__exIdInGenPc;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp 
					= ((IData)(4U) 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xaU;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				    if ((0x10U == (IData)(vlTOPp->BsrExUnit__DOT__exRegIdRn))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
					if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
					}
					if ((0U == vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2)) {
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					}
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					    = vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp 
					= (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp 
					   - (IData)(4U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x12U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if ((1U & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				} else {
				    if ((1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr 
					= vlTOPp->BsrExUnit__DOT__exIdInGenPc;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				}
			    }
			}
		    }
		}
	    } else {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
				= ((0xfffffffeU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
				   | (1U & ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
					     ? ((1U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
						 ? 
						((vlTOPp->BsrExUnit__DOT__exRegValRn 
						  >= vlTOPp->BsrExUnit__DOT__exRegValRm) 
						 ^ 
						 ((vlTOPp->BsrExUnit__DOT__exRegValRn 
						   ^ vlTOPp->BsrExUnit__DOT__exRegValRm) 
						  >> 0x1fU))
						 : 
						((vlTOPp->BsrExUnit__DOT__exRegValRn 
						  > vlTOPp->BsrExUnit__DOT__exRegValRm) 
						 ^ 
						 ((vlTOPp->BsrExUnit__DOT__exRegValRn 
						   ^ vlTOPp->BsrExUnit__DOT__exRegValRm) 
						  >> 0x1fU)))
					     : ((1U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
						 ? 
						(vlTOPp->BsrExUnit__DOT__exRegValRn 
						 > vlTOPp->BsrExUnit__DOT__exRegValRm)
						 : 
						(vlTOPp->BsrExUnit__DOT__exRegValRn 
						 == vlTOPp->BsrExUnit__DOT__exRegValRm)))));
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
				    = ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
				        ? ((0xffffff00U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
					       << 8U)) 
					   | (0xffU 
					      & vlTOPp->BsrExUnit__DOT__exImmValRi))
				        : vlTOPp->BsrExUnit__DOT__exImmValRi);
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr;
				    if ((2U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    }
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   ^ vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   | vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   & vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (0U == 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       & vlTOPp->BsrExUnit__DOT__exRegValRm)));
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & (IData)(
							   (VL_ULL(1) 
							    & ((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRn)) 
								 + (QData)((IData)(
										(~ vlTOPp->BsrExUnit__DOT__exRegValRm)))) 
								+ (QData)((IData)(
										(1U 
										& (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))))) 
							       >> 0x20U)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((vlTOPp->BsrExUnit__DOT__exRegValRn 
					    + (~ vlTOPp->BsrExUnit__DOT__exRegValRm)) 
					   + (IData)((QData)((IData)(
								     (1U 
								      & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & (IData)(
							   (VL_ULL(1) 
							    & ((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRn)) 
								 + (QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRm))) 
								+ (QData)((IData)(
										(1U 
										& vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)))) 
							       >> 0x20U)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((vlTOPp->BsrExUnit__DOT__exRegValRn 
					    + vlTOPp->BsrExUnit__DOT__exRegValRm) 
					   + (IData)((QData)((IData)(
								     (1U 
								      & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   - vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   + vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xdU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xcU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xaU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x12U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 9U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x11U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 8U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x10U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exImmValRi;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (0x80U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
					VL_WRITEF("BsrExOp: Missing %x-%x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd,
						  8,
						  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					if (VL_UNLIKELY(
							(0x40U 
							 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
					    VL_WRITEF("BsrExOp: Missing %x-%x\n",
						      8,
						      vlTOPp->BsrExUnit__DOT__exOpCmd,
						      8,
						      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					} else {
					    if (VL_UNLIKELY(
							    (0x20U 
							     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
						VL_WRITEF("BsrExOp: Missing %x-%x\n",
							  8,
							  vlTOPp->BsrExUnit__DOT__exOpCmd,
							  8,
							  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
						vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					    } else {
						if (VL_UNLIKELY(
								(0x10U 
								 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
						    VL_WRITEF("BsrExOp: Missing %x-%x\n",
							      8,
							      vlTOPp->BsrExUnit__DOT__exOpCmd,
							      8,
							      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
						    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
						} else {
						    if (
							(8U 
							 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
							if (VL_UNLIKELY(
									(4U 
									 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
							    VL_WRITEF("BsrExOp: Missing %x-%x\n",
								      8,
								      vlTOPp->BsrExUnit__DOT__exOpCmd,
								      8,
								      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
							    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
							} else {
							    if (VL_UNLIKELY(
									    (2U 
									     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
								VL_WRITEF("BsrExOp: Missing %x-%x\n",
									  8,
									  vlTOPp->BsrExUnit__DOT__exOpCmd,
									  8,
									  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
								vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
							    } else {
								vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
								    = 
								    ((1U 
								      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
								      ? 
								     ((0xfffffffdU 
								       & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								      | (2U 
									 & ((~ 
									     (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr 
									      >> 1U)) 
									    << 1U)))
								      : 
								     ((0xfffffffeU 
								       & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								      | (1U 
									 & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))));
							    }
							}
						    } else {
							if (
							    (4U 
							     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
							    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
								= 
								((2U 
								  & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
								  ? 
								 ((0xfffffffdU 
								   & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								  | (2U 
								     & ((IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt) 
									<< 1U)))
								  : 
								 ((0xfffffffeU 
								   & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								  | (1U 
								     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))));
							} else {
							    if (
								(2U 
								 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
								if (VL_LIKELY(
									      (1U 
									       & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
								} else {
								    VL_WRITEF("BsrExOp: Missing %x-%x\n",
									      8,
									      vlTOPp->BsrExUnit__DOT__exOpCmd,
									      8,
									      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
								}
							    } else {
								if (
								    (1U 
								     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
									= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr;
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    // ALWAYS at BsrMemTileB.v:156
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] = 0U;
    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB 
	    = (0xfffU & (vlTOPp->BsrExUnit__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB)));
    } else {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & (vlTOPp->BsrExUnit__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA;
    }
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileSrSt = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__mmioBypass = 0U;
    if ((1U & (((IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm) 
		>> 3U) | ((IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm) 
			  >> 4U)))) {
	if (((0x8000U >= vlTOPp->BsrExUnit__DOT__memRegInAddr) 
	     & (~ (vlTOPp->BsrExUnit__DOT__memRegInAddr 
		   >> 0xeU)))) {
	    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
	    }
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		= (((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
		    == (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA))
		    ? 1U : 2U);
	    if ((2U == (3U & (IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm)))) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		if ((4U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U]))) 
				   >> 0x20U));
		}
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
	    }
	} else {
	    if (vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileA
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		    = (((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
			== (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA))
		        ? 1U : 2U);
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileB
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileC
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileD
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		}
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm))) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[0U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[1U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[2U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[3U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileSrSt 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam;
		    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
		    } else {
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
		    }
		}
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__mmioBypass = 1U;
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioOK;
		VL_EXTEND_WI(128,32, __Vtemp3, vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioInData);
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = __Vtemp3[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = __Vtemp3[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = __Vtemp3[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = __Vtemp3[3U];
	    }
	}
    }
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__exHold = 0U;
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    vlTOPp->BsrExUnit__DOT__gprRegExHold = vlTOPp->BsrExUnit__DOT__exHold;
    // ALWAYS at BsrDcTileB.v:84
    if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
		      >> 3U)))) {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss 
	    = (1U & ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
		      != (0x1ffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
				      >> 3U))) | (~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady))));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr 
	    = (0x1ffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
			   >> 3U));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass 
	    = (7U == (7U & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
			    >> 0xdU)));
    } else {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss = 0U;
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass = 0U;
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr = 0U;
    }
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB;
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 0U;
    if (((((~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss)) 
	   & (~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass))) 
	  & (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady)) 
	 & (0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			 >> 3U))))) {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData 
	    = ((4U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
	        ? ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		    ? ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		        ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			    << 8U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x10U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		     ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x18U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 8U)) : vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U]))
	        : ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		    ? ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		        ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			    << 8U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x10U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		     ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x18U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 8U)) : vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U])));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore = 0U;
	if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			  >> 3U)))) {
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 1U;
	}
	if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
	    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
		    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
	    } else {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= (0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= (0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		}
	    }
	} else {
	    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
		}
	    } else {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= ((((0x8000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffU : 0U) << 0x10U) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= ((((0x80U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffffU : 0U) << 8U) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		}
	    }
	}
	if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore = 1U;
	}
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[3U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[4U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[4U];
	if ((4U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
	    if ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xff000000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xff000000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffff0000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffff00U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	} else {
	    if ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xff000000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xff000000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffff0000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffffff00U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	}
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore 
	    = (1U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
		     >> 4U));
    } else {
	if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass) {
	    if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__dcMemPcOK))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 1U;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U];
	    } else {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	} else {
	    if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			      >> 3U)))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	}
    }
    // ALWAYS at BsrRegGpr.v:150
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRn 
	= ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
	    ? 0U : ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
		     ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			 ? 0U : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				  ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				      ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr
					  : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr)
				      : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr
					  : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr))
				  : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				      ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr
					  : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)
				      : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr
					  : ((IData)(2U) 
					     + vlTOPp->BsrExUnit__DOT__gprIdValPc)))))
		     : ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			 ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			     ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12))
			     : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8)))
			 : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			     ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A))
			     : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A))))));
    if (((IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn) 
	 == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))) {
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRn 
	    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal;
    }
    // ALWAYS at BsrRegGpr.v:150
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRm 
	= ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
	    ? ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
	        ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
		    ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
		        ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			        ? 0U : 0U) : 0U) : 
		       ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			 ? 0U : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				  ? 0U : vlTOPp->BsrExUnit__DOT__gprIdImm)))
		    : 0U) : 0U) : ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				    ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				        ? 0U : ((4U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						 ? 
						((2U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						  ? 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr
						   : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr)
						  : 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr
						   : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr))
						 : 
						((2U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						  ? 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr
						   : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)
						  : 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr
						   : 
						  ((IData)(2U) 
						   + vlTOPp->BsrExUnit__DOT__gprIdValPc)))))
				    : ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				        ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					    ? ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12))
					    : ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8)))
				        : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					    ? ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A))
					    : ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A))))));
    if (((IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm) 
	 == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))) {
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRm 
	    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal;
    }
}

VL_INLINE_OPT void VBsrExUnit::_sequent__TOP__2(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_sequent__TOP__2\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdly__BsrExUnit__DOT__gprIdUCmd,7,0);
    VL_SIG8(__Vdly__BsrExUnit__DOT__gprRegIdRn,5,0);
    VL_SIG8(__Vdly__BsrExUnit__DOT__gprIdUIxt,7,0);
    VL_SIG8(__Vdly__BsrExUnit__DOT__exBranchFlush,0,0);
    VL_SIG8(__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0,0,0);
    VL_SIG8(__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty,0,0);
    VL_SIG8(__Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady,0,0);
    VL_SIG16(__Vdly__BsrExUnit__DOT__decIstrWord,15,0);
    VL_SIG16(__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileA__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileB__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileC__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileD__v0,8,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprIdImm,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprIdValPc,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__decRegPc,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__icRegInPc,31,0);
    VL_SIG(__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileA__v0,31,0);
    VL_SIG(__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileB__v0,31,0);
    VL_SIG(__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileC__v0,31,0);
    VL_SIG(__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileD__v0,31,0);
    VL_SIGW(__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData,159,0,5);
    VL_SIG(__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkAddr,16,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR0A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR1A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR2A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR3A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR4A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR5A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR6A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR7A,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR8,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR9,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR10,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR11,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR12,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR13,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR14,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__exOp__DOT__regMulDlr,31,0);
    VL_SIG(__Vdly__BsrExUnit__DOT__exOp__DOT__regMulDhr,31,0);
    VL_SIGW(__Vtemp4,127,0,4);
    // Body
    __Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0 = 0U;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR0A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR1A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR2A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR3A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR4A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR5A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR6A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR7A 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR8 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR9 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR10 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR11 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR12 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR13 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13;
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR14 
	= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14;
    __Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady 
	= vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkReady;
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDirty;
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[3U] 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[4U] 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[4U];
    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkAddr;
    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDlr = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr;
    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDhr = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr;
    __Vdly__BsrExUnit__DOT__gprIdUCmd = vlTOPp->BsrExUnit__DOT__gprIdUCmd;
    __Vdly__BsrExUnit__DOT__gprIdUIxt = vlTOPp->BsrExUnit__DOT__gprIdUIxt;
    __Vdly__BsrExUnit__DOT__exBranchFlush = vlTOPp->BsrExUnit__DOT__exBranchFlush;
    __Vdly__BsrExUnit__DOT__decRegPc = vlTOPp->BsrExUnit__DOT__decRegPc;
    __Vdly__BsrExUnit__DOT__gprRegIdRn = vlTOPp->BsrExUnit__DOT__gprRegIdRn;
    __Vdly__BsrExUnit__DOT__gprIdImm = vlTOPp->BsrExUnit__DOT__gprIdImm;
    __Vdly__BsrExUnit__DOT__icRegInPc = vlTOPp->BsrExUnit__DOT__icRegInPc;
    __Vdly__BsrExUnit__DOT__decIstrWord = vlTOPp->BsrExUnit__DOT__decIstrWord;
    __Vdly__BsrExUnit__DOT__gprIdValPc = vlTOPp->BsrExUnit__DOT__gprIdValPc;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxB 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileA
	[(0x7ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA) 
		    >> 1U))];
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileB
	[(0x7ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA) 
		    >> 1U))];
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileC
	[(0x7ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB) 
		    >> 1U))];
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileD
	[(0x7ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB) 
		    >> 1U))];
    // ALWAYS at BsrMemTileB.v:347
    if (vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileSrSt) {
	__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileA__v0 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U];
	__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0 = 1U;
	__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileA__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA) 
			 >> 1U));
	__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileB__v0 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U];
	__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileB__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA) 
			 >> 1U));
	__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileC__v0 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U];
	__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileC__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB) 
			 >> 1U));
	__Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileD__v0 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U];
	__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileD__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB) 
			 >> 1U));
    }
    // ALWAYS at BsrMemTileB.v:347
    if (vlTOPp->BsrExUnit__DOT__memTile__DOT__mmioBypass) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOutData 
	    = vlTOPp->BsrExUnit__DOT__memRegInData[0U];
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioAddr 
	    = vlTOPp->BsrExUnit__DOT__memRegInAddr;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOpm 
	    = vlTOPp->BsrExUnit__DOT__memRegInOpm;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioOK 
	    = vlTOPp->mmioOK;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioInData 
	    = vlTOPp->mmioInData;
    } else {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOutData = 0U;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioAddr = 0U;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOpm = 0U;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioOK = 0U;
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioInData = 0U;
    }
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR0A 
	= ((0U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR1A 
	= ((1U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR2A 
	= ((2U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR3A 
	= ((3U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR4A 
	= ((4U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR5A 
	= ((5U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR6A 
	= ((6U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR7A 
	= ((7U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR8 
	= ((8U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR9 
	= ((9U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR10 
	= ((0xaU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR11 
	= ((0xbU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR12 
	= ((0xcU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR13 
	= ((0xdU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13);
    // ALWAYS at BsrRegGpr.v:261
    __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR14 
	= ((0xeU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
	    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
	    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14);
    // ALWAYS at BsrDcTileB.v:239
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData;
    // ALWAYS at BsrDcTileB.v:239
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK;
    // ALWAYS at BsrIcTileB.v:83
    if (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkMiss) {
	if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__icMemPcOK))) {
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcAddr = 0U;
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm = 0U;
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[0U] 
		= vlTOPp->BsrExUnit__DOT__icMemPcData[0U];
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U] 
		= vlTOPp->BsrExUnit__DOT__icMemPcData[1U];
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U] 
		= vlTOPp->BsrExUnit__DOT__icMemPcData[2U];
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U] 
		= vlTOPp->BsrExUnit__DOT__icMemPcData[3U];
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr 
		= (0x1ffffU & (vlTOPp->BsrExUnit__DOT__icRegInPc 
			       >> 3U));
	    __Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady = 1U;
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[4U] = 0U;
	} else {
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcAddr 
		= (0xffff8U & vlTOPp->BsrExUnit__DOT__icRegInPc);
	    vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm = 0xfU;
	    __Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady = 0U;
	}
    } else {
	__Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady 
	    = vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkReady;
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcAddr = 0U;
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm = 0U;
    }
    // ALWAYS at BsrDcTileB.v:239
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore;
    if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass) {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr 
	    = (0xfffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr);
	VL_EXTEND_WI(128,32, __Vtemp4, vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData);
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] 
	    = __Vtemp4[0U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] 
	    = __Vtemp4[1U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] 
	    = __Vtemp4[2U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] 
	    = __Vtemp4[3U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm 
	    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm;
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB = 0U;
    } else {
	if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss) {
	    if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__dcMemPcOK))) {
		if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDirty) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr 
			   << 3U);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = 0U;
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady = 0U;
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = 0U;
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
			= vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U];
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			= vlTOPp->BsrExUnit__DOT__dcMemPcDataI[1U];
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			= vlTOPp->BsrExUnit__DOT__dcMemPcDataI[2U];
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[3U] 
			= vlTOPp->BsrExUnit__DOT__dcMemPcDataI[3U];
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr;
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady = 1U;
		    __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[4U] = 0xffffffffU;
		}
	    } else {
		if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDirty) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
			   << 3U);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = 0x17U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady = 0U;
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr 
			   << 3U);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = 0xfU;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady = 0U;
		}
	    }
	} else {
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr = 0U;
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = 0U;
	    if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore) {
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U];
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U];
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U];
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[3U] 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[3U];
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[4U] 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[4U];
		__Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty = 1U;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady = 1U;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB = 0U;
	    }
	}
    }
    // ALWAYS at BsrExOp.v:823
    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp))) {
	if (VL_LIKELY((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp)))) {
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp = 0U;
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr = 0U;
	} else {
	    VL_WRITEF("BsrExOp: Mul A: %x %x\n",32,
		      vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA,
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB);
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpE 
		= (((0x80000000U & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA)
		     ? VL_NEGATE_I(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB)
		     : 0U) + ((0x80000000U & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB)
			       ? VL_NEGATE_I(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA)
			       : 0U));
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp;
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPq;
	    VL_WRITEF("BsrExOp: Mul B: %x %x %x %x\n",
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA,
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB,
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC,
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpD);
	    VL_WRITEF("BsrExOp: Mul C: %x %x\n",32,
		      vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr,
		      32,vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr);
	    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDlr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqA;
	    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDhr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqB;
	}
    } else {
	if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp))) {
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpE = 0U;
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp;
	    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDlr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqA;
	    __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDhr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqB;
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr 
		= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPq;
	} else {
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp = 0U;
	    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr = 0U;
	}
    }
    // ALWAYS at BsrExUnit.v:360
    if ((1U & (~ (IData)(vlTOPp->BsrExUnit__DOT__exHold)))) {
	__Vdly__BsrExUnit__DOT__decIstrWord = (0xffffU 
					       & ((IData)(vlTOPp->BsrExUnit__DOT__exBranchFlush)
						   ? 0x3000U
						   : vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcVal));
	vlTOPp->BsrExUnit__DOT__gprRegIdRm = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM;
	vlTOPp->BsrExUnit__DOT__exRegIdIxt = vlTOPp->BsrExUnit__DOT__gprIdUIxt;
	vlTOPp->BsrExUnit__DOT__exRegIdRn = vlTOPp->BsrExUnit__DOT__gprRegIdRn;
	vlTOPp->BsrExUnit__DOT__exRegValRm = vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRm;
	vlTOPp->BsrExUnit__DOT__exRegValRn = vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRn;
	vlTOPp->BsrExUnit__DOT__exImmValRi = vlTOPp->BsrExUnit__DOT__gprIdImm;
	vlTOPp->BsrExUnit__DOT__exIdInGenPc = ((IData)(2U) 
					       + vlTOPp->BsrExUnit__DOT__gprIdValPc);
	__Vdly__BsrExUnit__DOT__gprIdUCmd = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd;
	vlTOPp->BsrExUnit__DOT__exOpCmd = vlTOPp->BsrExUnit__DOT__gprIdUCmd;
	__Vdly__BsrExUnit__DOT__gprRegIdRn = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN;
	__Vdly__BsrExUnit__DOT__gprIdImm = vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm;
	__Vdly__BsrExUnit__DOT__gprIdValPc = vlTOPp->BsrExUnit__DOT__decRegPc;
	__Vdly__BsrExUnit__DOT__gprIdUIxt = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt;
	if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
	    __Vdly__BsrExUnit__DOT__exBranchFlush = 1U;
	    __Vdly__BsrExUnit__DOT__decIstrWord = 0x3000U;
	    __Vdly__BsrExUnit__DOT__gprIdUCmd = 0U;
	    vlTOPp->BsrExUnit__DOT__exOpCmd = 0U;
	} else {
	    __Vdly__BsrExUnit__DOT__exBranchFlush = 0U;
	}
	__Vdly__BsrExUnit__DOT__decRegPc = vlTOPp->BsrExUnit__DOT__icRegInPc;
	__Vdly__BsrExUnit__DOT__icRegInPc = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPc;
    }
    // ALWAYSPOST at BsrMemTileB.v:396
    if (__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileA[__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileA__v0] 
	    = __Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileA__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:397
    if (__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileB[__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileB__v0] 
	    = __Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileB__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:398
    if (__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileC[__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileC__v0] 
	    = __Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileC__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:399
    if (__Vdlyvset__BsrExUnit__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileD[__Vdlyvdim0__BsrExUnit__DOT__memTile__DOT__sramTileD__v0] 
	    = __Vdlyvval__BsrExUnit__DOT__memTile__DOT__sramTileD__v0;
    }
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR0A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR1A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR2A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR3A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR4A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR5A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR6A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR7A;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR8;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR9;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR10;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR11;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR12;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR13;
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14 
	= __Vdly__BsrExUnit__DOT__gprFile__DOT__regGprR14;
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkReady 
	= __Vdly__BsrExUnit__DOT__icTile__DOT__icBlkReady;
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDirty 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkDirty;
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U] 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[4U] 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkData[4U];
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
	= __Vdly__BsrExUnit__DOT__dcTile__DOT__icBlkAddr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr = __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDlr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr = __Vdly__BsrExUnit__DOT__exOp__DOT__regMulDhr;
    vlTOPp->BsrExUnit__DOT__exBranchFlush = __Vdly__BsrExUnit__DOT__exBranchFlush;
    vlTOPp->BsrExUnit__DOT__gprIdUIxt = __Vdly__BsrExUnit__DOT__gprIdUIxt;
    vlTOPp->BsrExUnit__DOT__gprIdUCmd = __Vdly__BsrExUnit__DOT__gprIdUCmd;
    vlTOPp->BsrExUnit__DOT__decRegPc = __Vdly__BsrExUnit__DOT__decRegPc;
    vlTOPp->BsrExUnit__DOT__gprRegIdRn = __Vdly__BsrExUnit__DOT__gprRegIdRn;
    vlTOPp->BsrExUnit__DOT__gprIdImm = __Vdly__BsrExUnit__DOT__gprIdImm;
    vlTOPp->BsrExUnit__DOT__icRegInPc = __Vdly__BsrExUnit__DOT__icRegInPc;
    vlTOPp->BsrExUnit__DOT__decIstrWord = __Vdly__BsrExUnit__DOT__decIstrWord;
    vlTOPp->BsrExUnit__DOT__gprIdValPc = __Vdly__BsrExUnit__DOT__gprIdValPc;
    vlTOPp->mmioOutData = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOutData;
    vlTOPp->mmioAddr = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioAddr;
    vlTOPp->mmioOpm = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMmioOpm;
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPq = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpPp;
    // ALWAYS at BsrExOp.v:823
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpD = 
	((0xffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
		     >> 0x10U)) * (0xffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
					      >> 0x10U)));
    // ALWAYS at BsrExOp.v:823
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA = 
	((0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA) 
	 * (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB));
    // ALWAYS at BsrExOp.v:823
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB = 
	((0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA) 
	 * (0xffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
		       >> 0x10U)));
    // ALWAYS at BsrExOp.v:823
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC = 
	((0xffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
		     >> 0x10U)) * (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB));
    // ALWAYS at BsrIcTileB.v:51
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkHi = 
	((1U & vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr) 
	 != (1U & (vlTOPp->BsrExUnit__DOT__icRegInPc 
		   >> 3U)));
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx 
	= (((IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkHi) 
	    << 2U) | (3U & (vlTOPp->BsrExUnit__DOT__icRegInPc 
			    >> 1U)));
    vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkMiss 
	= (1U & (((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr 
		   != (0x1ffffU & (vlTOPp->BsrExUnit__DOT__icRegInPc 
				   >> 3U))) & (((IData)(1U) 
						+ vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkAddr) 
					       != (0x1ffffU 
						   & (vlTOPp->BsrExUnit__DOT__icRegInPc 
						      >> 3U)))) 
		 | (~ (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkReady))));
    if (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkMiss) {
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK = 2U;
    } else {
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK = 1U;
	vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcVal 
	    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
	        ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[4U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U])
		    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[3U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U]))
	        : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[2U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U])
		    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkWordIx))
		        ? ((vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[1U] 
			    << 0x10U) | (vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[0U] 
					 >> 0x10U))
		        : vlTOPp->BsrExUnit__DOT__icTile__DOT__icBlkData[0U])));
    }
    // ALWAYS at BsrDecOp.v:56
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegO_Dfl 
	= (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
		   >> 8U));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl 
	= (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
		   >> 4U));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl 
	= (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr = 
	(0x10U | (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
			  >> 4U)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Cr = 
	(0x10U | (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr = 
	(0x20U | (0xfU & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
			  >> 4U)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Sr = 
	(0x20U | (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 1U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x3fU;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0U;
    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
    if (((((((((0U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
	       | (0x100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	      | (0x200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	     | (0x300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	    | (0x400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	   | (0x500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	  | (0x600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	 | (0x700U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
	if ((0U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
	    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
	} else {
	    if ((0x100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
	    } else {
		if ((0x200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
		} else {
		    if ((0x300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
		    } else {
			if ((0x400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
			} else {
			    if ((0x500U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			    } else {
				if ((0x600U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xaU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
				}
			    }
			}
		    }
		}
	    }
	}
    } else {
	if (((((((((0x800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
		   | (0x900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		  | (0xa00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		 | (0xb00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		| (0xc00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	       | (0xd00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	      | (0xe00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
	     | (0xf00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
	    if ((0x800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
		vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
	    } else {
		if ((0x900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
		} else {
		    if ((0xa00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
		    } else {
			if ((0xb00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
			} else {
			    if ((0xc00U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
			    } else {
				if ((0xd00U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				} else {
				    if ((0xe00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
				    }
				}
			    }
			}
		    }
		}
	    }
	} else {
	    if (((((((((0x1000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
		       | (0x1100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		      | (0x1200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		     | (0x1300U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		    | (0x1400U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		   | (0x1500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		  | (0x1600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		 | (0x1700U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
		if ((0x1000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
		    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		} else {
		    if ((0x1100U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x11U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		    } else {
			if ((0x1200U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x12U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			} else {
			    if ((0x1300U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x13U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			    } else {
				if ((0x1400U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x14U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				} else {
				    if ((0x1500U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x15U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    } else {
					if ((0x1600U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x16U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					} else {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x17U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					}
				    }
				}
			    }
			}
		    }
		}
	    } else {
		if (((((((((0x1800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
			   | (0x1900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			  | (0x1a00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			 | (0x1b00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			| (0x1c00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		       | (0x1d00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		      | (0x1e00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
		     | (0x1f00U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
		    if ((0x1800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
		    } else {
			if ((0x1900U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x19U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
			} else {
			    if ((0x1a00U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
			    } else {
				if ((0x1b00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
				} else {
				    if ((0x1c00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
				    } else {
					if ((0x1d00U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1dU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					} else {
					    if ((0x1e00U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					    } else {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x18U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
					    }
					}
				    }
				}
			    }
			}
		    }
		} else {
		    if (((((((((0x2000U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
			       | (0x2100U == (0xff00U 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			      | (0x2200U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			     | (0x2300U == (0xff00U 
					    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			    | (0x2400U == (0xff00U 
					   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			   | (0x2500U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			  | (0x2600U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			 | (0x2800U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
			if ((0x2000U == (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			} else {
			    if ((0x2100U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
			    } else {
				if ((0x2200U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				} else {
				    if ((0x2300U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x13U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
				    } else {
					if ((0x2400U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 6U;
					} else {
					    if ((0x2500U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 7U;
					    } else {
						if (
						    (0x2600U 
						     == 
						     (0xff00U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1bU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 6U;
						} else {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1eU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
						}
					    }
					}
				    }
				}
			    }
			}
		    } else {
			if (((((((((0x2900U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				   | (0x2a00U == (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				  | (0x2b00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				 | (0x2c00U == (0xff00U 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
				| (0x2d00U == (0xff00U 
					       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			       | (0x2e00U == (0xff00U 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			      | (0x2f00U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
			     | (0x3000U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
			    if ((0x2900U == (0xff00U 
					     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1eU;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
			    } else {
				if ((0x2a00U == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1fU;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
				} else {
				    if ((0x2b00U == 
					 (0xff00U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1fU;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
				    } else {
					if ((0x2c00U 
					     == (0xff00U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
					} else {
					    if ((0x2d00U 
						 == 
						 (0xff00U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xfU;
					    } else {
						if (
						    (0x2e00U 
						     == 
						     (0xff00U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
						} else {
						    if (
							(0x2f00U 
							 == 
							 (0xff00U 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1fU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xeU;
						    } else {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    }
						}
					    }
					}
				    }
				}
			    }
			} else {
			    if (((((((((0x3010U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)) 
				       | (0x3020U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				      | (0x3030U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				     | (0x3040U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				    | (0x3050U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				   | (0x3060U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				  | (0x3070U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				 | (0x3080U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				if ((0x3010U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
				} else {
				    if ((0x3020U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
				    } else {
					if ((0x3030U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 3U;
					} else {
					    if ((0x3040U 
						 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
					    } else {
						if (
						    (0x3050U 
						     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 5U;
						} else {
						    if (
							(0x3060U 
							 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 6U;
						    } else {
							if (
							    (0x3070U 
							     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 7U;
							} else {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
							}
						    }
						}
					    }
					}
				    }
				}
			    } else {
				if (((((((((0x3090U 
					    == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)) 
					   | (0x30a0U 
					      == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					  | (0x30b0U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					 | (0x30c0U 
					    == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					| (0x3002U 
					   == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				       | (0x3012U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				      | (0x3022U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
				     | (0x3032U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
				    if ((0x3090U == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 9U;
				    } else {
					if ((0x30a0U 
					     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x45U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0U;
					} else {
					    if ((0x30b0U 
						 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x45U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm = 0xffffU;
					    } else {
						if (
						    (0x30c0U 
						     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 3U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 1U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0xcU;
						} else {
						    if (
							(0x3002U 
							 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
						    } else {
							if (
							    (0x3012U 
							     == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							} else {
							    if (
								(0x3022U 
								 == (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							    } else {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x11U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    if (((((((((0x3004U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
					       | (0x3005U 
						  == 
						  (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					      | (0x3006U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					     | (0x3007U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					    | (0x3008U 
					       == (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					   | (0x3009U 
					      == (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					  | (0x300aU 
					     == (0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					 | (0x300bU 
					    == (0xff0fU 
						& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
					if ((0x3004U 
					     == (0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
					    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					} else {
					    if ((0x3005U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					    } else {
						if (
						    (0x3006U 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xcU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						} else {
						    if (
							(0x3007U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						    } else {
							if (
							    (0x3008U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x24U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
							} else {
							    if (
								(0x3009U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x24U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
							    } else {
								if (
								    (0x300aU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x26U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
								} else {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x26U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
								}
							    }
							}
						    }
						}
					    }
					}
				    } else {
					if ((((((((
						   (0x300cU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
						   | (0x300dU 
						      == 
						      (0xff0fU 
						       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						  | (0x300eU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						 | (0x300fU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						| (0x3100U 
						   == 
						   (0xff0fU 
						    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					       | (0x3101U 
						  == 
						  (0xff0fU 
						   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					      | (0x3102U 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
					     | (0x3103U 
						== 
						(0xff0fU 
						 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
					    if ((0x300cU 
						 == 
						 (0xff0fU 
						  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
						vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
					    } else {
						if (
						    (0x300dU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						} else {
						    if (
							(0x300eU 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
						    } else {
							if (
							    (0x300fU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
							} else {
							    if (
								(0x3100U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x3101U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x11U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								} else {
								    if (
									(0x3102U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x12U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								    } else {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x13U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
								    }
								}
							    }
							}
						    }
						}
					    }
					} else {
					    if ((((
						   (((((0x3104U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
						       | (0x3105U 
							  == 
							  (0xff0fU 
							   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						      | (0x3106U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						     | (0x3107U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						    | (0x3108U 
						       == 
						       (0xff0fU 
							& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						   | (0x3109U 
						      == 
						      (0xff0fU 
						       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						  | (0x310aU 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						 | (0x310bU 
						    == 
						    (0xff0fU 
						     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
						if (
						    (0x3104U 
						     == 
						     (0xff0fU 
						      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x14U;
						    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						} else {
						    if (
							(0x3105U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x15U;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						    } else {
							if (
							    (0x3106U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x16U;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							} else {
							    if (
								(0x3107U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x17U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x3108U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3109U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1fU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x310aU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
									} else {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
									}
								    }
								}
							    }
							}
						    }
						}
					    } else {
						if (
						    ((((((((0x310cU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
							   | (0x310dU 
							      == 
							      (0xff0fU 
							       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							  | (0x310eU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							 | (0x310fU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							| (0x3200U 
							   == 
							   (0xff0fU 
							    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						       | (0x3201U 
							  == 
							  (0xff0fU 
							   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						      | (0x3202U 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
						     | (0x3203U 
							== 
							(0xff0fU 
							 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
						    if (
							(0x310cU 
							 == 
							 (0xff0fU 
							  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1cU;
							vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
						    } else {
							if (
							    (0x310dU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1dU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							} else {
							    if (
								(0x310eU 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1eU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
							    } else {
								if (
								    (0x310fU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xdU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
								} else {
								    if (
									(0x3200U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x28U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3201U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x29U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									} else {
									    if (
										(0x3202U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    }
									}
								    }
								}
							    }
							}
						    }
						} else {
						    if (
							((((((((0x3204U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
							       | (0x3205U 
								  == 
								  (0xff0fU 
								   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							      | (0x3206U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							     | (0x3207U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							    | (0x3208U 
							       == 
							       (0xff0fU 
								& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							   | (0x3209U 
							      == 
							      (0xff0fU 
							       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							  | (0x320aU 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							 | (0x320bU 
							    == 
							    (0xff0fU 
							     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
							if (
							    (0x3204U 
							     == 
							     (0xff0fU 
							      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2cU;
							    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							} else {
							    if (
								(0x3205U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2dU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
							    } else {
								if (
								    (0x3206U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2eU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3207U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x2fU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3208U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1aU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
									} else {
									    if (
										(0x3209U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
									    } else {
										if (
										(0x320aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
										}
									    }
									}
								    }
								}
							    }
							}
						    } else {
							if (
							    ((((((((0x320cU 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
								   | (0x320dU 
								      == 
								      (0xff0fU 
								       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								  | (0x320eU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								 | (0x320fU 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								| (0x3300U 
								   == 
								   (0xff0fU 
								    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							       | (0x3301U 
								  == 
								  (0xff0fU 
								   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							      | (0x3302U 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
							     | (0x3304U 
								== 
								(0xff0fU 
								 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
							    if (
								(0x320cU 
								 == 
								 (0xff0fU 
								  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
								vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
							    } else {
								if (
								    (0x320dU 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
								} else {
								    if (
									(0x320eU 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 4U;
								    } else {
									if (
									    (0x320fU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1bU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 5U;
									} else {
									    if (
										(0x3300U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x30U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										if (
										(0x3301U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x31U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x3302U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x32U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x34U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										}
										}
									    }
									}
								    }
								}
							    }
							} else {
							    if (
								((((((((0x3305U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
								       | (0x3306U 
									  == 
									  (0xff0fU 
									   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								      | (0x3307U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								     | (0x3308U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								    | (0x3309U 
								       == 
								       (0xff0fU 
									& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								   | (0x330aU 
								      == 
								      (0xff0fU 
								       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								  | (0x3400U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								 | (0x3401U 
								    == 
								    (0xff0fU 
								     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
								if (
								    (0x3305U 
								     == 
								     (0xff0fU 
								      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x35U;
								    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								} else {
								    if (
									(0x3306U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x36U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
								    } else {
									if (
									    (0x3307U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x37U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									} else {
									    if (
										(0x3308U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x38U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    } else {
										if (
										(0x3309U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x39U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x330aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										} else {
										if (
										(0x3400U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										}
										}
										}
									    }
									}
								    }
								}
							    } else {
								if (
								    ((((((((0x3402U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
									   | (0x3403U 
									      == 
									      (0xff0fU 
									       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									  | (0x3404U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									 | (0x3405U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									| (0x3406U 
									   == 
									   (0xff0fU 
									    & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								       | (0x3407U 
									  == 
									  (0xff0fU 
									   & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								      | (0x3408U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
								     | (0x3409U 
									== 
									(0xff0fU 
									 & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
								    if (
									(0x3402U 
									 == 
									 (0xff0fU 
									  & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
								    } else {
									if (
									    (0x3403U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
									} else {
									    if (
										(0x3404U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
									    } else {
										if (
										(0x3405U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										} else {
										if (
										(0x3406U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
										} else {
										if (
										(0x3407U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
										} else {
										if (
										(0x3408U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 2U;
										}
										}
										}
										}
									    }
									}
								    }
								} else {
								    if (
									((((((((0x340aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
									       | (0x340bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									      | (0x340cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									     | (0x340dU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									    | (0x340eU 
									       == 
									       (0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									   | (0x340fU 
									      == 
									      (0xff0fU 
									       & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									  | (0x3500U 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									 | (0x3501U 
									    == 
									    (0xff0fU 
									     & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
									if (
									    (0x340aU 
									     == 
									     (0xff0fU 
									      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
									    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 4U;
									} else {
									    if (
										(0x340bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 8U;
									    } else {
										if (
										(0x340cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340dU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340eU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x3eU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x10U;
										} else {
										if (
										(0x340fU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x33U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 2U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx = 0x1fU;
										} else {
										if (
										(0x3500U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										}
										}
										}
										}
										}
									    }
									}
								    } else {
									if (
									    ((((((((0x3502U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x3503U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x3508U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x3509U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x350aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									       | (0x350bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									      | (0x360cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
									     | (0x3f00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
									    if (
										(0x3502U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
									    } else {
										if (
										(0x3503U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x18U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x3508U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x3509U 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x350aU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
										} else {
										if (
										(0x350bU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x19U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x360cU 
										== 
										(0xff0fU 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x12U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										}
										}
										}
										}
										}
									    }
									} else {
									    if (
										((((((((0x4000U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x4100U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4200U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4300U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4400U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4500U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4600U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4700U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x4000U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4100U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xaU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4200U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x20U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1cU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4300U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x21U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xeU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4400U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4500U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0x4600U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x22U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xdU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										} else {
										if (
										(0xfU 
										== 
										(0xfU 
										& ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
										>> 4U)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x23U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xfU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										}
										}
										}
										}
										}
										}
										}
										}
									    } else {
										if (
										((((((((0x4800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x4900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4d00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4e00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x4f00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x4800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										if (
										(0x4900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x1dU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 7U;
										} else {
										if (
										(0x4a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										if (
										(0x4b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 7U;
										} else {
										if (
										(0x4c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 0U;
										} else {
										if (
										(0x4d00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 1U;
										} else {
										if (
										(0x4e00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 2U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 6U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0xbU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 3U;
										}
										}
										}
										}
										}
										}
										}
										} else {
										if (
										((((((((0x5800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) 
										| (0x5900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x5c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0x8000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0xa000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) 
										| (0xb000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))))) {
										if (
										(0x5800U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x40U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5900U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x41U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5a00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x42U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5b00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x43U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x5c00U 
										== 
										(0xff00U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x44U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 4U;
										} else {
										if (
										(0x8000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										if (
										(0x800U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 9U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0xcU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 8U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty = 6U;
										}
										} else {
										if (
										(0xa000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x16U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x1aU;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0x17U;
										}
										}
										}
										}
										}
										}
										}
										} else {
										if (
										(0xc000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 0x10U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 5U;
										} else {
										if (
										(0xe000U 
										== 
										(0xf000U 
										& (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)))) {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 5U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 5U;
										} else {
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmd = 1U;
										vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid = 0U;
										}
										}
										}
										}
									    }
									}
								    }
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0xfU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x28U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((0x10U & ((IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty) 
					 << 2U)) | 
			       (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0xfU;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x20U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((0x10U & ((IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty) 
					 << 2U)) | 
			       (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			} else {
			    if ((7U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    } else {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				    = (0x38U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			    }
			}
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x30U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))
			        ? 0x17U : 0x16U);
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))
			        ? 0x17U : 0x16U);
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (8U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty;
		    }
		}
	    }
	} else {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffff000U | (0xfffU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x28U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
		    } else {
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x14U;
			}
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x38U;
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= (0x38U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			}
		    }
		}
	    }
	}
    } else {
	if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfffffff0U | (0xfU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xfU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x10U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x18U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = (0x10U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x2aU;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (7U & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
					 >> 8U));
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= (8U | (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty));
			}
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt = 0x22U;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
				= (7U & ((IData)(vlTOPp->BsrExUnit__DOT__decIstrWord) 
					 >> 8U));
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty;
			}
		    }
		}
	    }
	} else {
	    if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xffffff00U | (0xffU 
					      & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    } else {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
		    }
		} else {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x38U;
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegO_Dfl;
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Sr;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Cr;
				} else {
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr;
				    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
					= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
				}
			    }
			} else {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM_Dfl;
			}
		    }
		}
	    } else {
		if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
		    if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opImm 
			    = ((((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord))
				  ? 0xffffffU : 0U) 
				<< 8U) | (0xffU & (IData)(vlTOPp->BsrExUnit__DOT__decIstrWord)));
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN = 0x14U;
		    } else {
			if ((4U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				= vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Cr;
			    vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			} else {
			    if ((6U == (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opBty))) {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Sr;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    } else {
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN 
				    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegN_Dfl;
				vlTOPp->BsrExUnit__DOT__decOp__DOT__opRegM = 0x3fU;
			    }
			}
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx;
		    }
		} else {
		    if (VL_LIKELY((1U & (IData)(vlTOPp->BsrExUnit__DOT__decOp__DOT__opFmid)))) {
			vlTOPp->BsrExUnit__DOT__decOp__DOT__opUIxt 
			    = vlTOPp->BsrExUnit__DOT__decOp__DOT__opUCmdIx;
		    } else {
			VL_WRITEF("BsrDecOp: Inv %x\n",
				  16,vlTOPp->BsrExUnit__DOT__decIstrWord);
		    }
		}
	    }
	}
    }
    // ALWAYS at BsrRegGpr.v:261
    if (vlTOPp->reset) {
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPc = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDhr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr = 0U;
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr = 0U;
    } else {
	if ((1U & (~ (IData)(vlTOPp->BsrExUnit__DOT__gprRegExHold)))) {
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr 
		= ((0x13U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutVbr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp 
		= ((0xfU == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPc 
		= ((0x10U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr 
		= ((0x11U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr 
		= ((0x12U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
		= ((0x14U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDhr 
		= ((0x15U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr 
		= ((0x16U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutGbr);
	    vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr 
		= ((0x17U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))
		    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal
		    : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutTbr);
	}
    }
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqA = 
	((vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA 
	  + (0xffff0000U & ((IData)((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB))) 
			    << 0x10U))) + (0xffff0000U 
					   & ((IData)((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC))) 
					      << 0x10U)));
    // ALWAYS at BsrExOp.v:787
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPqB = (IData)(
							   ((((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpA)) 
								+ 
								((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpB)) 
								 << 0x10U)) 
							       + 
							       ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpC)) 
								<< 0x10U)) 
							      + 
							      ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpD)) 
							       << 0x20U)) 
							     + 
							     ((QData)((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulPpE)) 
							      << 0x20U)) 
							    >> 0x20U));
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutVbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr;
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutGbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr;
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutTbr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr;
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__exRegNextPc = ((IData)(2U) 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPc);
    vlTOPp->BsrExUnit__DOT__exCtlInPc = vlTOPp->BsrExUnit__DOT__exRegNextPc;
    // ALWAYS at BsrExOp.v:143
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc = vlTOPp->BsrExUnit__DOT__exCtlInPc;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr = vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDhr;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId = 0x3fU;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 1U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt = (
						   (0x80000000U 
						    & vlTOPp->BsrExUnit__DOT__exRegValRn)
						    ? 0xffffffffU
						    : 0U);
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 0U;
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt = (0xfU 
						   & ((IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt) 
						      >> 4U));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty = (7U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaRbVal = 
	((8U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
	  ? vlTOPp->BsrExUnit__DOT__exRegValRm : vlTOPp->BsrExUnit__DOT__exRegValRn);
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal = 
	((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	  ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	      ? ((0xfffffff0U & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
				 << 4U)) | (0xfU & vlTOPp->BsrExUnit__DOT__exImmValRi))
	      : vlTOPp->BsrExUnit__DOT__exImmValRi)
	  : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxt))
	      ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr
	      : 0U));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal2 
	= ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	        ? (0xfffffff8U & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
				  << 3U)) : (0xfffffffcU 
					     & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
						<< 2U)))
	    : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaBty))
	        ? (0xfffffffeU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal 
				  << 1U)) : vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal));
    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr = 
	(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaRbVal 
	 + vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaIxVal2);
    if ((0x80U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
	vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
	vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
	if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
	    VL_WRITEF("BsrExOp: Missing %x\n",8,vlTOPp->BsrExUnit__DOT__exOpCmd);
	}
    } else {
	if ((0x40U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
	    if ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
		    VL_WRITEF("BsrExOp: Missing %x\n",
			      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
		}
		vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
		vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
	    } else {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
			VL_WRITEF("BsrExOp: Missing %x\n",
				  8,vlTOPp->BsrExUnit__DOT__exOpCmd);
		    }
		    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
		    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if (VL_UNLIKELY((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
			    VL_WRITEF("BsrExOp: Missing %x\n",
				      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
			}
			vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
			vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if (VL_UNLIKELY((1U 
						 != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
				    VL_WRITEF("BsrExOp: Missing %x\n",
					      8,vlTOPp->BsrExUnit__DOT__exOpCmd);
				}
				vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= ((0xffff0000U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
					       << 0x10U)) 
					   | (0xffffU 
					      & vlTOPp->BsrExUnit__DOT__exImmValRi));
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   ^ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   | vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   - vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRm 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		}
	    }
	} else {
	    if ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xffff0000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x10U)) 
						      | (0xffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 0x10U)))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xff000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x18U)) 
						      | (0xffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 8U))))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xf0000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1cU)) 
						      | (0xfffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 4U)))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     ((0xc0000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1eU)) 
						      | (0x3fffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 2U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     ((0x80000000U 
						       & (vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt 
							  << 0x1fU)) 
						      | (0x7fffffffU 
							 & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							    >> 1U)))
						      : 0U)))));
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 0x10U))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 8U)))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 4U))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0x3fffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 2U)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     (0x7fffffffU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 >> 1U))
						      : 0U)))));
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					    ? ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? 0U
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffff0000U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 0x10U))))))
					    : ((8U 
						& (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
					        ? (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 0U
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xffffff00U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 8U)))))
					        : (
						   (4U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 0U
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffff0U 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 4U))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 0U
						      : 
						     (0xfffffffcU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 2U)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
						      ? 
						     (0xfffffffeU 
						      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
							 << 1U))
						      : 0U)))));
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & vlTOPp->BsrExUnit__DOT__exRegValRn) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0x7fffffffU 
					   & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					      >> 1U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xfffffffeU 
					   & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					      << 1U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr 
					       << 0x1fU)) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0xfffffffeU 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 1U)) 
					   | (1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0x80000000U 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 0x1fU)) 
					   | (0x7fffffffU 
					      & (vlTOPp->BsrExUnit__DOT__exRegValRn 
						 >> 1U)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((0xfffffffeU 
					    & (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       << 1U)) 
					   | (1U & 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       >> 0x1fU)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regRnSxt;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((~ vlTOPp->BsrExUnit__DOT__exRegValRn) 
					   + (1U & 
					      (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= VL_NEGATE_I(vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (~ vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((((0x8000U 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn)
					      ? 0xffffU
					      : 0U) 
					    << 0x10U) 
					   | (0xffffU 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((((0x80U 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn)
					      ? 0xffffffU
					      : 0U) 
					    << 8U) 
					   | (0xffU 
					      & vlTOPp->BsrExUnit__DOT__exRegValRn));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xffffU 
					   & vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (0xffU & vlTOPp->BsrExUnit__DOT__exRegValRn);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if ((1U & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exRegValRn;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				} else {
				    if ((1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exRegValRn;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr 
					= vlTOPp->BsrExUnit__DOT__exIdInGenPc;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp 
					= ((IData)(4U) 
					   + vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xaU;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				    if ((0x10U == (IData)(vlTOPp->BsrExUnit__DOT__exRegIdRn))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
					if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
					}
					if ((0U == vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2)) {
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					}
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					    = vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp 
					= (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSp 
					   - (IData)(4U));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x12U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if ((1U & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				} else {
				    if ((1U & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					    = vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPr 
					= vlTOPp->BsrExUnit__DOT__exIdInGenPc;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
				}
			    }
			}
		    }
		}
	    } else {
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
				= ((0xfffffffeU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
				   | (1U & ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
					     ? ((1U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
						 ? 
						((vlTOPp->BsrExUnit__DOT__exRegValRn 
						  >= vlTOPp->BsrExUnit__DOT__exRegValRm) 
						 ^ 
						 ((vlTOPp->BsrExUnit__DOT__exRegValRn 
						   ^ vlTOPp->BsrExUnit__DOT__exRegValRm) 
						  >> 0x1fU))
						 : 
						((vlTOPp->BsrExUnit__DOT__exRegValRn 
						  > vlTOPp->BsrExUnit__DOT__exRegValRm) 
						 ^ 
						 ((vlTOPp->BsrExUnit__DOT__exRegValRn 
						   ^ vlTOPp->BsrExUnit__DOT__exRegValRm) 
						  >> 0x1fU)))
					     : ((1U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
						 ? 
						(vlTOPp->BsrExUnit__DOT__exRegValRn 
						 > vlTOPp->BsrExUnit__DOT__exRegValRm)
						 : 
						(vlTOPp->BsrExUnit__DOT__exRegValRn 
						 == vlTOPp->BsrExUnit__DOT__exRegValRm)))));
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
				    = ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))
				        ? ((0xffffff00U 
					    & (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regDlr 
					       << 8U)) 
					   | (0xffU 
					      & vlTOPp->BsrExUnit__DOT__exImmValRi))
				        : vlTOPp->BsrExUnit__DOT__exImmValRi);
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr;
				    if ((2U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulA 
					= vlTOPp->BsrExUnit__DOT__exRegValRn;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulB 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDlr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulDhr;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    }
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   ^ vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   | vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   & vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (0U == 
					      (vlTOPp->BsrExUnit__DOT__exRegValRn 
					       & vlTOPp->BsrExUnit__DOT__exRegValRm)));
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & (IData)(
							   (VL_ULL(1) 
							    & ((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRn)) 
								 + (QData)((IData)(
										(~ vlTOPp->BsrExUnit__DOT__exRegValRm)))) 
								+ (QData)((IData)(
										(1U 
										& (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))))) 
							       >> 0x20U)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((vlTOPp->BsrExUnit__DOT__exRegValRn 
					    + (~ vlTOPp->BsrExUnit__DOT__exRegValRm)) 
					   + (IData)((QData)((IData)(
								     (1U 
								      & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
					= ((0xfffffffeU 
					    & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
					   | (1U & (IData)(
							   (VL_ULL(1) 
							    & ((((QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRn)) 
								 + (QData)((IData)(vlTOPp->BsrExUnit__DOT__exRegValRm))) 
								+ (QData)((IData)(
										(1U 
										& vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)))) 
							       >> 0x20U)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= ((vlTOPp->BsrExUnit__DOT__exRegValRn 
					    + vlTOPp->BsrExUnit__DOT__exRegValRm) 
					   + (IData)((QData)((IData)(
								     (1U 
								      & vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)))));
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   - vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= (vlTOPp->BsrExUnit__DOT__exRegValRn 
					   + vlTOPp->BsrExUnit__DOT__exRegValRm);
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			}
		    }
		} else {
		    if ((8U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xdU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xcU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0xaU;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x12U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 9U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x11U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 8U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData2;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0x10U;
				    if ((1U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 0U;
				    }
				}
			    }
			}
		    } else {
			if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exOp__DOT__regMaAddr;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exImmValRi;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				} else {
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal 
					= vlTOPp->BsrExUnit__DOT__exRegValRm;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId 
					= vlTOPp->BsrExUnit__DOT__exRegIdRn;
				}
			    }
			} else {
			    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (0x80U 
						     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
					VL_WRITEF("BsrExOp: Missing %x-%x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd,
						  8,
						  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
					vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    } else {
					if (VL_UNLIKELY(
							(0x40U 
							 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
					    VL_WRITEF("BsrExOp: Missing %x-%x\n",
						      8,
						      vlTOPp->BsrExUnit__DOT__exOpCmd,
						      8,
						      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
					    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					} else {
					    if (VL_UNLIKELY(
							    (0x20U 
							     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
						VL_WRITEF("BsrExOp: Missing %x-%x\n",
							  8,
							  vlTOPp->BsrExUnit__DOT__exOpCmd,
							  8,
							  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
						vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
					    } else {
						if (VL_UNLIKELY(
								(0x10U 
								 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
						    VL_WRITEF("BsrExOp: Missing %x-%x\n",
							      8,
							      vlTOPp->BsrExUnit__DOT__exOpCmd,
							      8,
							      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
						    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
						} else {
						    if (
							(8U 
							 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
							if (VL_UNLIKELY(
									(4U 
									 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
							    VL_WRITEF("BsrExOp: Missing %x-%x\n",
								      8,
								      vlTOPp->BsrExUnit__DOT__exOpCmd,
								      8,
								      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
							    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
							} else {
							    if (VL_UNLIKELY(
									    (2U 
									     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
								VL_WRITEF("BsrExOp: Missing %x-%x\n",
									  8,
									  vlTOPp->BsrExUnit__DOT__exOpCmd,
									  8,
									  (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
								vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
							    } else {
								vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
								    = 
								    ((1U 
								      & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
								      ? 
								     ((0xfffffffdU 
								       & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								      | (2U 
									 & ((~ 
									     (vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr 
									      >> 1U)) 
									    << 1U)))
								      : 
								     ((0xfffffffeU 
								       & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								      | (1U 
									 & (~ vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr))));
							    }
							}
						    } else {
							if (
							    (4U 
							     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
							    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr 
								= 
								((2U 
								  & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))
								  ? 
								 ((0xfffffffdU 
								   & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								  | (2U 
								     & ((IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt) 
									<< 1U)))
								  : 
								 ((0xfffffffeU 
								   & vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSr) 
								  | (1U 
								     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))));
							} else {
							    if (
								(2U 
								 & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
								if (VL_LIKELY(
									      (1U 
									       & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt)))) {
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
								} else {
								    VL_WRITEF("BsrExOp: Missing %x-%x\n",
									      8,
									      vlTOPp->BsrExUnit__DOT__exOpCmd,
									      8,
									      (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt));
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
								}
							    } else {
								if (
								    (1U 
								     & (IData)(vlTOPp->BsrExUnit__DOT__exRegIdIxt))) {
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutPc 
									= vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr;
								    vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm = 2U;
								}
							    }
							}
						    }
						}
					    }
					}
				    }
				} else {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				}
			    } else {
				if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOpCmd))) {
				    if (VL_UNLIKELY(
						    (1U 
						     != (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOpDr)))) {
					VL_WRITEF("BsrExOp: Missing %x\n",
						  8,
						  vlTOPp->BsrExUnit__DOT__exOpCmd);
				    }
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK = 2U;
				    vlTOPp->BsrExUnit__DOT__exOp__DOT__regMulOp = 1U;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__exHold = 0U;
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutOK))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK2))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    if ((2U == (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__tRegOutPcOK))) {
	vlTOPp->BsrExUnit__DOT__exHold = 1U;
    }
    vlTOPp->BsrExUnit__DOT__gprRegExHold = vlTOPp->BsrExUnit__DOT__exHold;
    // ALWAYS at BsrRegGpr.v:150
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRn 
	= ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
	    ? 0U : ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
		     ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			 ? 0U : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				  ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				      ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr
					  : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr)
				      : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr
					  : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr))
				  : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				      ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr
					  : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)
				      : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
					  ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr
					  : ((IData)(2U) 
					     + vlTOPp->BsrExUnit__DOT__gprIdValPc)))))
		     : ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			 ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			     ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12))
			     : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8)))
			 : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
			     ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A))
			     : ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				 ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A)
				 : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn))
				     ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A
				     : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A))))));
    if (((IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRn) 
	 == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))) {
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRn 
	    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal;
    }
    // ALWAYS at BsrRegGpr.v:150
    vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRm 
	= ((0x20U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
	    ? ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
	        ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
		    ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
		        ? ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			    ? ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			        ? 0U : 0U) : 0U) : 
		       ((2U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
			 ? 0U : ((1U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				  ? 0U : vlTOPp->BsrExUnit__DOT__gprIdImm)))
		    : 0U) : 0U) : ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				    ? ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				        ? 0U : ((4U 
						 & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						 ? 
						((2U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						  ? 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regTbr
						   : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGbr)
						  : 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDhr
						   : vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutDlr))
						 : 
						((2U 
						  & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						  ? 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regVbr
						   : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regSr)
						  : 
						 ((1U 
						   & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						   ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regPr
						   : 
						  ((IData)(2U) 
						   + vlTOPp->BsrExUnit__DOT__gprIdValPc)))))
				    : ((8U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
				        ? ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					    ? ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__exOp__DOT__tCtlOutSp
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR14)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR13
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR12))
					    : ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR11
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR10)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR9
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR8)))
				        : ((4U & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					    ? ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR7A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR6A)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR5A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR4A))
					    : ((2U 
						& (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
					        ? (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR3A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR2A)
					        : (
						   (1U 
						    & (IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm))
						    ? vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR1A
						    : vlTOPp->BsrExUnit__DOT__gprFile__DOT__regGprR0A))))));
    if (((IData)(vlTOPp->BsrExUnit__DOT__gprRegIdRm) 
	 == (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutId))) {
	vlTOPp->BsrExUnit__DOT__gprFile__DOT__tRegValRm 
	    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tRegOutVal;
    }
}

void VBsrExUnit::_initial__TOP__3(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_initial__TOP__3\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp7,127,0,4);
    VL_SIGW(__Vtemp8,127,0,4);
    VL_SIGW(__Vtemp9,127,0,4);
    VL_SIGW(__Vtemp10,127,0,4);
    // Body
    // INITIAL at BsrMemTileB.v:145
    __Vtemp7[0U] = 0x2e747874U;
    __Vtemp7[1U] = 0x6d5f3161U;
    __Vtemp7[2U] = 0x6f74726fU;
    __Vtemp7[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp7, vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileA
		  ,0,~0);
    __Vtemp8[0U] = 0x2e747874U;
    __Vtemp8[1U] = 0x6d5f3162U;
    __Vtemp8[2U] = 0x6f74726fU;
    __Vtemp8[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp8, vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileB
		  ,0,~0);
    __Vtemp9[0U] = 0x2e747874U;
    __Vtemp9[1U] = 0x6d5f3163U;
    __Vtemp9[2U] = 0x6f74726fU;
    __Vtemp9[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp9, vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileC
		  ,0,~0);
    __Vtemp10[0U] = 0x2e747874U;
    __Vtemp10[1U] = 0x6d5f3164U;
    __Vtemp10[2U] = 0x6f74726fU;
    __Vtemp10[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp10, vlTOPp->BsrExUnit__DOT__memTile__DOT__romTileD
		  ,0,~0);
}

VL_INLINE_OPT void VBsrExUnit::_combo__TOP__4(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_combo__TOP__4\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp13,127,0,4);
    // Body
    // ALWAYS at BsrExUnit.v:271
    vlTOPp->BsrExUnit__DOT__icMemPcOK = 2U;
    vlTOPp->BsrExUnit__DOT__dcMemPcOK = 2U;
    vlTOPp->BsrExUnit__DOT__icMemPcData[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrExUnit__DOT__icMemPcData[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrExUnit__DOT__memRegInAddr = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInData[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memRegInOpm = 0U;
    if ((0U != (IData)(vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm))) {
	vlTOPp->BsrExUnit__DOT__icMemPcData[0U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[1U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[2U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
	vlTOPp->BsrExUnit__DOT__icMemPcData[3U] = vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
	vlTOPp->BsrExUnit__DOT__icMemPcOK = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK;
	vlTOPp->BsrExUnit__DOT__memRegInAddr = vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcAddr;
	vlTOPp->BsrExUnit__DOT__memRegInData[0U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[1U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[2U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInData[3U] = 0U;
	vlTOPp->BsrExUnit__DOT__memRegInOpm = vlTOPp->BsrExUnit__DOT__icTile__DOT__tMemPcOpm;
    } else {
	if ((0U != (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm))) {
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[1U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[2U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcDataI[3U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
	    vlTOPp->BsrExUnit__DOT__dcMemPcOK = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK;
	    vlTOPp->BsrExUnit__DOT__memRegInAddr = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcAddr;
	    vlTOPp->BsrExUnit__DOT__memRegInData[0U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[0U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[1U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[1U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[2U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[2U];
	    vlTOPp->BsrExUnit__DOT__memRegInData[3U] 
		= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcDataO[3U];
	    vlTOPp->BsrExUnit__DOT__memRegInOpm = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tMemPcOpm;
	} else {
	    vlTOPp->BsrExUnit__DOT__icMemPcOK = 0U;
	    vlTOPp->BsrExUnit__DOT__dcMemPcOK = 0U;
	}
    }
    // ALWAYS at BsrDcTileB.v:84
    if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
		      >> 3U)))) {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss 
	    = (1U & ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkAddr 
		      != (0x1ffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
				      >> 3U))) | (~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady))));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr 
	    = (0x1ffffU & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
			   >> 3U));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass 
	    = (7U == (7U & (vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr 
			    >> 0xdU)));
    } else {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss = 0U;
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass = 0U;
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icReqAddr = 0U;
    }
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore 
	= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB;
    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 0U;
    if (((((~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkMiss)) 
	   & (~ (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass))) 
	  & (IData)(vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkReady)) 
	 & (0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			 >> 3U))))) {
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData 
	    = ((4U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
	        ? ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		    ? ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		        ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			    << 8U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x10U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		     ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x18U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 8U)) : vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U]))
	        : ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		    ? ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		        ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			    << 8U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x10U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)
		     ? ((vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x18U) | (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 8U)) : vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U])));
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore = 0U;
	if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			  >> 3U)))) {
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 1U;
	}
	if ((4U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
	    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
		    = vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
	    } else {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= (0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= (0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		}
	    }
	} else {
	    if ((2U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData;
		}
	    } else {
		if ((1U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= ((((0x8000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffU : 0U) << 0x10U) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
			= ((((0x80U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffffU : 0U) << 8U) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemData));
		}
	    }
	}
	if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm))) {
	    vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore = 1U;
	}
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[0U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[1U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[2U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[3U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[3U];
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[4U] 
	    = vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkData[4U];
	if ((4U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
	    if ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xff000000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xff000000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffff0000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffff00U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	} else {
	    if ((2U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xff000000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xff000000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffff0000U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffff0000U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemAddr)) {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffU & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffffff00U & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffff00U & vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffU & (vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	}
	vlTOPp->BsrExUnit__DOT__dcTile__DOT__tIcBlkStore 
	    = (1U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
		     >> 4U));
    } else {
	if (vlTOPp->BsrExUnit__DOT__dcTile__DOT__icBlkBypass) {
	    if ((1U == (IData)(vlTOPp->BsrExUnit__DOT__dcMemPcOK))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 1U;
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrExUnit__DOT__dcMemPcDataI[0U];
	    } else {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	} else {
	    if ((0U != (3U & ((IData)(vlTOPp->BsrExUnit__DOT__exOp__DOT__tMemOpm) 
			      >> 3U)))) {
		vlTOPp->BsrExUnit__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	}
    }
    vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam 
	= ((0xc000U <= vlTOPp->BsrExUnit__DOT__memRegInAddr) 
	   & (0xe000U > vlTOPp->BsrExUnit__DOT__memRegInAddr));
    // ALWAYS at BsrMemTileB.v:156
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] = 0U;
    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB 
	    = (0xfffU & (vlTOPp->BsrExUnit__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB)));
    } else {
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & (vlTOPp->BsrExUnit__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxB 
	    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA;
    }
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileSrSt = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK = 0U;
    vlTOPp->BsrExUnit__DOT__memTile__DOT__mmioBypass = 0U;
    if ((1U & (((IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm) 
		>> 3U) | ((IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm) 
			  >> 4U)))) {
	if (((0x8000U >= vlTOPp->BsrExUnit__DOT__memRegInAddr) 
	     & (~ (vlTOPp->BsrExUnit__DOT__memRegInAddr 
		   >> 0xeU)))) {
	    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
	    }
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[0U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[1U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[2U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
		= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRomTile[3U];
	    vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		= (((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
		    == (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA))
		    ? 1U : 2U);
	    if ((2U == (3U & (IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm)))) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		if ((4U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U]))) 
				   >> 0x20U));
		}
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
	    }
	} else {
	    if (vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam) {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileA
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		    = (((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
			== (IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegTileIxA))
		        ? 1U : 2U);
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileB
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileC
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__sramTileD
		    [(0x1ffU & ((IData)(vlTOPp->BsrExUnit__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		}
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tSRamTile[3U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tMemTile[3U];
		if ((0x10U & (IData)(vlTOPp->BsrExUnit__DOT__memRegInOpm))) {
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[0U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[1U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[2U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U] 
			= vlTOPp->BsrExUnit__DOT__memRegInData[3U];
		    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileSrSt 
			= vlTOPp->BsrExUnit__DOT__memTile__DOT__addrIsSRam;
		    if ((8U & vlTOPp->BsrExUnit__DOT__memRegInAddr)) {
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
		    } else {
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrExUnit__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
		    }
		}
	    } else {
		vlTOPp->BsrExUnit__DOT__memTile__DOT__mmioBypass = 1U;
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK 
		    = vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioOK;
		VL_EXTEND_WI(128,32, __Vtemp13, vlTOPp->BsrExUnit__DOT__memTile__DOT__tmpMmioInData);
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
		    = __Vtemp13[0U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
		    = __Vtemp13[1U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
		    = __Vtemp13[2U];
		vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
		    = __Vtemp13[3U];
	    }
	}
    }
}

void VBsrExUnit::_eval(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_eval\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void VBsrExUnit::_eval_initial(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_eval_initial\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__3(vlSymsp);
}

void VBsrExUnit::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::final\n"); );
    // Variables
    VBsrExUnit__Syms* __restrict vlSymsp = this->__VlSymsp;
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VBsrExUnit::_eval_settle(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_eval_settle\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__1(vlSymsp);
}

VL_INLINE_OPT QData VBsrExUnit::_change_request(VBsrExUnit__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_change_request\n"); );
    VBsrExUnit* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[0]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[1]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[2]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[3])
	 | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tRegOutOK));
    VL_DEBUG_IF( if(__req && ((vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[0]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[1]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[2]) | (vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3] ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[3]))) VL_DBG_MSGF("        CHANGE: BsrMemTileB.v:101: BsrExUnit.memTile.tOutData\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK ^ vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tRegOutOK))) VL_DBG_MSGF("        CHANGE: BsrMemTileB.v:112: BsrExUnit.memTile.tRegOutOK\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[0U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[1U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[2U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData[3U] 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tRegOutOK 
	= vlTOPp->BsrExUnit__DOT__memTile__DOT__tRegOutOK;
    return __req;
}

#ifdef VL_DEBUG
void VBsrExUnit::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clock & 0xfeU))) {
	Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((reset & 0xfeU))) {
	Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((memOK & 0xfcU))) {
	Verilated::overWidthError("memOK");}
    if (VL_UNLIKELY((mmioOK & 0xfcU))) {
	Verilated::overWidthError("mmioOK");}
}
#endif // VL_DEBUG

void VBsrExUnit::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrExUnit::_ctor_var_reset\n"); );
    // Body
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,memInData);
    VL_RAND_RESET_W(128,memOutData);
    memAddr = VL_RAND_RESET_I(32);
    memOpm = VL_RAND_RESET_I(5);
    memOK = VL_RAND_RESET_I(2);
    mmioInData = VL_RAND_RESET_I(32);
    mmioOutData = VL_RAND_RESET_I(32);
    mmioAddr = VL_RAND_RESET_I(32);
    mmioOpm = VL_RAND_RESET_I(5);
    mmioOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__memRegInAddr = VL_RAND_RESET_I(20);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memRegInData);
    BsrExUnit__DOT__memRegInOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__dcMemPcDataI);
    BsrExUnit__DOT__dcMemPcOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__icRegInPc = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__icMemPcData);
    BsrExUnit__DOT__icMemPcOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__decRegPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__decIstrWord = VL_RAND_RESET_I(16);
    BsrExUnit__DOT__gprIdUCmd = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__gprIdUIxt = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__gprRegIdRm = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__gprRegIdRn = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__gprRegExHold = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__gprIdImm = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprIdValPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOpCmd = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__exRegIdIxt = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__exRegIdRn = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__exRegValRm = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exRegValRn = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exImmValRi = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exIdInGenPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exCtlInPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exHold = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__exBranchFlush = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__exRegNextPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__memTile__DOT__tMmioOutData = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__memTile__DOT__tMmioAddr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__memTile__DOT__tMmioOpm = VL_RAND_RESET_I(5);
    BsrExUnit__DOT__memTile__DOT__tmpMmioInData = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__memTile__DOT__tmpMmioOK = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__romTileA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__romTileB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__romTileC[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__romTileD[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__sramTileA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__sramTileB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__sramTileC[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrExUnit__DOT__memTile__DOT__sramTileD[__Vi0] = VL_RAND_RESET_I(32);
    }}
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tRomTile);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tSRamTile);
    BsrExUnit__DOT__memTile__DOT__tAccTileIxA = VL_RAND_RESET_I(12);
    BsrExUnit__DOT__memTile__DOT__tAccTileIxB = VL_RAND_RESET_I(12);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tMemTile);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tOutData);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tNextTile);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__memTile__DOT__tNextTileA);
    BsrExUnit__DOT__memTile__DOT__tRegTileIxA = VL_RAND_RESET_I(12);
    BsrExUnit__DOT__memTile__DOT__tRegTileIxB = VL_RAND_RESET_I(12);
    BsrExUnit__DOT__memTile__DOT__tNextTileSrSt = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__memTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__memTile__DOT__addrIsSRam = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__memTile__DOT__mmioBypass = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__dcTile__DOT__tRegOutData = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__dcTile__DOT__tRegOutData2 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__dcTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__dcTile__DOT__tRegOutOK2 = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__dcTile__DOT__tMemPcAddr = VL_RAND_RESET_I(20);
    VL_RAND_RESET_W(128,BsrExUnit__DOT__dcTile__DOT__tMemPcDataO);
    BsrExUnit__DOT__dcTile__DOT__tMemPcOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(160,BsrExUnit__DOT__dcTile__DOT__icBlkData);
    BsrExUnit__DOT__dcTile__DOT__icBlkAddr = VL_RAND_RESET_I(17);
    BsrExUnit__DOT__dcTile__DOT__icBlkDirty = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__dcTile__DOT__icBlkReady = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(160,BsrExUnit__DOT__dcTile__DOT__icBlkDataSt);
    BsrExUnit__DOT__dcTile__DOT__icBlkMiss = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__dcTile__DOT__icBlkBypass = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__dcTile__DOT__icReqAddr = VL_RAND_RESET_I(17);
    BsrExUnit__DOT__dcTile__DOT__tRegTmpData = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__dcTile__DOT__tIcBlkStore = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__icTile__DOT__tRegOutPcVal = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__icTile__DOT__tRegOutPcOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__icTile__DOT__tMemPcAddr = VL_RAND_RESET_I(20);
    BsrExUnit__DOT__icTile__DOT__tMemPcOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(160,BsrExUnit__DOT__icTile__DOT__icBlkData);
    BsrExUnit__DOT__icTile__DOT__icBlkAddr = VL_RAND_RESET_I(17);
    BsrExUnit__DOT__icTile__DOT__icBlkReady = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__icTile__DOT__icBlkMiss = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__icTile__DOT__icBlkHi = VL_RAND_RESET_I(1);
    BsrExUnit__DOT__icTile__DOT__icBlkWordIx = VL_RAND_RESET_I(3);
    BsrExUnit__DOT__decOp__DOT__opRegN = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegM = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opImm = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__decOp__DOT__opUCmd = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__decOp__DOT__opUIxt = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__decOp__DOT__opRegO_Dfl = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegN_Dfl = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegM_Dfl = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegN_Cr = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegM_Cr = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegN_Sr = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opRegM_Sr = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__decOp__DOT__opFmid = VL_RAND_RESET_I(5);
    BsrExUnit__DOT__decOp__DOT__opBty = VL_RAND_RESET_I(3);
    BsrExUnit__DOT__decOp__DOT__opUCmdIx = VL_RAND_RESET_I(8);
    BsrExUnit__DOT__gprFile__DOT__regSp = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regPr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regSr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regVbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regDlr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regDhr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regTbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__tRegValRm = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__tRegValRn = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR0A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR1A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR2A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR3A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR4A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR5A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR6A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR7A = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR8 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR9 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR10 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR11 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR12 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR13 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__gprFile__DOT__regGprR14 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutSp = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutPc = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutPr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutSr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutVbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutDlr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutDhr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutGbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tCtlOutTbr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tRegOutVal = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tRegOutId = VL_RAND_RESET_I(6);
    BsrExUnit__DOT__exOp__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__exOp__DOT__tMemAddr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tMemData = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__tMemOpm = VL_RAND_RESET_I(5);
    BsrExUnit__DOT__exOp__DOT__regMaRbVal = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMaIxVal = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMaIxVal2 = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMaIxt = VL_RAND_RESET_I(4);
    BsrExUnit__DOT__exOp__DOT__regMaBty = VL_RAND_RESET_I(3);
    BsrExUnit__DOT__exOp__DOT__regMaAddr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regRnSxt = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulA = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulB = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulOp = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__exOp__DOT__regMulDlr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulDhr = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulOpDr = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__exOp__DOT__regMulPpA = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulPpB = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulPpC = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulPpD = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulPpE = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulOpPp = VL_RAND_RESET_I(2);
    BsrExUnit__DOT__exOp__DOT__regMulPqA = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulPqB = VL_RAND_RESET_I(32);
    BsrExUnit__DOT__exOp__DOT__regMulOpPq = VL_RAND_RESET_I(2);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData);
    __Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
}

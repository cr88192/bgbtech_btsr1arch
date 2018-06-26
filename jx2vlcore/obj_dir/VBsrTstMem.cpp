// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VBsrTstMem.h for the primary calling header

#include "VBsrTstMem.h"        // For This
#include "VBsrTstMem__Syms.h"


//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(VBsrTstMem) {
    VBsrTstMem__Syms* __restrict vlSymsp = __VlSymsp = new VBsrTstMem__Syms(this, name());
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VBsrTstMem::__Vconfigure(VBsrTstMem__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

VBsrTstMem::~VBsrTstMem() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void VBsrTstMem::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VBsrTstMem::eval\n"); );
    VBsrTstMem__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void VBsrTstMem::_eval_initial_loop(VBsrTstMem__Syms* __restrict vlSymsp) {
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

void VBsrTstMem::_settle__TOP__1(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_settle__TOP__1\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp3,127,0,4);
    // Body
    vlTOPp->memOutData[0U] = 0U;
    vlTOPp->memOutData[1U] = 0U;
    vlTOPp->memOutData[2U] = 0U;
    vlTOPp->memOutData[3U] = 0U;
    vlTOPp->memAddr = 0U;
    vlTOPp->memOpm = 0U;
    // ALWAYS at BsrTstMem.v:148
    vlTOPp->BsrTstMem__DOT__dcRegInAddr = vlTOPp->regInAddr;
    vlTOPp->BsrTstMem__DOT__dcRegInOpm = vlTOPp->regInOpm;
    vlTOPp->BsrTstMem__DOT__dcRegInData = vlTOPp->regInData;
    vlTOPp->regOutOK = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK2;
    vlTOPp->mmioOutData = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOutData;
    vlTOPp->mmioAddr = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioAddr;
    vlTOPp->mmioOpm = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm;
    // ALWAYS at BsrIcTileB.v:48
    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkMiss 
	= (1U & (((vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr 
		   != (0x1ffffU & (vlTOPp->BsrTstMem__DOT__icRegInPc 
				   >> 3U))) & (((IData)(1U) 
						+ vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr) 
					       != (0x1ffffU 
						   & (vlTOPp->BsrTstMem__DOT__icRegInPc 
						      >> 3U)))) 
		 | (~ (IData)(vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkReady))));
    // ALWAYS at BsrIcTileB.v:48
    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkHi = 
	((1U & vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr) 
	 != (1U & (vlTOPp->BsrTstMem__DOT__icRegInPc 
		   >> 3U)));
    // ALWAYS at BsrTstMem.v:148
    vlTOPp->BsrTstMem__DOT__icMemPcOK = 2U;
    vlTOPp->BsrTstMem__DOT__dcMemPcOK = 2U;
    vlTOPp->BsrTstMem__DOT__icMemPcData[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrTstMem__DOT__memRegInAddr = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInOpm = 0U;
    if ((0U != (IData)(vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm))) {
	vlTOPp->BsrTstMem__DOT__icMemPcData[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
	vlTOPp->BsrTstMem__DOT__icMemPcOK = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK;
	vlTOPp->BsrTstMem__DOT__memRegInAddr = vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcAddr;
	vlTOPp->BsrTstMem__DOT__memRegInData[0U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[1U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[2U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[3U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInOpm = vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm;
    } else {
	if ((0U != (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm))) {
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[1U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[2U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[3U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcOK = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK;
	    vlTOPp->BsrTstMem__DOT__memRegInAddr = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr;
	    vlTOPp->BsrTstMem__DOT__memRegInData[0U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[1U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[2U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[3U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U];
	    vlTOPp->BsrTstMem__DOT__memRegInOpm = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm;
	} else {
	    vlTOPp->BsrTstMem__DOT__icMemPcOK = 0U;
	    vlTOPp->BsrTstMem__DOT__dcMemPcOK = 0U;
	}
    }
    // ALWAYS at BsrDcTileB.v:82
    if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
		      >> 3U)))) {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss 
	    = (1U & ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
		      != (0x1ffffU & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
				      >> 3U))) | (~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady))));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr 
	    = (0x1ffffU & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
			   >> 3U));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass 
	    = (7U == (7U & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
			    >> 0xdU)));
    } else {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss = 0U;
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass = 0U;
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr = 0U;
    }
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB;
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 0U;
    if (((((~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss)) 
	   & (~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass))) 
	  & (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady)) 
	 & (0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			 >> 3U))))) {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData 
	    = ((4U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
	        ? ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		    ? ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		        ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			    << 8U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x10U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		     ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x18U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 8U)) : vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U]))
	        : ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		    ? ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		        ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			    << 8U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x10U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		     ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x18U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 8U)) : vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U])));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore = 0U;
	if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			  >> 3U)))) {
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 1U;
	}
	if ((4U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
	    if ((2U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
		    = vlTOPp->BsrTstMem__DOT__dcRegInData;
	    } else {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= (0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= (0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		}
	    }
	} else {
	    if ((2U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrTstMem__DOT__dcRegInData;
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrTstMem__DOT__dcRegInData;
		}
	    } else {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= ((((0x8000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffU : 0U) << 0x10U) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= ((((0x80U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffffU : 0U) << 8U) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		}
	    }
	}
	if ((0x10U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore = 1U;
	}
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[3U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[4U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[4U];
	if ((4U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
	    if ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xff000000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xff000000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffff0000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffff00U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	} else {
	    if ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xff000000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xff000000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffff0000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffffff00U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	}
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore 
	    = (1U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
		     >> 4U));
    } else {
	if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass) {
	    if ((1U == (IData)(vlTOPp->BsrTstMem__DOT__dcMemPcOK))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 1U;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U];
	    } else {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	} else {
	    if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			      >> 3U)))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	}
    }
    vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam 
	= ((0xc000U <= vlTOPp->BsrTstMem__DOT__memRegInAddr) 
	   & (0xe000U > vlTOPp->BsrTstMem__DOT__memRegInAddr));
    vlTOPp->regOutData = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData;
    // ALWAYS at BsrMemTileB.v:156
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] = 0U;
    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB 
	    = (0xfffU & (vlTOPp->BsrTstMem__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB)));
    } else {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & (vlTOPp->BsrTstMem__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA;
    }
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileSrSt = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__mmioBypass = 0U;
    if ((1U & (((IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm) 
		>> 3U) | ((IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm) 
			  >> 4U)))) {
	if (((0x8000U >= vlTOPp->BsrTstMem__DOT__memRegInAddr) 
	     & (~ (vlTOPp->BsrTstMem__DOT__memRegInAddr 
		   >> 0xeU)))) {
	    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
	    } else {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
	    }
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		= (((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
		    == (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA))
		    ? 1U : 2U);
	    if ((2U == (3U & (IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm)))) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		if ((4U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U]))) 
				   >> 0x20U));
		}
	    } else {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
	    }
	} else {
	    if (vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileA
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		    = (((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
			== (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA))
		        ? 1U : 2U);
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileB
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileC
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileD
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		}
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		if ((0x10U & (IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[0U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[1U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[2U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[3U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileSrSt 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam;
		    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
		    } else {
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
		    }
		}
	    } else {
		VL_EXTEND_WI(128,32, __Vtemp3, vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioInData);
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = __Vtemp3[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = __Vtemp3[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = __Vtemp3[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = __Vtemp3[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__mmioBypass = 1U;
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioOK;
		if (VL_UNLIKELY((0U != (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm)))) {
		    VL_WRITEF("DcTile2 Bypass A=%x O=%x DO=%x DI=%x\n",
			      20,vlTOPp->BsrTstMem__DOT__memRegInAddr,
			      5,(IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm),
			      32,vlTOPp->BsrTstMem__DOT__memRegInData[0U],
			      32,vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U]);
		}
	    }
	}
    }
}

VL_INLINE_OPT void VBsrTstMem::_sequent__TOP__2(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_sequent__TOP__2\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0,0,0);
    VL_SIG8(__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty,0,0);
    VL_SIG8(__Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady,0,0);
    VL_SIG16(__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileA__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileB__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileC__v0,8,0);
    VL_SIG16(__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileD__v0,8,0);
    VL_SIG(__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileA__v0,31,0);
    VL_SIG(__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileB__v0,31,0);
    VL_SIG(__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileC__v0,31,0);
    VL_SIG(__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileD__v0,31,0);
    VL_SIGW(__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData,159,0,5);
    VL_SIG(__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkAddr,16,0);
    VL_SIGW(__Vtemp4,127,0,4);
    // Body
    __Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0 = 0U;
    __Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady 
	= vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkReady;
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDirty;
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[3U] 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[4U] 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[4U];
    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkAddr;
    // ALWAYS at BsrDcTileB.v:237
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK2 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxB 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB;
    // ALWAYS at BsrMemTileB.v:347
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileA
	[(0x7ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA) 
		    >> 1U))];
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileB
	[(0x7ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA) 
		    >> 1U))];
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileC
	[(0x7ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB) 
		    >> 1U))];
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileD
	[(0x7ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB) 
		    >> 1U))];
    // ALWAYS at BsrMemTileB.v:347
    if (vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileSrSt) {
	__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileA__v0 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U];
	__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0 = 1U;
	__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileA__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA) 
			 >> 1U));
	__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileB__v0 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U];
	__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileB__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA) 
			 >> 1U));
	__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileC__v0 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U];
	__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileC__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB) 
			 >> 1U));
	__Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileD__v0 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U];
	__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileD__v0 
	    = (0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB) 
			 >> 1U));
    }
    // ALWAYS at BsrMemTileB.v:347
    if (vlTOPp->BsrTstMem__DOT__memTile__DOT__mmioBypass) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOutData 
	    = vlTOPp->BsrTstMem__DOT__memRegInData[0U];
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioAddr 
	    = vlTOPp->BsrTstMem__DOT__memRegInAddr;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm 
	    = vlTOPp->BsrTstMem__DOT__memRegInOpm;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioOK 
	    = vlTOPp->mmioOK;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioInData 
	    = vlTOPp->mmioInData;
    } else {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOutData = 0U;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioAddr = 0U;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm = 0U;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioOK = 0U;
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioInData = 0U;
    }
    // ALWAYS at BsrIcTileB.v:80
    if (vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkMiss) {
	if ((1U == (IData)(vlTOPp->BsrTstMem__DOT__icMemPcOK))) {
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcAddr = 0U;
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm = 0U;
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkData[0U] 
		= vlTOPp->BsrTstMem__DOT__icMemPcData[0U];
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkData[1U] 
		= vlTOPp->BsrTstMem__DOT__icMemPcData[1U];
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkData[2U] 
		= vlTOPp->BsrTstMem__DOT__icMemPcData[2U];
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkData[3U] 
		= vlTOPp->BsrTstMem__DOT__icMemPcData[3U];
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr 
		= (0x1ffffU & (vlTOPp->BsrTstMem__DOT__icRegInPc 
			       >> 3U));
	    __Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady = 1U;
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkData[4U] = 0U;
	} else {
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcAddr 
		= (0xffff8U & vlTOPp->BsrTstMem__DOT__icRegInPc);
	    vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm = 0xfU;
	    __Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady = 0U;
	}
    } else {
	__Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady 
	    = vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkReady;
	vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcAddr = 0U;
	vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm = 0U;
    }
    // ALWAYS at BsrDcTileB.v:237
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore;
    if (VL_UNLIKELY(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass)) {
	VL_WRITEF("DcTile Bypass %x %x %x\n",32,vlTOPp->BsrTstMem__DOT__dcRegInAddr,
		  5,(IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm),
		  32,vlTOPp->BsrTstMem__DOT__dcRegInData);
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr 
	    = (0xfffffU & vlTOPp->BsrTstMem__DOT__dcRegInAddr);
	VL_EXTEND_WI(128,32, __Vtemp4, vlTOPp->BsrTstMem__DOT__dcRegInData);
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] 
	    = __Vtemp4[0U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] 
	    = __Vtemp4[1U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] 
	    = __Vtemp4[2U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] 
	    = __Vtemp4[3U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm 
	    = vlTOPp->BsrTstMem__DOT__dcRegInOpm;
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB = 0U;
    } else {
	if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss) {
	    if ((1U == (IData)(vlTOPp->BsrTstMem__DOT__dcMemPcOK))) {
		if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDirty) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr 
			   << 3U);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = 0U;
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady = 0U;
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = 0U;
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
			= vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U];
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			= vlTOPp->BsrTstMem__DOT__dcMemPcDataI[1U];
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			= vlTOPp->BsrTstMem__DOT__dcMemPcDataI[2U];
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[3U] 
			= vlTOPp->BsrTstMem__DOT__dcMemPcDataI[3U];
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr;
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady = 1U;
		    __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[4U] = 0xffffffffU;
		}
	    } else {
		if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDirty) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
			   << 3U);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = 0x17U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady = 0U;
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr 
			= (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr 
			   << 3U);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = 0xfU;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady = 0U;
		}
	    }
	} else {
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr = 0U;
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U] = 0U;
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U] = 0U;
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U] = 0U;
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U] = 0U;
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = 0U;
	    if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore) {
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U];
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U];
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U];
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[3U] 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[3U];
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[4U] 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[4U];
		__Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty = 1U;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady = 1U;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB = 0U;
	    }
	}
    }
    // ALWAYSPOST at BsrMemTileB.v:396
    if (__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileA[__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileA__v0] 
	    = __Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileA__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:397
    if (__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileB[__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileB__v0] 
	    = __Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileB__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:398
    if (__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileC[__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileC__v0] 
	    = __Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileC__v0;
    }
    // ALWAYSPOST at BsrMemTileB.v:399
    if (__Vdlyvset__BsrTstMem__DOT__memTile__DOT__sramTileA__v0) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileD[__Vdlyvdim0__BsrTstMem__DOT__memTile__DOT__sramTileD__v0] 
	    = __Vdlyvval__BsrTstMem__DOT__memTile__DOT__sramTileD__v0;
    }
    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkReady 
	= __Vdly__BsrTstMem__DOT__icTile__DOT__icBlkReady;
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDirty 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkDirty;
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U] 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[4U] 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkData[4U];
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
	= __Vdly__BsrTstMem__DOT__dcTile__DOT__icBlkAddr;
    vlTOPp->regOutOK = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK2;
    vlTOPp->mmioOutData = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOutData;
    vlTOPp->mmioAddr = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioAddr;
    vlTOPp->mmioOpm = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm;
    // ALWAYS at BsrIcTileB.v:48
    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkMiss 
	= (1U & (((vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr 
		   != (0x1ffffU & (vlTOPp->BsrTstMem__DOT__icRegInPc 
				   >> 3U))) & (((IData)(1U) 
						+ vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr) 
					       != (0x1ffffU 
						   & (vlTOPp->BsrTstMem__DOT__icRegInPc 
						      >> 3U)))) 
		 | (~ (IData)(vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkReady))));
    // ALWAYS at BsrIcTileB.v:48
    vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkHi = 
	((1U & vlTOPp->BsrTstMem__DOT__icTile__DOT__icBlkAddr) 
	 != (1U & (vlTOPp->BsrTstMem__DOT__icRegInPc 
		   >> 3U)));
}

void VBsrTstMem::_initial__TOP__3(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_initial__TOP__3\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp7, vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileA
		  ,0,~0);
    __Vtemp8[0U] = 0x2e747874U;
    __Vtemp8[1U] = 0x6d5f3162U;
    __Vtemp8[2U] = 0x6f74726fU;
    __Vtemp8[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp8, vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileB
		  ,0,~0);
    __Vtemp9[0U] = 0x2e747874U;
    __Vtemp9[1U] = 0x6d5f3163U;
    __Vtemp9[2U] = 0x6f74726fU;
    __Vtemp9[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp9, vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileC
		  ,0,~0);
    __Vtemp10[0U] = 0x2e747874U;
    __Vtemp10[1U] = 0x6d5f3164U;
    __Vtemp10[2U] = 0x6f74726fU;
    __Vtemp10[3U] = 0x626fU;
    VL_READMEM_W (true,32,2048, 0,4, __Vtemp10, vlTOPp->BsrTstMem__DOT__memTile__DOT__romTileD
		  ,0,~0);
}

VL_INLINE_OPT void VBsrTstMem::_combo__TOP__4(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_combo__TOP__4\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp13,127,0,4);
    // Body
    // ALWAYS at BsrTstMem.v:148
    vlTOPp->BsrTstMem__DOT__dcRegInAddr = vlTOPp->regInAddr;
    vlTOPp->BsrTstMem__DOT__dcRegInOpm = vlTOPp->regInOpm;
    vlTOPp->BsrTstMem__DOT__dcRegInData = vlTOPp->regInData;
    // ALWAYS at BsrTstMem.v:148
    vlTOPp->BsrTstMem__DOT__icMemPcOK = 2U;
    vlTOPp->BsrTstMem__DOT__dcMemPcOK = 2U;
    vlTOPp->BsrTstMem__DOT__icMemPcData[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrTstMem__DOT__icMemPcData[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->BsrTstMem__DOT__memRegInAddr = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInData[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memRegInOpm = 0U;
    if ((0U != (IData)(vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm))) {
	vlTOPp->BsrTstMem__DOT__icMemPcData[0U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[1U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[2U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
	vlTOPp->BsrTstMem__DOT__icMemPcData[3U] = vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
	vlTOPp->BsrTstMem__DOT__icMemPcOK = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK;
	vlTOPp->BsrTstMem__DOT__memRegInAddr = vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcAddr;
	vlTOPp->BsrTstMem__DOT__memRegInData[0U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[1U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[2U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInData[3U] = 0U;
	vlTOPp->BsrTstMem__DOT__memRegInOpm = vlTOPp->BsrTstMem__DOT__icTile__DOT__tMemPcOpm;
    } else {
	if ((0U != (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm))) {
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[1U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[2U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcDataI[3U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
	    vlTOPp->BsrTstMem__DOT__dcMemPcOK = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK;
	    vlTOPp->BsrTstMem__DOT__memRegInAddr = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcAddr;
	    vlTOPp->BsrTstMem__DOT__memRegInData[0U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[0U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[1U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[1U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[2U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[2U];
	    vlTOPp->BsrTstMem__DOT__memRegInData[3U] 
		= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcDataO[3U];
	    vlTOPp->BsrTstMem__DOT__memRegInOpm = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tMemPcOpm;
	} else {
	    vlTOPp->BsrTstMem__DOT__icMemPcOK = 0U;
	    vlTOPp->BsrTstMem__DOT__dcMemPcOK = 0U;
	}
    }
    // ALWAYS at BsrDcTileB.v:82
    if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
		      >> 3U)))) {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss 
	    = (1U & ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkAddr 
		      != (0x1ffffU & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
				      >> 3U))) | (~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady))));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr 
	    = (0x1ffffU & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
			   >> 3U));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass 
	    = (7U == (7U & (vlTOPp->BsrTstMem__DOT__dcRegInAddr 
			    >> 0xdU)));
    } else {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss = 0U;
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass = 0U;
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icReqAddr = 0U;
    }
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore 
	= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB;
    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 0U;
    if (((((~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkMiss)) 
	   & (~ (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass))) 
	  & (IData)(vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkReady)) 
	 & (0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			 >> 3U))))) {
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData 
	    = ((4U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
	        ? ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		    ? ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		        ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			    << 8U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x10U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		     ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U] 
			 << 0x18U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
				      >> 8U)) : vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U]))
	        : ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		    ? ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		        ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			    << 8U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x18U)) : 
		       ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x10U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 0x10U))) : 
		   ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)
		     ? ((vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U] 
			 << 0x18U) | (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U] 
				      >> 8U)) : vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U])));
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore = 0U;
	if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			  >> 3U)))) {
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 1U;
	}
	if ((4U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
	    if ((2U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
		    = vlTOPp->BsrTstMem__DOT__dcRegInData;
	    } else {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= (0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= (0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData);
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		}
	    }
	} else {
	    if ((2U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrTstMem__DOT__dcRegInData;
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData;
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= vlTOPp->BsrTstMem__DOT__dcRegInData;
		}
	    } else {
		if ((1U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= ((((0x8000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffU : 0U) << 0x10U) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
			= ((((0x80U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData)
			      ? 0xffffffU : 0U) << 8U) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpData) 
			   | (0xffU & vlTOPp->BsrTstMem__DOT__dcRegInData));
		}
	    }
	}
	if ((0x10U & (IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm))) {
	    vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore = 1U;
	}
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[0U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[1U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[2U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[3U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[3U];
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[4U] 
	    = vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkData[4U];
	if ((4U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
	    if ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xff000000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xff000000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffff0000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffff00U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U] 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[2U]) 
			   | (0xffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	} else {
	    if ((2U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xff000000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x18U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xff000000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					   >> 8U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffff0000U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 0x10U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffff0000U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					 >> 0x10U)));
		}
	    } else {
		if ((1U & vlTOPp->BsrTstMem__DOT__dcRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= ((0xffU & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U]) 
			   | (0xffffff00U & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
					     << 8U)));
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U] 
			= ((0xffffff00U & vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[1U]) 
			   | (0xffU & (vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt 
				       >> 0x18U)));
		} else {
		    vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkDataSt[0U] 
			= vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt;
		}
	    }
	}
	vlTOPp->BsrTstMem__DOT__dcTile__DOT__tIcBlkStore 
	    = (1U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
		     >> 4U));
    } else {
	if (vlTOPp->BsrTstMem__DOT__dcTile__DOT__icBlkBypass) {
	    if ((1U == (IData)(vlTOPp->BsrTstMem__DOT__dcMemPcOK))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 1U;
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData 
		    = vlTOPp->BsrTstMem__DOT__dcMemPcDataI[0U];
	    } else {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	} else {
	    if ((0U != (3U & ((IData)(vlTOPp->BsrTstMem__DOT__dcRegInOpm) 
			      >> 3U)))) {
		vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutOK = 2U;
	    }
	}
    }
    vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam 
	= ((0xc000U <= vlTOPp->BsrTstMem__DOT__memRegInAddr) 
	   & (0xe000U > vlTOPp->BsrTstMem__DOT__memRegInAddr));
    vlTOPp->regOutData = vlTOPp->BsrTstMem__DOT__dcTile__DOT__tRegOutData;
    // ALWAYS at BsrMemTileB.v:156
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] = 0U;
    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB 
	    = (0xfffU & (vlTOPp->BsrTstMem__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & ((IData)(1U) + (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB)));
    } else {
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA 
	    = (0xfffU & (vlTOPp->BsrTstMem__DOT__memRegInAddr 
			 >> 3U));
	vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxB 
	    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA;
    }
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileSrSt = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK = 0U;
    vlTOPp->BsrTstMem__DOT__memTile__DOT__mmioBypass = 0U;
    if ((1U & (((IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm) 
		>> 3U) | ((IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm) 
			  >> 4U)))) {
	if (((0x8000U >= vlTOPp->BsrTstMem__DOT__memRegInAddr) 
	     & (~ (vlTOPp->BsrTstMem__DOT__memRegInAddr 
		   >> 0xeU)))) {
	    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
	    } else {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U]))) 
			       >> 0x20U));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
		    = (IData)((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				<< 0x20U) | (QData)((IData)(
							    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))));
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
		    = (IData)(((((QData)((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U])) 
				 << 0x20U) | (QData)((IData)(
							     vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U]))) 
			       >> 0x20U));
	    }
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[0U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[1U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[2U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
		= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRomTile[3U];
	    vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		= (((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
		    == (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA))
		    ? 1U : 2U);
	    if ((2U == (3U & (IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm)))) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		if ((4U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U]))) 
				   >> 0x20U));
		}
	    } else {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
	    }
	} else {
	    if (vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam) {
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileA
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		    = (((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
			== (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegTileIxA))
		        ? 1U : 2U);
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileB
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxA) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileC
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__sramTileD
		    [(0x1ffU & ((IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tAccTileIxB) 
				>> 1U))];
		if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U]))) 
				   >> 0x20U));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))));
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U]))) 
				   >> 0x20U));
		}
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tSRamTile[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tMemTile[3U];
		if ((0x10U & (IData)(vlTOPp->BsrTstMem__DOT__memRegInOpm))) {
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[0U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[1U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[2U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U] 
			= vlTOPp->BsrTstMem__DOT__memRegInData[3U];
		    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileSrSt 
			= vlTOPp->BsrTstMem__DOT__memTile__DOT__addrIsSRam;
		    if ((8U & vlTOPp->BsrTstMem__DOT__memRegInAddr)) {
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
		    } else {
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[0U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[1U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[1U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[0U]))) 
				       >> 0x20U));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[2U] 
			    = (IData)((((QData)((IData)(
							vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					<< 0x20U) | (QData)((IData)(
								    vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))));
			vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTile[3U] 
			    = (IData)(((((QData)((IData)(
							 vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[3U])) 
					 << 0x20U) 
					| (QData)((IData)(
							  vlTOPp->BsrTstMem__DOT__memTile__DOT__tNextTileA[2U]))) 
				       >> 0x20U));
		    }
		}
	    } else {
		VL_EXTEND_WI(128,32, __Vtemp13, vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioInData);
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
		    = __Vtemp13[0U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
		    = __Vtemp13[1U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
		    = __Vtemp13[2U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
		    = __Vtemp13[3U];
		vlTOPp->BsrTstMem__DOT__memTile__DOT__mmioBypass = 1U;
		vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK 
		    = vlTOPp->BsrTstMem__DOT__memTile__DOT__tmpMmioOK;
		if (VL_UNLIKELY((0U != (IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm)))) {
		    VL_WRITEF("DcTile2 Bypass A=%x O=%x DO=%x DI=%x\n",
			      20,vlTOPp->BsrTstMem__DOT__memRegInAddr,
			      5,(IData)(vlTOPp->BsrTstMem__DOT__memTile__DOT__tMmioOpm),
			      32,vlTOPp->BsrTstMem__DOT__memRegInData[0U],
			      32,vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U]);
		}
	    }
	}
    }
}

void VBsrTstMem::_eval(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_eval\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void VBsrTstMem::_eval_initial(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_eval_initial\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__3(vlSymsp);
}

void VBsrTstMem::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::final\n"); );
    // Variables
    VBsrTstMem__Syms* __restrict vlSymsp = this->__VlSymsp;
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VBsrTstMem::_eval_settle(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_eval_settle\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__1(vlSymsp);
}

VL_INLINE_OPT QData VBsrTstMem::_change_request(VBsrTstMem__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_change_request\n"); );
    VBsrTstMem* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[0]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[1]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[2]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[3])
	 | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tRegOutOK));
    VL_DEBUG_IF( if(__req && ((vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[0]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[1]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[2]) | (vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3] ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[3]))) VL_DBG_MSGF("        CHANGE: BsrMemTileB.v:101: BsrTstMem.memTile.tOutData\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK ^ vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tRegOutOK))) VL_DBG_MSGF("        CHANGE: BsrMemTileB.v:112: BsrTstMem.memTile.tRegOutOK\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[0U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[0U];
    vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[1U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[1U];
    vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[2U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[2U];
    vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData[3U] 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tOutData[3U];
    vlTOPp->__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tRegOutOK 
	= vlTOPp->BsrTstMem__DOT__memTile__DOT__tRegOutOK;
    return __req;
}

#ifdef VL_DEBUG
void VBsrTstMem::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clock & 0xfeU))) {
	Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((reset & 0xfeU))) {
	Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((regInOpm & 0xe0U))) {
	Verilated::overWidthError("regInOpm");}
    if (VL_UNLIKELY((memOK & 0xfcU))) {
	Verilated::overWidthError("memOK");}
    if (VL_UNLIKELY((mmioOK & 0xfcU))) {
	Verilated::overWidthError("mmioOK");}
}
#endif // VL_DEBUG

void VBsrTstMem::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VBsrTstMem::_ctor_var_reset\n"); );
    // Body
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    regInAddr = VL_RAND_RESET_I(32);
    regInOpm = VL_RAND_RESET_I(5);
    regInData = VL_RAND_RESET_I(32);
    regOutData = VL_RAND_RESET_I(32);
    regOutOK = VL_RAND_RESET_I(2);
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
    BsrTstMem__DOT__memRegInAddr = VL_RAND_RESET_I(20);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memRegInData);
    BsrTstMem__DOT__memRegInOpm = VL_RAND_RESET_I(5);
    BsrTstMem__DOT__dcRegInAddr = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__dcRegInOpm = VL_RAND_RESET_I(5);
    BsrTstMem__DOT__dcRegInData = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__dcMemPcDataI);
    BsrTstMem__DOT__dcMemPcOK = VL_RAND_RESET_I(2);
    BsrTstMem__DOT__icRegInPc = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__icMemPcData);
    BsrTstMem__DOT__icMemPcOK = VL_RAND_RESET_I(2);
    BsrTstMem__DOT__memTile__DOT__tMmioOutData = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__memTile__DOT__tMmioAddr = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__memTile__DOT__tMmioOpm = VL_RAND_RESET_I(5);
    BsrTstMem__DOT__memTile__DOT__tmpMmioInData = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__memTile__DOT__tmpMmioOK = VL_RAND_RESET_I(2);
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__romTileA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__romTileB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__romTileC[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<2048; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__romTileD[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__sramTileA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__sramTileB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__sramTileC[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<512; ++__Vi0) {
	    BsrTstMem__DOT__memTile__DOT__sramTileD[__Vi0] = VL_RAND_RESET_I(32);
    }}
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tRomTile);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tSRamTile);
    BsrTstMem__DOT__memTile__DOT__tAccTileIxA = VL_RAND_RESET_I(12);
    BsrTstMem__DOT__memTile__DOT__tAccTileIxB = VL_RAND_RESET_I(12);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tMemTile);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tOutData);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tNextTile);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__memTile__DOT__tNextTileA);
    BsrTstMem__DOT__memTile__DOT__tRegTileIxA = VL_RAND_RESET_I(12);
    BsrTstMem__DOT__memTile__DOT__tRegTileIxB = VL_RAND_RESET_I(12);
    BsrTstMem__DOT__memTile__DOT__tNextTileSrSt = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__memTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    BsrTstMem__DOT__memTile__DOT__addrIsSRam = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__memTile__DOT__mmioBypass = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__dcTile__DOT__tRegOutData = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__dcTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    BsrTstMem__DOT__dcTile__DOT__tRegOutOK2 = VL_RAND_RESET_I(2);
    BsrTstMem__DOT__dcTile__DOT__tMemPcAddr = VL_RAND_RESET_I(20);
    VL_RAND_RESET_W(128,BsrTstMem__DOT__dcTile__DOT__tMemPcDataO);
    BsrTstMem__DOT__dcTile__DOT__tMemPcOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(160,BsrTstMem__DOT__dcTile__DOT__icBlkData);
    BsrTstMem__DOT__dcTile__DOT__icBlkAddr = VL_RAND_RESET_I(17);
    BsrTstMem__DOT__dcTile__DOT__icBlkDirty = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__dcTile__DOT__icBlkReady = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(160,BsrTstMem__DOT__dcTile__DOT__icBlkDataSt);
    BsrTstMem__DOT__dcTile__DOT__icBlkMiss = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__dcTile__DOT__icBlkBypass = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__dcTile__DOT__icReqAddr = VL_RAND_RESET_I(17);
    BsrTstMem__DOT__dcTile__DOT__tRegTmpData = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt = VL_RAND_RESET_I(32);
    BsrTstMem__DOT__dcTile__DOT__tIcBlkStore = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__icTile__DOT__tMemPcAddr = VL_RAND_RESET_I(20);
    BsrTstMem__DOT__icTile__DOT__tMemPcOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(160,BsrTstMem__DOT__icTile__DOT__icBlkData);
    BsrTstMem__DOT__icTile__DOT__icBlkAddr = VL_RAND_RESET_I(17);
    BsrTstMem__DOT__icTile__DOT__icBlkReady = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__icTile__DOT__icBlkMiss = VL_RAND_RESET_I(1);
    BsrTstMem__DOT__icTile__DOT__icBlkHi = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData);
    __Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tRegOutOK = VL_RAND_RESET_I(2);
}

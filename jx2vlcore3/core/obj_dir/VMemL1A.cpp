// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VMemL1A.h for the primary calling header

#include "VMemL1A.h"           // For This
#include "VMemL1A__Syms.h"


//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(VMemL1A) {
    VMemL1A__Syms* __restrict vlSymsp = __VlSymsp = new VMemL1A__Syms(this, name());
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VMemL1A::__Vconfigure(VMemL1A__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

VMemL1A::~VMemL1A() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void VMemL1A::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VMemL1A::eval\n"); );
    VMemL1A__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void VMemL1A::_eval_initial_loop(VMemL1A__Syms* __restrict vlSymsp) {
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

VL_INLINE_OPT void VMemL1A::_sequent__TOP__1(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_sequent__TOP__1\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemA__v0,5,0);
    VL_SIG8(__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0,0,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemB__v0,5,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrA__v0,5,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrB__v0,5,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tStBlkFlag,3,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tStBlkIx,5,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tReqIxA,5,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tReqIxB,5,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tReqIsMmio,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tInOpm,4,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0,5,0);
    VL_SIG8(__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemA__v0,0,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0,5,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0,5,0);
    VL_SIG8(__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemB__v0,0,0);
    VL_SIG8(__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0,5,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm,4,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMmioLatch,0,0);
    VL_SIG8(__Vdly__MemL1A__DOT__memDc__DOT__tMmioDone,0,0);
    VL_SIG(__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrA__v0,31,0);
    VL_SIG(__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrB__v0,31,0);
    VL_SIGW(__Vdly__MemL1A__DOT__memIc__DOT__tStBlkData,127,0,4);
    VL_SIG(__Vdly__MemL1A__DOT__memIc__DOT__tStBlkAddr,27,0);
    VL_SIG(__Vdly__MemL1A__DOT__memDc__DOT__tInAddr,31,0);
    VL_SIG(__Vdly__MemL1A__DOT__memDc__DOT__tReqAddrA,27,0);
    VL_SIG(__Vdly__MemL1A__DOT__memDc__DOT__tReqAddrB,27,0);
    VL_SIGW(__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0,127,0,4);
    VL_SIG(__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0,31,0);
    VL_SIGW(__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0,127,0,4);
    VL_SIG(__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0,31,0);
    VL_SIG(__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr,31,0);
    VL_SIGW(__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut,127,0,4);
    VL_SIGW(__Vtemp1,127,0,4);
    VL_SIG64(__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemA__v0,63,0);
    VL_SIG64(__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemB__v0,63,0);
    VL_SIG64(__Vdly__MemL1A__DOT__memDc__DOT__tDataIn,63,0);
    // Body
    __Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk = vlTOPp->MemL1A__DOT__memIc__DOT__tDoStBlk;
    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchA = vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA;
    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnA = vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnA;
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[0U] 
	= vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[0U];
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[1U] 
	= vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[1U];
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[2U] 
	= vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[2U];
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[3U] 
	= vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[3U];
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkAddr = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkAddr;
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkFlag = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkFlag;
    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkIx = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx;
    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchB = vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB;
    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnB = vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnB;
    __Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0 = 0U;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnA;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbA;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdA;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnB;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbB;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdB;
    __Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemA__v0 = 0U;
    __Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemB__v0 = 0U;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
    __Vdly__MemL1A__DOT__memDc__DOT__tInAddr = vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr;
    __Vdly__MemL1A__DOT__memDc__DOT__tReqAddrA = vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA;
    __Vdly__MemL1A__DOT__memDc__DOT__tReqAddrB = vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB;
    __Vdly__MemL1A__DOT__memDc__DOT__tReqIxA = vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA;
    __Vdly__MemL1A__DOT__memDc__DOT__tReqIxB = vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB;
    __Vdly__MemL1A__DOT__memDc__DOT__tReqIsMmio = vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio;
    __Vdly__MemL1A__DOT__memDc__DOT__tInOpm = vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm;
    __Vdly__MemL1A__DOT__memDc__DOT__tDataIn = vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA;
    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB;
    __Vdly__MemL1A__DOT__memDc__DOT__tMmioLatch = vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch;
    __Vdly__MemL1A__DOT__memDc__DOT__tMmioDone = vlTOPp->MemL1A__DOT__memDc__DOT__tMmioDone;
    // ALWAYS at MemIcA.v:182
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagA = (0xfU 
						  & (vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrA
						     [vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA] 
						     >> 0x1cU));
    // ALWAYS at MemIcA.v:182
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrA = (0xfffffffU 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrA
						  [vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA]);
    // ALWAYS at MemIcA.v:183
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagB = (0xfU 
						  & (vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrB
						     [vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB] 
						     >> 0x1cU));
    // ALWAYS at MemIcA.v:183
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrB = (0xfffffffU 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrB
						  [vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB]);
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA = vlTOPp->MemL1A__DOT__memIc__DOT__icCaMemA
	[vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA];
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB = vlTOPp->MemL1A__DOT__memIc__DOT__icCaMemB
	[vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB];
    // ALWAYS at MemL1A.v:165
    vlTOPp->MemL1A__DOT__tLatchIc = vlTOPp->MemL1A__DOT__tNxtLatchIc;
    // ALWAYS at MemL1A.v:165
    vlTOPp->MemL1A__DOT__tLatchDc = vlTOPp->MemL1A__DOT__tNxtLatchDc;
    // ALWAYS at MemDcA.v:334
    vlTOPp->MemL1A__DOT__memDc__DOT__tLstHold = vlTOPp->MemL1A__DOT__memDc__DOT__tHold;
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr = vlTOPp->icInPcAddr;
    if (vlTOPp->MemL1A__DOT__memIc__DOT__tDoStBlk) {
	__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemA__v0 
	    = (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[1U])) 
		<< 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[0U])));
	__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0 = 1U;
	__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemA__v0 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx;
	__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemB__v0 
	    = (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[3U])) 
		<< 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[2U])));
	__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemB__v0 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx;
	__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrA__v0 
	    = (((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkFlag) 
		<< 0x1cU) | vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkAddr);
	__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrA__v0 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx;
	__Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrB__v0 
	    = (((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkFlag) 
		<< 0x1cU) | vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkAddr);
	__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrB__v0 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx;
	__Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk = 0U;
    }
    if ((((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissA) 
	  | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA)) 
	 & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB)))) {
	if (vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnA) {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
	    if ((0U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK))) {
		__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchA = 0U;
		__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnA = 0U;
	    }
	} else {
	    if (((1U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK)) 
		 & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA))) {
		vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
		__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnA = 1U;
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[0U] 
		    = vlTOPp->MemL1A__DOT__ifMemData[0U];
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[1U] 
		    = vlTOPp->MemL1A__DOT__ifMemData[1U];
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[2U] 
		    = vlTOPp->MemL1A__DOT__ifMemData[2U];
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[3U] 
		    = vlTOPp->MemL1A__DOT__ifMemData[3U];
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkAddr 
		    = vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA;
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkFlag 
		    = (3U & (~ vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA));
		__Vdly__MemL1A__DOT__memIc__DOT__tStBlkIx 
		    = vlTOPp->MemL1A__DOT__memIc__DOT__tReqIxA;
		__Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk = 1U;
	    } else {
		if (((2U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK)) 
		     & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA))) {
		    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0xfU;
		    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr 
			= (vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA 
			   << 4U);
		} else {
		    if ((0U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK))) {
			__Vdly__MemL1A__DOT__memIc__DOT__tMemLatchA = 1U;
			vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0xfU;
			vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr 
			    = (vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA 
			       << 4U);
		    } else {
			vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
		    }
		}
	    }
	}
    } else {
	if ((((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissB) 
	      | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB)) 
	     & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA)))) {
	    if (vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnB) {
		vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
		if ((0U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK))) {
		    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchB = 0U;
		    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnB = 0U;
		}
	    } else {
		if (((1U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK)) 
		     & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB))) {
		    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
		    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnB = 1U;
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[0U] 
			= vlTOPp->MemL1A__DOT__ifMemData[0U];
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[1U] 
			= vlTOPp->MemL1A__DOT__ifMemData[1U];
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[2U] 
			= vlTOPp->MemL1A__DOT__ifMemData[2U];
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[3U] 
			= vlTOPp->MemL1A__DOT__ifMemData[3U];
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkAddr 
			= vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB;
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkFlag 
			= (3U & (~ vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB));
		    __Vdly__MemL1A__DOT__memIc__DOT__tStBlkIx 
			= vlTOPp->MemL1A__DOT__memIc__DOT__tReqIxB;
		    __Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk = 1U;
		} else {
		    if (((2U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK)) 
			 & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB))) {
			vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0xfU;
			vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr 
			    = (vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB 
			       << 4U);
		    } else {
			if ((0U == (IData)(vlTOPp->MemL1A__DOT__ifMemOK))) {
			    __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchB = 1U;
			    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0xfU;
			    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr 
				= (vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB 
				   << 4U);
			} else {
			    vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm = 0U;
			}
		    }
		}
	    }
	} else {
	    if (VL_UNLIKELY(((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissA) 
			     | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissB)))) {
		VL_WRITEF("L1I$ Sticky Miss, %1# %1#\n",
			  1,vlTOPp->MemL1A__DOT__memIc__DOT__tMissA,
			  1,(IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissB));
	    }
	    if (VL_UNLIKELY(((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA) 
			     | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB)))) {
		VL_WRITEF("L1I$ Sticky Latch, %1# %1#\n",
			  1,vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA,
			  1,(IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB));
	    }
	}
    }
    // ALWAYS at MemDcA.v:334
    if ((1U & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tHold)))) {
	__Vdly__MemL1A__DOT__memDc__DOT__tInAddr = vlTOPp->dcInAddr;
	__Vdly__MemL1A__DOT__memDc__DOT__tReqAddrA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA;
	__Vdly__MemL1A__DOT__memDc__DOT__tReqAddrB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB;
	__Vdly__MemL1A__DOT__memDc__DOT__tReqIxA = vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxA;
	__Vdly__MemL1A__DOT__memDc__DOT__tReqIxB = vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxB;
	__Vdly__MemL1A__DOT__memDc__DOT__tReqIsMmio 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIsMmio;
	__Vdly__MemL1A__DOT__memDc__DOT__tInOpm = vlTOPp->dcInOpm;
	__Vdly__MemL1A__DOT__memDc__DOT__tDataIn = vlTOPp->dcInVal;
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA)) {
	if (VL_UNLIKELY((1U & vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA))) {
	    VL_WRITEF("L1D$, DoStBlkA: Even/Odd Mismatch\n");
	}
	VL_WRITEF("L1D$, DoStBlkA(%x), Data=%x\n",6,
		  vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA,
		  128,vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA);
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U];
	__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemA__v0 = 1U;
	__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA;
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0 
	    = (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagA) 
		<< 0x1cU) | vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA);
	__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA = 0U;
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB)) {
	if (VL_UNLIKELY((1U & vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA))) {
	    VL_WRITEF("L1D$, DoStBlkA: Even/Odd Mismatch\n");
	}
	VL_WRITEF("L1D$, DoStBlkB(%x), Data=%x\n",6,
		  vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB,
		  128,vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB);
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U];
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U];
	__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemB__v0 = 1U;
	__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB;
	__Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0 
	    = (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagB) 
		<< 0x1cU) | vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB);
	__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB = 0U;
    }
    if (VL_UNLIKELY((((((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMiss) 
			& (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA)) 
		       | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA)) 
		      & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB))) 
		     & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch))))) {
	VL_WRITEF("L1D$ MissA, Miss=%1# Latch=%1# OK=%1# Dn=%1# Wb=%1#\n",
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMissA,
		  1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA),
		  2,vlTOPp->MemL1A__DOT__dfMemOK,1,
		  (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnA),
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbA);
	if (vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnA) {
	    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
	    if (VL_UNLIKELY((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)))) {
		VL_WRITEF("L1D$: MissA Done\n");
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA = 0U;
	    }
	} else {
	    if (((1U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
		 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA))) {
		__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		if (VL_UNLIKELY((1U & ((~ ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA) 
					   >> 2U)) 
				       | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdA))))) {
		    VL_WRITEF("L1D$: MissA Dn memDataIn=%x\n",
			      128,vlTOPp->MemL1A__DOT__dfMemDataIn);
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[0U] 
			= vlTOPp->MemL1A__DOT__dfMemDataIn[0U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[1U] 
			= vlTOPp->MemL1A__DOT__dfMemDataIn[1U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[2U] 
			= vlTOPp->MemL1A__DOT__dfMemDataIn[2U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[3U] 
			= vlTOPp->MemL1A__DOT__dfMemDataIn[3U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrA 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxA 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA = 1U;
		} else {
		    if ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA))) {
			__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA = 1U;
		    }
		}
	    } else {
		if (((2U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
		     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA))) {
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
		} else {
		    if ((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK))) {
			if (VL_UNLIKELY((1U & (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA) 
						>> 2U) 
					       & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbA)))))) {
			    VL_WRITEF("L1D$ MissA ReadySt\n");
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
				= (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA 
				   << 4U);
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
				= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U];
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
				= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U];
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
				= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U];
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
				= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U];
			    VL_WRITEF("L1D$ MissA, Write=%x\n",
				      128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA);
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA = 1U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0x17U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA = 0U;
			} else {
			    VL_WRITEF("L1D$ MissA ReadyLd\n");
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
				= (vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA 
				   << 4U);
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] = 0U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] = 0U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] = 0U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] = 0U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA = 1U;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0xfU;
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA 
				= vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbA;
			}
		    } else {
			__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		    }
		}
	    }
	}
    } else {
	if (VL_UNLIKELY((((((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMiss) 
			    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB)) 
			   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB)) 
			  & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA))) 
			 & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch))))) {
	    VL_WRITEF("L1D$ MissB, Miss=%1# Latch=%1#\n",
		      1,vlTOPp->MemL1A__DOT__memDc__DOT__tMissB,
		      1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB));
	    if (vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnB) {
		__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		if (VL_UNLIKELY((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)))) {
		    VL_WRITEF("L1D$: MissB Done\n");
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB = 0U;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB = 0U;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB = 0U;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB = 0U;
		}
	    } else {
		if (((1U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
		     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB))) {
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		    if (VL_UNLIKELY((1U & ((~ ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB) 
					       >> 2U)) 
					   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdB))))) {
			VL_WRITEF("L1D$: MissB Dn memDataIn=%x\n",
				  128,vlTOPp->MemL1A__DOT__dfMemDataIn);
			__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB = 1U;
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[0U] 
			    = vlTOPp->MemL1A__DOT__dfMemDataIn[0U];
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[1U] 
			    = vlTOPp->MemL1A__DOT__dfMemDataIn[1U];
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[2U] 
			    = vlTOPp->MemL1A__DOT__dfMemDataIn[2U];
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[3U] 
			    = vlTOPp->MemL1A__DOT__dfMemDataIn[3U];
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrB 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB;
			vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxB 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB;
			vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB = 1U;
		    } else {
			if ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB))) {
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB = 1U;
			}
		    }
		} else {
		    if (((2U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
			 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB))) {
			__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm;
			__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr;
			__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
			__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
			__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
			__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
		    } else {
			if ((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK))) {
			    if (VL_UNLIKELY((1U & (
						   ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB) 
						    >> 2U) 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbB)))))) {
				VL_WRITEF("L1D$ MissB ReadySt\n");
				__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
				    = (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB 
				       << 4U);
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
				    = vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U];
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
				    = vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U];
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
				    = vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U];
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
				    = vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U];
				VL_WRITEF("L1D$ MissB, Write=%x\n",
					  128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB);
				__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB = 1U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0x17U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB = 0U;
			    } else {
				VL_WRITEF("L1D$ MissB ReadyLd\n");
				__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
				    = (vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB 
				       << 4U);
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] = 0U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] = 0U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] = 0U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] = 0U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB = 1U;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0xfU;
				__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB 
				    = vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbB;
			    }
			} else {
			    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
			}
		    }
		}
	    }
	} else {
	    if (((~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)) 
		 & (0U != (3U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm) 
				 >> 3U))))) {
		if (VL_UNLIKELY(((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA) 
				 | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB)))) {
		    VL_WRITEF("L1D$ Sticky Miss, %1# %1#\n",
			      1,vlTOPp->MemL1A__DOT__memDc__DOT__tMissA,
			      1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB));
		}
		if (VL_UNLIKELY(((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA) 
				 | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB)))) {
		    VL_WRITEF("L1D$ Sticky Latch, %1# %1#\n",
			      1,vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA,
			      1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB));
		}
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB = 0U;
	    }
	}
    }
    if (((((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio) 
	   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA))) 
	  & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB))) 
	 | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch))) {
	VL_EXTEND_WQ(128,64, __Vtemp1, vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn);
	__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
	    = __Vtemp1[0U];
	__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
	    = __Vtemp1[1U];
	__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
	    = __Vtemp1[2U];
	__Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
	    = __Vtemp1[3U];
	__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm;
	__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr;
	if (vlTOPp->MemL1A__DOT__memDc__DOT__tMmioDone) {
	    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
	    if ((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK))) {
		__Vdly__MemL1A__DOT__memDc__DOT__tMmioLatch = 0U;
	    }
	} else {
	    if (((1U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
		 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch))) {
		__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		__Vdly__MemL1A__DOT__memDc__DOT__tMmioDone = 1U;
	    } else {
		if (((2U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK)) 
		     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch))) {
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr;
		    __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm;
		} else {
		    if ((0U == (IData)(vlTOPp->MemL1A__DOT__dfMemOK))) {
			__Vdly__MemL1A__DOT__memDc__DOT__tMemAddr 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr;
			__Vdly__MemL1A__DOT__memDc__DOT__tMmioLatch = 1U;
			__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm 
			    = vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm;
		    } else {
			__Vdly__MemL1A__DOT__memDc__DOT__tMemOpm = 0U;
		    }
		}
	    }
	}
    } else {
	__Vdly__MemL1A__DOT__memDc__DOT__tMmioDone = 0U;
    }
    vlTOPp->MemL1A__DOT__memIc__DOT__tDoStBlk = __Vdly__MemL1A__DOT__memIc__DOT__tDoStBlk;
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[0U] 
	= __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[0U];
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[1U] 
	= __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[1U];
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[2U] 
	= __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[2U];
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkData[3U] 
	= __Vdly__MemL1A__DOT__memIc__DOT__tStBlkData[3U];
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkIx = __Vdly__MemL1A__DOT__memIc__DOT__tStBlkIx;
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkFlag = __Vdly__MemL1A__DOT__memIc__DOT__tStBlkFlag;
    vlTOPp->MemL1A__DOT__memIc__DOT__tStBlkAddr = __Vdly__MemL1A__DOT__memIc__DOT__tStBlkAddr;
    vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchA = __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchA;
    vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchB = __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchB;
    vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnA = __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnA;
    vlTOPp->MemL1A__DOT__memIc__DOT__tMemLatchDnB = __Vdly__MemL1A__DOT__memIc__DOT__tMemLatchDnB;
    // ALWAYSPOST at MemIcA.v:187
    if (__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memIc__DOT__icCaMemA[__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemA__v0] 
	    = __Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemA__v0;
    }
    // ALWAYSPOST at MemIcA.v:188
    if (__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memIc__DOT__icCaMemB[__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaMemB__v0] 
	    = __Vdlyvval__MemL1A__DOT__memIc__DOT__icCaMemB__v0;
    }
    // ALWAYSPOST at MemIcA.v:189
    if (__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrA[__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrA__v0] 
	    = __Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrA__v0;
    }
    // ALWAYSPOST at MemIcA.v:190
    if (__Vdlyvset__MemL1A__DOT__memIc__DOT__icCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memIc__DOT__icCaAddrB[__Vdlyvdim0__MemL1A__DOT__memIc__DOT__icCaAddrB__v0] 
	    = __Vdlyvval__MemL1A__DOT__memIc__DOT__icCaAddrB__v0;
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnA = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbA = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdA = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchDnB = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchDnB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWdB = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWdB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchWbB = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchWbB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm = __Vdly__MemL1A__DOT__memDc__DOT__tMemOpm;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr = __Vdly__MemL1A__DOT__memDc__DOT__tMemAddr;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U] 
	= __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U] 
	= __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U] 
	= __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U] 
	= __Vdly__MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB = __Vdly__MemL1A__DOT__memDc__DOT__tMemLatchB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch = __Vdly__MemL1A__DOT__memDc__DOT__tMmioLatch;
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA = __Vdly__MemL1A__DOT__memDc__DOT__tReqAddrA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA = __Vdly__MemL1A__DOT__memDc__DOT__tReqIxA;
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB = __Vdly__MemL1A__DOT__memDc__DOT__tReqAddrB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB = __Vdly__MemL1A__DOT__memDc__DOT__tReqIxB;
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio = __Vdly__MemL1A__DOT__memDc__DOT__tReqIsMmio;
    vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm = __Vdly__MemL1A__DOT__memDc__DOT__tInOpm;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn = __Vdly__MemL1A__DOT__memDc__DOT__tDataIn;
    vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr = __Vdly__MemL1A__DOT__memDc__DOT__tInAddr;
    vlTOPp->MemL1A__DOT__memDc__DOT__tMmioDone = __Vdly__MemL1A__DOT__memDc__DOT__tMmioDone;
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tReqIxA = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA;
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tReqIxB = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB;
    // ALWAYS at MemIcA.v:78
    vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx = (3U 
						  & (vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr 
						     >> 1U));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB)
	  : (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB 
		     >> 0x20U)) : (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA 
					   >> 0x20U)));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA)
	  : (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[3U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA 
		     >> 0x20U)) : (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB 
					   >> 0x20U)));
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA0 = ((0x8000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						 ? 
						((0x4000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						  ? 
						 ((0x2000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						   ? 
						  ((0x800U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						    ? 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA1 = ((0x80000000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						 ? 
						((0x40000000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						  ? 
						 ((0x20000000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						   ? 
						  ((0x8000000U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						    ? 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA2 = ((0x8000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						 ? 
						((0x4000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						  ? 
						 ((0x2000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						   ? 
						  ((0x800U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						    ? 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA3 = ((0x80000000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						 ? 
						((0x40000000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						  ? 
						 ((0x20000000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						   ? 
						  ((0x8000000U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						    ? 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    if ((2U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	if ((1U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[3U])) 
		    << 0x30U) | (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
				  << 0x10U) | ((QData)((IData)(
							       vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
					       >> 0x10U)));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA3));
	} else {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
		    << 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA2));
	}
    } else {
	if ((1U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
		    << 0x30U) | (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
				  << 0x10U) | ((QData)((IData)(
							       vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])) 
					       >> 0x10U)));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA1));
	} else {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
		    << 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA0));
	}
    }
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA;
    // ALWAYS at MemIcA.v:173
    vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB;
    // ALWAYS at MemDcA.v:334
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA][0U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA][1U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA][2U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA][3U];
    // ALWAYS at MemDcA.v:334
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB][0U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB][1U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB][2U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U] 
	= vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB
	[vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB][3U];
    // ALWAYS at MemDcA.v:354
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA = (0xfU 
						  & (vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrA
						     [vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA] 
						     >> 0x1cU));
    // ALWAYS at MemDcA.v:354
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA = (0xfffffffU 
						  & vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrA
						  [vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA]);
    // ALWAYS at MemDcA.v:355
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB = (0xfU 
						  & (vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrB
						     [vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB] 
						     >> 0x1cU));
    // ALWAYS at MemDcA.v:355
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB = (0xfffffffU 
						  & vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrB
						  [vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB]);
    vlTOPp->icOutPcVal = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal;
    vlTOPp->icOutPcStep = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep;
    // ALWAYSPOST at MemDcA.v:364
    if (__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0][0U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0][1U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0][2U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemA[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemA__v0][3U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemA__v0[3U];
    }
    // ALWAYSPOST at MemDcA.v:376
    if (__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemB__v0) {
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0][0U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0][1U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0][2U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaMemB[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaMemB__v0][3U] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaMemB__v0[3U];
    }
    // ALWAYSPOST at MemDcA.v:365
    if (__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemA__v0) {
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrA[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrA__v0;
    }
    // ALWAYSPOST at MemDcA.v:377
    if (__Vdlyvset__MemL1A__DOT__memDc__DOT__dcCaMemB__v0) {
	vlTOPp->MemL1A__DOT__memDc__DOT__dcCaAddrB[__Vdlyvdim0__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0] 
	    = __Vdlyvval__MemL1A__DOT__memDc__DOT__dcCaAddrB__v0;
    }
    // ALWAYS at MemIcA.v:78
    vlTOPp->MemL1A__DOT__memIc__DOT__tMissA = ((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrA 
						!= vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrA) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagA)))));
    vlTOPp->MemL1A__DOT__memIc__DOT__tMissB = ((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrB 
						!= vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrB) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagB)))));
    vlTOPp->MemL1A__DOT__memIc__DOT__tMiss = ((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissA) 
					      | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissB));
    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcOK = 
	((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMiss)
	  ? 2U : 1U);
    vlTOPp->icOutPcOK = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcOK;
}

void VMemL1A::_settle__TOP__2(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_settle__TOP__2\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp33,127,0,4);
    VL_SIGW(__Vtemp39,127,0,4);
    VL_SIGW(__Vtemp46,127,0,4);
    VL_SIGW(__Vtemp48,127,0,4);
    // Body
    // ALWAYS at MemIcA.v:78
    vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx = (3U 
						  & (vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr 
						     >> 1U));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB)
	  : (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB 
		     >> 0x20U)) : (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA 
					   >> 0x20U)));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA)
	  : (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB));
    vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[3U] = 
	((8U & vlTOPp->MemL1A__DOT__memIc__DOT__tInAddr)
	  ? (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataA 
		     >> 0x20U)) : (IData)((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkDataB 
					   >> 0x20U)));
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA0 = ((0x8000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						 ? 
						((0x4000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						  ? 
						 ((0x2000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						   ? 
						  ((0x800U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						    ? 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA1 = ((0x80000000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						 ? 
						((0x40000000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						  ? 
						 ((0x20000000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						   ? 
						  ((0x8000000U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						    ? 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA2 = ((0x8000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						 ? 
						((0x4000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						  ? 
						 ((0x2000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						   ? 
						  ((0x800U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						    ? 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x400U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    vlTOPp->MemL1A__DOT__memIc__DOT__opLenA3 = ((0x80000000U 
						 & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						 ? 
						((0x40000000U 
						  & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						  ? 
						 ((0x20000000U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						   ? 
						  ((0x8000000U 
						    & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						    ? 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 3U
						     : 2U)
						    : 
						   ((0x4000000U 
						     & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])
						     ? 6U
						     : 2U))
						   : 1U)
						  : 1U)
						 : 1U);
    if ((2U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	if ((1U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[3U])) 
		    << 0x30U) | (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
				  << 0x10U) | ((QData)((IData)(
							       vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
					       >> 0x10U)));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA3));
	} else {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
		    << 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA2));
	}
    } else {
	if ((1U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tInWordIx))) {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[2U])) 
		    << 0x30U) | (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
				  << 0x10U) | ((QData)((IData)(
							       vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])) 
					       >> 0x10U)));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA1));
	} else {
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal 
		= (((QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[1U])) 
		    << 0x20U) | (QData)((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkData[0U])));
	    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep 
		= (3U & (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__opLenA0));
	}
    }
    // ALWAYS at MemL1A.v:110
    vlTOPp->MemL1A__DOT__tNxtLatchIc = 0U;
    vlTOPp->MemL1A__DOT__tNxtLatchDc = 0U;
    vlTOPp->MemL1A__DOT__tMemAddr = 0U;
    vlTOPp->MemL1A__DOT__tMemOpm = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 0U;
    vlTOPp->MemL1A__DOT__ifMemData[0U] = vlTOPp->memDataIn[0U];
    vlTOPp->MemL1A__DOT__ifMemData[1U] = vlTOPp->memDataIn[1U];
    vlTOPp->MemL1A__DOT__ifMemData[2U] = vlTOPp->memDataIn[2U];
    vlTOPp->MemL1A__DOT__ifMemData[3U] = vlTOPp->memDataIn[3U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[0U] = vlTOPp->memDataIn[0U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[1U] = vlTOPp->memDataIn[1U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[2U] = vlTOPp->memDataIn[2U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[3U] = vlTOPp->memDataIn[3U];
    vlTOPp->MemL1A__DOT__tIfNzOpm = (0U != (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm));
    vlTOPp->MemL1A__DOT__tDfNzOpm = (0U != (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm));
    vlTOPp->MemL1A__DOT__ifMemOK = ((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm)
				     ? 2U : 0U);
    vlTOPp->MemL1A__DOT__dfMemOK = ((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm)
				     ? 2U : 0U);
    if ((((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm) & 
	  (~ (IData)(vlTOPp->MemL1A__DOT__tLatchDc))) 
	 | (IData)(vlTOPp->MemL1A__DOT__tLatchIc))) {
	vlTOPp->MemL1A__DOT__ifMemOK = vlTOPp->memOK;
	vlTOPp->MemL1A__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr;
	vlTOPp->MemL1A__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm;
	vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 0U;
	vlTOPp->MemL1A__DOT__tNxtLatchIc = ((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm) 
					    | (0U != (IData)(vlTOPp->memOK)));
    } else {
	if ((((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm) 
	      & (~ (IData)(vlTOPp->MemL1A__DOT__tLatchIc))) 
	     | (IData)(vlTOPp->MemL1A__DOT__tLatchDc))) {
	    vlTOPp->MemL1A__DOT__dfMemOK = vlTOPp->memOK;
	    vlTOPp->MemL1A__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr;
	    vlTOPp->MemL1A__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm;
	    vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
	    vlTOPp->MemL1A__DOT__tNxtLatchDc = ((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm) 
						| (0U 
						   != (IData)(vlTOPp->memOK)));
	}
    }
    // ALWAYS at MemIcA.v:78
    if ((8U & vlTOPp->icInPcAddr)) {
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB 
	    = (0xfffffffU & (vlTOPp->icInPcAddr >> 4U));
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB));
    } else {
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA 
	    = (0xfffffffU & (vlTOPp->icInPcAddr >> 4U));
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA;
    }
    vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA = (0x3fU 
						& vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA);
    vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB = (0x3fU 
						& vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB);
    // ALWAYS at MemIcA.v:78
    vlTOPp->MemL1A__DOT__memIc__DOT__tMissA = ((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrA 
						!= vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrA) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrA) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagA)))));
    vlTOPp->MemL1A__DOT__memIc__DOT__tMissB = ((vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrB 
						!= vlTOPp->MemL1A__DOT__memIc__DOT__tReqAddrB) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memIc__DOT__tBlkAddrB) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tBlkFlagB)))));
    vlTOPp->MemL1A__DOT__memIc__DOT__tMiss = ((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissA) 
					      | (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMissB));
    vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcOK = 
	((IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMiss)
	  ? 2U : 1U);
    // ALWAYS at MemDcA.v:136
    if ((0x10U & vlTOPp->dcInAddr)) {
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
	    = (0xfffffffU & (vlTOPp->dcInAddr >> 4U));
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB));
    } else {
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
	    = (0xfffffffU & (vlTOPp->dcInAddr >> 4U));
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA));
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxA = (0x3fU 
						& (vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
						   >> 1U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxB = (0x3fU 
						& (vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
						   >> 1U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIsMmio = (0xaU 
						   == 
						   (0xfU 
						    & (vlTOPp->dcInAddr 
						       >> 0x1cU)));
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx = (7U 
						  & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr);
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz = (0U 
						  != 
						  (3U 
						   & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm) 
						      >> 3U)));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMissA = ((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA 
						!= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA)))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMissB = ((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB 
						!= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB)))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMiss = (1U & 
					      ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)
					        ? ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA) 
						   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB))
					        : 0U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tHold = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA) 
						 | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U]));
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)) {
	VL_WRITEF("Addr=%x tBlkData=%x\n",32,vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr,
		  128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData);
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData = (
						   (4U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x28U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 0x18U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x30U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 0x10U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x38U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 8U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x28U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 0x18U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x30U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 0x10U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x38U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 8U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutVal = (
						   (4U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData
						      : (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? (QData)((IData)(
									(0xffffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))
						      : (QData)((IData)(
									(0xffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData
						      : 
						     (((QData)((IData)(
								       ((1U 
									 & (IData)(
										(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
										>> 0x1fU)))
									 ? 0xffffffffU
									 : 0U))) 
						       << 0x20U) 
						      | (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? 
						     ((((1U 
							 & (IData)(
								   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								    >> 0xfU)))
							 ? VL_ULL(0xffffffffffff)
							 : VL_ULL(0)) 
						       << 0x10U) 
						      | (QData)((IData)(
									(0xffffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData)))))
						      : 
						     ((((1U 
							 & (IData)(
								   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								    >> 7U)))
							 ? VL_ULL(0xffffffffffffff)
							 : VL_ULL(0)) 
						       << 8U) 
						      | (QData)((IData)(
									(0xffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData = (
						   (2U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						    ? 
						   ((1U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn
						     : 
						    (((QData)((IData)(
								      (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								       >> 0x20U))) 
						      << 0x20U) 
						     | (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn))))
						    : 
						   ((1U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((VL_ULL(0xffffffffffff0000) 
						      & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData) 
						     | (QData)((IData)(
								       (0xffffU 
									& (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn)))))
						     : 
						    ((VL_ULL(0xffffffffffffff00) 
						      & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData) 
						     | (QData)((IData)(
								       (0xffU 
									& (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn)))))));
    __Vtemp33[1U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xff000000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 0x18U)) 
			     | (IData)(((VL_ULL(0xffffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U)))
			  : ((0xffff0000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 0x10U)) 
			     | (IData)(((VL_ULL(0xffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffff00U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 8U)) 
			     | (IData)(((VL_ULL(0xffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U)))
			  : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData)));
    __Vtemp33[2U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					   >> 8U)) 
			     | (0xff000000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x18U)))
			  : ((0xffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					 >> 0x10U)) 
			     | (0xffff0000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
				       >> 0x18U)) | 
			     (0xffffff00U & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     << 8U)))
			  : (IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
				     >> 0x20U))));
    __Vtemp39[3U] = ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((0xffffffU & ((IData)((VL_ULL(0xffffffffff) 
						& (((QData)((IData)(
								    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						    << 8U) 
						   | ((QData)((IData)(
								      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						      >> 0x18U)))) 
				       >> 8U)) | (0xff000000U 
						  & ((IData)(
							     ((VL_ULL(0xffffffffff) 
							       & (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								   << 8U) 
								  | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								     >> 0x18U))) 
							      >> 0x20U)) 
						     << 0x18U)))
		      : ((0xffffU & ((IData)((VL_ULL(0xffffffffffff) 
					      & (((QData)((IData)(
								  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						  << 0x10U) 
						 | ((QData)((IData)(
								    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						    >> 0x10U)))) 
				     >> 0x10U)) | (0xffff0000U 
						   & ((IData)(
							      ((VL_ULL(0xffffffffffff) 
								& (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								    << 0x10U) 
								   | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								      >> 0x10U))) 
							       >> 0x20U)) 
						      << 0x10U))));
    __Vtemp46[1U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					   >> 8U)) 
			     | (0xff000000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x18U)))
			  : ((0xffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					 >> 0x10U)) 
			     | (0xffff0000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
				       >> 0x18U)) | 
			     (0xffffff00U & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     << 8U)))
			  : (IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
				     >> 0x20U))));
    __Vtemp46[2U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xff000000U & ((IData)(
						     (VL_ULL(0xffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 0x18U)))) 
					     << 0x18U)) 
			     | (0xffffffU & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     >> 8U)))
			  : ((0xffff0000U & ((IData)(
						     (VL_ULL(0xffffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 0x10U)))) 
					     << 0x10U)) 
			     | (0xffffU & ((IData)(
						   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						    >> 0x20U)) 
					   >> 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffff00U & ((IData)(
						     (VL_ULL(0xffffffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 8U)))) 
					     << 8U)) 
			     | (0xffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						  >> 0x20U)) 
					 >> 0x18U)))
			  : (IData)((((QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
				      << 0x20U) | (QData)((IData)(
								  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U]))))));
    __Vtemp46[3U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? __Vtemp39[3U] : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
					  ? ((0xffU 
					      & ((IData)(
							 (VL_ULL(0xffffffffffffff) 
							  & (((QData)((IData)(
									      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							      << 0x18U) 
							     | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								>> 8U)))) 
						 >> 0x18U)) 
					     | (0xffffff00U 
						& ((IData)(
							   ((VL_ULL(0xffffffffffffff) 
							     & (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								 << 0x18U) 
								| ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								   >> 8U))) 
							    >> 0x20U)) 
						   << 8U)))
					  : (IData)(
						    ((((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U]))) 
						     >> 0x20U))));
    __Vtemp48[0U] = ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? (IData)((VL_ULL(0xffffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))))
			      : (IData)((VL_ULL(0xffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))))))
			  : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? (IData)((VL_ULL(0xffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))))
			      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
		      : ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? ((0xff000000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 0x18U)) 
				 | (0xffffffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
			      : ((0xffff0000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 0x10U)) 
				 | (0xffffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))
			  : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? ((0xffffff00U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 8U)) 
				 | (0xffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
			      : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U] 
	= __Vtemp48[0U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? __Vtemp33[1U] : __Vtemp46[1U]);
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? __Vtemp33[2U] : __Vtemp46[2U]);
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	        ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		    ? ((0xff000000U & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
		       | (0xffffffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						>> 0x20U)) 
				       >> 8U))) : (
						   (0xffff0000U 
						    & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
						   | (0xffffU 
						      & ((IData)(
								 (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
								  >> 0x20U)) 
							 >> 0x10U))))
	        : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		    ? ((0xffffff00U & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
		       | (0xffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
					    >> 0x20U)) 
				   >> 0x18U))) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]))
	    : __Vtemp46[3U]);
    if (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz) 
	 & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)))) {
	vlTOPp->MemL1A__DOT__memDc__DOT__tHold = (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMiss) 
						   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk)) 
						  | ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA) 
						     | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB)));
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tHold)) {
	VL_WRITEF("L1D$ Hold, Miss=%1#(%1#,%1#) MiBlk=%1#\n",
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMiss,
		  1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA),
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMissB,
		  1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk));
	if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA)) {
	    VL_WRITEF("L1D$ MissA, Blk=%x, Req=%x\n",
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA,
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA);
	}
	if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB)) {
	    VL_WRITEF("L1D$ MissB, Blk=%x, Req=%x\n",
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB,
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB);
	}
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk)) {
	VL_WRITEF("L1D$ Update Missed Block\n");
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrB;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[3U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[3U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagA 
	    = (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA));
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagB 
	    = (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB));
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxB;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB;
    } else {
	if (VL_UNLIKELY((1U & (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm) 
				>> 4U) & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)))))) {
	    VL_WRITEF("L1D$ Do Write Block A=%x D=%x\n",
		      32,vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr,
		      128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW);
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagA 
		= (4U | (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA)));
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagB 
		= (4U | (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB)));
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 0U;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 0U;
	    if ((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		if ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		}
	    } else {
		if ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		}
	    }
	}
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)) {
	VL_WRITEF("MMIO\n");
	if ((1U & ((~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioDone)) 
		   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch)))) {
	    vlTOPp->MemL1A__DOT__memDc__DOT__tHold = 1U;
	}
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutOK = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tHold)
						   ? 2U
						   : 
						  ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)
						    ? 1U
						    : 0U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tLstHold)
						 ? (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA)
						 : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxA));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tLstHold)
						 ? (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB)
						 : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxB));
    vlTOPp->icOutPcVal = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcVal;
    vlTOPp->icOutPcStep = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcStep;
    vlTOPp->memAddr = vlTOPp->MemL1A__DOT__tMemAddr;
    vlTOPp->memOpm = vlTOPp->MemL1A__DOT__tMemOpm;
    vlTOPp->memDataOut[0U] = vlTOPp->MemL1A__DOT__tMemDataOut[0U];
    vlTOPp->memDataOut[1U] = vlTOPp->MemL1A__DOT__tMemDataOut[1U];
    vlTOPp->memDataOut[2U] = vlTOPp->MemL1A__DOT__tMemDataOut[2U];
    vlTOPp->memDataOut[3U] = vlTOPp->MemL1A__DOT__tMemDataOut[3U];
    vlTOPp->icOutPcOK = vlTOPp->MemL1A__DOT__memIc__DOT__tRegOutPcOK;
    vlTOPp->dcOutVal = vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutVal;
    vlTOPp->dcOutOK = vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutOK;
}

VL_INLINE_OPT void VMemL1A::_combo__TOP__3(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_combo__TOP__3\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    VL_SIGW(__Vtemp74,127,0,4);
    VL_SIGW(__Vtemp80,127,0,4);
    VL_SIGW(__Vtemp87,127,0,4);
    VL_SIGW(__Vtemp89,127,0,4);
    // Body
    // ALWAYS at MemL1A.v:110
    vlTOPp->MemL1A__DOT__tNxtLatchIc = 0U;
    vlTOPp->MemL1A__DOT__tNxtLatchDc = 0U;
    vlTOPp->MemL1A__DOT__tMemAddr = 0U;
    vlTOPp->MemL1A__DOT__tMemOpm = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 0U;
    vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 0U;
    vlTOPp->MemL1A__DOT__ifMemData[0U] = vlTOPp->memDataIn[0U];
    vlTOPp->MemL1A__DOT__ifMemData[1U] = vlTOPp->memDataIn[1U];
    vlTOPp->MemL1A__DOT__ifMemData[2U] = vlTOPp->memDataIn[2U];
    vlTOPp->MemL1A__DOT__ifMemData[3U] = vlTOPp->memDataIn[3U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[0U] = vlTOPp->memDataIn[0U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[1U] = vlTOPp->memDataIn[1U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[2U] = vlTOPp->memDataIn[2U];
    vlTOPp->MemL1A__DOT__dfMemDataIn[3U] = vlTOPp->memDataIn[3U];
    vlTOPp->MemL1A__DOT__tIfNzOpm = (0U != (IData)(vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm));
    vlTOPp->MemL1A__DOT__tDfNzOpm = (0U != (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm));
    vlTOPp->MemL1A__DOT__ifMemOK = ((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm)
				     ? 2U : 0U);
    vlTOPp->MemL1A__DOT__dfMemOK = ((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm)
				     ? 2U : 0U);
    if ((((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm) & 
	  (~ (IData)(vlTOPp->MemL1A__DOT__tLatchDc))) 
	 | (IData)(vlTOPp->MemL1A__DOT__tLatchIc))) {
	vlTOPp->MemL1A__DOT__ifMemOK = vlTOPp->memOK;
	vlTOPp->MemL1A__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcAddr;
	vlTOPp->MemL1A__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memIc__DOT__tMemPcOpm;
	vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 0U;
	vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 0U;
	vlTOPp->MemL1A__DOT__tNxtLatchIc = ((IData)(vlTOPp->MemL1A__DOT__tIfNzOpm) 
					    | (0U != (IData)(vlTOPp->memOK)));
    } else {
	if ((((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm) 
	      & (~ (IData)(vlTOPp->MemL1A__DOT__tLatchIc))) 
	     | (IData)(vlTOPp->MemL1A__DOT__tLatchDc))) {
	    vlTOPp->MemL1A__DOT__dfMemOK = vlTOPp->memOK;
	    vlTOPp->MemL1A__DOT__tMemAddr = vlTOPp->MemL1A__DOT__memDc__DOT__tMemAddr;
	    vlTOPp->MemL1A__DOT__tMemOpm = vlTOPp->MemL1A__DOT__memDc__DOT__tMemOpm;
	    vlTOPp->MemL1A__DOT__tMemDataOut[0U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[0U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[1U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[1U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[2U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[2U];
	    vlTOPp->MemL1A__DOT__tMemDataOut[3U] = 
		vlTOPp->MemL1A__DOT__memDc__DOT__tMemDataOut[3U];
	    vlTOPp->MemL1A__DOT__tNxtLatchDc = ((IData)(vlTOPp->MemL1A__DOT__tDfNzOpm) 
						| (0U 
						   != (IData)(vlTOPp->memOK)));
	}
    }
    // ALWAYS at MemIcA.v:78
    if ((8U & vlTOPp->icInPcAddr)) {
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB 
	    = (0xfffffffU & (vlTOPp->icInPcAddr >> 4U));
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB));
    } else {
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA 
	    = (0xfffffffU & (vlTOPp->icInPcAddr >> 4U));
	vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB 
	    = vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA;
    }
    vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxA = (0x3fU 
						& vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrA);
    vlTOPp->MemL1A__DOT__memIc__DOT__tNxtIxB = (0x3fU 
						& vlTOPp->MemL1A__DOT__memIc__DOT__tNxtAddrB);
    // ALWAYS at MemDcA.v:136
    if ((0x10U & vlTOPp->dcInAddr)) {
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
	    = (0xfffffffU & (vlTOPp->dcInAddr >> 4U));
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB));
    } else {
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
	    = (0xfffffffU & (vlTOPp->dcInAddr >> 4U));
	vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
	    = (0xfffffffU & ((IData)(1U) + vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA));
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxA = (0x3fU 
						& (vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrA 
						   >> 1U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxB = (0x3fU 
						& (vlTOPp->MemL1A__DOT__memDc__DOT__tNxtAddrB 
						   >> 1U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIsMmio = (0xaU 
						   == 
						   (0xfU 
						    & (vlTOPp->dcInAddr 
						       >> 0x1cU)));
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx = (7U 
						  & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr);
    vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz = (0U 
						  != 
						  (3U 
						   & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm) 
						      >> 3U)));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMissA = ((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA 
						!= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagA)))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMissB = ((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB 
						!= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB) 
					       | ((3U 
						   & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB) 
						  != 
						  (3U 
						   & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkFlagB)))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tMiss = (1U & 
					      ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)
					        ? ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA) 
						   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB))
					        : 0U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tHold = 0U;
    vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA) 
						 | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
			  << 0x20U) | (QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))))
	      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U] = 
	((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	  ? ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])
	  : ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)
	      ? (IData)(((((QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
			   << 0x20U) | (QData)((IData)(
						       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))) 
			 >> 0x20U)) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U]));
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)) {
	VL_WRITEF("Addr=%x tBlkData=%x\n",32,vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr,
		  128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData);
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData = (
						   (4U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x28U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 0x18U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x30U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 0x10U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x38U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							    >> 8U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x28U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 0x18U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x30U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 0x10U))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
						      ? 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						       << 0x38U) 
						      | (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])) 
							    >> 8U)))
						      : 
						     (((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutVal = (
						   (4U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						    ? 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData
						      : (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData)))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? (QData)((IData)(
									(0xffffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))
						      : (QData)((IData)(
									(0xffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))))
						    : 
						   ((2U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData
						      : 
						     (((QData)((IData)(
								       ((1U 
									 & (IData)(
										(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
										>> 0x1fU)))
									 ? 0xffffffffU
									 : 0U))) 
						       << 0x20U) 
						      | (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))
						     : 
						    ((1U 
						      & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						      ? 
						     ((((1U 
							 & (IData)(
								   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								    >> 0xfU)))
							 ? VL_ULL(0xffffffffffff)
							 : VL_ULL(0)) 
						       << 0x10U) 
						      | (QData)((IData)(
									(0xffffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData)))))
						      : 
						     ((((1U 
							 & (IData)(
								   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								    >> 7U)))
							 ? VL_ULL(0xffffffffffffff)
							 : VL_ULL(0)) 
						       << 8U) 
						      | (QData)((IData)(
									(0xffU 
									 & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData))))))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData = (
						   (2U 
						    & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						    ? 
						   ((1U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn
						     : 
						    (((QData)((IData)(
								      (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData 
								       >> 0x20U))) 
						      << 0x20U) 
						     | (QData)((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn))))
						    : 
						   ((1U 
						     & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm))
						     ? 
						    ((VL_ULL(0xffffffffffff0000) 
						      & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData) 
						     | (QData)((IData)(
								       (0xffffU 
									& (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn)))))
						     : 
						    ((VL_ULL(0xffffffffffffff00) 
						      & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkExData) 
						     | (QData)((IData)(
								       (0xffU 
									& (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDataIn)))))));
    __Vtemp74[1U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xff000000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 0x18U)) 
			     | (IData)(((VL_ULL(0xffffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U)))
			  : ((0xffff0000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 0x10U)) 
			     | (IData)(((VL_ULL(0xffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffff00U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					     << 8U)) 
			     | (IData)(((VL_ULL(0xffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))) 
					>> 0x20U)))
			  : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData)));
    __Vtemp74[2U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					   >> 8U)) 
			     | (0xff000000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x18U)))
			  : ((0xffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					 >> 0x10U)) 
			     | (0xffff0000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
				       >> 0x18U)) | 
			     (0xffffff00U & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     << 8U)))
			  : (IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
				     >> 0x20U))));
    __Vtemp80[3U] = ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((0xffffffU & ((IData)((VL_ULL(0xffffffffff) 
						& (((QData)((IData)(
								    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						    << 8U) 
						   | ((QData)((IData)(
								      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						      >> 0x18U)))) 
				       >> 8U)) | (0xff000000U 
						  & ((IData)(
							     ((VL_ULL(0xffffffffff) 
							       & (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								   << 8U) 
								  | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								     >> 0x18U))) 
							      >> 0x20U)) 
						     << 0x18U)))
		      : ((0xffffU & ((IData)((VL_ULL(0xffffffffffff) 
					      & (((QData)((IData)(
								  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						  << 0x10U) 
						 | ((QData)((IData)(
								    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
						    >> 0x10U)))) 
				     >> 0x10U)) | (0xffff0000U 
						   & ((IData)(
							      ((VL_ULL(0xffffffffffff) 
								& (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								    << 0x10U) 
								   | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								      >> 0x10U))) 
							       >> 0x20U)) 
						      << 0x10U))));
    __Vtemp87[1U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					   >> 8U)) 
			     | (0xff000000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x18U)))
			  : ((0xffffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
					 >> 0x10U)) 
			     | (0xffff0000U & ((IData)(
						       (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
							>> 0x20U)) 
					       << 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffU & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
				       >> 0x18U)) | 
			     (0xffffff00U & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     << 8U)))
			  : (IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
				     >> 0x20U))));
    __Vtemp87[2U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xff000000U & ((IData)(
						     (VL_ULL(0xffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 8U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 0x18U)))) 
					     << 0x18U)) 
			     | (0xffffffU & ((IData)(
						     (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						      >> 0x20U)) 
					     >> 8U)))
			  : ((0xffff0000U & ((IData)(
						     (VL_ULL(0xffffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 0x10U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 0x10U)))) 
					     << 0x10U)) 
			     | (0xffffU & ((IData)(
						   (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						    >> 0x20U)) 
					   >> 0x10U))))
		      : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((0xffffff00U & ((IData)(
						     (VL_ULL(0xffffffffffffff) 
						      & (((QData)((IData)(
									  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							  << 0x18U) 
							 | ((QData)((IData)(
									    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
							    >> 8U)))) 
					     << 8U)) 
			     | (0xffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						  >> 0x20U)) 
					 >> 0x18U)))
			  : (IData)((((QData)((IData)(
						      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
				      << 0x20U) | (QData)((IData)(
								  vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U]))))));
    __Vtemp87[3U] = ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? __Vtemp80[3U] : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
					  ? ((0xffU 
					      & ((IData)(
							 (VL_ULL(0xffffffffffffff) 
							  & (((QData)((IData)(
									      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
							      << 0x18U) 
							     | ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								>> 8U)))) 
						 >> 0x18U)) 
					     | (0xffffff00U 
						& ((IData)(
							   ((VL_ULL(0xffffffffffffff) 
							     & (((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
								 << 0x18U) 
								| ((QData)((IData)(
										vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U])) 
								   >> 8U))) 
							    >> 0x20U)) 
						   << 8U)))
					  : (IData)(
						    ((((QData)((IData)(
								       vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U])) 
						       << 0x20U) 
						      | (QData)((IData)(
									vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[2U]))) 
						     >> 0x20U))));
    __Vtemp89[0U] = ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		      ? ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? (IData)((VL_ULL(0xffffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))))
			      : (IData)((VL_ULL(0xffffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))))))
			  : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? (IData)((VL_ULL(0xffffffffff) 
					 & (((QData)((IData)(
							     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[1U])) 
					     << 0x20U) 
					    | (QData)((IData)(
							      vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))))
			      : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
		      : ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			  ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? ((0xff000000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 0x18U)) 
				 | (0xffffffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
			      : ((0xffff0000U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 0x10U)) 
				 | (0xffffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U])))
			  : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
			      ? ((0xffffff00U & ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData) 
						 << 8U)) 
				 | (0xffU & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[0U]))
			      : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData))));
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U] 
	= __Vtemp89[0U];
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? __Vtemp74[1U] : __Vtemp87[1U]);
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? __Vtemp74[2U] : __Vtemp87[2U]);
    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U] 
	= ((4U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	    ? ((2U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
	        ? ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		    ? ((0xff000000U & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
		       | (0xffffffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
						>> 0x20U)) 
				       >> 8U))) : (
						   (0xffff0000U 
						    & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
						   | (0xffffU 
						      & ((IData)(
								 (vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
								  >> 0x20U)) 
							 >> 0x10U))))
	        : ((1U & (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInByteIx))
		    ? ((0xffffff00U & vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]) 
		       | (0xffU & ((IData)((vlTOPp->MemL1A__DOT__memDc__DOT__tBlkInData 
					    >> 0x20U)) 
				   >> 0x18U))) : vlTOPp->MemL1A__DOT__memDc__DOT__tBlkData[3U]))
	    : __Vtemp87[3U]);
    if (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz) 
	 & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)))) {
	vlTOPp->MemL1A__DOT__memDc__DOT__tHold = (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMiss) 
						   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk)) 
						  | ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchA) 
						     | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMemLatchB)));
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tHold)) {
	VL_WRITEF("L1D$ Hold, Miss=%1#(%1#,%1#) MiBlk=%1#\n",
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMiss,
		  1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA),
		  1,vlTOPp->MemL1A__DOT__memDc__DOT__tMissB,
		  1,(IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk));
	if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tMissA)) {
	    VL_WRITEF("L1D$ MissA, Blk=%x, Req=%x\n",
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrA,
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA);
	}
	if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tMissB)) {
	    VL_WRITEF("L1D$ MissB, Blk=%x, Req=%x\n",
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkAddrB,
		      28,vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB);
	}
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlk)) {
	VL_WRITEF("L1D$ Update Missed Block\n");
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkAddrB;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataA[3U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[0U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[1U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[2U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkDataB[3U];
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagA 
	    = (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA));
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagB 
	    = (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB));
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tMiBlkIxB;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkA;
	vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB 
	    = vlTOPp->MemL1A__DOT__memDc__DOT__tDoMiBlkB;
    } else {
	if (VL_UNLIKELY((1U & (((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tInOpm) 
				>> 4U) & (~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)))))) {
	    VL_WRITEF("L1D$ Do Write Block A=%x D=%x\n",
		      32,vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr,
		      128,vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW);
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrA;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqAddrB;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagA 
		= (4U | (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrA)));
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkFlagB 
		= (4U | (3U & (~ vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkAddrB)));
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxA 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkIxB 
		= vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 0U;
	    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 0U;
	    if ((0x10U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		if ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[2U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		}
	    } else {
		if ((8U & vlTOPp->MemL1A__DOT__memDc__DOT__tInAddr)) {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataA[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkB = 1U;
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[0U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[1U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U]))) 
				   >> 0x20U));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[2U] 
			= (IData)((((QData)((IData)(
						    vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
				    << 0x20U) | (QData)((IData)(
								vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))));
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataB[3U] 
			= (IData)(((((QData)((IData)(
						     vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[3U])) 
				     << 0x20U) | (QData)((IData)(
								 vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataB[2U]))) 
				   >> 0x20U));
		} else {
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[0U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[0U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[1U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[1U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[2U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[2U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tStBlkDataA[3U] 
			= vlTOPp->MemL1A__DOT__memDc__DOT__tBlkDataW[3U];
		    vlTOPp->MemL1A__DOT__memDc__DOT__tDoStBlkA = 1U;
		}
	    }
	}
    }
    if (VL_UNLIKELY(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIsMmio)) {
	VL_WRITEF("MMIO\n");
	if ((1U & ((~ (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioDone)) 
		   | (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tMmioLatch)))) {
	    vlTOPp->MemL1A__DOT__memDc__DOT__tHold = 1U;
	}
    }
    vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutOK = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tHold)
						   ? 2U
						   : 
						  ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqOpmNz)
						    ? 1U
						    : 0U));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxA = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tLstHold)
						 ? (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxA)
						 : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxA));
    vlTOPp->MemL1A__DOT__memDc__DOT__tNx2IxB = ((IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tLstHold)
						 ? (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tReqIxB)
						 : (IData)(vlTOPp->MemL1A__DOT__memDc__DOT__tNxtIxB));
    vlTOPp->memAddr = vlTOPp->MemL1A__DOT__tMemAddr;
    vlTOPp->memOpm = vlTOPp->MemL1A__DOT__tMemOpm;
    vlTOPp->memDataOut[0U] = vlTOPp->MemL1A__DOT__tMemDataOut[0U];
    vlTOPp->memDataOut[1U] = vlTOPp->MemL1A__DOT__tMemDataOut[1U];
    vlTOPp->memDataOut[2U] = vlTOPp->MemL1A__DOT__tMemDataOut[2U];
    vlTOPp->memDataOut[3U] = vlTOPp->MemL1A__DOT__tMemDataOut[3U];
    vlTOPp->dcOutVal = vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutVal;
    vlTOPp->dcOutOK = vlTOPp->MemL1A__DOT__memDc__DOT__tRegOutOK;
}

void VMemL1A::_eval(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_eval\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__3(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void VMemL1A::_eval_initial(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_eval_initial\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VMemL1A::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::final\n"); );
    // Variables
    VMemL1A__Syms* __restrict vlSymsp = this->__VlSymsp;
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VMemL1A::_eval_settle(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_eval_settle\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

VL_INLINE_OPT QData VMemL1A::_change_request(VMemL1A__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_change_request\n"); );
    VMemL1A* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VMemL1A::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clock & 0xfeU))) {
	Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((reset & 0xfeU))) {
	Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((dcInOpm & 0xe0U))) {
	Verilated::overWidthError("dcInOpm");}
    if (VL_UNLIKELY((memOK & 0xfcU))) {
	Verilated::overWidthError("memOK");}
}
#endif // VL_DEBUG

void VMemL1A::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VMemL1A::_ctor_var_reset\n"); );
    // Body
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    icInPcAddr = VL_RAND_RESET_I(32);
    icOutPcVal = VL_RAND_RESET_Q(64);
    icOutPcOK = VL_RAND_RESET_I(2);
    icOutPcStep = VL_RAND_RESET_I(2);
    dcInAddr = VL_RAND_RESET_I(32);
    dcInOpm = VL_RAND_RESET_I(5);
    dcOutVal = VL_RAND_RESET_Q(64);
    dcInVal = VL_RAND_RESET_Q(64);
    dcOutOK = VL_RAND_RESET_I(2);
    regInDlr = VL_RAND_RESET_Q(64);
    regInDhr = VL_RAND_RESET_Q(64);
    regInMmcr = VL_RAND_RESET_Q(64);
    regInKrr = VL_RAND_RESET_Q(64);
    memAddr = VL_RAND_RESET_I(32);
    memOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(128,memDataIn);
    VL_RAND_RESET_W(128,memDataOut);
    memOK = VL_RAND_RESET_I(2);
    MemL1A__DOT__tMemAddr = VL_RAND_RESET_I(32);
    MemL1A__DOT__tMemOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(128,MemL1A__DOT__tMemDataOut);
    VL_RAND_RESET_W(128,MemL1A__DOT__ifMemData);
    MemL1A__DOT__ifMemOK = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(128,MemL1A__DOT__dfMemDataIn);
    MemL1A__DOT__dfMemOK = VL_RAND_RESET_I(2);
    MemL1A__DOT__tLatchIc = VL_RAND_RESET_I(1);
    MemL1A__DOT__tLatchDc = VL_RAND_RESET_I(1);
    MemL1A__DOT__tNxtLatchIc = VL_RAND_RESET_I(1);
    MemL1A__DOT__tNxtLatchDc = VL_RAND_RESET_I(1);
    MemL1A__DOT__tIfNzOpm = VL_RAND_RESET_I(1);
    MemL1A__DOT__tDfNzOpm = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tRegOutPcVal = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memIc__DOT__tRegOutPcOK = VL_RAND_RESET_I(2);
    MemL1A__DOT__memIc__DOT__tRegOutPcStep = VL_RAND_RESET_I(2);
    MemL1A__DOT__memIc__DOT__tMemPcAddr = VL_RAND_RESET_I(32);
    MemL1A__DOT__memIc__DOT__tMemPcOpm = VL_RAND_RESET_I(5);
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memIc__DOT__icCaMemA[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memIc__DOT__icCaMemB[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memIc__DOT__icCaAddrA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memIc__DOT__icCaAddrB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    MemL1A__DOT__memIc__DOT__tNxtAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tNxtAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tNxtIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memIc__DOT__tNxtIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memIc__DOT__tBlkDataA = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memIc__DOT__tBlkDataB = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memIc__DOT__tBlkAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tBlkAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tBlkFlagA = VL_RAND_RESET_I(4);
    MemL1A__DOT__memIc__DOT__tBlkFlagB = VL_RAND_RESET_I(4);
    MemL1A__DOT__memIc__DOT__tReqAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tReqAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tReqIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memIc__DOT__tReqIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memIc__DOT__tInAddr = VL_RAND_RESET_I(32);
    MemL1A__DOT__memIc__DOT__tInWordIx = VL_RAND_RESET_I(2);
    MemL1A__DOT__memIc__DOT__tMissA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tMissB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tMiss = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memIc__DOT__tBlkData);
    MemL1A__DOT__memIc__DOT__opLenA0 = VL_RAND_RESET_I(3);
    MemL1A__DOT__memIc__DOT__opLenA1 = VL_RAND_RESET_I(3);
    MemL1A__DOT__memIc__DOT__opLenA2 = VL_RAND_RESET_I(3);
    MemL1A__DOT__memIc__DOT__opLenA3 = VL_RAND_RESET_I(3);
    MemL1A__DOT__memIc__DOT__tMemLatchA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tMemLatchB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tMemLatchDnA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memIc__DOT__tMemLatchDnB = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memIc__DOT__tStBlkData);
    MemL1A__DOT__memIc__DOT__tStBlkAddr = VL_RAND_RESET_I(28);
    MemL1A__DOT__memIc__DOT__tStBlkFlag = VL_RAND_RESET_I(4);
    MemL1A__DOT__memIc__DOT__tStBlkIx = VL_RAND_RESET_I(6);
    MemL1A__DOT__memIc__DOT__tDoStBlk = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tRegOutVal = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memDc__DOT__tRegOutOK = VL_RAND_RESET_I(2);
    MemL1A__DOT__memDc__DOT__tMemAddr = VL_RAND_RESET_I(32);
    MemL1A__DOT__memDc__DOT__tMemOpm = VL_RAND_RESET_I(5);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tMemDataOut);
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__dcCaMemA[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__dcCaMemB[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memDc__DOT__dcCaAddrA[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<64; ++__Vi0) {
	    MemL1A__DOT__memDc__DOT__dcCaAddrB[__Vi0] = VL_RAND_RESET_I(32);
    }}
    MemL1A__DOT__memDc__DOT__tNxtAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tNxtAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tNxtIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tNxtIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tNxtIsMmio = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tBlkDataA);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tBlkDataB);
    MemL1A__DOT__memDc__DOT__tBlkAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tBlkAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tBlkFlagA = VL_RAND_RESET_I(4);
    MemL1A__DOT__memDc__DOT__tBlkFlagB = VL_RAND_RESET_I(4);
    MemL1A__DOT__memDc__DOT__tReqAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tReqAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tReqIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tReqIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tReqIsMmio = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tNx2IxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tNx2IxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tInAddr = VL_RAND_RESET_I(32);
    MemL1A__DOT__memDc__DOT__tInByteIx = VL_RAND_RESET_I(3);
    MemL1A__DOT__memDc__DOT__tInOpm = VL_RAND_RESET_I(5);
    MemL1A__DOT__memDc__DOT__tDataIn = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memDc__DOT__tMissA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMissB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMiss = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tHold = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tReqOpmNz = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tLstHold = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tStBlkDataA);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tStBlkDataB);
    MemL1A__DOT__memDc__DOT__tStBlkAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tStBlkAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tStBlkFlagA = VL_RAND_RESET_I(4);
    MemL1A__DOT__memDc__DOT__tStBlkFlagB = VL_RAND_RESET_I(4);
    MemL1A__DOT__memDc__DOT__tStBlkIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tStBlkIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tDoStBlkA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tDoStBlkB = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tMiBlkDataA);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tMiBlkDataB);
    MemL1A__DOT__memDc__DOT__tMiBlkAddrA = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tMiBlkAddrB = VL_RAND_RESET_I(28);
    MemL1A__DOT__memDc__DOT__tMiBlkIxA = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tMiBlkIxB = VL_RAND_RESET_I(6);
    MemL1A__DOT__memDc__DOT__tDoMiBlkA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tDoMiBlkB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tDoMiBlk = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tBlkData);
    VL_RAND_RESET_W(128,MemL1A__DOT__memDc__DOT__tBlkDataW);
    MemL1A__DOT__memDc__DOT__tBlkExData = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memDc__DOT__tBlkInData = VL_RAND_RESET_Q(64);
    MemL1A__DOT__memDc__DOT__tMemLatchA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchDnA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchDnB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchWbA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchWbB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchWdA = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMemLatchWdB = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMmioLatch = VL_RAND_RESET_I(1);
    MemL1A__DOT__memDc__DOT__tMmioDone = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

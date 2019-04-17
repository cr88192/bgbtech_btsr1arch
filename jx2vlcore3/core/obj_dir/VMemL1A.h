// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VMemL1A_H_
#define _VMemL1A_H_

#include "verilated_heavy.h"

class VMemL1A__Syms;

//----------

VL_MODULE(VMemL1A) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_OUT8(icOutPcOK,1,0);
    VL_OUT8(icOutPcStep,1,0);
    VL_IN8(dcInOpm,4,0);
    VL_OUT8(dcOutOK,1,0);
    VL_OUT8(memOpm,4,0);
    VL_IN8(memOK,1,0);
    VL_IN(icInPcAddr,31,0);
    VL_IN(dcInAddr,31,0);
    VL_OUT(memAddr,31,0);
    VL_INW(memDataIn,127,0,4);
    VL_OUTW(memDataOut,127,0,4);
    VL_OUT64(icOutPcVal,63,0);
    VL_OUT64(dcOutVal,63,0);
    VL_IN64(dcInVal,63,0);
    VL_IN64(regInDlr,63,0);
    VL_IN64(regInDhr,63,0);
    VL_IN64(regInMmcr,63,0);
    VL_IN64(regInKrr,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
	VL_SIG8(MemL1A__DOT__tMemOpm,4,0);
	VL_SIG8(MemL1A__DOT__ifMemOK,1,0);
	VL_SIG8(MemL1A__DOT__dfMemOK,1,0);
	VL_SIG8(MemL1A__DOT__tLatchIc,0,0);
	VL_SIG8(MemL1A__DOT__tLatchDc,0,0);
	VL_SIG8(MemL1A__DOT__tNxtLatchIc,0,0);
	VL_SIG8(MemL1A__DOT__tNxtLatchDc,0,0);
	VL_SIG8(MemL1A__DOT__tIfNzOpm,0,0);
	VL_SIG8(MemL1A__DOT__tDfNzOpm,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tRegOutPcOK,1,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tRegOutPcStep,1,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMemPcOpm,4,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tNxtIxA,5,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tNxtIxB,5,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tBlkFlagA,3,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tBlkFlagB,3,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tReqIxA,5,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tReqIxB,5,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tInWordIx,1,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMissA,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMissB,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMiss,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__opLenA0,2,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__opLenA1,2,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__opLenA2,2,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__opLenA3,2,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMemLatchA,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMemLatchB,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMemLatchDnA,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tMemLatchDnB,0,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tStBlkFlag,3,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tStBlkIx,5,0);
	VL_SIG8(MemL1A__DOT__memIc__DOT__tDoStBlk,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tRegOutOK,1,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemOpm,4,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tNxtIxA,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tNxtIxB,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tNxtIsMmio,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tBlkFlagA,3,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tBlkFlagB,3,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tReqIxA,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tReqIxB,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tReqIsMmio,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tNx2IxA,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tNx2IxB,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tInByteIx,2,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tInOpm,4,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMissA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMissB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMiss,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tHold,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tReqOpmNz,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tLstHold,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tStBlkFlagA,3,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tStBlkFlagB,3,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tStBlkIxA,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tStBlkIxB,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tDoStBlkA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tDoStBlkB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMiBlkIxA,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMiBlkIxB,5,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tDoMiBlkA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tDoMiBlkB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tDoMiBlk,0,0);
    };
    struct {
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchDnA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchDnB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchWbA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchWbB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchWdA,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMemLatchWdB,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMmioLatch,0,0);
	VL_SIG8(MemL1A__DOT__memDc__DOT__tMmioDone,0,0);
	VL_SIG(MemL1A__DOT__tMemAddr,31,0);
	VL_SIGW(MemL1A__DOT__tMemDataOut,127,0,4);
	VL_SIGW(MemL1A__DOT__ifMemData,127,0,4);
	VL_SIGW(MemL1A__DOT__dfMemDataIn,127,0,4);
	VL_SIG(MemL1A__DOT__memIc__DOT__tMemPcAddr,31,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tNxtAddrA,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tNxtAddrB,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tBlkAddrA,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tBlkAddrB,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tReqAddrA,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tReqAddrB,27,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__tInAddr,31,0);
	VL_SIGW(MemL1A__DOT__memIc__DOT__tBlkData,127,0,4);
	VL_SIGW(MemL1A__DOT__memIc__DOT__tStBlkData,127,0,4);
	VL_SIG(MemL1A__DOT__memIc__DOT__tStBlkAddr,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tMemAddr,31,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tMemDataOut,127,0,4);
	VL_SIG(MemL1A__DOT__memDc__DOT__tNxtAddrA,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tNxtAddrB,27,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tBlkDataA,127,0,4);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tBlkDataB,127,0,4);
	VL_SIG(MemL1A__DOT__memDc__DOT__tBlkAddrA,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tBlkAddrB,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tReqAddrA,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tReqAddrB,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tInAddr,31,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tStBlkDataA,127,0,4);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tStBlkDataB,127,0,4);
	VL_SIG(MemL1A__DOT__memDc__DOT__tStBlkAddrA,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tStBlkAddrB,27,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tMiBlkDataA,127,0,4);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tMiBlkDataB,127,0,4);
	VL_SIG(MemL1A__DOT__memDc__DOT__tMiBlkAddrA,27,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__tMiBlkAddrB,27,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tBlkData,127,0,4);
	VL_SIGW(MemL1A__DOT__memDc__DOT__tBlkDataW,127,0,4);
	VL_SIG64(MemL1A__DOT__memIc__DOT__tRegOutPcVal,63,0);
	VL_SIG64(MemL1A__DOT__memIc__DOT__tBlkDataA,63,0);
	VL_SIG64(MemL1A__DOT__memIc__DOT__tBlkDataB,63,0);
	VL_SIG64(MemL1A__DOT__memDc__DOT__tRegOutVal,63,0);
	VL_SIG64(MemL1A__DOT__memDc__DOT__tDataIn,63,0);
	VL_SIG64(MemL1A__DOT__memDc__DOT__tBlkExData,63,0);
	VL_SIG64(MemL1A__DOT__memDc__DOT__tBlkInData,63,0);
	VL_SIG64(MemL1A__DOT__memIc__DOT__icCaMemA[64],63,0);
	VL_SIG64(MemL1A__DOT__memIc__DOT__icCaMemB[64],63,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__icCaAddrA[64],31,0);
	VL_SIG(MemL1A__DOT__memIc__DOT__icCaAddrB[64],31,0);
	VL_SIGW(MemL1A__DOT__memDc__DOT__dcCaMemA[64],127,0,4);
	VL_SIGW(MemL1A__DOT__memDc__DOT__dcCaMemB[64],127,0,4);
	VL_SIG(MemL1A__DOT__memDc__DOT__dcCaAddrA[64],31,0);
	VL_SIG(MemL1A__DOT__memDc__DOT__dcCaAddrB[64],31,0);
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clock,0,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VMemL1A__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VMemL1A);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VMemL1A(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VMemL1A();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VMemL1A__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VMemL1A__Syms* symsp, bool first);
  private:
    static QData _change_request(VMemL1A__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(VMemL1A__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset();
  public:
    static void _eval(VMemL1A__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif // VL_DEBUG
  public:
    static void _eval_initial(VMemL1A__Syms* __restrict vlSymsp);
    static void _eval_settle(VMemL1A__Syms* __restrict vlSymsp);
    static void _sequent__TOP__1(VMemL1A__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(VMemL1A__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif // guard

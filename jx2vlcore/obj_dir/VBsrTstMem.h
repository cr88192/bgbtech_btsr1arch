// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VBsrTstMem_H_
#define _VBsrTstMem_H_

#include "verilated_heavy.h"

class VBsrTstMem__Syms;

//----------

VL_MODULE(VBsrTstMem) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(regInOpm,4,0);
    VL_OUT8(regOutOK,1,0);
    VL_OUT8(memOpm,4,0);
    VL_IN8(memOK,1,0);
    VL_OUT8(mmioOpm,4,0);
    VL_IN8(mmioOK,1,0);
    VL_IN(regInAddr,31,0);
    VL_IN(regInData,31,0);
    VL_OUT(regOutData,31,0);
    VL_INW(memInData,127,0,4);
    VL_OUTW(memOutData,127,0,4);
    VL_OUT(memAddr,31,0);
    VL_IN(mmioInData,31,0);
    VL_OUT(mmioOutData,31,0);
    VL_OUT(mmioAddr,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(BsrTstMem__DOT__memRegInOpm,4,0);
    VL_SIG8(BsrTstMem__DOT__dcRegInOpm,4,0);
    VL_SIG8(BsrTstMem__DOT__dcMemPcOK,1,0);
    VL_SIG8(BsrTstMem__DOT__icMemPcOK,1,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__tMmioOpm,4,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__tmpMmioOK,1,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__tNextTileSrSt,0,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__tRegOutOK,1,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__addrIsSRam,0,0);
    VL_SIG8(BsrTstMem__DOT__memTile__DOT__mmioBypass,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__tRegOutOK,1,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__tRegOutOK2,1,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__tMemPcOpm,4,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__icBlkDirty,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__icBlkReady,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__icBlkMiss,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__icBlkBypass,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__tIcBlkStore,0,0);
    VL_SIG8(BsrTstMem__DOT__dcTile__DOT__tIcBlkStoreB,0,0);
    VL_SIG8(BsrTstMem__DOT__icTile__DOT__tMemPcOpm,4,0);
    VL_SIG8(BsrTstMem__DOT__icTile__DOT__icBlkReady,0,0);
    VL_SIG8(BsrTstMem__DOT__icTile__DOT__icBlkMiss,0,0);
    VL_SIG8(BsrTstMem__DOT__icTile__DOT__icBlkHi,0,0);
    VL_SIG16(BsrTstMem__DOT__memTile__DOT__tAccTileIxA,11,0);
    VL_SIG16(BsrTstMem__DOT__memTile__DOT__tAccTileIxB,11,0);
    VL_SIG16(BsrTstMem__DOT__memTile__DOT__tRegTileIxA,11,0);
    VL_SIG16(BsrTstMem__DOT__memTile__DOT__tRegTileIxB,11,0);
    VL_SIG(BsrTstMem__DOT__memRegInAddr,19,0);
    VL_SIGW(BsrTstMem__DOT__memRegInData,127,0,4);
    VL_SIG(BsrTstMem__DOT__dcRegInAddr,31,0);
    VL_SIG(BsrTstMem__DOT__dcRegInData,31,0);
    VL_SIGW(BsrTstMem__DOT__dcMemPcDataI,127,0,4);
    VL_SIG(BsrTstMem__DOT__icRegInPc,31,0);
    VL_SIGW(BsrTstMem__DOT__icMemPcData,127,0,4);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__tMmioOutData,31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__tMmioAddr,31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__tmpMmioInData,31,0);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tRomTile,127,0,4);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tSRamTile,127,0,4);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tMemTile,127,0,4);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tOutData,127,0,4);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tNextTile,127,0,4);
    VL_SIGW(BsrTstMem__DOT__memTile__DOT__tNextTileA,127,0,4);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__tRegOutData,31,0);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__tMemPcAddr,19,0);
    VL_SIGW(BsrTstMem__DOT__dcTile__DOT__tMemPcDataO,127,0,4);
    VL_SIGW(BsrTstMem__DOT__dcTile__DOT__icBlkData,159,0,5);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__icBlkAddr,16,0);
    VL_SIGW(BsrTstMem__DOT__dcTile__DOT__icBlkDataSt,159,0,5);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__icReqAddr,16,0);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__tRegTmpData,31,0);
    VL_SIG(BsrTstMem__DOT__dcTile__DOT__tRegTmpDataSt,31,0);
    VL_SIG(BsrTstMem__DOT__icTile__DOT__tMemPcAddr,19,0);
    VL_SIGW(BsrTstMem__DOT__icTile__DOT__icBlkData,159,0,5);
    VL_SIG(BsrTstMem__DOT__icTile__DOT__icBlkAddr,16,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__romTileA[2048],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__romTileB[2048],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__romTileC[2048],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__romTileD[2048],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__sramTileA[512],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__sramTileB[512],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__sramTileC[512],31,0);
    VL_SIG(BsrTstMem__DOT__memTile__DOT__sramTileD[512],31,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clock,0,0);
    VL_SIG8(__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tRegOutOK,1,0);
    VL_SIGW(__Vchglast__TOP__BsrTstMem__DOT__memTile__DOT__tOutData,127,0,4);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VBsrTstMem__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VBsrTstMem);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VBsrTstMem(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VBsrTstMem();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VBsrTstMem__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VBsrTstMem__Syms* symsp, bool first);
  private:
    static QData _change_request(VBsrTstMem__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(VBsrTstMem__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset();
  public:
    static void _eval(VBsrTstMem__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif // VL_DEBUG
  public:
    static void _eval_initial(VBsrTstMem__Syms* __restrict vlSymsp);
    static void _eval_settle(VBsrTstMem__Syms* __restrict vlSymsp);
    static void _initial__TOP__3(VBsrTstMem__Syms* __restrict vlSymsp);
    static void _sequent__TOP__2(VBsrTstMem__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(VBsrTstMem__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif // guard

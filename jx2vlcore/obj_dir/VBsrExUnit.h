// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VBsrExUnit_H_
#define _VBsrExUnit_H_

#include "verilated_heavy.h"

class VBsrExUnit__Syms;

//----------

VL_MODULE(VBsrExUnit) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_OUT8(memOpm,4,0);
    VL_IN8(memOK,1,0);
    VL_OUT8(mmioOpm,4,0);
    VL_IN8(mmioOK,1,0);
    VL_INW(memInData,127,0,4);
    VL_OUTW(memOutData,127,0,4);
    VL_OUT(memAddr,31,0);
    VL_IN(mmioInData,31,0);
    VL_OUT(mmioOutData,31,0);
    VL_OUT(mmioAddr,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
	VL_SIG8(BsrExUnit__DOT__memRegInOpm,4,0);
	VL_SIG8(BsrExUnit__DOT__dcMemPcOK,1,0);
	VL_SIG8(BsrExUnit__DOT__icMemPcOK,1,0);
	VL_SIG8(BsrExUnit__DOT__gprIdUCmd,7,0);
	VL_SIG8(BsrExUnit__DOT__gprIdUIxt,7,0);
	VL_SIG8(BsrExUnit__DOT__gprRegIdRm,5,0);
	VL_SIG8(BsrExUnit__DOT__gprRegIdRn,5,0);
	VL_SIG8(BsrExUnit__DOT__gprRegExHold,0,0);
	VL_SIG8(BsrExUnit__DOT__exOpCmd,7,0);
	VL_SIG8(BsrExUnit__DOT__exRegIdIxt,7,0);
	VL_SIG8(BsrExUnit__DOT__exRegIdRn,5,0);
	VL_SIG8(BsrExUnit__DOT__exHold,0,0);
	VL_SIG8(BsrExUnit__DOT__exBranchFlush,0,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__tMmioOpm,4,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__tmpMmioOK,1,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__tNextTileSrSt,0,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__tRegOutOK,1,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__addrIsSRam,0,0);
	VL_SIG8(BsrExUnit__DOT__memTile__DOT__mmioBypass,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__tRegOutOK,1,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__tRegOutOK2,1,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__tMemPcOpm,4,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__icBlkDirty,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__icBlkReady,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__icBlkMiss,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__icBlkBypass,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__tIcBlkStore,0,0);
	VL_SIG8(BsrExUnit__DOT__dcTile__DOT__tIcBlkStoreB,0,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__tRegOutPcOK,1,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__tMemPcOpm,4,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__icBlkReady,0,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__icBlkMiss,0,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__icBlkHi,0,0);
	VL_SIG8(BsrExUnit__DOT__icTile__DOT__icBlkWordIx,2,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegN,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegM,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opUCmd,7,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opUIxt,7,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegO_Dfl,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegN_Dfl,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegM_Dfl,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegN_Cr,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegM_Cr,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegN_Sr,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opRegM_Sr,5,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opFmid,4,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opBty,2,0);
	VL_SIG8(BsrExUnit__DOT__decOp__DOT__opUCmdIx,7,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__tRegOutId,5,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__tRegOutOK,1,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__tMemOpm,4,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMaIxt,3,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMaBty,2,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMulOp,1,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMulOpDr,1,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMulOpPp,1,0);
	VL_SIG8(BsrExUnit__DOT__exOp__DOT__regMulOpPq,1,0);
	VL_SIG16(BsrExUnit__DOT__decIstrWord,15,0);
	VL_SIG16(BsrExUnit__DOT__memTile__DOT__tAccTileIxA,11,0);
	VL_SIG16(BsrExUnit__DOT__memTile__DOT__tAccTileIxB,11,0);
	VL_SIG16(BsrExUnit__DOT__memTile__DOT__tRegTileIxA,11,0);
	VL_SIG16(BsrExUnit__DOT__memTile__DOT__tRegTileIxB,11,0);
	VL_SIG(BsrExUnit__DOT__memRegInAddr,19,0);
	VL_SIGW(BsrExUnit__DOT__memRegInData,127,0,4);
    };
    struct {
	VL_SIGW(BsrExUnit__DOT__dcMemPcDataI,127,0,4);
	VL_SIG(BsrExUnit__DOT__icRegInPc,31,0);
	VL_SIGW(BsrExUnit__DOT__icMemPcData,127,0,4);
	VL_SIG(BsrExUnit__DOT__decRegPc,31,0);
	VL_SIG(BsrExUnit__DOT__gprIdImm,31,0);
	VL_SIG(BsrExUnit__DOT__gprIdValPc,31,0);
	VL_SIG(BsrExUnit__DOT__exRegValRm,31,0);
	VL_SIG(BsrExUnit__DOT__exRegValRn,31,0);
	VL_SIG(BsrExUnit__DOT__exImmValRi,31,0);
	VL_SIG(BsrExUnit__DOT__exIdInGenPc,31,0);
	VL_SIG(BsrExUnit__DOT__exCtlInPc,31,0);
	VL_SIG(BsrExUnit__DOT__exRegNextPc,31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__tMmioOutData,31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__tMmioAddr,31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__tmpMmioInData,31,0);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tRomTile,127,0,4);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tSRamTile,127,0,4);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tMemTile,127,0,4);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tOutData,127,0,4);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tNextTile,127,0,4);
	VL_SIGW(BsrExUnit__DOT__memTile__DOT__tNextTileA,127,0,4);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__tRegOutData,31,0);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__tRegOutData2,31,0);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__tMemPcAddr,19,0);
	VL_SIGW(BsrExUnit__DOT__dcTile__DOT__tMemPcDataO,127,0,4);
	VL_SIGW(BsrExUnit__DOT__dcTile__DOT__icBlkData,159,0,5);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__icBlkAddr,16,0);
	VL_SIGW(BsrExUnit__DOT__dcTile__DOT__icBlkDataSt,159,0,5);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__icReqAddr,16,0);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__tRegTmpData,31,0);
	VL_SIG(BsrExUnit__DOT__dcTile__DOT__tRegTmpDataSt,31,0);
	VL_SIG(BsrExUnit__DOT__icTile__DOT__tRegOutPcVal,31,0);
	VL_SIG(BsrExUnit__DOT__icTile__DOT__tMemPcAddr,19,0);
	VL_SIGW(BsrExUnit__DOT__icTile__DOT__icBlkData,159,0,5);
	VL_SIG(BsrExUnit__DOT__icTile__DOT__icBlkAddr,16,0);
	VL_SIG(BsrExUnit__DOT__decOp__DOT__opImm,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regSp,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regPc,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regPr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regSr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regVbr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regDlr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regDhr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGbr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regTbr,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__tRegValRm,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__tRegValRn,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR0A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR1A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR2A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR3A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR4A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR5A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR6A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR7A,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR8,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR9,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR10,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR11,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR12,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR13,31,0);
	VL_SIG(BsrExUnit__DOT__gprFile__DOT__regGprR14,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutSp,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutPc,31,0);
    };
    struct {
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutPr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutSr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutVbr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutDlr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutDhr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutGbr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tCtlOutTbr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tRegOutVal,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tMemAddr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__tMemData,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMaRbVal,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMaIxVal,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMaIxVal2,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMaAddr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regRnSxt,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulA,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulB,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulDlr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulDhr,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPpA,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPpB,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPpC,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPpD,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPpE,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPqA,31,0);
	VL_SIG(BsrExUnit__DOT__exOp__DOT__regMulPqB,31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__romTileA[2048],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__romTileB[2048],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__romTileC[2048],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__romTileD[2048],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__sramTileA[512],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__sramTileB[512],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__sramTileC[512],31,0);
	VL_SIG(BsrExUnit__DOT__memTile__DOT__sramTileD[512],31,0);
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clock,0,0);
    VL_SIG8(__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tRegOutOK,1,0);
    VL_SIGW(__Vchglast__TOP__BsrExUnit__DOT__memTile__DOT__tOutData,127,0,4);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VBsrExUnit__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VBsrExUnit);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VBsrExUnit(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VBsrExUnit();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VBsrExUnit__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VBsrExUnit__Syms* symsp, bool first);
  private:
    static QData _change_request(VBsrExUnit__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(VBsrExUnit__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset();
  public:
    static void _eval(VBsrExUnit__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif // VL_DEBUG
  public:
    static void _eval_initial(VBsrExUnit__Syms* __restrict vlSymsp);
    static void _eval_settle(VBsrExUnit__Syms* __restrict vlSymsp);
    static void _initial__TOP__3(VBsrExUnit__Syms* __restrict vlSymsp);
    static void _sequent__TOP__2(VBsrExUnit__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(VBsrExUnit__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif // guard

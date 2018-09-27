// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VJx2FpuExOp_H_
#define _VJx2FpuExOp_H_

#include "verilated_heavy.h"

class VJx2FpuExOp__Syms;

//----------

VL_MODULE(VJx2FpuExOp) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(opCmd,7,0);
    VL_IN8(regIdIxt,7,0);
    VL_IN8(regIdRm,6,0);
    VL_IN8(regIdRn,6,0);
    VL_OUT8(regOutId,6,0);
    VL_OUT8(regOutOK,1,0);
    VL_IN8(memDataOK,1,0);
    VL_IN64(regValRm,63,0);
    VL_IN64(regValRn,63,0);
    VL_OUT64(regOutVal,63,0);
    VL_IN64(memDataLd,63,0);
    VL_IN64(ctlInDlr,63,0);
    VL_OUT64(ctlOutDlr,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
	VL_SIG8(Jx2FpuExOp__DOT__tRegOutId,6,0);
	VL_SIG8(Jx2FpuExOp__DOT__tRegOutOK,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__tRegAddExOp,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__tMulRcpStable,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__tMulRcpSeed,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__tMulRcpSeed0,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__tRegMulExOp,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__tRegDivStrobe,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tRegExOK,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tExEn1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tExEn2,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnA2,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnB2,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC2,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tExEn3,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC3B,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tExEn4,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tSgnC4B,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_add__DOT__tExEn5,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tRegExOK,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnA1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnB1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC2,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC3,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tSgnC4B,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn1,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn2,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn3,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn4,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__tExEn5,1,0);
	VL_SIG8(Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tSgnB1,0,0);
	VL_SIG8(Jx2FpuExOp__DOT__nrcp__DOT__tSgnB1,0,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpA1D,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpB1D,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpA2,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpC2,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpC3B,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_add__DOT__tExpC4B,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpA1,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpB1,10,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC1,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AB_C5,9,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BA_C5,9,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC2,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC3,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4,11,0);
	VL_SIG16(Jx2FpuExOp__DOT__fpu_mul__DOT__tExpC4B,11,0);
	VL_SIG(Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1P,31,0);
	VL_SIG(Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1P,31,0);
	VL_SIG64(Jx2FpuExOp__DOT__tRegOutVal,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__tCtlOutDlr,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__tRegAddRn,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__tRegMulRm,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__tRegMulRn,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__tMulDivRcp,63,0);
    };
    struct {
	VL_SIG64(Jx2FpuExOp__DOT__regRcpValRn,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tRegValRo,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraA1D,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraB1D,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraA2,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraB2,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraJ2,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraC2,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraC3B,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tFraC4B,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_add__DOT__tValC4,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tRegValRo,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraA1,53,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraB1,53,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_AC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BB,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_BC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CA,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CB,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC1_CC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_AC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BB,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_BC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CA,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CB,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_CC,35,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_P,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_Q,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC2_R,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_P,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_Q,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_R,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC3_S,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4_S,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tFraC4B,53,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__tValC4,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__fpu_mul__DOT__mrcp__DOT__tFraB1_Adj1,51,0);
	VL_SIG64(Jx2FpuExOp__DOT__nrcp__DOT__tRegValFRn,63,0);
	VL_SIG64(Jx2FpuExOp__DOT__nrcp__DOT__tFraB1_Adj1,51,0);
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clock,0,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VJx2FpuExOp__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VJx2FpuExOp);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VJx2FpuExOp(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VJx2FpuExOp();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VJx2FpuExOp__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VJx2FpuExOp__Syms* symsp, bool first);
  private:
    static QData _change_request(VJx2FpuExOp__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(VJx2FpuExOp__Syms* __restrict vlSymsp);
    static void _combo__TOP__4(VJx2FpuExOp__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset();
  public:
    static void _eval(VJx2FpuExOp__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif // VL_DEBUG
  public:
    static void _eval_initial(VJx2FpuExOp__Syms* __restrict vlSymsp);
    static void _eval_settle(VJx2FpuExOp__Syms* __restrict vlSymsp);
    static void _sequent__TOP__3(VJx2FpuExOp__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(VJx2FpuExOp__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif // guard

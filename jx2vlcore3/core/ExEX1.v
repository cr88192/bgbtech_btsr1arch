/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
EX1 Stage (Top Level)

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [5:0]=Depends on opcode

EX1 will only initiate a memory Load/Store.
Holding/Completing a memory access will be the responsibility of EX2.
 */

`include "CoreDefs.v"

`ifdef jx2_enable_vaddr48
// `include "ExAGUB.v"
`include "ExAGUC.v"
`else
`include "ExAGU.v"
`endif

`include "ExConv2R.v"

`include "ExCpuId.v"

`ifdef jx2_merge_shadfn
`include "ExShad64D.v"
`endif

`ifndef jx2_merge_shadfn
`ifndef jx2_merge_shadq
`include "ExShad32B.v"
`include "ExShad64B.v"
`endif

`ifdef jx2_merge_shadq
`include "ExShad64C.v"
`endif
`endif

`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
`include "ExBtcUtx1.v"
`endif
`endif

`ifdef jx2_enable_fmov
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
`endif

`ifdef jx2_agu_ribound64
`include "ExBoundFp8.v"
`endif

`ifdef jx2_ena_alufast
`include "ExFastALU.v"
`endif

`ifdef jx2_enable_pmov
`include "ExConv_Fp16PckAl.v"
`endif

/* verilator lint_off DEFPARAM */

module ExEX1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,	exTrapExc,
	timers,	opUCmdOut,
	opUExtFl, opIstrWord,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regValXs,		//Source A, 128-bit
	regOutLea,		//LEA Output
	regOutXLea,		//XLEA Output

//	regValFRs,		//Source A Value (FPR)
//	regValFRt,		//Source B Value (FPR)
	regValCRm,		//Source C Value (CR)
	regValBPc,		//Instruction Base PC

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdCn1,		//Destination ID (CR, EX1)
	regValCn1,		//Destination Value (CR, EX1)
	heldIdRn1,		//Held Destination ID (EX1)
	heldIdCn1,		//Held Destination ID (CR, EX1)
	
	exDelayOut,

	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regFpuGRn,		//FPU GPR Result
	regFpuSrT,		//FPU SR.T Result
	opBraFlush,
	opPredNoExec,
	opPreBraPc,
	opPreBra,
	aluSrJcmpT,
	regLastSr,

//	regValPcHi,		//PC High Bits
//	regValGbrHi,	//GBR High Bits
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	regOutSchm,	regInSchm,

`ifdef jx2_enable_vaddr96
	regOutPcHi,		regValPcHi,
	regOutLrHi,		regValLrHi,
	regOutGbrHi,	regValGbrHi,
`endif
	
	memAddr,	memOpm,
	memDataOut,	memDataOutB,
	memLdOp,
	memDataOK,	regInExc
	);


input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
input[7:0]		opUExtFl;
input[95:0]		opIstrWord;

output[1:0]		exHold;
output[127:0]	exTrapExc;

input[11:0]		timers;
output[8:0]		opUCmdOut;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
`input_gprval		regValRs;		//Source A Value
`input_gprval		regValRt;		//Source B Value
`input_gprval		regValRm;		//Source C Value

`input_gprval		regValXs;		//Source C Value
`output_gprval	regOutLea;
`output_gprval	regOutXLea;

// `input_gprval		regValFRs;		//Source A Value (FPR)
// `input_gprval		regValFRt;		//Source B Value (FPR)
`input_gprval		regValCRm;		//Source C Value (CR)

(* max_fanout = 200 *)
	input[47:0]		regValBPc;

`output_gpr		regIdRn1;		//Destination ID (EX1)
`output_gprval	regValRn1;		//Destination Value (EX1)
// output[4:0]		regIdCn1;		//Destination ID (CR, EX1)
`output_gpr		regIdCn1;		//Destination ID (CR, EX1)
`output_gprval	regValCn1;		//Destination Value (CR, EX1)
`output_gpr		heldIdRn1;		//Held Destination ID (EX1)
// output[4:0]		heldIdCn1;		//Held Destination ID (CR, EX1)
`output_gpr		heldIdCn1;		//Held Destination ID (CR, EX1)

`output_gprval	exDelayOut;

// input[47:0]		regValPc;		//PC Value (Synthesized)
input[63:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)
input[63:0]		regFpuGRn;		//FPU GPR Result
input			regFpuSrT;
input			opBraFlush;
input			opPredNoExec;
input[47:0]		opPreBraPc;
input[1:0]		opPreBra;
input			aluSrJcmpT;
input[7:0]		regLastSr;

`ifdef jx2_enable_vaddr96
input[47:0]		regValPcHi;
input[47:0]		regValLrHi;
input[47:0]		regValGbrHi;

output[47:0]	regOutPcHi;
output[47:0]	regOutLrHi;
output[47:0]	regOutGbrHi;
`endif

parameter		isAltCore = 0;
defparam		cpuid.isAltCore = isAltCore;

output[63:0]	regOutDlr;
input[63:0]		regInDlr;
output[63:0]	regOutDhr;
input[63:0]		regInDhr;
output[63:0]	regOutSp;
input[63:0]		regInSp;

output[63:0]	regOutLr;
input[63:0]		regInLr;
output[63:0]	regOutSr;

// (* max_fanout = 50 *)
	input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

// output[47:0]	memAddr;
`output_vaddr	memAddr;
output[ 4:0]	memOpm;
output[ 4:0]	memLdOp;
output[65:0]	memDataOut;
output[65:0]	memDataOutB;

input[ 1:0]		memDataOK;

input[63:0]		regInExc;


`reg_gpr		tRegIdRn1;		//Destination ID (EX1)
`reg_gprval		tRegValRn1;		//Destination Value (EX1)
//reg[ 4:0]		tRegIdCn1;		//Destination ID (CR, EX1)
`reg_gpr		tRegIdCn1;		//Destination ID (CR, EX1)
`reg_gprval		tRegValCn1;		//Destination Value (CR, EX1)
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[63:0]		tRegOutLr;
reg[63:0]		tRegOutSr;
reg[7:0]		tRegOutSchm;

`ifdef jx2_enable_vaddr96
reg[47:0]		tRegOutPcHi;
reg[47:0]		tRegOutLrHi;
reg[47:0]		tRegOutGbrHi;

assign		regOutPcHi	= tRegOutPcHi;
assign		regOutLrHi	= tRegOutLrHi;
assign		regOutGbrHi	= tRegOutGbrHi;
`endif

`reg_gpr		tHeldIdRn1;		//Destination ID (EX1)
//reg[ 4:0]		tHeldIdCn1;		//Destination ID (CR, EX1)
`reg_gpr		tHeldIdCn1;		//Destination ID (CR, EX1)

reg[127:0]		tExTrapExc;

assign	regIdRn1	= tRegIdRn1;		//Destination ID (EX1)
assign	regValRn1	= tRegValRn1;		//Destination Value (EX1)
assign	regIdCn1	= tRegIdCn1;		//Destination ID (CR, EX1)
assign	regValCn1	= tRegValCn1;		//Destination Value (CR, EX1)
assign	heldIdRn1	= tHeldIdRn1;		//Held Destination ID (EX1)
assign	heldIdCn1	= tHeldIdCn1;		//Held Destination ID (CR, EX1)

assign	regOutDlr	= tRegOutDlr;
assign	regOutDhr	= tRegOutDhr;
assign	regOutSp	= tRegOutSp;
assign	regOutLr	= tRegOutLr;
assign	regOutSr	= tRegOutSr;
assign	regOutSchm	= tRegOutSchm;
assign	exTrapExc	= tExTrapExc;

`reg_gprval	tExDelayOut;
assign	exDelayOut = tExDelayOut;

// reg[47:0]	tMemAddr;
`reg_vaddr		tMemAddr;
reg[ 4:0]		tMemOpm;
reg[ 4:0]		tMemLdOp;
reg[65:0]		tMemDataOut;
reg[65:0]		tMemDataOutB;

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memLdOp		= tMemLdOp;
assign	memDataOut	= tMemDataOut;
assign	memDataOutB	= tMemDataOutB;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

wire		tBraIsRiscv;
// wire		tBraIsXG3;
assign	tBraIsRiscv = regInSr[26] && (regInSr[23:22]==2'b00);

// assign	tBraIsXG3 = tBraIsRiscv;

wire[32:0]		regValImmJCmp;

`ifdef jx2_enable_riscv_xg3
assign regValImmJCmp = opUIxt[5] ? { regValImm[31:0], 1'b0 } : regValImm;
`else
assign regValImmJCmp = regValImm;
`endif

reg		tAguFlagJq;
reg		tAguFlagSv;

wire		tXmovEnable;
wire		tXmovAdd;
wire		tXmovIsWxa;
wire		tXmovIsBx;

assign		tXmovIsBx = regValRs[63:61]==3'b001;

wire[33:0]	tRegBoundX;

assign	tRegBoundX = {
	tXmovIsWxa, tXmovIsBx,
	regValRs[63:60], regValXs[63:48], regValRs[59:48] };

wire	tAguBaseIsPC;
wire	tAguIsLea;
wire	tAguIsLeat;
wire	tAguIsLeatPC;
assign	tAguBaseIsPC	= (regIdRs == JX2_GR_PC);
assign	tAguIsLea		= (opUCmd[5:0] == JX2_UCMD_LEA_MR);
assign	tAguIsLeat		= tAguIsLea && opUIxt[2];
assign	tAguIsLeatPC	= tAguIsLeat && tAguBaseIsPC;

// `ifdef def_true
`ifndef def_true
always @*
begin
	if(tAguIsLeat)
	begin
		$display("LEAT: Rs=%x, IsPc=%d ", regIdRs, tAguIsLeatPC);
	end
end
`endif

`ifdef jx2_enable_vaddr48
wire[47:0]	tValAgu;
wire		tValAguOob;
wire[15:0]	tAguXLeaTag;

// ExAGUB	exAgu(regValRs[47:0], regValRt[47:0],
//	opUIxt, tValAgu, tAguFlagJq);
ExAGUC	exAgu(
//	regValRs[47:0],		regValRt[47:0],
	regValRs[63:0],		regValRt[63:0],
	regValImm[15:0],	regValXs[47:0],
	opUCmd,				opUIxt,
	tValAgu,			{ 1'b0, tXmovAdd, tAguFlagSv, tAguFlagJq},
	tRegBoundX,			tValAguOob,
	regOutXLea[63:0],	tAguXLeaTag);

wire[63:0]	tRegOutLeatPc =
//	{ regValPc[63:48], tValAgu[47:1], 1'b1 };
	{ 8'h00, regValPc[55:52], 1'b0, regValPc[50], 2'b00,
		tValAgu[47:1], 1'b1 };

// assign	regOutLea = { tAguXLeaTag, tValAgu };
assign	regOutLea[63:0] = tAguIsLeatPC ? 
	tRegOutLeatPc : 
	{ tAguXLeaTag, tValAgu };

`ifdef jx2_enable_memcap
wire[1:0] tRegCap2R = 
	((regValRs[65:64] == 2'b01) &&
	(regValXs[65:64] == 2'b01) &&
	(opUIxt[8:6]==JX2_IUC_WX)) ? 2'b01 : 2'b00;
assign	regOutLea[65:64] = tRegCap2R;
assign	regOutXLea[65:64] = tRegCap2R;
`endif

`else
wire[47:0]	tValAgu;
wire		tValAguOob;
wire[15:0]	tAguXLeaTag;

assign	tValAgu[47:32]	= UV16_00;
assign	tValAguOob		= 0;
assign	tAguXLeaTag		= 0;
assign	regOutXLea		= 0;

ExAGU	exAgu(regValRs[31:0], regValRt[31:0], opUIxt, tValAgu[31:0]);
`endif

wire		tValBound8Oob;

`ifdef jx2_agu_ribound64
ExBoundFp8	tboundfp8(
	regValRt[63:0], regValRs[63:48],
	opUIxt[7:0], tValBound8Oob);
`else
assign	tValBound8Oob = 1'b0;
`endif

wire[7:0]	regRefSr;

`ifdef jx2_cpu_pred_id2
assign	regRefSr = regLastSr;
`else
assign	regRefSr = regInSr[7:0];
`endif

wire[63:0]	tValCnv;
wire		tCnvSrT;
ExConv2R	exConv2R(regValRs[63:0], opUIxt, regRefSr[0], tValCnv, tCnvSrT);

`ifdef jx2_merge_shadfn

wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64D	exShad64(clock, reset,
	regValRs[63:0],
	regValXs[63:0],
	regValRt[7:0],
	tValShad64,
	opUIxt[5:0], 0);

`endif


`ifndef jx2_merge_shadfn

`ifndef jx2_merge_shadq
wire[31:0]	tValShad32;
//ExShad32	exShad32(clock, reset,
ExShad32B	exShad32(clock, reset,
	regValRs[31:0], regValRt[7:0],
	tValShad32, opUCmd[0]);

wire[63:0]	tValShad64;
//ExShad64	exShad64(clock, reset,
ExShad64B	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
	tValShad64, opUCmd[0]);
`endif

`ifdef jx2_merge_shadq
wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64C	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
	tValShad64, opUIxt[3:0]);
`endif

`endif

`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
wire[63:0]	tValUtx1;
ExBtcUtx1	exUtx1(
	regValRs[63:0],
	regValXs[63:0],
	regValRt[3:0],
	opUIxt, tValUtx1);
`endif
`endif

`ifdef jx2_ena_alufast
wire[63:0]	tValFastAlu;
wire		tFastAluOK;

ExFastALU	fastalu(
	clock, reset,
	opUCmd,	opUIxt,
	regValRs, regValRt,
	tValFastAlu,
	tFastAluOK);
`endif

wire[63:0]	tValCpuIdRng;
wire[63:0]	tValCpuIdLo;
wire[63:0]	tValCpuIdHi;
ExCpuId		cpuid(clock, reset, timers,
//	regIdRm[4:0], tValCpuIdLo, tValCpuIdHi);
	regIdRt[4:0], tValCpuIdLo, tValCpuIdHi, tValCpuIdRng);

// reg[63:0]	tRegSpAdd8;
// reg[63:0]	tRegSpSub8;
// reg[63:0]	tRegSpAddImm;

// reg[63:0]	tRegSpAdd16;
// reg[63:0]	tRegSpSub16;

reg[15:0]	tCanaryMagic;
reg[15:0]	tNxtCanaryMagic;
reg[15:0]	tCanaryRef;

reg			tOpEnable;
reg			tDoMemOp;
reg[4:0]	tDoMemOpm;
reg			tDoDelayCycle;

reg			tOpEnableB;

(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;
reg[5:0]	tOpUCmdF;
reg[8:0]	tOpUCmd2;

assign		opUCmdOut = tOpUCmd2;

reg[47:0]	tValBraDispSc;

(* max_fanout = 200 *)
	reg[47:0]	tValBraBasePc;

reg[16:0]	tValAguBraA0;
reg[16:0]	tValAguBraB0;
reg[16:0]	tValAguBraB1;
reg[16:0]	tValAguBraC0;
reg[16:0]	tValAguBraC1;

reg[63:0]	tRegBraLr;
reg[63:0]	tValAguBra;

reg[15:0]	tValAguBraJCmpMi0;
reg[15:0]	tValAguBraJCmpMi1;
reg[15:0]	tValAguBraJCmpLo;
reg[47:0]	tValAguBraJCmp;

// reg[47:0]	tValBra;
reg[63:0]	tValBra;
reg[63:0]	tValBraHi;
reg			tDoBra;

`reg_gprval	tValOutDfl;
reg			tDoOutDfl;

reg			tTempBit0;
reg			tTempBit1;
reg			tTempBit2;
reg			tTempBit3;

reg[3:0]	tIfThenPP;
reg[3:0]	tIfThenNN;
reg[3:0]	tIfElsePP;
reg[3:0]	tIfElseNN;

assign	tXmovEnable		= regInSr[25] || regInSr[30];
assign	tXmovAdd		= regInSr[24];
assign	tXmovIsWxa		= (	((opUIxt[8:6]==JX2_IUC_WA) ||
		(opUIxt[8:6]==JX2_IUC_WXA)) && tXmovEnable);


reg tMsgLatch;
reg tNextMsgLatch;

`ifdef jx2_enable_fmov
wire[31:0]	regValRm_D2S;		//memory data (Double to Single)
wire[15:0]	regValRm_D2H;		//memory data (Double to Half)
FpuConvD2S	mem_cnv_d2s(regValRm[63:0], regValRm_D2S);
FpuConvD2H	mem_cnv_d2h(regValRm[63:0], regValRm_D2H);
`endif

`ifdef jx2_enable_pmov
wire[31:0]	regValRm_S2H;		//memory data (Single to Half)
wire[31:0]	regValRm_H2M8;		//memory data (Half to FP8)

ExConv_Fp16PckAl	mem_cnv_h2m8a(regValRm[15: 0], regValRm_H2M8[ 7: 0], 1);
ExConv_Fp16PckAl	mem_cnv_h2m8b(regValRm[31:16], regValRm_H2M8[15: 8], 1);
ExConv_Fp16PckAl	mem_cnv_h2m8c(regValRm[47:32], regValRm_H2M8[23:16], 1);
ExConv_Fp16PckAl	mem_cnv_h2m8d(regValRm[63:48], regValRm_H2M8[31:24], 1);

assign	regValRm_S2H = 0;
`endif

reg			tSrJcmpZT;
reg			tSrJcmpZT_ZL;
reg			tSrJcmpZT_ZQ;
reg			tSrJcmpZT_SL;
reg			tSrJcmpZT_SQ;

reg[7:0]	tRegValBPcLo;

always @*
begin

	tRegIdRn1	= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1	= UVGPRV_00;		//Destination Value (EX1)
//	tRegIdCn1	= JX2_CR_ZZR[4:0];	//Destination ID (CR, EX1)
	tRegIdCn1	= JX2_CR_ZZR;		//Destination ID (CR, EX1)
	tRegValCn1	= UVGPRV_00;		//Destination Value (CR, EX1)
	
	tHeldIdRn1	= JX2_GR_ZZR;
//	tHeldIdCn1	= JX2_CR_ZZR[4:0];
	tHeldIdCn1	= JX2_CR_ZZR;

	tRegOutDlr	= regInDlr;
	tRegOutDhr	= regInDhr;
	tRegOutSp	= regInSp;
	tRegOutLr	= regInLr;
	tRegOutSr	= regInSr;
	tRegOutSchm	= regInSchm;

	tNxtCanaryMagic		= tCanaryMagic;

`ifdef jx2_enable_vaddr96
	tRegOutPcHi		= regValPcHi;
	tRegOutLrHi		= regValLrHi;
	tRegOutGbrHi	= regValGbrHi;
`endif

//	tXmovEnable		= regInSr[25] || regInSr[30];
//	tXmovAdd		= regInSr[24];

`ifdef jx2_enable_vaddr96
//	tMemAddr		= { regValXs[47:0], tValAgu };
	tMemAddr		= { regValGbrHi[47:0], tValAgu };
	
//	if(regIdRs == JX2_GR_PC)
//		tMemAddr[95:48] = regValPcHi[47:0];
	
//	if(opUIxt[7:6] == 2'b11)
	if(	((opUIxt[8:6]==JX2_IUC_WA) ||
		(opUIxt[8:6]==JX2_IUC_WXA)) && tXmovEnable)
	begin
		tMemAddr[95:48] = regValXs[47:0];
	end

`ifdef jx2_enable_vaddr96qadd
//	else if(tXmovAdd)
	else if(tXmovAdd && regValRs[63:60]==4'b1111)
	begin
//		tMemAddr[63:48] = tMemAddr[63:48] + regValRs[63:48];
		tMemAddr[   47] = 0;
		tMemAddr[63:48] = regValGbrHi[15:0] + { 3'h0, regValRs[59:47] };
	end
`endif

`ifdef jx2_enable_vaddr96q64
	else if(tXmovAdd)
	begin
//		tMemAddr[63:48] = regValRs[59:44];
//		tMemAddr[47:44] = regValRs[63:60];

//		tMemAddr[46:44] = regValRs[   63] ? regValRs[62:60] : regValRs[46:44];
//		tMemAddr[   47] = regValRs[   63];
//		tMemAddr[50:48] = regValRs[   63] ? regValRs[46:44] : regValRs[62:60];
//		tMemAddr[63:51] = regValRs[59:47];

`ifndef def_true
//		tMemAddr[46:44] = tAguXLeaTag[15] ?
//			tAguXLeaTag[14:12] :     tValAgu[46:44];
//		tMemAddr[   47] = tAguXLeaTag[15];
//		tMemAddr[50:48] = tAguXLeaTag[15] ?
//			    tValAgu[46:44] : tAguXLeaTag[14:12];
//		tMemAddr[   51] = tValAgu    [47];
//		tMemAddr[63:52] = tAguXLeaTag[11:0];
`endif

`ifdef def_true
		tMemAddr[47:44] = tValAgu[47:44];
		tMemAddr[63:48] = tAguXLeaTag[15:0];
`endif
	
	end
`endif

`else
`ifdef jx2_enable_vaddr48
	tMemAddr		= tValAgu;
`else
	tMemAddr		= tValAgu[31:0];
`endif
`endif

	tMemOpm			= UMEM_OPM_READY;
	tMemLdOp		= 0;
	tMemDataOut			= 0;
	tMemDataOutB		= 0;
	tMemDataOut[63:0]	= regValRm[63:0];
	tMemDataOutB[63:0]	= regValRt[63:0];
`ifdef jx2_enable_memcap
	tMemDataOut[65:64]	= regValRm[65:64];
`endif

`ifdef jx2_use_mem_ldop
	tMemLdOp		= { 1'b0, opUExtFl[3:0] };
//	if(opUExtFl[3])
//	begin
//		tMemDataOut		= { 58'h00, regIdRm[5:0] };
//	end
`endif
	
	tDoMemOpm		= UMEM_OPM_READY;
	tDoMemOp		= 0;
	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tExTrapExc		= 0;
	tDoDelayCycle	= 0;

	tValOutDfl		= UVGPRV_00;
	tDoOutDfl		= 0;
	tExDelayOut		= 0;
	tSrJcmpZT		= 0;

//	tValAguBra		= { UV16_00, regValPc[47:32], tValAgu[31:0] };
//	tValAguBra		= { UV16_00, tValAgu };

	tValBraBasePc	= regValPc[47:0];

`ifdef jx2_enable_riscv
	if(tBraIsRiscv)
	begin
		/* RISC-V Branches relative to base PC. */
		tValBraBasePc	= regValBPc[47:0];
	end
`endif


//	tValAguBraJCmpMi = tValBraBasePc[31:16] + 1;
	tValAguBraJCmpMi0 = tValBraBasePc[31:16] + regValImmJCmp[30:15] + 0;
	tValAguBraJCmpMi1 = tValBraBasePc[31:16] + regValImmJCmp[30:15] + 1;
	tValAguBraJCmpLo = { 1'b0, tValBraBasePc[15:1] } +
		{ 1'b0, regValImmJCmp[14:0] };
	
	tValAguBraJCmp = {
		regValPc[47:32],
//		tValAguBraJCmpLo[15] ? tValAguBraJCmpMi : regValPc[31:16],
		tValAguBraJCmpLo[15] ? tValAguBraJCmpMi1 : tValAguBraJCmpMi0,
		tValAguBraJCmpLo[14:0],
		1'b0 };

	tValAguBra		= { UV16_00, tValAgu[47:0] };
	if(!tAguFlagJq)
		tValAguBra[47:32] = regValPc[47:32];

//	tValAguBraJCmp = tValAguBra[47:0];

//	tValAguBra		= { UV16_00, regValPc[47:32],
//		regValPc[31:0] + { regValRt[30:0], 1'b0 } };

`ifdef jx2_agu_bra32

	tValBraDispSc	= { regValRt[46:0], 1'b0 };
	tValAguBraA0	=
		{ 1'b0, regValPc[15:0] } +
		{ 1'b0, tValBraDispSc[15:0] };
	tValAguBraB0	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 0;
	tValAguBraB1	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 1;

	tValAguBra		= {
		UV16_00,
		regValPc[47:32],
		tValAguBraA0[16] ?
			tValAguBraB1[15:0] : tValAguBraB0[15:0],
		tValAguBraA0[15:0]
		};
`endif

`ifdef jx2_agu_bra48

	tValBraDispSc	= { regValRt[46:0], 1'b0 };
	tValAguBraA0	=
		{ 1'b0, tValBraBasePc[15:0] } +
		{ 1'b0, tValBraDispSc[15:0] };
	tValAguBraB0	=
		{ 1'b0, tValBraBasePc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 0;
	tValAguBraB1	=
		{ 1'b0, tValBraBasePc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 1;
	tValAguBraC0	=
		{ 1'b0, tValBraBasePc[47:32] } +
		{ 1'b0, tValBraDispSc[47:32] } + 0;
	tValAguBraC1	=
		{ 1'b0, tValBraBasePc[47:32] } +
		{ 1'b0, tValBraDispSc[47:32] } + 1;
	tValAguBra		= {
		UV16_00,
		( tValAguBraA0[16] ? tValAguBraB1[16] : tValAguBraB0[16] ) ?
			tValAguBraC1[15:0] : tValAguBraC0[15:0],
		tValAguBraA0[16] ?
			tValAguBraB1[15:0] : tValAguBraB0[15:0],
		tValAguBraA0[15:0]
		};
	if(!tAguFlagJq)
		tValAguBra[47:32] = regValPc[47:32];
`endif

//	tRegBraLr	= {
//		regInSr[15: 4],
//		regInSr[27:26],
//		regInSr[ 1: 0],
//		regValPc };
//	tRegBraLr	= regValPc;
	tRegBraLr	= { regValPc[63:1], 1'b1 };

//	tValBra			= tValAguBra[47:0];
//	tValBra			= { tRegBraLr[63:48], tValAguBra[47:0] };
	tValBra			= { regValPc[63:48], tValAguBra[47:0] };
`ifdef jx2_enable_vaddr96
	tValBraHi		= { UV16_00, regValPcHi[47:0] };
`else
	tValBraHi		= 0;
`endif
	tDoBra			= 0;

`ifdef jx2_enable_vaddr96
// `ifndef def_true
	if(	((opUIxt[8:6]==JX2_IUC_WA) ||
		(opUIxt[8:6]==JX2_IUC_WXA)) &&
		tXmovEnable)
	begin
		tValBraHi[47:0] = regValXs[47:0];
	end
`endif

	tTempBit0		= 1'bX;
	tTempBit1		= 1'bX;
	tTempBit2		= 1'bX;
	tTempBit3		= 1'bX;

	tIfThenPP		= regInSr[11: 8] + 1;
	tIfThenNN		= regInSr[11: 8] - 1;
	tIfElsePP		= regInSr[15:12] + 1;
	tIfElseNN		= regInSr[15:12] - 1;

//	tRegSpAdd8		= { regInSp[63:28], regInSp[27:3]+25'h1, regInSp[2:0]};
//	tRegSpSub8		= { regInSp[63:28], regInSp[27:3]-25'h1, regInSp[2:0]};
//	tRegSpAdd16		= { regInSp[63:28], regInSp[27:4]+24'h1, regInSp[3:0]};
//	tRegSpSub16		= { regInSp[63:28], regInSp[27:4]-24'h1, regInSp[3:0]};

`ifdef jx2_enable_pred_s
	casez( { opBraFlush, opUCmd[8:6], regInSr[1:0] } )
		6'b0000zz: 	tOpEnable = 1;
		6'b0001zz: 	tOpEnable = 0;
		6'b0010z0: 	tOpEnable = 0;
		6'b0010z1: 	tOpEnable = 1;
		6'b0011z0: 	tOpEnable = 1;
		6'b0011z1: 	tOpEnable = 0;
		6'b01000z: 	tOpEnable = 0;
		6'b01001z: 	tOpEnable = 1;
		6'b01010z: 	tOpEnable = 1;
		6'b01011z: 	tOpEnable = 0;
		6'b0110zz: 	tOpEnable = 1;
		6'b0111zz: 	tOpEnable = 1;
		6'b1zzzzz: 	tOpEnable = 0;
	endcase
`else
	casez( { opBraFlush, opUCmd[7:6], regInSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif

`ifdef jx2_cpu_pred_id2
	tOpEnableB = !opBraFlush && !opPredNoExec;
	
	if((tOpEnableB!=tOpEnable) && (opUCmd[5:0]!=JX2_UCMD_NOP))
	begin
		$display("ExEX1: Pred Mismatch %d %d",
			tOpEnableB,
			tOpEnable);
	end
	else
	begin
//		$display("ExEX1: Pred Match %d",
//			tOpEnableB);
	end
	
	tOpEnable	= tOpEnableB;
`endif

	tOpUCmdF	= ((opUCmd[5:0] == JX2_UCMD_BRA) && !opBraFlush) ?
		JX2_UCMD_BRA_NB : JX2_UCMD_NOP ;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : tOpUCmdF;
	
	tOpUCmd2	= { JX2_IXC_AL, tOpUCmd1 };
	tCanaryRef	= regValRt[15:0] ^ tCanaryMagic;

`ifdef jx2_alu_jcmpz
	tSrJcmpZT_ZL = (regValRs[31: 0] == 0);
	tSrJcmpZT_ZQ = (regValRs[63:32] == 0) && tSrJcmpZT_ZL;
	tSrJcmpZT_SL = regValRs[31];
	tSrJcmpZT_SQ = regValRs[63];

	case(opUIxt[3:0])
		4'h0:		tSrJcmpZT = tSrJcmpZT_ZL;					/* EQ */
		4'h1:		tSrJcmpZT = !tSrJcmpZT_ZL;					/* NE */
		4'h2:		tSrJcmpZT = tSrJcmpZT_ZL || tSrJcmpZT_SL;	/* LE */
		4'h3:		tSrJcmpZT = !tSrJcmpZT_ZL && !tSrJcmpZT_SL;	/* GT */
		4'h4:		tSrJcmpZT = tSrJcmpZT_SL;					/* LT */
		4'h5:		tSrJcmpZT = !tSrJcmpZT_SL;					/* GE */

		4'h8:		tSrJcmpZT = tSrJcmpZT_ZQ;					/* EQ */
		4'h9:		tSrJcmpZT = !tSrJcmpZT_ZQ;					/* NE */
		4'hA:		tSrJcmpZT = tSrJcmpZT_ZQ || tSrJcmpZT_SQ;	/* LE */
		4'hB:		tSrJcmpZT = !tSrJcmpZT_ZQ && !tSrJcmpZT_SQ;	/* GT */
		4'hC:		tSrJcmpZT = tSrJcmpZT_SQ;					/* LT */
		4'hD:		tSrJcmpZT = !tSrJcmpZT_SQ;					/* GE */

		default:	tSrJcmpZT = 0;					/* - */
	endcase
`endif

`ifndef def_true
// `ifdef def_true
	if(tBraIsRiscv && (tRegValBPcLo!=regValBPc[7:0]) && tOpEnable)
	begin
		$display(" %X %X %X/%X-%X s=%X:%X t=%X:%X",
			regValBPc[31:0], opIstrWord[31:0], opUCmd, tOpUCmd1, opUIxt,
			regIdRs, regValRs, regIdRt, regValRt);
		$display("   n=%X:%X    %b-%b-%b-%b-%b-%b-%b",
			regIdRm, regValRm,
			opIstrWord[31:25], opIstrWord[24:20],
			opIstrWord[19:15], opIstrWord[14:12],
			opIstrWord[11: 7], opIstrWord[ 6: 5],
			opIstrWord[ 4: 0]);
	end
`endif

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_INVOP: begin
			if(!tMsgLatch)
				$display("EX1: Invalid Opcode %X-%X, IsRV=%d PC=%X",
					tOpUCmd1, opUIxt, tBraIsRiscv, regValBPc);
			tNextMsgLatch	= 1;

//			tExTrapExc = {
//				tValBraHi,
//				regValBPc,
//				16'h800E };

//			tExTrapExc = { UV112_00, 16'h800E };
			tExTrapExc = { UV96_00, regValBPc[15:0], 16'h800E };

			if(opUIxt[5:0]==JX2_UCIX_INVOP_PRIVFAULT)
				tExTrapExc[7:0]=8'h11;
			if(opUIxt[5:0]==JX2_UCIX_INVOP_FPUFAULT)
				tExTrapExc[7:0]=8'h12;

//			tExHold		= 1;
//			tExHold		= !reset;
		end
	
		JX2_UCMD_LEA_MR: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= { UV16_00, tValAgu };
//			tValOutDfl		= { UV16_00, tValAgu };

`ifndef def_true
			if(	(tValAgu[31:24]==0) ||
				((tValAgu[31:28]!=0) && (tValAgu[31:28]!=4'hF)) ||
				((tValAgu[47:32]!=0) && (tValAgu[47:44]!=4'hF)))
			begin
				$display("LEA: %X", { tAguXLeaTag, tValAgu });
			end
`endif

`ifdef jx2_cpu_lea_ex2
			tRegHeld		= 1;
`else
//			tValOutDfl		= { tAguXLeaTag, tValAgu };
			tValOutDfl		= regOutLea;
			tDoOutDfl		= 1;
`endif

		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;

`ifndef def_true
			if(	(tValAgu[31:24]==0) ||
				((tValAgu[31:28]!=0) && (tValAgu[31:28]!=4'hF)) ||
				((tValAgu[47:32]!=0) && (tValAgu[47:44]!=4'hF)))
			begin
				$display("STORE(1): A=%X R=%X V=%X",
					tMemAddr, regIdRm, tMemDataOut);
			end
`endif

`ifdef jx2_debug_ldst
			$display("STORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;

`ifdef jx2_use_mem_ldop
			if(opUExtFl[3:0]==4'h1)
			begin
				tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			end
`endif

`ifdef jx2_debug_ldst
			$display("LOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif

`ifndef def_true
			if(	(tValAgu[31:24]==0) ||
				((tValAgu[31:28]!=0) && (tValAgu[31:28]!=4'hF)) ||
				((tValAgu[47:32]!=0) && (tValAgu[47:44]!=4'hF)))
			begin
				$display("LOAD(1): A=%X R=%X",
					tMemAddr, regIdRm);
			end
`endif
		end

`ifdef jx2_enable_fmov
		JX2_UCMD_FMOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
			
			tMemDataOut[31:0] = regValRm_D2S;
`ifdef jx2_enable_fmovh
//			if(opUIxt[4])
			if(opUIxt[5:4]==2'b01)
				tMemDataOut[15:0] = regValRm_D2H;
`endif

`ifdef jx2_enable_ldst48a
			if(opUIxt[2])
			begin
				tDoMemOpm	= { 2'b10, 3'b110 };
				if(opUIxt[5:4]==2'b00)
					tMemDataOut[47:0] = regValRm[47: 0];
				if(opUIxt[5:4]==2'b01)
					tMemDataOut[47:0] = regValRm[47: 0];

				if(opUIxt[5:4]==2'b10)
					tMemDataOut[47:0] = regValRm[63:16];
			end
`endif

`ifdef jx2_debug_ldst
			$display("FSTORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;

`ifdef jx2_enable_ldst48a
			if(opUIxt[2])
			begin
				tDoMemOpm	= { 2'b10, 3'b011 };
			end
`endif

`ifdef jx2_debug_ldst
			$display("FLOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end
`endif

`ifdef jx2_enable_pmov
		JX2_UCMD_PMOV_MR: begin
			/* Always fetch 64 bits.
			 * EX3 will ignore what it doesn't need.
			 */
			tDoMemOpm = { 2'b01, 3'b011 };
			tDoMemOp	= 1;
			tRegHeld		= 1;

`ifdef jx2_debug_ldst
			$display("PLOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end

		JX2_UCMD_PMOV_RM: begin
//			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOpm	= { 2'b10, 3'b010 };	//Store DWORD
			tDoMemOp	= 1;
			
			tMemDataOut[31:0] = regValRm_H2M8;
			if(opUIxt[5:4]==2'b01)
				tMemDataOut[15:0] = regValRm_S2H;

`ifdef jx2_debug_ldst
			$display("FSTORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
`endif

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3,
		JX2_UCMD_CONV2_RR, JX2_UCMD_CONV3_RR, JX2_UCMD_ALUB3: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;

`ifdef jx2_ena_alufast
			if(tFastAluOK)
			begin
				tValOutDfl		= tValFastAlu;
				tDoOutDfl		= 1;
				tRegHeld		= 0;
			end
`endif

		end

		JX2_UCMD_ALUCMP, JX2_UCMD_ALUCMPW, JX2_UCMD_ALUCMPB: begin
		end
		
		JX2_UCMD_ALUCMP3R: begin
			tRegHeld		= 1;
		end
	
		JX2_UCMD_CONV_RR: begin
`ifdef jx2_cpu_conv_ex2
			tExDelayOut[63:0]	= tValCnv;
			tRegHeld			= 1;
`else
			tValOutDfl[63:0]	= tValCnv;
			tDoOutDfl			= 1;
//			tRegIdRn1			= regIdRm;
//			tRegValRn1[63:0]	= tValCnv;
//			tRegOutSr[0]		= tCnvSrT;
`endif
		end
`ifdef jx2_gprs_mergecm
		JX2_UCMD_MOV_RC, JX2_UCMD_MOV_CR: begin
			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;

`ifdef jx2_enable_memcap
			if(opUIxt[8:6]!=JX2_IUC_WX)
				tValOutDfl[65:64]	= 0;
`endif
		end
`else
		JX2_UCMD_MOV_RC: begin
			tRegIdCn1	= regIdRm;
			tRegValCn1	= regValRs;

			tRegIdRn1	= regIdRm;
			tRegValRn1	= regValRs;
		end
		JX2_UCMD_MOV_CR: begin
			tValOutDfl		= regValCRm;
			tDoOutDfl		= 1;
		end
`endif

		JX2_UCMD_MOV_IR: begin
			tValOutDfl		= regValRt;
			tDoOutDfl		= 1;

			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
				end
				4'b0001: begin /* LDISH8 */
					tValOutDfl[63:0]	= { regValRs[55:0], regValRt[7:0] };
				end
				4'b0010: begin /* LDISH16 */
					tValOutDfl[63:0]	= { regValRs[47:0], regValRt[15:0] };
				end
				4'b0011: begin /* LDISH32 */
					tValOutDfl[63:0]	= { regValRs[31:0], regValRt[31:0] };
				end
				4'b0100: begin /* JLDIX */
				end

				4'b0101: begin /* FLDCH */
					tValOutDfl[63:0]	= {
						regValRt[15:14],
						(regValRt[14] || (regValRt[14:10]==0)) &&
								(regValRt[14:10]!=5'h1F) ?
							6'h00 : 6'h3F,
						regValRt[13: 0],
						10'h0,
						32'h0
						};
				end
				
				default: begin
					$display("ExEX1: MOV_IR, Invalid UIxt %X", opUIxt);
				end
			endcase

`ifdef def_true
			tExDelayOut		= tValOutDfl;
			tRegHeld		= 1;

			tValOutDfl		= UVGPRV_00;
			tDoOutDfl		= 0;
`endif
		end

		JX2_UCMD_BRA_NB: begin
			if(opPreBra!=2'b00)
			begin
				tValBra		= regValPc[63:0];
				tValBra[63:48] = UV16_00;
				tDoBra		= 1;
			end
			
//			if(1'b1)
			if(!opBraFlush)
			begin
//				$display("EX1 Non-BRA_NB %X Miss=%X", opPreBra, tDoBra);
			end
		end
	
		JX2_UCMD_BRA: begin
//			tDoMemOpm	= 5'b01011;		//Hack, Dummy Load (TLB)
//			tDoMemOpm	= UMEM_OPM_RD_UW;	//Hack, Dummy Load (TLB)
//			tDoMemOp	= 1;

//			if(!opPreBra)
			if(opPreBra!=2'b01)
			begin
//				tValBra		= tValAguBra[47:0];
				tValBra		= { tRegBraLr[63:48], tValAguBra[47:0] };
				tValBra[63:48] = UV16_00;
				tDoBra		= 1;
			end

//			if(opPreBra[1])
//			if(1'b1)
			if(!opBraFlush)
			begin
//				$display("EX1 BRA Pre=%X Miss=%X", opPreBra, tDoBra);
			end
		end

		JX2_UCMD_BSR: begin
//			tDoMemOpm	= 5'b01011;		//Hack, Dummy Load (TLB)
//			tDoMemOpm	= UMEM_OPM_RD_UW;	//Hack, Dummy Load (TLB)
//			tDoMemOp	= 1;

// `ifndef jx2_enable_riscv
`ifndef def_true
// `ifdef def_true
//			$display("EX: BSR: LR=%X PC2=%X", regValPc, tValAgu);
//			tRegOutLr	= regValPc;
			tRegOutLr	= tRegBraLr;

`ifdef jx2_enable_vaddr96
//			tRegOutLrHi	= regValPcHi;
`endif

//			tRegOutLr	= {
//				regInSr[15: 4],
//				regInSr[27:26],
//				regInSr[ 1: 0],
//				regValPc };
`endif

//`ifdef jx2_enable_riscv
`ifdef def_true
			tValOutDfl[63:0]	= tRegBraLr;
			tDoOutDfl			= 1;

`ifndef def_true
//			if(regIdRm==JX2_GR_ZZR)
			if(regIdRm==JX2_GR_DLR)
			begin
				tRegOutLr	= tRegBraLr;
`ifdef jx2_enable_vaddr96
//				tRegOutLrHi	= regValPcHi;
`endif
				tDoOutDfl	= 0;
			end
`endif

`endif

//			$display("BSR: LR=%X", tRegBraLr);

//			if(!opPreBra)
			if(opPreBra!=2'b01)
			begin
//				tValBra		= tValAguBra[47:0];
//				tValBra		= { tRegBraLr[63:48], tValAguBra[47:0] };
				tValBra		= { regValPc[63:48], tValAguBra[47:0] };
				tDoBra		= 1;
			end

			if(opPreBra[1])
			begin
//				$display("EX1 Non-BSR %X", opPreBra);
			end
		end

		JX2_UCMD_JMP: begin
//			tDoMemOpm	= 5'b01011;		//Hack, Dummy Load (TLB)
//			tDoMemOpm	= UMEM_OPM_RD_UW;	//Hack, Dummy Load (TLB)
//			tDoMemOp	= 1;

//			tValBra		= regValRs[47:0];
//			tValBra		= { tRegBraLr[63:48], regValRs[47:0] };
			tValBra		= { regValPc[63:48], regValRs[47:0] };
//			tValBra		= regValRs[47:0];
//			tValBra		= regValRs;
//			tDoBra		= 1;
//			tDoBra		= !opPreBra;
			tDoBra		= (opPreBra != 2'b01);

//			$display("JMP: Reg=%X", regValRs);

			if((regValPc[27:16]!=0) && (regValRs[27:16]==0))
			begin
				if(!tMsgLatch)
				begin
					$display("JMP: Reg=%X %X", regValRs, tValAgu);
				end
				tNextMsgLatch = 1;
				tExHold		= 1;
			end

// `ifndef jx2_enable_riscv
// `ifdef def_true
`ifndef def_true
			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
//				tValBra[63:48] = regValRs[63:48];
			end
`endif

`ifndef def_true
//			if(regIdRs==JX2_GR_LR)
			if(1'b1)
			begin
				tRegOutSr[ 1: 0] = regValRs[49:48];
				tRegOutSr[27:26] = regValRs[51:50];
				tRegOutSr[15: 4] = regValRs[63:52];
			end
`endif

// `ifdef jx2_enable_riscv
// `ifndef def_true
`ifdef def_true
			tValBra		= { regValPc[63:48], tValAgu[47:0] };

			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
//				tValBra[63:48] = regValRs[63:48];
			end
			tValBra[63:48] = UV16_00;

//			if(regInSr[26])
//			begin
//				$display("EX: JMP: RVI PC=%X", tValBra);
//			end

			if(opUIxt[2])
			begin
				/* Implicit Mode Switch */
//				tRegOutSr[27]	= opUIxt[3];	//WXE
//				tRegOutSr[26]	= opUIxt[4];	//WX2
//				tRegOutSr[23]	= opUIxt[5];	//WX3

				tRegOutSr[26]	= opUIxt[3];	//WX2
				tRegOutSr[23]	= opUIxt[4];	//WX3
				tRegOutSr[22]	= opUIxt[5];	//WX4
			end

			if(tValAgu[0])
			begin
`ifdef jx2_debug_isr
//				if(regInSr[26]!=regValRs[50])
				if(	(regInSr[26]!=regValRs[50]) ||
					(regInSr[23:22]!=regValRs[55:54]))
				begin
					$display("EX: JMP: Inter-ISA RV=%d Ixt=%X Rs=%X Tgt-PC=%X",
						regInSr[26], opUIxt,
						regValRs, tValBra);
				end
`endif

				tRegOutSr[26]	= !regInSr[26];
//				tRegOutSr[27]	= tValAgu[1];
				tRegOutSr[27]	= 0;
//				if(tValAgu[1])
				if(1'b1)
				begin
					tRegOutSr[27:26]	= regValRs[51:50];
//					tRegOutSr[23:20]	= regValRs[63:60];
					tRegOutSr[23:20]	= regValRs[55:52];
				end
//				tValBra[1:0]	= 0;
				tValBra[0]	= 0;
			end
`endif
		end
		JX2_UCMD_JSR: begin
//			tDoMemOpm	= 5'b01011;		//Hack, Dummy Load (TLB)
//			tDoMemOpm	= UMEM_OPM_RD_UW;	//Hack, Dummy Load (TLB)
//			tDoMemOp	= 1;

//			$display("EX: JSR: LR=%X PC2=%X", regValRs, regValPc);

`ifdef def_true
			if((regValPc[27:16]!=0) && (regValRs[27:16]==0))
			begin
				if(!tMsgLatch)
				begin
					$display("JSR: Reg=%X %X", regValRs, tValAgu);
				end
				tNextMsgLatch = 1;
				tExHold		= 1;
			end
`endif

// `ifndef jx2_enable_riscv
`ifndef def_true
// `ifdef def_true
//			tRegOutLr	= regValPc;
			tRegOutLr	= tRegBraLr;
`ifdef jx2_enable_vaddr96
//			tRegOutLrHi	= regValPcHi;
`endif
//			tRegOutLr	= {
//				regInSr[15: 4],
//				regInSr[27:26],
//				regInSr[ 1: 0],
//				regValPc };
//			tValBra		= regValRs[47:0];
//			tValBra		= regValRs;
			tValBra		= { regValPc[63:48], regValRs[47:0] };
			tDoBra		= 1;
`endif

// `ifdef jx2_enable_riscv
`ifdef def_true
// `ifndef def_true
			tValBra		= { regValPc[63:48], tValAgu[47:0] };
			tValOutDfl[63:0]	= tRegBraLr;
			tDoOutDfl			= 1;
			tDoBra				= 1;

`ifndef def_true
//			if(regIdRm==JX2_GR_ZZR)
			if(regIdRm==JX2_GR_DLR)
			begin
				tRegOutLr	= tRegBraLr;
`ifdef jx2_enable_vaddr96
//				tRegOutLrHi	= regValPcHi;
`endif
				tDoOutDfl	= 0;
			end
`endif

			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
//				tValBra[63:48] = regValRs[63:48];
			end
			tValBra[63:48] = UV16_00;

			if(opUIxt[2])
			begin
				/* Implicit Mode Switch */
//				tRegOutSr[27]	= opUIxt[3];
//				tRegOutSr[26]	= opUIxt[4];
//				tRegOutSr[23]	= opUIxt[5];

				tRegOutSr[26]	= opUIxt[3];	//WX2
				tRegOutSr[23]	= opUIxt[4];	//WX3
				tRegOutSr[22]	= opUIxt[5];	//WX4
			end

			if(tValAgu[0])
			begin
`ifdef jx2_debug_isr
//				if(regInSr[26]!=regValRs[50])
				if(	(regInSr[26]!=regValRs[50]) ||
					(regInSr[23:22]!=regValRs[55:54]))
				begin
					$display("EX: JSR: Inter-ISA Md=%X-%X -> %X-%X PC=%X",
						regInSr[27:26], regInSr[23:22],
						regValRs[51:50], regValRs[55:54],
						tValBra);
				end
`endif

				tRegOutSr[26]	= !regInSr[26];
//				tRegOutSr[27]	= tValAgu[1];
				tRegOutSr[27]	= 0;
//				if(tValAgu[1])
				if(1'b1)
				begin
					tRegOutSr[27:26]	= regValRs[51:50];
//					tRegOutSr[23:20]	= regValRs[63:60];
					tRegOutSr[23:20]	= regValRs[55:52];
				end
//				tValBra[1:0]	= 0;
				tValBra[0]	= 0;
			end
`endif
		end

`ifdef jx2_alu_jcmp
		JX2_UCMD_JCMP: begin
//			$display("JCMP, PC0=%X, PC1=%X, Imm=%X T=%d  Rs=%X Rt=%X Ixt=%X",
//				regValPc, tValAguBraJCmp, regValImm, aluSrJcmpT,
//				regValRs, regValRt, opUIxt);
			if(aluSrJcmpT)
			begin
//				tValBra		= { tRegBraLr[63:48], tValAguBraJCmp[47:0] };
				tValBra		= { regValPc[63:48], tValAguBraJCmp[47:0] };
//				tDoBra		= 1;
				tDoBra		= (opPreBra != 2'b01);
			end
			else
			begin
				tValBra		= regValPc[63:0];
				tDoBra		= (opPreBra != 2'b00);
			end
		end
`endif

`ifdef jx2_alu_jcmpz
		JX2_UCMD_JCMPZ: begin
			if(tSrJcmpZT)
			begin
				tValBra		= { regValPc[63:48], tValAguBraJCmp[47:0] };
				tDoBra		= (opPreBra != 2'b01);
			end
			else
			begin
				tValBra		= regValPc[63:0];
				tDoBra		= (opPreBra != 2'b00);
			end
		end
`endif

		JX2_UCMD_MULW3: begin
//			tHeldIdRn1	= regIdRm;			//
			tRegHeld		= 1;
		end

`ifdef jx2_merge_shadq
//		JX2_UCMD_SHAD3, JX2_UCMD_SHLD3,
//		JX2_UCMD_SHADQ3, JX2_UCMD_SHLDQ3:
		JX2_UCMD_SHAD3:
		begin
`ifdef jx2_cpu_shad_ex2
			tExDelayOut[63:0]	= tValShad64;
			tRegHeld			= 1;
`else
//			tRegIdRn1			= regIdRm;
//			tRegValRn1[63:0]	= tValShad64;
			tValOutDfl[63:0]	= tValShad64;
			tDoOutDfl			= 1;
`endif
		end
`else
		JX2_UCMD_SHAD3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1[63:0]= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
			tValOutDfl[63:0]= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLD3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1[63:0]	= { UV32_00, tValShad32 };
			tValOutDfl[63:0]		= { UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
	
		JX2_UCMD_SHADQ3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1[63:0]	= tValShad64;
			tValOutDfl[63:0]		= tValShad64;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLDQ3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1[63:0]	= tValShad64;
			tValOutDfl[63:0]	= tValShad64;
			tDoOutDfl		= 1;
		end

`endif

		JX2_UCMD_MUL3: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

`ifdef jx2_alu_slomuldiv
		JX2_UCMD_QMULDIV: begin
			tRegHeld		= 1;
		end
`endif
	
`ifdef jx2_enable_fpu
		JX2_UCMD_FSTCX: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
		
		JX2_UCMD_FCMP: begin
			if(	(opUIxt[3:0]==JX2_UCIX_FCMP_CMPEQ_R[3:0]) ||
				(opUIxt[3:0]==JX2_UCIX_FCMP_CMPGT_R[3:0]) ||
				(opUIxt[3:0]==JX2_UCIX_FCMP_CMPGE_R[3:0]) )
//			if(1'b1)
			begin
				tRegHeld		= 1;
			end
		end

		JX2_UCMD_FPU3: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
		JX2_UCMD_FIXS: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

		JX2_UCMD_FLDCX: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
		
		JX2_UCMD_FPUV4SF: begin
			tRegHeld		= 1;
		end
`endif

		JX2_UCMD_BLINT: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

		JX2_UCMD_OP_IXS: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXS_NOP: begin
				end
				JX2_UCIX_IXS_MOVT: begin
//					tRegIdRn1			= regIdRm;
//					tRegValRn1[63:0]	= {UV63_00, regInSr[0]};
//					tValOutDfl[63:0]	= {UV63_00, regInSr[0]};
					tValOutDfl[63:0]	= {UV63_00, regRefSr[0]};
					tDoOutDfl			= 1;
				end
				JX2_UCIX_IXS_MOVNT: begin
//					tRegIdRn1			= regIdRm;
//					tRegValRn1[63:0]	= {UV63_00, !regInSr[0]};
//					tValOutDfl[63:0]	= {UV63_00, !regInSr[0]};
					tValOutDfl[63:0]	= {UV63_00, !regRefSr[0]};
					tDoOutDfl			= 1;
				end

				JX2_UCIX_IXS_MOVST: begin
//					tValOutDfl[63:0]	= {UV62_00, regInSr[1:0]};
					tValOutDfl[63:0]	= {UV62_00, regRefSr[1:0]};
					tDoOutDfl			= 1;
				end
				JX2_UCIX_IXS_MOVPQ: begin
//					tValOutDfl[63:0]	= {UV60_00, regInSr[7:4]};
					tValOutDfl[63:0]	= {UV60_00, regRefSr[7:4]};
					tDoOutDfl			= 1;
				end

				JX2_UCIX_IXS_INVIC: begin
`ifdef jx2_debug_isr
					$display("EX1 JX2_UCIX_IXS_INVIC");
`endif

// `ifdef jx2_l1i_nohash
`ifndef def_true
					if(	(	(regValRm[31:28]!=4'hF) &&
							(regValRm[31:24]!=8'h00)) ||
						(	(regValRm[47:32]!=16'h0000) &&
							(regValRm[47:32]!=16'hFFFF)))
					begin
//						tMemOpm		= UMEM_OPM_FLUSHIS;
//						tMemAddr	= regValRm[47:0];
//						tMemAddr	= { 16'h0000, 16'h0003, regValRm[15:0] };
						tMemAddr	= { 16'hC000, 16'h0003, regValRm[15:0] };
						tRegOutLr	= tRegBraLr;
`ifdef jx2_enable_vaddr96
//						tRegOutLrHi	= regValPcHi;
`endif
						tValBra		= { regValPc[63:48], tMemAddr };
						tDoBra		= 1;
					end
					else
					begin
						tMemOpm		= UMEM_OPM_FLUSHIS;
						tMemAddr	= regValRm[47:0];

	//					tValBra		= regValPc[47:0];
						tValBra		= regValPc;
	//					tValBra		= { tRegBraLr[63:48], regValPc[47:0] };
						tDoBra		= 1;
					end
`else
					tMemOpm			= UMEM_OPM_FLUSHIS;
//					tMemAddr		= regValRm[47:0];
`ifdef jx2_enable_vaddr48
					tMemAddr[47:0]	= regValRm[47:0];
`else
					tMemAddr[31:0]	= regValRm[31:0];
`endif

//					tValBra		= regValPc[47:0];
					tValBra		= regValPc;
//					tValBra		= { tRegBraLr[63:48], regValPc[47:0] };
					tDoBra		= 1;
`endif

				end
				JX2_UCIX_IXS_INVDC: begin
`ifdef jx2_debug_isr
					$display("EX1 JX2_UCIX_IXS_INVDC");
`endif

// `ifdef jx2_l1d_nohash
`ifndef def_true
					if(	(	(regValRm[31:28]!=4'hF) &&
							(regValRm[31:24]!=8'h00)) ||
						(	(regValRm[47:32]!=16'h0000) &&
							(regValRm[47:32]!=16'hFFFF)))
					begin
						tMemOpm		= UMEM_OPM_RD_Q;
//						tMemAddr	= { 16'h0000, 16'h0001, regValRm[15:0] };
						tMemAddr	= { 16'hC000, 16'h0001, regValRm[15:0] };
					end
					else
					begin
						tMemOpm		= UMEM_OPM_FLUSHDS;
						tMemAddr	= regValRm[47:0];
					end
`else
					tMemOpm			= UMEM_OPM_FLUSHDS;
//					tMemAddr		= regValRm[47:0];
`ifdef jx2_enable_vaddr48
					tMemAddr[47:0]	= regValRm[47:0];
`else
					tMemAddr[31:0]	= regValRm[31:0];
`endif
`endif
				end

				JX2_UCIX_IXS_TRAPB: begin
					tDoMemOpm	= UMEM_OPM_TRAP;
					tDoMemOp	= 1;
				end
				
`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
				JX2_UCIX_IXS_BLKUTX1: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_BLKUTX2: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end

`ifdef jx2_enable_btcutx3
				JX2_UCIX_IXS_BLKUTX3H: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_BLKUTX3L: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
`endif

`endif
`endif

				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXS %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
				end
			endcase
		end

		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end
				JX2_UCIX_IXT_SLEEP: begin
				end
				JX2_UCIX_IXT_BREAK: begin
					if(!tMsgLatch)
						$display("EX: BREAK, PC=%X Imm=%X LR=%X",
							regValPc, regValImm, regInLr);
					tNextMsgLatch	= 1;
					tExHold		= 1;
					if(regInExc[15])
						tExHold		= 0;
					
				end
				JX2_UCIX_IXT_CLRT: begin
					tRegOutSr[0]	= 0;
				end
				JX2_UCIX_IXT_SETT: begin
					tRegOutSr[0]	= 1;
				end
				JX2_UCIX_IXT_CLRS: begin
					tRegOutSr[1]	= 0;
				end
				JX2_UCIX_IXT_SETS: begin
					tRegOutSr[1]	= 1;
				end

				JX2_UCIX_IXT_NOTT: begin
//					tRegOutSr[0]	= !regInSr[0];
					tRegOutSr[0]	= !regRefSr[0];
				end
				JX2_UCIX_IXT_NOTS: begin
//					tRegOutSr[1]	= !regInSr[1];
					tRegOutSr[1]	= !regRefSr[1];
				end

				JX2_UCIX_IXT_RTE: begin
//					$display("EX1: RTE, PC=%X", regValPc);
					tExTrapExc = { UV112_00, 16'hFF00 };
				end
				JX2_UCIX_IXT_TRAPA: begin
					tExTrapExc = { UV112_00, 12'hC08, regIdRm[3:0] };
				end

				JX2_UCIX_IXT_CPUID: begin
//					tRegIdRn1		= JX2_GR_DLR;
//					tRegValRn1		= tValCpuIdLo;

//					tValOutDfl[63:0]	= tValCpuIdLo;
//					tDoOutDfl			= 1;

					tExDelayOut[63:0]	= tValCpuIdLo;
					tRegHeld			= 1;

`ifndef def_true
					if(regIdRt[4:0]==5'h1E)
					begin
						if(!tNxtCanaryMagic[15] && regInSr[31])
						begin
							tNxtCanaryMagic = {1'b1, tValCpuIdRng[14:0]};
							$display("EX1: Init Canary Magic %X", 
								tNxtCanaryMagic);
						end
					end
`endif
				end

				JX2_UCIX_IXT_WEXMD: begin
`ifdef jx2_enable_wex
					case(regIdRm[4:0])
						5'h0:		tRegOutSr[27]	= 0;

						5'h1:		tRegOutSr[27]	= 1;

`ifdef jx2_enable_wex3w
						5'h2:		tRegOutSr[27]	= 1;
`endif

`ifdef jx2_mem_lane2
						5'h3:		tRegOutSr[27]	= 1;
`endif

`ifdef jx2_fpu_lane2
						5'h4:		tRegOutSr[27]	= 1;
`ifdef jx2_mem_lane2
						5'h5:		tRegOutSr[27]	= 1;
`endif
`endif

						5'h1F:
						begin
							tRegOutSr[27]	= 0;
//							if(!tNxtCanaryMagic[15] && regInSr[31])
							if(regInSr[31])
							begin
//								tNxtCanaryMagic = {1'b1, tValCpuIdRng[14:0]};
								tNxtCanaryMagic = tValCpuIdRng[15:0];
								$display("EX1: Init Canary Magic %X", 
									tNxtCanaryMagic);
							end
						end

						default:	tRegOutSr[27]	= 0;
					endcase
`else
					tRegOutSr[27]	= 0;
`endif
				end

				JX2_UCIX_IXT_SYSE: begin
`ifdef jx2_debug_isr
					$display("EX1: Do SYSE");
`endif
//					tExTrapExc = { UV112_00, 4'hE, regInDlr[11:0] };
					tExTrapExc = { UV112_00, 4'hE, regValRs[11:0] };

					if(regInSr[29] && regInSr[28])
					begin
						$display("EX1: SYSE in ISR");
					end
				end
				
				JX2_UCIX_IXT_LDTLB: begin
					tDoMemOpm	= UMEM_OPM_LDTLB;
					tDoMemOp	= 1;
				end
				JX2_UCIX_IXT_INVTLB: begin
					tDoMemOpm	= UMEM_OPM_INVTLB;
					tDoMemOp	= 1;
				end

				JX2_UCIX_IXT_LDACL: begin
					tDoMemOpm	= UMEM_OPM_LDACL;
					tDoMemOp	= 1;
				end

				JX2_UCIX_IXT_SVEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKEY: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEENC: begin
					tRegHeld		= 1;
				end

				JX2_UCIX_IXT_SXENTR: begin
					tRegOutSr[30:29]	= 2'b01;
					tValBra				= regValPc;
					tDoBra				= 1;
				end
				JX2_UCIX_IXT_SUENTR: begin
					tRegOutSr[30:29]	= 2'b00;
					tValBra				= regValPc;
					tDoBra				= 1;
				end
				JX2_UCIX_IXT_SVENTR: begin
					tRegOutSr[30:29]	= 2'b10;
					tValBra				= regValPc;
					tDoBra				= 1;
				end

				JX2_UCIX_IXT_VSKG: begin
//					tValOutDfl		= { UV48_00, tCanaryRef };
//					tDoOutDfl		= 1;

					tExDelayOut[63:0]	= { UV48_00, tCanaryRef };
					tRegHeld			= 1;
				end
				JX2_UCIX_IXT_VSKC: begin
					if(tCanaryRef != regValRs[15:0])
					begin
						tExTrapExc = { UV112_00, 16'h8010 };
					end
				end
				
				JX2_UCIX_IXT_BNDCHKB, JX2_UCIX_IXT_BNDCHKW,
				JX2_UCIX_IXT_BNDCHKL, JX2_UCIX_IXT_BNDCHKQ:
				begin
					if(tValBound8Oob)
					begin
						$display("ExEX1: Bound Check Fail, PC=%X Rs=%X Rt=%X",
							regValPc,
							regValRs,
							regValRt);
						tExTrapExc = { UV112_00, 16'h800F };
					end
				end

				JX2_UCIX_IXT_BNDCMPB, JX2_UCIX_IXT_BNDCMPW,
				JX2_UCIX_IXT_BNDCMPL, JX2_UCIX_IXT_BNDCMPQ:
				begin
					tRegOutSr[0]	= !tValBound8Oob;
				end

`ifdef jx2_enable_srtwid
				JX2_UCIX_IXT_SRTTWID: begin
//					tTempBit0	= regInSr[0] & regInSr[8];
					tTempBit0	= regRefSr[0] & regInSr[8];
//					tTempBit1	= regInSr[0] | regInSr[8];
					tTempBit1	= regRefSr[0] | regInSr[8];
					tIfThenPP	= regInSr[11: 8] + 1;
					tIfThenNN	= regInSr[11: 8] - 1;
					tIfElsePP	= regInSr[15:12] + 1;
					tIfElseNN	= regInSr[15:12] - 1;
					tTempBit2	= regInSr[15:12] == 0;
					tTempBit3	= regInSr[15:13] == 0;
								
					case(regIdRm[4:0])
						5'h00: begin	/* PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
//							tRegOutSr[   8]	= regInSr[   0];
							tRegOutSr[   8]	= regRefSr[   0];
						end
						5'h01: begin	/* POP */
							tRegOutSr[   15] = 0;
							tRegOutSr[14: 8] = regInSr[15:9];
							tRegOutSr[    0] = regInSr[   8];
						end
						5'h02: begin	/* POP2 */
							tRegOutSr[15:14] = 0;
							tRegOutSr[13: 8] = regInSr[15:10];
							tRegOutSr[    0] = regInSr[    9];
						end
						5'h03: begin	/* POP3 */
							tRegOutSr[14:13] = 0;
							tRegOutSr[12: 8] = regInSr[15:11];
							tRegOutSr[    0] = regInSr[   10];
						end
						5'h04: begin	/* AND */
							tRegOutSr[   0]	= tTempBit0;
						end
						5'h05: begin	/* OR */
							tRegOutSr[   0]	= tTempBit1;
						end
						5'h06: begin	/* AND+PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
							tRegOutSr[   8]	= tTempBit0;
							tRegOutSr[   0]	= tTempBit0;
						end
						5'h07: begin	/* OR+PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
							tRegOutSr[   8]	= tTempBit1;
							tRegOutSr[   0]	= tTempBit1;
						end

						5'h08: begin	/* CLEAR */
							tRegOutSr[15:8]	= 0;
						end


`ifdef def_true
// `ifndef def_true

						5'h0E: begin	/* ELSE */
							if(tTempBit3)
							begin
								if(regInSr[12])
								begin
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[    0] = 0;
								end
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end
						5'h0F: begin	/* ENDIF */
							if(!tTempBit2)
								tRegOutSr[15:12]=tIfElseNN;
							else
								tRegOutSr[11:8]=tIfThenNN;
							tRegOutSr[    0] = tTempBit3;
						end

						5'h10: begin	/* IF0T */
							tRegOutSr[15:8]	= 0;
//							if(regInSr[0])
							if(regRefSr[0])
								tRegOutSr[8]=1;
							else
								tRegOutSr[12]=1;
						end
						5'h11: begin	/* IF0F */
							tRegOutSr[15:8]	= 0;
//							if(!regInSr[0])
							if(!regRefSr[0])
								tRegOutSr[8]=1;
							else
								tRegOutSr[12]=1;
//							tRegOutSr[0] = !regInSr[0];
							tRegOutSr[0] = !regRefSr[0];
						end
						5'h12: begin		/* IF1T */
//							if(regInSr[0] && tTempBit2)
							if(regRefSr[0] && tTempBit2)
							begin
								tRegOutSr[11:8] = tIfThenPP;
								tRegOutSr[   0] = 1;
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end
						5'h13: begin		/* IF1F */
//							if(!regInSr[0] && tTempBit2)
							if(!regRefSr[0] && tTempBit2)
							begin
								tRegOutSr[11:8] =tIfThenPP;
								tRegOutSr[   0] = 1;
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end

						5'h14: begin		/* IFAAT */
							if(tTempBit2)
							begin
//								if(regInSr[0])
								if(regRefSr[0])
								begin
									tRegOutSr[   0] = 1;
								end else begin
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[    0] = 0;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h15: begin		/* IFAAF */
							if(tTempBit2)
							begin
//								if(!regInSr[0] && tTempBit2)
								if(!regRefSr[0] && tTempBit2)
								begin
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[    0] = 0;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h16: begin		/* IFOOT */
							if(tTempBit3)
							begin
//								if(regInSr[0] && !tTempBit2)
								if(regRefSr[0] && !tTempBit2)
								begin
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[    0] = tTempBit2;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h17: begin		/* IFOOF */
							if(tTempBit3)
							begin
//								if(!regInSr[0] && !tTempBit2)
								if(!regRefSr[0] && !tTempBit2)
								begin
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[    0] = tTempBit2;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
`endif

						default:	begin
						end
					endcase
				end
`endif

				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXT %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
				end
			endcase
		end

		default: begin
			if(!tMsgLatch)
				$display("EX1: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
			tExHold		= 1;
//			tExHold		= !reset;
		end
	
	endcase
	
	if(tDoOutDfl)
	begin
		tRegIdRn1		= regIdRm;
		tRegValRn1		= tValOutDfl;
	end
	
	if(tDoMemOp)
	begin
		tMemOpm			= tDoMemOpm;
`ifndef	jx2_mem_l1dstall
		if(memDataOK == UMEM_OK_HOLD)
			tExHold		= 1;
`endif

		if(tValAguOob)
			tExTrapExc = { UV112_00, 16'h800F };
	end
	
	if(tDoDelayCycle)
	begin
//		tRegIdCn1	= JX2_GR_IMM[4:0];
		tRegIdCn1	= JX2_GR_IMM;
	end
	
	if(tDoBra)
	begin
//		tRegIdCn1			= JX2_CR_PC[4:0];
		tRegIdCn1			= JX2_CR_PC;
//		tRegValCn1[63:0]	= {UV16_00, tValBra};
		tRegValCn1[63:0]	= tValBra;

`ifdef jx2_enable_vaddr96
		tRegOutPcHi		= tValBraHi[47:0];
`endif
		
// `ifdef def_true
`ifndef def_true
// `ifdef VERILATOR
		if(tValBra[31:0]==0)
		begin
			if(!tMsgLatch)
				$display("EX1: Branch to Zero, PC=%X", regValPc);
			tNextMsgLatch	= 1;
			tExHold		= 1;
			if(regInExc[15])
				tExHold		= 0;
		end
`endif

	end

	if(opBraFlush)
	begin
		tRegIdRn1	= JX2_GR_ZZR;
//		tRegIdCn1	= JX2_CR_ZZR[4:0];
		tRegIdCn1	= JX2_CR_ZZR;
	end

	if(tRegHeld)
		tHeldIdRn1	= regIdRm;

//	if(tHeldIdRn1 != JX2_GR_ZZR)
//		tRegHeld		= 1;

	if(reset)
	begin
		tExHold		= 0;
	end

end

always @(posedge clock)
begin
	tAguFlagJq	<= regInSr[31];
	tAguFlagSv	<= regInSr[30];
	tMsgLatch	<= tNextMsgLatch;
//	tCanaryMagic	<= tNxtCanaryMagic;
	tCanaryMagic	<= reset ? 0 : tNxtCanaryMagic;

	if(tOpEnable)
		tRegValBPcLo	<= regValBPc[7:0];

end

endmodule

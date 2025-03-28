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

PF IF ID1 ID2 EX1 EX2 EX3 WB

 */

/* verilator lint_off DEFPARAM */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNUSEDPARAM */

`include "CoreDefs.v"

`ifdef jx2_use_ringbus
`include "ringbus/RbiMemL1A.v"
`else
`include "MemL1A.v"
`endif

`include "RegCR.v"

`ifdef jx2_enable_wex3w
`include "DecOpWx3.v"
`ifdef jx2_gprs_usefsm
`include "RegGPR_6R3W_SM.v"
`else
`include "RegGPR_6R3W.v"
`endif
`else
`ifdef jx2_enable_wex2w
`include "DecOpWx2.v"
`include "RegGPR_4R2W.v"
`else
`include "DecOp.v"
`include "RegGPR.v"
`endif
`endif

`include "ExEX1.v"
`include "ExEX2.v"
`include "ExEX3.v"

`include "ExALU.v"
// `include "ExMulB.v"
`include "ExMulC.v"
`include "ExMulW.v"

`ifdef jx2_enable_wex
`include "ExEXB1.v"
`include "ExEXB2.v"
`include "ExALUB.v"
`include "ExEXB3.v"
`endif

`ifdef jx2_enable_wex3w
`include "ExEXC1.v"
`include "ExEXC2.v"
`include "ExEXC3.v"
`endif

`ifdef jx2_enable_prebra
`include "DecPreBra.v"
`endif

`ifdef jx2_enable_fpu
`ifdef jx2_use_fpu_w
`include "FpuExOpW.v"
`else
`include "FpuExOp.v"
`endif
`endif

`ifdef jx2_use_fpu_v4sf
`include "FpuVec4SF.v"
`endif

`ifdef jx2_enable_blint
`include "ExModBlint.v"
`endif

`ifdef jx2_enable_ldekey
`include "ExModKrrEnc.v"
`endif

`ifdef jx2_alu_slomuldiv
`include "ExOpSloMulDiv.v"
`endif

`ifdef jx2_cpu_pred_id2
`include "ExPredCheck.v"
`include "ExPredChkSrT.v"
`include "ExPredChkNeedSrT.v"
`endif

`ifdef jx2_enable_riscv_xg3
`include "DecOpRepXG3.v"
`endif

`ifdef jx2_enable_riscv_op48rep
`include "DecOpRepRv48A.v"
`endif

/* verilator lint_off DEFPARAM */

module ExUnit(
	clock,
	reset,
	timers,
	
	memAddrA,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK,
	memBusExc,
	
	memAddrInA,		memAddrInB,
	memOpmIn,
	memSeqIn,		memSeqOut,
	unitNodeId,
	
	dbgOutPc,		dbgOutIstr,
	dbgExHold1,		dbgExHold2,

	dbgDcInAddr,	dbgDcInOpm,
	dbgDcOutVal,	dbgDcInVal,
	dbgDcOutOK,		dbgExWidth,
	dbgMisc,
	
	dbgOutStatus1,	dbgOutStatus2,
	dbgOutStatus3,	dbgOutStatus4,
	dbgOutStatus5,	dbgOutStatus6,
	dbgOutStatus7,	dbgOutStatus8
	);

input			clock;

(* max_fanout = 100 *)
	input			reset;

input[11:0]		timers;

// `output_l2addr		memAddr;
// `output_l2addr		memAddrB;
// `input_l2addr		memAddrIn;

output[95:0]		memAddrA;
output[95:0]		memAddrB;
input[95:0]			memAddrInA;
input[95:0]			memAddrInB;

`wire_l2addr		memAddrOut;
`wire_l2addr		memAddrIn;

// assign			memAddrIn = { memAddrInB, memAddrInA };
// assign			memAddrA = memAddrOut[47: 0];
// assign			memAddrB = memAddrOut[95:48];

`ifdef jx2_enable_l2addr96
assign			memAddrIn = memAddrInA;
assign			memAddrA = memAddrOut;
`else
assign			memAddrIn = memAddrInA[47:0];
assign			memAddrA = { UV48_00, memAddrOut };
`endif

`input_tile		memDataIn;
`output_tile	memDataOut;

output[15:0]	memOpm;
input[15:0]		memOpmIn;

output[15:0]	memSeqOut;
input[15:0]		memSeqIn;

input[1:0]		memOK;
input[127:0]	memBusExc;

input[7:0]		unitNodeId;
	

// output[31:0]	dbgOutPc;
// `output_vaddr	dbgOutPc;
output[47:0]	dbgOutPc;
output[95:0]	dbgOutIstr;

output			dbgExHold1;
output			dbgExHold2;

// output[31:0]	dbgDcInAddr;
`output_vaddr	dbgDcInAddr;
output[4:0]		dbgDcInOpm;
output[63:0]	dbgDcOutVal;
output[63:0]	dbgDcInVal;
output[ 1:0]	dbgDcOutOK;

output[ 1:0]	dbgExWidth;
output[ 7:0]	dbgMisc;

output			dbgOutStatus1;
output			dbgOutStatus2;
output			dbgOutStatus3;
output			dbgOutStatus4;
output			dbgOutStatus5;
output			dbgOutStatus6;
output			dbgOutStatus7;
output			dbgOutStatus8;

parameter		isAltCore = 0;
// defparam		ex1.cpuid.isAltCore = isAltCore;
defparam		ex1.isAltCore = isAltCore;

parameter		isGpu = 0;
parameter		fpuLowPrec = isGpu;
parameter		noLane3 = isGpu;

defparam		memL1.disableTlb = isGpu;
defparam		regCr.disableTlb = isGpu;

`ifdef jx2_enable_wex
defparam		regGpr.noLane3 = isGpu;

defparam		decOp.noLane3 = isGpu;
defparam		decOp.noNoRiscV = isGpu;
defparam		decOp.fpuLowPrec = fpuLowPrec;
`endif

defparam		exAlu.noBcd = isGpu;
defparam		exAlu.noAlux = isGpu;

(* max_fanout = 200 *)
	reg				exHold1;
// (* max_fanout = 200 *)
//	reg				exHold2;

(* max_fanout = 200 *)
	wire			exHold2;

(* max_fanout = 200 *)
	reg				exResetL;
(* max_fanout = 200 *)
	reg				exResetL2;

(* max_fanout = 300 *)
	wire			exHold1N = !exHold1;

(* max_fanout = 300 *)
	wire			exHold2N = !exHold2;

reg				exHold1L;

reg				exHold1A;
reg				exHold1B;
reg				exHold1C;
reg				exHold1D;

reg				exHold1B1;
reg				exHold1B2;
reg				exHold1B3;

reg				exHold1C1;
reg				exHold1C2;
reg				exHold1C3;

reg				exHold1D1;
reg				exHold1D2;
reg				exHold1D3;

(* max_fanout = 300 *)
	reg				tDbgExHold1;
(* max_fanout = 300 *)
	reg				tDbgExHold2;

reg				tDbgExHold1B;
reg				tDbgExHold2B;

// assign		dbgExHold1 = exHold1;
// assign		dbgExHold2 = exHold2;

assign		dbgExHold1 = tDbgExHold1B;
assign		dbgExHold2 = tDbgExHold2B;

(* max_fanout = 400 *)
	reg			tDbgOutStatus1;
(* max_fanout = 400 *)
	reg			tDbgOutStatus2;
(* max_fanout = 400 *)
	reg			tDbgOutStatus3;
(* max_fanout = 400 *)
	reg			tDbgOutStatus4;
(* max_fanout = 400 *)
	reg			tDbgOutStatus5;
(* max_fanout = 400 *)
	reg			tDbgOutStatus6;
(* max_fanout = 400 *)
	reg			tDbgOutStatus7;
(* max_fanout = 400 *)
	reg			tDbgOutStatus8;

reg			tDbgOutStatus1B;
reg			tDbgOutStatus2B;
reg			tDbgOutStatus3B;
reg			tDbgOutStatus4B;
reg			tDbgOutStatus5B;
reg			tDbgOutStatus6B;
reg			tDbgOutStatus7B;
reg			tDbgOutStatus8B;

`ifndef def_true
assign		dbgOutStatus1 = tDbgOutStatus1B;
assign		dbgOutStatus2 = tDbgOutStatus2B;
assign		dbgOutStatus3 = tDbgOutStatus3B;
assign		dbgOutStatus4 = tDbgOutStatus4B;
assign		dbgOutStatus5 = tDbgOutStatus5B;
assign		dbgOutStatus6 = tDbgOutStatus6B;
assign		dbgOutStatus7 = tDbgOutStatus7B;
assign		dbgOutStatus8 = tDbgOutStatus8B;
`endif

reg			tDbgOutStatus1C;
reg			tDbgOutStatus2C;
reg			tDbgOutStatus3C;
reg			tDbgOutStatus4C;
reg			tDbgOutStatus5C;
reg			tDbgOutStatus6C;
reg			tDbgOutStatus7C;
reg			tDbgOutStatus8C;

`ifdef def_true
assign		dbgOutStatus1 = tDbgOutStatus1C;
assign		dbgOutStatus2 = tDbgOutStatus2C;
assign		dbgOutStatus3 = tDbgOutStatus3C;
assign		dbgOutStatus4 = tDbgOutStatus4C;
assign		dbgOutStatus5 = tDbgOutStatus5C;
assign		dbgOutStatus6 = tDbgOutStatus6C;
assign		dbgOutStatus7 = tDbgOutStatus7C;
assign		dbgOutStatus8 = tDbgOutStatus8C;
`endif

reg[1:0]	tDbgExWidth;
reg[1:0]	tDbgExWidthB;
reg[1:0]	tDbgExWidthC;

assign	dbgExWidth	= tDbgExWidthC;

wire[7:0]	tDbgMisc;
reg[7:0]	tDbgMiscB;
reg[7:0]	tDbgMiscC;

assign	dbgMisc	= tDbgMiscC;

/* IF */

(* max_fanout = 200 *)
	wire[63:0]		gprOutDlr;
(* max_fanout = 200 *)
	wire[63:0]		gprOutDhr;

wire[63:0]		crOutMmcr;
wire[63:0]		crOutKrr;
wire[47:0]		crOutVipt;

wire[63:0]		crOutSr;
reg [63:0]		crInSr;

reg[11:0]		ex1Timers;

reg[47:0]		ifValPc;
reg[47:0]		ifLastPc;
reg[47:0]		ifValPcHi;

reg[47:0]		ifValPcInc;

// assign		ifValPcHi = crOutPcHi;

`ifdef jx2_enable_vaddr96
`wire_vaddr		ifValPcW = { ifValPcHi, ifValPc };
`wire_vaddr		ifLastPcW = { ifValPcHi, ifLastPc };
`wire_vaddr		ifLastPcIncW = { ifValPcHi, ifValPcInc };
`else

`ifdef jx2_enable_vaddr48
`wire_vaddr		ifValPcW = ifValPc;
`wire_vaddr		ifLastPcW = ifLastPc;
`wire_vaddr		ifValPcIncW = ifValPcInc;
`else
`wire_vaddr		ifValPcW = ifValPc[31:0];
`wire_vaddr		ifLastPcW = ifLastPc[31:0];
`wire_vaddr		ifValPcIncW = ifValPcInc[31:0];
`endif

`endif

(* max_fanout = 200 *)
	reg[63:0]		ifValFetchSr;

(* max_fanout = 200 *)
 	reg[7:0]	opBraFlushMask;
(* max_fanout = 200 *)
	reg[7:0]	nxtBraFlushMask;
(* max_fanout = 200 *)
	reg[7:0]	tIsrBraFlushMask;

// wire[63:0]	ifIstrWord;	//source instruction word
wire[95:0]		ifIstrWord;	//source instruction word
wire[95:0]		ifIstrWordA;	//source instruction word
wire[5:0]		ifIstrMTag;		//source mode tags

wire[1:0]		ifOutPcOK;
wire[3:0]		ifOutPcStep;
wire[3:0]		ifOutPcStepAdj;
reg[3:0]		ifLastPcStep;
reg				ifInPcHold;
wire[3:0]		ifOutPcSxo;

wire[11:0]		ifOutPcLow;

reg[127:0]		tRegExc;

reg				ifValBraOk;
reg				ifNxtValBraOk;

// wire[1:0]			ifInPcWxe;
wire[5:0]			ifInPcWxe;

`ifdef jx2_enable_wex
// assign	ifInPcWxe = crOutSr[27:26];
assign	ifInPcWxe = { crOutSr[23:20], crOutSr[27:26] };
`else
// assign	ifInPcWxe = 2'b00;
assign	ifInPcWxe = 6'b00;
`endif

// assign	dbgOutPc	= ifLastPcW;
// `ifdef jx2_enable_vaddr48
`ifdef def_true
assign	dbgOutPc	= ifLastPc;
`else
assign	dbgOutPc	= { 16'h00, ifLastPc };
`endif

assign	dbgOutIstr	= ifIstrWord;

`reg_vaddr		dcInAddr;
reg [4:0]		dcInOpm;
reg [4:0]		dcInLdOp;
wire[65:0]		dcOutVal;
reg [65:0]		dcInVal;
wire[ 1:0]		dcOutOK;
reg				dcInHold;
wire			dcOutHold;
wire			dcBusWait;

wire[65:0]		dcOutValB;
reg [65:0]		dcInValB;

reg [4:0]		dcInOpmB;
reg [47:0]		dcInAddrB;

reg [63:0]		dcInTraPc;

wire[65:0]		dcOutValFast;

reg			tDeadlockLatch;
reg			tNxtDeadlockLatch;

assign	dbgDcInAddr	= dcInAddr;
assign	dbgDcInOpm	= dcInOpm;
assign	dbgDcOutVal	= dcOutVal[63:0];
assign	dbgDcInVal	= dcInVal[63:0];
assign	dbgDcOutOK	= dcOutOK;

wire[127:0]		memRegExc;
wire[63:0]		memRegTraPc;

wire			srFlushIf;		//Flushing the IF stage.
wire			srFlushId1;		//Flushing the ID1 stage.
wire			srFlushId2;		//Flushing the ID2 stage.

assign		srFlushIf = opBraFlushMask[3];
assign		srFlushId1 = opBraFlushMask[2];
assign		srFlushId2 = opBraFlushMask[1];

wire			srWxe;
wire			srXG2;
wire			srXG3;
wire			srRiscv;
wire			srXG2RV;
wire			srUser;
wire			srSxo;
// assign		srWxe	= crOutSr[27];
// assign		srUser	= !crOutSr[30];
// assign		srRiscv	= (crOutSr[27:26] == 2'b01);
// assign		srRiscv	= crOutSr[26];

assign		srWxe	= ifValFetchSr[27];
assign		srUser	= !ifValFetchSr[30];
assign		srRiscv	= ifValFetchSr[26];
assign		srXG3 = srRiscv && srWxe;

wire[63:0]		ifIstrWordB;
wire[63:0]		ifIstrWordC;

`ifdef jx2_enable_riscv_op48rep
wire[1:0]		ifIstrRv48MTag;

DecOpRepRv48A	repRv48a(
	clock, ifIstrWordA[63: 0], ifIstrWordB[63: 0],
	ifOutPcStep, ifOutPcStepAdj,
	ifIstrRv48MTag,
	srRiscv, srFlushIf);
`else
assign		ifIstrWordB = ifIstrWordA[63:0];
assign		ifOutPcStepAdj = ifOutPcStep;
`endif


`ifdef jx2_enable_riscv_xg3
DecOpRepXG3		repXg3a(
	clock, ifIstrWordA[31: 0], ifIstrWordC[31: 0], ifIstrMTag[1:0],
	srXG3, srFlushIf);
DecOpRepXG3		repXg3b(
	clock, ifIstrWordA[63:32], ifIstrWordC[63:32], ifIstrMTag[3:2],
	srXG3, srFlushIf);
DecOpRepXG3		repXg3c(
	clock, ifIstrWordA[95:64], ifIstrWord[95:64], ifIstrMTag[5:4],
	srXG3, srFlushIf);

assign	ifIstrWord[31: 0] = ifIstrMTag[0] ?
	ifIstrWordC[31: 0] : ifIstrWordB[31: 0];
assign	ifIstrWord[63:32] = ifIstrMTag[2] ?
	ifIstrWordC[63:32] : ifIstrWordB[63:32];

`else
assign	ifIstrWord = ifIstrWordA;
assign	ifIstrMTag = 0;
`endif

`ifdef jx2_use_ringbus

RbiMemL1A		memL1(
	clock,			exResetL,

	ifValPcW,		ifIstrWordA,
	ifOutPcOK,		ifOutPcStep,
	ifInPcHold,		ifInPcWxe,
	ifOutPcSxo,		ifOutPcLow,
	ifValPcIncW,

	dcInAddr,		dcInOpm,
	dcInAddrB,		dcInOpmB,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,		dcInHold,
	dcOutHold,		dcBusWait,
	dcInLdOp,		dcOutValFast,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,
	crOutSr,		crOutVipt,

	memRegExc,		memRegTraPc,
	dcInTraPc,		tDeadlockLatch,
	tRegExc,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpm,
	memSeqIn,		memSeqOut,
	unitNodeId,		ex1Timers
	);

assign		memAddrB = 0;

`else

// assign	memSeqOut	= UV16_00;
assign	memSeqOut	= 16'hFF00;

MemL1A		memL1(
	clock,			exResetL,

	ifValPc,		ifIstrWord,
	ifOutPcOK,		ifOutPcStep,
	ifInPcHold,		ifInPcWxe,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,		dcInHold,		dcOutHold,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,		crOutSr,

	memRegExc,		memRegTraPc,
	dcInTraPc,		tDeadlockLatch,

	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK
	);
`endif

/* ID1 */

// wire[47:0]		gprValGbr;
wire[63:0]		gprValGbr;
wire[47:0]		gprValTbr;
wire[63:0]		gprValLr;
wire[63:0]		gprValSsp;
`wire_gprval		gprValCm;

//reg[31:0]		id2IstrWord;	//source instruction word
//reg[31:0]		id2IstrWordL;	//source instruction word


(* max_fanout = 200 *)
	reg[47:0]		id1ValBPc;
(* max_fanout = 200 *)
	reg[47:0]		id1ValPc;

reg[3:0]		id1PcStep;

// reg[63:0]	id1IstrWord;	//source instruction word

(* max_fanout = 200 *)
	reg[95:0]		id1IstrWord;	//source instruction word

(* max_fanout = 200 *)
	reg[5:0]		id1IstrMTag;	//source instruction word

// reg[31:0]	id1IstrWordL1;	//source instruction word
// reg[31:0]	id1IstrWordL2;	//source instruction word
reg[95:0]		id1IstrWordL1;	//source instruction word
reg[95:0]		id1IstrWordL2;	//source instruction word
reg[3:0]		id1IstrSxo;

// (* max_fanout = 200 *)
//	reg[63:0]		ifValFetchSr;
(* max_fanout = 200 *)
	reg[63:0]		id1ValFetchSr;
(* max_fanout = 200 *)
	reg[63:0]		id2ValFetchSr;
(* max_fanout = 200 *)
	reg[63:0]		ex1ValFetchSr;

reg[1:0]		id2PreBra;

`ifdef jx2_enable_wex

`wire_gpr		idA1IdRegM;
`wire_gpr		idA1IdRegO;
`wire_gpr		idA1IdRegN;
wire[32:0]		idA1IdImm;
wire[8:0]		idA1IdUCmd;
wire[8:0]		idA1IdUIxt;

`wire_gpr		idB1IdRegM;
`wire_gpr		idB1IdRegO;
`wire_gpr		idB1IdRegN;
wire[32:0]		idB1IdImm;
wire[8:0]		idB1IdUCmd;
wire[8:0]		idB1IdUIxt;

`ifdef jx2_enable_wex3w
`wire_gpr		idC1IdRegM;
`wire_gpr		idC1IdRegO;
`wire_gpr		idC1IdRegN;
wire[32:0]		idC1IdImm;
wire[8:0]		idC1IdUCmd;
wire[8:0]		idC1IdUIxt;

wire[8:0]		idA1IdUCmd0;
wire[8:0]		idB1IdUCmd0;
wire[8:0]		idC1IdUCmd0;

assign		idA1IdUCmd = (id2PreBra == 2'b01) ?
	JX2_UCMDSP_BRANOP : idA1IdUCmd0;
assign		idB1IdUCmd = (id2PreBra == 2'b01) ?
	JX2_UCMDSP_BRANOP : idB1IdUCmd0;
assign		idC1IdUCmd = (id2PreBra == 2'b01) ?
	JX2_UCMDSP_BRANOP : idC1IdUCmd0;

DecOpWx3	decOp(
	clock,			exResetL,
//	id1IstrWord,	ifInPcWxe,
	id1IstrWord,	id1IstrMTag,
	id1ValBPc,		id1ValFetchSr,
	id1IstrSxo,		id1PcStep,

	idA1IdRegM,		idA1IdRegO,
	idA1IdRegN,		idA1IdImm,
	idA1IdUCmd0,	idA1IdUIxt,

	idB1IdRegM,		idB1IdRegO,
	idB1IdRegN,		idB1IdImm,
	idB1IdUCmd0,	idB1IdUIxt,

	idC1IdRegM,		idC1IdRegO,
	idC1IdRegN,		idC1IdImm,
	idC1IdUCmd0,	idC1IdUIxt
	);
`endif

`ifdef jx2_enable_wex2w
wire[8:0]		idA1IdUCmd0;
wire[8:0]		idB1IdUCmd0;

assign		idA1IdUCmd = (id2PreBra == 2'b01) ?
	JX2_UCMDSP_BRANOP : idA1IdUCmd0;
assign		idB1IdUCmd = (id2PreBra == 2'b01) ?
	JX2_UCMDSP_BRANOP : idB1IdUCmd0;

DecOpWx2	decOp(
	clock,			exResetL,
//	id1IstrWord,
//	id1ValFetchSr,
//	id1IstrSxo,

	id1IstrWord,	id1IstrMTag,
	id1ValBPc,		id1ValFetchSr,
	id1IstrSxo,		id1PcStep,

	idA1IdRegM,		idA1IdRegO,
	idA1IdRegN,		idA1IdImm,
	idA1IdUCmd0,	idA1IdUIxt,

	idB1IdRegM,		idB1IdRegO,
	idB1IdRegN,		idB1IdImm,
	idB1IdUCmd0,	idB1IdUIxt
	);
`endif

`else

`wire_gpr		id1IdRegN;
`wire_gpr		id1IdRegM;
`wire_gpr		id1IdRegO;
wire[32:0]		id1IdImm;
wire[32:0]		id1IdImmB;
wire[8:0]		id1IdUCmd;
wire[8:0]		id1IdUIxt;

DecOp	decOp(
	clock,	exResetL,
	id1IstrWord[63:0],
	{ id1IstrWordL2[31:0], id1IstrWordL1[31:0] },
	id1IdRegN,	id1IdRegM,
	id1IdRegO,	id1IdImm,	id1IdImmB,
	id1IdUCmd,	id1IdUIxt
	);

`endif

(* max_fanout = 200 *)
	reg[47:0]		ex1ValBPc;
reg[2:0]		ex1ValBraDir;

wire[ 4:0]		ex1MemOpm;
wire[ 4:0]		exB1MemOpm;
wire[ 4:0]		ex1MemLdOp;

wire[47:0]		id1PreBraPc;
wire[47:0]		id1PreBraPcInc;
wire[1:0]		id1PreBra;
wire			id1WasBra;

`ifndef jx2_enable_prebra
assign		id1PreBraPc		= 0;
assign		id1PreBraPcInc	= 0;
assign		id1PreBra		= 2'b10;
assign		id1WasBra		= 0;
`endif

`ifdef jx2_enable_prebra

`ifdef jx2_prebra_rts
reg				id1BraPipelineLr;
reg				id1BraPipelineR1;
// reg				id1BraPipelineDbl;
`else
wire			id1BraPipelineLr;
wire			id1BraPipelineR1;
// wire			id1BraPipelineDbl;
assign		id1BraPipelineLr = 1'b1;
assign		id1BraPipelineR1 = 1'b1;
// assign		id1BraPipelineDbl = 1'b1;
`endif

reg				id1BraPipelineDbl;

reg[7:0]			id1BraPipelineLrL;

DecPreBra	preBra(
	clock,				exResetL,
	id1IstrWord[63:0],	id1IstrMTag[3:0],
	id1ValBPc,			id1ValPc,
	id1PreBraPc,		id1PreBra,
	id1PreBraPcInc,		id1WasBra,

	gprValLr,			ifLastPc,
	gprOutDlr,			gprOutDhr,
	ex1ValBPc,			ex1ValBraDir,
	id1BraPipelineLrL,	ex1MemOpm);
`endif
	

/* ID2 */

reg[47:0]		id2ValBPc;
// reg[8:0]		id2IdUCmd;
// reg[8:0]		id2IdUIxt;
reg[47:0]		id2PreBraPc;
// reg[1:0]		id2PreBra;
// reg[31:0]		id2IstrWord;	//source instruction word
reg[95:0]		id2IstrWord;	//source instruction word
// reg[31:0]		id2IstrWordL;	//source instruction word

wire	crIsIsrEdge;
// assign	crIsIsrEdge	= crOutSr[29] ^ crInSr[29];
assign	crIsIsrEdge	= 1'b0;


/* ID2, GPR */

`ifdef jx2_enable_wex

reg[8:0]		idA2IdUCmd;
reg[8:0]		idA2IdUIxt;
reg[8:0]		idB2IdUCmd;
reg[8:0]		idB2IdUIxt;
`ifdef jx2_enable_wex3w
reg[8:0]		idC2IdUCmd;
reg[8:0]		idC2IdUIxt;
`endif

`reg_gpr		gprIdRs;		//Source A, ALU / Base
`reg_gpr		gprIdRt;		//Source B, ALU / Index
`reg_gpr		gprIdRu;		//Source C
`reg_gpr		gprIdRv;		//Source D, MemStore
`reg_gpr		gprIdRm;		//Dest A
`reg_gpr		gprIdRn;		//Dest B
`reg_gpr		gprIdRo;		//Dest C

`wire_gprval		gprValRs;
`wire_gprval		gprValRt;
`wire_gprval		gprValRu;
`wire_gprval		gprValRv;

(* max_fanout = 200 *)
	reg				gprEx1Flush;
(* max_fanout = 200 *)
	reg				gprEx2Flush;
(* max_fanout = 200 *)
	reg				gprEx3Flush;

reg				gprEx1DualLane;
reg				gprEx2DualLane;
reg				gprEx3DualLane;

`ifdef jx2_enable_wex3w

`reg_gpr		gprIdRx;		//Source C
`reg_gpr		gprIdRy;		//Source D, MemStore
`wire_gprval		gprValRx;
`wire_gprval		gprValRy;

`wire_gprval		gprValRm;
`wire_gprval		gprValRn;
`wire_gprval		gprValRo;
assign		gprValRm	= gprValRy;
assign		gprValRn	= gprValRx;
assign		gprValRo	= gprValRy;
// assign		gprValRo	= 0;

`else

`wire_gprval		gprValRm;
`wire_gprval		gprValRn;
assign		gprValRm	= gprValRv;
assign		gprValRn	= gprValRu;

`endif

`reg_gpr		gprIdRn1;
`reg_gprval		gprValRn1;
`reg_gpr		gprIdRn2;
`reg_gprval		gprValRn2;
`reg_gpr		gprIdRn3;
`reg_gprval		gprValRn3;
`reg_gpr		gprIdRn4;
`reg_gprval		gprValRn4;

`reg_gpr		gprIdRnB1;
`reg_gprval		gprValRnB1;
`reg_gpr		gprIdRnB2;
`reg_gprval		gprValRnB2;
`reg_gpr		gprIdRnB3;
`reg_gprval		gprValRnB3;
`reg_gpr		gprIdRnB4;
`reg_gprval		gprValRnB4;

`ifdef jx2_enable_wex3w
`reg_gpr		gprIdRnC1;
`reg_gprval		gprValRnC1;
`reg_gpr		gprIdRnC2;
`reg_gprval		gprValRnC2;
`reg_gpr		gprIdRnC3;
`reg_gprval		gprValRnC3;
`reg_gpr		gprIdRnC4;
`reg_gprval		gprValRnC4;
`endif

wire[1:0]		ex1Hold;
wire[1:0]		exB1Hold;
wire[1:0]		exC1Hold;

wire[1:0]		ex2Hold;
wire[1:0]		exB2Hold;
wire[1:0]		exC2Hold;

wire[1:0]		ex3Hold;
wire[1:0]		exB3Hold;
wire[1:0]		exC3Hold;

reg [47:0]		gprValPc;
reg [32:0]		gprValImm;
reg [32:0]		gprValImmB;
reg [32:0]		gprValImmC;

reg [32:0]		gprValImmA4;
reg [32:0]		gprValImmB4;
reg [32:0]		gprValImmC4;

wire[47:0]	crOutSsp;

assign		gprValGbr = crOutGbr;
assign		gprValTbr = crOutTbr;
assign		gprValLr = crOutLr;
assign		gprValSsp = { 16'h0, crOutSsp };
assign		gprValCm = crValCm;

reg [63:0]		gprInDlr;
reg [63:0]		gprInDhr;
wire[63:0]		gprOutSp;
reg [63:0]		gprInSp;

wire[63:0]		gprInDlrB;
wire[63:0]		gprInDhrB;
wire[63:0]		gprInSpB;

// assign		gprInDlrB = gprInDlr;
// assign		gprInDhrB = gprInDhr;

assign		gprInDlrB = gprOutDlr;
assign		gprInDhrB = gprOutDhr;

`ifdef jx2_reg_spdecswap
assign	gprInSpB = gprOutSp;
`else
assign	gprInSpB = gprInSp;
`endif

`ifdef jx2_sprs_elrehr
wire[63:0]		gprOutElr;
reg [63:0]		gprInElr;
wire[63:0]		gprOutEhr;
reg [63:0]		gprInEhr;
wire[63:0]		gprOutBp;
reg [63:0]		gprInBp;
`endif

wire			gprDoHold;

(* max_fanout = 200 *)
	wire exHold2NotIsrEdge = exHold2 && !crIsIsrEdge;

`ifdef jx2_enable_wex3w

`ifdef jx2_gprs_usefsm
RegGPR_6R3W_SM regGpr(
`else
RegGPR_6R3W regGpr(
`endif
	clock,
	exResetL,
//	exHold2,
//	exHold2 && !crIsIsrEdge,
	exHold2NotIsrEdge,

	idA2IdUCmd,
	idA2IdUIxt,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRu,		//Source C
	gprIdRv,		//Source D
	gprIdRx,		//Source E
	gprIdRy,		//Source F, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRu,		//Source C Value
	gprValRv,		//Source D Value
	gprValRx,		//Source E Value
	gprValRy,		//Source F Value

	gprIdRn1,		//Destination ID (EX1, L1)
	gprValRn1,		//Destination Value (EX1, L1)
	gprIdRnB1,		//Destination ID (EX1, L2)
	gprValRnB1,		//Destination Value (EX1, L2)
	gprIdRnC1,		//Destination ID (EX1, L3)
	gprValRnC1,		//Destination Value (EX1, L3)

	gprIdRn2,		//Destination ID (EX2, L1)
	gprValRn2,		//Destination Value (EX2, L1)
	gprIdRnB2,		//Destination ID (EX2, L2)
	gprValRnB2,		//Destination Value (EX2, L2)
	gprIdRnC2,		//Destination ID (EX2, L3)
	gprValRnC2,		//Destination Value (EX2, L3)

	gprIdRn3,		//Destination ID (EX3, L1)
	gprValRn3,		//Destination Value (EX3, L1)
	gprIdRnB3,		//Destination ID (EX3, L2)
	gprValRnB3,		//Destination Value (EX3, L2)
	gprIdRnC3,		//Destination ID (EX3, L3)
	gprValRnC3,		//Destination Value (EX3, L3)

	gprIdRn4,		//Destination ID (EX3, L1)
	gprValRn4,		//Destination Value (EX3, L1)
	gprIdRnB4,		//Destination ID (EX3, L2)
	gprValRnB4,		//Destination Value (EX3, L2)
	gprIdRnC4,		//Destination ID (EX3, L3)
	gprValRnC4,		//Destination Value (EX3, L3)

	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValTbr,		//TBR Value (CR)
	gprValImm,		//Immediate (Decode, A)
	gprValImmB,		//Immediate (Decode, B)
	gprValImmC,		//Immediate (Decode, C)
	gprValLr,		//LR Value (CR)
	gprValSsp,		//SSP Value (CR)
	gprValCm,		//Cm Port (CR)
	id2ValBPc,		//BPC

	gprValImmA4,	//Immediate (WB, A)
	gprValImmB4,	//Immediate (WB, B)
	gprValImmC4,	//Immediate (WB, C)

	gprEx1Flush,	//Flush EX1
	gprEx2Flush,	//Flush EX2
	gprEx3Flush,	//Flush EX3

	gprEx1DualLane,
	gprEx2DualLane,
	gprEx3DualLane,
	
	{	exC3Hold[1], exB3Hold[1], ex3Hold[1],
		exC2Hold[1], exB2Hold[1], ex2Hold[1],
		exC1Hold[1], exB1Hold[1], ex1Hold[1]	},

	crOutSr[31:0],
	gprDoHold,

	gprOutDlr,	gprInDlrB,
	gprOutDhr,	gprInDhrB,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSpB
	);

`else

assign		gprDoHold = 0;

RegGPR_4R2W regGpr(
	clock,
	exResetL,
//	exHold2 && !crIsIsrEdge,
	exHold2NotIsrEdge,

	idA2IdUCmd,
	idA2IdUIxt,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRu,		//Source C
	gprIdRv,		//Source D, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRu,		//Source C Value
	gprValRv,		//Source D Value

	gprIdRn1,		//Destination ID (EX1, L1)
	gprValRn1,		//Destination Value (EX1, L1)
	gprIdRnB1,		//Destination ID (EX1, L2)
	gprValRnB1,		//Destination Value (EX1, L2)

	gprIdRn2,		//Destination ID (EX2, L1)
	gprValRn2,		//Destination Value (EX2, L1)	
	gprIdRnB2,		//Destination ID (EX2, L2)
	gprValRnB2,		//Destination Value (EX2, L2)
	
	gprIdRn3,		//Destination ID (EX3, L1)
	gprValRn3,		//Destination Value (EX3, L1)	
	gprIdRnB3,		//Destination ID (EX3, L2)
	gprValRnB3,		//Destination Value (EX3, L2)

	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValTbr,		//TBR Value (CR)
	gprValImm,		//Immediate (Decode, A)
	gprValImmB,		//Immediate (Decode, B)
	gprValLr,		//LR Value (CR)
	gprValSsp,		//SSP Value (CR)
	gprValCm,		//Cm Port (CR)
	id2ValBPc,		//BPC

	gprEx1Flush,	//Flush EX1
	gprEx2Flush,	//Flush EX2
	gprEx3Flush,	//Flush EX3

	gprEx1DualLane,
	gprEx2DualLane,
	gprEx3DualLane,

	gprOutDlr,	gprInDlrB,
	gprOutDhr,	gprInDhrB,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSpB
	);
`endif

`else

wire[47:0]	crOutSsp;

reg				gprEx1Flush;
reg				gprEx2Flush;
reg				gprEx3Flush;

reg				gprEx1DualLane;
reg				gprEx2DualLane;
reg				gprEx3DualLane;

reg[8:0]		id2IdUCmd;
reg[8:0]		id2IdUIxt;

`reg_gpr		gprIdRs;		//Source A, ALU / Base
`reg_gpr		gprIdRt;		//Source B, ALU / Index
`reg_gpr		gprIdRm;		//Source C, MemStore

`wire_gprval		gprValRs;
`wire_gprval		gprValRt;
`wire_gprval		gprValRm;

`reg_gpr		gprIdRn1;
`reg_gprval		gprValRn1;
`reg_gpr		gprIdRn2;
`reg_gprval		gprValRn2;
`reg_gpr		gprIdRn3;
`reg_gprval		gprValRn3;

reg [47:0]		gprValPc;
reg [32:0]		gprValImm;
reg [32:0]		gprValImmB;
// reg [31:0]		gprValGbr;
// reg [31:0]		gprValLr;

// wire[31:0]		gprValGbr;
// wire[31:0]		gprValLr;
assign		gprValGbr = crOutGbr;
assign		gprValLr = crOutLr;
assign		gprValCm = crValCm;

// wire[63:0]		gprOutDlr;
reg [63:0]		gprInDlr;
// wire[63:0]		gprOutDhr;
reg [63:0]		gprInDhr;
wire[63:0]		gprOutSp;
reg [63:0]		gprInSp;

`ifdef jx2_sprs_elrehr
wire[63:0]		gprOutElr;
reg [63:0]		gprInElr;
wire[63:0]		gprOutEhr;
reg [63:0]		gprInEhr;
wire[63:0]		gprOutBp;
reg [63:0]		gprInBp;
`endif

RegGPR regGpr(
	clock,
	exResetL,
//	exHold2,
//	exHold2 && !crIsIsrEdge,
	exHold2NotIsrEdge,

	id2IdUCmd,
	id2IdUIxt,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRm,		//Source C, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRm,		//Source C Value

	gprIdRn1,		//Destination ID (EX1)
	gprValRn1,		//Destination Value (EX1)
	gprIdRn2,		//Destination ID (EX2)
	gprValRn2,		//Destination Value (EX2)
	gprIdRn3,		//Destination ID (EX3)
	gprValRn3,		//Destination Value (EX3)
	
	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValImm,		//Immediate (Decode)
	gprValImmB,		//Immediate (Decode)
	gprValLr,		//LR Value (CR)
	crOutSsp,
	gprValCm,		//Cm Port (CR)
	
	gprOutDlr,	gprInDlr,
	gprOutDhr,	gprInDhr,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSp
	);

`endif

/* ID2, FPR */

// wire[63:0]		crOutSr;

`ifdef jx2_enable_fpu

// `wire_gprval		gprValFRs;
// `wire_gprval		gprValFRt;

// assign			gprValFRs = gprValRs;
// assign			gprValFRt = gprValRt;

`endif

/* ID2, CR */

//reg [ 4:0]	crIdCm;		//Source ID
`reg_gpr	crIdCm;		//Source ID
`wire_gprval	crValCm;		//Source Value
//reg [ 4:0]	crIdCn1;		//Destination ID (EX1)
`reg_gpr	crIdCn1;		//Destination ID (EX1)
`reg_gprval	crValCn1;		//Destination Value (EX1)
//reg [ 4:0]	crIdCn2;		//Destination ID (EX2)
`reg_gpr	crIdCn2;		//Destination ID (EX2)
`reg_gprval	crValCn2;		//Destination Value (EX2)
//reg [ 4:0]	crIdCn3;		//Destination ID (EX3)
`reg_gpr	crIdCn3;		//Destination ID (EX3)
`reg_gprval	crValCn3;		//Destination Value (EX3)
`reg_gpr	crIdCn4;		//Destination ID (EX4/WB)
`reg_gprval	crValCn4;		//Destination Value (EX4/WB)
wire[47:0]	crOutPc;
reg [47:0]	crInPc;
wire[63:0]	crOutLr;
reg [63:0]	crInLr;
// wire[63:0]	crOutSr;
// reg [63:0]	crInSr;

wire[63:0]	crOutExsr;
reg [63:0]	crInExsr;
wire[47:0]	crOutSpc;
reg [47:0]	crInSpc;
// wire[47:0]	crOutSsp;
reg [47:0]	crInSsp;
wire[63:0]	crOutTea;
reg [63:0]	crInTea;
wire[63:0]	crOutTeaHi;
reg [63:0]	crInTeaHi;

wire [47:0]	crInSspB;

`ifdef jx2_reg_spdecswap
assign	crInSspB = crOutSsp;
`else
assign	crInSspB = crInSsp;
`endif

wire[15:0]	crOutFpsr;
// reg [15:0]	crInFpsr;
wire[15:0]	crInFpsr;

wire[15:0]	crOutVbrCm;

wire[47:0]	crOutVbr;
// wire[47:0]	crOutGbr;
wire[63:0]	crOutGbr;
wire[47:0]	crOutTbr;
// wire[63:0]	crOutMmcr;
// wire[63:0]	crOutKrr;

wire[47:0]	crOutPcHi;
reg [47:0]	crInPcHi;
wire[47:0]	crOutLrHi;
reg [47:0]	crInLrHi;
wire[47:0]	crOutSpcHi;
reg [47:0]	crInSpcHi;

wire[47:0]	crOutVbrHi;
wire[47:0]	crOutGbrHi;
// wire[47:0]	crOutTbrHi;

`ifndef jx2_enable_vaddr96
assign	crOutPcHi = 0;
assign	crOutLrHi = 0;
assign	crOutSpcHi = 0;
assign	crOutVbrHi = 0;
assign	crOutGbrHi = 0;
assign	crOutLrHi = 0;
`endif

RegCR regCr(
	clock,	exResetL,
//	exHold2,
//	exHold2 && !crIsIsrEdge,
	exHold2NotIsrEdge,

	crIdCm,		//Source ID
	crValCm,		//Source Value

	crIdCn1,		//Destination ID (EX1)
	crValCn1,		//Destination Value (EX1)
	crIdCn2,		//Destination ID (EX2)
	crValCn2,		//Destination Value (EX2)
	crIdCn3,		//Destination ID (EX3)
	crValCn3,		//Destination Value (EX3)
	crIdCn4,		//Destination ID (EX4/WB)
	crValCn4,		//Destination Value (EX4/WB)

	gprEx1Flush,	//Flush EX1
	gprEx2Flush,	//Flush EX2
	gprEx3Flush,	//Flush EX3

	gprValPc,		//PC Value (Synthesized)

	crOutPc,	crInPc,
	crOutLr,	crInLr,
	crOutSr,	crInSr,

	crOutExsr,	crInExsr,
	crOutSpc,	crInSpc,
	crOutSsp,	crInSspB,
	crOutTea,	crInTea,
	crOutTeaHi,	crInTeaHi,
	crOutFpsr,	crInFpsr,

`ifdef jx2_enable_vaddr96
	crOutPcHi,	crInPcHi,
	crOutLrHi,	crInLrHi,
	crOutSpcHi,	crInSpcHi,
`endif

	crOutVbr,
	crOutGbr,
	crOutTbr,

`ifdef jx2_enable_vaddr96
	crOutVbrHi,
	crOutGbrHi,
//	crOutTbrHi,
`endif

	crOutMmcr,
	crOutKrr,
	crOutVipt,
	crOutVbrCm

	);



wire[47:0]		id2RegValSp;		//SP Value (Capture)
wire[63:0]		id2RegValLr;		//LR Value (Capture)
wire[63:0]		id2RegValDlr;		//LR Value (Capture)
wire[63:0]		id2RegValDhr;		//LR Value (Capture)


/* EX1 */	

reg[47:0]		ex1RegValSp;		//SP Value (Capture)
reg[47:0]		ex2RegValSp;		//SP Value (Capture)
reg[47:0]		ex3RegValSp;		//SP Value (Capture)

reg[63:0]		ex1RegValLr;		//LR Value (Capture)
reg[63:0]		ex2RegValLr;		//LR Value (Capture)
reg[63:0]		ex3RegValLr;		//LR Value (Capture)

reg[63:0]		ex1RegValDlr;		//DLR Value (Capture)
reg[63:0]		ex2RegValDlr;		//DLR Value (Capture)
reg[63:0]		ex3RegValDlr;		//DLR Value (Capture)

reg[63:0]		ex1RegValDhr;		//DLR Value (Capture)
reg[63:0]		ex2RegValDhr;		//DLR Value (Capture)
reg[63:0]		ex3RegValDhr;		//DLR Value (Capture)

reg[63:0]		ex2RegInSr;

reg[127:0]		tNxtRegExc;
// reg[127:0]		tRegExc;
reg[127:0]		tNx1RegExc;

reg[11:0]		tRegExcCycInh;
reg[11:0]		tNxtRegExcCycInh;


wire[63:0]		ex1MulVal;
wire[63:0]		ex1MulWVal;

wire			ex1MulFaz;
wire			ex3MulFaz;


// `ifdef jx2_enable_fpu
`wire_gpr		ex1RegIdFRn;
wire[63:0]		ex1RegValFRn;
wire[63:0]		ex1FpuValGRn;
wire[63:0]		exB1FpuValGRn;
wire[63:0]		exC1FpuValGRn;
wire[63:0]		ex1FpuValLdGRn;
wire[1:0]		ex1FpuOK;
wire			ex1FpuSrT;
// `endif

`ifndef jx2_enable_fpu
assign	ex1FpuValGRn	= UV64_XX;
assign	exB1FpuValGRn	= UV64_XX;
assign	ex1FpuValLdGRn	= UV64_XX;
assign	ex1FpuOK		= UMEM_OK_READY;
assign	ex1FpuSrT		= 0;
`endif

`ifndef jx2_use_fpu_w
assign	exB1FpuValGRn	= UV64_XX;
`endif

assign	exC1FpuValGRn	= UV64_XX;

reg[65:0]		ex2MemDataIn;
reg[65:0]		ex2MemDataInB;
reg[1:0]		ex2MemDataOK;

wire[65:0]		ex2MemDataInFast;
assign		ex2MemDataInFast = dcOutValFast;


// wire[47:0]		ex1RegPcHi;
wire[47:0]		ex1RegLrHi;

reg[47:0]		ex1RegPcHi;
// reg[47:0]		ex1RegLrHi;
reg[47:0]		ex1RegGbrHi;

// assign		ex1RegPcHi = crOutPcHi;
assign		ex1RegLrHi = crOutLrHi;

// assign		ex1RegGbrHi = crOutGbrHi;

// reg[31:0]		ex1ValBPc;

reg[8:0]		ex1OpUCmd;
reg[8:0]		ex1OpUIxt;
// wire[1:0]		ex1Hold;
reg[47:0]		ex1PreBraPc;
reg[1:0]		ex1PreBra;
// reg[31:0]	ex1IstrWord;	//source instruction word
reg[95:0]		ex1IstrWord;	//source instruction word
(* max_fanout = 200 *)
	reg				ex1BraFlush;
reg				ex1TrapFlush;
// reg[11:0]		ex1Timers;

reg				ex1PredNoExec;
reg				exB1PredNoExec;
reg				exC1PredNoExec;

`reg_gpr		ex1RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex1RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex1RegIdRm;		//Source C, MemStore

(* max_fanout = 200 *)
	`reg_gprval		ex1RegValRs;		//Source A Value
(* max_fanout = 200 *)
	`reg_gprval		ex1RegValRt;		//Source B Value


`ifndef jx2_cpu_merge_rxy

`reg_gprval		ex1RegValRm;		//Source C Value
`reg_gprval		ex2RegValRm;		//Source C Value
`reg_gprval		ex3RegValRm;		//Source C Value

`else

`wire_gprval		ex1RegValRm;		//Source C Value
`wire_gprval		ex2RegValRm;		//Source C Value
`wire_gprval		ex3RegValRm;		//Source C Value

`ifdef jx2_enable_wex3w
assign		ex1RegValRm = exC1RegValRt;
assign		ex2RegValRm = exC2RegValRt;
assign		ex3RegValRm = exC3RegValRt;
`else
assign		ex1RegValRm = exB1RegValRt;
assign		ex2RegValRm = exB2RegValRt;
assign		ex3RegValRm = exB3RegValRt;
`endif
`endif


`ifndef jx2_cpu_merge_rxy

`reg_gprval		exB1RegValRm;		//Source C Value
`reg_gprval		exB2RegValRm;		//Source C Value
`reg_gprval		exB3RegValRm;		//Source C Value

`else

`wire_gprval		exB1RegValRm;		//Source C Value
`wire_gprval		exB2RegValRm;		//Source C Value
`wire_gprval		exB3RegValRm;		//Source C Value

`ifdef jx2_enable_wex3w
assign		exB1RegValRm = exC1RegValRs;
assign		exB2RegValRm = exC2RegValRs;
assign		exB3RegValRm = exC3RegValRs;
`else
assign		exB1RegValRm = exB1RegValRs;
assign		exB2RegValRm = exB2RegValRs;
assign		exB3RegValRm = exB3RegValRs;
`endif
`endif

`ifdef jx2_cpu_pred_id2
wire		id2_ex1UpdSrT;
wire		id2_isPred;
wire		id2_PredNeedStall;

wire		id2_idA2NeedSrT;
wire		id2_idB2NeedSrT;
wire		id2_idC2NeedSrT;

wire		idA2PredNoExec;
wire		idB2PredNoExec;
wire		idC2PredNoExec;

`ifndef def_true
assign		id2_ex1UpdSrT =
	(ex1OpUCmd[5:0] == JX2_UCMD_OP_IXS)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_OP_IXT)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALU3)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALUW3)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALUB3)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALUCMP)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_UNARY)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_CONV_RR)	||
	(ex1OpUCmd[5:0] == JX2_UCMD_CONV2_RR)	||
	(ex1OpUCmd[5:0] == JX2_UCMD_FCMP)		||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALUCMPW)	||
	(ex1OpUCmd[5:0] == JX2_UCMD_ALUCMPB)	;
`endif

ExPredChkSrT	ex1UpdSr1a(ex1OpUCmd, ex1OpUIxt, id2_ex1UpdSrT);

`ifdef jx2_enable_wex
ExPredChkNeedSrT	id2NeedSr1a(idA2IdUCmd, idA2IdUIxt, id2_idA2NeedSrT);
ExPredChkNeedSrT	id2NeedSr1b(idB2IdUCmd, idB2IdUIxt, id2_idB2NeedSrT);

`ifdef jx2_enable_wex3w
ExPredChkNeedSrT	id2NeedSr1c(idC2IdUCmd, idC2IdUIxt, id2_idC2NeedSrT);

//assign		id2_isPred =
//	idA2IdUCmd[7]	||
//	idB2IdUCmd[7]	||
//	idC2IdUCmd[7]	;

assign		id2_isPred =
	id2_idA2NeedSrT ||
	id2_idB2NeedSrT ||
	id2_idC2NeedSrT ;

`else
//assign		id2_isPred =
//	idA2IdUCmd[7]	||
//	idB2IdUCmd[7]	;
assign		id2_isPred =
	id2_idA2NeedSrT ||
	id2_idB2NeedSrT ;
`endif

`else

// assign		id2_isPred =
//	id2IdUCmd[7]	;

ExPredChkNeedSrT	id2NeedSr1a(id2IdUCmd, Id2OpUIxt, id2_isPred);
`endif

assign	id2_PredNeedStall =
	id2_ex1UpdSrT &&
	!ex1BraFlush &&
	id2_isPred	;
`else

wire		id2_PredNeedStall;
assign	id2_PredNeedStall = 0;

`endif


`ifdef jx2_cpu_pred_id2

`ifdef jx2_enable_wex
ExPredCheck		pred1a(idA2IdUCmd, idA2IdUIxt, ex1RegInSr[7:0], idA2PredNoExec);
ExPredCheck		pred1b(idB2IdUCmd, idB2IdUIxt, ex1RegInSr[7:0], idB2PredNoExec);
`ifdef jx2_enable_wex3w
ExPredCheck		pred1c(idC2IdUCmd, idC2IdUIxt, ex1RegInSr[7:0], idC2PredNoExec);
`else
assign		idC2PredNoExec = 0;
`endif
`else
ExPredCheck		pred1a(id2IdUCmd, id2IdUIxt, ex1RegInSr[7:0], idA2PredNoExec);
`endif

`endif

/* GPR Only: Timing Hack. */
(* max_fanout = 200 *)
	`wire_gprval		ex1RegValRsGpo;		//Source A Value (GPR Only)
(* max_fanout = 200 *)
	`wire_gprval		ex1RegValRtGpo;		//Source B Value (GPR Only)

`ifdef jx2_enable_xgpr
assign	ex1RegValRsGpo = ex1RegIdRs[6] ? UVGPRV_XX : ex1RegValRs;
// assign	ex1RegValRsGpo = ex1RegIdRs[6] ? UVGPRV_00 : ex1RegValRs;
// assign	ex1RegValRsGpo = ex1RegValRs;
// assign	ex1RegValRtGpo = ex1RegIdRt[6] ? UVGPRV_XX : ex1RegValRt;
assign	ex1RegValRtGpo = ex1RegValRt;
`else
// assign	ex1RegValRsGpo = ex1RegIdRs[5] ? UVGPRV_00 : ex1RegValRs;
assign	ex1RegValRsGpo = ex1RegIdRs[5] ? UVGPRV_XX : ex1RegValRs;
assign	ex1RegValRtGpo = ex1RegValRt;
`endif

(* max_fanout = 200 *)
	`reg_gprval		exB1RegValRs;		//Source A Value

// `ifdef jx2_enable_fpu
// `reg_gprval		ex1RegValFRs;		//Source A Value (FPR)
// `reg_gprval		ex1RegValFRt;		//Source B Value (FPR)
// `endif

`reg_gprval		ex1RegValCRm;		//Source C Value (CR)

`wire_gpr		ex1RegIdRn1;		//Destination ID (EX1)
`wire_gprval		ex1RegValRn1;		//Destination Value (EX1)
//wire[4:0]		ex1RegIdCn1;		//Destination ID (CR, EX1)
`wire_gpr		ex1RegIdCn1;		//Destination ID (CR, EX1)
`wire_gprval		ex1RegValCn1;		//Destination Value (CR, EX1)

`wire_gpr		ex1HldIdRn1;		//Held Dest ID (EX1)
// wire[4:0]		ex1HldIdCn1;		//Held Dest ID (CR, EX1)
`wire_gpr		ex1HldIdCn1;		//Held Dest ID (CR, EX1)
	
reg[47:0]		ex1RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex1RegValImm;		//Immediate (Decode)
	
wire[63:0]		ex1RegOutDlr;
reg[63:0]		ex1RegInDlr;
wire[63:0]		ex1RegOutDhr;
reg[63:0]		ex1RegInDhr;
wire[63:0]		ex1RegOutSp;
reg[63:0]		ex1RegInSp;

wire[63:0]		ex1RegOutLr;
reg[63:0]		ex1RegInLr;
wire[63:0]		ex1RegOutSr;
// reg[63:0]		ex1RegInSr;
wire[63:0]		ex1RegInSr;


wire[47:0]		ex1OutPcHi;
wire[47:0]		ex1OutLrHi;
wire[47:0]		ex1OutGbrHi;

`ifndef jx2_enable_vaddr96
assign	ex1OutLrHi = 0;
`endif

// wire[47:0]		ex1MemAddr;
`wire_vaddr		ex1MemAddr;
// wire[ 4:0]		ex1MemOpm;
wire[65:0]		ex1MemDataOut;
wire[65:0]		ex1MemDataOutB;

wire[ 7:0]		ex1RegOutSchm;
reg [ 7:0]		ex1RegInSchm;
wire[127:0]		ex1TrapExc;
reg[127:0]		ex1TrapExcB;

wire[8:0]		ex1OpUCmd2;

wire			ex1AluSrJcmpT;

`wire_gprval		ex1AguOutLea;		//LEA Output
`wire_gprval		ex1AguOutXLea;		//XLEA Output
wire[7:0]		ex1ExfFl;

`wire_gprval		ex1Delay1Out;		//
`reg_gprval		ex2Delay1In;		//
`reg_gprval		exB2Delay1In;		//
`reg_gprval		exC2Delay1In;		//

wire[7:0]		ex2RegOutSchm;
reg[7:0]		ex2RegInSchm;
// reg[7:0]		ex2RegInLastSr;
reg[31:0]		ex2RegInLastSr;

wire[63:0]		ex1AluInSr;
`ifdef jx2_cpu_pred_id2
assign	ex1AluInSr = crOutSr;
// assign	ex1AluInSr = ex1RegInSr;
`else
assign	ex1AluInSr = ex1RegInSr;
`endif

`ifdef jx2_enable_wex3w
assign		ex1ExfFl =
	(exC1OpUCmd[5:0] == 0) ? exC1OpUIxt[7:0] : 0;
`else
assign		ex1ExfFl = 0;
`endif

ExEX1	ex1(
	clock,			exResetL,
	ex1OpUCmd,		ex1OpUIxt,
	ex1Hold,		ex1TrapExc,
	ex1Timers,		ex1OpUCmd2,
	ex1ExfFl,		ex1IstrWord,

	ex1RegIdRs,		ex1RegIdRt,		ex1RegIdRm,
	ex1RegValRs,	ex1RegValRt,	ex1RegValRm,
	exB1RegValRs,
	
	ex1AguOutLea,	ex1AguOutXLea,
	
//	ex1RegValFRs,	ex1RegValFRt,
	ex1RegValCRm,	ex1ValBPc,

	ex1RegIdRn1,	ex1RegValRn1,
	ex1RegIdCn1,	ex1RegValCn1,
	ex1HldIdRn1,	ex1HldIdCn1,

	ex1Delay1Out,

//	ex1RegValPc,

//	{	crOutSr[15:4],
//		crOutSr[27:26],
//		crOutSr[1:0],
//		ex1RegValPc },

//	{	ex1RegInSr[15:4],
//	{	ex1RegInSr[23:20],
//		ex1RegInSr[11: 4],

//	{	ex1RegInSr[15:8],
//		ex1RegInSr[23:20],
//		ex1RegInSr[27:26],
//		ex1RegInSr[ 1: 0],
//		ex1RegValPc },

	{	ex1AluInSr[15:8],
		ex1AluInSr[23:20],
		ex1AluInSr[27:26],
		ex1AluInSr[ 1: 0],
		ex1RegValPc },

	ex1RegValImm,
	ex1FpuValGRn,	ex1FpuSrT,
//	ex1BraFlush,
	ex1BraFlush,	ex1PredNoExec,
//	ex1BraFlush || exResetL,
	ex1PreBraPc,	ex1PreBra,
	ex1AluSrJcmpT,
	ex2RegInLastSr[7:0],
	
//	ifValPcHi,		ex1RegGbrHi,
	
	ex1RegOutDlr,	ex1RegInDlr,
	ex1RegOutDhr,	ex1RegInDhr,
	ex1RegOutSp,	ex1RegInSp,

	ex1RegOutLr,	ex1RegInLr,
	ex1RegOutSr,	ex1RegInSr,
	ex1RegOutSchm,	ex1RegInSchm,

`ifdef jx2_enable_vaddr96
//	ex1OutPcHi,		crOutPcHi,
//	ex1OutLrHi,		crOutLrHi,
	ex1OutPcHi,		ex1RegPcHi,
	ex1OutLrHi,		ex1RegLrHi,
	ex1OutGbrHi,	ex1RegGbrHi,
`endif

	ex1MemAddr,		ex1MemOpm,
	ex1MemDataOut,	ex1MemDataOutB,
	ex1MemLdOp,
	ex2MemDataOK,	tRegExc[63:0]
	);

wire[7:0]	exB1ValCarryD;

// wire[65:0]	ex1ValAlu;
wire[72:0]	ex1ValAlu;
// wire		ex1AluSrT;

ExALU	exAlu(
	clock,				exResetL,
	ex1RegValRs[63:0],	ex1RegValRt[63:0],
	exB1RegValRs[63:0],	ex1RegValRm[63:0],
	ex1OpUCmd,			ex1OpUIxt,
//	exHold2,			ex1RegInSr[1:0],
	exHold2,			{ ex1AluInSr[7:4], ex1AluInSr[1:0] },

//`ifdef jx2_cpu_pred_id2
//	exHold2,			{ crOutSr[7:4], crOutSr[1:0] },
//`else
//	exHold2,			{ ex1RegInSr[7:4], ex1RegInSr[1:0] },
//`endif
//	ex1ValAlu,			ex1AluSrT);
//	ex1ValAlu[63:0],	ex1ValAlu[65:64]);
	ex1ValAlu[63:0],	ex1ValAlu[72:64],
	exB1ValCarryD,		ex1AluSrJcmpT);

// ExMul	ex1Mul(
// ExMulB	ex1Mul(
ExMulC	ex1Mul(
	clock,				exResetL,
	ex1RegValRsGpo[63:0],
	ex1RegValRtGpo[63:0],
	ex1RegValRm[63:0],
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1MulVal,
	ex1MulFaz,			ex3MulFaz
	);

ExMulW	ex1MulW(
	clock,				exResetL,
//	ex1RegValRs,		ex1RegValRt,
	ex1RegValRsGpo[63:0],		ex1RegValRtGpo[63:0],
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1MulWVal,
	0
	);

wire[65:0]				ex1KrreLo;
wire[65:0]				ex1KrreHi;

// `ifdef def_true
`ifdef jx2_enable_ldekey
// isGpu

wire[8:0]		krrEx1OpUCmd = isGpu ? 0 : ex1OpUCmd;
wire[8:0]		krrEx1OpUIxt = isGpu ? 0 : ex1OpUIxt;

ExModKrrEnc	ex1KrrEnc(
	clock,				exResetL,
	krrEx1OpUCmd,		krrEx1OpUIxt,
//	exHold2,			ex1BraFlush,
	exHold2,			ex1BraFlush || ex1PredNoExec,
	ex1RegInDlr,		ex1RegInDhr,	crOutKrr,
	ex1KrreLo,			ex1KrreHi);
`else
// assign		ex1KrreLo = UV66_00;
assign		ex1KrreLo = {
		((ex1OpUCmd[5:0]==JX2_UCMD_OP_IXT) &&
		(ex1OpUIxt[5:0]==JX2_UCIX_IXT_LDEKRR)) ? 2'b11 : 2'b00,
		UV64_00 };
assign		ex1KrreHi = UV66_00;
`endif

wire[63:0]				exA1FpuV4SfRn;
wire[63:0]				exB1FpuV4SfRn;

reg[8:0]		exB1OpUCmd;
reg[8:0]		exB1OpUIxt;
reg[32:0]		exB1RegValImm;		//Immediate (Decode)

`reg_gpr		exB1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exB1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exB1RegIdRm;		//Source C, MemStore
// `reg_gprval		exB1RegValRs;		//Source A Value

(* max_fanout = 200 *)
	`reg_gprval		exB1RegValRt;		//Source B Value

//`reg_gprval		exB1RegValRm;		//Source C Value



`ifdef jx2_enable_fpu

`ifdef jx2_use_fpu_w

wire[8:0]		fpuExA1OpUCmd;
wire[8:0]		fpuExA1OpUIxt;
wire[32:0]		fpuExA1RegValImm;		//Immediate (Decode)

wire[8:0]		fpuExB1OpUCmd;
wire[8:0]		fpuExB1OpUIxt;
wire[32:0]		fpuExB1RegValImm;		//Immediate (Decode)

assign		fpuExA1OpUCmd		= fpuLowPrec ? 0 : ex1OpUCmd;
assign		fpuExA1OpUIxt		= fpuLowPrec ? 0 : ex1OpUIxt;
assign		fpuExA1RegValImm	= fpuLowPrec ? 0 : ex1RegValImm;

assign		fpuExB1OpUCmd		= fpuLowPrec ? 0 : exB1OpUCmd;
assign		fpuExB1OpUIxt		= fpuLowPrec ? 0 : exB1OpUIxt;
assign		fpuExB1RegValImm	= fpuLowPrec ? 0 : exB1RegValImm;

assign	ex1FpuValLdGRn = UV64_00;

wire[63:0]		ex1FpuValGRnB;
wire[63:0]		exB1FpuValGRnB;
wire[1:0]		ex1FpuOKB;

wire[7:0]		ex1FpuTrapB;

assign	ex1FpuValGRn	= fpuLowPrec ? 0 : ex1FpuValGRnB;
assign	exB1FpuValGRn	= fpuLowPrec ? 0 : exB1FpuValGRnB;
assign	ex1FpuOK		= fpuLowPrec ? 0 : ex1FpuOKB;


FpuExOpW	ex1Fpu(
	clock,			exResetL,

	fpuExA1OpUCmd,		fpuExA1OpUIxt,
	fpuExB1OpUCmd,		fpuExB1OpUIxt,
	fpuExA1RegValImm,	fpuExB1RegValImm,

	ex1RegIdRs,		ex1RegValRs[63:0],
	ex1RegIdRt,		ex1RegValRt[63:0],
	ex1RegIdRm,		ex1RegValRm[63:0],

	exB1RegIdRs,	exB1RegValRs[63:0],
	exB1RegIdRt,	exB1RegValRt[63:0],
	exB1RegIdRm,	exB1RegValRm[63:0],

	ex1FpuOKB,		ex1FpuSrT,
	crOutFpsr,		crInFpsr,
	
//	ex2RegInSr,
	ex1AluInSr,
//	ex1BraFlush || exResetL,
	ex1BraFlush,
//	ex1BraFlush || ex1PredNoExec,
	exHold2,		ex1FpuTrapB,
	
	ex1FpuValGRnB,	exB1FpuValGRnB
	);

`ifdef jx2_use_fpu_v4sf
FpuVec4SF	ex1FpuV4(
	clock,			exResetL,

	ex1OpUCmd,		ex1OpUIxt,
	exB1OpUCmd,		exB1OpUIxt,
`ifdef jx2_enable_wex3w
	exC1OpUCmd,		exC1OpUIxt,
`else
	UV9_00, UV9_00,
`endif
	exHold2,

	ex1RegValImm,	exB1RegValImm,

	ex1RegIdRs,		ex1RegValRs[63:0],
	exB1RegIdRs,	exB1RegValRs[63:0],
	ex1RegIdRt,		ex1RegValRt[63:0],
	exB1RegIdRt,	exB1RegValRt[63:0],

	ex1RegValRm[63:0],	exB1RegValRm[63:0],

	exA1FpuV4SfRn,	exB1FpuV4SfRn);
`else
assign	exA1FpuV4SfRn = 0;
assign	exB1FpuV4SfRn = 0;
`endif

`else

wire[7:0]		ex1FpuTrapB;

assign		ex1FpuTrapB = 0;

assign		crInFpsr = crOutFpsr;

FpuExOp	ex1Fpu(
	clock,			exResetL,

	ex1OpUCmd,		ex1OpUIxt,
	ex1RegIdRs,		ex1RegValRs,
	ex1RegIdRt,		ex1RegValRt,
	ex1RegIdRm,		ex1RegValRs,
	ex1RegIdFRn,	ex1RegValFRn,
	ex1FpuOK,		ex1FpuSrT,
	
	ex2RegInSr,
//	ex1BraFlush || exResetL,
	ex1BraFlush,
//	ex1BraFlush || (ex1PredNoExec && exB1PredNoExec),
	exHold2,
	
	ex1RegValRs,	ex1FpuValGRn,
	ex1FpuValLdGRn,
	ex2MemDataIn,	ex2MemDataOK
	);

`ifdef jx2_use_fpu_v4sf

// `ifdef jx2_enable_wex
`ifdef jx2_use_fpu_v4sf_wx
FpuVec4SF	ex1FpuV4(
	clock,			exResetL,

	ex1OpUCmd,		ex1OpUIxt,
	exB1OpUCmd,		exB1OpUIxt,
	exC1OpUCmd,		exC1OpUIxt,
	exHold2,

	ex1RegValImm,	exB1RegValImm,

	ex1RegIdRs,		ex1RegValRs,
	exB1RegIdRs,	exB1RegValRs,
	ex1RegIdRt,		ex1RegValRt,
	exB1RegIdRt,	exB1RegValRt,

	ex1RegValRm,	exB1RegValRm,

	exA1FpuV4SfRn,	exB1FpuV4SfRn);
`else
FpuVec4SF	ex1FpuV4(
	clock,			exResetL,

	ex1OpUCmd,		ex1OpUIxt,
	UV9_00,			UV9_00,
	UV9_00,			UV9_00,
	exHold2,

	ex1RegValImm,	UV33_00,

	ex1RegIdRs,		ex1RegValRs,
	JX2_GR_ZZR,		UV64_00,
	ex1RegIdRt,		ex1RegValRt,
	JX2_GR_ZZR,		UV64_00,

	ex1RegValRm,	UV64_00,

	exA1FpuV4SfRn,	exB1FpuV4SfRn);
`endif

`else
assign	exA1FpuV4SfRn = 0;
assign	exB1FpuV4SfRn = 0;
`endif

`endif

`else

assign	exA1FpuV4SfRn = 0;
assign	exB1FpuV4SfRn = 0;
assign		crInFpsr = crOutFpsr;


`endif


wire		ex1BlintDoHold;
wire[63:0]	ex1BlintVal;

`ifdef jx2_enable_blint
ExModBlint	ex1Blint(
	clock,			exResetL,
	ex1OpUCmd,		ex1OpUIxt,
	exHold2,		ex1BlintDoHold,
	ex1RegValRsGpo,	ex1RegValRtGpo,
	exB1RegValRs,	exB1RegValRt,
	ex1RegValRm,	exB1RegValRm,
	ex1BlintVal		);
`else
assign		ex1BlintDoHold = 0;
assign		ex1BlintVal = 0;
`endif


wire		ex1SloMulDoHold;
wire[63:0]	ex1SloMulVal;
wire[63:0]	ex1SloMulValHi;

wire[7:0]	ex1SloMuTrap;

`ifdef jx2_alu_slomuldiv

// isGpu
wire[8:0]	smulEx1OpUCmd	= isGpu ? 0 : ex1OpUCmd;
wire[8:0]	smulEx1OpUIxt	= isGpu ? 0 : ex1OpUIxt;

ExOpSloMulDiv	ex1SloMul(
	clock,			exResetL,
	smulEx1OpUCmd,	smulEx1OpUIxt,
	ex1RegValRsGpo[63:0],	ex1RegValRtGpo[63:0],
	ex1SloMulVal,	ex1SloMulValHi,
	exHold2,		ex1SloMulDoHold,
	ex1MulFaz,		ex1SloMuTrap
	);

`else

assign		ex1SloMulDoHold	= 0;
assign		ex1SloMulVal	= 0;
assign		ex1SloMulValHi	= 0;
assign		ex1SloMuTrap	= 0;

`endif


/* EX2 */

reg[47:0]		ex2ValBPc;

reg[8:0]		ex2OpUCmd;
reg[8:0]		ex2OpUIxt;
// wire[1:0]		ex2Hold;
reg[47:0]		ex2PreBraPc;
reg[1:0]		ex2PreBra;
// reg[31:0]	ex2IstrWord;	//source instruction word
reg[95:0]		ex2IstrWord;	//source instruction word

`reg_gpr		ex2RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex2RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex2RegIdRm;		//Source C, MemStore
`reg_gprval		ex2RegValRs;		//Source A Value
`reg_gprval		ex2RegValRt;		//Source B Value

// `reg_gprval		ex2RegValRm;		//Source C Value

//`reg_gprval		ex2RegValFRs;		//Source A Value (FPR)
//`reg_gprval		ex2RegValFRt;		//Source B Value (FPR)
`reg_gprval		ex2RegValCRm;		//Source C Value (CR)

`reg_gpr		ex2RegIdRn1;		//Destination ID (EX1)
`reg_gprval		ex2RegValRn1;		//Destination Value (EX1)
// reg[4:0]		ex2RegIdCn1;		//Destination ID (CR, EX1)
`reg_gpr		ex2RegIdCn1;		//Destination ID (CR, EX1)
`reg_gprval		ex2RegValCn1;		//Destination Value (CR, EX1)
	
`wire_gpr		ex2RegIdRn2;		//Destination ID (EX1)
`wire_gprval		ex2RegValRn2;		//Destination Value (EX1)
//wire[4:0]		ex2RegIdCn2;		//Destination ID (CR, EX1)
`wire_gpr		ex2RegIdCn2;		//Destination ID (CR, EX1)
`wire_gprval		ex2RegValCn2;		//Destination Value (CR, EX1)
	
reg[47:0]		ex2RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex2RegValImm;		//Immediate (Decode)
// reg[65:0]		ex2RegAluRes;		//Arithmetic Result
reg[72:0]		ex2RegAluRes;		//Arithmetic Result
reg[63:0]		ex2RegMulRes;		//Multiplier Result
reg[63:0]		ex2RegMulWRes;		//Multiplier Result (Word)
// reg[63:0]		ex2RegFpuGRn;		//FPU GPR Result

reg[65:0]		ex2RegAluResB;		//Arithmetic Result (ALUB)

(* max_fanout = 200 *)
	reg				ex2BraFlush;		//Flush EX2
reg				ex2TrapFlush;		//Flush EX2

wire[63:0]		ex2RegOutDlr;
reg[63:0]		ex2RegInDlr;
wire[63:0]		ex2RegOutDhr;
reg[63:0]		ex2RegInDhr;
wire[63:0]		ex2RegOutSp;
reg[63:0]		ex2RegInSp;

wire[63:0]		ex2RegOutLr;
reg[63:0]		ex2RegInLr;
wire[63:0]		ex2RegOutSr;
// reg[63:0]		ex2RegInSr;

//wire[7:0]		ex2RegOutSchm;
//reg[7:0]		ex2RegInSchm;
//reg[7:0]		ex2RegInLastSr;

// reg[63:0]		ex2MemDataIn;
// reg[1:0]		ex2MemDataOK;

`reg_gprval		ex2AguOutLea;		//LEA Output
`reg_gprval		ex2AguOutXLea;		//XLEA Output

ExEX2	ex2(
	clock,			exResetL,
	ex2OpUCmd,		ex2OpUIxt,
	ex2Hold,

	ex2RegIdRs,		ex2RegIdRt,		ex2RegIdRm,
	ex2RegValRs,	ex2RegValRt,	ex2RegValRm,

	ex2AguOutLea,

//	ex2RegValFRs,	ex2RegValFRt,
	ex2RegValCRm,

	ex2RegIdRn1,	ex2RegValRn1,
	ex2RegIdCn1,	ex2RegValCn1,

	ex2RegIdRn2,	ex2RegValRn2,
	ex2RegIdCn2,	ex2RegValCn2,
	
	ex2RegValPc,	ex2RegValImm,
	ex2RegAluRes,	ex2RegMulRes,
	ex2RegMulWRes,	ex1KrreLo,
	ex2RegAluResB,	ex1BlintVal,
	ex1SloMulVal,
	ex2Delay1In,

	ex1FpuValGRn,	ex1FpuValLdGRn,
	ex1FpuSrT,		ex1FpuOK,
	
	ex2BraFlush,
//	ex2BraFlush || exResetL,
	ex2RegInLastSr[7:0],
	
	ex2RegOutDlr,	ex2RegInDlr,
	ex2RegOutDhr,	ex2RegInDhr,
	ex2RegOutSp,	ex2RegInSp,

	ex2RegOutLr,	ex2RegInLr,
	ex2RegOutSr,	ex2RegInSr,
	ex2RegOutSchm,	ex2RegInSchm,

	ex2MemDataIn,
	ex2MemDataInB,
	ex2MemDataOK,
	ex2MemDataInFast
	);

assign		ex1RegInSr = ex2RegOutSr;


/* EX3 */

`reg_gpr		ewbRegIdRm;		//Destination Register, Writeback
`reg_gpr		ewb2RegIdRm;	//Destination Register, Writeback
`reg_gpr		ewb3RegIdRm;	//Destination Register, Writeback


reg[47:0]		ex3ValBPc;

reg[8:0]		ex3OpUCmd;
reg[8:0]		ex3OpUIxt;
// wire[1:0]		ex3Hold;
reg[1:0]		ex3PreBra;
// reg[31:0]	ex3IstrWord;	//source instruction word
reg[95:0]		ex3IstrWord;	//source instruction word

`reg_gpr		ex3RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex3RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex3RegIdRm;		//Source C, MemStore
`reg_gprval		ex3RegValRs;		//Source A Value
`reg_gprval		ex3RegValRt;		//Source B Value

// `reg_gprval		ex3RegValRm;		//Source C Value


`reg_gpr		ex3RegIdRn2;		//Destination ID (EX2)
`reg_gprval		ex3RegValRn2;		//Destination Value (EX2)
//reg[4:0]		ex3RegIdCn2;		//Destination ID (CR, EX2)
`reg_gpr		ex3RegIdCn2;		//Destination ID (CR, EX2)
`reg_gprval		ex3RegValCn2;		//Destination Value (CR, EX2)
	
`wire_gpr		ex3RegIdRn3;		//Destination ID (EX3)
`wire_gprval		ex3RegValRn3;		//Destination Value (EX3)
`wire_gpr		ex3RegIdRn4;		//Destination ID (EX3)
`wire_gprval		ex3RegValRn4;		//Destination Value (EX3)

//wire[4:0]		ex3RegIdCn3;		//Destination ID (CR, EX3)
`wire_gpr		ex3RegIdCn3;		//Destination ID (CR, EX3)
`wire_gprval		ex3RegValCn3;		//Destination Value (CR, EX3)
	
reg[47:0]		ex3RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex3RegValImm;		//Immediate (Decode)
//reg[65:0]		ex3RegAluRes;		//Arithmetic Result
reg[69:0]		ex3RegAluRes;		//Arithmetic Result
reg[63:0]		ex3RegMulRes;		//Multiplier Result
reg[63:0]		ex3RegMulWRes;		//Multiplier Result (Word)
(* max_fanout = 200 *)
	reg				ex3BraFlush;		//Flush EX3
reg				ex3TrapFlush;		//Flush EX3

// reg[7:0]		ex3RegInLastSr;
reg[31:0]		ex3RegInLastSr;

wire[63:0]		ex3RegOutDlr;
reg[63:0]		ex3RegInDlr;
wire[63:0]		ex3RegOutDhr;
reg[63:0]		ex3RegInDhr;
wire[63:0]		ex3RegOutLr;
reg[63:0]		ex3RegInLr;

ExEX3	ex3(
	clock,			exResetL,
	ex3OpUCmd,		ex3OpUIxt,
	ex3Hold,		exHold2,

	ex3RegIdRs,		ex3RegIdRt,		ex3RegIdRm,
	ex3RegValRs,	ex3RegValRt,	ex3RegValRm,

	ex3RegIdRn2,	ex3RegValRn2,
	ex3RegIdCn2,	ex3RegValCn2,

	ex3RegIdRn3,	ex3RegValRn3,
	ex3RegIdCn3,	ex3RegValCn3,

	ex3RegIdRn4,	ex3RegValRn4,
	
	ex3RegValPc,	ex3RegValImm,
	ex3RegAluRes,	ex3RegMulRes,
	ex3RegMulWRes,	ex1KrreLo,
	ex1FpuValGRn,
	exA1FpuV4SfRn,

//	ex3BraFlush || exResetL,
	ex3BraFlush,

	ex3RegInLastSr[7:0],
	ex3MulFaz,

	ex2MemDataIn,
	ex2MemDataInB,
	ex2MemDataOK
	);

`ifdef jx2_enable_wex
/* EX1, Lane 2 */

wire[63:0]		exB1MulWVal;

`ifndef jx2_use_fpu_w
// reg[8:0]		exB1OpUCmd;
// reg[8:0]		exB1OpUIxt;
// reg[32:0]		exB1RegValImm;		//Immediate (Decode)

// `reg_gpr		exB1RegIdRs;		//Source A, ALU / Base
// `reg_gpr		exB1RegIdRt;		//Source B, ALU / Index
// `reg_gpr		exB1RegIdRm;		//Source C, MemStore
// reg[63:0]		exB1RegValRs;		//Source A Value
// `reg_gprval		exB1RegValRt;		//Source B Value
// `reg_gprval		exB1RegValRm;		//Source C Value
`endif

// wire[1:0]		exB1Hold;

`wire_gpr		exB1RegIdRn1;		//Destination ID (EX1)
`wire_gprval		exB1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exB1HldIdRn1;		//Held Dest ID (EX1)

//reg[32:0]		exB1RegValImm;		//Immediate (Decode)

wire[8:0]		exB1OpUCmd2;

`wire_gprval		exB1AguXLea;		//XLEA Output
assign		exB1AguXLea = ex1AguOutXLea;

wire[47:0]			exB1MemAddr;

`wire_gprval		exB1Delay1Out;		//
`wire_gprval		exC1Delay1Out;		//


ExEXB1	exb1(
	clock,			exResetL,
	exB1OpUCmd,		exB1OpUIxt,
	exB1Hold,		exB1OpUCmd2,
	
	exB1RegIdRs,	exB1RegIdRt,
	exB1RegIdRm,	exB1RegValRs,
	exB1RegValRt,	exB1RegValRm,
	ex1RegValRs,	exB1AguXLea,

	exB1RegIdRn1,	exB1RegValRn1,
	exB1HldIdRn1,
	
	exB1Delay1Out,
	
	ex1RegValPc,	exB1RegValImm,
//	ex1BraFlush,
	ex1BraFlush,	exB1PredNoExec,
//	ex1BraFlush || exResetL,
//`ifdef jx2_cpu_pred_id2
	ex1AluInSr,
//`else
//	ex1RegInSr,
//`endif
	1,
	exB1MemAddr, exB1MemOpm
	);
	

wire[65:0]	exB1ValAlu;
// wire[7:0]	exB1ValCarryD;

// ExALU	exAluB(
ExALUB	exAluB(
	clock,				exResetL,
	exB1RegValRs[63:0],	exB1RegValRt[63:0],
	exB1OpUCmd,			exB1OpUIxt,
	exHold2,			ex1AluInSr[1:0],
//	exHold2,			ex1RegInSr[1:0],
	exB1ValAlu[63:0],	exB1ValAlu[65:64],
	exB1ValCarryD,		1);

ExMulW	exB1MulW(
	clock,				exResetL,
	exB1RegValRs[63:0],	exB1RegValRt[63:0],
	exB1OpUCmd,			exB1OpUIxt,
	exHold2,			exB1MulWVal,
	1
	);

`else

assign	exB1ValCarryD = 0;

`endif

`ifdef jx2_enable_wex

/* EX2, Lane 2 */

reg[8:0]		exB2OpUCmd;
reg[8:0]		exB2OpUIxt;
// wire[1:0]		exB2Hold;

`reg_gpr		exB2RegIdRs;		//Source A, ALU / Base
`reg_gpr		exB2RegIdRt;		//Source B, ALU / Index
`reg_gpr		exB2RegIdRm;		//Source C, MemStore
`reg_gprval		exB2RegValRs;		//Source A Value
`reg_gprval		exB2RegValRt;		//Source B Value

// `reg_gprval		exB2RegValRm;		//Source C Value


`reg_gpr		exB2RegIdRn1;		//Destination ID (EX1)
`reg_gprval		exB2RegValRn1;		//Destination Value (EX1)
`wire_gpr		exB2RegIdRn2;		//Destination ID (EX1)
`wire_gprval		exB2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exB2RegValImm;		//Immediate (Decode)
reg[65:0]		exB2RegAluRes;		//Arithmetic Result
reg[63:0]		exB2RegMulWRes;		//Word Multiply Result

ExEXB2		exb2(
	clock,		exResetL,
	exB2OpUCmd,	exB2OpUIxt,
	exB2Hold,
	
	exB2RegIdRs,	exB2RegIdRt,
	exB2RegIdRm,	exB2RegValRs,
	exB2RegValRt,	exB2RegValRm,

	ex2AguOutXLea,

	exB2RegIdRn1,	exB2RegValRn1,
	exB2RegIdRn2,	exB2RegValRn2,
	
	ex2RegValPc,
	exB2RegValImm,	exB2RegAluRes,
	exB2RegMulWRes,	ex1KrreHi,

	exB2Delay1In,

	exB1FpuValGRn,
	ex2BraFlush,
//	ex2BraFlush || exResetL,

	ex2RegInLastSr[7:0],	ex2RegInSr,
	ex2MemDataIn,			ex2MemDataInB
	);

reg[8:0]		exB3OpUCmd;
reg[8:0]		exB3OpUIxt;
// wire[1:0]		exB3Hold;

`reg_gpr		exB3RegIdRs;		//Source A, ALU / Base
`reg_gpr		exB3RegIdRt;		//Source B, ALU / Index
`reg_gpr		exB3RegIdRm;		//Source C, MemStore
`reg_gprval		exB3RegValRs;		//Source A Value
`reg_gprval		exB3RegValRt;		//Source B Value
// `reg_gprval		exB3RegValRm;		//Source C Value


`reg_gpr		exB3RegIdRn2;		//Destination ID (EX1)
`reg_gprval		exB3RegValRn2;		//Destination Value (EX1)
`wire_gpr		exB3RegIdRn3;		//Destination ID (EX1)
`wire_gprval		exB3RegValRn3;		//Destination Value (EX1)
`wire_gpr		exB3RegIdRn4;		//Destination ID (EX1)
`wire_gprval		exB3RegValRn4;		//Destination Value (EX1)
	
reg[32:0]		exB3RegValImm;		//Immediate (Decode)
reg[65:0]		exB3RegAluRes;		//Arithmetic Result
reg[63:0]		exB3RegMulWRes;		//Word Multiply Result

ExEXB3		exb3(
	clock,		exResetL,
	exB3OpUCmd,	exB3OpUIxt,
	exB3Hold,
	
	exB3RegIdRs,	exB3RegIdRt,
	exB3RegIdRm,	exB3RegValRs,
	exB3RegValRt,	exB3RegValRm,
	exB3RegIdRn2,	exB3RegValRn2,
	exB3RegIdRn3,	exB3RegValRn3,
	exB3RegIdRn4,	exB3RegValRn4,
	
	ex3RegValPc,
	exB3RegValImm,	exB3RegAluRes,
	exB3RegMulWRes,	ex1KrreHi,
	exB1FpuValGRn,
	exB1FpuV4SfRn,

//	ex3BraFlush || exResetL,
	ex3BraFlush,

	ex3RegInLastSr[7:0],
	ex2MemDataIn,	ex2MemDataInB
	);

`endif


`ifdef jx2_enable_wex3w
/* EX1, Lane 3 */

wire[63:0]		exC1MulWVal;

reg[8:0]		exC1OpUCmd;
reg[8:0]		exC1OpUIxt;
// wire[1:0]		exC1Hold;

`reg_gpr		exC1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exC1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exC1RegIdRm;		//Source C, MemStore
(* max_fanout = 200 *)
	`reg_gprval		exC1RegValRs;		//Source A Value
(* max_fanout = 200 *)
	`reg_gprval		exC1RegValRt;		//Source B Value

`reg_gprval		exC1RegValRm;		//Source C Value

`wire_gpr		exC1RegIdRn1;		//Destination ID (EX1)
`wire_gprval		exC1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exC1HldIdRn1;		//Held Dest ID (EX1)

reg[32:0]		exC1RegValImm;		//Immediate (Decode)
wire[8:0]		exC1OpUCmd2;

ExEXC1	exc1(
	clock,			exResetL,
	exC1OpUCmd,		exC1OpUIxt,
	exC1Hold,		exC1OpUCmd2,
	
	exC1RegIdRs,	exC1RegIdRt,
	exC1RegIdRm,	exC1RegValRs,
	exC1RegValRt,	exC1RegValRm,
	UVGPRV_00,

	exC1RegIdRn1,	exC1RegValRn1,
	exC1HldIdRn1,

	exC1Delay1Out,

	ex1RegValPc,	exC1RegValImm,
//	ex1BraFlush,	ex1RegInSr
//	ex1BraFlush || exResetL,
//	ex1BraFlush,
	ex1BraFlush,	exC1PredNoExec,
//`ifdef jx2_cpu_pred_id2
	ex1AluInSr,
//`else
//	ex1RegInSr,
//`endif
	2
	);
	

wire[65:0]	exC1ValAlu;
wire[ 7:0]	exC1ValCarryD;

// ExALU	exAluC(
ExALUB	exAluC(
	clock,				exResetL,
	exC1RegValRs[63:0],	exC1RegValRt[63:0],
	exC1OpUCmd,			exC1OpUIxt,
	exHold2,			ex1AluInSr[1:0],
//	exHold2,			ex1RegInSr[1:0],
	exC1ValAlu[63:0],	exC1ValAlu[65:64],
	exC1ValCarryD,		2);

`ifdef jx2_mulw_nolane3
assign exC1MulWVal = 0;
`else
ExMulW	exC1MulW(
	clock,				exResetL,
	exC1RegValRs[63:0],	exC1RegValRt[63:0],
	exC1OpUCmd,			exC1OpUIxt,
	exHold2,			exC1MulWVal,
	2
	);
`endif

`endif

`ifdef jx2_enable_wex3w

/* EX2, Lane 3 */

reg[8:0]		exC2OpUCmd;
reg[8:0]		exC2OpUIxt;
// wire[1:0]		exC2Hold;

`reg_gpr		exC2RegIdRs;		//Source A, ALU / Base
`reg_gpr		exC2RegIdRt;		//Source B, ALU / Index
`reg_gpr		exC2RegIdRm;		//Source C, MemStore
`reg_gprval		exC2RegValRs;		//Source A Value
`reg_gprval		exC2RegValRt;		//Source B Value
`reg_gprval		exC2RegValRm;		//Source C Value

`reg_gpr		exC2RegIdRn1;		//Destination ID (EX1)
`reg_gprval		exC2RegValRn1;		//Destination Value (EX1)
`wire_gpr		exC2RegIdRn2;		//Destination ID (EX1)
`wire_gprval		exC2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exC2RegValImm;		//Immediate (Decode)
reg[65:0]		exC2RegAluRes;		//Arithmetic Result
reg[63:0]		exC2RegMulWRes;		//Word Multiply Result

ExEXC2		exc2(
	clock,		exResetL,
	exC2OpUCmd,	exC2OpUIxt,
	exC2Hold,
	
	exC2RegIdRs,	exC2RegIdRt,
	exC2RegIdRm,	exC2RegValRs,
	exC2RegValRt,	exC2RegValRm,
	exC2RegIdRn1,	exC2RegValRn1,
	exC2RegIdRn2,	exC2RegValRn2,
	
	ex2RegValPc,
	exC2RegValImm,	exC2RegAluRes,
	exC2RegMulWRes,	ex1KrreHi,

	exC2Delay1In,

	exC1FpuValGRn,
	ex2BraFlush,
//	ex2BraFlush || exResetL,

	ex2RegInLastSr[7:0],	ex2RegInSr,
	ex2MemDataIn,			ex2MemDataInB
	);

reg[8:0]		exC3OpUCmd;
reg[8:0]		exC3OpUIxt;
// wire[1:0]		exC3Hold;

`reg_gpr		exC3RegIdRs;		//Source A, ALU / Base
`reg_gpr		exC3RegIdRt;		//Source B, ALU / Index
`reg_gpr		exC3RegIdRm;		//Source C, MemStore
`reg_gprval		exC3RegValRs;		//Source A Value
`reg_gprval		exC3RegValRt;		//Source B Value
`reg_gprval		exC3RegValRm;		//Source C Value

`reg_gpr		exC3RegIdRn2;		//Destination ID (EX1)
`reg_gprval		exC3RegValRn2;		//Destination Value (EX1)
`wire_gpr		exC3RegIdRn3;		//Destination ID (EX1)
`wire_gprval		exC3RegValRn3;		//Destination Value (EX1)
`wire_gpr		exC3RegIdRn4;		//Destination ID (EX1)
`wire_gprval		exC3RegValRn4;		//Destination Value (EX1)
	
reg[32:0]		exC3RegValImm;		//Immediate (Decode)
reg[65:0]		exC3RegAluRes;		//Arithmetic Result
reg[63:0]		exC3RegMulWRes;		//Word Multiply Result

ExEXC3		exc3(
	clock,		exResetL,
	exC3OpUCmd,	exB3OpUIxt,
	exC3Hold,
	
	exC3RegIdRs,	exC3RegIdRt,
	exC3RegIdRm,	exC3RegValRs,
	exC3RegValRt,	exC3RegValRm,
	exC3RegIdRn2,	exC3RegValRn2,
	exC3RegIdRn3,	exC3RegValRn3,
	exC3RegIdRn4,	exC3RegValRn4,
	
	ex3RegValPc,
	exC3RegValImm,	exB3RegAluRes,
	exC3RegMulWRes,	ex1KrreHi,
	exC1FpuValGRn,
//	ex3BraFlush || exResetL,
	ex3BraFlush,

	ex3RegInLastSr[7:0],
	ex2MemDataIn,	ex2MemDataInB
	);

`endif


reg[47:0]		exL4ValBPc;

reg[8:0]		exL4OpUCmd;
reg[8:0]		exL4OpUIxt;
wire[1:0]		exL4Hold;
reg[1:0]		exL4PreBra;
// reg[31:0]	exL4IstrWord;	//source instruction word
reg[95:0]		exL4IstrWord;	//source instruction word

`reg_gpr		exL4RegIdRs;		//Source A, ALU / Base
`reg_gpr		exL4RegIdRt;		//Source B, ALU / Index
`reg_gpr		exL4RegIdRm;		//Source C, MemStore
`reg_gprval		exL4RegValRs;		//Source A Value
`reg_gprval		exL4RegValRt;		//Source B Value
`reg_gprval		exL4RegValRm;		//Source C Value

`reg_gpr		exL4RegIdRn2;		//Destination ID (EX2)
`reg_gprval		exL4RegValRn2;		//Destination Value (EX2)
`reg_gpr		exL4RegIdCn2;		//Destination ID (CR, EX2)
`reg_gprval		exL4RegValCn2;		//Destination Value (CR, EX2)
		
reg[47:0]		exL4RegValPc;		//PC Value (Synthesized)
reg[32:0]		exL4RegValImm;		//Immediate (Decode)
reg				exL4BraFlush;		//Flush EX2
reg				exL4TrapFlush;		//Flush EX2

`ifndef def_true
// reg[7:0]		exL4RegInLastSr;
reg[31:0]		exL4RegInSr;

reg[63:0]		exL4RegInDlr;
reg[63:0]		exL4RegInDhr;
reg[63:0]		exL4RegInLr;
reg[63:0]		exL4RegInSp;
`endif



reg[47:0]		exL5ValBPc;

reg[8:0]		exL5OpUCmd;
reg[8:0]		exL5OpUIxt;
wire[1:0]		exL5Hold;
reg[1:0]		exL5PreBra;
reg[95:0]		exL5IstrWord;	//source instruction word

`reg_gpr		exL5RegIdRs;		//Source A, ALU / Base
`reg_gpr		exL5RegIdRt;		//Source B, ALU / Index
`reg_gpr		exL5RegIdRm;		//Source C, MemStore
`reg_gprval		exL5RegValRs;		//Source A Value
`reg_gprval		exL5RegValRt;		//Source B Value
`reg_gprval		exL5RegValRm;		//Source C Value

`reg_gpr		exL5RegIdRn2;		//Destination ID (EX2)
`reg_gprval		exL5RegValRn2;		//Destination Value (EX2)
`reg_gpr		exL5RegIdCn2;		//Destination ID (CR, EX2)
`reg_gprval		exL5RegValCn2;		//Destination Value (CR, EX2)
		
reg[47:0]		exL5RegValPc;		//PC Value (Synthesized)
reg[32:0]		exL5RegValImm;		//Immediate (Decode)
reg				exL5BraFlush;		//Flush EX2
reg				exL5TrapFlush;		//Flush EX2



reg[47:0]		exL6ValBPc;

reg[8:0]		exL6OpUCmd;
reg[8:0]		exL6OpUIxt;
wire[1:0]		exL6Hold;
reg[1:0]		exL6PreBra;
reg[95:0]		exL6IstrWord;	//source instruction word

`reg_gpr		exL6RegIdRs;		//Source A, ALU / Base
`reg_gpr		exL6RegIdRt;		//Source B, ALU / Index
`reg_gpr		exL6RegIdRm;		//Source C, MemStore
`reg_gprval		exL6RegValRs;		//Source A Value
`reg_gprval		exL6RegValRt;		//Source B Value
`reg_gprval		exL6RegValRm;		//Source C Value

`reg_gpr		exL6RegIdRn2;		//Destination ID (EX2)
`reg_gprval		exL6RegValRn2;		//Destination Value (EX2)
`reg_gpr		exL6RegIdCn2;		//Destination ID (CR, EX2)
`reg_gprval		exL6RegValCn2;		//Destination Value (CR, EX2)
		
reg[47:0]		exL6RegValPc;		//PC Value (Synthesized)
reg[32:0]		exL6RegValImm;		//Immediate (Decode)
reg				exL6BraFlush;		//Flush EX2
reg				exL6TrapFlush;		//Flush EX2



reg[31:0]	tValStepPc;

reg[47:0]	tValNextPc;
reg[47:0]	tValNextPcHi;
reg[47:0]	tValNextPcInc;
reg[47:0]	tOpNextPc;
reg[47:0]	tIsrNextPc;

reg[47:0]	tValNextPcBra;
reg[47:0]	tValNextPcIncBra;
reg			tValNextPcDoBra;

reg			tIsrBranchMsgLatch;
reg			tNxtIsrBranchMsgLatch;

reg[47:0]	tValBraPc;
reg[47:0]	tValBraPcHi;
reg[47:0]	tValNextBraPc;
reg[47:0]	tValNextBraPcHi;
reg[15:0]	tValBraSrT;
reg[15:0]	tValNextBraSrT;
reg[47:0]	tIsrBraPc;

// (* max_fanout = 200 *)
// 	reg[7:0]	opBraFlushMask;
// (* max_fanout = 200 *)
//	reg[7:0]	nxtBraFlushMask;
// (* max_fanout = 200 *)
//	reg[7:0]	tIsrBraFlushMask;

// reg[63:0]	tNxtRegExc;
// reg[63:0]	tRegExc;
reg[7:0]	tRegExcOfs;

reg[127:0]	tDelayExc;
reg[127:0]	tNxtDelayExc;

reg[127:0]	tDelay2Exc;
reg[127:0]	tDelay3Exc;
reg[127:0]	tDelay4Exc;
reg[127:0]	tNxtDelay2Exc;
reg[127:0]	tNxtDelay3Exc;
reg[127:0]	tNxtDelay4Exc;

reg			tDelay2ExcAdv;
reg			tDelay3ExcAdv;
reg			tDelay4ExcAdv;

reg			tPreHold1;
reg			tNxtPreHold1;
reg			tLstPreHold1;
reg			tRegSkipExc;

`ifdef jx2_debug_hitmiss
reg[15:0]	tBraHitMiss;
reg[15:0]	tBraNxtHitMiss;

reg[15:0]	tBraNonBra;
reg[15:0]	tBraNxtNonBra;

reg[15:0]	tBraCycCnt;
reg[15:0]	tBraNxtCycCnt;
`endif

reg[15:0]	tHoldCycCnt;
reg[15:0]	tHoldNxtCycCnt;
// reg			tDeadlockLatch;
// reg			tNxtDeadlockLatch;

reg			tExcLatch;
reg			tNxtExcLatch;

// reg[ 5:0]	tValAddPc0;
// reg[26:0]	tValPcAdd1;

reg[ 8:0]	tValAddPc0;
reg[23:0]	tValPcAdd1;
reg[12:0]	tValPcAdd1L;
reg[12:0]	tValPcAdd1H;

reg[ 8:0]	tValAddPc0p2;
reg[ 8:0]	tValAddPc0p4;
reg[ 8:0]	tValAddPc0p8;
reg[ 8:0]	tValAddPc0p12;

reg[ 8:0]	tValAddPc0p16;
reg[ 8:0]	tValAddPc0p18;
reg[ 8:0]	tValAddPc0p20;
reg[ 8:0]	tValAddPc0p24;
reg[ 8:0]	tValAddPc0p28;

reg[ 8:0]	tValAddPc0p31;

reg[31:0]	tValPcAdd2p2;
reg[31:0]	tValPcAdd2p4;
reg[31:0]	tValPcAdd2p8;
reg[31:0]	tValPcAdd2p12;

reg[31:0]	tValPcAdd2p16;
reg[31:0]	tValPcAdd2p18;
reg[31:0]	tValPcAdd2p20;
reg[31:0]	tValPcAdd2p24;
reg[31:0]	tValPcAdd2p28;


reg[47:0]		braInSpc;
reg[63:0]		braInExsr;
reg[63:0]		braInTea;
reg[63:0]		braInTeaHi;
reg[63:0]		braInSr;
reg[63:0]		braInLr;
reg[63:0]		braInDlr;
reg[63:0]		braInDhr;
reg[47:0]		braInSp;
reg[47:0]		braInSsp;
reg				braIsIsr;

reg[47:0]		braIsrSp;

reg[47:0]		braNxtInSpc;
reg[63:0]		braNxtInExsr;
reg[63:0]		braNxtInTea;
reg[63:0]		braNxtInTeaHi;
reg[63:0]		braNxtInSr;
reg[63:0]		braNxtInLr;
reg[63:0]		braNxtInDlr;
reg[63:0]		braNxtInDhr;
reg[47:0]		braNxtInSp;
reg[47:0]		braNxtInSsp;
reg				braNxtIsIsr;

assign		id2RegValSp		=
			(ex1RegIdRn1==JX2_GR_SP) ?
				ex1RegValRn1[47:0] :
`ifdef jx2_enable_wex
			(exB1RegIdRn1==JX2_GR_SP) ?
				exB1RegValRn1[47:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC1RegIdRn1==JX2_GR_SP) ?
				exC1RegValRn1[47:0] :
`endif
			(ex2RegIdRn2==JX2_GR_SP) ?
				ex2RegValRn2[47:0] :
`ifdef jx2_enable_wex
			(exB2RegIdRn2==JX2_GR_SP) ?
				exB2RegValRn2[47:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC2RegIdRn2==JX2_GR_SP) ?
				exC2RegValRn2[47:0] :
`endif
			(ex3RegIdRn2==JX2_GR_SP) ?
				ex3RegValRn2[47:0] :
`ifdef jx2_enable_wex
			(exB3RegIdRn2==JX2_GR_SP) ?
				exB3RegValRn2[47:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC3RegIdRn2==JX2_GR_SP) ?
				exC3RegValRn2[47:0] :
`endif
			gprOutSp[47:0];

assign		id2RegValLr		=
			(ex1RegIdRn1==JX2_GR_LR) ?
			ex1RegValRn1[63:0] :
			(ex2RegIdRn2==JX2_GR_LR) ?
			ex2RegValRn2[63:0] :
			(ex3RegIdRn2==JX2_GR_LR) ?
			ex3RegValRn2[63:0] :
			crOutLr[63:0];


assign		id2RegValDlr		=
			(ex1RegIdRn1==JX2_GR_DLR) ?
				ex1RegValRn1[63:0] :
`ifdef jx2_enable_wex
			(exB1RegIdRn1==JX2_GR_DLR) ?
				exB1RegValRn1[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC1RegIdRn1==JX2_GR_DLR) ?
				exC1RegValRn1[63:0] :
`endif
			(ex2RegIdRn2==JX2_GR_DLR) ?
				ex2RegValRn2[63:0] :
`ifdef jx2_enable_wex
			(exB2RegIdRn2==JX2_GR_DLR) ?
				exB2RegValRn2[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC2RegIdRn2==JX2_GR_DLR) ?
				exC2RegValRn2[63:0] :
`endif
			(ex3RegIdRn2==JX2_GR_DLR) ?
				ex3RegValRn2[63:0] :
`ifdef jx2_enable_wex
			(exB3RegIdRn2==JX2_GR_DLR) ?
				exB3RegValRn2[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC3RegIdRn2==JX2_GR_DLR) ?
				exC3RegValRn2[63:0] :
`endif
			gprOutDlr[63:0];

assign		id2RegValDhr		=
			(ex1RegIdRn1==JX2_GR_DHR) ?
				ex1RegValRn1[63:0] :
`ifdef jx2_enable_wex
			(exB1RegIdRn1==JX2_GR_DHR) ?
				exB1RegValRn1[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC1RegIdRn1==JX2_GR_DHR) ?
				exC1RegValRn1[63:0] :
`endif
			(ex2RegIdRn2==JX2_GR_DHR) ?
				ex2RegValRn2[63:0] :
`ifdef jx2_enable_wex
			(exB2RegIdRn2==JX2_GR_DHR) ?
				exB2RegValRn2[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC2RegIdRn2==JX2_GR_DHR) ?
				exC2RegValRn2[63:0] :
`endif
			(ex3RegIdRn2==JX2_GR_DHR) ?
				ex3RegValRn2[63:0] :
`ifdef jx2_enable_wex
			(exB3RegIdRn2==JX2_GR_DHR) ?
				exB3RegValRn2[63:0] :
`endif
`ifdef jx2_enable_wex3w
			(exC3RegIdRn2==JX2_GR_DHR) ?
				exC3RegValRn2[63:0] :
`endif
			gprOutDhr[63:0];

`ifdef def_true
wire	exHold2a;

assign	exHold2a	=
		(ex1Hold[0])	||	(ex2Hold[0])	||
		(ex3Hold[0])	||	dcOutHold		||
		(ifOutPcOK[1])	||	(ex1FpuOK[1])	||
		ex1SloMulDoHold	||	gprDoHold;

assign	tDbgMisc = {
		ex1Hold[0],			ex2Hold[0],
		ex3Hold[0],			dcOutHold,
		ifOutPcOK[1],		ex1FpuOK[1],
		ex1SloMulDoHold,	ex1BraFlush && (ex1OpUCmd[5:0]!=0)
		};

//`ifdef jx2_isfpga
//BUFG	clk50_buf(.I(exHold2a), .O(exHold2));
//`else
assign	exHold2 = exHold2a;
//`endif

`endif

always @*
begin
	exHold1			= 0;
	exHold1A		= 0;
	exHold1B		= 0;
	exHold1C		= 0;
	exHold1D		= 0;

	exHold1B1		= 0;
	exHold1B2		= 0;
	exHold1B3		= 0;
	exHold1C1		= 0;
	exHold1C2		= 0;
	exHold1C3		= 0;
	exHold1D1		= 0;

//	exHold2			= 0;
	tNxtRegExc		= 0;
	tRegExcOfs		= 0;
	tNxtPreHold1	= 0;
	
	tDbgOutStatus1	= 0;
	tDbgOutStatus2	= 0;
	tDbgOutStatus3	= 0;
	tDbgOutStatus4	= 0;

	tDbgOutStatus5	= 0;
	tDbgOutStatus6	= 0;
	tDbgOutStatus7	= 0;
	tDbgOutStatus8	= 0;
	tNxtRegExcCycInh	= 0;

	tHoldNxtCycCnt		= 0;
	tNxtDeadlockLatch	= tDeadlockLatch;
//	tNxtExcLatch		= tExcLatch;

	ifNxtValBraOk	= 0;

`ifdef def_true
	ex1RegInSchm	= 0;
	ex2RegInSchm	= 0;
	ex1RegInLr		= 0;
	ex2RegInSr		= 0;
	ex1RegInSp		= 0;
	ex1RegInDhr		= 0;
	ex1RegInDlr		= 0;
	ex2RegInSp		= 0;
	ex2MemDataOK	= 0;
	ex2MemDataInB	= 0;
	ex2MemDataIn	= 0;
`ifdef jx2_enable_wex3w
	exC3RegMulWRes	= 0;
	exC2RegMulWRes	= 0;
`endif
	exB2RegMulWRes	= 0;
	exB3RegMulWRes	= 0;
	dcInTraPc		= 0;
	dcInValB		= 0;
	dcInVal			= 0;
	dcInOpmB		= 0;
	dcInAddrB		= 0;
	dcInLdOp		= 0;
	dcInOpm			= 0;
	dcInAddr		= 0;
	ex3RegMulWRes	= 0;
	ex3RegAluRes	= 0;
//	ex3RegAluResB	= 0;
	ex3RegMulRes	= 0;
	ex2RegMulWRes	= 0;
	ex2RegMulRes	= 0;
	ex2RegAluRes	= 0;

	exB2RegAluRes	= 0;
	exB3RegAluRes	= 0;

`ifdef jx2_enable_wex3w
	exC2RegAluRes	= 0;
	exC3RegAluRes	= 0;
`endif

	gprEx1Flush		= 0;
	gprEx2Flush		= 0;
	gprEx3Flush		= 0;
	crIdCn4			= 0;
	crValCn4		= 0;
	crIdCn3			= 0;
	crValCn3		= 0;
	crIdCn2			= 0;
	crValCn2		= 0;
	crIdCn1			= 0;
	crValCn1		= 0;

	gprIdRnB1		= 0;
	gprValRnB1		= 0;
	gprIdRnB2		= 0;
	gprValRnB2		= 0;
	gprIdRnB3		= 0;
	gprValRnB3		= 0;
	gprIdRnB4		= 0;
	gprValRnB4		= 0;

`ifdef jx2_enable_wex3w
	gprIdRnC1		= 0;
	gprValRnC1		= 0;
	gprIdRnC2		= 0;
	gprValRnC2		= 0;
	gprIdRnC3		= 0;
	gprValRnC3		= 0;
	gprIdRnC4		= 0;
	gprValRnC4		= 0;
`endif

	gprIdRn1		= 0;
	gprValRn1		= 0;
	gprIdRn2		= 0;
	gprValRn2		= 0;
	gprIdRn3		= 0;
	gprValRn3		= 0;
	gprIdRn4		= 0;
	gprValRn4		= 0;

`ifdef jx2_enable_wex3w
	gprValImmC4		= 0;
`endif

	gprValImmB4		= 0;
	gprValImmA4		= 0;
	
	tDbgExWidth		= 0;
	crInPc			= 0;
	ifValPcInc		= 0;
	ifValPcHi		= 0;
	ifValPc			= 0;
	tNxtIsrBranchMsgLatch	= 0;
	braIsrSp		= 0;
	
`endif

	crInExsr		= crOutExsr;
	crInSpc			= crOutSpc;
	crInSsp			= crOutSsp;
	crInTea			= crOutTea;
	crInTeaHi		= crOutTeaHi;

	crInPcHi		= crOutPcHi;
//	crInLrHi		= crOutLrHi;
	crInLrHi		= ex1OutLrHi;
	crInSpcHi		= crOutSpcHi;

//	tValNextBraPc	= UV32_XX;
//	tValNextBraPc	= UV32_00;
//	tValNextBraPc	= UV48_00;
//	tValNextBraPcHi	= UV48_00;

	tValNextBraPc	= tValBraPc;
	tValNextBraPcHi	= tValBraPcHi;

//	tValNextBraSrT	= UV16_00;
	tValNextBraSrT	= {
		crOutSr[15: 4],
		crOutSr[27:26],
		crOutSr[ 1: 0] };


	ex1TrapFlush	= 0;
	ex2TrapFlush	= 0;
	ex3TrapFlush	= 0;

//	gprEx1DualLane	= (ex1OpUIxt[7:6] == 2'b11);
//	gprEx2DualLane	= (ex2OpUIxt[7:6] == 2'b11);
//	gprEx3DualLane	= (ex3OpUIxt[7:6] == 2'b11);

	gprEx1DualLane	= (ex1OpUIxt[7:6] != 2'b00);
	gprEx2DualLane	= (ex2OpUIxt[7:6] != 2'b00);
	gprEx3DualLane	= (ex3OpUIxt[7:6] != 2'b00);


`ifdef jx2_debug_hitmiss
	tBraNxtHitMiss	= tBraHitMiss;
	tBraNxtNonBra	= tBraNonBra;
//	tBraNxtCycCnt	= tBraCycCnt + 1;
	tBraNxtCycCnt	= tBraCycCnt;
`endif

`ifndef def_true
	exHold2	=
		(ex1Hold[0])	||	(ex2Hold[0])	||
		(ex3Hold[0])	||	dcOutHold		||
		(ifOutPcOK[1])	||	(ex1FpuOK[1])	||
		ex1SloMulDoHold	||	gprDoHold;
`endif

	exHold1A	= exHold2;

`ifdef jx2_enable_wex3w

`ifdef def_true
	exHold1B1	=
		((	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRu) ||
			(ex1RegIdRm == gprIdRv) ||
			(ex1RegIdRm == gprIdRx) ||
			(ex1RegIdRm == gprIdRy)	) &&
			(ex1RegIdRm != JX2_GR_ZZR) &&
			(ex1Hold[1]));
//			(ex1Hold[1] || ex1RegIdRm[6]));
//			(ex1Hold[1] || (ex1RegIdRm[6:5]==2'b11)));
	exHold1B2	=
		((	(exB1RegIdRm == gprIdRs) ||
			(exB1RegIdRm == gprIdRt) ||
			(exB1RegIdRm == gprIdRu) ||
			(exB1RegIdRm == gprIdRv) ||
			(exB1RegIdRm == gprIdRx) ||
			(exB1RegIdRm == gprIdRy)	) &&
			(exB1RegIdRm != JX2_GR_ZZR) &&
			(exB1Hold[1]));
//			(exB1Hold[1] || exB1RegIdRm[6]));
//			(exB1Hold[1] || (exB1RegIdRm[6:5]==2'b11)));

`ifdef jx2_cpu_nofw_lane3
	exHold1B3	=
		(	(exC1RegIdRm == gprIdRs) ||
			(exC1RegIdRm == gprIdRt) ||
			(exC1RegIdRm == gprIdRu) ||
			(exC1RegIdRm == gprIdRv) ||
			(exC1RegIdRm == gprIdRx) ||
			(exC1RegIdRm == gprIdRy) );
`else
	exHold1B3	=
		((	(exC1RegIdRm == gprIdRs) ||
			(exC1RegIdRm == gprIdRt) ||
			(exC1RegIdRm == gprIdRu) ||
			(exC1RegIdRm == gprIdRv) ||
			(exC1RegIdRm == gprIdRx) ||
			(exC1RegIdRm == gprIdRy)	) &&
			(exC1RegIdRm != JX2_GR_ZZR) &&
			(exC1Hold[1]));
//			(exC1Hold[1] || exC1RegIdRm[6]));
`endif

	exHold1C1	=
		((	(ex2RegIdRm == gprIdRs) ||
			(ex2RegIdRm == gprIdRt) ||
			(ex2RegIdRm == gprIdRu) ||
			(ex2RegIdRm == gprIdRv) ||
			(ex2RegIdRm == gprIdRx) ||
			(ex2RegIdRm == gprIdRy)	) &&
			(ex2RegIdRm != JX2_GR_ZZR) &&
			(ex2Hold[1]));
//			(ex2Hold[1] || ex2RegIdRm[6]));
//			(ex2Hold[1] || (ex2RegIdRm[6:5]==2'b11)));
	exHold1C2	=
		((	(exB2RegIdRm == gprIdRs) ||
			(exB2RegIdRm == gprIdRt) ||
			(exB2RegIdRm == gprIdRu) ||
			(exB2RegIdRm == gprIdRv) ||
			(exB2RegIdRm == gprIdRx) ||
			(exB2RegIdRm == gprIdRy)	) &&
			(exB2RegIdRm != JX2_GR_ZZR) &&
			(exB2Hold[1]));
//			(exB2Hold[1] || exB2RegIdRm[6]));
//			(exB2Hold[1] || (exB2RegIdRm[6:5]==2'b11)));

`ifdef jx2_cpu_nofw_lane3
	exHold1C3	=
		(	(exC2RegIdRm == gprIdRs) ||
			(exC2RegIdRm == gprIdRt) ||
			(exC2RegIdRm == gprIdRu) ||
			(exC2RegIdRm == gprIdRv) ||
			(exC2RegIdRm == gprIdRx) ||
			(exC2RegIdRm == gprIdRy) );
`else
	exHold1C3	=
		((	(exC2RegIdRm == gprIdRs) ||
			(exC2RegIdRm == gprIdRt) ||
			(exC2RegIdRm == gprIdRu) ||
			(exC2RegIdRm == gprIdRv) ||
			(exC2RegIdRm == gprIdRx) ||
			(exC2RegIdRm == gprIdRy)	) &&
			(exC2RegIdRm != JX2_GR_ZZR) &&
			(exC2Hold[1]));
//			(exC2Hold[1] || exC2RegIdRm[6]));
`endif

	exHold1D1	=
		((	(ex3RegIdRm == gprIdRs) ||
			(ex3RegIdRm == gprIdRt) ||
			(ex3RegIdRm == gprIdRu) ||
			(ex3RegIdRm == gprIdRv) ||
			(ex3RegIdRm == gprIdRx) ||
			(ex3RegIdRm == gprIdRy)	) &&
			(ex3RegIdRm != JX2_GR_ZZR) &&
			(ex3Hold[1]));
//			(ex3Hold[1] || ex3RegIdRm[6]));
//			(ex3Hold[1] || (ex3RegIdRm[6:5]==2'b11)));

//	exHold1D2	= 0;

	exHold1D2	=
		((	(exB3RegIdRm == gprIdRs) ||
			(exB3RegIdRm == gprIdRt) ||
			(exB3RegIdRm == gprIdRu) ||
			(exB3RegIdRm == gprIdRv) ||
			(exB3RegIdRm == gprIdRx) ||
			(exB3RegIdRm == gprIdRy) ) &&
			(exB3RegIdRm != JX2_GR_ZZR) &&
			(exB3Hold[1]));
//			(exB3Hold[1] || exB3RegIdRm[6]));


`ifdef jx2_cpu_nofw_lane3
	exHold1D3	=
		(	(exC3RegIdRm == gprIdRs) ||
			(exC3RegIdRm == gprIdRt) ||
			(exC3RegIdRm == gprIdRu) ||
			(exC3RegIdRm == gprIdRv) ||
			(exC3RegIdRm == gprIdRx) ||
			(exC3RegIdRm == gprIdRy) ) ;
`else
	exHold1D3	= 0;
`endif

	exHold1B	=
		(( exHold1B1 || exHold1B2 || exHold1B3) && !ex1BraFlush) ||
		(( exHold1C1 || exHold1C2 || exHold1C3) && !ex2BraFlush) ||
		(( exHold1D1 || exHold1D2 || exHold1D3) && !ex3BraFlush) ||
		id2_PredNeedStall			;

//	if(idA2IdUCmd[5:0]==JX2_UCMD_NOP)
	if(idA2IdUCmd == JX2_UCMDSP_BRANOP)
		exHold1B	= 0;
	if(opBraFlushMask[1])
		exHold1B	= 0;
	if(ex1RegIdCn1 == JX2_CR_PC)
		exHold1B	= 0;

`endif

`endif


`ifndef jx2_enable_wex3w
`ifdef jx2_enable_wex2w
	exHold1B1	=
		((	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRu) ||
			(ex1RegIdRm == gprIdRv) ) &&
			(ex1Hold[1]));
	exHold1B2	=
		((	(exB1RegIdRm == gprIdRs) ||
			(exB1RegIdRm == gprIdRt) ||
			(exB1RegIdRm == gprIdRu) ||
			(exB1RegIdRm == gprIdRv) ) &&
			(exB1Hold[1]));

	exHold1C1	=
		((	(ex2RegIdRm == gprIdRs) ||
			(ex2RegIdRm == gprIdRt) ||
			(ex2RegIdRm == gprIdRu) ||
			(ex2RegIdRm == gprIdRv)	) &&
			(ex2Hold[1]));
	exHold1C2	=
		((	(exB2RegIdRm == gprIdRs) ||
			(exB2RegIdRm == gprIdRt) ||
			(exB2RegIdRm == gprIdRu) ||
			(exB2RegIdRm == gprIdRv) ) &&
			(exB2Hold[1]));

	exHold1D1	=
		((	(ex3RegIdRm == gprIdRs) ||
			(ex3RegIdRm == gprIdRt) ||
			(ex3RegIdRm == gprIdRu) ||
			(ex3RegIdRm == gprIdRv)	) &&
			(ex3Hold[1]));

	exHold1B	=
		(( exHold1B1 || exHold1B2) && !ex1BraFlush) ||
		(( exHold1C1 || exHold1C2) && !ex2BraFlush) ||
		(exHold1D1 && !ex3BraFlush) ||
		id2_PredNeedStall			;

	if(idA2IdUCmd == JX2_UCMDSP_BRANOP)
		exHold1B	= 0;
	if(opBraFlushMask[1])
		exHold1B	= 0;
	if(ex1RegIdCn1 == JX2_CR_PC)
		exHold1B	= 0;

`endif
`endif


`ifndef jx2_enable_wex
//	exHold1B	=
//		(	(ex1RegIdRm == gprIdRs) ||
//			(ex1RegIdRm == gprIdRt) ||
//			(ex1RegIdRm == gprIdRm)	) &&
//		(ex1HldIdRn1 != JX2_GR_ZZR);

	exHold1B1	=
		(	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRm)	) &&
		ex1Hold[1];

	exHold1C1	=
		(	(ex2RegIdRm == gprIdRs) ||
			(ex2RegIdRm == gprIdRt) ||
			(ex2RegIdRm == gprIdRm)	) &&
		ex2Hold[1];

	exHold1D1	=
		((	(ex3RegIdRm == gprIdRs) ||
			(ex3RegIdRm == gprIdRt) ||
			(ex3RegIdRm == gprIdRm)	) &&
			(ex3Hold[1]));

	exHold1B	=
		(exHold1B1 && !ex1BraFlush) ||
		(exHold1C1 && !ex2BraFlush) ||
		(exHold1D1 && !ex3BraFlush) ||
		id2_PredNeedStall			;

	if(idA2IdUCmd == JX2_UCMDSP_BRANOP)
		exHold1B	= 0;
	if(opBraFlushMask[1])
		exHold1B	= 0;
	if(ex1RegIdCn1 == JX2_CR_PC)
		exHold1B	= 0;
`endif

	exHold1C = 0;

// `ifndef def_true
`ifdef def_true

	/* Hack for fix MemOp+Branch bug;
	   TODO: Find/Fix actual cause of bug.
	 */
`ifdef jx2_enable_wex

// `ifdef def_true
`ifndef def_true
	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&
		((idA2IdUCmd[5:0]==JX2_UCMD_JMP) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_JSR) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_BRA) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_BSR) ))
			exHold1C = 1;
`endif

`ifndef def_true
// `ifdef def_true
//	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&

	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_FMOV_MR) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_FMOV_RM) ||

		 (ex1OpUCmd[5:0]==JX2_UCMD_FPUV4SF) ||

//		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_IR) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_CONV2_RR) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_QMULDIV) ||

//		 (ex1OpUCmd[5:0]==JX2_UCMD_FPU3) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_FLDCX) ||
//		 (ex1OpUCmd[5:0]==JX2_UCMD_FSTCX) ||
		 
		 (ex1OpUCmd[5:0]==JX2_UCMD_MULW3) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_MUL3))	&&

		(idA2IdUCmd[5:0]==JX2_UCMD_JMP)	)

//		((idA2IdUCmd[5:0]==JX2_UCMD_JMP) ||
//		 (idA2IdUCmd[5:0]==JX2_UCMD_JSR) ||
//		 (idA2IdUCmd[5:0]==JX2_UCMD_BRA) ||
//		 (idA2IdUCmd[5:0]==JX2_UCMD_BSR) ||
//		 (idA2IdUCmd[5:0]==JX2_UCMD_JCMP)) &&
//			id2PreBra[1])
	begin
		$display("ExUnit: Branch Hold ID2=%X EX1=%X PreBra=%X",
			idA2IdUCmd, ex1OpUCmd, id2PreBra);
		$display("  ID2.Rs/Rt/Rm=%X/%X/%X EX1.Rm=%X",
			gprIdRs, gprIdRt, gprIdRm, ex1RegIdRm);
			
		$display("  A: DLR=%X DHR=%X SR=%X", gprOutDlr, gprOutDhr, crOutSr);
//		exHold1C = 1;
	end


	if(	(ex1OpUCmd[5:0]==JX2_UCMD_NOP)	&&
		(idA2IdUCmd[5:0]==JX2_UCMD_JMP)	)
	begin
		$display("  B: DLR=%X DHR=%X SR=%X", gprOutDlr, gprOutDhr, crOutSr);
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(	(	(ex1OpUCmd[5:0]!=JX2_UCMD_NOP) &&
			(ex1OpUCmd[5:0]!=JX2_UCMD_INVOP) &&
			(ex1OpUCmd[5:0]!=JX2_UCMD_OP_IXT) &&
			(ex1OpUCmd[5:0]!=JX2_UCMD_ALUCMP)	)
		&&
		(	(idA2IdUCmd[5:0]==JX2_UCMD_JMP) ||
			(idA2IdUCmd[5:0]==JX2_UCMD_JSR) ||
			(idA2IdUCmd[5:0]==JX2_UCMD_BRA) ||
			(idA2IdUCmd[5:0]==JX2_UCMD_BSR) ||
			(idA2IdUCmd[5:0]==JX2_UCMD_JCMP))	)
			exHold1C = 1;
`endif

// `ifdef def_true
`ifndef def_true
	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM) ||
		 (ex2OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (ex2OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&
		((idA2IdUCmd[5:0]==JX2_UCMD_JMP) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_JSR) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_BRA) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_BSR) ||
		 (idA2IdUCmd[5:0]==JX2_UCMD_JCMP)))
			exHold1C = 1;
`endif

`ifndef def_true
	if(	((idA2IdUCmd[5:0]==JX2_UCMD_MOV_MR)		||
		 (idA2IdUCmd[5:0]==JX2_UCMD_MOV_RM))	)
	begin
		if(	(ex1RegIdRm != JX2_CR_ZZR) &&
			(	(ex1RegIdRm == ex1RegIdRs) ||
				(ex1RegIdRm == ex1RegIdRt)	)
			)
		begin
			exHold1C = 1;
		end
	end
`endif

`else
	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&
		((id2IdUCmd[5:0]==JX2_UCMD_JMP) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_JSR) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_BRA) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_BSR)))
			exHold1C = 1;
`endif

`endif

`ifdef def_true
`ifdef jx2_enable_wex
	/* Double Divide Bug? */
	if(	(ex1OpUCmd[5:0]==JX2_UCMD_QMULDIV)	&&
		(idA2IdUCmd[5:0]==JX2_UCMD_QMULDIV))
			exHold1C = 1;
`else
	/* Double Divide Bug? */
	if(	(ex1OpUCmd[5:0]==JX2_UCMD_QMULDIV)	&&
		(id2IdUCmd[5:0]==JX2_UCMD_QMULDIV))
			exHold1C = 1;
`endif
`endif

`ifdef jx2_enable_prebra
`ifdef jx2_prebra_rts
		id1BraPipelineLr =
//			(ex1RegIdCn1 == JX2_CR_LR) ||
//			(ex2RegIdCn1 == JX2_CR_LR) ||
//			(ex3RegIdCn1 == JX2_CR_LR) ||

			(gprIdRm    == JX2_GR_LR) ||
			(ex1RegIdRm == JX2_GR_LR) ||
			(ex2RegIdRm == JX2_GR_LR) ||
			(ex3RegIdRm == JX2_GR_LR) ||
			(ewbRegIdRm == JX2_GR_LR) ||

			(ex1OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(ex1OpUCmd[5:0]==JX2_UCMD_BSR) ||
			(ex2OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(ex2OpUCmd[5:0]==JX2_UCMD_BSR) ||
			(ex3OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(ex3OpUCmd[5:0]==JX2_UCMD_BSR);

`ifdef def_true		
		id1BraPipelineR1 =
			(gprIdRm    == JX2_GR_DHR) ||
			(ex1RegIdRm == JX2_GR_DHR) ||
			(ex2RegIdRm == JX2_GR_DHR) ||
			(ex3RegIdRm == JX2_GR_DHR) ||
			(ewbRegIdRm == JX2_GR_DHR) ;
`endif

`ifndef def_true		
		id1BraPipelineR1 =
			(gprIdRm    == JX2_GR_DHR) ||
			(gprIdRn    == JX2_GR_DHR) ||
			(ex1RegIdRm == JX2_GR_DHR) ||
			(ex2RegIdRm == JX2_GR_DHR) ||
			(ex3RegIdRm == JX2_GR_DHR) ||
			(ewbRegIdRm == JX2_GR_DHR) ||
			(exB1RegIdRm == JX2_GR_DHR) ||
			(exB2RegIdRm == JX2_GR_DHR) ||
			(exB3RegIdRm == JX2_GR_DHR) ;
`endif

//		id1BraPipelineDbl =
//			(id1PreBra!=0) ||
//			(id2PreBra!=0) ||
//			(ex1PreBra!=0) ||
//			(ex2PreBra!=0) ||
//			(ex3PreBra!=0) ;

`endif

		id1BraPipelineDbl =
			(!id1PreBra[1]) ||
			(!id2PreBra[1]) ||
//			(!ex1PreBra[1]) ;
			(!ex1PreBra[1]) ||
			opBraFlushMask[3];
//			opBraFlushMask[2];

//		id1BraPipelineDbl =
//			(!id1PreBra[1]) ||
//			opBraFlushMask[2];

		if(id1BraPipelineDbl && id1WasBra)
		begin
//			$display("id1BraPipelineDbl: %X %X %X %X PC=%X",
//				id1PreBra, id2PreBra, ex1PreBra, opBraFlushMask, id1ValBPc);
		end

//			(!ex1PreBra[1]) ||
//			(pcOutStep == 0) ;

//			(!ex1PreBra[1]) ||
//			(!ex2PreBra[1]) ;
//			(!ex2PreBra[1]) ||
//			(!ex3PreBra[1]) ;

`ifndef def_true
		if(	(idA1IdUCmd[5:0]==JX2_UCMD_MOV_MR) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_MOV_RM) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_FMOV_MR) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_FMOV_RM) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_FPUV4SF) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_MUL3) ||
			(idA1IdUCmd[5:0]==JX2_UCMD_MULW3))
		begin
			id1BraPipelineDbl = 1;
		end
`endif

`endif

`ifdef jx2_enable_xgpr

`ifndef def_true
	if( (ex1RegIdCn1 != JX2_CR_ZZR) &&
			(ex1RegIdCn1 != JX2_CR_PC) &&
			(ex1RegIdCn1[6:5] == 2'b11))
		exHold1D = 1;

	if( (ex2RegIdCn2 != JX2_CR_ZZR) &&
			(ex2RegIdCn2 != JX2_CR_PC) &&
			(ex2RegIdCn2[6:5] == 2'b11))
		exHold1D = 1;

	if( (ex3RegIdCn3 != JX2_CR_ZZR) &&
			(ex3RegIdCn3 != JX2_CR_PC) &&
			(ex3RegIdCn3[6:5] == 2'b11))
		exHold1D = 1;
`endif

`else

//	if( ({1'b1, ex1RegIdCn1} != JX2_CR_ZZR) &&
//			({1'b1, ex1RegIdCn1} != JX2_CR_PC))
	if( (ex1RegIdCn1 != JX2_CR_ZZR) &&
			(ex1RegIdCn1 != JX2_CR_PC) &&
			(ex1RegIdCn1[5:4] == 2'b11))
		exHold1D = 1;

//	if( ({1'b1, ex2RegIdCn2} != JX2_CR_ZZR) &&
//			({1'b1, ex2RegIdCn2} != JX2_CR_PC))
	if( (ex2RegIdCn2 != JX2_CR_ZZR) &&
			(ex2RegIdCn2 != JX2_CR_PC) &&
			(ex2RegIdCn2[5:4] == 2'b11))
		exHold1D = 1;

//	if( ({1'b1, ex3RegIdCn3} != JX2_CR_ZZR) &&
//			({1'b1, ex3RegIdCn3} != JX2_CR_PC))
	if( (ex3RegIdCn3 != JX2_CR_ZZR) &&
			(ex3RegIdCn3 != JX2_CR_PC) &&
			(ex3RegIdCn3[5:4] == 2'b11))
		exHold1D = 1;

`endif

	exHold1		= exHold1A || exHold1B || exHold1C || exHold1D;

	tDbgOutStatus1	= exHold1A;
	tDbgOutStatus2	= exHold1B;
	tDbgOutStatus3	= exHold1C;
	tDbgOutStatus4	= exHold1D;

`ifdef jx2_use_ringbus
//	tDbgOutStatus5	= ifOutPcOK[1];
	tDbgOutStatus5	= ifOutPcOK[1] || (ifOutPcStep==0);
	tDbgOutStatus6	= dcOutOK[1];
	tDbgOutStatus7	= dcBusWait;
	tDbgOutStatus8	= memOpm[7:0] != 0;
`else
	tDbgOutStatus5	= ifOutPcOK[1];
	tDbgOutStatus6	= dcOutOK[1];
	tDbgOutStatus7	= memOK[1];
	tDbgOutStatus8	= memOpm[3] || memOpm[4];
`endif

`ifdef jx2_debug_expipe
	if(exHold1 &&
		!((ex1OpUCmd[5:0]==JX2_UCMD_OP_IXT) && 
			(ex1OpUIxt[5:0]==JX2_UCIX_IXT_BREAK)))
	begin
		$display("ExHold: A=%d B=%d C=%d D=%d",
			exHold1A, exHold1B, exHold1C, exHold1D);
	
		if(exHold2)
		begin
			$display("ExHold2: Ex1=%d Ex2=%d I$=%d FPU=%d D$=%d",
				ex1Hold, ex2Hold, ifOutPcOK[1], ex1FpuOK[1], dcOutHold);
			$display("\tEX1=%d EX2=%d EX3=%d",
				(ex1Hold[0]), (ex2Hold[0]), (ex3Hold[0]));
		end
	end
`endif

`ifdef def_true
	if(exHold1)
	begin
		tHoldNxtCycCnt	= tHoldCycCnt + 1;
		if(tHoldCycCnt==65535)
		begin
			tNxtDeadlockLatch	= 1;
			tHoldNxtCycCnt		= 65535;
		end
		else
		if(tHoldCycCnt>65280)
		begin
//			tHoldNxtCycCnt	= tHoldCycCnt;
			tHoldNxtCycCnt	= 65535;
			$display("ExUnit: Deadlock Detected");

			$display("ExUnit: 1A=%d 1B=%d 1C=%d 1D=%d",
				exHold1A, exHold1B, exHold1C, exHold1D);

			$display("ExUnit: 1B1=%d 1B2=%d 1B3=%d",
				exHold1B1, exHold1B2, exHold1B3);

			$display("ExUnit: 1C1=%d 1C2=%d 1C3=%d",
				exHold1C1, exHold1C2, exHold1C3);

			$display("ExUnit: EX1=%d EX2=%d EX3=%d",
				ex1Hold[0], ex2Hold[0], ex3Hold[0]);
			$display("ExUnit: D$=%d I$=%d FPU=%d",
				dcOutHold, ifOutPcOK[1], ex1FpuOK[1]);

`ifdef jx2_enable_wex
			$display("ID1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				id1ValBPc,	id1ValPc,
				id1IstrWord[15: 0], id1IstrWord[31:16],
				id1IstrWord[47:32], id1IstrWord[63:48],
				id1IstrWord[79:64], id1IstrWord[95:80],
				idA1IdUCmd, idA1IdUIxt,
				idB1IdUCmd, idB1IdUIxt,
				opBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
				idA1IdRegM, idA1IdRegO, idB1IdRegM, idB1IdRegO,
				idA1IdRegN, idB1IdRegN);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X Ry=%X Ro=%X",
				idC1IdRegM, idC1IdRegO, idC1IdRegN);
`endif

			$display("ID2: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				id2ValBPc,	gprValPc,
				id2IstrWord[15: 0], id2IstrWord[31:16],
				id2IstrWord[47:32], id2IstrWord[63:48],
				id2IstrWord[79:64], id2IstrWord[95:80],
				idA2IdUCmd, idA2IdUIxt, idB2IdUCmd, idB2IdUIxt,
				opBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gprIdRs, gprValRs,
				gprIdRt, gprValRt,
				gprIdRm, gprValRm);
			$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
				gprIdRu, gprValRu,
				gprIdRv, gprValRv,
				gprIdRn, gprValRn);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
				gprIdRx, gprValRx,
				gprIdRy, gprValRy,
				gprIdRo, gprValRo);
`endif

`else
			$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				id1ValBPc,	id1ValPc,
				id1IstrWord[15: 0], id1IstrWord[31:16],
				id1IdUCmd, id1IdUIxt, opBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Rn=%X",
				id1IdRegM, id1IdRegO, id1IdRegN);

			$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				id2ValBPc,	gprValPc,
				id2IstrWord[15: 0], id2IstrWord[31:16],
				id2IdUCmd, id2IdUIxt, opBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gprIdRs, gprValRs,
				gprIdRt, gprValRt,
				gprIdRm, gprValRm);
`endif

			$display("EX1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex1ValBPc,	ex1RegValPc,
				ex1IstrWord[15: 0], ex1IstrWord[31:16],
				ex1IstrWord[47:32], ex1IstrWord[63:48],
				ex1IstrWord[79:64], ex1IstrWord[95:80],
				ex1OpUCmd, ex1OpUIxt, ex1BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
				ex1RegIdRs, ex1RegValRs,
				ex1RegIdRt, ex1RegValRt,
				ex1RegIdRm, ex1RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex1RegIdRn1, ex1RegValRn1,
				ex1RegIdCn1, ex1RegValCn1);

			$display("EX2: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex2ValBPc,	ex2RegValPc,
				ex2IstrWord[15: 0], ex2IstrWord[31:16],
				ex2IstrWord[47:32], ex2IstrWord[63:48],
				ex2IstrWord[79:64], ex2IstrWord[95:80],
				ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				ex2RegIdRs, ex2RegValRs,
				ex2RegIdRt, ex2RegValRt,
				ex2RegIdRm, ex2RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex2RegIdRn2, ex2RegValRn2,
				ex2RegIdCn2, ex2RegValCn2);

`ifdef jx2_enable_wex
			if(exB2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-B: Op=%X-%X",
					exB2OpUCmd, exB2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exB2RegIdRs, exB2RegValRs,
					exB2RegIdRt, exB2RegValRt,
					exB2RegIdRm, exB2RegValRm);
				$display("     Rn1=%X(%X)",
					exB2RegIdRn2, exB2RegValRn2);
			end

`ifdef jx2_enable_wex3w
			if(exC2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-C: Op=%X-%X",
					exC2OpUCmd, exC2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exC2RegIdRs, exC2RegValRs,
					exC2RegIdRt, exC2RegValRt,
					exC2RegIdRm, exC2RegValRm);
				$display("     Rn1=%X(%X)",
					exC2RegIdRn2, exC2RegValRn2);
			end
`endif
`endif


			$display("EX3: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex3ValBPc,	ex3RegValPc,
				ex3IstrWord[15: 0], ex3IstrWord[31:16],
				ex3IstrWord[47:32], ex3IstrWord[63:48],
				ex3IstrWord[79:64], ex3IstrWord[95:80],
				ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				ex3RegIdRs, ex3RegValRs,
				ex3RegIdRt, ex3RegValRt,
				ex3RegIdRm, ex3RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex3RegIdRn3, ex3RegValRn3,
				ex3RegIdCn3, ex3RegValCn3);

`ifdef jx2_enable_wex
			if(exB3OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX3-B: Op=%X-%X",
					exB3OpUCmd, exB3OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exB3RegIdRs, exB3RegValRs,
					exB3RegIdRt, exB3RegValRt,
					exB3RegIdRm, exB3RegValRm);
				$display("     Rn1=%X(%X)",
					exB3RegIdRn2, exB3RegValRn2);
			end

`ifdef jx2_enable_wex3w
			if(exC3OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX3-C: Op=%X-%X",
					exC3OpUCmd, exC3OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exC3RegIdRs, exC3RegValRs,
					exC3RegIdRt, exC3RegValRt,
					exC3RegIdRm, exC3RegValRm);
				$display("     Rn1=%X(%X)",
					exC3RegIdRn2, exC3RegValRn2);
			end
`endif
`endif

			$display("EXL4: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exL4ValBPc,	exL4RegValPc,
				exL4IstrWord[15: 0], exL4IstrWord[31:16],
				exL4IstrWord[47:32], exL4IstrWord[63:48],
				exL4IstrWord[79:64], exL4IstrWord[95:80],
				exL4OpUCmd, exL4OpUIxt, exL4BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exL4RegIdRs, exL4RegValRs,
				exL4RegIdRt, exL4RegValRt,
				exL4RegIdRm, exL4RegValRm);
//			$display("     Rn1=%X(%X) Cn1=%X(%X)",
//				exL4RegIdRn3, exL4RegValRn3,
//				exL4RegIdCn3, exL4RegValCn3);


			$display("EXL5: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exL5ValBPc,	exL5RegValPc,
				exL5IstrWord[15: 0], exL5IstrWord[31:16],
				exL5IstrWord[47:32], exL5IstrWord[63:48],
				exL5IstrWord[79:64], exL5IstrWord[95:80],
				exL5OpUCmd, exL5OpUIxt, exL5BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exL5RegIdRs, exL5RegValRs,
				exL5RegIdRt, exL5RegValRt,
				exL5RegIdRm, exL5RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				exL5RegIdRn2, exL5RegValRn2,
				exL5RegIdCn2, exL5RegValCn2);

			$display("EXL6: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exL6ValBPc,	exL6RegValPc,
				exL6IstrWord[15: 0], exL6IstrWord[31:16],
				exL6IstrWord[47:32], exL6IstrWord[63:48],
				exL6IstrWord[79:64], exL6IstrWord[95:80],
				exL6OpUCmd, exL5OpUIxt, exL5BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exL6RegIdRs, exL6RegValRs,
				exL6RegIdRt, exL6RegValRt,
				exL6RegIdRm, exL6RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				exL6RegIdRn2, exL6RegValRn2,
				exL6RegIdCn2, exL6RegValCn2);


			$display("");

		end
	end
`endif

//	case( {1'b1, ex1HldIdCn1} )
//		JX2_CR_LR:	 exHold1 = 1;
//		JX2_CR_SR:	 exHold1 = 1;
//		JX2_CR_GBR:	 exHold1 = 1;
//		JX2_CR_TBR:	 exHold1 = 1;
//		default: begin
//		end
//	endcase

	tNxtDelay2Exc	= tDelay2Exc;
	tNxtDelay3Exc	= tDelay3Exc;
	tNxtDelay4Exc	= tDelay4Exc;
	
	tDelay2ExcAdv	= 0;
	tDelay3ExcAdv	= 0;
//	tDelay4ExcAdv	= 0;

`ifndef def_true
// `ifdef def_true
	if((tDelay2Exc[15:12]==4'hA) && (tRegExc[15:12]==4'hA))
	begin
		if(tDelay2Exc[37:30]==tRegExc[37:30])
		begin
			tDelay2ExcAdv	= 1;
			tDelay3ExcAdv	= 1;
		end
	end
	else
//	if(tDelay2Exc[15] && !tRegExc[15])
	if(tDelay2Exc[15] && !tRegExc[15] && !crOutSr[28])
	begin
		tNxtRegExc		= tDelay2Exc;
		tDelay2ExcAdv	= 1;
		tDelay3ExcAdv	= 1;
	end
	else	if(tDelay3Exc[15] && !tDelay2Exc[15])
	begin
		tDelay2ExcAdv	= 1;
		tDelay3ExcAdv	= 1;
	end
	else	if(tDelay4Exc[15] && !tDelay3Exc[15])
	begin
		tDelay3ExcAdv	= 1;
	end
`endif


`ifndef def_true
	if(tDelay2ExcAdv)
	begin
		tNxtDelay2Exc	= tNxtDelay3Exc;
	end
	if(tDelay3ExcAdv)
	begin
		tNxtDelay3Exc	= tNxtDelay4Exc;
		tNxtDelay4Exc	= 0;
	end
`endif

`ifndef def_true
//	if(tDelayExc[15])
	if(tDelayExc[15] && !tRegExc[15])
	begin
//		tNxtRegExc = tDelayExc;

		if(tDelayExc[15:12]==4'h8)
		begin
			tNxtRegExc = tDelayExc;
		end
		else if(!crOutSr[28])
		begin
			if((tDelayExc[15:12]!=4'hC) || crOutSr[3])
				tNxtRegExc = tDelayExc;
		end
	end
`endif

//	if(memRegExc[15])
	if(memRegExc[15] && !tRegExc[15])
	begin
//		$display("memRegExc: %X", memRegExc);
//		tNxtRegExc = memRegExc;

		if(memRegExc[15:12]==4'h8)
		begin
			tNxtRegExc = memRegExc;
		end
		else if(!crOutSr[28])
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
				tNxtRegExc = memRegExc;
		end
`ifndef def_true
// `ifdef def_true
		else
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
			begin
//				if(!tDelay2Exc[15])
//					tNxtDelay2Exc = memRegExc;

//				if(!tDelay4Exc[15])
//					tNxtDelay4Exc = memRegExc;
			end
		end
`endif
	end
	else if(memRegExc[15])
	begin
`ifndef def_true
		if(memRegExc[15:12]==4'h8)
		begin
//			if(!tDelay2Exc[15])
//				tNxtDelay2Exc = memRegExc;

//			tNxtDelay2Exc = memRegExc;

//			if(!tDelay4Exc[15])
//				tNxtDelay4Exc = memRegExc;

		end
//		else if(!crOutSr[28])
		else
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
			begin
//				if(!tDelay2Exc[15])
//					tNxtDelay2Exc = memRegExc;
//				tNxtDelay2Exc = memRegExc;

//				if(!tDelay4Exc[15])
//					tNxtDelay4Exc = memRegExc;
			end
		end
`endif
	end

`ifndef jx2_use_ringbus
//	if(memBusExc[15] && (!opBraFlushMask[3]) &&
//		exHold1N && exHold2N && (ex1MemOpm==0))
//	if(memBusExc[15] && (!opBraFlushMask[0]) &&
//		exHold1N && exHold2N && !crOutSr[28])
//	if(memBusExc[15] && (!opBraFlushMask[3]) &&
//		exHold2N && !crOutSr[28])
//	if(memBusExc[15] && !crOutSr[28])
	if(memBusExc[15] && !crOutSr[28] && !tRegExc[15])
	begin
//		if((memBusExc[15:12]!=4'hC) || crOutSr[3])
		if((memBusExc[15:12]!=4'hC) || crOutSr[3])
			tNxtRegExc = memBusExc;
	end
	else if(memBusExc[15])
	begin
//		$display("Missed IRQ Flush=%d H1=%d H2=%d SR.BL=%d SR=%X",
//			opBraFlushMask[0],
//			exHold1, exHold2, crOutSr[28], crOutSr);
	end
`endif

//	if(ex1TrapExc[15])
//	if(ex1TrapExc[15] && !tRegExc[15])
//		tNxtRegExc = ex1TrapExc;

	if(ex1TrapExcB[15] && !tRegExc[15])
		tNxtRegExc = ex1TrapExcB;

	if(ex1SloMuTrap[7] && !tRegExc[15])
		tNxtRegExc = {
			UV64_00, UV48_00,
			ex1SloMuTrap[7:5], 8'h00, ex1SloMuTrap[4:0] };

	if(ex1FpuTrapB[7] && !tRegExc[15])
		tNxtRegExc = {
			UV64_00, UV48_00,
			ex1FpuTrapB[7:5], 8'h00, ex1FpuTrapB[4:0] };

`ifdef jx2_enable_ldekey
//	if(ex1KrreLo[65:64]==2'b11)
	if((ex1KrreLo[65:64]==2'b11) && !tRegExc[15])
	begin
		$display("ExUbit: Keyring Fault");

		$display("EX1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
			ex1ValBPc,	ex1RegValPc,
			ex1IstrWord[15: 0], ex1IstrWord[31:16],
			ex1IstrWord[47:32], ex1IstrWord[63:48],
			ex1IstrWord[79:64], ex1IstrWord[95:80],
			ex1OpUCmd, ex1OpUIxt, ex1BraFlush);

		tNxtRegExc = { UV64_00, UV48_00, 16'h800D };
	end
`endif

	if(exResetL2)
	begin
		exHold1		= 0;
//		exHold2		= 0;
		tNxtDeadlockLatch	= 0;
	end

	if(tNxtRegExc[15])
	begin
`ifdef jx2_debug_isr
		$display("tNxtRegExc %X PC=%X", tNxtRegExc, ifLastPc);
`endif
	end

	if(tRegExc[15] && !exResetL2)
	begin
//		$display("tRegExc %X", tRegExc);

//		if(!tNxtRegExc[15] && exHold2 && (tRegExc[15:12]!=4'hF))
//		if(!tNxtRegExc[15] && exHold2)
//		if(!tNxtRegExc[15] && (exHold2 || exHold1))
//		if(!tNxtRegExc[15] && !ifValBraOk)
//		if(!tNxtRegExc[15] && !(crOutSr[28] && crOutSr[29]))
		if(!tNxtRegExc[15] && (!ifValBraOk ||
			(!(crOutSr[28] && crOutSr[29]) && (tRegExc[15:12]!=4'hF))))
//		if(!tNxtRegExc[15] && (!ifValBraOk || exHold1 || exHold2))

//		if(!tNxtRegExc[15] && (!ifValBraOk || exHold1 || exHold2 ||
//			((ifOutPcStep==0) && (tRegExc[15:12]==4'hA))))

		begin
`ifdef jx2_debug_isr
			$display("EXC Sustain %X PC=%X Inh=%X",
				tRegExc, ifLastPc, tRegExcCycInh);
//			$display("\t%d %d %d",
//				ifValBraOk, exHold1, exHold2);
`endif
			tNxtRegExc = tRegExc;

			tNxtRegExcCycInh = tRegExcCycInh + 1;
		end
	end

`ifndef def_true
	if(tNxtRegExc[15])
	begin
		if(crOutSr[28] && crOutSr[29] &&
//			(tNxtRegExc[15:12]==4'hA) && ifValBraOk)
//			(tNxtRegExc[15:12]==4'hA) && ifValBraOk && exHold1N)
			(tNxtRegExc[15:12]==4'hA) && ifValBraOk &&
				!(exHold1 || exHold2 || (ifOutPcStep==0)))
		begin
			$display("EXC Filter %X", tNxtRegExc);
//			tNxtRegExc = 0;
			tNxtRegExc[15] = 0;
		end
		
		if(tRegExcCycInh[11:9] == 3'b111)
		begin
			$display("EXC Filter Timeout %X", tNxtRegExc);
//			tNxtRegExc[15] = 0;
		end
	end
`endif

	if(exResetL2)
	begin
		$display("ExUnit: RESET");
//		tNxtRegExc = 0;
//		tNxtRegExc[15] = 0;
		tNxtRegExcCycInh = 0;

//		tNxtDelayExc[15] = 0;
`ifndef def_true
		tNxtDelay2Exc[15] = 0;
		tNxtDelay3Exc[15] = 0;
		tNxtDelay4Exc[15] = 0;
`endif
	end

	ifInPcHold	= exHold1;
	dcInHold	= exHold2;

//	exHold1		= 0;
//	exHold2		= 0;

	/* Advance PC */
	tValStepPc		= { 28'b0, ifOutPcStep };
//	tValNextPc		= ifLastPc + tValStepPc;
//	tValNextPc		= { ifLastPc[47:32], ifLastPc[31:0] + tValStepPc };

`ifndef def_true
//	tValAddPc0		= {2'b0, ifOutPcStep} + { 1'b0, ifLastPc[4:1] };
	tValAddPc0		= {2'b0, ifOutPcStep} + { 1'b0, ifLastPc[4:0] };
	tValPcAdd1		= ifLastPc[31:5] + 1;
	tValNextPc		= { ifLastPc[47:32],
//		tValAddPc0[4] ? tValPcAdd1 : ifLastPc[31:5],
//		tValAddPc0[3:0], 1'b0 };
		tValAddPc0[5] ? tValPcAdd1 : ifLastPc[31:5],
		tValAddPc0[4:0] };
`endif

`ifdef def_true
	tValAddPc0p2	= {
		{ 5'b0, 3'b001 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p4	= {
		{ 5'b0, 3'b010 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p8	= {
		{ 5'b0, 3'b100 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p12	= {
		{ 5'b0, 3'b110 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };

	tValAddPc0p16	= {
		{ 5'b00001, 3'b000 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p18	= {
		{ 5'b00001, 3'b001 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p20	= {
		{ 5'b00001, 3'b010 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p24	= {
		{ 5'b00001, 3'b100 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };
	tValAddPc0p28	= {
		{ 5'b00001, 3'b110 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };

	tValAddPc0p31	= {
		{ 8'b00001111 } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };

//	tValAddPc0		= {5'b0, ifOutPcStep} + { 1'b0, ifLastPc[7:0] };

	tValAddPc0		= {
		{ 5'b0, ifOutPcStep[3:1] } +
//		{ 5'b0, opBraFlushMask[3] ? 3'b000 : ifOutPcStep[3:1] } +
		{ 1'b0, ifLastPc[7:1] }, 1'b0 };

	tValPcAdd1L		= { 1'b0, ifLastPc[19: 8] } + 1;
	tValPcAdd1H		= { 1'b0, ifLastPc[31:20] } + 1;
//	tValPcAdd1		= ifLastPc[31:8] + 1;
	tValPcAdd1		= {
//		tValPcAdd1L[12] ? tValPcAdd1H[11:0] : ifLastPc[31:20],
		(ifLastPc[19: 8]==12'hFFF) ? tValPcAdd1H[11:0] : ifLastPc[31:20],
		tValPcAdd1L[11:0] };

//	tValNextPc		= { ifLastPc[47:32],
//		tValAddPc0[8] ? tValPcAdd1 : ifLastPc[31:8],
//		tValAddPc0[7:0] };

`ifdef def_true
	tValPcAdd2p2 = {
		tValAddPc0p2[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p2[7:0] };
	tValPcAdd2p4 = {
		tValAddPc0p4[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p4[7:0] };
	tValPcAdd2p8 = {
		tValAddPc0p8[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p8[7:0] };
	tValPcAdd2p12 = {
		tValAddPc0p12[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p12[7:0] };

	tValPcAdd2p16 = {
		tValAddPc0p16[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p16[7:0] };
	tValPcAdd2p18 = {
		tValAddPc0p18[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p18[7:0] };
	tValPcAdd2p20 = {
		tValAddPc0p20[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p20[7:0] };
	tValPcAdd2p24 = {
		tValAddPc0p24[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p24[7:0] };
	tValPcAdd2p28 = {
		tValAddPc0p28[8] ? tValPcAdd1 : ifLastPc[31:8],
		tValAddPc0p28[7:0] };

//	tValNextPcInc = { tValAddPc0p31[8] ? tValPcAdd1 : ifLastPc[31:8],
//		tValAddPc0p31[7:0] };

	case(ifOutPcStep[3:1])
		3'b000:		tValNextPc = ifLastPc;
		3'b001:		tValNextPc = { ifLastPc[47:32], tValPcAdd2p2 };
		3'b010:		tValNextPc = { ifLastPc[47:32], tValPcAdd2p4 };
		3'b100:		tValNextPc = { ifLastPc[47:32], tValPcAdd2p8 };
		3'b110:		tValNextPc = { ifLastPc[47:32], tValPcAdd2p12 };
		default:	tValNextPc = ifLastPc;
	endcase

	case(ifOutPcStep[3:1])
		3'b000:		tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p16 };
		3'b001:		tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p18 };
		3'b010:		tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p20 };
		3'b100:		tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p24 };
		3'b110:		tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p28 };
		default:	tValNextPcInc = { ifLastPc[47:32], tValPcAdd2p16 };
	endcase
`endif
`endif

	if(ifOutPcLow[11:0] != ifLastPc[11:0])
	begin
		$display("ExUnit: PC-Low Mismatch %X %X",
			ifOutPcLow[11:0], ifLastPc[11:0]);
	end

	tValNextPcHi	= crOutPcHi;

	tOpNextPc		= tValNextPc;

	tValNextPcBra		= ifLastPc;
	tValNextPcIncBra	= { ifLastPc[47:32], tValPcAdd2p16 };
	tValNextPcDoBra = 0;

	/* Hold current PC if branching. */
//	if(opBraFlushMask[3])
//		tValNextPc		= ifLastPc;

	if(opBraFlushMask[3])
		tValNextPcDoBra = 1;

	tIsrNextPc		= tValNextPc;

	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };

`ifdef jx2_enable_prebra
	if((id1PreBra==2'b01) && !opBraFlushMask[2])		//ID1
	begin
//		$display("ExUnit PreBra %X->%X", ifLastPc, id1PreBraPc);
//		tValNextPc = id1PreBraPc;
//		tValNextBraPc = id1PreBraPc;

		tValNextPcBra		= id1PreBraPc;
		tValNextPcIncBra	= id1PreBraPcInc;
		tValNextPcDoBra	= 1;
	end
`endif

//	if((idA1IdUCmd[5:0]==JX2_UCMD_BRA) && idA1IdUCmd[7] && id1PreBra)
//	begin
//		$display("ExUnit: Predict CC Branch ID1");
//	end

`ifndef def_true
	if(ex1OpUCmd[5:0]==JX2_UCMD_JCMP)
	begin
		$display("JCMP X=%X-%X-%X-%X Rs=%X Rt=%X", 
			ex1IstrWord[15: 0], ex1IstrWord[31:16],
			ex1IstrWord[47:32], ex1IstrWord[63:48],
			ex1RegIdRs, ex1RegIdRt);
	end
`endif

	ex1ValBraDir = 3'b001;
//	if(ex1OpUCmd[5:0]==JX2_UCMD_BRA)
//	if(ex1OpUCmd[5:0]==JX2_UCMD_BRA)
//	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] &&
//		!ex1BraFlush && exHold2N)
	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] &&
		!ex1BraFlush)
	begin
		ex1ValBraDir[1:0] = ex1OpUCmd[7:6];
//		ex1ValBraDir[2]	= !(ex1OpUCmd[6]^ex2RegOutSr[0]);
//		ex1ValBraDir[2]	= (ex1OpUCmd[6]^ex2RegOutSr[0]);
		ex1ValBraDir[2]	= (ex1OpUCmd[6]^ex1AluInSr[0]);
//		ex1ValBraDir[2]	= !(ex1OpUCmd[6]^ex1AluInSr[0]);

`ifdef jx2_debug_hitmiss
		if(ex1PreBra[1])
		begin
//			if(tBraNonBra!=UV16_00)
//				tBraNxtNonBra = tBraNonBra - 1;
		end
		else
		begin
			if(tBraNonBra!=UV16_FF)
				tBraNxtNonBra = tBraNonBra + 1;

	//		if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
			if(ex1RegIdCn1 == JX2_CR_PC)
			begin
				if(tBraHitMiss!=UV16_00)
					tBraNxtHitMiss = tBraHitMiss - 1;
			end
			else
			begin
				if(tBraHitMiss!=UV16_FF)
					tBraNxtHitMiss = tBraHitMiss + 1;
			end
		end

		tBraNxtCycCnt	= tBraCycCnt + 1;
`endif

	end

	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && !ex1OpUCmd[7] &&
		!ex1BraFlush)
	begin
`ifdef jx2_debug_hitmiss
		if(ex1PreBra[1])
		begin
//			if(tBraNonBra!=UV16_00)
//				tBraNxtNonBra = tBraNonBra - 1;
		end
		else
		begin
			if(tBraNonBra!=UV16_FF)
				tBraNxtNonBra = tBraNonBra + 1;

	//		if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
			if(ex1RegIdCn1 == JX2_CR_PC)
			begin
				if(tBraHitMiss!=UV16_00)
					tBraNxtHitMiss = tBraHitMiss - 1;
			end
			else
			begin
				if(tBraHitMiss!=UV16_FF)
					tBraNxtHitMiss = tBraHitMiss + 1;
			end
		end

		tBraNxtCycCnt	= tBraCycCnt + 1;
`endif
	end

`ifdef jx2_alu_jcmpz
	if((ex1OpUCmd[5:0]==JX2_UCMD_JCMPZ) && !ex1BraFlush)
	begin
		ex1ValBraDir[1:0] = 2'b10;
		ex1ValBraDir[2]	= ((ex1RegIdCn1 == JX2_CR_PC) ^ ex1PreBra[0]);
	end
`endif


`ifdef jx2_alu_jcmp
	if((ex1OpUCmd[5:0]==JX2_UCMD_JCMP) && !ex1BraFlush)
	begin
		ex1ValBraDir[1:0] = 2'b10;
		ex1ValBraDir[2]	= ((ex1RegIdCn1 == JX2_CR_PC) ^ ex1PreBra[0]);
	end
`endif


`ifdef jx2_debug_hitmiss
	if(tBraCycCnt[15]!=tBraNxtCycCnt[15])
//	if(tBraCycCnt[12]!=tBraNxtCycCnt[12])
	begin
		$display("ExUnit: Branch Hit/Miss Count (Mid=8000): %X IsBra=%X",
			tBraHitMiss, tBraNonBra);
		tBraNxtHitMiss	= 16'h8000;
//		tBraNxtNonBra	= 16'h8000;
		tBraNxtNonBra	= 16'h0000;
	end
`endif

//	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] && ex1PreBra)
//	begin
//		$display("ExUnit: Predict CC Branch EX1");
//	end

	/* Handle if EX1 unit has initiated a branch. */
//	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };
//	if(({1'b1, ex1RegIdCn1} == JX2_CR_PC) && !ex1PreBra)
//	if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
	if(ex1RegIdCn1 == JX2_CR_PC)
//	if((ex1RegIdCn1 == JX2_CR_PC) && !ex1BraFlush)
//	if((ex1RegIdCn1 == JX2_CR_PC) && !ex1TrapFlush)
//	if((ex1RegIdCn1 == JX2_CR_PC) && !tRegExc[15] && !ifValBraOk)

//	if((ex1RegIdCn1 == JX2_CR_PC) && !tRegExc[15] &&
//		!ifValBraOk && ((ifOutPcStep!=0) || tRegExc[15:12]==4'hF))
	begin
//		if(crOutSr[28])
//		begin
//			$display("EX1 BRA %X", ex1RegValCn1);
//		end
//		tValNextPc = ex1RegValCn1[31:0];
//		tValNextBraPc = ex1RegValCn1[31:0];
		tValNextBraPc = ex1RegValCn1[47:0];
`ifdef jx2_enable_vaddr96
		tValNextBraPcHi = ex1OutPcHi[47:0];
`endif
		tValNextBraSrT = ex1RegValCn1[63:48];
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
//		nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;
//		ex1ValBraDir[2]	= 1;

		if(	(ex1RegOutSr[27:26]!=crOutSr[27:26]) ||
			(ex1RegOutSr[23:20]!=crOutSr[23:20]) )
		begin
`ifdef jx2_debug_isr
			$display("ExUnit: Do Slow Branch");
`endif
			nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;
		end

//		if(ex1RegValCn1[19:12]!=ifLastPc[19:12])
//			nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;

		if(ex1BraFlush)
		begin
			$display("EX1 Invalid BRA %X", ex1RegValCn1);
		end

`ifndef def_true
//		if(ex1PreBra)
		if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7])
			$display("ExUnit: Branch Miss, CC=%X, Rat=%X, PC=%X Istr=%X-%X",
				ex1OpUCmd[7:6], tBraHitMiss,
				ex1ValBPc,
				ex1IstrWord[15: 0], ex1IstrWord[31:16]);
`endif
	end

`ifndef def_true
	/* EX2 can branch, ignore if already in a branch. */
//	if(({1'b1, ex2RegIdCn2} == JX2_CR_PC) && !ex2PreBra &&
	if((ex2RegIdCn2 == JX2_CR_PC) && !ex2PreBra &&
		!opBraFlushMask[0])
	begin
//		$display("EX2 BRA %X", ex2RegValCn2);
//		tValNextPc = ex2RegValCn2[31:0];
//		tValNextBraPc = ex2RegValCn2[31:0];
		tValNextBraPc = ex2RegValCn2[47:0];
		tValNextBraSrT = ex2RegValCn2[63:48];
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
	end
`endif

	if(exResetL2)
	begin
//		tValNextPc = UV32_00;
//		tValNextBraPc = UV32_00;
		tValNextBraPc = UV48_00;
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
//		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;
		tNxtDeadlockLatch	= 0;
	end
	
	case(tRegExc[14:13])
		2'b00: tRegExcOfs=1;
		2'b01: tRegExcOfs=3;
		2'b10: tRegExcOfs=2;
		2'b11: tRegExcOfs=4;
	endcase

	crInSr			= ex1RegOutSr;
	gprInSp			= ex1RegOutSp;
	crInLr			= ex1RegOutLr;

	gprInDlr		= ex1RegOutDlr;
	gprInDhr		= ex1RegOutDhr;

	tIsrBraPc			= tValNextBraPc;
	tIsrBraFlushMask	= nxtBraFlushMask;

	braNxtInSpc		= crOutSpc;
	braNxtInExsr	= crOutExsr;
	braNxtInTea		= crOutTea;
	braNxtInTeaHi	= crOutTeaHi;
	braNxtInSr		= ex1RegOutSr;
	braNxtInLr		= ex1RegOutLr;
	braNxtInDlr		= ex1RegOutDlr;
	braNxtInDhr		= ex1RegOutDhr;
	braNxtInSp		= ex1RegOutSp[47:0];
	braNxtInSsp		= crOutSsp;
	braNxtIsIsr		= 0;

`ifndef def_true
	if(braIsIsr)
	begin
		braNxtInSpc		= braInSpc;
		braNxtInExsr	= braInExsr;
		braNxtInTea		= braInTea;
		braNxtInTeaHi	= braInTeaHi;
		braNxtInSr		= braInSr;
		braNxtInLr		= braInLr;
		braNxtInDlr		= braInDlr;
		braNxtInDhr		= braInDhr;
		braNxtInSp		= braInSp;
		braNxtInSsp		= braInSsp;
	end
`endif

//	if(exResetL2)
//	begin
//		crInSr			= 0;
//		gprInSp			= 64'h00000DFF0;
//	end

//	tDelayExc		<= tNxtDelayExc;

	tNxtExcLatch=0;
	if(tRegExc[15])
	begin
//		$display("tRegExc: %X l=%d", tRegExc, tExcLatch);
		if(exHold2N)
			tNxtExcLatch=1;
	end

	tNxtDelayExc	= 0;
	tRegSkipExc		= 0;
`ifndef def_true
// `ifdef def_true
	if(tRegExc[15:12]==4'hC)
	begin
`ifndef def_true
		if(crOutSr[28])
			tRegSkipExc		= 1;
		if(exHold1 || exHold2)
		begin
			tRegSkipExc		= 1;
			tNxtDelayExc	= tRegExc;
		end
`endif
	end
	else
	begin
`ifndef def_true
// `ifdef def_true
		if(tRegExc[15] && tExcLatch &&
			(tRegExc[15:12] != 4'hF) &&
			(tRegExc[15:12] != 4'hA))
		begin
//			tRegSkipExc		= 1;
		end

		if(crOutSr[28] && crOutSr[29] &&
			(tRegExc[15:12]==4'hA) && ifValBraOk)
//		if(crOutSr[28] && crOutSr[29] &&
//			(tRegExc[15:12]==4'hA))
		begin
			tRegSkipExc		= 1;
		end
`endif
	end
`endif

//	if(tRegExc[15] && !crOutSr[28])
	if(tRegExc[15] && !tRegSkipExc)
	begin
		if(tRegExc[15:10]==6'b111111)
		begin
`ifdef jx2_debug_isr
			$display("ExUnit: RTE, SPC=%X SR0=%X SR1=%X",
				crOutSpc,
				crOutSr[31: 0],
				crOutExsr[63:32]);
`endif
			tValNextBraPc	= crOutSpc;
			tValNextBraPcHi	= crOutSpcHi;
//			nxtBraFlushMask = JX2_BRA_FLUSHMSK;
			nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;
			tValNextBraPc[0]	= 0;

`ifndef jx2_isr2stage
			crInExsr		= crOutExsr;
			crInSr			= { crOutSr[63:32], crOutExsr[63:32] };
`endif

`ifndef def_true
// `ifdef def_true
			if(exL4RegInLr != crOutLr)
				$display("RTE: LR Mismatch, %X -> %X",
					exL4RegInLr, crOutLr);
			if(exL4RegInDlr != gprOutDlr)
				$display("RTE: DLR Mismatch, %X -> %X",
					exL4RegInDlr, gprOutDlr);
			if(exL4RegInDhr != gprOutDhr)
				$display("RTE: DHR Mismatch, %X -> %X",
					exL4RegInDhr, gprOutDhr);

			if(crOutSr[29] && crOutSr[30])
			begin
				if(exL4RegInSp[47:0] != crOutSsp[47:0])
					$display("RTE: SP Mismatch A, %X -> %X",
						exL4RegInSp, crOutSsp);
			end
			else
			begin
				if(exL4RegInSp[47:0] != gprOutSp[47:0])
					$display("RTE: SP Mismatch B, %X -> %X",
						exL4RegInSp, gprOutSp);
			end

			if(exL4RegInSr != crOutExsr[63:32])
				$display("RTE: SR Mismatch, %X -> %X",
					exL4RegInSr, crOutExsr[63:32]);
`endif

//			if(crOutSr[29])
			if(crOutSr[29] && crOutSr[30])
			begin
//`ifndef jx2_reg_spdecswap
`ifdef def_true

`ifndef jx2_isr2stage
				gprInSp			= { UV16_00, crOutSsp };
				crInSsp			= ex1RegOutSp[47:0];
`endif

`ifdef jx2_isr2stage
				braNxtInSp		= crOutSsp;
				braNxtInSsp		= ex1RegOutSp[47:0];
`endif

`endif
			end

`ifdef jx2_isr2stage
			braNxtInExsr	= crOutExsr;
//			braNxtInTea		= crOutTea;
			braNxtInSr		= { crOutSr[63:32], crOutExsr[63:32] };
//			braNxtInLr		= ex1RegOutLr;
//			braNxtInDlr		= ex1RegOutDlr;
//			braNxtInDhr		= ex1RegOutDhr;
			braNxtIsIsr		= 1;
`endif

`ifndef def_true
			if(crOutSr[30] && (crInSr[29] || crInSr[28]))
			begin
				$display("RTE, Exit RB Set, SPC=%X SR=",
					crOutSpc, crInSr);
			end

			if(exHold2)
			begin
				$display("ExUnit: EX1=%d EX2=%d EX3=%d",
					ex1Hold[0], ex2Hold[0], ex3Hold[0]);
				$display("ExUnit: D$=%d I$=%d FPU=%d",
					dcOutHold, ifOutPcOK[1], ex1FpuOK[1]);
			end
`endif

		end
		else if(!crOutSr[28] || !ifValBraOk)
		begin
// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_isr
//			if(tRegExc[15:12]!=4'b1010)
			if((tRegExc[15:12]!=4'b1010) && (tRegExc[15:12]!=4'b1110))
			begin
				$display("ExUnit: Fault %X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
					tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
			end
`endif
		
			tValNextBraPc = {
//				crOutVbr[47:8],
//				crOutVbr[7: 3]+tRegExcOfs[4:0],
				crOutVbr[47:7],
				crOutVbr[6: 3]+tRegExcOfs[3:0],
				crOutVbr[2:0] };
			tValNextBraPcHi = crOutVbrHi;
//			nxtBraFlushMask = JX2_BRA_FLUSHMSK;
			nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;

			braIsrSp	= ex1RegOutSp[47:0];
			crInSpcHi	= crOutPcHi;

			if(tRegExc[15:12]!=4'b1010)
			begin

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_isr
				if(crOutSr[30] && (crOutSr[29] || crOutSr[28]))
				begin
					$display("Fault, RB Set, SPC=%X SR=",
						crOutSpc, crOutSr);
				end
`endif

				if(!crOutSr[28])
				begin
`ifdef jx2_debug_isr
					$display("ISR VBR=%X PC=%X, SPC=%X PC.b=%X",
						crOutVbr, tValNextBraPc, crInSpc, tValBraPc);
`endif
				end
			end

			if(!crOutSr[28])
//			if(!crOutSr[28] && !braIsIsr)
			begin

`ifndef jx2_isr2stage
//				crInExsr		= { crOutSr[31:8], ex2RegOutSr[7:0],
//					UV16_00, tRegExc[15:0] };

				crInExsr		= {
					crOutSr[31:28],
					ex2RegOutSr[27:20],
					crOutSr[19:16],
					ex2RegOutSr[15:0],
					UV16_00, tRegExc[15:0] };

				crInTea			= { UV16_00, tRegExc[63:16] };
				crInTeaHi		= { tRegExc[127:64] };
				crInSr			= ex1RegOutSr;
				crInSr[30:28]	= 3'b111;
//				crInSr[27:26]	= 2'b00;
				crInSr[27:26]	= crOutVbrCm[ 3: 2];
				crInSr[23:20]	= crOutVbrCm[15:12];

				crInSr[19:18]	= crOutVbrCm[ 1: 0];

				crInLr			= crOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
`endif

`ifdef jx2_isr2stage

				braNxtInExsr		= {
//					crOutSr[31:8], ex2RegOutSr[7:0],
					crOutSr[31:28], ex2RegOutSr[27:20],
					crOutSr[19:16], ex2RegOutSr[15:0],
					UV16_00, tRegExc[15:0] };

				braNxtInTea			=  { UV16_00, tRegExc[63:16] };
				braNxtInTeaHi		=  { tRegExc[127:64] };
				braNxtInSr			= ex1RegOutSr;
				braNxtInSr[30:28]	= 3'b111;
//				braNxtInSr[27:26]	= 2'b00;
				braNxtInSr[27:26]	= crOutVbrCm[ 3: 2];
				braNxtInSr[23:20]	= crOutVbrCm[15:12];
				braNxtInLr			= crOutLr;
//				braNxtInDlr			= ex1RegOutDlr;
//				braNxtInDhr			= ex1RegOutDhr;
				braNxtIsIsr			= 1;
`endif

`ifndef def_true
				$display("gprOutSp = %X", gprOutSp);
				$display("ex2RegOutSp = %X", ex2RegOutSp);
				$display("ex1RegOutSp = %X", ex1RegOutSp);

				$display("id2RegValSp = %X", id2RegValSp);
				$display("ex1RegValSp = %X", ex1RegValSp);
				$display("ex2RegValSp = %X", ex2RegValSp);
				$display("ex3RegValSp = %X", ex3RegValSp);
				$display("exL4RegInSp = %X", exL4RegInSp);

				$display("id2RegValLr = %X", id2RegValLr);
				$display("ex1RegValLr = %X", ex1RegValLr);
				$display("ex2RegValLr = %X", ex2RegValLr);
				$display("ex3RegValLr = %X", ex3RegValLr);
				$display("exL4RegInLr = %X", exL4RegInLr);
`endif

// `ifdef def_true
`ifndef def_true
//				if(1'b1)
				if(ex3BraFlush && ex2BraFlush && ex1BraFlush)
				begin
					$display("ISR from EXL4, Msk=%X/%X",
						nxtBraFlushMask, opBraFlushMask);

//					$display("  EX1 PC=%X %X",
//						exL4ValBPc, exL4IstrWord);
//					$display("  ID2 PC=%X %X",
//						id2ValBPc, id2IstrWord);
//					$display("  ID1 PC=%X %X",
//						id1ValBPc, id1IstrWord);
//					$display("  IF PC=%X / %X", ifLastPc, tIsrNextPc);

`ifndef jx2_isr2stage
					crInSpc			= exL4ValBPc;
//					crInExsr[39:32]	= exL4RegInLastSr;
					crInExsr[31: 0]	= exL4RegInSr;
					crInLr			= exL4RegInLr;
					gprInDlr		= exL4RegInDlr;
					gprInDhr		= exL4RegInDhr;
					braIsrSp		= exL4RegInSp[47:0];
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= exL4ValBPc;
//					braNxtInExsr[39:32]	= exL4RegInLastSr;
					crInExsr[31: 0]		= exL4RegInSr;
					braNxtInLr			= exL4RegInLr;
					braNxtInDlr			= exL4RegInDlr;
					braNxtInDhr			= exL4RegInDhr;
					braIsrSp			= exL4RegInSp[47:0];
`endif

					ex3TrapFlush	= 1;
					ex2TrapFlush	= 1;
					ex1TrapFlush	= 1;

				end
				else
`endif

// `ifndef def_true
`ifdef def_true
				if(!ex3BraFlush)
//				if(!ex3BraFlush &&
//					(ex2BraFlush  || (ex2OpUCmd==0) ||
//						((	(ex3RegIdRm!=ex1RegIdRs) &&
//							(ex3RegIdRm!=ex1RegIdRt)) ||
//							(ex3RegIdRm==JX2_GR_ZZR)) )
//					)
//				if(!ex3BraFlush &&
//					(ex2BraFlush || (ex2OpUCmd==0)))
//					(ex2BraFlush ||
//						(ex2OpUCmd[5:1]!=2) ||
//						(ex3OpUCmd[5:1]==2)))
				begin
`ifdef jx2_debug_isr
					$display("ISR from EX3 Op=%X-%X Exc=%X",
						ex3OpUCmd, ex3OpUIxt, tRegExc);
					$display("    EX2 Op=%X-%X",
						ex2OpUCmd, ex2OpUIxt);

					if(!(ex2BraFlush || (ex2OpUCmd==0)))
						$display("ISR: EX3 Diverge");
`endif

`ifndef jx2_isr2stage
					crInSpc			= ex3ValBPc;
					crInExsr[39:32]	= ex3RegInLastSr[7:0];
//					crInExsr[39:32]	= ex2RegInLastSr[7:0];

					crInExsr[55:52]	= ex3RegInLastSr[23:20];
					crInExsr[59:58]	= ex3RegInLastSr[27:26];

					crInLr			= ex3RegInLr;
//					crInLr			= ex3RegValLr;
					gprInDlr		= gprOutDlr;
//					gprInDlr		= ex3RegValDlr;
					gprInDhr		= gprOutDhr;
//					gprInDhr		= ex3RegValDhr;
					braIsrSp		= gprOutSp[47:0];
//					braIsrSp		= ex3RegValSp;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex3ValBPc;
					braNxtInExsr[39:32]	= ex3RegInLastSr[7:0];
					braNxtInLr			= ex3RegInLr;
					braNxtInDlr			= gprOutDlr;
//					braNxtInDlr			= ex3RegValDlr;
					braNxtInDhr			= gprOutDhr;
					braIsrSp			= gprOutSp[47:0];
`endif

					ex3TrapFlush	= 1;
					ex2TrapFlush	= 1;
					ex1TrapFlush	= 1;

				end
				else
`endif
//`ifndef def_true
`ifdef def_true
				if(!ex2BraFlush)
				begin
`ifdef jx2_debug_isr
					$display("ISR from EX2 Op=%X-%X Exc=%X",
						ex2OpUCmd, ex2OpUIxt, tRegExc);
`endif

`ifndef jx2_isr2stage
					crInSpc			= ex2ValBPc;

					crInExsr[39:32]	= ex2RegInLastSr[7:0];

					crInExsr[55:52]	= ex2RegInLastSr[23:20];
					crInExsr[59:58]	= ex2RegInLastSr[27:26];

					crInLr			= ex2RegInLr;
//					crInLr			= ex2RegValLr;
					gprInDlr		= gprOutDlr;
//					gprInDlr		= ex2RegValDlr;
					gprInDhr		= gprOutDhr;
//					gprInDhr		= ex2RegValDhr;
					braIsrSp		= gprOutSp[47:0];
//					braIsrSp		= ex2RegValSp;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex2ValBPc;
					braNxtInExsr[39:32]	= ex2RegInLastSr[7:0];
					braNxtInLr			= ex2RegInLr;
//					braNxtInDlr			= gprOutDlr;
					braNxtInDlr			= ex2RegValDlr;
//					braNxtInDhr			= gprOutDhr;
					braNxtInDhr			= ex2RegValDhr;
					braIsrSp			= gprOutSp[47:0];
`endif

					ex2TrapFlush	= 1;
					ex1TrapFlush	= 1;

				end
				else
`endif
				if(!ex1BraFlush)
				begin
`ifdef jx2_debug_isr
					$display("ISR from EX1 Op=%X-%X Exc=%X",
						ex1OpUCmd, ex1OpUIxt, tRegExc);
`endif

`ifndef jx2_isr2stage
					crInSpc			= ex1ValBPc;
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInExsr[55:52]	= ex2RegInLastSr[23:20];
//					crInExsr[59:58]	= ex2RegInLastSr[27:26];

//					crInExsr[55:52]	= ex2RegOutSr[23:20];	//Unsure
//					crInExsr[59:58]	= ex2RegOutSr[27:26];

					crInExsr[55:52]	= ex1ValFetchSr[23:20];	//Unsure
					crInExsr[59:58]	= ex1ValFetchSr[27:26];

//					crInLr			= ex1RegInLr;
//					crInLr			= crOutLr;
//					gprInDlr		= gprOutDlr;
					gprInDlr		= ex1RegValDlr;
//					gprInDhr		= gprOutDhr;
					gprInDhr		= ex1RegValDhr;
//					braIsrSp		= gprOutSp;
//					braIsrSp		= ex2RegOutSp[47:0];
					braIsrSp		= ex1RegValSp;
					crInLr			= ex1RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex1ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
					braNxtInLr			= crOutLr;
//					braNxtInDlr			= gprOutDlr;
					braNxtInDlr			= ex1RegValDlr;
//					braNxtInDhr			= gprOutDhr;
					braNxtInDhr			= ex1RegValDhr;
//					braIsrSp			= gprOutSp;
//					braIsrSp			= ex2RegOutSp;
					braIsrSp			= ex1RegValSp;
`endif

					ex1TrapFlush	= 1;

				end
//				else if(!opBraFlushMask[0])
//				else if(!nxtBraFlushMask[0])
				else if(!tIsrBraFlushMask[0])
				begin
`ifdef jx2_debug_isr
					$display("ISR from ID2");
`endif

`ifndef jx2_isr2stage
					crInSpc			= id2ValBPc;
//					crInExsr[39:32]	= crOutSr[7:0];

					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInExsr[55:52]	= ex2RegOutSr[23:20];
//					crInExsr[59:58]	= ex2RegOutSr[27:26];

					crInExsr[55:52]	= id2ValFetchSr[23:20];	//Unsure
					crInExsr[59:58]	= id2ValFetchSr[27:26];

//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
//					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= id2ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
//					braNxtInDlr			= gprOutDlr;
//					braNxtInDhr			= gprOutDhr;
//					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif

				end
//				else if(!opBraFlushMask[1])
//				else if(!nxtBraFlushMask[1])
				else if(!tIsrBraFlushMask[1])
				begin
`ifdef jx2_debug_isr
//					$display("ISR from ID1, Msk=%X", opBraFlushMask);
					$display("ISR from ID1, Msk=%X/%X",
						nxtBraFlushMask, opBraFlushMask);
					$display("  EX1 PC=%X %X",
						ex1ValBPc, ex1IstrWord);
					$display("  ID2 PC=%X %X",
						id2ValBPc, id2IstrWord);
					$display("  ID1 PC=%X %X",
						id1ValBPc, id1IstrWord);
					$display("  IF PC=%X", ifLastPc);
`endif

`ifndef jx2_isr2stage
					crInSpc			= id1ValBPc;
//					crInExsr[39:32]	= crOutSr[7:0];
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInExsr[55:52]	= ex2RegOutSr[23:20];
//					crInExsr[59:58]	= ex2RegOutSr[27:26];

					crInExsr[55:52]	= id1ValFetchSr[23:20];	//Unsure
					crInExsr[59:58]	= id1ValFetchSr[27:26];

//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
//					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= id1ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif
				end

// `ifndef def_true
`ifdef def_true
				else
				begin
`ifdef jx2_debug_isr
//					$display("ISR from IF");
//					$display("ISR from IF, Msk=%X", opBraFlushMask);
					$display("ISR from IF, Msk=%X/%X",
						nxtBraFlushMask, opBraFlushMask);
					$display("  EX1 PC=%X %X",
						ex1ValBPc, ex1IstrWord);
					$display("  ID2 PC=%X %X",
						id2ValBPc, id2IstrWord);
					$display("  ID1 PC=%X %X",
						id1ValBPc, id1IstrWord);
					$display("  IF PC=%X / %X", ifLastPc, tIsrNextPc);
`endif

`ifndef jx2_isr2stage
					crInSpc			= ifLastPc;

//					crInExsr[39:32]	= crOutSr[7:0];
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInExsr[55:52]	= ex2RegOutSr[23:20];
//					crInExsr[59:58]	= ex2RegOutSr[27:26];

//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;

//					tIsrBraPc			= tValNextBraPc;
//					tIsrBraFlushMask	= nxtBraFlushMask;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ifLastPc;
//					braNxtInExsr[39:32]	= crOutSr[7:0];
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif

`ifdef jx2_bra2stage
					if(opBraFlushMask[4])
					begin
`ifdef jx2_debug_isr
						$display("ISR In Branch %X", tIsrBraPc);
`endif

`ifndef jx2_isr2stage
//						crInSpc = tIsrBraPc;
`else
//						braNxtInSpc		= tIsrBraPc;
`endif
					end
`else
					if(tIsrBraFlushMask[3])
					begin
`ifdef jx2_debug_isr
						$display("ISR In Branch %X", tIsrBraPc);
`endif

`ifndef jx2_isr2stage
//						crInSpc = tIsrBraPc;
`else
//						braNxtInSpc		= tIsrBraPc;
`endif
					end
`endif

				end
`endif

`ifdef jx2_debug_isr
				$display("ISR VBR=%X PC=%X, SPC=%X",
					crOutVbr, tValNextBraPc, crInSpc);
`endif

				if(tRegExc[15:12]==4'b1110)
				begin
`ifndef jx2_isr2stage
	//				crInSpc			= id1ValBPc;
					crInSpc			= ex1ValBPc;
`else
					braNxtInSpc		= ex1ValBPc;
`endif
				end

//				if(!crOutSr[29])
				if(!(crOutSr[29] && crOutSr[30]))
				begin
//`ifndef jx2_reg_spdecswap
`ifdef def_true

`ifndef jx2_isr2stage
//					crInSsp			= ex1RegOutSp[47:0];
					crInSsp			= braIsrSp[47:0];
					gprInSp			= { UV16_00, crOutSsp };
`else
//					braNxtInSsp		= ex1RegOutSp[47:0];
					braNxtInSsp		= braIsrSp[47:0];
					braNxtInSp		= crOutSsp;
`endif

`endif
				end

			end
		end
		else
		begin
`ifndef def_true
			$display("ExUnit: Double-Fault EXC=%X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
				tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
`endif
		end
	end


`ifdef jx2_isr2stage

// `ifndef def_true
`ifdef def_true
	if(braIsIsr)
	begin
		crInSpc		= braInSpc;
		crInExsr	= braInExsr;
		crInTea		= braInTea;
		crInTeaHi	= braInTeaHi;
		crInSr		= braInSr;

// `ifndef def_true
`ifdef def_true
		crInLr		= braInLr;
		gprInDlr	= braInDlr;
		gprInDhr	= braInDhr;
`endif

//`ifndef jx2_reg_spdecswap
`ifdef def_true
		gprInSp		= { UV16_00, braInSp };
		crInSsp		= braInSsp;
`endif
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(braNxtIsIsr)
	begin
		crInSpc		= braNxtInSpc;
		crInExsr	= braNxtInExsr;
		crInTea		= braNxtInTea;
		crInTeaHi	= braNxtInTeaHi;
		crInSr		= braNxtInSr;
		crInLr		= braNxtInLr;
		gprInDlr	= braNxtInDlr;
		gprInDhr	= braNxtInDhr;
//`ifndef jx2_reg_spdecswap
`ifdef def_true
		gprInSp		= { UV16_00, braNxtInSp };
		crInSsp		= braNxtInSsp;
`endif
	end
`endif

`endif

`ifdef jx2_reg_spdecswap
	gprInSp		= gprOutSp;
	crInSsp		= crOutSsp;
`endif

`ifdef def_true
//	if(ifOutPcSxo[3:2]!=0)
	if(id1IstrSxo[3:2]!=0)
	begin
//		case(ifOutPcSxo[3:2])
		case(id1IstrSxo[3:2])
			2'b00: begin end
			2'b01: crInSr[27:26]	= 2'b00;
			2'b10: crInSr[27:26]	= 2'b01;
			2'b11: crInSr[27:26]	= 2'b11;
		endcase
//		crInVbr[51:50]=crInSr[27:26];
		if(crInSr[27:26]!=crOutSr[27:26])
			$display("ExUnit: Trigger RV Mode, %X->%X",
				crOutSr[27:26], crInSr[27:26]);
	end
`endif

// `ifdef def_true
`ifndef def_true
//	if(id1ValBPc[47:4]==0)
	if(	!crOutMmcr[0] &&
		(id1ValBPc[31:16]==0) &&
		(id1ValBPc[15:4]==0))
	begin
		if(id1IstrWord[47:40]==8'h00)
		begin
			if(id1IstrWord[39:32]==8'h13)
			begin
				/* Hack: Detect ROM is in RISC-V Mode */
				crInSr[27:26]	= 2'b01;
				crInSr[23:20]	= 4'b0000;
			end
			if(id1IstrWord[39:32]==8'h0A)
			begin
				/* Hack: Detect ROM is in XG3RV Mode */
				crInSr[27:26]	= 2'b11;
				crInSr[23:20]	= 4'b0000;
			end

			if(crInSr[26] && !crOutSr[26] && !ifValBraOk)
			begin
				$display("ExUnit: Boot-Branch into RV Mode PC=%X", id1ValBPc);
				tValNextBraPc = 0;
				tValNextBraSrT = { 8'b00, crInSr[23:20], crInSr[27:26], 2'b0};
				nxtBraFlushMask = JX2_BRA_FLUSHMSK_XTRA;
			end
		end
	end
`endif


// reg			tIsrBranchMsgLatch;
// reg			tNxtIsrBranchMsgLatch;

	tNxtIsrBranchMsgLatch = tIsrBranchMsgLatch;
	if(!crOutSr[28])
		tNxtIsrBranchMsgLatch = 0;

`ifdef jx2_bra2stage
	if(opBraFlushMask[4])
	begin
//		if(crOutSr[28])
//		if(crOutSr[28] && !ifNxtValBraOk && !tIsrBranchMsgLatch)
		if(crOutSr[28] && !ifValBraOk && !tIsrBranchMsgLatch)
		begin
`ifdef jx2_debug_isr
			$display("ISR Branch %X %X", tValNextBraPc, tValBraPc);
`endif
		end
		tNxtIsrBranchMsgLatch = 1;

//		tValNextPc		= tValBraPc;
		tValNextPcHi	= tValBraPcHi;

		tValNextPcBra		= tValBraPc;
		tValNextPcIncBra	= {tValBraPc[47:12], tValBraPc[11: 0]+ 12'h010 };
		tValNextPcDoBra	= 1;

`ifndef def_true
		if(!braIsIsr)
		begin
			crInSr[15: 4]	= tValBraSrT[15:4];
			crInSr[27:26]	= tValBraSrT[3:2];
			crInSr[ 1: 0]	= tValBraSrT[1:0];
		end
`endif
		
		ifNxtValBraOk = 1;
	end
`else
	if(nxtBraFlushMask[3])
	begin
//		if(crOutSr[28])
//			$display("Branch %X", tValNextBraPc);
//		tValNextPc		= tValNextBraPc;
		tValNextPcHi	= tValNextBraPcHi;
		tValNextPcBra	= tValNextBraPc;
		tValNextPcIncBra	= {tValBraPc[47:12], tValBraPc[11: 0]+ 12'h010 };
		tValNextPcDoBra	= 1;
		ifNxtValBraOk	= 1;
	end
`endif

	if(tValNextPcDoBra)
	begin
		tValNextPc		= tValNextPcBra;
		tValNextPcInc	= tValNextPcIncBra;
	end

	ifValPc			= tValNextPc;
	ifValPcHi		= tValNextPcHi;
	ifValPcInc		= tValNextPcInc;

	crInPc			= tValNextPc;
	crInPcHi		= tValNextPcHi;
//	crInSr			= ex1RegOutSr;
//	crInLr			= ex1RegOutLr;

	gprIdRn1		= ex1RegIdRn1;
	gprValRn1		= ex1RegValRn1;
	gprIdRn2		= ex2RegIdRn2;
	gprValRn2		= ex2RegValRn2;
	gprIdRn3		= ex3RegIdRn3;
	gprValRn3		= ex3RegValRn3;
	gprIdRn4		= ex3RegIdRn4;
	gprValRn4		= ex3RegValRn4;
	
	tDbgExWidth		= 1;

`ifdef jx2_enable_wex
	gprIdRnB1		= exB1RegIdRn1;
	gprValRnB1		= exB1RegValRn1;
	gprIdRnB2		= exB2RegIdRn2;
	gprValRnB2		= exB2RegValRn2;
	gprIdRnB3		= exB3RegIdRn3;
	gprValRnB3		= exB3RegValRn3;
	gprIdRnB4		= exB3RegIdRn4;
	gprValRnB4		= exB3RegValRn4;
	exB3RegAluRes	= 0;
	
	exB2RegAluRes	= exB1ValAlu;
	
	if(idB2IdUCmd[5:0]!=0)
		tDbgExWidth		= 2;
`endif

`ifdef jx2_enable_wex3w
	gprIdRnC1		= exC1RegIdRn1;
	gprValRnC1		= exC1RegValRn1;
	gprIdRnC2		= exC2RegIdRn2;
	gprValRnC2		= exC2RegValRn2;
	gprIdRnC3		= exC3RegIdRn3;
	gprValRnC3		= exC3RegValRn3;
	gprIdRnC4		= exC3RegIdRn4;
	gprValRnC4		= exC3RegValRn4;
	exC3RegAluRes	= 0;

	gprValImmA4		= ex1RegValImm;
	gprValImmB4		= exB1RegValImm;
	gprValImmC4		= exC1RegValImm;

	exC2RegAluRes	= exC1ValAlu;

	if(idC2IdUCmd[5:0]!=0)
		tDbgExWidth		= 3;
`endif

//`ifndef def_true
`ifndef jx2_gprs_mergecm
	crIdCn1			= ex1RegIdCn1;
	crValCn1		= ex1RegValCn1;
	crIdCn2			= ex2RegIdCn2;
	crValCn2		= ex2RegValCn2;
	crIdCn3			= ex3RegIdCn3;
	crValCn3		= ex3RegValCn3;
	crIdCn4			= ex3RegIdCn4;
	crValCn4		= ex3RegValCn4;
`endif

// `ifdef def_true
`ifdef jx2_gprs_mergecm
	crIdCn1			= ex1RegIdRn1;
	crValCn1		= ex1RegValRn1;
	crIdCn2			= ex2RegIdRn2;
	crValCn2		= ex2RegValRn2;

//`ifdef jx2_enable_movc
	crIdCn3			= ex3RegIdRn3;
	crValCn3		= ex3RegValRn3;
// `else
//	crIdCn3			= ex3RegIdRn2;
//	crValCn3		= ex3RegValRn2;
// `endif

	crIdCn4			= ex3RegIdRn4;
	crValCn4		= ex3RegValRn4;

`endif

	gprEx1Flush = ex1BraFlush || ex1TrapFlush;
	gprEx2Flush = ex2BraFlush || ex2TrapFlush;
	gprEx3Flush = ex3BraFlush || ex3TrapFlush;


	/* ID2 */

//	gprValGbr		= crOutGbr;
//	gprInDlr		= ex1RegOutDlr;
//	gprInDhr		= ex1RegOutDhr;
//	gprInSp			= ex1RegOutSp;

`ifdef jx2_sprs_elrehr
	gprInElr		= gprOutElr;
	gprInEhr		= gprOutEhr;
	gprInBp			= gprOutBp;
`endif


	/* EX1 */
	ex1RegInDlr		= gprOutDlr;
	ex1RegInDhr		= gprOutDhr;
	ex1RegInSp		= gprOutSp;
	ex1RegInLr		= crOutLr;

//	ex1RegInDlr		= ex2RegOutDlr;
//	ex1RegInDhr		= ex2RegOutDhr;
//	ex1RegInSp		= ex2RegOutSp;
//	ex1RegInLr		= ex2RegOutLr;

//	ex1RegInSr		= crOutSr;
//	ex1RegInSr		= ex2RegOutSr;
//	ex1RegInSchm	= ex2RegOutSchm;
	ex1RegInSchm	= 0;

	ex2RegAluRes	= ex1ValAlu;
	ex2RegMulRes	= ex1MulVal;
	ex2RegMulWRes	= ex1MulWVal;
//	ex2RegFpuGRn	= ex1FpuValGRn;
`ifdef jx2_enable_wex
	ex2RegAluResB	= exB1ValAlu;
`endif

//	ex3RegAluRes	= ex1ValAlu;
	ex3RegMulRes	= ex1MulVal;
//	ex3RegMulWRes	= ex1MulWVal;

	ex3RegAluRes	= 0;
//	ex3RegMulRes	= 0;
	ex3RegMulWRes	= 0;

	dcInAddr		= ex1MemAddr;
	dcInOpm			= ex1MemOpm;
	dcInLdOp		= ex1MemLdOp;

`ifdef jx2_enable_wex
	dcInAddrB		= exB1MemAddr;
	dcInOpmB		= exB1MemOpm;
`else
	dcInAddrB		= 0;
	dcInOpmB		= 0;
`endif

	dcInVal			= ex1MemDataOut;
//	dcInValB		= ex1MemDataOutB;
`ifdef jx2_enable_wex
`ifdef jx2_enable_memcap
	dcInValB		= exB1RegValRn1;
`else
	dcInValB[63:0]	= exB1RegValRn1[63:0];
	dcInValB[65:64]	= 0;
`endif
`else
	dcInValB		= 0;
`endif
	dcInTraPc		= { UV16_00, ex1ValBPc };

//	if(exB1RegIdRn1 == JX2_GR_DCINB)
//		dcInValB	= exB1RegValRn1;

`ifdef jx2_enable_wex
	exB2RegMulWRes	= exB1MulWVal;
//	exB3RegMulWRes	= exB1MulWVal;
	exB3RegMulWRes	= 0;
`ifdef jx2_enable_wex3w
	exC2RegMulWRes	= exC1MulWVal;
//	exC3RegMulWRes	= exC1MulWVal;
	exC3RegMulWRes	= 0;
`endif
`endif

	/* EX2 */

	ex2MemDataIn	= dcOutVal;
	ex2MemDataInB	= dcOutValB;
	ex2MemDataOK	= dcOutOK;

//	ex2RegInDlr		= gprOutDlr;
//	ex2RegInDhr		= gprOutDhr;
	ex2RegInSp		= gprOutSp;
//	ex2RegInLr		= crOutLr;
	ex2RegInSr		= crOutSr;
	ex2RegInSchm	= 0;

//	if((ex1PreBra==2'b10) && (ex1OpUCmd[5:0]==JX2_UCMD_BRA))
//	begin
//		$display("ExUnit BRA: EX2=%X EX1=%X ID2=%X",
//			ex2PreBra, ex1PreBra, id2PreBra);
//	end

end

(* max_fanout = 400 *)
//	wire exHold1Not2 = exHold1 && exHold2N;
	wire exHold1Not2 = exHold1 && !exHold2;

always @(posedge clock)
begin

`ifndef jx2_enable_fpu
//		ex1RegValFRs	<= UV64_XX;
//		ex1RegValFRt	<= UV64_XX;
`endif

	tPreHold1		<= tNxtPreHold1;
	tLstPreHold1	<= tPreHold1;
	exHold1L		<= exHold1;
	exResetL		<= reset;
	exResetL2		<= exResetL;

//	tRegExc			<= tNxtRegExc;
	tDelayExc		<= tNxtDelayExc;
	tDelay2Exc		<= tNxtDelay2Exc;
	tDelay3Exc		<= tNxtDelay3Exc;
	tDelay4Exc		<= tNxtDelay4Exc;

	tRegExcCycInh	<= tNxtRegExcCycInh;

	tNx1RegExc		<= tNxtRegExc;

`ifndef def_true
	if(tNxtRegExc[15] && (tNxtRegExc[15:12] != 4'hC))
//	if(1'b1)
//	if(tNxtRegExc[15])
	begin
		tRegExc		<= tNxtRegExc;
	end
`endif

`ifdef def_true
	if(tNx1RegExc[15] && (tNx1RegExc[15:12] != 4'hC))
	begin
		tRegExc		<= tNx1RegExc;
	end
`endif

	tHoldCycCnt		<= tHoldNxtCycCnt;
	tDeadlockLatch	<= tNxtDeadlockLatch;
	tExcLatch		<= tNxtExcLatch;

	tIsrBranchMsgLatch	<= tNxtIsrBranchMsgLatch;


`ifdef def_true
	braInSpc		<= braNxtInSpc;
	braInExsr		<= braNxtInExsr;
	braInTea		<= braNxtInTea;
	braInTeaHi		<= braNxtInTeaHi;
	braInSr			<= braNxtInSr;
	braInLr			<= braNxtInLr;
	braInDlr		<= braNxtInDlr;
	braInDhr		<= braNxtInDhr;
	braInSp			<= braNxtInSp;
	braInSsp		<= braNxtInSsp;
	braIsIsr		<= braNxtIsIsr;
`endif

`ifndef def_true
	if(exResetL2)
	begin
//		ifValPc			<= UV32_00;
//		opBraFlushMask	<= 8'h07;
//		opBraFlushMask	<= 8'h0F;
		opBraFlushMask	<= JX2_BRA_FLUSHMSK;
		ifLastPc		<= 0;
		ifLastPcStep	<= 0;
		ex1BraFlush		<= 1;
		ex2BraFlush		<= 1;
		ex3BraFlush		<= 1;

		ex1PredNoExec	<= 0;
		exB1PredNoExec	<= 0;
		exC1PredNoExec	<= 0;

		id1BraPipelineLrL	<= 0;

//		id1IstrWord		<= 96'h300030003000300030003000;
		id1IstrWord		<= 96'h3000F0003000F0003000F000;

`ifdef jx2_enable_wex
//		gprIdRs			<= JX2_GR_ZZR;
//		gprIdRt			<= JX2_GR_ZZR;
//		gprIdRu			<= JX2_GR_ZZR;
//		gprIdRv			<= JX2_GR_ZZR;
		gprIdRm			<= JX2_GR_ZZR;
		gprIdRn			<= JX2_GR_ZZR;

		idA2IdUCmd		<= UV8_00;
		idA2IdUIxt		<= UV8_00;
		idB2IdUCmd		<= UV8_00;
		idB2IdUIxt		<= UV8_00;

`ifdef jx2_enable_wex3w
//		gprIdRx			<= JX2_GR_ZZR;
//		gprIdRy			<= JX2_GR_ZZR;
		gprIdRo			<= JX2_GR_ZZR;
		idC2IdUCmd		<= UV8_00;
		idC2IdUIxt		<= UV8_00;
`endif
`endif

		ex1OpUCmd		<= UV8_00;
		ex1OpUIxt		<= UV8_00;
		ex1PreBraPc		<= 0;
//		ex1PreBra		<= 0;
		ex1PreBra		<= 2'b11;

//		ex1IstrWord		<= UV32_XX;
//		ex1RegValPc		<= UV32_XX;
//		ex1RegValImm	<= UV33_XX;

//		ex1RegIdRs		<= JX2_GR_ZZR;
//		ex1RegIdRt		<= JX2_GR_ZZR;
		ex1RegIdRm		<= JX2_GR_ZZR;

`ifdef jx2_enable_wex
		exB1OpUCmd		<= UV8_00;
		exB1OpUIxt		<= UV8_00;
//		exB1RegIdRs		<= JX2_GR_ZZR;
//		exB1RegIdRt		<= JX2_GR_ZZR;
		exB1RegIdRm		<= JX2_GR_ZZR;

`ifdef jx2_enable_wex3w
		exC1OpUCmd		<= UV8_00;
		exC1OpUIxt		<= UV8_00;
//		exC1RegIdRs		<= JX2_GR_ZZR;
//		exC1RegIdRt		<= JX2_GR_ZZR;
		exC1RegIdRm		<= JX2_GR_ZZR;
`endif
`endif

	end
	else
`endif

		if(exHold1N)
//		if(exHold1N || (tNxtRegExc[15:13]==3'b111))
//		if(!opBraFlushMask[3])
	begin
//		tRegExc			<= tNxtRegExc;
		tRegExc			<= tNx1RegExc;

`ifdef jx2_debug_hitmiss
		tBraHitMiss		<= tBraNxtHitMiss;
		tBraNonBra		<= tBraNxtNonBra;
		tBraCycCnt		<= tBraNxtCycCnt;
`endif

// `ifdef jx2_bra2stage
`ifdef def_true
		tValBraPc		<= tValNextBraPc;
		tValBraPcHi		<= tValNextBraPcHi;
		tValBraSrT		<= tValNextBraSrT;
`endif

`ifndef def_true
		braInSpc		<= braNxtInSpc;
		braInExsr		<= braNxtInExsr;
		braInTea		<= braNxtInTea;
		braInTeaHi		<= braNxtInTeaHi;
		braInSr			<= braNxtInSr;
		braInLr			<= braNxtInLr;
		braInDlr		<= braNxtInDlr;
		braInDhr		<= braNxtInDhr;
		braInSp			<= braNxtInSp;
		braInSsp		<= braNxtInSsp;
		braIsIsr		<= braNxtIsIsr;
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("IF : PC=%X D=%X-%X-%X-%X Step=%X PC2=%X F=%d", ifLastPc,
			ifIstrWord[15: 0], ifIstrWord[31:16],
			ifIstrWord[47:32], ifIstrWord[63:48],
			ifOutPcStep, tValNextPc, opBraFlushMask[3]);

`ifdef jx2_enable_wex
		$display("ID1: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			id1ValBPc,	id1ValPc,
			id1IstrWord[15: 0], id1IstrWord[31:16],
			idA1IdUCmd, idA1IdUIxt, idB1IdUCmd, idB1IdUIxt, opBraFlushMask[2]);
//		$display("     RsA=%X RtA=%X RnA=%X",
//			idA1IdRegM, idA1IdRegO, idA1IdRegN);
//		$display("     RsB=%X RtB=%X RnB=%X",
//			idB1IdRegM, idB1IdRegO, idB1IdRegN);
		$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
			idA1IdRegM, idA1IdRegO, idB1IdRegM, idB1IdRegO,
			idA1IdRegN, idB1IdRegN);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X Ry=%X Ro=%X",
			idC1IdRegM, idC1IdRegO, idC1IdRegN);
`endif

		$display("ID2: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			id2ValBPc,	gprValPc,
			id2IstrWord[15: 0], id2IstrWord[31:16],
			idA2IdUCmd, idA2IdUIxt, idB2IdUCmd, idB2IdUIxt,
			opBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gprIdRs, gprValRs,
			gprIdRt, gprValRt,
			gprIdRm, gprValRm);
		$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
			gprIdRu, gprValRu,
			gprIdRv, gprValRv,
			gprIdRn, gprValRn);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
			gprIdRx, gprValRx,
			gprIdRy, gprValRy,
			gprIdRo, gprValRo);
`endif

`else
		$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			id1ValBPc,	id1ValPc,
			id1IstrWord[15: 0], id1IstrWord[31:16],
			id1IdUCmd, id1IdUIxt, opBraFlushMask[2]);
		$display("     Rs=%X Rt=%X Rn=%X",
			id1IdRegM, id1IdRegO, id1IdRegN);

		$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			id2ValBPc,	gprValPc,
			id2IstrWord[15: 0], id2IstrWord[31:16],
			id2IdUCmd, id2IdUIxt, opBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gprIdRs, gprValRs,
			gprIdRt, gprValRt,
			gprIdRm, gprValRm);
`endif
//		$display("");
`endif

`ifndef def_true
// `ifdef def_true
		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex2ValBPc,	ex2RegValPc,
			ex2IstrWord[15: 0], ex2IstrWord[31:16],
			ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex2RegIdRs, ex2RegValRs,
			ex2RegIdRt, ex2RegValRt,
			ex2RegIdRm, ex2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex2RegIdRn2, ex2RegValRn2,
			ex2RegIdCn2, ex2RegValCn2);

//		$display("");
`endif

		/* IF */
		ifLastPc		<= ifValPc;
		ifLastPcStep	<= ifOutPcStep;
		opBraFlushMask	<= nxtBraFlushMask;
//		ifValPc			<= tValNextPc;
		ifValBraOk		<= ifNxtValBraOk;
//		ifValBraOk		<= ifNxtValBraOk && (ifOutPcStep!=0);
		ifValFetchSr	<= crOutSr;


		/* ID1 */

//		id1ValBPc		<= ifValPc;
		id1ValBPc		<= ifLastPc;
//		id1ValPc		<= ifValPc + tValStepPc;
//		id1ValPc		<= tValNextPc;
		id1ValPc		<= tOpNextPc;
		id1IstrWord		<= ifIstrWord;
		id1IstrMTag		<= ifIstrMTag;
		id1IstrSxo		<= ifOutPcSxo;
//		id1ValFetchSr	<= crOutSr;
		id1ValFetchSr	<= ifValFetchSr;
		id1PcStep		<= ifOutPcStepAdj;

`ifdef jx2_enable_prebra
//		id1BraPipelineLrL	<= id1BraPipelineLr;
//		id1BraPipelineLrL	<= { id1BraPipelineR1, id1BraPipelineLr };
		id1BraPipelineLrL	<= {
			crOutSr[23:22],
			crOutSr[27:26],
			1'b1,
			id1BraPipelineDbl,
			id1BraPipelineR1, id1BraPipelineLr };
`endif

		/* ID2 */

`ifdef jx2_enable_wex
		gprIdRs			<= idA1IdRegM;
		gprIdRt			<= idA1IdRegO;
		gprIdRu			<= idB1IdRegM;
		gprIdRv			<= idB1IdRegO;
		gprIdRm			<= idA1IdRegN;
		gprIdRn			<= idB1IdRegN;

		gprValImm		<= idA1IdImm;
		gprValImmB		<= idB1IdImm;

		idA2IdUCmd		<= idA1IdUCmd;
		idA2IdUIxt		<= idA1IdUIxt;
		idB2IdUCmd		<= idB1IdUCmd;
		idB2IdUIxt		<= idB1IdUIxt;

`ifdef jx2_enable_wex3w
		gprIdRx			<= idC1IdRegM;
		gprIdRy			<= idC1IdRegO;
		gprIdRo			<= idC1IdRegN;
		gprValImmC		<= idC1IdImm;
		idC2IdUCmd		<= idC1IdUCmd;
		idC2IdUIxt		<= idC1IdUIxt;
`endif

//		crIdCm			<= idA1IdRegM[4:0];
//		crIdCm			<= idA1IdRegM;

`ifdef jx2_enable_movc

`ifndef jx2_enable_movclite
		crIdCm			<=
			(idA1IdUCmd[5:0] == JX2_UCMD_MOV_RM) ? 
				idC1IdRegO : idA1IdRegM;
`else
		crIdCm			<= idA1IdRegM;
`endif

`else
		crIdCm			<= idA1IdRegM;
`endif

		gprValPc		<= id1ValPc;
		id2PreBraPc		<= id1PreBraPc;
		id2PreBra		<= id1PreBra;
		id2ValFetchSr	<= id1ValFetchSr;
		ex1ValFetchSr	<= id2ValFetchSr;

//		id1IstrWordL1	<= nxtBraFlushMask[2] ? UV32_00: id1IstrWord[31:0];
		id1IstrWordL1	<= nxtBraFlushMask[2] ? UV96_00: id1IstrWord[95:0];
		id1IstrWordL2	<= id1IstrWordL1;

`ifdef jx2_debug_keepinstr
// `ifdef def_true
//		id2IstrWord		<= id1IstrWord[31:0];
		id2IstrWord		<= id1IstrWord;
//		id2IstrWordL	<= id2IstrWord;
		id2ValBPc		<= id1ValBPc;
`else
//		id2IstrWord		<= UV32_XX;
//		id2ValBPc		<= UV48_XX;
		id2IstrWord		<= UV32_00;
		id2ValBPc		<= UV48_00;
`endif

`else
		gprIdRs			<= id1IdRegM;
		gprIdRt			<= id1IdRegO;
		gprIdRm			<= id1IdRegN;
		gprValPc		<= id1ValPc;
		gprValImm		<= id1IdImm;
//		crIdCm			<= id1IdRegM[4:0];
		crIdCm			<= id1IdRegM;

`ifndef jx2_enable_wex
		gprValImmB		<= id1IdImmB;
`endif

//		id1IstrWordL1	<= nxtBraFlushMask[2] ? UV32_00: id1IstrWord[31:0];
		id1IstrWordL1	<= nxtBraFlushMask[2] ? UV96_00: id1IstrWord[95:0];
		id1IstrWordL2	<= id1IstrWordL1;

//		id2ValBPc		<= id1ValBPc;
		id2IdUCmd		<= id1IdUCmd;
		id2IdUIxt		<= id1IdUIxt;
		id2PreBraPc		<= id1PreBraPc;
		id2PreBra		<= id1PreBra;
//		id2IstrWord		<= id1IstrWord[31:0];

`ifdef jx2_debug_keepinstr
//		id2IstrWord		<= id1IstrWord[31:0];
		id2IstrWord		<= id1IstrWord[95:0];
		id2ValBPc		<= id1ValBPc;
`else
//		id2IstrWord		<= UV32_XX;
//		id2ValBPc		<= UV48_XX;
		id2IstrWord		<= UV32_00;
		id2ValBPc		<= UV48_00;
`endif

`endif

		/* EX1 */

`ifdef jx2_debug_keepinstr
		ex1ValBPc		<= id2ValBPc;
		ex1IstrWord		<= id2IstrWord;
`else
//		ex1ValBPc		<= UV48_XX;
//		ex1IstrWord		<= UV32_XX;
		ex1ValBPc		<= UV48_00;
		ex1IstrWord		<= UV32_00;
`endif

		ex1RegPcHi		<= crOutPcHi;
//		ex1RegLrHi		<= crOutLrHi;
		ex1RegGbrHi		<= crOutGbrHi;

//		ex1RegValSp		<= gprOutSp[47:0];
		ex1RegValSp		<= id2RegValSp;
		ex1RegValLr		<= id2RegValLr;
		ex1RegValDlr	<= id2RegValDlr;
		ex1RegValDhr	<= id2RegValDhr;

//		ex1RegValSp		<= (ex2RegIdRn2==JX2_GR_SP) ?
//			ex2RegValRn2[47:0] : gprOutSp[47:0];

//		ex1RegValSp		<=
//			(ex1RegIdRn1==JX2_GR_SP) ?
//			ex1RegValRn1[47:0] :
//			(ex2RegIdRn2==JX2_GR_SP) ?
//			ex2RegValRn2[47:0] :
//			(ex3RegIdRn2==JX2_GR_SP) ?
//			ex3RegValRn2[47:0] :
//			gprOutSp[47:0];

// assign		ex1RegGbrHi = crOutGbrHi;

//		ex1OpUCmd		<= id2IdUCmd;
//		ex1OpUCmd		<= {
//			opBraFlushMask[0] ? JX2_IXC_NV : id2IdUCmd[7:6],
//			id2IdUCmd[5:0] };
//		ex1OpUIxt		<= id2IdUIxt;
		ex1PreBraPc		<= id2PreBraPc;
		ex1PreBra		<= id2PreBra;
//		ex1PreBra		<= exHold1L ? { 1'b1, id2PreBra[0] } : id2PreBra;
//		ex1PreBra		<= exHold1L ? 2'b11 : id2PreBra;

		ex1RegValPc		<= gprValPc;
		ex1RegValImm	<= gprValImm;
//		ex1BraFlush		<= nxtBraFlushMask[0];
		ex1BraFlush		<= nxtBraFlushMask[0] || (ex1PreBra==2'b01);
		
//		ex1Timers		<= timers[11:0];
		ex1Timers		<= { timers[11:1], memOpmIn[15] };

`ifdef jx2_cpu_pred_id2

		if((ex1OpUCmd2[5:0] != ex1OpUCmd[5:0]) &&
			!ex1BraFlush && !ex1PredNoExec)
		begin
			$display("ExUnit: Pred Mismatch A:");
			$display("  EX1: %X-%X -> %X", ex1OpUCmd, ex1OpUIxt, ex1OpUCmd2);
			$display("  EX2: %X-%X ", ex2OpUCmd, ex2OpUIxt);
		end

		if(	(ex1OpUCmd2[5:0] == ex1OpUCmd[5:0]) &&
			(ex1OpUCmd[5:0] != JX2_UCMD_NOP) &&
			!ex1BraFlush && ex1PredNoExec)
		begin
			$display("ExUnit: Pred Mismatch B:");
			$display("  EX1: %X-%X ", ex1OpUCmd, ex1OpUIxt);
			$display("  EX2: %X-%X ", ex2OpUCmd, ex2OpUIxt);
		end

		ex1PredNoExec	<= idA2PredNoExec;
		exB1PredNoExec	<= idB2PredNoExec;
		exC1PredNoExec	<= idC2PredNoExec;
`else
		ex1PredNoExec	<= 0;
		exB1PredNoExec	<= 0;
		exC1PredNoExec	<= 0;
`endif

`ifdef jx2_enable_wex
		ex1OpUCmd		<= idA2IdUCmd;
		ex1OpUIxt		<= idA2IdUIxt;
		exB1OpUCmd		<= idB2IdUCmd;
		exB1OpUIxt		<= idB2IdUIxt;
		exB1RegValImm	<= gprValImmB;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;

		exB1RegIdRs		<= gprIdRu;
		exB1RegIdRt		<= gprIdRv;
		exB1RegIdRm		<= gprIdRn;
		exB1RegValRs	<= gprValRu;
		exB1RegValRt	<= gprValRv;

`ifndef jx2_cpu_merge_rxy
		ex1RegValRm		<= gprValRm;
		exB1RegValRm	<= gprValRn;
`endif

`ifdef jx2_enable_wex3w
		if(noLane3)
		begin
			exC1OpUCmd		<= 0;
			exC1OpUIxt		<= idC2IdUIxt;
			exC1RegValImm	<= 0;

			exC1RegIdRs		<= gprIdRx;
			exC1RegIdRt		<= gprIdRy;
			exC1RegIdRm		<= JX2_GR_ZZR;
			exC1RegValRs	<= gprValRx;
			exC1RegValRt	<= gprValRy;
			exC1RegValRm	<= 0;
		end
		else
		begin
			exC1OpUCmd		<= idC2IdUCmd;
			exC1OpUIxt		<= idC2IdUIxt;
			exC1RegValImm	<= gprValImmC;

			exC1RegIdRs		<= gprIdRx;
			exC1RegIdRt		<= gprIdRy;
			exC1RegIdRm		<= gprIdRo;
			exC1RegValRs	<= gprValRx;
			exC1RegValRt	<= gprValRy;
			exC1RegValRm	<= gprValRo;
//			exC1RegValRm	<= 0;
		end
`endif

`else
		ex1OpUCmd		<= id2IdUCmd;
		ex1OpUIxt		<= id2IdUIxt;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;

//		exB1RegIdRs		<= JX2_GR_ZZR;
//		exB1RegIdRt		<= JX2_GR_ZZR;
//		exB1RegIdRm		<= JX2_GR_ZZR;
		exB1RegValRs	<= 0;
//		exB1RegValRt	<= 0;
//		exB1RegValRm	<= 0;

//		exB1ValCarryD	<= 0;
		ex2RegAluResB	<= 0;
`endif

		ex1RegValCRm	<= crValCm;

`ifdef jx2_enable_fpu
//		ex1RegValFRs	<= gprValFRs;
//		ex1RegValFRt	<= gprValFRt;
`endif

	end
//	else
//		if(exHold2N)

//	if(exHold1 && exHold2N)
	if(exHold1Not2)
	begin
//		ex1OpUCmd		<= { JX2_IXC_NV, ex1OpUCmd[5:0] };
//		ex1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		ex1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		ex1OpUIxt		<= UV9_00;
		ex1BraFlush		<= 1;

`ifdef jx2_enable_wex
//		exB1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		exB1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		exB1OpUIxt		<= UV9_00;
`endif

`ifdef jx2_enable_wex3w
//		exC1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		exC1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		exC1OpUIxt		<= UV9_00;
`endif

		ex1PredNoExec	<= 0;
		exB1PredNoExec	<= 0;
		exC1PredNoExec	<= 0;

`ifdef def_true
//		ex1ValBPc		<= UV32_XX;
		ex1ValBPc		<= ex1ValBPc;
//		ex1OpUCmd		<= UV8_XX;
//		ex1OpUIxt		<= UV8_XX;
//		ex1OpUIxt		<= UV9_00;
		ex1PreBraPc		<= 0;
//		ex1PreBra		<= 0;
		ex1PreBra		<= 2'b11;
//		ex1IstrWord		<= UV32_XX;
//		ex1IstrWord		<= UV96_XX;
		ex1IstrWord		<= UV96_00;

//		ex1RegValPc		<= UV32_XX;
//		ex1RegValPc		<= UV48_XX;
		ex1RegValPc		<= ex1RegValPc;
//		ex1RegValImm	<= UV33_XX;
		ex1RegValImm	<= UV33_00;

//		ex1RegValSp		<= gprOutSp[47:0];
		ex1RegValSp		<= id2RegValSp;
		ex1RegValLr		<= id2RegValLr;
		ex1RegValDlr	<= id2RegValDlr;
		ex1RegValDhr	<= id2RegValDhr;

//		ex1RegValSp		<=
//			(ex1RegIdRn1==JX2_GR_SP) ?
//			ex1RegValRn1[47:0] :
//			(ex2RegIdRn2==JX2_GR_SP) ?
//			ex2RegValRn2[47:0] :
//			(ex3RegIdRn2==JX2_GR_SP) ?
//			ex3RegValRn2[47:0] :
//			gprOutSp[47:0];

		ex1RegIdRs		<= JX2_GR_ZZR;
		ex1RegIdRt		<= JX2_GR_ZZR;
		ex1RegIdRm		<= JX2_GR_ZZR;
//		ex1RegValRs		<= UVGPRV_XX;
//		ex1RegValRt		<= UVGPRV_XX;
		ex1RegValRs		<= UVGPRV_00;
		ex1RegValRt		<= UVGPRV_00;

`ifndef jx2_cpu_merge_rxy
		ex1RegValRm		<= UVGPRV_XX;
`endif

		ex1RegValCRm	<= UVGPRV_XX;

`ifdef jx2_enable_fpu
//		ex1RegValFRs	<= UVGPRV_XX;
//		ex1RegValFRt	<= UVGPRV_XX;
`endif

`ifdef jx2_enable_wex
		exB1RegValImm	<= UV33_XX;

		exB1RegIdRs		<= JX2_GR_ZZR;
		exB1RegIdRt		<= JX2_GR_ZZR;
		exB1RegIdRm		<= JX2_GR_ZZR;
		exB1RegValRs	<= UVGPRV_XX;
		exB1RegValRt	<= UVGPRV_XX;

`ifndef jx2_cpu_merge_rxy
		exB1RegValRm	<= UVGPRV_XX;
`endif

`endif

`ifdef jx2_enable_wex3w
		exC1RegValImm	<= UV33_XX;

		exC1RegIdRs		<= JX2_GR_ZZR;
		exC1RegIdRt		<= JX2_GR_ZZR;
		exC1RegIdRm		<= JX2_GR_ZZR;
		exC1RegValRs	<= UVGPRV_XX;
		exC1RegValRt	<= UVGPRV_XX;
		exC1RegValRm	<= UVGPRV_XX;
`endif

`endif
	end

	if(exHold2N)
	begin

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("EX1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex1ValBPc,	ex1RegValPc,
			ex1IstrWord[15: 0], ex1IstrWord[31:16],
			ex1OpUCmd, ex1OpUIxt, ex1BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
			ex1RegIdRs, ex1RegValRs,
			ex1RegIdRt, ex1RegValRt,
			ex1RegIdRm, ex1RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex1RegIdRn1, ex1RegValRn1,
			ex1RegIdCn1, ex1RegValCn1);

		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex2ValBPc,	ex2RegValPc,
			ex2IstrWord[15: 0], ex2IstrWord[31:16],
			ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex2RegIdRs, ex2RegValRs,
			ex2RegIdRt, ex2RegValRt,
			ex2RegIdRm, ex2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex2RegIdRn2, ex2RegValRn2,
			ex2RegIdCn2, ex2RegValCn2);

`ifdef jx2_enable_wex
		if(exB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exB2OpUCmd, exB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB2RegIdRs, exB2RegValRs,
				exB2RegIdRt, exB2RegValRt,
				exB2RegIdRm, exB2RegValRm);
			$display("     Rn1=%X(%X)",
				exB2RegIdRn2, exB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exC2OpUCmd, exC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC2RegIdRs, exC2RegValRs,
				exC2RegIdRt, exC2RegValRt,
				exC2RegIdRm, exC2RegValRm);
			$display("     Rn1=%X(%X)",
				exC2RegIdRn2, exC2RegValRn2);
		end
`endif

`endif


		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex3ValBPc,	ex3RegValPc,
			ex3IstrWord[15: 0], ex3IstrWord[31:16],
			ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex3RegIdRs, ex3RegValRs,
			ex3RegIdRt, ex3RegValRt,
			ex3RegIdRm, ex3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex3RegIdRn3, ex3RegValRn3,
			ex3RegIdCn3, ex3RegValCn3);

`ifdef jx2_enable_wex
		if(exB3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-B: Op=%X-%X",
				exB3OpUCmd, exB3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB3RegIdRs, exB3RegValRs,
				exB3RegIdRt, exB3RegValRt,
				exB3RegIdRm, exB3RegValRm);
			$display("     Rn1=%X(%X)",
				exB3RegIdRn3, exB3RegValRn3);
		end

`ifdef jx2_enable_wex3w
		if(exC3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-C: Op=%X-%X",
				exC3OpUCmd, exC3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC3RegIdRs, exC3RegValRs,
				exC3RegIdRt, exC3RegValRt,
				exC3RegIdRm, exC3RegValRm);
			$display("     Rn1=%X(%X)",
				exC3RegIdRn3, exC3RegValRn3);
		end
`endif

`endif

		$display("");
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_exopipe
		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex2ValBPc,	ex2RegValPc,
			ex2IstrWord[15: 0], ex2IstrWord[31:16],
			ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex2RegIdRs, ex2RegValRs,
			ex2RegIdRt, ex2RegValRt,
			ex2RegIdRm, ex2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex2RegIdRn2, ex2RegValRn2,
			ex2RegIdCn2, ex2RegValCn2);

`ifdef jx2_enable_wex

		if(exB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exB2OpUCmd, exB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB2RegIdRs, exB2RegValRs,
				exB2RegIdRt, exB2RegValRt,
				exB2RegIdRm, exB2RegValRm);
			$display("     Rn1=%X(%X)",
				exB2RegIdRn2, exB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exC2OpUCmd, exC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC2RegIdRs, exC2RegValRs,
				exC2RegIdRt, exC2RegValRt,
				exC2RegIdRm, exC2RegValRm);
			$display("     Rn1=%X(%X)",
				exC2RegIdRn2, exC2RegValRn2);
		end
`endif
`endif

		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex3ValBPc,	ex3RegValPc,
			ex3IstrWord[15: 0], ex3IstrWord[31:16],
			ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex3RegIdRs, ex3RegValRs,
			ex3RegIdRt, ex3RegValRt,
			ex3RegIdRm, ex3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex3RegIdRn3, ex3RegValRn3,
			ex3RegIdCn3, ex3RegValCn3);

`ifdef jx2_enable_wex
		if(exB3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-B: Op=%X-%X",
				exB3OpUCmd, exB3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB3RegIdRs, exB3RegValRs,
				exB3RegIdRt, exB3RegValRt,
				exB3RegIdRm, exB3RegValRm);
			$display("     Rn1=%X(%X)",
				exB3RegIdRn3, exB3RegValRn3);
		end

`ifdef jx2_enable_wex3w
		if(exC3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-C: Op=%X-%X",
				exC3OpUCmd, exC3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC3RegIdRs, exC3RegValRs,
				exC3RegIdRt, exC3RegValRt,
				exC3RegIdRm, exC3RegValRm);
			$display("     Rn1=%X(%X)",
				exC3RegIdRn3, exC3RegValRn3);
		end
`endif
`endif

//		$display("");
`endif

		ex1TrapExcB		<= ex1TrapExc;

		/* EX2 */
`ifdef jx2_debug_keepinstr
		ex2IstrWord		<= ex1IstrWord;
		ex2ValBPc		<= ex1ValBPc;
`else
		ex2IstrWord		<= UV32_XX;
		ex2ValBPc		<= UV48_XX;
`endif

//		ex2OpUCmd		<= ex1OpUCmd;
		ex2OpUCmd		<= ex1OpUCmd2;
		ex2OpUIxt		<= ex1OpUIxt;
		ex2PreBraPc		<= ex1PreBraPc;
		ex2PreBra		<= ex1PreBra;
//		ex2BraFlush		<= ex1BraFlush;
		ex2BraFlush		<= ex1BraFlush || ex1TrapFlush;
//		ex2RegInLastSr	<= ex1RegInSr[7:0];
		ex2RegInLastSr	<= ex1RegInSr[31:0];

		ex2RegIdRs		<= ex1RegIdRs;
		ex2RegIdRt		<= ex1RegIdRt;
		ex2RegIdRm		<= ex1RegIdRm;
		ex2RegValRs		<= ex1RegValRs;
		ex2RegValRt		<= ex1RegValRt;

`ifndef jx2_cpu_merge_rxy
		ex2RegValRm		<= ex1RegValRm;
`endif

//		ex2RegValFRs	<= ex1RegValFRs;
//		ex2RegValFRt	<= ex1RegValFRt;
		ex2RegValCRm	<= ex1RegValCRm;
		ex2RegIdRn1		<= ex1RegIdRn1;
		ex2RegValRn1	<= ex1RegValRn1;
		ex2RegIdCn1		<= ex1RegIdCn1;
		ex2RegValCn1	<= ex1RegValCn1;
		ex2RegValPc		<= ex1RegValPc;
		ex2RegValImm	<= ex1RegValImm;

		ex2RegInLr		<= ex1RegOutLr;
		ex2RegInDlr		<= ex1RegOutDlr;
		ex2RegInDhr		<= ex1RegOutDhr;

		ex2RegValSp		<= ex1RegValSp;
		ex2RegValLr		<= ex1RegValLr;
		ex2RegValDlr	<= ex1RegValDlr;
		ex2RegValDhr	<= ex1RegValDhr;

		ex2AguOutLea	<= ex1AguOutLea;
		ex2AguOutXLea	<= ex1AguOutXLea;

		ex2Delay1In		<= ex1Delay1Out;

`ifdef jx2_enable_wex
//		exB2OpUCmd		<= exB1OpUCmd;
		exB2OpUCmd		<= exB1OpUCmd2;
		exB2OpUIxt		<= exB1OpUIxt;

		exB2RegIdRs		<= exB1RegIdRs;
		exB2RegIdRt		<= exB1RegIdRt;
		exB2RegIdRm		<= exB1RegIdRm;
		exB2RegValRs	<= exB1RegValRs;
		exB2RegValRt	<= exB1RegValRt;

`ifndef jx2_cpu_merge_rxy
		exB2RegValRm	<= exB1RegValRm;
`endif

		exB2RegIdRn1	<= exB1RegIdRn1;
		exB2RegValRn1	<= exB1RegValRn1;
		exB2RegValImm	<= exB1RegValImm;
		exB2Delay1In	<= exB1Delay1Out;

`ifdef jx2_enable_wex3w
//		exC2OpUCmd		<= exC1OpUCmd;
//		exC2OpUCmd		<= exC1OpUCmd2;
		exC2OpUCmd		<= noLane3 ? 0 : exC1OpUCmd2;
		exC2OpUIxt		<= exC1OpUIxt;

		exC2RegIdRs		<= exC1RegIdRs;
		exC2RegIdRt		<= exC1RegIdRt;
//		exC2RegIdRm		<= exC1RegIdRm;
		exC2RegIdRm		<= noLane3 ? 0 : exC1RegIdRm;
		exC2RegValRs	<= exC1RegValRs;
		exC2RegValRt	<= exC1RegValRt;
//		exC2RegValRm	<= exC1RegValRm;
		exC2RegValRm	<= noLane3 ? 0 : exC1RegValRm;
//		exC2RegValRm	<= 0;
//		exC2RegIdRn1	<= exC1RegIdRn1;
		exC2RegIdRn1	<= noLane3 ? 0 : exC1RegIdRn1;
//		exC2RegValRn1	<= exC1RegValRn1;
		exC2RegValRn1	<= noLane3 ? 0 : exC1RegValRn1;
//		exC2RegValImm	<= exC1RegValImm;
		exC2RegValImm	<= noLane3 ? 0 : exC1RegValImm;
		exC2Delay1In	<= exC1Delay1Out;
`endif

`endif


		/* EX3 */
`ifdef jx2_debug_keepinstr
		ex3IstrWord		<= ex2IstrWord;
		ex3ValBPc		<= ex2ValBPc;
`else
		ex3IstrWord		<= UV32_XX;
		ex3ValBPc		<= UV48_XX;
`endif

		ex3OpUCmd		<= ex2OpUCmd;
		ex3OpUIxt		<= ex2OpUIxt;
		ex3PreBra		<= ex2PreBra;
//		ex3BraFlush		<= ex2BraFlush;
		ex3BraFlush		<= ex2BraFlush || ex2TrapFlush;
		ex3RegInLastSr	<= ex2RegInLastSr;

		ex3RegValSp		<= ex2RegValSp;
		ex3RegValLr		<= ex2RegValLr;
		ex3RegValDlr	<= ex2RegValDlr;
		ex3RegValDhr	<= ex2RegValDhr;

		ex3RegIdRs		<= ex2RegIdRs;
		ex3RegIdRt		<= ex2RegIdRt;
		ex3RegIdRm		<= ex2RegIdRm;
		ex3RegValRs		<= ex2RegValRs;
		ex3RegValRt		<= ex2RegValRt;

`ifndef jx2_cpu_merge_rxy
		ex3RegValRm		<= ex2RegValRm;
`endif

		ex3RegIdRn2		<= ex2RegIdRn2;
		ex3RegValRn2	<= ex2RegValRn2;
		ex3RegIdCn2		<= ex2RegIdCn2;
		ex3RegValCn2	<= ex2RegValCn2;
		ex3RegValPc		<= ex2RegValPc;
		ex3RegValImm	<= ex2RegValImm;

		ex3RegInLr		<= ex2RegOutLr;
		ex3RegInDlr		<= ex2RegOutDlr;
		ex3RegInDhr		<= ex2RegOutDhr;

`ifdef jx2_enable_wex
		exB3OpUCmd		<= exB2OpUCmd;
		exB3OpUIxt		<= exB2OpUIxt;

		exB3RegIdRs		<= exB2RegIdRs;
		exB3RegIdRt		<= exB2RegIdRt;
		exB3RegIdRm		<= exB2RegIdRm;
		exB3RegValRs	<= exB2RegValRs;
		exB3RegValRt	<= exB2RegValRt;

`ifndef jx2_cpu_merge_rxy
		exB3RegValRm	<= exB2RegValRm;
`endif

		exB3RegIdRn2	<= exB2RegIdRn2;
		exB3RegValRn2	<= exB2RegValRn2;
		exB3RegValImm	<= exB2RegValImm;

`ifdef jx2_enable_wex3w
//		exC3OpUCmd		<= exC2OpUCmd;
		exC3OpUCmd		<= noLane3 ? 0 : exC2OpUCmd;
		exC3OpUIxt		<= exC2OpUIxt;

		exC3RegIdRs		<= exC2RegIdRs;
		exC3RegIdRt		<= exC2RegIdRt;
//		exC3RegIdRm		<= exC2RegIdRm;
		exC3RegIdRm		<= noLane3 ? 0 : exC2RegIdRm;
		exC3RegValRs	<= exC2RegValRs;
		exC3RegValRt	<= exC2RegValRt;
//		exC3RegValRm	<= exC2RegValRm;
		exC3RegValRm	<= noLane3 ? 0 : exC2RegValRm;
//		exC3RegValRm	<= 0;
//		exC3RegIdRn2	<= exC2RegIdRn2;
		exC3RegIdRn2	<= noLane3 ? 0 : exC2RegIdRn2;
//		exC3RegValRn2	<= exC2RegValRn2;
		exC3RegValRn2	<= noLane3 ? 0 : exC2RegValRn2;
//		exC3RegValImm	<= exC2RegValImm;
		exC3RegValImm	<= noLane3 ? 0 : exC2RegValImm;
`endif

`endif

		/* WB */
		ewbRegIdRm		<= ex3RegIdRm;
`ifdef jx2_enable_wex3w
		ewb2RegIdRm		<= exC2RegIdRm;
		ewb3RegIdRm		<= exC3RegIdRm;
`endif

`ifdef def_true
		/* EXL4 */
//		if(!(ex3BraFlush || ex3TrapFlush || crOutSr[28]))
//		if(!(ex3BraFlush || ex3TrapFlush || crOutSr[28] || tRegExc[15]))
		if(!(ex3BraFlush || ex3TrapFlush))
		begin
			/*
			 * Try to keep track of the last non-flushed state.
			 */
		
			exL4ValBPc		<= ex3ValBPc;
			exL4IstrWord	<= ex3IstrWord;

			exL4OpUCmd		<= ex3OpUCmd;
			exL4OpUIxt		<= ex3OpUIxt;
			exL4PreBra		<= ex3PreBra;
			exL4BraFlush	<= ex3BraFlush || ex3TrapFlush;

			exL4RegIdRs		<= ex3RegIdRs;
			exL4RegIdRt		<= ex3RegIdRt;
			exL4RegIdRm		<= ex3RegIdRm;
			exL4RegValRs	<= ex3RegValRs;
			exL4RegValRt	<= ex3RegValRt;
			exL4RegValRm	<= ex3RegValRm;
			exL4RegIdRn2	<= ex3RegIdRn2;
			exL4RegValRn2	<= ex3RegValRn2;
			exL4RegIdCn2	<= ex3RegIdCn2;
			exL4RegValCn2	<= ex3RegValCn2;
			exL4RegValPc	<= ex3RegValPc;
			exL4RegValImm	<= ex3RegValImm;


			exL5ValBPc		<= exL4ValBPc;
			exL5IstrWord	<= exL4IstrWord;

			exL5OpUCmd		<= exL4OpUCmd;
			exL5OpUIxt		<= exL4OpUIxt;
			exL5PreBra		<= exL4PreBra;
			exL5BraFlush	<= exL4BraFlush;

			exL5RegIdRs		<= exL4RegIdRs;
			exL5RegIdRt		<= exL4RegIdRt;
			exL5RegIdRm		<= exL4RegIdRm;
			exL5RegValRs	<= exL4RegValRs;
			exL5RegValRt	<= exL4RegValRt;
			exL5RegValRm	<= exL4RegValRm;
			exL5RegIdRn2	<= exL4RegIdRn2;
			exL5RegValRn2	<= exL4RegValRn2;
			exL5RegIdCn2	<= exL4RegIdCn2;
			exL5RegValCn2	<= exL4RegValCn2;
			exL5RegValPc	<= exL4RegValPc;
			exL5RegValImm	<= exL4RegValImm;


			exL6ValBPc		<= exL5ValBPc;
			exL6IstrWord	<= exL5IstrWord;

			exL6OpUCmd		<= exL5OpUCmd;
			exL6OpUIxt		<= exL5OpUIxt;
			exL6PreBra		<= exL5PreBra;
			exL6BraFlush	<= exL5BraFlush;

			exL6RegIdRs		<= exL5RegIdRs;
			exL6RegIdRt		<= exL5RegIdRt;
			exL6RegIdRm		<= exL5RegIdRm;
			exL6RegValRs	<= exL5RegValRs;
			exL6RegValRt	<= exL5RegValRt;
			exL6RegValRm	<= exL5RegValRm;
			exL6RegIdRn2	<= exL5RegIdRn2;
			exL6RegValRn2	<= exL5RegValRn2;
			exL6RegIdCn2	<= exL5RegIdCn2;
			exL6RegValCn2	<= exL5RegValCn2;
			exL6RegValPc	<= exL5RegValPc;
			exL6RegValImm	<= exL5RegValImm;
		end
`endif

	end

	if(exHold2 && exHold1N)
	begin
		$display("ExUnit: Hold2 and !Hold1");
	end

	tDbgExHold1			<= exHold1;
	tDbgExHold2			<= exHold2;

	tDbgExHold1B		<= tDbgExHold1;
	tDbgExHold2B		<= tDbgExHold2;

	tDbgOutStatus1B		<= tDbgOutStatus1;
	tDbgOutStatus2B		<= tDbgOutStatus2;
	tDbgOutStatus3B		<= tDbgOutStatus3;
	tDbgOutStatus4B		<= tDbgOutStatus4;
	tDbgOutStatus5B		<= tDbgOutStatus5;
	tDbgOutStatus6B		<= tDbgOutStatus6;
	tDbgOutStatus7B		<= tDbgOutStatus7;
	tDbgOutStatus8B		<= tDbgOutStatus8;

`ifdef def_true
	tDbgOutStatus1C		<= tDbgOutStatus1B;
	tDbgOutStatus2C		<= tDbgOutStatus2B;
	tDbgOutStatus3C		<= tDbgOutStatus3B;
	tDbgOutStatus4C		<= tDbgOutStatus4B;
	tDbgOutStatus5C		<= tDbgOutStatus5B;
	tDbgOutStatus6C		<= tDbgOutStatus6B;
	tDbgOutStatus7C		<= tDbgOutStatus7B;
	tDbgOutStatus8C		<= tDbgOutStatus8B;
`endif

// reg[1:0]	tDbgExWidth;
	tDbgExWidthB	<= tDbgExWidth;
	tDbgExWidthC	<= tDbgExWidthB;

	tDbgMiscB	<= tDbgMisc;
	tDbgMiscC	<= tDbgMiscB;

end

endmodule

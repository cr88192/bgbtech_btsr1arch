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
GPR Module with 6 read ports and 3 write ports.
 */

`include "RegSpr_3W.v"

module RegGPR_6R3W(
	clock,
	reset,
	hold,

	regIdUCmd,
	regIdUIxt,

	regIdRs,		//Source A, ALU / Base (Lane 1)
	regIdRt,		//Source B, ALU / Index (Lane 1)
	regIdRu,		//Source C, ALU / Base (Lane 2)
	regIdRv,		//Source D, ALU / Index (Lane 2) / MemStore
	regIdRx,		//Source E, ALU / Base (Lane 3)
	regIdRy,		//Source F, ALU / Index (Lane 3) / MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRu,		//Source C Value
	regValRv,		//Source D Value
	regValRx,		//Source E Value
	regValRy,		//Source F Value

	regIdRnA1,		//Destination ID (EX1, Lane 1)
	regValRnA1,		//Destination Value (EX1, Lane 1)
	regIdRnB1,		//Destination ID (EX1, Lane 2)
	regValRnB1,		//Destination Value (EX1, Lane 2)
	regIdRnC1,		//Destination ID (EX1, Lane 3)
	regValRnC1,		//Destination Value (EX1, Lane 3)

	regIdRnA2,		//Destination ID (EX2, Lane 1)
	regValRnA2,		//Destination Value (EX2, Lane 1)	
	regIdRnB2,		//Destination ID (EX2, Lane 2)
	regValRnB2,		//Destination Value (EX2, Lane 2)
	regIdRnC2,		//Destination ID (EX2, Lane 3)
	regValRnC2,		//Destination Value (EX2, Lane 3)

	regIdRnA3,		//Destination ID (EX2, Lane 1)
	regValRnA3,		//Destination Value (EX2, Lane 1)
	regIdRnB3,		//Destination ID (EX2, Lane 2)
	regValRnB3,		//Destination Value (EX2, Lane 2)
	regIdRnC3,		//Destination ID (EX2, Lane 3)
	regValRnC3,		//Destination Value (EX2, Lane 3)
	
	regIdRnA4,		//Destination ID (EX2, Lane 1)
	regValRnA4,		//Destination Value (EX2, Lane 1)
	regIdRnB4,		//Destination ID (EX2, Lane 2)
	regValRnB4,		//Destination Value (EX2, Lane 2)
	regIdRnC4,		//Destination ID (EX2, Lane 3)
	regValRnC4,		//Destination Value (EX2, Lane 3)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValTbr,		//TBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValImmC,		//Immediate (Decode, Lane 3)
	regValLr,		//LR Value (CR)
	regValSsp,		//SSP Value (CR)
	regValCm,		//Cm Value (CR)
	regValBPc,		//Base PC Value

	gprValImmA4,	//Immediate (WB, A)
	gprValImmB4,	//Immediate (WB, B)
	gprValImmC4,	//Immediate (WB, C)
	
	gprEx1Flush,
	gprEx2Flush,
	gprEx3Flush,

	gprEx1DualLane,
	gprEx2DualLane,
	gprEx3DualLane,
	
	gprExHeldRegs,
	regInSr,	regInFpsr,
	regOutDoHold,

	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp
	);


input			clock;
input			reset;
input			hold;

input[8:0]		regIdUCmd;
input[8:0]		regIdUIxt;

`input_gpr		regIdRs;		//Source A
`input_gpr		regIdRt;		//Source B
`input_gpr		regIdRu;		//Source C
`input_gpr		regIdRv;		//Source D
`input_gpr		regIdRx;		//Source E
`input_gpr		regIdRy;		//Source F

`output_gprval	regValRs;
`output_gprval	regValRt;
`output_gprval	regValRu;
`output_gprval	regValRv;
`output_gprval	regValRx;
`output_gprval	regValRy;

`input_gpr		regIdRnA1;		//Destination ID
`input_gprval	regValRnA1;		//Destination Value
`input_gpr		regIdRnA2;		//Destination ID
`input_gprval	regValRnA2;		//Destination Value
`input_gpr		regIdRnA3;		//Destination ID
`input_gprval	regValRnA3;		//Destination Value
`input_gpr		regIdRnA4;		//Destination ID
`input_gprval	regValRnA4;		//Destination Value

`input_gpr		regIdRnB1;		//Destination ID
`input_gprval	regValRnB1;		//Destination Value
`input_gpr		regIdRnB2;		//Destination ID
`input_gprval	regValRnB2;		//Destination Value
`input_gpr		regIdRnB3;		//Destination ID
`input_gprval	regValRnB3;		//Destination Value
`input_gpr		regIdRnB4;		//Destination ID
`input_gprval	regValRnB4;		//Destination Value

`input_gpr		regIdRnC1;		//Destination ID
`input_gprval	regValRnC1;		//Destination Value
`input_gpr		regIdRnC2;		//Destination ID
`input_gprval	regValRnC2;		//Destination Value
`input_gpr		regIdRnC3;		//Destination ID
`input_gprval	regValRnC3;		//Destination Value
`input_gpr		regIdRnC4;		//Destination ID
`input_gprval	regValRnC4;		//Destination Value

input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValBPc;		//Base PC Value
// input [47:0]	regValGbr;		//GBR Value (CR)
input [63:0]	regValGbr;		//GBR Value (CR)
input [47:0]	regValTbr;		//TBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [32:0]	regValImmC;		//Immediate (Decode)
input [63:0]	regValLr;		//LR Value (CR)
input [63:0]	regValSsp;		//SSP Value (CR)
`input_gprval	regValCm;		//Cm Value (CR)

input [32:0]	gprValImmA4;	//Immediate (WB, A)
input [32:0]	gprValImmB4;	//Immediate (WB, B)
input [32:0]	gprValImmC4;	//Immediate (WB, C)

input			gprEx1Flush;
input			gprEx2Flush;
input			gprEx3Flush;

input			gprEx1DualLane;
input			gprEx2DualLane;
input			gprEx3DualLane;

input[8:0]		gprExHeldRegs;

input[31:0]		regInSr;
input[15:0]		regInFpsr;

output			regOutDoHold;

(* max_fanout = 200 *)
	wire		holdN = !hold;

wire			gprId2DualLane;
assign		gprId2DualLane = (regIdUIxt[7:6] == 2'b11);

parameter		noLane3 = 0;


output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

reg				tOutDoHold;
assign		regOutDoHold = tOutDoHold;

`reg_gprval	tRegValRs;
`reg_gprval	tRegValRt;
`reg_gprval	tRegValRu;
`reg_gprval	tRegValRv;
`reg_gprval	tRegValRx;
`reg_gprval	tRegValRy;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRu = tRegValRu;
assign	regValRv = tRegValRv;
assign	regValRx = tRegValRx;
assign	regValRy = tRegValRy;

wire		nMaskEnableRsCm =
//	(regIdUCmd[5:0] == JX2_UCMD_NOP) ||
	(regIdUCmd[5:0] == JX2_UCMD_MOV_CR) ;

//	(regIdUCmd[5:0] == JX2_UCMD_MOV_RM) ||
//	(regIdUCmd[5:0] == JX2_UCMD_MOV_CR) ||
//	(regIdUCmd[5:0] == JX2_UCMD_JMP) ||
//	(regIdUCmd[5:0] == JX2_UCMD_JSR);

//wire		nMaskEnableRyCm =
//	(regIdUCmd[5:0] == JX2_UCMD_NOP) ||
//	(regIdUCmd[5:0] == JX2_UCMD_MOV_RM) ;

// wire		nMaskEnableRsCm = 1;
wire		nMaskEnableRyCm = 1;


(* max_fanout = 200 *)
	wire		nRegHeldRnA1 = !gprExHeldRegs[0];
(* max_fanout = 200 *)
	wire		nRegHeldRnA2 = !gprExHeldRegs[3];
(* max_fanout = 200 *)
	wire		nRegHeldRnA3 = !gprExHeldRegs[6];

(* max_fanout = 200 *)
	wire		nRegHeldRnB1 = !gprExHeldRegs[1];
(* max_fanout = 200 *)
	wire		nRegHeldRnB2 = !gprExHeldRegs[4];
(* max_fanout = 200 *)
	wire		nRegHeldRnB3 = !gprExHeldRegs[7];

(* max_fanout = 200 *)
	wire		nRegHeldRnC1 = !gprExHeldRegs[2];
(* max_fanout = 200 *)
	wire		nRegHeldRnC2 = !gprExHeldRegs[5];
(* max_fanout = 200 *)
	wire		nRegHeldRnC3 = !gprExHeldRegs[8];


(* max_fanout = 200 *)
	`wire_gpr		regIdRnAW;		//Destination ID
(* max_fanout = 200 *)
	`wire_gprval		regValRnAW;		//Destination Value
(* max_fanout = 200 *)
	`wire_gpr		regIdRnBW;		//Destination ID
(* max_fanout = 200 *)
	`wire_gprval		regValRnBW;		//Destination Value
(* max_fanout = 200 *)
	`wire_gpr		regIdRnCW;		//Destination ID
(* max_fanout = 200 *)
	`wire_gprval		regValRnCW;		//Destination Value
(* max_fanout = 200 *)
	wire			regFlushRnW;	//Flush Stage

// assign	regFlushRnW	= 1'b0;

`ifndef def_true
assign	regIdRnAW	= regIdRnA3;
assign	regValRnAW	= regValRnA3;
assign	regIdRnBW	= regIdRnB3;
assign	regValRnBW	= regValRnB3;
assign	regIdRnCW	= noLane3 ? JX2_GR_ZZR : regIdRnC3;
assign	regValRnCW	= noLane3 ? 0 : regValRnC3;

assign	regFlushRnW	= gprEx3Flush;
`endif

`ifdef def_true
assign	regIdRnAW	= regIdRnA4;
assign	regValRnAW	= regValRnA4;
assign	regIdRnBW	= regIdRnB4;
assign	regValRnBW	= regValRnB4;
assign	regIdRnCW	= noLane3 ? JX2_GR_ZZR : regIdRnC4;
assign	regValRnCW	= noLane3 ? 0 : regValRnC4;
assign	regFlushRnW	= gprEx3Flush;
`endif


`wire_gpr		regIdRnA1B;
`wire_gpr		regIdRnA2B;
`wire_gpr		regIdRnB1B;
`wire_gpr		regIdRnB2B;
`wire_gpr		regIdRnC1B;
`wire_gpr		regIdRnC2B;

// assign		regIdRnA1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnA1;
// assign		regIdRnB1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnB1;
// assign		regIdRnC1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnC1;
// assign		regIdRnA2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnA2;
// assign		regIdRnB2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnB2;
// assign		regIdRnC2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnC2;

assign		regIdRnA1B = regIdRnA1;
assign		regIdRnB1B = regIdRnB1;
assign		regIdRnC1B = noLane3 ? JX2_GR_ZZR : regIdRnC1;
assign		regIdRnA2B = regIdRnA2;
assign		regIdRnB2B = regIdRnB2;
assign		regIdRnC2B = noLane3 ? JX2_GR_ZZR : regIdRnC2;

`wire_gpr		regIdRnA3B;		//Destination ID
`wire_gpr		regIdRnB3B;		//Destination ID
`wire_gpr		regIdRnC3B;		//Destination ID
// assign		regIdRnA3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnA3;
// assign		regIdRnB3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnB3;
// assign		regIdRnC3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnC3;
assign		regIdRnA3B = regIdRnA3;
assign		regIdRnB3B = regIdRnB3;
assign		regIdRnC3B = noLane3 ? JX2_GR_ZZR : regIdRnC3;

// `ifdef jx2_enable_gpr48
`ifdef jx2_enable_xgpr

// `ifdef jx2_enable_gpr_rbsel
`ifndef def_true
`reg_gprval	gprArrA[255:0];
`reg_gprval	gprArrB[255:0];
`reg_gprval	gprArrC[255:0];
reg[255:0]	gprArrMA;
reg[255:0]	gprArrMB;
`else
`reg_gprval	gprArrA[63:0];
`reg_gprval	gprArrB[63:0];
`reg_gprval	gprArrC[63:0];
reg[63:0]	gprArrMA;
reg[63:0]	gprArrMB;
`endif

`ifdef jx2_enable_gpr_rbsel
reg[63:0]	gprArrMC;
reg[63:0]	gprArrMD;

`reg_gprval	gprArrRbsBank[255:0];
`endif

`else
`reg_gprval	gprArrA[31:0];
`reg_gprval	gprArrB[31:0];
`reg_gprval	gprArrC[31:0];
reg[31:0]	gprArrMA;
reg[31:0]	gprArrMB;
`endif

// `reg_gprval	gprRegDlr;
// `reg_gprval	gprRegDhr;
// `reg_gprval	gprRegElr;
// `reg_gprval	gprRegEhr;
// `reg_gprval	gprRegSp;
// `reg_gprval	gprRegBp;

`ifdef def_true
(* max_fanout = 200 *)
	`wire_gprval	gprRegDlr;
(* max_fanout = 200 *)
	`wire_gprval	gprRegDhr;
(* max_fanout = 200 *)
	`wire_gprval	gprRegSp;
(* max_fanout = 200 *)
	`wire_gprval	gprRegSp0;
(* max_fanout = 200 *)
	`wire_gprval	gprRegSpZ;

`ifdef jx2_fpu_fpsr_sp
assign	gprRegSp = { regInFpsr, gprRegSp0[47:0] };
assign	gprRegSpZ = { 16'h00, gprRegSp0[47:0] };
`else
assign	gprRegSp = gprRegSp0;
assign	gprRegSpZ = gprRegSp0;
`endif

RegSpr_3W	gprModDlr(
	clock,		// reset,
	JX2_GR_DLR,	gprRegDlr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
//	regInDlr,	hold,
	gprRegDlr,	hold,
	regFlushRnW);

RegSpr_3W	gprModDhr(
	clock,		// reset,
	JX2_GR_DHR,	gprRegDhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
//	regIdRnCW,	regValRnCW,
	JX2_GR_ZZR, UVGPRV_XX,
//	regInDhr,	hold,
	gprRegDhr,	hold,
	regFlushRnW);

`wire_gprval regInSpB;

`ifdef jx2_enable_memcap
assign	regInSpB = { 2'b00, regInSp };
`else
assign	regInSpB = regInSp;
`endif

RegSpr_3W	gprModSp(
	clock,		// reset,
	JX2_GR_SP,	gprRegSp0,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
//	regIdRnCW,	regValRnCW,
	JX2_GR_ZZR, UVGPRV_XX,
	regInSpB,	hold,
	regFlushRnW);

`endif	


assign	regOutDlr = gprRegDlr[63:0];
assign	regOutDhr = gprRegDhr[63:0];
assign	regOutSp  = gprRegSp[63:0];


`reg_gprval	tValRsA0;
`reg_gprval	tValRtA0;
`reg_gprval	tValRuA0;
`reg_gprval	tValRvA0;
`reg_gprval	tValRxA0;
`reg_gprval	tValRyA0;

`reg_gprval	tValRsA;
`reg_gprval	tValRtA;
`reg_gprval	tValRuA;
`reg_gprval	tValRvA;
`reg_gprval	tValRxA;
`reg_gprval	tValRyA;

`reg_gprval	tValJimm;
`reg_gprval	tValJimm56;
`reg_gprval	tValJimm56Vf;
`reg_gprval	tValJimm56F;

`reg_gprval	tValFpImm16A;
`reg_gprval	tValFpImm10A;

`reg_gprval	tValFpImm16B;
`reg_gprval	tValFpImm10B;

`reg_gprval	tValImmRpA;
`reg_gprval	tValImmRpB;

reg		tValRsZz;
reg		tValRtZz;
reg		tValRuZz;
reg		tValRvZz;
reg		tValRxZz;
reg		tValRyZz;

reg		tValRsPair;
reg		tValRtPair;
reg		tValRxPair;
reg		tValRn3Pair;

reg		tRegEx1NoForward;
reg		tRegEx2NoForward;
reg		tRegEx3NoForward;

wire	tEnablePcLsb;
`ifdef jx2_enable_ops24
assign	tEnablePcLsb = (regIdUIxt[2:0] == JX2_BTY_SB);
`else
assign	tEnablePcLsb = 0;
`endif

wire[1:0]	regIdRbSel;
assign		regIdRbSel = regInSr[19:18];

reg[3:0]	tRegTagRs;
reg[3:0]	tRegTagRt;
reg[3:0]	tRegTagRu;
reg[3:0]	tRegTagRv;
reg[3:0]	tRegTagRx;
reg[3:0]	tRegTagRy;

`reg_gprval	tRsbValReg;
`reg_gpr	tRsbIdReg;
reg[1:0]	tRsbSelReg;
reg[1:0]	tRsbSelRegLd;

`reg_gprval	tValRbs;
`reg_gpr	tIdRbs;
reg			tRbsDoSwap;
reg			tRbsDidSwap;
reg			tRbsDoStore;

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRuZz=0;
	tValRvZz=0;
	tValRxZz=0;
	tValRyZz=0;

	tValRsPair = 0;
	tValRtPair = 0;
	tValRxPair = 0;
	tValRn3Pair = 0;

	tRegEx1NoForward = 0;
	tRegEx2NoForward = 0;
	tRegEx3NoForward = 0;

	tOutDoHold		= 0;
	tRbsDoSwap		= 0;
	tRbsDoStore		= 0;
	tRsbSelRegLd	= regIdRbSel;

// `ifdef jx2_enable_gpr48
`ifndef def_true
//	if(regIdUIxt[7:6]==2'b11)
	if(gprId2DualLane)
	begin
		tValRsPair = (regIdRs[5:1] == regIdRu[5:1]) &&
			(regIdRs[0] != regIdRu[0]);
		tValRtPair = (regIdRt[5:1] == regIdRv[5:1]) &&
			(regIdRs[0] != regIdRu[0]);
		tValRxPair = (regIdRx[5:1] == regIdRy[5:1]) &&
			(regIdRs[0] != regIdRu[0]);
			
		if((regIdRnA1[5]) && !gprEx1DualLane)
			tRegEx1NoForward = 1;
		if((regIdRnA2[5]) && !gprEx2DualLane)
			tRegEx2NoForward = 1;
		if((regIdRnA3[5]) && !gprEx3DualLane)
			tRegEx3NoForward = 1;
	end
	else
	begin
		if((regIdRnA1[5]) && gprEx1DualLane)
			tRegEx1NoForward = 1;
		if((regIdRnA2[5]) && gprEx2DualLane)
			tRegEx2NoForward = 1;
		if((regIdRnA3[5]) && gprEx3DualLane)
			tRegEx3NoForward = 1;
	end
	
//	if(1'b1)
	if(gprEx3DualLane)
	begin
		tValRn3Pair = (regIdRnA3[5:1] == regIdRnB3[5:1]) &&
			(regIdRnA3[0] != regIdRnB3[0]);
	end
`endif

//	if(gprEx1Flush)
//		tRegEx1NoForward = 1;
//	if(gprEx2Flush)
//		tRegEx2NoForward = 1;

//	if(gprEx3Flush)
//		tRegEx3NoForward = 1;


`ifdef jx2_enable_wexjumbo
	tValJimm = 0;
	tValJimm[63:0]={
		regValImmB[31:0],
		regValImmA[31:0] };
`endif

`ifdef jx2_use_fpu_fpimm
	tValFpImm16A	= 0;
	tValFpImm10A	= 0;
	tValFpImm16B	= 0;
	tValJimm56Vf	= 0;
	tValJimm56F		= 0;

	tValFpImm16A[63:0]	= {
		regValImmA[15:14],
		(regValImmA[14] || (regValImmA[14:10]==0)) &&
				(regValImmA[14:10]!=5'h1F) ?
			6'h00 : 6'h3F,
		regValImmA[13: 0],
		10'h0,
		32'h0
		};
//	tValFpImm10A[63:0]	= {
//		regValImmA[9:8],
//		(regValImmA[8] || (regValImmA[8:4]==0)) ?
//			6'h00 : 6'h3F,
//		regValImmA[3: 0],
//		16'h0,
//		32'h0
//		};

	tValFpImm10A = 0;

	tValFpImm16B[63:0]	= {
		regValImmB[15:14],
		(regValImmB[14] || (regValImmB[14:10]==0)) &&
				(regValImmB[14:10]!=5'h1F) ?
			6'h00 : 6'h3F,
		regValImmB[13: 0],
		10'h0,
		32'h0
		};
	
	tValJimm56Vf[63:0] = {
		tValJimm[55:42], 2'b0,
		tValJimm[41:28], 2'b0,
		tValJimm[27:14], 2'b0,
		tValJimm[13: 0], 2'b0
	};

	tValJimm56F[63:0] = {
		tValJimm[55: 0],
		tValJimm[62:56],
//		tValJimm[62:56] ^ (tValJimm[63] ? 7'h7F : 7'h00),
//		1'b0
		tValJimm[63]
	};
`endif
	
`ifdef jx2_alu_jcmp_rpimm
	tValImmRpA = 0;
	tValImmRpB = 0;
	tValImmRpA[63:0] = { regIdRy[5] ? UV59_FF, UV59_00, regIdRy[4:0] };
	tValImmRpB[63:0] = { regIdRx[5] ? UV59_FF, UV59_00, regIdRx[4:0] };
`endif

`ifdef jx2_enable_xgpr
// `ifdef jx2_enable_gpr_rbsel
`ifndef def_true
	tValRsA0=gprArrMB[{regIdRbSel, regIdRs[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRs[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRs[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRs[5:0]}] :
			gprArrA[{regIdRbSel, regIdRs[5:0]}]);
	tValRtA0=gprArrMB[{regIdRbSel, regIdRt[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRt[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRt[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRt[5:0]}] :
			gprArrA[{regIdRbSel, regIdRt[5:0]}]);
	tValRuA0=gprArrMB[{regIdRbSel, regIdRu[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRu[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRu[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRu[5:0]}] :
			gprArrA[{regIdRbSel, regIdRu[5:0]}]);
	tValRvA0=gprArrMB[{regIdRbSel, regIdRv[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRv[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRv[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRv[5:0]}] :
			gprArrA[{regIdRbSel, regIdRv[5:0]}]);
	tValRxA0=gprArrMB[{regIdRbSel, regIdRx[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRx[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRx[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRx[5:0]}] :
			gprArrA[{regIdRbSel, regIdRx[5:0]}]);
	tValRyA0=gprArrMB[{regIdRbSel, regIdRy[5:0]}] ?
		gprArrC[{regIdRbSel, regIdRy[5:0]}] :
		(	gprArrMA[{regIdRbSel, regIdRy[5:0]}] ?
			gprArrB[{regIdRbSel, regIdRy[5:0]}] :
			gprArrA[{regIdRbSel, regIdRy[5:0]}]);
`else
	tValRsA0=gprArrMB[regIdRs[5:0]] ?
		gprArrC[regIdRs[5:0]] :
		(	gprArrMA[regIdRs[5:0]] ?
			gprArrB[regIdRs[5:0]] :
			gprArrA[regIdRs[5:0]]);
	tValRtA0=gprArrMB[regIdRt[5:0]] ?
		gprArrC[regIdRt[5:0]] :
		(	gprArrMA[regIdRt[5:0]] ?
			gprArrB[regIdRt[5:0]] :
			gprArrA[regIdRt[5:0]]);
	tValRuA0=gprArrMB[regIdRu[5:0]] ?
		gprArrC[regIdRu[5:0]] :
		(	gprArrMA[regIdRu[5:0]] ?
			gprArrB[regIdRu[5:0]] :
			gprArrA[regIdRu[5:0]]);
	tValRvA0=gprArrMB[regIdRv[5:0]] ?
		gprArrC[regIdRv[5:0]] :
		(	gprArrMA[regIdRv[5:0]] ?
			gprArrB[regIdRv[5:0]] :
			gprArrA[regIdRv[5:0]]);
	tValRxA0=gprArrMB[regIdRx[5:0]] ?
		gprArrC[regIdRx[5:0]] :
		(	gprArrMA[regIdRx[5:0]] ?
			gprArrB[regIdRx[5:0]] :
			gprArrA[regIdRx[5:0]]);
	tValRyA0=gprArrMB[regIdRy[5:0]] ?
		gprArrC[regIdRy[5:0]] :
		(	gprArrMA[regIdRy[5:0]] ?
			gprArrB[regIdRy[5:0]] :
			gprArrA[regIdRy[5:0]]);
`endif
`else
	tValRsA0=gprArrMB[regIdRs[4:0]] ?
		gprArrC[regIdRs[4:0]] :
		(	gprArrMA[regIdRs[4:0]] ?
			gprArrB[regIdRs[4:0]] :
			gprArrA[regIdRs[4:0]]);
	tValRtA0=gprArrMB[regIdRt[4:0]] ?
		gprArrC[regIdRt[4:0]] :
		(	gprArrMA[regIdRt[4:0]] ?
			gprArrB[regIdRt[4:0]] :
			gprArrA[regIdRt[4:0]]);
	tValRuA0=gprArrMB[regIdRu[4:0]] ?
		gprArrC[regIdRu[4:0]] :
		(	gprArrMA[regIdRu[4:0]] ?
			gprArrB[regIdRu[4:0]] :
			gprArrA[regIdRu[4:0]]);
	tValRvA0=gprArrMB[regIdRv[4:0]] ?
		gprArrC[regIdRv[4:0]] :
		(	gprArrMA[regIdRv[4:0]] ?
			gprArrB[regIdRv[4:0]] :
			gprArrA[regIdRv[4:0]]);
	tValRxA0=gprArrMB[regIdRx[4:0]] ?
		gprArrC[regIdRx[4:0]] :
		(	gprArrMA[regIdRx[4:0]] ?
			gprArrB[regIdRx[4:0]] :
			gprArrA[regIdRx[4:0]]);
	tValRyA0=gprArrMB[regIdRy[4:0]] ?
		gprArrC[regIdRy[4:0]] :
		(	gprArrMA[regIdRy[4:0]] ?
			gprArrB[regIdRy[4:0]] :
			gprArrA[regIdRy[4:0]]);
`endif

`ifdef jx2_enable_gpr_rbsel
	tRegTagRs = {
		gprArrMD[regIdRs[5:0]],
		gprArrMC[regIdRs[5:0]],
		gprArrMB[regIdRs[5:0]],
		gprArrMA[regIdRs[5:0]] };
	tRegTagRt = {
		gprArrMD[regIdRt[5:0]],
		gprArrMC[regIdRt[5:0]],
		gprArrMB[regIdRt[5:0]],
		gprArrMA[regIdRt[5:0]] };
	tRegTagRu = {
		gprArrMD[regIdRu[5:0]],
		gprArrMC[regIdRu[5:0]],
		gprArrMB[regIdRu[5:0]],
		gprArrMA[regIdRu[5:0]] };
	tRegTagRv = {
		gprArrMD[regIdRv[5:0]],
		gprArrMC[regIdRv[5:0]],
		gprArrMB[regIdRv[5:0]],
		gprArrMA[regIdRv[5:0]] };
	tRegTagRx = {
		gprArrMD[regIdRx[5:0]],
		gprArrMC[regIdRx[5:0]],
		gprArrMB[regIdRx[5:0]],
		gprArrMA[regIdRx[5:0]] };
	tRegTagRy = {
		gprArrMD[regIdRy[5:0]],
		gprArrMC[regIdRy[5:0]],
		gprArrMB[regIdRy[5:0]],
		gprArrMA[regIdRy[5:0]] };
	
	tRsbValReg	= UVGPRV_00;
	tRsbIdReg	= JX2_GR_ZZR;

//	if(regIdRbSel != tRegTagRs[3:2])
	if((regIdRbSel != tRegTagRs[3:2]) && !regIdRs[6])
	begin
		tRsbValReg	= tValRsA0;
		tRsbIdReg	= regIdRs;
		tRsbSelReg	= tRegTagRs[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end

//	if(regIdRbSel != tRegTagRt[3:2])
	if((regIdRbSel != tRegTagRt[3:2]) && !regIdRt[6])
	begin
		tRsbValReg	= tValRtA0;
		tRsbIdReg	= regIdRt;
		tRsbSelReg	= tRegTagRt[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end

//	if(regIdRbSel != tRegTagRu[3:2])
	if((regIdRbSel != tRegTagRu[3:2]) && !regIdRu[6])
	begin
		tRsbValReg	= tValRuA0;
		tRsbIdReg	= regIdRu;
		tRsbSelReg	= tRegTagRu[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end
//	if(regIdRbSel != tRegTagRv[3:2])
	if((regIdRbSel != tRegTagRv[3:2]) && !regIdRv[6])
	begin
		tRsbValReg	= tValRvA0;
		tRsbIdReg	= regIdRv;
		tRsbSelReg	= tRegTagRv[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end

//	if(regIdRbSel != tRegTagRx[3:2])
	if((regIdRbSel != tRegTagRx[3:2]) && !regIdRx[6])
	begin
		tRsbValReg	= tValRxA0;
		tRsbIdReg	= regIdRx;
		tRsbSelReg	= tRegTagRx[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end

//	if(regIdRbSel != tRegTagRy[3:2])
	if((regIdRbSel != tRegTagRy[3:2]) && !regIdRy[6])
	begin
		tRsbValReg	= tValRyA0;
		tRsbIdReg	= regIdRy;
		tRsbSelReg	= tRegTagRy[3:2];
		tOutDoHold	= 1;
		tRbsDoSwap	= 1;
	end
	
	if(regIdRs == JX2_GR_RBSEL)
	begin
		tRsbIdReg		= { 1'b0, regValImmA[5:0] };
		tRsbSelRegLd	=  regValImmA[7:6];
//		tRsbSelReg	= tRegTagRy[3:2];

		if(tIdRbs[5:0] != regValImmA[5:0])
			tOutDoHold	= 1;
	end
	
	if(regIdRnAW == JX2_GR_RBSEL)
	begin
		tRsbValReg	= regValRnAW;
		tRsbIdReg	= { 1'b0, gprValImmA4[5:0] };
		tRsbSelReg	=  gprValImmA4[7:6];
		tRbsDoStore	= 1;

		if(tIdRbs[5:0] != gprValImmA4[5:0])
			tOutDoHold	= 1;
	end
	
//	if(tRsbIdReg != JX2_GR_ZZR)
//	begin
//		tOutDoHold		= 1;
//	end
`endif


	casez(regIdRs)
//		JX2_GR_GPR_Z:	tValRsA=gprArr[regIdRs[4:0]];
		JX2_GR_GPR_Z:	tValRsA=tValRsA0;
		JX2_GR_DLR:	tValRsA=gprRegDlr;
//		JX2_GR_DLR:	tValRsA=regInDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
//		JX2_GR_DHR:	tValRsA=regInDhr;
		JX2_GR_SP:	tValRsA=gprRegSp;
//		JX2_GR_SP:	tValRsA=regInSp;
		JX2_GR_ZSP:	tValRsA=gprRegSpZ;

		JX2_GR_SSP:
		begin
			tValRsA=0;
			tValRsA[63:0]=regValSsp;
		end

`ifdef jx2_enable_gpr_rbsel
		JX2_GR_RBSEL:
			tValRsA = tValRbs;
`endif

`ifdef jx2_enable_vaddr48
//		JX2_GR_PC:	tValRsA={ UV16_00, regValPc };
		JX2_GR_PC:
		begin
			tValRsA=0;
			tValRsA[63:0] = { UV16_00, regValPc[47:1],
				regValPc[0] &tEnablePcLsb };
			tValRsZz=1;
		end
		JX2_GR_GBR:
		begin
			tValRsA=0;
//			tValRsA[63:0] = { UV16_00, regValGbr };
			tValRsA[63:0] = regValGbr[63:0];
		end

		JX2_GR_TBR:
		begin
			tValRsA=0;
			tValRsA[63:0] = { UV16_00, regValTbr };
		end

//		JX2_GR_LR:	tValRsA = { UV16_00, regValLr };
		JX2_GR_LR:
		begin
			tValRsA=0;
			tValRsA[63:0] = regValLr;
		end

`ifdef jx2_enable_riscv
		JX2_GR_BPC:
		begin
//			$display("RegGPR: Debug BPC=%X PC=%X",
//				regValBPc, regValPc);
		
			tValRsA=0;
			tValRsA[63:0] = { UV16_00, regValBPc };
			tValRsZz=1;
		end
`endif

`else
//		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:0] };
		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:1],
			regValPc[0] &tEnablePcLsb };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr[31:0] };
		JX2_GR_TBR:	tValRsA={ UV32_00, regValTbr[31:0] };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr[31:0] };

`ifdef jx2_enable_riscv
		JX2_GR_BPC:	tValRsA={ UV32_00, regValBPc[31:0] };
`endif

`endif

`ifdef jx2_gprs_mergecm
		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, // JX2_GR_SSP,
		JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
		begin
//			tValRsA = regValCm;
//			tValRsA = 0;
			tValRsA = nMaskEnableRsCm ? regValCm : UVGPRV_XX;
			if(!nMaskEnableRsCm && (regIdUCmd[5:0] != JX2_UCMD_NOP))
				$display("GPR: Access Masked CRn=%X %X-%X",
					regIdRs, regIdUCmd, regIdUIxt);
		end

`ifdef jx2_enable_xgpr
		JX2_GR_TEAH, JX2_GR_VIPT: 
		begin
//			tValRsA = regValCm;

//			tValRsA = 0;
			tValRsA = nMaskEnableRsCm ? regValCm : UVGPRV_XX;
			if(!nMaskEnableRsCm && (regIdUCmd[5:0] != JX2_UCMD_NOP))
				$display("GPR: Access Masked CRn=%X %X-%X",
					regIdRs, regIdUCmd, regIdUIxt);
		end
`endif
`endif


		JX2_GR_IMM: begin
			tValRsA = 0;
			tValRsA[63:0] = {
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRsZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRsA=UVGPRV_00;
			tValRsZz=1;
		end

`ifdef jx2_enable_immb
		JX2_GR_IMMB: begin
			tValRsA = 0;
			tValRsA[63:0] = {
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRsZz=1;
		end
`endif

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
//			tValRsA = 0;
			tValRsA=tValJimm;
			tValRsZz=1;
		end

//		JX2_GR_JIMM56: begin
//			tValRsA=tValJimm56;
//			tValRsZz=1;
//		end
`endif

`ifdef jx2_use_fpu_fpimm
//		JX2_GR_FPIMM10: begin
//			tValRsA=tValFpImm10A;
//			tValRsZz=1;
//		end

		JX2_GR_FPIMM16: begin
			tValRsA=tValFpImm16A;
			tValRsZz=1;
		end

//		JX2_GR_FPIMM56VF: begin
//			tValRsA=tValJimm56Vf;
//			tValRsZz=1;
//		end
`endif

`ifdef jx2_alu_jcmp_rpimm
		JX2_GR_IMM_RP: begin
			tValRsA=tValImmRpA;
			tValRsZz=1;
		end
`endif

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1, JX2_GR_R4IMM2: begin
			tValRsA = 0;
			tValRsA[63:0] = {
				regValImmA[32]?UV36_FF:UV36_00,
				regValImmA[31:4]
				};
			tValRsZz=1;
		end
`endif

`ifdef jx2_enable_ldirot
// `ifndef def_true
		JX2_GR_R8IMMH: begin
			tValRsA = 0;
			tValRsA[63:0] = {
				regValImmA[32]?UV40_FF:UV40_00,
				regValImmA[31:8]
				};
			tValRsZz=1;
		end
`endif

		default: 	tValRsA=UVGPRV_00;
	endcase
	
	casez(regIdRt)
		JX2_GR_GPR_Z:	tValRtA=tValRtA0;
		JX2_GR_DLR:	tValRtA=gprRegDlr;
//		JX2_GR_DLR:	tValRtA=regInDlr;
		JX2_GR_DHR:	tValRtA=gprRegDhr;
//		JX2_GR_DHR:	tValRtA=regInDhr;
		JX2_GR_SP:	tValRtA=gprRegSp;
//		JX2_GR_SP:	tValRtA=regInSp;
//		JX2_GR_ZSP:	tValRtA=gprRegSpZ;

//		JX2_GR_SSP:	tValRtA=regValSsp;

		JX2_GR_IMM:	begin
			tValRtA = UVGPRV_00;
			tValRtA[63:0] = {
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRtZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRtA=UVGPRV_00;
			tValRtZz=1;
		end

`ifdef jx2_enable_immb
		JX2_GR_IMMB: begin
			tValRtA = 0;
			tValRtA[63:0] = {
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRtZz=1;
		end
`endif
		
		JX2_GR_IMM_HI: begin
			tValRtA[63:0] = {
				regValImmA[31:0],
				regValImmA[32]?UV32_FF:UV32_00 };
			tValRtZz=1;
		end
		
`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRtA=tValJimm;
			tValRtZz=1;
		end
//		JX2_GR_JIMM56: begin
//			tValRtA=tValJimm56;
//			tValRtZz=1;
//		end
`endif

`ifdef jx2_use_fpu_fpimm
//		JX2_GR_FPIMM10: begin
//			tValRtA=tValFpImm10A;
//			tValRtZz=1;
//		end

		JX2_GR_FPIMM16: begin
			tValRtA=tValFpImm16A;
			tValRtZz=1;
		end

		JX2_GR_FPIMM56VF: begin
			tValRtA=tValJimm56Vf;
			tValRtZz=1;
		end
		
		JX2_GR_FPIMM56F: begin
			tValRtA=tValJimm56F;
			tValRtZz=1;
		end
`endif

`ifdef jx2_alu_jcmp_rpimm
		JX2_GR_IMM_RP: begin
			tValRtA=tValImmRpA;
			tValRtZz=1;
		end
`endif

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1: begin
			tValRtA={ UV59_00, regValImmA[3:0], 1'b0 };
			tValRtZz=1;
		end
		JX2_GR_R4IMM2: begin
			tValRtA={ UV58_00, regValImmA[3:0], 2'b0 };
			tValRtZz=1;
		end
`endif

`ifdef jx2_enable_ldirot
// `ifndef def_true
		JX2_GR_R8IMML: begin
			tValRtA={ UV56_00, regValImmA[7:0] };
			tValRtZz=1;
		end
`endif


		default: 	tValRtA=UVGPRV_00;
	endcase

	casez(regIdRu)
		JX2_GR_GPR_Z:	tValRuA=tValRuA0;
		JX2_GR_DLR:	tValRuA=gprRegDlr;
//		JX2_GR_DLR:	tValRuA=regInDlr;
		JX2_GR_DHR:	tValRuA=gprRegDhr;
//		JX2_GR_DHR:	tValRuA=regInDhr;
		JX2_GR_SP:	tValRuA=gprRegSp;
//		JX2_GR_SP:	tValRuA=regInSp;
		JX2_GR_ZSP:	tValRuA=gprRegSpZ;

		JX2_GR_SSP:
		begin
			tValRuA = UVGPRV_00;
			tValRuA[63:0] = regValSsp;
		end

		JX2_GR_LR:
		begin
			tValRuA = UVGPRV_00;
			tValRuA[63:0] = regValLr;
		end

		JX2_GR_GBR:
		begin
			tValRuA=0;
//			tValRuA[63:0] = { UV16_00, regValGbr };
			tValRuA[63:0] = regValGbr[63:0];
		end

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRuA=tValJimm;
			tValRuZz=1;
		end
`endif

		JX2_GR_IMM:
		begin
			tValRuA = UVGPRV_00;
			tValRuA[63:0] = {
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRuZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRuA=UVGPRV_00;
			tValRuZz=1;
		end

`ifdef jx2_use_fpu_fpimm
		JX2_GR_FPIMM16: begin
			tValRuA=tValFpImm16B;
			tValRuZz=1;
		end
`endif

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1, JX2_GR_R4IMM2: begin
			tValRuA={
				regValImmB[32]?UV36_FF:UV36_00,
				regValImmB[31:4]
				};
			tValRuZz=1;
		end
`endif

		default: 	tValRuA=UVGPRV_00;
	endcase

	casez(regIdRv)
		JX2_GR_GPR_Z:	tValRvA=tValRvA0;
		JX2_GR_DLR:	tValRvA=gprRegDlr;
//		JX2_GR_DLR:	tValRvA=regInDlr;
		JX2_GR_DHR:	tValRvA=gprRegDhr;
//		JX2_GR_DHR:	tValRvA=regInDhr;
		JX2_GR_SP:	tValRvA=gprRegSp;		//Disabled from this lane
//		JX2_GR_SP:	tValRvA=regInSp;

//		JX2_GR_SSP:	tValRvA=regValSsp;

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRvA=tValJimm;
			tValRvZz=1;
		end
`endif

		JX2_GR_IMM:
		begin
			tValRvA = UVGPRV_00;
			tValRvA[63:0] = {
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRvZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRvA=UVGPRV_00;
			tValRvZz=1;
		end

`ifdef jx2_use_fpu_fpimm
		JX2_GR_FPIMM16: begin
			tValRvA=tValFpImm16B;
			tValRvZz=1;
		end
`endif

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1: begin
			tValRvA={ UV59_00, regValImmB[3:0], 1'b0 };
			tValRvZz=1;
		end
		JX2_GR_R4IMM2: begin
			tValRvA={ UV58_00, regValImmB[3:0], 2'b0 };
			tValRvZz=1;
		end
`endif

		default: 	tValRvA=UVGPRV_00;
	endcase

	casez(regIdRx)
		JX2_GR_GPR_Z:	tValRxA=tValRxA0;
		JX2_GR_DLR:	tValRxA=gprRegDlr;
//		JX2_GR_DLR:	tValRxA=regInDlr;
		JX2_GR_DHR:	tValRxA=gprRegDhr;		//Disabled from this lane
//		JX2_GR_DHR:	tValRxA=regInDhr;
		JX2_GR_SP:	tValRxA=gprRegSp;		//Disabled from this lane
//		JX2_GR_SP:	tValRxA=regInSp;
//		JX2_GR_ZSP:	tValRxA=gprRegSpZ;

//		JX2_GR_SSP:	tValRxA=regValSsp;

`ifndef def_true

`ifdef jx2_enable_vaddr48
		JX2_GR_PC:	tValRxA={ UV16_00, regValPc[47:1],
			regValPc[0] & tEnablePcLsb };
//		JX2_GR_GBR:	tValRxA={ UV16_00, regValGbr };
		JX2_GR_GBR:	tValRxA= regValGbr;
		JX2_GR_TBR:	tValRxA={ UV16_00, regValTbr };
		JX2_GR_LR:	tValRxA= regValLr;
`else
		JX2_GR_PC:	tValRxA={ UV32_00, regValPc[31:1],
			regValPc[0] & tEnablePcLsb };
		JX2_GR_GBR:	tValRxA={ UV32_00, regValGbr[31:0] };
		JX2_GR_TBR:	tValRxA={ UV32_00, regValTbr[31:0] };
		JX2_GR_LR:	tValRxA={ UV32_00, regValLr[31:0] };
`endif

`endif

		JX2_GR_IMM, JX2_GR_IMMB:
		begin
			tValRxA = UVGPRV_00;
			tValRxA[63:0] = {
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRxZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRxA=UVGPRV_00;
			tValRxZz=1;
		end

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1, JX2_GR_R4IMM2: begin
			tValRxA={
				regValImmC[32]?UV36_FF:UV36_00,
				regValImmC[31:4]
				};
			tValRxZz=1;
		end
`endif
		default: 	tValRxA=UVGPRV_00;
	endcase

	casez(regIdRy)
		JX2_GR_GPR_Z:	tValRyA=tValRyA0;
		JX2_GR_DLR:	tValRyA=gprRegDlr;
//		JX2_GR_DLR:	tValRyA=regInDlr;
		JX2_GR_DHR:	tValRyA=gprRegDhr;
//		JX2_GR_DHR:	tValRyA=regInDhr;
		JX2_GR_SP:	tValRyA=gprRegSp;		//Disabled from this lane
//		JX2_GR_SP:	tValRyA=regInSp;

//		JX2_GR_SSP:	tValRyA=regValSsp;


// `ifdef def_true
`ifdef jx2_enable_movc

`ifdef jx2_enable_movclite

`ifdef jx2_enable_vaddr48
//		JX2_GR_PC:	tValRyA={ UV16_00, regValPc[47:1],
//			regValPc[0] & tEnablePcLsb };
//		JX2_GR_GBR:	tValRyA={ UV16_00, regValGbr };
		JX2_GR_GBR:	tValRyA= regValGbr;
//		JX2_GR_TBR:	tValRyA={ UV16_00, regValTbr };
		JX2_GR_LR:	tValRyA= regValLr;
`else
//		JX2_GR_PC:	tValRyA={ UV32_00, regValPc[31:1],
//			regValPc[0] & tEnablePcLsb };
		JX2_GR_GBR:	tValRyA={ UV32_00, regValGbr[31:0] };
//		JX2_GR_TBR:	tValRyA={ UV32_00, regValTbr[31:0] };
		JX2_GR_LR:	tValRyA={ UV32_00, regValLr[31:0] };
`endif

`else

`ifdef jx2_enable_vaddr48
		JX2_GR_PC:
		begin
			tValRyA = UVGPRV_00;
			tValRyA[63:0] = {
				UV16_00, regValPc[47:1],
				regValPc[0] & tEnablePcLsb };
		end
		JX2_GR_GBR:
		begin
			tValRyA = UVGPRV_00;
//			tValRyA[63:0] = { UV16_00, regValGbr };
			tValRyA[63:0] = regValGbr[63:0];
		end
		JX2_GR_TBR:
		begin
			tValRyA = UVGPRV_00;
			tValRyA[63:0] = { UV16_00, regValTbr };
		end
		JX2_GR_LR:
		begin
			tValRyA = UVGPRV_00;
			tValRyA[63:0] = regValLr;
		end
`else
		JX2_GR_PC:	tValRyA={ UV32_00, regValPc[31:1],
			regValPc[0] & tEnablePcLsb };
		JX2_GR_GBR:	tValRyA={ UV32_00, regValGbr[31:0] };
		JX2_GR_TBR:	tValRyA={ UV32_00, regValTbr[31:0] };
		JX2_GR_LR:	tValRyA={ UV32_00, regValLr[31:0] };
`endif

`ifdef jx2_gprs_mergecm
		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, JX2_GR_SSP,
		JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
		begin
//			tValRyA = regValCm;
//			tValRyA = UVGPRV_00;
			tValRyA = nMaskEnableRyCm ? regValCm : UVGPRV_XX;
			if(!nMaskEnableRyCm && (regIdUCmd[5:0] != JX2_UCMD_NOP))
				$display("GPR: Access Ry Masked CRn=%X %X-%X",
					regIdRy, regIdUCmd, regIdUIxt);
		end
`ifdef jx2_enable_xgpr
		JX2_GR_TEAH, JX2_GR_VIPT:
		begin
//			tValRsA = regValCm;
//			tValRyA = UVGPRV_00;
			tValRyA = nMaskEnableRyCm ? regValCm : UVGPRV_XX;
			if(!nMaskEnableRyCm && (regIdUCmd[5:0] != JX2_UCMD_NOP))
				$display("GPR: Access Ry Masked CRn=%X %X-%X",
					regIdRy, regIdUCmd, regIdUIxt);
		end
`endif
`endif

`endif

`endif

		JX2_GR_IMM, JX2_GR_IMMB:
		begin
			tValRyA = UVGPRV_00;
			tValRyA[63:0] = {
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRyZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRyA=UVGPRV_00;
			tValRyZz=1;
		end

// `ifdef jx2_enable_ldirot
`ifndef def_true
		JX2_GR_R4IMM1: begin
			tValRyA={ UV59_00, regValImmC[3:0], 1'b0 };
			tValRyZz=1;
		end
		JX2_GR_R4IMM2: begin
			tValRyA={ UV58_00, regValImmC[3:0], 2'b0 };
			tValRyZz=1;
		end
`endif

		default: 	tValRyA=UVGPRV_00;
	endcase

// `ifdef def_true
`ifndef def_true
//	if(tValRsPair && (regIdRs[5:4]==2'b11))
	begin
		tValRsA=tValRsA0;
		tValRuA=tValRuA0;
	end

//	if(tValRtPair && (regIdRt[5:4]==2'b11))
	begin
		tValRtA=tValRtA0;
		tValRvA=tValRvA0;
	end

//	if(tValRxPair && (regIdRx[5:4]==2'b11))
	begin
		tValRxA=tValRxA0;
		tValRyA=tValRyA0;
	end
`endif

	tRegValRs = tValRsA;
	tRegValRt = tValRtA;
	tRegValRu = tValRuA;
	tRegValRv = tValRvA;
	tRegValRx = tValRxA;
	tRegValRy = tValRyA;


// `ifdef def_true
`ifndef def_true
//	if(!gprEx3Flush)
	if(1'b1)
	begin
		if(regValRnC3[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX3 Bad, Not Flushed");
		end
		if(regValRnB3[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX3 Bad, Not Flushed");
		end
		if(regValRnA3[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX3 Bad, Not Flushed");
		end
	end

//	if(!gprEx2Flush)
	if(1'b1)
	begin
		if(regValRnC2[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX2 Bad, Not Flushed");
		end
		if(regValRnB2[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX2 Bad, Not Flushed");
		end
		if(regValRnA2[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX2 Bad, Not Flushed");
		end
	end

//	if(!gprEx1Flush)
	if(1'b1)
	begin
		if(regValRnC1[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX1 Bad, Not Flushed");
		end
		if(regValRnB1[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX1 Bad, Not Flushed");
		end
		if(regValRnA1[31:0]==32'h55BAADAA)
		begin
			$display("RegCR: EX1 Bad, Not Flushed");
		end
	end
`endif

	if(!tValRsZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRs==regIdRnC3B) && nRegHeldRnC3)
				tRegValRs=regValRnC3;
`endif
			if((regIdRs==regIdRnB3B) && nRegHeldRnB3)
				tRegValRs=regValRnB3;
			if((regIdRs==regIdRnA3B) && nRegHeldRnA3)
				tRegValRs=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRs==regIdRnC2B) && nRegHeldRnC2)
				tRegValRs=regValRnC2;
`endif
			if((regIdRs==regIdRnB2B) && nRegHeldRnB2)
				tRegValRs=regValRnB2;
			if((regIdRs==regIdRnA2B) && nRegHeldRnA2)
				tRegValRs=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRs==regIdRnC1B) && nRegHeldRnC1)
				tRegValRs=regValRnC1;
`endif
			if((regIdRs==regIdRnB1B) && nRegHeldRnB1)
				tRegValRs=regValRnB1;
			if((regIdRs==regIdRnA1B) && nRegHeldRnA1)
				tRegValRs=regValRnA1;
		end
	end

	if(!tValRtZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRt==regIdRnC3B) && nRegHeldRnC3)
				tRegValRt=regValRnC3;
`endif
			if((regIdRt==regIdRnB3B) && nRegHeldRnB3)
				tRegValRt=regValRnB3;
			if((regIdRt==regIdRnA3B) && nRegHeldRnA3)
				tRegValRt=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRt==regIdRnC2B) && nRegHeldRnC2)
				tRegValRt=regValRnC2;
`endif
			if((regIdRt==regIdRnB2B) && nRegHeldRnB2)
				tRegValRt=regValRnB2;
			if((regIdRt==regIdRnA2B) && nRegHeldRnA2)
				tRegValRt=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRt==regIdRnC1B) && nRegHeldRnC1)
				tRegValRt=regValRnC1;
`endif
			if((regIdRt==regIdRnB1B) && nRegHeldRnB1)
				tRegValRt=regValRnB1;
			if((regIdRt==regIdRnA1B) && nRegHeldRnA1)
				tRegValRt=regValRnA1;
		end
	end

	if(!tValRuZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
// `ifndef jx2_cpu_nofw_lane3
			if((regIdRu==regIdRnC3B) && nRegHeldRnC3)
				tRegValRu=regValRnC3;
// `endif
			if((regIdRu==regIdRnB3B) && nRegHeldRnB3)
				tRegValRu=regValRnB3;
			if((regIdRu==regIdRnA3B) && nRegHeldRnA3)
				tRegValRu=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRu==regIdRnC2B) && nRegHeldRnC2)
				tRegValRu=regValRnC2;
`endif
			if((regIdRu==regIdRnB2B) && nRegHeldRnB2)
				tRegValRu=regValRnB2;
			if((regIdRu==regIdRnA2B) && nRegHeldRnA2)
				tRegValRu=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRu==regIdRnC1B) && nRegHeldRnC1)
				tRegValRu=regValRnC1;
`endif
			if((regIdRu==regIdRnB1B) && nRegHeldRnB1)
				tRegValRu=regValRnB1;
			if((regIdRu==regIdRnA1B) && nRegHeldRnA1)
				tRegValRu=regValRnA1;
		end
	end

	if(!tValRvZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRv==regIdRnC3B) && nRegHeldRnC3)
				tRegValRv=regValRnC3;
`endif
			if((regIdRv==regIdRnB3B) && nRegHeldRnB3)
				tRegValRv=regValRnB3;
			if((regIdRv==regIdRnA3B) && nRegHeldRnA3)
				tRegValRv=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRv==regIdRnC2B) && nRegHeldRnC2)
				tRegValRv=regValRnC2;
`endif
			if((regIdRv==regIdRnB2B) && nRegHeldRnB2)
				tRegValRv=regValRnB2;
			if((regIdRv==regIdRnA2B) && nRegHeldRnA2)
				tRegValRv=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRv==regIdRnC1B) && nRegHeldRnC1)
				tRegValRv=regValRnC1;
`endif
			if((regIdRv==regIdRnB1B) && nRegHeldRnB1)
				tRegValRv=regValRnB1;
			if((regIdRv==regIdRnA1B) && nRegHeldRnA1)
				tRegValRv=regValRnA1;
		end
	end


	if(!tValRxZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRx==regIdRnC3B) && nRegHeldRnC3)
				tRegValRx=regValRnC3;
`endif
			if((regIdRx==regIdRnB3B) && nRegHeldRnB3)
				tRegValRx=regValRnB3;
			if((regIdRx==regIdRnA3B) && nRegHeldRnA3)
				tRegValRx=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRx==regIdRnC2B) && nRegHeldRnC2)
				tRegValRx=regValRnC2;
`endif
			if((regIdRx==regIdRnB2B) && nRegHeldRnB2)
				tRegValRx=regValRnB2;
			if((regIdRx==regIdRnA2B) && nRegHeldRnA2)
				tRegValRx=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRx==regIdRnC1B) && nRegHeldRnC1)
				tRegValRx=regValRnC1;
`endif
			if((regIdRx==regIdRnB1B) && nRegHeldRnB1)
				tRegValRx=regValRnB1;
			if((regIdRx==regIdRnA1B) && nRegHeldRnA1)
				tRegValRx=regValRnA1;
		end
	end

	if(!tValRyZz)
	begin
//		if(!gprEx3Flush)
//		if(1'b1)
		if(!tRegEx3NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRy==regIdRnC3B) && nRegHeldRnC3)
				tRegValRy=regValRnC3;
`endif
			if((regIdRy==regIdRnB3B) && nRegHeldRnB3)
				tRegValRy=regValRnB3;
			if((regIdRy==regIdRnA3B) && nRegHeldRnA3)
				tRegValRy=regValRnA3;
		end

//		if(!gprEx2Flush)
//		if(1'b1)
		if(!tRegEx2NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRy==regIdRnC2B) && nRegHeldRnC2)
				tRegValRy=regValRnC2;
`endif
			if((regIdRy==regIdRnB2B) && nRegHeldRnB2)
				tRegValRy=regValRnB2;
			if((regIdRy==regIdRnA2B) && nRegHeldRnA2)
				tRegValRy=regValRnA2;
		end

//		if(!gprEx1Flush)
//		if(1'b1)
		if(!tRegEx1NoForward)
		begin
`ifndef jx2_cpu_nofw_lane3
			if((regIdRy==regIdRnC1B) && nRegHeldRnC1)
				tRegValRy=regValRnC1;
`endif
			if((regIdRy==regIdRnB1B) && nRegHeldRnB1)
				tRegValRy=regValRnB1;
			if((regIdRy==regIdRnA1B) && nRegHeldRnA1)
				tRegValRy=regValRnA1;
		end
	end
end

(* max_fanout = 200 *)
	wire		maskEnaG = !hold && !regFlushRnW;

`ifdef jx2_enable_xgpr
(* max_fanout = 200 *)
	wire		maskEnaA = !hold && !regFlushRnW && !regIdRnAW[6];
(* max_fanout = 200 *)
	wire		maskEnaB = !hold && !regFlushRnW && !regIdRnBW[6];
(* max_fanout = 200 *)
	wire		maskEnaC = !hold && !regFlushRnW && !regIdRnCW[6];
`else
(* max_fanout = 200 *)
	wire		maskEnaA = !hold && !regFlushRnW && !regIdRnAW[5];
(* max_fanout = 200 *)
	wire		maskEnaB = !hold && !regFlushRnW && !regIdRnBW[5];
(* max_fanout = 200 *)
	wire		maskEnaC = !hold && !regFlushRnW && !regIdRnCW[5];
`endif

always @(posedge clock)
begin
//	if(!hold)
//	if(!hold && !regFlushRnW)
//	if(holdN && !regFlushRnW)
	if(maskEnaG)
	begin
`ifndef def_true
		gprRegDlr	<= (regIdRnAW==JX2_GR_DLR) ? regValRnAW : regInDlr;
		gprRegDhr	<= (regIdRnAW==JX2_GR_DHR) ? regValRnAW : regInDhr;
		gprRegSp	<= (regIdRnAW==JX2_GR_SP ) ? regValRnAW : regInSp;
`endif
	end

`ifdef jx2_enable_gpr_rbsel
//	if(tRsbIdReg!=JX2_GR_ZZR)
	if(tRbsDoSwap || tRbsDoStore)
	begin
//		tRsbValReg	= tValRyA0;
//		tRsbIdReg	= regIdRy;
//		tRsbSelReg	= tRegTagRy[3:2];

		gprArrRbsBank[{tRsbSelReg, tRsbIdReg[5:0]}]	<= tRsbValReg;
	end
	tValRbs		<= gprArrRbsBank[{tRsbSelRegLd, tRsbIdReg[5:0]}];
	tIdRbs		<= tRsbIdReg;
	tRbsDidSwap	<= tRbsDoSwap;
`endif

	if(1'b1)
	begin
// `ifdef jx2_enable_gpr48
`ifdef jx2_enable_xgpr

// `ifdef jx2_enable_gpr_rbsel
`ifndef def_true

		if(maskEnaA)
		begin
			gprArrA[{regIdRbSel, regIdRnAW[5:0]}]	<= regValRnAW;
			gprArrMA[{regIdRbSel, regIdRnAW[5:0]}]	<= 1'b0;
			gprArrMB[{regIdRbSel, regIdRnAW[5:0]}]	<= 1'b0;
		end

		if(maskEnaB)
		begin
			gprArrB[{regIdRbSel, regIdRnBW[5:0]}]		<= regValRnBW;
			gprArrMA[{regIdRbSel, regIdRnBW[5:0]}]	<= 1'b1;
			gprArrMB[{regIdRbSel, regIdRnBW[5:0]}]	<= 1'b0;
		end

		if(maskEnaC)
		begin
			gprArrC[{regIdRbSel, regIdRnCW[5:0]}]		<= regValRnCW;
			gprArrMA[{regIdRbSel, regIdRnCW[5:0]}]	<= 1'b0;
			gprArrMB[{regIdRbSel, regIdRnCW[5:0]}]	<= 1'b1;
		end

`else

`ifdef jx2_enable_gpr_rbsel
//		if((tRsbIdReg!=JX2_GR_ZZR) && (tIdRbs == tRsbIdReg))
//		if(tIdRbs != JX2_GR_ZZR)
		if(tRbsDidSwap)
		begin
			gprArrA[tIdRbs[5:0]]	<= tValRbs;
			gprArrMA[tIdRbs[5:0]]	<= 1'b0;
			gprArrMB[tIdRbs[5:0]]	<= 1'b0;
			gprArrMC[tIdRbs[5:0]]	<= regIdRbSel[0];
			gprArrMD[tIdRbs[5:0]]	<= regIdRbSel[1];
		end
		else
`endif

//		if((regIdRnAW[5:4]!=2'b11) || tValRn3Pair)
//		if(!regIdRnAW[5] || tValRn3Pair)
//		if(!regIdRnAW[6])
		if(maskEnaA)
		begin
			gprArrA[regIdRnAW[5:0]]		<= regValRnAW;
			gprArrMA[regIdRnAW[5:0]]	<= 1'b0;
			gprArrMB[regIdRnAW[5:0]]	<= 1'b0;
`ifdef jx2_enable_gpr_rbsel
			gprArrMC[regIdRnAW[5:0]]	<= regIdRbSel[0];
			gprArrMD[regIdRnAW[5:0]]	<= regIdRbSel[1];
`endif
		end

//		if((regIdRnBW[5:4]!=2'b11) || tValRn3Pair)
//		if(!regIdRnBW[5] || tValRn3Pair)
//		if(!regIdRnBW[6])
		if(maskEnaB)
		begin
			gprArrB[regIdRnBW[5:0]]		<= regValRnBW;
			gprArrMA[regIdRnBW[5:0]]	<= 1'b1;
			gprArrMB[regIdRnBW[5:0]]	<= 1'b0;
`ifdef jx2_enable_gpr_rbsel
			gprArrMC[regIdRnBW[5:0]]	<= regIdRbSel[0];
			gprArrMD[regIdRnBW[5:0]]	<= regIdRbSel[1];
`endif
		end

//		if(regIdRnCW[5:4]!=2'b11)
//		if(!regIdRnCW[5])
//		if(!regIdRnCW[6])
		if(maskEnaC)
		begin
			gprArrC[regIdRnCW[5:0]]		<= regValRnCW;
			gprArrMA[regIdRnCW[5:0]]	<= 1'b0;
			gprArrMB[regIdRnCW[5:0]]	<= 1'b1;
`ifdef jx2_enable_gpr_rbsel
			gprArrMC[regIdRnCW[5:0]]	<= regIdRbSel[0];
			gprArrMD[regIdRnCW[5:0]]	<= regIdRbSel[1];
`endif
		end

// `ifdef jx2_enable_gpr_rbsel
//	reg[63:0]	gprArrMC;
//	reg[63:0]	gprArrMD;
// `endif

`endif

`else
//		if(!regIdRnAW[5])
		if(maskEnaA)
		begin
			gprArrA[regIdRnAW[4:0]]		<= regValRnAW;
			gprArrMA[regIdRnAW[4:0]]	<= 1'b0;
			gprArrMB[regIdRnAW[4:0]]	<= 1'b0;
		end

//		if(!regIdRnBW[5])
		if(maskEnaB)
		begin
			gprArrB[regIdRnBW[4:0]]		<= regValRnBW;
			gprArrMA[regIdRnBW[4:0]]	<= 1'b1;
			gprArrMB[regIdRnBW[4:0]]	<= 1'b0;
		end

//		if(!regIdRnCW[5])
		if(maskEnaC)
		begin
			gprArrC[regIdRnCW[4:0]]		<= regValRnCW;
			gprArrMA[regIdRnCW[4:0]]	<= 1'b0;
			gprArrMB[regIdRnCW[4:0]]	<= 1'b1;
		end
`endif
	end

end

endmodule

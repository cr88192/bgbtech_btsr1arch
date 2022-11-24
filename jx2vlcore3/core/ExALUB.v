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
ALU (Lane 2+)

Perform some ALU Operations
May Sign or Zero Extend output.

Leaves out operations which update SR.

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [  5]=QWord
  [  4]=ZExt (0=SX, 1=ZX)
  [3:0]=Op1

If GSV is enabled, Q+ZX = Packed DWord Ops

Op1:
  0000: ADD	/ PADD.L
  0001: SUB	/ PSUB.L
  0010: ADC	/ PADC.L
  0011: SBB	/ PSBB.L
  0100: - TST
  0101: AND
  0110: OR
  0111: XOR
  1000: - CMPNE
  1001: - CMPHS
  1010: - CMPGE
  1011: NOR
  1100: - CMPEQ
  1101: - CMPHI
  1110: - CMPGT
  1111: CSELT / PCSELT.L

V=!(S1^S2) & (S2^S3)

S1, S2, S3 -> V
 0,  0,  0 -> 0
 0,  0,  1 -> 1
 0,  1,  0 -> 0
 0,  1,  1 -> 0
 1,  0,  0 -> 0
 1,  0,  1 -> 0
 1,  1,  0 -> 1
 1,  1,  1 -> 0

 */

`include "CoreDefs.v"

`ifdef jx2_enable_clz
`include "ExOpClz.v"
`endif

`ifdef jx2_do_convfp16_alu

`ifdef jx2_enable_convfp16
`include "ExConv_Fp16Exp32.v"
`include "ExConv_Fp32Pck16.v"
`endif

`endif

module ExALUB(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	idUCmd,
	idUIxt,
	exHold,
	regInSrST,
	regOutVal,
	regOutSrST,
	regOutCarryD
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			exHold;
input[1:0]		regInSrST;

output[63:0]	regOutVal;
output[1:0]		regOutSrST;
output[7:0]		regOutCarryD;

wire			regInSrT;
wire			regInSrS;
assign		regInSrT = regInSrST[0];
assign		regInSrS = regInSrST[1];

reg[8:0]		idUIxt2;


reg[63:0]	tRegOutVal2;
reg			tRegOutSrT2;
reg			tRegOutSrS2;
assign	regOutVal = tRegOutVal2;
assign	regOutSrST = { tRegOutSrS2, tRegOutSrT2 };

reg[7:0]	tRegOutCarryD;
assign		regOutCarryD = tRegOutCarryD;

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
reg			tRegOutSrS;

// `ifdef def_true
`ifndef def_true
// `ifdef jx2_enable_clz
wire[7:0]		tClzVal;
ExOpClz	clz(
	clock,		reset,
	idUCmd,		idUIxt,
	regValRs,	tClzVal);
`endif

wire			tOpIsWx;

assign	tOpIsWx =
		(idUIxt[8:6] == JX2_IUC_WX) ||
		(idUIxt[8:6] == JX2_IUC_WT) ||
		(idUIxt[8:6] == JX2_IUC_WF) ||
		(idUIxt[8:6] == JX2_IUC_WXA);


`ifdef jx2_enable_conv2_alu

`ifdef jx2_enable_convfp16
wire[63:0]	tRegFp16Upck32;

wire[9:0]	tRegFp16UPckE = 10'h000;

// wire[31:0]	tRegFp16UPckT = (idUIxt[0] || tOpIsWx) ?
wire[31:0]	tRegFp16UPckT = idUIxt[0] ?
	regValRs[63:32] : regValRs[31: 0];
ExConv_Fp16Exp32	conv_fp16upcka(
	tRegFp16UPckT[15: 0], tRegFp16UPckE[4:0], tRegFp16Upck32[31: 0]);
ExConv_Fp16Exp32	conv_fp16upckb(
	tRegFp16UPckT[31:16], tRegFp16UPckE[9:5], tRegFp16Upck32[63:32]);
`endif

`endif


reg[16:0]	tAdd1A0;
reg[16:0]	tAdd1A1;
reg[16:0]	tAdd1B0;
reg[16:0]	tAdd1B1;

reg[16:0]	tAdd1C0;
reg[16:0]	tAdd1C1;
reg[16:0]	tAdd1D0;
reg[16:0]	tAdd1D1;

reg[16:0]	tSub1A0;
reg[16:0]	tSub1A1;
reg[16:0]	tSub1B0;
reg[16:0]	tSub1B1;

reg[16:0]	tSub1C0;
reg[16:0]	tSub1C1;
reg[16:0]	tSub1D0;
reg[16:0]	tSub1D1;

reg[32:0]	tAdd2A0;
reg[32:0]	tAdd2A1;
reg[32:0]	tAdd2B0;
reg[32:0]	tAdd2B1;

reg[32:0]	tSub2A0;
reg[32:0]	tSub2A1;
reg[32:0]	tSub2B0;
reg[32:0]	tSub2B1;

reg[1:0]	tAddCa1A0;
reg[1:0]	tAddCa1A1;
reg[1:0]	tAddCa1B0;
reg[1:0]	tAddCa1B1;

reg[1:0]	tSubCa1A0;
reg[1:0]	tSubCa1A1;
reg[1:0]	tSubCa1B0;
reg[1:0]	tSubCa1B1;

reg[3:0]	tAddCa2A0;
reg[3:0]	tAddCa2A1;
reg[3:0]	tSubCa2A0;
reg[3:0]	tSubCa2A1;

reg[4:0]	tAddCa2_Add;
reg[4:0]	tAddCa2_Adc;
reg[4:0]	tSubCa2_Sub;
reg[4:0]	tSubCa2_Sbb;

reg[64:0]	tAdd3A0;
reg[64:0]	tAdd3A1;
reg[64:0]	tSub3A0;
reg[64:0]	tSub3A1;

reg			tSub2ZF;
reg			tSub2CF;
reg			tSub2SF;
reg			tSub2VF;

reg			tSub1BZF;
reg			tSub1BCF;
reg			tSub1BSF;
reg			tSub1BVF;

reg			tSub1WZF_A;
reg			tSub1WCF_A;
reg			tSub1WSF_A;
reg			tSub1WZF_B;
reg			tSub1WCF_B;
reg			tSub1WSF_B;
reg			tSub1WZF_C;
reg			tSub1WCF_C;
reg			tSub1WSF_C;
reg			tSub1WZF_D;
reg			tSub1WCF_D;
reg			tSub1WSF_D;

reg			tSub1ZF;
reg			tSub1CF;
reg			tSub1SF;

reg[32:0]	tResult1A;
reg			tResult1T;

reg[32:0]	tResult1B;
reg			tResult1S;

reg[64:0]	tResult2A;
reg			tResult2T;

reg[64:0]	tResult_Add64;
reg[64:0]	tResult_Sub64;
reg[32:0]	tResult_Add32;
reg[32:0]	tResult_Sub32;

reg[32:0]	tResultu1A;
reg[32:0]	tResultu1B;
reg[64:0]	tResultu2A;

reg[32:0]	tResult1W;
reg[64:0]	tResult2W;
reg[32:0]	tResultShufB;
reg[64:0]	tResultShufW;

reg[63:0]	tRegConvVal;
reg			tRegConvSrT;
reg			tRegConvSrS;


always @*
begin
	tAdd1A0 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 0;
	tAdd1A1 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 1;
	tAdd1B0 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 0;
	tAdd1B1 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 1;
	tAdd1C0 = { 1'b0, regValRs[47:32] } + { 1'b0,  regValRt[47:32] } + 0;
	tAdd1C1 = { 1'b0, regValRs[47:32] } + { 1'b0,  regValRt[47:32] } + 1;
	tAdd1D0 = { 1'b0, regValRs[63:48] } + { 1'b0,  regValRt[63:48] } + 0;
	tAdd1D1 = { 1'b0, regValRs[63:48] } + { 1'b0,  regValRt[63:48] } + 1;

	tSub1A0 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 0;
	tSub1A1 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 1;
	tSub1B0 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 0;
	tSub1B1 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 1;
	tSub1C0 = { 1'b0, regValRs[47:32] } + { 1'b0, ~regValRt[47:32] } + 0;
	tSub1C1 = { 1'b0, regValRs[47:32] } + { 1'b0, ~regValRt[47:32] } + 1;
	tSub1D0 = { 1'b0, regValRs[63:48] } + { 1'b0, ~regValRt[63:48] } + 0;
	tSub1D1 = { 1'b0, regValRs[63:48] } + { 1'b0, ~regValRt[63:48] } + 1;
	
	tAdd2A0 = { tAdd1A0[16]?tAdd1B1:tAdd1B0, tAdd1A0[15:0] };
	tAdd2A1 = { tAdd1A1[16]?tAdd1B1:tAdd1B0, tAdd1A1[15:0] };
	tAdd2B0 = { tAdd1C0[16]?tAdd1D1:tAdd1D0, tAdd1C0[15:0] };
	tAdd2B1 = { tAdd1C1[16]?tAdd1D1:tAdd1D0, tAdd1C1[15:0] };

	tSub2A0 = { tSub1A0[16]?tSub1B1:tSub1B0, tSub1A0[15:0] };
	tSub2A1 = { tSub1A1[16]?tSub1B1:tSub1B0, tSub1A1[15:0] };
	tSub2B0 = { tSub1C0[16]?tSub1D1:tSub1D0, tSub1C0[15:0] };
	tSub2B1 = { tSub1C1[16]?tSub1D1:tSub1D0, tSub1C1[15:0] };

	tAdd3A0 = { tAdd2A0[32]?tAdd2B1:tAdd2B0, tAdd2A0[31:0] };
	tAdd3A1 = { tAdd2A1[32]?tAdd2B1:tAdd2B0, tAdd2A1[31:0] };
	tSub3A0 = { tSub2A0[32]?tSub2B1:tSub2B0, tSub2A0[31:0] };
	tSub3A1 = { tSub2A1[32]?tSub2B1:tSub2B0, tSub2A1[31:0] };

`ifdef def_true
	tAddCa1A0 = { tAdd1A0[16]?tAdd1B1[16]:tAdd1B0[16], tAdd1A0[16] };
	tAddCa1A1 = { tAdd1A1[16]?tAdd1B1[16]:tAdd1B0[16], tAdd1A1[16] };
	tAddCa1B0 = { tAdd1C0[16]?tAdd1D1[16]:tAdd1D0[16], tAdd1C0[16] };
	tAddCa1B1 = { tAdd1C1[16]?tAdd1D1[16]:tAdd1D0[16], tAdd1C1[16] };

	tSubCa1A0 = { tSub1A0[16]?tSub1B1[16]:tSub1B0[16], tSub1A0[16] };
	tSubCa1A1 = { tSub1A1[16]?tSub1B1[16]:tSub1B0[16], tSub1A1[16] };
	tSubCa1B0 = { tSub1C0[16]?tSub1D1[16]:tSub1D0[16], tSub1C0[16] };
	tSubCa1B1 = { tSub1C1[16]?tSub1D1[16]:tSub1D0[16], tSub1C1[16] };

	tAddCa2A0 = { tAddCa1A0[1]?tAddCa1B1:tAddCa1B0, tAddCa1A0 };
	tAddCa2A1 = { tAddCa1A1[1]?tAddCa1B1:tAddCa1B0, tAddCa1A1 };
	tSubCa2A0 = { tSubCa1A0[1]?tSubCa1B1:tSubCa1B0, tSubCa1A0 };
	tSubCa2A1 = { tSubCa1A1[1]?tSubCa1B1:tSubCa1B0, tSubCa1A1 };

	tAdd3A0 = {	tAddCa2A0[3],
				tAddCa2A0[2] ? tAdd1D1[15:0] :	tAdd1D0[15:0],
				tAddCa2A0[1] ? tAdd1C1[15:0] :	tAdd1C0[15:0],
				tAddCa2A0[0] ? tAdd1B1[15:0] :	tAdd1B0[15:0],
												tAdd1A0[15:0] };
	tAdd3A1 = {	tAddCa2A1[3],
				tAddCa2A1[2] ? tAdd1D1[15:0] :	tAdd1D0[15:0],
				tAddCa2A1[1] ? tAdd1C1[15:0] :	tAdd1C0[15:0],
				tAddCa2A1[0] ? tAdd1B1[15:0] :	tAdd1B0[15:0],
												tAdd1A1[15:0] };

	tSub3A0 = {	tSubCa2A0[3],	
				tSubCa2A0[2] ? tSub1D1[15:0] :	tSub1D0[15:0],
				tSubCa2A0[1] ? tSub1C1[15:0] :	tSub1C0[15:0],
				tSubCa2A0[0] ? tSub1B1[15:0] :	tSub1B0[15:0],
												tSub1A0[15:0] };
	tSub3A1 = {	tSubCa2A1[3],
				tSubCa2A1[2] ? tSub1D1[15:0] :	tSub1D0[15:0],
				tSubCa2A1[1] ? tSub1C1[15:0] :	tSub1C0[15:0],
				tSubCa2A1[0] ? tSub1B1[15:0] :	tSub1B0[15:0],
												tSub1A1[15:0] };

	tAdd2A0 = { tAddCa1A0[1], tAdd3A0[31:0] };
	tAdd2A1 = { tAddCa1A1[1], tAdd3A1[31:0] };
	tSub2A0 = { tSubCa1A0[1], tSub3A0[31:0] };
	tSub2A1 = { tSubCa1A1[1], tSub3A1[31:0] };

	tAddCa2_Add = { tAddCa2A0, 1'b0 };
	tSubCa2_Sub = { tSubCa2A1, 1'b1 };
	tAddCa2_Adc = { regInSrT ? tAddCa2A1 : tAddCa2A0, regInSrT };
	tSubCa2_Sbb = { regInSrT ? tSubCa2A0 : tSubCa2A1, !regInSrT };

	if(idUIxt[1])
	begin
		tAddCa2_Add = tAddCa2_Adc;
		tSubCa2_Sub = tSubCa2_Sbb;
	end

	tResult_Add64	= {	tAddCa2_Add[3],
				tAddCa2_Add[3] ? tAdd1D1[15:0] : tAdd1D0[15:0],
				tAddCa2_Add[2] ? tAdd1C1[15:0] : tAdd1C0[15:0],
				tAddCa2_Add[1] ? tAdd1B1[15:0] : tAdd1B0[15:0],
				tAddCa2_Add[0] ? tAdd1A1[15:0] : tAdd1A0[15:0]};
	tResult_Sub64	= {	tSubCa2_Sub[3],
				tSubCa2_Sub[3] ? tSub1D1[15:0] : tSub1D0[15:0],
				tSubCa2_Sub[2] ? tSub1C1[15:0] : tSub1C0[15:0],
				tSubCa2_Sub[1] ? tSub1B1[15:0] : tSub1B0[15:0],
				tSubCa2_Sub[0] ? tSub1A1[15:0] : tSub1A0[15:0] };

	tResult_Add32 = { tAddCa2_Add[2], tResult_Add64[31:0] };
	tResult_Sub32 = { tSubCa2_Sub[2], tResult_Sub64[31:0] };
`endif

	tSub1WZF_A	= (tSub2A1[15: 0]==0);
	tSub1WZF_B	= (tSub2A1[31:16]==0);
	tSub1WZF_C	= (tSub3A1[47:32]==0);
	tSub1WZF_D	= (tSub3A1[63:48]==0);

	tSub1WCF_A	= tSub1A0[16];
	tSub1WCF_B	= tSub1B0[16];
	tSub1WCF_C	= tSub1C0[16];
	tSub1WCF_D	= tSub1D0[16];

	tSub1WSF_A	= tSub1A0[15];
	tSub1WSF_B	= tSub1B0[15];
	tSub1WSF_C	= tSub1C0[15];
	tSub1WSF_D	= tSub1D0[15];

	tSub1ZF		= tSub1WZF_A && tSub1WZF_B;
	tSub1BZF	= tSub1WZF_C && tSub1WZF_D;

	tSub2ZF		= tSub1ZF && tSub1BZF;

//	tSub1CF = tSub2A1[32];
//	tSub2CF = tSub3A1[64];
	tSub1CF = tSubCa2A1[1];
	tSub2CF = tSubCa2A1[3];

	tSub1SF = tSub2A1[31];
	tSub2SF = tSub3A1[63];

	tSub1BCF = tSub2B1[32];
	tSub1BSF = tSub2B1[31];

	tRegOutCarryD = {
		3'b000,
		tSub2ZF,
		tSubCa2A1[3], tSubCa2A0[3], tAddCa2A1[3], tAddCa2A0[3] };

	tResult1A=UV33_XX;
	tResult2A=UV65_XX;
	tResult1T=regInSrT;
	tResult2T=regInSrT;
	tRegConvSrT=regInSrT;
	tRegConvSrS=regInSrS;

	tResult1B=UV33_XX;
	tResult1S=regInSrS;

	tResult1W = UV33_00;
	tResult2W = UV65_00;

	tResultShufB = UV33_00;
	tResultShufW = UV65_00;

`ifdef def_true
	case(regValRt[1:0])
		2'b00: tResultShufB[ 7: 0]=regValRs[ 7: 0];
		2'b01: tResultShufB[ 7: 0]=regValRs[15: 8];
		2'b10: tResultShufB[ 7: 0]=regValRs[23:16];
		2'b11: tResultShufB[ 7: 0]=regValRs[31:24];
	endcase
	case(regValRt[3:2])
		2'b00: tResultShufB[15: 8]=regValRs[ 7: 0];
		2'b01: tResultShufB[15: 8]=regValRs[15: 8];
		2'b10: tResultShufB[15: 8]=regValRs[23:16];
		2'b11: tResultShufB[15: 8]=regValRs[31:24];
	endcase
	case(regValRt[5:4])
		2'b00: tResultShufB[23:16]=regValRs[ 7: 0];
		2'b01: tResultShufB[23:16]=regValRs[15: 8];
		2'b10: tResultShufB[23:16]=regValRs[23:16];
		2'b11: tResultShufB[23:16]=regValRs[31:24];
	endcase
	case(regValRt[7:6])
		2'b00: tResultShufB[31:24]=regValRs[ 7: 0];
		2'b01: tResultShufB[31:24]=regValRs[15: 8];
		2'b10: tResultShufB[31:24]=regValRs[23:16];
		2'b11: tResultShufB[31:24]=regValRs[31:24];
	endcase
`endif

`ifdef def_true
	case(regValRt[1:0])
		2'b00: tResultShufW[15:0]=regValRs[15: 0];
		2'b01: tResultShufW[15:0]=regValRs[31:16];
		2'b10: tResultShufW[15:0]=regValRs[47:32];
		2'b11: tResultShufW[15:0]=regValRs[63:48];
	endcase
	case(regValRt[3:2])
		2'b00: tResultShufW[31:16]=regValRs[15: 0];
		2'b01: tResultShufW[31:16]=regValRs[31:16];
		2'b10: tResultShufW[31:16]=regValRs[47:32];
		2'b11: tResultShufW[31:16]=regValRs[63:48];
	endcase
	case(regValRt[5:4])
		2'b00: tResultShufW[47:32]=regValRs[15: 0];
		2'b01: tResultShufW[47:32]=regValRs[31:16];
		2'b10: tResultShufW[47:32]=regValRs[47:32];
		2'b11: tResultShufW[47:32]=regValRs[63:48];
	endcase
	case(regValRt[7:6])
		2'b00: tResultShufW[63:48]=regValRs[15: 0];
		2'b01: tResultShufW[63:48]=regValRs[31:16];
		2'b10: tResultShufW[63:48]=regValRs[47:32];
		2'b11: tResultShufW[63:48]=regValRs[63:48];
	endcase
`endif

	case(idUIxt[3:0])
		4'h0: begin		/* ADD */
//			tResult1A=tAdd2A0;
//			tResult2A=tAdd3A0;
			tResult1A=tResult_Add32;
			tResult2A=tResult_Add64;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tAdd2B0;
			tResult1S=regInSrS;

			tResult2W = { 1'b0,
				tAdd1D0[15:0], tAdd1C0[15:0],
				tAdd1B0[15:0], tAdd1A0[15:0] };
		end
		4'h1: begin		/* SUB */
//			tResult1A=tSub2A1;
//			tResult2A=tSub3A1;
			tResult1A=tResult_Sub32;
			tResult2A=tResult_Sub64;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tSub2B1;
			tResult1S=regInSrS;

			tResult2W = { 1'b0,
                tSub1D1[15:0], tSub1C1[15:0],
				tSub1B1[15:0], tSub1A1[15:0] };
		end
		4'h2: begin		/* ADC */
//			tResult1A=regInSrT ? tAdd2A1 : tAdd2A0;
//			tResult2A=regInSrT ? tAdd3A1 : tAdd3A0;
			tResult1A=tResult_Add32;
			tResult2A=tResult_Add64;
			tResult1T=tResult1A[32];
			tResult2T=tResult2A[64];

			tResult1B=regInSrS ? tAdd2B1 : tAdd2B0;
			tResult1S=tResult1B[32];

			tResult2W = { 1'b0, regValRs[63:32], regValRt[31:0] };
		end
		4'h3: begin		/* SBB */
//			tResult1A=regInSrT ? tSub2A0 : tSub2A1;
//			tResult2A=regInSrT ? tSub3A0 : tSub3A1;
			tResult1A=tResult_Sub32;
			tResult2A=tResult_Sub64;
			tResult1T=!tResult1A[32];
			tResult2T=!tResult2A[64];

			tResult1B=regInSrS ? tSub2B0 : tSub2B1;
			tResult1S=!tResult1B[32];

			tResult2W = { 1'b0, regValRs[31:0], regValRt[63:32] };
		end
		
		4'h4: begin		/* TST */
		end
		4'h5: begin		/* AND */
			tResult1A={1'b0, regValRs[31:0] & regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] & regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult1W = tResultShufB;
			tResult2W = tResultShufW;
		end
		4'h6: begin		/* OR */
			tResult1A={1'b0, regValRs[31:0] | regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] | regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult2W = { 1'b0, regValRs[31:0], regValRt[31:0] };
		end
		4'h7: begin		/* XOR */
			tResult1A={1'b0, regValRs[31:0] ^ regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] ^ regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult2W = { 1'b0, regValRs[63:32], regValRt[63:32] };
		end

		4'h8: begin		/* CMPNE */
		end
		4'h9: begin		/* CMPHS */
		end
		4'hA: begin		/* CMPGE */

			tResult2W = { 1'b0, regValRs[63:0] };
			casez(regValRt[4:0])
				5'bzzzz0: tResult2W[63:60]=regValRt[4:1];
				5'bzzz01: tResult2W[63:61]=regValRt[4:2];
				5'bzz011: tResult2W[63:62]=regValRt[4:3];
				5'bz0111: tResult2W[63   ]=regValRt[4];
				default: begin end
			endcase
		end
		4'hB: begin		/* NOR */
//			tResult2W = { 1'b0, regValRs[63:48], regValRt[47:0] };
			tResult2W = { 1'b0, regValRt[63:48], regValRs[47:0] };
		end

		4'hC: begin		/* CMPEQ */
//			tResult1T=tSub1ZF;
//			tResult2T=tSub2ZF;
//			tResult1S=tSub1BZF;
		end
		4'hD: begin		/* CMPHI */
//			tResult1T=tSub1CF && !tSub1ZF;
//			tResult2T=tSub2CF && !tSub2ZF;
//			tResult1S=tSub1BCF && !tSub1BZF;
		end
		4'hE: begin		/* CMPGT */
		end
		4'hF: begin		/* CSELT */
			tResult1A={1'b0, regInSrT ? regValRs[31: 0] : regValRt[31: 0] };
			tResult2A={1'b0, regInSrT ? regValRs[63: 0] : regValRt[63: 0] };
			tResult1B={1'b0, regInSrS ? regValRs[31: 0] : regValRt[31: 0] };
		end
	endcase

`ifdef jx2_enable_aluunary
// `ifndef def_true
	tResultu1A=0;
	tResultu1B=0;
	tResultu2A=0;

	case(idUIxt[3:0])
// `ifdef jx2_enable_clz
`ifndef def_true
		4'h0: begin
			tResultu1A = { UV25_00, tClzVal };
			tResultu2A = { UV57_00, tClzVal };
		end
`endif

`ifdef jx2_enable_pmort
		4'b0011: begin
			tResultu1B = { 1'b0,
				regValRs[47], regValRs[15], regValRs[46], regValRs[14],
				regValRs[45], regValRs[13], regValRs[44], regValRs[12],
				regValRs[43], regValRs[11], regValRs[42], regValRs[10],
				regValRs[41], regValRs[ 9], regValRs[40], regValRs[ 8],
				regValRs[39], regValRs[ 7], regValRs[38], regValRs[ 6],
				regValRs[37], regValRs[ 5], regValRs[36], regValRs[ 4],
				regValRs[35], regValRs[ 3], regValRs[34], regValRs[ 2],
				regValRs[33], regValRs[ 1], regValRs[32], regValRs[ 0]
			};
			tResultu1A = { 1'b0,
				regValRs[63], regValRs[31], regValRs[62], regValRs[30],
				regValRs[61], regValRs[29], regValRs[60], regValRs[28],
				regValRs[59], regValRs[27], regValRs[58], regValRs[26],
				regValRs[57], regValRs[25], regValRs[56], regValRs[24],
				regValRs[55], regValRs[23], regValRs[54], regValRs[22],
				regValRs[53], regValRs[21], regValRs[52], regValRs[20],
				regValRs[51], regValRs[19], regValRs[50], regValRs[18],
				regValRs[49], regValRs[17], regValRs[48], regValRs[16]
			};
			tResultu2A = { tResultu1A, tResultu1B[31:0] };
		end
`endif

		default: begin
		end
	endcase

	if(idUCmd[5:0]==JX2_UCMD_UNARY)
	begin
		tResult1A = tResultu1A;
		tResult1B = tResultu1B;
		tResult2A = tResultu2A;
	end
`endif

`ifdef jx2_enable_conv2_alu
	tRegConvVal = UV64_XX;
	case(idUIxt[5:0])
`ifdef jx2_enable_convfp16
		JX2_UCIX_CONV_FP16UPCK32L: begin
			tRegConvVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16UPCK32H: begin
			tRegConvVal = tRegFp16Upck32;
		end
`endif

		default: begin
			tRegConvVal = UV64_XX;
		end

	endcase
`endif

`ifdef jx2_enable_gsv
	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	begin
//		tResult1A = tResultu1A;
//		tResult1B = tResultu1B;
		tResult1A = tResult1W;
		tResult2A = tResult2W;
	end
`endif

	if(idUIxt[5])
	begin
`ifdef jx2_enable_gsv
		if(idUIxt[4])
		begin
			tRegOutVal = { tResult1B[31:0], tResult1A[31:0] };
			tRegOutSrT = tResult1T;
			tRegOutSrS = tResult1S;
		end
		else
		begin
			tRegOutVal = tResult2A[63:0];
			tRegOutSrT = tResult2T;
			tRegOutSrS = regInSrS;
		end
`else
		tRegOutVal = tResult2A[63:0];
		tRegOutSrT = tResult2T;
		tRegOutSrS = regInSrS;
`endif
	end
	else
	begin
//		if(idUIxt[4])
//			tRegOutVal = { UV32_00, tResult1A[31:0] };
//		else
//			tRegOutVal = { tResult1A[31]?UV32_FF:UV32_00, tResult1A[31:0] };

		tRegOutVal = {
			(tResult1A[31] && !idUIxt[4]) ? UV32_FF : UV32_00,
			tResult1A[31:0] };

		tRegOutSrT = tResult1T;
		tRegOutSrS = regInSrS;
	end

`ifdef jx2_debug_alu
	if((idUCmd[5:0]==JX2_UCMD_ALU3) && !exHold)
	begin
		$display("ALU: Op=%X Rs=%X Rt=%X Rn=%X",
			idUIxt,
			regValRs, regValRt, tRegOutVal);
	end

	if((idUCmd[5:0]==JX2_UCMD_ALUCMP) && !exHold)
	begin
		$display("ALUCMP: Op=%X Rs=%X Rt=%X SR.T=%X, S=%d V=%d Z=%d C=%d",
			idUIxt,
			regValRs, regValRt, tRegOutSrT,
			tSub1SF, tSub1VF, tSub1ZF, tSub1CF);
	end
`endif

`ifndef def_true
//`ifdef jx2_fcmp_alu
//`ifdef jx2_fpu_longdbl
	if(idUCmd[5:0]==JX2_UCMD_FCMP)
	begin
		if(idUIxt[3:0]==JX2_UCIX_FPU_CMPEQ[3:0])
		begin
			tRegOutSrT = tSub2ZF;
		end
		else
		begin
			tRegOutSrT = tSub2CF && !tSub2ZF;
		end
	end
//`endif
//`endif
`endif

`ifdef jx2_enable_conv2_alu
	if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
	begin
		tRegOutVal = tRegConvVal;
		tRegOutSrT = tRegConvSrT;
		tRegOutSrS = tRegConvSrS;
	end	
`endif

end

always @(posedge clock)
begin
	if(!exHold)
	begin
		idUIxt2			<= idUIxt;
		tRegOutVal2		<= tRegOutVal;
		tRegOutSrT2		<= tRegOutSrT;
		tRegOutSrS2		<= tRegOutSrS;
	end
end

endmodule

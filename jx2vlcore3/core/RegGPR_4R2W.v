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
GPR Module with 4 read ports and 2 write ports.
 */

`include "CoreDefs.v"

`include "RegSpr_2W.v"

module RegGPR_4R2W(
	clock,
	reset,
	hold,

	regIdUCmd,
	regIdUIxt,

	regIdRs,		//Source A, ALU / Base (Lane 1)
	regIdRt,		//Source B, ALU / Index (Lane 1)
	regIdRu,		//Source C, ALU / Base (Lane 2)
	regIdRv,		//Source D, ALU / Index (Lane 2) / MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRu,		//Source C Value
	regValRv,		//Source D Value

	regIdRnA1,		//Destination ID (EX1, Lane 1)
	regValRnA1,		//Destination Value (EX1, Lane 1)
	regIdRnB1,		//Destination ID (EX1, Lane 2)
	regValRnB1,		//Destination Value (EX1, Lane 2)

	regIdRnA2,		//Destination ID (EX2, Lane 1)
	regValRnA2,		//Destination Value (EX2, Lane 1)	
	regIdRnB2,		//Destination ID (EX2, Lane 2)
	regValRnB2,		//Destination Value (EX2, Lane 2)

	regIdRnA3,		//Destination ID (EX2, Lane 1)
	regValRnA3,		//Destination Value (EX2, Lane 1)
	regIdRnB3,		//Destination ID (EX2, Lane 2)
	regValRnB3,		//Destination Value (EX2, Lane 2)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValTbr,		//TBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValLr,		//LR Value (CR)
	regValSsp,		//SSP Value (CR)
	regValCm,		//Cm Value (CR)
	regValBPc,		//Base PC Value
	
	gprEx1Flush,
	gprEx2Flush,
	gprEx3Flush,

	gprEx1DualLane,
	gprEx2DualLane,
	gprEx3DualLane,

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

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRu;
output[63:0]	regValRv;

`input_gpr		regIdRnA1;		//Destination ID
input[63:0]		regValRnA1;		//Destination Value
`input_gpr		regIdRnA2;		//Destination ID
input[63:0]		regValRnA2;		//Destination Value
`input_gpr		regIdRnA3;		//Destination ID
input[63:0]		regValRnA3;		//Destination Value

`input_gpr		regIdRnB1;		//Destination ID
input[63:0]		regValRnB1;		//Destination Value
`input_gpr		regIdRnB2;		//Destination ID
input[63:0]		regValRnB2;		//Destination Value
`input_gpr		regIdRnB3;		//Destination ID
input[63:0]		regValRnB3;		//Destination Value


input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValBPc;		//Base PC Value
input [47:0]	regValGbr;		//GBR Value (CR)
input [47:0]	regValTbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [63:0]	regValLr;		//LR Value (CR)
input [63:0]	regValSsp;		//SSP Value (CR)
input [63:0]	regValCm;		//Cm Value (CR)

input			gprEx1Flush;
input			gprEx2Flush;
input			gprEx3Flush;

input			gprEx1DualLane;
input			gprEx2DualLane;
input			gprEx3DualLane;

wire			gprId2DualLane;
assign		gprId2DualLane = (regIdUIxt[7:6] == 2'b11);

parameter		noLane3 = 0;


output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

reg[63:0]	tRegValRs;
reg[63:0]	tRegValRt;
reg[63:0]	tRegValRu;
reg[63:0]	tRegValRv;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRu = tRegValRu;
assign	regValRv = tRegValRv;


`wire_gpr		regIdRnAW;		//Destination ID
wire[63:0]		regValRnAW;		//Destination Value
`wire_gpr		regIdRnBW;		//Destination ID
wire[63:0]		regValRnBW;		//Destination Value
wire			regFlushRnW;	//Flush Stage

assign	regIdRnAW	= regIdRnA3;
assign	regValRnAW	= regValRnA3;
assign	regIdRnBW	= regIdRnB3;
assign	regValRnBW	= regValRnB3;
assign	regFlushRnW	= gprEx3Flush;


`wire_gpr		regIdRnA1B;
`wire_gpr		regIdRnA2B;
`wire_gpr		regIdRnB1B;
`wire_gpr		regIdRnB2B;

assign		regIdRnA1B = regIdRnA1;
assign		regIdRnB1B = regIdRnB1;
assign		regIdRnA2B = regIdRnA2;
assign		regIdRnB2B = regIdRnB2;

`wire_gpr		regIdRnA3B;		//Destination ID
`wire_gpr		regIdRnB3B;		//Destination ID
assign		regIdRnA3B = regIdRnA3;
assign		regIdRnB3B = regIdRnB3;

// `ifdef jx2_enable_gpr48
`ifdef jx2_enable_xgpr
reg[63:0]	gprArrA[63:0];
reg[63:0]	gprArrB[63:0];
reg[63:0]	gprArrMA;
reg[63:0]	gprArrMB;
`else
reg[63:0]	gprArrA[31:0];
reg[63:0]	gprArrB[31:0];
reg[31:0]	gprArrMA;
reg[31:0]	gprArrMB;
`endif

`ifdef def_true
wire[63:0]	gprRegDlr;
wire[63:0]	gprRegDhr;
wire[63:0]	gprRegSp;

RegSpr_2W	gprModDlr(
	clock,		reset,
	JX2_GR_DLR,	gprRegDlr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	gprRegDlr,	hold,
	regFlushRnW);

RegSpr_2W	gprModDhr(
	clock,		reset,
	JX2_GR_DHR,	gprRegDhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	gprRegDhr,	hold,
	regFlushRnW);

RegSpr_2W	gprModSp(
	clock,		reset,
	JX2_GR_SP,	gprRegSp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regInSp,	hold,
	regFlushRnW);

`endif	


assign	regOutDlr = gprRegDlr;
assign	regOutDhr = gprRegDhr;
assign	regOutSp  = gprRegSp;


reg[63:0]	tValRsA0;
reg[63:0]	tValRtA0;
reg[63:0]	tValRuA0;
reg[63:0]	tValRvA0;

reg[63:0]	tValRsA;
reg[63:0]	tValRtA;
reg[63:0]	tValRuA;
reg[63:0]	tValRvA;

reg[63:0]	tValJimm;
reg[63:0]	tValJimm56;
reg[63:0]	tValJimm56Vf;

reg[63:0]	tValFpImm16A;
reg[63:0]	tValFpImm10A;

reg[63:0]	tValFpImm16B;
reg[63:0]	tValFpImm10B;

reg[63:0]	tValImmRpA;
reg[63:0]	tValImmRpB;

reg		tValRsZz;
reg		tValRtZz;
reg		tValRuZz;
reg		tValRvZz;

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

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRuZz=0;
	tValRvZz=0;

	tValRsPair = 0;
	tValRtPair = 0;
	tValRxPair = 0;
	tValRn3Pair = 0;

	tRegEx1NoForward = 0;
	tRegEx2NoForward = 0;
	tRegEx3NoForward = 0;

`ifdef jx2_enable_wexjumbo
	tValJimm={
		regValImmB[31:0],
		regValImmA[31:0] };
`endif

`ifdef jx2_use_fpu_fpimm
	tValFpImm16A	= {
		regValImmA[15:14],
		(regValImmA[14] || (regValImmA[14:10]==0)) &&
				(regValImmA[14:10]!=5'h1F) ?
			6'h00 : 6'h3F,
		regValImmA[13: 0],
		10'h0,
		32'h0
		};

	tValFpImm10A = 0;

	tValFpImm16B	= {
		regValImmB[15:14],
		(regValImmB[14] || (regValImmB[14:10]==0)) &&
				(regValImmB[14:10]!=5'h1F) ?
			6'h00 : 6'h3F,
		regValImmB[13: 0],
		10'h0,
		32'h0
		};
	
	tValJimm56Vf = {
		tValJimm[55:42], 2'b0,
		tValJimm[41:28], 2'b0,
		tValJimm[27:14], 2'b0,
		tValJimm[13: 0], 2'b0
	};
`endif

`ifdef jx2_enable_xgpr
	tValRsA0=
		gprArrMA[regIdRs[5:0]] ?
			gprArrB[regIdRs[5:0]] :
			gprArrA[regIdRs[5:0]];
	tValRtA0=
		gprArrMA[regIdRt[5:0]] ?
			gprArrB[regIdRt[5:0]] :
			gprArrA[regIdRt[5:0]];
	tValRuA0=
		gprArrMA[regIdRu[5:0]] ?
			gprArrB[regIdRu[5:0]] :
			gprArrA[regIdRu[5:0]];
	tValRvA0=
		gprArrMA[regIdRv[5:0]] ?
			gprArrB[regIdRv[5:0]] :
			gprArrA[regIdRv[5:0]];
`else
	tValRsA0=
		gprArrMA[regIdRs[4:0]] ?
			gprArrB[regIdRs[4:0]] :
			gprArrA[regIdRs[4:0]];
	tValRtA0=
		gprArrMA[regIdRt[4:0]] ?
			gprArrB[regIdRt[4:0]] :
			gprArrA[regIdRt[4:0]];
	tValRuA0=
		gprArrMA[regIdRu[4:0]] ?
			gprArrB[regIdRu[4:0]] :
			gprArrA[regIdRu[4:0]];
	tValRvA0=
		gprArrMA[regIdRv[4:0]] ?
			gprArrB[regIdRv[4:0]] :
			gprArrA[regIdRv[4:0]];
`endif

	casez(regIdRs)
		JX2_GR_GPR_Z:	tValRsA=tValRsA0;
		JX2_GR_DLR:	tValRsA=gprRegDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
		JX2_GR_SP:	tValRsA=gprRegSp;

`ifdef jx2_enable_vaddr48
		JX2_GR_PC:	tValRsA={ UV16_00, regValPc[47:1],
			regValPc[0] &tEnablePcLsb };
		JX2_GR_GBR:	tValRsA={ UV16_00, regValGbr };
		JX2_GR_TBR:	tValRsA={ UV16_00, regValTbr };
		JX2_GR_LR:	tValRsA= regValLr;

`ifdef jx2_enable_riscv
		JX2_GR_BPC:	tValRsA={ UV16_00, regValBPc };
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
		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, JX2_GR_SSP,
		JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
			tValRsA = regValCm;

`ifdef jx2_enable_xgpr
		JX2_GR_TEAH, JX2_GR_VIPT: 
			tValRsA = regValCm;
`endif
`endif


		JX2_GR_IMM: begin
			tValRsA={
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRsZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRsA=UV64_00;
			tValRsZz=1;
		end

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
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
			tValRsA={
				regValImmA[32]?UV36_FF:UV36_00,
				regValImmA[31:4]
				};
			tValRsZz=1;
		end
`endif

`ifdef jx2_enable_ldirot
// `ifndef def_true
		JX2_GR_R8IMMH: begin
			tValRsA={
				regValImmA[32]?UV40_FF:UV40_00,
				regValImmA[31:8]
				};
			tValRsZz=1;
		end
`endif

		default: 	tValRsA=UV64_00;
	endcase
	
	casez(regIdRt)
		JX2_GR_GPR_Z:	tValRtA=tValRtA0;
		JX2_GR_DLR:	tValRtA=gprRegDlr;
//		JX2_GR_DLR:	tValRtA=regInDlr;
		JX2_GR_DHR:	tValRtA=gprRegDhr;
//		JX2_GR_DHR:	tValRtA=regInDhr;
		JX2_GR_SP:	tValRtA=gprRegSp;
//		JX2_GR_SP:	tValRtA=regInSp;

//		JX2_GR_SSP:	tValRtA=regValSsp;

		JX2_GR_IMM:	begin
			tValRtA={
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRtZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRtA=UV64_00;
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


		default: 	tValRtA=UV64_00;
	endcase

	casez(regIdRu)
		JX2_GR_GPR_Z:	tValRuA=tValRuA0;
		JX2_GR_DLR:	tValRuA=gprRegDlr;
//		JX2_GR_DLR:	tValRuA=regInDlr;
		JX2_GR_DHR:	tValRuA=gprRegDhr;
//		JX2_GR_DHR:	tValRuA=regInDhr;
		JX2_GR_SP:	tValRuA=gprRegSp;
//		JX2_GR_SP:	tValRuA=regInSp;

		JX2_GR_SSP:	tValRuA=regValSsp;

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRuA=tValJimm;
			tValRuZz=1;
		end
`endif

		JX2_GR_IMM:begin
			tValRuA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRuZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRuA=UV64_00;
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

		default: 	tValRuA=UV64_00;
	endcase

	casez(regIdRv)
		JX2_GR_GPR_Z:	tValRvA=tValRvA0;
		JX2_GR_DLR:	tValRvA=gprRegDlr;
//		JX2_GR_DLR:	tValRvA=regInDlr;
		JX2_GR_DHR:	tValRvA=gprRegDhr;
//		JX2_GR_DHR:	tValRvA=regInDhr;
		JX2_GR_SP:	tValRvA=gprRegSp;
//		JX2_GR_SP:	tValRvA=regInSp;

//		JX2_GR_SSP:	tValRvA=regValSsp;

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRvA=tValJimm;
			tValRvZz=1;
		end
`endif

		JX2_GR_IMM:begin
			tValRvA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRvZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRvA=UV64_00;
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

		default: 	tValRvA=UV64_00;
	endcase

	tRegValRs = tValRsA;
	tRegValRt = tValRtA;
	tRegValRu = tValRuA;
	tRegValRv = tValRvA;

	if(!tValRsZz)
	begin
		if(!tRegEx3NoForward)
		begin
			if(regIdRs==regIdRnB3B)
				tRegValRs=regValRnB3;
			if(regIdRs==regIdRnA3B)
				tRegValRs=regValRnA3;
		end

		if(!tRegEx2NoForward)
		begin
			if(regIdRs==regIdRnB2B)
				tRegValRs=regValRnB2;
			if(regIdRs==regIdRnA2B)
				tRegValRs=regValRnA2;
		end

		if(!tRegEx1NoForward)
		begin
			if(regIdRs==regIdRnB1B)
				tRegValRs=regValRnB1;
			if(regIdRs==regIdRnA1B)
				tRegValRs=regValRnA1;
		end
	end

	if(!tValRtZz)
	begin
		if(!tRegEx3NoForward)
		begin
			if(regIdRt==regIdRnB3B)
				tRegValRt=regValRnB3;
			if(regIdRt==regIdRnA3B)
				tRegValRt=regValRnA3;
		end

		if(!tRegEx2NoForward)
		begin
			if(regIdRt==regIdRnB2B)
				tRegValRt=regValRnB2;
			if(regIdRt==regIdRnA2B)
				tRegValRt=regValRnA2;
		end

		if(!tRegEx1NoForward)
		begin
			if(regIdRt==regIdRnB1B)
				tRegValRt=regValRnB1;
			if(regIdRt==regIdRnA1B)
				tRegValRt=regValRnA1;
		end
	end

	if(!tValRuZz)
	begin
		if(!tRegEx3NoForward)
		begin
			if(regIdRu==regIdRnB3B)
				tRegValRu=regValRnB3;
			if(regIdRu==regIdRnA3B)
				tRegValRu=regValRnA3;
		end

		if(!tRegEx2NoForward)
		begin
			if(regIdRu==regIdRnB2B)
				tRegValRu=regValRnB2;
			if(regIdRu==regIdRnA2B)
				tRegValRu=regValRnA2;
		end

		if(!tRegEx1NoForward)
		begin
			if(regIdRu==regIdRnB1B)
				tRegValRu=regValRnB1;
			if(regIdRu==regIdRnA1B)
				tRegValRu=regValRnA1;
		end
	end

	if(!tValRvZz)
	begin
		if(!tRegEx3NoForward)
		begin
			if(regIdRv==regIdRnB3B)
				tRegValRv=regValRnB3;
			if(regIdRv==regIdRnA3B)
				tRegValRv=regValRnA3;
		end

		if(!tRegEx2NoForward)
		begin
			if(regIdRv==regIdRnB2B)
				tRegValRv=regValRnB2;
			if(regIdRv==regIdRnA2B)
				tRegValRv=regValRnA2;
		end

		if(!tRegEx1NoForward)
		begin
			if(regIdRv==regIdRnB1B)
				tRegValRv=regValRnB1;
			if(regIdRv==regIdRnA1B)
				tRegValRv=regValRnA1;
		end
	end
end

always @(posedge clock)
begin
//	if(!hold)
	if(!hold && !regFlushRnW)
	begin
`ifndef def_true
		gprRegDlr	<= (regIdRnAW==JX2_GR_DLR) ? regValRnAW : regInDlr;
		gprRegDhr	<= (regIdRnAW==JX2_GR_DHR) ? regValRnAW : regInDhr;
		gprRegSp	<= (regIdRnAW==JX2_GR_SP ) ? regValRnAW : regInSp;
`endif

// `ifdef jx2_enable_gpr48
`ifdef jx2_enable_xgpr

//		if((regIdRnAW[5:4]!=2'b11) || tValRn3Pair)
//		if(!regIdRnAW[5] || tValRn3Pair)
		if(!regIdRnAW[6])
		begin
			gprArrA[regIdRnAW[5:0]]		<= regValRnAW;
			gprArrMA[regIdRnAW[5:0]]	<= 1'b0;
			gprArrMB[regIdRnAW[5:0]]	<= 1'b0;
		end

//		if((regIdRnBW[5:4]!=2'b11) || tValRn3Pair)
//		if(!regIdRnBW[5] || tValRn3Pair)
		if(!regIdRnBW[6])
		begin
			gprArrB[regIdRnBW[5:0]]		<= regValRnBW;
			gprArrMA[regIdRnBW[5:0]]	<= 1'b1;
			gprArrMB[regIdRnBW[5:0]]	<= 1'b0;
		end
`else
		if(!regIdRnAW[5])
		begin
			gprArrA[regIdRnAW[4:0]]		<= regValRnAW;
			gprArrMA[regIdRnAW[4:0]]	<= 1'b0;
			gprArrMB[regIdRnAW[4:0]]	<= 1'b0;
		end

		if(!regIdRnBW[5])
		begin
			gprArrB[regIdRnBW[4:0]]		<= regValRnBW;
			gprArrMA[regIdRnBW[4:0]]	<= 1'b1;
			gprArrMB[regIdRnBW[4:0]]	<= 1'b0;
		end
`endif

	end
end

endmodule

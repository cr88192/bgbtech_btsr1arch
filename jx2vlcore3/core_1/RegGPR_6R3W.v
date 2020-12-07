/*
GPR Module with 6 read ports and 3 write ports.
 */

`include "RegSpr_3W.v"

module RegGPR_6R3W(
	clock,
	reset,
	hold,

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

`ifdef jx2_stage_ex3
	regIdRnA3,		//Destination ID (EX2, Lane 1)
	regValRnA3,		//Destination Value (EX2, Lane 1)
	regIdRnB3,		//Destination ID (EX2, Lane 2)
	regValRnB3,		//Destination Value (EX2, Lane 2)
	regIdRnC3,		//Destination ID (EX2, Lane 3)
	regValRnC3,		//Destination Value (EX2, Lane 3)
`endif
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValImmC,		//Immediate (Decode, Lane 3)
	regValLr,		//LR Value (CR)
	
	gprEx1Flush,
	gprEx2Flush,
`ifdef jx2_stage_ex3
	gprEx3Flush,
`endif

	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
`ifdef jx2_sprs_elrehr
	regOutElr,	regInElr,
	regOutEhr,	regInEhr,
	regOutBp,	regInBp,
`endif
	regOutSp,	regInSp
	);


input			clock;
input			reset;
input			hold;

input[5:0]		regIdRs;		//Source A
input[5:0]		regIdRt;		//Source B
input[5:0]		regIdRu;		//Source C
input[5:0]		regIdRv;		//Source D
input[5:0]		regIdRx;		//Source E
input[5:0]		regIdRy;		//Source F

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRu;
output[63:0]	regValRv;
output[63:0]	regValRx;
output[63:0]	regValRy;

input[5:0]		regIdRnA1;		//Destination ID
input[63:0]		regValRnA1;		//Destination Value
input[5:0]		regIdRnA2;		//Destination ID
input[63:0]		regValRnA2;		//Destination Value
`ifdef jx2_stage_ex3
input[5:0]		regIdRnA3;		//Destination ID
input[63:0]		regValRnA3;		//Destination Value
`endif

input[5:0]		regIdRnB1;		//Destination ID
input[63:0]		regValRnB1;		//Destination Value
input[5:0]		regIdRnB2;		//Destination ID
input[63:0]		regValRnB2;		//Destination Value
`ifdef jx2_stage_ex3
input[5:0]		regIdRnB3;		//Destination ID
input[63:0]		regValRnB3;		//Destination Value
`endif

input[5:0]		regIdRnC1;		//Destination ID
input[63:0]		regValRnC1;		//Destination Value
input[5:0]		regIdRnC2;		//Destination ID
input[63:0]		regValRnC2;		//Destination Value
`ifdef jx2_stage_ex3
input[5:0]		regIdRnC3;		//Destination ID
input[63:0]		regValRnC3;		//Destination Value
`endif

input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [32:0]	regValImmC;		//Immediate (Decode)
input [47:0]	regValLr;		//GBR Value (CR)

input			gprEx1Flush;
input			gprEx2Flush;
`ifdef jx2_stage_ex3
input			gprEx3Flush;
`endif


output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

`ifdef jx2_sprs_elrehr
output[63:0]	regOutElr;
input [63:0]	regInElr;
output[63:0]	regOutEhr;
input [63:0]	regInEhr;
output[63:0]	regOutBp;
input [63:0]	regInBp;
`endif

reg[63:0]	tRegValRs;
reg[63:0]	tRegValRt;
reg[63:0]	tRegValRu;
reg[63:0]	tRegValRv;
reg[63:0]	tRegValRx;
reg[63:0]	tRegValRy;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRu = tRegValRu;
assign	regValRv = tRegValRv;
assign	regValRx = tRegValRx;
assign	regValRy = tRegValRy;


wire[5:0]		regIdRnAW;		//Destination ID
wire[63:0]		regValRnAW;		//Destination Value
wire[5:0]		regIdRnBW;		//Destination ID
wire[63:0]		regValRnBW;		//Destination Value
wire[5:0]		regIdRnCW;		//Destination ID
wire[63:0]		regValRnCW;		//Destination Value
wire			regFlushRnW;	//Flush Stage

// assign	regFlushRnW	= 1'b0;

`ifdef jx2_stage_ex3
assign	regIdRnAW	= regIdRnA3;
assign	regValRnAW	= regValRnA3;
assign	regIdRnBW	= regIdRnB3;
assign	regValRnBW	= regValRnB3;
assign	regIdRnCW	= regIdRnC3;
assign	regValRnCW	= regValRnC3;
assign	regFlushRnW	= gprEx3Flush;
`else
assign	regIdRnAW	= regIdRnA2;
assign	regValRnAW	= regValRnA2;
assign	regIdRnBW	= regIdRnB2;
assign	regValRnBW	= regValRnB2;
assign	regIdRnCW	= regIdRnC2;
assign	regValRnCW	= regValRnC2;
assign	regFlushRnW	= gprEx2Flush;
`endif


wire[5:0]		regIdRnA1B;
wire[5:0]		regIdRnA2B;
wire[5:0]		regIdRnB1B;
wire[5:0]		regIdRnB2B;
wire[5:0]		regIdRnC1B;
wire[5:0]		regIdRnC2B;

// assign		regIdRnA1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnA1;
// assign		regIdRnB1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnB1;
// assign		regIdRnC1B = gprEx1Flush ? JX2_GR_ZZR : regIdRnC1;
// assign		regIdRnA2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnA2;
// assign		regIdRnB2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnB2;
// assign		regIdRnC2B = gprEx2Flush ? JX2_GR_ZZR : regIdRnC2;

assign		regIdRnA1B = regIdRnA1;
assign		regIdRnB1B = regIdRnB1;
assign		regIdRnC1B = regIdRnC1;
assign		regIdRnA2B = regIdRnA2;
assign		regIdRnB2B = regIdRnB2;
assign		regIdRnC2B = regIdRnC2;


`ifdef jx2_stage_ex3
wire[5:0]		regIdRnA3B;		//Destination ID
wire[5:0]		regIdRnB3B;		//Destination ID
wire[5:0]		regIdRnC3B;		//Destination ID
// assign		regIdRnA3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnA3;
// assign		regIdRnB3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnB3;
// assign		regIdRnC3B = gprEx3Flush ? JX2_GR_ZZR : regIdRnC3;
assign		regIdRnA3B = regIdRnA3;
assign		regIdRnB3B = regIdRnB3;
assign		regIdRnC3B = regIdRnC3;
`endif


reg[63:0]	gprArrA[31:0];
reg[63:0]	gprArrB[31:0];
reg[63:0]	gprArrC[31:0];
reg[31:0]	gprArrMA;
reg[31:0]	gprArrMB;

// reg[63:0]	gprRegDlr;
// reg[63:0]	gprRegDhr;
// reg[63:0]	gprRegElr;
// reg[63:0]	gprRegEhr;
// reg[63:0]	gprRegSp;
// reg[63:0]	gprRegBp;

`ifdef def_true
wire[63:0]	gprRegDlr;
wire[63:0]	gprRegDhr;
wire[63:0]	gprRegSp;

RegSpr_3W	gprModDlr(
	clock,		reset,
	JX2_GR_DLR,	gprRegDlr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInDlr,	hold,
	regFlushRnW);

RegSpr_3W	gprModDhr(
	clock,		reset,
	JX2_GR_DHR,	gprRegDhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInDhr,	hold,
	regFlushRnW);

RegSpr_3W	gprModSp(
	clock,		reset,
	JX2_GR_SP,	gprRegSp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInSp,	hold,
	regFlushRnW);

`ifdef jx2_sprs_elrehr
wire[63:0]	gprRegElr;
wire[63:0]	gprRegEhr;
wire[63:0]	gprRegBp;

RegSpr_3W	gprModElr(
	clock,		reset,
	JX2_GR_ELR,	gprRegElr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInElr,	hold,
	regFlushRnW);

RegSpr_3W	gprModEhr(
	clock,		reset,
	JX2_GR_EHR,	gprRegEhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInEhr,	hold,
	regFlushRnW);

RegSpr_3W	gprModBp(
	clock,		reset,
	JX2_GR_BP,	gprRegBp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInBp,	hold,
	regFlushRnW);
`endif

`endif	


assign	regOutDlr = gprRegDlr;
assign	regOutDhr = gprRegDhr;
assign	regOutSp  = gprRegSp;

`ifdef jx2_sprs_elrehr
assign	regOutElr = gprRegElr;
assign	regOutEhr = gprRegEhr;
assign	regOutBp  = gprRegBp;
`endif

reg[63:0]	tValRsA0;
reg[63:0]	tValRtA0;
reg[63:0]	tValRuA0;
reg[63:0]	tValRvA0;
reg[63:0]	tValRxA0;
reg[63:0]	tValRyA0;

reg[63:0]	tValRsA;
reg[63:0]	tValRtA;
reg[63:0]	tValRuA;
reg[63:0]	tValRvA;
reg[63:0]	tValRxA;
reg[63:0]	tValRyA;

reg[63:0]	tValJimm;
reg[63:0]	tValJimm56;

reg	tValRsZz;
reg	tValRtZz;
reg	tValRuZz;
reg	tValRvZz;
reg	tValRxZz;
reg	tValRyZz;

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRuZz=0;
	tValRvZz=0;
	tValRxZz=0;
	tValRyZz=0;

`ifdef jx2_enable_wexjumbo
//	tValJimm={
//		regValImmC[19:0],
//		regValImmB[19:0],
//		regValImmA[23:0] };

	tValJimm={
		regValImmB[31:0],
		regValImmA[31:0] };

//	tValJimm56={
//		regValImmA[31] ? UV8_FF : UV8_00,
//		regValImmB[31:8],
//		regValImmA[31:0] };
`endif

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

	casez(regIdRs)
//		6'b0zzzzz:	tValRsA=gprArr[regIdRs[4:0]];
		6'b0zzzzz:	tValRsA=tValRsA0;
		JX2_GR_DLR:	tValRsA=gprRegDlr;
//		JX2_GR_DLR:	tValRsA=regInDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
//		JX2_GR_DHR:	tValRsA=regInDhr;
		JX2_GR_SP:	tValRsA=gprRegSp;
//		JX2_GR_SP:	tValRsA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRsA=gprRegElr;
		JX2_GR_EHR:	tValRsA=gprRegEhr;
		JX2_GR_BP:	tValRsA=gprRegBp;
`endif

`ifdef jx2_enable_vaddr48
		JX2_GR_PC:	tValRsA={ UV16_00, regValPc };
		JX2_GR_GBR:	tValRsA={ UV16_00, regValGbr };
		JX2_GR_LR:	tValRsA={ UV16_00, regValLr };
`else
		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:0] };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr[31:0] };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr[31:0] };
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

		default: 	tValRsA=UV64_XX;
	endcase

	casez(regIdRt)
		6'b0zzzzz:	tValRtA=tValRtA0;
		JX2_GR_DLR:	tValRtA=gprRegDlr;
//		JX2_GR_DLR:	tValRtA=regInDlr;
		JX2_GR_DHR:	tValRtA=gprRegDhr;
//		JX2_GR_DHR:	tValRtA=regInDhr;
		JX2_GR_SP:	tValRtA=gprRegSp;
//		JX2_GR_SP:	tValRtA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRtA=gprRegElr;
		JX2_GR_EHR:	tValRtA=gprRegEhr;
		JX2_GR_BP:	tValRtA=gprRegBp;
`endif

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

		default: 	tValRtA=UV64_XX;
	endcase

	casez(regIdRu)
		6'b0zzzzz:	tValRuA=tValRuA0;
		JX2_GR_DLR:	tValRuA=gprRegDlr;
//		JX2_GR_DLR:	tValRuA=regInDlr;
		JX2_GR_DHR:	tValRuA=gprRegDhr;
//		JX2_GR_DHR:	tValRuA=regInDhr;
		JX2_GR_SP:	tValRuA=gprRegSp;
//		JX2_GR_SP:	tValRuA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRuA=gprRegElr;
		JX2_GR_EHR:	tValRuA=gprRegEhr;
		JX2_GR_BP:	tValRuA=gprRegBp;
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

		default: 	tValRuA=UV64_XX;
	endcase

	casez(regIdRv)
		6'b0zzzzz:	tValRvA=tValRvA0;
		JX2_GR_DLR:	tValRvA=gprRegDlr;
//		JX2_GR_DLR:	tValRvA=regInDlr;
		JX2_GR_DHR:	tValRvA=gprRegDhr;
//		JX2_GR_DHR:	tValRvA=regInDhr;
		JX2_GR_SP:	tValRvA=gprRegSp;
//		JX2_GR_SP:	tValRvA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRvA=gprRegElr;
		JX2_GR_EHR:	tValRvA=gprRegEhr;
		JX2_GR_BP:	tValRvA=gprRegBp;
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

		default: 	tValRvA=UV64_XX;
	endcase

	casez(regIdRx)
		6'b0zzzzz:	tValRxA=tValRxA0;
		JX2_GR_DLR:	tValRxA=gprRegDlr;
//		JX2_GR_DLR:	tValRxA=regInDlr;
		JX2_GR_DHR:	tValRxA=gprRegDhr;
//		JX2_GR_DHR:	tValRxA=regInDhr;
		JX2_GR_SP:	tValRxA=gprRegSp;
//		JX2_GR_SP:	tValRxA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRxA=gprRegElr;
		JX2_GR_EHR:	tValRxA=gprRegEhr;
		JX2_GR_BP:	tValRxA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRxA={
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRxZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRxA=UV64_00;
			tValRxZz=1;
		end

		default: 	tValRxA=UV64_XX;
	endcase

	casez(regIdRy)
		6'b0zzzzz:	tValRyA=tValRyA0;
		JX2_GR_DLR:	tValRyA=gprRegDlr;
//		JX2_GR_DLR:	tValRyA=regInDlr;
		JX2_GR_DHR:	tValRyA=gprRegDhr;
//		JX2_GR_DHR:	tValRyA=regInDhr;
		JX2_GR_SP:	tValRyA=gprRegSp;
//		JX2_GR_SP:	tValRyA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRyA=gprRegElr;
		JX2_GR_EHR:	tValRyA=gprRegEhr;
		JX2_GR_BP:	tValRyA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRyA={
				regValImmC[32]?UV32_FF:UV32_00,
				regValImmC[31:0] };
			tValRyZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRyA=UV64_00;
			tValRyZz=1;
		end

		default: 	tValRyA=UV64_XX;
	endcase

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
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRs==regIdRnC3B)
				tRegValRs=regValRnC3;
			if(regIdRs==regIdRnB3B)
				tRegValRs=regValRnB3;
			if(regIdRs==regIdRnA3B)
				tRegValRs=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRs==regIdRnC2B)
				tRegValRs=regValRnC2;
			if(regIdRs==regIdRnB2B)
				tRegValRs=regValRnB2;
			if(regIdRs==regIdRnA2B)
				tRegValRs=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRs==regIdRnC1B)
				tRegValRs=regValRnC1;
			if(regIdRs==regIdRnB1B)
				tRegValRs=regValRnB1;
			if(regIdRs==regIdRnA1B)
				tRegValRs=regValRnA1;
		end
	end

	if(!tValRtZz)
	begin
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRt==regIdRnC3B)
				tRegValRt=regValRnC3;
			if(regIdRt==regIdRnB3B)
				tRegValRt=regValRnB3;
			if(regIdRt==regIdRnA3B)
				tRegValRt=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRt==regIdRnC2B)
				tRegValRt=regValRnC2;
			if(regIdRt==regIdRnB2B)
				tRegValRt=regValRnB2;
			if(regIdRt==regIdRnA2B)
				tRegValRt=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRt==regIdRnC1B)
				tRegValRt=regValRnC1;
			if(regIdRt==regIdRnB1B)
				tRegValRt=regValRnB1;
			if(regIdRt==regIdRnA1B)
				tRegValRt=regValRnA1;
		end
	end

	if(!tValRuZz)
	begin
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRu==regIdRnC3B)
				tRegValRu=regValRnC3;
			if(regIdRu==regIdRnB3B)
				tRegValRu=regValRnB3;
			if(regIdRu==regIdRnA3B)
				tRegValRu=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRu==regIdRnC2B)
				tRegValRu=regValRnC2;
			if(regIdRu==regIdRnB2B)
				tRegValRu=regValRnB2;
			if(regIdRu==regIdRnA2B)
				tRegValRu=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRu==regIdRnC1B)
				tRegValRu=regValRnC1;
			if(regIdRu==regIdRnB1B)
				tRegValRu=regValRnB1;
			if(regIdRu==regIdRnA1B)
				tRegValRu=regValRnA1;
		end
	end

	if(!tValRvZz)
	begin
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRv==regIdRnC3B)
				tRegValRv=regValRnC3;
			if(regIdRv==regIdRnB3B)
				tRegValRv=regValRnB3;
			if(regIdRv==regIdRnA3B)
				tRegValRv=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRv==regIdRnC2B)
				tRegValRv=regValRnC2;
			if(regIdRv==regIdRnB2B)
				tRegValRv=regValRnB2;
			if(regIdRv==regIdRnA2B)
				tRegValRv=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRv==regIdRnC1B)
				tRegValRv=regValRnC1;
			if(regIdRv==regIdRnB1B)
				tRegValRv=regValRnB1;
			if(regIdRv==regIdRnA1B)
				tRegValRv=regValRnA1;
		end
	end


	if(!tValRxZz)
	begin
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRx==regIdRnC3B)
				tRegValRx=regValRnC3;
			if(regIdRx==regIdRnB3B)
				tRegValRx=regValRnB3;
			if(regIdRx==regIdRnA3B)
				tRegValRx=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRx==regIdRnC2B)
				tRegValRx=regValRnC2;
			if(regIdRx==regIdRnB2B)
				tRegValRx=regValRnB2;
			if(regIdRx==regIdRnA2B)
				tRegValRx=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRx==regIdRnC1B)
				tRegValRx=regValRnC1;
			if(regIdRx==regIdRnB1B)
				tRegValRx=regValRnB1;
			if(regIdRx==regIdRnA1B)
				tRegValRx=regValRnA1;
		end
	end

	if(!tValRyZz)
	begin
`ifdef jx2_stage_ex3
//		if(!gprEx3Flush)
		if(1'b1)
		begin
			if(regIdRy==regIdRnC3B)
				tRegValRy=regValRnC3;
			if(regIdRy==regIdRnB3B)
				tRegValRy=regValRnB3;
			if(regIdRy==regIdRnA3B)
				tRegValRy=regValRnA3;
		end
`endif

//		if(!gprEx2Flush)
		if(1'b1)
		begin
			if(regIdRy==regIdRnC2B)
				tRegValRy=regValRnC2;
			if(regIdRy==regIdRnB2B)
				tRegValRy=regValRnB2;
			if(regIdRy==regIdRnA2B)
				tRegValRy=regValRnA2;
		end

//		if(!gprEx1Flush)
		if(1'b1)
		begin
			if(regIdRy==regIdRnC1B)
				tRegValRy=regValRnC1;
			if(regIdRy==regIdRnB1B)
				tRegValRy=regValRnB1;
			if(regIdRy==regIdRnA1B)
				tRegValRy=regValRnA1;
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

`ifdef jx2_sprs_elrehr
		gprRegElr	<= (regIdRnAW==JX2_GR_ELR) ? regValRnAW : regInElr;
		gprRegEhr	<= (regIdRnAW==JX2_GR_EHR) ? regValRnAW : regInEhr;
		gprRegBp	<= (regIdRnAW==JX2_GR_BP ) ? regValRnAW : regInBp;
`endif

`endif

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

		if(!regIdRnCW[5])
		begin
			gprArrC[regIdRnCW[4:0]]		<= regValRnCW;
			gprArrMA[regIdRnCW[4:0]]	<= 1'b0;
			gprArrMB[regIdRnCW[4:0]]	<= 1'b1;
		end
	end
end

endmodule

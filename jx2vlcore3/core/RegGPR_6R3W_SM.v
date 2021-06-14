/*
GPR Module with 6 read ports and 3 write ports.

Implement registers entirely with state machines.
 */

`include "CoreDefs.v"
`include "RegSpr_3W.v"

module RegGPR_6R3W_SM(
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

	regIdRnA3,		//Destination ID (EX2, Lane 1)
	regValRnA3,		//Destination Value (EX2, Lane 1)
	regIdRnB3,		//Destination ID (EX2, Lane 2)
	regValRnB3,		//Destination Value (EX2, Lane 2)
	regIdRnC3,		//Destination ID (EX2, Lane 3)
	regValRnC3,		//Destination Value (EX2, Lane 3)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValImmC,		//Immediate (Decode, Lane 3)
	regValLr,		//LR Value (CR)
	regValCm,		//Cm Value (CR)
	
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

`input_gpr		regIdRs;		//Source A
`input_gpr		regIdRt;		//Source B
`input_gpr		regIdRu;		//Source C
`input_gpr		regIdRv;		//Source D
`input_gpr		regIdRx;		//Source E
`input_gpr		regIdRy;		//Source F

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRu;
output[63:0]	regValRv;
output[63:0]	regValRx;
output[63:0]	regValRy;

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

`input_gpr		regIdRnC1;		//Destination ID
input[63:0]		regValRnC1;		//Destination Value
`input_gpr		regIdRnC2;		//Destination ID
input[63:0]		regValRnC2;		//Destination Value
`input_gpr		regIdRnC3;		//Destination ID
input[63:0]		regValRnC3;		//Destination Value

input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [32:0]	regValImmC;		//Immediate (Decode)
input [47:0]	regValLr;		//LR Value (CR)
input [63:0]	regValCm;		//Cm Value (CR)
	
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


`wire_gpr		regIdRnAW;		//Destination ID
wire[63:0]		regValRnAW;		//Destination Value
`wire_gpr		regIdRnBW;		//Destination ID
wire[63:0]		regValRnBW;		//Destination Value
`wire_gpr		regIdRnCW;		//Destination ID
wire[63:0]		regValRnCW;		//Destination Value

assign	regIdRnAW	= regIdRnA3;
assign	regValRnAW	= regValRnA3;
assign	regIdRnBW	= regIdRnB3;
assign	regValRnBW	= regValRnB3;
assign	regIdRnCW	= regIdRnC3;
assign	regValRnCW	= regValRnC3;

// reg[63:0]	gprArrA[31:0];
// reg[63:0]	gprArrB[31:0];
// reg[63:0]	gprArrC[31:0];
// reg[31:0]	gprArrMA;
// reg[31:0]	gprArrMB;

// reg[63:0]	gprRegDlr;
// reg[63:0]	gprRegDhr;
// reg[63:0]	gprRegElr;
// reg[63:0]	gprRegEhr;
// reg[63:0]	gprRegSp;
// reg[63:0]	gprRegBp;

wire[63:0]	gprRegDlr;
wire[63:0]	gprRegDhr;
wire[63:0]	gprRegSp;

RegSpr_3W	gprModDlr(
	clock,		reset,
	JX2_GR_DLR,	gprRegDlr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInDlr,	hold);

RegSpr_3W	gprModDhr(
	clock,		reset,
	JX2_GR_DHR,	gprRegDhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInDhr,	hold);

RegSpr_3W	gprModSp(
	clock,		reset,
	JX2_GR_SP,	gprRegSp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInSp,	hold);

wire[63:0]	gprRegElr;
wire[63:0]	gprRegEhr;
wire[63:0]	gprRegBp;

`ifndef jx2_sprs_elrehr
// wire[63:0]	regInElr;
// wire[63:0]	regInEhr;
// wire[63:0]	regInBp;
// assign		regInElr = gprRegElr;
// assign		regInEhr = gprRegEhr;
// assign		regInBp = gprRegBp;
`endif

`ifdef jx2_sprs_elrehr

RegSpr_3W	gprModElr(
	clock,		reset,
	JX2_GR_ELR,	gprRegElr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInElr,	hold);

RegSpr_3W	gprModEhr(
	clock,		reset,
	JX2_GR_EHR,	gprRegEhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInEhr,	hold);

RegSpr_3W	gprModBp(
	clock,		reset,
	JX2_GR_BP,	gprRegBp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	regInBp,	hold);

`else

RegSpr_3W	gprModElr(
	clock,		reset,
	JX2_GR_R16,	gprRegElr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	gprRegElr,	hold);

RegSpr_3W	gprModEhr(
	clock,		reset,
	JX2_GR_R17,	gprRegEhr,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	gprRegEhr,	hold);

RegSpr_3W	gprModBp(
	clock,		reset,
	JX2_GR_R31,	gprRegBp,
	regIdRnAW,	regValRnAW,
	regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,
	gprRegBp,	hold);

`endif


assign	regOutDlr = gprRegDlr;
assign	regOutDhr = gprRegDhr;
assign	regOutSp  = gprRegSp;

`ifdef jx2_sprs_elrehr
assign	regOutElr = gprRegElr;
assign	regOutEhr = gprRegEhr;
assign	regOutBp  = gprRegBp;
`endif

wire[63:0]	gprRegR2;
wire[63:0]	gprRegR3;
wire[63:0]	gprRegR4;
wire[63:0]	gprRegR5;
wire[63:0]	gprRegR6;
wire[63:0]	gprRegR7;
wire[63:0]	gprRegR8;
wire[63:0]	gprRegR9;
wire[63:0]	gprRegR10;
wire[63:0]	gprRegR11;
wire[63:0]	gprRegR12;
wire[63:0]	gprRegR13;
wire[63:0]	gprRegR14;

RegSpr_3W	gprModR2(
	clock,		reset,			JX2_GR_R2,	gprRegR2,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR2,	hold);
RegSpr_3W	gprModR3(
	clock,		reset,			JX2_GR_R3,	gprRegR3,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR3,	hold);
RegSpr_3W	gprModR4(
	clock,		reset,			JX2_GR_R4,	gprRegR4,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR4,	hold);
RegSpr_3W	gprModR5(
	clock,		reset,			JX2_GR_R5,	gprRegR5,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR5,	hold);
RegSpr_3W	gprModR6(
	clock,		reset,			JX2_GR_R6,	gprRegR6,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR6,	hold);
RegSpr_3W	gprModR7(
	clock,		reset,			JX2_GR_R7,	gprRegR7,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR7,	hold);
RegSpr_3W	gprModR8(
	clock,		reset,			JX2_GR_R8,	gprRegR8,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR8,	hold);
RegSpr_3W	gprModR9(
	clock,		reset,			JX2_GR_R9,	gprRegR9,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR9,	hold);
RegSpr_3W	gprModR10(
	clock,		reset,			JX2_GR_R10,	gprRegR10,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR10,	hold);
RegSpr_3W	gprModR11(
	clock,		reset,			JX2_GR_R11,	gprRegR11,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR11,	hold);
RegSpr_3W	gprModR12(
	clock,		reset,			JX2_GR_R12,	gprRegR12,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR12,	hold);
RegSpr_3W	gprModR13(
	clock,		reset,			JX2_GR_R13,	gprRegR13,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR13,	hold);
RegSpr_3W	gprModR14(
	clock,		reset,			JX2_GR_R14,	gprRegR14,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR14,	hold);

wire[63:0]	gprRegR18;
wire[63:0]	gprRegR19;
wire[63:0]	gprRegR20;
wire[63:0]	gprRegR21;
wire[63:0]	gprRegR22;
wire[63:0]	gprRegR23;
wire[63:0]	gprRegR24;
wire[63:0]	gprRegR25;
wire[63:0]	gprRegR26;
wire[63:0]	gprRegR27;
wire[63:0]	gprRegR28;
wire[63:0]	gprRegR29;
wire[63:0]	gprRegR30;

RegSpr_3W	gprModR18(
	clock,		reset,			JX2_GR_R18,	gprRegR18,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR18,	hold);
RegSpr_3W	gprModR19(
	clock,		reset,			JX2_GR_R19,	gprRegR19,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR19,	hold);
RegSpr_3W	gprModR20(
	clock,		reset,			JX2_GR_R20,	gprRegR20,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR20,	hold);
RegSpr_3W	gprModR21(
	clock,		reset,			JX2_GR_R21,	gprRegR21,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR21,	hold);
RegSpr_3W	gprModR22(
	clock,		reset,			JX2_GR_R22,	gprRegR22,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR22,	hold);
RegSpr_3W	gprModR23(
	clock,		reset,			JX2_GR_R23,	gprRegR23,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR23,	hold);

RegSpr_3W	gprModR24(
	clock,		reset,			JX2_GR_R24,	gprRegR24,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR24,	hold);
RegSpr_3W	gprModR25(
	clock,		reset,			JX2_GR_R25,	gprRegR25,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR25,	hold);
RegSpr_3W	gprModR26(
	clock,		reset,			JX2_GR_R26,	gprRegR26,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR26,	hold);
RegSpr_3W	gprModR27(
	clock,		reset,			JX2_GR_R27,	gprRegR27,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR27,	hold);
RegSpr_3W	gprModR28(
	clock,		reset,			JX2_GR_R28,	gprRegR28,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR28,	hold);
RegSpr_3W	gprModR29(
	clock,		reset,			JX2_GR_R29,	gprRegR29,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR29,	hold);
RegSpr_3W	gprModR30(
	clock,		reset,			JX2_GR_R30,	gprRegR30,
	regIdRnAW,	regValRnAW,		regIdRnBW,	regValRnBW,
	regIdRnCW,	regValRnCW,		gprRegR30,	hold);





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
	tValJimm={
		regValImmB[31:0],
		regValImmA[31:0] };
`endif

	casez(regIdRs)
		6'b000000:	tValRsA=gprRegDlr;
		6'b000001:	tValRsA=gprRegDhr;
		6'b000010:	tValRsA=gprRegR2;
		6'b000011:	tValRsA=gprRegR3;
		6'b000100:	tValRsA=gprRegR4;
		6'b000101:	tValRsA=gprRegR5;
		6'b000110:	tValRsA=gprRegR6;
		6'b000111:	tValRsA=gprRegR7;
		6'b001000:	tValRsA=gprRegR8;
		6'b001001:	tValRsA=gprRegR9;
		6'b001010:	tValRsA=gprRegR10;
		6'b001011:	tValRsA=gprRegR11;
		6'b001100:	tValRsA=gprRegR12;
		6'b001101:	tValRsA=gprRegR13;
		6'b001110:	tValRsA=gprRegR14;
		6'b001111:	tValRsA=gprRegSp;
		6'b010000:	tValRsA=gprRegElr;
		6'b010001:	tValRsA=gprRegEhr;
		6'b010010:	tValRsA=gprRegR18;
		6'b010011:	tValRsA=gprRegR19;
		6'b010100:	tValRsA=gprRegR20;
		6'b010101:	tValRsA=gprRegR21;
		6'b010110:	tValRsA=gprRegR22;
		6'b010111:	tValRsA=gprRegR23;
		6'b011000:	tValRsA=gprRegR24;
		6'b011001:	tValRsA=gprRegR25;
		6'b011010:	tValRsA=gprRegR26;
		6'b011011:	tValRsA=gprRegR27;
		6'b011100:	tValRsA=gprRegR28;
		6'b011101:	tValRsA=gprRegR29;
		6'b011110:	tValRsA=gprRegR30;
		6'b011111:	tValRsA=gprRegBp;

		JX2_GR_DLR:	tValRsA=gprRegDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
		JX2_GR_SP:	tValRsA=gprRegSp;

		JX2_GR_ELR:	tValRsA=gprRegElr;
		JX2_GR_EHR:	tValRsA=gprRegEhr;
		JX2_GR_BP:	tValRsA=gprRegBp;

`ifdef jx2_enable_vaddr48
		JX2_GR_PC:	tValRsA={ UV16_00, regValPc };
		JX2_GR_GBR:	tValRsA={ UV16_00, regValGbr };
		JX2_GR_LR:	tValRsA={ UV16_00, regValLr };
`else
		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:0] };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr[31:0] };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr[31:0] };
`endif

`ifdef jx2_gprs_mergecm
		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, JX2_GR_SSP,
		JX2_GR_TBR, JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
			tValRsA = regValCm;
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
`endif

		default: 	tValRsA=UV64_XX;
	endcase

	casez(regIdRt)
		6'b000000:	tValRtA=gprRegDlr;
		6'b000001:	tValRtA=gprRegDhr;
		6'b000010:	tValRtA=gprRegR2;
		6'b000011:	tValRtA=gprRegR3;
		6'b000100:	tValRtA=gprRegR4;
		6'b000101:	tValRtA=gprRegR5;
		6'b000110:	tValRtA=gprRegR6;
		6'b000111:	tValRtA=gprRegR7;
		6'b001000:	tValRtA=gprRegR8;
		6'b001001:	tValRtA=gprRegR9;
		6'b001010:	tValRtA=gprRegR10;
		6'b001011:	tValRtA=gprRegR11;
		6'b001100:	tValRtA=gprRegR12;
		6'b001101:	tValRtA=gprRegR13;
		6'b001110:	tValRtA=gprRegR14;
		6'b001111:	tValRtA=gprRegSp;
		6'b010000:	tValRtA=gprRegElr;
		6'b010001:	tValRtA=gprRegEhr;
		6'b010010:	tValRtA=gprRegR18;
		6'b010011:	tValRtA=gprRegR19;
		6'b010100:	tValRtA=gprRegR20;
		6'b010101:	tValRtA=gprRegR21;
		6'b010110:	tValRtA=gprRegR22;
		6'b010111:	tValRtA=gprRegR23;
		6'b011000:	tValRtA=gprRegR24;
		6'b011001:	tValRtA=gprRegR25;
		6'b011010:	tValRtA=gprRegR26;
		6'b011011:	tValRtA=gprRegR27;
		6'b011100:	tValRtA=gprRegR28;
		6'b011101:	tValRtA=gprRegR29;
		6'b011110:	tValRtA=gprRegR30;
		6'b011111:	tValRtA=gprRegBp;

		JX2_GR_DLR:	tValRtA=gprRegDlr;
		JX2_GR_DHR:	tValRtA=gprRegDhr;
		JX2_GR_SP:	tValRtA=gprRegSp;

		JX2_GR_ELR:	tValRtA=gprRegElr;
		JX2_GR_EHR:	tValRtA=gprRegEhr;
		JX2_GR_BP:	tValRtA=gprRegBp;

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
`endif

		default: 	tValRtA=UV64_XX;
	endcase

	casez(regIdRu)
		6'b000000:	tValRuA=gprRegDlr;
		6'b000001:	tValRuA=gprRegDhr;
		6'b000010:	tValRuA=gprRegR2;
		6'b000011:	tValRuA=gprRegR3;
		6'b000100:	tValRuA=gprRegR4;
		6'b000101:	tValRuA=gprRegR5;
		6'b000110:	tValRuA=gprRegR6;
		6'b000111:	tValRuA=gprRegR7;
		6'b001000:	tValRuA=gprRegR8;
		6'b001001:	tValRuA=gprRegR9;
		6'b001010:	tValRuA=gprRegR10;
		6'b001011:	tValRuA=gprRegR11;
		6'b001100:	tValRuA=gprRegR12;
		6'b001101:	tValRuA=gprRegR13;
		6'b001110:	tValRuA=gprRegR14;
		6'b001111:	tValRuA=gprRegSp;
		6'b010000:	tValRuA=gprRegElr;
		6'b010001:	tValRuA=gprRegEhr;
		6'b010010:	tValRuA=gprRegR18;
		6'b010011:	tValRuA=gprRegR19;
		6'b010100:	tValRuA=gprRegR20;
		6'b010101:	tValRuA=gprRegR21;
		6'b010110:	tValRuA=gprRegR22;
		6'b010111:	tValRuA=gprRegR23;
		6'b011000:	tValRuA=gprRegR24;
		6'b011001:	tValRuA=gprRegR25;
		6'b011010:	tValRuA=gprRegR26;
		6'b011011:	tValRuA=gprRegR27;
		6'b011100:	tValRuA=gprRegR28;
		6'b011101:	tValRuA=gprRegR29;
		6'b011110:	tValRuA=gprRegR30;
		6'b011111:	tValRuA=gprRegBp;

		JX2_GR_DLR:	tValRuA=gprRegDlr;
		JX2_GR_DHR:	tValRuA=gprRegDhr;
		JX2_GR_SP:	tValRuA=gprRegSp;

		JX2_GR_ELR:	tValRuA=gprRegElr;
		JX2_GR_EHR:	tValRuA=gprRegEhr;
		JX2_GR_BP:	tValRuA=gprRegBp;

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
		6'b000000:	tValRvA=gprRegDlr;
		6'b000001:	tValRvA=gprRegDhr;
		6'b000010:	tValRvA=gprRegR2;
		6'b000011:	tValRvA=gprRegR3;
		6'b000100:	tValRvA=gprRegR4;
		6'b000101:	tValRvA=gprRegR5;
		6'b000110:	tValRvA=gprRegR6;
		6'b000111:	tValRvA=gprRegR7;
		6'b001000:	tValRvA=gprRegR8;
		6'b001001:	tValRvA=gprRegR9;
		6'b001010:	tValRvA=gprRegR10;
		6'b001011:	tValRvA=gprRegR11;
		6'b001100:	tValRvA=gprRegR12;
		6'b001101:	tValRvA=gprRegR13;
		6'b001110:	tValRvA=gprRegR14;
		6'b001111:	tValRvA=gprRegSp;
		6'b010000:	tValRvA=gprRegElr;
		6'b010001:	tValRvA=gprRegEhr;
		6'b010010:	tValRvA=gprRegR18;
		6'b010011:	tValRvA=gprRegR19;
		6'b010100:	tValRvA=gprRegR20;
		6'b010101:	tValRvA=gprRegR21;
		6'b010110:	tValRvA=gprRegR22;
		6'b010111:	tValRvA=gprRegR23;
		6'b011000:	tValRvA=gprRegR24;
		6'b011001:	tValRvA=gprRegR25;
		6'b011010:	tValRvA=gprRegR26;
		6'b011011:	tValRvA=gprRegR27;
		6'b011100:	tValRvA=gprRegR28;
		6'b011101:	tValRvA=gprRegR29;
		6'b011110:	tValRvA=gprRegR30;
		6'b011111:	tValRvA=gprRegBp;

		JX2_GR_DLR:	tValRvA=gprRegDlr;
		JX2_GR_DHR:	tValRvA=gprRegDhr;
		JX2_GR_SP:	tValRvA=gprRegSp;

		JX2_GR_ELR:	tValRvA=gprRegElr;
		JX2_GR_EHR:	tValRvA=gprRegEhr;
		JX2_GR_BP:	tValRvA=gprRegBp;

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
		6'b000000:	tValRxA=gprRegDlr;
		6'b000001:	tValRxA=gprRegDhr;
		6'b000010:	tValRxA=gprRegR2;
		6'b000011:	tValRxA=gprRegR3;
		6'b000100:	tValRxA=gprRegR4;
		6'b000101:	tValRxA=gprRegR5;
		6'b000110:	tValRxA=gprRegR6;
		6'b000111:	tValRxA=gprRegR7;
		6'b001000:	tValRxA=gprRegR8;
		6'b001001:	tValRxA=gprRegR9;
		6'b001010:	tValRxA=gprRegR10;
		6'b001011:	tValRxA=gprRegR11;
		6'b001100:	tValRxA=gprRegR12;
		6'b001101:	tValRxA=gprRegR13;
		6'b001110:	tValRxA=gprRegR14;
		6'b001111:	tValRxA=gprRegSp;
		6'b010000:	tValRxA=gprRegElr;
		6'b010001:	tValRxA=gprRegEhr;
		6'b010010:	tValRxA=gprRegR18;
		6'b010011:	tValRxA=gprRegR19;
		6'b010100:	tValRxA=gprRegR20;
		6'b010101:	tValRxA=gprRegR21;
		6'b010110:	tValRxA=gprRegR22;
		6'b010111:	tValRxA=gprRegR23;
		6'b011000:	tValRxA=gprRegR24;
		6'b011001:	tValRxA=gprRegR25;
		6'b011010:	tValRxA=gprRegR26;
		6'b011011:	tValRxA=gprRegR27;
		6'b011100:	tValRxA=gprRegR28;
		6'b011101:	tValRxA=gprRegR29;
		6'b011110:	tValRxA=gprRegR30;
		6'b011111:	tValRxA=gprRegBp;

		JX2_GR_DLR:	tValRxA=gprRegDlr;
		JX2_GR_DHR:	tValRxA=gprRegDhr;
		JX2_GR_SP:	tValRxA=gprRegSp;

		JX2_GR_ELR:	tValRxA=gprRegElr;
		JX2_GR_EHR:	tValRxA=gprRegEhr;
		JX2_GR_BP:	tValRxA=gprRegBp;

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
		6'b000000:	tValRyA=gprRegDlr;
		6'b000001:	tValRyA=gprRegDhr;
		6'b000010:	tValRyA=gprRegR2;
		6'b000011:	tValRyA=gprRegR3;
		6'b000100:	tValRyA=gprRegR4;
		6'b000101:	tValRyA=gprRegR5;
		6'b000110:	tValRyA=gprRegR6;
		6'b000111:	tValRyA=gprRegR7;
		6'b001000:	tValRyA=gprRegR8;
		6'b001001:	tValRyA=gprRegR9;
		6'b001010:	tValRyA=gprRegR10;
		6'b001011:	tValRyA=gprRegR11;
		6'b001100:	tValRyA=gprRegR12;
		6'b001101:	tValRyA=gprRegR13;
		6'b001110:	tValRyA=gprRegR14;
		6'b001111:	tValRyA=gprRegSp;
		6'b010000:	tValRyA=gprRegElr;
		6'b010001:	tValRyA=gprRegEhr;
		6'b010010:	tValRyA=gprRegR18;
		6'b010011:	tValRyA=gprRegR19;
		6'b010100:	tValRyA=gprRegR20;
		6'b010101:	tValRyA=gprRegR21;
		6'b010110:	tValRyA=gprRegR22;
		6'b010111:	tValRyA=gprRegR23;
		6'b011000:	tValRyA=gprRegR24;
		6'b011001:	tValRyA=gprRegR25;
		6'b011010:	tValRyA=gprRegR26;
		6'b011011:	tValRyA=gprRegR27;
		6'b011100:	tValRyA=gprRegR28;
		6'b011101:	tValRyA=gprRegR29;
		6'b011110:	tValRyA=gprRegR30;
		6'b011111:	tValRyA=gprRegBp;

		JX2_GR_DLR:	tValRyA=gprRegDlr;
		JX2_GR_DHR:	tValRyA=gprRegDhr;
		JX2_GR_SP:	tValRyA=gprRegSp;

		JX2_GR_ELR:	tValRyA=gprRegElr;
		JX2_GR_EHR:	tValRyA=gprRegEhr;
		JX2_GR_BP:	tValRyA=gprRegBp;

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
	
	if(!tValRsZz)
	begin
		if(regIdRs==regIdRnC3)
			tRegValRs=regValRnC3;
		if(regIdRs==regIdRnB3)
			tRegValRs=regValRnB3;
		if(regIdRs==regIdRnA3)
			tRegValRs=regValRnA3;

		if(regIdRs==regIdRnC2)
			tRegValRs=regValRnC2;
		if(regIdRs==regIdRnB2)
			tRegValRs=regValRnB2;
		if(regIdRs==regIdRnA2)
			tRegValRs=regValRnA2;

		if(regIdRs==regIdRnC1)
			tRegValRs=regValRnC1;
		if(regIdRs==regIdRnB1)
			tRegValRs=regValRnB1;
		if(regIdRs==regIdRnA1)
			tRegValRs=regValRnA1;
	end

	if(!tValRtZz)
	begin
		if(regIdRt==regIdRnC3)
			tRegValRt=regValRnC3;
		if(regIdRt==regIdRnB3)
			tRegValRt=regValRnB3;
		if(regIdRt==regIdRnA3)
			tRegValRt=regValRnA3;

		if(regIdRt==regIdRnC2)
			tRegValRt=regValRnC2;
		if(regIdRt==regIdRnB2)
			tRegValRt=regValRnB2;
		if(regIdRt==regIdRnA2)
			tRegValRt=regValRnA2;

		if(regIdRt==regIdRnC1)
			tRegValRt=regValRnC1;
		if(regIdRt==regIdRnB1)
			tRegValRt=regValRnB1;
		if(regIdRt==regIdRnA1)
			tRegValRt=regValRnA1;
	end

	if(!tValRuZz)
	begin
		if(regIdRu==regIdRnC3)
			tRegValRu=regValRnC3;
		if(regIdRu==regIdRnB3)
			tRegValRu=regValRnB3;
		if(regIdRu==regIdRnA3)
			tRegValRu=regValRnA3;

		if(regIdRu==regIdRnC2)
			tRegValRu=regValRnC2;
		if(regIdRu==regIdRnB2)
			tRegValRu=regValRnB2;
		if(regIdRu==regIdRnA2)
			tRegValRu=regValRnA2;

		if(regIdRu==regIdRnC1)
			tRegValRu=regValRnC1;
		if(regIdRu==regIdRnB1)
			tRegValRu=regValRnB1;
		if(regIdRu==regIdRnA1)
			tRegValRu=regValRnA1;
	end

	if(!tValRvZz)
	begin
		if(regIdRv==regIdRnC3)
			tRegValRv=regValRnC3;
		if(regIdRv==regIdRnB3)
			tRegValRv=regValRnB3;
		if(regIdRv==regIdRnA3)
			tRegValRv=regValRnA3;

		if(regIdRv==regIdRnC2)
			tRegValRv=regValRnC2;
		if(regIdRv==regIdRnB2)
			tRegValRv=regValRnB2;
		if(regIdRv==regIdRnA2)
			tRegValRv=regValRnA2;

		if(regIdRv==regIdRnC1)
			tRegValRv=regValRnC1;
		if(regIdRv==regIdRnB1)
			tRegValRv=regValRnB1;
		if(regIdRv==regIdRnA1)
			tRegValRv=regValRnA1;
	end


	if(!tValRxZz)
	begin
		if(regIdRx==regIdRnC3)
			tRegValRx=regValRnC3;
		if(regIdRx==regIdRnB3)
			tRegValRx=regValRnB3;
		if(regIdRx==regIdRnA3)
			tRegValRx=regValRnA3;

		if(regIdRx==regIdRnC2)
			tRegValRx=regValRnC2;
		if(regIdRx==regIdRnB2)
			tRegValRx=regValRnB2;
		if(regIdRx==regIdRnA2)
			tRegValRx=regValRnA2;

		if(regIdRx==regIdRnC1)
			tRegValRx=regValRnC1;
		if(regIdRx==regIdRnB1)
			tRegValRx=regValRnB1;
		if(regIdRx==regIdRnA1)
			tRegValRx=regValRnA1;
	end

	if(!tValRyZz)
	begin
		if(regIdRy==regIdRnC3)
			tRegValRy=regValRnC3;
		if(regIdRy==regIdRnB3)
			tRegValRy=regValRnB3;
		if(regIdRy==regIdRnA3)
			tRegValRy=regValRnA3;

		if(regIdRy==regIdRnC2)
			tRegValRy=regValRnC2;
		if(regIdRy==regIdRnB2)
			tRegValRy=regValRnB2;
		if(regIdRy==regIdRnA2)
			tRegValRy=regValRnA2;

		if(regIdRy==regIdRnC1)
			tRegValRy=regValRnC1;
		if(regIdRy==regIdRnB1)
			tRegValRy=regValRnB1;
		if(regIdRy==regIdRnA1)
			tRegValRy=regValRnA1;
	end
end

always @(posedge clock)
begin
//	if(!hold)
//	begin
//	end
end

endmodule

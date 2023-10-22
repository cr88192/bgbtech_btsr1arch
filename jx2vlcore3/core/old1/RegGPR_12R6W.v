/*
GPR Module with 12 read ports and 6 write ports.

This is intended for an SMT core.
 */

`include "CoreDefs.v"

`include "RegSpr_6W.v"
`include "RegFW_E3R6W.v"
`include "RegArrSel_6W.v"
`include "RegPrtSelRs.v"
`include "RegPrtSelRt.v"
`include "RegPrtSelRuv.v"

module RegGPR_12R6W(
	clock,
	reset,
	exHold1,
	exHold2,

	regIdUCmd1,
	regIdUIxt1,
	regIdUCmd2,
	regIdUIxt2,

	regIdR1s,		//Source A, ALU / Base (Lane 1)
	regIdR1t,		//Source B, ALU / Index (Lane 1)
	regIdR1u,		//Source C, ALU / Base (Lane 2)
	regIdR1v,		//Source D, ALU / Index (Lane 2) / MemStore
	regIdR1x,		//Source E, ALU / Base (Lane 3)
	regIdR1y,		//Source F, ALU / Index (Lane 3) / MemStore

	regIdR2s,		//Source A, ALU / Base (Lane 1)
	regIdR2t,		//Source B, ALU / Index (Lane 1)
	regIdR2u,		//Source C, ALU / Base (Lane 2)
	regIdR2v,		//Source D, ALU / Index (Lane 2) / MemStore
	regIdR2x,		//Source E, ALU / Base (Lane 3)
	regIdR2y,		//Source F, ALU / Index (Lane 3) / MemStore

	regValR1s,		//Source A Value
	regValR1t,		//Source B Value
	regValR1u,		//Source C Value
	regValR1v,		//Source D Value
	regValR1x,		//Source E Value
	regValR1y,		//Source F Value

	regValR2s,		//Source A Value
	regValR2t,		//Source B Value
	regValR2u,		//Source C Value
	regValR2v,		//Source D Value
	regValR2x,		//Source E Value
	regValR2y,		//Source F Value

	regIdR1nA1,		//Destination ID (EX1, Lane 1)
	regValR1nA1,	//Destination Value (EX1, Lane 1)
	regIdR1nB1,		//Destination ID (EX1, Lane 2)
	regValR1nB1,	//Destination Value (EX1, Lane 2)
	regIdR1nC1,		//Destination ID (EX1, Lane 3)
	regValR1nC1,	//Destination Value (EX1, Lane 3)

	regIdR2nA1,		//Destination ID (EX1, Lane 1)
	regValR2nA1,	//Destination Value (EX1, Lane 1)
	regIdR2nB1,		//Destination ID (EX1, Lane 2)
	regValR2nB1,	//Destination Value (EX1, Lane 2)
	regIdR2nC1,		//Destination ID (EX1, Lane 3)
	regValR2nC1,	//Destination Value (EX1, Lane 3)

	regIdR1nA2,		//Destination ID (EX2, Lane 1)
	regValR1nA2,	//Destination Value (EX2, Lane 1)	
	regIdR1nB2,		//Destination ID (EX2, Lane 2)
	regValR1nB2,	//Destination Value (EX2, Lane 2)
	regIdR1nC2,		//Destination ID (EX2, Lane 3)
	regValR1nC2,	//Destination Value (EX2, Lane 3)

	regIdR2nA2,		//Destination ID (EX2, Lane 1)
	regValR2nA2,	//Destination Value (EX2, Lane 1)	
	regIdR2nB2,		//Destination ID (EX2, Lane 2)
	regValR2nB2,	//Destination Value (EX2, Lane 2)
	regIdR2nC2,		//Destination ID (EX2, Lane 3)
	regValR2nC2,	//Destination Value (EX2, Lane 3)

	regIdR1nA3,		//Destination ID (EX2, Lane 1)
	regValR1nA3,	//Destination Value (EX2, Lane 1)
	regIdR1nB3,		//Destination ID (EX2, Lane 2)
	regValR1nB3,	//Destination Value (EX2, Lane 2)
	regIdR1nC3,		//Destination ID (EX2, Lane 3)
	regValR1nC3,	//Destination Value (EX2, Lane 3)
	
	regIdR2nA3,		//Destination ID (EX2, Lane 1)
	regValR2nA3,	//Destination Value (EX2, Lane 1)
	regIdR2nB3,		//Destination ID (EX2, Lane 2)
	regValR2nB3,	//Destination Value (EX2, Lane 2)
	regIdR2nC3,		//Destination ID (EX2, Lane 3)
	regValR2nC3,	//Destination Value (EX2, Lane 3)
	
	regVal1Pc,		//PC Value (Synthesized)
	regVal1Gbr,		//GBR Value (CR)
	regVal1ImmA,	//Immediate (Decode, Lane 1)
	regVal1ImmB,	//Immediate (Decode, Lane 2)
	regVal1ImmC,	//Immediate (Decode, Lane 3)
	regVal1Lr,		//LR Value (CR)
	regVal1Cm,		//Cm Value (CR)
	
	regVal2Pc,		//PC Value (Synthesized)
	regVal2Gbr,		//GBR Value (CR)
	regVal2ImmA,	//Immediate (Decode, Lane 1)
	regVal2ImmB,	//Immediate (Decode, Lane 2)
	regVal2ImmC,	//Immediate (Decode, Lane 3)
	regVal2Lr,		//LR Value (CR)
	regVal2Cm,		//Cm Value (CR)
	
	gprEx1Flush1,
	gprEx2Flush1,
	gprEx3Flush1,

	gprEx1Flush2,
	gprEx2Flush2,
	gprEx3Flush2,

	regOutDlr1,	regInDlr1,
	regOutDhr1,	regInDhr1,
	regOutSp1,	regInSp1,

	regOutDlr2,	regInDlr2,
	regOutDhr2,	regInDhr2,
	regOutSp2,	regInSp2
	);


input			clock;
input			reset;

input			exHold1;
input			exHold2;

input[8:0]		regIdUCmd1;
input[8:0]		regIdUIxt1;

input[8:0]		regIdUCmd2;
input[8:0]		regIdUIxt2;

`input_gpr		regIdR1s;		//Source A
`input_gpr		regIdR1t;		//Source B
`input_gpr		regIdR1u;		//Source C
`input_gpr		regIdR1v;		//Source D
`input_gpr		regIdR1x;		//Source E
`input_gpr		regIdR1y;		//Source F

`input_gpr		regIdR2s;		//Source A
`input_gpr		regIdR2t;		//Source B
`input_gpr		regIdR2u;		//Source C
`input_gpr		regIdR2v;		//Source D
`input_gpr		regIdR2x;		//Source E
`input_gpr		regIdR2y;		//Source F

output[63:0]	regValR1s;
output[63:0]	regValR1t;
output[63:0]	regValR1u;
output[63:0]	regValR1v;
output[63:0]	regValR1x;
output[63:0]	regValR1y;

output[63:0]	regValR2s;
output[63:0]	regValR2t;
output[63:0]	regValR2u;
output[63:0]	regValR2v;
output[63:0]	regValR2x;
output[63:0]	regValR2y;

`input_gpr		regIdR1nA1;		//Destination ID
input[63:0]		regValR1nA1;	//Destination Value
`input_gpr		regIdR1nA2;		//Destination ID
input[63:0]		regValR1nA2;	//Destination Value
`input_gpr		regIdR1nA3;		//Destination ID
input[63:0]		regValR1nA3;	//Destination Value

`input_gpr		regIdR2nA1;		//Destination ID
input[63:0]		regValR2nA1;	//Destination Value
`input_gpr		regIdR2nA2;		//Destination ID
input[63:0]		regValR2nA2;	//Destination Value
`input_gpr		regIdR2nA3;		//Destination ID
input[63:0]		regValR2nA3;	//Destination Value


`input_gpr		regIdR1nB1;		//Destination ID
input[63:0]		regValR1nB1;	//Destination Value
`input_gpr		regIdR1nB2;		//Destination ID
input[63:0]		regValR1nB2;	//Destination Value
`input_gpr		regIdR1nB3;		//Destination ID
input[63:0]		regValR1nB3;	//Destination Value

`input_gpr		regIdR2nB1;		//Destination ID
input[63:0]		regValR2nB1;	//Destination Value
`input_gpr		regIdR2nB2;		//Destination ID
input[63:0]		regValR2nB2;	//Destination Value
`input_gpr		regIdR2nB3;		//Destination ID
input[63:0]		regValR2nB3;	//Destination Value


`input_gpr		regIdR1nC1;		//Destination ID
input[63:0]		regValR1nC1;	//Destination Value
`input_gpr		regIdR1nC2;		//Destination ID
input[63:0]		regValR1nC2;	//Destination Value
`input_gpr		regIdR1nC3;		//Destination ID
input[63:0]		regValR1nC3;	//Destination Value

`input_gpr		regIdR2nC1;		//Destination ID
input[63:0]		regValR2nC1;	//Destination Value
`input_gpr		regIdR2nC2;		//Destination ID
input[63:0]		regValR2nC2;	//Destination Value
`input_gpr		regIdR2nC3;		//Destination ID
input[63:0]		regValR2nC3;	//Destination Value


input [47:0]	regVal1Pc;		//PC Value (Synthesized)
input [47:0]	regVal1Gbr;		//GBR Value (CR)
input [32:0]	regVal1ImmA;	//Immediate (Decode)
input [32:0]	regVal1ImmB;	//Immediate (Decode)
input [32:0]	regVal1ImmC;	//Immediate (Decode)
input [63:0]	regVal1Lr;		//LR Value (CR)
input [63:0]	regVal1Cm;		//Cm Value (CR)

input [47:0]	regVal2Pc;		//PC Value (Synthesized)
input [47:0]	regVal2Gbr;		//GBR Value (CR)
input [32:0]	regVal2ImmA;	//Immediate (Decode)
input [32:0]	regVal2ImmB;	//Immediate (Decode)
input [32:0]	regVal2ImmC;	//Immediate (Decode)
input [63:0]	regVal2Lr;		//LR Value (CR)
input [63:0]	regVal2Cm;		//Cm Value (CR)

input			gprEx1Flush1;
input			gprEx2Flush1;
input			gprEx3Flush1;

input			gprEx1Flush2;
input			gprEx2Flush2;
input			gprEx3Flush2;


output[63:0]	regOutDlr1;
input [63:0]	regInDlr1;
output[63:0]	regOutDhr1;
input [63:0]	regInDhr1;
output[63:0]	regOutSp1;
input [63:0]	regInSp1;

output[63:0]	regOutDlr2;
input [63:0]	regInDlr2;
output[63:0]	regOutDhr2;
input [63:0]	regInDhr2;
output[63:0]	regOutSp2;
input [63:0]	regInSp2;


reg[63:0]	tRegValR1s;
reg[63:0]	tRegValR1t;
reg[63:0]	tRegValR1u;
reg[63:0]	tRegValR1v;
reg[63:0]	tRegValR1x;
reg[63:0]	tRegValR1y;

reg[63:0]	tRegValR2s;
reg[63:0]	tRegValR2t;
reg[63:0]	tRegValR2u;
reg[63:0]	tRegValR2v;
reg[63:0]	tRegValR2x;
reg[63:0]	tRegValR2y;

assign	regValR1s = tRegValR1s;
assign	regValR1t = tRegValR1t;
assign	regValR1u = tRegValR1u;
assign	regValR1v = tRegValR1v;
assign	regValR1x = tRegValR1x;
assign	regValR1y = tRegValR1y;

assign	regValR2s = tRegValR2s;
assign	regValR2t = tRegValR2t;
assign	regValR2u = tRegValR2u;
assign	regValR2v = tRegValR2v;
assign	regValR2x = tRegValR2x;
assign	regValR2y = tRegValR2y;


`wire_gpr		regIdR1nAW;		//Destination ID
wire[63:0]		regValR1nAW;	//Destination Value
`wire_gpr		regIdR1nBW;		//Destination ID
wire[63:0]		regValR1nBW;	//Destination Value
`wire_gpr		regIdR1nCW;		//Destination ID
wire[63:0]		regValR1nCW;	//Destination Value
wire			regFlushR1nW;	//Flush Stage

assign	regIdR1nAW		= regIdR1nA3;
assign	regValR1nAW		= regValR1nA3;
assign	regIdR1nBW		= regIdR1nB3;
assign	regValR1nBW		= regValR1nB3;
assign	regIdR1nCW		= regIdR1nC3;
assign	regValR1nCW		= regValR1nC3;
assign	regFlushR1nW	= gprEx3Flush1;


`wire_gpr		regIdR2nAW;		//Destination ID
wire[63:0]		regValR2nAW;	//Destination Value
`wire_gpr		regIdR2nBW;		//Destination ID
wire[63:0]		regValR2nBW;	//Destination Value
`wire_gpr		regIdR2nCW;		//Destination ID
wire[63:0]		regValR2nCW;	//Destination Value
wire			regFlushR2nW;	//Flush Stage

assign	regIdR2nAW		= regIdR2nA3;
assign	regValR2nAW		= regValR2nA3;
assign	regIdR2nBW		= regIdR2nB3;
assign	regValR2nBW		= regValR2nB3;
assign	regIdR2nCW		= regIdR2nC3;
assign	regValR2nCW		= regValR2nC3;
assign	regFlushR2nW	= gprEx3Flush2;

reg[63:0]	gprArrA[63:0];
reg[63:0]	gprArrB[63:0];
reg[63:0]	gprArrC[63:0];
reg[63:0]	gprArrD[63:0];
reg[63:0]	gprArrE[63:0];
reg[63:0]	gprArrF[63:0];
reg[63:0]	gprArrMA;
reg[63:0]	gprArrMB;
reg[63:0]	gprArrMC;

wire[63:0]	gprRegDlr1;
wire[63:0]	gprRegDhr1;
wire[63:0]	gprRegSp1;

wire[63:0]	gprRegDlr2;
wire[63:0]	gprRegDhr2;
wire[63:0]	gprRegSp2;

RegSpr_6W	gprModDlr1(
	clock,			reset,
	JX2_GR_DLR,		gprRegDlr1,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInDlr1,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);

RegSpr_6W	gprModDhr1(
	clock,			reset,
	JX2_GR_DHR,		gprRegDhr1,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInDhr1,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);

RegSpr_6W	gprModSp1(
	clock,			reset,
	JX2_GR_SP,		gprRegSp1,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInSp1,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);


RegSpr_6W	gprModDlr2(
	clock,			reset,
	JX2_GR_DLR2,	gprRegDlr2,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInDlr2,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);

RegSpr_6W	gprModDhr2(
	clock,			reset,
	JX2_GR_DHR2,	gprRegDhr2,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInDhr2,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);

RegSpr_6W	gprModSp2(
	clock,			reset,
	JX2_GR_SP2,		gprRegSp2,
	regIdR1nAW,		regValR1nAW,
	regIdR1nBW,		regValR1nBW,
	regIdR1nCW,		regValR1nCW,
	regIdR2nAW,		regValR2nAW,
	regIdR2nBW,		regValR2nBW,
	regIdR2nCW,		regValR2nCW,
	regInSp2,
	exHold1,		exHold2,
	regFlushR1nW,	regFlushR2nW);


assign	regOutDlr1 = gprRegDlr1;
assign	regOutDhr1 = gprRegDhr1;
assign	regOutSp1  = gprRegSp1;

assign	regOutDlr2 = gprRegDlr2;
assign	regOutDhr2 = gprRegDhr2;
assign	regOutSp2  = gprRegSp2;


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

reg[63:0]	tValJimm1;
reg[63:0]	tValJimm2;

reg		tRegEx1NoForward;
reg		tRegEx2NoForward;
reg		tRegEx3NoForward;

wire	tEnablePcLsb;
`ifdef jx2_enable_ops24
assign	tEnablePcLsb = (regIdUIxt[2:0] == JX2_BTY_SB);
`else
assign	tEnablePcLsb = 0;
`endif


wire[63:0]	tValR1sA0;
RegArrSel_6W	selRegR1s(
	clock,		reset,		regIdR1s,	tValR1sA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1sA;
RegPrtSelRs			selPrtR1s(
	clock,			reset,
	regIdR1s,		tValR1sA0,		tValR1sA,
	regVal1Pc,		regVal1Gbr,
	regVal1ImmA,	regVal1ImmB,
	regVal1Lr,		regVal1Cm,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);
	

wire[63:0]	tValR1tA0;
RegArrSel_6W	selRegR1t(
	clock,		reset,		regIdR1t,	tValR1tA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1tA;
RegPrtSelRt			selPrtR1t(
	clock,			reset,
	regIdR1t,		tValR1tA0,		tValR1tA,
	regVal1Pc,		regVal1Gbr,
	regVal1ImmA,	regVal1ImmB,
	regVal1Lr,		regVal1Cm,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);

wire[63:0]	tValR1uA0;
RegArrSel_6W	selRegR1u(
	clock,		reset,		regIdR1u,	tValR1uA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1vA0;
RegArrSel_6W	selRegR1v(
	clock,		reset,		regIdR1v,	tValR1vA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1uA;
wire[63:0]	tValR1vA;

RegPrtSelRuv		selPrtR1u(
	clock,			reset,
	regIdR1u,		tValR1uA0,		tValR1uA,
	regVal1ImmB,	regVal1ImmC,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);
RegPrtSelRuv		selPrtR1v(
	clock,			reset,
	regIdR1v,		tValR1vA0,		tValR1vA,
	regVal1ImmB,	regVal1ImmC,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);

wire[63:0]	tValR1xA0;
RegArrSel_6W	selRegR1x(
	clock,		reset,		regIdR1x,	tValR1xA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1yA0;
RegArrSel_6W	selRegR1y(
	clock,		reset,		regIdR1y,	tValR1yA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR1xA;
wire[63:0]	tValR1yA;

RegPrtSelRuv		selPrtR1x(
	clock,			reset,
	regIdR1x,		tValR1xA0,		tValR1xA,
	regVal1ImmC,	UV33_00,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);
RegPrtSelRuv		selPrtR1y(
	clock,			reset,
	regIdR1y,		tValR1yA0,		tValR1yA,
	regVal1ImmC,	UV33_00,
	gprRegDlr1,		gprRegDhr1,
	gprRegSp1
	);



wire[63:0]	tValR2sA0;
RegArrSel_6W	selRegR2s(
	clock,		reset,		regIdR2s,	tValR2sA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2sA;
RegPrtSelRs			selPrtR2s(
	clock,			reset,
	regIdR2s,		tValR2sA0,		tValR2sA,
	regVal2Pc,		regVal2Gbr,
	regVal2ImmA,	regVal2ImmB,
	regVal2Lr,		regVal2Cm,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);

wire[63:0]	tValR2tA0;
RegArrSel_6W	selRegR2t(
	clock,		reset,		regIdR2t,	tValR2tA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2tA;
RegPrtSelRt			selPrtR2t(
	clock,			reset,
	regIdR2t,		tValR2tA0,		tValR2tA,
	regVal2Pc,		regVal2Gbr,
	regVal2ImmA,	regVal2ImmB,
	regVal2Lr,		regVal2Cm,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);

wire[63:0]	tValR2uA0;
RegArrSel_6W	selRegR2u(
	clock,		reset,		regIdR2u,	tValR2uA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2vA0;
RegArrSel_6W	selRegR2v(
	clock,		reset,		regIdR2v,	tValR2vA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2uA;
wire[63:0]	tValR2vA;

RegPrtSelRuv		selPrtR2u(
	clock,			reset,
	regIdR2u,		tValR2uA0,		tValR2uA,
	regVal2ImmB,	regVal2ImmC,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);
RegPrtSelRuv		selPrtR2v(
	clock,			reset,
	regIdR2v,		tValR2vA0,		tValR2vA,
	regVal2ImmB,	regVal2ImmC,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);

wire[63:0]	tValR2xA0;
RegArrSel_6W	selRegR2x(
	clock,		reset,		regIdR2x,	tValR2xA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2yA0;
RegArrSel_6W	selRegR2y(
	clock,		reset,		regIdR2y,	tValR2yA0,
	gprArrA,	gprArrB,	gprArrC,	gprArrD,
	gprArrE,	gprArrF,	gprArrMA,	gprArrMB,
	gprArrMC);

wire[63:0]	tValR2xA;
wire[63:0]	tValR2yA;

RegPrtSelRuv		selPrtR2x(
	clock,			reset,
	regIdR2x,		tValR2xA0,		tValR2xA,
	regVal2ImmC,	UV33_00,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);
RegPrtSelRuv		selPrtR2y(
	clock,			reset,
	regIdR2y,		tValR2yA0,		tValR2yA,
	regVal2ImmC,	UV33_00,
	gprRegDlr2,		gprRegDhr2,
	gprRegSp2
	);



wire[63:0]	tValR1sB;
wire		tValR1sH;
RegFW_E3R6W		fwRegR1s(
	clock,			reset,			exHold1,		exHold2,
	regIdR1s,		tValR1sA,		tValR1sB,		tValR1sH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR1tB;
wire		tValR1tH;
RegFW_E3R6W		fwRegR1t(
	clock,			reset,			exHold1,		exHold2,
	regIdR1t,		tValR1tA,		tValR1tB,		tValR1tH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR1uB;
wire		tValR1uH;
RegFW_E3R6W		fwRegR1u(
	clock,			reset,			exHold1,		exHold2,
	regIdR1u,		tValR1uA,		tValR1uB,		tValR1uH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR1vB;
wire		tValR1vH;
RegFW_E3R6W		fwRegR1v(
	clock,			reset,			exHold1,		exHold2,
	regIdR1v,		tValR1vA,		tValR1vB,		tValR1vH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR1xB;
wire		tValR1xH;
RegFW_E3R6W		fwRegR1x(
	clock,			reset,			exHold1,		exHold2,
	regIdR1x,		tValR1xA,		tValR1xB,		tValR1xH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR1yB;
wire		tValR1yH;
RegFW_E3R6W		fwRegR1y(
	clock,			reset,			exHold1,		exHold2,
	regIdR1y,		tValR1yA,		tValR1yB,		tValR1yH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);




wire[63:0]	tValR2sB;
wire		tValR2sH;
RegFW_E3R6W		fwRegR2s(
	clock,			reset,			exHold1,		exHold2,
	regIdR2s,		tValR2sA,		tValR2sB,		tValR2sH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR2tB;
wire		tValR2tH;
RegFW_E3R6W		fwRegR2t(
	clock,			reset,			exHold1,		exHold2,
	regIdR2t,		tValR2tA,		tValR2tB,		tValR2tH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR2uB;
wire		tValR2uH;
RegFW_E3R6W		fwRegR2u(
	clock,			reset,			exHold1,		exHold2,
	regIdR2u,		tValR2uA,		tValR2uB,		tValR2uH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR2vB;
wire		tValR2vH;
RegFW_E3R6W		fwRegR2v(
	clock,			reset,			exHold1,		exHold2,
	regIdR2v,		tValR2vA,		tValR2vB,		tValR2vH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR2xB;
wire		tValR2xH;
RegFW_E3R6W		fwRegR2x(
	clock,			reset,			exHold1,		exHold2,
	regIdR2x,		tValR2xA,		tValR2xB,		tValR2xH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);

wire[63:0]	tValR2yB;
wire		tValR2yH;
RegFW_E3R6W		fwRegR2y(
	clock,			reset,			exHold1,		exHold2,
	regIdR2y,		tValR2yA,		tValR2yB,		tValR2yH,
	regIdR1nA1,		regValR1nA1,	regIdR1nB1,		regValR1nB1,
	regIdR1nC1,		regValR1nC1,	regIdR2nA1,		regValR2nA1,
	regIdR2nB1,		regValR2nB1,	regIdR2nC1,		regValR2nC1,
	regIdR1nA2,		regValR1nA2,	regIdR1nB2,		regValR1nB2,
	regIdR1nC2,		regValR1nC2,	regIdR2nA2,		regValR2nA2,
	regIdR2nB2,		regValR2nB2,	regIdR2nC2,		regValR2nC2,
	regIdR1nA3,		regValR1nA3,	regIdR1nB3,		regValR1nB3,
	regIdR1nC3,		regValR1nC3,	regIdR2nA3,		regValR2nA3,
	regIdR2nB3,		regValR2nB3,	regIdR2nC3,		regValR2nC3,
	gprEx1Flush1,	gprEx1Flush2,	gprEx2Flush1,	gprEx2Flush2,
	gprEx3Flush1,	gprEx3Flush2
	);


always @*
begin
	tRegValR1s	= tValR1sB;
	tRegValR1t	= tValR1tB;
	tRegValR1u	= tValR1uB;
	tRegValR1v	= tValR1vB;
	tRegValR1x	= tValR1xB;
	tRegValR1y	= tValR1yB;
	tRegValR2s	= tValR2sB;
	tRegValR2t	= tValR2tB;
	tRegValR2u	= tValR2uB;
	tRegValR2v	= tValR2vB;
	tRegValR2x	= tValR2xB;
	tRegValR2y	= tValR2yB;
end

always @(posedge clock)
begin
	if(!exHold1 && !regFlushR1nW)
	begin
		if(!regIdR1nAW[6])
		begin
			gprArrA[regIdR1nAW[5:0]]	<= regValR1nAW;
			gprArrMA[regIdR1nAW[5:0]]	<= 1'b1;
			gprArrMB[regIdR1nAW[5:0]]	<= 1'b0;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b0;
		end

		if(!regIdR1nBW[6])
		begin
			gprArrB[regIdR1nBW[5:0]]	<= regValR1nBW;
			gprArrMA[regIdR1nBW[5:0]]	<= 1'b0;
			gprArrMB[regIdR1nBW[5:0]]	<= 1'b1;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b0;
		end

		if(!regIdR1nCW[6])
		begin
			gprArrC[regIdR1nCW[5:0]]	<= regValR1nCW;
			gprArrMA[regIdR1nCW[5:0]]	<= 1'b1;
			gprArrMB[regIdR1nCW[5:0]]	<= 1'b1;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b0;
		end
	end

	if(!exHold2 && !regFlushR2nW)
	begin
		if(!regIdR2nAW[6])
		begin
			gprArrD[regIdR2nAW[5:0]]	<= regValR2nAW;
			gprArrMA[regIdR2nAW[5:0]]	<= 1'b1;
			gprArrMB[regIdR2nAW[5:0]]	<= 1'b0;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b1;
		end

		if(!regIdR1nBW[6])
		begin
			gprArrE[regIdR2nBW[5:0]]	<= regValR2nBW;
			gprArrMA[regIdR2nBW[5:0]]	<= 1'b0;
			gprArrMB[regIdR2nBW[5:0]]	<= 1'b1;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b1;
		end

		if(!regIdR2nCW[6])
		begin
			gprArrF[regIdR2nCW[5:0]]	<= regValR2nCW;
			gprArrMA[regIdR2nCW[5:0]]	<= 1'b1;
			gprArrMB[regIdR2nCW[5:0]]	<= 1'b1;
			gprArrMC[regIdR1nAW[5:0]]	<= 1'b1;
		end
	end
end

endmodule

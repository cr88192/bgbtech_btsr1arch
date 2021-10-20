/*
GPR Module with 12 read ports and 6 write ports.
Combine Both the GPR and CR interfaces into a single module.

This is intended for an SMT core.
 */

`include "CoreDefs.v"
`include "RegGPR_6R3W.v"
`include "RegCR.v"

module RegGCR_12R6W(
	clock,
	reset,

	rg1ExHold,
	rg2ExHold,

	rg2IdUCmd,
	rg2IdUIxt,
	rg2IdUCmd,
	rg2IdUIxt,

	rg1IdRs,	rg1IdRt,
	rg1ValRs,	rg1ValRt,
	rg1IdRu,	rg1IdRv,
	rg1ValRu,	rg1ValRv,
	rg1IdRx,	rg1IdRy,
	rg1ValRx,	rg1ValRy,

	rg2IdRs,	rg2IdRt,
	rg2ValRs,	rg2ValRt,
	rg2IdRu,	rg2IdRv,
	rg2ValRu,	rg2ValRv,
	rg2IdRx,	rg2IdRy,
	rg2ValRx,	rg2ValRy,

	rg1IdRnA1,	rg1ValRnA1,	rg1HldRnA1,
	rg1IdRnB1,	rg1ValRnB1,	rg1HldRnB1,
	rg1IdRnC1,	rg1ValRnC1,	rg1HldRnC1,

	rg2IdRnA1,		//Destination ID (EX1, Lane 1)
	rg2ValRnA1,		//Destination Value (EX1, Lane 1)
	rg2HldRnA1,		//Destination Value (EX1, Lane 1)

	rg2IdRnB1,		//Destination ID (EX1, Lane 2)
	rg2ValRnB1,		//Destination Value (EX1, Lane 2)
	rg2HldRnB1,		//Destination Value (EX1, Lane 2)

	rg2IdRnC1,		//Destination ID (EX1, Lane 3)
	rg2ValRnC1,		//Destination Value (EX1, Lane 3)
	rg2HldRnC1,		//Destination Value (EX1, Lane 3)

	rg1IdRnA2,		//Destination ID (EX2, Lane 1)
	rg1ValRnA2,		//Destination Value (EX2, Lane 1)	
	rg1HldRnA2,		//Destination Value (EX2, Lane 1)	

	rg1IdRnB2,		//Destination ID (EX2, Lane 2)
	rg1ValRnB2,		//Destination Value (EX2, Lane 2)
	rg1HldRnB2,		//Destination Value (EX2, Lane 2)

	rg1IdRnC2,		//Destination ID (EX2, Lane 3)
	rg1ValRnC2,		//Destination Value (EX2, Lane 3)
	rg1HldRnC2,		//Destination Value (EX2, Lane 3)

	rg2IdRnA2,		//Destination ID (EX2, Lane 1)
	rg2ValRnA2,		//Destination Value (EX2, Lane 1)	
	rg2HldRnA2,		//Destination Value (EX2, Lane 1)	

	rg2IdRnB2,		//Destination ID (EX2, Lane 2)
	rg2ValRnB2,		//Destination Value (EX2, Lane 2)
	rg2HldRnB2,		//Destination Value (EX2, Lane 2)

	rg2IdRnC2,		//Destination ID (EX2, Lane 3)
	rg2ValRnC2,		//Destination Value (EX2, Lane 3)
	rg2HldRnC2,		//Destination Value (EX2, Lane 3)

	rg1IdRnA3,		//Destination ID (EX2, Lane 1)
	rg1ValRnA3,		//Destination Value (EX2, Lane 1)
	rg1HldRnA3,		//Destination Value (EX2, Lane 1)

	rg1IdRnB3,		//Destination ID (EX2, Lane 2)
	rg1ValRnB3,		//Destination Value (EX2, Lane 2)
	rg1HldRnB3,		//Destination Value (EX2, Lane 2)

	rg1IdRnC3,		//Destination ID (EX2, Lane 3)
	rg1ValRnC3,		//Destination Value (EX2, Lane 3)
	rg1HldRnC3,		//Destination Value (EX2, Lane 3)
	
	rg2IdRnA3,		//Destination ID (EX2, Lane 1)
	rg2ValRnA3,		//Destination Value (EX2, Lane 1)
	rg2HldRnA3,		//Destination Value (EX2, Lane 1)

	rg2IdRnB3,		//Destination ID (EX2, Lane 2)
	rg2ValRnB3,		//Destination Value (EX2, Lane 2)
	rg2HldRnB3,		//Destination Value (EX2, Lane 2)

	rg2IdRnC3,		//Destination ID (EX2, Lane 3)
	rg2ValRnC3,		//Destination Value (EX2, Lane 3)
	rg2HldRnC3,		//Destination Value (EX2, Lane 3)
	
	rg1ValPc,		//PC Value (Synthesized)
	rg1ValImmA,		//Immediate (Decode, Lane 1)
	rg1ValImmB,		//Immediate (Decode, Lane 2)
	rg1ValImmC,		//Immediate (Decode, Lane 3)
	
	rg2ValPc,		//PC Value (Synthesized)
	rg2ValImmA,		//Immediate (Decode, Lane 1)
	rg2ValImmB,		//Immediate (Decode, Lane 2)
	rg2ValImmC,		//Immediate (Decode, Lane 3)
	
	rg1Ex1Flush,
	rg1Ex2Flush,
	rg1Ex3Flush,

	rg2Ex1Flush,
	rg2Ex2Flush,
	rg2Ex3Flush,

	rg1OutDlr,	rg1InDlr,
	rg1OutDhr,	rg1InDhr,
	rg1OutSp,	rg1InSp,

	rg2OutDlr,	rg2InDlr,
	rg2OutDhr,	rg2InDhr,
	rg2OutSp,	rg2InSp


	rg1OutPc,	rg1InPc,
	rg1OutLr,	rg1InLr,
	rg1OutSr,	rg1InSr,

	rg2OutPc,	rg2InPc,
	rg2OutLr,	rg2InLr,
	rg2OutSr,	rg2InSr,

	regOutExsr,	regInExsr,
	regOutTea,	regInTea,

	rg1OutSpc,	rg1InSpc,
	rg1OutSsp,	rg1InSsp,
	rg1OutFpsr,	rg1InFpsr,

	rg2OutSpc,	rg2InSpc,
	rg2OutSsp,	rg2InSsp,
	rg2OutFpsr,	rg2InFpsr,

	regOutVbr,
	regOutMmcr,

	rg1OutGbr,
	rg1OutTbr,
	rg1OutKrr,

	rg2OutGbr,
	rg2OutTbr,
	rg2OutKrr
	);


input			clock;
input			reset;

input			exHold1;
input			exHold2;

input[8:0]		rg1IdUCmd;
input[8:0]		rg1IdUIxt;

input[8:0]		rg2IdUCmd;
input[8:0]		rg2IdUIxt;

`input_gpr		rg1IdRs;		//Source A
`input_gpr		rg1IdRt;		//Source B
`input_gpr		rg1IdRu;		//Source C
`input_gpr		rg1IdRv;		//Source D
`input_gpr		rg1IdRx;		//Source E
`input_gpr		rg1IdRy;		//Source F

`input_gpr		rg2IdRs;		//Source A
`input_gpr		rg2IdRt;		//Source B
`input_gpr		rg2IdRu;		//Source C
`input_gpr		rg2IdRv;		//Source D
`input_gpr		rg2IdRx;		//Source E
`input_gpr		rg2IdRy;		//Source F

output[63:0]	rg1ValRs;
output[63:0]	rg1ValRt;
output[63:0]	rg1ValRu;
output[63:0]	rg1ValRv;
output[63:0]	rg1ValRx;
output[63:0]	rg1ValRy;

output[63:0]	rg2ValRs;
output[63:0]	rg2ValRt;
output[63:0]	rg2ValRu;
output[63:0]	rg2ValRv;
output[63:0]	rg2ValRx;
output[63:0]	rg2ValRy;

`input_gpr		rg1IdRnA1;		//Destination ID
input[63:0]		rg1ValRnA1;		//Destination Value
input[1:0]		rg1HldRnA1;		//Destination Held

`input_gpr		rg1IdRnA2;		//Destination ID
input[63:0]		rg1ValRnA2;		//Destination Value
input[1:0]		rg1HldRnA2;		//Destination Held

`input_gpr		rg1IdRnA3;		//Destination ID
input[63:0]		rg1ValRnA3;		//Destination Value
input[1:0]		rg1HldRnA3;		//Destination Held

`input_gpr		rg2IdRnA1;		//Destination ID
input[63:0]		rg2ValRnA1;		//Destination Value
input[1:0]		rg2HldRnA1;		//Destination Held

`input_gpr		rg2IdRnA2;		//Destination ID
input[63:0]		rg2ValRnA2;		//Destination Value
input[1:0]		rg2HldRnA2;		//Destination Held

`input_gpr		rg2IdRnA3;		//Destination ID
input[63:0]		rg2ValRnA3;		//Destination Value
input[1:0]		rg2HldRnA3;		//Destination Held

`input_gpr		rg1IdRnB1;		//Destination ID
input[63:0]		rg1ValRnB1;		//Destination Value
input[1:0]		rg1HldRnB1;		//Destination Held

`input_gpr		rg1IdRnB2;		//Destination ID
input[63:0]		rg1ValRnB2;		//Destination Value
input[1:0]		rg1HldRnB2;		//Destination Held

`input_gpr		rg1IdRnB3;		//Destination ID
input[63:0]		rg1ValRnB3;		//Destination Value
input[1:0]		rg1HldRnB3;		//Destination Held

`input_gpr		rg2IdRnB1;		//Destination ID
input[63:0]		rg2ValRnB1;		//Destination Value
input[1:0]		rg2HldRnB1;		//Destination Held

`input_gpr		rg2IdRnB2;		//Destination ID
input[63:0]		rg2ValRnB2;		//Destination Value
input[1:0]		rg2HldRnB2;		//Destination Held

`input_gpr		rg2IdRnB3;		//Destination ID
input[63:0]		rg2ValRnB3;		//Destination Value
input[1:0]		rg2HldRnB3;		//Destination Held

`input_gpr		rg1IdRnC1;		//Destination ID
input[63:0]		rg1ValRnC1;		//Destination Value
input[1:0]		rg1HldRnC1;		//Destination Held

`input_gpr		rg1IdRnC2;		//Destination ID
input[63:0]		rg1ValRnC2;		//Destination Value
input[1:0]		rg1HldRnC2;		//Destination Held

`input_gpr		rg1IdRnC3;		//Destination ID
input[63:0]		rg1ValRnC3;		//Destination Value
input[1:0]		rg1HldRnC3;		//Destination Held

`input_gpr		rg2IdRnC1;		//Destination ID
input[63:0]		rg2ValRnC1;		//Destination Value
input[1:0]		rg2HldRnC1;		//Destination Held

`input_gpr		rg2IdRnC2;		//Destination ID
input[63:0]		rg2ValRnC2;		//Destination Value
input[1:0]		rg2HldRnC2;		//Destination Held

`input_gpr		rg2IdRnC3;		//Destination ID
input[63:0]		rg2ValRnC3;		//Destination Value
input[1:0]		rg2HldRnC3;		//Destination Held


input [47:0]	rg1ValPc;		//PC Value (Synthesized)
input [32:0]	rg1ValImmA;		//Immediate (Decode)
input [32:0]	rg1ValImmB;		//Immediate (Decode)
input [32:0]	rg1ValImmC;		//Immediate (Decode)

input [47:0]	rg2ValPc;		//PC Value (Synthesized)
input [32:0]	rg2ValImmA;		//Immediate (Decode)
input [32:0]	rg2ValImmB;		//Immediate (Decode)
input [32:0]	rg2ValImmC;		//Immediate (Decode)

input			rg1Ex1Flush;
input			rg1Ex2Flush;
input			rg1Ex3Flush;

input			rg2Ex1Flush;
input			rg2Ex2Flush;
input			rg2Ex3Flush;


output[63:0]	rg1OutDlr;
input [63:0]	rg1InDlr;
output[63:0]	rg1OutDhr;
input [63:0]	rg1InDhr;
output[63:0]	rg1OutSp;
input [63:0]	rg1InSp;

output[63:0]	rg2OutDlr;
input [63:0]	rg2InDlr;
output[63:0]	rg2OutDhr;
input [63:0]	rg2InDhr;
output[63:0]	rg2OutSp;
input [63:0]	rg2InSp;


output[47:0]	rg1OutPc;
input [47:0]	rg1InPc;
output[63:0]	rg1OutLr;
input [63:0]	rg1InLr;
output[63:0]	rg1OutSr;
input [63:0]	rg1InSr;

output[47:0]	rg2OutPc;
input [47:0]	rg2InPc;
output[63:0]	rg2OutLr;
input [63:0]	rg2InLr;
output[63:0]	rg2OutSr;
input [63:0]	rg2InSr;

output[63:0]	regOutExsr;
input [63:0]	regInExsr;
output[63:0]	regOutTea;
input [63:0]	regInTea;

output[47:0]	rg1OutSpc;
input [47:0]	rg1InSpc;
output[47:0]	rg1OutSsp;
input [47:0]	rg1InSsp;
output[15:0]	rg1OutFpsr;
input [15:0]	rg1InFpsr;

output[47:0]	rg2OutSpc;
input [47:0]	rg2InSpc;
output[47:0]	rg2OutSsp;
input [47:0]	rg2InSsp;
output[15:0]	rg2OutFpsr;
input [15:0]	rg2InFpsr;

output[47:0]	regOutVbr;
output[63:0]	regOutMmcr;

output[47:0]	rg1OutGbr;
output[47:0]	rg1OutTbr;
output[63:0]	rg1OutKrr;

output[47:0]	rg2OutGbr;
output[47:0]	rg2OutTbr;
output[63:0]	rg2OutKrr;



`wire_gpr		craIdCm;		//Source ID
wire[63:0]		craValCm;		//Source Value

`wire_gpr		crbIdCm;		//Source ID
wire[63:0]		crbValCm;		//Source Value

assign		craIdCm = rg1IdRs;
assign		crbIdCm = rg2IdRs;


`wire_gpr		craIdCn1;		//Destination ID (EX1)
`wire_gpr		craIdCn2;		//Destination ID (EX2)
`wire_gpr		craIdCn3;		//Destination ID (EX3)

`wire_gpr		crbIdCn1;		//Destination ID (EX1)
`wire_gpr		crbIdCn2;		//Destination ID (EX2)
`wire_gpr		crbIdCn3;		//Destination ID (EX3)

assign		craIdCn1 = rg1IdRnA1;
assign		craIdCn2 = rg1IdRnA2;
assign		craIdCn3 = rg1IdRnA3;

assign		crbIdCn1 = rg2IdRnA1;
assign		crbIdCn2 = rg2IdRnA2;
assign		crbIdCn3 = rg2IdRnA3;

wire[63:0]		craValCn1;		//Destination Value (EX1)
wire[63:0]		craValCn2;		//Destination Value (EX2)
wire[63:0]		craValCn3;		//Destination Value (EX3)

wire[63:0]		crbValCn1;		//Destination Value (EX1)
wire[63:0]		crbValCn2;		//Destination Value (EX2)
wire[63:0]		crbValCn3;		//Destination Value (EX3)

assign		craValCn1 = rg1ValRnA1;
assign		craValCn2 = rg1ValRnA2;
assign		craValCn3 = rg1ValRnA3;

assign		crbValCn1 = rg2ValRnA1;
assign		crbValCn2 = rg2ValRnA2;
assign		crbValCn3 = rg2ValRnA3;

wire[47:0]		craOutPc;
wire[47:0]		craInPc;
wire[63:0]		craOutLr;
wire[63:0]		craInLr;

wire[47:0]		crbOutPc;
wire[47:0]		crbInPc;
wire[63:0]		crbOutLr;
wire[63:0]		crbInLr;

assign	craInPc = rg1InPc;
assign	craInLr = rg1InLr;

assign	crbInPc = rg2InPc;
assign	crbInLr = rg2InLr;


RegGPR_6R3W regGprA(
	clock,
	reset,
	rg1ExHold,

	rg1IdUCmd,
	rg1IdUIxt,

	rg1IdRs,		//Source A, ALU / Base
	rg1IdRt,		//Source B, ALU / Index
	rg1IdRu,		//Source C
	rg1IdRv,		//Source D
	rg1IdRx,		//Source E
	rg1IdRy,		//Source F, MemStore
	rg1ValRs,		//Source A Value
	rg1ValRt,		//Source B Value
	rg1ValRu,		//Source C Value
	rg1ValRv,		//Source D Value
	rg1ValRx,		//Source E Value
	rg1ValRy,		//Source F Value

	rg1IdRn1,		//Destination ID (EX1, L1)
	rg1ValRn1,		//Destination Value (EX1, L1)
	rg1IdRnB1,		//Destination ID (EX1, L2)
	rg1ValRnB1,		//Destination Value (EX1, L2)
	rg1IdRnC1,		//Destination ID (EX1, L3)
	rg1ValRnC1,		//Destination Value (EX1, L3)

	rg1IdRn2,		//Destination ID (EX2, L1)
	rg1ValRn2,		//Destination Value (EX2, L1)
	rg1IdRnB2,		//Destination ID (EX2, L2)
	rg1ValRnB2,		//Destination Value (EX2, L2)
	rg1IdRnC2,		//Destination ID (EX2, L3)
	rg1ValRnC2,		//Destination Value (EX2, L3)

	rg1IdRn3,		//Destination ID (EX3, L1)
	rg1ValRn3,		//Destination Value (EX3, L1)
	rg1IdRnB3,		//Destination ID (EX3, L2)
	rg1ValRnB3,		//Destination Value (EX3, L2)
	rg1IdRnC3,		//Destination ID (EX3, L3)
	rg1ValRnC3,		//Destination Value (EX3, L3)

	rg1ValPc,		//PC Value (Synthesized)
	rg1ValGbr,		//GBR Value (CR)
	rg1ValImm,		//Immediate (Decode, A)
	rg1ValImmB,		//Immediate (Decode, B)
	rg1ValImmC,		//Immediate (Decode, C)
	rg1ValLr,		//LR Value (CR)
	rg1ValCm,		//Cm Port (CR)

	rg1Ex1Flush,	//Flush EX1
	rg1Ex2Flush,	//Flush EX2
	rg1Ex3Flush,	//Flush EX3

	rg1Ex1DualLane,
	rg1Ex2DualLane,
	rg1Ex3DualLane,

	rg1OutDlr,	rg1InDlr,
	rg1OutDhr,	rg1InDhr,
	rg1OutSp,	rg1InSp
	);


RegGPR_6R3W regGprB(
	clock,
	reset,
	rg2ExHold,

	rg2IdUCmd,
	rg2IdUIxt,

	rg2IdRs,		//Source A, ALU / Base
	rg2IdRt,		//Source B, ALU / Index
	rg2IdRu,		//Source C
	rg2IdRv,		//Source D
	rg2IdRx,		//Source E
	rg2IdRy,		//Source F, MemStore
	rg2ValRs,		//Source A Value
	rg2ValRt,		//Source B Value
	rg2ValRu,		//Source C Value
	rg2ValRv,		//Source D Value
	rg2ValRx,		//Source E Value
	rg2ValRy,		//Source F Value

	rg2IdRn1,		//Destination ID (EX1, L1)
	rg2ValRn1,		//Destination Value (EX1, L1)
	rg2IdRnB1,		//Destination ID (EX1, L2)
	rg2ValRnB1,		//Destination Value (EX1, L2)
	rg2IdRnC1,		//Destination ID (EX1, L3)
	rg2ValRnC1,		//Destination Value (EX1, L3)

	rg2IdRn2,		//Destination ID (EX2, L1)
	rg2ValRn2,		//Destination Value (EX2, L1)
	rg2IdRnB2,		//Destination ID (EX2, L2)
	rg2ValRnB2,		//Destination Value (EX2, L2)
	rg2IdRnC2,		//Destination ID (EX2, L3)
	rg2ValRnC2,		//Destination Value (EX2, L3)

	rg2IdRn3,		//Destination ID (EX2, L1)
	rg2ValRn3,		//Destination Value (EX2, L1)
	rg2IdRnB3,		//Destination ID (EX2, L2)
	rg2ValRnB3,		//Destination Value (EX2, L2)
	rg2IdRnC3,		//Destination ID (EX2, L3)
	rg2ValRnC3,		//Destination Value (EX2, L3)

	rg2ValPc,		//PC Value (Synthesized)
	rg2ValGbr,		//GBR Value (CR)
	rg2ValImm,		//Immediate (Decode, A)
	rg2ValImmB,		//Immediate (Decode, B)
	rg2ValImmC,		//Immediate (Decode, C)
	rg2ValLr,		//LR Value (CR)
	rg2ValCm,		//Cm Port (CR)

	rg2Ex1Flush,	//Flush EX1
	rg2Ex2Flush,	//Flush EX2
	rg2Ex3Flush,	//Flush EX3

	rg2Ex1DualLane,
	rg2Ex2DualLane,
	rg2Ex3DualLane,

	rg2OutDlr,	rg2InDlr,
	rg2OutDhr,	rg2InDhr,
	rg2OutSp,	rg2InSp
	);


RegCR regCrA(
	clock,	reset,
	exHold2,

	craIdCm,		//Source ID
	craValCm,		//Source Value

	craIdCn1,		//Destination ID (EX1)
	craValCn1,		//Destination Value (EX1)
	craIdCn2,		//Destination ID (EX2)
	craValCn2,		//Destination Value (EX2)
	craIdCn3,		//Destination ID (EX3)
	craValCn3,		//Destination Value (EX3)

	gpaEx1Flush,	//Flush EX1
	gpaEx2Flush,	//Flush EX2
	gpaEx3Flush,	//Flush EX3

	gpaValPc,		//PC Value (Synthesized)

	craOutPc,	craInPc,
	craOutLr,	craInLr,
	craOutSr,	craInSr,

	craOutExsr,	craInExsr,
	craOutSpc,	craInSpc,
	craOutSsp,	craInSsp,
	craOutTea,	craInTea,
	craOutFpsr,	craInFpsr,

	craOutVbr,
	craOutGbr,
	craOutTbr,
	craOutMmcr,
	craOutKrr
	);


RegCR regCrB(
	clock,	reset,
	exHold2,

	crbIdCm,		//Source ID
	crbValCm,		//Source Value

	crbIdCn1,		//Destination ID (EX1)
	crbValCn1,		//Destination Value (EX1)
	crbIdCn2,		//Destination ID (EX2)
	crbValCn2,		//Destination Value (EX2)
	crbIdCn3,		//Destination ID (EX3)
	crbValCn3,		//Destination Value (EX3)

	gpbEx1Flush,	//Flush EX1
	gpbEx2Flush,	//Flush EX2
	gpbEx3Flush,	//Flush EX3

	gpbValPc,		//PC Value (Synthesized)

	crbOutPc,	crbInPc,
	crbOutLr,	crbInLr,
	crbOutSr,	crbInSr,

	crbOutExsr,	craInExsr,
	crbOutSpc,	craInSpc,
	crbOutSsp,	craInSsp,
	crbOutTea,	craInTea,
	crbOutFpsr,	crbInFpsr,

	crbOutVbr,
	crbOutGbr,
	crbOutTbr,
	crbOutMmcr,
	crbOutKrr
	);


endmodule

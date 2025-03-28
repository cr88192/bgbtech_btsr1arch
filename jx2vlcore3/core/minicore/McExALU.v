/*
ALU

Perform some ALU Operations
May Sign or Zero Extend output.

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
  0100: TST
  0101: AND
  0110: OR
  0111: XOR
  1000: CMPNE
  1001: CMPHS
  1010: CMPGE
  1011: NOR
  1100: CMPEQ
  1101: CMPHI
  1110: CMPGT
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

module McExALU(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	regValXs,
	regValRp,
	idUCmd,
	idUIxt,
	exHold,
	regInSrST,
	regOutVal,
	regOutSrST,
	regInCarryD,
	regOutJcmpT
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[63:0]		regValXs;
input[63:0]		regValRp;
input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			exHold;
input[5:0]		regInSrST;

output[63:0]	regOutVal;
output[5:0]		regOutSrST;
input[7:0]		regInCarryD;
output			regOutJcmpT;

wire			regInSrT;
wire			regInSrS;
assign		regInSrT = regInSrST[0];
assign		regInSrS = regInSrST[1];

wire			regInSrP;
wire			regInSrQ;
wire			regInSrR;
wire			regInSrO;
assign		regInSrP = regInSrST[2];
assign		regInSrQ = regInSrST[3];
assign		regInSrR = regInSrST[4];
assign		regInSrO = regInSrST[5];

reg[8:0]		idUIxt2;


reg[63:0]	tRegOutVal2;
reg			tRegOutSrT2;
reg			tRegOutSrS2;

reg			tRegOutSrP2;
reg			tRegOutSrQ2;
reg			tRegOutSrR2;
reg			tRegOutSrO2;

assign	regOutVal = tRegOutVal2;
assign	regOutSrST = {
	tRegOutSrO2, tRegOutSrR2,
	tRegOutSrQ2, tRegOutSrP2,
	tRegOutSrS2, tRegOutSrT2
	};

reg			tRegOutJcmpT;
assign		regOutJcmpT = tRegOutJcmpT;

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
reg			tRegOutSrS;

reg			tRegOutSrP;
reg			tRegOutSrQ;
reg			tRegOutSrR;
reg			tRegOutSrO;

`ifdef jx2_enable_clz
wire[7:0]		tClzVal;
wire[63:0]		tClzRsVal2;
ExOpClz	clz(
	clock,		reset,
	idUCmd,		idUIxt,
	regValRs,	tClzVal,	tClzRsVal2);
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

reg[32:0]	tAdd2A0;
reg[32:0]	tAdd2A1;
reg[32:0]	tAdd2B0;
reg[32:0]	tAdd2B1;

reg[32:0]	tSub2A0;
reg[32:0]	tSub2A1;
reg[32:0]	tSub2B0;
reg[32:0]	tSub2B1;

reg[64:0]	tAdd3A0;
reg[64:0]	tAdd3A1;
reg[64:0]	tSub3A0;
reg[64:0]	tSub3A1;

reg[32:0]	tResult1A;
reg			tResult1T;
reg			tResultw1T;
reg			tResultb1T;

reg[32:0]	tResult1B;
reg			tResult1S;

reg[64:0]	tResult2A;
reg			tResult2T;

reg			tResult1P;
reg			tResult1Q;
reg			tResult1R;
reg			tResult1O;

reg			tResultb1P;
reg			tResultb1Q;
reg			tResultb1R;
reg			tResultb1O;

reg			tAdd1SF;
reg			tAdd2SF;
reg			tAdd1BSF;

reg			tSub1ZF;
reg			tSub1CF;
reg			tSub1SF;
reg			tSub1VF;
reg			tTst1ZF;

reg			tTst1WZF_A;
reg			tTst1WZF_B;
reg			tTst1WZF_C;
reg			tTst1WZF_D;

reg			tSubAZF;
reg			tSubACF;
reg[12:0]	tSubAA0;
reg[12:0]	tSubAA1;

reg			tSubTZF;
reg			tSubTHZF;
reg			tSubTLZF;

reg			tSubABZF;

reg			tSub2ZF;
reg			tSub2CF;
reg			tSub2SF;
reg			tSub2VF;
reg			tTst2ZF;

reg			tSub1BZF;
reg			tSub1BCF;
reg			tSub1BSF;
reg			tSub1BVF;
reg			tTst1BZF;

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

reg			tSub1BZF_A;
reg			tSub1BZF_B;
reg			tSub1BZF_C;
reg			tSub1BZF_D;

reg			tSub1BZF_E;
reg			tSub1BZF_F;
reg			tSub1BZF_G;
reg			tSub1BZF_H;

reg			tSub1SxVF;
reg			tSub2SxVF;
reg			tSub1BSxVF;

reg			tFCmpRsIsNaN;
reg			tFCmpRtIsNaN;
reg			tFCmpEqP;
reg			tFCmpxEqP;
reg			tFCmpGtP;
reg			tFCmpxGtP;

reg			tFCmpxRsIsNaN;
reg			tFCmpxRtIsNaN;

reg			tFCmpGtP_FA;
reg			tFCmpGtP_FB;

reg			tFCmpGtP_HA;
reg			tFCmpGtP_HB;
reg			tFCmpGtP_HC;
reg			tFCmpGtP_HD;

reg[32:0]	tResultu1A;
reg[32:0]	tResultu1B;
reg[64:0]	tResultu2A;

reg[64:0]	tResult_Add64;
reg[64:0]	tResult_Sub64;
// reg[64:0]	tResult_Adc64;
// reg[64:0]	tResult_Sbb64;

reg[32:0]	tResult_Add32;
reg[32:0]	tResult_Sub32;
// reg[32:0]	tResult_Adc32;
// reg[32:0]	tResult_Sbb32;

reg[32:0]	tResult1W;
reg[64:0]	tResult2W;
reg[32:0]	tResultShufB;
reg[64:0]	tResultShufW;

reg[32:0]	tResultb1W;
reg[64:0]	tResultb2W;

reg[63:0]	tRegConvVal;

reg			tOpIsWx;


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

`ifdef jx2_enable_cmptag

	tSubAA1 = { 1'b0, regValRs[59:48] } + { 1'b0, ~regValRt[11: 0] } + 1;
	tSubABZF	= regValRt[31:12]==0;

	tSubTHZF	=
		((regValRs[63:60]==regValRt[4:1]) && !regValRt[  0]) ||
		((regValRs[63:61]==regValRt[4:2]) && (regValRt[1:0]==2'b01)) ||
		((regValRs[63:62]==regValRt[4:3]) && (regValRt[2:0]==3'b011)) ||
		((regValRs[63   ]==regValRt[4  ]) && (regValRt[2:0]==3'b111)) ;
	tSubTLZF	=
		((regValRs[ 3: 0]==regValRt[4:1]) && !regValRt[  0]) ||
		((regValRs[ 2: 0]==regValRt[4:2]) && (regValRt[1:0]==2'b01)) ||
		((regValRs[ 1: 0]==regValRt[4:3]) && (regValRt[2:0]==3'b011)) ||
		((regValRs[ 0   ]==regValRt[4  ]) && (regValRt[2:0]==3'b111)) ;
	tSubTZF	= regValRt[5] ? tSubTLZF : tSubTHZF;

`else

	tSubAA1		= 0;
	tSubABZF	= 0;
	tSubTZF		= 0;

`endif


//	tOpIsWx = (idUIxt[7:6] == 2'b11);
	tOpIsWx =
		(idUIxt[8:6] == JX2_IUC_WX) ||
		(idUIxt[8:6] == JX2_IUC_WT) ||
		(idUIxt[8:6] == JX2_IUC_WF) ||
		(idUIxt[8:6] == JX2_IUC_WXA);


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

`ifdef jx2_alu_wx
	if(tOpIsWx)
	begin
		tAddCa2_Add = {
			regInCarryD[0] ? tAddCa2A1 : tAddCa2A0,
			regInCarryD[0] };
		tSubCa2_Sub = {
			regInCarryD[3] ? tSubCa2A1 : tSubCa2A0,
			regInCarryD[3] };
	end
`endif

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

	tSub1BZF_A	= (tSub2A1[ 7: 0]==0);
	tSub1BZF_B	= (tSub2A1[15: 8]==0);
	tSub1BZF_C	= (tSub2A1[23:16]==0);
	tSub1BZF_D	= (tSub2A1[31:24]==0);
	tSub1BZF_E	= (tSub3A1[39:32]==0);
	tSub1BZF_F	= (tSub3A1[47:40]==0);
	tSub1BZF_G	= (tSub3A1[55:48]==0);
	tSub1BZF_H	= (tSub3A1[63:56]==0);

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

	tSubACF		= tSubAA1[12] && tSubABZF;
	tSubAZF		= (tSubAA1[11:0]==0) && tSubABZF;

//	tSub1ZF		= (tSub2A1[15:0]==0) && (tSub2A1[31:16]==0);
//	tSub1BZF	= (tSub3A1[47:32]==0) && (tSub3A1[63:48]==0);
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
	
	tAdd1SF = tAdd2A1[31];
	tAdd2SF = tAdd3A1[63];
	tAdd1BSF = tAdd2B1[31];

	tTst1WZF_A	= ((regValRs[15: 0]&regValRt[15: 0])==0);
	tTst1WZF_B	= ((regValRs[31:16]&regValRt[31:16])==0);
	tTst1WZF_C	= ((regValRs[47:32]&regValRt[47:32])==0);
	tTst1WZF_D	= ((regValRs[63:48]&regValRt[63:48])==0);

	tTst1ZF		= tTst1WZF_A && tTst1WZF_B;
	tTst1BZF	= tTst1WZF_C && tTst1WZF_D;

//	tTst1ZF =
//		((regValRs[15: 0]&regValRt[15: 0])==0) &&
//		((regValRs[31:16]&regValRt[31:16])==0) ;
//	tTst1BZF =
//		((regValRs[47:32]&regValRt[47:32])==0) &&
//		((regValRs[63:48]&regValRt[63:48])==0) ;

	tTst2ZF =
		tTst1ZF && tTst1BZF;

`ifdef jx2_fcmp_alu
	tFCmpRsIsNaN	= (regValRs[62:52]==11'h7FF) && (regValRs[51:48]!=0);
	tFCmpRtIsNaN	= (regValRt[62:52]==11'h7FF) && (regValRt[51:48]!=0);

	tFCmpxRsIsNaN	= (regValRs[62:48]==15'h7FFF) && (regValRs[47:44]!=0);
	tFCmpxRtIsNaN	= (regValRt[62:48]==15'h7FFF) && (regValRt[47:44]!=0);

	tFCmpEqP		= tSub2ZF && !tFCmpRsIsNaN;
//	tFCmpxEqP		= tSub2ZF && !tFCmpxRsIsNaN;
	tFCmpxEqP		= tSub2ZF && !tFCmpxRsIsNaN && regInCarryD[4];
//	tFCmpGtP;

	casez({regValRs[63], regValRt[63], tSub2SF, tSub2ZF})
		4'b0000: tFCmpGtP=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP=0;	/* s==t */
		4'b001z: tFCmpGtP=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP=0;	/* (s<0) && (t>0) */

//		4'b110z: tFCmpGtP=1;	/* (s-t)>0 */
//		4'b1110: tFCmpGtP=0;	/* (s-t)<0 */
		4'b110z: tFCmpGtP=0;	/* (s-t)>0 */
//		4'b1100: tFCmpGtP=0;	/* (s-t)>0 */
//		4'b1101: tFCmpGtP=1;	/* (s-t)>0 */
		4'b1110: tFCmpGtP=1;	/* (s-t)<0 */

		4'b1111: tFCmpGtP=0;	/* s==t */
	endcase
	
	tFCmpxGtP = tFCmpGtP && (tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));

`ifdef def_true

	casez({regValRs[31], regValRt[31], tSub1SF, tSub1ZF})
		4'b0000: tFCmpGtP_FA=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_FA=0;	/* s==t */
		4'b001z: tFCmpGtP_FA=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_FA=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_FA=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_FA=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_FA=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_FA=0;	/* s==t */
	endcase

	tFCmpGtP_FB		= tFCmpGtP;

	casez({regValRs[15], regValRt[15], tSub1WSF_A, tSub1WZF_A})
		4'b0000: tFCmpGtP_HA=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HA=0;	/* s==t */
		4'b001z: tFCmpGtP_HA=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HA=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HA=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HA=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HA=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HA=0;	/* s==t */
	endcase
	casez({regValRs[31], regValRt[31], tSub1WSF_B, tSub1WZF_B})
		4'b0000: tFCmpGtP_HB=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HB=0;	/* s==t */
		4'b001z: tFCmpGtP_HB=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HB=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HB=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HB=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HB=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HB=0;	/* s==t */
	endcase
	casez({regValRs[47], regValRt[47], tSub1WSF_C, tSub1WZF_C})
		4'b0000: tFCmpGtP_HC=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HC=0;	/* s==t */
		4'b001z: tFCmpGtP_HC=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HC=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HC=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HC=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HC=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HC=0;	/* s==t */
	endcase
	casez({regValRs[63], regValRt[63], tSub1WSF_D, tSub1WZF_D})
		4'b0000: tFCmpGtP_HD=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HD=0;	/* s==t */
		4'b001z: tFCmpGtP_HD=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HD=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HD=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HD=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HD=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HD=0;	/* s==t */
	endcase

`endif

`endif

`ifdef def_true
	case({regValRs[31], regValRt[31], tSub1SF})
		3'b000: tSub1VF=0;
		3'b001: tSub1VF=0;
		3'b010: tSub1VF=0;
		3'b011: tSub1VF=1;
		3'b100: tSub1VF=1;
		3'b101: tSub1VF=0;
		3'b110: tSub1VF=0;
		3'b111: tSub1VF=0;
	endcase

	case({regValRs[63], regValRt[63], tSub2SF})
		3'b000: tSub2VF=0;
		3'b001: tSub2VF=0;
		3'b010: tSub2VF=0;
		3'b011: tSub2VF=1;
		3'b100: tSub2VF=1;
		3'b101: tSub2VF=0;
		3'b110: tSub2VF=0;
		3'b111: tSub2VF=0;
	endcase

`ifdef jx2_enable_gsv
	case({regValRs[63], regValRt[63], tSub1BSF})
		3'b000: tSub1BVF=0;
		3'b001: tSub1BVF=0;
		3'b010: tSub1BVF=0;
		3'b011: tSub1BVF=1;
		3'b100: tSub1BVF=1;
		3'b101: tSub1BVF=0;
		3'b110: tSub1BVF=0;
		3'b111: tSub1BVF=0;
	endcase
`else
	tSub1BVF=1'bX;
`endif

	tSub1SxVF = tSub1SF ^ tSub1VF;
	tSub2SxVF = tSub2SF ^ tSub2VF;
	tSub1BSxVF = tSub1BSF ^ tSub1BVF;

`endif

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
		2'b00: tResultShufW[15: 0]=regValRs[15: 0];
		2'b01: tResultShufW[15: 0]=regValRs[31:16];
		2'b10: tResultShufW[15: 0]=regValRs[47:32];
		2'b11: tResultShufW[15: 0]=regValRs[63:48];
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

`ifndef def_true

//	case({regValRs[31], regValRt[31], tSub1SF})
	case({regValRs[31], regValRt[31], tAdd1SF})
		3'b000: tSub1VF=0;
		3'b001: tSub1VF=1;
		3'b010: tSub1VF=0;
		3'b011: tSub1VF=0;
		3'b100: tSub1VF=0;
		3'b101: tSub1VF=0;
		3'b110: tSub1VF=1;
		3'b111: tSub1VF=0;
	endcase

`ifdef jx2_enable_gsv
//	case({regValRs[63], regValRt[63], tSub1BSF})
	case({regValRs[63], regValRt[63], tAdd1BSF})
		3'b000: tSub1BVF=0;
		3'b001: tSub1BVF=1;
		3'b010: tSub1BVF=0;
		3'b011: tSub1BVF=0;
		3'b100: tSub1BVF=0;
		3'b101: tSub1BVF=0;
		3'b110: tSub1BVF=1;
		3'b111: tSub1BVF=0;
	endcase
`else
	tSub1BVF=1'bX;
`endif

//	case({regValRs[63], regValRt[63], tSub2SF})
	case({regValRs[63], regValRt[63], tAdd2SF})
		3'b000: tSub2VF=0;
		3'b001: tSub2VF=1;
		3'b010: tSub2VF=0;
		3'b011: tSub2VF=0;
		3'b100: tSub2VF=0;
		3'b101: tSub2VF=0;
		3'b110: tSub2VF=1;
		3'b111: tSub2VF=0;
	endcase

`ifdef def_true
//	case({regValRs[31], regValRt[31], tSub1SF})
	case({regValRs[31], regValRt[31], tAdd1SF})
		3'b000: tSub1SxVF=0;
		3'b001: tSub1SxVF=0;
		3'b010: tSub1SxVF=0;
		3'b011: tSub1SxVF=1;
		3'b100: tSub1SxVF=0;
		3'b101: tSub1SxVF=1;
		3'b110: tSub1SxVF=1;
		3'b111: tSub1SxVF=1;
	endcase

`ifdef jx2_enable_gsv
//	case({regValRs[63], regValRt[63], tSub1BSF})
	case({regValRs[63], regValRt[63], tAdd1BSF})
		3'b000: tSub1BSxVF=0;
		3'b001: tSub1BSxVF=0;
		3'b010: tSub1BSxVF=0;
		3'b011: tSub1BSxVF=1;
		3'b100: tSub1BSxVF=0;
		3'b101: tSub1BSxVF=1;
		3'b110: tSub1BSxVF=1;
		3'b111: tSub1BSxVF=1;
	endcase
`else
	tSub1BVF=1'bX;
`endif

	case({regValRs[63], regValRt[63], tSub2SF})
		3'b000: tSub2SxVF=0;
		3'b001: tSub2SxVF=0;
		3'b010: tSub2SxVF=0;
		3'b011: tSub2SxVF=1;
		3'b100: tSub2SxVF=0;
		3'b101: tSub2SxVF=1;
		3'b110: tSub2SxVF=1;
		3'b111: tSub2SxVF=1;
	endcase
`endif

`endif


	tRegOutJcmpT = 0;

`ifdef jx2_alu_jcmp
	case(idUIxt[3:0])
		4'h0: tRegOutJcmpT = tSub2ZF;				/* EQ */
		4'h1: tRegOutJcmpT = !tSub2ZF;				/* NE */
		4'h4: tRegOutJcmpT = (tSub2SF^tSub2VF);		/* LT */
		4'h5: tRegOutJcmpT = !(tSub2SF^tSub2VF);	/* GE */
		4'h6: tRegOutJcmpT = !tSub2CF;				/* B  */
		4'h7: tRegOutJcmpT = tSub2CF;				/* HS */
		default: tRegOutJcmpT = 0;					/* - */
	endcase
`endif


	tResult1A=UV33_XX;
	tResult2A=UV65_XX;
	tResult1T=regInSrT;
	tResult2T=regInSrT;
	tResultw1T=regInSrT;
	tResultb1T=regInSrT;

	tResult1B=UV33_XX;
	tResult1S=regInSrS;

	tResult1W=UV33_XX;
	tResult2W=UV65_XX;

	tResultb1W = UV33_XX;
	tResultb2W = UV65_XX;

	tResult1P=regInSrP;
	tResult1Q=regInSrQ;
	tResult1R=regInSrR;
	tResult1O=regInSrO;

	tResultb1P=regInSrP;
	tResultb1Q=regInSrQ;
	tResultb1R=regInSrR;
	tResultb1O=regInSrO;
	
	tOpIsWx = (idUIxt[7:6] == 2'b11);

	case(idUIxt[3:0])
		4'h0: begin		/* ADD */
//			tResult1A=tAdd2A0;
//			tResult2A=tAdd3A0;
			tResult1A=tResult_Add32;
			tResult2A=tResult_Add64;
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			
`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = regInCarryD[0] ? tAdd3A1 : tAdd3A0;
//			end
`endif

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

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = regInCarryD[3] ? tSub3A1 : tSub3A0;
//			end
`endif

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

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = (regInSrT ? regInCarryD[1] : regInCarryD[0]) ?
//					tAdd3A1 : tAdd3A0;
//			end
`endif

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

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = (regInSrT ? regInCarryD[2] : regInCarryD[3]) ?
//					tSub3A1 : tSub3A0;
//			end
`endif

			tResult1B=regInSrS ? tSub2B0 : tSub2B1;
			tResult1S=!tResult1B[32];

			tResult2W = { 1'b0, regValRs[31:0], regValRt[63:32] };
		end
		
		4'h4: begin		/* TST */
			tResult1A=UV33_XX;
			tResult2A=UV65_XX;
			tResult1T=tTst1ZF;
			tResult2T=tTst2ZF;
			tResult1S=tTst1BZF;

			tResult1P=tTst1WZF_A;
			tResult1Q=tTst1WZF_A;
			tResult1R=tTst1WZF_A;
			tResult1O=tTst1WZF_A;
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
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=!tSub1ZF;
			tResult2T=!tSub2ZF;
			tResult1S=!tSub1BZF;

			tResult1P=!tSub1WZF_A;
			tResult1Q=!tSub1WZF_B;
			tResult1R=!tSub1WZF_C;
			tResult1O=!tSub1WZF_D;
			
			tResultw1T =
				tResult1P || tResult1Q ||
				tResult1R || tResult1O ;
			casez( { tResult1O, tResult1R, tResult1Q, tResult1P } )
				4'bzzz1: tResult1W = { 29'h0, 4'h0};
				4'bzz10: tResult1W = { 29'h0, 4'h1};
				4'bz100: tResult1W = { 29'h0, 4'h2};
				4'b1000: tResult1W = { 29'h0, 4'h3};
				4'b0000: tResult1W = { 29'h0, 4'h4};
			endcase

			tResultb1P=!tSub1BZF_A;
			tResultb1Q=!tSub1BZF_B;
			tResultb1R=!tSub1BZF_C;
			tResultb1O=!tSub1BZF_D;

			tResultb1T =
				!tSub1BZF_A || !tSub1BZF_B ||
				!tSub1BZF_C || !tSub1BZF_D ||
				!tSub1BZF_E || !tSub1BZF_F ||
				!tSub1BZF_G || !tSub1BZF_H ;
			casez( { 
					!tSub1BZF_H, !tSub1BZF_G, !tSub1BZF_F, !tSub1BZF_E,
					!tSub1BZF_D, !tSub1BZF_C, !tSub1BZF_B, !tSub1BZF_A } )
				8'bzzzzzzz1: tResultb1W = { 29'h0, 4'h0};
				8'bzzzzzz10: tResultb1W = { 29'h0, 4'h1};
				8'bzzzzz100: tResultb1W = { 29'h0, 4'h2};
				8'bzzzz1000: tResultb1W = { 29'h0, 4'h3};
				8'bzzz10000: tResultb1W = { 29'h0, 4'h4};
				8'bzz100000: tResultb1W = { 29'h0, 4'h5};
				8'bz1000000: tResultb1W = { 29'h0, 4'h6};
				8'b10000000: tResultb1W = { 29'h0, 4'h7};
				8'b00000000: tResultb1W = { 29'h0, 4'h8};
			endcase

//			tResultb1T =
//				tResultb1P || tResultb1Q ||
//				tResultb1R || tResultb1O ;
//			casez( { tResultb1O, tResultb1R, tResultb1Q, tResultb1P } )
//				4'bzzz1: tResultb1W = { 29'h0, 4'h0};
//				4'bzz10: tResultb1W = { 29'h0, 4'h1};
//				4'bz100: tResultb1W = { 29'h0, 4'h2};
//				4'b1000: tResultb1W = { 29'h0, 4'h3};
//				4'b0000: tResultb1W = { 29'h0, 4'h4};
//			endcase
		end
		4'h9: begin		/* CMPHS */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
//			tResult1T=!tSub1CF;
//			tResult2T=!tSub2CF;
//			tResult1S=!tSub1BCF;
			tResult1T=tSub1CF;
			tResult2T=tSub2CF;
			tResult1S=tSub1BCF;

			tResult1P=tSub1WCF_A;
			tResult1Q=tSub1WCF_B;
			tResult1R=tSub1WCF_C;
			tResult1O=tSub1WCF_D;
			
			tResultb1T	= tSubACF;
		end
		4'hA: begin		/* CMPGE */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
//			tResult1T=tSub1ZF || (tSub1SF^tSub1VF);
//			tResult2T=tSub2ZF || (tSub2SF^tSub2VF);
//			tResult1S=tSub1BZF || (tSub1BSF^tSub1BVF);
//			tResult1T=tSub1ZF || tSub1SxVF;
//			tResult2T=tSub2ZF || tSub2SxVF;
//			tResult1S=tSub1BZF || tSub1BSxVF;

			tResult1T=!(tSub1SF^tSub1VF);
			tResult2T=!(tSub2SF^tSub2VF);
			tResult1S=!(tSub1BSF^tSub1BVF);

			tResult1P=!tSub1WSF_A || tSub1WZF_A;
			tResult1Q=!tSub1WSF_B || tSub1WZF_B;
			tResult1R=!tSub1WSF_C || tSub1WZF_C;
			tResult1O=!tSub1WSF_D || tSub1WZF_D;

			tResultb1T	= tSubAZF;		//Array Equal

		end

		4'hB: begin		/* SLTxx */
`ifdef jx2_enable_riscv
//			tResult1T=tSub1CF && !tSub1ZF;

			if(idUIxt[4])
				tResult2T=!(tSub2CF && !tSub2ZF);
			else
				tResult2T=(tSub2SF^tSub2VF);

			tResult1A= { UV32_00, tResult2T };
			tResult2A= { UV64_00, tResult2T };
`endif
		end

`ifndef def_true
		4'hB: begin		/* NOR */
//			tResult1A={1'b0, ~(regValRs[31: 0] | regValRt[31: 0])};
//			tResult2A={1'b0, ~(regValRs[63:32] | regValRt[63:32]),
//				tResult1A[31:0]};
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;

//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;
		end
`endif

		4'hC: begin		/* CMPEQ */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=tSub1ZF;
			tResult2T=tSub2ZF;
			tResult1S=tSub1BZF;

`ifdef jx2_alu_wx
			if(tOpIsWx)
			begin
				tResult2T = tSub2ZF && regInCarryD[4];
			end
`endif

			tResult1P=tSub1WZF_A;
			tResult1Q=tSub1WZF_B;
			tResult1R=tSub1WZF_C;
			tResult1O=tSub1WZF_D;
			
			casez( { tResult1O, tResult1R, tResult1Q, tResult1P } )
				4'bzzz1: tResult1W = { 29'h0, 4'h0};
				4'bzz10: tResult1W = { 29'h0, 4'h1};
				4'bz100: tResult1W = { 29'h0, 4'h2};
				4'b1000: tResult1W = { 29'h0, 4'h3};
				4'b0000: tResult1W = { 29'h0, 4'h4};
			endcase

`ifndef def_true
			tResultb1P=tSub1BZF_A;
			tResultb1Q=tSub1BZF_B;
			tResultb1R=tSub1BZF_C;
			tResultb1O=tSub1BZF_D;
			tResultb1T =
				tResultb1P || tResultb1Q ||
				tResultb1R || tResultb1O ;
			casez( { tResultb1O, tResultb1R, tResultb1Q, tResultb1P } )
				4'bzzz1: tResultb1W = { 29'h0, 4'h0};
				4'bzz10: tResultb1W = { 29'h0, 4'h1};
				4'bz100: tResultb1W = { 29'h0, 4'h2};
				4'b1000: tResultb1W = { 29'h0, 4'h3};
				4'b0000: tResultb1W = { 29'h0, 4'h4};
			endcase
`endif

`ifdef def_true
			tResultb1P=tSub1BZF_A;
			tResultb1Q=tSub1BZF_B;
			tResultb1R=tSub1BZF_C;
			tResultb1O=tSub1BZF_D;
			tResultb1T =
				tSub1BZF_A || tSub1BZF_B ||
				tSub1BZF_C || tSub1BZF_D ||
				tSub1BZF_E || tSub1BZF_F ||
				tSub1BZF_G || tSub1BZF_H ;
			casez( { 
					tSub1BZF_H, tSub1BZF_G, tSub1BZF_F, tSub1BZF_E,
					tSub1BZF_D, tSub1BZF_C, tSub1BZF_B, tSub1BZF_A } )
				8'bzzzzzzz1: tResultb1W = { 29'h0, 4'h0};
				8'bzzzzzz10: tResultb1W = { 29'h0, 4'h1};
				8'bzzzzz100: tResultb1W = { 29'h0, 4'h2};
				8'bzzzz1000: tResultb1W = { 29'h0, 4'h3};
				8'bzzz10000: tResultb1W = { 29'h0, 4'h4};
				8'bzz100000: tResultb1W = { 29'h0, 4'h5};
				8'bz1000000: tResultb1W = { 29'h0, 4'h6};
				8'b10000000: tResultb1W = { 29'h0, 4'h7};
				8'b00000000: tResultb1W = { 29'h0, 4'h8};
			endcase
`endif

		end
		4'hD: begin		/* CMPHI */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
//			tResult1T=!tSub1CF && !tSub1ZF;
//			tResult2T=!tSub2CF && !tSub2ZF;
//			tResult1S=!tSub1BCF && !tSub1BZF;

			tResult1T=tSub1CF && !tSub1ZF;
			tResult2T=tSub2CF && !tSub2ZF;
			tResult1S=tSub1BCF && !tSub1BZF;

`ifdef jx2_alu_wx
			if(tOpIsWx)
			begin
				tResult2T = tSub2CF ||
					(tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));
			end
`endif

			tResult1P=tSub1WCF_A && !tSub1WZF_A;
			tResult1Q=tSub1WCF_B && !tSub1WZF_B;
			tResult1R=tSub1WCF_C && !tSub1WZF_C;
			tResult1O=tSub1WCF_D && !tSub1WZF_D;

			tResultb1T	= tSubACF && !tSubAZF;
		end
		4'hE: begin		/* CMPGT */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;

//			tResult1T=(tSub1SF^tSub1VF);
//			tResult2T=(tSub2SF^tSub2VF);
//			tResult1S=(tSub1BSF^tSub1BVF);

			tResult1T=!tSub1ZF && !(tSub1SF^tSub1VF);
			tResult2T=!tSub2ZF && !(tSub2SF^tSub2VF);
			tResult1S=!tSub1BZF && !(tSub1BSF^tSub1BVF);

`ifdef jx2_alu_wx
			if(tOpIsWx)
			begin
				tResult2T = (!tSub2ZF && !(tSub2SF^tSub2VF)) ||
					(tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));
			end
`endif

			tResult1P=!tSub1WSF_A && !tSub1WZF_A;
			tResult1Q=!tSub1WSF_B && !tSub1WZF_B;
			tResult1R=!tSub1WSF_C && !tSub1WZF_C;
			tResult1O=!tSub1WSF_D && !tSub1WZF_D;

//			tResult1T=tSub1SxVF;
//			tResult2T=tSub2SxVF;
//			tResult1S=tSub1BSxVF;

			tResultb1T	= tSubTZF;		//Tag Equal
		end
		4'hF: begin		/* CSELT */
			tResult1A={1'b0, regInSrT ? regValRs[31: 0] : regValRt[31: 0] };
			tResult2A={1'b0, regInSrT ? regValRs[63: 0] : regValRt[63: 0] };
			tResult1B={1'b0, regInSrS ? regValRs[31: 0] : regValRt[31: 0] };

			tResult2W = { 1'b0,
				regInSrO ? regValRs[63:48] : regValRt[63:48],
				regInSrR ? regValRs[47:32] : regValRt[47:32],
				regInSrQ ? regValRs[31:16] : regValRt[31:16],
				regInSrP ? regValRs[15: 0] : regValRt[15: 0] };

`ifdef jx2_enable_bitsel
			tResultb2W = { 1'b0,
				(regValRs &   regValRt ) |
				(regValRp & (~regValRt))
				};
`endif
		end
	endcase

`ifdef jx2_enable_aluunary
	tResultu1A=0;
	tResultu1B=0;
	tResultu2A=0;

	casez(idUIxt[3:0])
`ifdef jx2_enable_clz
//		4'b00zz: begin
		4'b000z: begin
			tResultu1A = { UV25_00, tClzVal };
			tResultu2A = { UV57_00, tClzVal };
		end
//		4'b01zz: begin
//		4'b001z: begin
		4'b0010: begin
			tResultu1A = { 1'b0, tClzRsVal2[63:32] };
			tResultu2A = { 1'b0, tClzRsVal2[63: 0] };
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

`ifdef jx2_enable_gsv
//	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	if(	(idUCmd[5:0]==JX2_UCMD_ALUW3) ||
		(idUCmd[5:0]==JX2_UCMD_ALUCMPW))
	begin
//		tResult1A	= tResultu1A;
//		tResult1B	= tResultu1B;
		tResult1A	= tResult1W;
//		tResult1B	= tResult2W[63:32];
		tResult2A	= tResult2W;
		
		tResult1T	= tResultw1T;
	end

//	if(idUCmd[5:0]==JX2_UCMD_ALUB3)
	if(	(idUCmd[5:0]==JX2_UCMD_ALUB3) ||
		(idUCmd[5:0]==JX2_UCMD_ALUCMPB))
	begin
		tResult1A	= tResultb1W;
		tResult2A	= tResultb2W;
		
		tResult1T	= tResultb1T;

		tResult1P	= tResultb1P;
		tResult1Q	= tResultb1Q;
		tResult1R	= tResultb1R;
		tResult1O	= tResultb1O;
	end
`endif

`ifdef jx2_fcmp_alu
	if(idUCmd[5:0]==JX2_UCMD_FCMP)
	begin
	
//		$display("Rs=%X Rt=%X SgA=%d SgB=%d SZ=%d ZF=%d   Gt=%d",
//			regValRs, regValRt,
//			regValRs[63], regValRt[63], tSub2SF, tSub2ZF,
//			tFCmpGtP);
	
		if(idUIxt[3:0]==JX2_UCIX_FPU_CMPEQ[3:0])
		begin
			tResult1T	= tFCmpEqP;
			tResult2T	= tFCmpEqP;

`ifdef jx2_alu_wx
			if(idUIxt[5:4]==2'b10)
			begin
				tResult1T	= tFCmpxEqP;
				tResult2T	= tFCmpxEqP;
			end
`endif
			
			if(idUIxt[5:4]==2'b11)
			begin
				tResult1T	= tSub1ZF;
				tResult1S	= tSub1BZF;
				tResult1P	= tSub1WZF_A;
				tResult1Q	= tSub1WZF_B;
				tResult1R	= tSub1WZF_C;
				tResult1O	= tSub1WZF_D;
			end
		end
		else
		begin
			tResult1T = tFCmpGtP;
			tResult2T = tFCmpGtP;
			tResult1S = tSub2ZF;

`ifdef jx2_alu_wx
			if(idUIxt[5:4]==2'b10)
			begin
				tResult1T	= tFCmpxGtP;
				tResult2T	= tFCmpxGtP;
			end
`endif

			if(idUIxt[5:4]==2'b11)
			begin
				tResult1T	= tFCmpGtP_FA;
				tResult1S	= tFCmpGtP_FB;
				tResult1P	= tFCmpGtP_HA;
				tResult1Q	= tFCmpGtP_HB;
				tResult1R	= tFCmpGtP_HC;
				tResult1O	= tFCmpGtP_HD;
			end
		end
	end
`endif

`ifdef jx2_enable_conv2_alu
	case(idUIxt[5:0])

		default: begin
			tRegConvVal = UV64_XX;
		end

	endcase
`endif

	tRegOutSrP = regInSrP;
	tRegOutSrQ = regInSrQ;
	tRegOutSrR = regInSrR;
	tRegOutSrO = regInSrO;

`ifdef jx2_enable_gsv
	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	begin
		if(idUIxt[3])
		begin
			tRegOutSrP = tResult1P;
			tRegOutSrQ = tResult1Q;
			tRegOutSrR = tResult1R;
			tRegOutSrO = tResult1O;
		end
	end
`endif

	if(idUIxt[5])
	begin
`ifdef jx2_enable_gsv
//		if(idUIxt[4])
		if(idUIxt[4] && (idUCmd[5:0]==JX2_UCMD_ALU3))
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
		tRegOutVal = {
			(tResult1A[31] && !idUIxt[4]) ? UV32_FF : UV32_00,
			tResult1A[31:0] };

		tRegOutSrT = tResult1T;
		tRegOutSrS = regInSrS;
	end

`ifdef jx2_enable_conv2_alu
	if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
	begin
		tRegOutVal = tRegConvVal;
	end	
`endif

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
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		idUIxt2			<= idUIxt;
		tRegOutVal2		<= tRegOutVal;
		tRegOutSrT2		<= tRegOutSrT;
		tRegOutSrS2		<= tRegOutSrS;

		tRegOutSrP2		<= tRegOutSrP;
		tRegOutSrQ2		<= tRegOutSrQ;
		tRegOutSrR2		<= tRegOutSrR;
		tRegOutSrO2		<= tRegOutSrO;
	end
end

endmodule

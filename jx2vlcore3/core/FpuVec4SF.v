/*
4-Wide SIMD Unit (Single / Half).
Performs SIMD faster but at lower accuracy than with the main FPU.
 */

`ifndef HAS_FPUVEC4SF
`define HAS_FPUVEC4SF

`include "CoreDefs.v"

`include "FpuAddSF.v"
`include "FpuMulSF.v"

`include "FpuConvH2S.v"
`include "FpuConvS2H.v"

`ifdef jx2_ena_fpu_v2sd
`include "FpuConvS2D.v"
`include "FpuConvD2SA.v"
`endif

`ifdef jx2_use_imm_shuffle
`include "FpuVecShMux4.v"
`endif

module FpuVec4SF(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmdA,		regIdIxtA,
	opCmdB,		regIdIxtB,
	opCmdC,		regIdIxtC,
	exHold,

	regValImmA,	regValImmB,

	regIdRsA,	regValRsA,
	regIdRsB,	regValRsB,
	regIdRtA,	regValRtA,
	regIdRtB,	regValRtB,
	regValRpA,	regValRpB,
	
	regValRnA,	regValRnB
	);

input	clock;
input	reset;
input	exHold;

input[8:0]		opCmdA;			//command opcode
input[8:0]		regIdIxtA;		//ALU Index / Opcode Extension
input[8:0]		opCmdB;			//command opcode
input[8:0]		regIdIxtB;		//ALU Index / Opcode Extension
input[8:0]		opCmdC;			//command opcode
input[8:0]		regIdIxtC;		//ALU Index / Opcode Extension

input[32:0]		regValImmA;
input[32:0]		regValImmB;

input[63:0]		regValRsA;
input[63:0]		regValRsB;
input[63:0]		regValRtA;
input[63:0]		regValRtB;
input[63:0]		regValRpA;
input[63:0]		regValRpB;

`input_gpr		regIdRsA;
`input_gpr		regIdRsB;
`input_gpr		regIdRtA;
`input_gpr		regIdRtB;

output[63:0]	regValRnA;
output[63:0]	regValRnB;

wire[63:0]		tRegValRnA;
wire[63:0]		tRegValRnB;
assign	regValRnA	= tRegValRnA;
assign	regValRnB	= tRegValRnB;

wire			opCmdLaneB;
reg				opCmdLaneB1;
reg				opCmdLaneB2;

assign		opCmdLaneB = 
	(opCmdB[5:0] == JX2_UCMD_FPUV4SF) &&
	(opCmdA[5:0] != JX2_UCMD_FPUV4SF) ;

wire[8:0]		opCmd;
wire[8:0]		regIdIxt;
wire[7:0]		regRMode;
`wire_gpr		regIdRt;
wire			regIdRtIsHalf;

assign		opCmd		= opCmdLaneB ? opCmdB : opCmdA;
assign		regIdIxt	= opCmdLaneB ? regIdIxtB : regIdIxtA;
assign		regRMode	= opCmdLaneB ? regValImmB[7:0] : regValImmA[7:0];
assign		regIdRt		= opCmdLaneB ? regIdRtB : regIdRtA;

`ifdef jx2_use_imm_vec4h
assign		regIdRtIsHalf	=
	(regIdRt == JX2_GR_FPIMM56VF)	||
	(regIdRt == JX2_GR_JIMM)		;
`else
assign		regIdRtIsHalf	= 0;
`endif

reg[8:0]		opCmd1;
reg[8:0]		regIdIxt1;
reg[8:0]		opCmd2;
reg[8:0]		regIdIxt2;
reg[8:0]		opCmd3;
reg[8:0]		regIdIxt3;

wire[3:0]		regExOp;
assign		regExOp =
	(regIdIxt[3:0]==0) ? 4'h1 :
	(regIdIxt[3:0]==1) ? 4'h2 :

	(regIdIxt[3:0]==5) ? 4'h1 :
	(regIdIxt[3:0]==6) ? 4'h2 :
	(regIdIxt[3:0]==7) ? 4'h8 :
	4'h0;

wire[63:0]		regValRsH;
wire[63:0]		regValRtH;
assign		regValRsH = opCmdLaneB ? regValRsB : regValRsA;
assign		regValRtH = opCmdLaneB ? regValRtB : regValRtA;

wire[31:0]		fpConvValRsA;
wire[31:0]		fpConvValRsB;
wire[31:0]		fpConvValRsC;
wire[31:0]		fpConvValRsD;
FpuConvH2S	fpConvRsA(regValRsH[15: 0], fpConvValRsA);
FpuConvH2S	fpConvRsB(regValRsH[31:16], fpConvValRsB);
FpuConvH2S	fpConvRsC(regValRsH[47:32], fpConvValRsC);
FpuConvH2S	fpConvRsD(regValRsH[63:48], fpConvValRsD);

wire[31:0]		fpConvValRtA;
wire[31:0]		fpConvValRtB;
wire[31:0]		fpConvValRtC;
wire[31:0]		fpConvValRtD;
FpuConvH2S	fpConvRtA(regValRtH[15: 0], fpConvValRtA);
FpuConvH2S	fpConvRtB(regValRtH[31:16], fpConvValRtB);
FpuConvH2S	fpConvRtC(regValRtH[47:32], fpConvValRtC);
FpuConvH2S	fpConvRtD(regValRtH[63:48], fpConvValRtD);


`ifdef jx2_ena_fpu_v2sd

wire[31:0]		fpConvValDblRsB;
wire[31:0]		fpConvValDblRsD;
wire[31:0]		fpConvValDblRtB;
wire[31:0]		fpConvValDblRtD;

FpuConvD2SA	fpConvDblRsA(regValRsA, fpConvValDblRsB);
FpuConvD2SA	fpConvDblRsB(regValRsB, fpConvValDblRsD);
FpuConvD2SA	fpConvDblRtA(regValRtA, fpConvValDblRtB);
FpuConvD2SA	fpConvDblRtB(regValRtB, fpConvValDblRtD);

`endif


wire			tVecIsHalf;
wire			tVecIsDbl;
assign		tVecIsHalf = regIdIxt[5:4]==2'b01;
// assign		tVecIsDbl = regIdIxt[5:4]==2'b11;
assign		tVecIsDbl = 
	(regIdIxt[5:4]==2'b11) || (regIdIxt[5:2]==4'b0000);

wire[31:0]		fpValRsA;
wire[31:0]		fpValRsB;
wire[31:0]		fpValRsC;
wire[31:0]		fpValRsD;
assign		fpValRsA = tVecIsHalf ? fpConvValRsA : regValRsA[31: 0];
// assign		fpValRsB = tVecIsHalf ? fpConvValRsB : regValRsA[63:32];
assign		fpValRsC = tVecIsHalf ? fpConvValRsC : regValRsB[31: 0];
// assign		fpValRsD = tVecIsHalf ? fpConvValRsD : regValRsB[63:32];

wire[31:0]		fpValRtA;
wire[31:0]		fpValRtB;
wire[31:0]		fpValRtC;
wire[31:0]		fpValRtD;
// assign		fpValRtA = tVecIsHalf ? fpConvValRtA : regValRtA[31: 0];
// assign		fpValRtB = tVecIsHalf ? fpConvValRtB : regValRtA[63:32];
// assign		fpValRtC = tVecIsHalf ? fpConvValRtC : regValRtB[31: 0];
// assign		fpValRtD = tVecIsHalf ? fpConvValRtD : regValRtB[63:32];

assign		fpValRtA = (tVecIsHalf || regIdRtIsHalf) ?
	fpConvValRtA : regValRtA[31: 0];
assign		fpValRtC = (tVecIsHalf || regIdRtIsHalf) ?
	fpConvValRtC : regValRtB[31: 0];

`ifdef jx2_ena_fpu_v2sd

assign		fpValRsB =
	tVecIsHalf ? fpConvValRsB :
	tVecIsDbl ? fpConvValDblRsB :
	regValRsA[63:32];
assign		fpValRsD =
	tVecIsHalf ? fpConvValRsD :
	tVecIsDbl ? fpConvValDblRsD :
	regValRsB[63:32];

assign		fpValRtB =
	(tVecIsHalf || regIdRtIsHalf) ? fpConvValRtB :
	tVecIsDbl ? fpConvValDblRtB :
	regValRtA[63:32];
assign		fpValRtD =
	(tVecIsHalf || regIdRtIsHalf) ? fpConvValRtD :
	tVecIsDbl ? fpConvValDblRtD :
	regValRtB[63:32];

`else

assign		fpValRsB = tVecIsHalf ? fpConvValRsB : regValRsA[63:32];
assign		fpValRsD = tVecIsHalf ? fpConvValRsD : regValRsB[63:32];

//assign		fpValRtB = tVecIsHalf ? fpConvValRtB : regValRtA[63:32];
// assign		fpValRtD = tVecIsHalf ? fpConvValRtD : regValRtB[63:32];

assign		fpValRtB = (tVecIsHalf || regIdRtIsHalf) ?
	fpConvValRtB : regValRtA[63:32];
assign		fpValRtD = (tVecIsHalf || regIdRtIsHalf) ?
	fpConvValRtD : regValRtB[63:32];

`endif

wire[31:0]		fpValRs1A;
wire[31:0]		fpValRs1B;
wire[31:0]		fpValRs1C;
wire[31:0]		fpValRs1D;

wire[31:0]		fpValRt1A;
wire[31:0]		fpValRt1B;
wire[31:0]		fpValRt1C;
wire[31:0]		fpValRt1D;

`ifdef jx2_use_imm_shuffle

wire	doShuffleRs;
wire	doShuffleRt;

wire	chkShuffleValid;
assign	chkShuffleValid = regIdIxtC[8] &&
	(	(opCmdC == UV9_00)			||
		(opCmdC[8:6]==JX2_IXC_NV)	);

assign	doShuffleRs = chkShuffleValid &&
	((opCmdC[1:0]!=2'b00) ? opCmdC[0] : regIdRtIsHalf);
assign	doShuffleRt = chkShuffleValid &&
	((opCmdC[1:0]!=2'b00) ? opCmdC[1] : !regIdRtIsHalf);

FpuVecShMux4 fpShufRs1A(
	fpValRsA, fpValRsB, fpValRsC, fpValRsD,
	fpValRs1A,	regIdIxtC[1:0], 2'b00, doShuffleRs);
FpuVecShMux4 fpShufRs1B(
	fpValRsA, fpValRsB, fpValRsC, fpValRsD,
	fpValRs1B,	regIdIxtC[3:2], 2'b01, doShuffleRs);
FpuVecShMux4 fpShufRs1C(
	fpValRsA, fpValRsB, fpValRsC, fpValRsD,
	fpValRs1C,	regIdIxtC[5:4], 2'b10, doShuffleRs);
FpuVecShMux4 fpShufRs1D(
	fpValRsA, fpValRsB, fpValRsC, fpValRsD,
	fpValRs1D,	regIdIxtC[7:6], 2'b11, doShuffleRs);

FpuVecShMux4 fpShufRt1A(
	fpValRtA, fpValRtB, fpValRtC, fpValRtD,
	fpValRt1A,	regIdIxtC[1:0], 2'b00, doShuffleRt);
FpuVecShMux4 fpShufRt1B(
	fpValRtA, fpValRtB, fpValRtC, fpValRtD,
	fpValRt1B,	regIdIxtC[3:2], 2'b01, doShuffleRt);
FpuVecShMux4 fpShufRt1C(
	fpValRtA, fpValRtB, fpValRtC, fpValRtD,
	fpValRt1C,	regIdIxtC[5:4], 2'b10, doShuffleRt);
FpuVecShMux4 fpShufRt1D(
	fpValRtA, fpValRtB, fpValRtC, fpValRtD,
	fpValRt1D,	regIdIxtC[7:6], 2'b11, doShuffleRt);

`else

assign		fpValRs1A = fpValRsA;
assign		fpValRs1B = fpValRsB;
assign		fpValRs1C = fpValRsC;
assign		fpValRs1D = fpValRsD;

assign		fpValRt1A = fpValRtA;
assign		fpValRt1B = fpValRtB;
assign		fpValRt1C = fpValRtC;
assign		fpValRt1D = fpValRtD;

`endif

wire[31:0]		fpAddValRnA;
wire[31:0]		fpAddValRnB;
wire[31:0]		fpAddValRnC;
wire[31:0]		fpAddValRnD;

FpuAddSF	fpvAddA(clock, reset, exHold,
	fpValRs1A, fpValRt1A, fpAddValRnA,
	regExOp, regRMode);
FpuAddSF	fpvAddB(clock, reset, exHold,
	fpValRs1B, fpValRt1B, fpAddValRnB,
	regExOp, regRMode);
FpuAddSF	fpvAddC(clock, reset, exHold,
	fpValRs1C, fpValRt1C, fpAddValRnC,
	regExOp, regRMode);
FpuAddSF	fpvAddD(clock, reset, exHold,
	fpValRs1D, fpValRt1D, fpAddValRnD,
	regExOp, regRMode);

wire[31:0]		fpMulValRnA;
wire[31:0]		fpMulValRnB;
wire[31:0]		fpMulValRnC;
wire[31:0]		fpMulValRnD;

FpuMulSF	fpvMulA(clock, reset, exHold,
	fpValRs1A, fpValRt1A, fpMulValRnA,
	regExOp, regRMode);
FpuMulSF	fpvMulB(clock, reset, exHold,
	fpValRs1B, fpValRt1B, fpMulValRnB,
	regExOp, regRMode);
FpuMulSF	fpvMulC(clock, reset, exHold,
	fpValRs1C, fpValRt1C, fpMulValRnC,
	regExOp, regRMode);
FpuMulSF	fpvMulD(clock, reset, exHold,
	fpValRs1D, fpValRt1D, fpMulValRnD,
	regExOp, regRMode);


wire	tVecRnIsLive;
wire	tVecRnIsHalf;
wire	tVecRnIsDbl;
wire	tVecRnIsFmul;

wire	tVecRnWilLive;
wire	tVecRnWasLive;
assign		tVecRnWilLive = (opCmd1[5:0] == JX2_UCMD_FPUV4SF);
assign		tVecRnWasLive = (opCmd3[5:0] == JX2_UCMD_FPUV4SF);

assign		tVecRnIsLive = (opCmd2[5:0] == JX2_UCMD_FPUV4SF);
// assign		tVecRnIsHalf = regIdIxt2[4];
assign		tVecRnIsHalf	= (regIdIxt2[5:4]==2'b01);
// assign		tVecRnIsDbl		= (regIdIxt2[5:4]==2'b11);
assign		tVecRnIsDbl		=
	(regIdIxt2[5:4]==2'b11) || (regIdIxt2[5:2]==4'b0000);
// assign		tVecRnIsFmul	= (regIdIxt2[3:0] == 4'h7);
assign		tVecRnIsFmul	=
	(regIdIxt2[3:0] == 4'h2) ||
	(regIdIxt2[3:0] == 4'h7);

wire[31:0]		fpValRnA;
wire[31:0]		fpValRnB;
wire[31:0]		fpValRnC;
wire[31:0]		fpValRnD;

assign		fpValRnA = tVecRnIsFmul ? fpMulValRnA : fpAddValRnA;
assign		fpValRnB = tVecRnIsFmul ? fpMulValRnB : fpAddValRnB;
assign		fpValRnC = tVecRnIsFmul ? fpMulValRnC : fpAddValRnC;
assign		fpValRnD = tVecRnIsFmul ? fpMulValRnD : fpAddValRnD;

wire[63:0]		fpConvRnA;
FpuConvS2H	fpDoConvRnA(fpValRnA, fpConvRnA[15: 0]);
FpuConvS2H	fpDoConvRnB(fpValRnB, fpConvRnA[31:16]);
FpuConvS2H	fpDoConvRnC(fpValRnC, fpConvRnA[47:32]);
FpuConvS2H	fpDoConvRnD(fpValRnD, fpConvRnA[63:48]);

`ifdef jx2_ena_fpu_v2sd

wire[63:0]		fpConvDblRnA;
wire[63:0]		fpConvDblRnB;
FpuConvS2D	fpDoConvDblRnA(fpValRnB, fpConvDblRnA);
FpuConvS2D	fpDoConvDblRnB(fpValRnD, fpConvDblRnB);

assign	tRegValRnA =
	tVecRnIsHalf ? fpConvRnA : 
	tVecRnIsDbl ? fpConvDblRnA : 
	{ fpValRnB, fpValRnA };
assign	tRegValRnB =
	tVecRnIsHalf ? fpConvRnA :
	tVecRnIsDbl ? fpConvDblRnB :
	{ fpValRnD, fpValRnC };

`else

assign	tRegValRnA = tVecRnIsHalf ? fpConvRnA : { fpValRnB, fpValRnA };
assign	tRegValRnB = tVecRnIsHalf ? fpConvRnA : { fpValRnD, fpValRnC };

`endif

//assign	tRegValRnA =
//	tVecRnIsLive ? ( tVecRnIsHalf ? fpConvRnA : { fpValRnB, fpValRnA } ) :
//	tVecRnWasLive ? 64'hDDDDDDDDDDDDDDDD :
//	tVecRnWilLive ? 64'hAAAAAAAAAAAAAAAA :
//	64'h7777777777777777;

always @(posedge clock)
begin
	if(!exHold)
	begin
		opCmd1		<= opCmd;
		regIdIxt1	<= regIdIxt;
		opCmdLaneB1	<= opCmdLaneB;

		opCmd2		<= opCmd1;
		regIdIxt2	<= regIdIxt1;
		opCmdLaneB2	<= opCmdLaneB1;

		opCmd3		<= opCmd2;
		regIdIxt3	<= regIdIxt2;
	end
	
	if(tVecRnIsLive)
	begin
//		$display("tRegValRn = %X %X", tRegValRnA, tRegValRnB);
	end
end

endmodule


`endif

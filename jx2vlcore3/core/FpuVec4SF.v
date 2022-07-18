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

module FpuVec4SF(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmdA,		regIdIxtA,
	opCmdB,		regIdIxtB,
	exHold,

	regValImmA,	regValImmB,

	regValRsA,	regValRsB,
	regValRtA,	regValRtB,
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

input[32:0]		regValImmA;
input[32:0]		regValImmB;

input[63:0]		regValRsA;
input[63:0]		regValRsB;
input[63:0]		regValRtA;
input[63:0]		regValRtB;
input[63:0]		regValRpA;
input[63:0]		regValRpB;

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

assign		opCmd		= opCmdLaneB ? opCmdB : opCmdA;
assign		regIdIxt	= opCmdLaneB ? regIdIxtB : regIdIxtA;
assign		regRMode	= opCmdLaneB ? regValImmB[7:0] : regValImmA[7:0];

reg[8:0]		opCmd1;
reg[8:0]		regIdIxt1;
reg[8:0]		opCmd2;
reg[8:0]		regIdIxt2;
reg[8:0]		opCmd3;
reg[8:0]		regIdIxt3;

wire[3:0]		regExOp;
assign		regExOp =
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

wire			tVecIsHalf;
assign		tVecIsHalf = regIdIxt[4];

wire[31:0]		fpValRsA;
wire[31:0]		fpValRsB;
wire[31:0]		fpValRsC;
wire[31:0]		fpValRsD;
assign		fpValRsA = tVecIsHalf ? fpConvValRsA : regValRsA[31: 0];
assign		fpValRsB = tVecIsHalf ? fpConvValRsB : regValRsA[63:32];
assign		fpValRsC = tVecIsHalf ? fpConvValRsC : regValRsB[31: 0];
assign		fpValRsD = tVecIsHalf ? fpConvValRsD : regValRsB[63:32];

wire[31:0]		fpValRtA;
wire[31:0]		fpValRtB;
wire[31:0]		fpValRtC;
wire[31:0]		fpValRtD;
assign		fpValRtA = tVecIsHalf ? fpConvValRtA : regValRtA[31: 0];
assign		fpValRtB = tVecIsHalf ? fpConvValRtB : regValRtA[63:32];
assign		fpValRtC = tVecIsHalf ? fpConvValRtC : regValRtB[31: 0];
assign		fpValRtD = tVecIsHalf ? fpConvValRtD : regValRtB[63:32];

wire[31:0]		fpAddValRnA;
wire[31:0]		fpAddValRnB;
wire[31:0]		fpAddValRnC;
wire[31:0]		fpAddValRnD;

FpuAddSF	fpvAddA(clock, reset, exHold,
	fpValRsA, fpValRtA, fpAddValRnA,
	regExOp, regRMode);
FpuAddSF	fpvAddB(clock, reset, exHold,
	fpValRsB, fpValRtB, fpAddValRnB,
	regExOp, regRMode);
FpuAddSF	fpvAddC(clock, reset, exHold,
	fpValRsC, fpValRtC, fpAddValRnC,
	regExOp, regRMode);
FpuAddSF	fpvAddD(clock, reset, exHold,
	fpValRsD, fpValRtD, fpAddValRnD,
	regExOp, regRMode);

wire[31:0]		fpMulValRnA;
wire[31:0]		fpMulValRnB;
wire[31:0]		fpMulValRnC;
wire[31:0]		fpMulValRnD;

FpuMulSF	fpvMulA(clock, reset, exHold,
	fpValRsA, fpValRtA, fpMulValRnA,
	regExOp, regRMode);
FpuMulSF	fpvMulB(clock, reset, exHold,
	fpValRsB, fpValRtB, fpMulValRnB,
	regExOp, regRMode);
FpuMulSF	fpvMulC(clock, reset, exHold,
	fpValRsC, fpValRtC, fpMulValRnC,
	regExOp, regRMode);
FpuMulSF	fpvMulD(clock, reset, exHold,
	fpValRsD, fpValRtD, fpMulValRnD,
	regExOp, regRMode);


wire	tVecRnIsLive;
wire	tVecRnIsHalf;
wire	tVecRnIsFmul;

wire	tVecRnWilLive;
wire	tVecRnWasLive;
assign		tVecRnWilLive = (opCmd1[5:0] == JX2_UCMD_FPUV4SF);
assign		tVecRnWasLive = (opCmd3[5:0] == JX2_UCMD_FPUV4SF);

assign		tVecRnIsLive = (opCmd2[5:0] == JX2_UCMD_FPUV4SF);
assign		tVecRnIsHalf = regIdIxt2[4];
assign		tVecRnIsFmul = (regIdIxt2[3:0] == 4'h7);

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

assign	tRegValRnA = tVecRnIsHalf ? fpConvRnA : { fpValRnB, fpValRnA };
assign	tRegValRnB = tVecRnIsHalf ? fpConvRnA : { fpValRnD, fpValRnC };

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
	end
	
	if(tVecRnIsLive)
	begin
//		$display("tRegValRn = %X %X", tRegValRnA, tRegValRnB);
	end
end

endmodule


`endif

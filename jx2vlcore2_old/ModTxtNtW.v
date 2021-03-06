`define FBUF_EN64K

`include "ModNtsc.v"
`include "ModTxtMemW.v"
`include "ModFbTxtW.v"


module ModTxtNtW(clock, reset, pwmOut,
	busAddr, busInData, busOutData, busOpm, busOK);

/* verilator lint_off UNUSED */

input clock;
input reset;

output[3:0] 	pwmOut;
input[31:0]		busAddr;
input[31:0]		busInData;
output[31:0]	busOutData;
input[4:0]		busOpm;
output[1:0]		busOK;

wire	busOE;
wire	busWR;
assign	busOE = busOpm[3];
assign	busWR = busOpm[4];

wire[7:0]	pixCy;
wire[7:0]	pixCu;
wire[7:0]	pixCv;

wire[9:0]	pixPosX;
wire[9:0]	pixPosY;
wire		pixLineOdd;

wire[13:0] pixCellIx;
wire[255:0] cellData1;
// wire[31:0] cellData2;

wire[15:0] fontGlyph;
wire[63:0] fontData1;
wire[63:0] ctrlRegVal;

ModNtsc ntsc(
	clock,		reset,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,
	pwmOut,
	pixPosX,	pixPosY,
	pixLineOdd);

ModTxtMemW fbmem(clock, reset,
	pixCellIx, cellData1,
	fontGlyph, fontData1,
	ctrlRegVal,
	busAddr, busInData, busOutData, busOE, busWR, busOK);

ModFbTxtW fbcc(clock, reset,
	pixPosX,	pixPosY,
	pixCy,		pixCu,		pixCv,
	pixCellIx,	cellData1,
	fontGlyph,	fontData1,
	ctrlRegVal,	pixLineOdd);

endmodule

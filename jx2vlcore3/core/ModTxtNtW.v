`define FBUF_EN128K
// `define FBUF_EN64K

`define FBUF_ENBM		//Enable Bitmap Modes

`include "CoreDefs.v"

// `include "ModNtsc.v"
`include "ModVga.v"
`include "ModTxtMemW.v"
`include "ModFbTxtW.v"


module ModTxtNtW(clock, reset, pwmOut,
	busInData, busOutData, busAddr, busOpm, busOK);

/* verilator lint_off UNUSED */

input clock;
input reset;

output[15:0] 	pwmOut;
input[31:0]		busAddr;
// input[31:0]		busInData;
// output[31:0]	busOutData;
input[63:0]		busInData;
output[63:0]	busOutData;
input[4:0]		busOpm;
output[1:0]		busOK;

wire	busOE;
wire	busWR;
wire	busQW;
assign	busOE = busOpm[3];
assign	busWR = busOpm[4];
assign	busQW = (busOpm[2:0] == 3'b011);

wire[7:0]	pixCy;
wire[7:0]	pixCu;
wire[7:0]	pixCv;
wire[15:0]	pixAux;

wire[11:0]	pixPosX;
wire[11:0]	pixPosY;
wire		pixLineOdd;

wire[13:0] pixCellIx;
wire[255:0] cellData1;
// wire[31:0] cellData2;

wire[15:0] fontGlyph;
wire[63:0] fontData1;
wire[63:0] ctrlRegVal;

// ModNtsc ntsc(
ModVga fbvga(
	clock,		reset,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,
	pixPosX,	pixPosY,
	pixLineOdd);

ModTxtMemW fbmem(clock, reset,
	pixCellIx, cellData1,
	fontGlyph, fontData1,
	ctrlRegVal,
	busAddr, busInData, busOutData, busOE, busWR, busQW, busOK);

ModFbTxtW fbcc(clock, reset,
	pixPosX,	pixPosY,
	pixCy,		pixCu,		pixCv,	pixAux,
	pixCellIx,	cellData1,
	fontGlyph,	fontData1,
	ctrlRegVal,	pixLineOdd);

endmodule

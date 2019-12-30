`define FBUF_EN128K
// `define FBUF_EN64K

`define FBUF_ENBM		//Enable Bitmap Modes

`include "CoreDefs.v"

// `include "ModNtsc.v"
`include "ModVga.v"
`include "ModTxtMemW.v"
`include "ModFbTxtW.v"


module ModTxtNtW(clock, reset, pwmOut,
	busInData, busOutData, busAddr, busOpm, busOK,
	timerNoise, timer256Hz);

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
input			timerNoise;
input			timer256Hz;

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

reg[7:0]	tTimer8Acc;
reg[7:0]	tNxtTimer8Acc;
reg			tBlinkSlow;
reg			tBlinkFast;

// ModNtsc ntsc(
ModVga fbvga(
	clock,		reset,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,
	pixPosX,	pixPosY,
	pixLineOdd,	timerNoise);

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
	ctrlRegVal,	pixLineOdd,
	tBlinkSlow,	tBlinkFast);

always @*
begin
	tNxtTimer8Acc = tTimer8Acc;
	if(timer256Hz)
		tNxtTimer8Acc = tTimer8Acc + 1;

	tBlinkSlow = tTimer8Acc[7];
	tBlinkFast = tTimer8Acc[5];
end

always @(posedge clock)
begin
	tTimer8Acc	<= tNxtTimer8Acc;
end

endmodule

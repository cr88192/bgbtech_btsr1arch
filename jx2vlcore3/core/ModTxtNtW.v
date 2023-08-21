/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

// `define JX2_FBUF_EN128K
// `define JX2_FBUF_EN64K
// `define JX2_FBUF_ENBM		//Enable Bitmap Modes

// `define jx2_modvga_pix4x

`include "CoreDefs.v"

// `include "ModNtsc.v"
`include "ModVga.v"
`include "ModFbTxtW.v"

`ifndef jx2_mem_l2vram
`include "ModTxtMemW.v"
`endif

`ifdef jx2_mem_l2vram
`include "ringbus/RbiMemVramA.v"
`endif

`ifdef jx2_enable_edgewalk
`include "ModEdgeWalk.v"
`endif

module ModTxtNtW(clock, reset,
	clock_100,
	pwmOut,
	busInData, busOutData, busAddr, busOpm, busOK,
	timerNoise, timer256Hz,
	dbgLeds,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

/* verilator lint_off UNUSED */

input clock;
input reset;
input clock_100;

output[16:0] 	pwmOut;
input[31:0]		busAddr;
// input[31:0]		busInData;
// output[31:0]	busOutData;
input[63:0]		busInData;
output[63:0]	busOutData;
input[4:0]		busOpm;
output[1:0]		busOK;
input			timerNoise;
input			timer256Hz;

input[9:0]		dbgLeds;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

wire	busOE;
wire	busWR;
wire	busQW;
assign	busOE = busOpm[3];
assign	busWR = busOpm[4];
assign	busQW = (busOpm[2:0] == 3'b011);

//wire[7:0]	pixCy;
//wire[7:0]	pixCu;
//wire[7:0]	pixCv;

`ifdef jx2_modvga_pix4x
wire[31:0]	pixCy;
wire[31:0]	pixCu;
wire[31:0]	pixCv;
`else
wire[15:0]	pixCy;
wire[15:0]	pixCu;
wire[15:0]	pixCv;
`endif

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

wire[7:0]	palIndex;
wire[15:0]	palData;

reg[7:0]	tTimer8Acc;
reg[7:0]	tNxtTimer8Acc;
reg			tBlinkSlow;
reg			tBlinkFast;

// ModNtsc ntsc(
ModVga fbvga(
	clock,		reset,		clock_100,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,
	pixPosX,	pixPosY,
	pixLineOdd,	timerNoise);

`ifndef jx2_mem_l2vram
assign	memAddrOut	= memAddrIn;	//memory input address
assign	memDataOut	= memDataIn;	//memory input data
assign	memSeqOut	= memSeqIn;		//operation sequence
assign	memOpmOut	= memOpmIn;		//memory operation mode

ModTxtMemW fbmem(clock, reset,
	pixCellIx, cellData1,
	fontGlyph, fontData1,
	ctrlRegVal,
	busAddr, busInData, busOutData, busOE, busWR, busQW, busOK);
`endif

`ifdef jx2_mem_l2vram

wire[63:0]	edsDataIn;
wire[63:0]	edsDataOut;
wire[31:0]	edsAddr;
wire[4:0]	edsOpm;
wire[1:0]	edsOK;

wire[63:0]	ebusOutData;
wire[63:0]	fbusOutData;
wire[1:0]	ebusOK;
wire[1:0]	fbusOK;

assign	busOutData = (ebusOK!=0) ? ebusOutData : fbusOutData;
assign	busOK = (ebusOK!=0) ? ebusOK : fbusOK;

RbiMemVramA		fbmem(
	clock,			reset,

	busAddr,		busOpm,
	fbusOutData,	busInData,
	fbusOK,

	pixCellIx,		cellData1,
	fontGlyph,		fontData1,
	palIndex,		palData,
	ctrlRegVal,

	edsDataIn,		edsDataOut,
	edsAddr,		edsOpm,
	edsOK,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,

	unitNodeId
	);

`ifdef jx2_enable_edgewalk
ModEdgeWalk edgewalk(
	clock,			reset,
	edsDataIn,		edsDataOut,
	edsAddr,		edsOpm,
	edsOK,

	busInData,		ebusOutData,
	busAddr,		busOpm,
	ebusOK
	);
`else

assign	edsDataOut = 0;
assign	edsAddr = 0;
assign	edsOpm = 0;

assign	ebusOutData = 0;
assign	ebusOK		= 0;

`endif

`endif


ModFbTxtW fbcc(clock, reset,
	pixPosX,	pixPosY,
	pixCy,		pixCu,		pixCv,	pixAux,
	pixCellIx,	cellData1,
	fontGlyph,	fontData1,
	palIndex,	palData,
	ctrlRegVal,	pixLineOdd,
	tBlinkSlow,	tBlinkFast,
	dbgLeds);

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

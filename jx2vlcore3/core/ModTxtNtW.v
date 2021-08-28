// `define JX2_FBUF_EN128K
// `define JX2_FBUF_EN64K
// `define JX2_FBUF_ENBM		//Enable Bitmap Modes

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

module ModTxtNtW(clock, reset, pwmOut,
	busInData, busOutData, busAddr, busOpm, busOK,
	timerNoise, timer256Hz,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

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

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
input [ 47:0]	memAddrIn;		//memory input address
output[ 47:0]	memAddrOut;		//memory output address
input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

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
RbiMemVramA		fbmem(
	clock,			reset,

	busAddr,		busOpm,
	busOutData,		busInData,
	busOK,

	pixCellIx,		cellData1,
	fontGlyph,		fontData1,
	ctrlRegVal,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,

	unitNodeId
	);
`endif


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

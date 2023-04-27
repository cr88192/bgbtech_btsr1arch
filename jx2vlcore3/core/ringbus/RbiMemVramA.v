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

/*

VRAM on the Ringbus.

This is a modified L1 cache which will either handle requests or provide a block for display.


Put the VRAM into C000_20A0000, which is assumed to be in the "wraparound" region located before the start of RAM.

For QWord access via MMIO, if the access is misaligned, then the low order bits are interpreted as high-order bits within an extended space, in units of 64K. This can allow access to framebuffer memory beyond the original 128K limit.

 */

`include "ringbus/RbiDefs.v"

module RbiMemVramA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutVal,		regInVal,
	regOutOK,

	pixCellIx,		cellData,
	fontGlyph,		fontData,
	ctrlRegVal,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

input [31: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;		//operation mode

output[63: 0]	regOutVal;		//output data value (Low 128 / Lane A)
input [63: 0]	regInVal;		//input data value (Low 128 / Lane A)
output[1:0]		regOutOK;


input[13:0]		pixCellIx;
output[255:0]	cellData;

input[15:0]		fontGlyph;
output[63:0]	fontData;

output[63:0]	ctrlRegVal;


input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[63: 0]	tRegOutVal;
reg[63: 0]	tRegOutValL;
assign	regOutVal = tRegOutVal;

reg[1:0]	tRegOutOK;
reg[1:0]	tRegOutOKL;
assign	regOutOK = tRegOutOK;

reg[255:0]	tCellData;
reg[255:0]	tCellDataL;
// assign	cellData = tCellData;
assign	cellData = tCellDataL;
// assign	cellData[31:0] = 32'h0F300041;

reg[63:0]	tCtrlRegVal;
assign		ctrlRegVal = tCtrlRegVal;
// assign		ctrlRegVal = 64'h0000_0000_0000_0001;

reg			tRegOutHold;
reg			tRegOutHoldL;


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l2addr	tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

wire			memRingIsIdle;
wire			memRingIsResp;
assign		memRingIsIdle = (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		memRingIsResp =
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire			memRingIsRespOkLd;
wire			memRingIsRespOkSt;
wire			memRingIsRespOkLdA;
wire			memRingIsRespOkLdB;
wire			memRingIsRespOkLdC;
wire			memRingIsRespOkLdD;
wire			memRingIsRespOkStA;
wire			memRingIsRespOkStB;
assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkSt =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b10);

assign		memRingIsRespOkLdA =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkLdB =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b01);

assign		memRingIsRespOkLdC =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);
assign		memRingIsRespOkLdD =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b11);

assign		memRingIsRespOkStA =
	memRingIsRespOkSt &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkStB =
	memRingIsRespOkSt &&
	(memSeqIn[7:6] == 2'b01);

reg		tMemRingSkipResp;

/*
Addr:
  (71:68): Flush Rover
  (67:48): PA Bits
  (47: 5): VA Bits
  ( 4: 0): Access Flags
 */


`define			reg_vram_ix	reg[5:0]
reg[ 27:0]		arrMemAddrA[63:0];
reg[ 27:0]		arrMemAddrB[63:0];
reg[127:0]		arrMemDataA[63:0];
reg[127:0]		arrMemDataB[63:0];

`define			reg_vram_ix2	reg[7:0]
reg[ 27:0]		arrMemAddrC[255:0];
reg[ 27:0]		arrMemAddrD[255:0];
reg[127:0]		arrMemDataC[255:0];
reg[127:0]		arrMemDataD[255:0];



reg[ 27:0]		tArrMemAddrStA;
reg[ 27:0]		tArrMemAddrStB;
reg[127:0]		tArrMemDataStA;
reg[127:0]		tArrMemDataStB;
`reg_vram_ix	tArrMemIdxStA;
`reg_vram_ix	tArrMemIdxStB;
reg				tArrMemDoStA;
reg				tArrMemDoStB;
reg				tArrMemDidStA;
reg				tArrMemDidStB;

reg[ 27:0]		tArrMemAddrStC;
reg[ 27:0]		tArrMemAddrStD;
reg[127:0]		tArrMemDataStC;
reg[127:0]		tArrMemDataStD;
`reg_vram_ix2	tArrMemIdxStC;
`reg_vram_ix2	tArrMemIdxStD;

reg				tArrMemDoStC;
reg				tArrMemDoStD;
reg				tArrMemDidStC;
reg				tArrMemDidStD;

reg[ 27:0]		tArrMemDidStAddrA;
reg[ 27:0]		tArrMemDidStAddrB;
reg[127:0]		tArrMemDidStDataA;
reg[127:0]		tArrMemDidStDataB;

`reg_vram_ix	tReqSeqIdxArr[15:0];
reg[ 23:0]		tReqSeqVaArr[15:0];

`reg_vram_ix	tReqSeqIdx;
reg[ 23:0]		tReqSeqVa;

reg[  3:0]		tFlushRov;
reg[  3:0]		tNxtFlushRov;

reg[  3:0]		tFlushRovTlb;
reg[  3:0]		tNxtFlushRovTlb;

reg[ 27:0]		tNxtReqAddr;
`reg_vram_ix	tNxtReqIxA;
`reg_vram_ix	tNxtReqIxB;
reg[ 23:0]		tNxtReqAxA;
reg[ 23:0]		tNxtReqAxB;
reg[ 27:0]		tReqAddr;
`reg_vram_ix	tReqIxA;
`reg_vram_ix	tReqIxB;
reg[ 23:0]		tReqAxA;
reg[ 23:0]		tReqAxB;
`reg_vram_ix	tReq1IxA;
`reg_vram_ix	tReq1IxB;
`reg_vram_ix	tArrMemDidStIxA;
`reg_vram_ix	tArrMemDidStIxB;

reg[ 27:0]		tNxtReqAddrC;
`reg_vram_ix2	tNxtReqIxC;
`reg_vram_ix2	tNxtReqIxD;
reg[ 23:0]		tNxtReqAxC;
reg[ 23:0]		tNxtReqAxD;
reg[ 27:0]		tReqAddrC;
`reg_vram_ix2	tReqIxC;
`reg_vram_ix2	tReqIxD;
reg[ 23:0]		tReqAxC;
reg[ 23:0]		tReqAxD;

reg[  4:0]		tNxtReqBix;
reg[  4:0]		tReqBix;
reg[  5:0]		tNxtReqOpm;
reg[  5:0]		tReqOpm;

reg[63: 0]		tReqInValA;
reg[63: 0]		tNxtReqInValA;

reg				tReqReadyA;
reg				tReqReadyB;
reg				tReqReady;
reg				tReqReadyL;
reg				tReqReady2;
reg				tReq2Ready;

reg				tReqMissAddrA;
reg				tReqMissAddrB;
reg				tReqMissAddrC;
reg				tReqMissAddrD;
reg				tReqMissA;
reg				tReqMissB;
reg				tReqMissC;
reg				tReqMissD;
reg				tReqDoMissA;
reg				tReqDoMissB;
reg				tReqDoMissC;
reg				tReqDoMissD;
reg				tReqMiss;
reg				tReqWaitResp;
reg				tReqSx;

reg				tReqMissAL;
reg				tReqMissBL;
reg				tReqMissL;

reg				tReqFlushAddrA;
reg				tReqFlushAddrB;

reg				tReqNoReadA;
reg				tReqNoReadB;

reg				tReqNoCross;
reg				tReqMissSkipA;
reg				tReqMissSkipB;
reg				tReqMissNoSkip;
reg				tReqMissNoSkipL;
reg				tReq2NoCross;
reg				tReq2MissSkipA;
reg				tReq2MissSkipB;
reg				tReq2MissA;
reg				tReq2MissB;

reg				tNxtReqIsCSel;
reg				tNxtReqIsCRs;
reg				tNxtReqIsNz;

reg				tReqIsCSel;
reg				tReqIsCRs;
reg				tReqIsNz;

reg				tReq2IsCRs;
reg				tReq2IsNz;

reg[ 27:0]		tBlkMemAddrA;
reg[ 27:0]		tBlkMemAddrB;
reg[127:0]		tBlkMemDataA;
reg[127:0]		tBlkMemDataB;
`reg_vram_ix	tBlkMemIdxA;
`reg_vram_ix	tBlkMemIdxB;
reg				tBlkIsDirtyA;
reg				tBlkIsDirtyB;

reg[ 27:0]		tBlkMemAddrC;
reg[ 27:0]		tBlkMemAddrD;
reg[127:0]		tBlkMemDataC;
reg[127:0]		tBlkMemDataD;
`reg_vram_ix2	tBlkMemIdxC;
`reg_vram_ix2	tBlkMemIdxD;

reg[ 27:0]		tBlkMemAddr2A;
reg[ 27:0]		tBlkMemAddr2B;
reg[127:0]		tBlkMemData2A;
reg[127:0]		tBlkMemData2B;

reg[ 27:0]		tBlkMemAddr2C;
reg[ 27:0]		tBlkMemAddr2D;
reg[127:0]		tBlkMemData2C;
reg[127:0]		tBlkMemData2D;

reg[ 27:0]		tBlk2StoreAddrA;
reg[ 27:0]		tBlk2StoreAddrB;
reg[127:0]		tBlk2StoreDataA;
reg[127:0]		tBlk2StoreDataB;

reg[255:0]		tBlkExData0;
reg[127:0]		tBlkExData1;
reg[ 87:0]		tBlkExData2;
reg[ 71:0]		tBlkExData3;
reg[ 63:0]		tBlkExData4;
reg[ 63:0]		tBlkExData;

// reg[ 63:0]		tBlkExData;
// reg[ 63:0]		tBlkExDataB;

reg[255:0]		tBlkInsData0;
reg[127:0]		tBlkInsData1;
reg[ 87:0]		tBlkInsData2;
reg[ 71:0]		tBlkInsData3;
reg[ 63:0]		tBlkInsData4;

reg[127:0]		tBlk2MemDataA;
reg[127:0]		tBlk2MemDataB;
reg[ 27:0]		tBlk2MemAddrA;
reg[ 27:0]		tBlk2MemAddrB;

reg[255:0]		tBlk2InsData;
`reg_vram_ix	tReq2IxA;
`reg_vram_ix	tReq2IxB;
reg[ 23:0]		tReq2AxA;
reg[ 23:0]		tReq2AxB;
reg[  5:0]		tReq2Opm;
reg[  4:0]		tReq2Bix;

reg				tReq2StoreSticky;
reg				tNxtReq2StoreSticky;
reg				tReg2MissInterlockA;
reg				tReg2MissInterlockB;
reg				tMemArrInterlockA;
reg				tMemArrInterlockB;

reg				tReg2StoreFwA;
reg				tReg2StoreFwB;
reg				tMemArrFwA;
reg				tMemArrFwB;


reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l2addr	tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

reg[ 15:0]		tMemSeqReqL;
reg[ 15:0]		tMemOpmReqL;
`reg_l2addr	tMemAddrReqL;
`reg_tile		tMemDataReqL;

reg				tMemReqStA;			//Store A
reg				tMemReqStB;			//Store B
reg				tMemReqStC;			//Store C
reg				tMemReqStD;			//Store D
reg				tNxtMemReqStA;		//Store A
reg				tNxtMemReqStB;		//Store B
reg				tNxtMemReqStC;		//Store C
reg				tNxtMemReqStD;		//Store D
reg				tNxtMemReqStAL;		//Store A
reg				tNxtMemReqStBL;		//Store B

reg				tMemRespStA;		//Store A
reg				tMemRespStB;		//Store B
reg				tMemRespStC;		//Store C
reg				tMemRespStD;		//Store D
reg				tNxtMemRespStA;		//Store A
reg				tNxtMemRespStB;		//Store B
reg				tNxtMemRespStC;		//Store C
reg				tNxtMemRespStD;		//Store D

reg				tMemRespLdA;		//Store A
reg				tMemRespLdB;		//Store B
reg				tNxtMemRespLdA;		//Store A
reg				tNxtMemRespLdB;		//Store B

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
reg				tMemReqLdC;		//Load C
reg				tMemReqLdD;		//Load D

reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
reg				tNxtMemReqLdC;	//Load C
reg				tNxtMemReqLdD;	//Load D

reg				tNxtMemReqLdAL;	//Load A
reg				tNxtMemReqLdBL;	//Load B

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

`reg_vram_ix	tMemSeqIx;
reg[23:0]		tMemSeqVa;
`reg_vram_ix	tMemSeqIxL;
reg[23:0]		tMemSeqVaL;

reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)

reg[3:0]		tRegInAddrHb;

reg[27:0]		tRegInAddr;
reg[27:0]		tRegInAddrC;
reg[27:0]		tRegInAddrD;
reg[5:0]		tRegInOpm;

reg[13:0]		tPixCellIxC;
reg[13:0]		tPixCellIxD;
reg[13:0]		tPixCellIxLim;
reg[13:0]		tNxtPixCellIxLim;
reg[14:0]		tPixCellIxCi;

reg[13:0]		tPixCellIxCL;
reg[13:0]		tPixCellIxDL;


`ifndef jx2_fbuf_nofont
reg[63:0]	fontMem[255:0];
reg[63:0]	fontGfx1Mem[127:0];
reg[63:0]	fontGfx2Mem[127:0];
`endif

// reg[31:0]	fontRamA[511:0];
// reg[31:0]	fontRamB[511:0];
reg[31:0]	fontRamA[1023:0];
reg[31:0]	fontRamB[1023:0];

reg[31:0]	tFontRamStDataA;
reg[31:0]	tFontRamStDataB;
// reg[8:0]	tFontRamStIxA;
// reg[8:0]	tFontRamStIxB;
reg[9:0]	tFontRamStIxA;
reg[9:0]	tFontRamStIxB;
reg			tFontRamDoStA;
reg			tFontRamDoStB;

reg[31:0]	scrRegCtrl0;	//Control Registers
reg[31:0]	scrRegCtrl1;	//Control Registers
reg[31:0]	scrRegCtrl2;	//Control Registers
reg[31:0]	scrRegCtrl3;	//Control Registers
reg[31:0]	scrRegCtrl4;	//Control Registers
reg[31:0]	scrRegCtrl5;	//Control Registers
reg[31:0]	scrRegCtrl6;	//Control Registers
reg[31:0]	scrRegCtrl7;	//Control Registers
reg[31:0]	scrRegCtrl8;	//Control Registers
reg[31:0]	scrRegCtrl9;	//Control Registers

reg[31:0]	nxtScrRegCtrl0;	//Control Registers
reg[31:0]	nxtScrRegCtrl1;	//Control Registers
reg[31:0]	nxtScrRegCtrl2;	//Control Registers
reg[31:0]	nxtScrRegCtrl3;	//Control Registers
reg[31:0]	nxtScrRegCtrl4;	//Control Registers
reg[31:0]	nxtScrRegCtrl5;	//Control Registers
reg[31:0]	nxtScrRegCtrl6;	//Control Registers
reg[31:0]	nxtScrRegCtrl7;	//Control Registers
reg[31:0]	nxtScrRegCtrl8;	//Control Registers
reg[31:0]	nxtScrRegCtrl9;	//Control Registers

reg[31:0]	tScrRegCtrl;

reg[11:0]	tHoldCyc;
reg[11:0]	tNxtHoldCyc;

reg[3:0]	tProbeCyc;
reg[3:0]	tNxtProbeCyc;


reg[15:0]	tFontGlyph;
reg[63:0]	tFontData1;
reg[63:0]	tFontData2;
reg[63:0]	tFontDataAsc1;
reg[63:0]	tFontDataGfx1;
reg[63:0]	tFontDataGfx2;
reg[63:0]	tFontDataRam;


// assign fontData = 64'h55AA55AA55AA55AA;

assign fontData = tFontData1;
// assign fontData = tFontData2;

initial begin
`ifndef jx2_fbuf_nofont
	$readmemh("fontmem.txt", fontMem);
	$readmemh("gfxfont0.txt", fontGfx1Mem);
	$readmemh("gfxfont1.txt", fontGfx2Mem);
`endif
end

always @*
begin
	/* EX1 */

	tRegInAddr = regInAddr[27:0];
//	if(regInAddr[0])
//		tRegInAddr[17]=1;
//	if(regInAddr[1])
//		tRegInAddr[18]=1;
//	tRegInAddr[1:0] = 0;

	tRegInOpm = { regInOpm[4:3], 1'b0, regInOpm[2:0] };

	if(tRegInOpm[5:4] == 2'b00)
//		tRegInAddr = { 8'b0, 3'b101, pixCellIx[11:0], 5'h000 };
		tRegInAddr = { 8'h0A, 1'b0, pixCellIx[13:0], 5'h000 };

//	tNxtReqAddr		= tRegInAddr[27:0];
	tNxtReqBix		= tRegInAddr[4:0];
	tNxtReqOpm		= tRegInOpm;
	tNxtReqInValA	= regInVal;
	
	tRegInAddrHb = { 3'b000, tRegInAddr[16] };

//	if((tRegInOpm[2:0] == 3'b011) && (tRegInAddr[2:0]!=3'b000))
//	begin
//		tRegInAddrHb = { tRegInAddr[16], tRegInAddr[2:0] };
//	end

	tNxtReqAddr		= { 8'h0A, tRegInAddrHb, tRegInAddr[15:2], 2'b00 };

	if((tRegInOpm[2:0] == 3'b011) && scrRegCtrl0[16])
	begin
		tNxtReqAddr		= { 8'h0A, tRegInAddr[2],
			tRegInAddr[16:3], tRegInAddr[1:0], 3'b000 };
		tNxtReqBix		= { tRegInAddr[1:0], 3'b000 };
	end

	if(tRegInOpm[5:4] == 2'b00)
		tNxtReqAddr = tRegInAddr;

//	if(tRegInAddr[4])
	if(tNxtReqAddr[4])
	begin
		tNxtReqAxB = tNxtReqAddr[27:4];
		tNxtReqAxA = tNxtReqAxB + 1;
	end
	else
	begin
		tNxtReqAxA = tNxtReqAddr[27:4];
		tNxtReqAxB = tNxtReqAxA + 1;
	end
	
	tNxtReqIxA[5:0] = tNxtReqAxA[6:1];
	tNxtReqIxB[5:0] = tNxtReqAxB[6:1];

	tReq1IxA		= tNxtReqIxA;
	tReq1IxB		= tNxtReqIxB;

`ifdef def_true
	tNxtReqIsCSel		=
		(tRegInAddr[27:20] == 8'h00) &&
		(tRegInAddr[19:17] == 3'b101);
	tNxtReqIsCRs = tNxtReqIsCSel && (tRegInAddr[16:8] == 9'h1FF);
	
	tNxtReqIsNz		=
		(tRegInOpm[5:4] != 2'b00) && tNxtReqIsCSel &&
		!reset;
`endif

//	tPixCellIxLim = 80 * 25;
	tNxtPixCellIxLim = 80 * 26;

	case( { scrRegCtrl0[8], scrRegCtrl0[10], scrRegCtrl0[3:0] } )

		6'h00: tNxtPixCellIxLim =  40 * 25 + 112;
		6'h01: tNxtPixCellIxLim =  80 * 25 + 112;
		6'h02: tNxtPixCellIxLim = (40 * 25 + 112) >> 1;
		6'h03: tNxtPixCellIxLim = (80 * 25 + 112) >> 1;
		6'h04: tNxtPixCellIxLim =  40 * 50 + 112;
		6'h05: tNxtPixCellIxLim =  80 * 50 + 112;
		6'h06: tNxtPixCellIxLim = (40 * 50 + 112) >> 1;
		6'h07: tNxtPixCellIxLim = (80 * 50 + 112) >> 1;

		6'h08: tNxtPixCellIxLim = ( 50 * 25 + 112);
		6'h09: tNxtPixCellIxLim = (100 * 25 + 112);
		6'h0A: tNxtPixCellIxLim = ( 50 * 25 + 112) >> 1;
		6'h0B: tNxtPixCellIxLim = (100 * 25 + 112) >> 1;
		6'h0C: tNxtPixCellIxLim = ( 50 * 50 + 112);
		6'h0D: tNxtPixCellIxLim = (100 * 50 + 112);
		6'h0E: tNxtPixCellIxLim = ( 25 * 50 + 112) >> 1;
		6'h0F: tNxtPixCellIxLim = ( 50 * 50 + 112) >> 1;

		6'h10: tNxtPixCellIxLim = (40 * 25 + 112) >> 2;
		6'h11: tNxtPixCellIxLim = (80 * 25 + 112) >> 2;
		6'h12: tNxtPixCellIxLim = (40 * 25 + 112) >> 3;
		6'h13: tNxtPixCellIxLim = (80 * 25 + 112) >> 3;
		6'h14: tNxtPixCellIxLim = (40 * 50 + 112) >> 2;
		6'h15: tNxtPixCellIxLim = (80 * 50 + 112) >> 2;
		6'h16: tNxtPixCellIxLim = (40 * 50 + 112) >> 3;
		6'h17: tNxtPixCellIxLim = (80 * 50 + 112) >> 3;

		6'h18: tNxtPixCellIxLim = ( 50 * 25 + 112) >> 2;
		6'h19: tNxtPixCellIxLim = (100 * 25 + 112) >> 2;
		6'h1A: tNxtPixCellIxLim = ( 50 * 25 + 112) >> 3;
		6'h1B: tNxtPixCellIxLim = (100 * 25 + 112) >> 3;
		6'h1C: tNxtPixCellIxLim = ( 50 * 50 + 112) >> 2;
		6'h1D: tNxtPixCellIxLim = (100 * 50 + 112) >> 2;
		6'h1E: tNxtPixCellIxLim = ( 25 * 50 + 112) >> 3;
		6'h1F: tNxtPixCellIxLim = ( 50 * 50 + 112) >> 3;

		default: tNxtPixCellIxLim = 16383;

	endcase

	tNxtProbeCyc	= tProbeCyc + 1;

//	tPixCellIxC = pixCellIx + 14'h0080;
//	tPixCellIxC = pixCellIx + 14'h0040;
	tPixCellIxC = pixCellIx + { 7'h00, tProbeCyc[1:0], 5'h0 };
//	tPixCellIxC = pixCellIx + { 7'h00, tProbeCyc[2:0], 4'h0 };
//	tPixCellIxC = pixCellIx + { 7'h00, tProbeCyc[3:0], 3'h0 };
	tPixCellIxD = pixCellIx;
	
	tPixCellIxCi = { 1'b0, tPixCellIxC } - { 1'b0, tPixCellIxLim };
//	if(!tPixCellIxCi[14])
//		tPixCellIxC = tPixCellIxCi[13:0];

//	if(tPixCellIxC >= tPixCellIxLim)
//		tPixCellIxC = tPixCellIxC - tPixCellIxLim;

//	tRegInAddrC = { 8'b0, 3'b101, tPixCellIxC[11:0], 5'h000 };
//	tRegInAddrD = { 8'b0, 3'b101, tPixCellIxD[11:0], 5'h000 };

	tRegInAddrC = { 8'h0A, 1'b0, tPixCellIxC[13:0], 5'h000 };
	tRegInAddrD = { 8'h0A, 1'b0, tPixCellIxD[13:0], 5'h000 };

	tNxtReqAxC = tRegInAddrC[27:4];
//	tNxtReqAxD = tNxtReqAxC + 1;
	tNxtReqAxD = tRegInAddrD[27:4];
	tNxtReqIxC[7:0] = tNxtReqAxC[8:1];
//	tNxtReqIxD[7:0] = tNxtReqAxD[8:1];
	tNxtReqIxD[7:0] = tRegInAddrD[12:5];
end

always @*
begin
	nxtScrRegCtrl0	= scrRegCtrl0;
	nxtScrRegCtrl1	= scrRegCtrl1;
	nxtScrRegCtrl2	= scrRegCtrl2;
	nxtScrRegCtrl3	= scrRegCtrl3;
	nxtScrRegCtrl4	= scrRegCtrl4;
	nxtScrRegCtrl5	= scrRegCtrl5;
	nxtScrRegCtrl6	= scrRegCtrl6;
	nxtScrRegCtrl7	= scrRegCtrl7;
	nxtScrRegCtrl8	= scrRegCtrl8;
	nxtScrRegCtrl9	= scrRegCtrl9;
	
	if(reset)
	begin
		nxtScrRegCtrl0	= 0;
		nxtScrRegCtrl1	= 0;
	end

	tReqSeqIdx		= tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa		= tReqSeqVaArr[memSeqIn[3:0]];

	tCtrlRegVal		= { scrRegCtrl1, scrRegCtrl0 };

	tArrMemDoStA		= 0;
	tArrMemDoStB		= 0;
	tRegOutHold			= 0;
//	tRegOutWait			= 0;
	
	tNxtMemRespStA		= tMemRespStA;
	tNxtMemRespStB		= tMemRespStB;
	tNxtMemRespLdA		= tMemRespLdA;
	tNxtMemRespLdB		= tMemRespLdB;
	tMemRingSkipResp	= 0;

//	tRegOutExc			= 0;


	/* EX2 */

	tReg2MissInterlockA = 0;
	tReg2MissInterlockB = 0;
	tMemArrInterlockA	= 0;
	tMemArrInterlockB	= 0;
	tReg2StoreFwA		= 0;
	tReg2StoreFwB		= 0;
	tMemArrFwA			= 0;
	tMemArrFwB			= 0;

	tArrMemAddrStA	= 0;
	tArrMemDataStA	= 0;
	tArrMemIdxStA	= 0;
	tArrMemDoStA	= 0;
	
	tArrMemAddrStB	= 0;
	tArrMemDataStB	= 0;
	tArrMemIdxStB	= 0;
	tArrMemDoStB	= 0;
	
	tArrMemAddrStC	= 0;
	tArrMemDataStC	= 0;
	tArrMemIdxStC	= 0;
	tArrMemDoStC	= 0;
	
	tArrMemAddrStD	= 0;
	tArrMemDataStD	= 0;
	tArrMemIdxStD	= 0;
	tArrMemDoStD	= 0;
	
	if(tReq2IsNz && tReq2Opm[5] && !tReq2StoreSticky)
	begin
		tReg2MissInterlockA		= (tReqIxA == tReq2IxA) && !tReq2MissSkipA;
		tReg2MissInterlockB		= (tReqIxB == tReq2IxB) && !tReq2MissSkipB;
	end


	tReqNoCross		= 0;
	tReqMissSkipA	= 0;
	tReqMissSkipB	= 0;
	tReqMissNoSkip	= 0;

`ifndef def_true
	tReqIsCSel		=
		(tReqAddr[27:20] == 8'h00) &&
		(tReqAddr[19:17] == 3'b101);
	tReqIsCRs = tReqIsCSel && (tReqAddr[16:8] == 9'h1FF);
	
	tReqIsNz		=
		(tReqOpm[5:4] != 2'b00) && tReqIsCSel;
`endif

	tFontRamStDataA = tReqInValA[31:0];
	tFontRamStDataB = tReqInValA[31:0];
//	tFontRamStIxA	= tReqInValA[40:32];
//	tFontRamStIxB	= tReqInValA[40:32];
	tFontRamStIxA	= tReqInValA[41:32];
	tFontRamStIxB	= tReqInValA[41:32];
	tFontRamDoStA	= 0;
	tFontRamDoStB	= 0;

	case(tReqAddr[5:2])
		4'h0: tScrRegCtrl = scrRegCtrl0;
		4'h1: tScrRegCtrl = scrRegCtrl1;
		4'h2: tScrRegCtrl = scrRegCtrl2;
		4'h3: tScrRegCtrl = scrRegCtrl3;
		4'h4: tScrRegCtrl = scrRegCtrl4;
		4'h5: tScrRegCtrl = scrRegCtrl5;
		4'h6: tScrRegCtrl = scrRegCtrl6;
		4'h7: tScrRegCtrl = scrRegCtrl7;
		4'h8: tScrRegCtrl = scrRegCtrl8;
		4'h9: tScrRegCtrl = scrRegCtrl9;
		default:	tScrRegCtrl = UV32_00;
	endcase

	if(tReqIsNz && tReqIsCRs)
	begin
		if(tReqOpm[5])
		begin
//			$display("RbiMemVRam: Set Ctrl %X %X",
//				tReqAddr[7:2], tReqInValA[31:0]);
		
			case(tReqAddr[7:2])
				6'h00: nxtScrRegCtrl0 = tReqInValA[31:0];
				6'h01: nxtScrRegCtrl1 = tReqInValA[31:0];
				6'h02: nxtScrRegCtrl2 = tReqInValA[31:0];
				6'h03: nxtScrRegCtrl3 = tReqInValA[31:0];
				6'h04: nxtScrRegCtrl4 = tReqInValA[31:0];
				6'h05: nxtScrRegCtrl5 = tReqInValA[31:0];
				6'h06: nxtScrRegCtrl6 = tReqInValA[31:0];
				6'h07: nxtScrRegCtrl7 = tReqInValA[31:0];
				6'h08: nxtScrRegCtrl8 = tReqInValA[31:0];
				6'h09: nxtScrRegCtrl9 = tReqInValA[31:0];

				6'h0C: tFontRamDoStA = 1;
//				6'h0D: tFontRamDoStB = 1;
				6'h0E: tFontRamDoStB = 1;
				default: begin end
			endcase
		end
	end

	tReqNoCross		= 1;
	if(tReqIsNz)
	begin
		if(tReqBix[4])
			tReqMissSkipA	= 1;
		else
			tReqMissSkipB	= 1;
	end

	tReqReadyA	= (tBlkMemIdxA == tReqIxA);
	tReqReadyB	= (tBlkMemIdxB == tReqIxB);
	tReqReady	= tReqReadyA && tReqReadyB;

	tBlkMemData2A = tBlkMemDataA;
	tBlkMemData2B = tBlkMemDataB;
	tBlkMemAddr2A = tBlkMemAddrA;
	tBlkMemAddr2B = tBlkMemAddrB;

	tBlkMemData2C = tBlkMemDataC;
	tBlkMemData2D = tBlkMemDataD;
	tBlkMemAddr2C = tBlkMemAddrC;
	tBlkMemAddr2D = tBlkMemAddrD;

	tBlk2StoreAddrA = {
		tBlk2MemAddrA[27:5], 1'b1,
		tBlk2MemAddrA[3:0]};
	tBlk2StoreAddrB = {
		tBlk2MemAddrB[27:5], 1'b1,
		tBlk2MemAddrB[3:0]};

	if(tReq2Bix[4])
	begin
		tBlk2StoreDataA = tBlk2InsData[255:128];
		tBlk2StoreDataB = tBlk2InsData[127:  0];
	end else begin
		tBlk2StoreDataA = tBlk2InsData[127:  0];
		tBlk2StoreDataB = tBlk2InsData[255:128];
	end

`ifndef def_true
	if(tReg2MissInterlockA || tReg2MissInterlockB)
	begin
		tReqReady	= 0;
	end
`endif

	tMemArrInterlockA	= tArrMemDidStA && (tReqIxA == tArrMemDidStIxA);
	tMemArrInterlockB	= tArrMemDidStB && (tReqIxB == tArrMemDidStIxB);

`ifndef def_true
//	if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
	if(tMemArrInterlockA)
	begin
//		$display("VRAM: Line A Store Clash");
		tReqReady	= 0;
	end
//	if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
	if(tMemArrInterlockB)
	begin
//		$display("VRAM: Line B Store Clash");
		tReqReady	= 0;
	end
`endif

	if(tReqAxA[0])
		$display("VRAM: tReqAxA Even/Odd Error");
	if(!tReqAxB[0])
		$display("VRAM: tReqAxB Even/Odd Error");

	tReqReady2	= tReqReady && tReqReadyL;

	tBlkIsDirtyA	= tBlkMemAddr2A[4];
	tBlkIsDirtyB	= tBlkMemAddr2B[4];

	tReqMissAddrA	=
		 (tBlkMemAddr2A[27: 5] != tReqAxA[23: 1]);
	tReqMissAddrB	=
		 (tBlkMemAddr2B[27: 5] != tReqAxB[23: 1]);

	tReqMissAddrC	=
		 (tBlkMemAddr2C[27: 5] != tReqAxC[23: 1]);
	tReqMissAddrD	=
		 (tBlkMemAddr2D[27: 5] != tReqAxC[23: 1]);

	tReqMissA	= tReqMissAddrA && !tReqMissSkipA;
	tReqMissB	= tReqMissAddrB && !tReqMissSkipB;
	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz && !tReqIsCRs;

	tReqMissC	= tReqMissAddrC;
	tReqMissD	= tReqMissAddrD;

//	if(tReqIsNz && tReqIsCRs)
	if(!tReqIsNz || tReqIsCRs)
	begin
		tReqMiss	= 0;
		tReqMissA	= 0;
		tReqMissB	= 0;
	end

//	if(!tReqIsNz)
//		tReqMiss	= 0;

//	if(!tReqIsNz)
//	begin
//		tReqMissA	= 0;
//		tReqMissB	= 0;
//	end


	tReqDoMissA	= tReqMissA && tReqIsNz;
	tReqDoMissB	= tReqMissB && tReqIsNz;

	tReqDoMissC	= tReqMissC;
	tReqDoMissD	= tReqMissD;

// `ifdef def_true
`ifndef def_true

	if((tReqMiss && tReqReady && !tReqMissL) ||
		(tReqMissNoSkipL && tRegOutHoldL))
	begin
		tReqMissNoSkip	= 1;

		tReqDoMissA	= tReqMissAddrA && tReqIsNz;
		tReqDoMissB	= tReqMissAddrB && tReqIsNz;
	end
`endif

	if(tReqIsNz)
	begin
		if(!tReqReady)
		begin
//			$display("VRAM: Not Ready Stall");
			tRegOutHold = 1;
		end

		if(tReqMiss)
		begin
//			$display("VRAM: Miss Stall");
			tRegOutHold = 1;
		end
	end
	
	tCellData	= tCellDataL;
//	if(!tReqIsNz)
//		tCellData	= { tBlkMemData2B, tBlkMemData2A };
	tCellData	= { tBlkMemData2D, tBlkMemData2C };

	tArrMemAddrStC	= tBlkMemAddr2A;
	tArrMemDataStC	= tBlkMemData2A;
	tArrMemIdxStC	= tReqIxD;

	tArrMemAddrStD	= tBlkMemAddr2B;
	tArrMemDataStD	= tBlkMemData2B;
	tArrMemIdxStD	= tReqIxD;
	
	if(tBlkMemAddr2A[27: 5] == tReqAxD[23: 1])
	begin
//		tCellData[127:  0] = tBlkMemData2A;
		tArrMemDoStC	= tBlkIsDirtyA;
	end
	if(tBlkMemAddr2B[27: 5] == tReqAxD[23: 1])
	begin
//		tCellData[255:128] = tBlkMemData2B;
		tArrMemDoStD	= tBlkIsDirtyB;
	end
		
//	tCellData[31:0]	= 32'h0F300041;
	
	if(tReqBix[4])
	begin
		tBlkExData0 = { tBlkMemData2A, tBlkMemData2B };
	end	else begin
		tBlkExData0 = { tBlkMemData2B, tBlkMemData2A };
	end

	tBlkExData1 = tReqBix[3] ? tBlkExData0[191:64] : tBlkExData0[127: 0];
	tBlkExData2 = tReqBix[2] ? tBlkExData1[119:32] : tBlkExData1[ 87: 0];
	tBlkExData3 = tBlkExData2[ 71: 0];
	tBlkExData4 = tBlkExData3[ 63: 0];

	tBlkExData = tBlkExData4;
	if(tReqOpm[1:0]!=2'b11)
		tBlkExData[63:32] = UV32_00;

//	tRegOutVal = tBlkExData;
		
	tBlkInsData4 = tReqInValA;
	if(tReqOpm[1:0]!=2'b11)
		tBlkInsData4[63:32] = tBlkExData4[63:32];

	tBlkInsData3 = { tBlkExData3 [71:64], tBlkInsData4[63:0] } ;
	tBlkInsData2 = { tBlkExData2 [87:72], tBlkInsData3[71:0] } ;
	tBlkInsData1 = tReqBix[2] ?
		{ tBlkExData1 [127:120], tBlkInsData2[ 87: 0], tBlkExData1 [31:0] } :
		{ tBlkExData1 [127:120], tBlkExData1 [119:88], tBlkInsData2[87:0] } ;
	tBlkInsData0 = tReqBix[3] ?
		{ tBlkExData0 [255:192], tBlkInsData1[127:  0], tBlkExData0 [ 63:0] } :
		{ tBlkExData0 [255:192], tBlkExData0 [191:128], tBlkInsData1[127:0] } ;

	if(tReqIsCRs)
		tBlkExData[31:0] = tScrRegCtrl;

	/* EX3 */


	tNxtReq2StoreSticky = 0;

	if(memRingIsRespOkStA)
	begin
//		if(tReqSeqIdx==tReqIxA)
		if(1'b1)
		begin
//			$display("VRAM: Store Response A, A=%X, Ix1/Ix2 %X / %X",
//				memAddrIn, tReqSeqIdx, tReqIxA);
			tNxtMemRespStA = 1;
		end
		else
		begin
			$display("VRAM: Mismatch Store Response A, A=%X Ix1/Ix2 = %X / %X", 
				memAddrIn, tReqSeqIdx, tReqIxA);
		end
	end
	
	if(memRingIsRespOkStB)
	begin
//		if(tReqSeqIdx==tReqIxB)
		if(1'b1)
		begin
//			$display("VRAM: Store Response B, A=%X", memAddrIn);
			tNxtMemRespStB = 1;
		end
		else
		begin
			$display("VRAM: Mismatch Store Response B, A=%X", memAddrIn);
		end
	end

	if(memRingIsRespOkLdA && !tMemRingSkipResp && !reset)
	begin
//		$display("VRAM: Load Response A, A=%X", memAddrIn);

//		if(tReqSeqIdx==tReqIxA)
		if(1'b1)
		begin
			tArrMemAddrStA = {
				memAddrIn[27:5], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStA = memDataIn;
			tArrMemIdxStA = tReqIxA;
//			tArrMemIdxStA = tReqSeqIdx;
			tArrMemDoStA = 1;
			tNxtMemRespLdA = 1;
		end
		
		if(memAddrIn[4])
			$display("VRAM: Load Even/Odd Mismatch A");
		
		if(tReqSeqIdx!=tReqIxA)
			$display("VRAM: In!=Req IxA, %X %X",
				tReqSeqIdx, tReqIxA);
	end

	if(memRingIsRespOkLdB && !tMemRingSkipResp && !reset)
	begin
//		$display("VRAM: Load Response B, A=%X", memAddrIn);

//		if(tReqSeqIdx==tReqIxB)
		if(1'b1)
		begin
			tArrMemAddrStB = {
				memAddrIn[27:5], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStB = memDataIn;
			tArrMemIdxStB = tReqIxB;
//			tArrMemIdxStB = tReqSeqIdx;
			tArrMemDoStB = 1;
			tNxtMemRespLdB = 1;
		end

		if(!memAddrIn[4])
			$display("VRAM: Load Even/Odd Mismatch B");

		if(tReqSeqIdx!=tReqIxB)
			$display("VRAM: In!=Req IxB, %X %X",
				tReqSeqIdx, tReqIxB);
	end


	tNxtMemReqLdC	= tMemReqLdC;
	tNxtMemReqLdD	= tMemReqLdD;

	if(memRingIsRespOkLdC)
	begin
//		$display("VRAM: Load Response C, A=%X", memAddrIn);

		tArrMemAddrStC	= { memAddrIn[27:4], memOpmIn[3:0] };
		tArrMemDataStC	= memDataIn;
		tArrMemIdxStC	= memAddrIn[12:5];
//		tArrMemDoStC	= 1;
		tArrMemDoStC	= !memOpmIn[14];
//		tNxtMemRespLdC	= 1;
		tNxtMemReqLdC	= 0;
	end

	if(memRingIsRespOkLdD)
	begin
//		$display("VRAM: Load Response D, A=%X", memAddrIn);

		tArrMemAddrStD	= { memAddrIn[27:4], memOpmIn[3:0] };
		tArrMemDataStD	= memDataIn;
		tArrMemIdxStD	= memAddrIn[12:5];
//		tArrMemDoStD	= 1;
		tArrMemDoStD	= !memOpmIn[14];
//		tNxtMemRespLdD	= 1;
		tNxtMemReqLdD	= 0;
	end


`ifdef def_true
	tReqWaitResp = 0;
	if(tMemReqLdA && !tNxtMemRespLdA)
		tReqWaitResp = 1;
	if(tMemReqLdB && !tNxtMemRespLdB)
		tReqWaitResp = 1;
	if(tMemReqStA && !tNxtMemRespStA)
		tReqWaitResp = 1;
	if(tMemReqStB && !tNxtMemRespStB)
		tReqWaitResp = 1;

	if(tReqWaitResp)
	begin
//		$display("VRAM: Wait Response");
		tRegOutHold = 1;
	end
`endif

	if(tArrMemDoStA || tArrMemDoStB)
	begin
//		$display("VRAM: Wait Store");
		tRegOutHold = 1;
	end

	if(tReq2IsNz && tReq2Opm[5] && !tReq2IsCRs)
	begin
		tNxtReq2StoreSticky = tReq2StoreSticky;

//		$display("Store Sticky=%d Data=%X",
//			tReq2StoreSticky, tBlk2InsData);

//		if(!tReq2Ready)
//			$display("VRAM: Store Fault: Non-Ready State");


		/* Stall if store needs to be delayed. */
		if(	(tArrMemDoStA && !tReq2MissSkipA) || 
			(tArrMemDoStB && !tReq2MissSkipB) )
		begin
			if(!tReq2StoreSticky)
			begin
//				$display("VRAM: Store Sticky Stall");
//				tRegOutHold = 1;
			end
		end
		else if(!tReq2StoreSticky)
		begin
			tArrMemAddrStA	= tBlk2StoreAddrA;
			tArrMemDataStA	= tBlk2StoreDataA;
			tArrMemIdxStA	= tReq2IxA;

			tArrMemAddrStB	= tBlk2StoreAddrB;
			tArrMemDataStB	= tBlk2StoreDataB;
			tArrMemIdxStB	= tReq2IxB;

			tNxtReq2StoreSticky = 1;

			tArrMemDoStA = !tReq2MissSkipA;
			tArrMemDoStB = !tReq2MissSkipB;

`ifndef def_true
			if((tBlk2MemAddrA[31:5]!=tReq2AxA[27:1]) && !tReq2MissSkipA)
			begin
				$display("VRAM: Store Addr Mismatch A");
//				tRegOutHold		= 1;
			end
			if((tBlk2MemAddrB[31:5]!=tReq2AxB[27:1]) && !tReq2MissSkipB)
			begin
				$display("VRAM: Store Addr Mismatch B");
//				tRegOutHold		= 1;
			end
`endif

`ifdef def_true
			if((tArrMemDataStA[127:0]!=tBlk2MemDataA[127:0]) && tReq2MissSkipA)
				$display("VRAM: Store Data Mismatch A");
			if((tArrMemDataStB[127:0]!=tBlk2MemDataB[127:0]) && tReq2MissSkipB)
				$display("VRAM: Store Data Mismatch B");
`endif

// `ifndef def_true
`ifdef def_true
			if(tReq2MissA && tArrMemDoStA)
			begin
				$display("VRAM: Store Fault: MissA");
				tArrMemDoStA = 0;
//				tRegOutHold		= 1;
			end

			if(tReq2MissB && tArrMemDoStB)
			begin
				$display("VRAM: Store Fault: MissB");
				tArrMemDoStB = 0;
//				tRegOutHold		= 1;
			end
`endif
			
//			$display("%X %X", tArrMemDoStA, tArrMemDoStB)
		end
	end

	if(tPixCellIxCL	!= tPixCellIxC)
		tNxtMemReqLdC	= 0;
	if(tPixCellIxDL	!= tPixCellIxD)
		tNxtMemReqLdD	= 0;

	if(reset)
	begin
		tReqMiss		= 0;
		tReqMissA		= 0;
		tReqMissB		= 0;
		tReqWaitResp	= 0;

		tNxtMemReqLdC	= 0;
		tNxtMemReqLdD	= 0;
	end

	/* Miss Handling */

`ifdef def_true
	if(tMemReqStA && !tMemReqLdA && !tReqDoMissA)
	begin
		$display("VRAM: Store Without Load A");
//		tReqDoMissA = 1;
	end
	if(tMemReqStB && !tMemReqLdB && !tReqDoMissB)
	begin
		$display("VRAM: Store Without Load B");
//		tReqDoMissB = 1;
	end

//	if(!tMemReqStA && tMemReqLdA && tBlkIsDirtyA)
//		$display("VRAM: Gain DirtyA");
//	if(!tMemReqStB && tMemReqLdB && tBlkIsDirtyB)
//		$display("VRAM: Gain DirtyB");
`endif

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UVB2AT_00;
//	tMemDataReq		= UVTILE_XX;
	tMemDataReq		= UVTILE_00;
	tNxtMemReqStA	= 0;
	tNxtMemReqStB	= 0;
	tNxtMemReqLdA	= 0;
	tNxtMemReqLdB	= 0;
	tMemSeqIx		= 0;
	tMemSeqVa		= 0;
	tNxtMemSeqRov	= tMemSeqRov;

	if(tReqDoMissA || tReqDoMissB || tReqWaitResp)
	begin
		tNxtMemReqStA	= tMemReqStA;
		tNxtMemReqStB	= tMemReqStB;
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;

		tMemSeqIx		= tMemSeqIxL;
		tMemSeqVa		= tMemSeqVaL;
	end

`ifdef def_true
	if(tReqDoMissC && !tMemReqLdC)
	begin
//			$display("VRAM: Send LDC Req A=%X", tReqAxC);
		tNxtMemSeqRov	= tMemSeqRov + 1;
		tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
//		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
`ifdef jx2_enable_l2addr96
		tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxC[23:1], 5'h00 };
`else
		tMemAddrReq		= { 20'hC0002, tReqAxC[23:1], 5'h00 };
`endif
		tNxtMemReqLdC	= 1;
	end
	else
		if(tReqDoMissD && !tMemReqLdD)
	begin
//			$display("VRAM: Send LDD Req A=%X", tReqAxD);
		tNxtMemSeqRov	= tMemSeqRov + 1;
		tMemSeqReq		= { unitNodeId, 4'b1100, tMemSeqRov };
//		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
`ifdef jx2_enable_l2addr96
		tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxC[23:1], 5'h10 };
`else
		tMemAddrReq		= { 20'hC0002, tReqAxC[23:1], 5'h10 };
`endif
		tNxtMemReqLdD	= 1;
	end
	else
`endif
	if(reset)
	begin
		tNxtMemSeqRov	= 0;
	end
	else
		if(tReqDoMissA || tReqDoMissB || tReqWaitResp)
	begin
`ifndef def_true
		tNxtMemReqStA	= tMemReqStA;
		tNxtMemReqStB	= tMemReqStB;
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;
//		tNxtMemSeqRov	= tMemSeqRov;

		tMemSeqIx		= tMemSeqIxL;
		tMemSeqVa		= tMemSeqVaL;
`endif

//		$display("VRAM: DoMiss %X %X %X",
//			tReqDoMissA, tReqDoMissB, tReqWaitResp);

//		tRegOutWait		= 1;
		tRegOutHold		= 1;

// `ifndef def_true
`ifdef def_true
		if(!tReqReady)
		begin
			/* Wait */
		end
		else
`endif

`ifdef def_true
		if(tReqDoMissA && tBlkIsDirtyA && !tMemReqStA)
		begin
//			$display("VRAM: Send STA Req, A=%X", tBlkMemAddr2A[27:4]);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
			tMemDataReq		= tBlkMemData2A;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
`ifdef jx2_enable_l2addr96
			tMemAddrReq		= { UV48_00, 20'hC0002,
				tBlkMemAddr2A[27: 5], 5'h00 };
`else
			tMemAddrReq		= { 20'hC0002, tBlkMemAddr2A[27: 5], 5'h00 };
`endif
			tNxtMemReqStA	= 1;
		end
		else if(tReqDoMissB && tBlkIsDirtyB && !tMemReqStB)
		begin
//			$display("VRAM: Send STB Req, A=%X", tBlkMemAddr2B[27:4]);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov };
			tMemDataReq		= tBlkMemData2B;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
`ifdef jx2_enable_l2addr96
			tMemAddrReq		= { UV48_00, 20'hC0002,
				tBlkMemAddr2B[27: 5], 5'h10 };
`else
			tMemAddrReq		= { 20'hC0002, tBlkMemAddr2B[27: 5], 5'h10 };
`endif
			tNxtMemReqStB	= 1;
		end
		else
			if(tReqDoMissA && !tMemReqLdA)
		begin
//			$display("VRAM: Send LDA Req A=%X", tReqAxA);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2A;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l2addr96
			tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxA, 4'h00 };
`else
			tMemAddrReq		= { 20'hC0002, tReqAxA, 4'h00 };
`endif

			tNxtMemReqLdA = 1;
		end
		else
			if(tReqDoMissB && !tMemReqLdB)
		begin
//			$display("VRAM: Send LDB Req A=%X", tReqAxB);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2B;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l2addr96
			tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxB, 4'h00 };
`else
			tMemAddrReq		= { 20'hC0002, tReqAxB, 4'h00 };
`endif

			tNxtMemReqLdB	= 1;
		end
`endif
	end
	else
	begin
	end

`ifndef jx2_fbuf_nofont
	case(tFontGlyph[9:7])
		3'b000:	tFontData2 = tFontDataAsc1;
		3'b001:	tFontData2 = tFontDataAsc1;
		3'b010:	tFontData2 = tFontDataGfx2;
		3'b011:	tFontData2 = tFontDataGfx1;
		3'b100:	tFontData2 = tFontDataRam;
		3'b101:	tFontData2 = tFontDataRam;
		3'b110:	tFontData2 = tFontDataRam;
		3'b111:	tFontData2 = tFontDataRam;
	endcase
`else
	tFontData2 = tFontDataRam;
`endif

	if(reset)
	begin
		tRegOutHold = 0;
	end

	tNxtHoldCyc = tHoldCyc;

	if(tRegOutHold)
	begin
		tNxtHoldCyc = tHoldCyc + 1;
	end
	
	if(!tReqIsNz)
		tNxtHoldCyc = 0;
	if(tHoldCyc[10])
	begin
		$display("VRAM: Timeout");
		tRegOutHold = 0;
	end

	tRegOutOK = tReqIsNz ?
		(tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK) : 
		UMEM_OK_READY;
	
	if(tReqIsNz && tReqIsCRs)
//	if(tReqIsNz)
	begin
//		tRegOutHold = 0;
//		tRegOutOK = UMEM_OK_OK;
	end
	
	tRegOutVal		= tBlkExData;

	if(!tRegOutHold &&
		tReqIsNz && (tNxtReqOpm[5:4]!=0) &&
		(tRegOutOKL == UMEM_OK_OK))
	begin
		tRegOutVal		= tRegOutValL;
		tRegOutOK		= tRegOutOKL;
	end
	
	if(reset)
	begin
		tRegOutOK		= UMEM_OK_READY;
		tRegOutHold		= 0;
	end
	
//	if(tRegOutOK)
//		$display("OK=%X", tRegOutOK);
end

always @(posedge clock)
begin
	tProbeCyc		<= tNxtProbeCyc;
	tHoldCyc		<= tNxtHoldCyc;
	tPixCellIxLim	<= tNxtPixCellIxLim;

//	if(!dcInHold)
	if(!tRegOutHold)
	begin
//		$display("VRAM: Pipe Advance");

		/* EX1 -> EX2 */
		tReqAddr		<= tNxtReqAddr;
		tReqIxA			<= tNxtReqIxA;
		tReqIxB			<= tNxtReqIxB;
		tReqAxA			<= tNxtReqAxA;
		tReqAxB			<= tNxtReqAxB;
		tReqBix			<= tNxtReqBix;
		tReqOpm			<= tNxtReqOpm;
		tReqInValA		<= tNxtReqInValA;

		tReqIsCSel		<= tNxtReqIsCSel;
		tReqIsCRs		<= tNxtReqIsCRs;
		tReqIsNz		<= tNxtReqIsNz;

		/* EX2 -> EX3 */
//		tRegOutVal		<= tBlkExData;
//		tRegOutValB		<= tBlkExDataB;

		tBlk2MemAddrA	<= tBlkMemAddr2A;
		tBlk2MemAddrB	<= tBlkMemAddr2B;
		tBlk2MemDataA	<= tBlkMemData2A;
		tBlk2MemDataB	<= tBlkMemData2B;

		tBlk2InsData	<= tBlkInsData0;
		tReq2IxA		<= tReqIxA;
		tReq2IxB		<= tReqIxB;
		tReq2AxA		<= tReqAxA;
		tReq2AxB		<= tReqAxB;
		tReq2Opm		<= tReqOpm;
		tReq2Bix		<= tReqBix;
		tReq2Ready		<= tReqReady;
		tReq2IsCRs		<= tReqIsCRs;
		tReq2IsNz		<= tReqIsNz;

		tReq2MissA		<= tReqMissA;
		tReq2MissB		<= tReqMissB;

		tReq2NoCross		<= tReqNoCross;
		tReq2MissSkipA		<= tReqMissSkipA;
		tReq2MissSkipB		<= tReqMissSkipB;

		tReq2StoreSticky	<= 0;

		tRegOutValL		<= tRegOutVal;
		tRegOutOKL		<= tRegOutOK;
	end
	else
	begin
		tReq2StoreSticky	<= tNxtReq2StoreSticky;
	end

	scrRegCtrl0		<= nxtScrRegCtrl0;
	scrRegCtrl1		<= nxtScrRegCtrl1;
	scrRegCtrl2		<= nxtScrRegCtrl2;
	scrRegCtrl3		<= nxtScrRegCtrl3;
	scrRegCtrl4		<= nxtScrRegCtrl4;
	scrRegCtrl5		<= nxtScrRegCtrl5;
	scrRegCtrl6		<= nxtScrRegCtrl6;
	scrRegCtrl7		<= nxtScrRegCtrl7;
	scrRegCtrl8		<= nxtScrRegCtrl8;
	scrRegCtrl9		<= nxtScrRegCtrl9;

	tCellDataL		<= tCellData;

	tReqIxC			<= tNxtReqIxC;
	tReqIxD			<= tNxtReqIxD;
	tReqAxC			<= tNxtReqAxC;
	tReqAxD			<= tNxtReqAxD;

	tPixCellIxCL	<= tPixCellIxC;
	tPixCellIxDL	<= tPixCellIxD;


//	tRegOutValL		<= tRegOutVal;
//	tRegOutOKL		<= tRegOutOK;

	tReqReadyL		<= tReqReady;

	tInOpm			<= tNxtReqOpm;
	tInOpmB			<= tInOpm;
	tInOpmC			<= tInOpmB;
	
	tRegOutHoldL	<= tRegOutHold;
	tReqMissNoSkipL	<= tReqMissNoSkip;

	/* EX1->EX2 */
	tBlkMemAddrA	<= arrMemAddrA[tReq1IxA];
	tBlkMemAddrB	<= arrMemAddrB[tReq1IxB];
	tBlkMemDataA 	<= arrMemDataA[tReq1IxA];
	tBlkMemDataB 	<= arrMemDataB[tReq1IxB];
	tBlkMemIdxA		<= tReq1IxA;
	tBlkMemIdxB		<= tReq1IxB;

	tBlkMemAddrC	<= arrMemAddrC[tNxtReqIxC];
	tBlkMemAddrD	<= arrMemAddrD[tNxtReqIxC];
	tBlkMemDataC 	<= arrMemDataC[tNxtReqIxD];
	tBlkMemDataD 	<= arrMemDataD[tNxtReqIxD];
	tBlkMemIdxC		<= tReqIxC;
	tBlkMemIdxD		<= tReqIxD;

`ifndef jx2_fbuf_nofont
	tFontDataAsc1	<= fontMem[tFontGlyph[7:0]];
	tFontDataGfx1	<= fontGfx1Mem[tFontGlyph[6:0]];
	tFontDataGfx2	<= fontGfx2Mem[tFontGlyph[6:0]];
`endif

//	tFontDataRam	<= {
//		fontRamB[tFontGlyph[8:0]],
//		fontRamA[tFontGlyph[8:0]] };
	tFontDataRam	<= {
		fontRamB[tFontGlyph[9:0]],
		fontRamA[tFontGlyph[9:0]] };
	tFontGlyph		<= fontGlyph;
	tFontData1		<= tFontData2;

//	tFontGlyph		<= 16'h0042;


	if(tArrMemDoStA)
	begin
//		$display("VRAM STA Ix=%X A=%X D=%X",
//			tArrMemIdxStA, tArrMemAddrStA, tArrMemDataStA);
		arrMemAddrA[tArrMemIdxStA]	<= tArrMemAddrStA;
		arrMemDataA[tArrMemIdxStA]	<= tArrMemDataStA;
	end

	if(tArrMemDoStB)
	begin
//		$display("VRAM STB Ix=%X A=%X D=%X",
//			tArrMemIdxStB, tArrMemAddrStB, tArrMemDataStB);
		arrMemAddrB[tArrMemIdxStB]	<= tArrMemAddrStB;
		arrMemDataB[tArrMemIdxStB]	<= tArrMemDataStB;
	end

	if(tArrMemDoStC)
	begin
//		$display("VRAM STC Ix=%X A=%X D=%X",
//			tArrMemIdxStC, tArrMemAddrStC, tArrMemDataStC);
		arrMemAddrC[tArrMemIdxStC]	<= tArrMemAddrStC;
		arrMemDataC[tArrMemIdxStC]	<= tArrMemDataStC;
	end

	if(tArrMemDoStD)
	begin
//		$display("VRAM STD Ix=%X A=%X D=%X",
//			tArrMemIdxStD, tArrMemAddrStD, tArrMemDataStD);
		arrMemAddrD[tArrMemIdxStD]	<= tArrMemAddrStD;
		arrMemDataD[tArrMemIdxStD]	<= tArrMemDataStD;
	end

	if(tFontRamDoStA)
	begin
		fontRamA[tFontRamStIxA]		<= tFontRamStDataA;
	end
	if(tFontRamDoStB)
	begin
		fontRamB[tFontRamStIxB]		<= tFontRamStDataB;
	end

	tArrMemDidStAddrA	<= tArrMemAddrStA;
	tArrMemDidStAddrB	<= tArrMemAddrStB;
	tArrMemDidStDataA	<= tArrMemDataStA;
	tArrMemDidStDataB	<= tArrMemDataStB;

	tArrMemDidStIxA	<= tArrMemIdxStA;
	tArrMemDidStIxB	<= tArrMemIdxStB;
	tArrMemDidStA	<= tArrMemDoStA;
	tArrMemDidStB	<= tArrMemDoStB;

	tMemRespStA		<= tRegOutHold ? tNxtMemRespStA : 0;
	tMemRespStB		<= tRegOutHold ? tNxtMemRespStB : 0;
	tMemRespLdA		<= tRegOutHold ? tNxtMemRespLdA : 0;
	tMemRespLdB		<= tRegOutHold ? tNxtMemRespLdB : 0;
//	tReqMissNoSkipL	<= tRegOutHold ? tReqMissNoSkip : 0;

	tMemSeqReqL		<= tMemSeqReq;
	tMemOpmReqL		<= tMemOpmReq;
	tMemAddrReqL	<= tMemAddrReq;
	tMemDataReqL	<= tMemDataReq;

	tMemSeqIxL		<= tMemSeqIx;
	tMemSeqVaL		<= tMemSeqVa;

	tReqMissAL		<= tReqMissA;
	tReqMissBL		<= tReqMissB;
	tReqMissL		<= tReqMiss;

//	tRegOutExc2		<= tRegOutExc;

	if(reset)
	begin
		tMemSeqOut  <= 0;
		tMemOpmOut  <= 0;
		tMemAddrOut <= 0;
		tMemDataOut <= 0;

		tMemReqSent	<= 0;

		tMemReqStA	<= 0;
		tMemReqStB	<= 0;
		tMemReqLdA	<= 0;
		tMemReqLdB	<= 0;
		tMemReqLdC	<= 0;
		tMemReqLdD	<= 0;
		tMemSeqRov	<= 0;
	end
	else
		if((memRingIsIdle || memRingIsResp) && !tMemRingSkipResp)
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqSent	<= 1;
		
//		if(tMemOpmReq[7:0]!=0)
//		begin
//			$display("VRAM: Rind Add O=%X S=%X A=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq);
//		end

		tMemSeqRov	<= tNxtMemSeqRov;

		tMemReqStA	<= tRegOutHold ? tNxtMemReqStA : 0;
		tMemReqStB	<= tRegOutHold ? tNxtMemReqStB : 0;
		tMemReqLdA	<= tRegOutHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tNxtMemReqLdB : 0;

		tMemReqLdC	<= tNxtMemReqLdC;
		tMemReqLdD	<= tNxtMemReqLdD;

		tReqSeqIdxArr[tMemSeqRov]	<= tMemSeqIx;
		tReqSeqVaArr[tMemSeqRov]	<= tMemSeqVa;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;

		tMemReqStA	<= tRegOutHold ? tMemReqStA : 0;
		tMemReqStB	<= tRegOutHold ? tMemReqStB : 0;
		tMemReqLdA	<= tRegOutHold ? tMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tMemReqLdB : 0;

		tMemReqLdC	<= tMemReqLdC;
		tMemReqLdD	<= tMemReqLdD;
	end
end

endmodule

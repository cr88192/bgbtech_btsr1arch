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

Audio Mixer on the Ringbus.

Put the VRAM into C000_2080000, which is assumed to be in the "wraparound" region located before the start of RAM.

 */

`include "ringbus/RbiDefs.v"

module RbiMemAuMix(
	/* verilator lint_off UNUSED */
	clock,			reset,

	pixCellIx,		cellData,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

input[15:0]		pixCellIx;
output[127:0]	cellData;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

reg[127:0]	tCellData;
reg[127:0]	tCellDataL;
assign	cellData = tCellDataL;

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
wire			memRingIsRespOkLdC;
wire			memRingIsRespOkLdD;
assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkSt =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b10);

assign		memRingIsRespOkLdC =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);
assign		memRingIsRespOkLdD =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b11);

reg		tMemRingSkipResp;

/*
Addr:
  (71:68): Flush Rover
  (67:48): PA Bits
  (47: 5): VA Bits
  ( 4: 0): Access Flags
 */

`define			reg_vram_ix2	reg[7:0]
reg[ 27:0]		arrMemAddrC[255:0];
reg[ 27:0]		arrMemAddrD[255:0];
reg[127:0]		arrMemDataC[255:0];
reg[127:0]		arrMemDataD[255:0];


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

reg[ 27:0]		tNxtReqAddr;
reg[ 27:0]		tReqAddr;

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

reg				tReqMissAddrC;
reg				tReqMissAddrD;
reg				tReqMissC;
reg				tReqMissD;
reg				tReqDoMissC;
reg				tReqDoMissD;
reg				tReqMiss;
reg				tReqWaitResp;
reg				tReqSx;

reg				tReqNoCross;
reg				tReqMissNoSkip;
reg				tReqMissNoSkipL;
reg				tReq2NoCross;

reg				tNxtReqIsCSel;
reg				tNxtReqIsCRs;
reg				tNxtReqIsNz;

reg				tReqIsCSel;
reg				tReqIsCRs;
reg				tReqIsNz;

reg				tReq2IsCRs;
reg				tReq2IsNz;

reg[ 27:0]		tBlkMemAddrC;
reg[ 27:0]		tBlkMemAddrD;
reg[127:0]		tBlkMemDataC;
reg[127:0]		tBlkMemDataD;
`reg_vram_ix2	tBlkMemIdxC;
`reg_vram_ix2	tBlkMemIdxD;

reg[ 27:0]		tBlkMemAddr2C;
reg[ 27:0]		tBlkMemAddr2D;
reg[127:0]		tBlkMemData2C;
reg[127:0]		tBlkMemData2D;

reg[  5:0]		tReq2Opm;
reg[  4:0]		tReq2Bix;



reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l2addr	tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

reg[ 15:0]		tMemSeqReqL;
reg[ 15:0]		tMemOpmReqL;
`reg_l2addr	tMemAddrReqL;
`reg_tile		tMemDataReqL;

reg				tMemReqStC;			//Store C
reg				tMemReqStD;			//Store D
reg				tNxtMemReqStC;		//Store C
reg				tNxtMemReqStD;		//Store D

reg				tMemRespStC;		//Store C
reg				tMemRespStD;		//Store D
reg				tNxtMemRespStC;		//Store C
reg				tNxtMemRespStD;		//Store D

reg				tMemReqLdC;		//Load C
reg				tMemReqLdD;		//Load D

reg				tNxtMemReqLdC;	//Load C
reg				tNxtMemReqLdD;	//Load D

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)

reg[3:0]		tRegInAddrHb;

reg[27:0]		tRegInAddr;
reg[27:0]		tRegInAddrC;
reg[27:0]		tRegInAddrD;
reg[5:0]		tRegInOpm;

reg[15:0]		tPixCellIxC;
reg[15:0]		tPixCellIxD;
reg[15:0]		tPixCellIxLim;
reg[15:0]		tNxtPixCellIxLim;
reg[16:0]		tPixCellIxCi;

reg[15:0]		tPixCellIxCL;
reg[15:0]		tPixCellIxDL;

reg[11:0]	tHoldCyc;
reg[11:0]	tNxtHoldCyc;

reg[3:0]	tProbeCyc;
reg[3:0]	tNxtProbeCyc;

always @*
begin
	/* EX1 */
		
	tNxtPixCellIxLim = 16383;

	tNxtProbeCyc	= tProbeCyc + 1;

//	tPixCellIxC = pixCellIx + { 9'h00, tProbeCyc[1:0], 5'h0 };
	tPixCellIxC = pixCellIx;
	tPixCellIxD = pixCellIx + 1;
	
	tPixCellIxCi = { 1'b0, tPixCellIxC } - { 1'b0, tPixCellIxLim };

	tRegInAddrC = { 8'h08, tPixCellIxC[15:0], 4'h0 };
	tRegInAddrD = { 8'h08, tPixCellIxD[15:0], 4'h0 };

	tNxtReqAxC = tRegInAddrC[27:4];
	tNxtReqAxD = tRegInAddrD[27:4];
	tNxtReqIxC[7:0] = tNxtReqAxC[7:0];
	tNxtReqIxD[7:0] = tNxtReqAxD[7:0];
end

always @*
begin
	tRegOutHold			= 0;
	tMemRingSkipResp	= 0;


	/* EX2 */
	
	tArrMemAddrStC	= 0;
	tArrMemDataStC	= 0;
	tArrMemIdxStC	= 0;
	tArrMemDoStC	= 0;
	
	tArrMemAddrStD	= 0;
	tArrMemDataStD	= 0;
	tArrMemIdxStD	= 0;
	tArrMemDoStD	= 0;

	tBlkMemData2C = tBlkMemDataC;
	tBlkMemData2D = tBlkMemDataD;
	tBlkMemAddr2C = tBlkMemAddrC;
	tBlkMemAddr2D = tBlkMemAddrD;

	tReqMissAddrC	=
		 (tBlkMemAddr2C[27: 4] != tReqAxC[23: 0]);
	tReqMissAddrD	=
		 (tBlkMemAddr2D[27: 4] != tReqAxC[23: 0]);

	tReqMissC	= tReqMissAddrC;
	tReqMissD	= tReqMissAddrD;

	tReqDoMissC	= tReqMissC;
	tReqDoMissD	= tReqMissD;
	
	if(!tReqMissAddrC || !tReqMissAddrD)
	begin
		tReqDoMissC	= 0;
		tReqDoMissD	= 0;
	end
	
	tCellData	= tCellDataL;
//	tCellData	= { tBlkMemData2D, tBlkMemData2C };

	tCellData	= tBlkMemData2C;
	if(!tReqMissAddrD)
		tCellData	= tBlkMemData2D;

	/* EX3 */


	tNxtMemReqLdC	= tMemReqLdC;
	tNxtMemReqLdD	= tMemReqLdD;

	if(memRingIsRespOkLdC)
	begin
		tArrMemAddrStC	= { memAddrIn[27:4], memOpmIn[3:0] };
		tArrMemDataStC	= memDataIn;
		tArrMemIdxStC	= memAddrIn[12:5];
		tArrMemDoStC	= !memOpmIn[14];
		tNxtMemReqLdC	= 0;
	end

	if(memRingIsRespOkLdD)
	begin
		tArrMemAddrStD	= { memAddrIn[27:4], memOpmIn[3:0] };
		tArrMemDataStD	= memDataIn;
		tArrMemIdxStD	= memAddrIn[12:5];
		tArrMemDoStD	= !memOpmIn[14];
		tNxtMemReqLdD	= 0;
	end

	if(tPixCellIxCL	!= tPixCellIxC)
		tNxtMemReqLdC	= 0;
	if(tPixCellIxDL	!= tPixCellIxD)
		tNxtMemReqLdD	= 0;

	if(reset)
	begin
		tReqWaitResp	= 0;

		tNxtMemReqLdC	= 0;
		tNxtMemReqLdD	= 0;
	end

	/* Miss Handling */

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UVB2AT_00;
//	tMemDataReq		= UVTILE_XX;
	tMemDataReq		= UVTILE_00;
	tNxtMemSeqRov	= tMemSeqRov;

`ifdef def_true
	if(tReqDoMissC && !tMemReqLdC)
	begin
//			$display("VRAM: Send LDC Req A=%X", tReqAxC);
		tNxtMemSeqRov	= tMemSeqRov + 1;
		tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
//		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
`ifdef jx2_enable_l2addr96
		tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxC[23:0], 4'h0 };
`else
		tMemAddrReq		= { 20'hC0002, tReqAxC[23:0], 4'h0 };
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
		tMemAddrReq		= { UV48_00, 20'hC0002, tReqAxC[23:0], 4'h0 };
`else
		tMemAddrReq		= { 20'hC0002, tReqAxC[23:0], 4'h0 };
`endif
		tNxtMemReqLdD	= 1;
	end
`endif

	if(reset)
	begin
		tNxtMemSeqRov	= 0;
	end

	if(reset)
	begin
		tRegOutHold = 0;
	end

	tNxtHoldCyc = tHoldCyc;

	if(tRegOutHold)
	begin
		tNxtHoldCyc = tHoldCyc + 1;
	end
	
	if(tHoldCyc[10])
	begin
		$display("AURAM: Timeout");
		tRegOutHold = 0;
	end
	
	if(reset)
	begin
		tRegOutHold		= 0;
	end
end

always @(posedge clock)
begin
	tProbeCyc		<= tNxtProbeCyc;
	tHoldCyc		<= tNxtHoldCyc;
	tPixCellIxLim	<= tNxtPixCellIxLim;

	tCellDataL		<= tCellData;

	tReqIxC			<= tNxtReqIxC;
	tReqIxD			<= tNxtReqIxD;
	tReqAxC			<= tNxtReqAxC;
	tReqAxD			<= tNxtReqAxD;

	tPixCellIxCL	<= tPixCellIxC;
	tPixCellIxDL	<= tPixCellIxD;

	tRegOutHoldL	<= tRegOutHold;
	tReqMissNoSkipL	<= tReqMissNoSkip;

	/* EX1->EX2 */
	tBlkMemAddrC	<= arrMemAddrC[tNxtReqIxC];
	tBlkMemAddrD	<= arrMemAddrD[tNxtReqIxC];
	tBlkMemDataC 	<= arrMemDataC[tNxtReqIxD];
	tBlkMemDataD 	<= arrMemDataD[tNxtReqIxD];
	tBlkMemIdxC		<= tReqIxC;
	tBlkMemIdxD		<= tReqIxD;

	if(tArrMemDoStC)
	begin
		arrMemAddrC[tArrMemIdxStC]	<= tArrMemAddrStC;
		arrMemDataC[tArrMemIdxStC]	<= tArrMemDataStC;
	end

	if(tArrMemDoStD)
	begin
		arrMemAddrD[tArrMemIdxStD]	<= tArrMemAddrStD;
		arrMemDataD[tArrMemIdxStD]	<= tArrMemDataStD;
	end

	tMemSeqReqL		<= tMemSeqReq;
	tMemOpmReqL		<= tMemOpmReq;
	tMemAddrReqL	<= tMemAddrReq;
	tMemDataReqL	<= tMemDataReq;

	if(reset)
	begin
		tMemSeqOut  <= 0;
		tMemOpmOut  <= 0;
		tMemAddrOut <= 0;
		tMemDataOut <= 0;

		tMemReqSent	<= 0;

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

		tMemSeqRov	<= tNxtMemSeqRov;

		tMemReqLdC	<= tNxtMemReqLdC;
		tMemReqLdD	<= tNxtMemReqLdD;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;

		tMemReqLdC	<= tMemReqLdC;
		tMemReqLdD	<= tMemReqLdD;
	end
end

endmodule

`include "ringbus/RbiDefs.v"

module RbiMemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutValA,		regInValA,
	regOutValB,		regInValB,
	dcInHold,		regOutHold,
	regInSr,		regOutWait,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,

	unitNodeId
	);

input			clock;
input			reset;

input [47: 0]	regInAddr;		//input address
input [ 5: 0]	regInOpm;		//operation mode

output[63: 0]	regOutValA;		//output data value (Low 128 / Lane A)
output[63: 0]	regOutValB;		//output data value (High 128 / Lane B)

input [63: 0]	regInValA;		//input data value (Low 128 / Lane A)
input [63: 0]	regInValB;		//input data value (High 128 / Lane B)

input			dcInHold;
output			regOutHold;
output			regOutWait;

input [63: 0]	regInSr;



input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
input [ 47:0]	memAddrIn;		//memory input address
output[ 47:0]	memAddrOut;		//memory output address
input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[63: 0]	tRegOutValA;
reg[63: 0]	tRegOutValB;
assign	regOutValA = tRegOutValA;
assign	regOutValB = tRegOutValB;


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
reg[ 47:0]		tMemAddrOut;		//memory output address
reg[127:0]		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

reg				tRegOutHold;
reg				tRegOutWait;
assign		regOutHold = tRegOutHold;
assign		regOutWait = tRegOutWait;

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
wire			memRingIsRespOkStA;
wire			memRingIsRespOkStB;
wire			memRingIsRespOkMmio;
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
assign		memRingIsRespOkMmio =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);

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

`ifdef jx2_mem_l1dsz_1024
`define			reg_l1d_ix	reg[9:0]
reg[ 71:0]		arrMemAddrA[1023:0];
reg[ 71:0]		arrMemAddrB[1023:0];
reg[127:0]		arrMemDataA[1023:0];
reg[127:0]		arrMemDataB[1023:0];
`endif

`ifdef jx2_mem_l1dsz_512
`define			reg_l1d_ix	reg[8:0]
reg[ 71:0]		arrMemAddrA[511:0];
reg[ 71:0]		arrMemAddrB[511:0];
reg[127:0]		arrMemDataA[511:0];
reg[127:0]		arrMemDataB[511:0];
`endif

`ifdef jx2_mem_l1dsz_256
`define			reg_l1d_ix	reg[7:0]
reg[ 71:0]		arrMemAddrA[255:0];
reg[ 71:0]		arrMemAddrB[255:0];
reg[127:0]		arrMemDataA[255:0];
reg[127:0]		arrMemDataB[255:0];
`endif

reg[ 71:0]		tArrMemAddrStA;
reg[ 71:0]		tArrMemAddrStB;
reg[127:0]		tArrMemDataStA;
reg[127:0]		tArrMemDataStB;
`reg_l1d_ix		tArrMemIdxStA;
`reg_l1d_ix		tArrMemIdxStB;
reg				tArrMemDoStA;
reg				tArrMemDoStB;
reg				tArrMemDidStA;
reg				tArrMemDidStB;

reg[ 71:0]		tArrMemDidStAddrA;
reg[ 71:0]		tArrMemDidStAddrB;
reg[127:0]		tArrMemDidStDataA;
reg[127:0]		tArrMemDidStDataB;

`reg_l1d_ix		tReqSeqIdxArr[15:0];
reg[ 43:0]		tReqSeqVaArr[15:0];

`reg_l1d_ix		tReqSeqIdx;
reg[ 43:0]		tReqSeqVa;

reg[  3:0]		tFlushRov;
reg[  3:0]		tNxtFlushRov;

reg[ 47:0]		tNxtReqAddr;
`reg_l1d_ix		tNxtReqIxA;
`reg_l1d_ix		tNxtReqIxB;
reg[ 43:0]		tNxtReqAxA;
reg[ 43:0]		tNxtReqAxB;
reg[ 47:0]		tReqAddr;
`reg_l1d_ix		tReqIxA;
`reg_l1d_ix		tReqIxB;
reg[ 43:0]		tReqAxA;
reg[ 43:0]		tReqAxB;
`reg_l1d_ix		tReq1IxA;
`reg_l1d_ix		tReq1IxB;
`reg_l1d_ix		tArrMemDidStIxA;
`reg_l1d_ix		tArrMemDidStIxB;


reg[  4:0]		tNxtReqBix;
reg[  4:0]		tReqBix;
reg[  5:0]		tNxtReqOpm;
reg[  5:0]		tReqOpm;

reg[63: 0]		tReqInValA;
reg[63: 0]		tReqInValB;
reg[63: 0]		tNxtReqInValA;
reg[63: 0]		tNxtReqInValB;

reg				tReqReadyA;
reg				tReqReadyB;
reg				tReqReady;
reg				tReqReadyL;
reg				tReqReady2;
reg				tReq2Ready;

reg				tReqMissAddrA;
reg				tReqMissAddrB;
reg				tReqMissA;
reg				tReqMissB;
reg				tReqMiss;
reg				tReqWaitResp;
reg				tReqSx;

reg				tReqFlushAddrA;
reg				tReqFlushAddrB;

reg				tReqNoCross;
reg				tReqMissSkipA;
reg				tReqMissSkipB;
reg				tReq2NoCross;
reg				tReq2MissSkipA;
reg				tReq2MissSkipB;
reg				tReq2MissA;
reg				tReq2MissB;

reg				tReqIsNz;
reg				tReqIsMmio;
reg				tReq2IsMmio;
reg				tReqIsCcmd;
reg				tReq2IsCcmd;


reg[ 71:0]		tBlkMemAddrA;
reg[ 71:0]		tBlkMemAddrB;
reg[127:0]		tBlkMemDataA;
reg[127:0]		tBlkMemDataB;
`reg_l1d_ix		tBlkMemIdxA;
`reg_l1d_ix		tBlkMemIdxB;
reg				tBlkIsDirtyA;
reg				tBlkIsDirtyB;

reg[ 71:0]		tBlkMemAddr2A;
reg[ 71:0]		tBlkMemAddr2B;
reg[127:0]		tBlkMemData2A;
reg[127:0]		tBlkMemData2B;

reg[255:0]		tBlkExData0;
reg[127:0]		tBlkExData1;
reg[ 87:0]		tBlkExData2;
reg[ 71:0]		tBlkExData3;
reg[ 63:0]		tBlkExData4;
reg[ 63:0]		tBlkExData;

reg[ 63:0]		tBlkExDataA;
reg[ 63:0]		tBlkExDataB;

reg[255:0]		tBlkInsData0;
reg[127:0]		tBlkInsData1;
reg[ 87:0]		tBlkInsData2;
reg[ 71:0]		tBlkInsData3;
reg[ 63:0]		tBlkInsData4;

reg[ 71:0]		tBlk2MemAddrA;
reg[ 71:0]		tBlk2MemAddrB;
reg[255:0]		tBlk2InsData;
`reg_l1d_ix		tReq2IxA;
`reg_l1d_ix		tReq2IxB;
reg[ 43:0]		tReq2AxA;
reg[ 43:0]		tReq2AxB;
reg[  5:0]		tReq2Opm;
reg[  4:0]		tReq2Bix;

reg				tReq2StoreSticky;
reg				tNxtReq2StoreSticky;
reg				tReg2MissInterlockA;
reg				tReg2MissInterlockB;

reg				tReg2StoreFwA;
reg				tReg2StoreFwB;

reg[63:0]		tMemMmioData;
reg[63:0]		tNxtMemMmioData;
reg				tMemMmioReady;
reg				tNxtMemMmioReady;


reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
reg[ 47:0]		tMemAddrReq;
reg[127:0]		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

reg				tMemReqStA;		//Store A
reg				tMemReqStB;		//Store B
reg				tNxtMemReqStA;	//Store A
reg				tNxtMemReqStB;	//Store B

reg				tMemRespStA;		//Store A
reg				tMemRespStB;		//Store B
reg				tNxtMemRespStA;		//Store A
reg				tNxtMemRespStB;		//Store B

reg				tMemRespLdA;		//Store A
reg				tMemRespLdB;		//Store B
reg				tNxtMemRespLdA;		//Store A
reg				tNxtMemRespLdB;		//Store B

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
reg				tMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
reg				tNxtMemReqLdM;	//Load MMIO

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

`reg_l1d_ix		tMemSeqIx;
reg[43:0]		tMemSeqVa;

reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg				tNxtDoFlush;
reg				tDoFlush;
reg				tDoFlushL;


always @*
begin
	tReqSeqIdx = tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa = tReqSeqVaArr[memSeqIn[3:0]];

	tArrMemDoStA		= 0;
	tArrMemDoStB		= 0;
	tRegOutHold			= 0;
	tRegOutWait			= 0;
	
	tNxtFlushRov		= tFlushRov;
	tNxtDoFlush			= 0;

	tNxtMemRespStA		= tMemRespStA;
	tNxtMemRespStB		= tMemRespStB;
	tNxtMemRespLdA		= tMemRespLdA;
	tNxtMemRespLdB		= tMemRespLdB;
	tMemRingSkipResp	= 0;


	/* EX1 */

`ifdef def_true
	if(regInAddr[4])
	begin
		tNxtReqAxB = regInAddr[47:4];
		tNxtReqAxA = tNxtReqAxB + 1;
	end
	else
	begin
		tNxtReqAxA = regInAddr[47:4];
		tNxtReqAxB = tNxtReqAxA + 1;
	end
	
	tNxtReqAddr		= regInAddr[47:0];
	tNxtReqBix		= regInAddr[4:0];
	tNxtReqOpm		= regInOpm;

`ifdef jx2_mem_l1dsz_256
	tNxtReqIxA[7:0] = tNxtReqAxA[7:0] ^ tNxtReqAxA[15:8];
	tNxtReqIxB[7:0] = tNxtReqAxB[7:0] ^ tNxtReqAxB[15:8];
`endif

`ifdef jx2_mem_l1dsz_512
//	tNxtReqIxA[7:0] = tNxtReqAxA[7:0] ^ tNxtReqAxA[15:8];
//	tNxtReqIxB[7:0] = tNxtReqAxB[7:0] ^ tNxtReqAxB[15:8];
//	tNxtReqIxA[8] = tNxtReqAxA[16] ^ tNxtReqAxA[17];
//	tNxtReqIxB[8] = tNxtReqAxB[16] ^ tNxtReqAxB[17];

	tNxtReqIxA[8:0] = tNxtReqAxA[8:0] ^ tNxtReqAxA[16:8];
	tNxtReqIxB[8:0] = tNxtReqAxB[8:0] ^ tNxtReqAxB[16:8];
`endif

`ifdef jx2_mem_l1dsz_1024
//	tNxtReqIxA[7:0] = tNxtReqAxA[7:0] ^ tNxtReqAxA[15:8];
//	tNxtReqIxB[7:0] = tNxtReqAxB[7:0] ^ tNxtReqAxB[15:8];
//	tNxtReqIxA[9:8] = tNxtReqAxA[17:16] ^ tNxtReqAxA[19:18];
//	tNxtReqIxB[9:8] = tNxtReqAxB[17:16] ^ tNxtReqAxB[19:18];

	tNxtReqIxA[9:0] = tNxtReqAxA[9:0] ^ tNxtReqAxA[17:8];
	tNxtReqIxB[9:0] = tNxtReqAxB[9:0] ^ tNxtReqAxB[17:8];
`endif

	tNxtReqInValA	= regInValA;
	tNxtReqInValB	= regInValB;

	if(dcInHold)
	begin
		tReq1IxA = tReqIxA;
		tReq1IxB = tReqIxB;
	end
	else
	begin
		tReq1IxA = tNxtReqIxA;
		tReq1IxB = tNxtReqIxB;
	end
`endif

	if(((tInOpm==JX2_DCOPM_FLUSHDS) && (tInOpmC!=JX2_DCOPM_FLUSHDS)) || reset)
	begin
		tNxtDoFlush = 1;
	end
	
	if((tFlushRov == 0) && !tDoFlush)
		tNxtDoFlush = 1;

	if(tDoFlush && !tDoFlushL)
	begin
		$display("L1 D$ DoFlush rov=%X", tFlushRov);
		tNxtFlushRov = tFlushRov + 1;
	end

	/* EX2 */

	tReg2MissInterlockA = 0;
	tReg2MissInterlockB = 0;
	tReg2StoreFwA		= 0;
	tReg2StoreFwB		= 0;

	tArrMemAddrStA = 0;
	tArrMemDataStA = 0;
	tArrMemIdxStA = 0;
	tArrMemDoStA = 0;
	
	tArrMemAddrStB = 0;
	tArrMemDataStB = 0;
	tArrMemIdxStB = 0;
	tArrMemDoStB = 0;
	
	if(tReq2Opm[5] && !tReq2IsMmio && !tReq2StoreSticky)
	begin
		tReg2MissInterlockA		= (tReqIxA == tReq2IxA) && !tReq2MissSkipA;
		tReg2MissInterlockB		= (tReqIxB == tReq2IxB) && !tReq2MissSkipB;
	end


	tReqNoCross		= 0;
	tReqMissSkipA	= 0;
	tReqMissSkipB	= 0;

	tNxtMemMmioData = tMemMmioData;
	tNxtMemMmioReady = tMemMmioReady;
	

	tReqIsNz		= tReqOpm[5:4] != 2'b00;
	tReqIsMmio		= tReqIsNz &&
		((tReqAddr[47:32] == 16'h0000) ||
		 (tReqAddr[47:32] == 16'hFFFF)) &&
		(tReqAddr[31:28] == 4'hF);
	tReqIsCcmd		= (tReqOpm[5:4] == 2'b00) && (tReqOpm[3:0] != 4'b0000);

	tReqNoCross		= 0;
	casez(tReqOpm[2:0])
		3'bz00:		tReqNoCross = 1;
		3'bz01:		tReqNoCross = !tReqBix[0];
		3'bz10:		tReqNoCross = (tReqBix[1:0]==2'h0);
		3'b011:		tReqNoCross = (tReqBix[2:0]==3'h0);
		3'b111:		tReqNoCross = (tReqBix[3:0]==4'h0);
//		default:	tReqNoCross		= 0;
	endcase

// `ifndef def_true	
`ifdef def_true	
	if(tReqNoCross)
	begin
		if(tReqBix[4])
			tReqMissSkipA	= 1;
		else
			tReqMissSkipB	= 1;
	end
`endif

	tReqReadyA	= (tBlkMemIdxA == tReqIxA);
	tReqReadyB	= (tBlkMemIdxB == tReqIxB);
	tReqReady	= tReqReadyA && tReqReadyB;

`ifdef def_true
// `ifndef def_true
	tBlkMemAddr2A = tBlkMemAddrA;
	tBlkMemAddr2B = tBlkMemAddrB;

`ifndef def_true
	if(tReg2MissInterlockA)
	begin
		tBlkMemAddr2A = {
			tBlk2MemAddrA[71:5], 1'b1,
			tBlk2MemAddrA[3:0]};
	end
	if(tReg2MissInterlockB)
	begin
		tBlkMemAddr2B = {
			tBlk2MemAddrB[71:5], 1'b1,
			tBlk2MemAddrB[3:0]};
	end
`endif

	tReqMissAddrA	= tBlkMemAddr2A[47:5] != tReqAxA[43:1];
	tReqMissAddrB	= tBlkMemAddr2B[47:5] != tReqAxB[43:1];
//	tReqMissA	= tReqMissAddrA;
//	tReqMissB	= tReqMissAddrB;
	tReqMissA	= tReqMissAddrA && !tReqMissSkipA;
	tReqMissB	= tReqMissAddrB && !tReqMissSkipB;
	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz && tReqReady;
	
//	if(tReqMiss && !tArrMemDidStA && !tArrMemDidStB &&
//		(tReqOpm[2:0]!=3'b111))
//	if(tReqMiss && !tArrMemDidStA && !tArrMemDidStB)
	if(tReqMiss)
	begin
		tReqMissSkipA	= 0;
		tReqMissSkipB	= 0;
	end
`endif

	if(tReqMissSkipA)
		tReg2MissInterlockA = 0;
	if(tReqMissSkipB)
		tReg2MissInterlockB = 0;


	tBlkMemData2A = tBlkMemDataA;
	tBlkMemData2B = tBlkMemDataB;
	tBlkMemAddr2A = tBlkMemAddrA;
	tBlkMemAddr2B = tBlkMemAddrB;

`ifdef def_true
// `ifndef def_true

//	if(tReg2MissInterlockA)
//	begin
//		tReg2StoreFwA = 1;
//	end
//	if(tReg2MissInterlockB)
//	begin
//		tReg2StoreFwB = 1;
//	end

//	if(tReg2StoreFwA)
	if(tReg2MissInterlockA)
	begin
		tReg2StoreFwA = 1;
//		tReg2MissInterlockA = 0;

//		tBlkMemData2A = tBlkMemDataA;
//		tBlkMemAddr2A = tBlkMemAddrA;
		if(tReq2Bix[4])
		begin
			tBlkMemData2A = tBlk2InsData[255:128];
		end else begin
			tBlkMemData2A = tBlk2InsData[127:  0];
		end
		tBlkMemAddr2A = {
			tBlk2MemAddrA[71:5], 1'b1,
			tBlk2MemAddrA[3:0]};
	end

//	if(tReg2StoreFwB)
	if(tReg2MissInterlockB)
	begin
//		tReg2MissInterlockB = 0;
		tReg2StoreFwB = 1;

		if(!tReq2Bix[4])
		begin
			tBlkMemData2B = tBlk2InsData[255:128];
		end else begin
			tBlkMemData2B = tBlk2InsData[127:  0];
		end
		tBlkMemAddr2B = {
			tBlk2MemAddrB[71:5], 1'b1,
			tBlk2MemAddrB[3:0]};
	end
`endif

//	if(tArrMemDidStA || tArrMemDidStB)
//		tReqReady	= 0;

`ifdef def_true
// `ifndef def_true
	if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA) && !tReg2StoreFwA)
	begin
		tBlkMemData2A = tArrMemDidStDataA;
		tBlkMemAddr2A = tArrMemDidStAddrA;
	end
	if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB) && !tReg2StoreFwB)
	begin
		tBlkMemData2B = tArrMemDidStDataB;
		tBlkMemAddr2B = tArrMemDidStAddrB;
	end
`else
	if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
	begin
//		$display("L1 D$: Line A Store Clash");
		tReqReady	= 0;
	end
	if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
	begin
//		$display("L1 D$: Line B Store Clash");
		tReqReady	= 0;
	end
`endif

//	if(!tReqReady && !tReqIsMmio)
//	if(!tReqReady && tReqIsNz && !tReqIsMmio)
//		tRegOutHold = 1;

	tReqReady2	= tReqReady && tReqReadyL;


	tBlkIsDirtyA	= tBlkMemAddr2A[4];
	tBlkIsDirtyB	= tBlkMemAddr2B[4];

	tReqMissAddrA	= tBlkMemAddr2A[47:5] != tReqAxA[43:1];
	tReqMissAddrB	= tBlkMemAddr2B[47:5] != tReqAxB[43:1];
	tReqFlushAddrA	= tBlkMemAddr2A[71:68] != tFlushRov;
	tReqFlushAddrB	= tBlkMemAddr2B[71:68] != tFlushRov;

	tReqMissA	= (tReqMissAddrA && !tReqMissSkipA) || tReqFlushAddrA;
	tReqMissB	= (tReqMissAddrB && !tReqMissSkipB) || tReqFlushAddrB;
	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz;

	if(!tReqIsNz)
	begin
		tReqMissA	= 0;
		tReqMissB	= 0;
	end

// `ifdef def_true
`ifndef def_true
	if(tReqMiss)
	begin
		tReqMissSkipA = 0;
		tReqMissSkipB = 0;
		tReqMissA	= tReqMissAddrA || tReqFlushAddrA;
		tReqMissB	= tReqMissAddrB || tReqFlushAddrB;
		tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz;
	end
`endif

	if(tReqIsNz && !tReqIsMmio)
	begin
		if(!tReqReady)
		begin
//			$display("L1 D$: Not Ready Stall");
			tRegOutHold = 1;
		end

		if(tReqMiss)
		begin
//			$display("L1 D$: Miss Stall");
			tRegOutHold = 1;
		end

		if(	(tReg2MissInterlockA && !tReg2StoreFwA) ||
			(tReg2MissInterlockB && !tReg2StoreFwB)	)
		begin
//			$display("L1 D$: Interlock Stall");
			tRegOutHold = 1;
		end
	end
	
	if(tReqBix[4])
	begin
		tBlkExData0 = { tBlkMemData2A, tBlkMemData2B };
	end	else begin
		tBlkExData0 = { tBlkMemData2B, tBlkMemData2A };
	end

	tBlkExData1 = tReqBix[3] ? tBlkExData0[191:64] : tBlkExData0[127: 0];
	tBlkExData2 = tReqBix[2] ? tBlkExData1[119:32] : tBlkExData1[ 87: 0];
	tBlkExData3 = tReqBix[1] ? tBlkExData2[ 87:16] : tBlkExData2[ 71: 0];
	tBlkExData4 = tReqBix[0] ? tBlkExData3[ 71: 8] : tBlkExData3[ 63: 0];

	casez(tReqOpm[2:0])
		3'b000: tReqSx = tBlkExData4[7];
		3'b001: tReqSx = tBlkExData4[15];
		3'b010: tReqSx = tBlkExData4[31];
		3'b011: tReqSx = 0;
		3'b1zz: tReqSx = 0;
	endcase

	tBlkExData = tBlkExData4;
	if(tReqOpm[1:0]==2'b00)
		tBlkExData[15:8] = tReqSx ? UV8_FF : UV8_00;
	if(tReqOpm[1]==1'b0)
		tBlkExData[31:16] = tReqSx ? UV16_FF : UV16_00;
	if(tReqOpm[1:0]!=2'b11)
		tBlkExData[63:32] = tReqSx ? UV32_FF : UV32_00;

	tBlkExDataB = tBlkExData1[127:64];

	if(tReqOpm[2:0]==3'b111)
	begin
		tBlkExDataA = tBlkExData1[ 63: 0];
//		tBlkExDataB = tBlkExData1[127:64];
	end
	else
	begin
		tBlkExDataA = tBlkExData;
//		tBlkExDataB = UV64_00;
	end
	
	tBlkInsData4 = tReqInValA;
	if(tReqOpm[1:0]==2'b00)
		tBlkInsData4[15:8] = tBlkExData4[15:8];
	if(tReqOpm[1]==1'b0)
		tBlkInsData4[31:16] = tBlkExData4[31:16];
	if(tReqOpm[1:0]!=2'b11)
		tBlkInsData4[63:32] = tBlkExData4[63:32];

	tBlkInsData3 = tReqBix[0] ?
		{ tBlkInsData4[63: 0], tBlkExData3 [ 7:0] } :
		{ tBlkExData3 [71:64], tBlkInsData4[63:0] } ;
	tBlkInsData2 = tReqBix[1] ?
		{ tBlkInsData3[71: 0], tBlkExData2 [15:0] } :
		{ tBlkExData2 [87:72], tBlkInsData3[71:0] } ;
	tBlkInsData1 = tReqBix[2] ?
		{ tBlkExData1 [127:120], tBlkInsData2[ 87: 0], tBlkExData1 [31:0] } :
		{ tBlkExData1 [127:120], tBlkExData1 [119:88], tBlkInsData2[87:0] } ;

	if(tReqOpm[2:0]==3'b111)
	begin
		tBlkInsData1 = { tReqInValB, tReqInValA };
	end

	tBlkInsData0 = tReqBix[3] ?
		{ tBlkExData0 [255:192], tBlkInsData1[127:  0], tBlkExData0 [ 63:0] } :
		{ tBlkExData0 [255:192], tBlkExData0 [191:128], tBlkInsData1[127:0] } ;

`ifndef def_true
// `ifdef def_true
	if(tReqOpm[5:4]!=0)
	begin
		$display("L1 D$, D-Ex =%X", tBlkExData0);
		$display("L1 D$, D-Ins=%X", tBlkInsData0);
		$display("       Ready=%X MissA=%X MissB=%X",
			tReqReady, tReqMissA, tReqMissB);
	end
`endif

	if(tReqIsMmio || tReqIsCcmd)
	begin
		if(!tMemMmioReady)
		begin
//			$display("L1 D$: MMIO Stall");			
			tRegOutHold = 1;
		end
//		tBlkExDataA = tNxtMemMmioData;
		tBlkExDataA = tMemMmioData;

`ifdef def_true
		if(tReqOpm[1:0]!=2'b11)
		begin
			if(tMemMmioData[31] && !tReqOpm[2])
				tBlkExDataA[63:32]=UV32_FF;
			else
				tBlkExDataA[63:32]=UV32_00;
		end
`endif

//		if(tMemMmioReady)
//			$display("MMIO result is Ready");
	end
	else
	begin
		tNxtMemMmioReady = 0;
	end

	/* EX3 */

	tNxtReq2StoreSticky = 0;

	if(memRingIsRespOkStA)
	begin
//		$display("L1D$: Store Response A, A=%X", memAddrIn);
		tNxtMemRespStA = 1;
	end
	
	if(memRingIsRespOkStB)
	begin
//		$display("L1D$: Store Response B, A=%X", memAddrIn);
		tNxtMemRespStB = 1;
	end
	
`ifndef def_true
// `ifdef def_true
	if(memRingIsRespOkLdA)
	begin
		if(tMemReqStA && !tMemRespStA)
		begin
//			$display("L1D$: Response Order Skip A");
			tMemRingSkipResp = 1;
		end
	end

	if(memRingIsRespOkLdB)
	begin
		if(tMemReqStB && !tMemRespStB)
		begin
//			$display("L1D$: Response Order Skip B");
			tMemRingSkipResp = 1;
		end
	end
`endif

	if(memRingIsRespOkLdA && !tMemRingSkipResp)
	begin
//		$display("L1D$: Load Response A, A=%X", memAddrIn);

		tArrMemAddrStA = {
			tFlushRov[3:0],
			memAddrIn[31:12],
			tReqSeqVa[43:1], 1'b0,
			memOpmIn[3:0]};
		tArrMemDataStA = memDataIn;
		tArrMemIdxStA = tReqSeqIdx;
		tArrMemDoStA = 1;
		tNxtMemRespLdA = 1;
//		tNxtMemRespStA = 0;
//		tNxtMemRespLdA = 0;
	end

	if(memRingIsRespOkLdB && !tMemRingSkipResp)
	begin
//		$display("L1D$: Load Response B, A=%X", memAddrIn);

		tArrMemAddrStB = {
			tFlushRov[3:0],
			memAddrIn[31:12],
			tReqSeqVa[43:1], 1'b0,
			memOpmIn[3:0]};
		tArrMemDataStB = memDataIn;
		tArrMemIdxStB = tReqSeqIdx;
		tArrMemDoStB = 1;
		tNxtMemRespLdB = 1;
//		tNxtMemRespStB = 0;
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
		tRegOutHold = 1;
`endif

	if(memRingIsRespOkMmio)
	begin
//		$display("L1 D$: MMIO Response Seen");
		tNxtMemMmioData = memDataIn[63:0];
		tNxtMemMmioReady = 1;
	end
	
//	if(!dcInHold)
//		tNxtMemMmioReady = 0;

	if(		memRingIsRespOkLdA ||
			memRingIsRespOkLdB ||
			tArrMemDidStA || tArrMemDidStB)
	begin
//		$display("L1 D$: WriteBack Stall");
//		tRegOutHold = 1;
	end

	if(tReq2Opm[5] && !tReq2IsMmio)
	begin
		tNxtReq2StoreSticky = tReq2StoreSticky;

//		$display("Store Sticky=%d Data=%X",
//			tReq2StoreSticky, tBlk2InsData);

		if(!tReq2Ready)
			$display("L1 D$: Store Fault: Non-Ready State");


		/* Stall if store needs to be delayed. */
//		if(tArrMemDoStA || tArrMemDoStB ||
//			tArrMemDidStA || tArrMemDidStB)
//		if(	((tArrMemDoStA || tArrMemDidStA) && !tReq2MissSkipA) || 
//			((tArrMemDoStB || tArrMemDidStB) && !tReq2MissSkipB) )
		if(	(tArrMemDoStA && !tReq2MissSkipA) || 
			(tArrMemDoStB && !tReq2MissSkipB) )
		begin
			if(!tReq2StoreSticky)
			begin
//				$display("L1 D$: Store Sticky Stall");
				tRegOutHold = 1;
			end
		end
		else if(!tReq2StoreSticky)
		begin
			tArrMemAddrStA = {
				tBlk2MemAddrA[71:5], 1'b1,
				tBlk2MemAddrA[3:0]};
			tArrMemIdxStA = tReq2IxA;

			tArrMemAddrStB = {
				tBlk2MemAddrB[71:5], 1'b1,
				tBlk2MemAddrB[3:0]};
			tArrMemIdxStB = tReq2IxB;

			tNxtReq2StoreSticky = 1;

			if(tReq2Bix[4])
			begin
				tArrMemDataStA = tBlk2InsData[255:128];
				tArrMemDataStB = tBlk2InsData[127:  0];
			end else begin
				tArrMemDataStA = tBlk2InsData[127:  0];
				tArrMemDataStB = tBlk2InsData[255:128];
			end

			tArrMemDoStA = !tReq2MissSkipA;
			tArrMemDoStB = !tReq2MissSkipB;

`ifndef def_true
			if(tReq2MissA && tArrMemDoStA)
			begin
				$display("L1 D$: Store Fault: MissA");
				tArrMemDoStA = 0;
			end

			if(tReq2MissB && tArrMemDoStB)
			begin
				$display("L1 D$: Store Fault: MissB");
				tArrMemDoStB = 0;
			end
`endif
			
//			$display("%X %X", tArrMemDoStA, tArrMemDoStB)
		end
	end
	
	/* Miss Handling */

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UV48_00;
	tMemDataReq		= UV128_XX;
	tNxtMemReqStA	= 0;
	tNxtMemReqStB	= 0;
	tNxtMemReqLdA	= 0;
	tNxtMemReqLdB	= 0;
	tNxtMemReqLdM	= 0;
	tMemSeqIx		= 0;
	tMemSeqVa		= 0;
	tNxtMemSeqRov	= tMemSeqRov;

//	if(tReqIsMmio && !tMemMmioReady)
	if(reset)
	begin
		tNxtMemSeqRov	= 0;
	end
	else
		if(tReqIsMmio || tReqIsCcmd)
	begin
		tNxtMemReqLdM	= tMemReqLdM;
//		tNxtMemSeqRov	= tMemSeqRov;
		tRegOutWait		= 1;

		if(!tMemReqLdM)
		begin
//			$display("L1 D$ MMIO Req A=%X", tReqAddr);
		
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
			tMemDataReq		= { UV64_00, tReqInValA };
//			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDSQ };
			tMemAddrReq		= tReqAddr;
			tNxtMemReqLdM	= 1;

			if(tReqIsCcmd)
			begin
				tMemOpmReq	= { UV8_00, 2'b10, tReqOpm[5:0] };
			end
			else
				if(tReqOpm[2:0]!=3'b011)
			begin
				tMemOpmReq	= { UV8_00,
					(tReqOpm[5]) ?
						JX2_RBI_OPM_STSL :
						JX2_RBI_OPM_LDSL };
			end
			else
			begin
				tMemOpmReq	= { UV8_00,
					(tReqOpm[5]) ?
						JX2_RBI_OPM_STSQ :
						JX2_RBI_OPM_LDSQ };
			end
		end
	end
	else
//		if((tReqMissA || tReqMissB) && tReqReady)
		if((tReqMissA || tReqMissB || tReqWaitResp) && tReqReady)
//		if(tReqMissA || tReqMissB)
	begin
		tNxtMemReqStA	= tMemReqStA;
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqStB	= tMemReqStB;
		tNxtMemReqLdB	= tMemReqLdB;
//		tNxtMemSeqRov	= tMemSeqRov;

		tRegOutWait		= 1;

`ifndef def_true
// `ifdef def_true
		if(tArrMemDidStA && tReg2MissInterlockA && !tMemReqLdA)
			tNxtMemReqStA	= 0;
		if(tArrMemDidStB && tReg2MissInterlockB && !tMemReqLdB)
			tNxtMemReqStB	= 0;

		if(tBlkIsDirtyA && !tMemReqStA)
				tNxtMemReqLdA = 0;
		if(tBlkIsDirtyB && !tMemReqStB)
				tNxtMemReqLdB = 0;
`endif

`ifndef def_true
// `ifdef def_true
		if(!tReqReady)
		begin
			/* Wait */
		end
		else
`endif

`ifndef def_true
// `ifdef def_true
		if(	tArrMemDidStA || tReg2MissInterlockA ||
			tArrMemDidStB || tReg2MissInterlockB)
		begin
		end
		else
`endif
			if(tReqMissA && !tMemReqLdA)
//			if((tReqMissA || tReqMissAddrA) && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
			tMemDataReq		= tBlkMemDataA;
			
//			$display("L1 D$ MissA Ix=%X Dirty=%X St=%X D=%X",
//				tBlkMemIdxA,
//				tBlkIsDirtyA, tMemReqStA, tBlkMemDataA);

//			if(tArrMemDidStA)
			if(tArrMemDidStA || tReg2MissInterlockA)
			begin
			end
//			else if(tBlkIsDirtyA && !tMemReqStA && !tArrMemDidStA)
			else if(tBlkIsDirtyA && !tMemReqStA)
			begin
//				$display("L1 D$: Send STA Req, A=%X", tBlkMemAddrA[47:4]);
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
				tMemAddrReq		= {
					JX2_RBI_ADDRHI_PHYS,
					tBlkMemAddrA[67:48],
					tBlkMemAddrA[11: 5],
					5'h00 };
				tNxtMemReqStA	= 1;
			end
			else if(!tReg2MissInterlockA && !tMemReqLdA)
			begin
//				$display("L1 D$: Send LDA Req A=%X", tReqAxA);
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
				tMemAddrReq		= { tReqAxA, 4'h00 };
				tNxtMemReqLdA = 1;
			end
		end
		else if(tReqMissB && !tMemReqLdB)
//		else if((tReqMissB || tReqMissAddrB) && !tMemReqLdB)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov};
			tMemDataReq		= tBlkMemDataB;

//			$display("L1 D$ MissB Ix=%X Dirty=%X St=%X D=%X",
//				tBlkMemIdxB,
//				tBlkIsDirtyB, tMemReqStB, tBlkMemDataB);

//			if(tArrMemDidStB)
			if(tArrMemDidStB || tReg2MissInterlockB)
			begin
			end
//			else if(tBlkIsDirtyB)
			else if(tBlkIsDirtyB && !tMemReqStB)
			begin
//				$display("L1 D$: Send STB Req, A=%X", tBlkMemAddrB[47:4]);
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
				tMemAddrReq		= {
					JX2_RBI_ADDRHI_PHYS,
					tBlkMemAddrB[67:48],
					tBlkMemAddrB[11: 5],
					1'b1, 4'h0 };
				tNxtMemReqStB = 1;
			end
			else if(!tReg2MissInterlockB && !tMemReqLdB)
			begin
//				$display("L1 D$: Send LDB Req, A=%X", tReqAxB);
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
				tMemAddrReq		= { tReqAxB, 4'h00 };
				tNxtMemReqLdB = 1;
			end
		end
	end
// `ifdef def_true
`ifndef def_true
	else if(tReqReady &&
		!tReqMissA && !tReqMissB &&
		(tReqMissAddrA || tReqMissAddrB))
	begin
		if(tReqMissAddrA && !tBlkIsDirtyA && !tReg2MissInterlockA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov};
			tMemDataReq		= tBlkMemDataA;

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
			tMemAddrReq		= { tReqAxA, 4'h00 };
			tNxtMemReqLdA	= 1;
		end

		if(tReqMissAddrB && !tBlkIsDirtyB && !tReg2MissInterlockB)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov};
			tMemDataReq		= tBlkMemDataB;

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
			tMemAddrReq		= { tReqAxB, 4'h00 };
			tNxtMemReqLdB	= 1;
		end
	end
`endif
	else
	begin
		if(	tMemReqStA | tMemReqStB |
			tMemReqLdA | tMemReqLdB |
			tMemReqLdM )
		begin
//			$display("L1 D$: Wait Response");
			tRegOutHold = 1;
		end
	end

//	if(tReqIsMmio && tMemMmioReady)
//		tRegOutHold = 0;

end

always @(posedge clock)
begin

	if(!dcInHold)
	begin
		/* EX1 -> EX2 */
		tReqAddr		<= tNxtReqAddr;
		tReqIxA			<= tNxtReqIxA;
		tReqIxB			<= tNxtReqIxB;
		tReqAxA			<= tNxtReqAxA;
		tReqAxB			<= tNxtReqAxB;
		tReqBix			<= tNxtReqBix;
		tReqOpm			<= tNxtReqOpm;
		tReqInValA		<= tNxtReqInValA;
		tReqInValB		<= tNxtReqInValB;

		/* EX2 -> EX3 */
		tRegOutValA		<= tBlkExDataA;
		tRegOutValB		<= tBlkExDataB;

		tBlk2MemAddrA	<= tBlkMemAddrA;
		tBlk2MemAddrB	<= tBlkMemAddrB;

		tBlk2InsData	<= tBlkInsData0;
		tReq2IxA		<= tReqIxA;
		tReq2IxB		<= tReqIxB;
		tReq2AxA		<= tReqAxA;
		tReq2AxB		<= tReqAxB;
		tReq2Opm		<= tReqOpm;
		tReq2Bix		<= tReqBix;
		tReq2Ready		<= tReqReady;

		tReq2MissA		<= tReqMissA;
		tReq2MissB		<= tReqMissB;
		tReq2IsMmio		<= tReqIsMmio;
		tReq2IsCcmd		<= tReqIsCcmd;

		tReq2NoCross		<= tReqNoCross;
		tReq2MissSkipA		<= tReqMissSkipA;
		tReq2MissSkipB		<= tReqMissSkipB;

		tReq2StoreSticky	<= 0;
		tMemMmioData		<= 0;
		tMemMmioReady		<= 0;

	end
	else
	begin
		tReq2StoreSticky	<= tNxtReq2StoreSticky;
		tMemMmioData		<= tNxtMemMmioData;
		tMemMmioReady		<= tNxtMemMmioReady;
	end

	tFlushRov		<= tNxtFlushRov;

	tReqReadyL		<= tReqReady;

	tInOpm			<= tNxtReqOpm;
	tInOpmB			<= tInOpm;
	tInOpmC			<= tInOpmB;
	tDoFlush		<= tNxtDoFlush;
	tDoFlushL		<= tDoFlush;


	/* EX1->EX2 */
	tBlkMemAddrA	<= arrMemAddrA[tReq1IxA];
	tBlkMemAddrB	<= arrMemAddrB[tReq1IxB];
	tBlkMemDataA	<= arrMemDataA[tReq1IxA];
	tBlkMemDataB	<= arrMemDataB[tReq1IxB];
	tBlkMemIdxA		<= tReq1IxA;
	tBlkMemIdxB		<= tReq1IxB;


	if(tArrMemDoStA)
	begin
//		$display("L1 D$ STA Ix=%X A=%X D=%X",
//			tArrMemIdxStA, tArrMemAddrStA, tArrMemDataStA);
		arrMemAddrA[tArrMemIdxStA]	<= tArrMemAddrStA;
		arrMemDataA[tArrMemIdxStA]	<= tArrMemDataStA;
	end

	if(tArrMemDoStB)
	begin
//		$display("L1 D$ STB Ix=%X A=%X D=%X",
//			tArrMemIdxStB, tArrMemAddrStB, tArrMemDataStB);
		arrMemAddrB[tArrMemIdxStB]	<= tArrMemAddrStB;
		arrMemDataB[tArrMemIdxStB]	<= tArrMemDataStB;
	end

	tArrMemDidStAddrA	<= tArrMemAddrStA;
	tArrMemDidStDataA	<= tArrMemDataStA;
	tArrMemDidStAddrB	<= tArrMemAddrStB;
	tArrMemDidStDataB	<= tArrMemDataStB;

	tArrMemDidStIxA	<= tArrMemIdxStA;
	tArrMemDidStIxB	<= tArrMemIdxStB;
	tArrMemDidStA	<= tArrMemDoStA;
	tArrMemDidStB	<= tArrMemDoStB;

//	tMemRespStA		<= tNxtMemRespStA;
//	tMemRespStB		<= tNxtMemRespStB;
//	tMemRespLdA		<= tNxtMemRespLdA;
//	tMemRespLdB		<= tNxtMemRespLdB;

	tMemRespStA		<= dcInHold ? tNxtMemRespStA : 0;
	tMemRespStB		<= dcInHold ? tNxtMemRespStB : 0;
	tMemRespLdA		<= dcInHold ? tNxtMemRespLdA : 0;
	tMemRespLdB		<= dcInHold ? tNxtMemRespLdB : 0;

	if(reset)
	begin
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;

		tMemReqStA	<= 0;
		tMemReqStB	<= 0;
		tMemReqLdA	<= 0;
		tMemReqLdB	<= 0;
		tMemReqLdM	<= 0;
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

//		tMemReqStA	<= tNxtMemReqStA;
//		tMemReqStB	<= tNxtMemReqStB;
//		tMemReqLdA	<= tNxtMemReqLdA;
//		tMemReqLdB	<= tNxtMemReqLdB;
		tMemSeqRov	<= tNxtMemSeqRov;

		tMemReqStA	<= dcInHold ? tNxtMemReqStA : 0;
		tMemReqStB	<= dcInHold ? tNxtMemReqStB : 0;
		tMemReqLdA	<= dcInHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= dcInHold ? tNxtMemReqLdB : 0;

		tMemReqLdM	<= dcInHold ? tNxtMemReqLdM : 0;
//		tMemReqLdM	<= tNxtMemReqLdM;

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

		tMemReqStA	<= dcInHold ? tMemReqStA : 0;
		tMemReqStB	<= dcInHold ? tMemReqStB : 0;
		tMemReqLdA	<= dcInHold ? tMemReqLdA : 0;
		tMemReqLdB	<= dcInHold ? tMemReqLdB : 0;
		tMemReqLdM	<= dcInHold ? tMemReqLdM : 0;
	end
end

endmodule

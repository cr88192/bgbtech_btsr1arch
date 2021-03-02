`include "RbiDefs.v"

module RbiMemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutValA,		regInValA,
	regOutValB,		regInValB,
	dcInHold,		regOutHold,
	regInSr,

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
assign		regOutHold = tRegOutHold;

wire			memRingIsIdle;
wire			memRingIsResp;
assign		memRingIsIdle = (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		memRingIsResp =
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire			memRingIsRespOkLd;
wire			memRingIsRespOkLdA;
wire			memRingIsRespOkLdB;
wire			memRingIsRespOkMmio;
assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkLdA =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkLdB =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b01);
assign		memRingIsRespOkMmio =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);

/*
Addr:
  (71:68): Flush Rover
  (67:48): PA Bits
  (47: 5): VA Bits
  ( 4: 0): Access Flags
 */

reg[ 71:0]		arrMemAddrA[255:0];
reg[ 71:0]		arrMemAddrB[255:0];
reg[127:0]		arrMemDataA[255:0];
reg[127:0]		arrMemDataB[255:0];

reg[ 71:0]		tArrMemAddrStA;
reg[ 71:0]		tArrMemAddrStB;
reg[127:0]		tArrMemDataStA;
reg[127:0]		tArrMemDataStB;
reg[  7:0]		tArrMemIdxStA;
reg[  7:0]		tArrMemIdxStB;
reg				tArrMemDoStA;
reg				tArrMemDoStB;
reg				tArrMemDidStA;
reg				tArrMemDidStB;

reg[  7:0]		tReqSeqIdxArr[15:0];
reg[ 43:0]		tReqSeqVaArr[15:0];

reg[  7:0]		tReqSeqIdx;
reg[ 43:0]		tReqSeqVa;

reg[  3:0]		tFlushRov;
reg[  3:0]		tNxtFlushRov;

reg[  7:0]		tNxtReqIxA;
reg[  7:0]		tNxtReqIxB;
reg[ 43:0]		tNxtReqAxA;
reg[ 43:0]		tNxtReqAxB;
reg[  7:0]		tReqIxA;
reg[  7:0]		tReqIxB;
reg[ 43:0]		tReqAxA;
reg[ 43:0]		tReqAxB;
reg[  7:0]		tReq1IxA;
reg[  7:0]		tReq1IxB;

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

reg				tReqMissAddrA;
reg				tReqMissAddrB;
reg				tReqMissA;
reg				tReqMissB;
reg				tReqMiss;
reg				tReqSx;

reg				tReqFlushAddrA;
reg				tReqFlushAddrB;

reg				tReqNoCross;
reg				tReqMissSkipA;
reg				tReqMissSkipB;
reg				tReq2NoCross;
reg				tReq2MissSkipA;
reg				tReq2MissSkipB;

reg				tReqIsNz;
reg				tReqIsMmio;


reg[ 71:0]		tBlkMemAddrA;
reg[ 71:0]		tBlkMemAddrB;
reg[127:0]		tBlkMemDataA;
reg[127:0]		tBlkMemDataB;
reg[  7:0]		tBlkMemIdxA;
reg[  7:0]		tBlkMemIdxB;
reg				tBlkIsDirtyA;
reg				tBlkIsDirtyB;

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
reg[  7:0]		tReq2IxA;
reg[  7:0]		tReq2IxB;
reg[ 43:0]		tReq2AxA;
reg[ 43:0]		tReq2AxB;
reg[  5:0]		tReq2Opm;
reg[  4:0]		tReq2Bix;

reg				tReq2StoreSticky;
reg				tNxtReq2StoreSticky;

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

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
reg				tMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
reg				tNxtMemReqLdM;	//Load MMIO

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

reg[7:0]		tMemSeqIx;
reg[43:0]		tMemSeqVa;


always @*
begin
	tReqSeqIdx = tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa = tReqSeqVaArr[memSeqIn[3:0]];

	tArrMemDoStA = 0;
	tArrMemDoStB = 0;
	tRegOutHold = 0;
	
	tNxtFlushRov = tFlushRov;


	/* EX1 */

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
	
	tNxtReqBix		= regInAddr[4:0];
	tNxtReqOpm		= regInOpm;
	tNxtReqIxA[7:0] = tNxtReqAxA[7:0] ^ tNxtReqAxA[15:8];
	tNxtReqIxB[7:0] = tNxtReqAxB[7:0] ^ tNxtReqAxB[15:8];

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


	/* EX2 */
	
	tReqNoCross		= 0;
	tReqMissSkipA	= 0;
	tReqMissSkipB	= 0;

	tNxtMemMmioData = tMemMmioData;
	tNxtMemMmioReady = tMemMmioReady;
	

	tReqIsNz		= tReqOpm[5:4] != 2'b00;
	tReqIsMmio		=
		(tReqAxA[43:28] == 0) &&
		(tReqAxA[27:24] == 4'hF);

	casez(tReqOpm[2:0])
		3'bz00: tReqNoCross = 1;
		3'bz01: tReqNoCross = !tReqBix[0];
		3'bz10: tReqNoCross = (tReqBix[1:0]==2'h0);
		3'b011: tReqNoCross = (tReqBix[2:0]==3'h0);
		3'b111: tReqNoCross = (tReqBix[3:0]==4'h0);
	endcase
	
	if(tReqNoCross)
	begin
		if(tReqBix[4])
			tReqMissSkipA	= 1;
		else
			tReqMissSkipB	= 1;
	end

	tBlkIsDirtyA	= tBlkMemAddrA[4];
	tBlkIsDirtyB	= tBlkMemAddrB[4];

	tReqReadyA	= (tBlkMemIdxA == tReqIxA);
	tReqReadyB	= (tBlkMemIdxB == tReqIxB);
	tReqReady	= tReqReadyA && tReqReadyB;

	tReqMissAddrA	= tBlkMemAddrA[47:5] != tReqAxA[43:1];
	tReqMissAddrB	= tBlkMemAddrB[47:5] != tReqAxB[43:1];
	tReqFlushAddrA	= tBlkMemAddrA[71:68] != tFlushRov;
	tReqFlushAddrB	= tBlkMemAddrB[71:68] != tFlushRov;

	tReqMissA	= (tReqMissAddrA && !tReqMissSkipA) || tReqFlushAddrA;
	tReqMissB	= (tReqMissAddrB && !tReqMissSkipA) || tReqFlushAddrB;
	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz;

	if(tReqIsNz)
	begin
		if(!tReqReady && !tReqIsMmio)
			tRegOutHold = 1;

		if(tReqMiss)
			tRegOutHold = 1;
	end
	
	if(tReqBix[4])
	begin
		tBlkExData0 = { tBlkMemDataA, tBlkMemDataB };
	end	else begin
		tBlkExData0 = { tBlkMemDataB, tBlkMemDataA };
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

	if(tReqOpm[2:0]==3'b111)
	begin
		tBlkExDataA = tBlkExData1[ 63: 0];
		tBlkExDataB = tBlkExData1[127:64];
	end
	else
	begin
		tBlkExDataA = tBlkExData;
		tBlkExDataB = UV64_00;
	end
	
	tBlkInsData4 = tReqInValA;
	if(tReqOpm[1:0]==2'b00)
		tBlkInsData4[15:8] = tBlkExData[15:8];
	if(tReqOpm[1]==1'b0)
		tBlkInsData4[31:16] = tBlkExData[31:16];
	if(tReqOpm[1:0]!=2'b11)
		tBlkInsData4[63:32] = tBlkExData[63:32];

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


	/* EX3 */

	tNxtReq2StoreSticky = 0;

	if(memRingIsRespOkLdA)
	begin
		tArrMemAddrStA = {
			tFlushRov[3:0],
			memAddrIn[31:12],
			tReqSeqVa[43:1], 1'b0,
			memOpmIn[3:0]};
		tArrMemDataStA = memDataIn;
		tArrMemIdxStA = tReqSeqIdx;
		tArrMemDoStA = 1;
	end

	if(memRingIsRespOkLdB)
	begin
		tArrMemAddrStB = {
			tFlushRov[3:0],
			memAddrIn[31:12],
			tReqSeqVa[43:1], 1'b0,
			memOpmIn[3:0]};
		tArrMemDataStB = memDataIn;
		tArrMemIdxStB = tReqSeqIdx;
		tArrMemDoStB = 1;
	end

	if(memRingIsRespOkMmio)
	begin
		tNxtMemMmioData = memDataIn[63:0];
		tNxtMemMmioReady = 1;
	end
	
	if(tReqIsMmio)
	begin
		if(!tMemMmioReady)
			tRegOutHold = 1;
		tBlkExDataA = tNxtMemMmioData;
	end
	else
	begin
		tNxtMemMmioReady = 0;
	end
	
	if(!dcInHold)
		tNxtMemMmioReady = 0;

	if(		memRingIsRespOkLdA ||
			memRingIsRespOkLdB ||
			tArrMemDidStA || tArrMemDidStB)
		tRegOutHold = 1;

	if(tReq2Opm[5] && !tReqIsMmio)
	begin
		/* Read-After-Store, Stall */
		if(	((tReqIxA == tReq2IxA) ||
			 (tReqIxB == tReq2IxB)) &&
			 !tReq2StoreSticky)
		begin
			tRegOutHold = 1;
		end

		/* Stall if store needs to be delayed. */
		if(tArrMemDoStA || tArrMemDoStB)
		begin
			if(!tReq2StoreSticky)
				tRegOutHold = 1;
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

			if(tReqBix[4])
			begin
				tArrMemDataStA = tBlk2InsData[255:128];
				tArrMemDataStB = tBlk2InsData[127:  0];
				tArrMemDoStA = !tReq2MissSkipA;
				tArrMemDoStB = !tReq2MissSkipB;
			end
			else
			begin
				tArrMemDataStA = tBlk2InsData[127:  0];
				tArrMemDataStB = tBlk2InsData[255:128];
				tArrMemDoStA = !tReq2MissSkipA;
				tArrMemDoStB = !tReq2MissSkipB;
			end
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

	if(tReqIsMmio && !tMemMmioReady)
	begin
		tNxtMemReqLdM	= tMemReqLdM;
		
		if(!tMemReqLdM)
		begin
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
			tMemDataReq		= { UV64_00, tReqInValA };
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDSQ };
			tMemAddrReq		= { tReqAxA, tReqBix[3:0] };
			tNxtMemReqLdM	= 1;
		end
	end
	else
		if((tReqMissA || tReqMissB) && tReqReady)
	begin
		tNxtMemReqStA	= tMemReqStA;
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqStB	= tMemReqStB;
		tNxtMemReqLdB	= tMemReqLdB;
		tNxtMemSeqRov	= tMemSeqRov;

		if(tReqMissA && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
			tMemDataReq		= tBlkMemDataA;

			if(tBlkIsDirtyA && !tMemReqStA)
			begin
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
				tMemAddrReq		= {
					JX2_RBI_ADDRHI_PHYS,
					tBlkMemAddrA[67:48],
					tBlkMemAddrA[11: 5],
					5'h00 };
				tNxtMemReqStA	= 1;
			end
			else
			begin
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
				tMemAddrReq		= { tReqAxA, 4'h00 };
				tNxtMemReqLdA = 1;
			end
		end
		else if(tReqMissB && !tMemReqLdB)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tBlkMemIdxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov};
			tMemDataReq		= tBlkMemDataB;

			if(tBlkIsDirtyB && !tMemReqStB)
			begin
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
				tMemAddrReq		= {
					JX2_RBI_ADDRHI_PHYS,
					tBlkMemAddrB[67:48],
					tBlkMemAddrB[11: 5],
					5'h00 };
				tNxtMemReqStB = 1;
			end
			else
			begin
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
				tMemAddrReq		= { tReqAxB, 4'h00 };
				tNxtMemReqLdB = 1;
			end
		end
	end
	else
	begin
		if(	tMemReqStA | tMemReqStB |
			tMemReqLdA | tMemReqLdB |
			tMemReqLdM )
		begin
			tRegOutHold = 1;
		end
	end
	
end

always @(posedge clock)
begin

	if(!dcInHold)
	begin
		/* EX1 -> EX2 */
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

		tReq2NoCross		<= tReqNoCross;
		tReq2MissSkipA		<= tReqMissSkipA;
		tReq2MissSkipB		<= tReqMissSkipB;
		tReq2StoreSticky	<= 0;
	end
	else
	begin
		tReq2StoreSticky	<= tNxtReq2StoreSticky;
	end

	tFlushRov		<= tNxtFlushRov;

	tMemMmioData	<= tNxtMemMmioData;
	tMemMmioReady	<= tNxtMemMmioReady;


	/* EX1->EX2 */
	tBlkMemAddrA	<= arrMemAddrA[tReq1IxA];
	tBlkMemAddrB	<= arrMemAddrB[tReq1IxB];
	tBlkMemDataA	<= arrMemDataA[tReq1IxA];
	tBlkMemDataB	<= arrMemDataB[tReq1IxB];
	tBlkMemIdxA		<= tReq1IxA;
	tBlkMemIdxB		<= tReq1IxB;


	if(tArrMemDoStA)
	begin
		arrMemAddrA[tArrMemIdxStA]	<= tArrMemAddrStA;
		arrMemDataA[tArrMemIdxStA]	<= tArrMemDataStA;
	end
	if(tArrMemDoStB)
	begin
		arrMemAddrB[tArrMemIdxStB]	<= tArrMemAddrStB;
		arrMemDataB[tArrMemIdxStB]	<= tArrMemDataStB;
	end

	tArrMemDidStA	<= tArrMemDoStA;
	tArrMemDidStB	<= tArrMemDoStB;

	if(memRingIsIdle || memRingIsResp)
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqSent	<= 1;

		tMemReqStA	<= tNxtMemReqStA;
		tMemReqStB	<= tNxtMemReqStB;
		tMemReqLdA	<= tNxtMemReqLdA;
		tMemReqLdB	<= tNxtMemReqLdB;
		tMemReqLdM	<= tNxtMemReqLdM;
		tMemSeqRov	<= tNxtMemSeqRov;

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
	end
end

endmodule

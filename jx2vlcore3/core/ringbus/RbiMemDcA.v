`include "ringbus/RbiDefs.v"

module RbiMemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutValA,		regInValA,
	regOutValB,		regInValB,
	dcInHold,		regOutHold,
	regInSr,		regOutWait,
	regOutExc,		regInMmcr,
	regKrrHash,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,

	unitNodeId
	);

input			clock;
input			reset;

`input_vaddr	regInAddr;		//input address
input [ 5: 0]	regInOpm;		//operation mode

output[63: 0]	regOutValA;		//output data value (Low 128 / Lane A)
output[63: 0]	regOutValB;		//output data value (High 128 / Lane B)

input [63: 0]	regInValA;		//input data value (Low 128 / Lane A)
input [63: 0]	regInValB;		//input data value (High 128 / Lane B)

input			dcInHold;
output			regOutHold;
output			regOutWait;

input [63: 0]	regInSr;
input [63: 0]	regInMmcr;
output[63: 0]	regOutExc;

input[7:0]		regKrrHash;



input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l1addr	memAddrIn;		//memory input address
`output_l1addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[63: 0]	tRegOutValA;
reg[63: 0]	tRegOutValB;
assign	regOutValA = tRegOutValA;
assign	regOutValB = tRegOutValB;

reg[63: 0]	tRegOutExc;
reg[63: 0]	tRegOutExc2;
assign	regOutExc = tRegOutExc2;

reg[63: 0]		tRegInSr;


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l1addr	tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

reg				tRegOutHold;
reg				tRegOutHoldL;
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

reg		tSrJQ;

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
reg[143:0]		arrMemDataA[1023:0];
reg[143:0]		arrMemDataB[1023:0];
`endif

`ifdef jx2_mem_l1dsz_512
`define			reg_l1d_ix	reg[8:0]
reg[ 71:0]		arrMemAddrA[511:0];
reg[ 71:0]		arrMemAddrB[511:0];
reg[143:0]		arrMemDataA[511:0];
reg[143:0]		arrMemDataB[511:0];
`endif

`ifdef jx2_mem_l1dsz_256
`define			reg_l1d_ix	reg[7:0]
reg[ 71:0]		arrMemAddrA[255:0];
reg[ 71:0]		arrMemAddrB[255:0];
reg[143:0]		arrMemDataA[255:0];
reg[143:0]		arrMemDataB[255:0];
`endif

`ifdef jx2_mem_l1dsz_128
`define			reg_l1d_ix	reg[6:0]
(* ram_style = "distributed" *)
	reg[ 71:0]		arrMemAddrA[127:0];
(* ram_style = "distributed" *)
	reg[ 71:0]		arrMemAddrB[127:0];
(* ram_style = "distributed" *)
	reg[143:0]		arrMemDataA[127:0];
(* ram_style = "distributed" *)
	reg[143:0]		arrMemDataB[127:0];
`endif

`ifdef jx2_mem_l1dsz_64
`define			reg_l1d_ix	reg[5:0]
(* ram_style = "distributed" *)
	reg[ 71:0]		arrMemAddrA[63:0];
(* ram_style = "distributed" *)
	reg[ 71:0]		arrMemAddrB[63:0];
(* ram_style = "distributed" *)
	reg[143:0]		arrMemDataA[63:0];
(* ram_style = "distributed" *)
	reg[143:0]		arrMemDataB[63:0];
`endif


reg[ 71:0]		tArrMemAddrStA;
reg[ 71:0]		tArrMemAddrStB;
reg[127:0]		tArrMemDataStA;
reg[127:0]		tArrMemDataStB;
reg[15:0]		tArrMemDextStA;
reg[15:0]		tArrMemDextStB;
reg[7:0]		tArrMemChkStA;
reg[7:0]		tArrMemChkStB;
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
reg[15:0]		tArrMemDidStDextA;
reg[15:0]		tArrMemDidStDextB;

`reg_l1d_ix		tReqSeqIdxArr[15:0];
reg[ 43:0]		tReqSeqVaArr[15:0];

`reg_l1d_ix		tReqSeqIdx;
reg[ 43:0]		tReqSeqVa;

reg[  3:0]		tFlushRov;
reg[  3:0]		tNxtFlushRov;

reg[  3:0]		tFlushRovTlb;
reg[  3:0]		tNxtFlushRovTlb;

reg[47:0]		tNxtReqAddrHi;
reg[47:0]		tReqAddrHi;

reg[47:0]		tNxtReqAddr;
`reg_l1d_ix		tNxtReqIxA;
`reg_l1d_ix		tNxtReqIxB;
reg[ 43:0]		tNxtReqAxA;
reg[ 43:0]		tNxtReqAxB;
reg[47:0]		tReqAddr;
`reg_l1d_ix		tReqIxA;
`reg_l1d_ix		tReqIxB;
reg[ 43:0]		tReqAxA;
reg[ 43:0]		tReqAxB;
`reg_l1d_ix		tReq1IxA;
`reg_l1d_ix		tReq1IxB;
`reg_l1d_ix		tArrMemDidStIxA;
`reg_l1d_ix		tArrMemDidStIxB;

// reg[ 47:0]		tReqAddrUtlb;
// reg				tReqAddrUtlbHit;

reg[15:0]		tReqAxH;
reg[15:0]		tNxtReqAxH;
reg[15:0]		tReq2AxH;


reg[  4:0]		tNxtReqBix;
reg[  4:0]		tReqBix;
reg[  5:0]		tNxtReqOpm;
reg[  5:0]		tReqOpm;

reg[3:0]		tInPmode;
reg[3:0]		tNxtInPmode;

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
reg				tReqDoMissA;
reg				tReqDoMissB;
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

reg				tReqReadOnlyA;
reg				tReqReadOnlyB;
reg				tReq2ReadOnlyA;
reg				tReq2ReadOnlyB;

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

reg				tReqIsNz;
reg				tReqIsMmio;
reg				tReq2IsMmio;
reg				tReqIsCcmd;
reg				tReq2IsCcmd;

reg				tReqDoPfxA;
reg				tReqDoPfxB;
reg				tReqDoSpxA;
reg				tReqDoSpxB;


reg[ 71:0]		tBlkMemAddrA;
reg[ 71:0]		tBlkMemAddrB;
reg[127:0]		tBlkMemDataA;
reg[127:0]		tBlkMemDataB;
reg[15:0]		tBlkMemDextA;
reg[15:0]		tBlkMemDextB;
`reg_l1d_ix		tBlkMemIdxA;
`reg_l1d_ix		tBlkMemIdxB;
reg				tBlkIsDirtyA;
reg				tBlkIsDirtyB;

reg[ 71:0]		tBlkMemAddr2A;
reg[ 71:0]		tBlkMemAddr2B;
reg[127:0]		tBlkMemData2A;
reg[127:0]		tBlkMemData2B;
reg[15:0]		tBlkMemDext2A;
reg[15:0]		tBlkMemDext2B;
reg[7:0]		tBlkMemChk2A;
reg[7:0]		tBlkMemChk2B;
reg[7:0]		tBlkMemRChk2A;
reg[7:0]		tBlkMemRChk2B;

reg[ 71:0]		tBlk2StoreAddrA;
reg[ 71:0]		tBlk2StoreAddrB;
reg[127:0]		tBlk2StoreDataA;
reg[127:0]		tBlk2StoreDataB;
reg[15:0]		tBlk2StoreDextA;
reg[15:0]		tBlk2StoreDextB;
// reg[ 7:0]		tBlk2StoreChkA;
// reg[ 7:0]		tBlk2StoreChkB;

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

reg[127:0]		tBlk2MemDataA;
reg[127:0]		tBlk2MemDataB;
reg[ 71:0]		tBlk2MemAddrA;
reg[ 71:0]		tBlk2MemAddrB;
reg[ 71:0]		tBlk2MemDextA;
reg[ 71:0]		tBlk2MemDextB;

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
reg				tMemArrInterlockA;
reg				tMemArrInterlockB;

reg				tReg2StoreFwA;
reg				tReg2StoreFwB;
reg				tMemArrFwA;
reg				tMemArrFwB;

reg[63:0]		tMemMmioData;
reg[63:0]		tNxtMemMmioData;
reg				tMemMmioReady;
reg				tNxtMemMmioReady;


reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l1addr		tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

reg[ 15:0]		tMemSeqReqL;
reg[ 15:0]		tMemOpmReqL;
`reg_l1addr		tMemAddrReqL;
`reg_tile		tMemDataReqL;

reg				tMemReqStA;		//Store A
reg				tMemReqStB;		//Store B
reg				tNxtMemReqStA;	//Store A
reg				tNxtMemReqStB;	//Store B
reg				tNxtMemReqStAL;	//Store A
reg				tNxtMemReqStBL;	//Store B

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
reg				tNxtMemReqLdAL;	//Load A
reg				tNxtMemReqLdBL;	//Load B
reg				tNxtMemReqLdML;	//Load MMIO

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

`reg_l1d_ix		tMemSeqIx;
reg[43:0]		tMemSeqVa;
`reg_l1d_ix		tMemSeqIxL;
reg[43:0]		tMemSeqVaL;

reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg				tNxtDoFlush;
reg				tNxtDoFlushTlb;
reg				tDoFlush;
reg				tDoFlushTlb;
reg				tDoFlushL;
reg				tDoFlushTlbL;

reg				tTlbMissInh;
reg				tNxtTlbMissInh;
reg				tNxtTlbMissInh2;

reg[2:0]		tVolatileInhSet;	//Volatile Inhibit Cycles (Set)
reg[2:0]		tVolatileInhCnt;	//Volatile Inhibit Cycles (Count)
reg[2:0]		tNxtVolatileInh;
`reg_l1d_ix		tVolatileIxA;		//Volatile Evict Index
`reg_l1d_ix		tVolatileIxB;		//Volatile Evict Index
`reg_l1d_ix		tNxtVolatileIxA;
`reg_l1d_ix		tNxtVolatileIxB;


reg[79:0]		tUtlbArr[15:0];
reg[79:0]		tUtlbStAddr;
reg[3:0]		tUtlbStIx;
reg				tUtlbDoSt;

reg[79:0]		tUtlbBlkAddr;
reg[3:0]		tUtlbBlkIx;
reg[3:0]		tUtlb1BlkIx;
reg[3:0]		tNxtUtlbBlkIx;
reg				tUtlbBlkFlush;

reg[43:0]		tReqUtlbAxA;
reg[43:0]		tReqUtlbAxB;
reg[3:0]		tReqUtlbAccA;
reg[3:0]		tReqUtlbAccB;
reg				tReqUtlbHitHi;
reg				tReqUtlbHitAxA;
reg				tReqUtlbHitAxB;


always @*
begin
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

	tNxtReqAddrHi	= 0;
`ifdef jx2_enable_vaddr96
	tNxtReqAddrHi	= regInAddr[95:48];
`endif

	tNxtReqAddr		= regInAddr[47:0];
	tNxtReqBix		= regInAddr[4:0];
	tNxtReqOpm		= regInOpm;

//	tNxtInPmode		= regInSr[31:28];
	tNxtInPmode		= regInSr[31:28] ^ regKrrHash[3:0] ^ regKrrHash[7:4];

	tNxtUtlbBlkIx	= regInAddr[15:12] ^ regInAddr[19:16];

//	tNxtReqAxH		=
//		tNxtReqAddrHi[15: 0] ^
//		tNxtReqAddrHi[31:16] ^
//		tNxtReqAddrHi[47:32] ;

	tNxtReqAxH		=
		tNxtReqAddrHi[15: 0] ^
		{	tNxtReqAddrHi[23:16], tNxtReqAddrHi[31:24] } ^
		{	tNxtReqAddrHi[35:32], tNxtReqAddrHi[39:36],
			tNxtReqAddrHi[43:40], tNxtReqAddrHi[47:44] } ;


`ifdef def_true

`ifdef jx2_mem_l1dsz_64
	tNxtReqIxA[5:0] = tNxtReqAxA[6:1];
	tNxtReqIxB[5:0] = tNxtReqAxB[6:1];
`endif

`ifdef jx2_mem_l1dsz_128
	tNxtReqIxA[6:0] = tNxtReqAxA[7:1];
	tNxtReqIxB[6:0] = tNxtReqAxB[7:1];
`endif

`ifdef jx2_l1d_nohash

`ifdef jx2_mem_l1dsz_256
	tNxtReqIxA[7:0] = tNxtReqAxA[8:1];
	tNxtReqIxB[7:0] = tNxtReqAxB[8:1];
`endif

`ifdef jx2_mem_l1dsz_512
	tNxtReqIxA[8:0] = tNxtReqAxA[9:1];
	tNxtReqIxB[8:0] = tNxtReqAxB[9:1];
`endif

`ifdef jx2_mem_l1dsz_1024
	tNxtReqIxA[9:0] = tNxtReqAxA[10:1];
	tNxtReqIxB[9:0] = tNxtReqAxB[10:1];
`endif


`else

`ifdef jx2_mem_l1dsz_256
//	tNxtReqIxA[7:0] = tNxtReqAxA[7:0] ^ tNxtReqAxA[15:8];
//	tNxtReqIxB[7:0] = tNxtReqAxB[7:0] ^ tNxtReqAxB[15:8];
	tNxtReqIxA[7:0] = tNxtReqAxA[8:1] ^ tNxtReqAxA[16:9];
	tNxtReqIxB[7:0] = tNxtReqAxB[8:1] ^ tNxtReqAxB[16:9];
`endif

`ifdef jx2_mem_l1dsz_512
//	tNxtReqIxA[8:0] = tNxtReqAxA[8:0] ^ tNxtReqAxA[16:8];
//	tNxtReqIxB[8:0] = tNxtReqAxB[8:0] ^ tNxtReqAxB[16:8];
	tNxtReqIxA[8:0] = tNxtReqAxA[9:1] ^ tNxtReqAxA[18:10];
	tNxtReqIxB[8:0] = tNxtReqAxB[9:1] ^ tNxtReqAxB[18:10];
//	tNxtReqIxA[8:0] = tNxtReqAxA[9:1] ^ { tNxtReqAxA[15:10], 3'b0 };
//	tNxtReqIxB[8:0] = tNxtReqAxB[9:1] ^ { tNxtReqAxB[15:10], 3'b0 };
`endif

`ifdef jx2_mem_l1dsz_1024
//	tNxtReqIxA[9:0] = tNxtReqAxA[9:0] ^ tNxtReqAxA[17:8];
//	tNxtReqIxB[9:0] = tNxtReqAxB[9:0] ^ tNxtReqAxB[17:8];
	tNxtReqIxA[9:0] = tNxtReqAxA[10:1] ^ tNxtReqAxA[19:10];
	tNxtReqIxB[9:0] = tNxtReqAxB[10:1] ^ tNxtReqAxB[19:10];
`endif

`endif

	if((tVolatileIxA != 0) && (regInOpm[5:4] == 2'b00))
		tNxtReqIxA = tVolatileIxA;
	if((tVolatileIxB != 0) && (regInOpm[5:4] == 2'b00))
		tNxtReqIxB = tVolatileIxB;

	tNxtReqInValA	= regInValA;
	tNxtReqInValB	= regInValB;

	if(dcInHold)
	begin
		tReq1IxA		= tReqIxA;
		tReq1IxB		= tReqIxB;
		tUtlb1BlkIx		= tUtlbBlkIx;
	end
	else
	begin
		tReq1IxA		= tNxtReqIxA;
		tReq1IxB		= tNxtReqIxB;
		tUtlb1BlkIx		= tNxtUtlbBlkIx;
	end
`endif
end

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tNxtFlushRov		= tFlushRov;
	tNxtFlushRovTlb		= tFlushRovTlb;
	tNxtDoFlush			= 0;
	tNxtDoFlushTlb		= 0;

//	if((tInOpm == JX2_DCOPM_LDTLB) || tRegInSr[29])
	if((tInOpm == JX2_DCOPM_LDTLB) || (tRegInSr[29] && tRegInSr[30]))
	begin
		if(tTlbMissInh)
			$display("L1D$ Clear TLB Inhibit");
		tNxtTlbMissInh = 0;
	end

//	if(((tInOpm==JX2_DCOPM_FLUSHDS) && (tInOpmC!=JX2_DCOPM_FLUSHDS)) || reset)
	if((tInOpm==JX2_DCOPM_FLUSHDS) && (tInOpmC!=JX2_DCOPM_FLUSHDS) && !reset)
	begin
		tNxtDoFlush = 1;
		tNxtDoFlushTlb = 1;
	end

	if((tInOpm==JX2_DCOPM_INVTLB) && (tInOpmC!=JX2_DCOPM_INVTLB) && !reset)
	begin
		tNxtDoFlushTlb = 1;
	end
	
	if((tFlushRov == 0) && !tDoFlush && !reset)
		tNxtDoFlush = 1;
	if((tFlushRovTlb == 0) && !tDoFlushTlb && !reset)
		tNxtDoFlushTlb = 1;

	if(tDoFlush && !tDoFlushL)
	begin
		$display("L1 D$ DoFlush rov=%X", tFlushRov);
		tNxtFlushRov = tFlushRov + 1;
	end

	if(tDoFlushTlb && !tDoFlushTlbL)
	begin
		$display("L1 D$ DoFlushTlb rov=%X", tFlushRovTlb);
		tNxtFlushRovTlb = tFlushRovTlb + 1;
	end

	if(reset)
	begin
		tNxtFlushRov		= 0;
		tNxtFlushRovTlb		= 0;
	end
end

always @*
begin
	tReqSeqIdx		= tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa		= tReqSeqVaArr[memSeqIn[3:0]];
	tNxtTlbMissInh2	= 0;

	tNxtVolatileInh		= tVolatileInhCnt;
	if(tVolatileInhCnt!=0)
		tNxtVolatileInh	= tVolatileInhCnt - 1;
	tNxtVolatileIxA		= tVolatileIxA;
	tNxtVolatileIxB		= tVolatileIxB;

	tArrMemDoStA		= 0;
	tArrMemDoStB		= 0;
	tRegOutHold			= 0;
	tRegOutWait			= 0;
	
	tNxtMemRespStA		= tMemRespStA;
	tNxtMemRespStB		= tMemRespStB;
	tNxtMemRespLdA		= tMemRespLdA;
	tNxtMemRespLdB		= tMemRespLdB;
	tMemRingSkipResp	= 0;

	tRegOutExc			= 0;


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
	tArrMemChkStA	= 0;
	tArrMemDextStA	= 0;
	
	tArrMemAddrStB	= 0;
	tArrMemDataStB	= 0;
	tArrMemIdxStB	= 0;
	tArrMemDoStB	= 0;
	tArrMemChkStB	= 0;
	tArrMemDextStB	= 0;
	
	if(tReq2Opm[5] && !tReq2IsMmio && !tReq2IsCcmd && !tReq2StoreSticky)
	begin
		tReg2MissInterlockA		= (tReqIxA == tReq2IxA) && !tReq2MissSkipA;
		tReg2MissInterlockB		= (tReqIxB == tReq2IxB) && !tReq2MissSkipB;
	end


	tReqNoCross		= 0;
	tReqMissSkipA	= 0;
	tReqMissSkipB	= 0;
	tReqMissNoSkip	= 0;
	tReqDoPfxA		= 0;
	tReqDoPfxB		= 0;
	tReqDoSpxA		= 0;
	tReqDoSpxB		= 0;

	tNxtMemMmioData = tMemMmioData;
	tNxtMemMmioReady = tMemMmioReady;
	

	tReqIsNz		= tReqOpm[5:4] != 2'b00;
	tReqIsMmio		= tReqIsNz &&
		((((tReqAddr[47:32] == 16'h0000) && !tSrJQ) ||
			(tReqAddr[47:32] == 16'hFFFF)) &&
			(tReqAddr[31:28] == 4'hF)) ||
//		(tReqAddr[47:32] == 16'hF000);
		((tReqAddr[47:44] == 4'hF) && tSrJQ);
	tReqIsCcmd		= (tReqOpm[5:4] == 2'b00) && (tReqOpm[3:0] != 4'b0000);

	tReqNoCross		= 0;
	casez(tReqOpm[2:0])
//		3'bz00:		tReqNoCross = 1;
//		3'bz01:		tReqNoCross = !tReqBix[0];
//		3'bz10:		tReqNoCross = (tReqBix[1:0]==2'h0);
//		3'b011:		tReqNoCross = (tReqBix[2:0]==3'h0);

		3'bz00:		tReqNoCross = 1;
//		3'bz00:		tReqNoCross = 0;
//		3'bz00:		tReqNoCross =
//			(tReqBix[3:1]!=3'b000) && (tReqBix[3:1]!=3'b111);
		3'bz01:		tReqNoCross = !tReqBix[0] || (tReqBix[3:1]!=3'b111);
		3'bz10:		tReqNoCross = (tReqBix[1:0]==2'h0) || (tReqBix[3:2]!=2'b11);
		3'b011:		tReqNoCross = (tReqBix[2:0]==3'h0) || !tReqBix[3];

		3'b111:		tReqNoCross = (tReqBix[3:0]==4'h0);
//		default:	tReqNoCross		= 0;
	endcase

`ifdef def_true
	if(tReqOpm[5] && !tReqOpm[1])
		tReqNoCross = 0;
`endif

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

`ifdef def_true	
// `ifndef def_true	
	if(tReqOpm[5:4]==2'b10)
	begin
		if((tReqOpm[2:0] == 3'b111) && tReqNoCross)
		begin
			if(tReqBix[4])
				tReqDoSpxB		= 1;
//				tReqDoPfxB		= 1;
			else
				tReqDoSpxA		= 1;
//				tReqDoPfxA		= 1;

	//		tReqDoPfxA		= 0;
	//		tReqDoPfxB		= 0;
		end
	end
`endif

	tReqReadyA	= (tBlkMemIdxA == tReqIxA);
	tReqReadyB	= (tBlkMemIdxB == tReqIxB);
	tReqReady	= tReqReadyA && tReqReadyB;

`ifndef def_true
	if(tReqMissSkipA)
		tReg2MissInterlockA = 0;
	if(tReqMissSkipB)
		tReg2MissInterlockB = 0;
`endif

	tBlkMemData2A = tBlkMemDataA;
	tBlkMemData2B = tBlkMemDataB;
	tBlkMemAddr2A = tBlkMemAddrA;
	tBlkMemAddr2B = tBlkMemAddrB;
	tBlkMemDext2A = tBlkMemDextA;
	tBlkMemDext2B = tBlkMemDextB;

	tBlk2StoreAddrA = {
		tBlk2MemAddrA[71:5], 1'b1,
		tBlk2MemAddrA[3:0]};
	tBlk2StoreAddrB = {
		tBlk2MemAddrB[71:5], 1'b1,
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
//	tBlk2StoreChkA		= tBlk2StoreAddrA[71:64] ^ tBlk2StoreAddrA[12:5];
//	tBlk2StoreChkB		= tBlk2StoreAddrB[71:64] ^ tBlk2StoreAddrB[12:5];
	tBlk2StoreChkA		= {
		~(tBlk2StoreAddrA[71:68] ^ tBlk2StoreAddrA[8:5]),
		 (tBlk2StoreAddrA[71:68] ^ tBlk2StoreAddrA[8:5]) };
	tBlk2StoreChkB		= {
		~(tBlk2StoreAddrB[71:68] ^ tBlk2StoreAddrB[8:5]),
		 (tBlk2StoreAddrB[71:68] ^ tBlk2StoreAddrB[8:5]) };

//	tBlk2StoreDextA		= { 8'h00, tBlk2StoreChkA };
//	tBlk2StoreDextB		= { 8'h00, tBlk2StoreChkB };

//	tBlk2StoreDextA		= { 4'h0, tInPmode, tBlk2StoreChkA };
//	tBlk2StoreDextB		= { 4'h0, tInPmode, tBlk2StoreChkB };
`endif

	tBlk2StoreDextA		= tReq2AxH;
	tBlk2StoreDextB		= tReq2AxH;

`ifdef jx2_mem_l1d_fwstore
// `ifdef def_true
// `ifndef def_true

	if(tReg2MissInterlockA)
	begin
		tReg2StoreFwA = 1;
		tBlkMemAddr2A = tBlk2StoreAddrA;
		tBlkMemData2A = tBlk2StoreDataA;
		tBlkMemDext2A = tBlk2StoreDextA;

//		if(tReq2Bix[4])
//		begin
//			tBlkMemData2A = tBlk2InsData[255:128];
//		end else begin
//			tBlkMemData2A = tBlk2InsData[127:  0];
//		end
//		tBlkMemAddr2A = {
//			tBlk2MemAddrA[71:5], 1'b1,
//			tBlk2MemAddrA[3:0]};
	end

	if(tReg2MissInterlockB)
	begin
		tReg2StoreFwB = 1;
		tBlkMemAddr2B = tBlk2StoreAddrB;
		tBlkMemData2B = tBlk2StoreDataB;
		tBlkMemDext2B = tBlk2StoreDextB;

//		if(!tReq2Bix[4])
//		begin
//			tBlkMemData2B = tBlk2InsData[255:128];
//		end else begin
//			tBlkMemData2B = tBlk2InsData[127:  0];
//		end
//		tBlkMemAddr2B = {
//			tBlk2MemAddrB[71:5], 1'b1,
//			tBlk2MemAddrB[3:0]};
	end
`else
	if(tReg2MissInterlockA)
	begin
//		$display("L1 D$: Line A Interlock");
		tReqReady	= 0;
	end
	if(tReg2MissInterlockB)
	begin
//		$display("L1 D$: Line B Interlock");
		tReqReady	= 0;
	end
`endif

	tMemArrInterlockA	= tArrMemDidStA && (tReqIxA == tArrMemDidStIxA);
	tMemArrInterlockB	= tArrMemDidStB && (tReqIxB == tArrMemDidStIxB);

`ifdef jx2_mem_l1d_fwarray
// `ifdef def_true
// `ifndef def_true
//	if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA) && !tReg2StoreFwA)
	if(tMemArrInterlockA && !tReg2StoreFwA)
	begin
		tMemArrFwA		= 1;
		tBlkMemData2A	= tArrMemDidStDataA;
		tBlkMemAddr2A	= tArrMemDidStAddrA;
		tBlkMemDext2A	= tArrMemDidStDextA;
	end
//	if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB) && !tReg2StoreFwB)
	if(tMemArrInterlockB && !tReg2StoreFwB)
	begin
		tMemArrFwB		= 1;
		tBlkMemData2B	= tArrMemDidStDataB;
		tBlkMemAddr2B	= tArrMemDidStAddrB;
		tBlkMemDext2B	= tArrMemDidStDextB;
	end
`else
//	if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
	if(tMemArrInterlockA)
	begin
//		$display("L1 D$: Line A Store Clash");
		tReqReady	= 0;
	end
//	if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
	if(tMemArrInterlockB)
	begin
//		$display("L1 D$: Line B Store Clash");
		tReqReady	= 0;
	end
`endif

`ifndef def_true
	if(tMemArrInterlockA && tReg2MissInterlockA)
	begin
//		$display("L1 D$: Line A Double Interlock");
		tReqReady	= 0;
	end

	if(tMemArrInterlockB && tReg2MissInterlockB)
	begin
//		$display("L1 D$: Line B Double Interlock");
		tReqReady	= 0;
	end
`endif

	if(tReqAxA[0])
		$display("L1D$: tReqAxA Even/Odd Error");
	if(!tReqAxB[0])
		$display("L1D$: tReqAxB Even/Odd Error");

	tReqReady2	= tReqReady && tReqReadyL;


//	tBlkMemChk2A	= tBlkMemDext2A[7:0];
//	tBlkMemChk2B	= tBlkMemDext2B[7:0];

	tBlkIsDirtyA	= tBlkMemAddr2A[4];
	tBlkIsDirtyB	= tBlkMemAddr2B[4];

	tReqMissAddrA	=
		((tBlkMemAddr2A[47:32] != tReqAxA[43:28]) && tSrJQ) ||
		 (tBlkMemAddr2A[31: 5] != tReqAxA[27: 1]);
	tReqMissAddrB	=
		((tBlkMemAddr2B[47:32] != tReqAxB[43:28]) && tSrJQ) ||
		 (tBlkMemAddr2B[31: 5] != tReqAxB[27: 1]);

//	tReqMissAddrA	= tBlkMemAddr2A[47:5] != tReqAxA[43:1];
//	tReqMissAddrB	= tBlkMemAddr2B[47:5] != tReqAxB[43:1];
//	tReqFlushAddrA	= (tBlkMemAddr2A[71:68] != tFlushRov) || (tFlushRov==0);
//	tReqFlushAddrB	= (tBlkMemAddr2B[71:68] != tFlushRov) || (tFlushRov==0);

	tReqFlushAddrA	= (tBlkMemAddr2A[71:68] != tFlushRov);
	tReqFlushAddrB	= (tBlkMemAddr2B[71:68] != tFlushRov);

	if(tBlkMemDext2A != tReqAxH)
		tReqMissAddrA	= 1;
	if(tBlkMemDext2B != tReqAxH)
		tReqMissAddrB	= 1;

`ifdef jx2_mem_l1d_utlb
	tUtlbBlkFlush	= (tUtlbBlkAddr[7:4] != tFlushRovTlb);
	tReqUtlbAxA		= { tUtlbBlkAddr[79:46], tReqAxA[9:0] };
	tReqUtlbAxB		= { tUtlbBlkAddr[79:46], tReqAxB[9:0] };
	tReqUtlbAccA	= tUtlbBlkAddr[3:0];
	tReqUtlbAccB	= tUtlbBlkAddr[3:0];

//	tReqUtlbHitHi	= tReqAxA[43:16] == tUtlbBlkAddr[43:16];
	tReqUtlbHitHi	=
		(tReqAxA[43:28] == tUtlbBlkAddr[43:28]) &&
		(tReqAxA[27:16] == tUtlbBlkAddr[27:16]) ;
	if(tReqAxA[16] != tReqAxB[16])
		tReqUtlbHitHi = 0;
	tReqUtlbHitAxA	= tReqUtlbHitHi &&
		(tReqAxA[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && regInMmcr[0];
	tReqUtlbHitAxB	= tReqUtlbHitHi &&
		(tReqAxB[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && regInMmcr[0];

	if(regInMmcr[5:4]==2'b00)
	begin
		/* 4K pages. */
		tReqUtlbAxA[9:8]=tUtlbBlkAddr[45:44];
		tReqUtlbAxB[9:8]=tUtlbBlkAddr[45:44];
		if(tReqAxA[9:8] != tUtlbBlkAddr[9:8])
			tReqUtlbHitAxA = 0;
		if(tReqAxB[9:8] != tUtlbBlkAddr[9:8])
			tReqUtlbHitAxB = 0;
	end

`ifndef def_true
	if(regInMmcr[0] && tReqIsNz && !tReqIsMmio && !tReqIsCcmd)
	begin
		if(tReqUtlbHitAxA)
			$display("Utlb Hit, %X %X", tUtlbBlkAddr, tReqAxA);
		else
			$display("Utlb Miss, %X %X", tUtlbBlkAddr, tReqAxA);
	end
`endif

`endif

	tReqNoReadA		= tBlkMemAddr2A[0];
	tReqNoReadB		= tBlkMemAddr2B[0];

	tReqReadOnlyA	= tBlkMemAddr2A[1];
	tReqReadOnlyB	= tBlkMemAddr2B[1];

//	if(!tTlbMissInh)
	if(!tTlbMissInh && !tReqIsMmio && !tReqIsCcmd)
	begin
//		if(tBlkMemAddr2A[3])
		if(tBlkMemAddr2A[3:2] == 2'b11)
		begin
			tReqFlushAddrA = 1;
		end
//		if(tBlkMemAddr2B[3])
		if(tBlkMemAddr2B[3:2] == 2'b11)
		begin
			tReqFlushAddrB = 1;
		end

		if(tVolatileInhCnt == 0)
		begin
			if(tBlkMemAddr2A[3:2] == 2'b10)
			begin
				tReqFlushAddrA	= 1;
				tReqIsNz		= 1;
			end
			if(tBlkMemAddr2B[3:2] == 2'b10)
			begin
				tReqFlushAddrB	= 1;
				tReqIsNz		= 1;
			end
		end

`ifndef def_true
// `ifdef def_true
		if((tBlkMemDext2A[11:8] != tInPmode) && !tReqMissSkipA)
		begin
			tReqFlushAddrA	= 1;
//			tReqIsNz		= 1;
		end
		if((tBlkMemDext2B[11:8] != tInPmode) && !tReqMissSkipB)
		begin
			tReqFlushAddrB	= 1;
//			tReqIsNz		= 1;
		end
`endif
	end

	tRegOutExc[63:16] = tReqAddr[47:0];

	if(!tBlkMemAddr2A[3] && !tBlkMemAddr2B[3])
	begin
		if(	(tReqNoReadA && !tReqMissSkipA) ||
			(tReqNoReadB && !tReqMissSkipB) )
		begin
			if(tReqOpm[4])
				tRegOutExc[15:0] = 16'h8001;
		end

		if(	(tReqReadOnlyA && !tReqMissSkipA) ||
			(tReqReadOnlyB && !tReqMissSkipB) )
		begin
			if(tReqOpm[5])
				tRegOutExc[15:0] = 16'h8002;
		end
	end

`ifndef def_true
//	if(tBlkMemChk2A != tBlkMemRChk2A)
	if(tBlkMemChk2A[3:0] != (~tBlkMemChk2A[7:4]))
	begin
		tBlkIsDirtyA	= 0;
		tReqFlushAddrA	= 1;
	end
//	if(tBlkMemChk2B != tBlkMemRChk2B)
	if(tBlkMemChk2B[3:0] != (~tBlkMemChk2B[7:4]))
	begin
		tBlkIsDirtyB	= 0;
		tReqFlushAddrB	= 1;
	end
`endif

	tReqMissA	= (tReqMissAddrA && !tReqMissSkipA) || tReqFlushAddrA;
	tReqMissB	= (tReqMissAddrB && !tReqMissSkipB) || tReqFlushAddrB;
	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz;

	if(tReqMiss)
		tRegOutExc[15] = 0;

	if(!tReqIsNz)
	begin
		tReqMissA	= 0;
		tReqMissB	= 0;
	end


	tReqDoMissA	= tReqMissA;
	tReqDoMissB	= tReqMissB;

`ifdef def_true
// `ifndef def_true

//	if(tReqMiss || (tReqMissNoSkipL && tRegOutHoldL))
//	if((tReqMiss && tReqReady) || (tReqMissNoSkipL && tRegOutHoldL))
	if((tReqMiss && tReqReady && !tReqMissL) ||
		(tReqMissNoSkipL && tRegOutHoldL))
//	if(1'b1)
	begin
//		tReqMissSkipA	= 0;
//		tReqMissSkipB	= 0;
		tReqMissNoSkip	= 1;

		tReqDoMissA	= (tReqMissAddrA || tReqFlushAddrA) && tReqIsNz;
		tReqDoMissB	= (tReqMissAddrB || tReqFlushAddrB) && tReqIsNz;
//		tReqMiss	= tReqMissA || tReqMissB;
		
// `ifdef def_true
`ifndef def_true
//		if(tReqMissSkipA && tBlkIsDirtyA)
//		if(tReqMissSkipA && tBlkIsDirtyA && tReg2MissInterlockA)
		if(tReqMissSkipA && tBlkIsDirtyA &&
				(tReg2MissInterlockA || tMemArrInterlockA))
			tReqDoMissA = 0;
//		if(tReqMissSkipB && tBlkIsDirtyB)
//		if(tReqMissSkipB && tBlkIsDirtyB && tReg2MissInterlockB)
		if(tReqMissSkipB && tBlkIsDirtyB &&
				(tReg2MissInterlockB || tMemArrInterlockB))
			tReqDoMissB = 0;
`endif

// `ifdef def_true
`ifndef def_true
		if(tReqMissL)
		begin
			if(tReqDoMissA && !tReqMissAL)
			begin
				$display("L1D$: Gained MissA");
//				tReqDoMissA	= tReqMissAL;
			end
			if(tReqDoMissB && !tReqMissBL)
			begin
				$display("L1D$: Gained MissB");
//				tReqDoMissB	= tReqMissBL;
			end
		end
`endif

//		tReqMiss	= tReqMissA || tReqMissB;
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(tReqMissNoSkip)
//	if(1'b1)
	begin
		if(tReg2MissInterlockA)
			tReqReady	= 0;
		if(tReg2MissInterlockB)
			tReqReady	= 0;

		if(tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
			tReqReady	= 0;
		if(tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
			tReqReady	= 0;
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(tReqMissA && tReg2MissInterlockA)
		tReqReady	= 0;
	if(tReqMissB && tReg2MissInterlockB)
		tReqReady	= 0;

	if(tReqMissA && tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
		tReqReady	= 0;
	if(tReqMissB && tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
		tReqReady	= 0;
`endif

// `ifdef def_true
`ifndef def_true
	if(tReg2MissInterlockA && tArrMemDidStA && (tReqIxA == tArrMemDidStIxA))
		tReqReady	= 0;
	if(tReg2MissInterlockB && tArrMemDidStB && (tReqIxB == tArrMemDidStIxB))
		tReqReady	= 0;
`endif

//	if(!tReqReady && tReqReadyL && tRegOutHoldL)
//		$display("L1D$: Loss of Ready");

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

`ifndef def_true
		if(	(tReg2MissInterlockA && !tReg2StoreFwA) ||
			(tReg2MissInterlockB && !tReg2StoreFwB)	)
		begin
//			$display("L1 D$: Interlock Stall");
			tRegOutHold = 1;
		end
`endif
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
	
	if(tReqNoReadA || tReqNoReadB)
	begin
		tBlkExDataA = UV64_00;
		tBlkExDataB = UV64_00;
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


	tUtlbStAddr		= 0;
	tUtlbStIx		= 0;
	tUtlbDoSt		= 0;


	tNxtReq2StoreSticky = 0;

	if(memRingIsRespOkStA)
	begin
//		if(tReqSeqIdx==tReqIxA)
		if(1'b1)
		begin
//			$display("L1D$: Store Response A, A=%X, Ix1/Ix2", memAddrIn, 
//				tReqSeqIdx, tReqIxA);
			tNxtMemRespStA = 1;
		end
		else
		begin
			$display("L1D$: Mismatch Store Response A, A=%X Ix1/Ix2=%X/%X", 
				memAddrIn, tReqSeqIdx, tReqIxA);
		end
	end
	
	if(memRingIsRespOkStB)
	begin
//		if(tReqSeqIdx==tReqIxB)
		if(1'b1)
		begin
//			$display("L1D$: Store Response B, A=%X", memAddrIn);
			tNxtMemRespStB = 1;
		end
		else
		begin
			$display("L1D$: Mismatch Store Response B, A=%X", memAddrIn);
		end
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

	if(memRingIsRespOkLdA && !tMemRingSkipResp && !reset)
	begin
//		$display("L1D$: Load Response A, A=%X", memAddrIn);

//		if(tReqSeqIdx==tReqIxA)
		if(1'b1)
		begin
			tArrMemAddrStA = {
				tFlushRov[3:0],
				memAddrIn[31:12],
				tReqSeqVa[43:1], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStA = memDataIn;
//			tArrMemIdxStA = tReqSeqIdx;
			tArrMemIdxStA = tReqIxA;
//			tArrMemChkStA	= tArrMemAddrStA[71:64] ^ tArrMemAddrStA[12:5];
			tArrMemChkStA		= {
				~(tArrMemAddrStA[71:68] ^ tArrMemAddrStA[8:5]),
				 (tArrMemAddrStA[71:68] ^ tArrMemAddrStA[8:5]) };
//			tArrMemDextStA	= { 8'h00, tArrMemChkStA };
//			tArrMemDextStA	= { 4'h0, tInPmode, tArrMemChkStA };
			tArrMemDextStA	= tReqAxH;
			tArrMemDoStA = 1;
			tNxtMemRespLdA = 1;

			tUtlbStAddr		= {
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
				memOpmIn[3:0] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);

			if(memOpmIn[3])
			begin
				if(memOpmIn[2])
				begin
					$display("L1D$ Set TLB Inhibit A");
					tNxtTlbMissInh2 = 1;
				end
				else
				begin
					tNxtVolatileInh		= 5;
					tNxtVolatileIxA		= tReqIxA;
				end
			end
		end
		
		if(memAddrIn[4])
			$display("L1D$: Load Even/Odd Mismatch A");
		
		if(tReqSeqIdx!=tReqIxA)
			$display("L1D$: In!=Req IxA, %X %X",
				tReqSeqIdx, tReqIxA);
		if(memAddrIn[31:5]!=tReqSeqVa[27:1])
			$display("L1D$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
		if(tReqSeqVa[43:1]!=tReqAxA[43:1])
			$display("L1D$: In!=Req A, %X %X",
				tReqAxA[43:1], tReqSeqVa[43:0]);
	end

	if(memRingIsRespOkLdB && !tMemRingSkipResp && !reset)
	begin
//		$display("L1D$: Load Response B, A=%X", memAddrIn);

//		if(tReqSeqIdx==tReqIxB)
		if(1'b1)
		begin
			tArrMemAddrStB = {
				tFlushRov[3:0],
				memAddrIn[31:12],
				tReqSeqVa[43:1], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStB = memDataIn;
//			tArrMemIdxStB = tReqSeqIdx;
			tArrMemIdxStB = tReqIxB;
//			tArrMemChkStB	= tArrMemAddrStB[71:64] ^ tArrMemAddrStB[12:5];
			tArrMemChkStB		= {
				~(tArrMemAddrStB[71:68] ^ tArrMemAddrStB[8:5]),
				 (tArrMemAddrStB[71:68] ^ tArrMemAddrStB[8:5]) };
//			tArrMemDextStB	= { 8'h00, tArrMemChkStB };
//			tArrMemDextStB	= { 4'h0, tInPmode, tArrMemChkStB };
			tArrMemDextStB	= tReqAxH;
			tArrMemDoStB = 1;
			tNxtMemRespLdB = 1;

			tUtlbStAddr		= {
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
				memOpmIn[3:0] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);

			if(memOpmIn[3])
			begin
				if(memOpmIn[2])
				begin
					$display("L1D$ Set TLB Inhibit B");
					tNxtTlbMissInh2 = 1;
				end
				else
				begin
					tNxtVolatileInh		= 5;
					tNxtVolatileIxB		= tReqIxB;
				end
			end
		end

		if(!memAddrIn[4])
			$display("L1D$: Load Even/Odd Mismatch B");

		if(tReqSeqIdx!=tReqIxB)
			$display("L1D$: In!=Req IxB, %X %X",
				tReqSeqIdx, tReqIxB);
		if(memAddrIn[31:5]!=tReqSeqVa[27:1])
			$display("L1D$: Virt!=Phys B, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
		if(tReqSeqVa[43:1]!=tReqAxB[43:1])
			$display("L1D$: In!=Req B, %X %X",
				tReqAxB[43:1], tReqSeqVa[43:0]);
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

//	if(memRingIsRespOkMmio)
//	if(memRingIsRespOkMmio && tReqIsMmio)
	if(memRingIsRespOkMmio && (tReqIsMmio || tReqIsCcmd))
	begin
//		$display("L1 D$: MMIO Response Seen");
		tNxtMemMmioData = memDataIn[63:0];
		tNxtMemMmioReady = 1;
	end

//	if(tReq2Opm[5] && !tReq2IsMmio)
	if(tReq2Opm[5] && !tReq2IsMmio && !tReq2IsCcmd)
	begin
		tNxtReq2StoreSticky = tReq2StoreSticky;

//		$display("Store Sticky=%d Data=%X",
//			tReq2StoreSticky, tBlk2InsData);

		if(!tReq2Ready)
			$display("L1 D$: Store Fault: Non-Ready State");


		/* Stall if store needs to be delayed. */
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
//			tArrMemAddrStA = {
//				tBlk2MemAddrA[71:5], 1'b1,
//				tBlk2MemAddrA[3:0]};
			tArrMemAddrStA	= tBlk2StoreAddrA;
			tArrMemDataStA	= tBlk2StoreDataA;
			tArrMemDextStA	= tBlk2StoreDextA;
			tArrMemIdxStA	= tReq2IxA;

//			tArrMemAddrStB = {
//				tBlk2MemAddrB[71:5], 1'b1,
//				tBlk2MemAddrB[3:0]};
			tArrMemAddrStB	= tBlk2StoreAddrB;
			tArrMemDataStB	= tBlk2StoreDataB;
			tArrMemDextStB	= tBlk2StoreDextB;
			tArrMemIdxStB	= tReq2IxB;

			tNxtReq2StoreSticky = 1;

`ifndef def_true
			if(tReq2Bix[4])
			begin
				tArrMemDataStA = tBlk2InsData[255:128];
				tArrMemDataStB = tBlk2InsData[127:  0];
			end else begin
				tArrMemDataStA = tBlk2InsData[127:  0];
				tArrMemDataStB = tBlk2InsData[255:128];
			end
`endif

//			tArrMemDoStA = !tReq2MissSkipA;
//			tArrMemDoStB = !tReq2MissSkipB;
			tArrMemDoStA = !tReq2MissSkipA && !tReq2ReadOnlyA;
			tArrMemDoStB = !tReq2MissSkipB && !tReq2ReadOnlyB;

			if(tBlk2StoreAddrA[3:2]==2'b11)
				tArrMemDoStA = 0;
			if(tBlk2StoreAddrB[3:2]==2'b11)
				tArrMemDoStB = 0;

			if((tBlk2MemAddrA[31:5]!=tReq2AxA[27:1]) && !tReq2MissSkipA)
			begin
				$display("L1 D$: Store Addr Mismatch A");
//				tRegOutHold		= 1;
			end
			if((tBlk2MemAddrB[31:5]!=tReq2AxB[27:1]) && !tReq2MissSkipB)
			begin
				$display("L1 D$: Store Addr Mismatch B");
//				tRegOutHold		= 1;
			end

`ifdef def_true
			if((tArrMemDataStA[127:0]!=tBlk2MemDataA[127:0]) && tReq2MissSkipA)
				$display("L1 D$: Store Data Mismatch A");
			if((tArrMemDataStB[127:0]!=tBlk2MemDataB[127:0]) && tReq2MissSkipB)
				$display("L1 D$: Store Data Mismatch B");
`endif

// `ifndef def_true
`ifdef def_true
			if(tReq2MissA && tArrMemDoStA)
			begin
				$display("L1 D$: Store Fault: MissA");
				tArrMemDoStA = 0;
//				tRegOutHold		= 1;
			end

			if(tReq2MissB && tArrMemDoStB)
			begin
				$display("L1 D$: Store Fault: MissB");
				tArrMemDoStB = 0;
//				tRegOutHold		= 1;
			end
`endif
			
//			$display("%X %X", tArrMemDoStA, tArrMemDoStB)
		end
	end

// `ifdef def_true
`ifndef def_true
	if(tReqMissSkipA && tArrMemDoStA && (tReqIxA == tArrMemIdxStA))
		tReqDoMissA = 0;
	if(tReqMissSkipB && tArrMemDoStB && (tReqIxB == tArrMemIdxStB))
		tReqDoMissB = 0;
`endif


	if(reset)
	begin
		tReqMiss		= 0;
		tReqMissA		= 0;
		tReqMissB		= 0;
		tReqWaitResp	= 0;
	end

	/* Miss Handling */

`ifdef def_true
	if(tMemReqStA && !tMemReqLdA && !tReqDoMissA)
	begin
		$display("L1D$: Store Without Load A");
//		tReqDoMissA = 1;
	end
	if(tMemReqStB && !tMemReqLdB && !tReqDoMissB)
	begin
		$display("L1D$: Store Without Load B");
//		tReqDoMissB = 1;
	end

	if(!tMemReqStA && tMemReqLdA && tBlkIsDirtyA)
		$display("L1D$: Gain DirtyA");
	if(!tMemReqStB && tMemReqLdB && tBlkIsDirtyB)
		$display("L1D$: Gain DirtyB");
`endif

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UVB1AT_00;
	tMemDataReq		= UVTILE_XX;
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
//		tRegOutHold		= 1;

		if(!tMemReqLdM)
		begin
//			$display("L1 D$ MMIO Req A=%X", tReqAddr);
		
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
			tMemDataReq		= { UV64_00, tReqInValA };
//			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDSQ };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddr };
`else
			tMemAddrReq		= tReqAddr;
`endif
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
		if(tReqDoMissA || tReqDoMissB || tReqWaitResp)
	begin
		tNxtMemReqStA	= tMemReqStA;
		tNxtMemReqStB	= tMemReqStB;
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;
//		tNxtMemSeqRov	= tMemSeqRov;

		tMemSeqIx		= tMemSeqIxL;
		tMemSeqVa		= tMemSeqVaL;

`ifndef def_true
		if(!tMemReqSent)
		begin
			tMemSeqReq		= tMemSeqReqL;
			tMemOpmReq		= tMemOpmReqL;
			tMemAddrReq		= tMemAddrReqL;
			tMemDataReq		= tMemDataReqL;
		end
`endif

		tRegOutWait		= 1;
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
//			$display("L1 D$: Send STA Req, A=%X", tBlkMemAddr2A[47:4]);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
			tMemDataReq		= tBlkMemData2A;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
			tMemAddrReq		= {
`ifdef jx2_enable_l1addr96
				UV48_00,
`endif
				JX2_RBI_ADDRHI_PHYS,
				tBlkMemAddr2A[67:48],
				tBlkMemAddr2A[11: 5],
				5'h00 };
			tNxtMemReqStA	= 1;
		end
		else if(tReqDoMissB && tBlkIsDirtyB && !tMemReqStB)
		begin
//			$display("L1 D$: Send STB Req, A=%X", tBlkMemAddr2B[47:4]);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov };
			tMemDataReq		= tBlkMemData2B;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
			tMemAddrReq		= {
`ifdef jx2_enable_l1addr96
				UV48_00,
`endif
				JX2_RBI_ADDRHI_PHYS,
				tBlkMemAddr2B[67:48],
				tBlkMemAddr2B[11: 5],
				5'h10 };
			tNxtMemReqStB	= 1;
		end
		else
			if(tReqDoMissA && !tMemReqLdA)
		begin
//			$display("L1 D$: Send LDA Req A=%X", tReqAxA);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2A;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAxA, 4'h00 };
`else
			tMemAddrReq		= { tReqAxA, 4'h00 };
`endif

			if(tReqDoPfxA)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
			if(tReqDoSpxA)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_SPX };

`ifdef jx2_mem_l1d_utlb
			if(tReqUtlbHitAxA)
			begin
`ifdef jx2_enable_l1addr96
				tMemAddrReq			= { UV48_00, tReqUtlbAxA, 4'h00 };
`else
				tMemAddrReq			= { tReqUtlbAxA, 4'h00 };
`endif
				tMemOpmReq[11:8]	= tReqUtlbAccA;
			end
`endif
			tNxtMemReqLdA = 1;
		end
		else
			if(tReqDoMissB && !tMemReqLdB)
		begin
//			$display("L1 D$: Send LDB Req A=%X", tReqAxB);
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2B;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAxB, 4'h00 };
`else
			tMemAddrReq		= { tReqAxB, 4'h00 };
`endif

			if(tReqDoPfxB)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
			if(tReqDoSpxB)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_SPX };

`ifdef jx2_mem_l1d_utlb
			if(tReqUtlbHitAxB)
			begin
`ifdef jx2_enable_l1addr96
				tMemAddrReq			= { UV48_00, tReqUtlbAxB, 4'h00 };
`else
				tMemAddrReq			= { tReqUtlbAxB, 4'h00 };
`endif
				tMemOpmReq[11:8]	= tReqUtlbAccB;
			end
`endif
			tNxtMemReqLdB	= 1;
		end
`endif
	end
	else
	begin
`ifndef def_true
		if(	tMemReqStA | tMemReqStB |
			tMemReqLdA | tMemReqLdB |
			tMemReqLdM )
		begin
//			$display("L1 D$: Wait Response");
			tRegOutHold = 1;
		end
`endif
	end

//	if(tReqIsMmio && tMemMmioReady)
//		tRegOutHold = 0;

	if(reset)
	begin
		tRegOutHold = 0;
	end

end

always @(posedge clock)
begin
	if(tRegOutHold && !dcInHold)
		$display("L1D$: Hold Signal Fault");
end

always @(posedge clock)
begin
//	tTlbMissInh		<= tNxtTlbMissInh;
	tTlbMissInh		<= tNxtTlbMissInh || tNxtTlbMissInh2;
//	tVolatileInh	<= tNxtVolatileInh;
	tVolatileIxA	<= tNxtVolatileIxA;
	tVolatileIxB	<= tNxtVolatileIxB;


	if(!dcInHold)
	begin
		tVolatileInhCnt	<= tNxtVolatileInh;
		tVolatileInhSet	<= 0;

		tInPmode		<= tNxtInPmode;

		tSrJQ			<= regInSr[31];
		tRegInSr		<= regInSr;
	
		/* EX1 -> EX2 */
		tReqAddr		<= tNxtReqAddr;
		tReqAddrHi		<= tNxtReqAddrHi;
		tReqIxA			<= tNxtReqIxA;
		tReqIxB			<= tNxtReqIxB;
		tReqAxA			<= tNxtReqAxA;
		tReqAxB			<= tNxtReqAxB;
		tReqAxH			<= tNxtReqAxH;
		tReqBix			<= tNxtReqBix;
		tReqOpm			<= tNxtReqOpm;
		tReqInValA		<= tNxtReqInValA;
		tReqInValB		<= tNxtReqInValB;

//		tUtlbBlkIx		<= tNxtReqAddr[11:8];
		tUtlbBlkIx		<= tNxtUtlbBlkIx;

		/* EX2 -> EX3 */
		tRegOutValA		<= tBlkExDataA;
		tRegOutValB		<= tBlkExDataB;

		tBlk2MemAddrA	<= tBlkMemAddr2A;
		tBlk2MemAddrB	<= tBlkMemAddr2B;
		tBlk2MemDataA	<= tBlkMemData2A;
		tBlk2MemDataB	<= tBlkMemData2B;

		tBlk2InsData	<= tBlkInsData0;
		tReq2IxA		<= tReqIxA;
		tReq2IxB		<= tReqIxB;
		tReq2AxA		<= tReqAxA;
		tReq2AxB		<= tReqAxB;
		tReq2AxH		<= tReqAxH;
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
		tReq2ReadOnlyA		<= tReqReadOnlyA;
		tReq2ReadOnlyB		<= tReqReadOnlyB;

		tReq2StoreSticky	<= 0;
		tMemMmioData		<= 0;
		tMemMmioReady		<= 0;

	end
	else
	begin
		tReq2StoreSticky	<= tNxtReq2StoreSticky;
		tMemMmioData		<= tNxtMemMmioData;
		tMemMmioReady		<= tNxtMemMmioReady;

		tVolatileInhSet	<= tNxtVolatileInh;
		tVolatileInhCnt	<= tVolatileInhCnt;
	end

	tFlushRov		<= tNxtFlushRov;
	tFlushRovTlb	<= tNxtFlushRovTlb;

	tReqReadyL		<= tReqReady;

	tInOpm			<= tNxtReqOpm;
	tInOpmB			<= tInOpm;
	tInOpmC			<= tInOpmB;
	tDoFlush		<= tNxtDoFlush;
	tDoFlushTlb		<= tNxtDoFlushTlb;
	tDoFlushL		<= tDoFlush;
	tDoFlushTlbL	<= tDoFlushTlb;
	
	tRegOutHoldL	<= tRegOutHold;
	tReqMissNoSkipL	<= tReqMissNoSkip;


	/* EX1->EX2 */
	tBlkMemAddrA	<= arrMemAddrA[tReq1IxA];
	tBlkMemAddrB	<= arrMemAddrB[tReq1IxB];
	{ tBlkMemDextA, tBlkMemDataA }	<= arrMemDataA[tReq1IxA];
	{ tBlkMemDextB, tBlkMemDataB }	<= arrMemDataB[tReq1IxB];
	tBlkMemIdxA		<= tReq1IxA;
	tBlkMemIdxB		<= tReq1IxB;


	if(tArrMemDoStA)
	begin
//		$display("L1 D$ STA Ix=%X A=%X D=%X",
//			tArrMemIdxStA, tArrMemAddrStA, tArrMemDataStA);
		arrMemAddrA[tArrMemIdxStA]	<= tArrMemAddrStA;
		arrMemDataA[tArrMemIdxStA]	<= { tArrMemDextStA, tArrMemDataStA };
	end

	if(tArrMemDoStB)
	begin
//		$display("L1 D$ STB Ix=%X A=%X D=%X",
//			tArrMemIdxStB, tArrMemAddrStB, tArrMemDataStB);
		arrMemAddrB[tArrMemIdxStB]	<= tArrMemAddrStB;
		arrMemDataB[tArrMemIdxStB]	<= { tArrMemDextStB, tArrMemDataStB };
	end

`ifdef jx2_mem_l1d_utlb
	if(tUtlbDoSt)
	begin
		tUtlbArr[tUtlbStIx]		 <= tUtlbStAddr;
	end
	tUtlbBlkAddr		<= tUtlbArr[tUtlb1BlkIx];
`endif


	tArrMemDidStAddrA	<= tArrMemAddrStA;
	tArrMemDidStAddrB	<= tArrMemAddrStB;
	tArrMemDidStDataA	<= tArrMemDataStA;
	tArrMemDidStDataB	<= tArrMemDataStB;
	tArrMemDidStDextA	<= tArrMemDextStA;
	tArrMemDidStDextB	<= tArrMemDextStB;

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
//	tReqMissNoSkipL	<= dcInHold ? tReqMissNoSkip : 0;

	tMemSeqReqL		<= tMemSeqReq;
	tMemOpmReqL		<= tMemOpmReq;
	tMemAddrReqL	<= tMemAddrReq;
	tMemDataReqL	<= tMemDataReq;
//	tNxtMemReqStAL	<= dcInHold ? tNxtMemReqStA : 0;
//	tNxtMemReqStBL	<= dcInHold ? tNxtMemReqStB : 0;
//	tNxtMemReqLdAL	<= dcInHold ? tNxtMemReqLdA : 0;
//	tNxtMemReqLdBL	<= dcInHold ? tNxtMemReqLdB : 0;
//	tNxtMemReqLdML	<= dcInHold ? tNxtMemReqLdM : 0;

	tMemSeqIxL		<= tMemSeqIx;
	tMemSeqVaL		<= tMemSeqVa;

	tReqMissAL		<= tReqMissA;
	tReqMissBL		<= tReqMissB;
	tReqMissL		<= tReqMiss;

	tRegOutExc2		<= tRegOutExc;

	if(reset)
	begin
		tMemSeqOut  <= 0;
		tMemOpmOut  <= 0;
		tMemAddrOut <= 0;
		tMemDataOut <= 0;

//		tMemSeqOut  <= memSeqIn;
//		tMemOpmOut  <= memOpmIn;
//		tMemAddrOut <= memAddrIn;
//		tMemDataOut <= memDataIn;
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

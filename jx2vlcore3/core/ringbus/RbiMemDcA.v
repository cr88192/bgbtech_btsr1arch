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

`include "ringbus/RbiDefs.v"

module RbiMemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regInAddrB,		regInOpmB,
	regOutValA,		regInValA,
	regOutValB,		regInValB,
	dcInHold,		regOutHold,
	regInSr,		regOutWait,
	regOutExc,		regInMmcr,
	regKrrHash,		regRng,
	regInLdOp,		regOutValFast,

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

input[47: 0]	regInAddrB;		//input address
input[ 5: 0]	regInOpmB;		//operation mode

input[ 7: 0]	regInLdOp;

output[63: 0]	regOutValA;		//output data value (Low 128 / Lane A)
output[63: 0]	regOutValB;		//output data value (High 128 / Lane B)
output[65: 0]	regOutValFast;	//fast-path output data value

input [63: 0]	regInValA;		//input data value (Low 128 / Lane A)
input [63: 0]	regInValB;		//input data value (High 128 / Lane B)

input			dcInHold;
output			regOutHold;
output			regOutWait;

input [63: 0]	regInSr;
input [63: 0]	regInMmcr;
output[127: 0]	regOutExc;

input[ 7:0]		regKrrHash;
input[15:0]		regRng;



input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l1addr	memAddrIn;		//memory input address
`output_l1addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

parameter		noLdOp		= 0;
parameter		disableTlb	= 0;


(* max_fanout = 200 *)
	wire			dcInHoldN;

assign	dcInHoldN = !dcInHold;



reg[63: 0]	tRegOutValA;
reg[63: 0]	tRegOutValB;
assign	regOutValA = tRegOutValA;
assign	regOutValB = tRegOutValB;

reg[65: 0]	tRegOutValFast;
assign	regOutValFast = tRegOutValFast;

reg[127: 0]	tRegOutExc;
reg[127: 0]	tRegOutExc2;
reg[127: 0]	tRegOutExc3;
assign	regOutExc = tRegOutExc2;
// assign	regOutExc = tRegOutExc3;

reg[63: 0]		tRegInSr;
reg[63: 0]		tRegInSrL;

reg[63: 0]		tRegRng1;
reg[63: 0]		tRegInMmcr;


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
  (107:92): Dext
  ( 91:72): Aext
  ( 71:68): Flush Rover
  ( 67:48): PA Bits
  ( 47: 5): VA Bits
  (  4: 0): Access Flags
 */

// `define jx2_mem_l1darr_addr		reg[ 71:0]
// `define jx2_mem_l1darr_data		reg[143:0]

`define jx2_mem_l1darr_addr		reg[ 71:0]
`define jx2_mem_l1darr_addrb	reg[107:0]
`define jx2_mem_l1darr_data		reg[127:0]

`ifdef jx2_mem_l1dsz_1024
`define			reg_l1d_ix	reg[9:0]
	`jx2_mem_l1darr_addrb		arrMemAddrA[1023:0];
	`jx2_mem_l1darr_addrb		arrMemAddrB[1023:0];
	`jx2_mem_l1darr_data		arrMemDataA[1023:0];
	`jx2_mem_l1darr_data		arrMemDataB[1023:0];
`endif

`ifdef jx2_mem_l1dsz_512
`define			reg_l1d_ix	reg[8:0]
	`jx2_mem_l1darr_addrb		arrMemAddrA[511:0];
	`jx2_mem_l1darr_addrb		arrMemAddrB[511:0];
	`jx2_mem_l1darr_data		arrMemDataA[511:0];
	`jx2_mem_l1darr_data		arrMemDataB[511:0];
`endif

`ifdef jx2_mem_l1dsz_256
`define			reg_l1d_ix	reg[7:0]
	`jx2_mem_l1darr_addrb		arrMemAddrA[255:0];
	`jx2_mem_l1darr_addrb		arrMemAddrB[255:0];
	`jx2_mem_l1darr_data		arrMemDataA[255:0];
	`jx2_mem_l1darr_data		arrMemDataB[255:0];
`endif

`ifdef jx2_mem_l1dsz_128
`define			reg_l1d_ix	reg[6:0]
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrA[127:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrB[127:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataA[127:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataB[127:0];
`endif

`ifdef jx2_mem_l1dsz_64
`define			reg_l1d_ix	reg[5:0]
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrA[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrB[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataA[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataB[63:0];
`endif


`ifdef jx2_mem_lane2
`define			reg_l1d_ix_b	reg[5:0]
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrE[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_addrb		arrMemAddrF[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataE[63:0];
(* ram_style = "distributed" *)
	`jx2_mem_l1darr_data		arrMemDataF[63:0];
`endif

reg				tResetL;

`jx2_mem_l1darr_addr		tArrMemAddrStA;
`jx2_mem_l1darr_addr		tArrMemAddrStB;
reg[127:0]		tArrMemDataStA;
reg[127:0]		tArrMemDataStB;
reg[15:0]		tArrMemDextStA;
reg[15:0]		tArrMemDextStB;
reg[19:0]		tArrMemAextStA;
reg[19:0]		tArrMemAextStB;
reg[7:0]		tArrMemChkStA;
reg[7:0]		tArrMemChkStB;
`reg_l1d_ix		tArrMemIdxStA;
`reg_l1d_ix		tArrMemIdxStB;
reg				tArrMemDoStA;
reg				tArrMemDoStB;
reg				tArrMemDidStA;
reg				tArrMemDidStB;

`jx2_mem_l1darr_addr		tArrMemDidStAddrA;
`jx2_mem_l1darr_addr		tArrMemDidStAddrB;
reg[127:0]		tArrMemDidStDataA;
reg[127:0]		tArrMemDidStDataB;
reg[15:0]		tArrMemDidStDextA;
reg[15:0]		tArrMemDidStDextB;
reg[19:0]		tArrMemDidStAextA;
reg[19:0]		tArrMemDidStAextB;

`reg_l1d_ix		tReqSeqIdxArr[15:0];
reg[ 43:0]		tReqSeqVaArr[15:0];

`reg_l1d_ix		tReqSeqIdx;
reg[ 43:0]		tReqSeqVa;

reg[  3:0]		tFlushRov;
reg[  3:0]		tNxtFlushRov;

reg[ 63:0]		tFlushRng;
reg[ 63:0]		tNxtFlushRng;

reg[  3:0]		tFlushRovTlb;
reg[  3:0]		tNxtFlushRovTlb;

`reg_l1d_ix		tSyncRov;
`reg_l1d_ix		tNxtSyncRov;
reg[11:0]		tSyncEpoch;
reg[11:0]		tNxtSyncEpoch;
reg[11:0]		tSyncEpochL;

reg[47:0]		tNxtReqAddrHi;
reg[47:0]		tReqAddrHi;

reg				tNxtReqAddrHiIsNz;
reg				tReqAddrHiIsNz;

reg[47:0]		tNxtReqAddr;
`reg_l1d_ix		tNxtReqIxA;
`reg_l1d_ix		tNxtReqIxB;
reg[ 43:0]		tNxtReqAxA;
reg[ 43:0]		tNxtReqAxB;
reg[47:0]		tReqAddr;
`reg_l1d_ix		tReqIxA;
`reg_l1d_ix		tReqIxB;
`reg_l1d_ix		tReqIxAL;
`reg_l1d_ix		tReqIxBL;
reg[ 43:0]		tReqAxA;
reg[ 43:0]		tReqAxB;
`reg_l1d_ix		tReq1IxA;
`reg_l1d_ix		tReq1IxB;
`reg_l1d_ix		tArrMemDidStIxA;
`reg_l1d_ix		tArrMemDidStIxB;

`reg_l1d_ix		tReqMissIxA;
`reg_l1d_ix		tReqMissIxB;
reg[ 43:0]		tReqMissAxA;
reg[ 43:0]		tReqMissAxB;

reg[15:0]		tReqAxH;
reg[15:0]		tNxtReqAxH;
reg[15:0]		tReq2AxH;

reg[  4:0]		tNxtReqBix;
reg[  4:0]		tReqBix;
reg[  5:0]		tNxtReqOpm;
reg[  5:0]		tReqOpm;
reg[  7:0]		tNxtReqLdOp;
reg[  7:0]		tReqLdOp;

`ifdef jx2_mem_lane2
reg[47:0]		tNxtReqAddrB;
`reg_l1d_ix_b	tNxtReqIxE;
`reg_l1d_ix_b	tNxtReqIxF;
reg[ 43:0]		tNxtReqAxE;
reg[ 43:0]		tNxtReqAxF;
reg[47:0]		tReqAddrB;
`reg_l1d_ix_b	tReqIxE;
`reg_l1d_ix_b	tReqIxF;
reg[ 43:0]		tReqAxE;
reg[ 43:0]		tReqAxF;
`reg_l1d_ix_b	tReq1IxE;
`reg_l1d_ix_b	tReq1IxF;
`reg_l1d_ix_b	tArrMemDidStIxE;
`reg_l1d_ix_b	tArrMemDidStIxF;

reg[  4:0]		tNxtReqBixB;
reg[  4:0]		tReqBixB;
reg[  5:0]		tNxtReqOpmB;
reg[  5:0]		tReqOpmB;
`endif

//reg[3:0]		tInPmode;
//reg[3:0]		tNxtInPmode;

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
reg				tReqMissAddrLoA;
reg				tReqMissAddrLoB;

reg				tReqMissA;
reg				tReqMissB;
reg				tReqDoMissA;
reg				tReqDoMissB;
reg				tReqDoMissAL;
reg				tReqDoMissBL;
reg				tReqMiss;
reg				tReqWaitResp;
reg				tReqSx;
reg				tReqSxB;

reg				tReqDoMissE;
reg				tReqDoMissF;

reg				tReqMissAL;
reg				tReqMissBL;
reg				tReqMissL;

reg				tReqFlushAddrA;
reg				tReqFlushAddrB;
reg				tReqWeakFlushAddrA;
reg				tReqWeakFlushAddrB;

reg				tReqMissAddrE;
reg				tReqMissAddrF;
reg				tReqMissAddrLoE;
reg				tReqMissAddrLoF;
reg				tReqMissE;
reg				tReqMissF;
reg				tReqFlushAddrE;
reg				tReqFlushAddrF;

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

reg				tReqNoCrossB;
reg				tReqMissSkipE;
reg				tReqMissSkipF;

reg				tReqIsNz;
reg				tReqIsNzB;
reg				tReqIsMmio;
reg				tReq2IsMmio;
reg				tReqIsCcmd;
reg				tReq2IsCcmd;

reg				tNxtReqIsNz;
reg				tNxtReqIsMmio;
reg				tNxtReqIsCcmd;

reg				tReqDoPfxA;
reg				tReqDoPfxB;
reg				tReqDoSpxA;
reg				tReqDoSpxB;


(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlkMemAddrA;
(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlkMemAddrB;

(* max_fanout = 200 *)
	reg[127:0]		tBlkMemDataA;
(* max_fanout = 200 *)
	reg[127:0]		tBlkMemDataB;

(* max_fanout = 200 *)
	reg[15:0]		tBlkMemDextA;
(* max_fanout = 200 *)
	reg[15:0]		tBlkMemDextB;
(* max_fanout = 200 *)
	reg[19:0]		tBlkMemAextA;
(* max_fanout = 200 *)
	reg[19:0]		tBlkMemAextB;

`reg_l1d_ix		tBlkMemIdxA;
`reg_l1d_ix		tBlkMemIdxB;
reg				tBlkIsDirtyA;
reg				tBlkIsDirtyB;
reg				tBlkIsNoCacheA;
reg				tBlkIsNoCacheB;
reg				tBlkIsWriteThruA;
reg				tBlkIsWriteThruB;

`ifdef jx2_mem_lane2
`jx2_mem_l1darr_addr	tBlkMemAddrE;
`jx2_mem_l1darr_addr	tBlkMemAddrF;
reg[127:0]				tBlkMemDataE;
reg[127:0]				tBlkMemDataF;
reg[15:0]				tBlkMemDextE;
reg[15:0]				tBlkMemDextF;
reg[19:0]				tBlkMemAextE;
reg[19:0]				tBlkMemAextF;

reg				tBlkIsDirtyE;
reg				tBlkIsDirtyF;
reg				tBlkIsNoCacheE;
reg				tBlkIsNoCacheF;
reg				tBlkIsWriteThruE;
reg				tBlkIsWriteThruF;
`endif

reg[3:0]		tBlkEpochDeltaA;
reg[3:0]		tBlkEpochDeltaB;

(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlkMemAddr2A;
(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlkMemAddr2B;

reg[127:0]		tBlkMemData2A;
reg[127:0]		tBlkMemData2B;

(* max_fanout = 200 *)
	reg[15:0]		tBlkMemDext2A;
(* max_fanout = 200 *)
	reg[15:0]		tBlkMemDext2B;
(* max_fanout = 200 *)
	reg[19:0]		tBlkMemAext2A;
(* max_fanout = 200 *)
	reg[19:0]		tBlkMemAext2B;

reg[7:0]		tBlkMemChk2A;
reg[7:0]		tBlkMemChk2B;
reg[7:0]		tBlkMemRChk2A;
reg[7:0]		tBlkMemRChk2B;

`ifdef jx2_mem_lane2
`jx2_mem_l1darr_addr	tBlkMemAddr2E;
`jx2_mem_l1darr_addr	tBlkMemAddr2F;
reg[127:0]				tBlkMemData2E;
reg[127:0]				tBlkMemData2F;
reg[15:0]				tBlkMemDext2E;
reg[15:0]				tBlkMemDext2F;
reg[19:0]				tBlkMemAext2E;
reg[19:0]				tBlkMemAext2F;
`endif

(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlk2StoreAddrA;
(* max_fanout = 200 *)
	`jx2_mem_l1darr_addr		tBlk2StoreAddrB;
reg[127:0]		tBlk2StoreDataA;
reg[127:0]		tBlk2StoreDataB;

(* max_fanout = 200 *)
	reg[15:0]		tBlk2StoreDextA;
(* max_fanout = 200 *)
	reg[15:0]		tBlk2StoreDextB;
(* max_fanout = 200 *)
	reg[19:0]		tBlk2StoreAextA;
(* max_fanout = 200 *)
	reg[19:0]		tBlk2StoreAextB;

// reg[ 7:0]		tBlk2StoreChkA;
// reg[ 7:0]		tBlk2StoreChkB;

`ifdef jx2_mem_misal_movx

reg[255:0]		tBlkExData0;
reg[191:0]		tBlkExData1;
reg[151:0]		tBlkExData2;
reg[135:0]		tBlkExData3;
reg[127:0]		tBlkExData4;

`ifdef jx2_mem_lane2
reg[255:0]		tBlkExData0B;
reg[191:0]		tBlkExData1B;
reg[151:0]		tBlkExData2B;
reg[135:0]		tBlkExData3B;
reg[127:0]		tBlkExData4B;
`endif

reg[255:0]		tBlkInsData0;
reg[191:0]		tBlkInsData1;
reg[151:0]		tBlkInsData2;
reg[135:0]		tBlkInsData3;
reg[127:0]		tBlkInsData4;

`else

reg[255:0]		tBlkExData0;
reg[127:0]		tBlkExData1;
reg[ 87:0]		tBlkExData2;
reg[ 71:0]		tBlkExData3;
reg[ 63:0]		tBlkExData4;

`ifdef jx2_mem_lane2
reg[255:0]		tBlkExData0B;
reg[127:0]		tBlkExData1B;
reg[ 87:0]		tBlkExData2B;
reg[ 71:0]		tBlkExData3B;
reg[ 63:0]		tBlkExData4B;
`endif

reg[255:0]		tBlkInsData0;
reg[127:0]		tBlkInsData1;
reg[ 87:0]		tBlkInsData2;
reg[ 71:0]		tBlkInsData3;
reg[ 63:0]		tBlkInsData4;

`endif

reg[ 63:0]		tBlkExData;

reg[ 63:0]		tBlkExDataA;
reg[ 63:0]		tBlkExDataB;


reg[127:0]		tBlk2MemDataA;
reg[127:0]		tBlk2MemDataB;

(* max_fanout = 100 *)
	`jx2_mem_l1darr_addr		tBlk2MemAddrA;
(* max_fanout = 100 *)
	`jx2_mem_l1darr_addr		tBlk2MemAddrB;

(* max_fanout = 200 *)
	reg[ 15:0]		tBlk2MemDextA;
(* max_fanout = 200 *)
	reg[ 15:0]		tBlk2MemDextB;
(* max_fanout = 200 *)
	reg[ 19:0]		tBlk2MemAextA;
(* max_fanout = 200 *)
	reg[ 19:0]		tBlk2MemAextB;

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

`ifdef jx2_mem_lane2
reg				tReg2MissInterlockE;
reg				tReg2MissInterlockF;
reg				tMemArrInterlockE;
reg				tMemArrInterlockF;
`endif

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

reg				tMemReqStA;			//Store A
reg				tMemReqStB;			//Store B
reg				tNxtMemReqStA;		//Store A
reg				tNxtMemReqStB;		//Store B
reg				tNxtMemReqStAL;		//Store A
reg				tNxtMemReqStBL;		//Store B

reg				tMemRespStA;		//Store A
reg				tMemRespStB;		//Store B
reg				tNxtMemRespStA;		//Store A
reg				tNxtMemRespStB;		//Store B

reg				tMemRespLdA;		//Store A
reg				tMemRespLdB;		//Store B
reg				tNxtMemRespLdA;		//Store A
reg				tNxtMemRespLdB;		//Store B

reg				tMemReqLdA;			//Load A
reg				tMemReqLdB;			//Load B
reg				tMemReqLdM;			//Load MMIO
reg				tNxtMemReqLdA;		//Load A
reg				tNxtMemReqLdB;		//Load B
reg				tNxtMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdAL;		//Load A
reg				tNxtMemReqLdBL;		//Load B
reg				tNxtMemReqLdML;		//Load MMIO

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

`reg_l1d_ix		tMemSeqIx;
reg[43:0]		tMemSeqVa;
`reg_l1d_ix		tMemSeqIxL;
reg[43:0]		tMemSeqVaL;

reg[5:0]		tInOpm;				//OPM (Used for cache-control)
reg[5:0]		tInOpmB;			//OPM (Used for cache-control)
reg[5:0]		tInOpmC;			//OPM (Used for cache-control)
reg				tNxtDoFlush;
reg				tNxtDoFlushTlb;
reg				tDoFlush;
reg				tDoFlushTlb;
reg				tDoFlushL;
reg				tDoFlushTlbL;

(* max_fanout = 400 *)
	reg				tTlbMissInh;

reg				tNxtTlbMissInh;
reg				tNxtTlbMissInh2;

(* max_fanout = 400 *)
	reg			tSkipTlb;
(* max_fanout = 400 *)
	reg			tNxtSkipTlb;

(* max_fanout = 400 *)
	reg			tReqAddrIsVirt;
reg				tWasMissInh;

reg				tNxtReqAddrIsVirt;

reg[2:0]		tVolatileInhSet;	//Volatile Inhibit Cycles (Set)
reg[2:0]		tVolatileInhCnt;	//Volatile Inhibit Cycles (Count)
reg[2:0]		tNxtVolatileInh;
`reg_l1d_ix		tVolatileIxA;		//Volatile Evict Index
`reg_l1d_ix		tVolatileIxB;		//Volatile Evict Index
`reg_l1d_ix		tNxtVolatileIxA;
`reg_l1d_ix		tNxtVolatileIxB;


reg[95:0]		tUtlbArr[15:0];
reg[95:0]		tUtlbStAddr;
reg[3:0]		tUtlbStIx;
reg				tUtlbDoSt;

reg[95:0]		tUtlbBlkAddr;
reg[3:0]		tUtlbBlkIx;
reg[3:0]		tUtlb1BlkIx;
reg[3:0]		tNxtUtlbBlkIx;
reg				tUtlbBlkFlush;

reg[43:0]		tReqUtlbAxA;
reg[43:0]		tReqUtlbAxB;
reg[15:0]		tReqUtlbAxH;
reg[3:0]		tReqUtlbAccA;
reg[3:0]		tReqUtlbAccB;
reg				tReqUtlbHitHi;
reg				tReqUtlbHitAxA;
reg				tReqUtlbHitAxB;

reg				tReqStrobe1;
reg				tReqStrobe2;

always @*
begin
	/* EX1 */

`ifdef jx2_enable_vaddr48
	if(regInAddr[4])
	begin
		tNxtReqAxB = regInAddr[47:4];
		tNxtReqAxA = tNxtReqAxB + 1;
	end
	else
	begin
		tNxtReqAxA = regInAddr[47:4];
//		tNxtReqAxB = tNxtReqAxA + 1;
		tNxtReqAxB = { tNxtReqAxA[43:1], 1'b1 };
	end
`else
	if(regInAddr[4])
	begin
		tNxtReqAxB = { 16'h00, regInAddr[31:4] };
		tNxtReqAxA = tNxtReqAxB + 1;
	end
	else
	begin
		tNxtReqAxA = { 16'h00, regInAddr[31:4] };
//		tNxtReqAxB = tNxtReqAxA + 1;
		tNxtReqAxB = { tNxtReqAxA[43:1], 1'b1 };
	end
`endif

`ifdef jx2_mem_lane2
	if(regInAddrB[4])
	begin
		tNxtReqAxF = regInAddrB[47:4];
		tNxtReqAxE = tNxtReqAxF + 1;
	end
	else
	begin
		tNxtReqAxE = regInAddrB[47:4];
		tNxtReqAxF = tNxtReqAxE + 1;
	end
	
	if(regInOpmB==0)
	begin
		tNxtReqAxE = tNxtReqAxA;
		tNxtReqAxF = tNxtReqAxB;
	end
`endif

	tNxtReqAddrHi		= 0;
	tNxtReqAddrHiIsNz	= 0;

`ifdef jx2_enable_vaddr96
	tNxtReqAddrHi	= regInAddr[95:48];
	if(disableTlb)
		tNxtReqAddrHi	= 0;
	tNxtReqAddrHiIsNz = (tNxtReqAddrHi != 0);
`endif

`ifdef jx2_enable_vaddr48
	tNxtReqAddr		= regInAddr[47:0];
`else
	tNxtReqAddr		= { 16'h00, regInAddr[31:0] };
`endif
	tNxtReqBix		= regInAddr[4:0];
	tNxtReqOpm		= regInOpm;
//	tNxtReqLdOp		= regInLdOp;
	tNxtReqLdOp		= noLdOp ? 0 : regInLdOp;

`ifdef jx2_mem_lane2
	tNxtReqAddrB	= regInAddrB[47:0];
	tNxtReqBixB		= regInAddrB[4:0];
	tNxtReqOpmB		= regInOpmB;
`endif

//	tNxtInPmode		= regInSr[31:28];
//	tNxtInPmode		= regInSr[31:28] ^ regKrrHash[3:0] ^ regKrrHash[7:4];

	tNxtUtlbBlkIx	= regInAddr[15:12] ^ regInAddr[19:16];

	if(regInSr[28] != tRegInSr[28])
		tNxtReqOpm		= 0;

//	tNxtReqAxH		=
//		tNxtReqAddrHi[15: 0] ^
//		tNxtReqAddrHi[31:16] ^
//		tNxtReqAddrHi[47:32] ;

`ifdef def_true
// `ifndef def_true
	tNxtReqAxH		=
		tNxtReqAddrHi[15: 0] ^
		{	tNxtReqAddrHi[23:16], tNxtReqAddrHi[31:24] } ^
		{	tNxtReqAddrHi[35:32], tNxtReqAddrHi[39:36],
			tNxtReqAddrHi[43:40], tNxtReqAddrHi[47:44] } ^
//		{	4'h0, regInSr[31:28], regKrrHash[7:0] } ;
		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;
`endif

`ifndef def_true
// `ifdef def_true
	tNxtReqAxH		=
		tNxtReqAddrHi[15: 0] +
		{	tNxtReqAddrHi[23:16], tNxtReqAddrHi[31:24] } +
		{	tNxtReqAddrHi[35:32], tNxtReqAddrHi[39:36],
			tNxtReqAddrHi[43:40], tNxtReqAddrHi[47:44] } +
//		{	4'h0, regInSr[31:28], regKrrHash[7:0] } ;
		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;
`endif

`ifdef jx2_mem_lane2
	tNxtReqIxE[5:0] = tNxtReqAxE[6:1];
	tNxtReqIxF[5:0] = tNxtReqAxF[6:1];
`endif

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
//	tNxtReqIxA[9:0] = tNxtReqAxA[10:1] ^ (tNxtReqAxA[11]?UV10_FF:UV10_00);
//	tNxtReqIxB[9:0] = tNxtReqAxB[10:1] ^ (tNxtReqAxB[11]?UV10_FF:UV10_00);
`endif

`endif

	tNxtSyncRov		= tSyncRov;
	tNxtSyncEpoch	= tSyncEpoch;

//	if(regInOpm[5:4] == 2'b00)
	if(regInOpm == 0)
//	if(1'b0)
	begin
		tNxtReqAddr		= 0;
		tNxtReqAddrHi	= 0;
		tNxtReqAxA		= 0;
		tNxtReqAxB		= 1;
		tNxtReqAxH		= UV16_FF;
		tNxtUtlbBlkIx	= 0;

		tNxtReqIxA		= tSyncRov;
		tNxtReqIxB		= tSyncRov;
	
		if((tVolatileIxA != 0) || (tVolatileIxB != 0))
		begin
			if(tVolatileIxA != 0)
				tNxtReqIxA = tVolatileIxA;
			if(tVolatileIxB != 0)
				tNxtReqIxB = tVolatileIxB;
		end
		else
		begin
			tNxtSyncRov	= tSyncRov + 1;
			if(tSyncRov == 0)
				tNxtSyncEpoch	= tSyncEpoch + 1;
		end
	end


`ifdef def_true
	tNxtReqIsNz		= tNxtReqOpm[5:4] != 2'b00;

	tNxtReqIsMmio		= tNxtReqIsNz &&
//		((((tNxtReqAddr[47:32] == 16'h0000) && !tSrJQ) ||
//			(tNxtReqAddr[47:32] == 16'hFFFF)) &&
//			(tNxtReqAddr[31:28] == 4'hF)) ||
		((tNxtReqAddr[31:28] == 4'hF) && !tSrJQ) ||

//		(tNxtReqAddr[47:32] == 16'hF000);
		((tNxtReqAddr[47:44] == 4'hF) && tSrJQ);
	tNxtReqIsCcmd		=
		(tNxtReqOpm[5:4] == 2'b00) &&
		(tNxtReqOpm[3:0] != 4'b0000);

	tNxtReqAddrIsVirt	=
//		((tNxtReqAddr[47:32] != 0) ||
//		 (tNxtReqAddr[31:28] != 0)) &&
		(tNxtReqAddr[31:28] != 0) &&
		!tNxtReqAddr[47] &&
		!tNxtReqIsMmio && !tNxtReqIsCcmd;

	if(tNxtReqAddrHiIsNz)
	begin
		tNxtReqIsMmio		= 0;
		tNxtReqAddrIsVirt	= 1;
	end
`endif

	tNxtReqInValA	= regInValA;
	tNxtReqInValB	= regInValB;

	if(dcInHold)
	begin
		tReq1IxA		= tReqIxA;
		tReq1IxB		= tReqIxB;
		tUtlb1BlkIx		= tUtlbBlkIx;

`ifdef jx2_mem_lane2
		tReq1IxE		= tReqIxE;
		tReq1IxF		= tReqIxF;
`endif
	end
	else
	begin
		tReq1IxA		= tNxtReqIxA;
		tReq1IxB		= tNxtReqIxB;
		tUtlb1BlkIx		= tNxtUtlbBlkIx;

`ifdef jx2_mem_lane2
		tReq1IxE		= tNxtReqIxE;
		tReq1IxF		= tNxtReqIxF;
`endif
	end
end

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tNxtFlushRov		= tFlushRov;
	tNxtFlushRng		= tFlushRng;
	tNxtFlushRovTlb		= tFlushRovTlb;
	tNxtDoFlush			= 0;
	tNxtDoFlushTlb		= 0;
	tNxtSkipTlb			= 0;

//	if((tInOpm == JX2_DCOPM_LDTLB) || tRegInSr[29])
//	if((tInOpm == JX2_DCOPM_LDTLB) || (tRegInSr[29] && tRegInSr[30]))
//	if(tRegInSr[29] && tRegInSr[30])
//	if(	(tRegInSr[29] && tRegInSr[30]) &&
//		(tRegInSrL[29] && tRegInSrL[30]))
	if(	(tRegInSr[29] && tRegInSr[30]) &&
		(regInSr[29] && regInSr[30]) &&
		(tRegInSrL[29] && tRegInSrL[30]))
	begin
//		if(tTlbMissInh)
//			$display("L1D$ Clear TLB Inhibit");
		tNxtTlbMissInh	= 0;
//		tNxtSkipTlb		= 1;
	end

//	tNxtSkipTlb		= (regInSr[29] && regInSr[30]);
	tNxtSkipTlb		= (tRegInSr[29] && tRegInSr[30]);
//	tNxtSkipTlb		= (tRegInSrL[29] && tRegInSrL[30]);

	if(disableTlb)
		tNxtSkipTlb = 1;

	if(!tRegInMmcr[0])
		tNxtSkipTlb = 1;

`ifdef jx2_debug_isr

`ifdef jx2_enable_vaddr48
	if(tNxtSkipTlb && (regInAddr[47:28]!=0) &&
//		(regInAddr[47:44]!=4'hF) &&
		(regInAddr[47:46]!=2'b11) &&
		(regInAddr[31:28]!=4'hF) &&
		dcInHoldN &&
		(regInOpm[5:4] != 2'b00))
	begin
		$display("L1 D$: Next Skip TLB and Addr is Virt, A=%X",
			regInAddr);
	end
`endif

`endif

`ifndef def_true
// `ifdef def_true
	if(tReqIxAL != tReqIxA)
	begin
		if(tTlbMissInh)
			$display("L1D$ Clear TLB Inhibit, Drop");
		tNxtTlbMissInh		= 0;
	end
`endif

//	if(((tInOpm==JX2_DCOPM_FLUSHDS) && (tInOpmC!=JX2_DCOPM_FLUSHDS)) || tResetL)
	if((tInOpm==JX2_DCOPM_FLUSHDS) && (tInOpmC!=JX2_DCOPM_FLUSHDS) && !tResetL)
	begin
		tNxtDoFlush = 1;
		tNxtDoFlushTlb = 1;
	end

	if((tInOpm==JX2_DCOPM_INVTLB) && (tInOpmC!=JX2_DCOPM_INVTLB) && !tResetL)
	begin
		tNxtDoFlushTlb = 1;
	end

	if((tInOpm == JX2_DCOPM_LDTLB) && (tInOpmC!=JX2_DCOPM_LDTLB) && !tResetL)
	begin
		tNxtDoFlushTlb = 1;
	end
	
	if((tFlushRov == 0) && !tDoFlush && !tResetL)
		tNxtDoFlush = 1;
	if((tFlushRovTlb == 0) && !tDoFlushTlb && !tResetL)
		tNxtDoFlushTlb = 1;

	if(tDoFlush && !tDoFlushL)
	begin
`ifdef jx2_debug_isr
		$display("L1 D$ DoFlush rov=%X", tFlushRov);
`endif
		tNxtFlushRov = tFlushRov + 1;
		tNxtFlushRng = tRegRng1;
	end

	if(tDoFlushTlb && !tDoFlushTlbL)
	begin
`ifdef jx2_debug_isr
		$display("L1 D$ DoFlushTlb rov=%X", tFlushRovTlb);
`endif
		tNxtFlushRovTlb = tFlushRovTlb + 1;
	end

	if(tResetL)
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

`ifdef jx2_mem_lane2
	tReg2MissInterlockE = 0;
	tReg2MissInterlockF = 0;
	tMemArrInterlockE	= 0;
	tMemArrInterlockF	= 0;
`endif

	tArrMemAddrStA	= 0;
	tArrMemDataStA	= 0;
	tArrMemIdxStA	= 0;
	tArrMemDoStA	= 0;
	tArrMemChkStA	= 0;
	tArrMemDextStA	= 0;
	tArrMemAextStA	= 0;
	
	tArrMemAddrStB	= 0;
	tArrMemDataStB	= 0;
	tArrMemIdxStB	= 0;
	tArrMemDoStB	= 0;
	tArrMemChkStB	= 0;
	tArrMemDextStB	= 0;
	tArrMemAextStB	= 0;
	
	if(tReq2Opm[5] && !tReq2IsMmio && !tReq2IsCcmd && !tReq2StoreSticky)
	begin
		tReg2MissInterlockA		= (tReqIxA == tReq2IxA) && !tReq2MissSkipA;
		tReg2MissInterlockB		= (tReqIxB == tReq2IxB) && !tReq2MissSkipB;

`ifdef jx2_mem_lane2
		tReg2MissInterlockE		= (tReqIxE == tReq2IxA[5:0]) && 
			!tReq2MissSkipA;
		tReg2MissInterlockF		= (tReqIxF == tReq2IxB[5:0]) && 
			!tReq2MissSkipB;
`endif
	end


	tReqNoCross		= 0;
	tReqNoCrossB	= 0;
	tReqMissSkipA	= 0;
	tReqMissSkipB	= 0;
	tReqMissNoSkip	= 0;
	tReqDoPfxA		= 0;
	tReqDoPfxB		= 0;
	tReqDoSpxA		= 0;
	tReqDoSpxB		= 0;

	tReqMissSkipE	= 0;
	tReqMissSkipF	= 0;

	tNxtMemMmioData = tMemMmioData;
	tNxtMemMmioReady = tMemMmioReady;
	


`ifndef def_true
// `ifdef def_true

	tReqIsNz		= tReqOpm[5:4] != 2'b00;

`ifdef jx2_mem_lane2
	tReqIsNzB		= tReqOpmB[5:4] != 2'b00;
`else
	tReqIsNzB		= 0;
`endif

	tReqIsMmio		= tReqIsNz &&
		((((tReqAddr[47:32] == 16'h0000) && !tSrJQ) ||
			(tReqAddr[47:32] == 16'hFFFF)) &&
			(tReqAddr[31:28] == 4'hF)) ||
//		(tReqAddr[47:32] == 16'hF000);
		((tReqAddr[47:44] == 4'hF) && tSrJQ);
	tReqIsCcmd		= (tReqOpm[5:4] == 2'b00) && (tReqOpm[3:0] != 4'b0000);

	tReqAddrIsVirt	= (tReqAddr[47:28] != 0) && !tReqAddr[47] &&
		!tReqIsMmio && !tReqIsCcmd;
`endif

`ifndef def_true
//	if(tReqAddrHi != 0)
	if(tReqAddrHiIsNz)
	begin
		/* Outside of Quadrant 0, there is no MMIO. */
		tReqIsMmio		= 0;
		tReqAddrIsVirt	= (tReqAddr[47:28] != 0) && !tReqIsCcmd;
	end
`endif
	
	tReqNoCross		= 0;

`ifndef def_true
	casez(tReqOpm[2:0])
		3'bz00:		tReqNoCross = 1;
		3'bz01:		tReqNoCross = !tReqBix[0] || (tReqBix[3:1]!=3'b111);
		3'bz10:		tReqNoCross =	(tReqBix[1:0]==2'h0) || 
									(tReqBix[3:2]!=2'b11);
		3'b011:		tReqNoCross = (tReqBix[2:0]==3'h0) || !tReqBix[3];
		3'b111:		tReqNoCross = (tReqBix[3:0]==4'h0);
	endcase
`endif

`ifdef def_true
	casez( { tReqOpm[2:0], tReqBix[3:0] } )
		7'bz00_zzzz:	tReqNoCross = 1;
		
		7'bz01_0zzz:	tReqNoCross = 1;
		7'bz01_10zz:	tReqNoCross = 1;
		7'bz01_110z:	tReqNoCross = 1;
		7'bz01_1110:	tReqNoCross = 1;
		7'bz01_1111:	tReqNoCross = 0;

		7'bz10_0zzz:	tReqNoCross = 1;
		7'bz10_10zz:	tReqNoCross = 1;
		7'bz10_1100:	tReqNoCross = 1;
		7'bz10_1101:	tReqNoCross = 0;
		7'bz10_111z:	tReqNoCross = 0;

		7'b011_0zzz:	tReqNoCross = 1;
		7'b011_1000:	tReqNoCross = 1;
		7'b011_1001:	tReqNoCross = 0;
		7'b011_101z:	tReqNoCross = 0;
		7'b011_11zz:	tReqNoCross = 0;

		7'b111_0000:	tReqNoCross = 1;
		7'b111_0001:	tReqNoCross = 0;
		7'b111_001z:	tReqNoCross = 0;
		7'b111_01zz:	tReqNoCross = 0;
		7'b111_1zzz:	tReqNoCross = 0;
	endcase
`endif

`ifdef jx2_mem_lane2
	tReqNoCrossB		= 0;
	casez(tReqOpmB[2:0])
		3'bz00:		tReqNoCrossB = 1;
		3'bz01:		tReqNoCrossB = !tReqBixB[0] || (tReqBixB[3:1]!=3'b111);
		3'bz10:		tReqNoCrossB =	(tReqBixB[1:0]==2'h0) || 
									(tReqBixB[3:2]!=2'b11);
		3'b011:		tReqNoCrossB = (tReqBixB[2:0]==3'h0) || !tReqBixB[3];
		3'b111:		tReqNoCrossB = (tReqBixB[3:0]==4'h0);
	endcase
`endif

	if(tReqOpm[5] && !tReqOpm[1])
//	if(tReqOpm[5])
		tReqNoCross = 0;

//	tReqNoCross = 0;

	if(tReqNoCross)
	begin
		if(tReqBix[4])
			tReqMissSkipA	= 1;
		else
			tReqMissSkipB	= 1;
	end

`ifdef jx2_mem_lane2
	if(tReqNoCrossB)
	begin
		if(tReqBixB[4])
			tReqMissSkipE	= 1;
		else
			tReqMissSkipF	= 1;
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
			else
				tReqDoSpxA		= 1;
		end
	end
`endif

	tReqReadyA	= (tBlkMemIdxA == tReqIxA);
	tReqReadyB	= (tBlkMemIdxB == tReqIxB);
	tReqReady	= tReqReadyA && tReqReadyB;

	tBlkMemData2A = tBlkMemDataA;
	tBlkMemData2B = tBlkMemDataB;
	tBlkMemAddr2A = tBlkMemAddrA;
	tBlkMemAddr2B = tBlkMemAddrB;
	tBlkMemDext2A = tBlkMemDextA;
	tBlkMemDext2B = tBlkMemDextB;
	tBlkMemAext2A = tBlkMemAextA;
	tBlkMemAext2B = tBlkMemAextB;

`ifdef jx2_mem_lane2
	tBlkMemData2E = tBlkMemDataE;
	tBlkMemData2F = tBlkMemDataF;
	tBlkMemAddr2E = tBlkMemAddrE;
	tBlkMemAddr2F = tBlkMemAddrF;
	tBlkMemDext2E = tBlkMemDextE;
	tBlkMemDext2F = tBlkMemDextF;
	tBlkMemAext2E = tBlkMemAextE;
	tBlkMemAext2F = tBlkMemAextF;
`endif

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

//	tBlk2StoreDextA		= tReq2AxH;
//	tBlk2StoreDextB		= tReq2AxH;
	tBlk2StoreDextA		= tBlk2MemDextA;
	tBlk2StoreDextB		= tBlk2MemDextB;

	tBlk2StoreAextA		= tBlk2MemAextA;
	tBlk2StoreAextB		= tBlk2MemAextB;

	tMemArrInterlockA	= tArrMemDidStA && (tReqIxA == tArrMemDidStIxA);
	tMemArrInterlockB	= tArrMemDidStB && (tReqIxB == tArrMemDidStIxB);

`ifdef jx2_mem_lane2
	tMemArrInterlockE	= tArrMemDidStA && (tReqIxE == tArrMemDidStIxE);
	tMemArrInterlockF	= tArrMemDidStB && (tReqIxF == tArrMemDidStIxF);
`endif


// `ifdef def_true
`ifndef def_true
	/* If entirely replacing the block, update to current epoch. */
	if(tReqDoSpxA)
	begin
		tBlk2StoreAextA[3:2]=tSyncEpoch[7:6];
		tBlk2StoreAextA[1:0]=tSyncEpoch[3:2];
	end
	if(tReqDoSpxB)
	begin
		tBlk2StoreAextB[3:2]=tSyncEpoch[7:6];
		tBlk2StoreAextB[1:0]=tSyncEpoch[3:2];
	end
`endif

`ifdef jx2_mem_l1d_fwstore
	if(tReg2MissInterlockA)
	begin		
`ifdef def_true
		/* If store forwarding does not forward address... */
		tReg2StoreFwA = 1;
		tBlkMemAddr2A[4] = 1'b1;
		tBlkMemData2A = tBlk2StoreDataA;
		if(tMemArrInterlockA)
		begin
			tReg2StoreFwA = 0;
			tReqReady	= 0;
		end
`else
		tReg2StoreFwA = 1;
		tBlkMemAddr2A = tBlk2StoreAddrA;
		tBlkMemData2A = tBlk2StoreDataA;
		tBlkMemDext2A = tBlk2StoreDextA;
		tBlkMemAext2A = tBlk2StoreAextA;
`endif
	end

	if(tReg2MissInterlockB)
	begin
`ifdef def_true
		/* If store forwarding does not forward address... */
		tReg2StoreFwB = 1;
		tBlkMemAddr2B[4] = 1'b1;
		tBlkMemData2B = tBlk2StoreDataB;
		if(tMemArrInterlockB)
		begin
			tReg2StoreFwB = 0;
			tReqReady	= 0;
		end
`else
		tReg2StoreFwB = 1;
		tBlkMemAddr2B = tBlk2StoreAddrB;
		tBlkMemData2B = tBlk2StoreDataB;
		tBlkMemDext2B = tBlk2StoreDextB;
		tBlkMemAext2B = tBlk2StoreAextB;

`endif
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

`ifdef jx2_mem_lane2
	if(tReg2MissInterlockE && tReqIsNzB)
	begin
//		$display("L1 D$: Line E Interlock");
		tReqReady	= 0;
	end
	if(tReg2MissInterlockF && tReqIsNzB)
	begin
//		$display("L1 D$: Line F Interlock");
		tReqReady	= 0;
	end
`endif

`ifdef jx2_mem_l1d_fwarray
	if(tMemArrInterlockA && !tReg2StoreFwA)
	begin
		tMemArrFwA		= 1;
		tBlkMemData2A	= tArrMemDidStDataA;
		tBlkMemAddr2A	= tArrMemDidStAddrA;
		tBlkMemDext2A	= tArrMemDidStDextA;
		tBlkMemAext2A	= tArrMemDidStAextA;
	end
	if(tMemArrInterlockB && !tReg2StoreFwB)
	begin
		tMemArrFwB		= 1;
		tBlkMemData2B	= tArrMemDidStDataB;
		tBlkMemAddr2B	= tArrMemDidStAddrB;
		tBlkMemDext2B	= tArrMemDidStDextB;
		tBlkMemAext2B	= tArrMemDidStAextB;
	end
`else
//	if(tMemArrInterlockA)
	if(tMemArrInterlockA && !tReg2StoreFwA)
	begin
//		$display("L1 D$: Line A Store Clash");
		tReqReady	= 0;
	end
//	if(tMemArrInterlockB)
	if(tMemArrInterlockB && !tReg2StoreFwB)
	begin
//		$display("L1 D$: Line B Store Clash");
		tReqReady	= 0;
	end
`endif

`ifdef jx2_mem_lane2
	if(tMemArrInterlockE && tReqIsNzB)
	begin
//		$display("L1 D$: Line E Store Clash");
		tReqReady	= 0;
	end
	if(tMemArrInterlockF && tReqIsNzB)
	begin
//		$display("L1 D$: Line F Store Clash");
		tReqReady	= 0;
	end
`endif

`ifndef def_true
// `ifdef def_true
	if(tMemArrInterlockA && tReg2MissInterlockA)
	begin
		$display("L1 D$: Line A Double Interlock");
		tReqReady	= 0;
	end

	if(tMemArrInterlockB && tReg2MissInterlockB)
	begin
		$display("L1 D$: Line B Double Interlock");
		tReqReady	= 0;
	end
`endif

//	if(!tReqIsNz && (tMemArrInterlockA || tReg2MissInterlockA))
//		tReqReady = 0;
//	if(!tReqIsNz && (tMemArrInterlockB || tReg2MissInterlockB))
//		tReqReady = 0;

	if(tReqAxA[0])
		$display("L1D$: tReqAxA Even/Odd Error");
	if(!tReqAxB[0])
		$display("L1D$: tReqAxB Even/Odd Error");

	tReqReady2	= tReqReady && tReqReadyL;

	tBlkIsDirtyA	= tBlkMemAddr2A[4];
	tBlkIsDirtyB	= tBlkMemAddr2B[4];

	tBlkIsNoCacheA		= tBlkMemAext2A[7];
	tBlkIsNoCacheB		= tBlkMemAext2B[7];
	tBlkIsWriteThruA	= tBlkMemAext2A[9];
	tBlkIsWriteThruB	= tBlkMemAext2B[9];

`ifdef jx2_mem_lane2
	tBlkIsDirtyE	= tBlkMemAddr2E[4];
	tBlkIsDirtyF	= tBlkMemAddr2F[4];
	tBlkIsNoCacheE		= tBlkMemAext2E[7];
	tBlkIsNoCacheF		= tBlkMemAext2F[7];
	tBlkIsWriteThruE	= tBlkMemAext2E[9];
	tBlkIsWriteThruF	= tBlkMemAext2F[9];
`endif


	tBlkEpochDeltaA	=
		{	tSyncEpoch[7:6] - tBlkMemAext2A[3:2],
			tSyncEpoch[3:2] - tBlkMemAext2A[1:0] };
	tBlkEpochDeltaB	=
		{	tSyncEpoch[7:6] - tBlkMemAext2B[3:2],
			tSyncEpoch[3:2] - tBlkMemAext2B[1:0] };

	if(tReqIsNz)
	begin
//		$display(" Epoch %X/%X", tSyncEpoch[7:6], tBlkMemAext2A[3:2]);
//		$display(" Epoch %X/%X %X",
//			tSyncEpoch[3:2], tBlkMemAext2A[1:0],
//			tBlkEpochDeltaA);
	end

	tReqMissAddrA	=
		((tBlkMemAddr2A[47:32] != tReqAxA[43:28]) && tSrJQ) ||
//		 (tBlkMemAddr2A[31: 5] != tReqAxA[27: 1]);
		 (tBlkMemAddr2A[31:20] != tReqAxA[27:16]) ||
		 (tBlkMemAddr2A[19: 5] != tReqAxA[15: 1]);
	tReqMissAddrB	=
		((tBlkMemAddr2B[47:32] != tReqAxB[43:28]) && tSrJQ) ||
//		 (tBlkMemAddr2B[31: 5] != tReqAxB[27: 1]);
		 (tBlkMemAddr2B[31:20] != tReqAxB[27:16]) ||
		 (tBlkMemAddr2B[19: 5] != tReqAxB[15: 1]);

`ifdef jx2_mem_lane2
	tReqMissAddrE	=
		((tBlkMemAddr2E[47:32] != tReqAxE[43:28]) && tSrJQ) ||
		 (tBlkMemAddr2E[31: 5] != tReqAxE[27: 1]);
	tReqMissAddrF	=
		((tBlkMemAddr2F[47:32] != tReqAxF[43:28]) && tSrJQ) ||
		 (tBlkMemAddr2F[31: 5] != tReqAxF[27: 1]);
`else
	tReqMissAddrE	= 0;
	tReqMissAddrF	= 0;
`endif

	tReqMissAddrLoA	= tReqMissAddrA;
	tReqMissAddrLoB	= tReqMissAddrB;
	tReqMissAddrLoE	= tReqMissAddrE;
	tReqMissAddrLoF	= tReqMissAddrF;

	tReqFlushAddrA	= (tBlkMemAddr2A[71:68] != tFlushRov);
	tReqFlushAddrB	= (tBlkMemAddr2B[71:68] != tFlushRov);
	tReqWeakFlushAddrA	= 0;
	tReqWeakFlushAddrB	= 0;

//	if(tBlkMemDext2A != tReqAxH)
	if((tBlkMemDext2A != tReqAxH) && !tTlbMissInh)
		tReqMissAddrA	= 1;
//	if(tBlkMemDext2B != tReqAxH)
	if((tBlkMemDext2B != tReqAxH) && !tTlbMissInh)
		tReqMissAddrB	= 1;

	tReqFlushAddrE = 0;
	tReqFlushAddrF = 0;

// `ifdef jx2_mem_lane2
`ifndef def_true
	tReqFlushAddrE	= (tBlkMemAddr2E[71:68] != tFlushRov);
	tReqFlushAddrF	= (tBlkMemAddr2F[71:68] != tFlushRov);

	if((tBlkMemDext2E != tReqAxH) && !tTlbMissInh)
		tReqMissAddrE	= 1;
	if((tBlkMemDext2F != tReqAxH) && !tTlbMissInh)
		tReqMissAddrF	= 1;
`endif
	
//	if(tBlkMemDext2A==UV16_FF)
//		tReqMissAddrA	= 1;
//	if(tBlkMemDext2B==UV16_FF)
//		tReqMissAddrB	= 1;

`ifdef jx2_mem_l1d_utlb
	tUtlbBlkFlush	= (tUtlbBlkAddr[7:4] != tFlushRovTlb);
	tReqUtlbAxA		= { tUtlbBlkAddr[79:46], tReqAxA[9:0] };
	tReqUtlbAxB		= { tUtlbBlkAddr[79:46], tReqAxB[9:0] };
	tReqUtlbAxH		= tUtlbBlkAddr[95:80];
	tReqUtlbAccA	= tUtlbBlkAddr[3:0];
	tReqUtlbAccB	= tUtlbBlkAddr[3:0];

//	tReqUtlbHitHi	= tReqAxA[43:16] == tUtlbBlkAddr[43:16];
	tReqUtlbHitHi	=
		(tReqAxA[43:28] == tUtlbBlkAddr[43:28]) &&
		(tReqAxA[27:16] == tUtlbBlkAddr[27:16]) &&
		(tReqAxH == tReqUtlbAxH);
	if(tReqAxA[16] != tReqAxB[16])
		tReqUtlbHitHi = 0;
	tReqUtlbHitAxA	= tReqUtlbHitHi &&
		(tReqAxA[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && tRegInMmcr[0];
	tReqUtlbHitAxB	= tReqUtlbHitHi &&
		(tReqAxB[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && tRegInMmcr[0];

//	if(regInMmcr[5:4]==2'b00)
	if(tRegInMmcr[5:4]==2'b00)
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
// `ifdef def_true
	if(tRegInMmcr[0] && tReqIsNz && !tReqIsMmio && !tReqIsCcmd)
	begin
		if(tReqUtlbHitAxA)
			$display("Utlb Hit, %X %X", tUtlbBlkAddr, tReqAxA);
		else
			$display("Utlb Miss, %X %X", tUtlbBlkAddr, tReqAxA);
	end
`endif

`endif

//	tReqNoReadA		= tBlkMemAddr2A[0];
//	tReqNoReadB		= tBlkMemAddr2B[0];

//	tReqReadOnlyA	= tBlkMemAddr2A[1];
//	tReqReadOnlyB	= tBlkMemAddr2B[1];
	
	tReqNoReadA		= tBlkMemAddr2A[0] && !tBlkMemAddr2A[3];
	tReqNoReadB		= tBlkMemAddr2B[0] && !tBlkMemAddr2B[3];

	tReqReadOnlyA	= tBlkMemAddr2A[1] && !tBlkMemAddr2A[3];
	tReqReadOnlyB	= tBlkMemAddr2B[1] && !tBlkMemAddr2B[3];
	
	tWasMissInh		= 0;

	if(tTlbMissInh && !tReqIsMmio && !tReqIsCcmd)
	begin
		if(tBlkMemAddr2A[3:2] == 2'b11)
			tWasMissInh		= 1;
		if(tBlkMemAddr2B[3:2] == 2'b11)
			tWasMissInh		= 1;		
	end

	if(!tTlbMissInh && !tReqIsMmio && !tReqIsCcmd)
	begin
		if(tBlkMemAddr2A[3:2] == 2'b11)
//		if((tBlkMemAddr2A[3:2] == 2'b11) && tReqIsNz && !tReqMissSkipA &&
//			!tRegInSr[29])
		begin
			tReqFlushAddrA		= 1;
//			tReqWeakFlushAddrA	= 1;
			tBlkIsDirtyA		= 0;
		end

		if(tBlkMemAddr2B[3:2] == 2'b11)
//		if((tBlkMemAddr2B[3:2] == 2'b11) && tReqIsNz && !tReqMissSkipB &&
//			!tRegInSr[29])
		begin
			tReqFlushAddrB		= 1;
//			tReqWeakFlushAddrB	= 1;
			tBlkIsDirtyB		= 0;
		end

`ifndef def_true
		/* write through */
//		if(tVolatileInhCnt == 0)
		if((tVolatileInhCnt == 0) && !tReqIsNz)
		begin
			if(tBlkMemAddr2A[3:2] == 2'b10)
			begin
				tReqFlushAddrA	= 1;

				/* If Write-Through, Don't evict if not dirty. */
				if(tBlkIsWriteThruA && !tBlkIsDirtyA)
					tReqFlushAddrA	= 0;
			end
			if(tBlkMemAddr2B[3:2] == 2'b10)
			begin
				tReqFlushAddrB	= 1;
				
				/* If Write-Through, Don't evict if not dirty. */
				if(tBlkIsWriteThruB && !tBlkIsDirtyB)
					tReqFlushAddrB	= 0;
			end
		end
`endif
		
`ifndef def_true
// `ifdef def_true
		/* Auto Flush cache lines that are getting old. */

		if(((tBlkEpochDeltaA[1] && tBlkIsDirtyA) || tBlkEpochDeltaA[3]) && 
			!tReqIsNz && (tBlkMemDext2A != UV16_FF))
//		if(((tBlkEpochDeltaA[1] && tBlkIsDirtyA) || tBlkEpochDeltaA[3]) && 
//			(tBlkMemDext2A != UV16_FF))
		begin
			tReqFlushAddrA	= 1;
		end

		if(((tBlkEpochDeltaB[1] && tBlkIsDirtyB) || tBlkEpochDeltaB[3]) && 
			!tReqIsNz && (tBlkMemDext2B != UV16_FF))
//		if(((tBlkEpochDeltaB[1] && tBlkIsDirtyB) || tBlkEpochDeltaB[3]) && 
//			(tBlkMemDext2B != UV16_FF))
		begin
			tReqFlushAddrB	= 1;
		end
		
//		if(tReqIsNz && tBlkEpochDeltaA[3] && !tReqMissSkipA)
//			tReqFlushAddrA	= 1;
//		if(tReqIsNz && tBlkEpochDeltaB[3] && !tReqMissSkipB)
//			tReqFlushAddrB	= 1;
`endif
	end

	tRegOutExc[ 63:16] = tReqAddr[47:0];
	tRegOutExc[111:64] = tReqAddrHi[47:0];

	if(!tBlkMemAddr2A[3] && !tBlkMemAddr2B[3] &&
		!tReqMissAddrA && !tReqMissAddrB)
	begin
		if(	(tReqNoReadA && !tReqMissSkipA) ||
			(tReqNoReadB && !tReqMissSkipB) )
		begin
			if(tReqOpm[4])
			begin
				$display("L1 D$: Bad Read %X %X/%X",
					tReqAddr, tBlkMemAddr2A, tBlkMemAddr2B);
				tRegOutExc[15:0] = 16'h8001;
			end
		end

		if(	(tReqReadOnlyA && !tReqMissSkipA) ||
			(tReqReadOnlyB && !tReqMissSkipB) )
		begin
			if(tReqOpm[5])
			begin
				$display("L1 D$: Bad Write %X %X/%X",
					tReqAddr, tBlkMemAddr2A, tBlkMemAddr2B);
				tRegOutExc[15:0] = 16'h8002;
			end
		end
	end

//	tReqMissA	= (tReqMissAddrA && !tReqMissSkipA) || tReqFlushAddrA;
//	tReqMissB	= (tReqMissAddrB && !tReqMissSkipB) || tReqFlushAddrB;

//	tReqMissA	= (tReqMissAddrA && !tReqMissSkipA && tReqIsNz) || 
//		tReqFlushAddrA;
//	tReqMissB	= (tReqMissAddrB && !tReqMissSkipB && tReqIsNz) || 
//		tReqFlushAddrB;

	tReqMissA	= (tReqMissAddrA && (!tReqMissSkipA || tReqMissAddrB)) || 
		tReqFlushAddrA;
	tReqMissB	= (tReqMissAddrB && (!tReqMissSkipB || tReqMissAddrA)) || 
		tReqFlushAddrB;

	tReqMissE	= 0;
	tReqMissF	= 0;

`ifdef jx2_mem_lane2
	tReqMissE	= (tReqMissAddrE && !tReqMissSkipE) || tReqFlushAddrE;
	tReqMissF	= (tReqMissAddrF && !tReqMissSkipF) || tReqFlushAddrF;
`endif

//	tReqMissA	= ((tReqMissAddrA || tReqWeakFlushAddrA) &&
//		!tReqMissSkipA) || tReqFlushAddrA;
//	tReqMissB	= ((tReqMissAddrB || tReqWeakFlushAddrB) &&
//		!tReqMissSkipB) || tReqFlushAddrB;

//	tReqMiss	= (tReqMissA || tReqMissB) && tReqIsNz;
	tReqMiss	= (tReqMissA || tReqMissB) &&
		(tReqIsNz || tReqFlushAddrA || tReqFlushAddrB) &&
		!(tReqIsMmio || tReqIsCcmd);

`ifdef jx2_mem_lane2
	tReqMiss	=
		(tReqMissA || tReqMissB ||
			((tReqMissE || tReqMissF) && tReqIsNzB)) &&
		(tReqIsNz || tReqIsNzB ||
			tReqFlushAddrA || tReqFlushAddrB || 
			tReqFlushAddrE || tReqFlushAddrF) &&
		!(tReqIsMmio || tReqIsCcmd);
`endif

//	if(tReqMiss)
//		tRegOutExc[15] = 0;

//	if(!tReqIsNz)
//	begin
//		tReqMissA	= 0;
//		tReqMissB	= 0;
//	end

//	if(tReqMiss && !tReqIsMmio && !tReqIsCcmd)
//		tRegOutHold = 1;

//	if(!tReqReady && !tReqIsMmio && !tReqIsCcmd)
//		tRegOutHold = 1;

	if(((tReqMiss || !tReqReady) && !tReqIsMmio && !tReqIsCcmd) ||
		((tReqIsMmio || tReqIsCcmd) && (!tMemMmioReady)))
			tRegOutHold = 1;

	/* Hold until EXC forwards. */
//	if(tRegOutExc[15] && !tRegOutExc3[15])
//	if(tRegOutExc[15] && !tRegOutExc2[15])
//		tRegOutHold = 1;

//	tReqDoMissA	= tReqMissA;
//	tReqDoMissB	= tReqMissB;
	tReqDoMissA	= (tReqMissA && tReqIsNz) || tReqFlushAddrA;
	tReqDoMissB	= (tReqMissB && tReqIsNz) || tReqFlushAddrB;

`ifdef jx2_mem_lane2
	tReqDoMissA	=
		(tReqMissA && tReqIsNz) ||
		(tReqMissE && tReqIsNzB) ||
		tReqFlushAddrA || tReqFlushAddrE;
	tReqDoMissB	=
		(tReqMissB && tReqIsNz) ||
		(tReqMissF && tReqIsNzB) ||
		tReqFlushAddrB || tReqFlushAddrF;

	tReqDoMissE =
		((tReqMissE && tReqIsNzB) || tReqFlushAddrE) &&
		!((tReqMissA && tReqIsNz) || tReqFlushAddrA) &&
		!tBlkIsDirtyA;
	tReqDoMissF =
		((tReqMissF && tReqIsNzB) || tReqFlushAddrF) &&
		!((tReqMissB && tReqIsNz) || tReqFlushAddrB) &&
		!tBlkIsDirtyB;
`endif

// `ifdef def_true
`ifndef def_true
	if(tReqIsMmio || tReqIsCcmd)
	begin
		tReqDoMissA	= 0;
		tReqDoMissB	= 0;
	end

	if(!tReqReady)
//	if(tReqIsMmio || tReqIsCcmd || !tReqReady ||
//		tReg2StoreFwA || tMemArrFwA)
	begin
//		if(!tReqDoMissAL)
		if(!(tMemReqStA || tMemReqLdA))
			tReqDoMissA	= 0;
//		if(!tReqDoMissBL)
		if(!(tMemReqStB || tMemReqLdB))
			tReqDoMissB	= 0;
	end
`endif
	

`ifndef def_true
	if(tReqIsMmio || tReqIsCcmd || !tReqReady)
//	if(tReqIsMmio || tReqIsCcmd || !tReqReady ||
//		tReg2StoreFwA || tMemArrFwA)
	begin
//		if(!tReqDoMissAL)
		if(!(tMemReqStA || tMemReqLdA))
			tReqDoMissA	= 0;
//		if(!tReqDoMissBL)
		if(!(tMemReqStB || tMemReqLdB))
			tReqDoMissB	= 0;
	end
`endif
	
// `ifdef def_true
`ifndef def_true

//	if(tReqMiss || (tReqMissNoSkipL && tRegOutHoldL))
//	if((tReqMiss && tReqReady) || (tReqMissNoSkipL && tRegOutHoldL))
	if((tReqMiss && tReqReady && !tReqMissL) ||
//	if(((tReqMissA && tReqIsNz) && tReqReady && !tReqMissL) ||
		(tReqMissNoSkipL && tRegOutHoldL))
//		(tReqMissNoSkipL && tRegOutHoldL && tReqReady))
//	if(1'b1)
	begin
//		tReqMissSkipA	= 0;
//		tReqMissSkipB	= 0;
		tReqMissNoSkip	= 1;

//		tReqDoMissA	= (tReqMissAddrA || tReqFlushAddrA) && tReqIsNz;
//		tReqDoMissB	= (tReqMissAddrB || tReqFlushAddrB) && tReqIsNz;

		tReqDoMissA	= (tReqMissAddrA && tReqIsNz) || tReqFlushAddrA;
		tReqDoMissB	= (tReqMissAddrB && tReqIsNz) || tReqFlushAddrB;

`ifdef jx2_mem_lane2
		tReqDoMissA	=
			(tReqMissAddrA && tReqIsNz) ||
			(tReqMissAddrE && tReqIsNzB) ||
			tReqFlushAddrA || tReqFlushAddrE;
		tReqDoMissB	=
			(tReqMissAddrB && tReqIsNz) ||
			(tReqMissAddrF && tReqIsNzB) ||
			tReqFlushAddrB || tReqFlushAddrF;

		tReqDoMissE =
			((tReqMissAddrE && tReqIsNzB) || tReqFlushAddrE) &&
			!((tReqMissAddrA && tReqIsNz) || tReqFlushAddrA) &&
			!tBlkIsDirtyA;
		tReqDoMissF =
			((tReqMissAddrF && tReqIsNzB) || tReqFlushAddrF) &&
			!((tReqMissAddrB && tReqIsNz) || tReqFlushAddrB) &&
			!tBlkIsDirtyA;
`endif

//		tReqMiss	= tReqMissA || tReqMissB;
//		tReqMiss	= tReqMissA || tReqMissB;
	end
`endif

`ifdef def_true
// `ifndef def_true
	if(tReqIsMmio || tReqIsCcmd)
	begin
		tReqDoMissA	= 0;
		tReqDoMissB	= 0;
	end

	if(!tReqReady)
//	if(tReqIsMmio || tReqIsCcmd || !tReqReady ||
//		tReg2StoreFwA || tMemArrFwA)
	begin
//		if(!tReqDoMissAL)
		if(!(tMemReqStA || tMemReqLdA))
			tReqDoMissA	= 0;
//		if(!tReqDoMissBL)
		if(!(tMemReqStB || tMemReqLdB))
			tReqDoMissB	= 0;
	end
`endif

//	if(!tReqReady && tReqReadyL && tRegOutHoldL)
//		$display("L1D$: Loss of Ready");

	if(tReqIsNz && !tReqIsMmio)
	begin
		if(!tReqReady)
		begin
//			$display("L1 D$: Not Ready Stall");
//			tRegOutHold = 1;
		end

		if(tReqMiss)
		begin
//			$display("L1 D$: Miss Stall");
//			tRegOutHold = 1;
		end

`ifndef def_true
		if(	(tReg2MissInterlockA && !tReg2StoreFwA) ||
			(tReg2MissInterlockB && !tReg2StoreFwB)	)
		begin
//			$display("L1 D$: Interlock Stall");
//			tRegOutHold = 1;
		end
`endif
	end
	
	tReqMissIxA		= tReqIxA;
	tReqMissIxB		= tReqIxB;
	tReqMissAxA		= tReqAxA;
	tReqMissAxB		= tReqAxB;
	
`ifdef jx2_mem_lane2
	if(tReqDoMissE)
	begin
		tReqMissIxA[5:0]	= tReqIxE;
		tReqMissAxA			= tReqAxE;
		tReqUtlbHitAxA		= 0;
		tReqDoPfxA			= 0;
		tReqDoSpxA			= 0;
	end

	if(tReqDoMissF)
	begin
		tReqMissIxB[5:0]	= tReqIxF;
		tReqMissAxB			= tReqAxF;
		tReqUtlbHitAxB		= 0;
		tReqDoPfxB			= 0;
		tReqDoSpxB			= 0;
	end
`endif

	if(tReqBix[4])
	begin
		tBlkExData0 = { tBlkMemData2A, tBlkMemData2B };
	end	else begin
		tBlkExData0 = { tBlkMemData2B, tBlkMemData2A };
	end

`ifdef jx2_mem_misal_movx
	tBlkExData1 = tReqBix[3] ? tBlkExData0[255:64] : tBlkExData0[191: 0];
	tBlkExData2 = tReqBix[2] ? tBlkExData1[183:32] : tBlkExData1[151: 0];
	tBlkExData3 = tReqBix[1] ? tBlkExData2[151:16] : tBlkExData2[135: 0];
	tBlkExData4 = tReqBix[0] ? tBlkExData3[135: 8] : tBlkExData3[127: 0];
`else
	tBlkExData1 = tReqBix[3] ? tBlkExData0[191:64] : tBlkExData0[127: 0];
	tBlkExData2 = tReqBix[2] ? tBlkExData1[119:32] : tBlkExData1[ 87: 0];
	tBlkExData3 = tReqBix[1] ? tBlkExData2[ 87:16] : tBlkExData2[ 71: 0];
	tBlkExData4 = tReqBix[0] ? tBlkExData3[ 71: 8] : tBlkExData3[ 63: 0];
`endif

	tRegOutValFast = 0;

`ifdef jx2_l1a_ena_loadfast
	tRegOutValFast[63:0] = tBlkExData2[63:0];
	if(tReqOpm[1:0] != 2'b11)
	begin
		tRegOutValFast[63:32] = (tBlkExData2[31] && !tReqOpm[2]) ?
			UV32_FF : UV32_00;
	end
	tRegOutValFast[64] = tReqOpm[1] && (tReqOpm[2:0]!=3'b111) &&
		(tReqBix[1:0]==2'b00);
	tRegOutValFast[65] = (tReqIsNz && !tReqIsMmio);

	if(	tReg2StoreFwA || tMemArrFwA ||
		tReg2StoreFwB || tMemArrFwB )
	begin
		/* Disable fast-case if store-forwarding. */
		tRegOutValFast[63:0] = UV64_XX;
		tRegOutValFast[65:64] = 0;
	end
`endif

`ifdef jx2_mem_lane2
	if(tReqBixB[4])
	begin
		tBlkExData0B = { tBlkMemData2E, tBlkMemData2F };
	end	else begin
		tBlkExData0B = { tBlkMemData2F, tBlkMemData2E };
	end

	tBlkExData1B = tReqBixB[3] ? tBlkExData0B[191:64] : tBlkExData0B[127: 0];
	tBlkExData2B = tReqBixB[2] ? tBlkExData1B[119:32] : tBlkExData1B[ 87: 0];
	tBlkExData3B = tReqBixB[1] ? tBlkExData2B[ 87:16] : tBlkExData2B[ 71: 0];
	tBlkExData4B = tReqBixB[0] ? tBlkExData3B[ 71: 8] : tBlkExData3B[ 63: 0];
`endif

// `ifdef def_true
`ifndef def_true
	if(tWasMissInh)		//BGB: Debug
	begin
		tBlkExData4[15:0] = 16'hAAAA;
	end
`endif

	casez(tReqOpm[2:0])
		3'b000: tReqSx = tBlkExData4[7];
		3'b001: tReqSx = tBlkExData4[15];
		3'b010: tReqSx = tBlkExData4[31];
		3'b011: tReqSx = 0;
		3'b1zz: tReqSx = 0;
	endcase

`ifdef jx2_mem_misal_movx
	tBlkExData = tBlkExData4[63:0];
`else
	tBlkExData = tBlkExData4;
`endif

	if(tReqOpm[1:0]==2'b00)
		tBlkExData[15:8] = tReqSx ? UV8_FF : UV8_00;
	if(tReqOpm[1]==1'b0)
		tBlkExData[31:16] = tReqSx ? UV16_FF : UV16_00;
	if(tReqOpm[1:0]!=2'b11)
		tBlkExData[63:32] = tReqSx ? UV32_FF : UV32_00;

`ifdef jx2_mem_lane2
	casez(tReqOpmB[2:0])
		3'b000: tReqSxB = tBlkExData4B[7];
		3'b001: tReqSxB = tBlkExData4B[15];
		3'b010: tReqSxB = tBlkExData4B[31];
		3'b011: tReqSxB = 0;
		3'b1zz: tReqSxB = 0;
	endcase

	tBlkExDataB = tBlkExData4B;
	if(tReqOpmB[1:0]==2'b00)
		tBlkExDataB[15:8] = tReqSxB ? UV8_FF : UV8_00;
	if(tReqOpmB[1]==1'b0)
		tBlkExDataB[31:16] = tReqSxB ? UV16_FF : UV16_00;
	if(tReqOpmB[1:0]!=2'b11)
		tBlkExDataB[63:32] = tReqSxB ? UV32_FF : UV32_00;
`endif

	tBlkExDataA = tBlkExData;

`ifndef jx2_mem_lane2
`ifdef jx2_mem_misal_movx
	tBlkExDataB = tBlkExData4[127:64];
`else
	tBlkExDataB = tBlkExData1[127:64];
`endif
`endif

	if(tReqOpm[2:0]==3'b111)
	begin
`ifdef jx2_mem_misal_movx
		tBlkExDataA = tBlkExData4[ 63: 0];
`ifdef jx2_mem_lane2
		tBlkExDataB = tBlkExData4[127:64];
`endif
`else
		tBlkExDataA = tBlkExData1[ 63: 0];
`ifdef jx2_mem_lane2
		tBlkExDataB = tBlkExData1[127:64];
`endif
`endif
	end

`ifndef def_true
	if(tReqNoReadA || tReqNoReadB)
	begin
		tBlkExDataA = UV64_00;
		tBlkExDataB = UV64_00;
	end
`endif

`ifdef jx2_mem_misal_movx
	tBlkInsData4 = { tReqInValB, tReqInValA };
`else
	tBlkInsData4 = tReqInValA;
`endif

`ifdef jx2_use_mem_ldop
	if(!noLdOp)
	begin
		case(tReqLdOp[2:0])
			3'b000: begin
				/* Normal Load/Store */
			end
			3'b001: begin
				/* Exchange */
			end
			3'b010: begin
				/* ADD */
				tBlkInsData4[31:0] = tBlkExData[31:0] + tReqInValA[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end
			3'b011: begin
				/* SUB */
				tBlkInsData4[31:0] = tBlkExData[31:0] - tReqInValA[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end
			3'b100: begin
				/* SUB */
				tBlkInsData4[31:0] = tReqInValA[31:0] - tBlkExData[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end

			3'b101: begin
				/* AND */
				tBlkInsData4[31:0] = tBlkExData[31:0] & tReqInValA[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end
			3'b110: begin
				/* OR */
				tBlkInsData4[31:0] = tBlkExData[31:0] | tReqInValA[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end
			3'b111: begin
				/* XOR */
				tBlkInsData4[31:0] = tBlkExData[31:0] ^ tReqInValA[31:0];
				tBlkExData[31:0] = tBlkInsData4[31:0];
			end
		endcase
	end
`endif

`ifdef jx2_mem_misal_movx
	tBlkInsData4 = {
		(tReqOpm[2:0]!=3'b111) ? tBlkExData4[127:64] : tBlkInsData4[127:64],
		(tReqOpm[1:0]!=2'b11 ) ? tBlkExData4[ 63:32] : tBlkInsData4[ 63:32],
		(tReqOpm[1  ]==1'b0  ) ? tBlkExData4[ 31:16] : tBlkInsData4[ 31:16],
		(tReqOpm[1:0]==2'b00 ) ? tBlkExData4[ 15: 8] : tBlkInsData4[ 15: 8],
		tBlkInsData4[7:0]
	};
`else
	tBlkInsData4 = {
		(tReqOpm[1:0]!=2'b11) ? tBlkExData4[63:32] : tBlkInsData4[63:32],
		(tReqOpm[1  ]==1'b0 ) ? tBlkExData4[31:16] : tBlkInsData4[31:16],
		(tReqOpm[1:0]==2'b00) ? tBlkExData4[15: 8] : tBlkInsData4[15: 8],
		tBlkInsData4[7:0]
	};
`endif
	
`ifdef jx2_enable_ldst48a
	if(tReqOpm[2:0]==3'b110)
	begin
`ifdef jx2_mem_misal_movx
		tBlkInsData4 = { tBlkExData4[127:48], tBlkInsData4[47:0] };
`else
		tBlkInsData4 = { tBlkExData4[ 63:48], tBlkInsData4[47:0] };
`endif
	end
`endif

`ifdef jx2_mem_misal_movx

	tBlkInsData3 = tReqBix[0] ?
		{ tBlkInsData4[127:  0], tBlkExData3 [  7:0] } :
		{ tBlkExData3 [135:128], tBlkInsData4[127:0] } ;
	tBlkInsData2 = tReqBix[1] ?
		{ tBlkInsData3[135:  0], tBlkExData2 [ 15:0] } :
		{ tBlkExData2 [151:136], tBlkInsData3[135:0] } ;

	tBlkInsData1 = { 
		tBlkExData1 [191:184], 
		tReqBix[2] ?
			{ tBlkInsData2[151: 0], tBlkExData1 [ 31:0] } :
			{ tBlkExData1 [183:152], tBlkInsData2[151:0] } 
		};

	tBlkInsData0 = {
		tReqBix[3] ? tBlkInsData1[191:128] : tBlkExData0 [255:192],
		tReqBix[3] ? tBlkInsData1[127: 64] : tBlkInsData1[191:128],
		tReqBix[3] ? tBlkInsData1[ 63:  0] : tBlkInsData1[127: 64],
		tReqBix[3] ? tBlkExData0 [ 63:  0] : tBlkInsData1[ 63:  0]	};

`else

	tBlkInsData3 = tReqBix[0] ?
		{ tBlkInsData4[63: 0], tBlkExData3 [ 7:0] } :
		{ tBlkExData3 [71:64], tBlkInsData4[63:0] } ;
	tBlkInsData2 = tReqBix[1] ?
		{ tBlkInsData3[71: 0], tBlkExData2 [15:0] } :
		{ tBlkExData2 [87:72], tBlkInsData3[71:0] } ;
//	tBlkInsData1 = tReqBix[2] ?
//		{ tBlkExData1 [127:120], tBlkInsData2[ 87: 0], tBlkExData1 [31:0] } :
//		{ tBlkExData1 [127:120], tBlkExData1 [119:88], tBlkInsData2[87:0] } ;

	tBlkInsData1 = { 
		tBlkExData1 [127:120], 
		tReqBix[2] ?
			{ tBlkInsData2[ 87: 0], tBlkExData1 [31:0] } :
			{ tBlkExData1 [119:88], tBlkInsData2[87:0] } 
		};

	if(tReqOpm[2:0]==3'b111)
	begin
		tBlkInsData1 = { tReqInValB, tReqInValA };
	end

//	tBlkInsData0 = tReqBix[3] ?
//		{ tBlkExData0 [255:192], tBlkInsData1[127:  0], tBlkExData0 [ 63:0] } :
//		{ tBlkExData0 [255:192], tBlkExData0 [191:128], tBlkInsData1[127:0] } ;

	tBlkInsData0 = {
		tBlkExData0 [255:192],
		tReqBix[3] ? tBlkInsData1[127: 64] : tBlkExData0 [191:128],
		tReqBix[3] ? tBlkInsData1[ 63:  0] : tBlkInsData1[127: 64],
		tReqBix[3] ? tBlkExData0 [ 63:  0] : tBlkInsData1[ 63:  0]	};

`endif

`ifdef jx2_debug_ldst
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
//			tRegOutHold = 1;
		end

		tBlkExDataA = tMemMmioData;

		if(tReqOpm[1:0]!=2'b11)
		begin
			if(tMemMmioData[31] && !tReqOpm[2])
				tBlkExDataA[63:32]=UV32_FF;
			else
				tBlkExDataA[63:32]=UV32_00;
		end
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
		if(tReqSeqIdx==tReqMissIxA)
//		if(1'b1)
		begin
`ifdef jx2_debug_l1ds
			$display("L1D$: Store Response A, A=%X, Ix1/Ix2=%X/%X", 
				memAddrIn, tReqSeqIdx, tReqIxA);
`endif
			tNxtMemRespStA = 1;
		end
		else
		begin
`ifdef def_true
			$display("L1D$: Bad Store Resp A, A=%X Ix:%X!=%X Rov=%X", 
				memAddrIn, tReqSeqIdx, tReqIxA, tMemSeqRov);
`endif
		end
	end
	
	if(memRingIsRespOkStB)
	begin
		if(tReqSeqIdx==tReqMissIxB)
//		if(1'b1)
		begin
`ifdef jx2_debug_l1ds
			$display("L1D$: Store Resp B, A=%X, Ix1/Ix2=%X/%X Rov=%X", 
				memAddrIn, tReqSeqIdx, tReqIxB, tMemSeqRov);
`endif
			tNxtMemRespStB = 1;
		end
		else
		begin
`ifdef def_true
			$display("L1D$: Bad Store Resp B, A=%X Ix:%X!=%X Rov=%X", 
				memAddrIn, tReqSeqIdx, tReqIxB, tMemSeqRov);
`endif
		end
	end
	
	if(memRingIsRespOkLdA && !tMemRingSkipResp && !tResetL)
	begin
`ifdef jx2_debug_l1ds
		$display("L1D$: Load Response A, A=%X, Ix1/Ix2=%X/%X", 
				memAddrIn, tReqSeqIdx, tReqIxA);
`endif

		if(memAddrIn[31:5]==0)
		begin
			if(tReqAxH!=UV16_FF)
			begin
				$display("L1D$: Load Null, Non-FF");
				$display("  A=%X AH=%X O=%X", tReqSeqVa, tReqAxH, tReqOpm);
			end
		end

		if(tReqSeqIdx==tReqMissIxA)
//		if(1'b1)
		begin
			tArrMemAddrStA = {
				tFlushRov[3:0],
				memAddrIn[31:12],
				tReqSeqVa[43:1], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStA = memDataIn;
			tArrMemIdxStA = tReqSeqIdx;
//			tArrMemIdxStA = tReqIxA;
			tArrMemDextStA	= tReqAxH;
//			tArrMemAextStA	= { 16'h0, tSyncEpoch[7:6], tSyncEpoch[3:2] };
			tArrMemAextStA	= { 8'h0, memOpmIn[15:8],
				tSyncEpoch[7:6], tSyncEpoch[3:2] };
			tArrMemDoStA = 1;
			tNxtMemRespLdA = 1;

			tUtlbStAddr		= {
				tReqAxH,
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
				memOpmIn[3:0] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);

//			if(tVolatileIxA == tReqMissIxA)
			if(tVolatileIxA == tReqSeqIdx)
				tNxtVolatileIxA = 0;

			if(memOpmIn[3])
			begin
				if(memOpmIn[2])
				begin
`ifdef jx2_debug_isr
					$display("L1D$ Set TLB Inhibit A");
`endif
					tNxtTlbMissInh2 = 1;
				end
				else
				begin
					tNxtVolatileInh		= 5;
//					tNxtVolatileIxA		= tReqMissIxA;
					tNxtVolatileIxA		= tReqSeqIdx;
				end
			end
		end
		
		if(memAddrIn[4])
			$display("L1D$: Load Even/Odd Mismatch A");
		
`ifdef jx2_debug_isr
		if(tReqSeqIdx!=tReqMissIxA)
			$display("L1D$: In!=Req IxA, %X %X",
				tReqSeqIdx, tReqMissIxA);
`endif
`ifdef jx2_debug_isr
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1D$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
`endif
`ifdef jx2_debug_isr
		if(tReqSeqVa[43:1]!=tReqMissAxA[43:1])
			$display("L1D$: In!=Req A, %X %X",
				tReqMissAxA[43:1], tReqSeqVa[43:0]);
`endif
	end

	if(memRingIsRespOkLdB && !tMemRingSkipResp && !tResetL)
	begin
`ifdef jx2_debug_l1ds
		$display("L1D$: Load Response B, A=%X, Ix1/Ix2=%X/%X", 
				memAddrIn, tReqSeqIdx, tReqIxB);
`endif

		if(memAddrIn[31:5]==0)
		begin
			if(tReqAxH!=UV16_FF)
			begin
				$display("L1D$: Load Null, Non-FF");
				$display("  A=%X AH=%X O=%X", tReqSeqVa, tReqAxH, tReqOpm);
			end
		end

		if(tReqSeqIdx==tReqMissIxB)
//		if(1'b1)
		begin
			tArrMemAddrStB = {
				tFlushRov[3:0],
				memAddrIn[31:12],
				tReqSeqVa[43:1], 1'b0,
				memOpmIn[3:0]};
			tArrMemDataStB = memDataIn;
			tArrMemIdxStB = tReqSeqIdx;
//			tArrMemIdxStB = tReqIxB;
			tArrMemDextStB	= tReqAxH;
//			tArrMemAextStB	= { 16'h0, tSyncEpoch[7:6], tSyncEpoch[3:2] };
			tArrMemAextStB	= { 8'h0, memOpmIn[15:8],
				tSyncEpoch[7:6], tSyncEpoch[3:2] };
			tArrMemDoStB	= 1;
			tNxtMemRespLdB	= 1;

			tUtlbStAddr		= {
				tReqAxH,
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
				memOpmIn[3:0] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);

//			if(tVolatileIxB == tReqMissIxB)
			if(tVolatileIxB == tReqSeqIdx)
				tNxtVolatileIxB = 0;

			if(memOpmIn[3])
			begin
				if(memOpmIn[2])
				begin
`ifdef jx2_debug_isr
					$display("L1D$ Set TLB Inhibit B");
`endif
					tNxtTlbMissInh2 = 1;
				end
				else
				begin
					tNxtVolatileInh		= 5;
//					tNxtVolatileIxB		= tReqMissIxB;
					tNxtVolatileIxB		= tReqSeqIdx;
				end
			end
		end

		if(!memAddrIn[4])
			$display("L1D$: Load Even/Odd Mismatch B");

`ifdef jx2_debug_isr
		if(tReqSeqIdx!=tReqMissIxB)
			$display("L1D$: In!=Req IxB, %X %X",
				tReqSeqIdx, tReqMissIxB);
`endif
`ifdef jx2_debug_isr
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1D$: Virt!=Phys B, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
`endif
`ifdef jx2_debug_isr
		if(tReqSeqVa[43:1]!=tReqMissAxB[43:1])
			$display("L1D$: In!=Req B, %X %X",
				tReqMissAxB[43:1], tReqSeqVa[43:0]);
`endif
	end

`ifdef def_true
	tReqWaitResp = 0;
	if(tMemReqLdA && !tMemRespLdA)
//	if(tMemReqLdA && !tNxtMemRespLdA)
		tReqWaitResp = 1;
	if(tMemReqLdB && !tMemRespLdB)
//	if(tMemReqLdB && !tNxtMemRespLdB)
		tReqWaitResp = 1;
	if(tMemReqStA && !tMemRespStA)
//	if(tMemReqStA && !tNxtMemRespStA)
		tReqWaitResp = 1;
	if(tMemReqStB && !tMemRespStB)
//	if(tMemReqStB && !tNxtMemRespStB)
		tReqWaitResp = 1;

	if(tReqWaitResp)
		tRegOutHold = 1;
`endif

`ifdef jx2_mem_lane2
	/* If we need something for Lanes E/F,
	 * But already handled A/B, clear response status.
	 * This allows E/F requests to be sent.
	 */
	if(tReqDoMissE && tMemRespLdA && !tArrMemDidStA)
		tNxtMemRespLdA	= 0;
	if(tReqDoMissF && tMemRespLdB && !tArrMemDidStB)
		tNxtMemRespLdA	= 0;
`endif

	if(memRingIsRespOkMmio && (tReqIsMmio || tReqIsCcmd))
	begin
//		$display("L1 D$: MMIO Response Seen");
		tNxtMemMmioData = memDataIn[63:0];
		tNxtMemMmioReady = 1;
	end

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
			tArrMemAddrStA	= tBlk2StoreAddrA;
			tArrMemDataStA	= tBlk2StoreDataA;
			tArrMemDextStA	= tBlk2StoreDextA;
			tArrMemAextStA	= tBlk2StoreAextA;
			tArrMemIdxStA	= tReq2IxA;

			tArrMemAddrStB	= tBlk2StoreAddrB;
			tArrMemDataStB	= tBlk2StoreDataB;
			tArrMemDextStB	= tBlk2StoreDextB;
			tArrMemAextStB	= tBlk2StoreAextB;
			tArrMemIdxStB	= tReq2IxB;

			tNxtReq2StoreSticky = 1;

//			tArrMemDoStA = !tReq2MissSkipA && !tReq2ReadOnlyA;
//			tArrMemDoStB = !tReq2MissSkipB && !tReq2ReadOnlyB;
			tArrMemDoStA = !tReq2MissSkipA;
			tArrMemDoStB = !tReq2MissSkipB;

			if(tBlk2StoreAddrA[3:2]==2'b11)
				tArrMemDoStA = 0;
			if(tBlk2StoreAddrB[3:2]==2'b11)
				tArrMemDoStB = 0;

			if((tBlk2MemAddrA[31:5]!=tReq2AxA[27:1]) && !tReq2MissSkipA)
			begin
				$display("L1 D$: Store Addr Mismatch A");
			end
			if((tBlk2MemAddrB[31:5]!=tReq2AxB[27:1]) && !tReq2MissSkipB)
			begin
				$display("L1 D$: Store Addr Mismatch B");
			end

`ifdef def_true
			if((tArrMemDataStA[127:0]!=tBlk2MemDataA[127:0]) && 
					tReq2MissSkipA)
				$display("L1 D$: Store Data Mismatch A");
			if((tArrMemDataStB[127:0]!=tBlk2MemDataB[127:0]) && 
					tReq2MissSkipB)
				$display("L1 D$: Store Data Mismatch B");
`endif

// `ifndef def_true
`ifdef def_true
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

	if(tResetL)
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
`ifdef jx2_debug_isr
		$display("L1D$: Store Without Load A");
`endif
		tReqDoMissA = 1;
	end
	if(tMemReqStB && !tMemReqLdB && !tReqDoMissB)
	begin
`ifdef jx2_debug_isr
		$display("L1D$: Store Without Load B");
`endif
		tReqDoMissB = 1;
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

	if(tResetL)
	begin
		tNxtMemSeqRov	= 0;
	end
	else
		if(tReqIsMmio || tReqIsCcmd)
	begin
		tNxtMemReqLdM	= tMemReqLdM;
		tRegOutWait		= 1;
//		tRegOutHold		= 1;

		if(!tMemReqLdM)
		begin
//			$display("L1 D$ MMIO Req A=%X", tReqAddr);
		
`ifdef jx2_debug_isr
//			if(tReqAddr[27:16]==12'h001)
			if(tReqAddr[31:28]!=4'hF)
			begin
				$display("L1 D$ MMIO Req Dbg A=%X IsMMIO=%d IsCCMD=%d", 
					tReqAddr, tReqIsMmio, tReqIsCcmd);
			end
`endif
		
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };
//			tMemDataReq		= { UV64_00, tReqInValA };
			tMemDataReq		= { tReqInValB, tReqInValA };
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

		tMemSeqIx		= tMemSeqIxL;
		tMemSeqVa		= tMemSeqVaL;

//		tReqAliasA		= tBlkMemAddr2A[23: 8] == tRegAxA[19:4];
//		tReqAliasB		= tBlkMemAddr2B[23: 8] == tRegAxB[19:4];

		tRegOutWait		= 1;
//		tRegOutHold		= 1;

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
`ifdef jx2_debug_l1ds
			$display("L1 D$: Send STA Req, A=%X Ix=%X Rov=%X",
				tBlkMemAddr2A[47:4], tReqIxA, tMemSeqRov);
`endif
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqMissIxA;
			tMemSeqVa		= tReqMissAxA;
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
`ifdef jx2_debug_l1ds
			$display("L1 D$: Send STB Req, A=%X Ix=%X Rov=%X",
				tBlkMemAddr2B[47:4], tReqIxB, tMemSeqRov);
`endif
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqMissIxB;
			tMemSeqVa		= tReqMissAxB;
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
//			if(tReqDoMissA && !tMemReqLdA)
			if(tReqDoMissA && !tMemReqLdA &&
				!(tMemReqStA && !tMemRespStA && !tReqMissAddrLoA))
//			if(tReqDoMissA && !tMemReqLdA &&
//				!(tMemReqStA && !tMemRespStA))
		begin
`ifdef jx2_debug_l1ds
			$display("L1 D$: Send LDA Req A=%X", tReqAxA);
`endif
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqMissIxA;
			tMemSeqVa		= tReqMissAxA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2A;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqMissAxA, 4'h00 };
`else
			tMemAddrReq		= { tReqMissAxA, 4'h00 };
`endif

//			if(tReqMissAxA[43:40]==4'hD)
			if((tReqMissAxA[43:40]==4'hD) && !tReqAddrHiIsNz)
				tMemOpmReq[11]=1;

			if(tSkipTlb)
			begin
				/* FIXME: This is ugly hacks. */
				/* If in an ISR, force physical addressing.
				 * Unless address is in virtual ranges,
				 * then just have it behave
				 * as if TLB lookup has failed.
				 */
			

//				if(tReqAxA[43:24]!=0)
				if(tReqAddrIsVirt)
				begin
`ifdef jx2_debug_isr
					$display("L1 D$: Send LDA Req Abs A=%X", tReqAxA);
`endif
				end
//				else

//				if(!tReqAddrIsVirt)
				tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;
		
				if(tReqAddrIsVirt)
				begin
					tMemOpmReq[11:8] = 4'hF;
//					tRegOutHold = 1;
				end

			end

// `ifndef def_true
`ifdef def_true
			if(tReqDoPfxA)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
			if(tReqDoSpxA)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_SPX };
`endif

`ifdef jx2_mem_l1d_utlb
			if(tReqUtlbHitAxA && !tSkipTlb)
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
//			if(tReqDoMissB && !tMemReqLdB)
			if(tReqDoMissB && !tMemReqLdB &&
				!(tMemReqStB && !tMemRespStB && !tReqMissAddrLoB))
//			if(tReqDoMissB && !tMemReqLdB &&
//				!(tMemReqStB && !tMemRespStB))
		begin
`ifdef jx2_debug_l1ds
			$display("L1 D$: Send LDB Req A=%X", tReqAxB);
`endif
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqMissIxB;
			tMemSeqVa		= tReqMissAxB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov };
//			tMemDataReq		= tBlkMemData2B;
			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqMissAxB, 4'h00 };
`else
			tMemAddrReq		= { tReqMissAxB, 4'h00 };
`endif

//			if(tReqMissAxB[43:40]==4'hD)
			if((tReqMissAxB[43:40]==4'hD) && !tReqAddrHiIsNz)
				tMemOpmReq[11]=1;

			if(tSkipTlb)
			begin
				/* FIXME: This is ugly hacks. */

//				if(tReqAxB[43:24]!=0)
				if(tReqAddrIsVirt)
				begin
`ifdef jx2_debug_isr
					$display("L1 D$: Send LDB Req Abs A=%X", tReqAxB);
`endif
				end
//				else


//				if(!tReqAddrIsVirt)
					tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;

				if(tReqAddrIsVirt)
				begin
					tMemOpmReq[11:8] = 4'hF;
//					tRegOutHold = 1;
				end
			end

// `ifndef def_true
`ifdef def_true
			if(tReqDoPfxB)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_PFX };
			if(tReqDoSpxB)
				tMemOpmReq		= { UV8_00, JX2_RBI_OPM_SPX };
`endif

`ifdef jx2_mem_l1d_utlb
			if(tReqUtlbHitAxB && !tSkipTlb)
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
//			tRegOutHold = 1;
		end
`endif
	end

	if((tReqDoMissA || tReqDoMissB) && !tRegOutHold &&
		!(tReqIsMmio || tReqIsCcmd))
	begin
`ifdef jx2_debug_isr
		$display("DoMiss but no Hold %d %d", tReqDoMissA, tReqDoMissB);
`endif
//		tRegOutHold = 1;
	end

	if(tResetL)
	begin
		tRegOutHold = 0;
	end

end

always @(posedge clock)
begin
	if(tRegOutHold && dcInHoldN)
		$display("L1D$: Hold Signal Fault");
end

always @(posedge clock)
begin
//	tTlbMissInh		<= tNxtTlbMissInh;
//	tTlbMissInh		<= tNxtTlbMissInh || tNxtTlbMissInh2;
	tTlbMissInh		<= (dcInHold ? tTlbMissInh : 0) ||
		tNxtTlbMissInh || tNxtTlbMissInh2;
//	tSkipTlb		<= tNxtSkipTlb;

//	tVolatileInh	<= tNxtVolatileInh;
	tVolatileIxA	<= tNxtVolatileIxA;
	tVolatileIxB	<= tNxtVolatileIxB;

	tRegInMmcr		<= regInMmcr;
	tRegRng1		<= { tRegRng1[47:0], regRng };
	tResetL			<= reset;

	if(dcInHoldN)
	begin
		tSyncRov		<= tNxtSyncRov;
		tSyncEpoch		<= tNxtSyncEpoch;
		tSyncEpochL		<= tSyncEpoch;

		tReqStrobe1		<= !tReqStrobe1;
		tReqStrobe2		<= tReqStrobe2 ^ tReqStrobe1;

		tVolatileInhCnt	<= tNxtVolatileInh;
		tVolatileInhSet	<= 0;

//		tInPmode		<= tNxtInPmode;
		tSkipTlb		<= tNxtSkipTlb;

		tSrJQ			<= regInSr[31];
		tRegInSr		<= regInSr;
		tRegInSrL		<= tRegInSr;
	
		/* EX1 -> EX2 */
		tReqAddr		<= tNxtReqAddr;
		tReqAddrHi		<= tNxtReqAddrHi;
		tReqAddrHiIsNz	<= tNxtReqAddrHiIsNz;
		tReqIxA			<= tNxtReqIxA;
		tReqIxB			<= tNxtReqIxB;
		tReqAxA			<= tNxtReqAxA;
		tReqAxB			<= tNxtReqAxB;
		tReqAxH			<= tNxtReqAxH;
		tReqBix			<= tNxtReqBix;
		tReqOpm			<= tNxtReqOpm;
		tReqLdOp		<= tNxtReqLdOp;
		tReqInValA		<= tNxtReqInValA;
		tReqInValB		<= tNxtReqInValB;

`ifdef def_true
		tReqIsNz		<= tNxtReqIsNz;
		tReqIsMmio		<= tNxtReqIsMmio;
		tReqIsCcmd		<= tNxtReqIsCcmd;
		tReqAddrIsVirt	<= tNxtReqAddrIsVirt;
`endif

`ifdef jx2_mem_lane2
		tReqAddrB		<= tNxtReqAddrB;
		tReqBixB		<= tNxtReqBixB;
		tReqOpmB		<= tNxtReqOpmB;
		tReqIxE			<= tNxtReqIxE;
		tReqIxF			<= tNxtReqIxF;
		tReqAxE			<= tNxtReqAxE;
		tReqAxF			<= tNxtReqAxF;
`endif

//		tUtlbBlkIx		<= tNxtReqAddr[11:8];
		tUtlbBlkIx		<= tNxtUtlbBlkIx;

		/* EX2 -> EX3 */
		tRegOutValA		<= tBlkExDataA;
		tRegOutValB		<= tBlkExDataB;

		tBlk2MemAddrA	<= tBlkMemAddr2A;
		tBlk2MemAddrB	<= tBlkMemAddr2B;
		tBlk2MemDataA	<= tBlkMemData2A;
		tBlk2MemDataB	<= tBlkMemData2B;
		tBlk2MemDextA	<= tBlkMemDext2A;
		tBlk2MemDextB	<= tBlkMemDext2B;
		tBlk2MemAextA	<= tBlkMemAext2A;
		tBlk2MemAextB	<= tBlkMemAext2B;

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
	tFlushRng		<= tNxtFlushRng;
	tFlushRovTlb	<= tNxtFlushRovTlb;

	tReqReadyL		<= tReqReady;

	tReqIxAL		<= tReqIxA;
	tReqIxBL		<= tReqIxB;

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

	{ tBlkMemDextA, tBlkMemAextA, tBlkMemAddrA }	<= arrMemAddrA[tReq1IxA];
	{ tBlkMemDextB, tBlkMemAextB, tBlkMemAddrB }	<= arrMemAddrB[tReq1IxB];
	tBlkMemDataA	<= arrMemDataA[tReq1IxA];
	tBlkMemDataB	<= arrMemDataB[tReq1IxB];
	tBlkMemIdxA		<= tReq1IxA;
	tBlkMemIdxB		<= tReq1IxB;

`ifdef jx2_mem_lane2
	{ tBlkMemDextE, tBlkMemAextE, tBlkMemAddrE }	<= arrMemAddrE[tReq1IxE];
	{ tBlkMemDextF, tBlkMemAextF, tBlkMemAddrF }	<= arrMemAddrF[tReq1IxF];
	tBlkMemDataE	<= arrMemDataE[tReq1IxE];
	tBlkMemDataF	<= arrMemDataF[tReq1IxF];
`endif

	if(tArrMemDoStA)
	begin
//		$display("L1 D$ STA Ix=%X A=%X D=%X",
//			tArrMemIdxStA, tArrMemAddrStA, tArrMemDataStA);
		arrMemAddrA[tArrMemIdxStA]	<=
			{ tArrMemDextStA, tArrMemAextStA, tArrMemAddrStA };
		arrMemDataA[tArrMemIdxStA]	<= tArrMemDataStA;

`ifdef jx2_mem_lane2
		arrMemAddrE[tArrMemIdxStA[5:0]]	<=
			{ tArrMemDextStA, tArrMemAextStA, tArrMemAddrStA };
		arrMemDataE[tArrMemIdxStA[5:0]]	<= tArrMemDataStA;
`endif
	end

	if(tArrMemDoStB)
	begin
//		$display("L1 D$ STB Ix=%X A=%X D=%X",
//			tArrMemIdxStB, tArrMemAddrStB, tArrMemDataStB);
		arrMemAddrB[tArrMemIdxStB]	<=
			{ tArrMemDextStB, tArrMemAextStB, tArrMemAddrStB };
		arrMemDataB[tArrMemIdxStB]	<= tArrMemDataStB;

`ifdef jx2_mem_lane2
		arrMemAddrF[tArrMemIdxStB[5:0]]	<=
			{ tArrMemDextStB, tArrMemAextStB, tArrMemAddrStB };
		arrMemDataF[tArrMemIdxStB[5:0]]	<= tArrMemDataStB;
`endif
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
	tArrMemDidStAextA	<= tArrMemAextStA;
	tArrMemDidStAextB	<= tArrMemAextStB;

	tArrMemDidStIxA	<= tArrMemIdxStA;
	tArrMemDidStIxB	<= tArrMemIdxStB;
	tArrMemDidStA	<= tArrMemDoStA;
	tArrMemDidStB	<= tArrMemDoStB;

`ifdef jx2_mem_lane2
	tArrMemDidStIxE	<= tArrMemIdxStA[5:0];
	tArrMemDidStIxF	<= tArrMemIdxStB[5:0];
`endif

	tMemRespStA		<= dcInHold ? tNxtMemRespStA : 0;
	tMemRespStB		<= dcInHold ? tNxtMemRespStB : 0;
	tMemRespLdA		<= dcInHold ? tNxtMemRespLdA : 0;
	tMemRespLdB		<= dcInHold ? tNxtMemRespLdB : 0;

	tReqDoMissAL	<= dcInHold ? tReqDoMissA : 0;
	tReqDoMissBL	<= dcInHold ? tReqDoMissB : 0;

	tMemSeqReqL		<= tMemSeqReq;
	tMemOpmReqL		<= tMemOpmReq;
	tMemAddrReqL	<= tMemAddrReq;
	tMemDataReqL	<= tMemDataReq;

	tMemSeqIxL		<= tMemSeqIx;
	tMemSeqVaL		<= tMemSeqVa;

	tReqMissAL		<= tReqMissA;
	tReqMissBL		<= tReqMissB;
	tReqMissL		<= tReqMiss;

	tRegOutExc2		<= tRegOutExc;
	tRegOutExc3		<= tRegOutExc2;

	if(tResetL)
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

		tMemSeqRov	<= tNxtMemSeqRov;

		tMemReqStA	<= dcInHold ? tNxtMemReqStA : 0;
		tMemReqStB	<= dcInHold ? tNxtMemReqStB : 0;
		tMemReqLdA	<= dcInHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= dcInHold ? tNxtMemReqLdB : 0;

		tMemReqLdM	<= dcInHold ? tNxtMemReqLdM : 0;

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

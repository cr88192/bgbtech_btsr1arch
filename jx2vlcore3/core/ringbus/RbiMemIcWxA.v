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
L1 Instruction Cache, WEX
 */

`include "ringbus/RbiDefs.v"

module RbiMemIcWxA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,	icOutPcLow,

	icInPcHold,		icInPcWxm,
	icInPcOpm,		regInSr,
	icMemWait,		regOutExc,
	regOutPcSxo,	regKrrHash,
	tlbExc,			icExecAcl,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

`input_vaddr	regInPc;		//input PC address
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 3: 0]	regOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	regOutPcSxo;
(* max_fanout = 100 *)
	input			icInPcHold;
// input[1:0]		icInPcWxm;
input[5:0]		icInPcWxm;
input[5:0]		icInPcOpm;		//OPM (Used for cache-control)

input [63: 0]	regInSr;
input [ 7: 0]	regKrrHash;
input [127:0]	tlbExc;

output[15:0]	icExecAcl;
output[11:0]	icOutPcLow;

output			icMemWait;
output[127: 0]	regOutExc;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l1addr	memAddrIn;		//memory input address
`output_l1addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg [63: 0]		tRegInSr;
reg [63: 0]		tRegInSrL;

reg[95:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 3: 0]		tRegOutPcStep;	//PC step (Normal Op)
reg[ 3: 0]		tRegOutPcStepA;	//PC step (Normal Op)
reg[ 3: 0]		tRegOutPcSxo;	//
reg[11: 0]		tRegOutPcLow;	//

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;
assign	regOutPcSxo		= tRegOutPcSxo;
assign	icOutPcLow		= tRegOutPcLow;

reg			tMemWait;
assign	icMemWait = tMemWait;

reg[15:0]		tIcExecAcl;
assign		icExecAcl = tIcExecAcl;

reg[127: 0]	tRegOutExc;
reg[127: 0]	tRegOutExc2;
reg[127: 0]	tRegOutExc3;
// assign	regOutExc = tRegOutExc2;
assign	regOutExc = tRegOutExc3;

reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l1addr	tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

wire		icInPcWxe;
assign		icInPcWxe = icInPcWxm[1];

wire		icInPcXG2;
`ifdef jx2_enable_xg2mode
assign		icInPcXG2 = icInPcWxm[5];
`else
assign		icInPcXG2 = 0;
`endif

`define	jx2_mem_l1i_regicdata	reg[143:0]
`define	jx2_mem_l1i_regicaddr	reg[ 71:0]

`ifdef jx2_mem_l1isz_1024
`jx2_mem_l1i_regicdata		icCaMemA[1023:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemB[1023:0];	//Local L1 tile memory (Odd)
`jx2_mem_l1i_regicdata		icCaMemC[1023:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemD[1023:0];	//Local L1 tile memory (Odd)

`jx2_mem_l1i_regicaddr		icCaAddrA[1023:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrB[1023:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrC[1023:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrD[1023:0];	//Local L1 tile address
`endif

`ifdef jx2_mem_l1isz_512
`jx2_mem_l1i_regicdata		icCaMemA[511:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemB[511:0];	//Local L1 tile memory (Odd)
`jx2_mem_l1i_regicdata		icCaMemC[511:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemD[511:0];	//Local L1 tile memory (Odd)

`jx2_mem_l1i_regicaddr		icCaAddrA[511:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrB[511:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrC[511:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrD[511:0];	//Local L1 tile address
`endif

`ifdef jx2_mem_l1isz_256
`jx2_mem_l1i_regicdata		icCaMemA[255:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemB[255:0];	//Local L1 tile memory (Odd)
`jx2_mem_l1i_regicdata		icCaMemC[255:0];	//Local L1 tile memory (Even)
`jx2_mem_l1i_regicdata		icCaMemD[255:0];	//Local L1 tile memory (Odd)

`jx2_mem_l1i_regicaddr		icCaAddrA[255:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrB[255:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrC[255:0];	//Local L1 tile address
`jx2_mem_l1i_regicaddr		icCaAddrD[255:0];	//Local L1 tile address
`endif

`ifdef jx2_mem_l1isz_128
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemA[127:0];	//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemB[127:0];	//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemC[127:0];	//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemD[127:0];	//Local L1 tile memory (Odd)

(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrA[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrB[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrC[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrD[127:0];	//Local L1 tile address

//reg[127:0]			icFlushMskA;
//reg[127:0]			icFlushMskB;
//reg[127:0]			icNxtFlushMskA;
//reg[127:0]			icNxtFlushMskB;
`endif

`ifdef jx2_mem_l1isz_64
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemB[63:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemC[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicdata		icCaMemD[63:0];		//Local L1 tile memory (Odd)

(* ram_style = "distributed" *)
`jx2_mem_l1i_regicaddr		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicaddr		icCaAddrB[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicaddr		icCaAddrC[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
`jx2_mem_l1i_regicaddr		icCaAddrD[63:0];	//Local L1 tile address

`endif

`ifdef jx2_enable_vaddr48
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrB;
`else
(* max_fanout = 50 *)
	reg[27:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[27:0]		tNxtAddrB;
`endif

`ifdef jx2_mem_l1isz_1024
reg[9:0]		tNxtIxA;
reg[9:0]		tNxtIxB;
reg[9:0]		tReqIxA;
reg[9:0]		tReqIxB;
reg[9:0]		tReqIxAL;
reg[9:0]		tReqIxBL;

wire[9:0]		tNx2IxA;
wire[9:0]		tNx2IxB;
reg[9:0]		tNx2IxAL;
reg[9:0]		tNx2IxBL;
`endif

`ifdef jx2_mem_l1isz_512
reg[8:0]		tNxtIxA;
reg[8:0]		tNxtIxB;
reg[8:0]		tReqIxA;
reg[8:0]		tReqIxB;
reg[8:0]		tReqIxAL;
reg[8:0]		tReqIxBL;

wire[8:0]		tNx2IxA;
wire[8:0]		tNx2IxB;
reg[8:0]		tNx2IxAL;
reg[8:0]		tNx2IxBL;
`endif

`ifdef jx2_mem_l1isz_256
reg[7:0]		tNxtIxA;
reg[7:0]		tNxtIxB;
reg[7:0]		tReqIxA;
reg[7:0]		tReqIxB;
reg[7:0]		tReqIxAL;
reg[7:0]		tReqIxBL;

wire[7:0]		tNx2IxA;
wire[7:0]		tNx2IxB;
reg[7:0]		tNx2IxAL;
reg[7:0]		tNx2IxBL;
`endif

`ifdef jx2_mem_l1isz_128
reg[6:0]		tNxtIxA;
reg[6:0]		tNxtIxB;
reg[6:0]		tReqIxA;
reg[6:0]		tReqIxB;
reg[6:0]		tReqIxAL;
reg[6:0]		tReqIxBL;

wire[6:0]		tNx2IxA;
wire[6:0]		tNx2IxB;
reg[6:0]		tNx2IxAL;
reg[6:0]		tNx2IxBL;
`endif

`ifdef jx2_mem_l1isz_64
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;
reg[5:0]		tReqIxAL;
reg[5:0]		tReqIxBL;

wire[5:0]		tNx2IxA;
wire[5:0]		tNx2IxB;
reg[5:0]		tNx2IxAL;
reg[5:0]		tNx2IxBL;
`endif

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[127:0]		tBlkDataC;
reg[127:0]		tBlkDataD;
reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData2B;
reg[127:0]		tBlkData2A2;
reg[127:0]		tBlkData2B2;

reg[15:0]		tBlkDextA;
reg[15:0]		tBlkDextB;
reg[15:0]		tBlkDext2A;
reg[15:0]		tBlkDext2B;

`ifdef jx2_enable_vaddr48
reg[43:0]		tBlkAddrA;
reg[43:0]		tBlkAddrB;
reg[43:0]		tBlkAddrC;
reg[43:0]		tBlkAddrD;
reg[43:0]		tBlkAddr2A;
reg[43:0]		tBlkAddr2B;
`else
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;
reg[27:0]		tBlkAddrC;
reg[27:0]		tBlkAddrD;
reg[27:0]		tBlkAddr2A;
reg[27:0]		tBlkAddr2B;
`endif
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFlagB;
reg[ 3:0]		tBlkFlagC;
reg[ 3:0]		tBlkFlagD;
reg[ 3:0]		tBlkFlag2A;
reg[ 3:0]		tBlkFlag2B;

reg[15:0]		tBlkPFlA;
reg[15:0]		tBlkPFlB;
// reg[7:0]		tBlkPFlC;
// reg[7:0]		tBlkPFlD;
reg[7:0]		tBlkPRovA;
reg[7:0]		tBlkPRovB;
reg[7:0]		tBlkPRovC;
reg[7:0]		tBlkPRovD;

reg[15:0]		tBlkPFl2A;
reg[15:0]		tBlkPFl2B;
reg[7:0]		tBlkPRov2A;
reg[7:0]		tBlkPRov2B;

reg[47:0]		tReqAddrHi;
reg[47:0]		tNxtAddrHi;
reg[15:0]		tReqAxH;
reg[15:0]		tNxtAxH;

`ifdef jx2_enable_vaddr48
reg[43:0]		tReqAddrA;
reg[43:0]		tReqAddrB;
reg[43:0]		tReqAddrAL;
reg[43:0]		tReqAddrBL;
`else
reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
reg[27:0]		tReqAddrAL;
reg[27:0]		tReqAddrBL;
`endif

`reg_vaddr		tInAddr;
`reg_vaddr		tInAddrNl;
reg[1:0]		tInWordIx;
`reg_vaddr		tRegInPc;		//input PC address
reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg[5:0]		tInPcOpm;		//OPM (Used for cache-control)
reg				tInPcWxe;
reg				tInPcXG2;
reg				tInPcRiscv;
reg				tNxtInPcWxe;
reg				tNxtInPcXG2;
reg				tNxtInPcRiscv;

reg[3:0]		tInPmode;
reg[3:0]		tNxtInPmode;

reg				tTlbMissInh;
reg				tNxtTlbMissInh;
reg				tClrTlbMissInh;
reg				tClrTlbMissInhL;

reg				tSkipTlb;
reg				tNxtSkipTlb;
reg				tSkipMiss;
reg				tNxtSkipMiss;

reg				tReqAddrIsVirt;
reg				tReqAddrNoExecute;

reg				tStuckTlbMissInh;
reg				tStuckTlbMissInhL;

reg				tBlkFlushA;
reg				tBlkFlushB;

reg				tFlushA;
reg				tFlushB;
reg				tFlushAL;
reg				tFlushBL;

reg				tMissA;
reg				tMissB;
reg				tMissC;
reg				tMissD;
reg				tMiss;
reg				tMissL;
reg				tMiss1L;
reg				tPcStepWA;
reg				tPcStepWB;
reg				tPcStepJA;
reg				tPcStepBA;
reg				tPcStepBB;

reg				tMissAL;
reg				tMissBL;
reg				tMissA1L;
reg				tMissB1L;

reg				tMissAddrA;
reg				tMissAddrB;
reg				tMissAddrC;
reg				tMissAddrD;

reg				tMissSkipA;
reg				tMissSkipB;

// reg[127:0]		tBlkData;
reg[159:0]		tBlkData;
reg[127:0]		tBlkData0A;
reg[255:0]		tBlkData1A;

reg[3:0]		opLenA0;
reg[3:0]		opLenA1;
reg[3:0]		opLenA2;
reg[3:0]		opLenA3;
reg[3:0]		opLenA4;
reg[3:0]		opLenA5;

reg				icDoFlush;
reg				icNxtDoFlush;

reg[43:0]		tRegInPcP0;
reg[43:0]		tRegInPcP1;
reg[14:0]		tRegInPcP1L;
reg[14:0]		tRegInPcP1H;

reg[7:0]		tFlushRov;
reg[7:0]		tNxtFlushRov;
reg				tAdvFlushRov;
reg				tNxtAdvFlushRov;


wire			memRingIsIdle;
wire			memRingIsResp;
assign		memRingIsIdle = (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		memRingIsResp =
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire			memRingIsRespOkLd;
wire			memRingIsRespOkLdA;
wire			memRingIsRespOkLdB;
assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkLdA =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);
assign		memRingIsRespOkLdB =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b11);

`ifdef jx2_mem_l1isz_1024
reg[  9:0]		tReqSeqIdxArr[15:0];
reg[  9:0]		tReqSeqIdx;
reg[9:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_512
reg[  8:0]		tReqSeqIdxArr[15:0];
reg[  8:0]		tReqSeqIdx;
reg[8:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_256
reg[  7:0]		tReqSeqIdxArr[15:0];
reg[  7:0]		tReqSeqIdx;
reg[7:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_128
reg[  6:0]		tReqSeqIdxArr[15:0];
reg[  6:0]		tReqSeqIdx;
reg[6:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_64
reg[  5:0]		tReqSeqIdxArr[15:0];
reg[  5:0]		tReqSeqIdx;
reg[5:0]		tMemSeqIx;
`endif

reg[ 43:0]		tReqSeqVaArr[15:0];
reg[ 43:0]		tReqSeqVa;
reg[43:0]		tMemSeqVa;

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l1addr		tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

// reg				tMemReqStA;		//Store A
// reg				tMemReqStB;		//Store B
// reg				tNxtMemReqStA;	//Store A
// reg				tNxtMemReqStB;	//Store B

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
// reg				tMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
// reg				tNxtMemReqLdM;	//Load MMIO

reg				tMemRespLdA;		//Load A
reg				tMemRespLdB;		//Load B
reg				tNxtMemRespLdA;		//Load A
reg				tNxtMemRespLdB;		//Load B

reg				tReqWaitResp;




// reg[ 7:0]		tMemReqRtcnt;
// reg[ 7:0]		tNxtMemReqRtcnt;
reg[11:0]		tMemReqRtcnt;
reg[11:0]		tNxtMemReqRtcnt;

reg[11:0]		tMemTlbInhRtcnt;
reg[11:0]		tNxtMemTlbInhRtcnt;

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;


reg				tRegOutHold;
reg				tRegOutHoldL;
reg				tReqReady;

reg				tBlkIsSxo;
reg				tBlkDoRiscV;
reg				tBlkDoRvC;
reg				tBlkDoRvW;
reg				tDoStallNop;

reg				tMsgLatch;
reg				tNxtMsgLatch;

reg				tMsgLatchTmiss;
reg				tNxtMsgLatchTmiss;

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;

reg[127:0]		tStBlkDataA;
reg[3:0]		tStBlkFlagA;
reg				tDoStBlkA;
reg				tDidStBlkA;

reg[127:0]		tStBlkDataB;
reg[3:0]		tStBlkFlagB;
reg				tDoStBlkB;
reg				tDidStBlkB;

reg[43:0]		tStBlkAddrA;
reg[43:0]		tStBlkAddrB;

reg[15:0]		tStBlkDextA;
reg[15:0]		tStBlkDextB;

reg[15:0]		tStBlkPFlA;
reg[15:0]		tStBlkPFlB;
// reg[7:0]		tStBlkPFlC;
// reg[7:0]		tStBlkPFlD;
reg[7:0]		tStBlkPRovA;
reg[7:0]		tStBlkPRovB;
reg[7:0]		tStBlkPRovC;
reg[7:0]		tStBlkPRovD;



`ifdef jx2_mem_l1isz_1024
reg[9:0]		tStBlkIxA;
reg[9:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_512
reg[8:0]		tStBlkIxA;
reg[8:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_256
reg[7:0]		tStBlkIxA;
reg[7:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_128
reg[6:0]		tStBlkIxA;
reg[6:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_64
reg[5:0]		tStBlkIxA;
reg[5:0]		tStBlkIxB;
`endif

reg[127:0]		tStickyTlbExc;
reg[127:0]		tNxtStickyTlbExc;

reg				tResetL;

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tClrTlbMissInh		= 0;
	tRegOutHold			= 0;
	tMemWait			= 0;
	tRegOutExc			= 0;
	tNxtMsgLatch		= 0;
	tNxtMsgLatchTmiss	= 0;
	tNxtSkipTlb			= 0;
	tNxtSkipMiss		= 0;
	
	tNxtStickyTlbExc	= tStickyTlbExc;
	if(tlbExc[15:12]==4'h7)
		tNxtStickyTlbExc	= tlbExc;

	if(tClrTlbMissInhL && tTlbMissInh)
		tClrTlbMissInh		= 1;

`ifndef def_true
// `ifdef def_true
	if(tReqIxAL != tReqIxA)
//	if((tReqIxAL != tReqIxA) && tStuckTlbMissInhL)
	begin
		if(tTlbMissInh)
			$display("L1I$ Clear TLB Inhibit, Drop");
		tNxtTlbMissInh		= 0;
	end
`endif

//	if(tMemNoRwx[5])
//		tNxtTlbMissInh = 1;
	
//	if((tInOpmB == JX2_DCOPM_LDTLB) || tRegInSr[29])
//	if((tInOpmB == JX2_DCOPM_LDTLB) || (tRegInSr[29] && tRegInSr[30]))
//	if(tRegInSr[29] && tRegInSr[30])
	if(	(tRegInSr[29] && tRegInSr[30]) &&
		(regInSr[29] && regInSr[30]) &&
		(tRegInSrL[29] && tRegInSrL[30]))
//	if(	(tRegInSr[29] && tRegInSr[30]) &&
//		(tRegInSrL[29] && tRegInSrL[30]))
	begin
		if(tTlbMissInh)
			$display("L1I$ Clear TLB Inhibit");
//			$display("L1I$ Clear TLB Inhibit, InPc=%X InAddr=%X",
//				regInPc, tInAddr);
		tNxtTlbMissInh	= 0;
		tClrTlbMissInh	= 1;
//		tSkipTlb		= 1;
	end
	

	/* Stage A */

//	tNxtSkipTlb			= regInSr[29] && regInSr[30];
	tNxtSkipTlb			= tRegInSr[29] && tRegInSr[30];
//	tNxtSkipMiss		= tRegInSr[29] != regInSr[29];
	tNxtSkipMiss		= !tRegInSr[29] && regInSr[29];

// `ifndef def_true
`ifdef def_true
//	if(tNxtSkipTlb && (regInPc[47:28]!=0) && !icInPcHold)
//	if(tNxtSkipTlb && (regInPc[47:28]!=0))
	if(tSkipTlb && (tInAddr[47:28]!=0))
	begin
		$display("L1 I$: Next Skip TLB and Addr is Virt, A=%X",
			tInAddr);
	end
`endif

	tRegInPc	= regInPc;
	tNxtAddrHi	= 0;
	tNxtAxH	= 0;
`ifdef jx2_enable_vaddr96
	tNxtAddrHi	= regInPc[95:48];
//	tNxtAxH	=
//		tNxtAddrHi[15: 0] ^
//		tNxtAddrHi[31:16] ^
//		tNxtAddrHi[47:32] ;

	tNxtAxH		=
		tNxtAddrHi[15: 0] ^
		{	tNxtAddrHi[23:16], tNxtAddrHi[31:24] } +
		{	tNxtAddrHi[35:32], tNxtAddrHi[39:36],
			tNxtAddrHi[43:40], tNxtAddrHi[47:44] } +
		{	4'h0, regInSr[31:28], regKrrHash[7:0] } ;
//		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;
`else

	tNxtAxH		=
		{	4'h0, regInSr[31:28], regKrrHash[7:0] } ;
//		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;

`endif

	tInPcOpm	= icInPcOpm;
	
//	tNxtInPmode		= regInSr[31:28];
	tNxtInPmode		= regInSr[31:28] ^ regKrrHash[3:0] ^ regKrrHash[7:4];

	tRegInPcP0	= tRegInPc[47:4];
	tRegInPcP1[27: 0]	= tRegInPcP0[27: 0]+1;
	tRegInPcP1[43:28]	= tRegInPcP0[43:28];

	if(tRegInPc[4])
	begin
		tNxtAddrB			= tRegInPcP0;
		tNxtAddrA			= tRegInPcP1;
	end else begin
		tNxtAddrA			= tRegInPcP0;
		tNxtAddrB			= tRegInPcP1;
	end

	tNxtInPcWxe		= icInPcWxe;
	tNxtInPcXG2		= icInPcXG2;
//	tNxtInPcRiscv	= (icInPcWxm == 2'b01);
	tNxtInPcRiscv	= (icInPcWxm[1:0] == 2'b01);

	if(icInPcHold)
	begin
		tRegInPc		= tInAddr;
		tNxtAddrA		= tReqAddrA;
		tNxtAddrB		= tReqAddrB;
		tNxtAddrHi		= tReqAddrHi;
		tNxtAxH			= tReqAxH;
		tNxtInPmode		= tInPmode;
		tNxtInPcWxe		= tInPcWxe;
		tNxtInPcXG2		= tInPcXG2;
		tNxtInPcRiscv	= tInPcRiscv;
		tNxtSkipTlb		= tSkipTlb;
		tNxtSkipMiss	= tSkipMiss;
	end

`ifdef jx2_l1i_nohash

`ifdef jx2_mem_l1isz_1024
	tNxtIxA=tNxtAddrA[10:1];
	tNxtIxB=tNxtAddrB[10:1];
`endif

`ifdef jx2_mem_l1isz_512
	tNxtIxA=tNxtAddrA[9:1];
	tNxtIxB=tNxtAddrB[9:1];
`endif

`ifdef jx2_mem_l1isz_256
	tNxtIxA=tNxtAddrA[8:1];
	tNxtIxB=tNxtAddrB[8:1];
`endif

`ifdef jx2_mem_l1isz_128
	tNxtIxA=tNxtAddrA[7:1];
	tNxtIxB=tNxtAddrB[7:1];
`endif

`ifdef jx2_mem_l1isz_64
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif

`else

`ifdef jx2_mem_l1isz_1024
//	tNxtIxA=tNxtAddrA[9:1];
//	tNxtIxB=tNxtAddrB[9:1];
	tNxtIxA=tNxtAddrA[10:1]^tNxtAddrA[20:11];
	tNxtIxB=tNxtAddrB[10:1]^tNxtAddrB[20:11];
`endif

`ifdef jx2_mem_l1isz_512
//	tNxtIxA=tNxtAddrA[9:1];
//	tNxtIxB=tNxtAddrB[9:1];
	tNxtIxA=tNxtAddrA[9:1]^tNxtAddrA[18:10];
	tNxtIxB=tNxtAddrB[9:1]^tNxtAddrB[18:10];
`endif

`ifdef jx2_mem_l1isz_256
//	tNxtIxA=tNxtAddrA[8:1];
//	tNxtIxB=tNxtAddrB[8:1];
	tNxtIxA=tNxtAddrA[8:1]^tNxtAddrA[16:9];
	tNxtIxB=tNxtAddrB[8:1]^tNxtAddrB[16:9];
`endif

`ifdef jx2_mem_l1isz_128
	tNxtIxA=tNxtAddrA[7:1];
	tNxtIxB=tNxtAddrB[7:1];
`endif

`ifdef jx2_mem_l1isz_64
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif

`endif

	icNxtDoFlush = 0;

	tNxtFlushRov	= tFlushRov;
	tNxtAdvFlushRov	= 0;

//	if(((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS)) || tResetL)
//	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS) && !tResetL)
	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS))
	begin
		icNxtDoFlush = 1;
	end

//	if((tFlushRov == 0) && !tAdvFlushRov && !tResetL)
	if((tFlushRov == 0) && !tAdvFlushRov)
		icNxtDoFlush = 1;
	
	if(icDoFlush)
	begin
		if(!tAdvFlushRov)
		begin
			$display("L1 I$ DoFlush rov=%X", tFlushRov);
			tNxtFlushRov	= tFlushRov+1;
			tNxtAdvFlushRov	= 1;
		end
	end

	if(tResetL)
	begin
		tNxtFlushRov		= 0;
		icNxtDoFlush		= 0;
	end

	/* Stage B */
	
	tStBlkAddrA		= 0;
	tStBlkAddrB		= 0;
	tStBlkDataA		= 0;
	tStBlkDataB		= 0;
	tStBlkFlagA		= 0;
	tStBlkFlagB		= 0;

	tStBlkIxA		= 0;
	tStBlkIxB		= 0;
	tStBlkPFlA		= 0;
	tStBlkPFlB		= 0;
	tStBlkPRovA		= tFlushRov;
	tStBlkPRovB		= tFlushRov;

	tStBlkDextA		= tReqAxH;
	tStBlkDextB		= tReqAxH;

	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	
//	tNxtMemReqStA	= 0;
//	tNxtMemReqStB	= 0;
//	tNxtMemReqLdM	= 0;
	tBlkDoRiscV		= 0;
	tBlkDoRvC		= 0;
	tBlkDoRvW		= 0;
	
	tInWordIx = tInAddr[2:1];

//	tFlushA = (tBlkPRovA != tFlushRov) || (tFlushRov == 0);
//	tFlushB = (tBlkPRovB != tFlushRov) || (tFlushRov == 0);
	tFlushA = tBlkPRovA != tFlushRov;
	tFlushB = tBlkPRovB != tFlushRov;
	
//	tReqReady	= 1;
	tReqReady	= (tReqIxA == tNx2IxAL) && (tReqIxB == tNx2IxBL);
	
	if(!tTlbMissInh)
	begin
//		if(tBlkFlagA[3])
		if(tBlkFlagA[3:2]==2'b11)
		begin
			if(tMemReqRtcnt == 0)
			begin
				if(!tMsgLatch)
					$display("L1I$: TLB Flush Flag A, A=%X", tBlkAddrA);
				tNxtMsgLatch = 1;
			end
			tFlushA = 1;
		end
//		if(tBlkFlagB[3])
		if(tBlkFlagB[3:2]==2'b11)
		begin
			if(tMemReqRtcnt == 0)
			begin
				if(!tMsgLatch)
					$display("L1I$: TLB Flush Flag B, A=%X", tBlkAddrB);
				tNxtMsgLatch = 1;
			end
			tFlushB = 1;
		end

`ifndef def_true
// `ifdef def_true
		if(tBlkPFlA[7:4]!=tInPmode)
		begin
			if(tMemReqRtcnt == 0)
				$display("L1I$: Flush PMode A, A=%X", tBlkAddrA);
			tFlushA = 1;
		end
		if(tBlkPFlB[7:4]!=tInPmode)
		begin
			if(tMemReqRtcnt == 0)
				$display("L1I$: Flush PMode B, A=%X", tBlkAddrB);
			tFlushB = 1;
		end
`endif

	end

//	tIcExecAcl = tBlkPFlA;
	tIcExecAcl = tBlkPFlB;
	
	tReqAddrIsVirt		= (tInAddr[47:28] != 0) && !tInAddr[47];
	tReqAddrNoExecute	= 0;

	tRegOutExc[ 63:16] = tInAddr[47:0];
	tRegOutExc[111:64] = tReqAddrHi[47:0];

	if(!tRegInSr[30])
	begin
	//	if(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])
		if(	(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])	||
			(tBlkFlagB[2] && !tBlkFlagB[3] &&  tInAddr[4])	||
			tInAddr[47] )
		begin
			tReqAddrNoExecute	= 0;
//			$display("L1 I$: Bad Execute %X %X/%X",
//				tInAddr, tBlkFlagA, tBlkFlagB);
//			tRegOutExc[15:0] = 16'h8003;
		end
	end
	
	tBlkIsSxo = 0;
//	if(tBlkFlagA[3:0]==4'b1011)
//	if(tBlkFlagA[3] && tBlkFlagA[1] && !tBlkFlagA[2])
	if(	(tBlkFlagA[3] && tBlkFlagA[1] && !tBlkFlagA[2] && !tInAddr[4])	||
		(tBlkFlagB[3] && tBlkFlagB[1] && !tBlkFlagB[2] &&  tInAddr[4])	)
			tBlkIsSxo = 1;


`ifndef def_true
	if((~(tBlkPFlA[3:0])) != tFlushRov[3:0])
		tFlushA = 1;
	if((~(tBlkPFlB[3:0])) != tFlushRov[3:0])
		tFlushB = 1;
`endif

// `ifdef def_true
`ifndef def_true
	if((~(tBlkPFlA[2:0])) != tFlushRov[2:0])
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
				$display("L1I$: Rover Flush A");
			tNxtMsgLatch = 1;
		end
		tFlushA = 1;
	end
	if((~(tBlkPFlB[2:0])) != tFlushRov[2:0])
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
				$display("L1I$: Rover Flush B");
			tNxtMsgLatch = 1;
		end
		tFlushB = 1;
	end
`endif

	tBlkData2A	= tBlkDataA;
	tBlkDext2A	= tBlkDextA;
	tBlkAddr2A	= tBlkAddrA;
	tBlkFlag2A	= tBlkFlagA;
	tBlkPFl2A	= tBlkPFlA;

	tBlkData2B	= tBlkDataB;
	tBlkDext2B	= tBlkDextB;
	tBlkAddr2B	= tBlkAddrB;
	tBlkFlag2B	= tBlkFlagB;
	tBlkPFl2B	= tBlkPFlB;

	if(tDidStBlkA || tDidStBlkB)
		tReqReady	= 0;


`ifdef def_true
	tMissAddrA =
		(tBlkAddr2A[43:36] != tReqAddrA[43:36]) ||
		(tBlkAddr2A[35:24] != tReqAddrA[35:24]) ||
		(tBlkAddr2A[24:12] != tReqAddrA[24:12]) ||
		(tBlkAddr2A[11: 0] != tReqAddrA[11: 0]) ;
	tMissAddrB =
		(tBlkAddr2B[43:36] != tReqAddrB[43:36]) ||
		(tBlkAddr2B[35:24] != tReqAddrB[35:24]) ||
		(tBlkAddr2B[23:12] != tReqAddrB[23:12]) ||
		(tBlkAddr2B[11: 0] != tReqAddrB[11: 0]) ;
`endif

//	if((tBlkDext2A != tReqAxH) && !tTlbMissInh)
	if(tBlkDext2A != tReqAxH)
		tMissAddrA = 1;
//	if((tBlkDext2B != tReqAxH) && !tTlbMissInh)
	if(tBlkDext2B != tReqAxH)
		tMissAddrB = 1;

// `ifdef def_true
`ifndef def_true
	if((tBlkData2A[31:0]==UV32_00) && !tTlbMissInh)
	begin
		$display("L1I$: Debug Miss A");
		tMissAddrA = 1;
	end
	if((tBlkData2B[31:0]==UV32_00) && !tTlbMissInh)
	begin
		$display("L1I$: Debug Miss B");
		tMissAddrB = 1;
	end
`endif

	tMissA =
		tMissAddrA ||
		tFlushA;
	tMissB =
		tMissAddrB ||
		tFlushB;

	tMissC = 1;
	tMissD = 1;

	if(tSkipMiss)
	begin
		tMissA = 0;
		tMissB = 0;
	end

	tMiss = tMissA || tMissB;

	if(tReqAddrNoExecute && !tMiss)
	begin
		tReqAddrNoExecute	= 0;
		$display("L1 I$: Bad Execute %X %X/%X",
			tInAddr, tBlkFlagA, tBlkFlagB);
		tRegOutExc[15:0] = 16'h8003;
	end

//	if(tMiss)
//		tRegOutExc[15] = 0;

	if(tResetL)
	begin
		tMiss		= 0;
		tReqReady	= 0;
	end

	tMissSkipA = 0;
	tMissSkipB = 0;
	if(tInAddr[4:2]==3'b000)
		tMissSkipB=1;
	if(tInAddr[4:1]==4'b0010)
		tMissSkipB=1;
	if(tInAddr[4])
		tMissSkipA=1;

	case(tInAddr[4:3])
		2'b00: tBlkData = { tBlkData2B[31:0], tBlkData2A[127: 0] };
		2'b01: tBlkData = { tBlkData2B[95:0], tBlkData2A[127:64] };
		2'b10: tBlkData = { tBlkData2A[31:0], tBlkData2B[127: 0] };
		2'b11: tBlkData = { tBlkData2A[95:0], tBlkData2B[127:64] };
	endcase


`ifdef def_true
	casez(tBlkData[12:9])
		4'b1111:	opLenA0=4'b1110;		//FE/FF
		4'b1110:	opLenA0=4'b0110;		//FC/FD
		4'b110z:	opLenA0=4'b0010;		//F8..FB
		4'b101z:	opLenA0=4'b0110;		//F4..F7
		4'b100z:	opLenA0=4'b0010;		//F0..F3
		4'b0111:	opLenA0=4'b0110;		//FE/FF
		4'b0110:	opLenA0=4'b0010;		//FC/FD
		4'b0101:	opLenA0=4'b0110;		//EA/EB
		4'b0100:	opLenA0=4'b0010;		//E8/E9
		4'b00zz:	opLenA0=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[28:25])
		4'b1111:	opLenA1=4'b1110;		//FE/FF
		4'b1110:	opLenA1=4'b0110;		//FC/FD
		4'b110z:	opLenA1=4'b0010;		//F8..FB
		4'b101z:	opLenA1=4'b0110;		//F4..F7
		4'b100z:	opLenA1=4'b0010;		//F0..F3
		4'b0111:	opLenA1=4'b0110;		//FE/FF
		4'b0110:	opLenA1=4'b0010;		//FC/FD
		4'b0101:	opLenA1=4'b0110;		//EA/EB
		4'b0100:	opLenA1=4'b0010;		//E8/E9
		4'b00zz:	opLenA1=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[44:41])
		4'b1111:	opLenA2=4'b1110;		//FE/FF
		4'b1110:	opLenA2=4'b0110;		//FC/FD
		4'b110z:	opLenA2=4'b0010;		//F8..FB
		4'b101z:	opLenA2=4'b0110;		//F4..F7
		4'b100z:	opLenA2=4'b0010;		//F0..F3
		4'b0111:	opLenA2=4'b0110;		//FE/FF
		4'b0110:	opLenA2=4'b0010;		//FC/FD
		4'b0101:	opLenA2=4'b0110;		//EA/EB
		4'b0100:	opLenA2=4'b0010;		//E8/E9
		4'b00zz:	opLenA2=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[60:57])
		4'b1111:	opLenA3=4'b1110;		//FE/FF
		4'b1110:	opLenA3=4'b0110;		//FC/FD
		4'b110z:	opLenA3=4'b0010;		//F8..FB
		4'b101z:	opLenA3=4'b0110;		//F4..F7
		4'b100z:	opLenA3=4'b0010;		//F0..F3
		4'b0111:	opLenA3=4'b0110;		//FE/FF
		4'b0110:	opLenA3=4'b0010;		//FC/FD
		4'b0101:	opLenA3=4'b0110;		//EA/EB
		4'b0100:	opLenA3=4'b0010;		//E8/E9
		4'b00zz:	opLenA3=4'b0010;		//E0..E7
	endcase

	if((tBlkData[15:13]!=3'b111) && !tInPcXG2)
		opLenA0=4'b0001;
	if((tBlkData[31:29]!=3'b111) && !tInPcXG2)
		opLenA1=4'b0001;
	if((tBlkData[47:45]!=3'b111) && !tInPcXG2)
		opLenA2=4'b0001;
	if((tBlkData[63:61]!=3'b111) && !tInPcXG2)
		opLenA3=4'b0001;

`ifdef jx2_enable_xgpr
// `ifndef def_true
	if(	((tBlkData[15:12]==4'b0111) ||
		(tBlkData[15:12]==4'b1001)) && !tInPcXG2)
			opLenA0=tBlkData[11]?4'b0110:4'b0010;
	if(	((tBlkData[31:28]==4'b0111) ||
		(tBlkData[31:28]==4'b1001)) && !tInPcXG2)
			opLenA1=tBlkData[27]?4'b0110:4'b0010;
	if(	((tBlkData[47:44]==4'b0111) ||
		(tBlkData[47:44]==4'b1001)) && !tInPcXG2)
			opLenA2=tBlkData[43]?4'b0110:4'b0010;
	if(	((tBlkData[63:60]==4'b0111) ||
		(tBlkData[63:60]==4'b1001)) && !tInPcXG2)
			opLenA3=tBlkData[59]?4'b0110:4'b0010;
`endif

`endif


	casez(tBlkData[76:73])
		4'b1111:	opLenA4=4'b1110;		//FE/FF
		4'b1110:	opLenA4=4'b0110;		//FC/FD
		4'b110z:	opLenA4=4'b0010;		//F8..FB
		4'b101z:	opLenA4=4'b0110;		//F4..F7
		4'b100z:	opLenA4=4'b0010;		//F0..F3
		4'b0111:	opLenA4=4'b0110;		//FE/FF
		4'b0110:	opLenA4=4'b0010;		//FC/FD
		4'b0101:	opLenA4=4'b0110;		//EA/EB
		4'b0100:	opLenA4=4'b0010;		//E8/E9
		4'b00zz:	opLenA4=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[92:89])
		4'b1111:	opLenA5=4'b1110;		//FE/FF
		4'b1110:	opLenA5=4'b0110;		//FC/FD
		4'b110z:	opLenA5=4'b0010;		//F8..FB
		4'b101z:	opLenA5=4'b0110;		//F4..F7
		4'b100z:	opLenA5=4'b0010;		//F0..F3
		4'b0111:	opLenA5=4'b0110;		//FE/FF
		4'b0110:	opLenA5=4'b0010;		//FC/FD
		4'b0101:	opLenA5=4'b0110;		//EA/EB
		4'b0100:	opLenA5=4'b0010;		//E8/E9
		4'b00zz:	opLenA5=4'b0010;		//E0..E7
	endcase
	if((tBlkData[79:77]!=3'b111) && !tInPcXG2)
		opLenA4=4'b0001;
	if((tBlkData[95:93]!=3'b111) && !tInPcXG2)
		opLenA5=4'b0001;

`ifdef jx2_enable_xgpr
// `ifndef def_true
	if(	((tBlkData[79:76]==4'b0111) ||
		(tBlkData[79:76]==4'b1001)) && !tInPcXG2)
			opLenA4=tBlkData[75]?4'b0110:4'b0010;
	if(	((tBlkData[95:92]==4'b0111) ||
		(tBlkData[95:92]==4'b1001)) && !tInPcXG2)
			opLenA5=tBlkData[91]?4'b0110:4'b0010;
`endif

`ifdef jx2_enable_riscv
	if(tInPcRiscv)
	begin
		tBlkDoRiscV = 1;
		tBlkDoRvC	= 1;
	end

	if(tBlkDoRiscV)
	begin
		opLenA0=4'b0010;
		opLenA1=4'b0010;
		opLenA2=4'b0010;
		opLenA3=4'b0010;
		opLenA4=4'b0010;
		opLenA5=4'b0010;
		
		if(tBlkDoRvC)
		begin
			if(tBlkData[ 1: 0]!=2'b11)
				opLenA0=4'b0001;
			if(tBlkData[17:16]!=2'b11)
				opLenA1=4'b0001;
			if(tBlkData[33:32]!=2'b11)
				opLenA2=4'b0001;
			if(tBlkData[49:48]!=2'b11)
				opLenA3=4'b0001;
			if(tBlkData[65:64]!=2'b11)
				opLenA4=4'b0001;
			if(tBlkData[81:80]!=2'b11)
				opLenA5=4'b0001;
		end

		if(tBlkDoRvW)
		begin
			if(tBlkData[ 1: 0]!=2'b11)
				opLenA0=4'b0110;
			if(tBlkData[33:32]!=2'b11)
				opLenA2=4'b0110;
			if(tBlkData[65:64]!=2'b11)
				opLenA4=4'b0110;
		end
	end
`endif


	tRegOutPcVal	= UV96_00;
	tRegOutPcStep	= 0;
	tRegOutPcStepA	= 0;
	tRegOutPcSxo	= 0;
	tPcStepWA		= 0;
	tPcStepWB		= 0;
	tPcStepJA		= 0;
	tRegOutPcLow	= tInAddr[11:0];

	if(tBlkIsSxo)
		tRegOutPcSxo[0]=1;

	if(tInWordIx[1])
		tBlkData0A = tBlkData[159:32];
	else
		tBlkData0A = tBlkData[127: 0];
	if(tInWordIx[0])
		tRegOutPcVal = tBlkData0A[111:16];
	else
		tRegOutPcVal = tBlkData0A[95: 0];

	case(tInWordIx)
		2'b00: begin
			tRegOutPcStepA	= { 1'b0, opLenA0[1:0], 1'b0 };
			tPcStepWA		= opLenA0[2];
			tPcStepWB		= opLenA2[2];
			tPcStepJA		= opLenA0[3];
			tPcStepBA		= opLenA0[0];
			tPcStepBB		= opLenA2[0];
		end
		2'b01: begin
			tRegOutPcStepA	= {1'b0, opLenA1[1:0], 1'b0 };
			tPcStepWA		= opLenA1[2];
			tPcStepWB		= opLenA3[2];
			tPcStepJA		= opLenA1[3];
			tPcStepBA		= opLenA1[0];
			tPcStepBB		= opLenA3[0];
		end
		2'b10: begin
			tRegOutPcStepA	= {1'b0, opLenA2[1:0], 1'b0 };
			tPcStepWA		= opLenA2[2];
			tPcStepWB		= opLenA4[2];
			tPcStepJA		= opLenA2[3];
			tPcStepBA		= opLenA2[0];
			tPcStepBB		= opLenA4[0];
		end
		2'b11: begin
			tRegOutPcStepA	= {1'b0, opLenA3[1:0], 1'b0 };
			tPcStepWA		= opLenA3[2];
			tPcStepWB		= opLenA5[2];
			tPcStepJA		= opLenA3[3];
			tPcStepBA		= opLenA3[0];
			tPcStepBB		= opLenA5[0];
		end
	endcase
	
// `ifdef jx2_enable_xg2mode
`ifndef def_true
	if(tInPcXG2)
	begin
		/* XG2 Mode, Eliminate 16-bit ops */
		tPcStepBA		= 0;
		tPcStepBB		= 0;
	end
`endif

	if(tInPcXG2)
	begin
		if(tPcStepBA || tPcStepBB)
		begin
			$display("L1 I$: XG2 Bad Step");
		end
		if(tInWordIx[0])
		begin
			$display("L1 I$: XG2 Misaligned Fetch, A=%X", tInAddr);
		end
	end

	if(tInAddr[0])
	begin
		$display("L1 I$: Misaligned Fetch, A=%X", tInAddr);
	end

`ifdef jx2_enable_wex3w

`ifdef jx2_enable_wexjumbo

//	casez( {icInPcWxe, tPcStepJA,
	casez( {tInPcWxe, tPcStepJA,
			tPcStepWA, tPcStepWB,
			tPcStepBB, tPcStepBA})
		6'b00zzz0: tRegOutPcStep = 4'b0100;
		6'b00zzz1: tRegOutPcStep = 4'b0010;

		6'b01z00z: tRegOutPcStep = 4'b1000;
		6'b01z01z: tRegOutPcStep = 4'b0110;
		6'b01z1zz: tRegOutPcStep = 4'b1100;

		6'b100zz0: tRegOutPcStep = 4'b0100;
		6'b100zz1: tRegOutPcStep = 4'b0010;
		6'b1010zz: tRegOutPcStep = 4'b1000;
		6'b1011zz: tRegOutPcStep = 4'b1100;

		6'b11z00z: tRegOutPcStep = 4'b1000;
		6'b11z01z: tRegOutPcStep = 4'b0110;
		6'b11z1zz: tRegOutPcStep = 4'b1100;
	endcase

`else

//	if(icInPcWxe && tPcStepWA)
	if(tInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = tPcStepWB ? 4'b1100 : 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end

`endif

`else
//	if(icInPcWxe && tPcStepWA)
	if(tInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end
`endif
	
//	tRegOutPcOK = tMiss ? UMEM_OK_HOLD : UMEM_OK_OK;

//	if(tResetL)
//	begin
//		tMiss = 0;
//	end

	if(!tReqReady)
	begin
		if(tMemReqLdA && !tMemRespLdA)
			tMissA = 1;
		if(tMemReqLdB && !tMemRespLdB)
			tMissB = 1;
//		tMiss = tMiss1L;
		tMiss = tMissA || tMissB;
	
		tRegOutHold = 1;
	end

	if(tMiss)
		tRegOutExc[15] = 0;

	if(tMiss)
		tRegOutHold = 1;

//	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;


	tReqSeqIdx	= tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa	= tReqSeqVaArr[memSeqIn[3:0]];

	tNxtMemRespLdA	= tMemRespLdA;
	tNxtMemRespLdB	= tMemRespLdB;

	if(memRingIsRespOkLdA && !tResetL)
	begin
//		$display("L1 I$: Resp A, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  Ix=%X VA=%X", tReqSeqIdx, tReqSeqVa);
	
		if(tReqSeqIdx==tReqIxA)
		begin
//	`ifdef jx2_mem_l1inostall
	`ifdef def_true
	// `ifndef def_true
			tStBlkIxA		= tReqSeqIdx;
			tStBlkAddrA		= tReqSeqVa[43:0];
	`else
			tStBlkIxA		= tReqIxA;
			tStBlkAddrA		= tReqAddrA[43:0];
	`endif

			tStBlkPFlA		= 0;
	//		tStBlkPFlA		= { 8'h00, tInPmode, ~(tFlushRov[3:0]) };
			tStBlkPRovA		= tFlushRov;
			tStBlkDataA		= memDataIn;
			tStBlkFlagA		= memOpmIn[3:0];

			if(tStickyTlbExc[15:12]==4'h7)
	//			tStBlkPFlA[15:8]=tStickyTlbExc[119:112];
				tStBlkPFlA[15:0]=tStickyTlbExc[127:112];

			tDoStBlkA		= 1;

			tNxtMemRespLdA	= 1;

			if(memOpmIn[3:2]==2'b11)
	//		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
			begin
				$display("L1I$ Set TLB Inhibit A");
				tNxtTlbMissInh = 1;
			end
		end
		else
		begin
`ifdef def_true
			$display("L1I$: Bad Resp A, A=%X Ix:%X!=%X Rov=%X", 
				memAddrIn, tReqSeqIdx, tReqIxA, tMemSeqRov);
`endif
		end

		if(tReqSeqIdx != tReqIxA)
		begin
			$display("L1 I$, Mismatch Index A, %X!=!X",
				tReqSeqIdx, tReqIxA);
		end
		if(tReqSeqVa[43:0] != tReqAddrA[43:0])
		begin
			$display("L1 I$, Mismatch Index A, %X!=!X",
				tReqSeqVa[43:0], tReqAddrA[43:0]);
		end
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1I$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
	end

	if(memRingIsRespOkLdB && !tResetL)
	begin
//		$display("L1 I$: Resp B, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  Ix=%X VA=%X", tReqSeqIdx, tReqSeqVa);

		if(tReqSeqIdx==tReqIxB)
		begin
//	`ifdef jx2_mem_l1inostall
	`ifdef def_true
	// `ifndef def_true
			tStBlkIxB		= tReqSeqIdx;
			tStBlkAddrB		= tReqSeqVa[43:0];
	`else
			tStBlkIxB		= tReqIxB;
			tStBlkAddrB		= tReqAddrB[43:0];
	`endif

			tStBlkPFlB		= 0;
	//		tStBlkPFlB		= { 8'h00, tInPmode, ~(tFlushRov[3:0]) };
			tStBlkPRovB		= tFlushRov;
			tStBlkDataB		= memDataIn;
			tStBlkFlagB		= memOpmIn[3:0];
			tDoStBlkB		= 1;

			tNxtMemRespLdB	= 1;

			if(tStickyTlbExc[15:12]==4'h7)
	//			tStBlkPFlB[15:8]=tStickyTlbExc[119:112];
				tStBlkPFlB[15:0]=tStickyTlbExc[127:112];

	//		if(memOpmIn[3])
			if(memOpmIn[3:2]==2'b11)
	//		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
			begin
				$display("L1I$ Set TLB Inhibit B");
				tNxtTlbMissInh = 1;
			end
		end
		else
		begin
`ifdef def_true
			$display("L1I$: Bad Resp B, A=%X Ix:%X!=%X Rov=%X", 
				memAddrIn, tReqSeqIdx, tReqIxB, tMemSeqRov);
`endif
		end

		if(tReqSeqIdx != tReqIxB)
		begin
			$display("L1 I$, Mismatch Index B, %X!=!X",
				tReqSeqIdx, tReqIxB);
		end
		if(tReqSeqVa[43:0] != tReqAddrB[43:0])
		begin
			$display("L1 I$, Mismatch Index B, %X!=!X",
				tReqSeqVa[43:0], tReqAddrB[43:0]);
		end
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1I$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
	end

	
	/* Miss Handling */

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UVB1AT_00;
	tMemDataReq		= UVTILE_XX;
	tNxtMemReqLdA	= 0;
	tNxtMemReqLdB	= 0;
	tMemSeqIx		= 0;
	tMemSeqVa		= 0;
	tNxtMemSeqRov	= tMemSeqRov;
//	tNxtMemReqRtcnt	= 255;
//	tNxtMemReqRtcnt	= 4095;
	tNxtMemReqRtcnt	= 1023;

//	tNxtMemTlbInhRtcnt = 1023;
	tNxtMemTlbInhRtcnt = 255;
	
`ifdef def_true
	tReqWaitResp = 0;
	if(tMemReqLdA && !tMemRespLdA)
		tReqWaitResp = 1;
	if(tMemReqLdB && !tMemRespLdB)
		tReqWaitResp = 1;

	if(tReqWaitResp)
		tRegOutHold = 1;
`endif

	if(tResetL)
	begin
		tNxtMemReqLdA	= 0;
		tNxtMemReqLdB	= 0;
		tMemSeqIx		= 0;
		tMemSeqVa		= 0;
		tNxtMemSeqRov	= 0;
	end
	else
//		if((tMissA || tMissB) && tReqReady)
		if(tMissA || tMissB)
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
			begin
				$display("L1 I$ Miss a=%X b=%X Rdy=%X Rt=%x",
					tMissA, tMissB, tReqReady, tMemReqRtcnt);

				$display("  RqA=%X BlkA=%X", tReqAddrA, tBlkAddr2A);
				$display("  RqB=%X BlkB=%X", tReqAddrB, tBlkAddr2B);
			end
			tNxtMsgLatch = 1;
		end

		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;
		tNxtMemSeqRov	= tMemSeqRov;
		tMemWait		= 1;

//		tNxtMemReqRtcnt	= tMemReqRtcnt - 1;
		tNxtMemReqRtcnt	= tMemReqRtcnt - ((tMemReqRtcnt != 0)?1:0);

// `ifndef def_true
`ifdef def_true
		if(tMemReqRtcnt == 0)
		begin
			$display("L1I$: Miss Retry");
//			tNxtMemReqRtcnt	= 255;
			tNxtMemReqRtcnt	= 1023;
			tNxtMemReqLdA	= 0;
			tNxtMemReqLdB	= 0;
		end
`endif

		if(!tReqReady)
		begin
		end
		else
			if(tMissA && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAddrA;
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrA, 4'h00 };
`else
			tMemAddrReq		= { tReqAddrA, 4'h00 };
`endif
			tNxtMemReqLdA = 1;

			if(tSkipTlb)
			begin
//				if(tReqAddrA[43:24]!=0)
				if(tReqAddrIsVirt)
					$display("L1 I$: Send LDA Req Abs A=%X", tReqAddrA);

				if(!tReqAddrIsVirt)
					tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;

				if(tReqAddrIsVirt)
					tMemOpmReq[11:8] = 4'hF;
			end

//			$display("I$ LDA %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxA);
		end
		else if(tMissB && !tMemReqLdB)
		begin

			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAddrB;
			tMemSeqReq		= { unitNodeId, 4'b1100, tMemSeqRov};

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrB, 4'h00 };
`else
			tMemAddrReq		= { tReqAddrB, 4'h00 };
`endif
			tNxtMemReqLdB = 1;

			if(tSkipTlb)
			begin
//				if(tReqAddrB[43:24]!=0)
				if(tReqAddrIsVirt)
					$display("L1 I$: Send LDB Req Abs A=%X", tReqAddrB);

				if(!tReqAddrIsVirt)
					tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;
//				tMemAddrReq[47:44]=4'hC;

				if(tReqAddrIsVirt)
					tMemOpmReq[11:8] = 4'hF;
			end

//			$display("I$ LDB %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxB);
		end
		else
		begin
//			$display("I$ Wait");
		end
	end
`ifndef def_true
	else
	begin
		if(tMemReqLdA || tMemReqLdB)
		begin
			tRegOutHold = 1;
		end
	end
`endif

	if(tDoStBlkA || tDoStBlkB)
	begin
		if(!tRegOutHold)
			$display("L1I$: DoSt");
		tRegOutHold = 1;
	end
	if(tDidStBlkA || tDidStBlkB)
	begin
		if(!tRegOutHold)
			$display("L1I$: DidSt");
		tRegOutHold = 1;
	end

	tDoStallNop = 0;
	
	if((tBlkFlagA[3:2]==2'b11) || (tBlkFlagB[3:2]==2'b11))
//	if(	((tBlkFlagA[3:2]==2'b11) && !tMissSkipA) ||
//		((tBlkFlagB[3:2]==2'b11) && !tMissSkipB) )
	begin
		if(!tMsgLatchTmiss)
			$display("L1I$: Hold TMiss A=%X", tInAddr);
		tNxtMsgLatchTmiss = 1;

		if(tTlbMissInh)
		begin
	//		tRegOutPcVal[15:0] = 16'h3002;
			tDoStallNop = 1;

			tNxtMemTlbInhRtcnt	= tMemTlbInhRtcnt-1;
			if(tMemTlbInhRtcnt==0)
			begin
				$display("L1I$ Timeout Clear TLB Inhibit");
				tNxtTlbMissInh = 0;
				tClrTlbMissInh = 1;
			end
		end
		else
		begin
			tRegOutHold = 1;
		end

	end

//	if(tRegOutPcVal[15:0]==16'h3002)
//	begin
//		tRegOutPcStep = 0;
//	end

//	if((tRegOutPcVal[15:0]==16'h0000) && !tRegOutHold)
	if((tRegOutPcVal[15:0]==16'h0000) && !tRegOutHold && !tDoStallNop)
//	if((tRegOutPcVal[31:0]==32'h0000) && !tRegOutHold && !tDoStallNop)
	begin
		if(!tMsgLatch)
		begin
			$display("L1I$: Op=00 A=%X D=%X Miss=%d/%d",
				tInAddr, tRegOutPcVal, tMissA, tMissB);
			$display("    L1I$: A BA=%X/%X AxH=%X/%X",
				tBlkAddr2A, tReqAddrA,
				tBlkDext2A, tReqAxH);
			$display("    L1I$: B BA=%X/%X AxH=%X/%X",
				tBlkAddr2B, tReqAddrB,
				tBlkDext2B, tReqAxH);
		end
		tNxtMsgLatch = 1;

//		if(!tTlbMissInh && !tResetL)
//			tRegOutHold = 1;
	end
		
	if(tResetL)
		tRegOutHold = 0;

	if(!tRegOutHold)
		tNxtStickyTlbExc[15:12]=0;

`ifdef jx2_mem_l1inostall
//	if(tRegOutHold)
	if(tRegOutHold || (tInAddrNl[15:0]!=tInAddr[15:0]))
	begin
//		$display("L1I$: Hold NoStall A=%X", tInAddr);
//		tRegOutPcVal[15:0] = 16'h3002;
		tDoStallNop = 1;
	end
//	tRegOutHold = 0;
	tRegOutPcOK = UMEM_OK_OK;

//	if(tRegOutHold)
//	begin
//		tNxtIxA=tReqIxA;
//		tNxtIxB=tReqIxB;
//	end

`else
	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;
`endif

	if(tDoStallNop)
	begin
		tRegOutPcVal = 96'h30023002_30023002_30023002;
		if(tInPcRiscv)
			tRegOutPcVal = 96'h00000013_00000013_00000013;
		tRegOutPcStep = 0;
	end

`ifndef def_true
	if(tRegOutHoldL != tRegOutHold)
	begin
		if(tRegOutHold)
			$display("I$ Hold");
		else
			$display("I$ Unhold");
	end
`endif
end

wire		tAdvHold;

always @*
begin
	tStuckTlbMissInh	= 0;

	if(tRegOutHold && !icInPcHold)
	begin
		$display("L1 I$: Hold Mismatch");
	end
	
	if(	tRegOutHold && tReqReady &&
		(	(tMemReqLdA && tMemRespLdA && tMissA) ||
			(tMemReqLdB && tMemRespLdB && tMissB) )
	)
	begin
		$display("L1I$: Stuck Miss %d %d", tMissA, tMissB);
	end

	if(tTlbMissInh)
	begin
		if(tClrTlbMissInhL)
		begin
			if(!tStuckTlbMissInhL)
				$display("L1I$ TLB Inhibit Stuck");
			tStuckTlbMissInh = 1;
		end

//		if(!tStuckTlbMissInh)
		if(1'b1)
		begin
			$display("L1I$ TLB Inhibit, InPc=%X InAddr=%X Clear=%X",
				regInPc, tInAddr, tClrTlbMissInh);
		end
	end
end

// assign		tAdvHold = icInPcHold;
// assign		tAdvHold = tRegOutHold;
assign		tAdvHold = tRegOutHold || icInPcHold;

// assign		tNx2IxA = tRegOutHold ? tReqIxA : tNxtIxA;
// assign		tNx2IxB = tRegOutHold ? tReqIxB : tNxtIxB;

assign		tNx2IxA = tAdvHold ? tReqIxA : tNxtIxA;
assign		tNx2IxB = tAdvHold ? tReqIxB : tNxtIxB;

always @(posedge clock)
begin
//	tTlbMissInh		<= tNxtTlbMissInh;
//	tFlushRov		<= tNxtFlushRov;
//	tAdvFlushRov	<= tNxtAdvFlushRov;

//	tMemPcData		<= memPcData;

	tRegInSr		<= regInSr;
	tRegInSrL		<= tRegInSr;
	tResetL			<= reset;

//	if(!icInPcHold)
//	begin
//		tRegInSr		<= regInSr;
//		tRegInSrL		<= tRegInSr;
//	end

	tRegOutHoldL	<= tRegOutHold;

	tRegOutExc2		<= tRegOutExc;
	tRegOutExc3		<= tRegOutExc2;

	tMsgLatch 		<= tNxtMsgLatch;
	tMsgLatchTmiss	<= tNxtMsgLatchTmiss;

	tMemReqRtcnt	<= tNxtMemReqRtcnt;
	tMemTlbInhRtcnt	<= tNxtMemTlbInhRtcnt;

	tInAddrNl		<= tRegInPc;

	tStickyTlbExc	<= tNxtStickyTlbExc;

	/* Stage A */
`ifdef jx2_mem_l1inostall
// `ifndef jx2_mem_l1inostall
//	if(!tRegOutHold)
	if(!tAdvHold)
`else
	if(1'b1)
`endif
	begin
		tInAddr			<= tRegInPc;
		tReqAddrA		<= tNxtAddrA;
		tReqAddrB		<= tNxtAddrB;
		tReqAddrHi		<= tNxtAddrHi;
		tReqAxH			<= tNxtAxH;
		tReqIxA			<= tNxtIxA;
		tReqIxB			<= tNxtIxB;
		tSkipTlb		<= tNxtSkipTlb;
		tSkipMiss		<= tNxtSkipMiss;

//		tTlbMissInh		<= tNxtTlbMissInh;
		tFlushRov		<= tNxtFlushRov;
		tAdvFlushRov	<= tNxtAdvFlushRov;

		tInPmode		<= tNxtInPmode;
//		tInPcWxe		<= icInPcWxe;
//		tInPcRiscv		<= (icInPcWxm == 2'b01);

		tInPcWxe		<= tNxtInPcWxe;
		tInPcRiscv		<= tNxtInPcRiscv;

`ifdef jx2_enable_xg2mode
		tInPcXG2		<= tNxtInPcXG2;
`else
		tInPcXG2		<= 0;
`endif

	end
	else
	begin
//		tTlbMissInh		<= (tTlbMissInh || tNxtTlbMissInh);
	end

	tTlbMissInh		<= (tAdvHold ? tTlbMissInh : 0) || tNxtTlbMissInh;
//	tTlbMissInh		<= ((tAdvHold ? tTlbMissInh : 0) || tNxtTlbMissInh) && 
//						!tClrTlbMissInh;
	tClrTlbMissInhL		<= tClrTlbMissInh;
	tStuckTlbMissInhL	<= tStuckTlbMissInh;

	tReqAddrAL	<= tReqAddrA;
	tReqAddrBL	<= tReqAddrB;
	tReqIxAL	<= tReqIxA;
	tReqIxBL	<= tReqIxB;
	tMissAL		<= tMissA;
	tMissBL		<= tMissB;
	tMissL		<= tMiss;
	tFlushAL	<= tFlushA;
	tFlushBL	<= tFlushB;

//	tMissA1L	<= tRegOutHold ? tMissA : 0;
//	tMissB1L	<= tRegOutHold ? tMissB : 0;
//	tMiss1L		<= tRegOutHold ? tMiss : 0;

	tMissA1L	<= tAdvHold ? tMissA : 0;
	tMissB1L	<= tAdvHold ? tMissB : 0;
	tMiss1L		<= tAdvHold ? tMiss : 0;

	tInOpm		<= tInPcOpm;
	tInOpmB		<= tInOpm;
	tInOpmC		<= tInOpmB;
	icDoFlush	<= icNxtDoFlush;

//	{ tBlkPFlA, tBlkPRovA, tBlkDataA }	<= icCaMemA[tNx2IxA];
//	{ tBlkPFlB, tBlkPRovB, tBlkDataB }	<= icCaMemB[tNx2IxB];
//	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
//	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];

	{ tBlkDextA, tBlkDataA }	<= icCaMemA[tNx2IxA];
	{ tBlkDextB, tBlkDataB }	<= icCaMemB[tNx2IxB];
	{ tBlkPFlA, tBlkPRovA, tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
	{ tBlkPFlB, tBlkPRovB, tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];

	tNx2IxAL	<= tNx2IxA;
	tNx2IxBL	<= tNx2IxB;

	if(tDoStBlkA)
	begin
//		$display("L1I$ DoStBlkA Ix=%X A=%X D=%X",
//			tStBlkIxA, tStBlkAddrA, tStBlkDataA);

//		icCaMemA[tStBlkIxA]		<= { tStBlkPFlA, tStBlkPRovA, tStBlkDataA };
//		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };

		icCaMemA[tStBlkIxA]		<= { tStBlkDextA, tStBlkDataA };
		icCaAddrA[tStBlkIxA]	<= {
			tStBlkPFlA, tStBlkPRovA,
			tStBlkFlagA, tStBlkAddrA };
	end

	if(tDoStBlkB)
	begin
//		$display("L1I$ DoStBlkB Ix=%X A=%X D=%X",
//			tStBlkIxB, tStBlkAddrB, tStBlkDataB);

//		icCaMemB[tStBlkIxB]		<= { tStBlkPFlB, tStBlkPRovB, tStBlkDataB };
//		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };

		icCaMemB[tStBlkIxB]		<= { tStBlkDextB, tStBlkDataB };
		icCaAddrB[tStBlkIxB]	<= {
			tStBlkPFlB, tStBlkPRovB,
			tStBlkFlagB, tStBlkAddrB };
	end

	tDidStBlkA		<= tDoStBlkA;
	tDidStBlkB		<= tDoStBlkB;

	/* Stage B */

	if(tResetL)
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

//		tMemReqStA	<= 0;
//		tMemReqStB	<= 0;
		tMemReqLdA	<= 0;
		tMemReqLdB	<= 0;
//		tMemReqLdM	<= 0;
		tMemSeqRov	<= 0;
	end
	else
		if(memRingIsIdle || memRingIsResp)
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqSent	<= 1;

`ifdef def_true
		tMemReqLdA	<= tAdvHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= tAdvHold ? tNxtMemReqLdB : 0;
		tMemRespLdA	<= tAdvHold ? tNxtMemRespLdA : 0;
		tMemRespLdB	<= tAdvHold ? tNxtMemRespLdB : 0;
`endif

`ifndef def_true
//		tMemReqStA	<= tRegOutHold ? tNxtMemReqStA : 0;
//		tMemReqStB	<= tRegOutHold ? tNxtMemReqStB : 0;
		tMemReqLdA	<= tRegOutHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tNxtMemReqLdB : 0;
		tMemRespLdA	<= tRegOutHold ? tNxtMemRespLdA : 0;
		tMemRespLdB	<= tRegOutHold ? tNxtMemRespLdB : 0;
`endif

`ifndef def_true
//		tMemReqStA	<= tNxtMemReqStA;
//		tMemReqStB	<= tNxtMemReqStB;
		tMemReqLdA	<= tNxtMemReqLdA;
		tMemReqLdB	<= tNxtMemReqLdB;
//		tMemReqLdM	<= tNxtMemReqLdM;
`endif
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

`ifdef def_true
		tMemReqLdA	<= tAdvHold ? tMemReqLdA : 0;
		tMemReqLdB	<= tAdvHold ? tMemReqLdB : 0;
		tMemRespLdA	<= tAdvHold ? tMemRespLdA : 0;
		tMemRespLdB	<= tAdvHold ? tMemRespLdB : 0;
`endif

`ifndef def_true
		tMemReqLdA	<= tRegOutHold ? tMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tMemReqLdB : 0;
		tMemRespLdA	<= tRegOutHold ? tNxtMemRespLdA : 0;
		tMemRespLdB	<= tRegOutHold ? tNxtMemRespLdB : 0;
`endif
	end

end

endmodule

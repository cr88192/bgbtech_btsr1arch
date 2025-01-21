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

`define jx2_memic_getoplen
// `define jx2_memic_blklenbits

`include "ringbus/RbiDefs.v"

`include "ringbus/MemIc_GetOpLen.v"

`ifdef jx2_dec_ssc_riscv

`include "DecRvSscFlag.v"
`include "DecRvSscChkReg.v"

`endif

`ifdef jx2_enable_riscv_xg3
// `include "DecOpRepXG3.v"
`endif

module RbiMemIcWxA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,	icOutPcLow,
	regInPcInc,

	icInPcHold,		icInPcWxm,
	icInPcOpm,		regInSr,
	icMemWait,		regOutExc,
	regOutPcSxo,	regKrrHash,
	tlbExc,			icExecAcl,
	regInMmcr,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

`input_vaddr	regInPc;		//input PC address
`input_vaddr	regInPcInc;		//input PC address
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

input[63:0]		regInMmcr;

reg[63:0]		tRegInMmcr;


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

`ifdef jx2_memic_blklenbits
`define	jx2_mem_l1i_regicaddr	reg[107:0]
`else
`define	jx2_mem_l1i_regicaddr	reg[ 71:0]
`endif

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
	`jx2_mem_l1i_regicaddr		icCaAddrA[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`jx2_mem_l1i_regicaddr		icCaAddrB[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`jx2_mem_l1i_regicaddr		icCaAddrC[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`jx2_mem_l1i_regicaddr		icCaAddrD[127:0];	//Local L1 tile address

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
(* max_fanout = 200 *)
	reg[43:0]		tBlkAddrA;
(* max_fanout = 200 *)
	reg[43:0]		tBlkAddrB;
(* max_fanout = 200 *)
	reg[43:0]		tBlkAddrC;
(* max_fanout = 200 *)
	reg[43:0]		tBlkAddrD;
(* max_fanout = 200 *)
	reg[43:0]		tBlkAddr2A;
(* max_fanout = 200 *)
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

reg[3:0]		tBlkPFxA;
reg[3:0]		tBlkPFxB;
reg[31:0]		tBlkPLenA;
reg[31:0]		tBlkPLenB;

reg[3:0]		tReqPFx;
reg[3:0]		tNxtReqPFx;

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
`reg_vaddr		tRegInPcInc;	//input PC address
reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg[5:0]		tInPcOpm;		//OPM (Used for cache-control)
reg				tInPcWxe;
reg				tInPcXG2;
reg				tInPcXG3;
reg				tInPcRiscv;
reg				tNxtInPcWxe;
reg				tNxtInPcXG2;
reg				tNxtInPcXG3;
reg				tNxtInPcRiscv;

reg[3:0]		tInPmode;
reg[3:0]		tNxtInPmode;

(* max_fanout = 200 *)
	reg				tTlbMissInh;

reg				tNxtTlbMissInh;
reg				tClrTlbMissInh;
reg				tClrTlbMissInhL;

(* max_fanout = 400 *)
	reg			tSkipTlb;

(* max_fanout = 400 *)
	reg			tNxtSkipTlb;
(* max_fanout = 400 *)
	reg			tSkipMiss;

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
reg				tPcStepJWA;

//reg				tPcStepWA0;
//reg				tPcStepWB0;
//reg				tPcStepJA0;
//reg				tPcStepBA0;
//reg				tPcStepBB0;

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

reg				tNxtDoFlushTlb;
reg				tDoFlushTlb;
reg				tDoFlushTlbL;

reg[43:0]		tRegInPcP0;
reg[43:0]		tRegInPcP1;
// reg[14:0]		tRegInPcP1L;
// reg[14:0]		tRegInPcP1H;

// reg[12:0]		tRegInPcP1L;
// reg[12:0]		tRegInPcP1H;

reg[8:0]		tRegInPcP1L;
reg[8:0]		tRegInPcP1M;
reg[8:0]		tRegInPcP1H;

reg				tRegInPcP1L_C;
reg				tRegInPcP1M_C;
reg				tRegInPcP1H_C;

reg[7:0]		tFlushRov;
reg[7:0]		tNxtFlushRov;
reg				tAdvFlushRov;
reg				tNxtAdvFlushRov;

reg[3:0]		tFlushRovTlb;
reg[3:0]		tNxtFlushRovTlb;


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
reg				tMemRespLdAN;		//Load A (Inverted)
reg				tMemRespLdBN;		//Load B (Inverted)

reg				tReqWaitResp;




// reg[ 7:0]		tMemReqRtcnt;
// reg[ 7:0]		tNxtMemReqRtcnt;
reg[11:0]		tMemReqRtcnt;
reg[11:0]		tNxtMemReqRtcnt;

reg[11:0]		tMemTlbInhRtcnt;
reg[11:0]		tNxtMemTlbInhRtcnt;

reg[11:0]		tMemHeldCyc;
reg[11:0]		tNxtMemHeldCyc;

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;


(* max_fanout = 200 *)
	reg				tRegOutHold;
(* max_fanout = 200 *)
	reg				tRegOutHoldL;
(* max_fanout = 200 *)
	reg				tReqReady;

(* max_fanout = 200 *)
	reg				tReqReadyN;

reg				tBlkIsSxo;
reg				tBlkDoRiscV;
reg				tBlkDoRvC;
reg				tBlkDoRvW;
reg				tDoStallNop;

reg				tCombJWA;

reg				tMsgLatch;
reg				tNxtMsgLatch;

reg				tMsgLatchTmiss;
reg				tNxtMsgLatchTmiss;

reg				tMsgLatchNop;
reg				tNxtMsgLatchNop;

reg				tMsgLatchTinh;
reg				tNxtMsgLatchTinh;

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

reg[3:0]		tStBlkPFxA;
reg[3:0]		tStBlkPFxB;
reg[31:0]		tStBlkPLenA;
reg[31:0]		tStBlkPLenB;


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

reg[127:0]		tTlbExc;
reg[127:0]		tStickyTlbExc;
reg[127:0]		tNxtStickyTlbExc;


reg[95:0]		tUtlbArr[15:0];
reg[95:0]		tUtlbStAddr;
reg[3:0]		tUtlbStIx;
reg				tUtlbDoSt;

reg[95:0]		tUtlbBlkAddr;
reg[3:0]		tUtlbBlkIx;
reg[3:0]		tUtlb1BlkIx;
reg[3:0]		tUtlb1BlkIxL;
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

(* max_fanout = 200 *)
	reg				tResetL;

//	tNxtInPcXG2		= tInPcXG2;
//	tNxtInPcRiscv	= tInPcRiscv;

// wire	icInPcRiscv	= (icInPcWxm[1:0] == 2'b01) && !icInPcXG2;
// wire	tReqPcRiscv	= (tReqPFx[1:0] == 2'b01) && !icInPcXG2;

`ifdef jx2_memic_blklenbits

`ifdef jx2_dec_ssc_riscv

wire[15:0]	memDataSsc;

DecRvSscFlag	rvssc_p0(
	memDataIn[ 31: 0], memDataSsc[ 3: 0],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p1(
	memDataIn[ 63:32], memDataSsc[ 7: 4],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p2(
	memDataIn[ 95:64], memDataSsc[11: 8],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p3(
	memDataIn[127:96], memDataSsc[15:12],
	tInPcXG2, tInPcRiscv, tInPcWxe);

`endif

wire[31:0]	memDataLens;
MemIc_GetOpLen	get_oplen_p0(
	memDataIn[ 15:  0], memDataLens[ 3: 0],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p1(
	memDataIn[ 31: 16], memDataLens[ 7: 4],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p2(
	memDataIn[ 47: 32], memDataLens[11: 8],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p3(
	memDataIn[ 63: 48], memDataLens[15:12],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p4(
	memDataIn[ 79: 64], memDataLens[19:16],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p5(
	memDataIn[ 95: 80], memDataLens[23:20],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p6(
	memDataIn[111: 96], memDataLens[27:24],
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_p7(
	memDataIn[127:112], memDataLens[31:28],
	tInPcXG2, tInPcRiscv, tInPcWxe);


wire[3:0]	tOpGetLen_B0a = tBlkPLenA[ 3: 0];
wire[3:0]	tOpGetLen_B1a = tBlkPLenA[ 7: 4];
wire[3:0]	tOpGetLen_B2a = tBlkPLenA[11: 8];
wire[3:0]	tOpGetLen_B3a = tBlkPLenA[15:12];
wire[3:0]	tOpGetLen_B4a = tBlkPLenA[19:16];
wire[3:0]	tOpGetLen_B5a = tBlkPLenA[23:20];
wire[3:0]	tOpGetLen_B6a = tBlkPLenA[27:24];
wire[3:0]	tOpGetLen_B7a = tBlkPLenA[31:28];

wire[3:0]	tOpGetLen_B0b = tBlkPLenB[ 3: 0];
wire[3:0]	tOpGetLen_B1b = tBlkPLenB[ 7: 4];
wire[3:0]	tOpGetLen_B2b = tBlkPLenB[11: 8];
wire[3:0]	tOpGetLen_B3b = tBlkPLenB[15:12];
wire[3:0]	tOpGetLen_B4b = tBlkPLenB[19:16];
wire[3:0]	tOpGetLen_B5b = tBlkPLenB[23:20];
wire[3:0]	tOpGetLen_B6b = tBlkPLenB[27:24];
wire[3:0]	tOpGetLen_B7b = tBlkPLenB[31:28];

`ifndef def_true
wire[3:0]	tOpGetLen_A0;
wire[3:0]	tOpGetLen_A1;
wire[3:0]	tOpGetLen_A2;
wire[3:0]	tOpGetLen_A3;
wire[3:0]	tOpGetLen_A4;
wire[3:0]	tOpGetLen_A5;

assign	tOpGetLen_A0 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B4b : tOpGetLen_B0b) :
		(tInAddr[3] ? tOpGetLen_B4a : tOpGetLen_B0a) ;
assign	tOpGetLen_A1 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B5b : tOpGetLen_B1b) :
		(tInAddr[3] ? tOpGetLen_B5a : tOpGetLen_B1a) ;
assign	tOpGetLen_A2 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B6b : tOpGetLen_B2b) :
		(tInAddr[3] ? tOpGetLen_B6a : tOpGetLen_B2a) ;
assign	tOpGetLen_A3 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B7b : tOpGetLen_B3b) :
		(tInAddr[3] ? tOpGetLen_B7a : tOpGetLen_B3a) ;
assign	tOpGetLen_A4 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B0a : tOpGetLen_B4b) :
		(tInAddr[3] ? tOpGetLen_B0b : tOpGetLen_B4a) ;
assign	tOpGetLen_A5 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B1a : tOpGetLen_B5b) :
		(tInAddr[3] ? tOpGetLen_B1b : tOpGetLen_B5a) ;

wire[3:0]	tOpGetLen_W0;
wire[3:0]	tOpGetLen_W1;

assign	tOpGetLen_W0 =
	tInAddr[2] ? 
		(tInAddr[1] ? tOpGetLen_A3 : tOpGetLen_A2) :
		(tInAddr[1] ? tOpGetLen_A1 : tOpGetLen_A0) ;
assign	tOpGetLen_W1 =
	tInAddr[2] ? 
		(tInAddr[1] ? tOpGetLen_A5 : tOpGetLen_A4) :
		(tInAddr[1] ? tOpGetLen_A3 : tOpGetLen_A2) ;

`endif

`ifdef def_true

reg[3:0]	tOpGetLen_W0;
reg[3:0]	tOpGetLen_W1;
always @*
begin
	case(tInAddr[4:1])
		4'h0: begin
			tOpGetLen_W0 = tOpGetLen_B0a;
			tOpGetLen_W1 = tOpGetLen_B2a;
		end
		4'h1: begin
			tOpGetLen_W0 = tOpGetLen_B1a;
			tOpGetLen_W1 = tOpGetLen_B3a;
		end
		4'h2: begin
			tOpGetLen_W0 = tOpGetLen_B2a;
			tOpGetLen_W1 = tOpGetLen_B4a;
		end
		4'h3: begin
			tOpGetLen_W0 = tOpGetLen_B3a;
			tOpGetLen_W1 = tOpGetLen_B5a;
		end
		4'h4: begin
			tOpGetLen_W0 = tOpGetLen_B4a;
			tOpGetLen_W1 = tOpGetLen_B6a;
		end
		4'h5: begin
			tOpGetLen_W0 = tOpGetLen_B5a;
			tOpGetLen_W1 = tOpGetLen_B7a;
		end
		4'h6: begin
			tOpGetLen_W0 = tOpGetLen_B6a;
			tOpGetLen_W1 = tOpGetLen_B0b;
		end
		4'h7: begin
			tOpGetLen_W0 = tOpGetLen_B7a;
			tOpGetLen_W1 = tOpGetLen_B1b;
		end
		4'h8: begin
			tOpGetLen_W0 = tOpGetLen_B0b;
			tOpGetLen_W1 = tOpGetLen_B2b;
		end
		4'h9: begin
			tOpGetLen_W0 = tOpGetLen_B1b;
			tOpGetLen_W1 = tOpGetLen_B3b;
		end
		4'hA: begin
			tOpGetLen_W0 = tOpGetLen_B2b;
			tOpGetLen_W1 = tOpGetLen_B4b;
		end
		4'hB: begin
			tOpGetLen_W0 = tOpGetLen_B3b;
			tOpGetLen_W1 = tOpGetLen_B5b;
		end
		4'hC: begin
			tOpGetLen_W0 = tOpGetLen_B4b;
			tOpGetLen_W1 = tOpGetLen_B6b;
		end
		4'hD: begin
			tOpGetLen_W0 = tOpGetLen_B5b;
			tOpGetLen_W1 = tOpGetLen_B7b;
		end
		4'hE: begin
			tOpGetLen_W0 = tOpGetLen_B6b;
			tOpGetLen_W1 = tOpGetLen_B0a;
		end
		4'hF: begin
			tOpGetLen_W0 = tOpGetLen_B7b;
			tOpGetLen_W1 = tOpGetLen_B1a;
		end
	endcase
end

`endif

`else

`ifndef def_true

wire[255:0]		tBlkDataCMux0;
wire[191:0]		tBlkDataCMux;
// assign	tBlkDataCMux0 = tInAddr[4] ?
//	{ tBlkDataA, tBlkDataB } :
//	{ tBlkDataB, tBlkDataA } ;
// assign	tBlkDataCMux = tInAddr[3] ?
//	tBlkDataCMux0[255:64] :
//	tBlkDataCMux0[191: 0] ;

assign	tBlkDataCMux = tInAddr[4] ?
	( tInAddr[3] ?
		{ tBlkDataA[127:0], tBlkDataB[127:64] } :
		{ tBlkDataA[ 63:0], tBlkDataB[127: 0] } ):
	( tInAddr[3] ?
		{ tBlkDataB[127:0], tBlkDataA[127:64] } :
		{ tBlkDataB[ 63:0], tBlkDataA[127: 0] } ) ;

wire[3:0]	tOpGetLen_A0;
wire[3:0]	tOpGetLen_A1;
wire[3:0]	tOpGetLen_A2;
wire[3:0]	tOpGetLen_A3;
wire[3:0]	tOpGetLen_A4;
wire[3:0]	tOpGetLen_A5;

MemIc_GetOpLen	get_oplen_a0(
	tBlkDataCMux[15: 0], tOpGetLen_A0,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a1(
	tBlkDataCMux[31:16], tOpGetLen_A1,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a2(
	tBlkDataCMux[47:32], tOpGetLen_A2,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a3(
	tBlkDataCMux[63:48], tOpGetLen_A3,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a4(
	tBlkDataCMux[79:64], tOpGetLen_A4,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a5(
	tBlkDataCMux[95:80], tOpGetLen_A5,
	tInPcXG2, tInPcRiscv, tInPcWxe);

`endif

`ifndef def_true
//`ifdef def_true

wire[255:0]		tBlkDataCMux0;
assign	tBlkDataCMux0 = tInAddr[4] ?
	{ tBlkDataA, tBlkDataB } :
	{ tBlkDataB, tBlkDataA } ;

wire[3:0]	tOpGetLen_B0;
wire[3:0]	tOpGetLen_B1;
wire[3:0]	tOpGetLen_B2;
wire[3:0]	tOpGetLen_B3;
wire[3:0]	tOpGetLen_B4;
wire[3:0]	tOpGetLen_B5;
wire[3:0]	tOpGetLen_B6;
wire[3:0]	tOpGetLen_B7;
wire[3:0]	tOpGetLen_B8;
wire[3:0]	tOpGetLen_B9;
// wire[3:0]	tOpGetLen_B10;
// wire[3:0]	tOpGetLen_B11;

MemIc_GetOpLen	get_oplen_a0(
	tBlkDataCMux0[ 15:  0], tOpGetLen_B0,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a1(
	tBlkDataCMux0[ 31: 16], tOpGetLen_B1,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a2(
	tBlkDataCMux0[ 47: 32], tOpGetLen_B2,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a3(
	tBlkDataCMux0[ 63: 48], tOpGetLen_B3,
	tInPcXG2, tInPcRiscv, tInPcWxe);

MemIc_GetOpLen	get_oplen_a4(
	tBlkDataCMux0[ 79: 64], tOpGetLen_B4,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a5(
	tBlkDataCMux0[ 95: 80], tOpGetLen_B5,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a6(
	tBlkDataCMux0[111: 96], tOpGetLen_B6,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a7(
	tBlkDataCMux0[127:112], tOpGetLen_B7,
	tInPcXG2, tInPcRiscv, tInPcWxe);

MemIc_GetOpLen	get_oplen_a8(
	tBlkDataCMux0[143:128], tOpGetLen_B8,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a9(
	tBlkDataCMux0[159:144], tOpGetLen_B9,
	tInPcXG2, tInPcRiscv, tInPcWxe);
// MemIc_GetOpLen	get_oplen_a10(
//	tBlkDataCMux0[175:160], tOpGetLen_B10,
//	tInPcXG2, tInPcRiscv, tInPcWxe);
// MemIc_GetOpLen	get_oplen_a11(
//	tBlkDataCMux0[191:176], tOpGetLen_B11,
//	tInPcXG2, tInPcRiscv, tInPcWxe);

wire[3:0]	tOpGetLen_A0;
wire[3:0]	tOpGetLen_A1;
wire[3:0]	tOpGetLen_A2;
wire[3:0]	tOpGetLen_A3;
wire[3:0]	tOpGetLen_A4;
wire[3:0]	tOpGetLen_A5;

assign	tOpGetLen_A0 = tInAddr[3] ? tOpGetLen_B4 : tOpGetLen_B0;
assign	tOpGetLen_A1 = tInAddr[3] ? tOpGetLen_B5 : tOpGetLen_B1;
assign	tOpGetLen_A2 = tInAddr[3] ? tOpGetLen_B6 : tOpGetLen_B2;
assign	tOpGetLen_A3 = tInAddr[3] ? tOpGetLen_B7 : tOpGetLen_B3;
assign	tOpGetLen_A4 = tInAddr[3] ? tOpGetLen_B8 : tOpGetLen_B4;
assign	tOpGetLen_A5 = tInAddr[3] ? tOpGetLen_B9 : tOpGetLen_B5;

`endif


`ifdef def_true
// `ifndef def_true

//wire[255:0]		tBlkDataCMux0;
//assign	tBlkDataCMux0 = tInAddr[4] ?
//	{ tBlkDataA, tBlkDataB } :
//	{ tBlkDataB, tBlkDataA } ;

wire[3:0]	tOpGetLen_B0a;
wire[3:0]	tOpGetLen_B1a;
wire[3:0]	tOpGetLen_B2a;
wire[3:0]	tOpGetLen_B3a;
wire[3:0]	tOpGetLen_B4a;
wire[3:0]	tOpGetLen_B5a;
wire[3:0]	tOpGetLen_B6a;
wire[3:0]	tOpGetLen_B7a;

wire[3:0]	tOpGetLen_B0b;
wire[3:0]	tOpGetLen_B1b;
wire[3:0]	tOpGetLen_B2b;
wire[3:0]	tOpGetLen_B3b;
wire[3:0]	tOpGetLen_B4b;
wire[3:0]	tOpGetLen_B5b;
wire[3:0]	tOpGetLen_B6b;
wire[3:0]	tOpGetLen_B7b;


MemIc_GetOpLen	get_oplen_a0a(
	tBlkDataA[ 15:  0], tOpGetLen_B0a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a1a(
	tBlkDataA[ 31: 16], tOpGetLen_B1a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a2a(
	tBlkDataA[ 47: 32], tOpGetLen_B2a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a3a(
	tBlkDataA[ 63: 48], tOpGetLen_B3a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a4a(
	tBlkDataA[ 79: 64], tOpGetLen_B4a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a5a(
	tBlkDataA[ 95: 80], tOpGetLen_B5a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a6a(
	tBlkDataA[111: 96], tOpGetLen_B6a,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a7a(
	tBlkDataA[127:112], tOpGetLen_B7a,
	tInPcXG2, tInPcRiscv, tInPcWxe);

MemIc_GetOpLen	get_oplen_a0b(
	tBlkDataB[ 15:  0], tOpGetLen_B0b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a1b(
	tBlkDataB[ 31: 16], tOpGetLen_B1b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a2b(
	tBlkDataB[ 47: 32], tOpGetLen_B2b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a3b(
	tBlkDataB[ 63: 48], tOpGetLen_B3b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a4b(
	tBlkDataB[ 79: 64], tOpGetLen_B4b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a5b(
	tBlkDataB[ 95: 80], tOpGetLen_B5b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a6b(
	tBlkDataB[111: 96], tOpGetLen_B6b,
	tInPcXG2, tInPcRiscv, tInPcWxe);
MemIc_GetOpLen	get_oplen_a7b(
	tBlkDataB[127:112], tOpGetLen_B7b,
	tInPcXG2, tInPcRiscv, tInPcWxe);

wire[3:0]	tOpGetLen_A0;
wire[3:0]	tOpGetLen_A1;
wire[3:0]	tOpGetLen_A2;
wire[3:0]	tOpGetLen_A3;
wire[3:0]	tOpGetLen_A4;
wire[3:0]	tOpGetLen_A5;

assign	tOpGetLen_A0 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B4b : tOpGetLen_B0b) :
		(tInAddr[3] ? tOpGetLen_B4a : tOpGetLen_B0a) ;
assign	tOpGetLen_A1 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B5b : tOpGetLen_B1b) :
		(tInAddr[3] ? tOpGetLen_B5a : tOpGetLen_B1a) ;
assign	tOpGetLen_A2 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B6b : tOpGetLen_B2b) :
		(tInAddr[3] ? tOpGetLen_B6a : tOpGetLen_B2a) ;
assign	tOpGetLen_A3 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B7b : tOpGetLen_B3b) :
		(tInAddr[3] ? tOpGetLen_B7a : tOpGetLen_B3a) ;
assign	tOpGetLen_A4 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B0a : tOpGetLen_B4b) :
		(tInAddr[3] ? tOpGetLen_B0b : tOpGetLen_B4a) ;
assign	tOpGetLen_A5 =
	tInAddr[4] ? 
		(tInAddr[3] ? tOpGetLen_B1a : tOpGetLen_B5b) :
		(tInAddr[3] ? tOpGetLen_B1b : tOpGetLen_B5a) ;

`endif


wire[3:0]	tOpGetLen_W0;
wire[3:0]	tOpGetLen_W1;

assign	tOpGetLen_W0 =
	tInAddr[2] ? 
		(tInAddr[1] ? tOpGetLen_A3 : tOpGetLen_A2) :
		(tInAddr[1] ? tOpGetLen_A1 : tOpGetLen_A0) ;
assign	tOpGetLen_W1 =
	tInAddr[2] ? 
		(tInAddr[1] ? tOpGetLen_A5 : tOpGetLen_A4) :
		(tInAddr[1] ? tOpGetLen_A3 : tOpGetLen_A2) ;

`endif


`ifdef jx2_dec_ssc_riscv

wire[15:0]	blkDataSscA;
wire[15:0]	blkDataSscB;

DecRvSscFlag	rvssc_p0a(
	tBlkDataA[ 31: 0], blkDataSscA[ 3: 0],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p1a(
	tBlkDataA[ 63:32], blkDataSscA[ 7: 4],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p2a(
	tBlkDataA[ 95:64], blkDataSscA[11: 8],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p3a(
	tBlkDataA[127:96], blkDataSscA[15:12],
	tInPcXG2, tInPcRiscv, tInPcWxe);

DecRvSscFlag	rvssc_p0b(
	tBlkDataB[ 31: 0], blkDataSscB[ 3: 0],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p1b(
	tBlkDataB[ 63:32], blkDataSscB[ 7: 4],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p2b(
	tBlkDataB[ 95:64], blkDataSscB[11: 8],
	tInPcXG2, tInPcRiscv, tInPcWxe);
DecRvSscFlag	rvssc_p3b(
	tBlkDataB[127:96], blkDataSscB[15:12],
	tInPcXG2, tInPcRiscv, tInPcWxe);

wire[15:0]	blkDataSscRegA;
wire[15:0]	blkDataSscRegB;

DecRvSscChkReg	rvssc_q0a(
	tBlkDataA[ 31: 0], tBlkDataA[ 63:32], blkDataSscRegA[ 3: 0]);
DecRvSscChkReg	rvssc_q1a(
	tBlkDataA[ 63:32], tBlkDataA[ 95:64], blkDataSscRegA[ 7: 4]);
DecRvSscChkReg	rvssc_q2a(
	tBlkDataA[ 95:64], tBlkDataA[127:96], blkDataSscRegA[11: 8]);
DecRvSscChkReg	rvssc_q3a(
	tBlkDataA[127:96], tBlkDataB[ 31: 0], blkDataSscRegA[15:12]);

DecRvSscChkReg	rvssc_q0b(
	tBlkDataB[ 31: 0], tBlkDataB[ 63:32], blkDataSscRegB[ 3: 0]);
DecRvSscChkReg	rvssc_q1b(
	tBlkDataB[ 63:32], tBlkDataB[ 95:64], blkDataSscRegB[ 7: 4]);
DecRvSscChkReg	rvssc_q2b(
	tBlkDataB[ 95:64], tBlkDataB[127:96], blkDataSscRegB[11: 8]);
DecRvSscChkReg	rvssc_q3b(
	tBlkDataB[127:96], tBlkDataA[ 31: 0], blkDataSscRegB[15:12]);

wire[3:0]	selBlkDataSsc0 =
	tInAddr[4] ? 
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscB[15:12] : blkDataSscB[11:8]) :
			(tInAddr[2] ? blkDataSscB[ 7: 4] : blkDataSscB[ 3:0]) ) :
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscA[15:12] : blkDataSscA[11:8]) :
			(tInAddr[2] ? blkDataSscA[ 7: 4] : blkDataSscA[ 3:0]) ) ;

wire[3:0]	selBlkDataSsc1 =
	tInAddr[4] ? 
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscA[ 3: 0] : blkDataSscB[15:12]) :
			(tInAddr[2] ? blkDataSscB[11: 8] : blkDataSscB[ 7: 4]) ) :
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscB[ 3: 0] : blkDataSscA[15:12]) :
			(tInAddr[2] ? blkDataSscA[11: 8] : blkDataSscA[ 7: 4]) ) ;

wire[3:0]	selBlkDataSscReg =
	tInAddr[4] ? 
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscRegB[15:12] : blkDataSscRegB[11:8]) :
			(tInAddr[2] ? blkDataSscRegB[ 7: 4] : blkDataSscRegB[ 3:0]) ) :
		(tInAddr[3] ? 
			(tInAddr[2] ? blkDataSscRegA[15:12] : blkDataSscRegA[11:8]) :
			(tInAddr[2] ? blkDataSscRegA[ 7: 4] : blkDataSscRegA[ 3:0]) ) ;

wire blkDataRvSscEna =
	selBlkDataSsc0[2] && selBlkDataSsc1[0] &&
	(tInAddr[1:0] == 2'b00) &&
	(selBlkDataSscReg[1:0]==2'b00);

`endif


always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tClrTlbMissInh		= 0;
	tRegOutHold			= 0;
	tMemWait			= 0;
	tRegOutExc			= 0;
	tNxtMsgLatch		= 0;
	tNxtMsgLatchTmiss	= 0;
	tNxtMsgLatchNop		= 0;
	tNxtMsgLatchTinh	= 0;
	tNxtSkipTlb			= 0;
	tNxtSkipMiss		= 0;
	
	tNxtStickyTlbExc	= tStickyTlbExc;
//	if(tlbExc[15:12]==4'h7)
//		tNxtStickyTlbExc	= tlbExc;
	if(tTlbExc[15:12]==4'h7)
		tNxtStickyTlbExc	= tTlbExc;

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
`ifdef jx2_debug_isr
			$display("L1I$ Clear TLB Inhibit");
//			$display("L1I$ Clear TLB Inhibit, InPc=%X InAddr=%X",
//				regInPc, tInAddr);
`endif

		tNxtTlbMissInh	= 0;
		tClrTlbMissInh	= 1;
//		tSkipTlb		= 1;
	end
	
	tNxtUtlbBlkIx	= regInPc[15:12] ^ regInPc[19:16];

	/* Stage A */

//	tNxtSkipTlb			= regInSr[29] && regInSr[30];
	tNxtSkipTlb			= tRegInSr[29] && tRegInSr[30];
//	tNxtSkipMiss		= tRegInSr[29] != regInSr[29];
	tNxtSkipMiss		= !tRegInSr[29] && regInSr[29];

//	if(disableTlb)
//		tNxtSkipTlb = 1;

	if(!tRegInMmcr[0])
		tNxtSkipTlb = 1;

	if(tInAddr[47:44]==4'hC)
		tNxtSkipTlb = 1;
	if(tInAddr[47:44]==4'hD)
		tNxtSkipTlb = 1;

// `ifndef def_true
//`ifdef def_true
`ifdef jx2_debug_isr
//	if(tNxtSkipTlb && (regInPc[47:28]!=0) && !icInPcHold)
//	if(tNxtSkipTlb && (regInPc[47:28]!=0))
`ifdef jx2_enable_vaddr48
	if(tSkipTlb && (tInAddr[47:28]!=0))
`else
	if(tSkipTlb && (tInAddr[31:28]!=0))
`endif
	begin
		$display("L1 I$: Next Skip TLB and Addr is Virt, A=%X",
			tInAddr);
	end
`endif

	tRegInPc	= regInPc;
	tRegInPcInc	= regInPcInc;
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
		{	regInSr[23:22], regInSr[27:26],
			regInSr[31:28], regKrrHash[7:0] } ;
//		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;
`else

	tNxtAxH		=
		{	regInSr[23:22], regInSr[27:26],
			regInSr[31:28], regKrrHash[7:0] } ;
//		{	4'h0, regInSr[31:28], regKrrHash[7:0] } ;
//		{	4'h0, tRegInSr[31:28], regKrrHash[7:0] } ;

`endif

//	tNxtReqPFx	= { regInSr[23:22], regInSr[27:26] };
	tNxtReqPFx	= { icInPcWxm[5:4], icInPcWxm[1:0] };
	
	tInPcOpm	= icInPcOpm;
	
//	tNxtInPmode		= regInSr[31:28];
	tNxtInPmode		= regInSr[31:28] ^ regKrrHash[3:0] ^ regKrrHash[7:4];

`ifdef jx2_enable_vaddr48
	tRegInPcP0	= tRegInPc[47:4];
	tRegInPcP1[27: 0]	= tRegInPcP0[27: 0]+1;
	tRegInPcP1[43:28]	= tRegInPcP0[43:28];

//	tRegInPcP1	= tRegInPcInc[47:4];

//	$display("%X %X", tRegInPcP0, tRegInPcP1);

`ifndef def_true
	tRegInPcP1L = { 1'b0, tRegInPcP0[11: 0] } + 1;
	tRegInPcP1H = { 1'b0, tRegInPcP0[23:12] } + 1;
	tRegInPcP1 = {
		tRegInPcP0[43:24],
//		tRegInPcP1L[12]?tRegInPcP1H[11:0]:tRegInPcP0[23:12],
		(tRegInPcP0[11: 0]==12'hFFF) ? tRegInPcP1H[11:0] : tRegInPcP0[23:12],
		tRegInPcP1L[11:0]
		};
`endif

// `ifdef def_true
`ifndef def_true
	tRegInPcP1L = { 1'b0, tRegInPcP0[ 7: 0] } + 1;
	tRegInPcP1M = { 1'b0, tRegInPcP0[15: 8] } + 1;
	tRegInPcP1H = { 1'b0, tRegInPcP0[23:16] } + 1;
//	tRegInPcP1L_C = tRegInPcP1L[8];
	tRegInPcP1L_C = (tRegInPcP0[ 7: 0] == 8'hFF);
	tRegInPcP1M_C = tRegInPcP1M[8] && tRegInPcP1L_C;
	tRegInPcP1H_C = tRegInPcP1H[8] && tRegInPcP1M_C;

	tRegInPcP1 = {
		tRegInPcP0[43:24],
		tRegInPcP1M_C ? tRegInPcP1H[7:0] : tRegInPcP0[23:16],
		tRegInPcP1L_C ? tRegInPcP1M[7:0] : tRegInPcP0[15: 8],
		tRegInPcP1L[7:0]
		};
`endif

	if(tRegInPc[4])
	begin
		tNxtAddrB			= tRegInPcP0;
		tNxtAddrA			= tRegInPcP1;
	end else begin
		tNxtAddrA			= tRegInPcP0;
//		tNxtAddrB			= tRegInPcP1;
		tNxtAddrB			= { tRegInPcP0[43:1], 1'b1 };
	end
`else
	tRegInPcP0	= { 16'h00, tRegInPc[31:4] };
	tRegInPcP1[27: 0]	= tRegInPcP0[27: 0]+1;
	tRegInPcP1[43:28]	= tRegInPcP0[43:28];

	if(tRegInPc[4])
	begin
		tNxtAddrB			= tRegInPcP0[27:0];
		tNxtAddrA			= tRegInPcP1[27:0];
	end else begin
		tNxtAddrA			= tRegInPcP0[27:0];
		tNxtAddrB			= tRegInPcP1[27:0];
	end
`endif

	tNxtInPcWxe		= icInPcWxe;
	tNxtInPcXG2		= icInPcXG2;
//	tNxtInPcRiscv	= (icInPcWxm == 2'b01);
//	tNxtInPcRiscv	= (icInPcWxm[1:0] == 2'b01);
//	tNxtInPcRiscv	= (icInPcWxm[1:0] == 2'b01) && !tNxtInPcXG2;
	tNxtInPcRiscv	= icInPcWxm[0] && !tNxtInPcXG2;
	tNxtInPcXG3		= tNxtInPcRiscv && tNxtInPcWxe;

//	tUtlb1BlkIx		= tUtlb1BlkIxL;
	tUtlb1BlkIx		= tNxtUtlbBlkIx;

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

	if(icInPcHold)
	begin
		tRegInPc		= tInAddr;
		tNxtAddrA		= tReqAddrA;
		tNxtAddrB		= tReqAddrB;
		tNxtIxA			= tReqIxA;
		tNxtIxB			= tReqIxB;
		tNxtAddrHi		= tReqAddrHi;
		tNxtAxH			= tReqAxH;
		tNxtInPmode		= tInPmode;
		tNxtReqPFx		= tReqPFx;
		tNxtInPcWxe		= tInPcWxe;
		tNxtInPcXG2		= tInPcXG2;
		tNxtInPcXG3		= tInPcXG3;
		tNxtInPcRiscv	= tInPcRiscv;
		tNxtSkipTlb		= tSkipTlb;
		tNxtSkipMiss	= tSkipMiss;
		tUtlb1BlkIx		= tUtlbBlkIx;
	end

	icNxtDoFlush		= 0;
	tNxtDoFlushTlb		= 0;

	tNxtFlushRov		= tFlushRov;
	tNxtFlushRovTlb		= tFlushRovTlb;
	tNxtAdvFlushRov		= 0;

//	if(((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS)) || tResetL)
//	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS) && !tResetL)
	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS))
	begin
		icNxtDoFlush = 1;
		tNxtDoFlushTlb = 1;
	end

//	if((tInOpmB==JX2_DCOPM_INVTLB) && (tInOpmC!=JX2_DCOPM_INVTLB) && !tResetL)
	if((tInOpmB==JX2_DCOPM_INVTLB) && (tInOpmC!=JX2_DCOPM_INVTLB))
	begin
		tNxtDoFlushTlb = 1;
	end

//	if((tInOpmB == JX2_DCOPM_LDTLB) && (tInOpmC!=JX2_DCOPM_LDTLB) && !tResetL)
	if((tInOpmB == JX2_DCOPM_LDTLB) && (tInOpmC!=JX2_DCOPM_LDTLB))
	begin
		tNxtDoFlushTlb = 1;
	end

//	if((tFlushRov == 0) && !tAdvFlushRov && !tResetL)
	if((tFlushRov == 0) && !tAdvFlushRov)
		icNxtDoFlush = 1;

//	if((tFlushRovTlb == 0) && !tDoFlushTlb && !tResetL)
	if((tFlushRovTlb == 0) && !tDoFlushTlb)
		tNxtDoFlushTlb = 1;

	if(icDoFlush)
	begin
		if(!tAdvFlushRov)
		begin
`ifdef jx2_debug_isr
			$display("L1 I$ DoFlush rov=%X", tFlushRov);
`endif
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
	tStBlkPFxA		= tReqPFx;
	tStBlkPFxB		= tReqPFx;

//	tStBlkPFxA		= { icInPcWxm[5:4], icInPcWxm[1:0] };
//	tStBlkPFxB		= tStBlkPFxA;

	tStBlkPRovA		= tFlushRov;
	tStBlkPRovB		= tFlushRov;

	tStBlkDextA		= tReqAxH;
	tStBlkDextB		= tReqAxH;

	tStBlkPLenA		= 0;
	tStBlkPLenB		= 0;

	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	
//	tNxtMemReqStA	= 0;
//	tNxtMemReqStB	= 0;
//	tNxtMemReqLdM	= 0;
	tBlkDoRiscV		= 0;
	tBlkDoRvC		= 0;
	tBlkDoRvW		= 0;
	
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
	
	tNxtMemHeldCyc	= tMemHeldCyc + 1;
	

	tInWordIx = tInAddr[2:1];

//	tFlushA = (tBlkPRovA != tFlushRov) || (tFlushRov == 0);
//	tFlushB = (tBlkPRovB != tFlushRov) || (tFlushRov == 0);
//	tFlushA = tBlkPRovA != tFlushRov;
//	tFlushB = tBlkPRovB != tFlushRov;

	tFlushA = (tBlkPRovA[3:0] != tFlushRov[3:0]) ||
		(!tTlbMissInh && (tBlkFlagA[3:2] == 2'b11)) ;
	tFlushB = (tBlkPRovB[3:0] != tFlushRov[3:0]) ||
		(!tTlbMissInh && (tBlkFlagB[3:2] == 2'b11)) ;
	
//	tReqReady	= 1;
	tReqReady	= (tReqIxA == tNx2IxAL) && (tReqIxB == tNx2IxBL);

`ifndef def_true
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

// `ifndef def_true
`ifdef def_true
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
`endif

//	tIcExecAcl = tBlkPFlA;
	tIcExecAcl = tBlkPFlB;
	
`ifdef jx2_enable_vaddr48
//	tReqAddrIsVirt		= (tInAddr[47:28] != 0) && !tInAddr[47];
	tReqAddrIsVirt		= (tInAddr[31:28] != 0) && !tInAddr[31];
`else
	tReqAddrIsVirt		= (tInAddr[31:28] != 0) && !tInAddr[31];
`endif

	tReqAddrNoExecute	= 0;

`ifdef jx2_enable_vaddr48
	tRegOutExc[ 63:16] = tInAddr[47:0];
	tRegOutExc[111:64] = tReqAddrHi[47:0];
`else
	tRegOutExc[ 47:16] = tInAddr[31:0];
	tRegOutExc[111:48] = 0;
`endif

	if(!tRegInSr[30])
	begin
	//	if(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])
		if(	(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])	||
			(tBlkFlagB[2] && !tBlkFlagB[3] &&  tInAddr[4])	||
`ifdef jx2_enable_vaddr48
			tInAddr[47] )
`else
			tInAddr[31] )
`endif
		begin
			tReqAddrNoExecute	= 1;
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


//	tDoStallNop = 0;
	tDoStallNop = tTlbMissInh &&
		((tBlkFlag2A[3:2]==2'b11) || (tBlkFlag2B[3:2]==2'b11));
	
	if((tBlkFlag2A[3:2]==2'b11) || (tBlkFlag2B[3:2]==2'b11))
	begin
`ifdef jx2_debug_isr
		if(!tMsgLatchTmiss)
			$display("L1I$: Hold TMiss A=%X", tInAddr);
`endif
		tNxtMsgLatchTmiss = 1;

		if(tTlbMissInh)
		begin
//			tDoStallNop = 1;

			tNxtMemTlbInhRtcnt	= tMemTlbInhRtcnt-1;
			if(tMemTlbInhRtcnt==0)
			begin
`ifdef jx2_debug_isr
				$display("L1I$ Timeout Clear TLB Inhibit");
`endif
				tNxtTlbMissInh = 0;
				tClrTlbMissInh = 1;
			end
		end
	end

`ifdef jx2_enable_vaddr48
	tMissAddrA =
		(tBlkAddr2A[43:36] != tReqAddrA[43:36]) ||
		(tBlkAddr2A[35:24] != tReqAddrA[35:24]) ||
		(tBlkAddr2A[24:12] != tReqAddrA[24:12]) ||
		(tBlkAddr2A[11: 0] != tReqAddrA[11: 0]) ||
		(tBlkDext2A != tReqAxH) ||
		(tBlkPFxA != tReqPFx) ;
	tMissAddrB =
		(tBlkAddr2B[43:36] != tReqAddrB[43:36]) ||
		(tBlkAddr2B[35:24] != tReqAddrB[35:24]) ||
		(tBlkAddr2B[23:12] != tReqAddrB[23:12]) ||
		(tBlkAddr2B[11: 0] != tReqAddrB[11: 0]) ||
		(tBlkDext2B != tReqAxH) ||
		(tBlkPFxB != tReqPFx) ;
`else
	tMissAddrA =
		(tBlkAddr2A[27:24] != tReqAddrA[27:24]) ||
		(tBlkAddr2A[24:12] != tReqAddrA[24:12]) ||
		(tBlkAddr2A[11: 0] != tReqAddrA[11: 0]) ||
		(tBlkDext2A != tReqAxH) ||
		(tBlkPFxA != tReqPFx) ;
	tMissAddrB =
		(tBlkAddr2B[27:24] != tReqAddrB[27:24]) ||
		(tBlkAddr2B[23:12] != tReqAddrB[23:12]) ||
		(tBlkAddr2B[11: 0] != tReqAddrB[11: 0]) ||
		(tBlkDext2B != tReqAxH) ||
		(tBlkPFxB != tReqPFx) ;
`endif

//	if((tBlkDext2A != tReqAxH) && !tTlbMissInh)
//	if(tBlkDext2A != tReqAxH)
//		tMissAddrA = 1;
//	if((tBlkDext2B != tReqAxH) && !tTlbMissInh)
//	if(tBlkDext2B != tReqAxH)
//		tMissAddrB = 1;

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

`ifdef jx2_mem_l1i_utlb
	tUtlbBlkFlush	= (tUtlbBlkAddr[7:4] != tFlushRovTlb);
	tReqUtlbAxA		= { tUtlbBlkAddr[79:46], tReqAddrA[9:0] };
	tReqUtlbAxB		= { tUtlbBlkAddr[79:46], tReqAddrB[9:0] };
	tReqUtlbAxH		= tUtlbBlkAddr[95:80];
	tReqUtlbAccA	= tUtlbBlkAddr[3:0];
	tReqUtlbAccB	= tUtlbBlkAddr[3:0];

	tReqUtlbHitHi	=
		(tReqAddrA[43:28] == tUtlbBlkAddr[43:28]) &&
		(tReqAddrA[27:16] == tUtlbBlkAddr[27:16]) &&
		(tReqAxH == tReqUtlbAxH);
	if(tReqAddrA[16] != tReqAddrB[16])
		tReqUtlbHitHi = 0;
	tReqUtlbHitAxA	= tReqUtlbHitHi &&
		(tReqAddrA[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && tRegInMmcr[0];
	tReqUtlbHitAxB	= tReqUtlbHitHi &&
		(tReqAddrB[15:10] == tUtlbBlkAddr[15:10]) &&
		!tUtlbBlkFlush && tRegInMmcr[0];

	if(tRegInMmcr[5:4]==2'b00)
	begin
		/* 4K pages. */
		tReqUtlbAxA[9:8]=tUtlbBlkAddr[45:44];
		tReqUtlbAxB[9:8]=tUtlbBlkAddr[45:44];
		if(tReqAddrA[9:8] != tUtlbBlkAddr[9:8])
			tReqUtlbHitAxA = 0;
		if(tReqAddrB[9:8] != tUtlbBlkAddr[9:8])
			tReqUtlbHitAxB = 0;
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
//		tReqAddrNoExecute	= 0;
		$display("L1 I$: Bad Execute %X %X/%X",
			tInAddr, tBlkFlagA, tBlkFlagB);
		tRegOutExc[15:0] = 16'h8003;
	end

//	if(tMiss)
//		tRegOutExc[15] = 0;

`ifndef def_true
	if(tResetL)
	begin
		tMiss		= 0;
		tReqReady	= 0;
	end
`endif

	tMissSkipA = 0;
	tMissSkipB = 0;

`ifndef def_true
	if(tInAddr[4:2]==3'b000)
		tMissSkipB=1;
	if(tInAddr[4:1]==4'b0010)
		tMissSkipB=1;
	if(tInAddr[4])
		tMissSkipA=1;
`endif

	case(tInAddr[4:3])
		2'b00: tBlkData = { tBlkData2B[31:0], tBlkData2A[127: 0] };
		2'b01: tBlkData = { tBlkData2B[95:0], tBlkData2A[127:64] };
		2'b10: tBlkData = { tBlkData2A[31:0], tBlkData2B[127: 0] };
		2'b11: tBlkData = { tBlkData2A[95:0], tBlkData2B[127:64] };
	endcase

//	opLenA0		= tOpGetLen_A0;
//	opLenA1		= tOpGetLen_A1;
//	opLenA2		= tOpGetLen_A2;
//	opLenA3		= tOpGetLen_A3;
//	opLenA4		= tOpGetLen_A4;
//	opLenA5		= tOpGetLen_A5;

	tRegOutPcVal	= UV96_00;
	tRegOutPcStep	= 0;
	tRegOutPcStepA	= 0;
	tRegOutPcSxo	= 0;
	tRegOutPcLow	= tInAddr[11:0];

	tPcStepWA		= tOpGetLen_W0[2];
	tPcStepBA		= tOpGetLen_W0[0];
	tPcStepJA		= tOpGetLen_W0[3];
	tPcStepWB		= tOpGetLen_W1[2];
	tPcStepBB		= tOpGetLen_W1[0];
	tPcStepJWA		= tOpGetLen_W0[1];

	if(tBlkIsSxo)
		tRegOutPcSxo[0]=1;

`ifdef def_true
	if((tReqAddrA[27:0]==0) && !tMiss && !tRegInMmcr[0])
	begin
		/* Detect boot into RISC-V Mode. */
		if(tBlkData2A[47:40]==8'h00)
		begin
			if(tBlkData2A[39:32]==8'h13)
				tRegOutPcSxo[3:2]=2'b10;	/* RV NOP */
			if(tBlkData2A[39:32]==8'h0A)
				tRegOutPcSxo[3:2]=2'b11;	/* XG3 NOP */

			if(tRegOutPcSxo[3] && !regInSr[26])
			begin
				$display("RbiMemIcWxA: Trigger RV-Mode %X", tRegOutPcSxo[3:2]);
				tDoStallNop=1;
			end
		end
	end
`endif

`ifndef def_true
	if(tInWordIx[1])
		tBlkData0A = tBlkData[159:32];
	else
		tBlkData0A = tBlkData[127: 0];
	if(tInWordIx[0])
		tRegOutPcVal = tBlkData0A[111:16];
	else
		tRegOutPcVal = tBlkData0A[95: 0];
`endif

	case(tInWordIx[1:0])
		2'b00: tRegOutPcVal = tBlkData[ 95: 0];
		2'b01: tRegOutPcVal = tBlkData[111:16];
		2'b10: tRegOutPcVal = tBlkData[127:32];
		2'b11: tRegOutPcVal = tBlkData[143:48];
	endcase

//	tPcStepWA0 = tPcStepWA;
//	tPcStepWB0 = tPcStepWB;
//	tPcStepJA0 = tPcStepJA;
//	tPcStepBA0 = tPcStepBA;
//	tPcStepBB0 = tPcStepBB;

	tRegOutPcStepA = tPcStepBA ? 4'b0010 : 4'b0100;

//	if(tInPcXG2)
	if(tInPcXG2 && !tMiss)
	begin
		if(tPcStepBA || tPcStepBB)
		begin
			if(!tMsgLatch)
				$display("L1 I$: XG2 Bad Step");
			tNxtMsgLatch=1;
		end
		if(tInWordIx[0])
		begin
			if(!tMsgLatch)
				$display("L1 I$: XG2 Misaligned Fetch, A=%X", tInAddr);
			tNxtMsgLatch=1;
		end

// `ifdef def_true
`ifndef def_true
//		$display("L1 I$: XG2 Debug A=%X V=%X", tInAddr, tRegOutPcVal);
		$display("L1 I$: XG2 Debug A=%X V=%X-%X-%X-%X-%X-%X",
			tInAddr,
			tRegOutPcVal[15: 0], tRegOutPcVal[31:16],
			tRegOutPcVal[47:32], tRegOutPcVal[63:48],
			tRegOutPcVal[79:64], tRegOutPcVal[95:80] );
`endif
	end

	if(tInAddr[0])
	begin
		$display("L1 I$: Misaligned Fetch, A=%X", tInAddr);
	end

// `ifdef jx2_enable_wex3w
`ifdef jx2_enable_wex

`ifdef jx2_enable_wexjumbo

`ifndef def_true
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

	if(tDoStallNop)
		tRegOutPcStep = 0;
`endif

`ifndef def_true
	casez( { tDoStallNop,
			tInPcWxe, tPcStepJA,
			tPcStepWA, tPcStepWB,
			tPcStepBB, tPcStepBA })
		7'b000zzz0: tRegOutPcStep = 4'b0100;
		7'b000zzz1: tRegOutPcStep = 4'b0010;

		7'b001z00z: tRegOutPcStep = 4'b1000;
		7'b001z01z: tRegOutPcStep = 4'b0110;
		7'b001z1zz: tRegOutPcStep = 4'b1100;

		7'b0100zz0: tRegOutPcStep = 4'b0100;
		7'b0100zz1: tRegOutPcStep = 4'b0010;
		7'b01010zz: tRegOutPcStep = 4'b1000;
		7'b01011zz: tRegOutPcStep = 4'b1100;

		7'b011z00z: tRegOutPcStep = 4'b1000;
		7'b011z01z: tRegOutPcStep = 4'b0110;
		7'b011z1zz: tRegOutPcStep = 4'b1100;

		7'b1zzzzzz: tRegOutPcStep = 4'b0000;
	endcase
`endif

`ifdef def_true
//	tCombJWA = tPcStepJA || (tInPcWxe && tPcStepWA);
	tCombJWA = tPcStepJWA;

`ifdef jx2_dec_ssc_riscv
	if(blkDataRvSscEna && tInPcRiscv && !tPcStepBA)
		tCombJWA = 1;
`endif

	casez( { tDoStallNop,
			tCombJWA, tPcStepWB,
			tPcStepBB , tPcStepBA })

		5'b00zz0: tRegOutPcStep = 4'b0100;
		5'b00zz1: tRegOutPcStep = 4'b0010;
		5'b0100z: tRegOutPcStep = 4'b1000;
		5'b0101z: tRegOutPcStep = 4'b0110;
		5'b011zz: tRegOutPcStep = 4'b1100;
		
		5'b1zzzz: tRegOutPcStep = 4'b0000;
	endcase
`endif

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

	if(tDoStallNop)
		tRegOutPcStep = 0;
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
	if(tDoStallNop)
		tRegOutPcStep = 0;
`endif
	
//	tRegOutPcOK = tMiss ? UMEM_OK_HOLD : UMEM_OK_OK;

//	if(tResetL)
//	begin
//		tMiss = 0;
//	end

	tReqReadyN = !tReqReady;

`ifndef def_true
//	if(!tReqReady)
	if(tReqReadyN)
	begin
		if(tMemReqLdA && tMemRespLdAN)
			tMissA = 1;
		if(tMemReqLdB && tMemRespLdBN)
			tMissB = 1;
//		tMiss = tMiss1L;
		tMiss = tMissA || tMissB;
	
		tRegOutHold = 1;
	end
`endif

	if(tMemReqLdA && tMemRespLdAN)
		tMissA = 1;
	if(tMemReqLdB && tMemRespLdBN)
		tMissB = 1;

//	if(tMiss)
//		tRegOutExc[15] = 0;

// `ifndef def_true
`ifdef def_true
//	if(tMiss)
//	if(tMiss || !tReqReady)
	if(tMiss || tReqReadyN)
		tRegOutHold = 1;

	if(tMemReqLdA && tMemRespLdAN)
		tRegOutHold = 1;
	if(tMemReqLdB && tMemRespLdBN)
		tRegOutHold = 1;
`endif

	if(	(tMemReqLdA && tMemRespLdAN) ||
		(tMemReqLdB && tMemRespLdBN) ||
//		(tMiss || !tReqReady)		 )
		(tMiss || tReqReadyN)		 )
			tRegOutHold = 1;

//	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;


	tUtlbStAddr		= 0;
	tUtlbStIx		= 0;
	tUtlbDoSt		= 0;

	tReqSeqIdx	= tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa	= tReqSeqVaArr[memSeqIn[3:0]];

	tNxtMemRespLdA	= tMemRespLdA;
	tNxtMemRespLdB	= tMemRespLdB;

//	if(memRingIsRespOkLdA && !tResetL)
	if(memRingIsRespOkLdA)
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
//			tStBlkFlagA		= memOpmIn[3:0];
			tStBlkFlagA		= memOpmIn[11:8];

`ifdef jx2_memic_blklenbits
			tStBlkPLenA		= memDataLens;
`endif

			if(tStickyTlbExc[15:12]==4'h7)
	//			tStBlkPFlA[15:8]=tStickyTlbExc[119:112];
				tStBlkPFlA[15:0]=tStickyTlbExc[127:112];

			tDoStBlkA		= 1;

			tNxtMemRespLdA	= 1;

			tUtlbStAddr		= {
				tReqAxH,
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
//				memOpmIn[3:0] };
				memOpmIn[11:8] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
//			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);
			tUtlbDoSt		= (memOpmIn[11:10] != 2'b11);

//			if(memOpmIn[3:2]==2'b11)
			if(memOpmIn[11:10]==2'b11)
	//		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
			begin
`ifdef jx2_debug_isr
				$display("L1I$ Set TLB Inhibit A");
`endif
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

`ifdef jx2_enable_vaddr48
		if(tReqSeqIdx != tReqIxA)
		begin
			$display("L1 I$, Mismatch Index A, %X!=%X",
				tReqSeqIdx, tReqIxA);
		end
		if(tReqSeqVa[43:0] != tReqAddrA[43:0])
		begin
			$display("L1 I$, Mismatch Index A, %X!=%X",
				tReqSeqVa[43:0], tReqAddrA[43:0]);
		end
`ifdef jx2_debug_isr
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1I$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
`endif
`endif
	end

//	if(memRingIsRespOkLdB && !tResetL)
	if(memRingIsRespOkLdB)
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
//			tStBlkFlagB		= memOpmIn[3:0];
			tStBlkFlagB		= memOpmIn[11:8];

`ifdef jx2_memic_blklenbits
			tStBlkPLenB		= memDataLens;
`endif

			tDoStBlkB		= 1;

			tNxtMemRespLdB	= 1;

			tUtlbStAddr		= {
				tReqAxH,
				memAddrIn[47:12],
				tReqSeqVa[43: 8],
				tFlushRovTlb,
//				memOpmIn[3:0] };
				memOpmIn[11:8] };
			tUtlbStIx		= tReqSeqVa[11:8] ^ tReqSeqVa[15:12];
//			tUtlbDoSt		= (memOpmIn[3:2] != 2'b11);
			tUtlbDoSt		= (memOpmIn[11:10] != 2'b11);

			if(tStickyTlbExc[15:12]==4'h7)
	//			tStBlkPFlB[15:8]=tStickyTlbExc[119:112];
				tStBlkPFlB[15:0]=tStickyTlbExc[127:112];

	//		if(memOpmIn[3])
//			if(memOpmIn[3:2]==2'b11)
			if(memOpmIn[11:10]==2'b11)
	//		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
			begin
`ifdef jx2_debug_isr
				$display("L1I$ Set TLB Inhibit B");
`endif
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

`ifdef jx2_enable_vaddr48
		if(tReqSeqIdx != tReqIxB)
		begin
			$display("L1 I$, Mismatch Index B, %X!=%X",
				tReqSeqIdx, tReqIxB);
		end
		if(tReqSeqVa[43:0] != tReqAddrB[43:0])
		begin
			$display("L1 I$, Mismatch Index B, %X!=%X",
				tReqSeqVa[43:0], tReqAddrB[43:0]);
		end
`ifdef jx2_debug_isr
		if((memAddrIn[31:5]!=tReqSeqVa[27:1]) && (tReqAxH!=UV16_FF) &&
				(tReqSeqVa[43:24]==0))
			$display("L1I$: Virt!=Phys A, PA=%X VA=%X O=%X",
				memAddrIn[31:4], tReqSeqVa[43:0], memOpmIn);
`endif
`endif
	end

	if(tResetL)
	begin
		tDoStBlkA	= 0;
		tDoStBlkB	= 0;
		tUtlbDoSt	= 0;
		tNxtTlbMissInh = 0;
	end
	
	/* Miss Handling */

`ifndef def_true
	tReqWaitResp = 0;
	if(tMemReqLdA && tMemRespLdAN)
		tReqWaitResp = 1;
	if(tMemReqLdB && tMemRespLdBN)
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

//		if(!tReqReady)
		if(tReqReadyN)
		begin
		end
		else
			if(tMissA && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
`ifdef jx2_enable_vaddr48
			tMemSeqVa		= tReqAddrA;
`else
			tMemSeqVa		= { 16'h00, tReqAddrA };
`endif
			tMemSeqReq		= { unitNodeId, 4'b1000, tMemSeqRov };

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrA, 4'h00 };
`else
`ifdef jx2_enable_vaddr48
			tMemAddrReq		= { tReqAddrA, 4'h00 };
`else
			tMemAddrReq		= { 16'h00, tReqAddrA, 4'h00 };
`endif
`endif
			tNxtMemReqLdA = 1;

			if(tSkipTlb)
			begin
`ifdef jx2_debug_isr
//				if(tReqAddrA[43:24]!=0)
				if(tReqAddrIsVirt)
					$display("L1 I$: Send LDA Req Abs A=%X", tReqAddrA);
`endif

//				if(!tReqAddrIsVirt)
					tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;

				if(tReqAddrIsVirt)
				begin
					tMemOpmReq[11:8] = 4'hF;
					tMemAddrReq[31:16]=16'h0001;
				end
			end

`ifdef jx2_mem_l1i_utlb
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

//			$display("I$ LDA %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxA);
		end
		else if(tMissB && !tMemReqLdB)
		begin

			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
`ifdef jx2_enable_vaddr48
			tMemSeqVa		= tReqAddrB;
`else
			tMemSeqVa		= { 16'h00, tReqAddrB };
`endif
			tMemSeqReq		= { unitNodeId, 4'b1100, tMemSeqRov};

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrB, 4'h00 };
`else
`ifdef jx2_enable_vaddr48
			tMemAddrReq		= { tReqAddrB, 4'h00 };
`else
			tMemAddrReq		= { 16'h00, tReqAddrB, 4'h00 };
`endif
`endif
			tNxtMemReqLdB = 1;

			if(tSkipTlb)
			begin
`ifdef jx2_debug_isr
//				if(tReqAddrB[43:24]!=0)
				if(tReqAddrIsVirt)
					$display("L1 I$: Send LDB Req Abs A=%X", tReqAddrB);
`endif

//				if(!tReqAddrIsVirt)
					tMemAddrReq[47:32]=JX2_RBI_ADDRHI_PHYS;
//				tMemAddrReq[47:44]=4'hC;

				if(tReqAddrIsVirt)
				begin
					tMemOpmReq[11:8] = 4'hF;
					tMemAddrReq[31:16]=16'h0001;
				end
			end

`ifdef jx2_mem_l1i_utlb
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
			if(!tRegOutHold)
				$display("L1 I$: Request Hold");
			tRegOutHold = 1;
		end
	end
`endif

`ifndef def_true
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
`endif

//	if(tDoStBlkA || tDoStBlkB || tDidStBlkA || tDidStBlkB)
//		tRegOutHold = 1;

`ifdef def_true
// `ifndef def_true
//	if(tNxtMemReqLdA || tNxtMemReqLdB)
	if(	(tNxtMemReqLdA && !tMemReqLdA) ||
		(tNxtMemReqLdB && !tMemReqLdB) )
	begin
		tRegOutHold = 1;
	end
`endif

`ifndef def_true
	if((tRegOutPcVal[15:0]==16'h0000) && !tRegOutHold && !tDoStallNop)
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
	end
`endif
		
	if(tResetL)
		tRegOutHold = 0;


	if(!tRegOutHold)		//BGB: Debug
		tNxtTlbMissInh = 0;


//	if(!tRegOutHold)
//		tNxtStickyTlbExc[15:12]=0;

`ifdef jx2_mem_l1inostall
	if(tRegOutHold || (tInAddrNl[15:0]!=tInAddr[15:0]))
	begin
		tDoStallNop = 1;
	end
	tRegOutPcOK = UMEM_OK_OK;
`else
	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;
`endif

	if(tDoStallNop)
	begin
`ifdef jx2_debug_isr
		if(!tMsgLatchNop)
			$display("RbiMemIcWxA: NOP Stall");
`endif
		tNxtMsgLatchNop = 1;

		tRegOutPcVal = 96'h3000F013_3000F013_3000F013;

`ifndef def_true

		tRegOutPcVal = 96'h3000F000_3000F000_3000F000;
`ifdef jx2_enable_riscv
		if(tInPcRiscv)
			tRegOutPcVal = 96'h00000013_00000013_00000013;
`endif

`endif
//		tRegOutPcStep = 0;
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

(* max_fanout = 200 *)
	wire		tAdvHold;

always @*
begin
	tStuckTlbMissInh	= 0;
	tNxtMsgLatchTinh	= 0;

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
`ifdef jx2_debug_isr
			if(!tStuckTlbMissInhL)
				$display("L1I$ TLB Inhibit Stuck");
`endif
			tStuckTlbMissInh = 1;
		end

//		if(!tStuckTlbMissInh)
		if(1'b1)
		begin
`ifdef jx2_debug_isr
			if(!tMsgLatchTinh)
				$display("L1I$ TLB Inhibit, InPc=%X InAddr=%X Clear=%X",
					regInPc, tInAddr, tClrTlbMissInh);
`endif
			tNxtMsgLatchTinh=1;
		end
	end
end

// assign		tAdvHold = icInPcHold;
// assign		tAdvHold = tRegOutHold;
assign		tAdvHold = tRegOutHold || icInPcHold;

`ifdef jx2_mem_l1i_fastix

// assign		tNx2IxA = tRegOutHold ? tReqIxA : tNxtIxA;
// assign		tNx2IxB = tRegOutHold ? tReqIxB : tNxtIxB;

assign		tNx2IxA = tAdvHold ? tReqIxA : tNxtIxA;
assign		tNx2IxB = tAdvHold ? tReqIxB : tNxtIxB;

`else

assign		tNx2IxA = tReqIxA;
assign		tNx2IxB = tReqIxB;

`endif

reg[15:0]	tBlkAddrHwA;
reg[15:0]	tBlkAddrHwB;

always @(posedge clock)
begin
//	tTlbMissInh		<= tNxtTlbMissInh;
//	tFlushRov		<= tNxtFlushRov;
//	tAdvFlushRov	<= tNxtAdvFlushRov;

//	tMemPcData		<= memPcData;

	tRegInSr		<= regInSr;
	tRegInSrL		<= tRegInSr;
	tResetL			<= reset;

	tRegInMmcr		<= regInMmcr;


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
	tMsgLatchNop	<= tNxtMsgLatchNop;
	tMsgLatchTinh	<= tNxtMsgLatchTinh;

	tMemReqRtcnt	<= tNxtMemReqRtcnt;
	tMemTlbInhRtcnt	<= tNxtMemTlbInhRtcnt;

	tInAddrNl		<= tRegInPc;

	tTlbExc			<= tlbExc;
//	tStickyTlbExc	<= tNxtStickyTlbExc;
//	tStickyTlbExc	<= tAdvHold ? tNxtStickyTlbExc : 0;

	tStickyTlbExc[127:16]	<= tNxtStickyTlbExc[127:16];
	tStickyTlbExc[ 15: 0]	<= tAdvHold ? tNxtStickyTlbExc[15:0] : 0;

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

		tReqPFx			<= tNxtReqPFx;

//		tTlbMissInh		<= tNxtTlbMissInh;
		tFlushRov		<= tNxtFlushRov;
		tAdvFlushRov	<= tNxtAdvFlushRov;

		tFlushRovTlb	<= tNxtFlushRovTlb;
		tDoFlushTlb		<= tNxtDoFlushTlb;

		tInPmode		<= tNxtInPmode;
//		tInPcWxe		<= icInPcWxe;
//		tInPcRiscv		<= (icInPcWxm == 2'b01);

		tInPcWxe		<= tNxtInPcWxe;
		tInPcRiscv		<= tNxtInPcRiscv;

`ifdef jx2_enable_xg2mode
		tInPcXG2		<= tNxtInPcXG2;
`else
		tInPcXG2		<= 0;
		tInPcXG3		<= 0;
`endif

`ifdef jx2_enable_riscv_xg3
		tInPcXG3		<= tNxtInPcXG3;
`else
		tInPcXG3		<= 0;
`endif

//		tUtlbBlkIx		<= tNxtReqAddr[11:8];
		tUtlbBlkIx		<= tNxtUtlbBlkIx;

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

	tMemHeldCyc		<= tAdvHold ? tNxtMemHeldCyc : 0;

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

`ifdef jx2_memic_blklenbits
	{	tBlkPFxA, tBlkPLenA,
		tBlkPFlA, tBlkPRovA,
		tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
	{ 	tBlkPFxB, tBlkPLenB,
		tBlkPFlB, tBlkPRovB,
		tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];
`else
	tBlkPFxA	<= tReqPFx;
	tBlkPFxB	<= tReqPFx;
`ifdef jx2_enable_vaddr48
	{ tBlkPFlA, tBlkPRovA, tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
	{ tBlkPFlB, tBlkPRovB, tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];
`else
	{ tBlkPFlA, tBlkPRovA, tBlkFlagA, tBlkAddrHwA, tBlkAddrA }	<= 
		icCaAddrA[tNx2IxA];
	{ tBlkPFlB, tBlkPRovB, tBlkFlagB, tBlkAddrHwB, tBlkAddrB }	<=
		icCaAddrB[tNx2IxB];
`endif
`endif

	tNx2IxAL	<= tNx2IxA;
	tNx2IxBL	<= tNx2IxB;

	if(tDoStBlkA)
	begin
//		$display("L1I$ DoStBlkA Ix=%X A=%X D=%X",
//			tStBlkIxA, tStBlkAddrA, tStBlkDataA);

//		icCaMemA[tStBlkIxA]		<= { tStBlkPFlA, tStBlkPRovA, tStBlkDataA };
//		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };

		icCaMemA[tStBlkIxA]		<= { tStBlkDextA, tStBlkDataA };

`ifdef jx2_memic_blklenbits
		icCaAddrA[tStBlkIxA]	<= {
			tStBlkPFxA, tStBlkPLenA,
			tStBlkPFlA, tStBlkPRovA,
			tStBlkFlagA, tStBlkAddrA };
`else
		icCaAddrA[tStBlkIxA]	<= {
			tStBlkPFlA, tStBlkPRovA,
			tStBlkFlagA, tStBlkAddrA };
`endif
	end

	if(tDoStBlkB)
	begin
//		$display("L1I$ DoStBlkB Ix=%X A=%X D=%X",
//			tStBlkIxB, tStBlkAddrB, tStBlkDataB);

//		icCaMemB[tStBlkIxB]		<= { tStBlkPFlB, tStBlkPRovB, tStBlkDataB };
//		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };

		icCaMemB[tStBlkIxB]		<= { tStBlkDextB, tStBlkDataB };

`ifdef jx2_memic_blklenbits
		icCaAddrB[tStBlkIxB]	<= {
			tStBlkPFxB, tStBlkPLenB,
			tStBlkPFlB, tStBlkPRovB,
			tStBlkFlagB, tStBlkAddrB };
`else
		icCaAddrB[tStBlkIxB]	<= {
			tStBlkPFlB, tStBlkPRovB,
			tStBlkFlagB, tStBlkAddrB };
`endif
	end

	tDidStBlkA		<= tDoStBlkA;
	tDidStBlkB		<= tDoStBlkB;

`ifdef jx2_mem_l1i_utlb
	if(tUtlbDoSt)
	begin
		tUtlbArr[tUtlbStIx]		 <= tUtlbStAddr;
	end
	tUtlbBlkAddr		<= tUtlbArr[tUtlb1BlkIx];
	tUtlb1BlkIxL		<= tUtlb1BlkIx;
`endif

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

		tMemRespLdA	<= 0;
		tMemRespLdB	<= 0;
		tMemRespLdAN	<= 1;
		tMemRespLdBN	<= 1;
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
		tMemReqLdA		<= tAdvHold ? tNxtMemReqLdA : 0;
		tMemReqLdB		<= tAdvHold ? tNxtMemReqLdB : 0;
		tMemRespLdA		<= tAdvHold ? tNxtMemRespLdA : 0;
		tMemRespLdB		<= tAdvHold ? tNxtMemRespLdB : 0;
		tMemRespLdAN	<= tAdvHold ? !tNxtMemRespLdA : 1;
		tMemRespLdBN	<= tAdvHold ? !tNxtMemRespLdB : 1;
`endif

`ifndef def_true
//		tMemReqStA		<= tRegOutHold ? tNxtMemReqStA : 0;
//		tMemReqStB		<= tRegOutHold ? tNxtMemReqStB : 0;
		tMemReqLdA		<= tRegOutHold ? tNxtMemReqLdA : 0;
		tMemReqLdB		<= tRegOutHold ? tNxtMemReqLdB : 0;
		tMemRespLdA		<= tRegOutHold ? tNxtMemRespLdA : 0;
		tMemRespLdB		<= tRegOutHold ? tNxtMemRespLdB : 0;
		tMemRespLdAN	<= tRegOutHold ? !tNxtMemRespLdA : 1;
		tMemRespLdBN	<= tRegOutHold ? !tNxtMemRespLdB : 1;
`endif

`ifndef def_true
//		tMemReqStA	<= tNxtMemReqStA;
//		tMemReqStB	<= tNxtMemReqStB;
		tMemReqLdA	<= tNxtMemReqLdA;
		tMemReqLdB	<= tNxtMemReqLdB;
//		tMemReqLdM	<= tNxtMemReqLdM;
`endif

		tReqSeqIdxArr[tMemSeqRov]	<= tMemSeqIx;
		tReqSeqVaArr[tMemSeqRov]	<= tMemSeqVa;
		tMemSeqRov					<= tNxtMemSeqRov;
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
		tMemReqLdA		<= tAdvHold ? tMemReqLdA : 0;
		tMemReqLdB		<= tAdvHold ? tMemReqLdB : 0;
		tMemRespLdA		<= tAdvHold ? tMemRespLdA : 0;
		tMemRespLdB		<= tAdvHold ? tMemRespLdB : 0;
		tMemRespLdAN	<= tAdvHold ? !tMemRespLdA : 1;
		tMemRespLdBN	<= tAdvHold ? !tMemRespLdB : 1;
`endif

`ifndef def_true
		tMemReqLdA		<= tRegOutHold ? tMemReqLdA : 0;
		tMemReqLdB		<= tRegOutHold ? tMemReqLdB : 0;
		tMemRespLdA		<= tRegOutHold ? tNxtMemRespLdA : 0;
		tMemRespLdB		<= tRegOutHold ? tNxtMemRespLdB : 0;
		tMemRespLdAN	<= tRegOutHold ? !tNxtMemRespLdA : 1;
		tMemRespLdBN	<= tRegOutHold ? !tNxtMemRespLdB : 1;
`endif
	end

end

endmodule

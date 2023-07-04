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
L2 Tile Cache, Ringbus

Presents an interface with 128-bit logical cache lines, with 512-bit DDR transfers.
*/

`include "ringbus/RbiDefs.v"

module RbiMemL2DcWa(
	/* verilator lint_off UNUSED */
	clock,			reset,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId,

	ddrMemAddr,		ddrMemAddrSw,
	ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,
	ddrOpSqI,		ddrOpSqO,

	regDeadlockStrobe
	);

input			clock;
input			reset;
	
input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


input [511:0]	ddrMemDataIn;
output[511:0]	ddrMemDataOut;
output[ 31:0]	ddrMemAddr;
output[ 31:0]	ddrMemAddrSw;
output[  4:0]	ddrMemOpm;
input [  1:0]	ddrMemOK;

input [3:0]		ddrOpSqI;
output[3:0]		ddrOpSqO;

input			regDeadlockStrobe;

reg[511:0]	tDdrMemDataIn;
reg[511:0]	tDdrMemDataInB;
reg[511:0]	tDdrMemDataOut;
reg[511:0]	tDdrMemDataOutB;

reg[1:0]		tDdrMemOK;
reg[1:0]		tDdrMemOKB;
reg[ 31:0]		tDdrMemAddr;
reg[ 31:0]		tDdrMemAddrB;
reg[ 31:0]		tDdrMemAddrSw;
reg[ 31:0]		tDdrMemAddrSwB;
reg[  4:0]		tDdrMemOpm;
reg[  4:0]		tDdrMemOpmB;
reg[  3:0]		tDdrMemOpSq;
reg[  3:0]		tDdrMemOpSqB;
reg[  3:0]		tDdrMemOpSqI;
reg[  3:0]		tDdrMemOpSqIB;

`ifndef jx2_l2d_nofw_ddrout
// `ifndef def_true
assign	ddrMemDataOut	= tDdrMemDataOutB;
assign	ddrMemAddr		= tDdrMemAddrB;
assign	ddrMemAddrSw	= tDdrMemAddrSwB;
assign	ddrMemOpm		= tDdrMemOpmB;
assign	ddrOpSqO		= tDdrMemOpSqB;
`endif

`ifdef jx2_l2d_nofw_ddrout
// `ifdef def_true
assign	ddrMemDataOut	= tDdrMemDataOut;
assign	ddrMemAddr		= tDdrMemAddr;
assign	ddrMemAddrSw	= tDdrMemAddrSw;
assign	ddrMemOpm		= tDdrMemOpm;
assign	ddrOpSqO		= tDdrMemOpSq;
`endif


// input [ 15:0]	memSeqIn;		//operation sequence
// input [ 15:0]	memOpmIn;		//memory operation mode
// input [ 31:0]	memAddrIn;		//memory input address
// input [127:0]	memDataIn;		//memory input data

reg[ 15:0]		tMemSeqOut;			//operation sequence
reg[ 15:0]		tMemOpmOut;			//memory operation mode
`reg_l2addr		tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data
assign		memSeqOut	= tMemSeqOut;
assign		memOpmOut	= tMemOpmOut;
assign		memAddrOut	= tMemAddrOut;
assign		memDataOut	= tMemDataOut;

reg[ 15:0]		tMemSeqReq;			//operation sequence
reg[ 15:0]		tMemOpmReq;			//memory operation mode
`reg_l2addr		tMemAddrReq;		//memory output address
`reg_tile		tMemDataReq;		//memory output data
reg				tMemDidResp;		//
reg				tNxtMemDoResp;		//
reg				tMemSkipReq;		//Skipped Request

reg[ 15:0]		mem2SeqIn;		//
reg[ 15:0]		mem2OpmIn;		//
`reg_l2addr		mem2AddrIn;		//
`reg_tile		mem2DataIn;		//

reg[ 15:0]		mem3SeqIn;		//
reg[ 15:0]		mem3OpmIn;		//
`reg_l2addr		mem3AddrIn;		//
`reg_tile		mem3DataIn;		//

reg[ 15:0]		mem4SeqIn;		//
reg[ 15:0]		mem4OpmIn;		//
`reg_l2addr		mem4AddrIn;		//
`reg_tile		mem4DataIn;		//


`ifdef jx2_mem_l2wsz_16384
reg[127:0]		memTileData0A[16383:0];
reg[127:0]		memTileData1A[16383:0];
reg[127:0]		memTileData2A[16383:0];
reg[127:0]		memTileData3A[16383:0];
reg[ 37:0]		memTileAddrA[16383:0];
`endif

`ifdef jx2_mem_l2wsz_8192
reg[127:0]		memTileData0A[8191:0];
reg[127:0]		memTileData1A[8191:0];
reg[127:0]		memTileData2A[8191:0];
reg[127:0]		memTileData3A[8191:0];
reg[ 37:0]		memTileAddrA[8191:0];
`endif

`ifdef jx2_mem_l2wsz_4096
reg[127:0]		memTileData0A[4095:0];
reg[127:0]		memTileData1A[4095:0];
reg[127:0]		memTileData2A[4095:0];
reg[127:0]		memTileData3A[4095:0];
reg[ 37:0]		memTileAddrA[4095:0];
`endif

`ifdef jx2_mem_l2wsz_2048
reg[127:0]		memTileData0A[2047:0];
reg[127:0]		memTileData1A[2047:0];
reg[127:0]		memTileData2A[2047:0];
reg[127:0]		memTileData3A[2047:0];
reg[ 37:0]		memTileAddrA[2047:0];
`endif

`ifdef jx2_mem_l2wsz_1024
reg[127:0]		memTileData0A[1023:0];
reg[127:0]		memTileData1A[1023:0];
reg[127:0]		memTileData2A[1023:0];
reg[127:0]		memTileData3A[1023:0];
reg[ 37:0]		memTileAddrA[1023:0];
`endif

`ifdef jx2_mem_l2d2way

`ifdef jx2_mem_l2wsz_16384
reg[127:0]		memTileData0B[16383:0];
reg[127:0]		memTileData1B[16383:0];
reg[127:0]		memTileData2B[16383:0];
reg[127:0]		memTileData3B[16383:0];
reg[ 37:0]		memTileAddrB[16383:0];
`endif

`ifdef jx2_mem_l2wsz_8192
reg[127:0]		memTileData0B[8191:0];
reg[127:0]		memTileData1B[8191:0];
reg[127:0]		memTileData2B[8191:0];
reg[127:0]		memTileData3B[8191:0];
reg[ 37:0]		memTileAddrB[8191:0];
`endif

`ifdef jx2_mem_l2wsz_4096
reg[127:0]		memTileData0B[4095:0];
reg[127:0]		memTileData1B[4095:0];
reg[127:0]		memTileData2B[4095:0];
reg[127:0]		memTileData3B[4095:0];
reg[ 37:0]		memTileAddrB[4095:0];
`endif

`ifdef jx2_mem_l2wsz_2048
reg[127:0]		memTileData0B[2047:0];
reg[127:0]		memTileData1B[2047:0];
reg[127:0]		memTileData2B[2047:0];
reg[127:0]		memTileData3B[2047:0];
reg[ 37:0]		memTileAddrB[2047:0];
`endif

`ifdef jx2_mem_l2wsz_1024
reg[127:0]		memTileData0B[1023:0];
reg[127:0]		memTileData1B[1023:0];
reg[127:0]		memTileData2B[1023:0];
reg[127:0]		memTileData3B[1023:0];
reg[ 37:0]		memTileAddrB[1023:0];
`endif

`endif


`ifdef jx2_mem_l2wsz_16384
reg[13:0]	nxtReqIx;
reg[13:0]	nxtReqIx2;
reg[13:0]	tReqIx0;
reg[13:0]	tReqIx;
reg[13:0]	tBlkIx;
reg[13:0]	tReqIxL;
reg[13:0]	tReqIxL2;
reg[13:0]	tBlkLdIx;
reg[13:0]	tBlkStIx;
reg[13:0]	tBlkStIxL;

reg[13:0]	tAccIx;
reg[13:0]	tNxtAccIx;
reg[13:0]	nxtRovIx;
reg[13:0]	tRovIx;

reg[13:0]	tReqMissIx;
reg[13:0]	tNxtReqMissIx;
`endif

`ifdef jx2_mem_l2wsz_8192
reg[12:0]	nxtReqIx;
reg[12:0]	nxtReqIx2;
reg[12:0]	tReqIx0;
reg[12:0]	tReqIx;
reg[12:0]	tBlkIx;
reg[12:0]	tReqIxL;
reg[12:0]	tReqIxL2;
reg[12:0]	tBlkLdIx;
reg[12:0]	tBlkStIx;
reg[12:0]	tBlkStIxL;

reg[12:0]	tAccIx;
reg[12:0]	tNxtAccIx;
reg[12:0]	nxtRovIx;
reg[12:0]	tRovIx;

reg[12:0]	tReqMissIx;
reg[12:0]	tNxtReqMissIx;
`endif

`ifdef jx2_mem_l2wsz_4096
reg[11:0]	nxtReqIx;
reg[11:0]	nxtReqIx2;
reg[11:0]	tReqIx0;
reg[11:0]	tReqIx;
reg[11:0]	tBlkIx;
reg[11:0]	tReqIxL;
reg[11:0]	tReqIxL2;
reg[11:0]	tBlkLdIx;
reg[11:0]	tBlkStIx;
reg[11:0]	tBlkStIxL;

reg[11:0]	tAccIx;
reg[11:0]	tNxtAccIx;
reg[11:0]	nxtRovIx;
reg[11:0]	tRovIx;

reg[11:0]	tReqMissIx;
reg[11:0]	tNxtReqMissIx;
`endif

`ifdef jx2_mem_l2wsz_2048
reg[10:0]	nxtReqIx;
reg[10:0]	nxtReqIx2;
reg[10:0]	tReqIx0;
reg[10:0]	tReqIx;
reg[10:0]	tBlkIx;
reg[10:0]	tReqIxL;
reg[10:0]	tReqIxL2;
reg[10:0]	tBlkLdIx;
reg[10:0]	tBlkStIx;
reg[10:0]	tBlkStIxL;

reg[10:0]	tAccIx;
reg[10:0]	tNxtAccIx;
reg[10:0]	nxtRovIx;
reg[10:0]	tRovIx;

reg[10:0]	tReqMissIx;
reg[10:0]	tNxtReqMissIx;
`endif

`ifdef jx2_mem_l2wsz_1024
reg[9:0]	nxtReqIx;
reg[9:0]	nxtReqIx2;
reg[9:0]	tReqIx0;
reg[9:0]	tReqIx;
reg[9:0]	tBlkIx;
reg[9:0]	tReqIxL;
reg[9:0]	tReqIxL2;
reg[9:0]	tBlkLdIx;
reg[9:0]	tBlkStIx;
reg[9:0]	tBlkStIxL;

reg[9:0]	tAccIx;
reg[9:0]	tNxtAccIx;
reg[9:0]	nxtRovIx;
reg[9:0]	tRovIx;

reg[9:0]	tReqMissIx;
reg[9:0]	tNxtReqMissIx;

`endif


reg[25:0]	nxtReqAddr;
reg[5:0]	nxtReqAddrLo;

reg[25:0]	tReqAddr;

reg[25:0]	tReqAddrL;
reg[25:0]	tAccAddr;
reg[25:0]	tNxtAccAddr;
reg			tAccNoClashA;
reg			tNxtAccNoClashA;

reg[5:0]	tReqAddrLo;

reg[15:0]	tReqOpm;
reg[15:0]	tReqOpmL;
reg[15:0]	tReqOpmL2;
reg[15:0]	tReqOpmL3;
`reg_tile	tReqDataIn;

reg[25:0]	tReqAddr0;
reg[5:0]	tReqAddrLo0;
reg[15:0]	tReqOpm0;
`reg_tile	tReqDataIn0;

reg[15:0]	tReqSeq;
reg[15:0]	tReqMissSeq;
reg[15:0]	tNxtReqMissSeq;

reg[15:0]	tReqSeqC3;
reg[15:0]	tReqSeqC4;

reg[127:0]		tBlkData0A;
reg[127:0]		tBlkData1A;
reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData3A;
reg[25:0]		tBlkAddrA;
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFrovA;
reg[ 3:0]		tBlkEpochA;

reg[ 3:0]	tBlkEpochRel;
reg[ 3:0]	tBlkEpochRelB;
reg			tBlkEpochCmp;

reg			tBlkDirtyA;
reg			tBlkFlushA;
reg			tBlkDirtyAL;
reg			tBlkFlushAL;

reg[127:0]	tBlkData0AL;
reg[127:0]	tBlkData1AL;
reg[127:0]	tBlkData2AL;
reg[127:0]	tBlkData3AL;

reg[127:0]	tBlkData0AC3;
reg[127:0]	tBlkData1AC3;
reg[127:0]	tBlkData2AC3;
reg[127:0]	tBlkData3AC3;

reg[127:0]	tBlkData0AC4;
reg[127:0]	tBlkData1AC4;
reg[127:0]	tBlkData2AC4;
reg[127:0]	tBlkData3AC4;

reg			tBlkFlushB;
reg			tBlkFlushBL;

`ifdef jx2_mem_l2d2way
reg[127:0]	tBlkData0B;
reg[127:0]	tBlkData1B;
reg[127:0]	tBlkData2B;
reg[127:0]	tBlkData3B;
reg[25:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlagB;
reg[ 3:0]	tBlkFrovB;
reg[ 3:0]	tBlkEpochB;

reg			tBlkDirtyB;
reg			tBlkDirtyBL;

reg[127:0]	tBlkData0BL;
reg[127:0]	tBlkData1BL;
reg[127:0]	tBlkData2BL;
reg[127:0]	tBlkData3BL;

reg[127:0]	tBlkData0BC3;
reg[127:0]	tBlkData1BC3;
reg[127:0]	tBlkData2BC3;
reg[127:0]	tBlkData3BC3;

`endif


reg[ 3:0]	tCurFrov;
reg[ 3:0]	tNxtFrov;

reg[3:0]	nxtRovIxCnt;
reg[3:0]	tRovIxCnt;

reg[ 3:0]	tCurEpoch;
reg[ 3:0]	tNxtEpoch;
reg[19:0]	tCurEpochCyc;
reg[19:0]	tNxtEpochCyc;
reg			tAdvEpoch;

reg[3:0]	tAccOpSq;
reg[3:0]	tAccOpSqDone;
reg[3:0]	tNxtAccOpSq;
reg			tAccIsOkSeq;


reg[511:0]		tBlkLdData;
reg[25:0]		tBlkLdAddr;
reg[3:0]		tBlkLdFrov;
reg				tBlkLdB;
reg				tBlkLdAzB;
reg				tBlkLdAtB;

`ifdef jx2_mem_l2d_ldatb
reg[511:0]		tBlkLdDataB;
reg[25:0]		tBlkLdAddrB;
`endif

reg[127:0]		tBlkStData0A;
reg[127:0]		tBlkStData1A;
reg[127:0]		tBlkStData2A;
reg[127:0]		tBlkStData3A;
reg[25:0]		tBlkStAddrA;
reg[3:0]		tBlkStFrovA;

reg[127:0]		tBlkStData0AL;
reg[127:0]		tBlkStData1AL;
reg[127:0]		tBlkStData2AL;
reg[127:0]		tBlkStData3AL;
reg[25:0]		tBlkStAddrAL;
reg[3:0]		tBlkStFrovAL;
reg				tBlkStDirtyAL;

reg				tBlkStDirtyA;
reg				tBlkStDirtyB;
reg				tBlkDoStAC3;
reg				tBlkDoStBC3;

reg				tBlkDoStA;
reg				tBlkDoStAL;
reg				tBlkDoStB;
reg				tBlkDoStBL;

`ifdef jx2_mem_l2d2way

reg[127:0]		tBlkStData0B;
reg[127:0]		tBlkStData1B;
reg[127:0]		tBlkStData2B;
reg[127:0]		tBlkStData3B;
reg[25:0]		tBlkStAddrB;
reg[3:0]		tBlkStFrovB;

reg[127:0]		tBlkStData0BL;
reg[127:0]		tBlkStData1BL;
reg[127:0]		tBlkStData2BL;
reg[127:0]		tBlkStData3BL;
reg[25:0]		tBlkStAddrBL;
reg[3:0]		tBlkStFrovBL;
reg				tBlkStDirtyBL;

`endif

reg		tMissAddr;
reg		tMissAddrB;
reg		tMissAddrL;
reg		tMissAddrBL;
reg		tMiss;
reg		tMissL;
reg		tMissL2;
reg		tMissL3;
reg		tHold;
reg		tHoldL;
reg		tHoldL2;
reg		tHoldL3;
reg		tAccess;

reg		tSkipC2;
reg		tSkipC3;
reg		tSkipC4;
reg		tHoldC4;

reg		tAddrIsRam;
reg		tBlkAddrIsRam;
reg 	tOpmIsNz;
reg 	tOpmIsLoad;
reg 	tOpmIsStore;
reg		tDoAcc;
reg		tDoSwAcc;
reg		tNxtDoAcc;
reg		tAccBlkDirty;
reg		tNxtAccBlkDirty;

reg		tAccBlkAddrIsRam;
reg		tNxtAccBlkAddrIsRam;

reg		tAccDoLdB;
reg		tAccDoLdAzB;
reg		tAccDoLdAtB;
reg		tNxtAccDoLdB;
reg		tNxtAccDoLdAzB;
reg		tNxtAccDoLdAtB;

reg[511:0]		tAccBlkData;
reg[511:0]		tNxtAccBlkData;
reg[25:0]		tAccBlkAddr;
reg[25:0]		tNxtAccBlkAddr;

// reg		tStLatch;
// reg		tNxtStLatch;

reg		tAccReady;
reg		tAccReadyL;

// reg		tSwFault;
// reg		tSwFaultL;
reg		tDbgFlushMiss;

reg		tAccLatch;
reg		tAccBusyLatch;
reg		tAccDone;
reg		tAccStDone;
reg		tNxtStDone;
reg		tAccSticky;
reg		tAccStickyLatch;
reg		tAccBlkHalf;
reg		tNxtBlkHalf;
reg 	tAccStoreOnly;
reg 	tNxtAccStoreOnly;

reg		tNxtDoFlushL2;
reg		tDoFlushL2;
reg		tDoFlushL2L;


wire		memRingIsIdle;
wire		memRingIsResp;
wire		memRingIsLdx;
wire		memRingIsStx;
wire		memRingIsPfx;
wire		memRingIsSpx;
assign	memRingIsIdle	= (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign	memRingIsLdx	= (memOpmIn[7:0] == JX2_RBI_OPM_LDX);
assign	memRingIsStx	= (memOpmIn[7:0] == JX2_RBI_OPM_STX);
assign	memRingIsPfx	= (memOpmIn[7:0] == JX2_RBI_OPM_PFX);
assign	memRingIsSpx	= (memOpmIn[7:0] == JX2_RBI_OPM_SPX);
assign	memRingIsResp	=
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire		memRingAddrIsRam;
assign	memRingAddrIsRam	=
		(memAddrIn[29:24] != 6'h00) &&
		(memAddrIn[31:30] == 2'b00);

wire		memRingAddrIsRamReq;
assign	memRingAddrIsRamReq	=
	memRingAddrIsRam && (memRingIsLdx || memRingIsStx ||
		memRingIsPfx || memRingIsSpx);

reg		mem2RingIsIdle;
reg		mem2RingIsResp;
reg		mem2RingIsLdx;
reg		mem2RingIsStx;
reg		mem2RingIsPfx;
reg		mem2RingIsSpx;

reg		mem2RingAddrIsRam;
reg		mem2RingAddrIsRamReq;

reg		mem3RingIsIdle;
reg		mem3RingIsResp;
reg		mem3RingIsLdx;
reg		mem3RingIsStx;
reg		mem3RingIsPfx;
reg		mem3RingIsSpx;

reg		mem3RingAddrIsRam;
reg		mem3RingAddrIsRamReq;

reg		mem4RingIsIdle;
reg		mem4RingIsResp;
reg		mem4RingIsLdx;
reg		mem4RingIsStx;
reg		mem4RingIsPfx;
reg		mem4RingIsSpx;

reg		mem4RingAddrIsRam;
reg		mem4RingAddrIsRamReq;

always @*
begin
	tNxtEpochCyc	= tCurEpochCyc + 1;
	tNxtEpoch		= tCurEpochCyc[17:14];

	if(reset)
		tNxtEpochCyc = 0;

	/* Cycle 1 */

	nxtReqAddr		= memAddrIn[31:6];
	nxtReqAddrLo	= memAddrIn[ 5:0];

`ifdef jx2_mem_l2wsz_1024
//	nxtReqIx	= nxtReqAddr [9:0] ^ nxtReqAddr [19:10];
//	nxtReqIx	= nxtReqAddr [9:0];
	nxtReqIx	= nxtReqAddr [9:0] ^
		{ nxtReqAddr [15:10], nxtReqAddr [19:16] };
`endif

`ifdef jx2_mem_l2wsz_2048
//	nxtReqIx	= nxtReqAddr [10:0] ^ nxtReqAddr [21:11];
	nxtReqIx	= nxtReqAddr [10:0] ^
		{ nxtReqAddr [15:11], nxtReqAddr [21:16] };
`endif

`ifdef jx2_mem_l2wsz_4096

//	nxtReqIx	= nxtReqAddr [11:0] ^ nxtReqAddr [23:12];
	nxtReqIx	= nxtReqAddr [11:0] ^
		{ nxtReqAddr [15:12], nxtReqAddr [19:16], nxtReqAddr [23:20] };
//	nxtReqIx	= nxtReqAddr [11:0] ^
//		{ nxtReqAddr [17:12], nxtReqAddr [23:18] };
//	nxtReqIx	= nxtReqAddr [11:0] ^
//		{	nxtReqAddr [14:12], nxtReqAddr [17:15],
//			nxtReqAddr [20:18], nxtReqAddr [23:21] };

`endif

`ifdef jx2_mem_l2wsz_8192
//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [24:12];
//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [25:13];
	nxtReqIx	= nxtReqAddr [12:0] ^
		{ nxtReqAddr [18:13], nxtReqAddr [25:19] };
`endif

`ifdef jx2_mem_l2wsz_16384
//	nxtReqIx	= nxtReqAddr [13:0] ^ nxtReqAddr [26:14];
//	nxtReqIx	= nxtReqAddr [13:0] ^ nxtReqAddr [25:13];
	nxtReqIx	= nxtReqAddr [13:0] ^
		{ nxtReqAddr [17:14], nxtReqAddr [19:16], nxtReqAddr [25:20] };
`endif

//	if(memRingAddrIsRamReq)
//	begin
//		$display("L2DC: nxtReqIx=%X A=%X", nxtReqIx, nxtReqAddr);
//	end

	tNxtAccOpSq	= tAccOpSq;

	if(memOpmIn[7:0] == 8'h00)
	begin
		nxtReqAddr	= tBlkAddrA[25:0];
		nxtReqIx	= tRovIx;
	end

	nxtReqIx2	= nxtReqIx;

`ifdef def_true
// `ifndef def_true
	nxtRovIx = tRovIx;
	nxtRovIxCnt	= tRovIxCnt + 1;
	if(tRovIxCnt == 0)
		nxtRovIx = tRovIx + 1;
`endif

	tNxtDoFlushL2	= 0;
	if(memOpmIn[7:0]==JX2_RBI_OPM_FLUSHDS)
	begin
		if(	(memAddrIn[31:28]==4'hF) &&
			(memAddrIn[ 3: 0]==4'hE))
		begin
			$display("L2 Flush");
			tNxtDoFlushL2 = 1;
		end
	end

	if((tCurFrov == 4'h0) && !tDoFlushL2 && !tDoFlushL2L && !reset)
		tNxtDoFlushL2 = 1;

	tNxtFrov	= tCurFrov;
	if(tDoFlushL2 && !tDoFlushL2L)
	begin
		$display("L2 Flush rov=%d", tCurFrov);
		tNxtFrov	= tCurFrov + 1;
	end

	if(reset)
	begin
		tNxtFrov	= 0;
	end

	/* Cycle 2 */
	
	tNxtAccIx		= tReqIx;
	tNxtAccAddr		= tReqAddr;
//	tNxtAccBlkData	= tBlkData;
	tNxtAccBlkData	= { tBlkData3A, tBlkData2A, tBlkData1A, tBlkData0A };
	tNxtAccBlkAddr	= tBlkAddrA;
	
	tNxtDoAcc		= 0;
	tNxtAccBlkDirty	= 0;
	tDoSwAcc		= 0;

	tNxtAccDoLdB	= 0;
	tNxtAccDoLdAzB	= 0;
	tNxtAccDoLdAtB	= 0;

	tSkipC2			= 0;
	tAccess			= 0;

	tNxtReqMissSeq	= tReqMissSeq;
	tNxtReqMissIx	= tReqMissIx;

	tBlkFlushA	= 0;
	tBlkDirtyA	= 0;
	
	tAccReady	= 1;

	tOpmIsLoad	=
		(tReqOpm[7:0]==JX2_RBI_OPM_LDX) || 
		(tReqOpm[7:0]==JX2_RBI_OPM_PFX);
	tOpmIsStore	=
		(tReqOpm[7:0]==JX2_RBI_OPM_STX) ||
		(tReqOpm[7:0]==JX2_RBI_OPM_SPX);
	tOpmIsNz	= tOpmIsLoad || tOpmIsStore ;

//	tNxtAccStoreOnly	= tOpmIsStore;
	tNxtAccStoreOnly	= 0;

	if(tOpmIsNz)
	begin
//		$display("L2DC: tReqIx=%X A=%X", tReqIx, tReqAddr);
	end

	tAddrIsRam	=
		(tReqAddr[23:18] != 6'h00) &&
		(tReqAddr[25:24] == 2'b00) ;
	tBlkAddrIsRam	=
		(tBlkAddrA[23:18] != 6'h00) &&
		(tBlkAddrA[25:24] == 2'b00) ;

`ifdef jx2_mem_l2d2way
	/* Select older cache line, or hash if same epoch. */
	tBlkEpochRel	= tCurEpoch - tBlkEpochA;
	tBlkEpochRelB	= tCurEpoch - tBlkEpochB;
	tBlkEpochCmp	= tBlkEpochRelB > tBlkEpochRel;
	if(tBlkEpochRelB == tBlkEpochRel)
		tBlkEpochCmp	= tBlkAddrA[0] ^ tBlkAddrA[7];
`endif

	tBlkFlushA		= (tCurFrov != tBlkFrovA) || (tCurFrov == 0);
`ifdef jx2_mem_l2d2way
	tBlkFlushB		= (tCurFrov != tBlkFrovB) || (tCurFrov == 0);
`endif

	tMissAddr	=
		(tReqAddr [25:16] != tBlkAddrA[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrA[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrA[ 7: 0]);

`ifdef jx2_mem_l2d2way
	tMissAddrB	=
		(tReqAddr [25:16] != tBlkAddrB[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrB[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrB[ 7: 0]);
`endif

`ifndef jx2_mem_l2d2way
	tBlkFlushB	= 0;
	tMissAddrB	= 1;
`endif

	if(tBlkIx != tReqIx)
	begin
		$display("L2DC: Not Ready Fault");
		tAccReady	= 0;
	end

	tBlkDirtyA			= tBlkFlagA[0];
	tNxtAccBlkAddrIsRam	= tBlkAddrIsRam;

	tMiss		=
		 (tMissAddr || tBlkFlushA) &&
		(tOpmIsNz && tAddrIsRam);
//	tNxtAccNoClashA	= tMissAddr;
//	tNxtAccNoClashA	= tMissAddr && !tBlkFlushA;
	tNxtAccNoClashA	= 	
//		((tReqAddr [15: 8] != tBlkAddrA[15: 8]) ||
//		 (tReqAddr [ 7: 0] != tBlkAddrA[ 7: 0])) &&
		!tBlkFlushA && !tBlkFlushB;

//	tNxtAccNoClashA	= 	0;

`ifdef jx2_mem_l2d2way
	tBlkDirtyB			= tBlkFlagB[0];

	tMiss		=
		(tMissAddr || tBlkFlushA) &&
		(tMissAddrB || tBlkFlushB ||
			tOpmIsStore || tBlkFlushA) &&
//			(tOpmIsStore && tBlkDirtyA) || tBlkFlushA) &&
		(tOpmIsNz && tAddrIsRam);
`endif

// `ifdef def_true
`ifndef def_true
	/* Auto-evict old dirty lines when idle. */
	if(tBlkEpochRel[3] && tBlkDirty && !tOpmIsNz && !tMissAddr)
	begin
//		tMiss = 1;
		tBlkFlush	= 1;
		tMiss		= 1;
		tAccess		= 1;
	end
`endif

`ifdef jx2_mem_l2d2way
	tNxtAccDoLdB	= 0;
	if(tOpmIsLoad && (tBlkDirtyA || tBlkEpochCmp) && !tBlkFlushA)
		tNxtAccDoLdB	= 1;
	if(tBlkEpochRel[3] && tBlkDirtyA)
		tNxtAccDoLdB	= 0;
	if(tBlkFlushA || tBlkFlushB)
		tNxtAccDoLdAzB	= 1;		
//	if(tNxtAccDoLdB)
//		$display("L2DC: Do Load B");

	if(tMiss && !tMissAddrB && !tNxtAccDoLdB)
		tNxtAccDoLdAzB	= 1;

//	if(!tNxtAccDoLdB && tBlkDirtyA && !tBlkFlushA)
	if(!tNxtAccDoLdB && !tBlkFlushA)
//		tNxtAccDoLdAtB	= 1;
		tNxtAccDoLdAtB	= tNxtAccDoLdAzB || !tBlkEpochCmp;		

`endif

	if((tBlkFlushA && !tMissAddr) && tMiss && !tMissL)
	begin
//		$display("L2DC: Flush Flag, Ix=%X A=%X Rov=%X,%X",
//			tReqIx, tReqAddr, tCurFrov, tBlkFrov);
	end
	
	if(tMiss && !tMissL)
	begin
//		$display("L2DC: Miss Ix=%X ReqAddr=%X BlkAddr=%X",
//			tReqIx, tReqAddr, tBlkAddr);
	end

	tDbgFlushMiss = 0;

//	tMemDataOut	= tBlkData;

//	tHold		= tMiss || tAccLatch || !tAccReady || tAccSticky;
//	tHold		= tMiss || tAccBusyLatch || !tAccReady || tAccSticky;
	tHold		= tMiss;
//	tAccess		= 0;
	
`ifndef def_true
	if(tHold)
	begin
		$display("L2DC: Miss=%X AccLatch=%X AccReady=%X AccSticky=%X",
			tMiss, tAccLatch, tAccReady, tAccSticky);
	end
`endif
	
	if(tMiss && (tReqMissSeq!=0) &&
		(tReqSeq!=tReqMissSeq) &&
		(tReqIx==tReqMissIx))
	begin
//		$display("L2DC: Pogo Miss Ix=%X ReqAddr=%X BlkAddr=%X",
//			tReqIx, tReqAddr, tBlkAddr);
		tHold		= 0;
		tSkipC2		= 1;
	end
	
	tBlkStData0A	= tBlkData0A;
	tBlkStData1A	= tBlkData1A;
	tBlkStData2A	= tBlkData2A;
	tBlkStData3A	= tBlkData3A;
	tBlkStAddrA		= UV26_00;
	tBlkStFrovA		= tCurFrov;

`ifdef jx2_mem_l2d2way
	tBlkStData0B	= tBlkData0B;
	tBlkStData1B	= tBlkData1B;
	tBlkStData2B	= tBlkData2B;
	tBlkStData3B	= tBlkData3B;
	tBlkStAddrB		= UV26_00;
	tBlkStFrovB		= tCurFrov;
`endif

	tBlkStIx	= tBlkIx;

	tBlkStDirtyA	= 0;
	tBlkDoStA		= 0;
`ifdef jx2_mem_l2d2way
	tBlkStDirtyB	= 0;
`endif
	tBlkDoStB		= 0;


`ifdef jx2_mem_l2d2way

// `ifdef def_true
`ifdef jx2_mem_l2d_stswab
	/* See if we can handle miss by swapping blocks. */
	if(tOpmIsStore && !tBlkDirtyA &&
		!tAccSticky && tMiss && tMissAddr && !tMissAddrB &&
		!tBlkFlushA && !tBlkFlushB && !tAccBusyLatch)
	begin
		tBlkStIx		= tBlkIx;

		tBlkStData0A	= tBlkData0B;
		tBlkStData1A	= tBlkData1B;
		tBlkStData2A	= tBlkData2B;
		tBlkStData3A	= tBlkData3B;
		tBlkStAddrA		= tBlkAddrB;
		tBlkStFrovA		= tCurFrov;
		tBlkStDirtyA	= 0;
		tBlkDoStA		= 1;

		tBlkStData0B	= tBlkData0A;
		tBlkStData1B	= tBlkData1A;
		tBlkStData2B	= tBlkData2A;
		tBlkStData3B	= tBlkData3A;
		tBlkStAddrB		= tBlkAddrA;
		tBlkStFrovB		= tCurFrov;
		tBlkStDirtyB	= 0;
		tBlkDoStB		= 1;

		tSkipC2			= 1;
	end
`endif

`endif


	if(tAccSticky)
	begin
`ifdef jx2_mem_l2d2way
		tBlkStIx		= tBlkLdIx;
		
		if(tBlkLdB)
		begin
//			$display("L2DC: Load Set B");
			tBlkStData0B	= tBlkLdData[127:  0];
			tBlkStData1B	= tBlkLdData[255:128];
			tBlkStData2B	= tBlkLdData[383:256];
			tBlkStData3B	= tBlkLdData[511:384];
			tBlkStAddrB		= tBlkLdAddr;
			tBlkStFrovB		= tCurFrov;
			tBlkStDirtyB	= 0;
			tBlkDoStB		= 1;
		end
		else
		begin
			tBlkStData0A	= tBlkLdData[127:  0];
			tBlkStData1A	= tBlkLdData[255:128];
			tBlkStData2A	= tBlkLdData[383:256];
			tBlkStData3A	= tBlkLdData[511:384];
			tBlkStAddrA		= tBlkLdAddr;
			tBlkStFrovA		= tCurFrov;
			tBlkStDirtyA	= 0;
			tBlkDoStA		= 1;

`ifdef jx2_mem_l2d_ldatb
			if(tBlkLdAtB)
			begin
				tBlkStData0B	= tBlkLdDataB[127:  0];
				tBlkStData1B	= tBlkLdDataB[255:128];
				tBlkStData2B	= tBlkLdDataB[383:256];
				tBlkStData3B	= tBlkLdDataB[511:384];
				tBlkStAddrB		= tBlkLdAddrB;
				tBlkStFrovB		= tCurFrov;
				tBlkStDirtyB	= 0;
				tBlkDoStB		= 1;
			end
			else
`endif

			if(tBlkLdAzB)
			begin
				tBlkStAddrB		= 0;
				tBlkStFrovB		= tCurFrov;
				tBlkStDirtyB	= 0;
				tBlkDoStB		= 1;
			end
		end
`else
		tBlkStData0A	= tBlkLdData[127:  0];
		tBlkStData1A	= tBlkLdData[255:128];
		tBlkStData2A	= tBlkLdData[383:256];
		tBlkStData3A	= tBlkLdData[511:384];
		tBlkStAddrA		= tBlkLdAddr;
		tBlkStIx		= tBlkLdIx;
//		tBlkStFrovA		= tBlkLdFrov;
		tBlkStFrovA		= tCurFrov;
		tBlkStDirtyA	= 0;
		tBlkDoStA		= 1;
`endif

//		if(tOpmIsNz)
		if(tOpmIsNz && (tReqIx == tBlkLdIx))
		begin
//			$display("L2 Hold: Request Sticky");
			tAccess	= 1;
//			tHold	= 1;

			if(!tHold)
				tSkipC2	= 1;

//			tNxtStLatch	= tStLatch;
//			tSwFault	= tSwFaultL;
		end
	end
	
	if(tAccBusyLatch)
//	if(1'b1)
	begin
//		if(tOpmIsNz && (tReqIx == tAccIx))
		if(tOpmIsNz && (tReqIx == tAccIx) && !tMiss)
//			tSkipC2		= 1;
			tHold		= 1;
	end

	if(tOpmIsNz && !tAddrIsRam)
		tSkipC2		= 1;
		
	if(tAddrIsRam)
	begin
		if(tOpmIsNz)
		begin
//			if(tBlkDoStC3)
//			if(tBlkDoStC3 && (tBlkStIxL == tBlkIx))
//			if(tBlkDoStL && (tBlkStIxL == tBlkIx))
			if((tBlkDoStAL || tBlkDoStBL) && (tBlkStIxL == tBlkIx))
			begin
//				$display("L2 Hold: Did St");
//				tHold		= 1;
				if(!tHold)
				begin
//					$display("L2DC: Load Skip 1");
					tSkipC2		= 1;
				end
			end
		end
	
// `ifndef def_true
`ifdef def_true
		if(	(tReqOpm[7:0]==JX2_RBI_OPM_PFX) ||
			(tReqOpm[7:0]==JX2_RBI_OPM_SPX))
		begin
			tAccess		= 1;
		end
		else if(tReqOpm[7:0]==JX2_RBI_OPM_LDX)
`else
		if(	(tReqOpm[7:0]==JX2_RBI_OPM_LDX) ||
			(tReqOpm[7:0]==JX2_RBI_OPM_PFX) ||
			(tReqOpm[7:0]==JX2_RBI_OPM_SPX)	)
`endif
		begin
//			$display("L2: LDX");
		
			tAccess		= 1;
//			tNxtStLatch	= 0;
		end
		else if(tReqOpm[7:0]==JX2_RBI_OPM_STX)
		begin
//			$display("L2: STX");

//			if(!tStLatch)
//			begin
//				tHold	= 1;
//				$display("L2 Hold: St Latch");
//			end

//			if(tReqIx == tAccIx)
//				tSkipC2		= 1;

			tAccess		= 1;

//			if(!tMiss && tAccReady && !tStLatch && !tSkipC2)
//			if(!tMiss && tAccReady && !tSkipC2)
//			if(!tMiss && tAccReady && !tSkipC2)
			if(!tMiss && tAccReady && !tSkipC2 && !tAccSticky)
			begin
				if(tReqAddrLo[5:4]==2'b00)
					tBlkStData0A	= tReqDataIn;
				if(tReqAddrLo[5:4]==2'b01)
					tBlkStData1A	= tReqDataIn;
				if(tReqAddrLo[5:4]==2'b10)
					tBlkStData2A	= tReqDataIn;
				if(tReqAddrLo[5:4]==2'b11)
					tBlkStData3A	= tReqDataIn;

				tBlkStIx		= tReqIx;

				tBlkStAddrA		= tReqAddr;
				tBlkStFrovA		= tCurFrov;
				tBlkStDirtyA	= 1;
				tBlkDoStA		= 1;

`ifdef jx2_mem_l2d2way
				if(!tMissAddrB)
				begin
//					$display("L2DC: Store Nuke B");
					tBlkStAddrB		= 0;
//					tBlkStFrovB		= 0;
					tBlkStFrovB		= tCurFrov;
					tBlkDoStB		= 1;
				end
`endif
			end
			else
			begin
				if(!tHold && !tSkipC2)
				begin
//					$display("L2DC Store Miss: Ix=%X A=%X",
//						tReqIx, tReqAddr);
					tSkipC2		= 1;
				end
			end
		end

	end

	if(tAccess && tMiss && tAccBusyLatch)
		tSkipC2 = 1;

	tNxtDoAcc	= (tAccess && tMiss && !tSkipC2) &&
		tAccReady && !tAccDone && !(tBlkDoStAL || tBlkDoStBL) && !reset;
	tNxtAccBlkDirty	= tBlkDirtyA;
	
//	if(tAccDone || reset)
//		tNxtDoAcc = 0;

	if(tAccBusyLatch && !reset)
	begin
		tNxtAccIx			= tAccIx;
		tNxtAccAddr			= tAccAddr;
		tNxtAccStoreOnly	= tAccStoreOnly;

		tNxtAccBlkData		= tAccBlkData;
		tNxtAccBlkAddr		= tAccBlkAddr;
		tNxtAccBlkDirty		= tAccBlkDirty;
		tNxtAccBlkAddrIsRam	= tAccBlkAddrIsRam;
		tNxtAccNoClashA		= tAccNoClashA;

		tNxtAccDoLdB		= tAccDoLdB;
		tNxtAccDoLdAzB		= tAccDoLdAzB;
		tNxtAccDoLdAtB		= tAccDoLdAtB;
	end

	if(tAccess && tMiss && !tSkipC2)
	begin
		tHold = 1;
	end
	
	if(tNxtDoAcc)
	begin
		tNxtReqMissSeq = tReqSeq;
		tNxtReqMissIx = tReqIx;
	
//		$display("L2 Hold: Do Acc");
		tHold = 1;
	end

	if(tNxtDoAcc && !tDoAcc)
	begin
		tNxtAccOpSq = tAccOpSq + 1;
	end

`ifdef def_true
// `ifndef def_true
	if(tHold)
		tSkipC2 = 1;
	tHold = 0;		//Non-Blocking Mode
`endif

	if(tHold)
	begin
// `ifndef def_true
`ifdef def_true
		$display("Hold=%d Miss=%d Skip=%d Ready=%d Done=%d DoStL=%d Cyc=%d",
			tHold, tMiss, tSkipC2,
			tAccReady, tAccDone, tBlkDoStAL, tCurEpochCyc);
`endif

		nxtReqIx2	= tReqIx;
	end
	
	/* Cycle 3 */

//	$display("Hold=%d Miss=%d", tHold, tMiss);

// `ifndef def_true
`ifdef def_true
	tMemSeqReq		= mem3SeqIn;
	tMemOpmReq		= mem3OpmIn;
	tMemAddrReq		= mem3AddrIn;

//	tMemDataReq		= mem3AddrIn[4] ?
//		tBlkDataC3[255:128] : tBlkDataC3[127:0];

//	if(mem3AddrIn[5:4]==2'b00)
		tMemDataReq		= tBlkData0AC3;
	if(mem3AddrIn[5:4]==2'b01)
		tMemDataReq		= tBlkData1AC3;
	if(mem3AddrIn[5:4]==2'b10)
		tMemDataReq		= tBlkData2AC3;
	if(mem3AddrIn[5:4]==2'b11)
		tMemDataReq		= tBlkData3AC3;

`ifdef jx2_mem_l2d2way
	if(	 (tMissAddrL || tBlkFlushAL) &&
		!(tMissAddrBL || tBlkFlushBL))
	begin
		if(mem3AddrIn[5:4]==2'b00)
			tMemDataReq		= tBlkData0BC3;
		if(mem3AddrIn[5:4]==2'b01)
			tMemDataReq		= tBlkData1BC3;
		if(mem3AddrIn[5:4]==2'b10)
			tMemDataReq		= tBlkData2BC3;
		if(mem3AddrIn[5:4]==2'b11)
			tMemDataReq		= tBlkData3BC3;
	end
`endif


	tMemSkipReq		= tSkipC3 || tHoldL || tHoldL2 || tHoldL3;

	tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
	if(mem3RingIsStx)
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKST;

	if(mem3RingIsPfx || mem3RingIsSpx)
	begin
		tMemOpmReq[14] = tMemSkipReq;
		tMemSkipReq = 0;
	end

	tMemOpmReq[3:0] = mem3OpmIn[11:8];

	if((tReqSeqC3!=mem3SeqIn) && !tMemSkipReq && !tHold)
	begin
		$display("L2DC: Request Misalignment");
		tMemSkipReq		= 1;
	end
`endif

	/* Cycle 4 */

	if(!tMemSkipReq && !tHold &&
		(tReqMissSeq != 0) &&
		(tMemSeqReq == tReqMissSeq))
	begin
//		$display("L2DC: Clear Miss Seq");
		tNxtReqMissSeq	= 0;
	end

`ifdef jx2_mem_l2dokseq
	tAccIsOkSeq		=
		(tDdrMemOK == UMEM_OK_OK) &&
		(tDdrMemOpSqI == tAccOpSqDone) &&
		(tDdrMemOpSqI != 0);
`else
	tAccIsOkSeq		= 0;
`endif

//	tAccIsOkSeq		= 0;

	if(tAccIsOkSeq)
	begin
//		$display("L2DC: tAccIsOkSeq");
	end

	if(reset)
	begin
		tNxtDoAcc		= 0;
		tNxtReqMissSeq	= 0;
		tNxtReqMissIx	= 0;
		tNxtAccOpSq		= 0;
		tAccIsOkSeq		= 0;
	end

end

always @(posedge clock)
begin
	if(!tHold)
//	if(1'b1)
	begin
		/* Cycle 1->2 */
		tReqAddr	<= nxtReqAddr;
		tReqAddrLo	<= nxtReqAddrLo;
		tReqIx		<= nxtReqIx;
		tReqOpm		<= memOpmIn;
		tReqDataIn	<= memDataIn;
		tReqSeq		<= memSeqIn;

		/* Cycle 2->3 */
		tSkipC3			<= tSkipC2;
		tBlkData0AC3	<= tBlkData0A;
		tBlkData1AC3	<= tBlkData1A;
		tBlkData2AC3	<= tBlkData2A;
		tBlkData3AC3	<= tBlkData3A;
		tBlkDoStAC3		<= tBlkDoStA;
		tReqSeqC3		<= tReqSeq;

`ifdef jx2_mem_l2d2way
		tBlkData0BC3	<= tBlkData0B;
		tBlkData1BC3	<= tBlkData1B;
		tBlkData2BC3	<= tBlkData2B;
		tBlkData3BC3	<= tBlkData3B;
		tBlkDoStBC3		<= tBlkDoStB;
`endif

		/* Cycle 3->4 */
//		tSkipC4			<= tSkipC3;
//		tBlkDataC4		<= tBlkDataC3;
//		tHoldC4			<= tHoldL;
//		tReqSeqC4		<= tReqSeqC3;
	end

//	$display("L2: Edge");

	tCurEpoch		<= tNxtEpoch;
	tCurEpochCyc	<= tNxtEpochCyc;

	tRovIx			<= nxtRovIx;
	tRovIxCnt		<= nxtRovIxCnt;

	tCurFrov		<= tNxtFrov;
	tDoFlushL2		<= tNxtDoFlushL2;
	tDoFlushL2L		<= tDoFlushL2;

	tReqMissSeq		<= tNxtReqMissSeq;
	tReqMissIx		<= tNxtReqMissIx;

	tReqAddrL		<= tReqAddr;
	tReqIxL			<= tReqIx;
	tReqOpmL		<= tReqOpm;
	tReqOpmL2		<= tReqOpmL;
	tReqOpmL3		<= tReqOpmL2;

	tReqIxL2		<= tReqIxL;

	tBlkDirtyAL		<= tBlkDirtyA;
	tBlkFlushAL		<= tBlkFlushA;
	tAccReadyL		<= tAccReady;

	tHoldL			<= tHold;
	tHoldL2			<= tHoldL;
	tHoldL3			<= tHoldL2;
	tMissL			<= tMiss;
	tMissL2			<= tMissL;
	tMissL3			<= tMissL2;
	tMissAddrL		<= tMissAddr;

`ifdef jx2_mem_l2d2way
	tMissAddrBL		<= tMissAddrB;
	tBlkFlushBL		<= tBlkFlushB;
`endif

//	tSwFaultL		<= tSwFault;

//	tStLatch			<= tNxtStLatch;
	tDoAcc				<= tNxtDoAcc;
	tAccAddr			<= tNxtAccAddr;
	tAccNoClashA		<= tNxtAccNoClashA;
	tAccIx				<= tNxtAccIx;
	tAccDoLdB			<= tNxtAccDoLdB;
	tAccDoLdAzB			<= tNxtAccDoLdAzB;
	tAccDoLdAtB			<= tNxtAccDoLdAtB;

	tAccBlkDirty		<= tNxtAccBlkDirty;
	tAccBlkData			<= tNxtAccBlkData;
	tAccBlkAddr			<= tNxtAccBlkAddr;
	tAccStoreOnly		<= tNxtAccStoreOnly;
	tAccBlkAddrIsRam	<= tNxtAccBlkAddrIsRam;

	tDdrMemDataOutB	<= tDdrMemDataOut;
	tDdrMemAddrB	<= tDdrMemAddr;
	tDdrMemAddrSwB	<= tDdrMemAddrSw;
	tDdrMemOpmB		<= tDdrMemOpm;
	tDdrMemOpSqB	<= tDdrMemOpSq;

`ifdef jx2_l2d_nofw_ddrin
	tDdrMemDataIn	<= ddrMemDataIn;
	tDdrMemOK		<= ddrMemOK;
	tDdrMemOpSqI	<= ddrOpSqI;
`else
	tDdrMemDataInB	<= ddrMemDataIn;
	tDdrMemOKB		<= ddrMemOK;
	tDdrMemOpSqIB	<= ddrOpSqI;
	tDdrMemDataIn	<= tDdrMemDataInB;
	tDdrMemOK		<= tDdrMemOKB;
	tDdrMemOpSqI	<= tDdrMemOpSqIB;
`endif

	tBlkData0A		<= memTileData0A[nxtReqIx2];
	tBlkData1A		<= memTileData1A[nxtReqIx2];
	tBlkData2A		<= memTileData2A[nxtReqIx2];
	tBlkData3A		<= memTileData3A[nxtReqIx2];
	{ tBlkEpochA, tBlkFrovA, tBlkFlagA, tBlkAddrA }	<=
		memTileAddrA[nxtReqIx2];
	tBlkIx								<= nxtReqIx2;

`ifdef jx2_mem_l2d2way
	tBlkData0B		<= memTileData0B[nxtReqIx2];
	tBlkData1B		<= memTileData1B[nxtReqIx2];
	tBlkData2B		<= memTileData2B[nxtReqIx2];
	tBlkData3B		<= memTileData3B[nxtReqIx2];
	{ tBlkEpochB, tBlkFrovB, tBlkFlagB, tBlkAddrB }	<=
		memTileAddrB[nxtReqIx2];
`endif

	tBlkStData0AL	<= tBlkStData0A;
	tBlkStData1AL	<= tBlkStData1A;
	tBlkStData2AL	<= tBlkStData2A;
	tBlkStData3AL	<= tBlkStData3A;
	tBlkStAddrAL	<= tBlkStAddrA;
	tBlkStFrovAL	<= tBlkStFrovA;
	tBlkStDirtyAL	<= tBlkStDirtyA;
	tBlkStIxL		<= tBlkStIx;

	tBlkData0AL		<= tBlkData0A;
	tBlkData1AL		<= tBlkData1A;
	tBlkData2AL		<= tBlkData2A;
	tBlkData3AL		<= tBlkData3A;
	tBlkDoStAL		<= tBlkDoStA;

`ifdef jx2_mem_l2d2way
	tBlkStData0BL		<= tBlkStData0B;
	tBlkStData1BL		<= tBlkStData1B;
	tBlkStData2BL		<= tBlkStData2B;
	tBlkStData3BL		<= tBlkStData3B;
	tBlkStAddrBL		<= tBlkStAddrB;
	tBlkStFrovBL		<= tBlkStFrovB;
	tBlkStDirtyBL		<= tBlkStDirtyB;

	tBlkData0BL			<= tBlkData0B;
	tBlkData1BL			<= tBlkData1B;
	tBlkData2BL			<= tBlkData2B;
	tBlkData3BL			<= tBlkData3B;
//	tBlkDoStBL			<= tBlkDoStB;
`endif

	tBlkDoStBL			<= tBlkDoStB;
//	tAccOpSq			<= tNxtAccOpSq;

//	tBlkDataC4		<= tBlkDataC3;

	if(tBlkDoStA)
	begin
//		$display("L2: Store Line A Ix=%X A=%X D=%X-%X-%X-%X, Cyc=%d",
//			tBlkStIx, tBlkStAddrA,
//				tBlkStData0A, tBlkStData1A,
//				tBlkStData2A, tBlkStData3A,
//				tCurEpochCyc);
		memTileData0A[tBlkStIx]	<= tBlkStData0A;
		memTileData1A[tBlkStIx]	<= tBlkStData1A;
		memTileData2A[tBlkStIx]	<= tBlkStData2A;
		memTileData3A[tBlkStIx]	<= tBlkStData3A;
		memTileAddrA[tBlkStIx]	<=
			{ tCurEpoch, tBlkStFrovA, 3'b100, tBlkStDirtyA, tBlkStAddrA};
		tAccSticky	<= 0;
	end

`ifdef jx2_mem_l2d2way
	if(tBlkDoStB)
	begin
//		$display("L2: Store Line B Ix=%X A=%X D=%X-%X-%X-%X, Cyc=%d",
//			tBlkStIx, tBlkStAddrB,
//			tBlkStData0B, tBlkStData1B,
//			tBlkStData2B, tBlkStData3B,
//			tCurEpochCyc);
		memTileData0B[tBlkStIx]	<= tBlkStData0B;
		memTileData1B[tBlkStIx]	<= tBlkStData1B;
		memTileData2B[tBlkStIx]	<= tBlkStData2B;
		memTileData3B[tBlkStIx]	<= tBlkStData3B;
		memTileAddrB[tBlkStIx]	<=
			{ tCurEpoch, tBlkStFrovB, 3'b100, tBlkStDirtyB, tBlkStAddrB};
		tAccSticky	<= 0;
	end
`endif

	if(reset)
	begin
		tAccLatch		<= 0;
		tAccBusyLatch	<= 0;
		tAccDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;

		tDdrMemOpm		<= UMEM_OPM_READY;
		tDdrMemOpSq		<= 0;
		tDdrMemAddr		<= 0;
		tDdrMemAddrSw	<= 0;
		tDdrMemDataOut	<= 0;
	end
	else
//		if(tDoAcc || tAccLatch)
		if(tDoAcc || tAccBusyLatch)
//		if(tNxtDoAcc || tAccBusyLatch)
	begin
//		$display("L2 DDR: Addr=%X OPM=%X OK=%X Sq=%X, Cyc=%d",
//			tDdrMemAddr, tDdrMemOpm, tDdrMemOK, tDdrMemOpSqI, tCurEpochCyc);

		if(!tAccDone)
			tAccBusyLatch	<= 1;

		if(tAccDone)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;
			tDdrMemOpSq		<= 0;

			/* Clear state for next access. */
			tAccStDone		<= 0;
			tNxtStDone		<= 0;
			tAccStickyLatch	<= 0;
			tAccBusyLatch	<= 0;
		end
		else
//			if((tDdrMemOK==UMEM_OK_OK) && tAccLatch)
			if((tDdrMemOK==UMEM_OK_OK) && tAccLatch && !tAccIsOkSeq)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;
			tDdrMemOpSq		<= 0;

`ifndef def_true
			if(tAccStickyLatch)
			begin
				/* Avoid setting sticky again. */
			end
			else
`endif

//				if(!tAccBlkDirty || tAccStDone)
				if(!tAccBlkDirty || tAccStDone || tAccStoreOnly)
			begin

`ifndef def_true
// `ifdef def_true
				if(tAccStoreOnly)
					$display("L2DC: Miss STO OK Ix=%X A=%X", tAccIx, tAccAddr);
				else
					$display("L2DC: Miss LD OK Ix=%X A=%X", tAccIx, tAccAddr);
`endif

				if(!tAccBlkDirty && tAccStoreOnly)
					$display("L2DC: STO Not-Dirty");
				
				tBlkLdData		<= tDdrMemDataIn;
//				tBlkLdAddr		<= tAccAddr;
//				tBlkLdAddr		<= tAccStoreOnly ? 0 : tAccAddr;
				tBlkLdAddr		<= tAccAddr;
				tBlkLdIx		<= tAccIx;
				tBlkLdFrov		<= tCurFrov;
//				tBlkLdB			<= tAccStoreOnly ? 0 : tAccDoLdB;
				tBlkLdB			<= tAccDoLdB;
				tBlkLdAzB		<= tAccDoLdAzB;
				tBlkLdAtB		<= tAccDoLdAtB;
				tAccDone		<= 1;
				tAccSticky		<= 1;
				tAccStickyLatch	<= 1;
				tAccLatch		<= 0;
				tAccBusyLatch	<= 0;

`ifdef jx2_mem_l2d_ldatb
				tBlkLdDataB		<= tAccBlkData;
				tBlkLdAddrB		<= tAccBlkAddr;
`endif

				if(!tAccDone)
				begin
//					$display("L2DC: OpSqA %X %X, In=%X",
//						tAccOpSqDone, tAccOpSq, tDdrMemOpSqI);
					tAccOpSqDone	<= tAccOpSq;
					tAccOpSq		<= tAccOpSq + 1;
				end
			end
			else if(tAccBlkDirty && !tNxtStDone)
			begin
//				$display("L2DC: Miss ST OK Ix=%X A=%X", tAccIx, tBlkAddr);
				tNxtStDone	<= 1;
				if(!tNxtStDone)
				begin
//					$display("L2DC: OpSqB %X %X",
//						tAccOpSqDone, tAccOpSq);
					tAccOpSqDone	<= tAccOpSq;
					tAccOpSq		<= tAccOpSq + 1;
				end
			end
		end
		else if((tDdrMemOK==UMEM_OK_HOLD) && tAccLatch)
		begin
			tDdrMemDataOut	<= tDdrMemDataOut;
			tDdrMemAddr		<= tDdrMemAddr;
			tDdrMemAddrSw	<= tDdrMemAddrSw;
			tDdrMemOpm		<= tDdrMemOpm;
			tDdrMemOpSq		<= tDdrMemOpSq;
		end
		else
//			if(tDdrMemOK==UMEM_OK_READY)
			if((tDdrMemOK==UMEM_OK_READY) || tAccIsOkSeq)
		begin
			tAccStDone	<= tNxtStDone;

			if(tAccBlkDirty && !tAccBlkAddrIsRam)
			begin
				$display("L2: Store Non-RAM Block Addr=%X", tBlkAddrA);
			end

			tDdrMemDataOut	<= tAccBlkData;
			tDdrMemAddrSw	<= {tAccBlkAddr, 6'h00};

`ifdef jx2_mem_ddrswap 
// `ifndef def_true
//			if(tAccBlkDirty && !tAccDone && tAccNoClashA)
			if(tAccBlkDirty && !tAccDone && tAccNoClashA && !tAccDoLdB)
			begin
				tDdrMemAddr		<= {tAccAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_SW_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
				tAccStDone		<= 1;
			end
			else if(tAccBlkDirty && !tNxtStDone)
			begin
				tDdrMemAddr		<= {tAccBlkAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemAddr		<= {tAccAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_RD_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
			end
`else
			if(tAccBlkDirty && !tNxtStDone)
			begin
//				tDdrMemDataOut	<= tAccBlkData;
				tDdrMemAddr		<= {tAccBlkAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
//				tAccStLatch		<= 1;
//				if(!tAccStLatch)
//					tAccOpSq	<= tAccOpSq + 1;
			end
			else if(!tAccDone)
			begin
//				tDdrMemDataOut	<= UVDDRW_00;
				tDdrMemAddr		<= {tAccAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_RD_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
//				tAccLdLatch		<= 1;
//				if(!tAccLdLatch)
//					tAccOpSq	<= tAccOpSq + 1;
			end
`endif
		end
		else
		begin
			tDdrMemOpm			<= UMEM_OPM_READY;
			tDdrMemOpSq			<= 0;
		end
	end
		else
	begin
		tAccDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;
		tAccStickyLatch	<= 0;
		tAccLatch		<= 0;
//		tAccBusyLatch	<= 0;

		tAccBusyLatch	<= tNxtDoAcc;

//		if(tNxtDoAcc)
//			tAccBusyLatch	<= 1;
//		else
//			tAccBusyLatch	<= 0;

		tDdrMemOpm		<= UMEM_OPM_READY;
		tDdrMemAddr		<= 0;
		tDdrMemDataOut	<= 0;
		tDdrMemOpSq		<= 0;
	end


	/* Cycle 1->2 */

	mem2SeqIn			<= memSeqIn;
	mem2OpmIn			<= memOpmIn;
	mem2AddrIn			<= memAddrIn;
	mem2DataIn			<= memDataIn;

	mem2RingIsIdle		<= memRingIsIdle;
	mem2RingIsResp		<= memRingIsResp;
	mem2RingIsLdx		<= memRingIsLdx;
	mem2RingIsStx		<= memRingIsStx;
	mem2RingIsPfx		<= memRingIsPfx;
	mem2RingIsSpx		<= memRingIsSpx;
	mem2RingAddrIsRam	<= memRingAddrIsRam;

	/* Cycle 2->3 */

	mem3SeqIn			<= mem2SeqIn;
	mem3OpmIn			<= mem2OpmIn;
	mem3AddrIn			<= mem2AddrIn;
	mem3DataIn			<= mem2DataIn;

	mem3RingIsIdle		<= mem2RingIsIdle;
	mem3RingIsResp		<= mem2RingIsResp;
	mem3RingIsLdx		<= mem2RingIsLdx;
	mem3RingIsStx		<= mem2RingIsStx;
	mem3RingIsPfx		<= mem2RingIsPfx;
	mem3RingIsSpx		<= mem2RingIsSpx;
	mem3RingAddrIsRam	<= mem2RingAddrIsRam;

	/* Cycle 3->4 */

`ifndef def_true
	mem4SeqIn			<= mem3SeqIn;
	mem4OpmIn			<= mem3OpmIn;
	mem4AddrIn			<= mem3AddrIn;
	mem4DataIn			<= mem3DataIn;

	mem4RingIsIdle		<= mem3RingIsIdle;
	mem4RingIsResp		<= mem3RingIsResp;
	mem4RingIsLdx		<= mem3RingIsLdx;
	mem4RingIsStx		<= mem3RingIsStx;
	mem4RingIsPfx		<= mem3RingIsPfx;
	mem4RingIsSpx		<= mem3RingIsSpx;
	mem4RingAddrIsRam	<= mem3RingAddrIsRam;
`endif

`ifdef def_true
	/* Cycle 4->Out */

	if(mem3RingAddrIsRam && (mem3RingIsLdx || mem3RingIsStx ||
			mem3RingIsPfx || mem3RingIsSpx) &&
		!tHold && !tMemSkipReq)
	begin
		tMemSeqOut		<= tMemSeqReq;
		tMemOpmOut		<= tMemOpmReq;
		tMemAddrOut		<= tMemAddrReq;
		tMemDataOut		<= tMemDataReq;
	end
	else
	begin

`ifndef def_true
		if((mem3RingIsLdx || mem3RingIsStx) && mem3RingAddrIsRam)
		begin
			$display("L2DC: Skip Req: IsRam=%X Hold=%X SkipReq=%X", 
				mem4RingAddrIsRam,
				tHold, tMemSkipReq);
		end
`endif

		tMemSeqOut		<= mem3SeqIn;
		tMemOpmOut		<= mem3OpmIn;
		tMemAddrOut		<= mem3AddrIn;
		tMemDataOut		<= mem3DataIn;
	end
`endif

end

endmodule

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

Modify L2 Cache to be 4-way set associative.
*/

`include "ringbus/RbiDefs.v"

`define jx2_mem_l2w4sz_1024

module RbiMemL2Dc4Wa(
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


`ifdef jx2_mem_l2w4sz_8192
`define		reg_l2w4idx		reg[12:0]
`define		reg_l2w4range	8191:0
`endif

`ifdef jx2_mem_l2w4sz_4096
`define		reg_l2w4idx		reg[11:0]
`define		reg_l2w4range	4095:0
`endif

`ifdef jx2_mem_l2w4sz_2048
`define		reg_l2w4idx		reg[10:0]
`define		reg_l2w4range	2047:0
`endif

`ifdef jx2_mem_l2w4sz_1024
`define		reg_l2w4idx		reg[ 9:0]
`define		reg_l2w4range	1023:0
`endif

`ifdef jx2_mem_l2w4sz_512
`define		reg_l2w4idx		reg[ 8:0]
`define		reg_l2w4range	511:0
`endif

`ifdef jx2_mem_l2w4sz_256
`define		reg_l2w4idx		reg[ 7:0]
`define		reg_l2w4range	255:0
`endif

reg[127:0]		memTileData0A[`reg_l2w4range];
reg[127:0]		memTileData1A[`reg_l2w4range];
reg[127:0]		memTileData2A[`reg_l2w4range];
reg[127:0]		memTileData3A[`reg_l2w4range];
reg[ 37:0]		memTileAddrA [`reg_l2w4range];

reg[127:0]		memTileData0B[`reg_l2w4range];
reg[127:0]		memTileData1B[`reg_l2w4range];
reg[127:0]		memTileData2B[`reg_l2w4range];
reg[127:0]		memTileData3B[`reg_l2w4range];
reg[ 37:0]		memTileAddrB [`reg_l2w4range];

reg[127:0]		memTileData0C[`reg_l2w4range];
reg[127:0]		memTileData1C[`reg_l2w4range];
reg[127:0]		memTileData2C[`reg_l2w4range];
reg[127:0]		memTileData3C[`reg_l2w4range];
reg[ 37:0]		memTileAddrC [`reg_l2w4range];

reg[127:0]		memTileData0D[`reg_l2w4range];
reg[127:0]		memTileData1D[`reg_l2w4range];
reg[127:0]		memTileData2D[`reg_l2w4range];
reg[127:0]		memTileData3D[`reg_l2w4range];
reg[ 37:0]		memTileAddrD [`reg_l2w4range];


`reg_l2w4idx	nxtReqIx;
`reg_l2w4idx	nxtReqIx2;
`reg_l2w4idx	tReqIx0;
`reg_l2w4idx	tReqIx;
`reg_l2w4idx	tBlkIx;
`reg_l2w4idx	tReqIxL;
`reg_l2w4idx	tReqIxL2;
`reg_l2w4idx	tBlkLdIx;
`reg_l2w4idx	tBlkStIx;
`reg_l2w4idx	tBlkStIxL;

`reg_l2w4idx	tAccIx;
`reg_l2w4idx	tNxtAccIx;
`reg_l2w4idx	nxtRovIx;
`reg_l2w4idx	tRovIx;

`reg_l2w4idx	tReqMissIx;
`reg_l2w4idx	tNxtReqMissIx;


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


reg[127:0]	tBlkData0B;
reg[127:0]	tBlkData1B;
reg[127:0]	tBlkData2B;
reg[127:0]	tBlkData3B;
reg[25:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlagB;
reg[ 3:0]	tBlkFrovB;
reg[ 3:0]	tBlkEpochB;

reg[127:0]	tBlkData0BL;
reg[127:0]	tBlkData1BL;
reg[127:0]	tBlkData2BL;
reg[127:0]	tBlkData3BL;

reg[127:0]	tBlkData0BC3;
reg[127:0]	tBlkData1BC3;
reg[127:0]	tBlkData2BC3;
reg[127:0]	tBlkData3BC3;

reg[127:0]	tBlkData0C;
reg[127:0]	tBlkData1C;
reg[127:0]	tBlkData2C;
reg[127:0]	tBlkData3C;
reg[25:0]	tBlkAddrC;
reg[ 3:0]	tBlkFlagC;
reg[ 3:0]	tBlkFrovC;
reg[ 3:0]	tBlkEpochC;

reg[127:0]	tBlkData0CL;
reg[127:0]	tBlkData1CL;
reg[127:0]	tBlkData2CL;
reg[127:0]	tBlkData3CL;

reg[127:0]	tBlkData0CC3;
reg[127:0]	tBlkData1CC3;
reg[127:0]	tBlkData2CC3;
reg[127:0]	tBlkData3CC3;

reg[127:0]	tBlkData0D;
reg[127:0]	tBlkData1D;
reg[127:0]	tBlkData2D;
reg[127:0]	tBlkData3D;
reg[25:0]	tBlkAddrD;
reg[ 3:0]	tBlkFlagD;
reg[ 3:0]	tBlkFrovD;
reg[ 3:0]	tBlkEpochD;

reg[127:0]	tBlkData0DL;
reg[127:0]	tBlkData1DL;
reg[127:0]	tBlkData2DL;
reg[127:0]	tBlkData3DL;

reg[127:0]	tBlkData0DC3;
reg[127:0]	tBlkData1DC3;
reg[127:0]	tBlkData2DC3;
reg[127:0]	tBlkData3DC3;


// reg[ 3:0]	tBlkEpochRel;
// reg[ 3:0]	tBlkEpochRelB;
// reg			tBlkEpochCmp;

reg			tBlkFlushA;
reg			tBlkFlushB;
reg			tBlkFlushC;
reg			tBlkFlushD;

reg			tBlkFlushAL;
reg			tBlkFlushBL;
reg			tBlkFlushCL;
reg			tBlkFlushDL;

reg			tBlkDirtyA;
reg			tBlkDirtyAL;
reg			tBlkDirtyB;
reg			tBlkDirtyBL;
reg			tBlkDirtyC;
reg			tBlkDirtyCL;
reg			tBlkDirtyD;
reg			tBlkDirtyDL;

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

reg[511:0]		tBlkLdDataB;
reg[25:0]		tBlkLdAddrB;

reg[511:0]		tBlkLdDataC;
reg[25:0]		tBlkLdAddrC;

reg[511:0]		tBlkLdDataD;
reg[25:0]		tBlkLdAddrD;

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

reg[127:0]		tBlkStData0C;
reg[127:0]		tBlkStData1C;
reg[127:0]		tBlkStData2C;
reg[127:0]		tBlkStData3C;
reg[25:0]		tBlkStAddrC;
reg[3:0]		tBlkStFrovC;

reg[127:0]		tBlkStData0CL;
reg[127:0]		tBlkStData1CL;
reg[127:0]		tBlkStData2CL;
reg[127:0]		tBlkStData3CL;
reg[25:0]		tBlkStAddrCL;
reg[3:0]		tBlkStFrovCL;
reg				tBlkStDirtyCL;

reg[127:0]		tBlkStData0D;
reg[127:0]		tBlkStData1D;
reg[127:0]		tBlkStData2D;
reg[127:0]		tBlkStData3D;
reg[25:0]		tBlkStAddrD;
reg[3:0]		tBlkStFrovD;

reg[127:0]		tBlkStData0DL;
reg[127:0]		tBlkStData1DL;
reg[127:0]		tBlkStData2DL;
reg[127:0]		tBlkStData3DL;
reg[25:0]		tBlkStAddrDL;
reg[3:0]		tBlkStFrovDL;
reg				tBlkStDirtyDL;


reg				tBlkStDirtyA;
reg				tBlkStDirtyB;
reg				tBlkStDirtyC;
reg				tBlkStDirtyD;
reg				tBlkDoStAC3;
reg				tBlkDoStBC3;
reg				tBlkDoStCC3;
reg				tBlkDoStDC3;

reg				tBlkDoStA;
reg				tBlkDoStB;
reg				tBlkDoStC;
reg				tBlkDoStD;
reg				tBlkDoStAL;
reg				tBlkDoStBL;
reg				tBlkDoStCL;
reg				tBlkDoStDL;


reg		tMissAddrA;
reg		tMissAddrB;
reg		tMissAddrC;
reg		tMissAddrD;
reg		tMissAddrAL;
reg		tMissAddrBL;
reg		tMissAddrCL;
reg		tMissAddrDL;
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

reg		tAccBlkDirtyA;
reg		tAccBlkDirtyB;
reg		tAccBlkDirtyC;
reg		tAccBlkDirtyD;
reg		tNxtAccBlkDirtyA;
reg		tNxtAccBlkDirtyB;
reg		tNxtAccBlkDirtyC;
reg		tNxtAccBlkDirtyD;

reg		tAccBlkAddrIsRam;
reg		tNxtAccBlkAddrIsRam;

reg		tAccDoLdB;
reg		tAccDoLdAzB;
reg		tAccDoLdAtB;
reg		tNxtAccDoLdB;
reg		tNxtAccDoLdAzB;
reg		tNxtAccDoLdAtB;

reg[511:0]		tAccBlkDataA;
reg[511:0]		tNxtAccBlkDataA;
reg[25:0]		tAccBlkAddrA;
reg[25:0]		tNxtAccBlkAddrA;

reg[511:0]		tAccBlkDataB;
reg[511:0]		tNxtAccBlkDataB;
reg[25:0]		tAccBlkAddrB;
reg[25:0]		tNxtAccBlkAddrB;

reg[511:0]		tAccBlkDataC;
reg[511:0]		tNxtAccBlkDataC;
reg[25:0]		tAccBlkAddrC;
reg[25:0]		tNxtAccBlkAddrC;

reg[511:0]		tAccBlkDataD;
reg[511:0]		tNxtAccBlkDataD;
reg[25:0]		tAccBlkAddrD;
reg[25:0]		tNxtAccBlkAddrD;

reg		tAccReady;
reg		tAccReadyL;

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

`ifdef jx2_mem_l2w4sz_256
	nxtReqIx	= nxtReqAddr [7:0] ^ nxtReqAddr [15:8];
`endif

`ifdef jx2_mem_l2w4sz_512
	nxtReqIx	= nxtReqAddr [8:0] ^ nxtReqAddr [17:9];
`endif

`ifdef jx2_mem_l2w4sz_1024
	nxtReqIx	= nxtReqAddr [9:0] ^ nxtReqAddr [19:10];
`endif

`ifdef jx2_mem_l2w4sz_2048
	nxtReqIx	= nxtReqAddr [10:0] ^ nxtReqAddr [21:11];
`endif

`ifdef jx2_mem_l2w4sz_4096
	nxtReqIx	= nxtReqAddr [11:0] ^ nxtReqAddr [23:12];
`endif

`ifdef jx2_mem_l2w4sz_8192
	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [24:12];
`endif

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
	tNxtAccBlkDataA	= { tBlkData3A, tBlkData2A, tBlkData1A, tBlkData0A };
	tNxtAccBlkDataB	= { tBlkData3B, tBlkData2B, tBlkData1B, tBlkData0B };
	tNxtAccBlkDataC	= { tBlkData3C, tBlkData2C, tBlkData1C, tBlkData0C };
	tNxtAccBlkDataD	= { tBlkData3D, tBlkData2D, tBlkData1D, tBlkData0D };
	tNxtAccBlkAddrA	= tBlkAddrA;
	tNxtAccBlkAddrB	= tBlkAddrB;
	tNxtAccBlkAddrC	= tBlkAddrC;
	tNxtAccBlkAddrD	= tBlkAddrD;
	
	tNxtDoAcc		= 0;

	tNxtAccBlkDirtyA	= 0;
	tNxtAccBlkDirtyB	= 0;
	tNxtAccBlkDirtyC	= 0;
	tNxtAccBlkDirtyD	= 0;

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
	
	tBlkFlushB	= 0;
	tBlkDirtyB	= 0;

	tBlkFlushC	= 0;
	tBlkDirtyC	= 0;

	tBlkFlushD	= 0;
	tBlkDirtyD	= 0;

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
	tBlkFlushB		= (tCurFrov != tBlkFrovB) || (tCurFrov == 0);
	tBlkFlushC		= (tCurFrov != tBlkFrovC) || (tCurFrov == 0);
	tBlkFlushD		= (tCurFrov != tBlkFrovD) || (tCurFrov == 0);

	tMissAddrA	=
		(tReqAddr [25:16] != tBlkAddrA[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrA[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrA[ 7: 0]);
	tMissAddrB	=
		(tReqAddr [25:16] != tBlkAddrB[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrB[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrB[ 7: 0]);
	tMissAddrC	=
		(tReqAddr [25:16] != tBlkAddrC[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrC[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrC[ 7: 0]);
	tMissAddrD	=
		(tReqAddr [25:16] != tBlkAddrD[25:16]) ||
		(tReqAddr [15: 8] != tBlkAddrD[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrD[ 7: 0]);


	if(tBlkIx != tReqIx)
	begin
		$display("L2DC: Not Ready Fault");
		tAccReady	= 0;
	end

	tBlkDirtyA			= tBlkFlagA[0];
	tNxtAccBlkAddrIsRam	= tBlkAddrIsRam;

	tBlkDirtyB			= tBlkFlagB[0];
	tBlkDirtyC			= tBlkFlagC[0];
	tBlkDirtyD			= tBlkFlagD[0];

	tMiss		=
		 (tMissAddrA || tBlkFlushA) &&
		(tOpmIsNz && tAddrIsRam);
	tNxtAccNoClashA	= 	
		!tBlkFlushA && !tBlkFlushB && !tBlkFlushC && !tBlkFlushD;

//	tNxtAccNoClashA	= 	0;

`ifdef jx2_mem_l2d2way
	tBlkDirtyB			= tBlkFlagB[0];

	tMiss		=
		(tMissAddrA || tBlkFlushA) &&
		(tMissAddrB || tBlkFlushB) &&
		(tMissAddrC || tBlkFlushC) &&
		(tMissAddrD || tBlkFlushD) &&
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

	tDbgFlushMiss = 0;

	tHold		= tMiss;
	
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

	tBlkStData0B	= tBlkData0B;
	tBlkStData1B	= tBlkData1B;
	tBlkStData2B	= tBlkData2B;
	tBlkStData3B	= tBlkData3B;
	tBlkStAddrB		= UV26_00;
	tBlkStFrovB		= tCurFrov;

	tBlkStData0C	= tBlkData0C;
	tBlkStData1C	= tBlkData1C;
	tBlkStData2C	= tBlkData2C;
	tBlkStData3C	= tBlkData3C;
	tBlkStAddrC		= UV26_00;
	tBlkStFrovC		= tCurFrov;

	tBlkStData0D	= tBlkData0D;
	tBlkStData1D	= tBlkData1D;
	tBlkStData2D	= tBlkData2D;
	tBlkStData3D	= tBlkData3D;
	tBlkStAddrD		= UV26_00;
	tBlkStFrovD		= tCurFrov;

	tBlkStIx	= tBlkIx;

	tBlkStDirtyA	= 0;
	tBlkStDirtyB	= 0;
	tBlkStDirtyC	= 0;
	tBlkStDirtyD	= 0;
	tBlkDoStA		= 0;
	tBlkDoStB		= 0;
	tBlkDoStC		= 0;
	tBlkDoStD		= 0;


	/* See if we can handle miss by swapping blocks. */

	if(	tOpmIsStore && !tAccSticky &&
		!tBlkFlushA && !tBlkFlushB &&
		!tBlkFlushC && !tBlkFlushD &&
		tMiss && !tAccBusyLatch)
	begin

		if(tMissAddrA && !tMissAddrB)
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
		else
			if(tMissAddrA && !tMissAddrC)
		begin
			tBlkStIx		= tBlkIx;

			tBlkStData0A	= tBlkData0C;
			tBlkStData1A	= tBlkData1C;
			tBlkStData2A	= tBlkData2C;
			tBlkStData3A	= tBlkData3C;
			tBlkStAddrA		= tBlkAddrC;
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

			tBlkStData0C	= tBlkData0B;
			tBlkStData1C	= tBlkData1B;
			tBlkStData2C	= tBlkData2B;
			tBlkStData3C	= tBlkData3B;
			tBlkStAddrC		= tBlkAddrB;
			tBlkStFrovC		= tCurFrov;
			tBlkStDirtyC	= 0;
			tBlkDoStC		= 1;

			tSkipC2			= 1;
		end
		else
			if(tMissAddrA && !tMissAddrD)
		begin
			tBlkStIx		= tBlkIx;

			tBlkStData0A	= tBlkData0D;
			tBlkStData1A	= tBlkData1D;
			tBlkStData2A	= tBlkData2D;
			tBlkStData3A	= tBlkData3D;
			tBlkStAddrA		= tBlkAddrD;
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

			tBlkStData0C	= tBlkData0B;
			tBlkStData1C	= tBlkData1B;
			tBlkStData2C	= tBlkData2B;
			tBlkStData3C	= tBlkData3B;
			tBlkStAddrC		= tBlkAddrB;
			tBlkStFrovC		= tCurFrov;
			tBlkStDirtyC	= 0;
			tBlkDoStC		= 1;

			tBlkStData0D	= tBlkData0C;
			tBlkStData1D	= tBlkData1C;
			tBlkStData2D	= tBlkData2C;
			tBlkStData3D	= tBlkData3C;
			tBlkStAddrD		= tBlkAddrC;
			tBlkStFrovD		= tCurFrov;
			tBlkStDirtyD	= 0;
			tBlkDoStD		= 1;

			tSkipC2			= 1;
		end

	end


	if(tAccSticky)
	begin
		tBlkStIx		= tBlkLdIx;
		
		tBlkStData0A	= tBlkLdData[127:  0];
		tBlkStData1A	= tBlkLdData[255:128];
		tBlkStData2A	= tBlkLdData[383:256];
		tBlkStData3A	= tBlkLdData[511:384];
		tBlkStAddrA		= tBlkLdAddr;
		tBlkStFrovA		= tCurFrov;
		tBlkStDirtyA	= 0;
		tBlkDoStA		= 1;

		tBlkStData0B	= tBlkLdDataB[127:  0];
		tBlkStData1B	= tBlkLdDataB[255:128];
		tBlkStData2B	= tBlkLdDataB[383:256];
		tBlkStData3B	= tBlkLdDataB[511:384];
		tBlkStAddrB		= tBlkLdAddrB;
		tBlkStFrovB		= tCurFrov;
		tBlkStDirtyB	= 0;
		tBlkDoStB		= 1;

		if(tOpmIsNz && (tReqIx == tBlkLdIx))
		begin
			tAccess	= 1;

			if(!tHold)
				tSkipC2	= 1;
		end
	end
	
	if(tAccBusyLatch)
	begin
		if(tOpmIsNz && (tReqIx == tAccIx) && !tMiss)
			tHold		= 1;
	end

	if(tOpmIsNz && !tAddrIsRam)
		tSkipC2		= 1;
		
	if(tAddrIsRam)
	begin
		if(tOpmIsNz)
		begin
			if((tBlkDoStAL || tBlkDoStBL) && (tBlkStIxL == tBlkIx))
			begin
				if(!tHold)
				begin
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
		end
		else if(tReqOpm[7:0]==JX2_RBI_OPM_STX)
		begin
			tAccess		= 1;

			if(!tMiss && tAccReady && !tSkipC2 && !tAccSticky)
			begin
				if(!tMissAddrA)
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
				end

				if(!tMissAddrB)
				begin
					if(tReqAddrLo[5:4]==2'b00)
						tBlkStData0B	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b01)
						tBlkStData1B	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b10)
						tBlkStData2B	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b11)
						tBlkStData3B	= tReqDataIn;

					tBlkStIx		= tReqIx;

					tBlkStAddrB		= tReqAddr;
					tBlkStFrovB		= tCurFrov;
					tBlkStDirtyB	= 1;
					tBlkDoStB		= 1;
				end

				if(!tMissAddrC)
				begin
					if(tReqAddrLo[5:4]==2'b00)
						tBlkStData0C	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b01)
						tBlkStData1C	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b10)
						tBlkStData2C	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b11)
						tBlkStData3C	= tReqDataIn;

					tBlkStIx		= tReqIx;

					tBlkStAddrC		= tReqAddr;
					tBlkStFrovC		= tCurFrov;
					tBlkStDirtyC	= 1;
					tBlkDoStC		= 1;
				end

				if(!tMissAddrD)
				begin
					if(tReqAddrLo[5:4]==2'b00)
						tBlkStData0D	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b01)
						tBlkStData1D	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b10)
						tBlkStData2D	= tReqDataIn;
					if(tReqAddrLo[5:4]==2'b11)
						tBlkStData3D	= tReqDataIn;

					tBlkStIx		= tReqIx;

					tBlkStAddrD		= tReqAddr;
					tBlkStFrovD		= tCurFrov;
					tBlkStDirtyD	= 1;
					tBlkDoStD		= 1;
				end
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
		tAccReady && !tAccDone &&
			!(tBlkDoStAL || tBlkDoStBL || tBlkDoStCL || tBlkDoStDL) &&
			!reset;

	tNxtAccBlkDirtyA	= tBlkDirtyA;
	tNxtAccBlkDirtyB	= tBlkDirtyB;
	tNxtAccBlkDirtyC	= tBlkDirtyC;
	tNxtAccBlkDirtyD	= tBlkDirtyD;

	if(tAccBusyLatch && !reset)
	begin
		tNxtAccIx			= tAccIx;
		tNxtAccAddr			= tAccAddr;
		tNxtAccStoreOnly	= tAccStoreOnly;

		tNxtAccBlkDataA		= tAccBlkDataA;
		tNxtAccBlkDataB		= tAccBlkDataB;
		tNxtAccBlkDataC		= tAccBlkDataC;
		tNxtAccBlkDataD		= tAccBlkDataD;

		tNxtAccBlkAddrA		= tAccBlkAddrA;
		tNxtAccBlkAddrB		= tAccBlkAddrB;
		tNxtAccBlkAddrC		= tAccBlkAddrC;
		tNxtAccBlkAddrD		= tAccBlkAddrD;

		tNxtAccBlkDirtyA	= tAccBlkDirtyA;
		tNxtAccBlkDirtyB	= tAccBlkDirtyB;
		tNxtAccBlkDirtyC	= tAccBlkDirtyC;
		tNxtAccBlkDirtyD	= tAccBlkDirtyD;

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

	if(!(tMissAddrAL || tBlkFlushAL))
	begin
		if(mem3AddrIn[5:4]==2'b00)
			tMemDataReq		= tBlkData0AC3;
		if(mem3AddrIn[5:4]==2'b01)
			tMemDataReq		= tBlkData1AC3;
		if(mem3AddrIn[5:4]==2'b10)
			tMemDataReq		= tBlkData2AC3;
		if(mem3AddrIn[5:4]==2'b11)
			tMemDataReq		= tBlkData3AC3;
	end else
		if(!(tMissAddrBL || tBlkFlushBL))
	begin
		if(mem3AddrIn[5:4]==2'b00)
			tMemDataReq		= tBlkData0BC3;
		if(mem3AddrIn[5:4]==2'b01)
			tMemDataReq		= tBlkData1BC3;
		if(mem3AddrIn[5:4]==2'b10)
			tMemDataReq		= tBlkData2BC3;
		if(mem3AddrIn[5:4]==2'b11)
			tMemDataReq		= tBlkData3BC3;
	end else
		if(!(tMissAddrCL || tBlkFlushCL))
	begin
		if(mem3AddrIn[5:4]==2'b00)
			tMemDataReq		= tBlkData0CC3;
		if(mem3AddrIn[5:4]==2'b01)
			tMemDataReq		= tBlkData1CC3;
		if(mem3AddrIn[5:4]==2'b10)
			tMemDataReq		= tBlkData2CC3;
		if(mem3AddrIn[5:4]==2'b11)
			tMemDataReq		= tBlkData3CC3;
	end else
		if(!(tMissAddrDL || tBlkFlushDL))
	begin
		if(mem3AddrIn[5:4]==2'b00)
			tMemDataReq		= tBlkData0DC3;
		if(mem3AddrIn[5:4]==2'b01)
			tMemDataReq		= tBlkData1DC3;
		if(mem3AddrIn[5:4]==2'b10)
			tMemDataReq		= tBlkData2DC3;
		if(mem3AddrIn[5:4]==2'b11)
			tMemDataReq		= tBlkData3DC3;
	end


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

		tBlkData0BC3	<= tBlkData0B;
		tBlkData1BC3	<= tBlkData1B;
		tBlkData2BC3	<= tBlkData2B;
		tBlkData3BC3	<= tBlkData3B;
		tBlkDoStBC3		<= tBlkDoStB;

		tBlkData0CC3	<= tBlkData0C;
		tBlkData1CC3	<= tBlkData1C;
		tBlkData2CC3	<= tBlkData2C;
		tBlkData3CC3	<= tBlkData3C;
		tBlkDoStCC3		<= tBlkDoStC;

		tBlkData0DC3	<= tBlkData0D;
		tBlkData1DC3	<= tBlkData1D;
		tBlkData2DC3	<= tBlkData2D;
		tBlkData3DC3	<= tBlkData3D;
		tBlkDoStDC3		<= tBlkDoStD;
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
	tBlkDirtyBL		<= tBlkDirtyB;
	tBlkDirtyCL		<= tBlkDirtyC;
	tBlkDirtyDL		<= tBlkDirtyD;

	tAccReadyL		<= tAccReady;

	tHoldL			<= tHold;
	tHoldL2			<= tHoldL;
	tHoldL3			<= tHoldL2;
	tMissL			<= tMiss;
	tMissL2			<= tMissL;
	tMissL3			<= tMissL2;
	tMissAddrAL		<= tMissAddrA;
	tBlkFlushAL		<= tBlkFlushA;

	tMissAddrBL		<= tMissAddrB;
	tBlkFlushBL		<= tBlkFlushB;

	tMissAddrCL		<= tMissAddrC;
	tBlkFlushCL		<= tBlkFlushD;

	tMissAddrDL		<= tMissAddrD;
	tBlkFlushDL		<= tBlkFlushD;

	tDoAcc				<= tNxtDoAcc;
	tAccAddr			<= tNxtAccAddr;
	tAccNoClashA		<= tNxtAccNoClashA;
	tAccIx				<= tNxtAccIx;
	tAccDoLdB			<= tNxtAccDoLdB;
	tAccDoLdAzB			<= tNxtAccDoLdAzB;
	tAccDoLdAtB			<= tNxtAccDoLdAtB;

	tAccBlkDirtyA		<= tNxtAccBlkDirtyA;
	tAccBlkDirtyB		<= tNxtAccBlkDirtyB;
	tAccBlkDirtyC		<= tNxtAccBlkDirtyC;
	tAccBlkDirtyD		<= tNxtAccBlkDirtyD;

	tAccBlkDataA		<= tNxtAccBlkDataA;
	tAccBlkDataB		<= tNxtAccBlkDataB;
	tAccBlkDataC		<= tNxtAccBlkDataC;
	tAccBlkDataD		<= tNxtAccBlkDataD;

	tAccBlkAddrA		<= tNxtAccBlkAddrA;
	tAccBlkAddrB		<= tNxtAccBlkAddrB;
	tAccBlkAddrC		<= tNxtAccBlkAddrC;
	tAccBlkAddrD		<= tNxtAccBlkAddrD;

	tAccStoreOnly		<= tNxtAccStoreOnly;
	tAccBlkAddrIsRam	<= tNxtAccBlkAddrIsRam;

	tDdrMemDataOutB		<= tDdrMemDataOut;
	tDdrMemAddrB		<= tDdrMemAddr;
	tDdrMemAddrSwB		<= tDdrMemAddrSw;
	tDdrMemOpmB			<= tDdrMemOpm;
	tDdrMemOpSqB		<= tDdrMemOpSq;

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

	tBlkData0B		<= memTileData0B[nxtReqIx2];
	tBlkData1B		<= memTileData1B[nxtReqIx2];
	tBlkData2B		<= memTileData2B[nxtReqIx2];
	tBlkData3B		<= memTileData3B[nxtReqIx2];
	{ tBlkEpochB, tBlkFrovB, tBlkFlagB, tBlkAddrB }	<=
		memTileAddrB[nxtReqIx2];

	tBlkData0C		<= memTileData0C[nxtReqIx2];
	tBlkData1C		<= memTileData1C[nxtReqIx2];
	tBlkData2C		<= memTileData2C[nxtReqIx2];
	tBlkData3C		<= memTileData3C[nxtReqIx2];
	{ tBlkEpochC, tBlkFrovC, tBlkFlagC, tBlkAddrC }	<=
		memTileAddrC[nxtReqIx2];

	tBlkData0D		<= memTileData0D[nxtReqIx2];
	tBlkData1D		<= memTileData1D[nxtReqIx2];
	tBlkData2D		<= memTileData2D[nxtReqIx2];
	tBlkData3D		<= memTileData3D[nxtReqIx2];
	{ tBlkEpochD, tBlkFrovD, tBlkFlagD, tBlkAddrD }	<=
		memTileAddrD[nxtReqIx2];


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

// `ifdef jx2_mem_l2d2way
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
	tBlkDoStBL			<= tBlkDoStB;
// `endif

// `ifdef jx2_mem_l2d2way
	tBlkStData0CL		<= tBlkStData0C;
	tBlkStData1CL		<= tBlkStData1C;
	tBlkStData2CL		<= tBlkStData2C;
	tBlkStData3CL		<= tBlkStData3C;
	tBlkStAddrCL		<= tBlkStAddrC;
	tBlkStFrovCL		<= tBlkStFrovC;
	tBlkStDirtyCL		<= tBlkStDirtyC;

	tBlkData0CL			<= tBlkData0C;
	tBlkData1CL			<= tBlkData1C;
	tBlkData2CL			<= tBlkData2C;
	tBlkData3CL			<= tBlkData3C;
	tBlkDoStCL			<= tBlkDoStC;
// `endif

// `ifdef jx2_mem_l2d2way
	tBlkStData0DL		<= tBlkStData0D;
	tBlkStData1DL		<= tBlkStData1D;
	tBlkStData2DL		<= tBlkStData2D;
	tBlkStData3DL		<= tBlkStData3D;
	tBlkStAddrDL		<= tBlkStAddrD;
	tBlkStFrovDL		<= tBlkStFrovD;
	tBlkStDirtyDL		<= tBlkStDirtyD;

	tBlkData0DL			<= tBlkData0D;
	tBlkData1DL			<= tBlkData1D;
	tBlkData2DL			<= tBlkData2D;
	tBlkData3DL			<= tBlkData3D;
	tBlkDoStDL			<= tBlkDoStD;
// `endif

//	tBlkDoStBL			<= tBlkDoStB;
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

	if(tBlkDoStC)
	begin
//		$display("L2: Store Line C Ix=%X A=%X D=%X-%X-%X-%X, Cyc=%d",
//			tBlkStIx, tBlkStAddrC,
//			tBlkStData0C, tBlkStData1C,
//			tBlkStData2C, tBlkStData3C,
//			tCurEpochCyc);
		memTileData0C[tBlkStIx]	<= tBlkStData0C;
		memTileData1C[tBlkStIx]	<= tBlkStData1C;
		memTileData2C[tBlkStIx]	<= tBlkStData2C;
		memTileData3C[tBlkStIx]	<= tBlkStData3C;
		memTileAddrC[tBlkStIx]	<=
			{ tCurEpoch, tBlkStFrovC, 3'b100, tBlkStDirtyC, tBlkStAddrC};
		tAccSticky	<= 0;
	end

	if(tBlkDoStD)
	begin
//		$display("L2: Store Line D Ix=%X A=%X D=%X-%X-%X-%X, Cyc=%d",
//			tBlkStIx, tBlkStAddrD,
//			tBlkStData0D, tBlkStData1D,
//			tBlkStData2D, tBlkStData3D,
//			tCurEpochCyc);
		memTileData0D[tBlkStIx]	<= tBlkStData0D;
		memTileData1D[tBlkStIx]	<= tBlkStData1D;
		memTileData2D[tBlkStIx]	<= tBlkStData2D;
		memTileData3D[tBlkStIx]	<= tBlkStData3D;
		memTileAddrD[tBlkStIx]	<=
			{ tCurEpoch, tBlkStFrovD, 3'b100, tBlkStDirtyD, tBlkStAddrD};
		tAccSticky	<= 0;
	end

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
				if(!tAccBlkDirtyD || tAccStDone || tAccStoreOnly)
			begin

`ifndef def_true
// `ifdef def_true
				if(tAccStoreOnly)
					$display("L2DC: Miss STO OK Ix=%X A=%X", tAccIx, tAccAddr);
				else
					$display("L2DC: Miss LD OK Ix=%X A=%X", tAccIx, tAccAddr);
`endif

				if(!tAccBlkDirtyD && tAccStoreOnly)
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

				tBlkLdDataB		<= tAccBlkDataA;
				tBlkLdAddrB		<= tAccBlkAddrA;

				tBlkLdDataC		<= tAccBlkDataB;
				tBlkLdAddrC		<= tAccBlkAddrB;

				tBlkLdDataD		<= tAccBlkDataC;
				tBlkLdAddrD		<= tAccBlkAddrC;

				if(!tAccDone)
				begin
//					$display("L2DC: OpSqA %X %X, In=%X",
//						tAccOpSqDone, tAccOpSq, tDdrMemOpSqI);
					tAccOpSqDone	<= tAccOpSq;
					tAccOpSq		<= tAccOpSq + 1;
				end
			end
			else if(tAccBlkDirtyD && !tNxtStDone)
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

			if(tAccBlkDirtyD && !tAccBlkAddrIsRam)
			begin
				$display("L2: Store Non-RAM Block Addr=%X", tBlkAddrA);
			end

			tDdrMemDataOut	<= tAccBlkDataD;
			tDdrMemAddrSw	<= {tAccBlkAddrD, 6'h00};

`ifdef jx2_mem_ddrswap 
//			if(tAccBlkDirty && !tAccDone && tAccNoClashA)
//			if(tAccBlkDirty && !tAccDone && tAccNoClashA && !tAccDoLdB)
			if(tAccBlkDirtyD && !tAccDone)
			begin
				tDdrMemAddr		<= {tAccAddr, 6'h00};
				tDdrMemOpm		<= UMEM_OPM_SW_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
				tAccStDone		<= 1;
			end
			else if(tAccBlkDirtyD && !tNxtStDone)
			begin
				tDdrMemAddr		<= {tAccBlkAddrD, 6'h00};
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
			if(tAccBlkDirtyD && !tNxtStDone)
			begin
				tDdrMemAddr		<= {tAccBlkAddrD, 6'h00};
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

		tAccBusyLatch	<= tNxtDoAcc;

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

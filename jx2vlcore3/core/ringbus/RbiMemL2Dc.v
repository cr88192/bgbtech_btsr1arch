/*
L2 Tile Cache, Ringbus

Caches 128-bit tiles.
*/

`include "ringbus/RbiDefs.v"

module RbiMemL2Dc(
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


`input_ddrtile	ddrMemDataIn;
`output_ddrtile	ddrMemDataOut;
output[31:0]	ddrMemAddr;
output[31:0]	ddrMemAddrSw;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;

input[3:0]		ddrOpSqI;
output[3:0]		ddrOpSqO;

input			regDeadlockStrobe;

`reg_ddrtile	tDdrMemDataIn;
`reg_ddrtile	tDdrMemDataInB;
`reg_ddrtile	tDdrMemDataOut;
`reg_ddrtile	tDdrMemDataOutB;

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
assign	ddrMemAddr		= tDdrMemAddrSwB;
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


`ifdef jx2_mem_l2sz_8192
`reg_ddrtile_pad	memTileData[8191:0];
reg[ 35:0]			memTileAddr[8191:0];
`endif

`ifdef jx2_mem_l2sz_4096
`reg_ddrtile_pad	memTileData[4095:0];
reg[ 35:0]			memTileAddr[4095:0];
`endif

`ifdef jx2_mem_l2sz_2048
`reg_ddrtile_pad	memTileData[2047:0];
reg[ 35:0]			memTileAddr[2047:0];
`endif

`ifdef jx2_mem_l2sz_1024
`reg_ddrtile_pad	memTileData[1023:0];
reg[ 35:0]			memTileAddr[1023:0];
`endif

`ifdef jx2_mem_l2d2way

`ifdef jx2_mem_l2sz_8192
`reg_ddrtile_pad	memTileDataB[8191:0];
reg[ 35:0]			memTileAddrB[8191:0];
`endif

`ifdef jx2_mem_l2sz_4096
`reg_ddrtile_pad	memTileDataB[4095:0];
reg[ 35:0]			memTileAddrB[4095:0];
`endif

`ifdef jx2_mem_l2sz_2048
`reg_ddrtile_pad	memTileDataB[2047:0];
reg[ 35:0]			memTileAddrB[2047:0];
`endif

`ifdef jx2_mem_l2sz_1024
`reg_ddrtile_pad	memTileDataB[1023:0];
reg[ 35:0]			memTileAddrB[1023:0];
`endif

`endif


`ifdef jx2_mem_l2sz_8192
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

`ifdef jx2_mem_l2sz_4096
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

`ifdef jx2_mem_l2sz_2048
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

`ifdef jx2_mem_l2sz_1024
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


reg[27:0]	nxtReqAddr;
reg[4:0]	nxtReqAddrLo;

reg[27:0]	tReqAddr;

reg[27:0]	tReqAddrL;
reg[27:0]	tAccAddr;
reg[27:0]	tNxtAccAddr;
reg			tAccNoClashA;
reg			tNxtAccNoClashA;

reg[4:0]	tReqAddrLo;

reg[15:0]	tReqOpm;
reg[15:0]	tReqOpmL;
reg[15:0]	tReqOpmL2;
reg[15:0]	tReqOpmL3;
`reg_tile	tReqDataIn;

reg[27:0]	tReqAddr0;
reg[4:0]	tReqAddrLo0;
reg[15:0]	tReqOpm0;
`reg_tile	tReqDataIn0;

reg[15:0]	tReqSeq;
reg[15:0]	tReqMissSeq;
reg[15:0]	tNxtReqMissSeq;

reg[15:0]	tReqSeqC3;
reg[15:0]	tReqSeqC4;

`reg_ddrtile	tBlkData;
reg[27:0]		tBlkAddr;
reg[ 3:0]		tBlkFlag;
reg[ 3:0]		tBlkFrov;
reg[ 3:0]		tBlkEpoch;

`ifdef jx2_mem_ddr32B
reg[9:0]		tBlkPad;
`else
reg[11:0]		tBlkPad;
`endif

reg[ 3:0]	tBlkEpochRel;
reg[ 3:0]	tBlkEpochRelB;
reg			tBlkEpochCmp;

reg			tBlkDirty;
reg			tBlkFlush;
reg			tBlkDirtyL;
reg			tBlkFlushL;
`reg_ddrtile	tBlkDataL;
`reg_ddrtile	tBlkDataC3;
`reg_ddrtile	tBlkDataC4;

// `reg_tile	tBlkData_A0;
// reg[27:0]	tBlkAddr_A0;
// reg[27:0]	tBlkAddrB_A0;
// reg[ 3:0]	tBlkFlag_A0;
// reg[ 3:0]	tBlkFlagB_A0;
// reg[ 3:0]	tBlkFrov_A0;
// reg[ 3:0]	tBlkFrovB_A0;


reg			tBlkFlushB;
reg			tBlkFlushBL;

`ifdef jx2_mem_l2d2way
`reg_ddrtile	tBlkDataB;
reg[27:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlagB;
reg[ 3:0]	tBlkFrovB;
reg[ 3:0]	tBlkEpochB;

`ifdef jx2_mem_ddr32B
reg[9:0]		tBlkPadB;
`else
reg[11:0]		tBlkPadB;
`endif

reg			tBlkDirtyB;
// reg			tBlkFlushB;
reg			tBlkDirtyBL;
// reg			tBlkFlushBL;
`reg_ddrtile	tBlkDataBL;
`reg_ddrtile	tBlkDataBC3;

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


`reg_ddrtile	tBlkLdData;
reg[27:0]		tBlkLdAddr;
reg[3:0]		tBlkLdFrov;
reg				tBlkLdB;
reg				tBlkLdAzB;
reg				tBlkLdAtB;

`reg_ddrtile	tBlkLdDataB;
reg[27:0]		tBlkLdAddrB;

`reg_ddrtile	tBlkStData;
reg[27:0]		tBlkStAddr;
reg[3:0]		tBlkStFrov;

`reg_ddrtile	tBlkStDataL;
reg[27:0]		tBlkStAddrL;
reg[3:0]		tBlkStFrovL;
reg				tBlkStDirtyL;

reg				tBlkStDirty;
reg				tBlkDoSt;
reg				tBlkDoStL;
reg				tBlkDoStC3;

reg				tBlkDoStB;
reg				tBlkDoStBL;

`ifdef jx2_mem_l2d2way

`reg_ddrtile	tBlkStDataB;
reg[27:0]		tBlkStAddrB;
reg[3:0]		tBlkStFrovB;

`reg_ddrtile	tBlkStDataBL;
reg[27:0]		tBlkStAddrBL;
reg[3:0]		tBlkStFrovBL;
reg				tBlkStDirtyBL;

reg				tBlkStDirtyB;
// reg			tBlkDoStB;
// reg			tBlkDoStBL;
reg				tBlkDoStBC3;

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

`reg_ddrtile	tAccBlkData;
`reg_ddrtile	tNxtAccBlkData;
reg[27:0]		tAccBlkAddr;
reg[27:0]		tNxtAccBlkAddr;

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
`ifdef jx2_mem_ddr32B
	nxtReqAddr		= { memAddrIn[31:5], 1'b0 };
	nxtReqAddrLo	= memAddrIn[ 4:0];
`else
	nxtReqAddr		= memAddrIn[31:4];
	nxtReqAddrLo	= { 1'b0, memAddrIn[ 3:0] };
`endif


`ifdef jx2_mem_l2sz_1024
	nxtReqIx	= nxtReqAddr [9:0] ^ nxtReqAddr [19:10];
`endif

`ifdef jx2_mem_l2sz_4096
	nxtReqIx	= nxtReqAddr [11:0] ^ nxtReqAddr [23:12];
//	nxtReqIx	= nxtReqAddr [11:0] ^ nxtReqAddr [23:12] ^ nxtReqAddr [17:6];
`endif

`ifdef jx2_mem_l2sz_8192
//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [25:13];
//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [25:13] ^ nxtReqAddr [19:7];

//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [25:13] ^
//		{ nxtReqAddr [13:7], nxtReqAddr [27:22] };
//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [24:12] ^ nxtReqAddr [18:6];
	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [24:12];

//	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [24:12] ^
//		{ nxtReqAddr [27:18], 3'b0 };

//	nxtReqIx	= nxtReqAddr [12:0] ^
//		{	nxtReqAddr [   24], nxtReqAddr [15:12],
//			nxtReqAddr [19:16], nxtReqAddr [23:20]};
//	nxtReqIx	= nxtReqAddr [12:0];

//	nxtReqIx	= { nxtReqAddr[12:4] ^ nxtReqAddr[20:12], nxtReqAddr[3:0] } ;
//	nxtReqIx	= { nxtReqAddr[12:4] ^ nxtReqAddr[20:12],
//		nxtReqAddr[3:0] ^ nxtReqAddr[23:20] } ;

//	nxtReqIx	= {
//		nxtReqAddr[12] ^ nxtReqAddr[20], 
//		nxtReqAddr[11:4] ^ nxtReqAddr[19:12] ^ nxtReqAddr[27:20], 
//		nxtReqAddr[3:0] } ;

`endif

//	if(memRingAddrIsRamReq)
//	begin
//		$display("L2DC: nxtReqIx=%X A=%X", nxtReqIx, nxtReqAddr);
//	end

	tNxtAccOpSq	= tAccOpSq;

	if(memOpmIn[7:0] == 8'h00)
	begin
		nxtReqAddr	= tBlkAddr[27:0];
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
//		if(	(tReqAddr  [27:24]==4'hF) && 
//			(tReqAddrLo[ 3: 0]==4'hE)	)
//		if(memAddrIn[3:0]==4'hE)
		if(	(memAddrIn[31:28]==4'hF) &&
			(memAddrIn[ 3: 0]==4'hE))
		begin
			$display("L2 Flush");
			tNxtDoFlushL2 = 1;
		end
	end

//	if((tCurFrov == 4'h0) && !tDoFlushL2 && !tDoFlushL2L)
	if((tCurFrov == 4'h0) && !tDoFlushL2 && !tDoFlushL2L && !reset)
		tNxtDoFlushL2 = 1;

	tNxtFrov	= tCurFrov;
//	if((tDoFlushL2 || (tCurFrov == 4'h0)) && !tDoFlushL2L)
	if(tDoFlushL2 && !tDoFlushL2L)
	begin
		$display("L2 Flush rov=%d", tCurFrov);
		tNxtFrov	= tCurFrov + 1;
	end

	if(reset)
	begin
		tNxtFrov	= 0;
	end

`ifndef def_true
	if(memRingAddrIsRam)
		$display("L2: Mem1 Is Ram");
	if(mem2RingAddrIsRam)
		$display("L2: Mem2 Is Ram");
	if(mem3RingAddrIsRam)
		$display("L2: Mem3 Is Ram");
	if(mem4RingAddrIsRam)
		$display("L2: Mem4 Is Ram");
`endif

	/* Cycle 2 */
	
	tNxtAccIx		= tReqIx;
	tNxtAccAddr		= tReqAddr;
	tNxtAccBlkData	= tBlkData;
	tNxtAccBlkAddr	= tBlkAddr;
	
	tNxtDoAcc		= 0;
	tNxtAccBlkDirty	= 0;
	tDoSwAcc		= 0;
//	tNxtStLatch		= tStLatch;

	tNxtAccDoLdB	= 0;
	tNxtAccDoLdAzB	= 0;
	tNxtAccDoLdAtB	= 0;

	tSkipC2			= 0;
	tAccess			= 0;

	tNxtReqMissSeq	= tReqMissSeq;
	tNxtReqMissIx	= tReqMissIx;

	tBlkFlush	= 0;
	tBlkDirty	= 0;
	
	tAccReady	= 1;

	tOpmIsLoad	=
		(tReqOpm[7:0]==JX2_RBI_OPM_LDX) || 
		(tReqOpm[7:0]==JX2_RBI_OPM_PFX);
	tOpmIsStore	=
		(tReqOpm[7:0]==JX2_RBI_OPM_STX) ||
		(tReqOpm[7:0]==JX2_RBI_OPM_SPX);
	tOpmIsNz	= tOpmIsLoad || tOpmIsStore ;

//	tAccStoreOnly	= tOpmIsStore;
	tNxtAccStoreOnly	= tOpmIsStore;

`ifdef jx2_mem_line32to16
	tNxtAccStoreOnly	= 0;
`endif

	if(tOpmIsNz)
	begin
//		$display("L2DC: tReqIx=%X A=%X", tReqIx, tReqAddr);
	end

//	tOpmIsNz	=
//		(tReqOpm[7:0]==JX2_RBI_OPM_LDX) ||
//		(tReqOpm[7:0]==JX2_RBI_OPM_STX) ;

	tAddrIsRam	=
		(tReqAddr[25:20] != 6'h00) &&
		(tReqAddr[27:26] == 2'b00) ;
	tBlkAddrIsRam	=
		(tBlkAddr[25:20] != 6'h00) &&
		(tBlkAddr[27:26] == 2'b00) ;

`ifdef jx2_mem_l2d2way
	/* Select older cache line, or hash if same epoch. */
	tBlkEpochRel	= tCurEpoch - tBlkEpoch;
	tBlkEpochRelB	= tCurEpoch - tBlkEpochB;
	tBlkEpochCmp	= tBlkEpochRelB > tBlkEpochRel;
	if(tBlkEpochRelB == tBlkEpochRel)
		tBlkEpochCmp	= tBlkAddr[0] ^ tBlkAddr[7];
`endif

	tBlkFlush		= (tCurFrov != tBlkFrov) || (tCurFrov == 0);
`ifdef jx2_mem_l2d2way
	tBlkFlushB		= (tCurFrov != tBlkFrovB) || (tCurFrov == 0);
`endif

	tMissAddr	=
		(tReqAddr [27:16] != tBlkAddr [27:16]) ||
		(tReqAddr [15: 8] != tBlkAddr [15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddr [ 7: 0]);

`ifdef jx2_mem_l2d2way
	tMissAddrB	=
		(tReqAddr [27:16] != tBlkAddrB[27:16]) ||
		(tReqAddr [15: 8] != tBlkAddrB[15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddrB[ 7: 0]);
	
//	if(!tMissAddr && !tBlkFlush && (tOpmIsNz && tAddrIsRam))
//		$display("L2DC: Hit A");
//	if(!tMissAddrB && !tBlkFlushB && (tOpmIsNz && tAddrIsRam))
//		$display("L2DC: Hit B");
`endif

// `ifdef jx2_mem_l2d2way
`ifndef def_true
	if(tBlkAddr == tBlkAddrB)
	begin
		if(tBlkAddr!=0)
		begin
			$display("L2DC: (Sanity) A/B Address Clash Ix=%X A=%X",
				tBlkIx, tBlkAddr);
		end
		tBlkFlush	= 1;
		tBlkFlushB	= 1;
	end
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

	tBlkDirty			= tBlkFlag[0];
	tNxtAccBlkAddrIsRam	= tBlkAddrIsRam;

	if(!tBlkDirty)
		tNxtAccStoreOnly = 0;

//	tMiss		= (tMissAddr || tBlkFlush) &&
	tMiss		=
		 (tMissAddr || tBlkFlush) &&
//		(	(tMissAddr && tOpmIsLoad ) ||
//			((tMissAddr && tBlkDirty) && tOpmIsStore) ||
//			tBlkFlush )	&&
		(tOpmIsNz && tAddrIsRam);
//		(tOpmIsNz && (tAddrIsRam || tBlkFlush));

`ifdef jx2_mem_l2d2way
	tBlkDirtyB			= tBlkFlagB[0];

	tMiss		=
		(tMissAddr || tBlkFlush) &&
//		(	(tMissAddr && tOpmIsLoad) ||
//			((tMissAddr && tBlkDirty) && tOpmIsStore) ||
//			tBlkFlush )	&&
		(tMissAddrB || tBlkFlushB || (tOpmIsStore && tBlkDirty) || tBlkFlush) &&
//		(tMissAddrB || tBlkFlushB || (tOpmIsStore && tBlkDirty)) &&
		(tOpmIsNz && tAddrIsRam);
//	if(tBlkFlush && tBlkDirty && (tOpmIsNz && tAddrIsRam))
//		tMiss = 1;
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
	if(tOpmIsLoad && (tBlkDirty || tBlkEpochCmp) && !tBlkFlush)
		tNxtAccDoLdB	= 1;
	if(tBlkEpochRel[3] && tBlkDirty)
		tNxtAccDoLdB	= 0;
	if(tBlkFlush || tBlkFlushB)
		tNxtAccDoLdAzB	= 1;		
//	if(tNxtAccDoLdB)
//		$display("L2DC: Do Load B");

//	if(!tNxtAccDoLdB && tBlkDirtyA && !tBlkFlushA)
	if(!tNxtAccDoLdB && !tBlkFlush)
//		tNxtAccDoLdAtB	= 1;
		tNxtAccDoLdAtB	= tNxtAccDoLdAzB || !tBlkEpochCmp;

`endif

`ifndef jx2_mem_line32to16
//	if(tOpmIsStore && !tBlkDirty && (tReqIx != tReqMissIx))
	if(tOpmIsStore && !tBlkDirty)
//	if(tOpmIsStore && !tBlkDirty &&
//		((tReqIx != tReqMissIx) || (tReqMissSeq==0)) )
	begin
		tMiss = 0;
//		tNxtAccStoreOnly = 1;

		if(tBlkDoStL && (tBlkStIxL == tBlkIx))
		begin
//			$display("L2DC: Store Skip 1");
			tSkipC2		= 1;
		end

	end
`endif

	tNxtAccNoClashA	= 	
		!tBlkFlush && !tBlkFlushB && !tNxtAccStoreOnly;

//	tNxtAccNoClashA	= 	0;

	if((tBlkFlush && !tMissAddr) && tMiss && !tMissL)
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

`ifndef def_true
//	if(tBlkFlush && tBlkDirty)
//	if(tBlkFlush && tBlkDirty && tOpmIsNz)
	if(tBlkFlush && tBlkDirty && (tOpmIsNz && tAddrIsRam))
//	if(tBlkFlush)
	begin
		$display("L2DC: Flush Line");
		tDbgFlushMiss	= 1;
		tMiss			= 1;
	end
`endif

//	tMemDataOut	= tBlkData;

//	tHold		= 0;

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
	
//	tBlkStData	= UVDDRT_XX;
	tBlkStData	= UVDDRT_00;
	tBlkStAddr	= UV28_00;
	tBlkStFrov	= tCurFrov;

`ifdef jx2_mem_l2d2way
//	tBlkStDataB	= UVDDRT_XX;
	tBlkStDataB	= UVDDRT_00;
	tBlkStAddrB	= UV28_00;
	tBlkStFrovB	= tCurFrov;
`endif

`ifdef jx2_mem_l2sz_8192
	tBlkStIx	= UV13_00;
`endif
`ifdef jx2_mem_l2sz_4096
	tBlkStIx	= UV12_00;
`endif
`ifdef jx2_mem_l2sz_2048
	tBlkStIx	= UV11_00;
`endif
`ifdef jx2_mem_l2sz_1024
	tBlkStIx	= UV10_00;
`endif

	tBlkStDirty	= 0;
	tBlkDoSt	= 0;
`ifdef jx2_mem_l2d2way
	tBlkStDirtyB	= 0;
//	tBlkDoStB		= 0;
`endif
	tBlkDoStB		= 0;

`ifdef jx2_mem_l2d2way

// `ifdef def_true
`ifndef def_true
	/* See if we can handle miss by swapping blocks. */
	if(tOpmIsStore && !tBlkDirty &&
		!tAccSticky && tMiss && tMissAddr && !tMissAddrB &&
		!tBlkFlush && !tBlkFlushB && !tAccBusyLatch)
	begin
		tBlkStIx		= tBlkIx;

		tBlkStData		= tBlkDataB;
		tBlkStAddr		= tBlkAddrB;
		tBlkStFrov		= tCurFrov;
		tBlkStDirty		= 0;
		tBlkDoSt		= 1;

		tBlkStDataB		= tBlkData;
		tBlkStAddrB		= tBlkAddr;
		tBlkStFrovB		= tCurFrov;
		tBlkStDirtyB	= 0;
		tBlkDoStB		= 1;

		tSkipC2	= 1;
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
			tBlkStDataB		= tBlkLdData;
			tBlkStAddrB		= tBlkLdAddr;
			tBlkStFrovB		= tCurFrov;
			tBlkStDirtyB	= 0;
			tBlkDoStB		= 1;
		end
		else
		begin
			tBlkStData		= tBlkLdData;
			tBlkStAddr		= tBlkLdAddr;
			tBlkStFrov		= tCurFrov;
			tBlkStDirty		= 0;
			tBlkDoSt		= 1;

`ifdef jx2_mem_l2d_ldatb
			if(tBlkLdAtB)
			begin
				tBlkStDataB		= tBlkLdDataB;
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
//				tBlkStFrovB		= 0;
				tBlkStDirtyB	= 0;
				tBlkDoStB		= 1;
			end
		end
`else
		tBlkStData	= tBlkLdData;
		tBlkStAddr	= tBlkLdAddr;
		tBlkStIx	= tBlkLdIx;
//		tBlkStFrov	= tBlkLdFrov;
		tBlkStFrov	= tCurFrov;
		tBlkStDirty	= 0;
		tBlkDoSt	= 1;
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
//	else
		
	if(tAddrIsRam)
	begin
		if(tReqOpm[7:0]==JX2_RBI_OPM_SPX)
		begin
//			tAccess		= tMissAddr && tBlkDirty;
//			tAccess		= tBlkFlush || tBlkDirty;
			tAccess		= 1;
		end
		else if(tReqOpm[7:0]==JX2_RBI_OPM_PFX)
		begin
			tAccess		= 1;
		end
		else if(tReqOpm[7:0]==JX2_RBI_OPM_LDX)
		begin
//			$display("L2: LDX");
	
//			if(tBlkDoStC3)
//			if(tBlkDoStC3 && (tBlkStIxL == tBlkIx))
//			if(tBlkDoStL && (tBlkStIxL == tBlkIx))
			if((tBlkDoStL || tBlkDoStBL) && (tBlkStIxL == tBlkIx))
			begin
//				$display("L2 Hold: Did St");
//				tHold		= 1;
				if(!tHold)
				begin
//					$display("L2DC: Load Skip 1");
					tSkipC2		= 1;
				end
			end
		
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

`ifndef def_true
			if(tBlkDoStL && (tBlkStIxL == tBlkIx))
			begin
//				$display("L2 Hold: Did St");
//				tHold		= 1;

				if(!tHold)
					tSkipC2		= 1;
			end
`endif

			if(tAccBusyLatch && (tReqIx == tAccIx) && !tMiss)
				tSkipC2		= 1;

			tAccess		= 1;

// `ifdef jx2_mem_l2d2way
`ifndef def_true
//			if((!tMissAddrB || (tMissAddr && tBlkFlushB)) &&
			if(tMissAddr && !tBlkFlush &&
				!tBlkDirty &&
				!tSkipC2 && !tAccSticky && !tBlkDoStL)

//			if(!(tMissAddrB || tBlkFlushB) && !tBlkDirty &&
//				!tSkipC2 && !tAccSticky && !tBlkDoStL)
//			if((tMissAddr && !tBlkFlush) && !tBlkDirty &&
//				!tSkipC2 && !tAccSticky && !tBlkDoStL)
			begin
`ifdef	jx2_mem_line32to16
				tBlkStData		= {
					 tReqAddrLo[4] ? tReqDataIn : tBlkData[255:128],
					!tReqAddrLo[4] ? tReqDataIn : tBlkData[127:  0]
					};
`else
				tBlkStData		= tReqDataIn;
`endif
				tBlkStAddr		= tReqAddr;
				tBlkStIx		= tReqIx;
				tBlkStFrov		= tCurFrov;
				tBlkStDirty		= 1;
				tBlkDoSt		= 1;

				tBlkStDataB		= tBlkData;
				tBlkStAddrB		= tBlkAddr;
//				tBlkStIxB		= tReqIx;
				tBlkStFrovB		= tBlkFrov;
				tBlkStDirtyB	= tBlkDirty;
				tBlkDoStB		= 1;

				if(tBlkFlush || !tMissAddr)
				begin
					tBlkStAddrB		= 0;
					tBlkStFrovB		= 0;
				end
			end
			else
`endif

//			if(!tMiss && tAccReady && !tStLatch && !tSkipC2)
//			if(!tMiss && tAccReady && !tSkipC2)
//			if(!tMiss && tAccReady && !tSkipC2)
			if(!tMiss && tAccReady && !tSkipC2 && !tAccSticky)
			begin

`ifdef	jx2_mem_line32to16
				tBlkStData		= {
					 tReqAddrLo[4] ? tReqDataIn : tBlkData[255:128],
					!tReqAddrLo[4] ? tReqDataIn : tBlkData[127:  0]
					};
`else
				tBlkStData	= tReqDataIn;
`endif

				tBlkStAddr	= tReqAddr;
				tBlkStIx	= tReqIx;
				tBlkStFrov	= tCurFrov;
				tBlkStDirty	= 1;
				tBlkDoSt	= 1;
//				tNxtStLatch	= 1;

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
					$display("L2DC Store Miss: Ix=%X A=%X",
						tReqIx, tReqAddr);
					tSkipC2		= 1;
				end
			end
		end
		else
		begin
//			tNxtStLatch	= 0;
		end

	end
	else
	begin
//		tNxtStLatch	= 0;
	end

	if(tAccess && tMiss && tAccBusyLatch)
		tSkipC2 = 1;

	tNxtDoAcc	= (tAccess && tMiss && !tSkipC2) &&
		tAccReady && !tAccDone && !(tBlkDoStL || tBlkDoStBL) && !reset;
	tNxtAccBlkDirty	= tBlkDirty;
	
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
			tAccReady, tAccDone, tBlkDoStL, tCurEpochCyc);
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

`ifdef jx2_mem_line32to16
	tMemDataReq		= mem3AddrIn[4] ?
		tBlkDataC3[255:128] : tBlkDataC3[127:0];

`ifdef jx2_mem_l2d2way
	if(	 (tMissAddrL || tBlkFlushL) &&
		!(tMissAddrBL || tBlkFlushBL))
		tMemDataReq		= mem3AddrIn[4] ?
		tBlkDataBC3[255:128] : tBlkDataBC3[127:0];
`endif

`else
	tMemDataReq		= tBlkDataC3;		//memory output data

`ifdef jx2_mem_l2d2way
	if(	 (tMissAddrL || tBlkFlushL) &&
		!(tMissAddrBL || tBlkFlushBL))
		tMemDataReq		= tBlkDataBC3;		//memory output data
`endif

`endif


	tMemSkipReq		= tSkipC3 || tHoldL || tHoldL2 || tHoldL3;

	tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
	if(mem3RingIsStx)
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKST;

	if(mem3RingIsPfx || mem3RingIsSpx)
		tMemSkipReq = 0;

	tMemOpmReq[3:0] = mem3OpmIn[11:8];

	if((tReqSeqC3!=mem3SeqIn) && !tMemSkipReq && !tHold)
	begin
		$display("L2DC: Request Misalignment");
		tMemSkipReq		= 1;
	end
`endif

	/* Cycle 4 */

// `ifdef def_true
`ifndef def_true
	tMemSeqReq		= mem4SeqIn;
	tMemOpmReq		= mem4OpmIn;
	tMemAddrReq		= mem4AddrIn;

//	tMemSeqReq		= mem3SeqIn;
//	tMemOpmReq		= mem3OpmIn;
//	tMemAddrReq		= mem3AddrIn;

//	tMemDataReq		= tBlkDataC3;		//memory output data
	tMemDataReq		= tBlkDataC4;		//memory output data

//	tMemSkipReq		= tSkipC4 || tHoldL || tHoldC4;
	tMemSkipReq		= tSkipC4 || tHoldL || tHoldL2 || tHoldL3 || tHoldC4;

	tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
	if(mem4RingIsStx)
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKST;

	if(mem4RingIsPfx || mem4RingIsSpx)
		tMemSkipReq = 0;

	if((tReqSeqC4!=mem4SeqIn) && !tMemSkipReq && !tHold)
	begin
		$display("L2DC: Request Misalignment");
		tMemSkipReq		= 1;
	end
`endif

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
		tBlkDataC3		<= tBlkData;
		tBlkDoStC3		<= tBlkDoSt;
//		tHoldC3			<= tHold;
		tReqSeqC3		<= tReqSeq;

`ifdef jx2_mem_l2d2way
		tBlkDataBC3		<= tBlkDataB;
		tBlkDoStBC3		<= tBlkDoStB;
`endif

		/* Cycle 3->4 */
		tSkipC4			<= tSkipC3;
		tBlkDataC4		<= tBlkDataC3;
		tHoldC4			<= tHoldL;
		tReqSeqC4		<= tReqSeqC3;
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

	tBlkDirtyL		<= tBlkDirty;
	tBlkFlushL		<= tBlkFlush;
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

	{ tBlkPad, tBlkEpoch, tBlkData }	<= memTileData[nxtReqIx2];
	{ tBlkFrov, tBlkFlag, tBlkAddr }	<= memTileAddr[nxtReqIx2];
	tBlkIx								<= nxtReqIx2;

`ifdef jx2_mem_l2d2way
	{ tBlkPadB, tBlkEpochB, tBlkDataB }	<= memTileDataB[nxtReqIx2];
	{ tBlkFrovB, tBlkFlagB, tBlkAddrB }	<= memTileAddrB[nxtReqIx2];
`endif

	tBlkStDataL		<= tBlkStData;
	tBlkStAddrL		<= tBlkStAddr;
	tBlkStFrovL		<= tBlkStFrov;
	tBlkStDirtyL	<= tBlkStDirty;
	tBlkStIxL		<= tBlkStIx;

	tBlkDataL		<= tBlkData;
	tBlkDoStL		<= tBlkDoSt;

`ifdef jx2_mem_l2d2way
	tBlkStDataBL		<= tBlkStDataB;
	tBlkStAddrBL		<= tBlkStAddrB;
	tBlkStFrovBL		<= tBlkStFrovB;
	tBlkStDirtyBL		<= tBlkStDirtyB;
	tBlkDataBL			<= tBlkDataB;
//	tBlkDoStBL			<= tBlkDoStB;
`endif

	tBlkDoStBL			<= tBlkDoStB;
//	tAccOpSq			<= tNxtAccOpSq;

//	tBlkDataC4		<= tBlkDataC3;

	if(tBlkDoSt)
	begin
//		$display("L2: Store Line A Ix=%X A=%X D=%X, Cyc=%d",
//			tBlkStIx, tBlkStAddr, tBlkStData, tCurEpochCyc);
		memTileData[tBlkStIx]	<=
			{ tBlkPad, tCurEpoch, tBlkStData };
		memTileAddr[tBlkStIx]	<=
			{ tBlkStFrov, 3'b100, tBlkStDirty, tBlkStAddr};
		tAccSticky	<= 0;
	end

`ifdef jx2_mem_l2d2way
	if(tBlkDoStB)
	begin
//		$display("L2: Store Line B Ix=%X A=%X D=%X, Cyc=%d",
//			tBlkStIx, tBlkStAddrB, tBlkStDataB, tCurEpochCyc);
		memTileDataB[tBlkStIx]	<=
			{ tBlkPadB, tCurEpoch, tBlkStDataB };
		memTileAddrB[tBlkStIx]	<=
			{ tBlkStFrovB, 3'b100, tBlkStDirtyB, tBlkStAddrB};
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
				tBlkLdAddr		<= tAccStoreOnly ? 0 : tAccAddr;
				tBlkLdIx		<= tAccIx;
				tBlkLdFrov		<= tCurFrov;
				tBlkLdB			<= tAccStoreOnly ? 0 : tAccDoLdB;
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
				$display("L2: Store Non-RAM Block Addr=%X", tBlkAddr);
			end

			tDdrMemDataOut		<= tAccBlkData;
			tDdrMemAddrSw		<= {tAccBlkAddr, 4'b0000};

`ifdef jx2_mem_ddrswap 
// `ifndef def_true
//			if(tAccBlkDirty && !tAccDone && tAccNoClashA)
			if(tAccBlkDirty && !tAccDone && tAccNoClashA &&
				!tAccDoLdB && !tAccStoreOnly)
			begin
				tDdrMemAddr		<= {tAccAddr, 4'h00};
				tDdrMemOpm		<= UMEM_OPM_SW_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
				tAccStDone		<= 1;
			end
			else if(tAccBlkDirty && !tNxtStDone)
			begin
				tDdrMemAddr		<= {tAccBlkAddr, 4'h00};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemAddr		<= {tAccAddr, 4'h00};
				tDdrMemOpm		<= UMEM_OPM_RD_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
			end
`else
			if(tAccBlkDirty && !tNxtStDone)
			begin
				tDdrMemDataOut	<= tAccBlkData;
				tDdrMemAddr		<= {tAccBlkAddr, 4'b0000};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tDdrMemOpSq		<= tAccOpSq;
				tAccLatch		<= 1;
//				tAccStLatch		<= 1;
//				if(!tAccStLatch)
//					tAccOpSq	<= tAccOpSq + 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemDataOut	<= UVDDRT_00;
				tDdrMemAddr		<= {tAccAddr, 4'b0000};
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


`ifndef def_true
	/* Cycle 4->Out */

	if(mem4RingAddrIsRam && (mem4RingIsLdx || mem4RingIsStx) &&
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
		if((mem4RingIsLdx || mem4RingIsStx) && mem4RingAddrIsRam)
		begin
			$display("L2DC: Skip Req: IsRam=%X Hold=%X SkipReq=%X", 
				mem4RingAddrIsRam,
				tHold, tMemSkipReq);
		end
`endif

		tMemSeqOut		<= mem4SeqIn;
		tMemOpmOut		<= mem4OpmIn;
		tMemAddrOut		<= mem4AddrIn;
		tMemDataOut		<= mem4DataIn;

//		tMemSeqOut		<= mem3SeqIn;
//		tMemOpmOut		<= mem3OpmIn;
//		tMemAddrOut		<= mem3AddrIn;
//		tMemDataOut		<= mem3DataIn;
	end
`endif

end

endmodule

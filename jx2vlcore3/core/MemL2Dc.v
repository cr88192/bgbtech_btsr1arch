/*
L2 Tile Cache

Caches 128-bit tiles, 
*/

`include "CoreDefs.v"

module MemL2Dc(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	
	memAddr,	memAddrB,
	memDataIn,	memDataOut,
	memOpm,		memOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,
	
	regDeadlockStrobe
	);

input			clock;
input			reset;
	
input[31:0]		memAddr;
input[31:0]		memAddrB;
input[4:0]		memOpm;
output[1:0]		memOK;

`input_tile		memDataIn;
`output_tile	memDataOut;

`input_ddrtile	ddrMemDataIn;
`output_ddrtile	ddrMemDataOut;

output[31:0]	ddrMemAddr;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;

input			regDeadlockStrobe;

`reg_tile		tMemDataOut;
`reg_tile		tMemDataOut2;

reg[  1:0]		tMemOK;
reg[  1:0]		tMemOK2;
reg[  1:0]		tMemOK3;
reg[  1:0]		tMemOK4;

`ifndef jx2_l2d_nofw_memout
assign	memDataOut		= tMemDataOut2;
assign	memOK			= tMemOK2;
`endif

`ifdef jx2_l2d_nofw_memout
assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;
`endif

`reg_ddrtile	tDdrMemDataIn;
`reg_ddrtile	tDdrMemDataOut;
`reg_ddrtile	tDdrMemDataOutB;

reg[1:0]		tDdrMemOK;
reg[ 31:0]		tDdrMemAddr;
reg[ 31:0]		tDdrMemAddrB;
reg[  4:0]		tDdrMemOpm;
reg[  4:0]		tDdrMemOpmB;

`ifndef jx2_l2d_nofw_ddrout
assign	ddrMemDataOut	= tDdrMemDataOutB;
assign	ddrMemAddr		= tDdrMemAddrB;
assign	ddrMemOpm		= tDdrMemOpmB;
`endif

`ifdef jx2_l2d_nofw_ddrout
assign	ddrMemDataOut	= tDdrMemDataOut;
assign	ddrMemAddr		= tDdrMemAddr;
assign	ddrMemOpm		= tDdrMemOpm;
`endif

`ifdef jx2_mem_l2sz_8192
`reg_tile	memTileData[8191:0];
reg[ 35:0]	memTileAddr[8191:0];
`ifdef jx2_mem_fulldpx
reg[ 35:0]	memTileAddrB[8191:0];
`endif
`endif

`ifdef jx2_mem_l2sz_4096
`reg_tile	memTileData[4095:0];
reg[ 35:0]	memTileAddr[4095:0];
`ifdef jx2_mem_fulldpx
reg[ 35:0]	memTileAddrB[4095:0];
`endif
`endif

`ifdef jx2_mem_l2sz_2048
`reg_tile	memTileData[2047:0];
reg[ 35:0]	memTileAddr[2047:0];
`ifdef jx2_mem_fulldpx
reg[ 35:0]	memTileAddrB[2047:0];
`endif
`endif

`ifdef jx2_mem_l2sz_1024
`reg_tile	memTileData[1023:0];
reg[ 35:0]	memTileAddr[1023:0];
`ifdef jx2_mem_fulldpx
reg[ 35:0]	memTileAddrB[1023:0];
`endif
`endif


`ifdef jx2_mem_l2sz_8192
reg[12:0]	nxtReqIx;
reg[12:0]	nxtReqIxB;
reg[12:0]	tReqIx0;
reg[12:0]	tReqIxB0;
reg[12:0]	tReqIx;
reg[12:0]	tReqIxL;
reg[12:0]	tReqIxL2;
reg[12:0]	tReqIxB;
reg[12:0]	tReqIxBL;
reg[12:0]	tReqIxBL2;
reg[12:0]	tBlkLdIx;
reg[12:0]	tBlkStIx;
reg[12:0]	tBlkStIxL;
reg[12:0]	tBlkSwIx;
reg[12:0]	tBlkSwIxL;

reg[12:0]	tAccIx;
reg[12:0]	tNxtAccIx;
reg[12:0]	nxtRovIx;
reg[12:0]	tRovIx;
`endif

`ifdef jx2_mem_l2sz_4096
reg[11:0]	nxtReqIx;
reg[11:0]	nxtReqIxB;
reg[11:0]	tReqIx0;
reg[11:0]	tReqIxB0;
reg[11:0]	tReqIx;
reg[11:0]	tReqIxL;
reg[11:0]	tReqIxL2;
reg[11:0]	tReqIxB;
reg[11:0]	tReqIxBL;
reg[11:0]	tReqIxBL2;
reg[11:0]	tBlkLdIx;
reg[11:0]	tBlkStIx;
reg[11:0]	tBlkStIxL;
reg[11:0]	tBlkSwIx;
reg[11:0]	tBlkSwIxL;

reg[11:0]	tAccIx;
reg[11:0]	tNxtAccIx;
reg[11:0]	nxtRovIx;
reg[11:0]	tRovIx;
`endif

`ifdef jx2_mem_l2sz_2048
reg[10:0]	nxtReqIx;
reg[10:0]	nxtReqIxB;
reg[10:0]	tReqIx0;
reg[10:0]	tReqIxB0;
reg[10:0]	tReqIx;
reg[10:0]	tReqIxL;
reg[10:0]	tReqIxL2;
reg[10:0]	tReqIxB;
reg[10:0]	tReqIxBL;
reg[10:0]	tReqIxBL2;
reg[10:0]	tBlkLdIx;
reg[10:0]	tBlkStIx;
reg[10:0]	tBlkStIxL;
reg[10:0]	tBlkSwIx;
reg[10:0]	tBlkSwIxL;

reg[10:0]	tAccIx;
reg[10:0]	tNxtAccIx;
reg[10:0]	nxtRovIx;
reg[10:0]	tRovIx;
`endif

`ifdef jx2_mem_l2sz_1024
reg[9:0]	nxtReqIx;
reg[9:0]	nxtReqIxB;
reg[9:0]	tReqIx0;
reg[9:0]	tReqIxB0;
reg[9:0]	tReqIx;
reg[9:0]	tReqIxL;
reg[9:0]	tReqIxL2;
reg[9:0]	tReqIxB;
reg[9:0]	tReqIxBL;
reg[9:0]	tReqIxBL2;
reg[9:0]	tBlkLdIx;
reg[9:0]	tBlkStIx;
reg[9:0]	tBlkStIxL;
reg[9:0]	tBlkSwIx;
reg[9:0]	tBlkSwIxL;

reg[9:0]	tAccIx;
reg[9:0]	tNxtAccIx;
reg[9:0]	nxtRovIx;
reg[9:0]	tRovIx;
`endif


reg[27:0]	nxtReqAddr;
reg[27:0]	nxtReqAddrB;
reg[4:0]	nxtReqAddrLo;

reg[27:0]	tReqAddr;
reg[27:0]	tReqAddrB;

reg[27:0]	tReqAddrL;
reg[27:0]	tReqAddrBL;
reg[27:0]	tAccAddr;
reg[27:0]	tNxtAccAddr;

reg[4:0]	tReqAddrLo;
// reg[4:0]	tReqAddrLoB;

reg[4:0]	tReqOpm;
reg[4:0]	tReqOpmL;
reg[4:0]	tReqOpmL2;
reg[4:0]	tReqOpmL3;
`reg_tile	tReqDataIn;

reg[27:0]	tReqAddr0;
reg[27:0]	tReqAddrB0;
reg[4:0]	tReqAddrLo0;
reg[4:0]	tReqOpm0;
`reg_tile	tReqDataIn0;

`reg_tile	tBlkData;
reg[27:0]	tBlkAddr;
reg[27:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlag;
reg[ 3:0]	tBlkFlagB;
reg[ 3:0]	tBlkFrov;
reg[ 3:0]	tBlkFrovB;

reg			tBlkDirty;
reg			tBlkDirtyB;
reg			tBlkFlush;
reg			tBlkFlushB;
reg			tBlkDirtyL;
reg			tBlkDirtyBL;
reg			tBlkFlushL;
reg			tBlkFlushBL;

`reg_tile	tBlkData_A0;
reg[27:0]	tBlkAddr_A0;
reg[27:0]	tBlkAddrB_A0;
reg[ 3:0]	tBlkFlag_A0;
reg[ 3:0]	tBlkFlagB_A0;
reg[ 3:0]	tBlkFrov_A0;
reg[ 3:0]	tBlkFrovB_A0;


reg[ 3:0]	tCurFrov;
reg[ 3:0]	tNxtFrov;

reg[3:0]	nxtRovIxCnt;
reg[3:0]	tRovIxCnt;


`reg_tile	tBlkLdData;
reg[27:0]	tBlkLdAddr;
reg[3:0]	tBlkLdFrov;

`reg_tile	tBlkStData;
reg[27:0]	tBlkStAddr;
reg[3:0]	tBlkStFrov;

`reg_tile	tBlkStDataL;
reg[27:0]	tBlkStAddrL;
reg[3:0]	tBlkStFrovL;
reg			tBlkStDirtyL;

reg			tBlkStDirty;
reg			tBlkDoSt;
reg			tBlkDoStL;

`reg_tile	tBlkSwData;
reg[27:0]	tBlkSwAddr;
reg			tBlkSwDirty;
reg			tBlkDoSw;

`reg_tile	tBlkSwDataL;
reg[27:0]	tBlkSwAddrL;
reg			tBlkSwDirtyL;
reg			tBlkDoSwL;

reg		tMissAddr;
reg		tMissAddrB;
reg		tMiss;
reg		tMissL;
reg		tMissL2;
reg		tMissL3;
reg		tMissB;
reg		tMissBL;
reg		tHold;
reg		tHoldL;
reg		tAccess;
reg		tAccessB;

reg		tAddrIsRam;
reg		tAddrIsRamB;
reg		tBlkAddrIsRam;
reg 	tOpmIsNz;
reg		tDoAcc;
reg		tDoSwAcc;
reg		tNxtDoAcc;
reg		tAccDirty;
reg		tNxtAccDirty;

reg		tStLatch;
reg		tNxtStLatch;

reg		tSwLatch;
reg		tNxtSwLatch;
reg		tAccReady;
reg		tAccReadyL;

reg		tSwFault;
reg		tSwFaultL;
reg		tDbgFlushMiss;

reg tAccLatch;
// reg tAccLatchB;
reg tAccDone;
reg tAccStDone;
reg tAccSwDone;
reg tNxtStDone;
reg tAccSticky;
reg	tAccBlkHalf;
reg	tNxtBlkHalf;

reg	tNxtDoFlushL2;
reg	tDoFlushL2;
reg	tDoFlushL2L;

always @*
begin
`ifdef jx2_l2d_nofw_ddrin
	tDdrMemDataIn	= ddrMemDataIn;
	tDdrMemOK		= ddrMemOK;
`endif

	/* Input End */
`ifdef jx2_mem_line32B
	nxtReqAddr		= { memAddr [31:5], 1'b0 };
	nxtReqAddrB		= { memAddrB[31:5], 1'b0 };
	nxtReqAddrLo	= memAddr [ 4:0];
`else
	nxtReqAddr		= memAddr [31:4];
	nxtReqAddrB		= memAddrB[31:4];
	nxtReqAddrLo	= { 1'b0, memAddr [ 3:0] };
`endif

	if((nxtReqAddrLo != 0) && (memOpm[4:3]!=0))
	begin
		$display("L2: Misaligned Cache Line");
	end

`ifdef jx2_mem_l2sz_8192
	nxtReqIx	= nxtReqAddr [12:0] ^ nxtReqAddr [25:13];
	nxtReqIxB	= nxtReqAddrB[12:0] ^ nxtReqAddrB[25:13];
`endif

`ifdef jx2_mem_l2sz_1024
	nxtReqIx	= nxtReqAddr [9:0] ^ nxtReqAddr [19:10];
	nxtReqIxB	= nxtReqAddrB[9:0] ^ nxtReqAddrB[19:10];
`endif

`ifdef jx2_mem_l2sz_4096
	nxtReqIx	= nxtReqAddr [11:0] ^ nxtReqAddr [23:12];
	nxtReqIxB	= nxtReqAddrB[11:0] ^ nxtReqAddrB[23:12];
`endif

// `ifdef def_true
`ifndef def_true
	if((memOpm==UMEM_OPM_READY) && (tReqOpmL==UMEM_OPM_READY))
		nxtReqIx = tRovIx;
`endif

`ifdef def_true
// `ifndef def_true
	nxtRovIx = tRovIx;
	nxtRovIxCnt	= tRovIxCnt + 1;
	if(tRovIxCnt == 0)
		nxtRovIx = tRovIx + 1;
`endif



	tNxtDoFlushL2	= 0;
	if(tReqOpm==UMEM_OPM_FLUSHDS)
	begin
		if(	(tReqAddr  [27:24]==4'hF) && 
			(tReqAddrLo[ 3: 0]==4'hE)	)
		begin
			$display("L2 Flush");
			tNxtDoFlushL2 = 1;
		end
	end

	tNxtFrov	= tCurFrov;
	if((tDoFlushL2 || (tCurFrov == 4'h0)) && !tDoFlushL2L)
	begin
		$display("L2 Flush rov=%d", tCurFrov);
		tNxtFrov	= tCurFrov + 1;
	end


	/* Swap State */
	
//	tAccIx			= tReqIx;
//	tAccAddr		= tReqAddr;
	tNxtAccIx		= tReqIx;
	tNxtAccAddr		= tReqAddr;

//	tDoAcc			= 0;
	tNxtDoAcc		= 0;
	tNxtAccDirty	= 0;
	tDoSwAcc		= 0;
	tNxtSwLatch		= tSwLatch;
	tNxtStLatch		= tStLatch;
//	tNxtStLatch		= tStLatch;

	tBlkSwData		= tBlkSwDataL;
	tBlkSwAddr		= tBlkSwAddrL;
	tBlkSwDirty		= tBlkSwDirtyL;
	tBlkSwIx		= tBlkSwIxL;
//	tBlkDoSw		= tBlkDoSwL;
	tBlkDoSw		= 0;
	tSwFault		= 0;
	tMissB			= 0;

	tAddrIsRam	= (tReqAddr[25:20]!=6'h00) &&
		(tReqAddr[27:26]==2'b00);

	tAddrIsRamB	= (tReqAddrB[25:20]!=6'h00) &&
		(tReqAddrB[27:26]==2'b00);

	tBlkAddrIsRam	= (tBlkAddr[25:20]!=6'h00) &&
		(tBlkAddr[27:26]==2'b00);

//	tAddrIsRam	= (tReqAddr[26:20]!=7'h00) &&
//		(!tReqAddr[27]);

//	tAddrIsRam	= (tReqAddr[25:24]!=2'b00) &&
//	tAddrIsRam	= (tReqAddr[27:24]!=4'h0);
	
//	$display("tAddrIsRam = %X, A=%X", tAddrIsRam, tReqAddr);
	
//	tOpmIsNz	= (tReqOpm[4:3]!=0);
	tOpmIsNz	= (tReqOpm[4:3]!=0) && (tReqOpm[2:0]==3'b111);
	tAccessB	= (tReqOpm[4:3]==3);
`ifdef jx2_mem_l2exldcyc
	tAccReady	= (tReqIxL == tReqIx) && (tReqIxL2 == tReqIxL);
`else
`ifdef jx2_mem_fulldpx
	tAccReady	= (tReqIxL == tReqIx) && (tReqIxBL == tReqIxB);
`else
	tAccReady	= (tReqIxL == tReqIx);
`endif
`endif
//	tAccReady	= (tReqIxL == tReqIx) && !tBlkDoStL;
//	tAccReady	= (tReqIxL == tReqIx) && (tReqOpmL == tReqOpm);

	tBlkFlush	= (tCurFrov != tBlkFrov);
	tBlkFlushB	= (tCurFrov != tBlkFrovB);

//	tMissAddr	= (tReqAddr != tBlkAddr);

	tMissAddr	=
		(tReqAddr [27:16] != tBlkAddr [27:16]) ||
		(tReqAddr [15: 8] != tBlkAddr [15: 8]) ||
		(tReqAddr [ 7: 0] != tBlkAddr [ 7: 0]);
	tMissAddrB	=
		(tReqAddrB[27:16] != tBlkAddrB[27:16]) ||
		(tReqAddrB[15: 8] != tBlkAddrB[15: 8]) ||
		(tReqAddrB[ 7: 0] != tBlkAddrB[ 7: 0]);

	tBlkDirty	= tBlkFlag[0];

//	tMiss		= (tReqAddr != tBlkAddr) && tOpmIsNz;
//	tMiss		= ((tReqAddr != tBlkAddr) || tBlkFlush) &&
//		((tOpmIsNz && tAddrIsRam) || tBlkDoSwL);
//	tMiss		= ((tReqAddr != tBlkAddr) || tBlkFlush) &&
	tMiss		= (tMissAddr || tBlkFlush) &&
		(tOpmIsNz && tAddrIsRam);
//		tOpmIsNz;
//		(tOpmIsNz && (tReqOpm[3] || tBlkDirty) && tAddrIsRam);
//		(tOpmIsNz && (tReqOpm[3] || tBlkDirty || tBlkFlush) && tAddrIsRam);

//	tMiss		= (tReqAddr != tBlkAddr) && (tOpmIsNz && tAddrIsRam);

	tDbgFlushMiss = 0;

	if(tBlkFlush && tBlkDirty)
	begin
		tDbgFlushMiss	= 1;
		tDoSwAcc		= 1;
		tMiss			= 1;
	end

`ifdef jx2_mem_fulldpx
//	tMissB		= (tReqAddrB != tBlkAddrB) || tBlkFlushB;
	tMissB		= tMissAddrB || tBlkFlushB;
`endif
	tMemDataOut	= tBlkData;
//	tBlkDirty	= tBlkFlag[0];

`ifdef jx2_mem_fulldpx
	tBlkDirtyB	= tBlkFlagB[0];
`endif
	
//	if(tBlkFlush && (tBlkAddr != 28'h0))
//	begin
//		$display("L2D Flush A=%X miss=%d dirty=%d",
//			tBlkAddr, tMiss, tBlkDirty);
//	end

//	tHold		= tMiss || tAccLatch || !tAccReady;
	tHold		= tMiss || tAccLatch || !tAccReady || tAccSticky;
//	tHold		= tMiss || tAccLatch || !tAccReady || tAccSticky || tBlkDoStL;
	tAccess		= 0;

`ifdef jx2_mem_fulldpx
`ifdef jx2_mem_fulldpx_syncb
// `ifndef def_true
//	if((tMissB && !tSwLatch) && (tReqOpm[4:3]==2'b11))
//	if(((tMissB && tBlkDirtyB) && !tSwLatch) && (tReqOpm[4:3]==2'b11))
	if(((tMissB && tBlkDirtyB) && !tSwLatch) &&
		(tReqOpm[4:3]==2'b11) && !tMiss)
//	if(((tMissB && tBlkDirtyB) && !tAccSwDone) && (tReqOpm[4:3]==2'b11))
	begin
//		$display("L2-A: SW %X %X", tReqAddr, tReqAddrB);
		tNxtAccAddr	= tReqAddrB;
		tNxtAccIx	= tReqIxB;
		tDoSwAcc	= 1;
		tHold		= 1;
	end
`endif
`endif
	
	if(tHold)
	begin
//		$display("L2 Hold tMiss=%d tAccLatch=%d", tMiss, tAccLatch);
	end

	tBlkStData	= UVTILE_XX;
//	tBlkStAddr	= UV28_XX;
	tBlkStAddr	= UV28_00;
	tBlkStFrov	= tCurFrov;

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

	if(tAccSticky)
	begin
//		if(tDbgFlushMiss)
//		begin
//			$display("L2 Sticky");
//		end

		tBlkStData	= tBlkLdData;
		tBlkStAddr	= tBlkLdAddr;
		tBlkStIx	= tBlkLdIx;
		tBlkStFrov	= tBlkLdFrov;
		tBlkStDirty	= 0;
		tBlkDoSt	= 1;
//		if(tReqOpm[4:3]!=2'b00)
		if(tOpmIsNz)
		begin
			tAccess	= 1;
			tHold	= 1;

			tNxtStLatch	= tStLatch;
			tNxtSwLatch	= tSwLatch;
			tSwFault	= tSwFaultL;
		end
	end
	else
	if(tAddrIsRam)
	begin
`ifdef jx2_mem_fulldpx
		if(tSwLatch && tOpmIsNz && (tReqOpm!=UMEM_OPM_SW_TILE))
		begin
			$display("L2: Swap Latch, Bad OPM %X", tReqOpm);
			tSwFault = 1;
		end
		else
`endif
		if(tReqOpm==UMEM_OPM_RD_TILE)
		begin
			tAccess		= 1;
			tNxtSwLatch	= 0;
			tNxtStLatch	= 0;
		end
		else if(tReqOpm==UMEM_OPM_WR_TILE)
		begin
//			if(!tAccReadyL || !tBlkDoStL)
			if(!tStLatch)
				tHold	= 1;

			tAccess		= 1;
			tNxtSwLatch	= 0;
//			if(!tMiss)
//			if(!tMissL && tAccReady)
//			if(!tMissL && tAccReady && !tBlkDoStL)
//			if(!tMissL && tAccReady && !tStLatch)
			if(!tMiss && tAccReady && !tStLatch)
			begin
				tBlkStData	= tReqDataIn;
				tBlkStAddr	= tReqAddr;
				tBlkStIx	= tReqIx;
				tBlkStFrov	= tCurFrov;
				tBlkStDirty	= 1;
				tBlkDoSt	= 1;
//				tBlkDoSt	= !tBlkDoStL;
				tNxtStLatch	= 1;
			end
		end
`ifdef jx2_mem_fulldpx
		else if(tReqOpm==UMEM_OPM_SW_TILE)
		begin
//			$display("L2-B: SW %X %X", tReqAddr, tReqAddrB);

			if(!tAccReady)
				tHold	= 1;

			tAccess		= 1;

			tNxtSwLatch	= tSwLatch;
			tNxtStLatch	= 0;

//			tSwFault = 1;

`ifndef def_true
			if(tReqIx == tReqIxB)
//			if((tReqIx == tReqIxB) || tBlkFlush)
			begin
//				$display("L2-B: SW, Duplex Collide %X %X",
//					tReqAddr, tReqAddrB);
				tSwFault = 1;
			end

			if(!tAddrIsRamB)
			begin
//				$display("L2-B: SW, Reject AddrB %X", tReqAddrB);
				tSwFault = 1;
			end
			
//			if(tSwFaultL)
//				tSwFault = 1;

//			if(tBlkFlush || !tAddrIsRamB)
//				tSwFault = 1;
`endif

			if(tAccReady)
//			if(tAccReadyL)
			begin
//				if(tBlkFlush)
//					tHold = 1;
			
`ifdef def_true
				if(tReqIx == tReqIxB)
				begin
					tSwFault = 1;
				end

				if(!tAddrIsRamB)
				begin
					tSwFault = 1;
				end
`endif

//				if(tSwFaultL)
//					tSwFault = 1;

//				tSwFault = 1;

`ifndef jx2_mem_fulldpx_syncb
	//			if(tMiss || tMissB)
//				if(tMiss || (tMissB && tBlkDirtyB))
				if(tMissB && tBlkDirtyB)
//				if(tMissB && tBlkDirtyB && !tMiss)
//				if(tMissB && tBlkDirtyB && !tHold)
//				if(tMissBL && tBlkDirtyBL)
					tSwFault = 1;
`endif
				
//				if(tBlkFlush || !tAddrIsRamB)
//					tSwFault = 1;
			end

			if(tSwFaultL && !tSwFault)
//			if(tSwFaultL && !tSwFault && !tMiss)
//			if(tSwFaultL && !tSwFault && !tSwLatch)
//			if(tSwFaultL && !tSwFault && !tMiss && tOpmIsNz)
			begin
				tSwFault = 1;
				$display("L2: Swap Fault, Ambiguous Transition, O=%X O1=%X",
					tReqOpm, tReqOpmL);
				$display("  MissB %X %X, DirtyB %X %X",
					tMissB, tMissBL,
					tBlkDirtyB, tBlkDirtyBL);
				$display("  tReqIx %X %X, tReqIxB %X %X",
					tReqIx, tReqIxL, tReqIxB, tReqIxBL);
			end

			if(!tSwLatch && !tSwFault)
				tHold	= 1;

			if(tAccReady && !tSwFault)
//			if(tAccReadyL && !tSwFault)
			begin
//				tAccess		= 1;

	//			if((tMissB && !tSwLatch) || tMiss)
//				if(((tMissB && tBlkDirtyB) && !tSwLatch) || tMiss)
//					tHold	= 1;
				
//				if((!tMissB || !tBlkDirtyB) &&
				if((!tMissB || !tBlkDirtyB) && !tMiss &&
//				if((!tMissBL || !tBlkDirtyBL) &&
	//				!tSwLatch && (tReqIxB == tReqIxBL))
//					!tSwLatch && (tReqIxB == tReqIxBL) && !tBlkDoStL)
					!tSwLatch && !tBlkDoStL)
//					!tSwLatch && !tBlkDoStL && !tMissL)
				begin
					if(!tAddrIsRamB)
					begin
						$display("L2: Swap Bad, Non-RAM Store, Addr=%X",
							tReqAddrB);
					end
				
	//				$display("L2-B-1: SW-ST %X", tReqAddrB);

					tBlkStData	= tReqDataIn;
					tBlkStAddr	= tReqAddrB;
					tBlkStIx	= tReqIxB;
					tBlkStFrov	= tCurFrov;
					tBlkStDirty	= 1;
					tBlkDoSt	= 1;
	//				tBlkDoSt	= !tBlkDoStL;
					tNxtSwLatch	= 1;
				end
			end
		end
`else
		else if(tReqOpm==UMEM_OPM_SW_TILE)
		begin
			$display("L2: Bad Swap A=%X", tReqAddr);
			tSwFault = 1;
		end
`endif
		else
		begin
			tNxtSwLatch	= 0;
			tNxtStLatch	= 0;
		end

	end
	else if(tReqOpm==UMEM_OPM_SW_TILE)
	begin
		tSwFault	= 1;
	end
// `ifdef def_true
`ifndef def_true
	else if(tBlkFlush && tBlkDirty && tAccReady &&
			(tReqOpmL==UMEM_OPM_READY))
	begin
//		tAccAddr	= tBlkAddr;
//		tAccIx		= tReqIxL;
		tNxtAccAddr	= tBlkAddr;
		tNxtAccIx	= tReqIxL;

		$display("L2D Flush Evict A=%X Ix=%X", tBlkAddr, tReqIxL);
		tAccess	= 1;
		tHold	= 1;
		tMiss	= 1;
	end
`endif
	else
	begin
		tNxtSwLatch	= 0;
		tNxtStLatch	= 0;
	end

	if(tBlkDoStL)
		tHold = 1;

//	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady;
//	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady && !tAccDone;
//	tNxtDoAcc	= ((tAccess && tMiss) || tDoSwAcc) && tAccReady && !tAccDone;
	tNxtDoAcc	= ((tAccess && tMiss) || tDoSwAcc) &&
		tAccReady && !tAccDone && !tBlkDoStL;
	tNxtAccDirty	= tBlkDirty;
	
	if(tAccDone)
		tNxtDoAcc = 0;
	
	if(tNxtDoAcc)
		tHold = 1;

//	tMemOK		= tAccess ?
//		(tHold ? UMEM_OK_HOLD : UMEM_OK_OK) :
//		UMEM_OK_READY;	

//	tMemOK		=  tHold ? UMEM_OK_HOLD :
//		(tOpmIsNz ? UMEM_OK_OK : UMEM_OK_READY);

	tMemOK		=  tHold ? UMEM_OK_HOLD :
		(tAccess ? UMEM_OK_OK : UMEM_OK_READY);

	if((tMemOK == UMEM_OK_HOLD) &&
		(tMemOK2 != UMEM_OK_HOLD) &&
		(tMemOK2 != UMEM_OK_READY))
	begin
		$display("L2: Bad OK Transition");
		$display("  L2: OK   %X %X %X %X",
			tMemOK, tMemOK2, tMemOK3, tMemOK4);
		$display("  L2: OPM  %X %X %X %X",
			tReqOpm, tReqOpmL, tReqOpmL2, tReqOpmL3);
		$display("  L2: Miss %X %X %X %X",
			tMiss, tMissL, tMissL2, tMissL3);
	end

//	if(tSwFaultL && !tSwFault)
//	if(tSwFaultL && !tSwFault && !tMiss)
	if(tSwFaultL && !tSwFault && !tMiss && tOpmIsNz)
	begin
		$display("L2: Swap Fault Transition");
		$display("  L2: OK   %X %X %X %X",
			tMemOK, tMemOK2, tMemOK3, tMemOK4);
		$display("  L2: OPM  %X %X %X %X",
			tReqOpm, tReqOpmL, tReqOpmL2, tReqOpmL3);
		$display("Hold=%X Miss=%X Acc=%X Ready=%X Sticky=%X SwLatch=%X",
			tHold, tMiss, tAccLatch, tAccReady, tAccSticky, tSwLatch);

	end

`ifndef def_true
//	if(tSwFault && tHold)
	if(tSwFault && tHold && tAddrIsRamB)
	begin
		$display("L2: Swap Fault Held");
		$display("  L2: OK   %X %X %X %X",
			tMemOK, tMemOK2, tMemOK3, tMemOK4);
		$display("  L2: OPM  %X %X %X %X",
			tReqOpm, tReqOpmL, tReqOpmL2, tReqOpmL3);
		$display("Hold=%X Miss=%X Acc=%X Ready=%X Sticky=%X SwLatch=%X",
			tHold, tMiss, tAccLatch, tAccReady, tAccSticky, tSwLatch);

	end
`endif

`ifdef jx2_mem_fulldpx
	if(tSwFault)
	begin
		tMemDataOut		= UVTILE_00;
//		tMemOK			= UMEM_OK_FAULT;
		tMemOK			= tHold ? UMEM_OK_HOLD : UMEM_OK_FAULT;
	end
`endif

	if(regDeadlockStrobe)
	begin
		$display("L2: Deadlock Strobe");
		$display("  L2: OK   %X %X %X %X",
			tMemOK, tMemOK2, tMemOK3, tMemOK4);
		$display("  L2: OPM  %X %X %X %X",
			tReqOpm, tReqOpmL, tReqOpmL2, tReqOpmL3);
		$display("  L2: Hold=%X Miss=%X Acc=%X Ready=%X Sticky=%X SwLatch=%X",
			tHold, tMiss, tAccLatch, tAccReady, tAccSticky, tSwLatch);
			
		$display("  L2: ddrAddr=%X ddrOpm=%X ddrOK=%X",
			ddrMemAddr, ddrMemOpm, ddrMemOK);
	end

`ifndef def_true
	if(reset)
	begin
		tMemOK = UMEM_OK_OK;
	end
`endif
end

always @(posedge clock)
begin
// `ifndef def_true
// `ifndef jx2_mem_l2exldcyc
`ifdef jx2_l2d_noedy_memin
	if(!tHold)
	begin
		tReqAddr	<= nxtReqAddr;
		tReqAddrB	<= nxtReqAddrB;
		tReqAddrLo	<= nxtReqAddrLo;
		tReqIx		<= nxtReqIx;
		tReqIxB		<= nxtReqIxB;
		tReqOpm		<= memOpm;
		tReqDataIn	<= memDataIn;
	end
`endif

// `ifdef def_true
// `ifdef jx2_mem_l2exldcyc
`ifndef jx2_l2d_noedy_memin
	tReqAddr0	<= nxtReqAddr;
	tReqAddrB0	<= nxtReqAddrB;
	tReqAddrLo0	<= nxtReqAddrLo;
	tReqIx0		<= nxtReqIx;
	tReqIxB0	<= nxtReqIxB;
	tReqOpm0	<= memOpm;
	tReqDataIn0	<= memDataIn;

//	if(!tHoldL)
	if(!tHold)
	begin
		tReqAddr	<= tReqAddr0;
		tReqAddrB	<= tReqAddrB0;
		tReqAddrLo	<= tReqAddrLo0;
		tReqIx		<= tReqIx0;
		tReqIxB		<= tReqIxB0;
		tReqOpm		<= tReqOpm0;
		tReqDataIn	<= tReqDataIn0;
	end
`endif

	tRovIx			<= nxtRovIx;
	tRovIxCnt		<= nxtRovIxCnt;

	tCurFrov		<= tNxtFrov;
	tDoFlushL2		<= tNxtDoFlushL2;
	tDoFlushL2L		<= tDoFlushL2;

	tReqAddrL		<= tReqAddr;
	tReqAddrBL		<= tReqAddrB;
	tReqIxL			<= tReqIx;
	tReqIxBL		<= tReqIxB;
	tReqOpmL		<= tReqOpm;
	tReqOpmL2		<= tReqOpmL;
	tReqOpmL3		<= tReqOpmL2;

	tReqIxL2		<= tReqIxL;
	tReqIxBL2		<= tReqIxBL;

	tBlkDirtyL		<= tBlkDirty;
	tBlkDirtyBL		<= tBlkDirtyB;
	tBlkFlushL		<= tBlkFlush;
	tBlkFlushBL		<= tBlkFlushB;
	tAccReadyL		<= tAccReady;

	tHoldL			<= tHold;
	tMissL			<= tMiss;
	tMissL2			<= tMissL;
	tMissL3			<= tMissL2;
	tMissBL			<= tMissB;

	tSwFaultL		<= tSwFault;

	tSwLatch		<= tNxtSwLatch;
	tStLatch		<= tNxtStLatch;
	tDoAcc			<= tNxtDoAcc;
	tAccDirty		<= tNxtAccDirty;
	tAccAddr		<= tNxtAccAddr;
	tAccIx			<= tNxtAccIx;

	tBlkSwDataL		<= tBlkSwData;
	tBlkSwAddrL		<= tBlkSwAddr;
	tBlkSwDirtyL	<= tBlkSwDirty;
	tBlkSwIxL		<= tBlkSwIx;
	tBlkDoSwL		<= tBlkDoSw;

	tDdrMemDataOutB	<= tDdrMemDataOut;
	tDdrMemAddrB	<= tDdrMemAddr;
	tDdrMemOpmB		<= tDdrMemOpm;

`ifndef jx2_l2d_nofw_ddrin
	tDdrMemDataIn	<= ddrMemDataIn;
	tDdrMemOK		<= ddrMemOK;
`endif

//	tAccBlkHalf	<= tNxtBlkHalf;

`ifdef jx2_mem_l2exldcyc
	tBlkData	<= tBlkData_A0;
	tBlkAddr	<= tBlkAddr_A0;
	tBlkFlag	<= tBlkFlag_A0;
	tBlkFrov	<= tBlkFrov_A0;
`ifdef jx2_mem_fulldpx
	tBlkAddrB	<= tBlkAddrB_A0;
	tBlkFlagB	<= tBlkFlagB_A0;
	tBlkFrovB	<= tBlkFrovB_A0;
`endif

	tBlkData_A0									<= memTileData[tReqIx];
	{ tBlkFrov_A0, tBlkFlag_A0, tBlkAddr_A0 }	<= memTileAddr[tReqIx];
`ifdef jx2_mem_fulldpx
	{ tBlkFrovB_A0, tBlkFlagB_A0, tBlkAddrB_A0 }	<= memTileAddrB[tReqIxB];
`endif

`else

	tBlkData					<= memTileData[tReqIx];
//	tBlkAddr					<= memTileAddr[tReqIx];
//	{ tBlkFrov, tBlkFlag }		<= memTileFlag[tReqIx];
	{ tBlkFrov, tBlkFlag, tBlkAddr }	<= memTileAddr[tReqIx];
`ifdef jx2_mem_fulldpx
//	tBlkAddrB					<= memTileAddrB[tReqIxB];
//	{ tBlkFrovB, tBlkFlagB }	<= memTileFlagB[tReqIxB];
	{ tBlkFrovB, tBlkFlagB, tBlkAddrB }	<= memTileAddrB[tReqIxB];
`endif

`endif

	tBlkStDataL		<= tBlkStData;
	tBlkStAddrL		<= tBlkStAddr;
	tBlkStFrovL		<= tBlkStFrov;
	tBlkStDirtyL	<= tBlkStDirty;
	tBlkStIxL		<= tBlkStIx;

	tBlkDoStL		<= tBlkDoSt;

// `ifndef def_true
`ifndef jx2_mem_l2exstcyc
	if(tBlkDoSt)
	begin
		memTileData[tBlkStIx]	<= tBlkStData;
//		memTileAddr[tBlkStIx]	<= tBlkStAddr;
//		memTileFlag[tBlkStIx]	<= { tBlkStFrov, 3'b100, tBlkStDirty};
		memTileAddr[tBlkStIx]	<=
			{ tBlkStFrov, 3'b100, tBlkStDirty, tBlkStAddr};
`ifdef jx2_mem_fulldpx
//		memTileAddrB[tBlkStIx]	<= tBlkStAddr;
//		memTileFlagB[tBlkStIx]	<= { tBlkStFrov, 3'b100, tBlkStDirty};
		memTileAddrB[tBlkStIx]	<=
			{ tBlkStFrov, 3'b100, tBlkStDirty, tBlkStAddr};
`endif
		tAccSticky	<= 0;
	end
`endif

// `ifdef def_true
`ifdef jx2_mem_l2exstcyc
	if(tBlkDoStL)
	begin
		memTileData[tBlkStIxL]	<= tBlkStDataL;
		memTileAddr[tBlkStIxL]	<=
			{ tBlkStFrovL, 3'b100,
			  tBlkStDirtyL, tBlkStAddrL};
		tAccSticky	<= 0;
	end
`endif

	if(reset)
	begin
		tAccLatch		<= 0;
		tAccDone		<= 0;
		tAccSwDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;

		tDdrMemOpm		<= UMEM_OPM_READY;
		tDdrMemAddr		<= 0;
	end
	else
		if(tDoAcc || tAccLatch)
	begin
		if(tAccDone)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;
		end
		else
			if((tDdrMemOK==UMEM_OK_OK) && tAccLatch)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;

//			if(!tBlkDirty || tAccStDone)
			if(!tAccDirty || tAccStDone)
			begin
				tBlkLdData	<= tDdrMemDataIn;
				tBlkLdAddr	<= tAccAddr;
				tBlkLdIx	<= tAccIx;
				tBlkLdFrov	<= tCurFrov;
				tAccDone	<= 1;
				tAccSticky	<= 1;
				tAccLatch	<= 0;
				tAccSwDone	<= tSwLatch;
			end
//			else if(tBlkDirty)
			else if(tAccDirty)
			begin
				tNxtStDone	<= 1;
			end
		end
		else if((tDdrMemOK==UMEM_OK_HOLD) && tAccLatch)
		begin
			tDdrMemDataOut	<= tDdrMemDataOut;
			tDdrMemAddr		<= tDdrMemAddr;
			tDdrMemOpm		<= tDdrMemOpm;
		end
		else
			if(tDdrMemOK==UMEM_OK_READY)
		begin
			tAccStDone	<= tNxtStDone;

			if(tAccDirty && !tBlkAddrIsRam)
			begin
				$display("L2: Store Non-RAM Block Addr=%X", tBlkAddr);
			end
				
//			if(tBlkDirty && !tNxtStDone)
			if(tAccDirty && !tNxtStDone)
			begin
				tDdrMemDataOut	<= tBlkData;
				tDdrMemAddr		<= {tBlkAddr, 4'b0000};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tAccLatch		<= 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemDataOut	<= UVDDRT_00;
				tDdrMemAddr		<= {tAccAddr, 4'b0000};
				tDdrMemOpm		<= UMEM_OPM_RD_TILE;
				tAccLatch		<= 1;
			end
		end
		else
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;
		end
	end
	else
	begin
		tAccDone		<= 0;
		tAccSwDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;

		tDdrMemOpm		<= UMEM_OPM_READY;
		tDdrMemAddr		<= 0;
	end

	tMemDataOut2		<= tMemDataOut;
	tMemOK2				<= tMemOK;
	tMemOK3				<= tMemOK2;
	tMemOK4				<= tMemOK3;
end

endmodule

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
	ddrMemOK
	);

input			clock;
input			reset;
	
input[31:0]		memAddr;
input[31:0]		memAddrB;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[4:0]		memOpm;
output[1:0]		memOK;

input[127:0]	ddrMemDataIn;
output[127:0]	ddrMemDataOut;
output[31:0]	ddrMemAddr;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;


reg[127:0]		tMemDataOut;
reg[  1:0]		tMemOK;

assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;


reg[127:0]		tDdrMemDataIn;
reg[1:0]		tDdrMemOK;

reg[127:0]		tDdrMemDataOut;
reg[ 31:0]		tDdrMemAddr;
reg[  4:0]		tDdrMemOpm;

reg[127:0]		tDdrMemDataOutB;
reg[ 31:0]		tDdrMemAddrB;
reg[  4:0]		tDdrMemOpmB;

assign	ddrMemDataOut	= tDdrMemDataOutB;
assign	ddrMemAddr		= tDdrMemAddrB;
assign	ddrMemOpm		= tDdrMemOpmB;

`ifdef jx2_expand_l2sz
reg[127:0]	memTileData[8191:0];
reg[ 27:0]	memTileAddr[8191:0];
// reg[  3:0]	memTileFlag[8191:0];
reg[  7:0]	memTileFlag[8191:0];
`ifdef jx2_mem_fulldpx
reg[ 27:0]	memTileAddrB[8191:0];
// reg[  3:0]	memTileFlagB[8191:0];
reg[  7:0]	memTileFlagB[8191:0];
`endif
`else

`ifdef jx2_reduce_l2sz
reg[127:0]	memTileData[1023:0];
reg[ 27:0]	memTileAddr[1023:0];
// reg[  3:0]	memTileFlag[1023:0];
reg[  7:0]	memTileFlag[1023:0];
`ifdef jx2_mem_fulldpx
reg[ 27:0]	memTileAddrB[1023:0];
// reg[  3:0]	memTileFlagB[1023:0];
reg[  7:0]	memTileFlagB[1023:0];
`endif
`else
reg[127:0]	memTileData[4095:0];
reg[ 27:0]	memTileAddr[4095:0];
// reg[  3:0]	memTileFlag[4095:0];
reg[  7:0]	memTileFlag[4095:0];
`ifdef jx2_mem_fulldpx
reg[ 27:0]	memTileAddrB[4095:0];
// reg[  3:0]	memTileFlagB[4095:0];
reg[  7:0]	memTileFlagB[4095:0];
`endif
`endif

`endif

`ifdef jx2_expand_l2sz
reg[12:0]	nxtReqIx;
reg[12:0]	nxtReqIxB;
reg[12:0]	tReqIx;
reg[12:0]	tReqIxL;
reg[12:0]	tReqIxB;
reg[12:0]	tReqIxBL;
reg[12:0]	tBlkLdIx;
reg[12:0]	tBlkStIx;
reg[12:0]	tBlkSwIx;
reg[12:0]	tBlkSwIxL;
reg[12:0]	tAccIx;

reg[12:0]	nxtRovIx;
reg[12:0]	tRovIx;

`else

`ifdef jx2_reduce_l2sz
reg[9:0]	nxtReqIx;
reg[9:0]	nxtReqIxB;
reg[9:0]	tReqIx;
reg[9:0]	tReqIxL;
reg[9:0]	tReqIxB;
reg[9:0]	tReqIxBL;
reg[9:0]	tBlkLdIx;
reg[9:0]	tBlkStIx;
reg[9:0]	tBlkSwIx;
reg[9:0]	tBlkSwIxL;

reg[9:0]	tAccIx;

reg[9:0]	nxtRovIx;
reg[9:0]	tRovIx;

`else
reg[11:0]	nxtReqIx;
reg[11:0]	nxtReqIxB;
reg[11:0]	tReqIx;
reg[11:0]	tReqIxL;
reg[11:0]	tReqIxB;
reg[11:0]	tReqIxBL;
reg[11:0]	tBlkLdIx;
reg[11:0]	tBlkStIx;
reg[11:0]	tBlkSwIx;
reg[11:0]	tBlkSwIxL;

reg[11:0]	tAccIx;

reg[11:0]	nxtRovIx;
reg[11:0]	tRovIx;

`endif
`endif

reg[27:0]	nxtReqAddr;
reg[27:0]	nxtReqAddrB;
reg[3:0]	nxtReqAddrLo;

reg[27:0]	tReqAddr;
reg[27:0]	tReqAddrB;
reg[27:0]	tReqAddrL;
reg[27:0]	tReqAddrBL;
reg[27:0]	tAccAddr;

reg[3:0]	tReqAddrLo;
// reg[3:0]	tReqAddrLoB;

reg[4:0]	tReqOpm;
reg[4:0]	tReqOpmL;
reg[127:0]	tReqDataIn;

reg[127:0]	tBlkData;
reg[27:0]	tBlkAddr;
reg[27:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlag;
reg[ 3:0]	tBlkFlagB;
reg[ 3:0]	tBlkFrov;
reg[ 3:0]	tBlkFrovB;
reg			tBlkDirty;
reg			tBlkDirtyB;
reg			tBlkFlush;

reg[ 3:0]	tCurFrov;
reg[ 3:0]	tNxtFrov;

reg[3:0]	nxtRovIxCnt;
reg[3:0]	tRovIxCnt;


reg[127:0]	tBlkLdData;
reg[27:0]	tBlkLdAddr;
reg[3:0]	tBlkLdFrov;

reg[127:0]	tBlkStData;
reg[27:0]	tBlkStAddr;
reg[3:0]	tBlkStFrov;
reg			tBlkStDirty;
reg			tBlkDoSt;
reg			tBlkDoStL;

reg[127:0]	tBlkSwData;
reg[27:0]	tBlkSwAddr;
reg			tBlkSwDirty;
reg			tBlkDoSw;

reg[127:0]	tBlkSwDataL;
reg[27:0]	tBlkSwAddrL;
reg			tBlkSwDirtyL;
reg			tBlkDoSwL;

reg		tMiss;
reg		tMissB;
reg		tHold;
reg		tAccess;
reg		tAccessB;

reg		tAddrIsRam;
reg 	tOpmIsNz;
reg		tDoAcc;
reg		tDoSwAcc;
reg		tNxtDoAcc;

reg		tSwLatch;
reg		tNxtSwLatch;
reg		tAccReady;

reg tAccLatch;
// reg tAccLatchB;
reg tAccDone;
reg tAccStDone;
reg tNxtStDone;
reg tAccSticky;
reg	tAccBlkHalf;
reg	tNxtBlkHalf;

reg	tNxtDoFlushL2;
reg	tDoFlushL2;
reg	tDoFlushL2L;

always @*
begin
	/* Input End */
	nxtReqAddr		= memAddr [31:4];
	nxtReqAddrB		= memAddrB[31:4];
	nxtReqAddrLo	= memAddr [ 3:0];

`ifdef jx2_expand_l2sz
//	nxtReqIx	= memAddr [16:4];
//	nxtReqIxB	= memAddrB[16:4];
	nxtReqIx	= memAddr [16:4]^memAddr [29:17];
	nxtReqIxB	= memAddrB[16:4]^memAddrB[29:17];
`else

`ifdef jx2_reduce_l2sz
	nxtReqIx	= memAddr [13:4];
	nxtReqIxB	= memAddrB[13:4];
`else
	nxtReqIx	= memAddr [15:4];
	nxtReqIxB	= memAddrB[15:4];
//	nxtReqIx	= memAddr [15:4] ^ memAddr[27:16];
//	nxtReqIxB	= memAddrB[15:4] ^ memAddrB[27:16];

//	nxtReqIx	= memAddr [15:4] ^
//		{ memAddr[19:16], memAddr[23:20], memAddr[27:24] };
//	nxtReqIxB	= memAddrB[15:4] ^
//		{ memAddrB[19:16], memAddrB[23:20], memAddrB[27:24] };

//	nxtReqIx	= {
//		memAddr [15:10] ^ memAddr [21:16] ^ memAddr [27:22],
//		memAddr [9:4] };
//	nxtReqIxB	= {
//		memAddrB[15:10] ^ memAddrB[21:16] ^ memAddrB[27:22],
//		memAddrB[9:4] };

`endif
`endif

// `ifdef def_true
`ifndef def_true
//	if((memOpm==UMEM_OPM_READY) && (tRovIxCnt == 0))
//	if(memOpm==UMEM_OPM_READY)
	if((memOpm==UMEM_OPM_READY) && (tReqOpmL==UMEM_OPM_READY))
		nxtReqIx = tRovIx;
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

`ifdef def_true
// `ifndef def_true
	nxtRovIx = tRovIx;
	nxtRovIxCnt	= tRovIxCnt + 1;
	if(tRovIxCnt == 0)
		nxtRovIx = tRovIx + 1;
`endif

	/* Swap State */
	
	tAccIx		= tReqIx;
	tAccAddr	= tReqAddr;
//	tDoAcc		= 0;
	tNxtDoAcc	= 0;
	tDoSwAcc	= 0;
	tNxtSwLatch	= tSwLatch;

	tBlkSwData	= tBlkSwDataL;
	tBlkSwAddr	= tBlkSwAddrL;
	tBlkSwDirty	= tBlkSwDirtyL;
	tBlkSwIx	= tBlkSwIxL;
//	tBlkDoSw	= tBlkDoSwL;
	tBlkDoSw	= 0;

	tAddrIsRam	= (tReqAddr[25:20]!=6'h00) &&
		(tReqAddr[27:26]==2'b00);

//	tAddrIsRam	= (tReqAddr[25:24]!=2'b00) &&
//	tAddrIsRam	= (tReqAddr[27:24]!=4'h0);
	
//	$display("tAddrIsRam = %X, A=%X", tAddrIsRam, tReqAddr);
	
//	tOpmIsNz	= (tReqOpm[4:3]!=0);
	tOpmIsNz	= (tReqOpm[4:3]!=0) && (tReqOpm[2:0]==3'b111);
	tAccessB	= (tReqOpm[4:3]==3);
	tAccReady	= (tReqIxL == tReqIx);
//	tAccReady	= (tReqIxL == tReqIx) && !tBlkDoStL;
//	tAccReady	= (tReqIxL == tReqIx) && (tReqOpmL == tReqOpm);

	tBlkFlush	= (tCurFrov != tBlkFrov);

//	tMiss		= (tReqAddr != tBlkAddr) && tOpmIsNz;
//	tMiss		= ((tReqAddr != tBlkAddr) || tBlkFlush) &&
//		((tOpmIsNz && tAddrIsRam) || tBlkDoSwL);
	tMiss		= ((tReqAddr != tBlkAddr) || tBlkFlush) &&
		(tOpmIsNz && tAddrIsRam);

//	tMiss		= (tReqAddr != tBlkAddr) && (tOpmIsNz && tAddrIsRam);
//	if(tBlkFlush)
//		tMiss	= 1;

`ifdef jx2_mem_fulldpx
	tMissB		= (tReqAddrB != tBlkAddrB);
`endif
	tMemDataOut	= tBlkData;
	tBlkDirty	= tBlkFlag[0];

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
//	if((tMissB && !tSwLatch) && (tReqOpm[4:3]==2'b11))
	if(((tMissB && tBlkDirtyB) && !tSwLatch) && (tReqOpm[4:3]==2'b11))
	begin
//		$display("L2-A: SW %X %X", tReqAddr, tReqAddrB);
		tAccAddr	= tReqAddrB;
		tAccIx		= tReqIxB;
		tDoSwAcc	= 1;
		tHold		= 1;
	end
`endif
	
	if(tHold)
	begin
//		$display("L2 Hold tMiss=%d tAccLatch=%d", tMiss, tAccLatch);
	end

	tBlkStData	= UV128_XX;
	tBlkStAddr	= UV28_XX;
	tBlkStFrov	= tCurFrov;
`ifdef jx2_expand_l2sz
	tBlkStIx	= UV13_XX;
`else
`ifdef jx2_reduce_l2sz
	tBlkStIx	= UV10_XX;
`else
	tBlkStIx	= UV12_XX;
`endif
`endif
	tBlkStDirty	= 0;
	tBlkDoSt	= 0;

	if(tAccSticky)
	begin
//		$display("L2 Sticky");

		tBlkStData	= tBlkLdData;
		tBlkStAddr	= tBlkLdAddr;
		tBlkStIx	= tBlkLdIx;
		tBlkStFrov	= tBlkLdFrov;
		tBlkStDirty	= 0;
		tBlkDoSt	= 1;
		if(tReqOpm[4:3]!=2'b00)
		begin
			tAccess	= 1;
			tHold	= 1;
		end
	end
	else
	if(tAddrIsRam)
	begin
		if(tReqOpm==UMEM_OPM_RD_TILE)
		begin
			tAccess		= 1;
			tNxtSwLatch	= 0;
		end
		else if(tReqOpm==UMEM_OPM_WR_TILE)
		begin
			tAccess		= 1;
			tNxtSwLatch	= 0;
			if(!tMiss)
			begin
				tBlkStData	= tReqDataIn;
				tBlkStAddr	= tReqAddr;
				tBlkStIx	= tReqIx;
				tBlkStFrov	= tCurFrov;
				tBlkStDirty	= 1;
				tBlkDoSt	= 1;
			end
		end
`ifdef jx2_mem_fulldpx
		else if(tReqOpm==UMEM_OPM_SW_TILE)
		begin
//			$display("L2-B: SW %X %X", tReqAddr, tReqAddrB);

			tAccess		= 1;
			
//			if((tMissB && !tSwLatch) || tMiss)
			if(((tMissB && tBlkDirtyB) && !tSwLatch) || tMiss)
				tHold	= 1;
			
			if((!tMissB || !tBlkDirtyB) &&
				!tSwLatch && (tReqIxB == tReqIxBL))
			begin
//				$display("L2-B-1: SW-ST %X", tReqAddrB);

				tBlkStData	= tReqDataIn;
				tBlkStAddr	= tReqAddrB;
				tBlkStIx	= tReqIxB;
				tBlkStFrov	= tCurFrov;
				tBlkStDirty	= 1;
				tBlkDoSt	= 1;
				tNxtSwLatch	= 1;
			end
		end
`else
		else if(tReqOpm==UMEM_OPM_SW_TILE)
		begin
			$display("L2: Bad Swap A=%X", tReqAddr);
		end
`endif
		else
		begin
			tNxtSwLatch	= 0;
		end

	end
// `ifdef def_true
`ifndef def_true
	else if(tBlkFlush && tBlkDirty && tAccReady &&
			(tReqOpmL==UMEM_OPM_READY))
	begin
		tAccAddr	= tBlkAddr;
		tAccIx		= tReqIxL;

		$display("L2D Flush Evict A=%X Ix=%X", tBlkAddr, tReqIxL);
		tAccess	= 1;
		tHold	= 1;
		tMiss	= 1;
	end
`endif
	else
	begin
		tNxtSwLatch	= 0;
	end

//	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady;
//	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady && !tAccDone;
	tNxtDoAcc	= ((tAccess && tMiss) || tDoSwAcc) && tAccReady && !tAccDone;

//	tMemOK		= tAccess ?
//		(tHold ? UMEM_OK_HOLD : UMEM_OK_OK) :
//		UMEM_OK_READY;	

	tMemOK		=  tHold ? UMEM_OK_HOLD :
		(tOpmIsNz ? UMEM_OK_OK : UMEM_OK_READY);	
end

always @(posedge clock)
begin
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

	tRovIx		<= nxtRovIx;
	tRovIxCnt	<= nxtRovIxCnt;

	tCurFrov	<= tNxtFrov;
	tDoFlushL2	<= tNxtDoFlushL2;
	tDoFlushL2L	<= tDoFlushL2;

	tReqAddrL	<= tReqAddr;
	tReqAddrBL	<= tReqAddrB;
	tReqIxL		<= tReqIx;
	tReqIxBL	<= tReqIxB;
	tReqOpmL	<= tReqOpm;

	tSwLatch	<= tNxtSwLatch;
	tDoAcc		<= tNxtDoAcc;

	tBlkSwDataL		<= tBlkSwData;
	tBlkSwAddrL		<= tBlkSwAddr;
	tBlkSwDirtyL	<= tBlkSwDirty;
	tBlkSwIxL		<= tBlkSwIx;
	tBlkDoSwL		<= tBlkDoSw;

	tDdrMemDataOutB	<= tDdrMemDataOut;
	tDdrMemAddrB	<= tDdrMemAddr;
	tDdrMemOpmB		<= tDdrMemOpm;

	tDdrMemDataIn	<= ddrMemDataIn;
	tDdrMemOK		<= ddrMemOK;

//	tAccBlkHalf	<= tNxtBlkHalf;

	tBlkData					<= memTileData[tReqIx];
	tBlkAddr					<= memTileAddr[tReqIx];
	{ tBlkFrov, tBlkFlag }		<= memTileFlag[tReqIx];
`ifdef jx2_mem_fulldpx
	tBlkAddrB					<= memTileAddrB[tReqIxB];
	{ tBlkFrovB, tBlkFlagB }	<= memTileFlagB[tReqIxB];
`endif
	
	tBlkDoStL	<= tBlkDoSt;
	if(tBlkDoSt)
	begin
		memTileData[tBlkStIx]	<= tBlkStData;
		memTileAddr[tBlkStIx]	<= tBlkStAddr;
		memTileFlag[tBlkStIx]	<= { tBlkStFrov, 3'b100, tBlkStDirty};
`ifdef jx2_mem_fulldpx
		memTileAddrB[tBlkStIx]	<= tBlkStAddr;
		memTileFlagB[tBlkStIx]	<= { tBlkStFrov, 3'b100, tBlkStDirty};
`endif
		tAccSticky	<= 0;
	end
	
	if(reset)
	begin
		tAccLatch		<= 0;
		tAccDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;
	end
	else
		if(tDoAcc || tAccLatch)
	begin
		if((tDdrMemOK==UMEM_OK_OK) && tAccLatch)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;

			if(!tBlkDirty || tAccStDone)
			begin
				tBlkLdData	<= tDdrMemDataIn;
				tBlkLdAddr	<= tAccAddr;
				tBlkLdIx	<= tAccIx;
				tBlkLdFrov	<= tCurFrov;
				tAccDone	<= 1;
				tAccSticky	<= 1;
				tAccLatch	<= 0;
			end
			else if(tBlkDirty)
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

			if(tBlkDirty && !tNxtStDone)
			begin
				tDdrMemDataOut	<= tBlkData;
				tDdrMemAddr		<= {tBlkAddr, 4'b0000};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tAccLatch		<= 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemDataOut	<= UV128_00;
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
		tAccStDone		<= 0;
		tNxtStDone		<= 0;
	end
end

endmodule

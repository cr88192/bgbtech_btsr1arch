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
reg[127:0]		tDdrMemDataOut;
reg[ 31:0]		tDdrMemAddr;
reg[  4:0]		tDdrMemOpm;

assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;
assign	ddrMemDataOut	= tDdrMemDataOut;
assign	ddrMemAddr		= tDdrMemAddr;
assign	ddrMemOpm		= tDdrMemOpm;

`ifdef jx2_reduce_l2sz
reg[127:0]	memTileData[1023:0];
reg[ 27:0]	memTileAddr[1023:0];
reg[  3:0]	memTileFlag[1023:0];
`ifdef jx2_mem_fulldpx
reg[ 27:0]	memTileAddrB[1023:0];
reg[  3:0]	memTileFlagB[1023:0];
`endif
`else
reg[127:0]	memTileData[4095:0];
reg[ 27:0]	memTileAddr[4095:0];
reg[  3:0]	memTileFlag[4095:0];
`ifdef jx2_mem_fulldpx
reg[ 27:0]	memTileAddrB[4095:0];
reg[  3:0]	memTileFlagB[4095:0];
`endif
`endif

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

`endif

reg[27:0]	nxtReqAddr;
reg[27:0]	nxtReqAddrB;

reg[27:0]	tReqAddr;
reg[27:0]	tReqAddrB;
reg[27:0]	tReqAddrL;
reg[27:0]	tReqAddrBL;
reg[27:0]	tAccAddr;

reg[4:0]	tReqOpm;
reg[127:0]	tReqDataIn;

reg[127:0]	tBlkData;
reg[27:0]	tBlkAddr;
reg[27:0]	tBlkAddrB;
reg[ 3:0]	tBlkFlag;
reg[ 3:0]	tBlkFlagB;
reg			tBlkDirty;
reg			tBlkDirtyB;

reg[127:0]	tBlkLdData;
reg[27:0]	tBlkLdAddr;

reg[127:0]	tBlkStData;
reg[27:0]	tBlkStAddr;
reg			tBlkStDirty;
reg			tBlkDoSt;

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


always @*
begin
	/* Input End */
	nxtReqAddr	= memAddr [31:4];
	nxtReqAddrB	= memAddrB[31:4];

`ifdef jx2_reduce_l2sz
	nxtReqIx	= memAddr [13:4];
	nxtReqIxB	= memAddrB[13:4];
`else
	nxtReqIx	= memAddr [15:4];
	nxtReqIxB	= memAddrB[15:4];
`endif

	/* Swap State */
	
	tAccIx		= tReqIx;
	tAccAddr	= tReqAddr;
	tDoAcc		= 0;
	tDoSwAcc	= 0;
	tNxtSwLatch	= tSwLatch;

	tBlkSwData	= tBlkSwDataL;
	tBlkSwAddr	= tBlkSwAddrL;
	tBlkSwDirty	= tBlkSwDirtyL;
	tBlkSwIx	= tBlkSwIxL;
	tBlkDoSw	= tBlkDoSwL;

	tAddrIsRam	= (tReqAddr[25:20]!=6'h00) &&
		(tReqAddr[27:26]==2'b00);

//	tAddrIsRam	= (tReqAddr[25:24]!=2'b00) &&
//	tAddrIsRam	= (tReqAddr[27:24]!=4'h0);
	
//	$display("tAddrIsRam = %X, A=%X", tAddrIsRam, tReqAddr);
	
	tOpmIsNz	= (tReqOpm[4:3]!=0);
	tAccessB	= (tReqOpm[4:3]==3);
	tAccReady	= (tReqIxL == tReqIx);

//	tMiss		= (tReqAddr != tBlkAddr) && tOpmIsNz;
	tMiss		= (tReqAddr != tBlkAddr) && ((tOpmIsNz && tAddrIsRam) || tBlkDoSwL);
	tMissB		= (tReqAddrB != tBlkAddrB);
	tMemDataOut	= tBlkData;
	tBlkDirty	= tBlkFlag[0];
	tBlkDirtyB	= tBlkFlagB[0];
	
	tHold		= tMiss || tAccLatch || !tAccReady;
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
`ifdef jx2_reduce_l2sz
	tBlkStIx	= UV10_XX;
`else
	tBlkStIx	= UV12_XX;
`endif
	tBlkStDirty	= 0;
	tBlkDoSt	= 0;

// `ifdef jx2_mem_fulldpx
`ifndef def_true
//	if(tBlkDoSwL && (tReqAddr==tBlkSwAddrL) && !tMiss)
	if(tBlkDoSwL && (tReqIxB == tReqIx) && !tMiss)
	begin
		tBlkStData	= tBlkSwDataL;
		tBlkStAddr	= tBlkSwAddrL;
		tBlkStIx	= tBlkSwIxL;
		tBlkStDirty	= tBlkSwDirtyL;
		tBlkDoSt	= 1;
		tBlkDoSw	= 0;
		if(tReqOpm[4:3]!=2'b00)
		begin
			tAccess	= 1;
			tHold	= 1;
		end
	end
	else
`endif
	if(tAccSticky)
	begin
//		$display("L2 Sticky");

		tBlkStData	= tBlkLdData;
		tBlkStAddr	= tBlkLdAddr;
		tBlkStIx	= tBlkLdIx;
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
			
// `ifndef def_true
`ifdef def_true
//			if(!tMissB && !tSwLatch && (tReqIxB == tReqIxBL))
			if((!tMissB || !tBlkDirtyB) &&
				!tSwLatch && (tReqIxB == tReqIxBL))
			begin
//				$display("L2-B-1: SW-ST %X", tReqAddrB);

				tBlkStData	= tReqDataIn;
				tBlkStAddr	= tReqAddrB;
				tBlkStIx	= tReqIxB;
				tBlkStDirty	= 1;
				tBlkDoSt	= 1;
				tNxtSwLatch	= 1;
			end
//			else
`endif

`ifndef def_true
			if(tBlkDoSwL || tMiss)
			begin
				tAccess	= 1;
				tHold	= 1;
			end
			else
			if(!tMiss && (tReqIxB == tReqIxBL))
			begin
				tBlkSwData	= tReqDataIn;
				tBlkSwAddr	= tReqAddrB;
				tBlkSwIx	= tReqIxB;
				tBlkSwDirty	= 1;
				tBlkDoSw	= 1;
			end
			else
`endif
//			begin
//				$display("L2-B-2: Hold A=%X MissB=%d Rqe=%d",
//					tReqAddrB, tMissB, (tReqIxB == tReqIxBL));
//				tAccess	= 1;
//				tHold	= 1;
//			end
		end
`endif
		else
		begin
			tNxtSwLatch	= 0;
		end

	end
	else
	begin
		tNxtSwLatch	= 0;
	end

//	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady;
	tDoAcc		= ((tAccess && tMiss) || tDoSwAcc) && tAccReady && !tAccDone;

	tMemOK		= tAccess ?
		(tHold ? UMEM_OK_HOLD : UMEM_OK_OK) :
		UMEM_OK_READY;	
end

always @(posedge clock)
begin
	if(!tHold)
	begin
		tReqAddr	<= nxtReqAddr;
		tReqAddrB	<= nxtReqAddrB;
		tReqIx		<= nxtReqIx;
		tReqIxB		<= nxtReqIxB;
		tReqOpm		<= memOpm;
		tReqDataIn	<= memDataIn;
	end

	tReqAddrL	<= tReqAddr;
	tReqAddrBL	<= tReqAddrB;
	tReqIxL		<= tReqIx;
	tReqIxBL	<= tReqIxB;

	tSwLatch	<= tNxtSwLatch;

	tBlkSwDataL		<= tBlkSwData;
	tBlkSwAddrL		<= tBlkSwAddr;
	tBlkSwDirtyL	<= tBlkSwDirty;
	tBlkSwIxL		<= tBlkSwIx;
	tBlkDoSwL		<= tBlkDoSw;

//	tAccBlkHalf	<= tNxtBlkHalf;

// `ifdef def_true
`ifndef def_true
	tBlkData	<= memTileData[nxtReqIx];
	tBlkAddr	<= memTileAddr[nxtReqIx];
	tBlkFlag	<= memTileFlag[nxtReqIx];
`ifdef jx2_mem_fulldpx
	tBlkAddrB	<= memTileAddr[nxtReqIxB];
`endif
`endif

// `ifndef def_true
`ifdef def_true
	tBlkData	<= memTileData[tReqIx];
	tBlkAddr	<= memTileAddr[tReqIx];
	tBlkFlag	<= memTileFlag[tReqIx];
`ifdef jx2_mem_fulldpx
	tBlkAddrB	<= memTileAddr[tReqIxB];
	tBlkFlagB	<= memTileFlag[tReqIxB];
`endif
`endif
	
	if(tBlkDoSt)
	begin
		memTileData[tBlkStIx]	<= tBlkStData;
		memTileAddr[tBlkStIx]	<= tBlkStAddr;
		memTileFlag[tBlkStIx]	<= { 3'b100, tBlkStDirty};
`ifdef jx2_mem_fulldpx
		memTileAddrB[tBlkStIx]	<= tBlkStAddr;
		memTileFlagB[tBlkStIx]	<= { 3'b100, tBlkStDirty};
`endif
		tAccSticky	<= 0;
	end
	
	if(reset)
	begin
		tAccLatch		<= 0;
		tAccDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;
`ifdef jx2_ddr_bl64b
		tAccBlkHalf		<= 0;
		tNxtBlkHalf		<= 0;
`endif
	end
	else
//		if((tAccess && tMiss) || tAccLatch)
		if(tDoAcc || tAccLatch)
	begin
		if((ddrMemOK==UMEM_OK_OK) && tAccLatch)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;

			if(!tBlkDirty || tAccStDone)
			begin
`ifdef jx2_ddr_bl64b
				if(tAccBlkHalf)
				begin
					tBlkLdData[127:64]	<= ddrMemDataIn[63:0];
					tBlkLdAddr	<= tAccAddr;
					tBlkLdIx	<= tAccIx;
					tAccDone	<= 1;
					tAccSticky	<= 1;
					tAccLatch	<= 0;
				end
				else
				begin
					tBlkLdData[63:0]	<= ddrMemDataIn[63:0];
					tNxtBlkHalf			<= 1;
				end
`else
				tBlkLdData	<= ddrMemDataIn;
				tBlkLdAddr	<= tAccAddr;
				tBlkLdIx	<= tAccIx;
				tAccDone	<= 1;
				tAccSticky	<= 1;
				tAccLatch	<= 0;
`endif
			end
			else if(tBlkDirty)
			begin
`ifdef jx2_ddr_bl64b
				if(tAccBlkHalf)
				begin
					tNxtStDone	<= 1;
					tNxtBlkHalf	<= 0;
				end
				else
				begin
					tNxtBlkHalf	<= 1;
				end
`else
				tNxtStDone	<= 1;
`endif
			end
		end
		else if((ddrMemOK==UMEM_OK_HOLD) && tAccLatch)
		begin
			tDdrMemDataOut	<= tDdrMemDataOut;
			tDdrMemAddr		<= tDdrMemAddr;
			tDdrMemOpm		<= tDdrMemOpm;
		end
		else
			if(ddrMemOK==UMEM_OK_READY)
		begin
			tAccStDone	<= tNxtStDone;

			if(tBlkDirty && !tNxtStDone)
			begin
`ifdef jx2_ddr_bl64b

				tDdrMemDataOut	<= { UV64_00,
					tNxtBlkHalf ? tBlkData[127:64] : tBlkData[63:0] };

				tDdrMemAddr		<= {tBlkAddr, tNxtBlkHalf, 3'b000};
				tAccBlkHalf		<= tNxtBlkHalf;
`else
				tDdrMemDataOut	<= tBlkData;
				tDdrMemAddr		<= {tBlkAddr, 4'b0000};
`endif
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tAccLatch		<= 1;
			end
//			else if(!tAccStDone)
			else if(!tAccDone)
			begin
//				tDdrMemDataOut	<= UV128_XX;
				tDdrMemDataOut	<= UV128_00;
`ifdef jx2_ddr_bl64b
				tDdrMemAddr		<= {tAccAddr, tNxtBlkHalf, 3'b000};
				tAccBlkHalf		<= tNxtBlkHalf;
`else
				tDdrMemAddr		<= {tAccAddr, 4'b0000};
`endif
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
`ifdef jx2_ddr_bl64b
		tAccBlkHalf		<= 0;
		tNxtBlkHalf		<= 0;
`endif
	end
end

endmodule

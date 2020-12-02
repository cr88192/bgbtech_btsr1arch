/*
L2 Tile Cache

Caches 128-bit tiles, 2-way associative.
*/

`include "CoreDefs.v"

module MemL2DcB(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	
	memAddr,	memOpm,
	memDataIn,	memDataOut,
	memOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK
	);

input			clock;
input			reset;
	
input[31:0]		memAddr;
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
reg[127:0]	memTileDataA[1023:0];
reg[ 27:0]	memTileAddrA[1023:0];
reg[  3:0]	memTileFlagA[1023:0];
reg[127:0]	memTileDataB[1023:0];
reg[ 27:0]	memTileAddrB[1023:0];
reg[  3:0]	memTileFlagB[1023:0];
`else
reg[127:0]	memTileDataA[4095:0];
reg[ 27:0]	memTileAddrA[4095:0];
reg[  3:0]	memTileFlagA[4095:0];
reg[127:0]	memTileDataB[4095:0];
reg[ 27:0]	memTileAddrB[4095:0];
reg[  3:0]	memTileFlagB[4095:0];
`endif

`ifdef jx2_reduce_l2sz
reg[9:0]	nxtReqIx;
reg[9:0]	tReqIx;
reg[9:0]	tBlkLdIx;
reg[9:0]	tBlkStIx;
`else
reg[11:0]	nxtReqIx;
reg[11:0]	tReqIx;
reg[11:0]	tBlkLdIx;
reg[11:0]	tBlkStIx;
`endif

reg[27:0]	nxtReqAddr;

reg[27:0]	tReqAddr;
reg[4:0]	tReqOpm;
reg[127:0]	tReqDataIn;

reg[127:0]	tBlkDataA;
reg[127:0]	tBlkDataB;
reg[ 27:0]	tBlkAddrA;
reg[ 27:0]	tBlkAddrB;
reg[  3:0]	tBlkFlagA;
reg[  3:0]	tBlkFlagB;
reg			tBlkDirtyA;
reg			tBlkDirtyB;

reg[127:0]	tBlkLdData;
reg[27:0]	tBlkLdAddr;

reg[127:0]	tBlkStDataA;
reg[127:0]	tBlkStDataB;
reg[27:0]	tBlkStAddrA;
reg[27:0]	tBlkStAddrB;
reg			tBlkStDirtyA;
reg			tBlkStDirtyB;
reg			tBlkDoStA;
reg			tBlkDoStB;

reg		tMissA;
reg		tMissB;
reg		tMiss;

reg		tHold;
reg		tAccess;

reg		tAddrIsRam;
reg 	tOpmIsNz;

reg tAccLatch;
reg tAccDone;
reg tAccStDone;
reg tNxtStDone;
reg tAccSticky;


always @*
begin
	nxtReqAddr	= memAddr[31:4];

`ifdef jx2_reduce_l2sz
	nxtReqIx	= memAddr[13:4];
`else
	nxtReqIx	= memAddr[15:4];
`endif

	tBlkStDataA		= UV128_XX;
	tBlkStDataB		= UV128_XX;
	tBlkStAddrA		= UV28_XX;
	tBlkStAddrB		= UV28_XX;
	tBlkStDirtyA	= 0;
	tBlkStDirtyB	= 0;
	tBlkDoStA		= 0;
	tBlkDoStB		= 0;

`ifdef jx2_reduce_l2sz
	tBlkStIxA	= UV10_XX;
	tBlkStIxB	= UV10_XX;
`else
	tBlkStIxA	= UV12_XX;
	tBlkStIxB	= UV12_XX;
`endif


	tAddrIsRam	= (tReqAddr[25:20]!=6'h00) &&
		(tReqAddr[27:26]==2'b00);

//	tAddrIsRam	= (tReqAddr[25:24]!=2'b00) &&
//	tAddrIsRam	= (tReqAddr[27:24]!=4'h0);
	
//	$display("tAddrIsRam = %X, A=%X", tAddrIsRam, tReqAddr);
	
	tOpmIsNz	= (tReqOpm[4:3]!=0);
//	tMiss		= (tReqAddr != tBlkAddr) && tOpmIsNz;
	tMissA		= (tReqAddr != tBlkAddrA) && tOpmIsNz && tAddrIsRam;
	tMissB		= (tReqAddr != tBlkAddrB) && tOpmIsNz && tAddrIsRam;
	tBlkDirty	= tBlkFlag[0];
	
	tMiss		= tMissA && tMissB;
	
	tHold		= tMiss || tAccLatch;
	tAccess		= 0;

	if(!tMissA)
	begin
		tMemDataOut	= tBlkDataA;
	end
	else if(!tMissB)
	begin
		tMemDataOut		= tBlkDataB;

		tBlkStDataA		= tBlkDataB;
		tBlkStAddrA		= tBlkAddrB;
		tBlkStDirtyA	= tBlkDirtyB;
		tBlkStIxA		= tReqIx;
		tBlkDoStA		= 1;

		tBlkStDataB		= tBlkDataA;
		tBlkStAddrB		= tBlkAddrA;
		tBlkStDirtyB	= tBlkDirtyA;
		tBlkStIxB		= tReqIx;
		tBlkDoStB		= 1;
	end

	
	if(tHold)
	begin
//		$display("L2 Hold tMiss=%d tAccLatch=%d", tMiss, tAccLatch);
	end

	if(tAccSticky)
	begin
//		$display("L2 Sticky");

		tBlkStDataB		= tBlkLdData;
		tBlkStAddrB		= tBlkLdAddr;
		tBlkStIxB		= tBlkLdIx;
		tBlkStDirtyB	= 0;
		tBlkDoStB		= 1;
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
		end
		else if(tReqOpm==UMEM_OPM_WR_TILE)
		begin
			tAccess			= 1;

			tBlkStDataA		= tReqDataIn;
			tBlkStAddrA		= tReqAddr;
			tBlkStIxA		= tReqIx;
			tBlkStDirtyA	= 1;
			tBlkDoStA		= 1;
		end
	end
	
	tMemOK		= tAccess ?
		((tHold ? UMEM_OK_HOLD : UMEM_OK_OK) : UMEM_OK_READY);
end

always @(posedge clock)
begin
	if(!tHold)
	begin
		tReqAddr	<= nxtReqAddr;
		tReqIx		<= nxtReqIx;
		tReqOpm		<= memOpm;
		tReqDataIn	<= memDataIn;
	end

//	tAccBlkHalf	<= tNxtBlkHalf;

	tBlkDataA	<= memTileDataA[nxtReqIx];
	tBlkDataB	<= memTileDataB[nxtReqIx];
	tBlkAddrA	<= memTileAddrA[nxtReqIx];
	tBlkAddrB	<= memTileAddrB[nxtReqIx];
	tBlkFlagA	<= memTileFlagA[nxtReqIx];
	tBlkFlagB	<= memTileFlagB[nxtReqIx];
	
	if(tBlkDoStA)
	begin
		memTileDataA[tBlkStIxA]	<= tBlkStDataA;
		memTileAddrA[tBlkStIxA]	<= tBlkStAddrA;
		memTileFlagA[tBlkStIxA]	<= { 3'b100, tBlkStDirtyA };
		tAccStickyA				<= 0;
	end

	if(tBlkDoStB)
	begin
		memTileDataB[tBlkStIxB]	<= tBlkStDataB;
		memTileAddrB[tBlkStIxB]	<= tBlkStAddrB;
		memTileFlagB[tBlkStIxB]	<= { 3'b100, tBlkStDirtyB };
		tAccStickyB				<= 0;
	end
	
	if(reset)
	begin
		tAccLatch		<= 0;
		tAccDone		<= 0;
		tAccStDone		<= 0;
		tNxtStDone		<= 0;
	end
	else
		if((tAccess && tMiss) || tAccLatch)
	begin
		if((ddrMemOK==UMEM_OK_OK) && tAccLatch)
		begin
			tDdrMemOpm		<= UMEM_OPM_READY;

			if(!tBlkDirty || tAccStDone)
			begin
				tBlkLdData	<= ddrMemDataIn;
				tBlkLdAddr	<= tReqAddr;
				tBlkLdIx	<= tReqIx;
				tAccDone	<= 1;
				tAccSticky	<= 1;
				tAccLatch	<= 0;
			end
			else if(tBlkDirty)
			begin
				tNxtStDone	<= 1;
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
				tDdrMemDataOut	<= tBlkDataB;
				tDdrMemAddr		<= {tBlkAddrB, 4'b0000};
				tDdrMemOpm		<= UMEM_OPM_WR_TILE;
				tAccLatch		<= 1;
			end
			else if(!tAccDone)
			begin
				tDdrMemDataOut	<= UV128_XX;
				tDdrMemAddr		<= {tReqAddr, 4'b0000};
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

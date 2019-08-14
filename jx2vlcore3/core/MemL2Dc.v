/*
L2 Tile Cache

Caches 128-bit tiles, 
*/

`include "CoreDefs.v"

module MemL2Dc(
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
reg[127:0]	memTileData[1023:0];
reg[ 27:0]	memTileAddr[1023:0];
reg[  3:0]	memTileFlag[1023:0];
`else
reg[127:0]	memTileData[4095:0];
reg[ 27:0]	memTileAddr[4095:0];
reg[  3:0]	memTileFlag[4095:0];
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

reg[127:0]	tBlkData;
reg[27:0]	tBlkAddr;
reg[ 3:0]	tBlkFlag;
reg			tBlkDirty;

reg[127:0]	tBlkLdData;
reg[27:0]	tBlkLdAddr;

reg[127:0]	tBlkStData;
reg[27:0]	tBlkStAddr;
reg			tBlkStDirty;
reg			tBlkDoSt;

reg		tMiss;
reg		tHold;
reg		tAccess;

reg		tAddrIsRam;
reg 	tOpmIsNz;

always @*
begin
	nxtReqAddr	= memAddr[31:4];

`ifdef jx2_reduce_l2sz
	nxtReqIx	= memAddr[13:4];
`else
	nxtReqIx	= memAddr[15:4];
`endif
	
	tAddrIsRam	= (tReqAddr[25:20]!=6'h00) &&
		(tReqAddr[27:26]==2'b00);
	
	tOpmIsNz	= (tReqOpm[4:3]!=0);
	tMiss		= (tReqAddr != tBlkAddr) && tOpmIsNz;
	tMemDataOut	= tBlkData;
	tBlkDirty	= tBlkFlag[0];
	
	tHold		= tMiss || tAccLatch;
	tAccess		= 0;

	tBlkStData	= UV128_XX;
	tBlkStAddr	= UV28_XX;
`ifdef jx2_reduce_l2sz
	tBlkStIx	= UV10_XX;
`else
	tBlkStIx	= UV12_XX;
`endif
	tBlkStDirty	= 0;
	tBlkDoSt	= 0;

	if(tAccSticky)
	begin
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
		end
		else if(tReqOpm==UMEM_OPM_WR_TILE)
		begin
			tAccess		= 1;
			tBlkStData	= tReqDataIn;
			tBlkStAddr	= tReqAddr;
			tBlkStIx	= tReqIx;
			tBlkStDirty	= 1;
			tBlkDoSt	= 1;
		end
	end
	
	tMemOK		= tAccess ?
		(tHold ? UMEM_OK_HOLD : UMEM_OK_OK) :
		UMEM_OK_READY;	
end

reg tAccLatch;
reg tAccDone;
reg tAccStDone;
reg tNxtStDone;
reg tAccSticky;
reg	tAccBlkHalf;
reg	tNxtBlkHalf;

always @(posedge clock)
begin
	if(!tHold)
	begin
		tReqAddr	<= nxtReqAddr;
		tReqIx		<= nxtReqIx;
		tReqOpm		<= memOpm;
		tReqDataIn	<= memDataIn;
	end

	tBlkData	<= memTileData[nxtReqIx];
	tBlkAddr	<= memTileAddr[nxtReqIx];
	tBlkFlag	<= memTileFlag[nxtReqIx];
	
	if(tBlkDoSt)
	begin
		memTileData[tBlkStIx]	<= tBlkStData;
		memTileAddr[tBlkStIx]	<= tBlkStAddr;
		memTileFlag[tBlkStIx]	<= { 3'b100, tBlkStDirty};
	end
	
	if((tAccess && tMiss) || tAccLatch)
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
					tBlkLdAddr	<= tReqAddr;
					tBlkLdIx	<= tReqIx;
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
				tBlkLdAddr	<= tReqAddr;
				tBlkLdIx	<= tReqIx;
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
					tNxtStDone	<= 0;
					tNxtBlkHalf	<= 0;
				end
				else
				begin
					tNxtBlkHalf	<= 1;
				end
`else
				tNxtStDone	<= 0;
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

				tDdrMemDataOut	<= { UV64_XX,
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
			else if(!tAccStDone)
			begin
				tDdrMemDataOut	<= UV128_XX;
`ifdef jx2_ddr_bl64b
				tDdrMemAddr		<= {tReqAddr, tNxtBlkHalf, 3'b000};
				tAccBlkHalf		<= tNxtBlkHalf;
`else
				tDdrMemAddr		<= {tReqAddr, 4'b0000};
`endif
				tDdrMemOpm		<= UMEM_OPM_RD_TILE;
				tAccLatch		<= 1;
			end
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

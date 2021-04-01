/*
L2 Tile ROM / SRAM

Implement a 32K ROM and 8K of SRAM.

0000..7FFF: ROM
C000..DFFF: SRAM
*/

`include "CoreDefs.v"

module MemL2Rom(
	clock,
	reset,
	
	memAddr,	memOpm,
	memDataIn,	memDataOut,
	memOK
	);


input			clock;
input			reset;
	
input[31:0]		memAddr;
`input_tile		memDataIn;
`output_tile	memDataOut;
input[4:0]		memOpm;
output[1:0]		memOK;


`reg_tile		tMemDataOut;
`reg_tile		tMemDataOut2;
reg[  1:0]		tMemOK;
reg[  1:0]		tMemOK2;

assign		memDataOut	= tMemDataOut2;
assign		memOK		= tMemOK2;

`ifdef jx2_mem_line32B

`reg_tile	romTileData[1023:0];
// reg[127:0]	romTileDataA[1023:0];
// reg[127:0]	romTileDataB[1023:0];

`reg_tile	ramTileData[ 255:0];

reg[9:0]	tRomBlkIx;
reg[9:0]	tRomBlkIxL;
reg[7:0]	tRamBlkIx;
reg[7:0]	tRamBlkIxL;
reg[7:0]	tRamStBlkIx;

`else

`reg_tile	romTileData[2047:0];
`reg_tile	ramTileData[ 511:0];

reg[10:0]	tRomBlkIx;
reg[10:0]	tRomBlkIxL;
reg[8:0]	tRamBlkIx;
reg[8:0]	tRamBlkIxL;
reg[8:0]	tRamStBlkIx;

`endif

`reg_tile	tRomBlkData;
`reg_tile	tRamBlkData;
`reg_tile	tRamStBlkData;

reg			tRamDoSt;
reg			tRamDoStL;

reg			tAddrIsRom;
reg			tAddrIsRam;
reg			tAddrIsLo64k;


reg[31:0]		tMemAddr;
reg[4:0]		tMemOpm;

`reg_tile		tMemDataIn;

initial begin

`ifdef jx2_cfg_ucrom

`ifdef jx2_mem_line32B
	$readmemh("bootrom_uc_2.txt", romTileData);
`else
	$readmemh("bootrom_uc_1.txt", romTileData);
`endif

`else

`ifdef jx2_mem_line32B
	$readmemh("bootrom_2.txt", romTileData);
//	$readmemh("bootrom_2a.txt", romTileDataA);
//	$readmemh("bootrom_2b.txt", romTileDataB);
`else
	$readmemh("bootrom_1.txt", romTileData);
`endif

`endif

end

always @*
begin
`ifdef jx2_mem_line32B
	tRomBlkIx		= memAddr[14:5];
	tRamBlkIx		= memAddr[12:5];
`else
	tRomBlkIx		= memAddr[14:4];
	tRamBlkIx		= memAddr[12:4];
`endif
	
	tAddrIsLo64k	= (tMemAddr[31:16] == UV16_00);
	tAddrIsRom		= tAddrIsLo64k && (tMemAddr[15]==1'b0);
	tAddrIsRam		= tAddrIsLo64k && (tMemAddr[15:13]==3'b110);
	tMemOK			= UMEM_OK_READY;

`ifdef jx2_mem_line32B
	tMemDataOut		= UV256_00;

	tRamStBlkData	= UV256_XX;
	tRamStBlkIx		= UV8_XX;
	tRamDoSt		= 0;
`else
	tMemDataOut		= UV128_00;

	tRamStBlkData	= UV128_XX;
	tRamStBlkIx		= UV9_XX;
	tRamDoSt		= 0;
`endif

	if(tMemOpm[2:0]!=3'b111)
	begin
		/* Request is not a memory request. */
		tMemOK			= UMEM_OK_READY;
	end
	else
	if(tMemOpm[4:3]==2'b11)
	begin
//		tMemDataOut		= UV128_00;
		tMemDataOut		= UVTILE_00;
		tMemOK			= UMEM_OK_FAULT;
	end
//	if(tAddrIsRom && tMemOpm[3])
	else
		if(tAddrIsRom && (tMemOpm[4:3]!=2'b00))
	begin
//		$display("L2 ROM Addr=%X Data=%X", tMemAddr, tRomBlkData);

		tMemDataOut		= tRomBlkData;
//		tMemOK			= UMEM_OK_OK;
		tMemOK			= (tRomBlkIxL==tRomBlkIx) ? UMEM_OK_OK : UMEM_OK_HOLD;

		if(tMemOpm[4])
		begin
			$display("L2 ROM Store A=%X D=%X O=%X",
				tMemAddr, tMemDataIn, tMemOpm);
		end

	end
	else
		if(tAddrIsRam && (tMemOpm[4:3]!=2'b00))
	begin
//		$display("L2 SRAM");

		tMemDataOut		= tRamBlkData;
		tMemOK			= UMEM_OK_OK;
//		tMemOK			= (tRamBlkIxL==tRamBlkIx) ? UMEM_OK_OK : UMEM_OK_HOLD;
//		tMemOK			= tRamDoStL ? UMEM_OK_OK : UMEM_OK_HOLD;
		
		if(tMemOpm[4])
		begin
			tRamStBlkData	= tMemDataIn;
			tRamStBlkIx		= tRamBlkIx;
			tRamDoSt		= 1;
		end
	end
end

always @(posedge clock)
begin
	tMemDataOut2	<= tMemDataOut;
	tMemOK2			<= tMemOK;

	tMemAddr		<= memAddr;
	tMemDataIn		<= memDataIn;
	tMemOpm			<= memOpm;

	tRomBlkIxL		<= tRomBlkIx;
	tRamBlkIxL		<= tRamBlkIx;
	
	tRomBlkData		<= romTileData[tRomBlkIx];
//	tRomBlkData		<= { romTileDataB[tRomBlkIx], romTileDataA[tRomBlkIx] };
	tRamBlkData		<= ramTileData[tRamBlkIx];

	tRamDoStL		<= tRamDoSt;
	
	if(tRamDoSt)
	begin
		ramTileData[tRamStBlkIx]	<= tRamStBlkData;
	end
end

endmodule

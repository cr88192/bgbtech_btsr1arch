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
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[4:0]		memOpm;
output[1:0]		memOK;


reg[127:0]		tMemDataOut;
reg[  1:0]		tMemOK;
reg[127:0]		tMemDataOut2;
reg[  1:0]		tMemOK2;

assign		memDataOut	= tMemDataOut2;
assign		memOK		= tMemOK2;

reg[127:0]	romTileData[2047:0];
reg[127:0]	ramTileData[ 511:0];

reg[127:0]	tRomBlkData;
reg[127:0]	tRamBlkData;
reg[10:0]	tRomBlkIx;
reg[8:0]	tRamBlkIx;
reg[10:0]	tRomBlkIxL;
reg[8:0]	tRamBlkIxL;

reg[127:0]	tRamStBlkData;
reg[8:0]	tRamStBlkIx;
reg			tRamDoSt;

reg			tAddrIsRom;
reg			tAddrIsRam;
reg			tAddrIsLo64k;


reg[31:0]		tMemAddr;
reg[127:0]		tMemDataIn;
reg[4:0]		tMemOpm;

initial begin
	$readmemh("bootrom_1.txt", romTileData);
end

always @*
begin
	tRomBlkIx		= memAddr[14:4];
	tRamBlkIx		= memAddr[12:4];
	
	tAddrIsLo64k	= (tMemAddr[31:16] == UV16_00);
	tAddrIsRom		= tAddrIsLo64k && (tMemAddr[15]==1'b0);
	tAddrIsRam		= tAddrIsLo64k && (tMemAddr[15:13]==3'b110);
	tMemOK			= UMEM_OK_READY;

	tMemDataOut		= UV128_XX;

	tRamStBlkData	= UV128_XX;
	tRamStBlkIx		= UV9_XX;
	tRamDoSt		= 0;
	
//	if(tAddrIsRom && tMemOpm[3])
	if(tAddrIsRom && (tMemOpm[4:3]!=2'b00))
	begin
//		$display("L2 ROM");

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
//		tMemOK			= UMEM_OK_OK;
		tMemOK			= (tRamBlkIxL==tRamBlkIx) ? UMEM_OK_OK : UMEM_OK_HOLD;
		
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
	
	tRomBlkData	<= romTileData[tRomBlkIx];
	tRamBlkData	<= ramTileData[tRamBlkIx];
	
	if(tRamDoSt)
	begin
		ramTileData[tRamStBlkIx]	<= tRamStBlkData;
	end
end

endmodule

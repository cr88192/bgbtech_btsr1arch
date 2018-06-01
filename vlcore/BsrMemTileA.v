/*

Transfers are in terms of 256 bit tiles, which map to 128 bits logical.
Each tile will contain part of the following tile.

Memory Map:
0000..7FFF: ROM
C000..DFFF: SRAM
E000..FFFF: MMIO (Bypass)

 */

`include "BsrCoreDefs.v"

module BsrMemTile(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInData,
	regOutData,		regOutOK,
	regInOpm,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset

input[19:0]		regInAddr;		//input PC address
input[255:0]	regInData;		//input data (store)
input[4:0]		regInOpm;		//Operation Size/Type

output[255:0]	regOutData;		//output data (load)
output[1:0]		regOutOK;		//set if operation suceeds

input[127:0]	memInData;	//memory PC data
output[127:0]	memOutData;	//memory PC data
output[31:0]	memAddr;	//memory PC address
output[4:0]		memOpm;		//memory PC operation mode
input[1:0]		memOK;		//memory PC OK

reg[127:0]		tMemOutData;	//memory PC data
reg[31:0]		tMemAddr;		//memory PC address
reg[4:0]		tMemOpm;		//memory PC output-enable

assign			memOutData	= tMemOutData;
assign			memAddr		= tMemAddr;
assign			memOpm		= tMemOpm;

input[31:0]		mmioInData;		//mmio data in
output[31:0]	mmioOutData;	//mmio data out
output[31:0]	mmioAddr;		//mmio address
output[4:0]		mmioOpm;		//mmio read
input[1:0]		mmioOK;			//mmio OK

reg[31:0]		tMmioOutData;	//mmio data out
reg[31:0]		tMmioAddr;		//mmio address
reg[4:0]		tMmioOpm;		//mmio read

assign			mmioOutData	= tMmioOutData;
assign			mmioAddr	= tMmioAddr;
assign			mmioOpm		= tMmioOpm;


// (* ram_style="block" *) reg[127:0]		memTile[2047:0];	//memory
// (* ram_style="block" *) reg[31:0]		memTileA[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileB[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileC[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileD[0:4095];	//memory

(* ram_style="block" *) reg[31:0]		romTileA[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileB[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileC[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileD[1023:0];		//ROM

(* ram_style="block" *) reg[31:0]		romTileE[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileF[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileG[1023:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileH[1023:0];		//ROM

(* ram_style="block" *) reg[31:0]		sramTileA[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileB[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileC[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileD[255:0];		//SRAM

(* ram_style="block" *) reg[31:0]		sramTileE[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileF[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileG[255:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileH[255:0];		//SRAM

reg[255:0]		tRomTile;
reg[255:0]		tRamTile;
reg[255:0]		tSRamTile;
reg[11:0]		tAccTileIx;


reg[255:0]		tMemTile;
reg[255:0]		tOutData;

reg[255:0]		tNextTile;
reg[255:0]		tNextTileA;

reg[11:0]		tRegTileIxA;
reg[11:0]		tRegTileIxB;

reg[11:0]		tNextTileIx;
reg				tNextTileSt;
reg				tNextTileSrSt;
reg[1:0]		tRegOutOK;

wire		regInOE;
wire		regInWR;
assign		regInOE = regInOpm[3];
assign		regInWR = regInOpm[4];

wire		addrIsSRam;
assign		addrIsSRam =
	(regInAddr[19:0] >= 20'h0C000) &&
	(regInAddr[19:0] <  20'h0E000) ;

wire		addrIsRom;
assign		addrIsRom =
	(regInAddr[19:0] <= 20'h08000) &&
	(regInAddr[14] == 0);

//wire			addrIsSRam;
//assign		addrIsSRam =
//	(regInAddr[28:0] <= 29'h0010_0000) &&
//	(regInAddr[14] == 1);

wire		addrIsRam;
assign		addrIsRam = 0;

assign		regOutData = tOutData;
assign		regOutOK = tRegOutOK;

initial begin
	$readmemh("bootrom_1a.txt", romTileA);
	$readmemh("bootrom_1b.txt", romTileB);
	$readmemh("bootrom_1c.txt", romTileC);
	$readmemh("bootrom_1d.txt", romTileD);
	$readmemh("bootrom_1e.txt", romTileE);
	$readmemh("bootrom_1f.txt", romTileF);
	$readmemh("bootrom_1g.txt", romTileG);
	$readmemh("bootrom_1h.txt", romTileH);
end

always @*
begin
	tMemTile		= 0;
	tOutData		= 0;
	tNextTile		= 0;
	
	if(regInAddr[4])
	begin
		tRegTileIxB		= regInAddr[15:4];
		tRegTileIxA		= tRegTileIxB+1;
	end
	else
	begin
		tRegTileIxA		= regInAddr[15:4];
		tRegTileIxB		= tRegTileIxA;
	end

	tNextTileIx		= tRegTileIx;
	tNextTileSt		= 0;
	tNextTileSrSt	= 0;
	tRegOutOK		= 0;

	tMemOutData		= 0;	//memory PC data
	tMemAddr		= 0;	//memory PC address
	tMemOE			= 0;	//memory PC output-enable
	tMemWR			= 0;	//memory PC output-enable

	tMmioOutData	= 0;	//mmio data out
	tMmioAddr		= 0;	//mmio address
	tMmioOE			= 0;	//mmio read
	tMmioWR			= 0;	//mmio write

	if(regInOE || regInWR)
	begin

		$display("DcTile2 %X %d %d", regInAddr, addrIsRom, addrIsRam);

		if(addrIsRom)
		begin
			if(regInAddr[4])
			begin
				tMemTile[127:  0] = tRomTile[255:128];
				tMemTile[255:128] = tRomTile[127:  0];
			end
			else
			begin
				tMemTile[127:  0] = tRomTile[127:  0];
				tMemTile[255:128] = tRomTile[255:128];
			end

			tNextTile = tRomTile;
			tRegOutOK = (tAccTileIx == tRegTileIx) ?
				UMEM_OK_OK : UMEM_OK_HOLD;
			
			$display("Rom: %X", tMemTile);

			case(regInOp[1:0])
				2'b00:
					tOutData=tMemTile;
				2'b01:
					tOutData=tMemTile;
				2'b10: case(regInAddr[3:2])
					2'b00: tOutData={96'h0, tMemTile[ 31: 0]};
					2'b01: tOutData={96'h0, tMemTile[ 63:32]};
					2'b10: tOutData={96'h0, tMemTile[ 95:64]};
					2'b11: tOutData={96'h0, tMemTile[127:96]};
				endcase
				2'b11: begin
					if(regInAddr[3])
						tOutData={64'h0, tMemTile[127:64]};
					else
						tOutData={64'h0, tMemTile[ 63: 0]};
				end
			endcase

			$display("Rom: Out=%X", tOutData);
		end
		else
		if(addrIsRam || addrIsSRam)
		begin
			if(regInAddr[4])
			begin
				tMemTile[127:  0] = tSRamTile[255:128];
				tMemTile[255:128] = tSRamTile[127:  0];
			end
			else
			begin
				tMemTile[127:  0] = tSRamTile[127:  0];
				tMemTile[255:128] = tSRamTile[255:128];
			end
			tNextTile = tSRamTile;
			tNextTileA = tMemTile;

//			tMemTile = addrIsSRam ? tSRamTile : tRamTile;
//			tNextTile = tMemTile;
//			tRegOutOK = 1;
			tRegOutOK = (tAccTileIx == tRegTileIx) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			case(regInOpm[2:0])
				3'b111:
					tOutData=tMemTile;
				3'b010: case(regInAddr[3:2])
					2'b00: tOutData={128'h0, tMemTile[ 31: 0]};
					2'b01: tOutData={128'h0, tMemTile[ 63:32]};
					2'b10: tOutData={128'h0, tMemTile[ 95:64]};
					2'b11: tOutData={128'h0, tMemTile[127:96]};
				endcase
				default:
					tOutData=tMemTile;
			endcase
		
			if(regInWR)
			begin
				tNextTileIx = tRegTileIx;
//				tNextTileSt = 1;
				tNextTileSt = addrIsRam;
				tNextTileSrSt = addrIsSRam;
				case(regInOp[1:0])
					3'b111:
						tNextTileA[159:0]=regInData;
					3'b010: case(regInAddr[3:2])
						2'b00: tNextTileA[ 31: 0] = regInData[31:0];
						2'b01: tNextTileA[ 63:32] = regInData[31:0];
						2'b10: tNextTileA[ 95:64] = regInData[31:0];
						2'b11: tNextTileA[127:96] = regInData[31:0];
					endcase
					default:
						tNextTileA[159:0]=regInData;
				endcase

				if(regInAddr[4])
				begin
					tNextTile[127:  0] = tNextTileA[255:128];
					tNextTile[255:128] = tNextTileA[127:  0];
				end
				else
				begin
					tNextTile[127:  0] = tNextTileA[127:  0];
					tNextTile[255:128] = tNextTileA[255:128];
				end
			end
		end
		else
		begin
			tMmioOutData	= regInData[31:0];
			tMmioAddr		= regInAddr;
			tMmioOE			= regInOE;
			tMmioWR			= regInWR;
			tRegOutOK		= mmioOK;
			tOutData		= { 96'h0, mmioInData[31:0] };
		end

	end
end

always @ (posedge clock)
begin
//	tRomTile <= romTile[tRegTileIx[7:0]];
//	tRamTile <= memTile[tRegTileIx];

	tRomTile[ 31:  0] <= romTileA[tRegTileIxA[10:1]];
	tRomTile[ 63: 32] <= romTileB[tRegTileIxA[10:1]];
	tRomTile[ 95: 64] <= romTileC[tRegTileIxA[10:1]];
	tRomTile[127: 96] <= romTileD[tRegTileIxA[10:1]];
	tRomTile[159:128] <= romTileE[tRegTileIxB[10:1]];
	tRomTile[191:160] <= romTileF[tRegTileIxB[10:1]];
	tRomTile[223:192] <= romTileG[tRegTileIxB[10:1]];
	tRomTile[255:224] <= romTileH[tRegTileIxB[10:1]];

	tSRamTile[ 31:  0] <= sramTileA[tRegTileIxA[8:1]];
	tSRamTile[ 63: 32] <= sramTileB[tRegTileIxA[8:1]];
	tSRamTile[ 95: 64] <= sramTileC[tRegTileIxA[8:1]];
	tSRamTile[127: 96] <= sramTileD[tRegTileIxA[8:1]];
	tSRamTile[159:128] <= sramTileE[tRegTileIxB[8:1]];
	tSRamTile[191:160] <= sramTileF[tRegTileIxB[8:1]];
	tSRamTile[223:192] <= sramTileG[tRegTileIxB[8:1]];
	tSRamTile[255:224] <= sramTileH[tRegTileIxB[8:1]];

//	tRamTile[ 31: 0] <= memTileA[tRegTileIx];
//	tRamTile[ 63:32] <= memTileB[tRegTileIx];
//	tRamTile[ 95:64] <= memTileC[tRegTileIx];
//	tRamTile[127:96] <= memTileD[tRegTileIx];

	tAccTileIx <= tRegTileIx;

//	if(tNextTileSt)
//	begin
//		memTile[tNextTileIx] <= tNextTile;
//		memTileA[tNextTileIx] <= tNextTile[ 31: 0];
//		memTileB[tNextTileIx] <= tNextTile[ 63:32];
//		memTileC[tNextTileIx] <= tNextTile[ 95:64];
//		memTileD[tNextTileIx] <= tNextTile[127:96];
//	end

	if(tNextTileSrSt)
	begin
		sramTileA[tRegTileIxA[8:1]] <= tNextTile[ 31:  0];
		sramTileB[tRegTileIxA[8:1]] <= tNextTile[ 63: 32];
		sramTileC[tRegTileIxA[8:1]] <= tNextTile[ 95: 64];
		sramTileD[tRegTileIxA[8:1]] <= tNextTile[127: 96];
		sramTileE[tRegTileIxB[8:1]] <= tNextTile[159:128];
		sramTileF[tRegTileIxB[8:1]] <= tNextTile[191:160];
		sramTileG[tRegTileIxB[8:1]] <= tNextTile[223:192];
		sramTileH[tRegTileIxB[8:1]] <= tNextTile[255:224];
	end
end


endmodule

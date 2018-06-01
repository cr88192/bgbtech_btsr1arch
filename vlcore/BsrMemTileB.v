/*

Transfers are in terms of 128 bit tiles, with a 64-bit alignment.

Memory Map:
0000..7FFF: ROM
C000..DFFF: SRAM
E000..FFFF: MMIO (Bypass)

 */

`include "BsrCoreDefs.v"

module BsrMemTileB(
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

/* verilator lint_off UNOPTFLAT */

input[19:0]		regInAddr;		//input PC address
input[127:0]	regInData;		//input data (store)
input[4:0]		regInOpm;		//Operation Size/Type

output[127:0]	regOutData;		//output data (load)
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

reg[31:0]		tmpMmioInData;		//mmio data in
reg[1:0]		tmpMmioOK;			//mmio OK

// / * verilator lint_on UNOPTFLAT * /

// (* ram_style="block" *) reg[127:0]		memTile[2047:0];	//memory
// (* ram_style="block" *) reg[31:0]		memTileA[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileB[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileC[0:4095];	//memory
// (* ram_style="block" *) reg[31:0]		memTileD[0:4095];	//memory

(* ram_style="block" *) reg[31:0]		romTileA[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileB[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileC[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileD[2047:0];		//ROM

(* ram_style="block" *) reg[31:0]		sramTileA[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileB[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileC[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileD[511:0];		//SRAM

//(* ram_style="block" *) reg[31:0]		sramTileA[255:0];		//SRAM
//(* ram_style="block" *) reg[31:0]		sramTileB[255:0];		//SRAM
//(* ram_style="block" *) reg[31:0]		sramTileC[255:0];		//SRAM
//(* ram_style="block" *) reg[31:0]		sramTileD[255:0];		//SRAM

reg[127:0]		tRomTile;
reg[127:0]		tRamTile;
reg[127:0]		tSRamTile;
reg[11:0]		tAccTileIxA;
reg[11:0]		tAccTileIxB;

reg[127:0]		tMemTile;
reg[127:0]		tOutData;

reg[127:0]		tNextTile;
reg[127:0]		tNextTileA;

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

wire		addrIsMmio;
assign		addrIsMmio =
	(regInAddr[19:0] >= 20'h0E000) &&
	(regInAddr[19:0] <  20'h0F800) ;

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
end

reg		mmioBypass;

always @*
begin
	tMemTile		= 0;
	tOutData		= 0;
	tNextTile		= 0;
	
	if(regInAddr[3])
	begin
		tRegTileIxB		= regInAddr[14:3];
		tRegTileIxA		= tRegTileIxB+1;
	end else begin
		tRegTileIxA		= regInAddr[14:3];
		tRegTileIxB		= tRegTileIxA;
	end

//	tNextTileIx		= tRegTileIx;
	tNextTileSt		= 0;
	tNextTileSrSt	= 0;
	tRegOutOK		= 0;

	tMemOutData		= 0;	//memory PC data
	tMemAddr		= 0;	//memory PC address
//	tMemOE			= 0;	//memory PC output-enable
//	tMemWR			= 0;	//memory PC output-enable

//	tMmioOutData	= 0;	//mmio data out
//	tMmioAddr		= 0;	//mmio address
//	tMmioOE			= 0;	//mmio read
//	tMmioWR			= 0;	//mmio write

	tMemOpm			= UMEM_OPM_READY;
//	tMmioOpm		= UMEM_OPM_READY;

	mmioBypass		= 0;

	if(regInOE || regInWR)
	begin

//		$display("DcTile2 %X %d %d", regInAddr, addrIsRom, addrIsRam);

		if(addrIsRom)
		begin
			if(regInAddr[3])
			begin
				tMemTile[ 63:  0] = tRomTile[127: 64];
				tMemTile[127: 64] = tRomTile[ 63:  0];
			end else begin
				tMemTile[ 63:  0] = tRomTile[ 63:  0];
				tMemTile[127: 64] = tRomTile[127: 64];
			end

			tNextTile = tRomTile;
			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;
			
//			$display("Rom: %X", tMemTile);

			case(regInOpm[1:0])
				2'b10: begin
					tOutData=tMemTile;
					if(regInAddr[2])
						tOutData={
							tMemTile[ 95:64], tMemTile[127:96],
							tMemTile[ 31: 0], tMemTile[ 63:32]};
				end
				default:
					tOutData=tMemTile;
			endcase

//			$display("Rom: Out=%X", tOutData);
		end
		else
		if(addrIsRam || addrIsSRam)
		begin
			tSRamTile[ 31:  0] = sramTileA[tAccTileIxA[9:1]];
			tSRamTile[ 63: 32] = sramTileB[tAccTileIxA[9:1]];
			tSRamTile[ 95: 64] = sramTileC[tAccTileIxB[9:1]];
			tSRamTile[127: 96] = sramTileD[tAccTileIxB[9:1]];

//			tSRamTile[ 31:  0] = sramTileA[tAccTileIxA[8:1]];
//			tSRamTile[ 63: 32] = sramTileB[tAccTileIxA[8:1]];
//			tSRamTile[ 95: 64] = sramTileC[tAccTileIxB[8:1]];
//			tSRamTile[127: 96] = sramTileD[tAccTileIxB[8:1]];

			if(regInAddr[3])
			begin
				tMemTile[ 63:  0] = tSRamTile[127: 64];
				tMemTile[127: 64] = tSRamTile[ 63:  0];
			end else begin
				tMemTile[ 63:  0] = tSRamTile[ 63:  0];
				tMemTile[127: 64] = tSRamTile[127: 64];
			end

			tNextTile = tSRamTile;
			tNextTileA = tMemTile;

//			tMemTile = addrIsSRam ? tSRamTile : tRamTile;
//			tNextTile = tMemTile;
//			tRegOutOK = 1;
			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;

/*
			case(regInOpm[1:0])
				2'b10: begin
					tOutData=tMemTile;
					if(regInAddr[2])
						tOutData={
							tMemTile[ 95:64], tMemTile[127:96],
							tMemTile[ 31: 0], tMemTile[ 63:32]};
				end
				default:
					tOutData=tMemTile;
			endcase
*/

			if(regInOE)
			begin
//				$display("SRam: A=%X Out=%X", regInAddr, tOutData);
			end
		
			if(regInWR)
			begin
//				$display("BsrMemTile: Store A=%X D=%X",
//					regInAddr, regInData);

//				tNextTileIx = tRegTileIx;
//				tNextTileSt = 1;
				tNextTileSt = addrIsRam;
				tNextTileSrSt = addrIsSRam;

/*
				case(regInOpm[1:0])
					2'b10:
						if(regInAddr[2])
							tNextTileA[ 63:32] = regInData[31:0];
						else
							tNextTileA[ 31: 0] = regInData[31:0];
					default:
						tNextTileA[127:0]=regInData;
				endcase
*/

				tNextTileA[127:0]=regInData;

				if(regInAddr[3])
				begin
					tNextTile[ 63: 0] = tNextTileA[127:64];
					tNextTile[127:64] = tNextTileA[ 63: 0];
				end else begin
					tNextTile[ 63: 0] = tNextTileA[ 63: 0];
					tNextTile[127:64] = tNextTileA[127:64];
				end

//				sramTileA[tRegTileIxA[9:1]] = tNextTile[ 31:  0];
//				sramTileB[tRegTileIxA[9:1]] = tNextTile[ 63: 32];
//				sramTileC[tRegTileIxB[9:1]] = tNextTile[ 95: 64];
//				sramTileD[tRegTileIxB[9:1]] = tNextTile[127: 96];
			end
		end
		else
		begin
			mmioBypass		= 1;

/*
			tMmioOutData	= regInData[31:0];
//			tMmioAddr		= regInAddr;
			tMmioAddr		= { 12'h0, regInAddr };
			tMmioOpm		= regInOpm;
			tRegOutOK		= mmioOK;
			tOutData		= { 96'h0, mmioInData[31:0] };
*/

			tRegOutOK		= tmpMmioOK;
			tOutData		= { 96'h0, tmpMmioInData[31:0] };

			if((tMmioOpm!=0) && (tmpMmioOK!=3))
			begin
//				$display("DcTile2 Bypass A=%X O=%X DO=%X DI=%X",
//					regInAddr, tMmioOpm, regInData[31:0], tOutData[31:0]);
			end
		end

	end
end

always @ (posedge clock)
begin
//	tRomTile <= romTile[tRegTileIx[7:0]];
//	tRamTile <= memTile[tRegTileIx];

	if(mmioBypass)
	begin
		tMmioOutData	<= regInData[31:0];
		tMmioAddr		<= { 12'h0, regInAddr };
		tMmioOpm		<= regInOpm;
		tmpMmioOK		<= mmioOK;
		tmpMmioInData	<= mmioInData;
	end
	else
	begin
		tMmioOutData	<= 0;
		tMmioAddr		<= 0;
		tMmioOpm		<= UMEM_OPM_READY;
		tmpMmioOK		<= UMEM_OK_READY;
		tmpMmioInData	<= 0;
	end

	tRomTile[ 31:  0] <= romTileA[tRegTileIxA[11:1]];
	tRomTile[ 63: 32] <= romTileB[tRegTileIxA[11:1]];
	tRomTile[ 95: 64] <= romTileC[tRegTileIxB[11:1]];
	tRomTile[127: 96] <= romTileD[tRegTileIxB[11:1]];

//	tSRamTile[ 31:  0] <= sramTileA[tRegTileIxA[9:1]];
//	tSRamTile[ 63: 32] <= sramTileB[tRegTileIxA[9:1]];
//	tSRamTile[ 95: 64] <= sramTileC[tRegTileIxB[9:1]];
//	tSRamTile[127: 96] <= sramTileD[tRegTileIxB[9:1]];

//	tRamTile[ 31: 0] <= memTileA[tRegTileIx];
//	tRamTile[ 63:32] <= memTileB[tRegTileIx];
//	tRamTile[ 95:64] <= memTileC[tRegTileIx];
//	tRamTile[127:96] <= memTileD[tRegTileIx];

	tAccTileIxA		<= tRegTileIxA;
	tAccTileIxB		<= tRegTileIxB;

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
//		$display("BsrMemTile: Store Tile A=%X D=%X",
//			regInAddr, tNextTile);
	
		sramTileA[tRegTileIxA[9:1]] <= tNextTile[ 31:  0];
		sramTileB[tRegTileIxA[9:1]] <= tNextTile[ 63: 32];
		sramTileC[tRegTileIxB[9:1]] <= tNextTile[ 95: 64];
		sramTileD[tRegTileIxB[9:1]] <= tNextTile[127: 96];

//		sramTileA[tRegTileIxA[8:1]] <= tNextTile[ 31:  0];
//		sramTileB[tRegTileIxA[8:1]] <= tNextTile[ 63: 32];
//		sramTileC[tRegTileIxB[8:1]] <= tNextTile[ 95: 64];
//		sramTileD[tRegTileIxB[8:1]] <= tNextTile[127: 96];
	end
end


endmodule

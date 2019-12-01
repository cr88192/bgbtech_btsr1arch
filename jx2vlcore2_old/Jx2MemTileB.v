/*

Transfers are in terms of 128 bit tiles, with a 64-bit alignment.

Memory Map:
0000..7FFF: ROM
C000..DFFF: SRAM
E000..FFFF: MMIO (Bypass)

 */

`include "Jx2CoreDefs.v"

module Jx2MemTileB(
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

input[ 47:0]	regInAddr;		//input PC address
input[127:0]	regInData;		//input data (store)
input[4:0]		regInOpm;		//Operation Size/Type

output[127:0]	regOutData;		//output data (load)
output[1:0]		regOutOK;		//set if operation suceeds

input [127:0]	memInData;	//memory PC data
output[127:0]	memOutData;	//memory PC data
output[ 31:0]	memAddr;	//memory PC address
output[  4:0]	memOpm;		//memory PC operation mode
input [  1:0]	memOK;		//memory PC OK

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

// (* ram_style="block" *) reg[31:0]	memTileA[0:4095];		//memory
// (* ram_style="block" *) reg[31:0]	memTileB[0:4095];		//memory
// (* ram_style="block" *) reg[31:0]	memTileC[0:4095];		//memory
// (* ram_style="block" *) reg[31:0]	memTileD[0:4095];		//memory
// (* ram_style="block" *) reg[31:0]	memTileAdA[0:4095];		//memory address
// (* ram_style="block" *) reg[31:0]	memTileAdB[0:4095];		//memory address

(* ram_style="block" *) reg[31:0]		ramTileA[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileB[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileC[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileD[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileE[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileF[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileG[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileH[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileAdA[2047:0];	//RAM Address
(* ram_style="block" *) reg[31:0]		ramTileAdB[2047:0];	//RAM Address

(* ram_style="block" *) reg[31:0]		romTileA[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileB[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileC[2047:0];		//ROM
(* ram_style="block" *) reg[31:0]		romTileD[2047:0];		//ROM

(* ram_style="block" *) reg[31:0]		sramTileA[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileB[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileC[511:0];		//SRAM
(* ram_style="block" *) reg[31:0]		sramTileD[511:0];		//SRAM

reg[127:0]		tRomTile;
reg[127:0]		tSRamTile;
reg[11:0]		tAccTileIxA;
reg[11:0]		tAccTileIxB;

reg[127:0]		tMemTile;
reg[127:0]		tOutData;

reg[127:0]		tNextTile;
reg[127:0]		tNextTileA;
reg[127:0]		tNextTileB;

reg[127:0]		tRamTileA;
reg[127:0]		tRamTileB;
reg[31:0]		tRamAdA;
reg[31:0]		tRamAdB;

reg[11:0]		tRegTileIxA;
reg[11:0]		tRegTileIxB;

reg[10:0]		tRamTileIxA;
reg[10:0]		tRamTileIxB;

reg[11:0]		tNextTileIxA;
reg[11:0]		tNextTileIxB;
reg				tNextTileSrSt;

reg[10:0]		tNextRamTileIxA;
reg[10:0]		tNextRamTileIxB;
reg				tNextRamTileStA;
reg				tNextRamTileStB;

reg[1:0]		tRegOutOK;

wire		regInOE;
wire		regInWR;
assign		regInOE = regInOpm[3];
assign		regInWR = regInOpm[4];

wire		addrIsSRam;
assign		addrIsSRam =
	(regInAddr[31:16] == 16'h0000) &&
	(regInAddr[15:0] >= 16'hC000) &&
	(regInAddr[15:0] <  16'hE000) ;

wire		addrIsRom;
assign		addrIsRom =
	(regInAddr[31:16] == 16'h0000) &&
	(regInAddr[15: 0] <  16'h8000);

// wire		addrIsMmio;
// assign		addrIsMmio =
//	((regInAddr[31:16] == 16'h0000) &&
//	(regInAddr[15:0] >= 16'hE000) &&
//	(regInAddr[15:0] <  16'hF800)) ||
//	 (regInAddr[47:44]==4'hE);

//wire			addrIsSRam;
//assign		addrIsSRam =
//	(regInAddr[28:0] <= 29'h0010_0000) &&
//	(regInAddr[14] == 1);

wire		addrIsRam;
//assign		addrIsRam = 0;
assign		addrIsRam =
	(regInAddr[31:0] >= 32'h01000000) &&
	(regInAddr[31:0] <  32'h7FFFFFFF);

reg		addrIsLow4GB;
reg		addrIsMmio;

reg[27:0]	tBlkAddrA;
reg[27:0]	tBlkAddrB;
reg			tBlkMissA;
reg			tBlkMissB;
reg			tBlkMiss;

reg[27:0]	tRamBlkAddrA;
reg[27:0]	tRamBlkAddrB;
reg[3:0]	tRamBlkFlagA;
reg[3:0]	tRamBlkFlagB;

reg[27:0]	tNextBlkAddrA;
reg[27:0]	tNextBlkAddrB;
reg[3:0]	tNextBlkFlagA;
reg[3:0]	tNextBlkFlagB;

assign		regOutData = tOutData;
assign		regOutOK = tRegOutOK;

initial begin
	$readmemh("bootrom_1a.txt", romTileA);
	$readmemh("bootrom_1b.txt", romTileB);
	$readmemh("bootrom_1c.txt", romTileC);
	$readmemh("bootrom_1d.txt", romTileD);
end

reg		mmioBypass;
reg		tRamLaneA;
reg		tRamLaneB;

always @*
begin
	tMemTile		= 0;
	tOutData		= 0;
	tNextTile		= 0;

	addrIsLow4GB	= (regInAddr[47:32]==16'h0000);
	addrIsMmio		= (regInAddr[47:45]==3'b101) ||
				((regInAddr[31:29]==3'b101) && addrIsLow4GB);

	if(regInAddr[3])
	begin
		tRegTileIxB		= regInAddr[14:3];
		tRegTileIxA		= tRegTileIxB+1;
//		tBlkAddrB		= regInAddr[31:4];
//		tBlkAddrA		= tBlkAddrB + 1;

		tRamTileIxA		= regInAddr[14:4];
		tRamTileIxB		= tRamTileIxA+1;
		tBlkAddrA		= regInAddr[31:4];
		tBlkAddrB		= tBlkAddrA + 1;
		tRamLaneB		= 1;
	end else begin
		tRegTileIxA		= regInAddr[14:3];
		tRegTileIxB		= tRegTileIxA;

		tRamTileIxA		= regInAddr[14:4];
		tRamTileIxB		= tRamTileIxA;
		tBlkAddrA		= regInAddr[31:4];
		tBlkAddrB		= tBlkAddrA + 1;
		tRamLaneB		= 0;
	end

	case(regInAddr[4:3])
		2'b00: begin
			tRamTileIxA		= regInAddr[14:4];
			tRamTileIxB		= tRamTileIxA + 1;
			tBlkAddrA		= regInAddr[31:4];
			tBlkAddrB		= tBlkAddrA + 1;
			tRamLaneA		= 1;
			tRamLaneB		= 0;
		end
		2'b01: begin
			tRamTileIxA		= regInAddr[14:4];
			tRamTileIxB		= tRamTileIxA + 1;
			tBlkAddrA		= regInAddr[31:4];
			tBlkAddrB		= tBlkAddrA + 1;
			tRamLaneA		= 1;
			tRamLaneB		= 1;
		end
		2'b10: begin
			tRamTileIxB		= regInAddr[14:4];
			tRamTileIxA		= tRamTileIxB + 1;
			tBlkAddrB		= regInAddr[31:4];
			tBlkAddrA		= tBlkAddrB + 1;
			tRamLaneA		= 0;
			tRamLaneB		= 1;
		end
		2'b11: begin
			tRamTileIxB		= regInAddr[14:4];
			tRamTileIxA		= tRamTileIxB + 1;
			tBlkAddrB		= regInAddr[31:4];
			tBlkAddrA		= tBlkAddrB + 1;
			tRamLaneA		= 1;
			tRamLaneB		= 1;
		end
	endcase

//	tRamTileIxA		= regInAddr[14:4];
//	tRamTileIxB		= tRamTileIxA+1;
//	tBlkAddrA		= regInAddr[31:4];
//	tBlkAddrB		= tBlkAddrA + 1;

//	tNextTileIx		= tRegTileIx;
	tNextRamTileStA	= 0;
	tNextRamTileStB	= 0;
	tNextTileSrSt	= 0;
	tRegOutOK		= 0;

//	tMemOutData		= 0;	//memory PC data
//	tMemAddr		= 0;	//memory PC address
//	tMemOE			= 0;	//memory PC output-enable
//	tMemWR			= 0;	//memory PC output-enable

//	tMmioOutData	= 0;	//mmio data out
//	tMmioAddr		= 0;	//mmio address
//	tMmioOE			= 0;	//mmio read
//	tMmioWR			= 0;	//mmio write

//	tMemOpm			= UMEM_OPM_READY;
//	tMmioOpm		= UMEM_OPM_READY;

	mmioBypass		= 0;

	if(regInOE || regInWR)
	begin

`ifndef JX2_QUIET
		$display("DcTile2 A=%X ROM=%d RAM=%d SRAM=%d",
			regInAddr, addrIsRom, addrIsRam, addrIsSRam);
`endif

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

//			tNextTile = tRomTile;
			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;

//			$display("Rom: Out=%X", tOutData);
		end else
			if(addrIsSRam)
		begin
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

			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;

			if(regInOE)
			begin
//				$display("SRam: A=%X Out=%X", regInAddr, tOutData);
			end
		end else
			if(addrIsRam)
		begin
/*
			if(regInAddr[3])
			begin
//				tMemTile[ 63:  0] = tRamTile[127: 64];
//				tMemTile[127: 64] = tRamTile[ 63:  0];
				tMemTile[ 63:  0] = tRamTileA[127: 64];
				tMemTile[127: 64] = tRamTileB[ 63:  0];
			end else begin
				tMemTile[ 63:  0] = tRamTileA[ 63:  0];
				tMemTile[127: 64] = tRamTileA[127: 64];
			end
*/

			case(regInAddr[4:3])
				2'b00: begin
					tMemTile[ 63:  0] = tRamTileA[ 63:  0];
					tMemTile[127: 64] = tRamTileA[127: 64];
				end
				2'b01: begin
					tMemTile[ 63:  0] = tRamTileA[127: 64];
					tMemTile[127: 64] = tRamTileB[ 63:  0];
				end
				2'b10: begin
					tMemTile[ 63:  0] = tRamTileB[ 63:  0];
					tMemTile[127: 64] = tRamTileB[127: 64];
				end
				2'b11: begin
					tMemTile[ 63:  0] = tRamTileB[127: 64];
					tMemTile[127: 64] = tRamTileA[ 63:  0];
				end
			endcase

			tBlkMissA	= (tRamBlkAddrA != tBlkAddrA) && tRamLaneA;
			tBlkMissB	= (tRamBlkAddrB != tBlkAddrB) && tRamLaneB;
			tBlkMiss	= tBlkMissA || tBlkMissB;

			tRegOutOK	= ((tAccTileIxA == tRegTileIxA) && !tBlkMiss) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;

			if(regInOE)
			begin
//				$display("Ram: A=%X Out=%X", regInAddr, tOutData);

				if(tBlkMissA || tBlkMissB)
				begin
					$display("MemTile: MissA=%d MissB=%d",
						tBlkMissA, tBlkMissB);
					$display("MemTile: MissA: %X->%X",
						tRamBlkAddrA, tBlkAddrA);
					$display("MemTile: MissB: %X->%X",
						tRamBlkAddrB, tBlkAddrB);
				end

			end
		end
		else
		begin
			mmioBypass		= 1;

			tRegOutOK		= tmpMmioOK;
			tOutData		= { 96'h0, tmpMmioInData[31:0] };

			if((tMmioOpm!=0) && (tmpMmioOK!=3))
			begin
//				$display("DcTile2 Bypass A=%X O=%X DO=%X DI=%X",
//					regInAddr, tMmioOpm, regInData[31:0], tOutData[31:0]);
			end
		end

		if(addrIsSRam)
		begin
			if(regInWR)
			begin
//				$display("Jx2MemTile: Store A=%X D=%X",
//					regInAddr, regInData);

				tNextTileIxA = tRegTileIxA;
				tNextTileIxB = tRegTileIxB;
				tNextTileSrSt = 1;

				if(regInAddr[3])
				begin
					tNextTile[ 63: 0] = regInData[127:64];
					tNextTile[127:64] = regInData[ 63: 0];
				end else begin
					tNextTile[ 63: 0] = regInData[ 63: 0];
					tNextTile[127:64] = regInData[127:64];
				end
			end
		end

		if(addrIsRam)
		begin
			if(tRamStickyA || tRamStickyB)
			begin
				tNextTileA		= tRamTileA;
				tNextTileB		= tRamTileB;
				tNextRamTileIxA = tRamTileIxA;
				tNextRamTileIxB = tRamTileIxB;
				tNextBlkAddrA = tRamBlkAddrA;
				tNextBlkAddrB = tRamBlkAddrB;
				tNextBlkFlagA = 1;
				tNextBlkFlagB = 1;

				tNextRamTileStA = tRamStickyA;
				tNextRamTileStB = tRamStickyB;
			end
			else
				if(regInWR && !tBlkMiss)
			begin
//				$display("Jx2MemTile: Store A=%X D=%X",
//					regInAddr, regInData);

				tNextRamTileIxA = tRamTileIxA;
				tNextRamTileIxB = tRamTileIxB;
				tNextBlkAddrA = tBlkAddrA;
				tNextBlkAddrB = tBlkAddrB;
				tNextBlkFlagA = 1;
				tNextBlkFlagB = 1;

				tNextRamTileStA = tRamLaneA;
				tNextRamTileStB = tRamLaneB;

/*
				if(regInAddr[3])
				begin
					tNextTileA[ 63: 0] = tRamTileA[ 63: 0];
					tNextTileA[127:64] = regInData[ 63: 0];
					tNextTileB[ 63: 0] = regInData[127:64];
					tNextTileB[127:64] = tRamTileB[127:64];

				end else begin
					tNextTileA[127: 0] = regInData[127: 0];
				end
*/

				case(regInAddr[4:3])
					2'b00: begin
						tNextTileA[ 63:  0] = regInData[ 63:  0];
						tNextTileA[127: 64] = regInData[127: 64];
					end
					2'b01: begin
						tNextTileA[ 63: 0] = tRamTileA[ 63: 0];
						tNextTileA[127:64] = regInData[ 63: 0];
						tNextTileB[ 63: 0] = regInData[127:64];
						tNextTileB[127:64] = tRamTileB[127:64];
					end
					2'b10: begin
						tNextTileB[ 63:  0] = regInData[ 63:  0];
						tNextTileB[127: 64] = regInData[127: 64];
					end
					2'b11: begin
						tNextTileB[ 63: 0] = tRamTileB[ 63: 0];
						tNextTileB[127:64] = regInData[ 63: 0];
						tNextTileA[ 63: 0] = regInData[127:64];
						tNextTileA[127:64] = tRamTileA[127:64];
					end
				endcase

			end
		end

	end
end

reg		tMissDoneFlagA;
reg		tMissDoneFlagB;
reg		tMissReqFlagA;
reg		tMissReqFlagB;
reg		tRamStickyA;
reg		tRamStickyB;

always @ (posedge clock)
begin
//	tRomTile <= romTile[tRegTileIx[7:0]];
//	tRamTile <= memTile[tRegTileIx];

	tMemOutData		<= 0;	//memory PC data
	tMemAddr		<= 0;	//memory PC address
	tMemOpm			<= UMEM_OPM_READY;

	if(mmioBypass)
	begin
		tMmioOutData	<= regInData[31:0];
		tMmioAddr		<= regInAddr[31:0];
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

	tRomTile[ 31:  0]	<= romTileA[tRegTileIxA[11:1]];
	tRomTile[ 63: 32]	<= romTileB[tRegTileIxA[11:1]];
	tRomTile[ 95: 64]	<= romTileC[tRegTileIxB[11:1]];
	tRomTile[127: 96]	<= romTileD[tRegTileIxB[11:1]];

	tSRamTile[ 31:  0]	<= sramTileA[tRegTileIxA[9:1]];
	tSRamTile[ 63: 32]	<= sramTileB[tRegTileIxA[9:1]];
	tSRamTile[ 95: 64]	<= sramTileC[tRegTileIxB[9:1]];
	tSRamTile[127: 96]	<= sramTileD[tRegTileIxB[9:1]];

	if(tNextTileSrSt)
	begin
//		$display("Jx2MemTile: Store Tile A=%X D=%X",
//			regInAddr, tNextTile);
	
		sramTileA[tRegTileIxA[9:1]] <= tNextTile[ 31:  0];
		sramTileB[tRegTileIxA[9:1]] <= tNextTile[ 63: 32];
		sramTileC[tRegTileIxB[9:1]] <= tNextTile[ 95: 64];
		sramTileD[tRegTileIxB[9:1]] <= tNextTile[127: 96];
	end

//	if(!tBlkMissA)
//	begin
//		tRamTile[ 31: 0]	<= memTileA[tRegTileIxA];
//		tRamTile[ 63:32]	<= memTileB[tRegTileIxA];
//		tRamTile[ 95:64]	<= memTileC[tRegTileIxB];
//		tRamTile[127:96]	<= memTileD[tRegTileIxB];
//		{ tRamBlkFlagA, tRamBlkAddrA }	<= memTileAdA[tRegTileIxA];
//		{ tRamBlkFlagB, tRamBlkAddrB }	<= memTileAdB[tRegTileIxB];
//	end

	if(!tBlkMissA && !tRamStickyA && tRamLaneA)
	begin
		tRamTileA[ 31: 0]				<= ramTileA[tRamTileIxA];
		tRamTileA[ 63:32]				<= ramTileB[tRamTileIxA];
		tRamTileA[ 95:64]				<= ramTileC[tRamTileIxA];
		tRamTileA[127:96]				<= ramTileD[tRamTileIxA];
		{ tRamBlkFlagA, tRamBlkAddrA }	<= ramTileAdA[tRamTileIxA];
	end
	else if((tBlkMissA || tRamBlkFlagA[0]) && !tMissReqFlagB &&
		!tRamStickyA && tRamLaneA)
	begin
	
		$display("MemTile, MissA-1");
	
		if((memOK==UMEM_OK_READY) && tMissDoneFlagA && tMissReqFlagA)
		begin
			tMissDoneFlagA		<= 0;
			tRamBlkAddrA		<= tBlkAddrA;
			tRamBlkFlagA		<= 0;
			tRamStickyA			<= 1;

			tMissReqFlagA		<= 0;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissA Done");
`endif
		end
			if((memOK==UMEM_OK_OK) && tMissReqFlagA)
		begin
			if(tRamBlkFlagA[0])
			begin
				tRamBlkFlagA		<= 0;
				tMemOpm				<= 5'b00000;
				tRamStickyA			<= 1;

`ifndef JX2_QUIET
				$display("RAM MissA Dirty Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyA, tMissReqFlagA);
`endif
			end
			else
			begin
				tRamTileA[127:0]	<= memInData;
				tMissDoneFlagA		<= 1;

`ifndef JX2_QUIET
				$display("RAM Miss Clean Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyA, tMissReqFlagA);
`endif
			end

			tMemAddr			<= UV32_XX;
			tMemOutData			<= UV128_XX;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissA Get: A=%X D=%X_%X_%X_%X",
				tBlkAddrA,
				memInData[127: 96], memInData[ 95: 64],
				memInData[ 63: 32], memInData[ 31:  0]);
`endif
		end
		else if(!tMissReqFlagB)
		begin
			if(tRamBlkFlagA[0])
			begin
				tMemAddr		<= { tRamBlkAddrA[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tRamTileA[127:0];
				tMissDoneFlagA	<= 0;
				tMissReqFlagA	<= 1;

`ifndef JX2_QUIET
				$display("RAM MissA, Store %X", tBlkAddrA);
`endif
			end
			else if(tBlkMissA)
			begin
				tMemAddr		<= { tBlkAddrA[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemOutData		<= UV128_XX;
				tMissDoneFlagA	<= 0;
				tMissReqFlagA	<= 1;

`ifndef JX2_QUIET
				$display("RAM MissA, Load %X OK=%d", tBlkAddrA, memOK);
`endif
			end
			else
			begin
`ifndef JX2_QUIET
				$display("RAM MissA, Neither %X", tBlkAddrA);
`endif
			end

`ifndef JX2_QUIET
			$display("RAM MissA %X %X", tBlkAddrA, tRamBlkAddrA);
`endif
		end
		
	end
	else
	begin
		tMissReqFlagA	<= 0;
		tMissDoneFlagA	<= 0;
	end

	if(!tBlkMissB && !tRamStickyB && tRamLaneB)
	begin
		tRamTileB[ 31: 0]				<= ramTileE[tRamTileIxB];
		tRamTileB[ 63:32]				<= ramTileF[tRamTileIxB];
		tRamTileB[ 95:64]				<= ramTileG[tRamTileIxB];
		tRamTileB[127:96]				<= ramTileH[tRamTileIxB];
		{ tRamBlkFlagB, tRamBlkAddrB }	<= ramTileAdB[tRamTileIxB];
	end
	else if((tBlkMissB || tRamBlkFlagB[0]) && !tMissReqFlagA &&
		!tBlkMissA && !tRamStickyB && tRamLaneB)
	begin
		$display("MemTile, MissB-1");

		if((memOK==UMEM_OK_READY) && tMissDoneFlagB && tMissReqFlagB)
		begin
			tMissDoneFlagB		<= 0;
			tRamBlkAddrB		<= tBlkAddrB;
			tRamBlkFlagB		<= 0;
			tRamStickyB			<= 1;

			tMissReqFlagB		<= 0;
			tMemOpm				<= 5'b00000;
		end
			if((memOK==UMEM_OK_OK) && tMissReqFlagB)
		begin
			if(tRamBlkFlagB[0])
			begin
				tRamBlkFlagB		<= 0;
				tMemOpm				<= 5'b00000;
				tRamStickyB			<= 1;

`ifndef JX2_QUIET
				$display("RAM MissB Dirty Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyB, tMissReqFlagB);
`endif
			end
			else
			begin
				tRamTileB[127:0]	<= memInData;
				tMissDoneFlagB		<= 1;

`ifndef JX2_QUIET
				$display("RAM Miss Clean Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyB, tMissReqFlagB);
`endif
			end

			tMemAddr			<= UV32_XX;
			tMemOutData			<= UV128_XX;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissB Get: A=%X D=%X_%X_%X_%X",
				tBlkAddrB,
				memInData[127: 96], memInData[ 95: 64],
				memInData[ 63: 32], memInData[ 31:  0]);
`endif
		end
		else if(!tMissReqFlagA)
		begin
			if(tRamBlkFlagB[0])
			begin
				tMemAddr		<= { tRamBlkAddrB[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tRamTileB[127:0];
				tMissDoneFlagB	<= 0;
				tMissReqFlagB	<= 1;
			end
			else if(tBlkMissB)
			begin
				tMemAddr		<= { tBlkAddrB[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemOutData		<= UV128_XX;
				tMissDoneFlagB	<= 0;
				tMissReqFlagB	<= 1;
			end
			
`ifndef JX2_QUIET
			$display("RAM MissB %X %X", tBlkAddrA, tRamBlkAddrA);
`endif
		end
		
	end
	else
	begin
		tMissReqFlagB	<= 0;
		tMissDoneFlagB	<= 0;
	end

	tAccTileIxA		<= tRegTileIxA;
	tAccTileIxB		<= tRegTileIxB;

/*
	if(tNextTileSt)
	begin
		memTileA[tNextTileIxA]		<= tNextTile[ 31: 0];
		memTileB[tNextTileIxA]		<= tNextTile[ 63:32];
		memTileC[tNextTileIxB]		<= tNextTile[ 95:64];
		memTileD[tNextTileIxB]		<= tNextTile[127:96];
		memTileAdA[tNextTileIxA]	<= { tNextBlkFlagA, tNextBlkAddrA };
		memTileAdB[tNextTileIxB]	<= { tNextBlkFlagB, tNextBlkAddrB };
		tRamSticky		<= 0;
	end
*/

	if(tNextRamTileStA && tRamLaneA)
	begin
		ramTileA[tNextRamTileIxA]		<= tNextTileA[ 31: 0];
		ramTileB[tNextRamTileIxA]		<= tNextTileA[ 63:32];
		ramTileC[tNextRamTileIxA]		<= tNextTileA[ 95:64];
		ramTileD[tNextRamTileIxA]		<= tNextTileA[127:96];
		ramTileAdA[tNextRamTileIxA]		<= { tNextBlkFlagA, tNextBlkAddrA };
		tRamStickyA						<= 0;
	end

	if(tNextRamTileStB && tRamLaneB)
	begin
		ramTileE[tNextRamTileIxB]		<= tNextTileB[ 31: 0];
		ramTileF[tNextRamTileIxB]		<= tNextTileB[ 63:32];
		ramTileG[tNextRamTileIxB]		<= tNextTileB[ 95:64];
		ramTileH[tNextRamTileIxB]		<= tNextTileB[127:96];
		ramTileAdB[tNextRamTileIxB]		<= { tNextBlkFlagB, tNextBlkAddrB };
		tRamStickyB						<= 0;
	end

end


endmodule

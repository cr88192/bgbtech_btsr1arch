/*

Transfers are in terms of 256 bit tiles, which map to 128 bits logical.
Each tile will contain part of the following tile.

Memory Map:
0000..7FFF: ROM
C000..DFFF: SRAM
E000..FFFF: MMIO (Bypass)

 */

`include "Jx2CoreDefs.v"

module Jx2MemTileA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInData,
	regOutData,		regOutOK,
	regInOpm,
	regOutExc,		regOutTea,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);

// /* verilator lint_off UNOPTFLAT */

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input PC address
input[255:0]	regInData;		//input data (store)
input[4:0]		regInOpm;		//Operation Size/Type

output[255:0]	regOutData;		//output data (load)
output[1:0]		regOutOK;		//set if operation suceeds
output[15:0]	regOutExc;		//Raise Exception
output[63:0]	regOutTea;		//Exception TEA

input[127:0]	memInData;	//memory PC data
output[127:0]	memOutData;	//memory PC data
output[31:0]	memAddr;	//memory PC address
output[4:0]		memOpm;		//memory PC operation mode
input[1:0]		memOK;		//memory PC OK

input[31:0]		mmioInData;		//mmio data in
output[31:0]	mmioOutData;	//mmio data out
output[31:0]	mmioAddr;		//mmio address
output[4:0]		mmioOpm;		//mmio read
input[1:0]		mmioOK;			//mmio OK


reg[255:0]		tOutData2;
reg[1:0]		tRegOutOK2;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea2;

assign		regOutData = tOutData2;
assign		regOutOK = tRegOutOK2;
assign		regOutExc = tRegOutExc2;
assign		regOutTea = tRegOutTea2;

reg[255:0]		tOutData;
reg[1:0]		tRegOutOK;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;


reg[127:0]		tMemOutData2;	//memory PC data
reg[31:0]		tMemAddr2;		//memory PC address
reg[4:0]		tMemOpm2;		//memory PC output-enable

assign			memOutData	= tMemOutData2;
assign			memAddr		= tMemAddr2;
assign			memOpm		= tMemOpm2;

reg[127:0]		tMemOutData;	//memory PC data
reg[31:0]		tMemAddr;		//memory PC address
reg[4:0]		tMemOpm;		//memory PC output-enable


reg[31:0]		tMmioOutData2;	//mmio data out
reg[31:0]		tMmioAddr2;		//mmio address
reg[4:0]		tMmioOpm2;		//mmio read

assign			mmioOutData	= tMmioOutData2;
assign			mmioAddr	= tMmioAddr2;
assign			mmioOpm		= tMmioOpm2;

reg[31:0]		tMmioOutData;	//mmio data out
reg[31:0]		tMmioAddr;		//mmio address
reg[4:0]		tMmioOpm;		//mmio read


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

(* ram_style="block" *) reg[31:0]		ramTileA[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileB[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileC[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileD[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileE[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileF[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileG[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileH[2047:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileAdA[2047:0];		//RAM Address
(* ram_style="block" *) reg[31:0]		ramTileAdB[2047:0];		//RAM Address


reg[255:0]		tRomTile;
reg[255:0]		tSRamTile;
reg[11:0]		tAccTileIx;

reg[255:0]		tRamTile;
reg[27:0]		tRamTileAddrA;			//RAM Address
reg[27:0]		tRamTileAddrB;			//RAM Address

reg[27:0]		tNextRamTileAddrA;		//RAM Address
reg[27:0]		tNextRamTileAddrB;		//RAM Address

reg[3:0]		tRamTileFlagA;			//RAM Flags
reg[3:0]		tRamTileFlagB;			//RAM Flags
reg[3:0]		tNextRamTileFlagA;		//RAM Flags
reg[3:0]		tNextRamTileFlagB;		//RAM Flags

reg[255:0]		tRamTile1;
reg[27:0]		tRamTile1AddrA;			//RAM Address
reg[27:0]		tRamTile1AddrB;			//RAM Address
reg[3:0]		tRamTile1FlagA;			//RAM Flags
reg[3:0]		tRamTile1FlagB;			//RAM Flags

reg[255:0]		tRamTile2;
reg[27:0]		tRamTile2AddrA;			//RAM Address
reg[27:0]		tRamTile2AddrB;			//RAM Address
reg[3:0]		tRamTile2FlagA;			//RAM Flags
reg[3:0]		tRamTile2FlagB;			//RAM Flags


reg[255:0]		tMemTile;

reg[255:0]		tNextTile;
reg[255:0]		tNextTileA;

reg[11:0]		tRegTileIxA;
reg[11:0]		tRegTileIxB;

reg[11:0]		tAccTileIxA;
reg[11:0]		tAccTileIxB;

reg[11:0]		tNextTileIxA;
reg[11:0]		tNextTileIxB;

reg[11:0]		tNextTileIx;
reg				tNextTileSt;
reg				tNextTileSrSt;

wire		regInOE;
wire		regInWR;
assign		regInOE = regInOpm[3];
assign		regInWR = regInOpm[4];

wire		addrIsSRam;
assign		addrIsSRam =
	(regInAddr[31:0] >= 32'h0000C000) &&
	(regInAddr[31:0] <  32'h0000E000) ;

wire		addrIsRom;
assign		addrIsRom =
	(regInAddr[31:0] <  32'h00008000);

//wire			addrIsSRam;
//assign		addrIsSRam =
//	(regInAddr[28:0] <= 29'h0010_0000) &&
//	(regInAddr[14] == 1);

wire		addrIsRam;
assign		addrIsRam =
	(regInAddr[31:0] >= 32'h01000000) &&
	(regInAddr[31:0] <  32'h7FFFFFFF);

reg			icBlkBypass;
reg			icReqLow4GB;

reg[27:0]	tRamBlkAddrA;
reg[27:0]	tRamBlkAddrB;

reg[27:0]	tRamReqBlkAddrA;
reg[27:0]	tRamReqBlkAddrB;

reg			tRamMissA;
reg			tRamMissB;
reg			tRamStickyA;
reg			tRamStickyB;

reg			tMissDoneFlag;
reg			tMissReqaFlag;
reg			tMissReqbFlag;
reg			tMissDoneaFlag;
reg			tMissDonebFlag;

reg			tNextTileStWR;

// /* verilator lint_on UNOPTFLAT */

initial begin
	$readmemh("bootrom_2a.txt", romTileA);
	$readmemh("bootrom_2b.txt", romTileB);
	$readmemh("bootrom_2c.txt", romTileC);
	$readmemh("bootrom_2d.txt", romTileD);
	$readmemh("bootrom_2e.txt", romTileE);
	$readmemh("bootrom_2f.txt", romTileF);
	$readmemh("bootrom_2g.txt", romTileG);
	$readmemh("bootrom_2h.txt", romTileH);
end

always @*
begin
//	tMemTile		= 0;
//	tOutData		= 0;
//	tNextTile		= 0;

	tMemTile		= UV256_XX;
	tOutData		= UV256_XX;
	tNextTile		= UV256_XX;

	icReqLow4GB	= (regInAddr[47:32]==16'h0000);
	icBlkBypass	= (regInAddr[47:45]==3'b101) ||
		((regInAddr[31:29]==3'b101) && icReqLow4GB);

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

	tNextTileIxA	= 0;
	tNextTileIxB	= 0;

	tNextTileSt		= 0;
	tNextTileSrSt	= 0;
	tRegOutOK		= UMEM_OK_READY;

	tRegOutExc		= 0;
	tRegOutTea		= UV64_XX;

//	tMemOutData		= 0;	//memory PC data
//	tMemAddr		= 0;	//memory PC address
//	tMemOpm			= 0;	//memory PC output-enable

	tMmioOutData	= 0;	//mmio data out
	tMmioAddr		= 0;	//mmio address
	tMmioOpm		= 0;	//mmio read

	if(!tRamStickyA && !tMissReqaFlag)
	begin
		tRamTile[127:0] = tRamTile2[127:0];
		tRamTileAddrA = tRamTile2AddrA;
		tRamTileFlagA = tRamTile2FlagA;
	end
	else
	begin
		tRamTile[127:0] = tRamTile1[127:0];
		tRamTileAddrA = tRamTile1AddrA;
		tRamTileFlagA = tRamTile1FlagA;
	end

	if(!tRamStickyB && !tMissReqbFlag)
	begin
		tRamTile[255:128] = tRamTile2[255:128];
		tRamTileAddrB = tRamTile2AddrB;
		tRamTileFlagB = tRamTile2FlagB;
	end
	else
	begin
		tRamTile[255:128] = tRamTile1[255:128];
		tRamTileAddrB = tRamTile1AddrB;
		tRamTileFlagB = tRamTile1FlagB;
	end


	if((regInOE || regInWR) && !icBlkBypass)
	begin

`ifndef JX2_QUIET
		$display("MemTileA Addr=%X Opm=%X IsRom=%d IsRam=%d IsSRam=%d", 
			regInAddr, regInOpm,
			addrIsRom, addrIsRam, addrIsSRam);
`endif

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
			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;
			
//			$display("Rom: %X", tMemTile);
//			$display("Rom: Out=%X", tOutData);
		end
		else
			if(addrIsSRam)
		begin
//			$display("SRAM");

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
//			tNextTileA = tMemTile;

			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;
		
			if(regInWR)
			begin
				tNextTileIxA = tRegTileIxA;
				tNextTileIxB = tRegTileIxB;
				tNextTileSrSt = 1;

				if(regInAddr[4])
				begin
					tNextTile[127:  0] = regInData[255:128];
					tNextTile[255:128] = regInData[127:  0];
				end
				else
				begin
					tNextTile[127:  0] = regInData[127:  0];
					tNextTile[255:128] = regInData[255:128];
				end
			end
		end
		else
			if(addrIsRam)
		begin
//			$display("RAM");
			if(regInAddr[4])
			begin
				tMemTile[127:  0] = tRamTile[255:128];
				tMemTile[255:128] = tRamTile[127:  0];
			end
			else
			begin
				tMemTile[127:  0] = tRamTile[127:  0];
				tMemTile[255:128] = tRamTile[255:128];
			end
			tNextTile = tSRamTile;
//			tNextTileA = tMemTile;

			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
				UMEM_OK_OK : UMEM_OK_HOLD;

			tOutData=tMemTile;
		
			if(regInWR)
			begin
				tNextTileIxA = tRegTileIxA;
				tNextTileIxB = tRegTileIxB;
				tNextTileSt = 1;

				if(regInAddr[4])
				begin
					tNextTile[127:  0] = regInData[255:128];
					tNextTile[255:128] = regInData[127:  0];
				end
				else
				begin
					tNextTile[127:  0] = regInData[127:  0];
					tNextTile[255:128] = regInData[255:128];
				end
			end
		end
		else
			if(icBlkBypass)
		begin
		end
		else
		begin
			$display("Invalid Address %X", regInAddr);
			tRegOutExc = 16'h8000;
			tRegOutTea = { UV16_00, regInAddr };
			tRegOutOK = UMEM_OK_OK;
		end

	end

	if(icBlkBypass)
	begin
`ifndef JX2_QUIET
		$display("MMIO A=%X DO=%X DI=%X Opm=%X OK=%X",
			regInAddr, regInData[31:0], mmioInData[31:0],
			regInOpm, mmioOK);
`endif
		
		tMmioOutData	= regInData[31:0];
		tMmioAddr		= regInAddr[31:0];
		tMmioOpm		= regInOpm;
		tRegOutOK		= mmioOK;
		tOutData		= { 224'h0, mmioInData[31:0] };
	end
end

always @ (posedge clock)
begin

	tOutData2		<= tOutData;
	tRegOutOK2		<= tRegOutOK;
	tRegOutExc2		<= tRegOutExc;
	tRegOutTea2		<= tRegOutTea;

	tMemOutData2	<= tMemOutData;		//memory PC data
	tMemAddr2		<= tMemAddr;		//memory PC address
	tMemOpm2		<= tMemOpm;			//memory PC output-enable

	tMmioOutData2	<= tMmioOutData;	//mmio data out
	tMmioAddr2		<= tMmioAddr;		//mmio address
	tMmioOpm2		<= tMmioOpm;		//mmio read

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

	tRamTile2[ 31:  0] <= ramTileA[tRegTileIxA[11:1]];
	tRamTile2[ 63: 32] <= ramTileB[tRegTileIxA[11:1]];
	tRamTile2[ 95: 64] <= ramTileC[tRegTileIxA[11:1]];
	tRamTile2[127: 96] <= ramTileD[tRegTileIxA[11:1]];
	tRamTile2[159:128] <= ramTileE[tRegTileIxB[11:1]];
	tRamTile2[191:160] <= ramTileF[tRegTileIxB[11:1]];
	tRamTile2[223:192] <= ramTileG[tRegTileIxB[11:1]];
	tRamTile2[255:224] <= ramTileH[tRegTileIxB[11:1]];
	{tRamTile2FlagA, tRamTile2AddrA} <=
		ramTileAdA[tRegTileIxA[11:1]];
	{tRamTile2FlagB, tRamTile2AddrB} <=
		ramTileAdB[tRegTileIxB[11:1]];

	tRamTile1		<= tRamTile;
	tRamTile1AddrA	<= tRamTileAddrA;
	tRamTile1FlagA	<= tRamTileFlagA;
	tRamTile1AddrB	<= tRamTileAddrB;
	tRamTile1FlagB	<= tRamTileFlagB;

	tAccTileIxA		<= tRegTileIxA;
	tAccTileIxB		<= tRegTileIxB;

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


	tAccTileIxA <= tRegTileIxA;
	tAccTileIxB <= tRegTileIxB;


	if(tRamMissA)
	begin
		if((memOK==UMEM_OK_READY) && tMissDoneFlag && tMissReqaFlag)
		begin
			tMissDoneFlag		<= 0;
			tRamTile1AddrA		<= tRamReqBlkAddrA;
			tRamTile1FlagA		<= 0;
			tRamStickyA			<= 1;

			tMissReqaFlag		<= 0;
			tMissReqbFlag		<= 0;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissA Done Ok=%d Stky=%d ReqA=%d Addr=%X",
				memOK, tRamStickyA, tMissReqaFlag, tRamReqBlkAddrA);
`endif
		end
		else
			if((memOK==UMEM_OK_OK) && tMissReqaFlag)
		begin
			if(tRamTileFlagA[0])
			begin
				tRamTile1FlagA		<= 0;
				tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
				$display("RAM MissA Dirty Ok=%d Stky=%d ReqA=%d",
					memOK, tRamStickyA, tMissReqaFlag);
`endif
			end
			else
			begin
				tRamTile1[127:  0]	<= memInData;
//				tRamTile1AddrA		<= tRamReqBlkAddrA;
//				tRamTile1FlagA		<= 0;
//				tRamStickyA			<= 1;
				tMissDoneFlag		<= 1;

`ifndef JX2_QUIET
				$display("RAM MissA Clean Ok=%d Stky=%d ReqA=%d",
					memOK, tRamStickyA, tMissReqaFlag);
`endif
			end

			tMemAddr			<= UV32_XX;
			tMemOutData			<= UV128_XX;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissA Get: A=%X D=%X_%X_%X_%X",
				tRamReqBlkAddrA,
				memInData[127: 96], memInData[ 95: 64],
				memInData[ 63: 32], memInData[ 31:  0]);
`endif

		end
		else
		begin
			if(tRamTileFlagA[0])
			begin
				tMemAddr		<= { tRamTileAddrA[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tRamTile[127:  0];
				tMissDoneFlag	<= 0;

`ifndef JX2_QUIET
				$display("RAM MissA WR Ok=%d", memOK);
`endif
			end
			else
			begin
				tMemAddr		<= { tRamReqBlkAddrA[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemOutData		<= UV128_XX;
				tMissDoneFlag	<= 0;
				
`ifndef JX2_QUIET
				$display("RAM MissA RD Ok=%d", memOK);
`endif
			end

`ifndef JX2_QUIET
			$display("RAM MissA %X %X", tRamReqBlkAddrA, tRamTileAddrA);
`endif
			tMissReqaFlag	<= 1;
			tMissReqbFlag	<= 0;
		end
	end
	else
		if(tRamMissB)
	begin
		if((memOK==UMEM_OK_READY) && tMissDoneFlag && tMissReqbFlag)
		begin
			tMissDoneFlag		<= 0;
			tRamTile1AddrB		<= tRamReqBlkAddrB;
			tRamTile1FlagB		<= 0;
			tRamStickyB			<= 1;

			tMissReqaFlag		<= 0;
			tMissReqbFlag		<= 0;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissA Done Ok=%d Stky=%d ReqA=%d",
				memOK, tRamStickyA, tMissReqaFlag);
`endif
		end
		else
			if((memOK==UMEM_OK_OK) && tMissReqbFlag)
		begin
			if(tRamTileFlagB[0])
			begin
				tRamTile1FlagB		<= 0;
				tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
				$display("RAM MissB Dirty Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyB, tMissReqbFlag);
`endif
			end
			else
			begin
				tRamTile1[255:128]	<= memInData;
//				tRamTile1AddrB		<= tRamReqBlkAddrB;
//				tRamTile1FlagB		<= 0;
//				tRamStickyB			<= 1;
				tMissDoneFlag		<= 1;

`ifndef JX2_QUIET
				$display("RAM MissB Clean Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyB, tMissReqbFlag);
`endif
			end

			tMemAddr			<= UV32_XX;
			tMemOutData			<= UV128_XX;
			tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
			$display("RAM MissB Get: A=%X D=%X_%X_%X_%X",
				tRamReqBlkAddrB,
				memInData[127: 96], memInData[ 95: 64],
				memInData[ 63: 32], memInData[ 31:  0]);
`endif
		end
		else
		begin
			if(tRamTileFlagB[0])
			begin
				tMemAddr		<= { tRamTileAddrB[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tRamTile[255:128];
				tMissDoneFlag	<= 0;
			end
			else
			begin
				tMemAddr		<= { tRamReqBlkAddrB[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemOutData		<= UV128_XX;
				tMissDoneFlag	<= 0;
			end
			
			tMissReqaFlag	<= 0;
			tMissReqbFlag	<= 1;

`ifndef JX2_QUIET
//			$display("RAM MissB %X", tRamReqBlkAddrB);
			$display("RAM MissB %X %X", tRamReqBlkAddrB, tRamTileAddrB);
`endif
		end
	end
	else if(tNextTileStWR)
	begin
			if(memOK==UMEM_OK_OK)
			begin
				tMemOpm			<= UMEM_OPM_READY;
				if(tMissReqaFlag)
					tMissDoneaFlag	<= 1;
				if(tMissReqbFlag)
					tMissDonebFlag	<= 1;
			end
			else
			if(tNextRamTileFlagA[0] && !tMissDoneaFlag)
			begin
				tMemAddr		<= { tNextRamTileAddrA[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tNextTile[127:  0];
				tMissDoneFlag	<= 0;
				tMissReqaFlag	<= 1;
				tMissReqbFlag	<= 0;
			end
			else if(tNextRamTileFlagB[0] && !tMissDonebFlag)
			begin
				tMemAddr		<= { tNextRamTileAddrB[27:0], 4'h0 };
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemOutData		<= tNextTile[255:128];
				tMissDoneFlag	<= 0;
				tMissReqaFlag	<= 0;
				tMissReqbFlag	<= 1;
			end
			else
			begin
				tMissDoneFlag	<= 1;
			end
	end
	else
	begin
//		$display("RAM Miss, Release");
		tMissDoneFlag	<= 0;
		tMissReqaFlag	<= 0;
		tMissReqbFlag	<= 0;
		tMissDoneaFlag	<= 0;
		tMissDonebFlag	<= 0;
	end

	if(tNextTileSt)
	begin
		if(!tRamMissA)
		begin
			ramTileA[tNextTileIxA[11:1]]	<= tNextTile[ 31:  0];
			ramTileB[tNextTileIxA[11:1]]	<= tNextTile[ 63: 32];
			ramTileC[tNextTileIxA[11:1]]	<= tNextTile[ 95: 64];
			ramTileD[tNextTileIxA[11:1]]	<= tNextTile[127: 96];
			ramTileAdA[tNextTileIxA[11:1]]	<=
				{ tNextRamTileFlagA, tNextRamTileAddrA };
			tRamStickyA			<= 0;

`ifndef JX2_QUIET
			$display("RAM St A: Ix=%X Addr=%X",
				tNextTileIxA, tNextRamTileAddrA);

			$display("RAM St A: D=%X_%X_%X_%X",
				tNextTile[127: 96], tNextTile[ 95: 64],
				tNextTile[ 63: 32], tNextTile[ 31:  0]);
`endif
		end

		if(!tRamMissB)
		begin
			ramTileE[tNextTileIxB[11:1]] 	<= tNextTile[159:128];
			ramTileF[tNextTileIxB[11:1]]	<= tNextTile[191:160];
			ramTileG[tNextTileIxB[11:1]]	<= tNextTile[223:192];
			ramTileH[tNextTileIxB[11:1]]	<= tNextTile[255:224];
			ramTileAdB[tNextTileIxB[11:1]]	<=
				{ tNextRamTileFlagB, tNextRamTileAddrB };
			tRamStickyB			<= 0;

`ifndef JX2_QUIET
			$display("RAM St B: Ix=%X Addr=%X",
				tNextTileIxB, tNextRamTileAddrB);
			$display("RAM St B: D=%X_%X_%X_%X",
				tNextTile[255:224], tNextTile[223:192],
				tNextTile[191:160], tNextTile[159:128]);
`endif
		end
	end

end


endmodule

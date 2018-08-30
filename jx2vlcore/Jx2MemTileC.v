/*

Cache Style C
  128-bit Logical Tiles
  256-bit Full Duplex (D$)
    Moves two tiles in each direction.


Transfers are in terms of 256 bit tile pairs, which map to 128 bits logical.
Each tile will contain part of the following tile.

Will use a 128-bit interface to DRAM, Half-Duplex.

Address Space is 48 bits.
Physical Space is 32 bits (48b reserved).

Memory Map (Physical):
0000_0000..0000_7FFF: ROM
0000_C000..0000_DFFF: SRAM
0000_E000..0000_FFFF: MMIO (Bypass)

0100_0000..7FFF_FFFF: DRAM

A000_0000..BFFF_FFFF: MMIO (Bypass)


Memory Map (Virtual):
0000_00000000 .. 0000_7FFFFFFF: Userland (MMU)
0000_80000000 .. 0000_FFFFFFFF: Shadow (NO-MMU for now)
0008_00000000 .. 7FFF_FFFFFFFF: Userland (MMU)


TLB Entries:

[111:76] = Virtual Physical Address (From PTE)
[ 47:12] = Page Physical Address (From PTE)
[ 11: 0] = Access Flags
 */

`include "Jx2CoreDefs.v"

`include "Jx2MmuChkAcc.v"

module Jx2MemTileC(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInData,
	regOutAddr,		regOutData,
	regOutOK,		regInOpm,
	regOutExc,		regOutTea,
	
	regInMMCR,		regInKRR,		regInSR,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input Address
input[255:0]	regInData;		//input data (store)

input[47:0]		regOutAddr;		//output Address
output[255:0]	regOutData;		//output data (load)

output[1:0]		regOutOK;		//set if operation suceeds
input[4:0]		regInOpm;		//Operation Size/Type
output[15:0]	regOutExc;		//Raise Exception
output[63:0]	regOutTea;		//Exception TEA

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register


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
reg[11:0]		tRegTileIxC;
reg[11:0]		tRegTileIxD;

reg[11:0]		tNextTileIx;
reg				tNextTileSt;
reg				tNextTileSrSt;
reg[1:0]		tRegOutOK;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;

assign		regOutData = tOutData;
assign		regOutOK = tRegOutOK;
assign		regOutExc = tRegOutExc;
assign		regOutTea = tRegOutTea;

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
	(regInAddr[31:0] >= 32'01000000) &&
	(regInAddr[31:0] <  32'7FFFFFFF);


reg[63:0]	tlbBlkHiA[15:0];
reg[63:0]	tlbBlkHiB[15:0];
reg[63:0]	tlbBlkHiC[15:0];
reg[63:0]	tlbBlkHiD[15:0];

reg[63:0]	tlbBlkLoA[15:0];
reg[63:0]	tlbBlkLoB[15:0];
reg[63:0]	tlbBlkLoC[15:0];
reg[63:0]	tlbBlkLoD[15:0];

reg[63:0]	tlbBlkHiE[15:0];
reg[63:0]	tlbBlkHiF[15:0];
reg[63:0]	tlbBlkHiG[15:0];
reg[63:0]	tlbBlkHiH[15:0];

reg[63:0]	tlbBlkLoE[15:0];
reg[63:0]	tlbBlkLoF[15:0];
reg[63:0]	tlbBlkLoG[15:0];
reg[63:0]	tlbBlkLoH[15:0];

reg[3:0]	tlbHixA;
reg[3:0]	tlbHixB;

reg[127:0]	tlbHdatA;
reg[127:0]	tlbHdatB;
reg[127:0]	tlbHdatC;
reg[127:0]	tlbHdatD;

reg[127:0]	tlbHdatE;
reg[127:0]	tlbHdatF;
reg[127:0]	tlbHdatG;
reg[127:0]	tlbHdatH;

reg			tlbMmuEnable;
reg			tlbMmuSkipA;
reg			tlbMmuSkipB;

reg			tlbHitA;
reg			tlbHitB;
reg			tlbHitC;
reg			tlbHitD;
reg			tlbHitE;
reg			tlbHitF;
reg			tlbHitG;
reg			tlbHitH;

reg			tlbMissA;
reg			tlbMissB;

reg[31:0]	tlbAddrA;
reg[31:0]	tlbAddrB;
reg[31:0]	tlbAccA;
reg[31:0]	tlbAccB;


reg[4:0]	tRegInOpmA;
reg[4:0]	tRegInOpmB;
wire[15:0]	tTlbExcA;
wire[15:0]	tTlbExcB;

wire[15:0]	tTlbExcC;
assign		tTlbExcC = (tTlbExcA[15]) ? tTlbExcA : tTlbExcB;

Jx2MmuChkAcc tlbChkAccA(
	regInMMCR,
	regInKRR,
	regInSR,
	tRegInOpmA,
	tlbAccA,
	tTlbExcA);

Jx2MmuChkAcc tlbChkAccB(
	regInMMCR,
	regInKRR,
	regInSR,
	tRegInOpmB,
	tlbAccB,
	tTlbExcB);


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
	
	tlbMmuEnable = 0;
	
	tlbHixA = regInAddr[15:12]^regInAddr[19:16];
	tlbHixB = regOutAddr[15:12]^regOutAddr[19:16];

	tlbHdatA = { tlbBlkHiA[tlbHixA], tlbBlkLoA[tlbHixA]};
	tlbHdatB = { tlbBlkHiB[tlbHixA], tlbBlkLoB[tlbHixA]};
	tlbHdatC = { tlbBlkHiC[tlbHixA], tlbBlkLoC[tlbHixA]};
	tlbHdatD = { tlbBlkHiD[tlbHixA], tlbBlkLoD[tlbHixA]};
	tlbHdatE = { tlbBlkHiE[tlbHixB], tlbBlkLoE[tlbHixB]};
	tlbHdatF = { tlbBlkHiF[tlbHixB], tlbBlkLoF[tlbHixB]};
	tlbHdatG = { tlbBlkHiG[tlbHixB], tlbBlkLoG[tlbHixB]};
	tlbHdatH = { tlbBlkHiH[tlbHixB], tlbBlkLoH[tlbHixB]};
	
	tlbHitA = (regInAddr [47:12] == tlbHdatA[111:76]) && tlbHdatA[0];
	tlbHitB = (regInAddr [47:12] == tlbHdatB[111:76]) && tlbHdatB[0];
	tlbHitC = (regInAddr [47:12] == tlbHdatC[111:76]) && tlbHdatC[0];
	tlbHitD = (regInAddr [47:12] == tlbHdatD[111:76]) && tlbHdatD[0];
	tlbHitE = (regOutAddr[47:12] == tlbHdatE[111:76]) && tlbHdatE[0];
	tlbHitF = (regOutAddr[47:12] == tlbHdatF[111:76]) && tlbHdatF[0];
	tlbHitG = (regOutAddr[47:12] == tlbHdatG[111:76]) && tlbHdatG[0];
	tlbHitH = (regOutAddr[47:12] == tlbHdatH[111:76]) && tlbHdatH[0];

	tlbMmuSkipA = 0;
	tlbMmuSkipB = 0;
	if(regInAddr[47:32]==16'h0000)
		if(regInAddr[31])
			tlbMmuSkipA = 1;
	if(regOutAddr[47:32]==16'h0000)
		if(regOutAddr[31])
			tlbMmuSkipB = 1;

	if(tlbMmuEnable && !tlbMmuSkipA)
	begin
		tlbAddrA =
			tlbHitA ? { tlbHdatA[31:12], regInAddr[11:0] } :
			tlbHitB ? { tlbHdatB[31:12], regInAddr[11:0] } :
			tlbHitC ? { tlbHdatC[31:12], regInAddr[11:0] } :
			tlbHitD ? { tlbHdatD[31:12], regInAddr[11:0] } :
			regInAddr[31:0];
		tlbAccA = 
			tlbHitA ? { tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
			tlbHitB ? { tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] } :
			tlbHitC ? { tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
			tlbHitD ? { tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] } :
			0;
		tlbMissA = !( tlbHitA || tlbHitB || tlbHitC || tlbHitD );
	end
	else
	begin
		tlbAddrA = regInAddr[31:0];
		tlbMissA = 0;
	end

	if(tlbMmuEnable && !tlbMmuSkipB)
	begin
		tlbAddrB =
			tlbHitE ? { tlbHdatE[31:12], regOutAddr[11:0] } :
			tlbHitF ? { tlbHdatF[31:12], regOutAddr[11:0] } :
			tlbHitG ? { tlbHdatG[31:12], regOutAddr[11:0] } :
			tlbHitH ? { tlbHdatH[31:12], regOutAddr[11:0] } :
			regOutAddr[31:0];
		tlbAccB = 
			tlbHitE ? { tlbHdatE[127:112], tlbHdatE[75:64], tlbHdatE[7:4] } :
			tlbHitF ? { tlbHdatF[127:112], tlbHdatF[75:64], tlbHdatF[7:4] } :
			tlbHitG ? { tlbHdatG[127:112], tlbHdatG[75:64], tlbHdatG[7:4] } :
			tlbHitH ? { tlbHdatH[127:112], tlbHdatH[75:64], tlbHdatH[7:4] } :
			0;
		tlbMissB = !( tlbHitE || tlbHitF || tlbHitG || tlbHitH );
	end
	else
	begin
		tlbAddrB = regOutAddr[31:0];
		tlbMissB = 0;
	end


	tRegInOpmA = regInOpm;
	tRegInOpmB = regInOpm;
	tRegInOpmA[4] = 0;
	tRegInOpmB[3] = 0;
	
//	tRegOutExc = tTlbExcC;

	tRegOutExc = 0;

	if(tTlbExcA[15])
	begin
		tRegOutExc = tTlbExcA;
		tRegOutTea = regInAddr;
	end

	if(tTlbExcB[15])
	begin
		tRegOutExc = tTlbExcB;
		tRegOutTea = regOutAddr;
	end

	if(tlbMissA)
	begin
		tRegOutExc = 16'hA001;
		tRegOutTea = regInAddr;
	end

	if(tlbMissB)
	begin
		tRegOutExc = 16'hA001;
		tRegOutTea = regOutAddr;
	end


	if(tlbAddrA[4])
	begin
		tRegTileIxB		= tlbAddrA[15:4];
		tRegTileIxA		= tRegTileIxB+1;
	end
	else
	begin
		tRegTileIxA		= tlbAddrA[15:4];
		tRegTileIxB		= tRegTileIxA;
	end

	if(tlbAddrB[4])
	begin
		tRegTileIxD		= tlbAddrB[15:4];
		tRegTileIxC		= tRegTileIxD+1;
	end
	else
	begin
		tRegTileIxC		= tlbAddrB[15:4];
		tRegTileIxD		= tRegTileIxC;
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

/*
				case(regInOp[1:0])
					3'b111:
						tNextTileA[255:0]=regInData;
					3'b010: case(regInAddr[3:2])
						2'b00: tNextTileA[ 31: 0] = regInData[31:0];
						2'b01: tNextTileA[ 63:32] = regInData[31:0];
						2'b10: tNextTileA[ 95:64] = regInData[31:0];
						2'b11: tNextTileA[127:96] = regInData[31:0];
					endcase
					default:
						tNextTileA[255:0]=regInData;
				endcase
*/

				tNextTileA[255:0]=regInData;

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

	tRamTile[ 31:  0] <= ramTileA[tRegTileIxA[11:1]];
	tRamTile[ 63: 32] <= ramTileB[tRegTileIxA[11:1]];
	tRamTile[ 95: 64] <= ramTileC[tRegTileIxA[11:1]];
	tRamTile[127: 96] <= ramTileD[tRegTileIxA[11:1]];
	tRamTile[159:128] <= ramTileE[tRegTileIxB[11:1]];
	tRamTile[191:160] <= ramTileF[tRegTileIxB[11:1]];
	tRamTile[223:192] <= ramTileG[tRegTileIxB[11:1]];
	tRamTile[255:224] <= ramTileH[tRegTileIxB[11:1]];

//	tRamTile[ 31: 0] <= memTileA[tRegTileIx];
//	tRamTile[ 63:32] <= memTileB[tRegTileIx];
//	tRamTile[ 95:64] <= memTileC[tRegTileIx];
//	tRamTile[127:96] <= memTileD[tRegTileIx];

	tAccTileIx <= tRegTileIx;

	if(tNextTileSt)
	begin
		ramTileA[tNextTileIxC[11:1]] <= tNextTile[ 31:  0];
		ramTileB[tNextTileIxC[11:1]] <= tNextTile[ 63: 32];
		ramTileC[tNextTileIxC[11:1]] <= tNextTile[ 95: 64];
		ramTileD[tNextTileIxC[11:1]] <= tNextTile[127: 96];
		ramTileE[tNextTileIxD[11:1]] <= tNextTile[159:128];
		ramTileF[tNextTileIxD[11:1]] <= tNextTile[191:160];
		ramTileG[tNextTileIxD[11:1]] <= tNextTile[223:192];
		ramTileH[tNextTileIxD[11:1]] <= tNextTile[255:224];
	end

	if(tNextTileSrSt)
	begin
		sramTileA[tRegTileIxC[8:1]] <= tNextTile[ 31:  0];
		sramTileB[tRegTileIxC[8:1]] <= tNextTile[ 63: 32];
		sramTileC[tRegTileIxC[8:1]] <= tNextTile[ 95: 64];
		sramTileD[tRegTileIxC[8:1]] <= tNextTile[127: 96];
		sramTileE[tRegTileIxD[8:1]] <= tNextTile[159:128];
		sramTileF[tRegTileIxD[8:1]] <= tNextTile[191:160];
		sramTileG[tRegTileIxD[8:1]] <= tNextTile[223:192];
		sramTileH[tRegTileIxD[8:1]] <= tNextTile[255:224];
	end
end


endmodule

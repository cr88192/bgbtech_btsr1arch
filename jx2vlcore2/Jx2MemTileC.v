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

// `include "Jx2MmuChkAcc.v"

module Jx2MemTileC(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInData,
	regOutAddr,		regOutData,
	regOutOK,		regInOpm,
	regOutExc,		regOutTea,
	
//	regInMMCR,		regInKRR,		regInSR,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input Address
input[47:0]		regOutAddr;		//output Address
input[255:0]	regInData;		//input data (store)
output[255:0]	regOutData;		//output data (load)
input[4:0]		regInOpm;		//Operation Size/Type

output[1:0]		regOutOK;		//set if operation suceeds
output[15:0]	regOutExc;		//Raise Exception
output[63:0]	regOutTea;		//Exception TEA

// input[63:0]		regInMMCR;		//MMU Control Register
// input[63:0]		regInKRR;		//Keyring Register
// input[63:0]		regInSR;		//Status Register


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

reg[31:0]		tMmioOutData2;	//mmio data out
reg[31:0]		tMmioAddr2;		//mmio address
reg[4:0]		tMmioOpm2;		//mmio read

assign			mmioOutData	= tMmioOutData2;
assign			mmioAddr	= tMmioAddr2;
assign			mmioOpm		= tMmioOpm2;

reg[31:0]		tMmioOutData;	//mmio data out
reg[31:0]		tMmioAddr;		//mmio address
reg[4:0]		tMmioOpm;		//mmio read

reg[31:0]		tMmioInData;	//mmio data in
reg[1:0]		tMmioOK;			//mmio OK

reg[47:0]		tRegInAddr;		//input Address
reg[47:0]		tRegOutAddr;		//input Address
reg[255:0]		tRegInData;		//input data (store)
reg[4:0]		tRegInOpm;		//Operation Size/Type


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

// /*
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
// */

/*
(* ram_style="block" *) reg[31:0]		ramTileA[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileB[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileC[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileD[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileE[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileF[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileG[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileH[1023:0];		//RAM/L2
(* ram_style="block" *) reg[31:0]		ramTileAdA[1023:0];	//RAM Address
(* ram_style="block" *) reg[31:0]		ramTileAdB[1023:0];	//RAM Address
*/

/* verilator lint_off UNOPTFLAT */

reg[255:0]		tRomTile;
reg[255:0]		tSRamTile;

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

reg[11:0]		tAccTileIxA;
reg[11:0]		tAccTileIxB;


reg[255:0]		tMemTile;

reg[255:0]		tNextTile;
reg[255:0]		tNextTileA;

reg[11:0]		tRegTileIxA;
reg[11:0]		tRegTileIxB;
reg[11:0]		tRegTileIxC;
reg[11:0]		tRegTileIxD;

reg[11:0]		tNextTileIxC;
reg[11:0]		tNextTileIxD;
reg				tNextTileSt;
reg				tNextTileSrSt;
reg				tNextTileStWR;

/* verilator lint_on UNOPTFLAT */

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


wire		regInOE;
wire		regInWR;
assign		regInOE = tRegInOpm[3];
assign		regInWR = tRegInOpm[4];

wire		regInIsLDTLB;
assign		regInIsLDTLB = (tRegInOpm==UMEM_OPM_LDTLB);

wire		addrIsSRamI;
wire		addrIsSRamO;
assign		addrIsSRamI =
	(tRegInAddr[31:0] >= 32'h0000C000) &&
	(tRegInAddr[31:0] <  32'h0000E000) ;
assign		addrIsSRamO =
	(tRegOutAddr[31:0] >= 32'h0000C000) &&
	(tRegOutAddr[31:0] <  32'h0000E000) ;

wire		addrIsRom;
assign		addrIsRom =
	(tRegOutAddr[31:0] <  32'h00008000);

//wire			addrIsSRam;
//assign		addrIsSRam =
//	(tRegInAddr[28:0] <= 29'h0010_0000) &&
//	(tRegInAddr[14] == 1);

wire		addrIsRamI;
wire		addrIsRamO;

assign		addrIsRamI =
	(tRegInAddr[31:0] >= 32'h01000000) &&
	(tRegInAddr[31:0] <  32'h7FFFFFFF);

assign		addrIsRamO =
	(tRegOutAddr[31:0] >= 32'h01000000) &&
	(tRegOutAddr[31:0] <  32'h7FFFFFFF);

reg[27:0]	tRamBlkAddrA;
reg[27:0]	tRamBlkAddrB;
//reg[27:0]	tRamBlkFlagA;
//reg[27:0]	tRamBlkFlagB;
reg			tRamMissA;
reg			tRamMissB;
reg			tRamStickyA;
reg			tRamStickyB;
reg[27:0]	tRamReqBlkAddrA;
reg[27:0]	tRamReqBlkAddrB;

reg[27:0]	tRamBlkAddrC;
reg[27:0]	tRamBlkAddrD;

reg[31:0]	tlbAddrA;
reg[31:0]	tlbAddrB;
reg[31:0]	tlbAccA;
reg[31:0]	tlbAccB;


reg[4:0]	tRegInOpmA;
reg[4:0]	tRegInOpmB;


reg		tRegOkO;
reg		tRegHoldO;
reg		tRegOkI;
reg		tRegHoldI;


reg			icBlkBypass;
reg			icReqLow4GB;
reg			icFaultHold;
reg			icNextFaultHold;

reg			icMsgHold;
reg			icNextMsgHold;

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
	tMemTile		= 0;
	tOutData		= 0;
	tNextTile		= 0;

	icReqLow4GB	= (tRegInAddr[47:32]==16'h0000);
	icBlkBypass	= (tRegInAddr[47:45]==3'b101) ||
		((tRegInAddr[31:29]==3'b101) && icReqLow4GB);
	
	tRegOutExc = 0;
	tRegOutTea[63:48] = 0;

	tlbAddrA	= tRegInAddr[31:0];
	tlbAddrB	= tRegOutAddr[31:0];

	if(tlbAddrB[4])
	begin
		tRegTileIxB		= tlbAddrB[15:4];
		tRegTileIxA		= tRegTileIxB+1;
	end
	else
	begin
		tRegTileIxA		= tlbAddrB[15:4];
		tRegTileIxB		= tRegTileIxA;
	end

	if(tlbAddrA[4])
	begin
		tRegTileIxD		= tlbAddrA[15:4];
		tRegTileIxC		= tRegTileIxD+1;
	end
	else
	begin
		tRegTileIxC		= tlbAddrA[15:4];
		tRegTileIxD		= tRegTileIxC;
	end

	tRamBlkAddrA=tlbAddrB[31:4];
	tRamBlkAddrB=tRamBlkAddrA+1;
	tRamMissA=0;
	tRamMissB=0;

	tRamBlkAddrC=tlbAddrA[31:4];
	tRamBlkAddrD=tRamBlkAddrC+1;

	tNextTileIxC	= tRegTileIxC;
	tNextTileIxD	= tRegTileIxD;
	tNextTileSt		= 0;
	tNextTileSrSt	= 0;
	tNextTileStWR	= 0;
	tRegOutOK		= 0;

//	tMemOutData		= 0;	//memory PC data
//	tMemAddr		= 0;	//memory PC address
//	tMemOpm			= 0;

	tMmioOutData	= 0;	//mmio data out
	tMmioAddr		= 0;	//mmio address
	tMmioOpm		= 0;
//	tMmioOE			= 0;	//mmio read
//	tMmioWR			= 0;	//mmio write

//	tlbDoLdtlb		= 0;

	tRegOkO			= 0;
	tRegOkI			= 0;
	tRegHoldO		= 0;
	tRegHoldI		= 0;
	
	icNextFaultHold	= 0;
	icNextMsgHold	= 0;

	tNextRamTileAddrA	= 0;
	tNextRamTileAddrB	= 0;
	tNextRamTileFlagA	= 0;
	tNextRamTileFlagB	= 0;
	tRegOutTea			= UV64_XX;

	tRamReqBlkAddrA=UV28_XX;
	tRamReqBlkAddrB=UV28_XX;

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
//		tRamTile = tRamTile2;
		tRamTile[255:128] = tRamTile2[255:128];
		tRamTileAddrB = tRamTile2AddrB;
		tRamTileFlagB = tRamTile2FlagB;
	end
	else
	begin
//		tRamTile = tRamTile1;
		tRamTile[255:128] = tRamTile1[255:128];
		tRamTileAddrB = tRamTile1AddrB;
		tRamTileFlagB = tRamTile1FlagB;
	end

	if(regInOE || regInWR)
	begin

// /*
`ifndef JX2_QUIET
		if(!icFaultHold && !icMsgHold)
		begin
			$display("MemTile AI=%X AO=%X O=%X ROM=%d RAM=%d/%d SRAM=%d/%d MMIO=%d",
				tRegInAddr, tRegOutAddr, tRegInOpm,
				addrIsRom,
				addrIsRamI, addrIsRamO,
				addrIsSRamI, addrIsSRamO,
				icBlkBypass);
			icNextMsgHold=1;
		end
`endif
// */

		if(regInWR)
		begin
//			$display("MemTile DI=%X", tRegInData);
		end

		if(addrIsRom)
		begin
			if(tRegOutAddr[4])
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
//			tRegOutOK = (tAccTileIx == tRegTileIx) ?
//			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
//				UMEM_OK_OK : UMEM_OK_HOLD;
				
			tRegOkO = 1;
			tRegHoldO = (tAccTileIxA != tRegTileIxA);
			
			$display("Rom(H): %X %X %X %X",
				tMemTile[255:224], tMemTile[223:192],
				tMemTile[191:160], tMemTile[159:128]);
			$display("Rom(L): %X %X %X %X",
				tMemTile[127:96], tMemTile[95:64],
				tMemTile[ 63:32], tMemTile[31: 0]);

			tOutData=tMemTile;

//			$display("Rom: Out=%X", tOutData);
		end
		else
		if(addrIsSRamO)
		begin
			if(tRegOutAddr[4])
			begin
				tMemTile[127:  0] = tSRamTile[255:128];
				tMemTile[255:128] = tSRamTile[127:  0];
			end else begin
				tMemTile[127:  0] = tSRamTile[127:  0];
				tMemTile[255:128] = tSRamTile[255:128];
			end

//			tRegOutOK = (tAccTileIxA == tRegTileIxA) ?
//				UMEM_OK_OK : UMEM_OK_HOLD;

			tRegOkO = 1;
			tRegHoldO = (tAccTileIxA != tRegTileIxA);

//			$display("RD RAM(H): %X_%X_%X_%X",
//				tMemTile[255:224], tMemTile[223:192],
//				tMemTile[191:160], tMemTile[159:128]);
//			$display("RD RAM(L): %X_%X_%X_%X",
//				tMemTile[127: 96], tMemTile[ 95: 64],
//				tMemTile[ 63: 32], tMemTile[ 31:  0]);

			tOutData=tMemTile;
		end
		else
			if(addrIsRamO)
		begin
			if(tRegOutAddr[4])
			begin
				tMemTile[127:  0] = tRamTile [255:128];
				tMemTile[255:128] = tRamTile [127:  0];
				if(tRamTileAddrA!=tRamBlkAddrB)
				begin
`ifndef JX2_QUIET
					$display("MissA(1) %X %X",
						tRamTileAddrA, tRamBlkAddrB);
`endif
					tRamMissA=1;
				end
				if(tRamTileAddrB!=tRamBlkAddrA)
				begin
`ifndef JX2_QUIET
					$display("MissB(1) %X %X",
						tRamTileAddrB, tRamBlkAddrA);
`endif
					tRamMissB=1;
				end
				tRamReqBlkAddrA=tRamBlkAddrB;
				tRamReqBlkAddrB=tRamBlkAddrA;
			end else begin
				tMemTile[127:  0] = tRamTile [127:  0];
				tMemTile[255:128] = tRamTile [255:128];
				if(tRamTileAddrA!=tRamBlkAddrA)
					tRamMissA=1;
				if(tRamTileAddrB!=tRamBlkAddrB)
					tRamMissB=1;
				tRamReqBlkAddrA=tRamBlkAddrA;
				tRamReqBlkAddrB=tRamBlkAddrB;
			end

//			tRegOutOK = (tAccTileIxA == tRegTileIxA) &&
//					!tRamMissA && !tRamMissB
//				?
//				UMEM_OK_OK : UMEM_OK_HOLD;

			tRegOkO = 1;
			tRegHoldO = (tAccTileIxA != tRegTileIxA) ||
				tRamMissA || tRamMissB;

//			$display("RD RAM(H): %X_%X_%X_%X",
//				tMemTile[255:224], tMemTile[223:192],
//				tMemTile[191:160], tMemTile[159:128]);
//			$display("RD RAM(L): %X_%X_%X_%X",
//				tMemTile[127: 96], tMemTile[ 95: 64],
//				tMemTile[ 63: 32], tMemTile[ 31:  0]);

			tOutData=tMemTile;

`ifndef JX2_QUIET
			$display("MemTile: Ram Read");
`endif
		end

		if(regInWR)
		begin
			tRegOkI = 1;
		end

		if(tRamStickyA || tRamStickyB)
		begin
`ifndef JX2_QUIET
			$display("RAM, Sticky Store IxA=%X IxB=%X",
				tRegTileIxA, tRegTileIxB);
			$display("RAM, Sticky Store AddrA=%X AddrB=%X",
				tNextRamTileAddrA, tNextRamTileAddrB);
`endif
		
			tNextTile = tRamTile;
			tNextTileIxC = tRegTileIxA;
			tNextTileIxD = tRegTileIxB;

			tNextRamTileAddrA=tRamTileAddrA;
			tNextRamTileAddrB=tRamTileAddrB;
			tNextRamTileFlagA=tRamTileFlagA;
			tNextRamTileFlagB=tRamTileFlagB;
			
			tNextTileSt = 1;
			if(regInWR)
				tRegHoldI = 1;
		end
		else
		begin
			if((addrIsSRamI || addrIsRamI) && regInWR)
			begin
				if(tRegInAddr[4])
				begin
					tNextTile[127:  0] = tRegInData[255:128];
					tNextTile[255:128] = tRegInData[127:  0];
					tNextRamTileAddrA=tRamBlkAddrD;
					tNextRamTileAddrB=tRamBlkAddrC;
				end else begin
					tNextTile[127:  0] = tRegInData[127:  0];
					tNextTile[255:128] = tRegInData[255:128];
					tNextRamTileAddrA=tRamBlkAddrC;
					tNextRamTileAddrB=tRamBlkAddrD;
				end

	//			$display("WR RAM(H): %X_%X_%X_%X",
	//				tRegInData[255:224], tRegInData[223:192],
	//				tRegInData[191:160], tRegInData[159:128]);
	//			$display("WR RAM(L): %X_%X_%X_%X",
	//				tRegInData[127: 96], tRegInData[ 95: 64],
	//				tRegInData[ 63: 32], tRegInData[ 31:  0]);
			end

			tNextTileIxC		= tRegTileIxC;
			tNextTileIxD		= tRegTileIxD;
			tNextTileSt			= addrIsRamI && regInWR;
			tNextTileSrSt		= addrIsSRamI && regInWR;
			tNextRamTileFlagA	= 1;
			tNextRamTileFlagB	= 1;
			tNextTileStWR		= regInWR;

/*
			if(addrIsSRamI)
			begin
				if(regInWR)
				begin
					tNextTileIxC = tRegTileIxC;
					tNextTileIxD = tRegTileIxD;
					tNextTileSt = addrIsRamI;
					tNextTileSrSt = addrIsSRamI;
				end
			end
			else
			if(addrIsRamI)
			begin
				if(regInWR)
				begin

`ifndef JX2_QUIET
					$display("MemTile: Ram Write");
`endif
				end
			end
*/
		end

//		if(!addrIsRom && !addrIsRamO && !addrIsSRamO)
		if(icBlkBypass)
		begin
`ifndef JX2_QUIET
			$display("MMIO A=%X DO=%X, DI=%X Opm=%X OK=%X",
			tRegInAddr[31:0],
				tRegInData[31:0], tMmioInData[31:0],
				tRegInOpm, tMmioOK);
`endif
		
			tMmioOutData	= tRegInData[31:0];
			tMmioAddr		= tRegInAddr[31:0];
			tMmioOpm		= tRegInOpm;
			tRegOutOK		= tMmioOK;
			tOutData		= { UV224_XX, tMmioInData[31:0] };

//			tRegOkI = 1;
//			if(regInWR)
//				tRegHoldI = 1;
		end

		if(!addrIsRom && !addrIsRamO && !addrIsSRamO && !icBlkBypass)
		begin
			if(regInOE)
			begin
`ifndef JX2_QUIET
				if(!icFaultHold)
					$display("Read Fault A=%X", tRegOutAddr);
`endif
				tRegHoldO = 1;
				icNextFaultHold = 1;

				tRegOutExc = 16'h8001;
				tRegOutTea[47:0] = tRegOutAddr;
			end
		end

		if(!addrIsRamI && !addrIsSRamI && !icBlkBypass)
		begin
			if(regInWR)
			begin
`ifndef JX2_QUIET
				if(!icFaultHold)
					$display("Write Fault A=%X", tRegInAddr);
`endif
				tRegHoldI = 1;
				icNextFaultHold = 1;

				tRegOutExc = 16'h8002;
				tRegOutTea[47:0] = tRegInAddr;
			end
		end

		if(!icBlkBypass)
		begin
			tRegOutOK = (tRegHoldI || tRegHoldO) ?
				UMEM_OK_HOLD :
				((tRegOkI || tRegOkO) ?
					UMEM_OK_OK : UMEM_OK_READY);
		end

		if(icNextFaultHold)
			tRegOutOK = UMEM_OK_FAULT;
	end

end

reg	tMissDoneFlag;
reg	tMissReqaFlag;
reg	tMissReqbFlag;
reg	tMissDoneaFlag;
reg	tMissDonebFlag;

always @ (posedge clock)
begin
//	tRomTile <= romTile[tRegTileIx[7:0]];
//	tRamTile <= memTile[tRegTileIx];

	tMmioOutData2	<= tMmioOutData;	//mmio data out
	tMmioAddr2		<= tMmioAddr;		//mmio address
	tMmioOpm2		<= tMmioOpm;		//mmio read

	tOutData2		<= tOutData;
	tRegOutOK2		<= tRegOutOK;
	tRegOutExc2		<= tRegOutExc;
	tRegOutTea2		<= tRegOutTea;

	tMmioInData		<= mmioInData;		//mmio data in
	tMmioOK			<= mmioOK;			//mmio OK

	tRegInAddr		<= regInAddr;		//input Address
	tRegOutAddr		<= regOutAddr;		//output Address
	tRegInData		<= regInData;		//input data (store)
	tRegInOpm		<= regInOpm;		//Operation Size/Type


	icFaultHold		<= icNextFaultHold;
	icMsgHold		<= icNextMsgHold;

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

/*
	if(!tRamStickyA && !tMissReqaFlag)
	begin
//		tRamTile[ 31:  0] <= ramTileA[tRegTileIxA[11:1]];
//		tRamTile[ 63: 32] <= ramTileB[tRegTileIxA[11:1]];
//		tRamTile[ 95: 64] <= ramTileC[tRegTileIxA[11:1]];
//		tRamTile[127: 96] <= ramTileD[tRegTileIxA[11:1]];
//		{tRamTileFlagA, tRamTileAddrA} <=
//			ramTileAdA[tRegTileIxA[11:1]];

		tRamTile2[ 31:  0] <= ramTileA[tRegTileIxA[10:1]];
		tRamTile2[ 63: 32] <= ramTileB[tRegTileIxA[10:1]];
		tRamTile2[ 95: 64] <= ramTileC[tRegTileIxA[10:1]];
		tRamTile2[127: 96] <= ramTileD[tRegTileIxA[10:1]];
		{tRamTile2FlagA, tRamTile2AddrA} <=
			ramTileAdA[tRegTileIxA[10:1]];
	end

	if(!tRamStickyB && !tMissReqbFlag)
	begin
//		tRamTile[159:128] <= ramTileE[tRegTileIxB[11:1]];
//		tRamTile[191:160] <= ramTileF[tRegTileIxB[11:1]];
//		tRamTile[223:192] <= ramTileG[tRegTileIxB[11:1]];
//		tRamTile[255:224] <= ramTileH[tRegTileIxB[11:1]];
//		{tRamTileFlagB, tRamTileAddrB} <=
//			ramTileAdB[tRegTileIxB[11:1]];

		tRamTile2[159:128] <= ramTileE[tRegTileIxB[10:1]];
		tRamTile2[191:160] <= ramTileF[tRegTileIxB[10:1]];
		tRamTile2[223:192] <= ramTileG[tRegTileIxB[10:1]];
		tRamTile2[255:224] <= ramTileH[tRegTileIxB[10:1]];
		{tRamTile2FlagB, tRamTile2AddrB} <=
			ramTileAdB[tRegTileIxB[10:1]];
	end
*/

//	tRamTile[ 31: 0] <= memTileA[tRegTileIx];
//	tRamTile[ 63:32] <= memTileB[tRegTileIx];
//	tRamTile[ 95:64] <= memTileC[tRegTileIx];
//	tRamTile[127:96] <= memTileD[tRegTileIx];

	tAccTileIxA <= tRegTileIxA;
	tAccTileIxB <= tRegTileIxB;

//	tMemOpm				<= 5'b00000;

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
//		if(!tRamMissA || tNextTileStWR)
		begin
			ramTileA[tNextTileIxC[11:1]]	<= tNextTile[ 31:  0];
			ramTileB[tNextTileIxC[11:1]]	<= tNextTile[ 63: 32];
			ramTileC[tNextTileIxC[11:1]]	<= tNextTile[ 95: 64];
			ramTileD[tNextTileIxC[11:1]]	<= tNextTile[127: 96];
			ramTileAdA[tNextTileIxC[11:1]]	<=
				{ tNextRamTileFlagA, tNextRamTileAddrA };
			tRamStickyA			<= 0;

/*
			ramTileA[tNextTileIxC[10:1]]	<= tNextTile[ 31:  0];
			ramTileB[tNextTileIxC[10:1]]	<= tNextTile[ 63: 32];
			ramTileC[tNextTileIxC[10:1]]	<= tNextTile[ 95: 64];
			ramTileD[tNextTileIxC[10:1]]	<= tNextTile[127: 96];
			ramTileAdA[tNextTileIxC[10:1]]	<=
				{ tNextRamTileFlagA, tNextRamTileAddrA };
			tRamStickyA			<= 0;
*/

`ifndef JX2_QUIET
			$display("RAM St A: Ix=%X Addr=%X",
				tNextTileIxC, tNextRamTileAddrA);

			$display("RAM St A: D=%X_%X_%X_%X",
				tNextTile[127: 96], tNextTile[ 95: 64],
				tNextTile[ 63: 32], tNextTile[ 31:  0]);
`endif
		end

		if(!tRamMissB)
//		if(!tRamMissB || tNextTileStWR)
		begin
			ramTileE[tNextTileIxD[11:1]] 	<= tNextTile[159:128];
			ramTileF[tNextTileIxD[11:1]]	<= tNextTile[191:160];
			ramTileG[tNextTileIxD[11:1]]	<= tNextTile[223:192];
			ramTileH[tNextTileIxD[11:1]]	<= tNextTile[255:224];
			ramTileAdB[tNextTileIxD[11:1]]	<=
				{ tNextRamTileFlagB, tNextRamTileAddrB };
			tRamStickyB			<= 0;

/*
			ramTileE[tNextTileIxD[10:1]] 	<= tNextTile[159:128];
			ramTileF[tNextTileIxD[10:1]]	<= tNextTile[191:160];
			ramTileG[tNextTileIxD[10:1]]	<= tNextTile[223:192];
			ramTileH[tNextTileIxD[10:1]]	<= tNextTile[255:224];
			ramTileAdB[tNextTileIxD[10:1]]	<=
				{ tNextRamTileFlagB, tNextRamTileAddrB };
			tRamStickyB			<= 0;
*/

`ifndef JX2_QUIET
			$display("RAM St B: Ix=%X Addr=%X",
				tNextTileIxD, tNextRamTileAddrB);
			$display("RAM St B: D=%X_%X_%X_%X",
				tNextTile[255:224], tNextTile[223:192],
				tNextTile[191:160], tNextTile[159:128]);
`endif
		end
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

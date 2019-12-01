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
(* ram_style="block" *) reg[31:0]		ramTileAdA[2047:0];	//RAM Address
(* ram_style="block" *) reg[31:0]		ramTileAdB[2047:0];	//RAM Address

/* verilator lint_off UNOPTFLAT */

reg[255:0]		tRomTile;
reg[255:0]		tRamTile;
reg[255:0]		tSRamTile;
reg[27:0]		tRamTileAddrA;			//RAM Address
reg[27:0]		tRamTileAddrB;			//RAM Address
reg[27:0]		tNextRamTileAddrA;		//RAM Address
reg[27:0]		tNextRamTileAddrB;		//RAM Address

reg[3:0]		tRamTileFlagA;			//RAM Flags
reg[3:0]		tRamTileFlagB;			//RAM Flags
reg[3:0]		tNextRamTileFlagA;		//RAM Flags
reg[3:0]		tNextRamTileFlagB;		//RAM Flags

reg[11:0]		tAccTileIxA;
reg[11:0]		tAccTileIxB;


reg[255:0]		tMemTile;
reg[255:0]		tOutData;

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
reg[1:0]		tRegOutOK;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;

/* verilator lint_on UNOPTFLAT */

assign		regOutData = tOutData;
assign		regOutOK = tRegOutOK;
assign		regOutExc = tRegOutExc;
assign		regOutTea = tRegOutTea;

wire		regInOE;
wire		regInWR;
assign		regInOE = regInOpm[3];
assign		regInWR = regInOpm[4];

wire		regInIsLDTLB;
assign		regInIsLDTLB = (regInOpm==UMEM_OPM_LDTLB);

wire		addrIsSRamI;
wire		addrIsSRamO;
assign		addrIsSRamI =
	(regInAddr[31:0] >= 32'h0000C000) &&
	(regInAddr[31:0] <  32'h0000E000) ;
assign		addrIsSRamO =
	(regOutAddr[31:0] >= 32'h0000C000) &&
	(regOutAddr[31:0] <  32'h0000E000) ;

wire		addrIsRom;
assign		addrIsRom =
	(regOutAddr[31:0] <  32'h00008000);

//wire			addrIsSRam;
//assign		addrIsSRam =
//	(regInAddr[28:0] <= 29'h0010_0000) &&
//	(regInAddr[14] == 1);

wire		addrIsRamI;
wire		addrIsRamO;

assign		addrIsRamI =
	(regInAddr[31:0] >= 32'h01000000) &&
	(regInAddr[31:0] <  32'h7FFFFFFF);

assign		addrIsRamO =
	(regOutAddr[31:0] >= 32'h01000000) &&
	(regOutAddr[31:0] <  32'h7FFFFFFF);

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

reg			tlbDoLdtlb;
reg			tlbLdtlbOK;

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

	icReqLow4GB	= (regInAddr[47:32]==16'h0000);
	icBlkBypass	= (regInAddr[47:45]==3'b101) ||
		((regInAddr[31:29]==3'b101) && icReqLow4GB);
	
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
	tRegOutTea[63:48] = 0;

	if(tTlbExcA[15])
	begin
		tRegOutExc = tTlbExcA;
		tRegOutTea[47:0] = regInAddr;
	end

	if(tTlbExcB[15])
	begin
		tRegOutExc = tTlbExcB;
		tRegOutTea[47:0] = regOutAddr;
	end

	if(tlbMissA)
	begin
		tRegOutExc = 16'hA001;
		tRegOutTea[47:0] = regInAddr;
	end

	if(tlbMissB)
	begin
		tRegOutExc = 16'hA001;
		tRegOutTea[47:0] = regOutAddr;
	end


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

	tlbDoLdtlb		= 0;

	tRegOkO			= 0;
	tRegOkI			= 0;
	tRegHoldO		= 0;
	tRegHoldI		= 0;
	
	icNextFaultHold	= 0;
	icNextMsgHold	= 0;

	tRamReqBlkAddrA=UV28_XX;
	tRamReqBlkAddrB=UV28_XX;

	if(regInIsLDTLB)
	begin
		$display("MemTile-LDTLB %X %X %X",
			regInAddr,
			regInData[127:64],
			regInData[ 63: 0]);
		tlbDoLdtlb	= 1;
		tRegOutOK = tlbLdtlbOK ?
			UMEM_OK_OK : UMEM_OK_HOLD;
	end
	else
		if(regInOE || regInWR)
	begin

// /*
`ifndef JX2_QUIET
		if(!icFaultHold && !icMsgHold)
		begin
			$display("MemTile AI=%X AO=%X O=%X ROM=%d RAM=%d/%d SRAM=%d/%d MMIO=%d",
				regInAddr, regOutAddr, regInOpm,
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
//			$display("MemTile DI=%X", regInData);
		end

		if(addrIsRom)
		begin
			if(regOutAddr[4])
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
			
//			$display("Rom: %X", tMemTile);

			tOutData=tMemTile;

//			$display("Rom: Out=%X", tOutData);
		end
		else
		if(addrIsSRamO)
		begin
			if(regOutAddr[4])
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
			if(regOutAddr[4])
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
				if(regInAddr[4])
				begin
					tNextTile[127:  0] = regInData[255:128];
					tNextTile[255:128] = regInData[127:  0];
					tNextRamTileAddrA=tRamBlkAddrD;
					tNextRamTileAddrB=tRamBlkAddrC;
				end else begin
					tNextTile[127:  0] = regInData[127:  0];
					tNextTile[255:128] = regInData[255:128];
					tNextRamTileAddrA=tRamBlkAddrC;
					tNextRamTileAddrB=tRamBlkAddrD;
				end

	//			$display("WR RAM(H): %X_%X_%X_%X",
	//				regInData[255:224], regInData[223:192],
	//				regInData[191:160], regInData[159:128]);
	//			$display("WR RAM(L): %X_%X_%X_%X",
	//				regInData[127: 96], regInData[ 95: 64],
	//				regInData[ 63: 32], regInData[ 31:  0]);
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
			$display("MMIO");
`endif
		
			tMmioOutData	= regInData[31:0];
			tMmioAddr		= regInAddr[31:0];
			tMmioOpm		= regInOpm;
			tRegOutOK		= mmioOK;
			tOutData		= { UV224_XX, mmioInData[31:0] };

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
					$display("Read Fault A=%X", regOutAddr);
`endif
				tRegHoldO = 1;
				icNextFaultHold = 1;

				tRegOutExc = 16'h8001;
				tRegOutTea[47:0] = regOutAddr;
			end
		end

		if(!addrIsRamI && !addrIsSRamI && !icBlkBypass)
		begin
			if(regInWR)
			begin
`ifndef JX2_QUIET
				if(!icFaultHold)
					$display("Write Fault A=%X", regInAddr);
`endif
				tRegHoldI = 1;
				icNextFaultHold = 1;

				tRegOutExc = 16'h8002;
				tRegOutTea[47:0] = regInAddr;
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

	if(!tRamStickyA && !tMissReqaFlag)
	begin
		tRamTile[ 31:  0] <= ramTileA[tRegTileIxA[11:1]];
		tRamTile[ 63: 32] <= ramTileB[tRegTileIxA[11:1]];
		tRamTile[ 95: 64] <= ramTileC[tRegTileIxA[11:1]];
		tRamTile[127: 96] <= ramTileD[tRegTileIxA[11:1]];
		{tRamTileFlagA, tRamTileAddrA} <=
			ramTileAdA[tRegTileIxA[11:1]];
	end

	if(!tRamStickyB && !tMissReqbFlag)
	begin
		tRamTile[159:128] <= ramTileE[tRegTileIxB[11:1]];
		tRamTile[191:160] <= ramTileF[tRegTileIxB[11:1]];
		tRamTile[223:192] <= ramTileG[tRegTileIxB[11:1]];
		tRamTile[255:224] <= ramTileH[tRegTileIxB[11:1]];
//		tRamTileAddrB     <= ramTileAdB[tRegTileIxB[11:1]];
		{tRamTileFlagB, tRamTileAddrB} <=
			ramTileAdB[tRegTileIxB[11:1]];
	end

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
			tRamTileAddrA		<= tRamReqBlkAddrA;
			tRamTileFlagA		<= 0;
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
				tRamTileFlagA		<= 0;
				tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
				$display("RAM MissA Dirty Ok=%d Stky=%d ReqA=%d",
					memOK, tRamStickyA, tMissReqaFlag);
`endif
			end
			else
			begin
				tRamTile[127:  0]	<= memInData;
//				tRamTileAddrA		<= tRamReqBlkAddrA;
//				tRamTileFlagA		<= 0;
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
			tRamTileAddrB		<= tRamReqBlkAddrB;
			tRamTileFlagB		<= 0;
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
				tRamTileFlagB		<= 0;
				tMemOpm				<= 5'b00000;

`ifndef JX2_QUIET
				$display("RAM MissB Dirty Ok=%d Stky=%d ReqB=%d",
					memOK, tRamStickyB, tMissReqbFlag);
`endif
			end
			else
			begin
				tRamTile[255:128]	<= memInData;
//				tRamTileAddrB		<= tRamReqBlkAddrB;
//				tRamTileFlagB		<= 0;
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

	if(tlbDoLdtlb && !tlbLdtlbOK)
	begin
		tlbBlkHiA[tlbHixA]	<= regInData[127:64];
		tlbBlkLoA[tlbHixA]	<= regInData[ 63: 0];
		tlbBlkHiB[tlbHixA]	<= tlbHdatA[127:64];
		tlbBlkLoB[tlbHixA]	<= tlbHdatA[ 63: 0];
		tlbBlkHiC[tlbHixA]	<= tlbHdatB[127:64];
		tlbBlkLoC[tlbHixA]	<= tlbHdatB[ 63: 0];
		tlbBlkHiD[tlbHixA]	<= tlbHdatC[127:64];
		tlbBlkLoD[tlbHixA]	<= tlbHdatC[ 63: 0];

		tlbBlkHiE[tlbHixA]	<= regInData[127:64];
		tlbBlkLoE[tlbHixA]	<= regInData[ 63: 0];
		tlbBlkHiF[tlbHixA]	<= tlbHdatE[127:64];
		tlbBlkLoF[tlbHixA]	<= tlbHdatE[ 63: 0];
		tlbBlkHiG[tlbHixA]	<= tlbHdatF[127:64];
		tlbBlkLoG[tlbHixA]	<= tlbHdatF[ 63: 0];
		tlbBlkHiH[tlbHixA]	<= tlbHdatG[127:64];
		tlbBlkLoH[tlbHixA]	<= tlbHdatG[ 63: 0];
		
		tlbLdtlbOK	<=	1;
	end
	else
	begin
		tlbLdtlbOK	<= 0;
	end

end


endmodule

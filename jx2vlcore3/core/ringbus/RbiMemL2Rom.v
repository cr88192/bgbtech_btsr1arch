/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
L2 Tile ROM / SRAM

Implement a 32K ROM and 8K of SRAM.

 0000.. 7FFF: ROM
 8000.. BFFF: ROM-1
   BF00.. BFFF: SRAM C-TAG
     If capability tagging exists, ROM otherwise.
 C000.. DFFF: SRAM
 E000.. FFFF: SRAM-1
10000..1FFFF: Zero Page
20000..3FFFF: RTS Page
*/

`include "ringbus/RbiDefs.v"

module RbiMemL2Rom(
	clock,			reset,
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);


input			clock;
input			reset;
	
input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[ 15:0]		tMemSeqOut;			//operation sequence
reg[ 15:0]		tMemOpmOut;			//memory operation mode
`reg_l2addr		tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data
assign		memSeqOut	= tMemSeqOut;
assign		memOpmOut	= tMemOpmOut;
assign		memAddrOut	= tMemAddrOut;
assign		memDataOut	= tMemDataOut;

reg[ 15:0]		tMemSeqReq;			//operation sequence
reg[ 15:0]		tMemOpmReq;			//memory operation mode
`reg_l2addr		tMemAddrReq;		//memory output address
`reg_tile		tMemDataReq;		//memory output data
reg				tMemDidResp;		//
reg				tNxtMemDoResp;		//
reg				tMemCcmdReq;

reg[ 15:0]		mem2SeqIn;		//operation sequence
reg[ 15:0]		mem2OpmIn;		//memory operation mode
`reg_l2addr		mem2AddrIn;		//memory input address
`reg_tile		mem2DataIn;		//memory input data


`ifdef jx2_enable_rom48k
reg[127:0]	romTileData[3071:0];
reg[11:0]	tRomBlkIx;
reg[11:0]	tRomBlkIxL;
`else
reg[127:0]	romTileData[2047:0];
reg[10:0]	tRomBlkIx;
reg[10:0]	tRomBlkIxL;
`endif

`ifdef jx2_enable_sram16k
reg[127:0]	ramTileData[1023:0];
reg[9:0]	tRamBlkIx;
reg[9:0]	tRamBlkIxL;
reg[9:0]	tRamStBlkIx;

reg[127:0]	ramTileCTag[7:0];
reg[2:0]	tRamCTagIx;
reg[2:0]	tRamCTagIxL;
reg[2:0]	tRamStCTagIx;

`else
reg[127:0]	ramTileData[ 511:0];
reg[8:0]	tRamBlkIx;
reg[8:0]	tRamBlkIxL;
reg[8:0]	tRamStBlkIx;

reg[127:0]	ramTileCTag[3:0];
reg[1:0]	tRamCTagIx;
reg[1:0]	tRamCTagIxL;
reg[1:0]	tRamStCTagIx;
`endif

reg[127:0]	tRamCTag;
reg[127:0]	tRamStCTag;

reg			tRamCBit;
reg			tRamStCBit;


reg[127:0]	tRomBlkData;
reg[127:0]	tRamBlkData;
reg[127:0]	tRamStBlkData;

reg			tRamDoSt;
reg			tRamDoStL;

reg			tRamDoStCTag;

reg			tAddrIsRom;
reg			tAddrIsRam;
reg			tAddrIsLo64k;


wire		memRingIsIdle;
wire		memRingIsResp;
wire		memRingIsLdx;
wire		memRingIsStx;
wire		memRingIsPfx;
wire		memRingIsCcmd;
assign	memRingIsIdle	= (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign	memRingIsLdx	=
	(memOpmIn[7:0] == JX2_RBI_OPM_LDX) ||
	(memOpmIn[7:0] == JX2_RBI_OPM_LDXC);
assign	memRingIsStx	=
	(memOpmIn[7:0] == JX2_RBI_OPM_STX) ||
	(memOpmIn[7:0] == JX2_RBI_OPM_STXC);
assign	memRingIsPfx	=
	(memOpmIn[7:0] == JX2_RBI_OPM_PFX) ||
	(memOpmIn[7:0] == JX2_RBI_OPM_SPX);
assign	memRingIsCcmd	= (memOpmIn[7:4] == 4'h8);
assign	memRingIsResp	=
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire		memAddrIsLo128k;
wire		memAddrIsRom;
wire		memAddrIsRam;
wire		memAddrIsZero;
wire		memAddrIsNop;
wire		memAddrIsRts;
wire		memAddrIsBad1;
wire		memAddrIsBad2;
wire		memAddrIsCTag;

`ifndef def_true
assign	memAddrIsLo128k		= (memAddrIn[31:17] == UV15_00);
assign	memAddrIsRom		= memAddrIsLo128k && (memAddrIn[16:15]==2'b00);
assign	memAddrIsRam		= memAddrIsLo128k && (memAddrIn[16:13]==4'b0110);
assign	memAddrIsZero		= memAddrIsLo128k && memAddrIn[16];
assign	memAddrIsNop		= 0;
assign	memAddrIsRts		= 0;
assign	memAddrIsBad1		= (memAddrIn[31:24] == UV8_00) && !memAddrIsLo128k;
assign	memAddrIsBad2		= (memAddrIn[31:30] != UV2_00);
`endif

`ifdef def_true
// assign	memAddrIsLo128k		= (memAddrIn[31:18] == UV14_00);
assign	memAddrIsLo128k		= 	(memAddrIn[31:18] == UV14_00) &&
								(memAddrIn[43:32] == UV12_00) &&
								(	(memAddrIn[47:44] == 4'h0) ||
									(memAddrIn[47:44] == 4'hC) ||
									(memAddrIn[47:44] == 4'hD));

`ifdef jx2_enable_rom48k
assign	memAddrIsRom		= memAddrIsLo128k &&
	((memAddrIn[17:15]==3'b000) || (memAddrIn[17:14]==4'b0010));
`else
assign	memAddrIsRom		= memAddrIsLo128k && (memAddrIn[17:15]==3'b000);
`endif

`ifdef jx2_enable_sram16k
assign	memAddrIsRam		= memAddrIsLo128k && (memAddrIn[17:14]==4'b0011);
`else
assign	memAddrIsRam		= memAddrIsLo128k && (memAddrIn[17:13]==5'b00110);
`endif

assign	memAddrIsZero		= memAddrIsLo128k && (memAddrIn[17:16]==2'b01);
assign	memAddrIsNop		= memAddrIsLo128k && (memAddrIn[17:16]==2'b10);
assign	memAddrIsRts		= memAddrIsLo128k && (memAddrIn[17:16]==2'b11);
assign	memAddrIsBad1		= (memAddrIn[31:24] == UV8_00) && !memAddrIsLo128k;
// assign	memAddrIsBad2		= (memAddrIn[31:30] != UV2_00);
assign	memAddrIsBad2		=
		(memAddrIn[31:30] != UV2_00) ||
		(memAddrIn[43:32] != UV12_00);
`endif

// `ifdef jx2_enable_memcap
assign	memAddrIsCTag		= memAddrIsLo128k &&
	(memAddrIn[17:8]==10'b0010_111111);
// `endif

reg		mem2RingIsIdle;
reg		mem2RingIsResp;
reg		mem2RingIsLdx;
reg		mem2RingIsStx;
reg		mem2RingIsPfx;
reg		mem2RingIsCcmd;

reg		mem2AddrIsLo128k;
reg		mem2AddrIsRom;
reg		mem2AddrIsRam;
reg		mem2AddrIsZero;
reg		mem2AddrIsNop;
reg		mem2AddrIsRts;
reg		mem2AddrIsCTag;

reg		mem2AddrIsBad1;
reg		mem2AddrIsBad2;

// reg[31:0]		tMemAddr;
// reg[4:0]		tMemOpm;

reg[127:0]		tMemDataIn;

initial begin
`ifdef jx2_cfg_ucrom
	$readmemh("bootrom_uc_1.txt", romTileData);
`else
`ifdef jx2_xc7s50
	$readmemh("bootrom_rbd_1.txt", romTileData);
`else
	$readmemh("bootrom_1.txt", romTileData);
`endif
`endif
end

always @*
begin
`ifdef jx2_enable_rom48k
	tRomBlkIx		= memAddrIn[15:4];
`else
	tRomBlkIx		= memAddrIn[14:4];
`endif

`ifdef jx2_enable_sram16k
	tRamBlkIx		= memAddrIn[13:4];
	tRamCTagIx		= tRamBlkIx[9:7];
`else
	tRamBlkIx		= memAddrIn[12:4];
	tRamCTagIx		= tRamBlkIx[8:7];
`endif
	
//	tMemOK			= UMEM_OK_READY;

//	tMemDataOut		= UV128_00;

	tRamStBlkData	= UV128_XX;
	tRamStBlkIx		= UV9_00;
	tRamDoSt		= 0;
	
`ifdef jx2_enable_memcap
	if(memAddrIsCTag)
	begin
`ifdef jx2_enable_sram16k
		tRamCTagIx		= memAddrIn[6:4];
`else
		tRamCTagIx		= memAddrIn[5:4];
`endif
	end
`endif
	
	tRamStCTagIx	= tRamCTagIx;
	tRamDoStCTag	= 0;

`ifdef jx2_enable_memcap
	tRamStCTag		= tRamCTag;
	tRamCBit		= tRamCTag[tRamBlkIxL[6:0]];
`else
	tRamStCTag		= 0;
	tRamCBit		= 0;
`endif


	if(memAddrIsRam && memRingIsStx)
	begin
		tRamStBlkData	= memDataIn;
		tRamStBlkIx		= tRamBlkIx;
		tRamDoSt		= 1;
	end

`ifdef jx2_enable_memcap
	if(memAddrIsCTag && memRingIsStx)
	begin
		tRamStCTag		= memDataIn;
		tRamStCTagIx	= tRamCTagIx;
		tRamDoStCTag	= 1;
	end
`endif

	tMemSeqReq		= mem2SeqIn;
	tMemOpmReq		= mem2OpmIn;
	tMemAddrReq		= mem2AddrIn;
//	tMemDataReq		= mem2DataIn;
	tMemCcmdReq		= 0;
	
//	if(mem2OpmIn[11])

	if(mem2OpmIn[11] && mem2OpmIn[8] && !mem2AddrIsLo128k)
		$display("L2ROM: TLB Missed O=%X A=%X", mem2OpmIn, mem2AddrIn);

//	if(mem2AddrIsZero)
//		$display("L2ROM: Zero O=%X A=%X", mem2OpmIn, mem2AddrIn);

	tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
//	if(tRamDoStL)
	if(mem2RingIsStx)
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKST;

`ifdef jx2_enable_memcap
	if(mem2AddrIsRam && mem2RingIsLdx && tRamCBit)
	begin
		tMemOpmReq[3:0] = 4'h8;
	end
`endif

`ifdef jx2_enable_memcap
	if(mem2AddrIsRam && mem2RingIsStx)
	begin
		tRamStCTagIx	= tRamCTagIxL;
		tRamStCTag[tRamBlkIxL[6:0]]=(mem2OpmIn[3:0]==4'hF);
		tRamDoStCTag	= 1;
	end
`endif

//	tMemOpmReq[3:0] = mem2OpmIn[11:8];
	tMemOpmReq[11:8] = mem2OpmIn[11:8];

	tMemDataReq		= UV128_00;
	if(mem2AddrIsRom)
		tMemDataReq		= tRomBlkData;
	if(mem2AddrIsRam)
		tMemDataReq		= tRamBlkData;
`ifdef jx2_enable_memcap
	if(mem2AddrIsCTag)
		tMemDataReq		= tRamCTag;
`else
	if(mem2AddrIsCTag)
		tMemDataReq		= UV128_00;
`endif
	if(mem2AddrIsNop)
		tMemDataReq		= 128'h3000_3000_3000_3000_3000_3000_3000_3000;
	if(mem2AddrIsRts)
		tMemDataReq		= 128'h3010_3010_3010_3010_3010_3010_3010_3010;
	
	if(mem2RingIsCcmd)
	begin
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
//		tMemOpmReq[3:0] = mem2OpmIn[11:8];
		tMemOpmReq[11:8] = mem2OpmIn[11:8];
		tMemCcmdReq		= 1;
	end

	if((mem2AddrIsBad1 || mem2AddrIsBad2) &&
		(mem2RingIsLdx || mem2RingIsStx || mem2RingIsPfx))
	begin
		$display("L2Rom: Skip Invalid Address S=%X O=%X A=%X D=%X",
			mem2SeqIn, mem2OpmIn, mem2AddrIn, mem2DataIn);
//		tMemOpmReq[3:0] = 4'b1011;
		tMemOpmReq[11:8] = 4'b1011;
		if(mem2OpmIn[11:8] == 4'b1111)
			tMemOpmReq[11:8] = 4'b1111;
		tMemCcmdReq		= 1;
	end

end

always @(posedge clock)
begin
	mem2SeqIn			<= memSeqIn;
	mem2OpmIn			<= memOpmIn;
	mem2AddrIn			<= memAddrIn;
	mem2DataIn			<= memDataIn;

	mem2RingIsIdle		<= memRingIsIdle;
	mem2RingIsResp		<= memRingIsResp;
	mem2RingIsLdx		<= memRingIsLdx;
	mem2RingIsStx		<= memRingIsStx;
	mem2RingIsPfx		<= memRingIsPfx;
	mem2RingIsCcmd		<= memRingIsCcmd;

	mem2AddrIsLo128k	<= memAddrIsLo128k;
//	mem2AddrIsLo128k	<= memAddrIsLo256k;
	mem2AddrIsRom		<= memAddrIsRom;
	mem2AddrIsRam		<= memAddrIsRam;
	mem2AddrIsZero		<= memAddrIsZero;
	mem2AddrIsNop		<= memAddrIsNop;
	mem2AddrIsRts		<= memAddrIsRts;
	mem2AddrIsCTag		<= memAddrIsCTag;

	mem2AddrIsBad1		<= memAddrIsBad1;
	mem2AddrIsBad2		<= memAddrIsBad2;

	tRomBlkIxL			<= tRomBlkIx;
	tRamBlkIxL			<= tRamBlkIx;
	tRamCTagIxL			<= tRamCTagIx;
	
	tRomBlkData		<= romTileData[tRomBlkIx];
	tRamBlkData		<= ramTileData[tRamBlkIx];

`ifdef jx2_enable_memcap
	tRamCTag		<= ramTileCTag[tRamCTagIx];
`else
	tRamCTag		<= 0;
`endif

	tRamDoStL		<= tRamDoSt;	
	if(tRamDoSt)
	begin
		ramTileData[tRamStBlkIx]		<= tRamStBlkData;
	end

`ifdef jx2_enable_memcap
	if(tRamDoStCTag)
	begin
		ramTileCTag[tRamStCTagIx]		<= tRamStCTag;
	end
`endif

	if((mem2AddrIsLo128k &&
			( mem2RingIsLdx || mem2RingIsStx || mem2RingIsPfx )	) ||
			tMemCcmdReq)
	begin
		tMemSeqOut		<= tMemSeqReq;
		tMemOpmOut		<= tMemOpmReq;
		tMemAddrOut		<= tMemAddrReq;
		tMemDataOut		<= tMemDataReq;
	end
	else
	begin
		tMemSeqOut		<= mem2SeqIn;
		tMemOpmOut		<= mem2OpmIn;
		tMemAddrOut		<= mem2AddrIn;
		tMemDataOut		<= mem2DataIn;
	end
end

endmodule

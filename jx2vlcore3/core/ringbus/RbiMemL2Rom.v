/*
L2 Tile ROM / SRAM

Implement a 32K ROM and 8K of SRAM.

 0000.. 7FFF: ROM
 8000.. BFFF: ROM-1
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
input [ 47:0]	memAddrIn;		//memory input address
output[ 47:0]	memAddrOut;		//memory output address
input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[ 15:0]		tMemSeqOut;			//operation sequence
reg[ 15:0]		tMemOpmOut;			//memory operation mode
reg[ 47:0]		tMemAddrOut;		//memory output address
reg[127:0]		tMemDataOut;		//memory output data
assign		memSeqOut	= tMemSeqOut;
assign		memOpmOut	= tMemOpmOut;
assign		memAddrOut	= tMemAddrOut;
assign		memDataOut	= tMemDataOut;

reg[ 15:0]		tMemSeqReq;			//operation sequence
reg[ 15:0]		tMemOpmReq;			//memory operation mode
reg[ 47:0]		tMemAddrReq;		//memory output address
reg[127:0]		tMemDataReq;		//memory output data
reg				tMemDidResp;		//
reg				tNxtMemDoResp;		//
reg				tMemCcmdReq;

reg[ 15:0]	mem2SeqIn;		//operation sequence
reg[ 15:0]	mem2OpmIn;		//memory operation mode
reg[ 47:0]	mem2AddrIn;		//memory input address
reg[127:0]	mem2DataIn;		//memory input data


reg[127:0]	romTileData[2047:0];
reg[127:0]	ramTileData[ 511:0];

reg[10:0]	tRomBlkIx;
reg[10:0]	tRomBlkIxL;
reg[8:0]	tRamBlkIx;
reg[8:0]	tRamBlkIxL;
reg[8:0]	tRamStBlkIx;

reg[127:0]	tRomBlkData;
reg[127:0]	tRamBlkData;
reg[127:0]	tRamStBlkData;

reg			tRamDoSt;
reg			tRamDoStL;

reg			tAddrIsRom;
reg			tAddrIsRam;
reg			tAddrIsLo64k;


wire		memRingIsIdle;
wire		memRingIsResp;
wire		memRingIsLdx;
wire		memRingIsStx;
wire		memRingIsCcmd;
assign	memRingIsIdle	= (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign	memRingIsLdx	= (memOpmIn[7:0] == JX2_RBI_OPM_LDX);
assign	memRingIsStx	= (memOpmIn[7:0] == JX2_RBI_OPM_STX);
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
assign	memAddrIsLo128k		= (memAddrIn[31:18] == UV14_00);
assign	memAddrIsRom		= memAddrIsLo128k && (memAddrIn[17:15]==3'b000);
assign	memAddrIsRam		= memAddrIsLo128k && (memAddrIn[17:13]==5'b00110);
assign	memAddrIsZero		= memAddrIsLo128k && (memAddrIn[17:16]==2'b01);
assign	memAddrIsNop		= memAddrIsLo128k && (memAddrIn[17:16]==2'b10);
assign	memAddrIsRts		= memAddrIsLo128k && (memAddrIn[17:16]==2'b11);
assign	memAddrIsBad1		= (memAddrIn[31:24] == UV8_00) && !memAddrIsLo128k;
assign	memAddrIsBad2		= (memAddrIn[31:30] != UV2_00);
`endif


reg		mem2RingIsIdle;
reg		mem2RingIsResp;
reg		mem2RingIsLdx;
reg		mem2RingIsStx;
reg		mem2RingIsCcmd;

reg		mem2AddrIsLo128k;
reg		mem2AddrIsRom;
reg		mem2AddrIsRam;
reg		mem2AddrIsZero;
reg		mem2AddrIsNop;
reg		mem2AddrIsRts;

reg		mem2AddrIsBad1;
reg		mem2AddrIsBad2;

// reg[31:0]		tMemAddr;
// reg[4:0]		tMemOpm;

reg[127:0]		tMemDataIn;

initial begin
`ifdef jx2_cfg_ucrom
	$readmemh("bootrom_uc_1.txt", romTileData);
`else
	$readmemh("bootrom_1.txt", romTileData);
`endif
end

always @*
begin
	tRomBlkIx		= memAddrIn[14:4];
	tRamBlkIx		= memAddrIn[12:4];
	
//	tMemOK			= UMEM_OK_READY;

//	tMemDataOut		= UV128_00;

	tRamStBlkData	= UV128_XX;
	tRamStBlkIx		= UV9_XX;
	tRamDoSt		= 0;

	if(memAddrIsRam && memRingIsStx)
	begin
		tRamStBlkData	= memDataIn;
		tRamStBlkIx		= tRamBlkIx;
		tRamDoSt		= 1;
	end

	tMemSeqReq		= mem2SeqIn;
	tMemOpmReq		= mem2OpmIn;
	tMemAddrReq		= mem2AddrIn;
//	tMemDataReq		= mem2DataIn;
	tMemCcmdReq		= 0;
	
	if(mem2OpmIn[11])
		$display("L2ROM: TLB Missed O=%X A=%X", mem2OpmIn, mem2AddrIn);

	if(mem2AddrIsZero)
		$display("L2ROM: Zero O=%X A=%X", mem2OpmIn, mem2AddrIn);

	tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
//	if(tRamDoStL)
	if(mem2RingIsStx)
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKST;

	tMemOpmReq[3:0] = mem2OpmIn[11:8];

	tMemDataReq		= UV128_00;
	if(mem2AddrIsRom)
		tMemDataReq		= tRomBlkData;
	if(mem2AddrIsRam)
		tMemDataReq		= tRamBlkData;
	if(mem2AddrIsNop)
		tMemDataReq		= 128'h3000_3000_3000_3000_3000_3000_3000_3000;
	if(mem2AddrIsRts)
		tMemDataReq		= 128'h3010_3010_3010_3010_3010_3010_3010_3010;
	
	if(mem2RingIsCcmd)
	begin
		tMemOpmReq[7:0]	= JX2_RBI_OPM_OKLD;
		tMemOpmReq[3:0] = mem2OpmIn[11:8];
		tMemCcmdReq		= 1;
	end

	if((mem2AddrIsBad1 || mem2AddrIsBad2) && (mem2RingIsLdx || mem2RingIsStx))
	begin
		$display("L2Rom: Skip Invalid Address S=%X O=%X A=%X D=%X",
			mem2SeqIn, mem2OpmIn, mem2AddrIn, mem2DataIn);
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
	mem2RingIsCcmd		<= memRingIsCcmd;

	mem2AddrIsLo128k	<= memAddrIsLo128k;
//	mem2AddrIsLo128k	<= memAddrIsLo256k;
	mem2AddrIsRom		<= memAddrIsRom;
	mem2AddrIsRam		<= memAddrIsRam;
	mem2AddrIsZero		<= memAddrIsZero;
	mem2AddrIsNop		<= memAddrIsNop;
	mem2AddrIsRts		<= memAddrIsRts;

	mem2AddrIsBad1		<= memAddrIsBad1;
	mem2AddrIsBad2		<= memAddrIsBad2;

	tRomBlkIxL			<= tRomBlkIx;
	tRamBlkIxL			<= tRamBlkIx;
	
	tRomBlkData		<= romTileData[tRomBlkIx];
	tRamBlkData		<= ramTileData[tRamBlkIx];

	tRamDoStL		<= tRamDoSt;	
	if(tRamDoSt)
	begin
		ramTileData[tRamStBlkIx]	<= tRamStBlkData;
	end

	if((mem2AddrIsLo128k && (mem2RingIsLdx || mem2RingIsStx)) || tMemCcmdReq)
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

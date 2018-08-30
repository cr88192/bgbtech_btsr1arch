/*

C-Style Cache
  128-bit Logical Tiles
  256-bit Full Duplex (Except I$)
    Moves two tiles in each direction.

 */

`include "Jx2CoreDefs.v"

module Jx2IcTileC(
	/* verilator lint_off UNUSED */
	clock, reset,
	regInPc,
	regOutPcVal, regOutPcOK,
	memPcData,
	memPcAddr, memPcOE, memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInPc;		//input PC address
output[15: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.

input [255:0]	memPcData;		//memory PC data
input [ 1: 0]	memPcOK;		//memory PC OK
output[19: 0]	memPcAddr;		//memory PC address
output			memPcOE;		//memory PC output-enable


reg[31:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.

reg[19: 0]		tMemPcAddr;		//memory PC address
reg				tMemPcOE;		//memory PC output-enable

assign	regOutPcVal = tRegOutPcVal[15:0];
assign	regOutPcOK	= tRegOutPcOK;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOE		= tMemPcOE;

reg[255:0]		icBlkData;		//Cached Block
reg[ 15:0]		icBlkAddr;		//Block Address
reg				icBlkReady;

reg				icBlkMiss;
reg[ 15:0]		icReqAddr;		//Request Address
reg				icBlkHi;
reg[3:0]		icBlkWordIx;

always @*
begin
	icBlkHi		= (icBlkAddr[0]!=regInPc[4]);
	icBlkWordIx	= {icBlkHi, regInPc[3:1]};
	icBlkMiss	=
		(((icBlkAddr[15:0]  )!=regInPc[19:4]) &&
		 ((icBlkAddr[15:0]+1)!=regInPc[19:4])) ||
		!icBlkReady;

//	icBlkMiss	=
//		(icBlkAddr[15:1]!=regInPc[19:5]) ||
//		((icBlkAddr[0] && !regInPc[4]) ||
//			(icBlkWordIx==4'b1111)) ||
//		!icBlkReady;
	icReqAddr	= regInPc[19:4];

	tRegOutPcOK = UMEM_OK_READY;
	
	if(!icBlkMiss)
	begin
		tRegOutPcOK = UMEM_OK_OK;
		case(icBlkWordIx)
			4'b0000: tRegOutPcVal=icBlkData[ 31:  0];
			4'b0001: tRegOutPcVal=icBlkData[ 47: 16];
			4'b0010: tRegOutPcVal=icBlkData[ 63: 32];
			4'b0011: tRegOutPcVal=icBlkData[ 79: 48];
			4'b0100: tRegOutPcVal=icBlkData[ 95: 64];
			4'b0101: tRegOutPcVal=icBlkData[111: 80];
			4'b0110: tRegOutPcVal=icBlkData[127: 96];
			4'b0111: tRegOutPcVal=icBlkData[143:112];

			4'b1000: tRegOutPcVal=icBlkData[159:128];
			4'b1001: tRegOutPcVal=icBlkData[175:144];
			4'b1010: tRegOutPcVal=icBlkData[191:160];
			4'b1011: tRegOutPcVal=icBlkData[207:176];
			4'b1100: tRegOutPcVal=icBlkData[223:192];
			4'b1101: tRegOutPcVal=icBlkData[239:208];
			4'b1110: tRegOutPcVal=icBlkData[255:224];
			4'b1111: tRegOutPcVal={UV16_XX, icBlkData[255:240]};
		endcase
	end
	else
	begin
		tRegOutPcOK = UMEM_OK_HOLD;
	end
end

always @ (posedge clock)
begin
	if(icBlkMiss)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			tMemPcAddr			<= 20'hXXXXX;
			tMemPcOE			<= 0;
			icBlkData[255:0]	<= memPcData[255:0];
			icBlkAddr[15:0]		<= icReqAddr[15:0];
			icBlkReady			<= 1;
		end
		else
		begin
			tMemPcAddr			<= { icReqAddr[15:0], 4'h0 };
			tMemPcOE			<= 1;
			icBlkReady			<= 0;
		end
	end
	else
	begin
		tMemPcAddr	<= 20'hXXXXX;
		tMemPcOE	<= 0;
		icBlkReady	<= icBlkReady;
	end
end

endmodule

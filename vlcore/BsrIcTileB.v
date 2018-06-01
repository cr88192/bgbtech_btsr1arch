`include "BsrCoreDefs.v"

module BsrIcTileB(
	/* verilator lint_off UNUSED */
	clock, reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInPc;		//input PC address
output[15: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 19:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[31:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.

reg[31:0]		tRegOutPcVal2;	//output PC value
reg[ 1:0]		tRegOutPcOK2;	//set if we have a valid value.

reg[19:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

assign	regOutPcVal = tRegOutPcVal[15:0];
assign	regOutPcOK	= tRegOutPcOK;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOpm	= tMemPcOpm;

reg[159:0]		icBlkData;		//Cached Block
reg[ 16:0]		icBlkAddr;		//Block Address
reg				icBlkReady;

reg				icBlkMiss;
reg[ 16:0]		icReqAddr;		//Request Address
reg				icBlkHi;
reg[2:0]		icBlkWordIx;

always @*
begin
	icBlkHi		= (icBlkAddr[0]!=regInPc[3]);
	icBlkWordIx	= {icBlkHi, regInPc[2:1]};
	icBlkMiss	=
		(((icBlkAddr[16:0]  )!=regInPc[19:3]) &&
		 ((icBlkAddr[16:0]+1)!=regInPc[19:3])) ||
		!icBlkReady;
	icReqAddr	= regInPc[19:3];

	tRegOutPcOK = UMEM_OK_READY;
	
	if(!icBlkMiss)
	begin
		tRegOutPcOK = UMEM_OK_OK;
		case(icBlkWordIx)
			3'b000: tRegOutPcVal=icBlkData[ 31:  0];
			3'b001: tRegOutPcVal=icBlkData[ 47: 16];
			3'b010: tRegOutPcVal=icBlkData[ 63: 32];
			3'b011: tRegOutPcVal=icBlkData[ 79: 48];
			3'b100: tRegOutPcVal=icBlkData[ 95: 64];
			3'b101: tRegOutPcVal=icBlkData[111: 80];
			3'b110: tRegOutPcVal=icBlkData[127: 96];
			3'b111: tRegOutPcVal=icBlkData[143:112];
		endcase
		
//		$display("BsrIcTile %X %X", regInPc, tRegOutPcVal);
	end
	else
	begin
		tRegOutPcOK = UMEM_OK_HOLD;
	end
end

always @ (posedge clock)
begin
//	tRegOutPcVal2	<= tRegOutPcVal;
//	tRegOutPcOK2	<= tRegOutPcOK;

	if(icBlkMiss)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			tMemPcAddr			<= 20'hXXXXX;
			tMemPcOpm			<= UMEM_OPM_READY;
			icBlkData[127:  0]	<= memPcData[127:0];
			icBlkData[159:128]	<= UV32_XX;
			icBlkAddr[ 16:  0]	<= icReqAddr[ 16:0];
			icBlkReady			<= 1;
		end
		else
		begin
			tMemPcAddr			<= { icReqAddr[16:0], 3'h0 };
			tMemPcOpm			<= UMEM_OPM_RD_TILE;
			icBlkReady			<= 0;
		end
	end
	else
	begin
		tMemPcAddr	<= 20'hXXXXX;
		tMemPcOpm	<= UMEM_OPM_READY;
		icBlkReady	<= icBlkReady;
	end
end

endmodule

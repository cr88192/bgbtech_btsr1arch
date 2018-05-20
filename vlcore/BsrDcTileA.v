/*
OPM: {WR, OE, Z0, S1, S0}

WR/OE:
  0, 0: Not being accessed.
  0, 1: Read
  1, 0: Write
  1, 1: Read+Write

Z0, S1, S0:
  0, 0, 0: Byte, Sign Extend
  0, 0, 1: Word, Sign Extend
  0, 1, 0: DWord, Sign Extend
  0, 1, 1: QWord, Reserved
  1, 0, 0: Byte, Zero Extend
  1, 0, 1: Word, Zero Extend
  1, 1, 0: DWord, Reserved
  1, 1, 1: Tile

 */

`include "BsrCoreDefs.v"

module BsrDcTile(
	/* verilator lint_off UNUSED */
	clock, reset,
	regInAddr,		regInOpm,
	regInData,		regOutData,
	regOutOK,

	memPcAddr,		memPcOpm,
	memPcDataI,		memPcDataO,
	memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;
input [31: 0]	regInData;		//input data (store)
output[31: 0]	regOutData;		//output data (load)
output[ 1: 0]	regOutOK;		//set if we have a valid value.

input [255:0]	memPcDataI;		//memory data in
output[255:0]	memPcDataO;		//memory data out
input [  1:0]	memPcOK;		//memory OK
output[ 19:0]	memPcAddr;		//memory address
output[  4:0]	memPcOpm;		//memory output-enable

reg[31:0]		tRegOutData;	//output PC value
reg[ 1:0]		tRegOutOK;	//set if we have a valid value.

reg[ 19: 0]		tMemPcAddr;		//memory PC address
reg[255: 0]		tMemPcDataO;	//memory data in
reg[  4: 0]		tMemPcOpm;		//memory PC output-enable

assign	regOutData	= tRegOutData;
assign	regOutOK	= tRegOutOK;

assign	memPcAddr	= tMemPcAddr;
assign	memPcDataO	= tMemPcDataO;
assign	memPcOpm	= tMemPcOpm;

reg[255:0]		icBlkData;		//Cached Block
reg[ 17:0]		icBlkAddr;		//Block Address
reg				icBlkDirty;		//Block is Dirty
reg				icBlkReady;		//Block is Dirty

reg[255:0]		icBlkDataSt;	//Cached Store Op

reg				icBlkMiss;
reg				icBlkBypass;
reg[ 15:0]		icReqAddr;		//Request Address

reg[31:0]		tRegTmpData;
reg[31:0]		tRegTmpDataSt;
reg				tIcBlkStore;

always @*
begin
	if(regInOpm[4:3]!=0)
	begin
		icBlkMiss	=
			(icBlkAddr[15:0]!=regInAddr[19:4]) ||
			(icBlkAddr[16]==icBlkAddr[3]) ||
			(icBlkAddr[17]==icBlkAddr[7]);
		icReqAddr	= regInAddr[19:4];
		icBlkBypass	= (regInAddr[15:13]==3'b111);	//MMIO
	end
	else
	begin
		icBlkMiss	= 0;
		icBlkBypass	= 0;
		icReqAddr	= 0;
	end

	tIcBlkStore = 0;
	tRegOutOK = UMEM_OK_READY;
	
	if(!icBlkMiss && !icBlkBypass && icBlkReady)
	begin
		if(regInOpm[4:3]!=0)
			tRegOutOK = UMEM_OK_OK;

		case(regInAddr[3:0])
			4'b0000: tRegTmpData=icBlkData[ 31:  0];
			4'b0001: tRegTmpData=icBlkData[ 39:  8];
			4'b0010: tRegTmpData=icBlkData[ 47: 16];
			4'b0011: tRegTmpData=icBlkData[ 55: 24];
			4'b0100: tRegTmpData=icBlkData[ 63: 32];
			4'b0101: tRegTmpData=icBlkData[ 71: 40];
			4'b0110: tRegTmpData=icBlkData[ 79: 48];
			4'b0111: tRegTmpData=icBlkData[ 87: 56];
			4'b1000: tRegTmpData=icBlkData[ 95: 64];
			4'b1001: tRegTmpData=icBlkData[103: 72];
			4'b1010: tRegTmpData=icBlkData[111: 80];
			4'b1011: tRegTmpData=icBlkData[119: 88];
			4'b1100: tRegTmpData=icBlkData[127: 96];
			4'b1101: tRegTmpData=icBlkData[135:104];
			4'b1110: tRegTmpData=icBlkData[143:112];
			4'b1111: tRegTmpData=icBlkData[151:120];
		endcase
		
		case(regInOpm[2:0])
			3'b000:
			begin
				tRegOutData = {
					tRegTmpData[7]?24'hFFFFFF:24'h000000,
					tRegTmpData[7:0] };
				tRegTmpDataSt = { tRegTmpData[31:8], regInData[7:0] };
			end

			3'b001:
			begin
				tRegOutData = {
					tRegTmpData[15]?16'hFFFF:16'h0000,
					tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[31:16], regInData[15:0] };
			end

			3'b010:
			begin
				tRegOutData = tRegTmpData;
				tRegTmpDataSt = regInData;
			end

			3'b100:
			begin
				tRegOutData = { 24'h000000, tRegTmpData[ 7:0] };
				tRegTmpDataSt = { tRegTmpData[31:8], regInData[7:0] };
			end

			3'b101:
			begin
				tRegOutData = { 16'h0000  , tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[31:16], regInData[15:0] };
			end

			default:
			begin
				tRegOutData = tRegTmpData;
				tRegTmpDataSt = regInData;
			end
		endcase

		icBlkDataSt=icBlkData;
		case(regInAddr[3:0])
			4'b0000: icBlkDataSt[ 31:  0]=tRegTmpDataSt;
			4'b0001: icBlkDataSt[ 39:  8]=tRegTmpDataSt;
			4'b0010: icBlkDataSt[ 47: 16]=tRegTmpDataSt;
			4'b0011: icBlkDataSt[ 55: 24]=tRegTmpDataSt;
			4'b0100: icBlkDataSt[ 63: 32]=tRegTmpDataSt;
			4'b0101: icBlkDataSt[ 71: 40]=tRegTmpDataSt;
			4'b0110: icBlkDataSt[ 79: 48]=tRegTmpDataSt;
			4'b0111: icBlkDataSt[ 87: 56]=tRegTmpDataSt;
			4'b1000: icBlkDataSt[ 95: 64]=tRegTmpDataSt;
			4'b1001: icBlkDataSt[103: 72]=tRegTmpDataSt;
			4'b1010: icBlkDataSt[111: 80]=tRegTmpDataSt;
			4'b1011: icBlkDataSt[119: 88]=tRegTmpDataSt;
			4'b1100: icBlkDataSt[127: 96]=tRegTmpDataSt;
			4'b1101: icBlkDataSt[135:104]=tRegTmpDataSt;
			4'b1110: icBlkDataSt[143:112]=tRegTmpDataSt;
			4'b1111: icBlkDataSt[151:120]=tRegTmpDataSt;
		endcase
		
		tIcBlkStore = regInOpm[4];
	end
	else
	if(icBlkBypass)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			tRegOutOK = UMEM_OK_OK;
			tRegOutData = memPcDataI[31:0];
		end
		else
		begin
			tRegOutOK = UMEM_OK_HOLD;
		end
	end
	else
	begin
		tRegOutOK = UMEM_OK_HOLD;
	end
end

always @ (posedge clock)
begin
	if(icBlkBypass)
	begin
		tMemPcAddr	<= regInAddr[19:0];
		tMemPcDataO	<= { UV224_XX, regInData };
		tMemPcOpm	<= tMemPcOpm;
	end
	else
	if(icBlkMiss)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			if(icBlkDirty)
			begin
				tMemPcAddr			<= { icReqAddr[15:0], 4'h0 };
				tMemPcDataO			<= icBlkData;
				tMemPcOpm			<= 5'b00000;
				icBlkDirty			<= 0;
				icBlkReady			<= 0;
			end
			else
			begin
				tMemPcAddr			<= 20'hXXXXX;
				tMemPcDataO			<= UV256_XX;
				tMemPcOpm			<= 5'b00000;
				icBlkData[255:0]	<= memPcDataI[255:0];
				icBlkAddr[15:0]		<= icReqAddr[15:0];
				icBlkAddr[16]		<= !icReqAddr[3];
				icBlkAddr[17]		<= !icReqAddr[7];
				icBlkDirty			<= 0;
				icBlkReady			<= 1;
			end
		end
		else
		begin
			if(icBlkDirty)
			begin
				tMemPcAddr			<= { icBlkAddr[15:0], 4'h0 };
				tMemPcDataO			<= icBlkData;
				tMemPcOpm			<= 5'b10111;
			end
			else
			begin
				tMemPcAddr			<= { icReqAddr[15:0], 4'h0 };
				tMemPcDataO			<= UV256_XX;
				tMemPcOpm			<= 5'b01111;
			end
		end
	end
	else
	begin
		tMemPcAddr	<= 20'hXXXXX;
		tMemPcDataO	<= UV256_XX;
		tMemPcOpm	<= 5'b00000;
		
		if(tIcBlkStore)
		begin
			icBlkData	<= icBlkDataSt;
			icBlkDirty	<= 1;
			icBlkReady	<= 1;
		end
	end
end

endmodule

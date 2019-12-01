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

`include "Jx2CoreDefs.v"

module Jx2DcTileA(
	/* verilator lint_off UNUSED */
	clock, reset,
	regInAddr,		regInOpm,
	regInData,		regOutData,
	regOutOK,

	memPcAddr,
	memPcDataI,		memPcDataO,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [63: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;
input [63: 0]	regInData;		//input data (store)
output[63: 0]	regOutData;		//output data (load)
output[ 1: 0]	regOutOK;		//set if we have a valid value.

input [255:0]	memPcDataI;		//memory data in
output[255:0]	memPcDataO;		//memory data out
input [  1:0]	memPcOK;		//memory OK
output[ 47:0]	memPcAddr;		//memory address
output[  4:0]	memPcOpm;		//memory output-enable

reg[ 47: 0]		tMemPcAddr;		//memory PC address
reg[255: 0]		tMemPcDataO;	//memory data in
reg[  4: 0]		tMemPcOpm;		//memory PC output-enable

reg[63:0]		tRegOutData2;	//output PC value
reg[ 1:0]		tRegOutOK2;	//set if we have a valid value.

assign	regOutData	= tRegOutData2;
assign	regOutOK	= tRegOutOK2;

reg[63:0]		tRegOutData;	//output PC value
reg[ 1:0]		tRegOutOK;	//set if we have a valid value.

assign	memPcAddr	= tMemPcAddr;
assign	memPcDataO	= tMemPcDataO;
assign	memPcOpm	= tMemPcOpm;

reg[63:0]		tRegInAddr;		//input address
reg[63:0]		tRegInData;		//input data (store)
reg[ 4:0]		tRegInOpm;


reg[319:0]		icBlkData;		//Cached Block
reg[ 43:0]		icBlkAddr;		//Block Address
reg				icBlkDirty;		//Block is Dirty
reg				icBlkReady;		//Block is Dirty

reg[319:0]		icBlkDataSt;	//Cached Store Op

reg				icBlkMiss;
reg				icBlkBypass;
reg[ 43:0]		icReqAddr;		//Request Address

reg				icBlkDoLdtlb;
reg				icBlkHi;

reg[63:0]		tRegTmpData;
reg[63:0]		tRegTmpDataSt;
reg				tIcBlkStore;
reg				tIcBlkStoreB;

reg				icReqLow4GB;
reg				icReqCrossEdge;
reg				icReqLow32KB;

reg			icMsgHold;
reg			icNextMsgHold;

reg			icMemIsReady;
reg			icNextMemIsReady;

reg			icReqLatch;
reg			icBypassLatch;

always @*
begin
	tRegInAddr		= regInAddr;		//input address
	tRegInData		= regInData;		//input data (store)
	tRegInOpm		= regInOpm;

	icBlkDataSt		= UV320_XX;
	tRegOutData		= UV64_XX;
//	tRegOutTea		= UV64_XX;

	icNextMemIsReady = (memPcOK==UMEM_OK_READY);

//	icCaIx=tRegInAddr[7:4];

	icReqLow4GB	= (tRegInAddr[47:32]==16'h0000);
	icBlkBypass	= (tRegInAddr[47:45]==3'b101) ||
		((tRegInAddr[31:29]==3'b101) && icReqLow4GB);

	icReqLow32KB = icReqLow4GB && (tRegInAddr[31:15]==17'h0000);

	icReqAddr		= tRegInAddr[47:4];
//	icBlkCaReady	= (icBlkCaAddr==icReqAddr);
	icBlkHi			= (icBlkAddr[0]!=tRegInAddr[4]);

	icBlkMiss	= 0;
	icReqCrossEdge = 0;

	icNextMsgHold = 0;

	tIcBlkStore = 0;
	tRegOutOK = UMEM_OK_READY;


	if((tRegInOpm[4:3]!=0) && !icBlkBypass && !icBypassLatch)
	begin
	
`ifndef JX2_MEM_QUIET
		$display("DcTileA A=%X D=%X Opm=%X",
			tRegInAddr, tRegInData, tRegInOpm);
`endif
	
		icReqCrossEdge = 0;
		case(tRegInOpm[1:0])
			2'b00:	icReqCrossEdge = 0;
			2'b01:	casez(tRegInAddr[3:0])
				4'b0zzz:	icReqCrossEdge = 0;
				4'b10zz:	icReqCrossEdge = 0;
				4'b110z:	icReqCrossEdge = 0;
				4'b1110:	icReqCrossEdge = 0;
				default:	icReqCrossEdge = 1;
			endcase
			2'b10:	casez(tRegInAddr[3:0])
				4'b0zzz:	icReqCrossEdge = 0;
				4'b10zz:	icReqCrossEdge = 0;
				4'b1100:	icReqCrossEdge = 0;
				default:	icReqCrossEdge = 1;
			endcase
			2'b11:	casez(tRegInAddr[3:0])
				4'b0zzz:	icReqCrossEdge = 0;
				4'b1000:	icReqCrossEdge = 0;
				default:	icReqCrossEdge = 1;
			endcase
		endcase

		icBlkMiss	=
//			icBlkCaReady &&
			(	(((icBlkAddr[27:0]  )!=tRegInAddr[31:4]) &&
				(((icBlkAddr[27:0]+1)!=tRegInAddr[31:4]) ||
					icReqCrossEdge)) ||
			((icBlkAddr[43:28]  )!=tRegInAddr[47:32]) ||
			!icBlkReady);

	end

/*
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
*/
	
	if(!icBlkMiss && !icBlkBypass && icBlkReady && !icBypassLatch)
	begin
		if(regInOpm[4:3]!=0)
			tRegOutOK = UMEM_OK_OK;

		case({icBlkHi, tRegInAddr[3:0]})
			5'b00000: tRegTmpData=icBlkData[ 63:  0];
			5'b00001: tRegTmpData=icBlkData[ 71:  8];
			5'b00010: tRegTmpData=icBlkData[ 79: 16];
			5'b00011: tRegTmpData=icBlkData[ 87: 24];
			5'b00100: tRegTmpData=icBlkData[ 95: 32];
			5'b00101: tRegTmpData=icBlkData[103: 40];
			5'b00110: tRegTmpData=icBlkData[111: 48];
			5'b00111: tRegTmpData=icBlkData[119: 56];
			5'b01000: tRegTmpData=icBlkData[127: 64];
			5'b01001: tRegTmpData=icBlkData[135: 72];
			5'b01010: tRegTmpData=icBlkData[143: 80];
			5'b01011: tRegTmpData=icBlkData[151: 88];
			5'b01100: tRegTmpData=icBlkData[159: 96];
			5'b01101: tRegTmpData=icBlkData[167:104];
			5'b01110: tRegTmpData=icBlkData[175:112];
			5'b01111: tRegTmpData=icBlkData[183:120];
			5'b10000: tRegTmpData=icBlkData[191:128];
			5'b10001: tRegTmpData=icBlkData[199:136];
			5'b10010: tRegTmpData=icBlkData[207:144];
			5'b10011: tRegTmpData=icBlkData[215:152];
			5'b10100: tRegTmpData=icBlkData[223:160];
			5'b10101: tRegTmpData=icBlkData[231:168];
			5'b10110: tRegTmpData=icBlkData[239:176];
			5'b10111: tRegTmpData=icBlkData[247:184];
			5'b11000: tRegTmpData=icBlkData[255:192];
			5'b11001: tRegTmpData=icBlkData[263:200];
			5'b11010: tRegTmpData=icBlkData[271:208];
			5'b11011: tRegTmpData=icBlkData[279:216];
			5'b11100: tRegTmpData=icBlkData[287:224];
			5'b11101: tRegTmpData=icBlkData[295:232];
			5'b11110: tRegTmpData=icBlkData[303:240];
			5'b11111: tRegTmpData=icBlkData[311:248];
		endcase
		
		case(tRegInOpm[2:0])
			3'b000:
			begin
				tRegOutData = {
					tRegTmpData[7]?UV56_FF:UV56_00,
					tRegTmpData[7:0] };
				tRegTmpDataSt = { tRegTmpData[63:8], tRegInData[7:0] };
			end

			3'b001:
			begin
				tRegOutData = {
					tRegTmpData[15]?UV48_FF:UV48_00,
					tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[63:16], tRegInData[15:0] };
			end

			3'b010:
			begin
				tRegOutData = {
					tRegTmpData[31]?UV32_FF:UV32_00,
					tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], tRegInData[31:0] };
			end

			3'b011:
			begin
				tRegOutData = tRegTmpData;
				tRegTmpDataSt = tRegInData;
			end

			3'b100:
			begin
				tRegOutData = { UV56_00, tRegTmpData[ 7:0] };
				tRegTmpDataSt = { tRegTmpData[63:8], tRegInData[7:0] };
			end

			3'b101:
			begin
				tRegOutData = { UV48_00, tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[63:16], tRegInData[15:0] };
			end

			3'b110:
			begin
				tRegOutData = { UV32_00, tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], tRegInData[31:0] };
			end

			default:
			begin
				tRegOutData = { UV32_00, tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], tRegInData[31:0] };
			end
		endcase

		icBlkDataSt=icBlkData;
		case({icBlkHi, tRegInAddr[3:0]})
			5'b00000: icBlkDataSt[ 63:  0]=tRegTmpDataSt;
			5'b00001: icBlkDataSt[ 71:  8]=tRegTmpDataSt;
			5'b00010: icBlkDataSt[ 79: 16]=tRegTmpDataSt;
			5'b00011: icBlkDataSt[ 87: 24]=tRegTmpDataSt;
			5'b00100: icBlkDataSt[ 95: 32]=tRegTmpDataSt;
			5'b00101: icBlkDataSt[103: 40]=tRegTmpDataSt;
			5'b00110: icBlkDataSt[111: 48]=tRegTmpDataSt;
			5'b00111: icBlkDataSt[119: 56]=tRegTmpDataSt;
			5'b01000: icBlkDataSt[127: 64]=tRegTmpDataSt;
			5'b01001: icBlkDataSt[135: 72]=tRegTmpDataSt;
			5'b01010: icBlkDataSt[143: 80]=tRegTmpDataSt;
			5'b01011: icBlkDataSt[151: 88]=tRegTmpDataSt;
			5'b01100: icBlkDataSt[159: 96]=tRegTmpDataSt;
			5'b01101: icBlkDataSt[167:104]=tRegTmpDataSt;
			5'b01110: icBlkDataSt[175:112]=tRegTmpDataSt;
			5'b01111: icBlkDataSt[183:120]=tRegTmpDataSt;
			5'b10000: icBlkDataSt[191:128]=tRegTmpDataSt;
			5'b10001: icBlkDataSt[199:136]=tRegTmpDataSt;
			5'b10010: icBlkDataSt[207:144]=tRegTmpDataSt;
			5'b10011: icBlkDataSt[215:152]=tRegTmpDataSt;
			5'b10100: icBlkDataSt[223:160]=tRegTmpDataSt;
			5'b10101: icBlkDataSt[231:168]=tRegTmpDataSt;
			5'b10110: icBlkDataSt[239:176]=tRegTmpDataSt;
			5'b10111: icBlkDataSt[247:184]=tRegTmpDataSt;
			5'b11000: icBlkDataSt[255:192]=tRegTmpDataSt;
			5'b11001: icBlkDataSt[263:200]=tRegTmpDataSt;
			5'b11010: icBlkDataSt[271:208]=tRegTmpDataSt;
			5'b11011: icBlkDataSt[279:216]=tRegTmpDataSt;
			5'b11100: icBlkDataSt[287:224]=tRegTmpDataSt;
			5'b11101: icBlkDataSt[295:232]=tRegTmpDataSt;
			5'b11110: icBlkDataSt[303:240]=tRegTmpDataSt;
			5'b11111: icBlkDataSt[311:248]=tRegTmpDataSt;
		endcase
		
		tIcBlkStore = regInOpm[4];
	end
	else
		if(icBlkBypass || icBypassLatch)
	begin
`ifndef JX2_MEM_QUIET
		$display("DcTileA Bypass A=%X D=%X Opm=%X OK=%d",
			tRegInAddr, tRegInData, tRegInOpm, memPcOK);
`endif

//		if(tRegInOpm[3] && (memPcDataI[7:0]!=0) && (memPcOK==UMEM_OK_OK))
//			$display("DcTileA: regInData=%X", memPcDataI[7:0]);

		tRegOutOK = memPcOK;
		tRegOutData = memPcDataI[63:0];
	end
	else if(regInOpm[4:3]!=0)
	begin
		tRegOutOK = UMEM_OK_HOLD;
	end
end

always @ (posedge clock)
begin
	tRegOutData2	<= tRegOutData;		//output PC value
	tRegOutOK2		<= tRegOutOK;		//set if we have a valid value.

	icMemIsReady	<= icNextMemIsReady;
	icBypassLatch	<= icBypassLatch;

`ifndef JX2_MEM_QUIET
	if(tRegOutOK!=0)
		$display("DcTileA: OK=%X", tRegOutOK);
`endif

	if(icBlkBypass || icBypassLatch)
	begin
//		if(tRegInOpm[3])
//			$display("DcTileA: regInData=%X", regInData[7:0]);

		tMemPcAddr	<= regInAddr[47:0];
		tMemPcDataO	<= { UV192_XX, regInData };
		tMemPcOpm	<= tRegInOpm;
		
		if(icBlkBypass)
		begin
			icBypassLatch <= 1;
		end
		else
			if(icNextMemIsReady)
		begin
			icBypassLatch <= 0;
		end
	end
	else
		if(icBlkMiss)
	begin
		if(!icMemIsReady && !icReqLatch)
		begin
//			$display("D$ Wait-Ready");
			tMemPcAddr			<= { icReqAddr[43:0], 4'h0 };
			tMemPcOpm			<= UMEM_OPM_READY;
			icBlkReady			<= 0;
		end
		else
			if(memPcOK==UMEM_OK_OK)
		begin
//			$display("DcTileA: Miss OK");

			if(icBlkDirty)
			begin
`ifndef JX2_MEM_QUIET
				$display("DcTileA: Miss OK, Dirty");
`endif
				tMemPcAddr			<= { icReqAddr[43:0], 4'h0 };
				tMemPcDataO			<= icBlkData[255:0];
				tMemPcOpm			<= UMEM_OPM_READY;
				icBlkDirty			<= 0;
				icBlkReady			<= 0;
				icReqLatch			<= 0;
			end
			else
			begin
`ifndef JX2_MEM_QUIET
				$display("DcTileA: Miss OK, Clean");
`endif
				tMemPcAddr			<= UV48_XX;
				tMemPcDataO			<= UV256_XX;
				tMemPcOpm			<= UMEM_OPM_READY;
				icBlkData[255:0]	<= memPcDataI[255:0];
				icBlkAddr[43:0]		<= icReqAddr[43:0];
				icBlkDirty			<= 0;
				icBlkReady			<= 1;
			end
		end
		else
		begin
			if(icBlkDirty)
			begin
`ifndef JX2_MEM_QUIET
				$display("DcTileA: Miss Dirty");
`endif

				tMemPcAddr			<= { icBlkAddr[43:0], 4'h0 };
				tMemPcDataO			<= icBlkData[255:0];
				tMemPcOpm			<= UMEM_OPM_WR_TILE;
				icReqLatch			<= 1;
			end
			else
			begin
`ifndef JX2_MEM_QUIET
				$display("DcTileA: Miss Clean");
`endif

				tMemPcAddr			<= { icReqAddr[43:0], 4'h0 };
				tMemPcDataO			<= UV256_XX;
				tMemPcOpm			<= UMEM_OPM_RD_TILE;
				icReqLatch			<= 1;
			end
		end
	end
	else
	begin
		tMemPcAddr	<= UV48_XX;
		tMemPcDataO	<= UV256_XX;
		tMemPcOpm	<= UMEM_OPM_READY;
		icReqLatch		<= 0;
		
		if(tIcBlkStore)
		begin
			icBlkData	<= icBlkDataSt;
			icBlkDirty	<= 1;
			icBlkReady	<= 1;
		end
	end
end

endmodule

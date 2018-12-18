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
  0, 1, 1: QWord, Nominal
  1, 0, 0: Byte, Zero Extend
  1, 0, 1: Word, Zero Extend
  1, 1, 0: DWord, Zero Extend
  1, 1, 1: Tile

 */

`include "Jx2CoreDefs.v"

module Jx2DcTileB(
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

input [63: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;
input [63: 0]	regInData;		//input data (store)
output[63: 0]	regOutData;		//output data (load)
output[ 1: 0]	regOutOK;		//set if we have a valid value.

input [127:0]	memPcDataI;		//memory data in
output[127:0]	memPcDataO;		//memory data out
input [  1:0]	memPcOK;		//memory OK
output[ 47:0]	memPcAddr;		//memory address
output[  4:0]	memPcOpm;		//memory output-enable

reg[63:0]		tRegOutData;	//output PC value
reg[63:0]		tRegOutData2;	//output PC value

reg[ 1:0]		tRegOutOK;	//set if we have a valid value.
reg[ 1:0]		tRegOutOK2;	//set if we have a valid value.

reg[ 47: 0]		tMemPcAddr;		//memory PC address
reg[127: 0]		tMemPcDataO;	//memory data in
reg[  4: 0]		tMemPcOpm;		//memory PC output-enable

assign	regOutData	= tRegOutData2;
assign	regOutOK	= tRegOutOK2;

assign	memPcAddr	= tMemPcAddr;
assign	memPcDataO	= tMemPcDataO;
assign	memPcOpm	= tMemPcOpm;

reg[159:0]		icBlkData;		//Cached Block
reg[ 44:0]		icBlkAddr;		//Block Address
reg				icBlkDirty;		//Block is Dirty
reg				icBlkReady;		//Block is Dirty

reg[159:0]		icBlkDataSt;	//Cached Store Op

reg				icBlkMiss;
reg				icBlkBypass;
reg[ 44:0]		icReqAddr;		//Request Address

reg[63:0]		tRegTmpData;
reg[63:0]		tRegTmpDataSt;
reg				tIcBlkStore;
reg				tIcBlkStoreB;

always @*
begin
	if(regInOpm[4:3]!=0)
	begin
		icBlkMiss	=
			(icBlkAddr[44:0]!=regInAddr[47:3]) ||
			!icBlkReady;
		icReqAddr	= regInAddr[47:3];
//		icBlkBypass	= (regInAddr[15:13]==3'b111);	//MMIO
		icBlkBypass	= (regInAddr[47:45]==3'b100);	//MMIO
	end
	else
	begin
		icBlkMiss	= 0;
		icBlkBypass	= 0;
		icReqAddr	= 0;
	end

//	tIcBlkStore = 0;
	tIcBlkStore = tIcBlkStoreB;
	tRegOutOK = UMEM_OK_READY;
	
	if(regInOpm!=0)
	begin
//		$display("Jx2DcTile A=%X O=%X D=%X acc=%d",
//			regInAddr, regInOpm, regInData,
//			{ icBlkMiss, icBlkBypass, icBlkReady} );
	end
	
	if(!icBlkMiss && !icBlkBypass && icBlkReady &&
		(regInOpm[4:3]!=0))
	begin
		tIcBlkStore = 0;

		if(regInOpm[4:3]!=0)
			tRegOutOK = UMEM_OK_OK;

		case({1'b0, regInAddr[2:0]})
			4'b0000: tRegTmpData=icBlkData[ 63:  0];
			4'b0001: tRegTmpData=icBlkData[ 71:  8];
			4'b0010: tRegTmpData=icBlkData[ 79: 16];
			4'b0011: tRegTmpData=icBlkData[ 87: 24];
			4'b0100: tRegTmpData=icBlkData[ 95: 32];
			4'b0101: tRegTmpData=icBlkData[103: 40];
			4'b0110: tRegTmpData=icBlkData[111: 48];
			4'b0111: tRegTmpData=icBlkData[119: 56];
			4'b1000: tRegTmpData=icBlkData[127: 64];
//			4'b1001: tRegTmpData=icBlkData[103: 72];
//			4'b1010: tRegTmpData=icBlkData[111: 80];
//			4'b1011: tRegTmpData=icBlkData[119: 88];
//			4'b1100: tRegTmpData=icBlkData[127: 96];
//			4'b1101: tRegTmpData=icBlkData[135:104];
//			4'b1110: tRegTmpData=icBlkData[143:112];
//			4'b1111: tRegTmpData=icBlkData[151:120];
			default:
				tRegTmpData = UV64_XX;
		endcase
		
		case(regInOpm[2:0])
			3'b000:
			begin
				tRegOutData = {
					tRegTmpData[7]?UV56_FF:UV56_00,
					tRegTmpData[7:0] };
				tRegTmpDataSt = { tRegTmpData[63:8], regInData[7:0] };
			end

			3'b001:
			begin
				tRegOutData = {
					tRegTmpData[15]?UV48_FF:UV48_00,
					tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[63:16], regInData[15:0] };
			end

			3'b010:
			begin
				tRegOutData = {
					tRegTmpData[31]?UV32_FF:UV32_00,
					tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], regInData[31:0] };
//				tRegOutData = tRegTmpData;
//				tRegTmpDataSt = regInData;
			end

			3'b011:
			begin
				tRegOutData = tRegTmpData;
				tRegTmpDataSt = regInData;
			end

			3'b100:
			begin
				tRegOutData = { UV56_00, tRegTmpData[ 7:0] };
				tRegTmpDataSt = { tRegTmpData[63:8], regInData[7:0] };
			end

			3'b101:
			begin
				tRegOutData = { UV48_00, tRegTmpData[15:0] };
				tRegTmpDataSt = { tRegTmpData[63:16], regInData[15:0] };
			end

			3'b110:
			begin
				tRegOutData = { UV32_00, tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], regInData[31:0] };
			end

			default:
			begin
				tRegOutData = { UV32_00, tRegTmpData[31:0] };
				tRegTmpDataSt = { tRegTmpData[63:32], regInData[31:0] };
//				tRegOutData = tRegTmpData;
//				tRegTmpDataSt = regInData;
			end
		endcase

		if(regInOpm[3])
		begin
//			$display("Jx2DcTile, A=%X Get=%X", regInAddr, tRegOutData);
		end

		if(regInOpm[4])
		begin
//			$display("Jx2DcTile, A=%X Set=%X", regInAddr, tRegTmpDataSt);
			tIcBlkStore = 1;
		end

		icBlkDataSt=icBlkData;
		case({1'b0, regInAddr[2:0]})
			4'b0000: icBlkDataSt[ 63:  0]=tRegTmpDataSt;
			4'b0001: icBlkDataSt[ 71:  8]=tRegTmpDataSt;
			4'b0010: icBlkDataSt[ 79: 16]=tRegTmpDataSt;
			4'b0011: icBlkDataSt[ 87: 24]=tRegTmpDataSt;
			4'b0100: icBlkDataSt[ 95: 32]=tRegTmpDataSt;
			4'b0101: icBlkDataSt[103: 40]=tRegTmpDataSt;
			4'b0110: icBlkDataSt[111: 48]=tRegTmpDataSt;
			4'b0111: icBlkDataSt[119: 56]=tRegTmpDataSt;
			4'b1000: icBlkDataSt[127: 64]=tRegTmpDataSt;
			default: begin
			end

//			4'b1001: icBlkDataSt[103: 72]=tRegTmpDataSt;
//			4'b1010: icBlkDataSt[111: 80]=tRegTmpDataSt;
//			4'b1011: icBlkDataSt[119: 88]=tRegTmpDataSt;
//			4'b1100: icBlkDataSt[127: 96]=tRegTmpDataSt;
//			4'b1101: icBlkDataSt[135:104]=tRegTmpDataSt;
//			4'b1110: icBlkDataSt[143:112]=tRegTmpDataSt;
//			4'b1111: icBlkDataSt[151:120]=tRegTmpDataSt;
		endcase
		
//		$display("Jx2DcTile: icBlkData=%X", icBlkData);
//		$display("Jx2DcTile: icBlkDataSt=%X", icBlkDataSt);
		
		tIcBlkStore = regInOpm[4];
	end
	else
	if(icBlkBypass)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			tRegOutOK = UMEM_OK_OK;
			tRegOutData = memPcDataI[63:0];
		end
		else
		begin
			tRegOutOK = UMEM_OK_HOLD;
		end
	end
	else
	begin
		if(regInOpm[4:3]!=0)
			tRegOutOK = UMEM_OK_HOLD;
	end
end

always @ (posedge clock)
begin
	tIcBlkStoreB	<= tIcBlkStore;

	tRegOutOK2		<=	tRegOutOK;
	tRegOutData2	<=	tRegOutData;

	if(icBlkBypass)
	begin
//		$display("DcTile Bypass %X %X %X",
//			regInAddr, regInOpm, regInData);
		tMemPcAddr	<= regInAddr[47:0];
		tMemPcDataO	<= { UV96_XX, regInData[31:0] };
		tMemPcOpm	<= regInOpm;
		tIcBlkStoreB	<= 0;
	end
	else
	if(icBlkMiss)
	begin
		if(memPcOK==UMEM_OK_OK)
		begin
			if(icBlkDirty)
			begin
				tMemPcAddr			<= { icReqAddr[44:0], 3'h0 };
				tMemPcDataO			<= icBlkData[127:0];
				tMemPcOpm			<= 5'b00000;
				icBlkDirty			<= 0;
				icBlkReady			<= 0;

//				$display("Jx2DcTile(A): icBlkData=%X_%X_%X_%X",
//					icBlkData[127:96], icBlkData[95:64],
//					icBlkData[63:32], icBlkData[31:0]);
			end
			else
			begin
				tMemPcAddr			<= UV48_XX;
				tMemPcDataO			<= UV128_XX;
				tMemPcOpm			<= 5'b00000;
				icBlkData[127:  0]	<= memPcDataI[127:0];
				icBlkData[159:128]	<= UV32_FF;
				icBlkAddr[44:0]		<= icReqAddr[44:0];
				icBlkDirty			<= 0;
				icBlkReady			<= 1;

//				$display("Jx2DcTile(B): Addr=%X", tMemPcAddr);
//				$display("Jx2DcTile(B): icBlkData=%X", icBlkData);
//				$display("Jx2DcTile(B): icBlkData=%X_%X_%X_%X",
//					icBlkData[127:96], icBlkData[95:64],
//					icBlkData[63:32], icBlkData[31:0]);
//				$display("Jx2DcTile(B): memPcDataI=%X_%X_%X_%X",
//					memPcDataI[127:96], memPcDataI[95:64],
//					memPcDataI[63:32], memPcDataI[31:0]);
			end
		end
		else
		begin

//			$display("Jx2DcTile(B-1): icBlkData=%X", icBlkData);

			if(icBlkDirty)
			begin
				tMemPcAddr			<= { icBlkAddr[44:0], 3'h0 };
				tMemPcDataO[127:0]	<= icBlkData[127:0];
				tMemPcOpm			<= 5'b10111;
				icBlkReady			<= 0;
			end
			else
			begin
				tMemPcAddr			<= { icReqAddr[44:0], 3'h0 };
				tMemPcDataO			<= UV128_XX;
				tMemPcOpm			<= 5'b01111;
				icBlkReady			<= 0;
			end
		end
	end
	else
	begin
		tMemPcAddr	<= UV48_XX;
		tMemPcDataO	<= UV128_XX;
		tMemPcOpm	<= 5'b00000;
		
		if(tIcBlkStore)
		begin
			icBlkData		<= icBlkDataSt;
			icBlkDirty		<= 1;
			icBlkReady		<= 1;
			tIcBlkStoreB	<= 0;

//			$display("Jx2DcTile(C): icBlkData=%X", icBlkData);
		end
		else
		begin
//			$display("Jx2DcTile(C-1): icBlkData=%X", icBlkData);
		end
	end
end

endmodule

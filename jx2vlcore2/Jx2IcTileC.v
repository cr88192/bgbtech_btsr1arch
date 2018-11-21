/*

L1 I-Cache, Simple Tile

Pulls 256 bits from L2.
An instruction crossing a tile boundary may result in a miss.

 */

`include "Jx2CoreDefs.v"

module Jx2IcTileC(
	/* verilator lint_off UNUSED */
	clock, reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [63: 0]	regInPc;		//input PC address
output[47: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 1: 0]	regOutPcStep;	//set if we have a valid value.

input [255:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 47:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[47:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.

reg[47:0]		tMemPcAddr2;		//memory PC address
reg[ 4:0]		tMemPcOpm2;		//memory PC output-enable

assign	regOutPcVal = tRegOutPcVal;
assign	regOutPcOK	= tRegOutPcOK;

assign	memPcAddr	= tMemPcAddr2;
assign	memPcOpm	= tMemPcOpm2;

reg[47:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

reg[255:0]		icBlkData;		//Cached Block
reg[ 43:0]		icBlkAddr;		//Block Address
reg				icBlkReady;

reg				icBlkMiss;
reg[ 43:0]		icReqAddr;			//Request Address
reg[ 43:0]		icReqLastAddr;		//Request Address
reg				icBlkHi;
reg[3:0]		icBlkWordIx;
reg				icBlkAddrEq;

reg				icBlkLenF14;
reg				icBlkLenF15;

reg[ 43:0]		icBlkAddrP1;	//Block Address (+1)
reg[ 43:0]		icReqAddrP1;	//Request Address (+1)

reg[1:0]		opLenA0;
reg[1:0]		opLenA1;
reg[1:0]		opLenA2;
reg[1:0]		opLenA3;
reg[1:0]		opLenA4;
reg[1:0]		opLenA5;
reg[1:0]		opLenA6;
reg[1:0]		opLenA7;
reg[1:0]		opLenA8;
reg[1:0]		opLenA9;
reg[1:0]		opLenA10;
reg[1:0]		opLenA11;
reg[1:0]		opLenA12;
reg[1:0]		opLenA13;
reg[1:0]		opLenA14;
reg[1:0]		opLenA15;

reg[1:0]		opLenA;

assign		regOutPcStep	= opLenA;

reg			icMemIsReady;
reg			icNextMemIsReady;

reg			icReqLatch;

always @*
begin
//	icBlkHi		= (icBlkAddr[0]!=regInPc[3]);
//	icBlkWordIx	= {icBlkHi, regInPc[2:1]};

	icBlkHi		= (icBlkAddr[0]!=regInPc[4]);
	icBlkWordIx	= {icBlkHi, regInPc[3:1]};
	

//	icBlkMiss	=
//		(((icBlkAddr[43:0]  )!=regInPc[47:4]) &&
//		 ((icBlkAddr[43:0]+1)!=regInPc[47:4])) ||
//		!icBlkReady ||
//		(icBlkWordIx[2:1]==2'b11);

//	icBlkMiss	=
//		(((icBlkAddr[43:0]  )!=regInPc[47:4]) &&
//		 ((icBlkAddr[43:0]+1)!=regInPc[47:4])) ||
//		!icBlkReady;

	icBlkMiss	=
		(((icBlkAddr[27: 0]  )!=regInPc[31: 4]) &&
		 ((icBlkAddr[27: 0]+1)!=regInPc[31: 4])) ||
//		 ((icBlkAddrP1[27: 0])!=regInPc[31: 4])) ||
		 ((icBlkAddr[43:28]  )!=regInPc[47:32]) ||
		!icBlkReady;

	icReqAddr	= regInPc[47:4];
	icReqAddrP1	= { regInPc[47:32], regInPc[31:4]+28'h1 };

	icBlkAddrEq = (icReqLastAddr == icReqAddr);

	tRegOutPcOK = UMEM_OK_READY;
	
	icNextMemIsReady = (memPcOK==UMEM_OK_READY);

	casez(icBlkData[15:10])
		6'b111111: opLenA0=2'b11;
		6'b111110: opLenA0=2'b10;
		6'b11110z: opLenA0=2'b10;
		default: opLenA0=2'b01; 
	endcase
	casez(icBlkData[31:26])
		6'b111111: opLenA1=2'b11;
		6'b111110: opLenA1=2'b10;
		6'b11110z: opLenA1=2'b10;
		default: opLenA1=2'b01; 
	endcase
	casez(icBlkData[47:42])
		6'b111111: opLenA2=2'b11;
		6'b111110: opLenA2=2'b10;
		6'b11110z: opLenA2=2'b10;
		default: opLenA2=2'b01; 
	endcase
	casez(icBlkData[63:58])
		6'b111111: opLenA3=2'b11;
		6'b111110: opLenA3=2'b10;
		6'b11110z: opLenA3=2'b10;
		default: opLenA3=2'b01; 
	endcase
	casez(icBlkData[79:74])
		6'b111111: opLenA4=2'b11;
		6'b111110: opLenA4=2'b10;
		6'b11110z: opLenA4=2'b10;
		default: opLenA4=2'b01; 
	endcase
	casez(icBlkData[95:90])
		6'b111111: opLenA5=2'b11;
		6'b111110: opLenA5=2'b10;
		6'b11110z: opLenA5=2'b10;
		default: opLenA5=2'b01; 
	endcase
	casez(icBlkData[111:106])
		6'b111111: opLenA6=2'b11;
		6'b111110: opLenA6=2'b10;
		6'b11110z: opLenA6=2'b10;
		default: opLenA6=2'b01; 
	endcase
	casez(icBlkData[127:122])
		6'b111111: opLenA7=2'b11;
		6'b111110: opLenA7=2'b10;
		6'b11110z: opLenA7=2'b10;
		default: opLenA7=2'b01; 
	endcase

	casez(icBlkData[143:138])
		6'b111111: opLenA8=2'b11;
		6'b111110: opLenA8=2'b10;
		6'b11110z: opLenA8=2'b10;
		default: opLenA8=2'b01; 
	endcase
	casez(icBlkData[159:154])
		6'b111111: opLenA9=2'b11;
		6'b111110: opLenA9=2'b10;
		6'b11110z: opLenA9=2'b10;
		default: opLenA9=2'b01; 
	endcase

	casez(icBlkData[175:170])
		6'b111111: opLenA10=2'b11;
		6'b111110: opLenA10=2'b10;
		6'b11110z: opLenA10=2'b10;
		default: opLenA10=2'b01; 
	endcase
	casez(icBlkData[191:186])
		6'b111111: opLenA11=2'b11;
		6'b111110: opLenA11=2'b10;
		6'b11110z: opLenA11=2'b10;
		default: opLenA11=2'b01; 
	endcase
	casez(icBlkData[207:202])
		6'b111111: opLenA12=2'b11;
		6'b111110: opLenA12=2'b10;
		6'b11110z: opLenA12=2'b10;
		default: opLenA12=2'b01; 
	endcase
	casez(icBlkData[223:218])
		6'b111111: opLenA13=2'b11;
		6'b111110: opLenA13=2'b10;
		6'b11110z: opLenA13=2'b10;
		default: opLenA13=2'b01; 
	endcase

	icBlkLenF14=0;
	icBlkLenF15=0;

	casez(icBlkData[239:234])
		6'b111111:
		begin
			opLenA14=2'b11;
			icBlkLenF14=1;
		end
		6'b111110: opLenA14=2'b10;
		6'b11110z: opLenA14=2'b10;
		default: opLenA14=2'b01; 
	endcase
	casez(icBlkData[255:250])
		6'b111111:
		begin
			opLenA15=2'b11;
			icBlkLenF15=1;
		end
		6'b111110:
		begin
			opLenA15=2'b10;
			icBlkLenF15=1;
		end
		6'b11110z:
		begin
			opLenA15=2'b10;
			icBlkLenF15=1;
		end
		default: opLenA15=2'b01; 
	endcase
	
	if((icBlkWordIx==4'b1110) && icBlkLenF14)
		icBlkMiss = 1;
	if((icBlkWordIx==4'b1111) && icBlkLenF15)
		icBlkMiss = 1;

	tRegOutPcVal	= UV48_XX;
	opLenA			= 2'bXX;

	if(!icBlkMiss)
	begin
		tRegOutPcOK = UMEM_OK_OK;
		case(icBlkWordIx)
			4'b0000: begin
				tRegOutPcVal=icBlkData[ 47:  0];
				opLenA=opLenA0;
			end
			4'b0001: begin
				tRegOutPcVal=icBlkData[ 63: 16];
				opLenA=opLenA1;
			end
			4'b0010:
			begin
				tRegOutPcVal=icBlkData[ 79: 32];
				opLenA=opLenA2;
			end
			4'b0011:
			begin
				tRegOutPcVal=icBlkData[ 95: 48];
				opLenA=opLenA3;
			end
			4'b0100:
			begin
				tRegOutPcVal=icBlkData[111: 64];
				opLenA=opLenA4;
			end
			4'b0101:
			begin
				tRegOutPcVal=icBlkData[127: 80];
				opLenA=opLenA5;
			end
			4'b0110:
			begin
				tRegOutPcVal=icBlkData[143: 96];
				opLenA=opLenA6;
			end
			4'b0111:
			begin
				tRegOutPcVal=icBlkData[159:112];
				opLenA=opLenA7;
			end
			4'b1000: begin
				tRegOutPcVal=icBlkData[175:128];
				opLenA=opLenA8;
			end
			4'b1001: begin
				tRegOutPcVal=icBlkData[191:144];
				opLenA=opLenA9;
			end
			4'b1010:
			begin
				tRegOutPcVal=icBlkData[207:160];
				opLenA=opLenA10;
			end
			4'b1011:
			begin
				tRegOutPcVal=icBlkData[223:176];
				opLenA=opLenA11;
			end
			4'b1100:
			begin
				tRegOutPcVal=icBlkData[239:192];
				opLenA=opLenA12;
			end
			4'b1101:
			begin
				tRegOutPcVal=icBlkData[255:208];
				opLenA=opLenA13;
			end
			4'b1110:
			begin
				tRegOutPcVal={UV16_XX, icBlkData[255:224]};
				opLenA=opLenA14;
			end
			4'b1111:
			begin
				tRegOutPcVal={UV32_XX, icBlkData[255:240]};
				opLenA=opLenA15;
			end
		endcase
		
//		$display("Jx2IcTile A=%X D=%X L=%d", regInPc, tRegOutPcVal, opLenA);
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

	tMemPcAddr2		<= tMemPcAddr;		//memory PC address
	tMemPcOpm2		<= tMemPcOpm;		//memory PC output-enable

	icMemIsReady	<= icNextMemIsReady;

	if(icBlkMiss)
	begin
		icReqLastAddr	<= icReqAddr;

		if(!icMemIsReady && !icReqLatch)
		begin
//			$display("I$ Wait-Ready");
			tMemPcAddr			<= { icReqAddr[43:0], 4'h0 };
			tMemPcOpm			<= UMEM_OPM_READY;
			icBlkReady			<= 0;
		end
		else
			if(memPcOK==UMEM_OK_OK)
//		if((memPcOK==UMEM_OK_OK) && icBlkAddrEq)
		begin
//			$display("I$ OK");

			tMemPcAddr			<= UV48_XX;
			tMemPcOpm			<= UMEM_OPM_READY;
//			icBlkData[127:  0]	<= memPcData[127:0];
//			icBlkData[159:128]	<= UV32_XX;
//			icBlkAddr[ 44:  0]	<= icReqAddr[ 44:0];

			icBlkData[255:  0]	<= memPcData[255:0];
			icBlkAddr[ 43:  0]	<= icReqAddr[ 43:0];
			icBlkAddrP1[ 43:  0]	<= icReqAddrP1[43:0];
			icBlkReady			<= 1;
		end
		else
		begin
//			tMemPcAddr			<= { icReqAddr[44:0], 3'h0 };
			tMemPcAddr			<= { icReqAddr[43:0], 4'h0 };
			tMemPcOpm			<= UMEM_OPM_RD_TILE;
			icBlkReady			<= 0;
			icReqLatch			<= 1;
		end
	end
	else
	begin
		tMemPcAddr	<= UV48_XX;
		tMemPcOpm	<= UMEM_OPM_READY;
		icBlkReady	<= icBlkReady;

		icReqLastAddr	<= 0;
		icReqLatch		<= 0;
	end
end

endmodule

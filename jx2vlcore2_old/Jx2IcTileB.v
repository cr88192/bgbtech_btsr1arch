`include "Jx2CoreDefs.v"

module Jx2IcTileB(
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

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 47:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[47:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.

// reg[63:0]		tRegOutPcVal2;	//output PC value
// reg[ 1:0]		tRegOutPcOK2;	//set if we have a valid value.

reg[47:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

assign	regOutPcVal = tRegOutPcVal;
assign	regOutPcOK	= tRegOutPcOK;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOpm	= tMemPcOpm;

//reg[127:0]		icBlkData;		//Cached Block
reg[159:0]		icBlkData;		//Cached Block
reg[ 44:0]		icBlkAddr;		//Block Address
reg				icBlkReady;

reg				icBlkMiss;
reg[ 44:0]		icReqAddr;		//Request Address
reg[ 44:0]		icReqLastAddr;	//Request Address
reg				icBlkHi;
reg[2:0]		icBlkWordIx;
reg				icBlkAddrEq;

reg[1:0]		opLenA0;
reg[1:0]		opLenA1;
reg[1:0]		opLenA2;
reg[1:0]		opLenA3;
reg[1:0]		opLenA4;
reg[1:0]		opLenA5;
reg[1:0]		opLenA6;
reg[1:0]		opLenA7;
reg[1:0]		opLenA;
reg				opLenF6;
reg				opLenF7;

assign		regOutPcStep	= opLenA;

always @*
begin
	icBlkHi		= (icBlkAddr[0]!=regInPc[3]);
	icBlkWordIx	= {icBlkHi, regInPc[2:1]};
//	icBlkMiss	=
//		((icBlkAddr[16:0]  )!=regInPc[19:3]) ||
//		!icBlkReady;
//	icBlkMiss	=
//		(((icBlkAddr[16:0]  )!=regInPc[19:3]) &&
//		 ((icBlkAddr[16:0]+1)!=regInPc[19:3])) ||
//		!icBlkReady;

//	icBlkMiss	=
//		(((icBlkAddr[44:0]  )!=regInPc[47:3]) &&
//		 ((icBlkAddr[44:0]+1)!=regInPc[47:3])) ||
//		!icBlkReady ||
//		(icBlkWordIx[2:1]==2'b11);

	icBlkMiss	=
		(((icBlkAddr[44:0]  )!=regInPc[47:3]) &&
		 ((icBlkAddr[44:0]+1)!=regInPc[47:3])) ||
		!icBlkReady;
	icReqAddr	= regInPc[47:3];

	icBlkAddrEq = (icReqLastAddr == icReqAddr);

	tRegOutPcOK = UMEM_OK_READY;

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

	opLenF6=0;
	opLenF7=0;

	casez(icBlkData[111:106])
		6'b111111: begin
			opLenA6=2'b11;
			opLenF6=1;
		end
		6'b111110: opLenA6=2'b10;
		6'b11110z: opLenA6=2'b10;
		default: opLenA6=2'b01; 
	endcase
	casez(icBlkData[127:122])
		6'b111111: begin
			opLenA7=2'b11;
			opLenF7=1;
		end
		6'b111110: begin
			opLenA7=2'b10;
			opLenF7=1;
		end
		6'b11110z: begin
			opLenA7=2'b10;
			opLenF7=1;
		end
		default: opLenA7=2'b01; 
	endcase

	if((icBlkWordIx[2:0]==3'b110) && opLenF6)
		icBlkMiss = 1;
	if((icBlkWordIx[2:0]==3'b111) && opLenF7)
		icBlkMiss = 1;
	
	if(!icBlkMiss)
	begin
		tRegOutPcOK = UMEM_OK_OK;
		case(icBlkWordIx)
			3'b000: begin
				tRegOutPcVal=icBlkData[ 47:  0];
				opLenA=opLenA0;
			end
			3'b001: begin
				tRegOutPcVal=icBlkData[ 63: 16];
				opLenA=opLenA1;
			end
			3'b010:
			begin
				tRegOutPcVal=icBlkData[ 79: 32];
				opLenA=opLenA2;
			end
			3'b011:
			begin
				tRegOutPcVal=icBlkData[ 95: 48];
				opLenA=opLenA3;
			end
			3'b100:
			begin
				tRegOutPcVal=icBlkData[111: 64];
				opLenA=opLenA4;
			end

			3'b101:
			begin
				tRegOutPcVal=icBlkData[127: 80];
				opLenA=opLenA5;
			end
			3'b110:
			begin
				tRegOutPcVal=icBlkData[143: 96];
				opLenA=opLenA6;
			end
			3'b111: begin
				tRegOutPcVal=icBlkData[159:112];
				opLenA=opLenA7;
			end
		endcase
		
//		$display("Jx2IcTile A=%X V=%X St=%d", regInPc, tRegOutPcVal, opLenA);
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
		icReqLastAddr	<= icReqAddr;

		if(memPcOK==UMEM_OK_OK)
		begin
			tMemPcAddr			<= UV48_XX;
			tMemPcOpm			<= UMEM_OPM_READY;
			icBlkData[127:  0]	<= memPcData[127:0];
			icBlkData[159:128]	<= UV32_XX;
			icBlkAddr[ 44:  0]	<= icReqAddr[ 44:0];
			icBlkReady			<= 1;
		end
		else
		begin
			tMemPcAddr			<= { icReqAddr[44:0], 3'h0 };
			tMemPcOpm			<= UMEM_OPM_RD_TILE;
			icBlkReady			<= 0;
		end
	end
	else
	begin
		tMemPcAddr	<= UV48_XX;
		tMemPcOpm	<= UMEM_OPM_READY;
		icBlkReady	<= icBlkReady;
		icReqLastAddr	<= 0;
	end
end

endmodule

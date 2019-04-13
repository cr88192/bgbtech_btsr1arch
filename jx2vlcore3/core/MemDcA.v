`include "CoreDefs.v"

module MemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutVal,		regInVal,
	regOutOK,

	memAddr,		memOpm,
	memDataIn,		memDataOut,
	memOK
	);

input			clock;
input			reset;

input [31: 0]	regInAddr;		//input PC address
input [ 4: 0]	regInOpm;		//input PC address

output[63: 0]	regOutVal;		//output PC value
input [63: 0]	regInVal;		//output PC value
output[ 1: 0]	regOutOK;		//set if we have a valid value.

output[ 31:0]	memAddr;		//memory PC address
output[  4:0]	memOpm;			//memory PC output-enable

input [127:0]	memDataIn;		//memory PC data
output[127:0]	memDataOut;		//memory PC data
input [  1:0]	memOK;			//memory PC OK


reg[63:0]		tRegOutVal;	//output PC value
reg[ 1:0]		tRegOutOK;	//set if we have a valid value.

assign	regOutVal		= tRegOutVal;
assign	regOutOK		= tRegOutOK;

reg[31:0]		tMemAddr;		//memory PC address
reg[ 4:0]		tMemOpm;		//memory PC output-enable
reg[127:0]		tMemDataOut;	//memory PC address

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

reg[63:0]		dcCaMemA[63:0];		//Local L1 tile memory (Even)
reg[63:0]		dcCaMemB[63:0];		//Local L1 tile memory (Odd)
reg[27:0]		dcCaAddrA[63:0];	//Local L1 tile address
reg[27:0]		dcCaAddrB[63:0];	//Local L1 tile address

reg[27:0]		tNxtAddrA;
reg[27:0]		tNxtAddrB;
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;
reg				tNxtIsMmio;

reg[63:0]		tBlkDataA;
reg[63:0]		tBlkDataB;
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;

reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;
reg				tReqIsMmio;

reg[31:0]		tInAddr;
reg[2:0]		tInByteIx;
reg[4:0]		tInOpm;
reg[63:0]		tDataIn;

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tHold;

reg[27:0]		tStBlkAddrA;
reg[27:0]		tStBlkAddrB;
reg[63:0]		tStBlkDataA;
reg[63:0]		tStBlkDataB;
reg[ 5:0]		tStBlkIxA;
reg[ 5:0]		tStBlkIxB;
reg				tDoStBlkA;
reg				tDoStBlkB;

reg[27:0]		tMiBlkAddrA;
reg[27:0]		tMiBlkAddrB;
reg[63:0]		tMiBlkDataA;
reg[63:0]		tMiBlkDataB;
reg[ 5:0]		tMiBlkIxA;
reg[ 5:0]		tMiBlkIxB;
reg				tDoMiBlkA;
reg				tDoMiBlkB;
reg				tDoMiBlk;


reg[127:0]		tBlkData;
reg[127:0]		tBlkDataW;
reg[63:0]		tBlkExData;
reg[63:0]		tBlkInData;

always @*
begin
	/* Stage A */
	if(regInAddr[3])
	begin
		tNxtAddrB=regInAddr[31:4];
		tNxtAddrA=tNxtAddrB+1;
	end else begin
		tNxtAddrA=regInAddr[31:4];
		tNxtAddrB=tNxtAddrA;
	end
	tNxtIxA=tNxtAddrA[5:0];
	tNxtIxB=tNxtAddrB[5:0];
	tNxtIsMmio=(regInAddr[31:28]==4'hA);


	/* Stage B */

	tStBlkDataA = UV64_XX;
	tStBlkDataB = UV64_XX;
	tStBlkAddrA	= UV28_XX;
	tStBlkAddrB	= UV28_XX;
	tStBlkIxA	= UV6_XX;
	tStBlkIxB	= UV6_XX;
	tDoStBlkA	= 0;
	tDoStBlkB	= 0;

	tInByteIx = tInAddr[2:0];
	
	tMissA = (tBlkAddrA != tReqAddrA);
	tMissB = (tBlkAddrB != tReqAddrB);
//	tMiss = tMissA || tMissB;
	tMiss = (tInOpm[4:3]!=0) ? (tMissA || tMissB) : 0;
	tHold = 0;
	
	tDoMiBlk	= tDoMiBlkA || tDoMiBlkB;
	
	if(tInAddr[3])
	begin
		tBlkData = { tBlkDataA, tBlkDataB };
	end else begin
		tBlkData = { tBlkDataB, tBlkDataA };
	end

	case(tInByteIx)
		3'b000: tBlkExData=tBlkData[ 63:  0];
		3'b001: tBlkExData=tBlkData[ 71:  8];
		3'b010: tBlkExData=tBlkData[ 79: 16];
		3'b011: tBlkExData=tBlkData[ 87: 24];
		3'b100: tBlkExData=tBlkData[ 95: 32];
		3'b101: tBlkExData=tBlkData[103: 40];
		3'b110: tBlkExData=tBlkData[111: 48];
		3'b111: tBlkExData=tBlkData[119: 56];
	endcase
	
	case(tInOpm[2:0])
		3'b000: tRegOutVal = {
			tBlkExData[ 7]?UV56_FF:UV56_00,
			tBlkExData[ 7:0] };
		3'b001: tRegOutVal = {
			tBlkExData[15]?UV48_FF:UV48_00,
			tBlkExData[15:0] };
		3'b010: tRegOutVal = {
			tBlkExData[31]?UV32_FF:UV32_00,
			tBlkExData[31:0] };
		3'b011: tRegOutVal = tBlkExData[63:0];
		3'b100: tRegOutVal = { UV56_00, tBlkExData[ 7:0] };
		3'b101: tRegOutVal = { UV48_00, tBlkExData[15:0] };
		3'b110: tRegOutVal = { UV32_00, tBlkExData[31:0] };
		3'b111: tRegOutVal = tBlkExData[63:0];
	endcase
	
	case(tInOpm[1:0])
		2'b00: tBlkInData = { tBlkExData [63: 8], tDataIn[ 7:0] };
		2'b01: tBlkInData = { tBlkExData [63:16], tDataIn[15:0] };
		2'b10: tBlkInData = { tBlkExData [63:32], tDataIn[31:0] };
		2'b11: tBlkInData = {                     tDataIn[63:0] };
	endcase
	
	case(tInByteIx)
		3'b000: tBlkDataW = { tBlkData[127: 64], tBlkInData                 };
		3'b001: tBlkDataW = { tBlkData[127: 72], tBlkInData, tBlkData[ 7:0] };
		3'b010: tBlkDataW = { tBlkData[127: 80], tBlkInData, tBlkData[15:0] };
		3'b011: tBlkDataW = { tBlkData[127: 88], tBlkInData, tBlkData[23:0] };
		3'b100: tBlkDataW = { tBlkData[127: 96], tBlkInData, tBlkData[31:0] };
		3'b101: tBlkDataW = { tBlkData[127:104], tBlkInData, tBlkData[39:0] };
		3'b110: tBlkDataW = { tBlkData[127:112], tBlkInData, tBlkData[47:0] };
		3'b111: tBlkDataW = { tBlkData[127:120], tBlkInData, tBlkData[55:0] };
	endcase

//	if((tInOpm[4:3]!=0)
//		tRegOutOK = (tMiss || tDoMiBlk) ? UMEM_OK_HOLD : UMEM_OK_OK;

	if((tInOpm[4:3]!=0) && !tReqIsMmio)
		tHold = (tMiss || tDoMiBlk);

	if(tDoMiBlk)
	begin
		tStBlkDataA = tMiBlkDataA;
		tStBlkDataB = tMiBlkDataB;
		tStBlkAddrA	= tMiBlkAddrA;
		tStBlkAddrB	= tMiBlkAddrB;
		tStBlkIxA	= tMiBlkIxA;
		tStBlkIxB	= tMiBlkIxB;
		tDoStBlkA	= tDoMiBlkA;
		tDoStBlkB	= tDoMiBlkB;
	end
	else
		if(tInOpm[4] && !tReqIsMmio)
	begin
		if(tInAddr[3])
		begin
//			tBlkData = { tBlkDataA, tBlkDataB };
			tStBlkDataA = tBlkDataW[127:64];
			tStBlkDataB = tBlkDataW[ 63: 0];
			tStBlkAddrA	= tReqAddrA;
			tStBlkAddrB	= tReqAddrB;
			tStBlkIxA	= tReqIxA;
			tStBlkIxB	= tReqIxB;
			tDoStBlkA	= 1;
			tDoStBlkB	= 1;
		end else begin
//			tBlkData = { tBlkDataB, tBlkDataA };
			tStBlkDataA = tBlkDataW[ 63: 0];
			tStBlkDataB = tBlkDataW[127:64];
			tStBlkAddrA	= tReqAddrA;
			tStBlkAddrB	= tReqAddrB;
			tStBlkIxA	= tReqIxA;
			tStBlkIxB	= tReqIxB;
			tDoStBlkA	= 1;
			tDoStBlkB	= 1;
		end
	end

	if(tReqIsMmio)
	begin
//		if(tMmioDone && !tMmioLatch)
//			tRegOutOK = UMEM_OK_OK;
//		else
//			tRegOutOK = UMEM_OK_HOLD;

		if(!tMmioDone || tMmioLatch)
			tHold = 1;
	end

	tRegOutOK = tHold ? UMEM_OK_HOLD : UMEM_OK_OK;

end

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;
reg		tMmioLatch;
reg		tMmioDone;


always @(posedge clock)
begin

	/* Stage A */
//	if(!tMiss)
	if(!tHold)
	begin
		tInAddr		<= regInAddr;
		tInOpm		<= regInOpm;
		tDataIn		<= regInVal;

		tReqAddrA	<= tNxtAddrA;
		tReqAddrB	<= tNxtAddrB;
		tReqIxA		<= tNxtIxA;
		tReqIxB		<= tNxtIxB;
		tReqIsMmio	<= tNxtIsMmio;
	end

	tBlkDataA	<= dcCaMemA [tReqIxA];
	tBlkDataB	<= dcCaMemB [tReqIxB];
	tBlkAddrA	<= dcCaAddrA[tReqIxA];
	tBlkAddrB	<= dcCaAddrB[tReqIxB];

	if(tDoStBlkA)
	begin
		dcCaMemA [tStBlkIxA]	<= tStBlkDataA;
		dcCaAddrA[tStBlkIxA]	<= tStBlkAddrA;
//		tDoStBlkA				<= 0;
		tDoMiBlkA				<= 0;
	end
	if(tDoStBlkB)
	begin
		dcCaMemB [tStBlkIxB]	<= tStBlkDataB;
		dcCaAddrB[tStBlkIxB]	<= tStBlkAddrB;
//		tDoStBlkB				<= 0;
		tDoMiBlkB				<= 0;
	end

	/* Stage B */
	
	if(tMiss && (tMissA || tMemLatchA) && !tMemLatchB && !tMmioLatch)
	begin

		if(tMemLatchDnA)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
			begin
				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMemLatchA)
		begin
			tMemOpm			<= UMEM_OPM_READY;
			tMemLatchDnA	<= 1;
			tMiBlkDataA		<= memDataIn[ 63: 0];
			tMiBlkDataB		<= memDataIn[127:64];
			tMiBlkAddrA		<= tReqAddrA;
			tMiBlkAddrB		<= tReqAddrA;
			tMiBlkIxA		<= tReqIxA;
			tMiBlkIxB		<= tReqIxA;
			tDoMiBlkA		<= 1;
			tDoMiBlkB		<= 1;
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMemLatchA)
		begin
			tMemOpm			<= UMEM_OPM_RD_TILE;
			tMemAddr		<= { tReqAddrA, 4'b0 };
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
			tMemLatchA		<= 1;
			tMemOpm			<= UMEM_OPM_RD_TILE;
			tMemAddr		<= { tReqAddrA, 4'b0 };
		end
		else
		begin
			tMemOpm		<= UMEM_OPM_READY;
		end

	end
	else
		if(tMiss && (tMissB || tMemLatchB) && !tMemLatchA && !tMmioLatch)
	begin

		if(tMemLatchDnB)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
			begin
				tMemLatchB		<= 0;
				tMemLatchDnB	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMemLatchB)
		begin
			tMemOpm			<= UMEM_OPM_READY;
			tMemLatchDnB	<= 1;
			tMiBlkDataA		<= memDataIn[ 63: 0];
			tMiBlkDataB		<= memDataIn[127:64];
			tMiBlkAddrA		<= tReqAddrB;
			tMiBlkAddrB		<= tReqAddrB;
			tMiBlkIxA		<= tReqIxB;
			tMiBlkIxB		<= tReqIxB;
			tDoMiBlkA		<= 1;
			tDoMiBlkB		<= 1;
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMemLatchB)
		begin
			tMemOpm			<= UMEM_OPM_RD_TILE;
			tMemAddr		<= { tReqAddrB, 4'b0 };
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
			tMemLatchB		<= 1;
			tMemOpm			<= UMEM_OPM_RD_TILE;
			tMemAddr		<= { tReqAddrB, 4'b0 };
		end
		else
		begin
			tMemOpm			<= UMEM_OPM_READY;
		end

	end
	
	if((tReqIsMmio && !tMemLatchA && !tMemLatchB) || tMmioLatch)
	begin
		tMemOpm			<= tInOpm;
		tMemAddr		<= tInAddr;
		tMemDataOut		<= { UV64_XX, tDataIn };

		if(tMmioDone)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
			begin
				tMmioLatch	<= 0;
//				tMmioDone	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMmioLatch)
		begin
			tMemOpm			<= UMEM_OPM_READY;
			tMmioDone		<= 1;
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMmioLatch)
		begin
			tMemOpm			<= tInOpm;
			tMemAddr		<= tInAddr;
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
			tMmioLatch		<= 1;
			tMemOpm			<= tInOpm;
			tMemAddr		<= tInAddr;
		end
		else
		begin
			tMemOpm			<= UMEM_OPM_READY;
		end
	end
	else
	begin
		tMmioDone	<= 0;
	end
	
end

endmodule

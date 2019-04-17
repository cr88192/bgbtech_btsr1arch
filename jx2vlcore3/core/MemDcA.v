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

reg[63:0]		tRegOutVal2;	//output PC value
reg[ 1:0]		tRegOutOK2;	//set if we have a valid value.

assign	regOutVal		= tRegOutVal2;
assign	regOutOK		= tRegOutOK2;

reg[31:0]		tMemAddr;		//memory PC address
reg[ 4:0]		tMemOpm;		//memory PC output-enable
reg[127:0]		tMemDataOut;	//memory PC address

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

reg[127:0]		dcCaMemA[63:0];		//Local L1 tile memory (Even)
reg[127:0]		dcCaMemB[63:0];		//Local L1 tile memory (Odd)
reg[ 31:0]		dcCaAddrA[63:0];	//Local L1 tile address
reg[ 31:0]		dcCaAddrB[63:0];	//Local L1 tile address

reg[27:0]		tNxtAddrA;
reg[27:0]		tNxtAddrB;
reg[ 5:0]		tNxtIxA;
reg[ 5:0]		tNxtIxB;
reg				tNxtIsMmio;

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[ 27:0]		tBlkAddrA;
reg[ 27:0]		tBlkAddrB;
reg[  3:0]		tBlkFlagA;
reg[  3:0]		tBlkFlagB;
reg				tBlkDirtyA;
reg				tBlkDirtyB;

reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
reg[ 5:0]		tReqIxA;
reg[ 5:0]		tReqIxB;
reg				tReqIsMmio;

reg[ 5:0]		tLstIxA;
reg[ 5:0]		tLstIxB;
reg[ 5:0]		tNx2IxA;
reg[ 5:0]		tNx2IxB;

reg[31:0]		tInAddr;
reg[ 2:0]		tInByteIx;
reg[ 4:0]		tInOpm;
reg[63:0]		tDataIn;

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tHold;
reg				tReqOpmNz;
reg				tLstHold;

reg[127:0]		tStBlkDataA;
reg[127:0]		tStBlkDataB;
reg[ 27:0]		tStBlkAddrA;
reg[ 27:0]		tStBlkAddrB;
reg[  3:0]		tStBlkFlagA;
reg[  3:0]		tStBlkFlagB;

reg[ 5:0]		tStBlkIxA;
reg[ 5:0]		tStBlkIxB;
reg				tDoStBlkA;
reg				tDoStBlkB;

reg[127:0]		tMiBlkDataA;
reg[127:0]		tMiBlkDataB;
reg[ 27:0]		tMiBlkAddrA;
reg[ 27:0]		tMiBlkAddrB;
reg[  5:0]		tMiBlkIxA;
reg[  5:0]		tMiBlkIxB;
reg				tDoMiBlkA;
reg				tDoMiBlkB;
reg				tDoMiBlk;


reg[127:0]		tBlkData;
reg[127:0]		tBlkDataW;
reg[ 63:0]		tBlkExData;
reg[ 63:0]		tBlkInData;


reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;
reg		tMemLatchWbA;
reg		tMemLatchWbB;
reg		tMemLatchWdA;
reg		tMemLatchWdB;
reg		tMmioLatch;
reg		tMmioDone;



always @*
begin
	/* Stage A */

	if(regInAddr[4])
	begin
		tNxtAddrB=regInAddr[31:4];
		tNxtAddrA=tNxtAddrB+1;
	end else begin
		tNxtAddrA=regInAddr[31:4];
		tNxtAddrB=tNxtAddrA+1;
	end
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
	tNxtIsMmio=(regInAddr[31:28]==4'hA);


	/* Stage B */

	tStBlkDataA = UV128_XX;
	tStBlkDataB = UV128_XX;
	tStBlkAddrA	= UV28_XX;
	tStBlkAddrB	= UV28_XX;
	tStBlkIxA	= UV6_XX;
	tStBlkIxB	= UV6_XX;
	tDoStBlkA	= 0;
	tDoStBlkB	= 0;

	tInByteIx	= tInAddr[2:0];
	tReqOpmNz	= (tInOpm[4:3]!=0);

	tBlkDirtyA	= tBlkFlagA[2];
	tBlkDirtyB	= tBlkFlagB[2];

	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));
//	tMiss = tMissA || tMissB;
//	tMiss = (tInOpm[4:3]!=0) ? (tMissA || tMissB) : 0;
	tMiss = tReqOpmNz ? (tMissA || tMissB) : 0;
	tHold = 0;
	
	tDoMiBlk	= tDoMiBlkA || tDoMiBlkB;
	
//	if(tInAddr[3])
//	begin
//		tBlkData = { tBlkDataA, tBlkDataB };
//	end else begin
//		tBlkData = { tBlkDataB, tBlkDataA };
//	end

	case(tInAddr[4:3])
		2'b00: tBlkData = tBlkDataA;
		2'b01: tBlkData = { tBlkDataB[63:0], tBlkDataA[127:64] };
		2'b10: tBlkData = tBlkDataB;
		2'b11: tBlkData = { tBlkDataA[63:0], tBlkDataB[127:64] };
	endcase

	if(tReqOpmNz)
		$display("Addr=%X tBlkData=%X", tInAddr, tBlkData);

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

	if(tReqOpmNz && !tReqIsMmio)
		tHold = (tMiss || tDoMiBlk) || (tMemLatchA || tMemLatchB);
		
	if(tHold)
	begin
		$display("L1D$ Hold, Miss=%d(%d,%d) MiBlk=%d",
			tMiss, tMissA, tMissB, tDoMiBlk);
		if(tMissA)
			$display("L1D$ MissA, Blk=%X, Req=%X", tBlkAddrA, tReqAddrA);
		if(tMissB)
			$display("L1D$ MissB, Blk=%X, Req=%X", tBlkAddrB, tReqAddrB);
	end

	if(tDoMiBlk)
	begin
		$display("L1D$ Update Missed Block");

		tStBlkDataA = tMiBlkDataA;
		tStBlkDataB = tMiBlkDataB;
		tStBlkAddrA	= tMiBlkAddrA;
		tStBlkAddrB	= tMiBlkAddrB;
		tStBlkFlagA	= { 2'b00, ~tStBlkAddrA[1:0] };
		tStBlkFlagB	= { 2'b00, ~tStBlkAddrB[1:0] };

		tStBlkIxA	= tMiBlkIxA;
		tStBlkIxB	= tMiBlkIxB;
		tDoStBlkA	= tDoMiBlkA;
		tDoStBlkB	= tDoMiBlkB;
	end
	else
		if(tInOpm[4] && !tReqIsMmio)
	begin

		$display("L1D$ Do Write Block A=%X D=%X", tInAddr, tBlkDataW);

		tStBlkAddrA	= tReqAddrA;
		tStBlkAddrB	= tReqAddrB;
		tStBlkFlagA	= { 2'b01, ~tStBlkAddrA[1:0] };
		tStBlkFlagB	= { 2'b01, ~tStBlkAddrB[1:0] };
		tStBlkIxA	= tReqIxA;
		tStBlkIxB	= tReqIxB;
		tDoStBlkA	= 0;
		tDoStBlkB	= 0;

		case(tInAddr[4:3])
			2'b00: begin
				tStBlkDataA = tBlkDataW;
				tDoStBlkA	= 1;
			end
			2'b01: begin
				tStBlkDataA = { tBlkDataW[ 63: 0], tBlkDataA[ 63: 0] };
				tStBlkDataB = { tBlkDataB[127:64], tBlkDataW[127:64] };
				tDoStBlkA	= 1;
				tDoStBlkB	= 1;
			end
			2'b10: begin
				tStBlkDataB = tBlkDataW;
				tDoStBlkB	= 1;
			end
			2'b11: begin
				tStBlkDataB = { tBlkDataW[ 63: 0], tBlkDataB[ 63: 0] };
				tStBlkDataA = { tBlkDataA[127:64], tBlkDataW[127:64] };
				tDoStBlkA	= 1;
				tDoStBlkB	= 1;
			end
		endcase
	end

	if(tReqIsMmio)
	begin
		$display("MMIO");
	
//		if(tMmioDone && !tMmioLatch)
//			tRegOutOK = UMEM_OK_OK;
//		else
//			tRegOutOK = UMEM_OK_HOLD;

		if(!tMmioDone || tMmioLatch)
			tHold = 1;
	end

//	tRegOutOK = tReqOpmNz ? (tHold ? UMEM_OK_HOLD : UMEM_OK_OK) : UMEM_OK_READY;
	tRegOutOK = tHold ? UMEM_OK_HOLD : (tReqOpmNz ? UMEM_OK_OK : UMEM_OK_READY);

	tNx2IxA		= tLstHold ? tReqIxA : tNxtIxA;
	tNx2IxB		= tLstHold ? tReqIxB : tNxtIxB;
end

always @(posedge clock)
begin

	/* Stage A */

	tLstHold	<= tHold;

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
//		tLstIxA		<= tNxtIxA;
//		tLstIxB		<= tNxtIxB;
	end

	tBlkDataA	<= dcCaMemA [tNx2IxA];
	tBlkDataB	<= dcCaMemB [tNx2IxB];
	{ tBlkFlagA, tBlkAddrA }	<= dcCaAddrA[tNx2IxA];
	{ tBlkFlagB, tBlkAddrB }	<= dcCaAddrB[tNx2IxB];

	if(tDoStBlkA)
	begin
		if(tStBlkAddrA[0])
			$display("L1D$, DoStBlkA: Even/Odd Mismatch");

		$display("L1D$, DoStBlkA(%X), Data=%X", tStBlkIxA, tStBlkDataA);
	
		dcCaMemA [tStBlkIxA]	<= tStBlkDataA;
		dcCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };
		tDoMiBlkA				<= 0;
	end

	if(tDoStBlkB)
	begin
		if(tStBlkAddrA[0])
			$display("L1D$, DoStBlkA: Even/Odd Mismatch");

		$display("L1D$, DoStBlkB(%X), Data=%X", tStBlkIxB, tStBlkDataB);

		dcCaMemB [tStBlkIxB]	<= tStBlkDataB;
		dcCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };
		tDoMiBlkB				<= 0;
	end


	/* Stage B */
	
	if(((tMiss && tMissA) || tMemLatchA) && !tMemLatchB && !tMmioLatch)
	begin

		$display("L1D$ MissA, Miss=%d Latch=%d OK=%d Dn=%d Wb=%d",
			tMissA, tMemLatchA, memOK, tMemLatchDnA, tMemLatchWbA);

		if(tMemLatchDnA)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
			begin
				$display("L1D$: MissA Done");

				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
				tMemLatchWbA	<= 0;
				tMemLatchWdA	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMemLatchA)
		begin
			tMemOpm			<= UMEM_OPM_READY;
			
			if(!tBlkDirtyA || tMemLatchWdA)
			begin
				$display("L1D$: MissA Dn memDataIn=%X", memDataIn);
			
				tMemLatchDnA	<= 1;
				tMiBlkDataA		<= memDataIn;
				tMiBlkAddrA		<= tReqAddrA;
				tMiBlkIxA		<= tReqIxA;
				tDoMiBlkA		<= 1;
			end
			else if(tBlkDirtyA)
			begin
				tMemLatchWbA	<= 1;
			end
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMemLatchA)
		begin
			tMemOpm			<= tMemOpm;
			tMemAddr		<= tMemAddr;
			tMemDataOut		<= tMemDataOut;
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
			if(tBlkDirtyA && !tMemLatchWbA)
			begin
				$display("L1D$ MissA ReadySt");
				$display("L1D$ MissA, Write=%X", tBlkDataA);
			
				tMemLatchA		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrA, 4'b0 };
				tMemDataOut		<= tBlkDataA;
				tMemLatchWdA	<= 0;
			end
			else
			begin
				$display("L1D$ MissA ReadyLd");

				tMemLatchA		<= 1;
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemAddr		<= { tReqAddrA, 4'b0 };
				tMemDataOut		<= UV128_XX;
				tMemLatchWdA	<= tMemLatchWbA;
			end
		end
		else
		begin
			tMemOpm		<= UMEM_OPM_READY;
		end

	end
	else
		if(((tMiss && tMissB) || tMemLatchB) && !tMemLatchA && !tMmioLatch)
	begin

		$display("L1D$ MissB, Miss=%d Latch=%d", tMissB, tMemLatchB);

		if(tMemLatchDnB)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
			begin
				$display("L1D$: MissB Done");

				tMemLatchB		<= 0;
				tMemLatchDnB	<= 0;
				tMemLatchWbB	<= 0;
				tMemLatchWdB	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMemLatchB)
		begin
			tMemOpm			<= UMEM_OPM_READY;

			if(!tBlkDirtyB || tMemLatchWdB)
			begin
				$display("L1D$: MissB Dn memDataIn=%X", memDataIn);

				tMemLatchDnB	<= 1;
				tMiBlkDataB		<= memDataIn;
				tMiBlkAddrB		<= tReqAddrB;
				tMiBlkIxB		<= tReqIxB;
				tDoMiBlkB		<= 1;
			end
			else if(tBlkDirtyB)
			begin
				tMemLatchWbB	<= 1;
			end
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMemLatchB)
		begin
			tMemOpm			<= tMemOpm;
			tMemAddr		<= tMemAddr;
			tMemDataOut		<= tMemDataOut;
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
			if(tBlkDirtyB && !tMemLatchWbB)
			begin
				$display("L1D$ MissB ReadySt");
				$display("L1D$ MissB, Write=%X", tBlkDataB);

				tMemLatchB		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrB, 4'b0 };
				tMemDataOut		<= tBlkDataB;
				tMemLatchWdB	<= 0;
			end
			else
			begin
				$display("L1D$ MissB ReadyLd");

				tMemLatchB		<= 1;
				tMemOpm			<= UMEM_OPM_RD_TILE;
				tMemAddr		<= { tReqAddrB, 4'b0 };
				tMemDataOut		<= UV128_XX;
				tMemLatchWdB	<= tMemLatchWbB;
			end
		end
		else
		begin
			tMemOpm			<= UMEM_OPM_READY;
		end

	end else if(!tReqIsMmio && (tInOpm[4:3]!=0))
	begin
		if(tMissA || tMissB)
			$display("L1D$ Sticky Miss, %d %d", tMissA, tMissB);
		if(tMemLatchA || tMemLatchB)
			$display("L1D$ Sticky Latch, %d %d", tMemLatchA, tMemLatchB);

		tMemLatchDnA	<= 0;
		tMemLatchWbA	<= 0;
		tMemLatchWdA	<= 0;

		tMemLatchDnB	<= 0;
		tMemLatchWbB	<= 0;
		tMemLatchWdB	<= 0;

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

	tRegOutVal2		<= tRegOutVal;	//output PC value
	tRegOutOK2		<= tRegOutOK;	//set if we have a valid value.
	
end

endmodule

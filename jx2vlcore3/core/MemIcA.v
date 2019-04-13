module MemIcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInPc;		//input PC address
output[63: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 1: 0]	regOutPcStep;	//PC step (Normal Op)

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 31:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[63:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 1: 0]		tRegOutPcStep;	//PC step (Normal Op)

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOpm	= tMemPcOpm;

reg[31:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

reg[63:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
reg[63:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
reg[27:0]		icCaAddrA[63:0];	//Local L1 tile address
reg[27:0]		icCaAddrB[63:0];	//Local L1 tile address

reg[27:0]		tNxtAddrA;
reg[27:0]		tNxtAddrB;
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;

reg[63:0]		tBlkDataA;
reg[63:0]		tBlkDataB;
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;

reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;

reg[31:0]		tInAddr;
reg[1:0]		tInWordIx;

reg				tMissA;
reg				tMissB;
reg				tMiss;

reg[127:0]		tBlkData;

reg[2:0]		opLenA0;
reg[2:0]		opLenA1;
reg[2:0]		opLenA2;
reg[2:0]		opLenA3;

always @*
begin
	/* Stage A */
	if(regInPc[3])
	begin
		tNxtAddrB=regInPc[31:4];
		tNxtAddrA=tNxtAddrB+1;
	end else begin
		tNxtAddrA=regInPc[31:4];
		tNxtAddrB=tNxtAddrA;
	end
	tNxtIxA=tNxtAddrA[5:0];
	tNxtIxB=tNxtAddrB[5:0];


	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	
	tMissA = (tBlkAddrA != tReqAddrA);
	tMissB = (tBlkAddrB != tReqAddrB);
	tMiss = tMissA || tMissB;
	
	if(tInAddr[3])
	begin
		tBlkData = { tBlkDataA, tBlkDataB };
	end else begin
		tBlkData = { tBlkDataB, tBlkDataA };
	end

	casez(tBlkData[15:10])
		6'b11z111:	opLenA0=3'b011;
		6'b11z110:	opLenA0=3'b010;
		6'b11z101:	opLenA0=3'b110;
		6'b11z100:	opLenA0=3'b010;
		default:	opLenA0=3'b001; 
	endcase
	casez(tBlkData[31:26])
		6'b11z111: opLenA1=3'b011;
		6'b11z110: opLenA1=3'b010;
		6'b11z101: opLenA1=3'b110;
		6'b11z100: opLenA1=3'b010;
		default:   opLenA1=3'b001; 
	endcase
	casez(tBlkData[47:42])
		6'b11z111: opLenA2=3'b011;
		6'b11z110: opLenA2=3'b010;
		6'b11z101: opLenA2=3'b110;
		6'b11z100: opLenA2=3'b010;
		default:   opLenA2=3'b001; 
	endcase
	casez(tBlkData[63:58])
		6'b11z111: opLenA3=3'b011;
		6'b11z110: opLenA3=3'b010;
		6'b11z101: opLenA3=3'b110;
		6'b11z100: opLenA3=3'b010;
		default:   opLenA3=3'b001; 
	endcase

	case(tInWordIx)
		2'b00: begin
			tRegOutPcVal=tBlkData[ 63:  0];
			tRegOutPcStep=opLenA0[1:0];
		end
		2'b01: begin
			tRegOutPcVal=tBlkData[ 79: 16];
			tRegOutPcStep=opLenA1[1:0];
		end
		2'b10: begin
			tRegOutPcVal=tBlkData[ 95: 32];
			tRegOutPcStep=opLenA2[1:0];
		end
		2'b11: begin
			tRegOutPcVal=tBlkData[111: 48];
			tRegOutPcStep=opLenA3[1:0];
		end
	endcase
	
	tRegOutPcOK = tMiss ? UMEM_OK_HOLD : UMEM_OK_OK;

end

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;

reg[127:0]		tStBlkData;
reg[27:0]		tStBlkAddr;
reg[5:0]		tStBlkIx;
reg				tDoStBlk;


always @(posedge clock)
begin

	/* Stage A */
	tInAddr		<= regInPc;

	tReqAddrA	<= tNxtAddrA;
	tReqAddrB	<= tNxtAddrB;
	tReqIxA		<= tNxtIxA;
	tReqIxB		<= tNxtIxB;

	tBlkDataA	<= icCaMemA[tReqIxA];
	tBlkDataB	<= icCaMemB[tReqIxB];
	tBlkAddrA	<= icCaAddrA[tReqIxA];
	tBlkAddrB	<= icCaAddrB[tReqIxB];

	if(tDoStBlk)
	begin
		icCaMemA[tStBlkIx]	<= tStBlkData[ 63: 0];
		icCaMemB[tStBlkIx]	<= tStBlkData[127:64];
		icCaAddrA[tStBlkIx]	<= tStBlkAddr;
		icCaAddrB[tStBlkIx]	<= tStBlkAddr;
		tDoStBlk			<= 0;
	end

	/* Stage B */
	
	if((tMissA || tMemLatchA) && !tMemLatchB)
	begin

		if(tMemLatchDnA)
		begin
			tMemPcOpm	<= UMEM_OPM_READY;
			if(memPcOK==UMEM_OK_READY)
			begin
				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
			end
		end
		else
			if((memPcOK==UMEM_OK_OK) && tMemLatchA)
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnA	<= 1;
			tStBlkData		<= memPcData;
			tStBlkAddr		<= tReqAddrA;
			tStBlkIx		<= tReqIxA;
			tDoStBlk		<= 1;
		end
		else
			if((memPcOK==UMEM_OK_HOLD) && tMemLatchA)
		begin
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
			tMemPcAddr		<= { tReqAddrA, 4'b0 };
		end
		else
			if(memPcOK==UMEM_OK_READY)
		begin
			tMemLatchA		<= 1;
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
			tMemPcAddr		<= { tReqAddrA, 4'b0 };
		end
		else
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
		end

	end
	else
		if(tMissB && !tMemLatchA)
	begin

		if(tMemLatchDnB)
		begin
			tMemPcOpm	<= UMEM_OPM_READY;
			if(memPcOK==UMEM_OK_READY)
			begin
				tMemLatchB		<= 0;
				tMemLatchDnB	<= 0;
			end
		end
		else
			if((memPcOK==UMEM_OK_OK) && tMemLatchB)
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnB	<= 1;
			tStBlkData		<= memPcData;
			tStBlkAddr		<= tReqAddrB;
			tStBlkIx		<= tReqIxB;
			tDoStBlk		<= 1;
		end
		else
			if((memPcOK==UMEM_OK_HOLD) && tMemLatchB)
		begin
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
			tMemPcAddr		<= { tReqAddrB, 4'b0 };
		end
		else
			if(memPcOK==UMEM_OK_READY)
		begin
			tMemLatchB		<= 1;
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
			tMemPcAddr		<= { tReqAddrB, 4'b0 };
		end
		else
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
		end

	end
	
end

endmodule

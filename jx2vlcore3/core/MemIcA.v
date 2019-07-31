module MemIcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,
	icInPcHold,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInPc;		//input PC address
output[63: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 1: 0]	regOutPcStep;	//PC step (Normal Op)
input			icInPcHold;

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

`ifdef jx2_reduce_l1sz
reg[63:0]		icCaMemA[15:0];		//Local L1 tile memory (Even)
reg[63:0]		icCaMemB[15:0];		//Local L1 tile memory (Odd)
reg[31:0]		icCaAddrA[15:0];	//Local L1 tile address
reg[31:0]		icCaAddrB[15:0];	//Local L1 tile address
`else
reg[63:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
reg[63:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
reg[31:0]		icCaAddrA[63:0];	//Local L1 tile address
reg[31:0]		icCaAddrB[63:0];	//Local L1 tile address
`endif

reg[27:0]		tNxtAddrA;
reg[27:0]		tNxtAddrB;

`ifdef jx2_reduce_l1sz
reg[3:0]		tNxtIxA;
reg[3:0]		tNxtIxB;
reg[3:0]		tReqIxA;
reg[3:0]		tReqIxB;
`else
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;
`endif

reg[63:0]		tBlkDataA;
reg[63:0]		tBlkDataB;
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFlagB;

reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;

reg[31:0]		tInAddr;
reg[1:0]		tInWordIx;
reg[31:0]		tRegInPc;		//input PC address

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

	tRegInPc	= icInPcHold ? tInAddr : regInPc;

//	if(regInPc[3])
	if(tRegInPc[3])
	begin
//		tNxtAddrB	= regInPc[31:4];
		tNxtAddrB	= tRegInPc[31:4];
		tNxtAddrA	= tNxtAddrB+1;
	end else begin
//		tNxtAddrA	= regInPc[31:4];
		tNxtAddrA	= tRegInPc[31:4];
		tNxtAddrB	= tNxtAddrA;
	end


`ifdef jx2_reduce_l1sz
	tNxtIxA=tNxtAddrA[3:0];
	tNxtIxB=tNxtAddrB[3:0];
`else
	tNxtIxA=tNxtAddrA[5:0];
	tNxtIxB=tNxtAddrB[5:0];
`endif


	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	
	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));
	tMiss = tMissA || tMissB;
	
	if(tInAddr[3])
	begin
		tBlkData = { tBlkDataA, tBlkDataB };
	end else begin
		tBlkData = { tBlkDataB, tBlkDataA };
	end

`ifdef jx2_enable_ops48
	casez(tBlkData[15:10])
		6'b111z11:	opLenA0=3'b011;
		6'b111z10:	opLenA0=3'b010;
		6'b111z01:	opLenA0=3'b110;
		6'b111z00:	opLenA0=3'b010;
		default:	opLenA0=3'b001; 
	endcase
	casez(tBlkData[31:26])
		6'b111z11: opLenA1=3'b011;
		6'b111z10: opLenA1=3'b010;
		6'b111z01: opLenA1=3'b110;
		6'b111z00: opLenA1=3'b010;
		default:   opLenA1=3'b001; 
	endcase
	casez(tBlkData[47:42])
		6'b111z11: opLenA2=3'b011;
		6'b111z10: opLenA2=3'b010;
		6'b111z01: opLenA2=3'b110;
		6'b111z00: opLenA2=3'b010;
		default:   opLenA2=3'b001; 
	endcase
	casez(tBlkData[63:58])
		6'b111z11: opLenA3=3'b011;
		6'b111z10: opLenA3=3'b010;
		6'b111z01: opLenA3=3'b110;
		6'b111z00: opLenA3=3'b010;
		default:   opLenA3=3'b001; 
	endcase
`else
	casez(tBlkData[15:13])
		3'b111:    opLenA0=3'b010;
		default:   opLenA0=3'b001; 
	endcase
	casez(tBlkData[31:29])
		3'b111:    opLenA1=3'b010;
		default:   opLenA1=3'b001; 
	endcase
	casez(tBlkData[47:45])
		3'b111:    opLenA2=3'b010;
		default:   opLenA2=3'b001; 
	endcase
	casez(tBlkData[63:61])
		3'b111:    opLenA3=3'b010;
		default:   opLenA3=3'b001; 
	endcase
`endif

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
	
	if(memPcOK==UMEM_OK_FAULT)
	begin
		$display("L1I$ Fault");
		tRegOutPcOK = UMEM_OK_FAULT;
	end

end

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;

reg[127:0]		tStBlkData;
reg[27:0]		tStBlkAddr;
reg[3:0]		tStBlkFlag;
reg				tDoStBlk;

`ifdef jx2_reduce_l1sz
reg[3:0]		tStBlkIx;
`else
reg[5:0]		tStBlkIx;
`endif


always @(posedge clock)
begin

	/* Stage A */
//	tInAddr		<= icInPcHold ? tInAddr : regInPc;
//	tInAddr		<= regInPc;
	tInAddr		<= tRegInPc;
	tReqAddrA	<= tNxtAddrA;
	tReqAddrB	<= tNxtAddrB;
	tReqIxA		<= tNxtIxA;
	tReqIxB		<= tNxtIxB;

	tBlkDataA	<= icCaMemA[tNxtIxA];
	tBlkDataB	<= icCaMemB[tNxtIxB];
	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNxtIxA];
	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNxtIxB];

	if(tDoStBlk)
	begin
//		$display("L1I$: StBlock A=%X D=%X", tStBlkAddr, tStBlkData);
	
		icCaMemA[tStBlkIx]	<= tStBlkData[ 63: 0];
		icCaMemB[tStBlkIx]	<= tStBlkData[127:64];
		icCaAddrA[tStBlkIx]	<= { tStBlkFlag, tStBlkAddr };
		icCaAddrB[tStBlkIx]	<= { tStBlkFlag, tStBlkAddr };
		tDoStBlk			<= 0;
	end

	/* Stage B */
	
	if((tMissA || tMemLatchA) && !tMemLatchB)
	begin

		if(tMemLatchDnA)
//		if(tMemLatchDnA || (memPcOK==UMEM_OK_FAULT))
		begin
			tMemPcOpm	<= UMEM_OPM_READY;
			if(memPcOK==UMEM_OK_READY)
//			if((memPcOK==UMEM_OK_READY) || (memPcOK==UMEM_OK_FAULT))
			begin
				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
			end
		end
		else
//			if((memPcOK==UMEM_OK_OK) && tMemLatchA)
			if(((memPcOK==UMEM_OK_OK) || (memPcOK==UMEM_OK_FAULT)) &&
				tMemLatchA)
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnA	<= 1;
			tStBlkData		<= memPcData;
			tStBlkAddr		<= tReqAddrA;
			tStBlkFlag		<= { 2'b00, ~tReqAddrA[1:0] };
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
		if((tMissB || tMemLatchB) && !tMemLatchA)
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
//			if((memPcOK==UMEM_OK_OK) && tMemLatchB)
			if(((memPcOK==UMEM_OK_OK) || (memPcOK==UMEM_OK_FAULT)) && tMemLatchB)
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnB	<= 1;
			tStBlkData		<= memPcData;
			tStBlkAddr		<= tReqAddrB;
			tStBlkFlag		<= { 2'b00, ~tReqAddrB[1:0] };
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
	else
	begin
		if(tMissA || tMissB)
			$display("L1I$ Sticky Miss, %d %d", tMissA, tMissB);
		if(tMemLatchA || tMemLatchB)
			$display("L1I$ Sticky Latch, %d %d", tMemLatchA, tMemLatchB);
	end
end

endmodule

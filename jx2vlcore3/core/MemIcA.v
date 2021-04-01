/*
L1 Instruction Cache
 */

`include "CoreDefs.v"

module MemIcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,		regOutPcVal,
	regOutPcOK,		regOutPcStep,
	icInPcHold,		icInPcWxe,
	icInPcOpm,		regInSr,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [47: 0]	regInPc;		//input PC address
// output[63: 0]	regOutPcVal;	//output PC value
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 3: 0]	regOutPcStep;	//PC step (Normal Op)
input			icInPcHold;
input			icInPcWxe;
input[4:0]		icInPcOpm;		//OPM (Used for cache-control)

input [63: 0]	regInSr;

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
// output[ 31:0]	memPcAddr;		//memory PC address
output[ 47:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[63:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 3: 0]		tRegOutPcStep;	//PC step (Normal Op)

// assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcVal		= { UV32_XX, tRegOutPcVal };
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;

// assign	memPcAddr	= tMemPcAddr;
assign	memPcAddr	= { 16'h00, tMemPcAddr };
assign	memPcOpm	= tMemPcOpm;

reg[31:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

`ifdef jx2_mem_l1isz_16
(* ram_style = "distributed" *)
	reg[71:0]		icCaMemA[15:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[71:0]		icCaMemB[15:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[15:0];	//Local L1 tile address
// reg[15:0]			icFlushMskA;
// reg[15:0]			icFlushMskB;
// reg[15:0]			icNxtFlushMskA;
// reg[15:0]			icNxtFlushMskB;
`endif

`ifdef jx2_mem_l1isz_64
(* ram_style = "distributed" *)
	reg[71:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[71:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[63:0];	//Local L1 tile address
// reg[63:0]			icFlushMskA;
// reg[63:0]			icFlushMskB;
// reg[63:0]			icNxtFlushMskA;
// reg[63:0]			icNxtFlushMskB;
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
reg[2:0]		tInByteIx;

reg[31:0]		tRegInPc;		//input PC address

reg[4:0]		tInOpm;			//OPM (Used for cache-control)
reg[4:0]		tInOpmB;		//OPM (Used for cache-control)
reg[4:0]		tInOpmC;		//OPM (Used for cache-control)
reg[4:0]		tInPcOpm;		//OPM (Used for cache-control)

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tPcStepW;

reg[127:0]		tBlkData;

reg[2:0]		opLenA0;
reg[2:0]		opLenA1;
reg[2:0]		opLenA2;
reg[2:0]		opLenA3;

reg[2:0]		opLenB0;
reg[2:0]		opLenB1;
reg[2:0]		opLenB2;
reg[2:0]		opLenB3;

`reg_tile_prov		tBlkPRovA;
`reg_tile_prov		tBlkPRovB;
`reg_tile_pflag		tBlkPFlA;
`reg_tile_pflag		tBlkPFlB;

reg				tMissAddrA;
reg				tMissAddrB;

reg				tFlushA;
reg				tFlushB;

reg				icDoFlush;
reg				icNxtDoFlush;

`reg_tile_prov	tFlushRov;
`reg_tile_prov	tNxtFlushRov;
reg				tAdvFlushRov;
reg				tNxtAdvFlushRov;

always @*
begin
	/* Stage A */

	tRegInPc	= icInPcHold ? tInAddr : regInPc[31:0];
	tInPcOpm	= icInPcHold ? tInOpm : icInPcOpm;

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


//	icNxtFlushMskA	= icFlushMskA;
//	icNxtFlushMskB	= icFlushMskB;
	icNxtDoFlush = 0;

	tNxtFlushRov	= tFlushRov;
	tNxtAdvFlushRov	= 0;

//	if(tDoStBlk)
//	begin
//		icNxtFlushMskA[tStBlkIx]=0;
//		icNxtFlushMskB[tStBlkIx]=0;
//	end

//	if((tInOpm==UMEM_OPM_FLUSHIS) || reset)
	if(((tInOpmB==UMEM_OPM_FLUSHIS) && (tInOpmC!=UMEM_OPM_FLUSHIS)) || reset)
	begin
//		icNxtFlushMskA = JX2_L1I_FLUSHMSK;
//		icNxtFlushMskB = JX2_L1I_FLUSHMSK;
		icNxtDoFlush = 1;
	end

	if((tFlushRov == 0) && !tAdvFlushRov)
		icNxtDoFlush = 1;

	if(icDoFlush)
	begin
//		$display("L1I: Do Flush");

		if(!tAdvFlushRov)
		begin
			tNxtFlushRov	= tFlushRov+1;
			tNxtAdvFlushRov	= 1;
		end

	end

	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	tInByteIx = tInAddr[2:0];
	
//	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
//	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));

//	tFlushA = (tBlkPRovA != tFlushRov) || (tBlkPRovA == 0);
//	tFlushB = (tBlkPRovB != tFlushRov) || (tBlkPRovB == 0);
	tFlushA = (tBlkPRovA != tFlushRov);
	tFlushB = (tBlkPRovB != tFlushRov);

	tMissAddrA = (tBlkAddrA != tReqAddrA);
	tMissAddrB = (tBlkAddrB != tReqAddrB);

//	tMissA = (tBlkAddrA != tReqAddrA) ||
//		(tBlkAddrA[1:0]!=(~tBlkFlagA[1:0])) ||
//		icFlushMskA[tReqIxA];
//	tMissB = (tBlkAddrB != tReqAddrB) ||
//		(tBlkAddrB[1:0]!=(~tBlkFlagB[1:0])) ||
//		icFlushMskB[tReqIxB];

	tMissA = tMissAddrA || tFlushA;
	tMissB = tMissAddrB || tFlushB;

	tMiss = tMissA || tMissB;
	
	if(tInAddr[3])
	begin
		tBlkData = { tBlkDataA, tBlkDataB };
	end else begin
		tBlkData = { tBlkDataB, tBlkDataA };
	end

`ifndef jx2_enable_ops24
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

`ifdef jx2_enable_ops24
	casez(tBlkData[15:12])
		4'b111z:	opLenA0=3'b010;
		4'b0111:	opLenA0=3'b101;
		4'b1001:	opLenA0=3'b101;
		default:	opLenA0=3'b001; 
	endcase
	casez(tBlkData[31:28])
		4'b111z:	opLenA1=3'b010;
		4'b0111:	opLenA1=3'b101;
		4'b1001:	opLenA1=3'b101;
		default:	opLenA1=3'b001; 
	endcase
	casez(tBlkData[47:44])
		4'b111z:	opLenA2=3'b010;
		4'b0111:	opLenA2=3'b101;
		4'b1001:	opLenA2=3'b101;
		default:	opLenA2=3'b001; 
	endcase
	casez(tBlkData[63:60])
		4'b111z:	opLenA3=3'b010;
		4'b0111:	opLenA3=3'b101;
		4'b1001:	opLenA3=3'b101;
		default:	opLenA3=3'b001; 
	endcase

	casez(tBlkData[23:20])
		4'b111z:	opLenB0=3'b010;
		4'b0111:	opLenB0=3'b101;
		4'b1001:	opLenB0=3'b101;
		default:	opLenB0=3'b001; 
	endcase
	casez(tBlkData[39:36])
		4'b111z:	opLenB1=3'b010;
		4'b0111:	opLenB1=3'b101;
		4'b1001:	opLenB1=3'b101;
		default:	opLenB1=3'b001; 
	endcase
	casez(tBlkData[55:52])
		4'b111z:	opLenB2=3'b010;
		4'b0111:	opLenB2=3'b101;
		4'b1001:	opLenB2=3'b101;
		default:	opLenB2=3'b001; 
	endcase
	casez(tBlkData[71:68])
		4'b111z:	opLenB3=3'b010;
		4'b0111:	opLenB3=3'b101;
		4'b1001:	opLenB3=3'b101;
		default:	opLenB3=3'b001; 
	endcase
`endif

	tRegOutPcVal	= UV64_XX;
	tRegOutPcStep	= 0;
	tPcStepW		= 0;

`ifdef jx2_enable_ops24
	case(tInByteIx)
		3'b000: begin
			tRegOutPcVal	= tBlkData[ 63:  0];
			tRegOutPcStep	= { 1'b0, opLenA0[1:0], opLenA0[2] };
			tPcStepW		= 0;
		end
		3'b001: begin
			tRegOutPcVal	= tBlkData[ 71:  8];
			tRegOutPcStep	= { 1'b0, opLenB0[1:0], opLenB0[2] };
			tPcStepW		= 0;
		end

		3'b010: begin
			tRegOutPcVal	= tBlkData[ 79: 16];
			tRegOutPcStep	= {1'b0, opLenA1[1:0], opLenA1[2] };
			tPcStepW		= 0;
		end
		3'b011: begin
			tRegOutPcVal	= tBlkData[ 87: 24];
			tRegOutPcStep	= { 1'b0, opLenB1[1:0], opLenB1[2] };
			tPcStepW		= 0;
		end

		3'b100: begin
			tRegOutPcVal	= tBlkData[ 95: 32];
			tRegOutPcStep	= {1'b0, opLenA2[1:0], opLenA2[2] };
			tPcStepW		= 0;
		end
		3'b101: begin
			tRegOutPcVal	= tBlkData[ 103: 40];
			tRegOutPcStep	= { 1'b0, opLenB2[1:0], opLenB2[2] };
			tPcStepW		= 0;
		end

		3'b110: begin
			tRegOutPcVal	= tBlkData[111: 48];
			tRegOutPcStep	= {1'b0, opLenA3[1:0], opLenA3[2] };
			tPcStepW		= 0;
		end
		3'b111: begin
			tRegOutPcVal	= tBlkData[119: 56];
			tRegOutPcStep	= { 1'b0, opLenB3[1:0], opLenB3[2] };
			tPcStepW		= 0;
		end
	endcase
`else
	case(tInWordIx)
		2'b00: begin
			tRegOutPcVal	= tBlkData[ 63:  0];
			tRegOutPcStep	= { 1'b0, opLenA0[1:0], 1'b0 };
			tPcStepW		= opLenA0[2] && tBlkData[12];
		end
		2'b01: begin
			tRegOutPcVal	= tBlkData[ 79: 16];
			tRegOutPcStep	= {1'b0, opLenA1[1:0], 1'b0 };
			tPcStepW		= opLenA1[2] && tBlkData[28];
		end
		2'b10: begin
			tRegOutPcVal	= tBlkData[ 95: 32];
			tRegOutPcStep	= {1'b0, opLenA2[1:0], 1'b0 };
			tPcStepW		= opLenA2[2] && tBlkData[44];
		end
		2'b11: begin
			tRegOutPcVal	= tBlkData[111: 48];
			tRegOutPcStep	= {1'b0, opLenA3[1:0], 1'b0 };
			tPcStepW		= opLenA3[2] && tBlkData[60];
		end
	endcase
`endif

// `ifdef jx2_enable_wex
//	if(icInPcWxe && tPcStepW)
//		tRegOutPcStep = 3'b100;
// `endif
	
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

`reg_tile_pflag		tStBlkPFl;
`reg_tile_prov		tStBlkPRov;

`ifdef jx2_reduce_l1sz
reg[3:0]		tStBlkIx;
`else
reg[5:0]		tStBlkIx;
`endif


always @(posedge clock)
begin
	tFlushRov		<= tNxtFlushRov;
	tAdvFlushRov	<= tNxtAdvFlushRov;

	/* Stage A */
//	tInAddr		<= icInPcHold ? tInAddr : regInPc;
//	tInAddr		<= regInPc;
	tInAddr		<= tRegInPc;
	tReqAddrA	<= tNxtAddrA;
	tReqAddrB	<= tNxtAddrB;
	tReqIxA		<= tNxtIxA;
	tReqIxB		<= tNxtIxB;

	tInOpm		<= tInPcOpm;
	tInOpmB		<= tInOpm;
	tInOpmC		<= tInOpmB;
//	icFlushMskA	<= icNxtFlushMskA;
//	icFlushMskB	<= icNxtFlushMskB;
	icDoFlush	<= icNxtDoFlush;

	{ tBlkPRovA, tBlkDataA }	<= icCaMemA[tNxtIxA];
	{ tBlkPRovB, tBlkDataB }	<= icCaMemB[tNxtIxB];
	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNxtIxA];
	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNxtIxB];

	if(tDoStBlk)
	begin
//		$display("L1I$: StBlock A=%X D=%X", tStBlkAddr, tStBlkData);
	
		icCaMemA[tStBlkIx]	<= { tStBlkPRov, tStBlkData[ 63: 0] };
		icCaMemB[tStBlkIx]	<= { tStBlkPRov, tStBlkData[127:64] };
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
			tStBlkPRov		<= tFlushRov;
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

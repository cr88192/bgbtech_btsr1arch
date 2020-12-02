/*
L1 Instruction Cache, WEX
 */

`include "CoreDefs.v"
// `include "MemIcOplen.v"

module MemIcWxA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,
	icInPcHold,		icInPcWxe,
	icInPcOpm,		regInSr,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK,
	memNoRwx
	);

input			clock;
input			reset;

input [47: 0]	regInPc;		//input PC address
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 2: 0]	regOutPcStep;	//PC step (Normal Op)
(* max_fanout = 100 *)
	input			icInPcHold;
input			icInPcWxe;
input[4:0]		icInPcOpm;		//OPM (Used for cache-control)

input [63: 0]	regInSr;

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 47:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable
input [  5:0]	memNoRwx;		//No Read/Write/Execute


reg[95:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 2: 0]		tRegOutPcStep;	//PC step (Normal Op)
reg[ 2: 0]		tRegOutPcStepA;	//PC step (Normal Op)

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOpm	= tMemPcOpm;

reg[47:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

`ifdef jx2_expand_l1isz
	reg[127:0]		icCaMemA[255:0];		//Local L1 tile memory (Even)
	reg[127:0]		icCaMemB[255:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
	reg[47:0]		icCaAddrA[255:0];	//Local L1 tile address
	reg[47:0]		icCaAddrB[255:0];	//Local L1 tile address
`else
	reg[31:0]		icCaAddrA[255:0];	//Local L1 tile address
	reg[31:0]		icCaAddrB[255:0];	//Local L1 tile address
`endif
reg[255:0]			icFlushMskA;
reg[255:0]			icFlushMskB;
reg[255:0]			icNxtFlushMskA;
reg[255:0]			icNxtFlushMskB;
`else
`ifdef jx2_reduce_l1sz
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemA[15:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemB[15:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrB[15:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[15:0];	//Local L1 tile address
`endif
reg[15:0]			icFlushMskA;
reg[15:0]			icFlushMskB;
reg[15:0]			icNxtFlushMskA;
reg[15:0]			icNxtFlushMskB;
`else
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrB[63:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[63:0];	//Local L1 tile address
`endif
reg[63:0]			icFlushMskA;
reg[63:0]			icFlushMskB;
reg[63:0]			icNxtFlushMskA;
reg[63:0]			icNxtFlushMskB;
`endif
`endif

`ifdef jx2_enable_vaddr48
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrB;
`else
(* max_fanout = 50 *)
	reg[27:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[27:0]		tNxtAddrB;
`endif

`ifdef jx2_expand_l1isz
reg[7:0]		tNxtIxA;
reg[7:0]		tNxtIxB;
reg[7:0]		tReqIxA;
reg[7:0]		tReqIxB;
`else
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
`endif

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
`ifdef jx2_enable_vaddr48
reg[43:0]		tBlkAddrA;
reg[43:0]		tBlkAddrB;
`else
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;
`endif
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFlagB;

`ifdef jx2_enable_vaddr48
reg[43:0]		tReqAddrA;
reg[43:0]		tReqAddrB;
`else
reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
`endif

reg[47:0]		tInAddr;
reg[1:0]		tInWordIx;
reg[47:0]		tRegInPc;		//input PC address
reg[4:0]		tInOpm;			//OPM (Used for cache-control)
reg[4:0]		tInOpmB;		//OPM (Used for cache-control)
reg[4:0]		tInPcOpm;		//OPM (Used for cache-control)

reg				tTlbMissInh;
reg				tNxtTlbMissInh;

reg				tFlushA;
reg				tFlushB;

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tPcStepWA;
reg				tPcStepWB;
reg				tPcStepJA;
reg				tPcStepBA;
reg				tPcStepBB;

// reg[127:0]		tBlkData;
reg[159:0]		tBlkData;

`ifdef def_true
reg[3:0]		opLenA0;
reg[3:0]		opLenA1;
reg[3:0]		opLenA2;
reg[3:0]		opLenA3;
reg[3:0]		opLenA4;
reg[3:0]		opLenA5;
`endif

reg				icDoFlush;
reg				icNxtDoFlush;

reg[43:0]		tRegInPcP0;
reg[43:0]		tRegInPcP1;
reg[14:0]		tRegInPcP1L;
reg[14:0]		tRegInPcP1H;

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;

	if(memNoRwx[5])
		tNxtTlbMissInh = 1;
	
//	if(tRegInOpm==UMEM_OPM_READY)
//	if(tInOpm == UMEM_OPM_LDTLB)
	if((tInOpm == UMEM_OPM_LDTLB) || regInSr[29])
		tNxtTlbMissInh = 0;
	

	/* Stage A */

	tRegInPc	= icInPcHold ? tInAddr : regInPc;
//	tInPcOpm	= icInPcHold ? tInOpm : icInPcOpm;

//	tRegInPc	= regInPc;
	tInPcOpm	= icInPcOpm;

	tRegInPcP0	= tRegInPc[47:4];

//	tRegInPcP1L[14: 0]	= { 1'b0, tRegInPcP0[13: 0] } + 1;
//	tRegInPcP1H[14: 0]	= { 1'b0, tRegInPcP0[27:14] } + 1;
//	tRegInPcP1 [27: 0]	= {
//		tRegInPcP1L[14] ? tRegInPcP1H[13:0] : tRegInPcP0[27:14],
//		tRegInPcP1L[13: 0] };

	tRegInPcP1[27: 0]	= tRegInPcP0[27: 0]+1;
	tRegInPcP1[43:28]	= tRegInPcP0[43:28];

	if(tRegInPc[4])
	begin
		tNxtAddrB			= tRegInPcP0;
		tNxtAddrA			= tRegInPcP1;
	end else begin
		tNxtAddrA			= tRegInPcP0;
		tNxtAddrB			= tRegInPcP1;
	end

`ifndef def_true
//	if(tRegInPc[3])
	if(tRegInPc[4])
	begin
//		tNxtAddrB			= tRegInPc[31:4];
		tNxtAddrB			= tRegInPc[47:4];
//		tNxtAddrA			= tNxtAddrB+1;
		tNxtAddrA[27: 0]	= tNxtAddrB[27: 0]+1;
		tNxtAddrA[43:28]	= tNxtAddrB[43:28];

	end else begin
//		tNxtAddrA			= tRegInPc[31:4];
		tNxtAddrA			= tRegInPc[47:4];
//		tNxtAddrB			= tNxtAddrA+1;
		tNxtAddrB[27: 0]	= tNxtAddrA[27: 0]+1;
		tNxtAddrB[43:28]	= tNxtAddrA[43:28];
	end
`endif

`ifdef jx2_expand_l1isz
	tNxtIxA=tNxtAddrA[8:1];
	tNxtIxB=tNxtAddrB[8:1];
`else
`ifdef jx2_reduce_l1sz
//	tNxtIxA=tNxtAddrA[3:0];
//	tNxtIxB=tNxtAddrB[3:0];
	tNxtIxA=tNxtAddrA[4:1];
	tNxtIxB=tNxtAddrB[4:1];
`else
//	tNxtIxA=tNxtAddrA[5:0];
//	tNxtIxB=tNxtAddrB[5:0];
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif
`endif

	icNxtFlushMskA	= icFlushMskA;
	icNxtFlushMskB	= icFlushMskB;
	icNxtDoFlush = 0;

	if(tDoStBlkA)
		icNxtFlushMskA[tStBlkIxA]=0;
	if(tDoStBlkB)
		icNxtFlushMskB[tStBlkIxB]=0;

	if(((tInOpm==UMEM_OPM_FLUSHIS) && (tInOpmB!=UMEM_OPM_FLUSHIS)) || reset)
	begin
//		icNxtFlushMskA = JX2_L1I_FLUSHMSK;
//		icNxtFlushMskB = JX2_L1I_FLUSHMSK;
		icNxtDoFlush = 1;
	end

	if(((tInOpm==UMEM_OPM_LDTLB) && (tInOpmB!=UMEM_OPM_LDTLB)) || reset)
	begin
//		icNxtDoFlush = 1;
	end
	
	if(icDoFlush)
//	if(icNxtDoFlush)
	begin
		icNxtFlushMskA = JX2_L1I_FLUSHMSK;
		icNxtFlushMskB = JX2_L1I_FLUSHMSK;
	end

	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	
//	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
//	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));

	tFlushA = icFlushMskA[tReqIxA];
	tFlushB = icFlushMskB[tReqIxB];
	
	if(!tTlbMissInh)
	begin
		if(tBlkFlagA[3])
			tFlushA = 1;
		if(tBlkFlagB[3])
			tFlushB = 1;
	end

	tMissA = (tBlkAddrA != tReqAddrA) ||
		(tBlkAddrA[1:0] != (~tBlkFlagA[1:0])) ||
//		icFlushMskA[tReqIxA];
		tFlushA;
	tMissB = (tBlkAddrB != tReqAddrB) ||
		(tBlkAddrB[1:0] != (~tBlkFlagB[1:0])) ||
//		icFlushMskB[tReqIxB];
		tFlushB;

	tMiss = tMissA || tMissB;
	
//	if(tInAddr[3])
//	begin
//		tBlkData = { tBlkDataA, tBlkDataB };
//	end else begin
//		tBlkData = { tBlkDataB, tBlkDataA };
//	end

	case(tInAddr[4:3])
		2'b00: tBlkData = { tBlkDataB[31:0], tBlkDataA[127: 0] };
		2'b01: tBlkData = { tBlkDataB[95:0], tBlkDataA[127:64] };
		2'b10: tBlkData = { tBlkDataA[31:0], tBlkDataB[127: 0] };
		2'b11: tBlkData = { tBlkDataA[95:0], tBlkDataB[127:64] };
	endcase

//	$display("BA=%X BB=%X", tBlkDataA, tBlkDataB);
//	$display("A=%X BD=%X Ix=%X",tInAddr, tBlkData, tInWordIx);


`ifdef def_true
	casez(tBlkData[15:8])
		8'b1111_111z:	opLenA0=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA0=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA0=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA0=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA0=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA0=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA0=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA0=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA0=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA0=4'b0010;		//E0..E7
		default:		opLenA0=4'b0001; 
	endcase
	casez(tBlkData[31:24])
		8'b1111_111z:	opLenA1=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA1=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA1=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA1=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA1=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA1=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA1=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA1=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA1=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA1=4'b0010;		//E0..E7
		default:		opLenA1=4'b0001; 
	endcase
	casez(tBlkData[47:40])
		8'b1111_111z:	opLenA2=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA2=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA2=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA2=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA2=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA2=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA2=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA2=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA2=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA2=4'b0010;		//E0..E7
		default:		opLenA2=4'b0001; 
	endcase
	casez(tBlkData[63:56])
		8'b1111_111z:	opLenA3=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA3=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA3=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA3=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA3=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA3=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA3=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA3=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA3=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA3=4'b0010;		//E0..E7
		default:		opLenA3=4'b0001; 
	endcase
`endif

`ifdef jx2_enable_wex3w

`ifdef def_true
// `ifndef def_true
	casez(tBlkData[79:72])
		8'b1111_111z:	opLenA4=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA4=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA4=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA4=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA4=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA4=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA4=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA4=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA4=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA4=4'b0010;		//E0..E7
		default:		opLenA4=4'b0001; 
	endcase
	casez(tBlkData[95:88])
		8'b1111_111z:	opLenA5=4'b1110;		//FE/FF
		8'b1111_110z:	opLenA5=4'b0110;		//FC/FD
		8'b1111_10zz:	opLenA5=4'b0010;		//F8..FB
		8'b1111_01zz:	opLenA5=4'b0110;		//F4..F7
		8'b1111_00zz:	opLenA5=4'b0010;		//F0..F3
		8'b1110_111z:	opLenA5=4'b0110;		//FE/FF
		8'b1110_110z:	opLenA5=4'b0010;		//FC/FD
		8'b1110_101z:	opLenA5=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA5=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA5=4'b0010;		//E0..E7
		default:		opLenA5=4'b0001; 
	endcase
`endif

`else
	opLenA4=4'b0000;
	opLenA5=4'b0000;
`endif


//	tRegOutPcVal	= UV96_XX;
	tRegOutPcVal	= UV96_00;
	tRegOutPcStep	= 0;
	tRegOutPcStepA	= 0;
	tPcStepWA		= 0;
	tPcStepWB		= 0;
	tPcStepJA		= 0;

	case(tInWordIx)
		2'b00: begin
			tRegOutPcVal	= tBlkData[ 95:  0];
			tRegOutPcStepA	= { 1'b0, opLenA0[1:0] };
//			tPcStepWA		= opLenA0[2] && tBlkData[12];
//			tPcStepWB		= opLenA2[2] && tBlkData[44];
			tPcStepWA		= opLenA0[2];
			tPcStepWB		= opLenA2[2];
//			tPcStepJA		= opLenA0[3] && (tRegOutPcVal[31:30]==2'b11);
			tPcStepJA		= opLenA0[3];
			tPcStepBA		= opLenA0[0];
			tPcStepBB		= opLenA2[0];
		end
		2'b01: begin
			tRegOutPcVal	= tBlkData[111: 16];
			tRegOutPcStepA	= {1'b0, opLenA1[1:0] };
//			tPcStepWA		= opLenA1[2] && tBlkData[28];
//			tPcStepWB		= opLenA3[2] && tBlkData[60];
			tPcStepWA		= opLenA1[2];
			tPcStepWB		= opLenA3[2];
//			tPcStepJA		= opLenA1[3] && (tRegOutPcVal[31:30]==2'b11);
			tPcStepJA		= opLenA1[3];
			tPcStepBA		= opLenA1[0];
			tPcStepBB		= opLenA3[0];
		end
		2'b10: begin
			tRegOutPcVal	= tBlkData[127: 32];
			tRegOutPcStepA	= {1'b0, opLenA2[1:0] };
//			tPcStepWA		= opLenA2[2] && tBlkData[44];
//			tPcStepWB		= opLenA4[2] && tBlkData[76];
			tPcStepWA		= opLenA2[2];
			tPcStepWB		= opLenA4[2];
//			tPcStepJA		= opLenA2[3] && (tRegOutPcVal[31:30]==2'b11);
			tPcStepJA		= opLenA2[3];
			tPcStepBA		= opLenA2[0];
			tPcStepBB		= opLenA4[0];
		end
		2'b11: begin
			tRegOutPcVal	= tBlkData[143: 48];
			tRegOutPcStepA	= {1'b0, opLenA3[1:0] };
//			tPcStepWA		= opLenA3[2] && tBlkData[60];
//			tPcStepWB		= opLenA5[2] && tBlkData[92];
			tPcStepWA		= opLenA3[2];
			tPcStepWB		= opLenA5[2];
//			tPcStepJA		= opLenA3[3] && (tRegOutPcVal[31:30]==2'b11);
			tPcStepJA		= opLenA3[3];
			tPcStepBA		= opLenA3[0];
			tPcStepBB		= opLenA5[0];
		end
	endcase
	
`ifdef jx2_enable_wex3w

`ifdef jx2_enable_wexjumbo

`ifdef def_true
	casez( {icInPcWxe, tPcStepJA,
			tPcStepWA, tPcStepWB,
			tPcStepBB, tPcStepBA})
		6'b00zzz0: tRegOutPcStep = 3'b010;
		6'b00zzz1: tRegOutPcStep = 3'b001;

		6'b01z00z: tRegOutPcStep = 3'b100;
		6'b01z01z: tRegOutPcStep = 3'b011;
		6'b01z1zz: tRegOutPcStep = 3'b110;

		6'b100zz0: tRegOutPcStep = 3'b010;
		6'b100zz1: tRegOutPcStep = 3'b001;
		6'b1010zz: tRegOutPcStep = 3'b100;
		6'b1011zz: tRegOutPcStep = 3'b110;

		6'b11z00z: tRegOutPcStep = 3'b100;
		6'b11z01z: tRegOutPcStep = 3'b011;
		6'b11z1zz: tRegOutPcStep = 3'b110;
	endcase
`endif

`ifndef def_true
	if(tPcStepJA)
	begin
`ifdef jx2_enable_ops48
		tRegOutPcStep = tPcStepWB ? 3'b110 : (tPcStepBB ? 3'b011 : 3'b100);
`else
		tRegOutPcStep = tPcStepWB ? 3'b110 : 3'b100;
`endif
	end
	else
		if(icInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = tPcStepWB ? 3'b110 : 3'b100;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end
`endif

`else

	if(icInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = tPcStepWB ? 3'b110 : 3'b100;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end

`endif

`else
	if(icInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = 3'b100;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end
`endif
	
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

reg[127:0]		tStBlkDataA;
reg[3:0]		tStBlkFlagA;
reg				tDoStBlkA;

reg[127:0]		tStBlkDataB;
reg[3:0]		tStBlkFlagB;
reg				tDoStBlkB;

`ifdef jx2_enable_vaddr48
reg[43:0]		tStBlkAddrA;
reg[43:0]		tStBlkAddrB;
`else
reg[27:0]		tStBlkAddrA;
reg[27:0]		tStBlkAddrB;
`endif

`ifdef jx2_expand_l1isz
reg[7:0]		tStBlkIxA;
reg[7:0]		tStBlkIxB;
`else
`ifdef jx2_reduce_l1sz
reg[3:0]		tStBlkIxA;
reg[3:0]		tStBlkIxB;
`else
reg[5:0]		tStBlkIxA;
reg[5:0]		tStBlkIxB;
`endif
`endif


always @(posedge clock)
begin
	tTlbMissInh		<= tNxtTlbMissInh;

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
	icFlushMskA	<= icNxtFlushMskA;
	icFlushMskB	<= icNxtFlushMskB;
	icDoFlush	<= icNxtDoFlush;

	tBlkDataA	<= icCaMemA[tNxtIxA];
	tBlkDataB	<= icCaMemB[tNxtIxB];
	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNxtIxA];
	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNxtIxB];

	if(tDoStBlkA)
	begin
//		$display("L1I$: StBlockA A=%X D=%X", tStBlkAddrA, tStBlkDataA);
	
		icCaMemA[tStBlkIxA]		<= tStBlkDataA;
		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };
		tDoStBlkA				<= 0;
	end

	if(tDoStBlkB)
	begin
//		$display("L1I$: StBlockB A=%X D=%X", tStBlkAddrB, tStBlkDataB);
	
		icCaMemB[tStBlkIxB]		<= tStBlkDataB;
		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };
		tDoStBlkB				<= 0;
	end

	/* Stage B */
	
	if((tMissA || tMemLatchA) && !tMemLatchB && !reset)
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
			tStBlkDataA		<= memPcData;
			tStBlkAddrA		<= tReqAddrA;
//			tStBlkFlagA		<= { 2'b00, ~tReqAddrA[1:0] };
			tStBlkFlagA		<= { memNoRwx[5], memNoRwx[2], ~tReqAddrA[1:0] };
			tStBlkIxA		<= tReqIxA;
			tDoStBlkA		<= 1;
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
		if((tMissB || tMemLatchB) && !tMemLatchA && !reset)
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
			tStBlkDataB		<= memPcData;
			tStBlkAddrB		<= tReqAddrB;
//			tStBlkFlagB		<= { 2'b00, ~tReqAddrB[1:0] };
			tStBlkFlagB		<= { memNoRwx[5], memNoRwx[2], ~tReqAddrB[1:0] };
			tStBlkIxB		<= tReqIxB;
			tDoStBlkB		<= 1;
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

		tMemLatchA		<= 0;
		tMemLatchDnA	<= 0;
		tMemLatchB		<= 0;
		tMemLatchDnB	<= 0;

		tMemPcOpm		<= UMEM_OPM_READY;
		tMemPcAddr		<= 0;
	end
end

endmodule

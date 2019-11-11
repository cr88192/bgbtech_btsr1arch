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
	icInPcOpm,
	memPcData,		memPcAddr,
	memPcOpm,		memPcOK
	);

input			clock;
input			reset;

input [31: 0]	regInPc;		//input PC address
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 2: 0]	regOutPcStep;	//PC step (Normal Op)
input			icInPcHold;
input			icInPcWxe;
input[4:0]		icInPcOpm;		//OPM (Used for cache-control)

input [127:0]	memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 31:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable


reg[95:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 2: 0]		tRegOutPcStep;	//PC step (Normal Op)

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;

assign	memPcAddr	= tMemPcAddr;
assign	memPcOpm	= tMemPcOpm;

reg[31:0]		tMemPcAddr;		//memory PC address
reg[ 4:0]		tMemPcOpm;		//memory PC output-enable

`ifdef jx2_expand_l1sz
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemA[255:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemB[255:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[255:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[255:0];	//Local L1 tile address
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
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[15:0];	//Local L1 tile address
reg[15:0]			icFlushMskA;
reg[15:0]			icFlushMskB;
reg[15:0]			icNxtFlushMskA;
reg[15:0]			icNxtFlushMskB;
`else
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[63:0];	//Local L1 tile address
reg[63:0]			icFlushMskA;
reg[63:0]			icFlushMskB;
reg[63:0]			icNxtFlushMskA;
reg[63:0]			icNxtFlushMskB;
`endif
`endif

reg[27:0]		tNxtAddrA;
reg[27:0]		tNxtAddrB;

`ifdef jx2_expand_l1sz
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
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFlagB;

reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;

reg[31:0]		tInAddr;
reg[1:0]		tInWordIx;
reg[31:0]		tRegInPc;		//input PC address
reg[4:0]		tInOpm;			//OPM (Used for cache-control)
reg[4:0]		tInOpmB;		//OPM (Used for cache-control)
reg[4:0]		tInPcOpm;		//OPM (Used for cache-control)

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tPcStepWA;
reg				tPcStepWB;
reg				tPcStepJA;

// reg[127:0]		tBlkData;
reg[159:0]		tBlkData;

// `ifndef def_true
`ifdef def_true
reg[3:0]		opLenA0;
reg[3:0]		opLenA1;
reg[3:0]		opLenA2;
reg[3:0]		opLenA3;
reg[3:0]		opLenA4;
reg[3:0]		opLenA5;
`endif

`ifndef def_true
// `ifdef def_true
wire[2:0]		opLenA0;
wire[2:0]		opLenA1;
wire[2:0]		opLenA2;
wire[2:0]		opLenA3;
wire[2:0]		opLenA4;
wire[2:0]		opLenA5;

reg[15:0]		opIstrA0;
reg[15:0]		opIstrA1;
reg[15:0]		opIstrA2;
reg[15:0]		opIstrA3;
reg[15:0]		opIstrA4;
reg[15:0]		opIstrA5;

MemIcOplen	oplA0(opIstrA0, opLenA0);
MemIcOplen	oplA1(opIstrA1, opLenA1);
MemIcOplen	oplA2(opIstrA2, opLenA2);
MemIcOplen	oplA3(opIstrA3, opLenA3);
`ifdef jx2_enable_wex3w
MemIcOplen	oplA4(opIstrA4, opLenA4);
MemIcOplen	oplA5(opIstrA5, opLenA5);
`endif

`endif

always @*
begin
	/* Stage A */

	tRegInPc	= icInPcHold ? tInAddr : regInPc;
	tInPcOpm	= icInPcHold ? tInOpm : icInPcOpm;

//	if(tRegInPc[3])
	if(tRegInPc[4])
	begin
		tNxtAddrB	= tRegInPc[31:4];
		tNxtAddrA	= tNxtAddrB+1;
	end else begin
		tNxtAddrA	= tRegInPc[31:4];
		tNxtAddrB	= tNxtAddrA+1;
	end


`ifdef jx2_expand_l1sz
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

	if(tDoStBlkA)
		icNxtFlushMskA[tStBlkIxA]=0;
	if(tDoStBlkB)
		icNxtFlushMskB[tStBlkIxB]=0;

	if(((tInOpm==UMEM_OPM_FLUSHIS) && (tInOpmB!=UMEM_OPM_FLUSHIS)) || reset)
	begin
		icNxtFlushMskA = JX2_L1_FLUSHMSK;
		icNxtFlushMskB = JX2_L1_FLUSHMSK;
	end

	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	
//	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
//	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));

	tMissA = (tBlkAddrA != tReqAddrA) ||
		(tBlkAddrA[1:0]!=(~tBlkFlagA[1:0])) ||
		icFlushMskA[tReqIxA];
	tMissB = (tBlkAddrB != tReqAddrB) ||
		(tBlkAddrB[1:0]!=(~tBlkFlagB[1:0])) ||
		icFlushMskB[tReqIxB];

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

`ifndef def_true
	opIstrA0	= tBlkData[15: 0];
	opIstrA1	= tBlkData[31:16];
	opIstrA2	= tBlkData[47:32];
	opIstrA3	= tBlkData[63:48];
`ifdef jx2_enable_wex3w
	opIstrA4	= tBlkData[79:64];
	opIstrA5	= tBlkData[95:80];
`endif
`endif

`ifdef def_true
	casez(tBlkData[15:8])
		8'b1111_11zz:	opLenA0=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA0=4'b0010;		//FB
		8'b1111_1001:	opLenA0=4'b0110;		//F9
		8'b1111_10z0:	opLenA0=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA0=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA0=4'b0110;		//F5
		8'b1111_0100:	opLenA0=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA0=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA0=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA0=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA0=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA0=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA0=4'b0010;		//E0..E7
		default:		opLenA0=4'b0001; 
	endcase
	casez(tBlkData[31:24])
		8'b1111_11zz:	opLenA1=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA1=4'b0010;		//FB
		8'b1111_1001:	opLenA1=4'b0110;		//F9
		8'b1111_10z0:	opLenA1=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA1=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA1=4'b0110;		//F5
		8'b1111_0100:	opLenA1=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA1=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA1=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA1=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA1=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA1=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA1=4'b0010;		//E0..E7
		default:		opLenA1=4'b0001; 
	endcase
	casez(tBlkData[47:40])
		8'b1111_11zz:	opLenA2=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA2=4'b0010;		//FB
		8'b1111_1001:	opLenA2=4'b0110;		//F9
		8'b1111_10z0:	opLenA2=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA2=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA2=4'b0110;		//F5
		8'b1111_0100:	opLenA2=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA2=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA2=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA2=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA2=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA2=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA2=4'b0010;		//E0..E7
		default:		opLenA2=4'b0001; 
	endcase
	casez(tBlkData[63:56])
		8'b1111_11zz:	opLenA3=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA3=4'b0010;		//FB
		8'b1111_1001:	opLenA3=4'b0110;		//F9
		8'b1111_10z0:	opLenA3=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA3=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA3=4'b0110;		//F5
		8'b1111_0100:	opLenA3=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA3=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA3=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA3=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA3=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA3=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA3=4'b0010;		//E0..E7
		default:		opLenA3=4'b0001; 
	endcase
`endif

`ifndef def_true
	casez({tBlkData[15:13], tBlkData[11:10], tBlkData[ 8] })
		6'b111_11z:	opLenA0=3'b011;
		6'b111_101:	opLenA0=3'b110;		//
		6'b111_100:	opLenA0=3'b010;		//8/A
		6'b111_01z:	opLenA0=3'b110;		//4..7
		6'b111_00z:	opLenA0=3'b010;		//0..3
		default:	opLenA0=3'b001; 
	endcase
	casez({tBlkData[31:29], tBlkData[27:26], tBlkData[24] })
		6'b111_11z:	opLenA1=3'b011;
		6'b111_101:	opLenA1=3'b110;
		6'b111_100:	opLenA1=3'b010;
		6'b111_01z:	opLenA1=3'b110;
		6'b111_00z:	opLenA1=3'b010;
		default:	opLenA1=3'b001; 
	endcase
	casez({tBlkData[47:45], tBlkData[43:42], tBlkData[40] })
		6'b111_11z:	opLenA2=3'b011;
		6'b111_101:	opLenA2=3'b110;
		6'b111_100:	opLenA2=3'b010;
		6'b111_01z:	opLenA2=3'b110;
		6'b111_00z:	opLenA2=3'b010;
		default:	opLenA2=3'b001; 
	endcase
	casez({tBlkData[63:61], tBlkData[59:58], tBlkData[56] })
		6'b111_11z:	opLenA3=3'b011;
		6'b111_101:	opLenA3=3'b110;
		6'b111_100:	opLenA3=3'b010;
		6'b111_01z:	opLenA3=3'b110;
		6'b111_00z:	opLenA3=3'b010;
		default:	opLenA3=3'b001; 
	endcase
`endif

`ifdef jx2_enable_wex3w

`ifdef def_true
// `ifndef def_true
	casez(tBlkData[79:72])
		8'b1111_11zz:	opLenA4=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA4=4'b0010;		//FB
		8'b1111_1001:	opLenA4=4'b0110;		//F9
		8'b1111_10z0:	opLenA4=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA4=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA4=4'b0110;		//F5
		8'b1111_0100:	opLenA4=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA4=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA4=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA4=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA4=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA4=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA4=4'b0010;		//E0..E7
		default:		opLenA4=4'b0001; 
	endcase
	casez(tBlkData[95:88])
		8'b1111_11zz:	opLenA5=4'b0011;		//FC..FF
		8'b1111_1011:	opLenA5=4'b0010;		//FB
		8'b1111_1001:	opLenA5=4'b0110;		//F9
		8'b1111_10z0:	opLenA5=4'b0010;		//F8/FA
`ifdef jx2_enable_wexjumbo
		8'b1111_011z:	opLenA5=4'b0110;		//F6/F7
		8'b1111_0101:	opLenA5=4'b0110;		//F5
		8'b1111_0100:	opLenA5=4'b1110;		//F4
`else
		8'b1111_01zz:	opLenA5=4'b0110;		//F4..F7
`endif
		8'b1111_00zz:	opLenA5=4'b0010;		//F0..F3
		8'b1110_11zz:	opLenA5=4'b0011;		//EC..EF
		8'b1110_101z:	opLenA5=4'b0110;		//EA/EB
		8'b1110_100z:	opLenA5=4'b0010;		//E8/E9
		8'b1110_0zzz:	opLenA5=4'b0010;		//E0..E7
		default:		opLenA5=4'b0001; 
	endcase
`endif

`ifndef def_true
	casez({tBlkData[79:77], tBlkData[75:74], tBlkData[72] })
		6'b111_11z:	opLenA4=3'b011;
		6'b111_101:	opLenA4=3'b110;
		6'b111_100:	opLenA4=3'b010;
		6'b111_01z:	opLenA4=3'b110;
		6'b111_00z:	opLenA4=3'b010;
		default:	opLenA4=3'b001; 
	endcase
	casez({tBlkData[95:93], tBlkData[91:90], tBlkData[88] })
		6'b111_11z:	opLenA5=3'b011;
		6'b111_101:	opLenA5=3'b110;
		6'b111_100:	opLenA5=3'b010;
		6'b111_01z:	opLenA5=3'b110;
		6'b111_00z:	opLenA5=3'b010;
		default:	opLenA5=3'b001; 
	endcase
`endif

`else
	opLenA4=4'b0000;
	opLenA5=4'b0000;
`endif


	tRegOutPcVal	= UV96_XX;
	tRegOutPcStep	= 0;
	tPcStepWA		= 0;
	tPcStepWB		= 0;
	tPcStepJA		= 0;

	case(tInWordIx)
		2'b00: begin
			tRegOutPcVal	= tBlkData[ 95:  0];
			tRegOutPcStep	= { 1'b0, opLenA0[1:0] };
//			tPcStepWA		= opLenA0[2] && tBlkData[12];
//			tPcStepWB		= opLenA2[2] && tBlkData[44];
			tPcStepWA		= opLenA0[2];
			tPcStepWB		= opLenA2[2];
			tPcStepJA		= opLenA0[3] && (tRegOutPcVal[31:30]==2'b11);
		end
		2'b01: begin
			tRegOutPcVal	= tBlkData[111: 16];
			tRegOutPcStep	= {1'b0, opLenA1[1:0] };
//			tPcStepWA		= opLenA1[2] && tBlkData[28];
//			tPcStepWB		= opLenA3[2] && tBlkData[60];
			tPcStepWA		= opLenA1[2];
			tPcStepWB		= opLenA3[2];
			tPcStepJA		= opLenA1[3] && (tRegOutPcVal[31:30]==2'b11);
		end
		2'b10: begin
			tRegOutPcVal	= tBlkData[127: 32];
			tRegOutPcStep	= {1'b0, opLenA2[1:0] };
//			tPcStepWA		= opLenA2[2] && tBlkData[44];
//			tPcStepWB		= opLenA4[2] && tBlkData[76];
			tPcStepWA		= opLenA2[2];
			tPcStepWB		= opLenA4[2];
			tPcStepJA		= opLenA2[3] && (tRegOutPcVal[31:30]==2'b11);
		end
		2'b11: begin
			tRegOutPcVal	= tBlkData[143: 48];
			tRegOutPcStep	= {1'b0, opLenA3[1:0] };
//			tPcStepWA		= opLenA3[2] && tBlkData[60];
//			tPcStepWB		= opLenA5[2] && tBlkData[92];
			tPcStepWA		= opLenA3[2];
			tPcStepWB		= opLenA5[2];
			tPcStepJA		= opLenA3[3] && (tRegOutPcVal[31:30]==2'b11);
		end
	endcase
	
`ifdef jx2_enable_wex3w
	if(icInPcWxe && tPcStepWA)
		tRegOutPcStep = tPcStepWB ? 3'b110 : 3'b100;

`ifdef jx2_enable_wexjumbo
//	if((tRegOutPcVal[15:8]==8'hF4) && (tRegOutPcVal[31:30]==2'b11))
	if(tPcStepJA)
	begin
//		tRegOutPcStep = 3'b110;
//		tRegOutPcStep = tRegOutPcVal[42] ? 3'b110 : 3'b100;
		tRegOutPcStep = tPcStepWB ? 3'b110 : 3'b100;
	end
`endif

`else
	if(icInPcWxe && tPcStepWA)
		tRegOutPcStep = 3'b100;
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
reg[27:0]		tStBlkAddrA;
reg[3:0]		tStBlkFlagA;
reg				tDoStBlkA;

reg[127:0]		tStBlkDataB;
reg[27:0]		tStBlkAddrB;
reg[3:0]		tStBlkFlagB;
reg				tDoStBlkB;

`ifdef jx2_expand_l1sz
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
			tStBlkFlagA		<= { 2'b00, ~tReqAddrA[1:0] };
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
			tStBlkFlagB		<= { 2'b00, ~tReqAddrB[1:0] };
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
	end
end

endmodule

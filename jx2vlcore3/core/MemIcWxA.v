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

`input_tile		memPcData;		//memory PC data
input [  1:0]	memPcOK;		//memory PC OK
output[ 47:0]	memPcAddr;		//memory PC address
output[  4:0]	memPcOpm;		//memory PC output-enable
input [  5:0]	memNoRwx;		//No Read/Write/Execute

`reg_tile		tMemPcData;		//memory PC data
reg [  1:0]		tMemPcOK;		//memory PC OK
reg [  5:0]		tMemNoRwx;		//No Read/Write/Execute

reg [63: 0]		tRegInSr;

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

`ifdef jx2_mem_l1isz_256
`reg_tile_pad		icCaMemA[255:0];		//Local L1 tile memory (Even)
`reg_tile_pad		icCaMemB[255:0];		//Local L1 tile memory (Odd)
`reg_tile_pad		icCaMemC[255:0];		//Local L1 tile memory (Even)
`reg_tile_pad		icCaMemD[255:0];		//Local L1 tile memory (Odd)

`ifdef jx2_enable_vaddr48
	reg[47:0]		icCaAddrA[255:0];	//Local L1 tile address
	reg[47:0]		icCaAddrB[255:0];	//Local L1 tile address
	reg[47:0]		icCaAddrC[255:0];	//Local L1 tile address
	reg[47:0]		icCaAddrD[255:0];	//Local L1 tile address
`else
	reg[31:0]		icCaAddrA[255:0];	//Local L1 tile address
	reg[31:0]		icCaAddrB[255:0];	//Local L1 tile address
	reg[31:0]		icCaAddrC[255:0];	//Local L1 tile address
	reg[31:0]		icCaAddrD[255:0];	//Local L1 tile address
`endif
//reg[255:0]			icFlushMskA;
//reg[255:0]			icFlushMskB;
//reg[255:0]			icNxtFlushMskA;
//reg[255:0]			icNxtFlushMskB;
`endif

`ifdef jx2_mem_l1isz_128
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemA[127:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemB[127:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemC[127:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemD[127:0];		//Local L1 tile memory (Odd)

`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrA[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrB[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrC[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrD[127:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrC[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrD[127:0];	//Local L1 tile address
`endif
//reg[127:0]			icFlushMskA;
//reg[127:0]			icFlushMskB;
//reg[127:0]			icNxtFlushMskA;
//reg[127:0]			icNxtFlushMskB;
`endif

`ifdef jx2_mem_l1isz_64
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemB[63:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemC[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
`reg_tile_pad		icCaMemD[63:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrB[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrC[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[47:0]		icCaAddrD[63:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrB[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrC[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[31:0]		icCaAddrD[63:0];	//Local L1 tile address
`endif
//reg[63:0]			icFlushMskA;
//reg[63:0]			icFlushMskB;
//reg[63:0]			icNxtFlushMskA;
//reg[63:0]			icNxtFlushMskB;

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

`ifdef jx2_mem_l1isz_256
reg[7:0]		tNxtIxA;
reg[7:0]		tNxtIxB;
reg[7:0]		tReqIxA;
reg[7:0]		tReqIxB;
reg[7:0]		tReqIxAL;
reg[7:0]		tReqIxBL;
`endif

`ifdef jx2_mem_l1isz_128
reg[6:0]		tNxtIxA;
reg[6:0]		tNxtIxB;
reg[6:0]		tReqIxA;
reg[6:0]		tReqIxB;
reg[6:0]		tReqIxAL;
reg[6:0]		tReqIxBL;
`endif

`ifdef jx2_mem_l1isz_64
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;
reg[5:0]		tReqIxAL;
reg[5:0]		tReqIxBL;
`endif

`reg_tile		tBlkDataA;
`reg_tile		tBlkDataB;
`reg_tile		tBlkDataC;
`reg_tile		tBlkDataD;
`reg_tile		tBlkData2A;
`reg_tile		tBlkData2B;
`reg_tile		tBlkData2A2;
`reg_tile		tBlkData2B2;

`ifdef jx2_enable_vaddr48
reg[43:0]		tBlkAddrA;
reg[43:0]		tBlkAddrB;
reg[43:0]		tBlkAddrC;
reg[43:0]		tBlkAddrD;
reg[43:0]		tBlkAddr2A;
reg[43:0]		tBlkAddr2B;
`else
reg[27:0]		tBlkAddrA;
reg[27:0]		tBlkAddrB;
reg[27:0]		tBlkAddrC;
reg[27:0]		tBlkAddrD;
reg[27:0]		tBlkAddr2A;
reg[27:0]		tBlkAddr2B;
`endif
reg[ 3:0]		tBlkFlagA;
reg[ 3:0]		tBlkFlagB;
reg[ 3:0]		tBlkFlagC;
reg[ 3:0]		tBlkFlagD;
reg[ 3:0]		tBlkFlag2A;
reg[ 3:0]		tBlkFlag2B;

`reg_tile_pflag		tBlkPFlA;
`reg_tile_pflag		tBlkPFlB;
`reg_tile_pflag		tBlkPFlC;
`reg_tile_pflag		tBlkPFlD;
`reg_tile_prov		tBlkPRovA;
`reg_tile_prov		tBlkPRovB;
`reg_tile_prov		tBlkPRovC;
`reg_tile_prov		tBlkPRovD;

`ifdef jx2_enable_vaddr48
reg[43:0]		tReqAddrA;
reg[43:0]		tReqAddrB;
reg[43:0]		tReqAddrAL;
reg[43:0]		tReqAddrBL;
`else
reg[27:0]		tReqAddrA;
reg[27:0]		tReqAddrB;
reg[27:0]		tReqAddrAL;
reg[27:0]		tReqAddrBL;
`endif

reg[47:0]		tInAddr;
reg[1:0]		tInWordIx;
reg[47:0]		tRegInPc;		//input PC address
reg[4:0]		tInOpm;			//OPM (Used for cache-control)
reg[4:0]		tInOpmB;		//OPM (Used for cache-control)
reg[4:0]		tInOpmC;		//OPM (Used for cache-control)
reg[4:0]		tInPcOpm;		//OPM (Used for cache-control)

reg				tTlbMissInh;
reg				tNxtTlbMissInh;

reg				tBlkFlushA;
reg				tBlkFlushB;

reg				tFlushA;
reg				tFlushB;
reg				tFlushAL;
reg				tFlushBL;

reg				tMissA;
reg				tMissB;
reg				tMissC;
reg				tMissD;
reg				tMiss;
reg				tMissL;
reg				tPcStepWA;
reg				tPcStepWB;
reg				tPcStepJA;
reg				tPcStepBA;
reg				tPcStepBB;

reg				tMissAL;
reg				tMissBL;

reg				tMissAddrA;
reg				tMissAddrB;
reg				tMissAddrC;
reg				tMissAddrD;

// reg[127:0]		tBlkData;
reg[159:0]		tBlkData;
reg[127:0]		tBlkData0A;
reg[255:0]		tBlkData1A;

reg[3:0]		opLenA0;
reg[3:0]		opLenA1;
reg[3:0]		opLenA2;
reg[3:0]		opLenA3;
reg[3:0]		opLenA4;
reg[3:0]		opLenA5;

reg				icDoFlush;
reg				icNxtDoFlush;

reg[43:0]		tRegInPcP0;
reg[43:0]		tRegInPcP1;
reg[14:0]		tRegInPcP1L;
reg[14:0]		tRegInPcP1H;

`reg_tile_prov	tFlushRov;
`reg_tile_prov	tNxtFlushRov;
reg				tAdvFlushRov;
reg				tNxtAdvFlushRov;

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;

	if(tMemNoRwx[5])
		tNxtTlbMissInh = 1;
	
//	if(tRegInOpm==UMEM_OPM_READY)
//	if(tInOpm == UMEM_OPM_LDTLB)
//	if((tInOpm == UMEM_OPM_LDTLB) || tRegInSr[29])
	if((tInOpmB == UMEM_OPM_LDTLB) || tRegInSr[29])
		tNxtTlbMissInh = 0;
	

	/* Stage A */

	tRegInPc	= regInPc;
//	tRegInPc	= icInPcHold ? tInAddr : regInPc;
//	tInPcOpm	= icInPcHold ? tInOpm : icInPcOpm;

//	tRegInPc	= regInPc;
	tInPcOpm	= icInPcOpm;

`ifdef jx2_mem_line32B
	tRegInPcP0	= { tRegInPc[47:5], 1'b0 };
	tRegInPcP1[27: 0]	= tRegInPcP0[27: 0]+2;
	tRegInPcP1[43:28]	= tRegInPcP0[43:28];
	if(tRegInPc[5])
	begin
		tNxtAddrB			= tRegInPcP0;
		tNxtAddrA			= tRegInPcP1;
	end else begin
		tNxtAddrA			= tRegInPcP0;
		tNxtAddrB			= tRegInPcP1;
	end
`else
	tRegInPcP0	= tRegInPc[47:4];
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
`endif

	if(icInPcHold)
	begin
		tRegInPc	= tInAddr;
		tNxtAddrA	= tReqAddrA;
		tNxtAddrB	= tReqAddrB;
	end

`ifdef jx2_mem_l1isz_256
	tNxtIxA=tNxtAddrA[8:1];
	tNxtIxB=tNxtAddrB[8:1];
`endif

`ifdef jx2_mem_l1isz_128
	tNxtIxA=tNxtAddrA[7:1];
	tNxtIxB=tNxtAddrB[7:1];
`endif

`ifdef jx2_mem_l1isz_64
//	tNxtIxA=tNxtAddrA[5:0];
//	tNxtIxB=tNxtAddrB[5:0];
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif

//	icNxtFlushMskA	= icFlushMskA;
//	icNxtFlushMskB	= icFlushMskB;
	icNxtDoFlush = 0;

	tNxtFlushRov	= tFlushRov;
	tNxtAdvFlushRov	= 0;


//	if(tDoStBlkA)
//		icNxtFlushMskA[tStBlkIxA]=0;
//	if(tDoStBlkB)
//		icNxtFlushMskB[tStBlkIxB]=0;

//	if(((tInOpm==UMEM_OPM_FLUSHIS) && (tInOpmB!=UMEM_OPM_FLUSHIS)) || reset)
	if(((tInOpmB==UMEM_OPM_FLUSHIS) && (tInOpmC!=UMEM_OPM_FLUSHIS)) || reset)
	begin
//		icNxtFlushMskA = JX2_L1I_FLUSHMSK;
//		icNxtFlushMskB = JX2_L1I_FLUSHMSK;
		icNxtDoFlush = 1;
	end

//	if(((tInOpm==UMEM_OPM_LDTLB) && (tInOpmB!=UMEM_OPM_LDTLB)) || reset)
//	begin
//		icNxtDoFlush = 1;
//	end
	
	if((tFlushRov == 0) && !tAdvFlushRov)
		icNxtDoFlush = 1;
	
//	if(icDoFlush || reset)
	if(icDoFlush)
//	if(icNxtDoFlush)
	begin
//		$display("L1I: Do Flush");
//		icNxtFlushMskA = JX2_L1I_FLUSHMSK;
//		icNxtFlushMskB = JX2_L1I_FLUSHMSK;

		if(!tAdvFlushRov)
		begin
			tNxtFlushRov	= tFlushRov+1;
			tNxtAdvFlushRov	= 1;
		end

	end

	/* Stage B */
	
	tInWordIx = tInAddr[2:1];
	
//	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
//	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));

//	tFlushA = icFlushMskA[tReqIxA];
//	tFlushB = icFlushMskB[tReqIxB];
//	tFlushA = tBlkFlushA;
//	tFlushB = tBlkFlushB;

	tFlushA = (tBlkPRovA != tFlushRov);
	tFlushB = (tBlkPRovB != tFlushRov);
	
	if(!tTlbMissInh)
	begin
		if(tBlkFlagA[3])
			tFlushA = 1;
		if(tBlkFlagB[3])
			tFlushB = 1;
`ifdef jx2_mem_l1i2way
		if(tBlkFlagC[3])
			tFlushA = 1;
		if(tBlkFlagD[3])
			tFlushB = 1;
`endif
	end

	tBlkData2A = tBlkDataA;
	tBlkAddr2A = tBlkAddrA;
	tBlkFlag2A = tBlkFlagA;

	tBlkData2B = tBlkDataB;
	tBlkAddr2B = tBlkAddrB;
	tBlkFlag2B = tBlkFlagB;

//	tMissAddrA = (tBlkAddrA != tReqAddrA);
//	tMissAddrB = (tBlkAddrB != tReqAddrB);

//`ifndef def_true
`ifdef def_true
	tMissAddrA =
		(tBlkAddrA[43:36] != tReqAddrA[43:36]) ||
		(tBlkAddrA[35:24] != tReqAddrA[35:24]) ||
		(tBlkAddrA[24:12] != tReqAddrA[24:12]) ||
		(tBlkAddrA[11: 0] != tReqAddrA[11: 0]) ;
	tMissAddrB =
		(tBlkAddrB[43:36] != tReqAddrB[43:36]) ||
		(tBlkAddrB[35:24] != tReqAddrB[35:24]) ||
		(tBlkAddrB[23:12] != tReqAddrB[23:12]) ||
		(tBlkAddrB[11: 0] != tReqAddrB[11: 0]) ;
`endif

	tMissA =
//		(tBlkAddrA != tReqAddrA) ||
		tMissAddrA ||
//		(tBlkAddrA[1:0] != (~tBlkFlagA[1:0])) ||
		tFlushA;
	tMissB =
//		(tBlkAddrB != tReqAddrB) ||
		tMissAddrB ||
//		(tBlkAddrB[1:0] != (~tBlkFlagB[1:0])) ||
		tFlushB;

	tMissC = 1;
	tMissD = 1;
`ifdef jx2_mem_l1i2way

	tMissAddrC = (tBlkAddrC != tReqAddrA);
	tMissAddrD = (tBlkAddrD != tReqAddrB);

	tMissC = 
//		(tBlkAddrC != tReqAddrA) ||
		tMissAddrC ||
//		(tBlkAddrC[1:0] != (~tBlkFlagC[1:0])) ||
		tFlushA;
	tMissD = 
//		(tBlkAddrD != tReqAddrB) ||
		tMissAddrB ||
//		(tBlkAddrD[1:0] != (~tBlkFlagD[1:0])) ||
		tFlushB;
`endif

//	tMissC = 1;
//	tMissD = 1;


`ifdef jx2_mem_l1i2way
	tMiss = (tMissA && tMissC) || (tMissB && tMissD);
`else
	tMiss = tMissA || tMissB;
`endif

	if(reset)
		tMiss = 0;


`ifdef jx2_mem_l1i2way
	if(tMissA && !tMissC)
	begin
		tBlkData2A = tBlkDataC;
		tBlkAddr2A = tBlkAddrC;
		tBlkFlag2A = tBlkFlagC;
	end

	if(tMissB && !tMissD)
	begin
		tBlkData2B = tBlkDataD;
		tBlkAddr2B = tBlkAddrD;
		tBlkFlag2B = tBlkFlagD;
	end
`endif
	
//	if(tInAddr[3])
//	begin
//		tBlkData = { tBlkDataA, tBlkDataB };
//	end else begin
//		tBlkData = { tBlkDataB, tBlkDataA };
//	end

`ifdef jx2_mem_line32B

`ifndef def_true
	case(tInAddr[5:3])
		3'b000: tBlkData = tBlkData2A[159: 0];
		3'b001: tBlkData = tBlkData2A[223:64];
		3'b010: tBlkData = { tBlkData2B[31:0], tBlkData2A[255:128] };
		3'b011: tBlkData = { tBlkData2B[95:0], tBlkData2A[255:192] };
		3'b100: tBlkData = tBlkData2B[159: 0];
		3'b101: tBlkData = tBlkData2B[223:64];
		3'b110: tBlkData = { tBlkData2A[31:0], tBlkData2B[255:128] };
		3'b111: tBlkData = { tBlkData2A[95:0], tBlkData2B[255:192] };
	endcase
`endif

`ifdef def_true
	if(tInAddr[5])
	begin
		tBlkData2A2 = tBlkData2B;
		tBlkData2B2 = tBlkData2A;
	end
	else
	begin
		tBlkData2A2 = tBlkData2A;
		tBlkData2B2 = tBlkData2B;
	end

	if(tInAddr[4])
		tBlkData1A = { tBlkData2B2[127:0], tBlkData2A2[255:128] };
	else
		tBlkData1A = tBlkData2A2[255: 0];

	if(tInAddr[3])
		tBlkData = tBlkData1A[223:64];
	else
		tBlkData = tBlkData1A[159: 0];

`ifndef def_true
	case(tInAddr[4:3])
		2'b00: tBlkData = tBlkData2A2[159: 0];
		2'b01: tBlkData = tBlkData2A2[223:64];
		2'b10: tBlkData = { tBlkData2B2[31:0], tBlkData2A2[255:128] };
		2'b11: tBlkData = { tBlkData2B2[95:0], tBlkData2A2[255:192] };
	endcase
`endif

`endif

`else
	case(tInAddr[4:3])
		2'b00: tBlkData = { tBlkData2B[31:0], tBlkData2A[127: 0] };
		2'b01: tBlkData = { tBlkData2B[95:0], tBlkData2A[127:64] };
		2'b10: tBlkData = { tBlkData2A[31:0], tBlkData2B[127: 0] };
		2'b11: tBlkData = { tBlkData2A[95:0], tBlkData2B[127:64] };
	endcase
`endif

//	$display("BA=%X BB=%X", tBlkDataA, tBlkDataB);
//	$display("A=%X BD=%X Ix=%X",tInAddr, tBlkData, tInWordIx);

`ifndef def_true
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

`ifdef def_true
	casez(tBlkData[12:9])
		4'b1111:	opLenA0=4'b1110;		//FE/FF
		4'b1110:	opLenA0=4'b0110;		//FC/FD
		4'b110z:	opLenA0=4'b0010;		//F8..FB
		4'b101z:	opLenA0=4'b0110;		//F4..F7
		4'b100z:	opLenA0=4'b0010;		//F0..F3
		4'b0111:	opLenA0=4'b0110;		//FE/FF
		4'b0110:	opLenA0=4'b0010;		//FC/FD
		4'b0101:	opLenA0=4'b0110;		//EA/EB
		4'b0100:	opLenA0=4'b0010;		//E8/E9
		4'b00zz:	opLenA0=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[28:25])
		4'b1111:	opLenA1=4'b1110;		//FE/FF
		4'b1110:	opLenA1=4'b0110;		//FC/FD
		4'b110z:	opLenA1=4'b0010;		//F8..FB
		4'b101z:	opLenA1=4'b0110;		//F4..F7
		4'b100z:	opLenA1=4'b0010;		//F0..F3
		4'b0111:	opLenA1=4'b0110;		//FE/FF
		4'b0110:	opLenA1=4'b0010;		//FC/FD
		4'b0101:	opLenA1=4'b0110;		//EA/EB
		4'b0100:	opLenA1=4'b0010;		//E8/E9
		4'b00zz:	opLenA1=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[44:41])
		4'b1111:	opLenA2=4'b1110;		//FE/FF
		4'b1110:	opLenA2=4'b0110;		//FC/FD
		4'b110z:	opLenA2=4'b0010;		//F8..FB
		4'b101z:	opLenA2=4'b0110;		//F4..F7
		4'b100z:	opLenA2=4'b0010;		//F0..F3
		4'b0111:	opLenA2=4'b0110;		//FE/FF
		4'b0110:	opLenA2=4'b0010;		//FC/FD
		4'b0101:	opLenA2=4'b0110;		//EA/EB
		4'b0100:	opLenA2=4'b0010;		//E8/E9
		4'b00zz:	opLenA2=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[60:57])
		4'b1111:	opLenA3=4'b1110;		//FE/FF
		4'b1110:	opLenA3=4'b0110;		//FC/FD
		4'b110z:	opLenA3=4'b0010;		//F8..FB
		4'b101z:	opLenA3=4'b0110;		//F4..F7
		4'b100z:	opLenA3=4'b0010;		//F0..F3
		4'b0111:	opLenA3=4'b0110;		//FE/FF
		4'b0110:	opLenA3=4'b0010;		//FC/FD
		4'b0101:	opLenA3=4'b0110;		//EA/EB
		4'b0100:	opLenA3=4'b0010;		//E8/E9
		4'b00zz:	opLenA3=4'b0010;		//E0..E7
	endcase
	if(tBlkData[15:13]!=3'b111)
		opLenA0=4'b0001;
	if(tBlkData[31:29]!=3'b111)
		opLenA1=4'b0001;
	if(tBlkData[47:45]!=3'b111)
		opLenA2=4'b0001;
	if(tBlkData[63:61]!=3'b111)
		opLenA3=4'b0001;
`endif

`ifdef jx2_enable_wex3w

`ifndef def_true
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

`ifdef def_true
	casez(tBlkData[76:73])
		4'b1111:	opLenA4=4'b1110;		//FE/FF
		4'b1110:	opLenA4=4'b0110;		//FC/FD
		4'b110z:	opLenA4=4'b0010;		//F8..FB
		4'b101z:	opLenA4=4'b0110;		//F4..F7
		4'b100z:	opLenA4=4'b0010;		//F0..F3
		4'b0111:	opLenA4=4'b0110;		//FE/FF
		4'b0110:	opLenA4=4'b0010;		//FC/FD
		4'b0101:	opLenA4=4'b0110;		//EA/EB
		4'b0100:	opLenA4=4'b0010;		//E8/E9
		4'b00zz:	opLenA4=4'b0010;		//E0..E7
	endcase
	casez(tBlkData[92:89])
		4'b1111:	opLenA5=4'b1110;		//FE/FF
		4'b1110:	opLenA5=4'b0110;		//FC/FD
		4'b110z:	opLenA5=4'b0010;		//F8..FB
		4'b101z:	opLenA5=4'b0110;		//F4..F7
		4'b100z:	opLenA5=4'b0010;		//F0..F3
		4'b0111:	opLenA5=4'b0110;		//FE/FF
		4'b0110:	opLenA5=4'b0010;		//FC/FD
		4'b0101:	opLenA5=4'b0110;		//EA/EB
		4'b0100:	opLenA5=4'b0010;		//E8/E9
		4'b00zz:	opLenA5=4'b0010;		//E0..E7
	endcase
	if(tBlkData[79:77]!=3'b111)
		opLenA4=4'b0001;
	if(tBlkData[95:93]!=3'b111)
		opLenA5=4'b0001;
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

	if(tInWordIx[1])
		tBlkData0A = tBlkData[159:32];
	else
		tBlkData0A = tBlkData[127: 0];
	if(tInWordIx[0])
		tRegOutPcVal = tBlkData0A[111:16];
	else
		tRegOutPcVal = tBlkData0A[95: 0];

	case(tInWordIx)
		2'b00: begin
//			tRegOutPcVal	= tBlkData[ 95:  0];
			tRegOutPcStepA	= { 1'b0, opLenA0[1:0] };
			tPcStepWA		= opLenA0[2];
			tPcStepWB		= opLenA2[2];
			tPcStepJA		= opLenA0[3];
			tPcStepBA		= opLenA0[0];
			tPcStepBB		= opLenA2[0];
		end
		2'b01: begin
//			tRegOutPcVal	= tBlkData[111: 16];
			tRegOutPcStepA	= {1'b0, opLenA1[1:0] };
			tPcStepWA		= opLenA1[2];
			tPcStepWB		= opLenA3[2];
			tPcStepJA		= opLenA1[3];
			tPcStepBA		= opLenA1[0];
			tPcStepBB		= opLenA3[0];
		end
		2'b10: begin
//			tRegOutPcVal	= tBlkData[127: 32];
			tRegOutPcStepA	= {1'b0, opLenA2[1:0] };
			tPcStepWA		= opLenA2[2];
			tPcStepWB		= opLenA4[2];
			tPcStepJA		= opLenA2[3];
			tPcStepBA		= opLenA2[0];
			tPcStepBB		= opLenA4[0];
		end
		2'b11: begin
//			tRegOutPcVal	= tBlkData[143: 48];
			tRegOutPcStepA	= {1'b0, opLenA3[1:0] };
			tPcStepWA		= opLenA3[2];
			tPcStepWB		= opLenA5[2];
			tPcStepJA		= opLenA3[3];
			tPcStepBA		= opLenA3[0];
			tPcStepBB		= opLenA5[0];
		end
	endcase
	
`ifdef jx2_enable_wex3w

`ifdef jx2_enable_wexjumbo

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
	
//	if(memPcOK==UMEM_OK_FAULT)
	if(tMemPcOK==UMEM_OK_FAULT)
	begin
		$display("L1I$ Fault");
		tRegOutPcOK = UMEM_OK_FAULT;
	end

end

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;

`reg_tile		tStBlkDataA;
reg[3:0]		tStBlkFlagA;
reg				tDoStBlkA;

`reg_tile		tStBlkDataB;
reg[3:0]		tStBlkFlagB;
reg				tDoStBlkB;

`ifdef jx2_mem_l1i2way
`reg_tile		tStBlkDataC;
reg[3:0]		tStBlkFlagC;
reg				tDoStBlkC;

`reg_tile		tStBlkDataD;
reg[3:0]		tStBlkFlagD;
reg				tDoStBlkD;
`endif

`ifdef jx2_enable_vaddr48
reg[43:0]		tStBlkAddrA;
reg[43:0]		tStBlkAddrB;
`else
reg[27:0]		tStBlkAddrA;
reg[27:0]		tStBlkAddrB;
`endif

`ifdef jx2_mem_l1i2way
`ifdef jx2_enable_vaddr48
reg[43:0]		tStBlkAddrC;
reg[43:0]		tStBlkAddrD;
`else
reg[27:0]		tStBlkAddrC;
reg[27:0]		tStBlkAddrD;
`endif
`endif

`reg_tile_pflag		tStBlkPFlA;
`reg_tile_pflag		tStBlkPFlB;
`reg_tile_pflag		tStBlkPFlC;
`reg_tile_pflag		tStBlkPFlD;
`reg_tile_prov		tStBlkPRovA;
`reg_tile_prov		tStBlkPRovB;
`reg_tile_prov		tStBlkPRovC;
`reg_tile_prov		tStBlkPRovD;



`ifdef jx2_mem_l1isz_256
reg[7:0]		tStBlkIxA;
reg[7:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_128
reg[6:0]		tStBlkIxA;
reg[6:0]		tStBlkIxB;
`endif

`ifdef jx2_mem_l1isz_64
reg[5:0]		tStBlkIxA;
reg[5:0]		tStBlkIxB;
`endif


always @(posedge clock)
begin
	tTlbMissInh		<= tNxtTlbMissInh;
	tFlushRov		<= tNxtFlushRov;
	tAdvFlushRov	<= tNxtAdvFlushRov;

	tMemPcData		<= memPcData;
	tMemPcOK		<= memPcOK;
	tMemNoRwx		<= memNoRwx;

	tRegInSr		<= regInSr;


	/* Stage A */
//	tInAddr		<= icInPcHold ? tInAddr : regInPc;
//	tInAddr		<= regInPc;
	tInAddr		<= tRegInPc;
	tReqAddrA	<= tNxtAddrA;
	tReqAddrB	<= tNxtAddrB;
	tReqIxA		<= tNxtIxA;
	tReqIxB		<= tNxtIxB;

	tReqAddrAL	<= tReqAddrA;
	tReqAddrBL	<= tReqAddrB;
	tReqIxAL	<= tReqIxA;
	tReqIxBL	<= tReqIxB;
	tMissAL		<= tMissA;
	tMissBL		<= tMissB;
	tMissL		<= tMiss;
	tFlushAL	<= tFlushA;
	tFlushBL	<= tFlushB;

	tInOpm		<= tInPcOpm;
	tInOpmB		<= tInOpm;
	tInOpmC		<= tInOpmB;
//	icFlushMskA	<= icNxtFlushMskA;
//	icFlushMskB	<= icNxtFlushMskB;
	icDoFlush	<= icNxtDoFlush;

//	tBlkDataA	<= icCaMemA[tNxtIxA];
//	tBlkDataB	<= icCaMemB[tNxtIxB];
	{ tBlkPFlA, tBlkPRovA, tBlkDataA }	<= icCaMemA[tNxtIxA];
	{ tBlkPFlB, tBlkPRovB, tBlkDataB }	<= icCaMemB[tNxtIxB];
	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNxtIxA];
	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNxtIxB];
//	tBlkFlushA					<= icFlushMskA[tNxtIxA];
//	tBlkFlushB					<= icFlushMskB[tNxtIxB];

`ifdef jx2_mem_l1i2way
//	tBlkDataC	<= icCaMemC[tNxtIxA];
//	tBlkDataD	<= icCaMemD[tNxtIxB];
	{ tBlkPFlC, tBlkPRovC, tBlkDataC }	<= icCaMemC[tNxtIxA];
	{ tBlkPFlD, tBlkPRovD, tBlkDataD }	<= icCaMemD[tNxtIxB];
	{ tBlkFlagC, tBlkAddrC }	<= icCaAddrC[tNxtIxA];
	{ tBlkFlagD, tBlkAddrD }	<= icCaAddrD[tNxtIxB];
`endif

	if(tDoStBlkA)
	begin
//		$display("L1I$: StBlockA A=%X D=%X", tStBlkAddrA, tStBlkDataA);
	
//		icCaMemA[tStBlkIxA]		<= tStBlkDataA;
		icCaMemA[tStBlkIxA]		<= { tStBlkPFlA, tStBlkPRovA, tStBlkDataA };
		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };
		tDoStBlkA				<= 0;
	end

	if(tDoStBlkB)
	begin
//		$display("L1I$: StBlockB A=%X D=%X", tStBlkAddrB, tStBlkDataB);
	
//		icCaMemB[tStBlkIxB]		<= tStBlkDataB;
		icCaMemB[tStBlkIxB]		<= { tStBlkPFlB, tStBlkPRovB, tStBlkDataB };
		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };
		tDoStBlkB				<= 0;
	end

`ifdef jx2_mem_l1i2way
	if(tDoStBlkC)
	begin
//		$display("L1I$: StBlockA A=%X D=%X", tStBlkAddrC, tStBlkDataC);
	
//		icCaMemC[tStBlkIxA]		<= tStBlkDataC;
		icCaMemC[tStBlkIxA]		<= { tStBlkPFlC, tStBlkPRovC, tStBlkDataC };
		icCaAddrC[tStBlkIxA]	<= { tStBlkFlagC, tStBlkAddrC };
		tDoStBlkC				<= 0;
	end

	if(tDoStBlkD)
	begin
//		$display("L1I$: StBlockB A=%X D=%X", tStBlkAddrD, tStBlkDataD);
	
//		icCaMemD[tStBlkIxB]		<= tStBlkDataD;
		icCaMemD[tStBlkIxB]		<= { tStBlkPFlD, tStBlkPRovD, tStBlkDataD };
		icCaAddrD[tStBlkIxB]	<= { tStBlkFlagD, tStBlkAddrD };
		tDoStBlkD				<= 0;
	end
`endif

	/* Stage B */
	
//	if((tMissA || tMemLatchA) && !tMemLatchB && !reset)
	if(((tMissL && tMissAL) || tMemLatchA) && !tMemLatchB && !reset)
	begin

		if(tMemLatchDnA)
//		if(tMemLatchDnA || (tMemPcOK==UMEM_OK_FAULT))
		begin
			tMemPcOpm	<= UMEM_OPM_READY;
			if(tMemPcOK==UMEM_OK_READY)
//			if((tMemPcOK==UMEM_OK_READY) || (tMemPcOK==UMEM_OK_FAULT))
			begin
				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
			end
		end
		else
//			if((tMemPcOK==UMEM_OK_OK) && tMemLatchA)
			if(((tMemPcOK==UMEM_OK_OK) || (tMemPcOK==UMEM_OK_FAULT)) &&
				tMemLatchA)
		begin
`ifdef jx2_mem_l1i2way
			tStBlkDataC		<= tBlkDataA;
			tStBlkAddrC		<= tBlkAddrA;
			tStBlkFlagC		<= tStBlkFlagA;
			tStBlkPRovC		<= tStBlkPRovA;
			tStBlkPFlC		<= tStBlkPFlA;
			tDoStBlkC		<= 1;
//			if(tFlushA)
			if(tFlushAL)
			begin
				tStBlkPRovC		<= 0;
//				tStBlkFlagC[1:0] <= tBlkAddrA[1:0];
			end
`endif

			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnA	<= 1;
			tStBlkDataA		<= tMemPcData;
//			tStBlkAddrA		<= tReqAddrA;
			tStBlkAddrA		<= tReqAddrAL;
//			tStBlkFlagA		<= { 2'b00, ~tReqAddrA[1:0] };
//			tStBlkFlagA		<= { tMemNoRwx[5], tMemNoRwx[2], ~tReqAddrA[1:0] };
			tStBlkFlagA		<= { tMemNoRwx[5], tMemNoRwx[2], ~tReqAddrAL[1:0] };

			tStBlkPRovA		<= tFlushRov;
			tStBlkPFlA		<= 0;

//			tStBlkIxA		<= tReqIxA;
			tStBlkIxA		<= tReqIxAL;
			tDoStBlkA		<= 1;
		end
		else
			if((tMemPcOK==UMEM_OK_HOLD) && tMemLatchA)
		begin
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
//			tMemPcAddr		<= { tReqAddrA, 4'b0 };
			tMemPcAddr		<= { tReqAddrAL, 4'b0 };
		end
		else
			if(tMemPcOK==UMEM_OK_READY)
		begin
			tMemLatchA		<= 1;
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
//			tMemPcAddr		<= { tReqAddrA, 4'b0 };
			tMemPcAddr		<= { tReqAddrAL, 4'b0 };
		end
		else
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
		end

	end
	else
//		if((tMissB || tMemLatchB) && !tMemLatchA && !reset)
		if(((tMissL && tMissBL) || tMemLatchB) && !tMemLatchA && !reset)
	begin

		if(tMemLatchDnB)
		begin
			tMemPcOpm	<= UMEM_OPM_READY;
			if(tMemPcOK==UMEM_OK_READY)
			begin
				tMemLatchB		<= 0;
				tMemLatchDnB	<= 0;
			end
		end
		else
//			if((tMemPcOK==UMEM_OK_OK) && tMemLatchB)
			if(((tMemPcOK==UMEM_OK_OK) || (tMemPcOK==UMEM_OK_FAULT)) && tMemLatchB)
		begin
`ifdef jx2_mem_l1i2way
			tStBlkDataD		<= tBlkDataB;
			tStBlkAddrD		<= tBlkAddrB;
			tStBlkFlagD		<= tStBlkFlagB;
			tStBlkPRovD		<= tStBlkPRovB;
			tStBlkPFlD		<= tStBlkPFlB;
			tDoStBlkD		<= 1;
//			if(tFlushB)
			if(tFlushBL)
			begin
				tStBlkPRovD		<= 0;
//				tStBlkFlagD[1:0] <= tBlkAddrB[1:0];
			end
`endif

			tMemPcOpm		<= UMEM_OPM_READY;
			tMemLatchDnB	<= 1;
			tStBlkDataB		<= tMemPcData;
//			tStBlkAddrB		<= tReqAddrB;
			tStBlkAddrB		<= tReqAddrBL;
//			tStBlkFlagB		<= { 2'b00, ~tReqAddrB[1:0] };
//			tStBlkFlagB		<= { tMemNoRwx[5], tMemNoRwx[2], ~tReqAddrB[1:0] };
			tStBlkFlagB		<= { tMemNoRwx[5], tMemNoRwx[2], ~tReqAddrBL[1:0] };
			tStBlkPRovB		<= tFlushRov;
			tStBlkPFlB		<= 0;

//			tStBlkIxB		<= tReqIxB;
			tStBlkIxB		<= tReqIxBL;
			tDoStBlkB		<= 1;
		end
		else
			if((tMemPcOK==UMEM_OK_HOLD) && tMemLatchB)
		begin
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
//			tMemPcAddr		<= { tReqAddrB, 4'b0 };
			tMemPcAddr		<= { tReqAddrBL, 4'b0 };
		end
		else
			if(tMemPcOK==UMEM_OK_READY)
		begin
			tMemLatchB		<= 1;
			tMemPcOpm		<= UMEM_OPM_RD_TILE;
//			tMemPcAddr		<= { tReqAddrB, 4'b0 };
			tMemPcAddr		<= { tReqAddrBL, 4'b0 };
		end
// `ifndef def_true
`ifdef def_true
		else
		begin
			tMemPcOpm		<= UMEM_OPM_READY;
		end
`endif

	end
	else
	begin
//		if(tMissA || tMissB)
//			$display("L1I$ Sticky Miss, %d %d", tMissA, tMissB);
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

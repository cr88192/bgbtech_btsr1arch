/*
L1 Instruction Cache, WEX
 */

`include "ringbus/RbiDefs.v"

module RbiMemIcWxA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInPc,
	regOutPcVal,	regOutPcOK,
	regOutPcStep,
	icInPcHold,		icInPcWxe,
	icInPcOpm,		regInSr,
	icMemWait,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

input [47: 0]	regInPc;		//input PC address
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 3: 0]	regOutPcStep;	//PC step (Normal Op)
(* max_fanout = 100 *)
	input			icInPcHold;
input			icInPcWxe;
input[5:0]		icInPcOpm;		//OPM (Used for cache-control)

input [63: 0]	regInSr;

output			icMemWait;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
input [ 47:0]	memAddrIn;		//memory input address
output[ 47:0]	memAddrOut;		//memory output address
input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg [63: 0]		tRegInSr;

reg[95:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 3: 0]		tRegOutPcStep;	//PC step (Normal Op)
reg[ 3: 0]		tRegOutPcStepA;	//PC step (Normal Op)

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;

reg			tMemWait;
assign	icMemWait = tMemWait;

reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
reg[ 47:0]		tMemAddrOut;		//memory output address
reg[127:0]		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;


`ifdef jx2_mem_l1isz_512
reg[143:0]		icCaMemA[511:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemB[511:0];		//Local L1 tile memory (Odd)
reg[143:0]		icCaMemC[511:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemD[511:0];		//Local L1 tile memory (Odd)

`ifdef jx2_enable_vaddr48
	reg[47:0]		icCaAddrA[511:0];	//Local L1 tile address
	reg[47:0]		icCaAddrB[511:0];	//Local L1 tile address
	reg[47:0]		icCaAddrC[511:0];	//Local L1 tile address
	reg[47:0]		icCaAddrD[511:0];	//Local L1 tile address
`else
	reg[31:0]		icCaAddrA[511:0];	//Local L1 tile address
	reg[31:0]		icCaAddrB[511:0];	//Local L1 tile address
	reg[31:0]		icCaAddrC[511:0];	//Local L1 tile address
	reg[31:0]		icCaAddrD[511:0];	//Local L1 tile address
`endif
`endif

`ifdef jx2_mem_l1isz_256
reg[143:0]		icCaMemA[255:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemB[255:0];		//Local L1 tile memory (Odd)
reg[143:0]		icCaMemC[255:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemD[255:0];		//Local L1 tile memory (Odd)

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
`endif

`ifdef jx2_mem_l1isz_128
(* ram_style = "distributed" *)
reg[143:0]		icCaMemA[127:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemB[127:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemC[127:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemD[127:0];		//Local L1 tile memory (Odd)

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
reg[143:0]		icCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemB[63:0];		//Local L1 tile memory (Odd)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemC[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
reg[143:0]		icCaMemD[63:0];		//Local L1 tile memory (Odd)
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

`ifdef jx2_mem_l1isz_512
reg[8:0]		tNxtIxA;
reg[8:0]		tNxtIxB;
reg[8:0]		tReqIxA;
reg[8:0]		tReqIxB;
reg[8:0]		tReqIxAL;
reg[8:0]		tReqIxBL;
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

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[127:0]		tBlkDataC;
reg[127:0]		tBlkDataD;
reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData2B;
reg[127:0]		tBlkData2A2;
reg[127:0]		tBlkData2B2;

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

reg[7:0]		tBlkPFlA;
reg[7:0]		tBlkPFlB;
reg[7:0]		tBlkPFlC;
reg[7:0]		tBlkPFlD;
reg[7:0]		tBlkPRovA;
reg[7:0]		tBlkPRovB;
reg[7:0]		tBlkPRovC;
reg[7:0]		tBlkPRovD;

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
reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg[5:0]		tInPcOpm;		//OPM (Used for cache-control)

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

reg[7:0]		tFlushRov;
reg[7:0]		tNxtFlushRov;
reg				tAdvFlushRov;
reg				tNxtAdvFlushRov;


wire			memRingIsIdle;
wire			memRingIsResp;
assign		memRingIsIdle = (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		memRingIsResp =
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire			memRingIsRespOkLd;
wire			memRingIsRespOkLdA;
wire			memRingIsRespOkLdB;
assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkLdA =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkLdB =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b01);


reg[  8:0]		tReqSeqIdxArr[15:0];
reg[ 43:0]		tReqSeqVaArr[15:0];

reg[  8:0]		tReqSeqIdx;
reg[ 43:0]		tReqSeqVa;

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
reg[ 47:0]		tMemAddrReq;
reg[127:0]		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

reg				tMemReqStA;		//Store A
reg				tMemReqStB;		//Store B
reg				tNxtMemReqStA;	//Store A
reg				tNxtMemReqStB;	//Store B

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
reg				tMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
reg				tNxtMemReqLdM;	//Load MMIO

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;

reg[8:0]		tMemSeqIx;
reg[43:0]		tMemSeqVa;

reg				tRegOutHold;
reg				tRegOutHoldL;
reg				tReqReady;

always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tRegOutHold			= 0;
	tMemWait			= 0;

//	if(tMemNoRwx[5])
//		tNxtTlbMissInh = 1;
	
//	if((tInOpmB == UMEM_OPM_LDTLB) || tRegInSr[29])
//		tNxtTlbMissInh = 0;
	

	/* Stage A */

	tRegInPc	= regInPc;

	tInPcOpm	= icInPcOpm;

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

	if(icInPcHold)
	begin
		tRegInPc	= tInAddr;
		tNxtAddrA	= tReqAddrA;
		tNxtAddrB	= tReqAddrB;
	end

`ifdef jx2_mem_l1isz_512
//	tNxtIxA=tNxtAddrA[9:1];
//	tNxtIxB=tNxtAddrB[9:1];
	tNxtIxA=tNxtAddrA[9:1]^tNxtAddrA[18:10];
	tNxtIxB=tNxtAddrB[9:1]^tNxtAddrB[18:10];
`endif

`ifdef jx2_mem_l1isz_256
//	tNxtIxA=tNxtAddrA[8:1];
//	tNxtIxB=tNxtAddrB[8:1];
	tNxtIxA=tNxtAddrA[8:1]^tNxtAddrA[16:9];
	tNxtIxB=tNxtAddrB[8:1]^tNxtAddrB[16:9];
`endif

`ifdef jx2_mem_l1isz_128
	tNxtIxA=tNxtAddrA[7:1];
	tNxtIxB=tNxtAddrB[7:1];
`endif

`ifdef jx2_mem_l1isz_64
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif

	icNxtDoFlush = 0;

	tNxtFlushRov	= tFlushRov;
	tNxtAdvFlushRov	= 0;

	if(reset)
	begin
		tNxtFlushRov		= 0;
	end

	if(((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS)) || reset)
	begin
		icNxtDoFlush = 1;
	end

	if((tFlushRov == 0) && !tAdvFlushRov)
		icNxtDoFlush = 1;
	
	if(icDoFlush)
	begin
		if(!tAdvFlushRov)
		begin
			$display("L1 I$ DoFlush rov=%X", tFlushRov);
			tNxtFlushRov	= tFlushRov+1;
			tNxtAdvFlushRov	= 1;
		end
	end

	/* Stage B */
	
	tStBlkAddrA		= 0;
	tStBlkAddrB		= 0;
	tStBlkDataA		= 0;
	tStBlkDataB		= 0;
	tStBlkFlagA		= 0;
	tStBlkFlagB		= 0;

	tStBlkIxA		= 0;
	tStBlkIxB		= 0;
	tStBlkPFlA		= 0;
	tStBlkPFlB		= 0;
	tStBlkPRovA		= tFlushRov;
	tStBlkPRovB		= tFlushRov;

	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	
	tNxtMemReqStA	= 0;
	tNxtMemReqStB	= 0;
	tNxtMemReqLdM	= 0;
	
	tInWordIx = tInAddr[2:1];

	tFlushA = (tBlkPRovA != tFlushRov) || (tFlushRov == 0);
	tFlushB = (tBlkPRovB != tFlushRov) || (tFlushRov == 0);
	
	tReqReady	= 1;
	
	if(!tTlbMissInh)
	begin
		if(tBlkFlagA[3])
			tFlushA = 1;
		if(tBlkFlagB[3])
			tFlushB = 1;
	end

	tBlkData2A = tBlkDataA;
	tBlkAddr2A = tBlkAddrA;
	tBlkFlag2A = tBlkFlagA;

	tBlkData2B = tBlkDataB;
	tBlkAddr2B = tBlkAddrB;
	tBlkFlag2B = tBlkFlagB;


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
		tMissAddrA ||
		tFlushA;
	tMissB =
		tMissAddrB ||
		tFlushB;

	tMissC = 1;
	tMissD = 1;

	tMiss = tMissA || tMissB;

	if(reset)
		tMiss = 0;

	case(tInAddr[4:3])
		2'b00: tBlkData = { tBlkData2B[31:0], tBlkData2A[127: 0] };
		2'b01: tBlkData = { tBlkData2B[95:0], tBlkData2A[127:64] };
		2'b10: tBlkData = { tBlkData2A[31:0], tBlkData2B[127: 0] };
		2'b11: tBlkData = { tBlkData2A[95:0], tBlkData2B[127:64] };
	endcase

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
			tRegOutPcStepA	= { 1'b0, opLenA0[1:0], 1'b0 };
			tPcStepWA		= opLenA0[2];
			tPcStepWB		= opLenA2[2];
			tPcStepJA		= opLenA0[3];
			tPcStepBA		= opLenA0[0];
			tPcStepBB		= opLenA2[0];
		end
		2'b01: begin
			tRegOutPcStepA	= {1'b0, opLenA1[1:0], 1'b0 };
			tPcStepWA		= opLenA1[2];
			tPcStepWB		= opLenA3[2];
			tPcStepJA		= opLenA1[3];
			tPcStepBA		= opLenA1[0];
			tPcStepBB		= opLenA3[0];
		end
		2'b10: begin
			tRegOutPcStepA	= {1'b0, opLenA2[1:0], 1'b0 };
			tPcStepWA		= opLenA2[2];
			tPcStepWB		= opLenA4[2];
			tPcStepJA		= opLenA2[3];
			tPcStepBA		= opLenA2[0];
			tPcStepBB		= opLenA4[0];
		end
		2'b11: begin
			tRegOutPcStepA	= {1'b0, opLenA3[1:0], 1'b0 };
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
		6'b00zzz0: tRegOutPcStep = 4'b0100;
		6'b00zzz1: tRegOutPcStep = 4'b0010;

		6'b01z00z: tRegOutPcStep = 4'b1000;
		6'b01z01z: tRegOutPcStep = 4'b0110;
		6'b01z1zz: tRegOutPcStep = 4'b1100;

		6'b100zz0: tRegOutPcStep = 4'b0100;
		6'b100zz1: tRegOutPcStep = 4'b0010;
		6'b1010zz: tRegOutPcStep = 4'b1000;
		6'b1011zz: tRegOutPcStep = 4'b1100;

		6'b11z00z: tRegOutPcStep = 4'b1000;
		6'b11z01z: tRegOutPcStep = 4'b0110;
		6'b11z1zz: tRegOutPcStep = 4'b1100;
	endcase

`else

	if(icInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = tPcStepWB ? 4'b1100 : 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end

`endif

`else
	if(icInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end
`endif
	
//	tRegOutPcOK = tMiss ? UMEM_OK_HOLD : UMEM_OK_OK;

	if(reset)
		tMiss = 0;

	if(tMiss)
		tRegOutHold = 1;

//	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;


	tReqSeqIdx = tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa = tReqSeqVaArr[memSeqIn[3:0]];

	if(memRingIsRespOkLdA)
	begin
//		$display("L1 I$: Resp A, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  IX=%X VA=%X", tReqSeqIdx, tReqSeqVa);
	
		tStBlkIxA		= tReqSeqIdx;
		tStBlkPFlA		= 0;
		tStBlkPRovA		= tFlushRov;
		tStBlkDataA		= memDataIn;
		tStBlkFlagA		= memOpmIn[3:0];
		tStBlkAddrA		= tReqSeqVa[43:0];
		tDoStBlkA		= 1;
	end

	if(memRingIsRespOkLdB)
	begin
//		$display("L1 I$: Resp B, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  IX=%X VA=%X", tReqSeqIdx, tReqSeqVa);

		tStBlkIxB		= tReqSeqIdx;
		tStBlkPFlB		= 0;
		tStBlkPRovB		= tFlushRov;
		tStBlkDataB		= memDataIn;
		tStBlkFlagB		= memOpmIn[3:0];
		tStBlkAddrB		= tReqSeqVa[43:0];
		tDoStBlkB		= 1;
	end

	
	/* Miss Handling */

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UV48_00;
	tMemDataReq		= UV128_XX;
	tNxtMemReqLdA	= 0;
	tNxtMemReqLdB	= 0;
	tMemSeqIx		= 0;
	tMemSeqVa		= 0;
	tNxtMemSeqRov	= tMemSeqRov;

	if(reset)
	begin
		tNxtMemReqLdA	= 0;
		tNxtMemReqLdB	= 0;
		tMemSeqIx		= 0;
		tMemSeqVa		= 0;
		tNxtMemSeqRov	= 0;
	end
	else
		if((tMissA || tMissB) && tReqReady)
	begin
//		$display("L1 I$ Miss %X %X %X", tMissA, tMissB, tReqReady);
	
		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;
		tNxtMemSeqRov	= tMemSeqRov;
		tMemWait		= 1;

		if(tMissA && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAddrA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
			tMemAddrReq		= { tReqAddrA, 4'h00 };
			tNxtMemReqLdA = 1;

//			$display("I$ LDA %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxA);
		end
		else if(tMissB && !tMemReqLdB)
		begin

			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxB;
			tMemSeqVa		= tReqAddrB;
			tMemSeqReq		= { unitNodeId, 4'b0100, tMemSeqRov};

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
			tMemAddrReq		= { tReqAddrB, 4'h00 };
			tNxtMemReqLdB = 1;

//			$display("I$ LDB %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxB);
		end
		else
		begin
//			$display("I$ Wait");
		end
	end
	else
	begin
		if(tMemReqLdA || tMemReqLdB)
		begin
			tRegOutHold = 1;
		end
	end

	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;

`ifndef def_true
	if(tRegOutHoldL != tRegOutHold)
	begin
		if(tRegOutHold)
			$display("I$ Hold");
		else
			$display("I$ Unhold");
	end
`endif

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

reg[43:0]		tStBlkAddrA;
reg[43:0]		tStBlkAddrB;

reg[7:0]		tStBlkPFlA;
reg[7:0]		tStBlkPFlB;
reg[7:0]		tStBlkPFlC;
reg[7:0]		tStBlkPFlD;
reg[7:0]		tStBlkPRovA;
reg[7:0]		tStBlkPRovB;
reg[7:0]		tStBlkPRovC;
reg[7:0]		tStBlkPRovD;



`ifdef jx2_mem_l1isz_512
reg[8:0]		tStBlkIxA;
reg[8:0]		tStBlkIxB;
`endif

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

//	tMemPcData		<= memPcData;

	tRegInSr		<= regInSr;

	tRegOutHoldL	<= tRegOutHold;


	/* Stage A */
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
	icDoFlush	<= icNxtDoFlush;

	{ tBlkPFlA, tBlkPRovA, tBlkDataA }	<= icCaMemA[tNxtIxA];
	{ tBlkPFlB, tBlkPRovB, tBlkDataB }	<= icCaMemB[tNxtIxB];
	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNxtIxA];
	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNxtIxB];

	if(tDoStBlkA)
	begin
		icCaMemA[tStBlkIxA]		<= { tStBlkPFlA, tStBlkPRovA, tStBlkDataA };
		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };
//		tDoStBlkA				<= 0;
	end

	if(tDoStBlkB)
	begin
		icCaMemB[tStBlkIxB]		<= { tStBlkPFlB, tStBlkPRovB, tStBlkDataB };
		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };
//		tDoStBlkB				<= 0;
	end


	/* Stage B */

	if(reset)
	begin
		tMemSeqOut  <= 0;
		tMemOpmOut  <= 0;
		tMemAddrOut <= 0;
		tMemDataOut <= 0;

//		tMemSeqOut  <= memSeqIn;
//		tMemOpmOut  <= memOpmIn;
//		tMemAddrOut <= memAddrIn;
//		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;

		tMemReqStA	<= 0;
		tMemReqStB	<= 0;
		tMemReqLdA	<= 0;
		tMemReqLdB	<= 0;
		tMemReqLdM	<= 0;
		tMemSeqRov	<= 0;
	end
	else
		if(memRingIsIdle || memRingIsResp)
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqSent	<= 1;

		tMemReqStA	<= tNxtMemReqStA;
		tMemReqStB	<= tNxtMemReqStB;
		tMemReqLdA	<= tNxtMemReqLdA;
		tMemReqLdB	<= tNxtMemReqLdB;
		tMemReqLdM	<= tNxtMemReqLdM;
		tMemSeqRov	<= tNxtMemSeqRov;

		tReqSeqIdxArr[tMemSeqRov]	<= tMemSeqIx;
		tReqSeqVaArr[tMemSeqRov]	<= tMemSeqVa;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;
	end

end

endmodule

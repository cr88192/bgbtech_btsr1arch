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
	icInPcHold,		icInPcWxm,
	icInPcOpm,		regInSr,
	icMemWait,		regOutExc,
	regOutPcSxo,	regKrrHash,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

`input_vaddr	regInPc;		//input PC address
output[95: 0]	regOutPcVal;	//output PC value
output[ 1: 0]	regOutPcOK;		//set if we have a valid value.
output[ 3: 0]	regOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	regOutPcSxo;
(* max_fanout = 100 *)
	input			icInPcHold;
input[1:0]		icInPcWxm;
input[5:0]		icInPcOpm;		//OPM (Used for cache-control)

input [63: 0]	regInSr;
input [ 7: 0]	regKrrHash;

output			icMemWait;
output[63: 0]	regOutExc;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l1addr	memAddrIn;		//memory input address
`output_l1addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg [63: 0]		tRegInSr;

reg[95:0]		tRegOutPcVal;	//output PC value
reg[ 1:0]		tRegOutPcOK;	//set if we have a valid value.
reg[ 3: 0]		tRegOutPcStep;	//PC step (Normal Op)
reg[ 3: 0]		tRegOutPcStepA;	//PC step (Normal Op)
reg[ 3: 0]		tRegOutPcSxo;	//

assign	regOutPcVal		= tRegOutPcVal;
assign	regOutPcOK		= tRegOutPcOK;
assign	regOutPcStep	= tRegOutPcStep;
assign	regOutPcSxo		= tRegOutPcSxo;

reg			tMemWait;
assign	icMemWait = tMemWait;

reg[63: 0]	tRegOutExc;
reg[63: 0]	tRegOutExc2;
assign	regOutExc = tRegOutExc2;

reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l1addr	tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

wire		icInPcWxe;
assign		icInPcWxe = icInPcWxm[1];


`ifdef jx2_mem_l1isz_512
reg[143:0]		icCaMemA[511:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemB[511:0];		//Local L1 tile memory (Odd)
reg[143:0]		icCaMemC[511:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemD[511:0];		//Local L1 tile memory (Odd)

reg[71:0]		icCaAddrA[511:0];	//Local L1 tile address
reg[71:0]		icCaAddrB[511:0];	//Local L1 tile address
reg[71:0]		icCaAddrC[511:0];	//Local L1 tile address
reg[71:0]		icCaAddrD[511:0];	//Local L1 tile address
`endif

`ifdef jx2_mem_l1isz_256
reg[143:0]		icCaMemA[255:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemB[255:0];		//Local L1 tile memory (Odd)
reg[143:0]		icCaMemC[255:0];		//Local L1 tile memory (Even)
reg[143:0]		icCaMemD[255:0];		//Local L1 tile memory (Odd)

reg[71:0]		icCaAddrA[255:0];	//Local L1 tile address
reg[71:0]		icCaAddrB[255:0];	//Local L1 tile address
reg[71:0]		icCaAddrC[255:0];	//Local L1 tile address
reg[71:0]		icCaAddrD[255:0];	//Local L1 tile address
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

(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrA[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrB[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrC[127:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	`reg_vaddr		icCaAddrD[127:0];	//Local L1 tile address

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

(* ram_style = "distributed" *)
	reg[71:0]		icCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[71:0]		icCaAddrB[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[71:0]		icCaAddrC[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[71:0]		icCaAddrD[63:0];	//Local L1 tile address

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

wire[8:0]		tNx2IxA;
wire[8:0]		tNx2IxB;
`endif

`ifdef jx2_mem_l1isz_256
reg[7:0]		tNxtIxA;
reg[7:0]		tNxtIxB;
reg[7:0]		tReqIxA;
reg[7:0]		tReqIxB;
reg[7:0]		tReqIxAL;
reg[7:0]		tReqIxBL;

wire[7:0]		tNx2IxA;
wire[7:0]		tNx2IxB;
`endif

`ifdef jx2_mem_l1isz_128
reg[6:0]		tNxtIxA;
reg[6:0]		tNxtIxB;
reg[6:0]		tReqIxA;
reg[6:0]		tReqIxB;
reg[6:0]		tReqIxAL;
reg[6:0]		tReqIxBL;

wire[6:0]		tNx2IxA;
wire[6:0]		tNx2IxB;
`endif

`ifdef jx2_mem_l1isz_64
reg[5:0]		tNxtIxA;
reg[5:0]		tNxtIxB;
reg[5:0]		tReqIxA;
reg[5:0]		tReqIxB;
reg[5:0]		tReqIxAL;
reg[5:0]		tReqIxBL;

wire[5:0]		tNx2IxA;
wire[5:0]		tNx2IxB;
`endif

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[127:0]		tBlkDataC;
reg[127:0]		tBlkDataD;
reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData2B;
reg[127:0]		tBlkData2A2;
reg[127:0]		tBlkData2B2;

reg[15:0]		tBlkDextA;
reg[15:0]		tBlkDextB;
reg[15:0]		tBlkDext2A;
reg[15:0]		tBlkDext2B;

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

reg[15:0]		tBlkPFlA;
reg[15:0]		tBlkPFlB;
// reg[7:0]		tBlkPFlC;
// reg[7:0]		tBlkPFlD;
reg[7:0]		tBlkPRovA;
reg[7:0]		tBlkPRovB;
reg[7:0]		tBlkPRovC;
reg[7:0]		tBlkPRovD;

reg[15:0]		tBlkPFl2A;
reg[15:0]		tBlkPFl2B;
reg[7:0]		tBlkPRov2A;
reg[7:0]		tBlkPRov2B;

reg[47:0]		tReqAddrHi;
reg[47:0]		tNxtAddrHi;
reg[15:0]		tReqAddrH;
reg[15:0]		tNxtAddrH;

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

`reg_vaddr		tInAddr;
`reg_vaddr		tInAddrNl;
reg[1:0]		tInWordIx;
`reg_vaddr		tRegInPc;		//input PC address
reg[5:0]		tInOpm;			//OPM (Used for cache-control)
reg[5:0]		tInOpmB;		//OPM (Used for cache-control)
reg[5:0]		tInOpmC;		//OPM (Used for cache-control)
reg[5:0]		tInPcOpm;		//OPM (Used for cache-control)
reg				tInPcWxe;
reg				tInPcRiscv;

reg[3:0]		tInPmode;
reg[3:0]		tNxtInPmode;

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


`ifdef jx2_mem_l1isz_512
reg[  8:0]		tReqSeqIdxArr[15:0];
reg[  8:0]		tReqSeqIdx;
reg[8:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_256
reg[  7:0]		tReqSeqIdxArr[15:0];
reg[  7:0]		tReqSeqIdx;
reg[7:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_128
reg[  6:0]		tReqSeqIdxArr[15:0];
reg[  6:0]		tReqSeqIdx;
reg[6:0]		tMemSeqIx;
`endif

`ifdef jx2_mem_l1isz_64
reg[  5:0]		tReqSeqIdxArr[15:0];
reg[  5:0]		tReqSeqIdx;
reg[5:0]		tMemSeqIx;
`endif

reg[ 43:0]		tReqSeqVaArr[15:0];
reg[ 43:0]		tReqSeqVa;
reg[43:0]		tMemSeqVa;

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l1addr		tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqSent;	//Request Was Sent

// reg				tMemReqStA;		//Store A
// reg				tMemReqStB;		//Store B
// reg				tNxtMemReqStA;	//Store A
// reg				tNxtMemReqStB;	//Store B

reg				tMemReqLdA;		//Load A
reg				tMemReqLdB;		//Load B
// reg				tMemReqLdM;		//Load MMIO
reg				tNxtMemReqLdA;	//Load A
reg				tNxtMemReqLdB;	//Load B
// reg				tNxtMemReqLdM;	//Load MMIO

// reg[ 7:0]		tMemReqRtcnt;
// reg[ 7:0]		tNxtMemReqRtcnt;
reg[11:0]		tMemReqRtcnt;
reg[11:0]		tNxtMemReqRtcnt;

reg[ 3:0]		tMemSeqRov;
reg[ 3:0]		tNxtMemSeqRov;


reg				tRegOutHold;
reg				tRegOutHoldL;
reg				tReqReady;

reg				tBlkIsSxo;
reg				tBlkDoRiscV;
reg				tBlkDoRvC;
reg				tBlkDoRvW;
reg				tDoStallNop;

reg				tMsgLatch;
reg				tNxtMsgLatch;

reg				tMsgLatchTmiss;
reg				tNxtMsgLatchTmiss;

reg		tMemLatchA;
reg		tMemLatchB;
reg		tMemLatchDnA;
reg		tMemLatchDnB;

reg[127:0]		tStBlkDataA;
reg[3:0]		tStBlkFlagA;
reg				tDoStBlkA;
reg				tDidStBlkA;

reg[127:0]		tStBlkDataB;
reg[3:0]		tStBlkFlagB;
reg				tDoStBlkB;
reg				tDidStBlkB;

reg[43:0]		tStBlkAddrA;
reg[43:0]		tStBlkAddrB;

reg[15:0]		tStBlkDextA;
reg[15:0]		tStBlkDextB;

reg[15:0]		tStBlkPFlA;
reg[15:0]		tStBlkPFlB;
// reg[7:0]		tStBlkPFlC;
// reg[7:0]		tStBlkPFlD;
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


always @*
begin
	tNxtTlbMissInh		= tTlbMissInh;
	tRegOutHold			= 0;
	tMemWait			= 0;
	tRegOutExc			= 0;
	tNxtMsgLatch		= 0;
	tNxtMsgLatchTmiss	= 0;

//	if(tMemNoRwx[5])
//		tNxtTlbMissInh = 1;
	
//	if((tInOpmB == JX2_DCOPM_LDTLB) || tRegInSr[29])
	if((tInOpmB == JX2_DCOPM_LDTLB) || (tRegInSr[29] && tRegInSr[30]))
	begin
		if(tTlbMissInh)
			$display("L1I$ Clear TLB Inhibit");
		tNxtTlbMissInh = 0;
	end
	

	/* Stage A */

	tRegInPc	= regInPc;
	tNxtAddrHi	= 0;
	tNxtAddrH	= 0;
`ifdef jx2_enable_vaddr96
	tNxtAddrHi	= regInPc[95:48];
	tNxtAddrH	=
		tNxtAddrHi[15: 0] ^
		tNxtAddrHi[31:16] ^
		tNxtAddrHi[47:32] ;
`endif

	tInPcOpm	= icInPcOpm;
	
//	tNxtInPmode		= regInSr[31:28];
	tNxtInPmode		= regInSr[31:28] ^ regKrrHash[3:0] ^ regKrrHash[7:4];

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
		tNxtAddrHi	= tReqAddrHi;
		tNxtAddrH	= tReqAddrH;
	end

`ifdef jx2_l1i_nohash

`ifdef jx2_mem_l1isz_512
	tNxtIxA=tNxtAddrA[9:1];
	tNxtIxB=tNxtAddrB[9:1];
`endif

`ifdef jx2_mem_l1isz_256
	tNxtIxA=tNxtAddrA[8:1];
	tNxtIxB=tNxtAddrB[8:1];
`endif

`ifdef jx2_mem_l1isz_128
	tNxtIxA=tNxtAddrA[7:1];
	tNxtIxB=tNxtAddrB[7:1];
`endif

`ifdef jx2_mem_l1isz_64
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif

`else

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

`endif

	icNxtDoFlush = 0;

	tNxtFlushRov	= tFlushRov;
	tNxtAdvFlushRov	= 0;

//	if(((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS)) || reset)
//	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS) && !reset)
	if((tInOpmB==JX2_DCOPM_FLUSHIS) && (tInOpmC!=JX2_DCOPM_FLUSHIS))
	begin
		icNxtDoFlush = 1;
	end

//	if((tFlushRov == 0) && !tAdvFlushRov && !reset)
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

	if(reset)
	begin
		tNxtFlushRov		= 0;
		icNxtDoFlush		= 0;
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

	tStBlkDextA		= tReqAddrH;
	tStBlkDextB		= tReqAddrH;

	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	
//	tNxtMemReqStA	= 0;
//	tNxtMemReqStB	= 0;
//	tNxtMemReqLdM	= 0;
	tBlkDoRiscV		= 0;
	tBlkDoRvC		= 0;
	tBlkDoRvW		= 0;
	
	tInWordIx = tInAddr[2:1];

//	tFlushA = (tBlkPRovA != tFlushRov) || (tFlushRov == 0);
//	tFlushB = (tBlkPRovB != tFlushRov) || (tFlushRov == 0);
	tFlushA = tBlkPRovA != tFlushRov;
	tFlushB = tBlkPRovB != tFlushRov;
	
	tReqReady	= 1;
	
	if(!tTlbMissInh)
	begin
//		if(tBlkFlagA[3])
		if(tBlkFlagA[3:2]==2'b11)
		begin
			if(tMemReqRtcnt == 0)
			begin
				if(!tMsgLatch)
					$display("L1I$: TLB Flush Flag A, A=%X", tBlkAddrA);
				tNxtMsgLatch = 1;
			end
			tFlushA = 1;
		end
//		if(tBlkFlagB[3])
		if(tBlkFlagB[3:2]==2'b11)
		begin
			if(tMemReqRtcnt == 0)
			begin
				if(!tMsgLatch)
					$display("L1I$: TLB Flush Flag B, A=%X", tBlkAddrB);
				tNxtMsgLatch = 1;
			end
			tFlushB = 1;
		end

// `ifndef def_true
`ifdef def_true
		if(tBlkPFlA[7:4]!=tInPmode)
		begin
			if(tMemReqRtcnt == 0)
				$display("L1I$: Flush PMode A, A=%X", tBlkAddrA);
			tFlushA = 1;
		end
		if(tBlkPFlB[7:4]!=tInPmode)
		begin
			if(tMemReqRtcnt == 0)
				$display("L1I$: Flush PMode B, A=%X", tBlkAddrB);
			tFlushB = 1;
		end
`endif

	end

	tRegOutExc[63:16] = tInAddr[47:0];
//	if(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])
	if(	(tBlkFlagA[2] && !tBlkFlagA[3] && !tInAddr[4])	||
		(tBlkFlagB[2] && !tBlkFlagB[3] &&  tInAddr[4])	)
			tRegOutExc[15:0] = 16'h8003;
	
	tBlkIsSxo = 0;
//	if(tBlkFlagA[3:0]==4'b1011)
//	if(tBlkFlagA[3] && tBlkFlagA[1] && !tBlkFlagA[2])
	if(	(tBlkFlagA[3] && tBlkFlagA[1] && !tBlkFlagA[2] && !tInAddr[4])	||
		(tBlkFlagB[3] && tBlkFlagB[1] && !tBlkFlagB[2] &&  tInAddr[4])	)
			tBlkIsSxo = 1;


`ifndef def_true
	if((~(tBlkPFlA[3:0])) != tFlushRov[3:0])
		tFlushA = 1;
	if((~(tBlkPFlB[3:0])) != tFlushRov[3:0])
		tFlushB = 1;
`endif

`ifdef def_true
	if((~(tBlkPFlA[2:0])) != tFlushRov[2:0])
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
				$display("L1I$: Rover Flush A");
			tNxtMsgLatch = 1;
		end
		tFlushA = 1;
	end
	if((~(tBlkPFlB[2:0])) != tFlushRov[2:0])
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
				$display("L1I$: Rover Flush B");
			tNxtMsgLatch = 1;
		end
		tFlushB = 1;
	end
`endif

	tBlkData2A	= tBlkDataA;
	tBlkDext2A	= tBlkDextA;
	tBlkAddr2A	= tBlkAddrA;
	tBlkFlag2A	= tBlkFlagA;
	tBlkPFl2A	= tBlkPFlA;

	tBlkData2B	= tBlkDataB;
	tBlkDext2B	= tBlkDextB;
	tBlkAddr2B	= tBlkAddrB;
	tBlkFlag2B	= tBlkFlagB;
	tBlkPFl2B	= tBlkPFlB;


`ifdef def_true
	tMissAddrA =
		(tBlkAddr2A[43:36] != tReqAddrA[43:36]) ||
		(tBlkAddr2A[35:24] != tReqAddrA[35:24]) ||
		(tBlkAddr2A[24:12] != tReqAddrA[24:12]) ||
		(tBlkAddr2A[11: 0] != tReqAddrA[11: 0]) ;
	tMissAddrB =
		(tBlkAddr2B[43:36] != tReqAddrB[43:36]) ||
		(tBlkAddr2B[35:24] != tReqAddrB[35:24]) ||
		(tBlkAddr2B[23:12] != tReqAddrB[23:12]) ||
		(tBlkAddr2B[11: 0] != tReqAddrB[11: 0]) ;
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

	if(tMiss)
		tRegOutExc[15] = 0;

	if(reset)
	begin
		tMiss		= 0;
		tReqReady	= 0;
	end

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

`ifdef jx2_enable_xgpr
// `ifndef def_true
	if(	(tBlkData[15:12]==4'b0111) ||
		(tBlkData[15:12]==4'b1001))
			opLenA0=tBlkData[11]?4'b0110:4'b0010;
	if(	(tBlkData[31:28]==4'b0111) ||
		(tBlkData[31:28]==4'b1001))
			opLenA1=tBlkData[27]?4'b0110:4'b0010;
	if(	(tBlkData[47:44]==4'b0111) ||
		(tBlkData[47:44]==4'b1001))
			opLenA2=tBlkData[43]?4'b0110:4'b0010;
	if(	(tBlkData[63:60]==4'b0111) ||
		(tBlkData[63:60]==4'b1001))
			opLenA3=tBlkData[59]?4'b0110:4'b0010;
`endif

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

`ifdef jx2_enable_xgpr
// `ifndef def_true
	if(	(tBlkData[79:76]==4'b0111) ||
		(tBlkData[79:76]==4'b1001))
			opLenA4=tBlkData[75]?4'b0110:4'b0010;
	if(	(tBlkData[95:92]==4'b0111) ||
		(tBlkData[95:92]==4'b1001))
			opLenA5=tBlkData[91]?4'b0110:4'b0010;
`endif

`ifdef jx2_enable_riscv
	if(tInPcRiscv)
	begin
		tBlkDoRiscV = 1;
		tBlkDoRvC	= 1;
	end

	if(tBlkDoRiscV)
	begin
		opLenA0=4'b0010;
		opLenA1=4'b0010;
		opLenA2=4'b0010;
		opLenA3=4'b0010;
		opLenA4=4'b0010;
		opLenA5=4'b0010;
		
		if(tBlkDoRvC)
		begin
			if(tBlkData[ 1: 0]!=2'b11)
				opLenA0=4'b0001;
			if(tBlkData[17:16]!=2'b11)
				opLenA1=4'b0001;
			if(tBlkData[33:32]!=2'b11)
				opLenA2=4'b0001;
			if(tBlkData[49:48]!=2'b11)
				opLenA3=4'b0001;
			if(tBlkData[65:64]!=2'b11)
				opLenA4=4'b0001;
			if(tBlkData[81:80]!=2'b11)
				opLenA5=4'b0001;
		end

		if(tBlkDoRvW)
		begin
			if(tBlkData[ 1: 0]!=2'b11)
				opLenA0=4'b0110;
			if(tBlkData[33:32]!=2'b11)
				opLenA2=4'b0110;
			if(tBlkData[65:64]!=2'b11)
				opLenA4=4'b0110;
		end
	end
`endif


	tRegOutPcVal	= UV96_00;
	tRegOutPcStep	= 0;
	tRegOutPcStepA	= 0;
	tRegOutPcSxo	= 0;
	tPcStepWA		= 0;
	tPcStepWB		= 0;
	tPcStepJA		= 0;

	if(tBlkIsSxo)
		tRegOutPcSxo[0]=1;

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

//	casez( {icInPcWxe, tPcStepJA,
	casez( {tInPcWxe, tPcStepJA,
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

//	if(icInPcWxe && tPcStepWA)
	if(tInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = tPcStepWB ? 4'b1100 : 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end

`endif

`else
//	if(icInPcWxe && tPcStepWA)
	if(tInPcWxe && tPcStepWA)
	begin
		tRegOutPcStep = 4'b1000;
	end
	else
	begin
		tRegOutPcStep = tRegOutPcStepA;
	end
`endif
	
//	tRegOutPcOK = tMiss ? UMEM_OK_HOLD : UMEM_OK_OK;

//	if(reset)
//	begin
//		tMiss = 0;
//	end

	if(tMiss)
		tRegOutHold = 1;

//	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;


	tReqSeqIdx = tReqSeqIdxArr[memSeqIn[3:0]];
	tReqSeqVa = tReqSeqVaArr[memSeqIn[3:0]];

	if(memRingIsRespOkLdA && !reset)
	begin
//		$display("L1 I$: Resp A, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  Ix=%X VA=%X", tReqSeqIdx, tReqSeqVa);
	
`ifdef jx2_mem_l1inostall
// `ifdef def_true
// `ifndef def_true
		tStBlkIxA		= tReqSeqIdx;
		tStBlkAddrA		= tReqSeqVa[43:0];
`else
		tStBlkIxA		= tReqIxA;
		tStBlkAddrA		= tReqAddrA[43:0];
`endif

//		tStBlkPFlA		= 0;
		tStBlkPFlA		= { 8'h00, tInPmode, ~(tFlushRov[3:0]) };
		tStBlkPRovA		= tFlushRov;
		tStBlkDataA		= memDataIn;
		tStBlkFlagA		= memOpmIn[3:0];

		tDoStBlkA		= 1;

//		if(memOpmIn[3:2]==2'b11)
//		begin
//			tStBlkDataA		= 128'h30003000_30003000_30003000_30003000;
//			tStBlkDataA		= 128'h30023002_30023002_30023002_30023002;
//		end

//		if(memOpmIn[3])
//		if(memOpmIn[3:2]==2'b11)
		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
		begin
			$display("L1I$ Set TLB Inhibit A");
			tNxtTlbMissInh = 1;
		end
	end

	if(memRingIsRespOkLdB && !reset)
	begin
//		$display("L1 I$: Resp B, O=%X S=%X A=%X D=%X",
//			memOpmIn, memSeqIn, memAddrIn, memDataIn);
//		$display("  Ix=%X VA=%X", tReqSeqIdx, tReqSeqVa);

`ifdef jx2_mem_l1inostall
// `ifdef def_true
// `ifndef def_true
		tStBlkIxB		= tReqSeqIdx;
		tStBlkAddrB		= tReqSeqVa[43:0];
`else
		tStBlkIxB		= tReqIxB;
		tStBlkAddrB		= tReqAddrB[43:0];
`endif

//		tStBlkPFlB		= 0;
		tStBlkPFlB		= { 8'h00, tInPmode, ~(tFlushRov[3:0]) };
		tStBlkPRovB		= tFlushRov;
		tStBlkDataB		= memDataIn;
		tStBlkFlagB		= memOpmIn[3:0];
		tDoStBlkB		= 1;

//		if(memOpmIn[3:2]==2'b11)
//		begin
//			tStBlkDataB		= 128'h30003000_30003000_30003000_30003000;
//			tStBlkDataB		= 128'h30023002_30023002_30023002_30023002;
//		end

//		if(memOpmIn[3])
//		if(memOpmIn[3:2]==2'b11)
		if((memOpmIn[3:2]==2'b11) && !(tRegInSr[29] && tRegInSr[30]))
		begin
			$display("L1I$ Set TLB Inhibit B");
			tNxtTlbMissInh = 1;
		end
	end

	
	/* Miss Handling */

	tMemSeqReq		= UV16_00;
	tMemOpmReq		= UV16_00;
	tMemAddrReq		= UVB1AT_00;
	tMemDataReq		= UVTILE_XX;
	tNxtMemReqLdA	= 0;
	tNxtMemReqLdB	= 0;
	tMemSeqIx		= 0;
	tMemSeqVa		= 0;
	tNxtMemSeqRov	= tMemSeqRov;
//	tNxtMemReqRtcnt	= 255;
//	tNxtMemReqRtcnt	= 4095;
	tNxtMemReqRtcnt	= 1023;

	if(reset)
	begin
		tNxtMemReqLdA	= 0;
		tNxtMemReqLdB	= 0;
		tMemSeqIx		= 0;
		tMemSeqVa		= 0;
		tNxtMemSeqRov	= 0;
	end
	else
//		if((tMissA || tMissB) && tReqReady)
		if(tMissA || tMissB)
	begin
		if(tMemReqRtcnt == 0)
		begin
			if(!tMsgLatch)
			begin
				$display("L1 I$ Miss a=%X b=%X Rdy=%X Rt=%x",
					tMissA, tMissB, tReqReady, tMemReqRtcnt);

				$display("  RqA=%X BlkA=%X", tReqAddrA, tBlkAddr2A);
				$display("  RqB=%X BlkB=%X", tReqAddrB, tBlkAddr2B);
			end
			tNxtMsgLatch = 1;
		end

		tNxtMemReqLdA	= tMemReqLdA;
		tNxtMemReqLdB	= tMemReqLdB;
		tNxtMemSeqRov	= tMemSeqRov;
		tMemWait		= 1;

//		tNxtMemReqRtcnt	= tMemReqRtcnt - 1;
		tNxtMemReqRtcnt	= tMemReqRtcnt - ((tMemReqRtcnt != 0)?1:0);

// `ifndef def_true
`ifdef def_true
		if(tMemReqRtcnt == 0)
		begin
			$display("L1I$: Miss Retry");
//			tNxtMemReqRtcnt	= 255;
			tNxtMemReqRtcnt	= 1023;
			tNxtMemReqLdA	= 0;
			tNxtMemReqLdB	= 0;
		end
`endif

		if(!tReqReady)
		begin
		end
		else
			if(tMissA && !tMemReqLdA)
		begin
			tNxtMemSeqRov	= tMemSeqRov + 1;
			tMemSeqIx		= tReqIxA;
			tMemSeqVa		= tReqAddrA;
			tMemSeqReq		= { unitNodeId, 4'b0000, tMemSeqRov };

			tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrA, 4'h00 };
`else
			tMemAddrReq		= { tReqAddrA, 4'h00 };
`endif
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
`ifdef jx2_enable_l1addr96
			tMemAddrReq		= { tReqAddrHi, tReqAddrB, 4'h00 };
`else
			tMemAddrReq		= { tReqAddrB, 4'h00 };
`endif
			tNxtMemReqLdB = 1;

//			$display("I$ LDB %X %X %X Ix=%X",
//				tMemOpmReq, tMemSeqReq, tMemAddrReq, tReqIxB);
		end
		else
		begin
//			$display("I$ Wait");
		end
	end
`ifndef def_true
	else
	begin
		if(tMemReqLdA || tMemReqLdB)
		begin
			tRegOutHold = 1;
		end
	end
`endif

	if(tDoStBlkA || tDoStBlkB)
	begin
		if(!tRegOutHold)
			$display("L1I$: DoSt");
		tRegOutHold = 1;
	end
	if(tDidStBlkA || tDidStBlkB)
	begin
		if(!tRegOutHold)
			$display("L1I$: DidSt");
		tRegOutHold = 1;
	end

	tDoStallNop = 0;
	
	if((tBlkFlagA[3:2]==2'b11) || (tBlkFlagB[3:2]==2'b11))
	begin
		if(!tMsgLatchTmiss)
			$display("L1I$: Hold TMiss A=%X", tInAddr);
		tNxtMsgLatchTmiss = 1;

//		tRegOutPcVal[15:0] = 16'h3002;
		tDoStallNop = 1;
	end

//	if(tRegOutPcVal[15:0]==16'h3002)
//	begin
//		tRegOutPcStep = 0;
//	end

	if((tRegOutPcVal[15:0]==16'h0000) && !tRegOutHold)
	begin
		if(!tMsgLatch)
			$display("L1I$: Op=00 A=%X D=%X", tInAddr, tRegOutPcVal);
		tNxtMsgLatch = 1;
//		tRegOutHold = 1;
	end
		
	if(reset)
		tRegOutHold = 0;

`ifdef jx2_mem_l1inostall
//	if(tRegOutHold)
	if(tRegOutHold || (tInAddrNl[15:0]!=tInAddr[15:0]))
	begin
//		$display("L1I$: Hold NoStall A=%X", tInAddr);
//		tRegOutPcVal[15:0] = 16'h3002;
		tDoStallNop = 1;
	end
//	tRegOutHold = 0;
	tRegOutPcOK = UMEM_OK_OK;

//	if(tRegOutHold)
//	begin
//		tNxtIxA=tReqIxA;
//		tNxtIxB=tReqIxB;
//	end

`else
	tRegOutPcOK = tRegOutHold ? UMEM_OK_HOLD : UMEM_OK_OK;
`endif

	if(tDoStallNop)
	begin
		tRegOutPcVal = 96'h30023002_30023002_30023002;
		if(tInPcRiscv)
			tRegOutPcVal = 96'h00000013_00000013_00000013;
		tRegOutPcStep = 0;
	end

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

assign		tNx2IxA = tRegOutHold ? tReqIxA : tNxtIxA;
assign		tNx2IxB = tRegOutHold ? tReqIxB : tNxtIxB;

always @(posedge clock)
begin
//	tTlbMissInh		<= tNxtTlbMissInh;
//	tFlushRov		<= tNxtFlushRov;
//	tAdvFlushRov	<= tNxtAdvFlushRov;

//	tMemPcData		<= memPcData;

	tRegInSr		<= regInSr;

	tRegOutHoldL	<= tRegOutHold;

	tRegOutExc2		<= tRegOutExc;

	tMsgLatch 		<= tNxtMsgLatch;
	tMsgLatchTmiss	<= tNxtMsgLatchTmiss;

	tMemReqRtcnt	<= tNxtMemReqRtcnt;

	tInAddrNl		<= tRegInPc;

	/* Stage A */
`ifdef jx2_mem_l1inostall
	if(!tRegOutHold)
`else
	if(1'b1)
`endif
	begin
		tInAddr			<= tRegInPc;
		tReqAddrA		<= tNxtAddrA;
		tReqAddrB		<= tNxtAddrB;
		tReqAddrHi		<= tNxtAddrHi;
		tReqAddrH		<= tNxtAddrH;
		tReqIxA			<= tNxtIxA;
		tReqIxB			<= tNxtIxB;

		tTlbMissInh		<= tNxtTlbMissInh;
		tFlushRov		<= tNxtFlushRov;
		tAdvFlushRov	<= tNxtAdvFlushRov;

		tInPmode		<= tNxtInPmode;
		tInPcWxe		<= icInPcWxe;
		tInPcRiscv		<= (icInPcWxm == 2'b01);

	end
	else
	begin
		tTlbMissInh		<= (tTlbMissInh || tNxtTlbMissInh);
	end

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

//	{ tBlkPFlA, tBlkPRovA, tBlkDataA }	<= icCaMemA[tNx2IxA];
//	{ tBlkPFlB, tBlkPRovB, tBlkDataB }	<= icCaMemB[tNx2IxB];
//	{ tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
//	{ tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];

	{ tBlkDextA, tBlkDataA }	<= icCaMemA[tNx2IxA];
	{ tBlkDextB, tBlkDataB }	<= icCaMemB[tNx2IxB];
	{ tBlkPFlA, tBlkPRovA, tBlkFlagA, tBlkAddrA }	<= icCaAddrA[tNx2IxA];
	{ tBlkPFlB, tBlkPRovB, tBlkFlagB, tBlkAddrB }	<= icCaAddrB[tNx2IxB];

	if(tDoStBlkA)
	begin
//		$display("L1I$ DoStBlkA Ix=%X A=%X", tStBlkIxA, tStBlkAddrA);

//		icCaMemA[tStBlkIxA]		<= { tStBlkPFlA, tStBlkPRovA, tStBlkDataA };
//		icCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };

		icCaMemA[tStBlkIxA]		<= { tStBlkDextA, tStBlkDataA };
		icCaAddrA[tStBlkIxA]	<= {
			tStBlkPFlA, tStBlkPRovA,
			tStBlkFlagA, tStBlkAddrA };
	end

	if(tDoStBlkB)
	begin
//		$display("L1I$ DoStBlkB Ix=%X A=%X", tStBlkIxB, tStBlkAddrB);

//		icCaMemB[tStBlkIxB]		<= { tStBlkPFlB, tStBlkPRovB, tStBlkDataB };
//		icCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };

		icCaMemB[tStBlkIxB]		<= { tStBlkDextB, tStBlkDataB };
		icCaAddrB[tStBlkIxB]	<= {
			tStBlkPFlB, tStBlkPRovB,
			tStBlkFlagB, tStBlkAddrB };
	end

	tDidStBlkA		<= tDoStBlkA;
	tDidStBlkB		<= tDoStBlkB;

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

//		tMemReqStA	<= 0;
//		tMemReqStB	<= 0;
		tMemReqLdA	<= 0;
		tMemReqLdB	<= 0;
//		tMemReqLdM	<= 0;
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

`ifdef def_true
//		tMemReqStA	<= tRegOutHold ? tNxtMemReqStA : 0;
//		tMemReqStB	<= tRegOutHold ? tNxtMemReqStB : 0;
		tMemReqLdA	<= tRegOutHold ? tNxtMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tNxtMemReqLdB : 0;
`endif

`ifndef def_true
//		tMemReqStA	<= tNxtMemReqStA;
//		tMemReqStB	<= tNxtMemReqStB;
		tMemReqLdA	<= tNxtMemReqLdA;
		tMemReqLdB	<= tNxtMemReqLdB;
//		tMemReqLdM	<= tNxtMemReqLdM;
`endif
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

`ifdef def_true
		tMemReqLdA	<= tRegOutHold ? tMemReqLdA : 0;
		tMemReqLdB	<= tRegOutHold ? tMemReqLdB : 0;
`endif
	end

end

endmodule

/*
L1 Data Cache.

Reads in blocks, and marks written to blocks as dirty.
Dirty blocks will be written back whenever there is a cache miss.

Flushing the cache will set all the the indices to be flushed.
Flushing an index will cause accesses to always miss.

The behavior will depend on the address during the Flush operation:
	MMIO range: Flush entire cache
	Otherwise: Flush cache lines corresponding to the address given.


Note that a "proper" L1 flush will be a process:
  Flush the L1 D$
  Do a range of memory loads to cause writeback of every dirty cache line.
  Flush the L1 D$ again, effectively invalidating whatever was just loaded.

*/

`include "CoreDefs.v"

module MemDcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regInOpm,
	regOutVal,		regInVal,
	regOutValB,		regInValB,
	regOutOK,		dcInHold,

	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK,
	memNoRwx
	);

input			clock;
input			reset;

input [47: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;		//operation mode

output[63: 0]	regOutVal;		//output data value
input [63: 0]	regInVal;		//input data value
output[ 1: 0]	regOutOK;		//set if we have a valid value.

output[63: 0]	regOutValB;		//output data value (alternate)
input [63: 0]	regInValB;		//input data value (alternate)

(* max_fanout = 100 *)
	input			dcInHold;


output[ 47:0]	memAddr;		//memory address
output[ 47:0]	memAddrB;		//memory address (alternate)
output[  4:0]	memOpm;			//memory operation mode

input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data
input [  1:0]	memOK;			//memory OK
input [  3:0]	memNoRwx;		//No Read/Write/Execute


reg[63:0]		tRegOutVal;		//output data value
reg[63:0]		tRegOutValB;	//output data value (alternate)
reg[ 1:0]		tRegOutOK;		//set if we have a valid value.

`ifdef jx2_do_ld1cyc
assign			regOutVal		= tRegOutVal;
assign			regOutValB		= tRegOutValB;
assign			regOutOK		= tRegOutOK;
`else
reg[63:0]		tRegOutVal2;	//output data value
reg[63:0]		tRegOutValB2;	//output data value (alternate)
reg[ 1:0]		tRegOutOK2;	//set if we have a valid value.
assign			regOutVal		= tRegOutVal2;
assign			regOutValB		= tRegOutValB2;
assign			regOutOK		= tRegOutOK2;
`endif

reg[47:0]		tMemAddr;		//memory PC address (primary)
reg[47:0]		tMemAddrB;		//memory PC address (secondary)
reg[ 4:0]		tMemOpm;		//memory PC output-enable
reg[127:0]		tMemDataOut;	//memory PC address

assign	memAddr		= tMemAddr;
assign	memAddrB	= tMemAddrB;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

`ifdef jx2_expand_l1dsz
	reg[127:0]		dcCaMemA[511:0];	//Local L1 tile memory (Even)
	reg[127:0]		dcCaMemB[511:0];	//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
	reg[ 47:0]		dcCaAddrA[511:0];	//Local L1 tile address
	reg[ 47:0]		dcCaAddrB[511:0];	//Local L1 tile address
`else
	reg[ 31:0]		dcCaAddrA[511:0];	//Local L1 tile address
	reg[ 31:0]		dcCaAddrB[511:0];	//Local L1 tile address
`endif
reg[511:0]			dcFlushMskA;
reg[511:0]			dcFlushMskB;
reg[511:0]			dcNxtFlushMskA;
reg[511:0]			dcNxtFlushMskB;
`else
`ifdef jx2_reduce_l1sz
(* ram_style = "distributed" *)
	reg[127:0]		dcCaMemA[15:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		dcCaMemB[15:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[ 47:0]		dcCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[ 47:0]		dcCaAddrB[15:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[ 31:0]		dcCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[ 31:0]		dcCaAddrB[15:0];	//Local L1 tile address
`endif
reg[15:0]			dcFlushMskA;
reg[15:0]			dcFlushMskB;
reg[15:0]			dcNxtFlushMskA;
reg[15:0]			dcNxtFlushMskB;
`else
(* ram_style = "distributed" *)
	reg[127:0]		dcCaMemA[63:0];		//Local L1 tile memory (Even)
(* ram_style = "distributed" *)
	reg[127:0]		dcCaMemB[63:0];		//Local L1 tile memory (Odd)
`ifdef jx2_enable_vaddr48
(* ram_style = "distributed" *)
	reg[ 47:0]		dcCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[ 47:0]		dcCaAddrB[63:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
	reg[ 31:0]		dcCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
	reg[ 31:0]		dcCaAddrB[63:0];	//Local L1 tile address
`endif
reg[63:0]			dcFlushMskA;
reg[63:0]			dcFlushMskB;
reg[63:0]			dcNxtFlushMskA;
reg[63:0]			dcNxtFlushMskB;
`endif
`endif


reg[9:0]	tDcDoFlushA;
reg[9:0]	tDcDoFlushB;
reg[9:0]	tNxtDcDoFlushA;
reg[9:0]	tNxtDcDoFlushB;

reg[1:0]	memOKL;			//memory PC OK

(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrB;
reg				tNxtIsMmio;

`ifdef jx2_expand_l1dsz
reg[ 8:0]		tNxtIxA;
reg[ 8:0]		tNxtIxB;
reg[ 8:0]		tReqIxA;
reg[ 8:0]		tReqIxB;
reg[ 8:0]		tLstIxA;
reg[ 8:0]		tLstIxB;
reg[ 8:0]		tNx2IxA;
reg[ 8:0]		tNx2IxB;
reg[ 8:0]		tStBlkIxA;
reg[ 8:0]		tStBlkIxB;
reg[ 8:0]		tMiBlkIxA;
reg[ 8:0]		tMiBlkIxB;
reg[ 8:0]		tLstStBlkIxA;
reg[ 8:0]		tLstStBlkIxB;
`else
`ifdef jx2_reduce_l1sz
reg[ 3:0]		tNxtIxA;
reg[ 3:0]		tNxtIxB;
reg[ 3:0]		tReqIxA;
reg[ 3:0]		tReqIxB;
reg[ 3:0]		tLstIxA;
reg[ 3:0]		tLstIxB;
reg[ 3:0]		tNx2IxA;
reg[ 3:0]		tNx2IxB;
reg[ 3:0]		tStBlkIxA;
reg[ 3:0]		tStBlkIxB;
reg[ 3:0]		tMiBlkIxA;
reg[ 3:0]		tMiBlkIxB;
reg[ 3:0]		tLstStBlkIxA;
reg[ 3:0]		tLstStBlkIxB;
`else
reg[ 5:0]		tNxtIxA;
reg[ 5:0]		tNxtIxB;
reg[ 5:0]		tReqIxA;
reg[ 5:0]		tReqIxB;
reg[ 5:0]		tLstIxA;
reg[ 5:0]		tLstIxB;
reg[ 5:0]		tNx2IxA;
reg[ 5:0]		tNx2IxB;
reg[ 5:0]		tStBlkIxA;
reg[ 5:0]		tStBlkIxB;
reg[ 5:0]		tMiBlkIxA;
reg[ 5:0]		tMiBlkIxB;
reg[ 5:0]		tLstStBlkIxA;
reg[ 5:0]		tLstStBlkIxB;
`endif
`endif

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[ 43:0]		tBlkAddrA;
reg[ 43:0]		tBlkAddrB;
reg[  3:0]		tBlkFlagA;
reg[  3:0]		tBlkFlagB;
reg				tBlkDirtyA;
reg				tBlkDirtyB;
reg				tBlkFlushA;
reg				tBlkFlushB;

reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData2B;

reg[ 43:0]		tReqAddrA;
reg[ 43:0]		tReqAddrB;
reg				tReqIsMmio;

// wire[11:0]		tReqL2IxA;
// wire[11:0]		tReqL2IxB;
// wire[11:0]		tBlkL2IxA;
// wire[11:0]		tBlkL2IxB;

wire[5:0]		tReqL2IxA;
wire[5:0]		tReqL2IxB;
wire[5:0]		tBlkL2IxA;
wire[5:0]		tBlkL2IxB;

`ifdef def_true
assign		tReqL2IxA = tReqAddrA[11:6];
assign		tReqL2IxB = tReqAddrB[11:6];
assign		tBlkL2IxA = tBlkAddrA[11:6];
assign		tBlkL2IxB = tBlkAddrB[11:6];
`endif

`ifndef def_true
assign		tReqL2IxA = tReqAddrA[11:6] ^ tReqAddrA[17:12] ^ tReqAddrA[23:18];
assign		tReqL2IxB = tReqAddrB[11:6] ^ tReqAddrB[17:12] ^ tReqAddrB[23:18];
assign		tBlkL2IxA = tBlkAddrA[11:6] ^ tBlkAddrA[17:12] ^ tBlkAddrA[23:18];
assign		tBlkL2IxB = tBlkAddrB[11:6] ^ tBlkAddrB[17:12] ^ tBlkAddrB[23:18];
`endif

`ifndef def_true
assign		tReqL2IxA = tReqAddrA[11:0];
assign		tReqL2IxB = tReqAddrB[11:0];
assign		tBlkL2IxA = tBlkAddrA[11:0];
assign		tBlkL2IxB = tBlkAddrB[11:0];
`endif

`ifndef def_true
assign		tReqL2IxA = tReqAddrA[11:0] ^
	{ tReqAddrA[15:12], tReqAddrA[19:16], tReqAddrA[23:20] };
assign		tReqL2IxB = tReqAddrB[11:0] ^
	{ tReqAddrB[15:12], tReqAddrB[19:16], tReqAddrB[23:20] };
assign		tBlkL2IxA = tBlkAddrA[11:0] ^
	{ tBlkAddrA[15:12], tBlkAddrA[19:16], tBlkAddrA[23:20] };
assign		tBlkL2IxB = tBlkAddrB[11:0] ^
	{ tBlkAddrB[15:12], tBlkAddrB[19:16], tBlkAddrB[23:20] };
`endif

reg[47:0]		tInAddr;
reg[ 2:0]		tInByteIx;
reg[ 4:0]		tInOpm;
reg[63:0]		tDataIn;
reg[63:0]		tDataInB;

reg				tMissA;
reg				tMissB;
reg				tMiss;
reg				tHold;
reg				tHoldB;
reg				tReqOpmNz;
reg				tReqOpmCcmd;
reg				tLstHold;

reg[127:0]		tStBlkDataA;
reg[127:0]		tStBlkDataB;
reg[ 43:0]		tStBlkAddrA;
reg[ 43:0]		tStBlkAddrB;
reg[  3:0]		tStBlkFlagA;
reg[  3:0]		tStBlkFlagB;

reg				tDoStBlkA;
reg				tDoStBlkB;


reg[127:0]		tLstStBlkDataA;
reg[127:0]		tLstStBlkDataB;
reg[ 43:0]		tLstStBlkAddrA;
reg[ 43:0]		tLstStBlkAddrB;
reg[  3:0]		tLstStBlkFlagA;
reg[  3:0]		tLstStBlkFlagB;
reg				tLstDoStBlkA;
reg				tLstDoStBlkB;
//reg[ 5:0]		tLstStBlkIxA;
//reg[ 5:0]		tLstStBlkIxB;


reg[127:0]		tMiBlkDataA;
reg[127:0]		tMiBlkDataB;
reg[ 43:0]		tMiBlkAddrA;
reg[ 43:0]		tMiBlkAddrB;
reg[3:0]		tMiNoRwxA;
reg[3:0]		tMiNoRwxB;
reg				tDoMiBlkA;
reg				tDoMiBlkB;
reg				tDoMiBlk;


reg[127:0]		tBlkData;
reg[127:0]		tBlkDataW;
reg[ 63:0]		tBlkExData;
reg[ 63:0]		tBlkInData;

reg[127:0]		tBlkData1;
reg[ 63:0]		tBlkInData1;

reg[ 95:0]		tBlkExData1;


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
reg		tMmioDoneHld;

reg		tMsgLatch;
reg		tNxtMsgLatch;
reg		tLstMsgLatch;

reg			tHoldWrCyc;
reg[2:0]	tHoldCyc;
reg[2:0]	tNextHoldCyc;

`ifdef jx2_stage_memex3
reg[47: 0]	tRegInAddr;		//input address
reg[ 4: 0]	tRegInOpm;
reg[63: 0]	tRegInVal;
reg[63: 0]	tRegInValB;
`else

wire[47: 0]		tRegInAddr;		//input address
wire[ 4: 0]		tRegInOpm;
wire[63: 0]		tRegInVal;
wire[63: 0]		tRegInValB;

assign	tRegInAddr	= dcInHold ? tInAddr : regInAddr;
// assign	tRegInAddr	= dcInHold ? tInAddr : regInAddr[31:0];
assign	tRegInOpm	= dcInHold ? tInOpm : regInOpm;
assign	tRegInVal	= dcInHold ? tDataIn : regInVal;
assign	tRegInValB	= dcInHold ? tDataInB : regInValB;
`endif

// reg[31: 0]	tMmioInData;
reg[63: 0]	tMmioInData;

reg			tAccFltR;
reg			tAccFltW;
reg			tAccFlt;


always @*
begin
	tNxtMsgLatch = 0;

	/* Stage A */

//	tRegInAddr = regInAddr;
//	tRegInAddr = dcInHold ? tInAddr : regInAddr;
//	tRegInOpm = dcInHold ? tInOpm : regInOpm;
//	tRegInVal	= dcInHold ? tDataIn : regInVal;

`ifdef jx2_stage_memex3
	tRegInAddr = regInAddr;
	tRegInOpm = regInOpm;
	tRegInVal	= regInVal;
	tRegInValB	= regInValB;
`endif

`ifndef def_true
`ifdef def_true
// `ifndef def_true
	if(dcInHold)
	begin
		tRegInAddr	= tInAddr;
		tRegInOpm	= tInOpm;
		tRegInVal	= tDataIn;
	end	else begin
		tRegInAddr	= regInAddr;
		tRegInOpm	= regInOpm;
		tRegInVal	= regInVal;
	end
`else
	tRegInAddr = dcInHold ? tInAddr : regInAddr;
//	tRegInAddr	= regInAddr;
	tRegInOpm	= regInOpm;
	tRegInVal	= regInVal;
`endif
`endif

//	if(regInAddr[4])
	if(tRegInAddr[4])
	begin
//		tNxtAddrB=regInAddr[31:4];
//		tNxtAddrB=tRegInAddr[31:4];
		tNxtAddrB=tRegInAddr[47:4];
		tNxtAddrA=tNxtAddrB+1;
	end else begin
//		tNxtAddrA=regInAddr[31:4];
//		tNxtAddrA=tRegInAddr[31:4];
		tNxtAddrA=tRegInAddr[47:4];
		tNxtAddrB=tNxtAddrA+1;
	end

`ifdef jx2_expand_l1dsz
	tNxtIxA=tNxtAddrA[9:1];
	tNxtIxB=tNxtAddrB[9:1];
`else
`ifdef jx2_reduce_l1sz
	tNxtIxA=tNxtAddrA[4:1];
	tNxtIxB=tNxtAddrB[4:1];
`else
	tNxtIxA=tNxtAddrA[6:1];
	tNxtIxB=tNxtAddrB[6:1];
`endif
`endif

	dcNxtFlushMskA	= dcFlushMskA;
	dcNxtFlushMskB	= dcFlushMskB;
	tNxtDcDoFlushA	= 0;
	tNxtDcDoFlushB	= 0;
	
	if((tRegInOpm==UMEM_OPM_FLUSHDS) || reset)
	begin
		if((tRegInAddr[31:28]==4'hF) || reset)
		begin
//			dcNxtFlushMskA = JX2_L1D_FLUSHMSK;
//			dcNxtFlushMskB = JX2_L1D_FLUSHMSK;
			tNxtDcDoFlushA = 10'h200;
			tNxtDcDoFlushB = 10'h200;
		end
		else
		begin
//			dcNxtFlushMskA[tNxtIxA]=1;
//			dcNxtFlushMskB[tNxtIxB]=1;

			tNxtDcDoFlushA[9] = 1;
			tNxtDcDoFlushB[9] = 1;
`ifdef jx2_expand_l1dsz
			tNxtDcDoFlushA[8:0]=tNxtIxA;
			tNxtDcDoFlushB[8:0]=tNxtIxB;
`else
			tNxtDcDoFlushA[5:0]=tNxtIxA;
			tNxtDcDoFlushB[5:0]=tNxtIxB;
`endif
		end
	end

//	if(tDcDoFlushA[9])
	if(tNxtDcDoFlushA[9])
	begin
		dcNxtFlushMskA = JX2_L1D_FLUSHMSK;
	end
//	if(tDcDoFlushB[9])
	if(tNxtDcDoFlushB[9])
	begin
		dcNxtFlushMskB = JX2_L1D_FLUSHMSK;
	end

//	tNxtIsMmio=(regInAddr[31:28]==4'hA);
//	tNxtIsMmio=(regInAddr[31:28]==4'hF) && (regInOpm[4:3]!=0);
	tNxtIsMmio=(tRegInAddr[31:28]==4'hF) && (tRegInOpm[4:3]!=0);

	tReqOpmCcmd	= (tRegInOpm!=UMEM_OPM_READY) && (tRegInOpm[4:3]==0);	
	if(tReqOpmCcmd)
		tNxtIsMmio = 1;


	/* Stage B */

//	tStBlkDataA		= UV128_XX;
//	tStBlkDataB		= UV128_XX;
//	tStBlkAddrA		= UV28_XX;
//	tStBlkAddrB		= UV28_XX;

	tStBlkDataA		= UV128_00;
	tStBlkDataB		= UV128_00;
//	tStBlkAddrA		= UV28_00;
	tStBlkAddrA		= UV44_00;
//	tStBlkAddrB		= UV28_00;
	tStBlkAddrB		= UV44_00;

	tStBlkFlagA		= 0;
	tStBlkFlagB		= 0;
	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	tHoldWrCyc		= 0;
	tNextHoldCyc	= 0;

	tAccFltR		= 0;
	tAccFltW		= 0;
//	tAccFlt;

	tRegOutVal		= UV64_00;
//	tRegOutValB		= UV64_00;
	tRegOutValB		= UV64_XX;

`ifdef jx2_expand_l1dsz
//	tStBlkIxA	= UV9_XX;
//	tStBlkIxB	= UV9_XX;
	tStBlkIxA	= UV9_00;
	tStBlkIxB	= UV9_00;
`else
`ifdef jx2_reduce_l1sz
//	tStBlkIxA	= UV4_XX;
//	tStBlkIxB	= UV4_XX;
	tStBlkIxA	= UV4_00;
	tStBlkIxB	= UV4_00;
`else
//	tStBlkIxA	= UV6_XX;
//	tStBlkIxB	= UV6_XX;
	tStBlkIxA	= UV6_00;
	tStBlkIxB	= UV6_00;
`endif
`endif

//	tBlkDataA	= dcCaMemA [tReqIxA];
//	tBlkDataB	= dcCaMemB [tReqIxB];
//	{ tBlkFlagA, tBlkAddrA }	= dcCaAddrA[tReqIxA];
//	{ tBlkFlagB, tBlkAddrB }	= dcCaAddrB[tReqIxB];

	tInByteIx	= tInAddr[2:0];
//	tReqOpmNz	= (tInOpm[4:3]!=0);
	tReqOpmNz	= (tInOpm!=UMEM_OPM_READY);
	
//	tReqOpmCcmd	= tReqOpmNz && (tInOpm[4:3]==0);
	
//	if(tReqOpmCcmd)
//		tNxtIsMmio = 1;

	tBlkDirtyA	= tBlkFlagA[2];
	tBlkDirtyB	= tBlkFlagB[2];
	tBlkFlushA	= dcFlushMskA[tReqIxA];
	tBlkFlushB	= dcFlushMskB[tReqIxB];

//	tMissA = (tBlkAddrA != tReqAddrA) || (tBlkAddrA[1:0]!=(~tBlkFlagA[1:0]));
//	tMissB = (tBlkAddrB != tReqAddrB) || (tBlkAddrB[1:0]!=(~tBlkFlagB[1:0]));

	tMissA = (tBlkAddrA != tReqAddrA) ||
		(tBlkAddrA[1:0]!=(~tBlkFlagA[1:0])) || tBlkFlushA;
	tMissB = (tBlkAddrB != tReqAddrB) ||
		(tBlkAddrB[1:0]!=(~tBlkFlagB[1:0])) || tBlkFlushB;

//	tMiss = tReqOpmNz ? (tMissA || tMissB) : 0;
	tMiss = (tReqOpmNz && !tReqIsMmio) ? (tMissA || tMissB) : 0;
	tHold = 0;
	
	tDoMiBlk	= tDoMiBlkA || tDoMiBlkB;

	if(tInOpm[4] && tBlkFlagA[3] && !tMissA)
		tAccFltW	= 1;

//	tAccFltR	= 0;
//	tAccFltW	= 0;

	tAccFlt		= tAccFltR || tAccFltW;

	tBlkData2A = tBlkDataA;
	tBlkData2B = tBlkDataB;

	if((tLstStBlkIxA==tReqIxA) && tLstDoStBlkA)
		tBlkData2A = tLstStBlkDataA;
	if((tLstStBlkIxB==tReqIxB) && tLstDoStBlkB)
		tBlkData2B = tLstStBlkDataB;

	case(tInAddr[4:3])
		2'b00: tBlkData = tBlkData2A;
		2'b01: tBlkData = { tBlkData2B[63:0], tBlkData2A[127:64] };
		2'b10: tBlkData = tBlkData2B;
		2'b11: tBlkData = { tBlkData2A[63:0], tBlkData2B[127:64] };
	endcase

`ifdef jx2_debug_l1ds
	if(tReqOpmNz && !tReqIsMmio)
	begin
		if(!tMsgLatch)
			$display("Addr=%X tBlkData=%X", tInAddr, tBlkData);
		tNxtMsgLatch=1;
	end
`endif

// `ifdef def_true
`ifndef def_true
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
`else
	if(tInByteIx[2])
		tBlkExData1=tBlkData[127: 32];
	else
		tBlkExData1=tBlkData[95: 0];
	case(tInByteIx[1:0])
		2'b00: tBlkExData=tBlkExData1[ 63:  0];
		2'b01: tBlkExData=tBlkExData1[ 71:  8];
		2'b10: tBlkExData=tBlkExData1[ 79: 16];
		2'b11: tBlkExData=tBlkExData1[ 87: 24];
	endcase
`endif

`ifdef jx2_debug_l1ds
	if(tReqOpmNz && !tReqIsMmio)
	begin
		if(!tMsgLatch)
			$display("tInByteIx=%X tBlkExData=%X", tInByteIx, tBlkExData);
		tNxtMsgLatch=1;
	end
`endif
	
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
		3'b111: begin
//			tRegOutVal = tBlkExData[63:0];
			tRegOutVal  = tBlkData[ 63: 0];
			tRegOutValB = tBlkData[127:64];
		end
	endcase
	
`ifdef jx2_debug_l1ds
	if(tReqOpmNz && !tReqIsMmio)
	begin
//		if(!!tMsgLatch)
			$display("tInOpm=%X tRegOutVal=%X A=%X",
				tInOpm, tRegOutVal, tInAddr);
		tNxtMsgLatch=1;
	end
`endif
	
	case(tInOpm[1:0])
		2'b00: tBlkInData = { tBlkExData [63: 8], tDataIn[ 7:0] };
		2'b01: tBlkInData = { tBlkExData [63:16], tDataIn[15:0] };
		2'b10: tBlkInData = { tBlkExData [63:32], tDataIn[31:0] };
		2'b11: tBlkInData = {                     tDataIn[63:0] };
	endcase

	/* Stage C */

`ifndef jx2_mem_do_st2cyc
	tBlkData1		= tBlkData;
	tBlkInData1		= tBlkInData;
`endif

	case(tInByteIx)
		3'b000: tBlkDataW =
			{ tBlkData1[127: 64], tBlkInData1                 };
		3'b001: tBlkDataW =
			{ tBlkData1[127: 72], tBlkInData1, tBlkData1[ 7:0] };
		3'b010: tBlkDataW =
			{ tBlkData1[127: 80], tBlkInData1, tBlkData1[15:0] };
		3'b011: tBlkDataW =
			{ tBlkData1[127: 88], tBlkInData1, tBlkData1[23:0] };
		3'b100: tBlkDataW =
			{ tBlkData1[127: 96], tBlkInData1, tBlkData1[31:0] };
		3'b101: tBlkDataW =
			{ tBlkData1[127:104], tBlkInData1, tBlkData1[39:0] };
		3'b110: tBlkDataW =
			{ tBlkData1[127:112], tBlkInData1, tBlkData1[47:0] };
		3'b111: tBlkDataW =
			{ tBlkData1[127:120], tBlkInData1, tBlkData1[55:0] };
	endcase
	
	if(tInOpm[2:0]==3'b111)
	begin
		tBlkDataW = { tDataInB[63:0], tDataIn[63:0] };
	end

	if(tReqOpmNz && !tReqIsMmio)
		tHold = (tMiss || tDoMiBlk) || (tMemLatchA || tMemLatchB);
		
`ifdef jx2_debug_l1ds
	if(tHold)
	begin
		if(!tMsgLatch)
		begin
			$display("L1D$ Hold, Miss=%d(%d,%d) MiBlk=%d",
				tMiss, tMissA, tMissB, tDoMiBlk);
			if(tMissA)
				$display("L1D$ MissA, Blk=%X, Req=%X", tBlkAddrA, tReqAddrA);
			if(tMissB)
				$display("L1D$ MissB, Blk=%X, Req=%X", tBlkAddrB, tReqAddrB);
		end
		tNxtMsgLatch=1;
	end
`endif

	if(tDoMiBlk)
	begin
`ifdef jx2_debug_l1ds
		if(!tMsgLatch)
		begin
			$display("L1D$ Update Missed Block");
		end
		tNxtMsgLatch=1;
`endif

		tStBlkDataA = tMiBlkDataA;
		tStBlkDataB = tMiBlkDataB;
		tStBlkAddrA	= tMiBlkAddrA;
		tStBlkAddrB	= tMiBlkAddrB;
//		tStBlkFlagA	= { 2'b00, ~tStBlkAddrA[1:0] };
//		tStBlkFlagB	= { 2'b00, ~tStBlkAddrB[1:0] };

		tStBlkFlagA	= { tMiNoRwxA[1], 1'b0, ~tStBlkAddrA[1:0] };
		tStBlkFlagB	= { tMiNoRwxA[1], 1'b0, ~tStBlkAddrB[1:0] };

		dcNxtFlushMskA[tMiBlkIxA] = 0;
		dcNxtFlushMskB[tMiBlkIxB] = 0;

		tStBlkIxA	= tMiBlkIxA;
		tStBlkIxB	= tMiBlkIxB;
		tDoStBlkA	= tDoMiBlkA;
		tDoStBlkB	= tDoMiBlkB;
	end
	else
		if(tInOpm[4] && !tReqIsMmio && !tMiss)
	begin

`ifdef jx2_debug_l1ds
		if(!tMsgLatch)
		begin
			$display("L1D$ Do Write Block A=%X D=%X",
				tInAddr, tBlkDataW);
		end
		tNxtMsgLatch=1;
`endif

		tStBlkAddrA	= tReqAddrA;
		tStBlkAddrB	= tReqAddrB;
		tStBlkFlagA	= { 2'b01, ~tStBlkAddrA[1:0] };
		tStBlkFlagB	= { 2'b01, ~tStBlkAddrB[1:0] };
		tStBlkIxA	= tReqIxA;
		tStBlkIxB	= tReqIxB;
		tDoStBlkA	= 0;
		tDoStBlkB	= 0;
		
`ifdef jx2_mem_do_st2cyc
		if(tHoldCyc!=2)
`else
		if(tHoldCyc!=1)
`endif
		begin
			tNextHoldCyc = tHoldCyc + 1;
			tHoldWrCyc	= 1;
//			tHoldWrCyc	= (tHoldCyc==1);
//			tHoldWrCyc	= (tHoldCyc!=1);
			tHold		= 1;
		end

		case(tInAddr[4:3])
			2'b00: begin
				tStBlkDataA = tBlkDataW;
//				tDoStBlkA	= 1;
				tDoStBlkA	= !tHoldWrCyc;
			end
			2'b01: begin
				tStBlkDataA = { tBlkDataW[ 63: 0], tBlkData2A[ 63: 0] };
				tStBlkDataB = { tBlkData2B[127:64], tBlkDataW[127:64] };
//				tDoStBlkA	= 1;
//				tDoStBlkB	= 1;
				tDoStBlkA	= !tHoldWrCyc;
				tDoStBlkB	= !tHoldWrCyc;
			end
			2'b10: begin
				tStBlkDataB = tBlkDataW;
//				tDoStBlkB	= 1;
				tDoStBlkB	= !tHoldWrCyc;
			end
			2'b11: begin
				tStBlkDataB = { tBlkDataW[ 63: 0], tBlkData2B[ 63: 0] };
				tStBlkDataA = { tBlkData2A[127:64], tBlkDataW[127:64] };
//				tDoStBlkA	= 1;
//				tDoStBlkB	= 1;
				tDoStBlkA	= !tHoldWrCyc;
				tDoStBlkB	= !tHoldWrCyc;
			end
		endcase
	end

	if(tReqIsMmio)
	begin
//		tRegOutVal = {
//			(memDataIn[31] && !tInOpm[2]) ? UV32_FF : UV32_00,
//			memDataIn[31:0]};
//		tRegOutVal = {
//			(tMmioInData[31] && !tInOpm[2]) ? UV32_FF : UV32_00,
//			tMmioInData[31:0]};
//		tRegOutVal = tMmioInData;

		casez(tInOpm[2:0])
			3'b011:
				tRegOutVal = tMmioInData;
			3'b1zz:
				tRegOutVal = { UV32_00, tMmioInData[31:0] };
			default:
				tRegOutVal = {
					tMmioInData[31] ? UV32_FF : UV32_00,
					tMmioInData[31:0]};
		endcase

		if(!tMmioDone || tMmioLatch)
//		if(!tMmioDoneHld || tMmioLatch)
		begin
`ifdef jx2_debug_l1ds
//			if(!tMsgLatch && !tLstMsgLatch)
			if(!tMsgLatch)
				$display("MMIO A=%X Opm=%X V=%X", tInAddr, tInOpm, tRegOutVal);
			tNxtMsgLatch=1;
`endif

			tHold = 1;
		end
	end


//	tHoldB	= tHold || tHoldWrCyc;
	tHoldB	= tHold;

//	tRegOutOK = tHold ? UMEM_OK_HOLD :
	tRegOutOK = tHoldB ? UMEM_OK_HOLD :
		(tReqOpmNz ? UMEM_OK_OK : UMEM_OK_READY);

//	if(memOK==UMEM_OK_FAULT)
//	begin
//		$display("L1D$ Fault");
//		tRegOutOK = UMEM_OK_FAULT;
//	end

`ifdef jx2_stage_memex3
	tNx2IxA		= (dcInHold || tHold) ? tReqIxA : tNxtIxA;
	tNx2IxB		= (dcInHold || tHold) ? tReqIxB : tNxtIxB;
//	tNx2IxA		= dcInHold ? tReqIxA : tNxtIxA;
//	tNx2IxB		= dcInHold ? tReqIxB : tNxtIxB;
//	tNx2IxA		= tLstHold ? tReqIxA : tNxtIxA;
//	tNx2IxB		= tLstHold ? tReqIxB : tNxtIxB;
//	tNx2IxA		= tNxtIxA;
//	tNx2IxB		= tNxtIxB;
`else
//	tNx2IxA		= tLstHold ? tReqIxA : tNxtIxA;
//	tNx2IxB		= tLstHold ? tReqIxB : tNxtIxB;

	tNx2IxA		= tNxtIxA;
	tNx2IxB		= tNxtIxB;
`endif

`ifndef jx2_mem_fulldpx
	tMemAddrB	= 0;
`endif

end

always @(posedge clock)
begin
	tLstMsgLatch	<= tMsgLatch;
	tMsgLatch		<= tNxtMsgLatch;

	/* Stage A */

//	tLstHold	<= tHold;
	tLstHold	<= dcInHold;
	tHoldCyc	<= tNextHoldCyc;
	
	memOKL		<= memOK;

// `ifdef jx2_stage_memex3
`ifndef def_true
//	if(!dcInHold)
	if(1'b1)
	begin
		tRegInAddr		<= regInAddr;
		tRegInOpm		<= regInOpm;
		tRegInVal		<= regInVal;
		tRegInValB		<= regInValB;
	end
`endif

`ifndef def_true
// `ifdef def_true
//	if(!tHold)
//	if(!tHoldB)
//	if(!tHoldB && !dcInHold)
	if(!dcInHold)
	begin
		tInAddr		<= regInAddr;
		tInOpm		<= regInOpm;
		tDataIn		<= regInVal;

		tReqAddrA	<= tNxtAddrA;
		tReqAddrB	<= tNxtAddrB;
		tReqIxA		<= tNxtIxA;
		tReqIxB		<= tNxtIxB;
		tReqIsMmio	<= tNxtIsMmio;
		tMmioDone		<= 0;
		tMmioDoneHld	<= 0;
	end
`endif

`ifdef def_true
// `ifndef def_true
`ifdef jx2_stage_memex3
//	if(!dcInHold)
	if(!dcInHold && !tHold)
	begin
		tInAddr		<= tRegInAddr;
		tInOpm		<= tRegInOpm;
		tDataIn		<= tRegInVal;
		tDataInB	<= tRegInValB;

		tReqAddrA	<= tNxtAddrA;
		tReqAddrB	<= tNxtAddrB;
		tReqIxA		<= tNxtIxA;
		tReqIxB		<= tNxtIxB;
		tReqIsMmio	<= tNxtIsMmio;
	end
`else
	tInAddr		<= tRegInAddr;
	tInOpm		<= tRegInOpm;
	tDataIn		<= tRegInVal;
	tDataInB	<= tRegInValB;

	tReqAddrA	<= tNxtAddrA;
	tReqAddrB	<= tNxtAddrB;
	tReqIxA		<= tNxtIxA;
	tReqIxB		<= tNxtIxB;
	tReqIsMmio	<= tNxtIsMmio;
`endif

`endif

	dcFlushMskA		<= dcNxtFlushMskA;
	dcFlushMskB		<= dcNxtFlushMskB;
	tDcDoFlushA		<= tNxtDcDoFlushA;
	tDcDoFlushB		<= tNxtDcDoFlushB;

	tBlkDataA	<= dcCaMemA [tNx2IxA];
	tBlkDataB	<= dcCaMemB [tNx2IxB];
	{ tBlkFlagA, tBlkAddrA }	<= dcCaAddrA[tNx2IxA];
	{ tBlkFlagB, tBlkAddrB }	<= dcCaAddrB[tNx2IxB];

	tLstStBlkDataA	<= tStBlkDataA;
	tLstStBlkDataB	<= tStBlkDataB;
	tLstStBlkAddrA	<= tStBlkAddrA;
	tLstStBlkAddrB	<= tStBlkAddrB;
	tLstStBlkFlagA	<= tStBlkFlagA;
	tLstStBlkFlagB	<= tStBlkFlagB;
	tLstDoStBlkA	<= tDoStBlkA;
	tLstDoStBlkB	<= tDoStBlkB;
	tLstStBlkIxA	<= tStBlkIxA;
	tLstStBlkIxB	<= tStBlkIxB;

`ifdef jx2_mem_do_st2cyc
	tBlkData1		<= tBlkData;
	tBlkInData1		<= tBlkInData;
`endif

	if(tDoStBlkA)
	begin
`ifdef jx2_debug_l1ds
		if(tStBlkAddrA[0])
			$display("L1D$, DoStBlkA: Even/Odd Mismatch");

		$display("L1D$, DoStBlkA(%X), Data=%X", tStBlkIxA, tStBlkDataA);
`endif
	
		dcCaMemA [tStBlkIxA]	<= tStBlkDataA;
		dcCaAddrA[tStBlkIxA]	<= { tStBlkFlagA, tStBlkAddrA };
		tDoMiBlkA				<= 0;
	end

	if(tDoStBlkB)
	begin
`ifdef jx2_debug_l1ds
		if(!tStBlkAddrB[0])
			$display("L1D$, DoStBlkB: Even/Odd Mismatch");

		$display("L1D$, DoStBlkB(%X), Data=%X", tStBlkIxB, tStBlkDataB);
`endif

		dcCaMemB [tStBlkIxB]	<= tStBlkDataB;
		dcCaAddrB[tStBlkIxB]	<= { tStBlkFlagB, tStBlkAddrB };
		tDoMiBlkB				<= 0;
	end


	/* Stage B */
	
	if(reset)
	begin
		tMmioLatch		<= 0;
		tMmioDone		<= 0;
		tMmioDoneHld	<= 0;

		tMemLatchA		<= 0;
		tMemLatchDnA	<= 0;
		tMemLatchWbA	<= 0;
		tMemLatchWdA	<= 0;

		tMemLatchB		<= 0;
		tMemLatchDnB	<= 0;
		tMemLatchWbB	<= 0;
		tMemLatchWdB	<= 0;
	end
	else
		if(tAccFlt && !tMemLatchA && !tMemLatchB && !tMmioLatch)
	begin
		tMemOpm		<= UMEM_OPM_FAULT;
//		tMemAddr	<= { UV28_00, 2'b00, tAccFltW, tAccFltR };
		tMemAddr	<= { UV44_00, 2'b00, tAccFltW, tAccFltR };
	end
	else
		if(((tMiss && tMissA) || tMemLatchA) && !tMemLatchB && !tMmioLatch)
	begin

`ifdef jx2_debug_l1ds
		$display("L1D$ MissA, Miss=%d Latch=%d OK=%d Dn=%d Wb=%d",
			tMissA, tMemLatchA, memOK, tMemLatchDnA, tMemLatchWbA);
`endif

		if(tMemLatchDnA)
//		if(tMemLatchDnA || (memOK==UMEM_OK_FAULT))
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
//			if((memOK==UMEM_OK_READY) || (memOK==UMEM_OK_FAULT))
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$: MissA Done, A=%X", tBlkAddrA);
`endif

				tMemLatchA		<= 0;
				tMemLatchDnA	<= 0;
				tMemLatchWbA	<= 0;
				tMemLatchWdA	<= 0;
			end
		end
		else
//			if((memOK==UMEM_OK_OK) && tMemLatchA)
			if(((memOK==UMEM_OK_OK) || (memOK==UMEM_OK_FAULT)) && tMemLatchA)
		begin
			tMemOpm			<= UMEM_OPM_READY;
			
			if(!tBlkDirtyA || tMemLatchWdA)
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$: MissA Dn memDataIn=%X A=%X",
					memDataIn, tBlkAddrA);
`endif
			
				tMemLatchDnA	<= 1;
				tMiBlkDataA		<= memDataIn;
				tMiBlkAddrA		<= tReqAddrA;
				tMiBlkIxA		<= tReqIxA;
				tMiNoRwxA		<= memNoRwx;
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
`ifdef jx2_debug_l1ds
				$display("L1D$ MissA ReadySt, A=%X", tBlkAddrA);
				$display("L1D$ MissA, Write=%X", tBlkDataA);
`endif

`ifdef jx2_mem_fulldpx
// `ifndef def_true
//				if((tReqAddrA[27:12]!=0) && (tBlkAddrA[27:12]!=0))
				if((tReqAddrA[27:16]!=0) && (tBlkAddrA[27:16]!=0) &&
//					!tReqAddrA[27] && !tBlkAddrA[27] )
//					!tReqAddrA[27] && !tBlkAddrA[27] &&
//					(tReqAddrA[11:6]!=tBlkAddrA[11:6]))
					!tReqAddrA[27] && !tBlkAddrA[27] &&
					(tReqL2IxA!=tBlkL2IxA))
				begin
					tMemLatchA		<= 1;
					tMemOpm			<= UMEM_OPM_SW_TILE;
					tMemAddr		<= { tReqAddrA, 4'b0 };
					tMemAddrB		<= { tBlkAddrA, 4'b0 };
					tMemDataOut		<= tBlkDataA;
					tMemLatchWdA	<= 1;
				end
				else
				begin
					tMemLatchA		<= 1;
					tMemOpm			<= UMEM_OPM_WR_TILE;
					tMemAddr		<= { tBlkAddrA, 4'b0 };
					tMemDataOut		<= tBlkDataA;
					tMemLatchWdA	<= 0;
				end
`else
				tMemLatchA		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrA, 4'b0 };
				tMemDataOut		<= tBlkDataA;
				tMemLatchWdA	<= 0;
`endif
			end
			else
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$ MissA ReadyLd, A=%X", tBlkAddrA);
`endif

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

`ifdef jx2_debug_l1ds
		$display("L1D$ MissB, Miss=%d Latch=%d", tMissB, tMemLatchB);
`endif

		if(tMemLatchDnB)
//		if(tMemLatchDnB || (memOK==UMEM_OK_FAULT))
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
//			if((memOK==UMEM_OK_READY) || (memOK==UMEM_OK_FAULT))
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$: MissB Done, A=%X", tBlkAddrB);
`endif

				tMemLatchB		<= 0;
				tMemLatchDnB	<= 0;
				tMemLatchWbB	<= 0;
				tMemLatchWdB	<= 0;
			end
		end
		else
//			if((memOK==UMEM_OK_OK) && tMemLatchB)
			if(((memOK==UMEM_OK_OK) || (memOK==UMEM_OK_FAULT)) &&
				tMemLatchB)
		begin
			tMemOpm			<= UMEM_OPM_READY;

			if(!tBlkDirtyB || tMemLatchWdB)
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$: MissB Dn memDataIn=%X A=%X",
					memDataIn, tBlkAddrB);
`endif

				tMemLatchDnB	<= 1;
				tMiBlkDataB		<= memDataIn;
				tMiBlkAddrB		<= tReqAddrB;
				tMiBlkIxB		<= tReqIxB;
				tMiNoRwxB		<= memNoRwx;
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
`ifdef jx2_debug_l1ds
				$display("L1D$ MissB ReadySt A=%X", tBlkAddrB);
				$display("L1D$ MissB, Write=%X", tBlkDataB);
`endif

`ifdef jx2_mem_fulldpx
// `ifndef def_true
//				if((tReqAddrB[27:12]!=0) && (tBlkAddrB[27:12]!=0))
				if((tReqAddrB[27:16]!=0) && (tBlkAddrB[27:16]!=0) &&
//					!tReqAddrB[27] && !tBlkAddrB[27] &&
//					(tReqAddrB[11:6]!=tBlkAddrB[11:6]))
					!tReqAddrB[27] && !tBlkAddrB[27] &&
					(tReqL2IxB!=tBlkL2IxB))
				begin
					tMemLatchB		<= 1;
					tMemOpm			<= UMEM_OPM_SW_TILE;
					tMemAddr		<= { tReqAddrB, 4'b0 };
					tMemAddrB		<= { tBlkAddrB, 4'b0 };
					tMemDataOut		<= tBlkDataB;
					tMemLatchWdB	<= 1;
				end
				else
				begin
					tMemLatchB		<= 1;
					tMemOpm			<= UMEM_OPM_WR_TILE;
					tMemAddr		<= { tBlkAddrB, 4'b0 };
					tMemDataOut		<= tBlkDataB;
					tMemLatchWdB	<= 0;
				end
`else
				tMemLatchB		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrB, 4'b0 };
				tMemDataOut		<= tBlkDataB;
				tMemLatchWdB	<= 0;
`endif
			end
			else
			begin
`ifdef jx2_debug_l1ds
				$display("L1D$ MissB ReadyLd A=%X", tBlkAddrB);
`endif

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
`ifdef jx2_debug_l1ds
		if(tMissA || tMissB)
			$display("L1D$ Sticky Miss, %d %d", tMissA, tMissB);
		if(tMemLatchA || tMemLatchB)
			$display("L1D$ Sticky Latch, %d %d", tMemLatchA, tMemLatchB);
`endif

		tMemLatchDnA	<= 0;
		tMemLatchWbA	<= 0;
		tMemLatchWdA	<= 0;

		tMemLatchDnB	<= 0;
		tMemLatchWbB	<= 0;
		tMemLatchWdB	<= 0;

	end
	
	if(!dcInHold)
	begin
		tMmioDone		<= 0;
		tMmioDoneHld	<= 0;
	end

//	if((tReqIsMmio && !tMemLatchA && !tMemLatchB) || tMmioLatch)
	if(((tReqIsMmio && !tMemLatchA && !tMemLatchB) || tMmioLatch) && !reset)
	begin
		tMemOpm			<= tInOpm;
		tMemAddr		<= tInAddr;
//		tMemDataOut		<= { UV64_XX, tDataIn };
		tMemDataOut		<= { UV64_00, tDataIn };

//		if(tMmioDone || tMmioDoneHld)
		if(tMmioDone)
		begin
//			$display("MMIO Done Hold");
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
//			if(memOKL==UMEM_OK_READY)
			begin
				tMmioLatch	<= 0;
			end
		end
		else
			if((memOK==UMEM_OK_OK) && tMmioLatch)
		begin
//			$display("MMIO Done OK");

			tMemOpm			<= UMEM_OPM_READY;
			tMmioDone		<= 1;
			tMmioDoneHld	<= 1;
//			tMmioInData		<= memDataIn[31:0];
			tMmioInData		<= memDataIn[63:0];
		end
		else
			if((memOK==UMEM_OK_HOLD) && tMmioLatch)
		begin
//			$display("MMIO Hold");

			tMemOpm			<= tInOpm;
			tMemAddr		<= tInAddr;
		end
		else
			if(memOK==UMEM_OK_READY)
		begin
//			$display("MMIO Ready, Latch");

			tMmioLatch		<= 1;
			tMemOpm			<= tInOpm;
			tMemAddr		<= tInAddr;
		end
		else
		begin
//			$display("MMIO Other");
			tMemOpm			<= UMEM_OPM_READY;
		end
	end
	else
	begin
//		if(tMmioDone)
//			$display("MMIO Clear Done");
		tMmioDone		<= 0;

//		if(!dcInHold)
		if(!tMmioDone)
		begin
//			if(tMmioDoneHld)
//				$display("MMIO Clear Done Hold");
			tMmioDoneHld	<= 0;
		end
	end

`ifndef jx2_do_ld1cyc
	tRegOutVal2		<= tRegOutVal;	//output PC value
	tRegOutValB2	<= tRegOutValB;	//output PC value
	tRegOutOK2		<= tRegOutOK;	//set if we have a valid value.
`endif

end

endmodule

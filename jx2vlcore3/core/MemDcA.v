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
	regOutOKB,

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
output[ 1: 0]	regOutOKB;		//set if we have a valid value.

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
input [  5:0]	memNoRwx;		//No Read/Write/Execute


reg[63:0]		tRegOutVal;		//output data value
reg[63:0]		tRegOutValB;	//output data value (alternate)
reg[ 1:0]		tRegOutOK;		//set if we have a valid value.
reg[ 1:0]		tRegOutOKB;		//OK, alternate

`ifdef jx2_do_ld1cyc
assign			regOutVal		= tRegOutVal;
assign			regOutValB		= tRegOutValB;
assign			regOutOK		= tRegOutOK;
assign			regOutOKB		= tRegOutOKB;
`else
reg[63:0]		tRegOutVal2;	//output data value
reg[63:0]		tRegOutValB2;	//output data value (alternate)
reg[ 1:0]		tRegOutOK2;	//set if we have a valid value.
assign			regOutVal		= tRegOutVal2;
assign			regOutValB		= tRegOutValB2;
// assign			regOutOK		= tRegOutOK2;
assign			regOutOK		= tRegOutOK2;
assign			regOutOKB		= tRegOutOKB;
`endif

reg				dcInHoldL;

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
//	reg[ 47:0]		dcCaAddrA[511:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrA[511:0];	//Local L1 tile address
//	reg[ 47:0]		dcCaAddrB[511:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrB[511:0];	//Local L1 tile address
`else
//	reg[ 31:0]		dcCaAddrA[511:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrA[511:0];	//Local L1 tile address
//	reg[ 31:0]		dcCaAddrB[511:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrB[511:0];	//Local L1 tile address
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
//	reg[ 47:0]		dcCaAddrA[15:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
//	reg[ 47:0]		dcCaAddrB[15:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrB[15:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
//	reg[ 31:0]		dcCaAddrA[15:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrA[15:0];	//Local L1 tile address
(* ram_style = "distributed" *)
//	reg[ 31:0]		dcCaAddrB[15:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrB[15:0];	//Local L1 tile address
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
//	reg[ 47:0]		dcCaAddrA[63:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
//	reg[ 47:0]		dcCaAddrB[63:0];	//Local L1 tile address
	reg[ 51:0]		dcCaAddrB[63:0];	//Local L1 tile address
`else
(* ram_style = "distributed" *)
//	reg[ 31:0]		dcCaAddrA[63:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrA[63:0];	//Local L1 tile address
(* ram_style = "distributed" *)
//	reg[ 31:0]		dcCaAddrB[63:0];	//Local L1 tile address
	reg[ 35:0]		dcCaAddrB[63:0];	//Local L1 tile address
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

(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrA;
(* max_fanout = 50 *)
	reg[43:0]		tNxtAddrB;
reg				tNxtIsMmio;

reg[43:0]		tNxtAddrAL;
reg[43:0]		tNxtAddrBL;
reg				tNxtIsMmioL;

`ifdef jx2_expand_l1dsz
reg[ 8:0]		tNxtIxAL;
reg[ 8:0]		tNxtIxBL;
reg[ 8:0]		tNxtIxA;
reg[ 8:0]		tNxtIxB;
reg[ 8:0]		tReqIxA;
reg[ 8:0]		tReqIxB;
reg[ 8:0]		tLstIxA;
reg[ 8:0]		tLstIxB;
reg[ 8:0]		tNx2IxA;
reg[ 8:0]		tNx2IxB;
reg[ 8:0]		tBlkIxA;
reg[ 8:0]		tBlkIxB;
reg[ 8:0]		tBlkIx2A;
reg[ 8:0]		tBlkIx2B;
reg[ 8:0]		tLstBlkIxA;
reg[ 8:0]		tLstBlkIxB;
reg[ 8:0]		tStBlkIxA;
reg[ 8:0]		tStBlkIxB;
reg[ 8:0]		tMiBlkIxA;
reg[ 8:0]		tMiBlkIxB;
reg[ 8:0]		tLstStBlkIxA;
reg[ 8:0]		tLstStBlkIxB;
`else
`ifdef jx2_reduce_l1sz
reg[ 3:0]		tNxtIxAL;
reg[ 3:0]		tNxtIxBL;
reg[ 3:0]		tNxtIxA;
reg[ 3:0]		tNxtIxB;
reg[ 3:0]		tReqIxA;
reg[ 3:0]		tReqIxB;
reg[ 3:0]		tLstIxA;
reg[ 3:0]		tLstIxB;
reg[ 3:0]		tNx2IxA;
reg[ 3:0]		tNx2IxB;
reg[ 3:0]		tBlkIxA;
reg[ 3:0]		tBlkIxB;
reg[ 3:0]		tBlkIx2A;
reg[ 3:0]		tBlkIx2B;
reg[ 3:0]		tLstBlkIxA;
reg[ 3:0]		tLstBlkIxB;
reg[ 3:0]		tStBlkIxA;
reg[ 3:0]		tStBlkIxB;
reg[ 3:0]		tMiBlkIxA;
reg[ 3:0]		tMiBlkIxB;
reg[ 3:0]		tLstStBlkIxA;
reg[ 3:0]		tLstStBlkIxB;
`else
reg[ 5:0]		tNxtIxAL;
reg[ 5:0]		tNxtIxBL;
reg[ 5:0]		tNxtIxA;
reg[ 5:0]		tNxtIxB;
reg[ 5:0]		tReqIxA;
reg[ 5:0]		tReqIxB;
reg[ 5:0]		tLstIxA;
reg[ 5:0]		tLstIxB;
reg[ 5:0]		tNx2IxA;
reg[ 5:0]		tNx2IxB;
reg[ 5:0]		tBlkIxA;
reg[ 5:0]		tBlkIxB;
reg[ 5:0]		tBlkIx2A;
reg[ 5:0]		tBlkIx2B;
reg[ 5:0]		tLstBlkIxA;
reg[ 5:0]		tLstBlkIxB;
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
reg[  7:0]		tBlkFlagA;
reg[  7:0]		tBlkFlagB;
reg				tBlkDirtyA;
reg				tBlkDirtyB;
reg				tBlkFlushA;
reg				tBlkFlushB;
reg				tBlkNoTlbA;
reg				tBlkNoTlbB;
reg				tBlkNoTlb;

reg[127:0]		tLstBlkDataA;
reg[127:0]		tLstBlkDataB;
reg[ 43:0]		tLstBlkAddrA;
reg[ 43:0]		tLstBlkAddrB;
reg[  7:0]		tLstBlkFlagA;
reg[  7:0]		tLstBlkFlagB;

reg[127:0]		tBlkData2A;
reg[127:0]		tBlkData2B;
reg[ 43:0]		tBlkAddr2A;
reg[ 43:0]		tBlkAddr2B;
reg[  7:0]		tBlkFlag2A;
reg[  7:0]		tBlkFlag2B;

reg[ 43:0]		tReqAddrA;
reg[ 43:0]		tReqAddrB;
reg				tReqIsMmio;


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
reg				tLstHoldA;

reg[127:0]		tStBlkDataA;
reg[127:0]		tStBlkDataB;
reg[ 43:0]		tStBlkAddrA;
reg[ 43:0]		tStBlkAddrB;
reg[  7:0]		tStBlkFlagA;
reg[  7:0]		tStBlkFlagB;

reg				tDoStBlkA;
reg				tDoStBlkB;

reg				tTlbMissInh;
reg				tNxtTlbMissInh;


reg[127:0]		tLstStBlkDataA;
reg[127:0]		tLstStBlkDataB;
reg[ 43:0]		tLstStBlkAddrA;
reg[ 43:0]		tLstStBlkAddrB;
reg[  7:0]		tLstStBlkFlagA;
reg[  7:0]		tLstStBlkFlagB;
reg				tLstDoStBlkA;
reg				tLstDoStBlkB;


reg[127:0]		tMiBlkDataA;
reg[127:0]		tMiBlkDataB;
reg[ 43:0]		tMiBlkAddrA;
reg[ 43:0]		tMiBlkAddrB;
// reg[  7:0]		tMiBlkFlagA;
// reg[  7:0]		tMiBlkFlagB;
reg[5:0]		tMiNoRwxA;
reg[5:0]		tMiNoRwxB;
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
reg		tBlkReady;

reg		tMsgLatch;
reg		tNxtMsgLatch;
reg		tLstMsgLatch;

reg			tHoldWrCyc;
reg[2:0]	tHoldCyc;
reg[2:0]	tNextHoldCyc;

reg[47: 0]	tRegInAddrL;		//input address
reg[ 4: 0]	tRegInOpmL;
reg[63: 0]	tRegInValL;
reg[63: 0]	tRegInValBL;


reg[47: 0]	tRegInAddr;		//input address
reg[ 4: 0]	tRegInOpm;
reg[63: 0]	tRegInVal;
reg[63: 0]	tRegInValB;

// reg[31: 0]	tMmioInData;
reg[63: 0]	tMmioInData;

reg			tAccFltR;
reg			tAccFltW;
reg			tAccFlt;

reg[15:0]	tDbgHoldCyc;
reg[15:0]	tNxtDbgHoldCyc;
reg			tDbgHoldLatch;
reg			tNxtDbgHoldLatch;

reg			tStoreHitA;
reg			tStoreHitB;


always @*
begin
	tNxtMsgLatch = 0;

	tNxtDbgHoldCyc		= 0;
	tNxtDbgHoldLatch	= 0;

	tNxtTlbMissInh		= tTlbMissInh;

	/* Stage A */

	tRegInAddr	= regInAddr;
	tRegInOpm	= regInOpm;
	tRegInVal	= regInVal;
	tRegInValB	= regInValB;

	if(tRegInAddr[4])
	begin
		tNxtAddrB=tRegInAddr[47:4];
		tNxtAddrA=tNxtAddrB+1;
	end else begin
		tNxtAddrA=tRegInAddr[47:4];
		tNxtAddrB=tNxtAddrA+1;
	end

`ifdef jx2_expand_l1dsz
//	tNxtIxA=tNxtAddrA[9:1];
//	tNxtIxB=tNxtAddrB[9:1];
	tNxtIxA=tNxtAddrA[9:1]^tNxtAddrA[18:10];
	tNxtIxB=tNxtAddrB[9:1]^tNxtAddrB[18:10];

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

	if(memNoRwx[5])
		tNxtTlbMissInh = 1;
	
//	if(tRegInOpm==UMEM_OPM_READY)
	if(tRegInOpm==UMEM_OPM_LDTLB)
		tNxtTlbMissInh = 0;
	
	if((tRegInOpm==UMEM_OPM_FLUSHDS) || reset)
	begin
		$display("L1D: FLUSH A");
		if((tRegInAddr[31:28]==4'hF) || reset)
		begin
			tNxtDcDoFlushA = 10'h200;
			tNxtDcDoFlushB = 10'h200;
		end
		else
		begin
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

	if(tDcDoFlushA[9])
//	if(tNxtDcDoFlushA[9])
	begin
		$display("L1D: FLUSH B");
//		dcNxtFlushMskA = JX2_L1D_FLUSHMSK;

		if(tDcDoFlushA[8:0]==0)
		begin
			dcNxtFlushMskA = JX2_L1D_FLUSHMSK;
		end
		else
		begin
`ifdef jx2_expand_l1dsz
			dcNxtFlushMskA[tDcDoFlushA[8:0]]=1;
`else
			dcNxtFlushMskA[tDcDoFlushA[5:0]]=1;
`endif
		end

	end

	if(tDcDoFlushB[9])
//	if(tNxtDcDoFlushB[9])
	begin
//		dcNxtFlushMskB = JX2_L1D_FLUSHMSK;

		if(tDcDoFlushB[8:0]==0)
		begin
			dcNxtFlushMskB = JX2_L1D_FLUSHMSK;
		end
		else
		begin
`ifdef jx2_expand_l1dsz
			dcNxtFlushMskB[tDcDoFlushB[8:0]]=1;
`else
			dcNxtFlushMskB[tDcDoFlushB[5:0]]=1;
`endif
		end

	end

	tNxtIsMmio=(tRegInAddr[31:28]==4'hF) && (tRegInOpm[4:3]!=0);

	tReqOpmCcmd	= (tRegInOpm!=UMEM_OPM_READY) && (tRegInOpm[4:3]==0);	
	if(tReqOpmCcmd)
		tNxtIsMmio = 1;

	if(dcInHoldL && !tLstHold)
	begin
	end


	/* Stage B */


	tStBlkDataA		= UV128_00;
	tStBlkDataB		= UV128_00;
	tStBlkAddrA		= UV44_00;
	tStBlkAddrB		= UV44_00;

	tStBlkFlagA		= 0;
	tStBlkFlagB		= 0;
	tDoStBlkA		= 0;
	tDoStBlkB		= 0;
	tHoldWrCyc		= 0;
	tNextHoldCyc	= 0;

	tAccFltR		= 0;
	tAccFltW		= 0;

	tRegOutVal		= UV64_00;
//	tRegOutValB		= UV64_00;
	tRegOutValB		= UV64_XX;

`ifdef jx2_expand_l1dsz
	tStBlkIxA	= UV9_00;
	tStBlkIxB	= UV9_00;
`else
`ifdef jx2_reduce_l1sz
	tStBlkIxA	= UV4_00;
	tStBlkIxB	= UV4_00;
`else
	tStBlkIxA	= UV6_00;
	tStBlkIxB	= UV6_00;
`endif
`endif

	tInByteIx	= tInAddr[2:0];
//	tReqOpmNz	= (tInOpm[4:3]!=0);
	tReqOpmNz	= (tInOpm!=UMEM_OPM_READY);


	tBlkData2A	= tBlkDataA;
	tBlkData2B	= tBlkDataB;
	tBlkAddr2A	= tBlkAddrA;
	tBlkAddr2B	= tBlkAddrB;
	tBlkFlag2A	= tBlkFlagA;
	tBlkFlag2B	= tBlkFlagB;
	tBlkIx2A	= tBlkIxA;
	tBlkIx2B	= tBlkIxB;

	tStoreHitA = 0;
	tStoreHitB = 0;

`ifdef jx2_mem_fwstore
	if((tLstStBlkIxA==tReqIxA) && tLstDoStBlkA)
	begin
		tBlkData2A	= tLstStBlkDataA;
		tBlkAddr2A	= tLstStBlkAddrA;
		tBlkFlag2A	= tLstStBlkFlagA;
		tBlkIx2A	= tLstStBlkIxA;
	end
	if((tLstStBlkIxB==tReqIxB) && tLstDoStBlkB)
	begin
		tBlkData2B	= tLstStBlkDataB;
		tBlkAddr2B	= tLstStBlkAddrB;
		tBlkFlag2B	= tLstStBlkFlagB;
		tBlkIx2B	= tLstStBlkIxB;
	end
`endif

`ifndef jx2_mem_fwstore
	if((tLstStBlkIxA==tReqIxA) && tLstDoStBlkA)
	begin
		tStoreHitA = 1;
	end
	if((tLstStBlkIxB==tReqIxB) && tLstDoStBlkB)
	begin
		tStoreHitB = 1;
	end
`endif


	tBlkDirtyA	= tBlkFlag2A[2];
	tBlkDirtyB	= tBlkFlag2B[2];
	tBlkFlushA	= dcFlushMskA[tReqIxA];
	tBlkFlushB	= dcFlushMskB[tReqIxB];
	tBlkNoTlbA	= tBlkFlag2A[7];
	tBlkNoTlbB	= tBlkFlag2B[7];
	
	tBlkNoTlb	= tBlkNoTlbA || tBlkNoTlbB;
	
	if(!tTlbMissInh)
	begin
		if(tBlkNoTlbA)
		begin
			tBlkDirtyA = 0;
			tBlkFlushA = 1;
		end
		if(tBlkNoTlbB)
		begin
			tBlkDirtyB = 0;
			tBlkFlushB = 1;
		end
	end

	tMissA = (tBlkAddr2A != tReqAddrA) ||
		(tBlkAddr2A[1:0]!=(~tBlkFlagA[1:0])) || tBlkFlushA;
	tMissB = (tBlkAddr2B != tReqAddrB) ||
		(tBlkAddr2B[1:0]!=(~tBlkFlagB[1:0])) || tBlkFlushB;

	tMiss = (tReqOpmNz && !tReqIsMmio) ? (tMissA || tMissB) : 0;
	tHold = 0;

	tBlkReady = 1;

	if((tBlkIx2A!=tReqIxA) || (tBlkIx2B!=tReqIxB))
	begin
`ifdef jx2_debug_l1ds
		$display("MemDcA: Not Ready");
`endif
		tHold		= 1;
		tBlkReady	= 0;
	end

	if(tStoreHitA || tStoreHitB)
	begin
`ifdef jx2_debug_l1ds
		$display("MemDcA: Store Hit Stall");
`endif
		tHold		= 1;
		tBlkReady	= 0;
	end


	tDoMiBlk	= tDoMiBlkA || tDoMiBlkB;

	if(tInOpm[4] && tBlkFlag2A[3] && !tMissA)
		tAccFltW	= 1;

	tAccFlt		= tAccFltR || tAccFltW;

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
			tRegOutVal  = tBlkData[ 63: 0];
			tRegOutValB = tBlkData[127:64];
		end
	endcase
	
`ifdef jx2_debug_l1ds
	if(tReqOpmNz && !tReqIsMmio)
	begin
//		if(!tMsgLatch)
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
		if(tInByteIx!=0)
		begin
			$display("L1D: MOV.X Misaligned, A=%X", tInAddr);
		end

		tBlkDataW = { tDataInB[63:0], tDataIn[63:0] };
	end

	if(tReqOpmNz && !tReqIsMmio)
	begin
		tHold = (tMiss || tDoMiBlk || !tBlkReady) ||
			(tMemLatchA || tMemLatchB);
	end
	
`ifdef jx2_debug_l1ds
	if(tHold)
	begin
		if(!tMsgLatch)
		begin
			$display("L1D$ Hold, Miss=%d(%d,%d) MiBlk=%d Rdy=%d",
				tMiss, tMissA, tMissB, tDoMiBlk, tBlkReady);
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

//		tStBlkFlagA	= { tMiBlkFlagA[7:4],
//			tMiNoRwxA[1], 1'b0, ~tStBlkAddrA[1:0] };
//		tStBlkFlagB	= { tMiBlkFlagB[7:4],
//			tMiNoRwxA[1], 1'b0, ~tStBlkAddrB[1:0] };

//		tStBlkFlagA	= { tMiNoRwxA[3:0],
		tStBlkFlagA	= { tMiNoRwxA[5], tMiNoRwxA[3:2], tMiNoRwxA[0],
			tMiNoRwxA[1], 1'b0, ~tStBlkAddrA[1:0] };
//		tStBlkFlagB	= { tMiNoRwxB[3:0],
		tStBlkFlagB	= { tMiNoRwxB[5], tMiNoRwxB[3:2], tMiNoRwxB[0],
			tMiNoRwxB[1], 1'b0, ~tStBlkAddrB[1:0] };

		dcNxtFlushMskA[tMiBlkIxA] = 0;
		dcNxtFlushMskB[tMiBlkIxB] = 0;

		tStBlkIxA	= tMiBlkIxA;
		tStBlkIxB	= tMiBlkIxB;
		tDoStBlkA	= tDoMiBlkA;
		tDoStBlkB	= tDoMiBlkB;
	end
	else
//		if(tInOpm[4] && !tReqIsMmio && !tMiss && tBlkReady)
		if(tInOpm[4] && !tReqIsMmio && !tMiss && tBlkReady && !tBlkNoTlb)
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
		tStBlkFlagA	= { tBlkFlagA[7:4], 2'b01, ~tStBlkAddrA[1:0] };
		tStBlkFlagB	= { tBlkFlagB[7:4], 2'b01, ~tStBlkAddrB[1:0] };
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

//	if(tReqIsMmio)
	if(tReqIsMmio || tMmioLatch)
	begin
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

//	tHoldB	= tHold || dcInHold;
	tHoldB	= tHold;
//	tHoldB	= tHold || tLstHoldA;

	tRegOutOK = tHold ? UMEM_OK_HOLD :
		(tReqOpmNz ? UMEM_OK_OK : UMEM_OK_READY);

	tRegOutOKB = tRegOutOK;

//	if(memOK==UMEM_OK_FAULT)
//	begin
//		$display("L1D$ Fault");
//		tRegOutOK = UMEM_OK_FAULT;
//	end

	tNx2IxA		= (tHoldB || dcInHoldL) ? tReqIxA : tNxtIxA;
	tNx2IxB		= (tHoldB || dcInHoldL) ? tReqIxB : tNxtIxB;

	tMemAddrB	= 0;

	if(tHold)
	begin
		tNxtDbgHoldCyc		= tDbgHoldCyc + 1;
		tNxtDbgHoldLatch	= tDbgHoldLatch;
		if(tDbgHoldCyc>65280)
		begin
			tNxtDbgHoldCyc		= 65535;
			tNxtDbgHoldLatch	= 1;
			
			if(!tDbgHoldLatch)
			begin
				$display("MemDcA: Deadlock Detect");

				$display("isMmio=%d mmioLatch=%d mmioDone=%d",
					tReqIsMmio, tMmioLatch, tMmioDone);

				$display(
					"memAddr=%X memAddrB=%X memDataIn=%X memDataOut=%X",
					memAddr,		memAddrB,
					memDataIn,		memDataOut);
				$display(
					"memOpm=%X memOK=%X memNoRwx=%X",
					memOpm,			memOK,
					memNoRwx		);

				$display("memLatchA=%d memLatchB=%d",
					tMemLatchA, tMemLatchB);
				$display("memLatchDnA=%d memLatchDnB=%d",
					tMemLatchDnA, tMemLatchDnB);
				$display("memLatchWbA=%d memLatchWbB=%d",
					tMemLatchWbA, tMemLatchWbB);
				$display("memLatchWdA=%d memLatchWdB=%d",
					tMemLatchWdA, tMemLatchWdB);

				$display("A=%X Opm=%X V=%X", tInAddr, tInOpm, tRegOutVal);

				$display("L1D$ Hold, Miss=%d(%d,%d) MiBlk=%d Rdy=%d",
					tMiss, tMissA, tMissB, tDoMiBlk, tBlkReady);
				if(tMissA)
					$display("L1D$ MissA, Blk=%X, Req=%X",
						tBlkAddrA, tReqAddrA);
				if(tMissB)
					$display("L1D$ MissB, Blk=%X, Req=%X",
						tBlkAddrB, tReqAddrB);

			end
		end
	end

end

always @(posedge clock)
begin
	tLstMsgLatch	<= tMsgLatch;
//	tMsgLatch		<= tNxtMsgLatch;
	tMsgLatch		<= 0;

	tDbgHoldCyc		<= tNxtDbgHoldCyc;
	tDbgHoldLatch	<= tNxtDbgHoldLatch;

	tTlbMissInh		<= tNxtTlbMissInh;

	/* Stage A */

	tLstHoldA	<= tHold;
	tLstHold	<= tHoldB;
//	tLstHold	<= dcInHold;
	dcInHoldL	<= dcInHold;
	tHoldCyc	<= tNextHoldCyc;

	if(!dcInHold)
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

	dcFlushMskA		<= dcNxtFlushMskA;
	dcFlushMskB		<= dcNxtFlushMskB;
	tDcDoFlushA		<= tNxtDcDoFlushA;
	tDcDoFlushB		<= tNxtDcDoFlushB;

	tBlkDataA					<= dcCaMemA [tNx2IxA];
	tBlkDataB					<= dcCaMemB [tNx2IxB];
	{ tBlkFlagA, tBlkAddrA }	<= dcCaAddrA[tNx2IxA];
	{ tBlkFlagB, tBlkAddrB }	<= dcCaAddrB[tNx2IxB];
	tBlkIxA						<= tNx2IxA;
	tBlkIxB						<= tNx2IxB;

	tLstBlkDataA	<= tBlkData2A;
	tLstBlkDataB	<= tBlkData2B;
	tLstBlkAddrA	<= tBlkAddr2A;
	tLstBlkAddrB	<= tBlkAddr2B;
	tLstBlkFlagA	<= tBlkFlag2A;
	tLstBlkFlagB	<= tBlkFlag2B;
	tLstBlkIxA		<= tBlkIx2A;
	tLstBlkIxB		<= tBlkIx2B;

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
		tMemAddr	<= { UV44_00, 2'b00, tAccFltW, tAccFltR };
	end
	else
		if(((tMiss && tMissA && tBlkReady) || tMemLatchA) &&
			!tMemLatchB && !tMmioLatch)
	begin

`ifdef jx2_debug_l1ds
		$display("L1D$ MissA, Miss=%d Latch=%d OK=%d Dn=%d Wb=%d",
			tMissA, tMemLatchA, memOK, tMemLatchDnA, tMemLatchWbA);
`endif

		if(tMemLatchDnA)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
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
				tMiNoRwxA		<= memNoRwx[5:0];
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

				tMemLatchA		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrA, 4'b0 };
				tMemDataOut		<= tBlkDataA;
				tMemLatchWdA	<= 0;
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
		if(((tMiss && tMissB && tBlkReady) || tMemLatchB) && !tMemLatchA &&
			!tMmioLatch)
	begin

`ifdef jx2_debug_l1ds
		$display("L1D$ MissB, Miss=%d Latch=%d", tMissB, tMemLatchB);
`endif

		if(tMemLatchDnB)
		begin
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
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
				tMiNoRwxB		<= memNoRwx[5:0];
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

				tMemLatchB		<= 1;
				tMemOpm			<= UMEM_OPM_WR_TILE;
				tMemAddr		<= { tBlkAddrB, 4'b0 };
				tMemDataOut		<= tBlkDataB;
				tMemLatchWdB	<= 0;
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
	
//	if(!dcInHold)
	if(!tHoldB)
	begin
		tMmioDone		<= 0;
		tMmioDoneHld	<= 0;
	end

//	if((tReqIsMmio && !tMemLatchA && !tMemLatchB) || tMmioLatch)
	if(((tReqIsMmio && !tMemLatchA && !tMemLatchB) || tMmioLatch) && !reset)
	begin
		tMemOpm			<= tInOpm;
		tMemAddr		<= tInAddr;
		tMemDataOut		<= { UV64_00, tDataIn };

		if(tMmioDone)
		begin
//			$display("MMIO Done Hold");
			tMemOpm	<= UMEM_OPM_READY;
			if(memOK==UMEM_OK_READY)
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

		if(!tMmioDone)
		begin
//			if(tMmioDoneHld)
//				$display("MMIO Clear Done Hold");
			tMmioDoneHld	<= 0;
		end
	end

`ifndef jx2_do_ld1cyc
	if(!dcInHold)
	begin
		tRegOutVal2		<= tRegOutVal;	//output value A
		tRegOutValB2	<= tRegOutValB;	//output value B
	end
	tRegOutOK2		<= tRegOutOK;	//OK status
`endif

end

endmodule

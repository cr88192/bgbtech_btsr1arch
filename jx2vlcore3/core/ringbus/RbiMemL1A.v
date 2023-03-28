/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Contain L1 Instruction and Data Caches.

These caches provide a pipelined interface, recieving an address on one cycle, and producing output the next.

These will produce a Hold signal if the request can't be handled immediately, and will not accept new input until the condition is resolved.

FAULT Signal:
Will interpret the low 16 bits of the Data as an exception code.
If Bit(15) is Set, this raises an interrupt.
If Bit(15) is Clear, this indicates a non-critical failure.

In the case of Full-Duplex / Swap operations, a FAULT with a zero exception will indicate that the Swap operation could not be performed with this combination of locations. In this case, the L1 cache should fall back to Half-Duplex Load/Store operations.
*/

`include "ringbus/RbiDefs.v"

/* verilator lint_off DEFPARAM */

`include "ringbus/RbiMemDcA.v"

`ifdef jx2_enable_wex
`include "ringbus/RbiMemIcWxA.v"
`else
`include "ringbus/RbiMemIcA.v"
`endif

`ifdef jx2_enable_mmu
`include "ringbus/RbiMmuTlb.v"
`endif

`include "ringbus/RbiMemL1Bridge.v"

module RbiMemL1A(
	/* verilator lint_off UNUSED */
	clock,			reset,

	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	icOutPcSxo,		icOutPcLow,

	dcInAddr,		dcInOpm,
	dcInAddrB,		dcInOpmB,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,		dcInHold,
	dcOutHold,		dcBusWait,
	dcInLdOp,

	regInDlr,		regInDhr,
	regInMmcr,		regInKrr,
	regInSr,
	
	regOutExc,		regTraPc,
	dcInTraPc,		deadlockLatch,
	regInCurExc,

	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,

	unitNodeId,		timers);

input			clock;
input			reset;

input[11:0]		timers;

`input_vaddr	icInPcAddr;		//input PC address
// output[63: 0]	icOutPcVal;		//output PC value
output[95: 0]	icOutPcVal;		//output PC value
output[ 1: 0]	icOutPcOK;		//set if we have a valid value.
output[ 3: 0]	icOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	icOutPcSxo;		//Secure Execute
input			icInPcHold;
// input[1:0]		icInPcWxe;
input[5:0]		icInPcWxe;
output[11:0]	icOutPcLow;

`input_vaddr	dcInAddr;		//input address A
input [ 4: 0]	dcInOpm;		//input command A
output[63: 0]	dcOutVal;		//output data value
input [63: 0]	dcInVal;		//input data value
output[ 1: 0]	dcOutOK;		//set if we have a valid value.
input			dcInHold;
output			dcOutHold;		//we need to stall the pipeline
output			dcBusWait;		//we are waiting on the bus
input [ 4: 0]	dcInLdOp;		//Load/Store Operation

input[47:0]		dcInAddrB;		//input address B
input [ 4: 0]	dcInOpmB;		//input command B

output[63: 0]	dcOutValB;		//output data value (alternate)
input [63: 0]	dcInValB;		//input data value (alternate)

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

input[127:0]	regInCurExc;

output[127:0]	regOutExc;
output[63:0]	regTraPc;
input [63:0]	dcInTraPc;		//input PC
input			deadlockLatch;	//CPU Is Deadlocked

input [ 15:0]	l2mSeqIn;		//operation sequence
output[ 15:0]	l2mSeqOut;		//operation sequence
input [ 15:0]	l2mOpmIn;		//memory operation mode
output[ 15:0]	l2mOpmOut;		//memory operation mode
`input_l2addr	l2mAddrIn;		//memory input address
`output_l2addr	l2mAddrOut;		//memory output address
`input_tile		l2mDataIn;		//memory input data
`output_tile	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

parameter		disableTlb = 0;
defparam		tlb.disableTlb		= disableTlb;
defparam		memDc.noLdOp		= disableTlb;
defparam		memDc.disableTlb	= disableTlb;

reg			tResetL;

reg[15:0]	tRngA;
reg[15:0]	tRngB;
reg[15:0]	tNxtRngA;
reg[15:0]	tNxtRngB;
reg			tRngBitA;
reg			tRngBitB;
reg			tRngBitAL;
reg			tRngBitBL;
reg			tRngNoiseA;
reg			tRngNoiseA1;
reg			tRngNoiseA2;
reg			tRngNoiseA3;
reg			tRngNoiseA4;

reg[15:0]	tNxtRngN;
reg[15:0]	tRngN;
reg[15:0]	tRngN2;

// `wire_tile		tBridgeDataI;
// wire[47:0]		tBridgeAddrI;
// wire[15:0]		tBridgeOpmI;
// wire[15:0]		tBridgeSeqI;

`reg_tile		tBridgeDataI;
`reg_l1addr		tBridgeAddrI;
reg[15:0]		tBridgeOpmI;
reg[15:0]		tBridgeSeqI;

`wire_tile		tBridgeDataO;
`wire_l1addr	tBridgeAddrO;
wire[15:0]		tBridgeOpmO;
wire[15:0]		tBridgeSeqO;
wire[ 7:0]		tBridgeNodeId;

RbiMemL1Bridge	l1bridge(
	clock,			tResetL,
	regInMmcr,		regInKrr,		regInSr,

	tBridgeAddrI,	tBridgeAddrO,
	tBridgeDataI,	tBridgeDataO,
	tBridgeOpmI,	tBridgeOpmO,
	tBridgeSeqI,	tBridgeSeqO,

	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,

	tBridgeNodeId
	);


`reg_l1addr	tMemAddr;		//Memory address
`reg_l1addr	tMemAddrB;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation

`reg_tile		tMemDataOut;	//Memory Data Out

reg[127:0]		tRegOutExc;
reg[127:0]		tRegOutExc2;
reg[127:0]		tRegOutExc3;

reg[63:0]		tRegTraPc;
reg[63:0]		tRegTraPc2;

wire[5:0]		tMemAccNoRwx;


wire[127:0]		tTlbExc;
wire[15:0]		tIcExecAcl;


// assign	regOutExc	= tRegOutExc2;
assign	regOutExc	= tRegOutExc3;
assign	regTraPc	= tRegTraPc2;

reg[1:0]		tDcOutOK;
reg[1:0]		tDcOutOK2;
reg				tDcOutHold;
reg				tDcBusWait;
assign	dcOutOK		= tDcOutOK2;
assign	dcOutHold	= tDcOutHold;
assign	dcBusWait	= tDcBusWait;


wire[  7:0]		tlbMemNodeId;

reg[127:0]		tTlbInLdtlbL;
wire[127:0]		tTlbInLdtlb;
assign		tTlbInLdtlb = { regInDhr, regInDlr };

`ifdef jx2_enable_mmu

// `wire_tile		tTlbDataI;
// wire[47:0]		tTlbAddrI;
// wire[15:0]		tTlbOpmI;
// wire[15:0]		tTlbSeqI;

`reg_tile		tTlbDataI;
`reg_l1addr		tTlbAddrI;
reg[15:0]		tTlbOpmI;
reg[15:0]		tTlbSeqI;

`wire_tile		tTlbDataO;
`wire_l1addr	tTlbAddrO;
wire[15:0]		tTlbOpmO;
wire[15:0]		tTlbSeqO;

RbiMmuTlb	tlb(
	clock,			tResetL,

	tTlbAddrI,		tTlbAddrO,
	tTlbDataI,		tTlbDataO,
	tTlbOpmI,		tTlbOpmO,
	tTlbSeqI,		tTlbSeqO,
	tlbMemNodeId,	tTlbInLdtlbL,

//	tTlbExc,		dcInHold,
	tTlbExc,		1'b0,
	regInMmcr,		regInKrr,
	regInSr,		tIcExecAcl);

`endif


reg [7:0]		regKrrRngDs;
reg [7:0]		regNxtKrrRngDs;

reg [7:0]		regKrrRngIs;
reg [7:0]		regNxtKrrRngIs;

wire[7:0]		regKrrHash;
reg [7:0]		regKrrHashL;
assign		regKrrHash = regInKrr[7:0]^regInKrr[15:8];
// assign		regKrrHash = (regInKrr[7:0]^regInKrr[15:8])+regKrrRng;

reg [7:0]		regKrrHashIsL;
reg [7:0]		regKrrHashDsL;


wire[5:0]		dfInOpm;
wire[5:0]		dfInOpmB;
assign		dfInOpm		= { dcInOpm [4:3], 1'b0, dcInOpm [2:0] };
assign		dfInOpmB	= { dcInOpmB[4:3], 1'b0, dcInOpmB[2:0] };

wire[7:0]		dfInLdOp;
assign		dfInLdOp		= { 3'b000, dcInLdOp [4:0] };

wire			ifMemWait;
wire[127:0]		ifOutExc;

reg			ifMemWaitL;

// `wire_tile		ifMemDataI;
// wire[ 47:0]		ifMemAddrI;
// wire[ 15:0]		ifMemOpmI;
// wire[ 15:0]		ifMemSeqI;

`reg_tile		ifMemDataI;
`reg_l1addr		ifMemAddrI;
reg[ 15:0]		ifMemOpmI;
reg[ 15:0]		ifMemSeqI;

`wire_tile		ifMemDataO;
`wire_l1addr	ifMemAddrO;
wire[ 15:0]		ifMemOpmO;
wire[ 15:0]		ifMemSeqO;
wire[  7:0]		ifMemNodeId;

RbiMemIcWxA		memIc(
	clock,			tResetL,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icOutPcLow,

	icInPcHold,		icInPcWxe,
	dfInOpm,		regInSr,
	ifMemWait,		ifOutExc,
	icOutPcSxo,		regKrrHashIsL,
	tTlbExc,		tIcExecAcl,
	regInMmcr,

	ifMemAddrI,		ifMemAddrO,
	ifMemDataI,		ifMemDataO,
	ifMemOpmI,		ifMemOpmO,
	ifMemSeqI,		ifMemSeqO,

	ifMemNodeId
	);

wire			dfOutHold;
wire			dfOutWait;
wire[127:0]		dfOutExc;

// `wire_tile		dfMemDataI;
// wire[ 47:0]		dfMemAddrI;
// wire[ 15:0]		dfMemOpmI;
// wire[ 15:0]		dfMemSeqI;

`reg_tile		dfMemDataI;
`reg_l1addr		dfMemAddrI;
reg[ 15:0]		dfMemOpmI;
reg[ 15:0]		dfMemSeqI;

`wire_tile		dfMemDataO;
`wire_l1addr	dfMemAddrO;
wire[ 15:0]		dfMemOpmO;
wire[ 15:0]		dfMemSeqO;
wire[  7:0]		dfMemNodeId;

RbiMemDcA		memDc(
	clock,			tResetL,
	dcInAddr,		dfInOpm,
	dcInAddrB,		dfInOpmB,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcInHold,		dfOutHold,
	regInSr,		dfOutWait,
	dfOutExc,		regInMmcr,
	regKrrHashDsL,	tRngN2,
	dfInLdOp,

	dfMemAddrI,		dfMemAddrO,
	dfMemDataI,		dfMemDataO,
	dfMemOpmI,		dfMemOpmO,
	dfMemSeqI,		dfMemSeqO,

	dfMemNodeId
	);

`ifndef def_true

assign		ifMemDataI		= tBridgeDataO;
assign		ifMemAddrI		= tBridgeAddrO;
assign		ifMemOpmI		= tBridgeOpmO;
assign		ifMemSeqI		= tBridgeSeqO;

assign		dfMemDataI		= ifMemDataO;
assign		dfMemAddrI		= ifMemAddrO;
assign		dfMemOpmI		= ifMemOpmO;
assign		dfMemSeqI		= ifMemSeqO;

`ifdef jx2_enable_mmu
// `ifndef def_true
assign		tTlbDataI		= dfMemDataO;
assign		tTlbAddrI		= dfMemAddrO;
assign		tTlbOpmI		= dfMemOpmO;
assign		tTlbSeqI		= dfMemSeqO;

assign		tBridgeDataI	= tTlbDataO;
assign		tBridgeAddrI	= tTlbAddrO;
assign		tBridgeOpmI		= tTlbOpmO;
assign		tBridgeSeqI		= tTlbSeqO;
`else
assign		tBridgeDataI	= dfMemDataO;
assign		tBridgeAddrI	= dfMemAddrO;
assign		tBridgeOpmI		= dfMemOpmO;
assign		tBridgeSeqI		= dfMemSeqO;
`endif

`endif


assign		tBridgeNodeId	= { unitNodeId[7:2], 2'b00 };
assign		ifMemNodeId		= { unitNodeId[7:2], 2'b01 };
assign		dfMemNodeId		= { unitNodeId[7:2], 2'b10 };
assign		tlbMemNodeId	= { unitNodeId[7:2], 2'b11 };

reg		tSkipTlb;

reg		tMsgLatch;
reg		tNxtMsgLatch;

reg		tExcLatch;
reg		tNxtExcLatch;

reg[9:0]	tExcLatchInhCnt;
reg[9:0]	tNxtExcLatchInhCnt;

always @*
begin
	dfMemDataI		= tBridgeDataO;
	dfMemAddrI		= tBridgeAddrO;
	dfMemOpmI		= tBridgeOpmO;
	dfMemSeqI		= tBridgeSeqO;

	ifMemDataI		= dfMemDataO;
	ifMemAddrI		= dfMemAddrO;
	ifMemOpmI		= dfMemOpmO;
	ifMemSeqI		= dfMemSeqO;

`ifdef jx2_enable_mmu
// `ifndef def_true
	tTlbDataI		= ifMemDataO;
	tTlbAddrI		= ifMemAddrO;
	tTlbOpmI		= ifMemOpmO;
	tTlbSeqI		= ifMemSeqO;

	tBridgeDataI	= tTlbDataO;
	tBridgeAddrI	= tTlbAddrO;
	tBridgeOpmI		= tTlbOpmO;
	tBridgeSeqI		= tTlbSeqO;

`ifdef def_true
// `ifndef def_true
	/* Check if L1 D$ request should skip over L1 I$ and TLB. */
	tSkipTlb		= 0;
	if(tTlbOpmO[7:0] == JX2_RBI_OPM_IDLE)
	begin
		if(	(dfMemOpmO[7:0]==JX2_RBI_OPM_LDSQ) ||
			(dfMemOpmO[7:0]==JX2_RBI_OPM_STSQ) ||
			(dfMemOpmO[7:0]==JX2_RBI_OPM_LDSL) ||
			(dfMemOpmO[7:0]==JX2_RBI_OPM_STSL) )
		begin
			tSkipTlb	= 1;
		end
		else if(	(dfMemOpmO[7:0]==JX2_RBI_OPM_LDX) ||
					(dfMemOpmO[7:0]==JX2_RBI_OPM_STX) ||
					(dfMemOpmO[7:0]==JX2_RBI_OPM_PFX) ||
					(dfMemOpmO[7:0]==JX2_RBI_OPM_SPX) )
		begin
			tSkipTlb	=
				(dfMemAddrO[47:44] == 4'hC) ||
				((dfMemAddrO[47:44] == 4'h0) && !regInMmcr[0]);
		end
	end

	if(disableTlb)
		tSkipTlb	= 1;

	if(tSkipTlb)
	begin
		/* Do Skip. */
		ifMemDataI		= tTlbDataO;
		ifMemAddrI		= tTlbAddrO;
		ifMemOpmI		= tTlbOpmO;
		ifMemSeqI		= tTlbSeqO;

		tBridgeDataI	= dfMemDataO;
		tBridgeAddrI	= dfMemAddrO;
		tBridgeOpmI		= dfMemOpmO;
		tBridgeSeqI		= dfMemSeqO;
	end

	if(disableTlb)
	begin
		dfMemDataI		= tBridgeDataO;
		dfMemAddrI		= tBridgeAddrO;
		dfMemOpmI		= tBridgeOpmO;
		dfMemSeqI		= tBridgeSeqO;

		ifMemDataI		= dfMemDataO;
		ifMemAddrI		= dfMemAddrO;
		ifMemOpmI		= dfMemOpmO;
		ifMemSeqI		= dfMemSeqO;

		tTlbDataI		= 0;
		tTlbAddrI		= 0;
		tTlbOpmI		= 0;
		tTlbSeqI		= 0;

		tBridgeDataI	= ifMemDataO;
		tBridgeAddrI	= ifMemAddrO;
		tBridgeOpmI		= ifMemOpmO;
		tBridgeSeqI		= ifMemSeqO;
	end
`endif

`else
	tBridgeDataI	= ifMemDataO;
	tBridgeAddrI	= ifMemAddrO;
	tBridgeOpmI		= ifMemOpmO;
	tBridgeSeqI		= ifMemSeqO;
`endif
end

always @*
begin
	tNxtMsgLatch	= 0;
	tRegOutExc		= UV128_00;
	tRegTraPc		= UV64_00;

	tNxtExcLatch		= tExcLatch;
	tNxtExcLatchInhCnt	= (tExcLatchInhCnt!=0) ? (tExcLatchInhCnt-1) : 0;

	regNxtKrrRngDs	= regKrrRngDs;
	regNxtKrrRngIs	= regKrrRngIs;

//	tDcOutOK		= dfOutOK[1:0];
	
//	tDcOutHold		= 0;
// `ifdef	jx2_mem_l1dstall
//	tDcOutHold		= dfOutOKB[1];
// `endif

	tDcOutHold	= dfOutHold;

	tDcBusWait	= dfOutWait || ifMemWait;
//	tDcBusWait	= dfOutWait || ifMemWait || ifMemWaitL;

	tRngBitA	= 0;
	tRngBitB	= 0;
	tNxtRngA	= 0;
	tNxtRngB	= 0;

// `ifndef def_true
`ifdef def_true
	tRngBitA	=
		tRngA[1] ^ tRngA[3] ^
		tRngA[5] ^ tRngA[7] ^
		tRngNoiseA ^ tRngBitBL ^ 1;
	tRngBitB	=
		tRngB[1] ^ tRngB[3] ^
		tRngB[5] ^ tRngB[7] ^
		tRngNoiseA ^ tRngBitAL ^ 1;

//	tNxtRngA	= { tRngBitA, tRngA[15:1] };
	tNxtRngA	= { tRngA[14:0], tRngBitA };
	tNxtRngB	= { tRngBitB, tRngB[15:1] };
	
	tNxtRngN	= tRngA + tRngB;
`endif

//	tNxtRngN	= 0;

	if(tResetL)
	begin
		tDcOutHold	= 0;
		tDcBusWait	= 0;
	end

	tDcOutOK	= UMEM_OK_OK;
	if(dfOutHold)
		tDcOutOK	= UMEM_OK_HOLD;

	if(!tResetL)
	begin
		if(tTlbExc[15])
			tRegOutExc = tTlbExc;
		else if(dfOutExc[15])
			tRegOutExc = dfOutExc;
		else if(ifOutExc[15])
			tRegOutExc = ifOutExc;
		
//		if((regInSr[29] && regInSr[28]) && (tRegOutExc[15:12]==4'hC))
		if((regInSr[29] && regInSr[28]) && (tRegOutExc[15:12]!=4'h8))
//		if(regInSr[29] && regInSr[28])
			tRegOutExc = UV128_00;
	end

	if(tRegOutExc[15])
	begin
		tDcOutHold		= 1;
		tNxtExcLatch	= 1;
		tNxtExcLatchInhCnt	= 1023;
//		if(	(tRegOutExc[15:12]==4'h8) ||
//			(tRegOutExc[15:12]==4'hA) )
//				regNxtKrrRng	= tNxtRngN[11:4];
	end
	
//	if(	(dcInOpm==UMEM_OPM_FLUSHDS) ||
//		(dcInOpm==UMEM_OPM_FLUSHIS))
//			regNxtKrrRng	= tNxtRngN[11:4];


	if(dcInOpm==UMEM_OPM_FLUSHDS)
	begin
//		regNxtKrrRngDs	= tNxtRngN[11:4];
//		regNxtKrrRngDs	= tNxtRngN[7:0];
		regNxtKrrRngDs	= tNxtRngN[7:0] + tNxtRngN[15:8];
		$display("KrrRngDs %X %X", regKrrRngDs, regNxtKrrRngDs);
	end
	if(dcInOpm==UMEM_OPM_FLUSHIS)
	begin
//		regNxtKrrRngIs	= tNxtRngN[11:4];
//		regNxtKrrRngIs	= tNxtRngN[7:0];
		regNxtKrrRngIs	= tNxtRngN[7:0] + tNxtRngN[15:8];
		$display("KrrRngIs %X %X", regKrrRngIs, regNxtKrrRngIs);
	end


//		(dcInOpm==UMEM_OPM_FLUSHIS))
//			regNxtKrrRng	= tNxtRngN[11:4];
	
//	if(tExcLatch)
	if(tExcLatch && !(regInSr[29] && regInSr[28]))
		tDcOutHold		= 1;

	if(tResetL)
	begin
		tDcOutHold		= 0;
		tNxtExcLatch	= 0;
	end

	if(regInCurExc[15])
	begin
		tNxtExcLatch	= 0;
	end

	if((tExcLatchInhCnt == 0) && tExcLatch)
	begin
		$display("RbiMemL1A: Clear stuck EXC Latch");
		tNxtExcLatch	= 0;
	end

	if(regInSr[29] && regInSr[28])
	begin
		tNxtExcLatch	= 0;
	end

end

always @(posedge clock)
begin

	tRegOutExc2		<= tRegOutExc;
	tRegOutExc3		<= tRegOutExc2;
	tRegTraPc2		<= tRegTraPc;
	tDcOutOK2		<= tDcOutOK;
	
	tTlbInLdtlbL	<= tTlbInLdtlb;

	ifMemWaitL		<= ifMemWait;

	regKrrHashL		<= regKrrHash;
//	regKrrHashDsL	<= regKrrHashL + regKrrRngDs;
//	regKrrHashIsL	<= regKrrHashL + regKrrRngIs;
	regKrrHashDsL	<= regKrrHashL ^ regKrrRngDs;
	regKrrHashIsL	<= regKrrHashL ^ regKrrRngIs;
	regKrrRngDs		<= regNxtKrrRngDs;
	regKrrRngIs		<= regNxtKrrRngIs;

	tExcLatch		<= tNxtExcLatch;

	tRngA			<= tNxtRngA;
	tRngB			<= tNxtRngB;
	tRngBitAL		<= tRngBitA;
	tRngBitBL		<= tRngBitB;
//	tRngNoiseA4		<= timers[0];

	tRngNoiseA4		<=
		l2mOpmIn[15] ^
		l2mAddrIn[4] ^ l2mAddrIn[7] ^
		l2mDataIn[0] ^ l2mDataIn[8];

	tRngNoiseA3		<= tRngNoiseA4;
	tRngNoiseA2		<= tRngNoiseA3;
	tRngNoiseA1		<= tRngNoiseA2;
	tRngNoiseA		<= tRngNoiseA1;

	tRngN			<= tNxtRngN;
	tRngN2			<= tRngN;
	tResetL			<= reset;


	tExcLatchInhCnt	<= tNxtExcLatchInhCnt;

`ifndef def_true
// `ifdef def_true
//	if(ifMemOpmI!=0)
	if(1'b0)
	begin
		$display("I$-In O=%X S=%X A=%X D=%X",
			ifMemOpmI,
			ifMemSeqI,
			ifMemAddrI,
			ifMemDataI);
	end

//	if(ifMemOpmO!=0)
	if(	(ifMemOpmO[7:0]==JX2_RBI_OPM_LDX) ||
		(ifMemOpmO[7:0]==JX2_RBI_OPM_LDSQ) ||
		(ifMemOpmO[7:0]==JX2_RBI_OPM_STX) ||
		(ifMemOpmO[7:0]==JX2_RBI_OPM_STSQ))
	begin
		$display("I$-Out O=%X S=%X A=%X D=%X",
			ifMemOpmO,
			ifMemSeqO,
			ifMemAddrO,
			ifMemDataO);
	end

//	if(ifMemOpmO!=0)
	if(	(dfMemOpmO[7:0]==JX2_RBI_OPM_LDX) ||
		(dfMemOpmO[7:0]==JX2_RBI_OPM_LDSQ) ||
		(dfMemOpmO[7:0]==JX2_RBI_OPM_STX) ||
		(dfMemOpmO[7:0]==JX2_RBI_OPM_STSQ))
	begin
		$display("D$-Out O=%X S=%X A=%X D=%X",
			dfMemOpmO,
			dfMemSeqO,
			dfMemAddrO,
			dfMemDataO);
	end
`endif


`ifndef def_true
// `ifdef def_true
	if(tBridgeOpmI!=0)
	begin
		$display("BridgeIn O=%X S=%X A=%X D=%X",
			tBridgeOpmI,
			tBridgeSeqI,
			tBridgeAddrI,
			tBridgeDataI);
	end

	if(tBridgeOpmO!=0)
	begin
		$display("BridgeOut O=%X S=%X A=%X D=%X",
			tBridgeOpmO,
			tBridgeSeqO,
			tBridgeAddrO,
			tBridgeDataO);
	end
`endif

end

endmodule

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

2W: Assumes a Dual-Pipeline / SMT Core
Both threads are assumed to be within a single address space.

These caches provide a pipelined interface, recieving an address on one cycle, and producing output the next.

These will produce a Hold signal if the request can't be handled immediately, and will not accept new input until the condition is resolved.

FAULT Signal:
Will interpret the low 16 bits of the Data as an exception code.
If Bit(15) is Set, this raises an interrupt.
If Bit(15) is Clear, this indicates a non-critical failure.

In the case of Full-Duplex / Swap operations, a FAULT with a zero exception will indicate that the Swap operation could not be performed with this combination of locations. In this case, the L1 cache should fall back to Half-Duplex Load/Store operations.
*/

`include "ringbus/RbiDefs.v"

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

module RbiMemL1A2W(
	/* verilator lint_off UNUSED */
	clock,			reset,

	icaInPcAddr,	icaOutPcVal,
	icaOutPcOK,		icaOutPcStep,
	icaInPcHold,	icaInPcWxe,
	icaOutPcSxo,

	icbInPcAddr,	icbOutPcVal,
	icbOutPcOK,		icbOutPcStep,
	icbInPcHold,	icbInPcWxe,
	icbOutPcSxo,

	dcOutOK,		dcInHold,
	dcOutHold,		dcBusWait,

	dcaInAddr,		dcaInOpm,
	dcaOutVal,		dcaInVal,
	dcaOutValB,		dcaInValB,

	dcbInAddr,		dcbInOpm,
	dcbOutVal,		dcbInVal,
	dcbOutValB,		dcbInValB,

	regInMmcr,
	
	raInDlr,		raInDhr,
	raInKrr,		raInSr,

	rbInDlr,		rbInDhr,
	rbInKrr,		rbInSr,

	regOutExc,		regTraPc,
	dcInTraPc,		deadlockLatch,

	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,

	unitNodeId	);

input			clock;
input			reset;

input [47: 0]	icaInPcAddr;	//input PC address
output[95: 0]	icaOutPcVal;	//output PC value
output[ 1: 0]	icaOutPcOK;		//set if we have a valid value.
output[ 3: 0]	icaOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	icaOutPcSxo;	//Secure Execute
input			icaInPcHold;
input			icaInPcWxe;

input [47: 0]	icbInPcAddr;	//input PC address
output[95: 0]	icbOutPcVal;	//output PC value
output[ 1: 0]	icbOutPcOK;		//set if we have a valid value.
output[ 3: 0]	icbOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	icbOutPcSxo;	//Secure Execute
input			icbInPcHold;
input			icbInPcWxe;

input [47: 0]	dcaInAddr;		//input PC address
input [ 4: 0]	dcaInOpm;		//input PC address
output[63: 0]	dcaOutVal;		//output data value
output[63: 0]	dcaOutValB;		//output data value (alternate)
input [63: 0]	dcaInVal;		//input data value
input [63: 0]	dcaInValB;		//input data value (alternate)

output[ 1: 0]	dcOutOK;		//set if we have a valid value.
input			dcInHold;
output			dcOutHold;		//we need to stall the pipeline
output			dcBusWait;		//we are waiting on the bus

input [47: 0]	dcbInAddr;		//input PC address
input [ 4: 0]	dcbInOpm;		//input PC address
output[63: 0]	dcbOutVal;		//output data value
output[63: 0]	dcbOutValB;		//output data value (alternate)
input [63: 0]	dcbInVal;		//input data value
input [63: 0]	dcbInValB;		//input data value (alternate)

input[63:0]		regInMmcr;

input[63:0]		raInDlr;
input[63:0]		raInDhr;
input[63:0]		raInKrr;
input[63:0]		raInSr;

input[63:0]		rbInDlr;
input[63:0]		rbInDhr;
input[63:0]		rbInKrr;
input[63:0]		rbInSr;

output[63:0]	regOutExc;
output[63:0]	regTraPc;
input [63:0]	dcInTraPc;		//input PC
input			deadlockLatch;	//CPU Is Deadlocked

input [ 15:0]	l2mSeqIn;		//operation sequence
output[ 15:0]	l2mSeqOut;		//operation sequence
input [ 15:0]	l2mOpmIn;		//memory operation mode
output[ 15:0]	l2mOpmOut;		//memory operation mode
input [ 47:0]	l2mAddrIn;		//memory input address
output[ 47:0]	l2mAddrOut;		//memory output address
`input_tile		l2mDataIn;		//memory input data
`output_tile	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


`reg_tile		tBridgeDataI;
reg[47:0]		tBridgeAddrI;
reg[15:0]		tBridgeOpmI;
reg[15:0]		tBridgeSeqI;

`wire_tile		tBridgeDataO;
wire[47:0]		tBridgeAddrO;
wire[15:0]		tBridgeOpmO;
wire[15:0]		tBridgeSeqO;
wire[ 7:0]		tBridgeNodeId;

RbiMemL1Bridge	l1bridge(
	clock,			reset,
	regInMmcr,		raInKrr,		raInSr,

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


reg[63:0]		tRegOutExc;
reg[63:0]		tRegOutExc2;

reg[63:0]		tRegTraPc;
reg[63:0]		tRegTraPc2;

wire[5:0]		tMemAccNoRwx;


wire[63:0]		tTlbExc;


assign	regOutExc	= tRegOutExc2;
assign	regTraPc	= tRegTraPc2;

reg[1:0]		tDcOutOK;
reg[1:0]		tDcOutOK2;
reg				tDcOutHold;
reg				tDcBusWait;
assign	dcOutOK		= tDcOutOK2;
assign	dcOutHold	= tDcOutHold;
assign	dcBusWait	= tDcBusWait;


wire[  7:0]		tlbMemNodeId;

wire[127:0]		tTlbInLdtlb;
assign		tTlbInLdtlb = { raInDhr, raInDlr };

`ifdef jx2_enable_mmu

`reg_tile		tTlbDataI;
reg[47:0]		tTlbAddrI;
reg[15:0]		tTlbOpmI;
reg[15:0]		tTlbSeqI;

`wire_tile		tTlbDataO;
wire[47:0]		tTlbAddrO;
wire[15:0]		tTlbOpmO;
wire[15:0]		tTlbSeqO;

RbiMmuTlb	tlb(
	clock,			reset,

	tTlbAddrI,		tTlbAddrO,
	tTlbDataI,		tTlbDataO,
	tTlbOpmI,		tTlbOpmO,
	tTlbSeqI,		tTlbSeqO,
	tlbMemNodeId,	tTlbInLdtlb,

	tTlbExc,		1'b0,
	regInMmcr,		raInKrr,
	raInSr);

`endif

wire[5:0]		dfaInOpm;
wire[5:0]		dfbInOpm;
assign		dfaInOpm	= { dcaInOpm[4:3], 1'b0, dcaInOpm[2:0] };
assign		dfbInOpm	= { dcbInOpm[4:3], 1'b0, dcbInOpm[2:0] };

wire			ifMemWait;
wire			ifaMemWait;
wire			ifbMemWait;

wire[63:0]		ifOutExc;
wire[63:0]		ifaOutExc;
wire[63:0]		ifbOutExc;

assign		ifMemWait = ifaMemWait || ifbMemWait;
assign		ifOutExc = ifaOutExc[15] ? ifaOutExc : ifbOutExc;

`reg_tile		ifMemDataI;
reg[ 47:0]		ifMemAddrI;
reg[ 15:0]		ifMemOpmI;
reg[ 15:0]		ifMemSeqI;

`wire_tile		ifMemDataC;
wire[ 47:0]		ifMemAddrC;
wire[ 15:0]		ifMemOpmC;
wire[ 15:0]		ifMemSeqC;

`wire_tile		ifMemDataO;
wire[ 47:0]		ifMemAddrO;
wire[ 15:0]		ifMemOpmO;
wire[ 15:0]		ifMemSeqO;

wire[  7:0]		ifMemNodeIdA;
wire[  7:0]		ifMemNodeIdB;

RbiMemIcWxA		memIcA(
	clock,			reset,
	icaInPcAddr,	icaOutPcVal,
	icaOutPcOK,		icaOutPcStep,
	icaInPcHold,	icaInPcWxe,
	dfaInOpm,		raInSr,
	ifaMemWait,		ifaOutExc,
	icaOutPcSxo,

	ifMemAddrI,		ifMemAddrC,
	ifMemDataI,		ifMemDataC,
	ifMemOpmI,		ifMemOpmC,
	ifMemSeqI,		ifMemSeqC,

	ifMemNodeIdA
	);

RbiMemIcWxA		memIcB(
	clock,			reset,
	icbInPcAddr,	icbOutPcVal,
	icbOutPcOK,		icbOutPcStep,
	icbInPcHold,	icbInPcWxe,
	dfbInOpm,		rbInSr,
	ifbMemWait,		ifbOutExc,
	icbOutPcSxo,

	ifMemAddrC,		ifMemAddrO,
	ifMemDataC,		ifMemDataO,
	ifMemOpmC,		ifMemOpmO,
	ifMemSeqC,		ifMemSeqO,

	ifMemNodeIdB
	);

wire			dfOutHold;
wire			dfOutWait;
wire[63:0]		dfOutExc;

wire			dfaOutHold;
wire			dfaOutWait;
wire[63:0]		dfaOutExc;

wire			dfbOutHold;
wire			dfbOutWait;
wire[63:0]		dfbOutExc;

assign		dfOutHold = dfaOutHold || dfbOutHold;
assign		dfOutWait = dfaOutWait || dfaOutWait;
assign		dfOutExc = dfaOutExc[15] ? dfaOutExc : dfbOutExc;

`reg_tile		dfMemDataI;
reg[ 47:0]		dfMemAddrI;
reg[ 15:0]		dfMemOpmI;
reg[ 15:0]		dfMemSeqI;

`wire_tile		dfMemDataC;
wire[ 47:0]		dfMemAddrC;
wire[ 15:0]		dfMemOpmC;
wire[ 15:0]		dfMemSeqC;

`wire_tile		dfMemDataO;
wire[ 47:0]		dfMemAddrO;
wire[ 15:0]		dfMemOpmO;
wire[ 15:0]		dfMemSeqO;

wire[  7:0]		dfMemNodeIdA;
wire[  7:0]		dfMemNodeIdB;

RbiMemDcA		memDcA(
	clock,			reset,
	dcaInAddr,		dfaInOpm,
	dcaOutVal,		dcaInVal,
	dcaOutValB,		dcaInValB,
	dcInHold,		dfaOutHold,
	raInSr,			dfaOutWait,
	dfaOutExc,		regInMmcr,

	dfMemAddrI,		dfMemAddrC,
	dfMemDataI,		dfMemDataC,
	dfMemOpmI,		dfMemOpmC,
	dfMemSeqI,		dfMemSeqC,

	dfMemNodeIdA
	);

RbiMemDcA		memDcB(
	clock,			reset,
	dcbInAddr,		dfbInOpm,
	dcbOutVal,		dcbInVal,
	dcbOutValB,		dcbInValB,
	dcInHold,		dfbOutHold,
	rbInSr,			dfbOutWait,
	dfbOutExc,		regInMmcr,

	dfMemAddrC,		dfMemAddrO,
	dfMemDataC,		dfMemDataO,
	dfMemOpmC,		dfMemOpmO,
	dfMemSeqC,		dfMemSeqO,

	dfMemNodeIdB
	);


assign		tBridgeNodeId	= { unitNodeId[7:3], 3'b000 };
assign		tlbMemNodeId	= { unitNodeId[7:3], 3'b001 };

assign		ifMemNodeIdA	= { unitNodeId[7:3], 3'b010 };
assign		dfMemNodeIdA	= { unitNodeId[7:3], 3'b011 };

assign		ifMemNodeIdB	= { unitNodeId[7:3], 3'b110 };
assign		dfMemNodeIdB	= { unitNodeId[7:3], 3'b111 };

reg		tSkipTlb;

reg		tMsgLatch;
reg		tNxtMsgLatch;

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
`endif

`else
	tBridgeDataI	= dfMemDataO;
	tBridgeAddrI	= dfMemAddrO;
	tBridgeOpmI		= dfMemOpmO;
	tBridgeSeqI		= dfMemSeqO;
`endif
end

always @*
begin
	tNxtMsgLatch	= 0;
	tRegOutExc	= UV64_00;
	tRegTraPc	= UV64_00;

	tDcOutHold	= dfOutHold;

	tDcBusWait	= dfOutWait || ifMemWait;

	if(reset)
	begin
		tDcOutHold	= 0;
		tDcBusWait	= 0;
	end

	tDcOutOK	= UMEM_OK_OK;
	if(dfOutHold)
		tDcOutOK	= UMEM_OK_HOLD;

	if(!reset)
	begin
		if(tTlbExc[15])
			tRegOutExc = tTlbExc;
		else if(dfOutExc[15])
			tRegOutExc = dfOutExc;
		else if(ifOutExc[15])
			tRegOutExc = ifOutExc;
	end

end

always @(posedge clock)
begin

	tRegOutExc2		<= tRegOutExc;
	tRegTraPc2		<= tRegTraPc;
	tDcOutOK2		<= tDcOutOK;

end

endmodule

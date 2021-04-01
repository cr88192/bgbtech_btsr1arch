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

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,
	dcInHold,
	dcOutHold,

	regInDlr,		regInDhr,
	regInMmcr,		regInKrr,
	regInSr,
	
	regOutExc,		regTraPc,
	dcInTraPc,		deadlockLatch,

	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,

	unitNodeId	);

input			clock;
input			reset;

input [47: 0]	icInPcAddr;		//input PC address
// output[63: 0]	icOutPcVal;		//output PC value
output[95: 0]	icOutPcVal;		//output PC value
output[ 1: 0]	icOutPcOK;		//set if we have a valid value.
output[ 2: 0]	icOutPcStep;	//PC step (Normal Op)
input			icInPcHold;
input			icInPcWxe;

input [47: 0]	dcInAddr;		//input PC address
input [ 4: 0]	dcInOpm;		//input PC address
output[63: 0]	dcOutVal;		//output data value
input [63: 0]	dcInVal;		//input data value
output[ 1: 0]	dcOutOK;		//set if we have a valid value.
input			dcInHold;
output			dcOutHold;		//we need to stall the pipeline

output[63: 0]	dcOutValB;		//output data value (alternate)
input [63: 0]	dcInValB;		//input data value (alternate)

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

output[63:0]	regOutExc;
output[63:0]	regTraPc;
input [63:0]	dcInTraPc;		//input PC
input			deadlockLatch;	//CPU Is Deadlocked

input [ 15:0]	l2mSeqIn;		//operation sequence
output[ 15:0]	l2mSeqOut;		//operation sequence
input [ 15:0]	l2mOpmIn;		//memory operation mode
output[ 15:0]	l2mOpmOut;		//memory operation mode
input [ 31:0]	l2mAddrIn;		//memory input address
output[ 31:0]	l2mAddrOut;		//memory output address
input [127:0]	l2mDataIn;		//memory input data
output[127:0]	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


`wire_tile		tBridgeDataI;
`wire_tile		tBridgeDataO;
wire[47:0]		tBridgeAddrI;
wire[47:0]		tBridgeAddrO;
wire[15:0]		tBridgeOpmI;
wire[15:0]		tBridgeOpmO;
wire[15:0]		tBridgeSeqI;
wire[15:0]		tBridgeSeqO;
wire[ 7:0]		tBridgeNodeId;

RbiMemL1Bridge	l1bridge(
	clock,			reset,
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


reg[ 47:0]		tMemAddr;		//Memory address
reg[ 47:0]		tMemAddrB;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation

`reg_tile		tMemDataOut;	//Memory Data Out

reg[63:0]		tRegOutExc;
reg[63:0]		tRegOutExc2;

reg[63:0]		tRegTraPc;
reg[63:0]		tRegTraPc2;

wire[5:0]		tMemAccNoRwx;


wire[63:0]		tTlbExc;


assign	regOutExc	= tRegOutExc2;
assign	regTraPc	= tRegTraPc2;

reg[1:0]		tDcOutOK;
reg				tDcOutHold;
assign	dcOutOK		= tDcOutOK;
assign	dcOutHold	= tDcOutHold;


`ifdef jx2_enable_mmu

`wire_tile		tTlbDataI;
`wire_tile		tTlbDataO;
wire[47:0]		tTlbAddrI;
wire[47:0]		tTlbAddrO;
wire[15:0]		tTlbOpmI;
wire[15:0]		tTlbOpmO;
wire[15:0]		tTlbSeqI;
wire[15:0]		tTlbSeqO;

RbiMmuTlb	tlb(
	clock,			reset,

	tTlbAddrI,		tTlbAddrO,
	tTlbDataI,		tTlbDataO,
	tTlbOpmI,		tTlbOpmO,
	tTlbSeqI,		tTlbSeqI,

	tTlbExc,		dcInHold,
	regInMmcr,		regInKrr,
	regInSr);

`endif


`wire_tile		ifMemDataI;
`wire_tile		ifMemDataO;
wire[ 47:0]		ifMemAddrI;
wire[ 47:0]		ifMemAddrO;
wire[ 15:0]		ifMemOpmI;
wire[ 15:0]		ifMemOpmO;
wire[ 15:0]		ifMemSeqI;
wire[ 15:0]		ifMemSeqO;
wire[  7:0]		ifMemNodeId;

RbiMemIcWxA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	dcInOpm,		regInSr,

	ifMemAddrI,		ifMemAddrO,
	ifMemDataI,		ifMemDataO,
	ifMemOpmI,		ifMemOpmO,
	ifMemSeqI,		ifMemSeqO,

	ifMemNodeId
	);

wire			dfOutHold;
wire[5:0]		dfInOpm;
assign		dfInOpm		= { dcInOpm[4], 1'b0, dcInOpm[3:0] };

`wire_tile		dfMemDataI;
`wire_tile		dfMemDataO;
wire[ 47:0]		dfMemAddrI;
wire[ 47:0]		dfMemAddrO;
wire[ 15:0]		dfMemOpmI;
wire[ 15:0]		dfMemOpmO;
wire[ 15:0]		dfMemSeqI;
wire[ 15:0]		dfMemSeqO;
wire[  7:0]		dfMemNodeId;

RbiMemDcA		memDc(
	clock,			reset,
	dcInAddr,		dfInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcInHold,		dcOutHold,
	regInSr,

	dfMemAddrI,		dfMemAddrO,
	dfMemDataI,		dfMemDataO,
	dfMemOpmI,		dfMemOpmO,
	dfMemSeqI,		dfMemSeqO,

	dfMemNodeId
	);

assign		ifMemDataI		= tBridgeDataO;
assign		ifMemAddrI		= tBridgeAddrO;
assign		ifMemOpmI		= tBridgeOpmO;
assign		ifMemSeqI		= tBridgeSeqO;

assign		dfMemDataI		= ifMemDataO;
assign		dfMemAddrI		= ifMemAddrO;
assign		dfMemOpmI		= ifMemOpmO;
assign		dfMemSeqI		= ifMemSeqO;

`ifdef jx2_enable_mmu
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

assign		tBridgeNodeId	= { unitNodeId[7:2], 2'b00 };
assign		ifMemNodeId		= { unitNodeId[7:2], 2'b01 };
assign		dfMemNodeId		= { unitNodeId[7:2], 2'b10 };


reg		tMsgLatch;
reg		tNxtMsgLatch;

always @*
begin
	tNxtMsgLatch	= 0;
	tRegOutExc	= UV64_00;
	tRegTraPc	= UV64_00;
	
//	tDcOutOK	= dfOutOK[1:0];
	
//	tDcOutHold	= 0;
// `ifdef	jx2_mem_l1dstall
//	tDcOutHold	= dfOutOKB[1];
// `endif

	tDcOutHold	= dfOutHold;


end

always @(posedge clock)
begin	

end

endmodule

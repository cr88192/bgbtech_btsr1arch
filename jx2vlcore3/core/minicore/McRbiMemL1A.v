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

`include "minicore/McRbiMemDcA.v"
`include "minicore/McRbiMemIcFxA.v"
`include "minicore/McRbiMmuTlb.v"
`include "minicore/McRbiMemL1Bridge.v"

module McRbiMemL1A(
	/* verilator lint_off UNUSED */
	clock,			reset,

	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	icOutPcSxo,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutOK,		dcInHold,
	dcOutHold,		dcBusWait,

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

`input_mvaddr	icInPcAddr;		//input PC address
output[95: 0]	icOutPcVal;		//output PC value
output[ 1: 0]	icOutPcOK;		//set if we have a valid value.
output[ 3: 0]	icOutPcStep;	//PC step (Normal Op)
output[ 3: 0]	icOutPcSxo;		//Secure Execute
input			icInPcHold;
input[1:0]		icInPcWxe;

`input_mvaddr	dcInAddr;		//input PC address
input [ 4: 0]	dcInOpm;		//input PC address
output[63: 0]	dcOutVal;		//output data value
input [63: 0]	dcInVal;		//input data value
output[ 1: 0]	dcOutOK;		//set if we have a valid value.
input			dcInHold;
output			dcOutHold;		//we need to stall the pipeline
output			dcBusWait;		//we are waiting on the bus

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

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

`reg_tile		tBridgeDataI;
`reg_l1maddr		tBridgeAddrI;
reg[15:0]		tBridgeOpmI;
reg[15:0]		tBridgeSeqI;

`wire_tile		tBridgeDataO;
`wire_l1maddr	tBridgeAddrO;
wire[15:0]		tBridgeOpmO;
wire[15:0]		tBridgeSeqO;
wire[ 7:0]		tBridgeNodeId;

McRbiMemL1Bridge	l1bridge(
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


`reg_l1maddr	tMemAddr;		//Memory address
`reg_l1maddr	tMemAddrB;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation

`reg_tile		tMemDataOut;	//Memory Data Out

reg[127:0]		tRegOutExc;
reg[127:0]		tRegOutExc2;

reg[63:0]		tRegTraPc;
reg[63:0]		tRegTraPc2;

wire[5:0]		tMemAccNoRwx;


wire[127:0]		tTlbExc;


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
assign		tTlbInLdtlb = { regInDhr, regInDlr };

`ifdef jx2_enable_mmu

// `wire_tile		tTlbDataI;
// wire[47:0]		tTlbAddrI;
// wire[15:0]		tTlbOpmI;
// wire[15:0]		tTlbSeqI;

`reg_tile		tTlbDataI;
`reg_l1maddr		tTlbAddrI;
reg[15:0]		tTlbOpmI;
reg[15:0]		tTlbSeqI;

`wire_tile		tTlbDataO;
`wire_l1maddr	tTlbAddrO;
wire[15:0]		tTlbOpmO;
wire[15:0]		tTlbSeqO;

McRbiMmuTlb	tlb(
	clock,			reset,

	tTlbAddrI,		tTlbAddrO,
	tTlbDataI,		tTlbDataO,
	tTlbOpmI,		tTlbOpmO,
	tTlbSeqI,		tTlbSeqO,
	tlbMemNodeId,	tTlbInLdtlb,

//	tTlbExc,		dcInHold,
	tTlbExc,		1'b0,
	regInMmcr,		regInKrr,
	regInSr);

`endif


wire[7:0]		regKrrHash;
reg [7:0]		regKrrHashL;
assign		regKrrHash = regInKrr[7:0]^regInKrr[15:8];


wire[5:0]		dfInOpm;
assign		dfInOpm		= { dcInOpm[4:3], 1'b0, dcInOpm[2:0] };

wire			ifMemWait;
wire[127:0]		ifOutExc;

reg			ifMemWaitL;

`reg_tile		ifMemDataI;
`reg_l1maddr	ifMemAddrI;
reg[ 15:0]		ifMemOpmI;
reg[ 15:0]		ifMemSeqI;

`wire_tile		ifMemDataO;
`wire_l1maddr	ifMemAddrO;
wire[ 15:0]		ifMemOpmO;
wire[ 15:0]		ifMemSeqO;
wire[  7:0]		ifMemNodeId;

McRbiMemIcFxA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	dfInOpm,		regInSr,
	ifMemWait,		ifOutExc,
	icOutPcSxo,		regKrrHashL,

	ifMemAddrI,		ifMemAddrO,
	ifMemDataI,		ifMemDataO,
	ifMemOpmI,		ifMemOpmO,
	ifMemSeqI,		ifMemSeqO,

	ifMemNodeId
	);

wire			dfOutHold;
wire			dfOutWait;
wire[127:0]		dfOutExc;

`reg_tile		dfMemDataI;
`reg_l1maddr		dfMemAddrI;
reg[ 15:0]		dfMemOpmI;
reg[ 15:0]		dfMemSeqI;

`wire_tile		dfMemDataO;
`wire_l1maddr	dfMemAddrO;
wire[ 15:0]		dfMemOpmO;
wire[ 15:0]		dfMemSeqO;
wire[  7:0]		dfMemNodeId;

McRbiMemDcA		memDc(
	clock,			reset,
	dcInAddr,		dfInOpm,
	dcOutVal,		dcInVal,
	dcInHold,		dfOutHold,
	regInSr,		dfOutWait,
	dfOutExc,		regInMmcr,
	regKrrHashL,

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
	tTlbDataI		= ifMemDataO;
	tTlbAddrI		= ifMemAddrO;
	tTlbOpmI		= ifMemOpmO;
	tTlbSeqI		= ifMemSeqO;

	tBridgeDataI	= tTlbDataO;
	tBridgeAddrI	= tTlbAddrO;
	tBridgeOpmI		= tTlbOpmO;
	tBridgeSeqI		= tTlbSeqO;
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
	tRegOutExc	= UV128_00;
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

	ifMemWaitL		<= ifMemWait;

	regKrrHashL		<= regKrrHash;

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

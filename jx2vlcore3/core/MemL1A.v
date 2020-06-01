/*
Contain L1 Instruction and Data Caches.

These caches provide a pipelined interface, recieving an address on one cycle, and producing output the next.

These will produce a Hold signal if the request can't be handled immediately, and will not accept new input until the condition is resolved.
*/

`include "MemDcA.v"

`ifdef jx2_enable_wex
`include "MemIcWxA.v"
`else
`include "MemIcA.v"
`endif

`ifdef jx2_enable_mmu
`include "MmuTlb.v"
`endif

module MemL1A(
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
	
	regOutExc,

	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK
	);

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

output[ 47:0]	memAddr;		//Memory address (Primary)
output[ 47:0]	memAddrB;		//Memory address (Secondary)
output[  4:0]	memOpm;			//Memory Operation
input [127:0]	memDataIn;		//Memory Data In
output[127:0]	memDataOut;		//Memory Data Out
input [  1:0]	memOK;			//Memory OK

reg[ 47:0]		tMemAddr;		//Memory address
reg[ 47:0]		tMemAddrB;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation
reg[127:0]		tMemDataOut;	//Memory Data Out

reg[63:0]		tRegOutExc;
reg[63:0]		tRegOutExc2;

wire[3:0]		tMemAccNoRwx;

`ifdef jx2_enable_mmu

wire[47:0]		tTlbAddr;
wire[47:0]		tTlbAddrB;
wire[63:0]		tTlbExc;
wire[1:0]		tTlbOK;
wire[4:0]		tTlbOpm;

assign	memAddr		= tTlbAddr;
assign	memAddrB	= tTlbAddrB;
assign	memOpm		= tTlbOpm;
assign	memDataOut	= tMemDataOut;

`else

assign	memAddr		= tMemAddr;
assign	memAddrB	= tMemAddrB;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

assign	tMemAccNoRwx	= 0;

`endif

assign	regOutExc	= tRegOutExc2;

reg[1:0]		tDcOutOK;
reg				tDcOutHold;
assign	dcOutOK		= tDcOutOK;
assign	dcOutHold	= tDcOutHold;

`ifdef jx2_enable_mmu

wire[127:0]		tTlbLdtlbData;
assign	tTlbLdtlbData = { regInDhr, regInDlr };

MmuTlb	tlb(
	clock,			reset,
	tMemAddr,		tTlbAddr,
	tMemAddrB,		tTlbAddrB,	tTlbLdtlbData,
	tMemOpm,		tTlbOpm,
	tTlbExc,		tTlbOK,		tMemAccNoRwx,
	regInMmcr,		regInKrr,
	regInSr);

`endif

reg [127:0]		ifMemData;
reg [  1:0]		ifMemOK;
wire[ 47:0]		ifMemAddr;
wire[  4:0]		ifMemOpm;

reg [127:0]		ifMemData2;
reg [  1:0]		ifMemOK2;

`ifdef jx2_enable_wex
MemIcWxA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,		dcInOpm,
	ifMemData2,		ifMemAddr,
	ifMemOpm,		ifMemOK2
	);
`else
MemIcA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,		dcInOpm,
	ifMemData,		ifMemAddr,
	ifMemOpm,		ifMemOK
	);
`endif


wire[  1:0]		dfOutOK;
wire[  1:0]		dfOutOKB;

wire[ 47:0]		dfMemAddr;
wire[ 47:0]		dfMemAddrB;
wire[  4:0]		dfMemOpm;
reg [127:0]		dfMemDataIn;
wire[127:0]		dfMemDataOut;
reg [  1:0]		dfMemOK;
reg [  3:0]		dfMemNoRwx;

MemDcA		memDc(
	clock,			reset,
	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dfOutOK,		dcInHold,
	dfOutOKB,

	dfMemAddr,		dfMemAddrB,
	dfMemDataIn,	dfMemDataOut,
	dfMemOpm,		dfMemOK,
	dfMemNoRwx
	);

reg		tLatchIc;
reg		tLatchDc;
reg		tNxtLatchIc;
reg		tNxtLatchDc;
reg		tIfNzOpm;
reg		tDfNzOpm;

reg		tMsgLatch;
reg		tNxtMsgLatch;

always @*
begin
	tNxtLatchIc	= 0;
	tNxtLatchDc	= 0;
	tNxtMsgLatch	= 0;
	tRegOutExc	= UV64_00;

//	tMemAddr	= UV32_XX;
//	tMemAddr	= UV32_00;
	tMemAddr	= UV48_00;
//	tMemAddrB	= UV32_XX;
//	tMemAddrB	= UV32_00;
	tMemAddrB	= UV48_00;
	tMemOpm		= UMEM_OPM_READY;
//	tMemDataOut	= UV128_XX;
	tMemDataOut	= UV128_00;
	
//	tDcOutOK	= dfOutOK;
	tDcOutOK	= dfOutOK[1:0];
	
	tDcOutHold	= 0;
`ifdef	jx2_mem_l1dstall
	tDcOutHold	= dfOutOKB[1];
`endif

	ifMemData	= memDataIn;
	dfMemDataIn	= memDataIn;

	tIfNzOpm	= (ifMemOpm != UMEM_OPM_READY);
	tDfNzOpm	= (dfMemOpm != UMEM_OPM_READY);
	
	ifMemOK	= tIfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;
	dfMemOK	= tDfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;
	dfMemNoRwx	= 0;

	if(dfMemOpm == UMEM_OPM_FAULT)
	begin
//		if(dfMemAddr[0])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8001 };
//		if(dfMemAddr[1])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8002 };

//		tRegOutExc = { UV16_00, dfMemAddr, 12'h800, 
		tRegOutExc = { dfMemAddr, 12'h800, 
			(dfMemAddr[1]) ? 4'h2 : 4'h1 };
	end

	if((tIfNzOpm && !tLatchDc) || tLatchIc)
	begin
//		$display("L1$, Latch I$, A=%X Opm=%X, OK=%d",
//			ifMemAddr, ifMemOpm, memOK);
	
		ifMemOK		= memOK;
		tMemAddr	= ifMemAddr;
		tMemAddrB	= 0;
		tMemOpm		= ifMemOpm;
//		tMemDataOut	= UV128_XX;
		tMemDataOut	= UV128_00;
		tNxtLatchIc	= tIfNzOpm || (memOK != UMEM_OK_READY);
		
		if(tMemAccNoRwx[2])
//			tRegOutExc = {UV16_00, ifMemAddr, 16'h8003 };
			tRegOutExc = { ifMemAddr, 16'h8003 };
	end
	else
		if((tDfNzOpm && !tLatchIc) || tLatchDc)
	begin
//		$display("L1$, Latch D$, Opm=%X", dfMemOpm);

		dfMemOK		= memOK;
		dfMemNoRwx	= tMemAccNoRwx;
		tMemAddr	= dfMemAddr;
		tMemAddrB	= dfMemAddrB;
		tMemOpm		= dfMemOpm;
		tMemDataOut	= dfMemDataOut;
		tNxtLatchDc	= tDfNzOpm || (memOK != UMEM_OK_READY);

		if(tMemAccNoRwx[0] && tMemAccNoRwx[1])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8001 };
			tRegOutExc = { dfMemAddr, 16'h8001 };
	end

	if(memOK==UMEM_OK_FAULT)
	begin
		if(!tMsgLatch)
		begin
//			$display("L1$ Fault, Ic=%d Dc=%d A=%X Opm=%X V=%X",
//				tLatchIc, tLatchDc, tMemAddr, tMemOpm, tMemDataOut);
		end
		tNxtMsgLatch = 1;
		
//		tRegOutExc = {UV16_00, tMemAddr, 16'h8000 };
		tRegOutExc = { tMemAddr, 16'h8000 };
	end

/*
	else
		if((dcInOpm == UMEM_OPM_LDTLB) && !tLatchIc && !tLatchDc)
	begin
		tDcOutOK	= memOK;
		tMemOpm		= dcInOpm;
		tMemAddr	= dcInAddr;
		tMemDataOut	= { regInDhr, regInDlr };
	end
*/
end

always @(posedge clock)
begin	
	ifMemData2		<= ifMemData;
	ifMemOK2		<= ifMemOK;

	if(reset)
	begin
		tRegOutExc2	<= 0;
		tLatchIc	<= 0;
		tLatchDc	<= 0;
		tMsgLatch	<= 0;
	end
	else
	begin
		tRegOutExc2	<= tRegOutExc;
		tLatchIc	<= tNxtLatchIc;
		tLatchDc	<= tNxtLatchDc;
		tMsgLatch	<= tNxtMsgLatch;
	end
end

endmodule

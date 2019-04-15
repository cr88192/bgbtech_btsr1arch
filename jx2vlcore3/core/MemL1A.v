/*
Contain L1 Instruction and Data Caches.

These caches provide a pipelined interface, recieving an address on one cycle, and producing output the next.

These will produce a Hold signal if the request can't be handled immediately, and will not accept new input until the condition is resolved.
*/

`include "MemDcA.v"
`include "MemIcA.v"

module MemL1A(
	/* verilator lint_off UNUSED */
	clock,			reset,

	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutOK,

	regInDlr,		regInDhr,
	regInMmcr,		regInKrr,

	memAddr,		memOpm,
	memDataIn,		memDataOut,
	memOK
	);

input			clock;
input			reset;

input [31: 0]	icInPcAddr;		//input PC address
output[63: 0]	icOutPcVal;		//output PC value
output[ 1: 0]	icOutPcOK;		//set if we have a valid value.
output[ 1: 0]	icOutPcStep;	//PC step (Normal Op)

input [31: 0]	dcInAddr;		//input PC address
input [ 4: 0]	dcInOpm;		//input PC address
output[63: 0]	dcOutVal;		//output PC value
input [63: 0]	dcInVal;		//output PC value
output[ 1: 0]	dcOutOK;		//set if we have a valid value.

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;

output[ 31:0]	memAddr;		//Memory address
output[  4:0]	memOpm;			//Memory Operation
input [127:0]	memDataIn;		//Memory Data In
output[127:0]	memDataOut;		//Memory Data Out
input [  1:0]	memOK;			//Memory OK

reg[ 31:0]		tMemAddr;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation
reg[127:0]		tMemDataOut;	//Memory Data Out

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

reg[1:0]		tDcOutOK;
assign	dcOutOK		= tDcOutOK;


reg [127:0]		ifMemData;
reg [  1:0]		ifMemOK;
wire[ 31:0]		ifMemAddr;
wire[  4:0]		ifMemOpm;

MemIcA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	ifMemData,		ifMemAddr,
	ifMemOpm,		ifMemOK
	);


wire[  1:0]		dfOutOK;

wire[ 31:0]		dfMemAddr;
wire[  4:0]		dfMemOpm;
reg [127:0]		dfMemDataIn;
wire[127:0]		dfMemDataOut;
reg [  1:0]		dfMemOK;

MemDcA		memDc(
	clock,			reset,
	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dfOutOK,

	dfMemAddr,		dfMemOpm,
	dfMemDataIn,	dfMemDataOut,
	dfMemOK
	);

reg		tLatchIc;
reg		tLatchDc;
reg		tNxtLatchIc;
reg		tNxtLatchDc;
reg		tIfNzOpm;
reg		tDfNzOpm;

always @*
begin
	tNxtLatchIc	= 0;
	tNxtLatchDc	= 0;

	tMemAddr	= UV32_XX;
	tMemOpm		= UMEM_OPM_READY;
	tMemDataOut	= UV128_XX;
	
	tDcOutOK	= dfOutOK;


	ifMemData	= memDataIn;
	dfMemDataIn	= memDataIn;

	tIfNzOpm	= (ifMemOpm != UMEM_OPM_READY);
	tDfNzOpm	= (dfMemOpm != UMEM_OPM_READY);
	
	ifMemOK	= tIfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;
	dfMemOK	= tDfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;

	if((tIfNzOpm && !tLatchDc) || tLatchIc)
	begin
//		$display("L1$, Latch I$, Opm=%X", ifMemOpm);
	
		ifMemOK		= memOK;
		tMemAddr	= ifMemAddr;
		tMemOpm		= ifMemOpm;
		tMemDataOut	= UV128_XX;
		tNxtLatchIc	= tIfNzOpm || (memOK != UMEM_OK_READY);
	end
	else
		if((tDfNzOpm && !tLatchIc) || tLatchDc)
	begin
//		$display("L1$, Latch D$, Opm=%X", dfMemOpm);

		dfMemOK		= memOK;
		tMemAddr	= dfMemAddr;
		tMemOpm		= dfMemOpm;
		tMemDataOut	= dfMemDataOut;
		tNxtLatchDc	= tDfNzOpm || (memOK != UMEM_OK_READY);
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
	tLatchIc	<= tNxtLatchIc;
	tLatchDc	<= tNxtLatchDc;
end

endmodule

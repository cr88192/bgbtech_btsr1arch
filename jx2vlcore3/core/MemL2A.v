/*
Deal with L2 Cache sending requests to the right place.
*/

`include "CoreDefs.v"
`include "MemL2Dc.v"
`include "MemL2Rom.v"

module MemL2A(
	clock,
	reset,
	
	memAddr,	memOpm,
	memDataIn,	memDataOut,
	memOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutData,
	mmioOK
	);

input			clock;
input			reset;
	
input[31:0]		memAddr;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[4:0]		memOpm;
output[1:0]		memOK;

input[127:0]	ddrMemDataIn;
output[127:0]	ddrMemDataOut;
output[31:0]	ddrMemAddr;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;

input[31:0]		mmioInData;
output[31:0]	mmioOutData;
output[31:0]	mmioAddr;
output[4:0]		mmioOpm;
input[1:0]		mmioOK;



reg[127:0]		tMemDataOut;
reg[1:0]		tMemOK;
// reg[127:0]		tDdrMemDataOut;
// reg[31:0]		tDdrMemAddr;
// reg[4:0]		tDdrMemOpm;
reg[31:0]		tMmioOutData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;
// assign	ddrMemDataOut	= tDdrMemDataOut;
// assign	ddrMemAddr		= tDdrMemAddr;
// assign	ddrMemOpm		= tDdrMemOpm;
assign	mmioOutData		= tMmioOutData;
assign	mmioAddr		= tMmioAddr;
assign	mmioOpm			= tMmioOpm;

wire	reqIsMmio;
assign	reqIsMmio	= (memOpm[2:0] != 3'b111);

wire[4:0]		l2MemOpm;
assign			l2MemOpm = reqIsMmio ? UMEM_OPM_READY : memOpm;

wire[127:0]		l2MemDataOut;
wire[1:0]		l2MemOK;

MemL2Dc		l2dc(
	clock,		reset,

	memAddr,	l2MemOpm,
	memDataIn,	l2MemDataOut,
	l2MemOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK
	);

wire	tAddrIsLo64k;
assign	tAddrIsLo64k	= (memAddr[31:16] == UV16_00);

wire[4:0]		l2rMemOpm;
assign			l2rMemOpm = tAddrIsLo64k ? UMEM_OPM_READY : memOpm;

wire[127:0]		l2rMemDataOut;
wire[1:0]		l2rMemOK;

MemL2Rom	l2rom(
	clock,		reset,

	memAddr,	l2rMemOpm,
	memDataIn,	l2rMemDataOut,
	l2rMemOK
	);

always @*
begin
end

always @(posedge clock)
begin

	if(reqIsMmio)
	begin
		tMemDataOut		<= { UV96_XX, mmioInData };
		tMemOK			<= mmioOK;
	end
	else
		if(tAddrIsLo64k)
	begin
		tMemDataOut		<= l2rMemDataOut;
		tMemOK			<= l2rMemOK;
	end
	else
	begin
		tMemDataOut		<= l2MemDataOut;
		tMemOK			<= l2MemOK;
	end

//	tDdrMemDataOut;
//	tDdrMemAddr;

	tMmioOutData		<= memDataIn[31:0];
	tMmioAddr			<= memAddr;
	tMmioOpm			<= reqIsMmio ? memOpm : UMEM_OPM_READY;

end

endmodule

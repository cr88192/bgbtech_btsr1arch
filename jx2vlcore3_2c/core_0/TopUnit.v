`include "ExUnit.v"
`include "MemL2A.v"

`include "MmiModGpio.v"
`include "MmiModDdr3.v"

module TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
	fixedPins,

	ddrData,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
	ddrClk
	);

input			clock;
input			reset;
inout[31:0]		gpioPins;
inout[15:0]		fixedPins;

inout[15:0]		ddrData;		//DDR data pins

output[13:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

output[1:0]		ddrClk;			//clock pins

// reg[15:0]		ddrData_I;		//DDR data pins
wire[15:0]		ddrData_I;		//DDR data pins
wire[15:0]		ddrData_O;		//DDR data pins
wire			ddrData_En;		//DDR data pins

assign			ddrData = ddrData_En ? ddrData_O : 16'hzzzz;
assign			ddrData_I	= ddrData;

// reg[127:0]	ddrMemDataIn;
wire[127:0]		ddrMemDataIn;
wire[127:0]		ddrMemDataOut;
// reg[31:0]	ddrMemAddr;
wire[31:0]		ddrMemAddr;
// reg[4:0]		ddrMemOpm;
wire[4:0]		ddrMemOpm;
wire[1:0]		ddrMemOK;

MmiModDdr3		ddr(
	clock, reset,

	ddrMemDataIn,	ddrMemDataOut,
	ddrMemAddr,	ddrMemOpm,
	ddrMemOK,
	
	ddrData_I,
	ddrData_O,
	ddrData_En,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
	ddrClk);

// reg[127:0]		memInData;
wire[127:0]		memInData;
wire[127:0]		memOutData;
wire[31:0]		memAddr;
wire[4:0]		memOpm;
// reg[1:0]		memOK;
wire[1:0]		memOK;

reg[31:0]		mmioInData;
wire[31:0]		mmioOutData;
wire[31:0]		mmioAddr;
wire[4:0]		mmioOpm;
reg[1:0]		mmioOK;

ExUnit	cpu(
	clock, 			reset,

	memAddr,		memOpm,
	memInData,		memOutData,
	memOK
	);


`ifdef def_true

reg[31:0]		gpioInData;
wire[31:0]		gpioOutData;
reg[31:0]		gpioAddr;
reg[4:0]		gpioOpm;
wire[1:0]		gpioOK;

MmiModGpio	gpio(
	clock,			reset,
	gpioPins,		fixedPins,
	
	gpioInData,		gpioOutData,	gpioAddr,
	gpioOpm,		gpioOK
	);

`endif

MemL2A	l2a(
	clock,		reset,

	memAddr,	memOpm,
	memOutData,	memInData,
	memOK,

	ddrMemAddr,		ddrMemOpm,
//	ddrMemDataIn,	ddrMemDataOut,
	ddrMemDataOut,	ddrMemDataIn,
	ddrMemOK,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutData,
	mmioOK
	);


always @*
begin

//	ddrData_I	= ddrData;

//	memInData	= UV128_XX;
//	memOK		= 0;

//	ddrMemDataIn	= memOutData;
//	memInData		= ddrMemDataOut;
//	ddrMemAddr		= memAddr;
//	ddrMemOpm		= memOpm;
//	memOK			= ddrMemOK;


	gpioInData	= mmioOutData[31:0];
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;

	mmioInData	= gpioOutData;
	mmioOK		= gpioOK;

//	memInData	= UV128_XX;
//	memOK		= UMEM_OK_READY;
	
//	if(gpioOK==UMEM_OK_OK)
//	if(memAddr[31:29]==3'b101)
//	if(0)
//	begin
//		memInData	= { UV96_XX, gpioOutData };
//		memOK		= gpioOK;
//	end
end


endmodule

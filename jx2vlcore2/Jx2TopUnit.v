`include "Jx2ExUnit.v"
`include "Jx2ModGpio.v"
`include "Jx2ModDdr.v"

module Jx2TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
	fixedPins,
	ddrData,
	ddrCmd,
	ddrClk
	);

input			clock;
input			reset;
inout[31:0]		gpioPins;
inout[15:0]		fixedPins;

/* verilator lint_off UNOPTFLAT */
inout[15:0]		ddrData;		//DDR data pins
output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins
// /* verilator lint_on UNOPTFLAT */

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

Jx2ExUnit	cpu(
	clock, 			reset,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

/* DDR DRAM Interface */

/* verilator lint_off UNOPTFLAT */

// inout[15:0]		ddrData;		//DDR data pins

wire[15:0]		ddrData_I;		//DDR data pins
wire[15:0]		ddrData_O;		//DDR data pins
wire			ddrData_En;		//DDR data pins

assign		ddrData_I = ddrData;
assign		ddrData = ddrData_En ? ddrData_O : 16'hZZZZ;

Jx2ModDdr ddrPhy(
	clock, 			reset,

	memOutData,		memInData,	
	memAddr,
	memOpm,			memOK,

	ddrData_I,		ddrData_O,		ddrData_En,
	ddrCmd,			ddrClk);

/* verilator lint_on UNOPTFLAT */

reg[31:0]		gpioInData;
wire[31:0]		gpioOutData;
reg[31:0]		gpioAddr;
reg[4:0]		gpioOpm;
wire[1:0]		gpioOK;

Jx2ModGpio	gpio(
	clock,			reset,
	gpioPins,		fixedPins,
	
	gpioInData,		gpioOutData,	gpioAddr,
	gpioOpm,		gpioOK
	);

always @*
begin

//	memInData	= UV128_XX;
//	memOK		= 0;

	gpioInData	= mmioOutData;
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;


	mmioInData	= UV32_XX;
	mmioOK		= UMEM_OK_READY;
	
	if(gpioOK==UMEM_OK_OK)
	begin
		mmioInData	= gpioOutData;
		mmioOK		= gpioOK;
	end
end


endmodule

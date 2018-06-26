`include "Jx2ExUnit.v"
`include "Jx2ModGpio.v"

module BsrTopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
	fixedPins
	);

input			clock;
input			reset;
inout[31:0]		gpioPins;
inout[15:0]		fixedPins;

reg[127:0]		memInData;
wire[127:0]		memOutData;
wire[31:0]		memAddr;
wire[4:0]		memOpm;
reg[1:0]		memOK;

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

	memInData	= UV128_XX;
	memOK		= 0;

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

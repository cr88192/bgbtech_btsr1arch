`include "Jx2ExUnit.v"
`include "Jx2ModGpio.v"
`include "Jx2ModDdr3.v"

`include "ModTxtNtW.v"
`include "ModSblk.v"

module Jx2TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
	fixedPins,
	ddrData,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
//	ddrCmd,
	ddrClk,
	ntscY
	);

input			clock;
input			reset;
inout[31:0]		gpioPins;
inout[15:0]		fixedPins;

/* verilator lint_off UNOPTFLAT */
inout[15:0]		ddrData;		//DDR data pins
output[13:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

output			ntscY;

// output[9:0]		ddrCmd;			//Command/Address pins (LPDDR3)

output[1:0]		ddrClk;			//clock pins
// /* verilator lint_on UNOPTFLAT */

wire			invReset;
assign		invReset = !reset;

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
reg[15:0]		mmioExc;

Jx2ExUnit	cpu(
	clock, 			invReset,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioExc
	);

/* DDR DRAM Interface */

/* verilator lint_off UNOPTFLAT */

// inout[15:0]		ddrData;		//DDR data pins

wire[15:0]		ddrData_I;		//DDR data pins
wire[15:0]		ddrData_O;		//DDR data pins
wire			ddrData_En;		//DDR data pins

assign		ddrData_I = ddrData;
assign		ddrData = ddrData_En ? ddrData_O : 16'hZZZZ;

/*
Jx2ModDdr ddrPhy(
	clock, 			invReset,

	memOutData,		memInData,	
	memAddr,
	memOpm,			memOK,

	ddrData_I,		ddrData_O,		ddrData_En,
	ddrCmd,			ddrClk);
*/

Jx2ModDdr3 ddrPhy(
	clock, 			invReset,

	memOutData,		memInData,	
	memAddr,
	memOpm,			memOK,

	ddrData_I,		ddrData_O,		ddrData_En,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
	ddrClk);

/* verilator lint_on UNOPTFLAT */

reg[31:0]		gpioInData;
wire[31:0]		gpioOutData;
reg[31:0]		gpioAddr;
reg[4:0]		gpioOpm;
wire[1:0]		gpioOK;

Jx2ModGpio	gpio(
	clock,			invReset,
	gpioPins,		fixedPins,
	
	gpioInData,		gpioOutData,	gpioAddr,
	gpioOpm,		gpioOK
	);

wire[3:0]	ntPwmOut;
reg[31:0]	ntAddr;
reg[31:0]	ntInData;
wire[31:0]	ntOutData;
reg[4:0]	ntOpm;
wire[1:0]	ntOK;

ModTxtNtW	ntTxt(
	clock, reset,
	ntPwmOut,
	ntAddr,
	ntInData,
	ntOutData,
	ntOpm,
	ntOK);
	
assign	ntscY = ntPwmOut[3];

wire[1:0]	sndAuPins;
wire		sndIrq1kHz;

reg[31:0]	sndAddr;
reg[31:0]	sndInData;
wire[31:0]	sndOutData;
reg[4:0]	sndOpm;
wire[1:0]	sndOK;

ModSblk		sndBlk(
	clock,		reset,
	sndAuPins,	sndIrq1kHz,
	sndAddr,	sndInData,	sndOutData,
	sndOpm,		sndOK);

reg[15:0]		tMmioExc;

always @*
begin

//	memInData	= UV128_XX;
//	memOK		= 0;

	tMmioExc	= 0;

	gpioInData	= mmioOutData;
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;

	ntInData	= mmioOutData;
	ntAddr		= mmioAddr;
	ntOpm		= mmioOpm;

	sndInData	= mmioOutData;
	sndAddr		= mmioAddr;
	sndOpm		= mmioOpm;


	mmioInData	= UV32_XX;
	mmioOK		= UMEM_OK_READY;
	
//	if(gpioOK==UMEM_OK_OK)
	if(gpioOK!=UMEM_OK_READY)
	begin
		mmioInData	= gpioOutData;
		mmioOK		= gpioOK;
	end

//	if(ntOK==UMEM_OK_OK)
	if(ntOK!=UMEM_OK_READY)
	begin
		mmioInData	= ntOutData;
		mmioOK		= ntOK;
	end

	if(sndOK!=UMEM_OK_READY)
	begin
		mmioInData	= sndOutData;
		mmioOK		= sndOK;
	end
	
	if(sndIrq1kHz)
	begin
		tMmioExc	= 16'hC001;
	end
end

always @(posedge clock)
begin
	mmioExc		<= tMmioExc;
end

endmodule

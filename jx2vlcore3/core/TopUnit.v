`include "ExUnit.v"
`include "MemL2A.v"

`include "MmiModGpio.v"
`include "MmiModDdr3.v"

`include "ModTxtNtW.v"

module TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
	fixedPins,

	ddrData,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
	ddrClk,
	vgaRed, vgaGrn, vgaBlu, vgaHsync, vgaVsync
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

output[3:0]		vgaRed;
output[3:0]		vgaGrn;
output[3:0]		vgaBlu;
output			vgaHsync;
output			vgaVsync;


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

wire[31:0]		dbgOutPc;
wire[63:0]		dbgOutIstr;
wire			dbgExHold1;
wire			dbgExHold2;

wire[31:0]		dbgDcInAddr;
wire[ 4:0]		dbgDcInOpm;
wire[63:0]		dbgDcOutVal;
wire[63:0]		dbgDcInVal;
wire[ 1:0]		dbgDcOutOK;

ExUnit	cpu(
	clock, 			reset,

	memAddr,		memOpm,
	memInData,		memOutData,
	memOK,
	
	dbgOutPc,		dbgOutIstr,
	dbgExHold1,		dbgExHold2,
	
	dbgDcInAddr,	dbgDcInOpm,
	dbgDcOutVal,	dbgDcInVal,
	dbgDcOutOK
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

wire[15:0]	scrnPwmOut;
wire[31:0]	scrnMmioOutData;
wire[1:0]	scrnMmioOK;

assign	vgaRed		= scrnPwmOut[11:8];
assign	vgaGrn		= scrnPwmOut[ 7:4];
assign	vgaBlu		= scrnPwmOut[ 3:0];
assign	vgaHsync	= scrnPwmOut[12];
assign	vgaVsync	= scrnPwmOut[13];

ModTxtNtW	scrn(
	clock,		reset,
	scrnPwmOut,
	mmioAddr,	mmioOutData,	scrnMmioOutData,
	mmioOpm,	scrnMmioOK);

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

	mmioOK		= UMEM_OK_READY;

	if(gpioOK != UMEM_OK_READY)
	begin
		mmioInData	= gpioOutData;
		mmioOK		= gpioOK;
	end

	if(scrnMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= scrnMmioOutData;
		mmioOK		= scrnMmioOK;
	end

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

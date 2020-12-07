`include "ExUnit.v"
`include "MemL2A.v"

`include "MmiModGpio.v"
`include "MmiModDdr3.v"

`include "ModTxtNtW.v"
`include "ModAudPcm.v"
`include "ModPs2Kb.v"
`include "ModSdSpi.v"

module TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
//	gpioPins,
//	fixedPins,

	ddrData,	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,

	vgaRed,		vgaGrn,		vgaBlu,
	vgaHsync,	vgaVsync,

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,
	ps2_clk,	ps2_data,

	sdc_dat,	sdc_clk,	sdc_cmd,	sdc_ena,

	aud_mono_out
	);

input			clock;
input			reset;
// inout[31:0]		gpioPins;
// inout[15:0]		fixedPins;

inout[15:0]		ddrData;		//DDR data pins

// output[13:0]	ddrAddr;		//Address pins
output[12:0]	ddrAddr;		//Address pins
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

output			uartTxD;
input			uartRxD;
input			uartCtS;
output			uartRtS;

inout			ps2_clk;
inout			ps2_data;

inout[3:0]		sdc_dat;
output			sdc_clk;
output			sdc_cmd;
output			sdc_ena;

output			aud_mono_out;

// assign			ps2_clk		= 1'bz;
// assign			ps2_data	= 1'bz;

wire			sdc_sclk;		//clock to SDcard
wire			sdc_do;			//data from SDcard
wire			sdc_di;			//data to SDcard
wire			sdc_cs;			//chip-select for SDcard

assign		sdc_clk	= sdc_sclk;
assign		sdc_do	= sdc_dat[0];
assign		sdc_dat[1]	= 1'bz;
assign		sdc_dat[2]	= 1'bz;
assign		sdc_dat[3]	= sdc_cs;
assign		sdc_cmd		= sdc_di;

reg			clock_halfMhz;

wire		clock_cpu;
`ifdef jx2_cpu_halfclock
assign	clock_cpu	= clock_halfMhz;
`else
assign	clock_cpu	= clock;
`endif

// reg[127:0]	ddrMemDataIn;
wire[127:0]		ddrMemDataIn;
wire[127:0]		ddrMemDataOut;
// reg[31:0]	ddrMemAddr;
wire[31:0]		ddrMemAddr;
// reg[4:0]		ddrMemOpm;
wire[4:0]		ddrMemOpm;
wire[1:0]		ddrMemOK;

wire[13:0]		ddrAddr1;		//Address pins
assign		ddrAddr = ddrAddr1[12:0];

MmiModDdr3		ddr(
	clock, reset,

	ddrMemDataIn,	ddrMemDataOut,
	ddrMemAddr,	ddrMemOpm,
	ddrMemOK,
	
	ddrData_I,
	ddrData_O,
	ddrData_En,
	ddrAddr1,
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
	clock_cpu, 		reset,

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

wire[31:0]		gpioPinsOut;
wire[31:0]		gpioPinsIn;
wire[31:0]		gpioPinsDir;
wire[15:0]		fixedPinsOut;
wire[15:0]		fixedPinsIn;

// assign			uartRxD = fixedPinsOut[0];
// assign			fixedPinsIn[1] = uartTxD;
assign			uartTxD = fixedPinsOut[0];
assign			fixedPinsIn[1] = uartRxD;

MmiModGpio	gpio(
	clock,			reset,
	gpioPinsOut,	gpioPinsIn,		gpioPinsDir,
	fixedPinsOut,	fixedPinsIn,
	
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

wire[1:0]	audPwmOut;
wire[31:0]	audMmioOutData;
wire[1:0]	audMmioOK;
assign		aud_mono_out	= audPwmOut[0];

ModAudPcm	pcm(
	clock,		reset,
	audPwmOut,
	mmioAddr,	mmioOutData,	audMmioOutData,
	mmioOpm,	audMmioOK);

wire[31:0]	kbMmioOutData;
wire[1:0]	kbMmioOK;

ModPs2Kb	ps2kb(
	clock,			reset,
	ps2_clk_i,		ps2_clk_o,		ps2_clk_d,	
	ps2_data_i,		ps2_data_o,		ps2_data_d,
	mmioAddr,		mmioOutData,	kbMmioOutData,
	mmioOpm,		kbMmioOK);

wire[31:0]	sdMmioOutData;
wire[1:0]	sdMmioOK;

ModSdSpi	sdspi(
	clock,		reset,
	sdc_sclk,	sdc_do,
	sdc_di,		sdc_cs,
	mmioAddr,	mmioOutData,	sdMmioOutData,
	mmioOpm,	sdMmioOK,		12'hE04);

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

	mmioInData	= UV32_XX;
	mmioOK		= UMEM_OK_READY;

	if(gpioOK != UMEM_OK_READY)
	begin
		mmioInData	= gpioOutData;
		mmioOK		= gpioOK;
	end
	else if(scrnMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= scrnMmioOutData;
		mmioOK		= scrnMmioOK;
	end
	else if(audMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= audMmioOutData;
		mmioOK		= audMmioOK;
	end
	else if(kbMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= kbMmioOutData;
		mmioOK		= kbMmioOK;
	end
	else if(sdMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= sdMmioOutData;
		mmioOK		= sdMmioOK;
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

always @(posedge clock)
begin
	clock_halfMhz	<= !clock_halfMhz;
end


endmodule

`include "ExUnit.v"
`include "MemL2A.v"

`include "MmiModGpio.v"
`include "MmiModDdr3.v"
`include "MmiModClkp.v"

`include "ModTxtNtW.v"
`include "ModAudPcm.v"
`include "ModAudFm.v"
`include "ModPs2Kb.v"
`include "ModSdSpi.v"

module CoreUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,

	ddrDataI,	ddrDataO,	ddrDataEn,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,

	vgaRed,		vgaGrn,		vgaBlu,
	vgaHsync,	vgaVsync,

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,

	ps2_clk_i,	ps2_data_i,
	ps2_clk_o,	ps2_data_o,
	ps2_clk_d,	ps2_data_d,

	sdc_dat_i,	sdc_dat_o,	sdc_dat_d,
	sdc_clk,	sdc_cmd,	sdc_ena,

	aud_mono_out
	);

input			clock;
input			reset;
// inout[31:0]		gpioPins;
// inout[15:0]		fixedPins;

input[15:0]		ddrDataI;		//DDR data pins
output[15:0]	ddrDataO;		//DDR data pins
output			ddrDataEn;

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

assign			ddrDataO	= ddrData_O;
assign			ddrData_I	= ddrDataI;
assign			ddrDataEn	= ddrData_En;

output[3:0]		vgaRed;
output[3:0]		vgaGrn;
output[3:0]		vgaBlu;
output			vgaHsync;
output			vgaVsync;

output			uartTxD;
input			uartRxD;
input			uartCtS;
output			uartRtS;

input			ps2_clk_i;
output			ps2_clk_o;
output			ps2_clk_d;
input			ps2_data_i;
output			ps2_data_o;
output			ps2_data_d;

input[3:0]		sdc_dat_i;
output[3:0]		sdc_dat_o;
output[3:0]		sdc_dat_d;
output			sdc_clk;
output			sdc_cmd;
output			sdc_ena;

output			aud_mono_out;

// assign			ps2_clk_o	= 1'bz;
// assign			ps2_data_o	= 1'bz;
// assign			ps2_clk_d	= 1'b0;
// assign			ps2_data_d	= 1'b0;

// assign			aud_mono_out	= 1'bz;

wire			sdc_sclk;		//clock to SDcard
wire			sdc_do;			//data from SDcard
wire			sdc_di;			//data to SDcard
wire			sdc_cs;			//chip-select for SDcard

assign		sdc_clk	= sdc_sclk;
assign		sdc_do	= sdc_dat_i[0];
assign		sdc_dat_o[1]	= 1'bz;
assign		sdc_dat_o[2]	= 1'bz;
// assign		sdc_dat_o[3]	= sdc_cs;
assign		sdc_dat_o[3]	= !sdc_cs;
assign		sdc_cmd		= sdc_di;
assign		sdc_dat_d	= 4'b1000;
// assign		sdc_ena		= sdc_cs;
assign		sdc_ena		= 1'b1;

reg			clock_halfMhz;

wire		clock_cpu;
`ifdef jx2_cpu_halfclock
assign	clock_cpu	= clock_halfMhz;
`else
assign	clock_cpu	= clock;
`endif

wire	timer1MHz;
wire	timer64kHz;
wire	timer1kHz;
wire	timer256Hz;

MmiModClkp		clkp(
	clock,	reset,
	timer1MHz,
	timer64kHz,
	timer1kHz,
	timer256Hz);

wire[127:0]		ddrMemDataIn;
wire[127:0]		ddrMemDataOut;
wire[31:0]		ddrMemAddr;
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

wire[127:0]		memInData;
wire[127:0]		memOutData;
wire[31:0]		memAddr;
wire[4:0]		memOpm;
wire[1:0]		memOK;
reg[63:0]		memBusExc;

// reg[31:0]		mmioInData;
wire[31:0]		mmioOutData;
reg[63:0]		mmioInData;
wire[63:0]		mmioOutDataQ;
wire[31:0]		mmioAddr;
wire[4:0]		mmioOpm;
reg[1:0]		mmioOK;
assign		mmioOutData = mmioOutDataQ[31:0];

wire[31:0]		dbgOutPc;
wire[95:0]		dbgOutIstr;
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
	memOK,			memBusExc,
	
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
wire[63:0]		outTimer1MHz;
wire[63:0]		outTimer100MHz;

// assign			uartRxD = fixedPinsOut[0];
// assign			fixedPinsIn[1] = uartTxD;
assign			uartTxD = fixedPinsOut[0];
assign			uartRtS = 1'b0;
assign			fixedPinsIn[1] = uartRxD;

MmiModGpio	gpio(
	clock,			reset,
	gpioPinsOut,	gpioPinsIn,		gpioPinsDir,
	fixedPinsOut,	fixedPinsIn,
	outTimer1MHz,	outTimer100MHz,

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
	ddrMemDataOut,	ddrMemDataIn,
	ddrMemOK,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutDataQ,
	mmioOK
	);

wire[15:0]	scrnPwmOut;
// wire[31:0]	scrnMmioOutData;
wire[63:0]	scrnMmioOutData;
wire[1:0]	scrnMmioOK;

assign	vgaRed		= scrnPwmOut[11:8];
assign	vgaGrn		= scrnPwmOut[ 7:4];
assign	vgaBlu		= scrnPwmOut[ 3:0];
assign	vgaHsync	= scrnPwmOut[12];
assign	vgaVsync	= scrnPwmOut[13];

ModTxtNtW	scrn(
	clock,			reset,				scrnPwmOut,
	mmioOutDataQ,	scrnMmioOutData,	mmioAddr,
	mmioOpm,		scrnMmioOK);

wire[1:0]	audPwmOut;
wire[31:0]	audMmioOutData;
wire[1:0]	audMmioOK;
assign		aud_mono_out	= audPwmOut[0];

wire[7:0]	audAuxPcmL;
wire[7:0]	audAuxPcmR;

ModAudPcm	pcm(
	clock,			reset,
	audPwmOut,		audAuxPcmL,			audAuxPcmR,
	mmioOutData,	audMmioOutData,		mmioAddr,
	mmioOpm,		audMmioOK);

wire[31:0]	fmMmioOutData;
wire[1:0]	fmMmioOK;

ModAudFm	fmsyn(
	clock,			reset,
	audAuxPcmL,		audAuxPcmR,
	mmioOutData,	fmMmioOutData,		mmioAddr,
	mmioOpm,		fmMmioOK);


wire[31:0]	kbMmioOutData;
wire[1:0]	kbMmioOK;

ModPs2Kb	ps2kb(
	clock,			reset,
	ps2_clk_i,		ps2_clk_o,		ps2_clk_d,	
	ps2_data_i,		ps2_data_o,		ps2_data_d,
	mmioOutData,	kbMmioOutData,	mmioAddr,
	mmioOpm,		kbMmioOK);

wire[31:0]	sdMmioOutData;
wire[1:0]	sdMmioOK;

ModSdSpi	sdspi(
	clock,			reset,
	sdc_sclk,		sdc_di,
	sdc_do,			sdc_cs,
	mmioOutData,	sdMmioOutData,	mmioAddr,
	mmioOpm,		sdMmioOK,		12'hE03);


reg[31:0]		mmioAddrL1;
reg[31:0]		mmioAddrL2;
reg[31:0]		mmioAddrL3;
reg[31:0]		mmioAddrL4;

reg				tBusMissLatch;
reg				tNxtBusMissLatch;

reg			timer1kHzL;

always @*
begin

	gpioInData	= mmioOutData[31:0];
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;

//	mmioInData	= UV32_XX;
	mmioInData	= UV64_XX;
	mmioOK		= UMEM_OK_READY;

	memBusExc	= UV64_00;
	tNxtBusMissLatch	= 0;

//	if(timer1MHz)
//		$display("timer1MHz");
//	if(timer64kHz)
//		$display("timer64kHz");
//	if(timer1kHz)
//		$display("timer1kHz");

//	if(timer1kHz && timer1kHzL)
//		$display("Timer: kHz multi-clock pulse");

//	if(timer1kHz)
//		$display("1kHz at %dus, %dns", outTimer1MHz, outTimer100MHz*10);

//	if(timer1kHz && !timer1kHzL)
	if(timer256Hz)
		memBusExc	= { UV48_00, 16'hC001 };

	if(gpioOK != UMEM_OK_READY)
	begin
//		mmioInData	= gpioOutData;
		mmioInData	= { UV32_XX, gpioOutData };
		mmioOK		= gpioOK;
	end
	else if(scrnMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= scrnMmioOutData;
		mmioOK		= scrnMmioOK;
	end
	else if(sdMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_XX, sdMmioOutData };
		mmioOK		= sdMmioOK;
	end
	else if(audMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_XX, audMmioOutData };
		mmioOK		= audMmioOK;
	end
	else if(fmMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_XX, fmMmioOutData };
		mmioOK		= fmMmioOK;
	end
	else if(kbMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_XX, kbMmioOutData };
		mmioOK		= kbMmioOK;
	end
	else if(mmioOpm!=0)
	begin
		if(mmioAddr == mmioAddrL4)
		begin
			if(!tBusMissLatch)
				$display("MMIO Bus Miss A=%X", mmioAddr);
			tNxtBusMissLatch = 1;
		end
	end

end

always @(posedge clock)
begin
//	$display("Clock Edge");

	clock_halfMhz	<= !clock_halfMhz;
	timer1kHzL		<= timer1kHz;

	mmioAddrL1	<= mmioAddr;
	mmioAddrL2	<= mmioAddrL1;
	mmioAddrL3	<= mmioAddrL2;
	mmioAddrL4	<= mmioAddrL3;
	tBusMissLatch	<= tNxtBusMissLatch;
end


endmodule

`include "ExUnit.v"
`include "MemL2A.v"

`ifdef jx2_enable_dualcore
`include "ExMemJoin.v"
`endif

`include "MmiModGpio.v"
`include "MmiModDdr3.v"
`include "MmiModClkp.v"

`include "ModTxtNtW.v"
`include "ModAudPcm.v"
`include "ModAudFm.v"
`include "ModPs2Kb.v"
`include "ModSdSpi.v"

`include "Mod7Seg.v"

module CoreUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,

	ddrDataI,	ddrDataO,	ddrDataEn,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En,

	vgaRed,		vgaGrn,		vgaBlu,
	vgaHsync,	vgaVsync,

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,

	ps2kb_clk_i,	ps2kb_data_i,
	ps2kb_clk_o,	ps2kb_data_o,
	ps2kb_clk_d,	ps2kb_data_d,

	sdc_dat_i,	sdc_dat_o,	sdc_dat_d,
	sdc_clk,	sdc_cmd,	sdc_ena,

	aud_mono_out,
	aud_mono_ena,
	seg_outCharBit,
	seg_outSegBit,

	dbg_exHold1,
	dbg_exHold2,
	dbg_outStatus1,
	dbg_outStatus2,
	dbg_outStatus3,
	dbg_outStatus4,
	dbg_outStatus5,
	dbg_outStatus6,
	dbg_outStatus7,
	dbg_outStatus8
	);

input			clock;
input			reset;
// inout[31:0]		gpioPins;
// inout[15:0]		fixedPins;

input[15:0]		ddrDataI;		//DDR data pins
output[15:0]	ddrDataO;		//DDR data pins
output			ddrDataEn;

output[13:0]	ddrAddr;		//Address pins
// output[12:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

output[1:0]		ddrClk;			//clock pins

input[1:0]		ddrDqsP_I;
input[1:0]		ddrDqsN_I;
output[1:0]		ddrDqsP_O;
output[1:0]		ddrDqsN_O;
output			ddrDqs_En;


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

input			ps2kb_clk_i;
output			ps2kb_clk_o;
output			ps2kb_clk_d;
input			ps2kb_data_i;
output			ps2kb_data_o;
output			ps2kb_data_d;

input[3:0]		sdc_dat_i;
output[3:0]		sdc_dat_o;
output[3:0]		sdc_dat_d;
output			sdc_clk;
output			sdc_cmd;
output			sdc_ena;

output			aud_mono_out;
output			aud_mono_ena;
output[7:0]		seg_outCharBit;
output[7:0]		seg_outSegBit;

output			dbg_exHold1;
output			dbg_exHold2;

output			dbg_outStatus1;
output			dbg_outStatus2;
output			dbg_outStatus3;
output			dbg_outStatus4;

output			dbg_outStatus5;
output			dbg_outStatus6;
output			dbg_outStatus7;
output			dbg_outStatus8;

reg[7:0]		tSegOutCharBit;
reg[7:0]		tSegOutSegBit;
assign	seg_outCharBit = tSegOutCharBit;
assign	seg_outSegBit = tSegOutSegBit;

reg				tDbgExHold1B;
reg				tDbgExHold2B;
reg				tDbgExHold1;
reg				tDbgExHold2;
assign	dbg_exHold1 = tDbgExHold1B;
assign	dbg_exHold2 = tDbgExHold2B;


reg				tDbgOutStatus1B;
reg				tDbgOutStatus2B;
reg				tDbgOutStatus3B;
reg				tDbgOutStatus4B;
reg				tDbgOutStatus5B;
reg				tDbgOutStatus6B;
reg				tDbgOutStatus7B;
reg				tDbgOutStatus8B;

reg				tDbgOutStatus1;
reg				tDbgOutStatus2;
reg				tDbgOutStatus3;
reg				tDbgOutStatus4;
reg				tDbgOutStatus5;
reg				tDbgOutStatus6;
reg				tDbgOutStatus7;
reg				tDbgOutStatus8;

assign	dbg_outStatus1 = tDbgOutStatus1B;
assign	dbg_outStatus2 = tDbgOutStatus2B;
assign	dbg_outStatus3 = tDbgOutStatus3B;
assign	dbg_outStatus4 = tDbgOutStatus4B;
assign	dbg_outStatus5 = tDbgOutStatus5B;
assign	dbg_outStatus6 = tDbgOutStatus6B;
assign	dbg_outStatus7 = tDbgOutStatus7B;
assign	dbg_outStatus8 = tDbgOutStatus8B;

// assign			ps2kb_clk_o	= 1'bz;
// assign			ps2kb_data_o	= 1'bz;
// assign			ps2kb_clk_d	= 1'b0;
// assign			ps2kb_data_d	= 1'b0;

// assign			aud_mono_out	= 1'bz;

wire			sdc_sclk;		//clock to SDcard
wire			sdc_do;			//data from SDcard
wire			sdc_di;			//data to SDcard
wire			sdc_cs;			//chip-select for SDcard

assign		sdc_clk	= sdc_sclk;
assign		sdc_do	= sdc_dat_i[0];
assign		sdc_dat_o[0]	= 1'bz;
assign		sdc_dat_o[1]	= 1'bz;
assign		sdc_dat_o[2]	= 1'bz;
// assign		sdc_dat_o[3]	= sdc_cs;
assign		sdc_dat_o[3]	= !sdc_cs;
assign		sdc_cmd		= sdc_di;
assign		sdc_dat_d	= 4'b1000;
// assign		sdc_ena		= sdc_cs;
// assign		sdc_ena		= 1'b1;
assign		sdc_ena		= 1'b0;

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
reg		timerNoise;
wire	timerNoiseL0;
reg		timerNoiseL1;
reg		timerNoiseL2;
reg		timerNoiseL3;
reg		timerNoiseL4;
reg		timerNoiseL5;
reg		timerNoiseL6;
reg		timerNoiseL7;

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
// assign		ddrAddr = ddrAddr1[12:0];
assign		ddrAddr = ddrAddr1[13:0];

MmiModDdr3		ddr(
	clock, reset,

	ddrMemDataIn,	ddrMemDataOut,
	ddrMemAddr,		ddrMemOpm,
	ddrMemOK,
	
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr1,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En);


`ifdef jx2_enable_dualcore

wire[127:0]		memInData;
wire[127:0]		memOutData;
wire[47:0]		memAddr;
wire[47:0]		memAddrB;
wire[4:0]		memOpm;
wire[1:0]		memOK;
reg[63:0]		memBusExc;

wire[47:0]		dbgOutPc;
wire[95:0]		dbgOutIstr;
wire			dbgExHold1;
wire			dbgExHold2;

wire[47:0]		dbgDcInAddr;
wire[ 4:0]		dbgDcInOpm;
wire[63:0]		dbgDcOutVal;
wire[63:0]		dbgDcInVal;
wire[ 1:0]		dbgDcOutOK;

wire			dbgOutStatus1;
wire			dbgOutStatus2;
wire			dbgOutStatus3;
wire			dbgOutStatus4;
wire			dbgOutStatus5;
wire			dbgOutStatus6;
wire			dbgOutStatus7;
wire			dbgOutStatus8;


wire[127:0]		mem1InData;
wire[127:0]		mem1OutData;
wire[47:0]		mem1AddrA;
wire[47:0]		mem1AddrB;
wire[4:0]		mem1Opm;
wire[1:0]		mem1OK;
wire[63:0]		mem1BusExc;

wire[47:0]		dbg1OutPc;
wire[95:0]		dbg1OutIstr;
wire			dbg1ExHold1;
wire			dbg1ExHold2;

wire[47:0]		dbg1DcInAddr;
wire[ 4:0]		dbg1DcInOpm;
wire[63:0]		dbg1DcOutVal;
wire[63:0]		dbg1DcInVal;
wire[ 1:0]		dbg1DcOutOK;

wire			dbg1OutStatus1;
wire			dbg1OutStatus2;
wire			dbg1OutStatus3;
wire			dbg1OutStatus4;
wire			dbg1OutStatus5;
wire			dbg1OutStatus6;
wire			dbg1OutStatus7;
wire			dbg1OutStatus8;

ExUnit	cpu1(
	clock_cpu, 		reset,

	mem1AddrA,		mem1AddrB,
	mem1InData,		mem1OutData,
	mem1Opm,		mem1OK,
	mem1BusExc,
	
	dbg1OutPc,		dbg1OutIstr,
	dbg1ExHold1,	dbg1ExHold2,
	
	dbg1DcInAddr,	dbg1DcInOpm,
	dbg1DcOutVal,	dbg1DcInVal,
	dbg1DcOutOK,

	dbg1OutStatus1,	dbg1OutStatus2,
	dbg1OutStatus3,	dbg1OutStatus4,
	dbg1OutStatus5,	dbg1OutStatus6,
	dbg1OutStatus7,	dbg1OutStatus8
	);


wire[127:0]		mem2InData;
wire[127:0]		mem2OutData;
wire[47:0]		mem2AddrA;
wire[47:0]		mem2AddrB;
wire[4:0]		mem2Opm;
wire[1:0]		mem2OK;
wire[63:0]		mem2BusExc;

wire[47:0]		dbg2OutPc;
wire[95:0]		dbg2OutIstr;
wire			dbg2ExHold1;
wire			dbg2ExHold2;

wire[47:0]		dbg2DcInAddr;
wire[ 4:0]		dbg2DcInOpm;
wire[63:0]		dbg2DcOutVal;
wire[63:0]		dbg2DcInVal;
wire[ 1:0]		dbg2DcOutOK;

wire			dbg2OutStatus1;
wire			dbg2OutStatus2;
wire			dbg2OutStatus3;
wire			dbg2OutStatus4;
wire			dbg2OutStatus5;
wire			dbg2OutStatus6;
wire			dbg2OutStatus7;
wire			dbg2OutStatus8;

ExUnit	cpu2(
	clock_cpu, 		reset,

	mem2AddrA,		mem2AddrB,
	mem2InData,		mem2OutData,
	mem2Opm,		mem2OK,
	mem2BusExc,
	
	dbg2OutPc,		dbg2OutIstr,
	dbg2ExHold1,	dbg2ExHold2,
	
	dbg2DcInAddr,	dbg2DcInOpm,
	dbg2DcOutVal,	dbg2DcInVal,
	dbg2DcOutOK,

	dbg2OutStatus1,	dbg2OutStatus2,
	dbg2OutStatus3,	dbg2OutStatus4,
	dbg2OutStatus5,	dbg2OutStatus6,
	dbg2OutStatus7,	dbg2OutStatus8
	);

defparam cpu1.isAltCore = 0;
defparam cpu2.isAltCore = 1;

assign		dbgOutPc		= dbg1OutPc;
assign		dbgOutIstr		= dbg1OutIstr;
assign		dbgExHold1		= dbg1ExHold1;
assign		dbgExHold2		= dbg1ExHold2;
assign		dbgDcInAddr		= dbg1DcInAddr;
assign		dbgDcInOpm		= dbg1DcInOpm;
assign		dbgDcOutVal		= dbg1DcOutVal;
assign		dbgDcInVal		= dbg1DcInVal;
assign		dbgDcOutOK		= dbg1DcOutOK;

assign		dbgOutStatus1 = dbg1OutStatus1 || dbg2OutStatus1;
assign		dbgOutStatus2 = dbg1OutStatus2 || dbg2OutStatus2;
assign		dbgOutStatus3 = dbg1OutStatus3 || dbg2OutStatus3;
assign		dbgOutStatus4 = dbg1OutStatus4 || dbg2OutStatus4;
assign		dbgOutStatus5 = dbg1OutStatus5 || dbg2OutStatus5;
assign		dbgOutStatus6 = dbg1OutStatus6 || dbg2OutStatus6;
assign		dbgOutStatus7 = dbg1OutStatus7 || dbg2OutStatus7;
assign		dbgOutStatus8 = dbg1OutStatus8 || dbg2OutStatus8;

ExMemJoin	cpuJoin(
	clock_cpu,	reset,
	memInData,	memOutData,
	memAddr,	memAddrB,
	memOpm,		memOK,
	memBusExc,

	mem1InData,	mem1OutData,
	mem1AddrA,	mem1AddrB,
	mem1Opm,	mem1OK,
	mem1BusExc,

	mem2InData,	mem2OutData,
	mem2AddrA,	mem2AddrB,
	mem2Opm,	mem2OK,
	mem2BusExc
	);

`else

wire[127:0]		memInData;
wire[127:0]		memOutData;
wire[47:0]		memAddr;
wire[47:0]		memAddrB;
wire[4:0]		memOpm;
wire[1:0]		memOK;
reg[63:0]		memBusExc;

wire[47:0]		dbgOutPc;
wire[95:0]		dbgOutIstr;
wire			dbgExHold1;
wire			dbgExHold2;

wire[47:0]		dbgDcInAddr;
wire[ 4:0]		dbgDcInOpm;
wire[63:0]		dbgDcOutVal;
wire[63:0]		dbgDcInVal;
wire[ 1:0]		dbgDcOutOK;

wire			dbgOutStatus1;
wire			dbgOutStatus2;
wire			dbgOutStatus3;
wire			dbgOutStatus4;
wire			dbgOutStatus5;
wire			dbgOutStatus6;
wire			dbgOutStatus7;
wire			dbgOutStatus8;

ExUnit	cpu(
	clock_cpu, 		reset,

	memAddr,		memAddrB,
	memInData,		memOutData,
	memOpm,			memOK,
	memBusExc,
	
	dbgOutPc,		dbgOutIstr,
	dbgExHold1,		dbgExHold2,
	
	dbgDcInAddr,	dbgDcInOpm,
	dbgDcOutVal,	dbgDcInVal,
	dbgDcOutOK,

	dbgOutStatus1,	dbgOutStatus2,
	dbgOutStatus3,	dbgOutStatus4,
	dbgOutStatus5,	dbgOutStatus6,
	dbgOutStatus7,	dbgOutStatus8
	);

`endif


// reg[31:0]		mmioInData;
wire[31:0]		mmioOutData;
reg[63:0]		mmioInData;
wire[63:0]		mmioOutDataQ;
wire[31:0]		mmioAddr;
wire[4:0]		mmioOpm;
reg[1:0]		mmioOK;
assign		mmioOutData = mmioOutDataQ[31:0];

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

assign			fixedPinsIn[0] = 0;
assign			fixedPinsIn[15:2] = 0;
assign			gpioPinsIn = 0;

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

	memAddr,		memAddrB,
	memOutData,		memInData,
	memOpm,			memOK,

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
	mmioOpm,		scrnMmioOK,
	timerNoise,		timer256Hz);

wire[1:0]	audPwmOut;
wire		audPwmEna;
wire[31:0]	audMmioOutData;
wire[1:0]	audMmioOK;

reg			audPwmOut2;
reg			audPwmEna2;
// assign		aud_mono_out	= audPwmOut[0];
assign		aud_mono_out	= audPwmOut2;
assign		aud_mono_ena	= audPwmEna2;

wire[7:0]	audAuxPcmL;
wire[7:0]	audAuxPcmR;

ModAudPcm	pcm(
	clock,			reset,
	audPwmOut,		audPwmEna,
	audAuxPcmL,		audAuxPcmR,
	mmioOutData,	audMmioOutData,		mmioAddr,
	mmioOpm,		audMmioOK,
	timer1MHz,		timer64kHz,
	timer1kHz,		timerNoise);

wire[31:0]	fmMmioOutData;
wire[1:0]	fmMmioOK;

ModAudFm	fmsyn(
	clock,			reset,
	audAuxPcmL,		audAuxPcmR,
	mmioOutData,	fmMmioOutData,		mmioAddr,
	mmioOpm,		fmMmioOK);

wire		timerNoise_NS0;
reg			timerNoise_S0;
wire		timerNoise_NS1;
reg			timerNoise_S1;
wire		timerNoise_NS2;
reg			timerNoise_S2;
wire		timerNoise_NS3;
reg			timerNoise_S3;

assign		timerNoise_NS0 = (scrnPwmOut[8] ^ scrnPwmOut[4] ^ scrnPwmOut[0]);
assign		timerNoise_NS1 = (sdc_di ^ sdc_do);
assign		timerNoise_NS2 = dbg_exHold1 ^ audPwmOut2;
assign		timerNoise_NS3 =
	memAddr[1] ^ memAddr[2] ^ memAddr[3] ^
	memAddr[5] ^ memAddr[7] ^ memAddr[11];

assign		timerNoiseL0 =
	timerNoiseL2	^ timerNoiseL4 ^
	timerNoiseL5	^ timerNoiseL7 ^
	timerNoise_S0	^ timerNoise_S1 ^
	timerNoise_S2	^ timerNoise_S3;


wire[31:0]	kbMmioOutData;
wire[1:0]	kbMmioOK;
reg			ps2kb_lclk_i;
reg			ps2kb_lclk2_i;

reg			ps2kb_clki;
reg			ps2kb_dati;
// assign	ps2kb_clki = ps2kb_clk_i;
// assign	ps2kb_dati = ps2kb_data_i;


ModPs2Kb	ps2kb(
	clock,			reset,
//	ps2kb_clk_i,		ps2kb_clk_o,		ps2kb_clk_d,	
	ps2kb_clki,		ps2kb_clk_o,		ps2kb_clk_d,	
//	ps2kb_data_i,		ps2kb_data_o,		ps2kb_data_d,
	ps2kb_dati,		ps2kb_data_o,		ps2kb_data_d,
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

reg[5:0]		tBusMissCnt;
reg[5:0]		tNxtBusMissCnt;

reg			timer1kHzL;

reg[31:0]		sevSegVal;
wire[7:0]		ssOutCharBit;
wire[7:0]		ssOutSegBit;

Mod7Seg		sevSeg(
	clock,			reset,
	sevSegVal,		timer1kHz,	timerNoise,
	ssOutCharBit,	ssOutSegBit);

always @*
begin

	gpioInData	= mmioOutData[31:0];
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;

//	mmioInData	= UV32_XX;
//	mmioInData	= UV64_XX;
	mmioInData	= UV64_00;
	mmioOK		= UMEM_OK_READY;

	memBusExc	= UV64_00;
	tNxtBusMissLatch	= 0;
	tNxtBusMissCnt		= 0;

//	if(ps2kb_lclk_i!=ps2kb_clk_i)
//	if(ps2kb_lclk2_i!=ps2kb_lclk_i)
//	begin
//		$display("CoreUnit: ps2clk=%d ps2dat=%d", ps2kb_clk_i, ps2kb_data_i);
//	end

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

	if(timer1kHz && !timer1kHzL)
//	if(timer256Hz)
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
		mmioInData	= { UV32_00, sdMmioOutData };
		mmioOK		= sdMmioOK;
	end
	else if(audMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_00, audMmioOutData };
		mmioOK		= audMmioOK;
	end
	else if(fmMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_00, fmMmioOutData };
		mmioOK		= fmMmioOK;
	end
	else if(kbMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= { UV32_00, kbMmioOutData };
		mmioOK		= kbMmioOK;
	end
	else if(mmioOpm!=0)
	begin
		tNxtBusMissCnt = tBusMissCnt + 1;
//		if(tBusMissCnt == 63)
		if(tBusMissCnt == 15)
		begin
			tNxtBusMissCnt	= tBusMissCnt;
			mmioInData		= UV64_00;
			mmioOK			= UMEM_OK_OK;
		end

//		if(mmioAddr == mmioAddrL4)
//		if(tBusMissCnt == 63)
		if(tBusMissCnt == 15)
		begin
			if(!tBusMissLatch)
				$display("MMIO Bus Miss A=%X", mmioAddr);
			tNxtBusMissLatch = 1;
		end		
	end

end


always @(posedge clock_cpu)
begin
	tDbgExHold1			<= dbgExHold1;
	tDbgExHold2			<= dbgExHold2;

	tDbgOutStatus1		<= dbgOutStatus1;
	tDbgOutStatus2		<= dbgOutStatus2;
	tDbgOutStatus3		<= dbgOutStatus3;
	tDbgOutStatus4		<= dbgOutStatus4;
	
	tDbgOutStatus5		<= dbgOutStatus5;
	tDbgOutStatus6		<= dbgOutStatus6;
	tDbgOutStatus7		<= dbgOutStatus7;
	tDbgOutStatus8		<= dbgOutStatus8;
	
	sevSegVal			<= dbgOutPc[31:0];
end

always @(posedge clock)
begin
//	$display("Clock Edge");

	clock_halfMhz	<= !clock_halfMhz;
	timer1kHzL		<= timer1kHz;
	
	timerNoise		<= timerNoiseL7;
	timerNoiseL1	<= timerNoiseL0;
	timerNoiseL2	<= timerNoiseL1;
	timerNoiseL3	<= timerNoiseL2;
	timerNoiseL4	<= timerNoiseL3;
	timerNoiseL5	<= timerNoiseL4;
	timerNoiseL6	<= timerNoiseL5;
	timerNoiseL7	<= timerNoiseL6;
	timerNoise_S0	<= timerNoise_NS0;
	timerNoise_S1	<= timerNoise_NS1;
	timerNoise_S2	<= timerNoise_NS2;
	timerNoise_S3	<= timerNoise_NS3;

	mmioAddrL1	<= mmioAddr;
	mmioAddrL2	<= mmioAddrL1;
	mmioAddrL3	<= mmioAddrL2;
	mmioAddrL4	<= mmioAddrL3;
	tBusMissLatch	<= tNxtBusMissLatch;
	tBusMissCnt		<= tNxtBusMissCnt;

	ps2kb_lclk_i	<= ps2kb_clk_i;
	ps2kb_lclk2_i	<= ps2kb_lclk_i;

	ps2kb_clki		<= ps2kb_clk_i;
	ps2kb_dati		<= ps2kb_data_i;

	tDbgExHold1B		<= tDbgExHold1;
	tDbgExHold2B		<= tDbgExHold2;

	tDbgOutStatus1B		<= tDbgOutStatus1;
	tDbgOutStatus2B		<= tDbgOutStatus2;
	tDbgOutStatus3B		<= tDbgOutStatus3;
	tDbgOutStatus4B		<= tDbgOutStatus4;

	tDbgOutStatus5B		<= tDbgOutStatus5;
	tDbgOutStatus6B		<= tDbgOutStatus6;
	tDbgOutStatus7B		<= tDbgOutStatus7;
	tDbgOutStatus8B		<= tDbgOutStatus8;

	tSegOutCharBit		<= ssOutCharBit;
	tSegOutSegBit		<= ssOutSegBit;
	
	audPwmOut2			<= audPwmOut[0];
	audPwmEna2			<= audPwmEna;

end


endmodule

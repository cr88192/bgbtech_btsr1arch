/*
MMIO Space:

* 0000E000 (R): 1MHz Timer Low
* 0000E004 (R): 1MHz Timer High

* 0000E008(R): PIT Current Value (Clock Cycles)
** Counts down and then triggers an interrupt once it reaches zero.
* 0000E00C(R/W): PIT Reset Value (Clock Cycles)
** Value which is loaded into the counter when the interrupt fires.

* 0000E010(R): Debug Uart RX
* 0000E014(W): Debug Uart TX
* 0000E018(R): Debug Uart Status
** Bit 0: Input Ready
** Bit 1: Input Buffer Full
** Bit 2: Output Buffer Empty
** Bit 3: Output Buffer Full
** Bit 4: Interrupt Status
** Bit 5: Buffer Overrun
** Bit 6: Frame Error
** Bit 7: Parity Error
* E01C(W): Debug Uart Control
** Bit 0: Clear Transmit
** Bit 1: Clear Recieve
** Bit 4: Enable Interrupt

* 0000E020(W): SPI Control
* 0000E024(R/W): SPI Data

* 0000E030(W): SPI Control
* 0000E034(R/W): SPI Data
* 0000E038(R/W): SPI Data 8x

* 0000E040(R): PS2 KB RX
* 0000E044(R): PS2 KB TX
* 0000E048(R): PS2 KB Status

* 0000E050(R): PS2 Mouse RX
* 0000E054(R): PS2 Mouse TX
* 0000E058(R): PS2 Mouse Status

* 0000E060(R/W): LCD Status / Control
* 0000E064(R/W): LCD Read/Write Data/Command
* 0000E068(R): LCD Status

* 0000E080(W): IRQ Bounce

* 0000E100(R): GPIO In
* 0000E104(R/W): GPIO Out
* 0000E108(R/W): GPIO Direction
* 0000E110(W): GPIO Set (Out=Out|Val)
* 0000E114(W): GPIO Clear (Out=Out&(~Val))

* 00080000 .. 00080FFF: Audio Loop Buffer / Registers

* 000A0000 .. 000BFFFF: Reserved for Display Memory (Bypass)
** 000A0000 .. 000BEFFF: Framebuffer Memory
** 000BF000 .. 000BFEFF: Control SRAM
** 000BFF00 .. 000BFFFF: Display Control Registers
*/


`include "ExUnit.v"

`ifdef jx2_use_ringbus
`include "ringbus/RbiMemL2A.v"
`else
`include "MemL2A.v"
`endif

`ifndef jx2_use_ringbus
`ifdef jx2_enable_dualcore
`include "ExMemJoin.v"
`else
`include "ExMemJoinSc.v"
`endif
`endif

`include "MmiModGpio.v"
`include "MmiModClkp.v"

`ifndef jx2_cfg_noddr
`ifdef jx2_mem_useddrb
`include "MmiModDdrB.v"
`else
`include "MmiModDdr3.v"
`endif
`endif

`ifndef jx2_cfg_novga
`include "ModTxtNtW.v"
`endif

`ifndef jx2_cfg_noaudio
`include "ModAudPcm.v"
`include "ModAudFm.v"
`endif

`include "ModPs2Kb.v"
`include "ModSdSpi.v"

`include "Mod7Seg.v"

module CoreUnit(
	/* verilator lint_off UNUSED */
	clock_300,
	clock_200,
	clock_150,
	clock_100,
	clock_75,
	clock_50,

	reset_300,
	reset_200,
	reset_150,
	reset_100,
	reset_75,
	reset_50,

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
	dbg_outStatus8,

	gpioPinsIn,
	gpioPinsOut,
	gpioPinsDir
	);

input			clock_300;
input			clock_200;
input			clock_150;
input			clock_100;
input			clock_75;
input			clock_50;

input			reset_300;
input			reset_200;
input			reset_150;
input			reset_100;
input			reset_75;
input			reset_50;

input[31:0]			gpioPinsIn;
output[31:0]		gpioPinsOut;
output[31:0]		gpioPinsDir;
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

reg[7:0]	regInitSanity;
reg			reset_sanity;
// reg			reset2;
// assign		reset_sanity = (regInitSanity!=8'h55);
// assign		reset2 = reset || reset_sanity;

wire			reset2_master;
wire			reset2_mmio;
wire			reset2_cpu;
wire			reset2_ddr;

wire			reset2_100;

assign		reset2_100 = reset_100;

wire		clock_master;
wire		clock_mmio;
wire		clock_cpu;
wire		clock_ddr;
wire		clock_ddr2;

`ifdef jx2_cpu_masterclock_50
assign		clock_master	= clock_50;
assign		reset2_master	= reset_50;
assign		clock_cpu		= clock_50;
assign		reset2_cpu		= reset_50;
`endif

`ifdef jx2_cpu_masterclock_75
assign		clock_master	= clock_75;
assign		reset2_master	= reset_75;
assign		clock_cpu		= clock_75;
assign		reset2_cpu		= reset_75;
`endif

`ifdef jx2_cpu_masterclock_150
assign		clock_master	= clock_150;
assign		reset2_master	= reset_150;

`ifdef jx2_cpu_halfclock
assign		clock_cpu	= clock_75;
assign		reset2_cpu	= reset_75;
`else
assign		clock_cpu	= clock_150;
assign		reset2_cpu	= reset_150;
`endif
`endif

`ifdef jx2_cpu_masterclock_100
assign		clock_master	= clock_100;
assign		reset2_master	= reset_100;

`ifdef jx2_cpu_halfclock
assign		clock_cpu	= clock_50;
assign		reset2_cpu	= reset_50;
`else
assign		clock_cpu	= clock_100;
assign		reset2_cpu	= reset_100;
`endif
`endif

`ifdef jx2_cpu_mmioclock_75
assign		clock_mmio		= clock_75;
assign		reset2_mmio		= reset_75;
`endif

`ifdef jx2_cpu_mmioclock_150
assign		clock_mmio		= clock_150;
assign		reset2_mmio		= reset_150;
`endif

`ifdef jx2_cpu_mmioclock_100
assign		clock_mmio		= clock_100;
assign		reset2_mmio		= reset_100;
`endif

`ifdef jx2_cpu_mmioclock_50
assign		clock_mmio		= clock_50;
assign		reset2_mmio		= reset_50;
`endif

`ifdef jx2_cpu_ddrclock_150
assign		clock_ddr		= clock_150;
assign		clock_ddr2		= clock_300;
assign		reset2_ddr		= reset_150;
`endif

`ifdef jx2_cpu_ddrclock_100
assign		clock_ddr		= clock_100;
assign		clock_ddr2		= clock_200;
assign		reset2_ddr		= reset_100;
`endif

`ifdef jx2_cpu_ddrclock_200
assign		clock_ddr		= clock_200;
assign		clock_ddr2		= clock_400;
assign		reset2_ddr		= reset_200;
`endif

wire	timer4MHz;
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
//	clock_100,	reset2_100,
	clock_mmio,	reset2_mmio,
	timer4MHz,
	timer1MHz,
	timer64kHz,
	timer1kHz,
	timer256Hz);

`wire_ddrtile	ddrMemDataIn;
`wire_ddrtile	ddrMemDataOut;
wire[31:0]		ddrMemAddr;
wire[4:0]		ddrMemOpm;
wire[1:0]		ddrMemOK;

wire[3:0]		ddrOpSqI;
wire[3:0]		ddrOpSqO;

wire[13:0]		ddrAddr1;		//Address pins
// assign		ddrAddr = ddrAddr1[12:0];
assign		ddrAddr = ddrAddr1[13:0];

`ifdef jx2_cfg_noddr
assign		ddrMemOK		= 0;
assign		ddrMemDataOut	= 0;

assign		ddrData_O	= 0;
assign		ddrData_En	= 0;
assign		ddrAddr1	= 0;
assign		ddrBa		= 0;
assign		ddrCs		= 0;
assign		ddrRas		= 0;
assign		ddrCas		= 0;
assign		ddrWe		= 0;
assign		ddrCke		= 0;
assign		ddrClk		= 0;
assign		ddrDqsP_O	= 0;
assign		ddrDqsN_O	= 0;
assign		ddrDqs_En	= 0;

`else

`ifdef jx2_mem_useddrb
MmiModDdrB		ddr(
	clock_master,	clock_ddr,	clock_ddr2,
	reset2_master,	reset2_ddr,
`else
MmiModDdr3		ddr(
//	clock_100,		clock_200,
//	clock_master,	clock_200,
	clock_master,	clock_ddr,
	reset2_master,	reset2_ddr,
`endif

	ddrMemDataIn,	ddrMemDataOut,
	ddrMemAddr,		ddrMemOpm,
	ddrMemOK,
	ddrOpSqI,		ddrOpSqO,
	
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr1,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En);

`endif

reg[63:0]		mmioInData_L;
reg[1:0]		mmioOK_L;
reg[1:0]		mmioOK_L2;
reg[63:0]		mmioBusExc_L;

reg[63:0]		mmioBusExc;
reg[63:0]		mmioBusExc_A2;
reg[63:0]		mmioBusExc_A1;
reg[63:0]		mmioBusExc_A0;
reg[63:0]		mmioBusExcIn;

reg[63:0]		memNxtBusExcIn;
reg[63:0]		memBusExcIn;


`ifdef jx2_enable_dualcore

`wire_tile		memInData;
`wire_tile		memOutData;
wire[47:0]		memAddr;
wire[47:0]		memAddrB;
wire[47:0]		memAddrIn;
wire[15:0]		memOpm;
wire[15:0]		memOpmIn;
wire[15:0]		memSeq;
wire[15:0]		memSeqIn;
wire[1:0]		memOK;

// reg[63:0]		memBusExcIn;

reg[7:0]		timers_A2;
reg[7:0]		timers_A1;
reg[7:0]		timers_A0;
reg[7:0]		timers;

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


`wire_tile		mem1InData;
`wire_tile		mem1OutData;
wire[47:0]		mem1AddrA;
wire[47:0]		mem1AddrB;
wire[15:0]		mem1Opm;
wire[1:0]		mem1OK;
wire[63:0]		mem1BusExc;

wire[47:0]		mem1AddrIn;
wire[15:0]		mem1OpmIn;
wire[15:0]		mem1SeqOut;
wire[15:0]		mem1SeqIn;
wire[7:0]		mem1NodeId;
assign			mem1NodeId = 8'h04;

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
	clock_cpu, 		reset2_cpu,
	{ 4'h0, timers },

	mem1AddrA,		mem1AddrB,
	mem1InData,		mem1OutData,
	mem1Opm,		mem1OK,
	mem1BusExc,
	
	mem1AddrIn,		mem1OpmIn,
	mem1SeqIn,		mem1SeqOut,
	mem1NodeId,
	
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


`wire_tile		mem2InData;
`wire_tile		mem2OutData;
wire[47:0]		mem2AddrA;
wire[47:0]		mem2AddrB;
wire[15:0]		mem2Opm;
wire[1:0]		mem2OK;
wire[63:0]		mem2BusExc;

wire[47:0]		mem2AddrIn;
wire[15:0]		mem2OpmIn;
wire[15:0]		mem2SeqIn;
wire[15:0]		mem2SeqOut;

wire[7:0]		mem2NodeId;
assign			mem2NodeId = 8'h08;

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
	clock_cpu, 		reset2_cpu,
	{ 4'h1, timers },

	mem2AddrA,		mem2AddrB,
	mem2InData,		mem2OutData,
	mem2Opm,		mem2OK,
	mem2BusExc,

	mem2AddrIn,		mem2OpmIn,
	mem2SeqIn,		mem2SeqOut,
	mem2NodeId,
	
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

`ifdef jx2_use_ringbus

assign	mem1InData	= memInData;
assign	mem2InData	= mem1OutData;
assign	memOutData	= mem2OutData;

assign	mem1AddrIn	= memAddrIn;
assign	mem2AddrIn	= mem1AddrA;
assign	memAddr		= mem2AddrA;

assign	mem1SeqIn	= memSeqIn;
assign	mem2SeqIn	= mem1SeqOut;
assign	memSeq		= mem2SeqOut;

assign	mem1OpmIn	= memOpmIn;
assign	mem2OpmIn	= mem1Opm;
assign	memOpm		= mem2Opm;

assign	mem1OK		= 0;
assign	mem2OK		= 0;

assign	mem1BusExc	= 0;
assign	mem2BusExc	= 0;

`else

ExMemJoin	cpuJoin(
	clock_cpu,	clock_master,	reset2_cpu,
	memInData,	memOutData,
	memAddr,	memAddrB,
	memOpm,		memOK,
	memBusExcIn,

	mem1InData,	mem1OutData,
	mem1AddrA,	mem1AddrB,
	mem1Opm,	mem1OK,
	mem1BusExc,

	mem2InData,	mem2OutData,
	mem2AddrA,	mem2AddrB,
	mem2Opm,	mem2OK,
	mem2BusExc
	);

`endif

`else

`wire_tile		memInData;
`wire_tile		memOutData;
wire[47:0]		memAddr;
wire[47:0]		memAddrB;
wire[15:0]		memOpm;
wire[1:0]		memOK;

wire[15:0]		memSeq;
wire[15:0]		memSeqIn;
wire[47:0]		memAddrIn;
wire[15:0]		memOpmIn;

`wire_tile		mem1InData;
`wire_tile		mem1OutData;
wire[47:0]		mem1AddrA;
wire[47:0]		mem1AddrB;
wire[15:0]		mem1Opm;
wire[1:0]		mem1OK;
wire[63:0]		mem1BusExc;

wire[47:0]		mem1AddrIn;
wire[15:0]		mem1OpmIn;
wire[15:0]		mem1SeqOut;
wire[15:0]		mem1SeqIn;
wire[ 7:0]		mem1NodeId;
assign			mem1NodeId = 8'h04;


// reg[63:0]		memBusExcIn;

reg[7:0]		timers_A2;
reg[7:0]		timers_A1;
reg[7:0]		timers_A0;
reg[7:0]		timers;


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
	clock_cpu, 		reset2_cpu,
	{ 4'h0, timers },

	mem1AddrA,		mem1AddrB,
	mem1InData,		mem1OutData,
	mem1Opm,		mem1OK,
	mem1BusExc,

	mem1AddrIn,		mem1OpmIn,
	mem1SeqIn,		mem1SeqOut,
	mem1NodeId,
	
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

`ifdef jx2_use_ringbus

assign	mem1InData	= memInData;
assign	memOutData	= mem1OutData;
assign	mem1AddrIn	= memAddrIn;
assign	memAddr		= mem1AddrA;

assign	mem1SeqIn	= memSeqIn;
assign	memSeq		= mem1SeqOut;

assign	mem1OpmIn	= memOpmIn;
assign	memOpm		= mem1Opm;
assign	mem1OK		= 0;

assign	mem1BusExc	= 0;

`else

ExMemJoinSc	cpuJoin(
	clock_cpu,	clock_master,	reset2_cpu,
	memInData,	memOutData,
	memAddr,	memAddrB,
	memOpm,		memOK,
	memBusExcIn,

	mem1InData,	mem1OutData,
	mem1AddrA,	mem1AddrB,
	mem1Opm,	mem1OK,
	mem1BusExc
	);
`endif

`endif


// reg[31:0]		mmioInData;
wire[63:0]		mmioOutDataQ_A0;
wire[31:0]		mmioAddr_A0;
wire[4:0]		mmioOpm_A0;
reg[63:0]		mmioOutDataQ_A1;
reg[31:0]		mmioAddr_A1;
reg[4:0]		mmioOpm_A1;
reg[63:0]		mmioOutDataQ_A2;
reg[31:0]		mmioAddr_A2;
reg[4:0]		mmioOpm_A2;
reg[63:0]		mmioOutDataQ_A3;
reg[31:0]		mmioAddr_A3;
reg[4:0]		mmioOpm_A3;
reg[4:0]		mmioOpm_A4;
reg[63:0]		mmioOutDataQ;
reg[31:0]		mmioAddr;
reg[4:0]		mmioOpm;

reg[63:0]		mmioInData;
reg[1:0]		mmioOK;
reg[63:0]		mmioInData_A3;
reg[1:0]		mmioOK_A3;
reg[63:0]		mmioInData_A2;
reg[1:0]		mmioOK_A2;
reg[63:0]		mmioInData_A1;
reg[1:0]		mmioOK_A1;
reg[63:0]		mmioInData_A0;
reg[1:0]		mmioOK_A0;

wire[31:0]		mmioOutData;
assign		mmioOutData = mmioOutDataQ[31:0];

`ifdef def_true

reg[63:0]		gpioInData;
wire[63:0]		gpioOutData;
reg[31:0]		gpioAddr;
reg[4:0]		gpioOpm;
wire[1:0]		gpioOK;

// wire[31:0]		gpioPinsOut;
// wire[31:0]		gpioPinsIn;
// wire[31:0]		gpioPinsDir;
wire[15:0]		fixedPinsOut;
wire[15:0]		fixedPinsIn;
wire[63:0]		outTimer1MHz;
wire[63:0]		outTimer100MHz;

wire[63:0]		gpioBounceIrq;

// assign			uartRxD = fixedPinsOut[0];
// assign			fixedPinsIn[1] = uartTxD;
assign			uartTxD = fixedPinsOut[0];
assign			uartRtS = 1'b0;
assign			fixedPinsIn[1] = uartRxD;

assign			fixedPinsIn[0] = 0;
assign			fixedPinsIn[15:2] = 0;
// assign			gpioPinsIn = 0;

MmiModGpio	gpio(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	gpioPinsOut,	gpioPinsIn,		gpioPinsDir,
	fixedPinsOut,	fixedPinsIn,
	outTimer1MHz,	outTimer100MHz,
	gpioBounceIrq,

	gpioInData,		gpioOutData,	gpioAddr,
	gpioOpm,		gpioOK
	);

`endif

wire[63:0]		memBounceIrq;

`ifdef jx2_use_ringbus

wire[127:0]		l2aDataIn;
wire[127:0]		l2aDataOut;
wire[47:0]		l2aAddrIn;
wire[47:0]		l2aAddrOut;
wire[15:0]		l2aOpmIn;
wire[15:0]		l2aOpmOut;
wire[15:0]		l2aSeqIn;
wire[15:0]		l2aSeqOut;
wire[7:0]		l2aNodeId;

RbiMemL2A	l2a(
	clock_master,	reset2_master,

	l2aAddrIn,		l2aAddrOut,
	l2aDataIn,		l2aDataOut,
	l2aOpmIn,		l2aOpmOut,
	l2aSeqIn,		l2aSeqOut,
	l2aNodeId,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataOut,	ddrMemDataIn,
	ddrMemOK,
	ddrOpSqO,		ddrOpSqI,

	mmioAddr_A0,	mmioOpm_A0,
	mmioInData_A0,	mmioOutDataQ_A0,
	mmioOK_A0,		mmioBusExcIn
	);

assign		l2aDataIn	= memOutData;
assign		l2aAddrIn	= memAddr;
assign		l2aOpmIn	= memOpm;
assign		l2aSeqIn	= memSeq;

assign		memInData	= l2aDataOut;
assign		memAddrIn	= l2aAddrOut;
assign		memOpmIn	= l2aOpmOut;
assign		memSeqIn	= l2aSeqOut;

assign		l2aNodeId	= 8'h82;

assign		memOK = 2'b00;

assign		memBounceIrq = 0;

`else

MemL2A	l2a(
//	clock_100,		reset2_100,
	clock_master,	reset2_master,

	memAddr,		memAddrB,
	memOutData,		memInData,
	memOpm,			memOK,
	memBounceIrq,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataOut,	ddrMemDataIn,
	ddrMemOK,

	mmioAddr_A0,	mmioOpm_A0,
	mmioInData_A0,	mmioOutDataQ_A0,
	mmioOK_A0
	);

assign	ddrOpSqI = 0;


`endif


wire[15:0]	scrnPwmOut;
// wire[31:0]	scrnMmioOutData;
wire[63:0]	scrnMmioOutData;
wire[1:0]	scrnMmioOK;

assign	vgaRed		= scrnPwmOut[11:8];
assign	vgaGrn		= scrnPwmOut[ 7:4];
assign	vgaBlu		= scrnPwmOut[ 3:0];
assign	vgaHsync	= scrnPwmOut[12];
assign	vgaVsync	= scrnPwmOut[13];


`ifndef jx2_cfg_novga

ModTxtNtW	scrn(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	scrnPwmOut,
	mmioOutDataQ,	scrnMmioOutData,	mmioAddr,
	mmioOpm,		scrnMmioOK,
	timerNoise,		timer256Hz);

`else

wire		tScrnCSel;
wire		tScrnOpmNz;

assign		tScrnCSel =
		(mmioAddr[27:16]==12'h00A) ||
		(mmioAddr[27:16]==12'h00B);
assign		tScrnOpmNz = (mmioOpm[4:3] != 0);

assign	scrnPwmOut		= 0;
assign	scrnMmioOutData	= 0;
assign	scrnMmioOK		= (tScrnCSel && tScrnOpmNz) ?
	UMEM_OK_OK : UMEM_OK_READY;

`endif


wire[1:0]	audPwmOut;
wire		audPwmEna;
wire[31:0]	audMmioOutData;
wire[1:0]	audMmioOK;

reg			audPwmOut2;
reg			audPwmEna2;
// assign		aud_mono_out	= audPwmOut[0];
assign		aud_mono_out	= audPwmOut2;
assign		aud_mono_ena	= audPwmEna2;

wire[31:0]	fmMmioOutData;
wire[1:0]	fmMmioOK;


`ifndef jx2_cfg_noaudio

wire[11:0]	audAuxPcmL;
wire[11:0]	audAuxPcmR;

ModAudPcm	pcm(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	audPwmOut,		audPwmEna,
	audAuxPcmL,		audAuxPcmR,
	mmioOutData,	audMmioOutData,		mmioAddr,
	mmioOpm,		audMmioOK,
	timer1MHz,		timer64kHz,
	timer1kHz,		timerNoise);


ModAudFm	fmsyn(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	audAuxPcmL,		audAuxPcmR,
	mmioOutData,	fmMmioOutData,		mmioAddr,
	mmioOpm,		fmMmioOK,
	timer4MHz,		timer1MHz,		timer64kHz,
	timer1kHz,		timerNoise);

`else

assign	audPwmOut		= 0;
assign	audPwmEna		= 0;
assign	audMmioOutData	= 0;
assign	audMmioOK		= 0;

assign	fmMmioOutData	= 0;
assign	fmMmioOK		= 0;

`endif

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
// assign		timerNoise_NS2 = dbg_exHold1 ^ audPwmOut2;
assign		timerNoise_NS2 = dbg_exHold1 ^ audPwmOut2 ^ clock_halfMhz;
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

wire[1:0]	ps2_clkdat_i;
wire[1:0]	ps2_clkdat_o;
wire[1:0]	ps2_clkdat_d;

assign		ps2_clkdat_i = { ps2kb_clk_i, ps2kb_data_i };

assign		ps2kb_clk_o = ps2_clkdat_o[1];
assign		ps2kb_data_o = ps2_clkdat_o[0];
assign		ps2kb_clk_d = ps2_clkdat_d[1];
assign		ps2kb_data_d = ps2_clkdat_d[0];

ModPs2Kb	ps2kb(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
//	ps2kb_clk_i,	ps2kb_clk_o,	ps2kb_clk_d,	
//	ps2kb_clki,		ps2kb_clk_o,	ps2kb_clk_d,	
//	ps2kb_data_i,	ps2kb_data_o,	ps2kb_data_d,
//	ps2kb_dati,		ps2kb_data_o,	ps2kb_data_d,
	ps2_clkdat_i,	ps2_clkdat_o,	ps2_clkdat_d,
	mmioOutData,	kbMmioOutData,	mmioAddr,
	mmioOpm,		kbMmioOK);

wire[63:0]	sdMmioOutData;
wire[1:0]	sdMmioOK;

ModSdSpi	sdspi(
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	sdc_sclk,		sdc_di,
	sdc_do,			sdc_cs,
	mmioOutDataQ,	sdMmioOutData,	mmioAddr,
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
//	clock_100,		reset2_100,
	clock_mmio,		reset2_mmio,
	sevSegVal,		timer1kHz,	timerNoise,
	ssOutCharBit,	ssOutSegBit);


initial begin
	regInitSanity = 0;
end

always @*
begin

//	gpioInData	= mmioOutData[31:0];
	gpioInData	= mmioOutDataQ;
	gpioAddr	= mmioAddr;
	gpioOpm		= mmioOpm;

//	mmioInData	= UV32_XX;
//	mmioInData	= UV64_XX;
	mmioInData	= UV64_00;
	mmioOK		= UMEM_OK_READY;

//	memBusExc			= UV64_00;
	mmioBusExc			= UV64_00;
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
		mmioBusExc	= { UV48_00, 16'hC001 };

//	if(gpioBounceIrq[15])
//		mmioBusExc	= gpioBounceIrq;
//	if(memBounceIrq[15])
//		mmioBusExc	= memBounceIrq;

	if(gpioOK != UMEM_OK_READY)
	begin
		mmioInData	= gpioOutData;
//		mmioInData	= { UV32_XX, gpioOutData };
		mmioOK		= gpioOK;
	end
	else if(scrnMmioOK != UMEM_OK_READY)
	begin
		mmioInData	= scrnMmioOutData;
		mmioOK		= scrnMmioOK;
	end
	else if(sdMmioOK != UMEM_OK_READY)
	begin
//		mmioInData	= { UV32_00, sdMmioOutData };
		mmioInData	= sdMmioOutData;
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
	
	timers_A2			<= {
		1'b0,
		1'b0,
		timer4MHz,
		timer1MHz,
		timer64kHz,
		timer1kHz,
		timer256Hz,
		timerNoise
	};
	
	timers_A1	<= timers_A2;
	timers_A0	<= timers_A1;
	timers		<= timers_A0;

end

always @*
begin
	memNxtBusExcIn = mmioBusExcIn;

	if(memBounceIrq[15])
		memNxtBusExcIn	= memBounceIrq;
end


always @(posedge clock_master)
begin
	memBusExcIn		<= memNxtBusExcIn;

`ifdef jx2_mmio_exbuf
	mmioInData_A3	<= mmioInData_L;
//	mmioOK_A3		<= mmioOK_L;
	mmioOK_A3		<= mmioOK_L2;
	mmioBusExc_A2	<= mmioBusExc_L;

	mmioInData_A2	<= mmioInData_A3;
	mmioOK_A2		<= mmioOK_A3;
	mmioBusExc_A1	<= mmioBusExc_A2;

	mmioInData_A1	<= mmioInData_A2;
	mmioOK_A1		<= mmioOK_A2;
	mmioBusExc_A0	<= mmioBusExc_A1;

	mmioInData_A0	<= mmioInData_A1;
	mmioOK_A0		<= mmioOK_A1;
	mmioBusExcIn	<= mmioBusExc_A0;


`else
//	mmioInData_A0	<= mmioInData;
//	mmioOK_A0		<= mmioOK;
//	mmioBusExcIn	<= mmioBusExc;

	mmioInData_A0	<= mmioInData_L;
//	mmioOK_A0		<= mmioOK_L;
	mmioOK_A0		<= mmioOK_L2;
	mmioBusExcIn	<= mmioBusExc_L;
`endif
end

// always @(posedge clock_100)
always @(posedge clock_mmio)
begin
`ifdef jx2_mmio_exbuf

	mmioOutDataQ_A1		<= mmioOutDataQ_A0;
	mmioAddr_A1			<= mmioAddr_A0;
	mmioOpm_A1			<= mmioOpm_A0;

	mmioOutDataQ_A2		<= mmioOutDataQ_A1;
	mmioAddr_A2			<= mmioAddr_A1;
	mmioOpm_A2			<= mmioOpm_A1;

	mmioOutDataQ_A3		<= mmioOutDataQ_A2;
	mmioAddr_A3			<= mmioAddr_A2;
	mmioOpm_A3			<= mmioOpm_A2;

	mmioOpm_A4			<= mmioOpm_A3;

	mmioOutDataQ		<= mmioOutDataQ_A3;
	mmioAddr			<= mmioAddr_A3;
//	mmioOpm				<= mmioOpm_A3;
	mmioOpm				<= mmioOpm_A4;
`else
	mmioOutDataQ		<= mmioOutDataQ_A0;
	mmioAddr			<= mmioAddr_A0;
//	mmioOpm				<= mmioOpm_A0;

	mmioOpm_A1			<= mmioOpm_A0;
	mmioOpm				<= mmioOpm_A1;
`endif
end

// always @(posedge clock_100)
always @(posedge clock_mmio)
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

	mmioAddrL1		<= mmioAddr;
	mmioAddrL2		<= mmioAddrL1;
	mmioAddrL3		<= mmioAddrL2;
	mmioAddrL4		<= mmioAddrL3;
	tBusMissLatch	<= tNxtBusMissLatch;
	tBusMissCnt		<= tNxtBusMissCnt;

	ps2kb_lclk_i	<= ps2kb_clk_i;
	ps2kb_lclk2_i	<= ps2kb_lclk_i;

	ps2kb_clki		<= ps2kb_clk_i;
	ps2kb_dati		<= ps2kb_data_i;

	mmioInData_L	<= mmioInData;
	mmioOK_L		<= mmioOK;
	mmioOK_L2		<= mmioOK_L;
	mmioBusExc_L	<= mmioBusExc;

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

`ifndef def_true
//	if(reset || (regInitSanity!=8'h55))
//	begin
//	end

	reset_sanity 	<= (regInitSanity!=8'h55);
	reset2			<= reset || reset_sanity;

	if(reset)
	begin
		regInitSanity	<= 0;
	end
	else
	begin
		regInitSanity	<= 8'h55;
	end
`endif

end


endmodule

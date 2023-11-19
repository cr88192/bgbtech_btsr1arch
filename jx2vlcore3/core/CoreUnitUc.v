/*
Core Unit for Microcontroller.

Wrap normal core-unit, but leave many IO pins as stubs.
Use modified configuration options for microcontroller.

Microcontroller config will have reduced or disabled display output, and assume a lack of external RAM.

*/


`define jx2_cfg_microctrl
`define jx2_cfg_novga
`define jx2_cfg_noddr
`define jx2_cfg_noaudio
`define jx2_cfg_l2noram
`define jx2_cfg_ucrom		//Use Microcontroller ROM

`include "CoreUnit.v"

module CoreUnitUc(
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

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,

	sdc_dat_i,	sdc_dat_o,	sdc_dat_d,
	sdc_clk,	sdc_cmd,	sdc_ena,

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
	dbg_outStatus9,
	dbg_outStatus10,
	dbg_outStatus11,
	dbg_outStatus12,

	gpioPinsIn,
	gpioPinsOut,
	gpioPinsDir,
	
	hbrPwmOut
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

// inout[31:0]		gpioPins;
// inout[15:0]		fixedPins;

output			uartTxD;
input			uartRxD;
input			uartCtS;
output			uartRtS;

input[3:0]		sdc_dat_i;
output[3:0]		sdc_dat_o;
output[3:0]		sdc_dat_d;
output			sdc_clk;
output			sdc_cmd;
output			sdc_ena;

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
output			dbg_outStatus9;
output			dbg_outStatus10;
output			dbg_outStatus11;
output			dbg_outStatus12;

input[31:0]			gpioPinsIn;
output[31:0]		gpioPinsOut;
output[31:0]		gpioPinsDir;

output[7:0]		hbrPwmOut;



wire[15:0]		ddrDataI;		//DDR data pins
wire[15:0]		ddrDataO;		//DDR data pins
wire			ddrDataEn;

assign		ddrDataI = 0;

wire[13:0]		ddrAddr;		//Address pins
wire[2:0]		ddrBa;			//Bank Address pins

wire			ddrCs;
wire			ddrRas;
wire			ddrCas;
wire			ddrWe;
wire			ddrCke;

wire[1:0]		ddrClk;			//clock pins

wire[1:0]		ddrDqsP_I;
wire[1:0]		ddrDqsN_I;
wire[1:0]		ddrDqsP_O;
wire[1:0]		ddrDqsN_O;
wire			ddrDqs_En;

assign		ddrDqsP_I = 0;
assign		ddrDqsN_I = 0;

wire[4:0]		vgaRed;
wire[4:0]		vgaGrn;
wire[4:0]		vgaBlu;
wire			vgaHsync;
wire			vgaVsync;


wire			ps2kb_clk_i;
wire			ps2kb_clk_o;
wire			ps2kb_clk_d;
wire			ps2kb_data_i;
wire			ps2kb_data_o;
wire			ps2kb_data_d;

assign		ps2kb_clk_i = 1;
assign		ps2kb_data_i = 1;

wire			aud_mic_clk;
wire			aud_mic_data;
assign	aud_mic_data = 1'b0;


wire[1:0]		aud_mono_out;
wire			aud_mono_ena;
wire[7:0]		seg_outCharBit;
wire[7:0]		seg_outSegBit;

wire[3:0]		usb_clkdat_i;
wire[3:0]		usb_clkdat_o;
wire[3:0]		usb_clkdat_d;
wire[1:0]		usb_clkref;

assign		usb_clkdat_i = 0;

wire[3:0]		ddrModeIn;
assign		ddrModeIn = 0;

// wire[7:0]	hbrPwmOut;

wire[1:0]	dbgExWidth;
wire[7:0]	dbgMisc;


CoreUnit	core(
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
	ddrModeIn,

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

	aud_mic_clk,
	aud_mic_data,

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
	
	dbg_outStatus9,
	dbg_outStatus10,
	dbg_outStatus11,
	dbg_outStatus12,
	dbgExWidth,
	dbgMisc,

	gpioPinsIn,
	gpioPinsOut,
	gpioPinsDir,
	
	usb_clkdat_i,
	usb_clkdat_o,
	usb_clkdat_d,
	usb_clkref,
	
	hbrPwmOut
	);

endmodule

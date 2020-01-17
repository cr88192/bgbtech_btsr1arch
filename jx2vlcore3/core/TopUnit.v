/*
Deal with tristate IO interfacing.
This is handled in its own module as Verilator can't deal with it.
 */

`include "CoreUnit.v"

module TopUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,
//	gpioPins,
//	fixedPins,

	ddrData,	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP,	ddrDqsN,

	vgaRed,		vgaGrn,		vgaBlu,
	vgaHsync,	vgaVsync,

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,
	ps2_clk,	ps2_data,

	sdc_dat,	sdc_clk,	sdc_cmd,	sdc_ena,

	aud_mono_out,	aud_mono_en,
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

inout[15:0]		ddrData;		//DDR data pins

output[13:0]	ddrAddr;		//Address pins
// output[12:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

output[1:0]		ddrClk;			//clock pins

inout[1:0]		ddrDqsP;
inout[1:0]		ddrDqsN;

wire	reset2;
assign	reset2 = !reset;

wire[15:0]		ddrData_I;		//DDR data pins
wire[15:0]		ddrData_O;		//DDR data pins
wire			ddrData_En;		//DDR data pins

wire			ddrDqs_En;
wire[1:0]		ddrDqsP_O;
wire[1:0]		ddrDqsN_O;
wire[1:0]		ddrDqsP_I;
wire[1:0]		ddrDqsN_I;

assign			ddrData		= ddrData_En ? ddrData_O : 16'hzzzz;
assign			ddrData_I	= ddrData;

assign			ddrDqsP		= ddrDqs_En ? ddrDqsP_O : 2'bzz;
assign			ddrDqsN		= ddrDqs_En ? ddrDqsN_O : 2'bzz;
assign			ddrDqsP_I	= ddrDqsP;
assign			ddrDqsN_I	= ddrDqsN;

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
output			aud_mono_en;
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

wire		ps2_clk_i;
wire		ps2_data_i;
wire		ps2_clk_o;
wire		ps2_data_o;
wire		ps2_clk_d;
wire		ps2_data_d;
assign		ps2_clk		= ps2_clk_d  ? ps2_clk_o  : 1'bz;
assign		ps2_data	= ps2_data_d ? ps2_data_o : 1'bz;
assign		ps2_clk_i	= ps2_clk;
assign		ps2_data_i	= ps2_data;

wire[3:0]		sdc_dat_i;
wire[3:0]		sdc_dat_o;
wire[3:0]		sdc_dat_d;

assign	sdc_dat[0]		= sdc_dat_d[0] ? sdc_dat_o[0] : 1'bz;
assign	sdc_dat[1]		= sdc_dat_d[1] ? sdc_dat_o[1] : 1'bz;
assign	sdc_dat[2]		= sdc_dat_d[2] ? sdc_dat_o[2] : 1'bz;
assign	sdc_dat[3]		= sdc_dat_d[3] ? sdc_dat_o[3] : 1'bz;
assign	sdc_dat_i[0]	= sdc_dat[0];
assign	sdc_dat_i[1]	= sdc_dat[1];
assign	sdc_dat_i[2]	= sdc_dat[2];
assign	sdc_dat_i[3]	= sdc_dat[3];

wire aud_mono_out1;
wire aud_mono_ena1;
assign	aud_mono_out	= aud_mono_out1 ? 1'bz : 1'b0;
// assign	aud_mono_out = aud_mono_out1;
// assign	aud_mono_en		= 1'b1;
assign	aud_mono_en		= aud_mono_ena1;

CoreUnit core(
	clock, 		reset2,
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En,

//	vgaRed,		vgaGrn,		vgaBlu,
	vgaBlu,		vgaGrn,		vgaRed,
	vgaHsync,	vgaVsync,
	uartTxD,	uartRxD,
	uartCtS,	uartRtS,
	ps2_clk_i,	ps2_data_i,
	ps2_clk_o,	ps2_data_o,
	ps2_clk_d,	ps2_data_d,
	sdc_dat_i,	sdc_dat_o,	sdc_dat_d,
	sdc_clk,	sdc_cmd,	sdc_ena,

	aud_mono_out1,	aud_mono_ena1,
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

endmodule

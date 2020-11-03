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
// assign	reset2 = !reset;

wire	reset2a;
assign	reset2a = !reset;

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

wire	clock_100mhz;
wire	clock_200mhz;
wire	clock_50mhz;

reg[15:0]	regInitSanity;
reg			reset_sanity;
reg			reset3;
// assign		reset_sanity = (regInitSanity!=8'h55);
// assign		reset2 = reset || reset_sanity;


CoreUnit core(
//	clock, 		reset2,
	clock_100mhz, 	clock_200mhz,
	clock_50mhz,	reset2,
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En,

	vgaRed,		vgaGrn,		vgaBlu,
//	vgaBlu,		vgaGrn,		vgaRed,
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

`ifdef def_true
wire	sys_clk;

wire	gen_clk_100mhz;
wire	gen_clk_200mhz;

wire	gen_clk_50mhz;
wire	gen_clk_25mhz;
wire	gen_clk_75mhz;
wire	gen_clk_66mhz;

wire	clk_feedback, clk_locked;
wire	gen_clk_100mhz_nobuf;
wire	gen_clk_200mhz_nobuf;
wire	gen_clk_50mhz_nobuf;

`ifndef def_true
PLLE2_BASE	#(
	.BANDWIDTH("OPTIMIZED"),	// OPTIMIZED, HIGH, LOW
	.CLKFBOUT_PHASE(0.0),		// Phase offset in degrees of CLKFB, (-360-360)
	.CLKIN1_PERIOD(10.0),		// Input clock period in ns resolution
	// CLKOUT0_DIVIDE - CLKOUT5_DIVIDE: divide amount for each CLKOUT(1-128)
	.CLKFBOUT_MULT(8),			// Multiply value for all CLKOUT (2-64)
	.CLKOUT0_DIVIDE(8),			// 100 MHz
//	.CLKOUT1_DIVIDE(4),			// 200 MHz
//	.CLKOUT1_DIVIDE(6),			// 133 MHz
	.CLKOUT1_DIVIDE(5),			// 160 MHz
	.CLKOUT2_DIVIDE(16),		//  50 MHz
	.CLKOUT3_DIVIDE(32),		//  25 MHz
	.CLKOUT4_DIVIDE(10),		//  80 MHz
	.CLKOUT5_DIVIDE(12),		//  66 MHz
	// CLKOUT0_DUTY_CYCLE -- Duty cycle for each CLKOUT
	.CLKOUT0_DUTY_CYCLE(0.5),
	.CLKOUT1_DUTY_CYCLE(0.5),
	.CLKOUT2_DUTY_CYCLE(0.5),
	.CLKOUT3_DUTY_CYCLE(0.5),
	.CLKOUT4_DUTY_CYCLE(0.5),
	.CLKOUT5_DUTY_CYCLE(0.5),
	// CLKOUT0_PHASE -- phase offset for each CLKOUT
	.CLKOUT0_PHASE(0.0),
	.CLKOUT1_PHASE(0.0),
	.CLKOUT2_PHASE(0.0),
	.CLKOUT3_PHASE(0.0),
	.CLKOUT4_PHASE(0.0),
	.CLKOUT5_PHASE(0.0),
	.DIVCLK_DIVIDE(1),		// Master division value , (1-56)
	.REF_JITTER1(0.0),		// Ref. input jitter in UI (0.000-0.999)
	.STARTUP_WAIT("TRUE")	// Delay DONE until PLL Locks, ("TRUE"/"FALSE")
) genclock(
	// Clock outputs: 1-bit (each) output
	.CLKOUT0(gen_clk_100mhz_nobuf),
	.CLKOUT1(gen_clk_200mhz_nobuf),
	.CLKOUT2(gen_clk_50mhz_nobuf),
	.CLKOUT3(gen_clk_25mhz),
	.CLKOUT4(gen_clk_75mhz),
	.CLKOUT5(gen_clk_66mhz),
	.CLKFBOUT(clk_feedback), // 1-bit output, feedback clock
	.LOCKED(clk_locked),
	.CLKIN1(sys_clk),
	.PWRDWN(1'b0),
	.RST(1'b0),
	.CLKFBIN(clk_feedback_bufd)	// 1-bit input, feedback clock
);
`endif

`ifdef def_true
PLLE2_BASE	#(
	.BANDWIDTH("OPTIMIZED"),	// OPTIMIZED, HIGH, LOW
	.CLKFBOUT_PHASE(0.0),		// Phase offset in degrees of CLKFB, (-360-360)
	.CLKIN1_PERIOD(10.0),		// Input clock period in ns resolution
	// CLKOUT0_DIVIDE - CLKOUT5_DIVIDE: divide amount for each CLKOUT(1-128)
	.CLKFBOUT_MULT(12),			// Multiply value for all CLKOUT (2-64)
	.CLKOUT0_DIVIDE(12),			// 100 MHz
//	.CLKOUT1_DIVIDE(6),			// 200 MHz
	.CLKOUT1_DIVIDE(8),			// 150 MHz
//	.CLKOUT1_DIVIDE(10),		// 120 MHz
	.CLKOUT2_DIVIDE(24),		//  50 MHz
	.CLKOUT3_DIVIDE(48),		//  25 MHz
	.CLKOUT4_DIVIDE(16),		//  80 MHz
	.CLKOUT5_DIVIDE(20),		//  66 MHz
	// CLKOUT0_DUTY_CYCLE -- Duty cycle for each CLKOUT
	.CLKOUT0_DUTY_CYCLE(0.5),
	.CLKOUT1_DUTY_CYCLE(0.5),
	.CLKOUT2_DUTY_CYCLE(0.5),
	.CLKOUT3_DUTY_CYCLE(0.5),
	.CLKOUT4_DUTY_CYCLE(0.5),
	.CLKOUT5_DUTY_CYCLE(0.5),
	// CLKOUT0_PHASE -- phase offset for each CLKOUT
	.CLKOUT0_PHASE(0.0),
	.CLKOUT1_PHASE(0.0),
	.CLKOUT2_PHASE(0.0),
	.CLKOUT3_PHASE(0.0),
	.CLKOUT4_PHASE(0.0),
	.CLKOUT5_PHASE(0.0),
	.DIVCLK_DIVIDE(1),		// Master division value , (1-56)
	.REF_JITTER1(0.0),		// Ref. input jitter in UI (0.000-0.999)
	.STARTUP_WAIT("TRUE")	// Delay DONE until PLL Locks, ("TRUE"/"FALSE")
) genclock(
	// Clock outputs: 1-bit (each) output
	.CLKOUT0(gen_clk_100mhz_nobuf),
	.CLKOUT1(gen_clk_200mhz_nobuf),
	.CLKOUT2(gen_clk_50mhz_nobuf),
	.CLKOUT3(gen_clk_25mhz),
	.CLKOUT4(gen_clk_75mhz),
	.CLKOUT5(gen_clk_66mhz),
	.CLKFBOUT(clk_feedback), // 1-bit output, feedback clock
	.LOCKED(clk_locked),
	.CLKIN1(sys_clk),
	.PWRDWN(1'b0),
	.RST(1'b0),
	.CLKFBIN(clk_feedback_bufd)	// 1-bit input, feedback clock
);
`endif

// BUFH	feedback_buffer(.I(clk_feedback),.O(clk_feedback_bufd));
BUFG	feedback_buffer(.I(clk_feedback),.O(clk_feedback_bufd));
IBUF	sysclk_buf(.I(clock), .O(sys_clk));

BUFG	clk100_buf(.I(gen_clk_100mhz_nobuf), .O(gen_clk_100mhz));
BUFG	clk200_buf(.I(gen_clk_200mhz_nobuf), .O(gen_clk_200mhz));
BUFG	clk50_buf(.I(gen_clk_50mhz_nobuf), .O(gen_clk_50mhz));

assign	clock_100mhz = gen_clk_100mhz;
assign	clock_200mhz = gen_clk_200mhz;
assign	clock_50mhz = gen_clk_50mhz;

`endif


initial begin
	regInitSanity = 0;
end

always @(posedge clock_50mhz)
begin
	reset_sanity 	<= (regInitSanity!=16'hAA55);
//	reset3			<= reset2a || reset_sanity;
	reset3			<= reset_sanity;

	if(reset2a)
	begin
		regInitSanity	<= 0;
	end
	else
	begin
		if(regInitSanity != 16'hAA55)
			regInitSanity	<= regInitSanity + 1;

//		regInitSanity	<= 16'hAA55;
	end
end

BUFG	reset2_buf(.I(reset3), .O(reset2));

endmodule

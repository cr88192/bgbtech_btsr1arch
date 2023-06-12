/*
Deal with tristate IO interfacing.
This is handled in its own module as Verilator can't deal with it.
 */

`include "CoreUnitUc.v"

module TopUnitUc(
	/* verilator lint_off UNUSED */
	clock,		reset,
	gpioPins,
//	fixedPins,

	uartTxD,	uartRxD,
//	uartCtS,	uartRtS,

//	sdc_dat,
	sdc_miso,	sdc_mosi,
	sdc_clk,	sdc_cs,

	dbg_Led1,
	dbg_Led2,
	dbg_Led3,
	dbg_Led4,

	dbg_RgbR,
	dbg_RgbG,
	dbg_RgbB

//	dbg_exHold1,
//	dbg_exHold2,
//	dbg_outStatus1,
//	dbg_outStatus2,
//	dbg_outStatus3,
//	dbg_outStatus4,
//	dbg_outStatus5,
//	dbg_outStatus6,
//	dbg_outStatus7,
//	dbg_outStatus8
	);

input			clock;
input			reset;
// inout[31:0]		gpioPins;
inout[15:0]		gpioPins;
// inout[15:0]		fixedPins;

output[7:0]		pwmPins;

wire	reset2_200;
wire	reset2_150;
wire	reset2_100;
wire	reset2_75;
wire	reset2_50;

// assign	reset2 = !reset;

wire	reset2a;
assign	reset2a = !reset;

output			uartTxD;
input			uartRxD;
// input			uartCtS;
// output			uartRtS;

wire			uartCtS;
wire			uartRtS;
assign			uartCtS = 1;

// inout[3:0]		sdc_dat;
input			sdc_miso;
output			sdc_clk;
output			sdc_mosi;
output			sdc_cs;

// output			dbg_exHold1;
// output			dbg_exHold2;

// output			dbg_outStatus1;
// output			dbg_outStatus2;
// output			dbg_outStatus3;
// output			dbg_outStatus4;
// output			dbg_outStatus5;
// output			dbg_outStatus6;
// output			dbg_outStatus7;
// output			dbg_outStatus8;

output			dbg_Led1;
output			dbg_Led2;
output			dbg_Led3;
output			dbg_Led4;

output			dbg_RgbR;
output			dbg_RgbG;
output			dbg_RgbB;

wire			dbg_exHold1;
wire			dbg_exHold2;

wire			dbg_outStatus1;
wire			dbg_outStatus2;
wire			dbg_outStatus3;
wire			dbg_outStatus4;
wire			dbg_outStatus5;
wire			dbg_outStatus6;
wire			dbg_outStatus7;
wire			dbg_outStatus8;

reg			dbg_LedDuty;
// reg			dbg_LedDuty1;
// reg			dbg_LedDuty2;

assign		dbg_Led1 = dbg_exHold1 && dbg_LedDuty;
assign		dbg_Led2 = dbg_exHold2 && dbg_LedDuty;
assign		dbg_Led3 = dbg_outStatus1 && dbg_LedDuty;
assign		dbg_Led4 = dbg_outStatus2 && dbg_LedDuty;

assign		dbg_RgbR = !(dbg_outStatus5 && dbg_LedDuty);
assign		dbg_RgbG = !(dbg_outStatus6 && dbg_LedDuty);
assign		dbg_RgbB = !(dbg_outStatus8 && dbg_LedDuty);


wire[3:0]		sdc_dat_i;
wire[3:0]		sdc_dat_o;
wire[3:0]		sdc_dat_d;
wire			sdc_ena;

// assign	sdc_dat[0]		= sdc_dat_d[0] ? sdc_dat_o[0] : 1'bz;
// assign	sdc_dat[1]		= sdc_dat_d[1] ? sdc_dat_o[1] : 1'bz;
// assign	sdc_dat[2]		= sdc_dat_d[2] ? sdc_dat_o[2] : 1'bz;
// assign	sdc_dat[3]		= sdc_dat_d[3] ? sdc_dat_o[3] : 1'bz;
// assign	sdc_dat_i[0]	= sdc_dat[0];
assign	sdc_dat_i[0]		= sdc_miso;
// assign	sdc_dat_i[1]	= sdc_dat[1];
// assign	sdc_dat_i[2]	= sdc_dat[2];
// assign	sdc_dat_i[3]	= sdc_dat[3];
assign		sdc_cs = sdc_dat_o[3];

wire[31:0]		gpioPinsIn;
wire[31:0]		gpioPinsOut;
wire[31:0]		gpioPinsDir;

assign		gpioPins[ 0] = gpioPinsDir[ 0] ? gpioPinsOut[ 0] : 1'bz;
assign		gpioPins[ 1] = gpioPinsDir[ 1] ? gpioPinsOut[ 1] : 1'bz;
assign		gpioPins[ 2] = gpioPinsDir[ 2] ? gpioPinsOut[ 2] : 1'bz;
assign		gpioPins[ 3] = gpioPinsDir[ 3] ? gpioPinsOut[ 3] : 1'bz;
assign		gpioPins[ 4] = gpioPinsDir[ 4] ? gpioPinsOut[ 4] : 1'bz;
assign		gpioPins[ 5] = gpioPinsDir[ 5] ? gpioPinsOut[ 5] : 1'bz;
assign		gpioPins[ 6] = gpioPinsDir[ 6] ? gpioPinsOut[ 6] : 1'bz;
assign		gpioPins[ 7] = gpioPinsDir[ 7] ? gpioPinsOut[ 7] : 1'bz;
assign		gpioPins[ 8] = gpioPinsDir[ 8] ? gpioPinsOut[ 8] : 1'bz;
assign		gpioPins[ 9] = gpioPinsDir[ 9] ? gpioPinsOut[ 9] : 1'bz;
assign		gpioPins[10] = gpioPinsDir[10] ? gpioPinsOut[10] : 1'bz;
assign		gpioPins[11] = gpioPinsDir[11] ? gpioPinsOut[11] : 1'bz;
assign		gpioPins[12] = gpioPinsDir[12] ? gpioPinsOut[12] : 1'bz;
assign		gpioPins[13] = gpioPinsDir[13] ? gpioPinsOut[13] : 1'bz;
assign		gpioPins[14] = gpioPinsDir[14] ? gpioPinsOut[14] : 1'bz;
assign		gpioPins[15] = gpioPinsDir[15] ? gpioPinsOut[15] : 1'bz;

assign		gpioPinsIn[ 0] = gpioPins[ 0];
assign		gpioPinsIn[ 1] = gpioPins[ 1];
assign		gpioPinsIn[ 2] = gpioPins[ 2];
assign		gpioPinsIn[ 3] = gpioPins[ 3];
assign		gpioPinsIn[ 4] = gpioPins[ 4];
assign		gpioPinsIn[ 5] = gpioPins[ 5];
assign		gpioPinsIn[ 6] = gpioPins[ 6];
assign		gpioPinsIn[ 7] = gpioPins[ 7];
assign		gpioPinsIn[ 8] = gpioPins[ 8];
assign		gpioPinsIn[ 9] = gpioPins[ 9];
assign		gpioPinsIn[10] = gpioPins[10];
assign		gpioPinsIn[11] = gpioPins[11];
assign		gpioPinsIn[12] = gpioPins[12];
assign		gpioPinsIn[13] = gpioPins[13];
assign		gpioPinsIn[14] = gpioPins[14];
assign		gpioPinsIn[15] = gpioPins[15];

assign		gpioPinsIn[31:16] = 0;


wire	clock_master;

wire	clock_300mhz;
wire	clock_200mhz;
wire	clock_150mhz;
wire	clock_100mhz;
wire	clock_75mhz;
wire	clock_50mhz;

reg[15:0]	regInitSanity;
reg			reset_sanity;

reg			reset3_300;
reg			reset3_200;
reg			reset3_150;
reg			reset3_100;
reg			reset3_75;
reg			reset3_50;

// assign		reset_sanity = (regInitSanity!=8'h55);
// assign		reset2 = reset || reset_sanity;


CoreUnitUc core(
//	clock, 		reset2,
	clock_300mhz,
	clock_200mhz,
	clock_150mhz,
	clock_100mhz,
	clock_75mhz,
	clock_50mhz,

	reset2_300,
	reset2_200,
	reset2_150,
	reset2_100,
	reset2_75,
	reset2_50,

	uartTxD,	uartRxD,
	uartCtS,	uartRtS,

	sdc_dat_i,	sdc_dat_o,	sdc_dat_d,
	sdc_clk,	sdc_mosi,	sdc_ena,

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
	gpioPinsDir,
	
	pwmPins
	);

// reg			dbg_LedDuty;
reg			dbg_LedDuty1;
reg			dbg_LedDuty2;
reg			dbg_LedDuty3;
reg			dbg_LedDuty4;

always @(posedge clock_50mhz)
begin
	dbg_LedDuty1	<= !dbg_LedDuty4;
	dbg_LedDuty2	<= dbg_LedDuty1;
	dbg_LedDuty3	<= dbg_LedDuty2;
	dbg_LedDuty4	<= dbg_LedDuty3;
	dbg_LedDuty		<= dbg_LedDuty1 && dbg_LedDuty3;
end

`ifdef def_true
wire	sys_clk;

wire	gen_clk_300mhz;
wire	gen_clk_200mhz;
wire	gen_clk_150mhz;
wire	gen_clk_100mhz;
wire	gen_clk_75mhz;
wire	gen_clk_50mhz;
wire	gen_clk_25mhz;

// wire	gen_clk_66mhz;

wire	clk_feedback, clk_locked;
wire	gen_clk_300mhz_nobuf;
wire	gen_clk_200mhz_nobuf;
wire	gen_clk_150mhz_nobuf;
wire	gen_clk_100mhz_nobuf;
wire	gen_clk_75mhz_nobuf;
wire	gen_clk_50mhz_nobuf;

`ifdef def_true
PLLE2_BASE	#(
	.BANDWIDTH("OPTIMIZED"),	// OPTIMIZED, HIGH, LOW
	.CLKFBOUT_PHASE(0.0),		// Phase offset in degrees of CLKFB, (-360-360)
	.CLKIN1_PERIOD(10.0),		// Input clock period in ns resolution
	// CLKOUT0_DIVIDE - CLKOUT5_DIVIDE: divide amount for each CLKOUT(1-128)
	.CLKFBOUT_MULT(12),			// Multiply value for all CLKOUT (2-64)
	.CLKOUT0_DIVIDE(6),			// 200 MHz
	.CLKOUT1_DIVIDE(8),			// 150 MHz
	.CLKOUT2_DIVIDE(12),		// 100 MHz
	.CLKOUT3_DIVIDE(16),		//  75 MHz
	.CLKOUT4_DIVIDE(24),		//  50 MHz
//	.CLKOUT5_DIVIDE(48),		//  25 MHz
	.CLKOUT5_DIVIDE(4),			//  300 MHz
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
	.CLKOUT0(gen_clk_200mhz_nobuf),
	.CLKOUT1(gen_clk_150mhz_nobuf),
	.CLKOUT2(gen_clk_100mhz_nobuf),
	.CLKOUT3(gen_clk_75mhz_nobuf),
	.CLKOUT4(gen_clk_50mhz_nobuf),
//	.CLKOUT5(gen_clk_25mhz_nobuf),
	.CLKOUT5(gen_clk_300mhz_nobuf),
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

// IBUF	sysclk_buf(.I(clock), .O(sys_clk));
assign	sys_clk = clock_master;

BUFG	clk300_buf(.I(gen_clk_300mhz_nobuf), .O(gen_clk_300mhz));
BUFG	clk200_buf(.I(gen_clk_200mhz_nobuf), .O(gen_clk_200mhz));
BUFG	clk150_buf(.I(gen_clk_150mhz_nobuf), .O(gen_clk_150mhz));
BUFG	clk100_buf(.I(gen_clk_100mhz_nobuf), .O(gen_clk_100mhz));
BUFG	clk75_buf(.I(gen_clk_75mhz_nobuf), .O(gen_clk_75mhz));
BUFG	clk50_buf(.I(gen_clk_50mhz_nobuf), .O(gen_clk_50mhz));

assign	clock_300mhz = gen_clk_300mhz;
assign	clock_200mhz = gen_clk_200mhz;
assign	clock_150mhz = gen_clk_150mhz;
assign	clock_100mhz = gen_clk_100mhz;
assign	clock_75mhz = gen_clk_75mhz;
assign	clock_50mhz = gen_clk_50mhz;

`endif


initial begin
	regInitSanity = 0;
end

always @(posedge clock_300mhz)
begin
	reset3_300		<= reset_sanity;
end
always @(posedge clock_200mhz)
begin
	reset3_200		<= reset_sanity;
end
always @(posedge clock_150mhz)
begin
	reset3_150		<= reset_sanity;
end
always @(posedge clock_100mhz)
begin
	reset3_100		<= reset_sanity;
end

always @(posedge clock_75mhz)
begin
	reset3_75		<= reset_sanity;
end

always @(posedge clock_50mhz)
begin
	reset3_50		<= reset_sanity;
end

`ifdef jx2_cpu_mmioclock_75
always @(posedge clock_75mhz)
`else
always @(posedge clock_50mhz)
`endif
begin
	reset_sanity 	<= (regInitSanity!=16'hAA55);

	if(reset2a)
	begin
		regInitSanity	<= 0;
	end
	else
	begin
		if(regInitSanity != 16'hAA55)
			regInitSanity	<= regInitSanity + 1;
	end
end

BUFG	reset2_300_buf(.I(reset3_300), .O(reset2_300));
BUFG	reset2_200_buf(.I(reset3_200), .O(reset2_200));
BUFG	reset2_150_buf(.I(reset3_150), .O(reset2_150));
BUFG	reset2_100_buf(.I(reset3_100), .O(reset2_100));

BUFG	reset2_75_buf(.I(reset3_75), .O(reset2_75));
BUFG	reset2_50_buf(.I(reset3_50), .O(reset2_50));


wire clkfb;

MMCME2_BASE #(
      .BANDWIDTH("OPTIMIZED"),	// Jitter programming (OPTIMIZED, HIGH, LOW)
      .CLKFBOUT_MULT_F(60.0),	// Multiply value for all CLKOUT (2.000-64.000).
      .CLKFBOUT_PHASE(0.0),		// Phase offset in degrees of CLKFB (-360.000-360.000).
      .CLKIN1_PERIOD(83.3333),	// Input clock period in ns to ps resolution (i.e. 33.333 is 30 MHz).
      // CLKOUT0_DIVIDE - CLKOUT6_DIVIDE: Divide amount for each CLKOUT (1-128)
      .CLKOUT0_DIVIDE_F(7.2),    // Divide amount for CLKOUT0 (1.000-128.000).
      .CLKOUT1_DIVIDE(6),
      .CLKOUT2_DIVIDE(6),
      .CLKOUT3_DIVIDE(6),
      .CLKOUT4_DIVIDE(6),
      .CLKOUT5_DIVIDE(6),
      .CLKOUT6_DIVIDE(6),
      // CLKOUT0_DUTY_CYCLE - CLKOUT6_DUTY_CYCLE: Duty cycle for each CLKOUT (0.01-0.99).
      .CLKOUT0_DUTY_CYCLE(0.5),
      .CLKOUT1_DUTY_CYCLE(0.5),
      .CLKOUT2_DUTY_CYCLE(0.5),
      .CLKOUT3_DUTY_CYCLE(0.5),
      .CLKOUT4_DUTY_CYCLE(0.5),
      .CLKOUT5_DUTY_CYCLE(0.5),
      .CLKOUT6_DUTY_CYCLE(0.5),
      // CLKOUT0_PHASE - CLKOUT6_PHASE: Phase offset for each CLKOUT (-360.000-360.000).
      .CLKOUT0_PHASE(0.0),
      .CLKOUT1_PHASE(0.0),
      .CLKOUT2_PHASE(0.0),
      .CLKOUT3_PHASE(0.0),
      .CLKOUT4_PHASE(0.0),
      .CLKOUT5_PHASE(0.0),
      .CLKOUT6_PHASE(0.0),
      .CLKOUT4_CASCADE("FALSE"), // Cascade CLKOUT4 counter with CLKOUT6 (FALSE, TRUE)
      .DIVCLK_DIVIDE(1),         // Master division value (1-106)
      .REF_JITTER1(0.0),         // Reference input jitter in UI (0.000-0.999).
      .STARTUP_WAIT("FALSE")     // Delays DONE until MMCM is locked (FALSE, TRUE)
   )
MMCME2_BASE_inst (
      // Clock Outputs: 1-bit (each) output: User configurable clock outputs
      .CLKOUT0(clock_master),	// 1-bit output: CLKOUT0
      .CLKOUT0B(CLKOUT0B),		// 1-bit output: Inverted CLKOUT0
      .CLKOUT1(CLKOUT1),		// 1-bit output: CLKOUT1
      .CLKOUT1B(CLKOUT1B),   // 1-bit output: Inverted CLKOUT1
      .CLKOUT2(CLKOUT2),     // 1-bit output: CLKOUT2
      .CLKOUT2B(CLKOUT2B),   // 1-bit output: Inverted CLKOUT2
      .CLKOUT3(CLKOUT3),     // 1-bit output: CLKOUT3
      .CLKOUT3B(CLKOUT3B),   // 1-bit output: Inverted CLKOUT3
      .CLKOUT4(CLKOUT4),     // 1-bit output: CLKOUT4
      .CLKOUT5(CLKOUT5),     // 1-bit output: CLKOUT5
      .CLKOUT6(CLKOUT6),     // 1-bit output: CLKOUT6
      // Feedback Clocks: 1-bit (each) output: Clock feedback ports
      .CLKFBOUT(clkfb),   // 1-bit output: Feedback clock
      .CLKFBOUTB(CLKFBOUTB), // 1-bit output: Inverted CLKFBOUT
      // Status Ports: 1-bit (each) output: MMCM status ports
      .LOCKED(LOCKED),		// 1-bit output: LOCK
      // Clock Inputs: 1-bit (each) input: Clock input
      .CLKIN1(clock),		// 1-bit input: Clock
      // Control Ports: 1-bit (each) input: MMCM control ports
      .PWRDWN(1'b0),       // 1-bit input: Power-down
      .RST(1'b0),             // 1-bit input: Reset
      // Feedback Clocks: 1-bit (each) input: Clock feedback ports
      .CLKFBIN(clkfb)      // 1-bit input: Feedback clock
   );

endmodule

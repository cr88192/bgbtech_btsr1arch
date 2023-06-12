/*
Camera module input with 8-bit CPI.
 */

`include "CoreDefs.v"

module ModCamCpi8b(
	/* verilator lint_off UNUSED */
	clock,			reset,

	in_data,		in_clk,
	in_hsync,		in_vsync,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioSelfAddr
	);

input			clock;
input			reset;

input[7:0]		in_data;
input			in_clk;
input			in_hsync;
input			in_vsync;

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;
input[11:0]		mmioSelfAddr;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (tMmioAddr[15:4]==mmioSelfAddr);

reg[7:0]		tOutPwm;
reg[7:0]		tOutPwm2;
assign		out_pwm4x = tOutPwm2;

reg				mmioInOE;
reg				mmioInWR;

reg				mmioLatchWR;
reg				mmioNxtLatchWR;


endmodule

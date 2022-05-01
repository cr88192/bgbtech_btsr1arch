`include "ExBcdAdd4.v"

module ExBcdAdd64(a, b, c, c_in, c_out, mode);
input [63:0]		a;
input [63:0]		b;
output[63:0]		c;
input			c_in;
output			c_out;
input			mode;

wire[63:0]	tBinv;
wire[63:0]	tBi;
wire		tCi;

assign tBinv =
{	4'h9-b[63:60], 4'h9-b[59:56],
	4'h9-b[55:52], 4'h9-b[51:48],
	4'h9-b[47:44], 4'h9-b[43:40],
	4'h9-b[39:36], 4'h9-b[35:32],
	4'h9-b[31:28], 4'h9-b[27:24],
	4'h9-b[23:20], 4'h9-b[19:16],
	4'h9-b[15:12], 4'h9-b[11: 8],
	4'h9-b[ 7: 4], 4'h9-b[ 3: 0]	};

assign	tBi = mode ? tBinv : b;
assign	tCi = mode ? !c_in : c_in;

wire[63:0]	tCa;
wire[63:0]	tCa0;
wire[63:0]	tCa1;
// wire[4:0]	tCc;

// reg[63:0]	tC;
// reg			tCo;
// assign		c = tC;
// assign		c_out = tCo;

wire	tCarry01a;
wire	tCarry02a;
wire	tCarry03a;
wire	tCarry04a;
wire	tCarry05a;
wire	tCarry06a;
wire	tCarry07a;
wire	tCarry08a;
wire	tCarry09a;
wire	tCarry10a;
wire	tCarry11a;
wire	tCarry12a;
wire	tCarry13a;
wire	tCarry14a;
wire	tCarry15a;
wire	tCarry16a;

wire	tCarry01b;
wire	tCarry02b;
wire	tCarry03b;
wire	tCarry04b;
wire	tCarry05b;
wire	tCarry06b;
wire	tCarry07b;
wire	tCarry08b;
wire	tCarry09b;
wire	tCarry10b;
wire	tCarry11b;
wire	tCarry12b;
wire	tCarry13b;
wire	tCarry14b;
wire	tCarry15b;
wire	tCarry16b;

`ifndef def_true
ExBcdAdd4	add01a(a[ 3: 0], tBi[ 3: 0], tCa0[ 3: 0], 1'b0     , tCarry01a);
ExBcdAdd4	add02a(a[ 7: 4], tBi[ 7: 4], tCa0[ 7: 4], tCarry01a, tCarry02a);
ExBcdAdd4	add03a(a[11: 8], tBi[11: 8], tCa0[11: 8], tCarry02a, tCarry03a);
ExBcdAdd4	add04a(a[15:12], tBi[15:12], tCa0[15:12], tCarry03a, tCarry04a);

ExBcdAdd4	add01b(a[ 3: 0], tBi[ 3: 0], tCa1[ 3: 0], 1'b1     , tCarry01b);
ExBcdAdd4	add02b(a[ 7: 4], tBi[ 7: 4], tCa1[ 7: 4], tCarry01b, tCarry02b);
ExBcdAdd4	add03b(a[11: 8], tBi[11: 8], tCa1[11: 8], tCarry02b, tCarry03b);
ExBcdAdd4	add04b(a[15:12], tBi[15:12], tCa1[15:12], tCarry03b, tCarry04b);

ExBcdAdd4	add05a(a[19:16], tBi[19:16], tCa0[19:16], 1'b0     , tCarry05a);
ExBcdAdd4	add06a(a[23:20], tBi[23:20], tCa0[23:20], tCarry05a, tCarry06a);
ExBcdAdd4	add07a(a[27:24], tBi[27:24], tCa0[27:24], tCarry06a, tCarry07a);
ExBcdAdd4	add08a(a[31:28], tBi[31:28], tCa0[31:28], tCarry07a, tCarry08a);

ExBcdAdd4	add05b(a[19:16], tBi[19:16], tCa1[19:16], 1'b1     , tCarry05b);
ExBcdAdd4	add06b(a[23:20], tBi[23:20], tCa1[23:20], tCarry05b, tCarry06b);
ExBcdAdd4	add07b(a[27:24], tBi[27:24], tCa1[27:24], tCarry06b, tCarry07b);
ExBcdAdd4	add08b(a[31:28], tBi[31:28], tCa1[31:28], tCarry07b, tCarry08b);

ExBcdAdd4	add09a(a[35:32], tBi[35:32], tCa0[35:32], 1'b0     , tCarry09a);
ExBcdAdd4	add10a(a[39:36], tBi[39:36], tCa0[39:36], tCarry09a, tCarry10a);
ExBcdAdd4	add11a(a[43:40], tBi[43:40], tCa0[43:40], tCarry10a, tCarry11a);
ExBcdAdd4	add12a(a[47:44], tBi[47:44], tCa0[47:44], tCarry11a, tCarry12a);

ExBcdAdd4	add09b(a[35:32], tBi[35:32], tCa1[35:32], 1'b1     , tCarry09b);
ExBcdAdd4	add10b(a[39:36], tBi[39:36], tCa1[39:36], tCarry09b, tCarry10b);
ExBcdAdd4	add11b(a[43:40], tBi[43:40], tCa1[43:40], tCarry10b, tCarry11b);
ExBcdAdd4	add12b(a[47:44], tBi[47:44], tCa1[47:44], tCarry11b, tCarry12b);

ExBcdAdd4	add13a(a[51:48], tBi[51:48], tCa0[51:48], 1'b0     , tCarry13a);
ExBcdAdd4	add14a(a[55:52], tBi[55:52], tCa0[55:52], tCarry13a, tCarry14a);
ExBcdAdd4	add15a(a[59:56], tBi[59:56], tCa0[59:56], tCarry14a, tCarry15a);
ExBcdAdd4	add16a(a[63:60], tBi[63:60], tCa0[63:60], tCarry15a, tCarry16a);

ExBcdAdd4	add13b(a[51:48], tBi[51:48], tCa1[51:48], 1'b1     , tCarry13b);
ExBcdAdd4	add14b(a[55:52], tBi[55:52], tCa1[55:52], tCarry13b, tCarry14b);
ExBcdAdd4	add15b(a[59:56], tBi[59:56], tCa1[59:56], tCarry14b, tCarry15b);
ExBcdAdd4	add16b(a[63:60], tBi[63:60], tCa1[63:60], tCarry15b, tCarry16b);

wire tCc0 = tCi  ? tCarry04b : tCarry04a;
wire tCc1 = tCc0 ? tCarry08b : tCarry08a;
wire tCc2 = tCc1 ? tCarry12b : tCarry12a;
wire tCc3 = tCc2 ? tCarry16b : tCarry16a;

assign	tCa[15: 0] = tCi  ? tCa1[15: 0] : tCa0[15: 0];
assign	tCa[31:16] = tCc0 ? tCa1[31:16] : tCa0[31:16];
assign	tCa[47:32] = tCc1 ? tCa1[47:32] : tCa0[47:32];
assign	tCa[63:48] = tCc2 ? tCa1[63:48] : tCa0[63:48];

assign		c = tCa;
assign		c_out = tCc3;

`endif


`ifdef def_true
ExBcdAdd4	add01a(a[ 3: 0], tBi[ 3: 0], tCa0[ 3: 0], 1'b0     , tCarry01a);
ExBcdAdd4	add02a(a[ 7: 4], tBi[ 7: 4], tCa0[ 7: 4], tCarry01a, tCarry02a);
ExBcdAdd4	add01b(a[ 3: 0], tBi[ 3: 0], tCa1[ 3: 0], 1'b1     , tCarry01b);
ExBcdAdd4	add02b(a[ 7: 4], tBi[ 7: 4], tCa1[ 7: 4], tCarry01b, tCarry02b);

ExBcdAdd4	add03a(a[11: 8], tBi[11: 8], tCa0[11: 8], 1'b0     , tCarry03a);
ExBcdAdd4	add04a(a[15:12], tBi[15:12], tCa0[15:12], tCarry03a, tCarry04a);
ExBcdAdd4	add03b(a[11: 8], tBi[11: 8], tCa1[11: 8], 1'b1     , tCarry03b);
ExBcdAdd4	add04b(a[15:12], tBi[15:12], tCa1[15:12], tCarry03b, tCarry04b);

ExBcdAdd4	add05a(a[19:16], tBi[19:16], tCa0[19:16], 1'b0     , tCarry05a);
ExBcdAdd4	add06a(a[23:20], tBi[23:20], tCa0[23:20], tCarry05a, tCarry06a);
ExBcdAdd4	add05b(a[19:16], tBi[19:16], tCa1[19:16], 1'b1     , tCarry05b);
ExBcdAdd4	add06b(a[23:20], tBi[23:20], tCa1[23:20], tCarry05b, tCarry06b);

ExBcdAdd4	add07a(a[27:24], tBi[27:24], tCa0[27:24], 1'b0     , tCarry07a);
ExBcdAdd4	add08a(a[31:28], tBi[31:28], tCa0[31:28], tCarry07a, tCarry08a);
ExBcdAdd4	add07b(a[27:24], tBi[27:24], tCa1[27:24], 1'b1     , tCarry07b);
ExBcdAdd4	add08b(a[31:28], tBi[31:28], tCa1[31:28], tCarry07b, tCarry08b);

ExBcdAdd4	add09a(a[35:32], tBi[35:32], tCa0[35:32], 1'b0     , tCarry09a);
ExBcdAdd4	add10a(a[39:36], tBi[39:36], tCa0[39:36], tCarry09a, tCarry10a);
ExBcdAdd4	add09b(a[35:32], tBi[35:32], tCa1[35:32], 1'b1     , tCarry09b);
ExBcdAdd4	add10b(a[39:36], tBi[39:36], tCa1[39:36], tCarry09b, tCarry10b);

ExBcdAdd4	add11a(a[43:40], tBi[43:40], tCa0[43:40], 1'b0     , tCarry11a);
ExBcdAdd4	add12a(a[47:44], tBi[47:44], tCa0[47:44], tCarry11a, tCarry12a);
ExBcdAdd4	add11b(a[43:40], tBi[43:40], tCa1[43:40], 1'b1     , tCarry11b);
ExBcdAdd4	add12b(a[47:44], tBi[47:44], tCa1[47:44], tCarry11b, tCarry12b);

ExBcdAdd4	add13a(a[51:48], tBi[51:48], tCa0[51:48], 1'b0     , tCarry13a);
ExBcdAdd4	add14a(a[55:52], tBi[55:52], tCa0[55:52], tCarry13a, tCarry14a);
ExBcdAdd4	add13b(a[51:48], tBi[51:48], tCa1[51:48], 1'b1     , tCarry13b);
ExBcdAdd4	add14b(a[55:52], tBi[55:52], tCa1[55:52], tCarry13b, tCarry14b);

ExBcdAdd4	add15a(a[59:56], tBi[59:56], tCa0[59:56], 1'b0     , tCarry15a);
ExBcdAdd4	add16a(a[63:60], tBi[63:60], tCa0[63:60], tCarry15a, tCarry16a);
ExBcdAdd4	add15b(a[59:56], tBi[59:56], tCa1[59:56], 1'b1     , tCarry15b);
ExBcdAdd4	add16b(a[63:60], tBi[63:60], tCa1[63:60], tCarry15b, tCarry16b);

`ifndef def_true
wire tCc0 = tCi  ? tCarry02b : tCarry02a;
wire tCc1 = tCc0 ? tCarry04b : tCarry04a;
wire tCc2 = tCc1 ? tCarry06b : tCarry06a;
wire tCc3 = tCc2 ? tCarry08b : tCarry08a;
wire tCc4 = tCc3 ? tCarry10b : tCarry10a;
wire tCc5 = tCc4 ? tCarry12b : tCarry12a;
wire tCc6 = tCc5 ? tCarry14b : tCarry14a;
wire tCc7 = tCc6 ? tCarry16b : tCarry16a;

assign	tCa[ 7: 0] = tCi  ? tCa1[ 7: 0] : tCa0[ 7: 0];
assign	tCa[15: 8] = tCc0 ? tCa1[15: 8] : tCa0[15: 8];
assign	tCa[23:16] = tCc1 ? tCa1[23:16] : tCa0[23:16];
assign	tCa[31:24] = tCc2 ? tCa1[31:24] : tCa0[31:24];
assign	tCa[39:32] = tCc3 ? tCa1[39:32] : tCa0[39:32];
assign	tCa[47:40] = tCc4 ? tCa1[47:40] : tCa0[47:40];
assign	tCa[55:48] = tCc5 ? tCa1[55:48] : tCa0[55:48];
assign	tCa[63:56] = tCc6 ? tCa1[63:56] : tCa0[63:56];
`endif

`ifdef def_true
//wire[7:0] tCc0;
//wire[7:0] tCc1;
// wire[7:0] tCc;

wire tCc0_0 = 1'b0   ? tCarry02b : tCarry02a;
wire tCc0_1 = tCc0_0 ? tCarry04b : tCarry04a;
wire tCc0_2 = tCc0_1 ? tCarry06b : tCarry06a;
wire tCc0_3 = tCc0_2 ? tCarry08b : tCarry08a;
wire tCc0_4 = tCc0_3 ? tCarry10b : tCarry10a;
wire tCc0_5 = tCc0_4 ? tCarry12b : tCarry12a;
wire tCc0_6 = tCc0_5 ? tCarry14b : tCarry14a;
wire tCc0_7 = tCc0_6 ? tCarry16b : tCarry16a;

wire tCc1_0 = 1'b1   ? tCarry02b : tCarry02a;
wire tCc1_1 = tCc1_0 ? tCarry04b : tCarry04a;
wire tCc1_2 = tCc1_1 ? tCarry06b : tCarry06a;
wire tCc1_3 = tCc1_2 ? tCarry08b : tCarry08a;
wire tCc1_4 = tCc1_3 ? tCarry10b : tCarry10a;
wire tCc1_5 = tCc1_4 ? tCarry12b : tCarry12a;
wire tCc1_6 = tCc1_5 ? tCarry14b : tCarry14a;
wire tCc1_7 = tCc1_6 ? tCarry16b : tCarry16a;

wire tCc_0 = tCi ? tCc1_0 : tCc0_0;
wire tCc_1 = tCi ? tCc1_1 : tCc0_1;
wire tCc_2 = tCi ? tCc1_2 : tCc0_2;
wire tCc_3 = tCi ? tCc1_3 : tCc0_3;
wire tCc_4 = tCi ? tCc1_4 : tCc0_4;
wire tCc_5 = tCi ? tCc1_5 : tCc0_5;
wire tCc_6 = tCi ? tCc1_6 : tCc0_6;
wire tCc_7 = tCi ? tCc1_7 : tCc0_7;

assign	tCa[ 7: 0] = tCi   ? tCa1[ 7: 0] : tCa0[ 7: 0];
assign	tCa[15: 8] = tCc_0 ? tCa1[15: 8] : tCa0[15: 8];
assign	tCa[23:16] = tCc_1 ? tCa1[23:16] : tCa0[23:16];
assign	tCa[31:24] = tCc_2 ? tCa1[31:24] : tCa0[31:24];
assign	tCa[39:32] = tCc_3 ? tCa1[39:32] : tCa0[39:32];
assign	tCa[47:40] = tCc_4 ? tCa1[47:40] : tCa0[47:40];
assign	tCa[55:48] = tCc_5 ? tCa1[55:48] : tCa0[55:48];
assign	tCa[63:56] = tCc_6 ? tCa1[63:56] : tCa0[63:56];
`endif

assign		c = tCa;
assign		c_out = tCc_7;

`endif


endmodule


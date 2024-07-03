/*
Binary Neural Net

ValRs:
  Will be 16x 1-bit inputs in 1-bit mode,
  or 16x 2b in 2-bit mode.

ValRt:
  (47: 0), Weights, 3 bits each
  (55:48), Bias, 8-bits
  (57:56), Mode
  (63:58), Reserved

Modes:
  00: 1-bit in, 1-bit out
  01: 2-bit in, 1 bit out
  10: 1-bit in, 2 bit out
  11: 2-bit in, 2-bit out

In 1-bit input mode:
  0 => -1, 1 => 1

Output will shift the 3rd input to the left and add the new output into the low-order bit(s).

 */

`include "ExBnn_BitNeur_Mul2x.v"

module ExBnn_BitNeur(
	clock,	reset,
	exHold,	idUIxt,
	valRs, valRt, valRnI, valRnO);

input			clock;
input			reset;

input			exHold;
input[8:0]		idUIxt;

input[63:0]		valRs;
input[63:0]		valRt;
input[63:0]		valRnI;
output[63:0]	valRnO;

reg[31:0]		tValRs;
reg[63:0]		tValRt;

reg[63:0]		tValRnI;
reg[63:0]		tValRnO;
reg[63:0]		tValRnO2;

// assign	tValRnI = valRnI;
assign	valRnO = tValRnO2;

wire[1:0]		tModeI;
assign		tModeI = valRt[57:56];

wire[7:0]		tBias;
wire[3:0]		tMode;

assign		tBias = tValRt[55:48];
assign		tMode = tValRt[59:56];

wire[31:0]		tValRs1b;
assign 		tValRs1b = {
	!valRs[15], 1'b1,	!valRs[14], 1'b1,
	!valRs[13], 1'b1,	!valRs[12], 1'b1,
	!valRs[11], 1'b1,	!valRs[10], 1'b1,
	!valRs[ 9], 1'b1,	!valRs[ 8], 1'b1,
	!valRs[ 7], 1'b1,	!valRs[ 6], 1'b1,
	!valRs[ 5], 1'b1,	!valRs[ 4], 1'b1,
	!valRs[ 3], 1'b1,	!valRs[ 2], 1'b1,
	!valRs[ 1], 1'b1,	!valRs[ 0], 1'b1
};

wire[31:0]		tNxtValRs;
assign	tNxtValRs = tModeI[0] ? valRs[31:0] : tValRs1b;

wire[4:0]	tMul0a;
wire[4:0]	tMul0b;
wire[4:0]	tMul0c;
wire[4:0]	tMul0d;

wire[4:0]	tMul0e;
wire[4:0]	tMul0f;
wire[4:0]	tMul0g;
wire[4:0]	tMul0h;
ExBnn_BitNeur_Mul2x	mul0a(tValRs[ 3: 0], tValRt[ 5: 0], tMul0a);
ExBnn_BitNeur_Mul2x	mul0b(tValRs[ 7: 4], tValRt[11: 6], tMul0b);
ExBnn_BitNeur_Mul2x	mul0c(tValRs[11: 8], tValRt[17:12], tMul0c);
ExBnn_BitNeur_Mul2x	mul0d(tValRs[15:12], tValRt[23:18], tMul0d);
ExBnn_BitNeur_Mul2x	mul0e(tValRs[19:16], tValRt[29:24], tMul0e);
ExBnn_BitNeur_Mul2x	mul0f(tValRs[23:20], tValRt[35:30], tMul0f);
ExBnn_BitNeur_Mul2x	mul0g(tValRs[27:24], tValRt[41:36], tMul0g);
ExBnn_BitNeur_Mul2x	mul0h(tValRs[31:28], tValRt[47:42], tMul0h);

reg[7:0]	tMul1a;
reg[7:0]	tMul1b;
reg[7:0]	tMul1c;
reg[7:0]	tMul1d;

reg[7:0]	tMul2a;
reg[7:0]	tMul2b;
reg[7:0]	tMul2c;

// reg[7:0]	tMul3a;
reg[8:0]	tMul3a;

reg			tBitOut;

always @*
begin
	tMul1a =
		{ tMul0a[4] ? 3'h7 : 3'h0, tMul0a } +
		{ tMul0b[4] ? 3'h7 : 3'h0, tMul0b } ;
	tMul1b =
		{ tMul0c[4] ? 3'h7 : 3'h0, tMul0c } +
		{ tMul0d[4] ? 3'h7 : 3'h0, tMul0d } ;
	tMul1c =
		{ tMul0e[4] ? 3'h7 : 3'h0, tMul0e } +
		{ tMul0f[4] ? 3'h7 : 3'h0, tMul0f } ;
	tMul1d =
		{ tMul0g[4] ? 3'h7 : 3'h0, tMul0g } +
		{ tMul0h[4] ? 3'h7 : 3'h0, tMul0h } ;
	
	tMul2a = tMul1a + tMul1b;
	tMul2b = tMul1c + tMul1d;
	tMul2c = tMul2a + tMul2b;

//	tMul3a = tMul2a + tMul2b + tBias;
	tMul3a = { tMul2c[7], tMul2c } + { tBias[7], tBias };
	
	tBitOut = !tMul3a[7];

	if(tMode[2])
		tBitOut = !tMul3a[8];
	
	if(tMode[1])
	begin
		tValRnO = { tValRnI[61:0], !tBitOut, 1'b1 };
	end
	else
	begin
		tValRnO = { tValRnI[62:0], tBitOut };
	end
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tValRs		<= tNxtValRs;
		tValRt		<= valRt;
		tValRnI		<= valRnI;

		tValRnO2	<= tValRnO;
	end
end

endmodule

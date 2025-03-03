// `include "ExCsShl64F.v"
`include "ExShad64D.v"
`include "ExCsMaskGen64.v"

module ExBit_BitMov(
	clock,	reset,
	exHold,	idUIxt,
	valRs, valRt, valRi, valRn);

input			clock;
input			reset;

input			exHold;
input[8:0]		idUIxt;

input[63:0]		valRs;
input[63:0]		valRt;
input[32:0]		valRi;
output[63:0]	valRn;

// reg[63:0]		tValRn;
// assign		valRn = tValRn;

wire[6:0]		valShlRs;
wire[6:0]		valShlRt;
wire[6:0]		valShlW;
wire[7:0]		valShlRtD;
wire[7:0]		valShlRsH;

assign		valShlRs	= valRi[ 6: 0];
assign		valShlRt	= valRi[14: 8];
assign		valShlW		= valRi[22:16];

assign		valShlRtD	= { 1'b0, valShlRs } - { 1'b0, valShlRt };
assign		valShlRsH	= { 1'b0, valShlRs } + { 1'b0, valShlW };

wire[63:0]		valRtShl;

ExShad64D	doShlRt(clock, reset, valRt, UV64_00,
	valShlRtD,
	valRtShl, 5'b00011, 0);

wire[63:0]		valMaskLo;
wire[63:0]		valMaskHi;
wire[63:0]		valMask;

ExCsMaskGen64	genMaskLo( { 1'b1, valShlRs [6:0] }, valMaskLo);
ExCsMaskGen64	genMaskHi( { 1'b0, valShlRsH[6:0] }, valMaskHi);

assign		valMask = valMaskLo & valMaskHi;

assign		valRn = (valRs & ~valMask) | (valRtShl & valMask);

endmodule

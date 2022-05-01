/*
  DAA/DAS on Packed BCD Digits
  
  mode=0: DAA (Adjust After ADD)
  mode=1: DAS (Adjust After SUB)
 */

`ifndef inc_ExMiscDaa64
`define inc_ExMiscDaa64

`include "ExMiscDaa4.v"

module ExMiscDaa64(inDigit, outDigit, inCarry, outCarry, inMode);
input[63:0]		inDigit;
input			inCarry;
input			inMode;

output[63:0]	outDigit;
output			outCarry;

wire[63:0]		tOut;

assign		outDigit	= tOut[63:0];


wire			tExDaa00C;
wire			tExDaa01C;
wire			tExDaa02C;
wire			tExDaa03C;
wire			tExDaa04C;
wire			tExDaa05C;
wire			tExDaa06C;
wire			tExDaa07C;
wire			tExDaa08C;
wire			tExDaa09C;
wire			tExDaa10C;
wire			tExDaa11C;
wire			tExDaa12C;
wire			tExDaa13C;
wire			tExDaa14C;
wire			tExDaa15C;

assign		outCarry	= tExDaa15C;


ExMiscDaa4	exDaa00(inDigit[ 3: 0], tOut[ 3: 0], inCarry  , tExDaa00C, inMode);
ExMiscDaa4	exDaa01(inDigit[ 7: 4], tOut[ 7: 4], tExDaa00C, tExDaa01C, inMode);
ExMiscDaa4	exDaa02(inDigit[11: 8], tOut[11: 8], tExDaa01C, tExDaa02C, inMode);
ExMiscDaa4	exDaa03(inDigit[15:12], tOut[15:12], tExDaa02C, tExDaa03C, inMode);
ExMiscDaa4	exDaa04(inDigit[19:16], tOut[19:16], tExDaa03C, tExDaa04C, inMode);
ExMiscDaa4	exDaa05(inDigit[23:20], tOut[23:20], tExDaa04C, tExDaa05C, inMode);
ExMiscDaa4	exDaa06(inDigit[27:24], tOut[27:24], tExDaa05C, tExDaa06C, inMode);
ExMiscDaa4	exDaa07(inDigit[31:28], tOut[31:28], tExDaa06C, tExDaa07C, inMode);
ExMiscDaa4	exDaa08(inDigit[35:32], tOut[35:32], tExDaa07C, tExDaa08C, inMode);
ExMiscDaa4	exDaa09(inDigit[39:36], tOut[39:36], tExDaa08C, tExDaa09C, inMode);
ExMiscDaa4	exDaa10(inDigit[43:40], tOut[43:40], tExDaa09C, tExDaa10C, inMode);
ExMiscDaa4	exDaa11(inDigit[47:44], tOut[47:44], tExDaa10C, tExDaa11C, inMode);
ExMiscDaa4	exDaa12(inDigit[51:48], tOut[51:48], tExDaa11C, tExDaa12C, inMode);
ExMiscDaa4	exDaa13(inDigit[55:52], tOut[55:52], tExDaa12C, tExDaa13C, inMode);
ExMiscDaa4	exDaa14(inDigit[59:56], tOut[59:56], tExDaa13C, tExDaa14C, inMode);
ExMiscDaa4	exDaa15(inDigit[63:60], tOut[63:60], tExDaa14C, tExDaa15C, inMode);

endmodule

`endif

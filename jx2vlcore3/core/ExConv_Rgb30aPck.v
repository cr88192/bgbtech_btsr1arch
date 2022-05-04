/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

`ifndef HAS_RGB30APCK
`define HAS_RGB30APCK

`include "ExConv_Fp8Exp12.v"

module ExConv_Rgb30aPck(
	/* verilator lint_off UNUSED */
	regValRs, regOutVal);

input[63:0]		regValRs;
output[31:0]	regOutVal;

// reg[31:0]	tRegOutVal;
// assign	regOutVal = tRegOutVal;

wire		tSignRgb;
wire		tSignRgba;
assign	tSignRgb = regValRs[15] || regValRs[31] || regValRs[47] ;
assign	tSignRgba = tSignRgb || regValRs[63] ;

wire[31:0]	tRegRgba30;
wire		tPadR, tPadB;
ExConv_Fp12Pck8		conv_pck32a(
	regValRs[15: 4], { tRegRgba30[ 6: 0], tPadB }, tSignRgba);
ExConv_Fp12Pck8		conv_pck32b(
	regValRs[31:20], { tRegRgba30[14: 7]        }, tSignRgba);
ExConv_Fp12Pck8		conv_pck32c(
	regValRs[47:36], { tRegRgba30[21:15], tPadR }, tSignRgba);
ExConv_Fp12Pck8		conv_pck32d(
	regValRs[63:52], { tRegRgba30[29:22]        }, tSignRgba);
assign	tRegRgba30[31:30] = { 1'b1, !tSignRgba };

wire[31:0]	tRegRgb30;
assign	tRegRgb30[31:30] = { 1'b0, !tSignRgb };
assign	tRegRgb30[29:20] = tSignRgb ? regValRs[47:38] : regValRs[46:37];
assign	tRegRgb30[19:10] = tSignRgb ? regValRs[31:22] : regValRs[30:21];
assign	tRegRgb30[ 9: 0] = tSignRgb ? regValRs[15: 6] : regValRs[14: 5];

wire		tHasAlpha;
assign	tHasAlpha = (regValRs[63:52] != 12'h3C0);

assign	regOutVal = tHasAlpha ? tRegRgba30 : tRegRgb30 ;

endmodule

`endif

/*
// SPDX-License-Identifier: CERN-OHL-P-2.0
// Copyright (c) 2018-2026 Brendan G Bohannon
//
// This source code is licensed under the CERN Open Hardware Licence 
// Strongly Reciprocal version 2 or later (CERN-OHL-P v2+).
//
// You may redistribute and modify this source code under the terms of 
// the CERN-OHL-P v2+. A copy of this license should be included with 
// this source code. If not, see <https://ohwr.org>.
//
// This source code is offered "as is" without any express or implied
// warranties. See the License for more details.
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

wire[7:0]	tRegPack8R;
wire[7:0]	tRegPack8G;
wire[7:0]	tRegPack8B;
wire[7:0]	tRegPack8A;

wire[31:0]	tRegRgba30;

ExConv_Fp12Pck8		conv_pck32a(
	regValRs[15: 4], tRegPack8B, 0);
ExConv_Fp12Pck8		conv_pck32b(
	regValRs[31:20], tRegPack8G, 0);
ExConv_Fp12Pck8		conv_pck32c(
	regValRs[47:36], tRegPack8R, 0);
ExConv_Fp12Pck8		conv_pck32d(
	regValRs[63:52], tRegPack8A, 0);

assign	tRegRgba30 = {
	1'b1, !tSignRgba,
	tRegPack8A[7:1], tSignRgba ? regValRs[63] : tRegPack8A[0],
	tRegPack8R[7:2], tSignRgba ? regValRs[47] : tRegPack8R[1],
	tRegPack8G[7:1], tSignRgba ? regValRs[31] : tRegPack8G[0],
	tRegPack8B[7:2], tSignRgba ? regValRs[15] : tRegPack8B[1]
	};

//wire		tPadR, tPadB;
//ExConv_Fp12Pck8		conv_pck32a(
//	regValRs[15: 4], { tRegRgba30[ 6: 0], tPadB }, 0);
//ExConv_Fp12Pck8		conv_pck32b(
//	regValRs[31:20], { tRegRgba30[14: 7]        }, 0);
//ExConv_Fp12Pck8		conv_pck32c(
//	regValRs[47:36], { tRegRgba30[21:15], tPadR }, 0);
//ExConv_Fp12Pck8		conv_pck32d(
//	regValRs[63:52], { tRegRgba30[29:22]        }, 0);
// assign	tRegRgba30[31:30] = { 1'b1, !tSignRgba };

wire[31:0]	tRegRgb30;
assign	tRegRgb30[31:30] = { 1'b0, !tSignRgb };
// assign	tRegRgb30[29:20] = tSignRgb ? regValRs[47:38] : regValRs[46:37];
// assign	tRegRgb30[19:10] = tSignRgb ? regValRs[31:22] : regValRs[30:21];
// assign	tRegRgb30[ 9: 0] = tSignRgb ? regValRs[15: 6] : regValRs[14: 5];

assign	tRegRgb30[29:20] =
	{ regValRs[46:38], tSignRgb ? regValRs[47] : regValRs[37] };
assign	tRegRgb30[19:10] =
	{ regValRs[30:22], tSignRgb ? regValRs[31] : regValRs[21] };
assign	tRegRgb30[ 9: 0] =
	{ regValRs[14: 6], tSignRgb ? regValRs[15] : regValRs[ 5] };

wire		tHasAlpha;
assign	tHasAlpha = (regValRs[63:52] != 12'h3C0);

assign	regOutVal = tHasAlpha ? tRegRgba30 : tRegRgb30 ;

endmodule

`endif

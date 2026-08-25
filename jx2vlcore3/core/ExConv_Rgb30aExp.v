/*
SPDX-License-Identifier: Apache-2.0
Copyright (c) 2018-2026 Brendan G Bohannon

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

`ifndef HAS_RGB30AEXP
`define HAS_RGB30AEXP

`include "ExConv_Fp9Exp12.v"

module ExConv_Rgb30aExp(
	/* verilator lint_off UNUSED */
	regValRs,	tagFmt,
	regOutVal
	);

input[31:0]		regValRs;
input[1:0]		tagFmt;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

`ifndef def_true
wire[7:0]	tFp8A_R;
wire[7:0]	tFp8A_G;
wire[7:0]	tFp8A_B;
wire[7:0]	tFp8A_A;

assign	tFp8A_A	= { regValRs[29:22]       };
assign	tFp8A_R	= { regValRs[21:15], 1'b0 };
assign	tFp8A_G	= { regValRs[14: 7]       };
assign	tFp8A_B	= { regValRs[ 6: 0], 1'b0 };
`endif

`ifdef def_true
wire[8:0]	tFp9A_R;
wire[8:0]	tFp9A_G;
wire[8:0]	tFp9A_B;
wire[8:0]	tFp9A_A;

assign	tFp9A_A	= { regValRs[22], regValRs[29:22]       };
assign	tFp9A_R	= { regValRs[15], regValRs[21:15], 1'b0 };
assign	tFp9A_G	= { regValRs[ 7], regValRs[14: 7]       };
assign	tFp9A_B	= { regValRs[ 0], regValRs[ 6: 0], 1'b0 };
`endif


wire[11:0]	tExpA_R;
wire[11:0]	tExpA_G;
wire[11:0]	tExpA_B;
wire[11:0]	tExpA_A;

wire		tExpIsSign;
assign		tExpIsSign = !tagFmt[0];

//ExConv_Fp8Exp12		exp8_Ar(tFp8A_R, tExpA_R, tExpIsSign);
//ExConv_Fp8Exp12		exp8_Ag(tFp8A_G, tExpA_G, tExpIsSign);
//ExConv_Fp8Exp12		exp8_Ab(tFp8A_B, tExpA_B, tExpIsSign);
//ExConv_Fp8Exp12		exp8_Aa(tFp8A_A, tExpA_A, tExpIsSign);

ExConv_Fp9Exp12		exp8_Ar(tFp9A_R, tExpA_R, tExpIsSign);
ExConv_Fp9Exp12		exp8_Ag(tFp9A_G, tExpA_G, tExpIsSign);
ExConv_Fp9Exp12		exp8_Ab(tFp9A_B, tExpA_B, tExpIsSign);
ExConv_Fp9Exp12		exp8_Aa(tFp9A_A, tExpA_A, tExpIsSign);

wire[11:0]	tLinA_R;
wire[11:0]	tLinA_G;
wire[11:0]	tLinA_B;

`ifndef def_true
assign		tLinA_R = tExpIsSign ?
	{       regValRs[29:20], 2'b0 } :
	{ 1'b0, regValRs[29:20], 1'b0 };
assign		tLinA_G = tExpIsSign ?
	{       regValRs[19:10], 2'b0 } :
	{ 1'b0, regValRs[19:10], 1'b0 };
assign		tLinA_B = tExpIsSign ?
	{       regValRs[ 9: 0], 2'b0 } :
	{ 1'b0, regValRs[ 9: 0], 1'b0 };
`endif

`ifdef def_true
assign		tLinA_R = 
	{ tExpIsSign ? regValRs[20] : 1'b0, regValRs[29:20], 1'b0 };
assign		tLinA_G = tExpIsSign ?
	{ tExpIsSign ? regValRs[10] : 1'b0, regValRs[19:10], 1'b0 };
assign		tLinA_B = tExpIsSign ?
	{ tExpIsSign ? regValRs[ 0] : 1'b0, regValRs[ 9: 0], 1'b0 };
`endif

reg[11:0]	tValA_R;
reg[11:0]	tValA_G;
reg[11:0]	tValA_B;
reg[11:0]	tValA_A;

always @*
begin
	if(tagFmt[1])
	begin
		tValA_R = tExpA_R;
		tValA_G = tExpA_G;
		tValA_B = tExpA_B;
		tValA_A = tExpA_A;
	end
	else
	begin
		tValA_R = tLinA_R;
		tValA_G = tLinA_G;
		tValA_B = tLinA_B;
		tValA_A = 12'h3C0;
	end
	
	tRegOutVal = {
		tValA_A, 4'h0, tValA_R, 4'h0,
		tValA_G, 4'h0, tValA_B, 4'h0 };
end

endmodule

`endif

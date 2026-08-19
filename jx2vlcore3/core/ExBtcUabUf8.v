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

`ifndef HAS_EXBTCUABUF8
`define HAS_EXBTCUABUF8

module ExBtcUabUf8(
	clock,	reset,
	valI,	valO);

input			clock;
input			reset;
input [ 7:0]	valI;
output[11:0]	valO;

reg[11:0]		tValO;
assign		valO = tValO;

reg[11:0]		tValA;

always @*
begin
	case(valI[6:4])
		3'b000: tValA = { 8'h00, valI[3:0]       };
		3'b001: tValA = { 8'h01, valI[3:0]       };
		3'b010: tValA = { 7'h01, valI[3:0], 1'b0 };
		3'b011: tValA = { 6'h01, valI[3:0], 2'b0 };
		3'b100: tValA = { 5'h01, valI[3:0], 3'b0 };
		3'b101: tValA = { 4'h01, valI[3:0], 4'b0 };
		3'b110: tValA = { 3'h01, valI[3:0], 5'b0 };
		3'b111: tValA = { 2'h01, valI[3:0], 6'b0 };
	endcase
	tValO = valI[7] ? (~tValA) : tValA;
end

endmodule

`endif

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

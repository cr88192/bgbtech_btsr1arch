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
limitations under the License.*/
*/

/*
Expand A-Law (S.E3.F4) to PCM12
 */

`ifndef HAS_AL8PCM12
`define HAS_AL8CMP12

module ExConv_Al8Pcm12(valI, valO);
input [ 7:0]	valI;
output[11:0]	valO;

reg[11:0]	tValC;
reg[11:0]	tValO;
assign		valO = tValO;

reg[2:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	tSgn = valI[7];
	tExpC = valI[6:4];
	tFraC = valI[3:0];

	case(tExpC)
		3'b000: tValC = { 8'h00, tFraC };
		3'b001: tValC = { 8'h01, tFraC };
		3'b010: tValC = { 7'h01, tFraC, 1'b0 };
		3'b011: tValC = { 6'h01, tFraC, 2'b0 };
		3'b100: tValC = { 5'h01, tFraC, 3'b0 };
		3'b101: tValC = { 4'h01, tFraC, 4'b0 };
		3'b110: tValC = { 3'h01, tFraC, 5'b0 };
		3'b111: tValC = { 2'h01, tFraC, 6'b0 };
	endcase

	tValO = tSgn ? ~tValC : tValC;

//	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

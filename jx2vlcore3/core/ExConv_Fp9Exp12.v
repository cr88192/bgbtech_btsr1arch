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

/*
Expand FP9 (S.E4.F4) to FP12 (S.E5.F6)
 */

`ifndef HAS_FP9EXP12
`define HAS_FP9EXP12

module ExConv_Fp9Exp12(valI, valO, hasSgn);
input [ 8:0]	valI;
output[11:0]	valO;
input[1:0]		hasSgn;

reg[11:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	tSgn = hasSgn[0] & valI[8];
	tExpC = { valI[7], !valI[7], valI[6:4] };
	tFraC = valI[3:0];

	if(!hasSgn[1] && (tExpC==5'h08))
		tExpC=0;
	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

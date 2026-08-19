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
/ Expand FP8 (E3.F5) to FP12 (S.E5.F6)
Expand FP8 (E4.F4) to FP12 (S.E5.F6)
 */

`ifndef HAS_FP8EXP12
`define HAS_FP8EXP12

module ExConv_Fp8Exp12(valI, valO, hasSgn);
input [ 7:0]	valI;
output[11:0]	valO;
input[1:0]		hasSgn;

reg[11:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	if(hasSgn[0])
	begin

`ifndef def_true
		tSgn = valI[7];
//		tExpC = { valI[6], !valI[5], valI[5:3] };
		tExpC = { valI[6], !valI[6], valI[5:3] };
		tFraC = { valI[2:0], 1'b0 };
`endif

`ifdef def_true
		tSgn = valI[0];
		tExpC = { valI[7], !valI[7], valI[6:4] };
		tFraC = { valI[3:1], 1'b0 };
`endif

	end
	else
	begin
		tSgn = 0;
//		tExpC = { !valI[7], valI[7:4] };
//		tExpC = { valI[7], !valI[6], valI[6:4] };
		tExpC = { valI[7], !valI[7], valI[6:4] };
		tFraC = valI[3:0];
	end

	if(!hasSgn[1] && (tExpC==5'h08))
		tExpC=0;
	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

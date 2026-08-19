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

`ifndef HAS_FPUCONVS2H
`define HAS_FPUCONVS2H

module FpuConvS2H(valI, valO);
input [31:0]	valI;
output[15:0]	valO;

reg[15:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[31];
	tExpC	= { valI[30], valI[26:23] };

	if((valI[30:26] != 5'h0F) && (valI[30:26] != 5'h10))
	begin
		tExpC = valI[30] ? 5'h1F : 5'h00 ;
		if(!valI[30])
			tSgn = 0;
	end

	tValO = { tSgn, tExpC, valI[22:13] };
end

endmodule

`endif

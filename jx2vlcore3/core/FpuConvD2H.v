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

`ifndef HAS_FPUCONVD2H
`define HAS_FPUCONVD2H

module FpuConvD2H(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[15:0]	regValFRn;

reg[15:0]		tRegValFRn;
reg[15:0]		tRegValFRn1;
reg[4:0]		tRound;

assign	regValFRn = tRegValFRn;

always @*
begin
	tRegValFRn1	= {
		regValFRm[63:62],
		regValFRm[55:52],
		regValFRm[51:42] };

`ifndef jx2_fpu_noround
	tRound = { 1'b0, regValFRm[45:42] } + 1;
	if(regValFRm[41] && !tRound[4])
		tRegValFRn1[3:0]=tRound[3:0];
`endif

	casez(regValFRm[62:56])
	7'b0111111:	tRegValFRn = tRegValFRn1;
	7'b1000000:	tRegValFRn = tRegValFRn1;
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 15'h7C00 };
		else
			tRegValFRn = 0;
	endcase
end

endmodule

`endif

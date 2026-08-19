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

`ifndef HAS_FPUCONVS2D
`define HAS_FPUCONVS2D

module FpuConvS2D(
	regValFRm,
	regValFRn);

input[31:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;
reg				tExpIsNaN;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[30:23]==8'h00);
	tExpIsNaN  = (regValFRm[30:23]==8'hFF);

	tRegValFRn = {
		regValFRm[31:30],
//		regValFRm[29] ? 4'b1111 : 4'b0000,
//		(!regValFRm[30] && !tExpIsZero) ? 4'b1111 : 4'b0000,
		((!regValFRm[30] && !tExpIsZero) ||
			tExpIsNaN) ? 3'b111 : 3'b000,
//		regValFRm[28:23],
		regValFRm[29:23],
		regValFRm[22: 0],
		29'h0 };
end

endmodule

`endif

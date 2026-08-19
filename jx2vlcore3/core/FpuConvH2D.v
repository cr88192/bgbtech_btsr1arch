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

`ifndef HAS_FPUCONVH2D
`define HAS_FPUCONVH2D

module FpuConvH2D(
	regValFRm,
	regValFRn);

input[15:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;
reg				tExpIsNaN;

reg[63:0]		tRegValSpConst;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[14:10]==5'h00);
	tExpIsNaN  = (regValFRm[14:10]==5'h1F);

	tRegValFRn = {
		regValFRm[15:14],
//		regValFRm[13] ? 7'b1111111 : 7'b0000000,
//		(!regValFRm[14] && !tExpIsZero) ? 7'b1111111 : 7'b0000000,
		((!regValFRm[14] && !tExpIsZero) || tExpIsNaN) ?
			6'b111111 : 6'b000000,
//		regValFRm[12:10],
		regValFRm[13:10],
		regValFRm[9:0],
		42'h0 };

// `ifdef def_true
`ifndef def_true
	case(regValFRm[5:0])
		6'h00:		tRegValSpConst = 64'h7FF0_0000_0000_0000;
		default:	tRegValSpConst = 64'h7FF0_0000_0000_0000;
	endcase

	if(tExpIsNaN && (regValFRm[9:6]==0))
		tRegValFRn = { regValFRm[15], tRegValSpConst[62:0] };
`endif
end

endmodule

`endif

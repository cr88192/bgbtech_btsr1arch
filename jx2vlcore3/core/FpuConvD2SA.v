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
 * Double to Single, Low-Cost Version
 */

`ifndef HAS_FPUCONVD2SA
`define HAS_FPUCONVD2SA

module FpuConvD2SA(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[31:0]	regValFRn;

reg[31:0]		tRegValFRn;
reg[31:0]		tRegValFRn1;

assign	regValFRn = tRegValFRn;

always @*
begin
	tRegValFRn1	= {
		regValFRm[63:62],
		regValFRm[58:52],
		regValFRm[51:29] };

	casez(regValFRm[62:58])
		5'b0111z:	tRegValFRn = tRegValFRn1;
		5'b1000z:	tRegValFRn = tRegValFRn1;

		default:
		begin
			if(regValFRm[62])
				tRegValFRn	= { regValFRm[63], 31'h7F800000 };
			else
				tRegValFRn = 0;
		end
	endcase
end

endmodule

`endif

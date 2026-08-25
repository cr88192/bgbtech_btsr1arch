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

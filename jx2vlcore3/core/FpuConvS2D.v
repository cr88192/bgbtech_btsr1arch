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

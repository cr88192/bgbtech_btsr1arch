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

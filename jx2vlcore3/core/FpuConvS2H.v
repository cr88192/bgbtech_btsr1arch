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

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

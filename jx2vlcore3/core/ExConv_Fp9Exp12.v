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

/*
Expand FP9 (S.E4.F4) to FP12 (S.E5.F6)
 */

`ifndef HAS_FP9EXP12
`define HAS_FP9EXP12

module ExConv_Fp9Exp12(valI, valO, hasSgn);
input [ 8:0]	valI;
output[11:0]	valO;
input[1:0]		hasSgn;

reg[11:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	tSgn = hasSgn[0] & valI[8];
	tExpC = { valI[7], !valI[7], valI[6:4] };
	tFraC = valI[3:0];

	if(!hasSgn[1] && (tExpC==5'h08))
		tExpC=0;
	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

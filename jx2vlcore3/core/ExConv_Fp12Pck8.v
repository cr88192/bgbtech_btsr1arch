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

`ifndef HAS_FP12PCK8
`define HAS_FP12PCK8

module ExConv_Fp12Pck8(valI, valO, hasSgn);
input [11:0]	valI;
output[ 7:0]	valO;
input[1:0]		hasSgn;

reg[7:0]	tValO;
assign		valO = tValO;

reg[3:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[11];
	tExpC	= { valI[10], valI[8:6] };

	if(valI[10] == valI[9])
		tExpC = valI[10] ? 4'hF : 4'h0 ;

	if(hasSgn[0])
//		tValO = { tSgn, tExpC, valI[5:3] };
		tValO = { tExpC, valI[5:3], tSgn };
	else
		tValO = { tExpC, valI[5:2] };
end

endmodule

`endif

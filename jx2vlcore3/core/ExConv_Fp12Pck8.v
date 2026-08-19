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

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

`ifndef HAS_FP16EXP32
`define HAS_FP16EXP32

module ExConv_Fp16Exp32(valI, valE, valO);
input [15:0]	valI;
output[31:0]	valO;
input [ 4:0]	valE;

reg[31:0]	tValO;
assign		valO = tValO;

reg[7:0]	tExpC;
reg[9:0]	tFraC;
reg			tSgn;

reg			tExpIsZero;
reg			tExpIsNaN;

always @*
begin
	tExpIsZero = (valI[14:10]==5'h00);
	tExpIsNaN  = (valI[14:10]==5'h1F);

	tSgn = valI[15];
	tExpC = {
		valI[14],
		((valI[14] || tExpIsZero) && !tExpIsNaN) ?
			3'b000 : 3'b111,
		valI[13:10] };
	tFraC = valI[9:0];

//	if(valI[14:10]==5'h00)
//	if((valI[14:10]==5'h00) || valE[5])
//	if(valI[14:10]==5'h00)
//		tExpC=0;
//	if(valI[14:10]==5'h1F)
//		tExpC=255;

//	tValO = { tSgn, tExpC, tFraC, 13'h0 };
	tValO = { tSgn, tExpC, tFraC, valE[4:0], 8'h0 };
end

endmodule

`endif

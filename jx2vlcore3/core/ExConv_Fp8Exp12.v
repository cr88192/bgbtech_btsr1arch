/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
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

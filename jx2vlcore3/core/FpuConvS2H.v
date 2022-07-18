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

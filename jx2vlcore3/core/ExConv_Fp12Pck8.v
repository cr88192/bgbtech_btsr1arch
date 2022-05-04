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
		tValO = { tSgn, tExpC, valI[5:3] };
	else
		tValO = { tExpC, valI[5:2] };
end

endmodule

`endif

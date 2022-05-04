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

`ifndef HAS_FPUCONVD2H
`define HAS_FPUCONVD2H

module FpuConvD2H(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[15:0]	regValFRn;

reg[15:0]		tRegValFRn;
reg[15:0]		tRegValFRn1;
reg[4:0]		tRound;

assign	regValFRn = tRegValFRn;

always @*
begin
	tRegValFRn1	= {
		regValFRm[63:62],
		regValFRm[55:52],
		regValFRm[51:42] };

	tRound = { 1'b0, regValFRm[45:42] } + 1;
	if(regValFRm[41] && !tRound[4])
		tRegValFRn1[3:0]=tRound[3:0];

	casez(regValFRm[62:56])
	7'b0111111:	tRegValFRn = tRegValFRn1;
	7'b1000000:	tRegValFRn = tRegValFRn1;
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 15'h7C00 };
		else
			tRegValFRn = 0;
	endcase
end

endmodule

`endif

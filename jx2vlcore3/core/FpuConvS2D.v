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

`ifndef HAS_FPUCONVS2D
`define HAS_FPUCONVS2D

module FpuConvS2D(
	regValFRm,
	regValFRn);

input[31:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;
reg				tExpIsNaN;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[30:23]==8'h00);
	tExpIsNaN  = (regValFRm[30:23]==8'hFF);

	tRegValFRn = {
		regValFRm[31:30],
//		regValFRm[29] ? 4'b1111 : 4'b0000,
//		(!regValFRm[30] && !tExpIsZero) ? 4'b1111 : 4'b0000,
		((!regValFRm[30] && !tExpIsZero) ||
			tExpIsNaN) ? 3'b111 : 3'b000,
//		regValFRm[28:23],
		regValFRm[29:23],
		regValFRm[22: 0],
		29'h0 };
end

endmodule

`endif

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

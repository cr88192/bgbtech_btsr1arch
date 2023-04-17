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
Expand A-Law (S.E3.F4) to PCM12
 */

`ifndef HAS_AL8PCM12
`define HAS_AL8CMP12

module ExConv_Al8Pcm12(valI, valO);
input [ 7:0]	valI;
output[11:0]	valO;

reg[11:0]	tValC;
reg[11:0]	tValO;
assign		valO = tValO;

reg[2:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	tSgn = valI[7];
	tExpC = valI[6:4];
	tFraC = valI[3:0];

	case(tExpC)
		3'b000: tValC = { 8'h00, tFraC };
		3'b001: tValC = { 8'h01, tFraC };
		3'b010: tValC = { 7'h01, tFraC, 1'b0 };
		3'b011: tValC = { 6'h01, tFraC, 2'b0 };
		3'b100: tValC = { 5'h01, tFraC, 3'b0 };
		3'b101: tValC = { 4'h01, tFraC, 4'b0 };
		3'b110: tValC = { 3'h01, tFraC, 5'b0 };
		3'b111: tValC = { 2'h01, tFraC, 6'b0 };
	endcase

	tValO = tSgn ? ~tValC : tValC;

//	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

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

`ifndef HAS_EXBTCUABUF8
`define HAS_EXBTCUABUF8

module ExBtcUabUf8(
	clock,	reset,
	valI,	valO);

input			clock;
input			reset;
input [ 7:0]	valI;
output[11:0]	valO;

reg[11:0]		tValO;
assign		valO = tValO;

reg[11:0]		tValA;

always @*
begin
	case(valI[6:4])
		3'b000: tValA = { 8'h00, valI[3:0]       };
		3'b001: tValA = { 8'h01, valI[3:0]       };
		3'b010: tValA = { 7'h01, valI[3:0], 1'b0 };
		3'b011: tValA = { 6'h01, valI[3:0], 2'b0 };
		3'b100: tValA = { 5'h01, valI[3:0], 3'b0 };
		3'b101: tValA = { 4'h01, valI[3:0], 4'b0 };
		3'b110: tValA = { 3'h01, valI[3:0], 5'b0 };
		3'b111: tValA = { 2'h01, valI[3:0], 6'b0 };
	endcase
	tValO = valI[7] ? (~tValA) : tValA;
end

endmodule

`endif

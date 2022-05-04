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

`include "ExBcdAdd4.v"

module ExBcdAdd16(a, b, c, c_in, c_out);
input [15:0]		a;
input [15:0]		b;
output[15:0]		c;
input			c_in;
output			c_out;

wire[16:0]	tCa;

reg[15:0]	tC;
reg			tCo;
assign		c = tC;
assign		c_out = tCo;

wire	tCarry1;
wire	tCarry2;
wire	tCarry3;
wire	tCarry4;

ExBcdAdd4	add1(a[ 3: 0], b[ 3: 0], tCa[ 3: 0], c_in, tCarry1);
ExBcdAdd4	add2(a[ 7: 4], b[ 7: 4], tCa[ 7: 4], tCarry1, tCarry2);
ExBcdAdd4	add3(a[11: 8], b[11: 8], tCa[11: 8], tCarry2, tCarry3);
ExBcdAdd4	add4(a[15:12], b[15:12], tCa[15:12], tCarry3, tCarry4);

always @*
begin
	tC	= tCa[15:0];
	tCo = tCarry4;
end

endmodule

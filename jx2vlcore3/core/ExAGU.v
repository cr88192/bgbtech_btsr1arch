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
Add With Scale

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [5:4]=Type (B/W/L/Q)
  [  3]=?
  [  2]=ZExt (0=SX, 1=ZX)
  [1:0]=Scale (B/W/L/Q)

 */

module ExAGU(
	regValRm,
	regValRi,
	idUIxt,
	regOutAddr);

input[31:0]		regValRm;
input[31:0]		regValRi;
input[8:0]		idUIxt;

output[31:0]	regOutAddr;

reg[31:0]	tRiSc0;
reg[31:0]	tRiSc1;
reg[31:0]	tRiSc2;
reg[31:0]	tRiSc3;

reg[31:0]	tAddrSc0;
reg[31:0]	tAddrSc1;
reg[31:0]	tAddrSc2;
reg[31:0]	tAddrSc3;

reg[31:0]	tAddr;

assign		regOutAddr = tAddr;

always @*
begin
	tRiSc0 = regValRi;
	tRiSc1 = { regValRi[30:0], 1'b0 };
	tRiSc2 = { regValRi[29:0], 2'b0 };
	tRiSc3 = { regValRi[28:0], 3'b0 };
	
	tAddrSc0 = regValRm + tRiSc0;
	tAddrSc1 = regValRm + tRiSc1;
	tAddrSc2 = regValRm + tRiSc2;
	tAddrSc3 = regValRm + tRiSc3;
	
	case(idUIxt[1:0])
		2'b00:	tAddr=tAddrSc0;
		2'b01:	tAddr=tAddrSc1;
		2'b10:	tAddr=tAddrSc2;
		2'b11:	tAddr=tAddrSc3;
	endcase
end

endmodule

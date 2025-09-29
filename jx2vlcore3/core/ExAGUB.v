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

`ifndef HAS_EXAGUB
`define HAS_EXAGUB

/* verilator lint_off UNUSEDSIGNAL */

module ExAGUB(
	regValRm,
	regValRi,
	idUIxt,
	regOutAddr,
	addrEnJq);

input[47:0]		regValRm;
input[47:0]		regValRi;
input[8:0]		idUIxt;
input			addrEnJq;

output[47:0]	regOutAddr;

reg[47:0]	tRiSc0;
reg[47:0]	tRiSc1;
reg[47:0]	tRiSc2;
reg[47:0]	tRiSc3;

reg[47:0]	tAddrSc0;
reg[47:0]	tAddrSc1;
reg[47:0]	tAddrSc2;
reg[47:0]	tAddrSc3;

reg[16:0]	tAddrSc0A;
reg[16:0]	tAddrSc1A;
reg[16:0]	tAddrSc2A;
reg[16:0]	tAddrSc3A;

reg[16:0]	tAddrSc0B;
reg[16:0]	tAddrSc1B;
reg[16:0]	tAddrSc2B;
reg[16:0]	tAddrSc3B;

reg[16:0]	tAddrSc0B0;
reg[16:0]	tAddrSc1B0;
reg[16:0]	tAddrSc2B0;
reg[16:0]	tAddrSc3B0;
reg[16:0]	tAddrSc0B1;
reg[16:0]	tAddrSc1B1;
reg[16:0]	tAddrSc2B1;
reg[16:0]	tAddrSc3B1;

reg[16:0]	tAddrSc0C0;
reg[16:0]	tAddrSc1C0;
reg[16:0]	tAddrSc2C0;
reg[16:0]	tAddrSc3C0;
reg[16:0]	tAddrSc0C1;
reg[16:0]	tAddrSc1C1;
reg[16:0]	tAddrSc2C1;
reg[16:0]	tAddrSc3C1;

reg[15:0]	tAddrSc0C;
reg[15:0]	tAddrSc1C;
reg[15:0]	tAddrSc2C;
reg[15:0]	tAddrSc3C;

reg[47:0]	tAddr;

assign		regOutAddr = tAddr;

always @*
begin
	tRiSc0 = regValRi;
	tRiSc1 = { regValRi[46:0], 1'b0 };
	tRiSc2 = { regValRi[45:0], 2'b0 };
	tRiSc3 = { regValRi[44:0], 3'b0 };

//	tAddrSc0 = regValRm + tRiSc0;
//	tAddrSc1 = regValRm + tRiSc1;
//	tAddrSc2 = regValRm + tRiSc2;
//	tAddrSc3 = regValRm + tRiSc3;

//	tAddrSc0A = regValRm[31:0] + tRiSc0[31:0];
//	tAddrSc1A = regValRm[31:0] + tRiSc1[31:0];
//	tAddrSc2A = regValRm[31:0] + tRiSc2[31:0];
//	tAddrSc3A = regValRm[31:0] + tRiSc3[31:0];

	tAddrSc0A = { 1'b0, regValRm[15:0] } + { 1'b0, tRiSc0[15:0] };
	tAddrSc1A = { 1'b0, regValRm[15:0] } + { 1'b0, tRiSc1[15:0] };
	tAddrSc2A = { 1'b0, regValRm[15:0] } + { 1'b0, tRiSc2[15:0] };
	tAddrSc3A = { 1'b0, regValRm[15:0] } + { 1'b0, tRiSc3[15:0] };

	tAddrSc0B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc0[31:16] } + 0;
	tAddrSc1B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc1[31:16] } + 0;
	tAddrSc2B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc2[31:16] } + 0;
	tAddrSc3B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc3[31:16] } + 0;
	tAddrSc0B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc0[31:16] } + 1;
	tAddrSc1B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc1[31:16] } + 1;
	tAddrSc2B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc2[31:16] } + 1;
	tAddrSc3B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc3[31:16] } + 1;

	tAddrSc0C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc0[47:32] } + 0;
	tAddrSc1C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc1[47:32] } + 0;
	tAddrSc2C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc2[47:32] } + 0;
	tAddrSc3C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc3[47:32] } + 0;
	tAddrSc0C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc0[47:32] } + 1;
	tAddrSc1C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc1[47:32] } + 1;
	tAddrSc2C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc2[47:32] } + 1;
	tAddrSc3C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc3[47:32] } + 1;
	
	tAddrSc0B = tAddrSc0A[16] ? tAddrSc0B1 : tAddrSc0B0;
	tAddrSc1B = tAddrSc1A[16] ? tAddrSc1B1 : tAddrSc1B0;
	tAddrSc2B = tAddrSc2A[16] ? tAddrSc2B1 : tAddrSc2B0;
	tAddrSc3B = tAddrSc3A[16] ? tAddrSc3B1 : tAddrSc3B0;

	if(addrEnJq)
	begin
//		tAddrSc0C = regValRm[47:32] + tRiSc0[47:32];
//		tAddrSc1C = regValRm[47:32] + tRiSc1[47:32];
//		tAddrSc2C = regValRm[47:32] + tRiSc2[47:32];
//		tAddrSc3C = regValRm[47:32] + tRiSc3[47:32];

		tAddrSc0C = tAddrSc0B[16] ? tAddrSc0C1[15:0] : tAddrSc0C0[15:0];
		tAddrSc1C = tAddrSc1B[16] ? tAddrSc1C1[15:0] : tAddrSc1C0[15:0];
		tAddrSc2C = tAddrSc2B[16] ? tAddrSc2C1[15:0] : tAddrSc2C0[15:0];
		tAddrSc3C = tAddrSc3B[16] ? tAddrSc3C1[15:0] : tAddrSc3C0[15:0];

	end
	else
	begin
		tAddrSc0C = 16'h0000;
		tAddrSc1C = 16'h0000;
		tAddrSc2C = 16'h0000;
		tAddrSc3C = 16'h0000;
	end

	tAddrSc0 = { tAddrSc0C[15:0], tAddrSc0B[15:0], tAddrSc0A[15:0] };
	tAddrSc1 = { tAddrSc1C[15:0], tAddrSc1B[15:0], tAddrSc1A[15:0] };
	tAddrSc2 = { tAddrSc2C[15:0], tAddrSc2B[15:0], tAddrSc2A[15:0] };
	tAddrSc3 = { tAddrSc3C[15:0], tAddrSc3B[15:0], tAddrSc3A[15:0] };
	
	case(idUIxt[1:0])
		2'b00:	tAddr=tAddrSc0;
		2'b01:	tAddr=tAddrSc1;
		2'b10:	tAddr=tAddrSc2;
		2'b11:	tAddr=tAddrSc3;
	endcase
	
//	if(!addrEnJq)
//		tAddr[47:32]=16'h0000;
end

endmodule

`endif

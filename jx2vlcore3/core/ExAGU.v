/*
// SPDX-License-Identifier: CERN-OHL-P-2.0
// Copyright (c) 2018-2026 Brendan G Bohannon
//
// This source code is licensed under the CERN Open Hardware Licence 
// Strongly Reciprocal version 2 or later (CERN-OHL-P v2+).
//
// You may redistribute and modify this source code under the terms of 
// the CERN-OHL-P v2+. A copy of this license should be included with 
// this source code. If not, see <https://ohwr.org>.
//
// This source code is offered "as is" without any express or implied
// warranties. See the License for more details.
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

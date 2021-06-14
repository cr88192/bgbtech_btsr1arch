/*
Carry Save for 3x 16-bit inputs, producing 2x 16-bit outputs.
 */

`include "ExCsa3x8.v"

module ExCsa3x16(valA, valB, valC, valP, valQ);
input[15:0]		valA;
input[15:0]		valB;
input[15:0]		valC;

output[15:0]		valP;
output[15:0]		valQ;

wire[7:0]	csaAp;
wire[7:0]	csaAq;
ExCsa3x8	csaA(valA[7:0], valB[7:0], valC[7:0], csaAp, csaAq);

wire[7:0]	csaBp;
wire[7:0]	csaBq;
ExCsa3x8	csaB(valA[15:8], valB[15:8], valC[15:8], csaBp, csaBq);

assign	valP = { csaBp, csaAp };
assign	valQ = { csaBq, csaAq };

endmodule

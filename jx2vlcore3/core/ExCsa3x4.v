/*
Carry Save for 3x 4-bit inputs, producing 2x 4-bit outputs.
 */

`include "ExCsa3x2.v"

module ExCsa3x4(valA, valB, valC, valP, valQ);
input[3:0]		valA;
input[3:0]		valB;
input[3:0]		valC;

output[3:0]		valP;
output[3:0]		valQ;

wire[1:0]	csaAp;
wire[1:0]	csaAq;
ExCsa3x2	csaA(valA[1:0], valB[1:0], valC[1:0], csaAp, csaAq);

wire[1:0]	csaBp;
wire[1:0]	csaBq;
ExCsa3x2	csaB(valA[3:2], valB[3:2], valC[3:2], csaBp, csaBq);

assign	valP = { csaBp, csaAp };
assign	valQ = { csaBq, csaAq };

endmodule

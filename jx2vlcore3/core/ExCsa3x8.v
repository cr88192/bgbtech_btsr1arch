/*
Carry Save for 3x 8-bit inputs, producing 2x 8-bit outputs.
 */

`include "ExCsa3x2.v"

module ExCsa3x8(valA, valB, valC, valP, valQ);
input[7:0]		valA;
input[7:0]		valB;
input[7:0]		valC;

output[7:0]		valP;
output[7:0]		valQ;

wire[1:0]	csaAp;
wire[1:0]	csaAq;
ExCsa3x2	csaA(valA[1:0], valB[1:0], valC[1:0], csaAp, csaAq);

wire[1:0]	csaBp;
wire[1:0]	csaBq;
ExCsa3x2	csaB(valA[3:2], valB[3:2], valC[3:2], csaBp, csaBq);

wire[1:0]	csaCp;
wire[1:0]	csaCq;
ExCsa3x2	csaC(valA[5:4], valB[5:4], valC[5:4], csaCp, csaCq);

wire[1:0]	csaDp;
wire[1:0]	csaDq;
ExCsa3x2	csaD(valA[7:6], valB[7:6], valC[7:6], csaDp, csaDq);

assign	valP = { csaDp, csaCp, csaBp, csaAp };
assign	valQ = { csaDq, csaCq, csaBq, csaAq };

endmodule

`ifndef HAS_ExMisc_MulVecFp8
`define HAS_ExMisc_MulVecFp8

`include "ExMisc_MulFp8.v"

module ExMisc_MulVecFp8(valA, valB, valRes);

input[31:0] valA;
input[31:0] valB;
output[63:0] valRes;

ExMisc_MulFp8	mula(valA[ 7: 0], valB[ 7: 0], valRes[15: 0]);
ExMisc_MulFp8	mulb(valA[15: 8], valB[15: 8], valRes[31:16]);
ExMisc_MulFp8	mulc(valA[23:16], valB[23:16], valRes[47:32]);
ExMisc_MulFp8	muld(valA[31:24], valB[31:24], valRes[63:48]);

endmodule

`endif

`ifndef HAS_CSADD144F
`define HAS_CSADD144F

module ExCsAdd144F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[143:0]	valA;
input[143:0]	valB;
output[143:0]	valC;

reg[16:0]		tVal0_A0;
reg[16:0]		tVal0_A1;
reg[16:0]		tVal0_B0;
reg[16:0]		tVal0_B1;
reg[16:0]		tVal0_C0;
reg[16:0]		tVal0_C1;
reg[16:0]		tVal0_D0;
reg[16:0]		tVal0_D1;
reg[16:0]		tVal0_E0;
reg[16:0]		tVal0_E1;
reg[16:0]		tVal0_F0;
reg[16:0]		tVal0_F1;
reg[16:0]		tVal0_G0;
reg[16:0]		tVal0_G1;
reg[16:0]		tVal0_H0;
reg[16:0]		tVal0_H1;
reg[16:0]		tVal0_I0;
reg[16:0]		tVal0_I1;

reg[32:0]		tVal1_A0;
reg[32:0]		tVal1_A1;
reg[32:0]		tVal1_B0;
reg[32:0]		tVal1_B1;
reg[32:0]		tVal1_C0;
reg[32:0]		tVal1_C1;
reg[32:0]		tVal1_D0;
reg[32:0]		tVal1_D1;
reg[16:0]		tVal1_E0;
reg[16:0]		tVal1_E1;

reg[ 64:0]		tVal2_A0;
reg[ 64:0]		tVal2_A1;
reg[ 64:0]		tVal2_B0;
reg[ 64:0]		tVal2_B1;
reg[ 16:0]		tVal2_C0;
reg[ 16:0]		tVal2_C1;

reg[128:0]		tVal3_A0;
reg[143:0]		tVal3;

assign valC = tVal3[143:0];

// /* verilator lint_on UNOPTFLAT */

always @*
begin

	tVal0_A0 = { 1'b0, valA[ 15:  0] } + { 1'b0, valB[ 15:  0] } + 0;
	tVal0_A1 = { 1'b0, valA[ 15:  0] } + { 1'b0, valB[ 15:  0] } + 1;
	tVal0_B0 = { 1'b0, valA[ 31: 16] } + { 1'b0, valB[ 31: 16] } + 0;
	tVal0_B1 = { 1'b0, valA[ 31: 16] } + { 1'b0, valB[ 31: 16] } + 1;
	tVal0_C0 = { 1'b0, valA[ 47: 32] } + { 1'b0, valB[ 47: 32] } + 0;
	tVal0_C1 = { 1'b0, valA[ 47: 32] } + { 1'b0, valB[ 47: 32] } + 1;
	tVal0_D0 = { 1'b0, valA[ 63: 48] } + { 1'b0, valB[ 63: 48] } + 0;
	tVal0_D1 = { 1'b0, valA[ 63: 48] } + { 1'b0, valB[ 63: 48] } + 1;
	tVal0_E0 = { 1'b0, valA[ 79: 64] } + { 1'b0, valB[ 79: 64] } + 0;
	tVal0_E1 = { 1'b0, valA[ 79: 64] } + { 1'b0, valB[ 79: 64] } + 1;
	tVal0_F0 = { 1'b0, valA[ 95: 80] } + { 1'b0, valB[ 95: 80] } + 0;
	tVal0_F1 = { 1'b0, valA[ 95: 80] } + { 1'b0, valB[ 95: 80] } + 1;
	tVal0_G0 = { 1'b0, valA[111: 96] } + { 1'b0, valB[111: 96] } + 0;
	tVal0_G1 = { 1'b0, valA[111: 96] } + { 1'b0, valB[111: 96] } + 1;
	tVal0_H0 = { 1'b0, valA[127:112] } + { 1'b0, valB[127:112] } + 0;
	tVal0_H1 = { 1'b0, valA[127:112] } + { 1'b0, valB[127:112] } + 1;
	tVal0_I0 = { 1'b0, valA[143:128] } + { 1'b0, valB[143:128] } + 0;
	tVal0_I1 = { 1'b0, valA[143:128] } + { 1'b0, valB[143:128] } + 1;
	
	tVal1_A0 = { tVal0_A0[16] ? tVal0_B1 : tVal0_B0, tVal0_A0[15:0] };
	tVal1_A1 = { tVal0_A1[16] ? tVal0_B1 : tVal0_B0, tVal0_A1[15:0] };
	tVal1_B0 = { tVal0_C0[16] ? tVal0_D1 : tVal0_D0, tVal0_C0[15:0] };
	tVal1_B1 = { tVal0_C1[16] ? tVal0_D1 : tVal0_D0, tVal0_C1[15:0] };
	tVal1_C0 = { tVal0_E0[16] ? tVal0_F1 : tVal0_F0, tVal0_E0[15:0] };
	tVal1_C1 = { tVal0_E1[16] ? tVal0_F1 : tVal0_F0, tVal0_E1[15:0] };
	tVal1_D0 = { tVal0_G0[16] ? tVal0_H1 : tVal0_H0, tVal0_G0[15:0] };
	tVal1_D1 = { tVal0_G1[16] ? tVal0_H1 : tVal0_H0, tVal0_G1[15:0] };
	tVal1_E0 = tVal0_I0;
	tVal1_E1 = tVal0_I1;

//	tVal2 = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
//	tVal2_A = {
//		 tVal1_A0[32] ? tVal1_B1[32:0] : tVal1_B0[32:0],
//		!tVal1_B0[32] ? tVal1_A1[31:0] : tVal1_A0[31:0] };

	tVal2_A0 = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
	tVal2_A1 = { tVal1_A1[32] ? tVal1_B1 : tVal1_B0, tVal1_A1[31:0] };
	tVal2_B0 = { tVal1_C0[32] ? tVal1_D1 : tVal1_D0, tVal1_C0[31:0] };
	tVal2_B1 = { tVal1_C1[32] ? tVal1_D1 : tVal1_D0, tVal1_C1[31:0] };
	tVal2_C0 = tVal1_E0;
	tVal2_C1 = tVal1_E1;

	tVal3A = { tVal2_A0[64] ? tVal2_B1 : tVal2_B0, tVal2_A0[63:0] };
	tVal3 = { tVal3A[128] ? tVal2_C1 : tVal2_C0, tVal3A[127:0] };
end

endmodule

`endif

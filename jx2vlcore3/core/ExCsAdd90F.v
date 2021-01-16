`ifndef HAS_CSADD90F
`define HAS_CSADD90F

module ExCsAdd90F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[89:0]		valA;
input[89:0]		valB;
output[89:0]	valC;

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
reg[10:0]		tVal0_F0;
reg[10:0]		tVal0_F1;

reg[32:0]		tVal1_A0;
reg[32:0]		tVal1_A1;
reg[32:0]		tVal1_B0;
reg[32:0]		tVal1_B1;
reg[26:0]		tVal1_C0;
reg[26:0]		tVal1_C1;

reg[64:0]		tVal2_A;
reg[90:0]		tVal2;

assign valC = tVal2[89:0];

// /* verilator lint_on UNOPTFLAT */

always @*
begin

	tVal0_A0 = { 1'b0, valA[15: 0] } + { 1'b0, valB[15: 0] } + 0;
	tVal0_A1 = { 1'b0, valA[15: 0] } + { 1'b0, valB[15: 0] } + 1;
	tVal0_B0 = { 1'b0, valA[31:16] } + { 1'b0, valB[31:16] } + 0;
	tVal0_B1 = { 1'b0, valA[31:16] } + { 1'b0, valB[31:16] } + 1;
	tVal0_C0 = { 1'b0, valA[47:32] } + { 1'b0, valB[47:32] } + 0;
	tVal0_C1 = { 1'b0, valA[47:32] } + { 1'b0, valB[47:32] } + 1;
	tVal0_D0 = { 1'b0, valA[63:48] } + { 1'b0, valB[63:48] } + 0;
	tVal0_D1 = { 1'b0, valA[63:48] } + { 1'b0, valB[63:48] } + 1;
	tVal0_E0 = { 1'b0, valA[79:64] } + { 1'b0, valB[79:64] } + 0;
	tVal0_E1 = { 1'b0, valA[79:64] } + { 1'b0, valB[79:64] } + 1;
	tVal0_F0 = { 1'b0, valA[89:80] } + { 1'b0, valB[89:80] } + 0;
	tVal0_F1 = { 1'b0, valA[89:80] } + { 1'b0, valB[89:80] } + 1;
	
	tVal1_A0 = { tVal0_A0[16] ? tVal0_B1 : tVal0_B0, tVal0_A0[15:0] };
	tVal1_A1 = { tVal0_A1[16] ? tVal0_B1 : tVal0_B0, tVal0_A1[15:0] };
	tVal1_B0 = { tVal0_C0[16] ? tVal0_D1 : tVal0_D0, tVal0_C0[15:0] };
	tVal1_B1 = { tVal0_C1[16] ? tVal0_D1 : tVal0_D0, tVal0_C1[15:0] };
	tVal1_C0 = { tVal0_E0[16] ? tVal0_F1 : tVal0_F0, tVal0_E0[15:0] };
	tVal1_C1 = { tVal0_E1[16] ? tVal0_F1 : tVal0_F0, tVal0_E1[15:0] };

//	tVal2 = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
//	tVal2_A = {
//		 tVal1_A0[32] ? tVal1_B1[32:0] : tVal1_B0[32:0],
//		!tVal1_B0[32] ? tVal1_A1[31:0] : tVal1_A0[31:0] };

	tVal2_A = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
	tVal2 = { tVal2_A[63] ? tVal1_C1 : tVal1_C0, tVal2_A[63:0] };
end

endmodule

`endif

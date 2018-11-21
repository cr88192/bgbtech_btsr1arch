module Jx2ExCsAdd48F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */

input[63:0]		valA;
input[63:0]		valB;
output[63:0]	valC;

reg[16:0]		tVal0_A0;
reg[16:0]		tVal0_B0;
reg[16:0]		tVal0_B1;
reg[16:0]		tVal0_C0;
reg[16:0]		tVal0_C1;
reg[16:0]		tVal0_D0;
reg[16:0]		tVal0_D1;

reg[32:0]		tVal1_A0;
reg[32:0]		tVal1_B0;
reg[32:0]		tVal1_B1;

reg[64:0]		tVal2;

assign valC = tVal2[63:0];

// /* verilator lint_on UNOPTFLAT */

always @*
begin

	tVal0_A0 = { 1'b0, valA[15: 0] } + { 1'b0, valB[15: 0] }    ;
	tVal0_B0 = { 1'b0, valA[31:16] } + { 1'b0, valB[31:16] } + 0;
	tVal0_B1 = { 1'b0, valA[31:16] } + { 1'b0, valB[31:16] } + 1;
	tVal0_C0 = { 1'b0, valA[47:32] } + { 1'b0, valB[47:32] } + 0;
	tVal0_C1 = { 1'b0, valA[47:32] } + { 1'b0, valB[47:32] } + 1;
//	tVal0_D0 = { 1'b0, valA[63:48] } + { 1'b0, valB[63:48] } + 0;
//	tVal0_D0 = { 1'b0, valA[63:48] };
//	tVal0_D0 = { 1'b0, valA[47]?UV16_FF:UV16_00 };
	tVal0_D0 = { 1'b0, UV16_00 };
	
	tVal1_A0 = { tVal0_A0[16] ? tVal0_B1 : tVal0_B0, tVal0_A0[15:0] };
//	tVal1_B0 = { tVal0_C0[16] ? tVal0_D1 : tVal0_D0, tVal0_C0[15:0] };
//	tVal1_B1 = { tVal0_C1[16] ? tVal0_D1 : tVal0_D0, tVal0_C1[15:0] };
	tVal1_B0 = { tVal0_D0, tVal0_C0[15:0] };
	tVal1_B1 = { tVal0_D0, tVal0_C1[15:0] };

	tVal2 = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
end

endmodule

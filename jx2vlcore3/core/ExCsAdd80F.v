`ifndef HAS_CSADD80F
`define HAS_CSADD80F

module ExCsAdd80F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[79:0]		valA;
input[79:0]		valB;
output[79:0]	valC;

reg[20:0]		tVal0_A0;
reg[20:0]		tVal0_A1;
reg[20:0]		tVal0_B0;
reg[20:0]		tVal0_B1;
reg[20:0]		tVal0_C0;
reg[20:0]		tVal0_C1;
reg[20:0]		tVal0_D0;
reg[20:0]		tVal0_D1;

reg[40:0]		tVal1_A0;
reg[40:0]		tVal1_A1;
reg[40:0]		tVal1_B0;
reg[40:0]		tVal1_B1;

reg[80:0]		tVal2;

assign valC = tVal2[79:0];

// /* verilator lint_on UNOPTFLAT */

always @*
begin

	tVal0_A0 = { 1'b0, valA[19: 0] } + { 1'b0, valB[19: 0] } + 0;
	tVal0_A1 = { 1'b0, valA[19: 0] } + { 1'b0, valB[19: 0] } + 1;
	tVal0_B0 = { 1'b0, valA[39:20] } + { 1'b0, valB[39:20] } + 0;
	tVal0_B1 = { 1'b0, valA[39:20] } + { 1'b0, valB[39:20] } + 1;
	tVal0_C0 = { 1'b0, valA[59:40] } + { 1'b0, valB[59:40] } + 0;
	tVal0_C1 = { 1'b0, valA[59:40] } + { 1'b0, valB[59:40] } + 1;
	tVal0_D0 = { 1'b0, valA[79:60] } + { 1'b0, valB[79:60] } + 0;
	tVal0_D1 = { 1'b0, valA[79:60] } + { 1'b0, valB[79:60] } + 1;
	
	tVal1_A0 = { tVal0_A0[20] ? tVal0_B1 : tVal0_B0, tVal0_A0[19:0] };
	tVal1_A1 = { tVal0_A1[20] ? tVal0_B1 : tVal0_B0, tVal0_A1[19:0] };
	tVal1_B0 = { tVal0_C0[20] ? tVal0_D1 : tVal0_D0, tVal0_C0[19:0] };
	tVal1_B1 = { tVal0_C1[20] ? tVal0_D1 : tVal0_D0, tVal0_C1[19:0] };

//	tVal2 = { tVal1_A0[40] ? tVal1_B1 : tVal1_B0, tVal1_A0[39:0] };
	tVal2 = {
		 tVal1_A0[40] ? tVal1_B1[40:0] : tVal1_B0[40:0],
		!tVal1_B0[40] ? tVal1_A1[39:0] : tVal1_A0[39:0] };
end

endmodule

`endif

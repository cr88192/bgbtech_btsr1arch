`ifndef HAS_CSADD72F
`define HAS_CSADD72F

module ExCsAdd72F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[71:0]		valA;
input[71:0]		valB;
output[71:0]	valC;

reg[18:0]		tVal0_A0;
reg[18:0]		tVal0_B0;
reg[18:0]		tVal0_B1;
reg[18:0]		tVal0_C0;
reg[18:0]		tVal0_C1;
reg[18:0]		tVal0_D0;
reg[18:0]		tVal0_D1;

reg[36:0]		tVal1_A0;
reg[36:0]		tVal1_B0;
reg[36:0]		tVal1_B1;

reg[72:0]		tVal2;

assign valC = tVal2[71:0];

// /* verilator lint_on UNOPTFLAT */

always @*
begin

	tVal0_A0 = { 1'b0, valA[17: 0] } + { 1'b0, valB[17: 0] }    ;
	tVal0_B0 = { 1'b0, valA[35:18] } + { 1'b0, valB[35:18] } + 0;
	tVal0_B1 = { 1'b0, valA[35:18] } + { 1'b0, valB[35:18] } + 1;
	tVal0_C0 = { 1'b0, valA[53:36] } + { 1'b0, valB[53:36] } + 0;
	tVal0_C1 = { 1'b0, valA[53:36] } + { 1'b0, valB[53:36] } + 1;
	tVal0_D0 = { 1'b0, valA[71:54] } + { 1'b0, valB[71:54] } + 0;
	tVal0_D1 = { 1'b0, valA[71:54] } + { 1'b0, valB[71:54] } + 1;
	
	tVal1_A0 = { tVal0_A0[18] ? tVal0_B1 : tVal0_B0, tVal0_A0[17:0] };
	tVal1_B0 = { tVal0_C0[18] ? tVal0_D1 : tVal0_D0, tVal0_C0[17:0] };
	tVal1_B1 = { tVal0_C1[18] ? tVal0_D1 : tVal0_D0, tVal0_C1[17:0] };

	tVal2 = { tVal1_A0[36] ? tVal1_B1 : tVal1_B0, tVal1_A0[35:0] };
end

endmodule

`endif

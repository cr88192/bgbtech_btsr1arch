/*
Add two 72 bit values with carry (73 bit).
 */

`ifndef HAS_CSADD72C
`define HAS_CSADD72C

module ExCsAdd72C(valA, valB, valC, cin);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[71:0]		valA;
input[71:0]		valB;
output[72:0]	valC;
intput			cin;

reg[12:0]		tVal0_A0;
reg[12:0]		tVal0_A1;
reg[12:0]		tVal0_B0;
reg[12:0]		tVal0_B1;
reg[12:0]		tVal0_C0;
reg[12:0]		tVal0_C1;
reg[12:0]		tVal0_D0;
reg[12:0]		tVal0_D1;
reg[12:0]		tVal0_E0;
reg[12:0]		tVal0_E1;
reg[12:0]		tVal0_F0;
reg[12:0]		tVal0_F1;

reg[24:0]		tVal1_A0;
reg[24:0]		tVal1_A1;
reg[24:0]		tVal1_B0;
reg[24:0]		tVal1_B1;
reg[24:0]		tVal1_C0;
reg[24:0]		tVal1_C1;

reg[1:0]		tCa1_A0;
reg[1:0]		tCa1_A1;
reg[1:0]		tCa1_B0;
reg[1:0]		tCa1_B1;
reg[1:0]		tCa1_C0;
reg[1:0]		tCa1_C1;

reg[3:0]		tCa2_A0;
reg[3:0]		tCa2_A1;
reg[1:0]		tCa2_B0;
reg[1:0]		tCa2_B1;

reg[5:0]		tCa3_A0;
reg[5:0]		tCa3_A1;
reg[6:0]		tCa3;

reg[48:0]		tVal2A;
reg[72:0]		tVal2;

assign valC = tVal2[72:0];

always @*
begin

	tVal0_A0 = { 1'b0, valA[11: 0] } + { 1'b0, valB[11: 0] } + 0;
	tVal0_A1 = { 1'b0, valA[11: 0] } + { 1'b0, valB[11: 0] } + 1;
	tVal0_B0 = { 1'b0, valA[23:12] } + { 1'b0, valB[23:12] } + 0;
	tVal0_B1 = { 1'b0, valA[23:12] } + { 1'b0, valB[23:12] } + 1;
	tVal0_C0 = { 1'b0, valA[35:24] } + { 1'b0, valB[35:24] } + 0;
	tVal0_C1 = { 1'b0, valA[35:24] } + { 1'b0, valB[35:24] } + 1;
	tVal0_D0 = { 1'b0, valA[47:36] } + { 1'b0, valB[47:36] } + 0;
	tVal0_D1 = { 1'b0, valA[47:36] } + { 1'b0, valB[47:36] } + 1;
	tVal0_E0 = { 1'b0, valA[59:48] } + { 1'b0, valB[59:48] } + 0;
	tVal0_E1 = { 1'b0, valA[59:48] } + { 1'b0, valB[59:48] } + 1;
	tVal0_F0 = { 1'b0, valA[71:60] } + { 1'b0, valB[71:60] } + 0;
	tVal0_F1 = { 1'b0, valA[71:60] } + { 1'b0, valB[71:60] } + 1;
	
// `ifdef def_true
`ifndef def_true
	tVal1_A0 = { tVal0_A0[12] ? tVal0_B1 : tVal0_B0, tVal0_A0[11:0] };
	tVal1_A1 = { tVal0_A1[12] ? tVal0_B1 : tVal0_B0, tVal0_A1[11:0] };
	tVal1_B0 = { tVal0_C0[12] ? tVal0_D1 : tVal0_D0, tVal0_C0[11:0] };
	tVal1_B1 = { tVal0_C1[12] ? tVal0_D1 : tVal0_D0, tVal0_C1[11:0] };
	tVal1_C0 = { tVal0_E0[12] ? tVal0_F1 : tVal0_F0, tVal0_E0[11:0] };
	tVal1_C1 = { tVal0_E1[12] ? tVal0_F1 : tVal0_F0, tVal0_E1[11:0] };

	tVal2A = { tVal1_A0[24] ? tVal1_B1 : tVal1_B0, tVal1_A0[23:0] };
	tVal2 = { tVal2A[48] ? tVal1_C1 : tVal1_C0, tVal2A[47:0] };
`endif

// `ifndef def_true
`ifdef def_true
	tCa1_A0 = { tVal0_A0[12] ? tVal0_B1[12] : tVal0_B0[12], tVal0_A0[12] };
	tCa1_A1 = { tVal0_A1[12] ? tVal0_B1[12] : tVal0_B0[12], tVal0_A1[12] };
	tCa1_B0 = { tVal0_C0[12] ? tVal0_D1[12] : tVal0_D0[12], tVal0_C0[12] };
	tCa1_B1 = { tVal0_C1[12] ? tVal0_D1[12] : tVal0_D0[12], tVal0_C1[12] };
	tCa1_C0 = { tVal0_E0[12] ? tVal0_F1[12] : tVal0_F0[12], tVal0_E0[12] };
	tCa1_C1 = { tVal0_E1[12] ? tVal0_F1[12] : tVal0_F0[12], tVal0_E1[12] };

	tCa2_A0 = { tCa1_A0[1] ? tCa1_B1 : tCa1_B0, tCa1_A0 };
	tCa2_A1 = { tCa1_A1[1] ? tCa1_B1 : tCa1_B0, tCa1_A1 };
	tCa2_B0 = tCa1_C0;
	tCa2_B1 = tCa1_C1;

	tCa3_A0 = { tCa2_A0[3] ? tCa2_B1 : tCa2_B0, tCa2_A0 };
	tCa3_A1 = { tCa2_A1[3] ? tCa2_B1 : tCa2_B0, tCa2_A1 };
	tCa3 = { cin ? tCa3_A1 : tCa3_A0, cin };

	tVal2 = {
		tCa3[6],
		tCa3[5] ? tVal0_F1[11:0] : tVal0_F0[11:0],
		tCa3[4] ? tVal0_E1[11:0] : tVal0_E0[11:0],
		tCa3[3] ? tVal0_D1[11:0] : tVal0_D0[11:0],
		tCa3[2] ? tVal0_C1[11:0] : tVal0_C0[11:0],
		tCa3[1] ? tVal0_B1[11:0] : tVal0_B0[11:0],
		tCa3[0] ? tVal0_A1[11:0] : tVal0_A0[11:0]
	 };
`endif
end

endmodule

`endif

`ifndef HAS_CSADD90CI
`define HAS_CSADD90CI

module ExCsAdd90CI(valA, valB, valC, valCi, cin);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[89:0]		valA;
input[89:0]		valB;
output[90:0]	valC;
output[90:0]	valCi;
input			cin;

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

reg[1:0]		tCa1_A0;
reg[1:0]		tCa1_A1;
reg[1:0]		tCa1_B0;
reg[1:0]		tCa1_B1;
reg[1:0]		tCa1_C0;
reg[1:0]		tCa1_C1;

reg[3:0]		tCa2_A0;
reg[3:0]		tCa2_A1;
// reg[4:0]		tCa2;

reg[5:0]		tCa3_A0;
reg[5:0]		tCa3_A1;
reg[6:0]		tCa3;


reg[64:0]		tVal2_A;
reg[90:0]		tVal2;


reg[16:0]		tVal0i_A0;
reg[16:0]		tVal0i_A1;
reg[16:0]		tVal0i_B0;
reg[16:0]		tVal0i_B1;
reg[16:0]		tVal0i_C0;
reg[16:0]		tVal0i_C1;
reg[16:0]		tVal0i_D0;
reg[16:0]		tVal0i_D1;
reg[16:0]		tVal0i_E0;
reg[16:0]		tVal0i_E1;
reg[10:0]		tVal0i_F0;
reg[10:0]		tVal0i_F1;

reg[32:0]		tVal1i_A0;
reg[32:0]		tVal1i_A1;
reg[32:0]		tVal1i_B0;
reg[32:0]		tVal1i_B1;
reg[26:0]		tVal1i_C0;
reg[26:0]		tVal1i_C1;

reg[1:0]		tCa1i_A0;
reg[1:0]		tCa1i_A1;
reg[1:0]		tCa1i_B0;
reg[1:0]		tCa1i_B1;
reg[1:0]		tCa1i_C0;
reg[1:0]		tCa1i_C1;

reg[3:0]		tCa2i_A0;
reg[3:0]		tCa2i_A1;
// reg[4:0]		tCa2i;

reg[5:0]		tCa3i_A0;
reg[5:0]		tCa3i_A1;
reg[6:0]		tCa3i;

reg[64:0]		tVal2i_A;
reg[90:0]		tVal2i;

assign valC = tVal2[90:0];
assign valCi = tVal2i[90:0];

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

	tVal0i_A0 = ~tVal0_A0;
	tVal0i_A1 = ~tVal0_A1;
	tVal0i_B0 = ~tVal0_B0;
	tVal0i_B1 = ~tVal0_B1;
	tVal0i_C0 = ~tVal0_C0;
	tVal0i_C1 = ~tVal0_C1;
	tVal0i_D0 = ~tVal0_D0;
	tVal0i_D1 = ~tVal0_D1;
	tVal0i_E0 = ~tVal0_E0;
	tVal0i_E1 = ~tVal0_E1;
	tVal0i_F0 = ~tVal0_F0;
	tVal0i_F1 = ~tVal0_F1;


	tCa1_A0 = { tVal0_A0[16] ? tVal0_B1[16] : tVal0_B0[16], tVal0_A0[16] };
	tCa1_A1 = { tVal0_A1[16] ? tVal0_B1[16] : tVal0_B0[16], tVal0_A1[16] };
	tCa1_B0 = { tVal0_C0[16] ? tVal0_D1[16] : tVal0_D0[16], tVal0_C0[16] };
	tCa1_B1 = { tVal0_C1[16] ? tVal0_D1[16] : tVal0_D0[16], tVal0_C1[16] };
	tCa1_C0 = { tVal0_E0[16] ? tVal0_F1[10] : tVal0_F0[10], tVal0_E0[16] };
	tCa1_C1 = { tVal0_E1[16] ? tVal0_F1[10] : tVal0_F0[10], tVal0_E1[16] };

	tCa2_A0 = { tCa1_A0[1] ? tCa1_B1 : tCa1_B0, tCa1_A0 };
	tCa2_A1 = { tCa1_A1[1] ? tCa1_B1 : tCa1_B0, tCa1_A1 };
//	tCa2 = { cin ? tCa2_A1 : tCa2_A0, cin };

	tCa3_A0 = { tCa2_A0[3] ? tCa1_C1 : tCa1_C0, tCa2_A0 };
	tCa3_A1 = { tCa2_A1[3] ? tCa1_C1 : tCa1_C0, tCa2_A1 };
	tCa3 = { cin ? tCa3_A1 : tCa3_A0, cin };


	tCa1i_A0 = { tVal0i_A0[16] ? tVal0i_B1[16] : tVal0i_B0[16], tVal0i_A0[16] };
	tCa1i_A1 = { tVal0i_A1[16] ? tVal0i_B1[16] : tVal0i_B0[16], tVal0i_A1[16] };
	tCa1i_B0 = { tVal0i_C0[16] ? tVal0i_D1[16] : tVal0i_D0[16], tVal0i_C0[16] };
	tCa1i_B1 = { tVal0i_C1[16] ? tVal0i_D1[16] : tVal0i_D0[16], tVal0i_C1[16] };
	tCa1i_C0 = { tVal0i_E0[16] ? tVal0i_F1[10] : tVal0i_F0[10], tVal0i_E0[16] };
	tCa1i_C1 = { tVal0i_E1[16] ? tVal0i_F1[10] : tVal0i_F0[10], tVal0i_E1[16] };

	tCa2i_A0 = { tCa1i_A0[1] ? tCa1i_B1 : tCa1i_B0, tCa1i_A0 };
	tCa2i_A1 = { tCa1i_A1[1] ? tCa1i_B1 : tCa1i_B0, tCa1i_A1 };

	tCa3i_A0 = { tCa2i_A0[3] ? tCa1i_C1 : tCa1i_C0, tCa2i_A0 };
	tCa3i_A1 = { tCa2i_A1[3] ? tCa1i_C1 : tCa1i_C0, tCa2i_A1 };
	tCa3i = { cin ? tCa3i_A1 : tCa3i_A0, cin };

	tVal2 = {
		tCa3[6],
		tCa3[5] ? tVal0_F1[ 9:0] : tVal0_F0[ 9:0],
		tCa3[4] ? tVal0_E1[15:0] : tVal0_E0[15:0],
		tCa3[3] ? tVal0_D1[15:0] : tVal0_D0[15:0],
		tCa3[2] ? tVal0_C1[15:0] : tVal0_C0[15:0],
		tCa3[1] ? tVal0_B1[15:0] : tVal0_B0[15:0],
		tCa3[0] ? tVal0_A1[15:0] : tVal0_A0[15:0]
	};

	tVal2i = {
		tCa3i[6],
		tCa3i[5] ? tVal0i_F1[ 9:0] : tVal0i_F0[ 9:0],
		tCa3i[4] ? tVal0i_E1[15:0] : tVal0i_E0[15:0],
		tCa3i[3] ? tVal0i_D1[15:0] : tVal0i_D0[15:0],
		tCa3i[2] ? tVal0i_C1[15:0] : tVal0i_C0[15:0],
		tCa3i[1] ? tVal0i_B1[15:0] : tVal0i_B0[15:0],
		tCa3i[0] ? tVal0i_A1[15:0] : tVal0i_A0[15:0]
	};

end

endmodule

`endif

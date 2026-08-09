/*
Add two 108 bit values with carry-out (109 bit).
 */

`ifndef HAS_CSADD108C
`define HAS_CSADD108C

module ExCsAdd108C(valA, valB, valC, valCIn);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[107:0]		valA;
input[107:0]		valB;
output[108:0]		valC;
input				valCIn;

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
reg[12:0]		tVal0_G0;
reg[12:0]		tVal0_G1;
reg[12:0]		tVal0_H0;
reg[12:0]		tVal0_H1;
reg[12:0]		tVal0_I0;
reg[12:0]		tVal0_I1;

reg[24:0]		tVal1_A0;
reg[24:0]		tVal1_A1;
reg[24:0]		tVal1_B0;
reg[24:0]		tVal1_B1;
reg[24:0]		tVal1_C0;
reg[24:0]		tVal1_C1;
reg[24:0]		tVal1_D0;
reg[24:0]		tVal1_D1;
reg[12:0]		tVal1_E0;
reg[12:0]		tVal1_E1;

reg[48:0]		tVal2_A0;
reg[48:0]		tVal2_A1;
reg[48:0]		tVal2_B0;
reg[48:0]		tVal2_B1;

reg[96:0]		tVal3A;
reg[108:0]		tVal3;

reg[9:0]		tValSel_A0;
reg[9:0]		tValSel_A1;
reg[9:0]		tValSel_B0;
reg[9:0]		tValSel_B1;
reg[9:0]		tValSel_C0;
reg[9:0]		tValSel_C1;
reg[9:0]		tValSel_D0;
reg[9:0]		tValSel_D1;
reg[9:0]		tValSel_E;

assign valC = tVal3[108:0];

always @*
begin

	tVal0_A0 = { 1'b0, valA[11 : 0] } + { 1'b0, valB[11 : 0] } + 0;
	tVal0_A1 = { 1'b0, valA[11 : 0] } + { 1'b0, valB[11 : 0] } + 1;
	tVal0_B0 = { 1'b0, valA[23 :12] } + { 1'b0, valB[23 :12] } + 0;
	tVal0_B1 = { 1'b0, valA[23 :12] } + { 1'b0, valB[23 :12] } + 1;
	tVal0_C0 = { 1'b0, valA[35 :24] } + { 1'b0, valB[35 :24] } + 0;
	tVal0_C1 = { 1'b0, valA[35 :24] } + { 1'b0, valB[35 :24] } + 1;
	tVal0_D0 = { 1'b0, valA[47 :36] } + { 1'b0, valB[47 :36] } + 0;
	tVal0_D1 = { 1'b0, valA[47 :36] } + { 1'b0, valB[47 :36] } + 1;
	tVal0_E0 = { 1'b0, valA[59 :48] } + { 1'b0, valB[59 :48] } + 0;
	tVal0_E1 = { 1'b0, valA[59 :48] } + { 1'b0, valB[59 :48] } + 1;
	tVal0_F0 = { 1'b0, valA[71 :60] } + { 1'b0, valB[71 :60] } + 0;
	tVal0_F1 = { 1'b0, valA[71 :60] } + { 1'b0, valB[71 :60] } + 1;
	tVal0_G0 = { 1'b0, valA[83 :72] } + { 1'b0, valB[ 83:72] } + 0;
	tVal0_G1 = { 1'b0, valA[83 :72] } + { 1'b0, valB[ 83:72] } + 1;
	tVal0_H0 = { 1'b0, valA[95 :84] } + { 1'b0, valB[ 95:84] } + 0;
	tVal0_H1 = { 1'b0, valA[95 :84] } + { 1'b0, valB[ 95:84] } + 1;
	tVal0_I0 = { 1'b0, valA[107:96] } + { 1'b0, valB[107:96] } + 0;
	tVal0_I1 = { 1'b0, valA[107:96] } + { 1'b0, valB[107:96] } + 1;

	tValSel_A0 = {
		tVal0_I0[12], tVal0_H0[12], tVal0_G0[12],
		tVal0_F0[12], tVal0_E0[12], tVal0_D0[12],
		tVal0_C0[12], tVal0_B0[12], tVal0_A0[12],
		valCIn};
	tValSel_A1 = {
		tVal0_I1[12], tVal0_H1[12], tVal0_G1[12],
		tVal0_F1[12], tVal0_E1[12], tVal0_D1[12],
		tVal0_C1[12], tVal0_B1[12], tVal0_A1[12],
		valCIn};
	tValSel_B0 = {
		tValSel_A0[8] ? tValSel_A1[9] : tValSel_A0[9], tValSel_A0[8],
		tValSel_A0[6] ? tValSel_A1[7] : tValSel_A0[7], tValSel_A0[6],
		tValSel_A0[4] ? tValSel_A1[5] : tValSel_A0[5], tValSel_A0[4],
		tValSel_A0[2] ? tValSel_A1[3] : tValSel_A0[3], tValSel_A0[2],
		tValSel_A0[0] ? tValSel_A1[1] : tValSel_A0[1], tValSel_A0[0]
	};
	tValSel_B1 = {
		tValSel_A1[8] ? tValSel_A1[9] : tValSel_A0[9], tValSel_A1[8],
		tValSel_A1[6] ? tValSel_A1[7] : tValSel_A0[7], tValSel_A1[6],
		tValSel_A1[4] ? tValSel_A1[5] : tValSel_A0[5], tValSel_A1[4],
		tValSel_A1[2] ? tValSel_A1[3] : tValSel_A0[3], tValSel_A1[2],
		tValSel_A1[0] ? tValSel_A1[1] : tValSel_A0[1], tValSel_A1[0]
	};
	tValSel_C0 = {
		tValSel_B0[9:8],
		tValSel_B0[5] ? tValSel_B1[7:6] : tValSel_B0[7:6], tValSel_B0[5:4],
		tValSel_B0[1] ? tValSel_B1[3:2] : tValSel_B0[3:2], tValSel_B0[1:0]
	};
	tValSel_C1 = {
		tValSel_B1[9:8],
		tValSel_B1[5] ? tValSel_B1[7:6] : tValSel_B0[7:6], tValSel_B1[5:4],
		tValSel_B1[1] ? tValSel_B1[3:2] : tValSel_B0[3:2], tValSel_B1[1:0]
	};
	tValSel_D0 = {
		tValSel_C0[9:8],
		tValSel_C0[3] ? tValSel_C1[7:4] : tValSel_C0[7:4], tValSel_C0[3:0]
	};
	tValSel_D1 = {
		tValSel_C1[9:8],
		tValSel_C1[3] ? tValSel_C1[7:4] : tValSel_C0[7:4], tValSel_C1[3:0]
	};
	tValSel_E = {
		tValSel_D0[7] ? tValSel_D1[9:8] : tValSel_D0[9:8],
		tValSel_D0[7:0]
	};
	
	tVal3 = {
		tValSel_E[9],
		tValSel_E[8] ? tVal0_I1[11:0] : tVal0_I0[11:0],
		tValSel_E[7] ? tVal0_H1[11:0] : tVal0_H0[11:0],
		tValSel_E[6] ? tVal0_G1[11:0] : tVal0_G0[11:0],
		tValSel_E[5] ? tVal0_F1[11:0] : tVal0_F0[11:0],
		tValSel_E[4] ? tVal0_E1[11:0] : tVal0_E0[11:0],
		tValSel_E[3] ? tVal0_D1[11:0] : tVal0_D0[11:0],
		tValSel_E[2] ? tVal0_C1[11:0] : tVal0_C0[11:0],
		tValSel_E[1] ? tVal0_B1[11:0] : tVal0_B0[11:0],
		tValSel_E[0] ? tVal0_A1[11:0] : tVal0_A0[11:0]
	};

`ifdef nothing
	tVal1_A0 = { tVal0_A0[12] ? tVal0_B1 : tVal0_B0, tVal0_A0[11:0] };
	tVal1_A1 = { tVal0_A1[12] ? tVal0_B1 : tVal0_B0, tVal0_A1[11:0] };
	tVal1_B0 = { tVal0_C0[12] ? tVal0_D1 : tVal0_D0, tVal0_C0[11:0] };
	tVal1_B1 = { tVal0_C1[12] ? tVal0_D1 : tVal0_D0, tVal0_C1[11:0] };
	tVal1_C0 = { tVal0_E0[12] ? tVal0_F1 : tVal0_F0, tVal0_E0[11:0] };
	tVal1_C1 = { tVal0_E1[12] ? tVal0_F1 : tVal0_F0, tVal0_E1[11:0] };
	tVal1_D0 = { tVal0_G0[12] ? tVal0_H1 : tVal0_H0, tVal0_G0[11:0] };
	tVal1_D1 = { tVal0_G1[12] ? tVal0_H1 : tVal0_H0, tVal0_G1[11:0] };
	tVal1_E0 = tVal0_I0;
	tVal1_E1 = tVal0_I1;

	tVal2_A0 = { tVal1_A0[24] ? tVal1_B1 : tVal1_B0, tVal1_A0[23:0] };
	tVal2_A1 = { tVal1_A1[24] ? tVal1_B1 : tVal1_B0, tVal1_A1[23:0] };
	tVal2_B0 = { tVal1_C0[24] ? tVal1_D1 : tVal1_D0, tVal1_C0[23:0] };
	tVal2_B1 = { tVal1_C1[24] ? tVal1_D1 : tVal1_D0, tVal1_C1[23:0] };
	tVal2_C0 = tVal1_E0;
	tVal2_C1 = tVal1_E1;

	tVal3A = { tVal2_A0[48] ? tVal2_B1 : tVal2_B0, tVal2_A0[23:0] };
	tVal3 = { tVal3A[96] ? tVal2_C1 : tVal2_C0, tVal3A[95:0] };
`endif

end

endmodule

`endif

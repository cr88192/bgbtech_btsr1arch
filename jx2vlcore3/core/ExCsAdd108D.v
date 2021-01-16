/*
Add two 108 bit values with carry-out (109 bit).
 */

`ifndef HAS_CSADD108D
`define HAS_CSADD108D

module ExCsAdd108D(clock, hold, valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input				clock;
input				hold;
input[107:0]		valA;
input[107:0]		valB;
output[108:0]		valC;

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

reg[24:0]		tVal2_A0;
reg[24:0]		tVal2_A1;
reg[24:0]		tVal2_B0;
reg[24:0]		tVal2_B1;
reg[24:0]		tVal2_C0;
reg[24:0]		tVal2_C1;
reg[24:0]		tVal2_D0;
reg[24:0]		tVal2_D1;
reg[12:0]		tVal2_E0;
reg[12:0]		tVal2_E1;

reg[48:0]		tVal3_A0;
reg[48:0]		tVal3_A1;
reg[48:0]		tVal3_B0;
reg[48:0]		tVal3_B1;
reg[12:0]		tVal3_C0;
reg[12:0]		tVal3_C1;

reg[96:0]		tVal4A;
reg[108:0]		tVal4;

assign valC = tVal4[108:0];

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

	tVal3_A0 = { tVal2_A0[24] ? tVal2_B1 : tVal2_B0, tVal2_A0[23:0] };
	tVal3_A1 = { tVal2_A1[24] ? tVal2_B1 : tVal2_B0, tVal2_A1[23:0] };
	tVal3_B0 = { tVal2_C0[24] ? tVal2_D1 : tVal2_D0, tVal2_C0[23:0] };
	tVal3_B1 = { tVal2_C1[24] ? tVal2_D1 : tVal2_D0, tVal2_C1[23:0] };
	tVal3_C0 = tVal2_E0;
	tVal3_C1 = tVal2_E1;

	tVal4A = { tVal3_A0[48] ? tVal3_B1 : tVal3_B0, tVal3_A0[47:0] };
	tVal4 = { tVal4A[96] ? tVal3_C1 : tVal3_C0, tVal4A[95:0] };
end

always @(posedge clock)
begin
	if(!hold)
	begin
		tVal2_A0	<= tVal1_A0;
		tVal2_A1	<= tVal1_A1;
		tVal2_B0	<= tVal1_B0;
		tVal2_B1	<= tVal1_B1;
		tVal2_C0	<= tVal1_C0;
		tVal2_C1	<= tVal1_C1;
		tVal2_D0	<= tVal1_D0;
		tVal2_D1	<= tVal1_D1;
		tVal2_E0	<= tVal1_E0;
		tVal2_E1	<= tVal1_E1;
	end
end

endmodule

`endif

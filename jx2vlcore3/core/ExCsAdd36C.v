/*
Add two 36 bit values with carry (37 bit output).
 */

`ifndef HAS_CSADD36C
`define HAS_CSADD36C

module ExCsAdd36C(valA, valB, valC, cin);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[35:0]		valA;
input[35:0]		valB;
output[36:0]	valC;
input			cin;

reg[12:0]		tVal0_A0;
reg[12:0]		tVal0_A1;
reg[12:0]		tVal0_B0;
reg[12:0]		tVal0_B1;
reg[12:0]		tVal0_C0;
reg[12:0]		tVal0_C1;

reg[24:0]		tVal1_A0;
reg[24:0]		tVal1_A1;
reg[12:0]		tVal1_B0;
reg[12:0]		tVal1_B1;

reg[1:0]		tCa1_A0;
reg[1:0]		tCa1_A1;
reg[1:0]		tCa1_B0;
reg[1:0]		tCa1_B1;

reg[3:0]		tCa2_A0;
reg[3:0]		tCa2_A1;
reg[4:0]		tCa2;


reg[36:0]		tVal2;

assign valC = tVal2[36:0];

always @*
begin

	tVal0_A0 = { 1'b0, valA[11: 0] } + { 1'b0, valB[11: 0] } + 0;
	tVal0_A1 = { 1'b0, valA[11: 0] } + { 1'b0, valB[11: 0] } + 1;
	tVal0_B0 = { 1'b0, valA[23:12] } + { 1'b0, valB[23:12] } + 0;
	tVal0_B1 = { 1'b0, valA[23:12] } + { 1'b0, valB[23:12] } + 1;
	tVal0_C0 = { 1'b0, valA[35:24] } + { 1'b0, valB[35:24] } + 0;
	tVal0_C1 = { 1'b0, valA[35:24] } + { 1'b0, valB[35:24] } + 1;
	
//	tVal1_A0 = { tVal0_A0[12] ? tVal0_B1 : tVal0_B0, tVal0_A0[11:0] };
//	tVal1_A1 = { tVal0_A1[12] ? tVal0_B1 : tVal0_B0, tVal0_A1[11:0] };
//	tVal1_B0 = tVal0_C0;
//	tVal1_B1 = tVal0_C1;

//	tVal2 = { tVal1_A0[24] ? tVal1_B1 : tVal1_B0, tVal1_A0[23:0] };

	tCa1_A0 = { tVal0_A0[12] ? tVal0_B1[12] : tVal0_B0[12], tVal0_A0[12] };
	tCa1_A1 = { tVal0_A1[12] ? tVal0_B1[12] : tVal0_B0[12], tVal0_A1[12] };
	tCa1_B0 = { 1'b0, tVal0_C0[12] };
	tCa1_B1 = { 1'b0, tVal0_C1[12] };

	tCa2_A0 = { tCa1_A0[1] ? tCa1_B1 : tCa1_B0, tCa1_A0 };
	tCa2_A1 = { tCa1_A1[1] ? tCa1_B1 : tCa1_B0, tCa1_A1 };
	tCa2 = { cin ? tCa2_A1 : tCa2_A0, cin };

	tVal2 = {
		tCa2[3],
		tCa2[2] ? tVal0_C1[11:0] : tVal0_C0[11:0],
		tCa2[1] ? tVal0_B1[11:0] : tVal0_B0[11:0],
		tCa2[0] ? tVal0_A1[11:0] : tVal0_A0[11:0]
	};

end

endmodule

`endif

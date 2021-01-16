`ifndef HAS_CSADD64F
`define HAS_CSADD64F

module ExCsAdd64F(valA, valB, valC);

// /* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSED */

input[63:0]		valA;
input[63:0]		valB;
output[63:0]	valC;

reg[16:0]		tVal0_A0;
reg[16:0]		tVal0_A1;
reg[16:0]		tVal0_B0;
reg[16:0]		tVal0_B1;
reg[16:0]		tVal0_C0;
reg[16:0]		tVal0_C1;
reg[16:0]		tVal0_D0;
reg[16:0]		tVal0_D1;

reg[32:0]		tVal1_A0;
reg[32:0]		tVal1_A1;
reg[32:0]		tVal1_B0;
reg[32:0]		tVal1_B1;

reg[1:0]		tCa1_A0;
reg[1:0]		tCa1_A1;
reg[1:0]		tCa1_B0;
reg[1:0]		tCa1_B1;

reg[3:0]		tCa2_A0;
reg[3:0]		tCa2_A1;
reg[4:0]		tCa2;

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
	tVal0_D0 = { 1'b0, valA[63:48] } + { 1'b0, valB[63:48] } + 0;
	tVal0_D1 = { 1'b0, valA[63:48] } + { 1'b0, valB[63:48] } + 1;
	
// `ifdef def_true
`ifndef def_true
	tVal1_A0 = { tVal0_A0[16] ? tVal0_B1 : tVal0_B0, tVal0_A0[15:0] };
	tVal1_B0 = { tVal0_C0[16] ? tVal0_D1 : tVal0_D0, tVal0_C0[15:0] };
	tVal1_B1 = { tVal0_C1[16] ? tVal0_D1 : tVal0_D0, tVal0_C1[15:0] };

	tVal2 = { tVal1_A0[32] ? tVal1_B1 : tVal1_B0, tVal1_A0[31:0] };
`endif

// `ifndef def_true
`ifdef def_true
	tCa1_A0 = { tVal0_A0[16] ? tVal0_B1[16] : tVal0_B0[16], tVal0_A0[16] };
//	tCa1_A1 = { tVal0_A1[16] ? tVal0_B1[16] : tVal0_B0[16], tVal0_A1[16] };
	tCa1_B0 = { tVal0_C0[16] ? tVal0_D1[16] : tVal0_D0[16], tVal0_C0[16] };
	tCa1_B1 = { tVal0_C1[16] ? tVal0_D1[16] : tVal0_D0[16], tVal0_C1[16] };

	tCa2_A0 = { tCa1_A0[1] ? tCa1_B1 : tCa1_B0, tCa1_A0 };
//	tCa2_A1 = { tCa1_A1[1] ? tCa1_B1 : tCa1_B0, tCa1_A1 };
	tCa2 = { tCa2_A0, 1'b0 };

	tVal2 = {
		tCa2[4],
		tCa2[3] ? tVal0_D1[15:0] : tVal0_D0[15:0],
		tCa2[2] ? tVal0_C1[15:0] : tVal0_C0[15:0],
		tCa2[1] ? tVal0_B1[15:0] : tVal0_B0[15:0],
								   tVal0_A0[15:0]
	};
`endif
end

endmodule

`endif

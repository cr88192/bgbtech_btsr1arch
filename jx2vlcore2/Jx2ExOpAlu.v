module Jx2ExOpAlu(
	/* verilator lint_off UNUSED */
	clock,		reset,

	regValRm,		regValRn,
	regValRi,

	addRmRi_A0,		addRmRi_A1,
	addRmRi_B0,		addRmRi_B1,
	subRmRi_A0,		subRmRi_A1,
	subRmRi_B0,		subRmRi_B1,

	aluPAddW,		aluPAddL,
	aluPSubW,		aluPSubL
	);

input			clock;
input			reset;

input[63:0]		regValRm;		//Rm input value
input[63:0]		regValRn;		//Rn input value
input[63:0]		regValRi;		//Ri input value

output[32:0]	addRmRi_A0;
output[32:0]	addRmRi_A1;
output[32:0]	addRmRi_B0;
output[32:0]	addRmRi_B1;
output[32:0]	subRmRi_A0;	
output[32:0]	subRmRi_A1;
output[32:0]	subRmRi_B0;
output[32:0]	subRmRi_B1;

output[63:0]	aluPAddW;
output[63:0]	aluPAddL;
output[63:0]	aluPSubW;
output[63:0]	aluPSubL;


reg[63:0]		regValRm_T0;
reg[63:0]		regValRm_T1;
reg[63:0]		regValRi_T0;
reg[63:0]		regValRi_T1;

reg[16:0]		tAdd0RmRi_A0;
reg[16:0]		tAdd0RmRi_A1;
reg[16:0]		tAdd0RmRi_B0;
reg[16:0]		tAdd0RmRi_B1;
reg[16:0]		tAdd0RmRi_C0;
reg[16:0]		tAdd0RmRi_C1;
reg[16:0]		tAdd0RmRi_D0;
reg[16:0]		tAdd0RmRi_D1;

reg[16:0]		tSub0RmRi_A0;
reg[16:0]		tSub0RmRi_A1;
reg[16:0]		tSub0RmRi_B0;
reg[16:0]		tSub0RmRi_B1;
reg[16:0]		tSub0RmRi_C0;
reg[16:0]		tSub0RmRi_C1;
reg[16:0]		tSub0RmRi_D0;
reg[16:0]		tSub0RmRi_D1;


reg[32:0]		tAddRmRi_A0;
reg[32:0]		tAddRmRi_A1;
reg[32:0]		tAddRmRi_B0;
reg[32:0]		tAddRmRi_B1;
reg[32:0]		tSubRmRi_A0;
reg[32:0]		tSubRmRi_A1;
reg[32:0]		tSubRmRi_B0;
reg[32:0]		tSubRmRi_B1;

assign	addRmRi_A0 = tAddRmRi_A0;
assign	addRmRi_A1 = tAddRmRi_A1;
assign	addRmRi_B0 = tAddRmRi_B0;
assign	addRmRi_B1 = tAddRmRi_B1;
assign	subRmRi_A0 = tSubRmRi_A0;	
assign	subRmRi_A1 = tSubRmRi_A1;
assign	subRmRi_B0 = tSubRmRi_B0;
assign	subRmRi_B1 = tSubRmRi_B1;

reg[63:0]		tAluPAddW;
reg[63:0]		tAluPAddL;
reg[63:0]		tAluPSubW;
reg[63:0]		tAluPSubL;

assign	aluPAddW = tAluPAddW;
assign	aluPAddL = tAluPAddL;
assign	aluPSubW = tAluPSubW;
assign	aluPSubL = tAluPSubL;


always @*
begin
	regValRm_T0 = regValRm;
	regValRm_T1 = regValRm;
	regValRi_T0 = regValRi;
	regValRi_T1 = regValRi;


	tAdd0RmRi_A0 =
		{ 1'b0, regValRm_T0[15:0] } +
		{ 1'b0, regValRi_T0[15:0] } + 0;
	tAdd0RmRi_A1 =
		{ 1'b0, regValRm_T0[15:0] } +
		{ 1'b0, regValRi_T0[15:0] } + 1;
	tAdd0RmRi_B0 =
		{ 1'b0, regValRm_T0[31:16] } +
		{ 1'b0, regValRi_T0[31:16] } + 0;
	tAdd0RmRi_B1 =
		{ 1'b0, regValRm_T0[31:16] } +
		{ 1'b0, regValRi_T0[31:16] } + 1;
	tAdd0RmRi_C0 =
		{ 1'b0, regValRm_T0[47:32] } +
		{ 1'b0, regValRi_T0[47:32] } + 0;
	tAdd0RmRi_C1 =
		{ 1'b0, regValRm_T0[47:32] } +
		{ 1'b0, regValRi_T0[47:32] } + 1;
	tAdd0RmRi_D0 =
		{ 1'b0, regValRm_T0[63:48] } +
		{ 1'b0, regValRi_T0[63:48] } + 0;
	tAdd0RmRi_D1 =
		{ 1'b0, regValRm_T0[63:48] } +
		{ 1'b0, regValRi_T0[63:48] } + 1;


	tSub0RmRi_A0 =
		{ 1'b0,  regValRm_T1[15:0] } +
		{ 1'b0, ~regValRi_T1[15:0] } + 0;
	tSub0RmRi_A1 =
		{ 1'b0,  regValRm_T1[15:0] } +
		{ 1'b0, ~regValRi_T1[15:0] } + 1;
	tSub0RmRi_B0 =
		{ 1'b0,  regValRm_T1[31:16] } +
		{ 1'b0, ~regValRi_T1[31:16] } + 0;
	tSub0RmRi_B1 =
		{ 1'b0,  regValRm_T1[31:16] } +
		{ 1'b0, ~regValRi_T1[31:16] } + 1;
	tSub0RmRi_C0 =
		{ 1'b0,  regValRm_T1[47:32] } +
		{ 1'b0, ~regValRi_T1[47:32] } + 0;
	tSub0RmRi_C1 =
		{ 1'b0,  regValRm_T1[47:32] } +
		{ 1'b0, ~regValRi_T1[47:32] } + 1;
	tSub0RmRi_D0 =
		{ 1'b0,  regValRm_T1[63:48] } +
		{ 1'b0, ~regValRi_T1[63:48] } + 0;
	tSub0RmRi_D1 =
		{ 1'b0,  regValRm_T1[63:48] } +
		{ 1'b0, ~regValRi_T1[63:48] } + 1;


	tAddRmRi_A0 = {
		tAdd0RmRi_A0[16] ? tAdd0RmRi_B1 : tAdd0RmRi_B0,
		tAdd0RmRi_A0[15:0] };
	tAddRmRi_A1 = {
		tAdd0RmRi_A1[16] ? tAdd0RmRi_B1 : tAdd0RmRi_B0,
		tAdd0RmRi_A1[15:0] };
	tAddRmRi_B0 = {
		tAdd0RmRi_C0[16] ? tAdd0RmRi_D1 : tAdd0RmRi_D0,
		tAdd0RmRi_C0[15:0] };
	tAddRmRi_B1 = {
		tAdd0RmRi_C1[16] ? tAdd0RmRi_D1 : tAdd0RmRi_D0,
		tAdd0RmRi_C1[15:0] };


	tSubRmRi_A0 = {
		tSub0RmRi_A0[16] ? tSub0RmRi_B1 : tSub0RmRi_B0,
		tSub0RmRi_A0[15:0] };
	tSubRmRi_A1 = {
		tSub0RmRi_A1[16] ? tSub0RmRi_B1 : tSub0RmRi_B0,
		tSub0RmRi_A1[15:0] };
	tSubRmRi_B0 = {
		tSub0RmRi_C0[16] ? tSub0RmRi_D1 : tSub0RmRi_D0,
		tSub0RmRi_C0[15:0] };
	tSubRmRi_B1 = {
		tSub0RmRi_C1[16] ? tSub0RmRi_D1 : tSub0RmRi_D0,
		tSub0RmRi_C1[15:0] };


	tAluPAddW = {
		tAdd0RmRi_D0[15:0], tAdd0RmRi_C0[15:0],
		tAdd0RmRi_B0[15:0], tAdd0RmRi_A0[15:0] };
	tAluPSubW = {
		tSub0RmRi_D1[15:0], tSub0RmRi_C1[15:0],
		tSub0RmRi_B1[15:0], tSub0RmRi_A1[15:0] };
	tAluPAddL = {
		tAddRmRi_B0[31:0], tAddRmRi_A0[31:0] };
	tAluPSubL = {
		tSubRmRi_B1[31:0], tSubRmRi_A1[31:0] };

end

endmodule
`ifndef HAS_RGB30AEXP
`define HAS_RGB30AEXP

`include "ExConv_Fp8Exp12.v"

module ExConv_Rgb30aExp(
	/* verilator lint_off UNUSED */
	regValRs,	tagFmt,
	regOutVal
	);

input[31:0]		regValRs;
input[1:0]		tagFmt;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

wire[7:0]	tFp8A_R;
wire[7:0]	tFp8A_G;
wire[7:0]	tFp8A_B;
wire[7:0]	tFp8A_A;

assign	tFp8A_A	= { regValRs[29:22]       };
assign	tFp8A_R	= { regValRs[21:15], 1'b0 };
assign	tFp8A_G	= { regValRs[14: 7]       };
assign	tFp8A_B	= { regValRs[ 6: 0], 1'b0 };

wire[11:0]	tExpA_R;
wire[11:0]	tExpA_G;
wire[11:0]	tExpA_B;
wire[11:0]	tExpA_A;

wire		tExpIsSign;
assign		tExpIsSign = !tagFmt[0];

ExConv_Fp8Exp12		exp8_Ar(tFp8A_R, tExpA_R, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ag(tFp8A_G, tExpA_G, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ab(tFp8A_B, tExpA_B, tExpIsSign);
ExConv_Fp8Exp12		exp8_Aa(tFp8A_A, tExpA_A, tExpIsSign);

wire[11:0]	tLinA_R;
wire[11:0]	tLinA_G;
wire[11:0]	tLinA_B;

assign		tLinA_R = tExpIsSign ?
	{       regValRs[29:20], 2'b0 } :
	{ 1'b0, regValRs[29:20], 1'b0 };
assign		tLinA_G = tExpIsSign ?
	{       regValRs[19:10], 2'b0 } :
	{ 1'b0, regValRs[19:10], 1'b0 };
assign		tLinA_B = tExpIsSign ?
	{       regValRs[ 9: 0], 2'b0 } :
	{ 1'b0, regValRs[ 9: 0], 1'b0 };

reg[11:0]	tValA_R;
reg[11:0]	tValA_G;
reg[11:0]	tValA_B;
reg[11:0]	tValA_A;

always @*
begin
	if(tagFmt[1])
	begin
		tValA_R = tExpA_R;
		tValA_G = tExpA_G;
		tValA_B = tExpA_B;
		tValA_A = tExpA_A;
	end
	else
	begin
		tValA_R = tLinA_R;
		tValA_G = tLinA_G;
		tValA_B = tLinA_B;
		tValA_A = 12'h3C0;
	end
	
	tRegOutVal = {
		tValA_A, 4'h0, tValA_R, 4'h0,
		tValA_G, 4'h0, tValA_B, 4'h0 };
end

endmodule

`endif

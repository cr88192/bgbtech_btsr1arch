/*
MULx.W
 */

module ExMulW(
	clock,		reset,
	valRs,		valRt,
	idUCmd,		idUIxt,
	exHold,		valRn
	);

input			clock;
input			reset;

input[63:0]		valRs;
input[63:0]		valRt;
output[63:0]	valRn;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
input			exHold;

reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[7:0]		tIdUCmd;
reg[7:0]		tIdUIxt;
reg[7:0]		tIdUCmdB;
reg[7:0]		tIdUIxtB;

reg[63:0]		tValRs;
reg[63:0]		tValRt;

reg[31:0]		tValRn1A;
reg[31:0]		tValRn1B;
reg[31:0]		tValRn1C;
reg[31:0]		tValRn1D;

reg[31:0]		tValRsSxA;
reg[31:0]		tValRtSxA;
reg[31:0]		tValRsSxB;
reg[31:0]		tValRtSxB;
reg[31:0]		tValRsSxC;
reg[31:0]		tValRtSxC;
reg[31:0]		tValRsSxD;
reg[31:0]		tValRtSxD;

always @*
begin
	tIdUCmd	= idUCmd;
	tIdUIxt	= idUIxt;
	tValRs	= valRs;
	tValRt	= valRt;

`ifndef def_true
	if(tIdUIxt[0])
	begin
		tValRsSxA= { UV16_00, tValRs[15:0] };
		tValRtSxA= { UV16_00, tValRt[15:0] };
	end
	else
	begin
		tValRsSxA= { tValRs[15] ? UV16_FF : UV16_00, tValRs[15:0] };
		tValRtSxA= { tValRt[15] ? UV16_FF : UV16_00, tValRt[15:0] };
	end
`endif

	tValRsSxA={(tValRs[15] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRs[15: 0]};
	tValRtSxA={(tValRt[15] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRt[15: 0]};
	tValRsSxB={(tValRs[31] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRs[31:16]};
	tValRtSxB={(tValRt[31] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRt[31:16]};
	tValRsSxC={(tValRs[47] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRs[47:32]};
	tValRtSxC={(tValRt[47] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRt[47:32]};
	tValRsSxD={(tValRs[63] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRs[63:48]};
	tValRtSxD={(tValRt[63] && !tIdUIxt[0]) ? UV16_FF:UV16_00, tValRt[63:48]};

	case(tIdUIxtB[4:1])
		4'h1:
			tValRn = {
				(tValRn1A[31] && !tIdUIxtB[0]) ? UV32_FF : UV32_00,
				tValRn1A };
		4'h3:
			tValRn = {
				tValRn1B,
				tValRn1A };
		4'h4:
			tValRn = {
				tValRn1D[15: 0],
				tValRn1C[15: 0],
				tValRn1B[15: 0],
				tValRn1A[15: 0] };
		4'h5:
			tValRn = {
				tValRn1D[31:16],
				tValRn1C[31:16],
				tValRn1B[31:16],
				tValRn1A[31:16] };
		default:
			tValRn = UV64_00;
	endcase
end

always @(posedge clock)
begin
//	if(!exHold)
//	begin
//		tIdUCmd	<= idUCmd;
//		tIdUIxt	<= idUIxt;
//		tValRs	<= valRs;
//		tValRt	<= valRt;
//	end
	
	if(!exHold)
	begin
		tIdUCmdB	<= tIdUCmd;
		tIdUIxtB	<= tIdUIxt;
		tValRn1A	<= tValRsSxA * tValRtSxA;
		tValRn1B	<= tValRsSxB * tValRtSxB;
		tValRn1C	<= tValRsSxC * tValRtSxC;
		tValRn1D	<= tValRsSxD * tValRtSxD;
	end
end

endmodule

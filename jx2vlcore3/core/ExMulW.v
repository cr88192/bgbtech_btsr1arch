module ExMulW(
	clock,		reset,
	valRs,		valRt,
	idUCmd,		idUIxt,
	exHold,		valRn
	);

input	clock;
input	reset;

input[31:0]		valRs;
input[31:0]		valRt;
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

reg[31:0]		tValRs;
reg[31:0]		tValRt;
reg[31:0]		tValRn1;

reg[31:0]		tValRsSx;
reg[31:0]		tValRtSx;

always @*
begin
	if(tIdUIxt[0])
	begin
		tValRsSx= { UV16_00, tValRs[15:0] };
		tValRtSx= { UV16_00, tValRt[15:0] };
	end
	else
	begin
		tValRsSx= { tValRs[15] ? UV16_FF : UV16_00, tValRs[15:0] };
		tValRtSx= { tValRt[15] ? UV16_FF : UV16_00, tValRt[15:0] };
	end

	tValRn = {
		(tValRn1[31] && !tIdUIxtB[0]) ? UV32_FF : UV32_00,
		tValRn1 };
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tIdUCmd	<= idUCmd;
		tIdUIxt	<= idUIxt;
		tValRs	<= valRs;
		tValRt	<= valRt;
	end
	
	tIdUCmdB	<= tIdUCmd;
	tIdUIxtB	<= tIdUIxt;
	tValRn1		<= tValRsSx * tValRtSx;
end

endmodule

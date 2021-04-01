module DecOpHz(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	isAltOp,	istrJBits,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt,		idUFl
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
input[3:0]		isAltOp;
input[25:0]		istrJBits;

output[5:0]		idRegN;
output[5:0]		idRegM;
output[5:0]		idRegO;
output[32:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;
output[3:0]		idUFl;

reg[31:0]		hzistrWord;

DecOpFz	decOpFz(
	clock,		reset,
	{ UV32_XX, hzistrWord },
	4'h2,		istrJBits,
	idRegN,		idRegM,
	idRegO,		idImm,
	idUCmd,		idUIxt,
	idUFl
	);

always @*
begin
	hzistrWord = {
		((istrWord[15:12] == 4'h7) ? 4'h1 : 4'h0),
		istrWord[23:20],
		istrWord[19:16],
		istrWord[11: 8],
		8'hF0,
		istrWord[7:0] };
end

endmodule

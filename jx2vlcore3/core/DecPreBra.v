/*
Detect unconditional branch instructions for early branch.
Happens in parallel with the main ID1 stage.
*/

module DecPreBra(
	istrWord,
	istrBraPc,
	preBraPc,
	preIsBra);

input[63:0]		istrWord;	//Instruction Word
input[31:0]		istrBraPc;	//Branch Base PC

output[31:0]	preBraPc;
output			preIsBra;

reg[31:0]	tPreBraPc;
reg			tPreBra;
assign	preBraPc	= tPreBraPc;
assign	preIsBra	= tPreBra;


reg[31:0]	tBraDisp8;
reg[31:0]	tBraDisp20;

reg[31:0]	tDisp8;
reg[31:0]	tDisp20;

reg			tIsBra8;
reg			tIsBra20;


always @*
begin
	tPreBraPc	= UV32_XX;
	tPreBra		= 0;
	
	tDisp8		= { istrWord[7]?UV24_FF:UV24_00, istrWord[7:0] };
	tDisp20		= {
		istrWord[7]?UV12_FF:UV12_00,
		istrWord[7:0], istrWord[27:16] };
	
	tBraDisp8	= istrBraPc + { tDisp8[30:0], 1'b0 };
	tBraDisp20	= istrBraPc + { tDisp20[30:0], 1'b0 };
	
	tIsBra8		=
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b000);
	tIsBra20		=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) &&
		(istrWord[31:29]==3'b110);
		
	if(tIsBra8)
	begin
		tPreBraPc	= tBraDisp8;
		tPreBra		= 1;
	end
	if(tIsBra20)
	begin
		tPreBraPc	= tBraDisp20;
		tPreBra		= 1;
	end
end

endmodule
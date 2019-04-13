/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.

 */

`include "CoreDefs.v"

`include "DecOpBz.v"
`include "DecOpFz.v"
`include "DecOpFC.v"

module DecOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word

output[5:0]		idRegN;
output[5:0]		idRegM;
output[5:0]		idRegO;
output[32:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[5:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

wire[5:0]		decOpBz_idRegN;
wire[5:0]		decOpBz_idRegM;
wire[5:0]		decOpBz_idRegO;
wire[32:0]		decOpBz_idImm;
wire[7:0]		decOpBz_idUCmd;
wire[7:0]		decOpBz_idUIxt;

DecOpBz	decOpBz(
	clock,		reset,
	istrWord,
	decOpBz_idRegN,		decOpBz_idRegM,
	decOpBz_idRegO,		decOpBz_idImm,
	decOpBz_idUCmd,		decOpBz_idUIxt
	);


wire[5:0]		decOpFz_idRegN;
wire[5:0]		decOpFz_idRegM;
wire[5:0]		decOpFz_idRegO;
wire[32:0]		decOpFz_idImm;
wire[7:0]		decOpFz_idUCmd;
wire[7:0]		decOpFz_idUIxt;

DecOpFz	decOpFz(
	clock,		reset,
	istrWord,
	decOpFz_idRegN,		decOpFz_idRegM,
	decOpFz_idRegO,		decOpFz_idImm,
	decOpFz_idUCmd,		decOpFz_idUIxt
	);

wire[5:0]		decOpFC_idRegN;
wire[5:0]		decOpFC_idRegM;
wire[5:0]		decOpFC_idRegO;
wire[32:0]		decOpFC_idImm;
wire[7:0]		decOpFC_idUCmd;
wire[7:0]		decOpFC_idUIxt;

DecOpFC	decOpFC(
	clock,		reset,
	istrWord,
	decOpFC_idRegN,		decOpFC_idRegM,
	decOpFC_idRegO,		decOpFC_idImm,
	decOpFC_idUCmd,		decOpFC_idUIxt
	);

reg opIsFx;
reg opIsFz;
reg opIsFC;
reg opIsDz;
reg opIsDf;

always @*
begin

	casez(istrWord[15:10])
		6'b11100z: begin
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDf = istrWord[10];
		end
		6'b111010: begin
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDf = istrWord[9];
		end
		6'b111011: begin
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 1;
			opIsDf = istrWord[9];
		end

		6'b11110z: begin
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
		6'b110110: begin
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
		6'b110111: begin
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 0;
			opIsDf = 0;
		end

		default: begin
			opIsFx = 0;		opIsFz = 0;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
	endcase


	if(opIsFx)
	begin
		if(opIsFC)
		begin
			opRegN	= decOpFC_idRegN;
			opRegM	= decOpFC_idRegM;
			opRegO	= decOpFC_idRegO;
			opImm	= decOpFC_idImm;
			opUCmd	= decOpFC_idUCmd;
			opUIxt	= decOpFC_idUIxt;
		end
		else
		begin
			opRegN	= decOpFz_idRegN;
			opRegM	= decOpFz_idRegM;
			opRegO	= decOpFz_idRegO;
			opImm	= decOpFz_idImm;
			opUCmd	= decOpFz_idUCmd;
			opUIxt	= decOpFz_idUIxt;
		end
	end
	else
	begin
		opRegN	= decOpBz_idRegN;
		opRegM	= decOpBz_idRegM;
		opRegO	= decOpBz_idRegO;
		opImm	= decOpBz_idImm;
		opUCmd	= decOpBz_idUCmd;
		opUIxt	= decOpBz_idUIxt;
	end
	
	if(opIsDz)
	begin
//		opUIxt[7:6]=opIsDf?JX2_IXC_CF:JX2_IXC_CT;
		opUCmd[7:6]=opIsDf?JX2_IXC_CF:JX2_IXC_CT;
	end
	
//	if(opUCmd == JX2_UCMD_INVOP)
	if(opUCmd[5:0] == JX2_UCMD_INVOP)
	begin
		$display("DecOp: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
	end
end

endmodule

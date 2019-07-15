/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.

Support for 48-bit encodings is optional.

Ex block is predicated.
Fx block is unconditional.

Decoder at this stage does not care about WEX vs non-WEX.

 */

`include "CoreDefs.v"

`ifdef jx2_enable_ops16
`include "DecOpBz.v"
`endif

`include "DecOpFz.v"

`ifdef jx2_enable_ops48
`include "DecOpFC.v"
`endif

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


reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[5:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

`ifdef jx2_enable_ops16
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
`endif

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

`ifdef jx2_enable_ops48
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
`endif

reg opIsFx;
reg opIsFz;
reg opIsFC;
reg opIsDz;
reg opIsDf;

always @*
begin

	casez(istrWord[15:10])
		6'b11100z: begin	//E0..E7
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDf = istrWord[10];
		end
		6'b111010: begin	//E8..EB
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
//			opIsDf = istrWord[9];
			opIsDf = istrWord[8];
		end
		6'b111011: begin	//EC..EF
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 1;
			opIsDf = istrWord[9];
		end

		6'b11110z: begin	//F0..F7
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
		6'b111110: begin	//F8..FB
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
		6'b111111: begin	//FC..FF
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

`ifndef jx2_enable_ops16
	opIsFx = 1;
`endif


	if(opIsFx)
	begin
`ifdef jx2_enable_ops48
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
`else
		opRegN	= decOpFz_idRegN;
		opRegM	= decOpFz_idRegM;
		opRegO	= decOpFz_idRegO;
		opImm	= decOpFz_idImm;
		opUCmd	= decOpFz_idUCmd;
		opUIxt	= decOpFz_idUIxt;
`endif
	end
	else
	begin
`ifdef jx2_enable_ops16
		opRegN	= decOpBz_idRegN;
		opRegM	= decOpBz_idRegM;
		opRegO	= decOpBz_idRegO;
		opImm	= decOpBz_idImm;
		opUCmd	= decOpBz_idUCmd;
		opUIxt	= decOpBz_idUIxt;
`endif
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

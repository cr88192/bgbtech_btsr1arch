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
	istrWord,	istrWordL,
	idRegN,		idRegM,		idRegO,
	idImm,		idImmB,
	idUCmd,		idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
input[63:0]		istrWordL;	//Last Instruction Words

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
output[32:0]	idImm;
output[32:0]	idImmB;
output[7:0]		idUCmd;
output[7:0]		idUIxt;


`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
reg[32:0]		opImm;
reg[32:0]		opImmB;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

assign	idRegN	= opRegN;
assign	idRegM	= opRegM;
assign	idRegO	= opRegO;
assign	idImm	= opImm;
assign	idImmB	= opImmB;
assign	idUCmd	= opUCmd;
assign	idUIxt	= opUIxt;

`ifdef jx2_enable_ops16
`wire_gpr		decOpBz_idRegN;
`wire_gpr		decOpBz_idRegM;
`wire_gpr		decOpBz_idRegO;
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

`wire_gpr		decOpFz_idRegN;
`wire_gpr		decOpFz_idRegM;
`wire_gpr		decOpFz_idRegO;
wire[32:0]		decOpFz_idImm;
wire[7:0]		decOpFz_idUCmd;
wire[7:0]		decOpFz_idUIxt;
wire[3:0]		decOpFz_idUFl;

wire[27:0]		istrJBits;
wire[27:0]		istrJBits2;


`ifdef jx2_enable_wexjumbo

wire	istrIsJumboA;
wire	istrIsJumboB;
assign	istrIsJumboA	= (istrWordL[15: 8] == 8'b1111_1110);
assign	istrIsJumboB	= (istrWordL[47:40] == 8'b1111_1110);

assign	istrJBits [15: 0]	= istrWordL[31:16];
assign	istrJBits [23:16]	= istrWordL[ 7: 0];
assign	istrJBits [   24]	= istrIsJumboA;
assign	istrJBits [   25]	= istrIsJumboA && istrIsJumboB;
assign	istrJBits [   26]	= istrWordL[ 8];
assign	istrJBits [   27]	= istrWordL[40];

assign	istrJBits2[15: 0]	= istrWordL[63:48];
assign	istrJBits2[23:16]	= istrWordL[39:32];
assign	istrJBits2[   24]	= istrIsJumboB;
assign	istrJBits2[   25]	= 0;
assign	istrJBits [   26]	= istrWordL[40];
assign	istrJBits [   27]	= 0;

`else

assign	istrJBits	= 0;
assign	istrJBits2	= 0;

`endif


DecOpFz	decOpFz(
	clock,		reset,
	istrWord,	4'h0,	istrJBits,
	decOpFz_idRegN,		decOpFz_idRegM,
	decOpFz_idRegO,		decOpFz_idImm,
	decOpFz_idUCmd,		decOpFz_idUIxt,
	decOpFz_idUFl
	);

`ifdef jx2_enable_ops48
`wire_gpr		decOpFC_idRegN;
`wire_gpr		decOpFC_idRegM;
`wire_gpr		decOpFC_idRegO;
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
reg opIsDz;		//Predicated Ops
reg opIsDf;		//Pred-False or WEX

always @*
begin

`ifdef jx2_enable_wexjumbo

//	istrJBits[15: 0]	= istrWordL[31:16];
//	istrJBits[23:16]	= istrWordL[ 7: 0];
//	istrJBits[   24]	= (istrWordL[15: 8] == 8'b1111_1110);
//	istrJBits[   25]	= (istrWordL[47:40] == 8'b1111_1110);
//	istrJBits2[15: 0]	= istrWordL[63:48];
//	istrJBits2[23:16]	= istrWordL[39:32];

//	opImmB = { 1'b0, istrJBits2[23:0], istrJBits[23:16] };

	opImmB	= decOpFz_idUFl[0] ?
		{ decOpFz_idImm[32] ? 9'h1FF : 9'h000, istrJBits2[23:0] } :
		{ 1'b0, istrJBits2[23:0], istrJBits[23:16] };
`else
//	istrJBits	= 0;
//	istrJBits2	= 0;
	opImmB = 0;
`endif

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
//			opIsDf = 0;
			opIsDf = istrWord[10];
		end
		6'b111110: begin	//F8..FB
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
//			opIsDf = 0;
			opIsDf = istrWord[8];
		end
		6'b111111: begin	//FC..FF
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 0;
//			opIsDf = 0;
			opIsDf = istrWord[9];
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

	if(opUIxt[7:6]==JX2_IUC_WX)
	begin
		$display("DecOp: WX_Bad %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		opUCmd[5:0] = JX2_UCMD_INVOP;
	end

//	if(opUCmd == JX2_UCMD_INVOP)
	if(opUCmd[5:0] == JX2_UCMD_INVOP)
	begin
//		$display("DecOp: Inv %X-%X-%X",
//			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
	end
end

endmodule

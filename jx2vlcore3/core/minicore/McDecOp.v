/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.

Support for 48-bit encodings is optional.

Ex block is predicated.
Fx block is unconditional.

Decoder at this stage does not care about WEX vs non-WEX.

 */

`include "CoreDefs.v"

`include "minicore/McDecOpFz.v"
`include "minicore/McDecOpRvI.v"

module McDecOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	istrWordL,
	regSr,		istrSxo,
	idRegN,		idRegM,		idRegO,
	idImm,		idImmB,
	idUCmd,		idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
input[63:0]		istrWordL;	//Last Instruction Words

input[63:0]		regSr;
input[3:0]		istrSxo;	//source instruction word

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
output[32:0]	idImm;
output[32:0]	idImmB;
output[8:0]		idUCmd;
output[8:0]		idUIxt;


wire			srWxe;
wire			srRiscv;
wire			srUser;
wire			srSxo;
assign		srWxe	= regSr[27];
assign		srUser	= !regSr[30];
assign		srSxo	= istrSxo[0];
assign		srRiscv	= (regSr[27:26] == 2'b01);

wire[2:0]		srMod;
assign		srMod = { regSr[29], srSxo, srUser };


`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
reg[32:0]		opImm;
reg[32:0]		opImmB;
reg[8:0]		opUCmd;
reg[8:0]		opUIxt;

assign	idRegN	= opRegN;
assign	idRegM	= opRegM;
assign	idRegO	= opRegO;
assign	idImm	= opImm;
assign	idImmB	= opImmB;
assign	idUCmd	= opUCmd;
assign	idUIxt	= opUIxt;

`wire_gpr		decOpFz_idRegN;
`wire_gpr		decOpFz_idRegM;
`wire_gpr		decOpFz_idRegO;
`wire_gpr		decOpFz_idRegP;
wire[32:0]		decOpFz_idImm;
wire[8:0]		decOpFz_idUCmd;
wire[8:0]		decOpFz_idUIxt;
wire[3:0]		decOpFz_idUFl;

wire[27:0]		istrJBits;
wire[27:0]		istrJBits2;


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
assign	istrJBits2[   26]	= istrWordL[40];
assign	istrJBits2[   27]	= 0;


McDecOpFz	decOpFz(
	clock,		reset,	srMod,
	istrWord,	4'h0,	istrJBits,
	decOpFz_idRegN,		decOpFz_idRegM,
	decOpFz_idRegO,		decOpFz_idRegP,
	decOpFz_idImm,
	decOpFz_idUCmd,		decOpFz_idUIxt,
	decOpFz_idUFl
	);

`wire_gpr		decOpRv_idRegN;
`wire_gpr		decOpRv_idRegM;
`wire_gpr		decOpRv_idRegO;
`wire_gpr		decOpRv_idRegP;
wire[32:0]		decOpRv_idImm;
wire[8:0]		decOpRv_idUCmd;
wire[8:0]		decOpRv_idUIxt;
wire[3:0]		decOpRv_idUFl;

McDecOpRvI	decOpRv(
	clock,		reset,	srMod,
	istrWord,	4'h0,	istrJBits,
	decOpRv_idRegN,		decOpRv_idRegM,
	decOpRv_idRegO,		decOpRv_idRegP,
	decOpRv_idImm,
	decOpRv_idUCmd,		decOpRv_idUIxt,
	decOpRv_idUFl
	);

reg		opIsFx;
reg		opIsFz;
reg		opIsFC;
reg		opIsDz;		//Predicated Ops
reg		opIsDf;		//Pred-False or WEX

reg		tMsgLatch;
reg		tNxtMsgLatch;

always @*
begin

	opImmB	= decOpFz_idUFl[0] ?
		{ decOpFz_idImm[32] ? 9'h1FF : 9'h000, istrJBits2[23:0] } :
		{ 1'b0, istrJBits2[23:0], istrJBits[23:16] };

	casez(istrWord[15:10])
		6'b1110zz: begin	//E0..EF
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDf = istrWord[10];
		end

		6'b1111zz: begin	//F0..FF
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = istrWord[10];
		end

		default: begin
			opIsFx = 0;		opIsFz = 0;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
	endcase


	if(srRiscv)
//	if(1'b0)
	begin
		opIsDz	= 0;
		opIsFz	= 1;
	
		opRegN	= decOpRv_idRegN;
		opRegM	= decOpRv_idRegM;
		opRegO	= decOpRv_idRegO;
		opImm	= decOpRv_idImm;
		opUCmd	= decOpRv_idUCmd;
		opUIxt	= decOpRv_idUIxt;
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
	
	if(opIsDz)
	begin
		opUCmd[8:6]=opIsDf?JX2_IXC_CF:JX2_IXC_CT;
	end

	tNxtMsgLatch = 0;

	if(!opIsFz)
	begin
		tNxtMsgLatch = 1;

		if(!tMsgLatch)
			$display("DecOp: Fix32 Bad %X-%X-%X",
				istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		opUCmd[5:0] = JX2_UCMD_INVOP;
	end

	if(opUIxt[8:6]==JX2_IUC_WX)
	begin
		tNxtMsgLatch = 1;

		if(!tMsgLatch)
			$display("DecOp: WX_Bad %X-%X-%X",
				istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		opUCmd[5:0] = JX2_UCMD_INVOP;
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNxtMsgLatch;
end

endmodule

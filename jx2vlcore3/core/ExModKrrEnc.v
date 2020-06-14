/*
Keyring Encode/Decode Module
 */

module ExModKrrEnc(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,
	regInDlr,	regInDhr,
	regOutDlrE,	regOutDhrE);

input			clock;
input			reset;
input[7:0]		opUCmd;
input[7:0]		opUIxt;
input			exHold;

input[63:0]		regInDlr;
input[63:0]		regInDhr;
output[65:0]	regOutDlrE;
output[65:0]	regOutDhrE;

reg[65:0]		tRegOutLo;
reg[65:0]		tRegOutHi;
reg[65:0]		tRegOutLo2;
reg[65:0]		tRegOutHi2;

assign		regOutDlrE = tRegOutLo2;
assign		regOutDhrE = tRegOutHi2;

reg[63:0]		tCurKey;
reg[63:0]		tNxtKey;

reg[63:0]		tDecKrr;
reg[15:0]		tDecPar;

reg[15:0]		tRegParityDlr;
reg[15:0]		tRegParityKrr;
reg				tKeyDisable;

always @*
begin
	tNxtKey		= tCurKey;
	tRegOutLo	= { 2'b00, regInDlr };
	tRegOutHi	= { 2'b00, 64'h0 };
	tKeyDisable	= (tCurKey[3:0] == 0);

	tDecKrr		= regInDlr ^ tCurKey;
	tDecPar		= regInDhr[15:0] ^ tCurKey[15:0];
	
	tRegParityDlr =
		regInDlr[15: 0] ^ regInDlr[31:16] ^
		regInDlr[47:32] ^ regInDlr[63:48];
	tRegParityKrr =
		tDecKrr[15: 0] ^ tDecKrr[31:16] ^
		tDecKrr[47:32] ^ tDecKrr[63:48];


	if(opUCmd[5:0]==JX2_UCMD_OP_IXT)
	begin
		if(opUIxt[5:0]==JX2_UCIX_IXT_LDEKEY)
		begin
			tNxtKey = regInDlr;
		end

		if(opUIxt[5:0]==JX2_UCIX_IXT_LDEKRR)
		begin
			tRegOutLo = { 2'b00, tDecKrr };
			if((tRegParityKrr == tDecPar) && !tKeyDisable)
				tRegOutLo[65:64] = 2'b10;
		end

		if(opUIxt[5:0]==JX2_UCIX_IXT_LDEENC)
		begin
			tRegOutLo = { 2'b00, regInDlr ^ tCurKey };
			tRegOutHi = { 2'b00, 48'h0, tRegParityDlr ^ tCurKey[15:0] };
		end
	end
end

always @(posedge clock)
begin
	tCurKey		<= tNxtKey;
	tRegOutLo2	<= tRegOutLo;
	tRegOutHi2	<= tRegOutHi;
end

endmodule

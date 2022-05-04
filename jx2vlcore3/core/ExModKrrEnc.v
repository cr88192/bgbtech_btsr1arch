/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Keyring Encode/Decode Module
 */

// `include "ExModKrrShufD.v"
// `include "ExModKrrShufE.v"
`include "ExModKrrFixSh.v"

module ExModKrrEnc(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,		exBraFlush,
	regInDlr,	regInDhr,	regInKrr,
	regOutDlrE,	regOutDhrE);

input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
input			exHold;
input			exBraFlush;

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInKrr;
output[65:0]	regOutDlrE;
output[65:0]	regOutDhrE;

wire[63:0]		tRegInDlrShuD;		//Shuffle
wire[63:0]		tRegInDhrShuD;		//Shuffle

// wire[15:0]		tRegShuf1;
// wire[15:0]		tRegShuf2;
// assign			tRegShuf1 = tCurKeyLo[15: 0] ^ tCurKeyLo[47:32];
// assign			tRegShuf2 = tCurKeyLo[31:16] ^ tCurKeyLo[63:48];

reg[15:0]		tRegShuf1;
reg[15:0]		tRegShuf2;
// reg[15:0]		tRegShuf1A;
// reg[15:0]		tRegShuf2A;

reg[15:0]		tRegShuf1A_S1;
reg[15:0]		tRegShuf1A_S2;
// reg[15:0]		tRegShuf2A;

// assign			tRegShuf1 = tCurKeyLo[15:0];
// assign			tRegShuf2 = tCurKeyHi[15:0];

// ExModKrrShufD	shu1d(regInDlr, tRegInDlrShuD, tRegShuf1);
// ExModKrrShufD	shu2d(regInDhr, tRegInDhrShuD, tRegShuf2);

wire[63:0]		tRegInKey;

assign		tRegInKey =
	(opUIxt[5:0] == JX2_UCIX_IXT_SVEKRR) ?
		regInKrr : regInDlr;


wire[63:0]		tRegInDlrXlo;
wire[63:0]		tRegInDlrXhi;
assign		tRegInDlrXlo = tRegInKey ^ tCurKeyLo;
assign		tRegInDlrXhi = tRegInKey ^ tCurKeyHi;

wire[63:0]		tRegInDlrShuE;		//Shuffle
wire[63:0]		tRegInDhrShuE;		//Shuffle

// ExModKrrShufE	shu1e(tRegInDlrXlo, tRegInDlrShuE, tRegShuf1);
// ExModKrrShufE	shu2e(tRegInDlrXhi, tRegInDhrShuE, tRegShuf2);

ExModKrrFixSh	shuf(
	tRegInDlrXlo,	tRegInDlrXhi,
	regInDlr,		regInDhr,
	tRegInDlrShuE,	tRegInDhrShuE,
	tRegInDlrShuD,	tRegInDhrShuD);
	

reg[63:0]		tRegInDlrShuDl;
reg[63:0]		tRegInDhrShuDl;

reg[63:0]		tRegInDlrShuEl;
reg[63:0]		tRegInDhrShuEl;

reg[65:0]		tRegOutLo;
reg[65:0]		tRegOutHi;
reg[65:0]		tRegOutLo2;
reg[65:0]		tRegOutHi2;

assign		regOutDlrE = tRegOutLo2;
assign		regOutDhrE = tRegOutHi2;

reg[63:0]		tCurKeyLo;
reg[63:0]		tNxtKeyLo;
reg[63:0]		tCurKeyHi;
reg[63:0]		tNxtKeyHi;

reg[63:0]		tDecKrrLo;
reg[63:0]		tDecKrrHi;
reg				tDecKrrEq;
// reg[15:0]		tDecPar;

reg[15:0]		tRegParityDlr;
reg[15:0]		tRegParityKrr;
reg				tKeyDisable;

always @*
begin
	tNxtKeyLo	= tCurKeyLo;
	tNxtKeyHi	= tCurKeyHi;
	tRegOutLo	= { 2'b00, regInDlr };
	tRegOutHi	= { 2'b00, 64'h0 };
	tKeyDisable	= (tCurKeyLo[3:0] == 0);

	if(reset)
	begin
		tNxtKeyLo	= 0;
		tNxtKeyHi	= 0;
	end

`ifndef def_true

	case(tCurKeyLo[1:0])
		2'b00: tRegShuf1A_S1 = { tCurKeyLo[39:32], tCurKeyLo[ 7: 0] };
		2'b01: tRegShuf1A_S1 = { tCurKeyLo[47:40], tCurKeyLo[15: 8] };
		2'b10: tRegShuf1A_S1 = { tCurKeyLo[55:48], tCurKeyLo[23:16] };
		2'b11: tRegShuf1A_S1 = { tCurKeyLo[63:56], tCurKeyLo[31:24] };
	endcase
	case(tCurKeyLo[33:32])
		2'b00: tRegShuf1A_S2 = { tCurKeyLo[31:24], tCurKeyLo[39:32] };
		2'b01: tRegShuf1A_S2 = { tCurKeyLo[23:16], tCurKeyLo[47:40] };
		2'b10: tRegShuf1A_S2 = { tCurKeyLo[15: 8], tCurKeyLo[55:48] };
		2'b11: tRegShuf1A_S2 = { tCurKeyLo[ 7: 0], tCurKeyLo[63:56] };
	endcase

	tRegShuf1A	= tRegShuf1A_S1 ^ tRegShuf1A_S2;

//	tRegShuf1A	= tCurKeyLo[15:0];
	tRegShuf2A	= tCurKeyHi[15:0];

`endif

//	tDecKrrLo	= tRegInDlrShu ^ tCurKeyLo;
//	tDecKrrHi	= tRegInDhrShu ^ tCurKeyHi;
	tDecKrrLo	= tRegInDlrShuDl ^ tCurKeyLo;
	tDecKrrHi	= tRegInDhrShuDl ^ tCurKeyHi;

//	tDecPar		= regInDhr[15:0] ^ tCurKey[15:0];
	
//	tDecKrrEq	= (tDecKrrLo == tDecKrrHi);
	tDecKrrEq	=
		(tDecKrrLo[15: 0] == tDecKrrHi[15: 0]) &&
		(tDecKrrLo[31:16] == tDecKrrHi[31:16]) ;
	
//	tRegParityDlr =
//		regInDlr[15: 0] ^ regInDlr[31:16] ^
//		regInDlr[47:32] ^ regInDlr[63:48];
//	tRegParityKrr =
//		tDecKrr[15: 0] ^ tDecKrr[31:16] ^
//		tDecKrr[47:32] ^ tDecKrr[63:48];


//	if(opUCmd[5:0]==JX2_UCMD_OP_IXT)
	if((opUCmd[5:0]==JX2_UCMD_OP_IXT) && !exBraFlush)
	begin
		if(opUIxt[5:0]==JX2_UCIX_IXT_LDEKEY)
		begin
			tNxtKeyLo = regInDlr;
			tNxtKeyHi = regInDhr;
		end

		if(opUIxt[5:0]==JX2_UCIX_IXT_LDEKRR)
		begin
			$display("ModKrrEnc: LDEKRR");

//			tRegOutLo = { 2'b00, tDecKrr };
			tRegOutLo = { 2'b11, tDecKrrLo };
//			if((tRegParityKrr == tDecPar) && !tKeyDisable)
			if(tDecKrrEq && !tKeyDisable)
				tRegOutLo[65:64] = 2'b10;
		end

		if(	(opUIxt[5:0]==JX2_UCIX_IXT_LDEENC) ||
			(opUIxt[5:0]==JX2_UCIX_IXT_SVEKRR) )
		begin
			$display("ModKrrEnc: LDEENC / SVEKRR, %X-%X", opUCmd, opUIxt);

//			tRegOutLo = { 2'b00, regInDlr ^ tCurKey };
//			tRegOutHi = { 2'b00, 48'h0, tRegParityDlr ^ tCurKey[15:0] };

//			tRegOutLo = { 2'b00, tRegInDlrShuE };
//			tRegOutHi = { 2'b00, tRegInDhrShuE };

			tRegOutLo = { 2'b00, tRegInDlrShuEl };
			tRegOutHi = { 2'b00, tRegInDhrShuEl };
			
			if(tKeyDisable)
				tRegOutLo[65:64] = 2'b11;
		end
	end
end

always @(posedge clock)
begin
	tRegInDlrShuDl	<= tRegInDlrShuD;
	tRegInDhrShuDl	<= tRegInDhrShuD;

	tRegInDlrShuEl	<= tRegInDlrShuE;
	tRegInDhrShuEl	<= tRegInDhrShuE;

//	tRegShuf1		<= tRegShuf1A;
//	tRegShuf2		<= tRegShuf2A;

	tCurKeyLo	<= tNxtKeyLo;
	tCurKeyHi	<= tNxtKeyHi;
	tRegOutLo2	<= tRegOutLo;
	tRegOutHi2	<= tRegOutHi;
end

endmodule

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
Detect unconditional branch instructions for early branch.
Happens in parallel with the main ID1 stage.

May also incorporate branch prediction.

exBraDir:
* (1:0) = Branch Direction (Predication Mode)
** 00: Unconditional
** 01: Not a Branch
** 10: Branch if True
** 11: Branch if False
* (  2) = Branch was not taken

State of Branch:
* 000: Branch not taken (Weak)
* 011: Branch not taken (Strong)
* 100: Branch taken (Strong)
* 111: Branch taken (Weak)

State Machine 2:
* 000: Weak Not Taken			(0->001, 1->110)
* 001: Strong Not Taken			(0->001, 1->000)
* 010: Trans Weak, Not Taken	(0->000, 1->111)
* 011: Trans Strong, Not Taken	(0->010, 1->111)
* 100: Weak Taken				(0->010, 1->101)
* 101: Strong Taken				(0->100, 1->101)
* 110: Trans Weak, Taken		(0->011, 1->100)
* 111: Trans Strong, Taken		(0->011, 1->110)

*/

/*
MTAG, in RV64 Mode:
 00: RV Op
 01: XG3 Op repacked to XG2RV.
 */

`ifndef HAS_DECPREBRA
`define HAS_DECPREBRA

`ifdef jx2_prebra_rts
`ifndef jx2_prebra_rtsu
`define jx2_prebra_rtsu
`endif
`endif

`define jx2_prebra_ga6bit

module DecPreBra(
	clock,			reset,
	istrWord,		istrMTag,
	istrBasePc,		istrBraPc,
	preBraPc,		preIsBra,
	preBraPcInc,	preWasBra,

	regValLr,		ifBraBPc,
	regValDlr,		regValDhr,
	exBraBPc,		exBraDir,
	pipeHasLr,		regMemOpm);

input			clock;
input			reset;
input[63:0]		istrWord;	//Instruction Word
input[47:0]		istrBasePc;	//Instruction Base PC
input[47:0]		istrBraPc;	//Branch Base PC
input[63:0]		regValLr;	//Link Register
input[63:0]		regValDlr;	//Link Register
input[63:0]		regValDhr;	//Link Register
input[4:0]		regMemOpm;	//OPM
input[3:0]		istrMTag;	//OPM

input[47:0]		ifBraBPc;	//Fetch Branch PC
input[47:0]		exBraBPc;	//Fetch Base PC
input[2:0]		exBraDir;
input[7:0]		pipeHasLr;

output[47:0]	preBraPc;
output[47:0]	preBraPcInc;
output[1:0]		preIsBra;
output			preWasBra;

reg[47:0]	tPreBraPc;
reg[47:0]	tPreBraPcInc;
reg			tPreBra;
reg			tNonBra;
reg			tWasBra;
assign	preBraPc	= tPreBraPc;
assign	preBraPcInc	= tPreBraPcInc;
// assign	preIsBra	= { 1'b0, tPreBra };
assign	preIsBra	= { tNonBra, tPreBra };
assign	preWasBra	= tWasBra;


// reg[32:0]	tBraDisp8;
// reg[32:0]	tBraDisp20;
// reg[24:0]	tBraDisp8;
reg[12:0]	tBraDisp8;
reg[12:0]	tBraDisp8B;
reg[16:0]	tBraDisp11B;
reg[24:0]	tBraDisp20;
reg[24:0]	tBraDispRv20;

reg[16:0]	tBraDispRv12;

reg[12:0]	tBraDispInc;

reg[31:0]	tDisp8;
reg[31:0]	tDisp8B;
reg[31:0]	tDisp11B;
reg[31:0]	tDisp20;

reg[31:0]	tDispRv12;
reg[31:0]	tDispRv20;

reg[11:0]	tBraDisp8HiP0;
reg[11:0]	tBraDisp8HiP1;
reg[11:0]	tBraDisp8HiN1;

reg[20:0]	tBraDisp8Lo;
reg[20:0]	tBraDisp20Lo;

reg			tIsBra8;		//Unconditional Branch (8-bit Disp)
reg			tIsBra20;		//Unconditional Branch (16-bit Disp)
reg			tIsRtsu;
reg			tIsRtsR1;

reg			tIsFz;
reg			tIsFz3x;

reg			tIsRtsuFz;
reg			tIsRtsR1Fz;

reg			tIsBraFz;		//Fn

reg			tIsBraCc8;		//Conditional Branch (8-bit Disp)
reg			tIsBraCc8B;		//Conditional Branch (8-bit Disp, JCMP)
reg			tIsBraCc8Br;	//Conditional Branch (8-bit Disp, JCMP)
reg			tIsBraCc8Bz;	//Conditional Branch (8-bit Disp, JCMP)
reg			tIsBraCc20;		//Conditional Branch (20-bit Disp)

reg			tIsBraCc11B;	//Conditional Branch (8-bit Disp, JCMP)

reg			tIsBraCc11Bz;	//Conditional Branch (11-bit Disp, JCMPZ)

reg			tIsBraCcP20;	//Conditional Branch (20-bit Disp)
reg			tIsBraCcF20;	//Conditional Branch (20-bit Disp)

reg			tIsBraRv12;		//RV Conditional Branch (12-bit Disp)
reg			tIsBraRv20;		//RV Conditional Branch (20-bit Disp)

reg			tDoBraCc8;		//Take Conditional Branch (Cc8)
reg			tDoBraCc8B;		//Take Conditional Branch (Cc8, JCMP)
reg			tDoBraCc11B;	//Take Conditional Branch (Cc8, JCMP)
reg			tDoBraCc20;		//Take Conditional Branch (Cc20)

reg			tDoBraRv12;		//Take Conditional Branch (12-bit Disp)

`ifdef jx2_prebra_ga6bit
reg[5:0]	preCnts[63:0];
reg[5:0]	tPreIfCnt;
reg[5:0]	tPreExCnt;
reg[5:0]	tPreExCntB;
`else
reg[2:0]	preCnts[63:0];
reg[2:0]	tPreIfCnt;
reg[2:0]	tPreExCnt;
reg[2:0]	tPreExCntB;
`endif

reg			tPreIfDir;

reg[2:0]	tPreExDir;
reg[47:0]	tPreExBPc;

// reg[5:0]	preBits[63:0];
reg[5:0]	tPreIfBit;
reg[5:0]	tPreExBit;
reg[5:0]	tPreExBitB;
reg			tPreBit;

reg[5:0]	tPreIfIx;
reg[5:0]	tPreIdIx;
reg[5:0]	tPreIbIx;
reg[5:0]	tPreExIx;
reg[5:0]	tPreExIxA;

//reg[11:0]	tPreIfIx;
//reg[11:0]	tPreIdIx;
//reg[11:0]	tPreIbIx;
//reg[11:0]	tPreExIx;
//reg[11:0]	tPreExIxA;

reg[5:0]	tHistBits;
reg[5:0]	tHistBitsB;

// `define		jx2_prebra_do_vtlb

// `define		jx2_prebra_basepc

reg[5:0]	tVtlbIx;
reg[17:0]	tVtlbArr[63:0];
reg[17:0]	tVtlbStVal;
reg[5:0]	tVtlbStIx;
reg			tDoVtlbSt;

`ifdef jx2_enable_riscv
wire	isBase	= (pipeHasLr[7:6] == 2'b00) && !pipeHasLr[4];
wire	isXG2	= (pipeHasLr[7:6] == 2'b01) && !pipeHasLr[4];
wire	isRV	= (pipeHasLr[7:6] == 2'b00) && pipeHasLr[4];
wire	isXG2RV	= (pipeHasLr[7:6] == 2'b01) && pipeHasLr[4];
`else
wire	isBase	= (pipeHasLr[7:6] == 2'b00);
wire	isXG2	= (pipeHasLr[7:6] == 2'b01);
wire	isRV	= 0;
wire	isXG2RV	= 0;
`endif

reg[11:0]	tStatBraTot;
reg[11:0]	tStatBraHit;
reg[11:0]	tNxtStatBraTot;
reg[11:0]	tNxtStatBraHit;

always @*
begin
//	tPreBraPc	= UV48_XX;

`ifndef jx2_prebra_basepc
	tPreBraPc	= istrBraPc;
`else
	tPreBraPc	= istrBasePc;
`endif

	tPreBra		= 0;
	tNonBra		= 1;
	tWasBra		= 0;
	tHistBitsB	= tHistBits;

	tNxtStatBraTot	= tStatBraTot;
	tNxtStatBraHit	= tStatBraHit;

// `ifdef def_true
`ifndef def_true
	if(tStatBraTot[11])
	begin
		$display("DecPreBra: %d hit=%d", tStatBraTot, tStatBraHit);
		tNxtStatBraTot	= 0;
		tNxtStatBraHit	= 0;
	end
`endif

`ifdef jx2_prebra_do_vtlb
	tVtlbStIx	= regValDhr[19:14] ^ regValDhr[25:20];
	tVtlbStVal	= regValDhr[31:14];
	tDoVtlbSt	= 0;
`endif
	
	tPreIfIx	= ifBraBPc[6:1]		^ tHistBits;
	tPreIbIx	= istrBasePc[6:1]	^ tHistBits;
	tPreExIxA	= exBraBPc[6:1]		^ tHistBits;

//	tPreIfIx	= ifBraBPc[12:1];
//	tPreIbIx	= istrBasePc[12:1];
//	tPreExIxA	= exBraBPc[12:1];
	
	tPreExCntB	= tPreExCnt;
	tPreExBitB	= tPreExBit;

	if(tPreExDir[1])
	begin
		tHistBitsB = { tHistBits[4:0], tPreExDir[2] };
		tPreExBitB = { tPreExBit[4:0], tPreExDir[2] };

		tNxtStatBraTot	= tStatBraTot + 1;

`ifdef jx2_prebra_ga6bit
		if(tPreExCnt[0] == tPreExDir[2])
			tNxtStatBraHit	= tStatBraHit + 1;

		case( {tPreExCnt[5:1], tPreExDir[2]} )
			6'b00000_0: tPreExCntB=6'b00101_1;
			6'b00000_1: tPreExCntB=6'b10111_0;
			6'b00001_0: tPreExCntB=6'b01111_1;
			6'b00001_1: tPreExCntB=6'b10100_0;
			6'b00010_0: tPreExCntB=6'b01010_0;
			6'b00010_1: tPreExCntB=6'b00011_0;
			6'b00011_0: tPreExCntB=6'b10000_0;
			6'b00011_1: tPreExCntB=6'b11110_1;
			6'b00100_0: tPreExCntB=6'b01000_1;
			6'b00100_1: tPreExCntB=6'b01101_1;
			6'b00101_0: tPreExCntB=6'b11111_1;
			6'b00101_1: tPreExCntB=6'b00010_0;
			6'b00110_0: tPreExCntB=6'b10011_0;
			6'b00110_1: tPreExCntB=6'b00110_1;
			6'b00111_0: tPreExCntB=6'b01011_0;
			6'b00111_1: tPreExCntB=6'b10110_1;
			6'b01000_0: tPreExCntB=6'b10000_0;
			6'b01000_1: tPreExCntB=6'b00111_1;
			6'b01001_0: tPreExCntB=6'b10111_0;
			6'b01001_1: tPreExCntB=6'b00110_0;
			6'b01010_0: tPreExCntB=6'b00000_0;
			6'b01010_1: tPreExCntB=6'b00100_1;
			6'b01011_0: tPreExCntB=6'b10000_1;
			6'b01011_1: tPreExCntB=6'b10111_0;
			6'b01100_0: tPreExCntB=6'b01000_0;
			6'b01100_1: tPreExCntB=6'b01010_1;
			6'b01101_0: tPreExCntB=6'b01110_1;
			6'b01101_1: tPreExCntB=6'b01101_1;
			6'b01110_0: tPreExCntB=6'b00101_0;
			6'b01110_1: tPreExCntB=6'b01100_1;
			6'b01111_0: tPreExCntB=6'b11100_1;
			6'b01111_1: tPreExCntB=6'b01100_0;
			6'b10000_0: tPreExCntB=6'b11110_0;
			6'b10000_1: tPreExCntB=6'b10001_0;
			6'b10001_0: tPreExCntB=6'b01011_0;
			6'b10001_1: tPreExCntB=6'b10110_0;
			6'b10010_0: tPreExCntB=6'b00101_0;
			6'b10010_1: tPreExCntB=6'b01001_1;
			6'b10011_0: tPreExCntB=6'b10010_1;
			6'b10011_1: tPreExCntB=6'b10111_1;
			6'b10100_0: tPreExCntB=6'b11011_0;
			6'b10100_1: tPreExCntB=6'b00110_1;
			6'b10101_0: tPreExCntB=6'b10011_0;
			6'b10101_1: tPreExCntB=6'b01110_0;
			6'b10110_0: tPreExCntB=6'b10011_1;
			6'b10110_1: tPreExCntB=6'b00100_0;
			6'b10111_0: tPreExCntB=6'b11011_1;
			6'b10111_1: tPreExCntB=6'b10110_0;
			6'b11000_0: tPreExCntB=6'b01101_1;
			6'b11000_1: tPreExCntB=6'b00110_1;
			6'b11001_0: tPreExCntB=6'b10011_1;
			6'b11001_1: tPreExCntB=6'b10110_1;
			6'b11010_0: tPreExCntB=6'b01100_0;
			6'b11010_1: tPreExCntB=6'b10001_0;
			6'b11011_0: tPreExCntB=6'b01110_0;
			6'b11011_1: tPreExCntB=6'b10111_0;
			6'b11100_0: tPreExCntB=6'b11100_0;
			6'b11100_1: tPreExCntB=6'b11010_0;
			6'b11101_0: tPreExCntB=6'b10001_1;
			6'b11101_1: tPreExCntB=6'b11110_0;
			6'b11110_0: tPreExCntB=6'b00001_0;
			6'b11110_1: tPreExCntB=6'b10100_0;
			6'b11111_0: tPreExCntB=6'b01111_0;
			6'b11111_1: tPreExCntB=6'b10100_0;
		endcase
`else
		if((tPreExCnt[2]^tPreExCnt[1]) == tPreExDir[2])
			tNxtStatBraHit	= tStatBraHit + 1;

		case( {tPreExDir[2], tPreExCnt[2:0]} )
			4'b0000: tPreExCntB=3'b001;
			4'b0001: tPreExCntB=3'b001;
			4'b0010: tPreExCntB=3'b000;
			4'b0011: tPreExCntB=3'b010;
			4'b0100: tPreExCntB=3'b010;
			4'b0101: tPreExCntB=3'b100;
			4'b0110: tPreExCntB=3'b011;
			4'b0111: tPreExCntB=3'b011;
			4'b1000: tPreExCntB=3'b110;
			4'b1001: tPreExCntB=3'b000;
			4'b1010: tPreExCntB=3'b111;
			4'b1011: tPreExCntB=3'b111;
			4'b1100: tPreExCntB=3'b101;
			4'b1101: tPreExCntB=3'b101;
			4'b1110: tPreExCntB=3'b100;
			4'b1111: tPreExCntB=3'b110;
		endcase
`endif

//		$display("PreBra: Predict State %X, BPc=%X",
//			{tPreExDir[2], tPreExCnt[2:0]},
//			tPreExBPc);
	end
		
	tDisp8		= { istrWord[ 7]?UV24_FF:UV24_00, istrWord[7:0] };
	tDisp8B		= { istrWord[23]?UV24_FF:UV24_00, istrWord[23:16] };
	tDisp11B	= { istrWord[ 0]?UV22_FF:UV22_00, istrWord[25:16] };

	tDisp20		= {
		istrWord[7]?UV12_FF:UV12_00,
		istrWord[7:0], istrWord[27:16] };
	
`ifdef jx2_enable_riscv
	tDispRv12 = {
		istrWord[31]?UV20_FF:UV20_00,
		istrWord[7], istrWord[30:25], istrWord[11:8], 1'b0 };
	tDispRv20 = {
		istrWord[31]?UV12_FF:UV12_00,
		istrWord[19:12],
		istrWord[20],
		istrWord[30:21], 1'b0 };
`endif
	
//	tBraDisp8	= {1'b0, istrBraPc[31:0] } + { tDisp8[31:0], 1'b0 };
//	tBraDisp20	= {1'b0, istrBraPc[31:0] } + { tDisp20[31:0], 1'b0 };
//	tBraDisp8	= {1'b0, istrBraPc[23:0] } + { tDisp8[23:0], 1'b0 };

`ifndef jx2_prebra_basepc
	tBraDisp8	= {1'b0, istrBraPc[11:0] } + { tDisp8[11:0], 1'b0 };
	tBraDisp20	= {1'b0, istrBraPc[23:0] } + { tDisp20[23:0], 1'b0 };
	tBraDisp8B	= {1'b0, istrBraPc[11:0] } + { tDisp8B[11:0], 1'b0 };
	tBraDisp11B	= {1'b0, istrBraPc[15:0] } + { tDisp11B[15:0], 1'b0 };
`else
	tBraDisp8	= {1'b0, istrBasePc[11:0] } + { tDisp8[11:0], 1'b0 } + 2;
	tBraDisp20	= {1'b0, istrBasePc[23:0] } + { tDisp20[23:0], 1'b0 } + 4;
	tBraDisp8B	= {1'b0, istrBasePc[11:0] } + { tDisp8B[11:0], 1'b0 } + 4;
	tBraDisp11B	= {1'b0, istrBasePc[15:0] } + { tDisp11B[15:0], 1'b0 } + 4;
`endif

`ifdef jx2_enable_riscv
	tBraDispRv12	= {1'b0, istrBasePc[15:0] } + tDispRv12[16:0];
	tBraDispRv20	= {1'b0, istrBasePc[23:0] } + tDispRv20[24:0];
`endif

// `ifdef def_true
`ifndef def_true
	tBraDisp8HiP0	= istrBraPc[31:21];
	tBraDisp8HiP1	= istrBraPc[31:21]+1;
	tBraDisp8HiN1	= istrBraPc[31:21]-1;
	tBraDisp8Lo		= { 1'b0, istrBraPc[20:1]} + tDisp8[19:0];
	tBraDisp20Lo	= { 1'b0, istrBraPc[20:1]} + tDisp20[19:0];

	if(tDisp8[30])
		tBraDisp8 = {
			tBraDisp8Lo[20] ? tBraDisp8HiP0 : tBraDisp8HiN1,
			tBraDisp8Lo[19:0], istrBraPc[0]	};
	else
		tBraDisp8 = {
			tBraDisp8Lo[20] ? tBraDisp8HiP1 : tBraDisp8HiP0,
			tBraDisp8Lo[19:0], istrBraPc[0]	};

	if(tDisp20[30])
		tBraDisp20 = {
			tBraDisp20Lo[20] ? tBraDisp8HiP0 : tBraDisp8HiN1,
			tBraDisp20Lo[19:0], istrBraPc[0]	};
	else
		tBraDisp20 = {
			tBraDisp20Lo[20] ? tBraDisp8HiP1 : tBraDisp8HiP0,
			tBraDisp20Lo[19:0], istrBraPc[0]	};
`endif

	tIsBra8		= 0;
	tIsBraCc8	= 0;

	tIsBraRv12	= 0;
	tIsBraRv20	= 0;

`ifndef jx2_prebra_no16b
	tIsBra8		=
		isBase &&
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b000);
//		(istrWord[11: 8]==4'b0000);

	tIsBraCc8		=
		isBase &&
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b001);
`endif

	tIsBra20		=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) &&
		(istrWord[31:29]==3'b110);
//		(istrWord[31:28]==4'b1100);

	tIsBraCcP20		=
		(istrWord[15:12]==4'hE) &&
		((istrWord[11: 8]==4'h0) || (istrWord[11: 8]==4'h4)) &&
		(istrWord[31:28]==4'b1100);
//	tIsBraCcP20		= 0;


	tIsBraRv12		=
		(istrWord[6:0]==7'b1100011) && pipeHasLr[4] && !istrMTag[0];
	tIsBraRv20		=
		(istrWord[6:0]==7'b1101111) && pipeHasLr[4] && !istrMTag[0];

//	tIsBraRv12		=	0;	//BGB: debug
//	tIsBraRv20		=	0;	//BGB: debug


	tIsBraCc8Br		=	0;
	tIsBraCc8Bz		=	0;
	tIsBraCc11Bz	=	0;

	tIsBraCc8B = 0;
	tIsBraCc11B = 0;

	tIsBraFz		=	(istrWord[15: 12]==4'hF);


// `ifdef jx2_alu_jcmpz
`ifdef jx2_alu_jcmp
// `ifndef def_true
	tIsBraCc8Br		=
		tIsBraFz				&&
		(istrWord[11: 8]==4'h1) &&
		(istrWord[31:30]==2'b11);
`endif

`ifdef jx2_alu_jcmpz2
	tIsBraCc8Bz		=
		tIsBraFz				&&
		(istrWord[11: 8]==4'h2) &&
		(istrWord[31:28]==4'hD) &&
		(istrWord[ 3: 1]==3'b011);

	tIsBraCc11Bz	=
		tIsBraFz				&&
		(istrWord[11: 8]==4'h2) &&
		(istrWord[31:28]==4'hE) &&
		((istrWord[ 3: 2]==2'b01) || istrWord[3]);
	tIsBraCc11B = tIsBraCc11Bz;
`endif

	tIsBraCc8B		= tIsBraCc8Br || tIsBraCc8Bz;

//	tIsBraCc20		=
//	tIsBraCcF20		=
//		(istrWord[15:12]==4'hF) &&
//		(istrWord[11: 8]==4'h0) &&
//		(istrWord[31:29]==3'b111);
	tIsBraCcF20		= 0;

	tIsBraCc20		= tIsBraCcF20 || tIsBraCcP20;

	tIsRtsu			= 0;
	tIsRtsR1		= 0;
	tIsRtsuFz		= 0;
	tIsRtsR1Fz		= 0;

`ifdef jx2_prebra_rts

`ifndef jx2_prebra_no16b
	tIsRtsu			=
		isBase &&
		((istrWord[15:0] == 16'h3012) ||
		((istrWord[15:0] == 16'h3010) && !pipeHasLr[0]));
//	tIsRtsR1		= 0;
	tIsRtsR1		=
		isBase &&
		(istrWord[15:0] == 16'h3210) && !pipeHasLr[1];
`endif

`ifdef def_true

	tIsFz	=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) ;
		
	tIsFz3x	=
		tIsFz	&&
		(istrWord[31:28]==4'h3) &&
		(istrWord[19:16]==4'h0) ;

	tIsRtsuFz	=
		tIsFz3x &&
		(istrWord[27:24]==4'h0) &&
		(istrWord[ 7: 4]==4'h0) &&
		(	((istrWord[ 3: 0]==4'h0) && !pipeHasLr[0]) ||
			(istrWord[ 3: 0]==4'h2)	) &&
		(istrWord[23:20]==4'h1) ;

	tIsRtsR1Fz	=
		tIsFz3x &&
		(istrWord[27:24]==4'h0) &&
		(istrWord[ 7: 4]==4'h2) &&
		(istrWord[ 3: 0]==4'h0) &&
		(istrWord[23:20]==4'h1) &&
		!pipeHasLr[1];

	if(tIsRtsuFz)
		tIsRtsu = 1;

	if(tIsRtsR1Fz)
		tIsRtsR1 = 1;

`endif

`else
`ifdef jx2_prebra_rtsu

`ifndef jx2_prebra_no16b
	tIsRtsu			=
		isBase &&
		(istrWord[15:0] == 16'h3012);
	tIsRtsR1		= 0;
`endif

	tIsRtsuFz	=
		tIsFz3x &&
		(istrWord[27:24]==4'h0) &&
		(istrWord[ 7: 4]==4'h0) &&
		(	((istrWord[ 3: 0]==4'h0) && !pipeHasLr[0]) ||
			(istrWord[ 3: 0]==4'h2)	) &&
		(istrWord[23:20]==4'h1) ;

	if(tIsRtsuFz)
		tIsRtsu = 1;

`else
	tIsRtsu			= 0;
	tIsRtsR1		= 0;
`endif
`endif

`ifdef jx2_enable_riscv
	if(pipeHasLr[4])
//	if(pipeHasLr[4] && !istrMTag[0])
//	if(isRV)
	begin
		/* RISC-V, Excluding XG3 */
		tIsBra8		= 0;
		tIsBraCc8	= 0;
		tIsBraCc8B	= 0;
		tIsBraCc11B	= 0;
		tIsBraCc20	= 0;
		tIsBra20	= 0;
		tIsRtsu		= 0;
		tIsRtsR1	= 0;
	end
	if(pipeHasLr[4])
	begin
		/* Excluded in RISC-V and XG3 */
		tIsBraCc20	= 0;
		tIsBra20	= 0;
	end
`endif

	if(pipeHasLr[7:6] != 2'b00)
	begin
		/* XG2 */
		tIsBra8		= 0;
		tIsBraCc8	= 0;
//		tIsBraCc8B	= 0;
//		tIsBraCc11B	= 0;
		tIsRtsu		= 0;
		tIsRtsR1	= 0;

		if(tIsRtsuFz)
			tIsRtsu = 1;

		if(tIsRtsR1Fz)
			tIsRtsR1 = 1;
	end

`ifndef def_true
	if(tBraDisp20[23:14] != istrBraPc[23:14])
	begin
		// Debug
		tIsBraCc20	= 0;
		tIsBra20	= 0;
	end
`endif

//	tIsBra8		= 0;
//	tIsBra20	= 0;
//	tIsRtsu		= 0;
//	tIsRtsR1	= 0;

	tDoBraCc8	= 0;
	tDoBraCc20	= 0;
	tDoBraCc8B	= 0;
	tDoBraCc11B	= 0;

	tDoBraRv12	= 0;
	
`ifdef jx2_prebra_ga6bit
	tPreIfDir	= tPreIfCnt[0];
`else
	tPreIfDir	= tPreIfCnt[2] ^ tPreIfCnt[1];
`endif

// `ifndef def_true
`ifdef def_true
	if(tIsBraCc8 && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc8 = tPreExCntB[1] ? tPreBit : tPreIfCnt[2];
//		tDoBraCc8 = tPreIfCnt[2] ^ tPreIfCnt[1];
		tDoBraCc8 = tPreIfDir;
		tNonBra		= 0;
		tWasBra		= 1;
		
//		tDoBraCc8 = tPreBit;
//		tDoBraCc8 = tPreIfCnt[2];
//		tDoBraCc8 = !tPreIfCnt[2];
//		$display("PreBra: Predict 8, Do=%d, BPc=%X",
//			tDoBraCc8, istrBasePc);
	end

	if(tIsBraCc20 && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc20 = tPreExCntB[1] ? tPreBit : tPreIfCnt[2];
//		tDoBraCc20 = tPreIfCnt[2] ^ tPreIfCnt[1];
		tDoBraCc20 = tPreIfDir;
		tNonBra		= 0;
		tWasBra		= 1;
		
//		tDoBraCc20 = tPreBit;
//		tDoBraCc20 = tPreIfCnt[2];
//		tDoBraCc20 = !tPreIfCnt[2];
//		$display("PreBra: Predict 20, Do=%d", tDoBraCc20);
	end

	if(tIsBraCc8B && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc8B = tPreIfCnt[2] ^ tPreIfCnt[1];
		tDoBraCc8B = tPreIfDir;
		tNonBra		= 0;
		tWasBra		= 1;
	end

	if(tIsBraCc11B && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc11B = tPreIfCnt[2] ^ tPreIfCnt[1];
		tDoBraCc11B = tPreIfDir;
		tNonBra		= 0;
		tWasBra		= 1;
	end

	if(tIsBraRv12 && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraRv12	= tPreIfCnt[2] ^ tPreIfCnt[1];
		tDoBraRv12	= tPreIfDir;
		tNonBra		= 0;
		tWasBra		= 1;
	end
`endif
	
	if(tIsBra8 || tIsBraCc8)
	begin
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp8[31:0] };
//		tPreBraPc	= { istrBraPc[47:24], tBraDisp8[23:0] };

`ifndef jx2_prebra_basepc
		tPreBraPc	= { istrBraPc[47:12], tBraDisp8[11:0] };
`else
		tPreBraPc	= { istrBasePc[47:12], tBraDisp8[11:0] };
`endif

		if((tPreBraPc[31:0]!=istrBraPc[31:0]+{tDisp8[30:0],1'b0}) && 
				!tBraDisp8[12])
			$display("DecPreBra: Disp8 Mismatch %X %X",
				tPreBraPc[31:0],
				istrBraPc[31:0]+{tDisp8[30:0],1'b0});

	end

	if(tIsBra20 || tIsBraCc20)
	begin
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp20[31:0] };

`ifndef jx2_prebra_basepc
		tPreBraPc	= { istrBraPc[47:24], tBraDisp20[23:0] };
`else
		tPreBraPc	= { istrBasePc[47:24], tBraDisp20[23:0] };
`endif

		if((tPreBraPc[31:0]!=(istrBraPc[31:0]+{tDisp20[30:0],1'b0})) && 
				!tBraDisp20[24])
			$display("DecPreBra: Disp20 Mismatch %X %X",
				tPreBraPc[31:0],
				istrBraPc[31:0]+{tDisp20[30:0],1'b0});

	end

`ifdef jx2_alu_jcmpz
	if(tIsBraCc8B)
	begin
`ifndef jx2_prebra_basepc
		tPreBraPc	= { istrBraPc[47:12], tBraDisp8B[11:0] };
`else
		tPreBraPc	= { istrBasePc[47:12], tBraDisp8B[11:0] };
`endif
	end

	if(tIsBraCc11B)
	begin
`ifndef jx2_prebra_basepc
		tPreBraPc	= { istrBraPc[47:16], tBraDisp11B[15:0] };
`else
		tPreBraPc	= { istrBasePc[47:16], tBraDisp11B[15:0] };
`endif
	end
`endif

`ifdef jx2_enable_riscv
	if(tIsBraRv12)
	begin
		tPreBraPc	= { istrBasePc[47:16], tBraDispRv12[15:0] };
	end

	if(tIsBraRv20)
	begin
		tPreBraPc	= { istrBasePc[47:24], tBraDispRv20[23:0] };
	end
`endif


//	if(tIsBra8)
	if(tIsBra8 || tDoBraCc8)
	begin
//		$display("PreBra: BRA8, PC=%X I=%X-%X PC2=%X",
//			istrBasePc,
//			istrWord[15:0], istrWord[31:16],
//			tBraDisp8);
//		tPreBraPc	= tBraDisp8;
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp8 };
		tNonBra		= 0;
		tPreBra		= 1;
		
//		if(tBraDisp8[32])
//		if(tBraDisp8[24])
		if(tBraDisp8[12])
		begin
//			$display("PreBra: Reject Cc8");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end
	
//	if(tIsBra20)
	if(tIsBra20 || tDoBraCc20)
	begin
//		$display("PreBra: BRA20, PC=%X I=%X-%X Disp=%X PC2=%X",
//			istrBasePc,
//			istrWord[15:0], istrWord[31:16], tDisp20, tBraDisp20);
//		tPreBraPc	= tBraDisp20;
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp20 };
		tNonBra		= 0;
		tPreBra		= 1;

//		if(tBraDisp20[32])
		if(tBraDisp20[24])
		begin
//			$display("PreBra: Reject Cc20");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end
	
`ifdef jx2_alu_jcmpz
	if(tDoBraCc8B)
	begin
		tNonBra		= 0;
		tPreBra		= 1;
		if(tBraDisp8B[12])
		begin
//			$display("PreBra: Reject Cc8B");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end

	if(tDoBraCc11B)
	begin
		tNonBra		= 0;
		tPreBra		= 1;
		if(tBraDisp11B[16])
		begin
//			$display("PreBra: Reject Cc11B");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end
`endif

`ifdef jx2_enable_riscv
	if(tDoBraRv12)
	begin
		tNonBra		= 0;
		tPreBra		= 1;
		if(tBraDispRv12[16])
		begin
//			$display("PreBra: Reject Rv12");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end

	if(tIsBraRv20)
	begin
		tNonBra		= 0;
		tPreBra		= 1;

		if(tBraDispRv20[24])
		begin
//			$display("PreBra: Reject Rv20");
			tNonBra		= 1;
			tPreBra		= 0;
		end
	end
`endif

`ifdef jx2_prebra_rtsu
	if(tIsRtsu)
	begin
//		$display("PreBra: RTSU, I=%X-%X PC2=%X",
//			istrWord[15:0], istrWord[31:16], regValLr);
		tPreBraPc	= { regValLr[47:1], 1'b0 };
//		tPreBra		= 1;
//		tPreBra		= !regValLr[0] ||
//			(regValLr[1] && (regValLr[50]==pipeHasLr[4]));
		tPreBra		=
			(	(regValLr[51:50]==pipeHasLr[5:4]) && 
				(regValLr[55:54]==pipeHasLr[7:6]) &&
				regValLr[0]);
		tNonBra		= !tPreBra;

//		if(tNonBra)
//		begin
//			$display("PreBra: Reject RTS");
//		end
	end
`endif

`ifdef jx2_prebra_rts
// `ifndef def_true
	if(tIsRtsR1)
	begin
		tPreBraPc	= { regValDhr[47:1], 1'b0 };
//		tPreBra		= 1;
//		tPreBra		= !regValDhr[0];
//		tPreBra		= !regValDhr[0] ||
//			(regValDhr[1] && (regValDhr[50]==pipeHasLr[4]));
		tPreBra		=
			(	(regValDhr[51:50]==pipeHasLr[5:4]) && 
				(regValDhr[55:54]==pipeHasLr[7:6]) &&
				regValDhr[0] );
		tNonBra		= !tPreBra;

//		if(tNonBra)
//			$display("PreBra: Reject JMP-R1");
	end
`endif

//	if(tPreBraPc[19:12] != istrBraPc[19:12])
//	if(tPreBraPc[21:14] != istrBraPc[21:14])
//		tPreBra		= 0;

`ifndef def_true
	if(istrBasePc[4:1]==istrBraPc[4:1])
	begin
		if(!tNonBra)
			$display("PreBra: Reject NOP Stall");

		tPreBra		= 0;
		tNonBra		= 1;
	end
`endif

//	tPreBraPcInc	= {
//		istrBraPc[47:24],
//		tPreBraPc[23: 0] + 24'h000010 };

//	tBraDispInc = {1'b0, tPreBraPc[11: 0]} + 13'h0010;
	tBraDispInc = { {1'b0, tPreBraPc[11: 4]} + 9'h001, 4'b0 };
	tPreBraPcInc	= { istrBraPc[47:12], tBraDispInc[11:0] };

	if(tBraDispInc[12])
	begin
//		if(!tNonBra)
//			$display("PreBra: Reject Inc-16");
		tNonBra		= 1;
		tPreBra		= 0;
	end

	if(pipeHasLr[2])
//	if(1'b1)
	begin
//		if(!tNonBra)
//			$display("PreBra: Reject LR2, BPC=%X", ifBraBPc);

		tPreBra		= 0;
		tNonBra		= 1;
	end
	else
	begin
//		if(!tNonBra)
//			$display("PreBra: Do BRA T=%X, BPC=%X", tPreBra, ifBraBPc);
	end

//	tPreBra		= 0;

`ifndef def_true
//	if(pipeHasLr[4])
	if(pipeHasLr[4] && !istrMTag[0])
	begin
		if(tPreBra)
		begin
			$display("DecPreBra: Predict Branch in RV Mode");
		end
	end
`endif

end

always @*
begin
//	if(preIsBra!=2'b10)
//		$display("PreBra: preIsBra %X", preIsBra);
end

always @(posedge clock)
begin
	tHistBits	<= tHistBitsB;
	tPreExBit	<= tPreExBitB;

	tStatBraTot	<= tNxtStatBraTot;
	tStatBraHit	<= tNxtStatBraHit;


	tPreIfCnt	<= preCnts[tPreIfIx];
	tPreIdIx	<= tPreIfIx;

	tPreExCnt			<= preCnts[tPreExIxA];
	preCnts[tPreExIx]	<= tPreExCntB;
	tPreExIx			<= tPreExIxA;
	tPreExDir			<= exBraDir;
	tPreExBPc			<= exBraBPc;

`ifdef jx2_prebra_do_vtlb
	if(tDoVtlbSt)
	begin
		tVtlbArr[tVtlbStIx]		<= tVtlbStVal;
	end
`endif

end

endmodule

`endif

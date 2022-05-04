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

`ifndef HAS_DECPREBRA
`define HAS_DECPREBRA

module DecPreBra(
	clock,		reset,
	istrWord,	istrBasePc,	istrBraPc,
	preBraPc,	preIsBra,
	regValLr,	ifBraBPc,
	regValDlr,	regValDhr,
	exBraBPc,	exBraDir,
	pipeHasLr,	regMemOpm);

input			clock;
input			reset;
input[63:0]		istrWord;	//Instruction Word
input[47:0]		istrBasePc;	//Instruction Base PC
input[47:0]		istrBraPc;	//Branch Base PC
input[63:0]		regValLr;	//Link Register
input[63:0]		regValDlr;	//Link Register
input[63:0]		regValDhr;	//Link Register
input[4:0]		regMemOpm;	//OPM

input[47:0]		ifBraBPc;	//Fetch Branch PC
input[47:0]		exBraBPc;	//Fetch Base PC
input[2:0]		exBraDir;
input[7:0]		pipeHasLr;

output[47:0]	preBraPc;
output[1:0]		preIsBra;

reg[47:0]	tPreBraPc;
reg			tPreBra;
assign	preBraPc	= tPreBraPc;
assign	preIsBra	= { 1'b0, tPreBra };


// reg[32:0]	tBraDisp8;
// reg[32:0]	tBraDisp20;
reg[24:0]	tBraDisp8;
reg[24:0]	tBraDisp20;

reg[31:0]	tDisp8;
reg[31:0]	tDisp20;

reg[11:0]	tBraDisp8HiP0;
reg[11:0]	tBraDisp8HiP1;
reg[11:0]	tBraDisp8HiN1;

reg[20:0]	tBraDisp8Lo;
reg[20:0]	tBraDisp20Lo;

reg			tIsBra8;		//Unconditional Branch (8-bit Disp)
reg			tIsBra20;		//Unconditional Branch (16-bit Disp)
reg			tIsRtsu;
reg			tIsRtsR1;

reg			tIsBraCc8;		//Conditional Branch (8-bit Disp)
reg			tIsBraCc20;		//Conditional Branch (20-bit Disp)

reg			tIsBraCcP20;	//Conditional Branch (20-bit Disp)
reg			tIsBraCcF20;	//Conditional Branch (20-bit Disp)

reg			tDoBraCc8;		//Take Conditional Branch (Cc8)
reg			tDoBraCc20;		//Take Conditional Branch (Cc20)

reg[2:0]	preCnts[63:0];
// reg[2:0]	preCnts[4095:0];
reg[2:0]	tPreIfCnt;
reg[2:0]	tPreExCnt;
reg[2:0]	tPreExCntB;
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

reg[5:0]	tVtlbIx;
reg[17:0]	tVtlbArr[63:0];
reg[17:0]	tVtlbStVal;
reg[5:0]	tVtlbStIx;
reg			tDoVtlbSt;

always @*
begin
//	tPreBraPc	= UV48_XX;
	tPreBraPc	= istrBraPc;
	tPreBra		= 0;
	tHistBitsB	= tHistBits;
	
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

//		$display("PreBra: Predict State %X, BPc=%X",
//			{tPreExDir[2], tPreExCnt[2:0]},
//			tPreExBPc);
	end
		
	tDisp8		= { istrWord[7]?UV24_FF:UV24_00, istrWord[7:0] };
	tDisp20		= {
		istrWord[7]?UV12_FF:UV12_00,
		istrWord[7:0], istrWord[27:16] };
	
//	tBraDisp8	= {1'b0, istrBraPc[31:0] } + { tDisp8[31:0], 1'b0 };
//	tBraDisp20	= {1'b0, istrBraPc[31:0] } + { tDisp20[31:0], 1'b0 };
	tBraDisp8	= {1'b0, istrBraPc[23:0] } + { tDisp8[23:0], 1'b0 };
	tBraDisp20	= {1'b0, istrBraPc[23:0] } + { tDisp20[23:0], 1'b0 };

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

	tIsBra8		=
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b000);
//		(istrWord[11: 8]==4'b0000);
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

	tIsBraCc8		=
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b001);

//	tIsBraCc20		=
	tIsBraCcF20		=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) &&
		(istrWord[31:29]==3'b111);

	tIsBraCc20		= tIsBraCcF20 || tIsBraCcP20;
	
`ifdef jx2_prebra_rts
	tIsRtsu			=
		(istrWord[15:0] == 16'h3012) ||
		((istrWord[15:0] == 16'h3010) && !pipeHasLr[0]);
//	tIsRtsR1		= 0;
	tIsRtsR1		=
		(istrWord[15:0] == 16'h3210) && !pipeHasLr[1];
`else
	tIsRtsu			=
		(istrWord[15:0] == 16'h3012);
	tIsRtsR1		= 0;
`endif

	if(pipeHasLr[4])
	begin
		/* RISC-V */
		tIsBraCc8	= 0;
		tIsBraCc20	= 0;
		tIsRtsu		= 0;
		tIsRtsR1	= 0;
	end

//	tIsBra8		= 0;
//	tIsBra20	= 0;
//	tIsRtsu		= 0;

	tDoBraCc8	= 0;
	tDoBraCc20	= 0;

// `ifndef def_true
`ifdef def_true
	if(tIsBraCc8 && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc8 = tPreExCntB[1] ? tPreBit : tPreIfCnt[2];
		tDoBraCc8 = tPreIfCnt[2] ^ tPreIfCnt[1];
		
//		tDoBraCc8 = tPreBit;
//		tDoBraCc8 = tPreIfCnt[2];
//		tDoBraCc8 = !tPreIfCnt[2];
//		$display("PreBra: Predict 8, Do=%d, BPc=%X",
//			tDoBraCc8, istrBasePc);
	end

	if(tIsBraCc20 && (tPreIbIx==tPreIdIx))
	begin
//		tDoBraCc20 = tPreExCntB[1] ? tPreBit : tPreIfCnt[2];
		tDoBraCc20 = tPreIfCnt[2] ^ tPreIfCnt[1];
		
//		tDoBraCc20 = tPreBit;
//		tDoBraCc20 = tPreIfCnt[2];
//		tDoBraCc20 = !tPreIfCnt[2];
//		$display("PreBra: Predict 20, Do=%d", tDoBraCc20);
	end
`endif
	
	if(tIsBra8 || tIsBraCc8)
	begin
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp8[31:0] };
		tPreBraPc	= { istrBraPc[47:24], tBraDisp8[23:0] };
	end

	if(tIsBra20 || tIsBraCc20)
	begin
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp20[31:0] };
		tPreBraPc	= { istrBraPc[47:24], tBraDisp20[23:0] };
	end

//	if(tIsBra8)
	if(tIsBra8 || tDoBraCc8)
	begin
//		$display("PreBra: BRA8, PC=%X I=%X-%X PC2=%X",
//			istrBasePc,
//			istrWord[15:0], istrWord[31:16],
//			tBraDisp8);
//		tPreBraPc	= tBraDisp8;
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp8 };
		tPreBra		= 1;
		
//		if(tBraDisp8[32])
		if(tBraDisp8[24])
			tPreBra		= 0;
	end

//	if(tIsBra20)
	if(tIsBra20 || tDoBraCc20)
	begin
//		$display("PreBra: BRA20, I=%X-%X PC2=%X",
//			istrWord[15:0], istrWord[31:16], tBraDisp20);
//		tPreBraPc	= tBraDisp20;
//		tPreBraPc	= { istrBraPc[47:32], tBraDisp20 };
		tPreBra		= 1;

//		if(tBraDisp20[32])
		if(tBraDisp20[24])
			tPreBra		= 0;
	end
	
	if(tIsRtsu)
	begin
//		$display("PreBra: RTSU, I=%X-%X PC2=%X",
//			istrWord[15:0], istrWord[31:16], regValLr);
		tPreBraPc	= regValLr[47:0];
		tPreBra		= 1;
	end

`ifdef jx2_prebra_rts
// `ifndef def_true
	if(tIsRtsR1)
	begin
		tPreBraPc	= regValDhr[47:0];
		tPreBra		= 1;
	end
`endif

//	if(tPreBraPc[19:12] != istrBraPc[19:12])
//	if(tPreBraPc[21:14] != istrBraPc[21:14])
//		tPreBra		= 0;

	if(pipeHasLr[2])
		tPreBra		= 0;

//	tPreBra		= 0;
end

always @(posedge clock)
begin
	tHistBits	<= tHistBitsB;
	tPreExBit	<= tPreExBitB;

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

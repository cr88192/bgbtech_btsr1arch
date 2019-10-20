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

module DecPreBra(
	clock,		reset,
	istrWord,	istrBasePc,	istrBraPc,
	preBraPc,	preIsBra,
	regValLr,	ifBraBPc,
	exBraBPc,	exBraDir);

input			clock;
input			reset;
input[63:0]		istrWord;	//Instruction Word
input[31:0]		istrBasePc;	//Instruction Base PC
input[31:0]		istrBraPc;	//Branch Base PC
input[31:0]		regValLr;	//Link Register

input[31:0]		ifBraBPc;	//Fetch Branch PC
input[31:0]		exBraBPc;	//Fetch Base PC
input[2:0]		exBraDir;

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

reg			tIsBra8;		//Unconditional Branch (8-bit Disp)
reg			tIsBra20;		//Unconditional Branch (16-bit Disp)
reg			tIsRtsu;

reg			tIsBraCc8;		//Conditional Branch (8-bit Disp)
reg			tIsBraCc20;		//Conditional Branch (20-bit Disp)
reg			tDoBraCc8;		//Take Conditional Branch (Cc8)
reg			tDoBraCc20;		//Take Conditional Branch (Cc20)

reg[2:0]	preCnts[63:0];
// reg[2:0]	preCnts[4095:0];
reg[2:0]	tPreIfCnt;
reg[2:0]	tPreExCnt;
reg[2:0]	tPreExCntB;
reg[2:0]	tPreExDir;
reg[31:0]	tPreExBPc;

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

always @*
begin
	tPreBraPc	= UV32_XX;
	tPreBra		= 0;
	tHistBitsB	= tHistBits;
	
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
	
	tBraDisp8	= istrBraPc + { tDisp8[30:0], 1'b0 };
	tBraDisp20	= istrBraPc + { tDisp20[30:0], 1'b0 };
	
	tIsBra8		=
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b000);
//		(istrWord[11: 8]==4'b0000);
	tIsBra20		=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) &&
		(istrWord[31:29]==3'b110);
//		(istrWord[31:28]==4'b1100);

	tIsBraCc8		=
		(istrWord[15:12]==4'h2) &&
		(istrWord[11: 9]==3'b001);
	tIsBraCc20		=
		(istrWord[15:12]==4'hF) &&
		(istrWord[11: 8]==4'h0) &&
		(istrWord[31:29]==3'b111);
	
	tIsRtsu			=
		(istrWord[15:0] == 16'h3012);

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
	
//	if(tIsBra8)
	if(tIsBra8 || tDoBraCc8)
	begin
//		$display("PreBra: BRA8, PC=%X I=%X-%X PC2=%X",
//			istrBasePc,
//			istrWord[15:0], istrWord[31:16],
//			tBraDisp8);
		tPreBraPc	= tBraDisp8;
		tPreBra		= 1;
	end

//	if(tIsBra20)
	if(tIsBra20 || tDoBraCc20)
	begin
//		$display("PreBra: BRA20, I=%X-%X PC2=%X",
//			istrWord[15:0], istrWord[31:16], tBraDisp20);
		tPreBraPc	= tBraDisp20;
		tPreBra		= 1;
	end
	
	if(tIsRtsu)
	begin
//		$display("PreBra: RTSU, I=%X-%X PC2=%X",
//			istrWord[15:0], istrWord[31:16], regValLr);
		tPreBraPc	= regValLr;
		tPreBra		= 1;
	end

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
end

endmodule
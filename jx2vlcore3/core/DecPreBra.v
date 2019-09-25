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

reg[5:0]	preBits[63:0];
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

always @*
begin
	tPreBraPc	= UV32_XX;
	tPreBra		= 0;
	
	tPreIfIx	= ifBraBPc[6:1];
	tPreIbIx	= istrBasePc[6:1];
	tPreExIxA	= exBraBPc[6:1];

//	tPreIfIx	= ifBraBPc[12:1];
//	tPreIbIx	= istrBasePc[12:1];
//	tPreExIxA	= exBraBPc[12:1];
	
	tPreExCntB	= tPreExCnt;
	tPreExBitB	= tPreExBit;

	if(tPreExDir[1])
	begin
		tPreExBitB = { tPreExBit[4:0], tPreExDir[2] };
	
		case( {tPreExDir[2], tPreExCnt[2:0]} )
			4'b0000: tPreExCntB=3'b001;
			4'b0001: tPreExCntB=3'b010;
			4'b0010: tPreExCntB=3'b011;
			4'b0011: tPreExCntB=3'b011;
			4'b0100: tPreExCntB=3'b101;
			4'b0101: tPreExCntB=3'b110;
			4'b0110: tPreExCntB=3'b111;
			4'b0111: tPreExCntB=3'b000;
			4'b1000: tPreExCntB=3'b111;
			4'b1001: tPreExCntB=3'b000;
			4'b1010: tPreExCntB=3'b001;
			4'b1011: tPreExCntB=3'b010;
			4'b1100: tPreExCntB=3'b100;
			4'b1101: tPreExCntB=3'b100;
			4'b1110: tPreExCntB=3'b101;
			4'b1111: tPreExCntB=3'b110;
		endcase
		
//		$display("PreBra: Predict State %X, BPc=%X",
//			{tPreExDir[2], tPreExCnt[2:0]},
//			tPreExBPc);
	end
	
/*
0..7
0F0F 000F 00FF 0FFF  000F 00FF 0FF0 0FFF
0F0F 000F 00FF 0F7F  100F 00FF 0FF0 0FFF
0F0F 000F 00FF 0FBF  F00F 10FF 0FF0 0FFF
0F0F 000F 00FF 0DFF  F00F 80FF 0CF0 0FFF
0F0F 000F 00FE 0FFF  000F 00FF 0EF0 0FFF
0F0F 000F 00FF 0FFF  200F 00FF 0FF0 0FFF
0F0F 000F 00FF 0FDF  000F 00FF 0FF0 0FFF
0E0F 000F 00FF 0FCF  000F 00FF 0FF0 0FFF

000F 000F 00F6 0D0F  0000 F0FF 0FF0 0FFF
000F 000F 00F1 0F0F  0000 F0FF 0FF0 0FFF
000F 000F 00F4 0F0F  00C2 F0FF 0FF0 2FFF

8..F
03CF F0FF 08F5 0FFF  0FFF FCFF 0FFF FFFF
01CF F09F 03F1 0FFF  0FEF EFFF 0FFF FFFF
00EF F0FF 07F2 0FFF  0FFF FDF0 0FFF FF0F
005F F0FF 0CFB 0FFF  0FFF F2FF 0FFF FFFF
000F F0FF 0FFF 0FFF  0FFF FBFF 0FFF FFFF
000F F0FF 0DF6 0FFF  0FFF EFFF 0FFF FFFF
030F F0FF 0BFF 0FFF  0FFF F5FF 0FFF FFFF
0F0F E0FF 0FFF 0FFF  0FFF F3FF 0FFF FFFF
000F F00F 00F0 FFFF  0D0F FFFF 0FFF FFFF
000F F00F 00F0 FFFF  0A0F FFF0 0FFF FFFF
000F F01F 00F0 FFFF  0F2F C2F0 0FFF FFFF
*/

	case(tPreIfBit)
		6'b000000: tPreBit=0;	6'b000001: tPreBit=0;
		6'b000010: tPreBit=0;	6'b000011: tPreBit=1;
		6'b000100: tPreBit=0;	6'b000101: tPreBit=0;
		6'b000110: tPreBit=0;	6'b000111: tPreBit=1;
		6'b001000: tPreBit=0;	6'b001001: tPreBit=0;
		6'b001010: tPreBit=1;	6'b001011: tPreBit=1;
		6'b001100: tPreBit=0;	6'b001101: tPreBit=1;
		6'b001110: tPreBit=0;	6'b001111: tPreBit=1;

		6'b010000: tPreBit=1;	6'b010001: tPreBit=0;
		6'b010010: tPreBit=0;	6'b010011: tPreBit=1;
		6'b010100: tPreBit=0;	6'b010101: tPreBit=0;
		6'b010110: tPreBit=1;	6'b010111: tPreBit=1;
		6'b011000: tPreBit=0;	6'b011001: tPreBit=1;
		6'b011010: tPreBit=1;	6'b011011: tPreBit=0;
		6'b011100: tPreBit=0;	6'b011101: tPreBit=1;
		6'b011110: tPreBit=1;	6'b011111: tPreBit=1;

		6'b100000: tPreBit=0;	6'b100001: tPreBit=0;
		6'b100010: tPreBit=0;	6'b100011: tPreBit=1;
		6'b100100: tPreBit=1;	6'b100101: tPreBit=0;
		6'b100110: tPreBit=1;	6'b100111: tPreBit=1;
		6'b101000: tPreBit=0;	6'b101001: tPreBit=1;
		6'b101010: tPreBit=1;	6'b101011: tPreBit=0;
		6'b101100: tPreBit=0;	6'b101101: tPreBit=1;
		6'b101110: tPreBit=1;	6'b101111: tPreBit=1;

		6'b110000: tPreBit=0;	6'b110001: tPreBit=1;
		6'b110010: tPreBit=1;	6'b110011: tPreBit=1;
		6'b110100: tPreBit=1;	6'b110101: tPreBit=1;
		6'b110110: tPreBit=1;	6'b110111: tPreBit=1;
		6'b111000: tPreBit=0;	6'b111001: tPreBit=1;
		6'b111010: tPreBit=1;	6'b111011: tPreBit=1;
		6'b111100: tPreBit=1;	6'b111101: tPreBit=1;
		6'b111110: tPreBit=1;	6'b111111: tPreBit=1;
	endcase
	
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
		tDoBraCc8 = tPreBit;
//		tDoBraCc8 = tPreIfCnt[2];
//		tDoBraCc8 = !tPreIfCnt[2];
//		$display("PreBra: Predict 8, Do=%d, BPc=%X",
//			tDoBraCc8, istrBasePc);
	end

	if(tIsBraCc20 && (tPreIbIx==tPreIdIx))
	begin
		tDoBraCc20 = tPreBit;
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
	tPreIfCnt	<= preCnts[tPreIfIx];
	tPreIfBit	<= preBits[tPreIfIx];
	tPreIdIx	<= tPreIfIx;

	tPreExCnt			<= preCnts[tPreExIxA];
	tPreExBit			<= preBits[tPreExIxA];
	preCnts[tPreExIx]	<= tPreExCntB;
	preBits[tPreExIx]	<= tPreExBitB;
	tPreExIx			<= tPreExIxA;
	tPreExDir			<= exBraDir;
	tPreExBPc			<= exBraBPc;
end

endmodule
`include "CoreDefs.v"


`ifndef HAS_JX2EXOPSWAPN
`define HAS_JX2EXOPSWAPN

module ExSwapN(clock, reset,
	regIdIxt,
	regValRm, regOutVal);

input			clock;
input			reset;

input [ 7:0]	regIdIxt;
input [63:0]	regValRm;
output[63:0]	regOutVal;

reg[63:0]		tRegOutVal;
assign		regOutVal = tRegOutVal;

reg[7:0]		tLowByte;
reg[15:0]		tLowWord;

reg[7:0]		tSelByte;
reg[15:0]		tSelWord;
reg				tSelIs0;
reg				tSelIs1;
reg				tSelIs2;
reg				tSelIs3;

reg[63:0]		tSwapByte;
reg[63:0]		tSwapWord;

always @*
begin
	tLowByte=regValRm[ 7:0];
	tLowWord=regValRm[15:0];

	tSelIs0=0;
	tSelIs1=0;
	tSelIs2=0;
	tSelIs3=0;

	case(regIdIxt[3:2])
		2'b00: begin
			tSelByte=UV8_XX;
			tSelWord=UV16_XX;
			tSelIs0=1;
		end
		2'b01: begin
			tSelByte=regValRm[15: 8];
			tSelWord=regValRm[31:16];
			tSelIs1=1;
		end
		2'b10: begin
			tSelByte=regValRm[23:16];
			tSelWord=regValRm[47:32];
			tSelIs2=1;
		end
		2'b11: begin
			tSelByte=regValRm[31:24];
			tSelWord=regValRm[63:48];
			tSelIs3=1;
		end
	endcase
	
	tSwapByte[ 7: 0]=regIdIxt[1]	?tLowByte:tSelByte;
	tSwapByte[15: 8]=tSelIs1		?tSelByte:regValRm[15: 8];
	tSwapByte[23:16]=tSelIs2		?tSelByte:regValRm[23:16];
	tSwapByte[31:24]=tSelIs3		?tSelByte:regValRm[31:24];
	tSwapByte[63:31]=regValRm[63:31];

	tSwapWord[15: 0]=regIdIxt[1]	?tLowWord:tSelWord;
	tSwapWord[31:16]=tSelIs1		?tSelWord:regValRm[31:16];
	tSwapWord[47:32]=tSelIs2		?tSelWord:regValRm[47:32];
	tSwapWord[63:48]=tSelIs3		?tSelWord:regValRm[63:48];

	if(tSelIs0)
	begin
		case(regIdIxt[1:0])
			2'b00: begin		// SWAP.B
				tRegOutVal	= {
					regValRm[63:32],
					regValRm[23:16], regValRm[31:24],
					regValRm[ 7: 0], regValRm[15: 8] };
			end
			2'b01: begin		// SWAP.W
				tRegOutVal	= {
					regValRm[47:32], regValRm[63:48],
					regValRm[15: 0], regValRm[31:16] };
			end
			2'b10: begin		// SWAP.8B
				tRegOutVal	= {
					regValRm[55:48], regValRm[63:56],
					regValRm[39:32], regValRm[47:40],
					regValRm[23:16], regValRm[31:24],
					regValRm[ 7: 0], regValRm[15: 8] };
			end
			2'b11: begin		// SWAP.L
				tRegOutVal	= {
					regValRm[31:0], regValRm[63:32] };
			end
		endcase
	end
	else
	begin
		tRegOutVal	= regIdIxt[0] ?
			tSwapWord : tSwapByte;
	end

end

endmodule

`endif

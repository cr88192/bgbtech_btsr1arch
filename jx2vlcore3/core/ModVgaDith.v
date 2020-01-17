module ModVgaDith(posX, posY, frNum, pixFrac, dithRndUp);
input[1:0]	posX;
input[1:0]	posY;
input[1:0]	frNum;
input[3:0]	pixFrac;
output		dithRndUp;

reg			tDithRndUp;
assign	dithRndUp = tDithRndUp;

reg[15:0]	tBayerMask;
reg[3:0]	tBayerIx;
reg[1:0]	tDithPixXor;

always @*
begin
`ifdef def_true
	case(pixFrac)
		4'h0: tBayerMask=16'h0000;
		4'h1: tBayerMask=16'h8000;
		4'h2: tBayerMask=16'h8020;
		4'h3: tBayerMask=16'hA020;
		4'h4: tBayerMask=16'hA0A0;
		4'h5: tBayerMask=16'hA4A0;
		4'h6: tBayerMask=16'hA4A1;
		4'h7: tBayerMask=16'hA5A1;
		4'h8: tBayerMask=16'hA5A5;
		4'h9: tBayerMask=16'hE5A5;
		4'hA: tBayerMask=16'hE5B5;
		4'hB: tBayerMask=16'hF5B5;
		4'hC: tBayerMask=16'hF5F5;
		4'hD: tBayerMask=16'hFBF5;
		4'hE: tBayerMask=16'hFBF7;
		4'hF: tBayerMask=16'hFBFF;
	endcase
	
	tBayerIx = { posY, posX } ^ { frNum, frNum };
	tDithRndUp = tBayerMask[tBayerIx];
`endif

`ifndef def_true
	tDithPixXor = posX[1:0] ^ posY[1:0];
	case({frNum^tDithPixXor, pixFrac[3:1]})
		/* 75.0% */
		5'b00000: tDithRndUp = 0;		5'b00001: tDithRndUp = 0;
		5'b00010: tDithRndUp = 0;		5'b00011: tDithRndUp = 0;
		5'b00100: tDithRndUp = 0;		5'b00101: tDithRndUp = 0;
		5'b00110: tDithRndUp = 1;		5'b00111: tDithRndUp = 1;
		/* 37.5% */
		5'b01000: tDithRndUp = 0;		5'b01001: tDithRndUp = 0;
		5'b01010: tDithRndUp = 0;		5'b01011: tDithRndUp = 1;
		5'b01100: tDithRndUp = 1;		5'b01101: tDithRndUp = 1;
		5'b01110: tDithRndUp = 1;		5'b01111: tDithRndUp = 1;
		/* 25.0% */
		5'b10000: tDithRndUp = 0;		5'b10001: tDithRndUp = 0;
		5'b10010: tDithRndUp = 1;		5'b10011: tDithRndUp = 1;
		5'b10100: tDithRndUp = 1;		5'b10101: tDithRndUp = 1;
		5'b10110: tDithRndUp = 1;		5'b10111: tDithRndUp = 1;
		/* 62.5% */
		5'b11000: tDithRndUp = 0;		5'b11001: tDithRndUp = 0;
		5'b11010: tDithRndUp = 0;		5'b11011: tDithRndUp = 0;
		5'b11100: tDithRndUp = 0;		5'b11101: tDithRndUp = 1;
		5'b11110: tDithRndUp = 1;		5'b11111: tDithRndUp = 1;
	endcase
`endif

end

endmodule

/*
Drive output to a 7-segment display.

Goal is to display a hexadecimal value via a 7-segment display.

*/

module Mod7Seg(
	clock,		reset,
	dispVal,	inStrobe1kHz,	inStrobeNoise,
	outCharBit,
	outSegBit
	);

input			clock;
input			reset;
input[31:0]		dispVal;
input			inStrobe1kHz;
input			inStrobeNoise;

output[7:0]		outCharBit;
output[7:0]		outSegBit;

reg[7:0]		tOutCharBit;
reg[7:0]		tOutSegBit;
reg[7:0]		tOutCharBit2;
reg[7:0]		tOutSegBit2;

assign		outCharBit = tOutCharBit2;
assign		outSegBit = tOutSegBit2;

reg[2:0]	tCharPos;
reg[2:0]	tNxtCharPos;
reg[3:0]	tHexNibA;
reg[3:0]	tHexNibB;
reg[7:0]	tHexSegBits;

reg[7:0]	tAccStrobe;
reg[7:0]	tNxtAccStrobe;
reg[7:0]	tDutyStrobe;
reg[7:0]	tNxtDutyStrobe;
reg			tStrobe;
reg			tNxtStrobe;

always @*
begin
//	tNxtDutyStrobe	= 8'h3F;
	tNxtDutyStrobe	= inStrobeNoise ? 8'h44 : 8'h3F;
//	tNxtDutyStrobe	= inStrobeNoise ? 8'h22 : 8'h1F;
//	tNxtDutyStrobe	= inStrobeNoise ? 8'h11 : 8'h0F;

	tOutCharBit		= 8'hFF;
	tOutSegBit		= 8'hFF;
	tNxtCharPos		= tCharPos + (inStrobe1kHz?1:0);
	
	{tNxtStrobe, tNxtAccStrobe}	=
		{1'b0, tAccStrobe} +
		{1'b0, tDutyStrobe};

	/* Attempt to misalign phase with redraw. */
//	if(inStrobe1kHz)
//		tNxtAccStrobe = ~tAccStrobe;
	
	if(tStrobe)
		tOutCharBit[tCharPos] = 0;

	case(tCharPos)
		3'b000: tHexNibA = dispVal[31:28];
		3'b001: tHexNibA = dispVal[27:24];
		3'b010: tHexNibA = dispVal[23:20];
		3'b011: tHexNibA = dispVal[19:16];
		3'b100: tHexNibA = dispVal[15:12];
		3'b101: tHexNibA = dispVal[11: 8];
		3'b110: tHexNibA = dispVal[ 7: 4];
		3'b111: tHexNibA = dispVal[ 3: 0];
	endcase
	
	case(tHexNibB)
		4'h0: tHexSegBits=8'h3F;
		4'h1: tHexSegBits=8'h06;
		4'h2: tHexSegBits=8'h5B;
		4'h3: tHexSegBits=8'h4F;
		4'h4: tHexSegBits=8'h66;
		4'h5: tHexSegBits=8'h6D;
		4'h6: tHexSegBits=8'h7D;
		4'h7: tHexSegBits=8'h07;
		4'h8: tHexSegBits=8'h7F;
		4'h9: tHexSegBits=8'h6F;
		4'hA: tHexSegBits=8'h77;
		4'hB: tHexSegBits=8'h7C;
		4'hC: tHexSegBits=8'h39;
		4'hD: tHexSegBits=8'h5E;
		4'hE: tHexSegBits=8'h79;
		4'hF: tHexSegBits=8'h71;
	endcase
	
	tOutSegBit = ~tHexSegBits;
//	tOutSegBit = tStrobe ? (~tHexSegBits) : 8'hFF;
	
end

always @(posedge clock)
begin
	tOutCharBit2	<= tOutCharBit;
	tOutSegBit2		<= tOutSegBit;
	tCharPos		<= tNxtCharPos;
	tHexNibB		<= tHexNibA;
	tAccStrobe		<= tNxtAccStrobe;
	tDutyStrobe		<= tNxtDutyStrobe;
	tStrobe			<= tNxtStrobe;
end

endmodule

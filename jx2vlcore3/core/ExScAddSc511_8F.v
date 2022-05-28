`ifndef HAS_EXSCADDSC511_8F
`define HAS_EXSCADDSC511_8F

module ExScAddSc511_8F(valA, valB, valC);

input[4:0]	valA;
input[4:0]	valB;
output[7:0]	valC;

reg[7:0]	tScA;
reg[7:0]	tScB;
reg[7:0]	tScC;

assign	valC = tScC;

always @*
begin
// `ifndef def_true
`ifdef def_true
	case(valA)
		5'h00: tScA=8'h00;		5'h01: tScA=8'h02;
		5'h02: tScA=8'h05;		5'h03: tScA=8'h08;
		5'h04: tScA=8'h0B;		5'h05: tScA=8'h0D;
		5'h06: tScA=8'h10;		5'h07: tScA=8'h13;
		5'h08: tScA=8'h16;		5'h09: tScA=8'h18;
		5'h0A: tScA=8'h1B;		5'h0B: tScA=8'h1E;
		5'h0C: tScA=8'h21;		5'h0D: tScA=8'h23;
		5'h0E: tScA=8'h26;		5'h0F: tScA=8'h29;
		5'h10: tScA=8'h2C;		5'h11: tScA=8'h2E;
		5'h12: tScA=8'h31;		5'h13: tScA=8'h34;
		5'h14: tScA=8'h37;		5'h15: tScA=8'h39;
		5'h16: tScA=8'h3C;		5'h17: tScA=8'h3F;
		5'h18: tScA=8'h42;		5'h19: tScA=8'h44;
		5'h1A: tScA=8'h47;		5'h1B: tScA=8'h4A;
		5'h1C: tScA=8'h4D;		5'h1D: tScA=8'h4F;
		5'h1E: tScA=8'h52;		5'h1F: tScA=8'h55;
	endcase

	case(valB)
		5'h00: tScB=8'h00;		5'h01: tScB=8'h05;
		5'h02: tScB=8'h0B;		5'h03: tScB=8'h10;
		5'h04: tScB=8'h16;		5'h05: tScB=8'h1B;
		5'h06: tScB=8'h21;		5'h07: tScB=8'h26;
		5'h08: tScB=8'h2C;		5'h09: tScB=8'h31;
		5'h0A: tScB=8'h37;		5'h0B: tScB=8'h3C;
		5'h0C: tScB=8'h42;		5'h0D: tScB=8'h47;
		5'h0E: tScB=8'h4D;		5'h0F: tScB=8'h52;
		5'h10: tScB=8'h58;		5'h11: tScB=8'h5D;
		5'h12: tScB=8'h63;		5'h13: tScB=8'h68;
		5'h14: tScB=8'h6E;		5'h15: tScB=8'h73;
		5'h16: tScB=8'h79;		5'h17: tScB=8'h7E;
		5'h18: tScB=8'h84;		5'h19: tScB=8'h89;
		5'h1A: tScB=8'h8F;		5'h1B: tScB=8'h94;
		5'h1C: tScB=8'h9A;		5'h1D: tScB=8'h9F;
		5'h1E: tScB=8'hA5;		5'h1F: tScB=8'hAA;
	endcase

	tScC = tScA + tScB;
`endif

`ifndef def_true
	tScC =
		{ 2'b0, valA, 1'b0 } +
		{ 1'b0, valB, 2'b0 } +
		{ 3'b0, valA } +
		{ 3'b0, valB } ;
`endif

end

endmodule

`endif

`ifndef HAS_EXSCADDSC511B_8F
`define HAS_EXSCADDSC511B_8F

module ExScAddSc511B_8F(valA, valB, valC);

input[7:0]	valA;
input[7:0]	valB;
output[7:0]	valC;

reg[7:0]	tScA;
reg[7:0]	tScB;
reg[7:0]	tScC;

assign	valC = tScC;

always @*
begin
	case(valA[7:2])
		6'h00: tScA=8'h00;	6'h01: tScA=8'h01;
		6'h02: tScA=8'h02;	6'h03: tScA=8'h04;
		6'h04: tScA=8'h05;	6'h05: tScA=8'h06;
		6'h06: tScA=8'h08;	6'h07: tScA=8'h09;
		6'h08: tScA=8'h0A;	6'h09: tScA=8'h0C;
		6'h0A: tScA=8'h0D;	6'h0B: tScA=8'h0E;
		6'h0C: tScA=8'h10;	6'h0D: tScA=8'h11;
		6'h0E: tScA=8'h12;	6'h0F: tScA=8'h14;
		6'h10: tScA=8'h15;	6'h11: tScA=8'h16;
		6'h12: tScA=8'h18;	6'h13: tScA=8'h19;
		6'h14: tScA=8'h1A;	6'h15: tScA=8'h1C;
		6'h16: tScA=8'h1D;	6'h17: tScA=8'h1E;
		6'h18: tScA=8'h20;	6'h19: tScA=8'h21;
		6'h1A: tScA=8'h22;	6'h1B: tScA=8'h24;
		6'h1C: tScA=8'h25;	6'h1D: tScA=8'h26;
		6'h1E: tScA=8'h28;	6'h1F: tScA=8'h29;
		6'h20: tScA=8'h2A;	6'h21: tScA=8'h2C;
		6'h22: tScA=8'h2D;	6'h23: tScA=8'h2E;
		6'h24: tScA=8'h30;	6'h25: tScA=8'h31;
		6'h26: tScA=8'h32;	6'h27: tScA=8'h34;
		6'h28: tScA=8'h35;	6'h29: tScA=8'h36;
		6'h2A: tScA=8'h38;	6'h2B: tScA=8'h39;
		6'h2C: tScA=8'h3A;	6'h2D: tScA=8'h3C;
		6'h2E: tScA=8'h3D;	6'h2F: tScA=8'h3E;
		6'h30: tScA=8'h40;	6'h31: tScA=8'h41;
		6'h32: tScA=8'h42;	6'h33: tScA=8'h44;
		6'h34: tScA=8'h45;	6'h35: tScA=8'h46;
		6'h36: tScA=8'h48;	6'h37: tScA=8'h49;
		6'h38: tScA=8'h4A;	6'h39: tScA=8'h4C;
		6'h3A: tScA=8'h4D;	6'h3B: tScA=8'h4E;
		6'h3C: tScA=8'h50;	6'h3D: tScA=8'h51;
		6'h3E: tScA=8'h52;	6'h3F: tScA=8'h54;
	endcase

	case(valB[7:2])
		6'h00: tScB=8'h00;	6'h01: tScB=8'h02;
		6'h02: tScB=8'h05;	6'h03: tScB=8'h08;
		6'h04: tScB=8'h0A;	6'h05: tScB=8'h0D;
		6'h06: tScB=8'h10;	6'h07: tScB=8'h12;
		6'h08: tScB=8'h15;	6'h09: tScB=8'h18;
		6'h0A: tScB=8'h1A;	6'h0B: tScB=8'h1D;
		6'h0C: tScB=8'h20;	6'h0D: tScB=8'h22;
		6'h0E: tScB=8'h25;	6'h0F: tScB=8'h28;
		6'h10: tScB=8'h2A;	6'h11: tScB=8'h2D;
		6'h12: tScB=8'h30;	6'h13: tScB=8'h32;
		6'h14: tScB=8'h35;	6'h15: tScB=8'h38;
		6'h16: tScB=8'h3A;	6'h17: tScB=8'h3D;
		6'h18: tScB=8'h40;	6'h19: tScB=8'h42;
		6'h1A: tScB=8'h45;	6'h1B: tScB=8'h48;
		6'h1C: tScB=8'h4A;	6'h1D: tScB=8'h4D;
		6'h1E: tScB=8'h50;	6'h1F: tScB=8'h52;
		6'h20: tScB=8'h55;	6'h21: tScB=8'h58;
		6'h22: tScB=8'h5A;	6'h23: tScB=8'h5D;
		6'h24: tScB=8'h60;	6'h25: tScB=8'h62;
		6'h26: tScB=8'h65;	6'h27: tScB=8'h68;
		6'h28: tScB=8'h6A;	6'h29: tScB=8'h6D;
		6'h2A: tScB=8'h70;	6'h2B: tScB=8'h72;
		6'h2C: tScB=8'h75;	6'h2D: tScB=8'h78;
		6'h2E: tScB=8'h7A;	6'h2F: tScB=8'h7D;
		6'h30: tScB=8'h80;	6'h31: tScB=8'h82;
		6'h32: tScB=8'h85;	6'h33: tScB=8'h88;
		6'h34: tScB=8'h8A;	6'h35: tScB=8'h8D;
		6'h36: tScB=8'h90;	6'h37: tScB=8'h92;
		6'h38: tScB=8'h95;	6'h39: tScB=8'h98;
		6'h3A: tScB=8'h9A;	6'h3B: tScB=8'h9D;
		6'h3C: tScB=8'hA0;	6'h3D: tScB=8'hA2;
		6'h3E: tScB=8'hA5;	6'h3F: tScB=8'hA8;
	endcase

	tScC = tScA + tScB;

end

endmodule

`endif

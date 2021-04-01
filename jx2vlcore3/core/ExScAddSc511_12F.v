`ifndef HAS_EXSCADDSC511_12F
`define HAS_EXSCADDSC511_12F

module ExScAddSc511_12F(valA, valB, valC);

input [11:0]	valA;
input [11:0]	valB;
output[11:0]	valC;

reg[ 7:0]	tScAh;
reg[ 7:0]	tScBh;

// reg[11:0]	tScAh;
// reg[11:0]	tScBh;
reg[ 7:0]	tScAl;
reg[ 7:0]	tScBl;

reg[11:0]	tScA;
reg[11:0]	tScB;
reg[11:0]	tScC;

assign	valC = tScC;

always @*
begin

// `ifndef def_true
`ifdef def_true
	case(valA[11:6])
		6'h00: tScAh=8'h00;	6'h01: tScAh=8'h01;
		6'h02: tScAh=8'h02;	6'h03: tScAh=8'h04;
		6'h04: tScAh=8'h05;	6'h05: tScAh=8'h06;
		6'h06: tScAh=8'h08;	6'h07: tScAh=8'h09;
		6'h08: tScAh=8'h0A;	6'h09: tScAh=8'h0C;
		6'h0A: tScAh=8'h0D;	6'h0B: tScAh=8'h0E;
		6'h0C: tScAh=8'h10;	6'h0D: tScAh=8'h11;
		6'h0E: tScAh=8'h12;	6'h0F: tScAh=8'h14;
		6'h10: tScAh=8'h15;	6'h11: tScAh=8'h16;
		6'h12: tScAh=8'h18;	6'h13: tScAh=8'h19;
		6'h14: tScAh=8'h1A;	6'h15: tScAh=8'h1C;
		6'h16: tScAh=8'h1D;	6'h17: tScAh=8'h1E;
		6'h18: tScAh=8'h20;	6'h19: tScAh=8'h21;
		6'h1A: tScAh=8'h22;	6'h1B: tScAh=8'h24;
		6'h1C: tScAh=8'h25;	6'h1D: tScAh=8'h26;
		6'h1E: tScAh=8'h28;	6'h1F: tScAh=8'h29;
		6'h20: tScAh=8'h2A;	6'h21: tScAh=8'h2C;
		6'h22: tScAh=8'h2D;	6'h23: tScAh=8'h2E;
		6'h24: tScAh=8'h30;	6'h25: tScAh=8'h31;
		6'h26: tScAh=8'h32;	6'h27: tScAh=8'h34;
		6'h28: tScAh=8'h35;	6'h29: tScAh=8'h36;
		6'h2A: tScAh=8'h38;	6'h2B: tScAh=8'h39;
		6'h2C: tScAh=8'h3A;	6'h2D: tScAh=8'h3C;
		6'h2E: tScAh=8'h3D;	6'h2F: tScAh=8'h3E;
		6'h30: tScAh=8'h40;	6'h31: tScAh=8'h41;
		6'h32: tScAh=8'h42;	6'h33: tScAh=8'h44;
		6'h34: tScAh=8'h45;	6'h35: tScAh=8'h46;
		6'h36: tScAh=8'h48;	6'h37: tScAh=8'h49;
		6'h38: tScAh=8'h4A;	6'h39: tScAh=8'h4C;
		6'h3A: tScAh=8'h4D;	6'h3B: tScAh=8'h4E;
		6'h3C: tScAh=8'h50;	6'h3D: tScAh=8'h51;
		6'h3E: tScAh=8'h52;	6'h3F: tScAh=8'h54;
	endcase

	case(valB[11:6])
		6'h00: tScBh=8'h00;	6'h01: tScBh=8'h02;
		6'h02: tScBh=8'h05;	6'h03: tScBh=8'h08;
		6'h04: tScBh=8'h0A;	6'h05: tScBh=8'h0D;
		6'h06: tScBh=8'h10;	6'h07: tScBh=8'h12;
		6'h08: tScBh=8'h15;	6'h09: tScBh=8'h18;
		6'h0A: tScBh=8'h1A;	6'h0B: tScBh=8'h1D;
		6'h0C: tScBh=8'h20;	6'h0D: tScBh=8'h22;
		6'h0E: tScBh=8'h25;	6'h0F: tScBh=8'h28;
		6'h10: tScBh=8'h2A;	6'h11: tScBh=8'h2D;
		6'h12: tScBh=8'h30;	6'h13: tScBh=8'h32;
		6'h14: tScBh=8'h35;	6'h15: tScBh=8'h38;
		6'h16: tScBh=8'h3A;	6'h17: tScBh=8'h3D;
		6'h18: tScBh=8'h40;	6'h19: tScBh=8'h42;
		6'h1A: tScBh=8'h45;	6'h1B: tScBh=8'h48;
		6'h1C: tScBh=8'h4A;	6'h1D: tScBh=8'h4D;
		6'h1E: tScBh=8'h50;	6'h1F: tScBh=8'h52;
		6'h20: tScBh=8'h55;	6'h21: tScBh=8'h58;
		6'h22: tScBh=8'h5A;	6'h23: tScBh=8'h5D;
		6'h24: tScBh=8'h60;	6'h25: tScBh=8'h62;
		6'h26: tScBh=8'h65;	6'h27: tScBh=8'h68;
		6'h28: tScBh=8'h6A;	6'h29: tScBh=8'h6D;
		6'h2A: tScBh=8'h70;	6'h2B: tScBh=8'h72;
		6'h2C: tScBh=8'h75;	6'h2D: tScBh=8'h78;
		6'h2E: tScBh=8'h7A;	6'h2F: tScBh=8'h7D;
		6'h30: tScBh=8'h80;	6'h31: tScBh=8'h82;
		6'h32: tScBh=8'h85;	6'h33: tScBh=8'h88;
		6'h34: tScBh=8'h8A;	6'h35: tScBh=8'h8D;
		6'h36: tScBh=8'h90;	6'h37: tScBh=8'h92;
		6'h38: tScBh=8'h95;	6'h39: tScBh=8'h98;
		6'h3A: tScBh=8'h9A;	6'h3B: tScBh=8'h9D;
		6'h3C: tScBh=8'hA0;	6'h3D: tScBh=8'hA2;
		6'h3E: tScBh=8'hA5;	6'h3F: tScBh=8'hA8;
	endcase

	case(valA[5:0])
		6'h00: tScAl=8'h00;	6'h01: tScAl=8'h00;
		6'h02: tScAl=8'h00;	6'h03: tScAl=8'h00;
		6'h04: tScAl=8'h01;	6'h05: tScAl=8'h01;
		6'h06: tScAl=8'h01;	6'h07: tScAl=8'h02;
		6'h08: tScAl=8'h02;	6'h09: tScAl=8'h02;
		6'h0A: tScAl=8'h03;	6'h0B: tScAl=8'h03;
		6'h0C: tScAl=8'h03;	6'h0D: tScAl=8'h04;
		6'h0E: tScAl=8'h04;	6'h0F: tScAl=8'h04;
		6'h10: tScAl=8'h05;	6'h11: tScAl=8'h05;
		6'h12: tScAl=8'h05;	6'h13: tScAl=8'h06;
		6'h14: tScAl=8'h06;	6'h15: tScAl=8'h06;
		6'h16: tScAl=8'h07;	6'h17: tScAl=8'h07;
		6'h18: tScAl=8'h07;	6'h19: tScAl=8'h08;
		6'h1A: tScAl=8'h08;	6'h1B: tScAl=8'h08;
		6'h1C: tScAl=8'h09;	6'h1D: tScAl=8'h09;
		6'h1E: tScAl=8'h09;	6'h1F: tScAl=8'h0A;
		6'h20: tScAl=8'h0A;	6'h21: tScAl=8'h0A;
		6'h22: tScAl=8'h0B;	6'h23: tScAl=8'h0B;
		6'h24: tScAl=8'h0B;	6'h25: tScAl=8'h0C;
		6'h26: tScAl=8'h0C;	6'h27: tScAl=8'h0C;
		6'h28: tScAl=8'h0D;	6'h29: tScAl=8'h0D;
		6'h2A: tScAl=8'h0D;	6'h2B: tScAl=8'h0E;
		6'h2C: tScAl=8'h0E;	6'h2D: tScAl=8'h0E;
		6'h2E: tScAl=8'h0F;	6'h2F: tScAl=8'h0F;
		6'h30: tScAl=8'h0F;	6'h31: tScAl=8'h10;
		6'h32: tScAl=8'h10;	6'h33: tScAl=8'h10;
		6'h34: tScAl=8'h11;	6'h35: tScAl=8'h11;
		6'h36: tScAl=8'h11;	6'h37: tScAl=8'h12;
		6'h38: tScAl=8'h12;	6'h39: tScAl=8'h12;
		6'h3A: tScAl=8'h13;	6'h3B: tScAl=8'h13;
		6'h3C: tScAl=8'h13;	6'h3D: tScAl=8'h14;
		6'h3E: tScAl=8'h14;	6'h3F: tScAl=8'h14;
	endcase

	case(valB[5:0])
		6'h00: tScBl=8'h00;	6'h01: tScBl=8'h00;
		6'h02: tScBl=8'h01;	6'h03: tScBl=8'h01;
		6'h04: tScBl=8'h02;	6'h05: tScBl=8'h03;
		6'h06: tScBl=8'h03;	6'h07: tScBl=8'h04;
		6'h08: tScBl=8'h05;	6'h09: tScBl=8'h05;
		6'h0A: tScBl=8'h06;	6'h0B: tScBl=8'h07;
		6'h0C: tScBl=8'h07;	6'h0D: tScBl=8'h08;
		6'h0E: tScBl=8'h09;	6'h0F: tScBl=8'h09;
		6'h10: tScBl=8'h0A;	6'h11: tScBl=8'h0B;
		6'h12: tScBl=8'h0B;	6'h13: tScBl=8'h0C;
		6'h14: tScBl=8'h0D;	6'h15: tScBl=8'h0D;
		6'h16: tScBl=8'h0E;	6'h17: tScBl=8'h0F;
		6'h18: tScBl=8'h0F;	6'h19: tScBl=8'h10;
		6'h1A: tScBl=8'h11;	6'h1B: tScBl=8'h11;
		6'h1C: tScBl=8'h12;	6'h1D: tScBl=8'h13;
		6'h1E: tScBl=8'h13;	6'h1F: tScBl=8'h14;
		6'h20: tScBl=8'h15;	6'h21: tScBl=8'h15;
		6'h22: tScBl=8'h16;	6'h23: tScBl=8'h17;
		6'h24: tScBl=8'h17;	6'h25: tScBl=8'h18;
		6'h26: tScBl=8'h19;	6'h27: tScBl=8'h19;
		6'h28: tScBl=8'h1A;	6'h29: tScBl=8'h1B;
		6'h2A: tScBl=8'h1B;	6'h2B: tScBl=8'h1C;
		6'h2C: tScBl=8'h1D;	6'h2D: tScBl=8'h1D;
		6'h2E: tScBl=8'h1E;	6'h2F: tScBl=8'h1F;
		6'h30: tScBl=8'h1F;	6'h31: tScBl=8'h20;
		6'h32: tScBl=8'h21;	6'h33: tScBl=8'h21;
		6'h34: tScBl=8'h22;	6'h35: tScBl=8'h23;
		6'h36: tScBl=8'h23;	6'h37: tScBl=8'h24;
		6'h38: tScBl=8'h25;	6'h39: tScBl=8'h25;
		6'h3A: tScBl=8'h26;	6'h3B: tScBl=8'h27;
		6'h3C: tScBl=8'h27;	6'h3D: tScBl=8'h28;
		6'h3E: tScBl=8'h29;	6'h3F: tScBl=8'h29;
	endcase

//	tScA = { tScAh, 4'h0 } + { 4'h0, tScAl };
//	tScB = { tScBh, 4'h0 } + { 4'h0, tScBl };

//	tScA = { tScAh[7:3], tScAl[7:0] };
//	tScB = { tScBh[7:3], tScBl[7:0] };

//	tScC = tScA + tScB;

	tScC = { tScAh + tScBh, 4'h0 } +
		{ 4'h0, tScAl + tScBl };
`endif

`ifndef def_true
// `ifdef def_true
	case(valA[11:7])
		5'h00: tScAh=12'h000;	5'h01: tScAh=12'h02A;
		5'h02: tScAh=12'h055;	5'h03: tScAh=12'h080;
		5'h04: tScAh=12'h0AA;	5'h05: tScAh=12'h0D5;
		5'h06: tScAh=12'h100;	5'h07: tScAh=12'h12A;
		5'h08: tScAh=12'h155;	5'h09: tScAh=12'h180;
		5'h0A: tScAh=12'h1AA;	5'h0B: tScAh=12'h1D5;
		5'h0C: tScAh=12'h200;	5'h0D: tScAh=12'h22A;
		5'h0E: tScAh=12'h255;	5'h0F: tScAh=12'h280;
		5'h10: tScAh=12'h2AB;	5'h11: tScAh=12'h2D5;
		5'h12: tScAh=12'h300;	5'h13: tScAh=12'h32B;
		5'h14: tScAh=12'h355;	5'h15: tScAh=12'h380;
		5'h16: tScAh=12'h3AB;	5'h17: tScAh=12'h3D5;
		5'h18: tScAh=12'h400;	5'h19: tScAh=12'h42B;
		5'h1A: tScAh=12'h455;	5'h1B: tScAh=12'h480;
		5'h1C: tScAh=12'h4AB;	5'h1D: tScAh=12'h4D5;
		5'h1E: tScAh=12'h500;	5'h1F: tScAh=12'h52B;
	endcase

	case(valB[11:7])
		5'h00: tScBh=12'h000;	5'h01: tScBh=12'h055;
		5'h02: tScBh=12'h0AA;	5'h03: tScBh=12'h100;
		5'h04: tScBh=12'h155;	5'h05: tScBh=12'h1AA;
		5'h06: tScBh=12'h200;	5'h07: tScBh=12'h255;
		5'h08: tScBh=12'h2AB;	5'h09: tScBh=12'h300;
		5'h0A: tScBh=12'h355;	5'h0B: tScBh=12'h3AB;
		5'h0C: tScBh=12'h400;	5'h0D: tScBh=12'h455;
		5'h0E: tScBh=12'h4AB;	5'h0F: tScBh=12'h500;
		5'h10: tScBh=12'h556;	5'h11: tScBh=12'h5AB;
		5'h12: tScBh=12'h600;	5'h13: tScBh=12'h656;
		5'h14: tScBh=12'h6AB;	5'h15: tScBh=12'h700;
		5'h16: tScBh=12'h756;	5'h17: tScBh=12'h7AB;
		5'h18: tScBh=12'h801;	5'h19: tScBh=12'h856;
		5'h1A: tScBh=12'h8AB;	5'h1B: tScBh=12'h901;
		5'h1C: tScBh=12'h956;	5'h1D: tScBh=12'h9AB;
		5'h1E: tScBh=12'hA01;	5'h1F: tScBh=12'hA56;
	endcase

	case(valA[6:2])
		5'h00: tScAl=8'h00;	5'h01: tScAl=8'h01;
		5'h02: tScAl=8'h02;	5'h03: tScAl=8'h03;
		5'h04: tScAl=8'h05;	5'h05: tScAl=8'h06;
		5'h06: tScAl=8'h07;	5'h07: tScAl=8'h09;
		5'h08: tScAl=8'h0A;	5'h09: tScAl=8'h0B;
		5'h0A: tScAl=8'h0D;	5'h0B: tScAl=8'h0E;
		5'h0C: tScAl=8'h0F;	5'h0D: tScAl=8'h11;
		5'h0E: tScAl=8'h12;	5'h0F: tScAl=8'h13;
		5'h10: tScAl=8'h15;	5'h11: tScAl=8'h16;
		5'h12: tScAl=8'h17;	5'h13: tScAl=8'h19;
		5'h14: tScAl=8'h1A;	5'h15: tScAl=8'h1B;
		5'h16: tScAl=8'h1D;	5'h17: tScAl=8'h1E;
		5'h18: tScAl=8'h1F;	5'h19: tScAl=8'h21;
		5'h1A: tScAl=8'h22;	5'h1B: tScAl=8'h23;
		5'h1C: tScAl=8'h25;	5'h1D: tScAl=8'h26;
		5'h1E: tScAl=8'h27;	5'h1F: tScAl=8'h29;
	endcase

	case(valB[6:2])
		5'h00: tScBl=8'h00;	5'h01: tScBl=8'h02;
		5'h02: tScBl=8'h05;	5'h03: tScBl=8'h07;
		5'h04: tScBl=8'h0A;	5'h05: tScBl=8'h0D;
		5'h06: tScBl=8'h0F;	5'h07: tScBl=8'h12;
		5'h08: tScBl=8'h15;	5'h09: tScBl=8'h17;
		5'h0A: tScBl=8'h1A;	5'h0B: tScBl=8'h1D;
		5'h0C: tScBl=8'h1F;	5'h0D: tScBl=8'h22;
		5'h0E: tScBl=8'h25;	5'h0F: tScBl=8'h27;
		5'h10: tScBl=8'h2A;	5'h11: tScBl=8'h2D;
		5'h12: tScBl=8'h2F;	5'h13: tScBl=8'h32;
		5'h14: tScBl=8'h35;	5'h15: tScBl=8'h37;
		5'h16: tScBl=8'h3A;	5'h17: tScBl=8'h3D;
		5'h18: tScBl=8'h3F;	5'h19: tScBl=8'h42;
		5'h1A: tScBl=8'h45;	5'h1B: tScBl=8'h47;
		5'h1C: tScBl=8'h4A;	5'h1D: tScBl=8'h4D;
		5'h1E: tScBl=8'h4F;	5'h1F: tScBl=8'h52;
	endcase

//	tScA = tScAh + { 4'h0, tScAl } + { 11'h0, valA[1] };
//	tScB = tScBh + { 4'h0, tScBl } + { 11'h0, valB[1] };
//	tScC = tScA + tScB;

	tScC = tScAh + tScBh + { 4'h0, tScAl + tScBl };

`endif


end

endmodule

`endif

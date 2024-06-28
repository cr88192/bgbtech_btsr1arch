module ExBnn_BitNeur_Mul2x(valV, valW, valO);
input[3:0]	valV;
input[5:0]	valW;
output[3:0]	valO;

reg[3:0]	tValO;
assign	valO = tValO;

reg[2:0]	tValOa;
reg[2:0]	tValOb;

always @*
begin
	casez({valV[1:0], valW[2:0]})
		5'b00_zzz: tValOa=3'b000;
		5'b10_zzz: tValOa=3'b000;
//		5'b01_000: tValOa=3'b000;
//		5'b1z_000: tValOa=3'b000;
//		5'b01_100: tValOa=3'b000;
//		5'b1z_100: tValOa=3'b000;

		5'b01_000: tValOa=3'b000;
		5'b01_001: tValOa=3'b001;
		5'b01_010: tValOa=3'b010;
		5'b01_011: tValOa=3'b011;
		5'b01_100: tValOa=3'b000;
		5'b01_101: tValOa=3'b111;
		5'b01_110: tValOa=3'b110;
		5'b01_111: tValOa=3'b101;

		5'b11_000: tValOa=3'b000;
		5'b11_100: tValOa=3'b000;
		5'b11_101: tValOa=3'b001;
		5'b11_110: tValOa=3'b010;
		5'b11_111: tValOa=3'b011;
		5'b11_001: tValOa=3'b111;
		5'b11_010: tValOa=3'b110;
		5'b11_011: tValOa=3'b101;
	endcase

	casez({valV[3:2], valW[5:3]})
		5'b00_zzz: tValOb=3'b000;
		5'b10_zzz: tValOb=3'b000;
//		5'b01_000: tValOb=3'b000;
//		5'b1z_000: tValOb=3'b000;
//		5'b01_100: tValOb=3'b000;
//		5'b1z_100: tValOb=3'b000;

		5'b01_000: tValOb=3'b000;
		5'b01_001: tValOb=3'b001;
		5'b01_010: tValOb=3'b010;
		5'b01_011: tValOb=3'b011;
		5'b01_100: tValOb=3'b000;
		5'b01_101: tValOb=3'b111;
		5'b01_110: tValOb=3'b110;
		5'b01_111: tValOb=3'b101;

		5'b11_000: tValOb=3'b000;
		5'b11_100: tValOb=3'b000;
		5'b11_101: tValOb=3'b001;
		5'b11_110: tValOb=3'b010;
		5'b11_111: tValOb=3'b011;
		5'b11_001: tValOb=3'b111;
		5'b11_010: tValOb=3'b110;
		5'b11_011: tValOb=3'b101;
	endcase
	
	case({tValOa, tValOb})
		6'b000_000: tValO=4'h0;
		6'b000_001: tValO=4'h1;
		6'b000_010: tValO=4'h2;
		6'b000_011: tValO=4'h3;
		6'b000_100: tValO=4'hC;
		6'b000_101: tValO=4'hD;
		6'b000_110: tValO=4'hE;
		6'b000_111: tValO=4'hF;
		6'b001_000: tValO=4'h1;
		6'b001_001: tValO=4'h2;
		6'b001_010: tValO=4'h3;
		6'b001_011: tValO=4'h4;
		6'b001_100: tValO=4'hD;
		6'b001_101: tValO=4'hE;
		6'b001_110: tValO=4'hF;
		6'b001_111: tValO=4'h0;

		6'b010_000: tValO=4'h2;
		6'b010_001: tValO=4'h3;
		6'b010_010: tValO=4'h4;
		6'b010_011: tValO=4'h5;
		6'b010_100: tValO=4'hE;
		6'b010_101: tValO=4'hF;
		6'b010_110: tValO=4'h0;
		6'b010_111: tValO=4'h1;
		6'b011_000: tValO=4'h3;
		6'b011_001: tValO=4'h4;
		6'b011_010: tValO=4'h5;
		6'b011_011: tValO=4'h6;
		6'b011_100: tValO=4'hF;
		6'b011_101: tValO=4'h0;
		6'b011_110: tValO=4'h1;
		6'b011_111: tValO=4'h2;

		6'b100_000: tValO=4'hC;
		6'b100_001: tValO=4'hD;
		6'b100_010: tValO=4'hE;
		6'b100_011: tValO=4'hF;
		6'b100_100: tValO=4'h8;
		6'b100_101: tValO=4'h9;
		6'b100_110: tValO=4'hA;
		6'b100_111: tValO=4'hB;
		6'b101_000: tValO=4'hD;
		6'b101_001: tValO=4'hE;
		6'b101_010: tValO=4'hF;
		6'b101_011: tValO=4'h0;
		6'b101_100: tValO=4'h9;
		6'b101_101: tValO=4'hA;
		6'b101_110: tValO=4'hB;
		6'b101_111: tValO=4'hC;

		6'b110_000: tValO=4'hE;
		6'b110_001: tValO=4'hF;
		6'b110_010: tValO=4'h0;
		6'b110_011: tValO=4'h1;
		6'b110_100: tValO=4'hA;
		6'b110_101: tValO=4'hB;
		6'b110_110: tValO=4'hC;
		6'b110_111: tValO=4'hD;
		6'b111_000: tValO=4'hF;
		6'b111_001: tValO=4'h0;
		6'b111_010: tValO=4'h1;
		6'b111_011: tValO=4'h2;
		6'b111_100: tValO=4'hB;
		6'b111_101: tValO=4'hC;
		6'b111_110: tValO=4'hD;
		6'b111_111: tValO=4'hE;
	endcase
end

endmodule

module ExBnn_BitNeur_Mul2x(valV, valW, valO);
input[3:0]	valV;
input[5:0]	valW;
output[4:0]	valO;

reg[4:0]	tValO;
assign	valO = tValO;

reg[2:0]	tValOa;
reg[2:0]	tValOb;

always @*
begin
	casez({valV[1:0], valW[2:0]})
		5'b00_zzz: tValOa=3'b000;
		5'b10_zzz: tValOa=3'b000;

		5'b01_000: tValOa=3'b000;
		5'b01_001: tValOa=3'b001;
		5'b01_010: tValOa=3'b010;
		5'b01_011: tValOa=3'b011;
		5'b01_100: tValOa=3'b000;
		5'b01_101: tValOa=3'b111;
		5'b01_110: tValOa=3'b110;
		5'b01_111: tValOa=3'b101;

		5'b11_000: tValOa=3'b000;
		5'b11_001: tValOa=3'b111;
		5'b11_010: tValOa=3'b110;
		5'b11_011: tValOa=3'b101;
		5'b11_100: tValOa=3'b000;
		5'b11_101: tValOa=3'b001;
		5'b11_110: tValOa=3'b010;
		5'b11_111: tValOa=3'b011;
	endcase

	casez({valV[3:2], valW[5:3]})
		5'b00_zzz: tValOb=3'b000;
		5'b10_zzz: tValOb=3'b000;

		5'b01_000: tValOb=3'b000;
		5'b01_001: tValOb=3'b001;
		5'b01_010: tValOb=3'b010;
		5'b01_011: tValOb=3'b011;
		5'b01_100: tValOb=3'b000;
		5'b01_101: tValOb=3'b111;
		5'b01_110: tValOb=3'b110;
		5'b01_111: tValOb=3'b101;

		5'b11_000: tValOb=3'b000;
		5'b11_001: tValOb=3'b111;
		5'b11_010: tValOb=3'b110;
		5'b11_011: tValOb=3'b101;
		5'b11_100: tValOb=3'b000;
		5'b11_101: tValOb=3'b001;
		5'b11_110: tValOb=3'b010;
		5'b11_111: tValOb=3'b011;
	endcase
	
	case({tValOa, tValOb})
`ifndef def_true
		6'b000_000: tValO=5'h00;
		6'b000_001: tValO=5'h01;
		6'b000_010: tValO=5'h02;
		6'b000_011: tValO=5'h03;
		6'b000_100: tValO=5'h1C;
		6'b000_101: tValO=5'h1D;
		6'b000_110: tValO=5'h1E;
		6'b000_111: tValO=5'h1F;
		6'b001_000: tValO=5'h01;
		6'b001_001: tValO=5'h02;
		6'b001_010: tValO=5'h03;
		6'b001_011: tValO=5'h04;
		6'b001_100: tValO=5'h1D;
		6'b001_101: tValO=5'h1E;
		6'b001_110: tValO=5'h1F;
		6'b001_111: tValO=5'h00;

		6'b010_000: tValO=5'h02;
		6'b010_001: tValO=5'h03;
		6'b010_010: tValO=5'h04;
		6'b010_011: tValO=5'h05;
		6'b010_100: tValO=5'h1E;
		6'b010_101: tValO=5'h1F;
		6'b010_110: tValO=5'h00;
		6'b010_111: tValO=5'h01;
		6'b011_000: tValO=5'h03;
		6'b011_001: tValO=5'h04;
		6'b011_010: tValO=5'h05;
		6'b011_011: tValO=5'h06;
		6'b011_100: tValO=5'h1F;
		6'b011_101: tValO=5'h00;
		6'b011_110: tValO=5'h01;
		6'b011_111: tValO=5'h02;

		6'b100_000: tValO=5'h1C;
		6'b100_001: tValO=5'h1D;
		6'b100_010: tValO=5'h1E;
		6'b100_011: tValO=5'h1F;
		6'b100_100: tValO=5'h18;
		6'b100_101: tValO=5'h19;
		6'b100_110: tValO=5'h1A;
		6'b100_111: tValO=5'h1B;
		6'b101_000: tValO=5'h1D;
		6'b101_001: tValO=5'h1E;
		6'b101_010: tValO=5'h1F;
		6'b101_011: tValO=5'h00;
		6'b101_100: tValO=5'h19;
		6'b101_101: tValO=5'h1A;
		6'b101_110: tValO=5'h1B;
		6'b101_111: tValO=5'h1C;

		6'b110_000: tValO=5'h1E;
		6'b110_001: tValO=5'h1F;
		6'b110_010: tValO=5'h00;
		6'b110_011: tValO=5'h01;
		6'b110_100: tValO=5'h1A;
		6'b110_101: tValO=5'h1B;
		6'b110_110: tValO=5'h1C;
		6'b110_111: tValO=5'h1D;
		6'b111_000: tValO=5'h1F;
		6'b111_001: tValO=5'h00;
		6'b111_010: tValO=5'h01;
		6'b111_011: tValO=5'h02;
		6'b111_100: tValO=5'h1B;
		6'b111_101: tValO=5'h1C;
		6'b111_110: tValO=5'h1D;
		6'b111_111: tValO=5'h1E;
`endif

`ifdef def_true
		6'b000_000: tValO=5'h00;	// 0+0 =  0
		6'b000_001: tValO=5'h01;	// 0+1 =  1
		6'b000_010: tValO=5'h03;	// 0+3 =  3
		6'b000_011: tValO=5'h07;	// 0+7 =  7
		6'b000_100: tValO=5'h18;	// 0-8 = -8
		6'b000_101: tValO=5'h19;	// 0-7 = -7
		6'b000_110: tValO=5'h1D;	// 0-3 = -3
		6'b000_111: tValO=5'h1F;	// 0-1 = -1

		6'b001_000: tValO=5'h01;	// 1+0 =  1
		6'b001_001: tValO=5'h02;	// 1+1 =  2
		6'b001_010: tValO=5'h04;	// 1+3 =  4
		6'b001_011: tValO=5'h08;	// 1+7 =  8
		6'b001_100: tValO=5'h19;	// 1-8 = -7
		6'b001_101: tValO=5'h1A;	// 1-7 = -6
		6'b001_110: tValO=5'h1E;	// 1-3 = -2
		6'b001_111: tValO=5'h00;	// 1-1 =  0

		6'b010_000: tValO=5'h03;	// 3+0 =  3
		6'b010_001: tValO=5'h04;	// 3+1 =  4
		6'b010_010: tValO=5'h06;	// 3+3 =  6
		6'b010_011: tValO=5'h0A;	// 3+7 = 10
		6'b010_100: tValO=5'h1B;    // 3-8 = -5
		6'b010_101: tValO=5'h1C;	// 3-7 = -4
		6'b010_110: tValO=5'h00;	// 3-3 =  0
		6'b010_111: tValO=5'h02;	// 3-1 =  2

		6'b011_000: tValO=5'h07;	// 7+0 =  7
		6'b011_001: tValO=5'h08;	// 7+1 =  8
		6'b011_010: tValO=5'h0A;	// 7+3 = 10
		6'b011_011: tValO=5'h0E;	// 7+7 = 14
		6'b011_100: tValO=5'h1F;	// 7-8 = -1
		6'b011_101: tValO=5'h00;	// 7-7 =  0
		6'b011_110: tValO=5'h04;	// 7-3 =  4
		6'b011_111: tValO=5'h06;	// 7-1 =  6

		6'b100_000: tValO=5'h18;	// -8+0 = - 8
		6'b100_001: tValO=5'h19;	// -8+1 = - 7
		6'b100_010: tValO=5'h1B;	// -8+3 = - 5
		6'b100_011: tValO=5'h1F;	// -8+7 = - 1
		6'b100_100: tValO=5'h10;	// -8-8 = -16
		6'b100_101: tValO=5'h11;	// -8-7 = -15
		6'b100_110: tValO=5'h15;	// -8-3 = -11
		6'b100_111: tValO=5'h17;	// -8-1 = - 9

		6'b101_000: tValO=5'h19;	// -7+0 = - 7
		6'b101_001: tValO=5'h1A;	// -7+1 = - 6
		6'b101_010: tValO=5'h1C;	// -7+3 = - 4
		6'b101_011: tValO=5'h00;	// -7+7 =   0
		6'b101_100: tValO=5'h11;	// -7-8 = -15
		6'b101_101: tValO=5'h12;	// -7-7 = -14
		6'b101_110: tValO=5'h16;	// -7-3 = -10
		6'b101_111: tValO=5'h18;	// -7-1 = - 8

		6'b110_000: tValO=5'h1D;	// -3+0 = - 3
		6'b110_001: tValO=5'h1E;	// -3+1 = - 2
		6'b110_010: tValO=5'h00;	// -3+3 =   0
		6'b110_011: tValO=5'h04;	// -3+7 =   4
		6'b110_100: tValO=5'h15;	// -3-8 = -11
		6'b110_101: tValO=5'h16;	// -3-7 = -10
		6'b110_110: tValO=5'h1A;	// -3-3 = - 6
		6'b110_111: tValO=5'h1C;	// -3-1 = - 4

		6'b111_000: tValO=5'h1F;	// -1+0 = -1
		6'b111_001: tValO=5'h00;	// -1+1 =  0
		6'b111_010: tValO=5'h02;	// -1+3 =  2
		6'b111_011: tValO=5'h06;	// -1+7 =  6
		6'b111_100: tValO=5'h17;	// -1-8 = -9
		6'b111_101: tValO=5'h18;	// -1-7 = -8
		6'b111_110: tValO=5'h1C;	// -1-3 = -4
		6'b111_111: tValO=5'h1E;	// -1-1 = -2
`endif
	endcase
end

endmodule

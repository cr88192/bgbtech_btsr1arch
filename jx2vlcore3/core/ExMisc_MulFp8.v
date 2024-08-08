`ifndef HAS_ExMisc_MulFp8
`define HAS_ExMisc_MulFp8

module ExMisc_MulFp8(valA, valB, valRes);

input[7:0] valA;
input[7:0] valB;
output[15:0] valRes;


reg[15:0]	tValC;
assign	valRes = tValC;


reg			tSgnA;
reg			tSgnB;
reg			tSgnC;

reg[4:0]	tExpA;
reg[4:0]	tExpB;
reg[4:0]	tExpC0;
reg[4:0]	tExpC1;
reg[4:0]	tExpC;

reg[2:0]	tFraA;
reg[2:0]	tFraB;
reg			tZeroA;
reg			tZeroB;
reg			tZeroC;

reg[7:0]	tFraC0;
reg[10:0]	tFraC;


always @*
begin
	tSgnA=valA[7];
	tSgnB=valB[7];
	tExpA={ valA[6], !valA[6], valA[5:3] };
	tExpB={ valB[6], !valB[6], valB[5:3] };
	tFraA=valA[2:0];
	tFraB=valB[2:0];
	tZeroA=(valA[6:0]==7'h00);
	tZeroB=(valB[6:0]==7'h00);
	tSgnC=tSgnA^tSgnB;
	tExpC0=tExpA+tExpB+(0-15);
	tExpC1=tExpA+tExpB+(1-15);
	tZeroC=tZeroA|tZeroB;

	case({tFraA, tFraB})
		6'b000_000: tFraC0=8'h40;
		6'b000_001: tFraC0=8'h48;
		6'b000_010: tFraC0=8'h50;
		6'b000_011: tFraC0=8'h58;
		6'b000_100: tFraC0=8'h60;
		6'b000_101: tFraC0=8'h68;
		6'b000_110: tFraC0=8'h70;
		6'b000_111: tFraC0=8'h78;

		6'b001_000: tFraC0=8'h48;
		6'b001_001: tFraC0=8'h51;
		6'b001_010: tFraC0=8'h5A;
		6'b001_011: tFraC0=8'h63;
		6'b001_100: tFraC0=8'h6C;
		6'b001_101: tFraC0=8'h75;
		6'b001_110: tFraC0=8'h7E;
		6'b001_111: tFraC0=8'h87;

		6'b010_000: tFraC0=8'h50;
		6'b010_001: tFraC0=8'h5A;
		6'b010_010: tFraC0=8'h64;
		6'b010_011: tFraC0=8'h6E;
		6'b010_100: tFraC0=8'h78;
		6'b010_101: tFraC0=8'h82;
		6'b010_110: tFraC0=8'h8C;
		6'b010_111: tFraC0=8'h96;

		6'b011_000: tFraC0=8'h58;
		6'b011_001: tFraC0=8'h63;
		6'b011_010: tFraC0=8'h6E;
		6'b011_011: tFraC0=8'h79;
		6'b011_100: tFraC0=8'h84;
		6'b011_101: tFraC0=8'h8F;
		6'b011_110: tFraC0=8'h9A;
		6'b011_111: tFraC0=8'hA5;

		6'b100_000: tFraC0=8'h60;
		6'b100_001: tFraC0=8'h6C;
		6'b100_010: tFraC0=8'h78;
		6'b100_011: tFraC0=8'h84;
		6'b100_100: tFraC0=8'h90;
		6'b100_101: tFraC0=8'h9C;
		6'b100_110: tFraC0=8'hA8;
		6'b100_111: tFraC0=8'hB4;

		6'b101_000: tFraC0=8'h68;
		6'b101_001: tFraC0=8'h75;
		6'b101_010: tFraC0=8'h82;
		6'b101_011: tFraC0=8'h8F;
		6'b101_100: tFraC0=8'h9C;
		6'b101_101: tFraC0=8'hA9;
		6'b101_110: tFraC0=8'hB6;
		6'b101_111: tFraC0=8'hC3;

		6'b110_000: tFraC0=8'h70;
		6'b110_001: tFraC0=8'h7E;
		6'b110_010: tFraC0=8'h8C;
		6'b110_011: tFraC0=8'h9A;
		6'b110_100: tFraC0=8'hA8;
		6'b110_101: tFraC0=8'hB6;
		6'b110_110: tFraC0=8'hC4;
		6'b110_111: tFraC0=8'hD2;

		6'b111_000: tFraC0=8'h78;
		6'b111_001: tFraC0=8'h87;
		6'b111_010: tFraC0=8'h96;
		6'b111_011: tFraC0=8'hA5;
		6'b111_100: tFraC0=8'hB4;
		6'b111_101: tFraC0=8'hC3;
		6'b111_110: tFraC0=8'hD2;
		6'b111_111: tFraC0=8'hE1;
	endcase

	if(tFraC0[7])
	begin
		tExpC=tExpC1;
		tFraC={tFraC0[7:0], 3'h0};
	end
	else
	begin
		tExpC=tExpC0;
		tFraC={tFraC0[6:0], 4'h0};
	end

	tValC={tSgnC, tExpC, tFraC[9:0]};

	if(tZeroC)
		tValC=16'h0000; 
end


endmodule

`endif

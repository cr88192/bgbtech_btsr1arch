`ifndef has_ModFmScVol6
`define has_ModFmScVol6

module ModFmScVol6(
	clock,	reset,
	valI,	valO,
	scale
	);

input			clock;
input			reset;
input[11:0]		valI;
output[11:0]	valO;
input[5:0]		scale;

reg[11:0]		scVal;

reg[23:0]	tValSc;
reg[11:0]	tValO;

assign		valO = tValO;

always @*
begin
	case(scale)
		6'b000_000: scVal = 12'h800;
		6'b000_001: scVal = 12'h780;
		6'b000_010: scVal = 12'h700;
		6'b000_011: scVal = 12'h680;
		6'b000_100: scVal = 12'h600;
		6'b000_101: scVal = 12'h580;
		6'b000_110: scVal = 12'h500;
		6'b000_111: scVal = 12'h480;

		6'b001_000: scVal = 12'h400;
		6'b001_001: scVal = 12'h3C0;
		6'b001_010: scVal = 12'h380;
		6'b001_011: scVal = 12'h340;
		6'b001_100: scVal = 12'h300;
		6'b001_101: scVal = 12'h2C0;
		6'b001_110: scVal = 12'h280;
		6'b001_111: scVal = 12'h240;

		6'b010_000: scVal = 12'h200;
		6'b010_001: scVal = 12'h1E0;
		6'b010_010: scVal = 12'h1C0;
		6'b010_011: scVal = 12'h1A0;
		6'b010_100: scVal = 12'h180;
		6'b010_101: scVal = 12'h160;
		6'b010_110: scVal = 12'h140;
		6'b010_111: scVal = 12'h120;

		6'b011_000: scVal = 12'h100;
		6'b011_001: scVal = 12'h0F0;
		6'b011_010: scVal = 12'h0E0;
		6'b011_011: scVal = 12'h0D0;
		6'b011_100: scVal = 12'h0C0;
		6'b011_101: scVal = 12'h0B0;
		6'b011_110: scVal = 12'h0A0;
		6'b011_111: scVal = 12'h090;

		6'b100_000: scVal = 12'h080;
		6'b100_001: scVal = 12'h078;
		6'b100_010: scVal = 12'h070;
		6'b100_011: scVal = 12'h068;
		6'b100_100: scVal = 12'h060;
		6'b100_101: scVal = 12'h058;
		6'b100_110: scVal = 12'h050;
		6'b100_111: scVal = 12'h048;

		6'b101_000: scVal = 12'h040;
		6'b101_001: scVal = 12'h03C;
		6'b101_010: scVal = 12'h038;
		6'b101_011: scVal = 12'h034;
		6'b101_100: scVal = 12'h030;
		6'b101_101: scVal = 12'h02C;
		6'b101_110: scVal = 12'h028;
		6'b101_111: scVal = 12'h024;

		6'b110_000: scVal = 12'h020;
		6'b110_001: scVal = 12'h01E;
		6'b110_010: scVal = 12'h01C;
		6'b110_011: scVal = 12'h01A;
		6'b110_100: scVal = 12'h018;
		6'b110_101: scVal = 12'h016;
		6'b110_110: scVal = 12'h014;
		6'b110_111: scVal = 12'h012;

		6'b111_000: scVal = 12'h010;
		6'b111_001: scVal = 12'h00F;
		6'b111_010: scVal = 12'h00E;
		6'b111_011: scVal = 12'h00D;
		6'b111_100: scVal = 12'h00C;
		6'b111_101: scVal = 12'h00B;
		6'b111_110: scVal = 12'h00A;
		6'b111_111: scVal = 12'h000;
	endcase
	
	tValSc =
		{ valI[11] ? 12'hFFF : 12'h000, valI[11:0] } *
		{ 12'h000, scVal };
	
	tValO = tValSc[23:12];
end

endmodule

`endif

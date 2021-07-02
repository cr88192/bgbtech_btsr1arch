module ExBtcUabUf8(
	clock,	reset,
	valI,	valO);

input			clock;
input			reset;
input [ 7:0]	valI;
output[11:0]	valO;

reg[11:0]		tValO;
assign		valO = tValO;

reg[11:0]		tValA;

always @*
begin
	case(valI[6:4])
		3'b000: tValA = { 8'h00, valI[3:0]       };
		3'b001: tValA = { 8'h01, valI[3:0]       };
		3'b010: tValA = { 7'h01, valI[3:0], 1'b0 };
		3'b011: tValA = { 6'h01, valI[3:0], 2'b0 };
		3'b100: tValA = { 5'h01, valI[3:0], 3'b0 };
		3'b101: tValA = { 4'h01, valI[3:0], 4'b0 };
		3'b110: tValA = { 3'h01, valI[3:0], 5'b0 };
		3'b111: tValA = { 2'h01, valI[3:0], 6'b0 };
	endcase
	tValO = valI[7] ? (~tValA) : tValA;
end

endmodule

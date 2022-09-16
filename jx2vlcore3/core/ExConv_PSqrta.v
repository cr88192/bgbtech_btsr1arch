/* verilator lint_off UNUSED */

module ExConv_PSqrta(valIn, valOut, opMod);
input[63:0]		valIn;
output[63:0]	valOut;
input[3:0]		opMod;

reg[63:0]	tValOut;
assign	valOut = tValOut;

reg[15:0]	tValIn0h;
reg[15:0]	tValIn1h;
reg[15:0]	tValIn2h;
reg[15:0]	tValIn3h;

reg[15:0]	tValIn0l;
reg[15:0]	tValIn2l;
reg[31:0]	tValIn0f;
reg[31:0]	tValIn1f;

reg[15:0]	tSqrta0h;
reg[15:0]	tSqrta1h;
reg[15:0]	tSqrta2h;
reg[15:0]	tSqrta3h;
reg[31:0]	tSqrta0f;
reg[31:0]	tSqrta1f;

reg[15:0]	tRcp0h;
reg[15:0]	tRcp1h;
reg[15:0]	tRcp2h;
reg[15:0]	tRcp3h;
reg[31:0]	tRcp0f;
reg[31:0]	tRcp1f;

always @*
begin
	tValOut = 0;
	
	tValIn0h = (opMod[1] && valIn[15]) ? 16'h0000 : valIn[15: 0];
	tValIn1h = (opMod[1] && valIn[31]) ? 16'h0000 : valIn[31:16];
	tValIn2h = (opMod[1] && valIn[47]) ? 16'h0000 : valIn[47:32];
	tValIn3h = (opMod[1] && valIn[63]) ? 16'h0000 : valIn[63:48];

	tValIn0l = (opMod[1] && valIn[31]) ? 16'h0000 : valIn[15: 0];
	tValIn2l = (opMod[1] && valIn[63]) ? 16'h0000 : valIn[47:32];
	tValIn0f = { tValIn1h, tValIn0l };
	tValIn1f = { tValIn3h, tValIn2l };
	
	tRcp0h = { ~valIn[15: 8] + 8'h78, ~valIn[ 7: 0] };
	tRcp1h = { ~valIn[31:24] + 8'h78, ~valIn[23:16] };
	tRcp2h = { ~valIn[47:40] + 8'h78, ~valIn[39:32] };
	tRcp3h = { ~valIn[63:56] + 8'h78, ~valIn[55:48] };

	tRcp0f = { ~valIn[31:24] + 8'h7F, ~valIn[23: 0] };
	tRcp1f = { ~valIn[63:56] + 8'h7F, ~valIn[55:32] };
	
	tSqrta0h =
		{ tValIn0h[15], 1'b0, tValIn0h[14:1] } +
		((tValIn0h[14:10]!=0) ? 16'h1E00 : 16'h0000);
	tSqrta1h =
		{ tValIn1h[15], 1'b0, tValIn1h[14:1] } +
		((tValIn1h[14:10]!=0) ? 16'h1E00 : 16'h0000);
	tSqrta2h =
		{ tValIn2h[15], 1'b0, tValIn2h[14:1] } +
		((tValIn2h[14:10]!=0) ? 16'h1E00 : 16'h0000);
	tSqrta3h =
		{ tValIn3h[15], 1'b0, tValIn3h[14:1] } +
		((tValIn3h[14:10]!=0) ? 16'h1E00 : 16'h0000);

	tSqrta0f = {
		{ tValIn1h[15], 1'b0, tValIn1h[14:1] } +
			((tValIn1h[14:10]!=0) ? 16'h1FC0 : 16'h0000),
		tValIn0f[16:1] };
	tSqrta1f = {
		{ tValIn3h[15], 1'b0, tValIn3h[14:1] } +
			((tValIn3h[14:10]!=0) ? 16'h1FC0 : 16'h0000),
		tValIn1f[16:1] };

	casez(opMod[2:0])
		3'b00z: tValOut = { tSqrta3h, tSqrta2h, tSqrta1h, tSqrta0h };
		3'b01z: tValOut = { tSqrta1f, tSqrta0f };

		3'b100: tValOut = { tRcp3h, tRcp2h, tRcp1h, tRcp0h };
		3'b101: tValOut = { tValIn3h, tValIn2h, tValIn1h, tValIn0h };
		3'b110: tValOut = { tRcp1f, tRcp0f };
		3'b111: tValOut = { tValIn1f, tValIn0f };
	endcase

end

endmodule

module ExModRgba_Avg555(
	valRs,	valRt,	valRn
	);

input[15:0]		valRs;
input[15:0]		valRt;
output[15:0]	valRn;

reg[15:0]		tValRn;

assign	valRn = tValRn;

reg[15:0]		tValRn00;

always @*
begin
	
	tValRn00 = { 1'b0,
		{ 1'b0, valRs[14:11] } + { 1'b0, valRt[14:11] },
		{ 1'b0, valRs[ 9: 6] } + { 1'b0, valRt[ 9: 6] },
		{ 1'b0, valRs[ 4: 1] } + { 1'b0, valRt[ 4: 1] }
		};
	
	case( { valRs[15], valRt[15] } )
		2'b00: tValRn = tValRn00;
	endcase
	
end

endmodule

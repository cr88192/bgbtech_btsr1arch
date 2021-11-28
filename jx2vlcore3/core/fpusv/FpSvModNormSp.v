module FpSvModNormSp(
	fraS,	exS,
	fraN,	exN
	);

input[27:0]		fraS;
input[8 :0]		exS;
output[27:0]	fraN;
output[8 :0]	exN;

reg[27:0]		tFraN;
reg[8:0]		tExN;
assign		fraN = tFraN;
assign		exN = tExN;

reg[7:0]	tZeroMask;

always @*
begin
	
	tZeroMask[6] = (fraS[27:24] != 0);
	tZeroMask[5] = (fraS[23:20] != 0);
	tZeroMask[4] = (fraS[19:16] != 0);
	tZeroMask[3] = (fraS[15:12] != 0);
	tZeroMask[2] = (fraS[11: 8] != 0);
	tZeroMask[1] = (fraS[ 7: 4] != 0);
	tZeroMask[0] = (fraS[ 3: 0] != 0);

	if(tFraN[26])
	begin
		tExN	= exS;
		tFraN	= { 1'b0, fraS[27:1] };
	end
	else
	begin
	end
end

endmodule

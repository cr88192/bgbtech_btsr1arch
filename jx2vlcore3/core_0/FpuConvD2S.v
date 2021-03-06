module FpuConvD2S(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[31:0]	regValFRn;

reg[31:0]		tRegValFRn;
reg[31:0]		tRegValFRn1;
reg[4:0]		tRound;

assign	regValFRn = tRegValFRn;

always @*
begin
	tRegValFRn1	= {
		regValFRm[63:62],
		regValFRm[58:52],
//		regValFRm[51:29]+(regValFRm[28]?23'h1:23'h0) };
		regValFRm[51:29] };
		
	tRound = { 1'b0, regValFRm[32:29] } + 1;
	if(regValFRm[28] && !tRound[4])
		tRegValFRn1[3:0]=tRound[3:0];

	casez(regValFRm[62:59])
	4'b0111:	tRegValFRn = tRegValFRn1;
	4'b1000:	tRegValFRn = tRegValFRn1;
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 31'h7F800000 };
		else
			tRegValFRn = 0;
	endcase
end

endmodule
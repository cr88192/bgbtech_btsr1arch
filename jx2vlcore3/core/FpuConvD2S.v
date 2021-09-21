`ifndef HAS_FPUCONVD2S
`define HAS_FPUCONVD2S

module FpuConvD2S(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[31:0]	regValFRn;

reg[31:0]		tRegValFRn;
reg[31:0]		tRegValFRn1;
// reg[4:0]		tRound;
reg[8:0]		tRound;

assign	regValFRn = tRegValFRn;

always @*
begin
	tRegValFRn1	= {
		regValFRm[63:62],
		regValFRm[58:52],
//		regValFRm[51:29]+(regValFRm[28]?23'h1:23'h0) };
		regValFRm[51:29] };
		
//	tRound = { 1'b0, regValFRm[32:29] } + 1;
//	if(regValFRm[28] && !tRound[4])
//		tRegValFRn1[3:0]=tRound[3:0];

//	tRound				= { 1'b0, regValFRm[32:29] } + { 4'b0, regValFRm[28] };
//	tRegValFRn1[3:0]	= tRound[4] ? regValFRm[32:29] : tRound[3:0];

	tRound				= { 1'b0, regValFRm[36:29] } + { 8'b0, regValFRm[28] };
	tRegValFRn1[7:0]	= tRound[8] ? regValFRm[36:29] : tRound[7:0];

`ifndef def_true
	casez(regValFRm[62:59])
	4'b0111:	tRegValFRn = tRegValFRn1;
	4'b1000:	tRegValFRn = tRegValFRn1;
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 31'h7F800000 };
		else
			tRegValFRn = 0;
	endcase
`endif

`ifdef def_true
	casez(regValFRm[62:58])
	5'b0111z:	tRegValFRn = tRegValFRn1;
	5'b1000z:	tRegValFRn = tRegValFRn1;

	5'b11111:
	begin
		if(regValFRm[57:52]==6'h3F)
			tRegValFRn	= { regValFRm[63], 8'hFF, regValFRm[51:29] };
		else
			tRegValFRn	= { regValFRm[63], 31'h7F800000 };
	end

	5'b00000:
	begin
		if(regValFRm[57:52]==6'h00)
			tRegValFRn	= { regValFRm[63], 8'h00, regValFRm[51:29] };
		else
			tRegValFRn	= 0;
	end

	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 31'h7F800000 };
		else
			tRegValFRn = 0;
	endcase
`endif
end

endmodule

`endif

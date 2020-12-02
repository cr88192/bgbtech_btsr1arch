module RegFPR(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRo,	regValRo,

	regSrVal,	regExHold
	);

input clock;
input reset;

input[5:0]		regIdRm;
input[5:0]		regIdRn;

output[63:0]	regValRm;
output[63:0]	regValRn;

input[5:0]		regIdRo;
input[63:0]		regValRo;

input[63:0]		regSrVal;
input			regExHold;

reg			regSrRB;
reg			nxtRegSrRB;

reg[63:0]	tRegValRm;
reg[63:0]	tRegValRn;

assign regValRm = tRegValRm;
assign regValRn = tRegValRn;

reg[63:0]	regFprArrRm[15:0];
reg[63:0]	regFprArrRn[15:0];

always @*
begin
	tRegValRm=UV64_XX;
	tRegValRn=UV64_XX;

	tRegValRm = regFprArrRm[regIdRm[3:0]];
	tRegValRn = regFprArrRn[regIdRn[3:0]];
	
	if(regIdRm==JX2_GR_ZZR)
		tRegValRm = 0;
	if(regIdRn==JX2_GR_ZZR)
		tRegValRn = 0;

	if(regIdRm == regIdRo)
		tRegValRm = regValRo;
	if(regIdRn == regIdRo)
		tRegValRn = regValRo;
end

always @ (posedge clock)
begin
	if(!regExHold)
	begin
		if(regIdRo[5:4]==0)
		begin
			regFprArrRm[regIdRo[3:0]]	<= regValRo;
			regFprArrRn[regIdRo[3:0]]	<= regValRo;
		end
	end
end

endmodule

module RegSpr_3W(
	clock,	reset,
	regId,	regVal,
	wrIdA,	wrValA,
	wrIdB,	wrValB,
	wrIdC,	wrValC,
	regInVal, regHold
	);

input			clock;
input			reset;
input[5:0]		regId;
output[63:0]	regVal;
input[5:0]		wrIdA;
input[63:0]		wrValA;
input[5:0]		wrIdB;
input[63:0]		wrValB;
input[5:0]		wrIdC;
input[63:0]		wrValC;
input[63:0]		regInVal;
input			regHold;

reg[63:0]		tRegVal;
reg[63:0]		tNxtRegVal;

assign 		regVal = tRegVal;

reg				tIsIdA;
reg				tIsIdB;
reg				tIsIdC;

always @*
begin
`ifndef def_true
	tIsIdA	= (wrIdA == regId);
	tIsIdB	= (wrIdB == regId);
	tIsIdC	= (wrIdC == regId);
	
	casez( {tIsIdC, tIsIdB, tIsIdA} )
		3'b000:		tNxtRegVal	= regInVal;
//		3'b001:		tNxtRegVal	= wrValA;
//		3'b01z:		tNxtRegVal	= wrValB;
//		3'b1zz:		tNxtRegVal	= wrValC;
		3'bzz1:		tNxtRegVal	= wrValA;
		3'bz10:		tNxtRegVal	= wrValB;
		3'b100:		tNxtRegVal	= wrValC;
	endcase
`endif

`ifdef def_true
	tNxtRegVal	= regInVal;
	if(wrIdC == regId)
		tNxtRegVal	= wrValC;
	if(wrIdB == regId)
		tNxtRegVal	= wrValB;
	if(wrIdA == regId)
		tNxtRegVal	= wrValA;
`endif
end

always @(posedge clock)
begin
	if(!regHold)
	begin
		tRegVal		<= tNxtRegVal;
	end
end

endmodule
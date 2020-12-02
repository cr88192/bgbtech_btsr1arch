module RegSpr_2W(
	clock,	reset,
	regId,	regOutVal,
	wrIdA,	wrValA,
	wrIdB,	wrValB,
	regInVal, regHold
	);

input			clock;
input			reset;
input[5:0]		regId;
output[63:0]	regOutVal;
input[5:0]		wrIdA;
input[63:0]		wrValA;
input[5:0]		wrIdB;
input[63:0]		wrValB;
input[63:0]		regInVal;
input			regHold;

reg[63:0]		tRegVal;
reg[63:0]		tNxtRegVal;

assign 		regOutVal = tRegVal;

reg				tIsIdA;
reg				tIsIdB;

always @*
begin
	tIsIdA	= (wrIdA == regId);
	tIsIdB	= (wrIdB == regId);
	
	casez( {tIsIdB, tIsIdA} )
		2'b00:		tNxtRegVal	= regInVal;
//		2'b01:		tNxtRegVal	= wrValA;
//		2'b1z:		tNxtRegVal	= wrValB;
		2'bz1:		tNxtRegVal	= wrValA;
		2'b10:		tNxtRegVal	= wrValB;
	endcase
end

always @(posedge clock)
begin
	if(!regHold)
	begin
		tRegVal		<= tNxtRegVal;
	end
end

endmodule
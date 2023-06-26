module RegSpr_2W(
	clock,	reset,
	regId,	regVal,
	wrIdA,	wrValA,
	wrIdB,	wrValB,
	regInVal, regHold,
	regFlush
	);

input			clock;
input			reset;
`input_gpr		regId;
output[63:0]	regVal;
`input_gpr		wrIdA;
input[63:0]		wrValA;
`input_gpr		wrIdB;
input[63:0]		wrValB;
input[63:0]		regInVal;
input			regHold;
input			regFlush;

(* max_fanout = 200 *)
	reg[63:0]		tRegVal;
(* max_fanout = 200 *)
	reg[63:0]		tNxtRegVal;

assign 		regVal = tRegVal;

reg				tIsIdA;
reg				tIsIdB;

always @*
begin
	tNxtRegVal	= regInVal;
	if(!regFlush)
	begin
		if(wrIdB == regId)
			tNxtRegVal	= wrValB;
		if(wrIdA == regId)
			tNxtRegVal	= wrValA;
	end
end

always @(posedge clock)
begin
	if(!regHold)
	begin
		tRegVal		<= tNxtRegVal;
	end
end

endmodule

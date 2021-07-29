module RegSpr_6W(
	clock,	reset,
	regId,	regVal,
	wrIdA,	wrValA,
	wrIdB,	wrValB,
	wrIdC,	wrValC,
	wrIdD,	wrValD,
	wrIdE,	wrValE,
	wrIdF,	wrValF,
	regInVal,
	regHold1,	regHold2,
	regFlush1,	regFlush2
	);

input			clock;
input			reset;
`input_gpr		regId;
output[63:0]	regVal;

`input_gpr		wrIdA;
input[63:0]		wrValA;
`input_gpr		wrIdB;
input[63:0]		wrValB;
`input_gpr		wrIdC;
input[63:0]		wrValC;

`input_gpr		wrIdD;
input[63:0]		wrValD;
`input_gpr		wrIdE;
input[63:0]		wrValE;
`input_gpr		wrIdF;
input[63:0]		wrValF;

input[63:0]		regInVal;
input			regHold1;
input			regHold2;
input			regFlush1;
input			regFlush2;

reg[63:0]		tRegVal;
reg[63:0]		tNxtRegVal;

assign 		regVal = tRegVal;

reg				tIsIdA;
reg				tIsIdB;
reg				tIsIdC;
reg				tIsIdD;
reg				tIsIdE;
reg				tIsIdF;

always @*
begin
	tNxtRegVal	= regInVal;
	if(!regFlush1 && !regHold1)
	begin
		if(wrIdC == regId)
			tNxtRegVal	= wrValC;
		if(wrIdB == regId)
			tNxtRegVal	= wrValB;
		if(wrIdA == regId)
			tNxtRegVal	= wrValA;
	end

	if(!regFlush2 && !regHold2)
	begin
		if(wrIdF == regId)
			tNxtRegVal	= wrValF;
		if(wrIdE == regId)
			tNxtRegVal	= wrValE;
		if(wrIdD == regId)
			tNxtRegVal	= wrValD;
	end
end

always @(posedge clock)
begin
	if(!regHold1 || !regHold2)
	begin
		tRegVal		<= tNxtRegVal;
	end
end

endmodule
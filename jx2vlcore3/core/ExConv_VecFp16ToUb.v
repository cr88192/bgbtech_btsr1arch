module ExConv_VecFp16ToUb(valIn, valOut, opMod);
input[63:0]		valIn;
output[63:0]	valOut;
input[3:0]		opMod;

reg[7:0]	tValA;
reg[7:0]	tValB;
reg[7:0]	tValC;
reg[7:0]	tValD;

reg[15:0]	tValSA;
reg[15:0]	tValSB;

reg[63:0]	tValOut;

assign	valOut = tValOut;

always @*
begin
	tValA = { valIn[ 9]^opMod[1], valIn[ 8: 2] };
	tValB = { valIn[25]^opMod[1], valIn[24:18] };
	tValC = { valIn[41]^opMod[1], valIn[40:34] };
	tValD = { valIn[57]^opMod[1], valIn[56:50] };
	
	tValSA = { valIn[22]^opMod[1], valIn[21: 7] };
	tValSB = { valIn[54]^opMod[1], valIn[53:39] };
	
	tValOut = opMod[2] ?
		{ 32'h0, tValSB, tValSA } :
		{ 32'h0, tValD, tValC, tValB, tValA };

	if(opMod[0])
	begin
		tValOut = {
			tValD, tValD,
			tValC, tValC,
			tValB, tValB,
			tValA, tValA };
	end
end

endmodule

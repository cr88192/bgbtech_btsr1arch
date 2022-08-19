module ExConv_VecUbToFp16(valIn, valOut, opMod);
input[63:0]		valIn;
output[63:0]	valOut;
input[3:0]		opMod;

wire[31:0]	valInSel;
assign		valInSel = opMod[0] ? valIn[63:32] : valIn[31:0];

reg[4:0]	tExp5;
reg[7:0]	tExp8;

wire		tValIs4xW;
wire		tValIs2xS;
assign		tValIs4xW = opMod[0] && opMod[3];
assign		tValIs2xS = opMod[2] && !opMod[3];

reg[9:0]	tSelFraA;
reg[9:0]	tSelFraB;
reg[9:0]	tSelFraC;
reg[9:0]	tSelFraD;

reg[15:0]	tValA;
reg[15:0]	tValB;
reg[15:0]	tValC;
reg[15:0]	tValD;

reg[31:0]	tValSA;
reg[31:0]	tValSB;

reg[63:0]	tValOut;

assign	valOut = tValOut;

always @*
begin

	tExp5 = opMod[1] ? 5'b10000 : 5'b01111;
	tExp8 = { tExp5, opMod[1] ? 3'b000 : 3'b111 };

	tSelFraA = { valInSel[ 7: 0], 2'b0 };
	tSelFraB = { valInSel[15: 8], 2'b0 };
	tSelFraC = { valInSel[23:16], 2'b0 };
	tSelFraD = { valInSel[31:24], 2'b0 };
	
	if(tValIs4xW)
	begin
		tSelFraA = valIn[15: 6];
		tSelFraB = valIn[31:22];
		tSelFraC = valIn[47:38];
		tSelFraD = valIn[63:54];
	end

	tValA = { 1'b0, tExp5, tSelFraA[9]^opMod[1], tSelFraA[8:0] };
	tValB = { 1'b0, tExp5, tSelFraB[9]^opMod[1], tSelFraB[8:0] };
	tValC = { 1'b0, tExp5, tSelFraC[9]^opMod[1], tSelFraC[8:0] };
	tValD = { 1'b0, tExp5, tSelFraD[9]^opMod[1], tSelFraD[8:0] };
	
	tValSA = { 1'b0, tExp8, valIn[15]^opMod[1], valInSel[14: 0], 7'b0 };
	tValSB = { 1'b0, tExp8, valIn[31]^opMod[1], valInSel[30:16], 7'b0 };
	
	tValOut = tValIs2xS ? { tValSB, tValSA } : { tValD, tValC, tValB, tValA };
end

endmodule

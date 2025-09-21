/*
Check if an operation may potentially update SR.T.
 */

module ExPredChkSrT(opUCmd, opUIxt, predSrT);

input[8:0]	opUCmd;
input[8:0]	opUIxt;
output		predSrT;

reg			tPredSrT;
assign	predSrT = tPredSrT;

always @*
begin
	tPredSrT =
		(opUCmd[5:0] == JX2_UCMD_OP_IXS)		||
		(opUCmd[5:0] == JX2_UCMD_OP_IXT)		||
		(opUCmd[5:0] == JX2_UCMD_ALU3)		||
		(opUCmd[5:0] == JX2_UCMD_ALUW3)		||
		(opUCmd[5:0] == JX2_UCMD_ALUB3)		||
		(opUCmd[5:0] == JX2_UCMD_ALUCMP)	||
//		(opUCmd[5:0] == JX2_UCMD_ALUCMP3R)	||
		(opUCmd[5:0] == JX2_UCMD_UNARY)		||
//		(opUCmd[5:0] == JX2_UCMD_CONV_RR)	||
		(opUCmd[5:0] == JX2_UCMD_CONV2_RR)	||
		(opUCmd[5:0] == JX2_UCMD_FCMP)		||
		(opUCmd[5:0] == JX2_UCMD_ALUCMPW)	||
		(opUCmd[5:0] == JX2_UCMD_ALUCMPB)	;

	if(opUCmd[5:0] == JX2_UCMD_ALU3)
	begin
		case(opUIxt[3:0])
			4'h0: tPredSrT = 0;
			4'h1: tPredSrT = 0;
			4'h5: tPredSrT = 0;
			4'h6: tPredSrT = 0;
			4'h7: tPredSrT = 0;
			4'hF: tPredSrT = 0;
			default: begin end
		endcase
	end

	if(opUCmd[5:0] == JX2_UCMD_CONV2_RR)
	begin
		if(opUIxt[5:4]!=2'b01)
			tPredSrT = 0;
	end

end

endmodule

/*
Operations for shift left or right by 1 bit, with SR.T update.
*/

`ifndef HAS_JX2EXOPSHLLT
`define HAS_JX2EXOPSHLLT

module ExShllT(
	regIdIxt,
	regValRm,	regOutVal,
	regInSrT,	regOutSrT);

input [ 7:0]	regIdIxt;
input [63:0]	regValRm;
output[63:0]	regOutVal;

input			regInSrT;
output			regOutSrT;

reg[63:0]		tRegOutVal;
assign		regOutVal = tRegOutVal;

reg			tRegOutSrT;
assign	regOutSrT = tRegOutSrT;

reg[63:0]		tRegSx;

reg[63:0]		tRegShll1;
reg[63:0]		tRegShlr1;

always @*
begin
	tRegOutVal	= regValRm;
	tRegOutSrT	= regInSrT;

//	tRegSx		= regValRm[63] ? UV64_FF : UV64_00;

	tRegShll1	= { regValRm[62:0], 1'b0 };
	tRegShlr1	= { 1'b0, regValRm[63:1] };

	case(regIdIxt[3:0])
		4'h0: begin
			tRegOutVal = { tRegShll1[63:1], regValRm[63] };
		end
		4'h1: begin
			tRegOutVal = { regValRm[0], tRegShlr1[62:0] };
		end
		4'h2: begin
			tRegOutVal = { tRegShll1[63:1], regInSrT };
			tRegOutSrT = regValRm[63];
		end
		4'h3: begin
			tRegOutVal = { regInSrT, tRegShlr1[62:0] };
			tRegOutSrT = regValRm[0];
		end

		4'h4: begin
			tRegOutVal = tRegShll1;
			tRegOutSrT = regValRm[63];
		end
		4'h5: begin
			tRegOutVal = tRegShlr1;
			tRegOutSrT = regValRm[0];
		end
		4'h6: begin
			tRegOutVal = { regValRm[63], tRegShlr1[62:0] };
			tRegOutSrT = regValRm[0];
		end
		
		default: begin
		end
	endcase

end

endmodule

module FpuVecShMux4(
	regValA,	regValB,	regValC,	regValD,
	regOut,		idxShuf,	idxDfl,		doShuf);

output[31:0]	regOut;
input [31:0]	regValA;
input [31:0]	regValB;
input [31:0]	regValC;
input [31:0]	regValD;
input [ 1:0]	idxShuf;
input [ 1:0]	idxDfl;
input			doShuf;

wire[1:0]		idx;
assign			idx = doShuf ? idxShuf : idxDfl;

assign		regOut = idx[1] ?
	(idx[0] ? regValD : regValC) :
	(idx[0] ? regValB : regValA) ;

endmodule

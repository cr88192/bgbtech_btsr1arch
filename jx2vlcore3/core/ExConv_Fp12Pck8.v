`ifndef HAS_FP12PCK8
`define HAS_FP12PCK8

module ExConv_Fp12Pck8(valI, valO, hasSgn);
input [11:0]	valI;
output[ 7:0]	valO;
input[1:0]		hasSgn;

reg[7:0]	tValO;
assign		valO = tValO;

reg[3:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[11];
	tExpC	= { valI[10], valI[8:6] };

	if(valI[10] == valI[9])
		tExpC = valI[10] ? 4'hF : 4'h0 ;

	if(hasSgn[0])
		tValO = { tSgn, tExpC, valI[5:3] };
	else
		tValO = { tExpC, valI[5:2] };
end

endmodule

`endif

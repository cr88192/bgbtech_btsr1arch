`ifndef HAS_FP32PCK16
`define HAS_FP32PCK16

module ExConv_Fp32Pck16(valI, valO);
input [31:0]	valI;
output[15:0]	valO;

reg[15:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[31];
	tExpC	= { valI[30], valI[26:23] };

	if((valI[30:26] != 5'h0F) && (valI[30:26] != 5'h10))
		tExpC = valI[30] ? 5'h1F : 5'h00 ;

	tValO = { tSgn, tExpC, valI[22:13] };
end

endmodule

`endif

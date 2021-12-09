/*
/ Expand FP8 (E3.F5) to FP12 (S.E5.F6)
Expand FP8 (E4.F4) to FP12 (S.E5.F6)
 */

`ifndef HAS_FP8EXP12
`define HAS_FP8EXP12

module ExConv_Fp8Exp12(valI, valO, hasSgn);
input [ 7:0]	valI;
output[11:0]	valO;
input[1:0]		hasSgn;

reg[11:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg[3:0]	tFraC;
reg			tSgn;

always @*
begin
	if(hasSgn[0])
	begin
		tSgn = valI[7];
//		tExpC = { valI[6], !valI[5], valI[5:3] };
		tExpC = { valI[6], !valI[6], valI[5:3] };
		tFraC = { valI[2:0], 1'b0 };
	end
	else
	begin
		tSgn = 0;
//		tExpC = { !valI[7], valI[7:4] };
//		tExpC = { valI[7], !valI[6], valI[6:4] };
		tExpC = { valI[7], !valI[7], valI[6:4] };
		tFraC = valI[3:0];
	end

	if(!hasSgn[1] && (tExpC==5'h08))
		tExpC=0;
	tValO = { tSgn, tExpC, tFraC, 2'b0 };
end

endmodule

`endif

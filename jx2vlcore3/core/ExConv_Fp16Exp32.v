`ifndef HAS_FP16EXP32
`define HAS_FP16EXP32

module ExConv_Fp16Exp32(valI, valE, valO);
input [15:0]	valI;
output[31:0]	valO;
input [ 4:0]	valE;

reg[31:0]	tValO;
assign		valO = tValO;

reg[7:0]	tExpC;
reg[9:0]	tFraC;
reg			tSgn;

always @*
begin
	tSgn = valI[15];
	tExpC = { valI[14]?3'b000:3'b111, valI[14:10] };
	tFraC = valI[9:0];
	if(valI[14:10]==5'h00)
//	if((valI[14:10]==5'h00) || valE[5])
		tExpC=0;
//	tValO = { tSgn, tExpC, tFraC, 13'h0 };
	tValO = { tSgn, tExpC, tFraC, valE[4:0], 8'h0 };
end

endmodule

`endif

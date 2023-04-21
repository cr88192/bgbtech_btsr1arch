`ifndef HAS_FP16UPCKAL
`define HAS_FP16UPCKAL

module ExConv_Fp16UPckAl(valI, valO);
input [ 7:0]	valI;
output[15:0]	valO;

reg[15:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[7];
//	tExpC	= { 2'b01, valI[6:4] };
	tExpC	= { 2'b01, valI[6:4] } - 1;
	tValO = { tSgn, tExpC, valI[3:0], 6'h00 };
end

endmodule

`endif

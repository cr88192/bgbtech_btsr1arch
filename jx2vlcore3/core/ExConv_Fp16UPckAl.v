`ifndef HAS_FP16UPCKAL
`define HAS_FP16UPCKAL

module ExConv_Fp16UPckAl(valI, valO, isfp8);
input [ 7:0]	valI;
output[15:0]	valO;
input			isfp8;

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

	if(isfp8)
	begin
		tExpC	= { valI[6], !valI[6], valI[5:3] };
		tValO = { tSgn, tExpC, valI[2:0], 7'h00 };
	end
end

endmodule

`endif

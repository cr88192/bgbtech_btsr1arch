`ifndef HAS_FP16PCKAL
`define HAS_FP16PCKAL

module ExConv_Fp16PckAl(valI, valO);
input [15:0]	valI;
output[ 7:0]	valO;

reg[7:0]	tValO;
assign		valO = tValO;

reg[4:0]	tExpA;
reg[2:0]	tExpC;
reg			tSgn;

always @*
begin
	tSgn	= valI[15];
	tExpA	= valI[14:10] + 1;
//	tExpC	= { valI[12:10] } + 1;
	tExpC	= tExpA[2:0];
//	if(valI[14] == valI[13])
//	if(tExpA[4] == tExpA[3])
	if(tExpA[4] || (tExpA[4:3] == 2'b00))
		tExpC = tExpA[4] ? 3'h7 : 3'h0 ;
	tValO = { tSgn, tExpC, valI[9:6] };
end

endmodule

`endif

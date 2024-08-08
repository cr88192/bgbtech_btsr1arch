`ifndef HAS_FP16PCKAL
`define HAS_FP16PCKAL

module ExConv_Fp16PckAl(valI, valO, isfp8);
input [15:0]	valI;
output[ 7:0]	valO;
input			isfp8;

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
//	if(tExpA[4] || (tExpA[4:3] == 2'b00))
//		tExpC = tExpA[4] ? 3'h7 : 3'h0 ;
	tValO = { tSgn, tExpC, valI[9:6] };

//	if(isfp8)
//		tValO = { tSgn, valI[14], valI[12:10], valI[9:7] };

//	if(tExpA[4] || (tExpA[4:3] == 2'b00))
	if((tExpA[4] && !isfp8) || (tExpA[4] == tExpA[3]))
		tValO = tExpA[4] ? { tSgn, 7'h7F } : 8'h00;
end

endmodule

`endif

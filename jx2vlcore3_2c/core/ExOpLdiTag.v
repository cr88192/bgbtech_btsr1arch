module ExOpLdiTag(
	clock,		reset,		idUCmd,		idUIxt,
	valRs,		valRi,		valRn,
	);

input	clock;
input	reset;

input[63:0]		valRs;
output[9:0]		valRi;
output[63:0]	valRn;
input[7:0]		idUCmd;
input[7:0]		idUIxt;

reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[31:0]	tValSelRs;
reg[31:0]	tValSelRn;

wire	tOpQ;

always @*
begin
	tOpQ	= idUIxt[4];
	tValRn	= valRs;
	
	tValSelRs	= tOpQ ? valRs[63:32] : valRs[31:0];
	tValSelRn	= tValSelRs;
	
	if(valRi[9:6]==0)
	begin
		casez(valRi[5:0])
			6'b000000: tValSelRn		= tValSelRs;
			6'b000001: tValSelRn		= tValSelRs;
			6'b000010: tValSelRn		= tValSelRs;
			6'b000011: tValSelRn		= tValSelRs;
			6'b00010z: tValSelRn[    0] = valRi[  0];
			6'b00011z: tValSelRn[   31] = valRi[  0];
			6'b0010zz: tValSelRn[1 : 0] = valRi[1:0];
			6'b0011zz: tValSelRn[31:30] = valRi[1:0];
			6'b010zzz: tValSelRn[2 : 0] = valRi[2:0];
			6'b011zzz: tValSelRn[31:29] = valRi[2:0];
			6'b10zzzz: tValSelRn[3 : 0] = valRi[3:0];
			6'b11zzzz: tValSelRn[31:28] = valRi[3:0];
		endcase
	end
	else
	begin
		casez(valRi[9:5])
			5'b00010: tValSelRn[4 : 0] = valRi[4:0];
			5'b00011: tValSelRn[31:27] = valRi[4:0];
			5'b0010z: tValSelRn[5 : 0] = valRi[5:0];
			5'b0011z: tValSelRn[31:26] = valRi[5:0];
			5'b010zz: tValSelRn[6 : 0] = valRi[6:0];
			5'b011zz: tValSelRn[31:25] = valRi[6:0];
			5'b10zzz: tValSelRn[7 : 0] = valRi[7:0];
			5'b11zzz: tValSelRn[31:24] = valRi[7:0];
		endcase
	end
	
	if(tOpQ)
		tValRn[63:32]=tValSelRn;
	else
		tValRn[31:0]=tValSelRn;
end

endmodule

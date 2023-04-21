`ifndef has_ExConv_Rgb5MinMax
`define has_ExConv_Rgb5MinMax

module ExConv_Rgb5MinMax(valI, valO);
input[63:0]		valI;
output[31:0]	valO;

reg[15:0]	tValMin;
reg[15:0]	tValMax;

// assign	valO = { tValMax, tValMin };
assign	valO = { tValMin, tValMax };

reg[15:0]	tValI0;
reg[15:0]	tValI1;
reg[15:0]	tValI2;
reg[15:0]	tValI3;

reg[7:0]	tValY0;
reg[7:0]	tValY1;
reg[7:0]	tValY2;
reg[7:0]	tValY3;

reg		tValGt01;
reg		tValGt12;
reg		tValGt23;
reg		tValGt30;
reg		tValGt02;
reg		tValGt13;
reg		tValGt03;

reg[1:0]	tSelMin;
reg[1:0]	tSelMax;


always @*
begin

	tValI0 = valI[15: 0];
	tValI1 = valI[31:16];
	tValI2 = valI[47:32];
	tValI3 = valI[63:48];
//	tValY0 = { 1'b0, tValI0[9:5] };
//	tValY1 = { 1'b0, tValI1[9:5] };
//	tValY2 = { 1'b0, tValI2[9:5] };
//	tValY3 = { 1'b0, tValI3[9:5] };

`ifndef def_true
	tValY0 = {
		tValI0[9], tValI0[15], tValI0[4], 
		tValI0[8], tValI0[14], tValI0[3], 
		tValI0[7], tValI0[13] };
	tValY1 = {
		tValI1[9], tValI1[15], tValI1[4], 
		tValI1[8], tValI1[14], tValI1[3], 
		tValI1[7], tValI1[13] };
	tValY2 = {
		tValI2[9], tValI2[15], tValI2[4], 
		tValI2[8], tValI2[14], tValI2[3], 
		tValI2[7], tValI2[13] };
	tValY3 = {
		tValI3[9], tValI3[15], tValI3[4], 
		tValI3[8], tValI3[14], tValI3[3], 
		tValI3[7], tValI3[13] };
`endif

`ifdef def_true
	tValY0 = {
		tValI0[ 9], tValI0[ 8], tValI0[15], tValI0[ 4], 
		tValI0[ 7], tValI0[14], tValI0[ 3], tValI0[ 6] };
	tValY1 = {
		tValI1[ 9], tValI1[ 8], tValI1[15], tValI1[ 4], 
		tValI1[ 7], tValI1[14], tValI1[ 3], tValI1[ 6] };
	tValY2 = {
		tValI2[ 9], tValI2[ 8], tValI2[15], tValI2[ 4], 
		tValI2[ 7], tValI2[14], tValI2[ 3], tValI2[ 6] };
	tValY3 = {
		tValI3[ 9], tValI3[ 8], tValI3[15], tValI3[ 4], 
		tValI3[ 7], tValI3[14], tValI3[ 3], tValI3[ 6] };
`endif

	tValGt01 = tValY0 > tValY1;
	tValGt12 = tValY1 > tValY2;
	tValGt23 = tValY2 > tValY3;
	tValGt30 = tValY3 > tValY0;
	tValGt02 = tValY0 > tValY2;
	tValGt13 = tValY1 > tValY3;
//	tValGt03 = !tValGt30;
	tValGt03 = tValY0 > tValY3;

	tSelMin = tValGt01 ?
		(tValGt23 ?
			(tValGt13 ? 2'b11 : 2'b01) :
			(tValGt12 ? 2'b10 : 2'b01) ) :
		(tValGt23 ?
			(tValGt03 ? 2'b11 : 2'b00) :
			(tValGt02 ? 2'b10 : 2'b00) ) ;

	tSelMax = tValGt01 ?
		(tValGt23 ?
			(tValGt02 ? 2'b00 : 2'b10) :
			(tValGt03 ? 2'b00 : 2'b11) ) :
		(tValGt23 ?
			(tValGt12 ? 2'b01 : 2'b10) :
			(tValGt13 ? 2'b01 : 2'b11) ) ;

	case(tSelMin)
		2'b00: tValMin = tValI0;
		2'b01: tValMin = tValI1;
		2'b10: tValMin = tValI2;
		2'b11: tValMin = tValI3;
	endcase

	case(tSelMax)
		2'b00: tValMax = tValI0;
		2'b01: tValMax = tValI1;
		2'b10: tValMax = tValI2;
		2'b11: tValMax = tValI3;
	endcase

end

endmodule

`endif

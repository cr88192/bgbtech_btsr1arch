/*
Color Cell Encode Helper.

Rs will give the 4 pixel values as RGB555.
Rt will give the Min and Max RGB555 values for the block.

Rp will give the initial state of the destination register.
The result will consist of Rp shifted right by 8 bits,
with the 4 new pixels added to the output.
 */

`ifndef HAS_EXBTCENCCC1
`define HAS_EXBTCENCCC1

module ExBtcEncCc(
	clock,	reset,
	regValRs,
	regValRt,
	regValRp,
	idUIxt,
	exHold,
	regOutVal
	);

input			clock;
input			reset;
input			exHold;

input[63:0]		regValRs;
input[31:0]		regValRt;
input[31:0]		regValRp;
input[8:0]		idUIxt;

output[31:0]	regOutVal;

(* max_fanout = 200 *)
	wire			exHoldN;

assign	exHoldN = !exHold;

reg[31:0]	tRegOutVal;
reg[31:0]	tRegOutVal2;
assign	regOutVal = tRegOutVal2;

reg[8:0]	tIdUIxtB;

reg[15:0]	tValI0;
reg[15:0]	tValI1;
reg[15:0]	tValI2;
reg[15:0]	tValI3;

reg[15:0]	tValI0B;
reg[15:0]	tValI1B;
reg[15:0]	tValI2B;
reg[15:0]	tValI3B;

reg[15:0]	tRefMin0;
reg[15:0]	tRefMax0;
reg[15:0]	tRefHi0;
reg[15:0]	tRefLo0;
reg[15:0]	tRefMi0;

reg[7:0]	tValY0;
reg[7:0]	tValY1;
reg[7:0]	tValY2;
reg[7:0]	tValY3;

reg[7:0]	tRefHiY0;
reg[7:0]	tRefLoY0;
reg[7:0]	tRefMiY0;

reg[7:0]	tValY0B;
reg[7:0]	tValY1B;
reg[7:0]	tValY2B;
reg[7:0]	tValY3B;

reg[7:0]	tRefHiY0B;
reg[7:0]	tRefLoY0B;
reg[7:0]	tRefMiY0B;

reg[1:0]	tIdxY0;
reg[1:0]	tIdxY1;
reg[1:0]	tIdxY2;
reg[1:0]	tIdxY3;

reg[31:0]		tRegValRpB;


reg		tValGt01;
reg		tValGt12;
reg		tValGt23;
reg		tValGt30;
reg		tValGt02;
reg		tValGt13;
reg		tValGt03;

reg[1:0]	tSelMin;
reg[1:0]	tSelMax;

reg[15:0]	tValMin;
reg[15:0]	tValMax;


always @*
begin
	tRegOutVal = UV32_XX;
	
	/* Stage 0 */

	tValI0 = regValRs[15: 0];
	tValI1 = regValRs[31:16];
	tValI2 = regValRs[47:32];
	tValI3 = regValRs[63:48];

	tRefMin0 = regValRt[31:16];
	tRefMax0 = regValRt[15: 0];

	/* 5/8 Max + 3/8 Min */
	tRefHi0 =
	{	2'b0, tRefMax0[14:11],
		1'b0, tRefMax0[ 9: 6],
		1'b0, tRefMax0[ 4: 1] } +
	{	3'b0, tRefMin0[14:12],
		2'b0, tRefMin0[ 9: 7],
		2'b0, tRefMin0[ 4: 2] } +
	{	4'b0, tRefMax0[14:13],
		3'b0, tRefMax0[ 9: 8],
		3'b0, tRefMax0[ 4: 3] } +
	{	4'b0, tRefMin0[14:13],
		3'b0, tRefMin0[ 9: 8],
		3'b0, tRefMin0[ 4: 3] } ;

	/* 5/8 Min + 3/8 Max */
	tRefLo0 =
	{	2'b0, tRefMin0[14:11],
		1'b0, tRefMin0[ 9: 6],
		1'b0, tRefMin0[ 4: 1] } +
	{	3'b0, tRefMax0[14:12],
		2'b0, tRefMax0[ 9: 7],
		2'b0, tRefMax0[ 4: 2] } +
	{	4'b0, tRefMin0[14:13],
		3'b0, tRefMin0[ 9: 8],
		3'b0, tRefMin0[ 4: 3] } +
	{	4'b0, tRefMax0[14:13],
		3'b0, tRefMax0[ 9: 8],
		3'b0, tRefMax0[ 4: 3] } ;

	/* 1/2 Min + 1/2 Max */
	tRefMi0 =
	{	2'b0, tRefMin0[14:11],
		1'b0, tRefMin0[ 9: 6],
		1'b0, tRefMin0[ 4: 1] } +
	{	2'b0, tRefMax0[14:11],
		1'b0, tRefMax0[ 9: 6],
		1'b0, tRefMax0[ 4: 1] } ;

	tRefHiY0 =
	{	tRefHi0[ 9], tRefHi0[ 8], tRefHi0[14], tRefHi0[ 4], 
		tRefHi0[ 7], tRefHi0[13], tRefHi0[ 3], tRefHi0[ 6] };
	tRefLoY0 =
	{	tRefLo0[ 9], tRefLo0[ 8], tRefLo0[14], tRefLo0[ 4], 
		tRefLo0[ 7], tRefLo0[13], tRefLo0[ 3], tRefLo0[ 6] };
	tRefMiY0 =
	{	tRefMi0[ 9], tRefMi0[ 8], tRefMi0[14], tRefMi0[ 4], 
		tRefMi0[ 7], tRefMi0[13], tRefMi0[ 3], tRefMi0[ 6] };

	tValY0 = {
		tValI0[ 9], tValI0[ 8], tValI0[14], tValI0[ 4], 
		tValI0[ 7], tValI0[13], tValI0[ 3], tValI0[ 6] };
	tValY1 = {
		tValI1[ 9], tValI1[ 8], tValI1[14], tValI1[ 4], 
		tValI1[ 7], tValI1[13], tValI1[ 3], tValI1[ 6] };
	tValY2 = {
		tValI2[ 9], tValI2[ 8], tValI2[14], tValI2[ 4], 
		tValI2[ 7], tValI2[13], tValI2[ 3], tValI2[ 6] };
	tValY3 = {
		tValI3[ 9], tValI3[ 8], tValI3[14], tValI3[ 4], 
		tValI3[ 7], tValI3[13], tValI3[ 3], tValI3[ 6] };

	/* Stage 1 */

	tIdxY0 = (tValY0B > tRefMiY0B) ?
		( (tValY0B > tRefHiY0B) ? 2'b11 : 2'b10 ) :
		( (tValY0B > tRefLoY0B) ? 2'b01 : 2'b00 ) ;
	tIdxY1 = (tValY1B > tRefMiY0B) ?
		( (tValY1B > tRefHiY0B) ? 2'b11 : 2'b10 ) :
		( (tValY1B > tRefLoY0B) ? 2'b01 : 2'b00 ) ;
	tIdxY2 = (tValY2B > tRefMiY0B) ?
		( (tValY2B > tRefHiY0B) ? 2'b11 : 2'b10 ) :
		( (tValY2B > tRefLoY0B) ? 2'b01 : 2'b00 ) ;
	tIdxY3 = (tValY3B > tRefMiY0B) ?
		( (tValY3B > tRefHiY0B) ? 2'b11 : 2'b10 ) :
		( (tValY3B > tRefLoY0B) ? 2'b01 : 2'b00 ) ;

`ifdef jx2_enable_rgb5minmax_mul
	tValGt01 = tValY0B > tValY1B;
	tValGt12 = tValY1B > tValY2B;
	tValGt23 = tValY2B > tValY3B;
	tValGt30 = tValY3B > tValY0B;
	tValGt02 = tValY0B > tValY2B;
	tValGt13 = tValY1B > tValY3B;
	tValGt03 = tValY0B > tValY3B;

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
		2'b00: tValMin = tValI0B;
		2'b01: tValMin = tValI1B;
		2'b10: tValMin = tValI2B;
		2'b11: tValMin = tValI3B;
	endcase

	case(tSelMax)
		2'b00: tValMax = tValI0B;
		2'b01: tValMax = tValI1B;
		2'b10: tValMax = tValI2B;
		2'b11: tValMax = tValI3B;
	endcase
`endif

//	tRegOutVal = {
//		tIdxY3, tIdxY2,
//		tIdxY1, tIdxY0,
//		tRegValRpB[31:8] };

	if(tIdUIxtB[5:0] == JX2_UCIX_MUL3_ENCCC1)
	begin
		tRegOutVal = {
			tRegValRpB[23:0],
			tIdxY3, tIdxY2,
			tIdxY1, tIdxY0 };
	end

	if(tIdUIxtB[5:0] == JX2_UCIX_MUL3_ENCCC2)
	begin
		tRegOutVal = {
			tRegValRpB[23:0],
			tIdxY0, tIdxY1,
			tIdxY2, tIdxY3 };
	end

`ifdef jx2_enable_rgb5minmax_mul
	if(tIdUIxtB[5:0] == JX2_UCIX_MUL3_RGB5MINMAX)
	begin
		tRegOutVal = { tValMin, tValMax };
	end
`endif

end

always @(posedge clock)
begin
	if(exHoldN)
	begin
		tIdUIxtB	<= idUIxt;

		tValI0B		<= tValI0;
		tValI1B		<= tValI1;
		tValI2B		<= tValI2;
		tValI3B		<= tValI3;

		tValY0B		<= tValY0;
		tValY1B		<= tValY1;
		tValY2B		<= tValY2;
		tValY3B		<= tValY3;

		tRefHiY0B	<= tRefHiY0;
		tRefLoY0B	<= tRefLoY0;
		tRefMiY0B	<= tRefMiY0;
		tRegValRpB	<= regValRp;
		
		tRegOutVal2	<= tRegOutVal;
	end
end


endmodule

`endif

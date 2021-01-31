/*
Extract a pixel from a block.
Block format resembles BC1, but uses a pair of RGB555 endpoints.
 */

module ExBcnBlk1(
	/* verilator lint_off UNUSED */
	regValRs,
	regValRt,
	idUIxt,
	regOutVal
	);

input[63:0]		regValRs;
input[ 3:0]		regValRt;
input[7:0]		idUIxt;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

reg[63:0]		tRegValRs;
reg[15:0]		tPb0A;
reg[15:0]		tPb0B;
reg[31:0]		tPb0P;
reg[ 5:0]		tPb0a;
reg[ 5:0]		tPb0b;

reg[23:0]		tPc0A;
reg[23:0]		tPc0B;
reg[23:0]		tPc0C;
reg[23:0]		tPc0D;
reg[23:0]		tPc0E;

reg[8:0]		tPcCr0;
reg[8:0]		tPcCg0;
reg[8:0]		tPcCb0;
reg[8:0]		tPcCa0;

reg[8:0]		tPcCr1;
reg[8:0]		tPcCg1;
reg[8:0]		tPcCb1;
reg[8:0]		tPcCa1;

reg[8:0]		tPcCr2;
reg[8:0]		tPcCg2;
reg[8:0]		tPcCb2;

reg[23:0]		tPc1A;
reg[23:0]		tPc1B;
reg[23:0]		tPc1C;
reg[23:0]		tPc1D;

reg[1:0]		tPx0;

always @*
begin
	tRegValRs = regValRs;
	tPb0A = tRegValRs[15: 0];
	tPb0B = tRegValRs[31:16];
	tPb0P = tRegValRs[63:32];
	
	tPb0a = { tPb0A[10], tPb0A[5], tPb0A[0], tPb0A[10], tPb0A[5], tPb0A[0] };
	tPb0b = { tPb0B[10], tPb0B[5], tPb0B[0], tPb0B[10], tPb0B[5], tPb0B[0] };
	
	tPc0A[ 5: 0] = { tPb0A[ 4: 0], tPb0A[ 4] };
	tPc0A[11: 6] = { tPb0A[ 9: 5], tPb0A[ 9] };
	tPc0A[17:12] = { tPb0A[14:10], tPb0A[14] };
	tPc0A[23:18] = tPb0a;
	tPc0B[ 5: 0] = { tPb0B[ 4: 0], tPb0B[ 4] };
	tPc0B[11: 6] = { tPb0B[ 9: 5], tPb0B[ 9] };
	tPc0B[17:12] = { tPb0B[14:10], tPb0B[14] };
	tPc0B[23:18] = tPb0b;
	
	tPcCr0 =	{ 1'b0, tPc0A[ 5: 0], 2'b0 } + { 3'b0, tPc0A[ 5: 0] } +
				{ 2'b0, tPc0B[ 5: 0], 1'b0 } + { 3'b0, tPc0B[ 5: 0] };
	tPcCg0 =	{ 1'b0, tPc0A[11: 6], 2'b0 } + { 3'b0, tPc0A[11: 6] } +
				{ 2'b0, tPc0B[11: 6], 1'b0 } + { 3'b0, tPc0B[11: 6] };
	tPcCb0 =	{ 1'b0, tPc0A[17:12], 2'b0 } + { 3'b0, tPc0A[17:12] } +
				{ 2'b0, tPc0B[17:12], 1'b0 } + { 3'b0, tPc0B[17:12] };
	tPcCa0 =	{ 1'b0, tPc0A[23:18], 2'b0 } + { 3'b0, tPc0A[23:18] } +
				{ 2'b0, tPc0B[23:18], 1'b0 } + { 3'b0, tPc0B[23:18] };
	tPc0C[ 5: 0] = tPcCr0[8:3];
	tPc0C[11: 6] = tPcCg0[8:3];
	tPc0C[17:12] = tPcCb0[8:3];
	tPc0C[23:18] = tPcCa0[8:3];
	tPcCr1 =	{ 1'b0, tPc0B[ 5: 0], 2'b0 } + { 3'b0, tPc0B[ 5: 0] } +
				{ 2'b0, tPc0A[ 5: 0], 1'b0 } + { 3'b0, tPc0A[ 5: 0] };
	tPcCg1 =	{ 1'b0, tPc0B[11: 6], 2'b0 } + { 3'b0, tPc0B[11: 6] } +
				{ 2'b0, tPc0A[11: 6], 1'b0 } + { 3'b0, tPc0A[11: 6] };
	tPcCb1 =	{ 1'b0, tPc0B[17:12], 2'b0 } + { 3'b0, tPc0B[17:12] } +
				{ 2'b0, tPc0A[17:12], 1'b0 } + { 3'b0, tPc0A[17:12] };
	tPcCa1 =	{ 1'b0, tPc0B[23:18], 2'b0 } + { 3'b0, tPc0B[23:18] } +
				{ 2'b0, tPc0A[23:18], 1'b0 } + { 3'b0, tPc0A[23:18] };
	tPc0D[ 5: 0] = tPcCr1[8:3];
	tPc0D[11: 6] = tPcCg1[8:3];
	tPc0D[17:12] = tPcCb1[8:3];
	tPc0D[23:18] = tPcCa1[8:3];

	tPcCr2 =	{ 1'b0, tPc0B[ 5: 0], 2'b0 } +
				{ 1'b0, tPc0A[ 5: 0], 2'b0 } ;
	tPcCg2 =	{ 1'b0, tPc0B[11: 6], 2'b0 } +
				{ 1'b0, tPc0A[11: 6], 2'b0 } ;
	tPcCb2 =	{ 1'b0, tPc0B[17:12], 2'b0 } +
				{ 1'b0, tPc0A[17:12], 2'b0 } ;
	tPc0E[ 5: 0] = tPcCr2[8:3];
	tPc0E[11: 6] = tPcCg2[8:3];
	tPc0E[17:12] = tPcCb2[8:3];
	tPc0E[23:18] = 6'h3F;
	
	case( { tPb0A[15], tPb0B[15] } )
		2'b00: begin
			/* Opaque Block */
			tPc1A = { 6'h3F, tPc0A[17:0] };
			tPc1B = { 6'h3F, tPc0B[17:0] };
			tPc1C = { 6'h3F, tPc0C[17:0] };
			tPc1D = { 6'h3F, tPc0D[17:0] };
		end
		2'b01: begin
			/* 1b Alpha */
			tPc1A = { 6'h3F, tPc0A[17:0] };
			tPc1B = { 6'h3F, tPc0B[17:0] };
			tPc1C = { 6'h3F, tPc0E[17:0] };
			tPc1D = 24'h0;
		end
		2'b10: begin
			/* 1b Color + 1b Alpha */
			tPc1A = { tPc0A[23:18], tPc0A[17:0] };
			tPc1B = { tPc0A[23:18], tPc0B[17:0] };
			tPc1C = { tPc0B[23:18], tPc0A[17:0] };
			tPc1D = { tPc0B[23:18], tPc0B[17:0] };
		end
		2'b11: begin
			/* Interpolated RGBA */
			tPc1A = { tPc0A[23:18], tPc0A[17:0] };
			tPc1B = { tPc0B[23:18], tPc0B[17:0] };
			tPc1C = { tPc0C[23:18], tPc0C[17:0] };
			tPc1D = { tPc0D[23:18], tPc0D[17:0] };
		end
	endcase
	
	case(regValRt[3:0])
		4'h0: tPx0 = tPb0P[1:0];
		4'h0: tPx0 = tPb0P[3:2];
	endcase
end

endmodule
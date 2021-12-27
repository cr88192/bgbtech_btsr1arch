`ifndef HAS_EXBTCUAB1
`define HAS_EXBTCUAB1

`include "ExBtcUabUf8.v"

module ExBtcUab1(
	clock,	reset,
	regValRs,
	regValRt,
	idUIxt,
	regOutVal
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[ 3:0]		regValRt;
input[8:0]		idUIxt;

output[63:0]	regOutVal;


wire[31:0]	blkI;
wire[ 3:0]	idxI;

assign	blkI = regValRs[31:0];
assign	idxI = regValRt[ 3:0];


reg[11:0]	tValOut;
reg[11:0]	tValBias;

assign	regOutVal = { 32'h0,
	tValOut-tValBias, 4'h0,
	tValOut+tValBias, 4'h0 };

wire[ 7:0]	tUfA;
wire[ 7:0]	tUfB;
wire[ 7:0]	tUfE;
wire[ 7:0]	tUfL;
wire[11:0]	tUfA1;
wire[11:0]	tUfB1;
wire[11:0]	tUfE1;
wire[11:0]	tUfL1;

wire[ 3:0]	tUfSxA1;
wire[ 3:0]	tUfSxB1;
assign	tUfSxA1 = tUfA1[11] ? 4'hF : 4'h0;
assign	tUfSxB1 = tUfB1[11] ? 4'hF : 4'h0;

ExBtcUabUf8		ufA(clock, reset, tUfA, tUfA1);
ExBtcUabUf8		ufB(clock, reset, tUfB, tUfB1);
ExBtcUabUf8		ufE(clock, reset, tUfE, tUfE1);
ExBtcUabUf8		ufL(clock, reset, tUfL, tUfL1);

assign	tUfA = idUIxt[0] ? blkI[ 7: 0] : {       blkI[ 5: 0], 2'b00 };
assign	tUfB = idUIxt[0] ? blkI[15: 8] : {       blkI[11: 6], 2'b00 };
assign	tUfE = idUIxt[0] ? blkI[23:16] : { 1'b0, blkI[15:12], 3'b00 };
assign	tUfL = idUIxt[0] ? blkI[31:24] : 8'h00;

wire[15:0]	tUbB;
assign		tUbB = blkI[31:16];

wire[3:0]	tIdxI1;
assign	tIdxI1 = idxI;

reg[1:0]		tSelBit1;
reg[1:0]		tSelBit2;
reg[1:0]		tSelBit3;


reg[11:0]	tUfA2;
reg[11:0]	tUfB2;
reg[11:0]	tUfE2;
reg[11:0]	tUfL2;
reg[ 3:0]	tUfSxA2;
reg[ 3:0]	tUfSxB2;
reg[3:0]	tIdxI2;
reg[11:0]	tUfE2B;
reg[11:0]	tUfE2A;

reg[11:0]	tLerpVa;
reg[11:0]	tLerpVb;
reg[11:0]	tLerpVc;
reg[11:0]	tLerpVd;

reg[11:0]	tLerpVe;
reg[11:0]	tLerpVf;
reg[11:0]	tLerpVg;
reg[11:0]	tLerpVh;

reg[11:0]	tUfV3;
reg[11:0]	tUfE3;
reg[11:0]	tUfL3;

always @*
begin
	/* Stage 1*/
//	tSelBit1 = tUfB[tIdxI1];

	if(idUIxt[0])
	begin
		case(tIdxI1)
			4'h0: tSelBit1 = regValRs[33:32];
			4'h1: tSelBit1 = regValRs[35:34];
			4'h2: tSelBit1 = regValRs[37:36];
			4'h3: tSelBit1 = regValRs[39:38];
			4'h4: tSelBit1 = regValRs[41:40];
			4'h5: tSelBit1 = regValRs[43:42];
			4'h6: tSelBit1 = regValRs[45:44];
			4'h7: tSelBit1 = regValRs[47:46];
			4'h8: tSelBit1 = regValRs[49:48];
			4'h9: tSelBit1 = regValRs[51:50];
			4'hA: tSelBit1 = regValRs[53:52];
			4'hB: tSelBit1 = regValRs[55:54];
			4'hC: tSelBit1 = regValRs[57:56];
			4'hD: tSelBit1 = regValRs[59:58];
			4'hE: tSelBit1 = regValRs[61:60];
			4'hF: tSelBit1 = regValRs[63:62];
		endcase
	end
	else
	begin
		tSelBit1 = tUbB[tIdxI1] ? 2'b11 : 2'b01;
	end

	
	/* Stage 2 */

	tLerpVa = tIdxI2[3] ?
		{tUfSxB2[3], tUfB2[11:1] } :
		{tUfSxA2[3], tUfA2[11:1] };
	tLerpVb = tIdxI2[2] ?
		{tUfSxB2[3:2], tUfB2[11:2] } :
		{tUfSxA2[3:2], tUfA2[11:2] };
	tLerpVc = tIdxI2[1] ?
		{tUfSxB2[3:1], tUfB2[11:3] } :
		{tUfSxA2[3:1], tUfA2[11:3] };
	tLerpVd = tIdxI2[0] ?
		{tUfSxB2[3:0], tUfB2[11:4] } :
		{tUfSxA2[3:0], tUfA2[11:4] };
	
	tLerpVe = tLerpVa + tLerpVb;
	tLerpVf = tLerpVc + tLerpVd;
	tLerpVg = tLerpVe + tLerpVf;
	tLerpVh = tLerpVg + {
		tLerpVg[11] ? 4'hF : 4'h0,
		tLerpVg[11:4] };

	tUfE2A = 
		{ 2'b0, tUfE2[11:2] } +
		{ 4'b0, tUfE2[11:4] } ;
	tUfE2A = tUfE2A + { 4'b0, tUfE2A[11:4] };

	tUfE2B = tIdxI2[0] ? tUfE2 : tUfE2A;

	/* Stage 3 */

	tValBias = tUfL3;
	tValOut = tUfV3 + tUfE3;
end

always @(posedge clock)
begin
	tUfA2		<= tUfA1;
	tUfB2		<= tUfB1;
	tUfE2		<= tUfE1;
	tUfL2		<= tUfL1;
	tUfSxA2		<= tUfSxA1;
	tUfSxB2		<= tUfSxB1;
	tSelBit2	<= tSelBit1;
	tIdxI2		<= tIdxI1;

	tUfL3		<= tUfL2;
	tUfV3		<= tLerpVh;
	tUfE3		<= tIdxI2[1] ? tUfE2B : (~tUfE2B);
end
endmodule

`endif

`ifndef HAS_ExRgbHBlend
`define HAS_ExRgbHBlend

module ExRgbHBlend(
	valIn00,	valIn01,
	valIn10,	valIn11,
	valSelS,	valSelT,
	valOut);

input[63:0]		valIn00;
input[63:0]		valIn01;
input[63:0]		valIn10;
input[63:0]		valIn11;

input[3:0]		valSelS;
input[3:0]		valSelT;

output[63:0]	valOut;

reg[63:0]	tValOut;
assign	valOut = tValOut;

reg[7:0]	tVal00R;
reg[7:0]	tVal00G;
reg[7:0]	tVal00B;
reg[7:0]	tVal00A;

reg[7:0]	tVal01R;
reg[7:0]	tVal01G;
reg[7:0]	tVal01B;
reg[7:0]	tVal01A;

reg[7:0]	tVal10R;
reg[7:0]	tVal10G;
reg[7:0]	tVal10B;
reg[7:0]	tVal10A;

reg[7:0]	tVal11R;
reg[7:0]	tVal11G;
reg[7:0]	tVal11B;
reg[7:0]	tVal11A;

reg[7:0]	tValAd0R;
reg[7:0]	tValAd0G;
reg[7:0]	tValAd0B;
reg[7:0]	tValAd0A;

reg[7:0]	tValAd1R;
reg[7:0]	tValAd1G;
reg[7:0]	tValAd1B;
reg[7:0]	tValAd1A;

reg[7:0]	tValAd2R;
reg[7:0]	tValAd2G;
reg[7:0]	tValAd2B;
reg[7:0]	tValAd2A;

always @*
begin
	tVal00A = valIn00[63:56];	tVal00R = valIn00[47:40];
	tVal00G = valIn00[31:24];	tVal00B = valIn00[15: 8];
	tVal01A = valIn01[63:56];	tVal01R = valIn01[47:40];
	tVal01G = valIn01[31:24];	tVal01B = valIn01[15: 8];
	tVal10A = valIn10[63:56];	tVal10R = valIn10[47:40];
	tVal10G = valIn10[31:24];	tVal10B = valIn10[15: 8];
	tVal11A = valIn11[63:56];	tVal11R = valIn11[47:40];
	tVal11G = valIn11[31:24];	tVal11B = valIn11[15: 8];

	tValAd0R =
		( valSelS[3] ? {1'b0, tVal01R[7:1]} : {1'b0, tVal00R[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal01R[7:2]} : {2'b0, tVal00R[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal01R[7:3]} : {3'b0, tVal00R[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal01R[7:3]} : {3'b0, tVal00R[7:3]} ) ;
	tValAd0G =
		( valSelS[3] ? {1'b0, tVal01G[7:1]} : {1'b0, tVal00G[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal01G[7:2]} : {2'b0, tVal00G[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal01G[7:3]} : {3'b0, tVal00G[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal01G[7:3]} : {3'b0, tVal00G[7:3]} ) ;
	tValAd0B =
		( valSelS[3] ? {1'b0, tVal01B[7:1]} : {1'b0, tVal00B[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal01B[7:2]} : {2'b0, tVal00B[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal01B[7:3]} : {3'b0, tVal00B[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal01B[7:3]} : {3'b0, tVal00B[7:3]} ) ;
	tValAd0A =
		( valSelS[3] ? {1'b0, tVal01A[7:1]} : {1'b0, tVal00A[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal01A[7:2]} : {2'b0, tVal00A[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal01A[7:3]} : {3'b0, tVal00A[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal01A[7:3]} : {3'b0, tVal00A[7:3]} ) ;

	tValAd1R =
		( valSelS[3] ? {1'b0, tVal11R[7:1]} : {1'b0, tVal10R[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal11R[7:2]} : {2'b0, tVal10R[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal11R[7:3]} : {3'b0, tVal10R[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal11R[7:3]} : {3'b0, tVal10R[7:3]} ) ;
	tValAd1G =
		( valSelS[3] ? {1'b0, tVal11G[7:1]} : {1'b0, tVal10G[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal11G[7:2]} : {2'b0, tVal10G[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal11G[7:3]} : {3'b0, tVal10G[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal11G[7:3]} : {3'b0, tVal10G[7:3]} ) ;
	tValAd1B =
		( valSelS[3] ? {1'b0, tVal11B[7:1]} : {1'b0, tVal10B[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal11B[7:2]} : {2'b0, tVal10B[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal11B[7:3]} : {3'b0, tVal10B[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal11B[7:3]} : {3'b0, tVal10B[7:3]} ) ;
	tValAd1A =
		( valSelS[3] ? {1'b0, tVal11A[7:1]} : {1'b0, tVal10A[7:1]} ) +
		( valSelS[2] ? {2'b0, tVal11A[7:2]} : {2'b0, tVal10A[7:2]} ) +
		( valSelS[1] ? {3'b0, tVal11A[7:3]} : {3'b0, tVal10A[7:3]} ) +
		( valSelS[0] ? {3'b0, tVal11A[7:3]} : {3'b0, tVal10A[7:3]} ) ;

	tValAd2R =
		( valSelT[3] ? {1'b0, tValAd1R[7:1]} : {1'b0, tValAd0R[7:1]} ) +
		( valSelT[2] ? {2'b0, tValAd1R[7:2]} : {2'b0, tValAd0R[7:2]} ) +
		( valSelT[1] ? {3'b0, tValAd1R[7:3]} : {3'b0, tValAd0R[7:3]} ) +
		( valSelT[0] ? {3'b0, tValAd1R[7:3]} : {3'b0, tValAd0R[7:3]} ) ;
	tValAd2G =
		( valSelT[3] ? {1'b0, tValAd1G[7:1]} : {1'b0, tValAd0G[7:1]} ) +
		( valSelT[2] ? {2'b0, tValAd1G[7:2]} : {2'b0, tValAd0G[7:2]} ) +
		( valSelT[1] ? {3'b0, tValAd1G[7:3]} : {3'b0, tValAd0G[7:3]} ) +
		( valSelT[0] ? {3'b0, tValAd1G[7:3]} : {3'b0, tValAd0G[7:3]} ) ;
	tValAd2B =
		( valSelT[3] ? {1'b0, tValAd1B[7:1]} : {1'b0, tValAd0B[7:1]} ) +
		( valSelT[2] ? {2'b0, tValAd1B[7:2]} : {2'b0, tValAd0B[7:2]} ) +
		( valSelT[1] ? {3'b0, tValAd1B[7:3]} : {3'b0, tValAd0B[7:3]} ) +
		( valSelT[0] ? {3'b0, tValAd1B[7:3]} : {3'b0, tValAd0B[7:3]} ) ;
	tValAd2A =
		( valSelT[3] ? {1'b0, tValAd1A[7:1]} : {1'b0, tValAd0A[7:1]} ) +
		( valSelT[2] ? {2'b0, tValAd1A[7:2]} : {2'b0, tValAd0A[7:2]} ) +
		( valSelT[1] ? {3'b0, tValAd1A[7:3]} : {3'b0, tValAd0A[7:3]} ) +
		( valSelT[0] ? {3'b0, tValAd1A[7:3]} : {3'b0, tValAd0A[7:3]} ) ;

	tValOut = {
		tValAd2A, 8'h80,
		tValAd2R, 8'h80,
		tValAd2G, 8'h80,
		tValAd2B, 8'h80	};
end

endmodule

`endif

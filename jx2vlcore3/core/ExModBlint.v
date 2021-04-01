/*
Linear and Bilinear Interpolator.

This op will span all 3 lanes, using 6 inputs and producing 1 output.
 */

`include "CoreDefs.v"

module ExModBlint(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,		exDoHold,
	pxA,		pxB,
	pxC,		pxD,
	frS,		frT,
	pxO
	);

input			clock;
input			reset;
input			exHold;
input[7:0]		opUCmd;
input[7:0]		opUIxt;

input[63:0]		pxA;
input[63:0]		pxB;
input[63:0]		pxC;
input[63:0]		pxD;
input[63:0]		frS;
input[63:0]		frT;

output[63:0]	pxO;
output			exDoHold;

reg[63:0]		tPxO;
reg[63:0]		tNxtPxO;
reg				tDoHold;

assign			pxO = tPxO;
assign			exDoHold = tDoHold;

reg[63:0]		tPxA;
reg[63:0]		tPxB;
reg[63:0]		tPxC;
reg[63:0]		tPxD;
reg[63:0]		tPxS;
reg[63:0]		tPxT;
reg[7:0]		tUCmd;
reg[7:0]		tUIxt;

reg[63:0]		tlPxA;
reg[63:0]		tlPxB;
reg[63:0]		tlPxC;
reg[63:0]		tlPxD;
reg[63:0]		tlPxS;
reg[63:0]		tlPxT;
reg[7:0]		tlUCmd;
reg[7:0]		tlUIxt;

reg[3:0]		tCnt;
reg[3:0]		tNxtCnt;

reg[15:0]		tCvA1;
reg[15:0]		tCvB1;
reg[15:0]		tCvC1;
reg[15:0]		tCvD1;
reg[15:0]		tCvS1P;
reg[15:0]		tCvT1P;
reg[15:0]		tCvS1N;
reg[15:0]		tCvT1N;

reg[31:0]		tCvP2W;
reg[31:0]		tCvQ2W;

reg[15:0]		tCvP2;
reg[15:0]		tCvQ2;
reg[15:0]		tCvT2P;
reg[15:0]		tCvT2N;

reg[31:0]		tCvR3W;
reg[15:0]		tCvR3;

always @*
begin
	tNxtPxO		= tPxO;
//	tNxtCnt		= tCnt + 1;
//	tNxtCnt		= 0;
	tDoHold		= 0;

	tCvA1 = UV16_XX;
	tCvB1 = UV16_XX;
	tCvC1 = UV16_XX;
	tCvD1 = UV16_XX;
	tCvS1P = UV16_XX;
	tCvT1P = UV16_XX;

`ifndef def_true
	tPxA		= pxA;
	tPxB		= pxB;
	tPxC		= pxC;
	tPxD		= pxD;
	tPxS		= frS;
	tPxT		= frT;
	tUCmd		= opUCmd;
	tUIxt		= opUIxt;
	
	if(exHold)
	begin
		tPxA		= tlPxA;
		tPxB		= tlPxB;
		tPxC		= tlPxC;
		tPxD		= tlPxD;
		tPxS		= tlPxS;
		tPxT		= tlPxT;
		tUCmd		= tlUCmd;
		tUIxt		= tlUIxt;
	end
`endif

	if(tUCmd[5:0] == JX2_UCMD_BLINT)
	begin
		tDoHold		= 1;
	end
	
	case(tCnt)
		4'h0: begin
			tCvA1 = tPxA[15:0];
			tCvB1 = tPxB[15:0];
			tCvC1 = tPxC[15:0];
			tCvD1 = tPxD[15:0];
			tCvS1P = tPxS[15:0];
			tCvT1P = tPxT[15:0];
		end
		4'h1: begin
			tCvA1 = tPxA[31:16];
			tCvB1 = tPxB[31:16];
			tCvC1 = tPxC[31:16];
			tCvD1 = tPxD[31:16];
			tCvS1P = tPxS[31:16];
			tCvT1P = tPxT[31:16];
		end
		4'h2: begin
			tCvA1 = tPxA[47:32];
			tCvB1 = tPxB[47:32];
			tCvC1 = tPxC[47:32];
			tCvD1 = tPxD[47:32];
			tCvS1P = tPxS[47:32];
			tCvT1P = tPxT[47:32];
		end
		4'h3: begin
			tCvA1 = tPxA[63:48];
			tCvB1 = tPxB[63:48];
			tCvC1 = tPxC[63:48];
			tCvD1 = tPxD[63:48];
			tCvS1P = tPxS[63:48];
			tCvT1P = tPxT[63:48];
		end
		
		default: begin
		end
	endcase

	if(tUIxt[0])
	begin
		/* BLERP */
		tCvT1P = 0;
	end

	tCvS1N	= ~tCvS1P;
	tCvT1N	= ~tCvT1P;
	
	tCvP2	= tCvP2W[31:16];
	tCvQ2	= tCvQ2W[31:16];
	tCvR3	= tCvR3W[31:16];

	case(tCnt)
		4'h2: begin
			tNxtPxO[15: 0] = tCvR3;
		end
		4'h3: begin
			tNxtPxO[31:16] = tCvR3;
		end
		4'h4: begin
			tNxtPxO[47:32] = tCvR3;
		end
		4'h5: begin
			tNxtPxO[63:48] = tCvR3;
		end
		4'h6: begin
			tDoHold = 0;
		end

		default: begin
		end
	endcase

	if(tDoHold)
	begin
		tNxtCnt		= tCnt + 1;
	end
	else if(exHold)
	begin
		tNxtCnt		= tCnt;
	end
	else
	begin
		tNxtCnt		= 0;
	end
end

always @(posedge clock)
begin
`ifdef def_true
//	if(!exHold)
	if(!exHold && !tDoHold)
	begin
		tPxA	<= pxA;
		tPxB	<= pxB;
		tPxC	<= pxC;
		tPxD	<= pxD;
		tPxS	<= frS;
		tPxT	<= frT;
		tUCmd	<= opUCmd;
		tUIxt	<= opUIxt;
	end
`endif

	tlPxA	<= tPxA;
	tlPxB	<= tPxB;
	tlPxC	<= tPxC;
	tlPxD	<= tPxD;
	tlPxS	<= tPxS;
	tlPxT	<= tPxT;
	tlUCmd	<= tUCmd;
	tlUIxt	<= tUIxt;

	tPxO		<= tNxtPxO;
	tCnt		<= tNxtCnt;

	tCvP2W		<=
		( { 16'h0000, tCvA1 } * { 16'h0000, tCvS1N } ) + 
		( { 16'h0000, tCvB1 } * { 16'h0000, tCvS1P } ) ;
	tCvQ2W		<=
		( { 16'h0000, tCvC1 } * { 16'h0000, tCvS1N } ) + 
		( { 16'h0000, tCvD1 } * { 16'h0000, tCvS1P } ) ;
	tCvT2P	<= tCvT1P;
	tCvT2N	<= tCvT1N;

	tCvR3W		<=
		( { 16'h0000, tCvP2 } * { 16'h0000, tCvT1N } ) + 
		( { 16'h0000, tCvQ2 } * { 16'h0000, tCvT1P } ) ;

end

endmodule

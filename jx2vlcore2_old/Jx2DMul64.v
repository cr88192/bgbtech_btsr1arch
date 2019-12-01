/*
Does a 64-bit Widening Multiply.

Takes two 64 bit inputs and produces a 128 bit output.
 */

module Jx2DMul64(clock,
	valInA, valInB,		vMode
	valOutLo, valOutHi,
	valOK);

input			clock;

input[63:0]		valInA;
input[63:0]		valInB;
input[2:0]		vMode;

output[63:0]	valOutLo;
output[63:0]	valOutHi;
output[2:0]		valOK;


reg[63:0]		tValOutLo;
reg[63:0]		tValOutHi;
reg[2:0]		tValOK;

assign			valOutLo	= tValOutLo;
assign			valOutHi	= tValOutHi;
assign			valOK		= tValOK;

reg[3:0]		tState;
reg[3:0]		tNextState;

reg[31:0]		tValSliceA0;
reg[31:0]		tValSliceB0;

reg[31:0]		tValSliceA1;
reg[31:0]		tValSliceB1;

reg[31:0]		tValMulP_AA0;
reg[31:0]		tValMulP_AB0;
reg[31:0]		tValMulP_BA0;
reg[31:0]		tValMulP_BB0;

reg[31:0]		tValMulP_AA1;
reg[31:0]		tValMulP_AB1;
reg[31:0]		tValMulP_BA1;
reg[31:0]		tValMulP_BB1;

reg[63:0]		tValMulQ_A0;
reg[63:0]		tValMulQ_B0;

reg[63:0]		tValMulQ_A1;
reg[63:0]		tValMulQ_B1;
reg[64:0]		tValMulQ_C1;

reg[64:0]		tValMulQ_C2;

reg[64:0]		tValMulR_A0;
reg[64:0]		tValMulR_B0;
reg[64:0]		tValMulR_C0;
reg[64:0]		tValMulR_D0;

reg[64:0]		tValMulR_A1;
reg[64:0]		tValMulR_B1;
reg[64:0]		tValMulR_C1;
reg[64:0]		tValMulR_D1;

reg[32:0]		tValMulS_A0;
reg[32:0]		tValMulS_B0;
reg[32:0]		tValMulS_C0;
reg[32:0]		tValMulS_D0;
reg[32:0]		tValMulS_E0;
reg[32:0]		tValMulS_F0;

reg[32:0]		tValMulS_A1;
reg[32:0]		tValMulS_B1;
reg[32:0]		tValMulS_C1;
reg[32:0]		tValMulS_D1;
reg[32:0]		tValMulS_E1;
reg[32:0]		tValMulS_F1;

reg[32:0]		tValMulS_A2;
reg[32:0]		tValMulS_B2;
reg[32:0]		tValMulS_C2;
reg[32:0]		tValMulS_D2;

reg[32:0]		tValMulS_A3;
reg[32:0]		tValMulS_B3;
reg[32:0]		tValMulS_C3;
reg[32:0]		tValMulS_D3;

reg[32:0]		tValMulS_A4;
reg[32:0]		tValMulS_B4;
reg[32:0]		tValMulS_C4;
reg[32:0]		tValMulS_D4;

reg[32:0]		tValMulS_A5;
reg[32:0]		tValMulS_B5;
reg[32:0]		tValMulS_C5;
reg[32:0]		tValMulS_D5;

reg[127:0]		tValMulT_A0;

always @*
begin
//	tNextState = (vMode != 0) ? tState : 0;
	
	tValMulR_A0 = tValMulR_A1;
	tValMulR_B0 = tValMulR_B1;
	tValMulR_C0 = tValMulR_C1;
	tValMulR_D0 = tValMulR_D1;
	
	tValMulS_A0 = tValMulS_A1;
	tValMulS_B0 = tValMulS_B1;
	tValMulS_C0 = tValMulS_C1;
	tValMulS_D0 = tValMulS_D1;
	tValMulS_E0 = tValMulS_E1;
	tValMulS_F0 = tValMulS_F1;

	tValOK = UMEM_OK_READY;
	tNextState = 0;

	if(vMode != 0)
	begin
		tValOK = UMEM_OK_HOLD;
		tNextState = tState + 1;
	end

	case(tState)
		4'h0: begin
			tValSliceA0 = valInA[31:0];
			tValSliceB0 = valInB[31:0];
		end
		4'h1: begin
			tValSliceA0 = valInA[31: 0];
			tValSliceB0 = valInB[63:32];
		end
		4'h2: begin
			tValSliceA0 = valInA[63:32];
			tValSliceB0 = valInB[31: 0];
		end
		4'h3: begin
			tValSliceA0 = valInA[63:32];
			tValSliceB0 = valInB[63:32];
		end

		4'h4: begin
			tValMulR_A0 = tValMulQ_C2;
		end
		4'h5: begin
			tValMulR_B0 = tValMulQ_C2;
		end
		4'h6: begin
			tValMulR_C0 = tValMulQ_C2;
		end
		4'h7: begin
			tValMulR_D0 = tValMulQ_C2;
		end

		4'h8: begin
		    /*
		          Ah Al
		       Bh Bl
		       Ch Cl
		    Dh Dl
		    
		      A = Al
		      B = Ah+Bl
		      C = Cl
		      D = Bh + Ch
		      E = Dl
		      F = Dh
		     */
			tValMulS_A0 = { 1'b0, tValMulR_A1[31:0] };
			tValMulS_B0 =
				{ 1'b0, tValMulR_A1[63:32] } +
				{ 1'b0, tValMulR_B1[31: 0] } 9 ;
			tValMulS_C0 = { 1'b0, tValMulR_C1[31:0] };
			tValMulS_D0 =
				{ 1'b0, tValMulR_B1[63:32] } +
				{ 1'b0, tValMulR_C1[63:32] } +
				tValMulR_A1[64];
			tValMulS_E0 = { 1'b0, tValMulR_D1[31: 0] };
			tValMulS_F0 = { 1'b0, tValMulR_D1[63:32] } +
				tValMulR_B1[64] +
				tValMulR_C1[64];
		end

		4'h9: begin
			tValMulS_A2 = tValMulS_A1;
			tValMulS_B2 =
				{ 1'b0, tValMulS_B1[31:0] } +
				{ 1'b0, tValMulS_C1[31:0] };
			tValMulS_C2 =
				{ 1'b0, tValMulS_D1[31:0] } +
				{ 1'b0, tValMulS_E1[31:0] } + 
				tValMulS_B1[32];
			tValMulS_D2 =
				{ 1'b0, tValMulS_F1[31:0] } + 
				tValMulS_D1[32];
		end

		4'hA: begin
			tValMulS_A4 = tValMulS_A3;
			tValMulS_B4 = tValMulS_B3 + tValMulS_A3[32];
			tValMulS_C4 = tValMulS_C3 + tValMulS_B3[32];
			tValMulS_D4 = tValMulS_D3 + tValMulS_C3[32];
		end

		4'hB: begin
			tValMulT_A0 =
				{ tValMulS_D5[31:0],	tValMulS_C5[31:0] ,
				  tValMulS_B5[31:0],	tValMulS_A5[31:0] };
			tValOutLo = tValMulT_A0[ 63: 0];
			tValOutHi = tValMulT_A0[127:64];

			tNextState = tState;
			tValOK = UMEM_OK_OK;
		end

	endcase

	tValMulP_AA0 =
		{ 16'h0000, tValSliceA1[15: 0] } *
		{ 16'h0000, tValSliceB1[15: 0] };
	tValMulP_AB0 =
		{ 16'h0000, tValSliceA1[15: 0] } *
		{ 16'h0000, tValSliceB1[31:16] };
	tValMulP_BA0 =
		{ 16'h0000, tValSliceA1[31:16] } *
		{ 16'h0000, tValSliceB1[15: 0] };
	tValMulP_BB0 =
		{ 16'h0000, tValSliceA1[31:16] } *
		{ 16'h0000, tValSliceB1[31:16] };
		
	tValMulQ_A0 = { tValMulP_BB1, tValMulP_AA1 };
	tValMulQ_B0 = { 15'h0000,
		{1'b0, tValMulP_AB1[31:0]} +
		{1'b0, tValMulP_BA1[31:0]},
		UV16_00 };
	
//	tValMulQ_A2 =
//		{ 1'b0, tValMulQ_A1[31:0] } +
//		{ 1'b0, tValMulQ_B1[31:0] };
//	tValMulQ_B2 =
//		{ 1'b0, tValMulQ_A1[63:32] } +
//		{ 1'b0, tValMulQ_B1[63:32] } + 0;
//	tValMulQ_C2 =
//		{ 1'b0, tValMulQ_A1[63:32] } +
//		{ 1'b0, tValMulQ_B1[63:32] } + 1;

	tValMulQ_C1 =
		{ 1'b0, tValMulQ_A1 } +
		{ 1'b0, tValMulQ_B1 };

end


always @(posedge clock)
begin
	tValSliceA1		<= tValSliceA0;		//Cyc+1
	tValSliceB1		<= tValSliceB0;		//Cyc+1

	tValMulP_AA1	<= tValMulP_AA0;	//Cyc+2
	tValMulP_AB1	<= tValMulP_AB0;	//Cyc+2
	tValMulP_BA1	<= tValMulP_BA0;	//Cyc+2
	tValMulP_BB1	<= tValMulP_BB0;	//Cyc+2
	tValMulQ_A1		<= tValMulQ_A0;		//Cyc+3
	tValMulQ_B1		<= tValMulQ_B0;		//Cyc+3
	tValMulQ_C2		<= tValMulQ_C1;		//Cyc+4
	
	tValMulR_A1		<= tValMulR_A0;		//Cyc+5
	tValMulR_B1		<= tValMulR_B0;		//Cyc+6
	tValMulR_C1		<= tValMulR_C0;		//Cyc+7
	tValMulR_D1		<= tValMulR_D0;		//Cyc+8

	tValMulS_A1		<= tValMulS_A0;
	tValMulS_B1		<= tValMulS_B0;
	tValMulS_C1		<= tValMulS_C0;
	tValMulS_D1		<= tValMulS_D0;
	tValMulS_E1		<= tValMulS_E0;
	tValMulS_F1		<= tValMulS_F0;

	tValMulS_A3		<= tValMulS_A2;
	tValMulS_B3		<= tValMulS_B2;
	tValMulS_C3		<= tValMulS_C2;
	tValMulS_D3		<= tValMulS_D2;

	tValMulS_A5		<= tValMulS_A4;
	tValMulS_B5		<= tValMulS_B4;
	tValMulS_C5		<= tValMulS_C4;
	tValMulS_D5		<= tValMulS_D4;
end


endmodule

/*
Approximate reciprocal of 32-bit fixed-point number.
Value is assumed to represent a 8.24 fixed point value.
 */

`ifndef HAS_ExLuRcpFix32A_8p24
`define HAS_ExLuRcpFix32A_8p24

module ExLuRcpFix32A_8p24(valA, valRcp);

input[31:0] valA;
output[31:0] valRcp;

reg[31:0]	tValRcp;
assign	valRcp = tValRcp;


reg[31:0]	tValU;
reg[31:0]	tValU1;
reg[31:0]	tValU2;
reg[31:0]	tValU3;
reg[31:0]	tValU4;
reg[31:0]	tValU5;

reg[15:0]	tFrac;
reg[15:0]	tFracN;
reg[15:0]	tBias;
reg[3:0]	tExp;
reg			tSgn;

always @*
begin
	tSgn	= valA[31];
	tValU	= tSgn?(~valA):valA;

	tValU1=tValU[31: 0];

	if(tValU1[31:24]==8'h00)
		begin	tValU2={ tValU1[23: 0],  8'h0 }; tExp[3]=0;		end
	else
		begin	tValU2=tValU1[31:0]; tExp[3]=1;		end

	if(tValU2[31:28]==4'h0)
		begin	tValU3={ tValU2[27: 0],  4'h0 }; tExp[2]=0;		end
	else
		begin	tValU3=tValU2[31:0]; tExp[2]=1;		end

	if(tValU3[31:30]==2'h0)
		begin	tValU4={ tValU3[29: 0],  2'h0 }; tExp[1]=0;		end
	else
		begin	tValU4=tValU3[31:0]; tExp[1]=1;		end

	if(tValU3[31]==1'h0)
		begin	tValU5={ tValU4[30: 0],  1'h0 }; tExp[0]=0;		end
	else
		begin	tValU5=tValU4[31:0]; tExp[0]=1;		end
		
	tFrac = tValU5[30:15];
	
	case(tFrac[15:10])
		6'h00: tBias=16'h0003;
		6'h01: tBias=16'h0023;
		6'h02: tBias=16'h0060;
		6'h03: tBias=16'h00B9;
		6'h04: tBias=16'h012E;
		6'h05: tBias=16'h01BD;
		6'h06: tBias=16'h0265;
		6'h07: tBias=16'h0325;
		6'h08: tBias=16'h03FC;
		6'h09: tBias=16'h04E9;
		6'h0A: tBias=16'h05EB;
		6'h0B: tBias=16'h0701;
		6'h0C: tBias=16'h082B;
		6'h0D: tBias=16'h0968;
		6'h0E: tBias=16'h0AB6;
		6'h0F: tBias=16'h0C16;
		6'h10: tBias=16'h0D87;
		6'h11: tBias=16'h0F07;
		6'h12: tBias=16'h1098;
		6'h13: tBias=16'h1237;
		6'h14: tBias=16'h13E4;
		6'h15: tBias=16'h15A0;
		6'h16: tBias=16'h1769;
		6'h17: tBias=16'h193E;
		6'h18: tBias=16'h1B21;
		6'h19: tBias=16'h1D0F;
		6'h1A: tBias=16'h1F09;
		6'h1B: tBias=16'h210F;
		6'h1C: tBias=16'h231F;
		6'h1D: tBias=16'h253A;
		6'h1E: tBias=16'h2760;
		6'h1F: tBias=16'h298F;
		6'h20: tBias=16'h2BC8;
		6'h21: tBias=16'h2E0A;
		6'h22: tBias=16'h3055;
		6'h23: tBias=16'h32A9;
		6'h24: tBias=16'h3506;
		6'h25: tBias=16'h376B;
		6'h26: tBias=16'h39D8;
		6'h27: tBias=16'h3C4C;
		6'h28: tBias=16'h3EC8;
		6'h29: tBias=16'h414C;
		6'h2A: tBias=16'h43D7;
		6'h2B: tBias=16'h4668;
		6'h2C: tBias=16'h4901;
		6'h2D: tBias=16'h4BA0;
		6'h2E: tBias=16'h4E45;
		6'h2F: tBias=16'h50F1;
		6'h30: tBias=16'h53A2;
		6'h31: tBias=16'h565A;
		6'h32: tBias=16'h5917;
		6'h33: tBias=16'h5BDA;
		6'h34: tBias=16'h5EA2;
		6'h35: tBias=16'h6170;
		6'h36: tBias=16'h6442;
		6'h37: tBias=16'h671A;
		6'h38: tBias=16'h69F7;
		6'h39: tBias=16'h6CD9;
		6'h3A: tBias=16'h6FBF;
		6'h3B: tBias=16'h72A9;
		6'h3C: tBias=16'h7599;
		6'h3D: tBias=16'h788C;
		6'h3E: tBias=16'h7B84;
		6'h3F: tBias=16'h7E80;
	endcase

	tFracN = (~tFrac) + tBias;

	tValU5 = { 1'b1, tFracN, 15'h0 };
	tValU4 = tExp[0] ? {  1'b0, tValU5[31: 1] } : tValU5[31:0];
	tValU3 = tExp[1] ? {  2'b0, tValU4[31: 2] } : tValU4[31:0];
	tValU2 = tExp[2] ? {  4'b0, tValU3[31: 4] } : tValU3[31:0];
	tValU1 = tExp[3] ? {  8'b0, tValU3[31: 8] } : tValU2[31:0];
	tValU  = tValU1[31:0];
	
	tValRcp = tSgn ? ~tValU : tValU;
end

endmodule

`endif

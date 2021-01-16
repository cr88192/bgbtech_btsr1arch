/*
Multiply two 72 bit values and produce a 144 bit value.

Square ( 72*72->144 ):
        AE
      AF  BE
    AG  BF  CE
  AH  BG  CF  DE
    BH  CG  DF
      CH  DG
        DH 
 
 */

`include "ExCsAdd36C.v"
`include "ExCsAdd72C.v"

`include "ExCsAdd80F.v"

// `include "ExCsAdd108C.v"
`include "ExCsAdd108D.v"

// `include "ExCsAdd144F.v"
`include "ExCsAdd144G.v"

`ifndef HAS_CSMUL72W
`define HAS_CSMUL72W

module ExCsMul72W(clock, hold, valA, valB, valC);

input			clock;
input			hold;

input[71:0]		valA;
input[71:0]		valB;
output[143:0]	valC;

reg[71:0]		tValA;
reg[71:0]		tValB;
reg[143:0]		tValC;

assign		valC = tValC;

reg[35:0]		tFraA1_AE;
reg[35:0]		tFraA1_AF;
reg[35:0]		tFraA1_AG;
reg[35:0]		tFraA1_AH;
reg[35:0]		tFraA1_BE;
reg[35:0]		tFraA1_BF;
reg[35:0]		tFraA1_BG;
reg[35:0]		tFraA1_BH;
reg[35:0]		tFraA1_CE;
reg[35:0]		tFraA1_CF;
reg[35:0]		tFraA1_CG;
reg[35:0]		tFraA1_CH;
reg[35:0]		tFraA1_DE;
reg[35:0]		tFraA1_DF;
reg[35:0]		tFraA1_DG;
reg[35:0]		tFraA1_DH;

reg[35:0]		tFraA2_AE;
reg[35:0]		tFraA2_AF;
reg[35:0]		tFraA2_AG;
reg[35:0]		tFraA2_AH;
reg[35:0]		tFraA2_BE;
reg[35:0]		tFraA2_BF;
reg[35:0]		tFraA2_BG;
reg[35:0]		tFraA2_BH;
reg[35:0]		tFraA2_CE;
reg[35:0]		tFraA2_CF;
reg[35:0]		tFraA2_CG;
reg[35:0]		tFraA2_CH;
reg[35:0]		tFraA2_DE;
reg[35:0]		tFraA2_DF;
reg[35:0]		tFraA2_DG;
reg[35:0]		tFraA2_DH;

reg[143:0]		tFraB2_P0;

reg[107:0]		tFraB2_P1S;
reg[107:0]		tFraB2_P1T;
wire[108:0]		tFraB2_P1C;
// ExCsAdd108C	taddb2p1(
ExCsAdd108D	taddb2p1(clock, hold,
	tFraB2_P1S, tFraB2_P1T, tFraB2_P1C);

reg[71:0]		tFraB2_P2S;
reg[71:0]		tFraB2_P2T;
wire[72:0]		tFraB2_P2C;
ExCsAdd72C	taddb2p2(tFraB2_P2S, tFraB2_P2T, tFraB2_P2C);

reg[35:0]		tFraB2_P3S;
reg[35:0]		tFraB2_P3T;
wire[36:0]		tFraB2_P3C;
ExCsAdd36C	taddb2p3(tFraB2_P3S, tFraB2_P3T, tFraB2_P3C);

reg[143:0]		tFraB3_P0;

// reg[108:0]	tFraB3_P1C;
reg[72:0]		tFraB3_P2C;
reg[36:0]		tFraB3_P3C;

reg[79:0]		tFraB3_Q1S;
reg[79:0]		tFraB3_Q1T;
wire[79:0]		tFraB3_Q1C;
ExCsAdd80F	taddb3q1(tFraB3_Q1S, tFraB3_Q1T, tFraB3_Q1C);


reg[143:0]		tFraB4_P0;
reg[108:0]		tFraB4_P1C;
// reg[72:0]		tFraB3_P2C;
// reg[36:0]		tFraB3_P3C;

reg[143:0]		tFraB4_Q0S;
reg[143:0]		tFraB4_Q0T;
wire[143:0]		tFraB4_Q0C;
// ExCsAdd144F	taddb4q0(
ExCsAdd144G		taddb4q0(clock, hold,
	tFraB4_Q0S, tFraB4_Q0T, tFraB4_Q0C);

reg[79:0]		tFraB4_Q1C;
reg[79:0]		tFraB5_Q1C;

reg[143:0]		tFraB6_Q0C;
reg[79:0]		tFraB6_Q1C;

reg[143:0]		tFraB6_RS;
reg[143:0]		tFraB6_RT;
wire[143:0]		tFraB6_RC;
// ExCsAdd144F	taddb6r0(
ExCsAdd144G		taddb6r0(clock, hold,
	tFraB6_RS, tFraB6_RT, tFraB6_RC);

// reg[143:0]		tFraB7_RC;
reg[143:0]		tFraB8_RC;


always @*
begin
	tFraA1_AE = {18'h0, tValA[71:54]} * {18'h0, tValB[71:54]};
	tFraA1_AF = {18'h0, tValA[71:54]} * {18'h0, tValB[53:36]};
	tFraA1_AG = {18'h0, tValA[71:54]} * {18'h0, tValB[35:18]};
	tFraA1_AH = {18'h0, tValA[71:54]} * {18'h0, tValB[17: 0]};
	tFraA1_BE = {18'h0, tValA[53:36]} * {18'h0, tValB[71:54]};
	tFraA1_BF = {18'h0, tValA[53:36]} * {18'h0, tValB[53:36]};
	tFraA1_BG = {18'h0, tValA[53:36]} * {18'h0, tValB[35:18]};
	tFraA1_BH = {18'h0, tValA[53:36]} * {18'h0, tValB[17: 0]};
	tFraA1_CE = {18'h0, tValA[35:18]} * {18'h0, tValB[71:54]};
	tFraA1_CF = {18'h0, tValA[35:18]} * {18'h0, tValB[53:36]};
	tFraA1_CG = {18'h0, tValA[35:18]} * {18'h0, tValB[35:18]};
	tFraA1_CH = {18'h0, tValA[35:18]} * {18'h0, tValB[17: 0]};
	tFraA1_DE = {18'h0, tValA[17: 0]} * {18'h0, tValB[71:54]};
	tFraA1_DF = {18'h0, tValA[17: 0]} * {18'h0, tValB[53:36]};
	tFraA1_DG = {18'h0, tValA[17: 0]} * {18'h0, tValB[35:18]};
	tFraA1_DH = {18'h0, tValA[17: 0]} * {18'h0, tValB[17: 0]};

	tFraB2_P0  = { tFraA2_AE, tFraA2_CE, tFraA2_DF, tFraA2_DH};
	tFraB2_P1S = { tFraA2_AF, tFraA2_AH, tFraA2_CH };
	tFraB2_P1T = { tFraA2_BE, tFraA2_BG, tFraA2_DG };
	tFraB2_P2S = { tFraA2_AG, tFraA2_BH };
	tFraB2_P2T = { tFraA2_BF, tFraA2_CG };
	tFraB2_P3S = { tFraA2_CF };
	tFraB2_P3T = { tFraA2_DE };

	tFraB3_Q1S = { 7'h0, tFraB3_P2C };
	tFraB3_Q1T = { 7'h0, 18'h0, tFraB3_P3C, 18'h0 };

	tFraB4_Q0S = { tFraB4_P0 };
	tFraB4_Q0T = { 17'h0, tFraB4_P1C, 18'h0 };

	tFraB6_RS = { tFraB6_Q0C };
	tFraB6_RT = { 28'h0, tFraB6_Q1C, 36'h0 };
	
	tValC = tFraB8_RC;
end

always @(posedge clock)
begin
	if(!hold)
	begin
		tValA		<= valA;
		tValB		<= valB;

		tFraA2_AE	<= tFraA1_AE;
		tFraA2_AF	<= tFraA1_AF;
		tFraA2_AG	<= tFraA1_AG;
		tFraA2_AH	<= tFraA1_AH;
		tFraA2_BE	<= tFraA1_BE;
		tFraA2_BF	<= tFraA1_BF;
		tFraA2_BG	<= tFraA1_BG;
		tFraA2_BH	<= tFraA1_BH;
		tFraA2_CE	<= tFraA1_CE;
		tFraA2_CF	<= tFraA1_CF;
		tFraA2_CG	<= tFraA1_CG;
		tFraA2_CH	<= tFraA1_CH;
		tFraA2_DE	<= tFraA1_DE;
		tFraA2_DF	<= tFraA1_DF;
		tFraA2_DG	<= tFraA1_DG;
		tFraA2_DH	<= tFraA1_DH;

		tFraB3_P0	<= tFraB2_P0;
//		tFraB3_P1C	<= tFraB2_P1C;
		tFraB3_P2C	<= tFraB2_P2C;
		tFraB3_P3C	<= tFraB2_P3C;

		tFraB4_P0	<= tFraB3_P0;
		tFraB4_P1C	<= tFraB2_P1C;

		tFraB4_Q1C	<= tFraB3_Q1C;

//		tFraB5_Q0C	<= tFraB4_Q0C;
		tFraB5_Q1C	<= tFraB4_Q1C;

		tFraB6_Q0C	<= tFraB4_Q0C;
		tFraB6_Q1C	<= tFraB5_Q1C;

		tFraB8_RC	<= tFraB6_RC;
	end
end

endmodule

`endif

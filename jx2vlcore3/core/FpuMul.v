/*
SPDX-License-Identifier: Apache-2.0
Copyright (c) 2018-2026 Brendan G Bohannon

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/
*/

/*
Binary64 Multiplier.
This will nominally multiply two Binary64 inputs and produce a Binary64 result.

This may additionally produce a 128-bit extended output.
The layout of this output will mirror Binary128, but will be non-normalized and non-rounded.
  (127)=Sign
  (126:125): Unused for now
  (124:112): Raw Exponent
  (111:  0): Mantissa (Non-Normalized)
 */

`ifndef HAS_FPUMUL
`define HAS_FPUMUL

`include "CoreDefs.v"
`include "ExCsAdd64F.v"

`ifdef jx2_fpu_fullround
`include "FpuDoRound64.v"
`endif

`ifdef jx2_fpu_fullfmac
`include "ExCsAdd112F.v"
`endif

module FpuMul(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRm,
	regValRn,
	regValRo,
	regExOp,
	regExOK,
	regRMode,
	regMulResExt
	);

input	clock;
input	reset;
input	exHold;

input[63:0]		regValRm;
input[63:0]		regValRn;
output[63:0]	regValRo;
input[3:0]		regExOp;
output[1:0]		regExOK;
input[7:0]		regRMode;
output[127:0]	regMulResExt;	//FMA wide output...


(* max_fanout = 200 *)
	wire			exHoldN;

assign	exHoldN = !exHold;

reg[63:0]		tRegValRo;
assign	regValRo	= tRegValRo;

reg[127:0]		tRegValRoExt;
assign	regMulResExt	= tRegValRoExt;

reg[1:0]		tRegExOK;
reg[1:0]		tRegExOK2;
assign	regExOK		= tRegExOK2;

reg				tSgnA1;
reg				tSgnB1;
reg				tSgnC1;
reg[10:0]		tExpA1;
reg[10:0]		tExpB1;
reg[12:0]		tExpC1;
reg[53:0]		tFraA1;
reg[53:0]		tFraB1;
reg				tInxC1;

reg[35:0]		tFraC1_AC;
reg[35:0]		tFraC1_BB;
reg[35:0]		tFraC1_BC;
reg[35:0]		tFraC1_CA;
reg[35:0]		tFraC1_CB;
reg[35:0]		tFraC1_CC;

reg				tFraC1_AB_Co;
reg				tFraC1_BA_Co;
reg[9:0]		tFraC1_AB_C5;
reg[9:0]		tFraC1_BA_C5;

reg[35:0]		tFraC1_AB;
reg[35:0]		tFraC1_BA;
reg[35:0]		tFraC1_AA;


reg				tSgnC2;
reg[12:0]		tExpC2;
reg				tInxC2;

reg[35:0]		tFraC2_AC;
reg[35:0]		tFraC2_BB;
reg[35:0]		tFraC2_BC;
reg[35:0]		tFraC2_CA;
reg[35:0]		tFraC2_CB;
reg[35:0]		tFraC2_CC;

reg[35:0]		tFraC2_AB;
reg[35:0]		tFraC2_BA;
reg[35:0]		tFraC2_AA;

reg				tFraC2_AB_Co;
reg				tFraC2_BA_Co;
reg[9:0]		tFraC2_AB_C5;
reg[9:0]		tFraC2_BA_C5;

reg[63:0]		tFraC2_P;
reg[63:0]		tFraC2_Q;
reg[63:0]		tFraC2_R;

reg[111:0]		tFraC2_XS;
reg[111:0]		tFraC2_XT;
reg[111:0]		tFraC2_XU;
reg[111:0]		tFraC2_XV;

wire[111:0]		tFraC2_XP;
wire[111:0]		tFraC2_XQ;

`ifdef jx2_fpu_fullfmac
ExCsAdd112F		fpmulAddXa(tFraC2_XS, tFraC2_XT, tFraC2_XP);
ExCsAdd112F		fpmulAddXb(tFraC2_XU, tFraC2_XV, tFraC2_XQ);
`else
assign		tFraC2_XP = 112'h0;
assign		tFraC2_XQ = 112'h0;
`endif


reg				tSgnC3;
reg[12:0]		tExpC3;
reg				tInxC3;

reg[63:0]		tFraC3_P;
reg[63:0]		tFraC3_Q;
reg[63:0]		tFraC3_R;

wire[63:0]		tFraC3_S;
ExCsAdd64F		fpmulAdd(tFraC3_Q, tFraC3_R, tFraC3_S);

reg[111:0]		tFraC3_XP;
reg[111:0]		tFraC3_XQ;

wire[111:0]		tFraC3_XS;

`ifdef jx2_fpu_fullfmac
ExCsAdd112F		fpmulAddXc(tFraC3_XP, tFraC3_XQ, tFraC3_XS);
`else
assign			tFraC3_XS = 112'h0;
`endif

reg				tSgnC4;
reg[12:0]		tExpC4;
reg[63:0]		tFraC4_S;
reg				tInxC4;

reg[111:0]		tFraC4_X;

reg				tSgnC4B;
reg[12:0]		tExpC4B;
reg[53:0]		tFraC4B;
reg				tInxC4B;

reg				tFraRbit4B;
reg				tFraRbit4B2;
reg[8:0]		tFraRnd4B;

reg[63:0]		tValC4;
wire[63:0]		tValC4B;
reg[127:0]		tValC4X;

`ifdef jx2_fpu_fullround
reg[1:0]		tValC4_Mode;
FpuDoRound64	fpMulRound(tValC4B, tValC4, tValC4_Mode);
`else
assign	tValC4B = tValC4;
`endif

//reg[1:0]		tExEn1;
//reg[1:0]		tExEn2;
//reg[1:0]		tExEn3;
//reg[1:0]		tExEn4;
//reg[1:0]		tExEn5;
//reg[1:0]		tExEn6;

reg[1:0]		tExDaz1;
reg[1:0]		tExDaz2;
reg[1:0]		tExDaz3;
reg[1:0]		tExDaz4;

reg[7:0]		tRegRMode1;
reg[7:0]		tRegRMode2;
reg[7:0]		tRegRMode3;
reg[7:0]		tRegRMode4;

reg[3:0]		tRegExOp1;
reg[3:0]		tRegExOp2;
reg[3:0]		tRegExOp3;
reg[3:0]		tRegExOp4;

/*
    AA
   AB
   BA
  AC
  CA
  BB
 BC
 CB
CC
RQP
 */

always @*
begin
	tRegExOK	= 0;

	/* Stage 1 */
	tSgnA1	= regValRn[63];
	tSgnB1	= regValRm[63];
	tExpA1	= regValRn[62:52];
	tExpB1	= regValRm[62:52];
//	tFraA1	= {1'b0, (tExpA1!=0), regValRn[51:0]};
//	tFraB1	= {1'b0, (tExpB1!=0), regValRm[51:0]};
	tFraA1	= {2'b01, regValRn[51:0]};
	tFraB1	= {2'b01, regValRm[51:0]};
	tExDaz1	= 0;
	tRegRMode1	= regRMode;
	tRegExOp1	= regExOp;
	
	tSgnC1	= tSgnA1 ^ tSgnB1;
	tExpC1	=
		{2'b00, tExpA1} +
		{2'b00, tExpB1} - 1023;
	
	if((tExpA1==0) || (tExpB1==0))
	begin
		tExpC1	= 0;
		
		/* Detect Subnormal */
		if(regValRn[51:44]!=0)
			tExDaz1[0]=1;
		if(regValRm[51:44]!=0)
			tExDaz1[0]=1;

`ifdef jx2_fpu_fullfmac
		if(tExpA1==0)
			tFraA1[52]=0;
		if(tExpB1==0)
			tFraB1[52]=0;
`endif
	end

	if(regValRn[17:8]!=0)
	begin
		if((regValRm[17:8]!=0) || (regValRm[35:28]!=0))
		begin
			/* Low order bits non-zero */
			tExDaz1[1]=1;
		end
	end

	if(regValRm[17:8]!=0)
	begin
		if((regValRn[17:8]!=0) || (regValRn[35:28]!=0))
		begin
			/* Low order bits non-zero */
			tExDaz1[1]=1;
		end
	end

	tInxC1=0;
	if(regRMode[3:0]==4)
		tInxC1 = (regValRn[1:0]!=0) || (regValRm[1:0]!=0);

//	$display("FpuMul: Exp %X %X %X", tExpA1, tExpB1, tExpC1);

	tFraC1_AC	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_BB	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_BC	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_CA	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[17: 0]};
	tFraC1_CB	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_CC	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_AB_Co = tFraA1[17] & tFraB1[35];
	tFraC1_BA_Co = tFraA1[35] & tFraB1[17];
	tFraC1_AB_C5 = { 5'h0, tFraA1[17:13] } * { 5'h0, tFraB1[35:31] };
	tFraC1_BA_C5 = { 5'h0, tFraA1[35:31] } * { 5'h0, tFraB1[17:13] };

`ifdef jx2_fpu_fullfmac
	tFraC1_AA	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[17: 0]};
	tFraC1_AB	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_BA	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[17: 0]};
`else
	tFraC1_AA	=	0;
	tFraC1_AB	=	0;
	tFraC1_BA	=	0;
`endif

	/* Stage 2 */

	tFraC2_P =
		{36'h0, tFraC2_AC[35:8]} +
		{36'h0, tFraC2_CA[35:8]};
	tFraC2_Q =
		{18'h0, tFraC2_BC, tFraC2_AB_C5} +
		{18'h0, tFraC2_CB, tFraC2_BA_C5} ;
	tFraC2_R =
		{tFraC2_CC, tFraC2_BB[35:8]} +
		{36'h0, tFraC2_AC[35:8]} +
		{36'h0, tFraC2_CA[35:8]};

`ifdef jx2_fpu_fullfmac
	tFraC2_XS =
		{ 4'h0, tFraC2_CC, tFraC2_BB, tFraC2_AA };
	tFraC2_XT =
		{ 22'h0, tFraC2_BC, tFraC2_AB, 18'h0 };
	tFraC2_XU =
		{ 22'h0, tFraC2_CB, tFraC2_BA, 18'h0 };
	tFraC2_XV =
		{ 40'h0, tFraC2_CA, 36'h0 } +
		{ 40'h0, tFraC2_AC, 36'h0 } ;
`endif

	/* Stage 3 */


	/* Stage 4 */

`ifdef jx2_fpu_fullround
	tValC4_Mode = 0;
`endif

//	if(tExpC4[11])
//	if(tExpC4[11] || (tExpC4==0))
	if(tExpC4[12] || (tExpC4==0))
	begin
		tSgnC4B = 0;
		tExpC4B = 0;
		tFraC4B = 0;
		tFraRbit4B	= 0;
		tFraRbit4B2	= 0;
		tInxC4B = tInxC4 || tExpC4[12];
`ifdef jx2_fpu_fullround
		tFraC4B		= tFraC4_S[61:8];
		tValC4_Mode = 2;
`endif
	end
	else
	if(tExpC4[11] || (tExpC4==2047))
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = 2047;
		tFraC4B = 0;
		tFraRbit4B	= 0;
		tFraRbit4B2	= 0;
		tInxC4B = tInxC4 || tExpC4[11];
`ifdef jx2_fpu_fullround
		tFraC4B		= tFraC4_S[61:8];
		tValC4_Mode = 3;
`endif
	end
	else
	if(tFraC4_S[61])
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4+1;
//		tFraC4B = tFraC4_S[62:9] +
//			(tFraC4_S[8]?54'h1:54'h0);
		tFraC4B = tFraC4_S[62:9];
		tFraRbit4B	= tFraC4_S[8];
		tFraRbit4B2	= tFraC4_S[10];
		tInxC4B = tInxC4 || (tFraC4_S[10:0]!=0);
	end
	else
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4;
//		tFraC4B = tFraC4_S[61:8] +
//			(tFraC4_S[7]?54'h1:54'h0);
		tFraC4B = tFraC4_S[61:8];
		tFraRbit4B	= tFraC4_S[7];
		tFraRbit4B2	= tFraC4_S[9];
		tInxC4B = tInxC4 || (tFraC4_S[9:0]!=0);
	end
	
`ifndef jx2_fpu_noround
	if(tRegRMode4[3:0]==1)
		tFraRbit4B=0;
	if(tRegRMode4[3:0]==2)
		tFraRbit4B=!tSgnC4;
	if(tRegRMode4[3:0]==3)
		tFraRbit4B=tSgnC4;
	if(tRegRMode4[3:0]!=4)
		tFraRbit4B2=0;

`ifdef jx2_fpu_fullround
	if(!tValC4_Mode[1])
		tValC4_Mode[0] = tFraRbit4B;
	tFraRnd4B	= 0;
`else

//	tFraRnd4B = { 1'b0, tFraC4B[7:0] } + { 8'b0, tFraRbit4B };
	tFraRnd4B = { 1'b0, tFraC4B[7:0] } + { 5'b0,
		tFraRbit4B2, 1'b0,
		tFraRbit4B };

	if(!tFraRnd4B[8])
		tFraC4B[7:0] = tFraRnd4B[7:0];

	if(tRegRMode4[3:0]==4)
		tFraC4B[1:0] = tInxC4B ? 2'b01 : 2'b00;
`endif

`endif

//	$display("FpuMul: ExpB %X %X", tExpC4, tExpC4B);

	tValC4		= { tSgnC4B, tExpC4B[10:0], tFraC4B[51:0] };
	tValC4X		= { tSgnC4, 2'b00, tExpC4[12:0], tFraC4_X };
	
//	$display("FpuMul: Val=%X", tValC4);

	tRegExOK[0] = tInxC4B;

`ifndef jx2_fpu_fullfmac
	if(tRegRMode4[4] && (tFraRnd4B[8] || (tExDaz4!=0)))
	begin
		//IEEE Mode & DAZ or Round Fail
		tRegExOK = UMEM_OK_FAULT;
	end
`endif

	if(tRegExOp4 != 4'h7)
		tRegExOK = 0;

end

always @(posedge clock)
begin
	if(exHoldN)
	begin
		tSgnC2			<= tSgnC1;
		tExpC2			<= tExpC1;
		tInxC2			<= tInxC1;
		tExDaz2			<= tExDaz1;
		tRegRMode2		<= tRegRMode1;
		tRegExOp2		<= tRegExOp1;

		tFraC2_AC		<= tFraC1_AC;
		tFraC2_BB		<= tFraC1_BB;
		tFraC2_BC		<= tFraC1_BC;
		tFraC2_CA		<= tFraC1_CA;
		tFraC2_CB		<= tFraC1_CB;
		tFraC2_CC		<= tFraC1_CC;

		tFraC2_AA		<= tFraC1_AA;
		tFraC2_AB		<= tFraC1_AB;
		tFraC2_BA		<= tFraC1_BA;

		tFraC2_AB_Co	<= tFraC1_AB_Co;
		tFraC2_BA_Co	<= tFraC1_BA_Co;
		tFraC2_AB_C5	<= tFraC1_AB_C5;
		tFraC2_BA_C5	<= tFraC1_BA_C5;
	end

	tSgnC3			<= tSgnC2;
	tExpC3			<= tExpC2;
	tInxC3			<= tInxC2;
	tFraC3_P		<= tFraC2_P;
	tFraC3_Q		<= tFraC2_Q;
	tFraC3_R		<= tFraC2_R;
	tExDaz3			<= tExDaz2;
	tRegRMode3		<= tRegRMode2;
	tRegExOp3		<= tRegExOp2;
	tFraC3_XP		<= tFraC2_XP;
	tFraC3_XQ		<= tFraC2_XQ;

	tSgnC4			<= tSgnC3;
	tExpC4			<= tExpC3;
	tInxC4			<= tInxC3;
	tFraC4_S		<= tFraC3_S;
	tFraC4_X		<= tFraC3_XS;
	tExDaz4			<= tExDaz3;
	tRegRMode4		<= tRegRMode3;
	tRegExOp4		<= tRegExOp3;

	tRegValRo		<= tValC4B;
	tRegExOK2		<= tRegExOK;
	tRegValRoExt	<= tValC4X;
end

endmodule

`endif

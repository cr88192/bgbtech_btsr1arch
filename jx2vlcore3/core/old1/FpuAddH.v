/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
FPU Add (Extended Precision)

Uses S.11.52 or S.15.64 as its nominal formats.

Accepts and produces either a Binary64 or Binary128.
Binary128 is partial-precision.

The AddH unit is externally similar to AddG, but is internally narrower.

Adds two input values, producing an output value.
ExOp: Is set to indicate that a floating-point op is requested.
ExOK: Set to indicate whether the operation has completed.

ExOp: 
	0: No Op
	1: Ro=Rn+Rm
	2: Ro=Rn-Rm
	3: Ro=I2F(Rn)
	4: Ro=F2I(Rn)
*/

`include "CoreDefs.v"

`include "ExCsAdd64C.v"

`include "ExCsShr64F.v"
`include "ExCsClz64F.v"
`include "ExCsShl64F.v"

module FpuAddH(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRmLo,	regValRmHi,
	regValRnLo,	regValRnHi,
	regValRoLo,	regValRoHi,
	regExOp,
	regExOK,
	regRMode
	);

input	clock;
input	reset;
input	exHold;

input[63:0]		regValRmLo;
input[63:0]		regValRmHi;
input[63:0]		regValRnLo;
input[63:0]		regValRnHi;
output[63:0]	regValRoLo;
output[63:0]	regValRoHi;
input[3:0]		regExOp;
output[1:0]		regExOK;
input[7:0]		regRMode;

reg[63:0]		tRegValRoLo2;
reg[63:0]		tRegValRoHi2;
reg[1:0]		tRegExOK2;

assign	regValRoLo	= tRegValRoLo2;
assign	regValRoHi	= tRegValRoHi2;
assign	regExOK		= tRegExOK2;

reg[63:0]		tRegValRm;
reg[63:0]		tRegValRn;

reg[63:0]		tRegValRoLo;
reg[63:0]		tRegValRoHi;
reg[1:0]		tRegExOK;

reg[127:0]		tRegValRmE;
reg[127:0]		tRegValRnE;

reg[3:0]		tRegExOp1;
reg				tExEn1;
reg				tSgnA1;
reg				tSgnB1;
reg				tInxC1;
reg				tFraNzA1;
reg				tFraNzB1;
reg[10:0]		tExpA1;
reg[10:0]		tExpB1;
reg[63:0]		tFraA1;
reg[63:0]		tFraB1;
reg				tFraGe1;
reg				tFraDti1;
reg				tFraItf1;
reg				tFraDte1;
reg				tFraEtd1;

// reg[63:0]		tFraJ1;
// reg				tFraJ1Sg;
reg[63:0]		tRegValRn1;

reg[11:0]		tExpA1D;
reg[11:0]		tExpB1D;
reg[63:0]		tFraA1D;
reg[63:0]		tFraB1D;


reg[3:0]		tRegExOp2;
reg				tExEn2;
reg				tSgnA2;
reg				tSgnB2;
reg[10:0]		tExpA2;
reg[10:0]		tExpB2;
reg[63:0]		tFraA2;
reg[63:0]		tFraB2;
// reg[63:0]		tFraJ2;
// reg				tFraJ2Sg;
reg[63:0]		tRegValRn2;
reg				tFraDti2;
reg				tFraItf2;
reg				tFraDte2;
reg				tFraEtd2;

reg				tSgnC2;
reg[10:0]		tExpC2;
reg				tInxC2;
reg[63:0]		tFraC2;
reg[63:0]		tFraC2I;

reg[63:0]		tFraC2_S;
reg[63:0]		tFraC2_T;
reg				tFraC2_Cin;
wire[64:0]		tFraC2_C;
ExCsAdd64C	tFraAdd(tFraC2_S, tFraC2_T, tFraC2_C, tFraC2_Cin);

reg[63:0]		tFraShr1_S;
wire[63:0]		tFraShr1_C;
reg[7:0]		tFraShr1_Shr;
ExCsShr64F	tFraShr(tFraShr1_S, tFraShr1_C, tFraShr1_Shr);

`ifdef def_true
reg[63:0]		tFraShr1A_S;
wire[63:0]		tFraShr1A_C;
reg[7:0]		tFraShr1A_Shr;
ExCsShr64F	tFraShrA(tFraShr1A_S, tFraShr1A_C, tFraShr1A_Shr);

reg[63:0]		tFraShr1B_S;
wire[63:0]		tFraShr1B_C;
reg[7:0]		tFraShr1B_Shr;
ExCsShr64F	tFraShrB(tFraShr1B_S, tFraShr1B_C, tFraShr1B_Shr);
`endif

reg[63:0]		tFraClz1_S;
wire[7:0]		tFraClz1_C;
ExCsClz64F	tFraClz(tFraClz1_S, tFraClz1_C);

reg[63:0]		tFraShl1_S;
wire[63:0]		tFraShl1_C;
reg[7:0]		tFraShl1_Shl;
ExCsShl64F	tFraShl(tFraShl1_S, tFraShl1_C, tFraShl1_Shl);


reg[3:0]		tRegExOp3;
reg				tExEn3;
// reg				tSgnC3;
reg				tSgnC3A;
reg[10:0]		tExpC3;
// reg[63:0]		tFraC3;
reg[63:0]		tFraC3A;
reg[63:0]		tFraC3I;
reg				tFraDte3;
reg				tFraEtd3;


reg				tInxC3;
wire			tSgnC3;
wire[63:0]		tFraC3;
assign		tSgnC3 = tSgnC3A ^ tFraC3A[63];
assign		tFraC3 = tFraC3A[63] ? (~tFraC3A) : tFraC3A;

reg				tSgnC3B;
reg[11:0]		tExpC3B;
reg[63:0]		tFraC3B;
reg				tExpIsZeroC3;
reg[6:0]		tFraShlC3B;

reg[3:0]		tRegExOp4;
reg				tExEn4;
reg				tSgnC4;
reg[11:0]		tExpC4;
reg[63:0]		tFraC4;
reg[63:0]		tFraC4I;
reg				tInxC4;
reg				tExpIsZeroC4;
reg				tExpIsZeroC4B;
reg[6:0]		tFraShlC4;
reg				tFraDte4;
reg				tFraEtd4;

reg				tSgnC4B;
reg[11:0]		tExpC4B;
reg[63:0]		tFraC4B;
reg				tInxC4B;

reg[4:0]		tValRoundC4;
reg[127:0]		tValC4E;
reg[63:0]		tValC4D;
reg[63:0]		tValC4Lo;
reg[63:0]		tValC4Hi;

reg				tFraRbit4B;
reg				tFraRbit4B2;

reg				tExEn5;

reg				tFraUseA;
reg				tFraUseB;

always @*
begin
	tRegExOp1	= regExOp;
//	tExEn1		= regExOp != 0;
	tExEn1		= regExOp[2:0] != 0;

	tRegValRm	= regValRmLo;
	tRegValRn	= regValRnLo;
	tRegValRn1	= regValRnLo;

// 	tRegExOK=UMEM_OK_READY;
	tRegExOK	= 0;
	
	/* Stage 1 */

	tRegValRmE		= { regValRmHi, regValRmLo };
	tRegValRnE		= { regValRnHi, regValRnLo };

	tFraDti1	= (tRegExOp1[2:0] == 4);
	tFraItf1	= (tRegExOp1[2:0] == 3);
	tFraDte1	= (tRegExOp1[2:0] == 5) && !tRegExOp1[3];
	tFraEtd1	= (tRegExOp1[2:0] == 5) &&  tRegExOp1[3];

	tSgnA1	= tRegValRn[63];
	tSgnB1	= tRegValRm[63] ^ regExOp[1];
	tExpA1	= tRegValRn[62:52];
	tExpB1	= tRegValRm[62:52];

	tFraNzA1	= (tExpA1 != 0);
	tFraNzB1	= (tExpB1 != 0);

	tFraA1	= {2'b0, tFraNzA1, tRegValRn[51:0], 9'h0};
	tFraB1	= {2'b0, tFraNzB1, tRegValRm[51:0], 9'h0};

	tInxC1=0;
	if(regRMode[3:0]==4)
		tInxC1 = (regValRn[1:0]!=0) || (regValRm[1:0]!=0);

	if(tFraDti1 || tFraItf1)
	begin
		tSgnB1		= 0;
//		tExpB1		= 1085;
		tExpB1		= 1084;
		tFraB1		= 0;
	end

//	tFraJ1Sg	= tRegValRn[63];
//	tFraJ1		= tRegValRn;
//	tFraJ1		= tFraJ1Sg ? (~tRegValRn) : tRegValRn;

	if(tFraItf1)
	begin
		tSgnA1	= 0;
		tExpA1	= 1084;
		tFraA1	= tRegValRn;
	end

	tExpA1D	= {1'b0, tExpA1} - {1'b0, tExpB1};
	tExpB1D	= {1'b0, tExpB1} - {1'b0, tExpA1};

	tFraUseB		= 1;

`ifndef def_true
	if(!tExpA1D[11] && !tExpB1D[11])
	begin
		if(tRegValRn[51:48] < tRegValRm[51:48])
			tFraUseB		= 0;
	end
`endif

`ifdef def_true
	tFraShr1A_S		= tFraA1;
	tFraShr1A_Shr	= { tExpB1D[9:7]!=0, tExpB1D[6:0] };

	tFraShr1B_S		= tFraB1;
	tFraShr1B_Shr	= { tExpA1D[9:7]!=0, tExpA1D[6:0] };
`endif


//	if(tExpA1D[11] || tFraDti1)
//	if(tExpA1D[11] || tFraDti1 || tFraItf1)
	if(tExpA1D[11] || tFraDti1 || tFraItf1 || !tFraUseB)
	begin
		tFraUseA		= 0;
		tFraShr1_S		= tFraA1;
		tFraShr1_Shr	= { tExpB1D[9:7]!=0, tExpB1D[6:0] };
	end else begin
		tFraUseA		= 1;
		tFraShr1_S		= tFraB1;
		tFraShr1_Shr	= { tExpA1D[9:7]!=0, tExpA1D[6:0] };
	end

	/* Stage 2 */

	tFraC2_S = tFraA2;
	tFraC2_T = ((tSgnA2 == tSgnB2) ^ tFraB2[63]) ? tFraB2 : (~tFraB2);
	tFraC2_Cin = (tSgnA2 != tSgnB2) ^ tFraB2[63];

	tSgnC2	= tSgnA2 ^ tFraB2[63];
	tExpC2	= tExpA2;
	tFraC2	= tFraC2_C[63:0];

`ifndef def_true
	if(tFraC2_C[63])
	begin
//		$display("FpuAdd: S2 Flip Result");
		tSgnC2	= !tSgnA2;
		tFraC2	= ~tFraC2_C[63:0];
	end
`endif

	tFraC2I = tFraC2_C[63: 0];
	
	
	/* Stage 3 */

//	tFraC3 = tFraC3A;
	tSgnC3B	= tSgnC3;

//	tSgnC3B	= tSgnC3 ^ tFraC3A[63];
//	if(tFraC3A[63])
//		tFraC3 = ~tFraC3A;

	tFraClz1_S = { tFraC3[61:0], 2'b0 };

	if(tFraC3[63] && tExEn3)
	begin
		$display("FpuAdd: S3 Fraction is Negative");
	end

	if(tFraC3[62])
	begin
		tExpC3B	= {1'b0, tExpC3} + 1;
		tFraC3B	= { 1'b0, tFraC3[63:1] };
		tExpIsZeroC3 = 0;
		tFraShlC3B	 = 0;

		if(tExpC3B[11])
		begin
//			if(tExEn3)
//				$display("FpuAdd: S3 Overflow");
			tExpIsZeroC3	= 1;
			tExpC3B			= 2047;
		end

	end
	else
	begin
		tExpC3B	= {1'b0, tExpC3} - { 5'h0, tFraClz1_C[6:0] };
		tFraC3B	= tFraC3;
		tExpIsZeroC3 = tFraClz1_C[  7] || tExpC3B[11];
		tFraShlC3B	 = tFraClz1_C[6:0];

		if(tExpC3B[11])
		begin
			if(tExEn3)
			begin
//				$display("FpuAdd: S3 Exp Went Negative, %d %d",
//					tExpC3, tFraClz1_C);
			end
			tExpIsZeroC3 = 1;
		end

		if(tExpIsZeroC3)
		begin
			if(tExEn3)
			begin
//				$display("FpuAdd: S3 Is Zero");
			end
			tSgnC3B	= 0;
			tExpC3B	= 0;
		end
	end

	/* Stage 4 */

	tFraShl1_S		= tFraC4;
	tFraShl1_Shl	= {
		tExpIsZeroC4,
		tFraShlC4 };

	tSgnC4B			= tSgnC4;
	tFraC4B			= tFraShl1_C;
	tExpC4B			= tExpC4;
	tInxC4B			= tInxC4 || (tFraC4B[10:0] != 0);
	tFraRbit4B		= tFraC4B[8];
	tFraRbit4B2		= tFraC4B[10];

	tValC4D		= { tSgnC4B, tExpC4B[10:0], tFraC4B[60:9] };

	if(regRMode[3:0]==1)
		tFraRbit4B=0;
	if(regRMode[3:0]==2)
		tFraRbit4B=!tSgnC4;
	if(regRMode[3:0]==3)
		tFraRbit4B=tSgnC4;
	if(regRMode[3:0]!=4)
		tFraRbit4B2=0;

	tValRoundC4 = { 1'b0, tValC4D[3:0] } + {
		1'b0, tFraRbit4B2,
		1'b0, tFraRbit4B };
	
//	tValRoundC4 = { 1'b0, tValC4[3:0] } + 1;
//	if(tFraC4B[8] && !tValRoundC4[4])
	if(!tValRoundC4[4])
		tValC4D[3:0] = tValRoundC4[3:0];

	if(regRMode[3:0]==4)
		tValC4D[1:0] = tInxC4B ? 2'b01 : 2'b00;

	tOutLd4 = tRegExOp4[3] && !tFraEtd4;

	if(tRegExOp4[2:0] == 4)
	begin
//		$display("FADD: D2I-4 %X", tFraC4I);
		tValC4D = tFraC4I;
	end

	tValC4Lo	= tOutLd4 ? tValC4E[63:0] : tValC4D;
	tValC4Hi	= tValC4E[127:64];
end

always @(posedge clock)
begin
	tRegValRoLo2	<= tRegValRoLo;
	tRegValRoHi2	<= tRegValRoHi;
	tRegExOK2		<= tRegExOK;

//	if(!exHold && tExEn1)
	if(!exHold)
	begin
		if(tFraUseA)
		begin
			tSgnA2 <= tSgnA1;	tSgnB2 <= tSgnB1;
			tExpA2 <= tExpA1;	tExpB2 <= tExpB1;
			tFraA2 <= tFraA1;
//			tFraB2 <= tFraShr1_C;
			tFraB2 <= tFraShr1B_C;
		end
		else
		begin
			tSgnA2 <= tSgnB1;	tSgnB2 <= tSgnA1;
			tExpA2 <= tExpB1;	tExpB2 <= tExpA1;
			tFraA2 <= tFraB1;
//			tFraB2 <= tFraShr1_C;
			tFraB2 <= tFraShr1A_C;
		end
		
		tRegExOp2	<= tRegExOp1;
//		tFraJ2		<= tFraJ1;
//		tFraJ2Sg	<= tFraJ1Sg;
		tExEn2		<= tExEn1;
		tRegValRn2	<= tRegValRn1;

		tFraDti2	<= tFraDti1;
		tFraItf2	<= tFraItf1;
		tInxC2		<= tInxC1;

		tFraDte2	<= tFraDte1;
		tFraEtd2	<= tFraEtd1;
	end

	tRegExOp3	<= tRegExOp2;
//	tSgnC3		<= tSgnC2;
	tSgnC3A		<= tSgnC2;
	tExpC3		<= tExpC2;
//	tFraC3		<= tFraC2;
	tFraC3A		<= tFraC2;
	tFraC3I		<= tFraC2I;
	tInxC3		<= tInxC2;

	tRegExOp4	<= tRegExOp3;
	tSgnC4		<= tSgnC3B;
	tExpC4		<= tExpC3B;
	tFraC4		<= tFraC3B;
	tFraC4I		<= tFraC3I;
	tInxC4		<= tInxC3;

	tExpIsZeroC4	<= tExpIsZeroC3;
	tFraShlC4		<= tFraShlC3B;
	
	tExEn3		<= tExEn2;
	tExEn4		<= tExEn3;
	tExEn5		<= tExEn4;

//	tRegExOp3	<= tRegExOp2;
//	tRegExOp4	<= tRegExOp3;
//	tRegExOp5	<= tRegExOp4;

	tFraDte3	<= tFraDte2;
	tFraEtd3	<= tFraEtd2;
	tFraDte4	<= tFraDte3;
	tFraEtd4	<= tFraEtd3;

	tRegValRoLo	<= tValC4Lo;
	tRegValRoHi	<= tValC4Hi;
end

endmodule

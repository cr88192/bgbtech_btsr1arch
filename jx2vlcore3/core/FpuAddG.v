/*
FPU Add (Extended Precision)

Uses S.15.80 as its nominal format.

Accepts and produces either a Binary64 or Binary128.
Binary128 is partial-precision.


Adds two input values, producing an output value.
ExOp: Is set to indicate that a floating-point op is requested.
ExOK: Set to indicate whether the operation has completed.

ExOp(2:0):
	0: No Op
	1: Ro=Rn+Rm
	2: Ro=Rn-Rm
	3: Ro=I2F(Rn), Int->Float
	4: Ro=F2I(Rn), Float->Int
	5: Ro=D2E / E2D
		(LD=0): Double->LongDouble
		(LD=1): LongDouble->Double
	6: ?
	7: Ro=Rn*Rm
ExOp(3): LD
	0: Double
	1: Long Double
*/

`include "CoreDefs.v"

`include "ExCsAdd90C.v"
`include "ExCsShr90F.v"
`include "ExCsClz88F.v"
`include "ExCsShl90F.v"

module FpuAddG(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRmLo,	regValRmHi,
	regValRnLo,	regValRnHi,
	regValRoLo,	regValRoHi,
	regExOp,
	regExOK
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

reg[63:0]		tRegValRoLo2;
reg[63:0]		tRegValRoHi2;
reg[1:0]		tRegExOK2;

assign	regValRoLo	= tRegValRoLo2;
assign	regValRoHi	= tRegValRoHi2;
assign	regExOK		= tRegExOK2;

reg[63:0]		tRegValRoLo;
reg[63:0]		tRegValRoHi;
reg[1:0]		tRegExOK;

// reg[95:0]		tRegValRmE;
// reg[95:0]		tRegValRnE;
reg[127:0]		tRegValRmE;
reg[127:0]		tRegValRnE;

reg[3:0]		tRegExOp1;
reg				tExEn1;
reg				tSgnA1;
reg				tSgnB1;
reg[14:0]		tExpA1;
reg[14:0]		tExpB1;
reg[89:0]		tFraA1;
reg[89:0]		tFraB1;

reg[63:0]		tRegValRn1;
reg[63:0]		tRegValRni1;

reg[15:0]		tExpA1D;
reg[15:0]		tExpB1D;
reg[89:0]		tFraA1D;
reg[89:0]		tFraB1D;


reg[3:0]		tRegExOp2;
reg				tExEn2;
reg				tSgnA2;
reg				tSgnB2;
reg[14:0]		tExpA2;
reg[14:0]		tExpB2;
reg[89:0]		tFraA2;
reg[89:0]		tFraB2;

reg[89:0]		tFraC2_S;
reg[89:0]		tFraC2_T;
reg				tFraC2_Cin;
wire[90:0]		tFraC2_C;
ExCsAdd90C	tFraAdd(tFraC2_S, tFraC2_T, tFraC2_C, tFraC2_Cin);

reg[89:0]		tFraShr1_S;
wire[89:0]		tFraShr1_C;
reg[7:0]		tFraShr1_Shr;
ExCsShr90F	tFraShr(tFraShr1_S, tFraShr1_C, tFraShr1_Shr);

reg[87:0]		tFraClz1_S;
wire[7:0]		tFraClz1_C;
ExCsClz88F	tFraClz(tFraClz1_S, tFraClz1_C);

reg[89:0]		tFraShl1_S;
wire[89:0]		tFraShl1_C;
reg[7:0]		tFraShl1_Shl;
ExCsShl90F	tFraShl(tFraShl1_S, tFraShl1_C, tFraShl1_Shl);

reg[63:0]		tRegValRn2;
// reg[63:0]		tRegValRni2;

reg				tSgnC2;
reg[14:0]		tExpC2;
reg[89:0]		tFraC2;
reg[63:0]		tFraC2I;

reg[3:0]		tRegExOp3;
reg				tExEn3;
reg				tSgnC3;
reg[14:0]		tExpC3;
reg[89:0]		tFraC3;
reg				tSgnC3B;

reg[16:0]		tExpC3A;
reg[89:0]		tFraC3A;
reg[16:0]		tExpC3B;
reg[89:0]		tFraC3B;
reg[63:0]		tFraC3I;
reg[6:0]		tExpAdjC3A;
reg[6:0]		tExpAdjC3B;
reg				tExpIsZeroC3;
reg[6:0]		tFraShlC3B;

reg[3:0]		tRegExOp4;
reg				tExEn4;
reg				tSgnC4;
reg[16:0]		tExpC4;
reg[89:0]		tFraC4;
reg[63:0]		tFraC4I;
reg				tExpIsZeroC4;
reg				tExpIsZeroC4B;
reg[6:0]		tFraShlC4;

reg[16:0]		tExpC4A;
reg[89:0]		tFraC4A;
reg[6:0]		tExpAdjC4A;

reg				tSgnC4B;
reg[16:0]		tExpC4B;
reg[89:0]		tFraC4B;
reg[6:0]		tExpAdjC4B;

// reg[95:0]		tValC4;
reg[127:0]		tValC4E;
reg[63:0]		tValC4D;
reg[63:0]		tValC4Lo;
reg[63:0]		tValC4Hi;

reg[8:0]		tValC4_Ru;

reg[3:0]		tRegExOp5;
reg				tExEn5;

reg				tExpRmIsZero;
reg				tExpRnIsZero;
reg				tExpRmIsNaN;
reg				tExpRnIsNaN;
reg[14:0]		tExpRmExt;
reg[14:0]		tExpRnExt;
reg[10:0]		tExpC4D;

reg				tFraDti1;
reg				tFraItf1;
reg				tOutLd4;
reg				tFraUseA;

reg				tFraDti2;
reg				tFraItf2;

reg				tFraDte1;
reg				tFraEtd1;
reg				tFraDte2;
reg				tFraEtd2;
reg				tFraDte3;
reg				tFraEtd3;
reg				tFraDte4;
reg				tFraEtd4;

reg				tInfC4B;
reg				tZeroC4B;
reg				tInfC4D;
reg				tZeroC4D;
reg				tInfC4;
reg				tZeroC4;

reg				tInfC3B;
reg				tZeroC3B;

always @*
begin
	tRegExOp1	= regExOp;
//	tExEn1		= regExOp != 0;
	tExEn1		= regExOp[2:0] != 0;
//	tRegValRn1	= regValRn;
	tRegValRn1	= regValRnLo;
//	tRegValRni1	= ~regValRnLo;
//	tRegValRni1	= regValRnLo ^ (regValRnLo[63] ? UV64_FF : UV64_00);
//	tRegValRni1	= regValRnLo[63] ? ~regValRnLo : regValRnLo;

	tRegExOK=UMEM_OK_READY;
	
	/* Stage 1 */
	tRegValRmE		= { regValRmHi, regValRmLo };
	tRegValRnE		= { regValRnHi, regValRnLo };

	tExpRmIsZero	= (regValRmLo[62:52]==11'h000);
	tExpRnIsZero	= (regValRnLo[62:52]==11'h000);
	tExpRmIsNaN		= (regValRmLo[62:52]==11'h7FF);
	tExpRnIsNaN		= (regValRnLo[62:52]==11'h7FF);

	tExpRmExt		= {
		regValRmLo[62],
		((!regValRmLo[62] && !tExpRmIsZero) ||
			tExpRmIsNaN) ? 4'b1111 : 4'b0000,
		regValRmLo[61:52]
		};
	tExpRnExt		= {
		regValRnLo[62],
		((!regValRnLo[62] && !tExpRnIsZero) ||
			tExpRnIsNaN) ? 4'b1111 : 4'b0000,
		regValRnLo[61:52]
		};

	tFraDte1	= (tRegExOp1[2:0] == 5) && !tRegExOp1[3];
	tFraEtd1	= (tRegExOp1[2:0] == 5) &&  tRegExOp1[3];

	if(tRegExOp1[3])
//	if(tRegExOp1[3] || tFraEtd1)
	begin
		tSgnA1	= tRegValRnE[127];
		tSgnB1	= tRegValRmE[127] ^ tRegExOp1[1];
		tExpA1	= tRegValRnE[126:112];
		tExpB1	= tRegValRmE[126:112];
		tFraA1	= { 2'b0, (tExpA1!=0), tRegValRnE[111:25] };
		tFraB1	= { 2'b0, (tExpB1!=0), tRegValRmE[111:25] };
	end
	else
	begin
		tSgnA1	= regValRnLo[63];
		tSgnB1	= regValRmLo[63] ^ tRegExOp1[1];
		tExpA1	= tExpRnExt;
		tExpB1	= tExpRmExt;
		tFraA1	= {2'b0, (tExpA1!=0), regValRnLo[51:0], 35'h0};
		tFraB1	= {2'b0, (tExpB1!=0), regValRmLo[51:0], 35'h0};
	end

	tFraDti1	= (tRegExOp1[2:0] == 4);
	tFraItf1	= (tRegExOp1[2:0] == 3);

//	if(tFraDti1)
	if(tFraDti1 || tFraItf1)
	begin
		tSgnB1	= 0;
		tExpB1	= 16470;
		tFraB1	= 0;
	end
	
	if(tFraItf1)
	begin
		tSgnA1	= 0;
		tExpA1	= 16470;
		tFraA1 = {
			regValRnLo[63] ? UV26_FF : UV26_00,
			tRegValRn1[63:0] };
	end

	tExpA1D	= {1'b0, tExpA1} - {1'b0, tExpB1};
	tExpB1D	= {1'b0, tExpB1} - {1'b0, tExpA1};

//	if(tExpA1D[15])
//	if(tExpA1D[15] || tFraDti1)
	if(tExpA1D[15] || tFraDti1 || tFraItf1)
//	if(tExpA1D[15] || tFraDti1 || tFraItf1 ||
//		(!tExpB1D[15] && (tSgnA1!=tSgnB1)))
	begin
		tFraUseA		= 0;
		tFraShr1_S		= tFraA1;
		tFraShr1_Shr	= { tExpB1D[14:7]!=0, tExpB1D[6:0] };
	end else begin
		tFraUseA		= 1;
		tFraShr1_S		= tFraB1;
		tFraShr1_Shr	= { tExpA1D[14:7]!=0, tExpA1D[6:0] };
	end

	/* Stage 2 */

	tFraC2_S = tFraA2;
	tFraC2_T = ((tSgnA2 == tSgnB2) ^ tFraB2[89]) ? tFraB2 : (~tFraB2);
	tFraC2_Cin = (tSgnA2 != tSgnB2) ^ tFraB2[89];

	tSgnC2	= tSgnA2 ^ tFraB2[89];
	tExpC2	= tExpA2;
	tFraC2	= tFraC2_C[89:0];

	if(tFraC2_C[89])
	begin
		tSgnC2	= !tSgnA2;
		tFraC2	= ~tFraC2_C[89:0];
	end
	
	tFraC2I = tFraC2_C[63: 0];
	
	/* Stage 3 */
	tSgnC3B	= tSgnC3;
	tFraClz1_S = tFraC3[87:0];

	if(tFraC3[88])
	begin
		tExpC3B	= {2'b0, tExpC3} + 1;
		tFraC3B	= tFraC3 >> 1;
		tExpIsZeroC3 = 0;
		tFraShlC3B	 = 0;

		tInfC3B		= tExpC3B[15] || ( tExpC3B[14] && tExpC3B[13:10]!=4'b0000);
		if(!tRegExOp3[3] && tInfC3B)
		begin
			tExpIsZeroC3	= 1;
			tExpC3B			= 32767;
		end

	end
	else
	begin
		tExpC3B	= {2'b0, tExpC3} - { 10'h0, tFraClz1_C[6:0] };
		tFraC3B	= tFraC3;
		tExpIsZeroC3 = tFraClz1_C[  7] || tExpC3B[16];
		tFraShlC3B	 = tFraClz1_C[6:0];

		tZeroC3B	= tExpC3B[16]	|| (!tExpC3B[14] && tExpC3B[13:10]!=4'b1111);
		if(!tRegExOp3[3] && tZeroC3B)
			tExpIsZeroC3 = 1;

		if(tExpIsZeroC3)
		begin
			tSgnC3B	= 0;
			tExpC3B	= 0;
		end
	end
	
	/* Stage 4 */

//	tInfC4B		= tExpC4[15]	|| (tExpC4==32767);
//	tInfC4D		= tInfC4B		|| ( tExpC4[14] && tExpC4[13:10]!=4'b0000);
//	tInfC4		= tRegExOp4[3] ? tInfC4B  : tInfC4D;

	tFraShl1_S		= tFraC4;
	tFraShl1_Shl	= {
		tExpIsZeroC4,
		tFraShlC4 };

	tSgnC4B			= tSgnC4;
	tFraC4B			= tFraShl1_C;
	tExpC4B			= tExpC4;	


	tExpC4D = { tExpC4B[14], tExpC4B[9:0] };

	tValC4E	= { tSgnC4B, tExpC4B[14:0], tFraC4B[86: 0], 25'h0 };
	tValC4D	= { tSgnC4B, tExpC4D[10:0], tFraC4B[86:35] };

	tValC4_Ru = { 1'b0, tValC4D[7:0] } + 1;
	if(tFraC4B[34] && !tValC4_Ru[8])
		tValC4D[7:0] = tValC4_Ru[7:0];

//	tOutLd4 = tRegExOp4[3];
	tOutLd4 = tRegExOp4[3] && !tFraEtd4;
	
	if(tRegExOp4[2:0] == 4)
	begin
		tValC4D = tFraC4I;
		tOutLd4 = 0;
	end

	tValC4Lo	= tOutLd4 ? tValC4E[63:0] : tValC4D;
	tValC4Hi	= tValC4E[127:64];
	
end

always @(posedge clock)
begin
	tRegValRoLo2	<= tRegValRoLo;
	tRegValRoHi2	<= tRegValRoHi;
	tRegExOK2		<= tRegExOK;

	if(tExEn1 && !exHold)
	begin
		if(tFraUseA)
		begin
			tSgnA2 <= tSgnA1;	tSgnB2 <= tSgnB1;
			tExpA2 <= tExpA1;	tExpB2 <= tExpB1;
			tFraA2 <= tFraA1;	tFraB2 <= tFraShr1_C;
		end
		else
		begin
			tSgnA2 <= tSgnB1;	tSgnB2 <= tSgnA1;
			tExpA2 <= tExpB1;	tExpB2 <= tExpA1;
			tFraA2 <= tFraB1;	tFraB2 <= tFraShr1_C;
		end
		
		tRegExOp2	<= tRegExOp1;
		tExEn2		<= tExEn1;
		tRegValRn2	<= tRegValRn1;
//		tRegValRni2	<= tRegValRni1;
		tFraDti2	<= tFraDti1;
		tFraItf2	<= tFraItf1;

		tFraDte2	<= tFraDte1;
		tFraEtd2	<= tFraEtd1;
	end
	else if(!exHold)
	begin
		tExEn2		<= 0;
	end

//	if(tExEn2)
	if(1'b1)
	begin
		tSgnC3		<= tSgnC2;
		tExpC3		<= tExpC2;
		tFraC3		<= tFraC2;
		tFraC3I		<= tFraC2I;

		tSgnC4		<= tSgnC3B;
		tExpC4		<= tExpC3B;
		tFraC4		<= tFraC3B;
		tFraC4I		<= tFraC3I;
		
		tExpIsZeroC4	<= tExpIsZeroC3;
		tFraShlC4		<= tFraShlC3B;
		
		tExEn3		<= tExEn2;
		tExEn4		<= tExEn3;
		tExEn5		<= tExEn4;

		tRegExOp3	<= tRegExOp2;
		tRegExOp4	<= tRegExOp3;
		tRegExOp5	<= tRegExOp4;

		tFraDte3	<= tFraDte2;
		tFraEtd3	<= tFraEtd2;
		tFraDte4	<= tFraDte3;
		tFraEtd4	<= tFraEtd3;

		tRegValRoLo	<= tValC4Lo;
		tRegValRoHi	<= tValC4Hi;
	end
	else
	begin
		tExEn3		<= 0;
		tExEn4		<= 0;
		tExEn5		<= 0;
		tRegValRoLo	<= UV64_00;
		tRegValRoHi	<= UV64_00;
	end
end

endmodule

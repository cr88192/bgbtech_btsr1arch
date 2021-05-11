/*
64-bit SHAD/SHLD Unit
Extend to also support 32-bit SHAD/SHLD.
This unit implements a 128-bit Funnel Shift.
 */

`include "CoreDefs.v"

`ifndef HAS_JX2EXSHADQ_D
`define HAS_JX2EXSHADQ_D

module ExShad64D(
	/* verilator lint_off UNUSED */
	clock, reset,
	valRs, valRx,
	valRt,
	valRn, shOpA,
	idLane
	);

input	clock;
input	reset;

// /* verilator lint_off UNOPTFLAT */

input[63:0]		valRs;
input[63:0]		valRx;
input[ 7:0]		valRt;
input[ 5:0]		shOpA;
output[63:0]	valRn;
input[1:0]		idLane;

wire			isLaneA;
wire			isLaneB;
wire			isLaneC;
assign		isLaneA = (idLane[1:0]==0);
assign		isLaneB = idLane[0];
assign		isLaneC = idLane[1];

wire			shOpU;
wire			shOpQ;
wire			shOpR;
wire			shOpO;
wire			shOpX;
wire			shOpL;
assign		shOpU = shOpA[0];	//Zero Extend
assign		shOpQ = shOpA[1];	//QuadWord
assign		shOpR = shOpA[2];	//Shift-Right
assign		shOpO = shOpA[3];	//Rotate
// assign		shOpO = shOpA[3] && !isLaneC;	//Rotate
assign		shOpX = shOpA[5] && !isLaneC;	//Funnel

assign		shOpL = !(shOpQ || shOpX);

reg[63:0]		tValRn;
assign			valRn = tValRn;
// /* verilator lint_on UNOPTFLAT */

reg[63:0]		tValRs;

reg[63:0]		tValRol;
reg[63:0]		tValRolX;
reg[63:0]		tValRor;
reg[63:0]		tValRorX;
reg[ 7:0]		tValSh;
reg[ 7:0]		tValShN;
reg[ 7:0]		tValShR;

reg[191:0]		tValSht128;
reg[127:0]		tValSht64;
reg[ 95:0]		tValSht32;
reg[ 79:0]		tValSht16;
reg[ 71:0]		tValSht8;
reg[ 67:0]		tValSht4;
reg[ 65:0]		tValSht2;
reg[ 64:0]		tValSht1;

//reg[79:0]		tValSht8;
//reg[79:0]		tValSht4;
//reg[79:0]		tValSht2;
//reg[79:0]		tValSht1;

reg				tValSgn;
reg				tValSgnX;

always @*
begin

	tValRol		= 0;
//	tValRolX	= UV64_XX;
	tValRolX	= valRs;
	tValRor		= 0;
//	tValRorX	= UV64_XX;
	tValRorX	= valRs;
	tValRn		= 0;
	tValSh		= valRt[7:0];
	if(!shOpX)	tValSh[6] = tValSh[7];
	if( shOpL)	tValSh[5] = tValSh[7];
	tValShN		= shOpR ? tValSh : (-tValSh);
	tValRs		= valRs;

	tValSgn		= valRs[63];
	tValSgnX	= (isLaneB ? valRx[63] : valRs[63]);

// `ifndef def_true
`ifdef def_true
//	if(shOpX)
	if(shOpX && !isLaneC)
	begin
		if(shOpO)
		begin
			tValRorX	= valRs;
			tValRolX	= valRs;
		end
		else if(!shOpU)
		begin
			tValRorX	= tValSgnX ? UV64_FF : UV64_00;
			tValRolX	= UV64_00;
		end

		tValRor		= isLaneB ? valRx : tValRorX;
		tValRol		= isLaneB ?     0 : valRx   ;
	end
	else if(shOpQ)
	begin
//		if(shOpO)
		if(shOpO && isLaneA)
		begin
			tValRor		= valRs;
			tValRol		= valRs;
		end
		else if(!shOpU)
		begin
//			tValRorX	= valRs[63] ? UV64_FF : UV64_00;
//			tValRor		= tValRorX;
			tValRor		= valRs[63] ? UV64_FF : UV64_00;
		end
//		tValShN[6]	= tValShN[7];
	end
	else
	begin
//		if(shOpO)
		if(shOpO && isLaneA)
		begin
			tValRs			= { valRs[31:0], valRs[31:0] };
			tValRor			= tValRs;
			tValRol			= tValRs;
		end
		else if(!shOpU)
		begin
			tValRor		= valRs[31] ? UV64_FF : UV64_00;
//			tValRorX	= valRs[31] ? UV64_FF : UV64_00;
//			tValRor		= tValRorX;
		end
		tValRs			= { tValRor[31:0], valRs[31:0] };
//		tValShN[6:5]	= { tValShN[7], tValShN[7] };
	end
`endif

`ifdef def_true
	tValShR		= tValShN;
	if(tValShN[7])
		tValSht128	= { tValRs, tValRol, tValRolX };
	else
		tValSht128	= { tValRorX, tValRor, tValRs };

	tValSht64	= tValShR[6] ? tValSht128[191:64] : tValSht128[127:0];
	tValSht32	= tValShR[5] ? tValSht64 [127:32] : tValSht64 [ 95:0];
	tValSht16	= tValShR[4] ? tValSht32 [ 95:16] : tValSht32 [ 79:0];
	tValSht8	= tValShR[3] ? tValSht16 [ 79: 8] : tValSht16 [ 71:0];
	tValSht4	= tValShR[2] ? tValSht8  [ 71: 4] : tValSht8  [ 67:0];
	tValSht2	= tValShR[1] ? tValSht4  [ 67: 2] : tValSht4  [ 65:0];
	tValSht1	= tValShR[0] ? tValSht2  [ 65: 1] : tValSht2  [ 64:0];
	tValRn		= tValSht1[63:0];
`endif

	if(shOpL)
	begin
		tValRn[63:32]=(tValRn[31] && !shOpU) ? UV32_FF : UV32_00 ;
	end

end

endmodule

`endif

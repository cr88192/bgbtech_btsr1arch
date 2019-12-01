/*
Half-Float Vector Operation

00=None
01=ADD
10=SUB
11=MUL
*/

`include "Jx2CoreDefs.v"

`include "Jx2FphfAdd.v"
`include "Jx2FphfMul.v"

module Jx2FphfVecOp(
	clock,		reset,
	regValRs,
	regValRt,
	regValRo,
	regExOp,
	regExOK
	);

input	clock;
input	reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
output[63:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[63:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

assign	regValRo	= tRegValRo2;
assign	regExOK		= tRegExOK2;

reg[63:0]		tRegValRo;
reg[1:0]		tRegExOK;


reg[63:0]	tVecRs;
reg[63:0]	tVecRt;
reg[63:0]	tVecRo;
reg[63:0]	tNextVecRs;
reg[63:0]	tNextVecRt;
reg[63:0]	tNextVecRo;

reg[3:0]	tVecRmsk;
reg[3:0]	tNextVecRmsk;

reg[63:0]	tHeldRo;
reg[63:0]	tNextHeldRo;

reg[1:0]	tHfopExOp;
reg[15:0]	tHfopValRs;
reg[15:0]	tHfopValRt;
wire[15:0]	tHfopAddRo;
wire[15:0]	tHfopMulRo;
wire[1:0]	tHfopAddOK;
wire[1:0]	tHfopMulOK;

reg[15:0]	tHfopRo;
reg[1:0]	tHfopOK;

Jx2FphfAdd hfAdd(clock, reset,
	tHfopValRs, tHfopValRt, tHfopAddRo,
	tHfopExOp, tHfopAddOK);

Jx2FphfMul hfMul(clock, reset,
	tHfopValRs, tHfopValRt, tHfopMulRo,
	tHfopExOp, tHfopMulOK);

reg tIsDone;
reg tNextIsDone;
reg tSeenOK;
reg tNextSeenOK;

reg tIsStart;
reg tNextIsStart;

reg tIsOpMul;

always @*
begin
	tNextVecRs		= tVecRs;
	tNextVecRt		= tVecRt;
	tNextVecRo		= tVecRo;
	tNextVecRmsk	= tVecRmsk;
	tNextHeldRo		= tHeldRo;

	tHfopValRs		= tVecRs[15:0];
	tHfopValRt		= tVecRt[15:0];
//	tHfopExOp		= regExOp;
	tHfopExOp		= 0;
	
	tNextIsDone		= tIsDone;
	tNextSeenOK		= tSeenOK;
	tNextIsStart	= tIsStart;
	tRegExOK		= UMEM_OK_READY;
	
	tIsOpMul		= (regExOp == 2'h3);
	tHfopRo			= tIsOpMul ? tHfopMulRo : tHfopAddRo;
	tHfopOK			= tIsOpMul ? tHfopMulOK : tHfopAddOK;
	
	tRegValRo		= tHeldRo;
	
	if(regExOp!=0)
	begin
		tRegExOK	= tIsDone ? UMEM_OK_OK : UMEM_OK_HOLD;
		
		if(tIsStart)
		begin
			if(tVecRmsk[0])
			begin
				tHfopExOp		= regExOp;
				tNextVecRs		= { UV16_XX, tVecRs[63:16] };
				tNextVecRt		= { UV16_XX, tVecRt[63:16] };
				tNextVecRmsk	= { 1'b0, tVecRmsk[3:1] };
			end
		end
		else
		begin
			tNextIsStart	= 1;
			tNextVecRs		= regValRs;
			tNextVecRt		= regValRt;
			tNextVecRmsk	= 4'hF;
		end
		
		if(tHfopOK==UMEM_OK_OK)
		begin
			tNextVecRo		= { tHfopRo, tVecRo[63:16] };
			tNextSeenOK = 1;
		end
		else if(tHfopOK==UMEM_OK_READY)
		begin
			if(tSeenOK)
			begin
				if(!tIsDone)
				begin
					tNextHeldRo = tVecRo;
				end
				tNextIsDone = 1;
			end
		end
	end
	else
	begin
		tNextIsDone		= 0;
		tNextSeenOK		= 0;
		tNextIsStart	= 0;
		tNextVecRmsk	= 0;
		tRegExOK		= tIsDone ? UMEM_OK_OK : UMEM_OK_READY;
	end
end

always @(posedge clock)
begin
	tRegValRo2	<= tRegValRo;
	tRegExOK2	<= tRegExOK;

	tVecRs		<= tNextVecRs;
	tVecRt		<= tNextVecRt;
	tVecRo		<= tNextVecRo;
	tVecRmsk	<= tNextVecRmsk;
	tHeldRo		<= tNextHeldRo;

	tIsDone		<= tNextIsDone;
	tSeenOK		<= tNextSeenOK;
	tIsStart	<= tNextIsStart;
end

endmodule

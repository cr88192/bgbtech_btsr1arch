/*

Load/Store Queue
	
If (exIdRn==IMM), the access passes through directly.
This would be used for atypical memory access (going somewhere other than a GPR).
	
Idea is that this will allow buffering up to four memory accesses to allow the average case memory access to be performed in fewer clock cycles.

Note: Doesn't seem to work correctly, still needs more debugging.

*/

module Jx2ExLsq(
	clock,		reset,

	exAddr,		exIdRn,
	exInData,	exOutData,
	exOpm,		exDataOK,

	memAddr,
	memInData,	memOutData,
	memOpm,		memDataOK,

	gprIdRm,	gprIdRn,
	gprIdRi,

	regOutId,	regOutVal,
	regOutOK
	);

input			clock;
input			reset;

input[6:0]		exIdRn;			//register to load to
input[63:0]		exAddr;			//memory address
output[63:0]	exInData;		//memory data (load)
input[63:0]		exOutData;		//memory data (store)
input[4:0]		exOpm;			//mem op mode
output[1:0]		exDataOK;		//memory status

output[63:0]	memAddr;		//memory address
input[63:0]		memInData;		//memory data (load)
output[63:0]	memOutData;		//memory data (store)
output[4:0]		memOpm;			//mem op mode
input[1:0]		memDataOK;		//memory status

input[6:0]		gprIdRm;		//checked register
input[6:0]		gprIdRn;		//checked register
input[6:0]		gprIdRi;		//checked register

output[63:0]	regOutVal;		//Output register value
output[6:0]		regOutId;		//Output register id
output[1:0]		regOutOK;		//execute status


reg[63:0]		tExInData2;		//memory data (load)
reg[1:0]		tExDataOK2;		//memory status
reg[63:0]		tMemAddr;		//memory address
reg[63:0]		tMemOutData;	//memory data (store)
reg[4:0]		tMemOpm;		//mem op mode
reg[63:0]		tRegOutVal;		//Output register value
reg[6:0]		tRegOutId;		//Output register id
reg[1:0]		tRegOutOK;		//execute status

assign		exInData	= tExInData2;
assign		exDataOK	= tExDataOK2;
assign		memAddr		= tMemAddr;
assign		memOutData	= tMemOutData;
assign		memOpm		= tMemOpm;
assign		regOutVal	= tRegOutVal;
assign		regOutId	= tRegOutId;
assign		regOutOK	= tRegOutOK;

reg[63:0]		tExInData;		//memory data (load)
reg[1:0]		tExDataOK;		//memory status


reg[6:0]	lsqIdRn0;
reg[6:0]	lsqIdRn1;
reg[6:0]	lsqIdRn2;
reg[6:0]	lsqIdRn3;

reg[47:0]	lsqAddr0;
reg[47:0]	lsqAddr1;
reg[47:0]	lsqAddr2;
reg[47:0]	lsqAddr3;

reg[63:0]	lsqOutData0;
reg[63:0]	lsqOutData1;
reg[63:0]	lsqOutData2;
reg[63:0]	lsqOutData3;

reg[4:0]	lsqOpm0;
reg[4:0]	lsqOpm1;
reg[4:0]	lsqOpm2;
reg[4:0]	lsqOpm3;


reg[6:0]	lsqNextIdRn0;
reg[6:0]	lsqNextIdRn1;
reg[6:0]	lsqNextIdRn2;
reg[6:0]	lsqNextIdRn3;

reg[47:0]	lsqNextAddr0;
reg[47:0]	lsqNextAddr1;
reg[47:0]	lsqNextAddr2;
reg[47:0]	lsqNextAddr3;

reg[63:0]	lsqNextOutData0;
reg[63:0]	lsqNextOutData1;
reg[63:0]	lsqNextOutData2;
reg[63:0]	lsqNextOutData3;

reg[4:0]	lsqNextOpm0;
reg[4:0]	lsqNextOpm1;
reg[4:0]	lsqNextOpm2;
reg[4:0]	lsqNextOpm3;

reg			tRegOkLatch;
reg			tNextOkLatch;
reg			tJointOkLatch;

reg			tRegOpmLatch;
reg			tNextOpmLatch;

reg			tLsqHold;

always @*
begin
	tRegOutOK	= UMEM_OK_OK;
	tLsqHold	= 0;
	tExDataOK	= UMEM_OK_READY;

	tNextOkLatch	= (memDataOK!=UMEM_OK_OK);
	tJointOkLatch	= tNextOkLatch || tRegOkLatch;

	tRegOutVal	= UV64_XX;
	tRegOutId	= JX2_REG_ZZR;

	if(gprIdRm!=JX2_REG_ZZR)
	begin
		if(	(gprIdRm==lsqIdRn0) ||
			(gprIdRm==lsqIdRn1) ||
			(gprIdRm==lsqIdRn2) ||
			(gprIdRm==lsqIdRn3))
				tRegOutOK = UMEM_OK_HOLD;
	end
	if(gprIdRn!=JX2_REG_ZZR)
	begin
		if(	(gprIdRn==lsqIdRn0) ||
			(gprIdRn==lsqIdRn1) ||
			(gprIdRn==lsqIdRn2) ||
			(gprIdRn==lsqIdRn3))
				tRegOutOK = UMEM_OK_HOLD;
	end
	if(gprIdRi!=JX2_REG_ZZR)
	begin
		if(	(gprIdRi==lsqIdRn0) ||
			(gprIdRi==lsqIdRn1) ||
			(gprIdRi==lsqIdRn2) ||
			(gprIdRi==lsqIdRn3))
				tRegOutOK = UMEM_OK_HOLD;
	end

	tMemAddr	= { UV16_00, lsqAddr0 };
	tMemOutData	= lsqOutData0;
	tMemOpm		= lsqOpm0;

	tExInData	= memInData;
	
	if(lsqOpm0[4:3]!=0)
	begin
		if(!tJointOkLatch)
		begin
			tMemOpm		= UMEM_OPM_READY;
			tLsqHold	= 1;
		end
		else
		if(memDataOK!=UMEM_OK_OK)
		begin
			tLsqHold	= 1;
		end
		else
		begin
			tMemOpm		= UMEM_OPM_READY;
		end

		if(lsqOpm0[3] && !tLsqHold)
		begin
			tRegOutVal	= memInData;
			tRegOutId	= lsqIdRn0;
		end
	end

	if(!tLsqHold)
	begin
		lsqNextIdRn0	= lsqIdRn1;
		lsqNextIdRn1	= lsqIdRn2;
		lsqNextIdRn2	= lsqIdRn3;
		lsqNextIdRn3	= JX2_REG_ZZR;
		lsqNextAddr0	= lsqAddr1;
		lsqNextAddr1	= lsqAddr2;
		lsqNextAddr2	= lsqAddr3;
		lsqNextAddr3	= UV48_XX;
		lsqNextOutData0	= lsqOutData1;
		lsqNextOutData1	= lsqOutData2;
		lsqNextOutData2	= lsqOutData3;
		lsqNextOutData3	= UV64_XX;
		lsqNextOpm0		= lsqOpm1;
		lsqNextOpm1		= lsqOpm2;
		lsqNextOpm2		= lsqOpm3;
		lsqNextOpm3		= UMEM_OPM_READY;
	end
	else
	begin
		lsqNextIdRn0	= lsqIdRn0;
		lsqNextIdRn1	= lsqIdRn1;
		lsqNextIdRn2	= lsqIdRn2;
		lsqNextIdRn3	= lsqIdRn3;
		lsqNextAddr0	= lsqAddr0;
		lsqNextAddr1	= lsqAddr1;
		lsqNextAddr2	= lsqAddr2;
		lsqNextAddr3	= lsqAddr3;
		lsqNextOutData0	= lsqOutData0;
		lsqNextOutData1	= lsqOutData1;
		lsqNextOutData2	= lsqOutData2;
		lsqNextOutData3	= lsqOutData3;
		lsqNextOpm0		= lsqOpm0;
		lsqNextOpm1		= lsqOpm1;
		lsqNextOpm2		= lsqOpm2;
		lsqNextOpm3		= lsqOpm3;
	end
	
	tNextOpmLatch = 0;
	
	if(exIdRn!=JX2_REG_IMM)
//	if(1'b0)
	begin
		if(exOpm!=0)
		begin
			if(!tLsqHold || (lsqOpm3==0) && !tRegOpmLatch)
			begin
				tExDataOK		= UMEM_OK_OK;
				lsqNextIdRn3	= exIdRn;
				lsqNextAddr3	= exAddr[47:0];
				lsqNextOutData3	= exOutData;
				lsqNextOpm3		= exOpm;
				tNextOpmLatch	= 1;
			end
			else
			begin
				if(tRegOpmLatch)
					tExDataOK		= UMEM_OK_OK;
				else
					tExDataOK		= UMEM_OK_HOLD;
				tNextOpmLatch	= tRegOpmLatch;
			end
		end
	end
	else
	begin
		if(	(lsqOpm3!=0) || (lsqOpm2!=0) ||
			(lsqOpm1!=0) || (lsqOpm0!=0))
		begin
			tExDataOK		= UMEM_OK_HOLD;
		end
		else
		begin
			$display("Jx2 LSQ Direct A=%X Opm=%X OK=%X",
				exAddr, exOpm, memDataOK);
			$display("  DO=%X DI=%X", exOutData, memInData);
		
			tMemAddr	= exAddr;
			tMemOutData	= exOutData;
			tMemOpm		= exOpm;

			tExInData	= memInData;
			tExDataOK	= memDataOK;

			if(exOpm[3] && (memDataOK==UMEM_OK_OK) &&
				(exIdRn!=JX2_REG_IMM))
			begin
				tRegOutVal	= memInData;
				tRegOutId	= exIdRn;
				tRegOutOK	= UMEM_OK_OK;
			end
			else
			begin
				tRegOutVal	= UV64_XX;
				tRegOutId	= JX2_REG_ZZR;
				tRegOutOK	= UMEM_OK_OK;
			end
		end
	end
end

always @(posedge clock)
begin
	tExInData2		<= tExInData;		//memory data (load)
	tExDataOK2		<= tExDataOK;		//memory status

	tRegOkLatch		<= tNextOkLatch;
	tRegOpmLatch	<= tNextOpmLatch;

	lsqIdRn0		<= lsqNextIdRn0;
	lsqIdRn1		<= lsqNextIdRn1;
	lsqIdRn2		<= lsqNextIdRn2;
	lsqIdRn3		<= lsqNextIdRn3;
	lsqAddr0		<= lsqNextAddr0;
	lsqAddr1		<= lsqNextAddr1;
	lsqAddr2		<= lsqNextAddr2;
	lsqAddr3		<= lsqNextAddr3;
	lsqOutData0		<= lsqNextOutData0;
	lsqOutData1		<= lsqNextOutData1;
	lsqOutData2		<= lsqNextOutData2;
	lsqOutData3		<= lsqNextOutData3;
	lsqOpm0			<= lsqNextOpm0;
	lsqOpm1			<= lsqNextOpm1;
	lsqOpm2			<= lsqNextOpm2;
	lsqOpm3			<= lsqNextOpm3;

end

endmodule

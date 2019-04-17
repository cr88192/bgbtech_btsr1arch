/*
EX2 Stage (Top Level)

Functionally mirrors EX1, but may complete actions EX1 had started.
Will hold on memory accesses until they complete.

The L1 D$ interface will be driven by a pipeline, which will need to remain in lockstep between the EX1/EX2 stages and the L1.

If an L1 access needs more cycles, it will produce a Hold signal.
EX2 will in turn produce a hold signal and stall the pipeline until the access has been completed.

During a hold, EX1 will be emitting the state of the next request, and as such the L1 may not update its internal request state while it is producing a hold signal.


Some inputs will be mirrored from EX1, or will be the prior outputs from EX1.

 */

`include "CoreDefs.v"

module ExEX2(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

	regValFRs,		//Source A Value (FPR)
	regValFRt,		//Source B Value (FPR)
	regValCRm,		//Source C Value (CR)

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdCn1,		//Destination ID (CR, EX1)
	regValCn1,		//Destination Value (CR, EX1)

	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	regIdCn2,		//Destination ID (CR, EX2)
	regValCn2,		//Destination Value (CR, EX2)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regValAluRes,	//ALU Result
	regValMulRes,	//Multiplier Result
	regFpuGRn,		//FPU GPR Result
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	
	memDataIn,
	memDataOK
	);

input			clock;
input			reset;
input[7:0]		opUCmd;
input[7:0]		opUIxt;
output			exHold;

input[5:0]		regIdRs;		//Source A, ALU / Base
input[5:0]		regIdRt;		//Source B, ALU / Index
input[5:0]		regIdRm;		//Source C, MemStore
input[63:0]		regValRs;		//Source A Value
input[63:0]		regValRt;		//Source B Value
input[63:0]		regValRm;		//Source C Value

input[63:0]		regValFRs;		//Source A Value (FPR)
input[63:0]		regValFRt;		//Source B Value (FPR)
input[63:0]		regValCRm;		//Source C Value (CR)

input[5:0]		regIdRn1;		//Destination ID (EX1)
input[63:0]		regValRn1;		//Destination Value (EX1)
input[4:0]		regIdCn1;		//Destination ID (CR, EX1)
input[63:0]		regValCn1;		//Destination Value (CR, EX1)
	
output[5:0]		regIdRn2;		//Destination ID (EX1)
output[63:0]	regValRn2;		//Destination Value (EX1)
output[4:0]		regIdCn2;		//Destination ID (CR, EX1)
output[63:0]	regValCn2;		//Destination Value (CR, EX1)
	
input[31:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

input[64:0]		regValAluRes;	//Multiplier Result
input[63:0]		regValMulRes;	//Multiplier Result
input[63:0]		regFpuGRn;		//FPU GPR Result
	
output[63:0]	regOutDlr;
input[63:0]		regInDlr;
output[63:0]	regOutDhr;
input[63:0]		regInDhr;
output[63:0]	regOutSp;
input[63:0]		regInSp;

output[31:0]	regOutLr;
input[31:0]		regInLr;
output[63:0]	regOutSr;
input[63:0]		regInSr;

input[63:0]		memDataIn;
input[ 1:0]		memDataOK;


reg				tExHold;
assign	exHold		= tExHold;

reg[ 5:0]		tRegIdRn2;
reg[63:0]		tRegValRn2;
reg[ 4:0]		tRegIdCn2;
reg[63:0]		tRegValCn2;
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[31:0]		tRegOutLr;
reg[63:0]		tRegOutSr;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;
assign	regIdCn2	= tRegIdCn2;
assign	regValCn2	= tRegValCn2;
assign	regOutDlr	= tRegOutDlr;
assign	regOutDhr	= tRegOutDhr;
assign	regOutSp	= tRegOutSp;
assign	regOutLr	= tRegOutLr;
assign	regOutSr	= tRegOutSr;



reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg			tDoMemOp;
reg			tOpEnable;

always @*
begin
	tRegIdRn2	= regIdRn1;		//Forward by default
	tRegValRn2	= regValRn1;	//Forward by default
	tRegIdCn2	= regIdCn1;		//Forward by default
	tRegValCn2	= regValCn1;	//Forward by default

	tRegOutDlr	= regInDlr;
	tRegOutDhr	= regInDhr;
	tRegOutSp	= regInSp;
	tRegOutLr	= regInLr;
	tRegOutSr	= regInSr;

	tDoMemOp	= 0;
	tExHold		= 0;

//	case(opUIxt[7:6])
	case(opUCmd[7:6])
		2'b00: 	tOpEnable = 1;
		2'b01: 	tOpEnable = 0;
		2'b10: 	tOpEnable = regInSr[0];
		2'b11: 	tOpEnable = !regInSr[0];
	endcase
	
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
	
		JX2_UCMD_LEA_MR: begin
		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOp	= 1;
			tRegValRn2	= memDataIn;
		end
		JX2_UCMD_FMOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOp	= 1;
		end

		JX2_UCMD_PUSHX: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_POPX: begin
			tDoMemOp	= 1;
			tRegValRn2	= memDataIn;
			tRegValCn2	= memDataIn;
		end

		JX2_UCMD_ALU3: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
			tRegOutSr[0]	= regValAluRes[64];
		end

		JX2_UCMD_ALUCMP: begin
			tRegOutSr[0]	= regValAluRes[64];
		end
	
		JX2_UCMD_BRA: begin
		end
		JX2_UCMD_BSR: begin
		end
		JX2_UCMD_JMP: begin
		end
		JX2_UCMD_JSR: begin
		end
		
		JX2_UCMD_MUL3: begin
			if(tHoldCyc!=1)
				tExHold=1;

			casez(opUIxt[1:0])
				2'b0z: begin
					tRegIdRn2	= regIdRm;			//
					tRegValRn2	= regValMulRes;		//
				end
				2'b10: begin
					tRegOutDlr  = { UV32_00, regValMulRes[31:0] };
					tRegOutDhr  = {
						regValMulRes[63] ? UV32_FF : UV32_00,
						regValMulRes[63:32] };
				end
				2'b11: begin
					tRegOutDlr  = { UV32_00, regValMulRes[31: 0] };
					tRegOutDhr  = { UV32_00, regValMulRes[63:32] };
				end
			endcase
		end
	
		default: begin
		end
	
	endcase
	
	if(tDoMemOp)
	begin
		if(tHoldCyc==0)
			tExHold=1;
		if(memDataOK[1])
			tExHold=1;

//		tMemOpm = tDoMemOpm;
	end
end

always @(posedge clock)
begin
	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else
		tHoldCyc <= 0;
end

endmodule

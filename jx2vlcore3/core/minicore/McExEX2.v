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

module McExEX2(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

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
	regValMulwRes,	//Multiplier Word Result
	regValKrreRes,	//Keyring Result
	regValAluResB,	//ALU Result (ALUB)
//	regBlintRes,	//ALU Result (ALUB)

	regFpuGRn,		//FPU GPR Result

	opBraFlush,
	regInLastSr,
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	regOutSchm,	regInSchm,

	memDataIn,
	memDataOK
	);

input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
input[63:0]		regValRs;		//Source A Value
input[63:0]		regValRt;		//Source B Value
input[63:0]		regValRm;		//Source C Value

input[63:0]		regValCRm;		//Source C Value (CR)

`input_gpr		regIdRn1;		//Destination ID (EX1)
input[63:0]		regValRn1;		//Destination Value (EX1)
`input_gpr		regIdCn1;		//Destination ID (CR, EX1)
input[63:0]		regValCn1;		//Destination Value (CR, EX1)
	
`output_gpr		regIdRn2;		//Destination ID (EX1)
output[63:0]	regValRn2;		//Destination Value (EX1)
`output_gpr		regIdCn2;		//Destination ID (CR, EX1)
output[63:0]	regValCn2;		//Destination Value (CR, EX1)
	
input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

input[69:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulRes;	//Multiplier Result
input[63:0]		regValMulwRes;	//Multiplier Result
input[65:0]		regValKrreRes;	//Keyring Result
input[65:0]		regValAluResB;	//ALU Result (ALUB)

// input[63:0]		regBlintRes;	//BLINT Result

input[63:0]		regFpuGRn;		//FPU GPR Result

input			opBraFlush;
input[ 7:0]		regInLastSr;

output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

output[63:0]	regOutLr;
input[63:0]		regInLr;
output[63:0]	regOutSr;
input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

input[63:0]		memDataIn;
input[ 1:0]		memDataOK;


reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

`reg_gpr		tRegIdRn2;
reg[63:0]		tRegValRn2;
//reg[ 4:0]		tRegIdCn2;
`reg_gpr		tRegIdCn2;
reg[63:0]		tRegValCn2;
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[63:0]		tRegOutLr;
reg[63:0]		tRegOutSr;
reg[7:0]		tRegOutSchm;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;
assign	regIdCn2	= tRegIdCn2;
assign	regValCn2	= tRegValCn2;
assign	regOutDlr	= tRegOutDlr;
assign	regOutDhr	= tRegOutDhr;
assign	regOutSp	= tRegOutSp;
assign	regOutLr	= tRegOutLr;
assign	regOutSr	= tRegOutSr;
assign	regOutSchm	= tRegOutSchm;


reg[63:0]	tValOutDfl;
reg			tDoOutDfl;


(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;
reg			tDoMemOp;
reg			tOpEnable;
reg			tDoAluSrT;
reg[1:0]	tAluSrbOp;

reg		tMsgLatch;
reg		tNextMsgLatch;


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
	tRegOutSchm	= regInSchm;

	tDoMemOp		= 0;
	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tDoHoldCyc		= 0;
	tDoAluSrT		= 0;
	tAluSrbOp		= 0;

	tValOutDfl		= UV64_XX;
	tDoOutDfl		= 0;
	
	tOpEnable	= !opBraFlush;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_OP_IXS: begin
		end
		
		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end
				JX2_UCIX_IXT_SLEEP: begin
					tDoHoldCyc = 15;
				end
				
				default: begin
				end
			endcase
		end
		
		JX2_UCMD_MOV_IR: begin
		end
	
		JX2_UCMD_LEA_MR: begin
		end
		JX2_UCMD_MOV_RM: begin
			tRegHeld	= 1;
		end
		JX2_UCMD_MOV_MR: begin
			tRegHeld	= 1;
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3,
		JX2_UCMD_ALUB3: begin
			tDoAluSrT		= 1;
			tValOutDfl		= regValAluRes[63:0];
			tDoOutDfl		= 1;
			
			if((opUIxt[3:0]==0) || (opUIxt[3:0]==1))
				tDoAluSrT		= 0;
		end

		JX2_UCMD_CONV2_RR: begin
			tValOutDfl		= regValAluRes[63:0];
			tDoOutDfl		= 1;
		end

		JX2_UCMD_ALUCMP, JX2_UCMD_ALUCMPW, JX2_UCMD_ALUCMPB: begin
			tDoAluSrT		= 1;
		end

		JX2_UCMD_BRA: begin
		end
		JX2_UCMD_BSR: begin
		end
		JX2_UCMD_JMP: begin
`ifdef def_true
			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
				tRegOutSr[15: 4] = regValRs[63:52];
				tRegOutSr[27:26] = regValRs[51:50];
				tRegOutSr[ 1: 0] = regValRs[49:48];
			end
`endif
		end
		JX2_UCMD_JSR: begin
		end
		
		JX2_UCMD_BRA_NB: begin
		end
		
		JX2_UCMD_MUL3: begin
			tRegHeld	= 1;
		end
		
		JX2_UCMD_MULW3: begin
			tValOutDfl		= regValMulwRes;
			tDoOutDfl		= 1;
		end
		
		JX2_UCMD_SHAD3: begin
		end

		JX2_UCMD_CONV_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end
		
		JX2_UCMD_FPU3: begin
			tValOutDfl		= regFpuGRn;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_FLDCX: begin
			tValOutDfl		= regFpuGRn;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_FSTCX: begin
			tValOutDfl		= regFpuGRn;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_FIXS: begin
			tValOutDfl		= regFpuGRn;
			tDoOutDfl		= 1;
		end

		JX2_UCMD_FCMP: begin
			tDoAluSrT		= 1;
			tAluSrbOp		= 0;
		end
	
		default: begin
			if(!tMsgLatch)
				$display("EX2: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase
	
	if(tDoOutDfl)
	begin
		tRegIdRn2		= regIdRm;
		tRegValRn2		= tValOutDfl;
	end
	
	if(tDoMemOp)
	begin
		$display("EX2: DoMemOp but MemEx3 %X", opUCmd);
	end

	if(tDoAluSrT)
	begin
		tRegOutSr[1:0]	= regValAluRes[65:64];
		tRegOutSr[7:4]	= regValAluRes[69:66];
	end

	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
		tRegIdCn2	= JX2_CR_ZZR;
	end

	if(tHoldCyc < tDoHoldCyc)
	begin
		tExHold=1;
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

	if(tExHold)
//		tHoldCyc <= tHoldCyc + 1;
		tHoldCyc <= tHoldCyc + ((tHoldCyc!=15)?1:0);
	else
		tHoldCyc <= 0;
end

endmodule

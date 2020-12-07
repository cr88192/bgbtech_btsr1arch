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
	regValMulwRes,	//Multiplier Word Result
	regValKrreRes,	//Keyring Result

	regFpuGRn,		//FPU GPR Result
	regFpuLdGRn,		//FPU GPR Result
	regFpuSrT,
	regFpuOK,

	opBraFlush,
	regInLastSr,
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	regOutSchm,	regInSchm,

	memDataIn,
	memDataInB,
	memDataOK
	);

input			clock;
input			reset;
input[7:0]		opUCmd;
input[7:0]		opUIxt;
output[1:0]		exHold;

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
	
input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

// input[65:0]		regValAluRes;	//ALU Result
input[69:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulRes;	//Multiplier Result
input[63:0]		regValMulwRes;	//Multiplier Result
input[65:0]		regValKrreRes;	//Keyring Result

input[63:0]		regFpuGRn;		//FPU GPR Result
input[63:0]		regFpuLdGRn;	//FPU GPR Result (Mem Load)
input			regFpuSrT;
input[ 1:0]		regFpuOK;		//FPU Status

input			opBraFlush;
input[ 7:0]		regInLastSr;

output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

output[47:0]	regOutLr;
input[47:0]		regInLr;
output[63:0]	regOutSr;
input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

input[63:0]		memDataIn;
input[63:0]		memDataInB;
input[ 1:0]		memDataOK;


reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

reg[ 5:0]		tRegIdRn2;
reg[63:0]		tRegValRn2;
reg[ 4:0]		tRegIdCn2;
reg[63:0]		tRegValCn2;
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[47:0]		tRegOutLr;
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



(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;
reg			tDoMemOp;
reg			tOpEnable;
reg			tDoAluSrT;

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

//	tRegOutSr[1:0]	= regValAluRes[65:64];

`ifndef def_true
//	case(opUIxt[7:6])
	casez( { opBraFlush, opUCmd[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
//		3'b010: 	tOpEnable = regInSr[0];
		3'b010: 	tOpEnable = regInLastSr[0];
//		3'b011: 	tOpEnable = !regInSr[0];
		3'b011: 	tOpEnable = !regInLastSr[0];
		3'b1zz: 	tOpEnable = 0;
	endcase
`endif

`ifdef def_true
	casez( { opBraFlush, opUCmd[7:6], regInLastSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif
	
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
				
				JX2_UCIX_IXT_LDEKRR: begin
					if(regValKrreRes[65:64]==2'b10)
					begin
						tRegIdCn2	= JX2_CR_KRR[4:0];
						tRegValCn2	= regValKrreRes[63:0];
					end
				end
				
				JX2_UCIX_IXT_LDEENC: begin
					tRegIdRn2	= JX2_GR_DLR;
					tRegValRn2	= regValKrreRes[63:0];
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
`ifdef jx2_stage_memex3
			tRegHeld	= 1;
`else
			tDoMemOp	= 1;
`endif
		end
		JX2_UCMD_MOV_MR: begin
`ifdef jx2_stage_memex3
			tRegHeld	= 1;
`else
			tDoMemOp	= 1;
			tRegIdRn2	= regIdRm;
			tRegValRn2	= memDataIn;
`ifdef jx2_debug_ldst
			$display("LOAD(2): R=%X V=%X", regIdRm, memDataIn);
`endif
`endif
		end

// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fmov
// `ifdef jx2_enable_fprs
		JX2_UCMD_FMOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOp	= 1;
			
`ifdef jx2_enable_fprs
			if(opUIxt[2])
			begin
				tRegIdRn2		= regIdRm;
				tRegValRn2		= regFpuLdGRn;
			end
`else
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuLdGRn;
`endif
		end
`endif

`ifndef def_true
		JX2_UCMD_PUSHX: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_POPX: begin
			tDoMemOp	= 1;
			casez(opUIxt[1:0])
				2'b00: tRegIdRn2	= regIdRm;
				2'b01: tRegIdCn2	= regIdRm[4:0];
				default: begin
				end
			endcase
			tRegValRn2	= memDataIn;
			tRegValCn2	= memDataIn;
`ifdef jx2_debug_ldst
			$display("POP(2): R=%X V=%X", regIdRm, memDataIn);
`endif
		end

		JX2_UCMD_ADDSP: begin
		end
`endif

//		JX2_UCMD_ALU3: begin
		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
//			tRegOutSr[1:0]	= regValAluRes[65:64];
			tDoAluSrT		= 1;
		end

		JX2_UCMD_ALUCMP: begin
//			tRegOutSr[0]	= regValAluRes[64];
//			tRegOutSr[1:0]	= regValAluRes[65:64];
//			tRegOutSr[7:4]	= regValAluRes[69:66];
			tDoAluSrT		= 1;
		end

`ifndef def_true
		JX2_UCMD_UNARY: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
//			tRegOutSr[1:0]	= regValAluRes[65:64];
			tDoAluSrT		= 1;
		end
`endif

		JX2_UCMD_BRA: begin
		end
		JX2_UCMD_BSR: begin
		end
		JX2_UCMD_JMP: begin
		end
		JX2_UCMD_JSR: begin
		end
		
		JX2_UCMD_MUL3: begin
			tDoHoldCyc	= 3;
//			if(tHoldCyc!=1)
//			if(tHoldCyc!=4)
//			if(tHoldCyc!=3)
//				tExHold=1;

			tRegIdRn2	= regIdRm;					//
			tRegValRn2	= regValMulRes[63:0];		//

`ifndef def_true
			casez(opUIxt[2:0])
				3'b000: begin
					tRegIdRn2	= regIdRm;			//
//					tRegValRn2	= regValMulRes;		//
					tRegValRn2	= {
						regValMulRes[31] ? UV32_FF : UV32_00,
						regValMulRes[31:0] };		//
				end
				3'b001: begin
					tRegIdRn2	= regIdRm;			//
//					tRegValRn2	= regValMulRes;		//
					tRegValRn2	= {
						UV32_00,
						regValMulRes[31:0] };		//
				end
`ifndef def_true
				3'b010: begin
					tRegOutDlr  = { UV32_00, regValMulRes[31:0] };
					tRegOutDhr  = {
						regValMulRes[63] ? UV32_FF : UV32_00,
						regValMulRes[63:32] };
				end
				3'b011: begin
					tRegOutDlr  = { UV32_00, regValMulRes[31: 0] };
					tRegOutDhr  = { UV32_00, regValMulRes[63:32] };
				end
`endif

				3'b100: begin
					tRegIdRn2	= regIdRm;					//
					tRegValRn2	= regValMulRes[63:0];		//
				end
				3'b101: begin
					tRegIdRn2	= regIdRm;					//
					tRegValRn2	= regValMulRes[63:0];		//
				end
			endcase
`endif
		end
		
		JX2_UCMD_MULW3: begin
			tRegIdRn2	= regIdRm;			//
			tRegValRn2	= regValMulwRes;		//
		end
		
		JX2_UCMD_SHAD3: begin
		end
`ifndef jx2_merge_shadq
		JX2_UCMD_SHLD3: begin
		end
		JX2_UCMD_SHADQ3: begin
		end
		JX2_UCMD_SHLDQ3: begin
		end
`endif
		
		JX2_UCMD_CONV_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end
		
		JX2_UCMD_FPU3: begin
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FLDCX: begin
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FSTCX: begin
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FIXS: begin
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end

		JX2_UCMD_FCMP: begin
`ifdef jx2_fcmp_alu
			tDoAluSrT		= 1;
//			tRegOutSr[1:0]	= regValAluRes[65:64];
`else
			tRegOutSr[0]	= regFpuSrT;
`endif
		end
	
		default: begin
			if(!tMsgLatch)
				$display("EX2: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase
	
`ifdef jx2_stage_memex3
	if(tDoMemOp)
	begin
		$display("EX2: DoMemOp but MemEx3 %X", opUCmd);
	end
`else
	if(tDoMemOp)
	begin
`ifdef jx2_debug_ldst
		$display("EX2: DoMemOp, OK=%X", memDataOK);
`endif

`ifndef jx2_do_ld1cyc
//		if(tHoldCyc==0)
//			tExHold=1;
		tDoHoldCyc	= 1;
`endif

		if(memDataOK[1])
		begin
			if(memDataOK[0])
			begin
				if(!tMsgLatch)
					$display("EX2: Memory Fault");
				tNextMsgLatch	= 1;
			end
			else
			begin
`ifdef jx2_debug_ldst
				$display("EX2: Memory Hold");
`endif
			end
			tExHold=1;
		end
`ifndef def_true
		else if(!memDataOK[0])
		begin
`ifdef jx2_debug_ldst
			$display("EX2: Memory Ready");
`endif
			tExHold=1;
		end
`endif

//		tMemOpm = tDoMemOpm;
	end
`endif

	if(tDoAluSrT)
	begin
		tRegOutSr[1:0]	= regValAluRes[65:64];
		tRegOutSr[7:4]	= regValAluRes[69:66];
	end

	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
		tRegIdCn2	= JX2_CR_ZZR[4:0];
	end

//	if((tDoHoldCyc != 0) && (tHoldCyc != tDoHoldCyc))
//	if(tHoldCyc != tDoHoldCyc)
	if(tHoldCyc < tDoHoldCyc)
	begin
//		if(!tExHold)
//			$display("Hold Cyc %d %d", tHoldCyc, tDoHoldCyc);
		tExHold=1;
	end

//	if(reset)
//		tExHold=0;
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

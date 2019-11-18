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

input[65:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulRes;	//Multiplier Result
input[63:0]		regValMulwRes;	//Multiplier Result

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

output[31:0]	regOutLr;
input[31:0]		regInLr;
output[63:0]	regOutSr;
input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

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



reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg			tDoMemOp;
reg			tOpEnable;

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

	tDoMemOp	= 0;
	tExHold		= 0;
	tNextMsgLatch	= 0;

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
	
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_OP_IXS: begin
		end
		
		JX2_UCMD_OP_IXT: begin
		end
		
		JX2_UCMD_MOV_IR: begin
		end
	
		JX2_UCMD_LEA_MR: begin
		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOp	= 1;
			tRegIdRn2	= regIdRm;
			tRegValRn2	= memDataIn;
`ifdef jx2_debug_ldst
			$display("LOAD(2): R=%X V=%X", regIdRm, memDataIn);
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

//		JX2_UCMD_ALU3: begin
		JX2_UCMD_ALU3, JX2_UCMD_UNARY: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
			tRegOutSr[1:0]	= regValAluRes[65:64];
		end

		JX2_UCMD_ALUCMP: begin
//			tRegOutSr[0]	= regValAluRes[64];
			tRegOutSr[1:0]	= regValAluRes[65:64];
		end

`ifndef def_true
		JX2_UCMD_UNARY: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
			tRegOutSr[1:0]	= regValAluRes[65:64];
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
//			if(tHoldCyc!=1)
//			if(tHoldCyc!=4)
			if(tHoldCyc!=3)
				tExHold=1;

			casez(opUIxt[1:0])
				2'b00: begin
					tRegIdRn2	= regIdRm;			//
//					tRegValRn2	= regValMulRes;		//
					tRegValRn2	= {
						regValMulRes[31] ? UV32_FF : UV32_00,
						regValMulRes[31:0] };		//
				end
				2'b01: begin
					tRegIdRn2	= regIdRm;			//
//					tRegValRn2	= regValMulRes;		//
					tRegValRn2	= {
						UV32_00,
						regValMulRes[31:0] };		//
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
		
		JX2_UCMD_MULW3: begin
			tRegIdRn2	= regIdRm;			//
			tRegValRn2	= regValMulwRes;		//
		end
		
		JX2_UCMD_SHAD3: begin
		end
		JX2_UCMD_SHLD3: begin
		end
		JX2_UCMD_SHADQ3: begin
		end
		JX2_UCMD_SHLDQ3: begin
		end
		
		JX2_UCMD_CONV_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end
		
		JX2_UCMD_FPU3: begin
//			if(regFpuOK[1])
			if(regFpuOK != UMEM_OK_OK)
				tExHold			= 1;
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FLDCX: begin
//			if(regFpuOK[1])
			if(regFpuOK != UMEM_OK_OK)
				tExHold			= 1;
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FSTCX: begin
//			if(regFpuOK[1])
			if(regFpuOK != UMEM_OK_OK)
				tExHold			= 1;
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end
		JX2_UCMD_FIXS: begin
//			if(regFpuOK[1])
			if(regFpuOK != UMEM_OK_OK)
				tExHold			= 1;
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end

		JX2_UCMD_FCMP: begin
//			if(regFpuOK[1])
			if(regFpuOK != UMEM_OK_OK)
				tExHold			= 1;
			tRegOutSr[0]	= regFpuSrT;
		end
	
		default: begin
			if(!tMsgLatch)
				$display("EX2: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase
	
	if(tDoMemOp)
	begin
`ifdef jx2_debug_ldst
		$display("EX2: DoMemOp, OK=%X", memDataOK);
`endif

`ifndef jx2_do_ld1cyc
		if(tHoldCyc==0)
			tExHold=1;
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
	
	if(reset)
		tExHold=0;
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else
		tHoldCyc <= 0;
end

endmodule

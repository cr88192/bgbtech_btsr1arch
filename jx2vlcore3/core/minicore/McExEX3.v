/*
EX3 Stage (Top Level, Lane 1)

Third Execute Stage.
Continues on the work from the first 2 stages.

 */

`include "CoreDefs.v"

module McExEX3(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,		exIsHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdCn1,		//Destination ID (EX1)
	regValCn1,		//Destination Value (EX1)

	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	regIdCn2,		//Destination ID (EX2)
	regValCn2,		//Destination Value (EX2)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regValAluRes,	//ALU Result
	regValMulRes,	//ALU Result
	regValMulwRes,	//ALU Result
	regValKrreRes,	//Keyring Result

	regFpuGRn,		//FPU GPR Result
	opBraFlush,

	regInLastSr,	
	memDataIn,
	memDataOK
	);

input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;
input			exIsHold;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
input[63:0]		regValRs;		//Source A Value
input[63:0]		regValRt;		//Source B Value
input[63:0]		regValRm;		//Source C Value

`input_gpr		regIdRn1;		//Destination ID (EX1)
input[63:0]		regValRn1;		//Destination Value (EX1)
//input[4:0]		regIdCn1;		//Destination ID (CR, EX1)
`input_gpr		regIdCn1;		//Destination ID (CR, EX1)
input[63:0]		regValCn1;		//Destination Value (CR, EX1)

`output_gpr		regIdRn2;		//Destination ID (EX1)
output[63:0]	regValRn2;		//Destination Value (EX1)
//output[4:0]		regIdCn2;		//Destination ID (CR, EX1)
`output_gpr		regIdCn2;		//Destination ID (CR, EX1)
output[63:0]	regValCn2;		//Destination Value (CR, EX1)

input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

input[69:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulwRes;	//MUL.W Result
input[65:0]		regValKrreRes;	//Keyring Result

input[63:0]		regValMulRes;	//MUL.W Result
input[63:0]		regFpuGRn;		//FPU GPR Result

input			opBraFlush;

input[ 7:0]		regInLastSr;

input[63:0]		memDataIn;
input[1:0]		memDataOK;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

`reg_gpr		tRegIdRn2;
reg[63:0]		tRegValRn2;
//reg[ 4:0]		tRegIdCn2;
`reg_gpr		tRegIdCn2;
reg[63:0]		tRegValCn2;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;
assign	regIdCn2	= tRegIdCn2;
assign	regValCn2	= tRegValCn2;


reg[63:0]	tValOutDfl;
reg			tDoOutDfl;


(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;
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

	tDoMemOp		= 0;
	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tDoHoldCyc		= 0;

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
				default: begin
				end
			endcase
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
			tValOutDfl		= memDataIn;
			tDoOutDfl		= 1;

`ifdef jx2_debug_ldst
			$display("LOAD(3): R=%X V=%X", regIdRm, memDataIn);
`endif
		end

		JX2_UCMD_BRA: begin
		end
		JX2_UCMD_BSR: begin
		end
		JX2_UCMD_JMP: begin
		end
		JX2_UCMD_JSR: begin
		end

		JX2_UCMD_BRA_NB: begin
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY,
		JX2_UCMD_ALUW3, JX2_UCMD_ALUB3: begin
		end

		JX2_UCMD_ALUCMP: begin
		end
		
		JX2_UCMD_MUL3: begin
			tValOutDfl		= regValMulRes[63:0];
			tDoOutDfl		= 1;
		end

		JX2_UCMD_FLDCX: begin
		end
		JX2_UCMD_FSTCX: begin
		end
		JX2_UCMD_FCMP: begin
		end

		JX2_UCMD_MULW3: begin
		end

		JX2_UCMD_SHAD3: begin
		end
		
		JX2_UCMD_CONV_RR: begin
		end
		JX2_UCMD_CONV2_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end

		JX2_UCMD_FPU3: begin
		end

		default: begin
			if(!tMsgLatch)
				$display("EX3: Unhandled UCmd %X", opUCmd);
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
`ifdef jx2_debug_ldst
		$display("EX3: DoMemOp, OK=%X", memDataOK);
`endif

		if(memDataOK[1])
		begin
			if(memDataOK[0])
			begin
				if(!tMsgLatch)
					$display("EX3: Memory Fault");
				tNextMsgLatch	= 1;
			end
`ifndef	jx2_mem_l1dstall
			tExHold=1;
`endif
		end
		else
		begin
`ifdef jx2_debug_ldst
			if(!memDataOK[0])
			begin
				$display("EX3: Memory Ready");
			end
`endif
		end
	end
	
	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
		tRegIdCn2	= JX2_CR_ZZR;
	end

	if(tHoldCyc < tDoHoldCyc)
	begin
//		if(!tExHold)
//			$display("Hold Cyc %d %d", tHoldCyc, tDoHoldCyc);
		tExHold=1;
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

	if(tExHold)
		tHoldCyc <= tHoldCyc + ((tHoldCyc!=15)?1:0);
	else
		tHoldCyc <= 0;
end

endmodule

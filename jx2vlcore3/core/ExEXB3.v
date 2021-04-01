/*
EX3 Stage (Top Level, Lane 2+)

Third Execute Stage.
Continues on the work from the first 2 stages.

 */

`include "CoreDefs.v"

module ExEXB3(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)

	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regValAluRes,	//ALU Result
	regValMulwRes,	//ALU Result
	regFpuGRn,		//FPU GPR Result
	opBraFlush,

	regInLastSr,	
	memDataIn,
	memDataInB
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

input[5:0]		regIdRn1;		//Destination ID (EX1)
input[63:0]		regValRn1;		//Destination Value (EX1)

output[5:0]		regIdRn2;		//Destination ID (EX1)
output[63:0]	regValRn2;		//Destination Value (EX1)

input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

input[65:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulwRes;	//MUL.W Result
input[63:0]		regFpuGRn;		//FPU GPR Result

input			opBraFlush;

input[ 7:0]		regInLastSr;

input[63:0]		memDataIn;
input[63:0]		memDataInB;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

reg[ 5:0]		tRegIdRn2;
reg[63:0]		tRegValRn2;
reg[ 5:0]		tRegIdCn2;
reg[63:0]		tRegValCn2;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;


(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

// reg[3:0]	tHoldCyc;
reg			tOpEnable;

reg		tMsgLatch;
reg		tNextMsgLatch;


always @*
begin
	tRegIdRn2	= regIdRn1;		//Forward by default
	tRegValRn2	= regValRn1;	//Forward by default

	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;

`ifndef def_true
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

	tOpEnable	= !opBraFlush;
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
		end
		JX2_UCMD_MOV_MR: begin
			tRegIdRn2	= regIdRm;
			tRegValRn2	= memDataInB;
		end

		JX2_UCMD_ADDSP: begin
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
		end

		JX2_UCMD_ALUCMP: begin
		end
		
		JX2_UCMD_MUL3: begin
		end

		JX2_UCMD_MULW3: begin
//			tRegIdRn2	= regIdRm;			//
//			tRegValRn2	= regValMulwRes;		//
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
//			tRegIdRn2		= regIdRm;
//			tRegValRn2		= regFpuGRn;
		end

		JX2_UCMD_BLINT: begin
		end

		default: begin
			if(!tMsgLatch)
				$display("EX3B: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase

	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

//	if(tExHold)
//		tHoldCyc <= tHoldCyc + 1;
//	else
//		tHoldCyc <= 0;
end

endmodule

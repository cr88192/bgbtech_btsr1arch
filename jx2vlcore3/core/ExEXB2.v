/*
EX2 Stage (Top Level, Lane 2+)

Functionally mirrors EX1, but may complete actions EX1 had started.

 */

`include "CoreDefs.v"

module ExEXB2(
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
	regValKrreRes,	//Keyring Result

	regFpuGRn,		//FPU GPR Result
	opBraFlush,

	regInLastSr,	
	regInSr,
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
input[65:0]		regValKrreRes;	//Keyring Result

input[63:0]		regFpuGRn;		//FPU GPR Result

input			opBraFlush;

input[63:0]		regInSr;
input[ 7:0]		regInLastSr;

input[63:0]		memDataIn;
input[63:0]		memDataInB;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

reg[ 5:0]		tRegIdRn2;
reg[63:0]		tRegValRn2;

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

				JX2_UCIX_IXT_LDEENC: begin
					tRegIdRn2	= JX2_GR_DHR;
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
		end
		JX2_UCMD_MOV_MR: begin
`ifdef jx2_stage_memex3
			tRegHeld	= 1;
`else
			tRegIdRn2	= regIdRm;
			tRegValRn2	= memDataInB;
`endif
		end

		JX2_UCMD_ADDSP: begin
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
			tRegIdRn2		= regIdRm;			//
			tRegValRn2		= regValAluRes[63:0];		//
//			tRegOutSr[1:0]	= regValAluRes[65:64];
		end

		JX2_UCMD_ALUCMP: begin
		end
		
		JX2_UCMD_MUL3: begin
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

`ifndef def_true
		JX2_UCMD_PUSHX: begin
		end

		JX2_UCMD_POPX: begin
			tRegIdRn2	= regIdRm;
			tRegValRn2	= memDataInB;
		end
`endif

		JX2_UCMD_FPU3: begin
//			if(regFpuOK[1])
//			if(regFpuOK != UMEM_OK_OK)
//				tExHold			= 1;
			tRegIdRn2		= regIdRm;
			tRegValRn2		= regFpuGRn;
		end

		default: begin
			if(!tMsgLatch)
				$display("EX2B: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase
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

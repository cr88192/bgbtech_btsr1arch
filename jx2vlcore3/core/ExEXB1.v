/*
EX1 Stage (Top Level, Lane 2+)

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [5:0]=Depends on opcode
 */

`include "CoreDefs.v"

// `include "ExConv2R.v"

`ifndef jx2_merge_shadq
`include "ExShad32B.v"
`include "ExShad64B.v"
`endif

`ifdef jx2_merge_shadq
`include "ExShad64C.v"
`endif

module ExEXB1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	heldIdRn1,		//Held Destination ID (EX1)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	opBraFlush,
	regInSr
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

output[5:0]		regIdRn1;		//Destination ID (EX1)
output[63:0]	regValRn1;		//Destination Value (EX1)
output[5:0]		heldIdRn1;		//Held Destination ID (EX1)
	
input[32:0]		regValImm;		//Immediate (Decode)
input[31:0]		regValPc;
input			opBraFlush;

input[63:0]		regInSr;


reg[ 5:0]		tRegIdRn1;		//Destination ID (EX1)
reg[63:0]		tRegValRn1;		//Destination Value (EX1)
reg[ 5:0]		tHeldIdRn1;		//Destination ID (EX1)

assign	regIdRn1	= tRegIdRn1;		//Destination ID (EX1)
assign	regValRn1	= tRegValRn1;		//Destination Value (EX1)
assign	heldIdRn1	= tHeldIdRn1;		//Held Destination ID (EX1)

reg				tExHold;
assign	exHold		= tExHold;


wire[63:0]	tValCnv;
wire		tCnvSrT;
ExConv2R	exConv2R(regValRs, opUIxt, regInSr[0], tValCnv, tCnvSrT);

`ifndef jx2_merge_shadq
wire[31:0]	tValShad32;
//ExShad32	exShad32(clock, reset,
ExShad32B	exShad32(clock, reset,
	regValRs[31:0], regValRt[7:0],
	tValShad32, opUCmd[0]);

wire[63:0]	tValShad64;
//ExShad64	exShad64(clock, reset,
ExShad64B	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
	tValShad64, opUCmd[0]);
`endif

`ifdef jx2_merge_shadq
wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64C	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
	tValShad64, opUCmd[1:0]);
`endif

reg			tOpEnable;

reg[5:0]	tOpUCmd1;

reg tMsgLatch;
reg tNextMsgLatch;

always @*
begin

	tRegIdRn1	= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1	= UV64_XX;			//Destination Value (EX1)
	tHeldIdRn1	= JX2_GR_ZZR;

	tExHold		= 0;
	tNextMsgLatch	= 0;

	casez( { opBraFlush, opUCmd[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = regInSr[0];
		3'b011: 	tOpEnable = !regInSr[0];
		3'b1zz: 	tOpEnable = 0;
	endcase
	
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_INVOP: begin
			if(!tMsgLatch)
				$display("EX: Invalid Opcode");
			tNextMsgLatch	= 1;
//			tExHold		= 1;
			tExHold		= !reset;
		end
	
		JX2_UCMD_LEA_MR: begin
		end

		JX2_UCMD_MOV_RM: begin
			tRegIdRn1		= JX2_GR_ZZR;
			tRegValRn1		= regValRs;
		end
		JX2_UCMD_MOV_MR: begin
		end

		JX2_UCMD_FMOV_RM: begin
		end
		JX2_UCMD_FMOV_MR: begin
		end

		JX2_UCMD_PUSHX: begin
//			$display("EXB1: PushX Id=%d Rm=%X", regIdRm, regValRm);
			tRegIdRn1		= JX2_GR_ZZR;
			tRegValRn1		= regValRm;
		end
		JX2_UCMD_POPX: begin
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY: begin
			tHeldIdRn1	= regIdRm;
		end

		JX2_UCMD_ALUCMP: begin
		end
	
		JX2_UCMD_CONV_RR: begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= tValCnv;
//			tRegOutSr[0]	= tCnvSrT;
		end
		JX2_UCMD_MOV_IR: begin
			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= {
						regValImm[32] ? UV32_FF : UV32_00,
						regValImm[31:0] };
				end
				4'b0001: begin /* LDISH8 */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= { regValRs[55:0], regValImm[7:0] };
				end
				4'b0010: begin /* LDISH16 */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= { regValRs[47:0], regValImm[15:0] };
				end
				4'b0011: begin /* LDISH32 */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= { regValRs[31:0], regValImm[31:0] };
				end

				default: begin
					$display("ExEXB1: MOV_IR, Invalid UIxt %X", opUIxt);
					tRegIdRn1	= regIdRm;
					tRegValRn1	= regValRt;
				end
			endcase
		end
		
		JX2_UCMD_MULW3: begin
			tHeldIdRn1	= regIdRm;
//			tRegIdRn1	= regIdRm;			//
//			tRegValRn1	= regValMulwRes;		//
		end

		JX2_UCMD_SHAD3: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
		end
		JX2_UCMD_SHLD3: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= { UV32_00, tValShad32 };
		end
	
		JX2_UCMD_SHADQ3: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= tValShad64;
		end
		JX2_UCMD_SHLDQ3: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= tValShad64;
		end

		JX2_UCMD_MUL3: begin
		end

		JX2_UCMD_OP_IXS: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXS_NOP: begin
				end
				JX2_UCIX_IXS_MOVT: begin
					tRegIdRn1		= regIdRm;
					tRegValRn1		= {UV63_00, regInSr[0]};
				end
				JX2_UCIX_IXS_MOVNT: begin
					tRegIdRn1		= regIdRm;
					tRegValRn1		= {UV63_00, !regInSr[0]};
				end
				default: begin
					if(!tMsgLatch)
						$display("EX1B: Unhandled Op-IXS %X", opUIxt);
					tNextMsgLatch	= 1;
//					tExHold		= 1;
					tExHold		= !reset;
				end
			endcase
		end

		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end
				JX2_UCIX_IXT_SLEEP: begin
				end
				JX2_UCIX_IXT_BREAK: begin
					if(!tMsgLatch)
						$display("EX1B: BREAK");
					tNextMsgLatch	= 1;
//					tExHold		= 1;
					tExHold		= !reset;
				end

				default: begin
					if(!tMsgLatch)
						$display("EX1B: Unhandled Op-IXT %X", opUIxt);
					tNextMsgLatch	= 1;
//					tExHold		= 1;
					tExHold		= !reset;
				end
			endcase
		end

		default: begin
			if(!tMsgLatch)
				$display("EX1B: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
//			tExHold		= 1;
			tExHold		= !reset;
		end
	
	endcase

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

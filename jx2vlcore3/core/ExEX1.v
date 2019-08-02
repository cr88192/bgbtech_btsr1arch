/*
EX1 Stage (Top Level)

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [5:0]=Depends on opcode

EX1 will only initiate a memory Load/Store.
Holding/Completing a memory access will be the responsibility of EX2.
 */

`include "CoreDefs.v"

`include "ExAGU.v"
// `include "ExALU.v"
`include "ExConv2R.v"

`ifndef jx2_merge_shadq
//`include "ExShad32.v"
`include "ExShad32B.v"
// `include "ExShad64.v"
`include "ExShad64B.v"
`endif

`ifdef jx2_merge_shadq
`include "ExShad64C.v"
`endif

`ifdef jx2_enable_swapn
`include "ExSwapN.v"
`endif

`ifdef jx2_enable_shlln
`include "ExShllN.v"
`endif

module ExEX1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regValFRs,		//Source A Value (FPR)
	regValFRt,		//Source B Value (FPR)
	regValCRm,		//Source C Value (CR)

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdCn1,		//Destination ID (CR, EX1)
	regValCn1,		//Destination Value (CR, EX1)
	heldIdRn1,		//Held Destination ID (EX1)
	heldIdCn1,		//Held Destination ID (CR, EX1)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regFpuGRn,		//FPU GPR Result
	regFpuSrT,		//FPU SR.T Result
	opBraFlush,
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	
	memAddr,	memOpm,
	memDataOut
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

output[5:0]		regIdRn1;		//Destination ID (EX1)
output[63:0]	regValRn1;		//Destination Value (EX1)
output[4:0]		regIdCn1;		//Destination ID (CR, EX1)
output[63:0]	regValCn1;		//Destination Value (CR, EX1)
output[5:0]		heldIdRn1;		//Held Destination ID (EX1)
output[4:0]		heldIdCn1;		//Held Destination ID (CR, EX1)
	
input[31:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)
input[63:0]		regFpuGRn;		//FPU GPR Result
input			regFpuSrT;
input			opBraFlush;

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

output[31:0]	memAddr;
output[ 4:0]	memOpm;
output[63:0]	memDataOut;


reg[ 5:0]		tRegIdRn1;		//Destination ID (EX1)
reg[63:0]		tRegValRn1;		//Destination Value (EX1)
reg[ 4:0]		tRegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		tRegValCn1;		//Destination Value (CR, EX1)
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[31:0]		tRegOutLr;
reg[63:0]		tRegOutSr;

reg[ 5:0]		tHeldIdRn1;		//Destination ID (EX1)
reg[ 4:0]		tHeldIdCn1;		//Destination ID (CR, EX1)

assign	regIdRn1	= tRegIdRn1;		//Destination ID (EX1)
assign	regValRn1	= tRegValRn1;		//Destination Value (EX1)
assign	regIdCn1	= tRegIdCn1;		//Destination ID (CR, EX1)
assign	regValCn1	= tRegValCn1;		//Destination Value (CR, EX1)
assign	heldIdRn1	= tHeldIdRn1;		//Held Destination ID (EX1)
assign	heldIdCn1	= tHeldIdCn1;		//Held Destination ID (CR, EX1)

assign	regOutDlr	= tRegOutDlr;
assign	regOutDhr	= tRegOutDhr;
assign	regOutSp	= tRegOutSp;
assign	regOutLr	= tRegOutLr;
assign	regOutSr	= tRegOutSr;

reg[31:0]		tMemAddr;
reg[ 4:0]		tMemOpm;
reg[63:0]		tMemDataOut;

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;

reg				tExHold;
assign	exHold		= tExHold;



wire[31:0]	tValAgu;
ExAGU	exAgu(regValRs[31:0], regValRt[31:0], opUIxt, tValAgu);

// wire[63:0]	tValAlu;
// wire		tAluSrT;
// ExALU	exAlu(regValRs, regValRt, opUIxt, regInSr[0], tValAlu, tAluSrT);

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

`ifdef jx2_enable_swapn
wire[63:0]	tValSwapN;
ExSwapN		exSwapN(clock, reset,
	opUIxt, regValRs, tValSwapN);
`endif

`ifdef jx2_enable_shlln
wire[63:0]	tValShllN;
wire		tShllSrT;
ExShllN		exShllN(
	opUIxt, regValRs, tValShllN,
	regInSr[0], tShllSrT);
`endif


reg[63:0]	tRegSpAdd8;
reg[63:0]	tRegSpSub8;

reg			tOpEnable;
reg			tDoMemOp;
reg[4:0]	tDoMemOpm;

reg[5:0]	tOpUCmd1;

reg tMsgLatch;
reg tNextMsgLatch;

always @*
begin

	tRegIdRn1	= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1	= UV64_XX;			//Destination Value (EX1)
	tRegIdCn1	= JX2_CR_ZZR[4:0];	//Destination ID (CR, EX1)
	tRegValCn1	= UV64_XX;			//Destination Value (CR, EX1)
	
	tHeldIdRn1	= JX2_GR_ZZR;
	tHeldIdCn1	= JX2_CR_ZZR[4:0];

	tRegOutDlr	= regInDlr;
	tRegOutDhr	= regInDhr;
	tRegOutSp	= regInSp;
	tRegOutLr	= regInLr;
	tRegOutSr	= regInSr;

	tMemAddr	= tValAgu;
	tMemOpm		= UMEM_OPM_READY;
	tMemDataOut	= regValRm;
	
	tDoMemOpm	= UMEM_OPM_READY;
	tDoMemOp	= 0;
	tExHold		= 0;
	tNextMsgLatch	= 0;


	tRegSpAdd8 	= { regInSp[63:28], regInSp[27:3]+25'h1, regInSp[2:0]};
	tRegSpSub8 	= { regInSp[63:28], regInSp[27:3]-25'h1, regInSp[2:0]};

//	case(opUIxt[7:6])
//	case(opUCmd[7:6])
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
			tExHold		= 1;
		end
	
		JX2_UCMD_LEA_MR: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= { UV32_00, tValAgu };
		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;

`ifdef jx2_debug_ldst
			$display("STORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;
			tHeldIdRn1	= regIdRm;

`ifdef jx2_debug_ldst
			$display("LOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end

// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
		JX2_UCMD_FMOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;
		end
`endif

		JX2_UCMD_PUSHX: begin
			tMemAddr	= tRegSpSub8[31:0];
			tRegOutSp	= tRegSpSub8;
			tDoMemOpm	= UMEM_OPM_WR_Q;
			tDoMemOp	= 1;
			
			case(opUIxt[1:0])
//				2'b00: 	tMemDataOut = regValRm;
				2'b00: 	tMemDataOut = regValRs;
				2'b01: 	tMemDataOut = regValCRm;
				2'b10: 	tMemDataOut = regValFRs;
//				2'b11: 	tMemDataOut = regValRm;
				2'b11: 	tMemDataOut = regValRs;
			endcase
			
`ifdef jx2_debug_ldst
			$display("PUSH: SP=%X R=%X V=%X", tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_POPX: begin
			tMemAddr	= regInSp[31:0];
			tRegOutSp	= tRegSpAdd8;
			tDoMemOpm	= UMEM_OPM_RD_Q;
			tDoMemOp	= 1;
			tHeldIdRn1	= regIdRm;
			tHeldIdCn1	= regIdRm[4:0];

`ifdef jx2_debug_ldst
			$display("POP(1): SP=%X R=%X", tMemAddr, regIdRm);
`endif
		end

		JX2_UCMD_ALU3: begin
			tHeldIdRn1	= regIdRm;
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValAlu;
		end

		JX2_UCMD_ALUCMP: begin
//			tRegOutSr[0]	= tAluSrT;
		end
	
		JX2_UCMD_CONV_RR: begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= tValCnv;
			tRegOutSr[0]	= tCnvSrT;
		end
		JX2_UCMD_MOV_RC: begin
			tRegIdCn1	= regIdRm[4:0];
			tRegValCn1	= regValRs;
		end
		JX2_UCMD_MOV_CR: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= regValCRm;
		end
		JX2_UCMD_MOV_IR: begin
			case(opUIxt[1:0])
				2'b00: begin /* LDIx */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= {
						regValImm[32] ? UV32_FF : UV32_00,
						regValImm[31:0] };
				end
				2'b01: begin /* LDISH8 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[55:0], regValImm[7:0] };
					tRegValRn1	= { regValRs[55:0], regValImm[7:0] };
				end
				2'b10: begin /* LDISH16 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[47:0], regValImm[15:0] };
					tRegValRn1	= { regValRs[47:0], regValImm[15:0] };
				end
				2'b11: begin /* LDISH32 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[31:0], regValImm[31:0] };
					tRegValRn1	= { regValRs[31:0], regValImm[31:0] };
				end
			endcase
		end
	
		JX2_UCMD_BRA: begin
			tRegIdCn1	= JX2_CR_PC[4:0];
			tRegValCn1	= {UV32_00, tValAgu};
		end
		JX2_UCMD_BSR: begin
			tRegOutLr	= regValPc;
			tRegIdCn1	= JX2_CR_PC[4:0];
			tRegValCn1	= {UV32_00, tValAgu};
		end
		JX2_UCMD_JMP: begin
			tRegIdCn1	= JX2_CR_PC[4:0];
			tRegValCn1	= {UV32_00, regValRs[31:0]};
		end
		JX2_UCMD_JSR: begin
			tRegOutLr	= regValPc;
			tRegIdCn1	= JX2_CR_PC[4:0];
			tRegValCn1	= {UV32_00, regValRs[31:0]};
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
			tRegIdCn1	= JX2_GR_IMM[4:0];
		end

`ifdef jx2_enable_swapn
		JX2_UCMD_SWAPN: begin
			tRegIdRn1	= regIdRm;
			tRegValRn1	= tValSwapN;
		end
`endif

`ifdef jx2_enable_shlln
		JX2_UCMD_SHLLN:	 begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= tValShllN;
			tRegOutSr[0]	= tShllSrT;
		end
`endif
	
`ifdef jx2_enable_fpu
		JX2_UCMD_FSTCX: begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= regFpuGRn;
		end
		
		JX2_UCMD_FCMP: begin
			tRegOutSr[0]	= regFpuSrT;
		end

`ifdef jx2_enable_fprs
		JX2_UCMD_FPU3: begin
			if(opUIxt[4])
			begin
				tRegIdRn1		= regIdRm;
				tRegValRn1		= regFpuGRn;
			end
		end
		JX2_UCMD_FIXS: begin
			if(opUIxt[4])
			begin
				tRegIdRn1		= regIdRm;
				tRegValRn1		= regFpuGRn;
			end
		end
`else
		JX2_UCMD_FPU3: begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= regFpuGRn;
		end
		JX2_UCMD_FIXS: begin
			tRegIdRn1		= regIdRm;
			tRegValRn1		= regFpuGRn;
		end
`endif

`endif

		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end
				JX2_UCIX_IXT_SLEEP: begin
				end
				JX2_UCIX_IXT_BREAK: begin
					if(!tMsgLatch)
						$display("EX: BREAK");
					tNextMsgLatch	= 1;
					tExHold		= 1;
				end
				JX2_UCIX_IXT_CLRT: begin
					tRegOutSr[0]	= 0;
				end
				JX2_UCIX_IXT_SETT: begin
					tRegOutSr[0]	= 1;
				end
				JX2_UCIX_IXT_CLRS: begin
					tRegOutSr[1]	= 0;
				end
				JX2_UCIX_IXT_SETS: begin
					tRegOutSr[1]	= 1;
				end
				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXT %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
				end
			endcase
		end

		default: begin
			if(!tMsgLatch)
				$display("EX: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
			tExHold		= 1;
		end
	
	endcase
	
	if(tDoMemOp)
	begin
		tMemOpm = tDoMemOpm;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

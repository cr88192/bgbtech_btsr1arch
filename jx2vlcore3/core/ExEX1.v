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

`ifdef jx2_enable_vaddr48
// `include "ExAGUB.v"
`include "ExAGUC.v"
`else
`include "ExAGU.v"
`endif

// `include "ExALU.v"
`include "ExConv2R.v"

`include "ExCpuId.v"

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

/* verilator lint_off DEFPARAM */

module ExEX1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,	exTrapExc,
	timers,

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
	opPreBra,
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	regOutSchm,	regInSchm,
	
	memAddr,	memOpm,
	memDataOut,	memDataOutB,
	memDataOK
	);


input			clock;
input			reset;
input[7:0]		opUCmd;
input[7:0]		opUIxt;
output[1:0]		exHold;
output[15:0]	exTrapExc;

input[7:0]		timers;

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
	
input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)
input[63:0]		regFpuGRn;		//FPU GPR Result
input			regFpuSrT;
input			opBraFlush;
input			opPreBra;

parameter		isAltCore = 0;
defparam		cpuid.isAltCore = isAltCore;

output[63:0]	regOutDlr;
input[63:0]		regInDlr;
output[63:0]	regOutDhr;
input[63:0]		regInDhr;
output[63:0]	regOutSp;
input[63:0]		regInSp;

output[47:0]	regOutLr;
input[47:0]		regInLr;
output[63:0]	regOutSr;

(* max_fanout = 50 *)
	input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

output[47:0]	memAddr;
output[ 4:0]	memOpm;
output[63:0]	memDataOut;
output[63:0]	memDataOutB;

input[ 1:0]		memDataOK;


reg[ 5:0]		tRegIdRn1;		//Destination ID (EX1)
reg[63:0]		tRegValRn1;		//Destination Value (EX1)
reg[ 4:0]		tRegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		tRegValCn1;		//Destination Value (CR, EX1)
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[47:0]		tRegOutLr;
reg[63:0]		tRegOutSr;
reg[7:0]		tRegOutSchm;

reg[ 5:0]		tHeldIdRn1;		//Destination ID (EX1)
reg[ 4:0]		tHeldIdCn1;		//Destination ID (CR, EX1)

reg[15:0]		tExTrapExc;

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
assign	regOutSchm	= tRegOutSchm;
assign	exTrapExc	= tExTrapExc;

reg[47:0]		tMemAddr;
reg[ 4:0]		tMemOpm;
reg[63:0]		tMemDataOut;
reg[63:0]		tMemDataOutB;

assign	memAddr		= tMemAddr;
assign	memOpm		= tMemOpm;
assign	memDataOut	= tMemDataOut;
assign	memDataOutB	= tMemDataOutB;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

reg		tAguFlagJq;

`ifdef jx2_enable_vaddr48
wire[47:0]	tValAgu;
// ExAGUB	exAgu(regValRs[47:0], regValRt[47:0], opUIxt, tValAgu, tAguFlagJq);
ExAGUC	exAgu(regValRs[47:0], regValRt[47:0], opUIxt, tValAgu, tAguFlagJq);
`else
wire[47:0]	tValAgu;
assign	tValAgu[47:32] = UV16_00;
ExAGU	exAgu(regValRs[31:0], regValRt[31:0], opUIxt, tValAgu[31:0]);
`endif

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

wire[63:0]	tValCpuIdLo;
wire[63:0]	tValCpuIdHi;
ExCpuId		cpuid(clock, reset, timers,
	regIdRm[4:0], tValCpuIdLo, tValCpuIdHi);

reg[63:0]	tRegSpAdd8;
reg[63:0]	tRegSpSub8;
reg[63:0]	tRegSpAddImm;

reg[63:0]	tRegSpAdd16;
reg[63:0]	tRegSpSub16;

reg			tOpEnable;
reg			tDoMemOp;
reg[4:0]	tDoMemOpm;
reg			tDoDelayCycle;

(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;
reg[5:0]	tOpUCmdF;

reg[63:0]	tValAguBra;
reg[47:0]	tValBra;
reg			tDoBra;

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
	tRegOutSchm	= regInSchm;

	tMemAddr		= tValAgu;
	tMemOpm			= UMEM_OPM_READY;
	tMemDataOut		= regValRm;
	tMemDataOutB	= regValRt;
	
	tDoMemOpm		= UMEM_OPM_READY;
	tDoMemOp		= 0;
	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tExTrapExc		= 0;
	tDoDelayCycle	= 0;

	tValAguBra		= { UV16_00, regValPc[47:32], tValAgu[31:0] };
//	tValAguBra		= { UV16_00, regValPc[47:32],
//		regValPc[31:1] + regValRt[30:0], 1'b0 };

	tValBra			= tValAguBra[47:0];
	tDoBra			= 0;


	tRegSpAdd8		= { regInSp[63:28], regInSp[27:3]+25'h1, regInSp[2:0]};
	tRegSpSub8		= { regInSp[63:28], regInSp[27:3]-25'h1, regInSp[2:0]};
	tRegSpAdd16		= { regInSp[63:28], regInSp[27:4]+24'h1, regInSp[3:0]};
	tRegSpSub16		= { regInSp[63:28], regInSp[27:4]-24'h1, regInSp[3:0]};

`ifdef jx2_enable_addsp
	tRegSpAddImm	=  { regInSp[63:28],
//		regInSp[27:3]+regValImm[27:3],
//		regInSp[2:0]};
		regInSp[27:0] + {
			regValImm[15] ? UV12_FF : UV12_00,
			regValImm[15:3], 3'h0 } };
`endif

`ifndef def_true
//	case(opUIxt[7:6])
//	case(opUCmd[7:6])
	casez( { opBraFlush, opUCmd[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = regInSr[0];
		3'b011: 	tOpEnable = !regInSr[0];
		3'b1zz: 	tOpEnable = 0;
	endcase
`endif

`ifdef def_true
	casez( { opBraFlush, opUCmd[7:6], regInSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif
	
`ifndef def_true
	tOpUCmdF	= JX2_UCMD_NOP;
	if(!opBraFlush)
	begin
		case(opUCmd[5:0])
			JX2_UCMD_BRA:	tOpUCmdF	= JX2_UCMD_BRA_NB;
			default:		tOpUCmdF	= JX2_UCMD_NOP;
		endcase
	end
	
//	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : tOpUCmdF;
`endif

`ifdef def_true
	tOpUCmdF	= ((opUCmd[5:0] == JX2_UCMD_BRA) && !opBraFlush) ?
		JX2_UCMD_BRA_NB : JX2_UCMD_NOP ;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : tOpUCmdF;
`endif

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_INVOP: begin
			if(!tMsgLatch)
				$display("EX: Invalid Opcode");
			tNextMsgLatch	= 1;
			tExHold		= 1;
//			tExHold		= !reset;
		end
	
		JX2_UCMD_LEA_MR: begin
`ifdef jx2_addxl_agu
			tRegIdRn1	= regIdRm;
			if(opUIxt[2])
			begin
				tRegValRn1	= {
					(opUIxt[3] && tValAgu[31]) ?
						UV32_FF : UV32_00,
					tValAgu[31:0] };
			end
			else
			begin
				tRegValRn1	= { UV32_00, tValAgu };
			end
`else
			tRegIdRn1	= regIdRm;
			tRegValRn1	= { UV16_00, tValAgu };
`endif
		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
//			tDoMemOpm	= { 2'b10, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;

`ifdef jx2_debug_ldst
			$display("STORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
//			tDoMemOpm = { 2'b01, opUIxt[3], opUIxt[5:4] };
			tDoMemOp	= 1;
			tHeldIdRn1	= regIdRm;

`ifdef jx2_debug_ldst
			$display("LOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end

// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fmov
// `ifdef jx2_enable_fprs
		JX2_UCMD_FMOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
			tMemDataOut = (opUIxt[1:0]==3) ? regValFRs : regFpuGRn;
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
		end
`endif

`ifndef def_true
		JX2_UCMD_PUSHX: begin
			tMemAddr	= tRegSpSub8[47:0];
			tRegOutSp	= tRegSpSub8;
			tDoMemOpm	= UMEM_OPM_WR_Q;
			tDoMemOp	= 1;

//			tHeldIdRn1	= JX2_GR_SP;
			tHeldIdCn1	= JX2_GR_IMM[4:0];
			tRegOutSchm[JX2_SCHM_SP]	= 1;
			
			case(opUIxt[2:0])
//				3'b000: 	tMemDataOut = regValRm;
				3'b000: 	tMemDataOut = regValRs;
				3'b001: 	tMemDataOut = regValCRm;
				3'b010: 	tMemDataOut = regValFRs;
//				3'b011: 	tMemDataOut = regValRm;
//				3'b011: 	tMemDataOut = regValRs;
				3'b011: 	tMemDataOut = regValCRm;

				3'b100: begin
//					$display("EXA1: PushX Id=%d Rm=%X", regIdRm, regValRm);

					tMemAddr	= tRegSpSub16[47:0];
					tRegOutSp	= tRegSpSub16;
					tDoMemOpm	= UMEM_OPM_WR_TILE;
				 	tMemDataOut		= regValRm;
//				 	tMemDataOut		= regValRs;
//				 	tMemDataOutB	= regValRt;
				end
				
				default: begin
				end
			endcase
			
`ifdef jx2_debug_ldst
			$display("PUSH: SP=%X R=%X V=%X", tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_POPX: begin
			tMemAddr	= regInSp[47:0];
			tRegOutSp	= tRegSpAdd8;
			tDoMemOpm	= UMEM_OPM_RD_Q;
			tDoMemOp	= 1;
//			tHeldIdRn1	= regIdRm;
//			tHeldIdCn1	= regIdRm[4:0];

			tHeldIdCn1	= JX2_GR_IMM[4:0];
			tRegOutSchm[JX2_SCHM_SP]	= 1;

//			casez(opUIxt[1:0])
//				2'b00:		tHeldIdRn1	= regIdRm;
//				2'b01:		tHeldIdCn1	= regIdRm[4:0];
//				default:	tHeldIdCn1	= JX2_GR_IMM[4:0];
//			endcase

			case(opUIxt[2:0])
				3'b100: begin
					tRegOutSp	= tRegSpAdd16;
					tDoMemOpm	= UMEM_OPM_RD_TILE;
				end
				
				default: begin
				end
			endcase

`ifdef jx2_debug_ldst
			$display("POP(1): SP=%X R=%X", tMemAddr, regIdRm);
`endif
		end
`endif

`ifdef jx2_enable_addsp
		JX2_UCMD_ADDSP: begin
			tRegOutSp	= tRegSpAddImm;
			tHeldIdCn1	= JX2_GR_IMM[4:0];
			tRegOutSchm[JX2_SCHM_SP]	= 1;
		end
`endif

//		JX2_UCMD_ALU3: begin
		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
			tHeldIdRn1	= regIdRm;
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValAlu;
		end

		JX2_UCMD_ALUCMP: begin
//			tRegOutSr[0]	= tAluSrT;
//			tHeldIdCn1	= JX2_GR_IMM[4:0];
		end
	
`ifndef def_true
		JX2_UCMD_UNARY: begin
			tHeldIdRn1	= regIdRm;
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValAlu;
		end
`endif

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
			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= {
						regValImm[32] ? UV32_FF : UV32_00,
						regValImm[31:0] };
//					tRegValRn1	= regValRs;
				end
				4'b0001: begin /* LDISH8 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[55:0], regValImm[7:0] };
					tRegValRn1	= { regValRs[55:0], regValImm[7:0] };
				end
				4'b0010: begin /* LDISH16 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[47:0], regValImm[15:0] };
					tRegValRn1	= { regValRs[47:0], regValImm[15:0] };
				end
				4'b0011: begin /* LDISH32 */
					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRm[31:0], regValImm[31:0] };
					tRegValRn1	= { regValRs[31:0], regValImm[31:0] };
				end
				4'b0100: begin /* JLDIX */
					tRegIdRn1	= regIdRm;
					tRegValRn1	= regValRt;
				end
				
				default: begin
					$display("ExEX1: MOV_IR, Invalid UIxt %X", opUIxt);
					tRegIdRn1	= regIdRm;
					tRegValRn1	= regValRt;
				end
			endcase
		end

		JX2_UCMD_BRA_NB: begin
			if(opPreBra)
			begin
//				$display("EX: BRA_NB: PC2=%X", regValPc);
//				tRegIdCn1	= JX2_CR_PC[4:0];
//				tRegValCn1	= {UV16_00, regValPc};
				tValBra		= regValPc[47:0];
				tDoBra		= 1;
			end
		end
	
		JX2_UCMD_BRA: begin
			if(!opPreBra)
			begin
//				$display("EX: BRA: PC2=%X", tValAgu);
//				tRegIdCn1	= JX2_CR_PC[4:0];
//				tRegValCn1	= {UV16_00, tValAgu};
//				tRegValCn1	= tValAguBra;

				tValBra		= tValAguBra[47:0];
				tDoBra		= 1;
			end
		end
		JX2_UCMD_BSR: begin
//			$display("EX: BSR: LR=%X PC2=%X", regValPc, tValAgu);
			tRegOutLr	= regValPc;
			if(!opPreBra)
			begin
//				tRegIdCn1	= JX2_CR_PC[4:0];
//				tRegValCn1	= {UV16_00, tValAgu};
//				tRegValCn1	= tValAguBra;

				tValBra		= tValAguBra[47:0];
				tDoBra		= 1;
			end
		end
		JX2_UCMD_JMP: begin
//			$display("EX: JMP: PC2=%X", regValRs);
//			tRegIdCn1	= JX2_CR_PC[4:0];
`ifdef jx2_enable_vaddr48
//			tRegValCn1	= {UV16_00, regValRs[47:0]};
`else
//			tRegValCn1	= {UV32_00, regValRs[31:0]};
`endif
			tValBra		= regValRs[47:0];
			tDoBra		= 1;
		end
		JX2_UCMD_JSR: begin
//			$display("EX: JSR: LR=%X PC2=%X", regValRs, regValPc);
			tRegOutLr	= regValPc;
//			tRegIdCn1	= JX2_CR_PC[4:0];
`ifdef jx2_enable_vaddr48
//			tRegValCn1	= {UV16_00, regValRs[47:0]};
`else
//			tRegValCn1	= {UV32_00, regValRs[31:0]};
`endif
			tValBra		= regValRs[47:0];
			tDoBra		= 1;
		end
		
		JX2_UCMD_MULW3: begin
			tHeldIdRn1	= regIdRm;			//
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
//			tRegIdCn1	= JX2_GR_IMM[4:0];
//			tHeldIdCn1	= JX2_GR_IMM[4:0];
//			tRegOutSchm[JX2_SCHM_DLR]	= 1;
//			tRegOutSchm[JX2_SCHM_DHR]	= 1;
			tHeldIdRn1	= regIdRm;

`ifndef def_true
			casez(opUIxt[2:0])
				3'b00z: begin
					tHeldIdRn1	= regIdRm;
				end
`ifndef def_true
				3'b01z: begin
					tDoDelayCycle = 1;
//					tRegIdCn1	= JX2_GR_IMM[4:0];
		//			tHeldIdCn1	= JX2_GR_IMM[4:0];
					tRegOutSchm[JX2_SCHM_DLR]	= 1;
					tRegOutSchm[JX2_SCHM_DHR]	= 1;
				end
`endif
				3'b10z: begin
					tHeldIdRn1	= regIdRm;
				end
				default: begin
					tHeldIdRn1	= regIdRm;
				end
			endcase
`endif

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
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= regFpuGRn;
			tHeldIdRn1	= regIdRm;
		end
		
		JX2_UCMD_FCMP: begin
//			tRegOutSr[0]	= regFpuSrT;
		end

`ifdef jx2_enable_fprs
		JX2_UCMD_FPU3: begin
//			if(opUIxt[4])
//			begin
//				tRegIdRn1		= regIdRm;
//				tRegValRn1		= regFpuGRn;
//			end
		end
		JX2_UCMD_FIXS: begin
//			if(opUIxt[4])
//			begin
//				tRegIdRn1		= regIdRm;
//				tRegValRn1		= regFpuGRn;
//			end
		end
		JX2_UCMD_FLDCX: begin
//			if(opUIxt[4])
//			begin
//				tRegIdRn1		= regIdRm;
//				tRegValRn1		= regFpuGRn;
//			end
		end
`else
		JX2_UCMD_FPU3: begin
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= regFpuGRn;
			tHeldIdRn1	= regIdRm;
		end
		JX2_UCMD_FIXS: begin
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= regFpuGRn;
			tHeldIdRn1	= regIdRm;
		end

		JX2_UCMD_FLDCX: begin
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= regFpuGRn;
			tHeldIdRn1	= regIdRm;
		end
`endif

`endif
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

				JX2_UCIX_IXS_INVIC: begin
					tMemOpm		= UMEM_OPM_FLUSHIS;
					tMemAddr	= regValRm[47:0];

//					tRegOutLr	= regValPc;
//					tRegIdCn1	= JX2_CR_PC[4:0];
//					tRegValCn1	= {UV16_00, regValPc};
					tValBra		= regValPc[47:0];
					tDoBra		= 1;

				end
				JX2_UCIX_IXS_INVDC: begin
					tMemOpm		= UMEM_OPM_FLUSHDS;
					tMemAddr	= regValRm[47:0];
				end

				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXS %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
//					tExHold		= !reset;
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
						$display("EX: BREAK, PC=%X", regValPc);
					tNextMsgLatch	= 1;
					tExHold		= 1;
//					tExHold		= !reset;
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

				JX2_UCIX_IXT_NOTT: begin
					tRegOutSr[0]	= !regInSr[0];
				end
				JX2_UCIX_IXT_NOTS: begin
					tRegOutSr[1]	= !regInSr[1];
				end

				JX2_UCIX_IXT_RTE: begin
					$display("EX1: RTE, PC=%X", regValPc);
					tExTrapExc = 16'hFF00;
//					tRegIdCn1	= JX2_GR_IMM[4:0];
				end
				JX2_UCIX_IXT_TRAPA: begin
					tExTrapExc = { 12'hC08, regIdRm[3:0] };
//					tRegIdCn1	= JX2_GR_IMM[4:0];
				end

				JX2_UCIX_IXT_CPUID: begin
//					tRegOutDlr	= tValCpuIdLo;
//					tRegOutDhr	= tValCpuIdHi;
//					tRegIdCn1	= JX2_GR_IMM[4:0];

					tRegIdRn1		= JX2_GR_DLR;
					tRegValRn1		= tValCpuIdLo;
				end

				JX2_UCIX_IXT_WEXMD: begin
`ifdef jx2_enable_wex
					case(regIdRm[3:0])
						4'h0:		tRegOutSr[27]	= 0;
						4'h1:		tRegOutSr[27]	= 1;
`ifdef jx2_enable_wex3w
						4'h2:		tRegOutSr[27]	= 1;
`endif
						default:	tRegOutSr[27]	= 0;
					endcase
`else
					tRegOutSr[27]	= 0;
`endif

//					tRegOutSr[27]	= 0;
				end

				JX2_UCIX_IXT_SYSE: begin
					tExTrapExc = { 4'hE, regInDlr[11:0] };
//					tRegIdCn1	= JX2_GR_IMM[4:0];
				end
				
				JX2_UCIX_IXT_LDTLB: begin
					tMemOpm = UMEM_OPM_LDTLB;
				end
				JX2_UCIX_IXT_INVTLB: begin
					tMemOpm = UMEM_OPM_INVTLB;
				end

				JX2_UCIX_IXT_LDEKRR: begin
				end
				JX2_UCIX_IXT_LDEKEY: begin
				end
				JX2_UCIX_IXT_LDEENC: begin
				end

				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXT %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
//					tExHold		= !reset;
				end
			endcase
		end

		default: begin
			if(!tMsgLatch)
				$display("EX1: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
			tExHold		= 1;
//			tExHold		= !reset;
		end
	
	endcase
	
	if(tDoMemOp)
	begin
		tMemOpm			= tDoMemOpm;
`ifndef	jx2_mem_l1dstall
		if(memDataOK == UMEM_OK_HOLD)
			tExHold		= 1;
`endif
	end
	
	if(tDoDelayCycle)
	begin
		tRegIdCn1	= JX2_GR_IMM[4:0];
	end
	
	if(tDoBra)
	begin
		tRegIdCn1	= JX2_CR_PC[4:0];
		tRegValCn1	= {UV16_00, tValBra};
	end

	if(tHeldIdRn1 != JX2_GR_ZZR)
		tRegHeld		= 1;

end

always @(posedge clock)
begin
	tAguFlagJq	<= regInSr[31];
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

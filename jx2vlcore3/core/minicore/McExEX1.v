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

`include "ExAGUC.v"

`include "ExConv2R.v"

`include "ExCpuId.v"

`include "ExShad64D.v"

/* verilator lint_off DEFPARAM */

module McExEX1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,	exTrapExc,
	timers,	opUCmdOut,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regValXs,		//Source A, 128-bit

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
	opPreBraPc,
	opPreBra,
	aluSrJcmpT,

	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutSp,	regInSp,
	regOutLr,	regInLr,
	regOutSr,	regInSr,
	regOutSchm,	regInSchm,
	
	memAddr,	memOpm,
	memDataOut,	memDataOutB,
	memDataOK,	regInExc
	);


input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;
output[127:0]	exTrapExc;

input[11:0]		timers;
output[8:0]		opUCmdOut;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
input[63:0]		regValRs;		//Source A Value
input[63:0]		regValRt;		//Source B Value
input[63:0]		regValRm;		//Source C Value

input[63:0]		regValXs;		//Source C Value

input[63:0]		regValCRm;		//Source C Value (CR)

`output_gpr		regIdRn1;		//Destination ID (EX1)
output[63:0]	regValRn1;		//Destination Value (EX1)
`output_gpr		regIdCn1;		//Destination ID (CR, EX1)
output[63:0]	regValCn1;		//Destination Value (CR, EX1)
`output_gpr		heldIdRn1;		//Held Destination ID (EX1)
`output_gpr		heldIdCn1;		//Held Destination ID (CR, EX1)
	
input[63:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)
input[63:0]		regFpuGRn;		//FPU GPR Result
input			regFpuSrT;
input			opBraFlush;
input[47:0]		opPreBraPc;
input[1:0]		opPreBra;
input			aluSrJcmpT;

parameter		isAltCore = 0;
defparam		cpuid.isAltCore = isAltCore;

output[63:0]	regOutDlr;
input[63:0]		regInDlr;
output[63:0]	regOutDhr;
input[63:0]		regInDhr;
output[63:0]	regOutSp;
input[63:0]		regInSp;

output[63:0]	regOutLr;
input[63:0]		regInLr;
output[63:0]	regOutSr;

// (* max_fanout = 50 *)
	input[63:0]		regInSr;

output[7:0]		regOutSchm;
input[7:0]		regInSchm;

// output[47:0]	memAddr;
`output_mvaddr	memAddr;
output[ 4:0]	memOpm;
output[63:0]	memDataOut;
output[63:0]	memDataOutB;

input[ 1:0]		memDataOK;

input[63:0]		regInExc;


`reg_gpr		tRegIdRn1;		//Destination ID (EX1)
reg[63:0]		tRegValRn1;		//Destination Value (EX1)
`reg_gpr		tRegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		tRegValCn1;		//Destination Value (CR, EX1)
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[63:0]		tRegOutLr;
reg[63:0]		tRegOutSr;
reg[7:0]		tRegOutSchm;

`reg_gpr		tHeldIdRn1;		//Destination ID (EX1)
`reg_gpr		tHeldIdCn1;		//Destination ID (CR, EX1)

reg[127:0]		tExTrapExc;

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

`reg_mvaddr		tMemAddr;
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

wire[47:0]	tValAgu;
// ExAGUB	exAgu(regValRs[47:0], regValRt[47:0], opUIxt, tValAgu, tAguFlagJq);
ExAGUC	exAgu(
	regValRs[47:0], regValRt[47:0], regValImm[15:0],
	opUIxt, tValAgu, tAguFlagJq);

wire[63:0]	tValCnv;
wire		tCnvSrT;
ExConv2R	exConv2R(regValRs, opUIxt, regInSr[0], tValCnv, tCnvSrT);

wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64D	exShad64(clock, reset,
	regValRs[63:0],
	regValXs[63:0],
	regValRt[7:0],
	tValShad64,
	opUIxt[5:0], 0);


wire[63:0]	tValCpuIdLo;
wire[63:0]	tValCpuIdHi;
ExCpuId		cpuid(clock, reset, timers,
	regIdRt[4:0], tValCpuIdLo, tValCpuIdHi);

reg			tOpEnable;
reg			tDoMemOp;
reg[4:0]	tDoMemOpm;
reg			tDoDelayCycle;

(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;
reg[5:0]	tOpUCmdF;
reg[8:0]	tOpUCmd2;

assign		opUCmdOut = tOpUCmd2;

reg[47:0]	tValBraDispSc;

reg[16:0]	tValAguBraA0;
reg[16:0]	tValAguBraB0;
reg[16:0]	tValAguBraB1;
reg[16:0]	tValAguBraC0;
reg[16:0]	tValAguBraC1;

reg[63:0]	tRegBraLr;
reg[63:0]	tValAguBra;

reg[15:0]	tValAguBraJCmpMi;
reg[15:0]	tValAguBraJCmpLo;
reg[47:0]	tValAguBraJCmp;

reg[63:0]	tValBra;
reg[63:0]	tValBraHi;
reg			tDoBra;

reg[63:0]	tValOutDfl;
reg			tDoOutDfl;

reg			tTempBit0;
reg			tTempBit1;
reg			tTempBit2;
reg			tTempBit3;

reg[3:0]	tIfThenPP;
reg[3:0]	tIfThenNN;
reg[3:0]	tIfElsePP;
reg[3:0]	tIfElseNN;

reg			tXmovEnable;
reg			tXmovAdd;

reg tMsgLatch;
reg tNextMsgLatch;

always @*
begin

	tRegIdRn1	= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1	= UV64_XX;			//Destination Value (EX1)
	tRegIdCn1	= JX2_CR_ZZR;	//Destination ID (CR, EX1)
	tRegValCn1	= UV64_XX;			//Destination Value (CR, EX1)
	
	tHeldIdRn1	= JX2_GR_ZZR;
	tHeldIdCn1	= JX2_CR_ZZR;

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

	tValOutDfl		= UV64_XX;
	tDoOutDfl		= 0;

	tValAguBraJCmpMi = regValPc[31:16] + 1;
	tValAguBraJCmpLo = { 1'b0, regValPc[15:1] } + { 1'b0, regValImm[14:0] };
	tValAguBraJCmp = {
		regValPc[47:32],
		tValAguBraJCmpLo[15] ? tValAguBraJCmpMi : regValPc[31:16],
		tValAguBraJCmpLo[14:0],
		1'b0 };

	tValAguBra		= { UV16_00, tValAgu[47:0] };
	if(!tAguFlagJq)
		tValAguBra[47:32] = regValPc[47:32];

`ifdef jx2_agu_bra32

	tValBraDispSc	= { regValRt[46:0], 1'b0 };
	tValAguBraA0	=
		{ 1'b0, regValPc[15:0] } +
		{ 1'b0, tValBraDispSc[15:0] };
	tValAguBraB0	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 0;
	tValAguBraB1	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 1;

	tValAguBra		= {
		UV16_00,
		regValPc[47:32],
		tValAguBraA0[16] ?
			tValAguBraB1[15:0] : tValAguBraB0[15:0],
		tValAguBraA0[15:0]
		};
`endif

`ifdef jx2_agu_bra48
	tValBraDispSc	= { regValRt[46:0], 1'b0 };
	tValAguBraA0	=
		{ 1'b0, regValPc[15:0] } +
		{ 1'b0, tValBraDispSc[15:0] };
	tValAguBraB0	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 0;
	tValAguBraB1	=
		{ 1'b0, regValPc[31:16] } +
		{ 1'b0, tValBraDispSc[31:16] } + 1;
	tValAguBraC0	=
		{ 1'b0, regValPc[47:32] } +
		{ 1'b0, tValBraDispSc[47:32] } + 0;
	tValAguBraC1	=
		{ 1'b0, regValPc[47:32] } +
		{ 1'b0, tValBraDispSc[47:32] } + 1;
	tValAguBra		= {
		UV16_00,
		( tValAguBraA0[16] ? tValAguBraB1[16] : tValAguBraB0[16] ) ?
			tValAguBraC1[15:0] : tValAguBraC0[15:0],
		tValAguBraA0[16] ?
			tValAguBraB1[15:0] : tValAguBraB0[15:0],
		tValAguBraA0[15:0]
		};
	if(!tAguFlagJq)
		tValAguBra[47:32] = regValPc[47:32];
`endif

	tRegBraLr	= regValPc;

	tValBra			= { regValPc[63:48], tValAguBra[47:0] };
	tDoBra			= 0;

	tTempBit0		= 1'bX;
	tTempBit1		= 1'bX;
	tTempBit2		= 1'bX;
	tTempBit3		= 1'bX;

	tIfThenPP		= regInSr[11: 8] + 1;
	tIfThenNN		= regInSr[11: 8] - 1;
	tIfElsePP		= regInSr[15:12] + 1;
	tIfElseNN		= regInSr[15:12] - 1;

	casez( { opBraFlush, opUCmd[7:6], regInSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase

	tOpUCmdF	= ((opUCmd[5:0] == JX2_UCMD_BRA) && !opBraFlush) ?
		JX2_UCMD_BRA_NB : JX2_UCMD_NOP ;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : tOpUCmdF;
	
	tOpUCmd2	= { JX2_IXC_AL, tOpUCmd1 };

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_INVOP: begin
			if(!tMsgLatch)
				$display("EX1: Invalid Opcode %X", tOpUCmd1);
			tNextMsgLatch	= 1;
			tExHold		= 1;
		end
	
		JX2_UCMD_LEA_MR: begin
			tValOutDfl		= { UV16_00, tValAgu };
			tDoOutDfl		= 1;
		end
		JX2_UCMD_MOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;

`ifdef jx2_debug_ldst
			$display("STORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;

`ifdef jx2_debug_ldst
			$display("LOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3,
		JX2_UCMD_CONV2_RR, JX2_UCMD_ALUB3: begin
			tRegHeld		= 1;
		end

		JX2_UCMD_ALUCMP, JX2_UCMD_ALUCMPW, JX2_UCMD_ALUCMPB: begin
		end
	
		JX2_UCMD_CONV_RR: begin
			tValOutDfl		= tValCnv;
			tDoOutDfl		= 1;
			tRegOutSr[0]	= tCnvSrT;
		end
		JX2_UCMD_MOV_RC: begin
			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_MOV_CR: begin
			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_MOV_IR: begin
			tValOutDfl		= regValRt;
			tDoOutDfl		= 1;

			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
				end
				4'b0001: begin /* LDISH8 */
					tValOutDfl	= { regValRs[55:0], regValRt[7:0] };
				end
				4'b0010: begin /* LDISH16 */
					tValOutDfl	= { regValRs[47:0], regValRt[15:0] };
				end
				4'b0011: begin /* LDISH32 */
					tValOutDfl	= { regValRs[31:0], regValRt[31:0] };
				end
				4'b0100: begin /* JLDIX */
				end
				
				default: begin
					$display("ExEX1: MOV_IR, Invalid UIxt %X", opUIxt);
				end
			endcase
		end

		JX2_UCMD_BRA_NB: begin
			if(opPreBra!=2'b00)
			begin
				tValBra		= regValPc[63:0];
				tDoBra		= 1;
			end
		end
	
		JX2_UCMD_BRA: begin
			if(opPreBra!=2'b01)
			begin
				tValBra		= { tRegBraLr[63:48], tValAguBra[47:0] };
				tDoBra		= 1;
			end
		end

		JX2_UCMD_BSR: begin
`ifdef jx2_enable_riscv
			tValOutDfl		= tRegBraLr;
			tDoOutDfl		= 1;

			if(regIdRm==JX2_GR_DLR)
			begin
				tRegOutLr	= tRegBraLr;
				tDoOutDfl	= 0;
			end
`endif

			if(opPreBra!=2'b01)
			begin
				tValBra		= { regValPc[63:48], tValAguBra[47:0] };
				tDoBra		= 1;
			end
		end

		JX2_UCMD_JMP: begin
			tValBra		= { regValPc[63:48], regValRs[47:0] };
			tDoBra		= (opPreBra != 2'b01);

`ifndef jx2_enable_riscv
// `ifdef def_true
			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
				tValBra[63:48] = regValRs[63:48];
			end
`endif

`ifdef jx2_enable_riscv
// `ifndef def_true
			tValBra		= { regValPc[63:48], tValAgu[47:0] };

			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
				tValBra[63:48] = regValRs[63:48];
			end

			if(regInSr[26])
			begin
				$display("EX: JMP: RVI PC=%X", tValBra);
			end
			
			if(tValAgu[0])
			begin
				$display("EX: JMP: Inter-ISA RV=%d Ixt=%X Rs=%X Tgt-PC=%X",
					regInSr[26], opUIxt,
					regValRs, tValBra);
				tRegOutSr[26]	= !regInSr[26];
				tRegOutSr[27]	= tValAgu[1];
				tValBra[1:0]	= 0;
			end
`endif
		end
		JX2_UCMD_JSR: begin

`ifndef jx2_enable_riscv
			tRegOutLr	= tRegBraLr;
			tValBra		= { regValPc[63:48], regValRs[47:0] };
			tDoBra		= 1;
`endif

`ifdef jx2_enable_riscv
// `ifndef def_true
			tValBra		= { regValPc[63:48], tValAgu[47:0] };
			tValOutDfl	= tRegBraLr;
			tDoOutDfl	= 1;
			tDoBra		= 1;

			if(regIdRm==JX2_GR_DLR)
			begin
				tRegOutLr	= tRegBraLr;
				tDoOutDfl	= 0;
			end

			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
				tValBra[63:48] = regValRs[63:48];
			end

			if(tValAgu[0])
			begin
				$display("EX: JSR: Inter-ISA %d PC=%X", regInSr[26], tValBra);
				tRegOutSr[26]	= !regInSr[26];
				tRegOutSr[27]	= tValAgu[1];
				tValBra[1:0]	= 0;
			end
`endif
		end

`ifdef jx2_alu_jcmp
		JX2_UCMD_JCMP: begin
			if(aluSrJcmpT)
			begin
				tValBra		= { tRegBraLr[63:48], tValAguBraJCmp[47:0] };
				tDoBra		= 1;
			end
		end
`endif

		JX2_UCMD_MULW3: begin
//			tHeldIdRn1	= regIdRm;			//
			tRegHeld		= 1;
		end

`ifdef jx2_merge_shadq
//		JX2_UCMD_SHAD3, JX2_UCMD_SHLD3,
//		JX2_UCMD_SHADQ3, JX2_UCMD_SHLDQ3:
		JX2_UCMD_SHAD3:
		begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValShad64;
			tValOutDfl	= tValShad64;
			tDoOutDfl	= 1;
		end
`else
		JX2_UCMD_SHAD3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
			tValOutDfl		= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLD3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= { UV32_00, tValShad32 };
			tValOutDfl		= { UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
	
		JX2_UCMD_SHADQ3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValShad64;
			tValOutDfl		= tValShad64;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLDQ3: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= tValShad64;
			tValOutDfl		= tValShad64;
			tDoOutDfl		= 1;
		end

`endif

		JX2_UCMD_MUL3: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
	
`ifdef jx2_enable_fpu
		JX2_UCMD_FSTCX: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
		
		JX2_UCMD_FCMP: begin
		end

		JX2_UCMD_FPU3: begin
			tRegHeld		= 1;
		end
		JX2_UCMD_FIXS: begin
			tRegHeld		= 1;
		end

		JX2_UCMD_FLDCX: begin
			tRegHeld		= 1;
		end
`endif

		JX2_UCMD_OP_IXS: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXS_NOP: begin
				end
				JX2_UCIX_IXS_MOVT: begin
					tValOutDfl		= {UV63_00, regInSr[0]};
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_MOVNT: begin
					tValOutDfl		= {UV63_00, !regInSr[0]};
					tDoOutDfl		= 1;
				end

				JX2_UCIX_IXS_MOVST: begin
					tValOutDfl		= {UV62_00, regInSr[1:0]};
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_MOVPQ: begin
					tValOutDfl		= {UV60_00, regInSr[7:4]};
					tDoOutDfl		= 1;
				end

				JX2_UCIX_IXS_INVIC: begin
					$display("EX1 JX2_UCIX_IXS_INVIC");
					tMemOpm			= UMEM_OPM_FLUSHIS;
					tMemAddr[47:0]	= regValRm[47:0];
					tValBra			= regValPc;
					tDoBra			= 1;
				end
				JX2_UCIX_IXS_INVDC: begin
					$display("EX1 JX2_UCIX_IXS_INVDC");
					tMemOpm			= UMEM_OPM_FLUSHDS;
					tMemAddr[47:0]	= regValRm[47:0];
				end

				JX2_UCIX_IXS_TRAPB: begin
					tDoMemOpm	= UMEM_OPM_TRAP;
					tDoMemOp	= 1;
				end

				default: begin
					if(!tMsgLatch)
						$display("EX: Unhandled Op-IXS %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
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
					if(regInExc[15])
						tExHold		= 0;
					
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
//					$display("EX1: RTE, PC=%X", regValPc);
					tExTrapExc = { UV112_00, 16'hFF00 };
				end
				JX2_UCIX_IXT_TRAPA: begin
					tExTrapExc = { UV112_00, 12'hC08, regIdRm[3:0] };
				end

				JX2_UCIX_IXT_CPUID: begin
//					tRegIdRn1		= JX2_GR_DLR;
//					tRegValRn1		= tValCpuIdLo;
					tValOutDfl		= tValCpuIdLo;
					tDoOutDfl		= 1;
				end

				JX2_UCIX_IXT_WEXMD: begin
					tRegOutSr[27]	= 0;
				end

				JX2_UCIX_IXT_SYSE: begin
					tExTrapExc = { UV112_00, 4'hE, regInDlr[11:0] };
				end
				
				JX2_UCIX_IXT_LDTLB: begin
					tDoMemOpm	= UMEM_OPM_LDTLB;
					tDoMemOp	= 1;
				end
				JX2_UCIX_IXT_INVTLB: begin
					tDoMemOpm	= UMEM_OPM_INVTLB;
					tDoMemOp	= 1;
				end

				JX2_UCIX_IXT_LDACL: begin
					tDoMemOpm	= UMEM_OPM_LDACL;
					tDoMemOp	= 1;
				end

				JX2_UCIX_IXT_SXENTR: begin
					tRegOutSr[30:29]	= 2'b01;
					tValBra				= regValPc;
					tDoBra				= 1;
				end
				JX2_UCIX_IXT_SUENTR: begin
					tRegOutSr[30:29]	= 2'b00;
					tValBra				= regValPc;
					tDoBra				= 1;
				end
				JX2_UCIX_IXT_SVENTR: begin
					tRegOutSr[30:29]	= 2'b10;
					tValBra				= regValPc;
					tDoBra				= 1;
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
				$display("EX1: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
			tExHold		= 1;
		end
	
	endcase
	
	if(tDoOutDfl)
	begin
		tRegIdRn1		= regIdRm;
		tRegValRn1		= tValOutDfl;
	end
	
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
//		tRegIdCn1	= JX2_GR_IMM[4:0];
		tRegIdCn1	= JX2_GR_IMM;
	end
	
	if(tDoBra)
	begin
//		tRegIdCn1	= JX2_CR_PC[4:0];
		tRegIdCn1	= JX2_CR_PC;
//		tRegValCn1	= {UV16_00, tValBra};
		tRegValCn1	= tValBra;
	end

	if(opBraFlush)
	begin
		tRegIdRn1	= JX2_GR_ZZR;
//		tRegIdCn1	= JX2_CR_ZZR[4:0];
		tRegIdCn1	= JX2_CR_ZZR;
	end

	if(tRegHeld)
		tHeldIdRn1	= regIdRm;

end

always @(posedge clock)
begin
	tAguFlagJq	<= regInSr[31];
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

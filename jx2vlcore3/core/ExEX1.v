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

`include "ExConv2R.v"

`include "ExCpuId.v"

`ifdef jx2_merge_shadfn
`include "ExShad64D.v"
`endif

`ifndef jx2_merge_shadfn
`ifndef jx2_merge_shadq
`include "ExShad32B.v"
`include "ExShad64B.v"
`endif

`ifdef jx2_merge_shadq
`include "ExShad64C.v"
`endif
`endif

`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
`include "ExBtcUtx1.v"
`endif
`endif

`ifdef jx2_enable_fmov
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
`endif

/* verilator lint_off DEFPARAM */

module ExEX1(
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

//	regValFRs,		//Source A Value (FPR)
//	regValFRt,		//Source B Value (FPR)
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
output[15:0]	exTrapExc;

input[11:0]		timers;
output[8:0]		opUCmdOut;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
input[63:0]		regValRs;		//Source A Value
input[63:0]		regValRt;		//Source B Value
input[63:0]		regValRm;		//Source C Value

input[63:0]		regValXs;		//Source C Value

// input[63:0]		regValFRs;		//Source A Value (FPR)
// input[63:0]		regValFRt;		//Source B Value (FPR)
input[63:0]		regValCRm;		//Source C Value (CR)

`output_gpr		regIdRn1;		//Destination ID (EX1)
output[63:0]	regValRn1;		//Destination Value (EX1)
// output[4:0]		regIdCn1;		//Destination ID (CR, EX1)
`output_gpr		regIdCn1;		//Destination ID (CR, EX1)
output[63:0]	regValCn1;		//Destination Value (CR, EX1)
`output_gpr		heldIdRn1;		//Held Destination ID (EX1)
// output[4:0]		heldIdCn1;		//Held Destination ID (CR, EX1)
`output_gpr		heldIdCn1;		//Held Destination ID (CR, EX1)
	
// input[47:0]		regValPc;		//PC Value (Synthesized)
input[63:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)
input[63:0]		regFpuGRn;		//FPU GPR Result
input			regFpuSrT;
input			opBraFlush;
input[47:0]		opPreBraPc;
input[1:0]		opPreBra;

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

output[47:0]	memAddr;
output[ 4:0]	memOpm;
output[63:0]	memDataOut;
output[63:0]	memDataOutB;

input[ 1:0]		memDataOK;

input[63:0]		regInExc;


`reg_gpr		tRegIdRn1;		//Destination ID (EX1)
reg[63:0]		tRegValRn1;		//Destination Value (EX1)
//reg[ 4:0]		tRegIdCn1;		//Destination ID (CR, EX1)
`reg_gpr		tRegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		tRegValCn1;		//Destination Value (CR, EX1)
reg[63:0]		tRegOutDlr;
reg[63:0]		tRegOutDhr;
reg[63:0]		tRegOutSp;
reg[63:0]		tRegOutLr;
reg[63:0]		tRegOutSr;
reg[7:0]		tRegOutSchm;

`reg_gpr		tHeldIdRn1;		//Destination ID (EX1)
//reg[ 4:0]		tHeldIdCn1;		//Destination ID (CR, EX1)
`reg_gpr		tHeldIdCn1;		//Destination ID (CR, EX1)

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

wire[63:0]	tValCnv;
wire		tCnvSrT;
ExConv2R	exConv2R(regValRs, opUIxt, regInSr[0], tValCnv, tCnvSrT);

`ifdef jx2_merge_shadfn

wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64D	exShad64(clock, reset,
	regValRs[63:0],
	regValXs[63:0],
	regValRt[7:0],
	tValShad64,
	opUIxt[5:0], 0);

`endif


`ifndef jx2_merge_shadfn

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
	tValShad64, opUIxt[3:0]);
`endif

`endif

`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
wire[63:0]	tValUtx1;
ExBtcUtx1	exUtx1(
	regValRs[63:0],
	regValXs[63:0],
	regValRt[3:0],
	opUIxt, tValUtx1);
`endif
`endif


wire[63:0]	tValCpuIdLo;
wire[63:0]	tValCpuIdHi;
ExCpuId		cpuid(clock, reset, timers,
//	regIdRm[4:0], tValCpuIdLo, tValCpuIdHi);
	regIdRt[4:0], tValCpuIdLo, tValCpuIdHi);

// reg[63:0]	tRegSpAdd8;
// reg[63:0]	tRegSpSub8;
// reg[63:0]	tRegSpAddImm;

// reg[63:0]	tRegSpAdd16;
// reg[63:0]	tRegSpSub16;

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
// reg[47:0]	tValBra;
reg[63:0]	tValBra;
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

reg tMsgLatch;
reg tNextMsgLatch;

`ifdef jx2_enable_fmov
wire[31:0]	regValRm_D2S;		//memory data (Double to Single)
wire[15:0]	regValRm_D2H;		//memory data (Double to Half)
FpuConvD2S	mem_cnv_d2s(regValRm[63:0], regValRm_D2S);
FpuConvD2H	mem_cnv_d2h(regValRm[63:0], regValRm_D2H);
`endif

always @*
begin

	tRegIdRn1	= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1	= UV64_XX;			//Destination Value (EX1)
//	tRegIdCn1	= JX2_CR_ZZR[4:0];	//Destination ID (CR, EX1)
	tRegIdCn1	= JX2_CR_ZZR;	//Destination ID (CR, EX1)
	tRegValCn1	= UV64_XX;			//Destination Value (CR, EX1)
	
	tHeldIdRn1	= JX2_GR_ZZR;
//	tHeldIdCn1	= JX2_CR_ZZR[4:0];
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

//	tValAguBra		= { UV16_00, regValPc[47:32], tValAgu[31:0] };
//	tValAguBra		= { UV16_00, tValAgu };

	tValAguBra		= { UV16_00, tValAgu[47:0] };
	if(!tAguFlagJq)
		tValAguBra[47:32] = regValPc[47:32];

//	tValAguBra		= { UV16_00, regValPc[47:32],
//		regValPc[31:0] + { regValRt[30:0], 1'b0 } };

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

//	tRegBraLr	= {
//		regInSr[15: 4],
//		regInSr[27:26],
//		regInSr[ 1: 0],
//		regValPc };
	tRegBraLr	= regValPc;

//	tValBra			= tValAguBra[47:0];
//	tValBra			= { tRegBraLr[63:48], tValAguBra[47:0] };
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

//	tRegSpAdd8		= { regInSp[63:28], regInSp[27:3]+25'h1, regInSp[2:0]};
//	tRegSpSub8		= { regInSp[63:28], regInSp[27:3]-25'h1, regInSp[2:0]};
//	tRegSpAdd16		= { regInSp[63:28], regInSp[27:4]+24'h1, regInSp[3:0]};
//	tRegSpSub16		= { regInSp[63:28], regInSp[27:4]-24'h1, regInSp[3:0]};

`ifdef jx2_enable_pred_s
	casez( { opBraFlush, opUCmd[8:6], regInSr[1:0] } )
		6'b0000zz: 	tOpEnable = 1;
		6'b0001zz: 	tOpEnable = 0;
		6'b0010z0: 	tOpEnable = 0;
		6'b0010z1: 	tOpEnable = 1;
		6'b0011z0: 	tOpEnable = 1;
		6'b0011z1: 	tOpEnable = 0;
		6'b01000z: 	tOpEnable = 0;
		6'b01001z: 	tOpEnable = 1;
		6'b01010z: 	tOpEnable = 1;
		6'b01011z: 	tOpEnable = 0;
		6'b0110zz: 	tOpEnable = 1;
		6'b0111zz: 	tOpEnable = 1;
		6'b1zzzzz: 	tOpEnable = 0;
	endcase
`else
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
//			tExHold		= !reset;
		end
	
		JX2_UCMD_LEA_MR: begin
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= { UV16_00, tValAgu };
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

`ifdef jx2_enable_fmov
		JX2_UCMD_FMOV_RM: begin
			tDoMemOpm	= { 2'b10, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
			
			tMemDataOut[31:0] = regValRm_D2S;
`ifdef jx2_enable_fmovh
			if(opUIxt[4])
				tMemDataOut[15:0] = regValRm_D2H;
`endif

`ifdef jx2_debug_ldst
			$display("FSTORE(1): A=%X R=%X V=%X",
				tMemAddr, regIdRm, tMemDataOut);
`endif
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOpm = { 2'b01, opUIxt[2], opUIxt[5:4] };
			tDoMemOp	= 1;
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;

`ifdef jx2_debug_ldst
			$display("FLOAD(1): A=%X R=%X",
				tMemAddr, regIdRm);
`endif
		end
`endif

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3,
		JX2_UCMD_CONV2_RR, JX2_UCMD_ALUB3: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

		JX2_UCMD_ALUCMP: begin
		end
	
		JX2_UCMD_CONV_RR: begin
			tValOutDfl		= tValCnv;
			tDoOutDfl		= 1;
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= tValCnv;
			tRegOutSr[0]	= tCnvSrT;
		end
		JX2_UCMD_MOV_RC: begin
`ifdef jx2_gprs_mergecm
			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;
`else
//			tRegIdCn1	= regIdRm[4:0];
			tRegIdCn1	= regIdRm;
			tRegValCn1	= regValRs;

			tRegIdRn1	= regIdRm;
			tRegValRn1	= regValRs;
`endif
		end
		JX2_UCMD_MOV_CR: begin
`ifdef jx2_gprs_mergecm
			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;
`else
//			tRegIdRn1	= regIdRm;
//			tRegValRn1	= regValCRm;
			tValOutDfl		= regValCRm;
			tDoOutDfl		= 1;
`endif
		end
		JX2_UCMD_MOV_IR: begin
			tValOutDfl		= regValRt;
			tDoOutDfl		= 1;

			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= {
//						regValImm[32] ? UV32_FF : UV32_00,
//						regValImm[31:0] };
//					tRegValRn1	= regValRt;
				end
				4'b0001: begin /* LDISH8 */
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRs[55:0], regValImm[7:0] };
//					tRegValRn1	= { regValRs[55:0], regValRt[7:0] };
					tValOutDfl	= { regValRs[55:0], regValRt[7:0] };
				end
				4'b0010: begin /* LDISH16 */
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRs[47:0], regValImm[15:0] };
//					tRegValRn1	= { regValRs[47:0], regValRt[15:0] };
					tValOutDfl	= { regValRs[47:0], regValRt[15:0] };
				end
				4'b0011: begin /* LDISH32 */
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= { regValRs[31:0], regValImm[31:0] };
//					tRegValRn1	= { regValRs[31:0], regValRt[31:0] };
					tValOutDfl	= { regValRs[31:0], regValRt[31:0] };
				end
				4'b0100: begin /* JLDIX */
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= regValRt;
				end
				
				default: begin
					$display("ExEX1: MOV_IR, Invalid UIxt %X", opUIxt);
//					tRegIdRn1	= regIdRm;
//					tRegValRn1	= regValRt;
				end
			endcase
		end

		JX2_UCMD_BRA_NB: begin
//			if(opPreBra)
			if(opPreBra!=2'b00)
			begin
//				tValBra		= regValPc[47:0];
				tValBra		= regValPc[63:0];
//				tValBra		= { tRegBraLr[63:48], regValPc[47:0] };
				tDoBra		= 1;
			end
		end
	
		JX2_UCMD_BRA: begin
//			if(!opPreBra)
			if(opPreBra!=2'b01)
			begin
//				tValBra		= tValAguBra[47:0];
				tValBra		= { tRegBraLr[63:48], tValAguBra[47:0] };
				tDoBra		= 1;
			end
		end
		JX2_UCMD_BSR: begin
//			$display("EX: BSR: LR=%X PC2=%X", regValPc, tValAgu);
//			tRegOutLr	= regValPc;
			tRegOutLr	= tRegBraLr;
//			tRegOutLr	= {
//				regInSr[15: 4],
//				regInSr[27:26],
//				regInSr[ 1: 0],
//				regValPc };
//			if(!opPreBra)

			if(opPreBra!=2'b01)
			begin
//				tValBra		= tValAguBra[47:0];
//				tValBra		= { tRegBraLr[63:48], tValAguBra[47:0] };
				tValBra		= { regValPc[63:48], tValAguBra[47:0] };
				tDoBra		= 1;
			end
		end
		JX2_UCMD_JMP: begin
//			tValBra		= regValRs[47:0];
//			tValBra		= { tRegBraLr[63:48], regValRs[47:0] };
			tValBra		= { regValPc[63:48], regValRs[47:0] };
//			tValBra		= regValRs[47:0];
//			tValBra		= regValRs;
//			tDoBra		= 1;
//			tDoBra		= !opPreBra;
			tDoBra		= (opPreBra != 2'b01);

			if(	(regIdRs==JX2_GR_LR) ||
				(regIdRs==JX2_GR_DHR))
			begin
				tValBra[63:48] = regValRs[63:48];
			end

`ifndef def_true
//			if(regIdRs==JX2_GR_LR)
			if(1'b1)
			begin
				tRegOutSr[ 1: 0] = regValRs[49:48];
				tRegOutSr[27:26] = regValRs[51:50];
				tRegOutSr[15: 4] = regValRs[63:52];
			end
`endif
		end
		JX2_UCMD_JSR: begin
//			$display("EX: JSR: LR=%X PC2=%X", regValRs, regValPc);
//			tRegOutLr	= regValPc;
			tRegOutLr	= tRegBraLr;
//			tRegOutLr	= {
//				regInSr[15: 4],
//				regInSr[27:26],
//				regInSr[ 1: 0],
//				regValPc };
//			tValBra		= regValRs[47:0];
//			tValBra		= regValRs;
			tValBra		= { regValPc[63:48], regValRs[47:0] };
			tDoBra		= 1;
		end
		
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
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
		JX2_UCMD_FIXS: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

		JX2_UCMD_FLDCX: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end
`endif

		JX2_UCMD_BLINT: begin
//			tHeldIdRn1	= regIdRm;
			tRegHeld		= 1;
		end

		JX2_UCMD_OP_IXS: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXS_NOP: begin
				end
				JX2_UCIX_IXS_MOVT: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= {UV63_00, regInSr[0]};
					tValOutDfl		= {UV63_00, regInSr[0]};
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_MOVNT: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= {UV63_00, !regInSr[0]};
					tValOutDfl		= {UV63_00, !regInSr[0]};
					tDoOutDfl		= 1;
				end

				JX2_UCIX_IXS_INVIC: begin
					$display("EX1 JX2_UCIX_IXS_INVIC");

// `ifdef jx2_l1i_nohash
`ifndef def_true
					if(	(	(regValRm[31:28]!=4'hF) &&
							(regValRm[31:24]!=8'h00)) ||
						(	(regValRm[47:32]!=16'h0000) &&
							(regValRm[47:32]!=16'hFFFF)))
					begin
//						tMemOpm		= UMEM_OPM_FLUSHIS;
//						tMemAddr	= regValRm[47:0];
//						tMemAddr	= { 16'h0000, 16'h0003, regValRm[15:0] };
						tMemAddr	= { 16'hC000, 16'h0003, regValRm[15:0] };
						tRegOutLr	= tRegBraLr;
						tValBra		= { regValPc[63:48], tMemAddr };
						tDoBra		= 1;
					end
					else
					begin
						tMemOpm		= UMEM_OPM_FLUSHIS;
						tMemAddr	= regValRm[47:0];

	//					tValBra		= regValPc[47:0];
						tValBra		= regValPc;
	//					tValBra		= { tRegBraLr[63:48], regValPc[47:0] };
						tDoBra		= 1;
					end
`else
					tMemOpm		= UMEM_OPM_FLUSHIS;
					tMemAddr	= regValRm[47:0];

//					tValBra		= regValPc[47:0];
					tValBra		= regValPc;
//					tValBra		= { tRegBraLr[63:48], regValPc[47:0] };
					tDoBra		= 1;
`endif

				end
				JX2_UCIX_IXS_INVDC: begin
					$display("EX1 JX2_UCIX_IXS_INVDC");

// `ifdef jx2_l1d_nohash
`ifndef def_true
					if(	(	(regValRm[31:28]!=4'hF) &&
							(regValRm[31:24]!=8'h00)) ||
						(	(regValRm[47:32]!=16'h0000) &&
							(regValRm[47:32]!=16'hFFFF)))
					begin
						tMemOpm		= UMEM_OPM_RD_Q;
//						tMemAddr	= { 16'h0000, 16'h0001, regValRm[15:0] };
						tMemAddr	= { 16'hC000, 16'h0001, regValRm[15:0] };
					end
					else
					begin
						tMemOpm		= UMEM_OPM_FLUSHDS;
						tMemAddr	= regValRm[47:0];
					end
`else
					tMemOpm		= UMEM_OPM_FLUSHDS;
					tMemAddr	= regValRm[47:0];
`endif
				end

				JX2_UCIX_IXS_TRAPB: begin
					tDoMemOpm	= UMEM_OPM_TRAP;
					tDoMemOp	= 1;
				end
				
`ifndef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
				JX2_UCIX_IXS_BLKUTX1: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_BLKUTX2: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end

`ifdef jx2_enable_btcutx3
				JX2_UCIX_IXS_BLKUTX3H: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
				JX2_UCIX_IXS_BLKUTX3L: begin
//					tRegIdRn1		= regIdRm;
//					tRegValRn1		= tValUtx1;
					tValOutDfl		= tValUtx1;
					tDoOutDfl		= 1;
				end
`endif

`endif
`endif

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
					tExTrapExc = 16'hFF00;
				end
				JX2_UCIX_IXT_TRAPA: begin
					tExTrapExc = { 12'hC08, regIdRm[3:0] };
				end

				JX2_UCIX_IXT_CPUID: begin
//					tRegIdRn1		= JX2_GR_DLR;
//					tRegValRn1		= tValCpuIdLo;
					tValOutDfl		= tValCpuIdLo;
					tDoOutDfl		= 1;
				end

				JX2_UCIX_IXT_WEXMD: begin
`ifdef jx2_enable_wex
					case(regIdRm[3:0])
						4'h0:		tRegOutSr[27]	= 0;
						4'h1:		tRegOutSr[27]	= 1;
						4'h2:		tRegOutSr[27]	= 1;
						default:	tRegOutSr[27]	= 0;
					endcase
`else
					tRegOutSr[27]	= 0;
`endif
				end

				JX2_UCIX_IXT_SYSE: begin
					tExTrapExc = { 4'hE, regInDlr[11:0] };
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

				JX2_UCIX_IXT_SVEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKEY: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEENC: begin
					tRegHeld		= 1;
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

`ifdef jx2_enable_srtwid
				JX2_UCIX_IXT_SRTTWID: begin
					tTempBit0	= regInSr[0] & regInSr[8];
					tTempBit1	= regInSr[0] | regInSr[8];
					tIfThenPP	= regInSr[11: 8] + 1;
					tIfThenNN	= regInSr[11: 8] - 1;
					tIfElsePP	= regInSr[15:12] + 1;
					tIfElseNN	= regInSr[15:12] - 1;
					tTempBit2	= regInSr[15:12] == 0;
					tTempBit3	= regInSr[15:13] == 0;
								
					case(regIdRm[4:0])
						5'h00: begin	/* PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
							tRegOutSr[   8]	= regInSr[   0];
						end
						5'h01: begin	/* POP */
							tRegOutSr[   15] = 0;
							tRegOutSr[14: 8] = regInSr[15:9];
							tRegOutSr[    0] = regInSr[   8];
						end
						5'h02: begin	/* POP2 */
							tRegOutSr[15:14] = 0;
							tRegOutSr[13: 8] = regInSr[15:10];
							tRegOutSr[    0] = regInSr[    9];
						end
						5'h03: begin	/* POP3 */
							tRegOutSr[14:13] = 0;
							tRegOutSr[12: 8] = regInSr[15:11];
							tRegOutSr[    0] = regInSr[   10];
						end
						5'h04: begin	/* AND */
							tRegOutSr[   0]	= tTempBit0;
						end
						5'h05: begin	/* OR */
							tRegOutSr[   0]	= tTempBit1;
						end
						5'h06: begin	/* AND+PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
							tRegOutSr[   8]	= tTempBit0;
							tRegOutSr[   0]	= tTempBit0;
						end
						5'h07: begin	/* OR+PUSH */
							tRegOutSr[15:9]	= regInSr[14:8];
							tRegOutSr[   8]	= tTempBit1;
							tRegOutSr[   0]	= tTempBit1;
						end

						5'h08: begin	/* CLEAR */
							tRegOutSr[15:8]	= 0;
						end


`ifdef def_true
// `ifndef def_true

						5'h0E: begin	/* ELSE */
							if(tTempBit3)
							begin
								if(regInSr[12])
								begin
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[    0] = 0;
								end
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end
						5'h0F: begin	/* ENDIF */
							if(!tTempBit2)
								tRegOutSr[15:12]=tIfElseNN;
							else
								tRegOutSr[11:8]=tIfThenNN;
							tRegOutSr[    0] = tTempBit3;
						end

						5'h10: begin	/* IF0T */
							tRegOutSr[15:8]	= 0;
							if(regInSr[0])
								tRegOutSr[8]=1;
							else
								tRegOutSr[12]=1;
						end
						5'h11: begin	/* IF0F */
							tRegOutSr[15:8]	= 0;
							if(!regInSr[0])
								tRegOutSr[8]=1;
							else
								tRegOutSr[12]=1;
							tRegOutSr[0] = !regInSr[0];
						end
						5'h12: begin		/* IF1T */
							if(regInSr[0] && tTempBit2)
							begin
								tRegOutSr[11:8] = tIfThenPP;
								tRegOutSr[   0] = 1;
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end
						5'h13: begin		/* IF1F */
							if(!regInSr[0] && tTempBit2)
							begin
								tRegOutSr[11:8] =tIfThenPP;
								tRegOutSr[   0] = 1;
							end else begin
								tRegOutSr[15:12] = tIfElsePP;
								tRegOutSr[    0] = 0;
							end
						end

						5'h14: begin		/* IFAAT */
							if(tTempBit2)
							begin
								if(regInSr[0])
								begin
									tRegOutSr[   0] = 1;
								end else begin
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[    0] = 0;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h15: begin		/* IFAAF */
							if(tTempBit2)
							begin
								if(!regInSr[0] && tTempBit2)
								begin
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[15:12] = tIfElsePP;
									tRegOutSr[11: 8] = tIfThenNN;
									tRegOutSr[    0] = 0;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h16: begin		/* IFOOT */
							if(tTempBit3)
							begin
								if(regInSr[0] && !tTempBit2)
								begin
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[    0] = tTempBit2;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
						5'h17: begin		/* IFOOF */
							if(tTempBit3)
							begin
								if(!regInSr[0] && !tTempBit2)
								begin
									tRegOutSr[15:12] = tIfElseNN;
									tRegOutSr[11: 8] = tIfThenPP;
									tRegOutSr[    0] = 1;
								end else begin
									tRegOutSr[    0] = tTempBit2;
								end
							end
							else
							begin
								tRegOutSr[0] = 0;
							end
						end
`endif

						default:	begin
						end
					endcase
				end
`endif

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
//			tExHold		= !reset;
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

//	if(tHeldIdRn1 != JX2_GR_ZZR)
//		tRegHeld		= 1;

end

always @(posedge clock)
begin
	tAguFlagJq	<= regInSr[31];
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

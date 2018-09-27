`include "Jx2CoreDefs.v"

`include "Jx2ExShad32.v"
`include "Jx2ExShad64.v"

module Jx2ExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRi,	regValRi,
	regIdCm,	regValCm,
	immValRi,	idInGenPc,
	regOutId,	regOutVal,
	regOutIdCn,	regOutValCn,
	regOutOK,

	regValFRm,	regValFRn,

	memAddr,	memData,
	memOpm,		memOpCmd2,
	memDataLd,	memDataOK,

	ctlInSp,	ctlOutSp,
	ctlInPc,	ctlOutPc,
	ctlInPr,	ctlOutPr,
	ctlInSr,	ctlOutSr,
	ctlInVbr,	ctlOutVbr,
	ctlInDlr,	ctlOutDlr,
	ctlInDhr,	ctlOutDhr,
	ctlInGbr,	ctlOutGbr,
	ctlInTbr,	ctlOutTbr
	);

input			clock;
input			reset;

input[7:0]		opCmd;			//command opcode
input[7:0]		regIdIxt;		//ALU Index / Opcode Extension

input[6:0]		regIdRm;
input[6:0]		regIdRn;
input[6:0]		regIdRi;
input[6:0]		regIdCm;

input[63:0]		regValRm;		//Rm input value
input[63:0]		regValRn;		//Rn input value
input[63:0]		regValRi;		//Rn input value
input[63:0]		regValCm;		//Cm input value
input[32:0]		immValRi;		//immediate/disp value
input[63:0]		idInGenPc;		//PC to next instruction

output[63:0]	regOutVal;		//Rn output value
output[6:0]		regOutId;		//Rn, register to write
output[1:0]		regOutOK;		//execute status

output[63:0]	regOutValCn;	//Rn output value
output[6:0]		regOutIdCn;		//Rn, value to write

input[63:0]		regValFRm;		//FRm input value
input[63:0]		regValFRn;		//FRn input value

/* Memory */
output[63:0]	memAddr;		//memory address
output[63:0]	memData;		//memory data (store)
output[4:0]		memOpm;			//mem op mode
output[7:0]		memOpCmd2;		//mem EX chain

input[63:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)


/* Special Registers */
input[63:0]		ctlInSp;		//SP in
output[63:0]	ctlOutSp;		//SP out

input[63:0]		ctlInPc;		//PC in
output[63:0]	ctlOutPc;		//PC out

input[63:0]		ctlInPr;		//PR in
output[63:0]	ctlOutPr;		//PR out

input[63:0]		ctlInSr;		//SR in
output[63:0]	ctlOutSr;		//SR out

input[63:0]		ctlInVbr;
output[63:0]	ctlOutVbr;

input[63:0]		ctlInDlr;		//DLR in
output[63:0]	ctlOutDlr;		//DLR out

input[63:0]		ctlInDhr;		//DHR in
output[63:0]	ctlOutDhr;		//DHR out

input[63:0]		ctlInGbr;
output[63:0]	ctlOutGbr;

input[63:0]		ctlInTbr;
output[63:0]	ctlOutTbr;


reg[63:0]	tCtlOutSp;		//SP out
reg[63:0]	tCtlOutPc;		//PC out
reg[63:0]	tCtlOutPr;		//PR out
reg[63:0]	tCtlOutSr;		//SR out
reg[63:0]	tCtlOutVbr;
reg[63:0]	tCtlOutDlr;		//MACL out
reg[63:0]	tCtlOutDhr;		//MACH out
reg[63:0]	tCtlOutGbr;
reg[63:0]	tCtlOutTbr;

assign	ctlOutSp	= tCtlOutSp;
assign	ctlOutPc	= tCtlOutPc;
assign	ctlOutPr	= tCtlOutPr;
assign	ctlOutSr	= tCtlOutSr;
assign	ctlOutVbr	= tCtlOutVbr;
assign	ctlOutDlr	= tCtlOutDlr;
assign	ctlOutDhr	= tCtlOutDhr;
assign	ctlOutGbr	= tCtlOutGbr;
assign	ctlOutTbr	= tCtlOutTbr;

reg[63:0]	tRegOutVal;		//Rn output value
reg[6:0]	tRegOutId;		//Rn, value to write
reg[1:0]	tRegOutOK;		//execute status
reg[63:0]	tMemAddr;		//memory address
reg[63:0]	tMemData;		//memory data (store)
reg[4:0]	tMemOpm;		//mem op mode
reg[7:0]	tMemOpCmd2;		//mem EX chain

reg[63:0]	tRegOutValCn;	//Rn output value
reg[6:0]	tRegOutIdCn;	//Rn, value to write

assign	regOutVal	= tRegOutVal;
assign	regOutId	= tRegOutId;
assign	regOutOK	= tRegOutOK;
assign	memAddr		= tMemAddr;
assign	memData		= tMemData;
assign	memOpm		= tMemOpm;
assign	memOpCmd2	= tMemOpCmd2;

assign	regOutValCn	= tRegOutValCn;
assign	regOutIdCn	= tRegOutIdCn;

wire[31:0]	tShadOut;
wire[63:0]	tShadQOut;
reg[2:0]	tShOp;

//wire	tShadIsImm3;
//assign	tShadIsImm3 =
//	(opCmd == JX2_UCMD_ALU_SHAD3 ) ||
//	(opCmd == JX2_UCMD_ALU_SHLD3 ) ||
//	(opCmd == JX2_UCMD_ALU_SHADQ3) ||
//	(opCmd == JX2_UCMD_ALU_SHLDQ3) ;

wire[63:0]	tShadValIn;
wire[7:0]	tShadValShl;
assign		tShadValIn = regValRm;
assign		tShadValShl = regValRi[7:0];

//assign		tShadValIn =
//		tShadIsImm3 ? regValRm : regValRn;
//assign		tShadValShl =
//		tShadIsImm3 ? regValRi[7:0] : regValRm[7:0];

Jx2ExShad32		shad(
	clock, 			reset,
	tShadValIn[31:0],	tShadValShl,
	tShadOut,		tShOp);
Jx2ExShad64		shadq(
	clock, 			reset,
	tShadValIn[63:0],	tShadValShl,
	tShadQOut,		tShOp);

reg[63:0]		regMaRbVal;
reg[63:0]		regMaIxVal;
reg[63:0]		regMaIxVal2;
reg[3:0]		regMaIxt;
reg[2:0]		regMaBty;
reg[63:0]		regMaAddr;

reg[32:0]		regMaAddrA0;
reg[31:0]		regMaAddrB0;
reg[31:0]		regMaAddrB1;

reg[63:0]		regLastMaAddr;

reg[63:0]		regMaIxVal3;
// reg[63:0]		regMaAddrPc;

reg[63:0]		regRnSxt;
reg[63:0]		immValRiB;
reg[31:0]		immValRiA;
reg[63:0]		regDlrA;
reg[63:0]		regDlrI4;

reg[31:0]		regMulA;
reg[31:0]		regMulB;
reg[1:0]		regMulOp;

reg[31:0]		regMulDlr;
reg[31:0]		regMulDhr;
reg[1:0]		regMulOpDr;

reg[3:0]		regHoldFixedCnt;
reg				regHoldFixed;
reg				regRnIsGpr;

reg[7:0]		opCmdA;
reg[7:0]		opCmdB;
reg[7:0]		opCmdC;

wire[31:0]		regValFRmSs;	//FRm input value (Single Precision)
wire[31:0]		regValFRnSs;	//FRn input value (Single Precision)

Jx2ConvFpuD2S	fd2s_rm(regValFRm, regValFRmSs);
Jx2ConvFpuD2S	fd2s_rn(regValFRn, regValFRnSs);

// wire[15:0]		regValFRmSh;	//FRm input value (Single Precision)
wire[15:0]		regValFRnSh;	//FRn input value (Single Precision)

// Jx2ConvFpuD2H	fd2h_rm(regValFRm, regValFRmSh);
Jx2ConvFpuD2H	fd2h_rn(regValFRn, regValFRnSh);

wire[63:0]		regValFRnSi;	//FRn input value (Int)
Jx2ConvFpuD2I	fd2i_rn(
	clock,		reset,
	regValFRn,	regValFRnSi);

reg		tRegValFRn_IsNaN;
reg		tRegValFRm_IsNaN;

reg[32:0]		tAddRmRi_A0;
reg[32:0]		tAddRmRi_A1;
reg[32:0]		tAddRmRi_B0;
reg[32:0]		tAddRmRi_B1;
reg[32:0]		tSubRmRi_A0;
reg[32:0]		tSubRmRi_A1;
reg[32:0]		tSubRmRi_B0;
reg[32:0]		tSubRmRi_B1;

always @*
begin
	tCtlOutSp	= ctlInSp;
	tCtlOutPc	= ctlInPc;
	tCtlOutPr	= ctlInPr;
	tCtlOutSr	= ctlInSr;
	tCtlOutVbr	= ctlInVbr;
	tCtlOutDlr	= ctlInDlr;
	tCtlOutDhr	= ctlInDhr;
	tCtlOutGbr	= ctlInGbr;
	tCtlOutTbr	= ctlInTbr;

	tRegOutVal	= UV64_XX;
	tRegOutId	= JX2_REG_ZZR;
	tRegOutOK	= UMEM_OK_OK;
	tMemAddr	= 0;
	tMemData	= UV64_XX;
	tMemOpm		= UMEM_OPM_READY;
	tMemOpCmd2	= JX2_UCMD_NOP;

	tRegOutValCn	= UV64_XX;
	tRegOutIdCn		= JX2_REG_ZZR;

	opCmdA		= opCmd;
	opCmdB		= opCmd;
	opCmdC		= opCmd;

	regRnSxt	= regValRn[63] ? UV64_FF : UV64_00;
	immValRiB	= { immValRi[32] ? UV32_FF : UV32_00, immValRi[31:0]};

	regMulA		= UV32_XX;
	regMulB		= UV32_XX;
	regMulOp	= 0;


/*
//	regMaIxt=regIdIxt[7:4];
//	regMaBty=regIdIxt[2:0];
//	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;

	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;
	case(regMaIxt[1:0])
	JX2_IX2_REG:
		regMaIxVal=0;
	JX2_IX2_RDL:
		regMaIxVal=ctlInDlr;
	JX2_IX2_RDI:
		regMaIxVal=immValRiB;
	JX2_IX2_RD4:
		regMaIxVal={ctlInDlr[59:0], immValRi[3:0]};
	endcase

	case(regMaBty[1:0])
		2'b00: regMaIxVal2=regMaIxVal;
		2'b01: regMaIxVal2={regMaIxVal[62:0], 1'b0};
		2'b10: regMaIxVal2={regMaIxVal[61:0], 2'b0};
		2'b11: regMaIxVal2={regMaIxVal[60:0], 3'b0};
	endcase

	regMaAddr=regMaRbVal+regMaIxVal2;
*/
	
	
	immValRiA = immValRi[31:0];
	regDlrA = ctlInDlr;
	regDlrI4 = {regDlrA[59:0], immValRiA[3:0]};

	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;
	casez({regIdIxt[6:4], regIdIxt[1:0]})
		5'b000zz: regMaIxVal2=0;
		5'b00100: regMaIxVal2=regDlrA[63:0];
		5'b00101: regMaIxVal2={regDlrA[62:0], 1'b0};
		5'b00110: regMaIxVal2={regDlrA[61:0], 2'b0};
		5'b00111: regMaIxVal2={regDlrA[60:0], 3'b0};
		5'b01000: regMaIxVal2=immValRiB[63:0];
		5'b01001: regMaIxVal2={immValRiB[62:0], 1'b0};
		5'b01010: regMaIxVal2={immValRiB[61:0], 2'b0};
		5'b01011: regMaIxVal2={immValRiB[60:0], 3'b0};

		5'b10100: regMaIxVal2=regValRi;
		5'b10101: regMaIxVal2={regValRi[62:0], 1'b0};
		5'b10110: regMaIxVal2={regValRi[61:0], 2'b0};
		5'b10111: regMaIxVal2={regValRi[60:0], 3'b0};
		default: begin
		end
	endcase
//	regMaAddr=regMaRbVal+regMaIxVal2;

	regMaAddrA0=
		{ 1'b0, regMaRbVal [31:0] } +
		{ 1'b0, regMaIxVal2[31:0] } ;
	regMaAddrB0=
		regMaRbVal [63:32] +
		regMaIxVal2[63:32] + 0;
	regMaAddrB1=
		regMaRbVal [63:32] +
		regMaIxVal2[63:32] + 1;

	regMaAddr[31: 0]=regMaAddrA0[31:0];
	regMaAddr[63:32]=regMaAddrA0[32]?regMaAddrB1:regMaAddrB0;

	tRegValFRn_IsNaN =
		(regValFRn[62:52]==11'h7FF) &&
		(regValFRn[51: 0]!=0);
	tRegValFRm_IsNaN =
		(regValFRm[62:52]==11'h7FF) &&
		(regValFRm[51: 0]!=0);
	
	/* ALU */
	tAddRmRi_A0 =
		{ 1'b0, regValRm[31:0] } +
		{ 1'b0, regValRi[31:0] } + 0;
	tAddRmRi_A1 =
		{ 1'b0, regValRm[31:0] } +
		{ 1'b0, regValRi[31:0] } + 1;
	tAddRmRi_B0 =
		{ 1'b0, regValRm[63:32] } +
		{ 1'b0, regValRi[63:32] } + 0;
	tAddRmRi_B1 =
		{ 1'b0, regValRm[63:32] } +
		{ 1'b0, regValRi[63:32] } + 1;
	tSubRmRi_A0 =
		{ 1'b0,  regValRm[31:0] } +
		{ 1'b0, ~regValRi[31:0] } + 0;
	tSubRmRi_A1 =
		{ 1'b0,  regValRm[31:0] } +
		{ 1'b0, ~regValRi[31:0] } + 1;
	tSubRmRi_B0 =
		{ 1'b0,  regValRm[63:32] } +
		{ 1'b0, ~regValRi[63:32] } + 0;
	tSubRmRi_B1 =
		{ 1'b0,  regValRm[63:32] } +
		{ 1'b0, ~regValRi[63:32] } + 1;

	regHoldFixed = 0;

	regRnIsGpr = 1;
	
	case(opCmdA)

		JX2_UCMD_NOP: begin
		end

/*
		JX2_UCMD_MOV_RR: begin
			if(regIdRn[6:5]==2'b00)
			begin
				tRegOutVal	= regValRm;
				tRegOutId	= regIdRn;
			end
			else
			begin
				tRegOutValCn	= regValRm;
				tRegOutIdCn		= regIdRn;
			end
		end
*/

		JX2_UCMD_MOV_RR: begin
			tRegOutVal	= regValRm;
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_MOV_RC: begin
			tRegOutValCn	= regValRm;
			tRegOutIdCn		= regIdRn;
		end
		JX2_UCMD_MOV_CR: begin
			tRegOutVal	= regValCm;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_MOV_IR: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= immValRiB;
				tRegOutId	= regIdRn;
			end
		end
		
		JX2_UCMD_LEA_MR: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regMaAddr;
				tRegOutVal	= regLastMaAddr;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_MOVB_RM: begin
			tMemAddr	= regMaAddr;
			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
	//		$display("MOV.B %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_MOVW_RM: begin
			tMemAddr	= regMaAddr;
			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SW;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
	//		$display("MOV.W %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_MOVL_RM: begin
			tMemAddr	= regMaAddr;
			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end

		JX2_UCMD_MOVB_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SB;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
	//		$display("MOV.B (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVW_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SW;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
	//		$display("MOV.W (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVL_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SL;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;

	//		$display("MOV.L (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVUB_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UB;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
	//		$display("MOVU.B (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVUW_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UW;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
	//		$display("MOVU.W (%X), %X", regMaAddr, memDataLd);
		end

		JX2_UCMD_MOVUL_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
	//		$display("MOVU.L (%X), %X", regMaAddr, memDataLd);
		end


		JX2_UCMD_FMOVS_RM: begin
			tMemAddr	= regMaAddr;
	//		tMemData	= regValFRm;
			tMemData	= { 32'h0, regValFRmSs };
			tMemOpm		= UMEM_OPM_WR_SL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_FMOVD_RM: begin
			tMemAddr	= regMaAddr;
			tMemData	= regValFRm;
			tMemOpm		= UMEM_OPM_WR_Q;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end

		JX2_UCMD_FMOVS_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
	//		$display("FMOV.S (%X), %X", regMaAddr, memDataLd);
		end

		JX2_UCMD_FMOVD_MR: begin
			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_Q;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
	//		$display("FMOV.S (%X), %X", regMaAddr, memDataLd);
		end


		JX2_UCMD_MOV_PUSH: begin
	//		$display("PUSH %X %X", ctlInSp-4, regValRn);
	//		tCtlOutSp	= ctlInSp-4;
	//		tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]-8 };
			tCtlOutSp	= ctlInSp-8;
			tMemAddr	= tCtlOutSp;
			tMemData	= regValRn;
			tMemOpm		= UMEM_OPM_WR_SL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end
		end

		JX2_UCMD_MOV_POP: begin
	//		tCtlOutSp	= ctlInSp+4;
	//		tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
			tCtlOutSp	= ctlInSp+8;
			tMemAddr	= ctlInSp;
			tMemOpm		= UMEM_OPM_RD_SL;
			if(memDataOK!=UMEM_OK_OK)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
				tMemOpm		= UMEM_OPM_READY;
			end

	//		$display("POP %X %X", ctlInSp, memDataLd);

			if(regIdRn==JX2_REG_PC)
			begin
	//			$display("RET %X", regMaAddr);
				tCtlOutPc	= memDataLd;

				if(memDataOK==UMEM_OK_OK)
					tMemOpm		= UMEM_OPM_CTRLF;
				
				if(memDataLd==0)
				begin
					tRegOutOK	= UMEM_OK_HOLD;
				end
				else
				begin
	//				$display("RET %X", memDataLd);
				end
			end
			else
			begin
	//			$display("POP %X %X", ctlInSp, memDataLd);
				tRegOutVal	= memDataLd;
				tRegOutId	= regIdRn;
			end
		end

		default: begin
		end
	
	endcase


	case(opCmdB)

		JX2_UCMD_ALU_TST: begin
			tCtlOutSr[0] = ((regValRn[31:0] & regValRm[31:0])==0);
		end
		JX2_UCMD_ALU_TSTQ: begin
			tCtlOutSr[0] = ((regValRn[63:0] & regValRm[63:0])==0);
		end

		JX2_UCMD_ALU_ADC3: begin
			if(regRnIsGpr)
			begin
				if(ctlInSr[0])
				begin
					tRegOutVal[31:0]=tAddRmRi_A1[31:0];
					tRegOutVal[63:32]=tAddRmRi_A1[32] ?
						tAddRmRi_B1[31:0] : tAddRmRi_B0[31:0];
					tCtlOutSr[0] = tAddRmRi_A1[32] ?
						tAddRmRi_B1[32] : tAddRmRi_B0[32];
				end
				else
				begin
					tRegOutVal[31:0]=tAddRmRi_A0[31:0];
					tRegOutVal[63:32]=tAddRmRi_A0[32] ?
						tAddRmRi_B1[31:0] : tAddRmRi_B0[31:0];
					tCtlOutSr[0] = tAddRmRi_A0[32] ?
						tAddRmRi_B1[32] : tAddRmRi_B0[32];
				end

				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SBB3: begin
			if(regRnIsGpr)
			begin
				if(!ctlInSr[0])
				begin
					tRegOutVal[31:0]=tSubRmRi_A1[31:0];
					tRegOutVal[63:32]=tSubRmRi_A1[32] ?
						tSubRmRi_B1[31:0] : tSubRmRi_B0[31:0];
					tCtlOutSr[0] = tSubRmRi_A1[32] ?
						tSubRmRi_B1[32] : tSubRmRi_B0[32];
				end
				else
				begin
					tRegOutVal[31:0]=tSubRmRi_A0[31:0];
					tRegOutVal[63:32]=tSubRmRi_A0[32] ?
						tSubRmRi_B1[31:0] : tSubRmRi_B0[31:0];
					tCtlOutSr[0] = tAddRmRi_A0[32] ?
						tSubRmRi_B1[32] : tSubRmRi_B0[32];
				end

				tRegOutId	= regIdRn;
			end
		end


		JX2_UCMD_ALU_ADD3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal[31: 0] = tAddRmRi_A0[31:0];
				tRegOutVal[63:32] = tAddRmRi_A0[32] ?
					tAddRmRi_B1[31:0] : tAddRmRi_B0[31:0];

//				tRegOutVal	= regValRm + regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_SUB3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal[31: 0] = tSubRmRi_A1[31:0];
				tRegOutVal[63:32] = tSubRmRi_A1[32] ?
					tSubRmRi_B1[31:0] : tSubRmRi_B0[31:0];

//				tRegOutVal	= regValRm - regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_AND3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm & regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_OR3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm | regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_XOR3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm ^ regValRi;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_MUL3: begin
			regMulA		= regValRm[31:0];
			regMulB		= regValRi[31:0];
			regMulOp	= 2'h2;
			tRegOutVal	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr };
			tRegOutId	= regIdRn;
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
	//			$display("MULS %X:%X", tCtlOutDhr, tCtlOutDlr);
			end

		end

		JX2_UCMD_ALU_CSELT: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= ctlInSr[0] ? regValRm : regValRi;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLD3: begin
			tShOp = 3'h1;
			if(regRnIsGpr)
			begin
				tRegOutVal	= {UV32_00, tShadOut[31:0]};
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHAD3: begin
			tShOp = 3'h2;
			if(regRnIsGpr)
			begin
				tRegOutVal	= {tShadOut[31]?UV32_FF:UV32_00, tShadOut[31:0]};
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLDQ3: begin
			tShOp = 3'h1;
			if(regRnIsGpr)
			begin
				tRegOutVal	= tShadQOut[63:0];
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHADQ3: begin
			tShOp = 3'h2;
			if(regRnIsGpr)
			begin
				tRegOutVal	= tShadQOut[63:0];
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_MULU: begin
			regMulA		= regValRn[31:0];
			regMulB		= regValRm[31:0];
			regMulOp	= 2'h1;
			tCtlOutDlr	= { UV32_00, regMulDlr };
			tCtlOutDhr	= { UV32_00, regMulDhr };

			if(regMulOpDr!=regMulOp)
				tRegOutOK	= UMEM_OK_HOLD;

		end

		JX2_UCMD_ALU_MULS: begin
			regMulA		= regValRn[31:0];
			regMulB		= regValRm[31:0];
			regMulOp	= 2'h2;
			tCtlOutDlr	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr };
			tCtlOutDhr	= { regMulDhr[31]?UV32_FF:UV32_00, regMulDhr };
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
	//			$display("MULS %X:%X", tCtlOutDhr, tCtlOutDlr);
			end

		end
		
		JX2_UCMD_ALU_LDIX: begin
			tCtlOutDlr	= immValRiB;
		end
		JX2_UCMD_ALU_LDISH: begin
			tCtlOutDlr	= { ctlInDlr[55:0], immValRi[7:0] };
		end

		JX2_UCMD_ALU_LDISH16: begin
			tCtlOutDlr	= { ctlInDlr[47:0], immValRi[15:0] };
		end
		JX2_UCMD_ALU_LDISH32: begin
			tCtlOutDlr	= { ctlInDlr[31:0], immValRi[31:0] };
		end

		JX2_UCMD_ALU_CMPEQ: begin
			tCtlOutSr[0] = (regValRn[31:0] == regValRm[31:0]);
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPHI: begin
			tCtlOutSr[0] = (regValRn[31:0] > regValRm[31:0]);
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGT: begin
			tCtlOutSr[0] = (regValRn[31:0] > regValRm[31:0]) ^
				(regValRn[31] ^ regValRm[31]);
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGE: begin
			tCtlOutSr[0] = (regValRn[31:0] >= regValRm[31:0]) ^
				(regValRn[31] ^ regValRm[31]);
	//		$display("CMPGE %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		JX2_UCMD_ALU_CMPQEQ: begin
			tCtlOutSr[0] = (regValRn == regValRm);
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQHI: begin
			tCtlOutSr[0] = (regValRn > regValRm);
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQGT: begin
			tCtlOutSr[0] = (regValRn > regValRm) ^
				(regValRn[31] ^ regValRm[31]);
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		default: begin
		end

	endcase


	case(opCmdC)

		JX2_UCMD_CF_BRA: begin
	//		$display("BRA %X", regMaAddr);
			tCtlOutPc	= regMaAddr;
//			tCtlOutPc	= regMaAddrPc;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_BSR: begin
	//		$display("BSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regMaAddr;
//			tCtlOutPc	= regMaAddrPc;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_BT: begin
	//		$display("BT %X %X", regMaAddr, ctlInSr[0]);
			if(ctlInSr[0])
			begin
	//			$display("BT %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
//				tCtlOutPc	= regMaAddrPc;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_BF: begin
	//		$display("BF %X %X", regMaAddr, ctlInSr[0]);
			if(!ctlInSr[0])
			begin
	//			$display("BF %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
//				tCtlOutPc	= regMaAddrPc;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end

		JX2_UCMD_CF_JMP: begin
	//		$display("JMP %X", regMaAddr);
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_JSR: begin
	//		$display("JSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_JT: begin
			if(ctlInSr[0])
			begin
				tCtlOutPc	= regValRn;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_JF: begin
			if(!ctlInSr[0])
			begin
				tCtlOutPc	= regValRn;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end

		JX2_UCMD_ALU_EXTUB: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { UV56_00, regValRn[7:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTUW: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { UV48_00, regValRn[15:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTSB: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[7]?UV56_FF:UV56_00, regValRn[7:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTSW: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[15]?UV48_FF:UV48_00, regValRn[15:0] };
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_NOT: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= ~regValRn;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_NEG: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= -regValRn;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_NEGC: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= (~regValRn) +
					{ 63'h0, ~ctlInSr[0]};
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_ROTL: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[62:0], regValRn[31] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTR: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[0], regValRn[63:1] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTCL: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[63];
				tRegOutVal		= { regValRn[62:0], ctlInSr[0] };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTCR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { ctlInSr[0], regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLL: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[63];
				tRegOutVal		= { regValRn[62:0], 1'b0 };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHLR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { 1'b0, regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHAR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { regValRn[63], regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLLN: begin
			if(regRnIsGpr)
			begin
				tRegOutId		= regIdRn;
				case(regIdIxt[4:0])
					5'h01: tRegOutVal = { regValRn[62:0], 1'b0 };
					5'h02: tRegOutVal = { regValRn[61:0], 2'b0 };
					5'h04: tRegOutVal = { regValRn[59:0], 4'b0 };
					5'h08: tRegOutVal = { regValRn[55:0], 8'b0 };
					5'h10: tRegOutVal = { regValRn[47:0], 16'b0 };
					default:	tRegOutVal = UV64_XX;
				endcase
			end
		end
		JX2_UCMD_ALU_SHLRN: begin
			if(regRnIsGpr)
			begin
				tRegOutId		= regIdRn;
				case(regIdIxt[4:0])
					5'h01: tRegOutVal = {  1'b0, regValRn[63: 1] };
					5'h02: tRegOutVal = {  2'b0, regValRn[63: 2] };
					5'h04: tRegOutVal = {  4'b0, regValRn[63: 4] };
					5'h08: tRegOutVal = {  8'b0, regValRn[63: 8] };
					5'h10: tRegOutVal = { 16'b0, regValRn[63:16] };
					default:	tRegOutVal = UV64_XX;
				endcase
			end
		end
		JX2_UCMD_ALU_SHARN: begin
			if(regRnIsGpr)
			begin
				tRegOutId		= regIdRn;
				case(regIdIxt[4:0])
					5'h01: tRegOutVal = { regRnSxt[   0], regValRn[63: 1] };
					5'h02: tRegOutVal = { regRnSxt[ 1:0], regValRn[63: 2] };
					5'h04: tRegOutVal = { regRnSxt[ 3:0], regValRn[63: 4] };
					5'h08: tRegOutVal = { regRnSxt[ 7:0], regValRn[63: 8] };
					5'h10: tRegOutVal = { regRnSxt[15:0], regValRn[63:16] };
					default:	tRegOutVal = UV64_XX;
				endcase
			end
		end

		JX2_UCMD_ALU_SHARSX: begin
			if(regRnIsGpr)
			begin
				tRegOutId		= regIdRn;
				tRegOutVal		= regRnSxt;
			end
		end

		JX2_UCMD_OP_IXT: begin
			case(regIdIxt[7:0])
				JX2_UCMD_IX_NOP: begin
				end

				JX2_UCMD_IX_RTS: begin
					tCtlOutPc	= ctlInPr;
					tMemOpm		= UMEM_OPM_CTRLF;
				end

				JX2_UCMD_IX_BREAK: begin
	//				tCtlOutPc	= ctlInPr;
					tRegOutOK	= UMEM_OK_HOLD;
				end

				JX2_UCMD_IX_CLRT: begin
					tCtlOutSr[0] = 1'b0;
				end
				JX2_UCMD_IX_SETT: begin
					tCtlOutSr[0] = 1'b1;
				end
				JX2_UCMD_IX_CLRS: begin
					tCtlOutSr[1] = 1'b0;
				end
				JX2_UCMD_IX_SETS: begin
					tCtlOutSr[1] = 1'b1;
				end

				JX2_UCMD_IX_NOTT: begin
					tCtlOutSr[0] = !ctlInSr[0];
				end
				JX2_UCMD_IX_NOTS: begin
					tCtlOutSr[1] = !ctlInSr[1];
				end

				default: begin
				$display("Jx2ExOp: Missing %X-%X", opCmd, regIdIxt);
				tRegOutOK	= UMEM_OK_HOLD;
				end
			endcase
		end

		JX2_UCMD_FPU_FADD: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FSUB: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FMUL: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FDIV: begin
			/* Ignored Here, Handled by FPU */
		end

		JX2_UCMD_FPU_FCMPEQ: begin
			tCtlOutSr[0] = (regValFRn == regValFRm) &&
				!tRegValFRn_IsNaN &&
				!tRegValFRm_IsNaN;
		end

		JX2_UCMD_FPU_FCMPGT: begin
			tCtlOutSr[0] = (regValFRn > regValFRm) ^
				(regValFRn[31] | regValFRm[31]);
	//		$display("FCMPGT %X %X %X", regValFRn, regValFRm, tCtlOutSr[0]);
		end

		JX2_UCMD_FPU_FMOV: begin
			/* Ignored Here, Handled by FPU */
		end

		JX2_UCMD_FPU_FSTCF: begin
			tCtlOutDlr	= {UV32_00, regValFRnSs};
		end

		JX2_UCMD_FPU_FSTCD: begin
			tCtlOutDlr	= regValFRn;
		end

		JX2_UCMD_FPU_FSTCI: begin
			tCtlOutDlr	= regValFRnSi;

			if(regHoldFixedCnt<3)
			begin
				regHoldFixed = 1;
				tRegOutOK	= UMEM_OK_HOLD;
			end

		end

		JX2_UCMD_FPU_FSTCH: begin
			tCtlOutDlr	= {UV48_00, regValFRnSh};
		end

		default: begin
		end

	endcase

end


reg[31:0]		regMulA1;
reg[31:0]		regMulB1;
reg[1:0]		regMulOp1;

// reg[31:0]	regMulPiAA;
// reg[31:0]	regMulPiAB;
// reg[31:0]	regMulPiBA;
// reg[31:0]	regMulPiBB;

reg[15:0]	regMulPiAA;
reg[15:0]	regMulPiAB;
reg[15:0]	regMulPiBA;
reg[15:0]	regMulPiBB;

reg[31:0]	regMulPiAC;
reg[31:0]	regMulPiBC;

// reg[31:0]	regMulPiD;

reg[1:0]	regMulOpPi;
reg[1:0]	regMulOpPj;

reg[31:0]	regMulPpA;
reg[31:0]	regMulPpB;
reg[31:0]	regMulPpC;
reg[31:0]	regMulPpD;
reg[31:0]	regMulPpE;

reg[1:0]	regMulOpPp;

reg[31:0]	regMulPpA1;
reg[31:0]	regMulPpB1;
reg[31:0]	regMulPpC1;
reg[31:0]	regMulPpD1;


reg[31:0]	regMulPqA;
reg[31:0]	regMulPqB;
reg[1:0]	regMulOpPq;

always @*
begin
// / *
//	regMulPiAA	= { 16'h0, regMulA[15: 0] };
//	regMulPiAB	= { 16'h0, regMulA[31:16] };
//	regMulPiBA	= { 16'h0, regMulB[15: 0] };
//	regMulPiBB	= { 16'h0, regMulB[31:16] };

	regMulPiAA	= regMulA1[15: 0];
	regMulPiAB	= regMulA1[31:16];
	regMulPiBA	= regMulB1[15: 0];
	regMulPiBB	= regMulB1[31:16];

//	regMulPiAC	= { regMulA[31]?16'hFFFF:16'h0000, regMulA[31:16] };
//	regMulPiBC	= { regMulB[31]?16'hFFFF:16'h0000, regMulB[31:16] };
// * /

	regMulPiAC	= regMulA1[31]?(-regMulB1):UV32_00;
	regMulPiBC	= regMulB1[31]?(-regMulA1):UV32_00;

//	regMulPpA1	= regMulPiAA * regMulPiBA;
//	regMulPpB1	= regMulPiAA * regMulPiBB;
//	regMulPpC1	= regMulPiAB * regMulPiBA;
//	regMulPpD1	= regMulPiAB * regMulPiBB;

	regMulOpPi	= regMulOp1;

	{ regMulPqB, regMulPqA } =
		{UV32_00, regMulPpA} +
		{UV16_00, regMulPpB, UV16_00} +
		{UV16_00, regMulPpC, UV16_00} +
		{regMulPpD, UV32_00} +
		{regMulPpE, UV32_00};

	regMulOpPq = regMulOpPp;
end

always @ (posedge clock)
begin
// /*
	if(regHoldFixed)
		regHoldFixedCnt		<= (regHoldFixedCnt+1);
	else
		regHoldFixedCnt		<= 0;

//	regLastMaAddr	<= regMaAddr;
//*/

	regMulA1	<= regMulA;
	regMulB1	<= regMulB;
	regMulOp1	<= regMulOp;

	regMulPpA	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBA };
	regMulPpB	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBB };
	regMulPpC	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBA };
	regMulPpD	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBB };

	case(regMulOp1)
		2'h0: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end

		2'h1: begin
			regMulPpE	<= 0;
			regMulOpPp	<= regMulOpPi;
			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
		end

		2'h2: begin
			regMulPpE	<= regMulPiAC + regMulPiBC;
			regMulOpPp	<= regMulOpPi;
			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
			
			$display("Jx2ExOp: Mul A: %X %X", regMulA, regMulB);
			$display("Jx2ExOp: Mul B: %X %X %X %X",
				regMulPpA, regMulPpB,
				regMulPpC, regMulPpD);
			$display("Jx2ExOp: Mul C: %X %X", regMulDlr, regMulDhr);
			
		end

		2'h3: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end
	endcase
end

endmodule

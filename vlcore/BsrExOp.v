`include "BsrCoreDefs.v"

module BsrExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	immValRi,	idInGenPc,
	regOutId,	regOutVal,
	regOutOK,

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

input[5:0]		regIdRm;
input[5:0]		regIdRn;

input[31:0]		regValRm;		//Rm input value
input[31:0]		regValRn;		//Rn input value
input[31:0]		immValRi;		//immediate/disp value
input[31:0]		idInGenPc;		//PC to next instruction

output[31:0]	regOutVal;		//Rn output value
output[5:0]		regOutId;		//Rn, value to write
output[1:0]		regOutOK;		//execute status

/* Memory */
output[31:0]	memAddr;		//memory address
output[31:0]	memData;		//memory data (store)
output[4:0]		memOpm;			//mem op mode
output[7:0]		memOpCmd2;		//mem EX chain

input[31:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)


/* Special Registers */
input[31:0]		ctlInSp;		//SP in
output[31:0]	ctlOutSp;		//SP out

input[31:0]		ctlInPc;		//PC in
output[31:0]	ctlOutPc;		//PC out

input[31:0]		ctlInPr;		//PR in
output[31:0]	ctlOutPr;		//PR out

input[31:0]		ctlInSr;		//SR in
output[31:0]	ctlOutSr;		//SR out

input[31:0]		ctlInVbr;
output[31:0]	ctlOutVbr;

input[31:0]		ctlInDlr;		//MACL in
output[31:0]	ctlOutDlr;		//MACL out

input[31:0]		ctlInDhr;		//MACH in
output[31:0]	ctlOutDhr;		//MACH out

input[31:0]		ctlInGbr;
output[31:0]	ctlOutGbr;

input[31:0]		ctlInTbr;
output[31:0]	ctlOutTbr;


reg[31:0]	tCtlOutSp;		//SP out
reg[31:0]	tCtlOutPc;		//PC out
reg[31:0]	tCtlOutPr;		//PR out
reg[31:0]	tCtlOutSr;		//SR out
reg[31:0]	tCtlOutVbr;
reg[31:0]	tCtlOutDlr;		//MACL out
reg[31:0]	tCtlOutDhr;		//MACH out
reg[31:0]	tCtlOutGbr;
reg[31:0]	tCtlOutTbr;

assign	ctlOutSp	= tCtlOutSp;
assign	ctlOutPc	= tCtlOutPc;
assign	ctlOutPr	= tCtlOutPr;
assign	ctlOutSr	= tCtlOutSr;
assign	ctlOutVbr	= tCtlOutVbr;
assign	ctlOutDlr	= tCtlOutDlr;
assign	ctlOutDhr	= tCtlOutDhr;
assign	ctlOutGbr	= tCtlOutGbr;
assign	ctlOutTbr	= tCtlOutTbr;

reg[31:0]	tRegOutVal;		//Rn output value
reg[5:0]	tRegOutId;		//Rn, value to write
reg[1:0]	tRegOutOK;		//execute status
reg[31:0]	tMemAddr;		//memory address
reg[31:0]	tMemData;		//memory data (store)
reg[4:0]	tMemOpm;		//mem op mode
reg[7:0]	tMemOpCmd2;		//mem EX chain

assign	regOutVal	= tRegOutVal;
assign	regOutId	= tRegOutId;
assign	regOutOK	= tRegOutOK;
assign	memAddr		= tMemAddr;
assign	memData		= tMemData;
assign	memOpm		= tMemOpm;
assign	memOpCmd2	= tMemOpCmd2;




reg[31:0]		regMaRbVal;
reg[31:0]		regMaIxVal;
reg[31:0]		regMaIxVal2;
reg[3:0]		regMaIxt;
reg[2:0]		regMaBty;
reg[31:0]		regMaAddr;

reg[31:0]		regRnSxt;

reg[31:0]		regMulA;
reg[31:0]		regMulB;
reg[1:0]		regMulOp;

reg[31:0]		regMulDlr;
reg[31:0]		regMulDhr;
reg[1:0]		regMulOpDr;

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

	tRegOutVal	= 0;
	tRegOutId	= BSR_REG_ZZR;
	tRegOutOK	= UMEM_OK_OK;
	tMemAddr	= 0;
	tMemData	= 0;
	tMemOpm		= UMEM_OPM_READY;
	tMemOpCmd2	= BSR_UCMD_NOP;


	regRnSxt	= regValRn[31] ? UV32_FF : UV32_00;

	regMulA		= UV32_XX;
	regMulB		= UV32_XX;
	regMulOp	= 0;

	regMaIxt=regIdIxt[7:4];
	regMaBty=regIdIxt[2:0];
	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;
	
/*
	case(regMaIxt)
	BSR_IXT_REG:
		regMaIxVal=0;
	BSR_IXT_RDL:
		regMaIxVal=ctlInDlr;
	BSR_IXT_RDI:
		regMaIxVal=immValRi;
	BSR_IXT_RD4:
		regMaIxVal={ctlInDlr[27:0], immValRi[3:0]};
	BSR_IXT_ADL:
		regMaIxVal=0;
	default:
		regMaIxVal=0;
	endcase
*/

	case(regMaIxt[1:0])
	BSR_IX2_REG:
		regMaIxVal=0;
	BSR_IX2_RDL:
		regMaIxVal=ctlInDlr;
	BSR_IX2_RDI:
		regMaIxVal=immValRi;
	BSR_IX2_RD4:
		regMaIxVal={ctlInDlr[27:0], immValRi[3:0]};
	endcase

	case(regMaBty[1:0])
		2'b00: regMaIxVal2=regMaIxVal;
		2'b01: regMaIxVal2={regMaIxVal[30:0], 1'b0};
		2'b10: regMaIxVal2={regMaIxVal[29:0], 2'b0};
		2'b11: regMaIxVal2={regMaIxVal[28:0], 3'b0};
	endcase
	
	regMaAddr=regMaRbVal+regMaIxVal2;
	
	case(opCmd)

	BSR_UCMD_NOP: begin
	end

	BSR_UCMD_MOV_RR: begin
		tRegOutVal	= regValRm;
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_MOV_IR: begin
		tRegOutVal	= immValRi;
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_LEA_MR: begin
		tRegOutVal	= regMaAddr;
		tRegOutId	= regIdRn;
	end

//	BSR_UCMD_MOVX_WB: begin
//		tRegOutVal	= memDataLd;
//		tRegOutId	= regIdRn;
//	end
	BSR_UCMD_MOVB_RM: begin
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
	BSR_UCMD_MOVW_RM: begin
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
	BSR_UCMD_MOVL_RM: begin
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

	BSR_UCMD_MOVB_MR: begin
		tMemAddr	= regMaAddr;
		tMemOpm		= UMEM_OPM_RD_SB;
//		tMemOpCmd2	= BSR_UCMD_MOVX_WB;
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
	BSR_UCMD_MOVW_MR: begin
		tMemAddr	= regMaAddr;
		tMemOpm		= UMEM_OPM_RD_SW;
//		tMemOpCmd2	= BSR_UCMD_MOVX_WB;
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
	BSR_UCMD_MOVL_MR: begin
		tMemAddr	= regMaAddr;
		tMemOpm		= UMEM_OPM_RD_SL;
//		tMemOpCmd2	= BSR_UCMD_MOVX_WB;
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
	BSR_UCMD_MOVUB_MR: begin
		tMemAddr	= regMaAddr;
		tMemOpm		= UMEM_OPM_RD_UB;
//		tMemOpCmd2	= BSR_UCMD_MOVX_WB;
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
	BSR_UCMD_MOVUW_MR: begin
		tMemAddr	= regMaAddr;
		tMemOpm		= UMEM_OPM_RD_UW;
//		tMemOpCmd2	= BSR_UCMD_MOVX_WB;
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


	BSR_UCMD_MOV_PUSH: begin
//		$display("PUSH %X %X", ctlInSp-4, regValRn);
		tCtlOutSp	= ctlInSp-4;
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

	BSR_UCMD_MOV_POP: begin
		tCtlOutSp	= ctlInSp+4;
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

		if(regIdRn==BSR_REG_PC)
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


	BSR_UCMD_ALU_ADD: begin
		tRegOutVal	= regValRn + regValRm;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_SUB: begin
		tRegOutVal	= regValRn - regValRm;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_ADC: begin
		{ tCtlOutSr[0], tRegOutVal }	=
			{ 1'b0, regValRn } +
			{ 1'b0, regValRm } +
			{ 31'h0, ctlInSr[0]};
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_SBB: begin
		{ tCtlOutSr[0], tRegOutVal }	=
			{ 1'b0, regValRn } +
			{ 1'b0, ~regValRm } +
			{ 31'h0, ~ctlInSr[0]};
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_TST: begin
		tCtlOutSr[0] = ((regValRn & regValRm)==0);
	end
	BSR_UCMD_ALU_AND: begin
		tRegOutVal	= regValRn & regValRm;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_OR: begin
		tRegOutVal	= regValRn | regValRm;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_XOR: begin
		tRegOutVal	= regValRn ^ regValRm;
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_ALU_ADD2: begin
		tRegOutVal	= regValRm + ctlInDlr;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_SUB2: begin
		tRegOutVal	= regValRm - ctlInDlr;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_AND2: begin
		tRegOutVal	= regValRm & ctlInDlr;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_OR2: begin
		tRegOutVal	= regValRm | ctlInDlr;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_XOR2: begin
		tRegOutVal	= regValRm ^ ctlInDlr;
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_ALU_MULU: begin
		regMulA		= regValRn;
		regMulB		= regValRm;
		regMulOp	= 2'h1;
		tCtlOutDlr	= regMulDlr;
		tCtlOutDhr	= regMulDhr;

		if(regMulOpDr!=regMulOp)
			tRegOutOK	= UMEM_OK_HOLD;

	end

	BSR_UCMD_ALU_MULS: begin
		regMulA		= regValRn;
		regMulB		= regValRm;
		regMulOp	= 2'h2;
		tCtlOutDlr	= regMulDlr;
		tCtlOutDhr	= regMulDhr;
		
		if(regMulOpDr!=regMulOp)
		begin
			tRegOutOK	= UMEM_OK_HOLD;
		end
		else
		begin
//			$display("MULS %X:%X", tCtlOutDhr, tCtlOutDlr);
		end

	end
	
	BSR_UCMD_ALU_LDIX: begin
		tCtlOutDlr	= immValRi;
	end
	BSR_UCMD_ALU_LDISH: begin
		tCtlOutDlr	= { ctlInDlr[23:0], immValRi[7:0] };
	end

	BSR_UCMD_ALU_LDISH16: begin
		tCtlOutDlr	= { ctlInDlr[15:0], immValRi[15:0] };
	end

	BSR_UCMD_ALU_CMPEQ: begin
		tCtlOutSr[0] = (regValRn == regValRm);
//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
	end
	BSR_UCMD_ALU_CMPHI: begin
		tCtlOutSr[0] = (regValRn > regValRm);
//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
	end
	BSR_UCMD_ALU_CMPGT: begin
		tCtlOutSr[0] = (regValRn > regValRm) ^
			(regValRn[31] ^ regValRm[31]);
//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
	end
	BSR_UCMD_ALU_CMPGE: begin
		tCtlOutSr[0] = (regValRn >= regValRm) ^
			(regValRn[31] ^ regValRm[31]);
//		$display("CMPGE %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
	end

	BSR_UCMD_CF_BRA: begin
//		$display("BRA %X", regMaAddr);
		tCtlOutPc	= regMaAddr;
		tMemOpm		= UMEM_OPM_CTRLF;
	end
	BSR_UCMD_CF_BSR: begin
//		$display("BSR %X", regMaAddr);
		tCtlOutPr	= idInGenPc;
		tCtlOutPc	= regMaAddr;
		tMemOpm		= UMEM_OPM_CTRLF;
	end
	BSR_UCMD_CF_BT: begin
//		$display("BT %X %X", regMaAddr, ctlInSr[0]);
		if(ctlInSr[0])
		begin
//			$display("BT %X", regMaAddr);
			tCtlOutPc	= regMaAddr;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
	end
	BSR_UCMD_CF_BF: begin
//		$display("BF %X %X", regMaAddr, ctlInSr[0]);
		if(!ctlInSr[0])
		begin
//			$display("BF %X", regMaAddr);
			tCtlOutPc	= regMaAddr;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
	end

	BSR_UCMD_CF_JMP: begin
//		$display("JMP %X", regMaAddr);
		tCtlOutPc	= regValRn;
		tMemOpm		= UMEM_OPM_CTRLF;
	end
	BSR_UCMD_CF_JSR: begin
//		$display("JSR %X", regMaAddr);
		tCtlOutPr	= idInGenPc;
		tCtlOutPc	= regValRn;
		tMemOpm		= UMEM_OPM_CTRLF;
	end
	BSR_UCMD_CF_JT: begin
		if(ctlInSr[0])
		begin
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
	end
	BSR_UCMD_CF_JF: begin
		if(!ctlInSr[0])
		begin
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
	end

	BSR_UCMD_ALU_EXTUB: begin
		tRegOutVal	= { UV24_00, regValRn[7:0] };
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_EXTUW: begin
		tRegOutVal	= { UV16_00, regValRn[15:0] };
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_EXTSB: begin
		tRegOutVal	= { regValRn[7]?UV24_FF:UV24_00, regValRn[7:0] };
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_EXTSW: begin
		tRegOutVal	= { regValRn[15]?UV16_FF:UV16_00, regValRn[15:0] };
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_ALU_NOT: begin
		tRegOutVal	= ~regValRn;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_NEG: begin
		tRegOutVal	= -regValRn;
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_NEGC: begin
		tRegOutVal	= (~regValRn) +
			{ 31'h0, ~ctlInSr[0]};
		tRegOutId	= regIdRn;
	end

	BSR_UCMD_ALU_ROTL: begin
		tRegOutVal	= { regValRn[30:0], regValRn[31] };
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_ROTR: begin
		tRegOutVal	= { regValRn[0], regValRn[31:1] };
		tRegOutId	= regIdRn;
	end
	BSR_UCMD_ALU_ROTCL: begin
		tCtlOutSr[0]	= regValRn[31];
		tRegOutVal		= { regValRn[30:0], ctlInSr[0] };
		tRegOutId		= regIdRn;
	end
	BSR_UCMD_ALU_ROTCR: begin
		tCtlOutSr[0]	= regValRn[0];
		tRegOutVal		= { ctlInSr[0], regValRn[31:1] };
		tRegOutId		= regIdRn;
	end

	BSR_UCMD_ALU_SHLL: begin
		tCtlOutSr[0]	= regValRn[31];
		tRegOutVal		= { regValRn[30:0], 1'b0 };
		tRegOutId		= regIdRn;
	end
	BSR_UCMD_ALU_SHLR: begin
		tCtlOutSr[0]	= regValRn[0];
		tRegOutVal		= { 1'b0, regValRn[31:1] };
		tRegOutId		= regIdRn;
	end
	BSR_UCMD_ALU_SHAR: begin
		tCtlOutSr[0]	= regValRn[0];
		tRegOutVal		= { regValRn[31], regValRn[31:1] };
		tRegOutId		= regIdRn;
	end

	BSR_UCMD_ALU_SHLLN: begin
		tRegOutId		= regIdRn;
		case(regIdIxt[4:0])
			5'h01: tRegOutVal = { regValRn[30:0], 1'b0 };
			5'h02: tRegOutVal = { regValRn[29:0], 2'b0 };
			5'h04: tRegOutVal = { regValRn[27:0], 4'b0 };
			5'h08: tRegOutVal = { regValRn[23:0], 8'b0 };
			5'h10: tRegOutVal = { regValRn[15:0], 16'b0 };
			default:	tRegOutVal = UV32_XX;
		endcase
	end
	BSR_UCMD_ALU_SHLRN: begin
		tRegOutId		= regIdRn;
		case(regIdIxt[4:0])
			5'h01: tRegOutVal = {  1'b0, regValRn[31: 1] };
			5'h02: tRegOutVal = {  2'b0, regValRn[31: 2] };
			5'h04: tRegOutVal = {  4'b0, regValRn[31: 4] };
			5'h08: tRegOutVal = {  8'b0, regValRn[31: 8] };
			5'h10: tRegOutVal = { 16'b0, regValRn[31:16] };
			default:	tRegOutVal = UV32_XX;
		endcase
	end
	BSR_UCMD_ALU_SHARN: begin
		tRegOutId		= regIdRn;
		case(regIdIxt[4:0])
			5'h01: tRegOutVal = { regRnSxt[   0], regValRn[31: 1] };
			5'h02: tRegOutVal = { regRnSxt[ 1:0], regValRn[31: 2] };
			5'h04: tRegOutVal = { regRnSxt[ 3:0], regValRn[31: 4] };
			5'h08: tRegOutVal = { regRnSxt[ 7:0], regValRn[31: 8] };
			5'h10: tRegOutVal = { regRnSxt[15:0], regValRn[31:16] };
			default:	tRegOutVal = UV32_XX;
		endcase
	end

	BSR_UCMD_ALU_SHARSX: begin
		tRegOutId		= regIdRn;
		tRegOutVal		= regRnSxt;
	end

	BSR_UCMD_OP_IXT: begin
		case(regIdIxt[7:0])
			BSR_UCMD_IX_NOP: begin
			end

			BSR_UCMD_IX_RTS: begin
				tCtlOutPc	= ctlInPr;
				tMemOpm		= UMEM_OPM_CTRLF;
			end

			BSR_UCMD_IX_BREAK: begin
//				tCtlOutPc	= ctlInPr;
				tRegOutOK	= UMEM_OK_HOLD;
			end

			BSR_UCMD_IX_CLRT: begin
				tCtlOutSr[0] = 1'b0;
			end
			BSR_UCMD_IX_SETT: begin
				tCtlOutSr[0] = 1'b1;
			end
			BSR_UCMD_IX_CLRS: begin
				tCtlOutSr[1] = 1'b0;
			end
			BSR_UCMD_IX_SETS: begin
				tCtlOutSr[1] = 1'b1;
			end

			BSR_UCMD_IX_NOTT: begin
				tCtlOutSr[0] = !ctlInSr[0];
			end
			BSR_UCMD_IX_NOTS: begin
				tCtlOutSr[1] = !ctlInSr[1];
			end

			default: begin
			$display("BsrExOp: Missing %X-%X", opCmd, regIdIxt);
			tRegOutOK	= UMEM_OK_HOLD;
			end
		endcase
	end

	default: begin
//		$display("BsrExOp: Missing %X", opCmd);
		tRegOutOK	= UMEM_OK_HOLD;
		regMulOp	= 1;
		if(regMulOpDr != regMulOp)
		begin
			$display("BsrExOp: Missing %X", opCmd);
		end
	end

	endcase

end


reg[31:0]	regMulPiAA;
reg[31:0]	regMulPiAB;
reg[31:0]	regMulPiBA;
reg[31:0]	regMulPiBB;
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
	regMulPiAA	= { 16'h0, regMulA[15: 0] };
	regMulPiAB	= { 16'h0, regMulA[31:16] };
	regMulPiBA	= { 16'h0, regMulB[15: 0] };
	regMulPiBB	= { 16'h0, regMulB[31:16] };

//	regMulPiAC	= { regMulA[31]?16'hFFFF:16'h0000, regMulA[31:16] };
//	regMulPiBC	= { regMulB[31]?16'hFFFF:16'h0000, regMulB[31:16] };
// * /

	regMulPiAC	= regMulA[31]?(-regMulB):UV32_00;
	regMulPiBC	= regMulB[31]?(-regMulA):UV32_00;

//	regMulPpA1	= regMulPiAA * regMulPiBA;
//	regMulPpB1	= regMulPiAA * regMulPiBB;
//	regMulPpC1	= regMulPiAB * regMulPiBA;
//	regMulPpD1	= regMulPiAB * regMulPiBB;

	regMulOpPi	= regMulOp;

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
//	regMulPiAA	<= { 16'h0, regMulA[15: 0] };
//	regMulPiAB	<= { 16'h0, regMulA[31:16] };
//	regMulPiBA	<= { 16'h0, regMulB[15: 0] };
//	regMulPiBB	<= { 16'h0, regMulB[31:16] };

	regMulPpA	<= regMulPiAA * regMulPiBA;
	regMulPpB	<= regMulPiAA * regMulPiBB;
	regMulPpC	<= regMulPiAB * regMulPiBA;
	regMulPpD	<= regMulPiAB * regMulPiBB;

	case(regMulOp)
		2'h0: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end

		2'h1: begin
/*
			regMulPpA	<= regMulPiAA * regMulPiBA;
			regMulPpB	<= regMulPiAA * regMulPiBB;
			regMulPpC	<= regMulPiAB * regMulPiBA;
			regMulPpD	<= regMulPiAB * regMulPiBB;
*/
//			regMulPpA	<= regMulPpA1;
//			regMulPpB	<= regMulPpB1;
//			regMulPpC	<= regMulPpC1;
//			regMulPpD	<= regMulPpD1;

			regMulPpE	<= 0;

			regMulOpPp	<= regMulOpPi;

//			regMulOpPj	<= regMulOpPi;
//			regMulOpPp	<= regMulOpPj;

			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
		end

		2'h2: begin
/*
			regMulPpA	<= regMulPiAA * regMulPiBA;
			regMulPpB	<= regMulPiAA * regMulPiBB;
			regMulPpC	<= regMulPiAB * regMulPiBA;
			regMulPpD	<= regMulPiAC * regMulPiBC;
*/

//			regMulPpA	<= regMulPpA1;
//			regMulPpB	<= regMulPpB1;
//			regMulPpC	<= regMulPpC1;
//			regMulPpD	<= regMulPpD1;

			regMulPpE	<= regMulPiAC + regMulPiBC;

			regMulOpPp	<= regMulOpPi;

//			regMulOpPj	<= regMulOpPi;
//			regMulOpPp	<= regMulOpPj;

			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
			
			$display("BsrExOp: Mul A: %X %X", regMulA, regMulB);
			$display("BsrExOp: Mul B: %X %X %X %X",
				regMulPpA, regMulPpB,
				regMulPpC, regMulPpD);
			$display("BsrExOp: Mul C: %X %X", regMulDlr, regMulDhr);
			
		end

		2'h3: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end
	endcase
end

endmodule

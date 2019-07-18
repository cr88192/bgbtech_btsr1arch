/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.

RegReg
  SB: Rm,Rn => Rm,Rn,Rn
 */

`include "CoreDefs.v"

// `include "DecOpFx.v"

module DecOpBz(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word

output[5:0]		idRegN;
output[5:0]		idRegM;
output[5:0]		idRegO;
output[32:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;

reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[5:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[5:0]	opRegO_Dfl;
reg[5:0]	opRegN_Dfl;
reg[5:0]	opRegM_Dfl;

reg[5:0]	opRegN_Cr;
reg[5:0]	opRegM_Cr;
reg[5:0]	opRegN_Sr;
reg[5:0]	opRegM_Sr;

reg[5:0]	opRegO_Er;
reg[5:0]	opRegN_Er;		//Rk, R16..R31
reg[5:0]	opRegM_Er;		//Rj, R16..R31

reg[5:0]	opRegN_ECr;		//C16..C31
reg[5:0]	opRegN_ESr;		//S16..S31

reg[5:0]	opRegN_Xr;		//3znz 1R, R0..R31
reg[5:0]	opRegN_Yr;		//2znz/6znz 1R, R0..R31

/*
Fixed registers for certain ops.
Ro:Rm, may also be used as an immed.
 */
reg[5:0]	opRegO_Fix;
reg[5:0]	opRegN_Fix;
reg[5:0]	opRegM_Fix;

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[1:0]	opCcty;

reg[5:0]	opUCmdIx;

// reg opIs32;

reg tRegRmIsRz;
reg tRegRnIsRz;
reg tRegRoIsRz;
reg tRegRmIsR0;
reg tRegRmIsR1;
reg tRegRnIsR0;
reg tRegRnIsR1;

reg tRegRmIsRs;
reg tRegRnIsRs;
reg tRegRoIsRs;

reg opIsFx;

reg	tMsgLatch;
reg	tNextMsgLatch;


always @*
begin

	tRegRoIsRz	= (istrWord[11:9]==3'b000);
	tRegRnIsRz	= (istrWord[ 7:5]==3'b000);
	tRegRmIsRz	= (istrWord[ 3:1]==3'b000);
	tRegRnIsR0	= tRegRnIsRz && !istrWord[4];
	tRegRnIsR1	= tRegRnIsRz &&  istrWord[4];
	tRegRmIsR0	= tRegRmIsRz && !istrWord[0];
	tRegRmIsR1	= tRegRmIsRz &&  istrWord[0];

	tRegRoIsRs = tRegRoIsRz || (istrWord[11:8]==4'b1111);
	tRegRnIsRs = tRegRnIsRz || (istrWord[ 7:4]==4'b1111);
	tRegRmIsRs = tRegRmIsRz || (istrWord[ 3:0]==4'b1111);

	opRegO_Dfl	= {tRegRoIsRs, 1'b0, istrWord[11:8]};
	opRegN_Dfl	= {tRegRnIsRs, 1'b0, istrWord[ 7:4]};
	opRegM_Dfl	= {tRegRmIsRs, 1'b0, istrWord[ 3:0]};

	opRegN_Cr	= {2'b10, istrWord[ 7:4]};
	opRegM_Cr	= {2'b10, istrWord[ 3:0]};

	opRegN_Sr	= {2'b11, istrWord[ 7:4]};
	opRegM_Sr	= {2'b11, istrWord[ 3:0]};

//	opRegO_Er	= {2'b01, istrWord[11:8]};
//	opRegN_Er	= {2'b01, istrWord[ 7:4]};
//	opRegM_Er	= {2'b01, istrWord[ 3:0]};
	opRegO_Er	= {tRegRoIsRs, 1'b1, istrWord[11:8]};
	opRegN_Er	= {tRegRnIsRs, 1'b1, istrWord[ 7:4]};
	opRegM_Er	= {tRegRmIsRs, 1'b1, istrWord[ 3:0]};

	opRegN_ECr	= opRegN_Cr;
	opRegN_ESr	= opRegN_Sr;
	
	opRegN_Xr	= {tRegRnIsRs, istrWord[11], istrWord[ 7:4]};
	opRegN_Yr	= {tRegRnIsRs, istrWord[14], istrWord[ 7:4]};

	opRegN_Fix	= JX2_GR_ZZR;
	opRegM_Fix	= JX2_GR_ZZR;
	opRegO_Fix	= JX2_GR_ZZR;


	opNmid		= JX2_UCMD_INVOP;
	opRegN		= JX2_GR_ZZR;
	opRegM		= JX2_GR_ZZR;
	opRegO		= JX2_GR_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIty		= 0;
	opIsFx		= 0;
	opCcty		= JX2_IXC_AL;

	tNextMsgLatch	= 0;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[15:0])

		16'h00zz: begin
			if(istrWord[7:0]==8'h00)
			begin
				opNmid	= JX2_UCMD_NOP;
				opFmid	= JX2_FMID_Z;
			end
			else
			begin
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h01zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SW;
		end
		16'h02zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SL;
		end
		16'h03zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h04zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h05zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SW;
		end
		16'h06zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SL;
		end
		16'h07zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h08zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h09zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h0Azz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h0Bzz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h0Czz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h0Dzz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h0Ezz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h0Fzz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h10zz: begin		/* ADD Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
		end
		16'h11zz: begin		/* SUB Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_SUB;
		end
		16'h12zz: begin		/* ADC Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_ADC;
		end
		16'h13zz: begin		/* SBB Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_SBB;
		end
		16'h14zz: begin		/* TST Rm, Rn */
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_TST;
		end
		16'h15zz: begin		/* AND Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_AND;
		end
		16'h16zz: begin		/* OR Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_OR;
		end
		16'h17zz: begin		/* XOR Rm, Rn */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_ALU_XOR;
		end

		16'h18zz: begin		/* MOV Rm, Rn */
			opNmid		= JX2_UCMD_CONV_RR;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_CONV_MOV;
		end
		16'h19zz: begin		/* MOV Rj, Rn */
			opNmid		= JX2_UCMD_CONV_RR;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_CONV_MOV;
		end
		16'h1Azz: begin		/* MOV Rm, Rk */
			opNmid		= JX2_UCMD_CONV_RR;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SL;
			opUCmdIx	= JX2_UCIX_CONV_MOV;
		end
		16'h1Bzz: begin		/* MOV Rj, Rk */
			opNmid		= JX2_UCMD_CONV_RR;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SQ;
			opUCmdIx	= JX2_UCIX_CONV_MOV;
		end

		16'h1Czz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
		end
		16'h1Dzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPHI;
		end
		16'h1Ezz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPGT;
		end
		16'h1Fzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPGE;
		end


		16'h20zz: begin
			opNmid	= JX2_UCMD_BRA;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
		end
		16'h21zz: begin
			opNmid	= JX2_UCMD_BSR;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
		end
		16'h22zz: begin
			opNmid	= JX2_UCMD_BRA;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
			opCcty	= JX2_IXC_CT;
		end
		16'h23zz: begin
			opNmid	= JX2_UCMD_BRA;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
			opCcty	= JX2_IXC_CF;
		end
		16'h24zz: begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8Z;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end
		16'h25zz: begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8N;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end
		16'h26zz: begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8Z;
			opUCmdIx	= JX2_UCIX_LDI_LDISH8;
		end

`ifndef def_true
		16'h28zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opBty	= JX2_BTY_SL;
		end
		16'h29zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opBty	= JX2_BTY_SQ;
		end
		16'h2Azz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opBty	= JX2_BTY_SL;
		end
		16'h2Bzz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opBty	= JX2_BTY_SQ;
		end
`endif

`ifdef def_true
		16'h28zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_SB;
			opBty	= JX2_BTY_UL;
		end

		16'h2Azz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_UB;
			opBty	= JX2_BTY_UL;
		end
`endif

		16'h2Czz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
		end
		16'h2Dzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4NREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
		end
		16'h2Ezz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPGT;
		end
		16'h2Fzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPGE;
		end

		16'h3zzz: casez(istrWord[10:0])
			11'h0zz: casez(istrWord[7:0])
				8'h00: begin
					opNmid		= JX2_UCMD_NOP;
					opFmid		= JX2_FMID_Z;
				end
				8'h10: begin
					opNmid		= JX2_UCMD_JMP;
					opFmid		= JX2_FMID_Z;
					opRegM_Fix	= JX2_GR_LR;
				end
				8'h20: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_SLEEP;
				end
				8'h30: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_BREAK;
				end
				8'h40: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_CLRT;
				end
				8'h50: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_SETT;
				end
				8'h60: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_CLRS;
				end
				8'h70: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_SETS;
				end
				8'h80: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_NOTT;
				end
				8'h90: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_NOTS;
				end

`ifndef def_true
				8'hA0: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_Z;
					opImm		= 33'h00000000;
					opUCmdIx	= JX2_UCIX_LDI_LDISH16;
				end
				8'hB0: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_Z;
					opImm		= 33'h0000FFFF;
					opUCmdIx	= JX2_UCIX_LDI_LDISH16;
				end
`endif

				8'hC0: begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_RTE;
				end

				8'h12: begin
					opNmid		= JX2_UCMD_JMP;
					opFmid		= JX2_FMID_Z;
					opRegM_Fix	= JX2_GR_LR;
				end
				
				default: begin
				end
			endcase

			11'h1z0: begin
				opNmid		= JX2_UCMD_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
	//			opIty		= JX2_ITY_UB;
			end
			11'h1z1: begin
				opNmid		= JX2_UCMD_BSR;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
			end
			11'h1z2: begin
				opNmid		= JX2_UCMD_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
				opCcty		= JX2_IXC_CT;
			end
			11'h1z3: begin
				opNmid		= JX2_UCMD_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
				opCcty		= JX2_IXC_CF;
			end

			11'h1z6: begin
				opNmid	= JX2_UCMD_SHADQ3;
				opFmid	= JX2_FMID_DRREG;
				opIty	= JX2_ITY_NB;
			end
			11'h1z7: begin
				opNmid	= JX2_UCMD_SHLDQ3;
				opFmid	= JX2_FMID_DRREG;
				opIty	= JX2_ITY_NB;
			end

			11'h1z8: begin
				opNmid		= JX2_UCMD_PUSHX;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_PUSH_GR;
			end
			11'h1z9: begin
				opNmid		= JX2_UCMD_PUSHX;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_NB;
				opUCmdIx	= JX2_UCIX_PUSH_CR;
			end
			11'h1zA: begin
				opNmid		= JX2_UCMD_POPX;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_PUSH_GR;
			end
			11'h1zB: begin
				opNmid		= JX2_UCMD_POPX;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_UB;
				opUCmdIx	= JX2_UCIX_PUSH_CR;

`ifndef def_true
				if(istrWord[7:4]==0)
				begin
	//				opNmid	= JX2_UCMD_RET;
	//				opFmid	= JX2_FMID_Z;
	//				opBty	= JX2_BTY_UB;
				end
				else
				begin
					opNmid		= JX2_UCMD_POPX;
					opFmid		= JX2_FMID_REG;
					opIty		= JX2_ITY_UB;
					opUCmdIx	= JX2_UCIX_PUSH_CR;
				end
`endif
			end

`ifndef def_true
			11'h1zC: begin
				opNmid		= JX2_UCMD_ALUCMP;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_CMPGT;
			end
			11'h1zD: begin
				opNmid		= JX2_UCMD_ALUCMP;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_CMPGE;
			end
`endif

			11'h2z0: begin
				opNmid		= JX2_UCMD_JMP;
				opFmid		= JX2_FMID_REG;
			end
			11'h2z1: begin
				opNmid		= JX2_UCMD_JSR;
				opFmid		= JX2_FMID_REG;
			end
			11'h2z2: begin
				opNmid		= JX2_UCMD_JMP;
				opFmid		= JX2_FMID_REG;
				opCcty		= JX2_IXC_CT;
			end
			11'h2z3: begin
				opNmid		= JX2_UCMD_JMP;
				opFmid		= JX2_FMID_REG;
				opCcty		= JX2_IXC_CF;
			end
			11'h2z4: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTUB;
			end
			11'h2z5: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTUW;
			end
			11'h2z6: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTSB;
			end
			11'h2z7: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTSW;
			end

			11'h3z0: begin
//				opNmid		= JX2_UCMD_SHLLN;
//				opFmid		= JX2_FMID_REG;
//				opUCmdIx	= 6'h13;

				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_NOT;
			end
			11'h3z1: begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_ALU_SUB;
			end
			11'h3z2: begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_ALU_SBB;
			end
			11'h3z3: begin
				opNmid		= JX2_UCMD_OP_IXS;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXS_MOVNT;
			end

`ifdef jx2_enable_shlln
			11'h3z4: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3z5: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3z6: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3z7: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3z8: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3z9: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h3zA: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
`else
`endif

//			11'h3zB: begin
//				opNmid		= JX2_UCMD_ALU3;
//				opFmid		= JX2_FMID_DRREG;
//				opIty		= JX2_ITY_UL;
//				opUCmdIx	= JX2_UCIX_ALU_ADD;
//			end

			11'h3zC: begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_XB;
				opUCmdIx	= JX2_UCIX_ALU_SUB;
			end

`ifdef jx2_shlln_shadq
			11'h4zz: begin
				opNmid		= JX2_UCMD_SHLDQ3;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_XL;
//				opUCmdIx	= { 2'h0, istrWord[3:0] };
				opRegM_Fix = 0;
				opRegO_Fix = 0;
				case(istrWord[3:0])
					4'h0:	begin opRegM_Fix = 6'h01; opRegO_Fix=6'h00; end
					4'h1:	begin opRegM_Fix = 6'h02; opRegO_Fix=6'h00; end
					4'h2:	begin opRegM_Fix = 6'h04; opRegO_Fix=6'h00; end
					4'h3:	begin opRegM_Fix = 6'h08; opRegO_Fix=6'h00; end
					4'h4:	begin opRegM_Fix = 6'h3F; opRegO_Fix=6'h03; end
					4'h5:	begin opRegM_Fix = 6'h3E; opRegO_Fix=6'h03; end
					4'h6:	begin opRegM_Fix = 6'h3C; opRegO_Fix=6'h03; end
					4'h7:	begin opRegM_Fix = 6'h38; opRegO_Fix=6'h03; end
					4'h8:	begin opRegM_Fix = 6'h3F; opRegO_Fix=6'h13; end
					4'h9:	begin opRegM_Fix = 6'h3E; opRegO_Fix=6'h13; end
					4'hA:	begin opRegM_Fix = 6'h3C; opRegO_Fix=6'h13; end
					4'hB:	begin opRegM_Fix = 6'h38; opRegO_Fix=6'h13; end
					4'hC:	begin opRegM_Fix = 6'h10; opRegO_Fix=6'h00; end
					4'hD:	begin opRegM_Fix = 6'h30; opRegO_Fix=6'h03; end
					4'hE:	begin opRegM_Fix = 6'h30; opRegO_Fix=6'h13; end
					4'hF:	begin opRegM_Fix = 6'h01; opRegO_Fix=6'h13; end
				endcase

				opNmid	= opRegO_Fix[4]?JX2_UCMD_SHADQ3 : JX2_UCMD_SHLDQ3;
			end
`else
			11'h4zz: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h0, istrWord[3:0] };
			end
`endif

`ifdef jx2_shlln_shadq
			11'h6z0: begin
				opNmid		= JX2_UCMD_SHLDQ3;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_XL;
				opRegM_Fix	= 6'h20;
				opRegO_Fix	= 6'h00;
			end
			11'h6z1: begin
				opNmid		= JX2_UCMD_SHLDQ3;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_XL;
				opRegM_Fix	= 6'h20;
				opRegO_Fix	= 6'h03;
			end
			11'h6z2: begin
				opNmid		= JX2_UCMD_SHADQ3;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_XL;
				opRegM_Fix	= 6'h20;
				opRegO_Fix	= 6'h03;
			end
`else
			11'h6z0: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h6z1: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
			11'h6z2: begin
				opNmid		= JX2_UCMD_SHLLN;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= { 2'h1, istrWord[3:0] };
			end
`endif
			11'h6z3: begin
				opNmid		= JX2_UCMD_OP_IXT;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXT_TRAPA;
			end
			11'h6z4: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTUL;
			end
			11'h6z5: begin
				opNmid		= JX2_UCMD_CONV_RR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_CONV_EXTSL;
			end
			11'h6z6: begin
				opNmid	= JX2_UCMD_SHAD3;
				opFmid	= JX2_FMID_DRREG;
				opIty	= JX2_ITY_NB;
			end
			11'h6z7: begin
				opNmid	= JX2_UCMD_SHLD3;
				opFmid	= JX2_FMID_DRREG;
				opIty	= JX2_ITY_NB;
			end


			11'h6z9: begin
				opNmid		= JX2_UCMD_NOP;
				opFmid		= JX2_FMID_Z;
			end
			11'h6zA: begin
				opNmid		= JX2_UCMD_OP_IXT;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXT_CPUID;
			end

			11'h6zD: begin
				opNmid		= JX2_UCMD_ALUCMP;
				opFmid		= JX2_FMID_DRREG;
				opUCmdIx	= JX2_UCIX_ALU_CMPHS;
			end
			11'h6zE: begin
				opNmid		= JX2_UCMD_ALUCMP;
				opFmid		= JX2_FMID_DRREG;
				opUCmdIx	= JX2_UCIX_ALU_CMPGE;
			end
			11'h6zF: begin
				opNmid		= JX2_UCMD_OP_IXS;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXS_MOVT;
			end

			11'h7zz: begin
				opNmid		= JX2_UCMD_SWAPN;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= { 2'h0, istrWord[3:0] };
			end

`ifndef def_true
			11'h8z4: begin
				opNmid		= JX2_UCMD_OP_IXS;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXS_LDSRMSK;
			end
			11'h8z5: begin
				opNmid		= JX2_UCMD_OP_IXT;
				opFmid		= JX2_FMID_REG;
				opUCmdIx	= JX2_UCIX_IXT_PLDMSK;
			end
`endif

			default: begin
			end

		endcase

`ifdef def_true
		16'h40zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opIty	= JX2_ITY_SB;
			opBty	= JX2_BTY_SL;
		end
		16'h41zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opIty	= JX2_ITY_SB;
			opBty	= JX2_BTY_SQ;
		end

		16'h42zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opIty	= JX2_ITY_UB;
			opBty	= JX2_BTY_SL;
		end
		16'h43zz: begin
			opNmid	= JX2_UCMD_MOV_RM;
			opFmid	= JX2_FMID_REGSTDI4SP;
			opIty	= JX2_ITY_UB;
			opBty	= JX2_BTY_SQ;
		end
		16'h44zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_SB;
			opBty	= JX2_BTY_SL;
		end
		16'h45zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_SB;
			opBty	= JX2_BTY_SQ;
		end
		16'h46zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_UB;
			opBty	= JX2_BTY_SL;
		end
		16'h47zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDI4SPREG;
			opIty	= JX2_ITY_UB;
			opBty	= JX2_BTY_SQ;
		end
`endif

		16'h48zz: begin
			opNmid	= JX2_UCMD_MOV_RC;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UB;
		end
		16'h49zz: begin
			opNmid	= JX2_UCMD_MOV_CR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UW;
		end
		16'h4Azz: begin
			opNmid	= JX2_UCMD_MOV_RC;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UL;
		end
		16'h4Bzz: begin
			opNmid	= JX2_UCMD_MOV_CR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UQ;
		end

		16'h4Czz: begin
			opNmid	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h4Dzz: begin
			opNmid	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h4Ezz: begin
			opNmid	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h4Fzz: begin
			opNmid	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h50zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_UB;
		end
		16'h51zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_UW;
		end
		16'h52zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_UB;
		end
		16'h53zz: begin
			opNmid	= JX2_UCMD_MOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_UW;
		end
		16'h54zz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_TSTQ;
		end
		16'h55zz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPQEQ;
		end
		16'h56zz: begin
			opNmid		= JX2_UCMD_MUL3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_MUL3_MULS;
		end
		16'h57zz: begin
			opNmid		= JX2_UCMD_MUL3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_MUL3_MULU;
		end

		16'h58zz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
		end
		16'h59zz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_SUB;
		end
		16'h5Azz: begin
			opNmid		= JX2_UCMD_MUL3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
		end
		16'h5Bzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPQHI;
		end
		16'h5Czz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_CMPQGT;
		end

		16'h5Dzz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_AND;
		end
		16'h5Ezz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_OR;
		end
		16'h5Fzz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_XOR;
		end

		16'h60zz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FADD_G;
		end
		16'h61zz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FSUB_G;
		end
		16'h62zz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FMUL_G;
		end
		16'h63zz: begin
			opNmid		= JX2_UCMD_FLDCX;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPCX_SG;
		end
		16'h64zz: begin
			opNmid		= JX2_UCMD_FCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_CMPEQ_G;
		end
		16'h65zz: begin
			opNmid		= JX2_UCMD_FCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_CMPGT_G;
		end
		16'h66zz: begin
			opNmid		= JX2_UCMD_FSTCX;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPCX_SG;
		end


		16'h6Czz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
		end
		16'h6Dzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4NREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
		end
		16'h6Ezz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPGT;
		end
		16'h6Fzz: begin
			opNmid		= JX2_UCMD_ALUCMP;
			opFmid		= JX2_FMID_IMM4ZREG;
			opUCmdIx	= JX2_UCIX_ALU_CMPGE;
		end

		16'h8zzz: begin
			if(istrWord[10:8]!=0)
			begin
				if(istrWord[11])
				begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opBty	= JX2_BTY_SL;
					opIty	= JX2_ITY_UL;
				end
				else
				begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opBty	= JX2_BTY_SL;
					opIty	= JX2_ITY_UL;
				end
			end
			else
			begin
				if(istrWord[11])
				begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_UL;
				end
				else
				begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opBty	= JX2_BTY_UL;
				end
			end
		end


// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
		16'h90zz: begin
			opNmid	= JX2_UCMD_FMOV_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SL;
		end
		16'h91zz: begin
			opNmid	= JX2_UCMD_FMOV_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h92zz: begin
			opNmid	= JX2_UCMD_FMOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SL;
		end
		16'h93zz: begin
			opNmid	= JX2_UCMD_FMOV_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h94zz: begin
			opNmid	= JX2_UCMD_FMOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h95zz: begin
			opNmid	= JX2_UCMD_FMOV_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h96zz: begin
			opNmid	= JX2_UCMD_FMOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h97zz: begin
			opNmid	= JX2_UCMD_FMOV_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h98zz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FADD;
		end
		16'h99zz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FSUB;
		end
		16'h9Azz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FMUL;
		end

// `ifndef JX2_FPU_NOFDIV
`ifndef def_true
		16'h9Bzz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_FDIV;
		end
`endif

		16'h9Czz: begin
			opNmid		= JX2_UCMD_FCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_CMPEQ;
		end
		16'h9Dzz: begin
			opNmid		= JX2_UCMD_FCMP;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_UCIX_FPU_CMPGT;
		end
		16'h9Ezz: begin
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_FPU_FMOV;
		end

		16'h9Fz0: begin
			opNmid		= JX2_UCMD_FLDCX;
			opFmid		= JX2_FMID_DRREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPCX_S;
		end
		16'h9Fz1: begin
			opNmid		= JX2_UCMD_FLDCX;
			opFmid		= JX2_FMID_DRREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPCX_D;
		end
		16'h9Fz2: begin
			opNmid		= JX2_UCMD_FLDCX;
			opFmid		= JX2_FMID_DRREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPCX_I;
		end
		16'h9Fz3: begin
			opNmid		= JX2_UCMD_FLDCX;
			opFmid		= JX2_FMID_DRREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPCX_H;
		end

		16'h9Fz4: begin
			opNmid		= JX2_UCMD_FSTCX;
			opFmid		= JX2_FMID_DRREG;
//			opIty		= JX2_ITY_NB;
			opIty		= JX2_ITY_UB;
			opUCmdIx	= JX2_UCIX_FPCX_S;
		end
		16'h9Fz5: begin
			opNmid		= JX2_UCMD_FSTCX;
			opFmid		= JX2_FMID_DRREG;
//			opIty		= JX2_ITY_NB;
			opIty		= JX2_ITY_UB;
			opUCmdIx	= JX2_UCIX_FPCX_D;
		end
		16'h9Fz6: begin
			opNmid		= JX2_UCMD_FSTCX;
			opFmid		= JX2_FMID_DRREG;
//			opIty		= JX2_ITY_NB;
			opIty		= JX2_ITY_UB;
			opUCmdIx	= JX2_UCIX_FPCX_I;
		end
		16'h9Fz7: begin
			opNmid		= JX2_UCMD_FSTCX;
			opFmid		= JX2_FMID_DRREG;
//			opIty		= JX2_ITY_NB;
			opIty		= JX2_ITY_UB;
			opUCmdIx	= JX2_UCIX_FPCX_H;
		end

		16'h9Fz8: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPIX_FNEG;
		end
		16'h9Fz9: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_FPIX_FABS;
		end

// `ifndef JX2_FPU_NOFDIV
`ifndef def_true
		16'h9FzA: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCIX_FPIX_FRCPA;
		end
		16'h9FzB: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCIX_FPIX_FSQRTA;
		end
		16'h9FzC: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCIX_FPIX_FRCP;
		end
		16'h9FzD: begin
			opNmid		= JX2_UCMD_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCIX_FPIX_FSQRT;
		end
`endif

`endif

		16'hAzzz: begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM12Z;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end
		16'hBzzz: begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM12N;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end
		16'hCzzz: begin
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_IMM8REG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
		end

		16'hDzzz: begin
			opNmid	= JX2_UCMD_MOV_IR;
			opFmid	= JX2_FMID_IMM8REG;
		end

		16'hEzzz: begin
			opIsFx	= 1;
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_Z;
		end

		16'hFzzz: begin
			opIsFx	= 1;
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_Z;
		end
	
		default: begin
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

	endcase

	opUCmd = { opCcty, opNmid };
	
	case(opFmid)
		JX2_FMID_INV: begin
//			$display("Jx2DecOp: Inv %x", istrWord);
			$display("Jx2DecOp: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		end

		JX2_FMID_Z: begin
			opUIxt	= {opCcty, opUCmdIx[5:0]};
			opRegN	= opRegN_Fix;
			opRegM	= opRegM_Fix;
			opRegO	= opRegO_Fix;
		end
	
		/*
			SB: ZZR, Rx, Rx
			SW: ZZR, Rn, Rn
			SL: Rx, ZZR, Rx
			SQ: Rn, ZZR, Rn

			XB: ZZR, Rx, DLR
			XW: ZZR, Rn, DLR
			SL: Rx, FixImm, Rx
			SQ: Rn, FixImm, Rn
		 */
		JX2_FMID_REG: begin
			opUIxt	= {opCcty, opUCmdIx[5:0]};

			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Xr;
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Dfl;
				end

				JX2_ITY_SL: begin
					opRegN	= opRegN_Xr;
					opRegM	= opRegN_Xr;
					opRegO	= JX2_GR_ZZR;
				end
				JX2_ITY_SQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= JX2_GR_ZZR;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_ECr;
					opRegM	= JX2_GR_ZZR;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= JX2_GR_ZZR;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_ESr;
					opRegM	= JX2_GR_ZZR;
				end

				JX2_ITY_NB: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_Cr;
				end
				JX2_ITY_NW: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_ECr;
				end
				JX2_ITY_NL: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_Sr;
				end
				JX2_ITY_NQ: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_ESr;
				end

				JX2_ITY_XB: begin
					opRegN	= JX2_GR_DLR;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Xr;
				end
				JX2_ITY_XW: begin
					opRegN	= JX2_GR_DLR;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Dfl;
				end

				JX2_ITY_XL: begin
					opRegN	= opRegN_Xr;
					opRegM	= opRegN_Xr;
					opRegO	= JX2_GR_IMM;
					opImm	= {
						opRegO_Fix[5]?UV21_FF:UV21_00,
						opRegO_Fix, opRegM_Fix };
				end
				JX2_ITY_XQ: begin
					opRegN	= JX2_GR_DLR;
					opRegM	= opRegN_Xr;
					opRegO	= JX2_GR_IMM;
					opImm	= {
						opRegO_Fix[5]?UV21_FF:UV21_00,
						opRegO_Fix, opRegM_Fix };
				end

				default: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_ZZR;
				end
			endcase
		end

		JX2_FMID_IMM8: begin
			opUIxt	= {opCcty, opUCmdIx[5:0]};
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
		end
		
		
		/*
			SB: Rm, Rn / Rm, DLR, Rn
			SW: Rj, Rn / Rj, DLR, Rn
			SL: Rm, Rk / Rm, DLR, Rk
			SQ: Rj, Rk / Rj, DLR, Rk
			
			UB: Rm, Cn
			UW: Cm, Rn
			UL: Rm, Sn
			UQ: Sn, Rn
			
			NB: Rn, Rm, Rn
			NW: Rm, Rn, Rn
		 */
		
		JX2_FMID_REGREG: begin
//			opUIxt	= opUCmdIx;
			opUIxt	= {opCcty, opUCmdIx[5:0]};

			case(opIty)

				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
				end

				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Er;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_SL: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_SQ: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Er;
					opRegO	= JX2_GR_DLR;
				end
				
				JX2_ITY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Cr;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Sr;
					opRegO	= JX2_GR_DLR;
				end
				
				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
				end
				JX2_ITY_NW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
				end

				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
				end
			endcase
		end
		JX2_FMID_IMM8REG: begin
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
			opRegM	= JX2_GR_IMM;
			opRegN	= opRegO_Dfl;
			opRegO	= opRegO_Dfl;
//			opUIxt	= opUCmdIx;
			opUIxt	= {opCcty, opUCmdIx[5:0]};
		end

		JX2_FMID_IMM8Z: begin
			opImm	= {UV25_00, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end
		JX2_FMID_IMM8N: begin
			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_LDREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opIty)
				JX2_ITY_UL: begin
					opRegO	= JX2_GR_IMM;
					opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b10};
					opImm	= {UV30_00, istrWord[10:8]};
				end
				default:
				begin
					opRegO	= JX2_GR_ZZR;
					opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
				end
			endcase
		end

		JX2_FMID_LDDRREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_GR_DLR;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b00};
			end else begin
				opRegM	= opRegM_Dfl;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			end
		end

		JX2_FMID_LDDRPCREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_DLR;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
		end

		JX2_FMID_IMM4ZREG: begin
			opImm	= {UV29_00, istrWord[3:0]};
//			opRegN	= opRegN_Dfl;
			opRegN	= opRegN_Yr;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_IMM4NREG: begin
			opImm	= {UV29_FF, istrWord[3:0]};
//			opRegN	= opRegN_Dfl;
			opRegN	= opRegN_Yr;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_REGPC: begin
			opRegM	= JX2_GR_PC;
			opRegO	= opRegN_Xr;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
		end

		JX2_FMID_DRREG: begin
			opUIxt	= {opCcty, opUCmdIx };
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_GR_DLR;
					opRegO	= opRegN_Dfl;
				end

				JX2_ITY_UL: begin
					opRegM	= opRegN_Cr;
					opRegN	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Xr;
					opRegM	= opRegN_Xr;
					opRegO	= JX2_GR_DLR;
				end
				JX2_ITY_NW: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_DLR;
					opRegO	= opRegN_Xr;
				end

				default: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
				end
			endcase
		end

		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV21_00, istrWord[11:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV21_FF, istrWord[11:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_LDDLRREG: begin
			opRegM	= JX2_GR_DLR;
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_GR_ZZR;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
		end

		JX2_FMID_LDDI4SPREG: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_SP;
					opRegN	= opRegN_Dfl;
				end
				JX2_ITY_UB: begin
					opRegM	= JX2_GR_SP;
					opRegN	= opRegN_Er;
				end
				default: begin
					opRegM	= JX2_GR_SP;
					opRegN	= opRegN_Dfl;
				end
			endcase
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm	= {UV25_00, 3'b0, opBty[2], istrWord[3:0]};
		end

		default: begin
			opUCmd = {opCcty, JX2_UCMD_INVOP};
			if(!tMsgLatch)
			begin
				$display("Unhandled FMID (16) %X", opFmid);
				$display("Jx2DecOp: Istr %X",
					istrWord[15:0]);
			end
			tNextMsgLatch=1;
		end

	endcase
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

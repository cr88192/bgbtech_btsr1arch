/*
RISC-V C Instruction Decoder

Decodes 16-bit RVC Instruction.
 */

`include "CoreDefs.v"

module DecOpRvC(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	srMod,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
input[2:0]		srMod;		//mode

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
output[32:0]	idImm;
output[8:0]		idUCmd;
output[8:0]		idUIxt;


wire			srUser;				//Usermode
wire			srSuperuser;		//Superuser mode
assign		srUser = srMod[0];
assign		srSuperuser = (srMod[0] && srMod[1]) || (srMod[0] && srMod[2]);

`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
reg[32:0]		opImm;
reg[8:0]		opUCmd;
reg[8:0]		opUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegM_Dfl;

`reg_gpr	opRegN_Cr;
`reg_gpr	opRegM_Cr;
`reg_gpr	opRegN_Sr;
`reg_gpr	opRegM_Sr;

`reg_gpr	opRegO_Er;
`reg_gpr	opRegN_Er;		//Rk, R16..R31
`reg_gpr	opRegM_Er;		//Rj, R16..R31

`reg_gpr	opRegN_ECr;		//C16..C31
`reg_gpr	opRegN_ESr;		//S16..S31

`reg_gpr	opRegN_Xr;		//3znz 1R, R0..R31
`reg_gpr	opRegN_Yr;		//2znz/6znz 1R, R0..R31
`reg_gpr	opRegN_ZXr;		//zznz 1R, R0..R31 (xxxy -> yxxx0)

/*
Fixed registers for certain ops.
Ro:Rm, may also be used as an immed.
 */
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;
`reg_gpr	opRegM_Fix;

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[2:0]	opCcty;
reg[2:0]	opUCty;

reg[5:0]	opUCmdIx;

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

wire[15:0]	usrRejectCmMask;
wire[15:0]	usrRejectCnMask;
assign	usrRejectCmMask = 16'b1111_1111_0011_1000;
assign	usrRejectCnMask = 16'b1111_1111_1011_1100;

reg		usrRejectCmR;
reg		usrRejectCmW;
reg		usrRejectCnR;
reg		usrRejectCnW;
reg		usrReject;
reg[1:0]	usrSuAllow;
reg			usrSuAllowEn;

always @*
begin

	tRegRoIsRz	= (istrWord[11:9]==3'b000);
	tRegRnIsRz	= (istrWord[ 7:5]==3'b000);
	tRegRmIsRz	= (istrWord[ 3:1]==3'b000);
	tRegRnIsR0	= tRegRnIsRz && !istrWord[4];
	tRegRnIsR1	= tRegRnIsRz &&  istrWord[4];
	tRegRmIsR0	= tRegRmIsRz && !istrWord[0];
	tRegRmIsR1	= tRegRmIsRz &&  istrWord[0];

//	tRegRnIsR0	= tRegRnIsRz & !istrWord[4];
//	tRegRnIsR1	= tRegRnIsRz &  istrWord[4];
//	tRegRmIsR0	= tRegRmIsRz & !istrWord[0];
//	tRegRmIsR1	= tRegRmIsRz &  istrWord[0];

	tRegRoIsRs = tRegRoIsRz || (istrWord[11:8]==4'b1111);
	tRegRnIsRs = tRegRnIsRz || (istrWord[ 7:4]==4'b1111);
	tRegRmIsRs = tRegRmIsRz || (istrWord[ 3:0]==4'b1111);

//	tRegRoIsRs = tRegRoIsRz | (istrWord[11:8]==4'b1111);
//	tRegRnIsRs = tRegRnIsRz | (istrWord[ 7:4]==4'b1111);
//	tRegRmIsRs = tRegRmIsRz | (istrWord[ 3:0]==4'b1111);

	opRegO_Dfl	= {tRegRoIsRs, 2'b00, istrWord[11:8]};
	opRegN_Dfl	= {tRegRnIsRs, 2'b00, istrWord[ 7:4]};
	opRegM_Dfl	= {tRegRmIsRs, 2'b00, istrWord[ 3:0]};

	opRegN_Sr	= {3'b100, istrWord[ 7:4]};
	opRegM_Sr	= {3'b100, istrWord[ 3:0]};

	opRegN_Cr	= {3'b110, istrWord[ 7:4]};
	opRegM_Cr	= {3'b110, istrWord[ 3:0]};

	opRegO_Er	= {3'b001, istrWord[11:8]};
	opRegN_Er	= {3'b001, istrWord[ 7:4]};
	opRegM_Er	= {3'b001, istrWord[ 3:0]};

	opRegN_ECr	= opRegN_Cr;
	opRegN_ESr	= opRegN_Sr;
	
	opRegN_Xr	= {tRegRnIsRs && (!istrWord[11]), 1'b0,
		istrWord[11], istrWord[ 7:4]};
	opRegN_Yr	= {tRegRnIsRs && (!istrWord[14]), 1'b0,
		istrWord[14], istrWord[ 7:4]};

	opRegN_ZXr	= {tRegRnIsRs & (!istrWord[ 4]), 1'b0,
		istrWord[ 4], istrWord[ 7:5], 1'b0};

	usrReject	= 0;
	usrSuAllow	= 0;

	usrRejectCmR = usrRejectCmMask[opRegM_Cr[3:0]];
	usrRejectCmW = usrRejectCnMask[opRegM_Cr[3:0]];
	usrRejectCnR = usrRejectCmMask[opRegN_Cr[3:0]];
	usrRejectCnW = usrRejectCnMask[opRegN_Cr[3:0]];

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
	opUCty		= JX2_IUC_SC;

	tNextMsgLatch	= 0;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[15:0])
	
		default: begin
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

	endcase

	opUCmd = { opCcty, opNmid };
	
	case(opFmid)
		JX2_FMID_INV: begin
			$display("Jx2DecOp: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		end

		JX2_FMID_Z: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};
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
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Xr;
					opRegN	= opRegN_Xr;
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
					if(usrRejectCnW)
						usrReject = 1;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_ECr;
					opRegM	= JX2_GR_ZZR;
					usrReject = 1;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= JX2_GR_ZZR;
					usrReject = 1;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_ESr;
					opRegM	= JX2_GR_ZZR;
					usrReject = 1;
				end

				JX2_ITY_NB: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_Cr;
					if(usrRejectCnR)
						usrReject = 1;
				end
				JX2_ITY_NW: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_ECr;
					usrReject = 1;
				end
				JX2_ITY_NL: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_Sr;
					usrReject = 1;
				end
				JX2_ITY_NQ: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= opRegN_ESr;
					usrReject = 1;
				end

				JX2_ITY_XB: begin
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Xr;
					opRegN	= JX2_GR_DLR;
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
//						opRegO_Fix, opRegM_Fix };
						opRegO_Fix[5:0], opRegM_Fix[5:0] };
				end
				JX2_ITY_XQ: begin
					opRegN	= JX2_GR_DLR;
					opRegM	= opRegN_Xr;
					opRegO	= JX2_GR_IMM;
					opImm	= {
						opRegO_Fix[5]?UV21_FF:UV21_00,
						opRegO_Fix[5:0], opRegM_Fix[5:0] };
				end

				default: begin
					opRegN	= opRegN_Xr;
					opRegM	= JX2_GR_ZZR;
				end
			endcase
		end

		JX2_FMID_IMM8: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};
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
			opUIxt	= {opUCty, opUCmdIx[5:0]};

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
					if(usrRejectCnW)
						usrReject = 1;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Cr;
					opRegO	= JX2_GR_DLR;
					if(usrRejectCmR)
						usrReject = 1;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_DLR;
					usrReject = 1;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Sr;
					opRegO	= JX2_GR_DLR;
					usrReject = 1;
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
			opRegM	= opRegO_Dfl;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegO_Dfl;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
		end

		JX2_FMID_IMM8Z: begin
			opImm	= {UV25_00, istrWord[7:0]};
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opRegN	= JX2_GR_DLR;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
		end
		JX2_FMID_IMM8N: begin
			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
		end

		JX2_FMID_LDREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opIty)
				JX2_ITY_UL: begin
					opRegO	= JX2_GR_IMM;
					opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty[2], 2'b10};
					opImm	= {UV30_00, istrWord[10:8]};
				end
				default:
				begin
					opRegO	= JX2_GR_ZZR;
					opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
				end
			endcase
		end

		JX2_FMID_LDDRREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_GR_DLR;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty[2], 2'b00};
			end else begin
				opRegM	= opRegM_Dfl;
				opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			end
		end

		JX2_FMID_LDDRPCREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_DLR;
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
		end

		JX2_FMID_IMM4ZREG: begin
			opImm	= {UV29_00, istrWord[3:0]};

			opUIxt	= {opUCty, opUCmdIx[5:0]};
			opRegN	= opRegN_Yr;
			case(opIty)
				JX2_ITY_SB: begin
					opRegO	= opRegN_Yr;
					opRegM	= JX2_GR_IMM;
				end
				JX2_ITY_SW: begin
					opRegM	= opRegN_Yr;
					opRegO	= JX2_GR_IMM;
				end
				default: begin
					opRegO	= opRegN_Yr;
					opRegM	= JX2_GR_IMM;
				end
			endcase
		end

		JX2_FMID_IMM4NREG: begin
			opImm	= {UV29_FF, istrWord[3:0]};

			opUIxt	= {opUCty, opUCmdIx[5:0]};
			opRegN	= opRegN_Yr;
			case(opIty)
				JX2_ITY_SB: begin
					opRegO	= opRegN_Yr;
					opRegM	= JX2_GR_IMM;
				end
				JX2_ITY_SW: begin
					opRegM	= opRegN_Yr;
					opRegO	= JX2_GR_IMM;
				end
				default: begin
					opRegO	= opRegN_Yr;
					opRegM	= JX2_GR_IMM;
				end
			endcase
		end

		JX2_FMID_REGPC: begin
			opRegM	= JX2_GR_PC;
			opRegO	= opRegN_Xr;
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
		end

		JX2_FMID_DRREG: begin
			opUIxt	= {opUCty, opUCmdIx };
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
					if(usrRejectCnR)
						usrReject = 1;
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
			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV21_00, istrWord[11:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV21_FF, istrWord[11:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
		end

		JX2_FMID_LDDLRREG: begin
			opRegM	= JX2_GR_DLR;
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_GR_ZZR;
			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
		end

		JX2_FMID_LDDI4SPREG: begin
			opRegM	= JX2_GR_SP;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegN_Dfl;

			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			opImm	= {UV25_00, 4'b0, istrWord[3:0]};

			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
				end
				JX2_ITY_UB: begin
					opRegN	= opRegN_Er;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_ZXr;
				end

				default: begin
					opRegN	= opRegN_Dfl;
				end
			endcase
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

	usrSuAllowEn = 0;
	case(usrSuAllow)
		2'b00: usrSuAllowEn = 0;
		2'b01: usrSuAllowEn = srSuperuser;
		2'b10: usrSuAllowEn = srMod[1];
		2'b11: usrSuAllowEn = srMod[1] && srMod[2];
	endcase

	if(usrReject && srUser && !usrSuAllowEn)
	begin
		$display("DecOpBz: Usermode Reject %X", istrWord[15:0]);
		opNmid	= JX2_UCMD_INVOP;
		opFmid	= JX2_FMID_INV;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

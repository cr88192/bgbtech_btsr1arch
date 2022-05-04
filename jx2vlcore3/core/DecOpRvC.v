/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

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

reg[9:0]	tOffsJmp;
reg[9:0]	tOffsJmpZ;

always @*
begin

//	tRegRoIsRs	= (istrWord[12]==1'b0);
	tRegRmIsRs	= (istrWord[ 6]==1'b0);
	tRegRnIsRs	= (istrWord[11]==1'b0);

	opRegM_Dfl	= { 2'b0, istrWord[ 6:2]};
	opRegN_Dfl	= { 2'b0, istrWord[11:7]};

	case(istrWord[5:2])
		4'b0000: opRegM_Sr = JX2_GR_ZZR;
		4'b0001: opRegM_Sr = JX2_GR_LR;
		4'b0010: opRegM_Sr = JX2_GR_SP;
		4'b0011: opRegM_Sr = JX2_GR_GBR;
		4'b0100: opRegM_Sr = JX2_GR_TBR;
		4'b0101: opRegM_Sr = JX2_GR_DHR;
		4'b0110: opRegM_Sr = JX2_GR_DLR;
		4'b0111: opRegM_Sr = JX2_GR_R7;
		4'b1000: opRegM_Sr = JX2_GR_R8;
		4'b1001: opRegM_Sr = JX2_GR_R9;
		4'b1010: opRegM_Sr = JX2_GR_R10;
		4'b1011: opRegM_Sr = JX2_GR_R11;
		4'b1100: opRegM_Sr = JX2_GR_R12;
		4'b1101: opRegM_Sr = JX2_GR_R13;
		4'b1110: opRegM_Sr = JX2_GR_R2;
		4'b1111: opRegM_Sr = JX2_GR_R3;
	endcase

	case(istrWord[10:7])
		4'b0000: opRegN_Sr = JX2_GR_ZZR;
		4'b0001: opRegN_Sr = JX2_GR_LR;
		4'b0010: opRegN_Sr = JX2_GR_SP;
		4'b0011: opRegN_Sr = JX2_GR_GBR;
		4'b0100: opRegN_Sr = JX2_GR_TBR;
		4'b0101: opRegN_Sr = JX2_GR_DHR;
		4'b0110: opRegN_Sr = JX2_GR_DLR;
		4'b0111: opRegN_Sr = JX2_GR_R7;
		4'b1000: opRegN_Sr = JX2_GR_R8;
		4'b1001: opRegN_Sr = JX2_GR_R9;
		4'b1010: opRegN_Sr = JX2_GR_R10;
		4'b1011: opRegN_Sr = JX2_GR_R11;
		4'b1100: opRegN_Sr = JX2_GR_R12;
		4'b1101: opRegN_Sr = JX2_GR_R13;
		4'b1110: opRegN_Sr = JX2_GR_R2;
		4'b1111: opRegN_Sr = JX2_GR_R3;
	endcase

//	if(tRegRoIsRs)
//		opRegO_Dfl = opRegO_Sr;
	if(tRegRmIsRs)
		opRegM_Dfl = opRegM_Sr;
	if(tRegRnIsRs)
		opRegN_Dfl = opRegN_Sr;

	case(istrWord[4:2])
		4'b000: opRegM_Er = JX2_GR_R8;
		4'b001: opRegM_Er = JX2_GR_R9;
		4'b010: opRegM_Er = JX2_GR_R10;
		4'b011: opRegM_Er = JX2_GR_R11;
		4'b100: opRegM_Er = JX2_GR_R12;
		4'b101: opRegM_Er = JX2_GR_R13;
		4'b110: opRegM_Er = JX2_GR_R2;
		4'b111: opRegM_Er = JX2_GR_R3;
	endcase

	case(istrWord[9:7])
		3'b000: opRegN_Er = JX2_GR_R8;
		3'b001: opRegN_Er = JX2_GR_R9;
		3'b010: opRegN_Er = JX2_GR_R10;
		3'b011: opRegN_Er = JX2_GR_R11;
		3'b100: opRegN_Er = JX2_GR_R12;
		3'b101: opRegN_Er = JX2_GR_R13;
		3'b110: opRegN_Er = JX2_GR_R2;
		3'b111: opRegN_Er = JX2_GR_R3;
	endcase
	
	tOffsJmp = {
		istrWord[12],	istrWord[ 8], 
		istrWord[10],	istrWord[ 9], 
		istrWord[ 6],	istrWord[ 7],
		istrWord[ 2],	istrWord[11],
		istrWord[ 5],	istrWord[ 4],
		istrWord[ 3],	1'b0 };

	tOffsJmpZ = {
		istrWord[12],
		istrWord[ 6],	istrWord[ 5],
		istrWord[ 2],	istrWord[11],
		istrWord[10],	istrWord[ 4],
		istrWord[ 3],	1'b0 };

	usrReject	= 0;
	usrSuAllow	= 0;

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
			$display("DecOpRvC: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		end

		JX2_FMID_Z: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};
			opRegN	= opRegN_Fix;
			opRegM	= opRegM_Fix;
			opRegO	= opRegO_Fix;
		end
	
		/*
			SW: ZZR, Rn, Rn
			SQ: Rn, ZZR, Rn

			XW: ZZR, Rn, DLR
			SQ: Rn, FixImm, Rn
		 */
		JX2_FMID_REG: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			case(opIty)
				JX2_ITY_SB: begin
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Dfl;
				end

				JX2_ITY_SL: begin
				end

				JX2_ITY_SQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
				end

				JX2_ITY_XW: begin
					opRegN	= JX2_GR_DLR;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegN_Dfl;
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
				$display("DecOpRvC: Istr %X",
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
		$display("DecOpRvC: Usermode Reject %X", istrWord[15:0]);
		opNmid	= JX2_UCMD_INVOP;
		opFmid	= JX2_FMID_INV;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule

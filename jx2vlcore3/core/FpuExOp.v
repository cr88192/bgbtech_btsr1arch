/*
FPU Execute Unit

The FPU is essentially a slave to the main Execute Unit.
It will recieve copies of some inputs, but it is the main Execute unit which will initiate the action.

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [  4]=Use GPRs
  [3:0]=Depends on opcode


 */


`include "CoreDefs.v"

`include "FpuAdd.v"
`include "FpuMul.v"

`ifndef jx2_fcmp_alu
// `include "FpuCmp.v"
`endif

`include "FpuConvS2D.v"
`include "FpuConvH2D.v"
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
// `include "FpuConvD2I.v"

module FpuExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regIdRs,	regValRs,
	regIdRt,	regValRt,
	regIdRn,	regValRn,
	regOutId,	regOutVal,
	regOutOK,	regOutSrT,

	regInSr,	braFlush,
	exHold,

	regValGRm,	regValGRn,
	regValLdGRn,
	memDataLd,	memDataOK
	);

input			clock;
input			reset;

input[8:0]		opCmd;			//command opcode
input[8:0]		regIdIxt;		//ALU Index / Opcode Extension

`input_gpr		regIdRs;
`input_gpr		regIdRt;
`input_gpr		regIdRn;

input[63:0]		regValRs;		//Rs input value (FPR)
input[63:0]		regValRt;		//Rt input value (FPR)
input[63:0]		regValRn;		//Rn input value (FPR, Duplicate)

input[63:0]		regValGRm;		//Rm input value (GPR)
output[63:0]	regValGRn;		//Rn output value (GPR, via EX1)
output[63:0]	regValLdGRn;	//Rn output (Mem Load)
output			regOutSrT;

output[63:0]	regOutVal;		//Ro output value
`output_gpr		regOutId;		//Ro, register to write (FPR)
output[1:0]		regOutOK;		//execute status

input[63:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)

input[63:0]		regInSr;		//input SR
input			braFlush;
input			exHold;

reg[63:0]	tRegOutVal;			//Rn output value
reg[5:0]	tRegOutId;			//Rn, value to write
reg[1:0]	tRegOutOK;			//execute status
reg			tRegOutSrT;
reg[63:0]	tRegValGRn;			//GRn Out
reg[63:0]	tRegValLdGRn;		//GRn Out

reg[63:0]	tRegOutVal2;		//Rn output value
reg[5:0]	tRegOutId2;			//Rn, value to write
reg[1:0]	tRegOutOK2;			//execute status
reg			tRegOutSrT2;
reg[63:0]	tRegValGRn2;			//GRn Out
reg[63:0]	tRegValLdGRn2;		//GRn Out

assign		regOutVal		= tRegOutVal;
assign		regOutId		= tRegOutId;
assign		regOutOK		= tRegOutOK;
assign		regOutSrT		= tRegOutSrT;
assign		regValGRn		= tRegValGRn;
assign		regValLdGRn		= tRegValLdGRn;

`ifndef def_true
reg[8:0]		tOpCmd;			//command opcode
reg[8:0]		tRegIdIxt;		//ALU Index / Opcode Extension
reg[5:0]		tRegIdRs;
reg[5:0]		tRegIdRt;
reg[5:0]		tRegIdRn;
reg[63:0]		tRegValRs;		//Rs input value (FPR)
reg[63:0]		tRegValRt;		//Rt input value (FPR)
reg[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSr;
reg				tBraFlush;
`endif

`ifdef def_true
wire[8:0]		tOpCmd;			//command opcode
wire[8:0]		tRegIdIxt;		//ALU Index / Opcode Extension
`wire_gpr		tRegIdRs;
`wire_gpr		tRegIdRt;
`wire_gpr		tRegIdRn;
wire[63:0]		tRegValRs;		//Rs input value (FPR)
wire[63:0]		tRegValRt;		//Rt input value (FPR)
wire[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)
wire[63:0]		tRegInSr;
wire			tBraFlush;
`endif

reg[8:0]		tOpCmdL;			//command opcode
reg[8:0]		tRegIdIxtL;		//ALU Index / Opcode Extension
reg[5:0]		tRegIdRsL;
reg[5:0]		tRegIdRtL;
reg[5:0]		tRegIdRnL;
reg[63:0]		tRegValRsL;		//Rs input value (FPR)
reg[63:0]		tRegValRtL;		//Rt input value (FPR)
reg[63:0]		tRegValRnL;		//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSrL;
reg				tBraFlushL;
	

wire[31:0]		cnvRegValGRm;

assign			cnvRegValGRm = tRegIdIxt[3] ?
//	regValGRm[63:32] : regValGRm[31: 0];
	tRegValRs[63:32] : tRegValRs[31: 0];

// `ifdef jx2_enable_fmov
`ifndef def_true
wire[63:0]		memDataLd_S2D;		//memory data (Single To Double)
FpuConvS2D mem_cnv_s2d(memDataLd[31:0], memDataLd_S2D);
`endif

reg[63:0]		ctlInDlr_S2D_L;		//memory data (Single To Double)
reg[63:0]		ctlInDlr_H2D_L;		//memory data (Half To Double)
reg[31:0]		fstcx_D2S_L;
reg[15:0]		fstcx_D2H_L;
// reg[63:0]		fstcx_D2I_L;

wire[63:0]		ctlInDlr_S2D;		//memory data (Single To Double)
// FpuConvS2D dlr_cnv_s2d(regValGRm[31:0], ctlInDlr_S2D);
FpuConvS2D dlr_cnv_s2d(cnvRegValGRm, ctlInDlr_S2D);

wire[63:0]		ctlInDlr_H2D;		//memory data (Half To Double)
// FpuConvH2D dlr_cnv_h2d(regValGRm[15:0], ctlInDlr_H2D);
FpuConvH2D dlr_cnv_h2d(cnvRegValGRm[15:0], ctlInDlr_H2D);

wire[31:0]		fstcx_D2S;
wire[15:0]		fstcx_D2H;
FpuConvD2S mem_cnv_d2s(tRegValRs[63:0], fstcx_D2S);
FpuConvD2H mem_cnv_d2h(tRegValRs[63:0], fstcx_D2H);

// wire[63:0]		fstcx_D2I;
// FpuConvD2I mem_cnv_d2i(clock, reset, exHold, tRegValRs[63:0], fstcx_D2I);


wire[63:0]	tRegAddRn;		//Rn input value
`ifdef jx2_enable_fprs
assign tRegAddRn =
	(opCmd[5:0]==JX2_UCMD_FLDCX) ?
		regValGRm : tRegValRs;
`else
assign tRegAddRn = tRegValRs;
`endif

wire[7:0]		regRMode;

assign		regRMode = 0;

wire[63:0]	tRegAddVal;		//Rn output value
wire[3:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOK;	

FpuAdd	fpu_add(
	clock,		reset,
	exHold,
	regValRt,	tRegAddRn,
	tRegAddVal,	tRegAddExOp,
	tRegAddExOK,
	regRMode);

wire[1:0]	tRegMulExOK;	

wire[63:0]	tRegMulVal;		//Rn output value
FpuMul	fpu_mul(
	clock,		reset,		exHold,
	tRegValRt,	tRegValRs,	tRegMulVal,
	tRegAddExOp,
	tRegMulExOK, regRMode);

wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
wire	tFpuIsFstcx;
assign	tFpuIsFpu3 = (tOpCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmd[5:0]==JX2_UCMD_FLDCX);
assign	tFpuIsFstcx = (tOpCmd[5:0]==JX2_UCMD_FSTCX);

assign	tRegAddExOp	=
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 4'h1 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 4'h2 :
	(tFpuIsFldcx && (tRegIdIxt[3:0]==4'h2)) ? 4'h3 :
	(tFpuIsFstcx && (tRegIdIxt[3:0]==4'h2)) ? 4'h4 :
	4'h0;

// `ifndef jx2_fcmp_alu
`ifndef def_true
wire[1:0]	tCmpExOK;
wire		tCmpSrT;
reg			tCmpSrTL;

FpuCmp	fpu_cmp(
	clock,		reset,		exHold,
	tOpCmd,		tRegIdIxt,	
	tRegValRs,	tRegValRt,
	tCmpExOK,	tCmpSrT);
`endif

reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;

`ifndef def_true
assign	tOpCmd			= exHold ? tOpCmdL : opCmd;
assign	tRegIdIxt		= exHold ? tRegIdIxtL : regIdIxt;
assign	tRegIdRs		= exHold ? tRegIdRsL : regIdRs;
assign	tRegIdRt		= exHold ? tRegIdRtL : regIdRt;
assign	tRegIdRn		= exHold ? tRegIdRnL : regIdRn;
assign	tRegValRs		= exHold ? tRegValRsL : regValRs;
assign	tRegValRt		= exHold ? tRegValRtL : regValRt;
assign	tRegValRn		= exHold ? tRegValRnL : regValRn;
assign	tRegInSr		= exHold ? tRegInSrL : regInSr;
assign	tBraFlush		= exHold ? tBraFlushL : braFlush;
`endif

`ifdef def_true
assign	tOpCmd			= opCmd;
assign	tRegIdIxt		= regIdIxt;
assign	tRegIdRs		= regIdRs;
assign	tRegIdRt		= regIdRt;
assign	tRegIdRn		= regIdRn;
assign	tRegValRs		= regValRs;
assign	tRegValRt		= regValRt;
assign	tRegValRn		= regValRn;
assign	tRegInSr		= regInSr;
assign	tBraFlush		= braFlush;
`endif

always @*
begin
//	tRegOutVal		= UV64_XX;
	tRegOutVal		= UV64_00;
	tRegOutId		= JX2_GR_ZZR;
	tRegOutOK		= UMEM_OK_READY;
//	tRegValGRn		= UV64_XX;
	tRegValGRn		= UV64_00;
	tExHold			= 0;
	tExValidCmd		= 0;
	tDoHoldCyc		= 0;

// `ifndef jx2_fcmp_alu
`ifndef def_true
//	tRegOutSrT		= tRegInSr[0];
	tRegOutSrT	= tCmpSrT;
//	tRegOutSrT	= tCmpSrTL;
`else
	tRegOutSrT		= tRegInSr[0];
`endif

//`ifdef jx2_enable_fmov
`ifndef def_true
	tRegValLdGRn	= memDataLd_S2D;
`else
	tRegValLdGRn	= UV64_XX;
`endif

//	casez( { braFlush || reset, opCmd[7:6] } )
//	casez( { tBraFlush || reset, tOpCmd[7:6] } )
	casez( { tBraFlushL || reset, tOpCmdL[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = tRegInSr[0];
//		3'b010: 	tOpEnable = tRegInSrL[0];
		3'b011: 	tOpEnable = !tRegInSr[0];
//		3'b011: 	tOpEnable = !tRegInSrL[0];
		3'b1zz:		tOpEnable = 0;
	endcase

//	tOpUCmd1	= tOpEnable ? opCmd[5:0] : JX2_UCMD_NOP;
//	tOpUCmd1	= tOpEnable ? tOpCmd[5:0] : JX2_UCMD_NOP;
	tOpUCmd1	= tOpEnable ? tOpCmdL[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)

// `ifdef jx2_enable_fprs
// `ifdef def_true
// `ifdef jx2_enable_fmov
`ifndef def_true
		JX2_UCMD_FMOV_MR: begin
			if(regIdIxt[4])
				tRegOutVal	= memDataLd;
			else
				tRegOutVal	= memDataLd_S2D;
			tRegValGRn	= tRegOutVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FMOV_RM: begin
			tRegValGRn	= { UV32_00, fstcx_D2S };
		end
`endif

		JX2_UCMD_FPU3: begin
			tRegOutId	= regIdRn;
			tExValidCmd	= 1;

//			case(regIdIxt[3:0])
//			case(tRegIdIxt[3:0])
			case(tRegIdIxtL[3:0])
				4'h0: begin
//					if(tHoldCyc != 5)
//						tExHold = 1;
					tDoHoldCyc = 5;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h1: begin
//					if(tHoldCyc != 5)
//						tExHold = 1;
					tDoHoldCyc = 5;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h2: begin
//					if(tHoldCyc != 5)
//						tExHold = 1;
					tDoHoldCyc = 5;
					tRegOutVal	= tRegMulVal;
					tRegValGRn	= tRegMulVal;
				end

				4'h4: begin
					tRegOutVal	= tRegValRs;
					tRegValGRn	= tRegValRs;
				end
				
				default: begin
				end
			endcase
			
//			$display("FPU3: UIxt=%X Rs=%X, Rt=%X, Rn=%X",
//				regIdIxt, regValRs, regValRt, tRegValGRn);
		end

// `ifndef jx2_fcmp_alu
`ifndef def_true
		JX2_UCMD_FCMP: begin
			tExHold		= tCmpExOK[1];
//			if(tHoldCyc != 1)
//				tExHold = 1;
			tRegOutSrT	= tCmpSrT;
//			tRegOutSrT	= tCmpSrTL;
			tExValidCmd	= 1;
		end
`endif

		JX2_UCMD_FLDCX: begin
			tRegOutId	= tRegIdRn;
			tExValidCmd	= 1;

//			case(tRegIdIxt[2:0])
			case(tRegIdIxtL[2:0])
				3'h0: begin
					tRegOutVal	= ctlInDlr_S2D_L;
					tRegValGRn	= tRegOutVal;
				end
				3'h1: begin
//					tRegOutVal	= tRegValGRm;
					tRegOutVal	= tRegValRs;
					tRegValGRn	= tRegOutVal;
				end
				3'h2: begin
//					if(tHoldCyc != 5)
//						tExHold = 1;
					tDoHoldCyc	= 5;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegOutVal;
				end
				3'h3: begin
					tRegOutVal	= ctlInDlr_H2D_L;
					tRegValGRn	= tRegOutVal;
				end
				
				default: begin
					tRegOutVal	= UV64_XX;
				end
			endcase
		end

		JX2_UCMD_FSTCX: begin
			tExValidCmd	= 1;

//			case(regIdIxt[3:0])
			case(tRegIdIxtL[3:0])
				4'h0: begin
					tRegValGRn	= { UV32_00, fstcx_D2S_L };
				end

				4'h1: begin
					tRegValGRn	= tRegValRsL;
				end

				4'h2: begin
//					if(tHoldCyc != 3)
//						tExHold = 1;
//					tRegValGRn	= fstcx_D2I;

					tDoHoldCyc = 6;
					tRegValGRn	= tRegAddVal;
				end

				4'h3: begin
					tRegValGRn	= { UV48_00, fstcx_D2H_L };
				end

				4'h8: begin
					tRegValGRn	= { fstcx_D2S, tRegValRsL[31:0] };
				end
				
				default: begin
					tRegValGRn	= UV64_XX;
				end
			endcase
		end

`ifndef def_true
		JX2_UCMD_FIXS: begin
			tExValidCmd	= 1;

//			case(tRegIdIxt[3:0])
			case(tRegIdIxtL[3:0])
//				JX2_UCIX_FPIX_FNEG: begin
				4'h0: begin
					tRegOutVal	= { ~tRegValRsL[63], tRegValRsL[62:0] };
					tRegOutId	= tRegIdRn;
					tRegValGRn	= tRegOutVal;
				end

//				JX2_UCIX_FPIX_FABS: begin
				4'h1: begin
					tRegOutVal	= { 1'b0, tRegValRsL[62:0] };
					tRegOutId	= tRegIdRn;
					tRegValGRn	= tRegOutVal;
				end
				
				default: begin
				end
			endcase
		end
`endif

		default: begin
		end
	endcase

	if(tHoldCyc < tDoHoldCyc)
		tExHold = 1;

	if(reset)
		tExHold = 0;

//	if(tExHold)
//		$display("FPU Hold %d", tHoldCyc);

//	tRegOutOK	= tExHold ? UMEM_OK_HOLD : UMEM_OK_OK;
	tRegOutOK	= tExHold ? UMEM_OK_HOLD :
		(tExValidCmd ? UMEM_OK_OK : UMEM_OK_READY);

end

reg[1:0]	tLastRegMulExOK;

always @(posedge clock)
begin
	if(!exHold)
	begin
		tRegOutVal2		<= tRegOutVal;
		tRegOutId2		<= tRegOutId;
		tRegOutOK2		<= tRegOutOK;
		tRegOutSrT2		<= tRegOutSrT;
		tRegValGRn2		<= tRegValGRn;
		tRegValLdGRn2	<= tRegValLdGRn;

`ifndef def_true
		tOpCmd			<= opCmd;
		tRegIdIxt		<= regIdIxt;
		tRegIdRs		<= regIdRs;
		tRegIdRt		<= regIdRt;
		tRegIdRn		<= regIdRn;
		tRegValRs		<= regValRs;
		tRegValRt		<= regValRt;
		tRegValRn		<= regValRn;
		tRegInSr		<= regInSr;
		tBraFlush		<= braFlush;
`endif

`ifdef def_true
		tOpCmdL			<= tOpCmd;
		tRegIdIxtL		<= tRegIdIxt;
		tRegIdRsL		<= tRegIdRs;
		tRegIdRtL		<= tRegIdRt;
		tRegIdRnL		<= tRegIdRn;
		tRegValRsL		<= tRegValRs;
		tRegValRtL		<= tRegValRt;
		tRegValRnL		<= tRegValRn;
		tRegInSrL		<= tRegInSr;
		tBraFlushL		<= tBraFlush;
`endif

// `ifndef	jx2_fcmp_alu
`ifndef	def_true
		tCmpSrTL		<= tCmpSrT;
`endif

		ctlInDlr_S2D_L	<= ctlInDlr_S2D;
		ctlInDlr_H2D_L	<= ctlInDlr_H2D;
		fstcx_D2S_L		<= fstcx_D2S;
		fstcx_D2H_L		<= fstcx_D2H;
//		fstcx_D2I_L		<= fstcx_D2I;
	end

//	tLastRegMulExOK		<= tRegMulExOK;

//	if(exHold)
//		tHoldCyc <= tHoldCyc;
//	else
	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else if(exHold)
		tHoldCyc <= tHoldCyc;
	else
		tHoldCyc <= 0;
end

endmodule

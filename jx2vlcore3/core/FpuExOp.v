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
`include "FpuCmp.v"

`include "FpuConvS2D.v"
`include "FpuConvH2D.v"
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
`include "FpuConvD2I.v"

module FpuExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regIdRs,	regValRs,
	regIdRt,	regValRt,
	regIdRn,	regValRn,
	regOutId,	regOutVal,
	regOutOK,	regOutSrT,

	regInSr,

	regValGRm,	regValGRn,
	regValLdGRn,
	memDataLd,	memDataOK
	);

input			clock;
input			reset;

input[7:0]		opCmd;			//command opcode
input[7:0]		regIdIxt;		//ALU Index / Opcode Extension

input[5:0]		regIdRs;
input[5:0]		regIdRt;
input[5:0]		regIdRn;

input[63:0]		regValRs;		//Rs input value (FPR)
input[63:0]		regValRt;		//Rt input value (FPR)
input[63:0]		regValRn;		//Rn input value (FPR, Duplicate)

input[63:0]		regValGRm;		//Rm input value (GPR)
output[63:0]	regValGRn;		//Rn output value (GPR, via EX1)
output[63:0]	regValLdGRn;	//Rn output (Mem Load)

output[63:0]	regOutVal;		//Ro output value
output[5:0]		regOutId;		//Ro, register to write (FPR)
output[1:0]		regOutOK;		//execute status

input[63:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)

input[63:0]		regInSr;		//input SR
output			regOutSrT;

reg[63:0]	tRegOutVal;			//Rn output value
reg[5:0]	tRegOutId;			//Rn, value to write
reg[1:0]	tRegOutOK;			//execute status
reg			tRegOutSrT;

assign	regOutVal	= tRegOutVal;
assign	regOutId	= tRegOutId;
assign	regOutOK	= tRegOutOK;
assign	regOutSrT	= tRegOutSrT;

reg[63:0]	tRegValGRn;			//GRn Out
reg[63:0]	tRegValLdGRn;		//GRn Out
assign		regValGRn	= tRegValGRn;
assign		regValLdGRn	= tRegValLdGRn;

wire[31:0]		cnvRegValGRm;

assign			cnvRegValGRm = regIdIxt[3] ?
	regValGRm[63:32] :
	regValGRm[31: 0];

`ifdef jx2_enable_fmov
wire[63:0]		memDataLd_S2D;		//memory data (Single To Double)
FpuConvS2D mem_cnv_s2d(memDataLd[31:0], memDataLd_S2D);
`endif

wire[63:0]		ctlInDlr_S2D;		//memory data (Single To Double)
// FpuConvS2D dlr_cnv_s2d(regValGRm[31:0], ctlInDlr_S2D);
FpuConvS2D dlr_cnv_s2d(cnvRegValGRm, ctlInDlr_S2D);

wire[63:0]		ctlInDlr_H2D;		//memory data (Half To Double)
// FpuConvH2D dlr_cnv_h2d(regValGRm[15:0], ctlInDlr_H2D);
FpuConvH2D dlr_cnv_h2d(cnvRegValGRm[15:0], ctlInDlr_H2D);

wire[31:0]		fstcx_D2S;
wire[15:0]		fstcx_D2H;
FpuConvD2S mem_cnv_d2s(regValRs[63:0], fstcx_D2S);
FpuConvD2H mem_cnv_d2h(regValRs[63:0], fstcx_D2H);

wire[63:0]		fstcx_D2I;
FpuConvD2I mem_cnv_d2i(clock, reset, regValRs[63:0], fstcx_D2I);


wire[63:0]	tRegAddRn;		//Rn input value
`ifdef jx2_enable_fprs
assign tRegAddRn =
	(opCmd[5:0]==JX2_UCMD_FLDCX) ?
		regValGRm : regValRs;
`else
assign tRegAddRn = regValRs;
`endif

wire[63:0]	tRegAddVal;		//Rn output value
wire[1:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOK;	

FpuAdd	fpu_add(
	clock,		reset,
	regValRt,	tRegAddRn,
	tRegAddVal,	tRegAddExOp,
	tRegAddExOK);

wire[63:0]	tRegMulVal;		//Rn output value
FpuMul	fpu_mul(
	clock,		reset,
	regValRt,	regValRs,
	tRegMulVal);

wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
assign	tFpuIsFpu3 = (opCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (opCmd[5:0]==JX2_UCMD_FLDCX);

assign	tRegAddExOp	=
	(tFpuIsFpu3 && (regIdIxt[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 2'h1 :
	(tFpuIsFpu3 && (regIdIxt[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 2'h2 :
	(tFpuIsFldcx && (regIdIxt[3:0]==4'h2)) ? 2'h3 :
	2'h0;

wire[1:0]	tCmpExOK;
wire		tCmpSrT;

FpuCmp	fpu_cmp(
	clock,		reset,
	opCmd,		regIdIxt,	
	regValRs,	regValRt,
	tCmpExOK,	tCmpSrT);
	

reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[3:0]	tHoldCyc;

always @*
begin
	tRegOutVal		= UV64_XX;
	tRegOutId		= JX2_GR_ZZR;
	tRegOutOK		= UMEM_OK_READY;
	tRegValGRn		= UV64_XX;
	tExHold			= 0;
	tExValidCmd		= 0;
	tRegOutSrT		= regInSr[0];
`ifdef jx2_enable_fmov
	tRegValLdGRn	= memDataLd_S2D;
`else
	tRegValLdGRn	= UV64_XX;
`endif

	case(opCmd[7:6])
		2'b00: 	tOpEnable = 1;
		2'b01: 	tOpEnable = 0;
		2'b10: 	tOpEnable = regInSr[0];
		2'b11: 	tOpEnable = !regInSr[0];
	endcase

	tOpUCmd1	= tOpEnable ? opCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)

// `ifdef jx2_enable_fprs
// `ifdef def_true
`ifdef jx2_enable_fmov
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

			case(regIdIxt[3:0])
				4'h0: begin
					if(tHoldCyc != 5)
//					if(tHoldCyc != 7)
						tExHold = 1;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h1: begin
					if(tHoldCyc != 5)
//					if(tHoldCyc != 7)
						tExHold = 1;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h2: begin
					if(tHoldCyc != 5)
						tExHold = 1;
					tRegOutVal	= tRegMulVal;
					tRegValGRn	= tRegMulVal;
				end

				4'h4: begin
					tRegOutVal	= regValRs;
					tRegValGRn	= regValRs;
				end
				
				default: begin
				end
			endcase
			
//			$display("FPU3: UIxt=%X Rs=%X, Rt=%X, Rn=%X",
//				regIdIxt, regValRs, regValRt, tRegValGRn);
		end

		JX2_UCMD_FCMP: begin
			tExHold		= tCmpExOK[1];
			if(tHoldCyc != 1)
				tExHold = 1;
			tRegOutSrT	= tCmpSrT;
			tExValidCmd	= 1;
		end

		JX2_UCMD_FLDCX: begin
			tRegOutId	= regIdRn;
			tExValidCmd	= 1;

			case(regIdIxt[2:0])
				3'h0: begin
					tRegOutVal	= ctlInDlr_S2D;
					tRegValGRn	= tRegOutVal;
				end
				3'h1: begin
					tRegOutVal	= regValGRm;
					tRegValGRn	= tRegOutVal;
				end
				3'h2: begin
					if(tHoldCyc != 5)
						tExHold = 1;
					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegOutVal;
				end
				3'h3: begin
					tRegOutVal	= ctlInDlr_H2D;
					tRegValGRn	= tRegOutVal;
				end
				
				default: begin
					tRegOutVal	= UV64_XX;
				end
			endcase
		end

		JX2_UCMD_FSTCX: begin
			tExValidCmd	= 1;

			case(regIdIxt[3:0])
				4'h0: begin
					tRegValGRn	= { UV32_00, fstcx_D2S };
				end

				4'h1: begin
					tRegValGRn	= regValRs;
				end

				4'h2: begin
					if(tHoldCyc != 3)
						tExHold = 1;
					tRegValGRn	= fstcx_D2I;
				end

				4'h3: begin
					tRegValGRn	= { UV48_00, fstcx_D2H };
				end

				4'h8: begin
					tRegValGRn	= { fstcx_D2S, regValRs[31:0] };
				end
				
				default: begin
					tRegValGRn	= UV64_XX;
				end
			endcase
		end
		
		JX2_UCMD_FIXS: begin
			tExValidCmd	= 1;

			case(regIdIxt[3:0])
//				JX2_UCIX_FPIX_FNEG: begin
				4'h0: begin
					tRegOutVal	= { ~regValRs[63], regValRs[62:0] };
					tRegOutId	= regIdRn;
					tRegValGRn	= tRegOutVal;
				end

//				JX2_UCIX_FPIX_FABS: begin
				4'h1: begin
					tRegOutVal	= { 1'b0, regValRs[62:0] };
					tRegOutId	= regIdRn;
					tRegValGRn	= tRegOutVal;
				end
				
				default: begin
				end
			endcase
		end

		default: begin
		end
	endcase

//	if(tExHold)
//		$display("FPU Hold %d", tHoldCyc);

//	tRegOutOK	= tExHold ? UMEM_OK_HOLD : UMEM_OK_OK;
	tRegOutOK	= tExHold ? UMEM_OK_HOLD :
		(tExValidCmd ? UMEM_OK_OK : UMEM_OK_READY);

end

reg[1:0]	tLastRegMulExOK;

always @(posedge clock)
begin

//	tLastRegMulExOK		<= tRegMulExOK;

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else
		tHoldCyc <= 0;
end

endmodule

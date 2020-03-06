/*
FPU Execute Unit (Wide)

The FPU is essentially a slave to the main Execute Unit.
It will recieve copies of some inputs, but it is the main Execute unit which will initiate the action.

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [  4]=Use GPRs
  [3:0]=Depends on opcode


The Wide variant will span two lanes and implement FP-SIMD.

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

module FpuExOpW(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmdA,		regIdIxtA,
	opCmdB,		regIdIxtB,

	regIdRsA,	regValRsA,
	regIdRtA,	regValRtA,
	regIdRnA,	regValRnA,

	regIdRsB,	regValRsB,
	regIdRtB,	regValRtB,
	regIdRnB,	regValRnB,

	regOutOK,	regOutSrT,

	regInSr,	braFlush,
	exHold,

	regValGRnA,	regValGRnB

//	regValLdGRn,
//	memDataLd,	memDataOK
	);

input			clock;
input			reset;

input[7:0]		opCmdA;			//command opcode
input[7:0]		opCmdB;			//command opcode
input[7:0]		regIdIxtA;		//ALU Index / Opcode Extension
input[7:0]		regIdIxtB;		//ALU Index / Opcode Extension

input[5:0]		regIdRsA;
input[5:0]		regIdRtA;
input[5:0]		regIdRnA;
input[5:0]		regIdRsB;
input[5:0]		regIdRtB;
input[5:0]		regIdRnB;

input[63:0]		regValRsA;		//Rs input value (Lane 1)
input[63:0]		regValRtA;		//Rt input value (Lane 1)
input[63:0]		regValRnA;		//Rn input value (Lane 1)
input[63:0]		regValRsB;		//Rs input value (Lane 2)
input[63:0]		regValRtB;		//Rt input value (Lane 2)
input[63:0]		regValRnB;		//Rn input value (Lane 2)

// input[63:0]		regValGRm;		//Rm input value (GPR)
output[63:0]	regValGRnA;		//Rn output value (Lane 1)
output[63:0]	regValGRnB;		//Rn output value (Lane 2)
// output[63:0]	regValLdGRn;	//Rn output (Mem Load)
output			regOutSrT;

// output[63:0]	regOutVal;		//Ro output value
// output[5:0]		regOutId;		//Ro, register to write (FPR)
output[1:0]		regOutOK;		//execute status

// input[63:0]		memDataLd;		//memory data (load)
// input[1:0]		memDataOK;		//memory status (load)

input[63:0]		regInSr;		//input SR
input			braFlush;
input			exHold;

// reg[63:0]	tRegOutVal;			//Rn output value
// reg[5:0]	tRegOutId;			//Rn, value to write
reg[1:0]	tRegOutOK;			//execute status
reg			tRegOutSrT;
reg[63:0]	tRegValGRn;			//GRn Out
reg[63:0]	tRegValGRnA;		//GRn Out
reg[63:0]	tRegValGRnB;		//GRn Out
// reg[63:0]	tRegValLdGRn;		//GRn Out

// reg[63:0]	tRegOutVal2;		//Rn output value
// reg[5:0]	tRegOutId2;			//Rn, value to write
reg[1:0]	tRegOutOK2;			//execute status
reg			tRegOutSrT2;
reg[63:0]	tRegValGRn2;			//GRn Out
reg[63:0]	tRegValLdGRn2;		//GRn Out

// assign		regOutVal		= tRegOutVal;
// assign		regOutId		= tRegOutId;
assign		regOutOK		= tRegOutOK;
assign		regOutSrT		= tRegOutSrT;
assign		regValGRnA		= tRegValGRnA;
assign		regValGRnB		= tRegValGRnB;
// assign		regValLdGRn		= tRegValLdGRn;

`ifndef def_true
reg[7:0]		tOpCmd;			//command opcode
reg[7:0]		tRegIdIxt;		//ALU Index / Opcode Extension
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
wire[7:0]		tOpCmd;			//command opcode
wire[7:0]		tRegIdIxt;		//ALU Index / Opcode Extension
wire[5:0]		tRegIdRs;
wire[5:0]		tRegIdRt;
wire[5:0]		tRegIdRn;
wire[63:0]		tRegValRs;		//Rs input value (FPR)
wire[63:0]		tRegValRt;		//Rt input value (FPR)
wire[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)

wire[7:0]		tOpCmdA;			//command opcode
wire[7:0]		tRegIdIxtA;		//ALU Index / Opcode Extension
wire[5:0]		tRegIdRsA;
wire[5:0]		tRegIdRtA;
wire[5:0]		tRegIdRnA;
wire[63:0]		tRegValRsA;		//Rs input value (FPR)
wire[63:0]		tRegValRtA;		//Rt input value (FPR)
wire[63:0]		tRegValRnA;		//Rn input value (FPR, Duplicate)

wire[7:0]		tOpCmdB;			//command opcode
wire[7:0]		tRegIdIxtB;		//ALU Index / Opcode Extension
wire[5:0]		tRegIdRsB;
wire[5:0]		tRegIdRtB;
wire[5:0]		tRegIdRnB;
wire[63:0]		tRegValRsB;		//Rs input value (FPR)
wire[63:0]		tRegValRtB;		//Rt input value (FPR)
wire[63:0]		tRegValRnB;		//Rn input value (FPR, Duplicate)

wire[63:0]		tRegInSr;
wire			tBraFlush;
`endif

reg[7:0]		tOpCmdL;			//command opcode
reg[7:0]		tRegIdIxtL;			//ALU Index / Opcode Extension
reg				tExCmdLaneBL;

reg[5:0]		tRegIdRsL;
reg[5:0]		tRegIdRtL;
reg[5:0]		tRegIdRnL;
reg[63:0]		tRegValRsL;			//Rs input value (FPR)
reg[63:0]		tRegValRtL;			//Rt input value (FPR)
reg[63:0]		tRegValRnL;			//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSrL;
reg				tBraFlushL;

reg[5:0]		tRegIdRsAL;
reg[5:0]		tRegIdRtAL;
reg[5:0]		tRegIdRnAL;
reg[63:0]		tRegValRsAL;		//Rs input value (FPR)
reg[63:0]		tRegValRtAL;		//Rt input value (FPR)
reg[63:0]		tRegValRnAL;		//Rn input value (FPR, Duplicate)


reg[5:0]		tRegIdRsBL;
reg[5:0]		tRegIdRtBL;
reg[5:0]		tRegIdRnBL;
reg[63:0]		tRegValRsBL;		//Rs input value (FPR)
reg[63:0]		tRegValRtBL;		//Rt input value (FPR)
reg[63:0]		tRegValRnBL;		//Rn input value (FPR, Duplicate)
	

wire[31:0]		cnvRegValGRm;

assign			cnvRegValGRm = tRegIdIxt[3] ?
//	regValGRm[63:32] : regValGRm[31: 0];
	tRegValRs[63:32] : tRegValRs[31: 0];

`ifdef jx2_enable_fmov
// wire[63:0]		memDataLd_S2D;		//memory data (Single To Double)
// FpuConvS2D mem_cnv_s2d(memDataLd[31:0], memDataLd_S2D);
`endif

reg[63:0]		ctlInDlr_S2D_L;		//memory data (Single To Double)
reg[63:0]		ctlInDlr_H2D_L;		//memory data (Half To Double)
reg[31:0]		fstcx_D2S_L;
reg[15:0]		fstcx_D2H_L;
reg[63:0]		fstcx_D2I_L;

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

wire[63:0]		fstcx_D2I;
FpuConvD2I mem_cnv_d2i(clock, reset, exHold, tRegValRs[63:0], fstcx_D2I);

reg[31:0]	tVecCnvRsI;
reg[31:0]	tVecCnvRtI;
wire[63:0]	tVecCnvRsO;
wire[63:0]	tVecCnvRtO;
FpuConvS2D vec_cnv_rs(tVecCnvRsI, tVecCnvRsO);
FpuConvS2D vec_cnv_rt(tVecCnvRtI, tVecCnvRtO);

reg[63:0]	tVecCnvRnI;
wire[31:0]	tVecCnvRnO;
FpuConvD2S vec_cnv_rn(tVecCnvRnI, tVecCnvRnO);

reg[63:0]	tVecRnA;
reg[63:0]	tVecRnB;
reg[63:0]	tNxtVecRnA;
reg[63:0]	tNxtVecRnB;


//wire[63:0]	tRegAddRn;		//Rn input value
//`ifdef jx2_enable_fprs
//assign tRegAddRn =
//	(opCmd[5:0]==JX2_UCMD_FLDCX) ?
//		regValGRm : tRegValRs;
//`else
//assign tRegAddRn = tRegValRs;
// `endif

reg[63:0]	tRegAddRs;		//Rn input value
reg[63:0]	tRegAddRt;		//Rn input value

wire[63:0]	tRegAddVal;		//Rn output value
wire[1:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOK;	

FpuAdd	fpu_add(
	clock,		reset,
	exHold,
//	tRegValRt,	tRegAddRn,
	tRegAddRt,	tRegAddRs,
	tRegAddVal,	tRegAddExOp,
	tRegAddExOK);

wire[63:0]	tRegMulVal;		//Rn output value
FpuMul	fpu_mul(
	clock,		reset,		exHold,
//	tRegValRt,	tRegValRs,	tRegMulVal);
	tRegAddRt,	tRegAddRs,	tRegMulVal);

wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
assign	tFpuIsFpu3 = (tOpCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmd[5:0]==JX2_UCMD_FLDCX);

assign	tRegAddExOp	=
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 2'h1 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 2'h2 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_PADD[3:0])) ? 2'h1 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_PSUB[3:0])) ? 2'h2 :
	(tFpuIsFldcx && (tRegIdIxt[3:0]==4'h2)) ? 2'h3 :
	2'h0;

wire[1:0]	tCmpExOK;
wire		tCmpSrT;
reg			tCmpSrTL;

FpuCmp	fpu_cmp(
	clock,		reset,		exHold,
	tOpCmd,		tRegIdIxt,	
	tRegValRs,	tRegValRt,
	tCmpExOK,	tCmpSrT);


reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[3:0]	tHoldCyc;

`ifdef def_true
assign	tOpCmdA			= opCmdA;
assign	tRegIdIxtA		= regIdIxtA;
assign	tOpCmdB			= opCmdB;
assign	tRegIdIxtB		= regIdIxtB;

assign	tRegIdRsA		= regIdRsA;
assign	tRegIdRtA		= regIdRtA;
assign	tRegIdRnA		= regIdRnA;
assign	tRegValRsA		= regValRsA;
assign	tRegValRtA		= regValRtA;
assign	tRegValRnA		= regValRnA;

assign	tRegIdRsB		= regIdRsB;
assign	tRegIdRtB		= regIdRtB;
assign	tRegIdRnB		= regIdRnB;
assign	tRegValRsB		= regValRsB;
assign	tRegValRtB		= regValRtB;
assign	tRegValRnB		= regValRnB;

assign	tRegInSr		= regInSr;
assign	tBraFlush		= braFlush;
`endif


wire	tExCmdLaneB;
reg		tExCmdVecW;

assign	tExCmdLaneB =
	((tOpCmdB[5:0]==JX2_UCMD_FPU3) && !regIdIxtB[5])	||
	(tOpCmdB[5:0]==JX2_UCMD_FCMP)	||
	(tOpCmdB[5:0]==JX2_UCMD_FLDCX)	||
	(tOpCmdB[5:0]==JX2_UCMD_FSTCX)	;
	
assign	tOpCmd			= tExCmdLaneB ? opCmdB : opCmdA;
assign	tRegIdIxt		= tExCmdLaneB ? regIdIxtB : regIdIxtA;

assign	tRegIdRs		= tExCmdLaneB ? regIdRsB : regIdRsA;
assign	tRegIdRt		= tExCmdLaneB ? regIdRtB : regIdRtA;
assign	tRegIdRn		= tExCmdLaneB ? regIdRnB : regIdRnA;
assign	tRegValRs		= tExCmdLaneB ? regValRsB : regValRsA;
assign	tRegValRt		= tExCmdLaneB ? regValRtB : regValRtA;
assign	tRegValRn		= tExCmdLaneB ? regValRnB : regValRnA;


always @*
begin
//	tRegOutVal		= UV64_XX;
//	tRegOutId		= JX2_GR_ZZR;
	tRegOutOK		= UMEM_OK_READY;
	tRegValGRn		= UV64_XX;
	tRegValGRnA		= UV64_XX;
	tRegValGRnB		= UV64_XX;
	tExHold			= 0;
	tExValidCmd		= 0;
	tExCmdVecW		= 0;
	tRegOutSrT		= tRegInSr[0];

	tNxtVecRnA		= tVecRnA;
	tNxtVecRnB		= tVecRnB;

	casez( { tBraFlushL || reset, tOpCmdL[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = tRegInSr[0];
		3'b011: 	tOpEnable = !tRegInSr[0];
		3'b1zz:		tOpEnable = 0;
	endcase

	tOpUCmd1	= tOpEnable ? tOpCmdL[5:0] : JX2_UCMD_NOP;

	tRegAddRs	= tRegValRs;
	tRegAddRt	= tRegValRt;

	tVecCnvRsI	= tRegValRsA[31:0];
	tVecCnvRtI	= tRegValRtA[31:0];
	tVecCnvRnI	= tRegAddVal;

	case(tHoldCyc)
		0: begin
//				tVecCnvRsI = tRegValRsA[31:0];
//				tVecCnvRtI = tRegValRtA[31:0];
				tVecCnvRsI = tRegValRs[31:0];
				tVecCnvRtI = tRegValRt[31:0];
		end
		1: begin
//				tVecCnvRsI = tRegValRsA[63:32];
//				tVecCnvRtI = tRegValRtA[63:32];
				tVecCnvRsI = tRegValRs[63:32];
				tVecCnvRtI = tRegValRt[63:32];
		end
		2: begin
				tVecCnvRsI = tRegValRsB[31:0];
				tVecCnvRtI = tRegValRtB[31:0];
		end
		3: begin
				tVecCnvRsI = tRegValRsB[63:32];
				tVecCnvRtI = tRegValRtB[63:32];
		end
		
		4: begin
		end

		5: begin
			tNxtVecRnA[31:0] = tVecCnvRnO;
		end
		6: begin
			tNxtVecRnA[63:32] = tVecCnvRnO;
		end
		7: begin
			tNxtVecRnB[31:0] = tVecCnvRnO;
		end
		8: begin
			tNxtVecRnB[63:32] = tVecCnvRnO;
		end
		
		default: begin
		end
	endcase

	case(tOpUCmd1)
		JX2_UCMD_FPU3: begin
//			tRegOutId	= regIdRn;
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
				4'h0: begin
					if(tHoldCyc != 5)
//					if(tHoldCyc != 7)
						tExHold = 1;
//					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h1: begin
					if(tHoldCyc != 5)
//					if(tHoldCyc != 7)
						tExHold = 1;
//					tRegOutVal	= tRegAddVal;
					tRegValGRn	= tRegAddVal;
				end

				4'h2: begin
					if(tHoldCyc != 5)
//					if(tHoldCyc != 6)
						tExHold = 1;
//					tRegOutVal	= tRegMulVal;
					tRegValGRn	= tRegMulVal;
				end

				4'h4: begin
//					tRegOutVal	= tRegValRs;
					tRegValGRn	= tRegValRs;
				end

				4'h5: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;

					if(tHoldCyc != 9)
						tExHold = 1;

//					tExCmdVecW	= 1;
					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;
				end
				4'h6: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;

					if(tHoldCyc != 9)
						tExHold = 1;

//					tExCmdVecW = 1;
					tExCmdVecW = tRegIdIxtL[5];
					tRegValGRnB = tVecRnB;
					tRegValGRnA = tVecRnA;
					tRegValGRn	= tVecRnA;
				end
				4'h7: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegMulVal;

					if(tHoldCyc != 9)
						tExHold = 1;

//					tExCmdVecW	= 1;
					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;
				end
				
				default: begin
				end
			endcase
			
//			$display("FPU3: UIxt=%X Rs=%X, Rt=%X, Rn=%X",
//				regIdIxt, regValRs, regValRt, tRegValGRn);
		end

		JX2_UCMD_FCMP: begin
			tExHold		= tCmpExOK[1];
//			if(tHoldCyc != 1)
//				tExHold = 1;
			tRegOutSrT	= tCmpSrT;
//			tRegOutSrT	= tCmpSrTL;
			tExValidCmd	= 1;
		end

		JX2_UCMD_FLDCX: begin
//			tRegOutId	= tRegIdRn;
			tExValidCmd	= 1;

//			case(tRegIdIxt[2:0])
			case(tRegIdIxtL[2:0])
				3'h0: begin
//					tRegOutVal	= ctlInDlr_S2D_L;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= ctlInDlr_S2D_L;
				end
				3'h1: begin
//					tRegOutVal	= tRegValGRm;
//					tRegOutVal	= tRegValRs;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= tRegValRs;
				end
				3'h2: begin
					if(tHoldCyc != 5)
						tExHold = 1;
//					tRegOutVal	= tRegAddVal;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= tRegAddVal;
				end
				3'h3: begin
//					tRegOutVal	= ctlInDlr_H2D_L;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= ctlInDlr_H2D_L;
				end
				
				default: begin
//					tRegOutVal	= UV64_XX;
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
					if(tHoldCyc != 3)
						tExHold = 1;
					tRegValGRn	= fstcx_D2I;
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
// `ifdef def_true
		JX2_UCMD_FIXS: begin
			tExValidCmd	= 1;

//			case(tRegIdIxt[3:0])
			case(tRegIdIxtL[3:0])
//				JX2_UCIX_FPIX_FNEG: begin
				4'h0: begin
//					tRegOutVal	= { ~tRegValRsL[63], tRegValRsL[62:0] };
//					tRegOutId	= tRegIdRn;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= { ~tRegValRsL[63], tRegValRsL[62:0] };
				end

//				JX2_UCIX_FPIX_FABS: begin
				4'h1: begin
//					tRegOutVal	= { 1'b0, tRegValRsL[62:0] };
//					tRegOutId	= tRegIdRn;
//					tRegValGRn	= tRegOutVal;
					tRegValGRn	= { 1'b0, tRegValRsL[62:0] };
				end
				
				default: begin
				end
			endcase
		end
`endif

		default: begin
		end
	endcase

	if(!tExCmdVecW)
	begin
		tRegValGRnB = tRegValGRn;
		tRegValGRnA = tRegValGRn;
	end

	if(reset)
		tExHold = 0;

//	if(tExHold)
//		$display("FPU Hold %d", tHoldCyc);

	tRegOutOK	= tExHold ? UMEM_OK_HOLD :
		(tExValidCmd ? UMEM_OK_OK : UMEM_OK_READY);

end

reg[1:0]	tLastRegMulExOK;

always @(posedge clock)
begin
	if(!exHold)
	begin
//		tRegOutVal2		<= tRegOutVal;
//		tRegOutId2		<= tRegOutId;
		tRegOutOK2		<= tRegOutOK;
		tRegOutSrT2		<= tRegOutSrT;
		tRegValGRn2		<= tRegValGRn;
//		tRegValLdGRn2	<= tRegValLdGRn;

`ifdef def_true
		tOpCmdL			<= tOpCmd;
		tRegIdIxtL		<= tRegIdIxt;
		tExCmdLaneBL	<= tExCmdLaneB;

		tRegIdRsL		<= tRegIdRs;
		tRegIdRtL		<= tRegIdRt;
		tRegIdRnL		<= tRegIdRn;
		tRegValRsL		<= tRegValRs;
		tRegValRtL		<= tRegValRt;
		tRegValRnL		<= tRegValRn;

		tRegIdRsAL		<= tRegIdRsA;
		tRegIdRtAL		<= tRegIdRtA;
		tRegIdRnAL		<= tRegIdRnA;
		tRegValRsAL		<= tRegValRsA;
		tRegValRtAL		<= tRegValRtA;
		tRegValRnAL		<= tRegValRnA;

		tRegIdRsBL		<= tRegIdRsB;
		tRegIdRtBL		<= tRegIdRtB;
		tRegIdRnBL		<= tRegIdRnB;
		tRegValRsBL		<= tRegValRsB;
		tRegValRtBL		<= tRegValRtB;
		tRegValRnBL		<= tRegValRnB;

		tRegInSrL		<= tRegInSr;
		tBraFlushL		<= tBraFlush;
`endif

		tCmpSrTL		<= tCmpSrT;

		ctlInDlr_S2D_L	<= ctlInDlr_S2D;
		ctlInDlr_H2D_L	<= ctlInDlr_H2D;
		fstcx_D2S_L		<= fstcx_D2S;
		fstcx_D2H_L		<= fstcx_D2H;
		fstcx_D2I_L		<= fstcx_D2I;
	end

	tVecRnA		<= tNxtVecRnA;
	tVecRnB		<= tNxtVecRnB;

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else if(exHold)
		tHoldCyc <= tHoldCyc;
	else
		tHoldCyc <= 0;
end

endmodule

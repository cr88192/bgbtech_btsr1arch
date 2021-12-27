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

`include "FpuConvS2D.v"
`include "FpuConvH2D.v"
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
`include "FpuConvD2I.v"

module McFpuExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regValRs,	regValRt,	regValRn,
	regOutVal,	regOutOK,

	regInSr,	braFlush,
	exHold,

	regValGRm,	regValGRn
	);

input			clock;
input			reset;

input[8:0]		opCmd;			//command opcode
input[8:0]		regIdIxt;		//ALU Index / Opcode Extension

input[63:0]		regValRs;		//Rs input value (FPR)
input[63:0]		regValRt;		//Rt input value (FPR)
input[63:0]		regValRn;		//Rn input value (FPR, Duplicate)

input[63:0]		regValGRm;		//Rm input value (GPR)
output[63:0]	regValGRn;		//Rn output value (GPR, via EX1)

output[63:0]	regOutVal;		//Ro output value
output[1:0]		regOutOK;		//execute status

input[63:0]		regInSr;		//input SR
input			braFlush;
input			exHold;

reg[63:0]	tRegOutVal;			//Rn output value
reg[1:0]	tRegOutOK;			//execute status
reg[63:0]	tRegValGRn;			//GRn Out

reg[63:0]	tRegOutVal2;		//Rn output value
reg[1:0]	tRegOutOK2;			//execute status
reg[63:0]	tRegValGRn2;		//GRn Out

assign		regOutVal		= tRegOutVal;
assign		regOutOK		= tRegOutOK;
assign		regValGRn		= tRegValGRn;

`ifdef def_true
wire[8:0]		tOpCmd;			//command opcode
wire[8:0]		tRegIdIxt;		//ALU Index / Opcode Extension
wire[63:0]		tRegValRs;		//Rs input value (FPR)
wire[63:0]		tRegValRt;		//Rt input value (FPR)
wire[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)
wire[63:0]		tRegInSr;
wire			tBraFlush;
`endif

reg[8:0]		tOpCmdL;			//command opcode
reg[8:0]		tRegIdIxtL;		//ALU Index / Opcode Extension
reg[63:0]		tRegValRsL;		//Rs input value (FPR)
reg[63:0]		tRegValRtL;		//Rt input value (FPR)
reg[63:0]		tRegValRnL;		//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSrL;
reg				tBraFlushL;
	

wire[31:0]		cnvRegValGRm;

assign			cnvRegValGRm = tRegIdIxt[3] ?
//	regValGRm[63:32] : regValGRm[31: 0];
	tRegValRs[63:32] : tRegValRs[31: 0];

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


wire[63:0]	tRegAddRn;		//Rn input value
`ifdef jx2_enable_fprs
assign tRegAddRn =
	(opCmd[5:0]==JX2_UCMD_FLDCX) ?
		regValGRm : tRegValRs;
`else
assign tRegAddRn = tRegValRs;
`endif

wire[63:0]	tRegAddVal;		//Rn output value
wire[3:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOK;	
wire[1:0]	tRegMulExOK;

FpuAdd	fpu_add(
	clock,		reset,
	exHold,
	regValRt,	tRegAddRn,
	tRegAddVal,	tRegAddExOp,
	tRegAddExOK, 0);

wire[63:0]	tRegMulVal;		//Rn output value
FpuMul	fpu_mul(
	clock,			reset,			exHold,
	tRegValRt,		tRegValRs,		tRegMulVal,
	tRegAddExOp,	tRegMulExOK,	0);

wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
assign	tFpuIsFpu3 = (tOpCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmd[5:0]==JX2_UCMD_FLDCX);

assign	tRegAddExOp	=
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 4'h1 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 4'h2 :
	(tFpuIsFldcx && (tRegIdIxt[3:0]==4'h2)) ? 4'h3 :
	4'h0;

reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[3:0]	tHoldCyc;

`ifdef def_true
assign	tOpCmd			= opCmd;
assign	tRegIdIxt		= regIdIxt;
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
	tRegOutOK		= UMEM_OK_READY;
//	tRegValGRn		= UV64_XX;
	tRegValGRn		= UV64_00;
	tExHold			= 0;
	tExValidCmd		= 0;

	casez( { tBraFlushL || reset, tOpCmdL[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = tRegInSr[0];
		3'b011: 	tOpEnable = !tRegInSr[0];
		3'b1zz:		tOpEnable = 0;
	endcase

	tOpUCmd1	= tOpEnable ? tOpCmdL[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)

		JX2_UCMD_FPU3: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
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
//					if(tHoldCyc != 6)
						tExHold = 1;
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
		end

		JX2_UCMD_FLDCX: begin
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
					if(tHoldCyc != 5)
						tExHold = 1;
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

		default: begin
		end
	endcase

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
		tRegOutOK2		<= tRegOutOK;
		tRegValGRn2		<= tRegValGRn;

`ifdef def_true
		tOpCmdL			<= tOpCmd;
		tRegIdIxtL		<= tRegIdIxt;
		tRegValRsL		<= tRegValRs;
		tRegValRtL		<= tRegValRt;
		tRegValRnL		<= tRegValRn;
		tRegInSrL		<= tRegInSr;
		tBraFlushL		<= tBraFlush;
`endif

		ctlInDlr_S2D_L	<= ctlInDlr_S2D;
		ctlInDlr_H2D_L	<= ctlInDlr_H2D;
		fstcx_D2S_L		<= fstcx_D2S;
		fstcx_D2H_L		<= fstcx_D2H;
		fstcx_D2I_L		<= fstcx_D2I;
	end

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else if(exHold)
		tHoldCyc <= tHoldCyc;
	else
		tHoldCyc <= 0;
end

endmodule

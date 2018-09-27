/*
FPU Execute Unit

The FPU is essentially a slave to the main Execute Unit.
It will recieve copies of some inputs, but it is the main Execute unit which will initiate the action.

 */


`include "Jx2CoreDefs.v"

`include "Jx2FpuAdd.v"
`include "Jx2FpuMul.v"
`include "Jx2FpuRcpA.v"
`include "Jx2FpuSqrtA.v"

module Jx2FpuExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regIdRs,	regValRs,
	regIdRt,	regValRt,
	regIdRn,	regValRn,
	regOutId,	regOutVal,
	regOutOK,

	memDataLd,	memDataOK,

	ctlInDlr,	ctlOutDlr
	);

input			clock;
input			reset;

input[7:0]		opCmd;			//command opcode
input[7:0]		regIdIxt;		//ALU Index / Opcode Extension

input[6:0]		regIdRs;
input[6:0]		regIdRt;
input[6:0]		regIdRn;

input[63:0]		regValRs;		//Rs input value (FPR)
input[63:0]		regValRt;		//Rt input value (FPR)
input[63:0]		regValRn;		//Rn input value (FPR, Duplicate)

output[63:0]	regOutVal;		//Ro output value
output[6:0]		regOutId;		//Ro, register to write (FPR)
output[1:0]		regOutOK;		//execute status

input[63:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)

input[63:0]		ctlInDlr;		//DLR in
output[63:0]	ctlOutDlr;		//DLR out


reg[63:0]	tRegOutVal;		//Rn output value
reg[6:0]	tRegOutId;		//Rn, value to write
reg[1:0]	tRegOutOK;		//execute status

assign	regOutVal	= tRegOutVal;
assign	regOutId	= tRegOutId;
assign	regOutOK	= tRegOutOK;

reg[63:0]	tCtlOutDlr;		//DLR out
assign		ctlOutDlr	= tCtlOutDlr;



wire[63:0]		memDataLd_S2D;		//memory data (Single To Double)
Jx2ConvFpuS2D mem_cnv_s2d(memDataLd[31:0], memDataLd_S2D);

wire[63:0]		ctlInDlr_S2D;		//memory data (Single To Double)
Jx2ConvFpuS2D dlr_cnv_s2d(ctlInDlr[31:0], ctlInDlr_S2D);

wire[63:0]		ctlInDlr_H2D;		//memory data (Half To Double)
Jx2ConvFpuH2D dlr_cnv_h2d(ctlInDlr[15:0], ctlInDlr_H2D);


wire[63:0]	tRegAddRn;		//Rn input value
assign tRegAddRn =
	(opCmd==JX2_UCMD_FPU_FLDCI) ?
		ctlInDlr : regValRs;

wire[63:0]	tRegAddVal;		//Rn output value
// reg[1:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOp;	
wire[1:0]	tRegAddExOK;	

Jx2FpuAdd	fpu_add(
	clock,		reset,
	regValRt,	tRegAddRn,
	tRegAddVal,	tRegAddExOp,
	tRegAddExOK);


wire[63:0]	tRegMulRm;		//Rm output value
wire[63:0]	tRegMulRn;		//Rn output value

reg[63:0] tMulDivRcp;
reg[63:0] tMulDivAdj;
reg	tMulRcpStable;
reg	tMulRcpSeed;
reg	tMulRcpSeed0;

assign tRegMulRm =
	(opCmd==JX2_UCMD_FPU_FMUL) ?
	regValRt	:
	(opCmd==JX2_UCMD_FPU_FDIV) ?
		(tMulRcpStable ? tMulDivRcp : regValRt) :
	regValRt;

assign tRegMulRn =
	(opCmd==JX2_UCMD_FPU_FMUL) ?
	regValRs	:
	(opCmd==JX2_UCMD_FPU_FDIV) ?
		(tMulRcpStable ? regValRs : tMulDivRcp) :
	regValRs;

wire[63:0]	tRegMulVal;		//Rn output value
// reg[1:0]	tRegMulExOp;	
wire[1:0]	tRegMulExOp;	
wire[1:0]	tRegMulExOK;	

Jx2FpuMul	fpu_mul(
	clock,		reset,
	tRegMulRm,	tRegMulRn,
	tRegMulVal,	tRegMulExOp,
	tRegMulExOK);


wire[63:0]		regRcpValRn;
assign	regRcpValRn =
	(opCmd==JX2_UCMD_FPU_FDIV) ? regValRt : regValRs;

wire[63:0]		regValRnRcp;
Jx2FpuRcpA	nrcp(
	regRcpValRn, regValRnRcp);


wire[63:0]		regValRnSqrt;
Jx2FpuSqrtA	nsqrt(
	regValRs, regValRnSqrt);

assign	tRegAddExOp	=
	(opCmd==JX2_UCMD_FPU_FADD) ? 2'h1 :
	(opCmd==JX2_UCMD_FPU_FSUB) ? 2'h2 :
	(opCmd==JX2_UCMD_FPU_FLDCI) ? 2'h3 :
	2'h0;

reg		tRegDivStrobe;
assign	tRegMulExOp	=
	(opCmd==JX2_UCMD_FPU_FMUL) ? 2'h1 :
//	(opCmd==JX2_UCMD_FPU_FDIV) ? 2'h2 :
	(opCmd==JX2_UCMD_FPU_FDIV) ?
		(tMulRcpStable ? 2'h1 : (tRegDivStrobe?2'h0:2'h3)) :
	2'h0;

//	tRegMulExOp	= 0;
	

always @*
begin

	tCtlOutDlr	= ctlInDlr;

	tRegOutVal	= UV64_XX;
	tRegOutId	= JX2_REG_ZZR;
	tRegOutOK	= UMEM_OK_READY;
//	tRegAddExOp	= 0;
//	tRegMulExOp	= 0;

//	tMulDivAdj = { 1'b0, 63'h3FF00000_00000000 - tRegMulVal[62:0] };
	tMulDivAdj = 64'h3FF00000_00000000 - tRegMulVal;

	case(opCmd)
		JX2_UCMD_FMOVS_MR: begin
			tRegOutVal	= memDataLd_S2D;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FMOVD_MR: begin
			tRegOutVal	= memDataLd;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FADD3: begin
			tRegOutOK	= tRegAddExOK;
			tRegOutVal	= tRegAddVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FSUB3: begin
			tRegOutOK	= tRegAddExOK;
			tRegOutVal	= tRegAddVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FMUL3: begin
			tRegOutOK	= tRegMulExOK;
			tRegOutVal	= tRegMulVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FDIV3: begin
			if((tRegMulExOK == UMEM_OK_OK) && tMulRcpStable)
				tRegOutOK	= UMEM_OK_OK;
			else
				tRegOutOK	= UMEM_OK_HOLD;

//			tRegOutOK	= tRegMulExOK;
			tRegOutVal	= tRegMulVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FCMPEQ: begin
			/* Ignore, handled by EX */
		end
		JX2_UCMD_FPU_FCMPGT: begin
			/* Ignore, handled by EX */
		end


		JX2_UCMD_FPU_FMOV: begin
			tRegOutOK	= UMEM_OK_OK;
			tRegOutVal	= regValRs;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FLDCF: begin
			tRegOutOK	= UMEM_OK_OK;
			tRegOutVal	= ctlInDlr_S2D;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FLDCD: begin
			tRegOutOK	= UMEM_OK_OK;
			tRegOutVal	= ctlInDlr;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FLDCI: begin
			tRegOutOK	= UMEM_OK_OK;
			tRegOutOK	= tRegAddExOK;
			tRegOutVal	= tRegAddVal;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FLDCH: begin
			tRegOutOK	= UMEM_OK_OK;
			tRegOutVal	= ctlInDlr_H2D;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_FPU_FSTCF: begin
			/* Ignore, Handled by EX */
		end
		JX2_UCMD_FPU_FSTCD: begin
			/* Ignore, Handled by EX */
		end
		JX2_UCMD_FPU_FSTCI: begin
			/* Ignore, Handled by EX */
		end
		JX2_UCMD_FPU_FSTCH: begin
			/* Ignore, Handled by EX */
		end
		
		JX2_UCMD_FPU_FIXS: begin
			case(regIdIxt)
				JX2_UCMD_FPIX_FNEG: begin
					tRegOutOK	= UMEM_OK_OK;
					tRegOutVal	= { ~regValRs[63], regValRs[62:0] };
					tRegOutId	= regIdRn;
				end

				JX2_UCMD_FPIX_FABS: begin
					tRegOutOK	= UMEM_OK_OK;
					tRegOutVal	= { 1'b0, regValRs[62:0] };
					tRegOutId	= regIdRn;
				end

				JX2_UCMD_FPIX_FRCPA: begin
					tRegOutOK	= UMEM_OK_OK;
					tRegOutVal	= regValRnRcp;
					tRegOutId	= regIdRn;
				end
				
				JX2_UCMD_FPIX_FSQRTA: begin
					tRegOutOK	= UMEM_OK_OK;
					tRegOutVal	= regValRnSqrt;
					tRegOutId	= regIdRn;
				end
				
				default: begin
				end
			endcase
		end

		default: begin
		end
	endcase

end

always @(posedge clock)
begin

	if(opCmd==JX2_UCMD_FPU_FDIV)
	begin
		if(tMulRcpStable)
		begin
			tMulDivRcp		<= tMulDivRcp;
			$display("FPU FDIV: Stable=%X", tMulDivRcp);
		end
		else
		if(tMulRcpSeed)
		begin
			if(tRegMulExOK == UMEM_OK_OK)
			begin
//				tMulDivRcp		<= tMulDivRcp + (tMulDivAdj>>2);
//				tMulDivRcp		<= tMulDivRcp +
//					{ tMulDivAdj[63], tMulDivAdj[63:1] };
				tMulDivRcp		<= tMulDivRcp + tMulDivAdj;
				if(tMulDivAdj[52:2]==0)
					tMulRcpStable	<= 1;
				if(tMulDivAdj[52:2]==-1)
					tMulRcpStable	<= 1;
				$display("FPU FDIV: Iter=%X Delta=%X",
					tMulDivRcp, tMulDivAdj);
				tRegDivStrobe	<= 1;
			end
			else
			begin
				tRegDivStrobe	<= 0;
			end

		end
		else
		begin
			tMulDivRcp		<= regValRnRcp;
//			tMulRcpSeed		<= 1;
			tMulRcpSeed0	<= 1;
			tMulRcpSeed		<= tMulRcpSeed0;
			tRegDivStrobe	<= 1;
			
			$display("FPU FDIV: Seed=%X", regValRnRcp);
		end
	end
	else
	begin
		tMulRcpStable	<= 0;
		tMulRcpSeed		<= 0;
		tMulRcpSeed0	<= 0;
	end

end

endmodule

/*
FPU2 Execute Unit (Wide)

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

`include "Fp2Mac.v"

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

output[63:0]	regValGRnA;		//Rn output value (Lane 1)
output[63:0]	regValGRnB;		//Rn output value (Lane 2)
output			regOutSrT;

output[1:0]		regOutOK;		//execute status

input[63:0]		regInSr;		//input SR
input			braFlush;
input			exHold;

reg[1:0]	tRegOutOK;			//execute status
reg			tRegOutSrT;
reg[63:0]	tRegValGRn;			//GRn Out
reg[63:0]	tRegValGRnA;		//GRn Out
reg[63:0]	tRegValGRnB;		//GRn Out

reg[1:0]	tRegOutOK2;			//execute status
reg			tRegOutSrT2;
reg[63:0]	tRegValGRn2;			//GRn Out
reg[63:0]	tRegValLdGRn2;		//GRn Out

assign		regOutOK		= tRegOutOK;
assign		regOutSrT		= tRegOutSrT;
assign		regValGRnA		= tRegValGRnA;
assign		regValGRnB		= tRegValGRnB;

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
	

reg[63:0]	tVecRnA;
reg[63:0]	tVecRnB;
reg[63:0]	tVechRn;
reg[63:0]	tNxtVecRnA;
reg[63:0]	tNxtVecRnB;
reg[63:0]	tNxtVechRn;

reg[63:0]	tVecdRnA;
reg[63:0]	tVecdRnB;
reg[63:0]	tNxtVecdRnA;
reg[63:0]	tNxtVecdRnB;


reg[63:0]	tRegAddRs;		//Rn input value
reg[63:0]	tRegAddRt;		//Rn input value

wire		tExCmdIsSimd;
wire			tAddExHold;
assign		tAddExHold = exHold && !tExCmdIsSimd;

wire[63:0]	tRegAddVal;		//Rn output value
wire[1:0]	tRegAddExOp;	
wire[1:0]	tRegAddSimdExOp;	
wire[1:0]	tRegAddExOK;	


wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
assign	tFpuIsFpu3 = (tOpCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmd[5:0]==JX2_UCMD_FLDCX);

reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;

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


wire	tExCmdLaneA;
wire	tExCmdLaneB1;
wire	tExCmdLaneB;
reg		tExCmdVecW;

assign	tExCmdLaneA =
	(tOpCmdA[5:0]==JX2_UCMD_FPU3) 	||
	(tOpCmdA[5:0]==JX2_UCMD_FCMP)	||
	(tOpCmdA[5:0]==JX2_UCMD_FLDCX)	||
	(tOpCmdA[5:0]==JX2_UCMD_FSTCX)	;

assign	tExCmdLaneB1 =
	(tOpCmdB[5:0]==JX2_UCMD_FPU3)	||
	(tOpCmdB[5:0]==JX2_UCMD_FCMP)	||
	(tOpCmdB[5:0]==JX2_UCMD_FLDCX)	||
	(tOpCmdB[5:0]==JX2_UCMD_FSTCX)	;

assign	tExCmdLaneB = tExCmdLaneB1 && !tExCmdLaneA;
	
assign	tOpCmd			= tExCmdLaneB ? opCmdB : opCmdA;
assign	tRegIdIxt		= tExCmdLaneB ? regIdIxtB : regIdIxtA;

assign	tRegIdRs		= tExCmdLaneB ? regIdRsB : regIdRsA;
assign	tRegIdRt		= tExCmdLaneB ? regIdRtB : regIdRtA;
assign	tRegIdRn		= tExCmdLaneB ? regIdRnB : regIdRnA;
assign	tRegValRs		= tExCmdLaneB ? regValRsB : regValRsA;
assign	tRegValRt		= tExCmdLaneB ? regValRtB : regValRtA;
assign	tRegValRn		= tExCmdLaneB ? regValRnB : regValRnA;

assign	tExCmdIsSimd =
	(tOpCmdL[5:0]==JX2_UCMD_FPU3) 	&&
	(	(tRegIdIxtL[3:0]==4'h5) ||
		(tRegIdIxtL[3:0]==4'h6) ||
		(tRegIdIxtL[3:0]==4'h7)	);

wire[127:0]		macValRs;
wire[127:0]		macValRt;
wire[127:0]		macValRu;
assign	macValRs = { regValRsB, regValRs };
assign	macValRt = { regValRtB, regValRt };
assign	macValRu = { regValRnB, regValRn };

reg [127:0]		macValRm;
wire[127:0]		macValRn;
reg[  5:0]		macValOp;
reg[  5:0]		macValPos;
reg[  5:0]		macValPosDst

Fp2Mac		fp2mac(
	clock,		reset,		1'b0,
	macValRs,	macValRt,	macValRn,
	macValRm,	macValRn,	macValOp,
	macValPos,	macValPosDst
	);


always @*
begin
	tRegOutOK		= UMEM_OK_READY;
	tRegValGRn		= UV64_00;
	tRegValGRnA		= UV64_00;
	tRegValGRnB		= UV64_00;
	tExHold			= 0;
	tExValidCmd		= 0;
	tExCmdVecW		= 0;
	tRegOutSrT		= tRegInSr[0];

	tNxtVecRnA		= tVecRnA;
	tNxtVecRnB		= tVecRnB;
	tNxtVechRn		= tVechRn;
	tNxtVecdRnA		= tVecdRnA;
	tNxtVecdRnB		= tVecdRnB;
	tDoHoldCyc		= 0;

	casez( { tBraFlushL || reset, tOpCmdL[7:6], regInSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase

	tOpUCmd1	= tOpEnable ? tOpCmdL[5:0] : JX2_UCMD_NOP;

	macValOp		= tRegIdIxtL[5:0];
	macValPos		= 6'h08;
	macValPosDst	= 6'h08;

	case(tHoldCyc)
		0: begin
			macValPos   [1:0]	= 2'h0;
			macValPosDst[1:0]	= 2'h0;
		end
		1: begin
			macValPos   [1:0]	= 2'h1;
			macValPosDst[1:0]	= 2'h1;
		end
		2: begin
			macValPos   [1:0]	= 2'h2;
			macValPosDst[1:0]	= 2'h2;
		end
		3: begin
			macValPos   [1:0]	= 2'h3;
			macValPosDst[1:0]	= 2'h3;
		end
	endcase

	case(tOpUCmd1)
		JX2_UCMD_FPU3: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
				4'h0: begin
					tDoHoldCyc = 5;
					tRegValGRn	= tRegAddVal;
				end

				4'h1: begin
					tDoHoldCyc = 5;
					tRegValGRn	= tRegAddVal;
				end

				4'h2: begin
					tDoHoldCyc = 5;
					tRegValGRn	= tRegMulVal;
				end

				4'h4: begin
					tRegValGRn	= tRegValRs;
				end

				4'h5: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;
				
					tDoHoldCyc = 10;

					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;
					
					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
							tRegAddRs	= tVecdCnvRs;
							tRegAddRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
							tRegAddRs	= tVechCnvRsO;
							tRegAddRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end
				4'h6: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;

					tDoHoldCyc = 10;

					tExCmdVecW = tRegIdIxtL[5];
					tRegValGRnB = tVecRnB;
					tRegValGRnA = tVecRnA;
					tRegValGRn	= tVecRnA;

					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
							tRegAddRs	= tVecdCnvRs;
							tRegAddRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
							tRegAddRs	= tVechCnvRsO;
							tRegAddRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end
				4'h7: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegMulValL;

					tDoHoldCyc = 10;

					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;

					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
							tRegAddRs	= tVecdCnvRs;
							tRegAddRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
							tRegAddRs	= tVechCnvRsO;
							tRegAddRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end
				
				default: begin
				end
			endcase
		end

		JX2_UCMD_FLDCX: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[2:0])
				3'h0: begin
					tRegValGRn	= ctlInDlr_S2D_L;
				end
				3'h1: begin
					tRegValGRn	= tRegValRs;
				end
				3'h2: begin
					tDoHoldCyc = 5;
					tRegValGRn	= tRegAddVal;
				end
				3'h3: begin
					tRegValGRn	= ctlInDlr_H2D_L;
				end
				
				default: begin
				end
			endcase
		end

		JX2_UCMD_FSTCX: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
				4'h0: begin
					tRegValGRn	= { UV32_00, fstcx_D2S_L };
				end

				4'h1: begin
					tRegValGRn	= tRegValRsL;
				end

				4'h2: begin
					tDoHoldCyc = 3;
					tRegValGRn	= fstcx_D2I;
				end

				4'h3: begin
					tRegValGRn	= { UV48_00, fstcx_D2H_L };
				end

				4'h8: begin
					tRegValGRn	= { fstcx_D2S, tRegValRsL[31:0] };
				end
				
				default: begin
					tRegValGRn	= UV64_00;
				end
			endcase
		end

		default: begin
		end
	endcase

	if(!tExCmdVecW)
	begin
		tRegValGRnB = tRegValGRn;
		tRegValGRnA = tRegValGRn;
	end

	if(tHoldCyc < tDoHoldCyc)
		tExHold = 1;

	if(reset)
		tExHold = 0;

	tRegOutOK	= tExHold ? UMEM_OK_HOLD :
		(tExValidCmd ? UMEM_OK_OK : UMEM_OK_READY);

end

reg[1:0]	tLastRegMulExOK;

always @(posedge clock)
begin
	if(!exHold)
	begin
		tRegOutOK2		<= tRegOutOK;
		tRegOutSrT2		<= tRegOutSrT;
		tRegValGRn2		<= tRegValGRn;

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

		ctlInDlr_S2D_L	<= ctlInDlr_S2D;
		ctlInDlr_H2D_L	<= ctlInDlr_H2D;
		fstcx_D2S_L		<= fstcx_D2S;
		fstcx_D2H_L		<= fstcx_D2H;
		fstcx_D2I_L		<= fstcx_D2I;
	end

//	tVecRnA		<= tNxtVecRnA;
//	tVecRnB		<= tNxtVecRnB;
//	tVechRn		<= tNxtVechRn;
//	tVecdRnA	<= tNxtVecdRnA;
//	tVecdRnB	<= tNxtVecdRnB;
//	tRegMulValL	<= tRegMulVal;

	macValRm	<= macValRn;

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else if(exHold)
		tHoldCyc <= tHoldCyc;
	else
		tHoldCyc <= 0;
end

endmodule

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
EX3 Stage (Top Level, Lane 1)

Third Execute Stage.
Continues on the work from the first 2 stages.

 */

`include "CoreDefs.v"

`ifdef jx2_enable_fmov
`include "FpuConvS2D.v"
`include "FpuConvH2D.v"
`endif

`ifdef jx2_enable_pmov
`include "ExConv_Fp16Exp32.v"
`include "ExConv_Fp8Exp12.v"
`endif

`ifdef jx2_agu_ldtex
`ifdef jx2_enable_btcutx
`include "ExBtcUtx1.v"
`endif
`endif

module ExEX3(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,		exIsHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdCn1,		//Destination ID (EX1)
	regValCn1,		//Destination Value (EX1)

	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	regIdCn2,		//Destination ID (EX2)
	regValCn2,		//Destination Value (EX2)
	
	regIdRn4,		//Destination ID (EX2)
	regValRn4,		//Destination Value (EX2)

	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regValAluRes,	//ALU Result
	regValMulRes,	//ALU Result
	regValMulwRes,	//ALU Result
	regValKrreRes,	//Keyring Result

	regFpuGRn,		//FPU GPR Result
	regFpuV4GRn,	//FPU GPR Result
	opBraFlush,

	regInLastSr,
	ex3MulFaz,
	
	memDataIn,
	memDataInB,
	memDataOK
	);

input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;
input			exIsHold;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
`input_gprval		regValRs;		//Source A Value
`input_gprval		regValRt;		//Source B Value
`input_gprval		regValRm;		//Source C Value

`input_gpr		regIdRn1;		//Destination ID (EX1)
`input_gprval		regValRn1;		//Destination Value (EX1)
//input[4:0]		regIdCn1;		//Destination ID (CR, EX1)
`input_gpr		regIdCn1;		//Destination ID (CR, EX1)
`input_gprval		regValCn1;		//Destination Value (CR, EX1)

`output_gpr		regIdRn2;		//Destination ID (EX1)
`output_gprval	regValRn2;		//Destination Value (EX1)
//output[4:0]		regIdCn2;		//Destination ID (CR, EX1)
`output_gpr		regIdCn2;		//Destination ID (CR, EX1)
`output_gprval	regValCn2;		//Destination Value (CR, EX1)

`output_gpr		regIdRn4;		//Destination ID (EX1)
`output_gprval	regValRn4;		//Destination Value (EX1)

input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

// input[65:0]		regValAluRes;	//ALU Result
input[69:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulwRes;	//MUL.W Result
input[65:0]		regValKrreRes;	//Keyring Result

input[63:0]		regValMulRes;	//MUL.W Result
input[63:0]		regFpuGRn;		//FPU GPR Result
input[63:0]		regFpuV4GRn;		//FPU GPR Result

input			opBraFlush;

input[ 7:0]		regInLastSr;
input			ex3MulFaz;

input[65:0]		memDataIn;
input[65:0]		memDataInB;
input[1:0]		memDataOK;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

`reg_gpr		tRegIdRn2;
`reg_gprval		tRegValRn2;
//reg[ 4:0]		tRegIdCn2;
`reg_gpr		tRegIdCn2;
`reg_gprval		tRegValCn2;

`reg_gpr		tRegIdRn4;
`reg_gprval		tRegValRn4;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;
assign	regIdCn2	= tRegIdCn2;
assign	regValCn2	= tRegValCn2;

assign	regIdRn4	= tRegIdRn4;
assign	regValRn4	= tRegValRn4;


`reg_gprval	tValOutDfl;
`reg_gprval	tValOutHeld;
reg			tDoOutDfl;
reg			tDoOutHeld;


(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

reg[3:0]	tHoldCyc;
reg[3:0]	tDoHoldCyc;
reg			tDoMemOp;
reg			tOpEnable;

reg		tMsgLatch;
reg		tNextMsgLatch;

`ifdef jx2_enable_fmov
wire[63:0]	memDataIn_S2D;		//memory data (Single To Double)
wire[63:0]	memDataIn_H2D;		//memory data (Single To Double)
FpuConvS2D	mem_cnv_s2d(memDataIn[31:0], memDataIn_S2D);
FpuConvH2D	mem_cnv_h2d(memDataIn[15:0], memDataIn_H2D);
`endif

`ifdef jx2_enable_pmov
wire[63:0]	memDataIn_2xHtoS;
wire[63:0]	memDataIn_4xM8toH;

ExConv_Fp16Exp32 mem_2xh2s_a(memDataIn[15: 0], 0, memDataIn_2xHtoS[31: 0]);
ExConv_Fp16Exp32 mem_2xh2s_b(memDataIn[31:16], 0, memDataIn_2xHtoS[63:32]);

wire[1:0]	mem_4xm2h_mode;
// assign		mem_4xm2h_mode = { 1'b0, opUIxt[4] };
assign		mem_4xm2h_mode = 2'b01;

ExConv_Fp8Exp12 mem_4xm2h_a(
	memDataIn[ 7: 0], memDataIn_4xM8toH[15: 4], mem_4xm2h_mode);
ExConv_Fp8Exp12 mem_4xm2h_b(
	memDataIn[15: 8], memDataIn_4xM8toH[31:20], mem_4xm2h_mode);
ExConv_Fp8Exp12 mem_4xm2h_c(
	memDataIn[23:16], memDataIn_4xM8toH[47:36], mem_4xm2h_mode);
ExConv_Fp8Exp12 mem_4xm2h_d(
	memDataIn[31:24], memDataIn_4xM8toH[63:52], mem_4xm2h_mode);

assign	memDataIn_4xM8toH[ 3: 0]=0;
assign	memDataIn_4xM8toH[19:16]=0;
assign	memDataIn_4xM8toH[35:32]=0;
assign	memDataIn_4xM8toH[51:48]=0;

`endif


`ifdef jx2_agu_ldtex
`ifdef jx2_enable_btcutx

wire[63:0]	tValUtx1;
// wire[8:0]	tUtxIdUIxt;
reg[8:0]	tUtxIdUIxt;
wire[3:0]	tUtxIx;

// assign	tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKUTX2 };
assign	tUtxIx = { regValRt[49], regValRt[17], regValRt[48], regValRt[16] };

always @*
begin
	case(regValRs[59:57])
		3'b000: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKUTX2 };
//		3'b001: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKUTX1 };
		3'b001: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_RGB30APCK64F };
		3'b010: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKUTX3L };
		3'b011: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKUTX3H };
		3'b100: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKRGB15F };
		3'b101: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKRGB15A };
		3'b110: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKRGBA32 };
//		3'b111: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_BLKRGB30A };
		3'b111: tUtxIdUIxt = { 3'b000, JX2_UCIX_CONV2_RGB32UPCK64FU };
	endcase
end

ExBtcUtx1	exUtx1(
	memDataIn [63:0],
	memDataInB[63:0],
	tUtxIx,
	tUtxIdUIxt, tValUtx1, 1'b1);

`endif
`endif


always @*
begin
	tRegIdRn2	= regIdRn1;		//Forward by default
	tRegValRn2	= regValRn1;	//Forward by default
	tRegIdCn2	= regIdCn1;		//Forward by default
	tRegValCn2	= regValCn1;	//Forward by default
	tRegIdRn4	= regIdRn1;		//Forward by default
	tRegValRn4	= regValRn1;	//Forward by default

	tDoMemOp		= 0;
	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tDoHoldCyc		= 0;

	tValOutDfl		= UVGPRV_00;
	tValOutHeld		= UVGPRV_00;
	tDoOutDfl		= 0;
	tDoOutHeld		= 0;

`ifndef def_true
// `ifdef def_true
	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
		tRegIdCn2	= JX2_CR_ZZR;
	end
`endif

`ifndef def_true
	casez( { opBraFlush, opUCmd[7:6], regInLastSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif

	tOpEnable	= !opBraFlush;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_OP_IXS: begin
		end
		
		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])

// `ifndef def_true
`ifdef jx2_enable_ldekey
				JX2_UCIX_IXT_LDEKRR: begin
					if(regValKrreRes[65:64]==2'b10)
					begin
//						tRegIdCn2	= JX2_CR_KRR[4:0];
						tRegIdCn2	= JX2_CR_KRR;
						tRegValCn2	= regValKrreRes[63:0];
					end
				end
				
				JX2_UCIX_IXT_LDEENC: begin
					tRegIdRn2	= JX2_GR_DLR;
					tRegValRn2	= regValKrreRes[63:0];
				end
				
				JX2_UCIX_IXT_SVEKRR: begin
					tRegIdRn2	= JX2_GR_DLR;
					tRegValRn2	= regValKrreRes[63:0];
				end
`endif

				default: begin
				end
			endcase
		end
		
		JX2_UCMD_MOV_IR: begin
		end
	
		JX2_UCMD_LEA_MR: begin
		end

		JX2_UCMD_MOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_MOV_MR: begin
			tDoMemOp	= 1;
//			tRegIdRn2	= regIdRm;
//			tRegValRn2	= memDataIn;
			tValOutDfl[63:0]	= memDataIn[63:0];
			tDoOutDfl			= 1;
//			tDoOutHeld			= 1;

//			tRegHeld		= 1;

`ifndef def_true
			if(memDataIn[31:0]==32'h55BAADAA)
			begin
				$display("EX3: Bad Marker Seen R=%X V=%X", regIdRm, memDataIn);
			end
`endif

`ifdef jx2_debug_ldst
			$display("LOAD(3): R=%X V=%X", regIdRm, memDataIn);
`endif
		end

`ifdef jx2_enable_fmov
		JX2_UCMD_FMOV_RM: begin
			tDoMemOp	= 1;
		end
		JX2_UCMD_FMOV_MR: begin
			tDoMemOp			= 1;
//			tValOutDfl			= memDataIn_S2D;
			tValOutHeld[63:0]	= memDataIn_S2D;

			tDoOutHeld		= 1;
			tRegHeld		= 1;

`ifdef jx2_enable_fmovh
//			if(opUIxt[4])
			if(opUIxt[5:4]==2'b01)
//				tValOutDfl[63:0]	= memDataIn_H2D;
				tValOutHeld[63:0]	= memDataIn_H2D;
`endif

`ifdef jx2_agu_ldtex
			if(opUIxt[5:4]==2'b11)
			begin
//				tValOutDfl[63:0]	= tValUtx1;
				tValOutHeld[63:0]	= tValUtx1;
			end
`endif

`ifdef jx2_enable_ldst48a
			if(opUIxt[2])
			begin
				if(opUIxt[5])
				begin
//					tValOutDfl[63:0]	= { memDataIn[47:0], 16'h0000 };
					tValOutHeld[63:0]	= { memDataIn[47:0], 16'h0000 };
				end
				else
				begin
//					tValOutDfl[63:0]	= {
					tValOutHeld[63:0]	= {
						(memDataIn[47] && !opUIxt[4]) ? 16'hFFFF : 16'h0000,
						memDataIn[47:0] };
				end
			end
`endif

//			if(regIdRm[6])
//				tValOutDfl	= UVGPRV_XX;

//			tDoOutDfl		= 1;
`ifdef jx2_debug_ldst
			$display("LOAD(3): R=%X V=%X", regIdRm, memDataIn);
`endif

`ifndef def_true
			if(regIdRm[6])
//				tDoOutDfl	= 0;
				tDoOutHeld	= 0;

//			if(regIdRm[6:5]==2'b11)
//				tDoOutDfl	= 0;
`endif
		end
`endif

`ifdef jx2_enable_pmov
		JX2_UCMD_PMOV_MR: begin
			tDoMemOp		= 1;
//			tValOutDfl		= memDataIn_2xHtoS;
			tValOutHeld		= memDataIn_2xHtoS;
			if(opUIxt[5])
//				tValOutDfl		= memDataIn_4xM8toH;
				tValOutHeld		= memDataIn_4xM8toH;
//			tDoOutDfl		= 1;
			tDoOutHeld		= 1;
		end
`endif

//		JX2_UCMD_ADDSP: begin
//		end

		JX2_UCMD_BRA: begin
		end
		JX2_UCMD_BSR: begin
		end
		JX2_UCMD_JMP: begin
		end
		JX2_UCMD_JSR: begin
		end

		JX2_UCMD_BRA_NB: begin
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY,
		JX2_UCMD_ALUW3, JX2_UCMD_ALUB3: begin
		end

		JX2_UCMD_ALUCMP: begin
		end
		JX2_UCMD_ALUCMPW: begin
		end
		JX2_UCMD_ALUCMPB: begin
		end
		JX2_UCMD_ALUCMP3R: begin
		end
		
		JX2_UCMD_MUL3: begin
//			tRegIdRn2			= regIdRm;					//
//			tRegValRn2			= regValMulRes[63:0];		//
//			tValOutDfl			= regValMulRes[63:0];
			tValOutHeld[63:0]	= regValMulRes[63:0];
//			tDoOutDfl			= 1;
			tDoOutHeld			= 1;
		end

`ifdef jx2_alu_slomuldiv
		JX2_UCMD_QMULDIV: begin
			if(ex3MulFaz)
			begin
				/* Multiplier has handled this divide. */
//				tValOutDfl[63:0]	= regValMulRes[63:0];
//				tDoOutDfl			= 1;

				tValOutHeld[63:0]	= regValMulRes[63:0];
				tDoOutHeld			= 1;
			end
		end
`endif

		JX2_UCMD_FLDCX: begin
		end
		JX2_UCMD_FSTCX: begin
		end
		JX2_UCMD_FCMP: begin
		end

`ifdef jx2_alu_jcmp
		JX2_UCMD_JCMP: begin
		end
`endif

`ifdef jx2_alu_jcmpz
		JX2_UCMD_JCMPZ: begin
		end
`endif

		JX2_UCMD_MULW3: begin
		end

		JX2_UCMD_SHAD3: begin
		end
`ifndef jx2_merge_shadq
		JX2_UCMD_SHLD3: begin
		end
		JX2_UCMD_SHADQ3: begin
		end
		JX2_UCMD_SHLDQ3: begin
		end
`endif
		
		JX2_UCMD_CONV_RR: begin
		end
		JX2_UCMD_CONV2_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end

		JX2_UCMD_FPU3: begin
		end

		JX2_UCMD_BLINT: begin
		end

		JX2_UCMD_FPUV4SF: begin
//			tRegIdRn2				= regIdRm;
//			tRegValRn2[63:0]		= regFpuV4GRn;
//			tValOutDfl[63:0]		= regFpuV4GRn;
			tValOutHeld[63:0]		= regFpuV4GRn;
//			tDoOutDfl				= 1;
//			tRegHeld				= 1;
			tDoOutHeld				= 1;

//			if(regIdRm[6])
//				tDoOutHeld	= 0;
			
//			$display("(A): Rs=%X Rt=%X Rn=%X Ixt=%X",
//				regValRs, regValRt, regFpuV4GRn, opUIxt);
		end

		default: begin
			if(!tMsgLatch)
				$display("EX3: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase

`ifndef def_true
	if(tDoOutHeld)
	begin
		tRegIdRn2		= regIdRm;
		tRegValRn2		= UVGPRV_00;
		tRegIdRn4		= regIdRm;
//		tRegValRn4		= tValOutDfl;
		tRegValRn4		= tValOutHeld;
		tRegHeld		= 1;
	end
`else
	if(tDoOutHeld)
	begin
		tRegIdRn2		= regIdRm;
		tRegValRn2		= tValOutHeld;
		tRegIdRn4		= regIdRm;
		tRegValRn4		= tValOutHeld;
	end
`endif
	
	if(tDoOutDfl)
	begin
		tRegIdRn2		= regIdRm;
		tRegValRn2		= tValOutDfl;
		tRegIdRn4		= regIdRm;
		tRegValRn4		= tValOutDfl;
	end
	
	if(tDoMemOp)
	begin
`ifdef jx2_debug_ldst
		$display("EX3: DoMemOp, OK=%X", memDataOK);
`endif

		if(memDataOK[1])
		begin
			if(memDataOK[0])
			begin
				if(!tMsgLatch)
					$display("EX3: Memory Fault");
				tNextMsgLatch	= 1;
			end
			else
			begin
// `ifdef jx2_debug_ldst
`ifdef def_true
//				if(!exIsHold)
//				begin
//					$display("EX3: Memory Hold");
//				end
`endif
			end
`ifndef	jx2_mem_l1dstall
//			tExHold=1;
`endif
		end
		else
		begin
`ifdef jx2_debug_ldst
			if(!memDataOK[0])
			begin
				$display("EX3: Memory Ready");
			end
`endif
		end

//		tMemOpm = tDoMemOpm;
	end
	
	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
//		tRegIdCn2	= JX2_CR_ZZR[4:0];
		tRegIdCn2	= JX2_CR_ZZR;
		tRegIdRn4	= JX2_CR_ZZR;
	end

	if(tHoldCyc < tDoHoldCyc)
	begin
//		if(!tExHold)
//			$display("Hold Cyc %d %d", tHoldCyc, tDoHoldCyc);
//		tExHold=1;
	end

	if(reset)
	begin
		tExHold		= 0;
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

	if(tExHold)
		tHoldCyc <= tHoldCyc + ((tHoldCyc!=15)?1:0);
	else
		tHoldCyc <= 0;
end

endmodule

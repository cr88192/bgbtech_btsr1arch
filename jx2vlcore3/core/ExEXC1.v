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
EX1 Stage (Top Level, Lane 2+)

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [5:0]=Depends on opcode
 */

`include "CoreDefs.v"

// `include "ExConv2R.v"

`ifndef jx2_shadq_nolane3

`ifdef jx2_merge_shadfn
`include "ExShad64D.v"
`endif

`ifndef jx2_merge_shadfn
`ifndef jx2_merge_shadq
`include "ExShad32B.v"
`include "ExShad64B.v"
`endif

`ifdef jx2_merge_shadq
`include "ExShad64C.v"
`endif
`endif

`endif

module ExEXC1(
	clock, reset,
	opUCmd, opUIxt,
	exHold,	opUCmdOut,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regValXs,		//Source A, 128-bit

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	heldIdRn1,		//Held Destination ID (EX1)
	
	exDelayOut,

	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	opBraFlush,
	opPredNoExec,
	regInSr,
	idLane
	);


input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;
output[8:0]		opUCmdOut;
input [1:0]		idLane;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
`input_gprval	regValRs;		//Source A Value
`input_gprval	regValRt;		//Source B Value
`input_gprval	regValRm;		//Source C Value

`input_gprval	regValXs;		//Source C Value

`output_gpr		regIdRn1;		//Destination ID (EX1)
`output_gprval	regValRn1;		//Destination Value (EX1)
`output_gpr		heldIdRn1;		//Held Destination ID (EX1)

`output_gprval	exDelayOut;

input[32:0]		regValImm;		//Immediate (Decode)
input[47:0]		regValPc;
input			opBraFlush;
input			opPredNoExec;

input[63:0]		regInSr;


`reg_gpr		tRegIdRn1;		//Destination ID (EX1)
`reg_gprval		tRegValRn1;		//Destination Value (EX1)
`reg_gpr		tHeldIdRn1;		//Destination ID (EX1)

assign	regIdRn1	= tRegIdRn1;		//Destination ID (EX1)
assign	regValRn1	= tRegValRn1;		//Destination Value (EX1)
assign	heldIdRn1	= tHeldIdRn1;		//Held Destination ID (EX1)

`reg_gprval	tExDelayOut;
assign	exDelayOut = tExDelayOut;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };


wire[63:0]	tValCnv;
wire		tCnvSrT;
ExConv2R	exConv2R(regValRs[63:0], opUIxt, regInSr[0], tValCnv, tCnvSrT);

`ifndef jx2_shadq_nolane3


`ifdef jx2_merge_shadfn

wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64D	exShad64(clock, reset,
	regValRs[63:0],
	regValXs[63:0],
	regValRt[7:0],
	tValShad64,
	opUIxt[5:0], idLane);

`endif


`ifndef jx2_merge_shadfn

`ifndef jx2_merge_shadq
wire[31:0]	tValShad32;
//ExShad32	exShad32(clock, reset,
ExShad32B	exShad32(clock, reset,
	regValRs[31:0], regValRt[7:0],
	tValShad32, opUCmd[0]);

wire[63:0]	tValShad64;
//ExShad64	exShad64(clock, reset,
ExShad64B	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
	tValShad64, opUCmd[0]);
`endif

`ifdef jx2_merge_shadq
wire[63:0]	tValShad64;
wire[31:0]	tValShad32;
assign	tValShad32 = tValShad64[31:0];

ExShad64C	exShad64(clock, reset,
	regValRs[63:0], regValRt[7:0],
//	tValShad64, opUCmd[1:0]);
//	tValShad64, {opUIxt[3:2], opUCmd[1:0]});
	tValShad64, opUIxt[3:0]);
`endif

`endif

`endif

reg			tOpEnable;

(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;
reg[8:0]	tOpUCmd2;

assign		opUCmdOut = tOpUCmd2;

`reg_gprval	tValOutDfl;
reg			tDoOutDfl;

reg tMsgLatch;
reg tNextMsgLatch;
reg tSlotUSup;

always @*
begin

	tRegIdRn1		= JX2_GR_ZZR;		//Destination ID (EX1)
	tRegValRn1		= UVGPRV_00;		//Destination Value (EX1)
	tHeldIdRn1		= JX2_GR_ZZR;

	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;
	tSlotUSup		= 0;

	tValOutDfl		= UVGPRV_00;
	tDoOutDfl		= 0;
	tExDelayOut		= 0;

`ifdef jx2_enable_pred_s
	casez( { opBraFlush, opUCmd[8:6], regInSr[1:0] } )
		6'b0000zz: 	tOpEnable = 1;
		6'b0001zz: 	tOpEnable = 0;
		6'b0010z0: 	tOpEnable = 0;
		6'b0010z1: 	tOpEnable = 1;
		6'b0011z0: 	tOpEnable = 1;
		6'b0011z1: 	tOpEnable = 0;
		6'b01000z: 	tOpEnable = 0;
		6'b01001z: 	tOpEnable = 1;
		6'b01010z: 	tOpEnable = 1;
		6'b01011z: 	tOpEnable = 0;
		6'b0110zz: 	tOpEnable = 1;
		6'b0111zz: 	tOpEnable = 1;
		6'b1zzzzz: 	tOpEnable = 0;
	endcase
`else
	casez( { opBraFlush, opUCmd[7:6], regInSr[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif

`ifdef jx2_cpu_pred_id2
	tOpEnable = !opBraFlush && !opPredNoExec;
`endif

	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;
	tOpUCmd2	= { JX2_IXC_AL, tOpUCmd1 };

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_INVOP: begin
			if(!tMsgLatch)
				$display("EX: Invalid Opcode");
			tNextMsgLatch	= 1;
			tExHold		= 1;
//			tExHold		= !reset;
		end
	
		JX2_UCMD_LEA_MR: begin
//			tSlotUSup		= 0;
			tSlotUSup		= 1;
		end

		JX2_UCMD_MOV_RM: begin
//			tRegValRn1		= regValRm;		/* MOV.X bits */
		end
		JX2_UCMD_MOV_MR: begin
			tSlotUSup		= 1;
//			if(opUIxt[8:6]==JX2_IUC_WX)
//				tSlotUSup	= 0;
//			tRegHeld		= 1;
		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
			tRegHeld		= 1;
		end

		JX2_UCMD_ALUCMP: begin
			tSlotUSup		= 1;
		end
	
		JX2_UCMD_CONV_RR: begin
`ifdef jx2_cpu_conv_ex2
			tExDelayOut		= tValCnv;
			tRegHeld		= 1;
`else
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= tValCnv;

			tValOutDfl[63:0]		= tValCnv;
			tDoOutDfl		= 1;
`endif
		end

`ifdef jx2_gprs_mergecm
		JX2_UCMD_MOV_RC, JX2_UCMD_MOV_CR: begin
//			tRegIdRn1		= regIdRm;
//			tRegValRn1		= regValRs;

			tValOutDfl		= regValRs;
			tDoOutDfl		= 1;
		end
`endif

		JX2_UCMD_MOV_IR: begin
			tValOutDfl		= regValRt;
			tDoOutDfl		= 1;

			case(opUIxt[3:0])
				4'b0000: begin /* LDIx */
				end
				4'b0001: begin /* LDISH8 */
					tValOutDfl[63:0]	= { regValRs[55:0], regValRt[7:0] };
				end
				4'b0010: begin /* LDISH16 */
					tValOutDfl[63:0]	= { regValRs[47:0], regValRt[15:0] };
				end
				4'b0011: begin /* LDISH32 */
					tValOutDfl[63:0]	= { regValRs[31:0], regValRt[31:0] };

				end

				4'b0101: begin /* FLDCH */
					tValOutDfl[63:0]	= {
						regValRt[15:14],
						(regValRt[14] || (regValRt[14:10]==0)) &&
								(regValRt[14:10]!=5'h1F) ?
							6'h00 : 6'h3F,
						regValRt[13: 0],
						10'h0,
						32'h0
						};
				end

				default: begin
					$display("ExEXB1: MOV_IR, Invalid UIxt %X", opUIxt);
				end
			endcase

`ifdef def_true
			tExDelayOut		= tValOutDfl;
			tRegHeld		= 1;

			tValOutDfl		= UVGPRV_00;
			tDoOutDfl		= 0;
`endif
		end
		
		JX2_UCMD_MULW3: begin
			tRegHeld		= 1;
		end

`ifndef jx2_shadq_nolane3

`ifdef jx2_merge_shadq

//		JX2_UCMD_SHAD3, JX2_UCMD_SHLD3,
//		JX2_UCMD_SHADQ3, JX2_UCMD_SHLDQ3:
		JX2_UCMD_SHAD3:
		begin
`ifndef jx2_shadq_nolane3

`ifdef jx2_cpu_shad_ex2
			tExDelayOut[63:0]	= tValShad64;
			tRegHeld			= 1;
`else
			tValOutDfl[63:0]	= tValShad64;
			tDoOutDfl			= 1;
`endif

`endif
		end

`else
		JX2_UCMD_SHAD3: begin
			tValOutDfl		= { tValShad32[31]?UV32_FF:UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLD3: begin
			tValOutDfl		= { UV32_00, tValShad32 };
			tDoOutDfl		= 1;
		end
	
		JX2_UCMD_SHADQ3: begin
			tValOutDfl		= tValShad64;
			tDoOutDfl		= 1;
		end
		JX2_UCMD_SHLDQ3: begin
			tValOutDfl		= tValShad64;
			tDoOutDfl		= 1;
		end
`endif

`endif

		JX2_UCMD_MUL3: begin
			tSlotUSup		= 1;
		end

		JX2_UCMD_FPU3: begin
			tRegHeld		= 1;
		end

		JX2_UCMD_BLINT: begin
			tRegHeld		= 1;
		end

		JX2_UCMD_OP_IXS: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXS_NOP: begin
				end
				JX2_UCIX_IXS_MOVT: begin
					tValOutDfl[63:0]	= {UV63_00, regInSr[0]};
					tDoOutDfl			= 1;
				end
				JX2_UCIX_IXS_MOVNT: begin
					tValOutDfl[63:0]	= {UV63_00, !regInSr[0]};
					tDoOutDfl			= 1;
				end
				default: begin
					if(!tMsgLatch)
						$display("EX1B: Unhandled Op-IXS %X", opUIxt);
					tNextMsgLatch	= 1;
					tExHold		= 1;
				end
			endcase
		end

		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end
				JX2_UCIX_IXT_SLEEP: begin
				end
				JX2_UCIX_IXT_BREAK: begin
					if(!tMsgLatch)
						$display("EX1B: BREAK");
					tNextMsgLatch	= 1;
//					tExHold		= 1;
				end

				JX2_UCIX_IXT_SVEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKRR: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEKEY: begin
					tRegHeld		= 1;
				end
				JX2_UCIX_IXT_LDEENC: begin
					tRegHeld		= 1;
				end

				default: begin
					if(!tMsgLatch)
						$display("EX1B: Unhandled Op-IXT %X", opUIxt);
					tNextMsgLatch	= 1;
//					tExHold		= 1;
//					tExHold		= !reset;
				end
			endcase
		end

		default: begin
			if(!tMsgLatch)
				$display("EX1C: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
			tExHold		= 1;
//			tExHold		= !reset;
		end
	
	endcase

	if(tDoOutDfl)
	begin
		tRegIdRn1		= regIdRm;
		tRegValRn1		= tValOutDfl;
	end
	
	if(opBraFlush)
	begin
		tRegIdRn1	= JX2_GR_ZZR;
	end

	if(tRegHeld)
		tHeldIdRn1	= regIdRm;

//	if(tHeldIdRn1 != JX2_GR_ZZR)
//		tRegHeld		= 1;

	if(tSlotUSup)
	begin
		if(!tMsgLatch)
		begin
			$display("EX1B: Unsupported Operation In Lane, %X-%X",
				opUCmd, opUIxt);
		end
		tNextMsgLatch	= 1;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
